//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "PreBuff.h"
#include "UserIni.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLoader LD1;

CLoader::CLoader(void)
{
    m_sPartName = "Loader " ;
    Reset();
    Load(true);
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
    Load(false);
}

bool CLoader::FindChip(int &r , int &c )
{
    r = DM.ARAY[riLD1].FindLastRow(csUnkwn) ;
    return (r > -1 && r > -1);
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
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
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLD1_ZWait      : dPos = PM.GetValue (_iMotr , pvLD1_ZWaitPs     ); break ;
            case piLD1_ZPickFwd   : dPos = PM.GetValue (_iMotr , pvLD1_ZPickFwdPs  ); break ;
            case piLD1_ZClampOn   : dPos = PM.GetValue (_iMotr , pvLD1_ZClampOnPs  ); break ;
            case piLD1_ZPickBwd   : dPos = PM.GetValue (_iMotr , pvLD1_ZPickBwdPs  ); break ;
            case piLD1_ZWorkStart : dPos = PM.GetValue (_iMotr , pvLD1_ZWorkStartPs); break ;
            case piLD1_ZPlaceFwd  : dPos = PM.GetValue (_iMotr , pvLD1_ZPlaceFwdPs ); break ;
            case piLD1_ZClampOff  : dPos = PM.GetValue (_iMotr , pvLD1_ZClampOffPs ); break ;
            case piLD1_ZPlaceBwd  : dPos = PM.GetValue (_iMotr , pvLD1_ZPlaceBwdPs ); break ;
            case piLD1_ZWork      : dPos = dZ1Pos                                   ; break ;
        }
    }

    else if(_iMotr == miLD1_YCmp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos  (_iMotr                    ); break ;
            case piLD1_YWait      : dPos = PM.GetValue   (_iMotr , pvLD1_YWaitPs    ); break ;
            case piLD1_YPick      : dPos = PM.GetValue   (_iMotr , pvLD1_YPickPs    ); break ;
            case piLD1_YWork      : dPos = PM.GetValue   (_iMotr , pvLD1_YWorkPs    ); break ;
            case piLD1_YPlace     : dPos = PM.GetValue   (_iMotr , pvLD1_YPlacePs    ); break ;
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

    bool isFwPusher      = AT_Complete(aiLD1_Psh,ccFwd); //IO_GetX(xLD1_PusherFw) || !IO_GetX(xLD1_PusherBw);
    bool isRailGateCnsr  = IO_GetX(xPRB_1Pkg) ;/*|| IO_GetX(xPRB_2Pkg)*/ 
    bool isClampYFwd     = MT_CmprPos(miLD1_YCmp , GetMotrPos(miLD1_YCmp , piLD1_YPick)) || MT_CmprPos(miLD1_YCmp , GetMotrPos(miLD1_YCmp , piLD1_YPlace));
    bool isClampZUpstair = MT_CmprPos(miLD1_ZCmp , piLD1_ZPickBwd)  || MT_CmprPos(miLD1_ZCmp , piLD1_ZPickFwd)  || MT_CmprPos(miLD1_ZCmp , piLD1_ZClampOn)   ;
    bool isClampZCenter  = MT_CmprPos(miLD1_ZCmp , piLD1_ZWait)     || MT_CmprPos(miLD1_ZCmp , piLD1_ZWork)     || MT_CmprPos(miLD1_ZCmp , piLD1_ZWorkStart) ;
    bool isClampZDnstair = MT_CmprPos(miLD1_ZCmp , piLD1_ZPlaceBwd) || MT_CmprPos(miLD1_ZCmp , piLD1_ZPlaceFwd) || MT_CmprPos(miLD1_ZCmp , piLD1_ZClampOff)  ;

    if(_iMotr == miLD1_ZCmp){
             if(isFwPusher    ) { sMsg = "Pusher Ac Motor is not Bwd" ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg = "Strip is feeding now      " ; bRet = false ;}
        switch(_iPstnId) {
            default                   :                                                                                                                   break ;
                //UpStair
                case piLD1_ZPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLD1_ZClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLD1_ZPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piLD1_ZWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLD1_ZWorkStart : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLD1_ZWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piLD1_ZPlaceFwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLD1_ZClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
                case piLD1_ZPlaceBwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = MT_GetName(miLD1_YCmp)+" is Fwd" ; bRet = false ;} break ;
        }
    }

    else if(_iMotr == miLD1_YCmp){
        if(isFwPusher) return false ;
        else {
            switch(_iPstnId) {
                default                   :                                                                                           break ;
                    case piLD1_YWait      :                                                                                           break ;
                    case piLD1_YWork      :                                                                                           break ;
                    case piLD1_YPick      : if(isClampZCenter) {sMsg = MT_GetName(miLD1_YCmp)+" is Middle Position" ; bRet = false ;} break ;
                    case piLD1_YPlace     : if(isClampZCenter) {sMsg = MT_GetName(miLD1_YCmp)+" is Middle Position" ; bRet = false ;} break ;
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

    bool isZMoving = !MT_GetStopInpos(miLD1_ZCmp);

    /*
        }
    else if(_iActr == aiPSB_PusherFB){
        if(!AT_Complete(aiPSB_PusherUD , ccFwd)){ sMsg = AT_GetName(aiPSB_PusherUD )+" is not Fwd" ; bRet = false ;}

        if(!MT_Stop(miPSB_XTrm))

    */

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
        EM_SetErr(eiLD1_HomeTO);
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

        case 10: MoveActr(aiLD1_Psh, ccBwd);
                 IO_SetY(yLD1_MgzInAC ,false);
                 IO_SetY(yLD1_MgzOutAC,false);
                 Step.iHome++;
                 return false ;

        case 11: if(!IO_GetX(xLD1_PusherBw))return false ;
                 MT_SetHomeEnd(miLD1_ZCmp , true) ;
                 MT_ClearPos(miLD1_ZCmp);
                 MT_GoIncMan(miLD1_ZCmp,-10); //12mm
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetStopInpos(miLD1_ZCmp))return false;
                 MT_DoHome(miLD1_YCmp);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miLD1_YCmp))return false ;
                 MT_DoHome(miLD1_ZCmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miLD1_ZCmp))return false ;
                 MT_GoAbsMan(miLD1_YCmp, GetMotrPos(miLD1_YCmp,piLD1_YWait));
                 MT_GoAbsMan(miLD1_ZCmp, GetMotrPos(miLD1_ZCmp,piLD1_ZWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miLD1_YCmp, GetMotrPos(miLD1_YCmp,piLD1_YWait)))return false ;
                 if(!MT_GoAbsMan(miLD1_ZCmp, GetMotrPos(miLD1_ZCmp,piLD1_ZWait)))return false ;
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

        bool isCycleSupply =  DM.ARAY[riLD1].CheckAllStat(csNone )  && !Stat.bChckSplyEmpty && !IO_GetX(xLD1_MgzIn )&& ((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || (GetLotNo()!="" || OM.MstOptn.bNotUstLot)) ;
        bool isCyclePick   =  DM.ARAY[riLD1].CheckAllStat(csNone )  && (IO_GetX(xLD1_MgzIn) ||  OM.CmnOptn.bDryRun )&& ((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || (GetLotNo()!="" || OM.MstOptn.bNotUstLot)) ;
        bool isCycleWork   =  DM.ARAY[riLD1].GetCntStat  (csUnkwn)  &&  DM.ARAY[riPR1].CheckAllStat(csNone ) && DM.ARAY[riPR2].CheckAllStat(csNone ) &&
                              PRB.GetCycleStep() == CPreBuff::scIdle&& !OM.CmnOptn.bLoadingStop;
        bool isCyclePlace  =  DM.ARAY[riLD1].CheckAllStat(csEmpty) ;
        bool isCycleEnd    =  DM.ARAY[riLD1].CheckAllStat(csNone ) && GetLotNo()=="" && m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;
                              /*m_iMgzCnt > OM.DevOptn.iLotEndMgzCnt*/


        //모르는 스트립에러.
        if(  DM.ARAY[riLD1].CheckAllStat(csNone) && (  IO_GetX(xLD1_MgzDetect1) ||  IO_GetX(xLD1_MgzDetect2) )     ) EM_SetErr(eiLD1_ClampUnknown) ;
        //카세트 사라짐.
        if( !DM.ARAY[riLD1].CheckAllStat(csNone) && ( !IO_GetX(xLD1_MgzDetect1) && !IO_GetX(xLD1_MgzDetect2) )     ) EM_SetErr(eiLD1_ClampDispr)   ;
        //카세트 OUT 매거진 풀


        if (((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || GetLotNo()!="")&& Stat.bChckSplyEmpty && !IO_GetX(xLD1_MgzIn) && !OM.CmnOptn.bLoadingStop) EM_SetErr(eiLD1_NeedMgz) ;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiLD1_ToStartTO);

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
                 MoveActr(aiLD1_Psh, ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLD1_Psh, ccBwd)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiLD1_ToStopTO);

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

        case 10: IO_SetY(yLD1_MgzInAC , false);
                 IO_SetY(yLD1_MgzOutAC, false);
                 MoveActr(aiLD1_Psh, ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiLD1_Psh, ccBwd)) return false;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLD1_CycleTO);
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

        case  10: IO_SetY(yLD1_MgzInAC,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: r1 = m_tmTemp.OnDelay(true , 4000) ;
                  r2 = IO_GetX(xLD1_MgzIn)           ;
                  if(!r1&&!r2) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmTemp.OnDelay(true ,  200)) return false ;
                  IO_SetY(yLD1_MgzInAC,false);
                  if(!IO_GetX(xLD1_MgzIn))Stat.bChckSplyEmpty = true ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CLoader::CyclePick()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLD1_CycleTO);
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

        case  10: MoveActr(aiLD1_Cmp    , ccFwd      );
                  MoveMotr(miLD1_YCmp   , piLD1_YWork);
                  MoveActr(aiLD1_Psh , ccBwd      );
                  IO_SetY(yLD1_MgzInAC ,true );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLD1_Cmp    , ccFwd      )) return false ;
                  if(!MoveMotr(miLD1_YCmp   , piLD1_YWork)) return false ;
                  if(!MoveActr(aiLD1_Psh , ccBwd      )) return false ;
                  MoveMotr(miLD1_ZCmp , piLD1_ZPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLD1_ZCmp , piLD1_ZPickFwd))return false ;
                  MoveMotr(miLD1_YCmp , piLD1_YPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLD1_YCmp , piLD1_YPick))return false ;
                  if(!IO_GetX(xLD1_MgzIn) && !OM.CmnOptn.bDryRun)return false ;
                  IO_SetY(yLD1_MgzInAC,false);
                  MoveMotr(miLD1_ZCmp , piLD1_ZClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLD1_ZCmp , piLD1_ZClampOn)) return false ;
                  DM.ARAY[riLD1].SetStat(csUnkwn) ;
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

                  if(m_iMgzCnt > OM.DevOptn.iLotEndMgzCnt) {
                      //LT.LotOpen(PopLotNo(), OM.m_sCrntDev );
                      //LT.SaveLotInfo();
                      if(OM.MstOptn.bNotUstLot) { DM.ARAY[riLD1].SetLotNo(Now().FormatString("yymmdd(hhnnss)")); }
                      else DM.ARAY[riLD1].SetLotNo(PopLotNo()) ;

                      LT.AddDayInfoLotCnt(1);
                      m_iMgzCnt = 1;
                  }
                  Step.iCycle++;
                  return false ;


        case  15: MoveActr(aiLD1_Cmp , ccBwd);
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiLD1_Cmp , ccBwd)) return false ;
                  IO_SetY(yLD1_MgzInAC,true);
                  MoveMotr(miLD1_ZCmp , piLD1_ZPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLD1_ZCmp , piLD1_ZPickBwd)) return false ;
                  if(OM.MstOptn.bLdr_YHome) { m_bIsHome = true ; MT_DoHome(miLD1_YCmp); }
                  Step.iCycle++;
                  return false ;

        case  18: if(OM.MstOptn.bLdr_YHome && !MT_GetHomeEnd(miLD1_YCmp) ) return false ;
                  m_bIsHome = false ;
                  MoveMotr(miLD1_YCmp , piLD1_YWork) ;
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveMotr(miLD1_YCmp , piLD1_YWork)) return false ;
                  IO_SetY(yLD1_MgzInAC,false);
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
        EM_SetErr(eiLD1_CycleTO);
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

        case  10: MoveMotr(miLD1_YCmp , piLD1_YWork) ;
                  MoveActr(aiLD1_Psh, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLD1_YCmp , piLD1_YWork)) return false ;
                  if(!MoveActr(aiLD1_Psh, ccBwd     )) return false ;
                  MoveMotr(miLD1_ZCmp , piLD1_ZWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLD1_ZCmp , piLD1_ZWork)) return false ;
                  MoveActr(aiLD1_Psh, ccFwd);

//                  if(OM.MstOptn.bNotUstLot) {
//                      iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLD1].FindLastRow(csUnkwn) ;
//                      DM.ARAY[riPR1].SetID(m_iMgzCnt * 100 + iSltCnt);
//                      DM.ARAY[riPR1].SetLotNo(Now().FormatString("yymmdd(hhnnss)"));
//                  }
//                  else {
                      iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLD1].FindLastRow(csUnkwn) ;
                      DM.ARAY[riPR1].SetID(m_iMgzCnt * 100 + iSltCnt);
                      DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLD1].GetLotNo());
//                  }

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiLD1_Psh, ccFwd)) return false;
                  if(IO_GetX(xLD1_PusherOverLoad)) {
                      MoveActr(aiLD1_Psh, ccBwd);
                      EM_SetErr(eiLD1_PusherOverload);
                      Step.iCycle = 0;
                      return true ;
                  }
//                  DM.ShiftArrayData(riLD1,riPRB);
                  if(!IO_GetX(xLD1_PusherFw)) return false;
                  MoveActr(aiLD1_Psh, ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiLD1_Psh, ccBwd)) return false;
                  sTemp1 = IO_GetX(xPRB_1Pkg) ;
                  sTemp2 = IO_GetX(xPRB_2Pkg) ;
                  if( IO_GetX(xPRB_1Pkg) /*|| IO_GetX(xPRB_2Pkg)*/) return false ;
                                         //센서위치가 이상해서 적용 하면 계속 리턴됌.
//                  iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLD1].FindLastRow(csUnkwn) ;
//                  DM.ARAY[riPR1].SetID(m_iMgzCnt * 100 + iSltCnt);
//                  DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLD1].GetLotNo());
                  DM.ARAY[riLD1].SetStat(DM.ARAY[riLD1].FindLastRow(csUnkwn),0,csEmpty);

                  MoveMotr(miLD1_ZCmp , piLD1_ZWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLD1_ZCmp , piLD1_ZWork))return false ;
                  Step.iCycle = 0;
                  return true ;
    }

}

bool CLoader::CyclePlace ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiLD1_CycleTO);
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

        case  10: MoveMotr(miLD1_YCmp , piLD1_YWork) ;
                  IO_SetY(yLD1_MgzOutAC,true);
                  IO_SetY(yLD1_MgzInAC ,true); //미리 좀 돌려 놓는다.
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLD1_YCmp , piLD1_YWork)) return false ;
                  MoveMotr(miLD1_ZCmp , piLD1_ZPlaceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLD1_ZCmp , piLD1_ZPlaceFwd))return false ;
                  
                  if(IO_GetX(xLD1_MgzOutFull)) {
                      EM_SetErr(eiLD1_MgzFull);
                      Step.iCycle = 0;
                      return true;
                  }
                  IO_SetY(yLD1_MgzOutAC,false);
                  MoveMotr(miLD1_YCmp , piLD1_YPlace) ; //풀 에러 ㄱㄱ씽.
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLD1_YCmp , piLD1_YPlace))return false ;
                  MoveMotr(miLD1_ZCmp , piLD1_ZClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLD1_ZCmp , piLD1_ZClampOff)) return false ;
                  MoveActr(aiLD1_Cmp , ccFwd);
                  DM.ARAY[riLD1].SetStat(csNone) ;
                  if(m_iMgzCnt == OM.DevOptn.iLotEndMgzCnt) DM.ARAY[riLD1].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLD1_Cmp , ccFwd)) return false ;
                  IO_SetY(yLD1_MgzOutAC,true);
                  MoveMotr(miLD1_ZCmp , piLD1_ZPlaceBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLD1_ZCmp , piLD1_ZPlaceBwd)) return false ;
                  MoveMotr(miLD1_YCmp , piLD1_YWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLD1_YCmp , piLD1_YWork)) return false ;
                  MoveMotr(miLD1_ZCmp , piLD1_ZPickFwd);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miLD1_ZCmp , piLD1_ZPickFwd)) return false ;
                  IO_SetY(yLD1_MgzOutAC,false);
                  IO_SetY(yLD1_MgzInAC ,false); 
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
    if(!MT_GetStop(miLD1_YCmp)) return false ;
    if(!MT_GetStop(miLD1_ZCmp)) return false ;

    if(!AT_Done(aiLD1_Cmp   )) return false ;

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
