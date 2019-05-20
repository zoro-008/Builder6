#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

void main(void)
{
	I16 card, err, card_num=0;
	U32 out_value = 1;
	U32 in_value;

    printf("This program outputs data to DO Port 0/1/2/3\n");
	printf("Please input a card number: ");
    scanf(" %d", &card_num);

    if ((card=Register_Card (PCI_7444, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

   do{
        system( "cls" );

        printf ("-------------------------------------------------\n");

  		err = DO_WritePort(card, P7444_CH0, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P0 output = 0x%x\n", out_value);

		err = DO_WritePort(card, P7444_CH1, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P1 output = 0x%x\n", out_value);

  		err = DO_WritePort(card, P7444_CH2, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P2 output = 0x%x\n", out_value);

		err = DO_WritePort(card, P7444_CH3, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P3 output = 0x%x\n", out_value);		
		

		printf ("-------------------------------------------------\n");


		err = DO_ReadPort (card, P7444_CH0, &in_value);
		if (err!=NoError) {
			printf("DO_ReadPort error=%d", err);
			exit(1);
		}
		printf(" P0 input = 0x%x\n", in_value);

		err = DO_ReadPort (card, P7444_CH1, &in_value);
		if (err!=NoError) {
			printf("DO_ReadPort error=%d", err);
			exit(1);
		}
		printf(" P1 input = 0x%x\n", in_value);

		err = DO_ReadPort (card, P7444_CH2, &in_value);
		if (err!=NoError) {
			printf("DO_ReadPort error=%d", err);
			exit(1);
		}
		printf(" P2 input = 0x%x\n", in_value);

		err = DO_ReadPort (card, P7444_CH3, &in_value);
		if (err!=NoError) {
			printf("DO_ReadPort error=%d", err);
			exit(1);
		}
		printf(" P3 input = 0x%x\n", in_value);

		Sleep(500);

		if(out_value < 0x80000000)
			out_value = out_value<<1;
        else
			out_value = 1;
	} while(!kbhit());

	Release_Card(card);
}
