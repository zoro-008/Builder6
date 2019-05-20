//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
//---------------------------------------------------------------------------
#include "JigPick.h"
#include "Dispnr.h"
#include "Place.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
//#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "SPCUnit.h"

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
    m_bBtnReset    = false ;
    m_bBtnStart    = false ;
    m_bBtnStop     = false ;
    m_bBtnAir      = false ;

    m_bRun         = false ;
    m_bFlick       = false ;
    m_iStep        = scIdle;
    m_iSeqStat     = ssStop;

    SEQ.Pt_Step.bIdle      = false;
    SEQ.Pt_Step.bJigPick   = false;
    SEQ.Pt_Step.bDispenser = false;
    SEQ.Pt_Step.bPlace     = false;

    Seq_Time = 0;
    End_Time = 0;
    bSEQEnd  = false;
}

void __fastcall CSequence::Close(void)
{

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


    //Lot End Flag Reset.
    LTQ.Reset();
    JIP.Reset();
    DIP.Reset();
    PLC.Reset();

    FM_CloseMsgOk();

    MT_SetServoAll(true);

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        InspectHomeEnd () ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw   = IO_GetXUp(xETC_StartSw ) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw    = IO_GetXUp(xETC_StopSw  ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw   = m_bBtnReset;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) ;

    //버튼 클릭시나
    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun       );
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !bStopBtnFlick);

    m_bBtnStart = false ;
    m_bBtnStop  = false ;

    //Init. Button Flags.
    if (bStartSw) {
        if(!isHomeEnd) {FM_MsgOk("WARNING" , "Please All Home"); bStartSw = false ; }
    }

    //Set Condition Flags.
    bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcNone ;
    bool isResetCon = bResetSw && !m_bRun;


    //Run.
    //if (isRunCon && !m_iStep) {
    //    m_iStep = scToStartCon ;
    //    EM_SetDisp(true );
    //}

    if (isRunCon && !m_iStep) {
        if((!Pt_Step.bIdle && !Pt_Step.bJigPick && !Pt_Step.bDispenser && !Pt_Step.bPlace) ||
           (!Pt_Step.bIdle && !Pt_Step.bJigPick && !Pt_Step.bDispenser &&  Pt_Step.bPlace)){
            Pt_Step.bIdle      = true;
            //Pt_Step.bPlace     = false;
        }
        m_iStep = scToStartCon ;
        EM_SetDisp(true );
    }

    if (isResetCon) Reset();
    if (isStopCon  &&  m_iStep) m_bReqStop = true ;

    //강제 리셋 신호 주기.JS
    if (!m_bRun && m_tmTemp.OnDelay(IO_GetX(xETC_StopSw  ) , 1000)) {
        m_bBtnReset = true;
    }
    if (!IO_GetX(xETC_StopSw)) m_tmTemp.Clear();

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
                            m_bMsgView = true;
                            m_iStep=scRun ;
                            return ;

        case scRun        : if(!m_bReqStop) {
                                if(AutoRun()) {
                                    //랏엔드 상황.
                                    LTQ.LotEnd();
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
                                    LTQ.LotEnd();
                                }
                                return ;
                            }
                            m_bRun = false ;
                            m_iStep=scToStop;
                            return ;

        case scToStop    :  if (!ToStop()) return ;
                            m_iStep = scIdle ;
                            m_bReqStop         = false ;
                            //Pt_Step.bIdle      = true;
                            //Pt_Step.bJigPick   = true;
                            //Pt_Step.bDispenser = true;
                            //Pt_Step.bPlace     = true;

                            DM.SaveMap();

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{
    bool isOk = true ;

    return isOk ;                                                                         
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectTemp(void)
{
    bool isOk = true ;

    //if(IO_GetX(xPSB_TempAlrm)){EM_SetErr(eiPRB_TempZ1Alrm); isOk = false ;}

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetX(xETC_EmgSw)) {
        EM_SetErr(eiETC_Emergency); MT_SetServoAll(false); isOk = false ;
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

    for(int i = 0 ; i < MAX_MOTR ; i++) {
        if (MT_GetAlarm   (i)                  ){EM_SetErrMsg(eiMTR_Alarm  , MT_GetName(i)); isOk = false; }
        if (MT_GetNLimSnsr(i)&&MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_NegLim , MT_GetName(i)); isOk = false; }
        if (MT_GetPLimSnsr(i)&&MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_PosLim , MT_GetName(i)); isOk = false; }
    }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    for(int i = 0 ; i < MAX_MOTR ; i++) {
        if (!MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_HomeEnd , MT_GetName(i)); isOk = false; }
    }


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

//    if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect)  && !OM.MstOptn.bDryRun ) isOk = false ;
//    if(DM.ARAY[riPSB].GetCntExist() &&  !OM.MstOptn.bDryRun) {
//        if(!IO_GetX(xPSB_LDetect) && !IO_GetX(xPSB_RDetect) && !IO_GetX(xPSB_Pkg1) && !IO_GetX(xPSB_Pkg2) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) isOk = false ;
//    }
//
//    if (OM.EqpOptn.bExistLoader  ) { if(!DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) ) ) isOk = false ; }
//    if (OM.EqpOptn.bExistUnLoader) { if(!DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) ) isOk = false ; }
//
    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripUnknown(void)
{
//    if (DM.ARAY[riPR2].CheckAllStat(csNone) && DM.ARAY[riPR1].CheckAllStat(csNone) && IO_GetX(xPRB_3Pkg) ) return false ;
    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStartCon(void)
{
    bool Ret[MAX_PART];
    //Call ToStop.
    Ret[piJIP] = JIP.ToStartCon();
    Ret[piDIP] = DIP.ToStartCon();
    Ret[piPLC] = PLC.ToStartCon();

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!Ret[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret[MAX_PART];
    //Call ToStop.
    Ret[piJIP] = JIP.ToStopCon();
    Ret[piDIP] = DIP.ToStopCon();
    Ret[piPLC] = PLC.ToStopCon();

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!Ret[i]) return false ;
    }

    return true ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret[MAX_PART];
    //Call ToStop.
    Ret[piJIP] = JIP.ToStart();
    Ret[piDIP] = DIP.ToStart();
    Ret[piPLC] = PLC.ToStart();


    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!Ret[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret[MAX_PART];
    //Call ToStop.
    Ret[piJIP] = JIP.ToStop();
    Ret[piDIP] = DIP.ToStop();
    Ret[piPLC] = PLC.ToStop();

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!Ret[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret[MAX_PART];
    //Call ToStop.
    Ret[piJIP] = JIP.Autorun();
    Ret[piDIP] = DIP.Autorun();
    Ret[piPLC] = PLC.Autorun();

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!Ret[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr() ;
    bool isRunning =  m_bRun     ;
    bool isStop    = !m_bRun     ;                 
    bool isLotEnd  =  LTQ.GetLotEnd() ;

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
    //LTQ.Update(m_iSeqStat);

    //Ten Key.
    //ADL.Update();

    //Update Motor State (Input).
    MM.Update();

    //SPC
    SPC.Update(m_iSeqStat);


}


