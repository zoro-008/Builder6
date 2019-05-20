/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/11                                                       */
/*                                                                            */
/* This sample infinitely updates AO voltage with DMA Double Buffer mode.     */
/* There are 2 AO channels in PCI-9222. You can use polling mode or DMA to    */
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
    U16 TrigCtrl = P922x_AO_TRGMOD_POST|P922x_AO_TRGSRC_SOFT;
    U32 ReTrgCnt = 0; //Ignored in non-retrigger mode
    U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
    U32 DLY2Cnt = 0; //Ignored in non-delay2 mode
    BOOLEAN AutoResetBuf = 1;
    U32 W_Buffer0[WRITECOUNT];
    U32 W_Buffer1[WRITECOUNT];
    U32 Patten0[WRITECOUNT];
    U32 Patten1[WRITECOUNT];
    U32 WriteCount = WRITECOUNT;
    U16 BufferId0;
    U16 BufferId1;
    U16 Channel = 0; //AO channel to be written
    U32 Iterations = 0; //Infinite repeate
    U32 CHUI = UPDATEINTRV;
    U16 definite = 0;
    U32 vi;
    BOOLEAN HalfReady;
    U32 AccessCnt;
    U16 RdyBufCnt = 0;

    for(vi=0; vi<WriteCount; vi++){
        Patten0[vi] = (I16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
        Patten1[vi] = (U16)((float)(vi*65535)/(float)WriteCount);
    }
    memcpy(W_Buffer0, Patten0, WriteCount*sizeof(U32));
    memcpy(W_Buffer1, Patten1, WriteCount*sizeof(U32));

    printf("This sample infinitely updates voltage to AO Channel 0\n");
    printf("with double buffer mode at %6.2f update rate.\n", (float)TIMEBASE/UPDATEINTRV);
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

    /*Enable Double Buffer Mode*/
    err = AO_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AO_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup buffer*/
    err = AO_ContBufferSetup(card, W_Buffer0, WriteCount, &BufferId0);
    if(err<0){
        printf("AO_ContBufferSetup0 Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    err = AO_ContBufferSetup(card, W_Buffer1, WriteCount, &BufferId1);
    if(err<0){
        printf("AO_ContBufferSetup1 Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Start AO DAC*/
    err = AO_ContWriteChannel(card, Channel, BufferId0, WriteCount, Iterations, CHUI, definite, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    do{
        err = AO_AsyncDblBufferHalfReady(card, &HalfReady);
        if(err<0){
            printf("AO_AsyncDblBufferHalfReady Error: %d\n", err);
            AO_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
        if(HalfReady){
            printf("AO Buffer HalfReady\n");
            RdyBufCnt++;
            if(RdyBufCnt%4==1){
                memcpy(W_Buffer0, Patten1, WriteCount*sizeof(U32));
            }
            else if(RdyBufCnt%4==2){
                memcpy(W_Buffer1, Patten0, WriteCount*sizeof(U32));
            }
            else if(RdyBufCnt%4==3){
                memcpy(W_Buffer0, Patten0, WriteCount*sizeof(U32));
            }
            else{
                memcpy(W_Buffer1, Patten1, WriteCount*sizeof(U32));
            }
        }
        Sleep(10);
    }while(!kbhit());getch();

    printf("\nAO Update Done...\n");
    AO_AsyncClear(card, &AccessCnt, 0);

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
