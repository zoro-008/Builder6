//---------------------------------------------------------------------------
#pragma hdrstop
#include "SEQUnit.h"
#include "PaixMotion.h"
#include "global.h"
#include "Timer.h"
#include "UtilDefine.h"
#include "FormMsgOk.h"
#include "OptionMan.h"
#include "SLogUnit.h"
#include <math.h>
#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
CSEQ *SEQ;

#define Msg if(m_pMsgFunc)Msg
__fastcall CSEQ::CSEQ(void)
{
    Init();
}
//-------------------------------------------------------------------------
__fastcall CSEQ::~CSEQ(void)
{
    Close();
}
//-------------------------------------------------------------------------
void __fastcall CSEQ::Reset(void)
{
    //Check running flag.
    if (m_bRun              ) return;
    m_bBtnResetDown = true;

    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmTemp     .Clear();

    MotrAlrmReset();

    //Init. Var.
    m_bBtnReset  = false ;
    m_bBtnStart  = false ;
    m_bBtnStop   = false ;

    m_bRun       = false ;
    m_iStep      = scIdle;
    m_bFlick     = false ;
    m_bBtnEmg    = false ;
    m_bErr       = false ;


    //정상완료했다는 부저.
    m_bWorkEnd    = false;
    m_bWorkfinish = false;

    m_bBtnResetDown = false;
    Trace("Sequence","Reset");

}
void CSEQ::Init()
{
    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmTemp     .Clear();

    //Init. Var.
    m_bBtnReset  = false ;
    m_bBtnStart  = false ;
    m_bBtnStop   = false ;

    m_bRun       = false ;
    m_iStep      = scIdle;
    m_bReqStop   = false ;
    m_bFlick     = false ;
    m_bBtnEmg    = false ;
    m_bErr       = false ;

    //정상완료했다는 부저.
    m_bWorkEnd    = false;
    m_bWorkfinish = false;
    m_bBtnResetDown = false;

    Trace("Sequence","Init");

}
void CSEQ::Close()
{
}
//-------------------------------------------------------------------------
void __fastcall CSEQ::UpdateButton(void)
{
    pPaix->GetOutputX(g_lOutputXIo);
    pPaix->GetInputX(g_lInputXIo);

    bool bStartSw = g_lInputXIo[xETC_StartSw] || m_bBtnStart;
    bool bStopSw  = g_lInputXIo[xETC_StopSw ] || m_bBtnStop ;
    bool bResetSw = m_bBtnReset ;
    bool bEmgSw   = g_lInputXIo[xETC_EmgSw  ] || m_bBtnEmg  ;

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;

    if(bEmgSw){
        pPaix -> SuddenStop(miSpinMt);
        pPaix -> SuddenStop(miTiltMt);
        pPaix -> Stat[miTiltMt].bHomeDone = false;
        m_bBtnStop = true;
    }

    //도어 센서 감지
//    pPaix -> GetInputX(g_lInputXIo);
//    if(!g_lInputXIo[xETC_DoorSnsrNotDetect]){

    bool bFwdCom =  g_lOutputXIo[yETC_DoorFw] && g_lInputXIo[xETC_DoorFw];
    bool bBwdCom =  g_lOutputXIo[yETC_DoorBw] && g_lInputXIo[xETC_DoorBw];
    bool bDoorSr = !g_lInputXIo[xETC_DoorSnsrNotDetect] ;
    static bool bCheck = false;

    if(bFwdCom || bBwdCom) bCheck = false;
    if(!bFwdCom && !bBwdCom && bDoorSr && !bCheck)
    {
        if(g_lOutputXIo[yETC_DoorFw])
        {
            pPaix -> SetOutputXBit(yETC_DoorFw, false);
            pPaix -> SetOutputXBit(yETC_DoorBw, true );
            FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
            Trace("Error","Door Sensor가 감지되었습니다.");
            m_bReqStop = true;
            bCheck = true;
        }
        else//if(g_lOutputXIo[yETC_DoorBw])
        {
//            pPaix -> SetOutputXBit(yETC_DoorFw, true );
            pPaix -> SetOutputXBit(yETC_DoorBw, false);
            FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
            Trace("Error","Door Sensor가 감지되었습니다.");
            m_bReqStop = true;
            bCheck = true;
        }
    }


    //Buzzer Off.
    //if (isErr && bStopSw) TL_MuteSnd();

    //Set Condition Flags.
    bool isStopCon  = bStopSw  && m_bRun  ;
    bool isRunCon   = bStartSw ;
    bool isResetCon = bResetSw && !m_bRun ;

    if(m_bRun){
        pPaix -> SetOutputXBit(yETC_StartLp, true );
        pPaix -> SetOutputXBit(yETC_StopLp , false);
    }
    else {
        pPaix -> SetOutputXBit(yETC_StartLp, false);
        pPaix -> SetOutputXBit(yETC_StopLp , true );
    }
    //Run.
    if (isRunCon && !m_iStep                      ) { m_iStep = scToStartCon; }
    if (isStopCon                                 ) { m_bReqStop = true ;     }
    if (isResetCon && !m_iStep && !m_bBtnResetDown)   Reset() ;



    //if(!OM.MstOptn.bUsedFrstMachine){
    //if(m_iStep != scIdle && !g_lInputXIo[xETC_MaskPosSnsr]){
    //    m_bErr = true;
    //    m_bReqStop = true ;
    //    FrmMsgOk->MsgOk("Error","상부 마스크 센서가 감지 되고 있습니다.");
    //    Trace("Error","상부 Mask 센서 감지 에러.");
    //    m_iStep = scToStopCon;
    //    m_bRun  = false;
    //}
    //}
    //else{
    //    if(m_iStep != scIdle ){
    //        m_bErr = true;
    //        m_iStep = scIdle;
    //        m_bRun  = false;
    //    }
    //}       //JH   
    if (m_tmToStrt.OnDelay(m_iStep == scToStart || m_iStep == scToStart , 15000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","Start 동작중 시간초과 에러.");
        Trace("Error","Start 동작중 시간초과 에러.");
        m_iStep = scIdle;
        m_bRun  = false;
    }

    //Time Out.
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , 15000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","Stop 동작중 시간초과 에러.");
        Trace("Error","Stop 동작중 시간초과 에러.");
        m_bRun = false ;
        m_iStep    = scIdle;
        m_bReqStop = false;
    }

    //Running Step.
    switch (m_iStep) {
        case scIdle       : return;

        case scToStartCon : m_bRun = true;
                            m_bReqStop = false;
                            m_iStep= scToStart;
                            pPaix -> SetOutputXBit(yETC_TwRedLp , false);
                            pPaix -> SetOutputXBit(yETC_TwGrnLp , false);
                            pPaix -> SetOutputXBit(yETC_TwYelLp , true );
                            Trace("ToStart","Start");
                            Step.iToStart = 10;
                            return ;

        case scToStart    : if(!m_bReqStop) {
                                if(!ToStart()) return ;
                                pPaix -> SetOutputXBit(yETC_TwRedLp , false);
                                pPaix -> SetOutputXBit(yETC_TwGrnLp , true );
                                pPaix -> SetOutputXBit(yETC_TwYelLp , false);
                                m_iStep=scCycleClean ;
                                Trace("CycleClean","Start");
                                Step.iCycle = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            m_bErr = true;
                            //m_bWorkfinish = true;
                            //m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            return ;

        case scCycleClean : if(!m_bReqStop) {
                                if(!CycleClean()) return ;
                                pPaix -> SetOutputXBit(yETC_TwRedLp , false);
                                pPaix -> SetOutputXBit(yETC_TwGrnLp , true );
                                pPaix -> SetOutputXBit(yETC_TwYelLp , false);
                                Trace("CycleAir","Start");
                                m_iStep=scCycleAIR;
                                Step.iCycle = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            m_bErr = true;
                            //m_bWorkfinish = true;
                            //m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            return ;

        case scCycleAIR   : if(!m_bReqStop) {
                                if(!CycleAir()) return ;
                                pPaix -> SetOutputXBit(yETC_TwRedLp , false);
                                pPaix -> SetOutputXBit(yETC_TwGrnLp , true );
                                pPaix -> SetOutputXBit(yETC_TwYelLp , false);
                                m_iStep=scCycleDRY;
                                Trace("CycleDry","Start");
                                Step.iCycle = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_bErr = true;
                            //m_bWorkfinish = true;
                            //m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            m_iStep=scToStopCon ;
                            return ;

        case scCycleDRY   : if(!m_bReqStop) {
                                if(!CycleDry()) return ;
                                pPaix -> SetOutputXBit(yETC_TwRedLp , false);
                                pPaix -> SetOutputXBit(yETC_TwGrnLp , true );
                                pPaix -> SetOutputXBit(yETC_TwYelLp , false);
                                m_iStep=scToStopCon;
                                Trace("ToStopCon","Start");
                                //Step.iToStop = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_bErr = true;
                            //m_bWorkfinish = true;
                            //m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            m_iStep=scToStopCon ;
                            return ;

        case scToStopCon :  m_iStep = scToStop;
                            Trace("ToStop","Start");
                            Step.iToStop = 10;
                            m_bRun = false;
                            return ;


        case scToStop    :  if(!ToStop()) return ;
                            if(OM.MstOptn.bUsedDryRun && !m_bReqStop){
                                //준선이의 가라놀이~~~!!!JS
                                if(!m_tmGara.OnDelay(true , 60000))return ;
                                m_iStep = scToStartCon;
                                m_bReqStop = false ;
                                return;
                            }
                            pPaix -> SetOutputXBit(yETC_TwRedLp , true );
                            pPaix -> SetOutputXBit(yETC_TwGrnLp , false);
                            pPaix -> SetOutputXBit(yETC_TwYelLp , false);
                            m_iStep = scIdle ;
                            m_bReqStop = false ;
                            return;
        }

}
//-------------------------------------------------------------------------
bool __fastcall CSEQ::ToStart(void)
{
    //Check Time Out.
    if (m_tmToStrt.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart , 5000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","Start 동작 중 시간초과 에러.");
        Trace("Error","Start 동작 중 시간초과 에러.");
    }
    //if(!OM.MstOptn.bUsedFrstMachine){
    if(!OM.MstOptn.bUsedDryRun && !g_lInputXIo[xETC_MaskPosSnsr]){  //JH  if(!g_lInputXIo[xETC_MaskPosSnsr]){ 에서 수정   근대 이거 다 9번으로 가야 할꺼 같은데요...IO 인풋 9번이 마스크 센서 아닌가요...??
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","마스크 센서가 감지 되고 있습니다.");
        Trace("Error","Mask 센서 감지 에러.");
        m_bReqStop = true;
        Step.iCycle = 0;
        return true;
    }
    //}
    PreStep.iToStart = Step.iToStart ;

    bool bHomeDone;
    //Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0;
                 return true ;


        case 10: m_bWorkEnd = false;
                 bHomeDone = pPaix -> GetHomeDone(miTiltMt);
                 if(!bHomeDone){
                     m_bReqStop = true;
                     m_bErr = true;
                     FrmMsgOk -> MsgOk("Error", "Reset을 눌러주세요.");
                     Trace("Error", "Reset을 눌러주세요.");
                     Step.iToStart = 0;
                     return true;
                 }
                 if(!OM.MstOptn.bUsedFrstMachine && !OM.MstOptn.bIngrCvrSnsr){
                     pPaix -> SetOutputXBit(yETC_MskCycleFwBw , true);
                 }
                 m_tmTemp.Clear();
                 Trace("ToStart",AnsiString(Step.iToStart).c_str());
                 Step.iToStart++;
                 return false;

        case 11: if(!m_tmTemp.OnDelay(true,500)) return false;
                 if(!OM.MstOptn.bUsedFrstMachine && !OM.MstOptn.bIngrCvrSnsr) {
                     if(!OM.MstOptn.bUsedDryRun){
                         if(g_lInputXIo[xETC_MskDetectBw]) return false;
                         if(g_lInputXIo[xETC_MskDetectNot]){
                             m_bErr = true;
                             FrmMsgOk->MsgOk("Error","내부 마스크가 감지되지 않았습니다.");
                             Trace("Error","내부 마스크가 감지되지 않았습니다.");
                             m_bReqStop = true;
                             Step.iToStart = 0;
                             return true;
                         }
                     }
                     pPaix -> SetOutputXBit(yETC_MskCycleFwBw , false);
                 }
                 m_tmTemp.Clear();
                 Trace("ToStart",AnsiString(Step.iToStart).c_str());
                 Step.iToStart++;
                 return false;

        case 12: if(!OM.MstOptn.bUsedFrstMachine){
                     if(!g_lInputXIo[xETC_MskDetectBw]) return false;
                 }
                 if(!m_tmTemp.OnDelay(true, 500))return false;
                 pPaix -> SetServoOn(miSpinMt , true);

                 if(!g_lInputXIo[xETC_DoorSnsrNotDetect]){
                     m_bErr = true;
                     FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
                     Trace("Error","Door Sensor가 감지되었습니다.");
                     m_bReqStop = true;
                     Step.iToStart = 0;
                     return true;
                 }
                 pPaix -> SetOutputXBit(yETC_DoorFw, true );
                 pPaix -> SetOutputXBit(yETC_DoorBw, false);
                 m_tmTemp.Clear();
                 Trace("ToStart",AnsiString(Step.iToStart).c_str());
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!g_lInputXIo[xETC_DoorSnsrNotDetect]){
                     pPaix -> SetOutputXBit(yETC_DoorFw, false);
                     pPaix -> SetOutputXBit(yETC_DoorBw, true );
                     m_bErr = true;
                     FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
                     Trace("Error","Door Sensor가 감지되었습니다.");
                     m_bReqStop = true;
                     Step.iToStart = 0;
                     return true;
                 }

                 if(m_tmTemp.OnDelay(true, 4000)){
                     if(!g_lInputXIo[xETC_DoorFw]){
                         m_bErr = true;
                         FrmMsgOk->MsgOk("Error","Door가 닫히지 않았습니다.");
                         Trace("Error","Door가 닫히지 않았습니다.");
                         m_bReqStop = true;
                         Step.iToStart = 0;
                         return true;
                     }
                 }
                 if(!g_lInputXIo[xETC_DoorFw])return false;
                 Trace("ToStart",AnsiString(Step.iToStart).c_str());
                 Step.iToStart = 0   ;
                 return true ;
    }
}
//-------------------------------------------------------------------------
bool __fastcall CSEQ::ToStop(void)
{
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop , 10000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","ToStop 동작 중 시간초과 에러.");
        Trace("Error","ToStop 동작 중 시간초과 에러.");
    }
    PreStep.iToStop = Step.iToStop ;


    //Stat.bReqStop = false ;

    double dTiltCmdPos ;
    int    iTiltCmdPos ;
    int iTiltHomePos = 0;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: pPaix -> DecStop(miSpinMt);
                 pPaix -> DecStop(miTiltMt);
                 Trace("ToStop",AnsiString(Step.iToStop).c_str());
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(g_NmcData.nBusy[miSpinMt]) return false;
                 if(g_NmcData.nBusy[miTiltMt]) return false;
                 pPaix -> SetServoOn(miSpinMt, false);
                 pPaix -> SetOutputXBit(yETC_BNRAir       , false);
                 pPaix -> SetOutputXBit(yETC_BNRValve     , false);
                 pPaix -> SetOutputXBit(yETC_PMPAir1      , false);
                 pPaix -> SetOutputXBit(yETC_BTMValve     , false);
                 pPaix -> SetOutputXBit(yETC_IONAir       , false);
                 pPaix -> SetOutputXBit(yETC_DrainValve   , false);
                 pPaix -> SetOutputXBit(yETC_MskCycleFwBw , false);
                 //옵션 추가.JS
                 if(!OM.MstOptn.bUsedFrstMachine){
                     pPaix -> SetOutputXBit(yETC_PMPAir2  , true );
                 }
                 Trace("ToStop",AnsiString(Step.iToStop).c_str());
                 Step.iToStop++;
                 return false;

        case 12: pPaix -> SetCmdPos(miSpinMt, 0);
                 pPaix -> SetEncPos(miSpinMt, 0);
                 pPaix -> AbsMove(miTiltMt , 0);
                 Trace("ToStop",AnsiString(Step.iToStop).c_str());
                 Step.iToStop++;
                 return false;

        case 13: //dTiltCmdPos = g_NmcData.dCmd[miTiltMt];
                 //iTiltCmdPos = (dTiltCmdPos+1) / 910 ;
                 //if(iTiltCmdPos != iTiltHomePos)return false;
                 dTiltCmdPos = pPaix -> GetCmdPos(miTiltMt);
                 if(dTiltCmdPos >= 1 )return false; //홈위치로 갔는지 확인.
                 pPaix -> GetInputX(g_lInputXIo);
                 if(!g_lInputXIo[xETC_DoorSnsrNotDetect]){
                     m_bErr = true;
                     FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
                     Trace("Error","Door Sensor가 감지되었습니다.");
                     m_bReqStop = true;
                     Step.iToStart = 0;
                     return true;
                 }
                 pPaix -> SetOutputXBit(yETC_DoorFw, false);
                 pPaix -> SetOutputXBit(yETC_DoorBw, true );
                 Trace("ToStop",AnsiString(Step.iToStop).c_str());
                 Step.iToStop++;
                 return false;

        case 14: if(!g_lInputXIo[xETC_DoorBw]) return false;
                 m_bWorkfinish = true;
                 //준선이의 가라놀이~~~!!!JS
                 if(OM.MstOptn.bUsedDryRun) m_tmGara.Clear();
                 Trace("ToStop",AnsiString(Step.iToStop).c_str());
                 Step.iToStop = 0   ;
                 return true ;
    }
}
//-------------------------------------------------------------------------
bool __fastcall CSEQ::CycleClean(void)
{
    if (m_tmCycle.OnDelay(Step.iCycle && !PreStep.iCycle == Step.iCycle , OM.DevOptn.iCleanTime + 1000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleClean 동작 중 시간초과 에러.");
        Trace("Error","CycleClean 동작 중 시간초과 에러.");
    }
    PreStep.iCycle = Step.iCycle ;

    pPaix -> GetInputX(g_lInputXIo);
    //if(!OM.MstOptn.bUsedFrstMachine){
    //if(!g_lInputXIo[xETC_MaskPosSnsr]){  //JH if(!g_lInputXIo[xETC_MaskPosSnsr]){에서 수정
    //    m_bErr = true;
    //    FrmMsgOk->MsgOk("Error","상부 마스크 센서가 감지 되고 있습니다.");
    //    Trace("Error","상부 Mask 센서 감지 에러.");
    //    m_bReqStop = true;
    //    Step.iCycle = 0;
    //    return true;
    //}
    //}
    AnsiString sReturn;

    const int iAcc = 327680;
    const int iDcc = 3276;                  //Dcc이 이상하여서 올라가거나 내려가면 Busy가 나중에 떨어짐......
    double dTiltCmdPos;                     //원인....파악 불가..............젠장.....
    dTiltCmdPos = pPaix -> GetCmdPos(miTiltMt);

    static double dLastTiltCmdPos = -1 ;

    static int iTiltStep = 0 ;
    if(Step.iCycle == 10) iTiltStep = 10 ;
    switch(iTiltStep) {
        default : iTiltStep = 0  ; break ;
        case 10 : //pPaix -> SetOverrideDSpeed(miTiltMt , pPaix -> GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));
                  if(g_NmcData.nBusy[miTiltMt]) pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed));
                  iTiltStep++;
                  break ;

        case 11 : if(g_NmcData.nBusy[miTiltMt]) break ;
                  if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                  else                                                    pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
                  break;
    }


    //Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iCycle) {
        default: Step.iCycle = 0;
                 return true ;

        case 10: m_tmClean.Clear();
                 m_tmTemp.Clear();
                 pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iCleanRPM);
                 sReturn = pPaix -> Log[miSpinMt].iReturn;
                 Trace(pPaix ->Log[miSpinMt].sFunction.c_str() , sReturn.c_str());
                 //pPaix -> SetSpeed(miTiltMt, 1, iAccDcc, iAccDcc, pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
                 //pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs);
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle++;
                 return false;

        case 11: //if(!OM.CmnOptn.bUsedWaterPump) {
                 //    pPaix -> SetOutputXBit(yETC_BNRAir      , true );
                 //    pPaix -> SetOutputXBit(yETC_BNRValve    , true );
                 //}
                 //else {
                 //    //옵션 추가. JS
                 //    pPaix -> SetOutputXBit(yETC_BTMValve     , true );
                 //}
                 if(OM.DevOptn.bUseTopNozzle){
                     pPaix -> SetOutputXBit(yETC_BNRAir      , true );
                     pPaix -> SetOutputXBit(yETC_BNRValve    , true );
                 }
                 else if(OM.DevOptn.bUseBtmNozzle){
                     pPaix -> SetOutputXBit(yETC_BTMValve    , true );
                 }
                 else if(OM.DevOptn.bUseTwinNozzle){
                     pPaix -> SetOutputXBit(yETC_BNRAir      , true );
                     pPaix -> SetOutputXBit(yETC_BNRValve    , true );
                     pPaix -> SetOutputXBit(yETC_BTMValve    , true );
                 }

                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 m_tmTemp1.Clear();
                 Step.iCycle++;
                 return false;


        case 12: if(!m_tmTemp1.OnDelay(true , 1000)) return false;
                 if(OM.MstOptn.bUsedFrstMachine){
                         pPaix -> SetOutputXBit(yETC_PMPAir1  , true);
                     }
                 else {
                         pPaix -> SetOutputXBit(yETC_PMPAir1  , true );
                         pPaix -> SetOutputXBit(yETC_PMPAir2  , false);
                 }
                 m_tmTemp1.Clear();
                 Step.iCycle++;
                 return false;

        //SpinMotor가 안돌아 가는 현상떄문에 가라작업....JS
        case 13: if(!m_tmTemp1.OnDelay(true , 500)) return false;
                 if(!g_NmcData.nBusy[miSpinMt]){
                     pPaix -> DecStop(miSpinMt);
                     Step.iCycle++;
                     return false;
                 }
                 else {
                     Step.iCycle = 20;
                     return false ;
                 }

        case 14: pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iCleanRPM);
                 m_tmTemp1.Clear();
                 Step.iCycle = 13;
                 return false;

        case 20: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime))return false;
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle = 0;
                 return true;
    /*
        case 10: m_tmClean.Clear();
                 m_tmTemp.Clear();
                 pPaix -> MoveRPM(miSpinMt , OM.DevOptn.iCleanRPM     );
                 pPaix -> MoveRPM(miTiltMt , OM.DevOptn.iTiltMtSpeed  );
                 pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                 Trace("CycleClean","10");
                 Step.iCycle ++ ;
                 return false ;

        case 11: dTiltCmdPos = NmcData.dCmd[miTiltMt];
                 iTiltCmdPos = NmcData.dCmd[miTiltMt]/(PULSE_PER_REV / 36) ;
                 if(m_tmTemp.OnDelay(true, 5000)){
                     m_bErr = true;
                     m_bReqStop = true;
                     Trace("CycleClean","모터 안움직임.");
                     Step.iCycle = 0;
                     return true;
                 }
                 if(iTiltCmdPos < OM.DevOptn.iTiltMtStartPs - 5)return false;
                 pPaix -> JogMove(miSpinMt, CW);
                 m_tmTemp.Clear();
                 //pPaix -> SetOutputXBit(yETC_Vaccum, true);
                 Trace("CycleClean","11");
                 Step.iCycle ++;
                 return false;

        case 12: if(!OM.CmnOptn.bUsedWaterPump) {
                     pPaix -> SetOutputXBit(yETC_BNRAir   , true);
                     pPaix -> SetOutputXBit(yETC_BNRValve , true);
                 }
                 else {
                     pPaix -> SetOutputXBit(yETC_PMPAir   , true);
                     pPaix -> SetOutputXBit(yETC_PMPValve , true);
                 }
                 //pPaix -> SetOutputXBit(yETC_AirBlower1, true);
                 //pPaix -> SetOutputXBit(yETC_SupplyValve, true);
                 if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 Trace("CycleClean","12");
                 Step.iCycle ++;
                 return false;

        case 13: if(m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime)){
                     Trace("CycleClean","End");
                     Step.iCycle = 0;
                     return true;
                 }                                                            //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(NmcData.nBusy[miTiltMt]) return false;
                 pPaix -> AbsMove(miTiltMt, OM.DevOptn.iTiltMtStopPs);
                 Trace("CycleClean","13");
                 Step.iCycle++;
                 return false;

        case 14: if(m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime)){
                     Trace("CycleClean","End");
                     Step.iCycle = 0;
                     return true;
                 }
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(NmcData.nBusy[miTiltMt]) return false;
                 pPaix -> AbsMove(miTiltMt, OM.DevOptn.iTiltMtStartPs);
                 Trace("CycleClean","14");
                 Step.iCycle++;
                 return false;

        case 15: if(m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime)){
                     Trace("CycleClean","End");
                     Step.iCycle = 0;
                     return true;
                 }
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(NmcData.nBusy[miTiltMt]) return false;
                 Trace("CycleClean","15");
                 Step.iCycle = 13;
                 return false;
    }
    */
    }
}
//-------------------------------------------------------------------------
bool __fastcall CSEQ::CycleAir(void)
{
    if (m_tmCycle.OnDelay(Step.iCycle && !PreStep.iCycle == Step.iCycle , 10000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleAir 동작 중 시간초과 에러.");
        Trace("Error","CycleAir 동작 중 시간초과 에러.");
    }
    PreStep.iCycle = Step.iCycle ;

    pPaix -> GetInputX(g_lInputXIo);
    //if(!OM.MstOptn.bUsedFrstMachine){
        //if(!g_lInputXIo[xETC_MaskPosSnsr]){  //JH if(!g_lInputXIo[xETC_MaskPosSnsr]){에서 수정
        //    m_bErr = true;
        //    FrmMsgOk->MsgOk("Error","상부 마스크 센서가 감지 되고 있습니다.");
        //    Trace("Error","상부 Mask 센서 감지 에러.");
        //    m_bReqStop = true;
        //    Step.iCycle = 0;
        //    return true;
        //}
    //}

    const int iAcc = 327680;
    const int iDcc = 3276;
    double dTiltCmdPos;
    dTiltCmdPos = pPaix -> GetCmdPos(miTiltMt);

    static double dLastTiltCmdPos = -1 ;

    static int iTiltStep = 0 ;
    if(Step.iCycle == 10) iTiltStep = 10 ;
    switch(iTiltStep) {
        default : iTiltStep = 0  ; break ;
        case 10 : //pPaix -> SetOverrideDSpeed(miTiltMt , pPaix -> GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));
                  if(g_NmcData.nBusy[miTiltMt]) pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed));
                  iTiltStep++;
                  break ;

        case 11 : if(g_NmcData.nBusy[miTiltMt]) break ;
                  if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                  else                                                    pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
                  break;
    }
    AnsiString sReturn;

    //Move Home.
    switch (Step.iCycle) {
        default: Step.iCycle = 0;
                 return true ;

        case 10: m_tmAir.Clear();
                 m_tmTemp.Clear();
                 pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iAirRPM);
                 sReturn = pPaix -> Log[miSpinMt].iReturn;
                 Trace(pPaix ->Log[miSpinMt].sFunction.c_str() , sReturn.c_str());
                 pPaix -> SetOutputXBit(yETC_IONAir     , true  );
                 pPaix -> SetOutputXBit(yETC_DrainValve , true  );
                 pPaix -> SetOutputXBit(yETC_PMPAir1  , false );
                 //WaterPump 관련된거 다 지움 진섭.
                 //if(!OM.CmnOptn.bUsedWaterPump) {
                 //    pPaix -> SetOutputXBit(yETC_BNRAir     , true  );
                 //    pPaix -> SetOutputXBit(yETC_BNRValve   , false );
                 //}
                 //else {
                 //    pPaix -> SetOutputXBit(yETC_BTMValve   , false );
                 //    //옵션추가.JS
                 //    if(OM.MstOptn.bUsedFrstMachine){
                 //        pPaix -> SetOutputXBit(yETC_PMPAir1  , false );
                 //    }
                 //    else {
                 //        pPaix -> SetOutputXBit(yETC_PMPAir1  , false );
                 //        pPaix -> SetOutputXBit(yETC_PMPAir2  , true  );
                 //    }
                 //}
                 pPaix -> SetOutputXBit(yETC_BNRAir     , true  );
                 pPaix -> SetOutputXBit(yETC_BNRValve   , false );
                 pPaix -> SetOutputXBit(yETC_BTMValve   , false );
                 m_tmDrain.Clear();
                 Trace("CycleAir",AnsiString(Step.iCycle).c_str());
                 Step.iCycle ++ ;
                 return false ;


        case 11: if(!m_tmDrain.OnDelay(true, OM.DevOptn.iWaterDrainTime))return false;
                 pPaix -> SetOutputXBit(yETC_BNRValve  , false);
                 pPaix -> SetOutputXBit(yETC_BNRAir    , false);
                 pPaix -> SetOutputXBit(yETC_BTMValve  , false);
                 pPaix -> SetOutputXBit(yETC_PMPAir1   , false);
                 //옵션 추가. JS
                 //if(!OM.MstOptn.bUsedFrstMachine){
                 //    pPaix -> SetOutputXBit(yETC_PMPAir1 , true );
                 //}
                 Trace("CycleAir",AnsiString(Step.iCycle).c_str());
                 m_tmTemp1.Clear();
                 Step.iCycle++;
                 return false;

        //SpinMotor가 안돌아 가는 현상떄문에 가라작업....JS
        case 12: if(!m_tmTemp1.OnDelay(true , 500)) return false;
                 Trace("CycleAir",AnsiString(Step.iCycle).c_str());
                 if(!g_NmcData.nBusy[miSpinMt]){
                     pPaix -> DecStop(miSpinMt);
                     Step.iCycle++;
                     return false;
                 }
                 else {
                     Step.iCycle = 20;
                     return false ;
                 }

        case 13: pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iCleanRPM);
                 Trace("CycleAir",AnsiString(Step.iCycle).c_str());
                 m_tmTemp1.Clear();
                 Step.iCycle = 12;
                 return false;

        case 20: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iAirTime)) return false;
                 pPaix -> SetOutputXBit(yETC_IONAir , false);
                 Trace("CycleAir",AnsiString(Step.iCycle).c_str());
                 Step.iCycle = 0;
                 return true;
    /*
    //Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iCycle) {
        default: Step.iCycle = 0;
                 return true ;

        case 10: m_tmAir.Clear();
                 m_tmTemp.Clear();
                 pPaix -> SetOutputXBit(yETC_IONAir , true );
                 Trace("CycleAir","10");
                 Step.iCycle ++ ;
                 return false ;

        case 11: pPaix -> MoveRPM(miSpinMt, OM.DevOptn.iAirRPM);
                 pPaix -> MoveRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed);
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(!OM.CmnOptn.bUsedWaterPump) pPaix -> SetOutputXBit(yETC_BNRAir , true );
                 else                           pPaix -> SetOutputXBit(yETC_PMPAir , true );
                 Trace("CycleAir","11");
                 Step.iCycle++;
                 return false;

        case 12: if(!OM.CmnOptn.bUsedWaterPump) pPaix -> SetOutputXBit(yETC_BNRValve , false);
                 else                           pPaix -> SetOutputXBit(yETC_PMPValve , false);
                 pPaix -> SetOutputXBit(yETC_DrainValve , true );
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 m_tmDrain.Clear();
                 Trace("CycleAir","12");
                 Step.iCycle ++;
                 return false;

        case 13: if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                 Trace("CycleAir","13");
                 Step.iCycle ++;
                 return false;

        case 14: if(m_tmTemp.OnDelay(true, OM.DevOptn.iAirTime)){
                     Trace("CycleAir","End");
                     pPaix -> SetOutputXBit(yETC_IONAir , false);
                     Step.iCycle = 0;
                     return true;
                 }
                 if(m_tmDrain.OnDelay(true, OM.DevOptn.iWaterDrainTime)) {
                     if(!OM.CmnOptn.bUsedWaterPump) pPaix -> SetOutputXBit(yETC_BNRValve , false);
                     else                           pPaix -> SetOutputXBit(yETC_PMPValve , false);

                     pPaix -> SetOutputXBit(yETC_DrainValve , true );
                 }
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(NmcData.nBusy[miTiltMt]) return false;
                 pPaix -> AbsMove(miTiltMt, OM.DevOptn.iTiltMtStopPs);
                 Trace("CycleAir","14");
                 Step.iCycle++;
                 return false;


        case 15: if(m_tmTemp.OnDelay(true, OM.DevOptn.iAirTime)){
                     Trace("CycleAir","End");
                     pPaix -> SetOutputXBit(yETC_IONAir , false);
                     Step.iCycle = 0;
                     return true;
                 }
                 if(m_tmDrain.OnDelay(true, OM.DevOptn.iWaterDrainTime)) {
                     if(!OM.CmnOptn.bUsedWaterPump) pPaix -> SetOutputXBit(yETC_BNRValve , false);
                     else                           pPaix -> SetOutputXBit(yETC_PMPValve , false);

                     pPaix -> SetOutputXBit(yETC_DrainValve , true );
                 }
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(NmcData.nBusy[miTiltMt]) return false;
                 pPaix -> AbsMove(miTiltMt, OM.DevOptn.iTiltMtStartPs);
                 Trace("CycleAir","15");
                 Step.iCycle++;
                 return false;

        case 16: if(m_tmTemp.OnDelay(true, OM.DevOptn.iAirTime)){
                     Trace("CycleAir","End");
                     pPaix -> SetOutputXBit(yETC_IONAir , false);
                     Step.iCycle = 0;
                     return true;
                 }
                 if(m_tmDrain.OnDelay(true, OM.DevOptn.iWaterDrainTime)) {
                     if(!OM.CmnOptn.bUsedWaterPump) pPaix -> SetOutputXBit(yETC_BNRValve , false);
                     else                           pPaix -> SetOutputXBit(yETC_PMPValve , false);

                     pPaix -> SetOutputXBit(yETC_DrainValve , true );
                 }
                 //if(!NmcData.nBusy[miSpinMt]) pPaix -> JogMove(miSpinMt, CW);
                 if(NmcData.nBusy[miTiltMt]) return false;
                 Trace("CycleAir","16");
                 Step.iCycle = 14;
                 return false;
    */
    }
}
//-------------------------------------------------------------------------
bool __fastcall CSEQ::CycleDry(void)
{
    if (m_tmCycle.OnDelay(Step.iCycle && !PreStep.iCycle == Step.iCycle , 10000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleDry 동작 중 시간초과 에러.");
        Trace("Error","CycleDry 동작 중 시간초과 에러.");
    }
    PreStep.iCycle = Step.iCycle ;

    pPaix -> GetInputX(g_lInputXIo);
    //if(!g_lInputXIo[xETC_MaskPosSnsr]){  //JH if(!g_lInputXIo[xETC_MaskPosSnsr]){에서 수정
    //    m_bErr = true;
    //    FrmMsgOk->MsgOk("Error","상부 마스크 센서가 감지 되고 있습니다.");
    //    Trace("Error","상부 Mask 센서 감지 에러.");
    //    m_bReqStop = true;
    //    Step.iCycle = 0;
    //    return true;
    //}
    AnsiString sReturn;

    double dTiltCmdPos ;
    int    iTiltCmdPos ;

    const int iAcc = 327680;
    const int iDcc = 3276;
    //Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iCycle) {
        default: Step.iCycle = 0;
                 return true ;

        case 10: m_tmDry.Clear();
                 m_tmTemp.Clear();
                 pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iDryRPM);
                 sReturn = pPaix -> Log[miSpinMt].iReturn;
                 Trace(pPaix ->Log[miSpinMt].sFunction.c_str() , sReturn.c_str());
                 pPaix -> DecStop(miTiltMt);
                 Trace("CycleDry",AnsiString(Step.iCycle).c_str());
                 Step.iCycle ++ ;
                 return false ;

        case 11: if(g_NmcData.nBusy[miTiltMt]) return false;
                 pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed));
                 pPaix -> AbsMove(miTiltMt, 0);
                 Trace("CycleDry",AnsiString(Step.iCycle).c_str());
                 Step.iCycle++;
                 return false;

        case 12: if(g_NmcData.nBusy[miTiltMt]) return false;
                 Trace("CycleDry",AnsiString(Step.iCycle).c_str());
                 Step.iCycle++;
                 return false;


        case 13: if(!m_tmTemp1.OnDelay(true , 500)) return false;
                 Trace("CycleDry",AnsiString(Step.iCycle).c_str());
                 if(!g_NmcData.nBusy[miSpinMt]){
                     pPaix -> DecStop(miSpinMt);
                     Step.iCycle++;
                     return false;
                 }
                 else {
                     Step.iCycle = 20;
                     return false ;
                 }

        case 14: pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iCleanRPM);
                 Trace("CycleDry",AnsiString(Step.iCycle).c_str());
                 m_tmTemp1.Clear();
                 Step.iCycle = 13;
                 return false;


        case 20: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iDryTime))return false;
                 pPaix -> DecStop(miSpinMt);
                 m_bWorkEnd = true;
                 Trace("CycleDry",AnsiString(Step.iCycle).c_str());
                 Step.iCycle = 0;
                 return true;
    }
}
//---------------------------------------------------------------------------
void __fastcall CSEQ::Update(void)
{

    Flick ();
    UpdateButton();
    //if(pPaix -> m_bConnect && !pPaix -> m_bNotConnect) ReConnect();
}

bool __fastcall CSEQ::MotrAlrmReset(void)
{
    pPaix -> SetAlarmResetOn(miSpinMt , true);
    pPaix -> SetAlarmResetOn(miTiltMt , true);

    ::Sleep(10);

    pPaix -> SetAlarmResetOn(miSpinMt , false);
    pPaix -> SetAlarmResetOn(miTiltMt , false);

}

bool __fastcall CSEQ::FlickErr(void)
{
    if(m_bErr && !m_tmWorkErr.OnDelay(true, 5000)){
        if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 1000)) m_bFlick = false; }
        else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 1000)) m_bFlick = true ; }
    }
    else {
        m_bErr = false;
        m_bFlick = false;
        m_tmWorkErr.Clear();
    }
}

bool __fastcall CSEQ::FlickWorkEnd(void)
{
    if(m_bWorkEnd && m_bWorkfinish && !m_tmWorkEnd.OnDelay(true, 1500)){
        if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 1000))m_bFlick = false; }
        else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 1000))m_bFlick = true ; }
    }
    else{
        //m_bWorkEnd    = false;
        m_bWorkfinish = false;
        m_bFlick = false;
        m_tmWorkEnd.Clear();
    }
}

void __fastcall CSEQ::Flick()
{
    if(m_bErr)             FlickErr();
    else if(m_bWorkfinish) FlickWorkEnd();
    else {
        m_tmWorkEnd.Clear();
        m_tmWorkErr.Clear();
    }

    if(m_bFlick) pPaix -> SetOutputXBit(yETC_TwBzz , true  );
    else         pPaix -> SetOutputXBit(yETC_TwBzz , false );
}

void __fastcall CSEQ::ReConnect(void)
{
    pPaix -> SetAlarmResetOn(miSpinMt, true);
    pPaix -> SetAlarmResetOn(miTiltMt, true);

    pPaix -> SetAlarmResetOn(miSpinMt, false);
    pPaix -> SetAlarmResetOn(miTiltMt, false);
}

