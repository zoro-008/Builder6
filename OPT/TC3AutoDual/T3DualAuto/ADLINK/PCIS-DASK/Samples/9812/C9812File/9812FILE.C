#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

U16 channel=3;  //4 channels
U16 range=AD_B_1_V;
char *file_name="9812d";
U32 read_count=400000;
F64 sample_rate=10000000;

main()
{
    I16 card, err, card_num;

    printf("This program inputs %d data from CH-0 to CH-%d of PCI-9812 in %d Hz, and\nstore data to file '%s.dat'.\nPlease press any key to start the operation.\n", read_count, channel, (int)sample_rate, file_name);
    printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_9812, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    err = AI_9812_Config(card, P9812_TRGMOD_SOFT, P9812_TRGSRC_CH0, P9812_TRGSLP_POS, P9812_AD2_GT_PCI|P9812_CLKSRC_INT, 0x80, 0);
    if (err!=0) {
       printf("AI_9812_Config error=%d", err);
       exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 0);
    if (err!=0) {
       printf("AI_DblBufferMode error=%d", err);
       exit(1);
    }
    err = AI_ContScanChannelsToFile(card, channel, range, file_name, read_count, sample_rate, SYNCH_OP);
    if (err!=0) {
       printf("AI_ContReadChannel error=%d", err);
       exit(1);
    }
    Release_Card(card);
    printf("\n\nThe input data is already stored in file '%s.dat'.\n", file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
