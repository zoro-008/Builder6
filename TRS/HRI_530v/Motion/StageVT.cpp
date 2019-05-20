//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "StageVT.h"
#include "SortCmn.h"
#include "Stage.h"
#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CStageVT VTI;



CStageVT::CStageVT(void)
{

}

CStageVT::~CStageVT (void)
{

}

void CStageVT::Init()
{
    m_sPartName = "Stage VT" ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    //IO_SetY(yVTI_VTPower , false ) ;
    IO_SetY(yVTI_VTPower , OM.DevOptn.bVTPwrAllwaysOn ) ;

    m_iVT1Step = 0 ; 
    m_iVT2Step = 0 ; 
    m_iVT3Step = 0 ; 
    m_iVT4Step = 0 ; 

    m_iBLKCntPock1 = 0 ;
    m_iBLKCntPock2 = 0 ;
    m_iBLKCntPock3 = 0 ;
    m_iBLKCntPock4 = 0 ;

    m_iDFTCntPock1 = 0 ;
    m_iDFTCntPock2 = 0 ;
    m_iDFTCntPock3 = 0 ;
    m_iDFTCntPock4 = 0 ;

    m_iDFT2CntPock1 = 0 ;
    m_iDFT2CntPock2 = 0 ;
    m_iDFT2CntPock3 = 0 ;
    m_iDFT2CntPock4 = 0 ;

    m_iVHCntPock1  = 0 ;
    m_iVHCntPock2  = 0 ;
    m_iVHCntPock3  = 0 ;
    m_iVHCntPock4  = 0 ;

}

void CStageVT::Close()
{
    Load(false);
}

void CStageVT::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

typedef void ( *Func )();
void ThreadVTReset()
{
    static bool bAlreadyDoing = false ;
    CDelayTimer Timer ;

    if(bAlreadyDoing) return ;
    bAlreadyDoing = true ;

    bool bNeedResetVT1 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip1stTol ;
    bool bNeedResetVT2 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip2ndTol ;
    bool bNeedResetVT3 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip3thTol ;
    bool bNeedResetVT4 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip4thTol ;

    int iStep    = 10 ;
    int iPreStep = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 3000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiVTI_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("VT Communction",sTemp.c_str());
            IO_SetY(yVTI_Reset1 , false) ;
            IO_SetY(yVTI_Reset2 , false) ;
            IO_SetY(yVTI_Reset3 , false) ;
            IO_SetY(yVTI_Reset4 , false) ;
            iStep = 0 ;
            bAlreadyDoing = false ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("VT Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("VT Communction", sTemp.c_str());
                      IO_SetY(yVTI_Reset1 , false) ;
                      IO_SetY(yVTI_Reset2 , false) ;
                      IO_SetY(yVTI_Reset3 , false) ;
                      IO_SetY(yVTI_Reset4 , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;

            case  10: iStep++;
                      continue ;

            case  11: IO_SetY(yVTI_Reset1 , bNeedResetVT1);
                      IO_SetY(yVTI_Reset2 , bNeedResetVT2);
                      IO_SetY(yVTI_Reset3 , bNeedResetVT3);
                      IO_SetY(yVTI_Reset4 , bNeedResetVT4);

                      iStep++;
                      continue ;


            case  13: if( IO_GetX(xVTI_Busy1)) continue ;
                      if( IO_GetX(xVTI_Busy2)) continue ;
                      if( IO_GetX(xVTI_Busy3)) continue ;
                      if( IO_GetX(xVTI_Busy4)) continue ;

                      IO_SetY(yVTI_Reset1 , false );
                      IO_SetY(yVTI_Reset2 , false );
                      IO_SetY(yVTI_Reset3 , false );
                      IO_SetY(yVTI_Reset4 , false );

                      iStep = 0 ;
                      bAlreadyDoing = false ;
                      return ;
        }
    }

}

void ThreadVTJobChange()
{

    CDelayTimer Timer ;


    int iStep    = 10 ;
    int iPreStep = 0  ;

    bool bNeedChangeVT1 = !OM.CmnOptn.bSkip1stTol ;
    bool bNeedChangeVT2 = !OM.CmnOptn.bSkip2ndTol ;
    bool bNeedChangeVT3 = !OM.CmnOptn.bSkip3thTol ;
    bool bNeedChangeVT4 = !OM.CmnOptn.bSkip4thTol ;


    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 3000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiVTI_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("VT Communction",sTemp.c_str());
            IO_SetY(yVTI_Change1 , false) ;
            IO_SetY(yVTI_Change2 , false) ;
            IO_SetY(yVTI_Change3 , false) ;
            IO_SetY(yVTI_Change4 , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("VT Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("VT Communction", sTemp.c_str());
                      IO_SetY(yVTI_Change1 , false) ;
                      IO_SetY(yVTI_Change2 , false) ;
                      IO_SetY(yVTI_Change3 , false) ;
                      IO_SetY(yVTI_Change4 , false) ;
                      iStep = 0 ;
                      return ;

            case  10: if((IO_GetX(xVTI_Busy1)&&bNeedChangeVT1)||
                         (IO_GetX(xVTI_Busy2)&&bNeedChangeVT2)||
                         (IO_GetX(xVTI_Busy3)&&bNeedChangeVT3)||
                         (IO_GetX(xVTI_Busy4)&&bNeedChangeVT4)){

                          if(IO_GetX(xVTI_Busy1))EM_SetErrMsg(eiVTI_Busy,"VT1");
                          if(IO_GetX(xVTI_Busy2))EM_SetErrMsg(eiVTI_Busy,"VT2");
                          if(IO_GetX(xVTI_Busy3))EM_SetErrMsg(eiVTI_Busy,"VT3");
                          if(IO_GetX(xVTI_Busy4))EM_SetErrMsg(eiVTI_Busy,"VT4");

                          return;
                      }
                      IO_SetY(yVTI_Change1 , true) ;
                      IO_SetY(yVTI_Change2 , true) ;
                      IO_SetY(yVTI_Change3 , true) ;
                      IO_SetY(yVTI_Change4 , true) ;

                      iStep++;
                      continue ;

            case  11: if(!IO_GetX(xVTI_Busy1)) continue ;
                      if(!IO_GetX(xVTI_Busy2)) continue ;
                      if(!IO_GetX(xVTI_Busy3)) continue ;
                      if(!IO_GetX(xVTI_Busy4)) continue ;

                      IO_SetY(yVTI_Change1 , false) ;
                      IO_SetY(yVTI_Change2 , false) ;
                      IO_SetY(yVTI_Change3 , false) ;
                      IO_SetY(yVTI_Change4 , false) ;

                      iStep++;
                      continue ;

            case  12: if(IO_GetX(xVTI_Busy1)) continue ;
                      if(IO_GetX(xVTI_Busy2)) continue ;
                      if(IO_GetX(xVTI_Busy3)) continue ;
                      if(IO_GetX(xVTI_Busy4)) continue ;

                      iStep = 0 ;
                      return ;
        }
    }
}
bool CStageVT::VTJobChange()
{

    if(!SaveVTDevice(OM.GetCrntDev())) {
        FM_MsgOk("ERR","VT Device 쓰기 실패");
        return false ;
    }

    CRunThread<Func> * func = new CRunThread<Func>(&ThreadVTJobChange) ;

}

void CStageVT::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    IO_SetY(yVTI_Start1 , false);
    IO_SetY(yVTI_Start2 , false);
    IO_SetY(yVTI_Start3 , false);
    IO_SetY(yVTI_Start4 , false);

    //다른 애러에도 클리어됌.
    //m_iBLKCntPock1 = 0 ;
    //m_iBLKCntPock2 = 0 ;
    //m_iBLKCntPock3 = 0 ;
    //m_iBLKCntPock4 = 0 ;










    bool bNeedResetVT1 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip1stTol ;
    bool bNeedResetVT2 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip2ndTol ;
    bool bNeedResetVT3 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip3thTol ;
    bool bNeedResetVT4 = IO_GetX(xVTI_Busy1) && !OM.CmnOptn.bSkip4thTol ;


    if(!OM.CmnOptn.bSkipVT) {
        if(bNeedResetVT1 ||
           bNeedResetVT2 ||
           bNeedResetVT3 ||
           bNeedResetVT4 ){ //LASER Not Ready Condition
            CRunThread<Func> * func = new CRunThread<Func>(&ThreadVTReset) ;
        }
    }
}


bool CStageVT::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    if(DM.ARAY[riSTB].FindFrstColRow(csGood , r,c)){
        return true ;
    }
    else {
        r=0 ;
        c=0 ;
        return false ;
    }

}

double CStageVT::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    //int    iR,iC ;
    //FindChip(iR ,iC);

    //double dWorkPos ;
    //dWorkPos = PM.GetValue(miLDR_YIndx , pvLDR_YIndxWorkStt) ;
    //dWorkPos = dWorkPos - (OM.DevInfo.dRowPitch * iR) ;
    //
    //if(_iMotr == miSRT_ZElev){
    //    switch(_iPstnId) {
    //        default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
    //        case piLDR_YIndxWait     : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWait      ); break ;
    //        case piLDR_YIndxGet      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxGet       ); break ;
    //        case piLDR_YIndxWorkStt  : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWorkStt   ); break ;
    //        case piLDR_YIndxOut      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxOut       ); break ;
    //        case piLDR_YIndxWork     : dPos = dWorkPos                                    ; break ;
    //
    //    }
    //}
    //
    //else {
        dPos = MT_GetCmdPos(_iMotr);
    //}

    return dPos ;
}


bool CStageVT::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStageVT::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CStageVT::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: for(int i = 0 ; i < (int)xVTI_Spare4_2 - (int)xVTI_Ready1 ; i++){
                     IO_SetY((int)xVTI_Ready1+i , false);
                 }          //?????

                 if(OM.DevOptn.bVTPwrAllwaysOn){IO_SetY(yVTI_VTPower , true  ) ;}
                 else                          {IO_SetY(yVTI_VTPower , false ) ;}

                 Step.iToStart++ ;
                 return true ;

        case 11: if(!OM.CmnOptn.bSkipVT &&
                    (!IO_GetX(xVTI_Ready1) && !OM.CmnOptn.bSkip1stTol )&&
                    (!IO_GetX(xVTI_Ready2) && !OM.CmnOptn.bSkip1stTol )&&
                    (!IO_GetX(xVTI_Ready3) && !OM.CmnOptn.bSkip1stTol )&&
                    (!IO_GetX(xVTI_Ready4) && !OM.CmnOptn.bSkip1stTol )){
                     EM_SetErr(eiVTI_NotReady);
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStageVT::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10: MoveActr(aiVTI_PTolUpDn , ccBwd);
                 IO_SetY(yVTI_VTPower , false ) ;
                 Step.iToStop++;
                 return false;

        case 11: if(!MoveActr(aiVTI_PTolUpDn , ccBwd))return false;
                 MoveActr(aiVTI_PshrDnUp , ccBwd);

                 Step.iToStop++;
                 return false;

        case 12: if(!MoveActr(aiVTI_PshrDnUp , ccBwd))return false;
                 IO_SetY(yVTI_Start1 , false);
                 IO_SetY(yVTI_Start2 , false);
                 IO_SetY(yVTI_Start3 , false);
                 IO_SetY(yVTI_Start4 , false);





                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CStageVT::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iSeq=%02d" , __FUNC__ , Step.iSeq );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //bool bVTAllReady = IO_GetX(xVTI_Ready1) && IO_GetX(xVTI_Ready1) && IO_GetX(xVTI_Ready1) && IO_GetX(xVTI_Ready1) ;

        bool isCycleWork = ScmGetCntStat(riSTB,csUnkwn) && MT_GetStopInpos(miSTG_TTble) && STG.GetSeqStep() == CStage::scIdle;
                           //!DM.ARAY[riSTB].CheckAllStat(csNone) && MT_GetStopInpos(miSTG_TTble) &&
                           //(DM.ARAY[riSTB].GetCntStat(csRslt0)  || DM.ARAY[riSG3].GetCntStat(csFail)) ;
        bool isCycleEnd  =  DM.ARAY[riSTB].CheckAllStat(csEmpty) ;

         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default    End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                            return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork  End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CStageVT::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiVTI_PTolUpDn, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiVTI_PTolUpDn, ccBwd)) return false ;
                 AT_MoveCyl(aiVTI_PshrDnUp, ccBwd);

                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiVTI_PshrDnUp, ccBwd)) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}


bool CStageVT::CycleVT1()
{
    static int iPreVT1Step = 0 ;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s m_iVT1Step=%02d" , __FUNC__ , m_iVT1Step );
    if(m_iVT1Step != iPreVT1Step) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    iPreVT1Step = m_iVT1Step ;


    switch (m_iVT1Step) {

        default: //sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iVT1Step = 0 ;
                 return true ;

        case 10: IO_SetY(yVTI_Start1 , true);
                 m_iVT1Step++;
                 return false;

        case 11: if(!IO_GetX(xVTI_Busy1)) return false;
                 IO_SetY(yVTI_Start1 , false);
                 m_iVT1Step++;
                 return false;

        case 12: if(IO_GetX(xVTI_Busy1)) return false;
                 m_iVT1Step = 0;
                 return true ;

    }
}
bool CStageVT::CycleVT2()
{
    static int iPreVT2Step = 0 ;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s m_iVT2Step=%02d" , __FUNC__ , m_iVT2Step );
    if(m_iVT2Step != iPreVT2Step) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    iPreVT2Step = m_iVT2Step ;


    switch (m_iVT2Step) {

        default: //sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iVT2Step = 0 ;
                 return true ;

        case 10: IO_SetY(yVTI_Start2 , true);
                 m_iVT2Step++;
                 return false;

        case 11: if(!IO_GetX(xVTI_Busy2)) return false;
                 IO_SetY(yVTI_Start2 , false);
                 m_iVT2Step++;
                 return false;

        case 12: if(IO_GetX(xVTI_Busy2)) return false;
                 m_iVT2Step = 0;
                 return true ;

    }
}

bool CStageVT::CycleVT3()
{
    static int iPreVT3Step = 0 ;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s m_iVT3Step=%02d" , __FUNC__ , m_iVT3Step );
    if(m_iVT3Step != iPreVT3Step) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    iPreVT3Step = m_iVT3Step ;


    switch (m_iVT3Step) {

        default: //sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iVT3Step = 0 ;
                 return true ;

        case 10: IO_SetY(yVTI_Start3 , true);
                 m_iVT3Step++;
                 return false;

        case 11: if(!IO_GetX(xVTI_Busy3)) return false;
                 IO_SetY(yVTI_Start3 , false);
                 m_iVT3Step++;
                 return false;

        case 12: if(IO_GetX(xVTI_Busy3)) return false;
                 m_iVT3Step = 0;
                 return true ;

    }
}

bool CStageVT::CycleVT4()
{
    static int iPreVT4Step = 0 ;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s m_iVT4Step=%02d" , __FUNC__ , m_iVT4Step );
    if(m_iVT4Step != iPreVT4Step) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    iPreVT4Step = m_iVT4Step ;



    switch (m_iVT4Step) {

        default: //sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 m_iVT4Step = 0 ;
                 return true ;

        case 10: IO_SetY(yVTI_Start4 , true);
                 m_iVT4Step++;
                 return false;

        case 11: if(!IO_GetX(xVTI_Busy4)) return false;
                 IO_SetY(yVTI_Start4 , false);
                 m_iVT4Step++;
                 return false;

        case 12: if(IO_GetX(xVTI_Busy4)) return false;
                 m_iVT4Step = 0;
                 return true ;

    }
}






//One Cycle.
bool CStageVT::CycleWork(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    String sTemp2 = "";
    static bool r1,r2,r3,r4 ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        if(Step.iCycle == 16) {//VT 검사.
            if(!r1) {
                if(sTemp2 == "") sTemp2 = "1" ;
                else             sTemp2+= ",1";
            }
            if(!r2) {
                if(sTemp2 == "") sTemp2 = "2" ;
                else             sTemp2+= ",2";
            }
            if(!r3) {
                if(sTemp2 == "") sTemp2 = "3" ;
                else             sTemp2+= ",3";
            }
            if(!r4) {
                if(sTemp2 == "") sTemp2 = "4" ;
                else             sTemp2+= ",4";
            }
            sTemp = sTemp2 + " VT TimeOut" ;
        }
        else {
            sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        }
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        if(!OM.DevOptn.bVTPwrAllwaysOn){IO_SetY(yVTI_VTPower , false) ;}



        IO_SetY(yVTI_Start1 , false);
        IO_SetY(yVTI_Start2 , false);
        IO_SetY(yVTI_Start3 , false);
        IO_SetY(yVTI_Start4 , false);

        MoveActr(aiVTI_PTolUpDn, ccBwd);
        MoveActr(aiVTI_PshrDnUp, ccBwd);


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




    //SubStat Index
    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;
    int iTemp ;

    bool bSkipInsp = OM.CmnOptn.bSkipVT;
    const bool bMotionSkipOnInspSkip = true ; //VT스킵시에 모션도 스킵 하는것 .. 셑업할때 드라이런 써야 되서 한다.

    //매뉴얼 동작 일때는 엠티 안따짐.
    bool bIdle = GetSeqStep()==scIdle ;

    bool b11 = DM.ARAY[riSTB].GetStat(0,3)!=csEmpty || bIdle ;
    bool b12 = DM.ARAY[riSTB].GetStat(0,2)!=csEmpty || bIdle ;
    bool b13 = DM.ARAY[riSTB].GetStat(0,1)!=csEmpty || bIdle ;
    bool b14 = DM.ARAY[riSTB].GetStat(0,0)!=csEmpty || bIdle ;

    bool b21 = !OM.CmnOptn.bSkip1stTol ;
    bool b22 = !OM.CmnOptn.bSkip2ndTol ;
    bool b23 = !OM.CmnOptn.bSkip3thTol ;
    bool b24 = !OM.CmnOptn.bSkip4thTol ;

    bool b31 = (DM.ARAY[riSTB].GetSubStat(0,3,iOSTRslt)==csGood||bIdle) ;
    bool b32 = (DM.ARAY[riSTB].GetSubStat(0,2,iOSTRslt)==csGood||bIdle) ;
    bool b33 = (DM.ARAY[riSTB].GetSubStat(0,1,iOSTRslt)==csGood||bIdle) ;
    bool b34 = (DM.ARAY[riSTB].GetSubStat(0,0,iOSTRslt)==csGood||bIdle) ;
                                              //OST 검사 불량은 검사 하면 VT프로그램 멈추는 현상이 발생하여 뺀다.
    bool bNeedInsp1 = b11 && b21 && b31 ;
    bool bNeedInsp2 = b12 && b22 && b32 ;
    bool bNeedInsp3 = b13 && b23 && b33 ;
    bool bNeedInsp4 = b14 && b24 && b34 ;

    EN_CHIP_STAT csStat ;
    EN_ARAY_ID   iAray ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10:  DM.ARAY[riMVT].SetStat(csEmpty);
                  if(OM.DevOptn.bVTPwrAllwaysOn){IO_SetY(yVTI_VTPower , true ) ;}//1030자제 경우 이상하게 온오프를 안해야 BLK가 안뜬다.

                  if(bMotionSkipOnInspSkip && bSkipInsp) {
                     for(int c = 0 ; c < MAX_TOOL_SOCK_COL ; c++){
                          iTemp = rand()%100;
                          if(iTemp < OM.MstOptn.iInspRandMask) {
                              iTemp = rand()%4;
                              if(iTemp == 0) csStat = OM.CmnOptn.iVTBLKBin+1 ; //콤보박스는 0부터시작 Rslt는 1부터 시작.
                              if(iTemp == 1) csStat = OM.CmnOptn.iVTDFTBin+1 ;
                              if(iTemp == 2) csStat = OM.CmnOptn.iVTDIMBin+1 ;
                              if(iTemp == 3) csStat = OM.CmnOptn.iVTVHBin +1 ;
                          }
                          else {
                              csStat = csGood;
                          }
                          DM.ARAY[riMVT].SetStat(0,MAX_TOOL_SOCK_COL - c - 1 ,csStat);

                          
                     }

                     iAray = riSTB ;
                     for(int c = 0 ; c < MAX_TOOL_SOCK_COL ; c++){
                         csStat = DM.ARAY[riMVT].GetStat(0,c);
                         if(GetSeqStep() != scIdle) {
                             if(DM.ARAY[riSTB].GetStat(0,c) != csEmpty)DM.ARAY[riSTB].SetStat(0,c,(EN_CHIP_STAT)csStat);
                         }
                     }

                     Step.iCycle=0;
                     return true ;
                 }

                 MoveActr(aiVTI_PshrDnUp, ccFwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiVTI_PshrDnUp, ccFwd)) return false ;
                 MoveActr(aiVTI_PTolUpDn, ccFwd);


                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveActr(aiVTI_PTolUpDn, ccFwd)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iBfrVTPowrDelay)) return false ;

                 IO_SetY(yVTI_VTPower , true) ;

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 14: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iBfrVTInspDelay)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 15: if(!bSkipInsp){
                     if(m_tmDelay.OnDelay(true , 1000)){

                         if(IO_GetX(xVTI_Busy1))EM_SetErrMsg(eiVTI_Busy,"VT1");
                         if(IO_GetX(xVTI_Busy2))EM_SetErrMsg(eiVTI_Busy,"VT2");
                         if(IO_GetX(xVTI_Busy3))EM_SetErrMsg(eiVTI_Busy,"VT3");
                         if(IO_GetX(xVTI_Busy4))EM_SetErrMsg(eiVTI_Busy,"VT4");


                         if(!OM.DevOptn.bVTPwrAllwaysOn){IO_SetY(yVTI_VTPower , false ) ;}


                         MoveActr(aiVTI_PTolUpDn, ccBwd);
                         MoveActr(aiVTI_PshrDnUp, ccBwd);
                         Step.iCycle = 0 ;
                         return true ;
                     }
                     if(IO_GetX(xVTI_Busy1) && bNeedInsp1) return false;
                     if(IO_GetX(xVTI_Busy2) && bNeedInsp2) return false;
                     if(IO_GetX(xVTI_Busy3) && bNeedInsp3) return false;
                     if(IO_GetX(xVTI_Busy4) && bNeedInsp4) return false;
                 }

                 m_iVT1Step = 10 ; r1=true ; //r1=true ; 이거 안해주면 스킵옵션 켰을때 타임아웃 에러 날릴때 서브 메세지 전에값때문에 막씀.20141212 선
                 m_iVT2Step = 10 ; r2=true ;
                 m_iVT3Step = 10 ; r3=true ;
                 m_iVT4Step = 10 ; r4=true ;

                 Step.iCycle++;
                 return false;

        //타임아웃에서 씀 번호바꿀때 확인해야함.
        case 16: if(!bSkipInsp){
                     if(bNeedInsp1) r1=CycleVT1();
                     if(bNeedInsp2) r2=CycleVT2();
                     if(bNeedInsp3) r3=CycleVT3();
                     if(bNeedInsp4) r4=CycleVT4();

                     if(bNeedInsp1 && !r1)return false ;
                     if(bNeedInsp2 && !r2)return false ;
                     if(bNeedInsp3 && !r3)return false ;
                     if(bNeedInsp4 && !r4)return false ;
                 }

                 MoveActr(aiVTI_PTolUpDn,ccBwd);
                 if(!OM.DevOptn.bVTPwrAllwaysOn){IO_SetY(yVTI_VTPower , false ) ;}



                 Step.iCycle++;
                 return false;

        case 17: if(!MoveActr(aiVTI_PTolUpDn,ccBwd))return false;
                 MoveActr(aiVTI_PshrDnUp,ccBwd);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveActr(aiVTI_PshrDnUp,ccBwd))return false;
                 iAray = riMVT ; //ScmGetRearSTGAray() ;
                 if(!bSkipInsp){
                     if(bNeedInsp1){
                         if(GetVTRslt(0 , csStat)){
                             DM.ARAY[riMVT].SetStat(0,3,csStat);
                         }
                         else                                {
                             EM_SetErrMsg(eiVTI_WrongData , "Data Read Fail VT1");
                             Step.iCycle=0;
                             return true ;
                         }
                     }
                     else {
                         if(DM.ARAY[riSTB].GetSubStat(0,3,iOSTRslt)==OM.CmnOptn.iOSTOSTBin +1){
                             DM.ARAY[riMVT].SetStat(0,3,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                         }
                         else if(DM.ARAY[riSTB].GetSubStat(0,3,iOSTRslt)==OM.CmnOptn.iOSTCrnBin +1){
                             DM.ARAY[riMVT].SetStat(0,3,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                         }
                     }
                     if(bNeedInsp2){
                         if(GetVTRslt(1 , csStat)){
                             DM.ARAY[riMVT].SetStat(0,2,csStat);
                         }
                         else                                {
                             EM_SetErrMsg(eiVTI_WrongData , "Data Read Fail VT2");
                             Step.iCycle=0;
                             return true ;
                         }
                     }
                     else {
                         if(DM.ARAY[riSTB].GetSubStat(0,2,iOSTRslt)==OM.CmnOptn.iOSTOSTBin +1){
                             DM.ARAY[riMVT].SetStat(0,2,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                         }
                         else if(DM.ARAY[riSTB].GetSubStat(0,2,iOSTRslt)==OM.CmnOptn.iOSTCrnBin +1){
                             DM.ARAY[riMVT].SetStat(0,2,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                         }
                     }
                     if(bNeedInsp3){
                         if(GetVTRslt(2 , csStat)){
                             DM.ARAY[riMVT].SetStat(0,1,csStat);
                         }
                         else                                {
                             EM_SetErrMsg(eiVTI_WrongData , "Data Read Fail VT3");
                             Step.iCycle=0;
                             return true ;
                         }
                     }
                     else {
                         if(DM.ARAY[riSTB].GetSubStat(0,1,iOSTRslt)==OM.CmnOptn.iOSTOSTBin +1){
                             DM.ARAY[riMVT].SetStat(0,1,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                         }
                         else if(DM.ARAY[riSTB].GetSubStat(0,1,iOSTRslt)==OM.CmnOptn.iOSTCrnBin +1){
                             DM.ARAY[riMVT].SetStat(0,1,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                         }
                     }
                     if(bNeedInsp4){
                         if(GetVTRslt(3 , csStat)){
                             DM.ARAY[riMVT].SetStat(0,0,csStat);
                         }
                         else                                {
                             EM_SetErrMsg(eiVTI_WrongData , "Data Read Fail VT4");
                             Step.iCycle=0;
                             return true ;
                         }
                     }
                     else {
                         if(DM.ARAY[riSTB].GetSubStat(0,0,iOSTRslt)==OM.CmnOptn.iOSTOSTBin +1){
                             DM.ARAY[riMVT].SetStat(0,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                         }
                         else if(DM.ARAY[riSTB].GetSubStat(0,0,iOSTRslt)==OM.CmnOptn.iOSTCrnBin +1){
                             DM.ARAY[riMVT].SetStat(0,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                         }
                     }
                 }//!bSkipInsp끝

                 else{//bSkipInsp시작
                      for(int c = 0 ; c < MAX_TOOL_SOCK_COL ; c++){
                          iTemp = rand()%100;
                          if(iTemp < OM.MstOptn.iInspRandMask) {
                              iTemp = rand()%4;
                              if(iTemp == 0) csStat = OM.CmnOptn.iVTBLKBin+1 ; //콤보박스는 0부터시작 Rslt는 1부터 시작.
                              if(iTemp == 1) csStat = OM.CmnOptn.iVTDFTBin+1 ;
                              if(iTemp == 2) csStat = OM.CmnOptn.iVTDIMBin+1 ;
                              if(iTemp == 3) csStat = OM.CmnOptn.iVTVHBin +1 ;
                          }
                          else {
                              csStat = csGood;
                          }
                          DM.ARAY[riMVT].SetStat(0,MAX_TOOL_SOCK_COL - c - 1 ,csStat);

                      }
                 }//bSkipInsp끝.

                 iAray = riSTB;
                 for(int c = 0 ; c < MAX_TOOL_SOCK_COL ; c++){
                     csStat = DM.ARAY[riMVT].GetStat(0,c);
                     if(GetSeqStep() != scIdle) {
                         if(DM.ARAY[riSTB].GetStat(0,c)!=csEmpty) DM.ARAY[riSTB].SetStat(0,c,(EN_CHIP_STAT)csStat);
                     }
                 }

                 if(GetSeqStep() != scIdle) {  //메뉴얼로 세팅할때 자꾸 스테이지 돌려 놓고 검사 하면 삑사리 나서...
                     if(DM.ARAY[riSTB].GetStat(0,3) != csEmpty && //자제가 있고.
                        DM.ARAY[riSTB].GetSubStat(0,3,iOSTRslt) == csGood) { //OST굿인 놈만.
                         if(DM.ARAY[riSTB].GetStat(0,3) == (EN_CHIP_STAT)OM.CmnOptn.iVTBLKBin+1 ) m_iBLKCntPock1 ++ ;
                         else                                                                     m_iBLKCntPock1 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,3) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFTBin+1 ) m_iDFTCntPock1 ++ ;
                         else                                                                     m_iDFTCntPock1 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,3) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFT2Bin+1) m_iDFT2CntPock1++ ;
                         else                                                                     m_iDFT2CntPock1=0 ;
                         if(DM.ARAY[riSTB].GetStat(0,3) == (EN_CHIP_STAT)OM.CmnOptn.iVTVHBin+1  ) m_iVHCntPock1  ++ ;
                         else                                                                     m_iVHCntPock1  =0 ;
                     }
                     if(DM.ARAY[riSTB].GetStat(0,2) != csEmpty&&
                        DM.ARAY[riSTB].GetSubStat(0,2,iOSTRslt) == csGood) {
                         if(DM.ARAY[riSTB].GetStat(0,2) == (EN_CHIP_STAT)OM.CmnOptn.iVTBLKBin+1 ) m_iBLKCntPock2 ++ ;
                         else                                                                     m_iBLKCntPock2 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,2) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFTBin+1 ) m_iDFTCntPock2 ++ ;
                         else                                                                     m_iDFTCntPock2 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,2) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFT2Bin+1) m_iDFT2CntPock2++ ;
                         else                                                                     m_iDFT2CntPock2=0 ;
                         if(DM.ARAY[riSTB].GetStat(0,2) == (EN_CHIP_STAT)OM.CmnOptn.iVTVHBin+1  ) m_iVHCntPock2  ++ ;
                         else                                                                     m_iVHCntPock2  =0 ;

                     }
                     if(DM.ARAY[riSTB].GetStat(0,1) != csEmpty&&
                        DM.ARAY[riSTB].GetSubStat(0,1,iOSTRslt) == csGood) {
                         if(DM.ARAY[riSTB].GetStat(0,1) == (EN_CHIP_STAT)OM.CmnOptn.iVTBLKBin+1 ) m_iBLKCntPock3 ++ ;
                         else                                                                     m_iBLKCntPock3 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,1) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFTBin+1 ) m_iDFTCntPock3 ++ ;
                         else                                                                     m_iDFTCntPock3 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,1) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFT2Bin+1) m_iDFT2CntPock3++ ;
                         else                                                                     m_iDFT2CntPock3=0 ;
                         if(DM.ARAY[riSTB].GetStat(0,1) == (EN_CHIP_STAT)OM.CmnOptn.iVTVHBin+1  ) m_iVHCntPock3  ++ ;
                         else                                                                     m_iVHCntPock3  =0 ;
                     }
                     if(DM.ARAY[riSTB].GetStat(0,0) != csEmpty&&
                        DM.ARAY[riSTB].GetSubStat(0,0,iOSTRslt) == csGood) {
                         if(DM.ARAY[riSTB].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iVTBLKBin+1 ) m_iBLKCntPock4 ++ ;
                         else                                                                     m_iBLKCntPock4 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFTBin+1 ) m_iDFTCntPock4 ++ ;
                         else                                                                     m_iDFTCntPock4 =0 ;
                         if(DM.ARAY[riSTB].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iVTDFT2Bin+1) m_iDFT2CntPock4++ ;
                         else                                                                     m_iDFT2CntPock4=0 ;
                         if(DM.ARAY[riSTB].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iVTVHBin+1  ) m_iVHCntPock4  ++ ;
                         else                                                                     m_iVHCntPock4  =0 ;
                     }

                     if(OM.CmnOptn.iVTContBlkCnt != 0){ //0으로 설정 되어 있으면 사용 안함.
                          sTemp = "";  //BLK
                          if(OM.CmnOptn.iVTContBlkCnt<=m_iBLKCntPock1){if(sTemp=="") sTemp =  "1"; else sTemp+= ",1";}
                          if(OM.CmnOptn.iVTContBlkCnt<=m_iBLKCntPock2){if(sTemp=="") sTemp =  "2"; else sTemp+= ",2";}
                          if(OM.CmnOptn.iVTContBlkCnt<=m_iBLKCntPock3){if(sTemp=="") sTemp =  "3"; else sTemp+= ",3";}
                          if(OM.CmnOptn.iVTContBlkCnt<=m_iBLKCntPock4){if(sTemp=="") sTemp =  "4"; else sTemp+= ",4";}
                          if(sTemp != "") { //만약 한포켓이라도 메세지가 세팅이 되었으면.
                              Trace("m_iBLKCntPock",(String(m_iBLKCntPock1) + "_" + String(m_iBLKCntPock2) + "_" + String(m_iBLKCntPock3) + "_" + String(m_iBLKCntPock4)).c_str() );
                              m_iBLKCntPock1 = m_iBLKCntPock2 = m_iBLKCntPock3 = m_iBLKCntPock4 = 0 ;
                              EM_SetErrMsg(eiVTI_SerialNG , (sTemp + "번포켓 BLK 불량 연속").c_str());
                          }
                     }

                     if(OM.CmnOptn.iVTContDftCnt != 0){ //0으로 설정 되어 있으면 사용 안함.
                          sTemp = "";  //Defect
                          if(OM.CmnOptn.iVTContDftCnt<=m_iDFTCntPock1){if(sTemp=="") sTemp =  "1"; else sTemp+= ",1";}
                          if(OM.CmnOptn.iVTContDftCnt<=m_iDFTCntPock2){if(sTemp=="") sTemp =  "2"; else sTemp+= ",2";}
                          if(OM.CmnOptn.iVTContDftCnt<=m_iDFTCntPock3){if(sTemp=="") sTemp =  "3"; else sTemp+= ",3";}
                          if(OM.CmnOptn.iVTContDftCnt<=m_iDFTCntPock4){if(sTemp=="") sTemp =  "4"; else sTemp+= ",4";}
                          if(sTemp != "") { //만약 한포켓이라도 메세지가 세팅이 되었으면.
                              Trace("m_iDFTCntPock1",(String(m_iDFTCntPock1) + "_" + String(m_iDFTCntPock2) + "_" + String(m_iDFTCntPock3) + "_" + String(m_iDFTCntPock4)).c_str() );
                              m_iDFTCntPock1 = m_iDFTCntPock2 = m_iDFTCntPock3 = m_iDFTCntPock4 = 0 ;
                              EM_SetErrMsg(eiVTI_SerialNG , (sTemp + "번포켓 Defect 불량 연속").c_str());
                          }
                     }

                     if(OM.CmnOptn.iVTContDft2Cnt != 0){ //0으로 설정 되어 있으면 사용 안함.
                          sTemp = "";  //Defect2
                          if(OM.CmnOptn.iVTContDft2Cnt<=m_iDFT2CntPock1){if(sTemp=="") sTemp =  "1"; else sTemp+= ",1";}
                          if(OM.CmnOptn.iVTContDft2Cnt<=m_iDFT2CntPock2){if(sTemp=="") sTemp =  "2"; else sTemp+= ",2";}
                          if(OM.CmnOptn.iVTContDft2Cnt<=m_iDFT2CntPock3){if(sTemp=="") sTemp =  "3"; else sTemp+= ",3";}
                          if(OM.CmnOptn.iVTContDft2Cnt<=m_iDFT2CntPock4){if(sTemp=="") sTemp =  "4"; else sTemp+= ",4";}
                          if(sTemp != "") { //만약 한포켓이라도 메세지가 세팅이 되었으면.
                              Trace("m_iDFT2CntPock1",(String(m_iDFT2CntPock1) + "_" + String(m_iDFT2CntPock2) + "_" + String(m_iDFT2CntPock3) + "_" + String(m_iDFT2CntPock4)).c_str() );
                              m_iDFT2CntPock1 = m_iDFT2CntPock2 = m_iDFT2CntPock3 = m_iDFT2CntPock4 = 0 ;
                              EM_SetErrMsg(eiVTI_SerialNG , (sTemp + "번포켓 Defect2 불량 연속").c_str());
                          }
                     }

                     if(OM.CmnOptn.iVTContVHCnt != 0){ //0으로 설정 되어 있으면 사용 안함.
                          sTemp = "";  //VH
                          if(OM.CmnOptn.iVTContVHCnt<=m_iVHCntPock1){if(sTemp=="") sTemp =  "1"; else sTemp+= ",1";}
                          if(OM.CmnOptn.iVTContVHCnt<=m_iVHCntPock2){if(sTemp=="") sTemp =  "2"; else sTemp+= ",2";}
                          if(OM.CmnOptn.iVTContVHCnt<=m_iVHCntPock3){if(sTemp=="") sTemp =  "3"; else sTemp+= ",3";}
                          if(OM.CmnOptn.iVTContVHCnt<=m_iVHCntPock4){if(sTemp=="") sTemp =  "4"; else sTemp+= ",4";}
                          if(sTemp != "") { //만약 한포켓이라도 메세지가 세팅이 되었으면.
                              Trace("m_iVHCntPock1",(String(m_iVHCntPock1) + "_" + String(m_iVHCntPock2) + "_" + String(m_iVHCntPock3) + "_" + String(m_iVHCntPock4)).c_str() );
                              m_iVHCntPock1 = m_iVHCntPock2 = m_iVHCntPock3 = m_iVHCntPock4 = 0 ;
                              EM_SetErrMsg(eiVTI_SerialNG , (sTemp + "번포켓 VH 불량 연속").c_str());
                          }
                     }
                 }

                 Step.iCycle=0;
                 return true;
    }
}

bool CStageVT::CycleBind()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: MoveActr(aiVTI_PshrDnUp, ccFwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiVTI_PshrDnUp, ccFwd)) return false ;
                 MoveActr(aiVTI_PTolUpDn, ccFwd);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveActr(aiVTI_PTolUpDn, ccFwd)) return false ;
                 Step.iCycle=0;
                 return true;
    }

}

bool CStageVT::CycleUnBind()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: MoveActr(aiVTI_PTolUpDn, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiVTI_PTolUpDn, ccBwd)) return false ;
                 MoveActr(aiVTI_PshrDnUp, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveActr(aiVTI_PshrDnUp, ccBwd)) return false ;
                 Step.iCycle=0;
                 return true;
    }

}


//---------------------------------------------------------------------------
bool CStageVT::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //bool bExistPRL   = DM.ARAY[riPRL].GetCntExist() ;
    //bool bExistPSL   = DM.ARAY[riPSL].GetCntExist() ;
    //bool bExistLDR   = DM.ARAY[riLDR].GetCntExist() ;
    //bool bIndxFwd    = !AT_Complete(aiLDR_IndxUpUp , ccBwd);


    //bool bUnderWorkPos = PM.GetValue(miLDR_YIndx , pvLDR_YIndxWorkStt)+10.0 >= MT_GetCmdPos(miLDR_YIndx) ;

    //if(_iMotr == miLDR_YIndx){
    //    if(bIndxFwd) {
    //        switch(_iPstnId) {
    //            default                   :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
    //            case piLDR_YIndxWait      : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
    //            case piLDR_YIndxGet       : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
    //            case piLDR_YIndxWorkStt   :                                                                                                                 break ;
    //            case piLDR_YIndxOut       : if(bExistPSL     && bExistLDR )  { sMsg = AnsiString(DM.ARAY[riPSL].GetName())+" is Exist"    ; bRet = false ;} break ;
    //            case piLDR_YIndxWork      :                                                                                                                 break ;

    //        }
    //    }
    //}

    //else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    //}

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CStageVT::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool bStgMtrStop = MT_GetStop(miSTG_TTble) ;


    double dStgPos = MT_GetCmdPos(miSTG_TTble);
    //double dAngle  = (dStgPos - PM.GetValue(miSTG_TTble , pvSTG_TTbleWork));
    int    iAngle  = dStgPos / 90 ;
    double dGap    = dStgPos - (iAngle * 90);
    double dTemp   = PM.GetValue(miSTG_TTble , pvSTG_TTbleWork) ;   //STG.GetMotrPos(miSTG_TTble , piSTG_TTbleWork) ;  //이곳은 오프셑 먹은게 기준이면 안될듯 하다.
    bool   bInpos  = dTemp - 0.5 < dGap && dGap < dTemp+0.5;


    //bool bStgMtrWorkZone = MT_GetCmdPos(miSTG_TTble) % 90 && MT_GetStop(miSTG_TTble)

    if(_iActr == aiVTI_PshrDnUp){
        if(_bFwd == ccFwd) {
            if(!bStgMtrStop) {sMsg = AnsiString("Stage Motor is Running!"          ); bRet = false ;}
            if(!bInpos     ) {sMsg = AnsiString("Stage Motor is Not Safe Position!"); bRet = false ;}
        }
    }
    else if(_iActr == aiVTI_PTolUpDn){
        if(_bFwd == ccFwd) {
            if(!bStgMtrStop) {sMsg = AnsiString("Stage Motor is Running!"); bRet = false ;}
            if(!bInpos     ) {sMsg = AnsiString("Stage Motor is Not Safe Position!"); bRet = false ;}
        }
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CStageVT::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CStageVT::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CStageVT::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CStageVT::SetLastCmd()
{
    //m_dLastIdxPos = MT_GetCmdPos(miLDR_YIndx);
    return ; //이파트는 필요 없다.

}

bool CStageVT::CheckMoved()
{   /*
    if(m_dLastIdxPos != MT_GetCmdPos(miLDR_YIndx)) return true  ; //이파트는 필요 없다.
    else                                           return false ;
    */
    return true;
}

bool CStageVT::CheckStop()
{
    //if(!MT_GetStop(miLDR_YIndx)) return false ;

    if(!AT_Done(aiVTI_PshrDnUp)) return false ;
    if(!AT_Done(aiVTI_PTolUpDn)) return false ;

    return true ;
}

void CStageVT::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}

bool CStageVT::SaveVTDevice(AnsiString _sDevice)
{

    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath  ;




    sPath  = OM.MstOptn.sVTDataFile1 ;
    if (!FileExists(sPath)) CreateDir(sPath);//폴더 뚫기 위해.
    if (FileExists(sPath)) DeleteFile(sPath);//원래 있던파일 지우고.
    try{UserINI.Save(sPath, "Change" , "Name" , _sDevice );}
    catch(...){FM_MsgOk("ERR","VT JobFile File 쓰기 실폐");}

    sPath  = OM.MstOptn.sVTDataFile2 ;
    if (!FileExists(sPath)) CreateDir(sPath);//폴더 뚫기 위해.
    if (FileExists(sPath)) DeleteFile(sPath);//원래 있던파일 지우고.
    try{UserINI.Save(sPath, "Change" , "Name" , _sDevice );}
    catch(...){FM_MsgOk("ERR","VT JobFile File 쓰기 실폐");}

    sPath  = OM.MstOptn.sVTDataFile3 ;
    if (!FileExists(sPath)) CreateDir(sPath);//폴더 뚫기 위해.
    if (FileExists(sPath)) DeleteFile(sPath);//원래 있던파일 지우고.
    try{UserINI.Save(sPath, "Change" , "Name" , _sDevice );}
    catch(...){FM_MsgOk("ERR","VT JobFile File 쓰기 실폐");}

    sPath  = OM.MstOptn.sVTDataFile4 ;
    if (!FileExists(sPath)) CreateDir(sPath);//폴더 뚫기 위해.
    if (FileExists(sPath)) DeleteFile(sPath);//원래 있던파일 지우고.
    try{UserINI.Save(sPath, "Change" , "Name" , _sDevice );}
    catch(...){FM_MsgOk("ERR","VT JobFile File 쓰기 실폐");}

    return true ;
}

bool CStageVT::GetVTRslt(int _iCol , EN_CHIP_STAT &_iStat)
{
    TUserINI    UserINI ;
    AnsiString  sPath ;
    int         iVTResult ;


    _iStat = OM.CmnOptn.iVTBLKBin+1;
         if(_iCol == 0) sPath = OM.MstOptn.sVTDataFile1 ;
    else if(_iCol == 1) sPath = OM.MstOptn.sVTDataFile2 ;
    else if(_iCol == 2) sPath = OM.MstOptn.sVTDataFile3 ;
    else if(_iCol == 3) sPath = OM.MstOptn.sVTDataFile4 ;
    else {Trace("Error", AnsiString("GetVTRslt Failed IdxOver").c_str()); return false ;}
    if (!FileExists(sPath)) {
        Trace("Error", AnsiString("GetVTRslt Failed Rslt=" +AnsiString(_iCol+1)).c_str()  );
        return false ;
    }

    UserINI.Load(sPath, "Result" , "VT" , iVTResult );

    UserFile.FileDelete(sPath+"bak");
    UserFile.FileCopy  (sPath, sPath+"bak");
    UserFile.FileDelete(sPath);


    const int GOOD_STAT = 0 ;
    const int BLK       = 1 ; //화면 안뜸 블랙
    const int DEFECT    = 2 ; //디펙트
    const int DIM       = 3 ; //얼룩. stan
    const int VH        = 4 ; //가로세로 줄간것.
    const int DEFECT2   = 5 ; //진성디펙트.
                                                         //OM.CmnOptn.iVTVHBin+1;
    AnsiString sTemp1 = "VT"+AnsiString(_iCol+1)+ " Rslt" ;
         if(iVTResult == GOOD_STAT) {_iStat = csGood ;                 Trace(sTemp1.c_str(), "GOOD_STAT");}
    else if(iVTResult == BLK      ) {_iStat = OM.CmnOptn.iVTBLKBin +1; Trace(sTemp1.c_str(), "BLK      ");}
    else if(iVTResult == DEFECT   ) {_iStat = OM.CmnOptn.iVTDFTBin +1; Trace(sTemp1.c_str(), "DEFECT   ");}
    else if(iVTResult == DEFECT2  ) {_iStat = OM.CmnOptn.iVTDFT2Bin+1; Trace(sTemp1.c_str(), "DEFECT2  ");}
    else if(iVTResult == DIM      ) {_iStat = OM.CmnOptn.iVTDIMBin +1; Trace(sTemp1.c_str(), "DIM      ");}
    else if(iVTResult == VH       ) {_iStat = OM.CmnOptn.iVTVHBin  +1; Trace(sTemp1.c_str(), "VH       ");}

    return true ;
}

//sunsun 90도씩 돌리는 것 하기...
//스테이지 라스트 인덱스 포지션 틀어짐 발생 원인 파악하기.
