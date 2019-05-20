/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/08/09                                                       */
/*                                                                            */
/* This sample performs X4 AB phase encode.                                   */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 GCtr;
    U16 Mode = x4_AB_Phase_Encoder;
    U16 SrcCtrl = 0; //Not used in encoder
    U16 PolCtrl = 0; //Not used in encoder
    U32 LReg1_Val = 0; //Not used in encoder
    U32 LReg2_Val = 0; //Not used in encoder
    U32 RD_Value;
    int i = 0;

    printf("This sample performs x4 AB phase mode.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Encoder Number? ");
    scanf(" %hd", &GCtr);
    GCtr = (GCtr==1)? P922x_ENCODER1:P922x_ENCODER0;

    /*Clear Encoder*/
    err = GPTC_Clear(card, GCtr);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        goto ERR_RET;
    }

    /*Setup Encoder*/
    err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        goto ERR_RET;
    }

    /*Enable Encoder*/
    printf("Press any key to enable counter...");
    getch();
    err = GPTC_Control(card, GCtr, IntENABLE, 1);
    if(err<0){
        printf("GPTC_Control(IntENABLE) Error: %d\n", err);
        goto ERR_RET;
    }

    printf("\n\nYou can press any key to stop...\n");
    do{
        /*Read Encoder*/
        err = GPTC_Read(card, GCtr, &RD_Value);
        if(err<0){
            printf("GPTC_Read Error: %d\n", err);
            goto ERR_RET2;
        }
        printf("Encoder%d Value: %11d\r", GCtr-4, RD_Value);
        Sleep(100);
    }while(!kbhit());getch();
    printf("Encoder%d Value: %11d\n", GCtr-4, RD_Value);

ERR_RET2:
    GPTC_Control(card, GCtr, IntENABLE, 0);
ERR_RET:
    GPTC_Clear(card, GCtr);
    Release_Card(card);

    printf("\n\nPress any key to exit...");
    getch();
    return 0;
}
