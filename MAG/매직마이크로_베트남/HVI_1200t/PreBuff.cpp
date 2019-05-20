//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UserIni.h"
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "Loader.h"
#include "Rail.h"
#include "Head.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
CPreBuff  PRB;

CPreBuff::CPreBuff(void)
{
    Init();
}

CPreBuff::~CPreBuff (void)
{
    //Close();
}

void CPreBuff::Init()
{
    m_sPartName = "Pre Buff " ;

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



bool CPreBuff::FindChip(int &r , int &c )
{
    return false;
}

double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;
    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    return bRet ;
}

//---------------------------------------------------------------------------
bool  CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;
    return bRet ;
}

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    return false;
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
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
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

    switch (Step.iHome) {
        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPRB_StprUpDn , ccFwd);
                 IO_SetY(yPRB_FeedingAC,false) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_StprUpDn , ccFwd))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPreBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (LDR.GetSeqStep()  == CLoader::scWork){
        Trace("scTWork","scBWork");
        Autorun();  //잠깐 넣어본다.  dd
    }

    if(Step.iSeq ) return false ;
//        LDR.GetSeqStep()  == CLoader::scTWork ||
//        LDR.GetSeqBStep() == CLoader::scBWork) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    return true ;
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

    static bool bWK1_FlpCmpBw = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: if(IO_GetX(xPRB_Pkg) && DM.ARAY[riPRB].CheckAllStat(csNone)){
                     DM.ARAY[riPRB].SetStat(csUnkwn);
                     DM.ARAY[riPRB].SetLotNo(m_sWorkedLotNo) ;

                     DM.ARAY[riPRB].SetID   (AnsiString(m_sWorkedId.ToIntDef(0)/100 * 100 +99))  ;
                     sTemp = DM.ARAY[riPRB].GetLotNo();
                     Trace("PRB Strip Add Lot" , sTemp.c_str());
                     sTemp = DM.ARAY[riPRB].GetID()   ;
                     Trace("PRB Strip Add ID" , sTemp.c_str());
                 }

                 IO_SetY(yPRB_AirBlower,false) ;
                 IO_SetY(yPRB_FeedingAC,false) ;

                 MoveActr(aiPRB_StprUpDn , ccFwd);

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPRB_StprUpDn , ccFwd)) return false ;
                 Step.iToStart = 0;
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
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yPRB_AirBlower,false) ;
                 IO_SetY(yPRB_FeedingAC,false) ;
                 Step.iToStop ++ ;
                 return false ;

        case 11:
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPreBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && (LDR.GetSeqStep()!= CLoader::scWork)) return false ;

        //2012.05.22변경전.
        //bool isStrpExst  = DM.ARAY[riPRB].GetCntExist() ;
        //bool isDoingInsp = DM.ARAY[riVS1].GetCntStat(csUnkwn) ;
        //bool isCycleOut  = DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() && !isDoingInsp && AT_Complete(aiWK1_Lift , ccBwd) &&
        //                  !DM.ARAY[riWR1].GetCntExist() ;
        //bool isCycleIn   = (IO_GetX(xPRB_PkgIn1)|| (OM.MstOptn.bDryRun && AT_Complete(aiLDR_Pusher , ccFwd))) && !DM.ARAY[riPRB].GetCntExist()  ;
        //bool isConEnd    = !DM.ARAY[riPRB].GetCntExist() ;

        //2012.05.22변경후.
        bool isVS1End      = DM.ARAY[riWR1].GetStep()==asInspDone; //자제 들어오면 asInspNeed로 세팅 되고 검사끝나서 UnBind될때 asInspDone으로 세팅
        bool isVS2End      = DM.ARAY[riWR2].GetStep()==asInspDone;
        bool isVS3End      = DM.ARAY[riWR3].GetStep()==asInspDone;
        bool isExistWRK    = !DM.ARAY[riWR1].CheckAllStat(csNone) ||
                             !DM.ARAY[riWR2].CheckAllStat(csNone) ||
                             !DM.ARAY[riWR3].CheckAllStat(csNone) ;

        bool isFinVs       = isVS1End || isVS2End || isVS3End ;

        bool isStrpExst  = DM.ARAY[riPRB].GetCntExist() ;
        bool isDoingInsp = HED.GetSeqStep() == CHead::scInsp ; //
        bool isDoingIdle = RAL.GetSeqStep() == CRail::scIdle ; //
        bool isDoingMove = RAL.GetSeqStep() == CRail::scMove ;
        bool isCycleOut  = !DM.ARAY[riPRB].CheckAllStat(csNone) && DM.ARAY[riWR1].CheckAllStat(csNone) && !isDoingInsp && AT_Complete(aiWK1_SttnUpDn , ccBwd) && (isDoingIdle || isDoingMove) ;//&& (!isFinVs ||!isExistWRK);
        bool isCycleIn   = (IO_GetX(xPRB_PkgIn1)|| (OM.MstOptn.bDryRun && AT_Complete(aiLDR_PshrFwBw , ccFwd))) && !DM.ARAY[riPRB].GetCntExist() ;
        bool isConEnd    = !DM.ARAY[riPRB].GetCntExist() ;



        //모르는 스트립에러.
        if(!DM.ARAY[riPRB ].GetCntExist() &&  IO_GetX(xPRB_Pkg) &&
            AT_Complete(aiPRB_StprUpDn,ccFwd)) EM_SetErr(eiPRB_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if( DM.ARAY[riPRB ].GetCntExist() && !IO_GetX(xPRB_Pkg)&& !OM.MstOptn.bDryRun ) EM_SetErr(eiPRB_PkgDispr ) ;
        bool bLdrWork = LDR.GetSeqStep()  == CLoader::scWork ;

        if(EM_IsErr() && !bLdrWork) return false ;

        //Normal Decide Step.
             if (isCycleIn  ) {Trace(m_sPartName.c_str(),"CycleIn  Stt"); Step.iSeq = scIn  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
//        else if (isCycleOut ) {Trace(m_sPartName.c_str(),"CycleOut Stt"); Step.iSeq = scOut ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default     :           /*Trace(m_sPartName.c_str(),"default     End");*/Step.iSeq = scIdle ;  return false ;
        case  scIn  : if(CycleIn  ()){ Trace(m_sPartName.c_str(),"CycleIn  End");Step.iSeq = scIdle ;} return false ;
//        case  scOut : if(CycleOut ()){ Trace(m_sPartName.c_str(),"CycleOut End");Step.iSeq = scIdle ;} return false ;
    }
}

bool CPreBuff::CycleIn(void) //스트립을 PreBuff로 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
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

    bool r1 , r2 ;
    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPRB_StprUpDn  , ccFwd);
                  IO_SetY(yPRB_AirBlower,true); //열을 식혀 보장...
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_StprUpDn  , ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC , true) ; //ana

                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xPRB_Pkg)&& !OM.MstOptn.bDryRun) return false ;

                  Step.iCycle++;
                  return false;

        case 13 : IO_SetY(yPRB_FeedingAC , false);//ana
                  DM.ARAY[riPRB].SetStat(csUnkwn) ;
                  IO_SetY(yPRB_AirBlower,false); //열을 식혀 보장...
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPreBuff::CycleOut(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
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

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 :
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 11 : if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPRBStprDnDelay)) return false ;
                  MoveActr(aiPRB_StprUpDn,ccBwd);
                  IO_SetY(yPRB_FeedingAC , true );
                  //if(RAL.GetSeqStep() != CRail::scMove) MoveActr(aiWK1_Stopper,ccFwd); // dd 개같애 너 여기 있으면 안될거 같다...레일 이동대기dd
                  //if(!IO_GetY(yPRB_FeedingAC)) { IO_SetY(yPRB_FeedingAC , true ); Trace("yPRB_FeedingAC","true"); }
                  //if(!IO_GetY(yWRK_FeedingAC)) { IO_SetY(yWRK_FeedingAC , true ); Trace("yWRK_FeedingAC","true"); }// dd
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveActr(aiPRB_StprUpDn,ccBwd)) return false ;
//                  if(!MoveActr(aiWK1_Stopper,ccFwd)) return false ;
//                  IO_SetY(yWRK_FeedingAC , true ); // dd
                  m_sWorkedLotNo = DM.ARAY[riPRB].GetLotNo() ;
                  m_sWorkedId    = DM.ARAY[riPRB].GetID   () ;
                  DM.ShiftArrayData(riPRB , riWR1);
                  DM.ARAY[riWR1].SetStep(asInspNeed);
                  RAL.riWR1Buff = DM.ARAY[riWR1] ;
                  Step.iCycle++;
                  return false ;

        case 13 : if(IO_GetX(xPRB_Pkg) && !OM.MstOptn.bDryRun) return false ;
                  MoveActr(aiPRB_StprUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveActr(aiPRB_StprUpDn , ccFwd)) return false ;
                  IO_SetY(yPRB_FeedingAC , false );
                  //if(!IO_GetX(xWR1_Pkg) && !OM.MstOptn.bDryRun) return false ; // dd
                  Step.iCycle = 0;
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
    if(!AT_Done(aiPRB_StprUpDn )) return false ;
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
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }

    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , m_sWorkedLotNo);
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }
}

bool CPreBuff::GetLastStrip()
{
    AnsiString sLastLotNo = DM.ARAY[riPRB].GetCntExist() ? DM.ARAY[riPRB].GetLotNo() : m_sWorkedLotNo ;//여기부터

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if( bLDRExist) return bLDRDiff ;
    else if(!bLastMgz ) return false    ;
    else                return true     ;
}

