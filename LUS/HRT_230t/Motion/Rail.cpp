//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Rail.h"
#include "Feed.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "Sort.h"
#include "Work.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CRail RAL;

CRail::CRail()
{
    Init();
}
CRail::~CRail (void)
{
    Close();
}
void CRail::Init()
{
    m_sPartName   = "Rail " ;

    m_iHomeTO  = (int)eiRAL_HomeTO    ;
    m_iStartTO = (int)eiRAL_ToStartTO ;
    m_iStopTO  = (int)eiRAL_ToStopTO  ;
    m_iCycleTO = (int)eiRAL_CycleTO   ;

    m_iBlowerStep = 0 ;
    //bFedStt = false ;
//    bRALAray      = false ;
//    bFirst        = false ;

    Reset();
    Load(true);
}
void CRail::Close()
{
    Load(false);
}
void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CRail::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    //int r1,c1;
    //if(_riWhre == riRAE) {
    //    c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
    //    return (c > -1 && c > -1);
    //}
    return false ;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miRAL_XIdx){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piRAL_XIdxWait     : dPos = PM.GetValue (_iMotr , pvRAL_XIdxWait     ); break ;
            case piRAL_XIdxOut      : dPos = PM.GetValue (_iMotr , pvRAL_XIdxOut      ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isNotWaitPrb = MT_CmprPos(miWRK_ZPrb,PM.GetValue(miWRK_ZPrb,pvWRK_ZPrbWait));
    if(_iMotr == miRAL_XIdx){
        switch(_iPstnId) {
            default                 : break ;
            case piRAL_XIdxWait     : if(!Step.iCycle && !isNotWaitPrb) {sMsg = "Probe Z Axis is Not Wait Position" ; bRet = false ;} break ;
            case piRAL_XIdxOut      : if(!isNotWaitPrb) {sMsg = "Probe Z Axis is Not Wait Position" ; bRet = false ;} break ;
        }

        //sMsg = "Epoxy_Z Axis is Not Wait Position" ;
        //bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = !MT_GetStopInpos(miRAL_XIdx);

    if(_iActr == aiRAL_IdxUpDn ){
        //sMsg = "Epoxy_X_Index is Moving" ;
        //bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRail::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_Reset(miRAL_XIdx);
                 MT_SetServo(miRAL_XIdx,true);

                 AT_MoveCyl(aiRAL_IdxUpDn  , ccFwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiRAL_IdxUpDn  , ccFwd))return false ;
                 MT_DoHome(miRAL_XIdx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miRAL_XIdx))return false ;
                 MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miRAL_XIdx , PM.GetValue(miRAL_XIdx , pvRAL_XIdxWait))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: //bFedStt = false ;
                 MoveActr(aiRAL_IdxUpDn  , ccFwd);
//                 bFirst = true ;
                 IO_SetY(yRAL_Blower , false);

                 m_iBlowerStep = 0 ;

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiRAL_IdxUpDn  ,ccFwd)) return false ;
                 MoveMotr(miRAL_XIdx,piRAL_XIdxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miRAL_XIdx,piRAL_XIdxWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: MoveActr(aiRAL_IdxUpDn  , ccFwd);
                 IO_SetY(yRAL_Blower , false);
                 m_iBlowerStep = 0 ;
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiRAL_IdxUpDn  ,ccFwd)) return false ;
                 MoveMotr(miRAL_XIdx,piRAL_XIdxWait);
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(miRAL_XIdx,piRAL_XIdxWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

void CRail::BlowerCycle()
{
    static int iStep = 0 ;
    static CDelayTimer BlowerTimer ;

    switch (m_iBlowerStep) {

        case 0 : if(!IO_GetY(yRAL_Blower)) return ;
                 BlowerTimer.Clear();
                 m_iBlowerStep++;
                 return ;

        case 1 : //if(!BlowerTimer.OnDelay(true , OM.DevOptn.iBlowerDelay)) return ;
                 IO_SetY(yRAL_Blower , false );
                 m_iBlowerStep = 0 ;
                 return ;
    }
}

bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

//    BlowerCycle();

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //if(DM.ARAY[riRAL].CheckAllStat(csNone) && DM.ARAY[riRAB].CheckAllStat(csNone) && IO_GetX(xRAL_Pkg)) DM.ARAY[riRAL].SetStat(csUnkwn);

        bool bFinishInsp    = !DM.ARAY[riWRK].CheckAllStat(csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone ) ;
        bool bFirstPkg      =  DM.ARAY[riWRK].CheckAllStat(csNone ) &&  DM.ARAY[riRAL].CheckAllStat(csUnkwn) ;

        bool bisFeeding     = FED.GetSeqStep() == CFeed::scFeed;
        bool bisInsp        = WRK.GetSeqStep() == CWork::scWork ;

        bool bDustPs        =  IO_GetX(xRAL_PkgDustChk);
        bool bPrbWaitPs     = MT_CmprPos(miWRK_ZPrb,PM.GetValue(miWRK_ZPrb , pvWRK_ZPrbWait));

        bool isCycleWorkFist  = (bFirstPkg || bFinishInsp  ) && bPrbWaitPs && !bisInsp && !bDustPs && !bisFeeding ;
        bool isCycleWorkScnd  = false ;//(bFinishInsp ) && bPrbWaitPs && !bisFeeding; 안씀. 만들다 맘 혹시나 해서 아직 있음.
        bool isCycleWorkThrd  = false ;//(bFinishInsp ) && bPrbWaitPs && !bisFeeding; 안씀.

        bool isConEnd       = !isCycleWorkFist && !isCycleWorkScnd;

        if(EM_IsErr()) return false ;

        if(DM.ARAY[riRAL].CheckAllStat(csUnkwn) && !bisFeeding && m_tmRailPkg.OnDelay(true , 2000)){
            if(IO_GetX(xRAL_Pkg)) EM_SetErr(eiRAL_DustChip);
        }
        if(!DM.ARAY[riRAL].CheckAllStat(csUnkwn)){
            m_tmRailPkg.Clear();
        }

         //Normal Decide Step.
             if (isCycleWorkFist) {Trace(m_sPartName.c_str(),"CycleWorkFist  Stt"); Step.iSeq = scWorkFst ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkScnd) {Trace(m_sPartName.c_str(),"CycleWorkScnd  Stt"); Step.iSeq = scWorkScd ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkThrd) {Trace(m_sPartName.c_str(),"CycleWorkScnd  Stt"); Step.iSeq = scWorkScd ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //

        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                      Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                          return false ;
        case scWorkFst : if(CycleWorkFist ()){ Trace(m_sPartName.c_str(),"CycleWorkFist End");Step.iSeq = scIdle ;} return false ;
        case scWorkScd : if(CycleWorkScnd ()){ Trace(m_sPartName.c_str(),"CycleWorkScnd End");Step.iSeq = scIdle ;} return false ;
    }
}
//One Cycle.
bool CRail::CycleWorkFist(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int iTemp = 0 ;
    static double dPreTime = GetTickTime();
    double dCrntTime ;
    static bool bWrkAray = false;
    static bool bRALAray = false;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;
                  //bFedStt = false ;

                  //일단 보류.if( DM.ARAY[riWRK ].CheckAllStat(csNone) &&  IO_GetX(xWRK_PkgExist)) {EM_SetErr(eiWRK_PkgUnknown ) ; return ;}

                       if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt1)) SRT.SetBin(1);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt2)) SRT.SetBin(2);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt3)) SRT.SetBin(3);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt4)) SRT.SetBin(4);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt5)) SRT.SetBin(5);
                  else                                                                                   SRT.SetBin(OM.DevOptn.iEpmtyBin);

                  if(SRT.GetBin() == 1 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin1FullCnt ) { EM_SetErr(eiSRT_Bin1Full); /*SRT.ClearBinCnt(1);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 2 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin2FullCnt ) { EM_SetErr(eiSRT_Bin2Full); /*SRT.ClearBinCnt(2);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 3 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin3FullCnt ) { EM_SetErr(eiSRT_Bin3Full); /*SRT.ClearBinCnt(3);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 4 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin4FullCnt ) { EM_SetErr(eiSRT_Bin4Full); /*SRT.ClearBinCnt(4);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 5 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin5FullCnt ) { EM_SetErr(eiSRT_Bin5Full); /*SRT.ClearBinCnt(5);*/ Step.iCycle = 0 ; return true ; }

                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
                  MoveActr(aiRAL_IdxUpDn, ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false ;
                  if(!MoveActr(aiRAL_IdxUpDn, ccFwd)) return false ;
                  if(IO_GetX(xRAL_PkgDustChk)) return false;
                  Step.iCycle++;
                  return false ;

        case  12: MoveActr(aiRAL_IdxUpDn , ccBwd);

                  Step.iCycle++;
                  return false;

        case  13: if(MM.GetManNo() == mcNoneCycle) {
                      if(SRT.GetCntBin() != SRT.GetBin()) return false ;
                  }
                  if(!MoveActr(aiRAL_IdxUpDn , ccBwd)) return false ;

                  DM.ShiftArrayData(riRAL, riRAB); //인덱스 모터 움직이는 동안에 자재 한개 넣기 위해서 어레이 추가
                  MoveMotr(miRAL_XIdx , piRAL_XIdxOut );
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxOut )) return false ;

                  DM.ShiftArrayData(riRAB, riWRK); //인덱스 모터 움직임 끝나고 어레이 넘겨주기
                  //DM.ARAY[riRAB].SetStat(csNone);

                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
                  MoveActr(aiRAL_IdxUpDn, ccFwd);

                  //일단 보류....  if(!DM.ARAY[riWRK ].CheckAllStat(csNone) && !IO_GetX(xWRK_PkgExist)) EM_SetErr(eiWRK_PkgDispr   ) ;

                  Step.iCycle++;
                  return false;


        case  15: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false;
                  if(!MoveActr(aiRAL_IdxUpDn, ccFwd        )) return false;
                  //if(SRT.GetSeqStep() == CSort::scDetect) return false ;
                  if(!DM.ARAY[riWRK].CheckAllStat(csNone) && !IO_GetX(xWRK_Pkg)){
                      DM.ARAY[riWRK].SetStat(csNone);
                      EM_SetErr(eiWRK_PkgDispr);
                      Step.iCycle=0;
                      return true;
                  }
                  if(MM.GetManNo() == mcNoneCycle) {
                      if(!SRT.GetSortEnd()) return false ;
                  }
                  //bFedStt = false ;
//                  if(!SRT.bSucDetect && bWrkAray) {
//                      EM_SetErr(eiSRT_PkgNotDetected);
//                      Step.iCycle = 0 ;
//                      return true ;
//                  }

                  if(SRT.GetBin() == 1 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin1FullCnt ) { EM_SetErr(eiSRT_Bin1Full); /*SRT.ClearBinCnt(1);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 2 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin2FullCnt ) { EM_SetErr(eiSRT_Bin2Full); /*SRT.ClearBinCnt(2);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 3 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin3FullCnt ) { EM_SetErr(eiSRT_Bin3Full); /*SRT.ClearBinCnt(3);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 4 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin4FullCnt ) { EM_SetErr(eiSRT_Bin4Full); /*SRT.ClearBinCnt(4);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 5 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin5FullCnt ) { EM_SetErr(eiSRT_Bin5Full); /*SRT.ClearBinCnt(5);*/ Step.iCycle = 0 ; return true ; }

                  Step.iCycle = 0 ;
                  return true ;
    }
}


//One Cycle.
bool CRail::CycleWorkScnd(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int iTemp = 0 ;
    static double dPreTime = GetTickTime();
    double dCrntTime ;
    static bool bWrkAray = false;
    static bool bRALAray = false;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;
//                  if(DM.ARAY[riRAL].CheckAllStat(csUnkwn)) bRALAray = true   ;
//                  else                                     bRALAray = false  ;

//                  if(IO_GetX(xWRK_PkgDoubleChk)){
//                      EM_SetErr(eiWRK_DoubleChip);
//                      Step.iCycle = 0;
//                      return true;
//                  }

//                  if(DM.ARAY[riWRK].CheckAllStat(csNone )) {
//                      SRT.SetBin(OM.DevOptn.iEpmtyBin);
//                  }

                       if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt1)) SRT.SetBin(1);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt2)) SRT.SetBin(2);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt3)) SRT.SetBin(3);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt4)) SRT.SetBin(4);
                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt5)) SRT.SetBin(5);




//                  if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && !IO_GetX(xWRK_PkgExist)) {
//                      SRT.SetBin(OM.DevOptn.iEpmtyBin ,true);
//                  }
//                  if(DM.ARAY[riWRK].CheckAllStat((EN_CHIP_STAT)OM.DevOptn.iEpmtyBin)) {
//                      SRT.SetBin(OM.DevOptn.iEpmtyBin ,true);
//                  }


//                  if(!DM.ARAY[riWRK].CheckAllStat(csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone ) ) {
                      if(SRT.GetBin() == 1 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin1FullCnt ) { EM_SetErr(eiSRT_Bin1Full); /*SRT.ClearBinCnt(1);*/ Step.iCycle = 0 ; return true ; }
                      if(SRT.GetBin() == 2 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin2FullCnt ) { EM_SetErr(eiSRT_Bin2Full); /*SRT.ClearBinCnt(2);*/ Step.iCycle = 0 ; return true ; }
                      if(SRT.GetBin() == 3 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin3FullCnt ) { EM_SetErr(eiSRT_Bin3Full); /*SRT.ClearBinCnt(3);*/ Step.iCycle = 0 ; return true ; }
                      if(SRT.GetBin() == 4 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin4FullCnt ) { EM_SetErr(eiSRT_Bin4Full); /*SRT.ClearBinCnt(4);*/ Step.iCycle = 0 ; return true ; }
                      if(SRT.GetBin() == 5 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin5FullCnt ) { EM_SetErr(eiSRT_Bin5Full); /*SRT.ClearBinCnt(5);*/ Step.iCycle = 0 ; return true ; }
//                  }


                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
                  MoveActr(aiRAL_IdxUpDn, ccFwd);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false ;
                  if(!MoveActr(aiRAL_IdxUpDn, ccFwd)) return false ;
                  if(m_tmTemp.OnDelay(IO_GetX(xRAL_Pkg) , OM.DevOptn.iFeedWaitTime)){
                      EM_SetErr(eiRAL_PkgJam);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(IO_GetX(xRAL_Pkg)) return false ;
                  Step.iCycle++;
                  return false ;

        case  12: MoveActr(aiRAL_IdxUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiRAL_IdxUpDn , ccBwd)) return false ;
                  //if(SRT.GetSeqStep() == CSort::scSort) return false ;
                  if(SRT.GetBin() != SRT.GetCntBin()) return false;
                  MoveMotr(miRAL_XIdx , piRAL_XIdxOut );

                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxOut )) return false ;
                  DM.ARAY[riRAL].SetStat(csNone);
                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
                  MoveActr(aiRAL_IdxUpDn, ccFwd);
                  //m_tmTemp.Clear();
//                  IO_SetY(yRAL_Blower,true); //BlowerCycle()에서 꺼줌.
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false;
                  if(!MoveActr(aiRAL_IdxUpDn, ccFwd        )) return false;
                  //if(SRT.GetSeqStep() == CSort::scDetect) return false ;
                  if(!SRT.GetSortEnd()) return false ;
//                  if(!SRT.bSucDetect && bWrkAray) {
//                      EM_SetErr(eiSRT_PkgNotDetected);
//                      Step.iCycle = 0 ;
//                      return true ;
//                  }

                  Step.iCycle = 0 ;
                  return true ;
    }
}

//
////One Cycle.
//bool CRail::CycleWorkIdx(void)
//{
////Check Cycle Time Out.
//    AnsiString sTemp ;
//    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
//        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
//        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//        Trace(m_sPartName.c_str(),sTemp.c_str());
//        Step.iCycle = 0 ;
//        return true;
//    }
//
//    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//    if(Step.iCycle != PreStep.iCycle) {
//        Trace(m_sPartName.c_str(),sTemp.c_str());
//    }
//
//    PreStep.iCycle = Step.iCycle ;
//
//    //if(Stat.bReqStop) {
//    //    Step.iCycle = 0;
//    //    return true;
//    //}
//
//    int iTemp = 0 ;
//    static double dPreTime = GetTickTime();
//    double dCrntTime ;
//    static bool bWrkAray = false;
//    static bool bRALAray = false;
//
//    //Cycle.
//    switch (Step.iCycle) {
//
//        default : if(Step.iCycle) {
//                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//                      Trace(m_sPartName.c_str(), sTemp.c_str());
//                  }
//                  Step.iCycle = 0 ;
//                  return true ;
//
//        case  10: dCrntTime = GetTickTime();
//                  m_dWorkCycleTime = dCrntTime - dPreTime ;
//                  dPreTime = dCrntTime ;
//                  if(DM.ARAY[riRAL].CheckAllStat(csUnkwn)) bRALAray = true   ;
//                  else                                     bRALAray = false  ;
//
////                  if(IO_GetX(xWRK_PkgDoubleChk)){
////                      EM_SetErr(eiWRK_DoubleChip);
////                      Step.iCycle = 0;
////                      return true;
////                  }
//
//                  if(DM.ARAY[riWRK].CheckAllStat(csNone )) {
//                      SRT.SetBin(OM.DevOptn.iEpmtyBin);
//                  }
//
//                       if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt1)) SRT.SetBin(1);
//                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt2)) SRT.SetBin(2);
//                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt3)) SRT.SetBin(3);
//                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt4)) SRT.SetBin(4);
//                  else if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riWRK].CheckAllStat(csRslt5)) SRT.SetBin(5);
//
////                  if(!DM.ARAY[riWRK].CheckAllStat(csNone ) && !IO_GetX(xWRK_PkgExist)) {
////                      SRT.SetBin(OM.DevOptn.iEpmtyBin ,true);
////                  }
////                  if(DM.ARAY[riWRK].CheckAllStat((EN_CHIP_STAT)OM.DevOptn.iEpmtyBin)) {
////                      SRT.SetBin(OM.DevOptn.iEpmtyBin ,true);
////                  }
//
//
////                  if(!DM.ARAY[riWRK].CheckAllStat(csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone ) ) {
//                      if(SRT.GetBin() == 1 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin1FullCnt ) { EM_SetErr(eiSRT_Bin1Full); /*SRT.ClearBinCnt(1);*/ Step.iCycle = 0 ; return true ; }
//                      if(SRT.GetBin() == 2 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin2FullCnt ) { EM_SetErr(eiSRT_Bin2Full); /*SRT.ClearBinCnt(2);*/ Step.iCycle = 0 ; return true ; }
//                      if(SRT.GetBin() == 3 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin3FullCnt ) { EM_SetErr(eiSRT_Bin3Full); /*SRT.ClearBinCnt(3);*/ Step.iCycle = 0 ; return true ; }
//                      if(SRT.GetBin() == 4 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin4FullCnt ) { EM_SetErr(eiSRT_Bin4Full); /*SRT.ClearBinCnt(4);*/ Step.iCycle = 0 ; return true ; }
//                      if(SRT.GetBin() == 5 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin5FullCnt ) { EM_SetErr(eiSRT_Bin5Full); /*SRT.ClearBinCnt(5);*/ Step.iCycle = 0 ; return true ; }
////                  }
//
//
//                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
//                  MoveActr(aiRAL_IdxUpDn, ccFwd);
//                  m_tmTemp.Clear();
//                  Step.iCycle++;
//                  return false;
//
//        case  11: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false ;
//                  if(!MoveActr(aiRAL_IdxUpDn, ccFwd)) return false ;
//                  //if(m_tmTemp.OnDelay(IO_GetX(xRAL_PkgDustChk) || IO_GetX(xWRK_PkgDustChk) ,2000)){//OM.DevOptn.iFeedWaitTime )) {
//                  //    if     (IO_GetX(xRAL_PkgDustChk)) EM_SetErr(eiRAL_DustChip);
//                  //    else if(IO_GetX(xWRK_PkgDustChk)) EM_SetErr(eiWRK_DustChip);
//                  //    Step.iCycle = 0 ;
//                  //    return true ;
//                  //}
//                  //if(IO_GetX(xRAL_PkgDustChk) || IO_GetX(xWRK_PkgDustChk)) return false ;
//                  //MoveActr(aiRAL_IdxUpDn, ccFwd);
//                  //bWrkAray = false ;
//                  //bRALAray = false  ;
//                  Step.iCycle++;
//                  return false ;
//
//        case  12: //if(!MoveActr(aiRAL_IdxUpDn, ccFwd)) return false ;
//                  MoveActr(aiRAL_IdxUpDn , ccBwd);
//                  //if(DM.ARAY[riRAL].CheckAllStat(csUnkwn)) bRALAray = true  ;
////                  if(DM.ARAY[riWRK].CheckAllStat(csNone )) bWrkAray = false ;
////                  else                                     bWrkAray = true  ;
//                  Step.iCycle++;
//                  return false;
//
//        case  13: if(!MoveActr(aiRAL_IdxUpDn , ccBwd)) return false ;
//                  //if(SRT.GetSeqStep() == CSort::scSort) return false ;
//                  if(SRT.GetBin() != SRT.GetCntBin()) return false;
//                  MoveMotr(miRAL_XIdx , piRAL_XIdxOut );
//
//
//                  //DM.ARAY[riWRK].SetStat(csNone);
//                  //DM.ARAY[riRAL].SetStat(csNone);
//                  Step.iCycle++;
//                  return false;
//
//        case  14: //if(MT_GetCmdPos(miRAL_XIdx) >= PM.GetValue(miRAL_XIdx,pvRAL_XIdxOutWait)) {
//                  //    DM.ARAY[riRAL].SetStat(csNone);
//                  //}
//                  if(!MoveMotr(miRAL_XIdx , piRAL_XIdxOut )) return false ;
//                  if(bRALAray) DM.ARAY[riRAL].SetStat(csNone);
//                  //IO_SetY(yRAL_Blower,true); //BlowerCycle()에서 꺼줌.
//                  //if(FED.GetSeqStep() != CFeed::scFeed ) DM.ARAY[riRAL].SetStat(csNone);
//                  //DM.ARAY[riRAL].SetStat(csNone);
////                  if(bRALAray) DM.ARAY[riWRK].SetStat(csUnkwn);
////                  else         DM.ARAY[riWRK].SetStat(csNone );
//
//                  //if(!IO_GetX(xWRK_PkgExist) && DM.ARAY[riWRK].CheckAllStat(csUnkwn)) {
//                  //    DM.ARAY[riWRK].SetStat((EN_CHIP_STAT)OM.DevOptn.iEpmtyBin);
//                  //}
//
//                  //if(bRALAray) DM.ARAY[riWRK].SetStat(csUnkwn);
////                  if(IO_GetX(xWRK_PkgDoubleChk)){
////                      EM_SetErr(eiWRK_DoubleChip);
////                      Step.iCycle = 0;
////                      return true;
////                  }
//                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
//                  MoveActr(aiRAL_IdxUpDn, ccFwd);
//                  //m_tmTemp.Clear();
////                  IO_SetY(yRAL_Blower,true); //BlowerCycle()에서 꺼줌.
//                  Step.iCycle++;
//                  return false ;
//
//        case  15: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false;
//                  if(!MoveActr(aiRAL_IdxUpDn, ccFwd        )) return false;
//                  //if(SRT.GetSeqStep() == CSort::scDetect) return false ;
//                  if(!SRT.GetSortEnd()) return false ;
////                  if(!SRT.bSucDetect && bWrkAray) {
////                      EM_SetErr(eiSRT_PkgNotDetected);
////                      Step.iCycle = 0 ;
////                      return true ;
////                  }
//
//                  Step.iCycle = 0 ;
//                  return true ;
//    }
//}



double CRail::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRail::CheckStop()
{
    if(!MT_GetStop(miRAL_XIdx)) return false ;

    if(!AT_Done(aiRAL_IdxUpDn )) return false ;

    return true ;
}

void CRail::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        //UserINI.Load(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
    }
    else {
        //UserINI.ClearFile(sPath) ;
        //UserINI.Save(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
    }
}
//---------------------------------------------------------------------------
