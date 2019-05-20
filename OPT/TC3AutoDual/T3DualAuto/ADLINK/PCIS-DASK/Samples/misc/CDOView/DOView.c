#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "dask.h"

#define DataSize  2000
#define sample_rate  10000

U32 out_count=DataSize;
U32 pView, *DoBuf;

main()
{
    I16 card, err, card_num;
    BOOLEAN bStopped;
    U32 count, i, k;
    LARGE_INTEGER freq, start_count, current_count;
    double take_msec;

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    if ((card=Register_Card (PCI_7200, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    err = DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW);
    if (err!=0) {
       printf("DO_7200_Config error=%d", err);
       exit(1);
    }
    err = DO_GetView(card, &pView); //pView : driver buffer
    if (err!=0) {
       printf("DO_GetView error=%d", err);
       exit(1);
    }
	DoBuf = (U32 *)pView;
    for (i=0; i<out_count; i++)
      DoBuf[i] = (U32)(sin((double)i/500)*0x7fff)+0x8000;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start_count);
    err = DO_ContWritePort(card, 0, 0, out_count, 1, (F64)sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("DO_ContWritePort error=%d", err);
    }
    do {
         DO_AsyncCheck(card, &bStopped, &count);
    } while (!bStopped);
    DO_AsyncClear(card, &count);
    QueryPerformanceCounter(&current_count);
    take_msec =((double)(current_count.QuadPart-start_count.QuadPart))/freq.QuadPart*1000;
    printf("\n\nThe time taken for DMA operation is: %4.3f msec\n",take_msec);
    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getch();
}
