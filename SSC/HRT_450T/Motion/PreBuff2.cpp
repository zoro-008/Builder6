//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreBuff1.h"
#include "PreBuff2.h"
#include "Loader.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPreBuff2 PR2;
//bool bDragWorkEnd ;      //아낭 넌 모닝.....mornig ;;;

CPreBuff2::CPreBuff2(void)
{
    m_sPartName = "PreBuff2 " ;
    Reset();
    Load(true);

    InitCycleName();
}

void CPreBuff2::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPreBuff2::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPreBuff2::~CPreBuff2 (void)
{
}
void CPreBuff2::Close()
{
    Load(false);
}

bool CPreBuff2::FindChip(int &r , int &c )
{
//    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;
}

double CPreBuff2::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos      = 0.0 ;
    int    iNextStep = 1;

    int    iGrCnt ;
    bool   bGrGrip;

    if(OM.DevInfo.iColGrCnt > 0){
        iGrCnt  = OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt ;
        bGrGrip = (DM.ARAY[riPR1].GetStep() + iNextStep) % iGrCnt ;
    }
    else { bGrGrip = false; }

    double dCmpMovePos   = PM.GetValue(miPR2_XCmp , pvPR2_XGrip) -
                          (OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt + OM.DevOptn.dWorkPtchOfs) ;
    double dCmpGrGap = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;



    if(_iMotr == miPR2_XCmp){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                 ); break ;
            case piPR2_XWait    : dPos = PM.GetValue (_iMotr , pvPR2_XWait   ); break ;
            case piPR2_XGrip    : dPos = PM.GetValue (_iMotr , pvPR2_XGrip   ); break ;
            case piPR2_XIdCheck : dPos = PM.GetValue (_iMotr , pvPR2_XIdCheck); break ;
            case piPR2_XPull    : dPos = PM.GetValue (_iMotr , pvPR2_XPull   ); break ;
            case piPR2_XBack    : dPos = PM.GetValue (_iMotr , pvPR2_XBack   ); break ;
            case piPR2_XOut     : dPos = PM.GetValue (_iMotr , pvPR2_XOut    ); break ;
            case piPR2_XWork    : if(bGrGrip) dPos = dCmpMovePos + dCmpGrGap  ;
                                  else        dPos = dCmpMovePos              ; break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff2::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isClampClose = AT_Complete(aiPR2_Cmp   , ccFwd ) ;
    //bool isIdClampDn  = AT_Complete(aiPRB_IdChk , ccBwd ) ;

    if(_iMotr == miPR2_XCmp){
        //if(isIdClampDn){ sMsg = AT_GetName(aiPRB_IdChk)+" is Bwd" ; bRet = false; }
        switch(_iPstnId) {
            default             :                                                                               break ;
            case piPR2_XWait    : if(isClampClose) { sMsg = AT_GetName(aiPR2_Cmp  )+" is Fwd" ; bRet = false ;} break ;
            case piPR2_XGrip    :                                                                               break ;
            case piPR2_XWork    :                                                                               break ;
            case piPR2_XIdCheck :                                                                               break ;
            case piPR2_XPull    :                                                                               break ;
            case piPR2_XBack    :                                                                               break ;
            case piPR2_XOut     :                                                                               break ;
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CPreBuff2::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPR1_XCmp);

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiPR1_Cmp) {
        if( !_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miPR1_XCmp)+" is Moving" ; bRet = false ;}
        }
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CPreBuff2::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreBuff2::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff2::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPR2_HomeTO);
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
                 //Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPR2_Cmp   , ccBwd);
                 //AT_MoveCyl(aiPRB_IdChk , ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPR2_Cmp   , ccBwd)) return false ;
                 //if(!AT_MoveCyl(aiPRB_IdChk , ccFwd)) return false ;
                 MT_DoHome(miPR2_XCmp);
                 MT_SetHomeEnd(miPR2_XCmp , false);
                 Step.iHome++;
                 return false;

        case 12: if(!MT_GetHomeEnd(miPR2_XCmp)) return false;
                 MoveMotr(miPR2_XCmp , piPR2_XGrip);
                 //MT_GoAbsMan(miPR2_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: //if(!MT_GoAbsMan(miPR2_XCmp, m_dLastIndPos)) return false ;
                 if(!MoveMotr(miPR2_XCmp , piPR2_XGrip))return false;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreBuff2::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPR2_Overload)) EM_SetErr(eiPR2_Overload );

    if (!DM.ARAY[riPR2].GetCntExist())tmWait.Clear() ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        //Cycle동작에 관련 되어 있으면 SetSubStep으로 사용.
        //Cycle동작말고 자재와 관련 있으면 SetStep으로 사용함. JS 2013. 9. 11.
        if (Stat.bReqStop && (!IO_GetX(xPR2_Detect) /*|| !IO_GetX(xPRB_2Pkg)*/))return false ;

        bool isPR1OutCycle = PR1.GetCycleStep() == CPreBuff1::scOut;

        //WorkZone의 스텝과 연동 해서 돌아가게 한다.
        bool isCycleIn      =  DM.ARAY[riPR2].CheckAllStat(csUnkwn) && !isPR1OutCycle && DM.ARAY[riPR2].GetSubStep() == 0 ;// OM.CmnOptn.bDryRun) ;

        bool isCycleDrag    =  DM.ARAY[riPR2].GetCntExist (      ) && DM.ARAY[riPR2].GetSubStep() == 1 &&
                               DM.ARAY[riPR2].GetStep() <= OM.DevOptn.iPR2MoveCnt &&
                              (DM.ARAY[riWRK].CheckAllStat(csNone) ||
                              (DM.ARAY[riWRK].GetMaxCol()-DM.ARAY[riWRK].FindLastCol(csUnkwn)-1) > DM.ARAY[riPR2].GetStep());


        bool isCycleOut     =  DM.ARAY[riPR2].GetCntExist () && DM.ARAY[riWRK].CheckAllStat(csNone) &&
                               DM.ARAY[riPR2].GetStep  () >= OM.DevOptn.iPR2MoveCnt &&
                              (DM.ARAY[riPSB].GetCntExist () || tmWait.OnDelay(true ,OM.DevOptn.iWaitDelay * 1000)) ;

        bool isCycleEnd     =  DM.ARAY[riPR2].CheckAllStat(csNone);

        //Unknown Strip
        if (IO_GetX(xPR2_Detect) && !OM.CmnOptn.bDryRun && DM.ARAY[riPR2].CheckAllStat(csNone) && (isCycleIn || isCycleDrag)) EM_SetErr(eiPR2_Unknown) ;

        //Dissapear Strip
        if (!isCycleOut && !OM.CmnOptn.bDryRun) {
            if (!DM.ARAY[riPR2].CheckAllStat(csNone) && !IO_GetX(xPR2_Detect)) EM_SetErr(eiPR2_Dispr)   ;
        }

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt"); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDrag    ) {Trace(m_sPartName.c_str(),"CycleDrag     Stt"); Step.iSeq = scDrag     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt"); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default         :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                           return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scDrag     : if(CycleDrag    ()){ Trace(m_sPartName.c_str(),"CycleDrag     End");Step.iSeq = scIdle ;} return false ;
        case scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CPreBuff2::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (LDR.GetSeqStep() == CLoader::scWork) return false ; //로더 워킹 하면서 푸쉬 하고 있을때 스탑을 되어 스트립 걸려 있는 것 방지.
    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CPreBuff2::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuff2::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiPR2_ToStartTO);

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

        case 10: if(!DM.ARAY[riPR2].CheckAllStat(csNone)){
                     Step.iToStart = 0;
                     return true;
                 }
                 MoveActr(aiPR2_Cmp   , ccBwd);
                 //MoveActr(aiPRB_IdChk , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiPR2_Cmp   , ccBwd)) return false ;
                 //if(!MoveActr(aiPRB_IdChk , ccFwd)) return false ;
                 MoveMotr(miPR2_XCmp , piPR2_XWait);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miPR2_XCmp , piPR2_XWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreBuff2::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErr(eiPR2_ToStopTO);

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

        case 11: if(!MT_GetAlarm   (miPR2_XCmp) &&
                    !MT_GetNLimSnsr(miPR2_XCmp) &&
                    !MT_GetPLimSnsr(miPR2_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPR2_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPR2_XCmp , piPR2_XWait) ;


                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CPreBuff2::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPR2_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPR2_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPR2_Cmp , ccBwd)) return false ;
                  MoveMotr(miPR2_XCmp , piPR2_XWait);
                  Step.iCycle++;
                  return false;

        case  12: //if(/*IO_GetX(xPRB_2Pkg) ||*/ IO_GetX(xPRB_PkgIn2) ) return false; //|| ( !OM.CmnOptn.bDryRun && !IO_GetX(xLD1_PusherBw)) ) return false;
                  if(!MoveMotr(miPR2_XCmp , piPR2_XWait)) return false ;
                  MoveActr(aiPR2_Cmp , ccFwd);
                  //DM.ARAY[riPR2].SetStat(csUnkwn) ;
                  //if(OM.EqpOptn.bExistUnLoader && !DM.ARAY[riULD].GetCntExist()) ULD.Reset();
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPR2_Cmp , ccFwd)) return false;
                  MoveMotr(miPR2_XCmp , piPR2_XIdCheck);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPR2_XCmp , piPR2_XIdCheck)) return false;
                  //if(!OM.CmnOptn.bIgnrIdCehck) MoveActr(aiPRB_IdChk,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: //if(!OM.CmnOptn.bIgnrIdCehck && !MoveActr(aiPRB_IdChk,ccBwd)) return false ;
                  //if(!OM.CmnOptn.bIgnrIdCehck){
                  //    if(!IO_GetX(xPRB_IdCheck)) {
                  //        MoveActr(aiPRB_IdChk,ccFwd);
                  //        EM_SetErr(eiPRB_IdCheck);
                  //        m_bIdCheck = false;
                  //        Step.iCycle = 0;
                  //        return true;
                  //    }
                  //}
                  m_bIdCheck = true;
                  //MoveActr(aiPRB_IdChk , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  16: //if(!MoveActr(aiPRB_IdChk , ccFwd)) return false ;
                  MoveActr(aiPR2_Cmp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPR2_Cmp , ccBwd)) return false ;
                  DM.ARAY[riPR2].SetSubStep(1);
                  DM.ARAY[riPR2].SetStep(0);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff2::CycleDrag()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPR2_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPR2_Cmp   , ccBwd);
                  //MoveActr(aiPRB_IdChk , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPR2_Cmp   , ccBwd)) return false ;
                  //if(!MoveActr(aiPRB_IdChk , ccFwd)) return false ;
                  MoveMotr(miPR2_XCmp , piPR2_XGrip);
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPR2_XCmp , piPR2_XGrip)) return false ;
                  MoveActr(aiPR2_Cmp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPR2_Cmp , ccFwd)) return false ;
                  MoveMotr(miPR2_XCmp , piPR2_XWork) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPR2_XCmp , piPR2_XWork)) return false ;
                  DM.ARAY[riPR2].SetStep(DM.ARAY[riPR2].GetStep() + OM.DevOptn.iContColCnt) ;
                  MoveActr(aiPR2_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPR2_Cmp,ccBwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff2::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPR2_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPR2_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPR2_Cmp , ccFwd)) return false ;
                  MoveMotr(miPR2_XCmp , piPR2_XPull);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPR2_XCmp , piPR2_XPull)) return false ;
                  MoveActr(aiPR2_Cmp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPR2_Cmp , ccBwd)) return false ;
                  MoveMotr(miPR2_XCmp , piPR2_XBack);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPR2_XCmp , piPR2_XBack)) return false ;
                  MoveActr(aiPR2_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPR2_Cmp , ccFwd)) return false ;
                  MT_GoAbsRun(miPR2_XCmp , PM.GetValue(miPR2_XCmp , pvPR2_XOut) + OM.DevOptn.dPR2OutOfs);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MT_GoAbsRun(miPR2_XCmp , PM.GetValue(miPR2_XCmp , pvPR2_XOut) + OM.DevOptn.dPR2OutOfs)) return false ;
                  MT_GoAbs(miPR2_XCmp , PM.GetValue(miPR2_XCmp , pvPR2_XOut) , OM.DevOptn.dPreRailOutVel);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GoAbs(miPR2_XCmp , PM.GetValue(miPR2_XCmp , pvPR2_XOut) , OM.DevOptn.dPreRailOutVel)) return false ;
                  m_sWorkedLotNo = DM.ARAY[riPR2].GetLotNo() ;
                  DM.ShiftArrayData(riPR2,riWRK);
                  //MT_GoAbsRun(miPR2_XCmp,piPR2_XBack);
                  Step.iCycle++;
                  return false ;

        case  18: //if(!MoveMotr(miPR2_XCmp,piPR2_XBack))return false ;
                  MoveActr(aiPR2_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPR2_Cmp,ccBwd)) return false ;
                  MoveMotr(miPR2_XCmp , piPR2_XGrip) ;
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miPR2_XCmp , piPR2_XGrip)) return false ;
                  DM.ARAY[riPR2].SetStat(csNone);
                  DM.ARAY[riPR2].SetSubStep(0);
                  Step.iCycle = 0 ;
                  return true ;

    }
}

void CPreBuff2::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPreBuff2::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreBuff2::CheckStop()
{
    if(!MT_GetStop(miPR2_XCmp)) return false ;

    if(!AT_Done(aiPR2_Cmp    )) return false ;
    //if(!AT_Done(aiPRB_IdChk  )) return false ;

    return true ;
}

void CPreBuff2::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuff2.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        //UserINI.Load(sPath, "Member" , "m_bIdCheck"    , m_bIdCheck    );

    }

    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        //UserINI.Save(sPath, "Member" , "m_bIdCheck"    , m_bIdCheck    );
    }

}
//---------------------------------------------------------------------------
