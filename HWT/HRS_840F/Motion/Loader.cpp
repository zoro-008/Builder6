//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
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
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLoader LDR_F;
CLoader LDR_R;

CLoader::CLoader(void)
{

}

CLoader::~CLoader (void)
{

}

void CLoader::Init(bool _bFront )
{

    if(_bFront){
        m_sPartName                 = "Front Loader"           ;
        Add.iPartId                 = piLDR_F                  ;
        Add.riLDR                   = riLDR_F                  ;
        
//        Add.riRAL                   = riPRE_F                  ;

        Add.aiLDR_GrprFwBw          = aiLDR_FGrprFwBw          ;
        Add.aiLDR_PickUpFwBw        = aiLDR_FPickUpFwBw        ;
        Add.aiLDR_GrprUpDn          = aiLDR_FGrprUpDn          ;
        Add.aiLDR_GrprXFwBw         = aiLDR_FGrprXFwBw         ;
        Add.aiLDR_LoadingFwBw       = aiLDR_FLoadingFwBw       ;

        Add.xLDR_MaskDetect         = xLDR_FMaskDetect         ;
        Add.xLDR_GrprOpenSnr        = xLDR_FGrprOpenSnr        ;
        Add.xLDR_MTPLimit           = xLDR_FMaskUpLmt          ;
        Add.xLDR_MTNLimit           = xLDR_FMaskDnLmt          ;

        Add.yLDR_MaskUpDnCw         = yLDR_FMaskUpDnCw         ;
        Add.yLDR_MaskUpDnCcw        = yLDR_FMaskUpDnCcw        ;
        Add.yLDR_AirBlowOn          = yLDR_FAirBlowOn          ;

        Add.bRunSkip                = OM.CmnOptn.bFRunSkip     ;

    }
    else {
        m_sPartName                 = "Rear Loader"            ;
        Add.iPartId                 = piLDR_R                  ;
        Add.riLDR                   = riLDR_R                  ;

//        Add.riRAL                   = riPRE_R                  ;

        Add.aiLDR_GrprFwBw          = aiLDR_RGrprFwBw          ;
        Add.aiLDR_PickUpFwBw        = aiLDR_RPickUpFwBw        ;
        Add.aiLDR_GrprUpDn          = aiLDR_RGrprUpDn          ;
        Add.aiLDR_GrprXFwBw         = aiLDR_RGrprXFwBw         ;
        Add.aiLDR_LoadingFwBw       = aiLDR_RLoadingFwBw       ;

        Add.xLDR_MaskDetect         = xLDR_RMaskDetect         ;
        Add.xLDR_GrprOpenSnr        = xLDR_RGrprOpenSnr        ;
        Add.xLDR_MTPLimit           = xLDR_RMaskUpLmt          ;
        Add.xLDR_MTNLimit           = xLDR_RMaskDnLmt          ;

        Add.yLDR_MaskUpDnCw         = yLDR_RMaskUpDnCw         ;
        Add.yLDR_MaskUpDnCcw        = yLDR_RMaskUpDnCcw        ;
        Add.yLDR_AirBlowOn          = yLDR_RAirBlowOn          ;

        Add.bRunSkip                = OM.CmnOptn.bRRunSkip     ;
    }

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CLoader::Close()
{
    Load(false);
}

void CLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoader::FindChip(int &r , EN_ARAY_ID &id)
{
    return true ;
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
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

void CLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiLDR_ToStartTO, m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: //IO_SetY(Add.yLDR_MaskUpDnCw, true);
                 //lay.Clear();
                 Step.iToStart++;
                 return false;

        case 11: //if(IO_GetX(Add.xLDR_MaskDetect)){
                 //     Step.iToStart++;
                 //     return false;
                 // }
                 // if(!m_tmDelay.OnDelay(true , 2000))return false;
                  Step.iToStart++;
                  return false;

        case 12: //IO_SetY(Add.yLDR_MaskUpDnCw , false);
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiLDR_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10:

                 Step.iToStop++;
                 return false;

        case 11:
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool isCycleSupply = !IO_GetX(Add.xLDR_MaskDetect) && !OM.CmnOptn.bLoadingStop && !Add.bRunSkip ;
        bool isCycleWork   = DM.ARAY[Add.riLDR].CheckAllStat(csNone) && !OM.CmnOptn.bLoadingStop && IO_GetX(Add.xLDR_MaskDetect) && !Add.bRunSkip ;
        bool isCycleEnd    = DM.ARAY[Add.riLDR].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Step.iSeq = scSupply ; InitCycleStep(); Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //m_iCycleSttTime = GetTime() ;
        else if (isCycleWork  ) {Step.iSeq = scWork   ; InitCycleStep(); Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //m_iCycleSttTime = GetTime() ;
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default       :                      Trace(m_sPartName.c_str(),"default     End"); Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                                          return false ;
        case scSupply : if(CycleSupply ()) { Trace(m_sPartName.c_str(),"CycleSupply End"); Step.iSeq = scIdle ;} return false ;
        case scWork   : if(CycleWork   ()) { Trace(m_sPartName.c_str(),"CycleWork   End"); Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiLDR_HomeTO, sTemp.c_str());
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

        case 10: AT_MoveCyl(Add.aiLDR_GrprFwBw    , ccBwd);
                 AT_MoveCyl(Add.aiLDR_PickUpFwBw  , ccBwd);
                 AT_MoveCyl(Add.aiLDR_GrprUpDn    , ccBwd);
                 AT_MoveCyl(Add.aiLDR_GrprXFwBw   , ccBwd);
                 AT_MoveCyl(Add.aiLDR_LoadingFwBw , ccBwd);
                  Step.iHome++;
                 return false;

        case 11: if(!AT_MoveCyl(Add.aiLDR_GrprFwBw    , ccBwd))return false;
                 if(!AT_MoveCyl(Add.aiLDR_PickUpFwBw  , ccBwd))return false;
                 if(!AT_MoveCyl(Add.aiLDR_GrprUpDn    , ccBwd))return false;
                 if(!AT_MoveCyl(Add.aiLDR_GrprXFwBw   , ccBwd))return false;
                 if(!AT_MoveCyl(Add.aiLDR_LoadingFwBw , ccBwd))return false;
                 Step.iHome = 0;
                 return true ;

    }
}
//---------------------------------------------------------------------------
bool CLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiLDR_CycleTO , sTemp.c_str());
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


        case  10: IO_SetY(Add.yLDR_MaskUpDnCw, true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(IO_GetX(Add.xLDR_MaskDetect)){
                      Step.iCycle++;
                      return false;
                  }
                  if(IO_GetX(Add.xLDR_MTPLimit) && !IO_GetX(Add.xLDR_MaskDetect)){
                      EM_SetErrMsg(eiLDR_Supply , "마스크가 없습니다.");
                      Step.iCycle++;
                      return false;
                  }
                  if(!IO_GetX(Add.xLDR_MTPLimit)) return false;

        case  12: IO_SetY(Add.yLDR_MaskUpDnCw , false);
                  Step.iCycle=0;
                  return true ;
    }

}

//---------------------------------------------------------------------------
bool CLoader::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiLDR_HomeTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

    int r ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //MoveActr(Add.aiLDR_BrushFwBw   , ccBwd);
                  //MoveActr(Add.aiLDR_ClampFwBw   , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: //if(!MoveActr(Add.aiLDR_BrushFwBw   , ccBwd)) return false;
                  //if(!MoveActr(Add.aiLDR_ClampFwBw   , ccBwd)) return false;
                  MoveActr(Add.aiLDR_LoadingFwBw , ccBwd);
                  MoveActr(Add.aiLDR_PickUpFwBw  , ccBwd);
                  MoveActr(Add.aiLDR_GrprXFwBw   , ccBwd);
                  MoveActr(Add.aiLDR_GrprUpDn    , ccBwd);
                  MoveActr(Add.aiLDR_GrprFwBw    , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(Add.aiLDR_LoadingFwBw , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_PickUpFwBw  , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_GrprXFwBw   , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_GrprUpDn    , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_GrprFwBw    , ccBwd)) return false;
                  //MoveActr(Add.aiLDR_BrushFwBw , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: //if(!MoveActr(Add.aiLDR_BrushFwBw , ccFwd)) return false;
                  //MoveActr(Add.aiLDR_BrushFwBw , ccBwd);
                  MoveActr(Add.aiLDR_PickUpFwBw , ccFwd);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: //if(!MoveActr(Add.aiLDR_BrushFwBw , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_PickUpFwBw , ccFwd)) return false;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.dLdrPickDely)) return false;
                  MoveActr(Add.aiLDR_PickUpFwBw , ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(Add.aiLDR_PickUpFwBw , ccBwd)) return false;
                  IO_SetY(Add.yLDR_AirBlowOn , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  17: if(!m_tmDelay.OnDelay(true , 500)) return false;
                  IO_SetY(Add.yLDR_AirBlowOn , false);
                  DM.ARAY[Add.riLDR].SetStat(csUnkwn);
                  MoveActr(Add.aiLDR_LoadingFwBw , ccFwd);
                  Step.iCycle++;
                  return false;

        case  18: MoveActr(Add.aiLDR_GrprXFwBw , ccFwd);
                  Step.iCycle++;
                  return false;

        case  19: if(!MoveActr(Add.aiLDR_GrprXFwBw , ccFwd)) return false;
                  MoveActr(Add.aiLDR_GrprFwBw , ccFwd);
                  Step.iCycle++;
                  return false;

        case  20: if(!MoveActr(Add.aiLDR_GrprFwBw , ccFwd)) return false;
                  MoveActr(Add.aiLDR_GrprUpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  21: if(!MoveActr(Add.aiLDR_GrprUpDn , ccFwd)) return false;
                  MoveActr(Add.aiLDR_GrprXFwBw , ccBwd);
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveActr(Add.aiLDR_GrprXFwBw , ccBwd)) return false;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  23: if(m_tmDelay.OnDelay(true , 3000)) {
                      EM_SetErrMsg(eiLDR_PlaceFail, "Mask Place Timming Error");
                      Step.iCycle = 0;
                      return true;
                  }
                  if(!IO_GetX(Add.xLDR_GrprOpenSnr)) return false;
                  MoveActr(Add.aiLDR_GrprFwBw , ccBwd);
                  Step.iCycle++;
                  return false;

        case  24: if(!MoveActr(Add.aiLDR_GrprFwBw , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_LoadingFwBw , ccFwd)) return false;
                  MoveActr(Add.aiLDR_GrprXFwBw   , ccBwd);
                  MoveActr(Add.aiLDR_GrprFwBw    , ccBwd);
                  MoveActr(Add.aiLDR_GrprUpDn    , ccBwd);
                  MoveActr(Add.aiLDR_PickUpFwBw  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  25: if(!MoveActr(Add.aiLDR_GrprXFwBw   , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_GrprFwBw    , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_GrprUpDn    , ccBwd)) return false;
                  if(!MoveActr(Add.aiLDR_PickUpFwBw  , ccBwd)) return false;
                  MoveActr(Add.aiLDR_LoadingFwBw , ccBwd);
                  DM.ShiftArrayData(Add.riLDR, Add.riRAL);
                  Step.iCycle++;
                  return false;

        case  26: if(!MoveActr(Add.aiLDR_LoadingFwBw , ccBwd)) return false;
                  Step.iCycle = 0 ;
                  return true;

    }

}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
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
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool bLoadingFwd = !AT_Complete(Add.aiLDR_LoadingFwBw , ccBwd);

    if(_iActr == Add.aiLDR_GrprFwBw    ){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == Add.aiLDR_PickUpFwBw  ){
        if(bLoadingFwd) { sMsg = AnsiString("Loading Cylinder is not Bwd"); bRet = false ; }
    }
    else if(_iActr == Add.aiLDR_GrprUpDn    ){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == Add.aiLDR_GrprXFwBw   ){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == Add.aiLDR_LoadingFwBw ){
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

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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

void CLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoader::SetLastCmd()
{
//    m_dLastIdxPos = MT_GetCmdPos(Add.miLDR_ZElev);
    return ; //이파트는 필요 없다.

}

bool CLoader::CheckMoved()
{
//    if(m_dLastIdxPos != MT_GetCmdPos(Add.miLDR_ZElev)) return true  ; //이파트는 필요 없다.
//    else                                               return false ;

}

bool CLoader::CheckStop()
{
//    if(!MT_GetStop(Add.miLDR_ZElev)) return false ;

    return true ;
}

void CLoader::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}









