#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

U16 channel=3;  //4 channels
U16 range=AD_B_5_V;
char *file_name="9116d";
U32 read_count=4000;
U16 adinput;

main()
{
    I16 card, err, card_num;

    printf("This program inputs %d data from CH-0 to CH-%d of PCI-9116 in %d Hz, and\nstore data to file '%s'.\nPlease press any key to start the operation.\n", read_count, channel, (int)100000, file_name);
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_9116, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
	AI_9116_Config(card,P9116_AI_BiPolar|P9116_AI_SingEnded,P9116_AI_DMA,0,0,0);
	AI_9116_CounterInterval (card, 240*(channel+1), 240);
    err = AI_ContScanChannelsToFile(card, channel, range, file_name, read_count/(channel+1), (F32)1, SYNCH_OP);
    if (err!=0) {
       printf("AI_ContScanChannels error=%d", err);
       exit(1);
    }
    Release_Card(card);
    printf("\n\nThe input data is already stored in file '%s.dat'.\n", file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
