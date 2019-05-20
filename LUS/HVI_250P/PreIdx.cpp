
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PreIdx.h"
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
#include "EqpComUnit.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#define INDEX_POS_OFFSET  2.0
#define INDEX_LAST_SPEED 10.0


CPreIdx PRI;


CPreIdx::CPreIdx(void)
{
    Init();
}

void CPreIdx::Init()
{
    m_sPartName = "Pre Index " ;

    m_sPreLotNo = "";

    m_sLastStrip = "";

    iReWorkCnt = 0 ;


    Reset();
    Load(true);


}
void CPreIdx::Close()
{
    Load(false);
}
void CPreIdx::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPreIdx::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPreIdx::~CPreIdx (void)
{
//    Close();
    //FormMain으로  Load(false);
}                                                                                         

bool CPreIdx::FindChip(int &r , int &c )
{
    c = DM.ARAY[riPRI].FindLastCol(csWork);
    c = DM.ARAY[riPRI].GetMaxCol  () - c -1;

    if(r == -1 && c == -1) return false ;
    return true ;
}

double CPreIdx::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos       = 0.0 ;

    int iPriEmpty = DM.ARAY[riPRI].GetCntStat(csEmpty) ;
    int iReWork   = iPriEmpty / (OM.DevOptn.iTrimColCnt * OM.DevInfo.iRowCnt ) ;

    double dWork  = PM.GetValue(miPRI_XIdx,pvPRI_XIdxWork) + iReWork * ( OM.DevOptn.iTrimColCnt * OM.DevInfo.dColPitch ) ;

    if(_iMotr == miPRI_XIdx){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPRI_XIdxWait      : dPos = PM.GetValue (_iMotr , pvPRI_XIdxWait     ); break ;
            case piPRI_XIdxWorkStt   : dPos = PM.GetValue (_iMotr , pvPRI_XIdxWork     ); break ;
//            case piPRI_XIdxRework1   : dPos = PM.GetValue (_iMotr , pvPRI_XIdxRework1  ); break ;
//            case piPRI_XIdxRework2   : dPos = PM.GetValue (_iMotr , pvPRI_XIdxRework2  ); break ;
            case piPRI_XIdxOut       : dPos = PM.GetValue (_iMotr , pvPRI_XIdxOut      ); break ;
            case piPRI_XIdxWork      : dPos = dWork                                     ; break ; 
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreIdx::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isZPunchUp   = PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait) + 1.0 >= MT_GetCmdPos(miWRK_ZPch) ;
    bool isPriStrip   =!DM.ARAY[riPRI].CheckAllStat(csNone);

    if(_iMotr == miPRI_XIdx){
        switch(_iPstnId) {
            default                :                                                                           break ;
            case piPRI_XIdxWait    :                                                                           break ;
            case piPRI_XIdxWorkStt : if(!isZPunchUp && isPriStrip) { sMsg = "Punch Tool Down"; bRet = false; } break ;
//            case piPRI_XIdxRework1 : if(!isZPunchUp && isPriStrip) { sMsg = "Punch Tool Down"; bRet = false; } break ;
//            case piPRI_XIdxRework2 : if(!isZPunchUp && isPriStrip) { sMsg = "Punch Tool Down"; bRet = false; } break ;

            case piPRI_XIdxOut     : if(!isZPunchUp              ) { sMsg = "Punch Tool Down"; bRet = false; } break ;
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
bool  CPreIdx::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool is1IdxMoving = !MT_GetStop(miPRI_XIdx);
    bool is2IdxMoving = !MT_GetStop(miPSI_XIdx);

    if(_iActr == aiPRI_Idx ) {
        if(is1IdxMoving) {
            sMsg = "Pre Index is Moving" ;
            bRet = false ;
        }
    }
    else if(_iActr == aiPSI_Idx ) {
        if(is2IdxMoving) {
            sMsg = "Post Index is Moving" ;
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

bool CPreIdx::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreIdx::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreIdx::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
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

        case 10: MT_Reset(miPRI_XIdx);
                 MT_SetServo(miPRI_XIdx,true);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRK_ZPch))return false ;
                 AT_MoveCyl(aiPRI_Idx  , ccBwd);
                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiPRI_Idx  , ccBwd)) return false ;
                 MT_DoHome(miPRI_XIdx) ;
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miPRI_XIdx))return false ;
                 MT_GoAbsMan(miPRI_XIdx , PM.GetValue(miPRI_XIdx , pvPRI_XIdxWait));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetStop(miPRI_XIdx)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPreIdx::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;
}

bool CPreIdx::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreIdx::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: //AT_MoveCyl(aiPRI_Idx ,ccBwd);
                 MoveMotr(miPRI_XIdx , piPRI_XIdxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!AT_MoveCyl(aiPRI_Idx ,ccBwd       ))return false ;
                 //MoveMotr(miPRI_XIdx , piPRI_XIdxWait);
                 if(!MoveMotr(miPRI_XIdx , piPRI_XIdxWait)) return false ;
                 AT_MoveCyl(aiPRI_Idx ,ccBwd);
                 Step.iToStart++;
                 return false ;

        case 12: //if(!MoveMotr(miPRI_XIdx , piPRI_XIdxWait)) return false ;
                 if(!AT_MoveCyl(aiPRI_Idx ,ccBwd       ))return false ;
                 if (!IO_GetX(xWRK_ToolDetected) && !OM.MstOptn.bDebugMode ) EM_SetErr(eiWRK_ToolSsrNotDectected );

                 Step.iToStart = 0;
                 return true ;
    }
}

bool CPreIdx::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: DM.ARAY[riPRI].SetStep(asNotReady) ;
                 //Last Position 기억 햇다가 해야 될수도 있는데 일단 이렇게 넣어보고 해봄. dd 모지? ;;; 밀고 찍을라고 그랫나 부다...대는지 확인좀. dd
                 MT_GoIncMan(miPRI_XIdx, -2.0 ) ;

                 Step.iToStop ++;
                 return false ;

        case 11: if(!MT_GetStopInpos(miPRI_XIdx)) return false ;
                 MoveActr(aiPRI_Idx,ccBwd);
                 Step.iToStop ++;
                 return false ;

        case 12: if(!MoveActr(aiPRI_Idx,ccBwd)) return false ;
                 MoveMotr(miPRI_XIdx,piPRI_XIdxWait);
                 Step.iToStop ++;
                 return false ;

        case 13: if(!MoveMotr(miPRI_XIdx,piPRI_XIdxWait)) return false ;
               
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPreIdx::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0 ) { //생각해 보니...안되네 나눔.dd

        bool bPunchEnd = DM.ARAY[riPRI].GetCntStat(csEmpty) >= OM.DevOptn.i1IdxReWorkCnt * OM.DevOptn.iTrimColCnt * OM.DevInfo.iRowCnt ;

//        bool isReWorkDev     = (OM.DevInfo.iColCnt / (double)OM.DevOptn.iTrimColCnt) > 2 ;
//        bool isReWorked      =  OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt / 2 < DM.ARAY[riPRI].GetCntStat(csEmpty) ; //나중에 맵핑 디스플레이 해야 하면 조건 바꿔야함.
        bool isPsiReady       =  DM.ARAY[riPSI].CheckAllStat(csNone) || (DM.ARAY[riPSI].CheckAllStat(csEmpty) && PSI.GetSeqStep() == CPstIdx::scOut) ;

        bool isPunchZWaitPs   = MT_CmprPos(miWRK_ZPch,PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait));

//수정 dd
//        bool isCycleToWork    =  isPsiReady &&  DM.ARAY[riPRI].GetStep() == asNotReady && DM.ARAY[riPRI].CheckAllStat(csWork) && !bPunchEnd ;
        bool isCycleToWork    =  isPsiReady &&  DM.ARAY[riPRI].GetStep() == asNotReady && !DM.ARAY[riPRI].CheckAllStat(csNone) && !bPunchEnd ;

        bool isCycleOut       =  DM.ARAY[riPSI].CheckAllStat(csNone) && !DM.ARAY[riPRI].CheckAllStat(csNone)    && DM.ARAY[riPRI].GetCntStat  (csWork) && bPunchEnd && WRK.GetSeqStep() == CWork::scIdle ;// && isPunchZWaitPs ;

        bool isCycleToReWork1 =  false ;
        bool isCycleToReWork2 =  false ;

//        bool isCycleToWork    =  isPsiReady &&  DM.ARAY[riPRI].GetStep() == asNotReady && DM.ARAY[riPRI].CheckAllStat(csWork) ;
//        bool isCycleToReWork =  isPsiReady &&  DM.ARAY[riPRI].GetStep() == asNotReady && DM.ARAY[riPRI].GetCntStat  (csWork) && DM.ARAY[riPRI].GetCntStat(csEmpty)&& ( isReWorkDev &&!isReWorked);
//        bool isCycleToReWork1 =  isPsiReady &&  DM.ARAY[riPRI].GetStep() == asNotReady && DM.ARAY[riPRI].GetCntStat  (csWork) && DM.ARAY[riPRI].GetCntStat(csEmpty)&& ( iReWorkCnt == 2) && WRK.GetSeqStep() == CWork::scIdle ;//&& isPunchZWaitPs ;
//        bool isCycleToReWork2 =  isPsiReady &&  DM.ARAY[riPRI].GetStep() == asNotReady && DM.ARAY[riPRI].GetCntStat  (csWork) && DM.ARAY[riPRI].GetCntStat(csEmpty)&& ( iReWorkCnt == 3) && WRK.GetSeqStep() == CWork::scIdle ;// && isPunchZWaitPs ;
        //아 졸려 죽겟어 살려줘... dd
        //iReWorkCnt == 2 3 ,,, 이거 더러워 좀 있다 좀 바꾸자. dd

//        bool isCycleOut       =  DM.ARAY[riPSI].CheckAllStat(csNone) && !DM.ARAY[riPRI].CheckAllStat(csNone)    && DM.ARAY[riPRI].GetCntStat  (csWork) && DM.ARAY[riPRI].GetCntStat(csEmpty)&& (iReWorkCnt == 0)&& WRK.GetSeqStep() == CWork::scIdle;
//        bool isCycleOut       =  DM.ARAY[riPSI].CheckAllStat(csNone) && !DM.ARAY[riPRI].CheckAllStat(csNone)    && DM.ARAY[riPRI].GetCntStat  (csWork) && (iReWorkCnt == 0) && WRK.GetSeqStep() == CWork::scIdle ;// && isPunchZWaitPs ;

        bool isConEnd         =  DM.ARAY[riPRI].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleToWork   ) {Trace(m_sPartName.c_str(),"CycleToWork    Stt"); Step.iSeq = scToWork    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleToReWork1) {Trace(m_sPartName.c_str(),"CycleToReWork1 Stt"); Step.iSeq = scToReWork1 ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleToReWork2) {Trace(m_sPartName.c_str(),"CycleToReWork2 Stt"); Step.iSeq = scToReWork2 ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleOut      ) {Trace(m_sPartName.c_str(),"CycleOut       Stt"); Step.iSeq = scOut       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq   ) {
        default           :                                                                       Step.iSeq = scIdle ;  return false ;
        case  scToWork    : if(CycleToWork   ()){ Trace(m_sPartName.c_str(),"CycleToWork    End");Step.iSeq = scIdle ;} return false ;
        case  scToReWork1 : if(CycleToReWork1()){ Trace(m_sPartName.c_str(),"CycleToReWork1 End");Step.iSeq = scIdle ;} return false ;
        case  scToReWork2 : if(CycleToReWork2()){ Trace(m_sPartName.c_str(),"CycleToReWork2 End");Step.iSeq = scIdle ;} return false ;
        case  scOut       : if(CycleOut      ()){ Trace(m_sPartName.c_str(),"CycleOut       End");Step.iSeq = scIdle ;} return false ;
    }
}
bool CPreIdx::CycleToWork(void) //
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

        case 10 :
                  Step.iCycle++;
                  return false ;

        case 11 :
                  Step.iCycle++;
                  return false ;

        case 12 : //if(!MoveMotr(miPRI_XIdx,piPRI_XIdxWait)) return false ;
                  MoveActr(aiPRI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPRI_Idx ,ccFwd)) return false ;
                  if(!CheckSafe(miPRI_XIdx , piPRI_XIdxWork)) return false ;
                  MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxWork) - INDEX_POS_OFFSET );

                  Step.iCycle++;
                  return false ;

        case 14 : if(IO_GetX(xWRK_1IdxOverload)) {
                      EM_SetErr(eiPRI_IdxOverLoad);
                      MT_Stop(miPRI_XIdx);

                      Step.iCycle=0;
                      return true ;
                  }

                  //if(!MT_GetStop(miPRI_XIdx)) return false ;
                  if(!MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxWork) - INDEX_POS_OFFSET )) return false ;
                  MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxWork) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 15 : //if(!MT_GetStop(miPRI_XIdx)) return false ;
                  if(IO_GetX(xWRK_1IdxOverload)) {
                      EM_SetErr(eiPRI_IdxOverLoad);
                      MT_Stop(miPRI_XIdx);

                      Step.iCycle=0;
                      return true ;
                  }
                  if(!MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxWork) , INDEX_LAST_SPEED )) return false ;

                  //Next Step .
                  DM.ARAY[riPRI].SetStep(asNeedTrim);

                  Step.iCycle++;
                  return false ;

        case 16 : //if(!MoveActr(aiPRI_Idx ,ccBwd)) return false ;

                  Step.iCycle = 0;
                  return true ;
    }
}
/*
bool CPreIdx::CycleToWork(void) //
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

        case 10 : iReWorkCnt = 0 ;
                  //MoveActr(aiPRI_Idx ,ccBwd);
                  MoveMotr(miPRI_XIdx,piPRI_XIdxWait);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiPRI_Idx ,ccBwd)) return false ;
                  if(!MoveMotr(miPRI_XIdx,piPRI_XIdxWait)) return false ;
//                  MoveActr(aiPRI_Idx ,ccBwd);
                  //MoveMotr(miPRI_XIdx,piPRI_XIdxWait);
                  Step.iCycle++;
                  return false ;

        case 12 : //if(!MoveMotr(miPRI_XIdx,piPRI_XIdxWait)) return false ;

                  MoveActr(aiPRI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPRI_Idx ,ccFwd)) return false ;
                  if(!CheckSafe(miPRI_XIdx , piPRI_XIdxWork)) return false ;
                  MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxWork) - INDEX_POS_OFFSET );

                  Step.iCycle++;
                  return false ;

        case 14 : if(!MT_GetStop(miPRI_XIdx)) return false ;
                  MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxWork) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MT_GetStop(miPRI_XIdx)) return false ;

                  if(OM.DevOptn.i1IdxReWorkCnt > 1) iReWorkCnt = 2;
                  //Next Step .
                  DM.ARAY[riPRI].SetStep(asNeedTrim);
                  //MoveActr(aiPRI_Idx ,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 16 : //if(!MoveActr(aiPRI_Idx ,ccBwd)) return false ;


                  Step.iCycle = 0;
                  return true ;
    }
}
*/

bool CPreIdx::CycleToReWork1(void) //
{
/*
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

        case 10 : //MoveActr(aiPRI_Idx ,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiPRI_Idx ,ccBwd)) return false ;

                  MoveMotr(miPRI_XIdx , piPRI_XIdxWorkStt);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPRI_XIdx,piPRI_XIdxWorkStt)) return false ;
                  MoveActr(aiPRI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPRI_Idx ,ccFwd)) return false ;
                  if(!CheckSafe(miPRI_XIdx , piPRI_XIdxRework1)) return false ;
                  MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxRework1) - INDEX_POS_OFFSET );

                  Step.iCycle++;
                  return false ;

        case 14 : if(!MT_GetStop(miPRI_XIdx)) return false ;
                  MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxRework1) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MT_GetStop(miPRI_XIdx)) return false ;
                  //Next Step .
                  DM.ARAY[riPRI].SetStep(asNeedTrim);

                  //MoveActr(aiPRI_Idx , ccBwd);

//        case 16 : //if(!MoveActr(aiPRI_Idx , ccBwd)) return false ;
                  if(OM.DevOptn.i1IdxReWorkCnt > 2) iReWorkCnt = 3;
                  else                              iReWorkCnt = 0;


                  Step.iCycle = 0;
                  return true ;
    }

*/
}
bool CPreIdx::CycleToReWork2(void) //
{
/*
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

        case 10 : //MoveActr(aiPRI_Idx ,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiPRI_Idx ,ccBwd)) return false ;

                  MoveMotr(miPRI_XIdx , piPRI_XIdxRework1);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPRI_XIdx,piPRI_XIdxRework1)) return false ;
                  MoveActr(aiPRI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPRI_Idx ,ccFwd)) return false ;
                  if(!CheckSafe(miPRI_XIdx , piPRI_XIdxRework2)) return false ;
                  MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxRework2) - INDEX_POS_OFFSET );

                  Step.iCycle++;
                  return false ;

        case 14 : if(!MT_GetStop(miPRI_XIdx)) return false ;
                  MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxRework2) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MT_GetStop(miPRI_XIdx)) return false ;
                  //Next Step .
                  DM.ARAY[riPRI].SetStep(asNeedTrim);

                  //MoveActr(aiPRI_Idx , ccBwd);

//        case 16 : //if(!MoveActr(aiPRI_Idx , ccBwd)) return false ;
                  iReWorkCnt  = 0;
                  Step.iCycle = 0;
                  return true ;
    }
*/
}

bool CPreIdx::CycleOut(void) //
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

        case 10 : //MoveActr(aiPRI_Idx ,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiPRI_Idx ,ccBwd)) return false ;

                  //MoveMotr(miPRI_XIdx , piPRI_XIdxRework);
                  Step.iCycle++;
                  return false ;

        case 12 : //if(!MoveMotr(miPRI_XIdx,piPRI_XIdxRework)) return false ;
                  MoveActr(aiPRI_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPRI_Idx ,ccFwd)) return false ;
                  if(!CheckSafe(miPRI_XIdx , piPRI_XIdxOut)) return false ;
                  if(OM.CmnOptn.dPreIdxOutSpd <= 0) OM.CmnOptn.dPreIdxOutSpd = 50 ;
                  MT_GoAbs(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxOut), OM.CmnOptn.dPreIdxOutSpd );

                  Step.iCycle++;
                  return false ;

        case 14 : if(IO_GetX(xWRK_1IdxOverload)) {
                      EM_SetErr(eiPRI_IdxOverLoad);
                      MT_Stop(miPRI_XIdx);
                      Step.iCycle=0;
                      return true ;
                  }
                  if(!MT_GoAbs(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxOut), OM.CmnOptn.dPreIdxOutSpd )) return false ;


/*
                  MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxOut) - INDEX_POS_OFFSET );

                  Step.iCycle++;
                  return false ;

        case 14 : if(IO_GetX(xWRK_1IdxOverload)) {
                      EM_SetErr(eiPRI_IdxOverLoad);
                      MT_Stop(miPRI_XIdx);

                      Step.iCycle=0;
                      return true ;
                  }

//                  if(!MT_GetStop(miPRI_XIdx)) return false ;
                  if(!MT_GoAbsRun(miPRI_XIdx, GetMotrPos(miPRI_XIdx , piPRI_XIdxOut) - INDEX_POS_OFFSET )) return false ;
                  MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxOut) , INDEX_LAST_SPEED );
                  Step.iCycle++;
                  return false ;

        case 15 : if(IO_GetX(xWRK_1IdxOverload)) {
                      EM_SetErr(eiPRI_IdxOverLoad);
                      MT_Stop(miPRI_XIdx);

                      Step.iCycle=0;
                      return true ;
                  }
//                  if(!MT_GetStop(miPRI_XIdx)) return false ;
                  if(!MT_GoAbs(miPRI_XIdx , GetMotrPos(miPRI_XIdx , piPRI_XIdxOut) , INDEX_LAST_SPEED )) return false ;
*/
                  //Next Step .
                  m_sPreLotNo = DM.ARAY[riPRI].GetLotNo() ;
                  //칩 카운트 때문에..JS
                  m_iChipCnt = DM.ARAY[riPRI].GetCntStat(csWork);

                  DM.ShiftArrayData(riPRI , riPSI) ;

                  MoveActr(aiPRI_Idx , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiPRI_Idx , ccBwd)) return false ;
                  MoveMotr(miPRI_XIdx , piPRI_XIdxWait);
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveMotr(miPRI_XIdx , piPRI_XIdxWait)) return false ;

                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPreIdx::GetLastStrip()
{
    //필요하면 Work꺼 가져다 만들어랏.
    return false ;
}
void CPreIdx::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPreIdx::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreIdx::CheckStop()
{
    if(!AT_Done(aiPRI_Idx    )) return false ;
    if(!MT_GetStop(miPRI_XIdx)) return false ;

    return true ;
}

void CPreIdx::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sLastStrip   " , m_sLastStrip     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo    " , m_sPreLotNo      );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iReWorkCnt     " , iReWorkCnt       );

    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sLastStrip   " , m_sLastStrip     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo    " , m_sPreLotNo      );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iReWorkCnt     " , iReWorkCnt       );

    }
}

