//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
//#include "UtilDefine.h"
#include "OptionMan.h"
//#include "Loader.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "LotUnit.h"
#include "SLogUnit.h"
#include "QuedataUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Head.h"
#include "Stage.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "STG" ;
    sgCycleStep -> Cells[0][2] = "HED" ;

    sgCycleStep -> Cells[1][0] = "Home   " ;
    sgCycleStep -> Cells[2][0] = "ToStart" ;
    sgCycleStep -> Cells[3][0] = "Seq    " ;
    sgCycleStep -> Cells[4][0] = "Cycle  " ;
    sgCycleStep -> Cells[5][0] = "ToStop " ;

    //¾Æ·¡¼­ AI¸§ ´U½A ¹U²Þ;;;;JS
    //sgCycleTime -> Cells[0][1] = "STG" ;
    //sgCycleTime -> Cells[0][2] = "HED" ;

    sgCycleTime -> Cells[1][0] = "Pitch" ;
    sgCycleTime -> Cells[2][0] = "Work " ;

    cbPart -> Clear() ;
    cbPart -> AddItem("STG",NULL);
    cbPart -> AddItem("HED",NULL);
    cbPart -> ItemIndex = 0 ;

    SetPartTime();
}

void TFrmMaster::SetPartTime()
{
    int iIndex = cbPart -> ItemIndex ;

    if(iIndex ==0) { //Stage
        sgCycleTime -> RowCount = CStage::MAX_SEQ_CYCLE + 1 ;
        for(int i = 1 ; i < CStage::MAX_SEQ_CYCLE ; i++) {
            sgCycleTime -> Cells[0][i] = STG.m_sCycleName[i] ;
            sgCycleTime -> Cells[1][i] = STG.GetTimePitch(i) ;
            sgCycleTime -> Cells[2][i] = STG.GetTimeWork (i) ;

        }

    }
    else if(iIndex ==1) {
        sgCycleTime -> RowCount = CHead::MAX_SEQ_CYCLE + 1 ;
        for(int i = 1 ; i < CHead::MAX_SEQ_CYCLE ; i++) {
            sgCycleTime -> Cells[0][i] = HED.m_sCycleName[i] ;
            sgCycleTime -> Cells[1][i] = HED.GetTimePitch(i) ;
            sgCycleTime -> Cells[2][i] = HED.GetTimeWork (i) ;

        }
    }
}


//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btSaveMotorClick(TObject *Sender)
{
    UpdateMstOptn (toBuff);
    UpdateEqpOptn (toBuff);
    OM.SaveMstOptn(      );
    OM.SaveEqpOptn(      );
    Trace("Operator", "FormMaster Form_Save Button Click");
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
        cbDryRun           -> Checked = OM.MstOptn.bDryRun           ;
        cbVacuum           -> Checked = OM.MstOptn.bVacuum           ;
        edDeviceBackUpPath -> Text    = OM.MstOptn.sDeviceBackUpPath ;
        cbHopJumpChip      -> Checked = OM.MstOptn.bHopJumpChip      ;
    }
    else{
        bool       bTemp ;
        AnsiString sTemp ;

        bTemp = cbDebugMode         -> Checked; if (OM.MstOptn.bDebugMode        != bTemp) { Trace("Operator" , "Both Mst_DebugMode values are different."); }
        bTemp = cbDryRun            -> Checked; if (OM.MstOptn.bDryRun           != bTemp) { Trace("Operator" , "Both Mst_DryRun values are different."); }
        bTemp = cbDryRun            -> Checked; if (OM.MstOptn.bVacuum           != bTemp) { Trace("Operator" , "Both Mst_Vacuum values are different."); }
        sTemp = edDeviceBackUpPath  -> Text   ; if (OM.MstOptn.sDeviceBackUpPath != sTemp) { Trace("Operator" , "Both Mst_DeviceBackUpPath values are different."); }
        bTemp = cbHopJumpChip       -> Checked; if (OM.MstOptn.bHopJumpChip      != bTemp) { Trace("Operator" , "Both Mst_Hop And Jump Chip values are different."); }

        OM.MstOptn.bDebugMode        = cbDebugMode         -> Checked         ;
        OM.MstOptn.bDryRun           = cbDryRun            -> Checked         ;
        OM.MstOptn.bVacuum           = cbVacuum            -> Checked         ;
        OM.MstOptn.sDeviceBackUpPath = edDeviceBackUpPath  -> Text            ;
        OM.MstOptn.bHopJumpChip      = cbHopJumpChip       -> Checked         ;
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
    sgCycleStep -> Cells[1][1] = STG.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = STG.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = STG.GetCycleName  () ;
    sgCycleStep -> Cells[4][1] = STG.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = STG.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = HED.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = HED.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = HED.GetCycleName  () ;
    sgCycleStep -> Cells[4][2] = HED.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = HED.GetToStopStep () ;

    SetPartTime();

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbPart1 -> Checked) STG.Autorun();
    if(cbPart2 -> Checked) HED.Autorun();
    Trace("Operator", "FormMaster Form_Auto Run Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbPart1 -> Checked) STG.Reset();
    if(cbPart2 -> Checked) HED.Reset();

    SEQ.Reset();
    Trace("Operator", "FormMaster Form_Reset Button Click");
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    SEQ.Reset();
    Trace("Operator", "FormMaster Form_Error Reset Button Click");
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btWChipClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","작업한 자제 카운터를 초기화 하시겠습니까?") != mrYes ) return ;
    LT.EqpInfo.iTotalChip = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btFChipClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","불량 자제 카운터를 초기화 하시겠습니까?") != mrYes ) return ;
    LT.EqpInfo.iTotalFailChip = 0;

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




void __fastcall TFrmMaster::cbPartChange(TObject *Sender)
{
    SetPartTime();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    StringGrid1 -> RowCount = MAX_QUE_DATA_CNT+1 ;

    StringGrid1 -> Cells[0][0] = "No" ;
    StringGrid1 -> Cells[1][0] = "C"  ;
    StringGrid1 -> Cells[2][0] = "R"  ;
    StringGrid1 -> Cells[3][0] = "F/E";


    for(int i= 1 ; i < MAX_QUE_DATA_CNT+1 ; i++) {
        StringGrid1 -> Cells[0][i] = i-1 ;
        StringGrid1 -> Cells[1][i] = HED.MarkData.GetDataC(i-1);
        StringGrid1 -> Cells[2][i] = HED.MarkData.GetDataR(i-1);
        StringGrid1 -> Cells[3][i] = HED.MarkData.GetDataF(i-1) ? "F":"E";
    }

}
//---------------------------------------------------------------------------

