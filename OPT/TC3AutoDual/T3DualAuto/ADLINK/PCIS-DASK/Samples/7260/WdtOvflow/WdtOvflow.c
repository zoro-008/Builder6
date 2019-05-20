#include <windows.h>
#include <stdio.h>
#include "dask.h"

void main()
{
  	I16 card=-1, err=0, card_num;
    U32 pattern= 0, waitMS=0, result=0;
	F32 rt = 0.0;
	U8 sts = 0;
    HANDLE hWDTEvent = NULL;
    U16 cardType=0, cardIndex=0;

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_7260, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
	err = IdentifyLED_Control (card, 1);
	//return 0;
	GetInitPattern (card, INIT_PTN, &pattern);
	printf("Init Pattern: %x\n", pattern);
	GetInitPattern (card, EMGSHDN_PTN, &pattern);
	printf("EMG Shutdown Pattern: %x\n", pattern);
    err = WDT_Setup (card, 0, 5.0, &rt, &hWDTEvent);
    if(err != NoError) {
		printf("WDT_Setup err %d!", err);
		Release_Card(card);
		exit(1);
	}	
	err = WDT_Control (card, 0, WDT_ARM);
    if(err != NoError) {
		printf("WDT_Control err %d!", err);
		Release_Card(card);
		exit(1);
	}
	printf("\nWDT will overflow after %.4f seconds ...\n", rt);
    waitMS = (U32) rt*1000*2;
	result = WaitForSingleObject(hWDTEvent, waitMS);
    if (result == WAIT_OBJECT_0) {
	  printf("\nWDT is overflowed !!\n");
	  EMGShutDownStatus (card, &sts);
	  if(sts) 
		  	EMGShutDownControl (card, EMGSHDN_RECOVERY);
	  Sleep(10);
	  DO_ReadPort (card, 0, &result);
	  printf("\nThe current pattern is : %x\n", result);
	  printf("\nPress any key to continue ...\n");
		err = IdentifyLED_Control (card, 0);
	  getch();
	} else if(result == WAIT_TIMEOUT) {
	   printf("\nWait Failed ...\n");
	} else
	   printf("\nWaitForSingleObject error %d ...\n", GetLastError());

    WDT_Control (card, 0, WDT_DISARM);
    Release_Card(card);
}