/*----------------------------------------------------------------------------*/
/* Date: 2007/03/10                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample uses DMA mode to acuquire AI data from AI Channel, and store   */
/* acquired data to a specified binary file.                                  */
/* You can use the data converter utility(Utility\DAQCvt.exe) to convert      */
/* the binary file to a text file.                                            */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCOUNT 4096
#define TimeOut 60000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;

    U32 Buffer1[READCOUNT];
    U32 ReadCount = READCOUNT;
    U16 BufID1;
    U8 *FileName = "acq";

    U16 Group;
    U16 XMode = P9524_AI_XFER_DMA;
    U16 ADC_Range;
    U16 ADC_SampRate;
    U16 ConfigCtrl;
    U16 TrigCtrl = 0;
    U32 dwTrigValue = 0;

    U16 Channel;
    F64 ActualRate;

    memset(Buffer1, '\0', ReadCount*4);

    printf("This sample performs Continuous AI operation to File.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Load Cell Channel[0] or General Purpose Channel[1]? [0]or[1] ");
    scanf(" %hd", &Group);
    if(Group>1){
        printf("Invalid Group\n");
        Release_Card(card);
        exit(1);
    }

    err = AI_SetTimeOut(card, TimeOut);
    if(err<0){
        printf("AI_SetTimeOut Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer1, ReadCount, &BufID1);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    if(!Group){
        /*Load Cell Group*/
        U16 DFStage = 5;
        U32 SPKRejThreshold = 268;

        Channel = P9524_AI_LC_CH0;
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode|P9524_AI_BufAutoReset;
        ADC_Range = 0;
        ADC_SampRate = P9524_ADC_30K_SPS;

        err = AI_9524_SetDSP(card, Channel, P9524_SPIKE_REJ_ENABLE, DFStage, SPKRejThreshold);
        if(err<0){
            printf("AI_9524_SetDSP Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }

        printf("\nPress any key to start AI Acquisition\n");
        getch();

        err = AI_ContReadChannelToFile(card, Channel, ADC_Range, FileName, ReadCount, (F64)ADC_SampRate, SYNCH_OP);
        if(err<0){
            printf("AI_ContReadChannelToFile Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }

        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nLoad Cell Channel 0 Acquisition Done in %f Actual Rate...\n", ActualRate);
        printf("The acquired data is stored in %s.dat\n", FileName);
    }
    else{
        /*General Purpose Group*/
        Channel = P9524_AI_GP_CH0;
        ConfigCtrl = P9524_AI_BufAutoReset;
        ADC_Range = AD_B_10_V;
        ADC_SampRate = P9524_ADC_30K_SPS;

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }

        printf("\nPress any key to start AI Acquisition\n");
        getch();

        err = AI_ContReadChannelToFile(card, Channel, ADC_Range, FileName, ReadCount, (F64)ADC_SampRate, SYNCH_OP);
        if(err<0){
            printf("AI_ContReadChannelToFile Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }

        GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
        printf("\n\nGeneral Purpose Channel 0 Acquisition Done in %f Actual Rate...\n", ActualRate);
        printf("The acquired data is stored in %s.dat\n", FileName);
    }

    if(!(ConfigCtrl&P9524_AI_BufAutoReset)){
        err = AI_ContBufferReset(card);
        if(err<0){
            printf("AI_ContBufferReset Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }

    Release_Card(card);
    printf("Press any key to exit\n");
    getch();
    return 0;
}
