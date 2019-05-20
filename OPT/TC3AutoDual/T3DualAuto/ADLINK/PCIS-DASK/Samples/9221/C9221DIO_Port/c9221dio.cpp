#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"


void main(int argc, char* argv[])
{
	I16 err, card, card_num;
	U32 rdata = 0; 
	U32 wdata = 0;


	printf("This sample output digital data to DO Port,\n");
	printf("and read digital data from DI Port.\n");
	printf("For this sample, You can connect\n");
	printf(" Line0 of DO (pin3)  and Line0 of DI (pin8)\n");
	printf(" Line1 of DO (pin21) and Line1 of DI (pin26)\n");
	printf(" Line2 of DO (pin1)  and Line2 of DI (pin25)\n");
	printf(" Line3 of DO (pin20) and Line3 of DI (pi6)\n");
	printf(" Line0 of DO (pin3)  and Line4 of DI (pin24)\n");
	printf(" Line1 of DO (pin21) and Line5 of DI (pin5)\n");
	printf(" Line2 of DO (pin1)  and Line6 of DI (pin4)\n");
	printf(" Line3 of DO (pin20) and Line7 of DI (pin22)\n");

	printf("\nPlease input a card number:");
	scanf(" %hd", &card_num);
	if ((card = Register_Card(PCI_9221, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	do{
		system("cls");

		err = DO_WritePort(card, 0, (wdata)%0x10);
		if(err < 0){
			printf("DO_WritePort err: %d\n", err);
			Release_Card(card);
			exit(1);
		}

		err = DI_ReadPort(card, 0, &rdata);
		if(err < 0){
			printf("DO_WritePort err: %d\n", err);
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