/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs infinite wave repeats with external delay trigger.    */
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

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AO_CONVSRC_INT;
    U16 TrigCtrl = P922x_AO_TRGMOD_DELAY|P922x_AO_TRGSRC_GPI0|P922x_AO_EnDelay2;
    U32 ReTrgCnt = 0; //Ignored in non-retrigger mode
    U32 DLY1Cnt = 80000000; //Trigger delay count; Delay time: DLY1Cnt/TIMEBASE sec.
    U32 DLY2Cnt = 40000000; //Wave separation count; Delay2 time: DLY2Cnt/TIMEBASE sec.
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptor built in kernel
    U32 W_Buffer[WRITECOUNT];
    U32 WriteCount = WRITECOUNT;
    U16 BufferId;
    U16 Channel = 0;
    U32 Iterations = 0; //0: means infinite wave repeats
    U32 CHUI = UPDATEINTRV;
    U16 definite = 0;
    U32 vi;
    U32 AccessCnt;
    BOOLEAN Stopped;


    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (I16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
    }

    printf("This sample perform infinite wave repeats to AO Channel %d \n", Channel);
    printf("with external delay trigger at %6.2f update rate.\n", (float)TIMEBASE/UPDATEINTRV);
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

    err = AO_ContWriteChannel(card, Channel, BufferId, WriteCount, Iterations, CHUI, definite, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("\n\nWait trigger from GPI0 to update infinite wave repeats...\n");
    printf("You can press any key to stop AO update...");
    do{
        err = AO_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AO_AsyncCheck Error: %d\n", err);
            AO_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
    }while(!kbhit());getch();

    AO_AsyncClear(card, &AccessCnt, 0);
    printf("\n\nAO had been stopped...\n");

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
