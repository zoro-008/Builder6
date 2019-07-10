//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"

#include "Loader.h"
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "SortingTool.h"
#include "UnLoader430t.h"

#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
//#include "AdlinkTenKey.h"
#include "SPCUnit.h"

#include "FormMain.h"

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
    //
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

}
void CSequence::Close(void)
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
    STL.Reset();
    ULD.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();

    MT_SetServoAll(true);

    LT.m_bLotEnd = false;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //외부 버튼들 및 윈도우즈 버튼들이 클릭이 되었을때 취합해서 동작진행 시키는 함수.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
//        if(!OM.CmnOptn.bIgnrHeatAlm  ) InspectTemp    () ;
        if(!OM.CmnOptn.bIgnrDoor     ) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    if(!OM.CmnOptn.bIgnrHeatAlm  ) InspectTemp    () ;

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    bool bStartSw = IO_GetXUp(xETC_LStartSw) || IO_GetXUp(xETC_RStartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw  = IO_GetXUp(xETC_LStopSw)  || IO_GetXUp(xETC_RStopSw)  || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
//    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;
    bool bResetSw = IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw   = IO_GetXUp(xETC_LMainAir) || IO_GetXUp(xETC_RMainAir) || m_bBtnAir   ;
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
    if (bStartSw) {
//        if(FrmMain -> tmMainSet -> Enabled || FrmMain -> tmSubSet -> Enabled) {FM_MsgOk("ERR","Probe Setting Now. Wait for a Second."); return ;}
        if(!OM.MstOptn.bNotUstLot) if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { /*m_bInspDispr = true ;*/ bStartSw = false ; } 
        if(!InspectStripUnknown()) { /*m_bInspUnkwn = true ;*/ bStartSw = false ; }
        //if(OM.EqpOptn.bExistUnLoader) { ULD.Reset(); }
//        if(!IO_GetX(xETC_PostEqpReady))EM_SetErr(eiETC_PostEqpNotReady);
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);

    }

    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
        IO_SetY(yETC_MainSol2, !IO_GetY(yETC_MainSol )) ;
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
    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , 10000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);

        //20190627 사이클인이 에러일때도 계속 타서 시끄러움 끔 dw
        IO_SetY(yPRB_AirBlower,false);
        IO_SetY(yPRB_FeedingAC,false);
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
                                    if(!OM.MstOptn.bNotUstLot) LT.LotEnd();
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
                                    if(!OM.MstOptn.bNotUstLot) LT.LotEnd();
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

    if(!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol) || !IO_GetY(yETC_MainSol2)) isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;                                                                         
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectTemp(void)
{
    bool isOk = true ;

    // juhyeon
    int  iTempErr;
    int  iTempCnt = 7;
    int  iMaxTemp = 3;

    if(OM.MstOptn.bUsedTempRS232){
        for(int i = 0 ; i < iTempCnt ; i++){ //i < iTempCnt = 8번째 온도 컨트롤러는 커버라서 이상하게 나옴.
            iTempErr = TCU.GetSetTemp(i) - TCU.GetCrntTemp(i);
            if(TCU.GetCrntTemp(i) > 200 + iMaxTemp )                              {TCU.SetTemp(i , 200); EM_SetErr(eiETC_TempAlrm); isOk = false ; }
            if(iTempErr > OM.MstOptn.iTempOfs || iTempErr < -OM.MstOptn.iTempOfs) {                      EM_SetErr(eiETC_TempAlrm); isOk = false ; }
        }
    }

    else{
        if(IO_GetX(xPRB_TempZ1Alrm1)){EM_SetErr(eiPRB_TempZ1Alrm1); isOk = false ;}
        if(IO_GetX(xPRB_TempZ1Wire1)){EM_SetErr(eiPRB_TempZ1Wire1); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Alrm2)){EM_SetErr(eiPRB_TempZ1Alrm2); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Wire2)){EM_SetErr(eiPRB_TempZ1Wire2); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Alrm3)){EM_SetErr(eiPRB_TempZ1Alrm3); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Wire3)){EM_SetErr(eiPRB_TempZ1Wire3); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Alrm4)){EM_SetErr(eiPRB_TempZ1Alrm4); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Wire4)){EM_SetErr(eiPRB_TempZ1Wire4); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Alrm5)){EM_SetErr(eiPRB_TempZ1Alrm5); isOk = false ;}   
        if(IO_GetX(xPRB_TempZ1Wire5)){EM_SetErr(eiPRB_TempZ1Wire5); isOk = false ;}   
                                                                                      
        if(IO_GetX(xPSB_TempZ2Alrm1)){EM_SetErr(eiPSB_TempZ2Alrm1); isOk = false ;}   
        if(IO_GetX(xPSB_TempZ2Wire1)){EM_SetErr(eiPSB_TempZ2Wire1); isOk = false ;}
        if(IO_GetX(xPSB_TempZ2Alrm2)){EM_SetErr(eiPSB_TempZ2Alrm2); isOk = false ;}   
        if(IO_GetX(xPSB_TempZ2Wire2)){EM_SetErr(eiPSB_TempZ2Wire2); isOk = false ;}   
        if(IO_GetX(xPSB_TempZ2Alrm3)){EM_SetErr(eiPSB_TempZ2Alrm3); isOk = false ;}   
        if(IO_GetX(xPSB_TempZ2Wire3)){EM_SetErr(eiPSB_TempZ2Wire3); isOk = false ;}   
        if(IO_GetX(xPSB_TempZ2Alrm3)){EM_SetErr(eiPSB_TempZ2Alrm3); isOk = false ;}   
    }


    // juhyeon 기존.
//    if(IO_GetX(xPRB_TempZ1Alrm1)){EM_SetErr(eiPRB_TempZ1Alrm1); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Wire1)){EM_SetErr(eiPRB_TempZ1Wire1); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Alrm2)){EM_SetErr(eiPRB_TempZ1Alrm2); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Wire2)){EM_SetErr(eiPRB_TempZ1Wire2); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Alrm3)){EM_SetErr(eiPRB_TempZ1Alrm3); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Wire3)){EM_SetErr(eiPRB_TempZ1Wire3); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Alrm4)){EM_SetErr(eiPRB_TempZ1Alrm4); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Wire4)){EM_SetErr(eiPRB_TempZ1Wire4); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Alrm5)){EM_SetErr(eiPRB_TempZ1Alrm5); isOk = false ;}
//    if(IO_GetX(xPRB_TempZ1Wire5)){EM_SetErr(eiPRB_TempZ1Wire5); isOk = false ;}

//    if(IO_GetX(xPSB_TempZ2Alrm1)){EM_SetErr(eiPSB_TempZ2Alrm1); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Wire1)){EM_SetErr(eiPSB_TempZ2Wire1); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Alrm2)){EM_SetErr(eiPSB_TempZ2Alrm2); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Wire2)){EM_SetErr(eiPSB_TempZ2Wire2); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Alrm3)){EM_SetErr(eiPSB_TempZ2Alrm3); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Wire3)){EM_SetErr(eiPSB_TempZ2Wire3); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Alrm3)){EM_SetErr(eiPSB_TempZ2Alrm3); isOk = false ;}
//    if(IO_GetX(xPSB_TempZ2Wire3)){EM_SetErr(eiPSB_TempZ2Wire3); isOk = false ;}


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
    if(AT_Err(aiPRB_Cmp  )) {EM_SetErr(eiPRB_CmpTO   ) ; isOk = false ; }
    //if(AT_Err(aiPRB_IdChk)) {EM_SetErr(eiPRB_IdChkTO ) ; isOk = false ; }
    if(AT_Err(aiWRK_Cmp  )) {EM_SetErr(eiWRK_CmpTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_Cmp  )) {EM_SetErr(eiPSB_CmpTO   ) ; isOk = false ; }
    if(AT_Err(aiLDR_Cmp  )) {EM_SetErr(eiLDR_CmpTO   ) ; isOk = false ; }
    if(AT_Err(aiLDR_Psh  )) {EM_SetErr(eiLDR_PushTO  ) ; isOk = false ; }
    if(AT_Err(aiSTL_Top  )) {EM_SetErr(eiSTL_CmpTO   ) ; isOk = false ; }
    if(AT_Err(aiSTL_Btm  )) {EM_SetErr(eiSTL_CmpTO   ) ; isOk = false ; }
    if(AT_Err(aiULD_Cmp  )) {EM_SetErr(eiULD_CmpTO   ) ; isOk = false ; }
    if(AT_Err(aiPSB_Psh  )) {EM_SetErr(eiULD_PushTO  ) ; isOk = false ; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

//    //Inspect Motor Alarm.
    bool basss = MT_GetAlarm(miWRK_ZTop);
    bool b1    = MT_GetAlarm(miPRB_XCmp);

    if (MT_GetAlarm(miPRB_XCmp)){EM_SetErr(eiPRB_XCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_XCmp)){EM_SetErr(eiWRK_XCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miPSB_XCmp)){EM_SetErr(eiPSB_XCmpAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_ZTop)){EM_SetErr(eiWRK_ZTopAlarm ); isOk = false; }
    if (MT_GetAlarm(miWRK_ZBtm)){EM_SetErr(eiWRK_ZBtmAlarm ); isOk = false; }
    if (MT_GetAlarm(miSTL_YTop)){EM_SetErr(eiSTL_YTopAlarm ); isOk = false; }
    if (MT_GetAlarm(miSTL_YBtm)){EM_SetErr(eiSTL_YBtmAlarm ); isOk = false; }
    if(OM.EqpOptn.bExistLoader  ){ if (MT_GetAlarm(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpAlarm ); isOk = false; } }
    if(OM.EqpOptn.bExistLoader  ){ if (MT_GetAlarm(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpAlarm ); isOk = false; } }
    if(OM.EqpOptn.bExistUnLoader){ if (MT_GetAlarm(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpAlarm ); isOk = false; } }
    if(OM.EqpOptn.bExistUnLoader){ if (MT_GetAlarm(miULD_YCmp)){EM_SetErr(eiULD_YCmpAlarm ); isOk = false; } }

//    if (MT_GetNLimSnsr(miWRK_ZTop)&&MT_GetHomeEnd(miWRK_ZTop)){EM_SetErr(eiWRK_ZTopNegLim); isOk = false;}
//    if (MT_GetNLimSnsr(miWRK_ZBtm)&&MT_GetHomeEnd(miWRK_ZBtm)){EM_SetErr(eiWRK_ZBtmNegLim); isOk = false;}
    if(OM.EqpOptn.bExistLoader  ){ if (MT_GetNLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpNegLim); isOk = false;} }
    if(OM.EqpOptn.bExistLoader  ){ if (MT_GetNLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpNegLim); isOk = false;} }
    if(OM.EqpOptn.bExistUnLoader){ if (MT_GetNLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpNegLim); isOk = false;} }
    if(OM.EqpOptn.bExistUnLoader){ if (MT_GetNLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpNegLim); isOk = false;} }

    if (MT_GetPLimSnsr(miPRB_XCmp)&&MT_GetHomeEnd(miPRB_XCmp)){EM_SetErr(eiPRB_XCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_XCmp)&&MT_GetHomeEnd(miWRK_XCmp)){EM_SetErr(eiWRK_XCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miPSB_XCmp)&&MT_GetHomeEnd(miPSB_XCmp)){EM_SetErr(eiPSB_XCmpPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miWRK_ZTop)&&MT_GetHomeEnd(miWRK_ZTop)){EM_SetErr(eiWRK_ZTopPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miSTL_YTop)&&MT_GetHomeEnd(miSTL_YTop)){EM_SetErr(eiSTL_YTopPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miREJ_XBtm)&&MT_GetHomeEnd(miREJ_XBtm)){EM_SetErr(eiREJ_XBtmPosLim); isOk = false;}
    if (MT_GetPLimSnsr(miSTL_YBtm)&&MT_GetHomeEnd(miSTL_YBtm)){EM_SetErr(eiSTL_YBtmPosLim); isOk = false;}
//    if (MT_GetPLimSnsr(miWRK_ZBtm)&&MT_GetHomeEnd(miWRK_ZBtm)){EM_SetErr(eiWRK_ZBtmPosLim); isOk = false;} //Home
    if(OM.EqpOptn.bExistLoader  ){ if (MT_GetPLimSnsr(miLDR_ZCmp)&&MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpPosLim); isOk = false;} }
    if(OM.EqpOptn.bExistLoader  ){ if (MT_GetPLimSnsr(miLDR_YCmp)&&MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpPosLim); isOk = false;} }
    if(OM.EqpOptn.bExistUnLoader){ if (MT_GetPLimSnsr(miULD_ZCmp)&&MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpPosLim); isOk = false;} }
    if(OM.EqpOptn.bExistUnLoader){ if (MT_GetPLimSnsr(miULD_YCmp)&&MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpPosLim); isOk = false;} }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    bool basss = !MT_GetHomeEnd(miWRK_ZTop);
    bool b1    = !MT_GetHomeEnd(miPRB_XCmp);

    //Inspect.
    if (!MT_GetHomeEnd(miPRB_XCmp)){EM_SetErr(eiPRB_XCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_XCmp)){EM_SetErr(eiWRK_XCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miPSB_XCmp)){EM_SetErr(eiPSB_XCmpHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_ZTop)){EM_SetErr(eiWRK_ZTopHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miWRK_ZBtm)){EM_SetErr(eiWRK_ZBtmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miSTL_YTop)){EM_SetErr(eiSTL_YTopHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miREJ_XBtm)){EM_SetErr(eiREJ_XBtmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miSTL_YBtm)){EM_SetErr(eiSTL_YBtmHomeEnd); isOk = false; }
    if (!MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpHomeEnd); isOk = false; }
    if(OM.EqpOptn.bExistLoader && !LDR.m_bIsHome ){ if (!MT_GetHomeEnd(miLDR_ZCmp)){EM_SetErr(eiLDR_ZCmpHomeEnd); isOk = false; } }
    if(OM.EqpOptn.bExistLoader && !LDR.m_bIsHome ){ if (!MT_GetHomeEnd(miLDR_YCmp)){EM_SetErr(eiLDR_YCmpHomeEnd); isOk = false; } }
    if(OM.EqpOptn.bExistLoader                   ){ if (!MT_GetHomeEnd(miULD_ZCmp)){EM_SetErr(eiULD_ZCmpHomeEnd); isOk = false; } }
    if(OM.EqpOptn.bExistLoader                   ){ if (!MT_GetHomeEnd(miULD_YCmp)){EM_SetErr(eiULD_YCmpHomeEnd); isOk = false; } }

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

//         if (!DM.ARAY[riPR2].CheckAllStat(csNone) && !IO_GetX(xPRB_3Pkg) ) return false ;
    if(DM.ARAY[riWRK].GetCntExist() && (!IO_GetX(xWRK_1Pkg) && !IO_GetX(xWRK_2Pkg) )  && !OM.CmnOptn.bDryRun ) {EM_SetErr(eiWRK_Dispr); isOk = false ;}
    if(DM.ARAY[riPSB].GetCntExist() &&  !OM.CmnOptn.bDryRun) {
        if(!IO_GetX(xPSB_1Pkg) && !IO_GetX(xPSB_2Pkg) && !IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_4Pkg) && !IO_GetX(xPSB_PkgOut1) ) {EM_SetErr(eiPSB_Dispr); isOk = false ;}
    }
//    else if ( DM.ARAY[riPS2].GetCntExist() && (!IO_GetX(xPSB_1Pkg) && !IO_GetX(xPSB_2Pkg) && !IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) && !OM.CmnOptn.bDryRun) return false ;

    if (OM.EqpOptn.bExistLoader  ) { if(!DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) ) ) { EM_SetErr(eiLDR_MgzDispr ); isOk = false ; }}
    if (OM.EqpOptn.bExistUnLoader) { if(!DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) ) { EM_SetErr(eiULD_MgzDispr ); isOk = false ; }}

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripUnknown(void)
{
    bool isOk = true;

    if(DM.ARAY[riPRB].CheckAllStat(csNone) && (IO_GetX(xPRB_1Pkg) || IO_GetX(xPRB_2Pkg) || IO_GetX(xPRB_3Pkg)                     ) ) {EM_SetErr(eiPRB_Unknown   ); isOk = false;}
    if(DM.ARAY[riWRK].CheckAllStat(csNone) && (IO_GetX(xWRK_1Pkg) ||(IO_GetX(xWRK_2Pkg) &&!IO_GetX(xPSB_1Pkg)                     ))) {EM_SetErr(eiWRK_Unknown   ); isOk = false;}
    //if(DM.ARAY[riPSB].CheckAllStat(csNone) && (IO_GetX(xPSB_1Pkg) || IO_GetX(xPSB_2Pkg) || IO_GetX(xPSB_3Pkg) || IO_GetX(xPSB_4Pkg))) {EM_SetErr(eiPSB_Unknown   ); isOk = false;}

    if(OM.EqpOptn.bExistLoader  ) { if(DM.ARAY[riLDR].CheckAllStat(csNone) && (IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2))) {EM_SetErr(eiLDR_MgzUnknown); isOk = false;} }
    if(OM.EqpOptn.bExistUnLoader) { if(DM.ARAY[riULD].CheckAllStat(csNone) && (IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2))) {EM_SetErr(eiULD_MgzUnknown); isOk = false;} }
//    if (DM.ARAY[riPR2].CheckAllStat(csNone) && DM.ARAY[riPR1].CheckAllStat(csNone) && IO_GetX(xPRB_3Pkg) ) return false ;
    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStartCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    if(OM.EqpOptn.bExistLoader  ) Ret0 = LDR.ToStartCon();
    else Ret0 = true ;
    if(OM.EqpOptn.bExistUnLoader) Ret1 = ULD.ToStartCon();
    else Ret1 = true ;
    Ret2 = PRB.ToStartCon();
    Ret3 = WRK.ToStartCon();
    Ret4 = PSB.ToStartCon();
    Ret5 = STL.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    if(OM.EqpOptn.bExistLoader  ) Ret0 = LDR.ToStopCon();
    else Ret0 = true ;
    if(OM.EqpOptn.bExistUnLoader) Ret1 = ULD.ToStopCon();
    else Ret1 = true ;
    Ret2 = PRB.ToStopCon();
    Ret3 = WRK.ToStopCon();
    Ret4 = PSB.ToStopCon();
    Ret5 = STL.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    if(OM.EqpOptn.bExistLoader  ) Ret0 = LDR.ToStart();
    else Ret0 = true ;
    if(OM.EqpOptn.bExistUnLoader) Ret1 = ULD.ToStart();
    else Ret1 = true ;
    Ret2 = PRB.ToStart();
    Ret3 = WRK.ToStart();
    Ret4 = PSB.ToStart();
    Ret5 = STL.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    if(OM.EqpOptn.bExistLoader  ) Ret0 = LDR.ToStop();
    else Ret0 = true ;
    if(OM.EqpOptn.bExistUnLoader) Ret1 = ULD.ToStop();
    else Ret1 = true ;
    Ret2 = PRB.ToStop();
    Ret3 = WRK.ToStop();
    Ret4 = PSB.ToStop();
    Ret5 = STL.ToStop();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5;
    //Call ToStop.
    if(OM.EqpOptn.bExistLoader  ) Ret0 = LDR.Autorun();
    else Ret0 = true;
    if(OM.EqpOptn.bExistUnLoader) Ret1 = ULD.Autorun();
    else Ret1 = true;
    Ret2 = PRB.Autorun();
    Ret3 = WRK.Autorun();
    Ret4 = PSB.Autorun();
    Ret5 = STL.Autorun();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5);
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
    //ADL.Update();

    //Update Motor State (Input).
    MM.Update();

    //Update PstEqp State

}


