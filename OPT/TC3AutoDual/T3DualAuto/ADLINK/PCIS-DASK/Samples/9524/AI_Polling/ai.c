/*-----------------------------------------------------------------------------*/
/* Date:    2007/02/19                                                         */
/* Company: AdLINK Technology Inc.                                             */
/*                                                                             */
/* This sample acquires voltage value from AI channel.                         */
/* PCI-9524 has two AI group, load cell group and general purpose group.       */
/* You must configure the AI group by calling AI_9524_Config function.         */
/* AI_9524_PollConfig function should be called to start ADC while the         */
/* AI transfer mode is set to polling mode.                                    */
/* Some advanced setting related DSP is required while load cell group is      */
/* performing.                                                                 */
/*-----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"


int main(int argc, char **argv)
{
    U16 card_num;
    I16 card, err;
    U16 Group;
    U16 XMode = P9524_AI_XFER_POLL;
    U16 ConfigCtrl;
    U16 PollChannel;
    U16 PollRange;
    U16 PollSpeed;
    F64 voltage;
    U32 Value;
    F64 ActualRate;

    printf("This sample performs AI Polling operation.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register Card Failed: %d\n", card);
        exit(1);
    }

    printf("Load Cell Channel[0] or General Purpose Channel[1]? [0]or[1] ");
    scanf(" %hd", &Group);
    if(Group>1){
        printf("Invalid group\n");
        Release_Card(card);
        exit(1);
    }

    if(Group==0){
        /*Load Cell Group*/
        U16 DFStage = 2;
        U32 SPKRejThreshold = 16;

        PollChannel = P9524_AI_LC_CH0;
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode;
        PollRange = 0;
        PollSpeed = P9524_ADC_60_SPS;

        err = AI_9524_SetDSP(card, PollChannel, P9524_SPIKE_REJ_ENABLE, DFStage, SPKRejThreshold);
        if(err<0){
            printf("AI_9524_SetDSP Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, 0, 0);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

        err = AI_9524_PollConfig(card, Group, PollChannel, PollRange, PollSpeed);
        if(err<0){
            printf("AI_9524_PollConfig Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

        err = GetActualRate_9524(card, Group, PollSpeed, &ActualRate);
        if(err<0)
            printf("GetActualRate_9524 Error: %d\n", err);
        printf("\nADC started at speed: %7.2lf SPS\n", ActualRate);
        printf("Press any key to start read AI channel...\n");
        getch();

        do{
            Sleep(100);
            err = AI_ReadChannel32(card, PollChannel, PollRange, &Value);
            if(err<0){
                printf("AI_ReadChannel32 Error: %d\n", err);
                Release_Card(card);
                exit(1);
            }
            AI_VoltScale32(card, PollRange, Value, &voltage);
            /*You can also replace AI_ReadChannel32() and AI_VoltScale32() by AI_VReadChannel()*/
            /*err = AI_VReadChannel(card, PollChannel, PollRange, &voltage);*/
            system("cls");
            printf("/*---------------------------------*/\n");
            printf("Load Cell Group - Channel 0\n\n");
            printf("raw data: 0x%x\n", Value);
            printf("scaled voltage: %lf\n\n", voltage);
            printf("            press any key to exit...\n");
            printf("/*---------------------------------*/\n");
        }while(!kbhit());
    }
    else{
        /*General Purpose Group*/
        PollChannel = P9524_AI_GP_CH0;
        ConfigCtrl = 0;
        PollRange = AD_B_10_V;
        PollSpeed = P9524_ADC_60_SPS;

        err = AI_9524_Config(card, Group, XMode, ConfigCtrl, 0, 0);
        if(err<0){
            printf("AI_9524_Config Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

        err = AI_9524_PollConfig(card, Group, PollChannel, PollRange, PollSpeed);
        if(err<0){
            printf("AI_9524_PollConfig Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

        err = GetActualRate_9524(card, Group, PollSpeed, &ActualRate);
        if(err<0)
            printf("GetActualRate_9524 Error: %d\n", err);
        printf("\nADC started at speed: %7.2lf SPS\n", ActualRate);
        printf("Press any key to start read AI channel...\n");
        getch();

        do{
            Sleep(100);
            err = AI_ReadChannel32(card, PollChannel, PollRange, &Value);
            if(err<0){
                printf("AI_ReadChannel32 Error: %d\n", err);
                Release_Card(card);
                exit(1);
            }
            AI_VoltScale32(card, PollRange, Value, &voltage);
            /*You can also replace AI_ReadChannel32() and AI_VoltScale32() by AI_VReadChannel()*/
            /*err = AI_VReadChannel(card, PollChannel, PollRange, &voltage);*/
            system("cls");
            printf("/*---------------------------------*/\n");
            printf("General Purpose Group - Channel 0\n\n");
            printf("raw data: 0x%x\n", Value);
            printf("scaled voltage: %lf\n\n", voltage);
            printf("            press any key to exit...\n");
            printf("/*---------------------------------*/\n");
        }while(!kbhit());
    }

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
