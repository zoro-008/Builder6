//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "WorkZone.h"
#include "PostBuff.h"
//---------------------------------------------------------------------------
#include "FormMain.h"
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
#include "SAInterface.h"


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
    m_sPartName = "WorkZone " ;
    Reset();
    Load(true);

    DM.ARAY[riWRK].SetStep(m_iLastStep);
    InitCycleName();
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

void CWorkZone::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CWorkZone::~CWorkZone (void)
{
}

void CWorkZone::Close()
{
    m_iLastStep = DM.ARAY[riWRK].GetStep();
    Load(false);
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

    if(OM.DevInfo.iColGrCnt > 0){
        iGrCnt      =  OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt ;
        bGrGrip     =  fc % iGrCnt ;
    }
    else {
        bGrGrip = false;
    }

    double dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) +
                         (OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt) + OM.DevOptn.dWRKPtchOfs;

    double dCmpGrGap   = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;

    /*
    if(DM.ARAY[riWRK].GetStep() == 0) { dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip); dReGripPos  = PM.GetValue(miWRK_XCmp , pvWRK_XGrip); }
    else                                dReGripPos  = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) - OM.DevOptn.dWRKReGripOfs;

    if(fc == 0) dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip);

    if(_iMotr == miWRK_XCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_XWait       : dPos = PM.GetValue (_iMotr , pvWRK_XWait      ); break ;
            case piWRK_XGrip       : dPos = dReGripPos                              ; break ;
            case piWRK_XWorkStart  : dPos = PM.GetValue (_iMotr , pvWRK_XWorkStart ); break ;
            case piWRK_XWork       : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap    ;
                                     else         dPos = dCmpMovePos                ; break ;
        }
    }
    */
    double dXGripOfs = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) - OM.DevOptn.dGripOfs;
    if(fc == 0) dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip);

    if(_iMotr == miWRK_XCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_XWait       : dPos = PM.GetValue (_iMotr , pvWRK_XWait      ); break ;
            case piWRK_XGrip       : dPos = PM.GetValue (_iMotr , pvWRK_XGrip      ); break ;
            case piWRK_XWorkStart  : dPos = PM.GetValue (_iMotr , pvWRK_XWorkStart ); break ;
            case piWRK_XWork       : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap    ;
                                     else         dPos = dCmpMovePos                ; break ;
            case piWRK_XGripOfs    : dPos = dXGripOfs                               ; break ;
        }
    }

    else if(_iMotr == miWRK_ZTop){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_ZTWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZTWait          ); break ;
            case piWRK_ZTWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZTWork          ); break ;
        }
    }
    else if(_iMotr == miWRK_ZBtm){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_ZBWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWait          ); break ;
            case piWRK_ZBWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWork          ); break ;
            case piWRK_ZBChange    : dPos = PM.GetValue (_iMotr , pvWRK_ZBChange        ); break ;
        }
    }
    else if(_iMotr == miWRK_YBtm){
        switch(_iPstnId){
            default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_YBWait      : dPos = PM.GetValue (_iMotr , pvWRK_YBWait          ); break ;
            case piWRK_YBWork      : dPos = PM.GetValue (_iMotr , pvWRK_YBWork          ); break ;
            case piWRK_YBChange    : dPos = PM.GetValue (_iMotr , pvWRK_YBChange        ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CWorkZone::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isContect    = !MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWait)) ||
                        !MT_CmprPos(miWRK_ZTop , PM.GetValue(miWRK_ZTop,pvWRK_ZTWait)) ;

    bool isXCmpMove   = !MT_GetStopInpos(miWRK_XCmp);
    bool isXCmpWorkStt=  MT_CmprPos(miWRK_XCmp , PM.GetValue(miWRK_XCmp,pvWRK_XWorkStart)) && MT_GetStop(miWRK_XCmp);

    bool isZbtmChange =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBChange)) && MT_GetStop(miWRK_ZBtm);
    bool isZbtmWork   =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWork  )) && MT_GetStop(miWRK_ZBtm);

    bool isYbtmWait   =  MT_CmprPos(miWRK_YBtm , PM.GetValue(miWRK_YBtm,pvWRK_YBWait  )) && MT_GetStop(miWRK_YBtm);

    if(_iMotr == miWRK_XCmp){
        switch(_iPstnId) {
            default                   :                                                                            break ;
            case piWRK_XWait          :                                                                            break ;
            case piWRK_XGrip          :                                                                            break ;
            case piWRK_XWorkStart     :  if(isContect) {sMsg = "During Inspection"               ; bRet = false; } break ;
            case piWRK_XWork          :                                                                            break ;
            case piWRK_XGripOfs       :                                                                            break ;
        }
    }
    else if(_iMotr == miWRK_ZTop){
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_ZTWait         :  break ;
            case piWRK_ZTWork         :  if(isXCmpMove   ) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"            ; bRet = false ; } break;
        }
    }
    else if(_iMotr == miWRK_ZBtm){
        if(!isYbtmWait  ) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ; } 
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_ZBWait         :  break ;
            case piWRK_ZBChange       :  break ;
            case piWRK_ZBWork         :  if(isXCmpMove   ) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"            ; bRet = false ; } break ;
        }
    }
    else if(_iMotr == miWRK_YBtm){
        if(!isZbtmChange && !isZbtmWork) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ;}
        //사이드 뷰 땜에 Z축 Work 위치에서 이동 떔에 주석...JS
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_YBWait         :  break ;
            case piWRK_YBWork         :  break ;
            case piWRK_YBChange       :  if(!isZbtmChange) {sMsg = MT_GetName(miWRK_YBtm) + " is not Change Position" ; bRet = false; break ;}
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
bool  CWorkZone::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isXMoving = !MT_GetStopInpos(miWRK_XCmp);

    if(_iActr == aiWRK_Cmp) {
        if( !_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miWRK_XCmp)+" is Moving" ; bRet = false ;}
        }
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

bool CWorkZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CWorkZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWorkZone::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_HomeTO);
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
//                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiWRK_Cmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_Cmp,ccBwd))return false ;
                 MT_DoHome(miWRK_ZBtm);
                 MT_DoHome(miWRK_ZTop);
                 MT_DoHome(miWRK_XCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRK_ZBtm))return false;
                 if(!MT_GetHomeEnd(miWRK_ZTop))return false;
                 if(!MT_GetHomeEnd(miWRK_XCmp))return false;
                 MT_DoHome(miWRK_YBtm);
                 Step.iHome++;
                 return false;

        case 13: if(!MT_GetHomeEnd(miWRK_YBtm))return false;
                 MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait));
                 Step.iHome++;
                 return false;

        case 14: if(!MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait)))return false;
                 MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBWait));
                 MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait));
                 MT_GoAbsMan(miWRK_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBWait)))return false ;
                 if(!MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait)))return false ;
                 if(!MT_GoAbsMan(miWRK_XCmp, m_dLastIndPos))return false ;
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

    if ( IO_GetX(xWRK_Overload) ) EM_SetErr(eiWRK_Overload);

    int r,c,iMaxCol ;
    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        //Cycle동작에 관련 되어 있으면 SetSubStep으로 사용.
        //Cycle동작말고 자재와 관련 있으면 SetStep으로 사용함. JS 2013. 9. 11.
        if (Stat.bReqStop)return false ;

        FindChip(r,c);

        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        FindChip(r , c);

        int fc = (iMaxCol - c) -1  ; //원래 데이터를 뒤집은 것.

        bool bExstWorkOkPst   = !DM.ARAY[riPSB].CheckAllStat(csNone) && (PSB.m_iReGripCnt == 1 || DM.ARAY[riPSB].GetStep() == 1 || DM.ARAY[riWRK].GetStep() < OM.DevOptn.iWRKMoveCnt / 2);
        bool isPSBWorkZone    =  PSB.GetCycleStep() == CPostBuff::scContact ;

        bool isCycleIn        = DM.ARAY[riWRK].CheckAllStat(csUnkwn) && DM.ARAY[riWRK].GetSubStep() == 0;
        bool isCycleWork      = DM.ARAY[riWRK].GetCntStat  (csUnkwn) && DM.ARAY[riWRK].GetStep() < OM.DevOptn.iWRKMoveCnt && (DM.ARAY[riPSB].CheckAllStat(csNone) || bExstWorkOkPst);
        bool isCycleOut       =!DM.ARAY[riWRK].GetCntStat  (csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riPSB].CheckAllStat(csNone) && !isPSBWorkZone;

        bool isCycleEnd      =  DM.ARAY[riWRK].CheckAllStat(csNone ) ;

        //모르는 스트립에러.
        if(isCycleIn || isCycleWork){
            if(DM.ARAY[riWRK].CheckAllStat(csNone) && !DM.ARAY[riPSB].GetSubStep() && (IO_GetX(xWRK_Detect1) || IO_GetX(xWRK_Detect2))) EM_SetErr(eiWRK_Unknown) ;
        }

        //스트립 사라짐.
        if(isCycleOut && !OM.CmnOptn.bDryRun){
            if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect1) && !IO_GetX(xWRK_Detect2)) EM_SetErr(eiWRK_Dispr) ;
        }

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn        ) {Trace(m_sPartName.c_str(),"CycleIn         Stt"); Step.iSeq = scIn        ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
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
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiWRK_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(DM.ARAY[riWRK].CheckAllStat(csNone)) MoveActr(aiWRK_Cmp , ccBwd);
                 MoveMotr(miWRK_YBtm , piWRK_YBWait);
                 Step.iToStart ++;
                 return false ;

        case 11: if(DM.ARAY[riWRK].CheckAllStat(csNone) && !MoveActr(aiWRK_Cmp , ccBwd)) return false ;
                 if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                 MoveMotr(miWRK_ZTop,piWRK_ZTWait) ;
                 MoveMotr(miWRK_ZBtm,piWRK_ZBWait) ;
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait))return false ;
                 if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait))return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWorkZone::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiWRK_ToStopTO);

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

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11: if(!MT_GetAlarm   (miWRK_XCmp) &&
                    !MT_GetNLimSnsr(miWRK_XCmp) &&
                    !MT_GetPLimSnsr(miWRK_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miWRK_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miWRK_XCmp , piWRK_XWait) ;

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
        EM_SetErr(eiWRK_CycleTO);
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
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRK_Cmp , ccBwd);
                  if(DM.ARAY[riWRK].GetLotNo() != m_sLastStrip) {
                      LT.LotOpen(DM.ARAY[riWRK].GetLotNo()) ;
                      m_sLastStrip = DM.ARAY[riWRK].GetLotNo() ;
                      Trace("First Strip",("LotOpen_" + DM.ARAY[riWRK].GetLotNo()).c_str());
                  }
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWait );
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XCmp , piWRK_XWait )) return false;
                  if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false;
                  MoveActr(aiWRK_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWorkStart);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRK_XCmp , piWRK_XWorkStart)) return false;
                  PCM.ClearData();
                  DM.ARAY[riWRK].SetSubStep(1);
                  DM.ARAY[riWRK].SetStep(0);
                  Step.iCycle = 0;
                  return true ;

        /*
        case  16: SV_Grab(viOri);
                  Step.iCycle++;
                  return false ;

        case  17: if(!SV_GetGrabEnd(viOri)) return false ;
                  SV_Inspect(viOri , false); //쓰레드 안씀.
                  SV_GetRslt(viOri , &OriRslt);

                  if(OriRslt.iRet != veOriOk) {
                      EM_SetErr(eiWRK_VisnNG);
                  }
                  */



                  Step.iCycle = 0;
                  return true ;

    }
}

bool CWorkZone::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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
    //    Step.iCycle = 0;
    //    return true ;
    //}

    TRetResult OriRslt ;

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

        case  10: if(DM.ARAY[riWRK].CheckAllStat(csUnkwn) && OM.MstOptn.bUsedVision){ //1호기 비전 없어서 Option처리 JH
                      SV_Grab(viOri);
                      m_tmTemp.Clear();
                      Step.iCycle++;
                      return false ;
                  }
                  Step.iCycle = 12;
                  return false;

        case  11: if(!m_tmTemp.OnDelay(true , 1000)){

                  }
                  if(!SV_GetGrabEnd(viOri)) return false ;
                  SV_Inspect(viOri , false); //쓰레드 안씀.
                  SV_GetRslt(viOri , &OriRslt);

                  if(OriRslt.iRet != veOriOk) {
                      EM_SetErr(eiWRK_VisnNG);
                      Step.iCycle = 0;
                      return true;
                  }
                  Step.iCycle++;
                  return false;


        case  12: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp  , ccBwd       );
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp  , ccBwd       )) return false ;
                  if(iChipTemp < OM.DevInfo.iRowCnt * OM.DevOptn.iContColCnt) MoveMotr(miWRK_XCmp , piWRK_XGrip);
                  else                                                                                  MoveMotr(miWRK_XCmp , piWRK_XGripOfs);
                  Step.iCycle++;
                  return false;

        case  15: if(iChipTemp < OM.DevInfo.iRowCnt * OM.DevOptn.iContColCnt){
                      if(!MoveMotr(miWRK_XCmp , piWRK_XGrip   ))  return false ;
                  }
                  else {
                      if(!MoveMotr(miWRK_XCmp , piWRK_XGripOfs))  return false ;
                  }
                  MoveActr(aiWRK_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWork);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miWRK_XCmp , piWRK_XWork)) return false;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  if(!OM.MstOptn.bUseReContact) MoveActr(aiWRK_Cmp  , ccBwd );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  19: if(!OM.MstOptn.bUseReContact && !MoveActr(aiWRK_Cmp  , ccBwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  //if(!OM.MstOptn.bUseReContact) MoveMotr(miWRK_XCmp , piWRK_XGrip); 기존
                  if(!OM.MstOptn.bUseReContact) MoveMotr(miWRK_XCmp , piWRK_XGripOfs);
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  //if(!OM.MstOptn.bUseReContact && !MoveMotr(miWRK_XCmp , piWRK_XGrip)) return false;
                  if(!OM.MstOptn.bUseReContact && !MoveMotr(miWRK_XCmp , piWRK_XGripOfs)) return false;
                  Step.iCycle++;
                  return false;

        //주의 밑에서 사용.
        case  21: OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;
                  if(OM.CmnOptn.bIgnrInsp) {
                      g_bPb1Rslt = true ;
                      g_bPb2Rslt = true ;
                  }
                  else {
                      g_bPb1Rslt = false ;
                      g_bPb2Rslt = false ;
                      if(OM.MstOptn.bByChInsp) PCM.Start(1);
                      else                     PCM.Start( );
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  23: if(m_tmTemp.OnDelay(true , OM.DevOptn.i1ChLedTime * 100 + 1000)) {
                      if(!g_bPb1Rslt) {
                      //if(!g_bPb1Rslt){
                      //EM_SetErr(eiWRK_FailProbeComm1) ;
                          MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPbr1Rslt) return false;

                  if(!g_bPb1Rslt)return false;
                  Trace("11111","OKOKOKOKOK");
                  if(OM.MstOptn.bByChInsp) PCM.Start(2);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  24: if(m_tmTemp.OnDelay(true , OM.DevOptn.i2ChLedTime * 100 + 1000)) {
                      if(!g_bPb2Rslt) {
                      //if(!g_bPb2Rslt) {
                        //EM_SetErr(eiWRK_FailProbeComm2);
                          MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPb2Rslt)return false;

                  if(!g_bPb2Rslt) return false;
                  Trace("222222","OKOKOKOKOK");
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;


        case  25: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  //마스킹 패턴 및 재 검사 패턴.
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //데이타 마스킹.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , g_sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , g_sPb2Msg);

                      if(!r1){ Step.iCycle = 30 ; m_iDataErrCnt++ ; MoveActr(aiWRK_Cmp , ccFwd); return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //데이터 마스킹.  나중에 PCM에 ARAY 걷어 내고 여기서 데이터 받아서 마스킹 한다.
                      if(!r2){ Step.iCycle = 30 ; m_iDataErrCnt++ ; MoveActr(aiWRK_Cmp , ccFwd); return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , g_sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , g_sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
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
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 31 ;
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
                              if(OM.DevOptn.iPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount);
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
                      }
                  }

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  Step.iCycle++;
                  return false;

        case  26: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  27: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  //MoveMotr(miWRK_XCmp , piWRK_XGrip);
                  MoveMotr(miWRK_XCmp , piWRK_XGripOfs);
                  Step.iCycle++;
                  return false;

        case  28: //if(!MoveMotr(miWRK_XCmp , piWRK_XGrip)) return false;
                  if(!MoveMotr(miWRK_XCmp , piWRK_XGripOfs)) return false;
                  if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep() + OM.DevOptn.iContColCnt);
                  m_iLastStep = DM.ARAY[riWRK].GetStep();
                  Step.iCycle=0;
                  return true;


        //PCM.CheckDataCnt 데이터 체크 Fail시 사용 스텝.
        case  30: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //밑에서 사용. 재검사 옵션 및 검사 실패시 사용.
        case  31: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  32: MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  33: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  34: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  35: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  Step.iCycle = 21;
                  return false;

        //검사 실패시 사용 되는 스텝.
        case  50: if(!MoveMotr(miWRK_YBtm, piWRK_YBWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  if(iCmmErrCnt < 4) { //검사결과 가끔 잘 안날라 와서 리트라이 넣음.
                      Step.iCycle = 31 ;
                      if(!g_bPb1Rslt               ) Trace("Cmmunication","g_bPb1Rslt ComRetry");
                      if( g_bPb1Rslt && !g_bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < 5) { //정 안돼면 세팅 다시해봄.

                      Step.iCycle = 31 ;
                      if(!g_bPb1Rslt               ) Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                      if( g_bPb1Rslt && !g_bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");

                      PCM.SetDaegyeom();
                      //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
                      return false ;
                  }
                  
                  //그래도 안되면 에러.
                  if(!g_bPb1Rslt               ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( g_bPb1Rslt && !g_bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2);
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  51: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  Step.iCycle = 0 ;
                  return false ;
    }
}
bool CWorkZone::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

        case  10: LT.AddDayInfoWorkStrp(1);
                  LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                  LT.AddDayInfoFailChip(DM.ARAY[riWRK].GetCntExist() - DM.ARAY[riWRK].GetCntStat(csGood));
                  LT.AddDayInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));

                  LT.AddLotInfoWorkStrp(1);
                  LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                  LT.AddLotInfoFailChip(DM.ARAY[riWRK].GetCntExist() - DM.ARAY[riWRK].GetCntStat(csGood));
                  LT.AddLotInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                  LT.AddLotInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                  bLastStrip = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR2].GetLotNo() && !DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR1].GetLotNo() && !DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone))  ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() && (DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone))) ||
                               (LDR.GetWorkEnd() && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)); //내려놓는 중에 하나 걸어놓음.
                  
                  
                               //기존.
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR1].GetLotNo() && !DM.ARAY[riPR1].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR2].GetLotNo() && !DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() && (DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone))) ||
                               //(LDR.GetWorkEnd() && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)); //내려놓는 중에 하나 걸어놓음.
                  
                  if(bLastStrip) {
                      m_bDispLotEnded = true ;
                      memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));
                  
                      Trace("LastStrip",DM.ARAY[riWRK].GetLotNo().c_str());
                      //LT.LotInfo.dEndTime = Now() ;
                      LT.LotInfo.dEndTime = Now() ;
                      LT.WriteLotLog   ();
                      LT.WriteLotDayLog(); // 데이인포 갱신은 업데이트에서 하고 여기서 쓴다.
                  }

                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  Step.iCycle++;
                  return false;

        //Case 18번에서 씀 바꿀때 주의.
        case  12: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  13: MoveActr(aiWRK_Cmp , ccBwd);
                  //for(int t = 1; t < 43 ; t += 4){
                  //    DM.ARAY[riWRK].SetStat(0, t, csGood);
                  //}//대기 JS
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWait);
                  PCM.ShiftArrayData(riWRK);
                  DM.ShiftArrayData(riWRK,riPSB);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRK_XCmp , piWRK_XWait)) return false;
                  //DM.ARAY[riWRK].SetSubStep(0);
                  Step.iCycle=0;
                  return true;
    }
}

bool CWorkZone::CycleToolWait()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait )) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait )) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait )) return false ;
                  Step.iCycle=0;
                  return true;

    }

}
bool CWorkZone::CycleToolChange()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait  );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBChange);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait  )) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBChange)) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBChange);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_YBtm , piWRK_YBChange)) return false ;
                  Step.iCycle=0;
                  return true;


    }

}

bool CWorkZone::CycleNextMove()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

    double dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) + OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt + OM.DevOptn.dWorkPtchOfs;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp , ccBwd)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_XCmp , piWRK_XGrip );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_XCmp , piWRK_XGrip )) return false ;
                  MoveActr(aiWRK_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false ;
                  MT_GoAbsMan(miWRK_XCmp , dCmpMovePos);
                  //MoveMotr(miWRK_XCmp , piWRK_XWork);
                  Step.iCycle++;
                  return false;

        case  14: //if(!MoveMotr(miWRK_XCmp , piWRK_XWork)) return false ;
                  if(!MT_GoAbsMan(miWRK_XCmp , dCmpMovePos)) return false;
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false ;
                  MoveMotr(miWRK_XCmp , piWRK_XGrip);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRK_XCmp , piWRK_XGrip)) return false;
                  Step.iCycle=0;
                  return true;
    }

}
bool CWorkZone::CycleRptCntct()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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
    //    Step.iCycle = 0;
    //    return true ;
    //}
    int r,c ;
    bool r1,r2 ;
    int iMaxRow,iMaxCol,icsWork,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();

    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;

    static int  iReInspCnt = 0 ;
    static int  iCmmErrCnt = 0 ;
    AnsiString sPinCount ;

    //static bool bRepeatAtStart = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  Step.iCycle++;
                  return false;

        //주의 밑에서 사용.
        case  16: OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  17: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;
                  if(OM.CmnOptn.bIgnrInsp) {
                      g_bPb1Rslt = true ;
                      g_bPb2Rslt = true ;
                  }
                  else {
                      g_bPb1Rslt = false ;
                      g_bPb2Rslt = false ;
                      if(OM.MstOptn.bByChInsp) PCM.Start(1);
                      else                     PCM.Start( );
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(m_tmTemp.OnDelay(true , OM.DevOptn.i1ChLedTime * 100 + 1000)) {
                      if(!g_bPb1Rslt) {
                      //if(!g_bPb1Rslt){
                          //EM_SetErr(eiWRK_FailProbeComm1) ;
                          MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPbr1Rslt) return false;
                  if(!g_bPb1Rslt) return false;
                  if( OM.MstOptn.bByChInsp) PCM.Start(2);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;


        case  19: if(m_tmTemp.OnDelay(true , OM.DevOptn.i2ChLedTime * 100 + 1000)) {
                      if(!g_bPb2Rslt) {
                      //if(!g_bPb2Rslt) {
                          //EM_SetErr(eiWRK_FailProbeComm2);
                          MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }

                  //if(!g_bPb2Rslt)return false;
                  if(!g_bPb2Rslt) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;


        case  20: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  //마스킹 패턴 및 재 검사 패턴.
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //데이타 마스킹.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , g_sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , g_sPb2Msg);

                      if(!r1){ Step.iCycle = 30 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //데이터 마스킹.  나중에 PCM에 ARAY 걷어 내고 여기서 데이터 받아서 마스킹 한다.
                      if(!r2){ Step.iCycle = 30 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , g_sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , g_sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
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
                          Step.iCycle = 31 ;
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
                              if(OM.DevOptn.iPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount);
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
                      }
                  }

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  Step.iCycle++;
                  return false;

        case  21: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  22: if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  //DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep() + OM.DevOptn.iContColCnt);
                  Step.iCycle=0;
                  return true;


        //PCM.CheckDataCnt 데이터 체크 Fail시 사용 스텝.
        case  30: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //밑에서 사용. 재검사 옵션 및 검사 실패시 사용.
        case  31: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  32: MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  33: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  34: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  35: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  Step.iCycle = 16;
                  return false;

        //검사 실패시 사용 되는 스텝.
        case  50: if(!MoveMotr(miWRK_YBtm, piWRK_YBWait)) return false ;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  if(iCmmErrCnt < OM.DevOptn.iReInspCnt) { //검사결과 가끔 잘 안날라 와서 리트라이 넣음.

                      Step.iCycle = 31 ;
                      if(!g_bPb1Rslt               ) Trace("Cmmunication","g_bPb1Rslt ComRetry");
                      if( g_bPb1Rslt && !g_bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < OM.DevOptn.iReInspCnt+1) { //정 안돼면 세팅 다시해봄.

                      Step.iCycle = 31 ;
                      if(!g_bPb1Rslt               ) Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                      if( g_bPb1Rslt && !g_bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");

                      PCM.SetDaegyeom();
                      //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
                      return false ;
                  }

                  //그래도 안되면 에러.
                  if(!g_bPb1Rslt               ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( g_bPb1Rslt && !g_bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2);
                  Step.iCycle = 0 ;
                  return false ;
        }
    }

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
    if(!MT_GetStop(miWRK_XCmp)) return false ;
    if(!MT_GetStop(miWRK_ZBtm)) return false ;
    if(!MT_GetStop(miWRK_ZTop)) return false ;
    if(!MT_GetStop(miWRK_YBtm)) return false ; 

    if(!AT_Done(aiWRK_Cmp    )) return false ;

    return true ;
}

void CWorkZone::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\WorkZone.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iReGripCnt"  , m_iReGripCnt  );
        UserINI.Load(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Load(sPath, "Member" , "m_iLastStep"   , m_iLastStep   );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iReGripCnt"  , m_iReGripCnt  );
        UserINI.Save(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Save(sPath, "Member" , "m_iLastStep"   , m_iLastStep   );
    }
}
//---------------------------------------------------------------------------

