//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"

#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#include "PstBuff.h"
#include "Unloader.h"

#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
//#include "AdlinkTenKey.h"
#include "SPCUnit.h"

#include "FrmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CSequence SEQ;

/***************************************************************************/
/* ������ & �Ҹ���. (Constructor & Destructor)                             */
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
    LDR.Reset();
    RAL.Reset();
    HED.Reset();
    PSB.Reset();
    ULD.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

    MT_SetServoAll(true);

    //�� ����ÿ� ���V�ؾ� ���� ������.
    LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //�ܺ� ��ư�� �� �������� ��ư���� Ŭ���� �Ǿ����� �����ؼ� �������� ��Ű�� �Լ�.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bDoorSkip       ) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_LStartSw) || IO_GetXUp(xETC_RStartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw  = IO_GetXUp(xETC_LStopSw)  || IO_GetXUp(xETC_RStopSw)  || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
//    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;
    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw   = IO_GetXUp(xETC_LMainAir) || IO_GetXUp(xETC_RMainAir) || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;
    bool bVisnSw  = IO_GetXUp(xETC_LVisnSw ) || IO_GetXUp(xETC_RVisnSw );

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw ) ||  m_bRun);
    IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);
    IO_SetY(yETC_LStopLp   , IO_GetX(xETC_LStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_RStopLp   , IO_GetX(xETC_RStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LResetLp  , IO_GetX(xETC_LResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_RResetLp  , IO_GetX(xETC_RResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_LAirLp    , IO_GetX(xETC_LMainAir ) || IO_GetY(yETC_MainSol));
    IO_SetY(yETC_RAirLp    , IO_GetX(xETC_RMainAir ) || IO_GetY(yETC_MainSol));
    IO_SetY(yETC_LInitLp   , IO_GetX(xETC_LInitSw  ));
    IO_SetY(yETC_RInitLp   , IO_GetX(xETC_RInitSw  ));


    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;




    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!"); return ;}
//        if(FrmMain -> tmMainSet -> Enabled || FrmMain -> tmSubSet -> Enabled) {FM_MsgOk("ERR","Probe Setting Now. Wait for a Second."); return ;}
        if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        ULD.Reset();
//        if(!IO_GetX(xETC_PostEqpReady))EM_SetErr(eiETC_PostEqpNotReady);
    }

    if (bStopSw) {
        MM.m_bStopTestMrk = true ;


    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);

    }
    if (bVisnSw) MM.SetManCycle(mcHED_InspMan );
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
    }

    //Time Out.
    bool bHeadInsp = HED.GetSeqStep() == CHead::scInsp ;
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop && !bHeadInsp , 20000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);

        IO_SetY(yLDR_MgzInAC  , false) ;
        IO_SetY(yLDR_MgzOutAC , false) ;

        IO_SetY(yPRB_FeedingAC , false) ;
        IO_SetY(yWR1_FeedingAC , false) ;
        IO_SetY(yWR2_FeedingAC , false) ;

        IO_SetY(yULD_MgzInAC  , false) ;
        IO_SetY(yULD_MgzOutAC , false) ;






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

        case scRun       : if(!m_bReqStop) {
                                if(AutoRun()) {
                                    //������ ��Ȳ. �̻��� ����� ������ �ϳ� ����� �ɼ��� ���⿡...
                                    LT.LotEnd();
                                    m_iStep=scToStopCon ;
                                }
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            return ;

        case scToStopCon : if(!ToStopCon()) {
                                if(AutoRun()) {
                                    //������ ��Ȳ. �̻��� ����� ������ �ϳ� ����� �ɼ��� ���⿡...
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
    if (IO_GetX(xETC_EmgSw1) || IO_GetX(xETC_EmgSw2) || IO_GetX(xLDR_Emg) || IO_GetX(xULD_Emg)) {
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
    if(AT_Err(aiLDR_Cmp    )) {EM_SetErr(eiLDR_CmpTo    ) ; isOk = false ; }
    if(AT_Err(aiLDR_Pusher )) {EM_SetErr(eiLDR_PusherTo ) ; isOk = false ; }
    if(AT_Err(aiPRB_Stopper)) {EM_SetErr(eiPRB_StopperTo) ; isOk = false ; }
    if(AT_Err(aiWK1_Stopper)) {EM_SetErr(eiWK1_StopperTo) ; isOk = false ; }
    if(AT_Err(aiWK1_Align  )) {EM_SetErr(eiWK1_AlignTo  ) ; isOk = false ; }
    if(AT_Err(aiWK1_FlpCmp )) {EM_SetErr(eiWK1_FlpCmpTo ) ; isOk = false ; }
    if(AT_Err(aiWK1_Ins    )) {EM_SetErr(eiWK1_InsTo    ) ; isOk = false ; }
    if(AT_Err(aiWK2_Stopper)) {EM_SetErr(eiWK2_StopperTo) ; isOk = false ; }
    if(AT_Err(aiWK2_Align  )) {EM_SetErr(eiWK2_AlignTo  ) ; isOk = false ; }
    if(AT_Err(aiWK2_Lift   )) {EM_SetErr(eiWK2_LiftTo   ) ; isOk = false ; }
    if(AT_Err(aiWK3_Stopper)) {EM_SetErr(eiWK3_StopperTo) ; isOk = false ; }
    if(AT_Err(aiWK3_Align  )) {EM_SetErr(eiWK3_AlignTo  ) ; isOk = false ; }
    if(AT_Err(aiWK3_Lift   )) {EM_SetErr(eiWK3_LiftTo   ) ; isOk = false ; }
    if(AT_Err(aiPSB_Stopper)) {EM_SetErr(eiPSB_StopperTo) ; isOk = false ; }
    if(AT_Err(aiPSB_Align  )) {EM_SetErr(eiPSB_AlignTo  ) ; isOk = false ; }
    if(AT_Err(aiPSB_Lift   )) {EM_SetErr(eiPSB_LiftTo   ) ; isOk = false ; }
    if(AT_Err(aiPSB_Marking)) {EM_SetErr(eiPSB_MarkingTo) ; isOk = false ; }
    if(AT_Err(aiPSB_Pusher )) {EM_SetErr(eiPSB_PusherTo ) ; isOk = false ; }
    if(AT_Err(aiULD_Cmp    )) {EM_SetErr(eiULD_CmpTo    ) ; isOk = false ; }

    //Special ������ 4���� ���߿����Ϳ��� �Ⱥ��� �ۿ��� ��.
    //���߿� ������ 3�� ���� �ִ°� ���� ���� ���� �̸� �ٲܰ�. ������ �ݴ�� �Ǿ� ����.
/*
    if(AT_Complete(aiWK1_FlpCmp,ccFwd) && (IO_GetX(xWR1_ClampFw1) ||
                                           IO_GetX(xWR1_ClampFw2) ||
                                           IO_GetX(xWR1_ClampFw3) ||
                                           IO_GetX(xWR1_ClampFw4) )){
        EM_SetErr(eiWK1_FlpCmpTo ) ;
        isOk = false ;
    }
    if(AT_Complete(aiWK1_FlpCmp,ccBwd) && (!IO_GetX(xWR1_ClampFw1) ||
                                           !IO_GetX(xWR1_ClampFw2) ||
                                           !IO_GetX(xWR1_ClampFw3) ||
                                           !IO_GetX(xWR1_ClampFw4) )){
        EM_SetErr(eiWK1_FlpCmpTo ) ;
        isOk = false ;
    }
*/

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.

    if (MT_GetAlarm(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK1_XIns)){EM_SetErr(eiWK1_XInsAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_YCmp)){EM_SetErr(eiULD_YCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpAlarm ); isOk = false; }

    if (MT_GetNLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_YVsn)&&MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_XMrk)&&MT_GetHomeEnd(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_YMrk)&&MT_GetHomeEnd(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK1_YFlp)&&MT_GetHomeEnd(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK1_XIns)&&MT_GetHomeEnd(miWK1_XIns)){EM_SetErr(eiWK1_XInsNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpNegLim); isOk = false;}

    if (MT_GetPLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_YVsn)&&MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XMrk)&&MT_GetHomeEnd(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_YMrk)&&MT_GetHomeEnd(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK1_YFlp)&&MT_GetHomeEnd(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK1_XIns)&&MT_GetHomeEnd(miWK1_XIns)){EM_SetErr(eiWK1_XInsPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpPosLim); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK1_XIns)){EM_SetErr(eiWK1_XInsHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpHomeEnd); isOk = false; }

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
    if(!DM.ARAY[riLDR ].CheckAllStat(csNone) && (!IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2))) return false ;
    else if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) return false ;
//    else if( DM.ARAY[riWR1 ].CheckExist(0,0) && !IO_GetX(xWR1_Pkg) && MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs )) && !OM.CmnOptn.bDryRun) return false ;
//    else if( DM.ARAY[riWR2 ].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.CmnOptn.bDryRun) return false ;
//    else if( DM.ARAY[riWR3 ].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_Lift , ccBwd) && !OM.CmnOptn.bDryRun) return false ;
    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2))) return false ;


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
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 = LDR.ToStartCon();
    Ret1 = ULD.ToStartCon();
    Ret2 = RAL.ToStartCon();
    Ret3 = HED.ToStartCon();
    Ret4 = PSB.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 );
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 = LDR.ToStopCon();
    Ret1 = ULD.ToStopCon();
    Ret2 = RAL.ToStopCon();
    Ret3 = HED.ToStopCon();
    Ret4 = PSB.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 );

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 = LDR.ToStart();
    Ret1 = ULD.ToStart();
    Ret2 = RAL.ToStart();
    Ret3 = HED.ToStart();
    Ret4 = PSB.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 = LDR.ToStop();
    Ret1 = ULD.ToStop();
    Ret2 = RAL.ToStop();
    Ret3 = HED.ToStop();
    Ret4 = PSB.ToStop();

    //������.
    DM .SaveMap();
    HED.SaveDpAray();
    LT .SaveLotInfo();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 );
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 ;
    //Call ToStop.
    Ret0 = LDR.Autorun();
    Ret1 = ULD.Autorun();
    Ret2 = RAL.Autorun();
    Ret3 = HED.Autorun();
    Ret4 = PSB.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 );
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

    //Update PstEqp State

}


