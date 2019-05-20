//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Loader.h"
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "SortingTool.h"
#include "ULD_HRT430S.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "LD1" ;
    sgCycleStep -> Cells[0][2] = "PRB" ;
    sgCycleStep -> Cells[0][3] = "WRK" ;
    sgCycleStep -> Cells[0][4] = "PSB" ;
    sgCycleStep -> Cells[0][5] = "TRIM";
    sgCycleStep -> Cells[0][6] = "UL1" ;

    sgCycleStep -> Cells[1][0] = "Home   " ;
    sgCycleStep -> Cells[2][0] = "ToStart" ;
    sgCycleStep -> Cells[3][0] = "Seq    " ;
    sgCycleStep -> Cells[4][0] = "Cycle  " ;
    sgCycleStep -> Cells[5][0] = "ToStop " ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btSaveMotorClick(TObject *Sender)
{
    UpdateMstOptn (toBuff);
    UpdateEqpOptn (toBuff);
    OM.SaveMstOptn(      );
    OM.SaveEqpOptn(      );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::FormShow(TObject *Sender)
{
    OM.LoadMstOptn(      );
    OM.LoadEqpOptn(      );
    UpdateMstOptn (toTabl);
    UpdateEqpOptn (toTabl);
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateMstOptn(bool bTable)
{
    if(bTable){
        cbDebugMode        -> Checked = OM.MstOptn.bDebugMode        ;
//        edPostEquipDrv     -> Text    = OM.MstOptn.sPstEquipDrv      ;
//        edPostEquipIp      -> Text    = OM.MstOptn.sPstEquipIp       ;
        edPostEquipPath    -> Text    = OM.MstOptn.sPstEquipPath     ;
        cbHeattingTest     -> Checked = OM.MstOptn.bHeattingTest     ;
        cbNoLdUl           -> Checked = OM.MstOptn.bNoLdUl           ;
        cbGripTest         -> Checked = OM.MstOptn.bGripTest         ;

        cbPRBCmp           -> Checked = OM.MstOptn.bPRBCmp           ;

        cbBarcodeUseOnly   -> Checked = OM.MstOptn.bBarcodeUseOnly   ;
        cbByChInsp         -> Checked = OM.MstOptn.bByChInsp         ;
        cbLdr_YHome        -> Checked = OM.MstOptn.bLdr_YHome        ;
        cbUseDaegyeomProbe -> Checked = OM.MstOptn.bUseDaegyeomProbe ;
        cbNotUstLot        -> Checked = OM.MstOptn.bNotUstLot        ;

        cbShortMonitor     -> Checked = OM.MstOptn.bShortMonitor     ;

        ed1chLedOffset     -> Text    = OM.MstOptn.d1chLedOffset     ;
        ed1chLedGain       -> Text    = OM.MstOptn.d1chLedGain       ;
        ed1chZennerOffset  -> Text    = OM.MstOptn.d1chZennerOffset  ;
        ed1chZennerGain    -> Text    = OM.MstOptn.d1chZennerGain    ;
        ed2chLedOffset     -> Text    = OM.MstOptn.d2chLedOffset     ;
        ed2chLedGain       -> Text    = OM.MstOptn.d2chLedGain       ;
        ed2chZennerOffset  -> Text    = OM.MstOptn.d2chZennerOffset  ;
        ed2chZennerGain    -> Text    = OM.MstOptn.d2chZennerGain    ;
        edBarcodeNum       -> Text    = OM.MstOptn.iBarcodeNum       ;
        cbDataAccEng       -> Checked = OM.MstOptn.bDataAccEng       ;

    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode        -> Checked ;
//        OM.MstOptn.sPstEquipDrv       = edPostEquipDrv     -> Text    ;
//        OM.MstOptn.sPstEquipIp        = edPostEquipIp      -> Text    ;
        OM.MstOptn.sPstEquipPath      = edPostEquipPath    -> Text    ;
        OM.MstOptn.bHeattingTest      = cbHeattingTest     -> Checked ;
        OM.MstOptn.bNoLdUl            = cbNoLdUl           -> Checked ;
        OM.MstOptn.bGripTest          = cbGripTest         -> Checked ;
        OM.MstOptn.bPRBCmp            = cbPRBCmp           -> Checked ;
        OM.MstOptn.bBarcodeUseOnly    = cbBarcodeUseOnly   -> Checked ;
        OM.MstOptn.bByChInsp          = cbByChInsp         -> Checked ;
        OM.MstOptn.bLdr_YHome         = cbLdr_YHome        -> Checked ;
        OM.MstOptn.bUseDaegyeomProbe  = cbUseDaegyeomProbe -> Checked ;
        OM.MstOptn.bNotUstLot         = cbNotUstLot        -> Checked ;
        OM.MstOptn.bShortMonitor      = cbShortMonitor     -> Checked ;

        OM.MstOptn.d1chLedOffset      = ed1chLedOffset     -> Text.ToDouble();
        OM.MstOptn.d1chLedGain        = ed1chLedGain       -> Text.ToDouble();
        OM.MstOptn.d1chZennerOffset   = ed1chZennerOffset  -> Text.ToDouble();
        OM.MstOptn.d1chZennerGain     = ed1chZennerGain    -> Text.ToDouble();
        OM.MstOptn.d2chLedOffset      = ed2chLedOffset     -> Text.ToDouble();
        OM.MstOptn.d2chLedGain        = ed2chLedGain       -> Text.ToDouble();
        OM.MstOptn.d2chZennerOffset   = ed2chZennerOffset  -> Text.ToDouble();
        OM.MstOptn.d2chZennerGain     = ed2chZennerGain    -> Text.ToDouble();
        OM.MstOptn.iBarcodeNum        = edBarcodeNum       -> Text.ToInt   ();

        OM.MstOptn.bDataAccEng        = cbDataAccEng       -> Checked        ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
        //edPostEquipPath -> Text    = OM.MstOptn.sPstEquipPath  ;
        cbExistLoader     -> Checked = OM.EqpOptn.bExistLoader   ;
        cbExistUnLoader   -> Checked = OM.EqpOptn.bExistUnLoader ;
    }
    else{
        //OM.MstOptn.sPstEquipPath  = edPostEquipPath -> Text    ;
        OM.EqpOptn.bExistLoader   = cbExistLoader   -> Checked ;
        OM.EqpOptn.bExistUnLoader = cbExistUnLoader -> Checked ;
    }
}
void __fastcall TFrmMaster::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::tmUpdateTimer(TObject *Sender)
{
    //LDR
    sgCycleStep -> Cells[1][1] = LD1.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = LD1.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = LD1.GetSeqStep    () ;
    sgCycleStep -> Cells[4][1] = LD1.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = LD1.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = PRB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = PRB.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = PRB.GetSeqStep    () ;
    sgCycleStep -> Cells[4][2] = PRB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = PRB.GetToStopStep () ;

    sgCycleStep -> Cells[1][3] = WRK.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = WRK.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = WRK.GetSeqStep    () ;
    sgCycleStep -> Cells[4][3] = WRK.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = WRK.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = PSB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][4] = PSB.GetToStartStep() ;
    sgCycleStep -> Cells[3][4] = PSB.GetSeqStep    () ;
    sgCycleStep -> Cells[4][4] = PSB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][4] = PSB.GetToStopStep () ;

    sgCycleStep -> Cells[1][5] = STL.GetHomeStep   () ;
    sgCycleStep -> Cells[2][5] = STL.GetToStartStep() ;
    sgCycleStep -> Cells[3][5] = STL.GetSeqStep    () ;
    sgCycleStep -> Cells[4][5] = STL.GetCycleStep  () ;
    sgCycleStep -> Cells[5][5] = STL.GetToStopStep () ;


    sgCycleStep -> Cells[1][6] = ULD.GetHomeStep   () ;
    sgCycleStep -> Cells[2][6] = ULD.GetToStartStep() ;
    sgCycleStep -> Cells[3][6] = ULD.GetSeqStep    () ;
    sgCycleStep -> Cells[4][6] = ULD.GetCycleStep  () ;
    sgCycleStep -> Cells[5][6] = ULD.GetToStopStep () ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbLDR -> Checked) LD1.Autorun();
    if(cbPRB -> Checked) PRB.Autorun();
    if(cbWRK -> Checked) WRK.Autorun();
    if(cbPSB -> Checked) PSB.Autorun();
    if(cbULD -> Checked) ULD.Autorun();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbLDR -> Checked) LD1.Reset();
    if(cbPRB -> Checked) PRB.Reset();
    if(cbWRK -> Checked) WRK.Reset();
    if(cbPSB -> Checked) PSB.Reset();
    if(cbSTL -> Checked) STL.Reset();
    if(cbULD -> Checked) ULD.Reset();

    SEQ.Reset();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------



