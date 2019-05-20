//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "FormMaster.h"
#include "OptionMan.h"
#include "Sequence.h"
//#include "PstnMan.h"
//#include "LotUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{

    sgCycleStep -> Cells[0][1] = "STG" ;


    sgCycleStep -> Cells[1][0] = "Idle " ;
    sgCycleStep -> Cells[2][0] = "Home " ;
    sgCycleStep -> Cells[3][0] = "Work " ;
    sgCycleStep -> Cells[4][0] = "Place" ;

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
        edCycleTm -> Text = OM.MstOptn.iCycleTm ;
    }
    else{
        OM.MstOptn.iCycleTm = StrToIntDef(edCycleTm -> Text,20000);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{   /*
    if(bTable){
        edModelName -> Text   = OM.EqpOptn.sModelName ;

    }
    else{
        OM.EqpOptn.sModelName = edModelName -> Text   ;
    } */
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

    //double dGapPosX = MT_GetCmdPos(miWRD_XStg) - OM.MstOptn.dStgXEjtCt ;
    //double dGapPosY = MT_GetCmdPos(miWRD_YStg) - OM.MstOptn.dStgYEjtCt ;

    //double dDist = pow(pow(dGapPosX,2) + pow(dGapPosY,2) , 0.5);


    //Label27 -> Caption = "Distance from CntrPos: " + AnsiString(dDist) ;


    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    STG.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    STG.Reset();

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
   // if(FM_MsgYesNo("Confirm","Equip Worked Chip Count Clear ?") != mrYes ) return ;
   // LT.EqpInfo.iTotalWork = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btFChipClearClick(TObject *Sender)
{
    //if(FM_MsgYesNo("Confirm","Equip Failed Chip Count Clear ?") != mrYes ) return ;
    //LT.EqpInfo.iTotalFailChip = 0;

}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false ;    
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::btChipStatsCngClick(TObject *Sender)
{
    /*static bool bTemp = true ;
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        DM.ARAY[i].PopupMenuEnable(bTemp);
    }
    bTemp = !bTemp ;*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
   /* TButton * pBt  = (TButton *)Sender ;
    int iTag       =  pBt -> Tag ;

    if(iTag == 0) DM.ARAY[riPRB].SetStat(csNone);
    if(iTag == 1) DM.ARAY[riREF].SetStat(csNone);
    if(iTag == 2) DM.ARAY[riRDF].SetStat(csNone);
    if(iTag == 3) DM.ARAY[riPSF].SetStat(csNone);
     */


}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{
    /*TButton * pBt  = (TButton *)Sender ;
    int iTag       =  pBt -> Tag ;

    if(iTag == 0) DM.ARAY[riPRB].SetStep(Edit1->Text.ToIntDef(0) );
    if(iTag == 1) DM.ARAY[riREF].SetStep(Edit2->Text.ToIntDef(1) );
    if(iTag == 2) DM.ARAY[riRDF].SetStep(Edit3->Text.ToIntDef(2) );
    if(iTag == 3) DM.ARAY[riPSF].SetStep(Edit4->Text.ToIntDef(3) );
      */
}
//---------------------------------------------------------------------------



void __fastcall TFrmMaster::Button9Click(TObject *Sender)
{
   // LDR._iMgzCnt = Edit9 -> Text.ToIntDef(0);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button13Click(TObject *Sender)
{
   //edToolCrashWfrY -> Text = MT_GetCmdPos(miWRD_YHed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button15Click(TObject *Sender)
{
   //edToolCrashZ -> Text = MT_GetCmdPos(miWRD_ZHed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button14Click(TObject *Sender)
{
   //edToolCrashRalY -> Text = MT_GetCmdPos(miWRD_YHed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button16Click(TObject *Sender)
{
    /*if(!AT_Complete(aiEJT_UpDn , ccBwd)) {
        FM_MsgOk("ERROR","Ejector is Fwd");
        return ;
    }
    if(!MT_CmprPos(miWRD_ZExd,PM.GetValue(miWRD_ZExd,pvWRD_ZExdExpend))) {
        FM_MsgOk("ERROR","Expend_Z is Not Expend Position");
        return ;
    }

    MT_GoAbsMan(miWRD_XStg , edStgXEjtCt -> Text.ToDouble());
    MT_GoAbsMan(miWRD_YStg , edStgYEjtCt -> Text.ToDouble()); */
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button17Click(TObject *Sender)
{
    //if(AT_Complete(aiEJT_UpDn , ccBwd)) AT_MoveCyl(aiEJT_UpDn , ccFwd);
    //else                                AT_MoveCyl(aiEJT_UpDn , ccBwd);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btFixClick(TObject *Sender)
{
   /* PM.SetType(miWRD_YHed,pvWRD_YHedWaferWait ,vtFixValue) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedWaferZMove,vtFixValue) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripWait ,vtFixValue) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripZMove,vtFixValue) ;
     */
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btNotFixClick(TObject *Sender)
{
    /*PM.SetType(miWRD_YHed,pvWRD_YHedWaferWait ,vtPosition) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedWaferZMove,vtPosition) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripWait ,vtPosition) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripZMove,vtPosition) ;
      */
}
//---------------------------------------------------------------------------




