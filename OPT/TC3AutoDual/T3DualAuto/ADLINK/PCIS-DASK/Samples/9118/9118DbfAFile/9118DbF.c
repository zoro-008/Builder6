#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

U16 channel=3;  //4 channels
U16 range=AD_B_5_V;
char *file_name="9118d";
U32 read_count=2000;
F64 sample_rate=2000;
U32 postCount = 1024;

main()
{
    I16 card, err, card_num;
    BOOLEAN halfReady, fStop;
    U32 count=0, count1;

    printf("This program inputs data from CH-0 to CH-%d of PCI-9118 in %d Hz by\ndouble-buffer mode, and store data to file '%s.dat'. The size of circular\nbuffer is %d. It will not stop until you press a key.\n\nPress any key to start the operation.\n",
            channel, (int)sample_rate, file_name, read_count);
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_9118DG, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    err = AI_9118_Config(card, P9118_AI_BiPolar|P9118_AI_SingEnded, P9118_AI_DtrgPositive|P9118_AI_EtrgPositive|P9118_AI_AboutTrgEn, 0, postCount);
    if (err!=0) {
       printf("AI_9118_Config error=%d", err);
       exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 1);  //double-buffer mode
    if (err!=0) {
       printf("AI_DblBufferMode error=%d", err);
       exit(1);
    }
    err = AI_ContScanChannelsToFile(card, channel, range, file_name, read_count, sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("AI_ContReadChannel error=%d", err);
       exit(1);
    }
    printf("\n\nPress any key to stop input operation.");
    printf("\n\nData count : \n");
    do {
        do {
             AI_AsyncDblBufferHalfReady(card, &halfReady, &fStop);
        } while (!halfReady && !fStop);
        if(halfReady) {
			AI_AsyncDblBufferTransfer(card, NULL);  //transfer to file
	        count += (read_count / 2);
            printf("%d\r", count);
		}
    } while(!kbhit() && !fStop);
    AI_AsyncClear(card, &count1);
	count += count1;
    printf("%d\r", count);
    Release_Card(card);
    printf("\n\n%d input data are stored in file '%s.dat'.\n", count, file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
