#include <windows.h>
#include <stdio.h>
#include "dask.h"


void main()
{
  	I16 card, err; 
	U16 card_num;
	U32 pattern0, pattern1, pattern2, pattern3, result;
    U32 waitMS;
	F32 rt;

    HANDLE hWDTEvent = NULL;
	HANDLE hWDTEvent2[2];

	printf("Please input a card number: ");
    scanf(" %d", &card_num);
	
	if ((card=Register_Card (PCI_7444, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

	printf("Please input the WDT SafetyOut Pattern of CH0: ");
	scanf("%x",&pattern0);
	printf("Please input the WDT SafetyOut Pattern of CH1: ");
	scanf("%x",&pattern1);
	printf("Please input the WDT SafetyOut Pattern of CH2: ");
	scanf("%x",&pattern2);
	printf("Please input the WDT SafetyOut Pattern of CH3: ");
	scanf("%x",&pattern3);
		
	/*Set SafetyOut Pattern for Channel 0/1/2/3*/
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
	err = SetInitPattern (card, SAFTOUT_PTN_CH2, pattern2);
	if(err != NoError) {
		printf("setpattern err %d!", err);
		Release_Card(card);
		exit(1);
	}
	err = SetInitPattern (card, SAFTOUT_PTN_CH3, pattern3);
	if(err != NoError) {
		printf("setpattern err %d!", err);
		Release_Card(card);
		exit(1);
	}	

	/*Set Watch-Dog Timer to 5.0 Sec.*/ 
    err = WDT_Setup (card, 0|WDT_OVRFLOW_SAFETYOUT, 5.0, &rt, &hWDTEvent);
    if(err != NoError) {
		printf("WDT_Setup err %d!", err);
		Release_Card(card);
		exit(1);
	}
	
	/*Enable Watch-Dog Timer*/
	err = WDT_Control (card, 0, WDT_ARM);
    if(err != NoError) {
		printf("WDT_Control err %d!", err);
		Release_Card(card);
		exit(1);
	}
	// WDT_Control function can be replaced by the following function, DIO_SetDualInterrupt, 
	// to enable the WDT interrupt and wait for the hWDTEvent2[1] handle. 
	// Also, DIO_SetDualInterrupt should be used to disable WDT interrupt.
	/*	
	err = DIO_SetDualInterrupt (card, INT1_DISABLE, INT2_WDT, hWDTEvent2);
	if(err != NoError) {
		printf("DIO_SetDualInterrupt err %d!", err);
		Release_Card(card);
		exit(1);
	}
	*/

	// you can use the following function for reloading WDT counter before WDT overflow
	/*Reload Watch-Dog Timer to 1.0 Sec.*/
	err = WDT_Reload (card, 0, 1.0, &rt);
    if(err != NoError) {
		printf("WDT_Reload err %d!", err);
		Release_Card(card);
		exit(1);
	}


	/*Wait Event*/
	printf("\nWDT will overflow after %.4f seconds ...\n", rt);
    waitMS = (U32) rt*1000*2;
	result = WaitForSingleObject(hWDTEvent/*hWDTEvent2[1]*/, waitMS);
  
	if (result == WAIT_OBJECT_0) {
		printf("\nWDT is overflowed !!\n");
		DO_ReadPort (card, P7444_CH0, &result);
		printf("\nThe CH0 current pattern is : %x\n", result);
		DO_ReadPort (card, P7444_CH1, &result);
		printf("The CH1 current pattern is : %x\n", result); 
		DO_ReadPort (card, P7444_CH2, &result);
		printf("The CH2 current pattern is : %x\n", result);
		DO_ReadPort (card, P7444_CH3, &result);
		printf("The CH3 current pattern is : %x\n", result);  

		printf("\nPress any key to continue ...\n");
		getch();
	} 
	else if(result == WAIT_TIMEOUT) {
	   printf("\nWait Failed ...\n");
	} 
	else
	   printf("\nWaitForSingleObject error %d ...\n", GetLastError());

	/*Disable Watch-dog Timer*/
	WDT_Control (card, 0, WDT_DISARM);
	/*
	DIO_SetDualInterrupt (card, INT1_DISABLE, INT2_DISABLE, NULL);
	*/
    
	Release_Card(card);
}