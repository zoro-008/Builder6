/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/11                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition.                            */
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
    U16 TrigCtrl = P922x_DI_TRGMOD_POST|P922x_DI_TRGSRC_SOFT;
    U32 ReTriggerCount = 0; //Ignore in non-retrigger mode
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptor built in driver
    U16 RDBuffer[DI_COUNT];
    U16 BufferId;
    U32 DI_ReadCount = DI_COUNT; //DI Read Count
    U16 Port = 0; //9222 supports only one DI port, port 0
    F64 SampleRate = 2000000;
    BOOLEAN Stopped;
    U32 AccessCnt;
    FILE *w_file;
    U8 FileName[] = "di_data.txt";
    U32 i;

    memset(RDBuffer, '\0', DI_COUNT*sizeof(U16));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }
    printf("This sample performs continuous DI acquisition from DI Port\n");
    printf("at %6.3lf Hz sampling rate.\n\n", SampleRate);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure DI*/
    err = DI_9222_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("DI_9222_Config Error: %d\n", err);
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
    err = DI_ContBufferSetup(card, RDBuffer, DI_ReadCount, &BufferId);
    if(err<0){
        printf("DI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*DI Acquisition Start*/
    err = DI_ContReadPort(card, Port, &BufferId, DI_ReadCount, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DI_ContReadPort Error: %d\n", err);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    do{
        /*Check whether DI Acquisition is done*/
        err = DI_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("DI_AsyncCheck Error: %d\n", err);
            DI_AsyncClear(card, &AccessCnt);
            DI_ContBufferReset(card);
            Release_Card(card);
            fclose(w_file);
            exit(1);
        }
    }while((!kbhit())&&(!Stopped));

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

    if(Stopped){
        printf("\n\nDI Acquisition Done... Acquired %d samples...\n", AccessCnt);
        printf("Write %d samples of Buffer to %s file...\n", AccessCnt, FileName);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%#x,\n", RDBuffer[i]);
        }
    }
    else{
        printf("\n\nDI Acquisition has been stopped manually... Acquired %d samples...\n", AccessCnt);
        printf("Write %d samples of Buffer to %s file...\n", AccessCnt, FileName);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%#x,\n", RDBuffer[i]);
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
