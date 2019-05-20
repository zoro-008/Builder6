//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
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
    Load(true);

    InitCycleName();
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
}
void CLoader::Close()
{
    Load(false);
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
    double dZ1Pos = PM.GetValue(miLDR_ZCmp , pvLDR_ZWorkStart) + (OM.DevInfo.iCsSlCnt-r-1) * OM.DevInfo.dCsSlPitch ;
//    double dZ2Pos = PM.GetValue(miLD2_Z , pvLD2_ZWorkStartPs) + (                    r  ) * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miLDR_ZCmp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piLDR_ZWait      : dPos = PM.GetValue (_iMotr , pvLDR_ZWait      ); break ;
            case piLDR_ZPickFwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZPickFwd   ); break ;
            case piLDR_ZClampOn   : dPos = PM.GetValue (_iMotr , pvLDR_ZClampOn   ); break ;
            case piLDR_ZPickBwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZPickBwd   ); break ;
            case piLDR_ZWorkStart : dPos = PM.GetValue (_iMotr , pvLDR_ZWorkStart ); break ;
            case piLDR_ZPlaceFwd  : dPos = PM.GetValue (_iMotr , pvLDR_ZPlaceFwd  ); break ;
            case piLDR_ZClampOff  : dPos = PM.GetValue (_iMotr , pvLDR_ZClampOff  ); break ;
            case piLDR_ZPlaceBwd  : dPos = PM.GetValue (_iMotr , pvLDR_ZPlaceBwd  ); break ;
            case piLDR_ZWork      : dPos = dZ1Pos                                  ; break ;
        }
    }

    else if(_iMotr == miLDR_YCmp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos  (_iMotr                  ); break ;
            case piLDR_YWait      : dPos = PM.GetValue   (_iMotr , pvLDR_YWait    ); break ;
            case piLDR_YPick      : dPos = PM.GetValue   (_iMotr , pvLDR_YPick    ); break ;
            case piLDR_YWork      : dPos = PM.GetValue   (_iMotr , pvLDR_YWork    ); break ;
            case piLDR_YPlace     : dPos = PM.GetValue   (_iMotr , pvLDR_YPlace   ); break ;
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

    bool isFwPusher      = !AT_Complete(aiLDR_PshrFwBw,ccBwd); //IO_GetX(xLD1_PusherFw) || !IO_GetX(xLD1_PusherBw);
    bool isRailGateCnsr  =  IO_GetX(xPRB_PkgIn1 ) ;/*|| IO_GetX(xPRB_2Pkg)*/
    bool isClampYFwd     =  MT_CmprPos(miLDR_YCmp , GetMotrPos(miLDR_YCmp , piLDR_YPick)) || MT_CmprPos(miLDR_YCmp , GetMotrPos(miLDR_YCmp , piLDR_YPlace));
    bool isClampZUpstair =  MT_CmprPos(miLDR_ZCmp , piLDR_ZPickBwd)  || MT_CmprPos(miLDR_ZCmp , piLDR_ZPickFwd)  || MT_CmprPos(miLDR_ZCmp , piLDR_ZClampOn)   ;
    bool isClampZCenter  =  MT_CmprPos(miLDR_ZCmp , piLDR_ZWait)     || MT_CmprPos(miLDR_ZCmp , piLDR_ZWork)     || MT_CmprPos(miLDR_ZCmp , piLDR_ZWorkStart) ;
    bool isClampZDnstair =  MT_CmprPos(miLDR_ZCmp , piLDR_ZPlaceBwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZPlaceFwd) || MT_CmprPos(miLDR_ZCmp , piLDR_ZClampOff)  ;

    if(_iMotr == miLDR_ZCmp){
             if(isFwPusher    ) { sMsg = "Pusher is not Bwd         " ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg = "Strip is feeding now      " ; bRet = false ;}
        switch(_iPstnId) {
            default                   :                                                                                                                   break ;
                //UpStair
                case piLDR_ZPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piLDR_ZWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZWorkStart : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piLDR_ZPlaceFwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLDR_ZPlaceBwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLDR_YCmp)+" is Fwd" ; bRet = false ;} break ;
        }
    }

    else if(_iMotr == miLDR_YCmp){
        if(isFwPusher    ) { sMsg = "Pusher is not Bwd         " ; bRet = false ;}
        else {
            switch(_iPstnId) {
                default                   :                                                                                           break ;
                    case piLDR_YWait      :                                                                                           break ;
                    case piLDR_YWork      :                                                                                           break ;
                    case piLDR_YPick      : if(isClampZCenter) {sMsg = MT_GetName(miLDR_YCmp)+" is Middle Position" ; bRet = false ;} break ;
                    case piLDR_YPlace     : if(isClampZCenter) {sMsg = MT_GetName(miLDR_YCmp)+" is Middle Position" ; bRet = false ;} break ;
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
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MoveActr(aiLDR_PshrFwBw, ccBwd);
                 IO_SetY(yLDR_MgzInAC ,false);
                 IO_SetY(yLDR_MgzOutAC,false);
                 Step.iHome++;
                 return false ;

        case 11: if(!IO_GetX(xLDR_PshrBw))return false ;
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
                 MT_GoAbsMan(miLDR_YCmp, GetMotrPos(miLDR_YCmp,piLDR_YWait));
                 MT_GoAbsMan(miLDR_ZCmp, GetMotrPos(miLDR_ZCmp,piLDR_ZWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miLDR_YCmp, GetMotrPos(miLDR_YCmp,piLDR_YWait)))return false ;
                 if(!MT_GoAbsMan(miLDR_ZCmp, GetMotrPos(miLDR_ZCmp,piLDR_ZWait)))return false ;
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
        AnsiString sTemp = GetLotNo() ;

        bool isCycleSupply =  DM.ARAY[riLDR].CheckAllStat(csNone )    && !Stat.bChckSplyEmpty && !IO_GetX(xLDR_MgzIn )&& ((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || (GetLotNo()!="")) ;
        bool isCyclePick   =  DM.ARAY[riLDR].CheckAllStat(csNone )    && (IO_GetX(xLDR_MgzIn) ||  OM.MstOptn.bDryRun )&& ((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || (GetLotNo()!="")) ;
        bool isCycleWork   =  DM.ARAY[riLDR].GetCntStat  (csUnkwn)    &&  DM.ARAY[riPRB].CheckAllStat(csNone) && !IO_GetX(xPRB_Pkg) &&
                              PRB.GetCycleStep() == CPreBuff::scIdle && !OM.CmnOptn.bLoadingStop;
        bool isCyclePlace  =  DM.ARAY[riLDR].CheckAllStat(csEmpty) ;
        bool isCycleEnd    =  DM.ARAY[riLDR].CheckAllStat(csNone ) && GetLotNo()=="" && m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;
                              /*m_iMgzCnt > OM.DevOptn.iLotEndMgzCnt*/


        //모르는 스트립에러.
        if(  DM.ARAY[riLDR].CheckAllStat(csNone) && (  IO_GetX(xLDR_MgzDetect1) ||  IO_GetX(xLDR_MgzDetect2) )     ) EM_SetErr(eiLDR_MgzUnknown) ;
        //카세트 사라짐.
        if( !DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) )     ) EM_SetErr(eiLDR_MgzDispr  ) ;
        //카세트 OUT 매거진 풀

        AnsiString sTemp9 ;

        sTemp9 = GetLotNo();

        if (((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt) || GetLotNo()!="") && Stat.bChckSplyEmpty && !IO_GetX(xLDR_MgzIn) && !OM.CmnOptn.bLoadingStop) {
            EM_SetErr(eiLDR_NeedMgz) ;
            Trace("m_iMgzCnt",String(m_iMgzCnt).c_str());
            Trace("GetLotNo",GetLotNo().c_str());
        }

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

        case 10: Stat.bChckSplyEmpty = false ;
                 MoveActr(aiLDR_PshrFwBw, ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_PshrFwBw, ccBwd)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiLDR_ToStopTO);

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
                 MoveActr(aiLDR_PshrFwBw, ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLDR_PshrFwBw, ccBwd)) return false;
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

    String sLotNo ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiLDR_ClmpUpDn , ccFwd      );
                  MoveMotr(miLDR_YCmp     , piLDR_YWork);
                  MoveActr(aiLDR_PshrFwBw , ccBwd      );
                  IO_SetY(yLDR_MgzInAC ,true );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_ClmpUpDn , ccFwd      )) return false ;
                  if(!MoveMotr(miLDR_YCmp     , piLDR_YWork)) return false ;
                  if(!MoveActr(aiLDR_PshrFwBw , ccBwd      )) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZPickFwd))return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YCmp , piLDR_YPick))return false ;
                  if(!IO_GetX(xLDR_MgzIn) && !OM.MstOptn.bDryRun)return false ;
                  //IO_SetY(yLDR_MgzInAC,false);
                  MoveMotr(miLDR_ZCmp , piLDR_ZClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZCmp , piLDR_ZClampOn)) return false ;
                  DM.ARAY[riLDR].SetStat(csUnkwn) ;
                  /*
                  if(m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt ) {
                      m_iMgzCnt++;
                      if(m_iMgzCnt == 1) LT.LotOpen(PopLotNo());
                  }
                  else if(m_iMgzCnt == 1 && OM.DevOptn.iLotEndMgzCnt == 1) {
                      m_iMgzCnt = 1;
                      LT.LotOpen(PopLotNo());
                  }
                  else if(m_iMgzCnt > OM.DevOptn.iLotEndMgzCnt){
                      m_iMgzCnt = 1 ;
                      LT.LotOpen(PopLotNo());
                  }*/
                  m_iMgzCnt++;              //1

                  if(m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt || m_iMgzCnt ==0) {
                      sLotNo = PopLotNo();
                      LT.LotOpen(PopLotNo(), OM.m_sCrntDev );
                      LT.LotInfo.sLotNo = PopLotNo();// JS 랏이 종료 되면 기록이 없음 그래서 추가///
                      //LT.SaveLotInfo();
                      //if(OM.MstOptn.bNotUstLot) { DM.ARAY[riLDR].SetLotNo(Now().FormatString("yymmdd(hhnnss)")); }
                      //else DM.ARAY[riLDR].SetLotNo(PopLotNo()) ;
                      DM.ARAY[riLDR].SetLotNo(PopLotNo()) ;

                      LT.AddDayInfoLotCnt(1);
                      m_iMgzCnt = 1;
                  }

                  Step.iCycle++;
                  return false ;


        case  15: MoveActr(aiLDR_ClmpUpDn , ccBwd);
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiLDR_ClmpUpDn , ccBwd)) return false ;
                  IO_SetY(yLDR_MgzInAC,true);
                  MoveMotr(miLDR_ZCmp , piLDR_ZPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_ZCmp , piLDR_ZPickBwd)) return false ;
                  if(OM.MstOptn.bLdr_YHome) { m_bIsHome = true ; MT_DoHome(miLDR_YCmp); }
                  Step.iCycle++;
                  return false ;

        case  18: if(OM.MstOptn.bLdr_YHome && !MT_GetHomeEnd(miLDR_YCmp) ) return false ;
                  m_bIsHome = false ;
                  MoveMotr(miLDR_YCmp , piLDR_YWork) ;
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveMotr(miLDR_YCmp , piLDR_YWork)) return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  LT.LotInfo.iLotMgzCnt++;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CycleWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
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

    int iSltCnt ;
    bool sTemp1 , sTemp2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miLDR_YCmp , piLDR_YWork) ;
                  MoveActr(aiLDR_PshrFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YCmp     , piLDR_YWork)) return false ;
                  if(!MoveActr(aiLDR_PshrFwBw , ccBwd     )) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZWork)) return false ;
                  MoveActr(aiLDR_PshrFwBw , ccFwd);

//                  if(OM.MstOptn.bNotUstLot) {
//                      iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
//                      DM.ARAY[riPR1].SetID(m_iMgzCnt * 100 + iSltCnt);
//                      DM.ARAY[riPR1].SetLotNo(Now().FormatString("yymmdd(hhnnss)"));
//                  }
//                  else {
                      iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
                      DM.ARAY[riPRB].SetID(m_iMgzCnt * 100 + iSltCnt);
                      DM.ARAY[riPRB].SetLotNo(DM.ARAY[riLDR].GetLotNo());
//                  }

                  Step.iCycle++;
                  return false;

        case  13: if(IO_GetX(xLDR_PshrOverLoad)) {
                      MoveActr(aiLDR_PshrFwBw, ccBwd);
                      EM_SetErr(eiLDR_PshrOverload);
                      Step.iCycle = 20;
                      return false ;
                  }
                  if(!MoveActr(aiLDR_PshrFwBw , ccFwd)) return false;

//                  DM.ShiftArrayData(riLDR,riPRB);
                  if(!IO_GetX(xLDR_PshrFw)) return false;
                  MoveActr(aiLDR_PshrFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiLDR_PshrFwBw, ccBwd)) return false;
                  if( IO_GetX(xPRB_PkgIn1) /*|| IO_GetX(xPRB_2Pkg)*/) return false ;
                                         //센서위치가 이상해서 적용 하면 계속 리턴됌.
//                  iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
//                  DM.ARAY[riPR1].SetID(m_iMgzCnt * 100 + iSltCnt);
//                  DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo());
                  DM.ARAY[riLDR].SetStat(DM.ARAY[riLDR].FindLastRow(csUnkwn),0,csEmpty);

                  MoveMotr(miLDR_ZCmp , piLDR_ZWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLDR_ZCmp , piLDR_ZWork))return false ;

                  Step.iCycle = 0;
                  return true ;

        case  20: if(!MoveActr(aiLDR_PshrFwBw , ccBwd))return false;
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

        case  10: MoveMotr(miLDR_YCmp , piLDR_YWork) ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  IO_SetY(yLDR_MgzInAC ,true); //미리 좀 돌려 놓는다.
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YCmp , piLDR_YWork)) return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZPlaceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZCmp , piLDR_ZPlaceFwd))return false ;

                  if(IO_GetX(xLDR_MgzOutFull)) {
                      EM_SetErr(eiLDR_MgzFull);
                      Step.iCycle = 0;
                      return true;
                  }
                  IO_SetY(yLDR_MgzOutAC,false);
                  MoveMotr(miLDR_YCmp , piLDR_YPlace) ; //풀 에러 ㄱㄱ씽.
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YCmp , piLDR_YPlace))return false ;
                  MoveMotr(miLDR_ZCmp , piLDR_ZClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZCmp , piLDR_ZClampOff)) return false ;
                  MoveActr(aiLDR_ClmpUpDn , ccFwd);
                  DM.ARAY[riLDR].SetStat(csNone) ;
                  if(m_iMgzCnt == OM.DevOptn.iLotEndMgzCnt) DM.ARAY[riLDR].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_ClmpUpDn , ccFwd)) return false ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  MoveMotr(miLDR_ZCmp , piLDR_ZPlaceBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_ZCmp , piLDR_ZPlaceBwd)) return false ;
                  MoveMotr(miLDR_YCmp , piLDR_YWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_YCmp , piLDR_YWork)) return false ;

                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , 3000))return false;
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

    if(!AT_Done(aiLDR_ClmpUpDn   )) return false ;
    if(!AT_Done(aiLDR_PshrFwBw   )) return false ;

    return true ;
}

void CLoader::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Loader.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
    }
}
//---------------------------------------------------------------------------
