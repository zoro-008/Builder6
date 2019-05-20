
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Transfer.h"
#include "FrontWork.h"
#include "BackWork.h"
#include "Feed.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CTransfer TRS;

CTransfer::CTransfer(void)
{

}

CTransfer::~CTransfer (void)
{

}

void CTransfer::Init()
{
    m_sPartName = "Transfer " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CTransfer::Close()
{
    Load(false);
}

void CTransfer::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CTransfer::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}



bool CTransfer::FindChip(int &r , int &c, EN_ARAY_ID riTRS)
{
    c = DM.ARAY[riTRS].FindLastCol(csNone) ;

    return (c > -1 && c > -1);
}

double CTransfer::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;
   /*
    int r,c ;

    int iMaxCol = DM.ARAY[riPSB].GetMaxCol();
    FindChip(r , c);

    //횡으로 플립된값.
    int fc = (iMaxCol - c) -1 ;

    //1그룹별 칩의 갯수.
    int iGroupChipCnt = iMaxCol / OM.DevInfo.iColGrCnt ;


    //그릅단위의 포지션.
    double dGroupStartPos = fc / iGroupChipCnt * OM.DevInfo.dColGrPitch ;

    //그룹을 뺀 나머지 칩단위 포지션.
    double dChipPos       = (fc % iGroupChipCnt * OM.DevInfo.dColGrPitch) * OM.DevInfo.dColPitch;
    double dWorkPos       = PM.GetValue (_iMotr , pvPSB_XCmpWorkStt ) + dGroupStartPos + dChipPos;
    */
    if(_iMotr == miTRS_YStc){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piTRS_YStcWait    : dPos = PM.GetValue (_iMotr , pvTRS_YStcWait ); break ;
            case piTRS_YStcBwd     : dPos = PM.GetValue (_iMotr , pvTRS_YStcBwd  ); break ;
            case piTRS_YStcFwd     : dPos = PM.GetValue (_iMotr , pvTRS_YStcFwd  ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CTransfer::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;


    //워크 모턱가 충돌 포지션이 아닌지..
    bool bFrntPushFwd  = !AT_Complete(aiFWK_Pshr_DnUp, ccBwd) ;
    bool bBackPushFwd  = !AT_Complete(aiBWK_Pshr_DnUp, ccBwd) ;

    bool bConFrntCrashPos = OM.MstOptn.dTrsCrashMinPos <= MT_GetCmdPos(miFWK_XIdx) &&
                            OM.MstOptn.dTrsCrashMaxPos >= MT_GetCmdPos(miFWK_XIdx) ;
    bool bConBackCrashPos = OM.MstOptn.dTrsCrashMinPos <= MT_GetCmdPos(miBWK_XIdx) &&
                            OM.MstOptn.dTrsCrashMaxPos >= MT_GetCmdPos(miBWK_XIdx) ;

    bool bFrntCrashPos = bFrntPushFwd && bConFrntCrashPos ;
    bool bBackCrashPos = bBackPushFwd && bConBackCrashPos ;

    if(_iMotr == miTRS_YStc){
        if(bFrntCrashPos || bBackCrashPos) {sMsg = "Pusher Cylinder is not Bwd"; bRet = false ;}
        switch(_iPstnId) {
            default                :  break ;
            case piTRS_YStcWait    :  break ;
            case piTRS_YStcBwd     :  break ;
            case piTRS_YStcFwd     :  break ;
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
bool  CTransfer::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;
                      /*
    bool isXMoving = !MT_GetStopInpos(miPSB_XCmp);


    AnsiString sMsg ;

    if(_iActr == aiPSB_CmpDnUp) {
        if(_bFwd) {
            if(isXMoving) { sMsg = AnsiString(MT_GetName(miPSB_XCmp))+" is Moving" ; bRet = false ;}
        }
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }
       */
    return bRet ;
}

bool CTransfer::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CTransfer::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CTransfer::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiPSB_HomeTo);
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

        case 10: //if(IO_GetX(xFED_PkgCntSnsr)) return false;  //이건 뭐지....요것도 다시 해야 함ㅜㅜ..JH
                 AT_MoveCyl(aiBWK_Pshr_DnUp  ,ccBwd);
                 AT_MoveCyl(aiFWK_Pshr_DnUp  ,ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiBWK_Pshr_DnUp  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiFWK_Pshr_DnUp  ,ccBwd)) return false ;
                 MT_SetHomeEnd(miTRS_YStc , false);
                 MT_DoHome(miTRS_YStc);
                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miTRS_YStc)) return false;
                 if(!DM.ARAY[riTRS_B].CheckAllStat(csNone)){
                     MT_GoAbsRun(miTRS_YStc, PM.GetValue(miTRS_YStc , pvTRS_YStcFwd));
                 }
                 else{
                     MT_GoAbsRun(miTRS_YStc, PM.GetValue(miTRS_YStc , pvTRS_YStcBwd));
                 }
                 Step.iHome++ ;
                 return false ;

        case 13: if(!DM.ARAY[riTRS_B].CheckAllStat(csNone)){
                     if(!MT_GoAbsRun(miTRS_YStc, PM.GetValue(miTRS_YStc , pvTRS_YStcFwd)))return false;
                 }
                 else {
                     if(MT_GoAbsRun(miTRS_YStc, PM.GetValue(miTRS_YStc , pvTRS_YStcBwd)))return false;
                 }
                 //if(!MT_GoAbsMan(miTRS_YStc, PM.GetValue(miTRS_YStc , pvTRS_YStcWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CTransfer::Autorun(void) //오토런닝시에 계속 타는 함수.
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //if (IO_GetX(xPSB_CmpOvld ) ) EM_SetErr(eiPSB_CmpOvld    );

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        bool isCycleGoBwd;
        bool isCycleGoFwd;

        bool bFEDWorkEnd  = FED.GetStat().bWorkEnd ;
        bool bBwdPos      = MT_GetCmdPos(miTRS_YStc) == PM.GetValue(miTRS_YStc,pvTRS_YStcBwd);
        bool bFwdPos      = MT_GetCmdPos(miTRS_YStc) == PM.GetValue(miTRS_YStc,pvTRS_YStcFwd);
        bool bTRS_FFull   = DM.ARAY[riTRS_F].CheckAllStat(csUnkwn);
        bool bTRS_BFull   = DM.ARAY[riTRS_B].CheckAllStat(csUnkwn);
        bool bTRSEmptyAll = DM.ARAY[riTRS_B].CheckAllStat(csNone ) && DM.ARAY[riTRS_F].CheckAllStat(csNone );
        bool bFEDIdle     = FED.GetSeqStep() == CFeed::scIdle ;
        bool bFWKInCycle  = FWK.GetSeqStep() == CFrontWork::scIn ;
        bool bBWKInCycle  = BWK.GetSeqStep() == CBackWork::scIn  ;
        bool bMovingOk    = bFEDIdle && !bFWKInCycle && !bBWKInCycle ;

        //처음 시작 할때는 Bwd포지션으로 가서 TRS_F부터 채우기 시작.
        //마지막 작업시에는 들 찾어두 배출이 되어야 한다.
        //riTRS_F 앞쪽 트렌스퍼 레일   pvTRS_YStcBwd 트렌스퍼 뒤로 갔을때
        //riTRS_B 뒷쪽 트렌스퍼 레일.  pvTRS_YStcFwd 트렌스퍼 앞으로 갔을때.

        bool bTRS_BFinish = bTRS_BFull || bFEDWorkEnd ;
        bool bTRS_FFinish = bTRS_FFull || bFEDWorkEnd ;

        bool bUseOnlyFrnt = OM.CmnOptn.iUseLsrPos == 1 && FWK.m_bFnshInCyc;
        bool bUseOnlyBack = OM.CmnOptn.iUseLsrPos == 2 && BWK.m_bFnshInCyc;

        if(OM.CmnOptn.iUseLsrPos == 0){
            isCycleGoBwd = (bMovingOk && DM.ARAY[riTRS_F].GetCntStat(csNone ) && DM.ARAY[riFWK].CheckAllStat(csNone ) &&!bBwdPos && (bTRS_BFinish || bTRSEmptyAll)) ;
            isCycleGoFwd = (bMovingOk && DM.ARAY[riTRS_B].GetCntStat(csNone ) && DM.ARAY[riBWK].CheckAllStat(csNone ) &&!bFwdPos && (bTRS_FFinish &&!bTRSEmptyAll)) ;
        }
        else if(bUseOnlyFrnt){
            isCycleGoBwd = (bMovingOk && DM.ARAY[riTRS_F].GetCntStat(csNone ) && DM.ARAY[riFWK].CheckAllStat(csNone ) && !bBwdPos && (bTRS_BFinish || bTRSEmptyAll)) ;
            isCycleGoFwd = (bMovingOk && DM.ARAY[riTRS_F].CheckAllStat(csUnkwn ) && !bFwdPos)  ;
            //isCycleGoFwd = (bMovingOk && DM.ARAY[riTRS_B].GetCntStat(csNone ) && DM.ARAY[riBWK].CheckAllStat(csNone ) && !bFwdPos && (bTRS_FFinish &&!bTRSEmptyAll)) ;
            //isCycleGoBwd = bMovingOk && DM.ARAY[riTRS_F].CheckAllStat(csUnkwn) && !bBwdPos ;
            //isCycleGoFwd = bMovingOk && DM.ARAY[riTRS_F].GetCntStat  (csNone ) && !bFwdPos ;
        }
        else {
            isCycleGoBwd = bMovingOk && DM.ARAY[riTRS_B].CheckAllStat(csUnkwn) && !bBwdPos ;
            isCycleGoFwd = bMovingOk && DM.ARAY[riTRS_B].GetCntStat  (csNone ) && !bFwdPos ;
        }
        //KJH
        //bool isCycleFwd = DM.ARAY[riTRS_F].CheckAllStat(csUnkwn) || !bLotOpened ;

        bool isCycleEnd = bFEDWorkEnd && !isCycleGoBwd && !isCycleGoFwd && DM.ARAY[riTRS_F].CheckAllStat(csNone) && DM.ARAY[riTRS_B].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleGoBwd ) {Trace(m_sPartName.c_str(),"CycleGoBwd Stt"); Step.iSeq = scBwd ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleGoFwd ) {Trace(m_sPartName.c_str(),"CycleGoFwd Stt"); Step.iSeq = scFwd ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default     :                   Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle :                                                                                     return false ;
        case scBwd  : if(CycleGoBwd()){ Trace(m_sPartName.c_str(),"CycleGoBwd End");Step.iSeq = scIdle ;} return false ;
        case scFwd  : if(CycleGoFwd()){ Trace(m_sPartName.c_str(),"CycleGoFwd End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

//     2010 여기까지....end

bool CTransfer::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    //if (Step.iSeq) return false;   //여기에 추가 해야 할 수도 자제가 중간에 끼면 안되니까 센서로 검지

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CTransfer::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CTransfer::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str());

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    bool bExstUnKnown = false ;

    double dWrkCrntPosF = MT_GetCmdPos(miFWK_XIdx) ;
    double dWrkCrntPosB = MT_GetCmdPos(miBWK_XIdx) ;
    double dCrashPosMin = OM.MstOptn.dTrsCrashMinPos;
    double dCrashPosMax = OM.MstOptn.dTrsCrashMaxPos;

    bool bCrashFrnt = dCrashPosMin <= dWrkCrntPosF && dWrkCrntPosF <= dCrashPosMax ;
    bool bCrashBack = dCrashPosMin <= dWrkCrntPosB && dWrkCrntPosB <= dCrashPosMax ;
    bool bCrashPos  = bCrashFrnt || bCrashBack ;

    bool bSldDn = AT_GetCmd(aiBWK_Pshr_DnUp)== ccFwd || AT_GetCmd(aiFWK_Pshr_DnUp)== ccFwd ; //이거 맞으려나...JH

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(!bCrashPos && !bSldDn){
                 Step.iToStart = 0 ;
                 return true ;
                 }
                 MoveActr(aiFWK_Pshr_DnUp , ccBwd);
                 MoveActr(aiBWK_Pshr_DnUp , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiFWK_Pshr_DnUp , ccBwd)) return false ;
                 if(!MoveActr(aiBWK_Pshr_DnUp , ccBwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CTransfer::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiPSB_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    double dWrkCrntPosF = MT_GetCmdPos(miFWK_XIdx) ;
    double dWrkCrntPosB = MT_GetCmdPos(miBWK_XIdx) ;
    double dCrashPosMin = OM.MstOptn.dTrsCrashMinPos;
    double dCrashPosMax = OM.MstOptn.dTrsCrashMaxPos;

    bool bCrashFrnt = dCrashPosMin <= dWrkCrntPosF && dWrkCrntPosF <= dCrashPosMax ;
    bool bCrashBack = dCrashPosMin <= dWrkCrntPosB && dWrkCrntPosB <= dCrashPosMax ;
    bool bCrashPos  = bCrashFrnt || bCrashBack ;

    bool bSldDn = AT_GetCmd(aiBWK_Pshr_DnUp)== ccFwd || AT_GetCmd(aiFWK_Pshr_DnUp)== ccFwd ;

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: if(!bCrashPos && !bSldDn){
                 Step.iToStop = 0 ;
                 return true ;
                 }
                 MoveActr(aiFWK_Pshr_DnUp , ccBwd);
                 MoveActr(aiBWK_Pshr_DnUp , ccBwd);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveActr(aiFWK_Pshr_DnUp , ccBwd)) return false ;
                 if(!MoveActr(aiBWK_Pshr_DnUp , ccBwd)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

//리워크 존까지 가져가는 것.
bool CTransfer::CycleGoBwd(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

        case  10: MoveMotr(miTRS_YStc, piTRS_YStcBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miTRS_YStc, piTRS_YStcBwd)) return false;
                  Step.iCycle=0;
                  return true;

    }
}

//리워크 존까지 가져가는 것.
bool CTransfer::CycleGoFwd(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

        case  10: MoveMotr(miTRS_YStc, piTRS_YStcFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miTRS_YStc, piTRS_YStcFwd)) return false;
                  Step.iCycle=0;
                  return true;



    }
}


void CTransfer::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CTransfer::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CTransfer::CheckStop()
{
    //if(!MT_GetStop(miPSB_XCmp)) return false ;

    //if(!AT_Done(aiPSB_CmpDnUp)) return false ;
    //if(!AT_Done(aiPSB_PshFwBw)) return false ;
    //if(!AT_Done(aiPSB_RlrDnUp)) return false ;
     // 여긴 나중에 다시 JH

    return true ;
}

void CTransfer::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName + ".INI";

}
//---------------------------------------------------------------------------
