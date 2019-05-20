#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

void main(int argc, char* argv[])
{
	I16 err, card;
	U16 card_num;
	F64 Volt_Read;

	printf("This sample polls voltage value from AI0.\n");

	printf("\nPlease input card number:");
	scanf(" %hd", &card_num);
	if ((card = Register_Card(PCI_9221, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	printf("press any key to start.\n");
	getch();

	do{
		Sleep(100);
		system("cls");
		err = AI_VReadChannel (card, 0, AD_B_5_V, &Volt_Read);
		if(err <0){
			printf("AI_ReadChannel err: %d\n", err);
			Release_Card(card);
			exit(1);
		}
		printf("\nInput Voltage: %4.4f\n", Volt_Read);
		printf("\n              press any key to stop.\n");
	}while(!kbhit());

	Release_Card(card);
	return;
}