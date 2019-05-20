/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/12                                                       */
/*                                                                            */
/* This sample simultaneously infinite updates AO voltage to AO two channels  */
/* with event call back functions.                                            */
/* In simultaneously continuous AO, AO_Buffer[0] and AO_Buffer[1] are         */
/* simultaneously updating to AO Channel 0 and 1, then, AO_Buffer[2] and      */
/* AO_Buffer[3] are simultaneously updating to AO Channel 0 and 1, ..., etc.  */
/* With event call back functions, the set CallBack function will be          */
/* performed while the set event is comming.                                  */
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
#define WRITECHNCNT 2

I16 card, err;
U16 card_num;
U16 ConfigCtrl = P922x_AO_CONVSRC_INT;
U16 TrigCtrl = P922x_AO_TRGMOD_POST|P922x_AO_TRGSRC_SOFT;
U32 ReTrgCnt = 0; //Ignored in non-retrigger mode
U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
U32 DLY2Cnt = 0; //Ignored in non-delay2
BOOLEAN AutoResetBuf = 1;
U32 W_Buffer0[WRITECOUNT*WRITECHNCNT]; //Data buffer to be written
U32 W_Buffer1[WRITECOUNT*WRITECHNCNT]; //Data buffer to be written
U32 Patten0[WRITECOUNT*WRITECHNCNT];
U32 Patten1[WRITECOUNT*WRITECHNCNT];
U32 WriteCount = WRITECOUNT;
U32 BufferCnt = WRITECOUNT*WRITECHNCNT; //needs extra buffer size for simultaneously AO update
U16 BufferId0;
U16 BufferId1;
U16 NumChans = WRITECHNCNT; //Numbers of AO channels
U16 Chans[WRITECHNCNT] = {0, 1}; //AO Channels
U32 Iterations = 0; //Infinite repeats
U32 CHUI = UPDATEINTRV;
U16 definite = 0;
U32 vi;
U32 AccessCnt;
U16 StopFlag = 0;
U16 ViewIdx = 0;
U32 BufChgIdx = 0;


void AO_END_CALLBACK()
{
    printf("\nAO End...\n");
    StopFlag = 1;
}

void AO_DBEVENT_CALLBACK()
{
    if(ViewIdx==0){
        printf("Buffer 0 updated...\n");
        if(BufChgIdx==0)
            memcpy(W_Buffer0, Patten1, BufferCnt*sizeof(U32));
        else
            memcpy(W_Buffer0, Patten0, BufferCnt*sizeof(U32));
    }
    else{
        printf("Buffer 1 updated...\n");
        if(BufChgIdx==0)
            memcpy(W_Buffer1, Patten0, BufferCnt*sizeof(U32));
        else
            memcpy(W_Buffer1, Patten1, BufferCnt*sizeof(U32));
    }
    BufChgIdx = (ViewIdx==1)? ((BufChgIdx==0)? 1:0):BufChgIdx;
    ViewIdx = ViewIdx? 0:1;
}


int main(int argc, char **argv)
{
    for(vi=0; vi<BufferCnt; vi++){
        if(vi%2==0){
            /*Channel 0*/
            Patten0[vi] = (I16)((sin((double)(vi/2)/WriteCount*2*PI)*0x7fff)+0x8000);
            Patten1[vi] = (U16)((float)((vi/2)*65535)/(float)WriteCount);
        }
        else{
            /*Channel 1*/
            Patten0[vi] = (U16)((float)((vi/2)*65535)/(float)WriteCount);
            Patten1[vi] = (I16)((sin((double)(vi/2)/WriteCount*2*PI)*0x7fff)+0x8000);
        }
    }
    memcpy(W_Buffer0, Patten0, BufferCnt*sizeof(U32));
    memcpy(W_Buffer1, Patten1, BufferCnt*sizeof(U32));

    printf("This sample performs AO simultaneously update at %6.2f update rate \n", (float)TIMEBASE/UPDATEINTRV);
    printf("with DBEvent and AOEnd event call back.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    err = AO_9222_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("AO_9222_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AO_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AO_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AO_ContBufferSetup(card, W_Buffer0, BufferCnt, &BufferId0);
    if(err<0){
        printf("AO_ContBufferSetup0 Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    err = AO_ContBufferSetup(card, W_Buffer1, BufferCnt, &BufferId1);
    if(err<0){
        printf("AO_ContBufferSetup1 Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Enable AO Double Buffer Half Ready Event*/
    err = AO_EventCallBack(card, 1, DBEvent, (U32)AO_DBEVENT_CALLBACK);
    if(err<0){
        printf("AO_EventCallBack Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    /*Enable AO END Event*/
    err = AO_EventCallBack(card, 1, AOEnd, (U32)AO_END_CALLBACK);
    if(err<0){
        printf("AO_EventCallBack2 Error: %d\n", err);
        AO_EventCallBack(card, 0, DBEvent, (U32)NULL);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }


    err = AO_ContWriteMultiChannels(card, NumChans, Chans, BufferId0, WriteCount, Iterations, CHUI, definite, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_EventCallBack(card, 0, DBEvent, (U32)NULL);
        AO_EventCallBack(card, 0, AOEnd, (U32)NULL);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    printf("AO is started... Wait AO DBEvent...\n");
    printf("You can press any key to stop it...\n\n");
    getch();
    AO_AsyncClear(card, &AccessCnt, 0);
    do{
        Sleep(1);
    }while(!StopFlag);

    AO_EventCallBack(card, 0, DBEvent, (U32)NULL);
    AO_EventCallBack(card, 0, AOEnd, (U32)NULL);

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
