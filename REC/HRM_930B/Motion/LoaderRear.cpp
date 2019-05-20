//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LoaderRear.h"
#include "Index.h"
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
CLoaderRear LDR_R;

CLoaderRear::CLoaderRear(void)
{

}

CLoaderRear::~CLoaderRear (void)
{

}

void CLoaderRear::Init()
{
    m_sPartName = "Module Magazine" ;         //20160419 수정 진섭

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CLoaderRear::Close()
{
    Load(false);
}

void CLoaderRear::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoaderRear::FindChip(int &r ,int &c , EN_CHIP_STAT _eStat)
{
    if(_eStat == csUnkwn) { //꺼내 갈때.
        if(-1 != DM.ARAY[riLDR_R].FindFrstRowLastCol(_eStat , r , c)){
            return true ;
        }
    }
    else if(_eStat == csMask) { //내려 놓을때  갈때.
        if(-1 != DM.ARAY[riLDR_R].FindFrstRowLastCol(_eStat , r , c)){
            return true ;
        }
    }
    else if(_eStat == csDetect) { //한번 훌고갈때.
        if(-1 != DM.ARAY[riLDR_R].FindFrstRowLastCol(_eStat , r , c)){
            return true ;
        }
    }

    return false ;
}

double CLoaderRear::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dPickStt  ; //작업해야할 메거진의 맨 아래 슬롯 위치.
    double dPlceStt  ; //작업해야할 메거진의 맨 아래 슬롯 위치.
    double dDetectStt;
    double dPick     ; //현재 작업해야할 포지션.
    double dPlce     ; //현재 작업해야할 포지션.
    double dDetect   ; //자재확인 위치.
    int r , c;
    EN_ARAY_ID riWork ;

    if(FindChip(r,c,csUnkwn )){
        dPickStt = PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRPickFrst) ;
        dPick    = dPickStt + r * OM.DevInfo.dLDRRMgzSlotPitch ;
    }
    if(FindChip(r,c,csMask  )){
        dPlceStt = PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRPlceFrst) ;
        dPlce    = dPlceStt + r * OM.DevInfo.dLDRRMgzSlotPitch ;
    }

    if(FindChip(r, c, csDetect)){
        dDetectStt = PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRCheckFrst) ;
        dDetect  = dDetectStt + r * OM.DevInfo.dLDRRMgzSlotPitch ;
    }

    if(_iMotr == miLDR_ZElevR){
             if(_iPstnId ==  piLDR_ZElevRWait        ) dPos = PM.GetValue (_iMotr , pvLDR_ZElevRWait     );
        else if(_iPstnId ==  piLDR_ZElevRPickFrst    ) dPos = PM.GetValue (_iMotr , pvLDR_ZElevRPickFrst );
        else if(_iPstnId ==  piLDR_ZElevRPlceFrst    ) dPos = PM.GetValue (_iMotr , pvLDR_ZElevRPlceFrst );
        else if(_iPstnId ==  piLDR_ZElevRCheckFrst   ) dPos = PM.GetValue (_iMotr , pvLDR_ZElevRCheckFrst);
        else if(_iPstnId ==  piLDR_ZElevRPick        ) dPos = dPick                                       ;
        else if(_iPstnId ==  piLDR_ZElevRPlce        ) dPos = dPlce                                       ;
        else if(_iPstnId ==  piLDR_ZElevRDetect      ) dPos = dDetect                                     ;
        else                                           dPos = MT_GetCmdPos(_iMotr                        );
    }
    else if(_iMotr == miLDR_YConvR){
             if(_iPstnId ==  piLDR_YConvRWait    ) dPos = PM.GetValue (_iMotr , pvLDR_YConvRWait    );
        else if(_iPstnId ==  piLDR_YConvRWork    ) dPos = PM.GetValue (_iMotr , pvLDR_YConvRWork    );
        else                                       dPos = MT_GetCmdPos(_iMotr                       );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CLoaderRear::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoaderRear::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoaderRear::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoaderRear::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: //if(IO_GetX(xPRI_Detect1)) {
                 //    EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "보트 충돌 방지 센서 감지중").c_str());
                 //    Step.iToStart = 0 ;
                 //    return true ;
                 //}

                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MT_GetStopInpos(miLDR_ZElev)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoaderRear::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10: //if(IO_GetX(xPRI_Detect1)){
                 //    Step.iCycle = 0;
                 //    return true;
                 //}


                 MoveMotr(miLDR_ZElevR , piLDR_ZElevRWait);
                 Step.iToStop++;
                 return false;

        case 11: if(!MoveMotr(miLDR_ZElevR , piLDR_ZElevRWait))return false;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoaderRear::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;
                                                              // CMOS Detect
        //if( DM.ARAY[riLDR_R].CheckAllStat(csNone) &&  IO_GetX(xIDX_Detect)) {EM_SetErrMsg(eiPKG_Unknwn , "Loader Rear Unknwn PKG Error."   ); return false;}
        //if(!DM.ARAY[riLDR_R].CheckAllStat(csNone) && !IO_GetX(xIDX_Detect)) {EM_SetErrMsg(eiPKG_Dispr  , "Loader Rear Disappear PKG Error."); return false;}

        //sunsun
        const int iWorkMode = OM.CmnOptn.iWorkMode ; //0:노말 1:측정.

        bool isZPickPos   = MT_CmprPos(miLDR_ZElevR  ,GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPick   )) ;
        bool isZDetectPos = MT_CmprPos(miLDR_ZElevR  ,GetMotrPos(miLDR_ZElevR , piLDR_ZElevRDetect )) ;
        bool isZPlcePos   = MT_CmprPos(miLDR_ZElevR  ,GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPlce   )) ;
        bool isSafeIdx    = MT_GetCmdPos(miIDX_XIndxR) <= GetMotrPos(miIDX_XIndxR , piIDX_XIndxRGetWait) ;

        bool isCycleDetect = DM.ARAY[riLDR_R].GetCntStat(csDetect) &&
                             IDX.GetSeqStep() != CIndex::scGet   &&
                             IDX.GetSeqStep() != CIndex::scPlace ;

        bool isCyclePick = DM.ARAY[riLDR_R].GetCntStat(csUnkwn) && !isZPickPos && isSafeIdx &&
                          !DM.ARAY[riLDR_R].GetCntStat(csMask ) &&
                          !DM.ARAY[riLDR_R].GetCntStat(csDetect)&&
                           IDX.GetSeqStep() != CIndex::scGet    &&
                           IDX.GetSeqStep() != CIndex::scPlace  ;
                           
        bool isCyclePlce = iWorkMode == wmHeight &&
                           DM.ARAY[riLDR_R].GetCntStat (csMask ) && !isZPlcePos && isSafeIdx &&
                           IDX.GetSeqStep() != CIndex::scGet &&
                           IDX.GetSeqStep() != CIndex::scPlace;

        bool isCycleEnd  = DM.ARAY[riLDR_R].GetCntStat(csUnkwn)  == 0 &&
                           DM.ARAY[riLDR_R].GetCntStat(csMask )  == 0 &&
                           DM.ARAY[riLDR_R].GetCntStat(csDetect) == 0 ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleDetect) {Trace(m_sPartName.c_str(),"CycleDetect Stt"); Step.iSeq = scDetect ; InitCycleStep(); m_iCycleSttTime=GetTime();}
        else if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; InitCycleStep(); m_iCycleSttTime=GetTime();}
        else if (isCyclePlce  ) {Trace(m_sPartName.c_str(),"CyclePlce   Stt"); Step.iSeq = scPlce   ; InitCycleStep(); m_iCycleSttTime=GetTime();}
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                          return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scDetect: if(CycleDetect()){ Trace(m_sPartName.c_str(),"CycleDetect End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlce  : if(CyclePlce  ()){ Trace(m_sPartName.c_str(),"CyclePlce   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}
//---------------------------------------------------------------------------
bool CLoaderRear::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: //if(IO_GetX(xPRI_Detect1)) {
                 //     EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " 레일 진입부 센서 감지 상태로 보트 돌출 확인하세요").c_str());
                 //     Step.iHome = 0 ;
                 //     return true ;
                 //}
                 MT_SetHomeEnd(miLDR_YConvR, true);
                 MT_SetPos(miLDR_YConvR, PM.GetValue(miLDR_YConvR, pvLDR_YConvRWork));

                 MT_DoHome(miLDR_ZElevR);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miLDR_ZElevR)) return false ;
                 MT_GoAbsMan(miLDR_ZElevR , PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRWait ));
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miLDR_ZElevR , PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRWait ))) return false ;
                 SetLastCmd();
                 MT_SetHomeEnd(miLDR_YConvR , true);
                 MT_SetPos(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork));
                 Step.iHome = 0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CLoaderRear::CycleDetect()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

    int r, c;

    FindChip(r, c, csDetect);

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveMotr(miLDR_ZElevR , piLDR_ZElevRDetect);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_ZElevR , piLDR_ZElevRDetect)) return false ;
                  Step.iCycle++;
                  return false;

        case  12: if(OM.DevInfo.iLDRRMgzBayCnt == 1) {
                      if( IO_GetX(xLDR_RPcbDetectR)) {DM.ARAY[riLDR_R].SetStat(r, 0, csUnkwn);}
                      else                           {DM.ARAY[riLDR_R].SetStat(r, 0, csEmpty);}
                  }
                  else if(OM.DevInfo.iLDRFMgzBayCnt == 2) {
                      if( IO_GetX(xLDR_RPcbDetectR)) {DM.ARAY[riLDR_R].SetStat(r, 1, csUnkwn);}
                      else                           {DM.ARAY[riLDR_R].SetStat(r, 1, csEmpty);}

                      if( IO_GetX(xLDR_RPcbDetectL)) {DM.ARAY[riLDR_R].SetStat(r, 0, csUnkwn);}
                      else                           {DM.ARAY[riLDR_R].SetStat(r, 0, csEmpty);}
                  }

                  Step.iCycle++;
                  return false;

        case  13: Step.iCycle = 0;
                  return true;
    }

}
//-----------------------------------------------------------------------------
bool CLoaderRear::CyclePick(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

//    int r, c;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveMotr(miLDR_ZElevR , piLDR_ZElevRPick);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_ZElevR , piLDR_ZElevRPick)) return false ;
                //  r = DM.ARAY[riLDR_R].FindFrstRow(csUnkwn);
                //
                //  if(OM.DevInfo.iLDRRMgzBayCnt == 1){
                //      if(!IO_GetX(xLDR_RPcbDetectR)){
                //          DM.ARAY[riLDR_R].SetStat(r, 0, csEmpty);
                //      }
                //  }
                //  else if(OM.DevInfo.iLDRRMgzBayCnt == 2){
                //      if(!IO_GetX(xLDR_RPcbDetectR)){
                //          DM.ARAY[riLDR_R].SetStat(r, 1, csEmpty);
                //      }
                //      if(!IO_GetX(xLDR_RPcbDetectL){
                //          DM.ARAY[riLDR_R].SetStat(r, 0, csEmpty);
                //      }
                //  }
                  Step.iCycle=0;
                  return true;
    }

}
//---------------------------------------------------------------------------
bool CLoaderRear::CyclePlce(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

    int r, c ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveMotr(miLDR_ZElevR, piLDR_ZElevRPlce);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLDR_ZElevR, piLDR_ZElevRPlce)) return false ;
                  Step.iCycle=0;
                  return true;
    }

}

//---------------------------------------------------------------------------
bool CLoaderRear::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miLDR_ZElevR){
//        if(IO_GetX(xPRI_Detect1)) {
//            sMsg = m_sPartName + " 레일 진입부 센서 감지 상태로 보트 돌출 확인하세요" ;
//            bRet = false;
//        }
        if(MT_GetCmdPos(miIDX_XIndxR) > PM.GetValue(miIDX_XIndxR , pvIDX_XIndxRGetWait)) {
            sMsg = m_sPartName + "리어 인덱스의 위치가 Get Wait위치 보다 높습니다.";
            bRet = false;
        }
    }
    else if (_iMotr == miLDR_YConvR) {
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iSeq)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLoaderRear::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    //if(_iActr == aiLDR_IndxUpUp){
    //    if(_bFwd == ccFwd) {
    //        //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
    //    }
    //}
    //else {
    //    sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
    //    bRet = false ;
    //}


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

bool CLoaderRear::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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

void CLoaderRear::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoaderRear::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoaderRear::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos( miLDR_ZElevR);
    return ; //이파트는 필요 없다.
}

bool CLoaderRear::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miLDR_ZElevR)) return true  ; //이파트는 필요 없다.
    else                                           return false ;
}

bool CLoaderRear::CheckStop()
{
    if(!MT_GetStop(miLDR_ZElevR)) return false ;
    if(!MT_GetStop(miLDR_YConvR)) return false ;

    return true ;
}

void CLoaderRear::Load(bool _bLoad)
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





