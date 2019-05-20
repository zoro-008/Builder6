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
#include "FormProbe.h"
#include "TempConUnit.h"
#include "ProbeComUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CRs232Man RSM ;

//TRS232C *Rs232_E;

TRS232C *RS232C_TempCon;


//밑에 있는 4개 배열 나중에 주석 처리 하던가 범용성 있게 변경.....



void CRs232Man::RcvTempCon  (DWORD _cbInQue)
{
    if(!OM.MstOptn.bUsedTempRS232){
        int iLen;
        BYTE RcvBuff[300];
        AnsiString sRcvMsg = "" ;
        memset(&RcvBuff, 0 , sizeof(RcvBuff));

        iLen = RS232C_TempCon->ReadData(_cbInQue, RcvBuff);

        sRcvMsg = (char*)RcvBuff ;
        TCU.SetRcvMsg(sRcvMsg);
    }

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
    delete RS232C_TempCon;
}


void CRs232Man::Init()
{

    RS232C_TempCon = new TRS232C;

    if(!OM.MstOptn.bUsedTempRS232) {
        RS232C_TempCon = new TRS232C ;
        if(!RS232C_TempCon->Open(2)) ShowMessage("RS-232C TempCon 3 Ch Open Failed!");
        else                         RS232C_TempCon->CallBackReg(RcvTempCon);
    }
}





















































































































































































































