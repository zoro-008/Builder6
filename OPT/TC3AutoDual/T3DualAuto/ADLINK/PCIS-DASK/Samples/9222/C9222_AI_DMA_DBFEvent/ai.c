/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/07/30                                                       */
/*                                                                            */
/* This sample performs infinite AI acquisition with DMA Double buffer mode.  */
/* There are 16(Single Ended Mode)/8(Differetial Mode) AI channels in         */
/* PCI-9222. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 4000
#define P922X_TIMEBASE 80000000.0

I16 card;
U16 card_num;
U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT;
U32 ReTriggerCnt = 0; /*Ignore if Double buffer mode is enable*/
BOOLEAN AutoResetBuf = 1; /*Autoreset buffer descriptor build in kernel*/
U32 ScanIntrv = 20000; /*Scan Rate: P922X_TIMEBASE/20000 = 4K Hz*/
U32 SampIntrv = 20000; /*Sampling Rate: P922X_TIMEBASE/20000 = 4K Hz*/
U16 RDBuffer0[READCOUNT]; /*Data Buffer for AI DMA*/
U16 RDBuffer1[READCOUNT]; /*Data Buffer for AI DMA*/
U32 AI_ReadCount = READCOUNT; /*AI Read Count per one buffer*/
U16 BufferId0;
U16 BufferId1;
U16 Channel = 0; /*AI Channel Number*/
U16 AdRange = AD_B_10_V; /*AI Ranges*/
U32 AccessCnt;
BOOLEAN StopFlag = FALSE;
U16 ViewIdx = 0;
U16 overrunFlag;
U8 filename[] = "ai_data";
U32 HalfRdyCnt = 0;


/*------------------------------------*/
/* AI End Call Back Function          */
/*------------------------------------*/
void AI_END_CALLBACK()
{
    printf("\n\nAcquisition End...\n");
    printf("Total Acquire %d Data from Channel %d and Write them to %s.dat...\n",
        HalfRdyCnt*AI_ReadCount+AccessCnt, Channel, filename
    );
    StopFlag = 1;
}


void AI_DBEVENT_CALLBACK()
{
    I16 err;
    if(ViewIdx==0){
        printf("\n\nBuffer%d Half Ready...\n", ViewIdx);
        printf("        press any key to exit...");
        err = AI_AsyncDblBufferToFile(card);
        if(err<0)
            printf("AI_AsyncDblBufferToFile Error: %d\n", err);
        err = AI_AsyncDblBufferHandled(card);
        if(err<0)
            printf("AI_AsyncDblBufferToFile Error: %d\n", err);
    }
    else{
        printf("\n\nBuffer%d Half Ready...\n", ViewIdx);
        printf("        press any key to exit...");
        err = AI_AsyncDblBufferToFile(card);
        if(err<0)
            printf("AI_AsyncDblBufferToFile Error: %d\n", err);
        err = AI_AsyncDblBufferHandled(card);
        if(err<0)
            printf("AI_AsyncDblBufferToFile Error: %d\n", err);
    }

    err = AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
    if(err<0){
        printf("AI_AsyncDblBufferOverrun Error: %d\n", err);
    }
    if(overrunFlag){
        printf("\nOVERRUN: %d\n", overrunFlag);
        err = AI_AsyncDblBufferOverrun(card, 1, NULL);
        if(err<0){
            printf("AI_AsyncDblBufferOverrun Error: %d\n", err);
        }
    }
    ViewIdx = ViewIdx? 0:1;
    HalfRdyCnt++;
}


int main(int argc, char **argv)
{
    I16 err;

    memset(RDBuffer0, '\0', READCOUNT*sizeof(U16));
    memset(RDBuffer1, '\0', READCOUNT*sizeof(U16));
    printf("This sample infinitely acquires analog voltage from AI Channel %d", Channel);
    printf("at %6.3lf Hz sampling rate with DMA Double Buffer mode.\n", P922X_TIMEBASE/SampIntrv);

    /*Open Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure AI*/
    err = AI_9222_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCnt, AutoResetBuf);
    if(err<0){
        printf("AI_9222_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Set AI Scan and Sample Interval*/
    err = AI_9222_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9222_CounterInterval Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Enable Double Buffer Mode*/
    err = AI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup Buffer*/
    err = AI_ContBufferSetup(card, RDBuffer0, AI_ReadCount, &BufferId0);
    if(err<0){
        printf("AI_ContBufferSetup0 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_ContBufferSetup(card, RDBuffer1, AI_ReadCount, &BufferId1);
    if(err<0){
        printf("AI_ContBufferSetup1 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Enable AI Double Buffer Half Ready Event*/
    err = AI_EventCallBack(card, 1, DBEvent, (U32)AI_DBEVENT_CALLBACK);
    if(err<0){
        printf("AI_EventCallBack Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    /*Enable AI END Event*/
    err = AI_EventCallBack(card, 1, AIEnd, (U32)AI_END_CALLBACK);
    if(err<0){
        printf("AI_EventCallBack2 Error: %d\n", err);
        AI_EventCallBack(card, 0, DBEvent, (U32)NULL);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    err = AI_ContReadChannelToFile(card, Channel, AdRange, filename, AI_ReadCount, 0, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChanneltoFile Error: %d\n", err);
        AI_EventCallBack(card, 0, DBEvent, (U32)NULL);
        AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("AI Acquisition is started... Wait AI DBEvent...\n");
    printf("You can press any key to stop it...");
    getch();
    AI_AsyncClear(card, &AccessCnt);
    do{
        Sleep(1);
    }while(!StopFlag);

    AI_EventCallBack(card, 0, DBEvent, (U32)NULL);
    AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);

    printf("press any key to exit...\n");
    getch();
    return 0;
}
