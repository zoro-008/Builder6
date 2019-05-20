/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs infinite AI acquisition by double buffer mode.        */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "dask.h"

#define AI_COUNT 4000
#define CHANNELCOUNT 4
#define P922X_TIMEBASE 80000000.0

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT;
    U32 ReTriggerCount = 0; /*Ignore in Double Buffer Mode*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U32 ScanIntrv = 20000*CHANNELCOUNT; /*Scan Rate: P922X_TIMEBASE/20000/4 = 1K Hz*/
    U32 SampIntrv = 20000; /*Sampling Rate: P922X_TIMEBASE/20000 = 4K Hz*/
    U16 RDBuffer0[AI_COUNT]; /*Buffer to be setup*/
    U16 RDBuffer1[AI_COUNT]; /*Buffer to be setup*/
    U32 AI_ReadCount = AI_COUNT; /*AI read count per one buffer*/
    U16 BufferId0;
    U16 BufferId1;
    U16 NumChans = CHANNELCOUNT; /*AI Channel Counts to be read*/
    U16 Chans[CHANNELCOUNT]; /*AI Channels array*/
    U16 AdRanges[CHANNELCOUNT]; /*AI Ranges array*/
    BOOLEAN Stopped;
    BOOLEAN HalfReady;
    U32 AccessCnt = 0;
    U16 BufIdx = 0;
    U8 FileName[] = "ai_data";
    U32 i, j;
    /*--------------------------------*/

    memset(RDBuffer0, '\0', AI_COUNT*sizeof(U16));
    memset(RDBuffer1, '\0', AI_COUNT*sizeof(U16));
    /*randon 4 channels*/
    srand(time(NULL));
    i = 0;
    do{
        Chans[i] = rand()%32;
        for(j=0; j<i; j++){
            if(Chans[i]==Chans[j])
                break;
        }
        if(j!=i)
            continue;
        if(i%2==0)
            AdRanges[i] = AD_B_10_V;
        else
            AdRanges[i] = AD_B_5_V;
        i++;
    }while(i<NumChans);

    printf("This sample performs infinite AI acquisition from randon %d AI Channels\n", NumChans);
    printf("at %6.3lf Hz scan rate by Double buffer mode.\n\n", P922X_TIMEBASE/ScanIntrv);

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

    /*Enable Double Buffer Mode*/
    err = AI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup Buffer for AI DMA Transfer*/
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

    /*AI Acquisition Start*/
    err = AI_ContReadMultiChannelsToFile(card, NumChans, Chans, AdRanges, FileName, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadMultiChannels Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    do{
        /*Check Buffer Ready*/
        err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &Stopped);
        if(err<0){
            printf("AI_AsyncDblBufferHalfReady Error: %d\n", err);
            AI_AsyncClear(card, &AccessCnt);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
        if(HalfReady){
            if(BufIdx==0){
                printf("\nBuffer0 Half Ready...\n");
                printf("Write %d samples of Buffer0 to %s.dat file...\n", AI_ReadCount, FileName);
                printf("                            Press any key to stop...\n");
                AI_AsyncDblBufferToFile(card);
                BufIdx = 1;
            }
            else{
                printf("\nBuffer1 Half Ready...\n");
                printf("Write %d samples of Buffer1 to %s.dat file...\n", AI_ReadCount, FileName);
                printf("                            Press any key to stop...\n");
                AI_AsyncDblBufferToFile(card);
                BufIdx = 0;
            }
        }
    }while(!kbhit());getch();

    /*Clear AI Setting and Get Remaining data*/
    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        AI_AsyncClear(card, &AccessCnt);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    if(BufIdx==0){
        printf("\nLast %d samples of Buffer0 had been written to %s.dat file...\n", AccessCnt, FileName);
    }
    else{
        printf("\nLast %d samples of Buffer1 had been written to %s.dat file...\n", AccessCnt, FileName);
    }

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);

    printf("                            Press any key to exit...\n");
    getch();
    return 0;
}
