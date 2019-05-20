//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <cstdlib>
#include <fstream.h>
#include <iostream.h>

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff1.h"
#include "PreBuff2.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "SortingTool.h"
#include "UnLoader.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "LD1" ;
    sgCycleStep -> Cells[0][2] = "PR1" ;
    sgCycleStep -> Cells[0][3] = "PR2" ;
    sgCycleStep -> Cells[0][4] = "WRK" ;
    sgCycleStep -> Cells[0][5] = "PSB" ;
    sgCycleStep -> Cells[0][6] = "TRIM";
    sgCycleStep -> Cells[0][7] = "UL1" ;

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
    AnsiString sTemp = "";
    if(bTable){
        cbDebugMode        -> Checked = OM.MstOptn.bDebugMode        ;
//        edPostEquipDrv     -> Text    = OM.MstOptn.sPstEquipDrv      ;
//        edPostEquipIp      -> Text    = OM.MstOptn.sPstEquipIp       ;
        edPostEquipPath    -> Text    = OM.MstOptn.sPstEquipPath     ;
        cbHeattingTest     -> Checked = OM.MstOptn.bHeattingTest     ;
        cbNoLdUl           -> Checked = OM.MstOptn.bNoLdUl           ;
        //cbGripTest         -> Checked = OM.MstOptn.bGripTest         ;

        cbPRBCmp           -> Checked = OM.MstOptn.bPRBCmp           ;

        cbBarcodeUseOnly   -> Checked = OM.MstOptn.bBarcodeUseOnly   ;
        cbByChInsp         -> Checked = OM.MstOptn.bByChInsp         ;
        cbLdr_YHome        -> Checked = OM.MstOptn.bLdr_YHome        ;
        //cbUseDaegyeomProbe -> Checked = OM.MstOptn.bUseDaegyeomProbe ;
        cbNotUstLot        -> Checked = OM.MstOptn.bNotUstLot        ;

        cbShortMonitor     -> Checked = OM.MstOptn.bShortMonitor     ;
        cbUsedTempRS232    -> Checked = OM.MstOptn.bUsedTempRS232   ;

        ed1chLedOffset     -> Text    = OM.MstOptn.d1chLedOffset     ;
        ed1chLedGain       -> Text    = OM.MstOptn.d1chLedGain       ;
        ed1chLedSetOfs     -> Text    = OM.MstOptn.d1chLedSetOfs     ;
        ed1chZennerOffset  -> Text    = OM.MstOptn.d1chZennerOffset  ;
        ed1chZennerGain    -> Text    = OM.MstOptn.d1chZennerGain    ;
        ed1chZrSetOfs      -> Text    = OM.MstOptn.d1chZennerSetOfs  ;
        ed2chLedOffset     -> Text    = OM.MstOptn.d2chLedOffset     ;
        ed2chLedGain       -> Text    = OM.MstOptn.d2chLedGain       ;
        ed2chLedSetOfs     -> Text    = OM.MstOptn.d2chLedSetOfs     ;
        ed2chZennerOffset  -> Text    = OM.MstOptn.d2chZennerOffset  ;
        ed2chZennerGain    -> Text    = OM.MstOptn.d2chZennerGain    ;
        ed2chZrSetOfs      -> Text    = OM.MstOptn.d2chZennerSetOfs  ;
        edBarcodeNum       -> Text    = OM.MstOptn.iBarcodeNum       ;
        cbDataAccEng       -> Checked = OM.MstOptn.bDataAccEng       ;
        edTempOfs          -> Text    = OM.MstOptn.iTempOfs          ;
        //edWRK_CmpOfs       -> Text    = OM.MstOptn.dWRKCmpOfs        ;
        cbUsedVsn          -> Checked = OM.MstOptn.bUsedVision       ;
        edMaxTmp           -> Text    = OM.MstOptn.iMaxTemp          ;

        cbUsedEngVersion   -> Checked = OM.MstOptn.bUsedEngVersion   ;
        cbUsed3040Pnch     -> Checked = OM.MstOptn.bUsed3040Pnch     ;
        cbUsedBinSorting   -> Checked = OM.MstOptn.bUsedBinSorting   ;
        cbUsedXAxis        -> Checked = OM.MstOptn.bUsedXAxis        ;
        cbUsedChinaSPC     -> Checked = OM.MstOptn.bUsedChinaSPC     ;

        cbUsedSmallSizePkg  -> Checked = OM.MstOptn.bUsedSmallSizePkg ;
    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode        -> Checked ;
//        OM.MstOptn.sPstEquipDrv       = edPostEquipDrv     -> Text    ;
//        OM.MstOptn.sPstEquipIp        = edPostEquipIp      -> Text    ;
        OM.MstOptn.sPstEquipPath      = edPostEquipPath    -> Text    ;
        OM.MstOptn.bHeattingTest      = cbHeattingTest     -> Checked ;
        OM.MstOptn.bNoLdUl            = cbNoLdUl           -> Checked ;
        //OM.MstOptn.bGripTest          = cbGripTest         -> Checked ;
        OM.MstOptn.bPRBCmp            = cbPRBCmp           -> Checked ;
        OM.MstOptn.bBarcodeUseOnly    = cbBarcodeUseOnly   -> Checked ;
        OM.MstOptn.bByChInsp          = cbByChInsp         -> Checked ;
        OM.MstOptn.bLdr_YHome         = cbLdr_YHome        -> Checked ;
        //OM.MstOptn.bUseDaegyeomProbe  = cbUseDaegyeomProbe -> Checked ;
        OM.MstOptn.bNotUstLot         = cbNotUstLot        -> Checked ;
        OM.MstOptn.bShortMonitor      = cbShortMonitor     -> Checked ;
        OM.MstOptn.bUsedVision        = cbUsedVsn          -> Checked ;
        OM.MstOptn.bUsedTempRS232     = cbUsedTempRS232    -> Checked ;


        OM.MstOptn.d1chLedOffset      = ed1chLedOffset     -> Text.ToDouble();
        OM.MstOptn.d1chLedGain        = ed1chLedGain       -> Text.ToDouble();
        OM.MstOptn.d1chZennerOffset   = ed1chZennerOffset  -> Text.ToDouble();
        OM.MstOptn.d1chZennerGain     = ed1chZennerGain    -> Text.ToDouble();
        OM.MstOptn.d1chLedSetOfs      = ed1chLedSetOfs     -> Text.ToDouble();
        OM.MstOptn.d1chZennerSetOfs   = ed1chZrSetOfs      -> Text.ToDouble();
        OM.MstOptn.d2chLedOffset      = ed2chLedOffset     -> Text.ToDouble();
        OM.MstOptn.d2chLedGain        = ed2chLedGain       -> Text.ToDouble();
        OM.MstOptn.d2chZennerOffset   = ed2chZennerOffset  -> Text.ToDouble();
        OM.MstOptn.d2chZennerGain     = ed2chZennerGain    -> Text.ToDouble();
        OM.MstOptn.d2chLedSetOfs      = ed2chLedSetOfs     -> Text.ToDouble();
        OM.MstOptn.d2chZennerSetOfs   = ed2chZrSetOfs      -> Text.ToDouble();
        //OM.MstOptn.dWRKCmpOfs         = edWRK_CmpOfs       -> Text.ToDouble();
        OM.MstOptn.iBarcodeNum        = edBarcodeNum       -> Text.ToInt   ();
        OM.MstOptn.bDataAccEng        = cbDataAccEng       -> Checked        ;
        OM.MstOptn.iTempOfs           = edTempOfs          -> Text.ToInt   ();
        OM.MstOptn.iMaxTemp           = edMaxTmp           -> Text.ToInt   ();

        OM.MstOptn.bUsedEngVersion    = cbUsedEngVersion   -> Checked;
        OM.MstOptn.bUsed3040Pnch      = cbUsed3040Pnch     -> Checked;

        OM.MstOptn.bUsedXAxis         = cbUsedXAxis        -> Checked ;
        OM.MstOptn.bUsedBinSorting    = cbUsedBinSorting   -> Checked;
        
        OM.MstOptn.bUsedSmallSizePkg  = cbUsedSmallSizePkg -> Checked ;

        if(OM.MstOptn.iPreMaxTemp != edMaxTmp -> Text.ToInt( )){
            OM.MstOptn.iMaxTemp           = edMaxTmp           -> Text.ToInt   ();
            sTemp.printf("MAX Tempe Change %d -> %d" , OM.MstOptn.iPreMaxTemp, OM.MstOptn.iMaxTemp);
            Trace("MAX Tempe Change", sTemp.c_str());
            OM.MstOptn.iPreMaxTemp = OM.MstOptn.iMaxTemp;
        }
        OM.MstOptn.bUsedChinaSPC      = cbUsedChinaSPC     -> Checked ;

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
    sgCycleStep -> Cells[1][1] = LDR.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = LDR.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = LDR.GetCycleName  () ;
    sgCycleStep -> Cells[4][1] = LDR.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = LDR.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = PR1.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = PR1.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = PR1.GetCycleName  () ;
    sgCycleStep -> Cells[4][2] = PR1.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = PR1.GetToStopStep () ;

    sgCycleStep -> Cells[1][3] = PR2.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = PR2.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = PR2.GetCycleName  () ;
    sgCycleStep -> Cells[4][3] = PR2.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = PR2.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = WRK.GetHomeStep   () ;
    sgCycleStep -> Cells[2][4] = WRK.GetToStartStep() ;
    sgCycleStep -> Cells[3][4] = WRK.GetCycleName  () ;
    sgCycleStep -> Cells[4][4] = WRK.GetCycleStep  () ;
    sgCycleStep -> Cells[5][4] = WRK.GetToStopStep () ;

    sgCycleStep -> Cells[1][5] = PSB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][5] = PSB.GetToStartStep() ;
    sgCycleStep -> Cells[3][5] = PSB.GetCycleName  () ;
    sgCycleStep -> Cells[4][5] = PSB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][5] = PSB.GetToStopStep () ;

    sgCycleStep -> Cells[1][6] = STL.GetHomeStep   () ;
    sgCycleStep -> Cells[2][6] = STL.GetToStartStep() ;
    sgCycleStep -> Cells[3][6] = STL.GetCycleName  () ;
    sgCycleStep -> Cells[4][6] = STL.GetCycleStep  () ;
    sgCycleStep -> Cells[5][6] = STL.GetToStopStep () ;

    sgCycleStep -> Cells[1][7] = ULD.GetHomeStep   () ;
    sgCycleStep -> Cells[2][7] = ULD.GetToStartStep() ;
    sgCycleStep -> Cells[3][7] = ULD.GetCycleName  () ;
    sgCycleStep -> Cells[4][7] = ULD.GetCycleStep  () ;
    sgCycleStep -> Cells[5][7] = ULD.GetToStopStep () ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Autorun();
    if(cbPR1 -> Checked) PR1.Autorun();
    if(cbPR2 -> Checked) PR2.Autorun();
    if(cbWRK -> Checked) WRK.Autorun();
    if(cbPSB -> Checked) PSB.Autorun();
    if(cbULD -> Checked) ULD.Autorun();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Reset();
    if(cbPR1 -> Checked) PR1.Reset();
    if(cbPR2 -> Checked) PR2.Reset();
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






