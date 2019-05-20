#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

#define DataSize  2000
#define sample_rate  10000

U32 read_count=DataSize;
U32 pView, *InBuf;

main()
{
    I16 card, err, card_num;
    BOOLEAN bStopped;
    U32 count, i, k;

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    if ((card=Register_Card (PCI_7200, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    err = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING);
    if (err!=0) {
       printf("DI_7200_Config error=%d", err);
       exit(1);
    }
    err = DI_AsyncDblBufferMode(card, 0);
    if (err!=0) {
       printf("DI_DblBufferMode error=%d", err);
       exit(1);
    }
    err = DI_GetView(card, &pView); //pView : driver buffer
    if (err!=0) {
       printf("DI_GetView error=%d", err);
       exit(1);
    }
	InBuf = (U32 *)pView;
    err = DI_ContReadPort(card, 0, 0, read_count, (F64)sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
    }
    do {
         DI_AsyncCheck(card, &bStopped, &count);
    } while (!bStopped);
    DI_AsyncClear(card, &count);

	printf(" >>>>>>>>>>>>>>> show data  <<<<<<<<<<<<<<< \n");
	for (k=0; k<read_count/80, k<10; k++) {
		for( i=0+k*80; i<80+k*80 ; i++ ){
			printf(" 0x%08X  ", InBuf[i]);
			if(i && ((i%4)==3))
				printf("\n");
		}
		if(k<(read_count/80-1) && k<9) {
			printf("Press any key for the continued data...\n");
			getch();
		}
	}
    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getch();
}
