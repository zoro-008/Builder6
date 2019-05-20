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
#include "UnLoaderMotor.h"
#include "SMInterfaceUnit.h"

#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SPCUnit.h"

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
    m_bBtnReset    = false ;
    m_bBtnStart    = false ;
    m_bBtnStop     = false ;
    m_bBtnAir      = false ;

    m_bRun       = false ;
    m_bFlick     = false ;
    m_iStep      = scIdle;
    m_iSeqStat   = ssStop;

    IO_SetY(yETC_MainSol, true);

}

//---------------------------------------------------------------------------
void __fastcall CSequence::Reset(void)
{
    //Check running flag.
    if (m_bRun                    ) return;
    if (m_iSeqStat    == ssInit   ) return;

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
    if(MM.GetManNo() != mcLDR_Out      &&
       MM.GetManNo() != mcULD_TrayMove &&
       MM.GetManNo() != mcETC_ManLotEnd   ) MM.Reset();


    //Manual.
    if(MM.GetManNo() != mcLDR_Out  &&
       MM.GetManNo() != mcETC_ManLotEnd   ) LDR.Reset();
    RAL.Reset();
    HED.Reset();
    PSB.Reset();
    if(MM.GetManNo() != mcULD_TrayMove &&
       MM.GetManNo() != mcETC_ManLotEnd) ULD.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

    MT_SetServoAll(true);

    //랏 엔드시에 리셑해야 부져 꺼지게.
    LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    if(m_iStep != scIdle ) {
        if(!OM.CmnOptn.bDoorSkip ) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_LStartSw) || IO_GetXUp(xETC_RStartSw) || m_bBtnStart ;
    bool bStopSw  = IO_GetXUp(xETC_LStopSw)  || IO_GetXUp(xETC_RStopSw)  || m_bBtnStop  ;
    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;
    bool bAirSw   = IO_GetXUp(xETC_LMainAir) || IO_GetXUp(xETC_RMainAir) || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;
    bool bVisnSw  = IO_GetXUp(xETC_LVisnSw ) || IO_GetXUp(xETC_RVisnSw );

    bool bPunchSw     = IO_GetXUp(xETC_MarkingSw  ) ;
    bool bPassSw      = IO_GetXUp(xETC_PassSw     ) ;
    bool bReviewEndSw = IO_GetXUp(xETC_ReviewEndSw) ;
    bool bULDOpenSw   = IO_GetXUp(xULD_OpenSw     ) ;

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
/*
    if (bPunchSw) {
        //Error.
        EM_Clear();
        FM_CloseMsgOk();
        if(DM.ARAY[riVS4].GetCntStat(csMarking)){
            DM.ARAY[riVS4].SetStat(PSB.m_iReviewR , PSB.m_iReviewC ,csReMark);
            if( DM.ARAY[riPSB].CheckStat(PSB.m_iReviewR , PSB.m_iReviewC ,csMarking))
            {
                DM.ARAY[riPSB].SetStat(PSB.m_iReviewR , PSB.m_iReviewC , csWork);
            }
            if(!DM.ARAY[riVS4].GetCntStat(csMarking)){
                bStartSw = true ;
            }
            else MM.SetManCycle(mcPSB_ReviewNext);
        }
        if(!DM.ARAY[riVS4].GetCntStat(csMarking)){
            bStartSw = true ;
        }
    }

    if (bPassSw) {
        //Error.
        EM_Clear();
        FM_CloseMsgOk();
        if(DM.ARAY[riVS4].GetCntStat(csMarking)) {
            DM.ARAY[riVS4].SetStat(PSB.m_iReviewR , PSB.m_iReviewC ,csWork);
            DM.ARAY[riPSB].SetStat(PSB.m_iReviewR , PSB.m_iReviewC ,csWork);
            DM.ARAY[riLOT] = DM.ARAY[riPSB] ; // 패스 하면 Lot Info 에서 빼야 된다구 해서...흑... dd
            if(!DM.ARAY[riVS4].GetCntStat(csUnkwn)) {
                bStartSw = true ;
            }
            else MM.SetManCycle(mcPSB_ReviewNext);
        }
        if(!DM.ARAY[riVS4].GetCntStat(csMarking)) {
            bStartSw = true ;
        }
    }

    if (bReviewEndSw) {
        //Error.
        EM_Clear();
        if(!DM.ARAY[riVS4].CheckAllStat(csNone)) DM.ARAY[riVS4].SetStat(csWork) ;
        bStartSw = true ;
    }
    if (bULDOpenSw) {
        MM.SetManCycle(mcULD_TrayOpen);
        DM.ARAY[riUPK].SetStat(csEmpty);
        DM.ARAY[riTRY].SetStat(csEmpty);
    }
*/

    if (bStartSw) {

    //리뷰 엔드 상황에
        if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please. " ) ; return ;}
        if(!DM.ARAY[riVS4].CheckAllStat(csNone)) DM.ARAY[riVS4].SetStat(csWork) ;

        if(!InspectMotor()) {FM_MsgOk("ERR","Initial First!!"); return ;}
        if(!MT_GetHomeEnd(miLDR_XPsh ) ||
           !MT_GetHomeEnd(miLDR_YFed ) ||
           !MT_GetHomeEnd(miLDR_XPck ) ||
           !MT_GetHomeEnd(miLDR_ZPck ) ||
           !MT_GetHomeEnd(miWK1_YFlp ) ||
           !MT_GetHomeEnd(miWK1_XIns ) ||
           !MT_GetHomeEnd(miWRK_XVsn ) ||
           !MT_GetHomeEnd(miWRK_YVsn ) ||
           !MT_GetHomeEnd(miPSB_XMrk ) ||
           !MT_GetHomeEnd(miPSB_YMrk ) ||
           !MT_GetHomeEnd(miULD_ZPck ) ||
           !MT_GetHomeEnd(miULD_YPck ) ||
           !MT_GetHomeEnd(miULD_ZTrI ) ||
           !MT_GetHomeEnd(miULD_ZTrO ) ||
           !MT_GetHomeEnd(miULD_XStp ) ||
           !MT_GetHomeEnd(miULD_ZPpr ) ) {FM_MsgOk("ERR","Initial First!!"); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        ULD.Reset();
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
    }

    if (bULDOpenSw) {
        MM.SetManCycle(mcULD_TrayOpen);
        DM.ARAY[riUPK].SetStat(csEmpty);
        DM.ARAY[riTRY].SetStat(csEmpty);
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
    }

    if (isStopCon  &&  m_iStep) m_bReqStop = true ;
    if (isResetCon && !m_iStep) Reset()           ;

    if (m_tmToStrt.OnDelay(m_iStep == scToStartCon || m_iStep == scToStart , 15000)) {
        //Trace Log.
        AnsiString Msg ;
        Msg.sprintf("ToStrtTimeOut : m_iStep=%d" ,m_iStep );
        Trace  ("SEQ",Msg.c_str() );
        EM_SetErr (eiETC_ToStartTO);
        m_iStep = scIdle;
        m_bRun  = false;
    }

    //Time Out.
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , 15000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);

        IO_SetY(yPRB_FeedingAC , false) ;
        IO_SetY(yWK1_FeedingAC , false) ;
        IO_SetY(yPSB_FeedingAC , false) ;
        IO_SetY(yULD_FeedingAC , false) ;

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

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{
    bool isOk = true ;

    if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol)) isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetXUp(xETC_EmgSw1) || IO_GetXUp(xETC_EmgSw2) || IO_GetXUp(xLDR_EmgSw) || IO_GetXUp(xULD_EmgSw)) {
        EM_SetErr(eiETC_Emergency);
        MT_SetServoAll(false);
        IO_SetY(yLDR_ZBreak     , true);
        IO_SetY(yULD_ZBreak1    , true);
        IO_SetY(yULD_ZBreak2    , true);
        IO_SetY(yULD_ZPickBreak , true);
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
    if (!IO_GetX(xETC_FDoor) ){EM_SetErr(eiETC_FDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xETC_BDoor) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xLDR_Door1) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xLDR_Door2) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xLDR_Door3) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xULD_Door1) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xULD_Door2) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    if (!IO_GetX(xULD_Door3) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor
    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if(AT_Err(aiLDR_Pusher        )) {EM_SetErr(eiLDR_PusherTo     ) ; isOk = false ; }
    if(AT_Err(aiLDR_Rotator       )) {EM_SetErr(eiLDR_RotatorTo    ) ; isOk = false ; }
    if(AT_Err(aiLDR_Clamp         )) {EM_SetErr(eiLDR_ClampTo      ) ; isOk = false ; }
    if(AT_Err(aiLDR_InMgz         )) {EM_SetErr(eiLDR_InMgzTo      ) ; isOk = false ; }
    if(AT_Err(aiLDR_OutMgz        )) {EM_SetErr(eiLDR_OutMgzTo     ) ; isOk = false ; }
    if(AT_Err(aiPRB_Stopper       )) {EM_SetErr(eiPRB_StopperTo    ) ; isOk = false ; }
    if(AT_Err(aiWK1_Stopper       )) {EM_SetErr(eiWK1_StopperTo    ) ; isOk = false ; }
    if(AT_Err(aiWK1_Align         )) {EM_SetErr(eiWK1_AlignTo      ) ; isOk = false ; }
    if(AT_Err(aiWK1_FlpCmp        )) {EM_SetErr(eiWK1_FlpCmpTo     ) ; isOk = false ; }
    if(AT_Err(aiWK1_Ins           )) {EM_SetErr(eiWK1_InsTo        ) ; isOk = false ; }
    if(AT_Err(aiWK2_Stopper       )) {EM_SetErr(eiWK2_StopperTo    ) ; isOk = false ; }
    if(AT_Err(aiWK2_Align         )) {EM_SetErr(eiWK2_AlignTo      ) ; isOk = false ; }
    if(AT_Err(aiWK3_Stopper       )) {EM_SetErr(eiWK3_StopperTo    ) ; isOk = false ; }
    if(AT_Err(aiWK3_Align         )) {EM_SetErr(eiWK3_AlignTo      ) ; isOk = false ; }
    if(AT_Err(aiPSB_Stopper       )) {EM_SetErr(eiPSB_StopperTo    ) ; isOk = false ; }
    if(AT_Err(aiPSB_Align         )) {EM_SetErr(eiPSB_AlignTo      ) ; isOk = false ; }
    if(AT_Err(aiPSB_Marking       )) {EM_SetErr(eiPSB_MarkingTo    ) ; isOk = false ; }
    if(AT_Err(aiPSB_RjctOpen      )) {EM_SetErr(eiPSB_RjctOpenTo   ) ; isOk = false ; }
    if(AT_Err(aiPSB_RjctUpDn      )) {EM_SetErr(eiPSB_RjctUpDnTo   ) ; isOk = false ; }
    if(AT_Err(aiULD_TrayClamp     )) {EM_SetErr(eiULD_TrayClampTo  ) ; isOk = false ; }
    if(AT_Err(aiULD_TrayClampPick )) {EM_SetErr(eiULD_TrayPickTo   ) ; isOk = false ; }
    if(AT_Err(aiULD_TrayClampMove )) {EM_SetErr(eiULD_TrayMoveTo   ) ; isOk = false ; }
    if(AT_Err(aiULD_TrayColSel    )) {EM_SetErr(eiULD_TrayColSelTo ) ; isOk = false ; }
    if(AT_Err(aiULD_TrayOpen      )) {EM_SetErr(eiULD_OpenTo       ) ; isOk = false ; }
    if(AT_Err(aiULD_Hold          )) {EM_SetErr(eiULD_HoldTo       ) ; isOk = false ; }
    if(AT_Err(aiULD_PaperClamp    )) {EM_SetErr(eiULD_PaperClampTo ) ; isOk = false ; }
    if(AT_Err(aiULD_PprRotator    )) {EM_SetErr(eiULD_PprRotator   ) ; isOk = false ; }

    if(AT_Complete(aiWK1_FlpCmp,ccFwd) && (!IO_GetX(xWK1_ClampFw1) ||
                                           !IO_GetX(xWK1_ClampFw2) ||
                                           !IO_GetX(xWK1_ClampFw3) ||
                                           !IO_GetX(xWK1_ClampFw4) )){
        EM_SetErr(eiWK1_FlpCmpTo ) ;
        isOk = false ;
    }
    if(AT_Complete(aiWK1_FlpCmp,ccBwd) && ( IO_GetX(xWK1_ClampFw1) ||
                                            IO_GetX(xWK1_ClampFw2) ||
                                            IO_GetX(xWK1_ClampFw3) ||
                                            IO_GetX(xWK1_ClampFw4) )){
        EM_SetErr(eiWK1_FlpCmpTo ) ;
        isOk = false;
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
    if (MT_GetAlarm(miLDR_XPsh)){EM_SetErr(eiLDR_XPshAlarm ); isOk = false; }
    if (MT_GetAlarm(miLDR_YFed)){EM_SetErr(eiLDR_YFedAlarm ); isOk = false; }
    if (MT_GetAlarm(miLDR_XPck)){EM_SetErr(eiLDR_XPckAlarm ); isOk = false; }
    if (MT_GetAlarm(miLDR_ZPck)){EM_SetErr(eiLDR_ZPckAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpAlarm ); isOk = false; }
    if (MT_GetAlarm(miWK1_XIns)){EM_SetErr(eiWK1_XInsAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZPck)){EM_SetErr(eiULD_ZPckAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_YPck)){EM_SetErr(eiULD_YPckAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZTrI)){EM_SetErr(eiULD_ZTrIAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZTrO)){EM_SetErr(eiULD_ZTrOAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_XStp)){EM_SetErr(eiULD_XStpAlarm ); isOk = false; }
    if (MT_GetAlarm(miULD_ZPpr)){EM_SetErr(eiULD_ZPprAlarm ); isOk = false; }

    if (MT_GetNLimSnsr(miLDR_XPsh)&&MT_GetHomeEnd(miLDR_XPsh)){EM_SetErr(eiLDR_XPshNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miLDR_YFed)&&MT_GetHomeEnd(miLDR_YFed)){EM_SetErr(eiLDR_YFedNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miLDR_XPck)&&MT_GetHomeEnd(miLDR_XPck)){EM_SetErr(eiLDR_XPckNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miLDR_ZPck)&&MT_GetHomeEnd(miLDR_ZPck)){EM_SetErr(eiLDR_ZPckNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK1_YFlp)&&MT_GetHomeEnd(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWK1_XIns)&&MT_GetHomeEnd(miWK1_XIns)){EM_SetErr(eiWK1_XInsNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miWRK_YVsn)&&MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_XMrk)&&MT_GetHomeEnd(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPSB_YMrk)&&MT_GetHomeEnd(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZPck)&&MT_GetHomeEnd(miULD_ZPck)){EM_SetErr(eiULD_ZPckNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_YPck)&&MT_GetHomeEnd(miULD_YPck)){EM_SetErr(eiULD_YPckNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZTrI)&&MT_GetHomeEnd(miULD_ZTrI)){EM_SetErr(eiULD_ZTrINegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZTrO)&&MT_GetHomeEnd(miULD_ZTrO)){EM_SetErr(eiULD_ZTrONegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_XStp)&&MT_GetHomeEnd(miULD_XStp)){EM_SetErr(eiULD_XStpNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miULD_ZPpr)&&MT_GetHomeEnd(miULD_ZPpr)){EM_SetErr(eiULD_ZPprNegLim); isOk = false;}

    if (MT_GetPLimSnsr(miLDR_XPsh)&&MT_GetHomeEnd(miLDR_XPsh)){EM_SetErr(eiLDR_XPshPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miLDR_YFed)&&MT_GetHomeEnd(miLDR_YFed)){EM_SetErr(eiLDR_YFedPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miLDR_XPck)&&MT_GetHomeEnd(miLDR_XPck)){EM_SetErr(eiLDR_XPckPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miLDR_ZPck)&&MT_GetHomeEnd(miLDR_ZPck)){EM_SetErr(eiLDR_ZPckPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK1_YFlp)&&MT_GetHomeEnd(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWK1_XIns)&&MT_GetHomeEnd(miWK1_XIns)){EM_SetErr(eiWK1_XInsPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_XVsn)&&MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_YVsn)&&MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XMrk)&&MT_GetHomeEnd(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_YMrk)&&MT_GetHomeEnd(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZPck)&&MT_GetHomeEnd(miULD_ZPck)){EM_SetErr(eiULD_ZPckPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_YPck)&&MT_GetHomeEnd(miULD_YPck)){EM_SetErr(eiULD_YPckPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZTrI)&&MT_GetHomeEnd(miULD_ZTrI)){EM_SetErr(eiULD_ZTrIPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZTrO)&&MT_GetHomeEnd(miULD_ZTrO)){EM_SetErr(eiULD_ZTrOPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_XStp)&&MT_GetHomeEnd(miULD_XStp)){EM_SetErr(eiULD_XStpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miULD_ZPpr)&&MT_GetHomeEnd(miULD_ZPpr)){EM_SetErr(eiULD_ZPprPosLim); isOk = false;}
    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT_GetHomeEnd(miLDR_XPsh)){EM_SetErr(eiLDR_XPshHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miLDR_YFed)){EM_SetErr(eiLDR_YFedHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miLDR_XPck)){EM_SetErr(eiLDR_XPckHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miLDR_ZPck)){EM_SetErr(eiLDR_ZPckHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK1_YFlp)){EM_SetErr(eiWK1_YFlpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWK1_XIns)){EM_SetErr(eiWK1_XInsHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_XVsn)){EM_SetErr(eiWRK_XVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_YVsn)){EM_SetErr(eiWRK_YVsnHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XMrk)){EM_SetErr(eiPSB_XMrkHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_YMrk)){EM_SetErr(eiPSB_YMrkHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZPck)){EM_SetErr(eiULD_ZPckHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_YPck)){EM_SetErr(eiULD_YPckHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZTrI)){EM_SetErr(eiULD_ZTrIHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZTrO)){EM_SetErr(eiULD_ZTrOHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_XStp)){EM_SetErr(eiULD_XStpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZPpr)){EM_SetErr(eiULD_ZPprHomeEnd); isOk = false; }

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
    //if(!DM.ARAY[riLDR ].CheckAllStat(csNone) && !IO_GetX(xLDR_MgzWork)) return false ;        //EM_SetErr(eiLDR_MgzDispr); 추가 할지도;;JS
    //else if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) return false ;
    //else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_Pkg1) && !IO_GetX(xULD_Pkg2) && !IO_GetX(xULD_Pkg3))) return false ;

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
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 ;
    //Call ToStartCon
    Ret0 = LDR.ToStartCon();
    Ret1 = ULD.ToStartCon();
    Ret2 = RAL.ToStartCon();
    Ret3 = HED.ToStartCon();
    Ret4 = PSB.ToStartCon();
    Ret5 = ULM.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 ;
    //Call ToStopCon
    Ret0 = LDR.ToStopCon();
    Ret1 = ULD.ToStopCon();
    Ret2 = RAL.ToStopCon();
    Ret3 = HED.ToStopCon();
    Ret4 = PSB.ToStopCon();
    Ret5 = ULM.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStart
    Ret0 = LDR.ToStart();
    Ret1 = ULD.ToStart();
    Ret2 = RAL.ToStart();
    Ret3 = HED.ToStart();
    Ret4 = PSB.ToStart();
    Ret5 = ULM.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 ;
    //Call ToStart
    Ret0 = LDR.ToStop();
    Ret1 = ULD.ToStop();
    Ret2 = RAL.ToStop();
    Ret3 = HED.ToStop();
    Ret4 = PSB.ToStop();
    Ret5 = ULM.ToStop();

    //맵저장.
    DM .SaveMap();
    HED.SaveDpAray();
    LT .SaveLotInfo();
    LT .SaveTrayInfo();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 ;
    //Call ToStop.
    Ret0 = LDR.Autorun();
    Ret1 = ULM.Autorun();
    Ret2 = RAL.Autorun();
    Ret3 = HED.Autorun();
    Ret4 = PSB.Autorun();
    Ret5 = ULD.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 );
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr()    ;
    bool isRunning =  m_bRun        ;
    bool isStop    = !m_bRun        ;
    bool isLotEnd  =  LT.m_bLotEnd  ;
    bool isRunWarn =  PSB.isRunWarn ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Sequence State. Tower Lamp
         if (isInit   ) { m_iSeqStat = ssInit       ;}
    else if (isLotEnd ) { m_iSeqStat = ssWorkEnd    ;}
    else if (isError  ) { m_iSeqStat = ssError      ;}
    else if (isRunWarn) { m_iSeqStat = ssRunWarn    ;}
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


