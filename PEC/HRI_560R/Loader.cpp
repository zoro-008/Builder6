//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loader.h"
//#include "Head.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "Rail.h"
#include "UserIni.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLoader LDR;

CLoader::CLoader(void)
{
    Init();
}
void CLoader::Init()
{
    m_sPartName = "Loader " ;
    m_iMgzEmptyCnt = 0;
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
    Close();
}
void CLoader::Close()
{
    Load(false);
}

bool CLoader::FindChip(int &r , int &c )
{
    DM.ARAY[riLDR].FindFrstRowCol(csUnkwn , r, c);
    //DM.ARAY[riLDR].FindFrstRowCol(csUnkwn , r, c);

    //r = DM.ARAY[riLDR].FindFrstRow(csUnkwn) ;
    //c = DM.ARAY[riLDR].FindFrstRow(csUnkwn) ;
    return (r > -1 && c > -1);
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dXPshPos = PM.GetValue(miLDR_XPsh , pvLDR_XPshWorkStt) + (c) * OM.DevInfo.dLDRColPitch ;
    double dXPckPos = PM.GetValue(miLDR_XPck , pvLDR_XPckWorkStt) + (c) * OM.DevInfo.dLDRColPitch ;
    double dYFedPos = PM.GetValue(miLDR_YFed , pvLDR_YFedWorkStt) + (r) * OM.DevInfo.dLDRRowPitch ;
//    double dZ2Pos = PM.GetValue(miLD2_Z , pvLD2_ZWorkStartPs) + (                    r  ) * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miLDR_XPsh){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLDR_XPshWait   : dPos = PM.GetValue (_iMotr , pvLDR_XPshWait    ); break ;
            case piLDR_XPshWorkStt: dPos = PM.GetValue (_iMotr , pvLDR_XPshWorkStt ); break ;
            case piLDR_XPshWork   : dPos = dXPshPos                                 ; break ;
        }
    }
    else if(_iMotr == miLDR_XPck){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLDR_XPckWait   : dPos = PM.GetValue (_iMotr , pvLDR_XPckWait    ); break ;
            case piLDR_XPckInsert : dPos = PM.GetValue (_iMotr , pvLDR_XPckInsert  ); break ;
            case piLDR_XPckWorkStt: dPos = PM.GetValue (_iMotr , pvLDR_XPckWorkStt ); break ;
            case piLDR_XPckWork   : dPos = dXPckPos                                 ; break ;
        }
    }
    else if(_iMotr == miLDR_YFed){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLDR_YFedWait   : dPos = PM.GetValue (_iMotr , pvLDR_YFedWait    ); break ;
            case piLDR_YFedPlace  : dPos = PM.GetValue (_iMotr , pvLDR_YFedPlace   ); break ;
            case piLDR_YFedWorkStt: dPos = PM.GetValue (_iMotr , pvLDR_YFedWorkStt ); break ;
            case piLDR_YFedWork   : dPos = dYFedPos                                 ; break ;
        }
    }
    else if(_iMotr == miLDR_ZPck){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piLDR_ZPckWait   : dPos = PM.GetValue (_iMotr , pvLDR_ZPckWait    ); break ;
            case piLDR_ZPckMove   : dPos = PM.GetValue (_iMotr , pvLDR_ZPckMove    ); break ;
            case piLDR_ZPckWork   : dPos = PM.GetValue (_iMotr , pvLDR_ZPckWork    ); break ;
            case piLDR_ZPckPlace  : dPos = PM.GetValue (_iMotr , pvLDR_ZPckPlace   ); break ;
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

//    bool isFwPusher      =  AT_Complete(aiLDR_Pusher , ccFwd);
    bool isFwPusher      = !AT_Complete(aiLDR_Pusher , ccBwd);

    bool isFwInCylinder  = AT_Complete(aiLDR_InMgz  , ccFwd);
    bool isFwOutCylinder = AT_Complete(aiLDR_OutMgz , ccFwd);
    bool isLDR_XPshWaitPs= MT_CmprPos(miLDR_XPsh , PM.GetValue(miLDR_XPsh , pvLDR_XPshWait))    && MT_GetStopInpos(miLDR_XPsh);
    bool isLDR_XPshWorkPs= MT_CmprPos(miLDR_XPsh , PM.GetValue(miLDR_XPsh , pvLDR_XPshWorkStt)) && MT_GetStopInpos(miLDR_XPsh);

    if(_iMotr == miLDR_XPsh){
        if( isFwPusher      ) { sMsg = "Pusher is not Bwd"       ; bRet = false ;}
        switch(_iPstnId) {
            default                :
            case piLDR_XPshWait    : break;
            case piLDR_XPshWork    : break;
            case piLDR_XPshWorkStt : break;
        }
    }
    else if(_iMotr == miLDR_YFed){
        if(isFwPusher       ) { sMsg = "Pusher is not Bwd"           ; bRet = false ;}
        switch(_iPstnId) {
                default                : break ;
                //UpStair
                case piLDR_YFedPlace   : if(!isLDR_XPshWaitPs)  { sMsg = MT_GetName(miLDR_XPsh)+" is Not Wait Position" ; bRet = false ;} break ;
        }
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

    bool isLDR_YFedMoving = !MT_GetStopInpos(miLDR_YFed);

    if(_iActr == aiLDR_Pusher){
        if(_bFwd) {
            if(isLDR_YFedMoving) { sMsg = "Loader MGZ Feeding Y is Moving" ; bRet = false ;}
        }
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

        case 10: if(IO_GetX(xLDR_MgzIn) && AT_Complete(aiLDR_InMgz,ccFwd)) { //TODO :: Pass 실린더 센서로 봐야 되는데 해놓으면 출장가게 생겻다.
                     EM_SetErr(eiLDR_InMgzDetected);
                     Step.iHome = 0 ;
                     return true ;
                 }
                 if(IO_GetX(xLDR_MgzOutStop) && AT_Complete(aiLDR_OutMgz,ccFwd)) {
                     EM_SetErr(eiLDR_OutMgzDetected);
                     Step.iHome = 0 ;
                     return true ;
                 }
                 AT_MoveCyl(aiLDR_Pusher , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiLDR_Pusher , ccBwd)) return false ;
                 MT_DoHome(miLDR_ZPck);
                 MT_DoHome(miLDR_XPsh);
                 AT_MoveCyl(aiLDR_InMgz   , ccBwd);
                 AT_MoveCyl(aiLDR_OutMgz  , ccBwd);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miLDR_ZPck))return false;
                 if(!MT_GetHomeEnd(miLDR_XPsh))return false;
                 if(!AT_MoveCyl(aiLDR_InMgz   , ccBwd))return false ;
                 if(!AT_MoveCyl(aiLDR_OutMgz  , ccBwd))return false ;
                 MT_DoHome(miLDR_XPck);
                 MT_DoHome(miLDR_YFed);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miLDR_XPck))return false;
                 if(!MT_GetHomeEnd(miLDR_YFed))return false;

                 MT_GoAbsMan(miLDR_XPsh , GetMotrPos(miLDR_XPsh , piLDR_XPshWait));
                 MT_GoAbsMan(miLDR_XPck , GetMotrPos(miLDR_XPck , piLDR_XPckWait));
                 MT_GoAbsMan(miLDR_YFed , GetMotrPos(miLDR_YFed , piLDR_YFedWait));
                 MT_GoAbsMan(miLDR_ZPck , GetMotrPos(miLDR_ZPck , piLDR_ZPckWait));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miLDR_XPsh , GetMotrPos(miLDR_XPsh , piLDR_XPshWait)))return false;
                 if(!MT_GoAbsMan(miLDR_XPck , GetMotrPos(miLDR_XPck , piLDR_XPckWait)))return false;
                 if(!MT_GoAbsMan(miLDR_YFed , GetMotrPos(miLDR_YFed , piLDR_YFedWait)))return false;
                 if(!MT_GoAbsMan(miLDR_ZPck , GetMotrPos(miLDR_ZPck , piLDR_ZPckWait)))return false;
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

        bool isChangeLot    =  GetLotNo()!="" && m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;
        bool isPreLotReady  =  isChangeLot && (!DM.ARAY[riPRB].GetCntExist() &&
                                               !DM.ARAY[riWR1].GetCntExist() &&
                                               !DM.ARAY[riWR2].GetCntExist() &&
                                               !DM.ARAY[riWR3].GetCntExist() &&
                                               !DM.ARAY[riPSB].GetCntExist() );
        bool isPrbReady     =  !IO_GetX(xPRB_Pkg) && !IO_GetX(xPRB_PkgIn) && DM.ARAY[riPRB].CheckAllStat(csNone ) ;

        //bool isCycleSupply  =  DM.ARAY[riLDR].CheckAllStat(csNone ) && IO_GetX(xLDR_MgzIn ) && (( m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt) || GetLotNo()!="") ;
        bool isCycleSupply  =  DM.ARAY[riLDR].CheckAllStat(csNone ) && IO_GetX(xLDR_MgzIn ) && (m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt); //|| GetLotNo()!="") ;
        //bool isCycleWork    =  DM.ARAY[riLDR].GetCntStat  (csUnkwn)  && ;
        bool isCyclePick    =  DM.ARAY[riLDR].GetCntStat  (csUnkwn) && isPrbReady && RAL.GetSeqStep() == ssNone ;
        bool isCycleOut     =  DM.ARAY[riLDR].CheckAllStat(csEmpty);
        bool isCycleEnd     =  DM.ARAY[riLDR].CheckAllStat(csNone ) && !IO_GetX(xLDR_MgzWork) ;

        //모르는 스트립에러.
        if(  DM.ARAY[riLDR].CheckAllStat(csNone) && IO_GetX(xLDR_MgzWork) ) EM_SetErr(eiLDR_MgzUnknown) ;
        if(  DM.ARAY[riLDR].CheckAllStat(csNone) && m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt && !IO_GetX(xLDR_MgzIn)) EM_SetErr(eiLDR_MgzEmpty) ;
        //카세트 사라짐.
        if( !DM.ARAY[riLDR].CheckAllStat(csNone) &&!IO_GetX(xLDR_MgzWork) ) EM_SetErr(eiLDR_MgzDispr  ) ;
        //카세트 OUT 매거진 풀
        if (((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || GetLotNo()!="") && IO_GetX(xLDR_MgzOut) && IO_GetX(xLDR_MgzOutStop)) EM_SetErr(eiLDR_MgzFull) ; //에러명 바꿈.

        bool test = EM_IsErr();

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut    Stt"); Step.iSeq = scOut    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default    :                      Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                       return false ;
        case scSupply: if(CycleSupply()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scOut   : if(CycleOut   ()){ Trace(m_sPartName.c_str(),"CycleOut    End");Step.iSeq = scIdle ;} return false ;
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

        case 10: //MoveActr(aiLDR_Clamp  , ccBwd);
                 //MoveActr(aiLDR_InMgz  , ccBwd);
                 //MoveActr(aiLDR_OutMgz , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MoveActr(aiLDR_Clamp  , ccBwd)) return false;
                 //if(!MoveActr(aiLDR_InMgz  , ccBwd)) return false;
                 //if(!MoveActr(aiLDR_OutMgz , ccBwd)) return false;
                 MoveActr(aiLDR_Pusher  , ccBwd);
                 MoveMotr(miLDR_ZPck , piLDR_ZPckWait);
                 //MoveActr(aiLDR_Rotator , ccFwd);
                 Step.iToStart ++;
                 return false;

        case 12: if(!MoveActr(aiLDR_Pusher  , ccBwd)) return false;
                 if(!MoveMotr(miLDR_ZPck  , piLDR_ZPckWait)) return false ;
                 //if(!MoveActr(aiLDR_Rotator , ccFwd)) return false;
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

        case 10: //MoveActr(aiLDR_InMgz  , ccBwd);
                 //MoveActr(aiLDR_OutMgz , ccBwd);
                 //MoveActr(aiLDR_Clamp  , ccFwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: //if(!MoveActr(aiLDR_Clamp  , ccFwd))return false;
                 //if(!MoveActr(aiLDR_InMgz  , ccBwd))return false;
                 //if(!MoveActr(aiLDR_OutMgz , ccBwd))return false;
                 //MoveActr(aiLDR_Pusher  , ccBwd);
                 //MoveActr(aiLDR_Rotator , ccFwd);

                 MoveActr(aiLDR_Pusher  , ccBwd);
                 MoveMotr(miLDR_ZPck , piLDR_ZPckWait);
                 Step.iToStop ++ ;
                 return false ;

        case 12: //if(!MoveActr(aiLDR_Pusher  , ccBwd))return false;
                 //if(!MoveActr(aiLDR_Rotator , ccFwd))return false;
                 if(!MoveActr(aiLDR_Pusher  , ccBwd))return false ;
                 if(!MoveMotr(miLDR_ZPck , piLDR_ZPckWait)) return false ;
                 Step.iToStop = 0;
                 return true;
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

    String    sLotNo ;
    TDateTime tTemp  ;
    String    sDate  ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10:if(IO_GetX(xLDR_MgzIn) && AT_Complete(aiLDR_InMgz,ccFwd)) {
                     EM_SetErr(eiLDR_InMgzDetected);
                     Step.iCycle = 0 ;
                     return true ;
                  }

                  MoveActr(aiLDR_InMgz  , ccBwd);
                  MoveActr(aiLDR_Pusher , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_InMgz  , ccBwd)) return false;
                  if(!MoveActr(aiLDR_Pusher , ccBwd)) return false;
                  MoveMotr(miLDR_YFed , piLDR_YFedWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_YFed , piLDR_YFedWait)) return false;
                  MoveActr(aiLDR_InMgz , ccFwd);
//                  m_tmTemp.Clear(); //빼고 실린더 딜레이로 넣어주세요.
                  Step.iCycle ++ ;
                  return false ;

        case  13: if(!MoveActr(aiLDR_InMgz , ccFwd)) return false;
//                  if(!m_tmTemp.OnDelay(true, OM.CmnOptn.dLDRInDeley))return false;
                  if(!IO_GetX(xLDR_MgzInStop)){
                      EM_SetErr(eiLDR_MgzDispr);
                      Step.iCycle = 0;
                      return true;
                  }
//                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: MoveMotr(miLDR_YFed , piLDR_YFedWorkStt);
                  MoveActr(aiLDR_InMgz , ccBwd); //시간 걸려서 확인 안함.
                  if(m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt || m_iMgzCnt == 0){
                      sLotNo = PopLotNo() ;
                      LT.LotOpen(sLotNo , OM.m_sCrntDev );

                      sTemp = m_iMgzCnt ;
                      Trace("m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt , m_iMgzCnt = ",sTemp.c_str());
                      tTemp.Val = LT.LotInfo.dStartTime ;
                      sDate = tTemp.FormatString("hh_nn_ss") ;
                      LT.AddDayInfoLotCnt(1);
                      m_iMgzCnt = 1;
                      DM.ARAY[riLDR].SetLotNo(sLotNo) ;
                      DM.ARAY[riLDR].SetID(m_iMgzCnt*1000) ;

                      if(!OM.CmnOptn.bVs1Skip ||
                         !OM.CmnOptn.bVs2Skip ||
                         !OM.CmnOptn.bVs3Skip ||
                         !OM.CmnOptn.bVsMrkSkip) VC.SendLotStart(true);
                  }

                  else {
                      tTemp.Val = LT.LotInfo.dStartTime ;
                      sDate = tTemp.FormatString("hh_nn_ss") ;
                      m_iMgzCnt++;


//                      sLotNo = "(" + sDate + ")" + LT.LotInfo.sLotNo ;
                      sLotNo = LT.LotInfo.sLotNo ;
                      DM.ARAY[riLDR].SetLotNo(sLotNo) ;
                      DM.ARAY[riLDR].SetID(m_iMgzCnt*1000) ;

                  }
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miLDR_YFed , piLDR_YFedWorkStt)) return false ;
//                  if(!MoveActr(aiLDR_InMgz , ccBwd)) return false ;
                  DM.ARAY[riLDR].SetStat(csUnkwn);
                  iMgzCnt = m_iMgzCnt;
                  Step.iCycle = 0;
                  return true ;
    }
}
//--------------------------------------------------------------------------
bool CLoader::CyclePick()
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

    int iSltCnt;
    int iRowSltCnt;
    int iRow , iCol;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(IO_GetX(xLDR_MgzOutStop)) {
                     EM_SetErr(eiLDR_OutMgzDetected);
                     Step.iHome = 0 ;
                     return true ;
                  }

                  MoveActr(aiLDR_Pusher , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_Pusher , ccBwd)) return false ;
                  MoveMotr(miLDR_YFed , piLDR_YFedWork);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miLDR_YFed , piLDR_YFedWork)) return false ;
                  Step.iCycle++;
                  return false;

        case  13: MoveActr(aiLDR_Rotator , ccFwd);
                  MoveActr(aiLDR_Clamp   , ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiLDR_Rotator , ccFwd))return false;
                  if(!MoveActr(aiLDR_Clamp   , ccBwd))return false;
//                  MoveMotr(miLDR_YFed , piLDR_YFedWork);
                  MoveMotr(miLDR_XPsh , piLDR_XPshWork);
                  MoveMotr(miLDR_XPck , piLDR_XPckWork);
                  MoveMotr(miLDR_ZPck , piLDR_ZPckWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLDR_XPsh , piLDR_XPshWork))return false ;
                  if(!MoveMotr(miLDR_XPck , piLDR_XPckWork))return false ;
                  if(!MoveMotr(miLDR_ZPck , piLDR_ZPckWork))return false ;
                  MoveActr(aiLDR_Pusher , ccFwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiLDR_Pusher , ccFwd))return false;
                  if(!IO_GetX(xLDR_PkgDetect)){
                      MoveActr(aiLDR_Pusher , ccBwd);
                      FindChip(iRow , iCol);
                      m_iMgzEmptyCnt++;
                      DM.ARAY[riLDR].SetStat(iRow , iCol , csEmpty);
                      if(OM.CmnOptn.bUseLDREmpty && m_iMgzEmptyCnt >= OM.CmnOptn.iLDREmptyCnt){
                          DM.ARAY[riLDR].SetStat(csEmpty);
                          m_iMgzEmptyCnt = 0;
                      }
                      Step.iCycle = 0;
                      return true;
                  }
                  MoveActr(aiLDR_Clamp , ccFwd);
                  m_iMgzEmptyCnt = 0;
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiLDR_Clamp , ccFwd))return false;
                  MoveMotr(miLDR_ZPck , piLDR_ZPckWait);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miLDR_ZPck , piLDR_ZPckWait))return false;
                  MoveActr(aiLDR_Rotator , ccBwd);
                  MoveActr(aiLDR_Pusher  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  19: if(!MoveActr(aiLDR_Rotator , ccBwd))return false;
                  //if(!MoveActr(aiLDR_Pusher  , ccBwd))return false; 오래 걸려서 나중에 확인
                  MoveMotr(miLDR_ZPck , piLDR_ZPckMove);
                  Step.iCycle++;
                  return false;

        case  20: if(!MoveMotr(miLDR_ZPck , piLDR_ZPckMove))return false;
                  MoveMotr(miLDR_XPck , piLDR_XPckInsert);
                  Step.iCycle++;
                  return false;

        case  21: if(!MoveMotr(miLDR_XPck , piLDR_XPckInsert))return false;
                  MoveMotr(miLDR_ZPck , piLDR_ZPckPlace);
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveMotr(miLDR_ZPck , piLDR_ZPckPlace))return false;
                  MoveActr(aiLDR_Clamp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  23: if(!MoveActr(aiLDR_Clamp , ccBwd))return false;
                  FindChip(iRow , iCol);
                  iSltCnt = (OM.DevInfo.iLDRColCnt * OM.DevInfo.iLDRRowCnt) - DM.ARAY[riLDR].GetCntStat(csUnkwn);
                  DM.ARAY[riPRB].SetID(DM.ARAY[riLDR].GetID().ToIntDef(999000) + iSltCnt);
                  DM.ARAY[riPRB].SetLotNo(DM.ARAY[riLDR].GetLotNo());
                  DM.ARAY[riLDR].SetStat(iRow, iCol, csEmpty) ;
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  Step.iCycle++;
                  return false;

        case  24: if(!MoveActr(aiLDR_Pusher  , ccBwd))return false;
                  if(DM.ARAY[riLDR].GetCntStat(csUnkwn)) MoveMotr(miLDR_XPck , piLDR_XPckWork);
                  else                                   MoveMotr(miLDR_XPck , piLDR_XPckWait);

                  if(DM.ARAY[riLDR].GetCntStat(csUnkwn)) MoveMotr(miLDR_XPsh , piLDR_XPshWork);
                  else                                   MoveMotr(miLDR_XPsh , piLDR_XPshWait);
                  Step.iCycle++;
                  return false ;

        case  25: if(!MT_GetStopInpos(miLDR_XPck)) return false ;
                  if(!MT_GetStopInpos(miLDR_XPsh)) return false ;
//                  if(!MoveActr(aiLDR_Pusher  , ccBwd))return false;
                  MoveActr(aiLDR_Rotator , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  26: if(!MoveActr(aiLDR_Rotator , ccFwd)) return false ;
                  Step.iCycle = 0;
                  return true;
    }

}


bool CLoader::CycleOut()
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

        case  10: if(IO_GetX(xLDR_MgzOutStop)) {
                     EM_SetErr(eiLDR_OutMgzDetected);
                     Step.iCycle = 0 ;
                     return true ;
                  }

                  if(IO_GetX(xLDR_MgzOut)){
                      EM_SetErr(eiLDR_MgzFull);
                      Step.iCycle = 0;
                      return true;
                  }

                  MoveActr(aiLDR_Pusher , ccBwd);
                  MoveActr(aiLDR_OutMgz , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_OutMgz , ccBwd))return false;
                  if(!MoveActr(aiLDR_Pusher , ccBwd))return false;
                  MoveMotr(miLDR_XPsh , piLDR_XPshWait);
                  MoveMotr(miLDR_ZPck , piLDR_ZPckWait);
                  MoveMotr(miLDR_XPck , piLDR_XPckWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_XPsh , piLDR_XPshWait))return false;
                  if(!MoveMotr(miLDR_ZPck , piLDR_ZPckWait))return false;
                  if(!MoveMotr(miLDR_XPck , piLDR_XPckWait))return false;
                  MoveMotr(miLDR_YFed , piLDR_YFedPlace);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miLDR_YFed , piLDR_YFedPlace))return false;
                  if(!IO_GetX(xLDR_MgzOutStop)){
                      EM_SetErr(eiLDR_MgzDispr);
                      Step.iCycle = 0;
                      return true;
                  }
                  MoveMotr(miLDR_YFed , piLDR_YFedWait);
                  Step.iCycle++;
                  return false;

        case  14: MoveActr(aiLDR_OutMgz , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiLDR_OutMgz , ccFwd))return false;
                  MoveActr(aiLDR_OutMgz , ccBwd);
                  DM.ARAY[riLDR].SetStat(csNone) ;
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiLDR_OutMgz , ccBwd))return false;
                  Step.iCycle = 0;
                  return true;
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
    if(!MT_GetStop(miLDR_XPck)) return false ;
    if(!MT_GetStop(miLDR_XPsh)) return false ;
    if(!MT_GetStop(miLDR_YFed)) return false ;
    if(!MT_GetStop(miLDR_ZPck)) return false ;

    if(!AT_Done(aiLDR_Clamp  )) return false ;
    if(!AT_Done(aiLDR_InMgz  )) return false ;
    if(!AT_Done(aiLDR_OutMgz )) return false ;
    if(!AT_Done(aiLDR_Pusher )) return false ;
    if(!AT_Done(aiLDR_Rotator)) return false ;

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
        UserINI.Load(sPath, "Member" , "iMgzCnt  " , iMgzCnt   );

    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
        UserINI.Save(sPath, "Member" , "iMgzCnt  " , iMgzCnt   );
    }
}
//---------------------------------------------------------------------------
