
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Work.h"
//#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "PreIdx.h"
#include "PstIdx.h"
//#include "FormRslt.h"
#include "EqpComUnit.h"
//---------------------------------------------------------------------------
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CWork WRK;

CWork::CWork(void)
{
    Init();
}

void CWork::Init()
{
    m_sPartName = "WorkZone " ;

    m_sPreLotNo = "";

    m_iBinNo    =-1 ;
    m_iBinCnt   = 0 ;
    EC.m_bEqpLotEnd = false;
    m_bLotEndCheck = true;

    Reset();
    Load(true);


}
void CWork::Close()
{
    Load(false);
}
void CWork::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CWork::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CWork::~CWork (void)
{
//    Close();
    //FormMain으로  Load(false);
    static int i = 0 ;
    AnsiString sTemp = i ;
    Trace("~CWork",sTemp.c_str());

}                                                                                         

bool CWork::FindChip(int &r , int &c )
{

    return false ;
}

double CWork::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos      = 0.0 ;
    double dBinPitch = PM.GetValue(miWRK_XSrt , ovWRK_XSrtPitch) ;
    double dBinStart = PM.GetValue(miWRK_XSrt , pvWRK_XSrtBin1 ) ;

    if(_iMotr == miWRK_ZPch){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piWRK_ZPchWait    : dPos = PM.GetValue (_iMotr , pvWRK_ZPchWait ); break ;
            case piWRK_ZPchWork    : dPos = PM.GetValue (_iMotr , pvWRK_ZPchWork ); break ;
        }
    }
    else if(_iMotr == miWRK_XSrt){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piWRK_XSrtWait    : dPos = PM.GetValue (_iMotr , pvWRK_ZPchWait ); break ;
            case piWRK_XSrtBin1    : dPos = dBinStart - dBinPitch * 0             ; break ;
            case piWRK_XSrtBin2    : dPos = dBinStart - dBinPitch * 1             ; break ;
            case piWRK_XSrtBin3    : dPos = dBinStart - dBinPitch * 2             ; break ;
            case piWRK_XSrtBin4    : dPos = dBinStart - dBinPitch * 3             ; break ;
            case piWRK_XSrtBin5    : dPos = dBinStart - dBinPitch * 4             ; break ;

        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CWork::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isZPunchDn   = !MT_CmprPos (miWRK_ZPch,PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait));

    bool isAreaSnsr   = IO_GetX(xETC_AreaFront) ;

    if(_iMotr == miWRK_ZPch){
        if(isAreaSnsr) {sMsg = "Safety Area Sensor is Detected"; bRet = false ;}
        switch(_iPstnId) {
            default                : break ;
            case piWRK_ZPchWait    : break ;
            case piWRK_ZPchWork    : break ;
        }
    }
    else if(_iMotr == miWRK_XSrt){
        switch(_iPstnId) {
            default                : break ;
            case piWRK_XSrtWait    : break ;
            case piWRK_XSrtBin1    : break ;
            case piWRK_XSrtBin2    : break ;
            case piWRK_XSrtBin3    : break ;
            case piWRK_XSrtBin4    : break ;
            case piWRK_XSrtBin5    : break ;
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
bool  CWork::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CWork::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CWork::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWork::CycleHome()
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

        case 10: IO_SetY(yWRK_PunchAir,false);
                 MT_Reset(miWRK_XSrt);
                 MT_Reset(miWRK_ZPch);

                 MT_SetServo(miWRK_XSrt,true);
                 MT_SetServo(miWRK_ZPch,true);

                 MT_DoHome(miWRK_XSrt) ;
                 MT_DoHome(miWRK_ZPch) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRK_XSrt))return false ;
                 if(!MT_GetHomeEnd(miWRK_ZPch))return false ;

                 Step.iHome++;
                 return false ;


        case 12: MT_GoAbsMan(miWRK_XSrt , PM.GetValue(miWRK_XSrt , pvWRK_XSrtWait));
                 MT_GoAbsMan(miWRK_ZPch , PM.GetValue(miWRK_ZPch , pvWRK_ZPchWait));

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetStop(miWRK_XSrt))return false ;
                 if(!MT_GetStop(miWRK_ZPch))return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CWork::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;
}

bool CWork::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CWork::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                 MoveMotr(miWRK_XSrt,piWRK_XSrtWait);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait))return false ;
                 if(!MoveMotr(miWRK_XSrt,piWRK_XSrtWait))return false ;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CWork::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: IO_SetY(yWRK_PunchAir,false);
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CWork::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0 ) { //생각해 보니...안되네 나눔.dd
        bool r1 = !DM.ARAY[riPRI].CheckAllStat(csNone) ;
        bool r2 =  DM.ARAY[riPRI].GetStep() == asNeedTrim ;
        bool r3 = !DM.ARAY[riPSI].CheckAllStat(csNone) ;
        bool r4 =  DM.ARAY[riPSI].GetStep() == asNeedTrim ;



        bool isCycleWork   =(!DM.ARAY[riPRI].CheckAllStat(csNone) && DM.ARAY[riPRI].GetStep() == asNeedTrim) ||
                            (!DM.ARAY[riPSI].CheckAllStat(csNone) && DM.ARAY[riPSI].GetStep() == asNeedTrim) ;
        bool isConEnd      = !isCycleWork ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //트림 한방 날리자.
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq   ) {
        default          :                                                             Step.iSeq = scIdle ;  return false ;
        case  scWork   : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork End");Step.iSeq = scIdle ;} return false ;
    }
}


bool CWork::CycleWork(void) //
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

    //그리드 센서 세이프 에러시에 그냥 튕겨 나가게함.
    //안그러면 계속 모터 내려올라고함.
    if(Stat.bReqStop) {
//        if(!DM.ARAY[riPRI].CheckAllStat(csNone)) DM.ARAY[riPRI].GetStep() == asNotReady ;
//        if(!DM.ARAY[riPSI].CheckAllStat(csNone)) DM.ARAY[riPSI].GetStep() == asNotReady ;

        Step.iCycle = 0;
        return true ;
    }

    static CArray * WorkAray ;

    unsigned long lCrntTime ;

    int c ;
    int iTempBin = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 :
                  if(!DM.ARAY[riPRI].CheckAllStat(csNone)) WorkAray = &DM.ARAY[riPRI] ;
                  else                                     WorkAray = &DM.ARAY[riPSI] ;

                  //Sorting Bin Setting.
                  if(OM.CmnOptn.bUseOnly1Bin) {
                      m_iBinNo  = 0 ;
                  }
                  else {

                      if(!DM.ARAY[riPRI].CheckAllStat(csNone)) {

                          if(m_sPreLotNo != WorkAray -> GetLotNo()) {
                              memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));
                              //EndedLot = LT.LotInfo ;
                              iTempBin = m_iBinNo + 1 ;
                              m_iBinNo++ ;
                              if(iTempBin > MAX_BIN) iTempBin = 0 ;
                              if(!CheckBinFull(iTempBin)) {//빈통 안비움 에러.
                                  Step.iCycle = 0 ;
                                  return true ;

                              }
                              m_iBinNo = iTempBin ; //빈통 비움 에러 떴을때 다음 빈으로 한번 더가는 오류 때문에.
                          }
                      }
                  }

                  if(!CheckBin(m_iBinNo)){ //빈통 없음 애러.
                      Step.iCycle = 0 ;
                      return true ;
                  }
                       if(m_iBinNo == 0) MoveMotr(miWRK_XSrt , piWRK_XSrtBin1);
                  else if(m_iBinNo == 1) MoveMotr(miWRK_XSrt , piWRK_XSrtBin2);
                  else if(m_iBinNo == 2) MoveMotr(miWRK_XSrt , piWRK_XSrtBin3);
                  else if(m_iBinNo == 3) MoveMotr(miWRK_XSrt , piWRK_XSrtBin4);
                  else if(m_iBinNo == 4) MoveMotr(miWRK_XSrt , piWRK_XSrtBin5);
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait); // case 13

                  Step.iCycle++;
                  return false ;

        case 11 : if(!MT_GetStop(miWRK_XSrt               )) return false ;
                  if(!MoveMotr  (miWRK_ZPch,piWRK_ZPchWait)) return false ;
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWork);
                  //IO_SetY(yWRK_PunchAir,true);

                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWork)) return false ;
                  IO_SetY(yWRK_PunchAir,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 13 : if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iPunchAirDelay)) return false ;
                  MT_GoAbsMan(miWRK_ZPch,PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait));
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MT_GoAbsMan(miWRK_ZPch,PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait))) return false ;

                  if(m_sPreLotNo != WorkAray -> GetLotNo() && m_bLotEndCheck ) LT.LotOpen (WorkAray -> GetLotNo()) ;
                  m_sPreLotNo = WorkAray -> GetLotNo() ;

                  c = WorkAray ->FindLastCol(csWork);
                  for(int j =0 ; j < OM.DevOptn.iTrimColCnt ; j++ ) {
                      for(int i = 0 ; i < WorkAray ->GetMaxRow() ; i++ )
                          WorkAray ->SetStat(i , c - j, csEmpty); //Led 따먹음.dd
                  }                                                                                   //첫줄이라 그냥 해도 되는데...이렇게함 dd
//                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);

                  //스트립 정보 쓰기 및 랏엔드 체크.
                  if(DM.ARAY[riPRI].CheckAllStat(csNone) && DM.ARAY[riPSI].CheckAllStat(csEmpty)) {

                      //디스플레이용 시간.
                      lCrntTime = GetTickCount() ;
                      m_lStrpCycleTime = lCrntTime - m_lPreCycleTime ;
                      m_lPreCycleTime  = lCrntTime ;

                      LotInfoAdd();
                  }

                  m_iBinCnt = PRI.m_iChipCnt ;

                  if(OM.CmnOptn.bUseOnly1Bin){
                      m_iBin1Cnt += m_iBinCnt ;
                      m_iBinCnt   = 0         ;
                  }
                  else{
                           if(m_iBinNo == 0){m_iBin1Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 1){m_iBin2Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 2){m_iBin3Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 3){m_iBin4Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 4){m_iBin5Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                  }

                  WorkAray -> SetStep(asNotReady) ;
                  IO_SetY(yWRK_PunchAir,false);
                  Step.iCycle = 0;
                  return true ;

/*
                  m_sPreLotNo = WorkAray -> GetLotNo() ;

                  c = WorkAray ->FindLastCol(csWork);
                  for(int j =0 ; j < OM.DevOptn.iTrimColCnt ; j++ ) {
                      for(int i = 0 ; i < WorkAray ->GetMaxRow() ; i++ )
                          WorkAray ->SetStat(i , c - j, csEmpty); //Led 따먹음.dd
                  }                                                                                   //첫줄이라 그냥 해도 되는데...이렇게함 dd
//                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);

                  //스트립 정보 쓰기 및 랏엔드 체크.
                  if(DM.ARAY[riPRI].CheckAllStat(csNone) && DM.ARAY[riPSI].CheckAllStat(csEmpty)) {

                      //디스플레이용 시간.
                      lCrntTime = GetTickCount() ;
                      m_lStrpCycleTime = lCrntTime - m_lPreCycleTime ;
                      m_lPreCycleTime  = lCrntTime ;

                      LotInfoAdd();
                  }

                  m_iBinCnt = PRI.m_iChipCnt ;

                  if(OM.CmnOptn.bUseOnly1Bin){
                      m_iBin1Cnt += m_iBinCnt ;
                      m_iBinCnt   = 0         ;
                  }
                  else{
                           if(m_iBinNo == 0){m_iBin1Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 1){m_iBin2Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 2){m_iBin3Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 3){m_iBin4Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                      else if(m_iBinNo == 4){m_iBin5Cnt += m_iBinCnt ; m_iBinCnt = 0;}
                  }

//                  WorkAray -> SetStep(asNotReady) ;
//                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                  MT_GoAbsMan(miWRK_ZPch,PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait));
                  Step.iCycle++;
                  return false ;

        case 14 : //if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;
                  if(!MT_GoAbsMan(miWRK_ZPch,PM.GetValue(miWRK_ZPch,pvWRK_ZPchWait))) return false ;
                  WorkAray -> SetStep(asNotReady) ;
                  IO_SetY(yWRK_PunchAir,false);
                  Step.iCycle = 0;
                  return true ;
*/
    }
}

void CWork::ClearBin(int _iBinNo)
{
    switch(_iBinNo) {
        default   :  break ;
        case 0 : m_iBin1Cnt = 0 ; break ;
        case 1 : m_iBin2Cnt = 0 ; break ;
        case 2 : m_iBin3Cnt = 0 ; break ;
        case 3 : m_iBin4Cnt = 0 ; break ;
        case 4 : m_iBin5Cnt = 0 ; break ;
    }
}

bool CWork::CheckBin(int _iBinNo)
{
    bool bRet = true ;

    //빈통 있는지 에러.
    switch(_iBinNo) {
        case 0 : if(!IO_GetX(xWRK_Bin1)) {EM_SetErr(eiWRK_Bin1Dispr);  bRet = false ;} break ;
        case 1 : if(!IO_GetX(xWRK_Bin2)) {EM_SetErr(eiWRK_Bin2Dispr);  bRet = false ;} break ;
        case 2 : if(!IO_GetX(xWRK_Bin3)) {EM_SetErr(eiWRK_Bin3Dispr);  bRet = false ;} break ;
        case 3 : if(!IO_GetX(xWRK_Bin4)) {EM_SetErr(eiWRK_Bin4Dispr);  bRet = false ;} break ;
        case 4 : if(!IO_GetX(xWRK_Bin5)) {EM_SetErr(eiWRK_Bin5Dispr);  bRet = false ;} break ;
        default : bRet = false ; break ;
    }

    return bRet ;
}

bool CWork::CheckBinFull(int _iBinNo)
{
    bool bRet = true ;
    bool bBin1 , bBin2 , bBin3 , bBin4, bBin5;
    bBin1 = m_iBin1Cnt == 0;
    bBin2 = m_iBin2Cnt == 0;
    bBin3 = m_iBin3Cnt == 0;
    bBin4 = m_iBin4Cnt == 0;
    bBin5 = m_iBin5Cnt == 0;


    //빈통 안비웠는지 에러.
    if(!bBin1 && !bBin2 && !bBin3 && !bBin4 && !bBin5) {
        EM_SetErr(eiWRK_BinTFull) ;
        bRet = false ;
    }
    switch(_iBinNo) {
        case 0 : if(!bBin1) {EM_SetErr(eiWRK_Bin1Full);  bRet = false ;} break ;
        case 1 : if(!bBin2) {EM_SetErr(eiWRK_Bin2Full);  bRet = false ;} break ;
        case 2 : if(!bBin3) {EM_SetErr(eiWRK_Bin3Full);  bRet = false ;} break ;
        case 3 : if(!bBin4) {EM_SetErr(eiWRK_Bin4Full);  bRet = false ;} break ;
        case 4 : if(!bBin5) {EM_SetErr(eiWRK_Bin5Full);  bRet = false ;} break ;
        default : bRet = false ; break ;
    }

    return bRet ;
}

bool CWork::LotInfoAdd()
{
    int iToolCnt ;
    int iChipCnt = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;

    LT.AddDayInfoWorkStrp(1);
    LT.AddLotInfoWorkStrp(1);
    m_iTotalFrameCnt += 1 ;

    if(OM.MstOptn.bGoodChipAdd) {
        LT.AddDayInfoWorkChip(DM.ARAY[riPSI].GetData() );
        LT.AddLotInfoWorkChip(DM.ARAY[riPSI].GetData());
        m_iTotalChip += DM.ARAY[riPSI].GetData() ;
        LT.AddDayInfoFailChip(iChipCnt - DM.ARAY[riPSI].GetData());
        LT.AddLotInfoFailChip(iChipCnt - DM.ARAY[riPSI].GetData());
    }
    else {
        LT.AddDayInfoWorkChip(iChipCnt );
        LT.AddLotInfoWorkChip(iChipCnt );
        m_iTotalChip += iChipCnt ;
    }

    if(OM.DevInfo.iColCnt % OM.DevOptn.iTrimColCnt) iToolCnt = OM.DevInfo.iColCnt / OM.DevOptn.iTrimColCnt + 1 ;
    else                                            iToolCnt = OM.DevInfo.iColCnt / OM.DevOptn.iTrimColCnt ;

    m_iTotalToolCnt  += iToolCnt ;

    if(m_iTotalChip < 0 || m_iTotalFailChip < 0) {//OverFlow
        m_iTotalChip     = 0 ;
        m_iTotalFailChip = 0 ;
    }

    if(m_iTotalToolCnt < 0) {
        m_iTotalToolCnt  = 0 ;
    }

    SPC.WriteArayData(riPSI); //SPC 용인듯...
/*
    LT.AddDayInfoChips(DM.ARAY[riLOT].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riLOT].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riLOT].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riLOT].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riLOT].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riLOT].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riLOT].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riLOT].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riLOT].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riLOT].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riLOT].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riLOT].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riLOT].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riLOT].GetCntStat(csRslt14) , //"V Flow"
                       DM.ARAY[riLOT].GetCntStat(csFail  ) ); //"Heat Fail");

    LT.AddLotInfoChips(DM.ARAY[riLOT].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riLOT].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riLOT].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riLOT].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riLOT].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riLOT].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riLOT].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riLOT].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riLOT].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riLOT].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riLOT].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riLOT].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riLOT].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riLOT].GetCntStat(csRslt14) ,
                       DM.ARAY[riLOT].GetCntStat(csFail  ) ); //"Heat Fail");//"V Flow"
                  //!DM.ARAY[riWK2].CheckAllStat(csNone) &&
*/
    if(GetLastStrip()) {
        m_bLotEnded = true ;
        m_bLotEndCheck = true ;
        LT.LotInfo.dEndTime = Now();
        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",DM.ARAY[riPSI].GetLotNo().c_str());
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        EC.m_bEqpLotEnd = false;
        Trace("WriteLotLog",DM.ARAY[riPSI].GetLotNo().c_str());
        //LT.LotEnd();
        return true ;
    }
    return false ;

}
bool CWork::GetLastStrip()
{
    AnsiString sLastLotNo = !DM.ARAY[riPSI].CheckAllStat(csNone) ? DM.ARAY[riPSI].GetLotNo() : PSI.m_sPreLotNo ;

    bool bPreEqpLotEnd = EC.m_bEqpLotEnd ;
    bool bINBExist = !DM.ARAY[riINB].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bPRIExist = !DM.ARAY[riPRI].CheckAllStat(csNone) ;

    bool bINBDiff  = sLastLotNo != DM.ARAY[riINB].GetLotNo() ;
    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
    bool bPRIDiff  = sLastLotNo != DM.ARAY[riPRI].GetLotNo() ;

         if(bPRIExist     ) return bPRIDiff ;
    else if(bPRBExist     ) return bPRBDiff ;
    else if(bINBExist     ) return bINBDiff ;
    else if(!bPreEqpLotEnd) return false    ;
    else                    return true     ;

    return false ;
}
void CWork::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CWork::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CWork::CheckStop()
{
    if(!AT_Done(aiPRI_Idx    )) return false ;
    if(!AT_Done(aiPSI_Idx    )) return false ;

    if(!MT_GetStop(miPRI_XIdx)) return false ;
    if(!MT_GetStop(miPSI_XIdx)) return false ;
    if(!MT_GetStop(miWRK_XSrt)) return false ;
    if(!MT_GetStop(miWRK_ZPch)) return false ;

    return true ;
}

void CWork::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBinNo        " , m_iBinNo         );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo     " , m_sPreLotNo      );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin1Cnt      " , m_iBin1Cnt       );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin2Cnt      " , m_iBin2Cnt       );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin3Cnt      " , m_iBin3Cnt       );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin4Cnt      " , m_iBin4Cnt       );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin5Cnt      " , m_iBin5Cnt       );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded     " , m_bLotEnded      );

        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalChip    " , m_iTotalChip     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalFailChip" , m_iTotalFailChip );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalFrameCnt" , m_iTotalFrameCnt );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalToolCnt " , m_iTotalToolCnt  );
    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBinNo        " , m_iBinNo         );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo     " , m_sPreLotNo      );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin1Cnt      " , m_iBin1Cnt       );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin2Cnt      " , m_iBin2Cnt       );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin3Cnt      " , m_iBin3Cnt       );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin4Cnt      " , m_iBin4Cnt       );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBin5Cnt      " , m_iBin5Cnt       );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded     " , m_bLotEnded      );

        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalChip    " , m_iTotalChip     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalFailChip" , m_iTotalFailChip );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalFrameCnt" , m_iTotalFrameCnt );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iTotalToolCnt " , m_iTotalToolCnt  );
    }
}

