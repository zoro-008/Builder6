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
enum EN_VAC_ERR_STAT { //현재 랏 런 하고 있는 상태.
    vsNomal            = 0 , //에러났던 곳을 다시 픽하고 안찝히면 다시 에러.
    vsErrSkipPick      = 1 , //픽했던데 다시 픽하고 없으면 그냥 다음 픽으로 넘어감 -> vsNomal.
    vsErrSkipRow       = 2 , //픽했던데 다시 픽하고 해당 에러열에서는 에러 스킵 -> vsNomal.
    vsErrSkipTray      = 3 , //픽했던데 다시 픽하고 해당 트레이에서는 에러 스킵 -> vsNomal.
    vsErrSkipRowOut    = 4 , //픽했던열 스킵으로 확인 하고 트레이 아웃.
    vsErrSkipRowOutLE  = 5 , //픽했던열 스킵으로 확인하고 트레이 아웃후 랏엔드요청.

    //이것은 클릭시에 바로 처리 되서 필요 없다.  vsTrayOut          = 6 , //현재 트레이 모두 Empty 시키기.
    //이것은 클릭시에 바로 처리 되서 필요 없다.  vsTrayOutLE        = 7 , //트레이 아웃 시키고 랏앤드 요청.

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

