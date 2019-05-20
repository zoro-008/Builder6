#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"


void main(int argc, char* argv[])
{
	I16 err, card, card_num;
	U16 rdata = 0; 
	U16	wdata = 0;


	printf("This sample output digital data to Line0 of DO port,\n");
	printf("and read digital data from Line0 of DI port.\n");
	printf("You can connect Line0 of DO port (pin3) and Line0 of DI port (pin8).\n");
	printf("\nPlease input a card number:");
	scanf(" %hd", &card_num);
	if ((card = Register_Card(PCI_9221, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	do{
		system("cls");
		wdata%=2;

		err = DO_WriteLine(card, 0, 0, wdata);
		if(err < 0){
			printf("DO_WriteLine err: %d\n", err);
			Release_Card(card);
			exit(1);
		}

		err = DI_ReadLine(card, 0, 0, &rdata);
		if(err < 0){
			printf("DI_ReadLine err: %d\n", err);
			Release_Card(card);
			exit(1);
		}
		printf("Read Value: %x\n", rdata);

		wdata++;
		Sleep(200);
	}while(!kbhit());	

	Release_Card(card);
	return;
}