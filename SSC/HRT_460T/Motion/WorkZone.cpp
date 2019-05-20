//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuffer.h"
#include "WorkZone.h"
#include "PostBuff.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ProbeComUnit.h"
#include "LotUnit.h"
#include "Array.h"
#include "SVInterface.h"

//#include "FormRslt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CWorkZone WRK;

int iPinFailCnt[MAX_ARRAY_ROW][MAX_PROB_COL]  ;


CWorkZone::CWorkZone(void)
{
}

CWorkZone::~CWorkZone (void)
{
}


void CWorkZone::Init()
{
    m_sPartName = "WorkZone " ;

    PCM.SetDaegyeom();

    Reset();
    Load(true);

    DM.ARAY[riWRK].SetStep(m_iLastStep);
    InitCycleName();

    bOverFail = false;
    bYieldErr = false;

    iOverFail = 0;
}

void CWorkZone::Close()
{
    m_iLastStep = DM.ARAY[riWRK].GetStep();
    Load(false);
}

void CWorkZone::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

void CWorkZone::Update()
{
}

void CWorkZone::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    memset(iPinFailCnt , 0 , sizeof(int)*MAX_ARRAY_ROW *  MAX_PROB_COL);
}


bool CWorkZone::FindChip(int &r , int &c )
{
    c = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;

    return (c > -1 && c > -1);
}

double CWorkZone::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r , c , fc , iMaxCol;
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    FindChip(r , c);
    fc = (iMaxCol - c) -1 ; //횡으로 플립된값.

    double dPos = 0.0;
    int    iGrCnt ;
    bool   bGrGrip;
    double dReGripPos;

    double dXBtmWorkPos = PM.GetValue(miWRK_XBtm , pvWRK_XBWorkStt) +
                         (OM.DevInfo.dColPitch * fc) + (OM.DevOptn.dWRKPtchOfs * fc) ;

    double dXBtmGrGap  = 0 ;
    double dXBtmGrPtch = 0 ;
    int    iGr         = 0 ;

    if(OM.DevInfo.iColGrCnt != 0) {
        iGr   = OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt   ;
        dXBtmGrGap   = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;
        dXBtmGrPtch  = dXBtmGrGap * iGr ;
    }

    //double dXGripOfs = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) - OM.DevOptn.dGripOfs;
    //if(fc == 0) dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip);
    if(_iMotr == miWRK_ZTop){
        switch(_iPstnId){
            default            : dPos = MT_GetCmdPos(_iMotr                                      ); break ;
            case piWRK_ZTWait  : dPos = PM.GetValue (_iMotr , pvWRK_ZTWait                       ); break ;
            case piWRK_ZTWork  : dPos = PM.GetValue (_iMotr , pvWRK_ZTWork                       ); break ;
        }
    }
    else if(_iMotr == miWRK_YCam){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                                  ); break ;
            case piWRK_YTWait      : dPos = PM.GetValue (_iMotr , pvWRK_YTWait                   ); break ;
            case piWRK_YTWork      : dPos = PM.GetValue (_iMotr , pvWRK_YTWork                   ); break ;
        }
    }
    else if(_iMotr == miWRK_XBtm){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                                  ); break ;
            case piWRK_XBWait      : dPos = PM.GetValue (_iMotr , pvWRK_XBWait                   ); break ;
            case piWRK_XBIn        : dPos = PM.GetValue (_iMotr , pvWRK_XBIn                     ); break ;
            case piWRK_XBWorkStt   : dPos = PM.GetValue (_iMotr , pvWRK_XBWorkStt                ); break ;
            case piWRK_XBWorkUp    : dPos = dXBtmWorkPos + dXBtmGrPtch - OM.DevOptn.dWorkXWaitOfs ; break ;
            case piWRK_XBWork      : dPos = dXBtmWorkPos + dXBtmGrPtch                            ; break ;
            case piWRK_XBOut       : dPos = PM.GetValue (_iMotr , pvWRK_XBOut                    ); break ;
            case piWRK_XBChange    : dPos = PM.GetValue (_iMotr , pvWRK_XBChange                 ); break ;
        }
    }

    else if(_iMotr == miWRK_YBtm){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_YBWait      : dPos = PM.GetValue (_iMotr , pvWRK_YBWait          ); break ;
            case piWRK_YBIn        : dPos = PM.GetValue (_iMotr , pvWRK_YBIn            ); break ;
            case piWRK_YBWork      : dPos = PM.GetValue (_iMotr , pvWRK_YBWork          ); break ;
            case piWRK_YBChange    : dPos = PM.GetValue (_iMotr , pvWRK_YBChange        ); break ;
        }
    }
    else if(_iMotr == miWRK_ZBtm){
        switch(_iPstnId){
            default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_ZBWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWait          ); break ;
            case piWRK_ZBOut       : dPos = PM.GetValue (_iMotr , pvWRK_ZBOut           ); break ;
            case piWRK_ZBWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWork          ); break ;
            case piWRK_ZBChange    : dPos = PM.GetValue (_iMotr , pvWRK_ZBChange        ); break ;
        }
    }


    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------

bool CWorkZone::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo, m_sPartName.c_str());
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iHome = 0 ;
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
//                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiWRK_Idx,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_Idx , ccBwd))return false ;
                 MT_DoHome(miWRK_ZBtm);
                 MT_DoHome(miWRK_ZTop);
                 MT_DoHome(miWRK_YCam);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRK_ZBtm))return false;
                 if(!MT_GetHomeEnd(miWRK_ZTop))return false;
                 if(!MT_GetHomeEnd(miWRK_YCam))return false;
                 MT_DoHome(miWRK_YBtm);
                 MT_DoHome(miWRK_XBtm);
                 Step.iHome++;
                 return false;

        case 13: if(!MT_GetHomeEnd(miWRK_YBtm))return false;
                 if(!MT_GetHomeEnd(miWRK_XBtm))return false;
                 MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait));
                 MT_GoAbsMan(miWRK_YCam, GetMotrPos(miWRK_YCam,piWRK_YTWait));
                 MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait));
                 Step.iHome++;
                 return false;

        case 14: if(!MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait)))return false;
                 if(!MT_GoAbsMan(miWRK_YCam, GetMotrPos(miWRK_YCam,piWRK_YTWait)))return false;
                 if(!MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait)))return false;
                 MT_GoAbsMan(miWRK_XBtm, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWRK_XBtm, m_dLastIndPos))return false ;
                 MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBWait));
                 Step.iHome = 0;
                 return true ;


    }
}

bool CWorkZone::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xWRK_IdxOvrld) ) {
        EM_SetErrMsg(eiPRT_OverLoad , m_sPartName.c_str());
        MT_Stop(miWRK_XBtm);
        Step.iCycle = 0;
    }
    int r,c,iMaxCol ;
    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        FindChip(r,c);

        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        FindChip(r , c);

        int fc = (iMaxCol - c) -1  ; //원래 데이터를 뒤집은 것.

        bool isCycleIn        = DM.ARAY[riWRK].CheckAllStat(csUnkwn) && DM.ARAY[riWRK].GetStep() == 0 ;
        bool isCycleVisn      = DM.ARAY[riWRK].CheckAllStat(csUnkwn) && DM.ARAY[riWRK].GetStep() != 0 && !OM.CmnOptn.bIgnrVisn && m_iVisnErr != vsGood;
        bool isCycleWork      = DM.ARAY[riWRK].GetCntStat  (csUnkwn) && DM.ARAY[riWRK].GetStep() != 0 && (m_iVisnErr == vsGood || OM.CmnOptn.bIgnrVisn);
        bool isCycleOut       =!DM.ARAY[riWRK].GetCntStat  (csUnkwn) && !DM.ARAY[riWRK].GetCntStat  (csMask ) &&!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riPSB].CheckAllStat(csNone) ;

        bool isCycleEnd      =  DM.ARAY[riWRK].CheckAllStat(csNone ) ;

        //모르는 스트립에러.
        //if(isCycleIn || isCycleWork){
        //    if(DM.ARAY[riWRK].CheckAllStat(csNone) && IO_GetX(xWRK_Detect)) EM_SetErrMsg(eiPKG_Unknwn, m_sPartName.c_str()) ;
        //}

        //스트립 사라짐.
        //if(isCycleOut && !OM.CmnOptn.bDryRun){
        //    if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect)) EM_SetErrMsg(eiPKG_Dispr, m_sPartName.c_str()) ;
        //}
        
        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn        ) {Trace(m_sPartName.c_str(),"CycleIn         Stt"); Step.iSeq = scIn        ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleVisn      ) {Trace(m_sPartName.c_str(),"CycleVisn       Stt"); Step.iSeq = scVisn      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleWork      ) {Trace(m_sPartName.c_str(),"CycleWork       Stt"); Step.iSeq = scWork      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut       ) {Trace(m_sPartName.c_str(),"CycleOut        Stt"); Step.iSeq = scOut       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd       ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default          :                       Trace(m_sPartName.c_str(),"default        End");Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                             return false ;
        case scIn        : if(CycleIn       ()){ Trace(m_sPartName.c_str(),"CycleIn        End");Step.iSeq = scIdle ;} return false ;
        case scVisn      : if(CycleVisn     ()){ Trace(m_sPartName.c_str(),"CycleVisn      End");Step.iSeq = scIdle ;} return false ;
        case scWork      : if(CycleWork     ()){ Trace(m_sPartName.c_str(),"CycleWork      End");Step.iSeq = scIdle ;} return false ;
        case scOut       : if(CycleOut      ()){ Trace(m_sPartName.c_str(),"CycleOut       End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;

}

bool CWorkZone::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    Stat.bReqStop = true ;
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;


}

bool CWorkZone::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false; ///fail 시 계속 검사 할수도...  Stat.iReInspCnt = 0;

    Step.iToStart = 10 ;

    //Ok.
    return true;
}

bool CWorkZone::ToStart(void) //스타트를 하기 위한 함수.
{
    AnsiString sTemp ;
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) {
        EM_SetErrMsg(eiPRT_ToStartTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iToStart = 0 ;
        return true;
    }
    if(Step.iToStart != PreStep.iToStart) {
        sTemp = sTemp.sprintf("%s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: MoveMotr(miWRK_ZBtm , piWRK_ZBChange);
                 MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                 MoveMotr(miWRK_YCam , piWRK_YTWait);
                 Step.iToStart++;
                 return false;

        case 11: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBChange))return false;
                 if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait))return false;
                 if(!MoveMotr(miWRK_YCam , piWRK_YTWait))return false;
                 MoveMotr(miWRK_XBtm, piWRK_XBWait);
                 MoveMotr(miWRK_YBtm, piWRK_YBWait);
                 Step.iToStart ++;
                 return false ;

        case 12: if(!MoveMotr(miWRK_XBtm, piWRK_XBWait))return false;
                 if(!MoveMotr(miWRK_YBtm, piWRK_YBWait))return false;
                 MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait))return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWorkZone::ToStop(void) //스탑을 하기 위한 함수.
{
    AnsiString sTemp ;
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) {
        EM_SetErrMsg(eiPRT_ToStopTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iToStart = 0 ;
        return true;
    }

    if(Step.iToStop != PreStep.iToStop) {
        sTemp = sTemp.sprintf("%s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }


    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11:
                 OM.SaveDevOptn(OM.m_sCrntDev);  //핀카운트 저장을 위해서.
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CWorkZone::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
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
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(DM.ARAY[riWRK].GetLotNo() != m_sLastStrip) {
                      LOT.LotOpen(DM.ARAY[riWRK].GetLotNo()) ;
                      m_sLastStrip = DM.ARAY[riWRK].GetLotNo() ;
                      bYieldErr = false;
                      Trace("First Strip",("LotOpen_" + DM.ARAY[riWRK].GetLotNo()).c_str());
                  }
                  MoveActr(aiWRK_Idx , ccBwd);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_YCam , piWRK_YTWork);   //확인이 급하지 않아 나중에 함.
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWRK_Idx , ccBwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false;
                  MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XBtm , piWRK_XBWait))return false;
                  if(!MoveMotr(miWRK_YBtm , piWRK_YBWait))return false;
                  //MoveActr(aiWRK_Idx , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: //if(!MoveActr(aiWRK_Idx , ccFwd))return false;
                  //MoveMotr(miWRK_XBtm , piWRK_XBContct);
                  Step.iCycle++;
                  return false;

        case  14: //if(!MoveMotr(miWRK_XBtm , piWRK_XBContct))return false;
                  if(!MoveMotr(miWRK_YCam , piWRK_YTWork  ))return false;
                  PCM.ClearData();
                  DM.ARAY[riWRK].ClearAllVf();
                  DM.ARAY[riWRK].ClearAllVr();
                  DM.ARAY[riWRK].SetStep(1);
                  m_iVisnErr = vsNomal;
                  iOverFail = 0;
                  Step.iCycle = 0;
                  return true ;

    }
}


bool CWorkZone::CycleVisn()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
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
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //TRetResult OriRslt ;
    //AnsiString sRslt;
    bool bRslt ;
    char sRslt[32];


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(OM.CmnOptn.bIgnrVisn) {
                      Step.iCycle = 17;
                      return false;
                  }
                  m_iTopMaskMoveCnt = 0;
                  Step.iCycle++;
                  return false;

        case  11: MoveMotr(miWRK_YCam , piWRK_YTWork);
                  //MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  MT_GoAbsRun(miWRK_ZTop, PM.GetValue(miWRK_ZTop ,pvWRK_ZTWork) - 5);
                  Step.iCycle++;
                  return false;

        case  12: if(!MT_GoAbsRun(miWRK_ZTop, PM.GetValue(miWRK_ZTop ,pvWRK_ZTWork) - 5))return false;
                  MT_GoAbs(miWRK_ZTop , PM.GetValue(miWRK_ZTop ,pvWRK_ZTWork), OM.DevOptn.dWRKZTopSpeed);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_YCam , piWRK_YTWork)) return false ;
                  //if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  if(!MT_GoAbs(miWRK_ZTop , PM.GetValue(miWRK_ZTop , pvWRK_ZTWork), OM.DevOptn.dWRKZTopSpeed))return false;
                  if(OM.DevOptn.bWRKZTopRepeat && m_iTopMaskMoveCnt == 0){
                      m_iTopMaskMoveCnt++;
                      Step.iCycle = 11;
                      return false;
                  }
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true, 1000))return false;
                  SV_Grab(0);
                  Step.iCycle++;
                  return false ;

        case  15: if(!SV_GetGrabEnd(0)) return false ;
                  SV_Inspect(0); //쓰레드 안씀.
                  Step.iCycle++;
                  return false ;

        case  16: if(!SV_GetInspEnd(0)) return false ;
                  Step.iCycle++;
                  return false;

        case  17: //bRslt = SV_GetRslt(0 , sRslt);
                  bRslt = SV_GetRslt(0 , sRslt);

                  if(!bRslt && !OM.CmnOptn.bIgnrVisn){
                      //m_bVisnErrDisp = true;
                      Step.iCycle = 20;
                      return false;
                  }
                  else {
                      m_iVisnErr = vsGood;
                      Step.iCycle=0;
                      return true;
                  }


        case  20: MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_YCam , piWRK_YTWait);
                  Step.iCycle++;
                  return false;

        case  21: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait))return false ;
                  if(!MoveMotr(miWRK_YCam , piWRK_YTWait))return false ;
                  EM_SetErr(eiVSN_InspNG);
                  Step.iCycle = 0;
                  return true;

    }
}

bool CWorkZone::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iLedTime = OM.DevOptn.i1ChLedTime > OM.DevOptn.i2ChLedTime ? OM.DevOptn.i1ChLedTime * 100 : OM.DevOptn.i2ChLedTime * 100;
    int iZrTime  = OM.DevOptn.i1ChZenTime > OM.DevOptn.i2ChZenTime ? OM.DevOptn.i1ChZenTime * 100 : OM.DevOptn.i2ChZenTime * 100;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iLedTime + iZrTime + 2000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
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
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //TRetResult OriRslt ;

    int r,c ;
    bool r1,r2 ;
    int iMaxRow,iMaxCol,icsWork,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();

    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;

    static int  iReInspCnt = 0 ;
    static int  iCmmErrCnt = 0 ;
    static int  iChipCnt = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt;
    int iChipTemp = iChipCnt - DM.ARAY[riWRK].GetCntStat(csUnkwn);
    AnsiString sPinCount ;

    //static bool bRepeatAtStart = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRK_Idx , ccBwd);
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  MoveActr(aiWRK_Idx  , ccBwd       );
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  if(!MoveActr(aiWRK_Idx  , ccBwd       )) return false ;
                  MoveMotr(miWRK_XBtm, piWRK_XBWorkUp);
                  MoveMotr(miWRK_YBtm, piWRK_YBWork  );
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miWRK_XBtm, piWRK_XBWorkUp)) return false ;
                  if(!MoveMotr(miWRK_YBtm, piWRK_YBWork  )) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false;
                  MoveMotr(miWRK_XBtm , piWRK_XBWork);
                  Step.iCycle++;
                  return false ;

        //주의 밑에서 사용.
        case  15: if(!MoveMotr(miWRK_XBtm , piWRK_XBWork)) return false;
                  Step.iCycle++;
                  return false ;


        case  16: OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmDelay.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  17: if(!m_tmDelay.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;
                  if(OM.CmnOptn.bIgnrInsp) {
                      PCM.bPb1Rslt = true ;
                      PCM.bPb2Rslt = true ;

                  }
                  else {
                      if(!OM.MstOptn.bUsed4RowCntct){
                          PCM.bPb1Rslt = false ;
                          PCM.bPb2Rslt = true  ;
                      }
                      else {
                          PCM.bPb1Rslt = false ;
                          PCM.bPb2Rslt = false ;
                      }
                      if(OM.MstOptn.bByChInsp || !OM.MstOptn.bUsed4RowCntct) PCM.Start(1);
                      else                                                   PCM.Start( );
                  }
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(m_tmDelay.OnDelay(true , OM.DevOptn.i1ChLedTime * 100 + 1000)) {
                      if(!PCM.bPb1Rslt) {
                          MoveMotr(miWRK_XBtm, piWRK_XBWorkUp) ;
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  if(!PCM.bPb1Rslt)return false;
                  Trace("Probe1","Result OK");
                  if(OM.MstOptn.bByChInsp && OM.MstOptn.bUsed4RowCntct) PCM.Start(2);
                  else  {
                      Step.iCycle = 20;
                      return false ;
                  }
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  19: if(m_tmDelay.OnDelay(true , OM.DevOptn.i2ChLedTime * 100 + 1000)) {
                      if(!PCM.bPb2Rslt) {
                          MoveMotr(miWRK_XBtm, piWRK_XBWorkUp) ;
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPb2Rslt)return false;

                  if(!PCM.bPb2Rslt) return false;
                  Trace("Probe2","Result OK");
                  Step.iCycle++;
                  return false ;

        case  20: MoveMotr(miWRK_XBtm, piWRK_XBWorkUp) ;
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miWRK_XBtm, piWRK_XBWorkUp)) return false;
                  //마스킹 패턴 및 재 검사 패턴.
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //데이타 마스킹.
                      }
                  }
                  else {
                      if(OM.MstOptn.bUsed4RowCntct){
                          r1 = PCM.CheckDataCnt(riWRK , PCM.sPb1Msg);
                          r2 = PCM.CheckDataCnt(riWRK , PCM.sPb2Msg);

                          if(!r1){ Step.iCycle = 40 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //데이터 마스킹.  나중에 PCM에 ARAY 걷어 내고 여기서 데이터 받아서 마스킹 한다.
                          if(!r2){ Step.iCycle = 40 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                          if(r1 && r2) {
                              PCM.ClearDataCrnt();
                              PCM.GetDataCh(riWRK , PCM.sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                              PCM.GetDataCh(riWRK , PCM.sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
                          }
                      }
                      else{
                          r1 = PCM.CheckDataCnt(riWRK , PCM.sPb1Msg);
                          if(!r1){ Step.iCycle = 40 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ;
                          if(r1) {
                              PCM.ClearDataCrnt();
                              PCM.GetDataCh(riWRK , PCM.sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          }
                      }
                  }
                  iFail = 0;
                  for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                      iFail += DM.ARAY[riWRK].GetCntColStat(c - j, csFail); //데이타 마스킹.
                  }

                  //재검사 패턴 및 리피트 테스트 패턴.
                  if (iFail >= OM.DevOptn.iLimInsFailCnt || Stat.bRepeat){
                      if(iReInspCnt < OM.DevOptn.iReInspCnt || Stat.bRepeat){
                          for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                              for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                          }
                          Step.iCycle = 15 ;
                          iReInspCnt++;
                          return false;
                      }
                  }

                  //프로브 개별 핀에 대한 카운트에러
                  //MAX_PROB_COL == 4 have to check.
                  for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                      for(int i =0 ; i < iMaxRow ; i++) {
                          if(DM.ARAY[riWRK].GetStat(i ,c - j)==csFail) {
                              iPinFailCnt[i][OM.DevOptn.iContColCnt-j]++ ;
                              if(OM.DevOptn.iPinCnt && !OM.DevOptn.bSkipPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount.c_str());
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
                      }
                  }

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  //옵션 추가후 변경. JS...
                  if(iFail >= OM.DevOptn.iFrtPrcFailCnt){
                      bOverFail = true;
                      bOverErr  = true;
                      iOverFail += iFail;
                  }
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  Step.iCycle=0;
                  return true;


        //PCM.CheckDataCnt 데이터 체크 Fail시 사용 스텝.
        case  40: if(MoveMotr(miWRK_ZBtm , piWRK_ZBWait))return false;
                  Step.iCycle++;
                  return false;

        case  41: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //검사 실패시 사용 되는 스텝.
        case  50: MoveMotr(miWRK_XBtm , piWRK_XBWork);
                  Step.iCycle++;
                  return false;

        case  51: if(!MoveMotr(miWRK_XBtm , piWRK_XBWork)) return false ;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  if(iCmmErrCnt < 4) { //검사결과 가끔 잘 안날라 와서 리트라이 넣음.
                      Step.iCycle = 15 ;
                      if(!PCM.bPb1Rslt                                              ) Trace("Cmmunication","g_bPb1Rslt ComRetry");
                      if( PCM.bPb1Rslt && !PCM.bPb2Rslt && OM.MstOptn.bUsed4RowCntct) Trace("Cmmunication","g_bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < 5) { //정 안돼면 세팅 다시해봄.

                      Step.iCycle = 15 ;
                      if(!PCM.bPb1Rslt                                              ) Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                      if( PCM.bPb1Rslt && !PCM.bPb2Rslt && OM.MstOptn.bUsed4RowCntct) Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");

                      PCM.SetDaegyeom();
                      //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
                      return false ;
                  }

                  //그래도 안되면 에러.
                  if(OM.MstOptn.bUsed4RowCntct)
                  if(!PCM.bPb1Rslt                                              ) EM_SetErrMsg(eiWRK_FailProbeComm,"Probe Com 1");
                  if( PCM.bPb1Rslt && !PCM.bPb2Rslt && OM.MstOptn.bUsed4RowCntct) EM_SetErrMsg(eiWRK_FailProbeComm,"Probe Com 2");
                  Step.iCycle++;
                  return false;

        case  52: 
                  Step.iCycle = 0 ;
                  return false ;
    }
}

bool CWorkZone::CycleOut()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
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
    //    Step.iCycle = 0;
    //    return true ;
    //}

    int r,c;
    bool r1,r2 ;
    FindChip(r,c);
    int iMaxRow,iMaxCol,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    //static bool bErr = false ;
    AnsiString sPinCount ;

    int iWorkCnt, iGoodCnt, iFailCnt;

    double dYield;

    static int iCmmErrCnt = 0 ;

    static int iInspCnt = 0 ;
    static bool bLastStrip;
    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(bOverErr){
                      EM_SetErr(eiWRK_OverFail);
                      bOverErr = false;
                      Step.iCycle = 0;
                      return true;
                  }
                  Step.iCycle++;
                  return false;

        case  11: if(bOverErr) return false;
                  if(bOverFail){
                      iWorkCnt = DM.ARAY[riWRK].GetCntExist();
                      iGoodCnt = DM.ARAY[riWRK].GetCntStat(csGood);
                      iFailCnt = iWorkCnt - (iGoodCnt + iOverFail);
                      LOT.AddDayInfoWorkStrp(1);
                      LOT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LOT.AddDayInfoFailChip(iFailCnt);
                      LOT.AddDayInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LOT.AddDayInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                      LOT.AddLotInfoWorkStrp(1);
                      LOT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LOT.AddLotInfoFailChip(iFailCnt);
                      LOT.AddLotInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LOT.AddLotInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                      bOverFail = false;
                  }
                  else{
                      LOT.AddDayInfoWorkStrp(1);
                      LOT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LOT.AddDayInfoFailChip(DM.ARAY[riWRK].GetCntExist() - DM.ARAY[riWRK].GetCntStat(csGood));
                      LOT.AddDayInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LOT.AddDayInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                      LOT.AddLotInfoWorkStrp(1);
                      LOT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LOT.AddLotInfoFailChip(DM.ARAY[riWRK].GetCntExist() - DM.ARAY[riWRK].GetCntStat(csGood));
                      LOT.AddLotInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LOT.AddLotInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                  }


                  bLastStrip = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPRB].GetLotNo()   && !DM.ARAY[riPRB].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo()   &&(!DM.ARAY[riLDR].CheckAllStat(csNone)  && DM.ARAY[riPRB].CheckAllStat(csNone)))||
                               (LDR.GetWorkEnd() && DM.ARAY[riPRB].CheckAllStat(csNone)) || //&& LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt ||
                               (DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPRB].CheckAllStat(csNone)); //내려놓는 중에 하나 걸어놓음.


                               //기존.
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR1].GetLotNo() && !DM.ARAY[riPR1].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR2].GetLotNo() && !DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() && (DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone))) ||
                               //(LDR.GetWorkEnd() && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)); //내려놓는 중에 하나 걸어놓음.

                  if(bLastStrip) {
                      m_bDispLotEnded = true ;
                      memcpy(&LOT.EndInfo , &LOT.LotInfo , sizeof(CLot::SLotInfo));

                      Trace("LastStrip",DM.ARAY[riWRK].GetLotNo().c_str());

                      //LotEnd할려고 하는데 PostBuff에서
                      //LOT.LotInfo.dEndTime = Now() ;
                      LOT.LotInfo.dEndTime = Now() ;
                      LOT.WriteLotLog   ();
                      LOT.WriteLotDayLog(); // 데이인포 갱신은 업데이트에서 하고 여기서 쓴다.
                      if(OM.DevOptn.bSkipPinCnt){
                          OM.DevOptn.bSkipPinCnt = !OM.DevOptn.bSkipPinCnt ;
                      }
                  }
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait))return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait))return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBIn);
                  MoveMotr(miWRK_XBtm , piWRK_XBIn);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_YBtm , piWRK_YBIn))return false ;
                  if(!MoveMotr(miWRK_XBtm , piWRK_XBIn))return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBOut);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBOut))return false;
                  MoveActr(aiWRK_Idx , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiWRK_Idx , ccFwd)) return false;
                  MoveMotr(miWRK_XBtm , piWRK_XBOut);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRK_XBtm , piWRK_XBOut))return false ;
                  MoveActr(aiWRK_Idx , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: //DM.ARAY[riWRK].SetSubStep(0);
                  if(!MoveActr(aiWRK_Idx , ccBwd))return false;
                  Step.iCycle++;
                  return false;

        case  18: MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  dYield = LOT.LotInfo.iWorkChip ? (LOT.LotInfo.iWorkChip - LOT.LotInfo.iFailChip) / (float)LOT.LotInfo.iWorkChip * 100 : 100 ;
                  //옵션 추가. 2개다 누가? 진섭이가
                  //1개는 bool 1개는 double ㅇㅋ?
                  PCM.ShiftArrayData(riWRK);
                  DM.ShiftArrayData(riWRK,riPSB);
                  if(dYield < OM.CmnOptn.dStandardYield && !OM.CmnOptn.bSkipYieldErr && !bYieldErr){
                      EM_SetErr(eiETC_YeildLow);
                      bYieldErr = true;
                  }
                  m_iVisnErr = vsNomal;

                  Step.iCycle=0;
                  return true;
    }
}

bool CWorkZone::CycleToolWait()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait );
                  MoveMotr(miWRK_XBtm , piWRK_XBWait );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait )) return false ;
                  if(!MoveMotr(miWRK_XBtm , piWRK_XBWait )) return false ;
                  Step.iCycle=0;
                  return true;

    }

}
bool CWorkZone::CycleToolChange()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_ZBtm , piWRK_ZBChange);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait  );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBChange)) return false;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait  )) return false;
                  MoveMotr(miWRK_XBtm , piWRK_XBChange);
                  //MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XBtm , piWRK_XBChange)) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBChange);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_YBtm , piWRK_YBChange)) return false ;
                  Step.iCycle=0;
                  return true;

    }

}

bool CWorkZone::CycleVisnErr()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }


    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false ;
                  if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  Step.iCycle=0;
                  return true;
    }

}

bool CWorkZone::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //bool isContect    =  MT_CmprPos(miWRK_ZTop , PM.GetValue(miWRK_ZTop,pvWRK_ZTWait)) ;
    bool isZbtmWork   =  MT_GetCmdPos(miWRK_ZBtm) > PM.GetValue(miWRK_ZBtm,pvWRK_ZBWait) + 1 &&
                         MT_GetStop(miWRK_ZBtm);

    if(_iMotr == miWRK_ZTop){
        switch(_iPstnId){
            default                :                                                                            break ;
            case piWRK_ZTWait      :                                                                            break ;
            case piWRK_ZTWork      :                                                                            break ;
        }
    }

    else if(_iMotr == miWRK_YCam){
        switch(_iPstnId) {
            default                :                                                                            break ;
            case piWRK_YTWait      :                                                                            break ;
            case piWRK_YTWork      :                                                                            break ;
        }
    }
    else if(_iMotr == miWRK_XBtm){
        //if(!isZbtmWork) {sMsg = (AnsiString)MT_GetName(miWRK_ZBtm) + " is Not Wait Position" ; bRet = false; }
        switch(_iPstnId) {
            default                :                                                                            break ;
            case piWRK_XBWait      :                                                                            break ;
            case piWRK_XBIn        :                                                                            break ;
            case piWRK_XBWorkStt   :                                                                            break ;
            case piWRK_XBWorkUp    :                                                                            break ;
            case piWRK_XBWork      :                                                                            break ;
            case piWRK_XBOut       :                                                                            break ;
            case piWRK_XBChange    :                                                                            break ;
        }
    }
    else if(_iMotr == miWRK_YBtm){
        if(isZbtmWork) {sMsg = (AnsiString)MT_GetName(miWRK_ZBtm) + " is Not Wait Position" ; bRet = false; }
        switch(_iPstnId) {
            default                :                                                                            break ;
            case piWRK_YBWait      :                                                                            break ;
            case piWRK_YBIn        :                                                                            break ;
            case piWRK_YBWork      :                                                                            break ;
            case piWRK_YBChange    :                                                                            break ;
        }
    }
    else if(_iMotr == miWRK_ZBtm){
        switch(_iPstnId) {
            default                :                                                                            break ;
            case piWRK_ZBWait      :                                                                            break ;
            case piWRK_ZBWork      :                                                                            break ;
            case piWRK_ZBOut       :                                                                            break ;
            case piWRK_ZBChange    :                                                                            break ;
        }
    }

    else{
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CWorkZone::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isXMoving = !MT_GetStopInpos(miWRK_XBtm);

    if(_iActr == aiWRK_Idx) {
        if( !_bFwd) {
            if(isXMoving) { sMsg = (AnsiString)MT_GetName(miWRK_XBtm)+" is Moving" ; bRet = false ;}
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}


bool CWorkZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlow) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}

bool CWorkZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}


void CWorkZone::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CWorkZone::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CWorkZone::CheckStop()
{
    if(!MT_GetStop(miWRK_XBtm)) return false ;
    if(!MT_GetStop(miWRK_YBtm)) return false ;
    if(!MT_GetStop(miWRK_YCam)) return false ;
    if(!MT_GetStop(miWRK_ZBtm)) return false ;
    if(!MT_GetStop(miWRK_ZTop)) return false ;

    if(!AT_Done(aiWRK_Idx    )) return false ;

    return true ;
}

void CWorkZone::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\WorkZone.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Load(sPath, "Member" , "m_iVisnErr"    , m_iVisnErr    );
        UserINI.Load(sPath, "Member" , "m_iArayStep"   , m_iArayStep   );


    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Save(sPath, "Member" , "m_iVisnErr"    , m_iVisnErr    );
        UserINI.Save(sPath, "Member" , "m_iArayStep"   , m_iArayStep   );
    }
}
//---------------------------------------------------------------------------


