//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Epoxy.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
//Rs232 Dispensor
//---------------------------------------------------------------------------
#include "Rs232Man.h"
//---------------------------------------------------------------------------
#include "RailEpx.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CEpoxy EPX;

CEpoxy::CEpoxy()
{
    Init();

}
CEpoxy::~CEpoxy (void)
{
    Close();
}
void CEpoxy::Init()
{
    m_sPartName   = "Epoxy " ;
    m_dLastIdxPos = 0.0 ;

    m_iHomeTO  = (int)eiEPX_HomeTO    ;
    m_iStartTO = (int)eiEPX_ToStartTO ;
    m_iStopTO  = (int)eiEPX_ToStopTO  ;
    m_iCycleTO = (int)eiEPX_CycleTO   ;

    m_iEpxCnt = 0 ;
    m_iEpxLotCnt = 0 ;
    m_iSlotCnt   = 0 ;
    m_iMgzCnt    = 0 ;

    m_bToStarted = false;

    Reset();
    Load(true);
}
void CEpoxy::Close()
{
    Load(false);
}
void CEpoxy::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CEpoxy::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CEpoxy::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
//축에서 가까운쪽이 스타트 포지션으로 하기로 햇음.
/*
    int r1,c1;
    if(_riWhre == riWRE) {
        c = DM.ARAY[riWRE].FindLastCol(csUnkwn);
        if(c % 2)DM.ARAY[riWRE].FindLastColRow    (csUnkwn , r1 , c1) ;
        else     DM.ARAY[riWRE].FindLastColFrstRow(csUnkwn , r1 , c1) ;
        r = r1 ;
        c = c1 ;
        return (r > -1 && c > -1);
    }
    return false ;
*/

    int r1,c1;
    if(_riWhre == riRAE) {
        c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
        if(c % 2)DM.ARAY[riRAE].FindLastColRow    (csUnkwn , r , c) ;
        else     DM.ARAY[riRAE].FindLastColFrstRow(csUnkwn , r , c) ;

        return (r > -1 && c > -1);
    }
    return false ;

}

double CEpoxy::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr) ;
    int r  , c  ;
    if(_iMotr == miWRE_YEpx){
        if( FindChip(riRAE,r,c)) {
//            r = OM.DevInfo.iRowCnt - r - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvWRE_YEpxWorkStt ) + ( OM.DevInfo.dRowPitch * r ) ;
        }
    }

    return dWorkPos ;

}

double CEpoxy::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;
    int r = -1 , c = -1 ;

    if(_iMotr == miWRE_XEpx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRE_XEpxWait    : dPos = PM.GetValue (_iMotr , pvWRE_XEpxWait    ); break ;
            case piWRE_XEpxWorkStt : dPos = PM.GetValue (_iMotr , pvWRE_XEpxWorkStt ); break ;
            case piWRE_XEpxOri     : dPos = PM.GetValue (_iMotr , pvWRE_XEpxOri     ); break ; //밑에꺼랑 같아 졋다고 햇는데 확신이 없어서 아직 놔두고 다음에 pos 에 같이 넣어라.
            case piWRE_XEpxClean   : dPos = PM.GetValue (_iMotr , pvWRE_XEpxClean   ); break ;
//            case piWRE_XEpxWork    : FindChip(riWRE,r,c);
//                                     r = OM.DevInfo.iRowCnt - r - 1 ;
//                                     dPos = PM.GetValue (_iMotr , pvWRE_XEpxWorkStt ) + ( OM.DevInfo.dRowPitch * r ) ;
//                                     break ;
        }
    }
    else if(_iMotr == miWRE_YEpx){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRE_YEpxWait     : dPos = PM.GetValue (_iMotr , pvWRE_YEpxWait      ); break ;
            case piWRE_YEpxWorkStt  : dPos = PM.GetValue (_iMotr , pvWRE_YEpxWorkStt   ); break ;
            case piWRE_YEpxOri      : dPos = PM.GetValue (_iMotr , pvWRE_YEpxOri       ); break ;
            case piWRE_YEpxClean    : dPos = PM.GetValue (_iMotr , pvWRE_YEpxClean     ); break ;
            case piWRE_YEpxWork     : dPos = GetWorkPos  (_iMotr                       ); break ;
        }
    }
    else if(_iMotr == miWRE_ZEpx){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRE_ZEpxWait     : dPos = PM.GetValue (_iMotr , pvWRE_ZEpxWait        ); break ;
//            case piWRE_ZEpxOriStt   : dPos = PM.GetValue (_iMotr , pvWRE_ZEpxOriStt    ); break ;
            case piWRE_ZEpxWork     : dPos = PM.GetValue (_iMotr , pvWRE_ZEpxWork        ); break ;
            case piWRE_ZEpxWorkAt   : dPos = PM.GetValue (_iMotr , piWRE_ZEpxWorkAt      ); break ;
            case piWRE_ZEpxCleanWait: dPos = PM.GetValue (_iMotr , pvWRE_ZEpxCleanWait   ); break ;
            case piWRE_ZEpxClean    : dPos = PM.GetValue (_iMotr , pvWRE_ZEpxClean       ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CEpoxy::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isEpxZWaitPs    = MT_GetStopInpos(miWRE_ZEpx) && MT_GetCmdPos(miWRE_ZEpx) <= PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWait) ; //MT_CmprPos(miWRE_ZEpx,PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWait)) ;
    bool isEpxZCleanAble = MT_GetStopInpos(miWRE_ZEpx) && MT_GetCmdPos(miWRE_ZEpx) <= PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxCleanWait ) ; //MT_CmprPos(miWRE_ZEpx,PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWait)) ;

    if(_iMotr == miWRE_XEpx){
        if(!isEpxZWaitPs) { sMsg = "Epoxy_Z Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRE_XEpxWait    : break ;
            case piWRE_XEpxWorkStt : break ;
            case piWRE_XEpxOri     : break ;
            case piWRE_XEpxClean   : if(!isEpxZCleanAble) { sMsg = "Epoxy_Z Axis is Not Clean Move Able Position" ; bRet = false ;} break ;
        }
    }
    else if(_iMotr == miWRE_YEpx){
        if(!isEpxZWaitPs) { sMsg = "Epoxy_Z Axis is Not Wait Position" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRE_YEpxWait    : break ;
            case piWRE_YEpxWorkStt : break ;
            case piWRE_YEpxOri     : break ;
            case piWRE_YEpxClean   : if(!isEpxZCleanAble) { sMsg = "Epoxy_Z Axis is Not Clean Move Able Position" ; bRet = false ;} break ;
            case piWRE_YEpxWork    : break ;
        }
    }
    else if(_iMotr == miWRE_ZEpx){
        if(!MT_GetStopInpos(miWRE_XIdx)) { sMsg = "WRE_X_Index Axis is Moving" ; bRet = false ;}
        switch(_iPstnId) {
            default                  : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRE_ZEpxWait      : break ;
//            case piWRE_ZEpxOriStt    : break ;
            case piWRE_ZEpxWork      : break ;
            case piWRE_ZEpxWorkAt    : break ;
            case piWRE_ZEpxCleanWait : break ;
            case piWRE_ZEpxClean     : break ;
        }
    }

    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CEpoxy::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CEpoxy::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CEpoxy::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CEpoxy::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmHome.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miWRE_XEpx,false);
                 MT_SetHomeEnd(miWRE_YEpx,false);
                 MT_SetHomeEnd(miWRE_ZEpx,false);
                 MT_DoHome(miWRE_ZEpx);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRE_ZEpx)) return false ;
                 MT_DoHome(miWRE_XEpx);
                 MT_DoHome(miWRE_YEpx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRE_XEpx))return false ;
                 if(!MT_GetHomeEnd(miWRE_YEpx))return false ;
                 MT_GoAbsMan(miWRE_XEpx , PM.GetValue(miWRE_XEpx , pvWRE_XEpxWait  ));
                 MT_GoAbsMan(miWRE_YEpx , PM.GetValue(miWRE_YEpx , pvWRE_YEpxWait  ));

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWRE_XEpx , PM.GetValue(miWRE_XEpx , pvWRE_XEpxWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRE_YEpx , PM.GetValue(miWRE_YEpx , pvWRE_YEpxWait  ))) return false ;
                 MT_GoAbsMan(miWRE_ZEpx , PM.GetValue(miWRE_ZEpx , pvWRE_ZEpxWait  ));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miWRE_ZEpx , PM.GetValue(miWRE_ZEpx , pvWRE_ZEpxWait  ))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CEpoxy::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CEpoxy::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CEpoxy::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: //if(DM.ARAY[riRAE].CheckAllStat(csUnkwn)){
                 //    DM.ARAY[riRAE].SetStep(0);
                 //}
//                 FindChip(riRAE,r,c);
//                   if(DM.ARAY[riWRE].GetStat(r,c) == csRslt0 && DM.ARAY[riRAE].GetStat(r,c) && bCheckIdxPos)

//                 MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait);
                 MT_GoAbsRun(miWRE_ZEpx,0.0);

                 IO_SetY(yWRE_CleanSuction,false);
                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait)) return false ;
                 if(!MT_GoAbsRun(miWRE_ZEpx,0.0)) return false ;

                 m_bToStarted = true ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CEpoxy::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: //MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait);
                 MT_GoAbsRun(miWRE_ZEpx,0.0);
                 IO_SetY(yWRE_CleanSuction,false);




                 Step.iToStop ++;
                 return false ;

        case 11: //if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait)) return false ;
                 if(!MT_GoAbsRun(miWRE_ZEpx,0.0)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CEpoxy::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool bCheckIdxPos =  MT_CmprPos(miWRE_XIdx,RAE.GetWorkPos(miWRE_XIdx)) && !RAE.CheckColAllFail() ; // RAE.CheckColAllFail() 안전빵...

        //bool isCycleClean = !OM.MstOptn.bIgnrEpoxy && OM.DevOptn.iEpxClenCnt && OM.DevOptn.iEpxClenCnt <= m_iEpxCnt ;
        bool isCycleClean = !OM.MstOptn.bIgnrEpoxy &&
                           ((OM.DevOptn.iEpxClenCnt && OM.DevOptn.iEpxClenCnt <= m_iEpxCnt) || m_bToStarted );
        bool isCycleOri   =  false ; //Manual.
        bool isCycleWork  = !OM.MstOptn.bIgnrEpoxy && !RAE.bRaeSkip && !OM.CmnOptn.bEpoxyStop &&
                            !DM.ARAY[riRAE].CheckAllStat(csNone) /*&& DM.ARAY[riWRE].GetCntStat(csUnkwn)*/ && bCheckIdxPos &&
                             RAE.GetSeqStep() != CRailEpx::scWork && DM.ARAY[riRAE].GetStep() > 0 &&
                            (OM.CmnOptn.iLotEndChipCnt > m_iEpxLotCnt || !OM.CmnOptn.iLotEndChipCnt ) ;

        bool isCycleWorkOut =!OM.MstOptn.bIgnrEpoxy &&
                             (OM.CmnOptn.iLotEndChipCnt == m_iEpxLotCnt && /*!*/OM.CmnOptn.iLotEndChipCnt) &&
                             !DM.ARAY[riRAE].CheckAllStat(csNone) /*&& DM.ARAY[riWRE].GetCntStat(csUnkwn)*/ && bCheckIdxPos &&
                              RAE.GetSeqStep() != CRailEpx::scWork && DM.ARAY[riRAE].GetStep() > 0 &&
                              DM.ARAY[riRAD].CheckAllStat(csNone) &&
                              DM.ARAY[riWRE].GetCntStat(csWork) ;


        bool isConEnd     =  DM.ARAY[riWRE].CheckAllStat(csNone) || (OM.CmnOptn.iLotEndChipCnt && m_iEpxLotCnt >= OM.CmnOptn.iLotEndChipCnt) ;


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleClean  ) {Trace(m_sPartName.c_str(),"CycleClean   Stt"); Step.iSeq = scClean  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOri    ) {Trace(m_sPartName.c_str(),"CycleOri     Stt"); Step.iSeq = scOri    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork   ) {Trace(m_sPartName.c_str(),"CycleWork    Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkOut) {Trace(m_sPartName.c_str(),"CycleWorkOut Stt"); Step.iSeq = scWorkOut; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default       :                    Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                                      return false ;
        case scClean  : if(CycleClean  ()){ Trace(m_sPartName.c_str(),"CycleClean   End");Step.iSeq = scIdle ;} return false ;
        case scOri    : if(CycleOri    ()){ Trace(m_sPartName.c_str(),"CycleOri     End");Step.iSeq = scIdle ;} return false ;
        case scWork   : if(CycleWork   ()){ Trace(m_sPartName.c_str(),"CycleWork    End");Step.iSeq = scIdle ;} return false ;
        case scWorkOut: if(CycleWorkOut()){ Trace(m_sPartName.c_str(),"CycleWorkOut End");Step.iSeq = scIdle ;} return false ;
    }

}
bool CEpoxy::CycleWorkOut (void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int      r1,r2 ;
    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if( FindChip(riRAE,r,c)) {
                      DM.ARAY[riWRE].SetStat(r,c,csRslt0);
                      DM.ARAY[riRAE].SetStat(r,c,csWork);
                  }
//                  m_iEpxLotCnt++;
                  Step.iCycle = 0;
                  return true ;
    }

}

//One Cycle.
bool CEpoxy::CycleClean(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int      r1,r2 ;

    static int iEpoxyCnt = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait );
                  MT_GoAbsRun(miWRE_ZEpx,0.0);

                  iEpoxyCnt = 0 ;
                  m_bToStarted = false ;

                  Step.iCycle++;
                  return false;

        case  11: //if(!MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait )) return false ;
                  if(!MT_GoAbsRun(miWRE_ZEpx,0.0)) return false ;
                  MoveMotr(miWRE_XEpx , piWRE_XEpxClean) ; //이줄 이상함.
                  MoveMotr(miWRE_YEpx , piWRE_YEpxClean) ;
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRE_XEpx,piWRE_XEpxClean)) return false ;
                  if(!MoveMotr(miWRE_YEpx,piWRE_YEpxClean)) return false ;
                  MoveMotr(miWRE_ZEpx , piWRE_ZEpxClean) ;
                  Step.iCycle++;
                  return false ;

        //밑에서 씀 스텝번호 주의.
        case  13: if(!MoveMotr(miWRE_ZEpx , piWRE_ZEpxClean)) return false ;
                  IO_SetY(yWRE_CleanSuction,true);

                  RSM.Shot();
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(m_tmTemp.OnDelay(true ,OM.DevOptn.iEpxTime + 1000)) {
                      EM_SetErr(eiEPX_Rs232ComFail);
                      MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait);
                      Step.iCycle = 0;
                      return true;
                  }
                  //사알짝 들어야 할수도 있어요. 알아
                  if(!RSM.CheckShot()) return false ;

                  iEpoxyCnt++;

                  if(iEpoxyCnt < OM.DevOptn.iEpxCleanShotCnt ) {
                      Step.iCycle = 13 ;
                      return false ;

                  }




                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  15: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iEpxCleanDly)) return false ;
                  IO_SetY(yWRE_CleanSuction,false);
                  //MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait );
                  MT_GoAbsRun(miWRE_ZEpx,0.0);
                  Step.iCycle++;
                  return false;

        case  16: //if(!MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait )) return false ;
                  if(!MT_GoAbsRun(miWRE_ZEpx,0.0)) return false ;

                  MoveMotr(miWRE_YEpx , piWRE_YEpxWait);
                  MoveMotr(miWRE_XEpx , piWRE_XEpxWait);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miWRE_YEpx , piWRE_YEpxWait)) return false ;
                  if(!MoveMotr(miWRE_XEpx , piWRE_XEpxWait)) return false ;



                  m_iEpxCnt   = 0 ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CEpoxy::CycleOri(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int r,c ;
//    static bool bVsGood = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait )) return false ;
                  MoveMotr(miWRE_XEpx , piWRE_XEpxOri) ; //이줄 이상함.
                  MoveMotr(miWRE_YEpx , piWRE_YEpxOri) ;
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRE_XEpx,piWRE_XEpxOri)) return false ;
                  if(!MoveMotr(miWRE_YEpx,piWRE_YEpxOri)) return false ;
//                  MoveMotr(miWRE_ZEpx , piWRE_ZEpxOriStt) ;
                  Step.iCycle++;
                  return false;

        case  13: //if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxOriStt)) return false ;

                  MT_GoAbs(miWRE_ZEpx,MT_GetMaxPos(miWRE_ZEpx)-0.001,OM.MstOptn.dEpxOriSpd) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(IO_GetX(xWRE_TouchSsr)) MT_EmgStop(miWRE_ZEpx) ;
                  else if(!MT_GoAbs(miWRE_ZEpx,MT_GetMaxPos(miWRE_ZEpx)-0.001,OM.MstOptn.dEpxOriSpd)) return false ;
                  if(!IO_GetX(xWRE_TouchSsr)) {
                      EM_SetErr(eiWRE_OriSsrDttFail);
                      MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait );
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  PM.SetValue(miWRE_ZEpx,pvWRE_ZEpxWork,MT_GetCmdPos(miWRE_ZEpx) + OM.MstOptn.dEpxOriOfs );
                  PM.Save(OM.m_sCrntDev);
                  MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait );
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miWRE_ZEpx , piWRE_ZEpxWait )) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CEpoxy::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int r,c ;
    int iTemp ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10:
                  //MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait);
                  Step.iCycle++;
                  return false;

        case  11: //if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait)) return false ;
                  MoveMotr(miWRE_XEpx,piWRE_XEpxWorkStt);
                  MoveMotr(miWRE_YEpx,piWRE_YEpxWork   );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRE_XEpx,piWRE_XEpxWorkStt)) return false ;
                  if(!MoveMotr(miWRE_YEpx,piWRE_YEpxWork   )) return false ;
                  MoveMotr(miWRE_ZEpx,piWRE_ZEpxWork);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxWork)) return false ;
//                  IO_SetY(yEPX_Eject,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iEpxEjtOnTime)) return false ;
                  RSM.Shot();
                  m_iEpxCnt ++ ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(m_tmTemp.OnDelay(!RSM.CheckShot() ,OM.DevOptn.iEpxTime + 500)) {
                      EM_SetErr(eiEPX_Rs232ComFail);
                      MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait);
                      Step.iCycle = 0;
                      return true;
                  }
                  //사알짝 들어야 할수도 있어요. 알아
                  if(!RSM.CheckShot()) return false ;

                  MT_GoAbsSlow(miWRE_ZEpx,PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWorkAt));
//                  MoveMotr(miWRE_ZEpx,piWRE_ZEpxWorkAt);
                  Step.iCycle++;
                  return false;

        case  16: //if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxWorkAt)) return false ;
                  if(!MT_GoAbsSlow(miWRE_ZEpx,PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWorkAt))) return false ;


//                  IO_SetY(yEPX_Eject,false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  17: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iEpxEjtOffTime)) return false ;
                  MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait);
/*
                  FindChip(riWRE,r,c);
                  if(DM.ARAY[riWRE].GetStat(r,c) == csRslt0) {
//                      DM.ARAY[riWRE].SetStat(r,c,csWork);
                      DM.ARAY[riRAE].SetStat(r,c,csWork);
                  }
                  else {
                      DM.ARAY[riWRE].SetStat(r,c,csWork);
                      DM.ARAY[riRAE].SetStat(r,c,csWork);
                  }
*///나는 지금 매우 귀찮다.
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miWRE_ZEpx,piWRE_ZEpxWait)) return false ;
                  FindChip(riRAE,r,c);

                  DM.ARAY[riWRE].SetStat(r,c,csWork);
                  DM.ARAY[riRAE].SetStat(r,c,csWork);


                  AddEpxLotCnt() ;

                  Load(false);

                  Step.iCycle = 0 ;
                  return true ;
    }
}
void CEpoxy::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CEpoxy::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CEpoxy::CheckStop()
{
    if(!MT_GetStop(miWRE_XEpx)) return false ;
    if(!MT_GetStop(miWRE_YEpx)) return false ;
    if(!MT_GetStop(miWRE_ZEpx)) return false ;

    return true ;
}

void CEpoxy::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iEpxCnt    " , m_iEpxCnt    );
        UserINI.Load(sPath, "Member" , "m_iEpxLotCnt " , m_iEpxLotCnt );
        UserINI.Load(sPath, "Member" , "m_iSlotCnt" , m_iSlotCnt );
        UserINI.Load(sPath, "Member" , "m_iMgzCnt " , m_iMgzCnt  );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iEpxCnt    " , m_iEpxCnt    );
        UserINI.Save(sPath, "Member" , "m_iEpxLotCnt " , m_iEpxLotCnt );
        UserINI.Save(sPath, "Member" , "m_iSlotCnt" , m_iSlotCnt );
        UserINI.Save(sPath, "Member" , "m_iMgzCnt " , m_iMgzCnt  );
    }
}
//---------------------------------------------------------------------------
