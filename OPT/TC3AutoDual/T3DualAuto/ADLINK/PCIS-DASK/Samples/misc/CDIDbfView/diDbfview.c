#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

#define DataSize  2000
#define sample_rate  10000

U32 read_count=DataSize;
U32 viewAddr, MemSize=0, Db_buf[2], *InBuf;

main()
{
    I16 card, err, card_num;
    U32 i;
    BOOLEAN halfReady;
    U32 count=0, count1;
    U16 buffer_idx=0;

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7200, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
	//here to get the starting addresses of two driver DB buffers 
    err = DI_GetView(card, &viewAddr); //viewAddr : driver buffer
    if (err!=0) {
       printf("DI_GetView error=%d", err);
       exit(1);
    }
	err = DI_InitialMemoryAllocated (card, &MemSize);//get driver buffer size
    if (err!=0) {
       printf("DI_InitialMemoryAllocated error=%d", err);
       exit(1);
    }

	Db_buf[0] = viewAddr; //1st DB buffer address
	Db_buf[1] = viewAddr + (MemSize*1024/2); //2nd DB buffer address

    err = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING);
    if (err!=0) {
       printf("DI_7200_Config error=%d", err);
       exit(1);
    }
    err = DI_AsyncDblBufferMode(card, 1);
    if (err!=0) {
       printf("DI_DblBufferMode error=%d", err);
       exit(1);
    }
    err = DI_ContReadPort(card, 0, 0, read_count, (F64)sample_rate, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
    }
    printf("\n\nPress any key to stop input operation.");
	buffer_idx = 0;
	printf("\nscale and print out the first 4 data\n");
	printf("-------------------------------------\n");
    do {
        do {
             DI_AsyncDblBufferHalfReady(card, &halfReady);
        } while (!halfReady);
        //DI_AsyncDblBufferTransfer(card, NULL);  //this function is not needed
		//DB_buf[buffer_idx] : the ready buffer address
		InBuf = (U32 *) Db_buf[buffer_idx];
        //fetch and handle the data
        {
			#define ShowSize 4
			if(buffer_idx)
				printf("2nd buffer ready\n");
			else
				printf("1st buffer ready\n");
			for(i=0; i<ShowSize; i++)
				printf(" 0x%+8X  ", InBuf[i]);
			printf("\n");
		}
        buffer_idx = (buffer_idx+1)%2;
    } while(!kbhit());
    DI_AsyncClear(card, &count);
    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getch();
}
