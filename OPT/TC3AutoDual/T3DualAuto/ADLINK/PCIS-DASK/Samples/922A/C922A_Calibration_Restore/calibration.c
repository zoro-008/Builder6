/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/02/05                                                       */
/*                                                                            */
/* This sample performs AI auto calibration for PCI-922A.                     */
/* There are two EEPROM banks, default bank and user bank, to store AI        */
/* calibration constants. For 922A, the driver loads calibration constants    */
/* of user bank and writes them to ADC during the period of driver loaded.    */
/* User can use PCI_DB_Auto_Calibration_ALL() to perform auto calibration,    */
/* and those auto calibration constants will be written to ADC                */
/* automatically. Then, User can use PCI_EEPROM_CAL_Constant_Update() to      */
/* store the auto calibration constants to user bank of EEPROM.               */
/* PCI_Load_CAL_Data() is used to load the calibration constants from default */
/* or user bank of EEPROM to ADC. It is useful to restore user bank constants */
/* to default.                                                                */
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
    U8 fWriteEE;

    printf("The sample restores calibration constants of user bank to default.\n");

    printf("Card Number: ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_922A, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Load Constants from Default Bank EEPROM*/
    err = PCI_Load_CAL_Data(card, EEPROM_DEFAULT_BANK);
    if(err<0){
        printf("PCI_Load_CAL_Data Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Update Read Constants to User Bank EEPROM*/
    printf("\nRestore the default calibration constants to user bank?\n");
    printf("It will rewrite the calibration constants of user bank!!\n");
    printf("Press 'Y' to store them to EEPROM, and others to exit.\n");
    fWriteEE = getch();
    if((fWriteEE=='Y')||(fWriteEE=='y')){
        printf("Store auto calibration constants to user bank of EEPROM...\n");
        err = PCI_EEPROM_CAL_Constant_Update(card, EEPROM_USER_BANK1);
        if(err<0){
            printf("Save Calibration Constants to EEProm Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\nAuto calibration constants are stored to EEPROM completely.\n");
    }

    printf("\nPress any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
