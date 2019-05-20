//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "VisnComUnit.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"

#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "UnLoaderMotor.h"


#include "Sequence.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{
    sgCycleStep -> Cells[0][1] = "LDR" ;
    sgCycleStep -> Cells[0][2] = "RAL" ;
    sgCycleStep -> Cells[0][3] = "HED" ;
    sgCycleStep -> Cells[0][4] = "PSB" ;
    sgCycleStep -> Cells[0][5] = "ULD" ;
    sgCycleStep -> Cells[0][6] = "ULM" ;

    sgCycleStep -> Cells[1][0] = "Home"    ;
    sgCycleStep -> Cells[2][0] = "ToStart" ;
    sgCycleStep -> Cells[3][0] = "Seq"     ;
    sgCycleStep -> Cells[4][0] = "Cycle"   ;
    sgCycleStep -> Cells[5][0] = "ToStop"  ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::FormShow(TObject *Sender)
{
    OM.LoadMstOptn(      );
    UpdateMstOptn (toTabl);
    tmMaster1 -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::FormHide(TObject *Sender)
{
    tmMaster1 -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::UpdateMstOptn(bool toTabl)
{
    if(toTabl){
        edVisnPath        -> Text    = OM.MstOptn.sVisnPath       ;
        cbDebugMode       -> Checked = OM.MstOptn.bDebugMode      ;
        cbUld_YHome       -> Checked = OM.MstOptn.bUld_YHome      ;
        cbLdr_YHome       -> Checked = OM.MstOptn.bLdr_YHome      ;
        cbDataAccEng      -> Checked = OM.MstOptn.bDataAccEng     ;
        edTrigerOffset    -> Text    = OM.MstOptn.dTrigerOffset   ;
        cbPsbVsErrIgnr    -> Checked = OM.MstOptn.bPsbVsErrIgnr   ;
        cbDryRun          -> Checked = OM.CmnOptn.bDryRun         ; //������� ������ �ɼ����� �ٲٱ�.
    }
    else{
        OM.MstOptn.sVisnPath         = edVisnPath        -> Text            ;
        OM.MstOptn.bDebugMode        = cbDebugMode       -> Checked         ;
        OM.MstOptn.bUld_YHome        = cbUld_YHome       -> Checked         ;
        OM.MstOptn.bLdr_YHome        = cbLdr_YHome       -> Checked         ;
        OM.MstOptn.bDataAccEng       = cbDataAccEng      -> Checked         ;
        OM.MstOptn.dTrigerOffset     = edTrigerOffset    -> Text.ToDouble() ;
        OM.MstOptn.bPsbVsErrIgnr     = cbPsbVsErrIgnr    -> Checked         ;
        OM.CmnOptn.bDryRun           = cbDryRun          -> Checked         ;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btSaveMotorClick(TObject *Sender)
{
    UpdateMstOptn (toBuff);

    OM.SaveMstOptn(      );
    OM.SaveCmnOptn(      );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button7Click(TObject *Sender)
{
    int iSel = ((TButton *)Sender) -> Tag ;

    if( iSel == 1) {
        if(!VC.ReadResult(iSel , riWR1)) FM_MsgOk("ERR","Read Failed!");
    }
    if( iSel == 2) {
        if(!VC.ReadResult(iSel , riWR1)) FM_MsgOk("ERR","Read Failed!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button9Click(TObject *Sender)
{
    VC.WriteVisnLot();
    VC.WriteVisnModel();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button13Click(TObject *Sender)
{
    // Ʈ���� ��½ð� ���� : 2usec ~ 4msec

    MT_OneShotAxtTrg(miWRK_XVsn , true , edT1_7->Text.ToIntDef(5000)) ;

    ::Sleep(100);

    MT_ResetAxtTrgPos(miWRK_XVsn) ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button14Click(TObject *Sender)
{

         if(!OM.CmnOptn.bVs1Skip && !OM.CmnOptn.bVs2Skip)VC.SendReset(vrBoth ) ;
    else if( OM.CmnOptn.bVs1Skip && !OM.CmnOptn.bVs2Skip)VC.SendReset(vsVisn2) ;
    else if(!OM.CmnOptn.bVs1Skip &&  OM.CmnOptn.bVs2Skip)VC.SendReset(vsVisn1) ;

    if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
    if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;

    if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].SetStat(csUnkwn) ;
    if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].SetStat(csUnkwn) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Label19Click(TObject *Sender)
{
    VC.WriteLogMap(riWR2);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    double dFisrtPos   = edT1_1->Text.ToDouble() ;
    double dLastPos    = edT1_2->Text.ToDouble() ;
    double dSpace      = edT1_3->Text.ToDouble() ;
    double dVel        = edT1_4->Text.ToDouble() ;
    int    iTrigerTime = edT1_5->Text.ToIntDef(1000);
    int    iTrgCnt     = edT1_6->Text.ToIntDef(0);

//    double dTemp    ;
//    double * dTrgPos = new double[iTrgCnt] ;

//    for(int i = 0 ; i < iTrgCnt ; i++) {
//        dTemp      =  dFisrtPos + dSpace ;
//        dTrgPos[i] = dTemp ;
//    }

//    HED.MoveMotr(miWRK_XVsn,pi)
    MT_GoAbs(miWRK_XVsn,dFisrtPos,dVel);

//    MT_ResetAxtTrgPos(miWRK_XVsn) ;

//    MT_SetAxtTrgPos(miWRK_XVsn , iTrgCnt , dTrgPos , iTrigerTime , true , true) ;

//    delete [] dTrgPos ;

}

//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
    double dFisrtPos   = edT1_1->Text.ToDouble() ;
    double dLastPos    = edT1_2->Text.ToDouble() ;
    double dSpace      = edT1_3->Text.ToDouble() ;
    double dVel        = edT1_4->Text.ToDouble() ;
    int    iTrigerTime = edT1_5->Text.ToIntDef(1000);
    int    iTrgCnt     = edT1_6->Text.ToIntDef(0);

    double dTemp = 0 ;
    double * dTrgPos = new double[iTrgCnt] ;

    dTrgPos[0] = dTemp ;
    for(int i = 1 ; i < iTrgCnt ; i++) {
        dTemp      +=  dFisrtPos + dSpace ;
        dTrgPos[i] = dTemp ;
    }

//    HED.MoveMotr(miWRK_XVsn,pi)

    MT_ResetAxtTrgPos(miWRK_XVsn) ;

    MT_SetAxtTrgPos(miWRK_XVsn , iTrgCnt , dTrgPos , iTrigerTime , true , true) ;

    delete [] dTrgPos ;

    MT_GoAbs(miWRK_XVsn,dLastPos,dVel);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button3Click(TObject *Sender)
{
    double dFisrtPos   = edT2_1->Text.ToDouble() ;
    double dLastPos    = edT2_2->Text.ToDouble() ;
    double dSpace      = edT2_3->Text.ToDouble() ;
    double dVel        = edT2_4->Text.ToDouble() ;
    int    iTrigerTime = edT2_5->Text.ToIntDef(1000);
    int    iTrgCnt     = edT2_6->Text.ToIntDef(0);

//    double dTemp    ;
//    double * dTrgPos = new double[iTrgCnt] ;

//    for(int i = 0 ; i < iTrgCnt ; i++) {
//        dTemp      =  dFisrtPos + dSpace ;
//        dTrgPos[i] = dTemp ;
//    }

//    HED.MoveMotr(miWRK_XVsn,pi)
    MT_GoAbs(miPSB_XMrk,dFisrtPos,dVel);

//    MT_ResetAxtTrgPos(miWRK_XVsn) ;

//    MT_SetAxtTrgPos(miWRK_XVsn , iTrgCnt , dTrgPos , iTrigerTime , true , true) ;

//    delete [] dTrgPos ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button4Click(TObject *Sender)
{
    double dFisrtPos   = edT2_1->Text.ToDouble() ;
    double dLastPos    = edT2_2->Text.ToDouble() ;
    double dSpace      = edT2_3->Text.ToDouble() ;
    double dVel        = edT2_4->Text.ToDouble() ;
    int    iTrigerTime = edT2_5->Text.ToIntDef(1000);
    int    iTrgCnt     = edT2_6->Text.ToIntDef(0);

    double dTemp = 0 ;
    double * dTrgPos = new double[iTrgCnt] ;
    dTrgPos[0] = dTemp ;
    for(int i = 1 ; i < iTrgCnt  ; i++) {

        dTemp     +=  dFisrtPos + dSpace ;
        dTrgPos[i] = dTemp ;
    }

//    HED.MoveMotr(miWRK_XVsn,pi)

    MT_ResetAxtTrgPos(miWRK_XVsn) ;

    MT_SetAxtTrgPos(miWRK_XVsn , iTrgCnt , dTrgPos , iTrigerTime , true , true) ;

    delete [] dTrgPos ;

    MT_GoAbs(miPSB_XMrk,dLastPos,dVel);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{
    MT_OneShotAxtTrg(miPSB_XMrk , true , edT2_7->Text.ToIntDef(5000)) ;
    ::Sleep(100);

    MT_ResetAxtTrgPos(miWRK_XVsn) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button6Click(TObject *Sender)
{
    VC.SendReset(vrBoth ) ;
//    VC.SendReset(vrVisn2,true ) ;
//    VC.SendReset(vrVisn3,true ) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    VC.SendReset(vsVisn4 ) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button17Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Autorun();
    if(cbRAL -> Checked) RAL.Autorun();
    if(cbHED -> Checked) HED.Autorun();
    if(cbPSB -> Checked) PSB.Autorun();
    if(cbULD -> Checked) ULD.Autorun();
    if(cbULM -> Checked) ULM.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button18Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Reset();
    if(cbRAL -> Checked) RAL.Reset();
    if(cbHED -> Checked) HED.Reset();
    if(cbPSB -> Checked) PSB.Reset();
    if(cbULD -> Checked) ULD.Reset();
    if(cbULM -> Checked) ULM.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button19Click(TObject *Sender)
{

    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::Button16Click(TObject *Sender)
{
    DM.ARAY[riPRB].SetStat(csNone);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button20Click(TObject *Sender)
{
    DM.ARAY[riPRB].SetStat(csUnkwn);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button21Click(TObject *Sender)
{
    LDR._iMgzCnt = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    VC.SendLotStart(true);
}
//---------------------------------------------------------------------------



void __fastcall TFrmMaster::tmMaster1Timer(TObject *Sender)
{
    tmMaster1->Enabled = false ;
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

    sgCycleStep -> Cells[1][3] = HED.GetHomeStep   () ;
    sgCycleStep -> Cells[2][3] = HED.GetToStartStep() ;
    sgCycleStep -> Cells[3][3] = HED.GetSeqStep    () ;
    sgCycleStep -> Cells[4][3] = HED.GetCycleStep  () ;
    sgCycleStep -> Cells[5][3] = HED.GetToStopStep () ;

    sgCycleStep -> Cells[1][4] = PSB.GetHomeStep   () ;
    sgCycleStep -> Cells[2][4] = PSB.GetToStartStep() ;
    sgCycleStep -> Cells[3][4] = PSB.GetSeqStep    () ;
    sgCycleStep -> Cells[4][4] = PSB.GetCycleStep  () ;
    sgCycleStep -> Cells[5][4] = PSB.GetToStopStep () ;

    sgCycleStep -> Cells[1][5] = ULD.GetHomeStep   () ;
    sgCycleStep -> Cells[2][5] = ULD.GetToStartStep() ;
    sgCycleStep -> Cells[3][5] = ULD.GetSeqStep    () ;
    sgCycleStep -> Cells[4][5] = ULD.GetCycleStep  () ;
    sgCycleStep -> Cells[5][5] = ULD.GetToStopStep () ;

    sgCycleStep -> Cells[1][6] = ULM.GetHomeStep   () ;
    sgCycleStep -> Cells[2][6] = ULM.GetToStartStep() ;
    sgCycleStep -> Cells[3][6] = ULM.GetSeqStep    () ;
    sgCycleStep -> Cells[4][6] = ULM.GetCycleStep  () ;
    sgCycleStep -> Cells[5][6] = ULM.GetToStopStep () ;

    tmMaster1->Enabled = true ;
}
//---------------------------------------------------------------------------

