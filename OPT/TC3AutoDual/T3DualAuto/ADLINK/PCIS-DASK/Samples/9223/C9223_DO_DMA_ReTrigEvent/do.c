/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs continuous DO retrigger operation with                */
/* call back function.                                                        */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "dask.h"

#define WRITECOUNT 4000
#define TIMEBASE 80000000
#define SAMPRATE 4000

I16 card, err;
U16 card_num;
U16 ConfigCtrl = P922x_DO_CONVSRC_INT;
U16 TrigCtrl = P922x_DO_TRGMOD_POST|P922x_DO_TRGSRC_GPI0|P922x_DO_EnReTigger;
U32 ReTrgCnt = 3; //Retrigger Count
U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
U32 DLY2Cnt = 0; //No used in 922x DO
BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptors built in driver
U32 W_Buffer[WRITECOUNT]; //Data buffer to be written
U32 WriteCount = WRITECOUNT; //Output count
U16 BufferId;
U16 Port = 0; //Only one DO port
U16 Iterations = 1; //922x has no DO repeats
F64 SampleRate = SAMPRATE;
BOOLEAN Stopped;
U32 AccessCnt = 0;
U32 vi;
BOOLEAN Stopped = 0;
U32 TrigEndCount = 0;


void DO_END_CALLBACK()
{
    if(!Stopped){
        DO_AsyncClear(card, &AccessCnt);
        printf("\nDO Done...\n");
        Stopped = 1;
    }
    else{
        printf("\nDO Done...\n");
        Stopped = 0;
    }
}

void DO_RETRIGGER_CALLBACK()
{
    TrigEndCount++;
    printf("\nDO Trigger End with next trigger: %d\n", TrigEndCount);
}


int main(int argc, char **argv)
{

    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (U16)(((F32)vi/WriteCount)*65535);
    }
    printf("This sample performs continuous DO retrigger mode with call back function \n");
    printf("at %6.2f update rate.\n", SampleRate);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    err = DO_9223_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("DO_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = DO_ContBufferSetup(card, W_Buffer, WriteCount, &BufferId);
    if(err<0){
        printf("DO_ContBufferSetup Error: %d\n", err);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Enable DO END Event*/
    err = DO_EventCallBack(card, 1, DOEnd, (U32)DO_END_CALLBACK);
    if(err<0){
        printf("DO_EventCallBack Error: %d\n", err);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Enable DO ReTrigger Event*/
    err = DO_EventCallBack(card, 1, TrigEvent, (U32)DO_RETRIGGER_CALLBACK);
    if(err<0){
        printf("DO_EventCallBack2 Error: %d\n", err);
        DO_EventCallBack(card, 0, DOEnd, (U32)NULL);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    err = DO_ContWritePort(card, Port, &BufferId, WriteCount, Iterations, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DO_ContWritePort Error: %d\n", err);
        DO_EventCallBack(card, 0, DOEnd, (U32)NULL);
        DO_EventCallBack(card, 0, TrigEvent, (U32)NULL);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    printf("\nWait %d Positive Trigger from GPI0...\n", ReTrgCnt+1);
    while((!Stopped)&&(!kbhit()));

    if(!Stopped){
        getch();
        printf("\n\nDO is stopped manually...\n");
        Stopped = 1;
        DO_AsyncClear(card, &AccessCnt);
        while(Stopped)
            Sleep(1);
    }
    DO_EventCallBack(card, 0, DOEnd, (U32)NULL);
    DO_EventCallBack(card, 0, TrigEvent, (U32)NULL);

    if(!AutoResetBuf)
        DO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
