//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmManual.h"
#include "FrmMain.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Loader.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "SMInterfaceUnit.h"
#include "LotUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormManual *FormManual;
//---------------------------------------------------------------------------
__fastcall TFormManual::TFormManual(TComponent* Owner)
    : TForm(Owner)
{
    btMan1_1  -> Tag = (int)mcLDR_Home              ; btMan1_1  -> Visible = true ;
    btMan1_2  -> Tag = (int)mcLDR_UnClamp           ; btMan1_2  -> Visible = true ;
    btMan1_3  -> Tag = (int)mcLDR_Clamp             ; btMan1_3  -> Visible = true ;
    btMan1_4  -> Tag = (int)mcLDR_PusherFw          ; btMan1_4  -> Visible = true ;
    btMan1_5  -> Tag = (int)mcLDR_PusherBw          ; btMan1_5  -> Visible = true ;
    btMan1_6  -> Tag = (int)mcLDR_TransferInAC      ; btMan1_6  -> Visible = true ;
    btMan1_7  -> Tag = (int)mcLDR_TransferOutAC     ; btMan1_7  -> Visible = true ;
    btMan1_8  -> Tag = (int)mcLDR_Supply            ; btMan1_8  -> Visible = true ;
    btMan1_9  -> Tag = (int)mcLDR_Pick              ; btMan1_9  -> Visible = true ;
    btMan1_10 -> Tag = (int)mcLDR_Place             ; btMan1_10 -> Visible = true ;
    btMan1_11 -> Tag = (int)mcLDR_Z1PichDn          ; btMan1_11 -> Visible = true ;
    btMan1_12 -> Tag = (int)mcLDR_Z1PichUp          ; btMan1_12 -> Visible = true ;
    btMan1_13 -> Tag = (int)mcLDR_SttPstn           ; btMan1_13 -> Visible = true ;
    btMan1_14 -> Tag = (int)mcRAL_PreStprUp         ; btMan1_14 -> Visible = true ;
    btMan1_15 -> Tag = (int)mcRAL_PreStprDn         ; btMan1_15 -> Visible = true ;

    btMan2_1  -> Tag = (int)mcRAL_Wk1Home           ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Tag = (int)mcRAL_Wk1CmpUp          ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Tag = (int)mcRAL_Wk1CmpDn          ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Tag = (int)mcRAL_Wk1StprUp         ; btMan2_4  -> Visible = true ;
    btMan2_5  -> Tag = (int)mcRAL_Wk1StprDn         ; btMan2_5  -> Visible = true ;
    btMan2_6  -> Tag = (int)mcRAL_Wk1AlignFB        ; btMan2_6  -> Visible = true ;
    btMan2_7  -> Tag = (int)mcRAL_Wk1NomalPos       ; btMan2_7  -> Visible = true ;
    btMan2_8  -> Tag = (int)mcRAL_Wk1ReversePos     ; btMan2_8  -> Visible = true ;
    btMan2_9  -> Tag = (int)mcRAL_Wk2LiftUp         ; btMan2_9  -> Visible = true ;
    btMan2_10 -> Tag = (int)mcRAL_Wk2LiftDn         ; btMan2_10 -> Visible = true ;
    btMan2_11 -> Tag = (int)mcRAL_Wk2StprUp         ; btMan2_11 -> Visible = true ;
    btMan2_12 -> Tag = (int)mcRAL_Wk2StprDn         ; btMan2_12 -> Visible = true ;
    btMan2_13 -> Tag = (int)mcRAL_Wk2AlignFB        ; btMan2_13 -> Visible = true ;
    btMan2_14 -> Tag = (int)mcRAL_Wk3LiftUp         ; btMan2_14 -> Visible = true ;
    btMan2_15 -> Tag = (int)mcRAL_Wk3LiftDn         ; btMan2_15 -> Visible = true ;
    btMan2_16 -> Tag = (int)mcRAL_Wk3StprUp         ; btMan2_16 -> Visible = true ;
    btMan2_17 -> Tag = (int)mcRAL_Wk3StprDn         ; btMan2_17 -> Visible = true ;
    btMan2_18 -> Tag = (int)mcRAL_Wk3AlignFB        ; btMan2_18 -> Visible = true ;

    btMan3_1  -> Tag = (int)mcPSB_Home              ; btMan3_1  -> Visible = true ;
    btMan3_2  -> Tag = (int)mcPSB_InitPos           ; btMan3_2  -> Visible = true ;
    btMan3_3  -> Tag = (int)mcPSB_WorkStPos         ; btMan3_3  -> Visible = true ;
    btMan3_4  -> Tag = (int)mcPSB_LiftUp            ; btMan3_4  -> Visible = true ;
    btMan3_5  -> Tag = (int)mcPSB_LiftDn            ; btMan3_5  -> Visible = true ;
    btMan3_6  -> Tag = (int)mcPSB_StprUp            ; btMan3_6  -> Visible = true ;
    btMan3_7  -> Tag = (int)mcPSB_StprDn            ; btMan3_7  -> Visible = true ;
    btMan3_8  -> Tag = (int)mcPSB_MarkCmpUp         ; btMan3_8  -> Visible = true ;
    btMan3_9  -> Tag = (int)mcPSB_MarkCmpDn         ; btMan3_9  -> Visible = true ;
    btMan3_10 -> Tag = (int)mcPSB_UD1Cycle          ; btMan3_10 -> Visible = true ;
    btMan3_11 -> Tag = (int)mcPSB_MrkVisnInsp       ; btMan3_11 -> Visible = true ;
//    btMan3_12 -> Tag = (int)mcPsb_TransferDn        ; btMan3_12 -> Visible = true ;
    btMan3_13 -> Tag = (int)mcPSB_MarkingChange     ; btMan3_13 -> Visible = true ;
    btMan3_14 -> Tag = (int)mcPSB_AlignFB           ; btMan3_14 -> Visible = true ;
    btMan3_15 -> Tag = (int)mcPSB_TestMarking       ; btMan3_15 -> Visible = true ;

    btMan4_1  -> Tag = (int)mcULD_Home              ; btMan4_1  -> Visible = true ;
    btMan4_2  -> Tag = (int)mcPSB_PusherFw          ; btMan4_2  -> Visible = true ;
    btMan4_3  -> Tag = (int)mcPSB_PusherBw          ; btMan4_3  -> Visible = true ;
    btMan4_4  -> Tag = (int)mcULD_TransferInAC      ; btMan4_4  -> Visible = true ;
    btMan4_5  -> Tag = (int)mcULD_TransferOutAC     ; btMan4_5  -> Visible = true ;
    btMan4_6  -> Tag = (int)mcULD_Supply            ; btMan4_6  -> Visible = true ;
    btMan4_7  -> Tag = (int)mcULD_Pick              ; btMan4_7  -> Visible = true ;
    btMan4_8  -> Tag = (int)mcULD_Place             ; btMan4_8  -> Visible = true ;
    btMan4_9  -> Tag = (int)mcULD_Z1PichUp          ; btMan4_9  -> Visible = true ;
    btMan4_10 -> Tag = (int)mcULD_Z1PichDn          ; btMan4_10 -> Visible = true ;
    btMan4_11 -> Tag = (int)mcULD_SttPstn           ; btMan4_11 -> Visible = true ;
    btMan4_12 -> Tag = (int)mcULD_UnClamp           ; btMan4_12 -> Visible = true ;
    btMan4_13 -> Tag = (int)mcULD_Clamp             ; btMan4_13 -> Visible = true ;

    btMan5_1  -> Tag = (int)mcETC_TransferDn        ; btMan5_1  -> Visible = true ;
    btMan5_4  -> Tag = (int)mcETC_TransferUp        ; btMan5_4  -> Visible = true ;
    btMan5_2  -> Tag = (int)mcETC_FlipHome          ; btMan5_2  -> Visible = true ;
    btMan5_3  -> Tag = (int)mcHED_InspMan           ; btMan5_3  -> Visible = true ;
    btMan5_5  -> Tag = (int)mcETC_AllBeltOn         ; btMan5_5  -> Visible = true ;
    btMan5_6  -> Tag = (int)mcETC_AllBeltOff        ; btMan5_6  -> Visible = true ;
    btMan5_7  -> Tag = (int)mcETC_AllStprUp         ; btMan5_7  -> Visible = true ;
    btMan5_8  -> Tag = (int)mcETC_ALlStprDn         ; btMan5_8  -> Visible = true ;
    btMan5_9  -> Tag = (int)mcETC_Wk1AlignLiftUp    ; btMan5_9  -> Visible = true ;
    btMan5_10 -> Tag = (int)mcETC_Wk2AlignLiftUp    ; btMan5_10 -> Visible = true ;
    btMan5_11 -> Tag = (int)mcETC_Wk3AlignLiftUp    ; btMan5_11 -> Visible = true ;
    btMan5_12 -> Tag = (int)mcETC_MrkAlignLiftUp    ; btMan5_12 -> Visible = true ;
    btMan5_13 -> Tag = (int)mcETC_AllAlignLiftUp    ; btMan5_13 -> Visible = true ;
    btMan5_14 -> Tag = (int)mcETC_AllAlignLiftDn    ; btMan5_14 -> Visible = true ;
//    btMan5_15 -> Tag = (int)mcETC_MainAirOnOff      ; btMan5_15 -> Visible = true ;
}

//---------------------------------------------------------------------------
void __fastcall TFormManual::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    Trace("뻑","1");
    MM.SetManCycle(iManNo);

    Trace("뻑","7");

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

    MM.SetManCycle(mcETC_ManLotEnd);

    PSB.m_bLotEnded = true ;

    memcpy(&PSB.EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

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

    for(int i = 1 ; i < 6/*페이지5개*/ ; i++) {
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
//    FormMain -> btnRun -> Enabled = true ;
//    FormMain -> pnRun  -> Enabled = true ;
}
//---------------------------------------------------------------------------

