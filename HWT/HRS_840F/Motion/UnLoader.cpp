//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "UnLoader.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoader ULD_F;
CUnLoader ULD_R;

CUnLoader::CUnLoader(void)
{

}

CUnLoader::~CUnLoader (void)
{

}

void CUnLoader::Init(bool _bFront )
{
    if(_bFront){
        m_sPartName                = "Front UnLoader"         ;
        Add.iPartId                = piULD_F                  ;
        Add.riULD                  = riULD_F                  ;
        Add.riPCK                  = riPCK_F                  ;

        Add.aiULD_FoldUpDn         = aiULD_FFoldUpDn          ;
        Add.aiULD_PickUpFwBw       = aiULD_FPickUpFwBw        ;
        Add.aiULD_RotateFwBw       = aiULD_FRotateFwBw        ;
        Add.aiULD_PackOpenFwBw     = aiULD_FPackOpenFwBw      ;
        Add.aiULD_PackSrtFwBw      = aiULD_FPackSrtFwBw       ;

        Add.xULD_MaskDetect        = xULD_FMaskDetect         ;
        Add.xULD_PackVccm          = xULD_FPackVccm           ;
        Add.xULD_PickVccm          = xULD_FPickVccm           ;

        Add.yULD_ConMotorCw        = yULD_FConMotorCw         ;
        Add.yULD_PickUpVccm        = yULD_FPickUpVccm         ;
        Add.yULD_PickUpEjet        = yULD_FPickUpEjct         ;
        Add.yULD_PackOpenVccm      = yULD_FPackOpenVccm       ;
        Add.yULD_PackOpenEjet      = yULD_FPackOpenEjct       ;

        Add.bRunSkip               = OM.CmnOptn.bFRunSkip     ;
    }

    else {
        m_sPartName                = "Rear UnLoader"          ;
        Add.iPartId                = piULD_R                  ;
        Add.riULD                  = riULD_R                  ;
        Add.riPCK                  = riPCK_R                  ;
        Add.aiULD_FoldUpDn         = aiULD_RFoldUpDn          ;
        Add.aiULD_PickUpFwBw       = aiULD_RPickUpFwBw        ;
        Add.aiULD_RotateFwBw       = aiULD_RRotateFwBw        ;
        Add.aiULD_PackOpenFwBw     = aiULD_RPackOpenFwBw      ;
        Add.aiULD_PackSrtFwBw      = aiULD_RPackSrtFwBw       ;

        Add.xULD_MaskDetect        = xULD_RMaskDetect         ;
        Add.xULD_PackVccm          = xULD_RPackVccm           ;
        Add.xULD_PickVccm          = xULD_RPickVccm           ;

        Add.yULD_ConMotorCw        = yULD_RConMotorCw         ;
        Add.yULD_PickUpVccm        = yULD_RPickUpVccm         ;
        Add.yULD_PickUpEjet        = yULD_RPickUpEjct         ;
        Add.yULD_PackOpenVccm      = yULD_RPackOpenVccm       ;
        Add.yULD_PackOpenEjet      = yULD_RPackOpenEjct       ;

        Add.bRunSkip               = OM.CmnOptn.bRRunSkip     ;

    }

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CUnLoader::Close()
{
    Load(false);
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CUnLoader::FindChip(int &r , EN_ARAY_ID &id)
{
    return true ;
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;
    //
    //double dWorkStt ; //작업해야할 메거진의 맨 아래 슬롯 위치.
    //double dWork    ; //현재 작업해야할 포지션.
    //int r ;
    //EN_ARAY_ID riWork ;

    //if(FindChip(r,riWork)){
    //    if(riWork == Add.riLDRBtm){
    //        dWorkStt = PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWorkStartBtm) ;
    //        dWork    = dWorkStt + (OM.DevInfo.iMgzSlotCnt - r - 1) * OM.DevInfo.dMgzSlotPitch ;
    //    }
    //    else {
    //        dWorkStt = PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWorkStartTop) ;
    //        dWork    = dWorkStt + (OM.DevInfo.iMgzSlotCnt - r - 1)  * OM.DevInfo.dMgzSlotPitch ;
    //    }
    //}
    //
    //if(_iMotr == Add.miLDR_ZElev){
    //         if(_iPstnId ==  Add.piLDR_ZElevWait        ) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWait         );
    //    else if(_iPstnId ==  Add.piLDR_ZElevWorkStartBtm) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWorkStartBtm );
    //    else if(_iPstnId ==  Add.piLDR_ZElevWorkStartTop) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWorkStartTop );
    //    else if(_iPstnId ==  Add.piLDR_ZElevWork        ) dPos = dWork                                              ;
    //    else                                              dPos = MT_GetCmdPos(_iMotr                               );
    //}
    //else {
    //    dPos = MT_GetCmdPos(_iMotr);
    //}

    return dPos ;
}

void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiULD_ToStartTO, m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;
                              
    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: // 언로더 쪽 Array 가 도저히 떠오르지가 않습니다...
                 Step.iToStart ++;
                 return false;

        case 11:
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiULD_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10: IO_SetY(Add.yULD_ConMotorCw, false);
//                 IO_SetY(Add.yULD_PackOpenVccm, false); // 장비 정지시 마스크 물고 있는 배큠이랑 이젝터를 Off 할 경우 떨어질꺼 같아서 냅둡니다
//                 IO_SetY(Add.yULD_PackOpenEjet, false);
                 Step.iToStop++;
                 return false;

        case 11:
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if(Add.iPartId == piLDR_F) Add.bRunSkip = OM.CmnOptn.bFRunSkip;
    if(Add.iPartId == piLDR_R) Add.bRunSkip = OM.CmnOptn.bRRunSkip;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //bool isCycleSupply = DM.ARAY[Add.riPCK].CheckAllStat(csNone ) && !IO_GetX(Add.xULD_PackVccm) && !IO_GetX(Add.xULD_PickVccm) && !Add.bRunSkip ;
        bool isCycleSupply = DM.ARAY[Add.riPCK].CheckAllStat(csNone ) && !Add.bRunSkip ;
        //bool isCycleIn     =!IO_GetX(Add.xULD_MaskDetect) && !Add.bRunSkip ;
        bool isCycleIn     = DM.ARAY[Add.riULD].CheckAllStat(csNone ) && !Add.bRunSkip ;

        bool isCycleWork   = DM.ARAY[Add.riULD].CheckAllStat(csUnkwn) && DM.ARAY[Add.riPCK].CheckAllStat(csEmpty) && IO_GetX(Add.xULD_MaskDetect) && !Add.bRunSkip  ;
        bool isCycleEnd    = true;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply ) {Step.iSeq = scSupply ; InitCycleStep(); Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //m_iCycleSttTime = GetTime() ;
        else if (isCycleIn     ) {Step.iSeq = scIn     ; InitCycleStep(); Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //m_iCycleSttTime = GetTime() ;
        else if (isCycleWork   ) {Step.iSeq = scWork   ; InitCycleStep(); Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //m_iCycleSttTime = GetTime() ;
        else if (isCycleEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default       :                      Trace(m_sPartName.c_str(),"default     End"); Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                                          return false ;
        case scSupply : if(CycleSupply ()) { Trace(m_sPartName.c_str(),"CycleSupply End"); Step.iSeq = scIdle ;} return false ;
        case scIn     : if(CycleIn     ()) { Trace(m_sPartName.c_str(),"CycleIn     End"); Step.iSeq = scIdle ;} return false ;
        case scWork   : if(CycleWork   ()) { Trace(m_sPartName.c_str(),"CycleWork   End"); Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CUnLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiULD_HomeTO, sTemp.c_str());
        Trace(m_sPartName.c_str(), sTemp.c_str());
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(Add.yULD_PackOpenVccm, false);
                 m_tmDelay.Clear();
                 Step.iHome++;
                 return false;

        case 11: if(!m_tmDelay.OnDelay(true, 200)) return false ;
                 AT_MoveCyl(Add.aiULD_FoldUpDn     , ccBwd);
                 AT_MoveCyl(Add.aiULD_PickUpFwBw   , ccBwd);
                 AT_MoveCyl(Add.aiULD_PackOpenFwBw , ccBwd);
                 AT_MoveCyl(Add.aiULD_PackSrtFwBw  , ccBwd);

                 IO_SetY(Add.yULD_ConMotorCw, false);
                 Step.iHome++;
                 return false;

        case 12: if(!AT_MoveCyl(Add.aiULD_FoldUpDn     , ccBwd)) return false;
                 if(!AT_MoveCyl(Add.aiULD_PickUpFwBw   , ccBwd)) return false;
                 if(!AT_MoveCyl(Add.aiULD_PackOpenFwBw , ccBwd)) return false;
                 if(!AT_MoveCyl(Add.aiULD_PackSrtFwBw  , ccBwd)) return false;
                 AT_MoveCyl(Add.aiULD_RotateFwBw   , ccBwd);
                 Step.iHome++;
                 return false;

        case 13: if(!AT_MoveCyl(Add.aiULD_RotateFwBw   , ccBwd)) return false;
                 DM.ARAY[Add.riPCK].SetStat(csNone);
                 Step.iHome = 0;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CUnLoader::CycleIn()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiULD_CycleTO , sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(Add.yULD_ConMotorCw , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(m_tmDelay.OnDelay(true , 5500)){
                      Step.iCycle=0;
                      return false ;
                  }
                  if(!IO_GetX(Add.xULD_MaskDetect)) return false ;
                  IO_SetY(Add.yULD_ConMotorCw , false);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.dUldMskDetSnrDely)) return false ;
                  DM.ARAY[Add.riULD].SetStat(csUnkwn);
                  Step.iCycle=0;
                  return true;
    }
}

bool CUnLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiULD_CycleTO , sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //IO_SetY(Add.yULD_PickUpVccm  , false);
                  IO_SetY(Add.yULD_PickUpEjet    , false);
                  IO_SetY(Add.yULD_PackOpenVccm  , false);
                  IO_SetY(Add.yULD_PackOpenEjet  , false);
                  MoveActr(Add.aiULD_PickUpFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(Add.aiULD_PickUpFwBw, ccBwd)) return false;
                  MoveActr(Add.aiULD_RotateFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(Add.aiULD_RotateFwBw, ccBwd)) return false;
                  //IO_SetY(Add.yULD_ConMotorCw, true); // juhyeon, Rail Part Autorun으로 조건 처리함.
                  Step.iCycle++;
                  return false;

        case  13: MoveActr(Add.aiULD_PickUpFwBw, ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(Add.aiULD_PickUpFwBw, ccFwd)) return false;
                  IO_SetY(Add.yULD_PickUpVccm, true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmDelay.OnDelay(true, OM.CmnOptn.dPickVccDely)) return false;
                  MoveActr(Add.aiULD_PickUpFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(Add.aiULD_PickUpFwBw, ccBwd)) return false;

                  if(!IO_GetX(Add.xULD_PickVccm)){
                      if(Add.iPartId == piULD_F)EM_SetErrMsg(eiULD_PickErr , "앞쪽 언로더 봉지를 집지 못했습니다.");
                      else                      EM_SetErrMsg(eiULD_PickErr , "뒷쪽 언로더 봉지를 집지 못했습니다.");
                      Step.iCycle=0;
                      return false ;
                  }
                  MoveActr(Add.aiULD_RotateFwBw, ccFwd);

                  //아직 준비가 다 되있지 않은 상태에서 어레이를 만들면 이 전에것들이
                  //동작을 할수 있어서 꼬일수가 있다 준비가 되면 어레이 만져야 한다.
                  //DM.ARAY[Add.riULD_PRE].SetStat(csUnkwn);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(Add.aiULD_RotateFwBw, ccFwd)) return false;
                  MoveActr(Add.aiULD_PickUpFwBw  , ccFwd); // 왠지 이 부분에서 실린더 충돌 있을꺼 같은 느낌이...
                  MoveActr(Add.aiULD_PackSrtFwBw , ccFwd);
                  MoveActr(Add.aiULD_PackOpenFwBw, ccFwd);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveActr(Add.aiULD_PickUpFwBw ,  ccFwd)) return false;
                  if(!MoveActr(Add.aiULD_PackSrtFwBw,  ccFwd)) return false;
                  if(!MoveActr(Add.aiULD_PackOpenFwBw, ccFwd)) return false;
                  IO_SetY(Add.yULD_PackOpenVccm, true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  19: if(!m_tmDelay.OnDelay(true, OM.CmnOptn.dPackOpenVccDely)) return false;
                  MoveActr(Add.aiULD_PackOpenFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  20: if(!MoveActr(Add.aiULD_PackOpenFwBw, ccBwd)) return false;
                  if(!IO_GetX(Add.xULD_PackVccm)){
                      if(Add.iPartId == piULD_F)EM_SetErrMsg(eiULD_PickErr , "앞쪽 언로더 봉지를 벌리지 못했습니다. 봉지를 제거해 주세요");
                      else                      EM_SetErrMsg(eiULD_PickErr , "뒷쪽 언로더 봉지를 벌리지 못했습니다. 봉지를 제거해 주세요");
                      Step.iCycle=0;
                      return false ;
                  }
                  //IO_SetY(Add.yULD_ConMotorCw, false);
                  DM.ARAY[Add.riPCK].SetStat(csEmpty);
                  Step.iCycle = 0 ;
                  return true;

    }

}
//---------------------------------------------------------------------------

bool CUnLoader::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiULD_CycleTO , sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(Add.aiULD_FoldUpDn, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(Add.aiULD_FoldUpDn, ccBwd)) return false;
                  if(m_iBin2Cnt >= OM.CmnOptn.iBin2FullMaxCnt &&
                     m_iBin1Cnt >= OM.CmnOptn.iBin1FullMaxCnt){
                      //Error.
                      EM_SetErrMsg(eiULD_BinMaxOverErr, "1번, 2번 Bin 모두 꽉 찼습니다." );
                      Step.iCycle = 0;
                      return true;
                  }
                  Step.iCycle++;
                  return false;

        case  12: MoveActr(Add.aiULD_FoldUpDn, ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(Add.aiULD_FoldUpDn, ccFwd)) return false;
                  MoveActr(Add.aiULD_FoldUpDn ,  ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(Add.aiULD_FoldUpDn , ccBwd)) return false ;
                  DM.ARAY[Add.riPCK].SetStat(csNone);
                  DM.ARAY[Add.riULD].SetStat(csNone);
                  IO_SetY(Add.yULD_PickUpVccm, false);
                  IO_SetY(Add.yULD_PickUpEjet, true );
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmDelay.OnDelay(true, OM.CmnOptn.dPickEjctDely)) return false ;
                  SPC.LOT.AddWorkCnt();
                  Step.iCycle++;
                  return false;

        case  16: // 누적 마스크 개수가 150개되면 첫 번째 박스로 배출해야함, 평소에는 두 번째 박스에 배출.
                  if(m_iBin2Cnt < OM.CmnOptn.iBin2FullMaxCnt) {
                      m_iBin2Cnt++;
                      Step.iCycle++;
                      return false ;
                  }

                  if(m_iBin2Cnt >= OM.CmnOptn.iBin2FullMaxCnt) {
                      m_iBin1Cnt++;
                      Step.iCycle = 30 ;
                      return false ;
                  }

        case  17: MoveActr(Add.aiULD_PackOpenFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveActr(Add.aiULD_PackOpenFwBw, ccBwd)) return false;
                  Step.iCycle++;
                  return false;

        case  19: IO_SetY(Add.yULD_PackOpenVccm, false);
                  IO_SetY(Add.yULD_PackOpenEjet, true );
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  20: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.dPackEjctDely)) return false ;
                  IO_SetY(Add.yULD_PickUpEjet  , false);
                  IO_SetY(Add.yULD_PackOpenEjet, false);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  21: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.dPickBwBfrDely)) return false ;
                  MoveActr(Add.aiULD_PickUpFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveActr(Add.aiULD_PickUpFwBw, ccBwd)) return false;
                  Step.iCycle = 0;
                  return true ;

                  /* 2번 Bin 으로 배출하는 Cycle */
        case  30: MoveActr(Add.aiULD_PackOpenFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  31: if(!MoveActr(Add.aiULD_PackOpenFwBw, ccBwd)) return false;
                  MoveActr(Add.aiULD_PackSrtFwBw, ccBwd);
                  Step.iCycle++;
                  return false;

        case  32: if(!MoveActr(Add.aiULD_PackSrtFwBw, ccBwd)) return false;
                  IO_SetY(Add.yULD_PackOpenVccm, false);
                  IO_SetY(Add.yULD_PackOpenEjet, true );
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  33: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.dPackEjctDely)) return false ;
                  IO_SetY(Add.yULD_PickUpEjet  , false);
                  IO_SetY(Add.yULD_PackOpenEjet, false);
                  Step.iCycle = 0 ;
                  return true;
    }
}
//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

//    if(_iMotr == Add.miLDR_ZElev){
//        if(IO_GetX(Add.xPRI_Detect1)) {
//            sMsg = m_sPartName + " 레일 진입부 센서 감지 상태로 보트 돌출 확인하세요" ;
//            bRet = false;
//        }
//        if(MT_GetCmdPos(Add.miPRI_XIndx) > PM.GetValue(Add.miPRI_XIndx , Add.pvPRI_XWait)) {
//            sMsg = m_sPartName + "프리 인덱스의 위치가 Wait위치 보다 높습니다.";
//            bRet = false;
//        }
//    }
//    else {
//        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
//        bRet = false ;
//    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool bPickUpFwBw = !AT_Complete(Add.aiULD_PickUpFwBw , ccBwd);

    if(_iActr == Add.aiULD_FoldUpDn    ){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == Add.aiULD_PickUpFwBw  ){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == Add.aiULD_RotateFwBw    ){
        if(bPickUpFwBw) { sMsg = AnsiString("UnLoader PickUp Cylinder is not Bwd"); bRet = false ; }
    }
    else if(_iActr == Add.aiULD_PackOpenFwBw   ){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == Add.aiULD_PackSrtFwBw ){
        if(_bFwd == ccFwd) {
        }
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}
//---------------------------------------------------------------------------
void CUnLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}
//---------------------------------------------------------------------------
bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
//---------------------------------------------------------------------------
void CUnLoader::SetLastCmd()
{
//    m_dLastIdxPos = MT_GetCmdPos(Add. miLDR_ZElev);
    return ; //이파트는 필요 없다.

}
//---------------------------------------------------------------------------
bool CUnLoader::CheckMoved()
{
//    if(m_dLastIdxPos != MT_GetCmdPos(Add.miLDR_ZElev)) return true  ; //이파트는 필요 없다.
//    else                                               return false ;

}
//---------------------------------------------------------------------------
bool CUnLoader::CheckStop()
{
//    if(!MT_GetStop(Add.miLDR_ZElev)) return false ;

    return true ;
}
//---------------------------------------------------------------------------
void CUnLoader::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
        UserINI.Load(sPath, "Member" , "m_iBin1Cnt"    , m_iBin1Cnt );
        UserINI.Load(sPath, "Member" , "m_iBin2Cnt"    , m_iBin2Cnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
        UserINI.Save(sPath, "Member" , "m_iBin1Cnt"    , m_iBin1Cnt );
        UserINI.Save(sPath, "Member" , "m_iBin2Cnt"    , m_iBin2Cnt );
    }
}
//---------------------------------------------------------------------------
