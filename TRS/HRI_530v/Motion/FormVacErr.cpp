//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVacErr.h"

#include "Sequence.h"
#include "Sort.h"
#include "LotUnit.h"
#include "Stage.h"
//#include "SortCmn.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
/*
enum EN_VAC_ERR_STAT { //���� �� �� �ϰ� �ִ� ����.
    vsNomal            = 0 , //�������� ���� �ٽ� ���ϰ� �������� �ٽ� ����.
    vsErrSkipPick      = 1 , //���ߴ��� �ٽ� ���ϰ� ������ �׳� ���� ������ �Ѿ -> vsNomal.
    vsErrSkipRow       = 2 , //���ߴ��� �ٽ� ���ϰ� �ش� ������������ ���� ��ŵ -> vsNomal.
    vsErrSkipTray      = 3 , //���ߴ��� �ٽ� ���ϰ� �ش� Ʈ���̿����� ���� ��ŵ -> vsNomal.
    vsErrSkipRowOut    = 4 , //���ߴ��� ��ŵ���� Ȯ�� �ϰ� Ʈ���� �ƿ�.
    vsErrSkipRowOutLE  = 5 , //���ߴ��� ��ŵ���� Ȯ���ϰ� Ʈ���� �ƿ��� �������û.

    //�̰��� Ŭ���ÿ� �ٷ� ó�� �Ǽ� �ʿ� ����.  vsTrayOut          = 6 , //���� Ʈ���� ��� Empty ��Ű��.
    //�̰��� Ŭ���ÿ� �ٷ� ó�� �Ǽ� �ʿ� ����.  vsTrayOutLE        = 7 , //Ʈ���� �ƿ� ��Ű�� ���ص� ��û.

};

*/
TFrmVacErr *FrmVacErr;
//---------------------------------------------------------------------------
__fastcall TFrmVacErr::TFrmVacErr(TComponent* Owner)
    : TForm(Owner)
{


}
//---------------------------------------------------------------------------






void __fastcall TFrmVacErr::btLDRRetryClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRSkipThisPickClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SRT.m_iVacErrStat = vsErrSkipPick ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRSkipThisRowClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SRT.m_iVacErrStat = vsErrSkipRow ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRSkipThisTrayClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SRT.m_iVacErrStat = vsErrSkipTray ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRSkipThisRowTrayOutClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SRT.m_iVacErrStat = vsErrSkipRowOut ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRTrayOutClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SRT.m_iVacErrStat = vsNomal ; //vsTrayOut
    if(!DM.ARAY[riLDR].CheckAllStat(csEmpty)) DM.ARAY[riLDR].SetStat(csEmpty) ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRSkipThisRowTrayOutLotEndClick(
      TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );
    SRT.m_iVacErrStat = vsErrSkipRowOutLE ; //
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btLDRTrayOutLotEndClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR Pick - "+sBtnCaption).c_str() );

    SRT.m_iVacErrStat = vsNomal ; //vsTrayOut
    if(!DM.ARAY[riLDR].CheckAllStat(csEmpty)) DM.ARAY[riLDR].SetStat(csEmpty) ;
    LOT.SetRqstLotEnd(true);   
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btZIGRetryClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("ZIG Pick - "+sBtnCaption).c_str() );
    SRT.m_bRetryZigPick=true ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btZIGSkipClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("ZIG Pick - "+sBtnCaption).c_str() );
    DM.ARAY[riZIG].SetStat(csEmpty);
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btSTGRetryClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("STG Pick - "+sBtnCaption).c_str() );

    SRT.m_bRetryStgPick=true ;
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::btSTGSkipClick(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("STG Pick - "+sBtnCaption).c_str() );

    DM.ARAY[riSTF].SetStat(csEmpty);

    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::BitBtn1Click(TObject *Sender)
{
    String sBtnCaption = ((TBitBtn *)Sender) -> Caption ;
    Trace("VacuumErr", ("LDR SUPPLY - "+sBtnCaption).c_str() );

    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::BitBtn2Click(TObject *Sender)
{
    //LSP.m_bRqstLotEnd = true ;
    LOT.SetRqstLotEnd(true);
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVacErr::FormShow(TObject *Sender)
{
    if(PageControl1 -> ActivePageIndex == 3) {
        pnLDRVacErr1 -> Visible = false ;
        pnLDRVacErr2 -> Visible = false ;
        pnLDRVacErr3 -> Visible = false ;
        pnLDRVacErr4 -> Visible = false ;
    }
    else {
        pnLDRVacErr1 -> Visible = true ;
        pnLDRVacErr2 -> Visible = true ;
        pnLDRVacErr3 -> Visible = true ;
        pnLDRVacErr4 -> Visible = true ;
    }

    pnLDRVacErr1 -> Color = SRT.m_bVacErr[0] ? clRed : clBtnFace ;
    pnLDRVacErr2 -> Color = SRT.m_bVacErr[1] ? clRed : clBtnFace ;
    pnLDRVacErr3 -> Color = SRT.m_bVacErr[2] ? clRed : clBtnFace ;
    pnLDRVacErr4 -> Color = SRT.m_bVacErr[3] ? clRed : clBtnFace ;

}
//---------------------------------------------------------------------------

