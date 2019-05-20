//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rs232Man.h"
#include "FormMaster.h"
#include "SLogUnit.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CRs232Man RSM ;

//TRS232C *Rs232_E;
TRS232C *Rs232_L;
TRS232C *Rs232_R;

void CRs232Man::DispenserL(DWORD _cbInQue)
{
    String sRcvMsg = "" ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//    iLen = Rs232_H->ReadData(_cbInQue, RcvBuff);
    Rs232_L -> ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;

    if(sRcvMsg != "") memcpy(Rs232_L->bData , RcvBuff , sizeof(Rs232_L->bData));
    Trace("DispenserL",(char*)Rs232_L->bData);

}

void CRs232Man::DispenserR(DWORD _cbInQue)
{
    String sRcvMsg = "" ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//    iLen = Rs232_H->ReadData(_cbInQue, RcvBuff);
    Rs232_R -> ReadData(_cbInQue, RcvBuff);

    sRcvMsg = (char*)RcvBuff ;

    if(sRcvMsg != "") memcpy(Rs232_R->bData , RcvBuff , sizeof(Rs232_R->bData));
    Trace("DispenserR",(char*)Rs232_R->bData);

}


CRs232Man::CRs232Man(void)
{
}
CRs232Man::~CRs232Man(void)
{
    Close();
}

void CRs232Man::Close()
{
    delete Rs232_L ;
    delete Rs232_R ;
}


void CRs232Man::Init()
{
    Rs232_L = new TRS232C;
    Rs232_R = new TRS232C;

    if(!Rs232_L->Open(0)) ShowMessage("Dispenser Left Rs232 Port Open Fail") ;
    else                  Rs232_L->CallBackReg(DispenserL);
    if(!Rs232_R->Open(1)) ShowMessage("Dispenser Right Rs232 Port Open Fail") ;
    else                  Rs232_R->CallBackReg(DispenserR);

    ChangeCh(toLeft, 0);
    ChangeCh(toRigt, 0);
    SetTime(toLeft, OM.DevOptn.iDispsrLTimer);
    SetTime(toRigt, OM.DevOptn.iDispsrRTimer);

}
void CRs232Man::Shot(int _iNum)
{
    BYTE RcvBuff[2];

    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    memset(&Rs232_L->bData, 0 , sizeof(Rs232_L->bData));
    memset(&Rs232_R->bData, 0 , sizeof(Rs232_R->bData));

    RcvBuff[0] = 0x51 ;
    RcvBuff[1] = 0xAA ;

    if(_iNum == toLeft) Rs232_L->SendData(sizeof( RcvBuff),(char*)RcvBuff);
    else                Rs232_R->SendData(sizeof( RcvBuff),(char*)RcvBuff);

}
bool CRs232Man::CheckShot(int _iNum)
{
    String sTemp ;

    if(_iNum == toLeft){
        sTemp = (char*)Rs232_L->bData ;
    }
    else{
        sTemp = (char*)Rs232_R->bData ;
    }

    if(sTemp != ""     ) return true ; //이상하게 들어올때가 있따네. TODO :: 미스테리 노트 #1
    return false ;
}

void CRs232Man::ChangeCh(int _iNum , int _iCh)
{
    BYTE RcvBuff[3];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    RcvBuff[0] = 0x55 ;
    RcvBuff[1] = _iCh ;
    RcvBuff[2] = 0xaa ;

    if(_iNum == toLeft) Rs232_L->SendData(sizeof( RcvBuff),(char*)RcvBuff);
    else                Rs232_R->SendData(sizeof( RcvBuff),(char*)RcvBuff);
}

void CRs232Man::SetTime(int _iNum , int _iTime)
{
    BYTE RcvBuff[8];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    int i1t = _iTime %10;
    int i2t = _iTime %100 /10;
    int i3t = _iTime %1000/100;
    int i4t = _iTime %10000/1000;
    int i5t = _iTime %100000/10000;

    RcvBuff[0] = 0x54 ;
    RcvBuff[1] = 0x00 ;
    RcvBuff[2] = i5t  ;
    RcvBuff[3] = i4t  ;
    RcvBuff[4] = i3t  ;
    RcvBuff[5] = i2t  ;
    RcvBuff[6] = i1t  ;
    RcvBuff[7] = 0xaa ;

    if(_iNum == toLeft) Rs232_L->SendData(sizeof( RcvBuff),(char*)RcvBuff);
    else                Rs232_R->SendData(sizeof( RcvBuff),(char*)RcvBuff);
}

