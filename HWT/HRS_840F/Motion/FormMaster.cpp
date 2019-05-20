//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------

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
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
    }
    else{
    }
}
//---------------------------------------------------------------------------
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

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartAutorunClick(TObject *Sender)
{

    for(int i= 0 ; i < MAX_PART ; i++) {
        if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Autorun();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartResetClick(TObject *Sender)
{
    for(int i= 0 ; i < MAX_PART ; i++) {
        if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Reset();
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
    if(cbPart[piLDR_F] -> Checked) {
        MM.SetManCycle(mcLDR_FHome); }
    else if(cbPart[piLDR_R] -> Checked) {
        MM.SetManCycle(mcLDR_RHome); }
    else if(cbPart[piPRE_F] -> Checked) {
        MM.SetManCycle(mcRAL_FHome); }
    else if(cbPart[piPRE_R] -> Checked) {
        MM.SetManCycle(mcRAL_RHome); }
    else if(cbPart[piULD_F] -> Checked) {
        MM.SetManCycle(mcULD_FHome); }
    else if(cbPart[piULD_R] -> Checked) {
        MM.SetManCycle(mcULD_RHome); }

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
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::cbPartSelChange(TObject *Sender)
{
    UpdateTimeName();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::btAllCheckClick(TObject *Sender)
{
    bool bChecked = cbPart[0] -> Checked ;

    for(int i = 0 ; i < MAX_PART ; i++) {
        cbPart[i] -> Checked = !bChecked ;
    }
}
//---------------------------------------------------------------------------





















