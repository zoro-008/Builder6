//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "FrontLaser.h"
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
#include "FrontFeed.h"
#include "LearLaser.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CFrontLaser FLS;

CFrontLaser::CFrontLaser(void)
{

}

CFrontLaser::~CFrontLaser (void)
{

}

void CFrontLaser::Init()
{
    m_sPartName = "Front Laser" ;
    Reset();
    Load(true);

    m_bTestMark = false;
    m_bAutoRun  = false;

    InitCycleName();
}
void CFrontLaser::Close()
{
    Load(false);
}




void CFrontLaser::Reset()
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

void CFrontLaser::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

bool CFrontLaser::FindChip(int &r , int &c )
{
    r = -1 ;
    c = -1 ;

    return (c > -1 && c > -1);
}

double CFrontLaser::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    if(_iMotr == miLSR_TFrt){
        switch(_iPstnId) {
            default              : dPos = MT_GetCmdPos(_iMotr                   ); break ;
            case piLSR_TFrtWait  : dPos = PM.GetValue (_iMotr , pvLSR_TFrtWait  ); break ;
            case piLSR_TFrtPos1  : dPos = PM.GetValue (_iMotr , pvLSR_TFrtPos1  ); break ;
            case piLSR_TFrtPos2  : dPos = PM.GetValue (_iMotr , pvLSR_TFrtPos2  ); break ;
            case piLSR_TFrtPos3  : dPos = PM.GetValue (_iMotr , pvLSR_TFrtPos3  ); break ;
            case piLSR_TFrtPos4  : dPos = PM.GetValue (_iMotr , pvLSR_TFrtPos4  ); break ;
            case piLSR_TFrtPos5  : dPos = PM.GetValue (_iMotr , pvLSR_TFrtPos5  ); break ;
            case piLSR_TFrtPlace : dPos = PM.GetValue (_iMotr , pvLSR_TFrtPlace ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CFrontLaser::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bFrntStgFwd    = !AT_Complete(aiLSR_StgF_FwBw , ccBwd);

    if(_iMotr == miLSR_TFrt){
        if( bFrntStgFwd   ) {sMsg = "Stage Cylinder is FWD"; bRet = false ;}
        switch(_iPstnId) {
            default              :  break ;
            case pvLSR_TFrtWait  :  break ;
            case pvLSR_TFrtPos1  :  break ;
            case pvLSR_TFrtPos2  :  break ;
            case pvLSR_TFrtPos3  :  break ;
            case pvLSR_TFrtPos4  :  break ;
            case pvLSR_TFrtPos5  :  break ;
            case pvLSR_TFrtPlace :  break ;

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
bool CFrontLaser::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg ;

    double dFrntTCrntPos = MT_GetCmdPos(miLSR_TFrt) ;
    double dFrntTWaitPos = GetMotrPos(miLSR_TFrt , piLSR_TFrtWait ) ;
    double dOffset       = OM.MstOptn.dOffset ;

    double dFrntYCrntPos = MT_GetCmdPos(miFED_YFrt) ;
    double dFrntYFeedPos = GetMotrPos(miFED_YFrt , piFED_YFrtFeed ) ;


    bool bFrntTWaitPos = (dFrntTWaitPos - dOffset <= dFrntTCrntPos) && (dFrntTCrntPos <= dFrntTWaitPos + dOffset) ;
    bool bFrntYFeedPos = (dFrntYFeedPos - dOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYFeedPos + dOffset) ;

    bool bCoverFwd     = !AT_Complete(aiFED_Covr_FwBw, ccBwd) ;

    if(_iActr == aiLSR_StgF_FwBw) {
        if( bCoverFwd    ) { sMsg = "Cover is Not BWD"              ; bRet = false ;}
        if(!bFrntTWaitPos) { sMsg = "Front T Moter is Not Wait Pos" ; bRet = false ;}
        if(_bFwd) {
            if(!bFrntYFeedPos) { sMsg = "Front Y Moter is Not Feed Pos" ; bRet = false ;}
        }
    }

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

bool CFrontLaser::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CFrontLaser::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CFrontLaser::CycleHome()     //sun DLL direct access.
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


        case 10: IO_SetY(yLSR_VccF , true);
                 AT_MoveCyl(aiFED_Algn_FwBw , ccBwd);
                 AT_MoveCyl(aiFED_Covr_FwBw , ccBwd);

                 Step.iHome++;
                 return false;

        case 11: if(!AT_MoveCyl(aiFED_Algn_FwBw , ccBwd)) return false;
                 if(!AT_MoveCyl(aiFED_Covr_FwBw , ccBwd)) return false;

                 AT_MoveCyl(aiLSR_StgF_FwBw , ccBwd);

                 Step.iHome++;
                 return false;

        case 12: if(!AT_MoveCyl(aiLSR_StgF_FwBw , ccBwd)) return false;

                 MT_SetHomeEnd(miLSR_TFrt , false);
                 MT_DoHome(miLSR_TFrt);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miLSR_TFrt)) return false;

                 MT_GoAbsMan(miLSR_TFrt, PM.GetValue(miLSR_TFrt , pvLSR_TFrtWait));
                 if(DM.ARAY[riFFD].CheckAllStat(csNone) && DM.ARAY[riFLS].CheckAllStat(csNone)) IO_SetY(yLSR_VccF , false);

                 Step.iHome = 0;
                 return true ;

    }
}

bool CFrontLaser::Autorun(void) //오토런닝시에 계속 타는 함수.
{

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

        double dFrntYCrntPos = MT_GetCmdPos(miFED_YFrt) ;
        double dFrntYWorkPos = GetMotrPos(miFED_YFrt , piFED_YFrtWork) ;
        double dOffset       = OM.MstOptn.dOffset ;

        bool bFEDMtrWorkPos  = dFrntYWorkPos - dOffset <= dFrntYCrntPos && dFrntYCrntPos <= dFrntYWorkPos + dOffset ;



        bool bLotOpenOutCon = LTQ.GetLotOpen() && ( DM.ARAY[riFLS].CheckAllStat(csWork ) && bFEDMtrWorkPos) ;
        bool bLotEndOutCon  =!LTQ.GetLotOpen() && ( DM.ARAY[riFLS].CheckAllStat(csWait ) && bFEDMtrWorkPos) ;
        /*
        bool isCycleWork    = LTQ.GetLotOpen() &&(( DM.ARAY[riFLS].CheckAllStat(csWait ) && bFEDMtrWorkPos &&
                                                   !DM.ARAY[riLLS].GetCntStat  (csWait )                  )||
                                                  ( DM.ARAY[riFLS].CheckAllStat(csWait ) && !DM.ARAY[riLLS].CheckAllStat(csWait )));
        */
        bool isCycleWork    = LTQ.GetLotOpen() &&( DM.ARAY[riFLS].CheckAllStat(csWait ) && bFEDMtrWorkPos &&
                                                   LLS.GetSeqStep() != CLearLaser :: scWork );


        bool isCyclePlace   = (bLotOpenOutCon || bLotEndOutCon) ;

        bool isCycleEnd     = DM.ARAY[riFLS].CheckAllStat(csNone);

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

bool CFrontLaser::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CFrontLaser::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CFrontLaser::ToStart(void) //스타트를 하기 위한 함수.
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

bool CFrontLaser::ToStop(void) //스탑을 하기 위한 함수.
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



        case 10: if(DM.ARAY[riFFD].GetCntStat(csUnkwn) || DM.ARAY[riFFD].GetCntStat(csGood )||
                    DM.ARAY[riFFD].GetCntStat(csWait ) || DM.ARAY[riFLS].GetCntStat(csWait )||
                    DM.ARAY[riFLS].GetCntStat(csWork ) ){

                     IO_SetY(yLSR_VccF , true);
                 }

                 //MoveActr(aiFED_Algn_FwBw , ccBwd);
                 //MoveActr(aiFED_Covr_FwBw , ccBwd);

                 Step.iToStop++;
                 return false;

        case 11: //if(!MoveActr(aiFED_Algn_FwBw , ccBwd)) return false;
                 //if(!MoveActr(aiFED_Covr_FwBw , ccBwd)) return false;

                 //MoveActr(aiLSR_StgF_FwBw , ccBwd);

                 Step.iToStop++;
                 return false ;

        case 12: //if(!MoveActr(aiLSR_StgF_FwBw , ccBwd)) return false ;

                 //MoveMotr(miLSR_TFrt , piLSR_TFrtWait);

                 Step.iToStop++;
                 return false ;

        case 13: //if(!MoveMotr(miLSR_TFrt , piLSR_TFrtWait))return false;
                 Step.iToStop = 0   ;
                 return true ;

    }
}

bool CFrontLaser::CycleWork(void)
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
    static int  iLaserWorkDev   = OM.DevOptn.iFrtLsrNo1 ; //OM.DevOptn.iLsrDevU에 값들, 레이저 디바이스 값
    static bool bPstnUsedRslt   = true ;                  //OM.DevOptn.bUsePstU에 값 ,이걸로 반복 스텝 탈지 안탈지 정함


    PreStep.iCycle = Step.iCycle ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!OM.CmnOptn.bSkipLaserWrk && LCU.GetBlockNoID() != OM.DevOptn.iFrtLsrNo1){
                      LCU.SetGABlockId(OM.DevOptn.iFrtLsrNo1);
                      LCU.SetSendMsgId(lmGA);
                  }

                  IO_SetY(yLSR_EjctF    , false);
                  IO_SetY(yLSR_VccF     , true );
                  IO_SetY(yETC_DustSuck , true );

                  if(!IO_GetX(xLSR_VccSnsrF)){
                      EM_SetErrMsg(eiLSR_Vaccum , "Front");
                      Step.iCycle = 0;
                      return false;
                  }

                  //if(!OM.CmnOptn.bSkipLaserWrk) {
                  //    if(!LCU.GetGAFinished()) return false ;
                  //}
                  iPosIdOffset  = 0 ;
                  iLaserWorkCnt = 0 ;

                  iLaserWorkDev = OM.DevOptn.iFrtLsrNo1 ;
                  if(!OM.CmnOptn.bSkipLaserWrk && LCU.GetBlockNoID() != iLaserWorkDev){
                      LCU.SetGABlockId(OM.DevOptn.iFrtLsrNo1);
                      LCU.SetSendMsgId(lmGA);
                      EM_SetErr(eiFLS_LaserBlock);
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
                      Step.iCycle = 15;    //false 이면 19번으로
                      return false;
                  }

        case  12: MoveMotr(miLSR_TFrt , (int)piLSR_TFrtPos1 + iPosIdOffset );
                  if(iPosIdOffset != 0 && !OM.CmnOptn.bSkipLaserWrk){
                      LCU.SetGABlockId(iLaserWorkDev ); //레이저 작업
                      LCU.SetSendMsgId(lmGA);
                  }

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miLSR_TFrt , (int)piLSR_TFrtPos1 + iPosIdOffset )) return false;

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

                      iLaserWorkDev = OM.DevOptn.iFrtLsrNo1 ;

                      bPstnUsedRslt = OM.DevOptn.bUseLsrPst1 ;
                      if(!OM.CmnOptn.bSkipLaserWrk){//여기 나누자
                          if(OM.CmnOptn.iUseLsrPos == 0){
                              LCU.SetGABlockId(OM.DevOptn.iLerLsrNo1);
                              LCU.SetSendMsgId(lmGA);
                          }
                          else if(OM.CmnOptn.iUseLsrPos == 1){
                              LCU.SetGABlockId(OM.DevOptn.iFrtLsrNo1);
                              LCU.SetSendMsgId(lmGA);
                          }
                      }

                      Step.iCycle++; //작업종료 절차.
                      return false;
                  }
                  //로테이트 카운트에 따라 옵션값 가져 오기....
                  if(iPosIdOffset == 0) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst2; iLaserWorkDev = OM.DevOptn.iFrtLsrNo2; }
                  if(iPosIdOffset == 1) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst3; iLaserWorkDev = OM.DevOptn.iFrtLsrNo3; }
                  if(iPosIdOffset == 2) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst4; iLaserWorkDev = OM.DevOptn.iFrtLsrNo4; }
                  if(iPosIdOffset == 3) { bPstnUsedRslt = OM.DevOptn.bUseLsrPst5; iLaserWorkDev = OM.DevOptn.iFrtLsrNo5; }
                  iPosIdOffset++;

                  Step.iCycle = 11;
                  return false    ;

        case  16: DM.ARAY[riFLS].SetStat(csWork);
                  SPC.m_iUserWorkCnt += DM.ARAY[riFLS].GetCntStat(csWork);
                  m_bAutoRun = false;




                  Step.iCycle=0;
                  return true ;

    }
}

//작업 위치로 이동.
bool CFrontLaser::CyclePlace()
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
        MT_GoAbsRun(miLSR_TFrt , PM.GetValue(miLSR_TFrt , pvLSR_TFrtWait));
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

        case  10: FFD.MoveMotr(miFED_YFrt , piFED_YFrtWork);

                  Step.iCycle++;
                  return false ;

        case  11: if(!FFD.MoveMotr(miFED_YFrt , piFED_YFrtWork)) return false;

                  MoveMotr(miLSR_TFrt , piLSR_TFrtPlace);

                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miLSR_TFrt , piLSR_TFrtPlace))return false;

                  IO_SetY(yLSR_VccF  , false);
                  IO_SetY(yLSR_EjctF , true );
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  13: if(!m_tmDelay.OnDelay(true , 200))return false;

                  if(IO_GetX(xLSR_VccSnsrF)){
                      EM_SetErr(eiLSR_Eject);
                      Step.iCycle=0;
                      return false;
                  }
                  IO_SetY (yLSR_EjctF    , false         );
                  MoveMotr(miLSR_TFrt    , piLSR_TFrtWait);


                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLSR_TFrt , piLSR_TFrtWait)) return false;
                  m_bAutoRun = false;
                  DM.ARAY[riFLS].SetStat(csNone);

                  Step.iCycle=0;
                  return true;
    }
}



void CFrontLaser::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CFrontLaser::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CFrontLaser::CheckStop()
{
    if(!MT_GetStop(miLSR_TFrt)) return false ;

    if(!AT_Done(aiLSR_StgF_FwBw)) return false ;

    return true ;
}

void CFrontLaser::Load(bool _bLoad)
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
