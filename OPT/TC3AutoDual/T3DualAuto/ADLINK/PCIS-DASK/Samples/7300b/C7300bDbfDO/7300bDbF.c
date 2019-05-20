#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

#define PI  3.14159

U16 channel=0;
U32 out_count=4096;
F64 samp_rate = 50000;
U16 pattern1[4096];
U16 pattern2[4096];
U16 ao_buf[4096];
U16 ao_buf2[4096];

main()
{
    I16 card, err, card_num,i,Id,j;
    BOOLEAN halfReady, fStop;
    U32 count=0, count1, startPos;
  U16 viewidx;

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7300A_RevB, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

	for (i=0;i<out_count;i++) 
		pattern1[i] = (U16)(sin((double)i/512*PI)*0x7fff)+0x8000;
	for (i=0;i<out_count;i++) {
		//if(i<2048)
			pattern2[i] = ((i*16)%65536) & 0xffff;
		//else 
		//	pattern2[i] = (2048-((i-2047)%65536)) & 0xffff;
	}	
	memcpy(ao_buf, pattern1, out_count*2);
	memcpy(ao_buf2, pattern1, out_count*2);
	//memcpy(ao_buf2, pattern2, out_count*2);

    err=DO_7300B_Config (card, 16, TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x00040004);
        if (err !=0) {
           printf("DO_7300B_Config Error = %d, card=%d\n", err, card);
           exit(1);
        }
    err = DO_ContMultiBufferSetup (card, ao_buf, out_count, &Id);
    if (err!=0) {
       printf("DO_ContMultiBufferSetup error=%d for the 1st buffer", err);
       exit(1);
    }
    err = DO_ContMultiBufferSetup (card, ao_buf2, out_count, &Id);
    if (err!=0) {
       printf("DO_ContMultiBufferSetup error=%d for the 2nd buffer", err);
       exit(1);
    }
    err = DO_ContMultiBufferStart (card, 0, samp_rate);
    if (err!=0) {
       printf("DO_ContMultiBufferStart error=%d", err);
       exit(1);
    }
	j=0;
    printf("\n\nPress any key to stop input operation.");
    printf("\n\nData count : \n");
    do {
        do {
           DO_AsyncMultiBufferNextReady(card, &halfReady, &viewidx);
        } while (!halfReady);
		if(halfReady) {
		if(!(j%4))
			memcpy(ao_buf, pattern2, out_count*2);
		else if((j%4)==1)
			memcpy(ao_buf2, pattern2, out_count*2);
		else if((j%4)==2)
			memcpy(ao_buf, pattern1, out_count*2);
		else
			memcpy(ao_buf2, pattern1, out_count*2);
        count += (out_count);
        printf("%d %d\r", viewidx, count);
		j++;
		}
    } while(!kbhit());
    DO_AsyncClear(card, &count1);
    count += (count1);
    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getch();
}
