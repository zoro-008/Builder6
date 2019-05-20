//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
//---------------------------------------------------------------------------
#include "UserFile.h"

#include "UtilDefine.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "LaserConUnit.h"
//#include "PstnMan.h"
#include "math.h"


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
    m_bBtnReset  = false ;
    m_bBtnStart  = false ;

    m_bRun       = false ;
    m_bAutoMode  = false ; //jh
    m_bFlick     = false ;
    m_iStep      = scIdle;
    m_iSeqStat   = ssStop;

    // IO 추가 되는거 확인 한 다음에...jh
    //IO_SetY(yETC_MainAir , true);
    //IO_SetY(yETC_MainAir , true);
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

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();
    //TODO :: 이거 다 죽여라.    
    if(!MT_GetServoAll()) MT_SetServoAll(true);
    //if(OM.CmnOptn.bUseVcCoil) IO_SetY(yHED_VoiceCoilServo,true );
    //else                      IO_SetY(yHED_VoiceCoilServo,false);

    //랏 엔드시에 리셑해야 부져 꺼지게.
    LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    if(!OM.CmnOptn.bDoorSkip && m_bRun) InspectDoor () ;

    //if(MT_GetServo(miSTG_TBase , true)) { mt_set }

    bool bStartSw  = m_bBtnStart || (IO_GetX(xETC_StartSwL) && IO_GetX(xETC_StartSwR));
    bool bResetSw  = m_bBtnReset || (IO_GetX(xETC_StartSwL) || IO_GetX(xETC_StartSwR));
    bool bRotateSw = m_bBtnRotate ;
    bool bHomeSw   = m_bBtnHome ;

    static bool bPreStartSw  = false ;
    static bool bPreResetSw  = false ;
    static bool bPreRotateSw = false ;
    static bool bPreHomeSw   = false ;

    //실제는 이놈들만 가져다 쓰면됌.
    //bool bStartCon  = bStartSw  || bPreStartSw  ;
    //bool bResetCon  = bResetSw  || bPreResetSw  ;
    //bool bRotateCon = bRotateSw || bPreRotateSw ;
    //bool bHomeSwCon = bHomeSw   || bPreHomeSw   ;

    bool bStartCon  = bStartSw  && !bPreStartSw  ;
    bool bResetCon  = bResetSw  && !bPreResetSw  ;
    bool bRotateCon = bRotateSw && !bPreRotateSw ;
    bool bHomeSwCon = bHomeSw   && !bPreHomeSw   ;



    bPreStartSw  = bStartSw  ;
    bPreResetSw  = bResetSw  ;
    bPreRotateSw = bRotateSw ;
    bPreHomeSw   = bHomeSw   ;

    //스타트나 메뉴얼이 클리어 한사이클에 안되면 병신짓 할 우려가 있음.
    //if(bStartCon ) m_bBtnStart = false ;
    //if(bResetCon ) m_bBtnStop  = false ;
    //if(bRotateCon) m_bBtnReset = false ;
    //if(bHomeSwCon) m_bBtnAir   = false ;

    m_bBtnStart  = false ;
    m_bBtnReset  = false ;
    m_bBtnRotate = false ;
    m_bBtnHome   = false ;

    //Door Sensor.
    bool isAllCloseDoor = true ; //IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    //if(bStartSw && !m_bAutoMode) {
    //    FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    //}

    //Err Log
    static bool bPreErr;
    bool isErr = EM_IsErr() ;
    if(isErr && !bPreErr) {

    }
    if(!isErr && bPreErr) {
        //Buzzer Off.
        TL_MuteSnd();

    }
    bPreErr = isErr ;


    //Set Condition Flags.
    //bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isHomeCon   = bHomeSwCon && !isErr && MM.GetManNo() == mcNone ;
    bool isRotateCon = bRotateCon && !isErr && MM.GetManNo() == mcNone ;
    bool isResetCon  = bResetCon  &&  MM.GetManNo() == mcNone ;
    bool isStartCon  = bStartCon  && !isErr && MM.GetManNo() == mcNone ;

    if (isHomeCon ) {
        if(SEQ.m_iSeqStat == ssInit) {
            FM_MsgOk("Warning" , "아직 홈 동작 중입니다." );
        }
        else {
            EM_SetDisp(true);
            MM.SetManCycle(mcAllHome);
        }
    }

    if (isRotateCon ) {
        EM_SetDisp(true);
        if(!OM.CmnOptn.bNotUsedVAxis){
            if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","모든 홈 동작을 실행 해 주세요.!"); return ;}
        }
        else {
            if( IO_GetX(xSTG_WrkPstDetect) && (!MT_GetServo(miSTG_TBase) && !MT_GetServo(miWRK_UBrd1) && !MT_GetServo(miWRK_UBrd2))){
                FM_MsgOk("ERR","모든 홈 동작을 실행 해 주세요.!"); return ;
            }
        }
        MM.SetManCycle(mcCycleRotate);

        if(STG.m_bNotWrkPstn ){ // JH
                FM_MsgOk("Warning" , "베이스 판을 워크포지션으로 옮겨주세요." );
                MT_SetServo(miSTG_TBase , false);
        }

    }

    if (isResetCon ) {
        Reset();
    }

    //Run.
    if (isStartCon) {
        if(SEQ.m_iSeqStat == ssRunning) {
            FM_MsgOk("Warning" , "아직 동작 중입니다." );
        }
        else {
        //에러 디스플레이 되게.
            EM_SetDisp(true);
            if(!OM.CmnOptn.bNotUsedVAxis){
                if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","모든 홈 동작을 실행 해 주세요.!"); return ;}
            }
            else {
                if( IO_GetX(xSTG_WrkPstDetect) && (!MT_GetServo(miSTG_TBase) && !MT_GetServo(miWRK_UBrd1) && !MT_GetServo(miWRK_UBrd2))){
                    FM_MsgOk("ERR","모든 홈 동작을 실행 해 주세요.!"); return ;
                }
            }
            MM.SetManCycle(mcCycleStart);

            if(STG.m_bNotWrkPstn ){ // JH
                FM_MsgOk("Warning" , "베이스 판을 워크포지션으로 옮겨주세요." );
                MT_SetServo(miSTG_TBase , false);
            }

        }
    }
    //if (isStopCon  &&  m_iStep) m_bReqStop = true ;



}

/*
//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{

    //Check Inspect.
    if(m_iStep != scIdle) {
        if( m_iStep == scToStart) InspectHomeEnd () ; //로더 언로더 Y축 중간 중간 홈잡는 것 때문에.
        InspectLightGrid() ;
    }


    //Local Var.
    bool isErr    = EM_IsErr() ;


    m_tmTemp.Clear();
    bool bErrCon  = isErr && (m_tmTemp.OnDelay(true , 1500) && (IO_GetX(xETC_StartSwL) || IO_GetX(xETC_StartSwR))) ;

    bool bStartSw = (IO_GetX(xETC_StartSwL) && IO_GetX(xETC_StartSwR)) || m_bBtnStart ;
    bool bResetSw =  bErrCon || m_bBtnReset || (IO_GetX(xETC_StartSwL) || IO_GetX(xETC_StartSwR)) ; //isErr && (IO_GetX(xETC_StartSwL) || IO_GetX(xETC_StartSwR) || m_bBtnReset) ;

    bool bAirSw   = false ;
    bool bInitSw  = false ;
    //bool bLampSw  = IO_GetXUp(xETC_LampSw ) ;//|| IO_GetXUp(xETC_PstEqpReset); IO 추가한 다음에 확인...JH

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    //IO_SetY(yETC_StartLpL , IO_GetX(xETC_StartSwL) ||  m_bRun);
    //IO_SetY(yETC_StartLpR , IO_GetX(xETC_StartSwR) ||  m_bRun);
    //IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick);
    //IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr));

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;



    //Door Sensor.
    bool isAllCloseDoor = true ; //IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    //if(bStartSw && !m_bAutoMode) {
    //    FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    //}

    if (bStartSw) {
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","모든 홈 동작을 실행 해 주세요.!"); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        //if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
    }

    //if (bStopSw) {
    //    if(MM.GetManNo() != mcNoneCycle) MM.m_bStop = true ;
    //    Trace("마지막업데이트", m_dLastTime.FormatString("mm-dd-hh-nn-ss-zzz").c_str());
    //}

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);
    }
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
   //     IO_SetY(yETC_MainAir , !IO_GetY(yETC_MainAir )) ;
    } //IO 추가한 다음에 확인...JH

    //if (bLampSw) {
    //    IO_SetY(yETC_LampOnOff,!IO_GetY(yETC_LampOnOff));
    //} IO 추가한 다음에 확인...JH


    //Err Log
    static bool bIsErr;
    isErr = EM_IsErr() ;
    if(isErr && !bIsErr) {
        bIsErr = true ;
    }
    if(!isErr && bIsErr) {
        bIsErr = false ;
    }

    //Buzzer Off.
    if (isErr) TL_MuteSnd();

    //Set Condition Flags.
    //bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcCycleStart ;
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

    //if (isStopCon  &&  m_iStep) m_bReqStop = true ;
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
                                MM.SetManCycle(mcCycleStart);
                                //if(AutoRun()) {
                                //랏엔드 상황.
                                LT.LotEnd();
                                m_iStep=scToStopCon ;

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

                            return;
        }
}
*/

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{                                                                                         
    bool isOk = true ;
    //if(!IO_GetX(xETC_MainAir))  isOk = false ;

    //if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetXUp(xETC_EmgSwL)) {
        EM_SetErr(eiETC_EmergencyL);
        MT_SetServoAll(false);
        isOk = false ;
    }

    if (IO_GetXUp(xETC_EmgSwR)) {
        EM_SetErr(eiETC_EmergencyR);
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

    if (!IO_GetX(xETC_Door1) || !IO_GetX(xETC_Door2) ){EM_SetErr(eiETC_Door);
    isOk = false;} //Front Door Sensor

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;
    /*
    //Inspect.
    if(AT_Err(aiLDR_Stopper)) {EM_SetErr(eiLDR_Stopper   ) ; isOk = false ; }
    if(AT_Err(aiLDR_MgzGrip)) {EM_SetErr(eiLDR_MgzGrip   ) ; isOk = false ; }
    if(AT_Err(aiLDR_Out    )) {EM_SetErr(eiLDR_Out       ) ; isOk = false ; }
    if(AT_Err(aiLDR_Pusher )) {EM_SetErr(eiLDR_Pusher    ) ; isOk = false ; }
    if(AT_Err(aiPRB_Stopper)) {EM_SetErr(eiPRB_Stopper   ) ; isOk = false ; }
    if(AT_Err(aiPRB_Clamp  )) {EM_SetErr(eiPRB_Clamp     ) ; isOk = false ; }
    //if(AT_Err(aiPRB_CAlign )) {EM_SetErr(eiPRB_CAlign    ) ; isOk = false ; }
    //if(AT_Err(aiWRE_BAlign )) {EM_SetErr(eiWRE_BAlign    ) ; isOk = false ; }
    if(AT_Err(aiWRE_Clamp  )) {EM_SetErr(eiWRE_Clamp     ) ; isOk = false ; }
    //if(AT_Err(aiWRE_CAlign )) {EM_SetErr(eiWRE_CAlign    ) ; isOk = false ; }
    if(AT_Err(aiWRD_Clamp  )) {EM_SetErr(eiWRD_Clamp     ) ; isOk = false ; }
    //if(AT_Err(aiWRD_CAlign )) {EM_SetErr(eiWRD_CAlign    ) ; isOk = false ; }
    if(AT_Err(aiPSB_Clamp  )) {EM_SetErr(eiPSB_Clamp     ) ; isOk = false ; }
    if(AT_Err(aiEJT_UpDn   )) {EM_SetErr(eiEJT_UpDn      ) ; isOk = false ; }
    if(AT_Err(aiTRS_Chuck  )) {EM_SetErr(eiTRS_Chuck     ) ; isOk = false ; }
    if(AT_Err(aiULD_Stopper)) {EM_SetErr(eiULD_Stopper   ) ; isOk = false ; }
    if(AT_Err(aiULD_MgzGrip)) {EM_SetErr(eiULD_MgzGrip   ) ; isOk = false ; }
    if(AT_Err(aiULD_Out    )) {EM_SetErr(eiULD_Out       ) ; isOk = false ; }
      */
    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

    //Inspect Motor Alarm.
    //모터 없애는것떔에 옛날방식으로 다시 감.JS
    if(!OM.CmnOptn.bNotUsedVAxis){
        for(int i=0; i<MAX_MOTR; i++) if (MT_GetAlarm   (i)){EM_SetErr((int)eiSTG_VBrd1Alarm   + i ); isOk = false; }
        for(int i=0; i<MAX_MOTR; i++) if (MT_GetNLimSnsr(i)){EM_SetErr((int)eiSTG_VBrd1NegLim  + i ); isOk = false; }
        for(int i=0; i<MAX_MOTR; i++) if (MT_GetPLimSnsr(i)){EM_SetErr((int)eiSTG_VBrd1PosLim  + i ); isOk = false; }
        for(int i=0; i<MAX_MOTR; i++) if (!MT_GetServo  (i)) MT_SetHomeEnd(i, false);
    }
    else{
        if (MT_GetAlarm   (miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd1Alarm); isOk = false; }
        if (MT_GetAlarm   (miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd2Alarm); isOk = false; }
        if (MT_GetAlarm   (miSTG_TBase)){EM_SetErr((int)eiSTG_TBaseAlarm); isOk = false; }
        if (MT_GetNLimSnsr(miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd1NegLim); isOk = false; }
        if (MT_GetNLimSnsr(miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd2NegLim); isOk = false; }
        if (MT_GetNLimSnsr(miSTG_TBase)){EM_SetErr((int)eiSTG_TBaseNegLim); isOk = false; }
        if (MT_GetPLimSnsr(miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd1PosLim); isOk = false; }
        if (MT_GetPLimSnsr(miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd2PosLim); isOk = false; }
        if (MT_GetPLimSnsr(miSTG_TBase)){EM_SetErr((int)eiSTG_TBasePosLim); isOk = false; }
        if (!MT_GetServo  (miWRK_UBrd1)) MT_SetHomeEnd(miWRK_UBrd1, false);
        if (!MT_GetServo  (miWRK_UBrd1)) MT_SetHomeEnd(miWRK_UBrd1, false);
        if (!MT_GetServo  (miSTG_TBase)) MT_SetHomeEnd(miSTG_TBase, false);
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
    if(!OM.CmnOptn.bNotUsedVAxis) {
        for(int i=0; i<MAX_MOTR; i++) if (!MT_GetHomeEnd   (i)){EM_SetErr((int)eiSTG_TBaseHomeEnd + i ); isOk = false;}
    }
    else {
        if (!MT_GetHomeEnd(miWRK_UBrd1)){EM_SetErr((int)eiSTG_UBrd1HomeEnd); isOk = false;}
        if (!MT_GetHomeEnd(miWRK_UBrd2)){EM_SetErr((int)eiSTG_UBrd2HomeEnd); isOk = false;}
        if (!MT_GetHomeEnd(miSTG_TBase)){EM_SetErr((int)eiSTG_TBaseHomeEnd); isOk = false;}
    }
    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectCrash(void)
{
    /*
    하드웨어 걸기...
    */

  /* if(IO_GetX(xEJT_Crash_Sensor)) {
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
    } */
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
    bool bRet;
    //Call ToStop.
    bRet = STG.ToStartCon();

    //Ok.
    return bRet ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool bRet;
    //Call ToStop.
    bRet = STG.ToStopCon();

    //Ok.
    return bRet ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool bRet;
    //Call ToStop.

    if(!IO_GetX(xSTG_WrkPstDetect)) {
        FM_MsgOk("ERR","베이스 판을 워크 위치로 돌려 주세요.");
        return bRet = false;
    }
    else {
        bRet = STG.ToStart();
    }

    //Ok.
    return bRet ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool bRet;
    //Call ToStop.
    bRet = STG.ToStop();

    //Ok.
    return bRet ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
   /* bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 , Ret7 , Ret8;
    //Call ToStop.
    Ret0 = LDR.Autorun();
    Ret1 = PRB.Autorun();
    Ret2 = RAE.Autorun();
    Ret3 = EPX.Autorun();
    Ret4 = RAD.Autorun();
    Ret5 = HED.Autorun();
    Ret6 = STG.Autorun(); //TODO ::
    Ret7 = PSB.Autorun();
    Ret8 = ULD.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6 && Ret7 && Ret8) ;      */
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr() ;
    bool isRunning =  MM.GetManNo() == mcCycleRotate || MM.GetManNo() == mcCycleStart ;
    bool isStop    = !isRunning    ;
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


    //Update Laser
    LCU.Update();

}


