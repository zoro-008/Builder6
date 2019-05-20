
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "WorkZone.h"
//#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"

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

    m_iBinCnt    = 0 ;
    m_sLastStrip = "";

    m_dFailChipPos   = 0 ;
    m_lStrpCycleTime = 0 ;
    m_lPreCycleTime  = 0 ;

    iGoodCnt         = 0 ;

    m_bLotEnded = false ;

    memset(&Bin1    , 0 , sizeof(SBinInfo ));
    memset(&Bin2    , 0 , sizeof(SBinInfo ));
    memset(&Bin3    , 0 , sizeof(SBinInfo ));
    memset(&Bin4    , 0 , sizeof(SBinInfo ));
    memset(&Bin5    , 0 , sizeof(SBinInfo ));

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
}                                                                                         

bool CWork::FindChip(int &r , int &c )
{
    c = DM.ARAY[riWRK].FindLastCol(csUnkwn);
    c = DM.ARAY[riWRK].GetMaxCol() - c -1;

    if(r == -1 && c == -1) return false ;
    return true ;
}
double CWork::RejectMove(void)
{
    if((DM.ARAY[riWRK].GetLotNo() != m_sLastStrip) /*&& (m_sLastID != "" )*/)
    {
        m_iBinCnt++;
        if(m_iBinCnt > MAXBIN)   m_iBinCnt = 1;
        m_sLastStrip = DM.ARAY[riWRK].GetLotNo();
    }

    if(OM.CmnOptn.bUseOnly1Bin) m_iBinCnt = 1 ;

    double RjtPitch = PM.GetValue(miWRK_XRjt , pvWRK_XRjtBin1) - PM.GetValue(miWRK_XRjt , pvWRK_XRjtBin2) ;
    m_dFailChipPos = PM.GetValue(miWRK_XRjt , pvWRK_XRjtBin1) - (RjtPitch * (m_iBinCnt - 1)) ;
    return m_dFailChipPos;

}
double CWork::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos       = 0.0 ;

    //piWRK_2IdxWork Position
    double Idx1OutGap = PM.GetValue (miWRK_1Idx , pvWRK_1IdxOut) - PM.GetValue (miWRK_1Idx , pvWRK_1IdxWorkStart);

    int r,c ;
    c = OM.DevOptn.iTrimColCnt ;
    double d2IdxWorkPos   = PM.GetValue (miWRK_1Idx , pvWRK_2IdxWait) +  ( c * OM.DevInfo.dColPitch - Idx1OutGap );
    double d2IdxBackGap   = d2IdxWorkPos - PM.GetValue(miWRK_2Idx,pvWRK_2IdxBack) ;
    double d2IdxReWorkPos = PM.GetValue (miWRK_1Idx , pvWRK_2IdxWait) +  d2IdxBackGap + ( c * OM.DevInfo.dColPitch );
    //

    FindChip(r,c);
    //Group Gap ++
    int iColGrCnt ;
    if( OM.DevInfo.iColGrCnt > 1 ) {
        iColGrCnt = c / OM.DevInfo.iColGrCnt ;
        d2IdxWorkPos += (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * iColGrCnt ;
    }

    if(_iMotr == miWRK_ZPch){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piWRK_ZPchWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZPchWait     ); break ;
            case piWRK_ZPchWorkStt   : dPos = PM.GetValue (_iMotr , pvWRK_ZPchWork     ); break ;
        }
    }
    else if(_iMotr == miWRK_XRjt){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piWRK_XRjtBin1      : dPos = PM.GetValue (_iMotr , pvWRK_XRjtBin1     ); break ;
            case piWRK_XRjtBin2      : dPos = PM.GetValue (_iMotr , pvWRK_XRjtBin2     ); break ;
            case piWRK_XRjtWork      : dPos = RejectMove();                                break ;
        }
    }
    else if(_iMotr == miWRK_1Idx){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piWRK_1IdxWait      : dPos = PM.GetValue (_iMotr , pvWRK_1IdxWait     ); break ;
            case piWRK_1IdxWorkStart : dPos = PM.GetValue (_iMotr , pvWRK_1IdxWorkStart); break ;
            case piWRK_1IdxOut       : dPos = PM.GetValue (_iMotr , pvWRK_1IdxOut      ); break ;
        }
    }
    else if(_iMotr == miWRK_2Idx){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piWRK_2IdxWait      : dPos = PM.GetValue (_iMotr , pvWRK_2IdxWait     ); break ;
            case piWRK_2IdxPull      : dPos = PM.GetValue (_iMotr , pvWRK_2IdxPull     ); break ;
            case piWRK_2IdxBack      : dPos = PM.GetValue (_iMotr , pvWRK_2IdxBack     ); break ;
            case piWRK_2IdxOut       : dPos = PM.GetValue (_iMotr , pvWRK_2IdxOut      ); break ;
            case piWRK_2IdxWork      : dPos = d2IdxWorkPos  ;                             break ;
            case piWRK_2IdxReWork    : dPos = d2IdxReWorkPos;                             break ;
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
    bool is1IdxDn     =  AT_Complete(aiWRK_1Idx,ccFwd);
    bool is2IdxDn     =  AT_Complete(aiWRK_2Idx,ccFwd);

    if(_iMotr == miWRK_ZPch){
        switch(_iPstnId) {
            default                  : break ;
            case piWRK_ZPchWait      : break ;
            case piWRK_ZPchWorkStt   : break ;
        }
    }
    else if(_iMotr == miWRK_XRjt){
        switch(_iPstnId) {
            default                  : break ;
            case piWRK_XRjtBin1      : break ;
            case piWRK_XRjtWork      : break ;
        }
    }
    else if(_iMotr == miWRK_1Idx){
        switch(_iPstnId) {
            default                  : break ;
            case piWRK_1IdxWait      : break ;//if(is1IdxDn  ) { sMsg = "WRK_1Index Down"; bRet = false; }  break ;
            case piWRK_1IdxWorkStart : if(isZPunchDn) { sMsg = "WRK_ZPunch Down"; bRet = false; }  break ;
            
        }
    }
    else if(_iMotr == miWRK_2Idx){
        switch(_iPstnId) {
            default                  : break ;
            case piWRK_2IdxWait      : if(is2IdxDn  ) { sMsg = "WRK_2Index Down"; bRet = false; }  break ;
            case piWRK_2IdxPull      : break ;
            case piWRK_2IdxBack      : if(is2IdxDn  ) { sMsg = "WRK_2Index Down"; bRet = false; }  break ;
            case piWRK_2IdxOut       : if(isZPunchDn) { sMsg = "WRK_ZPunch Down"; bRet = false; }  break ;
            case piWRK_2IdxWork      : if(isZPunchDn) { sMsg = "WRK_ZPunch Down"; bRet = false; }  break ;
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle || Step.iSubCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return bRet ;
}

//---------------------------------------------------------------------------
bool  CWork::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool is1IdxMoving = !MT_GetStop(miWRK_1Idx);
    bool is2IdxMoving = !MT_GetStop(miWRK_2Idx);

    if(_iActr == aiWRK_1Idx) {
        if(is1IdxMoving) {
            sMsg = "WRK 1 Index is Moving" ;
            bRet = false ;
        }
    }
    else if(_iActr == aiWRK_2Idx) {
        if(is2IdxMoving) {
            sMsg = "WRK 2 Index is Moving" ;
            bRet = false ;
        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle || Step.iSubCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CWork::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle || Step.iSubCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else                              return MT_GoAbsMan(_iMotr , dPosition);
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

        case 10: MT_Reset(miWRK_1Idx);
                 MT_Reset(miWRK_2Idx);
                 MT_Reset(miWRK_XRjt);
                 MT_Reset(miWRK_ZPch);

                 MT_SetServo(miWRK_1Idx,true);
                 MT_SetServo(miWRK_2Idx,true);
                 MT_SetServo(miWRK_XRjt,true);
                 MT_SetServo(miWRK_ZPch,true);

                 AT_MoveCyl(aiWRK_1Idx , ccBwd);
                 AT_MoveCyl(aiWRK_2Idx , ccBwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_1Idx , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWRK_2Idx , ccBwd)) return false ;
                 MT_DoHome(miWRK_1Idx) ;
                 MT_DoHome(miWRK_2Idx) ;
                 MT_DoHome(miWRK_XRjt) ;
                 MT_DoHome(miWRK_ZPch) ;
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRK_1Idx))return false ;
                 if(!MT_GetHomeEnd(miWRK_2Idx))return false ;
                 if(!MT_GetHomeEnd(miWRK_XRjt))return false ;
                 if(!MT_GetHomeEnd(miWRK_ZPch))return false ;

                 MT_GoAbsMan(miWRK_1Idx , PM.GetValue(miWRK_1Idx , pvWRK_1IdxWait));
                 MT_GoAbsMan(miWRK_2Idx , PM.GetValue(miWRK_2Idx , pvWRK_2IdxWait));
                 MT_GoAbsMan(miWRK_XRjt , PM.GetValue(miWRK_XRjt , pvWRK_XRjtBin1));
//                 MT_GoAbsMan(miWRK_ZPch , PM.GetValue(miWRK_ZPch , pvWRK_ZPchWait));
                 MT_GoAbsMan(miWRK_ZPch , m_dLastXSrtPos);

                 Step.iHome++;
                 return false ;

        case 13: if(m_dLast1IdxPos != 0 && !MT_GoAbsMan(miWRK_1Idx ,  m_dLast1IdxPos)) return false ;
                 if(m_dLast2IdxPos != 0 && !MT_GoAbsMan(miWRK_2Idx ,  m_dLast2IdxPos)) return false ;
                 if(m_dLastXSrtPos != 0 && !MT_GoAbsMan(miWRK_XRjt ,  m_dLastXSrtPos)) return false ;
                 if(!MT_GoAbsMan(miWRK_ZPch , PM.GetValue(miWRK_ZPch , pvWRK_ZPchWait))) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CWork::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;
//    m_ToStopTimer.Clear();

    if (Step.iSeq || Step.iSubSeq) return false;

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

        case 10: AT_MoveCyl(aiWRK_1Idx,ccBwd);
//                 AT_MoveCyl(aiWRK_2Idx,ccBwd);
                 MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                 MoveMotr(miWRK_XRjt,piWRK_XRjtWork);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_1Idx,ccBwd       ))return false ;
//                 if(!AT_MoveCyl(aiWRK_2Idx,ccBwd       ))return false ;
                 if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait))return false ;
                 if(!MoveMotr(miWRK_XRjt,piWRK_XRjtWork))return false ;
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

        case 10: if(!MT_GetAlarm   (miWRK_1Idx) && !MT_GetNLimSnsr(miWRK_1Idx) && !MT_GetPLimSnsr(miWRK_1Idx) ) m_dLast1IdxPos = MT_GetCmdPos(miWRK_1Idx) ;
                 else                                                                                           m_dLast1IdxPos = GetMotrPos  (miWRK_1Idx , piWRK_1IdxWait) ;
                 if(!MT_GetAlarm   (miWRK_2Idx) && !MT_GetNLimSnsr(miWRK_2Idx) && !MT_GetPLimSnsr(miWRK_2Idx) ) m_dLast2IdxPos = MT_GetCmdPos(miWRK_2Idx) ;
                 else                                                                                           m_dLast2IdxPos = GetMotrPos  (miWRK_2Idx , piWRK_2IdxWait) ;
//                 if(!MT_GetAlarm   (miWRK_XRjt) && !MT_GetNLimSnsr(miWRK_XRjt) && !MT_GetPLimSnsr(miWRK_XRjt) ) m_dLastXSrtPos = MT_GetCmdPos(miWRK_XRjt) ;
//                 else                                                                                           m_dLastXSrtPos = GetMotrPos  (miWRK_XRjt , piWRK_XRjtBin1) ;
//애 굳이 있을필요가 있나.?dd
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CWork::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.

    PreStep.iSeq    = Step.iSeq    ;
    PreStep.iSubSeq = Step.iSubSeq ;

    if ( IO_GetX(xWRK_1IdxOverload) ) {
        EM_SetErr(eiWRK_1IdxOverLoad );
        MT_Stop(miWRK_1Idx);
        Step.iSeq    = 0 ;
        Step.iSubSeq = 0 ;
        return false ;
    }

    if ( IO_GetX(xWRK_2IdxOverload) ) {
        EM_SetErr(eiWRK_2IdxOverLoad );
        MT_Stop(miWRK_2Idx);
        Step.iSeq    = 0 ;
        Step.iSubSeq = 0 ;
        return false ;
    }

    if (!IO_GetX(xWRK_ToolDetected) && !OM.MstOptn.bDebugMode ) EM_SetErr(eiWRK_ToolSsrNotDectected );
    if ( IO_GetX(xETC_AreaFront   ) ) {
        EM_SetErr(eiETC_AreaSsrDectected    );
//        for(int i=0 ; i< MAX_MOTR ; i++ ) MT_Stop(i);
        MT_Stop(miWRK_ZPch);
//        MT_SetServo(miWRK_ZPch,false);
        Trace("**** 트림구간 Area 센서 감지 ****","트림 모터 STOP & 트림모터 서보 Off");
        Step.iSeq    = 0 ;
        Step.iSubSeq = 0 ;
        return false ;
    }

    if(Step.iSeq == 0 && Step.iSubSeq == 0) {
        bool isConEnd          = (DM.ARAY[riWRK].CheckAllStat(csNone) && DM.ARAY[riWRL].CheckAllStat(csNone) );//&& DM.ARAY[riWRK].GetStep() == asWRK_None );
        if (isConEnd          ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Check Error & Decide Step.
    if (Step.iSeq == 0 ) { //생각해 보니...안되네 나눔.dd
        //Main
        bool isCycleToWrkStt   = !DM.ARAY[riWRK].CheckAllStat(csNone) && !DM.ARAY[riWRK].GetCntStat  (csEmpty) && DM.ARAY[riWRK].GetStep() == asWRK_ToWrkStt   ; //Enum 매겨야 되는데...7시라 지금 밥먹고 매우 졸림.dd
        bool isCycle1IdxWork   = !DM.ARAY[riWRK].CheckAllStat(csNone) && !DM.ARAY[riWRK].GetCntStat  (csEmpty) && DM.ARAY[riWRK].GetStep() == asWRK_1IdxWork   ; //결국함.dd
        bool isCycleTo2Idx     = !DM.ARAY[riWRK].CheckAllStat(csNone) &&  DM.ARAY[riWRK].GetCntStat  (csEmpty) && DM.ARAY[riWRK].GetStep() == asWRK_To2Idx     ;

        if(isCycle1IdxWork) {
            unsigned long lCrntTime = GetTickCount() ;
            m_lStrpCycleTime = lCrntTime - m_lPreCycleTime ;
            m_lPreCycleTime  = lCrntTime ;
        }

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleToWrkStt   ) {Trace(m_sPartName.c_str(),"CycleToWrkStt   Stt"); Step.iSeq = scWrkStt     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycle1IdxWork   ) {Trace(m_sPartName.c_str(),"Cycle1IdxWork   Stt"); Step.iSeq = sc1IdxWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleTo2Idx     ) {Trace(m_sPartName.c_str(),"CycleTo2Idx     Stt"); Step.iSeq = scTo2Idx     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //

    }
    if (Step.iSubSeq == 0 ) { //Sorry ;;; Part ...
        //Sub
        bool isCycle2IdxWorkStt=  !DM.ARAY[riWRL].CheckAllStat(csNone) &&  DM.ARAY[riWRL].GetCntStat  (csEmpty) && DM.ARAY[riWRL].GetStep() == asWRL_2IdxWorkStt;
        bool isCycle2IdxWork   =  !DM.ARAY[riWRL].CheckAllStat(csNone) &&  DM.ARAY[riWRL].GetCntStat  (csEmpty) && DM.ARAY[riWRL].GetStep() == asWRL_2IdxWork  ;
        bool isCycle2IdxReWork =  !DM.ARAY[riWRL].CheckAllStat(csNone) &&  DM.ARAY[riWRL].GetCntStat  (csEmpty) && DM.ARAY[riWRL].GetStep() == asWRL_2IdxReWork ; //3번이면 다찍는다네...dd
        bool isCycle2IdxOut    =  !DM.ARAY[riWRL].CheckAllStat(csNone) &&  DM.ARAY[riWRL].CheckAllStat(csEmpty) && DM.ARAY[riWRL].GetStep() >= asWRL_2IdxPull   ;

        if(DM.ARAY[riWRL ].CheckAllStat(csNone) &&  IO_GetX(xPSB_Strip3) && !OM.CmnOptn.bDryRun &&
            Step.iSubSeq != sc2IdxOut ) EM_SetErr(eiPSB_PkgUnknown ) ; //모르는 스트립에러.
//        if( DM.ARAY[riWRK ].GetCntExist() && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun ) EM_SetErr(eiPRB_PkgDispr ) ; //스트립 사라짐 에러.

        if(EM_IsErr()) return false ;

        //Sub Decide Step.
             if (isCycle2IdxWorkStt) {Trace(m_sPartName.c_str(),"Cycle2IdxWorkStt Stt"); Step.iSubSeq = sc2IdxWorkStt ; Step.iSubCycle = 10 ; PreStep.iSubCycle = 0 ;} //
        else if (isCycle2IdxWork   ) {Trace(m_sPartName.c_str(),"Cycle2IdxWork    Stt"); Step.iSubSeq = sc2IdxWork    ; Step.iSubCycle = 10 ; PreStep.iSubCycle = 0 ;} //
        else if (isCycle2IdxReWork ) {Trace(m_sPartName.c_str(),"Cycle2IdxReWork  Stt"); Step.iSubSeq = sc2IdxReWork  ; Step.iSubCycle = 10 ; PreStep.iSubCycle = 0 ;} //
        else if (isCycle2IdxOut    ) {Trace(m_sPartName.c_str(),"Cycle2IdxOut     Stt"); Step.iSubSeq = sc2IdxOut     ; Step.iSubCycle = 10 ; PreStep.iSubCycle = 0 ;} //
//        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq   ) {
        default            :                                                                           Step.iSeq = scIdle ;  break;//return false ;
        case  scWrkStt     : if(CycleToWrkStt   ()){ Trace(m_sPartName.c_str(),"CycleToWrkStt    End");Step.iSeq = scIdle ;} break;//return false ;
        case  sc1IdxWork   : if(Cycle1IdxWork   ()){ Trace(m_sPartName.c_str(),"Cycle1IdxWork    End");Step.iSeq = scIdle ;} break;//return false ;
        case  scTo2Idx     : if(CycleTo2Idx     ()){ Trace(m_sPartName.c_str(),"CycleTo2Idx      End");Step.iSeq = scIdle ;} break;//return false ;
    }
    //Cycle.
    switch (Step.iSubSeq) {
        default            :                                                                           Step.iSubSeq = scIdle ;  break;//return false ;
        case  sc2IdxWorkStt: if(Cycle2IdxWorkStt()){ Trace(m_sPartName.c_str(),"Cycle2IdxWorkStt End");Step.iSubSeq = scIdle ;} break;//return false ;
        case  sc2IdxWork   : if(Cycle2IdxWork   ()){ Trace(m_sPartName.c_str(),"Cycle2IdxWork    End");Step.iSubSeq = scIdle ;} break;//return false ;
        case  sc2IdxReWork : if(Cycle2IdxReWork ()){ Trace(m_sPartName.c_str(),"Cycle2IdxReWork  End");Step.iSubSeq = scIdle ;} break;//return false ;
        case  sc2IdxOut    : if(Cycle2IdxOut    ()){ Trace(m_sPartName.c_str(),"Cycle2IdxOut     End");Step.iSubSeq = scIdle ;} break;//return false ;
    }

    return false ;
}


bool CWork::CycleToWrkStt(void) //
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

        case 10 : MoveMotr(miWRK_XRjt,piWRK_XRjtWork); // case 14
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait); // case 13
                  MoveActr(aiWRK_1Idx,ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiWRK_1Idx,ccBwd)) return false ;

                  MoveMotr(miWRK_1Idx,piWRK_1IdxWait);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miWRK_1Idx,piWRK_1IdxWait)) return false ;

                  MoveActr(aiWRK_1Idx,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiWRK_1Idx,ccFwd)) return false ;
                  if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;

                  MT_GoAbsRun(miWRK_1Idx, PM.GetValue(miWRK_1Idx,pvWRK_1IdxWorkStart) - 5 );
//                  MoveMotr(miWRK_1Idx,piWRK_1IdxWorkStart);

                  Step.iCycle++;
                  return false ;

        case 14 : if(!MT_GetStop(miWRK_1Idx)) return false ;
                  MT_GoAbs(miWRK_1Idx, PM.GetValue(miWRK_1Idx,pvWRK_1IdxWorkStart),10);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MT_GetStop(miWRK_1Idx)) return false ;
                  if(!MoveMotr(miWRK_1Idx,piWRK_1IdxWorkStart)) return false ;
                  //Next Step .
                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep()+1);
                  if(!MoveMotr(miWRK_XRjt,piWRK_XRjtWork)) return false ;
//                  MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt);
                  MoveActr(aiWRK_1Idx, ccBwd);

                  Step.iCycle++;
                  return false;

        case 16:  if(!MoveActr(aiWRK_1Idx, ccBwd))return false ;

                  Step.iCycle = 0;
                  return true ;
    }
}

bool CWork::Cycle1IdxWork(void) //
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

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}
    int r,c ;
    int iMaxRow ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : if(m_sPreLotNo != DM.ARAY[riWRK].GetLotNo() ){ //여기서 LotOpen 해버렷.!!!dd
                      Trace("LotOpen",DM.ARAY[riWRK].GetLotNo().c_str());
                      LT.LotOpen(DM.ARAY[riWRK].GetLotNo(), OM.m_sCrntDev );
                      LT.AddDayInfoLotCnt(1);

                      m_sPreLotNo = DM.ARAY[riWRK].GetLotNo() ;
                      Load(false);
                  }
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;

                  if(!BinFullCheck()) return false ; //안에서 그냥 에러 띠움...

                  MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt)) return false ;
                  BinInfoAdd(riWRK);
                  iMaxRow = DM.ARAY[riWRK].GetMaxRow();
                  c = DM.ARAY[riWRK].FindLastCol(csUnkwn);
                  for(int j =0 ; j < OM.DevOptn.iTrimColCnt ; j++ ) {
                      for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csEmpty); //Led 따먹음.dd
                  }                                                                                   //첫줄이라 그냥 해도 되는데...이렇게함 dd
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;
                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep()+1);
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CWork::CycleTo2Idx(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

        case  10: MoveMotr(miWRK_1Idx,piWRK_1IdxWorkStart);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRK_1Idx,piWRK_1IdxWorkStart)) return false ;

                  MoveActr(aiWRK_2Idx,ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveActr(aiWRK_1Idx,ccFwd))return false ;
                  if(!MoveActr(aiWRK_2Idx,ccBwd))return false ;
                  MoveMotr(miWRK_1Idx,piWRK_1IdxOut);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveMotr(miWRK_1Idx,piWRK_1IdxOut))return false ;
                  MoveActr(aiWRK_1Idx,ccBwd);
                  MoveMotr(miWRK_1Idx,piWRK_1IdxWait); //핑거를 업하지 못하였다. dd 미는 방식이라 그냥 뒤로 빼는데 바뀌면 수정해야됨.dd
//                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep()+1);
                  DM.ShiftArrayData(riWRK,riWRL);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRK_1Idx, piWRK_1IdxWait)) return false ;
                  if(!MoveActr(aiWRK_1Idx, ccBwd         ))return false;

                  Step.iCycle = 0;
                  return true ;

//        case  15: if(!MoveActr(aiWRK_1Idx,ccBwd)) return false ;
//                  DM.ARAY[riWRK].SetStep(asWRK_2IdxWorkStt);
//                  Step.iCycle = 0 ;
//                  return true ;
    }
}

bool CWork::Cycle2IdxWorkStt(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmSubCycle.OnDelay(Step.iSubCycle == PreStep.iSubCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iSubCycle = 0 ;
        return true;
    }

    if(Step.iSubCycle != PreStep.iSubCycle) {
        sTemp = sTemp.sprintf("%s Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSubCycle = Step.iSubCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    int r,c ;
    int iMaxRow ;
    //Cycle.
    switch (Step.iSubCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iSubCycle );
                  if(Step.iSubCycle != PreStep.iSubCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iSubCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRK_2Idx,ccBwd);
//                  MoveActr(aiWRK_1Idx,ccBwd); //여기서 할게 아닌데.dd
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                  Step.iSubCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiWRK_2Idx,ccBwd)) return false ;
//                  if(!MoveActr(aiWRK_1Idx,ccBwd)) return false ;
                  if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;
                  MoveMotr(miWRK_2Idx,piWRK_2IdxWait);
                  Step.iSubCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxWait)) return false ;
                  MoveActr(aiWRK_2Idx,ccFwd);
                  Step.iSubCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiWRK_2Idx,ccFwd)) return false ;

//                  if(!BinFullCheck()) return false ; //안에서 그냥 에러 띠움...

//                  MoveMotr(miWRK_2Idx,piWRK_2IdxWork);
                  MT_GoAbsRun(miWRK_2Idx, GetMotrPos(miWRK_2Idx,piWRK_2IdxWork) - 5 );
//                  MoveMotr(miWRK_1Idx,piWRK_1IdxWorkStart);

                  Step.iSubCycle++;
                  return false ;

        case 14 : if(!MT_GetStop(miWRK_2Idx)) return false ;
                  MT_GoAbs(miWRK_2Idx, GetMotrPos(miWRK_2Idx,piWRK_2IdxWork),10 );
                  Step.iSubCycle++;
                  return false ;

        case 15 : if(!MT_GetStop(miWRK_2Idx)) return false ;
                  if(!MoveMotr(miWRK_2Idx,piWRK_2IdxWork)) return false ;

//                  Step.iSubCycle++ ;
//                  return false ;

//        case  14: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxWork)) return false ;
                  DM.ARAY[riWRL].SetStep(DM.ARAY[riWRL].GetStep()+1);
                  Step.iSubCycle = 0 ;
                  return true ;
    }
}

bool CWork::Cycle2IdxWork(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmSubCycle.OnDelay(Step.iSubCycle == PreStep.iSubCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iSubCycle = 0 ;
        return true;
    }

    if(Step.iSubCycle != PreStep.iSubCycle) {
        sTemp = sTemp.sprintf("%s Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSubCycle = Step.iSubCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    int r,c ;
    int iMaxRow ;
    //Cycle.
    switch (Step.iSubCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iSubCycle != PreStep.iSubCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iSubCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_2Idx,piWRK_2IdxWork);
                  Step.iSubCycle++ ;
                  return false ;

        case  11: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxWork)) return false ;
                  MoveActr(aiWRK_2Idx,ccFwd);

                  Step.iSubCycle++ ;
                  return false ;

        case  12: if(!MoveActr(aiWRK_2Idx,ccFwd)) return false ;

                  if(!BinFullCheck()) return false ; //안에서 그냥 에러 띠움...

                  MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt);
                  Step.iSubCycle++ ;
                  return false ;

        case  13: if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt)) return false ;
                  BinInfoAdd(riWRL);

                  iMaxRow = DM.ARAY[riWRL].GetMaxRow();
                  c = DM.ARAY[riWRL].FindLastCol(csUnkwn);
                  for(int j =0 ; j < OM.DevOptn.iTrimColCnt ; j++ ) {
                      for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRL].SetStat(i , c - j, csEmpty); //Led 따먹음.dd
                  } //

                  if(OM.DevInfo.iColCnt / (float)OM.DevOptn.iTrimColCnt > 2) { //2줄이상일때에 보내버림.dd
                      DM.ARAY[riWRL].SetStep(asWRL_2IdxReWork );
                      Step.iSubCycle = 0 ;
                      return true ;
                  }

//                  LotInfoAdd();

                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                  Step.iSubCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;
                  DM.ARAY[riWRL].SetStep(asWRL_2IdxOut    );
//                  if(OM.DevInfo.iColCnt / OM.DevOptn.iTrimColCnt == 3) DM.ARAY[riWRL].SetStep(asWRK_2IdxReWork );

                  Step.iSubCycle = 0 ;
                  return true ;
    }
}

bool CWork::Cycle2IdxReWork(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmSubCycle.OnDelay(Step.iSubCycle == PreStep.iSubCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iSubCycle = 0 ;
        return true;
    }

    if(Step.iSubCycle != PreStep.iSubCycle) {
        sTemp = sTemp.sprintf("%s Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSubCycle = Step.iSubCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    int iMaxRow;
    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
                  if(Step.iSubCycle != PreStep.iSubCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iSubCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRK_2Idx,ccBwd);

                  Step.iSubCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWRK_2Idx,ccBwd)) return false ;
                  MoveMotr(miWRK_2Idx,piWRK_2IdxWait);
                  Step.iSubCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxWait)) return false ;
                  MoveActr(aiWRK_2Idx , ccFwd);
                  Step.iSubCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiWRK_2Idx , ccFwd)) return false ;
                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);

                  Step.iSubCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;
//                  MoveMotr(miWRK_2Idx,piWRK_2IdxReWork);
                  MT_GoAbsRun(miWRK_2Idx, GetMotrPos(miWRK_2Idx,piWRK_2IdxReWork) - 5 );
//                  MoveMotr(miWRK_1Idx,piWRK_1IdxWorkStart);

                  Step.iSubCycle++;
                  return false ;

        case 15 : if(!MT_GetStop(miWRK_2Idx)) return false ;
                  MT_GoAbs(miWRK_2Idx, GetMotrPos(miWRK_2Idx,piWRK_2IdxReWork),10 );
                  Step.iSubCycle++;
                  return false ;

        case 16 : if(!MoveMotr(miWRK_2Idx,piWRK_2IdxReWork)) return false ;

//        case  15: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxReWork)) return false ;

                  if(!BinFullCheck()) return false ; //안에서 그냥 에러 띠움...

                  MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt);
                  Step.iSubCycle++ ;
                  return false ;

        case  17: if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWorkStt)) return false ;
                  BinInfoAdd(riWRL);
                  iMaxRow = DM.ARAY[riWRL].GetMaxRow();
                  c = DM.ARAY[riWRL].FindLastCol(csUnkwn);
                  for(int j =0 ; j < OM.DevOptn.iTrimColCnt ; j++ ) {
                      for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRL].SetStat(i , c - j, csEmpty); //Led 따먹음.dd
                  }

//                  LotInfoAdd();

                  MoveMotr(miWRK_ZPch,piWRK_ZPchWait);
                  Step.iSubCycle++ ;
                  return false ;

        case  18: if(!MoveMotr(miWRK_ZPch,piWRK_ZPchWait)) return false ;
                  DM.ARAY[riWRL].SetStep(asWRL_2IdxOut);
                  Step.iSubCycle = 0 ;
                  return true ;
    }
}

bool CWork::Cycle2IdxOut(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmSubCycle.OnDelay(Step.iSubCycle == PreStep.iSubCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iSubCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iSubCycle = 0 ;
        return true;
    }

    if(Step.iSubCycle != PreStep.iSubCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iSubCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSubCycle = Step.iSubCycle ;

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iSubCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iSubCycle != PreStep.iSubCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iSubCycle = 0 ;
                  return true ;

        case  10:
                  MoveActr(aiWRK_2Idx,ccFwd);
                  Step.iSubCycle ++;
                  return false ;

        case  11: if(!MoveActr(aiWRK_2Idx,ccFwd)) return false ;
                  MoveMotr(miWRK_2Idx,piWRK_2IdxPull);
                  DM.ARAY[riWRL].SetStep(asWRL_2IdxPull);
                  Step.iSubCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxPull)) return false ;
                  MoveActr(aiWRK_2Idx,ccBwd);
                  Step.iSubCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiWRK_2Idx,ccBwd)) return false ;
                  MoveMotr(miWRK_2Idx,piWRK_2IdxBack);
                  Step.iSubCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxBack)) return false ;
                  MoveActr(aiWRK_2Idx,ccFwd);
                  Step.iSubCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiWRK_2Idx,ccFwd)) return false ;
                  MoveMotr(miWRK_2Idx,piWRK_2IdxOut);
                  DM.ARAY[riWRL].SetStat(csNone); //Data Masking. dd
//                  DM.ARAY[riWRL].SetStep(asWRK_2IdxOut);
                  LotInfoAdd(); //랏 오픈이랑 안맞는데...랏오픈을 바꾸던가 해서 여기다가 넣어보자.dd

                  Step.iSubCycle++;
                  return false ;

        case  16: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxOut)) return false ;
                  MoveActr(aiWRK_2Idx,ccBwd);
                  Step.iSubCycle++;
                  return false ;

        case  17: if(!MoveActr(aiWRK_2Idx,ccBwd)) return false ;
                  MoveMotr(miWRK_2Idx,piWRK_2IdxWait);
                  Step.iSubCycle++;
                  return false ;

        case  18: if(!MoveMotr(miWRK_2Idx,piWRK_2IdxWait)) return false ;
                  if(IO_GetX(xPSB_Strip3)) {
                      EM_SetErr(eiWRK_OutSensorDetected);
                      return false ;
                  }

                  Step.iSubCycle = 0 ;
                  return true ;
    }
}
bool CWork::BinFullCheck()
{
    bool AllBinFull = ( IO_GetX(xWRK_Bin1Full) && IO_GetX(xWRK_Bin2Full) && IO_GetX(xWRK_Bin3Full) &&
                        IO_GetX(xWRK_Bin4Full) && IO_GetX(xWRK_Bin5Full) ) ;

    if(AllBinFull) return false ;

    bool bRet = true ;

    switch(m_iBinCnt)
    {
        case 1 : if(IO_GetX(xWRK_Bin1Full) || Bin1.bBinFull  ) {EM_SetErr(eiWRK_RejectBin1Full);  bRet = false ;} break ;
        case 2 : if(IO_GetX(xWRK_Bin2Full) || Bin2.bBinFull  ) {EM_SetErr(eiWRK_RejectBin2Full);  bRet = false ;} break ;
        case 3 : if(IO_GetX(xWRK_Bin3Full) || Bin3.bBinFull  ) {EM_SetErr(eiWRK_RejectBin3Full);  bRet = false ;} break ;
        case 4 : if(IO_GetX(xWRK_Bin4Full) || Bin4.bBinFull  ) {EM_SetErr(eiWRK_RejectBin4Full);  bRet = false ;} break ;
        case 5 : if(IO_GetX(xWRK_Bin5Full) || Bin5.bBinFull  ) {EM_SetErr(eiWRK_RejectBin5Full);  bRet = false ;} break ;
        default : bRet = false ; break ;
    }

    return bRet ;
}
bool CWork::BinInfoAdd(EN_ARAY_ID _riId )
{
//    Bin1.iBinCnt += OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt) ;
//    Bin2.iBinCnt += OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt) ;
//    Bin3.iBinCnt += OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt) ;
//    Bin4.iBinCnt += OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt) ;
//    Bin5.iBinCnt += OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt) ;
    int iLastTrimCnt ;

    if(DM.ARAY[_riId].GetCntStat(csUnkwn))
    {
        switch(m_iBinCnt)
        {
            case 1  : Bin1.iBinCnt += OM.DevInfo.iColCnt * OM.DevOptn.iTrimColCnt ; break ;
            case 2  : Bin2.iBinCnt += OM.DevInfo.iColCnt * OM.DevOptn.iTrimColCnt ; break ;
            case 3  : Bin3.iBinCnt += OM.DevInfo.iColCnt * OM.DevOptn.iTrimColCnt ; break ;
            case 4  : Bin4.iBinCnt += OM.DevInfo.iColCnt * OM.DevOptn.iTrimColCnt ; break ;
            case 5  : Bin5.iBinCnt += OM.DevInfo.iColCnt * OM.DevOptn.iTrimColCnt ; break ;
            default : break ;
        }
    }
    else
    {
        iLastTrimCnt = OM.DevInfo.iColCnt % OM.DevOptn.iTrimColCnt ;
        switch(m_iBinCnt)
        {
            case 1  : Bin1.iBinCnt += iLastTrimCnt ; break ;
            case 2  : Bin2.iBinCnt += iLastTrimCnt ; break ;
            case 3  : Bin3.iBinCnt += iLastTrimCnt ; break ;
            case 4  : Bin4.iBinCnt += iLastTrimCnt ; break ;
            case 5  : Bin5.iBinCnt += iLastTrimCnt ; break ;
            default : break ;
        }
    }
//TODO::    else TODO:: 아 집에 가야되서 못함.ㅠㅠ dd

    int iLotLedCnt = (OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt)) ;

    bool bBin1Full = Bin1.iBinCnt >= iLotLedCnt ;
    bool bBin2Full = Bin2.iBinCnt >= iLotLedCnt ;
    bool bBin3Full = Bin3.iBinCnt >= iLotLedCnt ;
    bool bBin4Full = Bin4.iBinCnt >= iLotLedCnt ;
    bool bBin5Full = Bin5.iBinCnt >= iLotLedCnt ;

    //Bin 통에 랏을 넣어서 그냥 랏 바뀌면 그걸로 풀 체크를 하는게 낳을듯........dd

    switch(m_iBinCnt)
    {
        case 1 : if(GetLastStrip() || bBin1Full ) { Bin1.bBinFull = true ; } break ;
        case 2 : if(GetLastStrip() || bBin2Full ) { Bin2.bBinFull = true ; } break ;
        case 3 : if(GetLastStrip() || bBin3Full ) { Bin3.bBinFull = true ; } break ;
        case 4 : if(GetLastStrip() || bBin4Full ) { Bin4.bBinFull = true ; } break ;
        case 5 : if(GetLastStrip() || bBin5Full ) { Bin5.bBinFull = true ; } break ;
        default : break ;
    }
}

bool CWork::LotInfoAdd  ()
{
    int iToolCnt ;
    int iChipCnt = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;

    LT.AddDayInfoWorkStrp(1);
    LT.AddLotInfoWorkStrp(1);
    OM.iTotalFrameCnt += 1 ;
    
    if(OM.MstOptn.bGoodChipAdd) {
        LT.AddDayInfoWorkChip(iGoodCnt );
        LT.AddLotInfoWorkChip(iGoodCnt );
        OM.iTotalChip += iGoodCnt ;
        LT.AddDayInfoFailChip(iChipCnt - iGoodCnt);
        LT.AddLotInfoFailChip(iChipCnt - iGoodCnt);
    }
    else {
        LT.AddDayInfoWorkChip(iChipCnt );
        LT.AddLotInfoWorkChip(iChipCnt );
        OM.iTotalChip += iChipCnt ;
    }
    iGoodCnt = 0 ;

    if(OM.DevInfo.iColCnt % OM.DevOptn.iTrimColCnt) iToolCnt = OM.DevInfo.iColCnt / OM.DevOptn.iTrimColCnt + 1 ;
    else iToolCnt = OM.DevInfo.iColCnt / OM.DevOptn.iTrimColCnt ;

    OM.iTotalToolCnt  += iToolCnt ;

    if(OM.iTotalChip < 0 || OM.iTotalFailChip < 0) {//OverFlow
        OM.iTotalChip     = 0 ;
        OM.iTotalFailChip = 0 ;
        OM.iTotalToolCnt  = 0 ;
    }

    SPC.WriteArayData(riWRK); //SPC 용인듯...
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
        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",DM.ARAY[riWRK].GetLotNo().c_str());
        LT.LotInfo.dEndTime = Now();
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        Trace("WriteLotLog",DM.ARAY[riWRK].GetLotNo().c_str());
        //LT.LotEnd();
    }

}
bool CWork::GetLastStrip()
{
//    m_sPreLotNo != DM.ARAY[riWRK].GetLotNo();

//    bool bPRBExist     = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
//    bool bWRKExist     = !DM.ARAY[riWRK].CheckAllStat(csNone) ;

//    bool bPRBDiff      = DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPRB].GetLotNo() ;
//    bool bWRKDiff  = DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPRB].GetLotNo() ;

    bool bPreEqpLotEnd = EC.GetPreEqpLotEnd() && DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riWRK].CheckAllStat(csNone) && DM.ARAY[riWRL].CheckAllStat(csNone) ;
    return bPreEqpLotEnd ;
    //너이러면 연속 작업이 안되...니혼자 힘으로 랏엔드를 띄울수 없니? dd
//    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

//         if(bWRKExist    ) return bPRBDiff ;
//    else if(bPRBExist    ) return bPRBDiff ;


//    if(bPreEqpLotEnd) return bPreEqpLotEnd ;
//    else if(!bLastMgz) return false    ;
//    else               return false     ;
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
    if(!AT_Done(aiWRK_1Idx   )) return false ;
    if(!AT_Done(aiWRK_2Idx   )) return false ;

    if(!MT_GetStop(miWRK_1Idx)) return false ;
    if(!MT_GetStop(miWRK_2Idx)) return false ;
    if(!MT_GetStop(miWRK_XRjt)) return false ;
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
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iBinCnt      " , m_iBinCnt        );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sLastStrip   " , m_sLastStrip     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_dFailChipPos " , m_dFailChipPos   );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo    " , m_sPreLotNo      );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin1.iBinCnt   " , Bin1.iBinCnt     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin2.iBinCnt   " , Bin2.iBinCnt     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin3.iBinCnt   " , Bin3.iBinCnt     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin4.iBinCnt   " , Bin4.iBinCnt     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin5.iBinCnt   " , Bin5.iBinCnt     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin1.bBinFull  " , Bin1.bBinFull    );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin2.bBinFull  " , Bin2.bBinFull    );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin3.bBinFull  " , Bin3.bBinFull    );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin4.bBinFull  " , Bin4.bBinFull    );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "Bin5.bBinFull  " , Bin5.bBinFull    );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded    " , m_bLotEnded      );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iGoodCnt       " , iGoodCnt         );

    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iBinCnt      " , m_iBinCnt        );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sLastStrip   " , m_sLastStrip     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_dFailChipPos " , m_dFailChipPos   );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo    " , m_sPreLotNo      );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin1.iBinCnt   " , Bin1.iBinCnt     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin2.iBinCnt   " , Bin2.iBinCnt     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin3.iBinCnt   " , Bin3.iBinCnt     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin4.iBinCnt   " , Bin4.iBinCnt     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin5.iBinCnt   " , Bin5.iBinCnt     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin1.bBinFull  " , Bin1.bBinFull    );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin2.bBinFull  " , Bin2.bBinFull    );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin3.bBinFull  " , Bin3.bBinFull    );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin4.bBinFull  " , Bin4.bBinFull    );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "Bin5.bBinFull  " , Bin5.bBinFull    );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded    " , m_bLotEnded      );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iGoodCnt       " , iGoodCnt         );
    }
}

