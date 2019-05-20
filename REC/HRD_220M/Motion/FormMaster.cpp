//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "JigPick.h"
#include "Dispnr.h"
#include "Place.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{

    sgCycleStep -> RowCount = MAX_PART + 1 ;

    sgCycleStep -> Cells[0][ 1] = JIP.GetPartName() ; cb1  -> Caption = JIP.GetPartName() ;
    sgCycleStep -> Cells[0][ 2] = DIP.GetPartName() ; cb1  -> Caption = DIP.GetPartName() ;
    sgCycleStep -> Cells[0][ 3] = PLC.GetPartName() ; cb1  -> Caption = PLC.GetPartName() ;



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
    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode         -> Checked ;
        OM.MstOptn.bDryRun            = cbDryRun            -> Checked ;
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
    sgCycleStep->Cells[1][ 1]=JIP.GetHomeStep();sgCycleStep->Cells[2][ 1]=JIP.GetToStartStep();sgCycleStep->Cells[3][ 1]=JIP.GetCycleName();sgCycleStep->Cells[4][ 1]=JIP.GetCycleStep();sgCycleStep->Cells[5][ 1]=JIP.GetToStopStep();
    sgCycleStep->Cells[1][ 2]=DIP.GetHomeStep();sgCycleStep->Cells[2][ 2]=DIP.GetToStartStep();sgCycleStep->Cells[3][ 2]=DIP.GetCycleName();sgCycleStep->Cells[4][ 2]=DIP.GetCycleStep();sgCycleStep->Cells[5][ 2]=DIP.GetToStopStep();
    sgCycleStep->Cells[1][ 3]=PLC.GetHomeStep();sgCycleStep->Cells[2][ 3]=PLC.GetToStartStep();sgCycleStep->Cells[3][ 3]=PLC.GetCycleName();sgCycleStep->Cells[4][ 3]=PLC.GetCycleStep();sgCycleStep->Cells[5][ 3]=PLC.GetToStopStep();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartAutorunClick(TObject *Sender)
{
    if(cb1  -> Checked) JIP.Autorun();
    if(cb2  -> Checked) DIP.Autorun();
    if(cb3  -> Checked) PLC.Autorun();
    //if(cb4  -> Checked) VSN.Autorun();
    //if(cb5  -> Checked) PSB.Autorun();
    //if(cb6  -> Checked) LDR.Autorun();
    //if(cb7  -> Checked) ULD.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartResetClick(TObject *Sender)
{
    if(cb1  -> Checked) JIP.Reset();
    if(cb2  -> Checked) DIP.Reset();
    if(cb3  -> Checked) PLC.Reset();
    //if(cb4  -> Checked) VSN.Reset();
    //if(cb5  -> Checked) PSB.Reset();
    //if(cb6  -> Checked) LDR.Reset();
    //if(cb7  -> Checked) ULD.Reset();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAllResetClick(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartHomeClick(TObject *Sender)
{
    //if(cb1  -> Checked) {MM.SetManCycle(mcSTG_Home);}
}
//---------------------------------------------------------------------------
