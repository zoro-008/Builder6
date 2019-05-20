/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs continuous AI acquisition with external               */
/* digital Trigger.                                                           */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define AI_COUNT 4000
#define P922X_TIMEBASE 80000000.0

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_GPI0; /*Trigger Source: GPI0*/
    U32 ReTriggerCount = 0; /*Ignore if Retrigger is not enabled*/
    BOOLEAN AutoResetBuf = 1;
    U32 ScanIntrv = 320; /*Scan Rate: P922X_TIMEBASE/320=250K Hz*/
    U32 SampIntrv = 320; /*Sampling Rate: P922X_TIMEBASE/320=250K Hz*/
    U16 RDBuffer[AI_COUNT]; /*Buffer to be setup*/
    U16 BufferId;
    U32 AI_ReadCount = AI_COUNT; /*AI read count*/
    U16 Channel = 0; /*AI Channel Number to be read*/
    U16 AdRange = AD_B_10_V; /*AI Range*/
    BOOLEAN Stopped;
    U32 AccessCnt;
    U8 FileName[] = "ai_data";
    /*--------------------------------*/

    memset(RDBuffer, '\0', AI_COUNT*sizeof(U16));
    printf("This sample performs continuous AI acquisition from AI Channel %d\n", Channel);
    printf("at %6.3lf Hz sampling rate with external digital trigger.\n\n", P922X_TIMEBASE/SampIntrv);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure AI*/
    err = AI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Set Scan and Sampling Rate*/
    err = AI_9223_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9223_CounterInterval Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Disable Double Buffer Mode*/
    err = AI_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup Buffer for AI DMA Transfer*/
    err = AI_ContBufferSetup(card, RDBuffer, AI_ReadCount, &BufferId);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*AI Acquisition Start*/
    printf("\nWait External Digital Trigger from GPI0...");
    err = AI_ContReadChannelToFile(card, Channel, AdRange, FileName, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    do{
        /*Check Buffer Ready*/
        err = AI_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AI_AsyncCheck Error: %d\n", err);
            AI_AsyncClear(card, &AccessCnt);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
    }while((!kbhit())&&(!Stopped));

    /*Clear AI Setting and Get Remaining data*/
    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        AI_AsyncClear(card, &AccessCnt);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    if(Stopped){
        printf("\n\nAI Acquisition Done... Acquired %d samples...\n", AccessCnt);
    }
    else{
        printf("\n\nAI Acquisition has been stopped manually... Acquired %d samples...\n", AccessCnt);
    }
    printf("%d samples of Buffer0 had been written to %s.dat file...\n", AccessCnt, FileName);

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
