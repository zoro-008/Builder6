//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PreBuff.h"
#include "Loader.h"

#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "EqpComUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPreBuff PRB;
//bool bDragWorkEnd ;      //아낭 넌 모닝.....mornig ;;;

CPreBuff::CPreBuff(void)
{
    m_sPartName = "PreBuff " ;
    Reset();
    Load(true);
}

void CPreBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPreBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPreBuff::~CPreBuff (void)
{
    Load(false);
}

bool CPreBuff::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;
    double dStepPos = 0.0 ;

    dStepPos = PM.GetValue(miPRB_XCmp,piPRB_XGrip) +
               OM.DevInfo.iColCnt * OM.DevInfo.dColPitch / 2.0 +
               DM.ARAY[riPR1].GetStep() * OM.DevInfo.dColPitch ;


    if(_iMotr == miPRB_XCmp){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                 ); break ;
            case piPRB_XWait    : dPos = PM.GetValue (_iMotr , pvPRB_XWaitPs ); break ;
            case piPRB_XGrip    : dPos = PM.GetValue (_iMotr , pvPRB_XGripPs ); break ;
            case piPRB_XPull    : dPos = PM.GetValue (_iMotr , pvPRB_XPullPs ); break ;
            case piPRB_XBack    : dPos = PM.GetValue (_iMotr , pvPRB_XBackPs ); break ;
            case piPRB_XIdCheck : dPos = PM.GetValue (_iMotr , pvPRB_XIdCheck); break ;
            case piPRB_XOut1    : dPos = PM.GetValue (_iMotr , pvPRB_XOut1Ps ); break ;
            case piPRB_XOut2    : dPos = PM.GetValue (_iMotr , pvPRB_XOut2Ps ); break ;
            case piPRB_XStep1   : dPos = dStepPos;                              break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isClampClose = AT_Complete(aiPRB_Cmp  , ccFwd ) ;
    bool isIdClampDn  = AT_Complete(aiPRB_IdChk, ccBwd ) ;

    if(_iMotr == miPRB_XCmp){
        switch(_iPstnId) {
            default             :                                                                               break ;
            case piPRB_XWait    : if(isClampClose) { sMsg = AT_GetName(aiPRB_Cmp  )+" is Fwd" ; bRet = false ;}
                                  if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XGrip    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XPull    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XBack    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XIdCheck : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XOut1    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
            case piPRB_XOut2    : if(isIdClampDn ) { sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false ;} break ;
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPRB_XCmp);

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiPRB_Cmp) {
        if( !_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miPRB_XCmp)+" is Moving" ; bRet = false ;}
        }
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_HomeTo);
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
                 //Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(yPRB_FeedingAC ,false) ;
                 IO_SetY(yPRB_AirBlower ,false);
                 AT_MoveCyl(aiPRB_Cmp  ,ccBwd);
                 AT_MoveCyl(aiPRB_IdChk,ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Cmp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPRB_IdChk,ccFwd)) return false ;
                 MT_DoHome(miPRB_XCmp);
                 MT_SetHomeEnd(miPRB_XCmp , false);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPRB_XCmp)) return false;
                 //MT_GoAbsMan(miPRB_XCmp, GetMotrPos(miPRB_XCmp,piPRB_XWait));
                 MT_GoAbsMan(miPRB_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: //if(!MT_GoAbsMan(miPRB_XCmp, GetMotrPos(miPRB_XCmp,piPRB_XWait))) return false;
                 if(!MT_GoAbsMan(miPRB_XCmp, m_dLastIndPos)) return false ;

                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPRB_Overload )                                ) EM_SetErr(eiPRB_Overload );
    if ( IO_GetX(xPRB_StripOver) && !OM.CmnOptn.bIgnrStrOverAlm ) EM_SetErr(eiPRB_StripOver);

    if (!DM.ARAY[riPR2].GetCntExist())tmWait.Clear() ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && (!IO_GetX(xPRB_1Pkg) || !IO_GetX(xPRB_2Pkg)))return false ;

        bool isCycleIn       =  DM.ARAY[riPR1].CheckAllStat(csNone) &&
                                DM.ARAY[riPR2].CheckAllStat(csNone) &&
                              ((IO_GetX(xPRB_1Pkg) && IO_GetX(xPRB_2Pkg)) || ( OM.CmnOptn.bDryRun && IO_GetX(xLD1_PusherFw) ));

        //드레그 스텝 사용하면서 바뀐조건.
        bool isCycleDrag     =  DM.ARAY[riPR1].GetCntExist (      ) &&  DM.ARAY[riPR2].CheckAllStat(csNone) && DM.ARAY[riWRP].CheckAllStat(csNone) &&
                                DM.ARAY[riPR1].GetStep() > DM.ARAY[riPR1].GetMaxCol()/2  ;

      //드레그스텝 사용전에 원래 조건.
      //bool isCycleDrag     =  DM.ARAY[riPR1].GetCntExist (      ) &&  DM.ARAY[riPR2].CheckAllStat(csNone) && DM.ARAY[riWRP].CheckAllStat(csNone) &&
      //                     ((!DM.ARAY[riWRK].GetCntColStat(DM.ARAY[riWRK].GetMaxCol()/2,csUnkwn) && DM.ARAY[riWRK].GetCntExist())|| DM.ARAY[riWRK].CheckAllStat(csNone)) ;

      //WorkZone의 스텝과 연동 해서 돌아가게 한다.
        bool isCycleDragStep =  DM.ARAY[riPR1].GetCntExist (      ) &&  DM.ARAY[riPR2].CheckAllStat(csNone) && DM.ARAY[riWRP].CheckAllStat(csNone) &&
                               (DM.ARAY[riWRK].GetMaxCol()-DM.ARAY[riWRK].FindLastCol(csUnkwn)-1) > DM.ARAY[riPR1].GetStep();


        


        bool isCycleOut      =  DM.ARAY[riPR2].GetCntExist (      ) &&
                               (DM.ARAY[riWRP].CheckAllStat(csNone) && DM.ARAY[riWRK].CheckAllStat(csNone )) &&
                                tmWait.OnDelay(true ,OM.DevOptn.iWaitDelay) ;// || (DM.ARAY[riWRK].GetCntExist())) ;//&& !DM.ARAY[riWRK].GetCntColStat(2,csUnkwn)))  ;
        bool isCycleEnd      =  DM.ARAY[riPR1].CheckAllStat(csNone) &&  DM.ARAY[riPR2].CheckAllStat(csNone) ;

        //Unknown Strip
        if ( IO_GetX(xPRB_3Pkg) && !OM.CmnOptn.bDryRun && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone) )
            EM_SetErr(eiPRB_Unknown) ;

        //Dissapear Strip
        if (isCycleOut && !OM.CmnOptn.bDryRun) {
            if (!DM.ARAY[riPR2].CheckAllStat(csNone) && !IO_GetX(xPRB_3Pkg) ) EM_SetErr(eiPRB_Dispr)   ;
        }

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt"); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDrag    ) {Trace(m_sPartName.c_str(),"CycleDrag     Stt"); Step.iSeq = scDrag     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDragStep) {Trace(m_sPartName.c_str(),"CycleDragStep Stt"); Step.iSeq = scDragStep ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt"); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default         :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scDrag     : if(CycleDrag    ()){ Trace(m_sPartName.c_str(),"CycleDrag     End");Step.iSeq = scIdle ;} return false ;
        case scDragStep : if(CycleDragStep()){ Trace(m_sPartName.c_str(),"CycleDragStep End");Step.iSeq = scIdle ;} return false ;
        case scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CPreBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (LD1.GetSeqStep() == CLoader::scWork) return false ; //로더 워킹 하면서 푸쉬 하고 있을때 스탑을 되어 스트립 걸려 있는 것 방지.
    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiPRB_ToStartTO);

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

        case 10: IO_SetY(yETC_BlowerAC,false);
                 Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErr(eiPRB_ToStopTO);

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

        case 10: IO_SetY(yPRB_FeedingAC,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MT_GetAlarm   (miPRB_XCmp) &&
                    !MT_GetNLimSnsr(miPRB_XCmp) &&
                    !MT_GetPLimSnsr(miPRB_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPRB_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPRB_XCmp , piPRB_XWait) ;


                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CPreBuff::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yPRB_AirBlower,false);
        IO_SetY(yPRB_FeedingAC,false);
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

        case  10: IO_SetY(yPRB_AirBlower,true);
                  MoveActr(aiPRB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp , ccBwd)) return false ;
                  IO_SetY(yPRB_FeedingAC,true);
                  MoveMotr(miPRB_XCmp , piPRB_XWait);
                  Step.iCycle++;
                  return false;

        case  12: if( IO_GetX(xPRB_2Pkg) || IO_GetX(xPRB_1Pkg) ) return false;//|| ( !OM.CmnOptn.bDryRun && !IO_GetX(xLD1_PusherBw)) ) return false;
                  if(!MoveMotr(miPRB_XCmp , piPRB_XWait)) return false ;
                  IO_SetY(yPRB_FeedingAC,false);
                  DM.ARAY[riPR1].SetStat(csUnkwn) ;
                  IO_SetY(yPRB_AirBlower,false);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleDrag()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Cmp,ccBwd  );
                  MoveActr(aiPRB_IdChk,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccBwd  )) return false ;
                  if(!MoveActr(aiPRB_IdChk,ccFwd)) return false ;
                  Step.iCycle= 15 ;
                  return false ;

        /*  미안 스텝피딩때문에..
                  MoveMotr(miPRB_XCmp,piPRB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XCmp,piPRB_XGrip))return false ;
                  MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_Cmp,ccFwd))return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPRB_XCmp , piPRB_XPull)) return false ;
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;
        */

        case  15: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPRB_XCmp , piPRB_XBack)) return false ;
                  MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPRB_Cmp,ccFwd)) return false ;
                  MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XIdCheck),OM.DevOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XIdCheck),OM.DevOptn.dPreRailOutVel)) return false ;
                  DM.ShiftArrayData(riPR1,riPR2);
                  if(!OM.CmnOptn.bIgnrIdCehck) MoveActr(aiPRB_IdChk,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!OM.CmnOptn.bIgnrIdCehck && !MoveActr(aiPRB_IdChk,ccBwd)) return false ;
                  if(IO_GetX(xPRB_IdCheck)) {
                      MoveActr(aiPRB_IdChk,ccFwd);
                      EM_SetErr(eiPRB_IdCheck);
                      Step.iCycle = 0;
                      return true;
                  }

                  MoveActr(aiPRB_IdChk,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveActr(aiPRB_IdChk,ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  21: //autorun 으로 뺌 if(!m_tmTemp.OnDelay(true , OM.DevOptn.iWaitDelay)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleDragStep()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

    double dPos ;

    bool bFollowWRK = !DM.ARAY[riWRK].CheckAllStat(csNone) ;

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

        case  10: dPos = GetMotrPos(miPRB_XCmp,piPRB_XGrip) +
                         OM.DevInfo.iColCnt * OM.DevInfo.dColPitch / 2.0 +
                         DM.ARAY[riPR1].GetStep() * OM.DevInfo.dColPitch ;

                  MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccFwd  )) return false ;
                  MoveMotr(miPRB_XCmp,piPRB_XStep1);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XCmp,piPRB_XStep1))return false ;
                  DM.ARAY[riPR1].SetStep(DM.ARAY[riPR1].GetStep() + OM.DevOptn.iContColCnt) ;
                  m_tmTemp.Clear();
                  Step.iCycle ++;
                  return false ;

        case  13: if(!bFollowWRK && !m_tmTemp.OnDelay(true , 500)) return false;
                  Step.iCycle = 0 ;
                  return true ;
    }




}

bool CPreBuff::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccFwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XOut1);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPRB_XCmp , piPRB_XOut1)) return false ;
                  MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut2),OM.DevOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut2),OM.DevOptn.dPreRailOutVel)) return false ;
                  m_sWorkedLotNo = DM.ARAY[riPR2].GetLotNo() ;
                  DM.ShiftArrayData(riPR2,riWRP);
                  MoveMotr(miPRB_XCmp,piPRB_XBack);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPRB_XCmp,piPRB_XBack))return false ;
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XGrip) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPRB_XCmp , piPRB_XGrip)) return false ;
                  //DM.ARAY[riPR1].SetStat(csNone);
                  DM.ARAY[riPR2].SetStat(csNone);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPreBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPreBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreBuff::CheckStop()
{
    if(!MT_GetStop(miPRB_XCmp)) return false ;
    if(!AT_Done(aiPRB_Cmp    )) return false ;
    if(!AT_Done(aiPRB_IdChk  )) return false ;

    return true ;
}

void CPreBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuff.INI";

    if ( IsLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    }

}
//---------------------------------------------------------------------------
