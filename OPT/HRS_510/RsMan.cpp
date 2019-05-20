//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RsMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#include "FormMaster.h"

#pragma package(smart_init)
TRS232C *RS232C1;
TRS232C *RS232C2;
TRS232C *RS232C3;

void CRsMan::procCom1(DWORD _cbInQue)
{
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    int    iLen = 0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = RS232C1->ReadData(_cbInQue, RcvBuff);

    sRcvMsg += (char*)RcvBuff ;

    if(!sRcvMsg.Pos("ID001,")) return ;
    if(!sRcvMsg.Pos("\r\n"  )) return ;

    sRcvMsg.Delete(sRcvMsg.Pos("ID001,"),6);
    sRcvMsg.Delete(sRcvMsg.Pos("\r\n"  ),4);

    Trace("procCom1",sRcvMsg.c_str());
    RS232C1->sData = sRcvMsg ;

    //정지 시켜 놓아서 컨트롤러에서 보여주기용.
    //Hold 해도 높은값은 들어옴. 이용도가 아니겟지? dd
    sTemp = "ID01H";
    RS232C1->WriteData(sTemp.Length(),sTemp.c_str());

    FrmMaster->mmRs232->Lines->Add(RS232C1->sData);

    sRcvMsg  = "" ;
//    RS232C1->bComplete = true ;
}

void CRsMan::procCom2(DWORD _cbInQue)
{
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    int    iLen = 0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = RS232C2->ReadData(_cbInQue, RcvBuff);

    sRcvMsg += (char*)RcvBuff ;

    if(!sRcvMsg.Pos("ID001,")) return ;
    if(!sRcvMsg.Pos("\r\n"  )) return ;

    sRcvMsg.Delete(sRcvMsg.Pos("ID001,"),6);
    sRcvMsg.Delete(sRcvMsg.Pos("\r\n"  ),4);

    Trace("procCom1",sRcvMsg.c_str());
    RS232C2->sData = sRcvMsg ;

    //정지 시켜 놓아서 컨트롤러에서 보여주기용.
    //Hold 해도 높은값은 들어옴. 이용도가 아니겟지? dd
    sTemp = "ID01H";
    RS232C2->WriteData(sTemp.Length(),sTemp.c_str());

//    FrmMaster->mmRs232->Lines->Add(RS232C1->sData);

    sRcvMsg  = "" ;
//    RS232C1->bComplete = true ;
}
void CRsMan::procCom3(DWORD _cbInQue)
{
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    int    iLen = 0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));

    iLen = RS232C3->ReadData(_cbInQue, RcvBuff);

    sRcvMsg += (char*)RcvBuff ;

    if(!sRcvMsg.Pos("ID001,")) return ;
    if(!sRcvMsg.Pos("\r\n"  )) return ;

    sRcvMsg.Delete(sRcvMsg.Pos("ID001,"),6);
    sRcvMsg.Delete(sRcvMsg.Pos("\r\n"  ),4);

    Trace("procCom1",sRcvMsg.c_str());
    RS232C3->sData = sRcvMsg ;

    //정지 시켜 놓아서 컨트롤러에서 보여주기용.
    //Hold 해도 높은값은 들어옴. 이용도가 아니겟지? dd
    sTemp = "ID01H";
    RS232C3->WriteData(sTemp.Length(),sTemp.c_str());

//    FrmMaster->mmRs232->Lines->Add(RS232C1->sData);

    sRcvMsg  = "" ;
//    RS232C1->bComplete = true ;
}

CRsMan::CRsMan(void)
{
    Init();
}
CRsMan::~CRsMan(void)
{
    Close();
}
void CRsMan::Init()
{
    RS232C1 = new TRS232C;
    RS232C2 = new TRS232C;
    RS232C3 = new TRS232C;

    if(!RS232C2->Open(0)) ShowMessage("1  Rs232 Port Open Fail") ;
    else                  RS232C1->CallBackReg(procCom1);
    if(!RS232C2->Open(1)) ShowMessage("2  Rs232 Port Open Fail") ;
    else                  RS232C2->CallBackReg(procCom1);
    if(!RS232C2->Open(2)) ShowMessage("3  Rs232 Port Open Fail") ;
    else                  RS232C3->CallBackReg(procCom1);

}

void CRsMan::Close()
{
    delete RS232C1 ;
    delete RS232C2 ;
    delete RS232C3 ;
}
