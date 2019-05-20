//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"

#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "ManualMan.h"
#include "SPCUnit.h"
#include "Loader.h"
#include "PreRail.h"
#include "PostRail.h"
#include "UnLoader.h"


#include "FormMain.h"
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
    m_bLoadChg   = false ;

    m_bRunWrn    = false ;
    m_bRun       = false ;
    m_bAutoMode  = true  ;
    m_bFlick     = false ;
    m_iStep      = scIdle;
    m_iSeqStat   = ssStop;

    m_pPart[piLDR_F] = &LDR_F ;
    m_pPart[piPRE_F] = &PRE_F ;
    m_pPart[piPST_F] = &PST_F ;
    m_pPart[piULD_F] = &ULD_F ;
    m_pPart[piLDR_R] = &LDR_R ;
    m_pPart[piPRE_R] = &PRE_R ;
    m_pPart[piPST_R] = &PST_R ;
    m_pPart[piULD_R] = &ULD_R ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::Reset(void)
{
    m_bRunWrn = false;     //Reel End 신호 Buzzer Off 하기 위해서.

    //Check running flag.
    if (m_bRun              ) return;
    if (m_iSeqStat == ssInit) return;

    LOT.Reset();

    if (!EM_IsErr()         ) return;

    Trace("Seq","Reset");

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
    m_bLoadChg   = false ;

    m_bRun       = false ;
    m_iStep      = scIdle;


    //System.
    MT_ResetAll();
    AT_Reset   ();

    //Error.
    EM_Clear();


    //Part.
    for(int i=0; i < MAX_PART; i++){
        m_pPart[i] -> Reset();
    }

    LOT.Reset();

    FM_CloseMsgOk();

    MT_SetServoAll(true);

    m_iSeqStat = ssStop;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
//        if(!OM.CmnOptn.bDoorSkip ) InspectDoor () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw   = IO_GetXUp(xETC_StartSw)   || m_bBtnStart ;
    bool bStopSw    = IO_GetXUp(xETC_StopSw )   || m_bBtnStop  ;
    bool bLoadChgSw = IO_GetXUp(xETC_LoadChgSw) || m_bLoadChg  ;
    bool bResetSw   = m_bBtnReset;

//    bool bInitSw  = IO_GetXUp(xETC_InitSw ) ;
///    bool bOperSw  = IO_GetXUp(xETC_OperSw ) ;
//    bool bStartSw   = m_bBtnStart ;
//    bool bStopSw    = m_bBtnStop  ;
//    bool bLoadChgSw = m_bLoadChg  ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;

    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw   ) ||  m_bRun);
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw    ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LoadChgLp, IO_GetX(xETC_LoadChgSw));

//    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr));
//    IO_SetY(yETC_InitLp   , IO_GetX(xETC_InitSw  ) );
//    IO_SetY(yETC_OperLp   , IO_GetX(xETC_OperSw  ) );

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bLoadChg  = false ;
    m_bBtnReset = false ;

    m_tmTemp.Clear(); // juhyeon, Stop 버튼 2초 이상 유지시 Reset 버튼 효과를 나타내기 위해 설정.

    //Door Sensor.                 -
//    bool isAllCloseDoor = true ;//IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!"); return ;}
    }


    if (bStopSw) {
//        if(!m_tmToStop.OnDelay(true, 2000)) return ;
//            Reset();
            IO_SetY(yRAL_ConMotorCw, false);
    }

    if (bLoadChgSw) {
        MM.SetManCycle(mcLDR_CycleManLoadChg);
    }

    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!"); return ;}
//        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
//        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
    }

    //Err Log
    static bIsErr;
    isErr = EM_IsErr() ;
    if(isErr && !bIsErr) {
        bIsErr = true ;
    }
    if(!isErr && bIsErr) {
        bIsErr = false ;
    }

    //Buzzer Off.
    if (isErr && bStopSw) TL_MuteSnd(true);

     //Set Condition Flags.
     bool isStopCon     = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
     bool isRunCon      = bStartSw && !isErr  ;
     bool isResetCon    = bResetSw ;

    //Run.
    if (isRunCon && !m_iStep) {
        m_iStep = scToStartCon ;
         EM_SetDisp(true);
    }

    if (isStopCon     &&  m_iStep) m_bReqStop = true ;
    if (isResetCon               ) Reset()           ;


     if (isErr && bStopSw) TL_MuteSnd(false);

     //Set Condition Flags.
//     bool isStopCon     = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
//     bool isRunCon      = bStartSw && !isErr  ;
//     bool isResetCon    = bResetSw ;


    //Run.
    if (isRunCon && !m_iStep) {
        m_iStep = scToStartCon ;
         EM_SetDisp(true);
    }

    if (isStopCon     &&  m_iStep) m_bReqStop = true ;
    if (isResetCon               ) Reset()           ;


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

//        IO_SetY(yLDR_RollAC , false) ;
//        MT_Stop(miRAL_TSpr);

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
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetX(xETC_EmgSw)){
        EM_SetErr(eiETC_Emergency);
        IO_SetY(yLDR_FMaskUpDnCw , false);
        IO_SetY(yLDR_FMaskUpDnCcw, false);
        IO_SetY(yLDR_FMaskUpDnCw , false);
        IO_SetY(yLDR_FMaskUpDnCcw, false);
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
    for(int i = 0 ; i < MAX_ACTR ; i++) {
        if(AT_Err(i)) {EM_SetErrMsg(eiATR_TimeOut ,AT_GetName(i)) ; isOk = false ; }
    }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.

//    if (MT_GetAlarm(miRAL_TSpr)){EM_SetErr(eiRAL_TSprAlarm ); isOk = false; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
//    if (!MT_GetHomeEnd(miRAL_TSpr)){EM_SetErr(eiRAL_TSprHomeEnd); isOk = false; }

//    for(int i = 0 ; i < MAX_MOTR ; i++) {
//        if (!MT_GetHomeEnd(i)) { EM_SetErrMsg(eiMTR_HomeEnd , MT_GetName(i)); isOk = false; }
//    }


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
//    bool Ret0, Ret1, Ret2;
    //Call ToStop.
//    Ret0 = LDR_F.ToStartCon();
//    Ret1 = RAL.ToStartCon();
//    Ret2 = ULD_F.ToStartCon();

    //Ok.
//    return (Ret0 && Ret1 & Ret2);

    bool bRet[MAX_PART];
    //Call ToStop.

    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStartCon();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool bRet[MAX_PART] ;

    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStopCon();
    }
    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool bRet[MAX_PART] ;

    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStart();
    }


    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool bRet[MAX_PART];

    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStop();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

//    DM .SaveMap();

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool bRet[MAX_PART];
    //Call ToStop.
    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> Autorun();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr() ;
    bool isRunWrn  =  m_bRunWrn  ;
    bool isRunning =  m_bRun     ;
    bool isStop    = !m_bRun     ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Sequence State. Tower Lamp
         if (isInit   ) { m_iSeqStat = ssInit       ;}
    else if (isError  ) { m_iSeqStat = ssError      ;}
    else if (isRunWrn ) { m_iSeqStat = ssRunWarn    ;}
    else if (isRunning) { m_iSeqStat = ssRunning    ;}
    else if (isStop   ) { m_iSeqStat = ssStop       ;}
    else                { m_iSeqStat = m_iSeqStat   ;}

}

//---------------------------------------------------------------------------
void __fastcall CSequence::Update(void)
{
    //Update DeviceNet (I/O).
    SM_Update(m_iSeqStat);

    //Part.
    
//    for(int i=0; i < MAX_PART; i++){
//        m_pPart[i] -> Update();
//    }

    //Inspection.
    InspectEmergency() ;
    InspectActuator () ;      
    InspectMotor    () ;
    InspectCrash    () ;

    //Update ErrorProc.
    UpdateButton();

    //Check Button.
    UpdateSeqState();

    //Update Motor State (Input).
    MM.Update();

    SPC.Update(m_iSeqStat);
}





