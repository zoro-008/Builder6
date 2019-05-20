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

I16 card;
U16 card_num;
U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT; /*922A Only Supports Internal Conversion Source*/
U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT; /*922A Only Supports Post Trigger Mode and Software Trigger Source*/
U32 ReTriggerCount = 0; /*922A dose not support Retrigger Mode*/
BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
U32 ScanIntrv = 320; /*Scan Rate: P922X_TIMEBASE/320 = 250K Hz; 922A Maximal Scan Rate = 250K Hz*/
U32 SampIntrv = 320; /*Sampling Rate: P922X_TIMEBASE/320 = 250K Hz; 922A Maximal Sample Rate = 250K Hz*/
U16 RDBuffer[AI_COUNT]; /*Buffer to be stored read data*/
U16 BufferId;
F64 VBuffer[AI_COUNT]; /*Buffer to be stored converted data*/
U32 AI_ReadCount = AI_COUNT; /*AI Read Count*/
U16 Channel = 0; /*AI Channel Number to be read*/
U16 AdRange = AD_B_10_V; /*AI range; 922A supports +-10V and +-5V AD Range*/
BOOLEAN fStop = FALSE;
U32 AccessCnt;
FILE *w_file;
U8 FileName[] = "ai_data.txt";
U32 i;


void AI_END_CALLBACK()
{
    printf("\n\n\nAcquisition Done...\n");
    AI_AsyncClear(card, &AccessCnt);
    printf("Acquire %d data and write them to %s...\n", AccessCnt, FileName);

    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
    }
    else{
        fprintf(w_file, "Channel %d,\n", Channel);
        AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AccessCnt);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%f,\n", VBuffer[i]);
        }
        fclose(w_file);
    }
    fStop = 1;
}


int main(int argc, char **argv)
{
    I16 err;

    memset(RDBuffer, '\0', AI_COUNT*sizeof(U16));
    memset(VBuffer, '\0', AI_COUNT*sizeof(F64));

    printf("This sample performs continuous AI acquisition from AI Channel %d\n", Channel);
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

    /*Enable AIEnd Event*/
    err = AI_EventCallBack(card, 1, AIEnd, (U32)AI_END_CALLBACK);
    if(err<0){
        printf("AI_EventCallBack Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*AI Acquisition Start*/
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("Wait AI Acquisition Done Event...\n");
    printf("You can press any key to stop waiting...");
    do{
        Sleep(1);
    }while((!fStop)&&(!kbhit()));

    if(!fStop){
        printf("\n\n\nManually Stop...\n");
        AI_AsyncClear(card, &AccessCnt);
    }

    AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();
    return 0;
}
