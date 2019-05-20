//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmMaster.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "VisnComUnit.h"
#include "DataMan.h"
#include "LogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "Work.h"
#include "PreIdx.h"
#include "PstIdx.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMaster *FormMaster;
//---------------------------------------------------------------------------
__fastcall TFormMaster::TFormMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "PRB " ;
    sgCycleStep -> Cells[0][2] = "WRK1" ;
    sgCycleStep -> Cells[0][3] = "WRK2" ;


    sgCycleStep -> Cells[1][0] = "Home"    ;
    sgCycleStep -> Cells[2][0] = "ToStart" ;
    sgCycleStep -> Cells[3][0] = "Seq"     ;
    sgCycleStep -> Cells[4][0] = "Cycle"   ;
    sgCycleStep -> Cells[5][0] = "ToStop"  ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMaster::FormShow(TObject *Sender)
{
    OM.LoadMstOptn(      );
    UpdateMstOptn (toTabl);
    tmMaster -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::FormHide(TObject *Sender)
{
    tmMaster -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::UpdateMstOptn(bool bTable)
{
    if(bTable){
        cbDebugMode       -> Checked = OM.MstOptn.bDebugMode      ;
//        cbDataAccEng      -> Checked = OM.MstOptn.bDataAccEng     ;
        cbDryRun          -> Checked = OM.CmnOptn.bDryRun         ; //본사람이 마스터 옵션으로 바꾸기.
        cbGoodChipAdd     -> Checked = OM.MstOptn.bGoodChipAdd    ;
        edPreEquipPath    -> Text    = OM.MstOptn.sPreEquipPath    ;
    }
    else{
        OM.MstOptn.bDebugMode        = cbDebugMode       -> Checked   ;
//        OM.MstOptn.bDataAccEng       = cbDataAccEng      -> Checked   ;
        OM.CmnOptn.bDryRun           = cbDryRun          -> Checked   ;
        OM.MstOptn.bGoodChipAdd      = cbGoodChipAdd     -> Checked   ;
        OM.MstOptn.sPreEquipPath     = edPreEquipPath    -> Text      ;

    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::btSaveMotorClick(TObject *Sender)
{
    UpdateMstOptn (toBuff);

    OM.SaveMstOptn(      );
    OM.SaveCmnOptn(      );
}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::btAutoRunClick(TObject *Sender)
{
    if(cbPRB -> Checked) PRB.Autorun();
    if(cbPRI -> Checked) PRI.Autorun();
    if(cbWRK -> Checked) WRK.Autorun();
    if(cbPSI -> Checked) PSI.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::btResetClick(TObject *Sender)
{
    if(cbPRB -> Checked) PRB.Reset();
    if(cbPRI -> Checked) PRI.Reset();
    if(cbWRK -> Checked) WRK.Reset();
    if(cbPSI -> Checked) PSI.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::btErrResetClick(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFormMaster::tmMasterTimer(TObject *Sender)
{
    //LDR
    sgCycleStep -> Cells[1][1] = PRB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = PRB.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = PRB.GetSeqStep    () ;
    sgCycleStep -> Cells[4][1] = PRB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = PRB.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = PRI.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = PRI.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = PRI.GetSeqStep    () ;
    sgCycleStep -> Cells[4][2] = PRI.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = PRI.GetToStopStep () ;


    sgCycleStep -> Cells[1][3] = WRK.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = WRK.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = WRK.GetSeqStep    () ;
    sgCycleStep -> Cells[4][3] = WRK.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = WRK.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = PSI.GetHomeStep    () ;
    sgCycleStep -> Cells[2][4] = PSI.GetToStartStep () ;
    sgCycleStep -> Cells[3][4] = PSI.GetSeqStep     () ;
    sgCycleStep -> Cells[4][4] = PSI.GetCycleStep   () ;
    sgCycleStep -> Cells[5][4] = PSI.GetToStopStep  () ;

    Label1->Caption = DM.ARAY[riPRI].GetStep() ;
//
//    sgCycleStep -> Cells[1][3] = HED.GetHomeStep   () ;
//    sgCycleStep -> Cells[2][3] = HED.GetToStartStep() ;
//    sgCycleStep -> Cells[3][3] = HED.GetSeqStep    () ;
//    sgCycleStep -> Cells[4][3] = HED.GetCycleStep  () ;
//    sgCycleStep -> Cells[5][3] = HED.GetToStopStep () ;
//
//    sgCycleStep -> Cells[1][4] = PSB.GetHomeStep   () ;
//    sgCycleStep -> Cells[2][4] = PSB.GetToStartStep() ;
//    sgCycleStep -> Cells[3][4] = PSB.GetSeqStep    () ;
//    sgCycleStep -> Cells[4][4] = PSB.GetCycleStep  () ;
//    sgCycleStep -> Cells[5][4] = PSB.GetToStopStep () ;
//
//    sgCycleStep -> Cells[1][5] = ULD.GetHomeStep   () ;
//    sgCycleStep -> Cells[2][5] = ULD.GetToStartStep() ;
//    sgCycleStep -> Cells[3][5] = ULD.GetSeqStep    () ;
//    sgCycleStep -> Cells[4][5] = ULD.GetCycleStep  () ;
//    sgCycleStep -> Cells[5][5] = ULD.GetToStopStep () ;
}
//---------------------------------------------------------------------------





void __fastcall TFormMaster::BitBtn1Click(TObject *Sender)
{
    DM.ARAY[riPRI].SetStep(0);
}
//---------------------------------------------------------------------------


void __fastcall TFormMaster::Button1Click(TObject *Sender)
{
    WRK.m_iBinNo = 1 ;
}
//---------------------------------------------------------------------------

