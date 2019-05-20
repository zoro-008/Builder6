/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs AO retrigger update with event call back function.    */
/* There are 2 AO channels in PCI-9223. You can use polling mode or DMA to    */
/* to update data to specified channels.                                      */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "dask.h"

#define WRITECOUNT 8000
#define TIMEBASE 80000000
#define UPDATEINTRV 10000
#define PI 3.1415926

I16 card, err;
U16 card_num;
U16 ConfigCtrl = P922x_AO_CONVSRC_INT;
U16 TrigCtrl = P922x_AO_TRGMOD_POST|P922x_AO_TRGSRC_GPI0|P922x_AO_EnReTigger;
U32 ReTrgCnt = 3; //Retrigger count
U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
U32 DLY2Cnt = 0; //Ignored in non-delay2
BOOLEAN AutoResetBuf = 1;
U32 W_Buffer[WRITECOUNT];
U32 WriteCount = WRITECOUNT;
U16 BufferId;
U16 Channel = 0; //Updated AO channel
U32 Iterations = 2; //wave repeats
U32 CHUI = UPDATEINTRV;
U16 definite = 1;
U32 vi;
U32 AccessCnt;
BOOLEAN Stopped;
U32 TrigEndCount = 0;


void AO_END_CALLBACK()
{
    AO_AsyncClear(card, &AccessCnt, 0);
    printf("\nAO Done...\n");
    Stopped = 1;
}

void AO_RETRIGGER_CALLBACK()
{
    TrigEndCount++;
    printf("\nAO Trigger End with next trigger: %d\n", TrigEndCount);
}


int main(int argc, char **argv)
{

    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (I16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
    }

    printf("This sample updates voltage to AO Channel 0 at %6.2f update rate.\n", (float)TIMEBASE/UPDATEINTRV);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    err = AO_9223_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("AO_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AO_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("AO_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AO_ContBufferSetup(card, W_Buffer, WriteCount, &BufferId);
    if(err<0){
        printf("AO_ContBufferSetup Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Enable AO END Event*/
    err = AO_EventCallBack(card, 1, AOEnd, (U32)AO_END_CALLBACK);
    if(err<0){
        printf("AO_EventCallBack Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Enable AO ReTrigger Event*/
    err = AO_EventCallBack(card, 1, TrigEvent, (U32)AO_RETRIGGER_CALLBACK);
    if(err<0){
        printf("AO_EventCallBack2 Error: %d\n", err);
        AO_EventCallBack(card, 0, AOEnd, (U32)NULL);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    err = AO_ContWriteChannel(card, Channel, BufferId, WriteCount, Iterations, CHUI, definite, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_EventCallBack(card, 0, AOEnd, (U32)NULL);
        AO_EventCallBack(card, 0, TrigEvent, (U32)NULL);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    printf("\nWait %d Positive Trigger from GPI0...\n", ReTrgCnt+1);
    while((!Stopped)&&(!kbhit()));

    if(!Stopped){
        getch();
        AO_AsyncClear(card, &AccessCnt, 0);
        while(!Stopped)
            Sleep(1);
    }
    AO_EventCallBack(card, 0, AOEnd, (U32)NULL);
    AO_EventCallBack(card, 0, TrigEvent, (U32)NULL);

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
