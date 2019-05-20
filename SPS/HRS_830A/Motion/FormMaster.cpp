//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Work.h"
#include "PostBuff.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{

    sgCycleStep -> RowCount = MAX_PART + 1 ;

    sgCycleStep -> Cells[0][ 1] = PRB.GetPartName() ; cb1  -> Caption = PRB.GetPartName() ;
    sgCycleStep -> Cells[0][ 2] = WRK.GetPartName() ; cb2  -> Caption = WRK.GetPartName() ;
    sgCycleStep -> Cells[0][ 3] = PSB.GetPartName() ; cb3  -> Caption = PSB.GetPartName() ;
    sgCycleStep -> Cells[0][ 4] = LDR.GetPartName() ; cb4  -> Caption = LDR.GetPartName() ;
    sgCycleStep -> Cells[0][ 5] = ULD.GetPartName() ; cb5  -> Caption = ULD.GetPartName() ;

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
        cbDebugMode        -> Checked = OM.MstOptn.bDebugMode         ;
        cbDryRun           -> Checked = OM.MstOptn.bDryRun            ;
        cbUsedSingleOptn   -> Checked = OM.MstOptn.bUsedSingleDisps   ;
        cbUsedDispsrOptn   -> Checked = OM.MstOptn.bUsedDispsOptn     ;
        edDispsrOptn       -> Text    = OM.MstOptn.iDispsOptn         ;
        cbOvldSnsrSkip     -> Checked = OM.MstOptn.bSkipOvldSnsr      ;

        
    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode         -> Checked ;
        OM.MstOptn.bDryRun            = cbDryRun            -> Checked ;
        OM.MstOptn.bUsedDispsOptn     = cbUsedDispsrOptn    -> Checked ;
        OM.MstOptn.bUsedSingleDisps   = cbUsedSingleOptn    -> Checked ;
        OM.MstOptn.iDispsOptn         = StrToIntDef(edDispsrOptn-> Text, 1);
        OM.MstOptn.bSkipOvldSnsr      = cbOvldSnsrSkip      -> Checked ;


    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
    }
    else{

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

    lbMan -> Caption = "<Man No = " + AnsiString(MM.GetManNo()) + "> <Man.m_iStep = " + MM.GetManStep() ; 
    //LDR
    sgCycleStep->Cells[1][ 1]=PRB.GetHomeStep(); sgCycleStep->Cells[2][ 1]=PRB.GetToStartStep(); sgCycleStep->Cells[3][ 1]=PRB.GetCycleName(); sgCycleStep->Cells[4][ 1]=PRB.GetCycleStep(); sgCycleStep->Cells[5][ 1]=PRB.GetToStopStep();
    sgCycleStep->Cells[1][ 2]=WRK.GetHomeStep(); sgCycleStep->Cells[2][ 2]=WRK.GetToStartStep(); sgCycleStep->Cells[3][ 2]=WRK.GetCycleName(); sgCycleStep->Cells[4][ 2]=WRK.GetCycleStep(); sgCycleStep->Cells[5][ 2]=WRK.GetToStopStep();
    sgCycleStep->Cells[1][ 3]=PSB.GetHomeStep(); sgCycleStep->Cells[2][ 3]=PSB.GetToStartStep(); sgCycleStep->Cells[3][ 3]=PSB.GetCycleName(); sgCycleStep->Cells[4][ 3]=PSB.GetCycleStep(); sgCycleStep->Cells[5][ 3]=PSB.GetToStopStep();
    sgCycleStep->Cells[1][ 4]=LDR.GetHomeStep(); sgCycleStep->Cells[2][ 4]=LDR.GetToStartStep(); sgCycleStep->Cells[3][ 4]=LDR.GetCycleName(); sgCycleStep->Cells[4][ 4]=LDR.GetCycleStep(); sgCycleStep->Cells[5][ 4]=LDR.GetToStopStep();
    sgCycleStep->Cells[1][ 5]=ULD.GetHomeStep(); sgCycleStep->Cells[2][ 5]=ULD.GetToStartStep(); sgCycleStep->Cells[3][ 5]=ULD.GetCycleName(); sgCycleStep->Cells[4][ 5]=ULD.GetCycleStep(); sgCycleStep->Cells[5][ 5]=ULD.GetToStopStep();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartAutorunClick(TObject *Sender)
{
    if(cb1  -> Checked) PRB.Autorun();
    if(cb2  -> Checked) WRK.Autorun();
    if(cb3  -> Checked) PSB.Autorun();
    if(cb4  -> Checked) LDR.Autorun();
    if(cb5  -> Checked) ULD.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartResetClick(TObject *Sender)
{
    if(cb1  -> Checked) PRB.Reset();
    if(cb2  -> Checked) WRK.Reset();
    if(cb3  -> Checked) PSB.Reset();
    if(cb4  -> Checked) LDR.Reset();
    if(cb5  -> Checked) ULD.Reset();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAllResetClick(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartHomeClick(TObject *Sender)
{
    if(cb1  -> Checked) {MM.SetManCycle(mcPRB_Home);}
    if(cb2  -> Checked) {MM.SetManCycle(mcWRK_Home);}
    if(cb3  -> Checked) {MM.SetManCycle(mcPSB_Home);}
    if(cb4  -> Checked) {MM.SetManCycle(mcLDR_Home);}
    if(cb5  -> Checked) {MM.SetManCycle(mcULD_Home);}   //잠시 후에 한다..JS
}
//---------------------------------------------------------------------------


