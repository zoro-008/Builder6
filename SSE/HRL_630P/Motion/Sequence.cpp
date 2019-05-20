//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
//---------------------------------------------------------------------------
#include "FrontFeed.h"
#include "LearFeed.h"
#include "FrontLaser.h"
#include "LearLaser.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
//#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "SPCUnit.h"
#include "LaserConUnit.h"

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

    m_pPart[piFFD] = &FFD ;
    m_pPart[piLFD] = &LFD ;
    m_pPart[piFLS] = &FLS ;
    m_pPart[piLLS] = &LLS ;
}

void __fastcall CSequence::Close(void)
{
}

//---------------------------------------------------------------------------
void CSequence::Reset(void)
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
    for(int i=0; i < MAX_PART; i++){
        m_pPart[i] -> Reset();
    }



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
        if(!OM.CmnOptn.bIgnrDoorOpen) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
        //IO_SetY(yETC_Feeder , true);
    }
    //else{
    //    IO_SetY(yETC_Feeder , false);
    //}

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_StartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw  = IO_GetXUp(xETC_StopSw ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw = IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bInitSw  = IO_GetXUp(xETC_InitSw )  ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun);

    //버튼 클릭시나
    //IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw ) ||  m_bRun);
    //IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);

    if(IO_GetX(xETC_StartSw ) ||  m_bRun) {
        IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun);
    }

    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_InitLp   , IO_GetX(xETC_InitSw  ));


    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_LearDoor) || IO_GetX(xETC_FrntDoor) || IO_GetX(xETC_SideDoor);

    //Init. Button Flags.
    if (bStartSw) {
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);

    }

    //Buzzer Off.
    if (isErr && bStopSw) TL_MuteSnd();

    //Set Condition Flags.
    bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcNone ;
    bool isResetCon = bResetSw && !m_bRun ;

    //Run.
    if (isRunCon && !m_iStep) {
        if(!LTQ.GetLotOpen()){
            AnsiString Msg ;
            Msg.sprintf("LOT NOT OPEN" ,"Lot is not Opened" );
            Trace("SEQ",Msg.c_str() );
            EM_SetErr(eiETC_LotOpen);
            m_iStep = scIdle;
            m_bRun = false;
        }
        else {
            m_iStep = scToStartCon ;
            EM_SetDisp(true);
        }
    }

    if (!MT_GetHomeEndAll) {
        FM_MsgTime("WARNING","Please All Home" , 1000);
        m_iStep = scIdle;
        m_bRun = false;
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
                            IO_SetY(yETC_Feeder , true);
                            return ;

        case scToStart    : if(!ToStart()) return ;
                            m_bRun = true ;
                            m_iStep=scRun ;
                            return ;

        case scRun        : if(!m_bReqStop) {
                                if(Autorun()) {
                                    //랏엔드 상황.
                                    //if(!OM.MstOptn.bNotUstLot) LT.LotEnd();
                                    m_iStep=scToStopCon ;
                                }
                                return ;
                            }
                            //IO_SetY(yETC_Feeder , true); //JH 지켜보자..
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            return ;

        case scToStopCon :  if(!ToStopCon()){
                                if(Autorun()) {
                                  //랏엔드 상황.
                                  //if(!OM.MstOptn.bNotUstLot) LT.LotEnd();
                                }
                                return ;
                            }

                            m_bRun = false ;
                            m_iStep=scToStop;
                            return ;

        case scToStop    :  if (!ToStop()) return ;
                            IO_SetY(yETC_Feeder , false);  //JH 지켜보자..
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

    //if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainSol)) isOk = false ;

    //잠시 주석 SPC 테스트...if(!isOk) EM_SetErr(eiETC_MainAir) ;


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

    //Check Door
    if (!IO_GetX(xETC_FrntDoor) ){EM_SetErr(eiETC_FDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_SideDoor) ){EM_SetErr(eiETC_SDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_LearDoor) ){EM_SetErr(eiETC_BDoor); isOk = false;}
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
    if(!isOk){
        MT_SetHomeEnd(miFED_YFrt , false);
        MT_SetHomeEnd(miFED_YLer , false);
        MT_SetHomeEnd(miLSR_TFrt , false);
        MT_SetHomeEnd(miLSR_TLer , false);
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
    //if (MT_GetAlarm(miLDR_YCmp)){EM_SetErrMsg(eiMTR_Alarm , MT_GetName(miLDR_YCmp)); isOk = false; }
    //if (MT_GetAlarm(miLDR_ZCmp)){EM_SetErrMsg(eiMTR_Alarm , MT_GetName(miLDR_ZCmp)); isOk = false; }
    //if (MT_GetAlarm(miULD_YCmp)){EM_SetErrMsg(eiMTR_Alarm , MT_GetName(miULD_YCmp)); isOk = false; }
    //if (MT_GetAlarm(miULD_ZCmp)){EM_SetErrMsg(eiMTR_Alarm , MT_GetName(miULD_ZCmp)); isOk = false; }

    for(int i = 0 ; i < MAX_MOTR ; i++) {
        if (MT_GetAlarm   (i)                  ){EM_SetErrMsg(eiMTR_Alarm  , MT_GetName(i)); isOk = false; }
        if (MT_GetNLimSnsr(i)&&MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_NegLim , MT_GetName(i)); isOk = false; }
        if (MT_GetPLimSnsr(i)&&MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_PosLim , MT_GetName(i)); isOk = false; }
    }
    //Ok.
    return isOk;
}
//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectOverLoad()
{
    //Local Var.
    bool isOk = true;
    /*
    //Inspect.
    if (IO_GetX(xFWK_PshOvld)){
        EM_SetErrMsg(eiFWK_FrntPshOvld , MT_GetName(miFED_TIdx));
        MT_Stop(miTRS_YStc);
        MT_Stop(miFWK_XIdx);
        MT_Stop(miBWK_XIdx);
        isOk = false;
    }
    if (IO_GetX(xBWK_PshOvld)){
        EM_SetErrMsg(eiBWK_BackPshOvld , MT_GetName(miTRS_YStc));
        MT_Stop(miTRS_YStc);
        MT_Stop(miFWK_XIdx);
        MT_Stop(miBWK_XIdx);
        isOk = false;
    }
    */
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
    /*
    double dATPCmd = MT_GetCmdPos(miATP_YPkr);
    double dATPTrg = MT_GetTrgPos(miATP_YPkr);//GetMotrPos(_iMotr , _iPstnId );GetMotrPos(miATP_YPkr , _iPstnId );
    double dPCPCmd = MT_GetCmdPos(miPCP_YPkr);
    double dPCPTrg = MT_GetTrgPos(miPCP_YPkr);//GetMotrPos(_iMotr , _iPstnId );

    bool bCrash1 = dATPCmd + dATPCmd > OM.MstOptn.dToolCrashStroke ;
    bool bCrash2 = dATPTrg + dATPCmd > OM.MstOptn.dToolCrashStroke ;
    bool bCrash3 = dATPCmd + dATPTrg > OM.MstOptn.dToolCrashStroke ;
    bool bCrash4 = dATPTrg + dATPTrg > OM.MstOptn.dToolCrashStroke ;

    if(bCrash1 || bCrash2 || bCrash3 ||  bCrash4) {
        EM_SetErr(eiPCP_ToolMoveToCrash);
        MT_SetServo(miATP_YPkr , false);
        MT_SetServo(miPCP_YPkr , false);
        return false ;
    }

    //테스트해보자.
    if(IO_GetX(xPCP_PkrCrash)) {
        MT_EmgStop(miATP_YPkr);
        MT_EmgStop(miPCP_YPkr);
        if(MT_GetStop(miATP_YPkr) && MT_GetStop(miPCP_YPkr)) {
            MT_SetServo(miATP_YPkr,false);
            MT_SetServo(miPCP_YPkr,false);
            EM_SetErr(eiPCP_ToolCrash);
        }
    }
    */
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
bool CSequence::ToStartCon(void)
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
bool CSequence::ToStopCon(void)
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
bool CSequence::ToStart(void)
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
bool CSequence::ToStop(void)
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
bool CSequence::Autorun(void)
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
    bool isRunning =  m_bRun     ;
    bool isStop    = !m_bRun     ;
    bool isLotEnd  = !LTQ.GetLotOpen() ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    ////Set Sequence State. Tower Lamp
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
    Trace("<@SETBUFF_10>","1");
    SM_Update(m_iSeqStat);
    Trace("<@SETBUFF_10>","2");
    //Inspection.
    InspectMainAir  () ;
    InspectEmergency() ;
    InspectActuator () ;
    InspectMotor    () ;
    InspectCrash    () ;
    InspectOverLoad () ;
    Trace("<@SETBUFF_10>","3");

    //Update ErrorProc.
    UpdateButton();
    Trace("<@SETBUFF_10>","4");

    //Check Button.
    UpdateSeqState();
    Trace("<@SETBUFF_10>","5");

    //Update Actuator (Input).
    //LTQ.Update(m_iSeqStat);

    //Ten Key.
    //ADL.Update();

    //Update Motor State (Input).
    MM.Update();
    Trace("<@SETBUFF_10>","6");

    //SPC
    SPC.Update(m_iSeqStat);
    Trace("<@SETBUFF_10>","7");

    LCU.Update();
    Trace("<@SETBUFF_10>","0");

}


