//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PostRail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPostRail PSR;
const AnsiString sPartName = "PostRail " ;

CPostRail::CPostRail(void)
{
    Reset();
}

void CPostRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPostRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPostRail::~CPostRail (void)
{

}

bool CPostRail::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
//    return (r > -1 && r > -1);
    return false;
}

double CPostRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dZ1Pos = PM.GetValue(miLD1_ZCmp , pvLD1_ZWorkStartPs) + (OM.DevInfo.iCsSlCnt-r-1) * OM.DevInfo.dCsSlPitch ;
//    double dZ2Pos = PM.GetValue(miLD2_Z , pvLD2_ZWorkStartPs) + (                    r  ) * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miLD1_ZCmp){
        switch(_iPstnId) {
            default          : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piPSB_XWait : dPos = PM.GetValue (_iMotr , pvPSB_XWaitPs  ); break ;
            case piPSB_XGrip  : dPos = PM.GetValue (_iMotr , pvPSB_XGripPs  ); break ;
            case piPSB_XPull : dPos = PM.GetValue (_iMotr , pvPSB_XPullPs  ); break ;
            case piPSB_XBack : dPos = PM.GetValue (_iMotr , pvPSB_XBackPs  ); break ;
            case piPSB_XOut  : dPos = PM.GetValue (_iMotr , pvPSB_XOutPs   ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPostRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    if(_iMotr == miPSB_XCmp){
        switch(_iPstnId) {
            default          :                                                     break ;
            case piPSB_XWait :                                                     break ;
            case piPSB_XGrip  :                                                    break ;
            case piPSB_XPull :                                                     break ;
            case piPSB_XBack :                                                     break ;
            case piPSB_XOut  :                                                     break ;
        }
    }

    else {
        return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CPostRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPSB_XCmp);

    if(_iActr == aiPSB_Cmp) {
        if(!_bFwd) {
            if(isXMoving) return false ;
        }
    }

    //Ok.
    return true;
}

bool CPostRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CPostRail::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CPostRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPostRail::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPSB_HomeTo);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    if(Stat.bReqStop) {
        Step.iHome = 0;
        return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 Trace(sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPSB_Cmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Cmp,ccBwd))return false ;
                 MT_Home(miPSB_XCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XCmp))return false;
                 MoveMotr(miPSB_XCmp,piPSB_XWait);
                 Step.iHome++;
                 return false ;

        case 13: if(!MoveMotr(miPSB_XCmp,piPSB_XWait))return false;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPostRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPSB_Overload) ) EM_SetErr(xPSB_Overload);

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //모르는 스트립에러.
//        if(  DM.ARAY[riLD1].CheckAllStat(csNone) && (  IO_GetX(xLD1_MgzDetect1) ||  IO_GetX(xLD1_MgzDetect2) )     ) EM_SetErr(eiLD1_ClampUnknown) ;
        //카세트 사라짐.
//        if( !DM.ARAY[riLD1].CheckAllStat(csNone) && ( !IO_GetX(xLD1_MgzDetect1) && !IO_GetX(xLD1_MgzDetect2) )     ) EM_SetErr(eiLD1_ClampDispr)   ;

        if(EM_IsErr()) return false ;

        bool isCycleWork   = !DM.ARAY[riPSB].CheckAllStat(csNone ) && ( IO_GetX(xPSB_1Pkg) || IO_GetX(xPSB_2Pkg) );
        bool isCycleEnd    =  DM.ARAY[riPSB].CheckAllStat(csNone ) ;

         //Normal Decide Step.
             if (isCycleWork  ) {Trace(sPartName.c_str(),"CycleWork Stt"); Step.iSeq = scWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default    :                      Trace(sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scWork  : if(CycleWork  ()){ Trace(sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CPostRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;
}

bool CPostRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CPostRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiLD1_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPostRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiLD1_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CPostRail::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        Step.iCycle = 0;
        return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp,piPSB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XCmp,piPRB_XGrip))return false ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp,ccFwd))return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPSB_XCmp , piPSB_XBack)) return false ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;
                  MT_GoAbs(miPSB_XCmp,GetMotrPos(miPSB_XCmp,piPSB_XOut),OM.CmnOptn.dPostRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MT_GoAbs(miPSB_XCmp,GetMotrPos(miPSB_XCmp,piPSB_XOut),OM.CmnOptn.dPostRailOutVel)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp,piPSB_XGrip);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miPSB_XCmp,piPSB_XGrip)) return false ;
                  DM.ARAY[riPSB].SetStat(csNone);
                  //다음 장비에 몬가 ㄱㄱ씽. dm.araafd[rt ad].shift ?
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPostRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.
}

bool CPostRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPostRail::CheckStop()
{
    if(!MT_GetStopInpos(miPSB_XCmp)) return false ;
    if(!AT_Done(aiPSB_Cmp         )) return false ;

    return true ;
}

void CPostRail::Load(bool IsLoad , FILE *fp)
{

}
//---------------------------------------------------------------------------
