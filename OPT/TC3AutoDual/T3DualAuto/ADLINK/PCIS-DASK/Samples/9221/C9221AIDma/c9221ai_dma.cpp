#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define ChnCount	16

#define SampIntv	160
#define ScanIntv	SampIntv*ChnCount

#define DataCount	3200

#define TimeBase 40000

void main()
{
	FILE *svfile;
	int i, j;
	I16 err, card;
	U16 card_num;
	F64 Volt_Val;

	U16 buffer[DataCount];
	U16 bufferID;
	
	U16 Chans[ChnCount] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	U16 AdRanges[ChnCount] = {AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V};
	const char file[] = "9221d.txt";

	memset(buffer, '\0', DataCount*2);

	svfile = fopen(file, "w");

	printf("This sample performs DMA AI Acquisition from channel 0 to channel 15, and acquires %d AI Voltage Data in %4.2f KHz sampling rate.", DataCount, (F32)TimeBase/SampIntv);
	printf(" And Stores acquired data to %s.\n", file);

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

	err = AI_SetTimeOut(card, 10000);
	if(err <0){
		printf("AI_SetTimeOut err: %d\n", err);
		exit(1);
	}

	err = AI_ContBufferSetup (card, buffer, DataCount, &bufferID);
	if(err <0){
		printf("AI_ContBufferSetup err: %d\n", err);
		Release_Card(card);
		exit(1);
	}

	printf("\nPress any key to start.\n");
	getch();

	err = AI_ContReadMultiChannels (card, ChnCount, Chans, AdRanges, &bufferID, DataCount, 0, SYNCH_OP);
	if(err <0){
		printf("AI_ContReadMultiChannels err: %d\n", err);
		AI_ContBufferReset(card);
		Release_Card(card);
		exit(1);
	}

	printf("\nAcquisition Done.\n");
	
	for(i=0; i<ChnCount-1; i++)
		fprintf(svfile, "CH %d\t", i);
	fprintf(svfile, "CH %d\n", i);

	for(i=0; i<DataCount/ChnCount; i++) {
		for(j=0; j<ChnCount; j++){
			AI_VoltScale (card, AdRanges[j], buffer[i*ChnCount+j], &Volt_Val);
			fprintf(svfile, "%4.4f\t", Volt_Val);
			if(j%ChnCount == ChnCount-1)
				fprintf(svfile, "\n");
		}
	}

	printf("Press any key to exit.\n");
	getch();
	fclose(svfile);
	AI_ContBufferReset(card);
	Release_Card(card);
	
	return;
}