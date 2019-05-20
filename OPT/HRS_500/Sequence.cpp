//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Rail.h"
#include "UnLoader.h"
#include "Picker.h"
#include "Sorter.h"
#include "Supply.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SpcUnit.h"

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

    SPL.Init(lrLeftBin );
    SPR.Init(lrRightBin);
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
    IO_SetY(yETC_VacuSol , true);
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
    ULD.Reset();
    PCK.Reset();
    SRT.Reset();
    SPL.Reset();
    SPR.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

    MT_SetServoAll(true);

    //랏 엔드시에 리셑해야 부져 꺼지게.
    //LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        //if(!OM.CmnOptn.bDoorSkip       ) InspectDoor    () ;
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
    bool bAirSw   = IO_GetXUp(xETC_LAirSw  ) || IO_GetXUp(xETC_RAirSw  ) || m_bBtnAir   ;
    bool bInitSw  = IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_LStartLp  , IO_GetX(xETC_LStartSw ) ||  m_bRun);
    IO_SetY(yETC_RStartLp  , IO_GetX(xETC_RStartSw ) ||  m_bRun);
    IO_SetY(yETC_LStopLp   , IO_GetX(xETC_LStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_RStopLp   , IO_GetX(xETC_RStopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_LResetLp  , IO_GetX(xETC_LResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_RResetLp  , IO_GetX(xETC_RResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_LAirLp    , IO_GetX(xETC_LAirSw   ) || IO_GetY(yETC_MainSol) || IO_GetY(yETC_VacuSol));
    IO_SetY(yETC_RAirLp    , IO_GetX(xETC_RAirSw   ) || IO_GetY(yETC_MainSol) || IO_GetY(yETC_VacuSol));
    IO_SetY(yETC_LInitLp   , IO_GetX(xETC_LInitSw  ));
    IO_SetY(yETC_RInitLp   , IO_GetX(xETC_RInitSw  ));


    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;




    //Door Sensor.
    bool isAllCloseDoor = true ; //IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","Initial First!!"); return ;}
//        if(FrmMain -> tmMainSet -> Enabled || FrmMain -> tmSubSet -> Enabled) {FM_MsgOk("ERR","Probe Setting Now. Wait for a Second."); return ;}
        //if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        ULD.Reset();
//        if(!IO_GetX(xETC_PostEqpReady))EM_SetErr(eiETC_PostEqpNotReady);
    }

    if (bStopSw) {
        MM.m_bStopTestTrm = true ;


    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);

    }
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
        IO_SetY(yETC_VacuSol , !IO_GetY(yETC_VacuSol )) ;
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

    if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_VacuSol))  isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;                                                                         
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    //if (IO_GetX(xETC_EmgSw1) || IO_GetX(xETC_EmgSw2) || IO_GetX(xLDR_Emg) || IO_GetX(xULD_Emg)) {
    //    EM_SetErr(eiETC_Emergency); MT_SetServoAll(false); isOk = false ;
    //}

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
    if(AT_Err(aiLDR_Sptr)) {EM_SetErr(eiLDR_SptrTO) ; isOk = false ; }
    if(AT_Err(aiPCK_UpDn)) {EM_SetErr(eiPCK_UpDnTO) ; isOk = false ; }
    if(AT_Err(aiRAL_Indx)) {EM_SetErr(eiRAL_IndxTO) ; isOk = false ; }
    if(AT_Err(aiSRT_Grip)) {EM_SetErr(eiSRT_GripTO) ; isOk = false ; }
    if(AT_Err(aiSRT_Flip)) {EM_SetErr(eiSRT_FlipTO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn0)) {EM_SetErr(eiSPL_LBn0TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn1)) {EM_SetErr(eiSPL_LBn1TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn2)) {EM_SetErr(eiSPL_LBn2TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn3)) {EM_SetErr(eiSPL_LBn3TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn4)) {EM_SetErr(eiSPL_LBn4TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn5)) {EM_SetErr(eiSPL_LBn5TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn6)) {EM_SetErr(eiSPL_LBn6TO) ; isOk = false ; }
    if(AT_Err(aiSPL_LBn7)) {EM_SetErr(eiSPL_LBn7TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn0)) {EM_SetErr(eiSPL_RBn0TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn1)) {EM_SetErr(eiSPL_RBn1TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn2)) {EM_SetErr(eiSPL_RBn2TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn3)) {EM_SetErr(eiSPL_RBn3TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn4)) {EM_SetErr(eiSPL_RBn4TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn5)) {EM_SetErr(eiSPL_RBn5TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn6)) {EM_SetErr(eiSPL_RBn6TO) ; isOk = false ; }
    if(AT_Err(aiSPL_RBn7)) {EM_SetErr(eiSPL_RBn7TO) ; isOk = false ; }
    if(AT_Err(aiULD_Stck)) {EM_SetErr(eiULD_StckTO) ; isOk = false ; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.

    if (MT_GetAlarm(miLDR_Z)){EM_SetErr(eiLDR_ZAlarm ); isOk = false; }
    if (MT_GetAlarm(miRAL_X)){EM_SetErr(eiRAL_XAlarm ); isOk = false; }
    if (MT_GetAlarm(miPCK_Y)){EM_SetErr(eiPCK_YAlarm ); isOk = false; }
    if (MT_GetAlarm(miSRT_X)){EM_SetErr(eiSRT_XAlarm ); isOk = false; }

    if (MT_GetNLimSnsr(miLDR_Z)&&MT_GetHomeEnd(miLDR_Z)){EM_SetErr(eiLDR_ZNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miRAL_X)&&MT_GetHomeEnd(miRAL_X)){EM_SetErr(eiRAL_XNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miPCK_Y)&&MT_GetHomeEnd(miPCK_Y)){EM_SetErr(eiPCK_YNegLim); isOk = false;}
    if (MT_GetNLimSnsr(miSRT_X)&&MT_GetHomeEnd(miSRT_X)){EM_SetErr(eiSRT_XNegLim); isOk = false;}

    if (MT_GetPLimSnsr(miLDR_Z)&&MT_GetHomeEnd(miLDR_Z)){EM_SetErr(eiLDR_ZPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miRAL_X)&&MT_GetHomeEnd(miRAL_X)){EM_SetErr(eiRAL_XPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPCK_Y)&&MT_GetHomeEnd(miPCK_Y)){EM_SetErr(eiPCK_YPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miSRT_X)&&MT_GetHomeEnd(miSRT_X)){EM_SetErr(eiSRT_XPosLim); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT_GetHomeEnd(miLDR_Z)){EM_SetErr(eiLDR_ZHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miRAL_X)){EM_SetErr(eiRAL_XHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPCK_Y)){EM_SetErr(eiPCK_YHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miSRT_X)){EM_SetErr(eiSRT_XHomeEnd); isOk = false; }

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
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 ;
    //Call ToStop.
    Ret0 = LDR.ToStartCon();
    Ret1 = RAL.ToStartCon();
    Ret2 = ULD.ToStartCon();
    Ret3 = PCK.ToStartCon();
    Ret4 = SRT.ToStartCon();
    Ret5 = SPL.ToStartCon();
    Ret6 = SPR.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 ;
    //Call ToStop.
    Ret0 = LDR.ToStopCon();
    Ret1 = RAL.ToStopCon();
    Ret2 = ULD.ToStopCon();
    Ret3 = PCK.ToStopCon();
    Ret4 = SRT.ToStopCon();
    Ret5 = SPL.ToStopCon();
    Ret6 = SPR.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6);

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 ;
    //Call ToStop.
    Ret0 = LDR.ToStart();
    Ret1 = RAL.ToStart();
    Ret2 = ULD.ToStart();
    Ret3 = PCK.ToStart();
    Ret4 = SRT.ToStart();
    Ret5 = SPL.ToStart();
    Ret6 = SPR.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 ;
    //Call ToStop.
    Ret0 = LDR.ToStop();
    Ret1 = RAL.ToStop();
    Ret2 = ULD.ToStop();
    Ret3 = PCK.ToStop();
    Ret4 = SRT.ToStop();
    Ret5 = SPL.ToStop();
    Ret6 = SPR.ToStop();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 ;
    //Call ToStop.
    Ret0 = LDR.Autorun();
    Ret1 = RAL.Autorun();
    Ret2 = ULD.Autorun();
    Ret3 = PCK.Autorun();
    Ret4 = SRT.Autorun();
    Ret5 = SPL.Autorun();
    Ret6 = SPR.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6);
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit     =  MM.GetManNo() == mcAllHome ;
    bool isError    =  EM_IsErr()    ;
    bool isRunning  =  m_bRun        ;
    bool isStop     = !m_bRun        ;
    bool isLotEnd   =  LT.m_bLotEnd  ;
    bool isRunWarn  =  SRT.isRunWarn ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Sequence State. Tower Lamp
         if (isInit   ) { m_iSeqStat = ssInit       ;}
    else if (isLotEnd ) { m_iSeqStat = ssWorkEnd    ;}
    else if (isError  ) { m_iSeqStat = ssError      ;}
    else if (isStop   ) { m_iSeqStat = ssStop       ;}
    else if (isRunWarn) { m_iSeqStat = ssRunWarn    ;}
    else if (isRunning) { m_iSeqStat = ssRunning    ;}
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

    //오토런에 안물리고 자체 시퀜스.
    SPL.Update();
    SPR.Update();
}


