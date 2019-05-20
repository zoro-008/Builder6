//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Rs232Man.h"
//---------------------------------------------------------------------------
#include "FormMaster.h"
#include "SLogUnit.h"
#include "UserFile.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CRs232Man RSM ;

TRS232C *Rs232_E;
//TRS232C *Rs232_L;
//TRS232C *Rs232_R;

void CRs232Man::Dispenser(DWORD _cbInQue)
{
//    static AnsiString sRcvMsg = "" ;
    String sTemp ;
//    int    iLen = 0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//    iLen = Rs232_H->ReadData(_cbInQue, RcvBuff);
    Rs232_E->ReadData(_cbInQue, RcvBuff);
//    sRcvMsg += (char*)RcvBuff ;

    memcpy(Rs232_E->bData , RcvBuff , sizeof(Rs232_E->bData));
    Trace("procComE",(char*)RcvBuff);

}


CRs232Man::CRs232Man(void)
{
    InitDispenser();
}
CRs232Man::~CRs232Man(void)
{
    Close();
}

void CRs232Man::Close()
{
    delete Rs232_E ;
}


void CRs232Man::InitDispenser()
{
    Rs232_E = new TRS232C;

    if(!Rs232_E->Open(0)) ShowMessage("Epoxy Rs232 Port Open Fail") ;
    else                  Rs232_E->CallBackReg(Dispenser);

    ChangeCh(0);

}
void CRs232Man::Shot()
{
    BYTE RcvBuff[2];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    memset(&Rs232_E->bData, 0 , sizeof(Rs232_E->bData));

    RcvBuff[0] = 0x51 ;
    RcvBuff[1] = 0xAA ;

    Rs232_E->SendData(sizeof( RcvBuff),(char*)RcvBuff);
}
bool CRs232Man::CheckShot()
{
    String sTemp ;
    sTemp = (char*)Rs232_E->bData ;

    if(sTemp == "Q\xaa") return true ;

    return false ;
}

void CRs232Man::ChangeCh(int _iCh)
{
    BYTE RcvBuff[3];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    RcvBuff[0] = 0x55 ;
    RcvBuff[1] = _iCh ;
    RcvBuff[2] = 0xaa ;

    Rs232_E->SendData(sizeof( RcvBuff),(char*)RcvBuff);
}

void CRs232Man::SetTime(int _iTime)
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

    Rs232_E->SendData(sizeof( RcvBuff),(char*)RcvBuff);
}

