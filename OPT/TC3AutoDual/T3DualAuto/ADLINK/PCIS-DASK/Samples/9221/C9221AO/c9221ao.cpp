#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

void main(void)
{
	I16 err, card, card_num;
	U16 chn = 0;
	F32 Vot_Out;
	char bStop;

	printf("This sample output voltage to AO0.\n");

	printf("\nPlease input a card number:");
	scanf(" %hd", &card_num);
	if ((card = Register_Card(PCI_9221, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	do{
		printf("\nKey in Output Voltage(-5 ~ +5):");
		scanf("%f", &Vot_Out);
		if(Vot_Out>5.0||Vot_Out<-5.0){
			printf("Invalid Voltage Value\n");
			continue;
		}
		AO_VWriteChannel (card, chn, (F64)Vot_Out);
		printf("                    (Q)uit?");
		bStop = getch();
	}while((bStop!='q')&&(bStop!='Q'));

	Release_Card(card);
	return;
}