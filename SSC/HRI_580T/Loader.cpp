//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loader.h"

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
CLoader LDR;

CLoader::CLoader(void)
{
    m_sPartName = "Loader " ;
    Reset();
}

void CLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CLoader::~CLoader (void)
{
    Trace("","");
}

bool CLoader::FindChip(int &r , int &c )
{
    r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    return (r > -1 && r > -1);
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dZ1Pos = PM.GetValue(miLDR_ZCmp , pvLDR_ZCmpWorkSttPs) + (OM.DevInfo.iCsSlCnt-r-1) * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miLDR_ZCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piLDR_ZCmpWait      : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpWaitPs     ); break ;
            case piLDR_ZCmpPickFwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPickFwdPs  ); break ;
            case piLDR_ZCmpClampOn   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpClampOnPs  ); break ;
            case piLDR_ZCmpPickBwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPickBwdPs  ); break ;
            case piLDR_ZCmpWorkStt   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpWorkSttPs  ); break ;
            case piLDR_ZCmpPlceFwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPlceFwdPs  ); break ;
            case piLDR_ZCmpClampOff  : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpClampOffPs ); break ;
            case piLDR_ZCmpPlceBwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPlceBwdPs  ); break ;
            case piLDR_ZCmpWork      : dPos = dZ1Pos                                      ; break ;
        }
    }

    else if(_iMotr == miLDR_YCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos  (_iMotr                       ); break ;
            case piLDR_YCmpWait      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpWaitPs    ); break ;
            case piLDR_YCmpPick      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpPickPs    ); break ;
            case piLDR_YCmpWork      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpWorkPs    ); break ;
            case piLDR_YCmpPlce      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpPlcePs    ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isFwPusher        = AT_Complete(aiLDR_Pusher , ccFwd ) ;
    bool isRailGateCnsr    = IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_PkgIn2) ;
    bool isClampYFwd       = MT_CmprPos(miLDR_YCmp , GetMotrPos(miLDR_YCmp , piLDR_YCmpPick)) || MT_CmprPos(miLDR_YCmp , GetMotrPos(miLDR_YCmp , piLDR_YCmpPlce));
    bool isClampZUpstair   = MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPickBwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPickFwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpClampOn ) ;
    bool isClampZCenter    = MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpWait   ) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpWork   ) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpWorkStt ) ;
    bool isClampZDnstair   = MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPlceBwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPlceFwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpClampOff) ;

    if(_iMotr == miLDR_ZCmp){
             if(isFwPusher    ) { sMsg = AT_GetName(aiLDR_Pusher  )+" is not Bwd" ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg =                   "Strip is feeding now" ; bRet = false ;}
        else {
            switch(_iPstnId) {
                default                  :                                                                                                                   break ;
                //UpStair
                case piLDR_ZCmpPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piLDR_ZCmpWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpWorkStt   : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piLDR_ZCmpPlceFwd   : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpPlceBwd   : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;

            }
        }
    }

    else if(_iMotr == miLDR_YCmp){
        if(isFwPusher) return false ;
        else {
            switch(_iPstnId) {
                default                  :                                                                                           break ;
                case piLDR_YCmpWait      :                                                                                           break ;
                case piLDR_YCmpWork      :                                                                                           break ;
                case piLDR_YCmpPick      : if(isClampZCenter) {sMsg = AT_GetName(miLDR_ZCmp)+" is Middle Position" ; bRet = false ;} break ;
                case piLDR_YCmpPlce      : if(isClampZCenter) {sMsg = AT_GetName(miLDR_ZCmp)+" is Middle Position" ; bRet = false ;} break ;
            }
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isZMoving = !MT_GetStop(miLDR_ZCmp);

    if(_iActr == aiLDR_Pusher) {
        if( _bFwd) {
            if(isZMoving) { sMsg = MT_GetName(miLDR_ZCmp)+" is Moving" ; bRet = false ;}
        }
    }
    else if(_iActr == aiLDR_Cmp) {
        bRet = true ;
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLoader::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLDR_HomeTO);
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

        case 10: MT_Reset(miLDR_YCmp);
                 MT_Reset(miLDR_ZCmp);

                 MT_SetServo(miLDR_YCmp,true);
                 MT_SetServo(miLDR_ZCmp,true);


                 AT_MoveCyl(aiLDR_Pusher,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiLDR_Pusher,ccBwd))return false ;
                 MT_SetHomeEnd(miLDR_ZCmp , true) ;
                 MT_ClearPos(miLDR_ZCmp);
                 MT_GoIncMan(miLDR_ZCmp,-10); //12mm
                 Step.iHome++;
                 return false ;

        case 12: Step.iHome++;
                 return false ;

        case 13: if(!MT_GetStop(miLDR_ZCmp))return false;
                 MT_DoHome(miLDR_YCmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miLDR_YCmp))return false ;
                 MT_DoHome(miLDR_ZCmp);
                 Step.iHome++;
                 return false ;
        case 15: if(!MT_GetHomeEnd(miLDR_ZCmp))return false ;
                 MT_GoAbsMan(miLDR_YCmp, GetMotrPos(miLDR_YCmp,piLDR_YCmpWait));
                 MT_GoAbsMan(miLDR_ZCmp, GetMotrPos(miLDR_ZCmp,piLDR_ZCmpWait));
                 Step.iHome++;
                 return false ;
        case 16: if(!MT_GoAbsMan(miLDR_YCmp, GetMotrPos(miLDR_YCmp,piLDR_YCmpWait)))return false ;
                 if(!MT_GoAbsMan(miLDR_ZCmp, GetMotrPos(miLDR_ZCmp,piLDR_ZCmpWait)))return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
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
        if (Stat.bReqStop)return false ;

        bool isCycleSupply = DM.ARAY[riLDR].CheckAllStat(csNone ) && !IO_GetX(xLDR_MgzIn) && !Stat.bChckSplyEmpty ;
        bool isCyclePick   = DM.ARAY[riLDR].CheckAllStat(csNone ) && (IO_GetX(xLDR_MgzIn) ||  OM.CmnOptn.bDryRun );
        bool isCycleWork   = DM.ARAY[riLDR].GetCntStat  (csUnkwn) &&  DM.ARAY[riPRB].CheckAllStat(csNone );
        bool isCyclePlace  = DM.ARAY[riLDR].CheckAllStat(csEmpty) ;
        bool isCycleEnd    = DM.ARAY[riLDR].CheckAllStat(csNone ) && !IO_GetX(xLDR_MgzIn) && Stat.bChckSplyEmpty  ;

        //모르는 스트립에러.
        if(  DM.ARAY[riLDR].CheckAllStat(csNone) && (  IO_GetX(xLDR_CmpMgz1 ) ||  IO_GetX(xLDR_CmpMgz2 ))                       ) EM_SetErr(eiLDR_MgzUnknown) ;
        //카세트 사라짐.
        if( !DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_CmpMgz1 ) && !IO_GetX(xLDR_CmpMgz2 )) && OM.CmnOptn.bDryRun ) EM_SetErr(eiLDR_MgzDispr  ) ; //sun bDryRun 옵션 추가.
        //카세트 OUT 매거진 풀                                                                                                                                  //중복되는 의미 제거.

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default    :                  /*Trace(m_sPartName.c_str(),"default     End");*/Step.iSeq = scIdle ;  return false ;
        case scSupply: if(CycleSupply()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{       
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiLDR_ToStartTO);

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

        case 10: Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiLDR_ToStopTO);

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

        case 10: IO_SetY(yLDR_MgzInAC ,false);
                 IO_SetY(yLDR_MgzOutAC,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLDR_CycleTO);
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

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yLDR_MgzInAC,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: r1 = m_tmTemp.OnDelay(true , 4000) ;
                  r2 = IO_GetX(xLDR_MgzIn) ;
                  if(!r1&&!r2) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmTemp.OnDelay(true ,  200)) return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  if(!IO_GetX(xLDR_MgzIn))Stat.bChckSplyEmpty = true ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CyclePick()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLDR_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_Cmp    , ccFwd );
                  MoveActr(aiLDR_Pusher , ccBwd);
                  MoveMotr(miLDR_YCmp   , piLDR_YCmpWork) ;
                  IO_SetY(yLDR_MgzInAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_Cmp    , ccFwd  )) return false ;
                  if(!MoveActr(aiLDR_Pusher , ccBwd  )) return false ;
                  if(!MoveMotr(miLDR_YCmp   , piLDR_YCmpWork)) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickFwd))return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpPick))return false ;
                  if(!IO_GetX(xLDR_MgzIn) && !OM.CmnOptn.bDryRun)return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOn)) return false ;
                  MoveActr(aiLDR_Cmp , ccBwd);
                  DM.ARAY[riLDR].SetStat(csUnkwn) ;
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_Cmp , ccBwd)) return false ;
                  IO_SetY(yLDR_MgzInAC,true);
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickBwd)) return false ;
                  if(OM.MstOptn.bLdr_YHome) { m_bIsHome = true; MT_DoHome(miLDR_YCmp); }
                  Step.iCycle++;
                  return false ;

        case  17: if(OM.MstOptn.bLdr_YHome && !MT_GetHomeEnd(miLDR_YCmp)) return false ;
                  m_bIsHome = false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CycleWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLDR_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork)) return false ;
                  MoveActr(aiLDR_Pusher,ccFwd); // 다른 파트 참조.
                  Step.iCycle++;
                  return false;

        case  13: if(IO_GetX(xLDR_PushOver )) {
                      EM_SetErr(eiLDR_PshrOverload);
                      MoveActr(aiLDR_Pusher,ccBwd);
                      Step.iCycle = 0;
                      return true ;
                  }
//                  DM.ShiftArrayData(riLDR,riPRB);
                  if(!AT_MoveCyl(aiLDR_Pusher,ccFwd)) return false;

                  DM.ARAY[riLDR].SetStat(DM.ARAY[riLDR].FindLastRow(csUnkwn),0,csEmpty);
                  //DM.ARAY[riPRB].SetStat(csUnkwn) ;

                  AT_MoveCyl(aiLDR_Pusher,ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!AT_MoveCyl(aiLDR_Pusher,ccBwd))return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  Step.iCycle = 0;
                  return true ;
    }

}

bool CLoader::CyclePlace ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLDR_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlceFwd))return false ;
                  if(!IO_GetX(xLDR_MgzOutFull)) {
                      EM_SetErr(eiLDR_MgzFull);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  IO_SetY(yLDR_MgzOutAC,false);
                  MoveMotr(miLDR_YCmp , piLDR_YCmpPlce) ;
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpPlce))return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOff)) return false ;
                  MoveActr(aiLDR_Cmp , ccFwd);
                  DM.ARAY[riLDR].SetStat(csNone) ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_Cmp , ccFwd)) return false ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlceBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlceBwd)) return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  IO_SetY(yLDR_MgzOutAC,false);
                  Step.iCycle = 0;
                  return true ;
    }
}

void CLoader::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLoader::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CLoader::CheckStop()
{
    if(!MT_GetStop(miLDR_YCmp)) return false ;
    if(!MT_GetStop(miLDR_ZCmp)) return false ;

    if(!AT_Done(aiLDR_Cmp   )) return false ;
    if(!AT_Done(aiLDR_Pusher)) return false ;

    return true ;
}

void CLoader::Load(bool IsLoad , FILE *fp)
{

}
//---------------------------------------------------------------------------
