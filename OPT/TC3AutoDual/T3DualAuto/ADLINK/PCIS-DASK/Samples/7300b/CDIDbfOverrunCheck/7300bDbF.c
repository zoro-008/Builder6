#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

#define PI  3.14159

U16 channel=0;
U32 out_count=10000;
U16 ao_buf[10000];
U16 ao_buf2[10000];
U32 cb=0;
U16 viewidx[10000];
I16 card, err, card_num,i,Id,j;
BOOLEAN halfReady, fStop;
U32 t=0;
U16 sts=0;

void cbAddr() {
    DI_AsyncMultiBufferNextReady(card, &halfReady, &viewidx[(cb%10000)]);
    //TODO: here to handle data
    DI_AsyncDblBufferTransfer(card, NULL);  //notify driver the data have been handled
	DI_AsyncDblBufferOverrun(card, 0, &sts);
	if(sts) {
			t++;
			DI_AsyncDblBufferOverrun(card, 1, &sts);
	}
	printf("\roverrun:%d cb:%d", t, cb);

	cb++;
}

void cbAddr0() {
  printf("\n\ndone\n"); 
}

main()
{
    U32 count=0, count1, startPos;
    
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7300A_RevB, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    err = DI_7300B_Config(card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
    err = DI_ContMultiBufferSetup (card, ao_buf, out_count, &Id);
    if (err!=0) {
       printf("DO_ContMultiBufferSetup error=%d for the 1st buffer", err);
       exit(1);
    }
    err = DI_ContMultiBufferSetup (card, ao_buf2, out_count, &Id);
    if (err!=0) {
       printf("DO_ContMultiBufferSetup error=%d for the 2nd buffer", err);
       exit(1);
    }
	DI_EventCallBack (card, 1, 1, (U32) cbAddr );
	DI_EventCallBack (card, 1, 0, (U32) cbAddr0 );
    err = DI_ContMultiBufferStart (card, 0, 1);
    if (err!=0) {
       printf("DO_ContMultiBufferStart error=%d", err);
       exit(1);
    }
	j=0;
	Sleep(1000);
    DI_AsyncClear(card, &count1);
	Sleep(1);
    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getch();
}
