//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Head.h"
#include "Stage.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SpcUnit.h"
#include "math.h"

#include "VisionMan.h"

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

//    SPL.Init(lrLeftBin );
//    SPR.Init(lrRightBin);
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

    IO_SetY(yETC_MainAirSol , true);

    Reset(true);

    iTempStep = 0 ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::Reset(bool _Option)
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

    if(_Option) AT_Reset   (); //위험해.

    //Error.
    EM_Clear();

    //Manual.
    MM.Reset();

    //Part.
    STG.Reset();
    HED.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();
    //TODO :: 이거 다 죽여라.    
    if(!MT_GetServoAll()) MT_SetServoAll(true);
    //if(OM.CmnOptn.bUseVcCoil) IO_SetY(yHED_VoiceCoilServo,true );
    //else                      IO_SetY(yHED_VoiceCoilServo,false);

    //랏 엔드시에 리셑해야 부져 꺼지게.
    //LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bDoorSkip) InspectDoor () ;
        if( m_iStep == scToStart) InspectHomeEnd () ; //로더 언로더 Y축 중간 중간 홈잡는 것 때문에.
        InspectLightGrid() ;
    }


    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_StartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw  = IO_GetXUp(xETC_StopSw ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw = IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw   = false ; //IO_GetXUp(xETC_LAirSw  ) || IO_GetXUp(xETC_RAirSw  ) || m_bBtnAir   ;
    bool bInitSw  = false ; //IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;
    //bool bLampSw  = IO_GetXUp(xETC_LampSw ) ;//|| IO_GetXUp(xETC_PstEqpReset);

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun);
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr));
    //IO_SetY(yETC_LampLp   , IO_GetX(xETC_LampSw  ) );

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //sun  20141022 이게 왜 이렇게 되어 있지?????
    //일단 지움.
    //if(IO_GetX(xETC_StopSw ) &&IO_GetX(xETC_ResetSw)) {
    //    DM.SaveMap();
    //}

    //Door Sensor.
    bool isAllCloseDoor = true ; //IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","자동모드로 변경!!"); return ;
    }

    if (bStartSw) {
        VSM.LiveAllOff(false);
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","초기화를 수행하세요!"); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
    }

    if (bStopSw) {
        if(MM.GetManNo() != mcNoneCycle) MM.m_bStop = true ;
        Trace("마지막업데이트", m_dLastTime.FormatString("mm-dd-hh-nn-ss-zzz").c_str());
        Trace("m_iTempDpCnt1", String(DM.m_iTempDpCnt1).c_str());
        Trace("m_iTempDpCnt2", String(DM.m_iTempDpCnt2).c_str());

//        MM.m_bStopTestTrm = true ;
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);
    }
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainAirSol , !IO_GetY(yETC_MainAirSol )) ;
//        IO_SetY(yETC_VacuSol , !IO_GetY(yETC_VacuSol )) ;
//        IO_SetY(yETC_MainSol2, !IO_GetY(yETC_MainSol )) ;
    }

    //if (bLampSw) {
    //    IO_SetY(yETC_LampOnOff,!IO_GetY(yETC_LampOnOff));
    //}

    //일단 여기 있어봐.
//    if(!m_bRun) if( IO_GetXUp(xCST_Detect) &&/*  IO_GetX(xCST_Detect) &&*/ DM.ARAY[riCST].CheckAllStat(csNone)) DM.ARAY[riCST].SetStat(csUnkwn);
//    if(!m_bRun) if( IO_GetXDn(xCST_Detect) &&/* !IO_GetX(xCST_Detect) &&*/ DM.ARAY[riCST].CheckAllStat(csWork)) DM.ARAY[riCST].SetStat(csNone );


//    if(!m_bRun) {
//        if( IO_GetXDn(xCST_Detect) ) {
//            if(FM_MsgYesNo("CONFIRM","CASSETTE DATA DELETE ?") == mrYes){
//                DM.ARAY[riCST].SetStat(csNone);
//            }
//        }
//        else if( IO_GetXUp(xCST_Detect) ) {
//            if(FM_MsgYesNo("CONFIRM","CASSETTE DATA INPUT ?") == mrYes){
//                DM.ARAY[riCST].SetStat(csUnkwn);
//            }
//        }
//    }



    //Err Log
    static bool bIsErr;
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
                                if(AutoRun()) {
                                    //랏엔드 상황.
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
                                    //랏엔드 상황.
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

                            DM.SaveMap(); //오퍼창 타이머로 이전 자꾸 롹됌.

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{                                                                                         
    bool isOk = true ;

    if(!IO_GetX(xETC_MainAir))  isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;                                                                         
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetXUp(xETC_EmgSwF) || IO_GetXUp(xETC_EmgSwL) || IO_GetXUp(xETC_EmgSwR)) {
        EM_SetErr(eiETC_Emergency);
        MT_SetServoAll(false);
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
    if (!IO_GetX(xETC_BackDoorL) || !IO_GetX(xETC_BackDoorR) ){EM_SetErr(eiETC_BDoor    ); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_FrntDoorL) || !IO_GetX(xETC_FrntDoorR) ){EM_SetErr(eiETC_FDoor    ); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_RightDoor)                             ){EM_SetErr(eiETC_RSideDoor); isOk = false;}
    if (!IO_GetX(xETC_LeftDoor )                             ){EM_SetErr(eiETC_LSideDoor); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(AT_Err(aiSTG_TrsCmpGR  )) {EM_SetErr(eiSTG_TrsCmpGRTO) ; isOk = false ; }
    if(AT_Err(aiHED_PenMrkDU  )) {EM_SetErr(eiHED_PenMrkDUTO) ; isOk = false ; }
    if(AT_Err(aiSTG_ChkTblUD  )) {EM_SetErr(eiSTG_ChkTblUDTO) ; isOk = false ; }
    if(AT_Err(aiHED_PenCoverUD)) {EM_SetErr(eiHED_PenCover  ) ; isOk = false ; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

    //Inspect Motor Alarm.
    for(int i=0; i<MAX_MOTR; i++) if (MT_GetAlarm   (i)){EM_SetErr((int)eiSTG_ZCstAlarm   + i ); isOk = false; }
    for(int i=0; i<MAX_MOTR; i++) if (MT_GetNLimSnsr(i)){EM_SetErr((int)eiSTG_ZCstNegLim  + i ); isOk = false; }
    for(int i=0; i<MAX_MOTR; i++) if (MT_GetPLimSnsr(i)){EM_SetErr((int)eiSTG_ZCstPosLim  + i ); isOk = false; }
//TODO ::    if(!IO_GetX(xHED_VoiceCoilSevo)) {EM_SetErr((int)eiHED_VoiceCoilSevo); isOk = false; }
    //if(OM.CmnOptn.bUseVcCoil) {if(!IO_GetY(yHED_VoiceCoilServo)) {EM_SetErr(eiHED_VoiceCoilSevo); isOk = false; }}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    for(int i=0; i<MAX_MOTR; i++) {
        if (!MT_GetHomeEnd   (i)){EM_SetErr((int)eiSTG_ZCstHomeEnd + i ); isOk = false; }
    }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------

bool  __fastcall CSequence::InspectCrash(void)
{
/*
    if(IO_GetX(xEJT_Crash_Sensor)) {
        MT_EmgStop(miWRD_XStg);
        MT_EmgStop(miWRD_YStg);
        EM_SetErr(eiSTG_EjectorCrash);
        MT_SetServo(miWRD_XStg , false );
        MT_SetServo(miWRD_YStg , false );
        AT_MoveCyl(aiEJT_UpDn , ccBwd);
        return false ;
    }

    bool isEjtUp = !AT_Complete(aiEJT_UpDn , ccBwd);

    if(!isEjtUp) return true ;

    //ejector
    double dPowActDia = pow((OM.MstOptn.dStgDia - OM.MstOptn.dEjtDia)/2.0 , 2 );
    //double dPowPosX   = pow(PM.GetValue(_iMotr,_iPstnId) - OM.MstOptn.dStgXEjtCt,2);

    double dPowCmdX   = pow(MT_GetCmdPos(miWRD_XStg) - OM.MstOptn.dStgXEjtCt ,2);
    double dPowEncX   = pow(MT_GetEncPos(miWRD_XStg) - OM.MstOptn.dStgXEjtCt ,2);
    double dPowCmdY   = pow(MT_GetCmdPos(miWRD_YStg) - OM.MstOptn.dStgYEjtCt ,2);  //
    double dPowEncY   = pow(MT_GetEncPos(miWRD_YStg) - OM.MstOptn.dStgYEjtCt ,2);  //

    if( dPowActDia < dPowEncX + dPowEncY ) {
        //조그에서도 잡기 위해..
        AT_MoveCyl(aiEJT_UpDn , ccBwd);
        MT_EmgStop(miWRD_XStg);
        MT_EmgStop(miWRD_YStg);
        EM_SetErr(eiSTG_EjectorSoftLimit);
        return false ;
    }
*/
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
    bool Ret0 , Ret1 ;
    //Call ToStop.
    Ret0 = HED.ToStartCon();
    Ret1 = STG.ToStartCon();

    //Ok.
    return (Ret0 && Ret1);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1;
    //Call ToStop.
    Ret0 = HED.ToStopCon();
    Ret1 = STG.ToStopCon();

    //Ok.
    return (Ret0 && Ret1);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1;
    //Call ToStop.
    Ret0 = HED.ToStart();
    Ret1 = STG.ToStart();

    //Ok.
    return (Ret0 && Ret1);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 ;
    //Call ToStop.
    Ret0 = HED.ToStop();
    Ret1 = STG.ToStop();

    //Ok.
    return (Ret0 && Ret1);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 ;
    //Call ToStop.
    Ret0 = HED.Autorun();
    Ret1 = STG.Autorun();

    //Ok.
    return (Ret0 && Ret1);
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit        =  MM.GetManNo() == mcAllHome ;
    bool isError       =  EM_IsErr() ;
    bool isRunning     =  m_bRun     ;
    bool isStop        = !m_bRun     ;
    bool isLotEnd      =  LT.m_bLotEnd ;
    bool isWarning     =  HED.m_bNeedOriDataSet ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Sequence State. Tower Lamp
         if (isInit    ) { m_iSeqStat = ssInit       ;}
    else if (isLotEnd  ) { m_iSeqStat = ssWorkEnd    ;}
    else if (isError   ) { m_iSeqStat = ssError      ;}
    else if (isRunning ) { m_iSeqStat = ssRunning    ;}
    else if (isWarning ) { m_iSeqStat = ssWarning    ;}
    else if (isStop    ) { m_iSeqStat = ssStop       ;}
    else                 { m_iSeqStat = m_iSeqStat   ;}

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


    //뻑테스트
    //if(DM.ARAY[riSTG].GetCntStat(csUnkwn)) DM.ARAY[riSTG].SetStat(csEmpty);
    //else                                   DM.ARAY[riSTG].SetStat(csUnkwn);

    //DM.ARAY[riSTG].ChangeStat()

    //Check Button.
    UpdateSeqState();

    //Update Actuator (Input).
    LT.Update(m_iSeqStat);

    //Ten Key.
//    ADL.Update();

    //Update Motor State (Input).
    MM.Update();

    //Update PstEqp State

    InspectTopLight();





    if(iTempStep != 0) {
        switch(iTempStep){
            case 10 : VSM.Grab(viWfr);
                      iTempStep++;
                      break ;

            case 11 : if(!VSM.GetGrabEnd(viWfr)) break ;
                      VSM.Inspect(viWfr , false);
                      iTempStep=10;
                      break ;
        }
    }

    if(bShowErr){
        EM_SetDisp(true);
        EM_SetErr(eiETC_AllHomeTO);
        bShowErr = false ;
    }

}

//---------------------------------------------------------------------------
void __fastcall CSequence::InspectTopLight(void)
{
    bool bCrntDoor = IO_GetX(xETC_FrntDoorL) && IO_GetX(xETC_FrntDoorR) &&
                     IO_GetX(xETC_BackDoorL) && IO_GetX(xETC_BackDoorR) &&
                     IO_GetX(xETC_LeftDoor ) && IO_GetX(xETC_RightDoor);

    static bool bPreDoor = bCrntDoor ;

    if(!bPreDoor && bCrntDoor) {
        IO_SetY(yETC_TopLight , false);
    }

    if(bPreDoor && !bCrntDoor) {
        IO_SetY(yETC_TopLight , true);
    }

    bPreDoor = bCrntDoor ;

}
