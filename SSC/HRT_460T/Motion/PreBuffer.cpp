//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuffer.h"
//---------------------------------------------------------------------------
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
CPreBuffer PRB;

CPreBuffer::CPreBuffer()
{

}

CPreBuffer::~CPreBuffer (void)
{
}

void CPreBuffer::Init()
{
    m_sPartName = "PreBuffer " ;
    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();
}
void CPreBuffer::Close()
{
    Load(false);
}

void CPreBuffer::Update()
{
}
void CPreBuffer::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


double CPreBuffer::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miPRB_XIdx){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piPRB_XWait    : dPos = PM.GetValue (_iMotr , pvPRB_XWait    ); break ;
            case piPRB_XLIn     : dPos = PM.GetValue (_iMotr , pvPRB_XLIn     ); break ;
            case piPRB_XLHeat   : dPos = PM.GetValue (_iMotr , pvPRB_XLHeat   ); break ;
            case piPRB_XRGet    : dPos = PM.GetValue (_iMotr , pvPRB_XRGet    ); break ;
            case piPRB_XROut    : dPos = PM.GetValue (_iMotr , pvPRB_XROut    ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
void CPreBuffer::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CPreBuffer::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

//---------------------------------------------------------------------------

bool CPreBuffer::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (LDR.GetSeqStep() == CLoader::scWork) return false ; //로더 워킹 하면서 푸쉬 하고 있을때 스탑을 되어 스트립 걸려 있는 것 방지.
    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPreBuffer::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuffer::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: if(!DM.ARAY[riPRB].CheckAllStat(csNone)){
                     Step.iToStart = 0;
                     return true;
                 }
                 MoveActr(aiPRB_Lft , ccBwd);
                 MoveActr(aiPRB_Rgt , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiPRB_Lft , ccBwd)) return false ;
                 if(!MoveActr(aiPRB_Rgt , ccBwd)) return false ;
                 MoveMotr(miPRB_XIdx , piPRB_XWait) ;
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miPRB_XIdx , piPRB_XWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreBuffer::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: IO_SetY(yPRB_FeedingAC,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: //if(!MT_GetAlarm   (miPR1_XCmp) &&
                 //   !MT_GetNLimSnsr(miPR1_XCmp) &&
                 //   !MT_GetPLimSnsr(miPR1_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPR1_XCmp) ;
                 //else                             m_dLastIndPos = GetMotrPos  (miPR1_XCmp , piPR1_XWait) ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//---------------------------------------------------------------------------

bool CPreBuffer::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
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

        case 10: IO_SetY(yPRB_FeedingAC ,false);
                 AT_MoveCyl(aiPRB_Lft   ,ccBwd);
                 AT_MoveCyl(aiPRB_Rgt   ,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Lft   ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPRB_Rgt   ,ccBwd)) return false ;
                 MT_DoHome(miPRB_XIdx);
                 MT_SetHomeEnd(miPRB_XIdx , false);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPRB_XIdx)) return false;
                 MoveMotr(miPRB_XIdx, piPRB_XWait);
                 Step.iHome++;
                 return false ;

        case 13: if(!MoveMotr(miPRB_XIdx, piPRB_XWait)) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreBuffer::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPRB_FstOvrld ) || IO_GetX(xPRB_SndOvrld)      ) {
        EM_SetErrMsg(eiPRT_OverLoad , m_sPartName.c_str());
        MT_Stop(miPRB_XIdx);
        Step.iCycle = 0;
    }

    //Check Error & Decide Step.
    //다시 하자 JS.
    if (Step.iSeq == 0) {
        if(Stat.bReqStop && !IO_GetX(xPRB_PkgIn1))return false ;

        bool isCycleIn       =  DM.ARAY[riPRB].CheckAllStat(csNone) &&
                               (IO_GetX(xPRB_PkgIn1) || (OM.CmnOptn.bDryRun && IO_GetX(xLDR_PusherFw)));

        bool isCycleWait     =  DM.ARAY[riPRB].GetCntExist() &&
                              ((DM.ARAY[riPRB].GetStep () == 0) || DM.ARAY[riPRB].CheckAllStat(csNone));

        bool isCycleOut      =  DM.ARAY[riPRB].GetCntExist() &&
                               !DM.ARAY[riPRB].CheckAllStat(csNone) &&
                                DM.ARAY[riWRK].CheckAllStat(csNone) &&
                                m_tmHeat.OnDelay(true, OM.DevOptn.iWaitDelay) &&
                                DM.ARAY[riPRB].GetStep () == 1;

        bool isCycleEnd      =  DM.ARAY[riPRB].CheckAllStat(csNone);

        m_iArayStep = DM.ARAY[riPRB].GetStep();

        //Unknown Strip
        //if (IO_GetX(xPR1_Detect) && !OM.CmnOptn.bDryRun && DM.ARAY[riPR1].CheckAllStat(csNone)) EM_SetErr(eiPR1_Unknown) ;

        //Dissapear Strip
        //if (!isCycleIn && !isCycleOut && !OM.CmnOptn.bDryRun) {
        //if (!isCycleOut && !isCycleIn && isCycleDrag && !OM.CmnOptn.bDryRun && !DM.ARAY[riPR1].CheckAllStat(csNone) && !IO_GetX(xPR1_Detect) ) EM_SetErr(eiPR1_Dispr)   ;
        //if (!isCycleOut && !OM.CmnOptn.bDryRun && !OM.MstOptn.bUsedSmallSizePkg) {
        //    if (!DM.ARAY[riPR1].CheckAllStat(csNone) && !IO_GetX(xPR1_Detect) ) EM_SetErr(eiPR1_Dispr)   ;
        //}
        //
        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt"); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWait    ) {Trace(m_sPartName.c_str(),"CycleWait     Stt"); Step.iSeq = scWait     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt"); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default         :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                           return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scWait     : if(CycleWait    ()){ Trace(m_sPartName.c_str(),"CycleWait     End");Step.iSeq = scIdle ;} return false ;
        case scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}


//One Cycle.
bool CPreBuffer::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_CycleTO, sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        //IO_SetY(yPRB_AirBlower,false);
        IO_SetY(yPRB_FeedingAC,false);
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

        case  10: MoveActr(aiPRB_Lft , ccBwd);
                  MoveActr(aiPRB_Rgt , ccBwd);
                  MoveActr(aiPRB_Rlr , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Lft , ccBwd)) return false ;
                  if(!MoveActr(aiPRB_Rgt , ccBwd)) return false ;
                  if(!MoveActr(aiPRB_Rlr , ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC,true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPRBRlrDelay))return false;
                  MoveActr(aiPRB_Rlr, ccBwd);
                  MoveMotr(miPRB_XIdx , piPRB_XLIn);

                  Step.iCycle++;
                  return false;

        case  13: //|| ( !OM.CmnOptn.bDryRun && !IO_GetX(xLD1_PusherBw)) ) return false;
                  if(!MoveActr(aiPRB_Rlr , ccBwd)) return false ;
                  if(!MoveMotr(miPRB_XIdx , piPRB_XLIn)) return false ;
                  if(/*IO_GetX(xPRB_2Pkg) ||*/ IO_GetX(xPRB_PkgIn1) ) return false;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPRBRlrOffDelay))return false;
                  IO_SetY(yPRB_FeedingAC,false);
                  MoveActr(aiPRB_Rlr, ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPRB_Rlr , ccFwd))return false;
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuffer::CycleWait()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_CycleTO, sTemp.c_str());
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

    if(IO_GetX(xPRB_FstOvrld) || IO_GetX(xPRB_SndOvrld)) {

        EM_SetErr(eiPRT_OverLoad);
        Step.iCycle = 0;
        return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPRB_Lft, ccBwd);
                  MoveActr(aiPRB_Rgt, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Lft , ccBwd)) return false;
                  if(!MoveActr(aiPRB_Rgt , ccBwd)) return false;
                  MoveMotr(miPRB_XIdx, piPRB_XLIn);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XIdx , piPRB_XLIn)) return false;
                  MoveActr(aiPRB_Lft,ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPRB_Lft, ccFwd)) return false ;
                  MoveMotr(miPRB_XIdx, piPRB_XLHeat);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miPRB_XIdx, piPRB_XLHeat))return false ;
                  if(!IO_GetX(xWRK_Detect)){
                      EM_SetErrMsg(eiPKG_Dispr, m_sPartName.c_str());
                  }
                  m_tmHeat.Clear();
                  MoveActr(aiPRB_Lft , ccBwd);
                  Step.iCycle ++;
                  return false ;

        case  15: if(!MoveActr(aiPRB_Lft , ccBwd)) return false;
                  MoveMotr(miPRB_XIdx , piPRB_XRGet);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miPRB_XIdx , piPRB_XRGet))return false;

                  DM.ARAY[riPRB].SetStep(1);
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPreBuffer::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_CycleTO, sTemp.c_str());
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

        case  10: MoveActr(aiPRB_Lft , ccBwd);
                  MoveActr(aiPRB_Rgt , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Lft , ccBwd)) return false ;
                  if(!MoveActr(aiPRB_Rgt , ccBwd)) return false ;
                  MoveMotr(miPRB_XIdx , piPRB_XRGet);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPRB_XIdx , piPRB_XRGet)) return false ;
                  MoveActr(aiPRB_Rgt , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_Rgt , ccFwd)) return false;
                  Step.iCycle++;
                  return false ;

        case  14: MoveMotr(miPRB_XIdx , piPRB_XROut);

                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miPRB_XIdx , piPRB_XROut)) return false;
                  MoveActr(aiPRB_Rgt , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiPRB_Rgt , ccBwd)) return false;
                  MoveMotr(miPRB_XIdx, piPRB_XLIn);
                  m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                  DM.ShiftArrayData(riPRB,riWRK);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miPRB_XIdx, piPRB_XLIn))return false;
                  DM.ARAY[riPRB].SetStat(csNone);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuffer::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isFstFwd = AT_Complete(aiPRB_Lft  , ccFwd );
    bool isSndFwd = AT_Complete(aiPRB_Rgt  , ccFwd );
    //bool isIdClampDn  = AT_Complete(aiPRB_IdChk, ccBwd ) ;

    if(_iMotr == miPRB_XIdx){
        //if(isIdClampDn){ sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false; }
        switch(_iPstnId) {
            default             :                                                                                  break ;
            case piPRB_XWait    : if(isFstFwd) { sMsg = AT_GetName(aiPRB_Lft); sMsg += " is Fwd" ; bRet = false ;}
                                  if(isSndFwd) { sMsg = AT_GetName(aiPRB_Rgt); sMsg += " is Fwd" ; bRet = false ;} break ;
            case piPRB_XLIn     :                                                                                  break ;
            case piPRB_XLHeat   :                                                                                  break ;
            case piPRB_XRGet    :                                                                                  break ;
            case piPRB_XROut    :                                                                                  break ;
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

bool  CPreBuffer::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPRB_XIdx);

    bool bRet = true ;
    AnsiString sMsg ;

    //if(_iActr == aiPR1_Cmp) {
    //    if( !_bFwd) {
    //        if(isXMoving) { sMsg = MT_GetName(miPR1_XCmp)+" is Moving" ; bRet = false ;}
    //    }
    //}

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CPreBuffer::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}
bool CPreBuffer::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}


void CPreBuffer::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPreBuffer::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreBuffer::CheckStop()
{
    if(!MT_GetStop(miPRB_XIdx)) return false ;
    
    if(!AT_Done(aiPRB_Lft  )) return false ;
    if(!AT_Done(aiPRB_Rgt  )) return false ;
    if(!AT_Done(aiPRB_Rlr  )) return false ;
    //if(!AT_Done(aiPRB_IdChk)) return false ;

    return true ;
}

void CPreBuffer::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuffer.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
        UserINI.Load(sPath, "Member" , "m_iArayStep  " , m_iArayStep   );
    }


    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
        UserINI.Save(sPath, "Member" , "m_iArayStep  " , m_iArayStep   );
    }

}
//---------------------------------------------------------------------------
