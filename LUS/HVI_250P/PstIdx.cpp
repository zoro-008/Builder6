
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PstIdx.h"
#include "Work.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"

//#include "FormRslt.h"
#include "EqpComUnit.h"
//---------------------------------------------------------------------------
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define INDEX_POS_OFFSET  2.0
#define INDEX_LAST_SPEED  5.0
//---------------------------------------------------------------------------
CPstIdx PSI;

CPstIdx::CPstIdx(void)
{
    Init();
}

void CPstIdx::Init()
{
    m_sPartName = "PostBuff " ;

    m_sPreLotNo = "";

    Reset();
    Load(true);
}

void CPstIdx::Close()
{
    Load(false);
}
void CPstIdx::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPstIdx::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPstIdx::~CPstIdx (void)
{
//    Close();
    //FormMain으로  Load(false);
}                                                                                         

bool CPstIdx::FindChip(int &r , int &c )
{
    c = DM.ARAY[riPSI].FindLastCol(csWork);
    c = DM.ARAY[riPSI].GetMaxCol() - c -1;

    if(r == -1 && c == -1) return false ;
    return true ;
}

double CPstIdx::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    int iPriReWork = OM.DevOptn.i1IdxReWorkCnt * (OM.DevOptn.iTrimColCnt * OM.DevInfo.iRowCnt ) ;

    int iPsiEmpty  = DM.ARAY[riPSI].GetCntStat(csEmpty) - iPriReWork ;
    int iReWork    = iPsiEmpty / (OM.DevOptn.iTrimColCnt * OM.DevInfo.iRowCnt ) ;

//    double dWork   = PM.GetValue(miPRI_XIdx,pvPRI_XIdxWork) + iReWork * ( OM.DevOptn.iTrimColCnt * OM.DevInfo.dColPitch ) ;
    double dWork   = PM.GetValue(miPSI_XIdx,pvPSI_XIdxWork) + iReWork * ( OM.DevOptn.iTrimColCnt * OM.DevInfo.dColPitch ) ;

    if(_iMotr == miPSI_XIdx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piPSI_XIdxWait    : dPos = PM.GetValue (_iMotr , pvPSI_XIdxWait   ); break ;
            case piPSI_XIdxWorkStt : dPos = PM.GetValue (_iMotr , pvPSI_XIdxWork   ); break ;
            case piPSI_XIdxPull    : dPos = PM.GetValue (_iMotr , pvPSI_XIdxPull   ); break ;
            case piPSI_XIdxBack    : dPos = PM.GetValue (_iMotr , pvPSI_XIdxBack   ); break ;
            case piPSI_XIdxOut     : dPos = PM.GetValue (_iMotr , pvPSI_XIdxOut    ); break ;
            case piPSI_XIdxWork    : dPos = dWork;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPstIdx::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miPSI_XIdx){
        switch(_iPstnId) {
            default                :  break ;
            case piPSI_XIdxWait    :  break ;
            case piPSI_XIdxWorkStt :  break ;
            case piPSI_XIdxPull    :  break ;
            case piPSI_XIdxBack    :  break ;
            case piPSI_XIdxOut     :  break ;
            case piPSI_XIdxWork    :  break ;
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return bRet ;
}

//---------------------------------------------------------------------------
bool  CPstIdx::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isIdxMoving = !MT_GetStop(miPSI_XIdx);

    if(_iActr == aiPSI_Idx ) {
        if(isIdxMoving) {
            sMsg = "PSI 2 Index is Moving" ;
            bRet = false ;
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CPstIdx::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle ) return MT_GoAbsRun(_iMotr , dPosition);
    else             return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPstIdx::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPstIdx::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_HomeTO);
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

        case 10: MT_Reset(miPSI_XIdx);
                 MT_SetServo(miPSI_XIdx,true);
                 AT_MoveCyl(aiPSI_Idx  , ccBwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSI_Idx  , ccBwd)) return false ;
                 MT_DoHome(miPSI_XIdx) ;
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSI_XIdx))return false ;
                 Step.iHome++;
                 return false ;

        case 13: MT_GoAbsMan(miPSI_XIdx , PM.GetValue(miPSI_XIdx,pvPSI_XIdxWait));



                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetStop(miPSI_XIdx)) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CPstIdx::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;
//    m_ToStopTimer.Clear();

    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;
}

bool CPstIdx::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPstIdx::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiWRK_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPSI_Idx ,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSI_Idx ,ccBwd       ))return false ;
                 MT_GoAbsMan(miPSI_XIdx , m_dLastIdxPos);
                 Step.iToStart ++;
                 return false ;

        case 12: if(!MT_GetStop(miPSI_XIdx)) return false ;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CPstIdx::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiWRK_ToStopTO);

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

        case 10: if(!MT_GetAlarm   (miPSI_XIdx) && !MT_GetNLimSnsr(miPSI_XIdx) && !MT_GetPLimSnsr(miPSI_XIdx) ) m_dLastIdxPos = MT_GetCmdPos(miPSI_XIdx) ;
                 else                                                                                           m_dLastIdxPos = GetMotrPos  (miPSI_XIdx , piPSI_XIdxWait) ;
                 DM.ARAY[riPSI].GetStep() == asNotReady ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPstIdx::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0 ) { //생각해 보니...안되네 나눔.dd

        bool bReWorkNeed     = (DM.ARAY[riPSI].GetCntStat(csEmpty) > OM.DevOptn.i1IdxReWorkCnt * OM.DevOptn.iTrimColCnt * OM.DevInfo.iRowCnt) && OM.DevOptn.i2IdxReWorkCnt;

        bool isCycleToWork   =  DM.ARAY[riPSI].GetCntStat  (csWork ) && DM.ARAY[riPSI].GetStep() == asNotReady && !bReWorkNeed ;
        bool isCycleReWork   =  DM.ARAY[riPSI].GetCntStat  (csWork ) && DM.ARAY[riPSI].GetStep() == asNotReady &&  bReWorkNeed ;//&& !bPunchEnd ;
        bool isCycleOut      =  DM.ARAY[riPSI].CheckAllStat(csEmpty) ;
        bool isConEnd        =  DM.ARAY[riPSI].CheckAllStat(csNone ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleToWork  ) {Trace(m_sPartName.c_str(),"CycleToWork   Stt"); Step.iSeq = scToWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleReWork  ) {Trace(m_sPartName.c_str(),"CycleToReWork Stt"); Step.iSeq = scToReWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt"); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd       ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq   ) {
        default          :                                                                     Step.iSeq = scIdle ;  return false ;
        case  scToWork   : if(CycleToWork  ()){ Trace(m_sPartName.c_str(),"CycleToWork   End");Step.iSeq = scIdle ;} return false ;
        case  scToReWork : if(CycleToReWork()){ Trace(m_sPartName.c_str(),"CycleToReWork End");Step.iSeq = scIdle ;} return false ;
        case  scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
    }
}


bool CPstIdx::CycleToWork(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSI_Idx ,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSI_Idx ,ccBwd)) return false ;

                  MoveMotr(miPSI_XIdx,piPSI_XIdxWait);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSI_XIdx,piPSI_XIdxWait)) return false ;
                  MoveActr(aiPSI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSI_Idx ,ccFwd)) return false ;
                  if(IO_GetX(xWRK_2IdxOverload2)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      Step.iCycle=0;
//                      MT_Stop(miPSI_XIdx);
                      return false ;
                  }
                  MT_GoAbsRun(miPSI_XIdx, GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) - INDEX_POS_OFFSET );
                  Step.iCycle++;
                  return false ;

        case 14 : if(IO_GetX(xWRK_2IdxOverload)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      MT_Stop(miPSI_XIdx);
//                      Step.iCycle=0;
                      return false ;
                  }
                  if(!MT_GoAbsRun(miPSI_XIdx, GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) - INDEX_POS_OFFSET )) return false ;
                  MT_GoAbs(miPSI_XIdx , GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 15 : if(IO_GetX(xWRK_2IdxOverload)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      MT_Stop(miPSI_XIdx);

//                      Step.iCycle=0;
                      return false ;
                  }
                  if(!MT_GoAbs(miPSI_XIdx , GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) , INDEX_LAST_SPEED )) return false ;
                  DM.ARAY[riPSI].SetStep(asNeedTrim);
                  Step.iCycle = 0;
                  return true ;
    }
}


bool CPstIdx::CycleToReWork(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSI_Idx ,ccFwd)) return false ;
                  if(IO_GetX(xWRK_2IdxOverload2)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      Step.iCycle=0;
//                      MT_Stop(miPSI_XIdx);
                      return false ;
                  }
                  MT_GoAbsRun(miPSI_XIdx, GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) - INDEX_POS_OFFSET );
                  Step.iCycle++;
                  return false ;

        case 12 : if(IO_GetX(xWRK_2IdxOverload)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      MT_Stop(miPSI_XIdx);
//                      Step.iCycle=0;
                      return false ;
                  }
                  if(!MT_GoAbsRun(miPSI_XIdx, GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) - INDEX_POS_OFFSET )) return false ;
                  MT_GoAbs(miPSI_XIdx , GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 13 : if(IO_GetX(xWRK_2IdxOverload)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      MT_Stop(miPSI_XIdx);

//                      Step.iCycle=0;
                      return false ;
                  }
                  if(!MT_GoAbs(miPSI_XIdx , GetMotrPos(miPSI_XIdx , piPSI_XIdxWork) , INDEX_LAST_SPEED )) return false ;
                  DM.ARAY[riPSI].SetStep(asNeedTrim);
                  Step.iCycle = 0;
                  return true ;

    }
}

bool CPstIdx::CycleOut(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : //MoveActr(aiPSI_Idx ,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiPSI_Idx ,ccBwd)) return false ;
                  //MoveMotr(miPSI_XIdx , piPSI_XIdxRework);
                  if(!MT_GetStop(miWRK_ZPch))return false;
                  Step.iCycle++;
                  return false ;

        case 12 : //if(!MoveMotr(miPSI_XIdx,piPSI_XIdxRework)) return false ;
                  MoveActr(aiPSI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSI_Idx ,ccFwd)) return false ;
                  MoveMotr(miPSI_XIdx , piPSI_XIdxPull);

                  Step.iCycle++;
                  return false ;

        case 14 : if(IO_GetX(xWRK_2IdxOverload)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      MT_Stop(miPSI_XIdx);
                      return false ;
                  }
                  if(!MoveMotr(miPSI_XIdx , piPSI_XIdxPull)) return false ;
                  MoveActr(aiPSI_Idx , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiPSI_Idx , ccBwd)) return false ;
                  MoveMotr(miPSI_XIdx , piPSI_XIdxBack);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveMotr(miPSI_XIdx , piPSI_XIdxBack)) return false ;
                  MoveActr(aiPSI_Idx , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(!MoveActr(aiPSI_Idx , ccFwd)) return false ;
                  MoveMotr(miPSI_XIdx , piPSI_XIdxOut);
                  m_sPreLotNo = DM.ARAY[riPSI].GetLotNo();
                  DM.ARAY[riPSI].SetStat(csNone);
                  Step.iCycle++;
                  return false ;

        case 18 : if(IO_GetX(xWRK_2IdxOverload)) {
                      EM_SetErr(eiPSI_IdxOverLoad);
                      MT_Stop(miPSI_XIdx);
                      return false ;
                  }

                  if(!MoveMotr(miPSI_XIdx , piPSI_XIdxOut)) return false ;
                  MoveActr(aiPSI_Idx , ccBwd);
                  MoveMotr(miPSI_XIdx , piPSI_XIdxWait);
                  Step.iCycle++;
                  return false ;

        case 19 : if(!MoveActr(aiPSI_Idx  , ccBwd         )) return false ;
                  if(!MoveMotr(miPSI_XIdx , piPSI_XIdxWait)) return false ;

                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPstIdx::GetLastStrip()
{
    //필요하면 Work꺼 가져다 만들어랏.
    return false ;
}
void CPstIdx::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPstIdx::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPstIdx::CheckStop()
{
    if(!AT_Done(aiPSI_Idx    )) return false ;

    if(!MT_GetStop(miPSI_XIdx)) return false ;

    return true ;
}

void CPstIdx::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo    " , m_sPreLotNo      ); 
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_dLastIdxPos  " , m_dLastIdxPos    );
    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo    " , m_sPreLotNo      );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_dLastIdxPos  " , m_dLastIdxPos    );
    }
}

