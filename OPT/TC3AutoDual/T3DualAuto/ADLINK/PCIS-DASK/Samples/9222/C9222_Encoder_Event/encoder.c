/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/11                                                       */
/*                                                                            */
/* This sample performs X4 AB phase encode with position event.               */
/* GPTC_EventSetup() can be used to setup encoder position configuration.     */
/* PCI-922x will generate a pulse output to GPO4/GPO5 (Encoder0/Encoder1)     */
/* or generate an interrupt local interrupt to do the callback Function       */
/* set by GPTC_EventCallBack() while multiples of the set encode position     */
/* reachs.                                                                    */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

U32 EPT0_Count = 0;
U32 EPT1_Count = 0;


/*Call Back Function of Encoder0 Encode Position*/
void EPT0_CallBack()
{
    EPT0_Count++;
}

/*Call Back Function of Encoder1 Encode Position*/
void EPT1_CallBack()
{
    EPT1_Count++;
}


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 GCtr;
    U16 Mode = x4_AB_Phase_Encoder;
    U16 SrcCtrl = 0; //Not used in encoder
    U16 PolCtrl = 0; //Not used in encoder
    U32 LReg1_Val = 0; //Not used in encoder
    U32 LReg2_Val = 0; //Not used in encoder
    U16 EventMode = P922x_EVT_MOD_EPT;
    U16 EventCtrl = P922x_EPT_PULWIDTH_200ms|P922x_EPT_TRGOUT_GPO|P922x_EPT_TRGOUT_CALLBACK;
    U32 LVal_1 = 100; //Position steps
    U32 LVal_2 = 0;   //Not used
    I16 EventType;
    U32 callbackAddr;
    U32 RD_Value;
    int i = 0;

    printf("This sample performs x4 AB phase mode with position event.\n");
    printf("A pulse will be generated to GPO4/GPO5 (Encoder0/Encoder1)\n");
    printf("and the set callback function will be performed while multiples\n");
    printf("of the set position reachs.\n");

    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Encoder Number? ");
    scanf(" %hd", &GCtr);
    GCtr = (GCtr==1)? P922x_ENCODER1:P922x_ENCODER0;

    /*Clear Encoder*/
    err = GPTC_Clear(card, GCtr);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        goto ERR_RET;
    }

    /*Setup Encoder*/
    err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        goto ERR_RET;
    }

    /*Encoder Event Setup*/
    err = GPTC_EventSetup(card, GCtr, EventMode, EventCtrl, LVal_1,  LVal_2);
    if(err<0){
        printf("GPTC_EventSetup Error: %d\n", err);
        goto ERR_RET;
    }

    EventType = (GCtr==P922x_ENCODER1)? P922x_EVT_TYPE_EPT1:P922x_EVT_TYPE_EPT0;
    callbackAddr = (U32)((GCtr==P922x_ENCODER1)? EPT1_CallBack:EPT0_CallBack);
    /*Setup EventCallBack for Encoder Position*/
    err = GPTC_EventCallBack(card, 1, EventType, callbackAddr);
    if(err<0){
        printf("GPTC_EventCallBack Error: %d\n", err);
        goto ERR_RET;
    }

    /*Enable Encoder*/
    printf("Press any key to enable counter...");
    getch();
    err = GPTC_Control(card, GCtr, IntENABLE, 1);
    if(err<0){
        printf("GPTC_Control(IntENABLE) Error: %d\n", err);
        goto ERR_RET;
    }

    printf("\n\nYou can press any key to stop...\n");
    do{
        /*Read Encoder*/
        err = GPTC_Read(card, GCtr, &RD_Value);
        if(err<0){
            printf("GPTC_Read Error: %d\n", err);
            goto ERR_RET;
        }
        printf("Encoder%d Value: %11d\r", GCtr-4, RD_Value);
        Sleep(100);
    }while(!kbhit());getch();
    printf("Encoder%d Value: %11d\n", GCtr-4, RD_Value);

    if(GCtr==P922x_ENCODER0)
        printf("Total EPT0 Event Count: %d\n", EPT0_Count);
    else
        printf("Total EPT1 Event Count: %d\n", EPT1_Count);

ERR_RET:
    EventType = (GCtr==P922x_ENCODER1)? P922x_EVT_TYPE_EPT1:P922x_EVT_TYPE_EPT0;
    GPTC_EventCallBack(card, 0, EventType, (U32)NULL);
    GPTC_Clear(card, GCtr);
    Release_Card(card);

    printf("\n\nPress any key to exit...");
    getch();
    return 0;
}
