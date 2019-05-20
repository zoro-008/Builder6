//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Laser.h"
#include "PreBuff.h"
#include "RunThread.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CLaser LSR;

CLaser::CLaser(void)
{

}

CLaser::~CLaser (void)
{

}

void CLaser::Init()
{
    m_sPartName = "Laser " ;
    Reset();
    Load(true);

    m_bTestMark = false;
    m_bAutoRun  = false;

    InitCycleName();
}
void CLaser::Close()
{
    Load(false);
}

typedef void ( *Func )();
void ThreadLaserReset()
{
    static bool bAlreadyDoing = false ;
    CDelayTimer Timer ;

    if(bAlreadyDoing) return ;



    bAlreadyDoing = true ;

    int iStep    = 10 ;
    int iPreStep = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiLSR_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Laser Communction",sTemp.c_str());
            IO_SetY(yLSC_Reset , false) ;
            iStep = 0 ;
            bAlreadyDoing = false ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Laser Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Laser Communction", sTemp.c_str());
                      IO_SetY(yLSC_Reset , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;

            case  10: IO_SetY(yLSC_Reset , false) ;

                      iStep++;
                      continue ;

            case  11: IO_SetY(yLSC_Reset , true ) ;
                      iStep++;
                      continue ;

            case  12: if(!IO_GetX(xLSC_Ready)) continue ;
                      IO_SetY(yLSC_Reset , false ) ;
                      iStep = 0 ;
                      bAlreadyDoing = false ;
                      return ;
        }
    }

}

void ThreadLaserJobChange()
{

    CDelayTimer Timer ;


    int iStep    = 10 ;
    int iPreStep = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiLSR_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Laser Communction",sTemp.c_str());
            IO_SetY(yLSC_DataRead , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Laser Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Laser Communction", sTemp.c_str());
                      IO_SetY(yLSC_Reset , false) ;
                      iStep = 0 ;
                      return ;

            case  10: if(IO_GetX(xLSC_DataReadComp)) {
                          EM_SetErrMsg(eiLSR_CommErr , "DataReadComp is High");
                          return ;
                      }

                      IO_SetY(yLSC_DataRead , true) ;

                      iStep++;
                      continue ;

            case  11: if(!IO_GetX(xLSC_DataReadComp)) continue ;

                      IO_SetY(yLSC_DataRead , false ) ;
                      iStep++;
                      continue ;

            case  12: if(IO_GetX(xLSC_DataReadComp)) continue ;
                      iStep = 0 ;
                      return ;
        }
    }
}
bool CLaser::LaserJobChange()
{

    if(!SaveLaserDevice(OM.GetCrntDev())) {
        FM_MsgOk("ERR","Laser Jobfile 쓰기 실패");
        return false ;
    }
    CRunThread<Func> * func = new CRunThread<Func>(&ThreadLaserJobChange) ;

}


void CLaser::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    IO_SetY(yVSC_AlnStart   , false);
    IO_SetY(yLSC_MarkEnable , false);
    IO_SetY(yLSC_DataRead   , false);
    IO_SetY(yLSC_Reset      , false);
    IO_SetY(yLSC_Start      , false);

    if(!IO_GetX(xLSC_Ready)&&!OM.CmnOptn.bSkipLaser) { //LASER Not Ready Condition
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadLaserReset) ;
    }

    //if((!IO_GetX(xVSC_OriReady) && !OM.CmnOptn.bSkipOriVisn) ||
    //   (!IO_GetX(xVSC_AlnReady) && !OM.CmnOptn.bSkipAlnVisn) ||
    //   (!IO_GetX(xVSC_MrkReady) && OM.CmnOptn.iMarkInsp != moNone) ){
    //    CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnReset) ;
    //}


    /*
    if((!IO_GetX(xVSC_OriBusy) && !OM.CmnOptn.bSkipOriVisn) ||
       (!IO_GetX(xVSC_AlnBusy) && !OM.CmnOptn.bSkipAlnVisn) ||
       (!IO_GetX(xVSC_MrkBusy) &&  OM.CmnOptn.iMarkInsp != moNone) ){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnReset) ;
    }
    */



}

void CLaser::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

bool CLaser::FindChip(int &r , int &c )
{
    r = -1 ;
    c = -1 ;

    return (c > -1 && c > -1);
}

double CLaser::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    if(_iMotr == miLSR_YAlVs){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                   ); break ;
            case piLSR_YAlVsWait  : dPos = PM.GetValue (_iMotr , pvLSR_YAlVsWait ); break ;
            case piLSR_YAlVsWork  : dPos = PM.GetValue (_iMotr , pvLSR_YAlVsWork ); break ;
            case piLSR_YAlVsCal   : dPos = PM.GetValue (_iMotr , pvLSR_YAlVsCal  ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CLaser::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miLSR_YAlVs){
        //if(IO_GetX(!xLSC_Ready) {sMsg = "Laser is Working"; bRet = false ;} 상황보자...
        switch(_iPstnId) {
            default              :  break ;
            case piLSR_YAlVsWait :  break ;
            case piLSR_YAlVsWork :  break ;
            case piLSR_YAlVsCal  :  break ;
        }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLaser::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg ;

    //if(aiPSB_RailOpCl){}
    //else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    //}

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CLaser::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLaser::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CLaser::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiLSR_HomeTo);
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

        case 10: MT_SetHomeEnd(miLSR_YAlVs , false);
                 MT_DoHome(miLSR_YAlVs);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miLSR_YAlVs)) return false;
                 MT_GoAbsMan(miLSR_YAlVs, PM.GetValue(miLSR_YAlVs , pvLSR_YAlVsWait));
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miLSR_YAlVs, PM.GetValue(miLSR_YAlVs , pvLSR_YAlVsWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CLaser::Autorun(void) //오토런닝시에 계속 타는 함수.
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;
//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail
    if (Step.iSeq == 0) {           
        if(Stat.bReqStop)return false ;

        bool isCycleWork =  DM.ARAY[riLSR].CheckAllStat(csMask) && AT_GetCmd(aiRAL_LStnUpDn)==ccFwd;

        bool isCycleEnd   = DM.ARAY[riLSR].CheckAllStat(csNone) && PRB.GetWorkEnd();

        //Unknown Strip
        /*
        if(isCycleWork){
            if (IO_GetX(xRAL_LPkg3) && DM.ARAY[riLSR].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riLSR].GetName().c_str()) ;

            //Dissapear Strip
            if (!OM.MstOptn.bDryRun) {
                if (!IO_GetX(xRAL_LPkg3) && !DM.ARAY[riLSR].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riLSR].GetName().c_str()) ;
            }
        }
        */
        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleWork ) {Trace(m_sPartName.c_str(),"CycleWork Stt"); Step.iSeq = scWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                   return false ;
        case scWork  : if(CycleWork()){ Trace(m_sPartName.c_str(),"CycleWork End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CLaser::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CLaser::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLaser::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                 

                 IO_SetY(yVSC_AlnStart  , false);
                 IO_SetY(yVSC_Spear2    , false);

                 IO_SetY(yLSC_Start     , false);
                 IO_SetY(yLSC_MarkEnable, false);
                 IO_SetY(yLSC_DataRead  , false);
                 IO_SetY(yLSC_Reset     , false);

                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                 if(!OM.CmnOptn.bSkipLaser &&!IO_GetX(xLSC_Ready   )) EM_SetErrMsg(eiLSR_CommErr , "Laser Not Ready");
                 if(!OM.CmnOptn.bSkipAlnVisn && !IO_GetX(xVSC_AlnReady)) {
                     EM_SetErrMsg(eiVSN_NotReady , "Align Vision");
                     Step.iToStart = 0 ;
                     return true ;
                 }





                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLaser::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiLSR_ToStopTO);

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

        case 10: MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);

                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CLaser::CycleVisn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLSR_CycleTO);
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

    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miLSR_YAlVs , piLSR_YAlVsWork);
                  IO_SetY(yVSC_AlnStart , false);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWork)) return false ;

                  if(IO_GetX(xVSC_AlnBusy)) {
                      EM_SetErrMsg(eiVSN_TimeOut , "Align Vision");
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  IO_SetY(yVSC_AlnStart , true);

                  Step.iCycle++;
                  return false;

        case  12: if(!IO_GetX(xVSC_AlnBusy)) return false ;
                  IO_SetY(yVSC_AlnStart , false);
                  Step.iCycle++;
                  return false ;

        case  13: if(IO_GetX(xVSC_AlnBusy)) return false ; //Inspection End

                  if(!IO_GetX(xVSC_AlnRslt)){
                      EM_SetErrMsg(eiVSN_NotReady , "Align Vision");
                  }
                  MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                  Step.iCycle=0;
                  return true ;
    }
}

//작업 위치로 이동.
bool CLaser::CycleLaser()
{
//Check Cycle Time Out.
    AnsiString sTemp ;

    int iTimeOutTime = Step.iCycle == 14 ? 30000 : 5000 ;

    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iTimeOutTime )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLSR_CycleTO);
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

    int r,c ;
    String sMark;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: //일단 안씀.
                  Step.iCycle = 0 ;
                  return true ;

                  if(m_bManMark) {//오퍼화면에서 메뉴얼 마킹 체크 되었을때.
                      sMark = m_sManMark ;
                  }
                  else { //정상 동작시에
                      sMark = DM.ARAY[riLSR].GetLotNo() ;
                  }

                  if(DM.ARAY[riLSR].GetLotNo() != m_sLaserWorkLotNo && DM.ARAY[riLSR].GetLotNo() != ""){ //전에 날렸던 데이터랑 지금 랏이 달라졌으면.
                      if(IO_GetX(xLSC_DataReadComp)){
                          EM_SetErrMsg(eiLSR_CommErr,"DataReadComp is true");
                      }
                          //SaveLaserLotNo(DM.ARAY[riLSR].GetLotNo());
                          SaveLaserLotNo(sMark);
                          m_sLaserWorkLotNo = DM.ARAY[riLSR].GetLotNo() ;
                      //랏 넘버 바뀌는 패턴.
                      IO_SetY(yLSC_DataRead , true);
                  }
                  else { //계속 같은 랏이면
                      Step.iCycle= 13;
                      return false ;
                  }

                  Step.iCycle++;
                  return false ;

        case  11: if(!IO_GetX(xLSC_DataReadComp)) return false ;
                  IO_SetY(yLSC_DataRead , false);
                  Step.iCycle++;
                  return false ;

        case  12: if(IO_GetX(xLSC_DataReadComp)) return false ;
                  Step.iCycle++;
                  return false ;

        //위에서 씀 번호 주의
        case  13: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                  if(!IO_GetX(xLSC_DataReady   )) return false ;
                  //if(!IO_GetX(xLSC_VisnDataRead)) return false ; //비젼 연동 작업일때는 확인 해야한다. 비전 데이터 레이저에서 읽어 갔는지.
                  //xLSC_DataReady 이것에 xLSC_VisnDataRead이 수렴...
                  IO_SetY(yLSC_MarkEnable , true);
                  IO_SetY(yLSC_Start , true);

                  Step.iCycle++;
                  return false;

        //타임아웃 조건에서씀 번호 주의
        case  14: if(IO_GetX(xLSC_DataReady)) return false ;
                  //if(IO_GetX(xLSC_VisnDataRead)) return false ;
                  IO_SetY(yLSC_MarkEnable , false);
                  IO_SetY(yLSC_Start , false);
                  if(!DM.ARAY[riLSR].CheckAllStat(csNone)){
                      DM.ARAY[riLSR].SetStat(csWork);
                  }

                  Step.iCycle=0;
                  return true;

        /*
        case  10: MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;

                  if(!IO_GetX(xLSC_Ready)) {
                      EM_SetErrMsg(eiLSR_CommErr , "Laser Not Ready" );
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  IO_SetY(yLSC_MarkEnable , true);
                  IO_SetY(yLSC_Start , true);

                  Step.iCycle++;
                  return false;

        case  12: if(IO_GetX(xLSC_Ready)) return false ;
                  if(!DM.ARAY[riLSR].CheckAllStat(csNone)) DM.ARAY[riLSR].SetStat(csWork);
                  Step.iCycle++;
                  return false ;

        case  13: if(!IO_GetX(xLSC_Ready)) return false ;
                  IO_SetY(yLSC_Start , false);
                  IO_SetY(yLSC_MarkEnable , false);

                  Step.iCycle=0;
                  return true;
        */
    }
}

bool CLaser::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iTimeOutTime = Step.iCycle == 19 ? 30000 : 5000 ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iTimeOutTime )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiLSR_CycleTO);
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

    int r,c ;

    String sMark ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yVSC_AlnStart , false);

                  if(OM.CmnOptn.bSkipAlnVisn) {
                      MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                      Step.iCycle = 15 ;
                      return false;
                  }

                  MoveMotr(miLSR_YAlVs , piLSR_YAlVsWork);


                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWork)) return false ;

                  //if(IO_GetX(xVSC_AlnBusy)) {
                  //    EM_SetErrMsg(eiVSN_NotReady , "Align Vision");
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}
                  IO_SetY(yVSC_AlnStart , true);

                  Step.iCycle++;
                  return false;

        case  12: if(!IO_GetX(xVSC_AlnBusy)) return false ;
                  IO_SetY(yVSC_AlnStart , false);
                  Step.iCycle++;
                  return false ;

        case  13: if(IO_GetX(xVSC_AlnBusy)) return false ; //Inspection End
                  MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                  if(!IO_GetX(xVSC_AlnRslt)){
                      EM_SetErrMsg(eiVSN_InspNG , "Align Vision");
                      Step.iCycle=0;
                      return true ;
                  }


                  Step.iCycle=15;
                  return false ;



        //랏넘버 바뀔때 딜레이 때문에.
        //case  14: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false




        //위에서 번호씀...
        case  15:
                  if(OM.CmnOptn.bSkipLaser) {  //레이져 스킵 조건.
                      DM.ARAY[riLSR].SetStat(csWork);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(m_bManMark) {//오퍼화면에서 메뉴얼 마킹 체크 되었을때.
                      sMark = m_sManMark ;
                  }
                  else { //정상 동작시에
                      sMark = DM.ARAY[riLSR].GetLotNo() ;
                  }
                  if(!OM.MstOptn.bAllStrpSendLsrJob){
                      if(sMark != m_sLaserWorkLotNo && sMark != ""){ //전에 날렸던 데이터랑 지금 랏이 달라졌으면.
                          if(IO_GetX(xLSC_DataReadComp)){
                              EM_SetErrMsg(eiLSR_CommErr,"DataReadComp is true");
                          }
                          SaveLaserLotNo(sMark);
                          m_sLaserWorkLotNo = sMark ;
                          //랏 넘버 바뀌는 패턴.
                          IO_SetY(yLSC_DataRead , true);
                      }
                      else { //계속 같은 랏이면
                          Step.iCycle= 18;
                          return false ;
                      }
                  }
                  else{
                      if(IO_GetX(xLSC_DataReadComp)){
                          EM_SetErrMsg(eiLSR_CommErr,"DataReadComp is true");
                      }
                      SaveLaserLotNo(sMark);
                      m_sLaserWorkLotNo = sMark ;
                      IO_SetY(yLSC_DataRead , true);
                  }                                   // 모든 스트립 레이저 데이터 전송 해주는 파트...JH

                  Step.iCycle++;
                  return false ;

        case  16: if(!IO_GetX(xLSC_DataReadComp)) return false ;
                  IO_SetY(yLSC_DataRead , false);
                  Step.iCycle++;
                  return false ;

        case  17: if(IO_GetX(xLSC_DataReadComp)) return false ;
                  Step.iCycle++;
                  return false ;

        //위에서 씀 번호 주의
        case  18: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                  if(!IO_GetX(xLSC_DataReady   )) return false ;
                  //if(!IO_GetX(xLSC_VisnDataRead)) return false ; //비젼 연동 작업일때는 확인 해야한다. 비전 데이터 레이저에서 읽어 갔는지.
                  //xLSC_DataReady 이것에 xLSC_VisnDataRead이 수렴...
                  IO_SetY(yLSC_MarkEnable , true);
                  IO_SetY(yLSC_Start , true);

                  Step.iCycle++;
                  return false;

        //타임아웃에서 번호씀 주의.
        case  19: if(IO_GetX(xLSC_DataReady)) return false ;
                  //if(IO_GetX(xLSC_VisnDataRead)) return false ;
                  IO_SetY(yLSC_MarkEnable , false);
                  IO_SetY(yLSC_Start , false);
                  if(!DM.ARAY[riLSR].CheckAllStat(csNone)){
                      DM.ARAY[riLSR].SetStat(csWork);
                  }
        //          Step.iCycle++;
        //          return false ;
        //
        //case  20: if(!IO_GetX(xLSC_DataReady)) return false ;
        //


                  Step.iCycle=0;
                  return true;
    }
}

void CLaser::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLaser::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CLaser::CheckStop()
{
    if(!MT_GetStop(miLSR_YAlVs)) return false ;

    //if(!AT_Done(aiLSR_EpxFwBw)) return false ;

    return true ;
}

void CLaser::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Laser.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLaserWorkLotNo" , m_sLaserWorkLotNo);
    }

    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLaserWorkLotNo" , m_sLaserWorkLotNo);
    }

}

//Device Set Save시에 포멧이 잘 맞는지 확인.
bool CLaser::CheckFormat(AnsiString _sFormat)
{
    AnsiString sFormat = _sFormat ;

    int iLeftCnt = 0 ;
    int iRightCnt = 0 ;
    int iBlockCnt = 0 ;
    AnsiString sTemp ;
    int iTemp ;

    while(sFormat.Pos("[")){

        sFormat.Delete(sFormat.Pos("[") , 1);
        iLeftCnt++;
    }

    while(sFormat.Pos("]")){

        sFormat.Delete(sFormat.Pos("]") , 1);
        iRightCnt++;
    }

    if(iLeftCnt != iRightCnt) return false ; //왼괄호 수와 오른괄호 수가 맞지않음.

    sFormat = _sFormat ;

    while(sFormat.Pos("[") ||sFormat.Pos("]")){
        sTemp = sFormat.SubString(sFormat.Pos("[")+1 , sFormat.Pos("]") - sFormat.Pos("[")-1);
        iTemp = sTemp.ToIntDef(-9999) ;
        if(iTemp < 0) return false ; //가로 안에 내용이 정수형이 아니거나 음수가 들어가 있다.
        sFormat.Delete(sFormat.Pos("[") , sFormat.Pos("]") - sFormat.Pos("[") + 1);
    }
    iBlockCnt = 1 ;
    while(sFormat.Pos("@")){
        sFormat.Delete(sFormat.Pos("@") , 1);
        iBlockCnt++;
    }

    if(sFormat != "") return false ; //괄호 밖에 있는 글짜 발견.

    return true ;
}

AnsiString CLaser::GetLaserDataFromLotNot(AnsiString _sFormat , AnsiString _sLotNo)
{
    AnsiString sLotNo  = _sLotNo  ;
    AnsiString sFormat = _sFormat ;
    AnsiString sTemp ;
    int        iTemp ;
    int        iBlockCnt = 1 ;

    AnsiString sLaser ;

    if(!CheckFormat(sFormat)) { EM_SetErr(eiLSR_DislikeFormat); return ""; }//에러


    while(sFormat.Pos("[") ||sFormat.Pos("]")){
        if(sFormat.SubString(1,1) == "@"){
            sFormat.Delete(1 , 1);
            sLaser+="@" ;
            iBlockCnt++;
            continue ;
        }
        sTemp = sFormat.SubString(sFormat.Pos("[")+1 , sFormat.Pos("]") - sFormat.Pos("[")-1);
        iTemp = sTemp.ToIntDef(-1);
        sLaser += sLotNo.SubString(iTemp+1 , 1) ;

        sFormat.Delete(sFormat.Pos("[") , sFormat.Pos("]") - sFormat.Pos("[") + 1);
    }

    sLaser = AnsiString(iBlockCnt) + "@" + sLaser ;

    return sLaser ;
}



void CLaser::SaveLaserLotNo(AnsiString _sLotNo)
{

    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath     = OM.MstOptn.sLaserDataFile ;
    AnsiString  sFormat   = OM.DevOptn.sLaserLotFormat;
    AnsiString  sLaserData = _sLotNo;

    if(!m_bTestMark || m_bAutoRun){
        sLaserData  = GetLaserDataFromLotNot(sFormat , _sLotNo);
        m_bTestMark = false;
        m_bAutoRun  = false;
    }
    //ex) LotNo : "20140401_HANRA" 이고 '_'를 제외하고 (연도,날짜,메이커) 3블럭 으로 작업.할때
    //[0][1][2][3]@[4][5][6][7]@[9][10][11][12][13]


    try{
        UserINI.Save(sPath, "MarkingData" , "LotNo" , sLaserData );
    }
    catch(...){
        FM_MsgOk("ERR","Laser LotNo File 쓰기 실폐");
    }

}

bool CLaser::SaveLaserDevice(AnsiString _sDevice)
{

    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath     = OM.MstOptn.sLaserDataFile ;

    try{
        UserINI.Save(sPath, "Device" , "Name" , _sDevice );
    }
    catch(...){
        FM_MsgOk("ERR","Laser JobFile File 쓰기 실폐");
    }

    return true ;
}



//---------------------------------------------------------------------------
