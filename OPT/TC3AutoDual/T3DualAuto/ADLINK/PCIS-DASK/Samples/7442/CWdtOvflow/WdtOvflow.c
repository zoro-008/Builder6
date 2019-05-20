#include <windows.h>
#include <stdio.h>
#include "dask.h"


void main()
{
  	I16 card=-1, err=0, card_num;
    U32 pattern = 0, pattern0 = 0, pattern1 = 0, waitMS=0, result=0;
	F32 rt = 0.0;
	U8 sts = 0;
    HANDLE hWDTEvent = NULL;
	HANDLE hWDTEvent2[2];
    U16 cardType=0, cardIndex=0;
	ULONG wdt_value=0;

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    
	if ((card=Register_Card (PCI_7442, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

	printf("Please input the WDT SafetyOut Pattern of CH0: ");
	scanf("%x",&pattern0);
	printf("Please input the WDT SafetyOut Pattern of CH1: ");
	scanf("%x",&pattern1);
		
	err = SetInitPattern (card, SAFTOUT_PTN_CH0, pattern0);
	if(err != NoError) {
		printf("setpattern err %d!", err);
		Release_Card(card);
		exit(1);
	}

	err = SetInitPattern (card, SAFTOUT_PTN_CH1, pattern1);
	if(err != NoError) {
		printf("setpattern err %d!", err);
		Release_Card(card);
		exit(1);
	}

	Sleep(500);

	GetInitPattern (card, SAFTOUT_PTN_CH0, &pattern);
	if(err != NoError) {
		printf("getpattern err %d!", err);
		Release_Card(card);
		exit(1);
	}
	printf("\nWDT SafetyOut Pattern of CH0: %x\n", pattern);

	GetInitPattern (card, SAFTOUT_PTN_CH1, &pattern);
	if(err != NoError) {
		printf("getpattern err %d!", err);
		Release_Card(card);
		exit(1);
	}
	printf("WDT SafetyOut Pattern of CH1: %x\n", pattern);

    err = WDT_Setup (card, 0 | WDT_OVRFLOW_SAFETYOUT, 5.0, &rt, &hWDTEvent);
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
//  you can also use the following function to enable the WDT interrupt
//	and wait for the hWDTEvent2[1] handle. And use DIO_SetDualInterrupt
//	to disable WDT interrupt	
/*	err = DIO_SetDualInterrupt (card, -1, INT2_WDT, hWDTEvent2);
	if(err != NoError) {
		printf("DIO_SetDualInterrupt err %d!", err);
		Release_Card(card);
		exit(1);
	}
*/

// you can use the following function for reloading WDT counter before WDT overflow
/*	err = WDT_Reload (card, 0, 1.0, &rt);
    if(err != NoError) {
		printf("WDT_Reload err %d!", err);
		Release_Card(card);
		exit(1);
	}
*/
	printf("\nWDT will overflow after %.4f seconds ...\n", rt);
    waitMS = (U32) rt*1000*2;
	result = WaitForSingleObject(hWDTEvent/*hWDTEvent2[1]*/, waitMS);
  
	if (result == WAIT_OBJECT_0) {
	  printf("\nWDT is overflowed !!\n");

	  Sleep(10);
	
	  DO_ReadPort (card, 0, &result);
	  printf("\nThe CH0 current pattern is : %x\n", result);
	  DO_ReadPort (card, 1, &result);
	  printf("The CH1 current pattern is : %x\n", result); 
 
	  printf("\nPress any key to continue ...\n");
	  getch();
	
	} 
	else if(result == WAIT_TIMEOUT) {
	   printf("\nWait Failed ...\n");
	} 
	else
	   printf("\nWaitForSingleObject error %d ...\n", GetLastError());
	
	WDT_Control (card, 0, WDT_DISARM);
//	DIO_SetDualInterrupt (card, -1, -1, 0);

    Release_Card(card);
}