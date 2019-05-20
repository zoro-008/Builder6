/*----------------------------------------------------------------------------*/
/* Date:    2007/03/10                                                        */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample scans AI channels by DMA mode with external trigger.           */
/* Trigger source is set to QDecoder0 and PulseGen0.                          */
/* The decoder decodes X4 AB Phase, and the timer/counter generates           */
/* cw/ccw or clk/dir pulse.                                                   */
/* If trigger source is set to QD0, the trigger comes while the decoder       */
/* decodes to the indicated step.                                             */
/* If trigger source is set to PG0, the trigger comes while the timer/        */
/* counter generates the indicated number of pulses.                          */
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
#include <process.h>
#include "dask.h"

#define READCOUNT 4096

I16 card;

int Start_PulseGen()
{
    I16 err;
    U16 Pulse_Type = P9524_PulseGen_CCW;
    U32 Pulse_Init = 200000;
    U32 Pulse_Len = 200000;
    U32 Pulse_Count = 0;

    err = GPTC_Clear(card, P9524_CTR_PG0);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        return 0;
    }

    /*Generate CW/CCW Pulse of Period of (Pulse_Init+Pulse_Len)/40000000 sec.*/
    err = GPTC_Setup(card, P9524_CTR_PG0, Pulse_Type, 0, 0, Pulse_Init, Pulse_Len);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        return 0;
    }

    /*Generate infinite CW/CCW Pulses*/
    err = GPTC_9524_PG_Config(card, P9524_CTR_PG0, Pulse_Count);
    if(err<0){
        printf("GPTC_9524_PG_Config Error: %d\n", err);
        return 0;
    }

    /*Enable Timer/Counter*/
    err = GPTC_Control(card, P9524_CTR_PG0, P9524_CTR_Enable, 1);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        return 0;
    }

    return 1;
}


int Stop_PulseGen()
{
    I16 err;

    err = GPTC_Control(card, P9524_CTR_PG0, P9524_CTR_Enable, 0);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        return 0;
    }

    err = GPTC_Clear(card, P9524_CTR_PG0);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        return 0;
    }
    return 1;
}


int Start_Decoder()
{
    I16 err;

    err = GPTC_Clear(card, P9524_CTR_QD0);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        return 0;
    }

    /*Setup decoder to decode x4 AB Phase Pulse*/
    err = GPTC_Setup(card, P9524_CTR_QD0, P9524_x4_AB_Phase_Decoder, 0, 0, 0, 0);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        return 0;
    }

    /*Enable Decoder*/
    err = GPTC_Control(card, P9524_CTR_QD0, P9524_CTR_Enable, 1);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        return 0;
    }

    return 1;
}


int Stop_Decoder()
{
    I16 err;

    err = GPTC_Control(card, P9524_CTR_QD0, P9524_CTR_Enable, 0);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        return 0;
    }

    err = GPTC_Clear(card, P9524_CTR_QD0);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        return 0;
    }
    return 1;
}


int main(int argc, char **argv)
{
    I16 err;
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
    U16 TrigCtrl;
    U32 dwTrigValue;

    U16 Channel;
    F64 voltageArray[READCOUNT];
    F64 ActualRate;

    BOOLEAN Stopped = 0;
    U32 AccessCnt;

    memset(Buffer1, '\0', ReadCount*4);
    memset(voltageArray, '\0', ReadCount*8);

    if(!(fin=fopen("acq.dat", "w"))){
        printf("fopen error\n");
        exit(1);
    }

    printf("This sample performs Continuous AI operation.\n");
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
        Channel = P9524_AI_LC_CH3; /*Scans from ch0 to ch3 of load cell group*/
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode|P9524_AI_BufAutoReset;
        TrigCtrl = P9524_TRGMOD_POST|P9524_TRGSRC_QD0;
        // If the trigger source is set to QDecoder0, the trigger is comming
        // while the decoder decodes the (dwTrigValue)th step.
        dwTrigValue = 100;
        ADC_Range = 0;
        ADC_SampRate = P9524_ADC_30K_SPS;

        for(i=0; i<4; i++){
            err = AI_9524_SetDSP(card, (U16)i, P9524_SPIKE_REJ_ENABLE, DFStage, SPKRejThreshold);
            if(err<0){
                printf("AI_9524_SetDSP Error: %d\n", err);
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

        err = AI_ContScanChannels(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
        if(err<0){
            printf("AI_ContScanChannels Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

        /*Start QD*/
		printf("\nStart Decoder 0...");
        if(!Start_Decoder()){
            printf("Start_Decoder Error\n");
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

		printf("\nWait Decoder 0 decodes to 100 to generate AI trigger...\n");
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

        err = AI_AsyncClear(card, &AccessCnt);
        if(err<0){
            printf("AI_AsyncClear Error: %d\n", err);
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

        Stop_Decoder();
    }
    else{
        /*General Purpose Group*/
        Channel = P9524_AI_GP_CH3; /*Scans from ch0 to ch3 of load cell group*/
        ConfigCtrl = P9524_AI_BufAutoReset;
        TrigCtrl = P9524_TRGMOD_POST|P9524_TRGSRC_PG0;
        dwTrigValue = 500;
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

        err = AI_ContScanChannels(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
        if(err<0){
            printf("AI_ContScanChannels Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }

		printf("\nStart GPTC 0...");
        /*Start Pulse Generation*/
        if(!Start_PulseGen()){
            printf("Start_Encoder Error\n");
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(fin);
            exit(1);
        }
		printf("\nWait GPTC 0 generates 500 pulse to generate AI trigger...\n");

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

        err = AI_AsyncClear(card, &AccessCnt);
        if(err<0){
            printf("AI_AsyncClear Error: %d\n", err);
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
                fprintf(fin, "%10.8f, ", voltageArray[i*(Channel-4+1)+j]);
            }
            fprintf(fin, "\n");
        }

        Stop_PulseGen();
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
