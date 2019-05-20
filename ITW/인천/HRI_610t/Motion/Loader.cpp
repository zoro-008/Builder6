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

}

CLoader::~CLoader (void)
{

}

void CLoader::Init()
{
    m_sPartName = "Loader " ;
    Reset();
    Load(true);

    InitCycleName();

    m_iEmptyStrpCnt = 0;
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
    return (r > -1 );
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dZ1Pos = PM.GetValue(miLDR_ZClmp , pvLDR_ZClmpWorkStart) + (OM.DevInfo.iCsSlCnt-r-1) * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miLDR_ZClmp){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piLDR_ZClmpWait      : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpWait      ); break ;
            case piLDR_ZClmpPickFwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpPickFwd   ); break ;
            case piLDR_ZClmpClampOn   : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpClampOn   ); break ;
            case piLDR_ZClmpPickBwd   : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpPickBwd   ); break ;
            case piLDR_ZClmpWorkStart : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpWorkStart ); break ;
            case piLDR_ZClmpPlaceFwd  : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpPlaceFwd  ); break ;
            case piLDR_ZClmpClampOff  : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpClampOff  ); break ;
            case piLDR_ZClmpPlaceBwd  : dPos = PM.GetValue (_iMotr , pvLDR_ZClmpPlaceBwd  ); break ;
            case piLDR_ZClmpWork      : dPos = dZ1Pos                                     ; break ;
        }
    }

    else if(_iMotr == miLDR_YClmp){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos  (_iMotr                      ); break ;
            case piLDR_YClmpWait      : dPos = PM.GetValue   (_iMotr , pvLDR_YClmpWait    ); break ;
            case piLDR_YClmpPick      : dPos = PM.GetValue   (_iMotr , pvLDR_YClmpPick    ); break ;
            case piLDR_YClmpWork      : dPos = PM.GetValue   (_iMotr , pvLDR_YClmpWork    ); break ;
            case piLDR_YClmpPlace     : dPos = PM.GetValue   (_iMotr , pvLDR_YClmpPlace   ); break ;
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

    bool isFwPusher      = AT_GetCmd(aiLDR_PshrFwBw)==ccFwd; //IO_GetX(xLD1_PusherFw) || !IO_GetX(xLD1_PusherBw);
    bool isRailGateCnsr  = IO_GetX(xPRB_PkgIn1)  ;
    bool isClampYFwd     = MT_CmprPos(miLDR_YClmp , GetMotrPos(miLDR_YClmp , piLDR_YClmpPick)) || MT_CmprPos(miLDR_YClmp , GetMotrPos(miLDR_YClmp , piLDR_YClmpPlace));
    bool isClampZUpstair = MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpPickBwd)  || MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpPickFwd)  || MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpClampOn)   ;
    bool isClampZCenter  = MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpWait)     || MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpWork)     || MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpWorkStart) ;
    bool isClampZDnstair = MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpPlaceBwd) || MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpPlaceFwd) || MT_CmprPos(miLDR_ZClmp , piLDR_ZClmpClampOff)  ;


    if(_iMotr == miLDR_ZClmp){
             if(isFwPusher    ) { sMsg = "Pusher Ac Motor is not Bwd" ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg = "Strip is feeding now      " ; bRet = false ;}
        switch(_iPstnId) {
            default                   :                                                       { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            //UpStair
            case piLDR_ZClmpPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            case piLDR_ZClmpClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            case piLDR_ZClmpPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            //Middle
            case piLDR_ZClmpWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            case piLDR_ZClmpWorkStart : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            case piLDR_ZClmpWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            //DnStatir
            case piLDR_ZClmpPlaceFwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            case piLDR_ZClmpClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
            case piLDR_ZClmpPlaceBwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Fwd" ; bRet = false ;} break ;
        }
    }

    else if(_iMotr == miLDR_YClmp){
        if(isFwPusher) return false ;
        else {
            switch(_iPstnId) {
                default               :                    {sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;}  break ;                                                                                                      break ;
                case piLDR_YClmpWait  :                                                                                                        break ;
                case piLDR_YClmpWork  :                                                                                                        break ;
                case piLDR_YClmpPick  : if(isClampZCenter) {sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Middle Position" ; bRet = false ;} break ;
                case piLDR_YClmpPlace : if(isClampZCenter) {sMsg = AnsiString(MT_GetName(miLDR_YClmp))+" is Middle Position" ; bRet = false ;} break ;
            }
        }
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
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

    bool bLDR_YClmpMoving = !MT_GetStop(miLDR_YClmp) ;
    bool bLDR_ZClmpMoving = !MT_GetStop(miLDR_ZClmp) ;

    if(_iActr == aiLDR_ClmpUpDn){
        if(bLDR_YClmpMoving) {sMsg = AnsiString(MT_GetName(miLDR_YClmp)) + "is Moving!"; bRet = false ;}
        if(bLDR_ZClmpMoving) {sMsg = AnsiString(AT_GetName(miLDR_ZClmp)) + "is Moving!"; bRet = false ;}
    }
    else if(_iActr == aiLDR_PshrFwBw){
        if(bLDR_YClmpMoving) {sMsg = AnsiString(MT_GetName(miLDR_YClmp)) + "is Moving!"; bRet = false ;}
        if(bLDR_ZClmpMoving) {sMsg = AnsiString(AT_GetName(miLDR_ZClmp)) + "is Moving!"; bRet = false ;}
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }


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

        case 10: MoveActr(aiLDR_PshrFwBw, ccBwd);
                 IO_SetY(yLDR_MgzInAC ,false);
                 IO_SetY(yLDR_MgzOutAC,false);
                 Step.iHome++;
                 return false ;

        case 11: if(!IO_GetX(xLDR_PusherBw))return false ;
                 MT_SetHomeEnd(miLDR_ZClmp , true) ;
                 //MT_ClearPos(miLDR_ZClmp);
                 MT_GoIncMan(miLDR_ZClmp,-10); //12mm
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetStopInpos(miLDR_ZClmp))return false;
                 MT_DoHome(miLDR_YClmp);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miLDR_YClmp))return false ;
                 MT_DoHome(miLDR_ZClmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miLDR_ZClmp))return false ;
                 MT_GoAbsMan(miLDR_YClmp, GetMotrPos(miLDR_YClmp,piLDR_YClmpWait));
                 MT_GoAbsMan(miLDR_ZClmp, GetMotrPos(miLDR_ZClmp,piLDR_ZClmpWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miLDR_YClmp, GetMotrPos(miLDR_YClmp,piLDR_YClmpWait)))return false ;
                 if(!MT_GoAbsMan(miLDR_ZClmp, GetMotrPos(miLDR_ZClmp,piLDR_ZClmpWait)))return false ;
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
        if(  DM.ARAY[riLDR].CheckAllStat(csNone) && (  IO_GetX(xLDR_MgzDetect1) ||  IO_GetX(xLDR_MgzDetect2)))
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riLDR].GetName().c_str());//"Loader") ;
        //카세트 사라짐.
        if( !DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2)))
            EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riLDR].GetName().c_str());//"Loader") ;
        //카세트 OUT 매거진 풀


        if (((m_iMgzCnt < OM.DevOptn.iLotEndMgzCnt)  || LTQ.GetLotCnt()!=0)&& Stat.bChckSplyEmpty && !IO_GetX(xLDR_MgzIn) && !OM.CmnOptn.bLoadingStop) EM_SetErr(eiLDR_NeedMgz) ;

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
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  11: r1 = m_tmDelay.OnDelay(true , 5000) ;
                  r2 = IO_GetX(xLDR_MgzIn)           ;
                  if(!r1&&!r2) return false ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmDelay.OnDelay(true ,  200)) return false ;
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

    String sTempLot ;
    int    iTemp ;
    TDateTime  tCrntTime = Now();

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

        case  10: MoveActr(aiLDR_ClmpUpDn , ccFwd         );
                  MoveMotr(miLDR_YClmp    , piLDR_YClmpWork);
                  MoveActr(aiLDR_PshrFwBw , ccBwd  );
                  IO_SetY (yLDR_MgzInAC ,true );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiLDR_ClmpUpDn, ccFwd         )) return false ;
                  if(!MoveMotr(miLDR_YClmp   , piLDR_YClmpWork)) return false ;
                  if(!MoveActr(aiLDR_PshrFwBw, ccBwd         )) return false ;
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpPickFwd))return false ;
                  MoveMotr(miLDR_YClmp , piLDR_YClmpPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YClmp , piLDR_YClmpPick))return false ;
                  if(!IO_GetX(xLDR_MgzIn) && !OM.MstOptn.bDryRun)return false ;
                  IO_SetY(yLDR_MgzInAC,false);
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpClampOn)) return false ;
                  DM.ARAY[riLDR].SetStat(csUnkwn) ;

                  if(m_iMgzCnt >= OM.DevOptn.iLotEndMgzCnt) {
                      m_iMgzCnt = 0;
                      LTQ.PopLotNo();

                  }
                  sTempLot = LTQ.GetCrntLotNo() ; //연속된 랏 뒤쪽에 시간 추가..
                  if(sTempLot.Pos("<time>")){
                      iTemp = sTempLot.Pos("<time>");
                      sTempLot.Delete(iTemp ,String("<time>").Length() );
                      sTempLot += tCrntTime.FormatString("_hhnnss");
                  }

                  DM.ARAY[riLDR].SetLotNo(sTempLot) ;
                  DM.ARAY[riLDR].SetID(m_iMgzCnt * 100 ) ;
                  m_iMgzCnt++;
                  Step.iCycle++;
                  return false ;


        case  15: MoveActr(aiLDR_ClmpUpDn , ccBwd);
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiLDR_ClmpUpDn , ccBwd)) return false ;
                  IO_SetY(yLDR_MgzInAC,true);
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpPickBwd)) return false ;
                  //if(OM.MstOptn.bLdr_YHome) { m_bIsHome = true ; MT_DoHome(miLDR_YClmp); }
                  Step.iCycle++;
                  return false ;

        case  18: //if(OM.MstOptn.bLdr_YHome && !MT_GetHomeEnd(miLDR_YClmp) ) return false ;
                  //m_bIsHome = false ;
                  MoveMotr(miLDR_YClmp , piLDR_YClmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveMotr(miLDR_YClmp , piLDR_YClmpWork)) return false ;
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

    static bool bPkgExist = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miLDR_YClmp   , piLDR_YClmpWork);
                  MoveActr(aiLDR_PshrFwBw, ccBwd         );
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YClmp   , piLDR_YClmpWork)) return false ;
                  if(!MoveActr(aiLDR_PshrFwBw, ccBwd         )) return false ;
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpWork)) return false ;
                  MoveActr(aiLDR_PshrFwBw, ccFwd);

                  bPkgExist = false ;

                  Step.iCycle++;
                  return false;

        case  13: if(IO_GetX(xLDR_PusherOvld)) {
                      MoveActr(aiLDR_PshrFwBw, ccBwd);
                      EM_SetErr(eiLDR_PshrOvld);
                      Step.iCycle = 0;
                      return true ;
                  }

                  if(IO_GetX(xPRB_PkgIn1)) {
                      bPkgExist = true ;
                  }
                  if(!MoveActr(aiLDR_PshrFwBw, ccFwd)) return false;
                  if(bPkgExist){
                      iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindLastRow(csUnkwn) - 1;
                      DM.ARAY[riPRB].SetID((DM.ARAY[riLDR].GetID().ToIntDef(0) / 100 * 100) + iSltCnt);
                      DM.ARAY[riPRB].SetLotNo(DM.ARAY[riLDR].GetLotNo());
                      m_iEmptyStrpCnt = 0;
                  }
                  else{
                      if(OM.DevOptn.bEmptyStrpPlace){
                          m_iEmptyStrpCnt++;
                          if(m_iEmptyStrpCnt >= OM.DevOptn.iEmptyStrpCnt){
                              m_iEmptyStrpCnt = 0;
                              DM.ARAY[riLDR].SetStat(csEmpty);
                          }
                      }
                      else {
                          m_iEmptyStrpCnt = 0;
                      }
                  }  //연속으로 빈 슬롯 일 때 배출 하는 파트 추가....JH








//                  DM.ShiftArrayData(riLDR,riPRB);
                  MoveActr(aiLDR_PshrFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiLDR_PshrFwBw, ccBwd)) return false;
                  if( IO_GetX(xPRB_PkgIn1)) return false ;
                  DM.ARAY[riLDR].SetStat(DM.ARAY[riLDR].FindLastRow(csUnkwn),0,csEmpty);

                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpWork))return false ;

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

        case  10: MoveMotr(miLDR_YClmp , piLDR_YClmpWork) ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  IO_SetY(yLDR_MgzInAC ,true); //미리 좀 돌려 놓는다.
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_YClmp , piLDR_YClmpWork)) return false ;
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpPlaceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpPlaceFwd))return false ;
                  
                  if(IO_GetX(xLDR_MgzOutFull)) {
                      EM_SetErr(eiLDR_FullMgz);
                      Step.iCycle = 0;
                      return true;
                  }
                  IO_SetY(yLDR_MgzOutAC,false);
                  MoveMotr(miLDR_YClmp , piLDR_YClmpPlace) ; //풀 에러 ㄱㄱ씽.
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miLDR_YClmp , piLDR_YClmpPlace))return false ;
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpClampOff)) return false ;
                  MoveActr(aiLDR_ClmpUpDn , ccFwd);
                  DM.ARAY[riLDR].ClearMap();
                  if(m_iMgzCnt == OM.DevOptn.iLotEndMgzCnt) DM.ARAY[riLDR].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiLDR_ClmpUpDn , ccFwd)) return false ;
                  IO_SetY(yLDR_MgzOutAC,true);
                  MoveMotr(miLDR_ZClmp , piLDR_ZClmpPlaceBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miLDR_ZClmp , piLDR_ZClmpPlaceBwd)) return false ;
                  MoveMotr(miLDR_YClmp , piLDR_YClmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miLDR_YClmp , piLDR_YClmpWork)) return false ;

                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  18: if(!m_tmDelay.OnDelay(true,3000)) return false;

                  IO_SetY(yLDR_MgzOutAC,false);
                  IO_SetY(yLDR_MgzInAC ,false);

                  m_iEmptyStrpCnt = 0;

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
    if(!MT_GetStop(miLDR_YClmp)) return false ;
    if(!MT_GetStop(miLDR_ZClmp)) return false ;

    if(!AT_Done(aiLDR_ClmpUpDn)) return false ;
    if(!AT_Done(aiLDR_PshrFwBw)) return false ;

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
