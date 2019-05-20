#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

U16 DoBuf[20000];
char *file_name="7300d";
U32 read_count=20000;
main()
{
    I16 card, err, card_num;
    BOOLEAN bStopped;
    U32 count, i;

    printf("This program inputs and outputs %d data by PCI-7300A RevB in 10MHz\nsimultaneously (DI16DO16), and store input data to file '%s.dat'.\n\nPress any key to start the operation.\n", read_count, file_name);
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7300A_RevB, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    for (i=0; i<read_count; i++)
      //DoBuf[i] = (U32)(sin((double)i/500)*0x7fff)+0x8000;
        DoBuf[i] = (U16) i;
    err=DI_7300B_Config(card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
    if (err!=0) {
       printf("DI_7300B_Config error=%d\n", err);
       exit(1);
    }
    err=DO_7300B_Config (card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x40004000);
    if (err!=0) {
       printf("DO_7300B_Config error=%d\n", err);
       exit(1);
    }
    err = DI_ContReadPortToFile(card, 0, file_name, read_count, 1/*ignored*/, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
    }
    err = DO_ContWritePort(card, 0, DoBuf, read_count, 1, 1/*ignored*/, ASYNCH_OP);
    if (err!=0) {
       printf("DO_ContWritePort error=%d", err);
    }
    do {
         DO_AsyncCheck(card, &bStopped, &count);
    } while (!bStopped);
    do {
         DI_AsyncCheck(card, &bStopped, &count);
    } while (!bStopped);
    DI_AsyncClear(card, &count);

    DO_AsyncClear(card, &count);	
    Release_Card(card);
    printf("\n\nThe input data is already stored in file '%s.dat'.\n", file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
