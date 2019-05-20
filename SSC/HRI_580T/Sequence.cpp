//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"

#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#include "PstBuff.h"
#include "UnLoader.h"

#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"

#include "LotUnit.h"
#include "OptionMan.h"
#include "SPCUnit.h"

#include "EqpComUnit.h"

#include "AdlinkTenKey.h"

#include "FormRslt.h"
#include "FormOperation.h"
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
    Trace("","");
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
    if(OM.EqpOptn.bExistLoader)LDR.Reset();
    RAL.Reset();
    HED.Reset();
    PSB.Reset();
    ULD.Reset();

    FM_CloseMsgOk();

    MT_SetServoAll(true) ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bDryRun  ) InspectTemp() ;
        if(!OM.CmnOptn.bIgnrDoor) InspectDoor() ;
        InspectHomeEnd  () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_LStartSw   ) || IO_GetXUp(xETC_RStartSw) || IO_GetXUp(xETC_StartSw) || m_bBtnStart ;
    bool bStopSw  = IO_GetXUp(xETC_LStopSw    ) || IO_GetXUp(xETC_RStopSw ) || IO_GetXUp(xETC_StopSw ) || m_bBtnStop  ;
    bool bResetSw = IO_GetXUp(xETC_LResetSw   ) || IO_GetXUp(xETC_RResetSw) || IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;
    bool bAirSw   = IO_GetXUp(xETC_LAirSw     ) || IO_GetXUp(xETC_RAirSw  )                            || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_LInitSw    ) || IO_GetXUp(xETC_RInitSw ) ;
    bool bVisnSw  = IO_GetXUp(xETC_VisnTestSw ) ;
    bool bInkSw   = IO_GetXUp(xETC_InkTestSw  ) ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;

    IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw  )||  m_bRun);
    IO_SetY(yETC_LStopLp   , IO_GetX(xETC_LStopSw   )|| !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LResetLp  , IO_GetX(xETC_LResetSw  )|| (m_bFlick && isErr));
    IO_SetY(yETC_LAirLp    , IO_GetX(xETC_LAirSw    )||  IO_GetY(yETC_MainSol));
    IO_SetY(yETC_LInitLp   , IO_GetX(xETC_LInitSw   ));

    IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw  )||  m_bRun);
    IO_SetY(yETC_RStopLp   , IO_GetX(xETC_RStopSw   )|| !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_RResetLp  , IO_GetX(xETC_RResetSw  )|| (m_bFlick && isErr));
    IO_SetY(yETC_RAirLp    , IO_GetX(xETC_RAirSw    )||  IO_GetY(yETC_MainSol));
    IO_SetY(yETC_RInitLp   , IO_GetX(xETC_RInitSw   ));

    IO_SetY(yETC_StartLp   , IO_GetX(xETC_StartSw   )||  m_bRun);
    IO_SetY(yETC_StopLp    , IO_GetX(xETC_StopSw    )|| !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_ResetLp   , IO_GetX(xETC_ResetSw   )|| (m_bFlick && isErr));

    IO_SetY(yETC_VisnTestLp, IO_GetX(xETC_VisnTestSw));
    IO_SetY(yETC_InkTestLp , IO_GetX(xETC_InkTestSw ));

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    /*
    if(m_bTest) {
        TFrmRslt * Form = new TFrmRslt(Application);
        Application -> ProcessMessages();
        Form -> Show();
        m_bTest = false ;
        Application -> ProcessMessages();
    }*/

    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_Door) ;

    //Init. Button Flags.
    if (bStartSw) {
        if(!IO_GetX(xHED_1Ready)&&!OM.CmnOptn.bVisn1Skip)EM_SetErr(eiHED_Visn1NotReady) ;
        if(!IO_GetX(xHED_2Ready)&&!OM.CmnOptn.bVisn2Skip)EM_SetErr(eiHED_Visn2NotReady) ;
        if(!InspectStripDispr()) { m_bInspDispr = true ; bStartSw = false ; }

        EC.SendPreEqp(rsStart);
    }

    if (bStopSw) {
        //Reset() ;
        //여기에 리셑하면 메뉴얼 동작도 하다 맘. 명심.
        //if(HED._iSeqStep == 70) UserMsg2("Check","디스펜싱 중이라 끝난후에 멈춥니다.");
        EC.SendPreEqp(rsStop);
    }

    if (bInitSw) MM.SetManCycle(mcAllHome     );
    if (bVisnSw) MM.SetManCycle(mcHED_InspMan );
    if (bInkSw ) {
        //if(DM.ARAY[riPSB].
        //FM_MsgOk("ERR","There is no fail Chips!");
        MM.SetManCycle(mcPSB_Work    );
    }

    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol)) ;

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
    bool isStopCon  = bStopSw  ||  isErr  && !m_bReqStop &&  m_bRun ;
    bool isRunCon   = bStartSw && !isErr  ;//&& !MAN._bHoming && MAN._iManNo == mcNoneCycle ;
    bool isResetCon = bResetSw && !m_bRun ;

    //Run.
    if (isRunCon && !m_iStep) {
        m_iStep = scToStartCon ;
         //Init. Updated Flag.
         EM_SetDisp(true);
         if ((FM_GetLevel() != lvOperator) && isAllCloseDoor) {
              //OM.CmnOptn.bIgnrDoor = false ;
              //OM.SaveCmnOptn();
              FM_SetLevel(lvOperator) ;
         }
    }

    if (isStopCon  &&  m_iStep) { m_bReqStop = true ; }
    if (isResetCon && !m_iStep) { Reset()           ; }

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
    if (m_tmToStop.OnDelay((m_iStep == scToStopCon || m_iStep == scToStop) && (HED.GetSeqStep() != CHead::scInsp), 15000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);
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
                                    //랏엔드 상황.
                                    EM_SetErr(eiETC_LotEnd) ;
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
                                    EM_SetErr(eiETC_LotEnd) ;
                                    LT.LotEnd();
                                }
                                return ;
                            }
                            m_iStep=scToStop;
                            m_bRun = false ;
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

    //if(!IO_GetX(xETC_MainAir)){EM_SetErr(eiETC_MainAir); isOk = false ;}
    if(!IO_GetY(yETC_MainSol)){EM_SetErr(eiETC_MainAir); isOk = false ;}

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectTemp(void)
{
    bool isOk = true ;
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
    if (!IO_GetX(xETC_Door)){EM_SetErr(eiETC_Door); isOk = false;} //Front Door Sensor

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(OM.EqpOptn.bExistLoader) {
        if(AT_Err(aiLDR_Cmp     )) {EM_SetErr(eiLDR_CmpTO     ) ; isOk = false ; }
        if(AT_Err(aiLDR_Pusher  )) {EM_SetErr(eiLDR_PusherTO  ) ; isOk = false ; }
    }
    if(AT_Err(aiPRB_Stopper )) {EM_SetErr(eiPRB_StopperTO ) ; isOk = false ; }
    if(AT_Err(aiWK1_Align   )) {EM_SetErr(eiWK1_AlignTO   ) ; isOk = false ; }
    if(AT_Err(aiWK1_Stopper )) {EM_SetErr(eiWK1_StopperTO ) ; isOk = false ; }
    if(AT_Err(aiWK1_Lift    )) {EM_SetErr(eiWK1_LiftTO    ) ; isOk = false ; }
    if(AT_Err(aiWK2_Align   )) {EM_SetErr(eiWK2_AlignTO   ) ; isOk = false ; }
    if(AT_Err(aiWK2_Stopper )) {EM_SetErr(eiWK2_StopperTO ) ; isOk = false ; }
    if(AT_Err(aiWK2_Lift    )) {EM_SetErr(eiWK2_LiftTO    ) ; isOk = false ; }
    if(AT_Err(aiHED_UVLight )) {EM_SetErr(eiHED_UVLightTO ) ; isOk = false ; }
    if(AT_Err(aiPSB_Align   )) {EM_SetErr(eiPSB_AlignTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_FlprCmp )) {EM_SetErr(eiPSB_FlprCmpTO ) ; isOk = false ; }
    if(AT_Err(aiPSB_Flpr    )) {EM_SetErr(eiPSB_FlprTO    ) ; isOk = false ; }
    if(AT_Err(aiPSB_Trimmer )) {EM_SetErr(eiPSB_TrimmerTO ) ; isOk = false ; }
    if(AT_Err(aiPSB_Finger  )) {EM_SetErr(eiPSB_FingerTO  ) ; isOk = false ; }
    if(AT_Err(aiPSB_PusherUD)) {EM_SetErr(eiPSB_PusherUDTO) ; isOk = false ; }
    if(AT_Err(aiPSB_PusherFB)) {EM_SetErr(eiPSB_PusherFBTO) ; isOk = false ; }
    if(AT_Err(aiULD_Cmp     )) {EM_SetErr(eiULD_CmpTO     ) ; isOk = false ; }


    //Ok.
    return isOk;
}
//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripDispr(void)
{
    if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) return false ;
    else if( DM.ARAY[riWK1 ].CheckExist(0,0) && !IO_GetX(xWK1_Pkg) && AT_Complete(aiWK1_Lift , ccBwd) && !OM.CmnOptn.bDryRun) return false ;
    else if( DM.ARAY[riWK2 ].CheckExist(0,0) && !IO_GetX(xWK2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.CmnOptn.bDryRun) return false ;
    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_CmpMgz1) && !IO_GetX(xULD_CmpMgz2))) return false ;
    //else if(DM.ARAY[riPSB].GetCntExist () && !IO_GetX(xPSB_Pkg) && !OM.CmnOptn.bDryRun) return false ;
    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.
    if(OM.EqpOptn.bExistLoader) {
        if (MT_GetAlarm(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpAlarm ); isOk = false; }
        if (MT_GetAlarm(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpAlarm ); isOk = false; }
    }
    if (MT_GetAlarm(miPSB_TSrt)){EM_SetErr(eiPSB_TSrtAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK1_YVsn)){EM_SetErr(eiWK1_YVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK1_ZVsn)){EM_SetErr(eiWK1_ZVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK2_YVsn)){EM_SetErr(eiWK2_YVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_YCmp)){EM_SetErr(eiULD_YCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XTrm)){EM_SetErr(eiPSB_XTrmAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_YTrm)){EM_SetErr(eiPSB_YTrmAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XIns)){EM_SetErr(eiPSB_XInsAlarm ); isOk = false; }

    if(OM.EqpOptn.bExistLoader) {
        if (MT_GetNLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpNegLim); isOk = false;}
        if (MT_GetNLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpNegLim); isOk = false;}
    }
    if (MT_GetNLimSnsr(miPSB_TSrt)&&MT_GetHomeEnd(miPSB_TSrt)){EM_SetErr(eiPSB_TSrtNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK1_YVsn)&&MT_GetHomeEnd(miWK1_YVsn)){EM_SetErr(eiWK1_YVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK1_ZVsn)&&MT_GetHomeEnd(miWK1_ZVsn)){EM_SetErr(eiWK1_ZVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK2_YVsn)&&MT_GetHomeEnd(miWK2_YVsn)){EM_SetErr(eiWK2_YVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_XTrm)&&MT_GetHomeEnd(miPSB_XTrm)){EM_SetErr(eiPSB_XTrmNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_YTrm)&&MT_GetHomeEnd(miPSB_YTrm)){EM_SetErr(eiPSB_YTrmNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_XIns)&&MT_GetHomeEnd(miPSB_XIns)){EM_SetErr(eiPSB_XInsNegLim); isOk = false;}

    if(OM.EqpOptn.bExistLoader) {
        if (MT_GetPLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpPosLim); isOk = false;}
        if (MT_GetPLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpPosLim); isOk = false;}
    }
    if (MT_GetPLimSnsr(miPSB_TSrt)&&MT_GetHomeEnd(miPSB_TSrt)){EM_SetErr(eiPSB_TSrtPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK1_YVsn)&&MT_GetHomeEnd(miWK1_YVsn)){EM_SetErr(eiWK1_YVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK1_ZVsn)&&MT_GetHomeEnd(miWK1_ZVsn)){EM_SetErr(eiWK1_ZVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK2_YVsn)&&MT_GetHomeEnd(miWK2_YVsn)){EM_SetErr(eiWK2_YVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XTrm)&&MT_GetHomeEnd(miPSB_XTrm)){EM_SetErr(eiPSB_XTrmPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_YTrm)&&MT_GetHomeEnd(miPSB_YTrm)){EM_SetErr(eiPSB_YTrmPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XIns)&&MT_GetHomeEnd(miPSB_XIns)){EM_SetErr(eiPSB_XInsPosLim); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(OM.EqpOptn.bExistLoader) {
        if (!MT_GetHomeEnd(miLDR_YCmp) && !LDR.m_bIsHome){EM_SetErr(eiLDR_YCmpHomeEnd); isOk = false; }
        if (!MT_GetHomeEnd(miLDR_ZCmp) && !LDR.m_bIsHome){EM_SetErr(eiLDR_ZCmpHomeEnd); isOk = false; }
    }
    if (!MT_GetHomeEnd(miPSB_TSrt)){EM_SetErr(eiPSB_TSrtHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK1_YVsn)){EM_SetErr(eiWK1_YVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK1_ZVsn)){EM_SetErr(eiWK1_ZVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK2_YVsn)){EM_SetErr(eiWK2_YVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_YCmp) && !ULD.m_bIsHome){EM_SetErr(eiULD_YCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZCmp) && !ULD.m_bIsHome){EM_SetErr(eiULD_ZCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XTrm) && !PSB.m_bIsHome){EM_SetErr(eiPSB_XTrmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_YTrm) && !PSB.m_bIsHome){EM_SetErr(eiPSB_YTrmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XIns)){EM_SetErr(eiPSB_XInsHomeEnd); isOk = false; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectCrash(void)
{
    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStartCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 =!OM.EqpOptn.bExistLoader || LDR.ToStartCon();
    Ret1 = RAL.ToStartCon();
    Ret2 = HED.ToStartCon();
    Ret3 = PSB.ToStartCon();
    Ret4 = ULD.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 =!OM.EqpOptn.bExistLoader ||  LDR.ToStopCon();
    Ret1 = RAL.ToStopCon();
    Ret2 = HED.ToStopCon();
    Ret3 = PSB.ToStopCon();
    Ret4 = ULD.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 =!OM.EqpOptn.bExistLoader ||  LDR.ToStart();
    Ret1 = RAL.ToStart();
    Ret2 = HED.ToStart();
    Ret3 = PSB.ToStart();
    Ret4 = ULD.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 =!OM.EqpOptn.bExistLoader ||  LDR.ToStop();
    Ret1 = RAL.ToStop();
    Ret2 = HED.ToStop();
    Ret3 = PSB.ToStop();
    Ret4 = ULD.ToStop();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 = (OM.EqpOptn.bExistLoader && LDR.Autorun()) ||
          (!OM.EqpOptn.bExistLoader && EC.GetPreEqpLotEnd());
    Ret1 = RAL.Autorun();
    Ret2 = HED.Autorun();
    Ret3 = PSB.Autorun();
    Ret4 = ULD.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4);
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

    //Set Sequence State.
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
    ADL.Update();

    //Update Motor State (Input).
    MM.Update();

    //Update Equipment Communication.
    EC.Update();

}


