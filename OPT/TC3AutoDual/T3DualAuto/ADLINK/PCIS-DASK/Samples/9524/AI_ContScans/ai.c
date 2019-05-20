/*----------------------------------------------------------------------------*/
/* Date:    2007/03/10                                                        */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample performs continuous AI channel scan operation by DMA mode.     */
/* PCI-9524 has two AI group, load cell group and general purpose group.      */
/* You should configure the AI group/channels and setup Buffer before         */
/* acquired data.                                                             */
/* There are some advaned configuration releated DSP should be set while      */
/* load cell group is performing.                                             */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 4096
#define TimeOut 20000


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    FILE *fin;
    int i, j;

    U32 Buffer1[READCOUNT];
    U32 ReadCount = READCOUNT;
    U16 BufID1;

    U16 Group;
    U16 XMode = P9524_AI_XFER_DMA;
    U16 ADC_Range;
    U16 ADC_SampRate;
    U16 ConfigCtrl;
    U16 TrigCtrl = 0;
    U32 dwTrigValue = 0;

    U16 Channel;
    F64 voltageArray[READCOUNT];
    F64 ActualRate;


    memset(Buffer1, '\0', ReadCount*4);
    memset(voltageArray, '\0', ReadCount*8);

    if(!(fin=fopen("acq.dat", "w"))){
        printf("fopen error\n");
        exit(1);
    }

    printf("This sample performs Continuous AI Scans.\n");
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
        printf("Invalid Group\n");
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    err = AI_SetTimeOut(card, TimeOut);
    if(err<0){
        printf("AI_SetTimeOut Error: %d\n", err);
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer1, ReadCount, &BufID1);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(fin);
        exit(1);
    }

    if(!Group){
        /*Load Cell Group*/
        U16 DFStage = 5;
        U32 SPKRejThreshold = 268;
        /*Max Scanned channel - Scan channels from ch0 of load cell group to set max scanned channel*/
        Channel = P9524_AI_LC_CH3;
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode|P9524_AI_BufAutoReset;
        ADC_Range = 0;
        ADC_SampRate = P9524_ADC_30K_SPS;

        for(i=0; i<4; i++){
            err = AI_9524_SetDSP(card, (U16)i, P9524_SPIKE_REJ_ENABLE, DFStage, SPKRejThreshold);
            if(err<0){
                printf("AI_9524_SetDSP%d Error: %d\n", i, err);
                AI_ContBufferReset(card);
                Release_Card(card);
                fclose(fin);
                exit(1);
            }
        }

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        printf("\nPress any key to start AI Acquisition\n");
        getch();

        err = AI_ContScanChannels(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, SYNCH_OP);
        if(err<0){
            printf("AI_ContScanChannels Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nLoad Cell Group Channel Scan Done in %f Actual Rate...\n", ActualRate);

        printf("Write data to acq.dat file...\n");

        AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
        for(i=0; i<READCOUNT/4; i++){
            for(j=0; j<4; j++){
                fprintf(fin, "%10.8f, ", voltageArray[i*4+j]);
            }
            fprintf(fin, "\n");
        }
    }
    else{
        /*General Purpose Group*/
        /*Max Scanned channel - Scan channels from ch0 of general purpose group to set max scanned channel*/
        Channel = P9524_AI_GP_CH3;
        ConfigCtrl = P9524_AI_BufAutoReset;
        ADC_Range = AD_B_10_V;
        ADC_SampRate = P9524_ADC_30K_SPS;

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        printf("\nPress any key to start AI Acquisition\n");
        getch();

        err = AI_ContScanChannels(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, SYNCH_OP);
        if(err<0){
            printf("AI_ContScanChannels Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nGeneral Purpose Group Channel Scan Done in %f Actual Rate...\n", ActualRate);

        printf("Write data to acq.dat file...\n");

        AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
        for(i=0; i<READCOUNT/4; i++){
            for(j=0; j<4; j++){
                fprintf(fin, "%10.8f, ", voltageArray[i*4+j]);
            }
            fprintf(fin, "\n");
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

    Release_Card(card);
    fclose(fin);
    printf("Press any key to exit\n");
    getch();
    return 0;
}