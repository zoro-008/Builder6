//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "VisnComUnit.h"
#include "EqpComUnit.h"
#include "DataMan.h"
#include "LogUnit.h"
#include "SMInterfaceUnit.h"

#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#include "PstBuff.h"
#include "UnLoader.h"

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
    UpdateEqpOptn (toTabl);
    tmMaster -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::FormHide(TObject *Sender)
{
    tmMaster -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::UpdateMstOptn(bool bTable)
{
    if(bTable){
        edPreEquipPath    -> Text    = OM.MstOptn.sPreEquipPath   ;
        edVisnPath        -> Text    = OM.MstOptn.sVisnPath       ;
        cbDebugMode       -> Checked = OM.MstOptn.bDebugMode      ;
        edPstEquipDrv     -> Text    = OM.MstOptn.sPstEquipDrv    ;
        edPstEquipIp      -> Text    = OM.MstOptn.sPstEquipIp     ;
        edPstEquipPath    -> Text    = OM.MstOptn.sPstEquipPath   ;
        cbBarcodeOnly     -> Checked = OM.MstOptn.bBarcodeOnly    ;
        cbUld_YHome       -> Checked = OM.MstOptn.bUld_YHome      ;
        cbPSB_XYTrmHome   -> Checked = OM.MstOptn.bPSB_XYTrmHome  ;
        cbLdr_YHome       -> Checked = OM.MstOptn.bLdr_YHome      ;
        cbEmptyNoTrim     -> Checked = OM.MstOptn.bEmptyNoTrim    ;
        cbUseHangul       -> Checked = OM.MstOptn.bUseHangul      ;
    }
    else{
        OM.MstOptn.sPreEquipPath     = edPreEquipPath    -> Text    ;
        OM.MstOptn.sVisnPath         = edVisnPath        -> Text    ;
        OM.MstOptn.bDebugMode        = cbDebugMode       -> Checked ;
        OM.MstOptn.sPstEquipDrv      = edPstEquipDrv     -> Text    ;
        OM.MstOptn.sPstEquipIp       = edPstEquipIp      -> Text    ;
        OM.MstOptn.sPstEquipPath     = edPstEquipPath    -> Text    ;
        OM.MstOptn.bBarcodeOnly      = cbBarcodeOnly     -> Checked ;
        OM.MstOptn.bUld_YHome        = cbUld_YHome       -> Checked ;
        OM.MstOptn.bPSB_XYTrmHome    = cbPSB_XYTrmHome   -> Checked ;
        OM.MstOptn.bLdr_YHome        = cbLdr_YHome       -> Checked ;
        OM.MstOptn.bEmptyNoTrim      = cbEmptyNoTrim     -> Checked ;
        OM.MstOptn.bUseHangul        = cbUseHangul       -> Checked ;

    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
        cbExistLoader   -> Checked = OM.EqpOptn.bExistLoader   ;
        cbExistUnLoader -> Checked = OM.EqpOptn.bExistUnLoader ;
    }
    else{
        OM.EqpOptn.bExistLoader    = cbExistLoader   -> Checked ;
        OM.EqpOptn.bExistUnLoader  = cbExistUnLoader -> Checked ;
    }

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

void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    if(!EC.NetDrvConnect(OM.MstOptn.sPstEquipDrv, OM.MstOptn.sPstEquipIp, OM.MstOptn.sPstEquipPath)) FM_MsgOk("ERROR","PreEqipment Net Connect Failed!");
//    COM.NetDrvConnect(OM.MstOptn.sVisnDrv    , OM.MstOptn.sVisnIp    , OM.MstOptn.sVisnPath    );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
//    COM.NetDrvConnect(OM.MstOptn.sPreEquipDrv, OM.MstOptn.sPstEquipIp, OM.MstOptn.sPreEquipPath);
    //if(!VC.NetDrvConnect(OM.MstOptn.sVisnDrv    , OM.MstOptn.sVisnIp    , OM.MstOptn.sVisnPath    )) FM_MsgOk("ERROR","Vision Net Connect Failed!");
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button3Click(TObject *Sender)
{
    EC.WriteLotData();
    EC.WriteChange();
    EC.WriteArayData(riPSB);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button4Click(TObject *Sender)
{
    EC.ReadChange();
    EC.ReadLotData();
    EC.ReadArayData(riPRB); //여기다가 담아 줌요... Get
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{
//    LOG.HanraVs();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button6Click(TObject *Sender)
{
//    LOG.DmLog(riPSB);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button7Click(TObject *Sender)
{
    int iSel = ((TButton *)Sender) -> Tag ;

    if( iSel == 1) {
        if(!VC.ReadResult(iSel , riWK1)) FM_MsgOk("ERR","Read Failed!");
    }
    if( iSel == 2) {
        if(!VC.ReadResult(iSel , riWK2)) FM_MsgOk("ERR","Read Failed!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button9Click(TObject *Sender)
{
    VC.WriteVisnLot();
    VC.WriteVisnModel();        
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::tmMasterTimer(TObject *Sender)
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




}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Autorun();
    if(cbRAL -> Checked) RAL.Autorun();
    if(cbHED -> Checked) HED.Autorun();
    if(cbPSB -> Checked) PSB.Autorun();
    if(cbULD -> Checked) ULD.Autorun();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
    if(cbLDR -> Checked) LDR.Reset();
    if(cbRAL -> Checked) RAL.Reset();
    if(cbHED -> Checked) HED.Reset();
    if(cbPSB -> Checked) PSB.Reset();
    if(cbULD -> Checked) ULD.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button13Click(TObject *Sender)
{
   	// axis : 축번호 , trig_time : 트리거 출력 유지시간
	// 트리거 출력시간 범위 : 2usec ~ 4msec

        MT_OneShotAxtTrg(miWRK_XVsn , true , 5000) ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button14Click(TObject *Sender)
{


         if(!OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrBoth ) ;
    else if( OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn2) ;
    else if(!OM.CmnOptn.bVisn1Skip &&  OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn1) ;

    if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
    if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;

    if(DM.ARAY[riWK1].GetCntExist())DM.ARAY[riWK1].SetStat(csUnkwn) ;
    if(DM.ARAY[riWK2].GetCntExist())DM.ARAY[riWK2].SetStat(csUnkwn) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Label19Click(TObject *Sender)
{
    VC.WriteLogMap(riWK2);
}
//---------------------------------------------------------------------------

