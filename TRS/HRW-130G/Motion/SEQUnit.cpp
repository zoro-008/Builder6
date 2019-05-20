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

    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmCycle    .Clear();
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

    Trace("Sequence","Reset");

}
void CSEQ::Init()
{
    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmCycle    .Clear();
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

    pPaix -> SetThreadCallBack(Update);
    
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

    static bool bPreStartSw = false;
    static bool bPreStopSw  = false;
    static bool bPreResetSw = false;

    bool bSwEgStart = false;
    bool bSwEgStop  = false;
    bool bSwEgReset = false;


    if(bStartSw&&!bPreStartSw){bSwEgStart= true; Trace("Button","Start 버튼을 눌렀습니다.");}
    if(bStopSw &&!bPreStopSw ){bSwEgStop = true; Trace("Button","Stop  버튼을 눌렀습니다.");}
    if(bResetSw&&!bPreResetSw){bSwEgReset= true; Trace("Button","Reset 버튼을 눌렀습니다.");}

    bPreStartSw = bStartSw ;
    bPreStopSw  = bStopSw  ;
    bPreResetSw = bResetSw ;


    //Buzzer Off.
    //if (isErr && bStopSw) TL_MuteSnd();

    //Set Condition Flags.
    bool isStopCon  = bSwEgStop /*bStopSw */ && m_bRun  ;
    bool isRunCon   = bSwEgStart/*bStartSw*/ ;
    bool isResetCon = bSwEgReset/*bResetSw*/ && !m_bRun ;

    if(m_bRun){
        pPaix -> SetOutputXBit(yETC_StartLp, true );
        pPaix -> SetOutputXBit(yETC_StopLp , false);
    }
    else {
        pPaix -> SetOutputXBit(yETC_StartLp, false);
        pPaix -> SetOutputXBit(yETC_StopLp , true );
    }

    //Run.
    if (isRunCon   && !m_iStep  ){ m_iStep = scToStartCon; }
    if (isStopCon               ){ m_bReqStop = true ;     }
    if (isResetCon && !m_iStep  ) Reset() ;

    if (m_tmToStrt.OnDelay(m_iStep == scToStartCon || m_iStep == scToStart , 15000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","Start 동작중 시간초과 에러.");
        //Trace("Error","Start 동작중 시간초과 에러.");
        Trace("Error",("Start 동작 중 시간초과 에러. ToStartStep=" + String(Step.iToStart)).c_str());
        m_iStep = scIdle;
        m_bRun  = false;
    }

    //Time Out.
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , 10000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","Stop 동작중 시간초과 에러.");
        //Trace("Error","Stop 동작중 시간초과 에러.");\
        Trace("Error",("Stop 동작 중 시간초과 에러. ToStopStep=" + String(Step.iToStop)).c_str());
        m_bRun = false ;
        m_iStep    = scIdle;
        m_bReqStop = false;
    }

    //Running Step.
    switch (m_iStep) {
        case scIdle       : return;

        case scToStartCon : m_bRun = true;
                            m_iStep= scToStart;
                            Trace("ToStart","Start");
                            Step.iToStart = 10;
                            return ;

        case scToStart    : if(!m_bReqStop) {
                                if(!ToStart()) return ;
                                m_iStep=scCycleClean ;
                                Trace("CycleClean","Start");
                                Step.iCycle = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            m_bWorkfinish = true;
                            m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            return ;

        case scCycleClean : if(!m_bReqStop) {
                                if(!CycleClean()) return ;
                                Trace("CycleClean","End");
                                Trace("CycleAir","Start");
                                m_iStep=scCycleAIR;
                                Step.iCycle = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            m_bWorkfinish = true;
                            m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            return ;

        case scCycleAIR   : if(!m_bReqStop) {
                                if(!CycleAir()) return ;
                                m_iStep=scCycleDRY;
                                Trace("CycleAir","End");
                                Trace("CycleDry","Start");
                                Step.iCycle = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_bWorkfinish = true;
                            m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            m_iStep=scToStopCon ;
                            return ;

        case scCycleDRY   : if(!m_bReqStop) {
                                if(!CycleDry()) return ;
                                m_iStep=scToStopCon;
                                Trace("CycleDry","End");
                                Trace("ToStopCon","Start");
                                //Step.iToStop = 10;
                                return ;
                            }
                            m_bReqStop = false ;
                            m_bWorkfinish = true;
                            m_bWorkEnd    = true;
                            FrmMsgOk->MsgOk("Warring","동작 중 문제가 발생하였습니다.");
                            Trace("Warring","동작 중 문제가 발생하였습니다.");
                            m_iStep=scToStopCon ;
                            return ;

        case scToStopCon :  m_iStep = scToStop;
                            Trace("ToStop","Start");
                            Step.iToStop = 10;
                            m_bRun = false;
                            return ;


        case scToStop    :  if (!ToStop()) return ;
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
        Trace("Error",("Start 동작 중 시간초과 에러. ToStartStep=" + String(Step.iToStart)).c_str());
    }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(__FUNC__,sTemp.c_str());
    }
    PreStep.iToStart = Step.iToStart ;


    bool bHomeDone;
    //Stat.bReqStop = false ;

    AnsiString sReturn;
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
                 Step.iToStart++;
                 return false;

        case 11: pPaix -> SetOutputXBit(yETC_Vaccum, true);
                 m_tmTemp.Clear();
                 Step.iToStart++;
                 return false;

        case 12: if(!m_tmTemp.OnDelay(true, 3000))return false;
                 pPaix -> GetInputX(g_lInputXIo);
                 if(!g_lInputXIo[xETC_Vaccum]){
                     m_bReqStop = true;
                     m_bErr = true;
                     FrmMsgOk->MsgOk("Error","동작중 자재 없음 에러.");
                     Trace("Error","동작중 자재 없음 에러.");
                     Step.iToStart = 0;
                     return true;
                 }
                 //m_bVaccumErr = false;
                 pPaix -> SetServoOn(miSpinMt , true);
                 pPaix -> SetOutputXBit(yETC_DoorFw, true );
                 pPaix -> SetOutputXBit(yETC_DoorBw, false);
                 m_tmTemp.Clear();
                 Step.iToStart ++ ;
                 return false ;

        case 13: pPaix -> GetInputX(g_lInputXIo);
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
                 Step.iToStart++;
                 return false;

        case 14: //pPaix -> MoveRPM(miSpinMt , OM.DevOptn.iCleanRPM     );
                 //sReturn = pPaix -> Log[miSpinMt].iReturn;
                 //Trace(pPaix ->Log[miSpinMt].sFunction.c_str() , sReturn.c_str());
                 //pPaix -> MoveRPM(miTiltMt , OM.DevOptn.iTiltMtSpeed  );
                 //sReturn = pPaix ->Log[miTiltMt].iReturn;
                 //Trace(pPaix ->Log[miTiltMt].sFunction.c_str() , sReturn.c_str());
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
        Trace("Error",("ToStop 동작 중 시간초과 에러. ToStopStep=" + String(Step.iToStop)).c_str());
        //Trace("Error",("Start 동작 중 시간초과 에러. ToStartStep=" + String(Step.iToStart)).c_str());
    }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(__FUNC__,sTemp.c_str());
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
                 Step.iToStop ++ ;
                 return false ;

        case 11: //20141014 JS
                 if(g_NmcData.nBusy[miSpinMt]) return false;     //움직이는 도중 Busy꺼짐..JS
                 if(g_NmcData.nBusy[miTiltMt]) return false;
                 //if(!g_NmcData.nInpo[miSpinMt]) return false;       
                 //if(!g_NmcData.nInpo[miTiltMt]) return false;
                 Step.iToStop++;
                 return false;

        case 12: pPaix -> SetOutputXBit(yETC_AirBlower1  , false);
                 pPaix -> SetOutputXBit(yETC_AirBlower2  , false);
                 pPaix -> SetOutputXBit(yETC_SupplyValve , false);
                 pPaix -> SetOutputXBit(yETC_Drainage    , false);





                 Step.iToStop++;
                 return false;

        case 13: if(g_NmcData.nBusy[miTiltMt]) return false;
                 //pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                 pPaix -> AbsMove(miTiltMt, 0);
                 Step.iToStop++;
                 return false;

        case 14: if(g_NmcData.nBusy[miTiltMt]) return false;

                 m_tmTemp.Clear();
                 Step.iToStop++;
                 return false ;

        case 15: if(!m_tmTemp.OnDelay(true , 2000)) return false ;
                 pPaix -> SetServoOn(miSpinMt, false);
                 pPaix -> SetOutputXBit(yETC_Vaccum      , false);

                 m_tmTemp.Clear();
                 Step.iToStop++;
                 return false ;

        case 16: if(!m_tmTemp.OnDelay(true , 2000)) return false ;


                 //pPaix -> GetInputX(g_lInputXIo); 이건 왠 찐따.
                 pPaix -> SetOutputXBit(yETC_DoorFw, false);
                 pPaix -> SetOutputXBit(yETC_DoorBw, true );
                 Step.iToStop++;
                 return false;

        case 17: if(!g_lInputXIo[xETC_DoorBw]) return false;

                 m_bWorkfinish = true;
                 Step.iToStop++;
                 return false;

        case 18: pPaix -> SetCmdPos(miSpinMt, 0);
                 pPaix -> SetEncPos(miSpinMt, 0);
                 pPaix -> AbsMove(miTiltMt , 0);
                 Trace("Tostop","ToStop End");
                 Step.iToStop = 0   ;
                 return true ;

/*
                 pPaix -> GetInputX(g_lInputXIo);
                 pPaix -> SetOutputXBit(yETC_DoorFw, false);
                 pPaix -> SetOutputXBit(yETC_DoorBw, true );
                 Step.iToStop++;
                 return false;

        case 15: if(!g_lInputXIo[xETC_DoorBw]) return false;
                 pPaix -> SetServoOn(miSpinMt, false);
                 pPaix -> SetOutputXBit(yETC_Vaccum      , false);
                 m_bWorkfinish = true;
                 Step.iToStop++;
                 return false;

        case 16: pPaix -> SetCmdPos(miSpinMt, 0);
                 pPaix -> SetEncPos(miSpinMt, 0);
                 pPaix -> AbsMove(miTiltMt , 0);
                 Trace("Tostop","ToStop End");
                 Step.iToStop = 0   ;
                 return true ;
*/
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

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(__FUNC__,sTemp.c_str());
    }
    PreStep.iCycle = Step.iCycle ;

    pPaix -> GetInputX(g_lInputXIo);
    if(!g_lInputXIo[xETC_Vaccum]){
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleClean 동작중 자재 없음 에러.");
        Trace("Error","CycleClean 동작중 자재 없음 에러.");
        m_bReqStop = true;
        Step.iCycle = 0;
        return true;
    }


    const int iAcc = 327680;
    const int iDcc = 327680;
    double dTiltCmdPos;
    if(!g_NmcData.nBusy[miTiltMt]) {
        pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
        if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) {
            pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
            Trace("CycleClean Tilt","Go To TiltMtStartPs");
        }
        else {
            pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
            Trace("CycleClean Tilt","Go To TiltMtStopPs");
        }
    }






    /*dTiltCmdPos = pPaix -> GetCmdPos(miTiltMt);

    static double dLastTiltCmdPos = -1 ;

    static int iTiltStep = 0 ;
    if(Step.iCycle == 10) iTiltStep = 10 ;
    switch(iTiltStep) {
        default : iTiltStep = 0  ; break ;
        case 10 : //pPaix -> SetOverrideDSpeed(miTiltMt , pPaix -> GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));
                  if(g_NmcData.nBusy[miTiltMt]) {
                      pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
                      Trace("CtckeCleanTilt","g_NmcData.nBusy[miTiltMt] is true SetSpeed");
                  }

                  iTiltStep++;       //여기 속도 전환 하는데 비지가 살아있을때 한다. 확인해보기....
                  break ;

        case 11 : if(g_NmcData.nBusy[miTiltMt]) break ;
                  if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) {
                      pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                      Trace("CycleClean Tilt","Go To TiltMtStartPs");
                  }
                  else {
                      pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
                      Trace("CycleClean Tilt","Go To TiltMtStopPs");
                  }
                  break;
    }     */

    /*
    if(g_NmcData.nInpo[miTiltMt]){
        if(OM.DevOptn.iTiltMtStartPs - 1 <= dTiltCmdPos && dTiltCmdPos <= OM.DevOptn.iTiltMtStartPs + 1) { //스타트 포지션에 있을때는
            pPaix -> SetSpeed(miTiltMt, 1, iAccDcc, iAccDcc, pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
            if(dLastTiltCmdPos != OM.DevOptn.iTiltMtStopPs) {
                pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs);
                dLastTiltCmdPos = OM.DevOptn.iTiltMtStopPs ;
            }
            Trace("TiltMove","MoveTo iTiltMtStopPs");
        }
        else{
        //if(OM.DevOptn.iTiltMtStopPs - 1 <= dTiltCmdPos && dTiltCmdPos <= OM.DevOptn.iTiltMtStopPs + 1) { //스타트 포지션에 있을때는
            pPaix -> SetSpeed(miTiltMt, 1, iAccDcc, iAccDcc, pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
            if(dLastTiltCmdPos != OM.DevOptn.iTiltMtStartPs) {
                pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                dLastTiltCmdPos = OM.DevOptn.iTiltMtStartPs ;
            }
            Trace("TiltMove","MoveTo iTiltMtStopPs");
        }
    }*/

    AnsiString sReturn;

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
                 Step.iCycle++;
                 return false;

        case 11: pPaix -> SetOutputXBit(yETC_AirBlower1, true);
                 pPaix -> SetOutputXBit(yETC_SupplyValve, true);
                 Step.iCycle ++;
                 return false;

        case 12: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime))return false;
                 Step.iCycle=0;
                 return true;
    }

    /*
    case 10: pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iCleanRPM);
                 sReturn = pPaix -> Log[miSpinMt].iReturn;
                 Trace(pPaix ->Log[miSpinMt].sFunction.c_str() , sReturn.c_str());
                 pPaix -> MoveRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed  );
                 sReturn = pPaix ->Log[miTiltMt].iReturn;
                 Trace(pPaix ->Log[miTiltMt].sFunction.c_str() , sReturn.c_str());
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle++;
                 return false;

        case 11: m_tmClean.Clear();
                 m_tmTemp .Clear();
                 m_tmTemp1.Clear();
                 pPaix -> IncMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                 sReturn = pPaix ->Log[miTiltMt].iReturn;
                 Trace(pPaix ->Log[miTiltMt].sFunction.c_str() , sReturn.c_str());
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle ++ ;
                 return false ;

        case 12: dTiltCmdPos = NmcData.dCmd[miTiltMt];
                 iTiltCmdPos = (dTiltCmdPos+1) / 910 ;
                 sTiltCmdPos = iTiltCmdPos;
                 if(iTiltCmdPos < OM.DevOptn.iTiltMtStartPs - 5)return false;
                 pPaix -> JogMove(miSpinMt, CW);
                 m_tmTemp.Clear();
                 //pPaix -> SetOutputXBit(yETC_Vaccum, true);
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle ++;
                 return false;

        case 13: pPaix -> SetOutputXBit(yETC_AirBlower1, true);
                 pPaix -> SetOutputXBit(yETC_SupplyValve, true);
                 //pPaix -> JogMove(miSpinMt, CW);
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle ++;
                 return false;

        case 14: if(m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime)){
                     Trace("CycleClean","End");
                     Step.iCycle = 0;
                     return true;
                 }
                 if(NmcData.nBusy[miTiltMt]) return false;
                 if(!NmcData.nBusy[miSpinMt]) Trace("CycleClean", "Spin Motor not Move");
                 pPaix -> AbsMove(miTiltMt, OM.DevOptn.iTiltMtStopPs);
                 pPaix -> JogMove(miSpinMt, CW);
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle++;
                 return false;

        case 14: if(m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime)){
                     Trace("CycleClean","End");
                     Step.iCycle = 0;
                     return true;
                 }
                 if(NmcData.nBusy[miTiltMt]) return false;
                 if(!NmcData.nBusy[miSpinMt]) Trace("CycleClean", "Spin Motor not Move");
                 pPaix -> AbsMove(miTiltMt, OM.DevOptn.iTiltMtStartPs);
                 pPaix -> JogMove(miSpinMt, CW);
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle++;
                 return false;

        case 15: if(m_tmTemp.OnDelay(true, OM.DevOptn.iCleanTime)){
                     Trace("CycleClean","End");
                     Step.iCycle = 0;
                     return true;
                 }
                 if(NmcData.nBusy[miTiltMt]) return false;
                 if(!NmcData.nBusy[miSpinMt]) Trace("CycleClean", "Spin Motor not Move");
                 pPaix -> JogMove(miSpinMt, CW);
                 Trace("CycleClean",AnsiString(Step.iCycle).c_str());
                 Step.iCycle = 13;
                 return false;
    */
}
//-------------------------------------------------------------------------
bool __fastcall CSEQ::CycleAir(void)
{
    if (m_tmCycle.OnDelay(Step.iCycle && !PreStep.iCycle == Step.iCycle , 10000)) {
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleAir 동작 중 시간초과 에러.");
        Trace("Error","CycleAir 동작 중 시간초과 에러.");
    }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(__FUNC__,sTemp.c_str());
    }
    PreStep.iCycle = Step.iCycle ;

    pPaix -> GetInputX(g_lInputXIo);
    if(!g_lInputXIo[xETC_Vaccum]){
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleAir 동작중 자재 없음 에러.");
        Trace("Error","CycleAir 동작중 자재 없음 에러.");
        m_bReqStop = true;
        Step.iCycle = 0;
        return true;
    }

    //Stat.bReqStop = false ;
    /*
    const int iAcc = 327680;
    const int iDcc = 3276;       //20141223 선계원 Busy 씨그널 펄스 끝에 떨어지는지 아니면 인포지션 되어 떨어지는지지 설정 옵션이 있는데 확인 해봐야 함...
    double dTiltCmdPos;
    dTiltCmdPos = pPaix -> GetCmdPos(miTiltMt);

    static double dLastTiltCmdPos = -1 ;

    static int iTiltStep = 0 ;
    if(Step.iCycle == 10) iTiltStep = 10 ;
    switch(iTiltStep) {
        default : iTiltStep = 0  ; break ;
        case 10 : //pPaix -> SetOverrideDSpeed(miTiltMt , pPaix -> GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));
                  if(g_NmcData.nBusy[miTiltMt]) {
                      pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
                      Trace("CtckeCleanTilt","g_NmcData.nBusy[miTiltMt] is true SetSpeed");
                  }
                  iTiltStep++;    //여기 속도 전환 하는데 비지가 살아있을때 한다. 확인해보기....
                  break ;

        case 11 : if(g_NmcData.nBusy[miTiltMt]) break ;
                  //if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                  //else                                                    pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
                  if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) {
                      pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
                      Trace("CycleClean Tilt","Go To TiltMtStartPs");
                  }
                  else {
                      pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
                      Trace("CycleClean Tilt","Go To TiltMtStopPs");
                  }
                  break;
    }*/

    //비지시그널 설정이 원인인듯 한데..
    const int iAcc = 327680;
    const int iDcc = 327680;
    double dTiltCmdPos;
    if(!g_NmcData.nBusy[miTiltMt]) {
        pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed));
        if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) {
            pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs);
            Trace("CycleAir Tilt","Go To TiltMtStartPs");
        }
        else {
            pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs );
            Trace("CycleAir Tilt","Go To TiltMtStopPs");
        }
    }


    //Move Home.
    switch (Step.iCycle) {
        default: Step.iCycle = 0;
                 return true ;

        case 10: m_tmAir.Clear();
                 m_tmDrain.Clear();
                 m_tmTemp.Clear();
                 pPaix -> MoveJogRPM(miSpinMt , OM.DevOptn.iAirRPM);
                 pPaix -> SetOutputXBit(yETC_AirBlower2 , true );
                 pPaix -> SetOutputXBit(yETC_AirBlower1 , true );
                 pPaix -> SetOutputXBit(yETC_SupplyValve, false);
                 pPaix -> SetOutputXBit(yETC_Drainage   , true );
                 Step.iCycle ++ ;
                 return false ;


        case 11: if(m_tmDrain.OnDelay(true, OM.DevOptn.iWaterDrainTime)) {
                     //pPaix -> SetOutputXBit(yETC_AirBlower1 , false);
                     pPaix -> SetOutputXBit(yETC_Drainage   , false);
                 }
                 if(!m_tmTemp.OnDelay(true, OM.DevOptn.iAirTime)) return false;
                 pPaix -> SetOutputXBit(yETC_AirBlower1 , false);
                 pPaix -> SetOutputXBit(yETC_AirBlower2 , false);

                 pPaix -> DecStop(miTiltMt);

                 Step.iCycle = 0;
                 return true;
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

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(__FUNC__,sTemp.c_str());
    }
    PreStep.iCycle = Step.iCycle ;

    pPaix -> GetInputX(g_lInputXIo);
    if(!g_lInputXIo[xETC_Vaccum]){
        m_bErr = true;
        FrmMsgOk->MsgOk("Error","CycleDry 동작중 자재 없음 에러.");
        Trace("Error", "CycleDry 동작중 자재 없음 에러.");
        m_bReqStop = true;
        Step.iCycle = 0;
        return true;
    }

    double dTiltCmdPos ;
    int    iTiltCmdPos ;

    //Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iCycle) {
        default: Step.iCycle = 0;
                 return true ;

        case 10: m_tmDry.Clear();
                 m_tmTemp.Clear();
                 pPaix -> DecStop(miTiltMt);
                 Step.iCycle ++ ;
                 return false ;

        case 11: pPaix -> MoveJogRPM(miSpinMt, OM.DevOptn.iDryRPM);
                 Step.iCycle++;
                 return false;

        case 12: if(g_NmcData.nBusy[miTiltMt])return false;
                 pPaix -> AbsMove(miTiltMt, 0);
                 Step.iCycle++;
                 return false;

        case 13: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iDryTime))return false;
                 pPaix -> DecStop(miSpinMt);
                 m_bWorkEnd = true;
                 Step.iCycle = 0;
                 return true;
    }
}
//---------------------------------------------------------------------------
void CSEQ::Update(void)
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
        if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 4000)) m_bFlick = false; }
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
    if(m_bWorkEnd && m_bWorkfinish && !m_tmWorkEnd.OnDelay(true, 5000)){
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

    if(m_bFlick) pPaix -> SetOutputXBit(yETC_Buzz , true  );
    else         pPaix -> SetOutputXBit(yETC_Buzz , false );
}

void __fastcall CSEQ::ReConnect(void)
{
    pPaix -> SetAlarmResetOn(miSpinMt, true);
    pPaix -> SetAlarmResetOn(miTiltMt, true);

    pPaix -> SetAlarmResetOn(miSpinMt, false);
    pPaix -> SetAlarmResetOn(miTiltMt, false);
}
