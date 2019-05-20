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
#include "Rail.h"
#include "LaserEO.h"
#include "MarkVisn.h"
#include "PostBuff.h"
#include "Unloader.h"
#include "Rs232Laser.h"
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
    sgCycleStep -> Cells[0][ 2] = RAL.GetPartName() ; cb2  -> Caption = RAL.GetPartName() ;
    sgCycleStep -> Cells[0][ 3] = LSR.GetPartName() ; cb3  -> Caption = LSR.GetPartName() ;
    sgCycleStep -> Cells[0][ 4] = VSN.GetPartName() ; cb4  -> Caption = VSN.GetPartName() ;
    sgCycleStep -> Cells[0][ 5] = PSB.GetPartName() ; cb5  -> Caption = PSB.GetPartName() ;
    sgCycleStep -> Cells[0][ 6] = LDR.GetPartName() ; cb6  -> Caption = LDR.GetPartName() ;
    sgCycleStep -> Cells[0][ 7] = ULD.GetPartName() ; cb7  -> Caption = ULD.GetPartName() ;

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
        cbAllStrpSendLsrJob-> Checked = OM.MstOptn.bAllStrpSendLsrJob ;
        edTrigerOffset     -> Text    = OM.MstOptn.dTrigerOffset      ;
        edVisnRsltFile     -> Text    = OM.MstOptn.sVisnRsltFile      ;
        edVisnDeviceFile   -> Text    = OM.MstOptn.sVisnDeviceFile    ;
        edLaserDataFile    -> Text    = OM.MstOptn.sLaserDataFile     ;
        //edLSRFontCnt       -> Text    = OM.MstOptn.iMaxFontCnt        ;

        edVsnLsrYCntrPos   -> Text    = OM.MstOptn.dVsnLsrYCntrPos    ;

        edLsrXOfs          -> Text    = OM.MstOptn.dLsrXOfs           ;
        edLsrYOfs          -> Text    = OM.MstOptn.dLsrYOfs           ;
        edLsrTOfs          -> Text    = OM.MstOptn.dLsrTOfs           ;

        
    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode         -> Checked ;
        OM.MstOptn.bDryRun            = cbDryRun            -> Checked ;
        OM.MstOptn.bAllStrpSendLsrJob = cbAllStrpSendLsrJob -> Checked ;
        OM.MstOptn.sVisnRsltFile      = edVisnRsltFile      -> Text    ;
        OM.MstOptn.sVisnDeviceFile    = edVisnDeviceFile    -> Text    ;
        OM.MstOptn.sLaserDataFile     = edLaserDataFile     -> Text    ;
        OM.MstOptn.dTrigerOffset      = StrToFloatDef(edTrigerOffset  -> Text , 0.0);
        //OM.MstOptn.iMaxFontCnt        = StrToIntDef  (edLSRFontCnt    -> Text , 0  );

        OM.MstOptn.dVsnLsrYCntrPos    = StrToFloatDef(edVsnLsrYCntrPos-> Text , 0.0);

        OM.MstOptn.dLsrXOfs           = StrToFloatDef(edLsrXOfs       -> Text , 0.0);
        OM.MstOptn.dLsrYOfs           = StrToFloatDef(edLsrYOfs       -> Text , 0.0);
        OM.MstOptn.dLsrTOfs           = StrToFloatDef(edLsrTOfs       -> Text , 0.0);
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
    sgCycleStep->Cells[1][ 1]=PRB.GetHomeStep();sgCycleStep->Cells[2][ 1]=PRB.GetToStartStep();sgCycleStep->Cells[3][ 1]=PRB.GetCycleName();sgCycleStep->Cells[4][ 1]=PRB.GetCycleStep();sgCycleStep->Cells[5][ 1]=PRB.GetToStopStep();
    sgCycleStep->Cells[1][ 2]=RAL.GetHomeStep();sgCycleStep->Cells[2][ 2]=RAL.GetToStartStep();sgCycleStep->Cells[3][ 2]=RAL.GetCycleName();sgCycleStep->Cells[4][ 2]=RAL.GetCycleStep();sgCycleStep->Cells[5][ 2]=RAL.GetToStopStep();
    sgCycleStep->Cells[1][ 3]=LSR.GetHomeStep();sgCycleStep->Cells[2][ 3]=LSR.GetToStartStep();sgCycleStep->Cells[3][ 3]=LSR.GetCycleName();sgCycleStep->Cells[4][ 3]=LSR.GetCycleStep();sgCycleStep->Cells[5][ 3]=LSR.GetToStopStep();
    sgCycleStep->Cells[1][ 4]=VSN.GetHomeStep();sgCycleStep->Cells[2][ 4]=VSN.GetToStartStep();sgCycleStep->Cells[3][ 4]=VSN.GetCycleName();sgCycleStep->Cells[4][ 4]=VSN.GetCycleStep();sgCycleStep->Cells[5][ 4]=VSN.GetToStopStep();
    sgCycleStep->Cells[1][ 5]=PSB.GetHomeStep();sgCycleStep->Cells[2][ 5]=PSB.GetToStartStep();sgCycleStep->Cells[3][ 5]=PSB.GetCycleName();sgCycleStep->Cells[4][ 5]=PSB.GetCycleStep();sgCycleStep->Cells[5][ 5]=PSB.GetToStopStep();
    sgCycleStep->Cells[1][ 6]=LDR.GetHomeStep();sgCycleStep->Cells[2][ 6]=LDR.GetToStartStep();sgCycleStep->Cells[3][ 6]=LDR.GetCycleName();sgCycleStep->Cells[4][ 6]=LDR.GetCycleStep();sgCycleStep->Cells[5][ 6]=LDR.GetToStopStep();
    sgCycleStep->Cells[1][ 7]=ULD.GetHomeStep();sgCycleStep->Cells[2][ 7]=ULD.GetToStartStep();sgCycleStep->Cells[3][ 7]=ULD.GetCycleName();sgCycleStep->Cells[4][ 7]=ULD.GetCycleStep();sgCycleStep->Cells[5][ 7]=ULD.GetToStopStep();



    lbRsvMsg -> Caption = Rs232Laser.ReadMsg();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartAutorunClick(TObject *Sender)
{
    if(cb1  -> Checked) PRB.Autorun();
    if(cb2  -> Checked) RAL.Autorun();
    if(cb3  -> Checked) LSR.Autorun();
    if(cb4  -> Checked) VSN.Autorun();
    if(cb5  -> Checked) PSB.Autorun();
    if(cb6  -> Checked) LDR.Autorun();
    if(cb7  -> Checked) ULD.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartResetClick(TObject *Sender)
{
    if(cb1  -> Checked) PRB.Reset();
    if(cb2  -> Checked) RAL.Reset();
    if(cb3  -> Checked) LSR.Reset();
    if(cb4  -> Checked) VSN.Reset();
    if(cb5  -> Checked) PSB.Reset();
    if(cb6  -> Checked) LDR.Reset();
    if(cb7  -> Checked) ULD.Reset();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAllResetClick(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------





void __fastcall TFrmMaster::btVisnRsltFileClick(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVisnRsltFile->Text = dgFilePath->FileName;


}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edLaserDataFile ->Text = dgFilePath->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartHomeClick(TObject *Sender)
{
    if(cb1  -> Checked) {MM.SetManCycle(mcPRB_Home);}
    if(cb2  -> Checked) {MM.SetManCycle(mcRAL_Home);}
    if(cb3  -> Checked) {MM.SetManCycle(mcLSR_Home);}
    if(cb4  -> Checked) {MM.SetManCycle(mcVSN_Home);}
    if(cb5  -> Checked) {MM.SetManCycle(mcPSB_Home);}
    if(cb6  -> Checked) {MM.SetManCycle(mcLDR_Home);}
    if(cb7  -> Checked) {MM.SetManCycle(mcULD_Home);}
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button3Click(TObject *Sender)
{
    MT_OneShotAxtTrg(miVSN_XInsp , true , 1000);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVisnDeviceFile->Text = dgFilePath->FileName;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button4Click(TObject *Sender)
{
    if(MT_GetCmdPos(miLSR_YAlVs) != 0) MT_GoAbsRun(miLSR_YAlVs, 0);
    else                               MT_GoAbsRun(miLSR_YAlVs, OM.MstOptn.dVsnLsrYCntrPos);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{
    double dX = StrToFloatDef(edLsrXOfs       -> Text , 0.0);
    double dY = StrToFloatDef(edLsrYOfs       -> Text , 0.0);
    double dT = StrToFloatDef(edLsrTOfs       -> Text , 0.0);

    AnsiString sMsg;
    sMsg  = dX ;
    sMsg += ",";
    sMsg += dY ;
    sMsg += ",";
    sMsg += dT ;

    Rs232Laser.SendMsg(eoVAD, sMsg);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button6Click(TObject *Sender)
{
    IO_SetY(yLSC_Start, !IO_GetY(yLSC_Start));
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button7Click(TObject *Sender)
{
    //IO_SetY(yLSC_MarkEnable, !IO_GetY(yLSC_MarkEnable));
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button8Click(TObject *Sender)
{
    Rs232Laser.ClearReadMsg();
    EN_EO_CMD _SendCmd;
    _SendCmd = cbMsgCmd->ItemIndex ;
    Rs232Laser.SendMsg(_SendCmd, edLSRSendMsg->Text);
}
//---------------------------------------------------------------------------

