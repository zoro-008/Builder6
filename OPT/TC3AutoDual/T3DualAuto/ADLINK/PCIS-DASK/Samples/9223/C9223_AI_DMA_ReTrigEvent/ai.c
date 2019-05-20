/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs AI acquisition with DMA mode with retrigger mode.     */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 4000
#define RETRIGCOUNT 4
#define TOTALTRIGCOUNT (RETRIGCOUNT+1)
#define BUFFERCOUNT TOTALTRIGCOUNT*READCOUNT
#define P922X_TIMEBASE 80000000.0


I16 card, err;
U16 card_num;
U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_GPI0|P922x_AI_EnReTigger;
U32 ReTriggerCount = RETRIGCOUNT; /*ReTrigger Count = Total Trigger Count - 1*/
BOOLEAN AutoResetBuf = 1;
U32 ScanIntrv = 20000; /*Scan Rate: P922X_TIMEBASE/20000=4K*/
U32 SampIntrv = 20000; /*Sampling Rate: P922X_TIMEBASE/20000=4K*/
U16 RDBuffer[BUFFERCOUNT];
U16 BufferId;
F64 VBuffer[BUFFERCOUNT];
U32 AI_ReadCount = READCOUNT; /*AI Read Count per oe trigger*/
U32 AI_BufferCount = BUFFERCOUNT; /*Total read count*/
U16 Channel = 0; /*AI Channel Number*/
U16 AdRange = AD_B_10_V; /*AI Range*/
U32 i;
FILE *in_file;
U8 FileName[] = "ai_data.txt";
BOOLEAN Stopped = FALSE;
U32 AccessCnt = 0;
U32 TrigEndCount = 0;



void AI_END_CALLBACK()
{
    AI_AsyncClear(card, &AccessCnt);
    printf("\nAI Acquisition Done... Acquires %d samples...\n", AccessCnt);
    printf("Write Date to %s...\n", FileName);
    err = AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AccessCnt);
    if(err<0){
        for(i=0; i<AccessCnt; i++){
            fprintf(in_file, "%#x\n",  RDBuffer[i]);
        }
    }
    else{
        for(i=0; i<AccessCnt; i++){
            fprintf(in_file, "%#x, %6.4lf\n", RDBuffer[i], VBuffer[i]);
        }
    }
    Stopped = 1;
}

void AI_RETRIGGER_CALLBACK()
{
    TrigEndCount++;
    printf("\nAI Trigger End with next trigger: %d\n", TrigEndCount);
}


int main(int argc, char **argv)
{
    memset(RDBuffer, '\0', BUFFERCOUNT*sizeof(U16));
    memset(VBuffer, '\0', BUFFERCOUNT*sizeof(F64));
    printf("This sample performs continuous AI acquisition from AI Channel %d\n", Channel);
    printf("at %6.3lf Hz sampling rate with external digital re-trigger.\n", P922X_TIMEBASE/SampIntrv);

    if(!(in_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(in_file);
        exit(1);
    }

    /*Configure AI*/
    err = AI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9223_Config Error: %d\n", err);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Set AI Scan and Sample Interval*/
    err = AI_9223_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9223_CounterInterval Error: %d\n", err);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Disable Double Buffer Mode*/
    err = AI_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Setup Buffer for AI DMA*/
    err = AI_ContBufferSetup(card, RDBuffer, AI_BufferCount, &BufferId);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Enable AI END Event*/
    err = AI_EventCallBack(card, 1, AIEnd, (U32)AI_END_CALLBACK);
    if(err<0){
        printf("AI_EventCallBack Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Enable AI ReTrigger Event*/
    err = AI_EventCallBack(card, 1, TrigEvent, (U32)AI_RETRIGGER_CALLBACK);
    if(err<0){
        printf("AI_EventCallBack2 Error: %d\n", err);
        AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Start AI Acquisition*/
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId, AI_ReadCount, 0, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
        AI_EventCallBack(card, 0, TrigEvent, (U32)NULL);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }
    printf("\nWait %d Positive Trigger from GPI0...\n", ReTriggerCount+1);
    while((!Stopped)&&(!kbhit()));

    if(!Stopped)
        AI_AsyncClear(card, &AccessCnt);
    AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
    AI_EventCallBack(card, 0, TrigEvent, (U32)NULL);
    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);
    fclose(in_file);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
