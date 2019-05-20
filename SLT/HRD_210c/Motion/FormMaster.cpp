//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "FormMaster.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "Stage.h"
#include "PostBuff.h"
#include "UnLoader.h"

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
    sgCycleStep -> Cells[0][3] = "RAE" ;
    sgCycleStep -> Cells[0][4] = "EPX" ;
    sgCycleStep -> Cells[0][5] = "RAD" ;
    sgCycleStep -> Cells[0][6] = "HED" ;
    sgCycleStep -> Cells[0][7] = "STG" ;
    sgCycleStep -> Cells[0][8] = "PSB" ;
    sgCycleStep -> Cells[0][9] = "ULD" ;


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
        cbbIgnrEpoxy     -> Checked = OM.MstOptn.bIgnrEpoxy     ;
        cbbIgnrDieAttach -> Checked = OM.MstOptn.bIgnrDieAttach ;

        ediEpxOriSpd     -> Text    = OM.MstOptn.dEpxOriSpd     ;
        eddEpxOriOfs     -> Text    = OM.MstOptn.dEpxOriOfs     ;

        eddVoiceCoilOffset -> Text  = OM.MstOptn.dVoiceCoilOffset     ;
        edVoiceCoilA       -> Text  = OM.MstOptn.dVoiceCoilA     ;
        edVoiceCoilB       -> Text  = OM.MstOptn.dVoiceCoilB     ;

        edd1stDis          -> Text  = OM.MstOptn.d1stDis     ;
        edd2stDis          -> Text  = OM.MstOptn.d2stDis     ;
        edd3stDis          -> Text  = OM.MstOptn.d3stDis     ;

        edStgXEjtCt        -> Text  = OM.MstOptn.dStgXEjtCt  ;
        edStgYEjtCt        -> Text  = OM.MstOptn.dStgYEjtCt  ;

        cbIgnrBfVision     -> Checked = OM.MstOptn.bIgnrBfVision ;
        cbIgnrWfVision     -> Checked = OM.MstOptn.bIgnrWfVision ;
        edEjtDia           -> Text  = OM.MstOptn.dEjtDia     ;
        edStgDia           -> Text  = OM.MstOptn.dStgDia     ;

        edStgYMaxExdAble   -> Text  = OM.MstOptn.dStgYMaxExdAble     ;
        edDeviceBackUpPath -> Text  = OM.MstOptn.sDeviceBackUpPath   ;

        edToolCrashWfrY    -> Text  = OM.MstOptn.dToolCrashWfrY ;
        edToolCrashRalY    -> Text  = OM.MstOptn.dToolCrashRalY ;
        edToolCrashZ       -> Text  = OM.MstOptn.dToolCrashZ    ;

        cbPickUpTestMode   -> Checked = OM.MstOptn.bPickUpTestMode ;
        cbEjectorDnFirst   -> Checked = OM.MstOptn.bEjectorDnFirst ;

        cbDryRun           -> Checked = OM.MstOptn.bDryRun ;

        edRegripOfs        -> Text  = OM.MstOptn.dRegripOfs ;
    }
    else{
        OM.MstOptn.bDebugMode       = cbDebugMode      -> Checked ;
        OM.MstOptn.bIgnrEpoxy       = cbbIgnrEpoxy     -> Checked ;
        OM.MstOptn.bIgnrDieAttach   = cbbIgnrDieAttach -> Checked ;

        OM.MstOptn.dEpxOriSpd       = StrToFloatDef(ediEpxOriSpd -> Text , 0.0 );
        OM.MstOptn.dEpxOriOfs       = StrToFloatDef(eddEpxOriOfs -> Text , 0.0 );

        OM.MstOptn.dVoiceCoilOffset = StrToFloatDef(eddVoiceCoilOffset -> Text , 0.0 );
        OM.MstOptn.dVoiceCoilA      = StrToFloatDef(edVoiceCoilA       -> Text , 0.0 );
        OM.MstOptn.dVoiceCoilB      = StrToFloatDef(edVoiceCoilB       -> Text , 0.0 );

        OM.MstOptn.d1stDis          = edd1stDis -> Text.ToDouble();
        OM.MstOptn.d2stDis          = edd2stDis -> Text.ToDouble();
        OM.MstOptn.d3stDis          = edd3stDis -> Text.ToDouble();

        OM.MstOptn.dStgXEjtCt       = edStgXEjtCt -> Text.ToDouble();
        OM.MstOptn.dStgYEjtCt       = edStgYEjtCt -> Text.ToDouble();

        OM.MstOptn.bIgnrBfVision    = cbIgnrBfVision     -> Checked  ;
        OM.MstOptn.bIgnrWfVision    = cbIgnrWfVision     -> Checked  ;

        OM.MstOptn.dEjtDia          = edEjtDia    -> Text.ToDouble();
        OM.MstOptn.dStgDia          = edStgDia    -> Text.ToDouble();

        OM.MstOptn.dStgYMaxExdAble  = edStgYMaxExdAble   -> Text.ToDouble()    ;
        OM.MstOptn.sDeviceBackUpPath= edDeviceBackUpPath -> Text    ;

        OM.MstOptn.dToolCrashWfrY   = edToolCrashWfrY    -> Text.ToDouble()  ;
        OM.MstOptn.dToolCrashRalY   = edToolCrashRalY    -> Text.ToDouble()  ;
        OM.MstOptn.dToolCrashZ      = edToolCrashZ       -> Text.ToDouble()  ;

        OM.MstOptn.bPickUpTestMode  = cbPickUpTestMode   -> Checked  ;
        OM.MstOptn.bEjectorDnFirst  = cbEjectorDnFirst   -> Checked  ;

        OM.MstOptn.bDryRun          = cbDryRun           -> Checked  ;

        OM.MstOptn.dRegripOfs       = edRegripOfs        -> Text.ToDouble() ;
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
    sgCycleStep -> Cells[1][1] = LDR.GetHomeStep   () ;
    sgCycleStep -> Cells[2][1] = LDR.GetToStartStep() ;
    sgCycleStep -> Cells[3][1] = LDR.GetCycleName  () ;
    sgCycleStep -> Cells[4][1] = LDR.GetCycleStep  () ;
    sgCycleStep -> Cells[5][1] = LDR.GetToStopStep () ;

    sgCycleStep -> Cells[1][2] = PRB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][2] = PRB.GetToStartStep() ;
    sgCycleStep -> Cells[3][2] = PRB.GetCycleName  () ;
    sgCycleStep -> Cells[4][2] = PRB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][2] = PRB.GetToStopStep () ;

    sgCycleStep -> Cells[1][3] = RAE.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = RAE.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = RAE.GetCycleName  () ;
    sgCycleStep -> Cells[4][3] = RAE.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = RAE.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = EPX.GetHomeStep   () ;
    sgCycleStep -> Cells[2][4] = EPX.GetToStartStep() ;
    sgCycleStep -> Cells[3][4] = EPX.GetCycleName  () ;
    sgCycleStep -> Cells[4][4] = EPX.GetCycleStep  () ;
    sgCycleStep -> Cells[5][4] = EPX.GetToStopStep () ;

    sgCycleStep -> Cells[1][5] = RAD.GetHomeStep   () ;
    sgCycleStep -> Cells[2][5] = RAD.GetToStartStep() ;
    sgCycleStep -> Cells[3][5] = RAD.GetCycleName  () ;
    sgCycleStep -> Cells[4][5] = RAD.GetCycleStep  () ;
    sgCycleStep -> Cells[5][5] = RAD.GetToStopStep () ;

    sgCycleStep -> Cells[1][6] = HED.GetHomeStep   () ;
    sgCycleStep -> Cells[2][6] = HED.GetToStartStep() ;
    sgCycleStep -> Cells[3][6] = HED.GetCycleName  () ;
    sgCycleStep -> Cells[4][6] = HED.GetCycleStep  () ;
    sgCycleStep -> Cells[5][6] = HED.GetToStopStep () ;

    sgCycleStep -> Cells[1][7] = STG.GetHomeStep   () ;
    sgCycleStep -> Cells[2][7] = STG.GetToStartStep() ;
    sgCycleStep -> Cells[3][7] = STG.GetCycleName  () ;
    sgCycleStep -> Cells[4][7] = STG.GetCycleStep  () ;
    sgCycleStep -> Cells[5][7] = STG.GetToStopStep () ;

    sgCycleStep -> Cells[1][8] = PSB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][8] = PSB.GetToStartStep() ;
    sgCycleStep -> Cells[3][8] = PSB.GetCycleName  () ;
    sgCycleStep -> Cells[4][8] = PSB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][8] = PSB.GetToStopStep () ;

    sgCycleStep -> Cells[1][9] = ULD.GetHomeStep   () ;
    sgCycleStep -> Cells[2][9] = ULD.GetToStartStep() ;
    sgCycleStep -> Cells[3][9] = ULD.GetCycleName  () ;
    sgCycleStep -> Cells[4][9] = ULD.GetCycleStep  () ;
    sgCycleStep -> Cells[5][9] = ULD.GetToStopStep () ;

    Edit5 -> Text = DM.ARAY[riPRB].GetStep() ;
    Edit6 -> Text = DM.ARAY[riREF].GetStep() ;
    Edit7 -> Text = DM.ARAY[riRDF].GetStep() ;
    Edit8 -> Text = DM.ARAY[riPSF].GetStep() ;

    double dGapPosX = MT_GetCmdPos(miWRD_XStg) - OM.MstOptn.dStgXEjtCt ;
    double dGapPosY = MT_GetCmdPos(miWRD_YStg) - OM.MstOptn.dStgYEjtCt ;

    double dDist = pow(pow(dGapPosX,2) + pow(dGapPosY,2) , 0.5);


    Label27 -> Caption = "Distance from CntrPos: " + AnsiString(dDist) ;


    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbPart1 -> Checked) LDR.Autorun();
    if(cbPart2 -> Checked) PRB.Autorun();
    if(cbPart3 -> Checked) RAE.Autorun();
    if(cbPart4 -> Checked) EPX.Autorun();
    if(cbPart5 -> Checked) RAD.Autorun();
    if(cbPart6 -> Checked) HED.Autorun();
    if(cbPart7 -> Checked) STG.Autorun();
    if(cbPart8 -> Checked) PSB.Autorun();
    if(cbPart9 -> Checked) ULD.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbPart1 -> Checked) LDR.Reset();
    if(cbPart2 -> Checked) PRB.Reset();
    if(cbPart3 -> Checked) RAE.Reset();
    if(cbPart4 -> Checked) EPX.Reset();
    if(cbPart5 -> Checked) RAD.Reset();
    if(cbPart6 -> Checked) HED.Reset();
    if(cbPart7 -> Checked) STG.Reset();
    if(cbPart8 -> Checked) PSB.Reset();
    if(cbPart9 -> Checked) ULD.Reset();

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

void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    TButton * pBt  = (TButton *)Sender ;
    int iTag       =  pBt -> Tag ;

    if(iTag == 0) DM.ARAY[riPRB].SetStat(csNone);
    if(iTag == 1) DM.ARAY[riREF].SetStat(csNone);
    if(iTag == 2) DM.ARAY[riRDF].SetStat(csNone);
    if(iTag == 3) DM.ARAY[riPSF].SetStat(csNone);



}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{
    TButton * pBt  = (TButton *)Sender ;
    int iTag       =  pBt -> Tag ;

    if(iTag == 0) DM.ARAY[riPRB].SetStep(Edit1->Text.ToIntDef(0) );
    if(iTag == 1) DM.ARAY[riREF].SetStep(Edit2->Text.ToIntDef(1) );
    if(iTag == 2) DM.ARAY[riRDF].SetStep(Edit3->Text.ToIntDef(2) );
    if(iTag == 3) DM.ARAY[riPSF].SetStep(Edit4->Text.ToIntDef(3) );

}
//---------------------------------------------------------------------------



void __fastcall TFrmMaster::Button9Click(TObject *Sender)
{
    LDR._iMgzCnt = Edit9 -> Text.ToIntDef(0);    
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button13Click(TObject *Sender)
{
   edToolCrashWfrY -> Text = MT_GetCmdPos(miWRD_YHed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button15Click(TObject *Sender)
{
   edToolCrashZ -> Text = MT_GetCmdPos(miWRD_ZHed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button14Click(TObject *Sender)
{
   edToolCrashRalY -> Text = MT_GetCmdPos(miWRD_YHed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button16Click(TObject *Sender)
{
    if(!AT_Complete(aiEJT_UpDn , ccBwd)) {
        FM_MsgOk("ERROR","Ejector is Fwd");
        return ;
    }
    if(!MT_CmprPos(miWRD_ZExd,PM.GetValue(miWRD_ZExd,pvWRD_ZExdExpend))) {
        FM_MsgOk("ERROR","Expend_Z is Not Expend Position");
        return ;
    }

    MT_GoAbsMan(miWRD_XStg , edStgXEjtCt -> Text.ToDouble());
    MT_GoAbsMan(miWRD_YStg , edStgYEjtCt -> Text.ToDouble());
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button17Click(TObject *Sender)
{
    if(AT_Complete(aiEJT_UpDn , ccBwd)) AT_MoveCyl(aiEJT_UpDn , ccFwd);
    else                                AT_MoveCyl(aiEJT_UpDn , ccBwd);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btFixClick(TObject *Sender)
{
    PM.SetType(miWRD_YHed,pvWRD_YHedWaferWait ,vtFixValue) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedWaferZMove,vtFixValue) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripWait ,vtFixValue) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripZMove,vtFixValue) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btNotFixClick(TObject *Sender)
{
    PM.SetType(miWRD_YHed,pvWRD_YHedWaferWait ,vtPosition) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedWaferZMove,vtPosition) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripWait ,vtPosition) ;
    PM.SetType(miWRD_YHed,pvWRD_YHedStripZMove,vtPosition) ;

}
//---------------------------------------------------------------------------




