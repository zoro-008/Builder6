//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormProbe.h"
#include "FormMain.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "ProbeComUnit.h"
#include "FormMain.h"
#include "Sequence.h"
#include "SortingTool.h"
#include "LanguageUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)                                                                                                                                                             
{

    PM.SetWindow(pnRl1X     , miPRB_XCmp);
    PM.SetWindow(pnRl2X     , miWRK_XCmp);
    PM.SetWindow(pnRl3X     , miPSB_XCmp);
    PM.SetWindow(pnPbT      , miWRK_ZTop);
    PM.SetWindow(pnPbB      , miWRK_ZBtm);
    PM.SetWindow(pnLDRY     , miLDR_YCmp);
    PM.SetWindow(pnLDRZ     , miLDR_ZCmp);
    PM.SetWindow(pnSTLYT    , miSTL_YTop);
    PM.SetWindow(pnREJXB    , miREJ_XBtm);
    PM.SetWindow(pnSTLYB    , miSTL_YBtm);
    PM.SetWindow(pnULDY     , miULD_YCmp);
    PM.SetWindow(pnULDZ     , miULD_ZCmp);
    PM.SetWindow(pnWRK_YWrk , miWRK_YBtm);

//    PM.SetWindow(pnLd2Z,miLD2_Z);

    pcDeviceSet -> ActivePageIndex = 0 ;
    btLDR_CmpUp     -> Tag = (int)aiLDR_Cmp     ;
    btLDR_CmpDn     -> Tag = (int)aiLDR_Cmp     ;

    btLDR_PusherFW  -> Tag = (int)aiLDR_Psh     ;
    btLDR_PusherBW  -> Tag = (int)aiLDR_Psh     ;
    btLDR_In        -> Tag = (int)yLDR_MgzInAC  ;
    btLDR_Out       -> Tag = (int)yLDR_MgzOutAC ;

    btLdr_ZUp       -> Tag = (int)miLDR_ZCmp    ;
    btLdr_ZDn       -> Tag = (int)miLDR_ZCmp    ;
    btLdr_YBw       -> Tag = (int)miLDR_YCmp    ;
    btLdr_YFw       -> Tag = (int)miLDR_YCmp    ;

    btUp_RlClamp1   -> Tag = (int)aiPRB_Cmp     ;
    btDn_RlClamp1   -> Tag = (int)aiPRB_Cmp     ;
    //btUp_IdCheck    -> Tag = (int)aiPRB_IdChk   ;
    //btUp_IdCheck    -> Tag = (int)aiPRB_IdChk   ;
    btUp_RlClamp2   -> Tag = (int)aiWRK_Cmp     ;
    btUp_RlClamp2   -> Tag = (int)aiWRK_Cmp     ;
    btUp_RlClamp3   -> Tag = (int)aiPSB_Cmp     ;
    btUp_RlClamp3   -> Tag = (int)aiPSB_Cmp     ;

    btPRB_In        -> Tag = (int)yPRB_FeedingAC;

    btPrb_XBw       -> Tag = (int)miPRB_XCmp    ;
    btPrb_XFw       -> Tag = (int)miPRB_XCmp    ;
    btWrk_XBw       -> Tag = (int)miWRK_XCmp    ;
    btWrk_XFw       -> Tag = (int)miWRK_XCmp    ;
    btPsb_XBw       -> Tag = (int)miPSB_XCmp    ;
    btPsb_XFw       -> Tag = (int)miPSB_XCmp    ;

    btWrk_TUp       -> Tag = (int)miWRK_ZTop    ;
    btWrk_TDn       -> Tag = (int)miWRK_ZTop    ;
    btWrk_BUp       -> Tag = (int)miWRK_ZBtm    ;
    btWrk_BDn       -> Tag = (int)miWRK_ZBtm    ;
    btWrk_BYUp      -> Tag = (int)miWRK_YBtm    ;
    btWrk_BYDn      -> Tag = (int)miWRK_YBtm    ;

//
    btWRK_1ClampUp  -> Tag = (int)aiPRB_Cmp     ;
    btWRK_1ClampDn  -> Tag = (int)aiPRB_Cmp     ;
    btWRK_2ClampUp  -> Tag = (int)aiWRK_Cmp     ;
    btWRK_2ClampDn  -> Tag = (int)aiWRK_Cmp     ;
    btWRK_3ClampUp  -> Tag = (int)aiPSB_Cmp     ;
    btWRK_3ClampDn  -> Tag = (int)aiPSB_Cmp     ;
    btPSB_Out2      -> Tag = (int)yPSB_FeedingAC;
//
    btPSB_PusherFw  -> Tag = (int)aiPSB_Psh     ;
    btPSB_PusherBw  -> Tag = (int)aiPSB_Psh     ;
    btPSB_Out       -> Tag = (int)yPSB_FeedingAC;

    btSTL_YTFw      -> Tag = (int)miSTL_YTop    ;
    btSTL_YTBw      -> Tag = (int)miSTL_YTop    ;
    btSTL_YBFw      -> Tag = (int)miSTL_YBtm    ;
    btSTL_YBBw      -> Tag = (int)miSTL_YBtm    ;
    btREJ_XFw       -> Tag = (int)miREJ_XBtm    ;
    btREJ_XBw       -> Tag = (int)miREJ_XBtm    ;

    btULD_CmpUp     -> Tag = (int)aiULD_Cmp     ;
    btULD_CmpDn     -> Tag = (int)aiULD_Cmp     ;

    btULD_In        -> Tag = (int)yULD_MgzInAC  ;
    btULD_Out       -> Tag = (int)yULD_MgzOutAC ;

    btULD_ZUp       -> Tag = (int)miULD_ZCmp    ;
    btULD_ZDn       -> Tag = (int)miULD_ZCmp    ;
    btULD_YBw       -> Tag = (int)miULD_YCmp    ;
    btULD_YFw       -> Tag = (int)miULD_YCmp    ;
                    
                    
    BtVaccumIn      -> Tag = (int)ySTL_Vaccum   ;
    btVaccumOut     -> Tag = (int)ySTL_VaccumEje;


    bLedInsp = false;
    //LAG.ReadFrmDeviceSet(); //�۾���.
//    if(FM_GetLan() == lsKorean) LAG.LoadFrmDeviceSetHan();
//    switch(FM_GetLan())
//    {
//        default        :                             break ;
//        case lsEnglish :                             break ;
//        case lsKorean  : LAG.LoadFrmDeviceSetHan();  break ;
//        case lsChinese :                             break ;
//    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;                  

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);



    DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riTmp].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riMST].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riSTL].SetMaxColRow(1                 ,1                  );

    //DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    //DM.ARAY[riWRP].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    //DM.ARAY[riPS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );

    /*
    if(OM.DevOptn.bLtHeaterNum2) IO_SetY(yETC_Heater1_2,true );
    else                         IO_SetY(yETC_Heater1_2,false);
    if(OM.DevOptn.bLtHeaterNum3) IO_SetY(yETC_Heater1_3,true );
    else                         IO_SetY(yETC_Heater1_3,false);
    if(OM.DevOptn.bRtHeaterNum2) IO_SetY(yETC_Heater2_2,true );
    else                         IO_SetY(yETC_Heater2_2,false);
    if(OM.DevOptn.bRtHeaterNum3) IO_SetY(yETC_Heater2_3,true );
    else                         IO_SetY(yETC_Heater2_3,false);
    �������� ���� ������ ���� ���� ���� �뵵�ε� ���ΰ������� ����. JS
    */
    FrmProbe -> Init() ;

//    if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //��� Eng ���κ� ����.
//    else                             FrmMain -> tmMainSet -> Enabled = true ;

    PCM.SetDaegyeom();  //��� Eng ���κ� ����.

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    if(bLedInsp){
        if((WRK.sPb1Msg != sPb1Insp && PCM.CheckDataCnt(riWRK, WRK.sPb1Msg)) ||
           (WRK.sPb2Msg != sPb2Insp && PCM.CheckDataCnt(riWRK, WRK.sPb2Msg))) {
            sPb1Insp = WRK.sPb1Msg;
            sPb2Insp = WRK.sPb2Msg;

            PCM.GetDataCh(riWRK , WRK.sPb1Msg , PROBE_CH1 , false);
            PCM.GetDataCh(riWRK , WRK.sPb2Msg , PROBE_CH2 , false);
            bLedInsp = false;
        }
    }

    bool bFoward;
    AnsiString sMsg;

    //lbSnsrCnt -> Caption = MM.m_iSTLSnsrCnt;
    //lbPnchCnt -> Caption = MM.m_iSTLPnchCnt;

    btSaveDevice -> Enabled = !SEQ._bRun ;//&& !FrmMain -> tmMainSet -> Enabled && !FrmMain -> tmSubSet -> Enabled ;

    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :
        break;

        case 1 : lbLDR_ZCmdPos    -> Caption = MT_GetCmdPos   (miLDR_ZCmp) ;
                 lbLDR_ZEncPos    -> Caption = MT_GetEncPos   (miLDR_ZCmp) ;
                 lbLDR_ZNLim      -> Color   = MT_GetNLimSnsr (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZHome      -> Color   = MT_GetHomeSnsr (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZPLim      -> Color   = MT_GetPLimSnsr (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZAlarm     -> Color   = MT_GetAlarm    (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZServo     -> Color   = MT_GetServo    (miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZStop      -> Color   = MT_GetStopInpos(miLDR_ZCmp) ? clLime : clSilver ;
                 lbLDR_ZHomeDone  -> Color   = MT_GetHomeEnd  (miLDR_ZCmp) ? clLime : clSilver ;

                 lbLDR_YCmdPos    -> Caption = MT_GetCmdPos   (miLDR_YCmp) ;
                 lbLDR_YEncPos    -> Caption = MT_GetEncPos   (miLDR_YCmp) ;
                 lbLDR_YNLim      -> Color   = MT_GetNLimSnsr (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YHome      -> Color   = MT_GetHomeSnsr (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YPLim      -> Color   = MT_GetPLimSnsr (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YAlarm     -> Color   = MT_GetAlarm    (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YServo     -> Color   = MT_GetServo    (miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YStop      -> Color   = MT_GetStopInpos(miLDR_YCmp) ? clLime : clSilver ;
                 lbLDR_YHomeDone  -> Color   = MT_GetHomeEnd  (miLDR_YCmp) ? clLime : clSilver ;

                 btLDR_In         ->Font->Color   = IO_GetY(yLDR_MgzInAC ) ? clLime : clBlack ;
                 btLDR_In         ->Caption       = IO_GetY(yLDR_MgzInAC ) ? "ON"   : "OFF"   ;
                 btLDR_Out        ->Font->Color   = IO_GetY(yLDR_MgzOutAC) ? clLime : clBlack ;
                 btLDR_Out        ->Caption       = IO_GetY(yLDR_MgzOutAC) ? "ON"   : "OFF"   ;

                 btLDR_PusherFW   ->Font->Color   = AT_Complete(aiLDR_Psh, ccFwd) ? clLime : clBlack ;
                 btLDR_PusherFW   ->Caption       = AT_Complete(aiLDR_Psh, ccFwd) ? "ON"   : "OFF"   ;
                 btLDR_PusherBW   ->Font->Color   = AT_Complete(aiLDR_Psh, ccBwd) ? clLime : clBlack ;
                 btLDR_PusherBW   ->Caption       = AT_Complete(aiLDR_Psh, ccBwd) ? "ON"   : "OFF"   ;

                 btLDR_CmpUp      ->Font->Color = AT_Complete(aiLDR_Cmp ,ccFwd) ? clLime : clBlack ;
                 btLDR_CmpDn      ->Font->Color = AT_Complete(aiLDR_Cmp ,ccBwd) ? clLime : clBlack ;

        break;
        case 2 : lbCmdP_Rl1X   ->Caption       = MT_GetCmdPos     (miPRB_XCmp);
                 lbEncP_Rl1X   ->Caption       = MT_GetEncPos     (miPRB_XCmp);
                 lbNLim_Rl1X   ->Color         = MT_GetNLimSnsr   (miPRB_XCmp) ? clLime : clSilver ;
                 lbHome_Rl1X   ->Color         = MT_GetHomeSnsr   (miPRB_XCmp) ? clLime : clSilver ;
                 lbPLim_Rl1X   ->Color         = MT_GetPLimSnsr   (miPRB_XCmp) ? clLime : clSilver ;
                 lbAlar_Rl1X   ->Color         = MT_GetAlarm      (miPRB_XCmp) ? clLime : clSilver ;
                 lbServ_Rl1X   ->Color         = MT_GetServo      (miPRB_XCmp) ? clLime : clSilver ;
                 lbStop_Rl1X   ->Color         = MT_GetStopInpos  (miPRB_XCmp) ? clLime : clSilver ;
                 lbHDon_Rl1X   ->Color         = MT_GetHomeEnd    (miPRB_XCmp) ? clLime : clSilver ;

                 lbCmdP_Rl2X   ->Caption       = MT_GetCmdPos     (miWRK_XCmp);
                 lbEncP_Rl2X   ->Caption       = MT_GetEncPos     (miWRK_XCmp);
                 lbNLim_Rl2X   ->Color         = MT_GetNLimSnsr   (miWRK_XCmp) ? clLime : clSilver ;
                 lbHome_Rl2X   ->Color         = MT_GetHomeSnsr   (miWRK_XCmp) ? clLime : clSilver ;
                 lbPLim_Rl2X   ->Color         = MT_GetPLimSnsr   (miWRK_XCmp) ? clLime : clSilver ;
                 lbAlar_Rl2X   ->Color         = MT_GetAlarm      (miWRK_XCmp) ? clLime : clSilver ;
                 lbServ_Rl2X   ->Color         = MT_GetServo      (miWRK_XCmp) ? clLime : clSilver ;
                 lbStop_Rl2X   ->Color         = MT_GetStopInpos  (miWRK_XCmp) ? clLime : clSilver ;
                 lbHDon_Rl2X   ->Color         = MT_GetHomeEnd    (miWRK_XCmp) ? clLime : clSilver ;

                 lbCmdP_Rl3X   ->Caption       = MT_GetCmdPos     (miPSB_XCmp);
                 lbEncP_Rl3X   ->Caption       = MT_GetEncPos     (miPSB_XCmp);
                 lbNLim_Rl3X   ->Color         = MT_GetNLimSnsr   (miPSB_XCmp) ? clLime : clSilver ;
                 lbHome_Rl3X   ->Color         = MT_GetHomeSnsr   (miPSB_XCmp) ? clLime : clSilver ;
                 lbPLim_Rl3X   ->Color         = MT_GetPLimSnsr   (miPSB_XCmp) ? clLime : clSilver ;
                 lbAlar_Rl3X   ->Color         = MT_GetAlarm      (miPSB_XCmp) ? clLime : clSilver ;
                 lbServ_Rl3X   ->Color         = MT_GetServo      (miPSB_XCmp) ? clLime : clSilver ;
                 lbStop_Rl3X   ->Color         = MT_GetStopInpos  (miPSB_XCmp) ? clLime : clSilver ;
                 lbHDon_Rl3X   ->Color         = MT_GetHomeEnd    (miPSB_XCmp) ? clLime : clSilver ;

                 btUp_RlClamp1 ->Font->Color   = AT_Complete(aiPRB_Cmp   ,ccBwd) ? clLime   : clBlack ;
                 btDn_RlClamp1 ->Font->Color   = AT_Complete(aiPRB_Cmp   ,ccFwd) ? clLime   : clBlack ;

                 //btUp_IdCheck  ->Font->Color   = AT_Complete(aiPRB_IdChk ,ccFwd) ? clLime   : clBlack ;
                 //btDn_IdCheck  ->Font->Color   = AT_Complete(aiPRB_IdChk ,ccBwd) ? clLime   : clBlack ;

                 btUp_RlClamp2 ->Font->Color   = AT_Complete(aiWRK_Cmp   ,ccBwd) ? clLime   : clBlack ;
                 btDn_RlClamp2 ->Font->Color   = AT_Complete(aiWRK_Cmp   ,ccFwd) ? clLime   : clBlack ;

                 btUp_RlClamp3 ->Font->Color   = AT_Complete(aiPSB_Cmp   ,ccBwd) ? clLime   : clBlack ;
                 btDn_RlClamp3 ->Font->Color   = AT_Complete(aiPSB_Cmp   ,ccFwd) ? clLime   : clBlack ;

                 btRail_Up ->Font->Color   = AT_Complete(aiPRB_HtBlk   ,ccFwd) ? clLime   : clBlack ;
                 btRail_Dn ->Font->Color   = AT_Complete(aiPRB_HtBlk   ,ccBwd) ? clLime   : clBlack ;

                 btPRB_In      ->Font->Color   = IO_GetY(yPRB_FeedingAC ) ? clLime : clBlack ;
                 btPRB_In      ->Caption       = IO_GetY(yPRB_FeedingAC ) ? "ON"   : "OFF"   ;
        break;
        case 3 : lbCmdP_PbB  ->Caption     = MT_GetCmdPos     (miWRK_ZBtm);
                 lbEncP_PbB  ->Caption     = MT_GetEncPos     (miWRK_ZBtm);
                 lbNLim_PbB  ->Color       = MT_GetNLimSnsr   (miWRK_ZBtm) ? clLime : clSilver ;
                 lbHome_PbB  ->Color       = MT_GetHomeSnsr   (miWRK_ZBtm) ? clLime : clSilver ;
                 lbPLim_PbB  ->Color       = MT_GetPLimSnsr   (miWRK_ZBtm) ? clLime : clSilver ;
                 lbAlar_PbB  ->Color       = MT_GetAlarm      (miWRK_ZBtm) ? clLime : clSilver ;
                 lbServ_PbB  ->Color       = MT_GetServo      (miWRK_ZBtm) ? clLime : clSilver ;
                 lbStop_PbB  ->Color       = MT_GetStopInpos  (miWRK_ZBtm) ? clLime : clSilver ;
                 lbHDon_PbB  ->Color       = MT_GetHomeEnd    (miWRK_ZBtm) ? clLime : clSilver ;

                 lbCmdP_PbBY ->Caption     = MT_GetCmdPos     (miWRK_YBtm);
                 lbEncP_PbBY ->Caption     = MT_GetEncPos     (miWRK_YBtm);
                 lbNLim_PbBY ->Color       = MT_GetNLimSnsr   (miWRK_YBtm) ? clLime : clSilver ;
                 lbHome_PbBY ->Color       = MT_GetHomeSnsr   (miWRK_YBtm) ? clLime : clSilver ;
                 lbPLim_PbBY ->Color       = MT_GetPLimSnsr   (miWRK_YBtm) ? clLime : clSilver ;
                 lbAlar_PbBY ->Color       = MT_GetAlarm      (miWRK_YBtm) ? clLime : clSilver ;
                 lbServ_PbBY ->Color       = MT_GetServo      (miWRK_YBtm) ? clLime : clSilver ;
                 lbStop_PbBY ->Color       = MT_GetStopInpos  (miWRK_YBtm) ? clLime : clSilver ;
                 lbHDon_PbBY ->Color       = MT_GetHomeEnd    (miWRK_YBtm) ? clLime : clSilver ;

                 lbCmdP_PbT  ->Caption     = MT_GetCmdPos     (miWRK_ZTop);
                 lbEncP_PbT  ->Caption     = MT_GetEncPos     (miWRK_ZTop);
                 lbNLim_PbT  ->Color       = MT_GetNLimSnsr   (miWRK_ZTop) ? clLime : clSilver ;
                 lbHome_PbT  ->Color       = MT_GetHomeSnsr   (miWRK_ZTop) ? clLime : clSilver ;
                 lbPLim_PbT  ->Color       = MT_GetPLimSnsr   (miWRK_ZTop) ? clLime : clSilver ;
                 lbAlar_PbT  ->Color       = MT_GetAlarm      (miWRK_ZTop) ? clLime : clSilver ;
                 lbServ_PbT  ->Color       = MT_GetServo      (miWRK_ZTop) ? clLime : clSilver ;
                 lbStop_PbT  ->Color       = MT_GetStopInpos  (miWRK_ZTop) ? clLime : clSilver ;
                 lbHDon_PbT  ->Color       = MT_GetHomeEnd    (miWRK_ZTop) ? clLime : clSilver ;

                 btWRK_1ClampUp  -> Font->Color = AT_Complete(aiPRB_Cmp   ,ccBwd) ? clLime : clBlack ;
                 btWRK_1ClampDn  -> Font->Color = AT_Complete(aiPRB_Cmp   ,ccFwd) ? clLime : clBlack ;
                 btWRK_2ClampUp  -> Font->Color = AT_Complete(aiWRK_Cmp   ,ccBwd) ? clLime : clBlack ;
                 btWRK_2ClampDn  -> Font->Color = AT_Complete(aiWRK_Cmp   ,ccFwd) ? clLime : clBlack ;
                 btWRK_3ClampUp  -> Font->Color = AT_Complete(aiPSB_Cmp   ,ccBwd) ? clLime : clBlack ;
                 btWRK_3ClampDn  -> Font->Color = AT_Complete(aiPSB_Cmp   ,ccFwd) ? clLime : clBlack ;
                 btPSB_Out2      -> Font->Color = IO_GetY(yPSB_FeedingAC ) ? clLime : clBlack ;
                 btPSB_Out2      -> Caption     = IO_GetY(yPSB_FeedingAC ) ? "ON"   : "OFF"   ;
/*
                 btUp_RlClamp1_->Font->Color = AT_Complete(aiPRB_Cmp   ,ccFwd) ? clLime : clBlack ;
                 btDn_RlClamp1_->Font->Color = AT_Complete(aiPRB_Cmp   ,ccBwd) ? clLime : clBlack ;
                 btUp_IdCheck_ ->Font->Color = AT_Complete(aiPRB_IdChk ,ccFwd) ? clLime : clBlack ;
                 btDn_IdCheck_ ->Font->Color = AT_Complete(aiPRB_IdChk ,ccBwd) ? clLime : clBlack ;

                 btUp_RlClamp2_->Font->Color = AT_Complete(aiWRK_Cmp   ,ccFwd) ? clLime : clBlack ;
                 btDn_RlClamp2_->Font->Color = AT_Complete(aiWRK_Cmp   ,ccBwd) ? clLime : clBlack ;
                 btUp_RlClamp3_->Font->Color = AT_Complete(aiPSB_Cmp   ,ccFwd) ? clLime : clBlack ;
                 btDn_RlClamp3_->Font->Color = AT_Complete(aiPSB_Cmp   ,ccBwd) ? clLime : clBlack ;

                 btPRB_In_     ->Font->Color = IO_GetY(yPRB_FeedingAC ) ? clLime : clBlack ;
                 btPRB_In_     ->Caption     = IO_GetY(yPRB_FeedingAC ) ? "ON"   : "OFF"   ;
*/
                 if(sgProbeCrnt -> RowCount != OM.DevInfo.iRowCnt ||
                    sgProbeCrnt -> ColCount != OM.DevOptn.iContColCnt){
                    sgProbeCrnt -> RowCount  = OM.DevInfo.iRowCnt     ;
                    sgProbeCrnt -> ColCount  = OM.DevOptn.iContColCnt ;
                 }

//                 for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++){
//                     for(int c = 0 ; c < OM.DevInfo.iRowCnt ; c++){
//                         sgProbeCrnt -> Cells[c][r] = dCChipVfCrnt[r][c] ;
//                     }
//                 }

        break;
        case 4 : lbPSB_YCmdPos    -> Caption = MT_GetCmdPos   (miPSB_XCmp) ;
                 lbPSB_YEncPos    -> Caption = MT_GetEncPos   (miPSB_XCmp) ;
                 lbPSB_YNLim      -> Color   = MT_GetNLimSnsr (miPSB_XCmp) ? clLime : clSilver ;
                 lbPSB_YHome      -> Color   = MT_GetHomeSnsr (miPSB_XCmp) ? clLime : clSilver ;
                 lbPSB_YPLim      -> Color   = MT_GetPLimSnsr (miPSB_XCmp) ? clLime : clSilver ;
                 lbPSB_YAlarm     -> Color   = MT_GetAlarm    (miPSB_XCmp) ? clLime : clSilver ;
                 lbPSB_YServo     -> Color   = MT_GetServo    (miPSB_XCmp) ? clLime : clSilver ;
                 lbPSB_YStop      -> Color   = MT_GetStopInpos(miPSB_XCmp) ? clLime : clSilver ;
                 lbPSB_YHomeDone  -> Color   = MT_GetHomeEnd  (miPSB_XCmp) ? clLime : clSilver ;

                 lbSTL_YTCmdPos    -> Caption = MT_GetCmdPos   (miSTL_YTop) ;
                 lbSTL_YTEncPos    -> Caption = MT_GetEncPos   (miSTL_YTop) ;
                 lbSTL_YTNLim      -> Color   = MT_GetNLimSnsr (miSTL_YTop) ? clLime : clSilver ;
                 lbSTL_YTHome      -> Color   = MT_GetHomeSnsr (miSTL_YTop) ? clLime : clSilver ;
                 lbSTL_YTPLim      -> Color   = MT_GetPLimSnsr (miSTL_YTop) ? clLime : clSilver ;
                 lbSTL_YTAlarm     -> Color   = MT_GetAlarm    (miSTL_YTop) ? clLime : clSilver ;
                 lbSTL_YTServo     -> Color   = MT_GetServo    (miSTL_YTop) ? clLime : clSilver ;
                 lbSTL_YTStop      -> Color   = MT_GetStopInpos(miSTL_YTop) ? clLime : clSilver ;
                 lbSTL_YTHomeDone  -> Color   = MT_GetHomeEnd  (miSTL_YTop) ? clLime : clSilver ;

                 lbSTL_YBCmdPos    -> Caption = MT_GetCmdPos   (miSTL_YBtm) ;
                 lbSTL_YBEncPos    -> Caption = MT_GetEncPos   (miSTL_YBtm) ;
                 lbSTL_YBNLim      -> Color   = MT_GetNLimSnsr (miSTL_YBtm) ? clLime : clSilver ;
                 lbSTL_YBHome      -> Color   = MT_GetHomeSnsr (miSTL_YBtm) ? clLime : clSilver ;
                 lbSTL_YBPLim      -> Color   = MT_GetPLimSnsr (miSTL_YBtm) ? clLime : clSilver ;
                 lbSTL_YBAlarm     -> Color   = MT_GetAlarm    (miSTL_YBtm) ? clLime : clSilver ;
                 lbSTL_YBServo     -> Color   = MT_GetServo    (miSTL_YBtm) ? clLime : clSilver ;
                 lbSTL_YBStop      -> Color   = MT_GetStopInpos(miSTL_YBtm) ? clLime : clSilver ;
                 lbSTL_YBHomeDone  -> Color   = MT_GetHomeEnd  (miSTL_YBtm) ? clLime : clSilver ;

                 lbREJ_XCmdPos    -> Caption = MT_GetCmdPos   (miREJ_XBtm) ;
                 lbREJ_XEncPos    -> Caption = MT_GetEncPos   (miREJ_XBtm) ;
                 lbREJ_XNLim      -> Color   = MT_GetNLimSnsr (miREJ_XBtm) ? clLime : clSilver ;
                 lbREJ_XHome      -> Color   = MT_GetHomeSnsr (miREJ_XBtm) ? clLime : clSilver ;
                 lbREJ_XPLim      -> Color   = MT_GetPLimSnsr (miREJ_XBtm) ? clLime : clSilver ;
                 lbREJ_XAlarm     -> Color   = MT_GetAlarm    (miREJ_XBtm) ? clLime : clSilver ;
                 lbREJ_XServo     -> Color   = MT_GetServo    (miREJ_XBtm) ? clLime : clSilver ;
                 lbREJ_XStop      -> Color   = MT_GetStopInpos(miREJ_XBtm) ? clLime : clSilver ;
                 lbREJ_XHomeDone  -> Color   = MT_GetHomeEnd  (miREJ_XBtm) ? clLime : clSilver ;

                 btSTL_YTCmpUp    ->Font->Color = AT_Complete(aiSTL_Top ,ccBwd)  ? clLime : clBlack ;
                 btSTL_YTCmpDn    ->Font->Color = AT_Complete(aiSTL_Top ,ccFwd)  ? clLime : clBlack ;

                 btSTL_YBCmpUp    ->Font->Color = AT_Complete(aiSTL_Btm ,ccFwd)  ? clLime : clBlack ;
                 btSTL_YBCmpDn    ->Font->Color = AT_Complete(aiSTL_Btm ,ccBwd)  ? clLime : clBlack ;

                 btPSB_PusherFw   ->Font->Color = AT_Complete(aiPSB_Psh ,ccFwd)  ? clLime : clBlack ;
                 btPSB_PusherBw   ->Font->Color = AT_Complete(aiPSB_Psh ,ccBwd)  ? clLime : clBlack ;

                 btPSB_Out        ->Font->Color = IO_GetY(yPSB_FeedingAC )       ? clLime : clBlack ;
                 btPSB_Out        ->Caption     = IO_GetY(yPSB_FeedingAC )       ? "ON"   : "OFF"   ;

                 btPSB_CmpUp      ->Font->Color = AT_Complete(aiPSB_Cmp , ccBwd) ? clLime : clBlack ;
                 btPSB_CmpDn      ->Font->Color = AT_Complete(aiPSB_Cmp , ccFwd) ? clLime : clBlack ;

                 BtVaccumIn       ->Font->Color = IO_GetY(ySTL_Vaccum      )     ? clLime : clBlack ;
                 BtVaccumIn       ->Caption     = IO_GetY(ySTL_Vaccum      )     ? "ON"   : "OFF"   ;

                 btVaccumOut      ->Font->Color = IO_GetY(ySTL_VaccumEje   )    ? clLime : clBlack ;

                 btVaccumOut      ->Caption     = IO_GetY(ySTL_VaccumEje   )    ? "ON"   : "OFF"   ;

                 btPSB_SrtTFwPnch ->Caption     = AnsiString("FW ") + OM.DevInfo.dRowPitch ;
                 btPSB_SrtTBwPnch ->Caption     = AnsiString("BW ") + OM.DevInfo.dRowPitch ;

                 btPSB_SrtBFw     ->Caption     = AnsiString("FW ") + OM.DevInfo.dRowPitch ;
                 btPSB_SrtBBw     ->Caption     = AnsiString("BW ") + OM.DevInfo.dRowPitch ;

                 btPSB_SrtTFwSnsr ->Caption     = AnsiString("FW ") + OM.DevInfo.dRowPitch ;
                 btPSB_SrtTBwSnsr ->Caption     = AnsiString("BW ") + OM.DevInfo.dRowPitch ;


        break;
        case 5 : lbULD_ZCmdPos    -> Caption = MT_GetCmdPos   (miULD_ZCmp) ;
                 lbULD_ZEncPos    -> Caption = MT_GetEncPos   (miULD_ZCmp) ;
                 lbULD_ZNLim      -> Color   = MT_GetNLimSnsr (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZHome      -> Color   = MT_GetHomeSnsr (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZPLim      -> Color   = MT_GetPLimSnsr (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZAlarm     -> Color   = MT_GetAlarm    (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZServo     -> Color   = MT_GetServo    (miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZStop      -> Color   = MT_GetStopInpos(miULD_ZCmp) ? clLime : clSilver ;
                 lbULD_ZHomeDone  -> Color   = MT_GetHomeEnd  (miULD_ZCmp) ? clLime : clSilver ;

                 lbULD_YCmdPos    -> Caption = MT_GetCmdPos   (miULD_YCmp) ;
                 lbULD_YEncPos    -> Caption = MT_GetEncPos   (miULD_YCmp) ;
                 lbULD_YNLim      -> Color   = MT_GetNLimSnsr (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YHome      -> Color   = MT_GetHomeSnsr (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YPLim      -> Color   = MT_GetPLimSnsr (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YAlarm     -> Color   = MT_GetAlarm    (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YServo     -> Color   = MT_GetServo    (miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YStop      -> Color   = MT_GetStopInpos(miULD_YCmp) ? clLime : clSilver ;
                 lbULD_YHomeDone  -> Color   = MT_GetHomeEnd  (miULD_YCmp) ? clLime : clSilver ;

                 btULD_In         ->Font->Color   = IO_GetY(yULD_MgzInAC ) ? clLime : clBlack ;
                 btULD_In         ->Caption       = IO_GetY(yULD_MgzInAC ) ? "ON"   : "OFF"   ;
                 btULD_Out        ->Font->Color   = IO_GetY(yULD_MgzOutAC) ? clLime : clBlack ;
                 btULD_Out        ->Caption       = IO_GetY(yULD_MgzOutAC) ? "ON"   : "OFF"   ;

                 btULD_CmpUp      ->Font->Color = AT_Complete(aiULD_Cmp ,ccFwd) ? clLime : clBlack ;
                 btULD_CmpDn      ->Font->Color = AT_Complete(aiULD_Cmp ,ccBwd) ? clLime : clBlack ;
        break;




    }

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo)
    {
        edColPitch      -> Text = OM.DevInfo.dColPitch   ;
        edRowPitch      -> Text = OM.DevInfo.dRowPitch   ;
        edColGroupCount -> Text = OM.DevInfo.iColGrCnt   ;
        edRowGroupCount -> Text = OM.DevInfo.iRowGrCnt   ;

        edColGroupGap   -> Text = OM.DevInfo.dColGrGap   ;
        edRowGroupGap   -> Text = OM.DevInfo.dRowGrGap   ;

        edColCount      -> Text = OM.DevInfo.iColCnt     ;
        edRowCount      -> Text = OM.DevInfo.iRowCnt     ;

        edCsSlotCnt     -> Text = OM.DevInfo.iCsSlCnt    ;
        edCsSlotPitch   -> Text = OM.DevInfo.dCsSlPitch  ;

    }
    else
    {
        OM.DevInfo.dColPitch  = edColPitch      -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch  = edRowPitch      -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt  = edColGroupCount -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt  = edRowGroupCount -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap  = edColGroupGap   -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap  = edRowGroupGap   -> Text.ToDouble() ;

        OM.DevInfo.iColCnt    = edColCount      -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt    = edRowCount      -> Text.ToInt()    ;

        OM.DevInfo.iCsSlCnt   = edCsSlotCnt     -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch = edCsSlotPitch   -> Text.ToDouble() ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        //Probe.
        ed1ChHigh            -> Text      = OM.DevOptn.d1ChHigh          ;
        ed1ChLow             -> Text      = OM.DevOptn.d1ChLow           ;
        ed2ChHigh            -> Text      = OM.DevOptn.d2ChHigh          ;
        ed2ChLow             -> Text      = OM.DevOptn.d2ChLow           ;
        edZennerHigh         -> Text      = OM.DevOptn.dZenerHigh        ;
        edZennerLow          -> Text      = OM.DevOptn.dZenerLow         ;
        edPinCnt             -> Text      = OM.DevOptn.iPinCnt           ;
        edLimInsFailCnt      -> Text      = OM.DevOptn.iLimInsFailCnt    ;
        edReInspCnt          -> Text      = OM.DevOptn.iReInspCnt        ;
        edContactCnt         -> Text      = OM.DevOptn.iContColCnt       ;
        edReGrabCnt1         -> Text      = OM.DevOptn.iReGrabCnt1       ;
        edReGrabPs1          -> Text      = OM.DevOptn.dReGrabPs1        ;
        edReGrabCnt2         -> Text      = OM.DevOptn.iReGrabCnt2       ;
        edReGrabPs2          -> Text      = OM.DevOptn.dReGrabPs2        ;
        edPrbSttDelay        -> Text      = OM.DevOptn.iPrbSttDelay      ;
        cbUseZenner          -> Checked   = OM.DevOptn.bUseZener         ;
//        cb1Ch1P              -> ItemIndex = OM.DevOptn.i1Ch1P            ;
//        cb1Ch1N              -> ItemIndex = OM.DevOptn.i1Ch1N            ;
//        cb1Ch2P              -> ItemIndex = OM.DevOptn.i1Ch2P            ;
//        cb1Ch2N              -> ItemIndex = OM.DevOptn.i1Ch2N            ;
//        cb1Ch3P              -> ItemIndex = OM.DevOptn.i1Ch3P            ;
//        cb1Ch3N              -> ItemIndex = OM.DevOptn.i1Ch3N            ;
        ed1ChLedCur          -> Text      = OM.DevOptn.i1ChLedCur        ;
        ed1ChZenCur          -> Text      = OM.DevOptn.i1ChZenCur        ;
//        cb2Ch1P              -> ItemIndex = OM.DevOptn.i2Ch1P            ;
//        cb2Ch1N              -> ItemIndex = OM.DevOptn.i2Ch1N            ;
//        cb2Ch2P              -> ItemIndex = OM.DevOptn.i2Ch2P            ;
//        cb2Ch2N              -> ItemIndex = OM.DevOptn.i2Ch2N            ;
//        cb2Ch3P              -> ItemIndex = OM.DevOptn.i2Ch3P            ;
//        cb2Ch3N              -> ItemIndex = OM.DevOptn.i2Ch3N            ;
        ed2ChLedCur          -> Text      = OM.DevOptn.i2ChLedCur        ;
        ed2ChZenCur          -> Text      = OM.DevOptn.i2ChZenCur        ;
        cbUseTwiceInsp       -> Checked   = OM.DevOptn.bUseTwiceInsp     ;

        ed1ChDelay           -> Text      = OM.DevOptn.i1ChDelay         ;
        ed1ChLedTime         -> Text      = OM.DevOptn.i1ChLedTime       ;
        ed1ChZenTime         -> Text      = OM.DevOptn.i1ChZenTime       ;
        ed2ChDelay           -> Text      = OM.DevOptn.i2ChDelay         ;
        ed2ChLedTime         -> Text      = OM.DevOptn.i2ChLedTime       ;
        ed2ChZenTime         -> Text      = OM.DevOptn.i2ChZenTime       ;
        cbChRvsCrnt          -> Checked   = OM.DevOptn.bChRvsCrnt        ;

        //Daegyoem
        edDgChipCnt          -> Text      = OM.DevOptn.iDgChipCnt        ;
        edSwitchingDly       -> Text      = OM.DevOptn.iSwitchingDly     ;

        //Dev Option
        edPreRailOutVel      -> Text      = OM.DevOptn.dPreRailOutVel    ;
        edPostRailOutVel     -> Text      = OM.DevOptn.dPostRailOutVel   ;

        edLotEndMgzCnt       -> Text      = OM.DevOptn.iLotEndMgzCnt     ;
        edWaitDelay          -> Text      = OM.DevOptn.iWaitDelay / 1000 ;

//        cbLtHeaterNum2       -> Checked   = OM.DevOptn.bLtHeaterNum2     ;
//        cbLtHeaterNum3       -> Checked   = OM.DevOptn.bLtHeaterNum3     ;
//        cbRtHeaterNum2       -> Checked   = OM.DevOptn.bRtHeaterNum2     ;
//        cbRtHeaterNum3       -> Checked   = OM.DevOptn.bRtHeaterNum3     ;

        edREJPitch           -> Text      = OM.DevOptn.dREJPitch         ;

        edTrimPtchOfs        -> Text      = OM.DevOptn.dTrimPtchOfs      ;
        edWorkPtchOfs        -> Text      = OM.DevOptn.dWorkPtchOfs      ;

        edPSBReGripOfs       -> Text      = OM.DevOptn.dPSBReGripOfs     ;
        edPSBReGripCol       -> Text      = OM.DevOptn.iPSBReGripCol     ;

        edTrimRjtDly         -> Text      = OM.DevOptn.iTrimRjtDly       ;
        edTrimSnrDly         -> Text      = OM.DevOptn.iTrimSnrDly       ;

        edContactPinCnt      -> Text      = OM.DevOptn.iContactPinCnt    ;
        edLimContactPinCnt   -> Text      = OM.DevOptn.iLimContactPinCnt ;

        ed1chLedOffset       -> Text      = OM.DevOptn.d1chLedOffset     ;
        ed1chZennerOffset    -> Text      = OM.DevOptn.d1chZennerOffset  ;
        ed2chLedOffset       -> Text      = OM.DevOptn.d2chLedOffset     ;
        ed2chZennerOffset    -> Text      = OM.DevOptn.d2chZennerOffset  ;

        edDeviceType         -> Text      = OM.DevOptn.sDeviceType       ;
        edDeviceName         -> Text      = OM.DevOptn.sDeviceName       ;
    }
    else
    {
        // Probe
        OM.DevOptn.d1ChHigh          =  ed1ChHigh           -> Text.ToDouble()  ;
        OM.DevOptn.d1ChLow           =  ed1ChLow            -> Text.ToDouble()  ;
        OM.DevOptn.d2ChHigh          =  ed2ChHigh           -> Text.ToDouble()  ;
        OM.DevOptn.d2ChLow           =  ed2ChLow            -> Text.ToDouble()  ;
        OM.DevOptn.dZenerHigh        =  edZennerHigh        -> Text.ToDouble()  ;
        OM.DevOptn.iPrbSttDelay      =  edPrbSttDelay       -> Text.ToIntDef(0) ;
        OM.DevOptn.dZenerLow         =  edZennerLow         -> Text.ToDouble()  ;
        OM.DevOptn.iPinCnt           =  edPinCnt            -> Text.ToIntDef(0) ;
        OM.DevOptn.iLimInsFailCnt    =  edLimInsFailCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iReInspCnt        =  edReInspCnt         -> Text.ToIntDef(0) ;
        OM.DevOptn.iContColCnt       =  edContactCnt        -> Text.ToIntDef(0) ;
        OM.DevOptn.iReGrabCnt1       =  edReGrabCnt1        -> Text.ToIntDef(0) ;
        OM.DevOptn.dReGrabPs1        =  edReGrabPs1         -> Text.ToDouble( ) ;
        OM.DevOptn.iReGrabCnt2       =  edReGrabCnt2        -> Text.ToIntDef(0) ;
        OM.DevOptn.dReGrabPs2        =  edReGrabPs2         -> Text.ToDouble( ) ;
        OM.DevOptn.bUseZener         =  cbUseZenner         -> Checked          ;
//        OM.DevOptn.i1Ch1P            =  cb1Ch1P             -> ItemIndex        ;
//        OM.DevOptn.i1Ch1N            =  cb1Ch1N             -> ItemIndex        ;
//        OM.DevOptn.i1Ch2P            =  cb1Ch2P             -> ItemIndex        ;
//        OM.DevOptn.i1Ch2N            =  cb1Ch2N             -> ItemIndex        ;
//        OM.DevOptn.i1Ch3P            =  cb1Ch3P             -> ItemIndex        ;
//        OM.DevOptn.i1Ch3N            =  cb1Ch3N             -> ItemIndex        ;
        OM.DevOptn.i1ChLedCur        =  ed1ChLedCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i1ChZenCur        =  ed1ChZenCur         -> Text.ToIntDef(0) ;
//        OM.DevOptn.i2Ch1P            =  cb2Ch1P             -> ItemIndex        ;
//        OM.DevOptn.i2Ch1N            =  cb2Ch1N             -> ItemIndex        ;
//        OM.DevOptn.i2Ch2P            =  cb2Ch2P             -> ItemIndex        ;
//        OM.DevOptn.i2Ch2N            =  cb2Ch2N             -> ItemIndex        ;
//        OM.DevOptn.i2Ch3P            =  cb2Ch3P             -> ItemIndex        ;
//        OM.DevOptn.i2Ch3N            =  cb2Ch3N             -> ItemIndex        ;
        OM.DevOptn.i2ChLedCur        =  ed2ChLedCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i2ChZenCur        =  ed2ChZenCur         -> Text.ToIntDef(0) ;

        OM.DevOptn.i1ChDelay         =  ed1ChDelay          -> Text.ToIntDef(0) ;
        OM.DevOptn.i1ChLedTime       =  ed1ChLedTime        -> Text.ToIntDef(0) ;
        OM.DevOptn.i1ChZenTime       =  ed1ChZenTime        -> Text.ToIntDef(0) ;
        OM.DevOptn.i2ChDelay         =  ed2ChDelay          -> Text.ToIntDef(0) ;
        OM.DevOptn.i2ChLedTime       =  ed2ChLedTime        -> Text.ToIntDef(0) ;
        OM.DevOptn.i2ChZenTime       =  ed2ChZenTime        -> Text.ToIntDef(0) ;
        OM.DevOptn.bChRvsCrnt        =  cbChRvsCrnt         -> Checked          ;
        OM.DevOptn.bUseTwiceInsp     =  cbUseTwiceInsp      -> Checked          ;

        //Daegyoem
        OM.DevOptn.iDgChipCnt        =  edDgChipCnt         -> Text.ToIntDef(1) ;
        OM.DevOptn.iSwitchingDly     =  edSwitchingDly      -> Text.ToIntDef(1) ;

        //Dev Option
        OM.DevOptn.dPreRailOutVel    =  edPreRailOutVel     -> Text.ToDouble( ) ;
        OM.DevOptn.dPostRailOutVel   =  edPostRailOutVel    -> Text.ToDouble( ) ;

//        OM.DevOptn.bLtHeaterNum2     =  cbLtHeaterNum2      -> Checked          ;
//        OM.DevOptn.bLtHeaterNum3     =  cbLtHeaterNum3      -> Checked          ;
//        OM.DevOptn.bRtHeaterNum2     =  cbRtHeaterNum2      -> Checked          ;
//        OM.DevOptn.bRtHeaterNum3     =  cbRtHeaterNum3      -> Checked          ;

        OM.DevOptn.dREJPitch         =  edREJPitch          -> Text.ToDouble( ) ;

        if(OM.DevOptn.iLotEndMgzCnt != edLotEndMgzCnt       -> Text.ToIntDef(0)){
            LDR._iMgzCnt             = edLotEndMgzCnt       -> Text.ToIntDef(0);
        }
        OM.DevOptn.iLotEndMgzCnt     =  edLotEndMgzCnt      -> Text.ToIntDef(0) ;
        OM.DevOptn.iWaitDelay        =  edWaitDelay         -> Text.ToIntDef(1) * 1000 ;

        OM.DevOptn.dPSBReGripOfs     =  edPSBReGripOfs      -> Text.ToDouble( ) ;
        OM.DevOptn.iPSBReGripCol     =  edPSBReGripCol      -> Text.ToIntDef(0) ;

        OM.DevOptn.dTrimPtchOfs      =  edTrimPtchOfs       -> Text.ToDouble( ) ;
        OM.DevOptn.dWorkPtchOfs      =  edWorkPtchOfs       -> Text.ToDouble( ) ;

        OM.DevOptn.iTrimRjtDly       =  edTrimRjtDly        -> Text.ToIntDef(0) ;
        OM.DevOptn.iTrimSnrDly       =  edTrimSnrDly        -> Text.ToIntDef(0) ;

        OM.DevOptn.iContactPinCnt    =  edContactPinCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iLimContactPinCnt =  edLimContactPinCnt  -> Text.ToIntDef(0) ;

        OM.DevOptn.d1chLedOffset     = ed1chLedOffset       -> Text.ToDouble();
        OM.DevOptn.d1chZennerOffset  = ed1chZennerOffset    -> Text.ToDouble();
        OM.DevOptn.d2chLedOffset     = ed2chLedOffset       -> Text.ToDouble();
        OM.DevOptn.d2chZennerOffset  = ed2chZennerOffset    -> Text.ToDouble();

        OM.DevOptn.sDeviceType       = edDeviceType         -> Text           ;
        OM.DevOptn.sDeviceName       = edDeviceName         -> Text           ;



    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);
    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btMtFwDn(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int    iSel  = ((TBitBtn *)Sender) -> Tag ;
    String sName = ((TBitBtn *)Sender) -> Name ;
    bool   bJog  = false ;
    double dUnit = 0.0 ;

    bool isZbtmWait  =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBChangePs)) && MT_GetStop(miWRK_ZBtm);

    bool isSrtTopFwd = !AT_Complete(aiSTL_Top,ccBwd) ;
    bool isSrtBtmFwd = !AT_Complete(aiSTL_Btm,ccBwd) ;

    switch(rgUnitUser -> ItemIndex) {
        case 0 : dUnit = 0.0                           ;
                 bJog  = true                          ; break ;
        case 1 : dUnit = 1                             ; break ;
        case 2 : dUnit = 0.5                           ; break ;
        case 3 : dUnit = 0.1                           ; break ;
        case 4 : dUnit = 0.05                          ; break ;
        case 5 : dUnit = edUnitUser -> Text.ToDouble() ; break ;
    }

    if(bJog) {
        if(sName == btWrk_BYUp->Name || sName == btWrk_BYDn->Name) if(!isZbtmWait) return;
        if(sName == btSTL_YTFw->Name || sName == btSTL_YTBw->Name) if(isSrtTopFwd) return;
        if(sName == btSTL_YBFw->Name || sName == btSTL_YBBw->Name) if(isSrtBtmFwd) return;

        MT_JogP(iSel) ;
    }
    else {
        if(sName == btWrk_BYUp->Name || sName == btWrk_BYDn->Name) {
        if(sName == btSTL_YTFw->Name || sName == btSTL_YTBw->Name) if(isSrtTopFwd) return;
        if(sName == btSTL_YBFw->Name || sName == btSTL_YBBw->Name) if(isSrtBtmFwd) return;

            if(!isZbtmWait) return;
        }
        MT_GoIncMan(iSel , dUnit) ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btMtUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int    iSel  = ((TBitBtn *)Sender) -> Tag ;

    if(rgUnitUser -> ItemIndex == 0) MT_Stop(iSel) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btMtBwDn(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int    iSel  = ((TBitBtn *)Sender) -> Tag ;
    bool   bJog  = false ;
    double dUnit = 0.0 ;
    String sName = ((TBitBtn *)Sender) -> Name ;

    bool isZbtmWait =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBChangePs)) && MT_GetStop(miWRK_ZBtm);

    bool isSrtTopFwd = !AT_Complete(aiSTL_Top,ccBwd) ;
    bool isSrtBtmFwd = !AT_Complete(aiSTL_Btm,ccBwd) ;

    switch(rgUnitUser -> ItemIndex) {
        case 0 : dUnit = 0.0                             ;
                 bJog  = true                            ; break ;
        case 1 : dUnit = 1                               ; break ;
        case 2 : dUnit = 0.5                             ; break ;
        case 3 : dUnit = 0.1                             ; break ;
        case 4 : dUnit = 0.05                            ; break ;
        case 5 : dUnit = edUnitUser -> Text.ToDouble()   ; break ;
    }

    if(bJog) {
        if(sName == btWrk_BYUp->Name || sName == btWrk_BYDn->Name) if(!isZbtmWait) return;
        if(sName == btSTL_YTFw->Name || sName == btSTL_YTBw->Name) if(isSrtTopFwd) return;
        if(sName == btSTL_YBFw->Name || sName == btSTL_YBBw->Name) if(isSrtBtmFwd) return;

        MT_JogN(iSel) ;
    }
    else {
        if(sName == btWrk_BYUp->Name || sName == btWrk_BYDn->Name) if(!isZbtmWait) return;
        if(sName == btSTL_YTFw->Name || sName == btSTL_YTBw->Name) if(isSrtTopFwd) return;
        if(sName == btSTL_YBFw->Name || sName == btSTL_YBBw->Name) if(isSrtBtmFwd) return;

        MT_GoIncMan(iSel , -dUnit) ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{

     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                             break ;
         case 1 : rgUnitUser = rgLDRJogUnit ;
                  edUnitUser = edLDRJogUnit ; break ;
         case 2 : rgUnitUser = rgRALJogUnit ;
                  edUnitUser = edRALJogUnit ; break ;
         case 3 : rgUnitUser = rgWRKJogUnit ;
                  edUnitUser = edWRKJogUnit ; break ;
         case 4 : rgUnitUser = rgSTLJogUnit ;
                  edUnitUser = edSTLJogUnit ; break ;
         case 5 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
         case 6 :                             break ;
     }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btLDR_CmpUpClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLDR_Cmp      )
    {
        if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
        bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    }
    if(iSel == aiPRB_Cmp      )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPRB_HtBlk    )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiWRK_Cmp      )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Cmp      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiSTL_Top      )  bRet = STL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiSTL_Btm      )  bRet = STL.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiULD_Cmp      )
    {
        if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
        bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    }
    if(iSel == aiLDR_Psh      )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;
    if(iSel == aiPSB_Psh      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccFwd) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btLDR_CmpDnClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

    if(iSel == aiLDR_Cmp      )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

    if(iSel == aiPRB_Cmp      )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPRB_HtBlk    )  bRet = PRB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiWRK_Cmp      )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Cmp      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiSTL_Top      )  bRet = STL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiSTL_Btm      )  bRet = STL.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiULD_Cmp      )  bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiLDR_Psh      )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;
    if(iSel == aiPSB_Psh      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , ccBwd) ;

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccBwd) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btToolChangeClick(TObject *Sender)
{
    MM.SetManCycle(mcWRK_ToolChng);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btResetRptClick(TObject *Sender)
{
    //SEQ.Reset();
    CWorkZone::SStat Stat = WRK.GetStat(); Stat.bRepeat = false ; WRK.SetStat(Stat);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btCntctRptClick(TObject *Sender)                   
{
    if(DM.ARAY[riWRK].CheckAllStat(csNone)) {FM_MsgOk("ERR","No Strip in the WorkZone"); return ;}

    MM.SetManCycle(mcWRK_RptCntct) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btAutoCal1Click(TObject *Sender)
{
    double dReGrabPs ;
    int    iReGrabCol;
    int    iGrCnt    ;
    iReGrabCol = edReGrabCnt1 -> Text.ToIntDef(0) ;
    dReGrabPs  = (iReGrabCol - OM.DevOptn.iContColCnt) * (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs ) ;

    if( OM.DevInfo.iColGrCnt > 1 ) {
//        iGrCnt     = (iReGrabCol - 1 ) / OM.DevInfo.iColGrCnt ;
//        dReGrabPs +=  iGrCnt * OM.DevInfo.dColGrGap ;
    }
    edReGrabPs1 -> Text = dReGrabPs ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btAutoCal2Click(TObject *Sender)
{
    double dReGrabPs ;
    int    iReGrabCol;
    int    iGrCnt    ;
    iReGrabCol = edReGrabCnt2 -> Text.ToIntDef(0) - edReGrabCnt1 -> Text.ToIntDef(0) ;
    dReGrabPs  = (iReGrabCol )* (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs) ;
//    if( OM.DevInfo.iColGrCnt > 1 ) {
//        iGrCnt     = (iReGrabCol - 1 ) / OM.DevInfo.iColGrCnt ;
//        dReGrabPs +=  iGrCnt * OM.DevInfo.dColGrGap ;
//    }
    edReGrabPs2 -> Text = dReGrabPs ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btLDR_InClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  IO_GetY(iSel);
    IO_SetY(iSel , !bState);

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btInspStartClick(TObject *Sender)
{
    int iSel = ((TButton *)Sender) -> Tag ;
//    AnsiString sMsg, sTemp;

    //SET,12,0400,0100,15,T#
    AnsiString sMsg  = "" ;
    AnsiString sTemp ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;
    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 1000 ) / 50 ;

    switch(iSel) {
        case 0 : RS232C_Pb1ch->Open(0);
                 RS232C_Pb2ch->Open(1);
                 break;
        case 1 : RS232C_Pb1ch->Close();
                 RS232C_Pb2ch->Close();
                 break;
        case 2 : //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();
                 //else                             FrmMain-> tmMainSet -> Enabled = true ;
                 PCM.SetDaegyeom();
                 break;
        case 3 : PCM.Start(1);
                 PCM.Start(2);
                 bLedInsp = true;
                 break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btPSBAutoCALClick(TObject *Sender)
{
    double dReGrabPs ;
    int    iReGrabCol;

    iReGrabCol = edPSBReGripCol -> Text.ToIntDef(0) ;
    dReGrabPs  = iReGrabCol * (OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs)  ;

    edPSBReGripOfs -> Text = dReGrabPs ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::brContactCntResetClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to Reset?") != mrYes) return ;
    edContactPinCnt -> Text = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btPSB_SrtTFwPnchClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

















