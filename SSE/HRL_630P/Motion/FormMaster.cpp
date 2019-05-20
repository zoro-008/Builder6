//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "ManualMan.h"
#include "sLogUnit.h"
#include "LaserConUnit.h"
//---------------------------------------------------------------------------
#include "FrontFeed.h"
#include "FrontLaser.h"
#include "LearFeed.h"
#include "Learlaser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> RowCount = MAX_PART + 1 ;
    sgCycleStep -> ColWidths[0,0] = 100;
    sgCycleStep -> Cells[1][0] = "Home    " ;
    sgCycleStep -> Cells[2][0] = "ToStart " ;
    sgCycleStep -> Cells[3][0] = "Seq     " ;
    sgCycleStep -> Cells[4][0] = "Cycle   " ;
    sgCycleStep -> Cells[5][0] = "ToStop  " ;
    sgCycleStep -> Cells[6][0] = "Pre Time" ;

    const int iCheckBoxGap = 25 ;
    for(int i= 0 ; i < MAX_PART ; i++) {
        cbPart[i] = new TCheckBox (this);
        cbPart[i] -> Parent = GroupBox8 ;
        cbPart[i] -> Left   = 10        ;
        cbPart[i] -> Top    = 20 + iCheckBoxGap * i ;
        cbPart[i] -> Font -> Size = 10;
        cbPart[i] -> Caption = SEQ.m_pPart[i]->GetPartName();
        sgCycleStep -> Cells[0][i+1]  = SEQ.m_pPart[i]->GetPartName();
    }


    for(int i= 0 ; i < MAX_PART ; i++) {
        sgCycleStep -> Cells[0][i+1]  = SEQ.m_pPart[i]->GetPartName();
    }

    cbPartSel -> Clear() ;
    for(int i = 0 ; i < MAX_PART ; i++) {
        cbPartSel -> AddItem(SEQ.m_pPart[i]->GetPartName(),NULL) ;
    }
    cbPartSel -> ItemIndex = 0 ;

    

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
    UpdateTimeName();
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateMstOptn(bool bTable)
{
    if(bTable){
        cbDebugMode  -> Checked = OM.MstOptn.bDebugMode          ;
        cbDryRun     -> Checked = OM.MstOptn.bDryRun             ;
        edPkgCnt     -> Text    = OM.MstOptn.iPkgCnt             ;
        edMtrOfset   -> Text    = OM.MstOptn.dOffset             ;

    }
    else{
        OM.MstOptn.bDebugMode  = cbDebugMode              -> Checked    ;
        OM.MstOptn.bDryRun     = cbDryRun                 -> Checked    ;
        OM.MstOptn.iPkgCnt     = StrToIntDef  (edPkgCnt   -> Text,15  ) ;
        OM.MstOptn.dOffset     = StrToFloatDef(edMtrOfset -> Text,0.01) ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
        edModelName -> Text = OM.EqpOptn.sModelName ;
    }
    else{
        OM.EqpOptn.sModelName = edModelName -> Text ;
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
    tmUpdate -> Enabled = false;

    //lbMan -> Caption = "<Man No = " + AnsiString(MM.GetManNo()) + "> <Man.m_iStep = " + MM.GetManStep() ;
    //LDR

    for(int i= 0 ; i < MAX_PART ; i++) {
        sgCycleStep->Cells[1][i+1]=SEQ.m_pPart[i]->GetHomeStep   ();
        sgCycleStep->Cells[2][i+1]=SEQ.m_pPart[i]->GetToStartStep();
        sgCycleStep->Cells[3][i+1]=SEQ.m_pPart[i]->GetCycleName  (SEQ.m_pPart[i]->GetSeqStep());
        sgCycleStep->Cells[4][i+1]=SEQ.m_pPart[i]->GetCycleStep  ();
        sgCycleStep->Cells[5][i+1]=SEQ.m_pPart[i]->GetToStopStep ();
        sgCycleStep->Cells[6][i+1]=SEQ.m_pPart[i]->GetCycleTime  (SEQ.m_pPart[i]->GetSeqStep());
    }

    for(int i=0; i< SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleMaxCnt(); i++ ) {
        sgTime -> Cells[1][i] = SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleTime(i);
    }






    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartAutorunClick(TObject *Sender)
{
    for(int i= 0 ; i < MAX_PART ; i++) {
        if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Autorun() ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartResetClick(TObject *Sender)
{
    for(int i= 0 ; i < MAX_PART ; i++) {
        if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Reset() ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAllResetClick(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartHomeClick(TObject *Sender)
{
    
    if(cbPart[piFFD] -> Checked) MM.SetManCycle(mcFFD_Home);
    if(cbPart[piLFD] -> Checked) MM.SetManCycle(mcLFD_Home);
    if(cbPart[piFLS] -> Checked) MM.SetManCycle(mcFLS_Home);
    if(cbPart[piLLS] -> Checked) MM.SetManCycle(mcLLS_Home);


}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{
     ShowMessage(LCU.GetChksum("GA,0020,"));
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btSendClick(TObject *Sender)
{
    LCU.SendMsg(edGAMsg -> Text) ;
}
//---------------------------------------------------------------------------
void TFrmMaster::UpdateTimeName()
{
    sgTime-> RowCount = SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleMaxCnt() ;
    for(int i=0; i< sgTime->RowCount; i++ ) {
        sgTime->Rows[i]->Clear();
    }
    for(int i=0; i< SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleMaxCnt(); i++ ) {
        sgTime -> Cells[0][i] = SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleName(i);
    }
}

void __fastcall TFrmMaster::cbPartSelChange(TObject *Sender)
{
    UpdateTimeName();
}
//---------------------------------------------------------------------------

