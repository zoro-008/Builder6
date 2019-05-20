/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/02/05                                                       */
/*                                                                            */
/* This sample performs continuous AI acquisition.                            */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-922A. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define AI_COUNT 4000
#define P922X_TIMEBASE 80000000.0
#define CHANNELCOUNT 4

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT; /*922A Only Supports Internal Conversion Source*/
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT; /*922A Only Supports Post Trigger Mode and Software Trigger Source*/
    U32 ReTriggerCount = 0; /*922A dose not support Retrigger Mode*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U32 ScanIntrv = 320*CHANNELCOUNT; /*Scan Rate: P922X_TIMEBASE/ScanIntrv = 62.5K Hz*/
    U32 SampIntrv = 320; /*Sampling Rate: P922X_TIMEBASE/320 = 250K Hz; 922A Maximal Sample Rate = 250K Hz*/
    U16 RDBuffer[AI_COUNT]; /*Buffer to be stored read data*/
    U16 BufferId;
    F64 VBuffer[AI_COUNT]; /*Buffer to be stored converted data*/
    U32 AI_ReadCount = AI_COUNT; /*AI Read Count*/
    U16 NumChans = CHANNELCOUNT; /*AI Channel Count to be read*/
    U16 Chans[CHANNELCOUNT] = {0, 8, 16, 24}; /*AI Channels to be read*/
    U16 AdRanges[CHANNELCOUNT] = {AD_B_10_V, AD_B_5_V, AD_B_10_V, AD_B_5_V}; /*AI range; 922A supports +-10V and +-5V AD Range*/
    BOOLEAN Stopped;
    U32 AccessCnt;
    U8 FileName[] = "ai_data";
    /*--------------------------------*/

    memset(RDBuffer, '\0', AI_COUNT*sizeof(U16));
    memset(VBuffer, '\0', AI_COUNT*sizeof(F64));

    printf("This sample performs continuous AI acquisition from %d AI Channels\n", NumChans);
    printf("at %6.3lf Hz sampling rate.\n\n", P922X_TIMEBASE/SampIntrv);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_922A, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure AI*/
    err = AI_922A_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_922A_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Set Scan and Sampling Rate*/
    err = AI_922A_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_922A_CounterInterval Error: %d\n", err);
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
    err = AI_ContReadMultiChannelsToFile(card, NumChans, Chans, AdRanges, FileName, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadMultiChannelsToFile Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    do{
        /*Check whether AI Acquisition is done*/
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
        printf("%d samples of Buffer had been written to %s.dat file...\n", AccessCnt, FileName);
    }
    else{
        printf("\n\nAI Acquisition has been stopped... Acquired %d samples...\n", AccessCnt);
        printf("%d samples of Buffer had been written to %s.dat file...\n", AccessCnt, FileName);
    }

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();
    return 0;
}
