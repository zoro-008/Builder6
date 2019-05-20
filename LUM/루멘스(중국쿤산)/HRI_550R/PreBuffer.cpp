//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PreBuffer.h"
#include "Head.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "Loader.h"
//#include "FormRslt.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CRail  RAL;

CRail::CRail(void)
{
    m_sPartName = "Rail " ;

    Reset();
    Load(true);

    m_bLotEnded = false ;
}

void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CRail::~CRail (void)
{
    Load(false);

}

bool CRail::FindChip(int &r , int &c )
{
    return false;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miWK1_YFlp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWK1_YFlpNormal  : dPos = PM.GetValue (_iMotr , pvWK1_YFlpNormalPs  ); break ;
            case piWK1_YFlpInverse : dPos = PM.GetValue (_iMotr , pvWK1_YFlpInversePs ); break ;
        }
    }
    else if(_iMotr == miWK1_XIns){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWK1_XInsWait    : dPos = PM.GetValue (_iMotr , pvWK1_XInsWaitPs    ); break ;
            case piWK1_XInsStpr    : dPos = PM.GetValue (_iMotr , pvWK1_XInsStprPs    ); break ;
            case piWK1_XInsOut     : dPos = PM.GetValue (_iMotr , pvWK1_XInsOutPs     ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isClampClose  = AT_Complete(aiWK1_FlpCmp , ccFwd ) ;
    bool isClampOpen   = AT_Complete(aiWK1_FlpCmp , ccBwd ) ;
    bool isFlpDone     = MT_GetStopInpos(miWK1_YFlp) && (MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs)) || MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInversePs))) ;
    bool isInsUp       = AT_Complete(aiWK1_Ins    , ccBwd ) ;
    bool isInsWait     = MT_CmprPos (miWK1_XIns , PM.GetValue(miWK1_XIns,pvWK1_XInsWaitPs)) ;
    bool isWRK_YWait   = MT_CmprPos (miWRK_YVsn , PM.GetValue(miWRK_YVsn,pvWRK_YVsnWaitPs)) ;
    bool isPrbStripOutExist = IO_GetX(xPRB_PkgOut2);

    if(_iMotr == miWK1_YFlp){
        switch(_iPstnId) {
            default                :                                                   break ;
            case piWK1_YFlpNormal  : if(!isClampClose || !isInsUp || !isInsWait || !isWRK_YWait || isPrbStripOutExist) { sMsg = "!isClampClose || !isInsUp || !isInsWait || !isWRK_YWait || isPrbStripOutExist" ; bRet = false ;} break ;
            case piWK1_YFlpInverse : if(!isClampClose || !isInsUp || !isInsWait || !isWRK_YWait || isPrbStripOutExist) { sMsg = "!isClampClose || !isInsUp || !isInsWait || !isWRK_YWait || isPrbStripOutExist" ; bRet = false ;} break ;
        }
    }
    else if(_iMotr == miWK1_XIns){
        switch(_iPstnId) {
            default                :                                                   break ;
            case piWK1_XInsWait    : if(!isFlpDone                ) { sMsg = "!isFlpDone                " ; bRet = false ;} break ;
            case piWK1_XInsStpr    : if(!isFlpDone || !isClampOpen) { sMsg = "!isFlpDone || !isClampOpen" ; bRet = false ;} break ;
            case piWK1_XInsOut     : if(!isFlpDone || !isClampOpen) { sMsg = "!isFlpDone || !isClampOpen" ; bRet = false ;} break ;
        }
    }


    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return bRet ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool b1 = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs)) ;
    bool b2 = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInversePs)) ;

    bool isFlpDone     = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs)) || MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInversePs)) ;
    bool isInsWait     = MT_CmprPos (miWK1_XIns , PM.GetValue(miWK1_XIns,pvWK1_XInsWaitPs  )) ;

    if(_iActr == aiWK1_Ins) {
        if(!_bFwd) {
            if(!isFlpDone ) { sMsg = "!isFlpDone || !isInsWait" ; bRet = false ;}
        }
    }
    else if(_iActr == aiWK1_Align || _iActr == aiWK2_Align || _iActr == aiWK3_Align) {
    }
    else if(_iActr == aiPRB_Stopper ||_iActr == aiWK1_Stopper || _iActr == aiWK2_Stopper || _iActr == aiWK3_Stopper) {
   }
    else if(_iActr == aiWK2_Lift || _iActr == aiWK3_Lift) {
    }
    else if(_iActr == aiWK1_FlpCmp) {
    }


    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRail::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_HomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    //if(Stat.bReqStop) {
    //    Step.iHome = 0;
    //    return true ;
    //}

    switch (Step.iHome) {
        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_Reset(miWK1_XIns);
                 MT_Reset(miWK1_YFlp);
                 MT_SetServo(miWK1_XIns,true);
                 MT_SetServo(miWK1_YFlp,true);

                 AT_MoveCyl(aiPRB_Stopper , ccFwd);
                 AT_MoveCyl(aiWK1_FlpCmp  , ccFwd);
                 AT_MoveCyl(aiWK1_Ins     , ccBwd);
                 AT_MoveCyl(aiWK1_Stopper , ccBwd);
                 AT_MoveCyl(aiWK1_Align   , ccBwd);
                 AT_MoveCyl(aiWK2_Align   , ccBwd);
                 AT_MoveCyl(aiWK2_Lift    , ccBwd);
                 AT_MoveCyl(aiWK2_Stopper , ccFwd);
                 AT_MoveCyl(aiWK3_Align   , ccBwd);
                 AT_MoveCyl(aiWK3_Lift    , ccBwd);
                 AT_MoveCyl(aiWK3_Stopper , ccFwd);

                 IO_SetY(yPRB_FeedingAC,false) ;
                 IO_SetY(yWR1_FeedingAC,false) ;
                 IO_SetY(yPRB_AirBlower,false) ;

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Stopper , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_FlpCmp  , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_Ins     , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_Stopper , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_Align   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_Align   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_Lift    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_Stopper , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_Align   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_Lift    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_Stopper , ccFwd)) return false ;
                 MT_DoHome(miWK1_XIns) ;
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWK1_XIns))return false ;
                 MT_GoAbsMan(miWK1_XIns , GetMotrPos(miWK1_XIns , piWK1_XInsWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWK1_XIns , GetMotrPos(miWK1_XIns , piWK1_XInsWait))) return false ;
                 MT_DoHome(miWK1_YFlp) ;
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miWK1_YFlp))return false ;
                 MT_GoAbsMan(miWK1_YFlp , GetMotrPos(miWK1_YFlp , piWK1_YFlpNormal));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWK1_YFlp , GetMotrPos(miWK1_YFlp , piWK1_YFlpNormal))) return false ;
                 AT_MoveCyl(aiWK1_FlpCmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 16: if(!AT_MoveCyl(aiWK1_FlpCmp,ccBwd)) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
//    EC.SetPreEqpReady(false);
//    if (EC.GetPreEqpSending()) return false ;
    if (Step.iSeq) return false;


    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: IO_SetY(yPRB_FeedingAC,false) ;
                 IO_SetY(yWR1_FeedingAC,false) ;
//                 IO_SetY(yWR2_FeedingAC,false) ;
                 Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
                 return true ;
    }
}

bool CRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yPRB_AirBlower,false) ;
                 IO_SetY(yPRB_FeedingAC,false) ;
                 IO_SetY(yWR1_FeedingAC,false) ;
//                 IO_SetY(yWR2_FeedingAC,false) ;
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}


bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop) return false ;

        bool isVS1End = DM.ARAY[riWR1].GetCntExist() && (!DM.ARAY[riVS1].GetCntStat(csUnkwn) && !DM.ARAY[riWR1].GetCntStat (csUnkwn));
        bool isVS2End = DM.ARAY[riWR2].GetCntExist() && (!DM.ARAY[riVS2].GetCntStat(csUnkwn) && !DM.ARAY[riWR2].GetCntStat (csUnkwn));
        bool isVS3End = DM.ARAY[riWR3].GetCntExist() && (!DM.ARAY[riVS3].GetCntStat(csUnkwn) && !DM.ARAY[riWR3].GetCntStat (csUnkwn));

        bool isWK1End =!DM.ARAY[riWR1].GetCntExist() || isVS1End ;
        bool isWK2End =!DM.ARAY[riWR2].GetCntExist() || isVS2End ;
        bool isWK3End =!DM.ARAY[riWR3].GetCntExist() || isVS3End ;

        bool isCycleBind  = //(DM.ARAY[riVS1].GetCntStat(csUnkwn)&& IO_GetX(xWR1_Pkg)) || //&& !AT_Complete(aiWK1_Lift,ccFwd)) ||
                            (DM.ARAY[riVS2].GetCntStat(csUnkwn)&& IO_GetX(xWR2_Pkg) && !AT_Complete(aiWK2_Lift,ccFwd)) ||
                            (DM.ARAY[riVS3].GetCntStat(csUnkwn)&& IO_GetX(xWR3_Pkg) && !AT_Complete(aiWK3_Lift,ccFwd)) ;

        bool isDoingInsp  =  HED.GetSeqStep() == CHead::scInsp ;

        bool isExistPSBX  =  IO_GetX(xPSB_Pkg) || IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4) ;

        bool isPSBReady   = AT_Complete(aiPSB_Lift    , ccBwd) && AT_Complete(aiPSB_Align  , ccBwd) &&
                            AT_Complete(aiPSB_Marking , ccFwd) && AT_Complete(aiPSB_Pusher , ccBwd) && AT_Complete(aiPSB_Stopper , ccFwd) &&
                            !isExistPSBX;

        bool isWK1Ready   = MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs)) &&
                            AT_Complete(aiWK1_Align,ccBwd) && AT_Complete(aiWK1_FlpCmp,ccFwd) && AT_Complete(aiWK1_Ins,ccBwd) && AT_Complete(aiWK1_Stopper,ccFwd) ;

//        m_bWaitLotChange  =  DM.ARAY[riPRB].GetCntExist() && DM.ARAY[riWK1].GetCntExist() && //TODO : 이거 기다리면 안될거 같다.
//                             DM.ARAY[riPRB].GetLotNo   () != DM.ARAY[riWK1].GetLotNo   () ;

        bool isCycleWK1Rdy = !IO_GetX(xWR1_Pkg) && !AT_Complete(aiWK1_FlpCmp,ccFwd) && DM.ARAY[riWR1].GetCntExist();

//        bool isCycleMvAll  =  DM.ARAY[riPRB].GetCntExist() &&  isVS1End &&  isVS2End &&  isVS3End &&
//                              DM.ARAY[riPSB].CheckAllStat(csNone) && isPSBReady;

        bool isCycleMvAll  =  isVS1End &&  isVS2End &&  isVS3End && isPSBReady;

//        bool isCycleMvR1   =  DM.ARAY[riPRB].GetCntExist() && isWK1End ;

        bool isCycleMv1    = isVS1End && isWK1End ;
        bool isCycleMv2    = isVS2End && isWK2End ;
        bool isCycleMv3    = isVS3End && isWK3End && isPSBReady && DM.ARAY[riPSB].CheckAllStat(csNone) ;

        bool isCycleMvAuto = (isWK1End && isWK2End && isWK3End) && !isDoingInsp &&
                             (isCycleMvAll || isCycleMv1 || isCycleMv2 || isCycleMv3 ) ;

        bool isCycleMvPrb  =  DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() && !isDoingInsp ;

//        bool isCycleMvAuto = (isWK1End && isWK2End && isWK3End) && !isDoingInsp &&
//                             (isCycleMvAll || isCycleMvR1 || isCycleMv1 || isCycleMv2 || isCycleMv3 ) ;

        bool isCycleIn    = (IO_GetX(xPRB_PkgIn1) || OM.CmnOptn.bDryRun) && !DM.ARAY[riPRB].GetCntExist() ;//&& !isCycleMvAuto;


        bool isConEnd      = !DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() && !DM.ARAY[riWR2].GetCntExist() && !DM.ARAY[riWR3].GetCntExist() && !DM.ARAY[riPSB].GetCntExist() ;

        //모르는 스트립에러.
        if(!DM.ARAY[riPRB ].CheckExist(0,0) &&  IO_GetX(xPRB_Pkg) ) EM_SetErr(eiPRB_PkgUnknown ) ;
        if(!DM.ARAY[riWR1 ].CheckExist(0,0) &&  IO_GetX(xWR1_Pkg) ) EM_SetErr(eiWK1_PkgUnknown ) ;
        if(!DM.ARAY[riWR2 ].CheckExist(0,0) &&  IO_GetX(xWR2_Pkg) ) EM_SetErr(eiWK2_PkgUnknown ) ;
        if(!DM.ARAY[riWR3 ].CheckExist(0,0) &&  IO_GetX(xWR3_Pkg) ) EM_SetErr(eiWK3_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun ) EM_SetErr(eiPRB_PkgDispr ) ;

        //스테이지 올리면 센서가 감지 않되어 뺀다.
        if( DM.ARAY[riWR1 ].CheckExist(0,0) && !IO_GetX(xWR1_Pkg) && MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs)) && !isCycleWK1Rdy && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK1_PkgDispr ) ;
        if( DM.ARAY[riWR2 ].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK2_PkgDispr ) ;
        if( DM.ARAY[riWR3 ].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_Lift , ccBwd) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK3_PkgDispr ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn    ) {Trace(m_sPartName.c_str(),"CycleIn     Stt"); Step.iSeq = scIn     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleWK1Rdy) {Trace(m_sPartName.c_str(),"CycleWK1Rdy Stt"); Step.iSeq = scWK1Rdy ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleMvAuto) {Trace(m_sPartName.c_str(),"CycleMvAuto Stt"); Step.iSeq = scMvAuto ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleMvPrb ) {Trace(m_sPartName.c_str(),"CycleMvPrb  Stt"); Step.iSeq = scMvPrb  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBind  ) {Trace(m_sPartName.c_str(),"CycleBind   Stt"); Step.iSeq = scBind   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                 /*Trace(m_sPartName.c_str(),"default     End");*/Step.iSeq = scIdle ;  return false ;
        case  scIn     : if(CycleIn     ()){ Trace(m_sPartName.c_str(),"CycleIn     End");Step.iSeq = scIdle ;} return false ;
        case  scWK1Rdy : if(CycleWK1Rdy ()){ Trace(m_sPartName.c_str(),"CycleWK1Rdy End");Step.iSeq = scIdle ;} return false ;
        case  scMvAuto : if(CycleMvAuto ()){ Trace(m_sPartName.c_str(),"CycleMvAuto End");Step.iSeq = scIdle ;} return false ;
        case  scMvPrb  : if(CycleMvPrb  ()){ Trace(m_sPartName.c_str(),"CycleMvPrb  End");Step.iSeq = scIdle ;} return false ;
        case  scBind   : if(CycleBind   ()){ Trace(m_sPartName.c_str(),"CycleBind   End");Step.iSeq = scIdle ;} return false ;

    }
}

bool CRail::CycleIn(void) //스트립을 PreBuff로 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPRB_Stopper  , ccFwd);
                  IO_SetY(yPRB_AirBlower,true); //열을 식혀 보장...
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_Stopper  , ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC , true) ;  //ana
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xPRB_Pkg)&&!OM.CmnOptn.bDryRun) return false ;
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  IO_SetY(yPRB_AirBlower,false); //열을 식혀 보장...
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CRail::CycleWK1Rdy(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : if(IO_GetX(xPRB_PkgOut2)) {
                      EM_SetErr(eiPRB_OutStripExist);
                      return false ;
                  }

                  MoveActr(aiWK1_FlpCmp   , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiWK1_FlpCmp   , ccBwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  MoveActr(aiWK1_Ins   , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiWK1_Ins   , ccBwd)) return false ;
                  MoveMotr(miWK1_YFlp,piWK1_YFlpNormal);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveMotr(miWK1_YFlp,piWK1_YFlpNormal)) return false ;
                  if(!IO_GetX(xWR1_TurnDetect))
                  {
                      EM_SetErr(eiWK1_FlipperMoveFail);
                      return false ;
                  }
                  MoveActr(aiWK1_Ins   , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiWK1_Ins   , ccFwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsStpr);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveMotr(miWK1_XIns   , piWK1_XInsStpr)) return false ;
                  MoveActr(aiWK1_Align   , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(!MoveActr(aiWK1_Align   , ccFwd)) return false ;
                  MoveActr(aiWK1_FlpCmp  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 18 : if(!MoveActr(aiWK1_FlpCmp   , ccFwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  MoveActr(aiWK1_Align  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 19 : if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  if(!MoveActr(aiWK1_Align   , ccBwd)) return false ;
                  MoveActr(aiWK1_Ins  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 20 : if(!MoveActr(aiWK1_Ins,ccBwd)) return false ;
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CRail::CycleMvAuto(void) //자동 계산  이동.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    static bool bCanMvPRB = false ;
    static bool bCanMvWK1 = false ;
    static bool bCanMvWK2 = false ;
    static bool bCanMvWK3 = false ;

    bool r1 , r2 , r3 , r4 ;

    int  r , c , iCnt;

    static bool bInPkg = false ;

    if(IO_GetX(xPRB_PkgIn1)) bInPkg = true ;

//    if( bCanMvWK2&& IO_GetXDn(xWK2_Out)) {MoveActr(aiPSB_FlprCmp , ccFwd); bCatch = true ; Trace("Catch","PSB_PKG");} 다운엣지로 이렇게 음냥.

    bool bLastStrip ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bInPkg = false ;

                  bCanMvWK3 = DM.ARAY[riWR3].GetCntExist()&& DM.ARAY[riVS3].GetCntExist() &&  !DM.ARAY[riPSB].GetCntExist() ;
                  bCanMvWK2 = DM.ARAY[riWR2].GetCntExist()&& DM.ARAY[riVS2].GetCntExist() && (!DM.ARAY[riWR3].GetCntExist()||bCanMvWK3) ;
                  bCanMvWK1 = DM.ARAY[riWR1].GetCntExist()&& DM.ARAY[riVS1].GetCntExist() && (!DM.ARAY[riWR2].GetCntExist()||bCanMvWK2) ;
//                  bCanMvPRB = DM.ARAY[riPRB].GetCntExist()&& (!DM.ARAY[riWR1].GetCntExist()||bCanMvWK1) ;

                  //Trace("Rail","WaitforLotChange");
                  if(bCanMvWK3) {
                      LT.AddDayInfoWorkStrp(1);
                      LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

                      LT.AddLotInfoWorkStrp(1);
                      LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

                      LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riWR3].GetCntStat(csWork));
                      LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riWR3].GetCntStat(csWork));
                      OM.iTotalChip     = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;
                      OM.iTotalFailChip = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riWR3].GetCntStat(csWork) ;

                      if(OM.iTotalChip > 4000000000 ) {
                          OM.iTotalChip     = 0 ;
                          OM.iTotalFailChip = 0 ;
                      }
                      LT.AddDayInfoChips(DM.ARAY[riWR3].GetCntStat(csRslt1 ) , //"V Empty"
                                         DM.ARAY[riWR3].GetCntStat(csRslt2 ) , //"V Chip"
                                         DM.ARAY[riWR3].GetCntStat(csRslt3 ) , //"V Dust"
                                         DM.ARAY[riWR3].GetCntStat(csRslt4 ) , //"V LFlow"
                                         DM.ARAY[riWR3].GetCntStat(csRslt5 ) , //"V Broken"
                                         DM.ARAY[riWR3].GetCntStat(csRslt6 ) , //"V Crack"
                                         DM.ARAY[riWR3].GetCntStat(csRslt7 ) , //"V Burr"
                                         DM.ARAY[riWR3].GetCntStat(csRslt8 ) , //"V Zener"
                                         DM.ARAY[riWR3].GetCntStat(csRslt9 ) , //"V Wire"
                                         DM.ARAY[riWR3].GetCntStat(csRslt10) , //"V Orient"
                                         DM.ARAY[riWR3].GetCntStat(csRslt11) , //"V Pmeasure"
                                         DM.ARAY[riWR3].GetCntStat(csRslt12) , //"V Pdist"
                                         DM.ARAY[riWR3].GetCntStat(csRslt13) , //"V Match"
                                         DM.ARAY[riWR3].GetCntStat(csRslt14) , //"V Flow"
                                         DM.ARAY[riWR3].GetCntStat(csFail  ) ); //"Heat Fail");

                      LT.AddLotInfoChips(DM.ARAY[riWR3].GetCntStat(csRslt1 ) , //"V Empty"
                                         DM.ARAY[riWR3].GetCntStat(csRslt2 ) , //"V Chip"
                                         DM.ARAY[riWR3].GetCntStat(csRslt3 ) , //"V Dust"
                                         DM.ARAY[riWR3].GetCntStat(csRslt4 ) , //"V LFlow"
                                         DM.ARAY[riWR3].GetCntStat(csRslt5 ) , //"V Broken"
                                         DM.ARAY[riWR3].GetCntStat(csRslt6 ) , //"V Crack"
                                         DM.ARAY[riWR3].GetCntStat(csRslt7 ) , //"V Burr"
                                         DM.ARAY[riWR3].GetCntStat(csRslt8 ) , //"V Zener"
                                         DM.ARAY[riWR3].GetCntStat(csRslt9 ) , //"V Wire"
                                         DM.ARAY[riWR3].GetCntStat(csRslt10) , //"V Orient"
                                         DM.ARAY[riWR3].GetCntStat(csRslt11) , //"V Pmeasure"
                                         DM.ARAY[riWR3].GetCntStat(csRslt12) , //"V Pdist"
                                         DM.ARAY[riWR3].GetCntStat(csRslt13) , //"V Match"
                                         DM.ARAY[riWR3].GetCntStat(csRslt14) ,
                                         DM.ARAY[riWR3].GetCntStat(csFail  ) ); //"Heat Fail");//"V Flow"
                                    //!DM.ARAY[riWK2].CheckAllStat(csNone) &&

                      bLastStrip = (DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riLDR].GetLotNo() &&  LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riWR1].CheckAllStat(csNone)&& DM.ARAY[riWR2].CheckAllStat(csNone)) ||
                                   (DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riWR2].GetLotNo() && !DM.ARAY[riWR2].CheckAllStat(csNone) ) ||
                                   (DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riWR1].GetLotNo() && !DM.ARAY[riWR1].CheckAllStat(csNone) &&  DM.ARAY[riWR2].CheckAllStat(csNone) ) ||
                                   (DM.ARAY[riWR3].GetLotNo() != DM.ARAY[riPRB].GetLotNo() && !DM.ARAY[riPRB].CheckAllStat(csNone) &&  DM.ARAY[riWR1].CheckAllStat(csNone) &&  DM.ARAY[riWR2].CheckAllStat(csNone) ) ||
                                   (DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPRB].CheckAllStat(csNone) &&  DM.ARAY[riWR1].CheckAllStat(csNone) &&  DM.ARAY[riWR2].CheckAllStat(csNone)) ; //내려놓는 중에 하나 걸어놓음.
//                                   (LDR.GetWorkEnd() && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPRB].CheckAllStat(csNone) &&  DM.ARAY[riWK1].CheckAllStat(csNone) &&  DM.ARAY[riWK2].CheckAllStat(csNone)) ;
                      if(bLastStrip) {
                          m_bLotEnded = true ;
                          memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

                          Trace("LotEnd",DM.ARAY[riWR3].GetLotNo().c_str());
                          LT.LotInfo.dEndTime = Now();
                          LT.WriteLotLog   ();
                          LT.WriteLotDayLog();
                          Trace("WriteLotLog",DM.ARAY[riWR3].GetLotNo().c_str());
                          //LT.LotEnd();
                      }

                  }

//                  if(bCanMvPRB && (m_sPreLotNo != DM.ARAY[riPRB].GetLotNo())  ){
//                      Trace("LotOpen",DM.ARAY[riPRB].GetLotNo().c_str());
//
//                      LT.LotOpen(DM.ARAY[riPRB].GetLotNo());
//
//                      if(!OM.EqpOptn.bExistLoader)LT.AddDayInfoLotCnt(1);
//                      if(!OM.CmnOptn.bVisn1Skip || !OM.CmnOptn.bVisn2Skip) VC.SendLotStart();
//                      /*sun*/
//                      m_sPreLotNo = DM.ARAY[riPRB].GetLotNo() ;
//                      Load(false);
//                  }

//                  MoveActr(aiWK1_Lift    , ccBwd);
                  MoveActr(aiWK2_Lift    , ccBwd);
                  MoveActr(aiWK3_Lift    , ccBwd);

                  MoveActr(aiWK1_Align   , ccBwd);
                  MoveActr(aiWK2_Align   , ccBwd);
                  MoveActr(aiWK3_Align   , ccBwd);

                  MoveActr(aiWK1_FlpCmp,ccFwd);
                  MoveActr(aiWK1_Ins   ,ccBwd);
//                  MoveActr(aiWK1_FlpCmp,ccBwd);

                  MoveActr(aiPRB_Stopper  , ccFwd);
                  MoveActr(aiWK1_Stopper  , ccFwd);
                  MoveActr(aiWK2_Stopper  , ccFwd);
                  MoveActr(aiWK3_Stopper  , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  11: //if(!MoveActr(aiWK1_Lift    , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Lift    , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_Lift    , ccBwd)) return false ;

                  if(!MoveActr(aiWK1_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_Align   , ccBwd)) return false ;

                  if(!MoveActr(aiWK1_FlpCmp,ccFwd)) return false ;
                  if(!MoveActr(aiWK1_Ins   ,ccBwd)) return false ;

                  if(!MoveActr(aiPRB_Stopper  , ccFwd)) return false ;
                  if(!MoveActr(aiWK1_Stopper  , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Stopper  , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Stopper  , ccFwd)) return false ;

                  IO_SetY(yPRB_FeedingAC , true);   //ana
                  IO_SetY(yWR1_FeedingAC , true);
//                  IO_SetY(yWR2_FeedingAC , true);

                  if(bCanMvWK1) MoveMotr(miWK1_YFlp,piWK1_YFlpInverse) ;

                  if(bCanMvWK3)
                  {
                      IO_SetY(yWR2_FeedingAC , true);
                      MoveActr(aiWK3_Stopper , ccBwd);
                  }

                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  12: if(bCanMvWK2 && !m_tmTemp.OnDelay(true , OM.CmnOptn.iWK2StprDnDelay))return false ;
                  if(bCanMvWK2)MoveActr(aiWK2_Stopper , ccBwd);
                  Step.iCycle++ ;
                  return false ;


        case  13: if(bCanMvWK3&&!MoveActr(aiWK3_Stopper , ccBwd)) return false ;
                  if(bCanMvWK2&&!MoveActr(aiWK2_Stopper , ccBwd)) return false ;


                  //비젼어레이는 나중에 스테이지 올리고 바꾼다.
                  //플립할때 뒤집지 않고 들어 왔을때 뒤집어야 예외상황이 적다. 플립에 뒤집으면 트림하다 말고 장비 껐다 키면 데이터 뒤집힐수 있음
                  if(bCanMvWK3){
                      if(OM.CmnOptn.bMrkAllSkip){
                          DM.ARAY[riWR3].SetStat(csWork);
                          /*DM.ARAY[riWK2].SetStat(0,0,csFail);*/
                      }
                  }
                  if(bCanMvWK3){
                      DM.ShiftArrayData(riWR3 , riPSB) ;

//                      DM.ARAY[riVS4].SetStat(csUnkwn)  ;
//                      r  = OM.DevInfo.iInsMrkVsRowCnt*OM.DevInfo.iRowInspMrkCnt ;
//                      r1 = r  * iPsbiRowCnt;
//                      r2 = r1 + r;
//                      c = OM.DevInfo.iInsMrkVsColCnt * OM.DevInfo.iColInspMrkCnt ;
//                      if(r > OM.DevInfo.iRowCnt ) r= OM.DevInfo.iRowCnt ;
//                      if(OM.DevInfo.iInsMrkVsColCnt > OM.DevInfo.iColCnt ) c = OM.DevInfo.iColCnt ;
//                      for(int j = 0 ; j < c ; j++ ) {
//                          for(int i = r1 ; i < r2 ; i ++) {
//                              DM.ARAY[riVS4].SetStat(i,j,csUnkwn) ;
//                          }
//                      }
//                      iPsbiRowCnt++;
//                      if((OM.DevInfo.iRowCnt / r)%2 ) if( (OM.DevInfo.iRowCnt / r - 1) > iPsbiRowCnt )  iPsbiRowCnt = 0 ;
//                      else{
//                          if( (OM.DevInfo.iRowCnt / r ) > iPsbiRowCnt )  iPsbiRowCnt = 0 ;
//                      }
//
//
//                      DM.ShiftArrayData(riVB4 , riVS4) ;
                      DM.ARAY[riVS3].SetStat(csNone)   ;
                      DM.ARAY[riPSB].ChangeStat(csWork , csUnkwn) ;
                      DM.ARAY[ri3B1].SetStat(csNone)   ;
                      DM.ARAY[ri3B2].SetStat(csNone)   ;
                      DM.ARAY[ri3B3].SetStat(csNone)   ;
                  }


                  if(bCanMvWK2){
                      DM.ShiftArrayData(riWR2 , riWR3) ;
                      DM.ShiftArrayData(ri2B2 , ri3B2) ;
                      DM.ShiftArrayData(ri2B1 , ri3B1) ;
                      DM.ARAY[riVS2].SetStat(csNone)   ;
                      DM.ARAY[riVS3].SetStat(csUnkwn)  ;
                      DM.ARAY[riWR3].ChangeStat(csWork , csUnkwn) ;
                  }
//                  if(bCanMvWK1) MoveMotr(miWK1_YFlp,piWK1_YFlpInverse) ;

                  Step.iCycle++ ;
                  return false ;

        case  14: if(bCanMvWK3){if(!IO_GetX(xWR3_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiWK3_Stopper , ccFwd) ;}
                  if(bCanMvWK2){if(!IO_GetX(xWR2_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiWK2_Stopper , ccFwd) ;}
                  if(bCanMvWK3)if(!AT_GetCmd(aiWK3_Stopper)) return false ;
                  if(bCanMvWK2)if(!AT_GetCmd(aiWK2_Stopper)) return false ;
//                  if(!AT_GetCmd(aiWK2_Stopper) || !AT_GetCmd(aiWK3_Stopper)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  15: if(bCanMvWK3&&!MoveActr(aiWK3_Stopper , ccFwd)) return false ;
                  if(bCanMvWK2&&!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
//                  if(bCanMvWK1) MoveMotr(miWK1_YFlp,piWK1_YFlpInverse) ; 위로 올려봄.
                  Step.iCycle++ ;
                  return false ;

        case  16: if(bCanMvWK1) if(!MoveMotr(miWK1_YFlp,piWK1_YFlpInverse)) return false ;
                  if(bCanMvWK1 && !IO_GetX(xWR1_TurnDetect))
                  {
                      EM_SetErr(eiWK1_FlipperMoveFail);
                      return false ;
                  }

                  if(bCanMvWK1) MoveActr(aiWK1_FlpCmp ,ccBwd);
                  if(bCanMvWK1) MoveActr(aiWK1_Ins    ,ccFwd);
                  if(bCanMvWK1) MoveActr(aiWK1_Stopper,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  17: if(bCanMvWK1) if(!MoveActr(aiWK1_FlpCmp ,ccBwd )) return false ;
                  if(bCanMvWK1) if(!MoveActr(aiWK1_Ins    ,ccFwd )) return false ;
                  if(bCanMvWK1) if(!MoveActr(aiWK1_Stopper,ccBwd )) return false ;
                  if(bCanMvWK1) MoveMotr(miWK1_XIns,piWK1_XInsOut);
                  if(bCanMvWK1){
                      DM.ShiftArrayData(riWR1 , riWR2) ;
                      DM.ARAY[riWR2].FlipX();
                      DM.ARAY[ri2B1] = DM.ARAY[riWR2] ;
                      /*DM.ShiftArrayData(riWK1 , riW2T);*/
                      DM.ARAY[riVS1].SetStat(csNone) ;
                      DM.ARAY[riVS2].SetStat(csUnkwn);
                      DM.ARAY[riWR2].ChangeStat(csWork , csUnkwn) ;
                      //DM.ARAY[riWB2] = DM.ARAY[riWK2] ; 생각해보니 굳이 여기에 있을필요 없고 메뉴얼로 가면 될듯함.
                  }

                  if(bCanMvWK1) IO_SetY(yWR1_FeedingAC , true);    //xWK2_Pkg ?
                  Step.iCycle++ ;
                  return false ;

        case  18: if(bCanMvWK1) if(!MoveMotr(miWK1_XIns,piWK1_XInsOut)) return false ;
                  if(bCanMvWK1 && !IO_GetX(xWR1_IndxOverload))
                  {
                      EM_SetErr(eiWK1_IndexOverload);
                      return false ;
                  }

//                  if(bCanMvWK1){
//                      DM.ShiftArrayData(riWR1 , riWR2) ;
//                      DM.ARAY[riWR2].FlipX();
//                      /*DM.ShiftArrayData(riWK1 , riW2T);*/
//                      DM.ARAY[riVS1].SetStat(csNone) ;
//                      DM.ARAY[riVS2].SetStat(csUnkwn);
//                      DM.ARAY[riWR2].ChangeStat(csWork , csUnkwn) ;
//                      //DM.ARAY[riWB2] = DM.ARAY[riWK2] ; 생각해보니 굳이 여기에 있을필요 없고 메뉴얼로 가면 될듯함.
//                  }

                  if(bCanMvWK1) MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++ ;
                  return false ;

        case  19: if(bCanMvWK1) if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  if(bCanMvWK1) MoveActr(aiWK1_Ins    ,ccBwd);
                  if(bCanMvWK1) MoveActr(aiWK1_Stopper,ccFwd);
                  if(bCanMvWK1) MoveActr(aiWK1_FlpCmp ,ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  20: if(bCanMvWK1) if(!MoveActr(aiWK1_Ins    ,ccBwd)) return false ;
                  if(bCanMvWK1) if(!MoveActr(aiWK1_Stopper,ccFwd)) return false ;
                  if(bCanMvWK1) if(!MoveActr(aiWK1_FlpCmp ,ccFwd)) return false ;
                  if(bCanMvWK1) MoveMotr(miWK1_YFlp,piWK1_YFlpNormal) ;
                  Step.iCycle++ ;
                  return false ;

        case  21: if(bCanMvWK1) if(!MoveMotr(miWK1_YFlp,piWK1_YFlpNormal)) return false ;
                  if(bCanMvWK1 && !IO_GetX(xWR1_TurnDetect))
                  {
                      EM_SetErr(eiWK1_FlipperMoveFail);
                      return false ;
                  }

                  Step.iCycle++ ;
                  return false ;
        case  22:
                  if( bInPkg                        &&!IO_GetX(xPRB_Pkg)) return false ;
//                  if( bCanMvPRB&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWR1_Pkg)) return false ;
                  if( bCanMvWK1&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWR2_Pkg)) return false ;
                  if( bCanMvWK2&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWR3_Pkg)) return false ;
                  if( bCanMvWK3&&!OM.CmnOptn.bDryRun&&!IO_GetX(xPSB_Pkg))
                  {
                      return false ;
                  }

                  if(bInPkg) {
                      DM.ARAY[riPRB].SetStat   (csUnkwn) ;
                  }

                  IO_SetY(yPRB_FeedingAC , false);
                  IO_SetY(yWR1_FeedingAC , false);
//                  if( bCanMvWK3 )IO_SetY(yWR2_FeedingAC , false);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleMvPrb(void) //자동 계산  이동.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    static bool bInPkg = false ;

    if(IO_GetX(xPRB_PkgIn1)) bInPkg = true ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bInPkg = false ;
                  MoveActr(aiWK1_FlpCmp , ccBwd) ;
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiWK1_FlpCmp , ccBwd)) return false ;
                  IO_SetY(yPRB_FeedingAC,true);
                  MoveActr(aiPRB_Stopper , ccBwd) ;
                  MoveActr(aiWK1_Stopper , ccFwd) ;
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveActr(aiPRB_Stopper , ccBwd)) return false ;
                  if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  DM.ShiftArrayData(riPRB , riWR1) ;
                  DM.ARAY[riVS1].SetStat(csUnkwn);

                  Step.iCycle++ ;
                  return false ;

        case  13: if(!IO_GetX(xPRB_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiPRB_Stopper , ccFwd) ;
                  if(!AT_GetCmd(aiPRB_Stopper)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiPRB_Stopper , ccFwd)) return false ;
                  if(m_tmTemp.OnDelay(true , OM.CmnOptn.iPrbToWk1Delay))
                  {
                      EM_SetErr(eiPRB_StripFeedingFail);
                      IO_SetY(yPRB_FeedingAC,false);
                      return false ;
                  }
                  if(IO_GetX(xPRB_PkgOut2) || IO_GetX(xPRB_Pkg) )return false ;//Doking ...X

//                  IO_SetY(yPRB_FeedingAC,false);
                  MoveActr(aiWK1_Ins   ,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiWK1_Ins , ccBwd)) return false ;
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  MoveActr(aiWK1_Ins    ,ccFwd);
                  MoveActr(aiWK1_FlpCmp ,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  17: if(!MoveActr(aiWK1_Ins    ,ccFwd)) return false ;
                  if(!MoveActr(aiWK1_FlpCmp ,ccBwd)) return false ;
//                  MT_GoAbs(miWK1_XIns,GetMotrPos(miWK1_XIns,piWK1_XInsStpr),OM.CmnOptn.dWK1InsStprPsSpeed ) ;
                  MoveMotr(miWK1_XIns,piWK1_XInsStpr);
//                  DM.ShiftArrayData(riPRB , riWR1) ;
//                  DM.ARAY[riVS1].SetStat(csUnkwn);
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!MoveMotr(miWK1_XIns,piWK1_XInsStpr)) return false ;
//                  if(!IO_GetX(xWR1_IndxOverload))
//                  {
//                      EM_SetErr(eiWK1_IndexOverload);
//                      return false ;
//                  }
                  if(!IO_GetX(xWR1_Pkg         )) {
                      EM_SetErr(eiWK1_PkgDispr     );
                      return false ;
                  }


                  MoveActr(aiWK1_Align,ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiWK1_Align,ccFwd)) return false ;
                  MoveActr(aiWK1_FlpCmp,ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  20: if(!MoveActr(aiWK1_FlpCmp,ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC , false);
                  MoveActr(aiWK1_Align,ccBwd);
                  MoveMotr(miWK1_XIns,piWK1_XInsWait);
                  Step.iCycle++ ;
                  return false ;

        case  21: if(!MoveActr(aiWK1_Align,ccBwd)) return false ;
                  if(!MoveMotr(miWK1_XIns,piWK1_XInsWait)) return false ;
                  MoveActr(aiWK1_Ins,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!MoveActr(aiWK1_Ins,ccBwd)) return false ;
                  if( bInPkg &&!IO_GetX(xPRB_Pkg)) return false ;
                  if(bInPkg) {
                      DM.ARAY[riPRB].SetStat   (csUnkwn) ;
                  }

                  IO_SetY(yPRB_FeedingAC, false);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    static bool bExistWK1 = false ;
    static bool bExistWK2 = false ;
    static bool bExistWK3 = false ;    

    bool r1 , r2 , r3 , r4 ;

    //static bool bInPkg = false ;

    //if(IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_PkgIn2)) bInPkg = true ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bExistWK3 = DM.ARAY[riWR3].GetCntExist();
                  bExistWK2 = DM.ARAY[riWR2].GetCntExist();
//                  bExistWK1 = DM.ARAY[riWR1].GetCntExist();

//                  MoveActr(aiWK1_Stopper , ccFwd);
                  MoveActr(aiWK2_Stopper , ccFwd);
                  MoveActr(aiWK3_Stopper , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  11: //if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Stopper , ccFwd)) return false ;
                  IO_SetY(yWR1_FeedingAC , true);
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
//                  IO_SetY(yWR1_FeedingAC , false);

//                  MoveActr(aiWK1_Align , ccFwd);
                  MoveActr(aiWK2_Align , ccFwd);
                  MoveActr(aiWK3_Align , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  13: //if(!MoveActr(aiWK1_Align , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Align , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Align , ccFwd)) return false ;

//                  MoveActr(aiWK1_Align , ccBwd);
//                  MoveActr(aiWK2_Align , ccBwd);
//
//                  Step.iCycle++ ;
//                  return false ;
//
//        case  14: if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
//                  if(!MoveActr(aiWK2_Align , ccBwd)) return false ;

//                  MoveActr(aiWK1_Lift , ccFwd);
                  MoveActr(aiWK2_Lift , ccFwd);
                  MoveActr(aiWK3_Lift , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  14: //if(!MoveActr(aiWK1_Lift , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Lift , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_Lift , ccFwd)) return false ;
                  IO_SetY(yWR1_FeedingAC , false);

//                  MoveActr(aiWK1_Align , ccBwd);
                  MoveActr(aiWK2_Align , ccBwd);
                  MoveActr(aiWK3_Align , ccBwd);                  
                  Step.iCycle++ ;
                  return false ;

        case  15: //if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Align , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_Align , ccBwd)) return false ;
                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.                  DM.ARAY[riWK1].GetCntExist()
//                  if( bExistWK1 && DM.ARAY[riVS1].CheckAllStat(csNone))DM.ARAY[riVS1].SetStat(csUnkwn);
                  if( bExistWK2 && DM.ARAY[riVS2].CheckAllStat(csNone))DM.ARAY[riVS2].SetStat(csUnkwn);
                  if( bExistWK3 && DM.ARAY[riVS3].CheckAllStat(csNone))DM.ARAY[riVS3].SetStat(csUnkwn);
                  Step.iCycle = 0 ;
                  return true ;
    }
}


void CRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRail::CheckStop()
{
    if(!AT_Done(aiPRB_Stopper )) return false ;
    if(!AT_Done(aiWK1_Align   )) return false ;
    if(!AT_Done(aiWK1_FlpCmp  )) return false ;
    if(!AT_Done(aiWK1_Ins     )) return false ;
    if(!AT_Done(aiWK1_Stopper )) return false ;
    if(!AT_Done(aiWK2_Align   )) return false ;
    if(!AT_Done(aiWK2_Lift    )) return false ;
    if(!AT_Done(aiWK2_Stopper )) return false ;
    if(!AT_Done(aiWK3_Align   )) return false ;
    if(!AT_Done(aiWK3_Lift    )) return false ;
    if(!AT_Done(aiWK3_Stopper )) return false ;

    if(!MT_GetStop(miWK1_XIns)) return false ;
    if(!MT_GetStop(miWK1_YFlp)) return false ;

    return true ;
}

void CRail::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CRail","`Load Start") ;

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iPsbiRowCnt" , iPsbiRowCnt  );

    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iPsbiRowCnt" , iPsbiRowCnt  );
    }
    Trace( "CRail","Load End") ;
}

