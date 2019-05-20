//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LearLaser.h"
#include "MainThread.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "SPCUnit.h"
#include "LaserConUnit.h"
#include "LotUnit.h"
#include "LearFeed.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CLearLaser LLS;

CLearLaser::CLearLaser(void)
{

}

CLearLaser::~CLearLaser (void)
{

}

void CLearLaser::Init()
{
    m_sPartName = "Lear Laser" ;
    Reset();
    Load(true);

    m_bTestMark = false;
    m_bAutoRun  = false;

    InitCycleName();
}
void CLearLaser::Close()
{
    Load(false);
}




void CLearLaser::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    //IO_SetY(yVSC_AlnStart   , false);
    //IO_SetY(yLSC_MarkEnable , false);
    //IO_SetY(yLSC_DataRead   , false);
    //IO_SetY(yLSC_Reset      , false);
    //IO_SetY(yLSC_Start      , false);

    //if((!IO_GetX(xVSC_OriReady) && !OM.CmnOptn.bSkipOriVisn) ||
    //   (!IO_GetX(xVSC_AlnReady) && !OM.CmnOptn.bSkipAlnVisn) ||
    //   (!IO_GetX(xVSC_MrkReady) && OM.CmnOptn.iMarkInsp != moNone) ){
    //    CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnReset) ;
    //}


    /*
    if((!IO_GetX(xVSC_OriBusy) && !OM.CmnOptn.bSkipOriVisn) ||
       (!IO_GetX(xVSC_AlnBusy) && !OM.CmnOptn.bSkipAlnVisn) ||
       (!IO_GetX(xVSC_MrkBusy) &&  OM.CmnOptn.iMarkInsp != moNone) ){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnReset) ;
    }
    */



}

void CLearLaser::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

bool CLearLaser::FindChip(int &r , int &c )
{
    r = -1 ;
    c = -1 ;

    return (c > -1 && c > -1);
}

double CLearLaser::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    if(_iMotr == miLSR_TLer){
        switch(_iPstnId) {
            default              : dPos = MT_GetCmdPos(_iMotr                   ); break ;
            case piLSR_TLerWait  : dPos = PM.GetValue (_iMotr , pvLSR_TLerWait  ); break ;
            case piLSR_TLerPos1  : dPos = PM.GetValue (_iMotr , pvLSR_TLerPos1  ); break ;
            case piLSR_TLerPos2  : dPos = PM.GetValue (_iMotr , pvLSR_TLerPos2  ); break ;
            case piLSR_TLerPos3  : dPos = PM.GetValue (_iMotr , pvLSR_TLerPos3  ); break ;
            case piLSR_TLerPos4  : dPos = PM.GetValue (_iMotr , pvLSR_TLerPos4  ); break ;
            case piLSR_TLerPos5  : dPos = PM.GetValue (_iMotr , pvLSR_TLerPos5  ); break ;
            case piLSR_TLerPlace : dPos = PM.GetValue (_iMotr , pvLSR_TLerPlace ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CLearLaser::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bLearStgFwd    = !AT_Complete(aiLSR_StgL_FwBw , ccBwd);

    if(_iMotr == miLSR_TLer){
        if( bLearStgFwd   ) {sMsg = "Stage Cylinder is FWD"; bRet = false ;}
        switch(_iPstnId) {
            default              :  break ;
            case pvLSR_TLerWait  :  break ;
            case pvLSR_TLerPos1  :  break ;
            case pvLSR_TLerPos2  :  break ;
            case pvLSR_TLerPos3  :  break ;
            case pvLSR_TLerPos4  :  break ;
            case pvLSR_TLerPos5  :  break ;
            case pvLSR_TLerPlace :  break ;

        }
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLearLaser::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg ;

    double dLearTCrntPos = MT_GetCmdPos(miLSR_TLer) ;
    double dLearTWaitPos = GetMotrPos(miLSR_TLer , piLSR_TLerWait) ;
    double dOffset       = OM.MstOptn.dOffset ;
    double dLearYCrntPos = MT_GetCmdPos(miFED_YLer) ;
    double dLearYFeedPos = GetMotrPos(miFED_YLer , piFED_YLerFeed ) ;

    bool bLearTWaitPos = (dLearTWaitPos - dOffset <= dLearTCrntPos) && (dLearTCrntPos <= dLearTWaitPos + dOffset) ;
    bool bLearYFeedPos = (dLearYFeedPos - dOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYFeedPos + dOffset) ;

    bool bCoverFwd     = !AT_Complete(aiFED_Covr_FwBw, ccBwd) ;

    if(_iActr == aiLSR_StgL_FwBw) {
        if( bCoverFwd    ) { sMsg = "Cover is Not BWD"             ; bRet = false ;}
        if(!bLearTWaitPos) { sMsg = "Lear T Moter is Not Wait Pos" ; bRet = false ;}
        if(_bFwd) {
            if(!bLearYFeedPos) { sMsg = "Lear Y Moter is Not Feed Pos" ; bRet = false ;}
        }
    }
    else if(_iActr == aiLSR_StgF_FwBw){}
    else if(_iActr == aiFED_Algn_FwBw){}
    else if(_iActr == aiFED_Covr_FwBw){}

    else {
      sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
      bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CLearLaser::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLearLaser::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLearLaser::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 15000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiLSR_HomeTo);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    if((!IO_GetX(xLSR_FStgBw) || !IO_GetX(xLSR_LStgBw)) && (Step.iHome > 11)){
        FM_MsgOk("WARNING","Stage Cylinder is Not Bwd");
        Step.iCycle = 0 ;
        return true;
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


        case 10: IO_SetY(yLSR_VccL , true);
                 AT_MoveCyl(aiFED_Algn_FwBw , ccBwd);
                 AT_MoveCyl(aiFED_Covr_FwBw , ccBwd);

                 Step.iHome++;
                 return false;

        case 11: if(!AT_MoveCyl(aiFED_Algn_FwBw , ccBwd)) return false;
                 if(!AT_MoveCyl(aiFED_Covr_FwBw , ccBwd)) return false;

                 AT_MoveCyl(aiLSR_StgL_FwBw , ccBwd);

                 Step.iHome++;
                 return false;

        case 12: if(!AT_MoveCyl(aiLSR_StgL_FwBw , ccBwd)) return false;

                 MT_SetHomeEnd(miLSR_TLer , false);
                 MT_DoHome(miLSR_TLer);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miLSR_TLer)) return false;

                 MT_GoAbsMan(miLSR_TLer, PM.GetValue(miLSR_TLer , pvLSR_TLerWait));
                 if(DM.ARAY[riLLS].CheckAllStat(csNone) && DM.ARAY[riLFD].CheckAllStat(csNone)) IO_SetY(yLSR_VccL , false);

                 Step.iHome = 0;
                 return true ;

    }
}

bool CLearLaser::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    // 작업자 카운트 추가 하고 초기화 가능 하도록 JH


    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        double dLearYCrntPos = MT_GetCmdPos(miFED_YLer) ;
        double dLearYWorkPos = GetMotrPos(miFED_YLer , piFED_YLerWork) ;
        double dOffset       = OM.MstOptn.dOffset ;

        bool bFEDMtrWaitPos  = dLearYWorkPos - dOffset <= dLearYCrntPos && dLearYCrntPos <= dLearYWorkPos + dOffset ;

        bool bLotOpenOutCon = LTQ.GetLotOpen() && ( DM.ARAY[riLLS].CheckAllStat(csWork ) && bFEDMtrWaitPos) ;
        bool bLotEndOutCon  =!LTQ.GetLotOpen() && ( DM.ARAY[riLLS].CheckAllStat(csWait ) && bFEDMtrWaitPos) ;

        //bool isCycleWork    = LTQ.GetLotOpen() && ( DM.ARAY[riLLS].CheckAllStat(csUnkwn) && bIDXMtrWaitPos) ;

        bool isCycleWork    = LTQ.GetLotOpen() && ( DM.ARAY[riLLS].CheckAllStat(csWait ) && bFEDMtrWaitPos &&
                                                   !DM.ARAY[riFLS].GetCntStat  (csWait )                  ) ;

        bool isCyclePlace   = (bLotOpenOutCon || bLotEndOutCon) ;

        bool isCycleEnd     = DM.ARAY[riLLS].CheckAllStat(csNone);

        //랏 오픈이 아닐때도 타긴 타야 한다...  JH 스타트를
        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork Stt" ); Step.iSeq = scWork  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; m_bAutoRun = true;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace Stt"); Step.iSeq = scPlace ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; m_bAutoRun = true;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                   Trace(m_sPartName.c_str(),"default   End"  );Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                      return false ;
        case scWork  : if(CycleWork ()){ Trace(m_sPartName.c_str(),"CycleWork End"  );Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace()){ Trace(m_sPartName.c_str(),"CyclePlacce End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CLearLaser::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CLearLaser::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLearLaser::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: //MoveActr(aiLSR_StgF_FwBw , ccBwd);
                 //MoveActr(aiLSR_StgB_FwBw , ccBwd);

                 Step.iToStart++;
                 return false ;

        case 11: //if(!MoveActr(aiLSR_StgF_FwBw , ccBwd)) return false ;
                 //if(!MoveActr(aiLSR_StgB_FwBw , ccBwd)) return false ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLearLaser::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 20000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiLSR_ToStopTO);

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

        case 10: if(DM.ARAY[riLFD].GetCntStat(csUnkwn) || DM.ARAY[riLFD].GetCntStat(csGood )||
                    DM.ARAY[riLFD].GetCntStat(csWait ) || DM.ARAY[riLLS].GetCntStat(csWait )||
                    DM.ARAY[riLLS].GetCntStat(csWork ) ){

                     IO_SetY(yLSR_VccL , true);
                 }


                 //MoveActr(aiFED_Algn_FwBw , ccBwd);
                 //MoveActr(aiFED_Covr_FwBw , ccBwd);

                 Step.iToStop++;
                 return false;

        case 11: //if(!MoveActr(aiFED_Algn_FwBw , ccBwd)) return false;
                 //if(!MoveActr(aiFED_Covr_FwBw , ccBwd)) return false;

                 //MoveActr(aiLSR_StgL_FwBw , ccBwd);

                 Step.iToStop++;
                 return false ;

        case 12: //if(!MoveActr(aiLSR_StgL_FwBw , ccBwd)) return false ;

                 //MoveMotr(miLSR_TFrt , piLSR_TLerWait);

                 Step.iToStop++;
                 return false ;

        case 13: //if(!MoveMotr(miLSR_TFrt , piLSR_TLerWait))return false;

                 Step.iToStop = 0   ;
                 return true ;

    }
}

bool CLearLaser::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLSR_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    int iChekCount      = 0 ;

    if(OM.DevOptn.bUseLsrPst1) iChekCount++;
    if(OM.DevOptn.bUseLsrPst2) iChekCount++;
    if(OM.DevOptn.bUseLsrPst3) iChekCount++;
    if(OM.DevOptn.bUseLsrPst4) iChekCount++;
    if(OM.DevOptn.bUseLsrPst5) iChekCount++;

    const  int  iMaxLaserWrkCnt = iChekCount;             //총 작업하는 카운트 체크 된 체크 박스 수
    static int  iLaserWorkCnt   = 0 ;                     //레이저 작업 한 횟수
    static int  iPosIdOffset    = 0 ;                     //포지션 아이디 옵셑
    static int  iLaserWorkDev   = OM.DevOptn.iLerLsrNo1 ; //OM.DevOptn.iLsrDevU에 값들, 레이저 디바이스 값
    static bool bPstnUsedRslt   = true ;                  //OM.DevOptn.bUsePstU에 값 ,이걸로 반복 스텝 탈지 안탈지 정함

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!OM.CmnOptn.bSkipLaserWrk && LCU.GetBlockNoID() != OM.DevOptn.iLerLsrNo1){

                      //20150127 트레이스 추가 하였으나 아직 적용 안됨. 프로그램 보내지 않았음.
                      Trace("SetGABlockId 1" , String(OM.DevOptn.iLerLsrNo1).c_str());
                      LCU.SetGABlockId(OM.DevOptn.iLerLsrNo1);


                      Trace("SetSendMsgId 1" , String(OM.DevOptn.iLerLsrNo1).c_str());
                      LCU.SetSendMsgId(lmGA);
                  }

                  IO_SetY(yLSR_EjctL    , false);
                  IO_SetY(yLSR_VccL     , true );
                  IO_SetY(yETC_DustSuck , true );

                  if(!IO_GetX(xLSR_VccSnsrL)){
                      EM_SetErrMsg(eiLSR_Vaccum , "Rear");
                      Step.iCycle = 0;
                      return false;
                  }

                  //if(!OM.CmnOptn.bSkipLaserWrk) {
                  //    if(!LCU.GetGAFinished()) return false ;
                  //}
                  iPosIdOffset  = 0 ;
                  iLaserWorkCnt = 0 ;



                  iLaserWorkDev = OM.DevOptn.iLerLsrNo1 ;
                  if(!OM.CmnOptn.bSkipLaserWrk && LCU.GetBlockNoID() != iLaserWorkDev){ //이부분 및 아마 Front도 이렇게 되어 있을듯 한데.은 빼도 될듯 한데...위에 있는데 왜 또 있을까....
                      LCU.SetGABlockId(OM.DevOptn.iLerLsrNo1);
                      LCU.SetSendMsgId(lmGA);
                      EM_SetErr(eiLLS_LaserBlock);
                  }

                  if(!OM.CmnOptn.bSkipLaserWrk) {
                      if(!LCU.GetGAFinished()) return false ;
                  }


                  bPstnUsedRslt = OM.DevOptn.bUseLsrPst1 ;

                  Step.iCycle++;
                  return false;

                  //스탭 밑에서 사용...
        case  11: if(bPstnUsedRslt) { //OM.DevOptn.bUsePstU 이 true 이면 다음 스텝으로...
                      Step.iCycle++;
                      return false;
                  }
                  else {
                      Step.iCycle = 15;    //false 이면 15번으로
                      return false;
                  }

        case  12: MoveMotr(miLSR_TLer , (int)piLSR_TLerPos1 + iPosIdOffset );
                  if(iPosIdOffset != 0 && !OM.CmnOptn.bSkipLaserWrk){
                      LCU.SetGABlockId(iLaserWorkDev ); //레이저 작업
                      LCU.SetSendMsgId(lmGA);
                  }

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miLSR_TLer , (int)piLSR_TLerPos1 + iPosIdOffset )) return false;

                  if(!OM.CmnOptn.bSkipLaserWrk){
                      if(iPosIdOffset != 0){
                          if(!LCU.GetGAFinished()) return false ;
                      }
                      LCU.SetSendMsgId(lmTX);
                  }
                  iLaserWorkCnt++;

                  Step.iCycle++;
                  return false;

        case  14: if(!OM.CmnOptn.bSkipLaserWrk){
                      if(LCU.GetLaserMsgId() != lmNone)return false ;
                  }
                  Step.iCycle++;
                  return false;

        //스탭 위에서 쓰임...
        case  15: if(iLaserWorkCnt >= iMaxLaserWrkCnt) { //총 작업해야 할 레이저 카운트와 현재 작업한 레이저 카운트 비교
                      iPosIdOffset  = 0 ;
                      iLaserWorkCnt = 0 ;

                      iLaserWorkDev = OM.DevOptn.iLerLsrNo1 ;

                      bPstnUsedRslt = OM.DevOptn.bUseLsrPst1 ;
                      if(!OM.CmnOptn.bSkipLaserWrk){//여기 나누자
                          if(OM.CmnOptn.iUseLsrPos == 0){
                              LCU.SetGABlockId(OM.DevOptn.iFrtLsrNo1);
                              LCU.SetSendMsgId(lmGA);
                          }
                          else if(OM.CmnOptn.iUseLsrPos == 2){
                              LCU.SetGABlockId(OM.DevOptn.iLerLsrNo1);
                              LCU.SetSendMsgId(lmGA);
                          }
                      }

                      Step.iCycle++; //작업종료 절차.
                      return false;
                  }
                  //로테이트 카운트에 따라 옵션값 가져 오기....
                  if(iPosIdOffset == 0) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst2; iLaserWorkDev = OM.DevOptn.iLerLsrNo2; }
                  if(iPosIdOffset == 1) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst3; iLaserWorkDev = OM.DevOptn.iLerLsrNo3; }
                  if(iPosIdOffset == 2) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst4; iLaserWorkDev = OM.DevOptn.iLerLsrNo4; }
                  if(iPosIdOffset == 3) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst5; iLaserWorkDev = OM.DevOptn.iLerLsrNo5; }
                  iPosIdOffset++;

                  Step.iCycle = 11;
                  return false    ;

        case  16: DM.ARAY[riLLS].SetStat(csWork);
                  SPC.m_iUserWorkCnt += DM.ARAY[riLLS].GetCntStat(csWork);
                  m_bAutoRun = false;

                  Step.iCycle=0;
                  return true ;

    }
}

//작업 위치로 이동.
bool CLearLaser::CyclePlace()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLSR_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    if(IO_GetX(xETC_BoxOut)){
        MT_GoAbsRun(miLSR_TLer , PM.GetValue(miLSR_TLer , pvLSR_TLerWait));
        EM_SetErr(eiETC_BoxOut);
        Step.iCycle = 0 ;
        return true ;
    }



    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: LFD.MoveMotr(miFED_YLer , piFED_YLerWork);

                  Step.iCycle++;
                  return false ;





        case  11: if(!LFD.MoveMotr(miFED_YLer , piFED_YLerWork))return false;

                  MoveMotr(miLSR_TLer , piLSR_TLerPlace);

                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miLSR_TLer , piLSR_TLerPlace))return false;

                  IO_SetY(yLSR_VccL  , false);
                  IO_SetY(yLSR_EjctL , true );
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  13: if(!m_tmDelay.OnDelay(true , 200))return false;

                  if(IO_GetX(xLSR_VccSnsrL)){
                      EM_SetErr(eiLSR_Eject);
                      Step.iCycle=0;
                      return false;
                  }
                  IO_SetY (yLSR_EjctL    , false         );
                  MoveMotr(miLSR_TLer    , piLSR_TLerWait);


                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLSR_TLer , piLSR_TLerWait)) return false;
                  m_bAutoRun = false;
                  DM.ARAY[riLLS].SetStat(csNone);

                  Step.iCycle=0;
                  return true;
    }
}



void CLearLaser::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLearLaser::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CLearLaser::CheckStop()
{
    if(!MT_GetStop(miLSR_TLer)) return false ;

    if(!AT_Done(aiLSR_StgL_FwBw)) return false ;

    return true ;
}

void CLearLaser::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName + ".INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLaserWorkLotNo" , m_sLaserWorkLotNo);
    }

    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLaserWorkLotNo" , m_sLaserWorkLotNo);
    }

}





//---------------------------------------------------------------------------
