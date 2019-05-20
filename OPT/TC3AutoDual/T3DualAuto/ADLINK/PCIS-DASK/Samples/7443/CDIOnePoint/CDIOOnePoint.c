#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

void main(void)
{

	I16 card, err, card_num=0;
	U32 out_value = 0;
	U32 in_value;

    printf("This program inputs data from DI Port 0/1/2/3\n");
	printf("Please input a card number: ");
    scanf(" %d", &card_num);

    if ((card=Register_Card (PCI_7443, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

   do{
        system( "cls" );

		printf ("-------------------------------------------------\n");

		err = DI_ReadPort(card,  P7443_CH0, &in_value);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P0 input = 0x%x\n", in_value);

		err = DI_ReadPort(card,  P7443_CH1, &in_value);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P1 input = 0x%x\n", in_value);

		err = DI_ReadPort(card,  P7443_CH2, &in_value);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P2 input = 0x%x\n", in_value);

		err = DI_ReadPort(card,  P7443_CH3, &in_value);
		if (err!=NoError) {
			printf("DI_ReadPort error=%d", err);
			exit(1);
		}
  	    printf(" P3 input = 0x%x\n", in_value);


		printf ("-------------------------------------------------\n");

		printf ("\n Press any key to stop...\n");
		Sleep(200);

	} while(!kbhit());

	Release_Card(card);
}
