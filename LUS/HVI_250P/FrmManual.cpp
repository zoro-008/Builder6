//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmManual.h"
#include "FrmMain.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "SMInterfaceUnit.h"
#include "LotUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "Work.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormManual *FormManual;
//---------------------------------------------------------------------------
__fastcall TFormManual::TFormManual(TComponent* Owner)
    : TForm(Owner)
{
    btMan1_1  -> Tag = (int)mcPRB_Home             ; btMan1_1  -> Visible = true ;
    btMan1_2  -> Tag = (int)mcPRB_FlpNormal        ; btMan1_2  -> Visible = true ;
    btMan1_3  -> Tag = (int)mcPRB_FlpInverse       ; btMan1_3  -> Visible = true ;

    btMan1_13 -> Tag = (int)mcPRB_IdxUp            ; btMan1_13 -> Visible = true ;
    btMan1_14 -> Tag = (int)mcPRB_IdxDn            ; btMan1_14 -> Visible = true ;
    btMan1_15 -> Tag = (int)mcPRB_IdxWait          ; btMan1_15 -> Visible = true ;
    btMan1_16 -> Tag = (int)mcPRB_IdxStrip         ; btMan1_16 -> Visible = true ;
    btMan1_17 -> Tag = (int)mcPRB_IdxOut           ; btMan1_17 -> Visible = true ;

    btMan2_1  -> Tag = (int)mcPRI_Home             ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Tag = (int)mcPRI_XIdxWait         ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Tag = (int)mcPRI_XIdxWork         ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Tag = (int)mcPRI_XIdxOut          ; btMan2_4  -> Visible = true ;
    btMan2_5  -> Tag = (int)mcPRI_IdxUp            ; btMan2_5  -> Visible = true ;
    btMan2_6  -> Tag = (int)mcPRI_IdxDn            ; btMan2_6  -> Visible = true ;

    btMan2_7  -> Tag = (int)mcWRK_Home             ; btMan2_7  -> Visible = true ;
    btMan2_8  -> Tag = (int)mcWRK_XSrtBin1         ; btMan2_8  -> Visible = true ;
    btMan2_9  -> Tag = (int)mcWRK_XSrtBin2         ; btMan2_9  -> Visible = true ;
    btMan2_10 -> Tag = (int)mcWRK_XSrtBin3         ; btMan2_10 -> Visible = true ;
    btMan2_11 -> Tag = (int)mcWRK_XSrtBin4         ; btMan2_11 -> Visible = true ;
    btMan2_12 -> Tag = (int)mcWRK_XSrtBin5         ; btMan2_12 -> Visible = true ;
    btMan2_13 -> Tag = (int)mcWRK_PunchUp          ; btMan2_13 -> Visible = true ;

    btMan2_14 -> Tag = (int)mcPSI_Home             ; btMan2_14 -> Visible = true ;
    btMan2_15 -> Tag = (int)mcPSI_XIdxWait         ; btMan2_15 -> Visible = true ;
    btMan2_16 -> Tag = (int)mcPSI_XIdxPull         ; btMan2_16 -> Visible = true ;
    btMan2_17 -> Tag = (int)mcPSI_XIdxBack         ; btMan2_17 -> Visible = true ;
    btMan2_18 -> Tag = (int)mcPSI_XIdxOut          ; btMan2_18 -> Visible = true ;
    btMan2_19 -> Tag = (int)mcPSI_IdxUp            ; btMan2_19 -> Visible = true ;
    btMan2_20 -> Tag = (int)mcPSI_IdxDn            ; btMan2_20 -> Visible = true ;

    btnInit   -> Tag = (int)mcAllHome ;
}

//---------------------------------------------------------------------------
void __fastcall TFormManual::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);

}
//---------------------------------------------------------------------------

void __fastcall TFormManual::btLotEndClick(TObject *Sender)
{
    //이상시에 데이터 클리어 하기 위해.. 그냥 타게 함.
    //if(!LT.m_bLotOpen) {FM_MsgOk("ERROR" , "Can't Use While Work Closed"); return ;}

    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    String sLotNo = LT.GetCrntLot() ;
    Trace("LotEnd",sLotNo.c_str());
    LT.LotEnd();
    DM.ClearMap();

//    MM.SetManCycle(mcETC_ManLotEnd);

//    PSB.m_bLotEnded = true ;

//    memcpy(&PSB.EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

    LT.LotInfo.dEndTime = Now();
    LT.WriteLotLog   ();
    LT.WriteLotDayLog();
    Trace("WriteLotLog",sLotNo.c_str());

}
//---------------------------------------------------------------------------

void __fastcall TFormManual::FormShow(TObject *Sender)
{
//    PageControl->ActivePageIndex = 0;
    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------

void __fastcall TFormManual::btnInitClick(TObject *Sender)
{
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------

void __fastcall TFormManual::tmUpdateTimerTimer(TObject *Sender)
{
    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;
    bool       Temp = false;

    for(int i = 1 ; i < 3/*페이지5개*/ ; i++) {
        for(int j = 1 ; j < 25 /*버튼24개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;
            if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
            else                             pBtn -> Font -> Color = clBlack ;
        }
    }
    if(btnInit -> Tag == MM.GetManNo()) btnInit -> Font -> Color = clLime  ;
    else                                btnInit -> Font -> Color = clBlack ;


}
//---------------------------------------------------------------------------

void __fastcall TFormManual::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormManual::FormHide(TObject *Sender)
{
    tmUpdate->Enabled = false ;
}
//---------------------------------------------------------------------------

