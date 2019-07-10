//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoader.h"

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
CUnLoader ULD;
const AnsiString sPartName = "Loader " ;

CUnLoader::CUnLoader(void)
{
    Reset();
}

void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CUnLoader::~CUnLoader (void)
{

}

bool CUnLoader::FindChip(int &r , int &c )
{
    r = DM.ARAY[riULD].FindFrstRow(csUnkwn) ;
    return (r > -1 && r > -1);
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dZ1Pos = PM.GetValue(miULD_ZCmp , pvULD_ZCmpWorkSttPs) - r * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miULD_ZCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piULD_ZCmpWait      : dPos = PM.GetValue (_iMotr , pvULD_ZCmpWaitPs     ); break ;
            case piULD_ZCmpPickFwd   : dPos = PM.GetValue (_iMotr , pvULD_ZCmpPickFwdPs  ); break ;
            case piULD_ZCmpClampOn   : dPos = PM.GetValue (_iMotr , pvULD_ZCmpClampOnPs  ); break ;
            case piULD_ZCmpPickBwd   : dPos = PM.GetValue (_iMotr , pvULD_ZCmpPickBwdPs  ); break ;
            case piULD_ZCmpWorkStt   : dPos = PM.GetValue (_iMotr , pvULD_ZCmpWorkSttPs  ); break ;
            case piULD_ZCmpPlceFwd   : dPos = PM.GetValue (_iMotr , pvULD_ZCmpPlceFwdPs  ); break ;
            case piULD_ZCmpClampOff  : dPos = PM.GetValue (_iMotr , pvULD_ZCmpClampOffPs ); break ;
            case piULD_ZCmpPlceBwd   : dPos = PM.GetValue (_iMotr , pvULD_ZCmpPlceBwdPs  ); break ;
            case piULD_ZCmpWork      : dPos = dZ1Pos                                      ; break ;
        }
    }

    else if(_iMotr == miULD_YCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos  (_iMotr                       ); break ;
            case piULD_YCmpPick      : dPos = PM.GetValue   (_iMotr , pvULD_YCmpWaitPs    ); break ;
            case piULD_YCmpWork      : dPos = PM.GetValue   (_iMotr , pvULD_YCmpWorkPs    ); break ;
            case piULD_YCmpPlce      : dPos = PM.GetValue   (_iMotr , pvULD_YCmpPickPs    ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isRailGateCnsr    = IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2) ;
    bool isClampYFwd       = MT_CmprPos(miULD_YCmp , GetMotrPos(miULD_YCmp , piULD_YCmpPick)) || MT_CmprPos(miULD_YCmp , GetMotrPos(miULD_YCmp , piULD_YCmpPlce));
    bool isClampZDnstair   = MT_CmprPos(miULD_ZCmp , piULD_ZCmpPickBwd) || MT_CmprPos(miULD_ZCmp , piULD_ZCmpPickFwd) || MT_CmprPos(miULD_ZCmp , piULD_ZCmpClampOn ) ;
    bool isClampZCenter    = MT_CmprPos(miULD_ZCmp , piULD_ZCmpWait   ) || MT_CmprPos(miULD_ZCmp , piULD_ZCmpWork   ) || MT_CmprPos(miULD_ZCmp , piULD_ZCmpWorkStt ) ;
    bool isClampZUpstair   = MT_CmprPos(miULD_ZCmp , piULD_ZCmpPlceBwd) || MT_CmprPos(miULD_ZCmp , piULD_ZCmpPlceFwd) || MT_CmprPos(miULD_ZCmp , piULD_ZCmpClampOff) ;

    if(_iMotr == miULD_ZCmp){
        if(isRailGateCnsr) { sMsg =                   "Strip is feeding now" ; bRet = false ;}
        else {
            switch(_iPstnId) {
                default                  :                                                                                                                   break ;
                //UpStair
                case piULD_ZCmpPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piULD_ZCmpWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpWorkStt   : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piULD_ZCmpPlceFwd   : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZCmpPlceBwd   : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miULD_YCmp)+" is Fwd" ; bRet = false ;} break ;

            }
        }
    }

    else if(_iMotr == miULD_YCmp){
            switch(_iPstnId) {
                default                  :                                                                                           break ;
                case piULD_YCmpWait      :                                                                                           break ;
                case piULD_YCmpWork      :                                                                                           break ;
                case piULD_YCmpPick      : if(isClampZCenter) {sMsg = AT_GetName(miULD_ZCmp)+" is Middle Position" ; bRet = false ;} break ;
                case piULD_YCmpPlce      : if(isClampZCenter) {sMsg = AT_GetName(miULD_ZCmp)+" is Middle Position" ; bRet = false ;} break ;
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
bool  CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiULD_Cmp) {
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

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CUnLoader::CycleHome()    
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_HomeTO);
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

        case 10:
                 Step.iHome++;
                 return false ;

        case 11:
                 MT_GoIncMan(miULD_ZCmp,12); //12mm
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_Stop(miULD_ZCmp))return false;
                 MT_Home(miULD_YCmp);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miULD_YCmp))return false ;
                 MT_Home(miULD_ZCmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miULD_ZCmp))return false ;
                 MT_GoAbsMan(miULD_YCmp, GetMotrPos(miULD_YCmp,piULD_YCmpWait));
                 MT_GoAbsMan(miULD_ZCmp, GetMotrPos(miULD_ZCmp,piULD_ZCmpWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miULD_YCmp, GetMotrPos(miULD_YCmp,piULD_YCmpWait)))return false ;
                 if(!MT_GoAbsMan(miULD_ZCmp, GetMotrPos(miULD_ZCmp,piULD_ZCmpWait)))return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CUnLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isZWorkPos    = MT_CmprPos(miULD_ZCmp ,GetMotrPos(miULD_ZCmp , piULD_ZCmpWork)) ;

        bool isCycleSupply = DM.ARAY[riULD].CheckAllStat(csNone ) && !IO_GetX(xULD_MgzIn) && !Stat.bChckSplyEmpty ;
        bool isCyclePick   = DM.ARAY[riULD].CheckAllStat(csNone ) && (IO_GetX(xULD_MgzIn) ||  OM.CmnOptn.bDryRun );
        bool isCycleWork   = DM.ARAY[riULD].GetCntStat  (csEmpty) && !isZWorkPos          ;
        bool isCyclePlace  = DM.ARAY[riULD].CheckAllStat(csWork ) ;
        bool isCycleEnd    = DM.ARAY[riULD].CheckAllStat(csNone ) && !IO_GetX(xULD_MgzIn) && Stat.bChckSplyEmpty  ;

        //모르는 스트립에러.
        if(  DM.ARAY[riULD].CheckAllStat(csNone) && (  IO_GetX(xULD_CmpMgz1 ) ||  IO_GetX(xULD_CmpMgz2 ))                       ) EM_SetErr(eiULD_MgzUnknown) ;
        //카세트 사라짐.
        if( !DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_CmpMgz1 ) && !IO_GetX(xULD_CmpMgz2 )) && OM.CmnOptn.bDryRun ) EM_SetErr(eiULD_MgzDispr  ) ; //sun bDryRun 옵션 추가.
        //카세트 OUT 매거진 풀                                                                                                                                  //중복되는 의미 제거.

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Trace(sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick  ) {Trace(sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork  ) {Trace(sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default    :                      Trace(sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scSupply: if(CycleSupply()){ Trace(sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scPick  : if(CyclePick  ()){ Trace(sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace ()){ Trace(sPartName.c_str(),"CyclePlace  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiULD_ToStartTO);

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

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiULD_ToStopTO);

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

        case 10: IO_SetY(yULD_MgzInAC ,false);
                 IO_SetY(yULD_MgzOutAC,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CUnLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yULD_MgzInAC,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: r1 = m_tmTemp.OnDelay(true , 4000) ;
                  r2 = IO_GetX(xULD_MgzIn)           ;
                  if(!r1&&!r2) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmTemp.OnDelay(true ,  200)) return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  if(!IO_GetX(xULD_MgzIn))Stat.bChckSplyEmpty = true ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CyclePick()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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

        case  10: MoveActr(aiULD_Cmp    , ccFwd );
                  MoveMotr(miULD_YCmp   , piULD_YCmpWork) ;
                  IO_SetY(yULD_MgzInAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_Cmp    , ccFwd  )) return false ;
                  if(!MoveMotr(miULD_YCmp   , piULD_YCmpWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpPickFwd))return false ;
                  MoveMotr(miULD_YCmp , piULD_YCmpPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miULD_YCmp , piULD_YCmpPick))return false ;
                  if(!IO_GetX(xULD_MgzIn) && !OM.CmnOptn.bDryRun)return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  MoveMotr(miULD_ZCmp , piULD_ZCmpClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpClampOn)) return false ;
                  MoveActr(aiULD_Cmp , ccBwd);
                  DM.ARAY[riULD].SetStat(csUnkwn) ;
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_Cmp , ccBwd)) return false ;
                  IO_SetY(yULD_MgzInAC,true);
                  MoveMotr(miULD_ZCmp , piULD_ZCmpPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpPickBwd)) return false ;
                  MoveMotr(miULD_YCmp , piULD_YCmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_YCmp , piULD_YCmpWork)) return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CycleWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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

        case  10: MoveMotr(miULD_YCmp , piULD_YCmpWork) ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YCmp , piULD_YCmpWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpWork)) return false ;
                  Step.iCycle = 0;
                  return true ;
    }

}

bool CUnLoader::CyclePlace ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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

        case  10: MoveMotr(miULD_YCmp , piULD_YCmpWork) ;
                  IO_SetY(yULD_MgzOutAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YCmp , piULD_YCmpWork)) return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpPlceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpPlceFwd))return false ;
                  if(!IO_GetX(xULD_MgzOutFull)) {
                      EM_SetErr(eiULD_MgzFull);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  IO_SetY(yLDR_MgzOutAC,false);

                  MoveMotr(miULD_YCmp , piULD_YCmpPlce) ;
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miULD_YCmp , piULD_YCmpPlce))return false ;
                  MoveMotr(miULD_ZCmp , piULD_ZCmpClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpClampOff)) return false ;
                  MoveActr(aiULD_Cmp , ccFwd);
                  DM.ARAY[riULD].SetStat(csNone) ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_Cmp , ccFwd)) return false ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  MoveMotr(miULD_ZCmp , piULD_ZCmpPlceBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miULD_ZCmp , piULD_ZCmpPlceBwd)) return false ;
                  MoveMotr(miULD_YCmp , piULD_YCmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_YCmp , piULD_YCmpWork)) return false ;
                  IO_SetY(yULD_MgzOutAC,false);
                  Step.iCycle = 0;
                  return true ;
    }
}

void CUnLoader::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CUnLoader::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStopInpos(miULD_YCmp)) return false ;
    if(!MT_GetStopInpos(miULD_ZCmp)) return false ;

    if(!AT_Done(aiULD_Cmp   )) return false ;

    return true ;
}

void CUnLoader::Load(bool IsLoad , FILE *fp)
{

}
//---------------------------------------------------------------------------
