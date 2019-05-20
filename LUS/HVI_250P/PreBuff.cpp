//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PreBuff.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"
//#include "Loader.h"

#include "EqpComUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
//#include "PreBuff.h"
#include "Work.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//dd  Init 만들기 . 월요일날...!!!
//---------------------------------------------------------------------------
CPreBuff  PRB;

CPreBuff::CPreBuff(void)
{
    Init();
}
void CPreBuff::Init()
{
    m_sPartName = "PreBuff " ;
    m_sPreLotNo = "" ;

    Reset();
    Load(true);
}

void CPreBuff::Close()
{
    Load(false);
}

void CPreBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPreBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPreBuff::~CPreBuff (void)
{
//    Close();
    //FormMain으로  Load(false);
}

bool CPreBuff::FindChip(int &r , int &c )
{
    return false;
}

double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miPRB_XIdx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piPRB_XIdxWait    : dPos = PM.GetValue (_iMotr , pvPRB_XIdxWait    ); break ;
            case piPRB_XIdxTurn    : dPos = PM.GetValue (_iMotr , pvPRB_XIdxTurn    ); break ;
            case piPRB_XIdxOut     : dPos = PM.GetValue (_iMotr , pvPRB_XIdxOut     ); break ;
        }
    }
    else if(_iMotr == miPRB_TFlp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piPRB_TFlpWait    : dPos = PM.GetValue (_iMotr , pvPRB_TFlpWait    ); break ;
            case piPRB_TFlpNormal  : dPos = PM.GetValue (_iMotr , pvPRB_TFlpNormal  ); break ;
            case piPRB_TFlpInverse : dPos = PM.GetValue (_iMotr , pvPRB_TFlpInverse ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isIdxWaitPs  = MT_CmprPos (miPRB_XIdx , PM.GetValue(miPRB_XIdx,pvPRB_XIdxWait    )) ; //
    bool isFlpNormal  = MT_CmprPos (miPRB_TFlp , PM.GetValue(miPRB_TFlp,pvPRB_TFlpNormal  )) ; //
    bool isFlpInverse = MT_CmprPos (miPRB_TFlp , PM.GetValue(miPRB_TFlp,pvPRB_TFlpInverse )) ; //

    bool Pkg1Ssr      = IO_GetX(xPRB_Strip2);
    bool Pkg2Ssr      = IO_GetX(xPRI_Strip3);

//    bool  !IO_GetX(xPRB_Strip2   ) || !IO_GetX(xPRI_Strip3      )

    if(_iMotr == miPRB_XIdx){
        switch(_iPstnId) {
            default              : break ;
            case piPRB_XIdxWait  : if(!isFlpNormal && !isFlpInverse) { sMsg = "Flipper Not Ready" ; bRet = false ; } break ;
            case piPRB_XIdxTurn  : if(!isFlpNormal && !isFlpInverse) { sMsg = "Flipper Not Ready" ; bRet = false ; } break ;
            case piPRB_XIdxOut   : if(!isFlpInverse) { sMsg = "Flipper Not Ready" ; bRet = false ; } break ;
        }
    }
    else if(_iMotr == miPRB_TFlp){
        switch(_iPstnId) {
            default                 : break ;
            case piPRB_TFlpWait     : if(!isIdxWaitPs || ( Pkg1Ssr || Pkg2Ssr)) { sMsg = "Flipper Zone Check" ; bRet = false ; } break ;
            case piPRB_TFlpNormal   : if(!isIdxWaitPs || ( Pkg1Ssr || Pkg2Ssr)) { sMsg = "Flipper Zone Check" ; bRet = false ; } break ;
            case piPRB_TFlpInverse  : if(!isIdxWaitPs || ( Pkg1Ssr || Pkg2Ssr)) { sMsg = "Flipper Zone Check" ; bRet = false ; } break ;
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
bool  CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiPRB_Idx) {
        bRet = true  ;
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_HomeTO);
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

        case 10: MT_Reset(miPRB_TFlp);
                 MT_Reset(miPRB_XIdx);
                 MT_SetServo(miPRB_TFlp,true);
                 MT_SetServo(miPRB_XIdx,true);

                 AT_MoveCyl(aiPRB_Idx , ccBwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Idx , ccBwd)) return false ;
                 MT_DoHome(miPRB_XIdx) ;
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPRB_XIdx))return false ;
                 MT_GoAbsMan(miPRB_XIdx , PM.GetValue(miPRB_XIdx , pvPRB_XIdxWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPRB_XIdx , PM.GetValue(miPRB_XIdx , pvPRB_XIdxWait))) return false ;
                 MT_DoHome(miPRB_TFlp) ;
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miPRB_TFlp))return false ;
                 MT_GoAbsMan(miPRB_TFlp , PM.GetValue(miPRB_TFlp , pvPRB_TFlpNormal));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miPRB_TFlp , PM.GetValue(miPRB_TFlp , pvPRB_TFlpNormal))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPreBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    EC.SetPreEqpReady(false);
    if (EC.GetPreEqpSending()) return false ;
    if (Step.iSeq ) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    if(DM.ARAY[riPRB].GetCntExist() && IO_GetX(xPRB_Strip2)) {
        EM_SetErr(eiPRB_FlpSsr1Detected);
        return false ;
    }
    if(DM.ARAY[riPRB].GetCntExist() && IO_GetX(xPRI_Strip3)) {
        EM_SetErr(eiPRB_FlpSsr2Detected);
        return false ;
    }

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPRB_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: MoveActr(aiPRB_Idx , ccBwd);

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPRB_Idx , ccBwd)) return false ;
                 MoveMotr(miPRB_XIdx,piPRB_XIdxWait);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miPRB_XIdx,piPRB_XIdxWait)) return false ;
                 MoveMotr(miPRB_TFlp,piPRB_TFlpNormal);

                 if(IO_GetX(xPRB_Strip) && DM.ARAY[riPRB].CheckAllStat(csNone)){
                     DM.ARAY[riPRB].SetStat(csWork);

                     DM.ARAY[riPRB].SetLotNo(m_sPreLotNo) ;
                     DM.ARAY[riPRB].SetID   (AnsiString(m_sPreId.ToIntDef(0)/100 * 100 +99))  ;

                     sTemp = DM.ARAY[riPRB].GetLotNo();
                     Trace("PRB Strip Add Lot" , sTemp.c_str());
                     sTemp = DM.ARAY[riPRB].GetID()   ;
                     Trace("PRB Strip Add ID" , sTemp.c_str());
                 }

                 Step.iToStart++;
                 return false ;

        case 13: if(!MoveMotr(miPRB_TFlp,piPRB_TFlpNormal)) return false ;

                 Step.iToStart = 0 ;
                 return true ;

    }
}

bool CPreBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPRB_ToStopTO);

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
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPreBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.

    PreStep.iSeq = Step.iSeq ;

    //
    //
    //

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && !EC.GetPreEqpSending())return false ;

        bool bFlpNor   = MT_CmprPos(miPRB_TFlp,PM.GetValue(miPRB_TFlp,pvPRB_TFlpNormal )) ;
        bool bFlpInv   = MT_CmprPos(miPRB_TFlp,PM.GetValue(miPRB_TFlp,pvPRB_TFlpInverse)) ;
        bool bPrbNone  = DM.ARAY[riPRB].CheckAllStat(csNone );
        bool bPriNone  = DM.ARAY[riPRI].CheckAllStat(csNone );
        bool bPsiNone  = DM.ARAY[riPSI].CheckAllStat(csNone );
        bool bPsiEmpty = DM.ARAY[riPSI].CheckAllStat(csEmpty);
        bool bPrbStrip = IO_GetX(xPRB_Strip ) ; //플리퍼 자제감지.

        bool b1 = DM.ARAY[riINB].CheckAllStat(csNone) ;
        bool b2 = IO_GetX(xPRB_Strip1) ;
        bool b3 = EC.GetPreEqpSending() ;
//        bool isCycleIn    = DM.ARAY[riINB].CheckAllStat(csNone) && (IO_GetX(xPRB_Strip1) || EC.GetPreEqpSending()); ;
        bool isCycleIn    = DM.ARAY[riINB].CheckAllStat(csNone) && (IO_GetX(xPRB_Strip1) ); ;

        bool isCycleToFlp =!DM.ARAY[riINB].CheckAllStat(csNone) && DM.ARAY[riPRB].CheckAllStat(csNone);  //xPRB_Strip2 && !DM.ARAY[riPRB].GetCntExist();
        bool isCycleTurn  = bPrbStrip && !bPrbNone && bFlpNor  ;
        bool isCycleOut   =!bPrbNone  && bFlpInv   && bPriNone && (bPsiNone ||bPsiEmpty) ;

        bool isConEnd     = DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riINB].CheckAllStat(csNone) && EC.GetPreEqpLotEnd();

        //UnKnown
        if(DM.ARAY[riINB].CheckAllStat(csNone) ){
            if( IO_GetX(xPRB_Strip2)) EM_SetErr(eiINB_PkgUnknown ) ;
        }
        if( bPrbNone && bFlpNor){
            if( IO_GetX(xPRB_Strip)) EM_SetErr(eiPRB_PkgUnknown ) ;
        }

        //Disappear
        if(isCycleToFlp ){
            if(!IO_GetX(xPRB_Strip2)) EM_SetErr(eiINB_PkgDispr ) ;
        }
        if(!bPrbNone ){
            if(!IO_GetX(xPRB_Strip)) EM_SetErr(eiPRB_PkgDispr ) ;
        }

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn     ) {Trace(m_sPartName.c_str(),"CycleIn    Stt"); Step.iSeq = scIn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleToFlp  ) {Trace(m_sPartName.c_str(),"CycleToFlp Stt"); Step.iSeq = scToFlp; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleTurn   ) {Trace(m_sPartName.c_str(),"CycleTurn  Stt"); Step.iSeq = scTurn ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut    ) {Trace(m_sPartName.c_str(),"CycleOut   Stt"); Step.iSeq = scOut  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default       :                 /*Trace(m_sPartName.c_str(),"default    End");*/Step.iSeq = scIdle ;  return false ;
        case  scIn    : if(CycleIn   ()){ Trace(m_sPartName.c_str(),"CycleIn    End");  Step.iSeq = scIdle ;} return false ;
        case  scToFlp : if(CycleToFlp()){ Trace(m_sPartName.c_str(),"CycleToFlp End");  Step.iSeq = scIdle ;} return false ;
        case  scTurn  : if(CycleTurn ()){ Trace(m_sPartName.c_str(),"CycleTurn  End");  Step.iSeq = scIdle ;} return false ;
        case  scOut   : if(CycleOut  ()){ Trace(m_sPartName.c_str(),"CycleOut   End");  Step.iSeq = scIdle ;} return false ;
    }
}

bool CPreBuff::CycleIn(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : Step.iCycle++;
                  return false ;

        case 11 : //if(EC.GetPreEqpSending()) if(!IO_GetX(xPRB_Strip1)) return false ;
                  Step.iCycle++;
                  return false ;

        case 12 : if( IO_GetX(xPRB_Strip1))return false ;
//                  if(!IO_GetX(xPRB_Strip2))return false ;
                  if(!OM.MstOptn.bDebugMode) {
                      Trace(m_sPartName.c_str(),"EC.ReadArayData");
                      if(!EC.ReadArayData(&DM.ARAY[riINB]) ) {
                          EM_SetErr(eiETC_PreEqpRead) ;
                          Step.iCycle = 0;
                          return true ;
                      }
                  }
                  else {
                      DM.ARAY[riINB].SetStat(csWork);
                  }


                  DM.ARAY[riINB].SetData(DM.ARAY[riINB].GetCntStat(csWork));
                  DM.ARAY[riINB].SetStat(csWork);

                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPreBuff::CycleToFlp(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPRB_Idx ,ccBwd           );
                  MoveMotr(miPRB_XIdx,piPRB_XIdxWait  );
                  MoveMotr(miPRB_TFlp,piPRB_TFlpNormal);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_Idx ,ccBwd           )) return false ;
                  if(!MoveMotr(miPRB_XIdx,piPRB_XIdxWait  )) return false ;
                  if(!MoveMotr(miPRB_TFlp,piPRB_TFlpNormal)) return false ;
                  //if(IO_GetX(xPRB_Strip2)) return false ; // 팅겨 팅겨...dd
                  MoveActr(aiPRB_Idx ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveActr(aiPRB_Idx ,ccFwd)) return false ;
                  MoveMotr(miPRB_XIdx,piPRB_XIdxTurn);
                  Step.iCycle++;
                  return false ;

        case 13 : if(IO_GetX(xPRB_IdxOverLoad)) {
                      EM_SetErr(eiPRB_IdxOverLoad);
                      MT_Stop(miPRB_XIdx);

                      Step.iCycle=0;
                      return true ;
                  }

                  if(!MoveMotr(miPRB_XIdx,piPRB_XIdxTurn)) return false ;

                  DM.ShiftArrayData(riINB , riPRB) ;

                  Step.iCycle++;
                  return false ;

        case 14 : MoveMotr(miPRB_XIdx,piPRB_XIdxWait);
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPreBuff::CycleTurn(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveMotr(miPRB_XIdx,piPRB_XIdxWait  );
                  MoveMotr(miPRB_TFlp,piPRB_TFlpNormal);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveMotr(miPRB_XIdx,piPRB_XIdxWait  )) return false ;
                  if(!MoveMotr(miPRB_TFlp,piPRB_TFlpNormal)) return false ;
                  MoveMotr(miPRB_TFlp,piPRB_TFlpInverse);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPRB_TFlp,piPRB_TFlpInverse)) return false ;
                  MoveActr(aiPRB_Idx,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPRB_Idx,ccFwd)) return false ;
                  MoveMotr(miPRB_XIdx,piPRB_XIdxTurn);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveMotr(miPRB_XIdx,piPRB_XIdxTurn)) return false ;

                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPreBuff::CycleOut(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPRB_Idx,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPRB_Idx,ccFwd)) return false ;
                  MoveMotr(miPRB_XIdx,piPRB_XIdxOut);
                  Step.iCycle++ ;
                  return false ;

        case  12: if(IO_GetX(xPRB_IdxOverLoad)) {
                      EM_SetErr(eiPRB_IdxOverLoad);
                      MT_Stop(miPRB_XIdx);

                      Step.iCycle=0;
                      return true ;
                  }

                  if(!MoveMotr(miPRB_XIdx,piPRB_XIdxOut))return false ;

                  m_sPreLotNo = DM.ARAY[riPRB].GetLotNo() ;
                  m_sPreId    = DM.ARAY[riPRB].GetID   () ;

                  DM.ShiftArrayData(riPRB,riPRI);
                  MoveMotr(miPRB_XIdx,piPRB_XIdxWait);
                  MoveActr(aiPRB_Idx,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveMotr(miPRB_XIdx,piPRB_XIdxWait))return false ;
                  if(!MoveActr(aiPRB_Idx,ccBwd          ))return false ;
                  MoveMotr(miPRB_TFlp,piPRB_TFlpNormal);


                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miPRB_TFlp,piPRB_TFlpNormal))return false ;



                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPreBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.
}

bool CPreBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreBuff::CheckStop()
{
    if(!AT_Done(aiPRB_Idx    )) return false ;

    if(!MT_GetStop(miPRB_TFlp)) return false ;
    if(!MT_GetStop(miPRB_XIdx)) return false ;

    return true ;
}

void CPreBuff::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo " , m_sPreLotNo );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreId    " , m_sPreId    );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo " , m_sPreLotNo );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreId    " , m_sPreId    );
    }
}

