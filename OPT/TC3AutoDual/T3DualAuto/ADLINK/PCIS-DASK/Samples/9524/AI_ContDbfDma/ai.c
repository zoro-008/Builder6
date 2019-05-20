/*----------------------------------------------------------------------------*/
/* Date: 2007/03/10                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample setups two system buffer to perform infinitely continuous AI   */
/* read.                                                                      */
/* Step1. Setup two buffers and configure AI                                  */
/* Step2. Start AI                                                            */
/* Step3. Wait Buffer 0 Full                                                  */
/* Step4. You can process the data of buffer while buffer 0 is full, since    */
/*        the DMA chip accesses buffer 1 at that time. Note that you should   */
/*        complete your data process before the next buffer (Buffer 1) is     */
/*        full(Since Buffer 1 is full, the DMA chip accesses Buffer 0).       */
/* Step5. Wait Buffer 1 Full                                                  */
/* Step6. You can process the data of buffer while buffer 1 is full, since    */
/*        the DMA chip accesses buffer 0 at that time. Note that you should   */
/*        complete your data process before the next buffer (Buffer 0) is     */
/*        full(Since Buffer 0 is full, the DMA chip accesses Buffer 1).       */
/* Step7. repeat Step3 to Step6                                               */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 60

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    FILE *fin;
    unsigned int i;

    U32 ReadCount = READCOUNT;
    U32 Buffer1[READCOUNT];
    U16 BufID1;
    U32 Buffer2[READCOUNT];
    U16 BufID2;

    U16 Group;
    U16 XMode = P9524_AI_XFER_DMA;
    U16 ADC_Range;
    U16 ADC_SampRate;
    U16 ConfigCtrl;
    U16 TrigCtrl = 0;
    U32 dwTrigValue = 0;

    U16 Channel;
    BOOLEAN Stopped = 0;
    BOOLEAN HalfReady = 0;
    U32 AccessCnt;
    int viewidx = 0;
    U16 overrunFlag = 0;
    F64 ActualRate;
    F64 voltageArray[READCOUNT];

    memset(Buffer1, '\0', ReadCount*4);
    memset(Buffer2, '\0', ReadCount*4);
    memset(voltageArray, '\0', ReadCount*8);

    if(!(fin=fopen("acq.dat", "w"))){
        printf("fopen error\n");
        exit(1);
    }

    printf("This sample performs Continuous AI operation by Double-Buffer Mode.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(fin);
        exit(1);
    }

    printf("Load Cell Channel[0] or General Purpose Channel[1]? [0]or[1] ");
    scanf(" %hd", &Group);
    if(Group>1){
        printf("invalid group\n");
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    err = AI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    /*In Double Buffer Mode, you should setup two buffers*/
    err = AI_ContBufferSetup(card, Buffer1, ReadCount, &BufID1);
    if(err<0){
        printf("AI_ContBufferSetup1 Error: %d\n", err);
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer2, ReadCount, &BufID2);
    if(err<0){
        printf("AI_ContBufferSetup2 Error: %d\n", err);
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    if(!Group){
    /*Load Cell Group*/
        /*Set DSP - it is necessary fot Load Cell Group*/
        U16 DFStage = 2;
        U32 SPKRejThreshold = 16;

        Channel = P9524_AI_LC_CH0;
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode|P9524_AI_BufAutoReset;
        ADC_Range = 0;
        ADC_SampRate = P9524_ADC_60_SPS;

        err = AI_9524_SetDSP(card, Channel, P9524_SPIKE_REJ_ENABLE, DFStage, SPKRejThreshold);
        if(err<0){
            printf("AI_9524_SetDSP Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(0);
        }

        printf("\nPress any key to start AI Infinite Acquisition\n");
        getch();
        printf("\nYou can press any key to stop...\n\n");

        err = AI_ContReadChannel(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
        if(err<0){
            printf("AI_ContReadChannel Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        /*Process Double Buffer Mode*/
        do{
            err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &Stopped);
            if(err<0){
                printf("AI_AsyncCheck Error: %d\n", err);
                AI_ContBufferReset(card);
                Release_Card(card);
                fclose(fin);
                exit(1);
            }
            if(HalfReady){
                if(!viewidx){
                    printf("Buffer 1 HalfReady, Process the data of Buffer 1\n");
                    /*Process buffer 1 data*/
                    AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
                    for(i=0; i<ReadCount; i++){
                        fprintf(fin, "%13.9f\n", voltageArray[i]);
                    }
                    viewidx = 1;
                    /*Tell the driver you complete the buffer 1 process*/
                    err = AI_AsyncDblBufferHandled(card);
                    if(err<0){
                        printf("AI_AsyncDblBufferHandled1 Error: %d\n", err);
                    }
                }
                else{
                    /*Process buffer 2 data*/
                    printf("Buffer 2 HalfReady, Process the data of Buffer 2\n");
                    AI_ContVScale(card, ADC_Range, Buffer2, voltageArray, ReadCount);
                    for(i=0; i<ReadCount; i++){
                        fprintf(fin, "%13.9f\n", voltageArray[i]);
                    }
                    viewidx = 0;
                    /*Tell the driver you complete the buffer 2 process*/
                    err = AI_AsyncDblBufferHandled(card);
                    if(err<0){
                        printf("AI_AsyncDblBufferHandled2 Error: %d\n", err);
                    }
                }

                // This function can check if the overrun occurs. If the
                // function is called, AI_AsyncDblBufferHandled() should
                // be called to let driver to know user buffer is processed
                // completely
                err = AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
                if(err<0){
                    printf("AI_AsyncDblBufferOverrun Error: %d\n", err);
                }
                if(overrunFlag){
                    printf("OVERRUN: %d, Lose Some data\n", overrunFlag);
                    AI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
                }
            }
        }while((!Stopped)&&(!kbhit())); getch();

        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nLoad Cell Channel 0 Acquisition Done in %f Actual Rate...\n", ActualRate);
        printf("The acquired data stored in acq.dat\n");

        /*Clear AI setting while existing*/
        err = AI_AsyncClear(card, &AccessCnt);
        if(err<0){
            printf("AI_AsyncClear Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }
    }
    else{ //General Purpose Group
    /*General Purpose Group*/
        Channel = P9524_AI_GP_CH0;
        ConfigCtrl = P9524_AI_BufAutoReset;
        ADC_Range = AD_B_10_V;
        ADC_SampRate = P9524_ADC_60_SPS;

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(0);
        }

        printf("\nPress any key to start AI Infinite Acquisition\n");
        getch();
        printf("\nYou can press any key to stop...\n\n");

        err = AI_ContReadChannel(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
        if(err<0){
            printf("AI_ContReadChannel Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        /*Process Double Buffer Mode*/
        do{
            err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &Stopped);
            if(err<0){
                printf("AI_AsyncCheck Error: %d\n", err);
                AI_ContBufferReset(card);
                Release_Card(card);
                fclose(fin);
                exit(1);
            }
            if(HalfReady){
                if(!viewidx){
                    printf("Buffer 1 HalfReady, Process the data of Buffer 1\n");
                    /*Process buffer 1 data*/
                    AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
                    for(i=0; i<ReadCount; i++){
                        fprintf(fin, "%13.9f\n", voltageArray[i]);
                    }
                    viewidx = 1;
                    /*Tell the driver you complete the buffer 1 process*/
                    err = AI_AsyncDblBufferHandled(card);
                    if(err<0){
                        printf("AI_AsyncDblBufferHandled1 Error: %d\n", err);
                    }
                }
                else{
                    /*Process buffer 2 data*/
                    printf("Buffer 2 HalfReady, Process the data of Buffer 2\n");
                    AI_ContVScale(card, ADC_Range, Buffer2, voltageArray, ReadCount);
                    for(i=0; i<ReadCount; i++){
                        fprintf(fin, "%13.9f\n", voltageArray[i]);
                    }
                    viewidx = 0;
                    /*Tell the driver you complete the buffer 2 process*/
                    err = AI_AsyncDblBufferHandled(card);
                    if(err<0){
                        printf("AI_AsyncDblBufferHandled2 Error: %d\n", err);
                    }
                }

                // This function can check if the overrun occurs. If the
                // function is called, AI_AsyncDblBufferHandled() should
                // be called to let driver to know user buffer is processed
                // completely
                err = AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
                if(err<0){
                    printf("AI_AsyncDblBufferOverrun Error: %d\n", err);
                }
                if(overrunFlag){
                    printf("OVERRUN: %d, Lose Some data\n", overrunFlag);
                    AI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
                }
            }
        }while((!Stopped)&&(!kbhit())); getch();

        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nGeneral Purpose Channel 0 Acquisition Done in %f Actual Rate...\n", ActualRate);
        printf("The acquired data stored in acq.dat\n");

        /*Clear AI setting while existing*/
        err = AI_AsyncClear(card, &AccessCnt);
        if(err<0){
            printf("AI_AsyncClear Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }
    }

    if(!(ConfigCtrl&P9524_AI_BufAutoReset)){
        err = AI_ContBufferReset(card);
        if(err<0){
            printf("AI_ContBufferReset Error: %d\n", err);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }
    }

    printf("\npress any key to exit...\n");
    getch();
    Release_Card(card);
    fclose(fin);

    return 0;
}
