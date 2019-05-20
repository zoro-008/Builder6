/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/07/25                                                       */
/*                                                                            */
/* This sample performs DI read ans DO read/write with polling mode.          */
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
    U32 DO_Write = 0xffff;
    U32 DO_Read, DI_Read;

    printf("This sample write digital data to DO port and read digital value from DI port.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        /*Write DO port*/
        err = DO_WritePort(card, 0, DO_Write);
        if(err<0){
            printf("DO_WritePort Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        /*Read the written data from DO port*/
        err = DO_ReadPort(card, 0, &DO_Read);
        if(err<0){
            printf("DO_ReadPort Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        /*Read DI Port*/
        err = DI_ReadPort(card, 0, &DI_Read);
        if(err<0){
            printf("DI_ReadPort Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        system("cls");
        printf("/*--------------------------------*/\n");
        printf("  DO Read Data : %#x\n", DO_Read);
        printf("  DI Read Data : %#x\n", DI_Read);
        printf("\n\tPress any key to exit...\n");
        printf("/*--------------------------------*/\n");

        DO_Write = (DO_Write==0)? 0xffff: (DO_Write-1);
        Sleep(100);
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
