//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"

#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#include "PreBuff.h"
#include "PstBuff.h"
#include "Unloader.h"

#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
//#include "AdlinkTenKey.h"
#include "SPCUnit.h"
#include "FrmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CSequence SEQ;

/***************************************************************************/
/* 생성자 & 소멸자. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall CSequence::CSequence(void)
{
    Init();
}

//---------------------------------------------------------------------------
__fastcall CSequence::~CSequence(void)
{
    //
}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall CSequence::Init(void)
{
    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmFlickOn  .Clear();
    m_tmFlickOff .Clear();
    m_tmTemp     .Clear();
    m_tmCloseDoor.Clear();

    //Init. Var.
    m_bBtnReset  = false ;
    m_bBtnStart  = false ;
    m_bBtnStop   = false ;
    m_bBtnAir    = false ;

    m_bRun       = false ;
    m_bAutoMode  = false  ;
    m_bFlick     = false ;
    m_iStep      = scIdle;
    m_iSeqStat   = ssStop;

//    if(!OM.MstOptn.bDebugMode) IO_SetY(yETC_MainSol , true);
    IO_SetY(yETC_MainSol , true);

}

//---------------------------------------------------------------------------
void __fastcall CSequence::Reset(void)
{
    //Check running flag.
    if (m_bRun              ) return;
    if (m_iSeqStat == ssInit) return;

    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmFlickOn  .Clear();
    m_tmFlickOff .Clear();
    m_tmTemp     .Clear();
    m_tmCloseDoor.Clear();

    //Init. Var.
    m_bBtnReset  = false ;
    m_bBtnStart  = false ;
    m_bBtnStop   = false ;

    m_bRun       = false ;
    m_iStep      = scIdle;


    //System.
    MT_ResetAll();
    AT_Reset   ();

    //Error.
    EM_Clear();

    //Manual.
    MM.Reset();

    //Part.
    LDR.Reset();
    PRB.Reset();
    RAL.Reset();
    HED.Reset();
    PSB.Reset();
    ULD.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

//    if(!OM.MstOptn.bDebugMode) MT_SetServoAll(true); // dd
    MT_SetServoAll(true); // dd

    //랏 엔드시에 리셑해야 부져 꺼지게.
    LT.m_bLotEnd = false ; //TODO :: 이해 안감.dd

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bDoorSkip ) InspectDoor() ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw   = IO_GetXUp(xETC_LStartSw) || IO_GetXUp(xETC_RStartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw    = IO_GetXUp(xETC_LStopSw ) || IO_GetXUp(xETC_RStopSw ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw   = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bResetHdSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) ;
    bool bResetSfSw =                                                         m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw     = IO_GetXUp(xETC_LMainAir) || IO_GetXUp(xETC_RMainAir) || m_bBtnAir   ;
    bool bInitSw    = IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;
    bool bVisnSw    = IO_GetXUp(xETC_LVisnSw ) || IO_GetXUp(xETC_RVisnSw );

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw ) ||  m_bRun);
    IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);
    IO_SetY(yETC_LStopLp   , IO_GetX(xETC_LStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_RStopLp   , IO_GetX(xETC_RStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LResetLp  , IO_GetX(xETC_LResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_RResetLp  , IO_GetX(xETC_RResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_LAirLp    , IO_GetX(xETC_LMainAir ) || IO_GetY(yETC_MainSol));
    IO_SetY(yETC_RAirLp    , IO_GetX(xETC_RMainAir ) || IO_GetY(yETC_MainSol));
    IO_SetY(yETC_LInitLp   , IO_GetX(xETC_LInitSw  ));
    IO_SetY(yETC_RInitLp   , IO_GetX(xETC_RInitSw  ));

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;




    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if(bStartSw) {
        //리뷰 엔드 상황에
        if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please. " ) ; return ;}
        //if(!DM.ARAY[riVS4].CheckAllStat(csNone)) DM.ARAY[riVS4].SetStat(csWork) ;

        if(!InspectMotorServo()) {FM_MsgOk("ERR","Initial First!!"); return ;}
//        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!"); return ;}
        if(!MT_GetHomeEnd(miLDR_YCmp ) ||
           !MT_GetHomeEnd(miLDR_ZCmp ) ||
           !MT_GetHomeEnd(miWRK_XVsn ) ||
           !MT_GetHomeEnd(miWRK_YVsn ) ||
           !MT_GetHomeEnd(miWR1_XCvr ) ||
           !MT_GetHomeEnd(miWR2_XCvr ) ||
           !MT_GetHomeEnd(miWR3_XCvr ) ||
           !MT_GetHomeEnd(miPSB_XTrm ) ||
           !MT_GetHomeEnd(miPSB_YTrm ) ||
           !MT_GetHomeEnd(miPSB_TFlp ) ||
           !MT_GetHomeEnd(miULD_YCmp ) ||
           !MT_GetHomeEnd(miULD_ZCmp )) { FM_MsgOk("ERR","Initial First!!"); return ;}

        if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        ULD.Reset();
    }

    if(bStopSw) {
        MM.m_bStop = true ;
        MM.m_bStopTestMrk = true ;

    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);

    }
    if (bVisnSw) MM.SetManCycle(mcETC_Sampling1Cycle );
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
    }

    //Err Log
    static bIsErr;
    isErr = EM_IsErr() ;
    if(isErr && !bIsErr) {
        bIsErr = true ;
        SPC.WriteErrData(EM_GetLastErr());
    }
    if(!isErr && bIsErr) {
        bIsErr = false ;
        SPC.WriteErrData(EM_GetLastErr(),true);
    }

    //Buzzer Off.
    //2012 11 28 선계원 루셈요청에 의해 에러 떳을때 리셑버튼이 소리 죽이고
    //화면에서 리셑을 눌러야만 리셑되게 바꿈.
    //1200단독장비에서 로더에 손이 껴서 안전사고 발생한 것 때문에.
    if(isErr && bStopSw   ) TL_MuteSnd();
    if(isErr && bResetHdSw) TL_MuteSnd();

    //Set Condition Flags.
    bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcNone ;
    //bool isResetCon = bResetSw && !m_bRun ;
    bool isResetCon = bResetSfSw && !m_bRun ;

    //Run.
    if (isRunCon && !m_iStep) {
        m_iStep = scToStartCon ;
         //Init. Updated Flag.
         EM_SetDisp(true);
//         if ((FM_GetLevel() != lvOperator) && isAllCloseDoor) {
              //OM.CmnOptn.bIgnrDoor = true ;
              //OM.SaveCmnOptn();
//              FM_SetLevel(lvOperator) ;
//         }
    }

    if (isStopCon  &&  m_iStep) m_bReqStop = true ;
    if (isResetCon && !m_iStep) Reset()           ;

    if (m_tmToStrt.OnDelay(m_iStep == scToStartCon || m_iStep == scToStart , 10000)) {
        //Trace Log.
        AnsiString Msg ;
        Msg.sprintf("ToStrtTimeOut : m_iStep=%d" ,m_iStep );
        Trace  ("SEQ",Msg.c_str() );
        EM_SetErr (eiETC_ToStartTO);
        m_iStep = scIdle;
        m_bRun  = false;
    }

    //Time Out.
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , 20000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);

        IO_SetY(yLDR_MgzInAC  , false) ;
        IO_SetY(yLDR_MgzOutAC , false) ;

        IO_SetY(yPRB_FeedingAC , false) ;
        IO_SetY(yWRK_FeedingAC , false) ;

        IO_SetY(yULD_MgzInAC  , false) ;
        IO_SetY(yULD_MgzOutAC , false) ;

        m_iStep    = scIdle;
        m_bReqStop = false;
    }

    //Running Step.
    switch (m_iStep) {
        case scIdle       : return;

        case scToStartCon : if(!ToStartCon()) return ;
                            m_iStep=scToStart;
                            return ;

        case scToStart    : if(!ToStart()) return ;
                            m_bRun = true ;
                            m_iStep=scRun ;
                            return ;

        case scRun        : if(!m_bReqStop) {
                                if(AutoRun()) {
                                    //랏엔드 상황. 이상혁 사장님 같은거 하나 띄워야 될수도 여기에...
                                    LT.LotEnd();
                                    m_iStep=scToStopCon ;
                                }
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            return ;

        case scToStopCon :  if(!ToStopCon()) {
                                if(AutoRun()) {
                                    //랏엔드 상황. 이상혁 사장님 같은거 하나 띄워야 될수도 여기에...
                                    LT.LotEnd();
                                }
                                return ;
                            }
                            m_bRun = false ;
                            m_iStep=scToStop;
                            return ;

        case scToStop    :  if (!ToStop()) return ;
                            m_iStep = scIdle ;
                            m_bReqStop = false ;

                            DM.SaveMap();
                            LT.SaveLotInfo();

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{
    bool isOk = true ;

    if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol))  isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;                                                                         
}
//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainPower(void)
{
    bool isOk = true ;

    if(!IO_GetX(xETC_PowerOn))  isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainPower) ;


    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetXUp(xETC_EmgSw1) || IO_GetXUp(xETC_EmgSw2) || IO_GetXUp(xETC_LEmgLDRSw) ) { //|| IO_GetX(xULD_Emg)) {
        EM_SetErr(eiETC_Emergency);
        MT_SetServoAll(false);
        
        IO_SetY(yLDR_ZBreak,true);
        IO_SetY(yULD_ZBreak,true);
        isOk = false ;
    }

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectLightGrid(void)
{
    bool isOk = true ;
    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectDoor(void)
{
    //Local Var.
    bool isOk = true;

    //Check Door
    if (!IO_GetX(xETC_FDoor) ){EM_SetErr(eiETC_FDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_BDoor) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(AT_Err(aiLDR_ClmpUpDn   )) {EM_SetErr(eiLDR_ClmpUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiLDR_PshrFwBw   )) {EM_SetErr(eiLDR_PshrFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiPRB_StprUpDn   )) {EM_SetErr(eiPRB_StprUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiWK1_StprUpDn   )) {EM_SetErr(eiWK1_StprUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiWK1_AlgnFwBw   )) {EM_SetErr(eiWK1_AlgnFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiWK1_SttnUpDn   )) {EM_SetErr(eiWK1_SttnUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiWK2_StprUpDn   )) {EM_SetErr(eiWK2_StprUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiWK2_AlgnFwBw   )) {EM_SetErr(eiWK2_AlgnFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiWK2_SttnUpDn   )) {EM_SetErr(eiWK2_SttnUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiWK3_StprUpDn   )) {EM_SetErr(eiWK3_StprUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiWK3_AlgnFwBw   )) {EM_SetErr(eiWK3_AlgnFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiWK3_SttnUpDn   )) {EM_SetErr(eiWK3_SttnUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_AlgnFwBw   )) {EM_SetErr(eiPSB_AlgnFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_IndxUpDn   )) {EM_SetErr(eiPSB_IndxUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_FlprOpCl   )) {EM_SetErr(eiPSB_FlprOpClTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_GripUpDn   )) {EM_SetErr(eiPSB_GripUpDnTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_PnchDnUp   )) {EM_SetErr(eiPSB_PnchDnUpTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_SortFwBw   )) {EM_SetErr(eiPSB_SortFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_SrtStprFwBw)) {EM_SetErr(eiPSB_SrtStprFwBwTO) ; isOk = false ; }
    if(AT_Err(aiPSB_PshrFwBw   )) {EM_SetErr(eiPSB_PshrFwBwTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_PshrDnUp   )) {EM_SetErr(eiPSB_PshrDnUpTO   ) ; isOk = false ; }
    if(AT_Err(aiULD_ClmpUpDn   )) {EM_SetErr(eiULD_ClmpUpDnTO   ) ; isOk = false ; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotorServo(void)
{
    bool isOk = true ;
    if(!MT_GetServo(miWRK_XVsn )) isOk = false ;
    if(!MT_GetServo(miWRK_YVsn )) isOk = false ;
    if(!MT_GetServo(miPSB_XTrm )) isOk = false ;
    if(!MT_GetServo(miPSB_YTrm )) isOk = false ;
    if(!MT_GetServo(miPSB_TFlp )) isOk = false ;
    if(!MT_GetServo(miPSB_XIns )) isOk = false ;
    if(!MT_GetServo(miWR1_XCvr )) isOk = false ;
    if(!MT_GetServo(miWR2_XCvr )) isOk = false ;
    if(!MT_GetServo(miWR3_XCvr )) isOk = false ;
    if(!MT_GetServo(miLDR_YCmp )) isOk = false ;
    if(!MT_GetServo(miLDR_ZCmp )) isOk = false ;
    if(!MT_GetServo(miULD_YCmp )) isOk = false ;
    if(!MT_GetServo(miULD_ZCmp )) isOk = false ;

    return isOk ;

}
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.

    if (MT_GetAlarm(miWRK_XVsn )){EM_SetErr(eiWRK_XVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_YVsn )){EM_SetErr(eiWRK_YVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XTrm )){EM_SetErr(eiPSB_XTrmAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_YTrm )){EM_SetErr(eiPSB_YTrmAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_TFlp )){EM_SetErr(eiPSB_TFlpAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XIns )){EM_SetErr(eiPSB_XInsAlarm ); isOk = false; }
    if (MT_GetAlarm(miWR1_XCvr )){EM_SetErr(eiWR1_XCvrAlarm ); isOk = false; }
    if (MT_GetAlarm(miWR2_XCvr )){EM_SetErr(eiWR2_XCvrAlarm ); isOk = false; }
    if (MT_GetAlarm(miWR3_XCvr )){EM_SetErr(eiWR3_XCvrAlarm ); isOk = false; }
    if (MT_GetAlarm(miLDR_YCmp )){EM_SetErr(eiLDR_YCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miLDR_ZCmp )){EM_SetErr(eiLDR_ZCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_YCmp )){EM_SetErr(eiULD_YCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZCmp )){EM_SetErr(eiULD_ZCmpAlarm ); isOk = false; }

    if (MT_GetNLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_YVsn)&&MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_XTrm)&&MT_GetHomeEnd(miPSB_XTrm)){EM_SetErr(eiPSB_XTrmNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_YTrm)&&MT_GetHomeEnd(miPSB_YTrm)){EM_SetErr(eiPSB_YTrmNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_TFlp)&&MT_GetHomeEnd(miPSB_TFlp)){EM_SetErr(eiPSB_TFlpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_XIns)&&MT_GetHomeEnd(miPSB_XIns)){EM_SetErr(eiPSB_XInsNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWR1_XCvr)&&MT_GetHomeEnd(miWR1_XCvr)){EM_SetErr(eiWR1_XCvrNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWR2_XCvr)&&MT_GetHomeEnd(miWR2_XCvr)){EM_SetErr(eiWR2_XCvrNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWR3_XCvr)&&MT_GetHomeEnd(miWR3_XCvr)){EM_SetErr(eiWR3_XCvrNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpNegLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_YVsn)&&MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XTrm)&&MT_GetHomeEnd(miPSB_XTrm)){EM_SetErr(eiPSB_XTrmPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_YTrm)&&MT_GetHomeEnd(miPSB_YTrm)){EM_SetErr(eiPSB_YTrmPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_TFlp)&&MT_GetHomeEnd(miPSB_TFlp)){EM_SetErr(eiPSB_TFlpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XIns)&&MT_GetHomeEnd(miPSB_XIns)){EM_SetErr(eiPSB_XInsPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWR1_XCvr)&&MT_GetHomeEnd(miWR1_XCvr)){EM_SetErr(eiWR1_XCvrPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWR2_XCvr)&&MT_GetHomeEnd(miWR2_XCvr)){EM_SetErr(eiWR2_XCvrPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWR3_XCvr)&&MT_GetHomeEnd(miWR3_XCvr)){EM_SetErr(eiWR3_XCvrPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpPosLim); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XTrm)){EM_SetErr(eiPSB_XTrmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_YTrm)){EM_SetErr(eiPSB_YTrmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_TFlp)){EM_SetErr(eiPSB_TFlpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XIns)){EM_SetErr(eiPSB_XInsHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWR1_XCvr)){EM_SetErr(eiWR1_XCvrHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWR2_XCvr)){EM_SetErr(eiWR2_XCvrHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWR3_XCvr)){EM_SetErr(eiWR3_XCvrHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpHomeEnd); isOk = false; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectCrash(void)
{
    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripDispr(void)
{
    bool isOk  = true;
//    if(!DM.ARAY[riLDR ].CheckAllStat(csNone) && (!IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2))) return false ;
//    else if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.MstOptn.bDryRun) return false ;
//    else if( DM.ARAY[riWR1 ].CheckExist(0,0) && !IO_GetX(xWR1_Pkg) && MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs )) && !OM.MstOptn.bDryRun) return false ;
//    else if( DM.ARAY[riWR2 ].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.MstOptn.bDryRun) return false ;
//    else if( DM.ARAY[riWR3 ].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_Lift , ccBwd) && !OM.MstOptn.bDryRun) return false ;
//    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2))) return false ;


    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripUnknown(void)
{

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStartCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    Ret0 = LDR.ToStartCon();
    Ret1 = ULD.ToStartCon();
    Ret2 = RAL.ToStartCon();
    Ret3 = HED.ToStartCon();
    Ret4 = PSB.ToStartCon();
    Ret5 = PRB.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    Ret0 = LDR.ToStopCon();
    Ret1 = ULD.ToStopCon();
    Ret2 = RAL.ToStopCon();
    Ret3 = HED.ToStopCon();
    Ret4 = PSB.ToStopCon();
    Ret5 = PRB.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    Ret0 = LDR.ToStart();
    Ret1 = ULD.ToStart();
    Ret2 = RAL.ToStart();
    Ret3 = HED.ToStart();
    Ret4 = PSB.ToStart();
    Ret5 = PRB.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    Ret0 = LDR.ToStop();
    Ret1 = ULD.ToStop();
    Ret2 = RAL.ToStop();
    Ret3 = HED.ToStop();
    Ret4 = PSB.ToStop();
    Ret5 = PRB.ToStop();

    //맵저장.
    //DM .SaveMap();
    //LT .SaveLotInfo();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 );
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    Ret0 = LDR.Autorun();
    Ret1 = ULD.Autorun();
    Ret2 = RAL.Autorun();
    Ret3 = HED.Autorun();
    Ret4 = PSB.Autorun();
    Ret5 = PRB.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr() ;
    bool isRunning =  m_bRun     ;
    bool isStop    = !m_bRun     ;
    bool isLotEnd  =  LT.m_bLotEnd ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Sequence State. Tower Lamp
         if (isInit   ) { m_iSeqStat = ssInit       ;}
    else if (isLotEnd ) { m_iSeqStat = ssWorkEnd    ;}
    else if (isError  ) { m_iSeqStat = ssError      ;}
    else if (isRunning) { m_iSeqStat = ssRunning    ;}
    else if (isStop   ) { m_iSeqStat = ssStop       ;}
    else                { m_iSeqStat = m_iSeqStat   ;}

}

//---------------------------------------------------------------------------
void __fastcall CSequence::Update(void)
{
    //Update DeviceNet (I/O).
    SM_Update(m_iSeqStat);

    //Inspection.
    InspectMainPower() ;    
    InspectMainAir  () ;
    InspectEmergency() ;
    InspectActuator () ;
    InspectMotor    () ;
    InspectCrash    () ;

    //Update ErrorProc.
    UpdateButton();

    //Check Button.
    UpdateSeqState();

    //Update Actuator (Input).
    LT.Update(m_iSeqStat);

    //Ten Key.
//    ADL.Update();

    //Update Motor State (Input).
    MM.Update();

    //Update PstEqp State
}


