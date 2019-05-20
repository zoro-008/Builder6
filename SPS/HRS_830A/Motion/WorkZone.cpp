//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "WorkZone.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CWorkZone WRK;

CWorkZone::CWorkZone(void)
{

}

CWorkZone::~CWorkZone (void)
{

}

void CWorkZone::Init()
{
    m_sPartName = "WorkZone " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CWorkZone::Close()
{
    Load(false);
}

void CWorkZone::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CWorkZone::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}



bool CWorkZone::FindChip(int &r , int &c )
{
    int iUnkwnC = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;
    int iMaskC  = DM.ARAY[riWRK].FindLastCol(csMask ) ;
    int iWaitC  = DM.ARAY[riWRK].FindLastCol(csWait ) ;
    int iMaxC = -1 ;

    if(iMaxC < iUnkwnC) iMaxC = iUnkwnC ;
    if(iMaxC < iMaskC ) iMaxC = iMaskC  ;
    if(iMaxC < iWaitC ) iMaxC = iWaitC  ;

    c = iMaxC ;
    r = 0     ;







    //스트립 상태
    //None    = 스트립 없는 상태.
    //Unkwn   = 스트립 들어와서 솔더 검사할 차례
    //Mask    = 솔더 검사 다 하고 자제 내려 놓을 상태.
    //Wait    = 자제 내려 놓고 안착 검사 할 차례
    //Work    = 안착 검사 완료한 Good시료.
    //Fail    = 안착 검사시에 불량시료들. 뒤에서 리웍 작업하는 것들.

    return (c > -1 && c > -1);
}

double CWorkZone::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miWRK_YVsn){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_YVsnWait    : dPos = PM.GetValue (_iMotr , pvWRK_YVsnWait   ); break ;
            case piWRK_YVsnWork    : dPos = PM.GetValue (_iMotr , pvWRK_YVsnWork   ); break ;
        }
    }
    if(_iMotr == miWRK_XStp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_XStpWait    : dPos = PM.GetValue (_iMotr , pvWRK_XStpWait   ); break ;
            case piWRK_XStpWork    : dPos = PM.GetValue (_iMotr , pvWRK_XStpWork   ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CWorkZone::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //인덱스 핑거 올라와 있는지.
    bool isStprUp   = !AT_Complete(aiWRK_StpUpDn , ccBwd) ;
    bool isLsrWork  = IO_GetY(yRMI_Marking);

    //모터 -방향으로 가는지.

    if(_iMotr == miWRK_XStp){
        if(isStprUp ) {sMsg = "Work Zone Stopper is Up"; bRet = false ;}
        switch(_iPstnId) {
            default                :  break ;
            case piWRK_XStpWait    :  break ;
            case piWRK_XStpWork    :  break ;
        }
    }

    if(_iMotr == miWRK_YVsn){
        if(isLsrWork ) {sMsg = "Laser is Working"; bRet = false ;}
        switch(_iPstnId) {
            default                 : break ;
            case piWRK_YVsnWait     : break ;
            case piWRK_YVsnWork     : break ;
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CWorkZone::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miWRK_XStp);

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiWRK_StpUpDn) {
        //if(_bFwd){
        if(isXMoving) { sMsg = AnsiString(MT_GetName(miWRK_XStp))+" is Moving" ; bRet = false ;}
        //}
    }
    if(_iActr == aiWRK_LftFwBw) {
        //if(_bFwd){
        if(isXMoving) { sMsg = AnsiString(MT_GetName(miWRK_XStp))+" is Moving" ; bRet = false ;}
        //}
    }
    if(_iActr == aiWRK_AlnFwBw) {
        //if(_bFwd){
        if(isXMoving) { sMsg = AnsiString(MT_GetName(miWRK_XStp))+" is Moving" ; bRet = false ;}
        //}
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CWorkZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CWorkZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWorkZone::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiWRK_HomeTo);
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

        case 10: AT_MoveCyl(aiWRK_AlnFwBw  ,ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_AlnFwBw  ,ccBwd)) return false ;
                 //AT_MoveCyl(aiWRK_StpUpDn  ,ccBwd);
                 AT_MoveCyl(aiWRK_LftFwBw  ,ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 12: //if(!AT_MoveCyl(aiWRK_StpUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWRK_LftFwBw  ,ccBwd)) return false ;
                 MT_SetHomeEnd(miWRK_XStp , false);
                 MT_SetHomeEnd(miWRK_YVsn , false);
                 MT_DoHome(miWRK_XStp);
                 MT_DoHome(miWRK_YVsn);
                 Step.iHome++;
                 return false;

        case 13: if(!MT_GetHomeEnd(miWRK_XStp)) return false;
                 if(!MT_GetHomeEnd(miWRK_YVsn)) return false;
                 MT_GoAbsMan(miWRK_XStp, PM.GetValue(miWRK_XStp , pvWRK_XStpWait));
                 MT_GoAbsMan(miWRK_YVsn, PM.GetValue(miWRK_YVsn , pvWRK_YVsnWait));
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWRK_XStp, PM.GetValue(miWRK_XStp , pvWRK_XStpWait))) return false ;
                 if(!MT_GoAbsMan(miWRK_YVsn, PM.GetValue(miWRK_YVsn , pvWRK_YVsnWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CWorkZone::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if (IO_GetX(xWRK_IdxOvld ) ) EM_SetErr(eiWRK_IdxOvld    );

    if (Step.iSeq == 0) {
        if(Stat.bReqStop )return false ;
        bool bWorkPos     = GetMotrPos(miWRK_XIdx,piWRK_XIdxWork) == MT_GetCmdPos(miWRK_XIdx) ;
        bool bWaitPos     = GetMotrPos(miWRK_XIdx,piWRK_XIdxWait) == MT_GetCmdPos(miWRK_XIdx) ;

        bool isCycleBind  = DM.ARAY[riWRK].CheckAllStat(csNone)  && !bWaitPos ;
        bool isCycleWork  = DM.ARAY[riWRK].GetCntStat  (csUnkwn) && !bWorkPos ;
        bool isCycleOut   =
        bool isCycleEnd   = DM.ARAY[riWRK].CheckAllStat(csNone) ;

        //Unknown Strip
        //if (IO_GetX(xWRK_Detect) && !OM.MstOptn.bDryRun && DM.ARAY[riWRK].CheckAllStat(csNone)) EM_SetErr(eiWRK_Unknown) ;

        //Dissapear Strip
        //if (isCycleOut && !OM.MstOptn.bDryRun) {
        //    if (!DM.ARAY[riWRK].CheckAllStat(csNone) && !IO_GetX(xWRK_Detect) ) EM_SetErr(eiWRK_Dispr)   ;
        //}

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleReady) {Trace(m_sPartName.c_str(),"CycleReady Stt"); Step.iSeq = scReady ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleMove ) {Trace(m_sPartName.c_str(),"CycleMove  Stt"); Step.iSeq = scMove  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                   Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                     return false ;
        case scReady : if(CycleReady()){ Trace(m_sPartName.c_str(),"CycleReady End");Step.iSeq = scIdle ;} return false ;
        case scMove  : if(CycleMove ()){ Trace(m_sPartName.c_str(),"CycleMove  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CWorkZone::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CWorkZone::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CWorkZone::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str());

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    bool bExstUnKnown = false ;

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: bExstUnKnown = DM.ARAY[riWRK].GetCntStat(csUnkwn)>0 ;
                 IO_SetY(yWRK_RalVac,bExstUnKnown);
                 //MoveActr(aiWRK_CmpDnUp , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 11: //if(!MoveActr(aiWRK_CmpDnUp , ccFwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWorkZone::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiWRK_ToStopTO);

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

        case 10: MoveActr(aiWRK_IdxUpDn , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiWRK_IdxUpDn , ccBwd))return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//중간에 스트립 빠지거나 하면 레디 위치로 복귀.
bool CWorkZone::CycleReady()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiWRK_CycleTO);
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

        case  10: IO_SetY(yWRK_RalVac,false);
                  MoveActr(aiWRK_IdxUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRK_IdxUpDn , ccBwd)) return false ;
                  MoveMotr(miWRK_XIdx , piWRK_XIdxWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XIdx , piWRK_XIdxWait)) return false ;
                  Step.iCycle=0;
                  return true;
    }
}



//작업 위치로 이동.
bool CWorkZone::CycleMove()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiWRK_CycleTO);
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

        case  10: IO_SetY(yWRK_RalVac,false);
                  MoveActr(aiWRK_IdxUpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRK_IdxUpDn , ccFwd)) return false ;
                  MoveMotr(miWRK_XIdx , piWRK_XIdxWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XIdx , piWRK_XIdxWork)) return false ;
                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep() + 1); //워크 시작 트리거.
                  IO_SetY(yWRK_RalVac,true);

                  //마지막 작업 포지션 아니면 그자리에서 끝..
                  if(DM.ARAY[riWRK].GetStep() <= OM.DevInfo.iColGrCnt){
                      Step.iCycle=0;
                      return true;
                  }

                  //마지막 위치면 대기 포지션으로 이동.
                  MoveMotr(miWRK_XIdx , piWRK_XIdxWait);
                  MoveActr(aiWRK_IdxUpDn , ccBwd);


        case  13: if(!MoveActr(aiWRK_IdxUpDn , ccBwd         )) return false ;
                  if(!MoveMotr(miWRK_XIdx    , piWRK_XIdxWait)) return false ;
                  Step.iCycle=0;
                  return true;
    }
}



void CWorkZone::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CWorkZone::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CWorkZone::CheckStop()
{
    if(!MT_GetStop(miWRK_XIdx)) return false ;

    if(!AT_Done(aiWRK_IdxUpDn)) return false ;

    return true ;
}

void CWorkZone::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuff.INI";

    //if ( _bLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );

    //else {
    //    UserINI.ClearFile(sPath) ;
    //    UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    //}

}
//---------------------------------------------------------------------------
