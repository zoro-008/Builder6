//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "FrontFeed.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "FrontLaser.h"
#include "SAInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CFrontFeed FFD;

CFrontFeed::CFrontFeed(void)
{

}

CFrontFeed::~CFrontFeed (void)
{

}

void CFrontFeed::Init()
{
    m_sPartName = "Front Feed" ;
    Reset();
    Load(true);

    m_InspVsnCnt = 0;

    InitCycleName();
}
void CFrontFeed::Close()
{
    Load(false);
}

void CFrontFeed::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CFrontFeed::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CFrontFeed::FindChip(int &r , int &c )
{
    c = DM.ARAY[riFFD].FindLastCol (csNone);

    //스트립 상태
    //None    = 스트립 없는 상태.
    //Unkwn   = 스트립 들어와서 솔더 검사할 차례
    //Mask    = 솔더 검사 다 하고 자제 내려 놓을 상태.
    //Wait    = 자제 내려 놓고 안착 검사 할 차례
    //Work    = 안착 검사 완료한 Good시료.
    //Fail    = 안착 검사시에 불량시료들. 뒤에서 리웍 작업하는 것들.

    return (c > -1 && c > -1);
}

double CFrontFeed::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miFED_YFrt){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piFED_YFrtWait : dPos = PM.GetValue (_iMotr , pvFED_YFrtWait ); break ;
            case piFED_YFrtWork : dPos = PM.GetValue (_iMotr , pvFED_YFrtWork ); break ;
            case piFED_YFrtFeed : dPos = PM.GetValue (_iMotr , pvFED_YFrtFeed ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }
    return dPos ;
}

//---------------------------------------------------------------------------
bool CFrontFeed::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    double dLearYCrntPos = MT_GetCmdPos(miFED_YLer) ;
    double dLearYWorkPos = PM.GetValue(miFED_YLer , pvFED_YLerWork ) ;
    double dLearYWaitPos = PM.GetValue(miFED_YLer , pvFED_YLerWait ) ;
    double dOffset       = OM.MstOptn.dOffset ;

    bool   bLearYWorkPos = (dLearYWorkPos - dOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYWorkPos + dOffset) ;
    bool   bLearYWaitPos = (dLearYWaitPos - dOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYWaitPos + dOffset) ;

    //double dFrntTCrntPos = MT_GetCmdPos(miLSR_TFrt) ;
    //double dFrntTWaitPos = GetMotrPos(miLSR_TFrt , piLSR_TFrtWait ) ;
    //bool bFrntTWaitPos = (dFrntTWaitPos - dOffset <= dFrntTCrntPos) && (dFrntTCrntPos <= dFrntTWaitPos + dOffset) ;

    bool bFrntStgFwd  = !AT_Complete(aiLSR_StgF_FwBw, ccBwd) ;

    if(_iMotr == miFED_YFrt){

        switch(_iPstnId) {
            default                :                                                      break ;
            case piFED_YFrtWait    : if( bFrntStgFwd  ) {
                                        sMsg = "Front Stage is FWD"; bRet = false ;}      break ;
            case piFED_YFrtWork    : if( bFrntStgFwd  ) {
                                        sMsg = "Front Stgae is FWD"; bRet = false ;}      break ;
            case piFED_YFrtFeed    : if( bFrntStgFwd  ) {
                                        sMsg = "Front Stgae is FWD"; bRet = false ;}
                                     if(!bLearYWorkPos && !bLearYWaitPos) {
                                        sMsg = "Lear Y Motor is Not WaitPos or Not WorkPos"; bRet = false ;} break ;
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
bool  CFrontFeed::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //bool isXMoving = !MT_GetStopInpos(miWRK_XIdx);

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
        else{

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

bool CFrontFeed::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CFrontFeed::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CFrontFeed::CycleHome()     //sun DLL direct access.
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

                 MT_SetHomeEnd(miFED_YFrt , false);
                 MT_DoHome(miFED_YFrt);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miFED_YFrt)) return false;
                 MT_GoAbsMan(miFED_YFrt, PM.GetValue(miFED_YFrt , pvFED_YFrtWait));
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GetStopInpos(miFED_YFrt)) return false;
                 if(DM.ARAY[riFFD].CheckAllStat(csNone) && DM.ARAY[riFLS].CheckAllStat(csNone)) IO_SetY(yLSR_VccF , false);
                 Step.iHome = 0;
                 return true ;
    }
}

bool CFrontFeed::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        if(OM.CmnOptn.iUseLsrPos ==2) return false;


        //아웃 사이클 조건
        bool bLotOpOutCon = LTQ.GetLotOpen() && DM.ARAY[riFFD].CheckAllStat(csUnkwn) ; //랏 오픈 시 아웃 조건
        bool bLotClOutCon =!LTQ.GetLotOpen() && DM.ARAY[riFFD].CheckAllStat(csUnkwn) ; //랏 종료 시 아웃 조건

        // 인 얼라인 앤드 사이클 공용 조건
        double dLearYCrntPos = MT_GetCmdPos(miFED_YLer) ;

        //인 , 얼라인 사이클 조건
        double dLearYWorkPos = PM.GetValue(miFED_YLer , pvFED_YLerWork ) ;
        double dWrkOffset    = OM.MstOptn.dOffset ;
        bool   bLearYWorkPos = (dLearYWorkPos - dWrkOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYWorkPos + dWrkOffset) ;

        double dLearYWaitPos = PM.GetValue(miFED_YLer , pvFED_YLerWait) ;
        bool   bLearYWaitPos = (dLearYWaitPos - dWrkOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYWaitPos + dWrkOffset) ;


        //엔드 사이클 조건
        double dLearYFeedPos = PM.GetValue(miFED_YLer , pvFED_YLerFeed ) ;
        double dFedOffset    = OM.MstOptn.dOffset * 100;
        bool   bLearYFeedPos = (dLearYFeedPos - dFedOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYFeedPos + dFedOffset) ;

        bool isCycleIn    = DM.ARAY[riFLS].CheckAllStat(csNone ) && DM.ARAY[riFFD].CheckAllStat(csNone ) && (bLearYWorkPos || bLearYWaitPos) &&
                            DM.ARAY[riLFD].CheckAllStat(csNone ) ;
        //bool isCycleAlign = LTQ.GetLotOpen() && DM.ARAY[riFFD].CheckAllStat(csUnkwn) && bLearYWorkPos ;
        bool isCycleOut   = DM.ARAY[riFLS].CheckAllStat(csNone) && bLotOpOutCon || bLotClOutCon ;
        bool isCycleEnd   = !isCycleOut && bLearYFeedPos ; // 이건 수정 가능 성 있음...

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

bool CFrontFeed::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CFrontFeed::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CFrontFeed::ToStart(void) //스타트를 하기 위한 함수.
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

bool CFrontFeed::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: //MoveActr(aiFED_Covr_FwBw , ccBwd);


                 IO_SetY(yFED_Vcc1 , true);

                 //if(EM_GetErr(eiATR_TimeOut)){
                 //    MoveActr(aiLSR_StgF_FwBw , ccBwd);
                 //}


                 Step.iToStop ++ ;
                 return false ;

        case 11: //if(!MoveActr(aiFED_Covr_FwBw , ccBwd))return false ;

                 //if(EM_GetErr(eiATR_TimeOut)){
                 //    if(!MoveActr(aiLSR_StgF_FwBw , ccBwd)) return false;
                 //}
                 Step.iToStop = 0   ;
                 return true ;

    }
}

//중간에 스트립 빠지거나 하면 레디 위치로 복귀.
bool CFrontFeed::CycleIn()
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

    double dFrntTCrntPos = MT_GetCmdPos(miLSR_TFrt) ;
    double dFrntTWaitPos = PM.GetValue(miLSR_TFrt , pvLSR_TFrtWait ) ;
    double dOffset       = OM.MstOptn.dOffset;
    bool   bFrntTWaitPos = (dFrntTWaitPos - dOffset <= dFrntTCrntPos) && (dFrntTCrntPos <= dFrntTWaitPos + dOffset) ;





    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dPreTickTime   = GetTickCount();
                  MoveActr(aiFED_Covr_FwBw , ccBwd);
                  MoveActr(aiFED_Algn_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiFED_Covr_FwBw , ccBwd))return false;
                  if(!MoveActr(aiFED_Algn_FwBw , ccBwd))return false;

                  if(!bFrntTWaitPos){
                      MoveActr(aiLSR_StgF_FwBw , ccBwd);
                      Step.iCycle = 100 ;
                      return false ;
                  }

                  //MoveActr(aiLSR_StgF_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  12: //if(!MoveActr(aiLSR_StgF_FwBw , ccBwd))return false;

                  //FLS.MoveMotr(miLSR_TFrt , piLSR_TFrtWait);

                  Step.iCycle++;
                  return false;

        case  13: //if(!FLS.MoveMotr(miLSR_TFrt , piLSR_TFrtWait))return false;

                  MoveMotr(miFED_YFrt , piFED_YFrtFeed);

                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miFED_YFrt , piFED_YFrtFeed))return false;

                  MoveActr(aiLSR_StgF_FwBw , ccFwd);

                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiLSR_StgF_FwBw , ccFwd)) return false;

                  MoveActr(aiFED_Covr_FwBw , ccFwd);

                  Step.iCycle++;
                  return false;

                  // 스텝 밑에서 사용
        case  16: if(!MoveActr(aiFED_Covr_FwBw , ccFwd)) return false;

                  IO_SetY(yFED_AirBlow1 , true );
                  IO_SetY(yFED_AirBlow2 , true );
                  IO_SetY(yFED_Vcc1     , false);
                  IO_SetY(yFED_Vcc2     , false);
                  IO_SetY(yLSR_VccF     , false);
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  17: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iAirBlowTm)) return false;

                  IO_SetY(yFED_Vcc1     , true );
                  IO_SetY(yFED_Vcc2     , true );
                  IO_SetY(yLSR_VccF     , true );
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
                              EM_SetErr(eiFED_FrntVision);                // 에러
                              m_InspVsnCnt= 0;
                              Step.iCycle = 0;
                              return true;
                          }
                          m_InspVsnCnt++;                             // 검사 카운트 보다 적을 경우
                          Step.iCycle = 16;                           // 다시 에어 불고 검사
                          return false;
                      }
                  }
                  IO_SetY (yLSR_VccF , true );
                  DM.ARAY[riFFD].SetStat(csUnkwn);
                  m_InspVsnCnt= 0;

                  if(DM.ARAY[riFFD].CheckAllStat(csUnkwn)){
                      m_dWorkTotalTm = GetTickCount() - dPreTickTime  ;
                      //dPreTickTime   = GetTickCount();
                  }

                  Step.iCycle=0;
                  return true;


        case 100: if(!MoveActr(aiLSR_StgF_FwBw , ccBwd)) return false;

                  FLS.MoveMotr(miLSR_TFrt , piLSR_TFrtWait);

                  Step.iCycle++;
                  return false;

        case 101: if(!FLS.MoveMotr(miLSR_TFrt , piLSR_TFrtWait)) return false;

                  Step.iCycle=11;
                  return false;

    }
}


bool CFrontFeed::CycleOut()
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

    static DWORD dPreTickTime = GetTickCount();

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

                  MoveActr(aiLSR_StgF_FwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiLSR_StgF_FwBw , ccBwd)) return false;

                  MoveMotr(miFED_YFrt , piFED_YFrtWork);

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miFED_YFrt , piFED_YFrtWork)) return false;

                  DM.ARAY[riFFD].SetStat(csWait);
                  DM.ShiftArrayData(riFFD , riFLS);

                  Step.iCycle=0;
                  return false;
    }
}


void CFrontFeed::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CFrontFeed::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CFrontFeed::CheckStop()
{
    if(!MT_GetStop(miFED_YFrt)) return false ;
    //
    //if(!AT_Done(aiWRK_IdxUpDn)) return false ;

    return true ;
}

void CFrontFeed::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName + ".INI";

}
//---------------------------------------------------------------------------

