//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Loader.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Rail.h"
#include "UnLoader.h"
#include "Picker.h"
#include "Sorter.h"
#include "Supply.h"
#include "Rs232Man.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "LDR"    ;
    sgCycleStep -> Cells[0][2] = "RAL"    ;
    sgCycleStep -> Cells[0][3] = "ULD"    ;
    sgCycleStep -> Cells[0][4] = "PICK"   ;
    sgCycleStep -> Cells[0][5] = "SORTER" ;
//    sgCycleStep -> Cells[0][6] = "SUPPLY" ;

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
        cbDebugMode     -> Checked = OM.MstOptn.bDebugMode    ;
        cbRandomSort    -> Checked = OM.MstOptn.bRandomSort   ;
        cbRandomSupply  -> Checked = OM.MstOptn.bRandomSupply ;
        cbCheckCycle    -> Checked = OM.MstOptn.bCheckCycle   ;

        edMgzShowTime   -> Text    = OM.MstOptn.iMgzShowTime  ;

    }
    else{
        OM.MstOptn.bDebugMode      = cbDebugMode     -> Checked ;
        OM.MstOptn.bRandomSort     = cbRandomSort    -> Checked ;
        OM.MstOptn.bRandomSupply   = cbRandomSupply  -> Checked ;
        OM.MstOptn.bCheckCycle     = cbCheckCycle    -> Checked ;

        OM.MstOptn.iMgzShowTime    = edMgzShowTime   -> Text.ToIntDef(3000) ;
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
    //LDR
    sgCycleStep -> Cells[1][1] = LDR.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = LDR.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = LDR.GetSeqStep    () ;
    sgCycleStep -> Cells[4][1] = LDR.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = LDR.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = RAL.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = RAL.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = RAL.GetSeqStep    () ;
    sgCycleStep -> Cells[4][2] = RAL.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = RAL.GetToStopStep () ;

    sgCycleStep -> Cells[1][3] = ULD.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = ULD.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = ULD.GetSeqStep    () ;
    sgCycleStep -> Cells[4][3] = ULD.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = ULD.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = PCK.GetHomeStep   () ;
    sgCycleStep -> Cells[2][4] = PCK.GetToStartStep() ;
    sgCycleStep -> Cells[3][4] = PCK.GetSeqStep    () ;
    sgCycleStep -> Cells[4][4] = PCK.GetCycleStep  () ;
    sgCycleStep -> Cells[5][4] = PCK.GetToStopStep () ;

    sgCycleStep -> Cells[1][5] = SRT.GetHomeStep   () ;
    sgCycleStep -> Cells[2][5] = SRT.GetToStartStep() ;
    sgCycleStep -> Cells[3][5] = SRT.GetSeqStep    () ;
    sgCycleStep -> Cells[4][5] = SRT.GetCycleStep  () ;
    sgCycleStep -> Cells[5][5] = SRT.GetToStopStep () ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbPart1 -> Checked) LDR.Autorun();
    if(cbPart2 -> Checked) RAL.Autorun();
    if(cbPart3 -> Checked) ULD.Autorun();
    if(cbPart4 -> Checked) PCK.Autorun();
    if(cbPart5 -> Checked) SRT.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbPart1 -> Checked) LDR.Reset();
    if(cbPart2 -> Checked) RAL.Reset();
    if(cbPart3 -> Checked) ULD.Reset();
    if(cbPart4 -> Checked) PCK.Reset();
    if(cbPart5 -> Checked) SRT.Reset();

    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------
                    
void __fastcall TFrmMaster::btWChipClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Equip Worked Chip Count Clear ?") != mrYes ) return ;
    LT.EqpInfo.iTotalChip = 0 ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::btFChipClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Equip Failed Chip Count Clear ?") != mrYes ) return ;
    LT.EqpInfo.iTotalFailChip = 0;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    String sTemp = Edit1->Text ;
    Rs232_H->SendData(sTemp.Length(),sTemp.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
    //String sTemp = "ID01R";
    //Rs232_H->WriteData(sTemp.Length(),sTemp.c_str());
    //mmRs232->Lines->Add(">"+sTemp);
    String sTemp = "ID01P";
    Rs232_H->SendData(sTemp.Length(),sTemp.c_str());
    mmRs232->Lines->Add(">"+sTemp);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button3Click(TObject *Sender)
{
    int iTemp = 0 ;
    iTemp = Rs232_H->IsOpened() ;
    mmRs232->Lines->Add((String)iTemp);
    iTemp = Rs232_L->IsOpened() ;
    mmRs232->Lines->Add((String)iTemp);
    iTemp = Rs232_R->IsOpened() ;
    mmRs232->Lines->Add((String)iTemp);

/*
    String sTemp ;
    sTemp = Rs232_H->sData ;
    mmRs232->Lines->Add(sTemp);
    sTemp = Rs232_L->sData ;
    mmRs232->Lines->Add(sTemp);
    sTemp = Rs232_R->sData ;
    mmRs232->Lines->Add(sTemp);
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false ;    
}
//---------------------------------------------------------------------------


