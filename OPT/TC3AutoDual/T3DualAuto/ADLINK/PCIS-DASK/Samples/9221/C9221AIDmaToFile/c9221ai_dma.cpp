#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define ChnCount	15

#define SampIntv	160
#define ScanIntv	SampIntv*(ChnCount+1)

#define DataCount	3200

void main()
{
	int i, j;
	I16 err, card;
	U16 card_num;
	F64 Volt_Val;
	U8 file[] = "9221d";

	U16 buffer[DataCount];
	U16 bufferID;

	memset(buffer, '\0', DataCount*2);

	printf("This sample converts %d AI Voltage from AI Channel 0 to 15, and stores data to 9221d.dat file.\n", DataCount);

	printf("\nPlease input Card Number:");
	scanf(" %hd", &card_num);

	if ((card = Register_Card(PCI_9221, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	err = AI_9221_Config (card, P9221_AI_SingEnded|P9221_AI_IntTimeBase, P9221_TRGMOD_SOFT, 0);
	if(err <0){
		printf("AI_9221_CounterInterval err: %d\n", err);
		Release_Card(card);
		exit(1);
	}

	err = AI_9221_CounterInterval (card, ScanIntv, SampIntv);
	if(err <0){
		printf("AI_ContBufferSetup1 err: %d\n", err);
		exit(1);
	}

	err = AI_ContBufferSetup (card, buffer, DataCount, &bufferID);
	if(err <0){
		printf("AI_ContBufferSetup err: %d\n", err);
		Release_Card(card);
		exit(1);
	}

	printf("\nPlease any key to start.\n");
	getch();

	err = AI_ContScanChannelsToFile (card, ChnCount, AD_B_5_V, file, DataCount, 0, SYNCH_OP);
	if(err <0){
		printf("AI_ContScanChannelsToFile err: %d\n", err);
		AI_ContBufferReset(card);
		Release_Card(card);
		exit(1);
	}

	printf("\nAcqusition Done.\n");
	printf("Press any key to exit.\n");
	getch();

	AI_ContBufferReset(card);
	Release_Card(card);
	return;
}