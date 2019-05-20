//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Rail.h"
#include "Head.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "EqpComUnit.h"
#include "VisnComUnit.h"
#include "LotUnit.h"
#include "UserIni.h"

//#include "FormRslt.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CRail  RAL;

CRail::CRail(void)
{
    m_sPartName = "Rail " ;
    Reset();
    Load(true);

    m_bLotEnded = false ;
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

CRail::~CRail (void)
{
    Load(false);

}

bool CRail::FindChip(int &r , int &c )
{
    return false;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    dPos = MT_GetCmdPos(_iMotr);

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    return true ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //Ok.
    return true;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    return false ;
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
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
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
        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10:




                 MoveActr(aiPRB_Stopper , ccFwd);

                 MoveActr(aiWK1_Align   , ccBwd);
                 MoveActr(aiWK1_Lift    , ccBwd);
                 MoveActr(aiWK1_Stopper , ccFwd);

                 MoveActr(aiWK2_Align   , ccBwd);
                 MoveActr(aiWK2_Lift    , ccBwd);
                 MoveActr(aiWK2_Stopper , ccFwd);


                 IO_SetY(yPRB_Belt,false) ;
                 IO_SetY(yWRK_Belt,false) ;
                 Trace("yPRB_Belt","false");

                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiPRB_Stopper , ccFwd))return false ;

                 if(!MoveActr(aiWK1_Align   , ccBwd))return false ;
                 if(!MoveActr(aiWK1_Lift    , ccBwd))return false ;
                 if(!MoveActr(aiWK1_Stopper , ccFwd))return false ;

                 if(!MoveActr(aiWK2_Align   , ccBwd))return false ;
                 if(!MoveActr(aiWK2_Lift    , ccBwd))return false ;
                 if(!MoveActr(aiWK2_Stopper , ccFwd))return false ;

                 //if(DM.ARAY[riVS1].GetCntStat(csUnkwn)) AT_MoveCyl(aiWK1_Align , ccFwd) ;
                 //if(DM.ARAY[riVS2].GetCntStat(csUnkwn)) AT_MoveCyl(aiWK2_Align , ccFwd) ;

                 Step.iHome++;
                 return false ;

        case 12: //if(DM.ARAY[riVS1].GetCntStat(csUnkwn)&& !AT_MoveCyl(aiWK1_Align , ccFwd)) return false ;
                 //if(DM.ARAY[riVS2].GetCntStat(csUnkwn)&& !AT_MoveCyl(aiWK2_Align , ccFwd)) return false ;

                 //if(DM.ARAY[riVS1].GetCntStat(csUnkwn)) AT_MoveCyl(aiWK1_Lift , ccFwd) ;
                 //if(DM.ARAY[riVS2].GetCntStat(csUnkwn)) AT_MoveCyl(aiWK2_Lift , ccFwd) ;

                 Step.iHome++;
                 return false ;

        case 13: //if(DM.ARAY[riVS1].GetCntStat(csUnkwn)&& !AT_MoveCyl(aiWK1_Lift , ccFwd)) return false ;
                 //if(DM.ARAY[riVS2].GetCntStat(csUnkwn)&& !AT_MoveCyl(aiWK2_Lift , ccFwd)) return false ;

                 //if(DM.ARAY[riVS1].GetCntStat(csUnkwn)) AT_MoveCyl(aiWK1_Align , ccBwd) ;
                 //if(DM.ARAY[riVS2].GetCntStat(csUnkwn)) AT_MoveCyl(aiWK2_Align , ccBwd) ;

                 Step.iHome++;
                 return false ;

        case 14: //if(DM.ARAY[riVS1].GetCntStat(csUnkwn)&& !AT_MoveCyl(aiWK1_Align , ccBwd)) return false ;
                 //if(DM.ARAY[riVS2].GetCntStat(csUnkwn)&& !AT_MoveCyl(aiWK2_Align , ccBwd)) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    EC.SetPreEqpReady(false);
    if (EC.GetPreEqpSending()) return false ;
    if (Step.iSeq) return false;


    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
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

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10:
                 IO_SetY(yPRB_Belt,false) ;
                 IO_SetY(yWRK_Belt,false) ;
                 Trace("yPRB_Belt","false");
                 Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
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
        default: Step.iToStop = 0;
                 return true ;

        case 10:
                 IO_SetY(yPRB_Belt,false) ;
                 IO_SetY(yWRK_Belt,false) ;
                 Trace("yPRB_Belt","false");
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}


bool CRail::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    bool b1 = !DM.ARAY[riPRB].GetCntExist() ;
    bool b2 = !IO_GetX(xPRB_PkgIn2) ;
    bool b3 = !IO_GetX(xPRB_PkgIn1) ;
    bool b4 = !IO_GetX(xPRB_Pkg   ) ;



    EC.SetPreEqpReady(b1 && b2 && b3 && b4 && Step.iSeq == 0);

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && !EC.GetPreEqpSending())return false ;

        bool isVS1End = DM.ARAY[riWK1].GetCntExist() && (!DM.ARAY[riVS1].GetCntStat(csUnkwn) && !DM.ARAY[riWK1].GetCntStat (csUnkwn));
        bool isVS2End = DM.ARAY[riWK2].GetCntExist() && (!DM.ARAY[riVS2].GetCntStat(csUnkwn) && !DM.ARAY[riWK2].GetCntStat (csUnkwn));
        bool isWK1End =!DM.ARAY[riWK1].GetCntExist() || isVS1End ;
        bool isWK2End =!DM.ARAY[riWK2].GetCntExist() || isVS2End ;



        bool isCycleIn    = ((IO_GetX(xPRB_PkgIn1) ||  IO_GetX(xPRB_PkgIn2)) || OM.CmnOptn.bDryRun) &&
                             !DM.ARAY[riPRB].GetCntExist() ;

        bool isCycleBind  = (DM.ARAY[riVS1].GetCntStat(csUnkwn)&& IO_GetX(xWK1_Pkg) && !AT_Complete(aiWK1_Lift,ccFwd)) ||
                            (DM.ARAY[riVS2].GetCntStat(csUnkwn)&& IO_GetX(xWK2_Pkg) && !AT_Complete(aiWK2_Lift,ccFwd)) ;

        bool isDoingInsp  =  HED.GetSeqStep() == CHead::scInsp ;

        bool isExistPSBX  =  IO_GetX(xPSB_Pkg) || IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2) ;


        bool isPSBReady   = AT_Complete(aiPSB_Flpr , ccBwd) && AT_Complete(aiPSB_FlprCmp , ccBwd) && !isExistPSBX;

        //bool isChangeLot  =

        m_bWaitLotChange  =  DM.ARAY[riPRB].GetCntExist() && DM.ARAY[riWK1].GetCntExist() &&
                             DM.ARAY[riPRB].GetLotNo   () != DM.ARAY[riWK1].GetLotNo   () ;

        bool isCycleMvAll  =  DM.ARAY[riPRB].GetCntExist() &&  isVS1End                     &&  isVS2End                     && DM.ARAY[riPSB].CheckAllStat(csNone) && !m_bWaitLotChange && isPSBReady;
        bool isCycleMvR2   =  DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWK1].GetCntExist() &&  isVS2End                     && DM.ARAY[riPSB].CheckAllStat(csNone) && !m_bWaitLotChange && isPSBReady;
        bool isCycleMvR1   =  DM.ARAY[riPRB].GetCntExist() &&  isVS1End                     && !DM.ARAY[riWK2].GetCntExist() ;
//      bool isCycleMv12   =(!DM.ARAY[riPRB].GetCntExist() ||
//                            m_bWaitLotChange            )&&  isVS1End                     &&  isVS2End                     && DM.ARAY[riPSB].CheckAllStat(csNone) &&  isPSBReady;
        bool isCycleMv12   =  m_bWaitLotChange             &&  isVS1End                     &&  isVS2End                     && DM.ARAY[riPSB].CheckAllStat(csNone) &&  isPSBReady;
        bool isCycleMvR    =  DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWK1].GetCntExist() &&  DM.ARAY[riPSB].CheckAllStat(csNone);
        bool isCycleMv1    =                                   isVS1End                     && !DM.ARAY[riWK2].GetCntExist() ;
        bool isCycleMv2    =                                                                    isVS2End                     && DM.ARAY[riPSB].CheckAllStat(csNone) &&  isPSBReady;

        if(!EC.GetPreEqpLotEnd()) { //스트립 차곡차곡 쌓고싶을때 쓰고 아니면 주석처리.
            isCycleMvAll  = isCycleMvAll ;
            isCycleMvR2   = isCycleMvR2  ;
            isCycleMvR1   = isCycleMvR1  ;
            isCycleMv12   = isCycleMv12  ;
            isCycleMvR    = isCycleMvR   ;
            isCycleMv1    = false        ;
            isCycleMv2    = false        ;
        }






        bool isCycleMvAuto = (isWK1End   && isWK2End) && !isDoingInsp && //DM.ARAY[riPSB].CheckAllStat(csNone) &&
                             (isCycleMvAll || isCycleMvR2 || isCycleMvR1 || isCycleMv12 || isCycleMvR || isCycleMv1 || isCycleMv2) ;



        bool isConEnd      = !DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWK1].GetCntExist()  &&  !DM.ARAY[riWK2].GetCntExist() &&  !DM.ARAY[riPSB].GetCntExist() ;


        //모르는 스트립에러.
        if(!DM.ARAY[riPRB ].CheckExist(0,0) &&  IO_GetX(xPRB_Pkg) ) EM_SetErr(eiPRB_PkgUnknown ) ;
        if(!DM.ARAY[riWK1 ].CheckExist(0,0) &&  IO_GetX(xWK1_Pkg) ) EM_SetErr(eiWK1_PkgUnknown ) ;
        if(!DM.ARAY[riWK2 ].CheckExist(0,0) &&  IO_GetX(xWK2_Pkg) ) EM_SetErr(eiWK2_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) EM_SetErr(eiPRB_PkgDispr ) ;

        //스테이지 올리면 센서가 감지 않되어 뺀다.
        if( DM.ARAY[riWK1 ].CheckExist(0,0) && !IO_GetX(xWK1_Pkg) && AT_Complete(aiWK1_Lift , ccBwd) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK1_PkgDispr ) ;
        if( DM.ARAY[riWK2 ].CheckExist(0,0) && !IO_GetX(xWK2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.CmnOptn.bDryRun) EM_SetErr(eiWK2_PkgDispr ) ;


        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn    ) {Trace(m_sPartName.c_str(),"CycleIn     Stt"); Step.iSeq = scIn     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleMvAuto) {Trace(m_sPartName.c_str(),"CycleMvAuto Stt"); Step.iSeq = scMvAuto ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(pRb->wk1->wk2)->pSb
        else if (isCycleBind  ) {Trace(m_sPartName.c_str(),"CycleBind   Stt"); Step.iSeq = scBind   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(pRb->wk1->wk2)->pSb
//      else if (isCycleMvAll ) {Trace(m_sPartName.c_str(),"CycleMvAll  Stt"); Step.iSeq = scMvAll  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(pRb->wk1->wk2)->pSb
//      else if (isCycleMvR2  ) {Trace(m_sPartName.c_str(),"CycleMvR2   Stt"); Step.iSeq = scMvR2   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(pRb->wk1  wk2)->pSb
//      else if (isCycleMvR1  ) {Trace(m_sPartName.c_str(),"CycleMvR1   Stt"); Step.iSeq = scMvR1   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(pRb->wk1->wk2)
//      else if (isCycleMv12  ) {Trace(m_sPartName.c_str(),"CycleMv12   Stt"); Step.iSeq = scMv12   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(     wk1->wk2)->pSb
//      else if (isCycleMvR   ) {Trace(m_sPartName.c_str(),"CycleMvR    Stt"); Step.iSeq = scMvR    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(pRb->wk1     )
//      else if (isCycleMv1   ) {Trace(m_sPartName.c_str(),"CycleMv1    Stt"); Step.iSeq = scMv1    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(     wk1->wk2)
//      else if (isCycleMv2   ) {Trace(m_sPartName.c_str(),"CycleMv2    Stt"); Step.iSeq = scMv2    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //(          wk2)->pSb
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                 /*Trace(m_sPartName.c_str(),"default     End");*/Step.iSeq = scIdle ;  return false ;
        case  scIn     : if(CycleIn     ()){ Trace(m_sPartName.c_str(),"CycleIn     End");Step.iSeq = scIdle ;} return false ;
        case  scMvAuto : if(CycleMvAuto ()){ Trace(m_sPartName.c_str(),"CycleMvAuto End");Step.iSeq = scIdle ;} return false ;
        case  scBind   : if(CycleBind   ()){ Trace(m_sPartName.c_str(),"CycleBind   End");Step.iSeq = scIdle ;} return false ;

/*
        case  scMvAll  : if(CycleMvAll  ()){ Trace(m_sPartName.c_str(),"CycleMvAll  End");Step.iSeq = scIdle ;} return false ;
        case  scMvR2   : if(CycleMvR2   ()){ Trace(m_sPartName.c_str(),"CycleMvR2   End");Step.iSeq = scIdle ;} return false ;
        case  scMvR1   : if(CycleMvR1   ()){ Trace(m_sPartName.c_str(),"CycleMvR1   End");Step.iSeq = scIdle ;} return false ;
        case  scMv12   : if(CycleMv12   ()){ Trace(m_sPartName.c_str(),"CycleMv12   End");Step.iSeq = scIdle ;} return false ;
        case  scMvR    : if(CycleMvR    ()){ Trace(m_sPartName.c_str(),"CycleMvR    End");Step.iSeq = scIdle ;} return false ;
        case  scMv1    : if(CycleMv1    ()){ Trace(m_sPartName.c_str(),"CycleMv1    End");Step.iSeq = scIdle ;} return false ;
        case  scMv2    : if(CycleMv2    ()){ Trace(m_sPartName.c_str(),"CycleMv2    End");Step.iSeq = scIdle ;} return false ;
*/
    }
}

bool CRail::CycleIn(void) //스트립을 PreBuff로 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiRAL_CycleTO);
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
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPRB_Stopper  , ccFwd);
                  IO_SetY(yPRB_AirBlow,true); //열을 식혀 보장...
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_Stopper  , ccFwd)) return false ;

                  IO_SetY(yPRB_Belt , true) ;  //ana
                  Trace("yPRB_Belt","true");
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xPRB_Pkg)&&!OM.CmnOptn.bDryRun) return false ;

                  if(OM.EqpOptn.bExistLoader || OM.CmnOptn.bDryRun) {
                      DM.ARAY[riPRB].SetStat   (csUnkwn) ;
                  }
                  else { //여기서 전장비에서 데이터 가저오는 루틴.
                      Trace("Try","CycleIn");
                      if(!EC.ReadArayData(riPRB)) {
                          EM_SetErr(eiETC_PreEqpRead) ;
                          Step.iCycle = 0;
                          return true ;

                      }
                      DM.ARAY[riPRB].ChangeStat(csGood , csUnkwn) ;
                      VC.WriteLogMap(riPRB);
                      LT.WriteArayData(riPRB);
                  }

//                  IO_SetY(yPRB_Belt , false) ;
                  IO_SetY(yPRB_AirBlow,false); //열을 식혀 보장...
//                  Trace("yPRB_Belt","false");
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CRail::CycleMvAuto(void) //자동 계산  이동.
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

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    static bool bCanMvPRB = false ;
    static bool bCanMvWK1 = false ;
    static bool bCanMvWK2 = false ;

    bool r1 , r2 , r3 , r4 ;

    static bool bInPkg = false ;
    static bool bCatch = false ;

    if(IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_PkgIn2)) bInPkg = true ;

    if( bCanMvWK2&& IO_GetXDn(xWK2_Out)) {MoveActr(aiPSB_FlprCmp , ccFwd); bCatch = true ; Trace("Catch","PSB_PKG");}

    bool bLastStrip ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bInPkg = false ;
                  bCatch = false ;
                  r1 = !DM.ARAY[riWK1].GetCntExist() ;
                  r2 =  DM.ARAY[riPRB].GetCntExist() ;

                  bCanMvWK2 = DM.ARAY[riWK2].GetCntExist()&& DM.ARAY[riVS2].GetCntExist() &&  !DM.ARAY[riPSB].GetCntExist() ;
                  bCanMvWK1 = DM.ARAY[riWK1].GetCntExist()&& DM.ARAY[riVS1].GetCntExist() && (!DM.ARAY[riWK2].GetCntExist()||bCanMvWK2) ;
                  bCanMvPRB = DM.ARAY[riPRB].GetCntExist()&& (!DM.ARAY[riWK1].GetCntExist()||bCanMvWK1) && !m_bWaitLotChange;

                  //Trace("Rail","WaitforLotChange");
                  if(bCanMvWK2) {
                      LT.AddDayInfoWorkStrp(1);
                      LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

                      LT.AddLotInfoWorkStrp(1);
                      LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

                      LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riWK2].GetCntStat(csWork));
                      LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riWK2].GetCntStat(csWork));

                      LT.AddDayInfoChips(DM.ARAY[riWK2].GetCntStat(csRslt1 ) , //"V Empty"
                                         DM.ARAY[riWK2].GetCntStat(csRslt2 ) , //"V Chip"
                                         DM.ARAY[riWK2].GetCntStat(csRslt3 ) , //"V Dust"
                                         DM.ARAY[riWK2].GetCntStat(csRslt4 ) , //"V LFlow"
                                         DM.ARAY[riWK2].GetCntStat(csRslt5 ) , //"V Broken"
                                         DM.ARAY[riWK2].GetCntStat(csRslt6 ) , //"V Crack"
                                         DM.ARAY[riWK2].GetCntStat(csRslt7 ) , //"V Burr"
                                         DM.ARAY[riWK2].GetCntStat(csRslt8 ) , //"V Zener"
                                         DM.ARAY[riWK2].GetCntStat(csRslt9 ) , //"V Wire"
                                         DM.ARAY[riWK2].GetCntStat(csRslt10) , //"V Orient"
                                         DM.ARAY[riWK2].GetCntStat(csRslt11) , //"V Pmeasure"
                                         DM.ARAY[riWK2].GetCntStat(csRslt12) , //"V Pdist"
                                         DM.ARAY[riWK2].GetCntStat(csRslt13) , //"V Match"
                                         DM.ARAY[riWK2].GetCntStat(csRslt14) , //"V Flow"
                                         DM.ARAY[riWK2].GetCntStat(csFail  ) ); //"Heat Fail");

                      LT.AddLotInfoChips(DM.ARAY[riWK2].GetCntStat(csRslt1 ) , //"V Empty"
                                         DM.ARAY[riWK2].GetCntStat(csRslt2 ) , //"V Chip"
                                         DM.ARAY[riWK2].GetCntStat(csRslt3 ) , //"V Dust"
                                         DM.ARAY[riWK2].GetCntStat(csRslt4 ) , //"V LFlow"
                                         DM.ARAY[riWK2].GetCntStat(csRslt5 ) , //"V Broken"
                                         DM.ARAY[riWK2].GetCntStat(csRslt6 ) , //"V Crack"
                                         DM.ARAY[riWK2].GetCntStat(csRslt7 ) , //"V Burr"
                                         DM.ARAY[riWK2].GetCntStat(csRslt8 ) , //"V Zener"
                                         DM.ARAY[riWK2].GetCntStat(csRslt9 ) , //"V Wire"
                                         DM.ARAY[riWK2].GetCntStat(csRslt10) , //"V Orient"
                                         DM.ARAY[riWK2].GetCntStat(csRslt11) , //"V Pmeasure"
                                         DM.ARAY[riWK2].GetCntStat(csRslt12) , //"V Pdist"
                                         DM.ARAY[riWK2].GetCntStat(csRslt13) , //"V Match"
                                         DM.ARAY[riWK2].GetCntStat(csRslt14) ,
                                         DM.ARAY[riWK2].GetCntStat(csFail  ) ); //"Heat Fail");//"V Flow"
                                    //!DM.ARAY[riWK2].CheckAllStat(csNone) &&

                      bLastStrip = (DM.ARAY[riWK1].CheckAllStat(csNone) && DM.ARAY[riWK2].GetLotNo() != DM.ARAY[riPRB].GetLotNo() && !DM.ARAY[riPRB].CheckAllStat(csNone)) ||
                                   (EC.GetPreEqpLotEnd() && DM.ARAY[riWK1].CheckAllStat(csNone) &&  DM.ARAY[riPRB].CheckAllStat(csNone)) ;

                      if(bLastStrip) {
                          m_bLotEnded = true ;
                          memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

                          Trace("LotEnd",DM.ARAY[riWK2].GetLotNo().c_str());
                          LT.LotInfo.dEndTime = Now();
                          LT.WriteLotLog   ();
                          LT.WriteLotDayLog();
                          Trace("WriteLotLog",DM.ARAY[riWK2].GetLotNo().c_str());
                          //LT.LotEnd();
                      }

                  }

                  if(bCanMvPRB && (m_sPreLotNo != DM.ARAY[riPRB].GetLotNo())  ){
                      Trace("LotOpen",DM.ARAY[riPRB].GetLotNo().c_str());

                      LT.LotOpen(DM.ARAY[riPRB].GetLotNo());

                      if(!OM.EqpOptn.bExistLoader)LT.AddDayInfoLotCnt(1);
                      if(!OM.CmnOptn.bVisn1Skip || !OM.CmnOptn.bVisn2Skip) VC.SendLotStart();
                      /*sun*/
                      m_sPreLotNo = DM.ARAY[riPRB].GetLotNo() ;
                      Load(false);
                  }

                  MoveActr(aiWK1_Lift    , ccBwd);
                  MoveActr(aiWK2_Lift    , ccBwd);

                  MoveActr(aiWK1_Align   , ccBwd);
                  MoveActr(aiWK2_Align   , ccBwd);
                  MoveActr(aiPSB_Align   , ccBwd);




                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWK1_Lift    , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Lift    , ccBwd)) return false ;

                  if(!MoveActr(aiWK1_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Align   , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Align   , ccBwd)) return false ;

                  IO_SetY(yPRB_Belt , true);   //ana
                  IO_SetY(yWRK_Belt , true);
                  Trace("yPRB_Belt","true");



                  if(bCanMvWK2)MoveActr(aiWK2_Stopper , ccBwd);
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iWK1StprDnDelay))return false ;
                  if(bCanMvWK1)MoveActr(aiWK1_Stopper , ccBwd);
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iPRBStprDnDelay))return false ;
                  if(bCanMvPRB)MoveActr(aiPRB_Stopper , ccBwd);

                  m_tmTemp.Clear();

                  Step.iCycle++ ;
                  return false ;

        case  14: if(bCanMvPRB&&!MoveActr(aiPRB_Stopper , ccBwd)) return false ;
                  if(bCanMvWK1&&!MoveActr(aiWK1_Stopper , ccBwd)) return false ;
                  if(bCanMvWK2&&!MoveActr(aiWK2_Stopper , ccBwd)) return false ;

                  //비젼어레이는 나중에 스테이지 올리고 바꾼다.
                  //플립할때 뒤집지 않고 들어 왔을때 뒤집어야 예외상황이 적다. 플립에 뒤집으면 트림하다 말고 장비 껐다 키면 데이터 뒤집힐수 있음
                  if(bCanMvWK2){
                      if(OM.CmnOptn.bNoTrimFail){
                          DM.ARAY[riWK2].SetStat(csWork);
                          /*DM.ARAY[riWK2].SetStat(0,0,csFail);*/
                      }
                  }
                  if(bCanMvWK2){
                      DM.ShiftArrayData(riWK2 , riPSB) ;
                      DM.ARAY[riVS2].SetStat(csNone)  ;
                      DM.ARAY[riPSB].FlipY();
                  }
                  if(bCanMvWK1){

                      DM.ShiftArrayData(riWK1 , riWK2) ;
                      /*DM.ShiftArrayData(riWK1 , riW2T);*/
                      DM.ARAY[riVS1].SetStat(csNone) ;
                      DM.ARAY[riVS2].SetStat(csUnkwn);
                      DM.ARAY[riWK2].ChangeStat(csWork , csUnkwn) ;
                      //DM.ARAY[riWB2] = DM.ARAY[riWK2] ; 생각해보니 굳이 여기에 있을필요 없고 메뉴얼로 가면 될듯함.
                  }
                  if(bCanMvPRB){
                      DM.ShiftArrayData(riPRB , riWK1) ;
                      DM.ARAY[riVS1].SetStat(csUnkwn);
                      //DM.ARAY[riWB1] = DM.ARAY[riWK1] ;
                  }

                  IO_SetY(yPRB_Belt , true);
                  IO_SetY(yWRK_Belt , true);
                  Trace("yPRB_Belt","true");
                  Trace("Get yPRB_Belt" , IO_GetY(yPRB_Belt)?"True":"False");


                  Step.iCycle++ ;
                  return false ;

        case  15:
                  if(!IO_GetX(xPRB_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiPRB_Stopper , ccFwd) ;
                  if(!IO_GetX(xWK1_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiWK1_Stopper , ccFwd) ;
                  if(!IO_GetX(xWK2_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiWK2_Stopper , ccFwd) ;

                  if(!AT_GetCmd(aiPRB_Stopper)|| !AT_GetCmd(aiWK1_Stopper) || !AT_GetCmd(aiWK2_Stopper)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  16: if(           !MoveActr(aiPRB_Stopper , ccFwd)) return false ;
                  if(bCanMvPRB&&!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  if(bCanMvWK1&&!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  Step.iCycle++ ;
                  return false ;

        case  17:
                  if( bInPkg                        &&!IO_GetX(xPRB_Pkg)) return false ;
                  if( bCanMvPRB&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWK1_Pkg)) return false ;
                  if( bCanMvWK1&&!OM.CmnOptn.bDryRun&&!IO_GetX(xWK2_Pkg)) return false ;
                  if( bCanMvWK2&&!OM.CmnOptn.bDryRun&& IO_GetX(xWK2_Out)) return false ;

                  if(bInPkg) {
                      if(OM.EqpOptn.bExistLoader) {
                          DM.ARAY[riPRB].SetStat   (csUnkwn) ;
                      }
                      else { //여기서 전장비에서 데이터 가저오는 루틴.
                          Trace("Try","CycleMvAuto");
                          if(!EC.ReadArayData(riPRB)) {
                              EM_SetErr(eiETC_PreEqpRead) ;
                              Step.iCycle = 0;
                              return true ;

                          }
                          DM.ARAY[riPRB].ChangeStat(csGood , csUnkwn) ;
                      }
                      Trace("PreBuff","PkgIn While MvAuto");
                  }
                  /*
                  if(OM.EqpOptn.bExistLoader || OM.CmnOptn.bDryRun) {
                      DM.ARAY[riPRB].SetStat   (csUnkwn) ;
                  }
                  else { //여기서 전장비에서 데이터 가저오는 루틴.
                      if(!EC.ReadArayData(riPRB)) {
                          EM_SetErr(eiETC_PreEqpRead) ;
                          Step.iCycle = 0;
                          return true ;

                      }
                      DM.ARAY[riPRB].ChangeStat(csGood , csUnkwn) ;
                  }
                  IO_SetY(yPRB_Belt , false) ;
                  Step.iCycle = 0;
                  return true ;
                  */

//                  IO_SetY(yPRB_Belt , false);
                  IO_SetY(yWRK_Belt , false);
//                  Trace("yPRB_Belt","false");

                  Step.iCycle++;
                  return false ;

        case  18: if( bCatch && !MoveActr(aiPSB_FlprCmp , ccFwd)) return false ;
                  if( bCatch                                    )MoveActr(aiPSB_FlprCmp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if( bCatch && !MoveActr(aiPSB_FlprCmp , ccBwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  20: if(!m_tmTemp.OnDelay(true , 100)) return false ;
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

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    static bool bExistWK1 = false ;
    static bool bExistWK2 = false ;

    bool r1 , r2 , r3 , r4 ;

    //static bool bInPkg = false ;

    //if(IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_PkgIn2)) bInPkg = true ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: bExistWK2 = DM.ARAY[riWK2].GetCntExist();
                  bExistWK1 = DM.ARAY[riWK1].GetCntExist();

                  MoveActr(aiWK1_Stopper , ccFwd);
                  MoveActr(aiWK2_Stopper , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWK1_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Stopper , ccFwd)) return false ;
                  IO_SetY(yWRK_Belt , true);
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 50)) return false ;
                  IO_SetY(yWRK_Belt , false);

                  MoveActr(aiWK1_Align , ccFwd);
                  MoveActr(aiWK2_Align , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiWK1_Align , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Align , ccFwd)) return false ;

//                  MoveActr(aiWK1_Align , ccBwd);
//                  MoveActr(aiWK2_Align , ccBwd);
//
//                  Step.iCycle++ ;
//                  return false ;
//
//        case  14: if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
//                  if(!MoveActr(aiWK2_Align , ccBwd)) return false ;

                  MoveActr(aiWK1_Lift , ccFwd);
                  MoveActr(aiWK2_Lift , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiWK1_Lift , ccFwd)) return false ;
                  if(!MoveActr(aiWK2_Lift , ccFwd)) return false ;
                  MoveActr(aiWK1_Align , ccBwd);
                  MoveActr(aiWK2_Align , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiWK1_Align , ccBwd)) return false ;
                  if(!MoveActr(aiWK2_Align , ccBwd)) return false ;
                  //스테이지 올리고 바꿔야 바인딩 되고 HEAD에서 검사를 제때 시작 한다.                  DM.ARAY[riWK1].GetCntExist()
                  if( bExistWK1 && DM.ARAY[riVS1].CheckAllStat(csNone))DM.ARAY[riVS1].SetStat(csUnkwn);
                  if( bExistWK2 && DM.ARAY[riVS2].CheckAllStat(csNone))DM.ARAY[riVS2].SetStat(csUnkwn);

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
    if(!AT_Done(aiPRB_Stopper )) return false ;

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
    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo" , m_sPreLotNo  );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sPreLotNo" , m_sPreLotNo  );
    }
}

