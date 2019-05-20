//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Feed.h"
#include "Transfer.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "LotUnit.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CFeed FED;

double CFeed::Get100000From360(double _dAbsAngle) //0.0기준으로 Abs포지션을 입력 해야함.
{
    double dRet = 100000 *_dAbsAngle / 360.0 ;
    return dRet ;
}

double CFeed::GetTCmdPos()
{
    return (MT_GetCmdPos(miFED_TIdx) * 360) /100000.0 ;
}

CFeed::CFeed(void)
{

}

void CFeed::Init()
{
    m_sPartName = "Feed " ;
    Reset();
    Load(true);

    InitCycleName();

    m_bOutSnsrDetect = false;
    m_bNeedFeedHome  = false;
    m_bOutFlick      = false;
     
    m_iSolFlickCnt = 0;

    m_iPitchCnt = 0;

    m_iVibrationCnt = 0;
    m_dTempTm = 0.0 ;
}

void CFeed::Close()
{
    Load(false);
}

void CFeed::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    //m_iPitchCnt = 0;
    //MT_SetPos(miFED_TIdx, 0);
}

void CFeed::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CFeed::~CFeed (void)
{

}

bool CFeed::FindChip(int &r , int &c ,EN_ARAY_ID riTRS)
{
    c = DM.ARAY[riTRS].FindLastCol(csNone) ;

    //return c ;
    return (c > -1 && c > -1);

    //return false;
}

double CFeed::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    if(_iMotr == miFED_TIdx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                               ); break ;
            case piFED_TIdxWait    : dPos = PM.GetValue (_iMotr , pvFED_TIdxWait              ); break ;
            case piFED_TIdxWork    : dPos = Get100000From360(PM.GetValue (_iMotr , poFED_TIdxPitch ) * m_iPitchCnt + PM.GetValue (_iMotr , pvFED_TIdxWait )); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CFeed::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //bool isClampClose = AT_Complete(aiPRB_CmpDnUp  , ccFwd) ;

    bool bMtTrsStop = MT_GetStop(miTRS_YStc);

    if(_iMotr == miFED_TIdx){
        if(!bMtTrsStop) { sMsg = "Transfer Motor is Moving."   ; bRet = false ;}
        switch(_iPstnId) {
            default             : break ;
            case piFED_TIdxWait : break ;
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
bool  CFeed::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;

    //bool isXMoving = !MT_GetStopInpos(miPRB_XCmp);

    //AnsiString sMsg ;

    //if(_iActr == aiPRB_CmpDnUp) {
    //    if(!_bFwd) {
    //        if(isXMoving) { sMsg = AnsiString(MT_GetName(miPRB_XCmp))+" is Moving" ; bRet = false ;}
    //    }
    //}

    //if(!bRet){
    //    Trace(AT_GetName(_iActr), sMsg.c_str());
    //    if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    //}

    return bRet ;
}

bool CFeed::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);

}

bool CFeed::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CFeed::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 20000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiPRB_HomeTo);
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

        case 10: IO_SetY(yFED_InSolLR  ,false) ;
                 IO_SetY(yFED_OutSolLR ,false);
                 Step.iHome++;
                 return false ;

        case 11: //if(IO_GetX(xFED_OutSnsr)){
                 //    EM_SetErr(eiFED_HomeToOutSnsrDetect);
                 //    Step.iHome=0;
                 //    return true;
                 //}
                 MT_SetHomeEnd(miFED_TIdx , false);
                 MT_DoHome(miFED_TIdx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miFED_TIdx)) return false;
                 MT_GoAbsMan(miFED_TIdx, PM.GetValue(miFED_TIdx , pvFED_TIdxWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miFED_TIdx, PM.GetValue(miFED_TIdx , pvFED_TIdxWait))) return false ;
                 m_iPitchCnt=0;
                 m_bNeedFeedHome = false;
                 Step.iHome = 0;
                 return true ;

    }
}

//     2010 여기까지....end

bool CFeed::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //if(MT_GetStop(miFED_TIdx) && !IO_GetX(xFED_InSnsr)){
    //    IO_SetY(yFED_InSolLR, true);
    //}

    static bool bCycWait = false;

    //if(m_tmInSolLRFlickTimer.OnDelay(true , OM.CmnOptn.iSolFlickTm)) {
    //    IO_SetY(yFED_InSolLR, !IO_GetY(yFED_InSolLR)); //껏다 켰다.
    //    m_tmInSolLRFlickTimer.Clear() ;
    //}   //상시로 쏜다..InSolLR....





    //Check Error & Decide Step.  riPRB(프리버퍼)->riPRV(프리버퍼비젼존)->riWRK(
    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        const double dPosOfs = OM.MstOptn.dTrsPosOfs;

        bool bWorkEnd  = false;
        bool bLotOpened  = LTQ.GetLotOpen() ;

        bool bTrsBwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd ) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                            MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) + dPosOfs  ;

        bool bTrsFwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                            MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) + dPosOfs  ;

        bool bFeedOutCon ;
        bool bFeedEndCon ;
        if(OM.CmnOptn.iUseLsrPos == 0){
            bFeedOutCon  = TRS.GetSeqStep() == CTransfer::scIdle &&
                           (bTrsBwdPos && DM.ARAY[riTRS_B].GetCntStat(csNone)) ||
                           (bTrsFwdPos && DM.ARAY[riTRS_F].GetCntStat(csNone)) ;

            bFeedEndCon  = TRS.GetSeqStep() == CTransfer::scIdle &&
                           (bTrsFwdPos && DM.ARAY[riTRS_B].CheckAllStat(csUnkwn)) ||
                           (bTrsBwdPos && DM.ARAY[riTRS_F].CheckAllStat(csUnkwn)) ;
        }
        else if(OM.CmnOptn.iUseLsrPos == 1){
            bFeedOutCon  = TRS.GetSeqStep() == CTransfer::scIdle &&  (bTrsBwdPos && DM.ARAY[riTRS_F].GetCntStat  (csNone )) ;
            bFeedEndCon  = TRS.GetSeqStep() == CTransfer::scIdle && ((bTrsFwdPos && DM.ARAY[riTRS_F].CheckAllStat(csUnkwn)) || bTrsFwdPos ) ;
        }
        else{
            bFeedOutCon  = TRS.GetSeqStep() == CTransfer::scIdle &&  (bTrsFwdPos && DM.ARAY[riTRS_B].GetCntStat  (csNone )) ;
            bFeedEndCon  = TRS.GetSeqStep() == CTransfer::scIdle && ((bTrsBwdPos && DM.ARAY[riTRS_B].CheckAllStat(csUnkwn)) || bTrsBwdPos ) ;
        }  //KJH 마킹 포지션 선택 옵션 때문에...

        bool bMtrStop     = MT_GetStop(miFED_TIdx);
        bool bFeedWrkCon  = bMtrStop && bFeedOutCon ;
        bool bFeedInSnsr  = IO_GetX(xFED_InSnsr) ; //확인후 온 딜레이 IO할당.
        bool bFeedInCon   = bFeedInSnsr ;

        bool bPstnOutSnsr = IO_GetX(xFED_PstnOut);


        bool bOnePartLsr  = OM.CmnOptn.iUseLsrPos != 0 && (DM.ARAY[riTRS_F].CheckAllStat(csUnkwn) || DM.ARAY[riTRS_B].CheckAllStat(csUnkwn));


        if(bFeedInSnsr) m_bFirstChipIn = false ;
        //Feeding in Timeout /*
        if(!OM.MstOptn.bDryRun){
            if(m_bFirstChipIn){
                if(bFeedInSnsr) m_tmFeedStartTimeOut.Clear();
                //int iFeedStartTimeOut = 30000 ;
                if(m_tmFeedStartTimeOut.OnDelay(true , OM.CmnOptn.iFedSttTO /*iFeedStartTimeOut*/)) EM_SetErr(eiFED_PkgInStartTO);
            }
            else {
                if(bFeedInSnsr) m_tmFeedTimeOut     .Clear();
                //const int iFeedTimeOut = 10000;
                if(m_tmFeedTimeOut.OnDelay(true , OM.CmnOptn.iFedTO /*iFeedTimeOut*/) && bLotOpened) EM_SetErr(eiFED_PkgInTO);
            }

            //Rqst LotEnd 후에 WorkEnd결정 싸이클.
            if(!bLotOpened) {
                if(bFeedInSnsr) m_tmRqstLotEndTimer.Clear();
                //const int iWorkEndTime = 30000 ;
                if(m_tmRqstLotEndTimer.OnDelay(true , OM.CmnOptn.iWorkEndTime /*iWorkEndTime*/))bWorkEnd = true ;
            }
        }





        bool isCycleHome = TRS.GetSeqStep() != CTransfer::scIdle && m_bNeedFeedHome;
        bool isCycleWait = !bFeedInSnsr && !bWorkEnd;
        bool isCycleWork = !bWorkEnd && !bFeedEndCon && bFeedWrkCon && (bFeedInCon || !bLotOpened ) && !OM.MstOptn.bDryRun;


        if(OM.MstOptn.bDryRun){
           //      if(MT_GetCmdPos(miTRS_YStc) == PM.GetValue(miTRS_YStc,pvTRS_YStcFwd)) { DM.ARAY[riTRS_B].SetStat(csUnkwn);}
           // else if(MT_GetCmdPos(miTRS_YStc) == PM.GetValue(miTRS_YStc,pvTRS_YStcBwd)) { DM.ARAY[riTRS_F].SetStat(csUnkwn);}
           //if(m_bbbb){DM.ARAY[riTRS_B].SetStat(csUnkwn); m_bbbb = false; }
           //else      {DM.ARAY[riTRS_F].SetStat(csUnkwn); m_bbbb = true ; }
        }
        bool isCycleEnd  = bOnePartLsr || bWorkEnd ; //!isCycleWork ; //TRS.m_iPkgCnt >= OM.DevOptn.iPkgCnt ; //여기에 조건 추가 해야 할지도...
        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleWait ) {Trace(m_sPartName.c_str(),"CycleWait Stt"); Step.iSeq = scWait  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork ) {Trace(m_sPartName.c_str(),"CycleWork Stt"); Step.iSeq = scWork  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleHome ) {Trace(m_sPartName.c_str(),"CycleHome Stt"); Step.iSeq = scHome  ; Step.iHome  = 10 ; PreStep.iHome  = 0 ;} //
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default         :                      Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                       return false ;
        case scWait     : if(CycleWait    ()){ Trace(m_sPartName.c_str(),"CycleWait End");Step.iSeq = scIdle ;} return false ;
        case scWork     : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork End");Step.iSeq = scIdle ;} return false ;
        case scHome     : if(CycleHome    ()){ Trace(m_sPartName.c_str(),"CycleHome End");Step.iSeq = scIdle ;} return false ;
    }
    return false ;

}

bool CFeed::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    //if (LDR.GetSeqStep() == CLoader::scWork) return false ; //로더 워킹 하면서 푸쉬 하고 있을때 스탑을 되어 스트립 걸려 있는 것 방지.
    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CFeed::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
   // if (TRS.GetSeqStep() == CTransfort::scMove) return false ;

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CFeed::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiPRB_ToStartTO);

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

        case 10: //IO_SetY(yFED_InSolLR  , true);
                 //IO_SetY(yFED_OutSolLR , true);
                 IO_SetY(yETC_Feed , true);
                 Step.iToStart++;
                 return false ;

        case 11: m_tmFeedTimeOut      .Clear() ;
                 m_tmFeedStartTimeOut .Clear() ;
                 m_tmRqstLotEndTimer  .Clear() ;
                 m_tmInSolLRFlickTimer.Clear() ;
                 m_tmInSolRLFlickTimer.Clear() ;
                 m_tmInSolRLWaitTimer .Clear() ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CFeed::ToStop(void) //스탑을 하기 위한 함수.
{

    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiPRB_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    double dTemp = 0;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: m_tmTemp.Clear();
                 IO_SetY(yFED_OutSolLR ,true);
                 IO_SetY(yFED_InSolLR  ,true);

                 //m_iPitchCnt = 0;
                 //MT_SetPos(miFED_TIdx , 0);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!m_tmTemp.OnDelay(true , 500)) return false;
                 IO_SetY(yFED_OutSolLR , false);
                 IO_SetY(yFED_InSolLR  , false);
                 IO_SetY(yFED_InSolRL  , false);
                 IO_SetY(yETC_Feed     , false);
                 Step.iToStop ++ ;
                 return false ;

        case 12: //if(!IO_GetY(yFED_OutSolLR)) return false;
                 //if(!IO_GetY(yFED_InSolLR )) return false;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CFeed::CycleWait(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPRB_CycleTO);
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

    //만약에 어떤스텝에서라도 미리 들어 왔으면
    //if(IO_GetX(xFed

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: IO_SetY(yFED_InSolLR , true);
                 m_tmInSolLRFlickTimer.Clear();

                 Step.iCycle++ ;
                 return false ;

        case 11: if(IO_GetX(xFED_InSnsr)){
                     IO_SetY(yFED_InSolLR , false);
                     Step.iCycle = 0 ;
                     return true ;

                 }
                 if(!m_tmInSolLRFlickTimer.OnDelay(true , OM.CmnOptn.iInSolLftTm)) return false;
                 IO_SetY(yFED_InSolLR , false);
                 m_tmTemp.Clear();

                 Step.iCycle++ ;
                 return false ;

        case 12: if(IO_GetX(xFED_InSnsr)){
                     IO_SetY(yFED_InSolLR , false);
                     Step.iCycle = 0 ;
                     return true ;
                 }

                 if(!m_tmTemp.OnDelay(true , 1000))return false;

                 IO_SetY(yFED_InSolRL , true);
                 m_tmInSolRLFlickTimer.Clear();

                 Step.iCycle++ ;
                 return false ;

        case 13: if(!m_tmInSolRLFlickTimer.OnDelay(true , OM.CmnOptn.iInSolRghTm)) return false;

                 IO_SetY(yFED_InSolRL , false);
                 m_tmTemp.Clear();

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!m_tmTemp.OnDelay(true , 500)) return false;
                 Step.iCycle = 0 ;
                 return true ;
    }
}

bool CFeed::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPRB_CycleTO);
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

    int r=0,c ;
    const double dPosOfs = OM.MstOptn.dTrsPosOfs ;
    bool bTrsFwdPos , bTrsBwdPos;

    static DWORD dPreTickTime = GetTickCount();
    //double dCrntTickTime = GetTickTime();
    static bool bCntSnsrDnEg = false ;
    static bool bCntSnsrUpEg = false ;


    //bool bOutSnsr = IO_GetX(xFED_OutSnsr);

    //static bool bPreOutSnsr  = false ;


    //if(!bPreOutSnsr && bOutSnsr ) { //어맛! 새로운 랏.
    //    m_bOutSnsrDetect = true;

    //}
    //bPreOutSnsr = bOutSnsr;







    if(Step.iCycle >= 12) {
        if(MT_GetStop(miFED_TIdx)){
            if(IO_GetXDn(xFED_PkgCntSnsr)){
                bCntSnsrDnEg = true ;
            }
            if(IO_GetXDn(xFED_OutSnsr)){
                m_bOutSnsrDetect  = true ;
            }
        }
    }
    //if(IO_GetXDn(xFED_PkgCntSnsr)){
    //    bCntSnsrDnEg = true ;
    //}



    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;
        case  10: //IO_SetY(yFED_InSolLR  , true);
                  IO_SetY(yFED_OutSolLR , true);

                  bCntSnsrDnEg     = false ;
                  m_bOutSnsrDetect = false ;
                  m_bOutFlick      = false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!m_tmTemp.OnDelay(true , 400))return false; //JS
                  Step.iCycle ++;
                  return false;


        //스탭바꿀때 주의 위에서씀.
        case  12: //IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);

                  m_iPitchCnt++;
                  MoveMotr(miFED_TIdx , piFED_TIdxWork);

                  Step.iCycle++;
                  return false;

        case  13: //if(!MT_GetStop(miFED_TIdx)) return false; //|| ( !OM.MstOptn.bDryRun && !IO_GetX(xLD1_PusherBw)) ) return false;
                  m_iVibrationCnt = 0;
                  //if(!IO_GetXUp(xFED_PstnOut) && !IO_GetXDn(xFED_PstnOut) ){
                  //if(bCntSnsrUpEg){
                  //    EM_SetErr(eiFED_PstnOut);
                  //    Step.iCycle=0;
                  //    return true;
                  //}
                  if(!MoveMotr(miFED_TIdx , piFED_TIdxWork)) return false;
                  if(m_iPitchCnt >= 24){
                      m_iPitchCnt = 0;
                      MT_SetPos(miFED_TIdx, 0);
                  }

                  //TwstMT(true);
                  if(!IO_GetXDn(xFED_PstnOut) && IO_GetX(xFED_PstnOut)) {
                      if(!IO_GetX(xFED_OutSnsr) && bCntSnsrDnEg){
                          bTrsFwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                         MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) + dPosOfs  ;
                          bTrsBwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                         MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) + dPosOfs  ;

                          if(bTrsFwdPos) {
                              FindChip(r, c, riTRS_B);
                              DM.ARAY[riTRS_B].SetStat(r,c,csUnkwn);
                          }
                          else if(bTrsBwdPos) {
                              FindChip(r, c, riTRS_F);
                              DM.ARAY[riTRS_F].SetStat(r,c,csUnkwn);
                          }
                      }
                      EM_SetErrMsg(eiFED_PstnOut," FEED");
                      Step.iCycle=0;
                      return true;
                  }

                  //IO_SetY(yFED_InSolLR  , true);
                  IO_SetY(yFED_OutSolLR , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: //if(!TwstMT(false)) return false;
                  if(!IO_GetX(xFED_OutSnsr)){
                      //IO_SetY(yFED_OutSolLR , false);
                      m_tmTemp.Clear();
                      Step.iCycle++;
                      return false;
                  }


                  if(!m_tmTemp.OnDelay(true , 400)) return false;  //JS
                  //IO_SetY(yFED_OutSolLR , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15:
                  IO_SetY(yFED_OutSolRL , false);
                  IO_SetY(yFED_OutSolLR , true );
                  if(!IO_GetX(xFED_OutSnsr) && bCntSnsrDnEg){ //정상적으로 트렌스퍼 까지 배출. 정상 종료.
                      IO_SetY(yFED_OutSolLR , false);
                      Step.iCycle ++ ;
                      return false ;
                  }
                  if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iOutSolLfTm))return false ;


                  //배출 불량 에러 삽입
                  if(IO_GetX(xFED_OutSnsr)){

                      Step.iCycle = 17;
                      return false ;
                  //EM_SetErr(eiFED_PkgOutTO );
                  }

                  if(m_bOutSnsrDetect && !bCntSnsrDnEg){
                      bTrsFwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                     MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) + dPosOfs  ;
                      bTrsBwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                     MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) + dPosOfs  ;

                      if(bTrsFwdPos) {
                          FindChip(r, c, riTRS_B);
                          DM.ARAY[riTRS_B].SetStat(r,c,csUnkwn);
                          //dPreTickTime = GetTickTime() - dCntTickTime;
                          //dCntTickTime = dPreTickTime;

                      }
                      else if(bTrsBwdPos) {
                          FindChip(r, c, riTRS_F);
                          DM.ARAY[riTRS_F].SetStat(r,c,csUnkwn);
                          //dPreTickTime = GetTickTime() - dCntTickTime;
                          //dCntTickTime = dPreTickTime;

                      }
                      //if(!m_bOutFlick){
                          if(!OM.MstOptn.bSkipCntMissErr) EM_SetErr(eiFED_PkgOut2TO);
                          m_bOutSnsrDetect = false;
                      //}

                  }
                  //IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);

                  //Step.iCycle++;
                  //return false; 이거 수정함...밑에 어래이 추가 되니까...JH
                  m_bOutFlick = false;
                  m_iSolFlickCnt = 0;

                  if(OM.MstOptn.bSkipCntMissErr) { // 체크 되어 있으면,
                      if(DM.ARAY[riTRS_B].CheckAllStat(csUnkwn) || DM.ARAY[riTRS_F].CheckAllStat(csUnkwn)){
                          m_dWorkTotalTm = GetTickCount() - dPreTickTime  ;
                          dPreTickTime   = GetTickCount();
                      }

                      IO_SetY(yFED_InSolLR  , false);
                      IO_SetY(yFED_OutSolLR , false);

                      m_bNeedFeedHome = true;
                  }


                  Step.iCycle=0;
                  return true;


                  // 스퉵 밑에서 쓴다...
        case  16: bTrsFwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                 MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) + dPosOfs  ;
                  bTrsBwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                 MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) + dPosOfs  ;

                  if(bTrsFwdPos) {
                      FindChip(r, c, riTRS_B);
                      DM.ARAY[riTRS_B].SetStat(r,c,csUnkwn);

                  }
                  else if(bTrsBwdPos) {
                      FindChip(r, c, riTRS_F);
                      DM.ARAY[riTRS_F].SetStat(r,c,csUnkwn);

                  }

                  if(DM.ARAY[riTRS_B].CheckAllStat(csUnkwn) || DM.ARAY[riTRS_F].CheckAllStat(csUnkwn)){
                      //dPreTickTime = GetTickCount() - dCntTickTime;
                      //dCntTickTime = dPreTickTime;
                      //m_dWorkTotalTm = dPreTickTime;
                      m_dWorkTotalTm = GetTickCount() - dPreTickTime  ;
                      dPreTickTime   = GetTickCount();

                  }
                  //else return false ;

                  IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);

                  m_bNeedFeedHome = true;

                  //m_dWorkTotalTm = dCntTickTime - dPreTickTime;
                  //dPreTickTime = dCntTickTime ;
                  m_iSolFlickCnt = 0;

                  Step.iCycle=0;
                  return true;

        case  17: IO_SetY(yFED_OutSolLR , true);

                  m_tmOutSolRLFlickTimer.Clear();

                  Step.iCycle ++ ;
                  return false ;

        case  18: m_bOutFlick = true;

                  if(!IO_GetX(xFED_OutSnsr)){
                      IO_SetY(yFED_OutSolRL , false);
                      IO_SetY(yFED_OutSolLR , true );
                      //Step.iCycle=15 ;
                      Step.iCycle=16 ;
                      return false ;
                  }

                  if(m_tmOutSolRLFlickTimer.OnDelay(true , OM.CmnOptn.iSolFlickTm)) {
                      //IO_SetY(yFED_OutSolLR , false);
                      if(m_iSolFlickCnt <= OM.CmnOptn.iSolFlickCnt){
                          if(IO_GetX(xFED_OutSnsr)) { //자제 진입 못했을 때에만.
                          //요거 타임은 나중에 바꿔야 한다....LR보다 짧은 시간으로 쏘게 끔.....
                              IO_SetY(yFED_OutSolLR , !IO_GetY(yFED_OutSolLR));
                              IO_SetY(yFED_OutSolRL , !IO_GetY(yFED_OutSolRL)); //껏다 켰다.
                              m_tmOutSolRLFlickTimer.Clear() ;
                              m_iSolFlickCnt++;
                          }
                          else{
                              IO_SetY(yFED_OutSolRL , false);
                              IO_SetY(yFED_OutSolLR , true );
                              m_iSolFlickCnt = 0;
                              //Step.iCycle = 15;
                              Step.iCycle=16 ;
                              return false;
                          }
                      }
                      else{
                          bTrsFwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                         MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) + dPosOfs  ;
                          bTrsBwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                         MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) + dPosOfs  ;

                          if(bTrsFwdPos) {
                              FindChip(r, c, riTRS_B);
                              DM.ARAY[riTRS_B].SetStat(r,c,csUnkwn);
                             //dPreTickTime = GetTickTime() - dCntTickTime;
                             //dCntTickTime = dPreTickTime;

                          }
                          else if(bTrsBwdPos) {
                              FindChip(r, c, riTRS_F);
                              DM.ARAY[riTRS_F].SetStat(r,c,csUnkwn);
                             //dPreTickTime = GetTickTime() - dCntTickTime;
                             //dCntTickTime = dPreTickTime;

                          }

                          m_iSolFlickCnt = 0;

                          IO_SetY(yFED_OutSolRL , false);
                          EM_SetErr(eiFED_PkgOutTO );
                          Step.iCycle=0;
                          return true;
                      }
                  }
                  return false;

    }

        /*
        case  10: IO_SetY(yFED_InSolLR  , true);
                  IO_SetY(yFED_OutSolLR , true);
                  m_iPitchCnt++;
                  bCntSnsrDnEg = false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!IO_GetX(xFED_OutSnsr) && !IO_GetX(xFED_PkgCntSnsr)){ //알이 아직안빠져나갔다.
                      if(!m_tmTemp.OnDelay(true , 300)) return false;  //300미리 기둘리다
                      if(m_iPitchCnt >= 25){
                          m_iPitchCnt = 1;
                          MT_SetPos(miFED_TIdx, 0);
                      }
                      Step.iCycle ++;

                      return false;
                  }
                  if(!m_tmTemp.OnDelay(true , OM.DevOptn.iAirDelay))return false ;
                  //배출 불량 에러 삽입
                  if(!IO_GetX(xFED_OutSnsr   ))EM_SetErr(eiFED_PkgOutTO );
                  if(!IO_GetX(xFED_PkgCntSnsr))EM_SetErr(eiFED_PkgOut2TO);
                  IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);
                  Step.iCycle = 0;
                  return true;

        //스탭바꿀때 주의 위에서씀.
        case  12: IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);
                  MoveMotr(miFED_TIdx , piFED_TIdxWork);

                  Step.iCycle++;
                  return false;

        case  13: //if(!MT_GetStop(miFED_TIdx)) return false; //|| ( !OM.MstOptn.bDryRun && !IO_GetX(xLD1_PusherBw)) ) return false;
                  m_iVibrationCnt = 0;
                  if(!MoveMotr(miFED_TIdx , piFED_TIdxWork)) return false;
                  TwstMT(true);
                  if(!IO_GetXDn(xFED_PstnOut) && IO_GetX(xFED_PstnOut)) {
                      EM_SetErr(eiFED_PstnOut);
                      Step.iCycle=0;
                      return true;
                  }
                  IO_SetY(yFED_InSolLR  , true);
                  IO_SetY(yFED_OutSolLR , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!TwstMT(false)) return false;
                  if(!m_tmTemp.OnDelay(true , 300)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!IO_GetX(xFED_OutSnsr) && bCntSnsrDnEg){ //정상적으로 트렌스퍼 까지 배출. 정상 종료.

                      Step.iCycle ++ ;
                      return false ;
                  }
                  if(!m_tmTemp.OnDelay(true , OM.DevOptn.iAirDelay))return false ;
                  //배출 불량 에러 삽입
                  if(IO_GetX(xFED_OutSnsr))EM_SetErr(eiFED_PkgOutTO );
                  if(bCntSnsrDnEg         )EM_SetErr(eiFED_PkgOut2TO);

                  IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);

                  //Step.iCycle++;
                  //return false; 이거 수정함...밑에 어래이 추가 되니까...JH

                  Step.iCycle=0;
                  return true;


        case  16: bTrsFwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                 MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcFwd) + dPosOfs  ;
                  bTrsBwdPos   = TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) - dPosOfs <= MT_GetCmdPos(miTRS_YStc) &&
                                 MT_GetCmdPos(miTRS_YStc) <= TRS.GetMotrPos(miTRS_YStc , piTRS_YStcBwd) + dPosOfs  ;

                  if(bTrsFwdPos) {
                      FindChip(r, c, riTRS_B);
                      DM.ARAY[riTRS_B].SetStat(r,c,csUnkwn);
                  }
                  else if(bTrsBwdPos) {
                      FindChip(r, c, riTRS_F);
                      DM.ARAY[riTRS_F].SetStat(r,c,csUnkwn);
                  }
                  if(DM.ARAY[riTRS_B].CheckAllStat(csUnkwn) || DM.ARAY[riTRS_F].CheckAllStat(csUnkwn)){
                      dPreTickTime = GetTickTime() - dCntTickTime;
                      dCntTickTime = dPreTickTime;
                  }
                  //else return false ;

                  IO_SetY(yFED_InSolLR  , false);
                  IO_SetY(yFED_OutSolLR , false);

                  Step.iCycle=0;
                  return true;


    }
    */
}



bool CFeed::TwstMT(bool _bVal)     //sun DLL direct access.
{
    /*
    if(_bVal) Step.iTemp = 10;
    PreStep.iTemp = Step.iTemp ;

    AnsiString sTemp;
    switch (Step.iTemp) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iTemp );
              if(Step.iTemp != PreStep.iTemp) Trace(m_sPartName.c_str(), sTemp.c_str());
              Step.iTemp = 0 ;
              return true ;

        case 10: MT_GoIncRun(miFED_TIdx , 0.2);
                 Step.iTemp++;
                 return false ;

        case 11: if(!MT_GetStopInpos(miFED_TIdx)) return false;
                 MT_GoIncRun(miFED_TIdx ,-0.4);
                 Step.iTemp++;
                 return false ;

        case 12: if(!MT_GetStopInpos(miFED_TIdx)) return false;
                 MT_GoIncRun(miFED_TIdx , 0.2);
                 Step.iTemp++;
                 return false ;

        case 13: if(!MT_GetStopInpos(miFED_TIdx)) return false;
                 if(m_iVibrationCnt <= OM.CmnOptn.iVibCount){
                     m_iVibrationCnt++;
                     Step.iTemp=10;
                     return false;
                 }
                 else{
                     Step.iTemp=0;
                     return true;
                 }
    }
 */
}


void CFeed::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CFeed::CheckMoved()
{

    return true ; //이파트는 필요 없다.
}

bool CFeed::CheckStop()
{
    //if(!MT_GetStop(miPRB_XCmp)) return false ;

    //if(!AT_Done(aiPRB_CmpDnUp)) return false ;

    return true ;
}

void CFeed::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName + ".INI";


}
//---------------------------------------------------------------------------
