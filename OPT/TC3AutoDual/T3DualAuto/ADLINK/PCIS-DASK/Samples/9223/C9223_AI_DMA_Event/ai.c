/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs Continuous AI Channel Scan with Call Back Function.   */
/* There are 16(Single Ended Mode)/8(Differetial Mode) AI channels in         */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 4000
#define SCANCHNCNT 4
#define P922X_TIMEBASE 80000000.0

I16 card;
U16 card_num;
U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT;
U32 RetriggerCount = 0; /*Ignore if ReTrigger mode is not enabled*/
BOOLEAN AutoResetBuf = 0;
U32 ScanIntrv = 320*SCANCHNCNT; /*Scan Rate: P922X_TIMEBASE/320/4=67.5K Hz*/
U32 SampIntrv = 320; /*Scan Rate: P922X_TIMEBASE/320=250K Hz*/
U16 RDBuffer[READCOUNT]; /*Data Buffer to be setup*/
U16 BufferId;
F64 VBuffer[READCOUNT];
U32 AI_ReadCount = READCOUNT; /*AI Read Count*/
U16 Channel = SCANCHNCNT-1; /*Max Scan Channel Number*/
U16 AdRange = AD_B_10_V; /*AI Ranges*/
BOOLEAN fStop = FALSE;
U32 AccessCnt;
FILE *in_file;
U8 FileName[] = "ai_data.txt";
U32 i, j;

void AI_END_CALLBACK()
{
    printf("\n\n\nAcquisition Done...\n");
    AI_AsyncClear(card, &AccessCnt);
    printf("Acquire %d data and write them to %s...\n", AccessCnt, FileName);
    AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AccessCnt);
    for(i=0; i<AccessCnt/SCANCHNCNT; i++){
        for(j=0; j<SCANCHNCNT; j++){
            fprintf(in_file, "%6.4lf;     ", VBuffer[i*SCANCHNCNT+j]);
        }
        fprintf(in_file, "\n");
    }
    fStop = 1;
}


int main(int argc, char **argv)
{
    I16 err;

    memset(RDBuffer, '\0', READCOUNT*sizeof(U16));
    memset(VBuffer, '\0', READCOUNT*sizeof(F64));
    printf("This sample performs continuous AI channel scan from channel 0 to %d\n", Channel);
    printf("at %6.3lf Hz scan rate.\n", P922X_TIMEBASE/ScanIntrv);
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
    err = AI_9223_Config(card, ConfigCtrl, TrigCtrl, RetriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9223_Config Error: %d\n", err);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Set Scan and Sample Interval for AI*/
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

    /*Build Buffer Descriptor for AI DMA*/
    err = AI_ContBufferSetup(card, RDBuffer, AI_ReadCount, &BufferId);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    /*Enable AIEnd Event*/
    err = AI_EventCallBack(card, 1, AIEnd, (U32)AI_END_CALLBACK);
    if(err<0){
        printf("AI_EventCallBack Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(in_file);
        exit(1);
    }

    err = AI_ContScanChannels(card, Channel, AdRange, &BufferId, AI_ReadCount, 0, ASYNCH_OP);
    if(err<0){
        printf("AI_ContScanChannels Error: %d\n", err);
        AI_EventCallBack(card, 0, AIEnd, (U32)NULL);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(in_file);
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
    AI_ContBufferReset(card);
    Release_Card(card);
    fclose(in_file);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
