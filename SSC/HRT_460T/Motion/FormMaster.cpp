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
#include "PreBuffer.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "UnLoader.h"
#include "SLogUnit.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "LDR" ;
    sgCycleStep -> Cells[0][2] = "PRB" ;
    sgCycleStep -> Cells[0][3] = "WRK" ;
    sgCycleStep -> Cells[0][4] = "PSB" ;
    sgCycleStep -> Cells[0][5] = "ULD" ;

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
        //cbGripTest         -> Checked = OM.MstOptn.bGripTest         ;

        cbBarcodeUseOnly   -> Checked = OM.MstOptn.bBarcodeUseOnly   ;
        cbByChInsp         -> Checked = OM.MstOptn.bByChInsp         ;
        cbNotUstLot        -> Checked = OM.MstOptn.bNotUstLot        ;

        cbShortMonitor     -> Checked = OM.MstOptn.bShortMonitor     ;

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
        edTempOfs          -> Text    = OM.MstOptn.iTempOfs          ;
        //edWRK_CmpOfs       -> Text    = OM.MstOptn.dWRKCmpOfs        ;
        edMaxTmp           -> Text    = OM.MstOptn.iMaxTemp          ;

        cbUsedEngVersion   -> Checked = OM.MstOptn.bUsedEngVersion   ;
        cbUsedBinSorting   -> Checked = OM.MstOptn.bUsedBinSorting   ;

        cbUsed4RowCntct    -> Checked = OM.MstOptn.bUsed4RowCntct    ;
    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode        -> Checked ;
//        OM.MstOptn.sPstEquipDrv       = edPostEquipDrv     -> Text    ;
//        OM.MstOptn.sPstEquipIp        = edPostEquipIp      -> Text    ;
        OM.MstOptn.sPstEquipPath      = edPostEquipPath    -> Text    ;
        OM.MstOptn.bHeattingTest      = cbHeattingTest     -> Checked ;
        OM.MstOptn.bBarcodeUseOnly    = cbBarcodeUseOnly   -> Checked ;
        OM.MstOptn.bByChInsp          = cbByChInsp         -> Checked ;
        OM.MstOptn.bNotUstLot         = cbNotUstLot        -> Checked ;
        OM.MstOptn.bShortMonitor      = cbShortMonitor     -> Checked ;


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
        OM.MstOptn.iTempOfs           = edTempOfs          -> Text.ToInt   ();
        OM.MstOptn.iMaxTemp           = edMaxTmp           -> Text.ToInt   ();

        OM.MstOptn.bUsedEngVersion    = cbUsedEngVersion   -> Checked;

        OM.MstOptn.bUsedBinSorting    = cbUsedBinSorting   -> Checked;

        if(OM.MstOptn.iPreMaxTemp != edMaxTmp -> Text.ToInt( )){
            OM.MstOptn.iMaxTemp           = edMaxTmp           -> Text.ToInt   ();
            sTemp.printf("MAX Tempe Change %d -> %d" , OM.MstOptn.iPreMaxTemp, OM.MstOptn.iMaxTemp);
            Trace("MAX Tempe Change", sTemp.c_str());
            OM.MstOptn.iPreMaxTemp = OM.MstOptn.iMaxTemp;
        }
        OM.MstOptn.bUsed4RowCntct     = cbUsed4RowCntct    -> Checked ;
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
    for(int i= 0 ; i < MAX_PART ; i++) {
        sgCycleStep->Cells[1][i+1]=SEQ.m_pPart[i]->GetHomeStep   ();
        sgCycleStep->Cells[2][i+1]=SEQ.m_pPart[i]->GetToStartStep();
        sgCycleStep->Cells[3][i+1]=SEQ.m_pPart[i]->GetCycleName  (SEQ.m_pPart[i]->GetSeqStep());
        sgCycleStep->Cells[4][i+1]=SEQ.m_pPart[i]->GetCycleStep  ();
        sgCycleStep->Cells[5][i+1]=SEQ.m_pPart[i]->GetToStopStep ();
        sgCycleStep->Cells[6][i+1]=SEQ.m_pPart[i]->GetCycleTime  (SEQ.m_pPart[i]->GetSeqStep());
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Autorun();
    if(cbPRB -> Checked) PRB.Autorun();
    if(cbWRK -> Checked) WRK.Autorun();
    if(cbPSB -> Checked) PSB.Autorun();
    if(cbULD -> Checked) ULD.Autorun();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Reset();
    if(cbPRB -> Checked) PRB.Reset();
    if(cbWRK -> Checked) WRK.Reset();
    if(cbPSB -> Checked) PSB.Reset();
    if(cbULD -> Checked) ULD.Reset();

    SEQ.Reset();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    SEQ.Reset();

}
//---------------------------------------------------------------------------






