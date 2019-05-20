/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/02/05                                                       */
/*                                                                            */
/* This sample performs AI acquisition with polling mode.                     */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-922A. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

#define READCHNCNT 4

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 NumChans = READCHNCNT; //AI Channel Count to be read
    U16 Chans[READCHNCNT] = {0, 8, 16, 24}; //AI Channels Number to be read
    U16 AdRanges[READCHNCNT] = {AD_B_10_V, AD_B_5_V, AD_B_10_V, AD_B_5_V}; //AI Range to be performed
    U16 Buffer[READCHNCNT]; //Buffer to be stored read data
    F64 Voltage; //Converted voltage data
    U32 vi;

    printf("This sample acquires analog voltage from %d AI Channels with polling mode.\n", NumChans);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_922A, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        err = AI_ReadMultiChannels(card, NumChans, Chans, AdRanges, Buffer);
        if(err<0){
            printf("AI_ReadChannel Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        system("cls");
        printf("------------------------------------\n");
        for(vi=0; vi<NumChans; vi++){
            AI_VoltScale(card, AdRanges[vi], Buffer[vi], &Voltage);
            printf("  Channel %d :\n", Chans[vi]);
            printf("  Acquired Raw Data : 0x%x\n", Buffer[vi]);
            printf("  Scaled Voltage : %8.6lf\n\n", Voltage);
        }
        printf("\n          Press any key to exit...\n");
        printf("------------------------------------\n");
        Sleep(100);
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
