/*----------------------------------------------------------------------------*/
/* Date:    2008/03/10                                                        */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample performs continuous AI operation with External Digital Trigger.*/
/* The external digital trigger pin is bit 0 of DI port.                      */
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


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    FILE *fin;
    int i;

    U32 Buffer1[READCOUNT];
    U32 ReadCount = READCOUNT;
    U16 BufID1;

    U16 Group;
    U16 XMode = P9524_AI_XFER_DMA;
    U16 ADC_Range;
    U16 ADC_SampRate;
    U16 ConfigCtrl;
    U16 TrigCtrl;
    U32 dwTrigValue = 0;

    U16 Channel;
    BOOLEAN Stopped = FALSE;
    U32 AccessCnt;
    F64 voltageArray[READCOUNT];
    F64 ActualRate;

    memset(Buffer1, '\0', ReadCount*4);
    memset(voltageArray, '\0', ReadCount*8);

    if(!(fin=fopen("acq.dat", "w"))){
        printf("fopen error\n");
        exit(1);
    }

    printf("This sample performs Continuous AI operation with External Digital Trigger.\n");
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

        ADC_Range = 0;
		ADC_SampRate = P9524_ADC_30K_SPS;
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode|P9524_AI_BufAutoReset;
        TrigCtrl = P9524_TRGMOD_POST|P9524_TRGSRC_ExtD|P9524_AI_TrgNegative;
        Channel = P9524_AI_LC_CH0;

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

        printf("\nPress any key to wait external digital falling trigger\n");
        getch();

        err = AI_ContReadChannel(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
        if(err<0){
            printf("AI_ContReadChannel Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        // If AsyncMode is set, you should use AI_AsyncCheck() to check if the
        // AI acquisition is done.
        // Note that AI_AsyncClear() should be called to clear AI setting.
        do{
            err = AI_AsyncCheck(card, &Stopped, &AccessCnt);
            if(err<0){
                printf("AI_AsyncCheck Error: %d\n", err);
                AI_ContBufferReset(card);
                Release_Card(card);
                fclose(fin);
                exit(1);
            }
        }while((!Stopped)&&(!kbhit()));

        /*Clear and reset AI setting*/
        err = AI_AsyncClear(card, &AccessCnt);
        if(err<0){
            printf("AI_AsyncCheck Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        /*Get Actual ADC Speed*/
        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nLoad Cell Channel 0 Acquisition Done in %f Actual Rate...\n", ActualRate);

        printf("Write data to acq.dat file...\n");

        /*Scale Raw Data to related Voltage Value*/
        AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
        fprintf(fin, "Scaled Voltage Value\n\n");
        for(i=0; i<READCOUNT; i++){
            fprintf(fin, "%13.9f\n", voltageArray[i]);
        }
    }
    else{
        ADC_Range = AD_B_10_V;
		ADC_SampRate = P9524_ADC_30K_SPS;
        ConfigCtrl = P9524_AI_BufAutoReset;
        TrigCtrl = P9524_TRGMOD_POST|P9524_TRGSRC_ExtD;
        Channel = P9524_AI_GP_CH0;

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(0);
        }

        printf("\nPress any key to wait external digital rising trigger\n");
        getch();

        err = AI_ContReadChannel(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
        if(err<0){
            printf("AI_ContReadChannel Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        // If AsyncMode is set, you should use AI_AsyncCheck() to check if the
        // AI acquisition is done.
        // Note that AI_AsyncClear() should be called to clear AI setting.
        do{
            err = AI_AsyncCheck(card, &Stopped, &AccessCnt);
            if(err<0){
                printf("AI_AsyncCheck Error: %d\n", err);
                AI_ContBufferReset(card);
                Release_Card(card);
                fclose(fin);
                exit(1);
            }
        }while((!Stopped)&&(!kbhit()));

        /*Clear and reset AI setting*/
        err = AI_AsyncClear(card, &AccessCnt);
        if(err<0){
            printf("AI_AsyncCheck Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        /*Get Actual ADC Speed*/
        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nGeneral Purpose Channel 0 Acquisition Done in %f Actual Rate...\n", ActualRate);

        printf("Write data to acq.dat file...\n");

        /*Scale Raw Data to related Voltage Value*/
        AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
        fprintf(fin, "Scaled Voltage Value\n\n");
        for(i=0; i<READCOUNT; i++){
            fprintf(fin, "%13.9f\n", voltageArray[i]);
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
