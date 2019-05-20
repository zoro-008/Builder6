#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define ScanIntv	160
#define SampIntv	160
#define DataCount	4000
#define ChnNum		0
#define	TimeBase	40000

void main()
{

	FILE *svfile;
	int i;
	I16 err, card;
	U16 card_num;
	F64 Volt_Val;

	U16 buffer[DataCount];
	U16 bufferID;
	svfile = fopen("9221d.txt", "w");

	printf("This sample converts AI Voltage from channel %d in %6.2f KHz sampling rate.\n", ChnNum, (F32)TimeBase/SampIntv );
	printf("This sample is differential mode and external digital trigger mode.\n");
	printf("please connect external digital trigger to GPI0(pin8).\n");	
		
	printf("\nPlease Input Card Number:");
	scanf(" %hd", &card_num);
	if ((card = Register_Card(PCI_9221, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	err = AI_9221_Config (card, P9221_AI_Differential|P9221_AI_IntTimeBase, P9221_TRGMOD_ExtD|P9221_TRGSRC_GPI0, 0);
	if(err <0){
		printf("AI_9221_CounterInterval err: %d\n", err);
		Release_Card(card);
		exit(1);
	}

	err = AI_9221_CounterInterval (card, ScanIntv, SampIntv);
	if(err <0){
		printf("AI_9221_CounterInterval err: %d\n", err);
		Release_Card(card);
		exit(1);
	}
	
	err = AI_SetTimeOut (card, 60000);
	if(err <0){
		printf("AI_SetTimeOut err: %d\n", err);
		Release_Card(card);
		exit(1);
	}
	
	err = AI_ContBufferSetup (card, buffer, DataCount, &bufferID);
	if(err <0){
		printf("AI_ContBufferSetup err: %d\n", err);
		Release_Card(card);
		exit(1);
	}

	printf("\npress any key to start.\n");
	getch();
	err = AI_ContReadChannel (card, ChnNum, AD_B_5_V, &bufferID, DataCount, 0, SYNCH_OP);
	if(err <0){
		printf("AI_ContReadChannel err: %d\n", err);
		AI_ContBufferReset(card);
		Release_Card(card);
		exit(1);
	}

	for (i=0; i<DataCount; i++) {
		fprintf(svfile, "%4.4f,",((float)((I16)(buffer[i])))*5/32768);
		fprintf(svfile, "\n");
	}

	err = AI_ContBufferReset (card);
	if(err <0){
		printf("AI_ContBufferSetup err: %d\n", err);
		exit(1);
	}
	
	printf("\nAcquisition Done...\n");
	printf("Press any key to exit.\n");
	getch();

	Release_Card(card);
	fclose(svfile);
	return;
}