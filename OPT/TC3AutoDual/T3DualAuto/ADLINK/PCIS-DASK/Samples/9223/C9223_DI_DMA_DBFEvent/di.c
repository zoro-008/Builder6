/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs infinite DI DBF acquisition with call back function.  */
/* Two call back functions are set for DBEvent and AIEnd events.              */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 4000

I16 card;
U16 card_num;
U16 ConfigCtrl = P922x_DI_CONVSRC_INT;
U16 TrigCtrl = P922x_DI_TRGMOD_POST|P922x_DI_TRGSRC_SOFT;
U32 ReTriggerCnt = 0; /*Ignore if Double buffer mode is enable*/
BOOLEAN AutoResetBuf = 1; /*Autoreset buffer descriptor build in kernel*/
U16 RDBuffer0[READCOUNT]; /*Data Buffer for DI DMA*/
U16 RDBuffer1[READCOUNT]; /*Data Buffer for DI DMA*/
U32 DI_ReadCount = READCOUNT; /*DI Read Count per one buffer*/
U16 BufferId0;
U16 BufferId1;
U16 Port = 0;
F64 SampleRate = 4000;
U32 AccessCnt;
BOOLEAN StopFlag = FALSE;
U16 ViewIdx = 0;
U16 overrunFlag;
U8 filename[] = "di_data";
U32 HalfRdyCnt = 0;


void DI_END_CALLBACK()
{
    printf("\n\nAcquisition End...\n");
    printf("Total Acquire %d Data from DI port and Write them to %s.dat...\n",
        HalfRdyCnt*DI_ReadCount+AccessCnt, filename
    );
    StopFlag = 1;
}


void DI_DBEVENT_CALLBACK()
{
    I16 err;
    if(ViewIdx==0){
        printf("\n\nBuffer%d Half Ready...\n", ViewIdx);
        printf("        press any key to exit...");
        err = DI_AsyncDblBufferToFile(card);
        if(err<0)
            printf("DI_AsyncDblBufferToFile Error: %d\n", err);
        err = DI_AsyncDblBufferHandled(card);
        if(err<0)
            printf("DI_AsyncDblBufferToFile Error: %d\n", err);
    }
    else{
        printf("\n\nBuffer%d Half Ready...\n", ViewIdx);
        printf("        press any key to exit...");
        err = DI_AsyncDblBufferToFile(card);
        if(err<0)
            printf("DI_AsyncDblBufferToFile Error: %d\n", err);
        err = DI_AsyncDblBufferHandled(card);
        if(err<0)
            printf("DI_AsyncDblBufferToFile Error: %d\n", err);
    }

    err = DI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
    if(err<0){
        printf("DI_AsyncDblBufferOverrun Error: %d\n", err);
    }
    if(overrunFlag){
        printf("\nOVERRUN: %d\n", overrunFlag);
        err = DI_AsyncDblBufferOverrun(card, 1, NULL);
        if(err<0){
            printf("DI_AsyncDblBufferOverrun Error: %d\n", err);
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
    printf("This sample infinitely acquires data from DI Port\n");
    printf("at %6.3lf Hz sampling rate with DMA Double Buffer mode.\n", SampleRate);

    /*Open Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure DI*/
    err = DI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCnt, AutoResetBuf);
    if(err<0){
        printf("DI_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Enable Double Buffer Mode*/
    err = DI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("DI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup Buffer*/
    err = DI_ContBufferSetup(card, RDBuffer0, DI_ReadCount, &BufferId0);
    if(err<0){
        printf("DI_ContBufferSetup0 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DI_ContBufferSetup(card, RDBuffer1, DI_ReadCount, &BufferId1);
    if(err<0){
        printf("DI_ContBufferSetup1 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Enable DI Double Buffer Half Ready Event*/
    err = DI_EventCallBack(card, 1, DBEvent, (U32)DI_DBEVENT_CALLBACK);
    if(err<0){
        printf("DI_EventCallBack Error: %d\n", err);
        DI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    /*Enable DI END Event*/
    err = DI_EventCallBack(card, 1, DIEnd, (U32)DI_END_CALLBACK);
    if(err<0){
        printf("DI_EventCallBack2 Error: %d\n", err);
        DI_EventCallBack(card, 0, DBEvent, (U32)NULL);
        DI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    err = DI_ContReadPortToFile(card, Port, filename, DI_ReadCount, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DI_ContReadChanneltoFile Error: %d\n", err);
        DI_EventCallBack(card, 0, DBEvent, (U32)NULL);
        DI_EventCallBack(card, 0, DIEnd, (U32)NULL);
        DI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("DI Acquisition is started... Wait DI DBEvent...\n");
    printf("You can press any key to stop it...");
    getch();
    DI_AsyncClear(card, &AccessCnt);
    do{
        Sleep(1);
    }while(!StopFlag);

    DI_EventCallBack(card, 0, DBEvent, (U32)NULL);
    DI_EventCallBack(card, 0, DIEnd, (U32)NULL);
    if(!AutoResetBuf)
        DI_ContBufferReset(card);
    Release_Card(card);

    printf("press any key to exit...\n");
    getch();
    return 0;
}
