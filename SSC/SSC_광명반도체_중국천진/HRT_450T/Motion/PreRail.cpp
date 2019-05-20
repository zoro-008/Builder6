//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PreRail.h"

#include "UserINI.h"
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
CPreRail PRR;
const AnsiString sPartName = "PreRail " ;

bool bDragWorkEnd ;      //아낭 넌 모닝.....mornig ;;;

CPreRail::CPreRail(void)
{
    bDragWorkEnd = false ;
    Reset();
    Load(toTabl);
}

void CPreRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPreRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPreRail::~CPreRail (void)
{
    Load(toBuff);
}

bool CPreRail::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CPreRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miPRB_XCmp){
        switch(_iPstnId) {
            default          : dPos = MT_GetCmdPos(_iMotr                ); break ;
            case piPRB_XWait : dPos = PM.GetValue (_iMotr , pvPRB_XWaitPs); break ;
            case piPRB_XGrip : dPos = PM.GetValue (_iMotr , pvPRB_XGripPs); break ;
            case piPRB_XPull : dPos = PM.GetValue (_iMotr , pvPRB_XPullPs); break ;
            case piPRB_XBack : dPos = PM.GetValue (_iMotr , pvPRB_XBackPs); break ;
            case piPRB_XOut  : dPos = PM.GetValue (_iMotr , pvPRB_XOutPs ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool isClampClose        = AT_Complete(aiPRB_Cmp , ccFwd ) ;
    if(_iMotr == miPRB_XCmp){
        switch(_iPstnId) {
            default          :                                                     break ;
            case piPRB_XWait : if(isClampClose)                     return false ; break ;
            case piPRB_XGrip :                                                     break ;
            case piPRB_XPull :                                                     break ;
            case piPRB_XBack :                                                     break ;
            case piPRB_XOut  :                                                     break ;
        }
    }

    else {
        return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CPreRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPRB_XCmp);
//    bool is2ZMoving = !MT_GetStopInpos(miLD2_Z);


    if(_iActr == aiPRB_Cmp) {
        if( !_bFwd) {
            if(isXMoving) return false ;
        }
    }

    //Ok.
    return true;
}

bool CPreRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CPreRail::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CPreRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreRail::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_HomeTo);
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

        case 10: AT_MoveCyl(aiPRB_Cmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Cmp,ccBwd))return false ;
                 MT_Home(miPRB_XCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPRB_XCmp))return false;
                 MoveMotr(miPRB_XCmp,piPRB_XWait);
                 Step.iHome++;
                 return false ;

        case 13: if(!MoveMotr(miPRB_XCmp,piPRB_XWait))return false;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPRB_Overload) ) EM_SetErr(xPRB_Overload);

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleIn         =  (IO_GetX(xPRB_1Pkg) || IO_GetX(xPRB_2Pkg));
        bool isCycleDrag       =  DM.ARAY[riPRB].CheckAllStat(csUnkwn) && !bDragWorkEnd ;
        bool isCycleOut        =  DM.ARAY[riPRB].CheckAllStat(csUnkwn) &&  DM.ARAY[riWRK].CheckAllStat(csNone ) && bDragWorkEnd ;
        bool isCycleEnd        =  DM.ARAY[riPRB].CheckAllStat(csNone );

        //Unknown Strip
        if (isCycleIn ) {
            if ( DM.ARAY[riPRB].CheckAllStat(csNone) &&  IO_GetX(xPRB_3Pkg) ) EM_SetErr(eiPRB_Unknown) ;
        }
        //Dissapear Strip
        if (isCycleIn ) {
            if (!DM.ARAY[riPRB].CheckAllStat(csNone) && !IO_GetX(xPRB_3Pkg) ) EM_SetErr(eiPRB_Dispr)   ;
        }

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleIn  ) {Trace(sPartName.c_str(),"CycleIn   Stt"); Step.iSeq = scIn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDrag) {Trace(sPartName.c_str(),"CycleDrag Stt"); Step.iSeq = scDrag ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut ) {Trace(sPartName.c_str(),"CycleOut  Stt"); Step.iSeq = scOut  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default     :                  Trace(sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIn   : if(CycleIn  ()){ Trace(sPartName.c_str(),"CycleIn   End");Step.iSeq = scIdle ;} return false ;
        case scDrag : if(CycleDrag()){ Trace(sPartName.c_str(),"CycleDrag End");Step.iSeq = scIdle ;} return false ;
        case scOut  : if(CycleOut ()){ Trace(sPartName.c_str(),"CycleOut  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CPreRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CPreRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CPreRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiPRB_ToStartTO);

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

bool CPreRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiPRB_ToStopTO);

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

        case 10: IO_SetY(yPRB_FeedingAC,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CPreRail::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: IO_SetY(yPRB_FeedingAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!IO_GetX(xPRB_2Pkg) || !IO_GetX(xPRB_1Pkg)) return false;
                  IO_SetY(yPRB_FeedingAC,false);
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  bDragWorkEnd = false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreRail::CycleDrag()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
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

        case  15: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPRB_XCmp , piPRB_XBack)) return false ;
                  MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPRB_Cmp,ccFwd)) return false ;
                  MT_GoInc(miPRB_XCmp,12,OM.CmnOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MT_Stop(miPRB_XCmp)) return false ;
                  bDragWorkEnd = true;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreRail::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Cmp,ccFwd)) return false ;
                  MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut),OM.CmnOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MT_GoAbs(miPRB_XCmp,GetMotrPos(miPRB_XCmp,piPRB_XOut),OM.CmnOptn.dPreRailOutVel)) return false ;
                  DM.ARAY[riPRB].SetStat(csNone);
                  DM.ARAY[riWRK].SetStat(csUnkwn);
                  MoveMotr(miPRB_XCmp,piPRB_XPull);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miPRB_XCmp,piPRB_XPull))return false ;
                  MoveActr(aiPRB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiPRB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPRB_XCmp , piPRB_XGrip) ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miPRB_XCmp , piPRB_XGrip)) return false ;
                  bDragWorkEnd = false;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPreRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPreRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreRail::CheckStop()
{
    if(!MT_GetStopInpos(miPRB_XCmp)) return false ;
    if(!AT_Done(aiPRB_Cmp         )) return false ;

    return true ;
}

void CPreRail::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "Util\\PreRail.INI";

    if ( IsLoad ) UserINI.Load(sPath, "Stat" , "bDragWorkEnd" , bDragWorkEnd );
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Stat" , "bDragWorkEnd" , bDragWorkEnd );
    }
}
//---------------------------------------------------------------------------
