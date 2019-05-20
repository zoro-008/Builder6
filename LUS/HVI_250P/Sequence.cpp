//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
//#include "AdlinkTenKey.h"
#include "SPCUnit.h"
#include "EqpComUnit.h"
//---------------------------------------------------------------------------
//Form Refernce
//---------------------------------------------------------------------------
#include "FrmMain.h"
//---------------------------------------------------------------------------
//Part Refernce
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "PreIdx.h"
#include "Work.h"
#include "PstIdx.h"
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
    m_bAutoMode  = true  ;
    m_bFlick     = false ;
    m_iStep      = scIdle;
    m_iSeqStat   = ssStop;

    IO_SetY(yETC_MainSol , true); // 시작할때 솔 켜달라고 했나봄. dd 내가 안함.

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
    PRB.Reset();
    PRI.Reset();
    WRK.Reset();
    PSI.Reset();

    FM_CloseMsgOk();

    MT_SetServoAll(true); //꺼진 애들만 켜줌.

    //랏 엔드시에 리셑해야 부져 꺼지게.
    LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    InspectLightGrid() ;

    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bDoorSkip ) InspectDoor    () ;
        InspectHomeEnd () ;
    }

    //빈통 비웠다~~~~
    if(IO_GetXDn(xWRK_Bin1)) WRK.ClearBin(0);
    if(IO_GetXDn(xWRK_Bin2)) WRK.ClearBin(1);
    if(IO_GetXDn(xWRK_Bin3)) WRK.ClearBin(2);
    if(IO_GetXDn(xWRK_Bin4)) WRK.ClearBin(3);
    if(IO_GetXDn(xWRK_Bin5)) WRK.ClearBin(4);


    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_StartSw ) || m_bBtnStart ;
    bool bStopSw  = IO_GetXUp(xETC_StopSw  ) || m_bBtnStop  ;
    bool bResetSw = IO_GetXUp(xETC_ResetSw ) || m_bBtnReset ;
    bool bAirSw   = IO_GetXUp(xETC_AirSw   ) || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_InitSw  )                ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;

    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun                  );
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick );
    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr)      );
    IO_SetY(yETC_AirLp    , IO_GetX(xETC_AirSw   ) || IO_GetY(yETC_MainSol)    );
    IO_SetY(yETC_InitLp   , IO_GetX(xETC_InitSw  )                             );

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //Door Sensor.
//    bool isAllCloseDoor = IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear);

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode && !OM.MstOptn.bDebugMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        FM_CloseMsgOk();    
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!" ); return ;} //이거 로더 없을때 로더 모터 잡아 놓으면 안될듯.dd
//        if(!LT.m_bLotOpen     ) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        EC.SendPreEqp(rsStart);
//        ULD.Reset();
//        if(!IO_GetX(xETC_PostEqpReady))EM_SetErr(eiETC_PostEqpNotReady);
    }

    if (bStopSw) {
        EC.SendPreEqp(rsStop);
//        MM.m_bStopTestMrk = true ;
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);
    }
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
//        IO_SetY(yETC_MainSol2, !IO_GetY(yETC_MainSol )) ;
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
    if (isErr && bStopSw) TL_MuteSnd();

    //Set Condition Flags.
    bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcNone ;
    bool isResetCon = bResetSw && !m_bRun ;

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
        EC.SetPreEqpReady(false) ;
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

//        IO_SetY(yLDR_MgzInAC  , false) ;
//        IO_SetY(yLDR_MgzOutAC , false) ;

        m_iStep    = scIdle;
        m_bReqStop = false;
        EC.SetPreEqpReady(false) ;
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
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetX(xETC_EmgSw1) || IO_GetX(xETC_EmgSw2)) {// || IO_GetX(xLDR_Emg) || IO_GetX(xULD_Emg)) {
        EM_SetErr(eiETC_Emergency); MT_SetServoAll(false); isOk = false ;
    }

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectLightGrid(void)
{
    bool isOk = true ;

    //Z 축이 움직일 때만.
    if (IO_GetX(xETC_AreaFront) && !MT_GetStop(miWRK_ZPch) ) {
        EM_SetErr(eiETC_AreaSsrDectected );
        MT_EmgStop(miWRK_ZPch);
        Trace("**** 버퍼구간 Area 센서 감지 ****","펀치 Z 모터 STOP ");
        isOk = false ;
    }

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectDoor(void)
{
    //Local Var.
    bool isOk = true;

    //Check Door
    if (!IO_GetX(xETC_DoorBack ) ){EM_SetErr(eiETC_FDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_DoorFront) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Back  Door Sensor
    if (!IO_GetX(xETC_DoorRear ) ){EM_SetErr(eiETC_RDoor); isOk = false;} //Rear  Door Sensor

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(AT_Err(aiPRB_Idx )) {EM_SetErr(eiPRB_IdxTo  ) ; isOk = false ; }
    if(AT_Err(aiPRI_Idx )) {EM_SetErr(eiWRK_1IdxTo ) ; isOk = false ; }
    if(AT_Err(aiPSI_Idx )) {EM_SetErr(eiWRK_2IdxTo ) ; isOk = false ; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.

    if (MT_GetAlarm(miWRK_ZPch)){EM_SetErr(eiWRK_ZPchAlarm ); isOk = false; }
    if (MT_GetAlarm(miPRB_XIdx)){EM_SetErr(eiPRB_XIdxAlarm ); isOk = false; }
    if (MT_GetAlarm(miPRB_TFlp)){EM_SetErr(eiPRB_TFlpAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_XSrt)){EM_SetErr(eiWRK_XSrtAlarm ); isOk = false; }
    if (MT_GetAlarm(miPRI_XIdx)){EM_SetErr(eiPRI_XIdxAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSI_XIdx)){EM_SetErr(eiPSI_XIdxAlarm ); isOk = false; }

    if (MT_GetNLimSnsr(miWRK_ZPch)&&MT_GetHomeEnd(miWRK_ZPch)){EM_SetErr(eiWRK_ZPchNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPRB_XIdx)&&MT_GetHomeEnd(miPRB_XIdx)){EM_SetErr(eiPRB_XIdxNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPRB_TFlp)&&MT_GetHomeEnd(miPRB_TFlp)){EM_SetErr(eiPRB_TFlpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_XSrt)&&MT_GetHomeEnd(miWRK_XSrt)){EM_SetErr(eiWRK_XSrtNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPRI_XIdx)&&MT_GetHomeEnd(miPRI_XIdx)){EM_SetErr(eiPRI_XIdxNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSI_XIdx)&&MT_GetHomeEnd(miPSI_XIdx)){EM_SetErr(eiPSI_XIdxNegLim); isOk = false;}

    if (MT_GetPLimSnsr(miWRK_ZPch)&&MT_GetHomeEnd(miWRK_ZPch)){EM_SetErr(eiWRK_ZPchPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPRB_XIdx)&&MT_GetHomeEnd(miPRB_XIdx)){EM_SetErr(eiPRB_XIdxPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPRB_TFlp)&&MT_GetHomeEnd(miPRB_TFlp)){EM_SetErr(eiPRB_TFlpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_XSrt)&&MT_GetHomeEnd(miWRK_XSrt)){EM_SetErr(eiWRK_XSrtPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPRI_XIdx)&&MT_GetHomeEnd(miPRI_XIdx)){EM_SetErr(eiPRI_XIdxPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSI_XIdx)&&MT_GetHomeEnd(miPSI_XIdx)){EM_SetErr(eiPSI_XIdxPosLim); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT_GetHomeEnd(miWRK_ZPch)){EM_SetErr(eiWRK_ZPchHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPRB_XIdx)){EM_SetErr(eiPRB_XIdxHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPRB_TFlp)){EM_SetErr(eiPRB_TFlpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_XSrt)){EM_SetErr(eiWRK_XSrtHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPRI_XIdx)){EM_SetErr(eiPRI_XIdxHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSI_XIdx)){EM_SetErr(eiPSI_XIdxHomeEnd); isOk = false; }

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
//    else if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) return false ;

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
    bool Ret0 , Ret1 , Ret2 , Ret3 ;//, Ret4 ;
    //Call ToStop.
    Ret0 = PRB.ToStartCon();
    Ret1 = PRI.ToStartCon();
    Ret2 = WRK.ToStartCon();
    Ret3 = PSI.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 );//&& Ret4 );
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;//, Ret4 ;
    //Call ToStop.
    Ret0 = PRB.ToStopCon();
    Ret1 = PRI.ToStopCon();
    Ret2 = WRK.ToStopCon();
    Ret3 = PSI.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 );//&& Ret4 );

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;//, Ret4 ;
    //Call ToStop.
    Ret0 = PRB.ToStart();
    Ret1 = PRI.ToStart();
    Ret2 = WRK.ToStart();
    Ret3 = PSI.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 );//&& Ret4 );
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;// , Ret4 ;
    //Call ToStop.
    Ret0 = PRB.ToStop();
    Ret1 = PRI.ToStop();
    Ret2 = WRK.ToStop();
    Ret3 = PSI.ToStop();


    //맵저장.
//    DM.SaveMap();
//    LT.SaveLotInfo();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3); // && Ret4 );
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 ;//, Ret4 ;
    //Call ToStop.
    Ret0 = PRB.Autorun();
    Ret1 = PRI.Autorun();
    Ret2 = WRK.Autorun();
    Ret3 = PSI.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3); // && Ret4 );
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

    //Ec Update
    EC.Update();

}


