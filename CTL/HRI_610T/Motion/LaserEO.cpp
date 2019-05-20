//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LaserEO.h"
#include "PreBuff.h"
#include "RunThread.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "UserFile.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Rs232Laser.h"
#include <math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)
#define VISN_FOLDER "c:\\data"

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

    AnsiString sLaserMsg = "";

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiLSR_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Laser Communction",sTemp.c_str());
            //IO_SetY(yLSC_Reset , false) ;
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
                      //IO_SetY(yLSC_Reset , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;

            case  10: if(!OM.CmnOptn.bSkipLaser){
                          sLaserMsg = "";
                          Rs232Laser.SendMsg(eoRDY, sLaserMsg);
                          Rs232Laser.ClearReadMsg();
                          iStep++;
                          continue ;
                      }
                      iStep = 0;
                      return ;



            case  11: //if(!Rs232Laser.ReadMsg().Pos((char)0x03)) continue ;
                      //if(Rs232Laser.ReadMsg().Pos("NAK") != 0) {
                      //    EM_SetErrMsg(eiLSR_CommErr, "Laser Comm Error : Reset");
                      //    Trace("Laser", "Rs232Laser.ReadMsg() != NAK");
                      //    iStep = 0;
                      //    return ;
                      //}
                      //EO���� �ȳ�����..
                      if(!IO_GetX(xLSC_Ready)) continue ;
                      iStep++;
                      continue ;

            case  12: iStep = 0 ;
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

    AnsiString sLaserMsg = "";

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiLSR_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Laser Communction",sTemp.c_str());
            IO_SetY(yLSC_Start , false) ;
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
                      //IO_SetY(yLSC_Reset , false) ;
                      iStep = 0 ;
                      return ;

            case  10: if(!OM.CmnOptn.bSkipLaser){
                          sLaserMsg = OM.GetCrntDev() + ".mrk";
                          Rs232Laser.SendMsg(eoCHM, sLaserMsg);
                          Rs232Laser.ClearReadMsg();
                          iStep++;
                          continue ;
                      }
                      iStep = 0;
                      return ;

            case  11: if(!Rs232Laser.ReadMsg().Pos((char)0x03)) continue;
                      if(Rs232Laser.ReadMsg().Pos("NAK") != 0) {
                          EM_SetErrMsg(eiLSR_CommErr, "Laser Comm Error : Device Change");
                          Trace("Laser", "Rs232Laser.ReadMsg() != NAK");
                          iStep = 0;
                          return ;
                      }
                      iStep++;
                      continue ;

            case  12:
                      iStep = 0 ;
                      return ;
        }
    }
}
bool CLaser::LaserJobChange(AnsiString _sDevName)
{

    if(!SaveLaserDevice(_sDevName)) {
        FM_MsgOk("ERR","Laser Jobfile ���� ����");
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
    IO_SetY(yLSC_Start      , false);
    //IO_SetY(yLSC_DataRead   , false);
    //IO_SetY(yLSC_Reset      , false);
    //IO_SetY(yLSC_Start      , false);

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
        //if(IO_GetX(!xLSC_Ready) {sMsg = "Laser is Working"; bRet = false ;} ��Ȳ����...
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
    //    sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
    //    bRet = false ;
    //}

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CLaser::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CLaser::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
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

bool CLaser::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;
//�������� �����˻� ���������� �����˻�
//Unkwn -> Mask     Work       Good
//                             Fail
    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        if(m_tmSPM.OnDelay(true, 60000)){
            m_bSPMClear = true;
            m_tmSPM.Clear();
        }

        bool isCycleWork =  DM.ARAY[riLSR].CheckAllStat(csMask) && AT_GetCmd(aiRAL_LStnUpDn)==ccFwd && !OM.CmnOptn.bSkipLaser;

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

bool CLaser::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CLaser::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLaser::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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
                 //IO_SetY(yLSC_MarkEnable, false);
                 //IO_SetY(yLSC_DataRead  , false);
                 //IO_SetY(yLSC_Reset     , false);

                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                 if(!OM.CmnOptn.bSkipLaser && !IO_GetX(xLSC_Ready   )) EM_SetErrMsg(eiLSR_CommErr , "Laser Not Ready");
                 if(!OM.CmnOptn.bSkipAlnVisn && !IO_GetX(xVSC_AlnReady)) {
                     EM_SetErrMsg(eiVSN_NotReady , "Align Vision");
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLaser::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

//�۾� ��ġ�� �̵�.
bool CLaser::CycleLaser()
{
//Check Cycle Time Out.
    AnsiString sTemp ;

    //int iTimeOutTime = Step.iCycle == 14 ? 30000 : 5000 ;
    int iTimeOutTime = 20000;

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

    AnsiString sFormat, sLaserData;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: //�ϴ� �Ⱦ�.
                  Step.iCycle = 0 ;
                  return true ;

                  if(m_bManMark) {//����ȭ�鿡�� �޴��� ��ŷ üũ �Ǿ�����.
                      sMark = m_sManMark ;
                  }
                  else { //���� ���۽ÿ�
                      sMark = DM.ARAY[riLSR].GetLotNo() ;
                  }

                  if(DM.ARAY[riLSR].GetLotNo() != m_sLaserWorkLotNo && DM.ARAY[riLSR].GetLotNo() != ""){ //���� ���ȴ� �����Ͷ� ���� ���� �޶�������.
                      if(!IO_GetX(xLSC_Ready)){
                          EM_SetErrMsg(eiLSR_CommErr,"Laser Not Ready");
                      }
                      if(!SaveLaserLotNo(sMark)){//FM_MsgOk("ERR",);
                          EM_SetErrMsg(eiLSR_CommErr , "Laser LotNo File writing failed!");
                          Step.iCycle = 0 ;
                          return true ;
                      }

                      m_sLaserWorkLotNo = DM.ARAY[riLSR].GetLotNo() ;
                      //�� �ѹ� �ٲ�� ����.
                      sFormat = OM.DevOptn.sLaserLotFormat;
                      sLaserData  = GetLaserDataFromLotNot(sFormat , m_sLaserWorkLotNo);

                      Rs232Laser.SendMsg(eoDAT, sLaserData);
                  }
                  else { //��� ���� ���̸�
                      Step.iCycle= 13;
                      return false ;
                  }
                  Rs232Laser.ClearReadMsg();
                  Step.iCycle++;
                  return false ;

        case  11: if(!Rs232Laser.ReadMsg().Pos((char)0x03)) return false;
                  if(Rs232Laser.ReadMsg().Pos("NAK") != 0) {
                      EM_SetErrMsg(eiLSR_CommErr, "Laser Comm Error : Device Change");
                      Trace("Laser", "Rs232Laser.ReadMsg() != NAK");
                      Step.iCycle = 0;
                      return false ;
                  }
                  Step.iCycle++;
                  return false ;

        case  12: //if(IO_GetX(xLSC_DataReadComp)) return false ;
                  Step.iCycle++;
                  return false ;

        //������ �� ��ȣ ����
        case  13: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                  //if(!IO_GetX(xLSC_DataReady   )) return false ;

                  //if(!IO_GetX(xLSC_VisnDataRead)) return false ; //���� ���� �۾��϶��� Ȯ�� �ؾ��Ѵ�. ���� ������ ���������� �о� ������.
                  //xLSC_DataReady �̰Ϳ� xLSC_VisnDataRead�� ����...
                  //IO_SetY(yLSC_MarkEnable , true);
                  IO_SetY(yLSC_Start , true);

                  Step.iCycle++;
                  return false;

        //Ÿ�Ӿƿ� ���ǿ����� ��ȣ ����
        case  14: //if(IO_GetX(xLSC_DataReady)) return false ;
                  //if(IO_GetX(xLSC_VisnDataRead)) return false ;
                  //IO_SetY(yLSC_MarkEnable , false);
                  IO_SetY(yLSC_Start , false);
                  if(!DM.ARAY[riLSR].CheckAllStat(csNone)){
                      DM.ARAY[riLSR].SetStat(csWork);
                  }

                  Step.iCycle=0;
                  return true;
    }
}

bool CLaser::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    //int iTimeOutTime = Step.iCycle == 19 ? 30000 : 5000 ;
    int iTimeOutTime = 30000;
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

    double dX, dY, dT;
    double dLaserX, dLaserY, dLaserT;
    AnsiString sLaserMsg;
    String sMark ;
    AnsiString sFormat, sLaserData;

    if(m_bSPMClear) {
        m_iSPMCnt = m_iPreSPMCnt;
        m_iPreSPMCnt = 0;
        m_bSPMClear = false;
    }

    double TX, TY;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


                  //�ϴ� ������ ��ŷ������ �ٲ������ �̸� ���� ���´�.
                          //������ ��ȣ��...
        case  10: //if(OM.CmnOptn.bSkipLaser){
                  //    DM.ARAY[riLSR].SetStat(csWork);
                  //    Step.iCycle = 13 ;
                  //    return true ;
                  //}


                  //������ ��ŷ ������ �ľ�
                  if(m_bManMark) {//����ȭ�鿡�� �޴��� ��ŷ üũ �Ǿ�����.
                      sMark = m_sManMark ;
                  }
                  else { //���� ���۽ÿ�
                      sMark = DM.ARAY[riLSR].GetLotNo() ;

                  }

                  Trace("PRE LASER MASKING DATA", m_sLaserWorkLotNo.c_str());

                  if(!OM.MstOptn.bAllStrpSendLsrJob){//�ٲ𶧸� �����ͳ���.
                      if(sMark != m_sLaserWorkLotNo && sMark != ""){ //���� ���ȴ� �����Ͷ� ���� ���� �޶�������.
                          //if(IO_GetX(xLSC_DataReadComp)){
                          //    EM_SetErrMsg(eiLSR_CommErr,"DataReadComp is true");
                          //}
                          Trace("LASER MASKING DATA", sMark.c_str());
                          if(!SaveLaserLotNo(sMark)){//FM_MsgOk("ERR",);
                              EM_SetErrMsg(eiLSR_CommErr , "Laser LotNo File writing failed!");
                              Step.iCycle = 0 ;
                              return true ;
                          }
                          m_sLaserWorkLotNo = sMark ;

                          sFormat = OM.DevOptn.sLaserLotFormat;
                          sLaserData  = GetLaserDataFromLotNot(sFormat , m_sLaserWorkLotNo);

                          //Rs232Laser.SendMsg(eoCHM, sLaserData);
                          Rs232Laser.SendMsg(eoDAT, sLaserData);


                          //�� �ѹ� �ٲ�� ����.
                          //IO_SetY(yLSC_DataRead , true);
                      }
                      else { //��� ���� �����͸�
                          Step.iCycle= 13;
                          return false ;
                      }
                  }
                  else{ //�Ź� ��Ʈ�� ���� ����.
                      //if(IO_GetX(xLSC_DataReadComp)){
                      //    EM_SetErrMsg(eiLSR_CommErr,"DataReadComp is true");
                      //}
                      //Trace("LASER MASKING DATA", sMark.c_str());
                      if(!SaveLaserLotNo(sMark)){//FM_MsgOk("ERR",);
                          EM_SetErrMsg(eiLSR_CommErr , "Laser LotNo File writing failed!");
                          Step.iCycle = 0 ;
                          return true ;
                      }
                      m_sLaserWorkLotNo = sMark ;

                      sFormat = OM.DevOptn.sLaserLotFormat;
                      sLaserData  = GetLaserDataFromLotNot(sFormat , m_sLaserWorkLotNo);

                      Rs232Laser.SendMsg(eoDAT, sLaserData);

                      //IO_SetY(yLSC_DataRead , true);
                  }                                   // ��� ��Ʈ�� ������ ������ ���� ���ִ� ��Ʈ...JH
                  Rs232Laser.ClearReadMsg();
                  Step.iCycle++;
                  return false ;

        case  11: if(!Rs232Laser.ReadMsg().Pos((char)0x03)) return false ;
                  if(Rs232Laser.ReadMsg().Pos("NAK") != 0) {
                      EM_SetErrMsg(eiLSR_CommErr, "Laser Comm Error : Data Change");
                      Trace("Laser", "Rs232Laser.ReadMsg() != NAK");
                      Step.iCycle = 0;
                      return false ;
                  }
                  Step.iCycle++;
                  return false ;

        case  12: //if(IO_GetX(xLSC_DataReadComp)) return false ;

                  Step.iCycle++;
                  return false ;

//=======================������� ������ ��ŷ������ ���
        //��������
        case  13: IO_SetY(yVSC_AlnStart , false);
                  Step.iCycle++;
                  return false;

        case  14: if(OM.CmnOptn.bSkipAlnVisn) {
                      MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                      Step.iCycle = 17 ;
                      return false;
                  }

                  MoveMotr(miLSR_YAlVs , piLSR_YAlVsWork);

                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWork)) return false ;

                  //if(IO_GetX(xVSC_AlnBusy)) {
                  //    EM_SetErrMsg(eiVSN_NotReady , "Align Vision");
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}
                  IO_SetY(yVSC_AlnStart , true);

                  Step.iCycle++;
                  return false;

        case  16: if(!IO_GetX(xVSC_AlnBusy)) return false ;
                  IO_SetY(yVSC_AlnStart , false);
                  Step.iCycle++;
                  return false ;

        case  17: if(IO_GetX(xVSC_AlnBusy)) return false ; //Inspection End
                  MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                  if(MM.GetManNo() == mcNoneCycle && !IO_GetX(xVSC_AlnRslt)){
                      EM_SetErrMsg(eiVSN_InspNG , "Align Vision");
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!OM.CmnOptn.bSkipAlnVisn && !LoadAlignRslt(&m_tAlign)){
                      EM_SetErrMsg(eiVSN_CommErr , "Vision file Error");
                      Step.iCycle = 0;
                      return true;
                  }

                  //�Է� x , �Է� y , ��������. , +�� �ݽð����.
                  //������ ������ x , y
                  //����� x,y

                  //���������� ���.
                  dX = 0 ;
                  dY = PM.GetValue(miLSR_YAlVs, pvLSR_YAlVsWork) - OM.MstOptn.dVsnLsrYCntrPos  ;
                  dT = 0 ;

                  //���������� �����V ���.
                  //RMI���� ���.
                  //dX += OM.DevOptn.dHoleXCntrOfs ; //������ ���Ϳ�
                  //dY += OM.DevOptn.dHoleYCntrOfs ; //�Ϲ������� ������.
                  //dT += OM.DevOptn.dHoleTCntrOfs ; //�Ϲ������� ������.

                  //���� ��Ÿ ���.
                  dT += m_tAlign.dTheta ;

                  GetRotPntFromCntrPnt(0, 0, dT, //������ 0,0 �� ���������� ������ �Է��Ͽ�
                                       dX, dY,     //������� Ȧ��ġ�� �������� ������.
                                       &dLaserX, &dLaserY);

                  //After Compute Theta Apply Vision X,Y
                  //JS.
                  //dLaserX += m_tAlign.dMainX;
                  //dLaserY += m_tAlign.dMainY;
                  //dLaserT =  m_tAlign.dTheta;
                  dLaserX =  m_tAlign.dMainX;
                  dLaserY =  m_tAlign.dMainY;
                  dLaserT =  m_tAlign.dTheta;

                  //��� �� Off set ���ش�.
                  //RMI���� ���.
                  //dLaserX -= OM.DevOptn.dHoleXCntrOfs;
                  //dLaserY -= OM.DevOptn.dHoleYCntrOfs;
                  //dLaserT -= OM.DevOptn.dHoleTCntrOfs;

                  //���⼭ �������� ��ȣ�� �ٲ��ش�.
                  //�������� ������鿡�� ������  ����ĸ� , �������� , ��� ��ο��������� �ð������ +
                  if(dLaserX != 0) dLaserX =  dLaserX ;
                  if(dLaserY != 0) dLaserY =  dLaserY ;
                  if(dLaserT != 0) dLaserT = -dLaserT ;

                  //RMI Test.
                  //���߿� ����..
                  //dLaserX = OM.MstOptn.dLsrXOfs;
                  //dLaserY = OM.MstOptn.dLsrYOfs;
                  //dLaserT = OM.MstOptn.dLsrTOfs;

//================������� ������
                  //RMI ��� ��������.
                  //Rs232Laser.ClearReadMsg();
                  //if(OM.CmnOptn.bSkipAlnVisn) Rs232Laser.SendMsg(      0,       0,       0,       0,       0);
                  //else                        Rs232Laser.SendMsg(dLaserX, dLaserY, dLaserX, dLaserY, dLaserT);

                  //EO��� ��������.

                  sLaserMsg  = 0;
                  sLaserMsg += ","     ;
                  sLaserMsg += 0;
                  sLaserMsg += ","     ;
                  sLaserMsg += dLaserX ;
                  sLaserMsg += ","     ;
                  sLaserMsg += dLaserY ;
                  sLaserMsg += ","     ;
                  sLaserMsg += dLaserT ;
                  //���� �Ǿ� �ϴ°�.
                  //�Ʒ������� �ٲٸ� EO���α׷� �ٲ�� �ϱ� ������ �׳� ���.
                  //sLaserMsg  = dLaserX ;
                  //sLaserMsg += ","     ;
                  //sLaserMsg += dLaserY ;
                  //sLaserMsg += ","     ;
                  //sLaserMsg += dLaserT ;

                  Rs232Laser.ClearReadMsg();
                  Rs232Laser.SendMsg(eoVAD, sLaserMsg);
                  Rs232Laser.ClearReadMsg();
                  Step.iCycle++;
                  return false ;

        case 18:  if(!Rs232Laser.ReadMsg().Pos((char)0x03)) return false;
                  if(Rs232Laser.ReadMsg().Pos("NAK") != 0) {
                      EM_SetErrMsg(eiLSR_CommErr, "Laser Comm Error");
                      Trace("Laser", "Rs232Laser.ReadMsg() != NAK");
                      Step.iCycle=0;
                      return true;
                  }

                  Step.iCycle++;
                  return false ;



        //������ ��ȣ��...
        case  19: MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miLSR_YAlVs , piLSR_YAlVsWait)) return false ;
                  if(OM.CmnOptn.bSkipLaser){// && MM.GetManNo() != mcNoneCycle) {  //������ ��ŵ ����.
                      DM.ARAY[riLSR].SetStat(csWork);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  21: if(m_tmTemp.OnDelay(true, 1000)){
                      EM_SetErrMsg(eiLSR_CommErr, "Laser Not Ready");
                      Step.iCycle = 0;
                      return true;
                  }
                  if(!IO_GetX(xLSC_Ready   )) return false ;

                  //IO_SetY(yLSC_MarkEnable , true);
                  IO_SetY(yLSC_Start , true);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        //Ÿ�Ӿƿ����� ��ȣ�� ����.
        case  22: if(!m_tmTemp.OnDelay(true, 500))return false;
                  if(IO_GetX(xLSC_Progress)) return false ;
                  //if(IO_GetX(xLSC_VisnDataRead)) return false ;
                  //IO_SetY(yLSC_MarkEnable , false);
                  IO_SetY(yLSC_Start , false);
                  if(!DM.ARAY[riLSR].CheckAllStat(csNone)){
                      DM.ARAY[riLSR].SetStat(csWork);
                  }
                  m_iPreSPMCnt++;


                  Step.iCycle=0;
                  return true;
    }
}

void CLaser::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CLaser::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
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

//Device Set Save�ÿ� ������ �� �´��� Ȯ��.
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

    if(iLeftCnt != iRightCnt) return false ; //�ް�ȣ ���� ������ȣ ���� ��������.

    sFormat = _sFormat ;

    while(sFormat.Pos("[") ||sFormat.Pos("]")){
        sTemp = sFormat.SubString(sFormat.Pos("[")+1 , sFormat.Pos("]") - sFormat.Pos("[")-1);
        iTemp = sTemp.ToIntDef(-9999) ;
        if(iTemp < 0) return false ; //���� �ȿ� ������ �������� �ƴϰų� ������ �� �ִ�.
        sFormat.Delete(sFormat.Pos("[") , sFormat.Pos("]") - sFormat.Pos("[") + 1);
    }
    iBlockCnt = 1 ;
    while(sFormat.Pos(";")){
        sFormat.Delete(sFormat.Pos(";") , 1);
        iBlockCnt++;
    }

    if(sFormat != "") return false ; //��ȣ �ۿ� �ִ� ��¥ �߰�.

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

    if(!CheckFormat(sFormat)) { EM_SetErr(eiLSR_DislikeFormat); return ""; }//����


    while(sFormat.Pos("[") ||sFormat.Pos("]")){
        if(sFormat.SubString(1,1) == ";"){
            sFormat.Delete(1 , 1);
            sLaser+=";" ;
            iBlockCnt++;
            continue ;
        }
        sTemp = sFormat.SubString(sFormat.Pos("[")+1 , sFormat.Pos("]") - sFormat.Pos("[")-1);
        iTemp = sTemp.ToIntDef(-1);
        sLaser += sLotNo.SubString(iTemp+1 , 1) ;

        sFormat.Delete(sFormat.Pos("[") , sFormat.Pos("]") - sFormat.Pos("[") + 1);
    }

    //sLaser = AnsiString(iBlockCnt) + "@" + sLaser ;

    return sLaser ;
}

bool CLaser::SaveLaserLotNo(AnsiString _sLotNo)
{

    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath      = OM.MstOptn.sLaserDataFile ;
    AnsiString  sFormat    = "";
    AnsiString  sLaserData = "";
    AnsiString  sCheckData = "";

    sFormat = OM.DevOptn.sLaserLotFormat;
    sLaserData  = GetLaserDataFromLotNot(sFormat , _sLotNo);

    Trace("Laser Format" , sFormat.c_str());
    Trace("Laser LotNo"  , _sLotNo.c_str());
    Trace("Laser Data"   , sLaserData.c_str());



    //ex) LotNo : "20140401_HANRA" �̰� '_'�� �����ϰ� (����,��¥,����Ŀ) 3���� ���� �۾�.�Ҷ�
    //[0][1][2][3]@[4][5][6][7]@[9][10][11][12][13]


    try{
        UserINI.Save(sPath, "MarkingData" , "LotNo" , sLaserData );
    }
    catch(...){
        Trace("Laser Data"   , "Catch Laser LotNo File ���� ����");


    }

    UserINI.Load(sPath, "MarkingData" , "LotNo" , sCheckData );
    if(sLaserData != sCheckData){
        Trace("Laser Data"   , "Laser LotNo File ���� ����");
        return false ;
    }

    return true;

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
        FM_MsgOk("ERR","Laser JobFile File ���� ����");
    }

    return true ;
}


bool CLaser::LoadAlignRslt(TAlignResult *_tAlign)
{
    //Align
    //������ ���� ��� �д´�.
    //"c:\\Control\\VisnLResultAlign.dat�����Ͽ�
    //���ΰ˻� ī�޶��Ϳ����VX,Y ����˻� ī�޶��Ϳ����V x,y �� ���� �����ڴ¡�,�����Ѵ�.
    //Ex) X3.001,Y1.002,T1.221,x1.032,y0.102

    //Local Var.
    int    hFile ;
    String sPath ;
    String sData ;
    String sTemp ;


    //���� �ձ� ����.
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) {
        CreateDir(sPath);
        Trace("Err",(sPath + " no Folder").c_str());
        return false ;
    }

    sPath += String("\\") + "AlignData.dat" ;
    if(!FileExists(sPath)) {
        EM_SetErrMsg(eiVSN_CommErr, "File not exist");
        Trace("Err",(sPath + " no File").c_str());
        return false ;
    }

    int iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    int iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    char * pData = new char[iFileLength+1];
    memset(pData , 0 , sizeof(char)*(iFileLength+1));

    //�����б�.
    FileRead (iFileHandle, pData, iFileLength);
    sData = pData ;
    delete [] pData;
    FileClose(iFileHandle);

    UserFile.FileDelete(sPath.c_str());

    //sData == "X3.001,Y1.002,T0.001,x1.032,y0.102";
    memset(_tAlign , 0 , sizeof(TAlignResult));

    //���� X��.
    if(sData.Pos("X")!=1){
        Trace("Err",(sPath + " no Data X").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(";")-1);
    sData.Delete(1,sData.Pos(";"));
    _tAlign->dMainX = StrToFloatDef(sTemp , 0.0);

    //���� Y��.
    if(!sData.Pos("Y")){
        Trace("Err",(sPath + " no Data Y").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(";")-1);
    sData.Delete(1,sData.Pos(";"));
    _tAlign->dMainY = StrToFloatDef(sTemp , 0.0);

    //���� T��.
    if(!sData.Pos("T")){
        Trace("Err",(sPath + " no Data T").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(";")-1);
    sData.Delete(1,sData.Pos(";"));
    _tAlign->dTheta = StrToFloatDef(sTemp , 0.0);

    //���� X��.
    if(!sData.Pos("x")){
        Trace("Err",(sPath + " no Data x").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(";")-1);
    sData.Delete(1,sData.Pos(";"));
    _tAlign->dSubX = StrToFloatDef(sTemp , 0.0);

    //���� Y��.
    if(!sData.Pos("y")){
        Trace("Err",(sPath + " no Data y").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData;
    sData.Delete(1,sData.Pos(";"));
    _tAlign->dSubY = StrToFloatDef(sTemp , 0.0);

    return true ;
}


//---------------------------------------------------------------------------

void CLaser::GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng , //�Է� x , �Է� y , ��������. , +�� �ݽð����.
                                  double _dCntrX , double _dCntrY ,                //������ ������ x , y
                                  double *_pRetX , double *_pRetY )                //����� x,y
{
    const double MATH_PI = 3.14159265358979 ;
    double x = _dX - _dCntrX;
    double y = _dY - _dCntrY;
    double t = -_dAng ;

    *_pRetX = x*cos(t*MATH_PI/180.0) - y*sin(t*MATH_PI/180.0) ;
    *_pRetY = x*sin(t*MATH_PI/180.0) + y*cos(t*MATH_PI/180.0) ;

    *_pRetX += _dCntrX ;
    *_pRetY += _dCntrY ;



}