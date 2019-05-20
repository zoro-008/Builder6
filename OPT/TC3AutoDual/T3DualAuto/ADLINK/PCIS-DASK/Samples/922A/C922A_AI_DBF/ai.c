/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/02/05                                                       */
/*                                                                            */
/* This sample performs infinite AI acquisition by double buffer mode.        */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-922A. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "dask.h"

#define AI_COUNT 4000
#define P922X_TIMEBASE 80000000.0

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT; /*922A Only Supports Internal Conversion Source*/
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT; /*922A Only Supports Post Trigger Mode and Software Trigger Source*/
    U32 ReTriggerCount = 0; /*922A dose not support Retrigger Mode*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U32 ScanIntrv = 20000; /*Scan Rate: P922X_TIMEBASE/20000 = 4K Hz; 922A Maximal Scan Rate = 250K Hz*/
    U32 SampIntrv = 20000; /*Sampling Rate: P922X_TIMEBASE/20000 = 4K Hz; 922A Maximal Sample Rate = 250K Hz*/
    U16 RDBuffer0[AI_COUNT]; /*Buffer to be stored read data*/
    U16 RDBuffer1[AI_COUNT]; /*Buffer to be stored read data*/
    U32 AI_ReadCount = AI_COUNT; /*Buffer to be stored converted data*/
    U16 BufferId0;
    U16 BufferId1;
    U16 Channel = 0; /*AI Channel Number to be read*/
    U16 AdRange = AD_B_10_V; /*AI range; 922A supports +-10V and +-5V AD Range*/
    BOOLEAN Stopped = FALSE;
    BOOLEAN HalfReady = FALSE;
    U32 AccessCnt = 0;
    U16 BufIdx = 0;
    U16 OverrunFlag;
    U8 FileName[] = "ai_data";
    /*--------------------------------*/

    memset(RDBuffer0, '\0', AI_COUNT*sizeof(U16));
    memset(RDBuffer1, '\0', AI_COUNT*sizeof(U16));

    printf("This sample performs infinite AI acquisition from AI Channel %d\n", Channel);
    printf("at %6.3lf Hz sampling rate by Double buffer mode.\n\n", P922X_TIMEBASE/SampIntrv);

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
    err = AI_ContReadChannelToFile(card, Channel, AdRange, FileName, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
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
            //Let driver know that ready buffer is handled
            AI_AsyncDblBufferHandled(card);
            //Check if buffer overrun
            AI_AsyncDblBufferOverrun(card, 0, &OverrunFlag);
            if(OverrunFlag){
                printf("Buffer overrun...\n");
                AI_AsyncDblBufferOverrun(card, 1, NULL);
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
