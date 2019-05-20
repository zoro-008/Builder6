#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

#define CardNum   12
#define DataSize  2048
#define sample_rate  100000

U16 channel=0;  //1 channels
U16 range=AD_B_5_V;
U32 read_count = DataSize, count = 0, count1 = 0;
char *file_name="AIData";

U16 CardType[CardNum] = {PCI_9111DG, PCI_9111HR, PCI_9112, PCI_9113, PCI_9114DG, PCI_9114HG, PCI_9116, PCI_9118DG, PCI_9118HG, PCI_9118HR, PCI_9810, PCI_9812};

void exch_data(U16 *tbuf, U32 cnt);

main()
{
    I16 card, err, card_num=0, card_type, i;
    BOOLEAN halfReady, fStop;
    U16 t=0;
	U16 sts=0;

    printf("--------------------------------------------\n");
    printf("0. PCI_9111DG\n1. PCI_9111HR\n2. PCI_9112\n3. PCI_9113\n4. PCI_9114DG\n5. PCI_9114HG\n6. PCI_9116\n7. PCI_9118DG\n8. PCI_9118HG\n9. PCI_9118HR\n10. PCI_9810\n11. PCI_9812\n");
    printf("--------------------------------------------\n");
    printf("Please select a card type: ");
    scanf(" %d", &card_type);
    card_type = CardType[card_type];
	printf("Please input a card number: ");
    scanf(" %d", &card_num);	
    if ((card=Register_Card (card_type, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }	
	switch(card_type) {
	case PCI_9111DG:
	case PCI_9111HR:
       err=AI_9111_Config(card,TRIG_INT_PACER, 0, 0);
	   break;
    case PCI_9112:
	   err = AI_9112_Config(card, TRIG_INT_PACER);
	   break;
    case PCI_9113:
	   err = AI_9113_Config(card, TRIG_INT_PACER);
	   break;
    case PCI_9114DG:
    case PCI_9114HG:
	   err = AI_9114_Config(card, TRIG_INT_PACER);
	   break;
    case PCI_9116:
	   err = AI_9116_Config(card,0,P9116_AI_DMA,0,0,0);
	   break;
    case PCI_9118DG:
    case PCI_9118HG:
    case PCI_9118HR:
       err = AI_9118_Config(card,P9118_AI_BiPolar|P9118_AI_SingEnded,0,0,0);
	   break;
    case PCI_9810:
    case PCI_9812:
	   err = AI_9812_Config(card,P9812_TRGMOD_SOFT, P9812_TRGSRC_CH0, P9812_TRGSLP_POS, P9812_AD2_GT_PCI|P9812_CLKSRC_INT,0x80,0);
	   break;
	}
    if (err!=0) {
       printf("AI_9xxx_Config error=%d", err);
       exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 1);
    if (err!=0) {
       printf("AI_DblBufferMode error=%d", err);
       exit(1);
    }
	if(card_type == PCI_9116)
		AI_9116_CounterInterval (card, 12000, 12000);
    err = AI_ContScanChannelsToFile(card, channel, range, file_name, read_count, (F64) sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("AI_ContReadChannelToFile error=%d", err);
       exit(1);
    }
    printf("\n\nPress any key to stop input operation.\n\n");
	do {
		 //use AI_AsyncDblBufferHalfReady
		do {
           AI_AsyncDblBufferHalfReady(card, &halfReady, &fStop);
		} while (!halfReady);
	    AI_AsyncDblBufferTransfer(card, NULL);  //transfer to file
        count += (read_count / 2);
		AI_AsyncDblBufferOverrun(card, 0, &sts);
		if(sts) {
			t++;
			AI_AsyncDblBufferOverrun(card, 1, &sts);
		}
		printf("\roverrun:%d, count: %d", t, count);
    } while(!kbhit());
	AI_AsyncClear(card, &count1);
	count += count1;
    Release_Card(card);
    printf("\n\n%d input data are stored in file '%s.dat'.\n", count, file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
