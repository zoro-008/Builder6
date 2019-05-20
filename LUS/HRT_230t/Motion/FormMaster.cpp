//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "OptionMan.h"
#include "Sequence.h"
//#include "LotUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Feed.h"
#include "Rail.h"
#include "Work.h"
#include "Sort.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{

    sgCycleStep -> Cells[0][1] = "FED" ;
    sgCycleStep -> Cells[0][2] = "RAL" ;
    sgCycleStep -> Cells[0][3] = "WRK" ;
    sgCycleStep -> Cells[0][4] = "SRT" ;


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
        cbDebugMode      -> Checked = OM.MstOptn.bDebugMode     ;
        cbInspSkip       -> Checked = OM.MstOptn.bInspSkip      ;
        cbIgnrPkgDustChk -> Checked = OM.MstOptn.bIgnrPkgDustChk;
        cbFedMTPsChange  -> Checked = OM.MstOptn.bFedMTPsChange ;
    }
    else{
        OM.MstOptn.bDebugMode       = cbDebugMode      -> Checked ;
        OM.MstOptn.bInspSkip        = cbInspSkip       -> Checked ;
        OM.MstOptn.bIgnrPkgDustChk  = cbIgnrPkgDustChk -> Checked ;
        OM.MstOptn.bFedMTPsChange   = cbFedMTPsChange  -> Checked ; 

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
        edModelName -> Text   = OM.EqpOptn.sModelName ;

    }
    else{
        OM.EqpOptn.sModelName = edModelName -> Text   ;
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
    tmUpdate -> Enabled = false ;
    //LDR
    sgCycleStep -> Cells[1][1] = FED.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = FED.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = FED.GetSeqStep    () ;
    sgCycleStep -> Cells[4][1] = FED.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = FED.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = RAL.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = RAL.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = RAL.GetSeqStep    () ;
    sgCycleStep -> Cells[4][2] = RAL.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = RAL.GetToStopStep () ;

    sgCycleStep -> Cells[1][3] = WRK.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = WRK.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = WRK.GetSeqStep    () ;
    sgCycleStep -> Cells[4][3] = WRK.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = WRK.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = SRT.GetHomeStep   () ;
    sgCycleStep -> Cells[2][4] = SRT.GetToStartStep() ;
    sgCycleStep -> Cells[3][4] = SRT.GetSeqStep    () ;
    sgCycleStep -> Cells[4][4] = SRT.GetCycleStep  () ;
    sgCycleStep -> Cells[5][4] = SRT.GetToStopStep () ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbPart1 -> Checked) FED.Autorun();
    if(cbPart2 -> Checked) RAL.Autorun();
    if(cbPart3 -> Checked) WRK.Autorun();
    if(cbPart4 -> Checked) SRT.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbPart1 -> Checked) FED.Reset();
    if(cbPart2 -> Checked) RAL.Reset();
    if(cbPart3 -> Checked) WRK.Reset();
    if(cbPart4 -> Checked) SRT.Reset();

    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false ;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btChipStatsCngClick(TObject *Sender)
{
    static bool bTemp = true ;
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        DM.ARAY[i].PopupMenuEnable(bTemp);
    }
    bTemp = !bTemp ;
}
//---------------------------------------------------------------------------




