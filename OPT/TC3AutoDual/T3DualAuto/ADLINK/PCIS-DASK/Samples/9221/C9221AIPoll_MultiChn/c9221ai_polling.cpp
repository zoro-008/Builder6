#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define	ChnCount	8
#define	ScanCount	500

void main()
{
	FILE *svfile;
	int i;
	I16 err, card;
	U16 card_num;
	F64 Volt_Val;

	U16 buffer[ChnCount*ScanCount];
	U16 Chans[ChnCount] = {0, 1, 2, 3, 4, 5, 6, 7};
	U16 AdRanges[ChnCount] = {AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V, AD_B_5_V};

	svfile = fopen("9221d.txt", "w");

	printf("This sample performs %d AI scan, and scans %d channels per scan\n", ScanCount, ChnCount);
	printf("And the scaned data will be stored in 9221d.txt.\n");

	printf("\nPlease input card number:");
	scanf(" %hd", &card_num);
	if ((card = Register_Card(PCI_9221, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

	memset(buffer, '\0', ChnCount*ScanCount*2);
	AO_VWriteChannel(card, 0, -5.0);
	AO_VWriteChannel(card, 1, +3.0);
	printf("\nPress any key to start.\n");
	getch();

	for(i=0; i<ScanCount; i++){
		err = AI_ReadMultiChannels (card, ChnCount, Chans, AdRanges, buffer+(i*ChnCount));
		if(err <0){
			printf("AI_ReadMultiChannels err: %d\n", err);
			Release_Card(card);
			exit(1);
		}
	}

	for(i=0; i<ChnCount-1; i++)
		fprintf(svfile, "Channel %d\t\t", Chans[i]);
	fprintf(svfile, "Channel %d\n", Chans[ChnCount-1]);

	for (i=0; i<ChnCount*ScanCount; i++) {
		 AI_VoltScale (card, AD_B_5_V, *(buffer+i), &Volt_Val);
		 fprintf(svfile, "%4.4f\t\t", Volt_Val);
		 if(i%ChnCount == (ChnCount-1))
			fprintf(svfile,"\n");
	 }
	 printf("\nRead Multiple AI Channel Finish...\n");

	Release_Card(card);
	fclose(svfile);
	return;
}