#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

char *file_name="7200d";
U32 DoBuf[2000];
U32 read_count=2000;
F64 sample_rate=1000;

main()
{
    I16 card, err, card_num;
    U32 i;
    BOOLEAN halfReady;
    U32 count=0, count1;

    printf("This program inputs %d data by PCI-7200 in %d Hz by double-buffer mode, and store data to file '%s.dat'. It will not stop until you press a key.\n\nPress any key to start the operation.\n",
            read_count, (int)sample_rate, file_name);
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7200, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    for (i=0; i<read_count; i++)
      //DoBuf[i] = (U32)(sin((double)i/500)*0x7fff)+0x8000;
        DoBuf[i] = i;
    err = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING);
    if (err!=0) {
       printf("DI_7200_Config error=%d", err);
       exit(1);
    }
    err = DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW);
    if (err!=0) {
       printf("DO_7200_Config error=%d", err);
       exit(1);
    }
    err = DI_AsyncDblBufferMode(card, 1);
    if (err!=0) {
       printf("DI_DblBufferMode error=%d", err);
       exit(1);
    }
    err = DO_ContWritePort(card, 0, DoBuf, read_count, 0, (F64)sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("DO_ContWritePort error=%d", err);
    }
    err = DI_ContReadPortToFile(card, 0, file_name, read_count, sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
    }
    printf("\n\nPress any key to stop input operation.");
    printf("\n\nData count : \n");
    do {
        do {
             DI_AsyncDblBufferHalfReady(card, &halfReady);
        } while (!halfReady);
        DI_AsyncDblBufferTransfer(card, NULL);  //transfer to file
        count += (read_count / 2);
        printf("%d\r", count);
    } while(!kbhit());
    DI_AsyncClear(card, &count);
    DO_AsyncClear(card, &count1);
    Release_Card(card);
    printf("\n\n%d input data are stored in file '%s.dat'.\n", count, file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
