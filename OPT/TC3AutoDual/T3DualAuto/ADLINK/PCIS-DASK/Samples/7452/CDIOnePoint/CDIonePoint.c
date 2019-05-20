#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

void main(void)
{
    I16 card, err, card_num;
	U32 input = 0;

    printf("This program inputs data from Port 0/1/2/3.\n");
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7452, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    do{
        system( "cls" );
        printf ("-------------------------------------------------\n");
  		err = DI_ReadPort(card,  0, &input);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P0 input = 0x%x\n", input);
  		err = DI_ReadPort(card, 1, &input);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P1 input = 0x%x\n", input);
  		err = DI_ReadPort(card, 2, &input);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P2 input = 0x%x\n", input);
  		err = DI_ReadPort(card, 3, &input);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P3 input = 0x%x\n", input);
        printf ("-------------------------------------------------\n");
		printf ("\n Press any key to stop...\n");
		Sleep(200);
	} while(!kbhit());

	Release_Card(card);
}
