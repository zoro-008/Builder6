/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/12                                                       */
/*                                                                            */
/* This sample performs continuous DO operation with call back function.      */
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
U16 TrigCtrl = P922x_DO_TRGMOD_POST|P922x_DO_TRGSRC_SOFT;
U32 ReTrgCnt = 0; //Not used in non-retrigger mode
U32 DLY1Cnt = 0; //Not used in post trigger mode
U32 DLY2Cnt = 0; //No used in 922x do
BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptors built in driver
U32 W_Buffer[WRITECOUNT]; //Data buffer to be written
U32 WriteCount = WRITECOUNT; //Output count
U16 BufferId;
U16 Port = 0; //Only one DO port
U16 Iterations = 1; //922x has no output repeats of digital output
F64 SampleRate = SAMPRATE;
BOOLEAN Stopped;
U32 AccessCnt = 0;
U32 vi;
U16 fStop;


void DO_END_CALLBACK()
{
    printf("\n\n\nDO Done...\n");
    if(!fStop){
        DO_AsyncClear(card, &AccessCnt);
        printf("%d data has been updated...\n", AccessCnt);
        fStop = 1;
    }
    else{
        printf("%d data has been updated...\n", AccessCnt);
        fStop = 0;
    }
}

int main(int argc, char **argv)
{
    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (U16)(((F32)vi/WriteCount)*65535);
    }
    printf("This sample performs continuous DO operation with call back function \n");
    printf("at %6.2f update rate.\n", SampleRate);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    err = DO_9222_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("DO_9222_Config Error: %d\n", err);
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

    /*Enable DOEnd Event*/
    err = DO_EventCallBack(card, 1, DOEnd, (U32)DO_END_CALLBACK);
    if(err<0){
        printf("DO_EventCallBack Error: %d\n", err);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    err = DO_ContWritePort(card, Port, &BufferId, WriteCount, Iterations, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DO_ContWritePort Error: %d\n", err);
        DO_EventCallBack(card, 0, DOEnd, (U32)NULL);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    printf("Wait DO Done Event...\n");
    printf("You can press any key to stop waiting...");

    while((!fStop)&&(!kbhit()));

    if(!fStop){
        getch();
        printf("\n\n\nManually Stop...\n");
        fStop = 1;
        DO_AsyncClear(card, &AccessCnt);
        do{
            Sleep(1);
        }while(fStop);
    }

    DO_EventCallBack(card, 0, DOEnd, (U32)NULL);
    if(!AutoResetBuf)
        DO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
