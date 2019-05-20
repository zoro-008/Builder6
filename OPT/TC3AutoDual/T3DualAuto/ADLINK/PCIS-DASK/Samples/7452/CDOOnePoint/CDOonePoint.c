#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

void main(void)
{
    I16 card, err, card_num=0;
	U32 out_value = 0;

    printf("This program outputs data to Port 0/1/2/3.\n");
    printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card = Register_Card (PCI_7452, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    do{
        system( "cls" );
        printf ("-------------------------------------------------\n");
  		err = DO_WritePort(card, 0, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P0 output = 0x%x\n", out_value);
  		err = DO_WritePort(card, 1, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P1 output = 0x%x\n", out_value);
  		err = DO_WritePort(card, 2, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P2 output = 0x%x\n", out_value);
  		err = DO_WritePort(card, 3, out_value);
		if (err!=NoError) {
			printf("DO_WritePort error=%d", err);
			exit(1);
		}
  	    printf(" P3 output = 0x%x\n", out_value);
        printf ("-------------------------------------------------\n");
		printf ("\n Press any key to stop...\n");
		Sleep(200);
		if(out_value < 0xffffffff)
			out_value = (out_value + 1);
        else
			out_value = 0;
	} while(!kbhit());

	Release_Card(card);
}
