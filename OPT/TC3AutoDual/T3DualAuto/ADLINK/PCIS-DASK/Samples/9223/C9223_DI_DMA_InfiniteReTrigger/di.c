/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/19                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition with infinite retriggrt.    */
/* With infinite retrigger mode, there are two buffer should be setup for     */
/* infinite acquisition.                                                      */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define DI_COUNT 4000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_DI_CONVSRC_INT;
    U16 TrigCtrl = P922x_DI_TRGMOD_POST|P922x_DI_TRGSRC_GPI0|P922x_DI_EnReTigger;
    U32 ReTriggerCount = 0; /*Infinite trigger*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U16 RDBuffer0[DI_COUNT];
    U16 RDBuffer1[DI_COUNT];
    U16 BufferId0;
    U16 BufferId1;
    U32 DI_ReadCount = DI_COUNT; /*DI Read Count*/
    U16 Port = 0; /*9223 supports only one DI port, port 0*/
    F64 SampleRate = 4000;
    BOOLEAN Ready, StopFlag;
    U16 RdyTrigCnt;
    U32 AccessCnt;
    FILE *w_file;
    U8 FileName[] = "di_data.txt";
    U32 i;
    /*--------------------------------*/

    memset(RDBuffer0, '\0', DI_COUNT*sizeof(U16));
    memset(RDBuffer1, '\0', DI_COUNT*sizeof(U16));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }
    printf("This sample performs continuous DI acquisition from DI Port\n");
    printf("with infinite retrigger at %6.3lf Hz sampling rate.\n\n", SampleRate);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure DI*/
    err = DI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("DI_9223_Config Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Disable Double Buffer Mode*/
    err = DI_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("DI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Setup Buffer for DI DMA Transfer*/
    err = DI_ContBufferSetup(card, RDBuffer0, DI_COUNT, &BufferId0);
    if(err<0){
        printf("DI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    err = DI_ContBufferSetup(card, RDBuffer1, DI_COUNT, &BufferId1);
    if(err<0){
        printf("DI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*DI Acquisition Start*/
    err = DI_ContReadPort(card, Port, &BufferId0, DI_ReadCount, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DI_ContReadPort Error: %d\n", err);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    printf("\nWait Infinite Retrigger from GPI0...\n");
    printf("You can press any key to stop...\n");
    do{
        err = DI_AsyncReTrigNextReady(card, &Ready, &StopFlag, &RdyTrigCnt);
        if(err<0){
            printf("DI_AsyncReTrigNextReady Error: %d\n", err);
            DI_AsyncClear(card, &AccessCnt);
            DI_ContBufferReset(card);
            Release_Card(card);
            fclose(w_file);
            exit(1);
        }
        if(Ready){
            printf("\nDI Trigger End with Next Trigger...\n");
            printf("Trigger Ready: %d, Trigger End: %d, Trigger Count: %d\n", Ready, StopFlag, RdyTrigCnt);
            if(RdyTrigCnt==0){
                printf("Write %d samples of Buffer0 to %s file...\n", DI_ReadCount, FileName);
                for(i=0; i<DI_ReadCount; i++){
                    fprintf(w_file, "%#x,\n", RDBuffer0[i]);
                }
                memset(RDBuffer0, '\0', DI_COUNT*sizeof(U16));
            }
            else{
                printf("Write %d samples of Buffer1 to %s file...\n", DI_ReadCount, FileName);
                for(i=0; i<DI_ReadCount; i++){
                    fprintf(w_file, "%#x,\n", RDBuffer1[i]);
                }
                memset(RDBuffer1, '\0', DI_COUNT*sizeof(U16));
            }
        }
    }while(!kbhit());getch();

    /*Clear DI Setting and Get Remaining data*/
    err = DI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("DI_AsyncClear Error: %d\n", err);
        DI_AsyncClear(card, &AccessCnt);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    if(RdyTrigCnt==0){
        printf("\nDI Acquisition had been stopped manually...\n");
        printf("Write Last %d samples of Buffer0 to %s file...\n", AccessCnt, FileName);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%#x,\n", RDBuffer0[i]);
        }
    }
    else{
        printf("\nDI Acquisition had been stopped manually...\n");
        printf("Write Last %d samples of Buffer1 to %s file...\n", AccessCnt, FileName);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%#x,\n", RDBuffer1[i]);
        }
    }

    if(!AutoResetBuf)
        DI_ContBufferReset(card);
    Release_Card(card);
    fclose(w_file);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
