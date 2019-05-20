//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoaderMotor.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "Rail.h"
#include "UserIni.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "UnLoader.h"
#include "Loader.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoaderMotor ULM;

CUnLoaderMotor::CUnLoaderMotor(void)
{
    m_sPartName = "UnUnLoaderMotorH2 " ;
    Reset();
    Load(true);
}

void CUnLoaderMotor::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CUnLoaderMotor::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CUnLoaderMotor::~CUnLoaderMotor (void)
{
    //FormMain으로  Load(false);
}

double CUnLoaderMotor::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    //X , Y Pos Setting.
    double dPos  = 0.0 ;

    if(_iMotr == miULD_ZPpr){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piULD_ZPprWait   : dPos = PM.GetValue (_iMotr , pvULD_ZPprWait    ); break ;
            case piULD_ZPprWorkStt: dPos = PM.GetValue (_iMotr , pvULD_ZPprWorkStt ); break ;
            case piULD_ZPprWorkEnd: dPos = PM.GetValue (_iMotr , pvULD_ZPprWorkEnd ); break ;
        }
    }
    else if(_iMotr == miULD_ZTrI){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piULD_ZTrIWait   : dPos = PM.GetValue (_iMotr , pvULD_ZTrIWait    ); break ;
            case piULD_ZTrIWorkStt: dPos = PM.GetValue (_iMotr , pvULD_ZTrIWorkStt ); break ;
            case piULD_ZTrIWorkEnd: dPos = PM.GetValue (_iMotr , pvULD_ZTrIWorkEnd ); break ;
        }
    }
    else if(_iMotr == miULD_ZTrO){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piULD_ZTrOWait   : dPos = PM.GetValue (_iMotr ,pvULD_ZTrOWait     ); break ;
            case piULD_ZTrOWorkStt: dPos = PM.GetValue (_iMotr ,pvULD_ZTrOWorkStt  ); break ;
            case piULD_ZTrOWorkEnd: dPos = PM.GetValue (_iMotr ,pvULD_ZTrOWorkEnd  ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CUnLoaderMotor::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
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
bool CUnLoaderMotor::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
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

bool CUnLoaderMotor::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CUnLoaderMotor::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CUnLoaderMotor::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CUnLoaderMotor::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiULM_HomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iHome = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_DoHome(miULD_ZPpr);
                 MT_DoHome(miULD_ZTrI);
                 MT_DoHome(miULD_ZTrO);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miULD_ZPpr))return false;
                 if(!MT_GetHomeEnd(miULD_ZTrI))return false;
                 if(!MT_GetHomeEnd(miULD_ZTrO))return false;

                 MT_GoAbsMan(miULD_ZPpr , PM.GetValue(miULD_ZPpr, pvULD_ZPprWait));
                 MT_GoAbsMan(miULD_ZTrI , PM.GetValue(miULD_ZTrI, pvULD_ZTrIWait));
                 MT_GoAbsMan(miULD_ZTrO , PM.GetValue(miULD_ZTrO, pvULD_ZTrOWait));
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miULD_ZPpr , PM.GetValue(miULD_ZPpr, pvULD_ZPprWait))) return false ;
                 if(!MT_GoAbsMan(miULD_ZTrI , PM.GetValue(miULD_ZTrI, pvULD_ZTrIWait))) return false ;
                 if(!MT_GoAbsMan(miULD_ZTrO , PM.GetValue(miULD_ZTrO, pvULD_ZTrOWait))) return false ;

                 Step.iHome = 0;
                 return true;
    }
}

bool CUnLoaderMotor::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        AnsiString sTemp = GetLotNo() ;

        bool isExstStrip   = !DM.ARAY[riLDR].CheckAllStat(csNone) || !DM.ARAY[riPRB].CheckAllStat(csNone) ||
                             !DM.ARAY[riWR1].CheckAllStat(csNone) || !DM.ARAY[riWR2].CheckAllStat(csNone) ||
                             !DM.ARAY[riWR3].CheckAllStat(csNone) || !DM.ARAY[riPSB].CheckAllStat(csNone) ||
                             !DM.ARAY[riULD].CheckAllStat(csNone) ;

        //bool isTrayHold      =  IO_GetX(xULD_TrayPushDetect) && AT_Complete(aiULD_Hold , ccFwd);
        //bool isPprDetect     =  IO_GetX(xULD_PprDetect1);
        //bool isInTrayDetect  =  IO_GetX(xULD_LTrayDetect);
        //bool isOutTrayDetect =  IO_GetX(xULD_RTrayDetect);

        //bool isCyclePpr      = !isPprDetect     && ULD.GetSeqStep() != ULD.scWorkPpr ;
        //bool isCycleInTray   = !isInTrayDetect  && ULD.GetSeqStep() != ULD.scWorkPkg && ULD.GetSeqStep() != ULD.scPlace ;
        //bool isCycleOutTray  = !isOutTrayDetect && ULD.GetSeqStep() != ULD.scWorkPkg && ULD.GetSeqStep() != ULD.scPlace ;


        bool isCycleInTray     =  Stat.bReqInTray  ;
        bool isCyclePaper      =  Stat.bReqPaper   ;
        bool isCycleOutTray    =  Stat.bReqOutTray ;
        //bool isCycleInTrayStt  =  Stat.bReqInTrayStt  ;
        //bool isCycleOutTrayStt =  Stat.bReqOutTrayStt ;
        //bool isCyclePaperStt   =  Stat.bReqPaperStt   ;
        bool isCycleEnd        =  DM.ARAY[riTRY].CheckAllStat(csNone ) && !isExstStrip && LDR.GetStat().bWorkEnd;

        //모르는 스트립에러.
        if(!IO_GetX(xULD_TrayPushDetect) && !AT_Complete(aiULD_Hold , ccFwd)) EM_SetErr(eiULD_HoldingOpen); //
        //카세트 사라짐.

        //카세트 OUT 매거진 풀

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleInTray     ) {Trace(m_sPartName.c_str(),"CycleInTray     Stt"); Step.iSeq = scInTray     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePaper      ) {Trace(m_sPartName.c_str(),"CyclePaper      Stt"); Step.iSeq = scPaper      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOutTray    ) {Trace(m_sPartName.c_str(),"CycleOutTray    Stt"); Step.iSeq = scOutTray    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        //else if (isCycleInTrayStt  ) {Trace(m_sPartName.c_str(),"CycleInTrayStt  Stt"); Step.iSeq = scInTrayStt  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        //else if (isCycleOutTrayStt ) {Trace(m_sPartName.c_str(),"CycleOutTrayStt Stt"); Step.iSeq = scOutTrayStt ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        //else if (isCyclePaperStt   ) {Trace(m_sPartName.c_str(),"CyclePaperStt   Stt"); Step.iSeq = scPaperStt   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default           :                        Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle       :                                                                                            return false ;
        case scInTray     : if(CycleInTray    ()){ Trace(m_sPartName.c_str(),"CycleInTray  End");Step.iSeq = scIdle ;} return false ;
        case scPaper      : if(CyclePaper     ()){ Trace(m_sPartName.c_str(),"CyclePpr     End");Step.iSeq = scIdle ;} return false ;
        case scOutTray    : if(CycleOutTray   ()){ Trace(m_sPartName.c_str(),"CycleOutTray End");Step.iSeq = scIdle ;} return false ;
        case scInTrayStt  : if(CycleInTrayStt ()){ Trace(m_sPartName.c_str(),"CycleInTray  End");Step.iSeq = scIdle ;} return false ;
        case scOutTrayStt : if(CycleOutTrayStt()){ Trace(m_sPartName.c_str(),"CycleOutTray End");Step.iSeq = scIdle ;} return false ;
        case scPaperStt   : if(CyclePaperStt  ()){ Trace(m_sPartName.c_str(),"CyclePpr     End");Step.iSeq = scIdle ;} return false ;
    }

}

bool CUnLoaderMotor::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CUnLoaderMotor::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoaderMotor::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiULD_ToStartTO);

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

        case 10: //MoveMotr(miULD_ZTrI , piULD_ZTrIWait);
                 //MoveMotr(miULD_ZTrO , piULD_ZTrOWait);
                 //MoveMotr(miULD_ZPpr , piULD_ZPprWait);
                 Step.iToStart++ ;
                 return false ;

        case 11: //if(!MoveMotr(miULD_ZTrI , piULD_ZTrIWait)) return false;
                 //if(!MoveMotr(miULD_ZTrO , piULD_ZTrOWait)) return false;
                 //if(!MoveMotr(miULD_ZPpr , piULD_ZPprWait)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoaderMotor::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 8000)) EM_SetErr(eiULD_ToStopTO);

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
                 Step.iToStop ++ ;
                 return false ;

        case 11:
                 Step.iToStop = 0;
                 return true;
    }
}



//One Cycle.
bool CUnLoaderMotor::CyclePaper(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiULD_PprRotator , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiULD_PprRotator , ccFwd)) return false;
                  if(IO_GetX(xULD_PaperDetect)){
                      MoveMotr(miULD_ZPpr , piULD_ZPprWait);
                  }
                  Step.iCycle++;
                  return false;

        case  12: if(!IO_GetX(xULD_PaperDetect)){
                      MT_Stop(miULD_ZPpr);
                  }
                  if(!MT_GetStop(miULD_ZPpr)) return false;
                  MoveMotr(miULD_ZPpr , piULD_ZPprWorkEnd);
                  Step.iCycle++;
                  return false;

        case  13: if(IO_GetX(xULD_PaperDetect)){
                      MT_Stop(miULD_ZPpr);
                  }
                  if(!MT_GetStop(miULD_ZPpr)) return false;
                  if(MT_GetStop(miULD_ZPpr) && !IO_GetX(xULD_PaperDetect)){
                      EM_SetErr(eiULD_PprEmpty);
                      Step.iCycle=0;
                      return true;
                  }
                  Stat.bReqPaper = false ;
                  Step.iCycle = 0;
                  return true;
    }
}

bool CUnLoaderMotor::CycleInTray()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiULD_CycleTO);
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
    //    return true ;
    //}
    TDateTime tTemp ;
    String    sDate ;
//    String    sTemp ;

    String sLotNo ;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_ZTrI , piULD_ZTrIWorkEnd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(IO_GetX(xULD_LTrayDetect)){
                      MT_Stop(miULD_ZTrI);
                  }
                  if(!MT_GetStop(miULD_ZTrI)) return false ;
                  Step.iCycle++;
                  return false;

        case  12: if(IO_GetX(xULD_LTrayDetect)){
                      if(!DM.ARAY[riTRY].GetCntStat(csEmpty) && !DM.ARAY[riTRY].GetCntStat(csWait)){
                          DM.ARAY[riTRY].SetStat(csEmpty);
                      }
                  }
                  else {
                      EM_SetErr(eiULD_TrayInEmpty);
                      MoveMotr(miULD_ZTrI , piULD_ZTrIWait);
                      Step.iCycle++;
                      return false;
                  }
                  Stat.bReqInTray = false ;
                  Step.iCycle = 0;
                  return true;

        case  13: if(!MoveMotr(miULD_ZTrI , piULD_ZTrIWait))return false;
                  Stat.bReqInTray = false ;
                  DM.ARAY[riTRY].SetStat(csNone);
                  Step.iCycle = 0;
                  return true;
    }
}
bool CUnLoaderMotor::CycleOutTray()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    String sLotNo ;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveMotr(miULD_ZTrO , piULD_ZTrOWorkStt);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(IO_GetX(xULD_RTrayDetect)){
                      MT_Stop(miULD_ZTrO);
                  }
                  if(!MT_GetStop(miULD_ZTrO)) return false ;

                  MoveMotr(miULD_ZTrO , piULD_ZTrOWorkEnd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!IO_GetX(xULD_RTrayDetect)){
                      MT_Stop(miULD_ZTrO);
                  }
                  if(!MT_GetStop(miULD_ZTrO)) return false ;
                  if(IO_GetX(xULD_RTrayDetect)){
                      EM_SetErr(eiULD_TrayOutFull);
                  }

                  Stat.bReqOutTray = false ;
                  Step.iCycle = 0;
                  return true;
    }
}

bool CUnLoaderMotor::CyclePaperStt()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiULD_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_ZPpr , piULD_ZPprWorkStt);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_ZPpr , piULD_ZPprWorkStt)) return false;
                  Stat.bReqPaperStt = false ;
                  Step.iCycle = 0;
                  return true;
    }

}

bool CUnLoaderMotor::CycleInTrayStt ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiULD_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_ZTrI , piULD_ZTrIWorkStt);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_ZTrI , piULD_ZTrIWorkStt)) return false;
                  Stat.bReqInTrayStt = false ;
                  Step.iCycle = 0;
                  return true;
    }
}

bool CUnLoaderMotor::CycleOutTrayStt()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    String sLotNo ;
    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_ZTrO , piULD_ZTrOWorkStt);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_ZTrO , piULD_ZTrOWorkStt)) return false;
                  Stat.bReqOutTrayStt = false ;
                  Step.iCycle = 0;
                  return true;
    }

}


void CUnLoaderMotor::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CUnLoaderMotor::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CUnLoaderMotor::CheckStop()
{
    if(!MT_GetStop(miULD_ZTrI)) return false ;
    if(!MT_GetStop(miULD_ZTrO)) return false ;
    if(!MT_GetStop(miULD_ZPpr)) return false ;

    return true ;
}

void CUnLoaderMotor::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;

    if ( IsLoad ) {
    }
    else {
    }
}
//---------------------------------------------------------------------------
