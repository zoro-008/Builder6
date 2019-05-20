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
    bFedStt = false ;
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
            case piRAL_XIdxWork     : dPos = PM.GetValue (_iMotr , pvRAL_XIdxWork     ); break ;
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
            case piRAL_XIdxWork     : if(!isNotWaitPrb) {sMsg = "Probe Z Axis is Not Wait Position" ; bRet = false ;} break ;
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

    //bool isPkgDustPos = IO_GetX(xRAL_PkgDustChk);

    //if(_iActr == aiRAL_Index && _bFwd){//bRet){
    //    if(isPkgDustPos) {
    //        sMsg = "Rail Index Is Not Fwd" ;
    //        bRet = false ;
    //    }
    //}

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

                 AT_MoveCyl(aiRAL_Index  , ccFwd);
                 AT_MoveCyl(aiRAL_Stopper , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiRAL_Index   , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_Stopper , ccBwd)) return false ;
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

        case 10: bFedStt = false ;
                 MoveActr(aiRAL_Index  , ccFwd);
//                 bFirst = true ;
                 IO_SetY(yRAL_AirBlower , true);
                 //IO_SetY(y)

                 m_iBlowerStep = 0 ;

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiRAL_Index  ,ccFwd)) return false ;
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

        case 10: MoveActr(aiRAL_Index  , ccFwd);
                 IO_SetY(yRAL_AirBlower , false);
                 m_iBlowerStep = 0 ;
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiRAL_Index  ,ccFwd)) return false ;
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

        case 0 : if(!IO_GetY(yRAL_AirBlower)) return ;
                 BlowerTimer.Clear();
                 m_iBlowerStep++;
                 return ;

        case 1 : if(!BlowerTimer.OnDelay(true , OM.DevOptn.iBlowerDelay)) return ;
                 IO_SetY(yRAL_AirBlower , false );
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

        if(DM.ARAY[riRAL].CheckAllStat(csNone) && DM.ARAY[riRAB].CheckAllStat(csNone) && IO_GetX(xRAL_Pkg)) DM.ARAY[riRAL].SetStat(csUnkwn);

        bool bFinishInsp    = !DM.ARAY[riWRK].CheckAllStat(csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone );
        bool bFirstInsp     =  DM.ARAY[riRAL].CheckAllStat(csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csUnkwn);// && IO_GetX(xRAL_Pkg);

        bool bisInsp        =  WRK.GetSeqStep() == CWork::scWork ;
//        bool bisFeeding    =  false ;
        bool bPrbWaitPs     =  MT_CmprPos(miWRK_ZPrb,PM.GetValue(miWRK_ZPrb , pvWRK_ZPrbWait));
        bool bDustPs        =  IO_GetX(xRAL_PkgDustChk);
        bool bisFeed        =  FED.GetSeqStep() == CFeed::scFeed;
        //MT_GetCmdPos(miWRK_ZPrb) == PM.GetValue(miWRK_ZPrb , pvWRK_ZPrbWait);

        //Judgment
        //bool isCycleIn        = bRailIn ;
        bool isCycleMove      = (bFirstInsp || bFinishInsp  ) && bPrbWaitPs && !bisInsp && !bDustPs && !bisFeed;
        bool isCycleWorkScnd  = false ;//(bFinishInsp ) && bPrbWaitPs && !bisFeeding; 안씀. 만들다 맘 혹시나 해서 아직 있음.
        bool isCycleWorkThrd  = false ;//(bFinishInsp ) && bPrbWaitPs && !bisFeeding; 안씀.

        bool isConEnd         = !isCycleMove;

        //자재 파손 위치 에러.
        if(!DM.ARAY[riRAL].CheckAllStat(csNone) && IO_GetX(xRAL_PkgDustChk)) EM_SetErr(eiRAL_DustChip) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             //if (isCycleIn  ) {Trace(m_sPartName.c_str(),"isCycleIn   Stt"); Step.iSeq = scIn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
             if (isCycleMove   ) {Trace(m_sPartName.c_str(),"isCycleMove Stt"); Step.iSeq = scMove ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                   Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                    return false ;
        //case scIn      : if(CycleIn   ()){ Trace(m_sPartName.c_str(),"CycleIn   End");Step.iSeq = scIdle ;} return false ;
        case scMove    : if(CycleMove ()){ Trace(m_sPartName.c_str(),"CycleMove End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.
bool CRail::CycleIn(void)
{

}

//One Cycle.
bool CRail::CycleMove(void)
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
    //static bool bWrkAray = false;
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

                  if(SRT.GetBin() == 1 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin1FullCnt ) { SRT.SetBin(2); DM.ARAY[riWRK].SetStat(csRslt2);}
                  if(SRT.GetBin() == 2 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin2FullCnt ) { EM_SetErr(eiSRT_PkgGoodFull); /*SRT.ClearBinCnt(2);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 3 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin3FullCnt ) { EM_SetErr(eiSRT_Bin3Full   ); /*SRT.ClearBinCnt(3);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 4 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin4FullCnt ) { EM_SetErr(eiSRT_Bin4Full   ); /*SRT.ClearBinCnt(4);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 5 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin5FullCnt ) { EM_SetErr(eiSRT_Bin5Full   ); /*SRT.ClearBinCnt(5);*/ Step.iCycle = 0 ; return true ; }

                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
                  MoveActr(aiRAL_Index, ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(IO_GetX(xRAL_PkgDustChk)) return false;

                  MoveActr(aiRAL_Index , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(MM.GetManNo() == mcNoneCycle) {
                      if(SRT.GetCntBin() != SRT.GetBin()) return false ;
                  }

                  // 잠시 주석.

                  if(!MoveActr(aiRAL_Index , ccBwd)) return false ;

                  Step.iCycle++;
                  return false;

        case  14: MoveMotr(miRAL_XIdx , piRAL_XIdxWork );
                  DM.ShiftArrayData(riRAL, riRAB);
                  //DM.ARAY[riRAL].SetStat(csNone);

                  //if(DM.ARAY[riRAB].CheckAllStat(csNone)) bWrkAray = false;  //여기 지우고 센서로 감지 하는 시퀀스로 수정 해야 함
                  //else                                    bWrkAray = true ;

                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWork )) return false ;
                  if(!DM.ARAY[riRAB].GetCntStat(csNone) && !IO_GetX(xWRK_Detect1)){
                       DM.ShiftArrayData(riRAB , riWRK);
                       DM.ARAY[riWRK].SetStat(csNone);
                       EM_SetErr(eiWRK_PkgDispr);
                       Step.iCycle = 0;
                       return true;
                  }
                  if( IO_GetX(xWRK_Detect1)) DM.ShiftArrayData(riRAB , riWRK);
                  else                       DM.ARAY[riWRK].SetStat(csNone);


                  //if(bWrkAray) DM.ARAY[riWRK].SetStat(csUnkwn);
                  //else         DM.ARAY[riWRK].SetStat(csNone );

                  DM.ARAY[riRAB].SetStat(csNone); //피딩 여기서 오게 하고.

                  MoveMotr(miRAL_XIdx , piRAL_XIdxWait );
                  MoveActr(aiRAL_Index, ccFwd);

                  //일단 보류....  if(!DM.ARAY[riWRK ].CheckAllStat(csNone) && !IO_GetX(xWRK_PkgExist)) EM_SetErr(eiWRK_PkgDispr   ) ;

                  Step.iCycle++;
                  return false;


        case  16: if(!MoveMotr(miRAL_XIdx , piRAL_XIdxWait )) return false;
                  if(!MoveActr(aiRAL_Index, ccFwd        )) return false;

                  //if(SRT.GetSeqStep() == CSort::scDetect) return false ;

                  if(MM.GetManNo() == mcNoneCycle) {
                      if(!SRT.GetSortEnd()) return false ;
                  }
                  // 잠시 주석... JS

//                  if(!SRT.bSucDetect && bWrkAray) {
//                      EM_SetErr(eiSRT_PkgNotDetected);
//                      Step.iCycle = 0 ;
//                      return true ;
//                  }

                  //if(SRT.GetBin() == 1 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin1FullCnt ) { EM_SetErr(eiSRT_PkgGoodFull); /*SRT.ClearBinCnt(1);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 2 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin2FullCnt ) { EM_SetErr(eiSRT_PkgGoodFull); /*SRT.ClearBinCnt(2);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 3 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin3FullCnt ) { EM_SetErr(eiSRT_Bin3Full   ); /*SRT.ClearBinCnt(3);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 4 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin4FullCnt ) { EM_SetErr(eiSRT_Bin4Full   ); /*SRT.ClearBinCnt(4);*/ Step.iCycle = 0 ; return true ; }
                  if(SRT.GetBin() == 5 && SRT.GetBinCnt(SRT.GetBin()) >= OM.DevOptn.iBin5FullCnt ) { EM_SetErr(eiSRT_Bin5Full   ); /*SRT.ClearBinCnt(5);*/ Step.iCycle = 0 ; return true ; }

                  Step.iCycle = 0 ;
                  return true ;


    }
}


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

    if(!AT_Done(aiRAL_Index )) return false ;

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
