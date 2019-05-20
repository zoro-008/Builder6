//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmMotorPos.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Loader.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "SMInterfaceUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMotorPos *FormMotorPos;
int iMrkManCol = -1;
int iMrkManRow = -1;
//---------------------------------------------------------------------------
__fastcall TFormMotorPos::TFormMotorPos(TComponent* Owner)
    : TForm(Owner)
{
    PM.SetWindow (pnLDR_YPos,miLDR_YCmp);
    PM.SetWindow (pnLDR_ZPos,miLDR_ZCmp);
    PM.SetWindow (pnPSB_XPos,miPSB_XMrk);
    PM.SetWindow (pnPSB_YPos,miPSB_YMrk);
    PM.SetWindow (pnWK1_XIns,miWK1_XIns);
    PM.SetWindow (pnWK1_YFlp,miWK1_YFlp);
    PM.SetWindow (pnWRK_XPos,miWRK_XVsn);
    PM.SetWindow (pnWRK_YPos,miWRK_YVsn);
    PM.SetWindow (pnULD_ZPos,miULD_ZCmp);
    PM.SetWindow (pnULD_YPos,miULD_YCmp);
}
//---------------------------------------------------------------------------
bool __fastcall TFormMotorPos::MotorCheck(EN_MOTR_ID _iMotor)
{
    bool bRet1 = true;

    bool bInsFwd = AT_Complete(aiWK1_Ins , ccFwd);

         if(_iMotor == miLDR_YCmp) {}
    else if(_iMotor == miLDR_ZCmp) {}
    else if(_iMotor == miWRK_XVsn) {
        if(!MT_CmprPos(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInversePs)) && !MT_CmprPos(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormalPs ))) {
            FM_MsgOk("Error", "Flipper Position is not Normal Position");
            bRet1 = false ;
        }
    }
    else if(_iMotor == miWRK_YVsn) {
        if(!MT_CmprPos(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInversePs)) && !MT_CmprPos(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormalPs ))) {
            FM_MsgOk("Error", "Flipper Position is not Normal Position");
            bRet1 = false ;
        }
    }
    else if(_iMotor == miPSB_XMrk) {
        if(!AT_Complete(aiPSB_Marking, ccBwd)){ AT_MoveCyl(aiPSB_Marking, ccBwd); bRet1 = false;}
    }
    else if(_iMotor == miPSB_YMrk) {
        if(!AT_Complete(aiPSB_Marking, ccBwd)){ AT_MoveCyl(aiPSB_Marking, ccBwd); bRet1 = false;}
    }
    else if(_iMotor == miWK1_YFlp) {
        if(!MT_CmprPos(miWRK_YVsn, PM.GetValue(miWRK_YVsn, pvWRK_YVsnWaitPs  ))){
            FM_MsgOk("Error", "Vision Y Position is not Wait Position");
            bRet1 = false ;
        }
    }
    else if(_iMotor == miWK1_XIns) {
        if(bInsFwd && !MT_CmprPos(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpInversePs)) && !MT_CmprPos(miWK1_YFlp, PM.GetValue(miWK1_YFlp, pvWK1_YFlpNormalPs))) {
            FM_MsgOk("Error", "Flipper Position is not Normal Position");
            bRet1 = false;
        }
        if(!MT_CmprPos(miWRK_YVsn, PM.GetValue(miWRK_YVsn, pvWRK_YVsnWaitPs))){
            FM_MsgOk("Error", "Vision Y Position is not Wait Position");
            bRet1 = false;
        }
    }
    else if(_iMotor == miULD_YCmp) {}
    else if(_iMotor == miULD_ZCmp) {}

    return bRet1 ; // FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes  return ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::UpDownChanging(TObject *Sender,
      bool &AllowChange)
{
    double Temp, Temp2;
    double Inc  = (double)(UpDown->Increment*10)/10000;
    int    iSel;

    iSel = ((TUpDown *)Sender) -> Tag;

        if(iSel == 0) {Temp = edMTPosJog0 -> Text.ToDouble();
             Temp += Inc;
             edMTPosJog0->Text = Temp;
         }
    else if(iSel == 1) {Temp = edMTPosJog1 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog1->Text = Temp;
    }
    else if(iSel == 2) {Temp = edMTPosJog2 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog2->Text = Temp;
    }
    else if(iSel == 3) {Temp = edMTPosJog3 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog3->Text = Temp;
    }
    else if(iSel == 4) {Temp = edMTPosJog4 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog4->Text = Temp;
    }
    else if(iSel == 5) {Temp = edMTPosJog5 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog5->Text = Temp;
    }
    else if(iSel == 6) {Temp = edMTPosJog6 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog6->Text = Temp;
    }
    else if(iSel == 7) {Temp = edMTPosJog7 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog7->Text = Temp;
    }
    else if(iSel == 8) {Temp = edMTPosJog8 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog8->Text = Temp;
    }
    else if(iSel == 9) {Temp = edMTPosJog9 -> Text.ToDouble();
        Temp += Inc;
        edMTPosJog9->Text = Temp;
    }
}

//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btSaveDeviceClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    //강제 세팅. 비젼 Y축 무조건 0.0으로.
    PM.SetPos(miWRK_YVsn , pvWRK_YVsnWaitPs , 0.0 );

    PM.UpdatePstn(toBuff);

    PM.Save(OM.m_sCrntDev);

    DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWR1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWR3].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS3].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);

}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::FormShow(TObject *Sender)
{
    PM.UpdatePstn (toTabl);
    PageControl->ActivePageIndex = 0;
    tmUpdateTimer->Enabled = true ;
}
//-------------------------------------------------------------------------

void __fastcall TFormMotorPos::BtLDR_CmpUpClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLDR_Cmp      )
    {
        if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes ) return ;
        bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    }
    if(iSel == aiLDR_Pusher   ) bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPRB_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK1_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK1_Align    ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK1_FlpCmp   ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK1_Ins      ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK2_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK2_Align    ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK2_Lift     ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK3_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK3_Align    ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWK3_Lift     ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Stopper  ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Align    ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Lift     ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Marking  ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Pusher   ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiULD_Cmp      )
    {
        if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes ) return ;
        bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    }
    if(!bRet) return ;

    AT_MoveCyl(iSel , ccFwd) ;

}
//---------------------------------------------------------------------------




void __fastcall TFormMotorPos::BtMT_VisnXStopClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    MT_Stop(iSel) ;
}
//---------------------------------------------------------------------------



void __fastcall TFormMotorPos::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    double MaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 2);
    double MaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 2);

//    btSaveDevice -> Enabled = !SEQ._bRun && !FrmMain -> tmMainSet -> Enabled && !FrmMain -> tmSubSet -> Enabled ;

    switch ( PageControl -> ActivePageIndex )
    {
        case 0 : lbCmdPos_XVsn    -> Caption = MT_GetCmdPos   (miWRK_XVsn) ;
                 lbActPos_XVsn    -> Caption = MT_GetEncPos   (miWRK_XVsn) ;
                 lbNLim_XVsn      -> Color   = MT_GetNLimSnsr (miWRK_XVsn) ? clLime : clGray ;
                 lbHome_XVsn      -> Color   = MT_GetHomeSnsr (miWRK_XVsn) ? clLime : clGray ;
                 lbPLim_XVsn      -> Color   = MT_GetPLimSnsr (miWRK_XVsn) ? clLime : clGray ;
                 lbAlar_XVsn      -> Color   = MT_GetAlarm    (miWRK_XVsn) ? clLime : clGray ;
                 lbServ_XVsn      -> Color   = MT_GetServo    (miWRK_XVsn) ? clLime : clGray ;
                 lbStop_XVsn      -> Color   = MT_GetStopInpos(miWRK_XVsn) ? clLime : clGray ;
                 lbHDon_XVsn      -> Color   = MT_GetHomeEnd  (miWRK_XVsn) ? clLime : clGray ;

                 lbCmdPos_YVsn    -> Caption = MT_GetCmdPos   (miWRK_YVsn) ;
                 lbActPos_YVsn    -> Caption = MT_GetEncPos   (miWRK_YVsn) ;
                 lbNLim_YVsn      -> Color   = MT_GetNLimSnsr (miWRK_YVsn) ? clLime : clGray ;
                 lbHome_YVsn      -> Color   = MT_GetHomeSnsr (miWRK_YVsn) ? clLime : clGray ;
                 lbPLim_YVsn      -> Color   = MT_GetPLimSnsr (miWRK_YVsn) ? clLime : clGray ;
                 lbAlar_YVsn      -> Color   = MT_GetAlarm    (miWRK_YVsn) ? clLime : clGray ;
                 lbServ_YVsn      -> Color   = MT_GetServo    (miWRK_YVsn) ? clLime : clGray ;
                 lbStop_YVsn      -> Color   = MT_GetStopInpos(miWRK_YVsn) ? clLime : clGray ;
                 lbHDon_YVsn      -> Color   = MT_GetHomeEnd  (miWRK_YVsn) ? clLime : clGray ;

                 pnWR1_StpUp      -> Color   = AT_Complete(aiWK1_Stopper, ccFwd) ? clLime : clGray ;
                 pnWR1_StpDn      -> Color   = AT_Complete(aiWK1_Stopper, ccBwd) ? clLime : clGray ;
                 pnWR2_StprUp     -> Color   = AT_Complete(aiWK2_Stopper, ccFwd) ? clLime : clGray ;
                 pnWR2_StprDn     -> Color   = AT_Complete(aiWK2_Stopper, ccBwd) ? clLime : clGray ;
                 pnWR3_StprUp     -> Color   = AT_Complete(aiWK3_Stopper, ccFwd) ? clLime : clGray ;
                 pnWR3_StprDn     -> Color   = AT_Complete(aiWK3_Stopper, ccBwd) ? clLime : clGray ;
                 pnWR2_FeedingOn  -> Color   = IO_GetY(yWR1_FeedingAC )          ? clLime : clGray ;
                 pnWR2_FeedingOff -> Color   = IO_GetY(yWR1_FeedingAC )          ? clGray : clLime ;
                 pnWR2_LiftUp     -> Color   = AT_Complete(aiWK2_Lift   , ccFwd) ? clLime : clGray ;
                 pnWR2_LiftDn     -> Color   = AT_Complete(aiWK2_Lift   , ccBwd) ? clLime : clGray ;
                 pnWR3_LiftUp     -> Color   = AT_Complete(aiWK3_Lift   , ccFwd) ? clLime : clGray ;
                 pnWR3_LiftDn     -> Color   = AT_Complete(aiWK3_Lift   , ccBwd) ? clLime : clGray ;
        break;

        case 1 : lbCmdPos_XMrk    -> Caption = MT_GetCmdPos   (miPSB_XMrk) ;
                 lbActPos_XMrk    -> Caption = MT_GetEncPos   (miPSB_XMrk) ;
                 lbNLim_XMrk      -> Color   = MT_GetNLimSnsr (miPSB_XMrk) ? clLime : clGray ;
                 lbHome_XMrk      -> Color   = MT_GetHomeSnsr (miPSB_XMrk) ? clLime : clGray ;
                 lbPLim_XMrk      -> Color   = MT_GetPLimSnsr (miPSB_XMrk) ? clLime : clGray ;
                 lbAlar_XMrk      -> Color   = MT_GetAlarm    (miPSB_XMrk) ? clLime : clGray ;
                 lbServ_XMrk      -> Color   = MT_GetServo    (miPSB_XMrk) ? clLime : clGray ;
                 lbStop_XMrk      -> Color   = MT_GetStopInpos(miPSB_XMrk) ? clLime : clGray ;
                 lbHDon_XMrk      -> Color   = MT_GetHomeEnd  (miPSB_XMrk) ? clLime : clGray ;

                 lbCmdPos_YMrk    -> Caption = MT_GetCmdPos   (miPSB_YMrk) ;
                 lbActPos_YMrk    -> Caption = MT_GetEncPos   (miPSB_YMrk) ;
                 lbNLim_YMrk      -> Color   = MT_GetNLimSnsr (miPSB_YMrk) ? clLime : clGray ;
                 lbHome_YMrk      -> Color   = MT_GetHomeSnsr (miPSB_YMrk) ? clLime : clGray ;
                 lbPLim_YMrk      -> Color   = MT_GetPLimSnsr (miPSB_YMrk) ? clLime : clGray ;
                 lbAlar_YMrk      -> Color   = MT_GetAlarm    (miPSB_YMrk) ? clLime : clGray ;
                 lbServ_YMrk      -> Color   = MT_GetServo    (miPSB_YMrk) ? clLime : clGray ;
                 lbStop_YMrk      -> Color   = MT_GetStopInpos(miPSB_YMrk) ? clLime : clGray ;
                 lbHDon_YMrk      -> Color   = MT_GetHomeEnd  (miPSB_YMrk) ? clLime : clGray ;

                 pnPSB_LiftUp     -> Color   = AT_Complete(aiPSB_Lift   , ccFwd) ? clLime : clGray ;
                 pnPSB_LiftDn     -> Color   = AT_Complete(aiPSB_Lift   , ccBwd) ? clLime : clGray ;
                 pnPSB_StprUp     -> Color   = AT_Complete(aiPSB_Stopper, ccFwd) ? clLime : clGray ;
                 pnPSB_StprDn     -> Color   = AT_Complete(aiPSB_Stopper, ccBwd) ? clLime : clGray ;
                 pnPSB_CmpUp      -> Color   = AT_Complete(aiPSB_Marking, ccBwd) ? clLime : clGray ;
                 pnPSB_CmpDn      -> Color   = AT_Complete(aiPSB_Marking, ccFwd) ? clLime : clGray ;
                 pnPSB_FeedingOn  -> Color   = IO_GetY(yWR2_FeedingAC )          ? clLime : clGray ;
                 pnPSB_FeedingOff -> Color   = IO_GetY(yWR2_FeedingAC )          ? clGray : clLime ;
                 pnPSB_MrkChange  -> Color   = MT_CmprPos(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWorkStartPs)) ?  clLime : clGray ;

        break;

        case 2 : lbCmdPos_XIns    -> Caption = MT_GetCmdPos   (miWK1_XIns) ;
                 lbActPos_XIns    -> Caption = MT_GetEncPos   (miWK1_XIns) ;
                 lbNLim_XIns      -> Color   = MT_GetNLimSnsr (miWK1_XIns) ? clLime : clGray ;
                 lbHome_XIns      -> Color   = MT_GetHomeSnsr (miWK1_XIns) ? clLime : clGray ;
                 lbPLim_XIns      -> Color   = MT_GetPLimSnsr (miWK1_XIns) ? clLime : clGray ;
                 lbAlar_XIns      -> Color   = MT_GetAlarm    (miWK1_XIns) ? clLime : clGray ;
                 lbServ_XIns      -> Color   = MT_GetServo    (miWK1_XIns) ? clLime : clGray ;
                 lbStop_XIns      -> Color   = MT_GetStopInpos(miWK1_XIns) ? clLime : clGray ;
                 lbHDon_XIns      -> Color   = MT_GetHomeEnd  (miWK1_XIns) ? clLime : clGray ;

                 lbCmdPos_YFlp    -> Caption = MT_GetCmdPos   (miWK1_YFlp) ;
                 lbActPos_YFlp    -> Caption = MT_GetEncPos   (miWK1_YFlp) ;
                 lbNLim_YFlp      -> Color   = MT_GetNLimSnsr (miWK1_YFlp) ? clLime : clGray ;
                 lbHome_YFlp      -> Color   = MT_GetHomeSnsr (miWK1_YFlp) ? clLime : clGray ;
                 lbPLim_YFlp      -> Color   = MT_GetPLimSnsr (miWK1_YFlp) ? clLime : clGray ;
                 lbAlar_YFlp      -> Color   = MT_GetAlarm    (miWK1_YFlp) ? clLime : clGray ;
                 lbServ_YFlp      -> Color   = MT_GetServo    (miWK1_YFlp) ? clLime : clGray ;
                 lbStop_YFlp      -> Color   = MT_GetStopInpos(miWK1_YFlp) ? clLime : clGray ;
                 lbHDon_YFlp      -> Color   = MT_GetHomeEnd  (miWK1_YFlp) ? clLime : clGray ;

                 pnPRB_TrnUp      -> Color   = AT_Complete(aiWK1_Ins, ccBwd)     ? clLime : clGray ;
                 pnPRB_TrnDn      -> Color   = AT_Complete(aiWK1_Ins, ccFwd)     ? clLime : clGray ;
                 pnPRB_FlpCmpUp   -> Color   = AT_Complete(aiWK1_FlpCmp, ccFwd)  ? clLime : clGray ;
                 pnPRB_FlpCmpDn   -> Color   = AT_Complete(aiWK1_FlpCmp, ccBwd)  ? clLime : clGray ;
                 pnPRB_FeedingOn  -> Color   = IO_GetY(yPRB_FeedingAC ) ? clLime : clGray ;
                 pnPRB_FeedingOff -> Color   = IO_GetY(yPRB_FeedingAC ) ? clGray : clLime ;
                 pnPRB_StprUp     -> Color   = AT_Complete(aiPRB_Stopper, ccFwd) ? clLime : clGray ;
                 pnPRB_StprDn     -> Color   = AT_Complete(aiPRB_Stopper, ccBwd) ? clLime : clGray ;
        break;

        case 3 : lbCmdPos_ZLdr    -> Caption = MT_GetCmdPos   (miLDR_ZCmp) ;
                 lbActPos_ZLdr    -> Caption = MT_GetEncPos   (miLDR_ZCmp) ;
                 lbNLim_ZLdr      -> Color   = MT_GetNLimSnsr (miLDR_ZCmp) ? clLime : clGray ;
                 lbHome_ZLdr      -> Color   = MT_GetHomeSnsr (miLDR_ZCmp) ? clLime : clGray ;
                 lbPLim_ZLdr      -> Color   = MT_GetPLimSnsr (miLDR_ZCmp) ? clLime : clGray ;
                 lbAlar_ZLdr      -> Color   = MT_GetAlarm    (miLDR_ZCmp) ? clLime : clGray ;
                 lbServ_ZLdr      -> Color   = MT_GetServo    (miLDR_ZCmp) ? clLime : clGray ;
                 lbStop_ZLdr      -> Color   = MT_GetStopInpos(miLDR_ZCmp) ? clLime : clGray ;
                 lbHDon_ZLdr      -> Color   = MT_GetHomeEnd  (miLDR_ZCmp) ? clLime : clGray ;

                 lbCmdPos_YLdr    -> Caption = MT_GetCmdPos   (miLDR_YCmp) ;
                 lbActPos_YLdr    -> Caption = MT_GetEncPos   (miLDR_YCmp) ;
                 lbNLim_YLdr      -> Color   = MT_GetNLimSnsr (miLDR_YCmp) ? clLime : clGray ;
                 lbHome_YLdr      -> Color   = MT_GetHomeSnsr (miLDR_YCmp) ? clLime : clGray ;
                 lbPLim_YLdr      -> Color   = MT_GetPLimSnsr (miLDR_YCmp) ? clLime : clGray ;
                 lbAlar_YLdr      -> Color   = MT_GetAlarm    (miLDR_YCmp) ? clLime : clGray ;
                 lbServ_YLdr      -> Color   = MT_GetServo    (miLDR_YCmp) ? clLime : clGray ;
                 lbStop_YLdr      -> Color   = MT_GetStopInpos(miLDR_YCmp) ? clLime : clGray ;
                 lbHDon_YLdr      -> Color   = MT_GetHomeEnd  (miLDR_YCmp) ? clLime : clGray ;

                 pnLDR_CmpUp      -> Color   = AT_Complete(aiLDR_Cmp   , ccFwd) ? clLime : clGray ;
                 pnLDR_CmpDn      -> Color   = AT_Complete(aiLDR_Cmp   , ccBwd) ? clLime : clGray ;
                 pnLDR_PshFw      -> Color   = AT_Complete(aiLDR_Pusher, ccFwd) ? clLime : clGray ;
                 pnLDR_PshBw      -> Color   = AT_Complete(aiLDR_Pusher, ccBwd) ? clLime : clGray ;
        break;

        case 4 : lbCmdPos_ZUld    -> Caption = MT_GetCmdPos   (miULD_ZCmp) ;
                 lbActPos_ZUld    -> Caption = MT_GetEncPos   (miULD_ZCmp) ;
                 lbNLim_ZUld      -> Color   = MT_GetNLimSnsr (miULD_ZCmp) ? clLime : clGray ;
                 lbHome_ZUld      -> Color   = MT_GetHomeSnsr (miULD_ZCmp) ? clLime : clGray ;
                 lbPLim_ZUld      -> Color   = MT_GetPLimSnsr (miULD_ZCmp) ? clLime : clGray ;
                 lbAlar_ZUld      -> Color   = MT_GetAlarm    (miULD_ZCmp) ? clLime : clGray ;
                 lbServ_ZUld      -> Color   = MT_GetServo    (miULD_ZCmp) ? clLime : clGray ;
                 lbStop_ZUld      -> Color   = MT_GetStopInpos(miULD_ZCmp) ? clLime : clGray ;
                 lbHDon_ZUld      -> Color   = MT_GetHomeEnd  (miULD_ZCmp) ? clLime : clGray ;

                 lbCmdPos_YUld    -> Caption = MT_GetCmdPos   (miULD_YCmp) ;
                 lbActPos_YUld    -> Caption = MT_GetEncPos   (miULD_YCmp) ;
                 lbNLim_YUld      -> Color   = MT_GetNLimSnsr (miULD_YCmp) ? clLime : clGray ;
                 lbHome_YUld      -> Color   = MT_GetHomeSnsr (miULD_YCmp) ? clLime : clGray ;
                 lbPLim_YUld      -> Color   = MT_GetPLimSnsr (miULD_YCmp) ? clLime : clGray ;
                 lbAlar_YUld      -> Color   = MT_GetAlarm    (miULD_YCmp) ? clLime : clGray ;
                 lbServ_YUld      -> Color   = MT_GetServo    (miULD_YCmp) ? clLime : clGray ;
                 lbStop_YUld      -> Color   = MT_GetStopInpos(miULD_YCmp) ? clLime : clGray ;
                 lbHDon_YUld      -> Color   = MT_GetHomeEnd  (miULD_YCmp) ? clLime : clGray ;

                 pnULD_CmpUp      -> Color   = AT_Complete(aiULD_Cmp   , ccFwd) ? clLime : clGray ;
                 pnULD_CmpDn      -> Color   = AT_Complete(aiULD_Cmp   , ccBwd) ? clLime : clGray ;
                 pnULD_PshFw      -> Color   = AT_Complete(aiPSB_Pusher, ccFwd) ? clLime : clGray ;
                 pnULD_PshBw      -> Color   = AT_Complete(aiPSB_Pusher, ccBwd) ? clLime : clGray ;
        break;
    }

    tmUpdateTimer->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdateTimer ->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::FormDestroy(TObject *Sender)
{
    tmUpdateTimer -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btWk1_FeedingOnClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iSel , true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btWk1_FeedingOffClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iSel , false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btMrkLeftUpPsClick(TObject *Sender)
{
    if(MotorCheck(miPSB_XMrk) && MotorCheck(miPSB_YMrk) )
    {
        MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWorkStartPs));
        MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWorkStartPs));
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btMrkRightUpPsClick(TObject *Sender)
{
    double MaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 2);
    double MaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 2);

    //AT_MoveCyl(aiPSB_Marking, ccBwd);
    if(MotorCheck(miPSB_XMrk) && MotorCheck(miPSB_YMrk)){
        MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWorkStartPs) - MaxColPich);
        MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWorkStartPs));
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btMrkLeftDnPsClick(TObject *Sender)
{
    double MaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 2);
    double MaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 2);

    //AT_MoveCyl(aiPSB_Marking, ccBwd);
    if(MotorCheck(miPSB_XMrk) && MotorCheck(miPSB_YMrk)){
        MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWorkStartPs));
        MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWorkStartPs) + MaxRowPich);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btMrkRightDnPsClick(TObject *Sender)
{
    double MaxColPich = OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 2);
    double MaxRowPich = OM.DevInfo.dRowPitch * (OM.DevInfo.iRowCnt - 2);

    //AT_MoveCyl(aiPSB_Marking, ccBwd);
    if(MotorCheck(miPSB_XMrk) && MotorCheck(miPSB_YMrk)){
        MT_GoAbsMan(miPSB_XMrk, PM.GetValue(miPSB_XMrk, pvPSB_XMrkWorkStartPs) - MaxColPich);
        MT_GoAbsMan(miPSB_YMrk, PM.GetValue(miPSB_YMrk, pvPSB_YMrkWorkStartPs) + MaxRowPich);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::BtVisn2_LiftUpClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_MrkYnClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    MotorCheck(miPSB_XMrk);
//    AT_MoveCyl(aiPSB_Marking, ccBwd);
    //while (!AT_MoveCyl(aiPSB_Marking, ccBwd)) {
    //    Sleep(1);
    //}
    iMrkManRow -- ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = OM.DevInfo.iColCnt - 1;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = OM.DevInfo.iRowCnt - 1;
    MT_GoAbsMan(miPSB_XMrk , PSB.GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , PSB.GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_MrkXpClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking, ccBwd);
    //while (!AT_MoveCyl(aiPSB_Marking, ccBwd)) {
    //    Sleep(1);
    //}
    iMrkManCol -- ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = OM.DevInfo.iColCnt - 1;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = OM.DevInfo.iRowCnt - 1;
    MT_GoAbsMan(miPSB_XMrk , PSB.GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , PSB.GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_MrkXnClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking, ccBwd);
    //while (!AT_MoveCyl(aiPSB_Marking, ccBwd)) {
    //    Sleep(1);
    //}
    iMrkManCol ++ ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = OM.DevInfo.iColCnt - 1;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = OM.DevInfo.iRowCnt - 1;
    MT_GoAbsMan(miPSB_XMrk , PSB.GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , PSB.GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_MrkYpClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking, ccBwd);
    //while (!AT_MoveCyl(aiPSB_Marking, ccBwd)) {
    //    Sleep(1);
    //}
    iMrkManRow ++ ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = OM.DevInfo.iColCnt - 1;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = OM.DevInfo.iRowCnt - 1;
    MT_GoAbsMan(miPSB_XMrk , PSB.GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , PSB.GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;

}
//---------------------------------------------------------------------------


void __fastcall TFormMotorPos::BtLDR_CmpDnClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLDR_Cmp      )
    {
        if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes ) return ;
        bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    }
    if(iSel == aiLDR_Pusher   ) bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPRB_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK1_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK1_Align    ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK1_FlpCmp   ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK1_Ins      ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK2_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK2_Align    ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK2_Lift     ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK3_Stopper  ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK3_Align    ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWK3_Lift     ) bRet = RAL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Stopper  ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Align    ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Lift     ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Marking  ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Pusher   ) bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiULD_Cmp      )
    {
        if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes ) return ;
        bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    }
    if(!bRet) return ;

    AT_MoveCyl(iSel , ccBwd) ;
}
//---------------------------------------------------------------------------




void __fastcall TFormMotorPos::btBwdMoveClick(TObject *Sender)
{
    int    iSel      = ((TBitBtn *)Sender) -> Tag ;
    double dUnit     = 0.0 ;
    bool   bRet1     = false ;
    bRet1 = MotorCheck(iSel);

         if(iSel == 0) {dUnit = edMTPosJog0 -> Text.ToDouble();}
    else if(iSel == 1) {dUnit = edMTPosJog1 -> Text.ToDouble();}
    else if(iSel == 2) {dUnit = edMTPosJog2 -> Text.ToDouble();}
    else if(iSel == 3) {dUnit = edMTPosJog3 -> Text.ToDouble();}
    else if(iSel == 4) {dUnit = edMTPosJog4 -> Text.ToDouble();}
    else if(iSel == 5) {dUnit = edMTPosJog5 -> Text.ToDouble();}
    else if(iSel == 6) {dUnit = edMTPosJog6 -> Text.ToDouble();}
    else if(iSel == 7) {dUnit = edMTPosJog7 -> Text.ToDouble();}
    else if(iSel == 8) {dUnit = edMTPosJog8 -> Text.ToDouble();}
    else if(iSel == 9) {dUnit = edMTPosJog9 -> Text.ToDouble();}

    if(bRet1) {MT_GoIncMan(iSel, -dUnit);}
    else return ;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btFwdMoveClick(TObject *Sender)
{
//    TBitBtn * bt =

    int    iSel      = ((TBitBtn *)Sender) -> Tag ;
    double dUnit     = 0.0 ;
    bool bRet1 = false ;
    bRet1 = MotorCheck(iSel);

         if(iSel == 0) {dUnit = edMTPosJog0 -> Text.ToDouble();}
    else if(iSel == 1) {dUnit = edMTPosJog1 -> Text.ToDouble();}
    else if(iSel == 2) {dUnit = edMTPosJog2 -> Text.ToDouble();}
    else if(iSel == 3) {dUnit = edMTPosJog3 -> Text.ToDouble();}
    else if(iSel == 4) {dUnit = edMTPosJog4 -> Text.ToDouble();}
    else if(iSel == 5) {dUnit = edMTPosJog5 -> Text.ToDouble();}
    else if(iSel == 6) {dUnit = edMTPosJog6 -> Text.ToDouble();}
    else if(iSel == 7) {dUnit = edMTPosJog7 -> Text.ToDouble();}
    else if(iSel == 8) {dUnit = edMTPosJog8 -> Text.ToDouble();}
    else if(iSel == 9) {dUnit = edMTPosJog9 -> Text.ToDouble();}

    if (bRet1) {MT_GoIncMan(iSel , dUnit) ;}
    else return;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btBwdJogMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int  iSel  = ((TBitBtn *)Sender) -> Tag ;
    bool bRet1 = false ;
    bRet1 = MotorCheck(iSel);

    if (bRet1) {MT_JogN(iSel);}
    else return ;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btBwdJogMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int  iSel  = ((TBitBtn *)Sender) -> Tag ;

    MT_Stop(iSel) ;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btFwdJogMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int  iSel  = ((TBitBtn *)Sender) -> Tag ;
    int  bRet1 = false ;
    bRet1 = MotorCheck(iSel);

    if (bRet1) {MT_JogP(iSel);}
    else return;
}
//---------------------------------------------------------------------------






