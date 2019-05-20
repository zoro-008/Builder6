//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Rail.h"
#include "LaserEO.h"
#include "MarkVisn.h"
#include "PostBuff.h"
#include "UnLoader.h"
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
    PRB.Reset();
    RAL.Reset();
    LSR.Reset();
    VSN.Reset();
    PSB.Reset();
    LDR.Reset();
    ULD.Reset();

    //Lot End Flag Reset.
    LTQ.Reset();

    FM_CloseMsgOk();

    MT_SetServoAll(true);

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bIgnrDoor) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw   = IO_GetXUp(xETC_LStartSw) || IO_GetXUp(xETC_CStartSw) || IO_GetXUp(xETC_RStartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw    = IO_GetXUp(xETC_LStopSw ) || IO_GetXUp(xETC_CStopSw ) || IO_GetXUp(xETC_RStopSw ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw   = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_CResetSw) || IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw     = IO_GetXUp(xETC_LAirSw  ) || IO_GetXUp(xETC_RAirSw  ) || m_bBtnAir   ;
    bool bInitSw    = IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;
    bool bVisionSw  = IO_GetXUp(xETC_CVisnSw ) ;
    bool bMarkingSw = IO_GetXUp(xETC_CMarkSw ) ;



    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;


    //버튼 클릭시나
    IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw ) ||  m_bRun);
    IO_SetY(yETC_CStartLp  , IO_GetX(xETC_CStartSw ) ||  m_bRun);
    IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);
    IO_SetY(yETC_LStopLp   , IO_GetX(xETC_LStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_CStopLp   , IO_GetX(xETC_CStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_RStopLp   , IO_GetX(xETC_RStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LResetLp  , IO_GetX(xETC_LResetSw ) || (m_bFlick && isErr)     );
    IO_SetY(yETC_CResetLp  , IO_GetX(xETC_CResetSw ) || (m_bFlick && isErr)     );
    IO_SetY(yETC_RResetLp  , IO_GetX(xETC_RResetSw ) || (m_bFlick && isErr)     );
    IO_SetY(yETC_LAirLp    , IO_GetX(xETC_LAirSw   ) || IO_GetY(yETC_MainSol)   );
    IO_SetY(yETC_RAirLp    , IO_GetX(xETC_RAirSw   ) || IO_GetY(yETC_MainSol)   );
    IO_SetY(yETC_LInitLp   , IO_GetX(xETC_LInitSw  ) );
    IO_SetY(yETC_RInitLp   , IO_GetX(xETC_RInitSw  ) );

    //Center Man Button
    IO_SetY(yETC_CVisnLp   , IO_GetX(xETC_CVisnSw  ) );
    IO_SetY(yETC_CMarkLp   , IO_GetX(xETC_CMarkSw  ) );


    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if (bStartSw) {
        bool bAllArayNone = DM.ARAY[riLDR].CheckAllStat(csNone) && DM.ARAY[riLSR].CheckAllStat(csNone) && DM.ARAY[riPRB].CheckAllStat(csNone) &&
                            DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) && DM.ARAY[riVSN].CheckAllStat(csNone)  ;
        if(!isHomeEnd) {FM_MsgOk("WARNING" , "Please All Home"); bStartSw = false ; }
        if(LTQ.GetLotCnt() == 0 && bAllArayNone) {FM_MsgOk("WARNING" , "Please Lot Open"); bStartSw = false ; }
        if(LSR.m_bManMark) {FM_MsgOk("WARNING" , "Please Manual Mark UnChecked"); bStartSw = false ; }
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
    }

    if (bInitSw) {
        //sun sun InspectMainAir(void) 이거 관련해서 따져보기.
        MM.SetManCycle(mcAllHome);
    }

    if (bVisionSw) {
        //sun sun InspectMainAir(void) 이거 관련해서 따져보기.
        MM.SetManCycle(mcLSR_CycleWork);
    }

    if (bMarkingSw) {
        if(OM.CmnOptn.iMarkInsp == moNone){ FM_MsgTime("ERROR","MarkVision Option is None!",1000); }
        else                              { MM.SetManCycle(mcVSN_CycleVisn);                       }
    }

    /*
    if (bMarkingSw) {
        //sun sun InspectMainAir(void) 이거 관련해서 따져보기.
        MM.SetManCycle(mcVSN_CycleVisn);
    }
    */

    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
    }

    if (!IO_GetX(xPSB_IdxOvld ) ){
        EM_SetErr (eiPSB_IdxOvld );
        MT_Stop   (miPSB_XIndx);
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
        EM_SetDisp(true );
        TL_MuteSnd(false);
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
                                LSR.m_bAutoRun = true;
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
                            m_bReqStop = false ;

                            DM.SaveMap();

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{                                                                                         
    bool isOk = true ;

    if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainSol)) isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


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
    if (IO_GetX(xLDR_EmgSw) || IO_GetX(xULD_EmgSw) ) {
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
    Ret[piPRB] = PRB.ToStartCon();
    Ret[piRAL] = RAL.ToStartCon();
    Ret[piLSR] = LSR.ToStartCon();
    Ret[piVSN] = VSN.ToStartCon();
    Ret[piPSB] = PSB.ToStartCon();
    Ret[piLDR] = LDR.ToStartCon();
    Ret[piULD] = ULD.ToStartCon();

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
    Ret[piPRB] = PRB.ToStopCon();
    Ret[piRAL] = RAL.ToStopCon();
    Ret[piLSR] = LSR.ToStopCon();
    Ret[piVSN] = VSN.ToStopCon();
    Ret[piPSB] = PSB.ToStopCon();
    Ret[piLDR] = LDR.ToStopCon();
    Ret[piULD] = ULD.ToStopCon();

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
    Ret[piPRB] = PRB.ToStart();
    Ret[piRAL] = RAL.ToStart();
    Ret[piLSR] = LSR.ToStart();
    Ret[piVSN] = VSN.ToStart();
    Ret[piPSB] = PSB.ToStart();
    Ret[piLDR] = LDR.ToStart();
    Ret[piULD] = ULD.ToStart();


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
    Ret[piPRB] = PRB.ToStop();
    Ret[piRAL] = RAL.ToStop();
    Ret[piLSR] = LSR.ToStop();
    Ret[piVSN] = VSN.ToStop();
    Ret[piPSB] = PSB.ToStop();
    Ret[piLDR] = LDR.ToStop();
    Ret[piULD] = ULD.ToStop();

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
    Ret[piPRB] = PRB.Autorun();
    Ret[piRAL] = RAL.Autorun();
    Ret[piLSR] = LSR.Autorun();
    Ret[piVSN] = VSN.Autorun();
    Ret[piPSB] = PSB.Autorun();
    Ret[piLDR] = LDR.Autorun();
    Ret[piULD] = ULD.Autorun();

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


