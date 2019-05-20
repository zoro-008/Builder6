//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Feed.h"
#include "Rail.h"
#include "Work.h"
#include "Sort.h"
#include "Heater.h"
#include "ProbTester.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SpcUnit.h"

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
}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall CSequence::Init(void)
{
    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmToStrtCon.Clear();
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
    m_bAutoMode  = true  ;
    m_bFlick     = false ;
    m_iStep      = scIdle;
    m_iSeqStat   = ssStop;

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
    m_tmToStrtCon.Clear();
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
    FED.Reset();
    RAL.Reset();
    WRK.Reset();
    SRT.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

    MT_SetServoAll(true);

    TL_MuteSnd(false);
    //랏 엔드시에 리셑해야 부져 꺼지게.
    //LT.m_bLotEnd = false ;

}

void __fastcall CSequence::tmReset(void)
{
    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmToStrtCon.Clear();
    m_tmFlickOn  .Clear();
    m_tmFlickOff .Clear();
    m_tmTemp     .Clear();
    m_tmCloseDoor.Clear();
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        //if(!OM.CmnOptn.bDoorSkip) InspectDoor    () ;
        InspectHomeEnd  () ;
        InspectBin      () ;
        //InspectLightGrid() ;
    }
    InspectTemp     () ;

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_StartSw) || m_bBtnStart ;
    bool bStopSw  = IO_GetXUp(xETC_StopSw ) || m_bBtnStop  ;
    bool bResetSw = IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;
    bool bAirSw   = IO_GetXUp(xETC_AirSw  ) || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_InitSw ) ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun);
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_AirLp    , IO_GetY(yETC_MainSol));
    IO_SetY(yETC_InitLp   , IO_GetX(xETC_InitSw  ) ) ;

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //Door Sensor.
    bool isAllCloseDoor = true ;

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!"); return ;}
        if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        FED.bConEnd = false ;
    }

    if (bStopSw) {
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);
    }
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
    }

    //Err Log
    static bool bIsErr ;
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
    if (isErr && bStopSw) TL_MuteSnd(true);

    //Set Condition Flags.
    bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcNone ;
    bool isResetCon = bResetSw && !m_bRun ;

    //Run.
    if (isRunCon && !m_iStep) {
        m_iStep = scToStartCon ;
        m_tmHeatSec.Clear();
         //Init. Updated Flag.
        EM_SetDisp(true);
    }

    if (isStopCon  &&  m_iStep) m_bReqStop = true ;
    if (isResetCon && !m_iStep) Reset()           ;

    //static bool bStartDeley = false;
    //if(m_iStep == scToStart){
    //    IO_SetY(yETC_FeederOn , true);
    //    if(m_tmSttSec.OnDelay (true , 10 * 1000)) {
    //        bStartDeley = true;
    //    }
    //    if(m_tmStopSec.OnDelay (true , OM.DevOptn.iIdleTime * 1000)) {
    //        bStartDeley = true;
    //    }
    //    if(bStartDeley){
    //        if(!m_tmHeatSec.OnDelay(m_iStep == scToStart , OM.DevOptn.iPKGHeatSec * 1000)) {
    //            tmReset();
    //            return ;
    //        }
    //        else bStartDeley = false;
    //    }
    //}

    if (m_tmToStrtCon.OnDelay(m_iStep == scToStartCon , OM.DevOptn.iStartHeatSec * 1000)) {
        //Trace Log.
        AnsiString Msg ;
        Msg.sprintf("ToStrtTimeOut : m_iStep=%d" ,m_iStep );
        Trace  ("SEQ",Msg.c_str() );
        EM_SetErr (eiETC_ToStartTO);
        m_iStep = scIdle;
        m_bRun  = false;
    }
    if(m_iStep == scToStartCon && isStopCon) {
        m_iStep = scToStopCon ;
    }

    if(m_iStep == scToStart){
        if(!m_tmHeatSec.OnDelay(m_iStep == scToStart , OM.DevOptn.iPKGHeatSec * 1000)) {
            tmReset();
            return ;
        }
    }

    if (m_tmToStrt.OnDelay(m_iStep == scToStart , 10000)) {
        //Trace Log.
        AnsiString Msg ;
        Msg.sprintf("ToStrtTimeOut : m_iStep=%d" ,m_iStep );
        Trace  ("SEQ",Msg.c_str() );
        EM_SetErr (eiETC_ToStartTO);
        m_iStep = scIdle;
        m_bRun  = false;
    }

    //Time Out.
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , 10000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);

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
                                    //랏엔드 상황.
                                    //LT.LotEnd();
                                    m_iStep=scToStopCon ;
                                }
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            return ;

        case scToStopCon :  if(!ToStopCon()) {
                                if(AutoRun()) {
                                    //랏엔드 상황.
                                    //LT.LotEnd();
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

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectPowerAndAir(void)
{                                                                                         
    bool isOk = true ;

    //if(!IO_GetX(xETC_MainSol))  isOk = false ;
    if(!IO_GetY(yETC_MainSol)) { isOk = false ; EM_SetErr(eiETC_MainAir  ) ; }
    if(!IO_GetX(xETC_PowerOn)) { isOk = false ; EM_SetErr(eiETC_MainPower) ; }

    return isOk ;
}

bool  __fastcall CSequence::InspectTemp(void)
{
    if(OM.CmnOptn.bIgnrHeatErr || !IO_GetY(yETC_HeatOn))return true ;

    bool isOk = true ;

    if(IO_GetX(xRAL_Heat1UpLmt  )) {isOk = false ; EM_SetErr(eiRAL_Heat1UpLmt ); }
    if(IO_GetX(xRAL_Heat1Discnct)) {isOk = false ; EM_SetErr(eiRAL_Heat1Discnt); }
    if(IO_GetX(xRAL_Heat2UpLmt  )) {isOk = false ; EM_SetErr(eiRAL_Heat2UpLmt ); }
//    if(IO_GetX(xRAL_Heat2DnLmt  )) {isOk = false ; EM_SetErr(eiRAL_Heat2DnLmt ); }


    if(m_bRun) {
        if(OM.DevOptn.iTempSet1 + OM.DevOptn.iTempGap < HTR.GetTemp1() ){ isOk = false ; EM_SetErr(eiRAL_Heat1SoftLmt) ; IO_SetY(yETC_HeatOn,false);}
        if(OM.DevOptn.iTempSet1 - OM.DevOptn.iTempGap > HTR.GetTemp1() ){ isOk = false ; EM_SetErr(eiRAL_Heat1SoftLmt) ;}

        if(OM.DevOptn.iTempSet2 + OM.DevOptn.iTempGap < HTR.GetTemp2() ){ isOk = false ; EM_SetErr(eiRAL_Heat2SoftLmt) ; IO_SetY(yETC_HeatOn,false);}
        if(OM.DevOptn.iTempSet2 - OM.DevOptn.iTempGap > HTR.GetTemp2() ){ isOk = false ; EM_SetErr(eiRAL_Heat2SoftLmt) ;}
    }    

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    //if (IO_GetX(xETC_EmgSw)) {
    //    EM_SetErr(eiETC_Emergency);
    //    MT_SetServoAll(false);
    //    IO_SetY(yLDR_BreakOff, true);
    //    IO_SetY(yULD_BreakOff, true);
    //    isOk = false ;
    //}

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
    //if (!IO_GetX(xETC_FDoor) ){EM_SetErr(eiETC_FDoor); isOk = false;} //Front Door Sensor
    //if (!IO_GetX(xETC_BDoor) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(AT_Err(aiRAL_IdxUpDn)) {EM_SetErr(eiRAL_IdxUpDnTo   ) ; isOk = false ; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

    //Inspect Motor Alarm.
//    for(int i=0; i<MAX_MOTR; i++) if (MT_GetAlarm   (i)){EM_SetErr((int)eiFED_TFedAlarm  + i ); isOk = false; }
//    for(int i=0; i<MAX_MOTR; i++) if (MT_GetNLimSnsr(i)){EM_SetErr((int)eiFED_TFedNegLim + i ); isOk = false; }
//    for(int i=0; i<MAX_MOTR; i++) if (MT_GetPLimSnsr(i)){EM_SetErr((int)eiFED_TFedPosLim + i ); isOk = false; }

    if (MT_GetAlarm   (miRAL_XIdx)){EM_SetErr(eiRAL_XCmpAlarm); isOk = false; }
    if (MT_GetAlarm   (miWRK_ZPrb)){EM_SetErr(eiWRK_ZCmpAlarm); isOk = false; }
    if (MT_GetAlarm   (miSRT_TSrt)){EM_SetErr(eiSRT_TIdxAlarm); isOk = false; }

    if (MT_GetNLimSnsr(miRAL_XIdx)){EM_SetErr(eiRAL_XIdxNegLim); isOk = false; }
    if (MT_GetNLimSnsr(miWRK_ZPrb)){EM_SetErr(eiWRK_ZPrbNegLim); isOk = false; }
    if (MT_GetNLimSnsr(miSRT_TSrt)){EM_SetErr(eiSRT_TSrtPosLim); isOk = false; }

    if (MT_GetPLimSnsr(miRAL_XIdx)){EM_SetErr(eiRAL_XIdxPosLim); isOk = false; }
    if (MT_GetPLimSnsr(miWRK_ZPrb)){EM_SetErr(eiWRK_ZPrbPosLim); isOk = false; }
    if (MT_GetPLimSnsr(miSRT_TSrt)){EM_SetErr(eiSRT_TSrtPosLim); isOk = false; }


    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT_GetHomeEnd   (miRAL_XIdx)){EM_SetErr(eiFED_TFedHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd   (miWRK_ZPrb)){EM_SetErr(eiRAL_XIdxHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd   (miSRT_TSrt)){EM_SetErr(eiSRT_TSrtHomeEnd); isOk = false; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectCrash(void)
{
    return true ;
}
//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectBin(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(!IO_GetX(xSRT_Bin1Exist)) {EM_SetErr(eiSRT_Bin1NoExst); isOk = false ; }
    if(!IO_GetX(xSRT_Bin2Exist)) {EM_SetErr(eiSRT_Bin2NoExst); isOk = false ; }
    if(!IO_GetX(xSRT_Bin3Exist)) {EM_SetErr(eiSRT_Bin3NoExst); isOk = false ; }
    if(!IO_GetX(xSRT_Bin4Exist)) {EM_SetErr(eiSRT_Bin4NoExst); isOk = false ; }
    if(!IO_GetX(xSRT_Bin5Exist)) {EM_SetErr(eiSRT_Bin5NoExst); isOk = false ; }

    //Ok.
    return isOk;

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
    bool Ret0 , Ret1 , Ret2 , Ret3 ;
    //Call ToStop.
    Ret0 = FED.ToStartCon();
    Ret1 = RAL.ToStartCon();
    Ret2 = WRK.ToStartCon();
    Ret3 = SRT.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 ) ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;
    //Call ToStop.
    Ret0 = FED.ToStopCon();
    Ret1 = RAL.ToStopCon();
    Ret2 = WRK.ToStopCon();
    Ret3 = SRT.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 ) ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;
    //Call ToStop.
    Ret0 = FED.ToStart();
    Ret1 = RAL.ToStart();
    Ret2 = WRK.ToStart();
    Ret3 = SRT.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 ) ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;
    //Call ToStop.
    Ret0 = FED.ToStop();
    Ret1 = RAL.ToStop();
    Ret2 = WRK.ToStop();
    Ret3 = SRT.ToStop();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 ) ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;
    //Call ToStop.

    Ret0 = FED.Autorun();
    Ret1 = WRK.Autorun();
    Ret2 = RAL.Autorun();
    Ret3 = SRT.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3) ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr() ;
    bool isRunWarn =  m_iStep == scToStartCon || m_iStep == scToStart ;
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
    else if (isRunWarn) { m_iSeqStat = ssRunWarn    ;}
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
    InspectPowerAndAir() ;
    InspectEmergency  () ;
    InspectActuator   () ;
    InspectMotor      () ;
    InspectCrash      () ;
//    InspectBin        () ;

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

    //update ProbeTester
    PTR.Update();

    //update Heater.
    HTR.Update();

}


