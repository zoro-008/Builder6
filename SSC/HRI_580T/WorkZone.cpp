//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "WorkZone.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "DataMan.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CWorkZone WRK;
const AnsiString sPartName = "Work Zone " ;

CWorkZone::CWorkZone(void)
{
    Reset();
}

void CWorkZone::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CWorkZone::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CWorkZone::~CWorkZone (void)
{

}

bool CWorkZone::FindChip(int &r , int &c )
{
    return false;
}

double CWorkZone::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    dPos = MT_GetCmdPos(_iMotr);

    return dPos ;
}

//---------------------------------------------------------------------------
bool CWorkZone::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool  CWorkZone::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //Ok.
    return true;
}

bool CWorkZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    return false ;
}

bool CWorkZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWorkZone::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiWRK_HomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    if(Stat.bReqStop) {
        Step.iHome = 0;
        return true ;
    }

    switch (Step.iHome) {
        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 Trace(sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MoveActr(aiWK1_Align   , ccBwd);
                 MoveActr(aiWK1_Lift    , ccBwd);
                 MoveActr(aiWK1_Stopper , ccFwd);

                 MoveActr(aiWK2_Align   , ccBwd);
                 MoveActr(aiWK2_Lift    , ccBwd);
                 MoveActr(aiWK2_Stopper , ccFwd);

                 IO_SetY(yWRK_Belt,false) ;

                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiWK1_Align   , ccBwd))return false ;
                 if(!MoveActr(aiWK1_Lift    , ccBwd))return false ;
                 if(!MoveActr(aiWK1_Stopper , ccFwd))return false ;

                 if(!MoveActr(aiWK2_Align   , ccBwd))return false ;
                 if(!MoveActr(aiWK2_Lift    , ccBwd))return false ;
                 if(!MoveActr(aiWK2_Stopper , ccFwd))return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CWorkZone::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CWorkZone::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CWorkZone::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiWRK_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: IO_SetY(yWRK_Belt,false);
                 Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWorkZone::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiWRK_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yWRK_Belt,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

bool CWorkZone::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleMoveAll =  DM.ARAY[riWK1].GetCntExist() &&  DM.ARAY[riWK2].GetCntExist() && !DM.ARAY[riPSB].GetCntExist() ; //WK1 -> WK2 & WK2 -> PSB
        bool isCycleMvToWK2 =  DM.ARAY[riWK1].GetCntExist() && !DM.ARAY[riWK2].GetCntExist() ;                                  //WK1 -> WK2
        bool isCycleOut     = !DM.ARAY[riPSB].GetCntExist() &&  AT_Complete(aiPRB_Stopper , ccFwd) ;                            //WK2 -> PSB
        bool isConEnd       = !OM.EqpOptn.bExistOnLoader    && !isExistStrpOnRail ;

        //모르는 스트립에러.
        if(!DM.ARAY[riWK1 ].CheckExist(0,0) &&  IO_GetX(xWK1_Pkg) ) EM_SetErr(eiWK1_PkgUnknown ) ;
        if(!DM.ARAY[riWK2 ].CheckExist(0,0) &&  IO_GetX(xWK2_Pkg) ) EM_SetErr(eiWK2_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if( DM.ARAY[riWK1 ].CheckExist(0,0) && !IO_GetX(xWK1_Pkg) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK1_PkgDispr   ) ;
        if( DM.ARAY[riWK2 ].CheckExist(0,0) && !IO_GetX(xWK2_Pkg) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK2_PkgDispr   ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleOut       ) {Trace(sPartName.c_str(),"CycleOut        Stt"); Step.iSeq = scOut        ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleFlip      ) {Trace(sPartName.c_str(),"CycleFlip       Stt"); Step.iSeq = scFlip       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleInsp3Out  ) {Trace(sPartName.c_str(),"CycleInsp3Out   Stt"); Step.iSeq = scInsp3Out   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleInsp2Out  ) {Trace(sPartName.c_str(),"CycleInsp2Out   Stt"); Step.iSeq = scInsp2Out   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleInsp1Out  ) {Trace(sPartName.c_str(),"CycleInsp1Out   Stt"); Step.iSeq = scInsp1Out   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePreBuffOut) {Trace(sPartName.c_str(),"CyclePreBuffOut Stt"); Step.iSeq = scPreBuffOut ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn        ) {Trace(sPartName.c_str(),"CycleIn         Stt"); Step.iSeq = scIn         ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd         ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default           :                         Trace(sPartName.c_str(),"default         End");Step.iSeq = scIdle ;  return false ;
        case  scOut       : if(CycleOut        ()){ Trace(sPartName.c_str(),"CyclePbWkToWkOt End");Step.iSeq = scIdle ;} return false ;
        case  scFlip      : if(CycleFlip       ()){ Trace(sPartName.c_str(),"CycleRdWk       End");Step.iSeq = scIdle ;} return false ;
        case  scInsp3Out  : if(CycleInsp3Out   ()){ Trace(sPartName.c_str(),"CycleWkToOt     End");Step.iSeq = scIdle ;} return false ;
        case  scInsp2Out  : if(CycleInsp2Out   ()){ Trace(sPartName.c_str(),"CycleWkToOb     End");Step.iSeq = scIdle ;} return false ;
        case  scInsp1Out  : if(CycleInsp1Out   ()){ Trace(sPartName.c_str(),"CycleObToOt     End");Step.iSeq = scIdle ;} return false ;
        case  scPreBuffOut: if(CyclePreBuffOut ()){ Trace(sPartName.c_str(),"CycleWkToBf2    End");Step.iSeq = scIdle ;} return false ;
        case  scIn        : if(CycleIn         ()){ Trace(sPartName.c_str(),"CycleMask       End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CWorkZone::CycleIn(void) //스트립을 PreBuff로 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_PreBufBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiRL_PreBuffStp  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiRL_PreBuffStp  , ccFwd)) return false ;
                  IO_SetY(yRL_PreBufBelt , true) ;
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xRL_PreBufPkg)&&!OM.CmnOptn.bDryRun) return false ;

                  //여기서 전장비에서 데이터 가저오는 루틴.
                  //DM.ARAY[riPreBuff].ChangeStat(csGood , csUnkwn) ;
                    DM.ARAY[riPreBuff].ChangeStat(csGood , csUnkwn) ;
                  
                  IO_SetY(yRL_PreBufBelt , false) ;
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CWorkZone::CycleMoveAll(void)
{











}

bool CWorkZone::CycleMvToWK2(void)
{




}

//One Cycle.
bool CWorkZone::CycleOut(void) //Rail끝단에서 장비 밖으로 배출 되는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRL_PstBfGr , ccFwd);
                  MoveActr(aiID_Finger  , ccBwd);
                  MoveActr(aiRL_FlprAln , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiRL_PstBfGr , ccFwd)) return false ;
                  if(!MoveActr(aiID_Finger  , ccBwd)) return false ;
                  if(!MoveActr(aiRL_FlprAln , ccBwd)) return false ;
                  MoveMotr(miIDX_X , piIDX_XWait) ;
                  MoveMotr(miFRP_T , piFRP_TRail) ;
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  if(!MoveMotr(miFRP_T , piFRP_TRail)) return false ;
                  MoveActr(aiID_Finger  , ccFwd);
                  MoveActr(aiRL_PstBfGr , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiID_Finger  , ccFwd )) return false ;
                  if(!MoveActr(aiRL_PstBfGr , ccBwd )) return false ;
                  MoveMotr(miIDX_X , piIDX_XPreFrpr);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miIDX_X      , piIDX_XPreFrpr)) return false ;
                  MoveMotr(miIDX_X , piIDX_XOut) ;
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveMotr(miIDX_X , piIDX_XOut)) return false ;
                  //다음장비에 데이터 남김.
                  DM.ARAY[riPostBuff].SetStat(csEmpty) ;
                  MoveMotr(miIDX_X , piIDX_XWait);
                  MoveActr(aiRL_InspStp3 , ccFwd );
                  MoveActr(aiID_Finger   , ccBwd );
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  if(!MoveActr(aiRL_InspStp3 , ccFwd)) return false ;
                  if(!MoveActr(aiID_Finger   , ccBwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CWorkZone::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CWorkZone::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CWorkZone::CheckStop()
{
    if(!MT_GetStopInpos(miIDX_X)) return false ;
    if(!MT_GetStopInpos(miFRP_T)) return false ;

    if(!AT_Done(aiRL_InspAln1  )) return false ;
    if(!AT_Done(aiRL_InspAln2  )) return false ;
    if(!AT_Done(aiRL_InspAln3  )) return false ;
    if(!AT_Done(aiRL_FlprAln   )) return false ;
    if(!AT_Done(aiRL_InspStn1  )) return false ;
    if(!AT_Done(aiRL_InspStn2  )) return false ;
    if(!AT_Done(aiRL_InspStn3  )) return false ;
    if(!AT_Done(aiID_Finger    )) return false ;
    if(!AT_Done(aiRL_PstBfGr   )) return false ;
    if(!AT_Done(aiRJ_Sorter    )) return false ;
    if(!AT_Done(aiRJ_Stopper   )) return false ;
    if(!AT_Done(aiRL_PreBuffStp)) return false ;
    if(!AT_Done(aiRL_InspStp1  )) return false ;
    if(!AT_Done(aiRL_InspStp2  )) return false ;
    if(!AT_Done(aiRL_InspStp3  )) return false ;

    return true ;
}

void CWorkZone::Load(bool IsLoad , FILE *fp)
{

}

















