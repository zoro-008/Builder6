//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "Stage.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "UserFile.h"

#include "UtilDefine.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SpcUnit.h"
#include "math.h"

#include "SAInterface.h"

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
//    IO_SetY(yETC_VacuSol , true);
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

    if(_Option) AT_Reset   (); //������.

    //Error.
    EM_Clear();

    //Manual.
    MM.Reset();

    //Part.
    LDR.Reset();
    PRB.Reset();
    RAE.Reset();
    EPX.Reset();
    RAD.Reset();
    HED.Reset();
    STG.Reset();
    PSB.Reset();
    ULD.Reset();

    //g_sPb2Msg = "" ;
    //g_sPb1Msg = "" ;

    FM_CloseMsgOk();
    //TODO :: �̰� �� �׿���.    
    if(!MT_GetServoAll()) MT_SetServoAll(true);
    //if(OM.CmnOptn.bUseVcCoil) IO_SetY(yHED_VoiceCoilServo,true );
    //else                      IO_SetY(yHED_VoiceCoilServo,false);

    //�� ����ÿ� ���V�ؾ� ���� ������.
    LT.m_bLotEnd = false ;

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //�ܺ� ��ư�� �� �������� ��ư���� Ŭ���� �Ǿ����� �����ؼ� �������� ��Ű�� �Լ�.
{




    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bDoorSkip) InspectDoor () ;
        if( m_iStep == scToStart) InspectHomeEnd () ; //�δ� ��δ� Y�� �߰� �߰� Ȩ��� �� ������.
        InspectLightGrid() ;
    }


    //Local Var.
    bool isErr     = EM_IsErr() ;

    bool bStartSw = IO_GetXUp(xETC_StartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw  = IO_GetXUp(xETC_StopSw ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw = IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_RResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw   = false ; //IO_GetXUp(xETC_LAirSw  ) || IO_GetXUp(xETC_RAirSw  ) || m_bBtnAir   ;
    bool bInitSw  = false ; //IO_GetXUp(xETC_LInitSw ) || IO_GetXUp(xETC_RInitSw ) ;
    bool bLampSw  = IO_GetXUp(xETC_LampSw ) ;//|| IO_GetXUp(xETC_PstEqpReset);

    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;
    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun);
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr));
    IO_SetY(yETC_LampLp   , IO_GetX(xETC_LampSw  ) );

    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;


    if(IO_GetX(xETC_StopSw ) &&IO_GetX(xETC_ResetSw)) {
        DM.SaveMap();
    }

    //Door Sensor.
    bool isAllCloseDoor = true ; //IO_GetX(xETC_FDoor) || IO_GetX(xETC_BDoor);

    //Init. Button Flags.
    if(bStartSw && !m_bAutoMode) {
        FM_MsgOk("ERR","Change To AutoMode!!"); return ;
    }

    if (bStartSw) {
        if(!LT.m_bLotOpen){
            FM_MsgOk("ERR","�������� ���ּ���.");
            return ;
        }

        if(DM.ARAY[riSTG].GetMaxCol() == 1 &&
           DM.ARAY[riSTG].GetMaxRow() == 1 &&
           DM.ARAY[riSTG].CheckAllStat(csUnkwn) &&
           OM.CmnOptn.bUseMapFile) {
               FM_MsgOk("ERR","�����ϸ���̰� �����۵����Ͱ� �����ϴ�.");
               return ;
        }
        if(!MT_GetHomeEndAll()) {FM_MsgOk("ERR","�ʱ�ȭ�� �����ϼ���!"); return ;}
//        if(FrmMain -> tmMainSet -> Enabled || FrmMain -> tmSubSet -> Enabled) {FM_MsgOk("ERR","Probe Setting Now. Wait for a Second."); return ;}
//        if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please."); return ;}
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
        ULD.Reset();
//        if(!IO_GetX(xETC_PostEqpReady))EM_SetErr(eiETC_PostEqpNotReady);
    }

    if (bStopSw) {
        if(MM.GetManNo() != mcNoneCycle) MM.m_bStop = true ;
        Trace("������������Ʈ", m_dLastTime.FormatString("mm-dd-hh-nn-ss-zzz").c_str());
//        MM.m_bStopTestTrm = true ;
    }

    if (bInitSw) {
        MM.SetManCycle(mcAllHome);
    }
    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainSol , !IO_GetY(yETC_MainSol )) ;
//        IO_SetY(yETC_VacuSol , !IO_GetY(yETC_VacuSol )) ;
//        IO_SetY(yETC_MainSol2, !IO_GetY(yETC_MainSol )) ;
    }

    if (bLampSw) {
        IO_SetY(yETC_LampOnOff,!IO_GetY(yETC_LampOnOff));
    }

    //�ϴ� ���� �־��.
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
                                    //������ ��Ȳ.
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
                                    //������ ��Ȳ.
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

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{                                                                                         
    bool isOk = true ;

    if(!IO_GetX(xETC_MainSol))  isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;                                                                         
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetXUp(xETC_EmgSw)) {
        EM_SetErr(eiETC_Emergency);
        MT_SetServoAll(false);
        IO_SetY(yHED_VoiceCoilServo,false);
        IO_SetY(yLDR_BreakOff, false);
        IO_SetY(yULD_BreakOff, false);
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

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

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

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

    //Inspect Motor Alarm.
    for(int i=0; i<MAX_MOTR; i++) if (MT_GetAlarm   (i)){EM_SetErr((int)eiLDR_YCmpAlarm   + i ); isOk = false; }
    for(int i=0; i<MAX_MOTR; i++) if (MT_GetNLimSnsr(i)){EM_SetErr((int)eiLDR_YCmpNegLim  + i ); isOk = false; }
    for(int i=0; i<MAX_MOTR; i++) if (MT_GetPLimSnsr(i)){EM_SetErr((int)eiLDR_YCmpPosLim  + i ); isOk = false; }
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
        if (!MT_GetHomeEnd   (i)){EM_SetErr((int)eiLDR_YCmpHomeEnd + i ); isOk = false; }
    }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectCrash(void)
{
    /*
    �ϵ���� �ɱ�...
    */

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
        //���׿����� ��� ����..
        AT_MoveCyl(aiEJT_UpDn , ccBwd);
        MT_EmgStop(miWRD_XStg);
        MT_EmgStop(miWRD_YStg);
        EM_SetErr(eiSTG_EjectorSoftLimit);
        return false ;
    }
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
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 , Ret7 , Ret8;
    //Call ToStop.
    Ret0 = LDR.ToStartCon();
    Ret1 = PRB.ToStartCon();
    Ret2 = RAE.ToStartCon();
    Ret3 = EPX.ToStartCon();
    Ret4 = RAD.ToStartCon();
    Ret5 = HED.ToStartCon();
    Ret6 = STG.ToStartCon();
    Ret7 = PSB.ToStartCon();
    Ret8 = ULD.ToStartCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6 && Ret7 && Ret8) ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStopCon(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 , Ret7 , Ret8;
    //Call ToStop.
    Ret0 = LDR.ToStopCon();
    Ret1 = PRB.ToStopCon();
    Ret2 = RAE.ToStopCon();
    Ret3 = EPX.ToStopCon();
    Ret4 = RAD.ToStopCon();
    Ret5 = HED.ToStopCon();
    Ret6 = STG.ToStopCon();
    Ret7 = PSB.ToStopCon();
    Ret8 = ULD.ToStopCon();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6 && Ret7 && Ret8) ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStart(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 , Ret7 , Ret8;
    //Call ToStop.
    Ret0 = LDR.ToStart();
    Ret1 = PRB.ToStart();
    Ret2 = RAE.ToStart();
    Ret3 = EPX.ToStart();
    Ret4 = RAD.ToStart();
    Ret5 = HED.ToStart();
    Ret6 = STG.ToStart();
    Ret7 = PSB.ToStart();
    Ret8 = ULD.ToStart();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6 && Ret7 && Ret8) ;

}

//---------------------------------------------------------------------------
bool __fastcall CSequence::ToStop(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 , Ret7 , Ret8;
    //Call ToStop.
    Ret0 = LDR.ToStop();
    Ret1 = PRB.ToStop();
    Ret2 = RAE.ToStop();
    Ret3 = EPX.ToStop();
    Ret4 = RAD.ToStop();
    Ret5 = HED.ToStop();
    Ret6 = STG.ToStop();
    Ret7 = PSB.ToStop();
    Ret8 = ULD.ToStop();

    //Ok.
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6 && Ret7 && Ret8) ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::AutoRun(void)
{
    bool Ret0 , Ret1 , Ret2 , Ret3 , Ret4 , Ret5 , Ret6 , Ret7 , Ret8;
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
    return (Ret0 && Ret1 && Ret2 && Ret3 && Ret4 && Ret5 && Ret6 && Ret7 && Ret8) ;
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


    //���׽�Ʈ
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

    for(int i = 0 ; i < MAX_ARAY ; i++) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++) {
            if(DM.ARAY[i].m_sMenuName[j] == "" || DM.ARAY[i].m_sMenuName[j] == NULL)
                ShowMessage(AnsiString(i) +"Aray " + AnsiString(j) + "Stat is null");
        }
    }

    //Update PstEqp State

}


