//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LearFeed.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "FrontFeed.h"
#include "LearLaser.h"
#include "SAInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CLearFeed LFD;

CLearFeed::CLearFeed(void)
{

}

CLearFeed::~CLearFeed (void)
{

}

void CLearFeed::Init()
{
    m_sPartName = "Rear Feed" ;
    Reset();
    Load(true);

    InitCycleName();

    m_InspVsnCnt = 0;
}
void CLearFeed::Close()
{
    Load(false);
}

void CLearFeed::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CLearFeed::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLearFeed::FindChip(int &r , int &c )
{
    c = DM.ARAY[riLFD].FindLastCol (csNone);

    //스트립 상태
    //None    = 스트립 없는 상태.
    //Unkwn   = 스트립 들어와서 솔더 검사할 차례
    //Mask    = 솔더 검사 다 하고 자제 내려 놓을 상태.
    //Wait    = 자제 내려 놓고 안착 검사 할 차례
    //Work    = 안착 검사 완료한 Good시료.
    //Fail    = 안착 검사시에 불량시료들. 뒤에서 리웍 작업하는 것들.

    return (c > -1 && c > -1);
}

double CLearFeed::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miFED_YLer){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piFED_YLerWait : dPos = PM.GetValue (_iMotr , pvFED_YLerWait ); break ;
            case piFED_YLerWork : dPos = PM.GetValue (_iMotr , pvFED_YLerWork ); break ;
            case piFED_YLerFeed : dPos = PM.GetValue (_iMotr , pvFED_YLerFeed ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }
    return dPos ;
}

//---------------------------------------------------------------------------
bool CLearFeed::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //double dLearTCrntPos = MT_GetCmdPos(miLSR_TLer) ;
    //double dLearTWaitPos = GetMotrPos(miLSR_TLer , piLSR_TLerWait ) ;

    double dFrntYCrntPos = MT_GetCmdPos(miFED_YFrt) ;
    double dFrntYWorkPos = PM.GetValue(miFED_YFrt , pvFED_YFrtWork ) ;
    double dFrntYWaitPos = PM.GetValue(miFED_YFrt , pvFED_YFrtWait ) ;
    double dOffset       = OM.MstOptn.dOffset ;

    bool   bFrntYWorkPos = (dFrntYWorkPos - dOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYWorkPos + dOffset) ;
    bool   bFrntYWaitPos = (dFrntYWaitPos - dOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYWaitPos + dOffset) ;

    bool bLearStgFwd  = !AT_Complete(aiLSR_StgL_FwBw, ccBwd) ;

    if(_iMotr == miFED_YLer){

        switch(_iPstnId) {
            default                :                                                      break ;
            case piFED_YLerWait    : if( bLearStgFwd  ) {
                                        sMsg = "Lear Stage is FWD"; bRet = false ;} break ;
            case piFED_YLerWork    : if( bLearStgFwd  ) {
                                        sMsg = "Lear Stgae is FWD"; bRet = false ;} break ;
            case piFED_YFrtFeed    : if( bLearStgFwd  ) {
                                        sMsg = "Lear Stgae is FWD"; bRet = false ;}
                                     if(!bFrntYWorkPos && !bFrntYWaitPos) {
                                        sMsg = "Front Y Motor is Not WaitPos or Not WorkPos"; bRet = false ;} break ;
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle){
            FM_MsgOk("CheckSafe",sMsg.c_str());
        }
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CLearFeed::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //bool isXMoving = !MT_GetStopInpos(miWRK_XIdx);

    bool bRet = true ;
    AnsiString sMsg ;

    double dLearTCrntPos = MT_GetCmdPos(miLSR_TLer) ;
    double dLearTWaitPos = GetMotrPos(miLSR_TLer , piLSR_TLerWait ) ;

    double dLearYCrntPos = MT_GetCmdPos(miFED_YLer) ;
    double dLearYFeedPos = GetMotrPos(miFED_YLer , piFED_YLerFeed ) ;

    double dOffset       = OM.MstOptn.dOffset ;

    bool   bLearTWaitPos = (dLearTWaitPos - dOffset <= dLearTCrntPos) && (dLearTCrntPos <= dLearTWaitPos + dOffset) ;
    bool   bLearYFeedPos = (dLearYFeedPos - dOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYFeedPos + dOffset) ;

    bool bCoverFwd     = !AT_Complete(aiFED_Covr_FwBw, ccBwd) ;

    if(_iActr == aiLSR_StgL_FwBw) {
        if( bCoverFwd    ) { sMsg = "Cover is Not BWD"             ; bRet = false ;}
        if(!bLearTWaitPos) { sMsg = "Lear T Moter is Not Wait Pos" ; bRet = false ;}
        if(_bFwd) {
            if(!bLearYFeedPos) { sMsg = "Lear Y Moter is Not Feed Pos" ; bRet = false ;}
        }
    }


    else if(_iActr == aiFED_Covr_FwBw) { }
    else if(_iActr == aiFED_Algn_FwBw) { }



    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CLearFeed::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLearFeed::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLearFeed::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 20000 )) {
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

                 MT_SetHomeEnd(miFED_YLer , false);
                 MT_DoHome(miFED_YLer);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miFED_YLer)) return false;
                 MT_GoAbsMan(miFED_YLer, PM.GetValue(miFED_YLer , pvFED_YLerWait));
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GetStopInpos(miFED_YLer)) return false;
                 if(DM.ARAY[riLFD].CheckAllStat(csNone) && DM.ARAY[riLLS].CheckAllStat(csNone)) IO_SetY(yLSR_VccL , false);
                 Step.iHome = 0;
                 return true ;
    }
}

bool CLearFeed::Autorun(void) //오토런닝시에 계속 타는 함수.
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
        if(Stat.bReqStop )return false ;

        if(OM.CmnOptn.iUseLsrPos ==1) return false;

        bool bLotOpOutCon = LTQ.GetLotOpen() && DM.ARAY[riLFD].CheckAllStat(csUnkwn) ; //랏 오픈 시 아웃 조건
        bool bLotClOutCon =!LTQ.GetLotOpen() && DM.ARAY[riLFD].CheckAllStat(csUnkwn) ; //랏 종료 시 아웃 조건

        // 인 얼라인 앤드 사이클 공용 조건
        double dFrntYCrntPos = MT_GetCmdPos(miFED_YFrt) ;

        //인 , 얼라인 사이클 조건
        double dFrntYWorkPos = PM.GetValue(miFED_YFrt , pvFED_YFrtWork ) ;
        double dWrkOffset    = OM.MstOptn.dOffset ;
        bool   bFrntYWorkPos = (dFrntYWorkPos - dWrkOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYWorkPos + dWrkOffset) ;

        double dFrntYWaitPos = PM.GetValue(miFED_YFrt , pvFED_YFrtWait ) ;
        bool   bFrntYWaitPos = (dFrntYWaitPos - dWrkOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYWaitPos + dWrkOffset) ;

        //엔드 사이클 조건
        double dFrntYFeedPos = PM.GetValue(miFED_YFrt , pvFED_YFrtFeed ) ;
        double dFedOffset    = OM.MstOptn.dOffset * 100;
        bool   bFrntYFeedPos = (dFrntYFeedPos - dFedOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYFeedPos + dFedOffset) ;

        bool isCycleIn    = FFD.GetSeqStep() == CFrontFeed :: scIdle && DM.ARAY[riLLS].CheckAllStat(csNone ) && DM.ARAY[riLFD].CheckAllStat(csNone ) && (bFrntYWorkPos || bFrntYWaitPos) ;

        //bool isCycleAlign = LTQ.GetLotOpen() && DM.ARAY[riLFD].CheckAllStat(csUnkwn) && bFrntYWorkPos ;
        bool isCycleOut   = bLotOpOutCon || bLotClOutCon ;
        bool isCycleEnd   = !isCycleOut && bFrntYFeedPos ; // 이건 수정 가능 성 있음...

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleIn   ) {Trace(m_sPartName.c_str(),"CycleIn    Stt"); Step.iSeq = scIn    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        //else if (isCycleAlign) {Trace(m_sPartName.c_str(),"CycleAlign Stt"); Step.iSeq = scAlign ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleOut  ) {Trace(m_sPartName.c_str(),"CycleOut   Stt"); Step.iSeq = scOut   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default     :                   Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle :                                                                                     return false ;
        case scIn   : if(CycleIn   ()){ Trace(m_sPartName.c_str(),"CycleIn    End");Step.iSeq = scIdle ;} return false ;
        //case scAlign: if(CycleAlign()){ Trace(m_sPartName.c_str(),"CycleAlign End");Step.iSeq = scIdle ;} return false ;
        case scOut  : if(CycleOut  ()){ Trace(m_sPartName.c_str(),"CycleOut   End");Step.iSeq = scIdle ;} return false ;

    }

    return false ;

}

bool CLearFeed::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CLearFeed::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLearFeed::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: //MoveActr(aiFWK_Stp_UpDn , ccFwd);
                 //m_tmTemp.Clear();
                 Step.iToStart++;
                 return false ;

        case 11: //if(!MoveActr(aiFWK_Stp_UpDn , ccFwd)) return false;
                 //if(!m_tmTemp.OnDelay(true , 200)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLearFeed::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: IO_SetY(yFED_Vcc1 , true);//MoveActr(aiFWK_Pshr_DnUp , ccBwd);
                 //if(EM_GetErr(eiATR_TimeOut)){
                 //    MoveActr(aiLSR_StgL_FwBw , ccBwd);
                 //}
                 //MoveActr(aiFED_Covr_FwBw , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: //if(!MoveActr(aiFED_Covr_FwBw , ccBwd))return false ;
                 //if(EM_GetErr(eiATR_TimeOut)){
                 //    if(!MoveActr(aiLSR_StgL_FwBw , ccBwd)) return false;
                 //}

                 Step.iToStop = 0   ;
                 return true ;

    }
}

bool CLearFeed::CycleIn()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 20000 )) {
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

    TRetResult InspRslt ;

    static DWORD dPreTickTime = GetTickCount();

    PreStep.iCycle = Step.iCycle ;

    double dRearTCrntPos = MT_GetCmdPos(miLSR_TLer) ;
    double dRearTWaitPos = PM.GetValue(miLSR_TLer , pvLSR_TLerWait ) ;
    double dOffset       = OM.MstOptn.dOffset;
    bool   bRearTWaitPos = (dRearTWaitPos - dOffset <= dRearTCrntPos) && (dRearTCrntPos <= dRearTWaitPos + dOffset) ;







    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!bRearTWaitPos){
                      EM_SetErr(eiLSR_RearTNotWaitPos);
                      Step.iCycle = 0 ;
                      return true ;
                  }



                  dPreTickTime   = GetTickCount();
                  MoveActr(aiFED_Covr_FwBw , ccBwd);
                  MoveActr(aiFED_Algn_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiFED_Covr_FwBw , ccBwd))return false;
                  if(!MoveActr(aiFED_Algn_FwBw , ccBwd))return false;

                  if(!bRearTWaitPos){
                      MoveActr(aiLSR_StgL_FwBw , ccBwd);
                      Step.iCycle = 100 ;
                      return false ;
                  }






                  //MoveActr(aiLSR_StgL_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  12: //if(!MoveActr(aiLSR_StgL_FwBw , ccBwd))return false;

                  //LLS.MoveMotr(miLSR_TLer , piLSR_TLerWait);

                  Step.iCycle++;
                  return false;

        case  13: //if(!LLS.MoveMotr(miLSR_TLer , piLSR_TLerWait))return false;

                  MoveMotr(miFED_YLer , piFED_YLerFeed);

                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miFED_YLer , piFED_YLerFeed))return false;

                  MoveActr(aiLSR_StgL_FwBw , ccFwd);

                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiLSR_StgL_FwBw , ccFwd)) return false;

                  MoveActr(aiFED_Covr_FwBw , ccFwd);

                  Step.iCycle++;
                  return false;

                  // 스텝 밑에서 사용
        case  16: if(!MoveActr(aiFED_Covr_FwBw , ccFwd)) return false;

                  IO_SetY(yFED_AirBlow1 , true );
                  IO_SetY(yFED_AirBlow2 , true );
                  IO_SetY(yFED_Vcc1     , false);
                  IO_SetY(yFED_Vcc2     , false);
                  IO_SetY(yLSR_VccL     , false);
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  17: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iAirBlowTm)) return false;

                  IO_SetY(yFED_Vcc1     , true );
                  IO_SetY(yFED_Vcc2     , true );
                  IO_SetY(yLSR_VccL     , true );
                  MoveActr(aiFED_Algn_FwBw , ccFwd);
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false;

        case  18: if(!MoveActr(aiFED_Algn_FwBw , ccFwd))return false;
                  if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iAlignTm))return false;

                  MoveActr(aiFED_Algn_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  19: if(!MoveActr(aiFED_Algn_FwBw , ccBwd))return false;

                  IO_SetY(yFED_AirBlow1 , false);
                  IO_SetY(yFED_AirBlow2 , false);
                  if(LTQ.GetLotOpen())SV_Grab(viPkg); //비젼검사

                  Step.iCycle++;
                  return false;

        case  20: if(LTQ.GetLotOpen()){
                      if(!SV_GetGrabEnd(viPkg)) return false ;
                      SV_Inspect(viPkg , false); //쓰레드 안씀.
                      SV_GetRslt(viPkg , &InspRslt);

                      if(InspRslt.iRet != vePkgOk) {                 // 비젼검사 실패 하면
                          if(m_InspVsnCnt >= OM.CmnOptn.iVsnFailCnt-1){ // 검사 카운트 보다 클 경우
                              EM_SetErr(eiFED_RearVision);                // 에러
                              m_InspVsnCnt= 0;
                              Step.iCycle = 0;
                              return true;
                          }
                          m_InspVsnCnt++;                             // 검사 카운트 보다 적을 경우
                          Step.iCycle = 16;                           // 다시 에어 불고 검사
                          return false;
                      }
                  }
                  IO_SetY (yLSR_VccL , true );
                  DM.ARAY[riLFD].SetStat(csUnkwn);
                  m_InspVsnCnt= 0;

                  if(DM.ARAY[riLFD].CheckAllStat(csUnkwn)){
                      m_dWorkTotalTm = GetTickCount() - dPreTickTime  ;
                  }

                  Step.iCycle=0;
                  return true;

        case 100: if(!MoveActr(aiLSR_StgL_FwBw , ccBwd)) return false;

                  LLS.MoveMotr(miLSR_TLer , piLSR_TLerWait);

                  Step.iCycle++;
                  return false;

        case 101: if(!LLS.MoveMotr(miLSR_TLer , piLSR_TLerWait)) return false;

                  Step.iCycle=11;
                  return false;





    }
}



bool CLearFeed::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 20000 )) {
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




    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveActr(aiFED_Algn_FwBw , ccBwd);
                  MoveActr(aiFED_Covr_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiFED_Algn_FwBw , ccBwd)) return false;
                  if(!MoveActr(aiFED_Covr_FwBw , ccBwd)) return false;

                  MoveActr(aiLSR_StgL_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiLSR_StgL_FwBw , ccBwd)) return false;

                  MoveMotr(miFED_YLer , piFED_YLerWork);

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miFED_YLer , piFED_YLerWork)) return false;

                  DM.ARAY[riLFD].SetStat(csWait);
                  DM.ShiftArrayData(riLFD , riLLS);

                  Step.iCycle=0;
                  return false;


    }
}



void CLearFeed::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLearFeed::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CLearFeed::CheckStop()
{
    if(!MT_GetStop(miFED_YLer)) return false ;

    //if(!AT_Done(aiWRK_IdxUpDn)) return false ;

    return true ;
}

void CLearFeed::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName + ".INI";

}
//---------------------------------------------------------------------------

