//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rail.h"
#include "PreBuff.h"
#include "Head.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "Loader.h"
#include "PstBuff.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CRail RAL;

CRail::CRail(void)
{
    Init();

}
CRail::~CRail (void)
{
    //Close();
}

void CRail::Init()
{
    m_sPartName = "Rail " ;
    m_lStrpCycleTime = 0 ;
    m_lPreCycleTime  = 0 ;
    bTrmOneSkip = false  ;

    Reset();
    Load(true);


    AnsiString BinPath ;
    AnsiString IniPath ;

    BinPath = EXE_FOLDER + "SeqData\\DataMapWR1Buffer.dat" ;
    IniPath = EXE_FOLDER + "SeqData\\DataMapWR1Buffer.ini" ;
    riWR1Buff.Load(true,BinPath , IniPath , 1);

    BinPath = EXE_FOLDER + "SeqData\\DataMapWR2Buffer.dat" ;
    IniPath = EXE_FOLDER + "SeqData\\DataMapWR2Buffer.ini" ;
    riWR2Buff.Load(true,BinPath , IniPath , 2);

    BinPath = EXE_FOLDER + "SeqData\\DataMapWR3Buffer.dat" ;
    IniPath = EXE_FOLDER + "SeqData\\DataMapWR3Buffer.ini" ;
    riWR3Buff.Load(true,BinPath , IniPath , 3);
}

void CRail::Close()
{
    AnsiString BinPath ;
    AnsiString IniPath ;

    BinPath = EXE_FOLDER + "SeqData\\DataMapWR1Buffer.dat" ;
    IniPath = EXE_FOLDER + "SeqData\\DataMapWR1Buffer.ini" ;
    riWR1Buff.Load(false,BinPath , IniPath , 1);

    BinPath = EXE_FOLDER + "SeqData\\DataMapWR2Buffer.dat" ;
    IniPath = EXE_FOLDER + "SeqData\\DataMapWR2Buffer.ini" ;
    riWR2Buff.Load(false,BinPath , IniPath , 2);

    BinPath = EXE_FOLDER + "SeqData\\DataMapWR3Buffer.dat" ;
    IniPath = EXE_FOLDER + "SeqData\\DataMapWR3Buffer.ini" ;
    riWR3Buff.Load(false,BinPath , IniPath , 3);



    Load(false);
}

void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}



bool CRail::FindChip(int &r , int &c )
{
    return false;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isWK1Exist   = DM.ARAY[riWR1].GetCntExist();

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return bRet ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    return bRet ;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRail::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_HomeTO);
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
        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd);
                 AT_MoveCyl(aiWK1_SttnUpDn , ccBwd);
                 AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd);
                 AT_MoveCyl(aiWK2_SttnUpDn , ccBwd);
                 AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd);
                 AT_MoveCyl(aiWK3_SttnUpDn , ccBwd);
                 AT_MoveCyl(aiWK1_StprUpDn , ccFwd); //이상갑 차장님 요청사항.dd Fwd Stpr
                 AT_MoveCyl(aiWK2_StprUpDn , ccFwd);
                 AT_MoveCyl(aiWK3_StprUpDn , ccFwd);

                 IO_SetY(yWRK_FeedingAC,false) ;
                 IO_SetY(yPRB_AirBlower,false) ;

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWK1_AlgnFwBw , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_SttnUpDn , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_AlgnFwBw , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_SttnUpDn , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_AlgnFwBw , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_SttnUpDn , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWK1_StprUpDn , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK2_StprUpDn , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiWK3_StprUpDn , ccFwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq ) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
}

bool CRail::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bWK1_FlpCmpBw = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: if(IO_GetX(xPSB_PkgIn2)) {
                     EM_SetErr(eiRAL_4Pkg);
                     return false ;
                 }
                 IO_SetY(yWRK_FeedingAC,false) ;

                 MoveActr(aiWK1_StprUpDn , ccFwd);
                 MoveActr(aiWK2_StprUpDn , ccFwd);
                 MoveActr(aiWK3_StprUpDn , ccFwd);

                 MoveActr(aiWK1_AlgnFwBw  , ccBwd);
                 MoveActr(aiWK2_AlgnFwBw  , ccBwd);
                 MoveActr(aiWK3_AlgnFwBw  , ccBwd);

                 MoveActr(aiWK1_SttnUpDn   , ccBwd);
                 MoveActr(aiWK2_SttnUpDn   , ccBwd);
                 MoveActr(aiWK3_SttnUpDn   , ccBwd);

                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiWK1_StprUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiWK2_StprUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiWK3_StprUpDn , ccFwd)) return false ;

                 if(!MoveActr(aiWK1_AlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiWK2_AlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiWK3_AlgnFwBw , ccBwd)) return false ;

                 if(!MoveActr(aiWK1_SttnUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiWK2_SttnUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiWK3_SttnUpDn , ccBwd)) return false ;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CRail::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiRAL_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yWRK_FeedingAC,false) ;

                 MoveActr(aiWK1_AlgnFwBw , ccBwd);
                 MoveActr(aiWK2_AlgnFwBw , ccBwd);
                 MoveActr(aiWK3_AlgnFwBw , ccBwd);

                 MT_GoAbsRun(miWRK_YVsn,0);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiWK1_AlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiWK2_AlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiWK3_AlgnFwBw , ccBwd)) return false ;

                 if(!MT_GoAbsRun(miWRK_YVsn,0)) return false ;

                 MoveActr(aiWK1_SttnUpDn , ccBwd);
                 MoveActr(aiWK2_SttnUpDn , ccBwd);
                 MoveActr(aiWK3_SttnUpDn , ccBwd);

                 MoveActr(aiPRB_StprUpDn , ccFwd);
                 MoveActr(aiWK1_StprUpDn , ccFwd);
                 MoveActr(aiWK2_StprUpDn , ccFwd);
                 MoveActr(aiWK3_StprUpDn , ccFwd);

                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveActr(aiWK1_SttnUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiWK2_SttnUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiWK3_SttnUpDn , ccBwd)) return false ;

                 if(!MoveActr(aiPRB_StprUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiWK1_StprUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiWK2_StprUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiWK3_StprUpDn , ccFwd)) return false ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && (LDR.GetSeqStep()  != CLoader::scWork)) return false ;

        //비젼 검사 끝난거.
        bool isVS1End      = DM.ARAY[riWR1].GetStep()==asInspDone; //자제 들어오면 asInspNeed로 세팅 되고 검사끝나서 UnBind될때 asInspDone으로 세팅.
        bool isVS2End      = DM.ARAY[riWR2].GetStep()==asInspDone;
        bool isVS3End      = DM.ARAY[riWR3].GetStep()==asInspDone;

        bool isPSBReady    = DM.ARAY[riPSB].CheckAllStat(csNone) &&
                             MT_CmprPos(miPSB_TFlp , PM.GetValue(miPSB_TFlp , pvPSB_TFlpWait)) && // 이거 파트별로 만들려고 한거일수도 있으나 dd
                             AT_Complete(aiPSB_AlgnFwBw , ccBwd) &&
                             AT_Complete(aiPSB_IndxUpDn , ccBwd) &&
                             AT_Complete(aiPSB_FlprOpCl , ccBwd);

        bool isCanMovePRB  = !DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riWR1].CheckAllStat(csNone) ;

//        bool isDoingOut    = CPreBuff::scOut == PRB.GetSeqStep() ;

        bool isWaitWR1     =( DM.ARAY[riWR2].GetCntStat(csUnkwn)|| DM.ARAY[riWR3].GetCntStat(csUnkwn)) &&
                             (DM.ARAY[riLDR].GetCntExist()      || DM.ARAY[riPRB].GetCntExist())       &&
                              DM.ARAY[riWR1].CheckAllStat(csNone) && !IO_GetX(xWR1_Pkg) && !OM.CmnOptn.bLoadingStop ;

        bool isDoingInsp   =  DM.ARAY[riVS1].GetCntExist() || DM.ARAY[riVS2].GetCntExist() || DM.ARAY[riVS3].GetCntExist();

        bool isPrbOut      =  PRB.GetSeqStep() == CPreBuff::scOut ;//DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() ;

        bool isCycleBind   = !isWaitWR1 && //!isDoingInsp &&                                                                //리프트 블럭 없으면 센서 감지 됨.dd
                             ((DM.ARAY[riWR1].GetCntExist()&& IO_GetX(xWR1_Pkg) && DM.ARAY[riWR1].GetStep()==asInspNeed && !DM.ARAY[riVS1].CheckAllStat(csWork)) ||
                              (DM.ARAY[riWR2].GetCntExist()&& IO_GetX(xWR2_Pkg) && DM.ARAY[riWR2].GetStep()==asInspNeed && !DM.ARAY[riVS2].CheckAllStat(csWork)) ||
                              (DM.ARAY[riWR3].GetCntExist()&& IO_GetX(xWR3_Pkg) && DM.ARAY[riWR3].GetStep()==asInspNeed && !DM.ARAY[riVS3].CheckAllStat(csWork)) ) &&
                               !isPrbOut;

                              //Bind시에 asInspNeed이거 보고 올리고
                              //바인드 끝나고 csUnkown으로 바뀌고
                              //다 csWork로 바뀌면 언바인드 타고.
                              //UnBind마지막에 DM.ARAY[riWR3].GetStep()==asInspDone 됌.


        bool isExistStrip  = DM.ARAY[riPRB].GetCntExist() ||
                             DM.ARAY[riWR1].GetCntExist() ||
                             DM.ARAY[riWR2].GetCntExist() ||
                             DM.ARAY[riWR3].GetCntExist() ;

//        bool isCycleUnBind =  DM.ARAY[riVS1].CheckAllStat(csWork) && DM.ARAY[riVS2].CheckAllStat(csWork) && DM.ARAY[riVS3].CheckAllStat(csWork) ;
        bool isCycleUnBind = ((!DM.ARAY[riWR1].GetCntExist() || DM.ARAY[riVS1].CheckAllStat(csWork)) &&
                              (!DM.ARAY[riWR2].GetCntExist() || DM.ARAY[riVS2].CheckAllStat(csWork)) &&
                              (!DM.ARAY[riWR3].GetCntExist() || DM.ARAY[riVS3].CheckAllStat(csWork)) ) && isExistStrip && HED.GetSeqStep() != CHead::scInsp;

        bool isCycleMove   = ( isCanMovePRB && PRB.GetSeqStep() != CPreBuff::scIn && HED.GetSeqStep() != CHead::scInsp) || //PRB
                             ((isVS1End || DM.ARAY[riWR1].CheckAllStat(csNone)) &&
                              (isVS2End || DM.ARAY[riWR2].CheckAllStat(csNone)) &&
                              (isVS3End || DM.ARAY[riWR3].CheckAllStat(csNone)) &&
                               isExistStrip && // isExistStrip 이거좀 이상한거 같기도 하다 생각부족.dd
                              (isPSBReady || DM.ARAY[riWR3].CheckAllStat(csNone)) ) ;


//                             PRB.GetSeqStep() == CPreBuff::scOut ;// Rail 에서 프리버퍼 있을때만으로 무브 사이클 태우기는에는 안될때가 있음.dd
                             //!isPrbOut ; //프리버퍼좀 기다려봐.dd

        bool isConEnd      = !DM.ARAY[riWR1].GetCntExist() &&
                             !DM.ARAY[riWR2].GetCntExist() && !DM.ARAY[riWR3].GetCntExist() ;

        if(isCycleMove) {
            unsigned long lCrntTime = GetTickCount() ;
            m_lStrpCycleTime = lCrntTime - m_lPreCycleTime ;
            m_lPreCycleTime  = lCrntTime ;
        }

        //모르는 스트립에러.
        if(!DM.ARAY[riWR1 ].CheckExist(0,0) &&  IO_GetX(xWR1_Pkg)) EM_SetErr(eiWR1_PkgUnknown ) ;
        if(!DM.ARAY[riWR2 ].CheckExist(0,0) &&  IO_GetX(xWR2_Pkg)) EM_SetErr(eiWR2_PkgUnknown ) ;
        if(!DM.ARAY[riWR3 ].CheckExist(0,0) &&  IO_GetX(xWR3_Pkg)) EM_SetErr(eiWR3_PkgUnknown ) ;

        //스트립 사라짐 에러.
        //스테이지 올리면 센서가 감지 않되어 뺀다.
        if( DM.ARAY[riWR1 ].CheckExist(0,0) && !IO_GetX(xWR1_Pkg) && AT_Complete(aiWK1_SttnUpDn , ccBwd) && !OM.MstOptn.bDryRun && !isPrbOut  ) EM_SetErr(eiWR1_PkgDispr ) ;
        if( DM.ARAY[riWR2 ].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_SttnUpDn , ccBwd) && !OM.MstOptn.bDryRun               ) EM_SetErr(eiWR2_PkgDispr ) ;
        if( DM.ARAY[riWR3 ].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_SttnUpDn , ccBwd) && !OM.MstOptn.bDryRun               ) EM_SetErr(eiWR3_PkgDispr ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleMove  ) {Trace(m_sPartName.c_str(),"CycleMove   Stt"); Step.iSeq = scMove   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleUnBind) {Trace(m_sPartName.c_str(),"CycleUnBind Stt"); Step.iSeq = scUnBind ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBind  ) {Trace(m_sPartName.c_str(),"CycleBind   Stt"); Step.iSeq = scBind   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                 /*Trace(m_sPartName.c_str(),"default     End");*/Step.iSeq = scIdle ;  return false ;
        case  scMove   : if(CycleMove   ()){ Trace(m_sPartName.c_str(),"CycleMove   End");Step.iSeq = scIdle ;} return false ;
        case  scUnBind : if(CycleUnBind ()){ Trace(m_sPartName.c_str(),"CycleUnBind End");Step.iSeq = scIdle ;} return false ;
        case  scBind   : if(CycleBind   ()){ Trace(m_sPartName.c_str(),"CycleBind   End");Step.iSeq = scIdle ;} return false ;
    }
}

bool CRail::CycleMove(void) //자동 계산  이동.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
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

    static bool bCanMvPRB = false ;
    static bool bCanMvWK1 = false ;
    static bool bCanMvWK2 = false ;
    static bool bCanMvWK3 = false ;


    static bool bPSBPassIn  = false ;
    static bool bPSBPassOut = false ;


    int r1 , r2 , r3 , r4 ;
    bool bTmTemp ;

    int iRanR ;
    int iRanC ;
    int iRanS ;


    if(IO_GetXDn(xPSB_PkgIn2)) bPSBPassIn = true ;
    if(bPSBPassIn) if(!IO_GetX(xPSB_PkgIn2)) bPSBPassOut = true ;
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bPSBPassIn  = false ;
                  bPSBPassOut = false ;
//                  IO_SetY(yWRK_FeedingAC , true);
                  bCanMvPRB = DM.ARAY[riPRB].GetCntExist();
                  bCanMvWK3 = DM.ARAY[riWR3].GetCntExist()&& DM.ARAY[riWR3].GetStep()==asInspDone /*DM.ARAY[riVS3].GetCntExist()*/ &&  DM.ARAY[riPSB].CheckAllStat(csNone) ;
                  bCanMvWK2 = DM.ARAY[riWR2].GetCntExist()&& DM.ARAY[riWR2].GetStep()==asInspDone /*DM.ARAY[riVS2].GetCntExist()*/ && (DM.ARAY[riWR3].CheckAllStat(csNone)||bCanMvWK3) ;
                  bCanMvWK1 = DM.ARAY[riWR1].GetCntExist()&& DM.ARAY[riWR1].GetStep()==asInspDone /*DM.ARAY[riVS1].GetCntExist()*/ && (DM.ARAY[riWR2].CheckAllStat(csNone)||bCanMvWK2) ;

                  MoveActr(aiWK1_SttnUpDn , ccBwd);
                  MoveActr(aiWK2_SttnUpDn , ccBwd);
                  MoveActr(aiWK3_SttnUpDn , ccBwd);

                  MoveActr(aiWK1_AlgnFwBw , ccBwd);
                  MoveActr(aiWK2_AlgnFwBw , ccBwd);
                  MoveActr(aiWK3_AlgnFwBw , ccBwd);

                  MoveActr(aiWK1_StprUpDn , ccFwd);
                  MoveActr(aiWK2_StprUpDn , ccFwd);
                  MoveActr(aiWK3_StprUpDn , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWK1_SttnUpDn , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_SttnUpDn , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_SttnUpDn , ccBwd)) return false ;

                  if(!MoveActr(aiWK1_AlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_AlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_AlgnFwBw , ccBwd)) return false ;

                  if(!MoveActr(aiWK1_StprUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_StprUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_StprUpDn , ccFwd)) return false ;

                  IO_SetY(yWRK_FeedingAC , true);                  

                  Step.iCycle++ ;
                  return false ;

        case  12: if(bCanMvWK3){
                      MoveActr(aiWK3_StprUpDn , ccBwd);

                      m_sWorkedLotNo = DM.ARAY[riWR3].GetLotNo();
                      m_sWorkedId    = DM.ARAY[riWR3].GetID   ();


                      //TODO :: 정말 마스터 옵션임.......................
                      //지워줘 나중에...........dd
                      if(OM.MstOptn.bUseRamVsData && DM.ARAY[riWR1].GetCntExist())
                      {
                          for(int i=0;i<10;i++){
                              iRanR = rand()%24 ;
                              iRanC = rand()%16 ;
                              iRanS = 4         ;//rand()%12 ;
                              DM.ARAY[riWR3].SetStat(iRanR,iRanC,(EN_CHIP_STAT)iRanS);
                          }

                      }
                      //지워줘 나중에...........dd

                      DM.ShiftArrayData(riWR3 , riPSB);
                      DM.ARAY[riLot] = DM.ARAY[riPSB] ;
                      DM.ARAY[riBuf] = DM.ARAY[riPSB] ;

                      //Option Add
                      if(OM.CmnOptn.bTrm1Skip) {//마킹 하는 자재를 스킵 하거나 없으면 지금 자재를 스킵.
                          DM.ARAY[riPSB].SetStat(csWork);
                          OM.CmnOptn.bTrm1Skip = false ;
                          bTrmOneSkip          = true  ; //체크박스 풀기용.Operation 폼
                      }

                      else if(OM.CmnOptn.bTrmAllSkip ) DM.ARAY[riPSB].SetStat(csWork);


                      //DM.ARAY[riPSB].FlipY(); //미리 뒤집어 놔야 오류가 적다. 뒤집기 전에 Review검사 해야 해서 포스트버퍼에서 한다.
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  13: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iWK2StprDnDelay)) return false ;
                  if(bCanMvWK2){
                      MoveActr(aiWK2_StprUpDn , ccBwd);
                      DM.ShiftArrayData(riWR2 , riWR3);
                      DM.ARAY[riWR3].ChangeStat(csWork , csUnkwn);
                      DM.ARAY[riWR3].SetStep(asInspNeed);
                      riWR3Buff = DM.ARAY[riWR3] ;

                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iWK1StprDnDelay)) return false ;
                  if(bCanMvWK1){
                      MoveActr(aiWK1_StprUpDn , ccBwd);
                      DM.ShiftArrayData(riWR1 , riWR2);
                      DM.ARAY[riWR2].ChangeStat(csWork , csUnkwn);
                      DM.ARAY[riWR2].SetStep(asInspNeed);
                      riWR2Buff = DM.ARAY[riWR2] ;
                  }
                  Step.iCycle++ ;
                  return false ;

        case  15: if(bCanMvPRB){
                      IO_SetY(yPRB_FeedingAC , true );
                      MoveActr(aiPRB_StprUpDn,ccBwd);
                      PRB.m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                      PRB.m_sWorkedId    = DM.ARAY[riPRB].GetID   () ;
                      DM.ShiftArrayData(riPRB , riWR1);
                      DM.ARAY[riWR1].SetStep(asInspNeed);
                      riWR1Buff = DM.ARAY[riWR1] ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!IO_GetX(xWR3_Pkg)) MoveActr(aiWK3_StprUpDn , ccFwd);
                  if(!IO_GetX(xWR2_Pkg)) MoveActr(aiWK2_StprUpDn , ccFwd);
                  if(!IO_GetX(xWR1_Pkg)) MoveActr(aiWK1_StprUpDn , ccFwd);
                  if(!IO_GetX(xPRB_Pkg)) MoveActr(aiPRB_StprUpDn , ccFwd);

                  if(AT_GetCmd(aiPRB_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK1_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK2_StprUpDn) != ccFwd) return false ;
                  if(AT_GetCmd(aiWK3_StprUpDn) != ccFwd) return false ;

//                  bTmTemp = m_tmTemp.OnDelay(true,8000) ;

                  if(!bPSBPassIn  && bCanMvWK3 ) { return false ; }
//                  if(bTmTemp) {
//                      EM_SetErr(eiRAL_4Pkg);
//                      IO_SetY(yWRK_FeedingAC , false);
//                      Step.iCycle++;
//                      return false ;
//                  }
                  if(!bPSBPassOut && bCanMvWK3 ) { return false ; }
//                  if(bTmTemp) {
//                      EM_SetErr(eiRAL_4Pkg);
//                      IO_SetY(yWRK_FeedingAC , false);
//                      Step.iCycle++;
//                      return false ;
//                  }
//                  if(bCanMvWK3 &&
                  if(bCanMvWK2 && !IO_GetX(xWR3_Pkg) ) { return false ; }
//                  if(bTmTemp) {
//                      EM_SetErr(eiRAL_3Pkg);
//                      IO_SetY(yWRK_FeedingAC , false);
//                      Step.iCycle++;
//                      return false ;
//                  }
                  if(bCanMvWK1 && !IO_GetX(xWR2_Pkg) ) { return false ; }
//                  if(bTmTemp) {
//                      EM_SetErr(eiRAL_2Pkg);
//                      IO_SetY(yWRK_FeedingAC , false);
//                      Step.iCycle++;
//                      return false ;
//                  }
                  if(bCanMvPRB && !IO_GetX(xWR1_Pkg) ) { return false ; }
//                  if(bTmTemp) {
//                      EM_SetErr(eiRAL_1Pkg);
//                      IO_SetY(yWRK_FeedingAC , false);
//                      Step.iCycle++;
//                      return false ;
//                  }
                  IO_SetY(yWRK_FeedingAC , false);
//                  IO_SetY(yPRB_FeedingAC , false);
                  Step.iCycle++;
                  return false ;


        case  17: if(bCanMvWK3){ //Fail Count Error Set.
                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
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

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWK1_StprUpDn , ccFwd);
                  MoveActr(aiWK2_StprUpDn , ccFwd);
                  MoveActr(aiWK3_StprUpDn , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWK1_StprUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_StprUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_StprUpDn , ccFwd)) return false ;
                  IO_SetY(yWRK_FeedingAC , true);
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;

                  MoveActr(aiWK1_AlgnFwBw , ccFwd);
                  MoveActr(aiWK2_AlgnFwBw , ccFwd);
                  MoveActr(aiWK3_AlgnFwBw , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiWK1_AlgnFwBw , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_AlgnFwBw , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_AlgnFwBw , ccFwd)) return false ;

                  MoveActr(aiWK1_SttnUpDn , ccFwd);
                  MoveActr(aiWK2_SttnUpDn , ccFwd);
                  MoveActr(aiWK3_SttnUpDn , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiWK1_SttnUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_SttnUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_SttnUpDn , ccFwd)) return false ;
                  IO_SetY(yWRK_FeedingAC , false);

                  MoveActr(aiWK1_AlgnFwBw , ccBwd);
                  MoveActr(aiWK2_AlgnFwBw , ccBwd);
                  MoveActr(aiWK3_AlgnFwBw , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiWK1_AlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_AlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_AlgnFwBw , ccBwd)) return false ;

                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.                  DM.ARAY[riWK1].GetCntExist()
                  if(DM.ARAY[riWR1].GetCntExist()){DM.ARAY[riVS1].SetStat(csUnkwn); DM.ARAY[riWR1] = riWR1Buff ;}
                  if(DM.ARAY[riWR2].GetCntExist()){DM.ARAY[riVS2].SetStat(csUnkwn); DM.ARAY[riWR2] = riWR2Buff ;}
                  if(DM.ARAY[riWR3].GetCntExist()){DM.ARAY[riVS3].SetStat(csUnkwn); DM.ARAY[riWR3] = riWR3Buff ;}

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleUnBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRAL_CycleTO);
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

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWK1_StprUpDn , ccFwd);
                  MoveActr(aiWK2_StprUpDn , ccFwd);
                  MoveActr(aiWK3_StprUpDn , ccFwd);

                  MoveActr(aiWK1_AlgnFwBw , ccBwd);
                  MoveActr(aiWK2_AlgnFwBw , ccBwd);
                  MoveActr(aiWK3_AlgnFwBw , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWK1_StprUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_StprUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiWK3_StprUpDn , ccFwd)) return false ;

                  if(!MoveActr(aiWK1_AlgnFwBw   , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_AlgnFwBw   , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_AlgnFwBw   , ccBwd)) return false ;

                  MoveActr(aiWK1_SttnUpDn , ccBwd);
                  MoveActr(aiWK2_SttnUpDn , ccBwd);
                  MoveActr(aiWK3_SttnUpDn , ccBwd);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveActr(aiWK1_SttnUpDn , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_SttnUpDn , ccBwd)) return false ;
                  if(!MoveActr(aiWK3_SttnUpDn , ccBwd)) return false ;

                  DM.ARAY[riVS1].SetStat(csNone);
                  DM.ARAY[riVS2].SetStat(csNone);
                  DM.ARAY[riVS3].SetStat(csNone);

                  DM.ARAY[riWR1].SetStep(asInspDone);
                  DM.ARAY[riWR2].SetStep(asInspDone);
                  DM.ARAY[riWR3].SetStep(asInspDone);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRail::CheckStop()
{
    if(!AT_Done(aiWK1_AlgnFwBw )) return false ;
    if(!AT_Done(aiWK1_SttnUpDn )) return false ;
    if(!AT_Done(aiWK1_StprUpDn )) return false ;
    if(!AT_Done(aiWK2_AlgnFwBw )) return false ;
    if(!AT_Done(aiWK2_SttnUpDn )) return false ;
    if(!AT_Done(aiWK2_StprUpDn )) return false ;
    if(!AT_Done(aiWK3_AlgnFwBw )) return false ;
    if(!AT_Done(aiWK3_SttnUpDn )) return false ;
    if(!AT_Done(aiWK3_StprUpDn )) return false ;

    return true ;
}

void CRail::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CRail","Load Start") ;

    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "bTrmOneSkip   " , bTrmOneSkip   );
    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "bTrmOneSkip   " , bTrmOneSkip   );
    }
    Trace( "CRail","Load End") ;
}

bool CRail::GetLastStrip()
{
    AnsiString sLastLotNo = DM.ARAY[riWR3].GetCntExist() ? DM.ARAY[riWR3].GetLotNo() : m_sWorkedLotNo ;

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
    bool bWR1Diff  = sLastLotNo != DM.ARAY[riWR1].GetLotNo() ;
    bool bWR2Diff  = sLastLotNo != DM.ARAY[riWR2].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if( bWR2Exist) return bWR2Diff ;
    else if( bWR1Exist) return bWR1Diff ;
    else if( bPRBExist) return bPRBDiff ;
    else if( bLDRExist) return bLDRDiff ;
    else if(!bLastMgz ) return false    ;
    else                return true     ;
}

