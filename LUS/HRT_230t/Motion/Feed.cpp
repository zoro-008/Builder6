3//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Feed.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "Rail.h"
#include "Heater.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CFeed FED;

CFeed::CFeed()
{
    Init();
}
CFeed::~CFeed (void)
{
    Close();
}
void CFeed::Init()
{
    m_sPartName   = "Feed " ;
    bConEnd       = false;

    m_iHomeTO  = (int)eiFED_HomeTO    ;
    m_iStartTO = (int)eiFED_ToStartTO ;
    m_iStopTO  = (int)eiFED_ToStopTO  ;
    m_iCycleTO = (int)eiFED_CycleTO   ;

    m_iFeedFailed = 0;

    Reset();
    Load(true);
}
void CFeed::Close()
{
    Load(false);
}
void CFeed::Reset()
{
    ResetTimer();

    iFeedCount = 0;

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CFeed::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CFeed::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    //int r1,c1;
    //if(_riWhre == riRAE) {
    //    c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
    //    return (c > -1 && c > -1);
    //}
    return false ;
}

double CFeed::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    //if(_iMotr == miFED_TFed){
    //    switch(_iPstnId) {
    //        default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
    //        case piFED_TFedWait : dPos = PM.GetValue (_iMotr , pvFED_TFedWait ); break ;
    //        case piFED_TFedWork : dPos = PM.GetValue (_iMotr , pvFED_TFedWork ); break ;
    //    }
    //}
    //
    //else {
    //    dPos = MT_GetCmdPos(_iMotr);
    //}

    return dPos ;
}

//---------------------------------------------------------------------------
bool CFeed::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CFeed::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
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

bool CFeed::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CFeed::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CFeed::CycleHome()     //sun DLL direct access.
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

        case 10: AT_MoveCyl(aiFED_StpFwBw , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiFED_StpFwBw , ccBwd)) return false;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CFeed::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CFeed::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    bool bRet = true ;

//    if(!OM.CmnOptn.bIgnrHeatErr) {
        if(OM.DevOptn.iTempSet1 + OM.DevOptn.iTempGap < HTR.GetTemp1() ) bRet = false ;
        if(OM.DevOptn.iTempSet1 - OM.DevOptn.iTempGap > HTR.GetTemp1() ) bRet = false ;

        if(OM.DevOptn.iTempSet2 + OM.DevOptn.iTempGap < HTR.GetTemp2() ) bRet = false ;
        if(OM.DevOptn.iTempSet2 - OM.DevOptn.iTempGap > HTR.GetTemp2() ) bRet = false ;
//    }

    if(!bRet) return false ;


    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CFeed::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: m_tmNoPkg.Clear();
                 m_tmNoPkgDust.Clear();
                 bConEnd = false;
                 MoveActr(aiFED_StpFwBw , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiFED_StpFwBw , ccBwd)) return false;
                 //IO_SetY(yFED_Vacuum,false);

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CFeed::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: //MT_Stop(miFED_TFed);
                 //IO_SetY(yFED_Vacuum,false);
                 MoveActr(aiFED_StpFwBw , ccBwd);
                 Step.iToStop++;
                 return false;

        case 11: if(!MoveActr(aiFED_StpFwBw , ccBwd)) return false;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CFeed::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Judgment
//        bool isCycleFeed = DM.ARAY[riRAL].CheckAllStat(csNone) && !OM.CmnOptn.bFeedingStop && ( RAL.GetSeqStep() == CRail::scWorkFst || DM.ARAY[riWRK].CheckAllStat(csNone) )&&
//                           IO_GetX(xRAL_PkgInChk) && AT_Done(aiRAL_IdxUpDn) && !IO_GetX(xRAL_PkgDustChk) ;

        //bool isCycleFeed = DM.ARAY[riRAL].CheckAllStat(csNone) && !OM.CmnOptn.bFeedingStop && ( RAL.bFedStt || DM.ARAY[riWRK].CheckAllStat(csNone) )&&
        //                   IO_GetX(xRAL_PkgInChk) && AT_Done(aiRAL_IdxUpDn) && !IO_GetX(xRAL_PkgDustChk) ;


        bool isCycleFeed = DM.ARAY[riRAL].CheckAllStat(csNone ) && IO_GetX(xRAL_PkgInChk) && (!IO_GetX(xRAL_Pkg) && !IO_GetX(xRAL_PkgDustChk));

        //bool isFeed      = DM.ARAY[riRAL].CheckAllStat(csNone) && !OM.CmnOptn.bFeedingStop && //RAL.GetSeqStep() != CRail::scWorkIdx &&
        //                   IO_GetX(xRAL_PkgInChk) && AT_Done(aiRAL_IdxUpDn) &&  IO_GetX(xRAL_PkgDustChk) ;

//                           DM.ARAY[riRLO].CheckAllStat(csNone);
        bool isConEnd    = bConEnd ;//!isCycleFeed ;

        //자제 없음.

        //if(m_tmNoPkgDust.OnDelay( isFeed && !OM.CmnOptn.bFeedingStop, 3000)) {
        //    EM_SetErr(eiRAL_DustChip);
        //    bConEnd = true ;
        //}

        if(isCycleFeed) m_tmNoPkg.Clear();// 자재가 일정 시간 안들어오면 Error
        
        if(m_tmNoPkg.OnDelay( !isCycleFeed && !OM.CmnOptn.bFeedingStop, OM.DevOptn.iNoPkgSec * 1000)) {
            EM_SetErr(eiRAL_NoPkg);
            bConEnd = true ;
        }

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleFeed ) {Trace(m_sPartName.c_str(),"CycleFeed  Stt"); Step.iSeq = scFeed ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
//             if (isCycleFeed ) {Trace(m_sPartName.c_str(),"CycleFeed  Stt"); Step.iSeq = scFeed ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                   return false ;
        case scFeed    : if(CycleFeed()){ Trace(m_sPartName.c_str(),"CycleFeed End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.
bool CFeed::CycleFeed(void)
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

    //static bool bRAL_PkgPassed = false ;
    //if(IO_GetXDn(xRAL_Pkg)){
    //    bRAL_PkgPassed = true ;
    //
    //}



    //Cycle.
    switch (Step.iCycle) {
        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;


        case  10: //bRAL_PkgPassed = false ;
                  MoveActr(aiFED_StpFwBw , ccBwd);
                  m_iFeedFailed++;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiFED_StpFwBw , ccBwd)) return false;
                  MoveActr(aiFED_StpFwBw , ccFwd);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiFED_StpFwBw , ccFwd)) return false;
                  if(m_tmTemp.OnDelay(IO_GetX(xRAL_Pkg), 2*1000)){
                      if(m_iFeedFailed > OM.DevOptn.iFedFailCnt){
                          EM_SetErr(eiFED_FeedFailed);
                          m_iFeedFailed = 0;
                          Step.iCycle = 0;
                          return true;
                      }
                      else {
                          Step.iCycle = 10;
                          return false;
                      }
                  }
                  if(IO_GetX(xRAL_Pkg)) return false;
                  m_iFeedFailed = 0;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  13: //if(m_tmTemp.OnDelay(xRAL_Pkg , 1000)){
                  //    EM_SetErr(eiRAL_PkgJam);
                  //    Step.iCycle = 0;
                  //    return true;
                  //}
                  //if(!bRAL_PkgPassed) return false;
                  //if(m_tmTemp.OnDelay(!IO_GetX(xRAL_Pkg), 1000)){   기존..JS
                  //    EM_SetErr(eiRAL_PkgJam);
                  //    Step.iCycle = 0;
                  //    return true;
                  //}
                  //if(!IO_GetX(xRAL_Pkg)) return false;
                  Step.iCycle++;
                  return false;

        case  14: MoveActr(aiFED_StpFwBw , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiFED_StpFwBw , ccBwd)) return false;
                  DM.ARAY[riRAL].SetStat(csUnkwn);
                  Step.iCycle = 0;
                  return true;

    }
}

/*

*/
//One Cycle.
//    20140625 선계원 수정전.
//    bool CFeed::CycleFeed(void)
//    {
//    //Check Cycle Time Out.
//        AnsiString sTemp ;
//        if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
//            EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
//            sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//            Trace(m_sPartName.c_str(),sTemp.c_str());
//            Step.iCycle = 0 ;
//            return true;
//        }
//
//        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//        if(Step.iCycle != PreStep.iCycle) {
//            Trace(m_sPartName.c_str(),sTemp.c_str());
//        }
//
//        PreStep.iCycle = Step.iCycle ;
//
//
//
//        //Cycle.
//        switch (Step.iCycle) {
//            default : if(Step.iCycle) {
//                          sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//                          Trace(m_sPartName.c_str(), sTemp.c_str());
//                      }
//                      Step.iCycle = 0 ;
//                      return true ;
//    
//    
//            case  10: bRAL_PkgPassed = false ;
//                      MoveActr(aiFED_StpFwBw , ccBwd);
//                      m_iFeedFailed++;
//                      Step.iCycle++;
//                      return false;
//
//            case  11: if(!MoveActr(aiFED_StpFwBw , ccBwd)) return false;
//                      MoveActr(aiFED_StpFwBw , ccFwd);
//                      m_tmTemp.Clear();
//                      Step.iCycle++;
//                      return false;
//    
//            case  12: if(!MoveActr(aiFED_StpFwBw , ccFwd)) return false;
//                      if(m_tmTemp.OnDelay(IO_GetX(xRAL_PkgInChk), 500)){
//                          if(m_iFeedFailed > OM.DevOptn.iFedFailCnt){
//                              EM_SetErr(eiFED_FeedFailed);
//                              m_iFeedFailed = 0;
//                              Step.iCycle = 0;
//                              return true;
//                          }
//                          else {
//                              Step.iCycle = 10;
//                              return false;
//                          }
//                      }
//                      if(IO_GetX(xRAL_PkgInChk)) return false;
//                      m_iFeedFailed = 0;
//                      m_tmTemp.Clear();
//                      Step.iCycle++;
//                      return false;
//
//            case  13: if(m_tmTemp.OnDelay(xRAL_Pkg , 1000)){
//                          EM_SetErr(eiRAL_PkgJam);
//                          Step.iCycle = 0;
//                          return true;
//                      }
//                      if(!IO_GetXDn(xRAL_Pkg)) return false;
//                      //if(m_tmTemp.OnDelay(!IO_GetX(xRAL_Pkg), 1000)){   기존..JS
//                      //    EM_SetErr(eiRAL_PkgJam);
//                      //    Step.iCycle = 0;
//                      //    return true;
//                      //}
//                      //if(!IO_GetX(xRAL_Pkg)) return false;
//                      Step.iCycle++;
//                      return false;
//
//            case  14: MoveActr(aiFED_StpFwBw , ccBwd);
//                      Step.iCycle++;
//                      return false;
//    
//            case  15: if(!MoveActr(aiFED_StpFwBw , ccBwd)) return false;
//                      DM.ARAY[riRAL].SetStat(csUnkwn);
//                      Step.iCycle = 0;
//                      return true;
//    
//            /*
//            case  10: IO_SetY(yFED_Vacuum,true); // 추가 IO 피딩때 두개 들어간다고 뒤에꺼 잡기용.
//                      //민수형 요청 옵션 추가. 센서 감지 햇을때만 마스킹.
//                      bPkgDustChkDn = false ;
//    //                  iFeedCount    = 0 ;
//                      //
//                      m_tmTemp.Clear();
//                      Step.iCycle++;
//                      return false;
//    
//            case  11: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iFedVacuumDelay)) return false ;
//    
//                      //MT_JogP(miFED_TFed);
//                      if(MM.GetManNo() == mcNoneCycle) {
//                          if(IO_GetX(xRAL_PkgDustChk)) {
//                              EM_SetErr(eiRAL_DustChip);
//                              IO_SetY(yFED_Vacuum,false); // 추가 IO 피딩때 두개 들어간다고 뒤에꺼 잡기용.
//                              Step.iCycle = 0 ;
//                              return true ;
//                          }
//                      }
//    
//                      MT_SetPos(miFED_TFed,0);
//                      MoveMotr(miFED_TFed,piFED_TFedWork);
//                      iCnt ++ ;
//                      sTemp = iCnt ;
//                      Trace(m_sPartName.c_str(),sTemp.c_str());
//                      //MT_GoAbsRun(miFED_TFed,120);
//                      //m_tmTemp.Clear();
//                      //아 센서 위치에 따라 살짝 들어왓다가 꺼진다고 해서...추가. 예외처리.dd
//                      if(!OM.MstOptn.bIgnrPkgDustChk){
//                          if(OM.DevOptn.dFedMinMove > PM.GetValue(miFED_TFed,pvFED_TFedWork)) {
//                              OM.DevOptn.dFedMinMove = PM.GetValue(miFED_TFed,pvFED_TFedWork) ;
//                              bFedMTPsCheck = false;
//                          }
//                          else bFedMTPsCheck = true;
//                      }
//                      if(iFeedCount > OM.DevOptn.iFedFailCnt){
//                          EM_SetErr(eiFED_FeedFailed);
//                          Step.iCycle = 0;
//                          return true;
//                      }
//                      Step.iCycle++;
//                      return false;
//    
//            case  12: if(IO_GetXDn(xRAL_PkgDustChk)) bPkgDustChkDn = true ;
//                      if(bFedMTPsCheck){
//                          if(bPkgDustChkDn && MT_GetCmdPos(miFED_TFed) > OM.DevOptn.dFedMinMove){
//                              MT_EmgStop(miFED_TFed);
//                              Step.iCycle++;
//                              return false;
//                          }
//                          else if(!bPkgDustChkDn && MT_GetCmdPos(miFED_TFed) >= PM.GetValue(miFED_TFed , pvFED_TFedWork)){
//                              Step.iCycle++;
//                              return false;
//                          }
//                          else {
//                              return false;
//                          }
//                      }
//                          //if(IO_GetXDn(xRAL_PkgDustChk) && ( MT_GetCmdPos(miFED_TFed) > (OM.DevOptn.dFedMinMove )) ){ //아 센서 위치에 따라 살짝 들어왓다가 꺼진다고 해서...추가.
//                          //if(IO_GetXDn(xRAL_PkgDustChk)){
//                               //다음 자제 빨려 나갈까봐. AND 메뉴얼에서 사용 하려면 계속 돌면 안된다.
//                      //else {
//                      //    MT_SetPos(miFED_TFed,0);
//                      //    //DM.ARAY[riRAL].SetStat(csUnkwn);
//                      //    bConEnd = false;
//                      //    Step.iCycle=0;
//                      //    return true ;
//                      //}
//                      //else if (!MT_GoAbsRun(miFED_TFed,120)) return false;
//                      if(OM.MstOptn.bIgnrPkgDustChk) bPkgDustChkDn = true ;
//                      if(!MoveMotr   (miFED_TFed , piFED_TFedWork        )) return false ;
//
//                      Step.iCycle++;
//                      return false;
//    
//            case  13: //MT_ClearPos(miFED_TFed);
//                      MT_SetPos(miFED_TFed,0);
//                      if(bPkgDustChkDn){
//                          //IO_SetY(yFED_Vacuum,false); // 추가 IO 피딩때 두개 들어간다고 뒤에꺼 잡기용. 너무 짧은거 같다고 민수형이...흠냐...밑으로 내림
//                          m_tmTemp.Clear();
//                          Step.iCycle++;
//                          return false;
//                      }
//                      else {
//                          iFeedCount += 1;
//                          Step.iCycle = 10;
//                          return false;
//                      }
//
//            case  14: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iFeedComDelay)) return false ;
//                      IO_SetY(yFED_Vacuum,false); // 추가 IO 피딩때 두개 들어간다고 뒤에꺼 잡기용. 너무 짧은거 같다고 민수형이...흠냐...밑으로 내림
//                      if(bPkgDustChkDn) DM.ARAY[riRAL].SetStat(csUnkwn);
//                      iFeedCount = 0;
//                      bConEnd = false;
//                      Step.iCycle=0;
//                      return true ;
//
//    
//    //                  MT_EmgStop(miFED_TFed); //다음 자제 빨려 나갈까봐. AND 메뉴얼에서 사용 하려면 계속 돌면 안된다.
//    //                  Step.iCycle++;
//    //                  return false ;
//    //
//    //        case  12:
//    //                  DM.ARAY[riRAL].SetStat(csUnkwn);
//    //                  bConEnd = false;
//    //                  Step.iCycle=0;
//    //                  return true ;
//                        */
//        }
//    }

/*
//One Cycle.
bool CFeed::CycleFeed(void)
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

    bool      r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {
        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //MT_JogP(miFED_TFed);
                  if(IO_GetX(xRAL_PkgDustChk)) {
                      EM_SetErr(eiRAL_DustChip);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  MoveMotr(miFED_TFed,piFED_TFedWork);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: //if(IO_GetX(xRAL_PkgInChk)) {
                  //    MT_EmgStop(miFED_TFed); //다음 자제 빨려 나갈까봐.
                  //    m_tmNoPkg.Clear(); //노페키지 타이머 클리어.
                  //    m_tmTemp.Clear();
                  //    Step.iCycle++;
                  //    return false ;
                  //}
                  //if(!m_tmTemp.OnDelay(true, 1000))return false ;
                  //
                  //자제 없음.
                  //if(m_tmNoPkg.OnDelay(Step.iSeq == scFeed , OM.DevOptn.iNoPkgSec * 1000)) {
                  //    EM_SetErr(eiRAL_NoPkg);
                  //}
                  //                        //나중에 문제 될듯 함...... 봐서 에어 브로우어 달아야 할듯.
                  //MT_EmgStop(miFED_TFed); //다음 자제 빨려 나갈까봐. AND 메뉴얼에서 사용 하려면 계속 돌면 안된다.
                  //
                  //Step.iCycle=0;
                  //if(!m_tmTemp.OnDelay(true, OM.DevOptn.iFeedMtMoveTime))return false ;
                  if(!MoveMotr(miFED_TFed,piFED_TFedWork)) return false ;
                  //MT_ClearPos(miFED_TFed);
                  MT_SetPos(miFED_TFed,0);


//                  MT_EmgStop(miFED_TFed); //다음 자제 빨려 나갈까봐. AND 메뉴얼에서 사용 하려면 계속 돌면 안된다.
                  Step.iCycle++;
                  return false ;

        case  12:
                  DM.ARAY[riRAL].SetStat(csUnkwn);
                  bConEnd = false;
                  Step.iCycle=0;
                  return true ;
    }
}
*/
double CFeed::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CFeed::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CFeed::CheckStop()
{
    //if(!MT_GetStop(miFED_TFed)) return false ;

    //if(!AT_Done(aiFED_BAlign )) return false ;

    return true ;
}

void CFeed::Load(bool IsLoad)
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
