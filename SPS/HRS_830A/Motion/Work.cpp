//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Work.h"
#include "PreBuff.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "Rs232Man.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CWork WRK;

CWork::CWork(void)
{

}

CWork::~CWork (void)
{

}

void CWork::Init()
{
    m_sPartName = "Work Zone" ;
    Reset();
    Load(true);

    InitCycleName();

    m_iShotCnt = 0;

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

    m_bNeedStrip = false ;
}

void CWork::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CWork::FindChip(int &r , int &c , EN_GETFINDCHIP iFCNum)
{
         if(iFCNum == fcCol   ) DM.ARAY[riLWK].FindLastColRow    (csUnkwn , r, c);
    else if(iFCNum == fcRowOdd) DM.ARAY[riLWK].FindLastColFrstRow(csUnkwn , r, c);
    else                        DM.ARAY[riLWK].FindLastColRow    (csUnkwn , r, c);

    return (r > -1 && c > -1);
}

double CWork::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = MT_GetCmdPos(_iMotr) ;

    int r1, r2, c1, c2;
    //     기본 X위치     Single X 위치   Y위치        JS. 2014.7.15 AP Y축 추가..
    double dXIndxWorkPos, dXIndxRWorkPos, dYIndxWorkPos;
    FindChip(r1, c1, fcCol);
    int iColOfs = OM.DevInfo.iColCnt / 2;
    if(c1 < OM.DevInfo.iColCnt / 2 && OM.MstOptn.bUsedSingleDisps) c1 = OM.DevInfo.iColCnt - c1 - 1 - iColOfs;  //뒤집어 줘야 한다.
    else                                                           c1 = OM.DevInfo.iColCnt - c1 - 1          ;  //뒤집어 줘야 한다.
    dXIndxWorkPos  = PM.GetValue(miWRK_XIndx , pvWRK_XIndxWorkStt ) - (c1 * OM.DevInfo.dColPitch);
    dXIndxRWorkPos = PM.GetValue(miWRK_XIndx , pvWRK_XIndxRWorkStt) - (c1 * OM.DevInfo.dColPitch);
    if(c1 % 2) FindChip(r2, c2, fcRowOdd );
    else       FindChip(r2, c2, fcRowEven);
    dYIndxWorkPos  = PM.GetValue(miWRK_YDsps , pvWRK_YDspsWorkStt ) - (r2 * OM.DevInfo.dRowPitch);
    if(_iMotr == miWRK_XIndx){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piWRK_XIndxWait     : dPos = PM.GetValue (_iMotr , pvWRK_XIndxWait      ); break ;
            case piWRK_XIndxWorkStt  : dPos = PM.GetValue (_iMotr , pvWRK_XIndxWorkStt   ); break ;
            case piWRK_XIndxRWorkStt : dPos = PM.GetValue (_iMotr , pvWRK_XIndxRWorkStt  ); break ;
            case piWRK_XIndxOut      : dPos = PM.GetValue (_iMotr , pvWRK_XIndxOut       ); break ;
            case piWRK_XIndxWork     : dPos = dXIndxWorkPos                               ; break ;
            case piWRK_XIndxRWork    : dPos = dXIndxRWorkPos                              ; break ;
        }
    }

    else if(_iMotr == miWRK_YDsps){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piWRK_YDspsWait     : dPos = PM.GetValue (_iMotr , pvWRK_YDspsWait      ); break ;
            case piWRK_YDspsWorkStt  : dPos = PM.GetValue (_iMotr , pvWRK_YDspsWorkStt   ); break ;
            case piWRK_YDspsWork     : dPos = dYIndxWorkPos                               ; break ;
            //case piWRK_YDspsWork  : dPos = PM.GetValue (_iMotr , pvWRK_YDspsWork      ); break ;
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

    bool isLIndxUp = AT_Complete(yWRK_LIdxUpDn , ccFwd);
    bool isRIndxUp = AT_Complete(yWRK_RIdxUpDn , ccFwd);

    if(_iMotr == miWRK_XIndx){
        switch(_iPstnId) {
            default                  :                                                                         break ;
            case piWRK_XIndxWait     : if(isLIndxUp || isRIndxUp) {sMsg = "Index Cylinder Up"; bRet = false;}  break ;
            case piWRK_XIndxWorkStt  :                                                                         break ;
            case piWRK_XIndxWork     :                                                                         break ;
            case piWRK_XIndxOut      :                                                                         break ;
        }
    }

    else if(_iMotr == miWRK_YDsps){
        switch(_iPstnId) {
            default               :                                                                           break ;
            case piWRK_YDspsWait  :                                                                           break ;
            case piWRK_YDspsWork  :                                                                           break ;
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CWork::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd )) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiWRK_LIdUpDn) {
        //if(_bFwd) {
        //    if(isXMoving) { sMsg = AnsiString(MT_GetName(miRAL_XIdx))+" is Moving" ; bRet = false ;}
        //}
    }
    else if(_iActr == aiWRK_RIdUpDn) {
    }

    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CWork::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CWork::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWork::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 8000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiRAL_HomeTo);
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

        case 10: AT_MoveCyl(aiWRK_LIdUpDn , ccBwd);
                 AT_MoveCyl(aiWRK_RIdUpDn , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_LIdUpDn , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWRK_RIdUpDn , ccBwd)) return false ;
                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiWRK_LIdUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiWRK_RIdUpDn  ,ccBwd)) return false ;
                 MT_SetHomeEnd(miWRK_XIndx , false);
                 MT_SetHomeEnd(miWRK_YDsps , false);

                 MT_DoHome(miWRK_XIndx);
                 MT_DoHome(miWRK_YDsps);
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miWRK_XIndx)) return false;
                 if(!MT_GetHomeEnd(miWRK_YDsps)) return false;

                 MT_GoAbsMan(miWRK_XIndx , m_dLastIndPos);
                 MT_GoAbsMan(miWRK_YDsps, PM.GetValue(miWRK_YDsps , pvWRK_YDspsWait));
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GoAbsMan(miWRK_XIndx , m_dLastIndPos))return false ;
                 if(!MT_GoAbsMan(miWRK_YDsps, PM.GetValue(miWRK_YDsps , pvWRK_YDspsWait)))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CWork::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

//프리버퍼 오리검사 레이져조사  마킹검사필요  비젼검사
//Unkwn -> Mask     Work        Wait          Good
//                                            Fail

    int r, c;
    FindChip(r, c, fcCol);

    if (Step.iSeq == 0) {
        if(Stat.bReqStop )return false ;

        bool bLWKOut     = DM.ARAY[riLWK].CheckAllStat(csWork ) && DM.ARAY[riRWK].CheckAllStat(csNone) ;
        bool bRWKOut     =!DM.ARAY[riLWK].GetCntStat  (csUnkwn) && DM.ARAY[riRWK].CheckAllStat(csWork) &&
                            DM.ARAY[riPSB].CheckAllStat(csNone ) && PSB.GetSeqStep() != PSB.CPostBuff::scOut &&
                            PRB.GetSeqStep() != PRB.CPreBuff ::scOut &&
                            AT_Complete(aiPSB_IdxUpDn , ccBwd);

        bool isCycleIn    = (DM.ARAY[riLWK].CheckAllStat(csUnkwn) && DM.ARAY[riLWK].GetStep() == 0) ||
                           ((DM.ARAY[riRWK].CheckAllStat(csUnkwn) && DM.ARAY[riRWK].GetStep() == 0) && PRB.GetSeqStep() != PRB.CPreBuff::scOut);

        bool isCycleWork  = DM.ARAY[riLWK].GetCntStat  (csUnkwn) && DM.ARAY[riLWK].GetStep() != 0 && DM.ARAY[riULD].GetCntStat(csEmpty) && ULD.GetSeqStep() == CUnLoader :: scIdle ; // 준선이형이 추가하라고 한 부분 ULD.GetSeqStep() 부터...
        bool isCycleOut   = bLWKOut || bRWKOut ;

        //if(c >= OM.DevInfo.iColCnt / 2)  { isCycleWork = DM.ARAY[riLWK].GetCntStat  (csUnkwn) && DM.ARAY[riLWK].GetStep() != 0 ;
        //else                           { isCycleWork = DM.ARAY[riLWK].GetCntStat  (csUnkwn) && DM.ARAY[riLWK].GetStep() != 0 &&
        //                                               DM.ARAY[riPSB].CheckAllStat(csNone ) && PSB.GetSeqStep() != PSB.CPostBuff::scOut ; }
        //bool isCycleWork  =  DM.ARAY[riLWK].GetCntStat  (csUnkwn) && DM.ARAY[riLWK].GetStep() != 0;

        //if(!OM.MstOptn.bUsedSingleDisps){ isCycleOut =  isLWKOut || isRWKOut ;}
        //else                            { isCycleOut = (isLWKOut || isRWKOut ) && DM.ARAY[riPSB].CheckAllStat(csNone) && PSB.GetSeqStep() != PSB.CPostBuff::scOut; }


        //bool isCycleOut   = isLWKOut || isRWKOut;

        bool isCycleEnd   = DM.ARAY[riLWK].CheckAllStat(csNone) &&
                            DM.ARAY[riRWK].CheckAllStat(csNone) && PRB.GetWorkEnd();



        //Unknown Strip
        //if (IO_GetX(xRAL_VPkg4) && DM.ARAY[riVSN].CheckAllStat(csNone)){
        //    EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riVSN].GetName().c_str()) ;
        //}

        //Dissapear Strip
        //if (!OM.MstOptn.bDryRun) {
        //    if (!IO_GetX(xRAL_LPkg3) && !DM.ARAY[riLSR].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riLSR].GetName().c_str()) ;
        //    bool bTemp = !IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone) ;
        //    if (!IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riVSN].GetName().c_str()) ;
        //}

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut ) {Trace(m_sPartName.c_str(),"CycleOut  Stt"); Step.iSeq = scOut  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleWork) {Trace(m_sPartName.c_str(),"CycleWork Stt"); Step.iSeq = scWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn  ) {Trace(m_sPartName.c_str(),"CycleIn   Stt"); Step.iSeq = scIn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default     :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle :                                                                                   return false ;
        case scIn   : if(CycleIn  ()){ Trace(m_sPartName.c_str(),"CycleIn   End");Step.iSeq = scIdle ;} return false ;
        case scWork : if(/*c < OM.DevInfo.iColCnt/2*/DM.ARAY[riLWK].GetStep() == 2 && OM.MstOptn.bUsedSingleDisps){
                          if(CycleSingleWrk()){ Trace(m_sPartName.c_str(),"CycleWork End");Step.iSeq = scIdle ;} return false;
                      }
                      else{
                          if(CycleWork()     ){ Trace(m_sPartName.c_str(),"CycleWork End");Step.iSeq = scIdle ;} return false ;
                      }
        case scOut  : if(CycleOut ()){ Trace(m_sPartName.c_str(),"CycleOut  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CWork::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str());

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    bool bExstUnKnown = false ;

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: MoveMotr(miWRK_YDsps , piWRK_YDspsWait);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miWRK_YDsps , piWRK_YDspsWait)) return false;
                 if(OM.DevOptn.iDispsrCnt==0){
                     Step.iToStart=0;
                     return true;
                 }


                 Step.iToStart++;
                 return false;

        case 12: IO_SetY(yWRK_LDispensor , true);
                 IO_SetY(yWRK_RDispensor , true);
                 m_tmLDisps.Clear();
                 m_tmRDisps.Clear();
                 Step.iToStart++;
                 return false;

        case 13: if(!m_tmLDisps.OnDelay(true, OM.DevOptn.iDispsrLTimer))return false;
                 if(!m_tmRDisps.OnDelay(true, OM.DevOptn.iDispsrRTimer))return false;
                 IO_SetY(yWRK_LDispensor , false);
                 IO_SetY(yWRK_RDispensor , false);

                 m_iShotCnt++;


                 if(OM.DevOptn.iDispsrCnt > m_iShotCnt){
                     Step.iToStart=12;
                     return false;
                 }
                 else{
                     m_iShotCnt    = 0;
                     Step.iToStart = 0 ;
                     return true ;
                 }
    }
}

bool CWork::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiRAL_ToStopTO);

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

        case 10: if(!MT_GetAlarm   (miWRK_XIndx) &&
                    !MT_GetNLimSnsr(miWRK_XIndx) &&
                    !MT_GetPLimSnsr(miWRK_XIndx) ) m_dLastIndPos = MT_GetCmdPos(miWRK_XIndx) + 10;   //10미리 더 가서 저장.
                 else                              m_dLastIndPos = GetMotrPos  (miWRK_XIndx , piWRK_XIndxWait) ;
                 MoveMotr(miWRK_YDsps , piWRK_YDspsWait);
                 MoveActr(aiWRK_LIdUpDn , ccBwd);
                 MoveActr(aiWRK_RIdUpDn , ccBwd);
                 IO_SetY(yWRK_LDispensor , false);
                 IO_SetY(yWRK_RDispensor , false);
                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveActr(aiWRK_LIdUpDn , ccBwd)) return false;
                 if(!MoveActr(aiWRK_RIdUpDn , ccBwd)) return false;
                 Step.iToStop=0;
                 return true ;
    }
}

//자제 투입.
bool CWork::CycleIn()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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

    if(!OM.MstOptn.bSkipOvldSnsr){
        if(IO_GetX(xWRK_IdxOvld)){
            EM_SetErr(eiWRK_IndxOvld);
            Step.iCycle = 0;
            return true;
        }
    }

    static double dPreTime = GetTickTime();
    double dCrntTime ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dCrntTime = GetTickTime();
                  m_dWorkCycleTime = dCrntTime - dPreTime ;
                  dPreTime = dCrntTime ;

                  MoveActr(aiWRK_LIdUpDn , ccBwd);
                  MoveActr(aiWRK_RIdUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRK_LIdUpDn , ccBwd)) return false;
                  if(!MoveActr(aiWRK_RIdUpDn , ccBwd)) return false;
                  MoveMotr(miWRK_XIndx , piWRK_XIndxWait);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxWait))return false ;
                  if(!DM.ARAY[riLWK].GetCntStat(csNone)) MoveActr(aiWRK_LIdUpDn , ccFwd);
                  if(!DM.ARAY[riRWK].GetCntStat(csNone)) MoveActr(aiWRK_RIdUpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!DM.ARAY[riLWK].GetCntStat(csNone) && !MoveActr(aiWRK_LIdUpDn , ccFwd)) return false;
                  if(!DM.ARAY[riRWK].GetCntStat(csNone) && !MoveActr(aiWRK_RIdUpDn , ccFwd)) return false;
                  MoveMotr(miWRK_XIndx , piWRK_XIndxWorkStt);
                  MoveMotr(miWRK_YDsps , piWRK_YDspsWork);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxWorkStt)) return false;
                  if(!MoveMotr(miWRK_YDsps , piWRK_YDspsWork   )) return false;
                  DM.ARAY[riLWK].SetStep(1);
                  Step.iCycle = 0;
                  return true;
    }
}

//작업 위치로 이동.
bool CWork::CycleSingleWrk()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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

    if(!OM.MstOptn.bSkipOvldSnsr){
        if(IO_GetX(xWRK_IdxOvld)){
            EM_SetErr(eiWRK_IndxOvld);
            Step.iCycle = 0;
            return true;
        }
    }

    int r1, c1, r2, c2;
    int iRightCol;
    static bool bLShot = false;
    static bool bRShot = false;
    bool bLeftWork;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRK_RIdUpDn , ccFwd);
                  MoveMotr(miWRK_YDsps , piWRK_YDspsWork);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWRK_RIdUpDn , ccFwd))return false;
                  if(!MoveMotr(miWRK_YDsps , piWRK_YDspsWork))return false;
                  MoveMotr(miWRK_XIndx , piWRK_XIndxRWork);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxRWork)) return false ;
                  if(!IO_GetX(xDSP_RQSnsr)){
                      EM_SetErr(eiDSP_RQSensor);
                      Step.iCycle = 0;
                      return true;
                  }
                  bLShot = false;
                  bRShot = false;

                  IO_SetY(yWRK_RDispensor , true);
                  m_tmRDisps.Clear();
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  13: if(!OM.CmnOptn.bIgnrDsprSnsr){
                      if(IO_GetXUp(xDSP_RDiscSnsr) || IO_GetX(xDSP_RDiscSnsr) || IO_GetXDn(xDSP_RDiscSnsr)) bRShot = true;
                  }
                  else{
                      bRShot = true;
                  }
                  if(!m_tmRDisps.OnDelay(true, OM.DevOptn.iDispsrRTimer))IO_SetY(yWRK_RDispensor , false);
                  if(!m_tmTemp.OnDelay(true, OM.DevOptn.iDispsrRTimer))return false;

                  FindChip(r1, c1, fcCol);
                  int iTemp = OM.DevInfo.iColCnt - c1;
                  if(iTemp % 2) FindChip(r2, c2, fcRowOdd );
                  else          FindChip(r2, c2, fcRowEven);
                  if(!bRShot) EM_SetErr(eiDSP_discharge);
                  else {DM.ARAY[riLWK].SetStat(r2, c2, csWork); }
                  Step.iCycle=0;
                  return true;
    }
}

//작업 위치로 이동.
bool CWork::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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

    if(!OM.MstOptn.bSkipOvldSnsr){
        if(IO_GetX(xWRK_IdxOvld)){
            EM_SetErr(eiWRK_IndxOvld);
            Step.iCycle = 0;
            return true;
        }
    }

    int r1, c1 , r2, c2;
    int iRightCol;
    int iTemp;
    static bool bLShot = false;
    static bool bRShot = false;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(m_iDisprCnt >= OM.CmnOptn.iDisprCleanCnt){
                      Step.iCycle = 50;
                      return false;
                  }
                  if(!DM.ARAY[riLWK].GetCntStat(csNone)) MoveActr(aiWRK_LIdUpDn , ccFwd);
                  if(!DM.ARAY[riRWK].GetCntStat(csNone)) MoveActr(aiWRK_RIdUpDn , ccFwd);
                  MoveMotr(miWRK_YDsps , piWRK_YDspsWork);
                  Step.iCycle++;
                  return false ;

        case  11: if(!DM.ARAY[riLWK].GetCntStat(csNone) && !MoveActr(aiWRK_LIdUpDn , ccFwd)) return false ;
                  if(!DM.ARAY[riRWK].GetCntStat(csNone) && !MoveActr(aiWRK_RIdUpDn , ccFwd)) return false ;
                  if(!MoveMotr(miWRK_YDsps , piWRK_YDspsWork))return false;
                  MoveMotr(miWRK_XIndx , piWRK_XIndxWork);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxWork)) return false ;
                  if(!IO_GetX(xDSP_RQSnsr)){
                      EM_SetErr(eiDSP_RQSensor);
                      Step.iCycle = 0;
                      return true;
                  }
                  bLShot = false;
                  bRShot = false;
                  FindChip(r1, c1 , fcCol);
                  if(OM.DevInfo.iColCnt%2 == 0) iRightCol = OM.DevInfo.iColCnt/2;
                  else                          iRightCol = OM.DevInfo.iColCnt/2 - 1;
                  if((!OM.MstOptn.bUsedSingleDisps &&  OM.MstOptn.bUsedDispsOptn &&
                       DM.ARAY[riLWK].GetStat(r1, c1-OM.MstOptn.iDispsOptn) == csUnkwn) ||
                     (!OM.MstOptn.bUsedSingleDisps && !OM.MstOptn.bUsedDispsOptn &&
                       DM.ARAY[riLWK].GetStat(r1, c1-iRightCol            ) == csUnkwn)){
                      IO_SetY(yWRK_LDispensor , true);
                      IO_SetY(yWRK_RDispensor , true);
                  }
                  else{
                      IO_SetY(yWRK_RDispensor , true);
                  }
                  m_tmLDisps.Clear();
                  m_tmRDisps.Clear();
                  m_tmTemp  .Clear();
                  m_iDisprCnt++;         //디스펜서 일정 횟수 되면 초기화.
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!OM.CmnOptn.bIgnrDsprSnsr){
                      if(IO_GetXUp(xDSP_LDiscSnsr) || IO_GetX(xDSP_LDiscSnsr) || IO_GetXDn(xDSP_LDiscSnsr)) bLShot = true;
                      if(IO_GetXUp(xDSP_RDiscSnsr) || IO_GetX(xDSP_RDiscSnsr) || IO_GetXDn(xDSP_RDiscSnsr)) bRShot = true;
                  }
                  else{
                      bLShot = true;
                      bRShot = true;
                  }
                  if(m_tmLDisps.OnDelay(true, OM.DevOptn.iDispsrLTimer))IO_SetY(yWRK_LDispensor , false);    //시간 되면 끄기
                  if(m_tmRDisps.OnDelay(true, OM.DevOptn.iDispsrRTimer))IO_SetY(yWRK_RDispensor , false);    //시간 되면 끄기
                  if(!m_tmTemp.OnDelay(true, OM.DevOptn.iDispsrSnsrTimer))return false;                                    // 시간 후 센서 감지 시간.

                  FindChip(r1, c1, fcCol);
                  iTemp = OM.DevInfo.iColCnt - c1;
                  if(iTemp % 2) FindChip(r2, c2, fcRowOdd );
                  else          FindChip(r2, c2, fcRowEven);


                  if(OM.DevInfo.iColCnt%2 == 0) iRightCol = OM.DevInfo.iColCnt/2;
                  else                          iRightCol = OM.DevInfo.iColCnt/2 - 1;
                  if(!OM.MstOptn.bUsedSingleDisps && OM.MstOptn.bUsedDispsOptn &&
                      DM.ARAY[riLWK].GetStat(r2, c2-OM.MstOptn.iDispsOptn) == csUnkwn ){
                      if(!bLShot || !bRShot) EM_SetErr(eiDSP_discharge);
                      else {
                          DM.ARAY[riLWK].SetStat(r2, c2                      , csWork);
                          DM.ARAY[riLWK].SetStat(r2, c2-OM.MstOptn.iDispsOptn, csWork);
                      }
                  }
                  else if(!OM.MstOptn.bUsedSingleDisps && !OM.MstOptn.bUsedDispsOptn &&
                            DM.ARAY[riLWK].GetStat(r2, c2-iRightCol ) == csUnkwn ){
                      if(!bLShot || !bRShot) EM_SetErr(eiDSP_discharge);
                      else {
                          DM.ARAY[riLWK].SetStat(r2, c2                      , csWork);
                          DM.ARAY[riLWK].SetStat(r2, c2-iRightCol            , csWork);
                      }
                  }
                  else {
                      if(!bRShot) EM_SetErr(eiDSP_discharge);
                      else        DM.ARAY[riLWK].SetStat(r2, c2, csWork);
                  }
                  Step.iCycle++;
                  return false;

        case  14: FindChip(r1 , c1 , fcCol);
                  if(OM.MstOptn.bUsedSingleDisps && c1 < OM.DevInfo.iColCnt / 2){
                      Step.iCycle = 20;
                      return false;
                  }
                  else {
                      Step.iCycle=0;
                      return true;
                  }

        //Single일때 오른쪽으로 넘기기 위해서.
        case  20: MoveActr(aiWRK_LIdUpDn , ccBwd);
                  MoveActr(aiWRK_RIdUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  21: if(!MoveActr(aiWRK_LIdUpDn , ccBwd))return false;
                  if(!MoveActr(aiWRK_RIdUpDn , ccBwd))return false;
                  MoveMotr(miWRK_XIndx , piWRK_XIndxWait);
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxWait))return false;
                  DM.ARAY[riLWK].SetStep(2);
                  Step.iCycle = 0;
                  return true;

        case  50: MoveMotr(miWRK_YDsps , piWRK_YDspsWait);
                  Step.iCycle++;
                  return false;

        case  51: if(!MoveMotr(miWRK_YDsps , piWRK_YDspsWait)) return false;
                  IO_SetY(yWRK_LDispensor , true);
                  IO_SetY(yWRK_RDispensor , true);
                  m_tmLDisps.Clear();
                  m_tmRDisps.Clear();
                  Step.iCycle++;
                  return false;

        case  52: if(!m_tmLDisps.OnDelay(true , OM.DevOptn.iDispsrLTimer)) return false;
                  if(!m_tmRDisps.OnDelay(true , OM.DevOptn.iDispsrRTimer)) return false;
                  IO_SetY(yWRK_LDispensor , false);
                  IO_SetY(yWRK_RDispensor , false);
                  m_iDisprCnt = 0;
                  Step.iCycle = 10;
                  return false;

    }
}


//작업 위치로 이동.
bool CWork::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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

    if(!OM.MstOptn.bSkipOvldSnsr){
        if(IO_GetX(xWRK_IdxOvld)){
            EM_SetErr(eiWRK_IndxOvld);
            Step.iCycle = 0;
            return true;
        }
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!OM.MstOptn.bUsedSingleDisps){
                      if(!DM.ARAY[riLWK].GetCntStat(csNone)) MoveActr(aiWRK_LIdUpDn , ccFwd);
                      if(!DM.ARAY[riRWK].GetCntStat(csNone)) MoveActr(aiWRK_RIdUpDn , ccFwd);
                  }
                  else {
                      MoveActr(aiWRK_RIdUpDn , ccFwd);
                  }
                  Step.iCycle++;
                  return false;

        case  11: if(!OM.MstOptn.bUsedSingleDisps){
                      if(!DM.ARAY[riLWK].GetCntStat(csNone) && !MoveActr(aiWRK_LIdUpDn , ccFwd)) return false;
                      if(!DM.ARAY[riRWK].GetCntStat(csNone) && !MoveActr(aiWRK_RIdUpDn , ccFwd)) return false;
                  }
                  else {
                      if(!MoveActr(aiWRK_RIdUpDn , ccFwd)) return false;
                  }
                  MoveMotr(miWRK_XIndx , piWRK_XIndxOut);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxOut)) return false;
                  MoveActr(aiWRK_LIdUpDn , ccBwd);
                  MoveActr(aiWRK_RIdUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiWRK_LIdUpDn , ccBwd)) return false;
                  if(!MoveActr(aiWRK_RIdUpDn , ccBwd)) return false;
                  MoveMotr(miWRK_XIndx , piWRK_XIndxWait);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_XIndx , piWRK_XIndxWait)) return false;
                  if(!OM.MstOptn.bUsedSingleDisps){
                      if(!DM.ARAY[riRWK].GetCntStat(csNone)){
                          DM.ShiftArrayData(riRWK, riPSB);
                      }
                      if(!DM.ARAY[riLWK].GetCntStat(csNone)){
                          DM.ShiftArrayData(riLWK, riRWK);
                      }
                  }
                  else {
                      DM.ShiftArrayData(riLWK, riPSB);
                  }
                  Step.iCycle = 0;
                  return true;
    }
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
    if(!MT_GetStop(miWRK_XIndx)) return false ;
    if(!MT_GetStop(miWRK_YDsps)) return false ;

    if(!AT_Done(aiWRK_LIdUpDn)) return false ;
    if(!AT_Done(aiWRK_RIdUpDn)) return false ;

    return true ;
}

void CWork::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Work.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIndPos  " , m_dLastIndPos   );
        UserINI.Load(sPath, "Member" , "m_iDisprCnt    " , m_iDisprCnt     );
        if(m_dLastIndPos > MT_GetMaxPos(miWRK_XIndx)) m_dLastIndPos = MT_GetMaxPos(miWRK_XIndx);
    }

    else {
        UserINI.ClearFile(sPath) ;
        if(m_dLastIndPos > MT_GetMaxPos(miWRK_XIndx)) m_dLastIndPos = MT_GetMaxPos(miWRK_XIndx);
        UserINI.Save(sPath, "Member" , "m_dLastIndPos  " , m_dLastIndPos   );
        UserINI.Save(sPath, "Member" , "m_iDisprCnt    " , m_iDisprCnt     );

    }

}
//---------------------------------------------------------------------------

