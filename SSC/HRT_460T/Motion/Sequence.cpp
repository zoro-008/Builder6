//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include "FormMain.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuffer.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "SPCUnit.h"
#include "TempConUnit.h"
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
}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void CSequence::Init(void)
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

    m_pPart[piLDR] = &LDR;
    m_pPart[piPRB] = &PRB;
    m_pPart[piWRK] = &WRK;
    m_pPart[piPSB] = &PSB;
    m_pPart[piULD] = &ULD;

}

void CSequence::Close()      
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
    //MAN.Reset();

    //Part.
    LDR.Reset();
    PRB.Reset();
    WRK.Reset();
    PSB.Reset();
    ULD.Reset();
    LOT.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

    MT_SetServoAll(true);
    TL_MuteSnd(true);

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bIgnrHeatAlm  ) InspectTemp    () ;
        if(!OM.CmnOptn.bIgnrDoor     ) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

//    if(!OM.CmnOptn.bIgnrHeatAlm  ) InspectTemp    () ;

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_LStartSw) || IO_GetXUp(xETC_RStartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw  = IO_GetXUp(xETC_LStopSw)  || IO_GetXUp(xETC_RStopSw)  || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
//    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;
    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw   = IO_GetXUp(xETC_LAirSw  ) || IO_GetXUp(xETC_RAirSw  ) || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw ) ||  m_bRun);

    if(IO_GetX(xETC_RStartSw ) ||  m_bRun) {
        IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);
    }
    else {
        IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);
    }

    IO_SetY(yETC_LStopLp   , IO_GetX(xETC_LStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_RStopLp   , IO_GetX(xETC_RStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LResetLp  , IO_GetX(xETC_LResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_RResetLp  , IO_GetX(xETC_RResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_LAirLp    , IO_GetX(xETC_LAirSw   ) || IO_GetY(yETC_MainSol));
    IO_SetY(yETC_RAirLp    , IO_GetX(xETC_RAirSw   ) || IO_GetY(yETC_MainSol));
    IO_SetY(yETC_LInitLp   , IO_GetX(xETC_LInitSw  ));
    IO_SetY(yETC_RInitLp   , IO_GetX(xETC_RInitSw  ));


    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_DoorFr) || IO_GetX(xETC_DoorRr);

    //Init. Button Flags.
    if (bStartSw) {
//        if(FrmMain -> tmMainSet -> Enabled || FrmMain -> tmSubSet -> Enabled) {FM_MsgOk("ERR","Probe Setting Now. Wait for a Second."); return ;}
        if(!OM.MstOptn.bNotUstLot) if(!LOT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        if(OM.EqpOptn.bExistUnLoader) { ULD.Reset(); }
//        if(!IO_GetX(xETC_PostEqpReady))EM_SetErr(eiETC_PostEqpNotReady);
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);

    }

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
    if (isErr && bStopSw) TL_MuteSnd(false);

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
              FM_SetLevel(lvOperator) ;
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
}
void  __fastcall CSequence::UpdateRunStep   (void)
{

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
                                    if(!OM.MstOptn.bNotUstLot) LOT.LotEnd();
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
                                    //if(!OM.MstOptn.bNotUstLot) LOT.LotEnd();
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

    int  iTempErr;
    int  iTempCnt = 8;
    int  iMaxTemp = 3;

    for(int i = 0 ; i < iTempCnt ; i++){ //i < iTempCnt = 8번째 온도 컨트롤러는 커버라서 이상하게 나옴.
        iTempErr = TCU.GetSetTemp(i) - TCU.GetCrntTemp(i);
        if(TCU.GetCrntTemp(i) > 200 + iMaxTemp )                              {TCU.SetTemp(i , 200); EM_SetErr(eiETC_TempAlrm); isOk = false ; }
        if(iTempErr > OM.MstOptn.iTempOfs || iTempErr < -OM.MstOptn.iTempOfs) {                      EM_SetErr(eiETC_TempAlrm); isOk = false ; }

    }
    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetX(xETC_LEmgLDRSw) || IO_GetX(xETC_EmgSw1) || IO_GetX(xETC_EmgSw2) ) {
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
    if (!IO_GetX(xETC_DoorFr) ){EM_SetErrMsg(eiETC_Door, "Front"); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_DoorRr) ){EM_SetErrMsg(eiETC_Door, "Rear" ); isOk = false;} //Front Door Sensor

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
        if(AT_Err(i)) {EM_SetErrMsg(eiATR_TimeOut , AT_GetName(i)) ; isOk = false ; }
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
bool  __fastcall CSequence::InspectCrash(void)
{
    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripDispr(void)
{
    bool isOk  = true;
    //
//  //       if (!DM.ARAY[riPR2].CheckAllStat(csNone) && !IO_GetX(xPRB_3Pkg) ) return false ;
    //if(DM.ARAY[riWRK].GetCntExist() && (!IO_GetX(xWRK_Detect1) && !IO_GetX(xWRK_Detect2))  && !OM.CmnOptn.bDryRun )  {FM_MsgOk("Err" , "WorkZone Strip Not Detected"); isOk = false ;}
    //if(DM.ARAY[riPSB].GetCntExist() && !OM.CmnOptn.bDryRun) {
    //    if(!IO_GetX(xWRK_Detect2) && !IO_GetX(xPSB_Pkg1) && !IO_GetX(xPSB_Pkg2) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) {FM_MsgOk("Err" , "PostBuffer Strip Clear"); isOk = false ;}
    //}
//  //  else if ( DM.ARAY[riPS2].GetCntExist() && (!IO_GetX(xPSB_1Pkg) && !IO_GetX(xPSB_2Pkg) && !IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) && !OM.CmnOptn.bDryRun) return false ;
    //
    //if (OM.EqpOptn.bExistLoader  ) { if(!DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) ) ) isOk = false ; }
    //if (OM.EqpOptn.bExistUnLoader) { if(!DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) ) isOk = false ; }
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
    bool bRet[MAX_PART];
    //Call ToStop.


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
    bool bRet[MAX_PART];
    //Call ToStop.
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
    //Call ToStop.
    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStop();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

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
    bool isError   =  EM_IsErr()             ;
    bool isRunning =  m_bRun                 ;
    bool isStop    = !m_bRun && !LOT.m_bLotEnd;
    bool isLotEnd  =  LOT.m_bLotEnd           ;

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

    //Update RunningStep.
    UpdateRunStep();

    //Update Actuator (Input).
    LOT.Update(m_iSeqStat);

    //Ten Key.
    //ADL.Update();

    //Update Motor State (Input).
    MM.Update();

    //Update PstEqp State

}


