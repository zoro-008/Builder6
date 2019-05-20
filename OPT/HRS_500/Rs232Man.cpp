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

TRS232C *Rs232_H;
TRS232C *Rs232_L;
TRS232C *Rs232_R;

void CRs232Man::procComH(DWORD _cbInQue)
{
    //Ex : ID001,+000.007
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;
//    int    iLen = 0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
//    iLen = Rs232_H->ReadData(_cbInQue, RcvBuff);
    Rs232_H->ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    //상시 data 전송.
    if(sRcvMsg.Pos("ST") || sRcvMsg.Pos("UT")) { sRcvMsg = "" ; return ; }

    if(!sRcvMsg.Pos("ID001,")) return ;
    if(!sRcvMsg.Pos("\r\n"  )) return ;
    Trace("procCom1",sRcvMsg.c_str());
    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("ID001,")+6,8);
    if(TryStrToFloat(sTemp , dTemp)) Rs232_H->sData = (String)dTemp ;
    else                             Rs232_H->sData = "" ;
    sRcvMsg  = "" ;

    //정지 시켜 놓아서 컨트롤러에서 보여주기용.
    //Hold 해도 높은값은 들어옴. 이용도가 아니겟지? dd
//    sTemp = "ID01H";
//    Rs232_H->WriteData(sTemp.Length(),sTemp.c_str());

//    FrmMaster->mmRs232->Lines->Add(Rs232_H->sData);


}

void CRs232Man::procComL(DWORD _cbInQue)
{
    /* EX
NG,05,+0.000,+0.050,+0.010,+0.001,+0.001@@
OK,05,+0.001,+0.000,+0.009,+0.001,+0.001@@
NG,05,+0.072,+0.000,+0.001,+0.001,+0.000@@
NG,05,+0.000,+0.049,+0.001,+0.024,+0.001@@
NG,05,+0.001,+0.051,+0.010,+0.001,+0.001@@
    String sRcvMsg = "NG,05,+0.071,+0.048,+0.009,+0.025,+0.023@@";

    //EX
    01,01,+0002@@
    x0501,01,-2005|x03@@\r\n
    01,01,로 짜름.
    */
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    Rs232_L->ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    if(!sRcvMsg.Pos("01,01,") ) return ;
    if(!sRcvMsg.Pos("@@" ) ) return ;
//    if(!sRcvMsg.Pos("\r\n"  )) return ;
    Trace("procCom2",sRcvMsg.c_str());
    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+6,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+8,3);

    if(TryStrToFloat(sTemp , dTemp)) Rs232_L->sData = (String)dTemp ;
    else                             Rs232_L->sData = "" ;

//    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+6,5);
//    동도 이것이 소숫점을 안붙여서 오옴...아나.

//    Rs232_L->sData = sTemp ;

    sRcvMsg  = "" ;

//    FrmMaster->mmRs232->Lines->Add("Resut" + Rs232_L->sData);
}
void CRs232Man::procComR(DWORD _cbInQue)
{
    /* EX
NG,05,+0.000,+0.050,+0.010,+0.001,+0.001@@
OK,05,+0.001,+0.000,+0.009,+0.001,+0.001@@
NG,05,+0.072,+0.000,+0.001,+0.001,+0.000@@
NG,05,+0.000,+0.049,+0.001,+0.024,+0.001@@
NG,05,+0.001,+0.051,+0.010,+0.001,+0.001@@
    String sRcvMsg = "NG,05,+0.071,+0.048,+0.009,+0.025,+0.023@@";
    */
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;    

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    Rs232_R->ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    if(!sRcvMsg.Pos("01,01,") ) return ;
    if(!sRcvMsg.Pos("@@" ) ) return ;
//    if(!sRcvMsg.Pos("\r\n"  )) return ;
    Trace("procCom2",sRcvMsg.c_str());
    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+6,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+8,3);
//    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+6,5);
//    동도 이것이 소숫점을 안붙여서 오옴...아나.

    if(TryStrToFloat(sTemp , dTemp)) Rs232_R->sData = (String)dTemp ;
    else                             Rs232_R->sData = "" ;

//    Rs232_R->sData = sTemp ;

    sRcvMsg  = "" ;

//    FrmMaster->mmRs232->Lines->Add("Resut" + Rs232_R->sData);
}

CRs232Man::CRs232Man(void)
{
    Init();
}
CRs232Man::~CRs232Man(void)
{
    Close();
}
void CRs232Man::Init()
{
    Rs232_H = new TRS232C;
    Rs232_L = new TRS232C;
    Rs232_R = new TRS232C;

    if(!Rs232_H->Open(0)) ShowMessage("1  Rs232 Port Open Fail") ;
    else                  Rs232_H->CallBackReg(procComH);
    if(!Rs232_L->Open(1)) ShowMessage("2  Rs232 Port Open Fail") ;
    else                  Rs232_L->CallBackReg(procComL);
    if(!Rs232_R->Open(2)) ShowMessage("3  Rs232 Port Open Fail") ;
    else                  Rs232_R->CallBackReg(procComR);

//    ClearData();
//    memset(dRotorH , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL) ;

}

void CRs232Man::Close()
{
    delete Rs232_H ;
    delete Rs232_L ;
    delete Rs232_R ;
}


