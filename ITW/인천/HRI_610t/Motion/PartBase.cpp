//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PartBase.h"
//#include "PreBuff.h"
//---------------------------------------------------------------------------
//#include "SLogUnit.h"
//#include "SMInterfaceUnit.h"
//#include "UtilDefine.h"
//#include "PstnMan.h"
//#include "OptionMan.h"
//#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLoader LDR;

CLoader::CLoader(void)
{

}

CLoader::~CLoader (void)
{

}

void CLoader::Init()
{
    m_sPartName = "PartBase " ;
    Reset();
    Load(true);
}

void CLoader::Close()
{
    Load(false);
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
    m_tmDelay  .Clear();
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
    double dZ1Pos = PM.GetValue(miLDR_ZCmp , pvLDR_ZCmpWorkStart) + (OM.DevInfo.iCsSlCnt-r-1) * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miLDR_ZCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piLDR_ZCmpWait      : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpWait      ); break ;
            case piLDR_ZCmpPickFwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPickFwd   ); break ;
            case piLDR_ZCmpClampOn   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpClampOn   ); break ;
            case piLDR_ZCmpPickBwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPickBwd   ); break ;
            case piLDR_ZCmpWorkStart : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpWorkStart ); break ;
            case piLDR_ZCmpPlaceFwd  : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPlaceFwd  ); break ;
            case piLDR_ZCmpClampOff  : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpClampOff  ); break ;
            case piLDR_ZCmpPlaceBwd  : dPos = PM.GetValue (_iMotr , pvLDR_ZCmpPlaceBwd  ); break ;
            case piLDR_ZCmpWork      : dPos = dZ1Pos                                     ; break ;
        }
    }

    else if(_iMotr == miLDR_YCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos  (_iMotr                     ); break ;
            case piLDR_YCmpWait      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpWait    ); break ;
            case piLDR_YCmpPick      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpPick    ); break ;
            case piLDR_YCmpWork      : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpWork    ); break ;
            case piLDR_YCmpPlace     : dPos = PM.GetValue   (_iMotr , pvLDR_YCmpPlace   ); break ;
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

    bool isFwPusher      = AT_Complete(aiLDR_PshFwBw,ccFwd); //IO_GetX(xLD1_PusherFw) || !IO_GetX(xLD1_PusherBw);
    bool isRailGateCnsr  = IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_PkgIn2) ;
    bool isClampYFwd     = MT_CmprPos(miLDR_YCmp , GetMotrPos(miLDR_YCmp , piLDR_YCmpPick)) || MT_CmprPos(miLDR_YCmp , GetMotrPos(miLDR_YCmp , piLDR_YCmpPlace));
    bool isClampZUpstair = MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPickBwd)  || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPickFwd)  || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpClampOn)   ;
    bool isClampZCenter  = MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpWait)     || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpWork)     || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpWorkStart) ;
    bool isClampZDnstair = MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPlaceBwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpPlaceFwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZCmpClampOff)  ;

    if(_iMotr == miLDR_ZCmp){
             if(isFwPusher    ) { sMsg = "Pusher Ac Motor is not Bwd" ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg = "Strip is feeding now      " ; bRet = false ;}
        switch(_iPstnId) {
            default                   :                                                                                                                                  break ;
                //UpStair
                case piLDR_ZCmpPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piLDR_ZCmpWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpWorkStart : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piLDR_ZCmpPlaceFwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZCmpPlaceBwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Fwd" ; bRet = false ;} break ;
        }
    }

    else if(_iMotr == miLDR_YCmp){
        if(isFwPusher) return false ;
        else {
            switch(_iPstnId) {
                default                      :                                                                                                       break ;
                    case piLDR_YCmpWait      :                                                                                                       break ;
                    case piLDR_YCmpWork      :                                                                                                       break ;
                    case piLDR_YCmpPick      : if(isClampZCenter) {sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Middle Position" ; bRet = false ;} break ;
                    case piLDR_YCmpPlace     : if(isClampZCenter) {sMsg = AnsiString(MT_GetName(miLDR_YCmp))+" is Middle Position" ; bRet = false ;} break ;
            }
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
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

void CLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MoveActr(aiLDR_PshFwBw, ccBwd);
                 IO_SetY(yLDR_MgzInAC ,false);
                 IO_SetY(yLDR_MgzOutAC,false);
                 Step.iHome++;
                 return false ;

        case 11: if(!IO_GetX(xLDR_PusherBw))return false ;
                 MT_SetHomeEnd(miLDR_ZCmp , true) ;
                 MT_ClearPos(miLDR_ZCmp);
                 MT_GoIncMan(miLDR_ZCmp,-10); //12mm
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetStopInpos(miLDR_ZCmp))return false;
                 MT_DoHome(miLDR_YCmp);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miLDR_YCmp))return false ;
                 MT_DoHome(miLDR_ZCmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miLDR_ZCmp))return false ;
                 MT_GoAbsMan(miLDR_YCmp, GetMotrPos(miLDR_YCmp,piLDR_YCmpWait));
                 MT_GoAbsMan(miLDR_ZCmp, GetMotrPos(miLDR_ZCmp,piLDR_ZCmpWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miLDR_YCmp, GetMotrPos(miLDR_YCmp,piLDR_YCmpWait)))return false ;
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
        bool isNeedWork    = (m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)||LTQ.GetLotCnt() ;

        bool isCycleSupply =  DM.ARAY[riLDR].CheckAllStat(csNone )  && !Stat.bChckSplyEmpty && !IO_GetX(xLDR_MgzIn )&& isNeedWork ;
        bool isCyclePick   =  DM.ARAY[riLDR].CheckAllStat(csNone )  && (IO_GetX(xLDR_MgzIn) ||  OM.MstOptn.bDryRun )&& isNeedWork ;
        bool isCycleWork   =  DM.ARAY[riLDR].GetCntStat  (csUnkwn)  &&  DM.ARAY[riPRB].CheckAllStat(csNone ) &&
                              PRB.GetCycleStep() == CPreBuff::scIdle&& !OM.CmnOptn.bLoadingStop;
        bool isCyclePlace  =  DM.ARAY[riLDR].CheckAllStat(csEmpty) ;
        bool isCycleEnd    =  DM.ARAY[riLDR].CheckAllStat(csNone ) && (LTQ.GetLotCnt()==0 && m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt)  ;


        //모르는 스트립에러.
        if(  DM.ARAY[riLDR].CheckAllStat(csNone) && (  IO_GetX(xLDR_MgzDetect1) ||  IO_GetX(xLDR_MgzDetect2))) EM_SetErrMsg(eiPKG_Unknwn , m_sPartName.c_str());//"Loader") ;
        //카세트 사라짐.
        if( !DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2))) EM_SetErrMsg(eiPKG_Dispr  , m_sPartName.c_str());//"Loader") ;
        //카세트 OUT 매거진 풀


        if (((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || LTQ.GetLotCnt()==0)&& Stat.bChckSplyEmpty && !IO_GetX(xLDR_MgzIn) && !OM.CmnOptn.bLoadingStop) EM_SetErr(eiLDR_NeedMgz) ;

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
        default    :                      Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                       return false ;
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

    Step.iToStop = 10;
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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: Stat.bChckSplyEmpty = false ;
                 MoveActr(aiLDR_PshFwBw, ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_PshFwBw, ccBwd)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10: IO_SetY(yLDR_MgzInAC , false);
                 IO_SetY(yLDR_MgzOutAC, false);
                 MoveActr(aiLDR_PshFwBw, ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_PshFwBw, ccBwd)) return false;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

        case  11: r1 = m_tmTemp.OnDelay(true , 5000) ;
                  r2 = IO_GetX(xLDR_MgzIn)           ;
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
        //EM_SetErr(eiLDR_CycleTO);
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str());
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

        case  10: MoveActr(aiLDR_CmpUpDn , ccFwd         );
                  MoveMotr(miLDR_YCmp    , piLDR_YCmpWork);
                  MoveActr(aiLDR_PshFwBw , ccBwd  );
                  IO_SetY (yLDR_MgzInAC ,true );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_CmpUpDn, ccFwd         )) return false ;
                  if(!MoveMotr(miLDR_YCmp   , piLDR_YCmpWork)) return false ;
                  if(!MoveActr(aiLDR_PshFwBw, ccBwd         )) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickFwd))return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpPick))return false ;
                  if(!IO_GetX(xLDR_MgzIn) && !OM.MstOptn.bDryRun)return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOn)) return false ;
                  DM.ARAY[riLDR].SetStat(csUnkwn) ;

                  if(m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt) {
                      m_iMgzCnt = 0;
                      DM.ARAY[riLDR].SetLotNo(LTQ.PopLotNo()) ;
                      DM.ARAY[riLDR].SetID(m_iMgzCnt * 100 ) ;
                  }
                  m_iMgzCnt++;
                  Step.iCycle++;
                  return false ;


        case  15: MoveActr(aiLDR_CmpUpDn , ccBwd);
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiLDR_CmpUpDn , ccBwd)) return false ;
                  IO_SetY(yLDR_MgzInAC,true);
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPickBwd)) return false ;
                  if(OM.MstOptn.bLdr_YHome) { m_bIsHome = true ; MT_DoHome(miLDR_YCmp); }
                  Step.iCycle++;
                  return false ;

        case  18: if(OM.MstOptn.bLdr_YHome && !MT_GetHomeEnd(miLDR_YCmp) ) return false ;
                  m_bIsHome = false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CycleWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

    int iSltCnt ;
    bool sTemp1 , sTemp2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miLDR_YCmp   , piLDR_YCmpWork);
                  MoveActr(aiLDR_PshFwBw, ccBwd         );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YCmp   , piLDR_YCmpWork)) return false ;
                  if(!MoveActr(aiLDR_PshFwBw, ccBwd         )) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork)) return false ;
                  MoveActr(aiLDR_PshFwBw, ccFwd);

                  iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindLastRow(csUnkwn) - 1;
                  DM.ARAY[riPRB].SetID((DM.ARAY[riLDR].GetID() / 100 * 100) + iSltCnt);
                  DM.ARAY[riPRB].SetLotNo(DM.ARAY[riLDR].GetLotNo());

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiLDR_PshFwBw, ccFwd)) return false;
                  if(IO_GetX(xLDR_PusherOvld)) {
                      MoveActr(aiLDR_PshFwBw, ccBwd);
                      EM_SetErr(eiLDR_PshrOvld);
                      Step.iCycle = 0;
                      return true ;
                  }
//                  DM.ShiftArrayData(riLDR,riPRB);
                  if(!IO_GetX(xLDR_PusherFw)) return false;
                  MoveActr(aiLDR_PshFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiLDR_PshFwBw, ccBwd)) return false;
                  if( IO_GetX(xPRB_PkgIn1) /*|| IO_GetX(xPRB_2Pkg)*/) return false ;
                                         //센서위치가 이상해서 적용 하면 계속 리턴됌.
                  DM.ARAY[riLDR].SetStat(DM.ARAY[riLDR].FindLastRow(csUnkwn),0,csEmpty);

                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpWork))return false ;

                  Step.iCycle = 0;
                  return true ;
    }

}

bool CLoader::CyclePlace ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str());//EM_SetErr(eiLDR_CycleTO);
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
                  IO_SetY(yLDR_MgzInAC ,true); //미리 좀 돌려 놓는다.
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlaceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlaceFwd))return false ;
                  
                  if(IO_GetX(xLDR_MgzOutFull)) {
                      EM_SetErr(eiLDR_FullMgz);
                      Step.iCycle = 0;
                      return true;
                  }
                  IO_SetY(yLDR_MgzOutAC,false);
                  MoveMotr(miLDR_YCmp , piLDR_YCmpPlace) ; //풀 에러 ㄱㄱ씽.
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpPlace))return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpClampOff)) return false ;
                  MoveActr(aiLDR_CmpUpDn , ccFwd);
                  DM.ARAY[riLDR].SetStat(csNone) ;
                  if(m_iMgzCnt == OM.DevOptn.iLotEndMgzCnt) DM.ARAY[riLDR].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_CmpUpDn , ccFwd)) return false ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlaceBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_ZCmp , piLDR_ZCmpPlaceBwd)) return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YCmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_YCmp , piLDR_YCmpWork)) return false ;
                  IO_SetY(yLDR_MgzOutAC,false);
                  IO_SetY(yLDR_MgzInAC ,false); 
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

    if(!AT_Done(aiLDR_CmpUpDn)) return false ;
    if(!AT_Done(aiLDR_PshFwBw)) return false ;

    return true ;
}

void CLoader::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Loader.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
    }
}
//---------------------------------------------------------------------------
