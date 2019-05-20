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
//SMDLL Reference
//---------------------------------------------------------------------------
#include "SMDllDefine.h"
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormManual *FormManual;
//---------------------------------------------------------------------------
__fastcall TFormManual::TFormManual(TComponent* Owner)
    : TForm(Owner)
{
    bStopper = false ;
    bLift    = false ;
    bAlign   = false ;


    btMan1_1  -> Tag = (int)mcLDR_Home       ;
    btMan1_2  -> Tag = (int)mcLDR_Supply     ;
    btMan1_3  -> Tag = (int)mcLDR_Pick       ;
    btMan1_4  -> Tag = (int)mcLDR_SttPstn    ;
    btMan1_5  -> Tag = (int)mcLDR_Place      ;
    btMan1_6  -> Tag = (int)mcLDR_UpSlPitch  ;
    btMan1_7  -> Tag = (int)mcLDR_DnSlPitch  ;
    btMan1_8  -> Tag = (int)mcLDR_Pusher     ;
    btMan1_9  -> Tag = (int)mcLDR_Clamp      ;
    btMan1_10 -> Tag = (int)mcLDR_TranInAC   ;
    btMan1_11 -> Tag = (int)mcLDR_TranOutAC  ;
    btMan1_12 -> Tag = (int)mcRAL_PreStprUp  ;
    btMan1_13 -> Tag = (int)mcRAL_PreStprDn  ;

    btMan2_1  -> Tag = (int)mcRAL_Home           ;
    btMan2_2  -> Tag = (int)mcRAL_WR1LiftUp      ;
    btMan2_3  -> Tag = (int)mcRAL_WR1LiftDn      ;
    btMan2_4  -> Tag = (int)mcRAL_WR1StprUp      ;
    btMan2_5  -> Tag = (int)mcRAL_WR1StprDn      ;
    btMan2_6  -> Tag = (int)mcRAL_WR1AlgnFw      ;
    btMan2_7  -> Tag = (int)mcRAL_WR1AlgnBw      ;
    btMan2_8  -> Tag = (int)mcRAL_WR2LiftUp      ;
    btMan2_9  -> Tag = (int)mcRAL_WR2LiftDn      ;
    btMan2_10 -> Tag = (int)mcRAL_WR2StprUp      ;   
    btMan2_11 -> Tag = (int)mcRAL_WR2StprDn      ;   
    btMan2_12 -> Tag = (int)mcRAL_WR2AlgnFw      ;   
    btMan2_13 -> Tag = (int)mcRAL_WR2AlgnBw      ;
    btMan2_14 -> Tag = (int)mcRAL_WR3LiftUp      ;   
    btMan2_15 -> Tag = (int)mcRAL_WR3LiftDn      ;
    btMan2_16 -> Tag = (int)mcRAL_WR3StprUp      ;   
    btMan2_17 -> Tag = (int)mcRAL_WR3StprDn      ;   
    btMan2_18 -> Tag = (int)mcRAL_WR3AlgnFw      ;
    btMan2_19 -> Tag = (int)mcRAL_WR3AlgnBw      ;
    btMan2_20 -> Tag = (int)mcHED_WorkFstPs      ;

    btMan3_1  -> Tag = (int)mcPSB_Home           ;
    btMan3_2  -> Tag = (int)mcPSB_TWaitPs        ; 
    btMan3_3  -> Tag = (int)mcPSB_TFirstPs       ;
    btMan3_4  -> Tag = (int)mcPSB_TFlpCmpUp      ; 
    btMan3_5  -> Tag = (int)mcPSB_TFlpCmpDn      ;
    btMan3_6  -> Tag = (int)mcPSB_TAlignFw       ;
    btMan3_7  -> Tag = (int)mcPSB_TFlpNorPs      ;
    btMan3_8  -> Tag = (int)mcPSB_TFlpInsPs      ;
    btMan3_9  -> Tag = (int)mcPSB_TTrimAllTest   ;
    btMan3_10 -> Tag = (int)mcPSB_TTrimUpDn      ;
    btMan3_11 -> Tag = (int)mcPSB_TTrimXMv       ;
    btMan3_12 -> Tag = (int)mcPSB_TTrimYMv       ;
    btMan3_13 -> Tag = (int)mcPSB_IdxUp          ;
    btMan3_14 -> Tag = (int)mcPSB_IdxDn          ;

    btMan4_1  -> Tag = (int)mcULD_Home      ;
    btMan4_2  -> Tag = (int)mcULD_Supply    ;
    btMan4_3  -> Tag = (int)mcULD_Pick      ;
    btMan4_4  -> Tag = (int)mcULD_SttPstn   ;
    btMan4_5  -> Tag = (int)mcULD_Place     ;
    btMan4_6  -> Tag = (int)mcULD_UpSlPitch ;
    btMan4_7  -> Tag = (int)mcULD_DnSlPitch ;
    btMan4_8  -> Tag = (int)mcULD_Clamp     ;
    btMan4_9  -> Tag = (int)mcPSB_PshrUpDn  ;
    btMan4_10 -> Tag = (int)mcPSB_PshrFwBw  ;
    btMan4_11 -> Tag = (int)mcULD_TranInAC  ;
    btMan4_12 -> Tag = (int)mcULD_TranOutAC ;

    btMan5_1 -> Tag = (int)mcETC_Sampling1Cycle ;
    btMan5_2 -> Tag = (int)mcETC_AllBeltOn      ;
    btMan5_3 -> Tag = (int)mcETC_AllBeltOff     ;
    btMan5_4 -> Tag = (int)mcETC_AllStprUp      ;
    btMan5_5 -> Tag = (int)mcETC_AllStprDn      ;
    btMan5_6 -> Tag = (int)mcETC_Wk1AlignLiftUp ;
    btMan5_7 -> Tag = (int)mcETC_Wk2AlignLiftUp ;
    btMan5_8 -> Tag = (int)mcETC_Wk3AlignLiftUp ;
    btMan5_9 -> Tag = (int)mcETC_InsAlignFlp    ;
    btMan5_10-> Tag = (int)mcPSB_TFlpNorPs      ;
    btMan5_11-> Tag = (int)mcETC_AllAlignLiftUp ;
    btMan5_12-> Tag = (int)mcETC_AllLiftDn      ;
    btMan5_13-> Tag = (int)mcETC_MainAirOnOff   ;
    btMan5_14-> Tag = (int)mcETC_AllStripOut    ;
    btMan5_15-> Tag = (int)mcETC_1CycleMv       ;
    btMan5_16-> Tag = (int)mcHED_WorkFstPs      ;

    btnInit  -> Tag = (int)mcAllHome            ;
    btLotEnd -> Tag = (int)mcETC_ManLotEnd      ;

    int iTag = 0 ;
    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TBitBtn"))
        {
            iTag = ((TBitBtn*)FindComponent(Components[i]->Name))->Tag ;
            if(iTag) ((TBitBtn*)FindComponent(Components[i]->Name))->Visible = true   ;
            else     ((TBitBtn*)FindComponent(Components[i]->Name))->Visible = false  ;
        }
    }

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
}
//---------------------------------------------------------------------------

void __fastcall TFormManual::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    if(btMan3_17->Tag == iManNo || btMan3_18->Tag == iManNo) {
        if(FM_MsgYesNo("Confirm","Reject Tool Remove Plz ") != mrYes) return ;
    }
    MM.SetManCycle(iManNo);

}
//---------------------------------------------------------------------------





