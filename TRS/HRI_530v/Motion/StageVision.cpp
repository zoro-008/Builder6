//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "StageVision.h"
#include "Stage.h"
//#include "SortCmn.h"
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
CStageVision VSN;

CStageVision::CStageVision(void)
{

}

CStageVision::~CStageVision (void)
{

}

void CStageVision::Init()
{
    m_sPartName = "Stage Vision" ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_iNGCntPock1 = 0 ;
    m_iNGCntPock2 = 0 ;
    m_iNGCntPock3 = 0 ;
    m_iNGCntPock4 = 0 ;
}

void CStageVision::Close()
{
    Load(false);
}

void CStageVision::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CStageVision::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    if(DM.ARAY[riSTR].FindFrstColRow(csRslt11 , r,c)){
        return true ;
    }
    else {
        r=0 ;
        c=0 ;
        return false ;
    }

}

double CStageVision::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    const double dOfsforTrg = 5.0 ; //아진 인터럽트 트리거를 위한 옵셑.


    double dSTG_YVisnWorkStt1 = PM.GetValue(miSTG_YVisn , pvSTG_YVisnTrgStt1) - dOfsforTrg ;
    double dSTG_YVisnWorkEnd1 = PM.GetValue(miSTG_YVisn , pvSTG_YVisnTrgStt1) + dOfsforTrg + OM.DevInfo.dScktPitch * (MAX_TOOL_SOCK_COL - 1);
    double dSTG_YVisnWorkStt2 = PM.GetValue(miSTG_YVisn , pvSTG_YVisnTrgStt2) + dOfsforTrg ;
    double dSTG_YVisnWorkEnd2 = PM.GetValue(miSTG_YVisn , pvSTG_YVisnTrgStt2) - dOfsforTrg - OM.DevInfo.dScktPitch * (MAX_TOOL_SOCK_COL - 1);

    if(_iMotr == miSTG_YVisn){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSTG_YVisnWait     : dPos = PM.GetValue (_iMotr , pvSTG_YVisnWait    ); break ;
            case piSTG_YVisnTrgStt1  : dPos = PM.GetValue (_iMotr , pvSTG_YVisnTrgStt1 ); break ;
            case piSTG_YVisnTrgStt2  : dPos = PM.GetValue (_iMotr , pvSTG_YVisnTrgStt2 ); break ;
            case piSTG_YVisnWorkStt1 : dPos = dSTG_YVisnWorkStt1                        ; break ;
            case piSTG_YVisnWorkEnd1 : dPos = dSTG_YVisnWorkEnd1                        ; break ;
            case piSTG_YVisnWorkStt2 : dPos = dSTG_YVisnWorkStt2                        ; break ;
            case piSTG_YVisnWorkEnd2 : dPos = dSTG_YVisnWorkEnd2                        ; break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

typedef void ( *Func )();
void ThreadVisnReset()
{
    static bool bAlreadyDoing = false ;
    CDelayTimer Timer ;
    CDelayTimer RetryTimer ;

    if(bAlreadyDoing) return ;

    bAlreadyDoing = true ;

    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiVSN_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
            IO_SetY(yVSN_VsnReset1 , false) ;
            IO_SetY(yVSN_VsnReset2 , false) ;
            bAlreadyDoing = false ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Vision Communction", sTemp.c_str());
                      IO_SetY(yVSN_VsnReset1 , false) ;
                      IO_SetY(yVSN_VsnReset2 , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;

            case  10: //iSendCnt = 0 ;
                      IO_SetY(yVSN_VsnReset1 , false) ;
                      IO_SetY(yVSN_VsnReset2 , false) ;
                      if(iSendCnt > 1) {//비젼쪽에 레디 안풀리면 한번 더 보낸다.
                          EM_SetErr(eiVSN_TimeOut);
                          bAlreadyDoing = false ;
                          iStep = 0 ;
                          return ;
                      }
                      iStep++;
                      continue ;

            case  11: if(!OM.CmnOptn.bSkipVisn1 && IO_GetX(xVSN_VsnBusy1))IO_SetY(yVSN_VsnReset1 , true ) ;    //sun Visn1,Visn2 스킵 따로 가기에 따른 사용여부를 여기로 옮긴다.
                      if(!OM.CmnOptn.bSkipVisn2 && IO_GetX(xVSN_VsnBusy2))IO_SetY(yVSN_VsnReset2 , true ) ;
                      RetryTimer.Clear();
                      iStep++;
                      continue ;

            case  12: if(RetryTimer.OnDelay(true , 5000)){
                          IO_SetY(yVSN_VsnReset1 , false) ;
                          IO_SetY(yVSN_VsnReset2 , false) ;
                          iStep = 10 ;
                          iSendCnt++;
                          continue ;
                      }

                      if(!OM.CmnOptn.bSkipVisn1 &&IO_GetX(xVSN_VsnBusy1)) continue ;
                      if(!OM.CmnOptn.bSkipVisn2 &&IO_GetX(xVSN_VsnBusy2)) continue ;

                      IO_SetY(yVSN_VsnReset1 , false) ;
                      IO_SetY(yVSN_VsnReset2 , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;
        }
    }
}

void ThreadVisnJobChange()
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
            EM_SetErr(eiVSN_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
            IO_SetY(yVSN_VsnChang1 , false) ;
            IO_SetY(yVSN_VsnChang2 , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Vision Communction", sTemp.c_str());
                      IO_SetY(yVSN_VsnChang1 , false) ;
                      IO_SetY(yVSN_VsnChang2 , false) ;
                      iStep = 0 ;
                      return ;

            case  10: IO_SetY(yVSN_VsnChang1 , false) ;
                      IO_SetY(yVSN_VsnChang2 , false) ;
                      iStep++;
                      continue ;

            case  11: if(!OM.CmnOptn.bSkipVisn1 && IO_GetX(xVSN_VsnBusy1)){EM_SetErrMsg(eiVSN_Busy , "VISION1"); return ;}
                      if(!OM.CmnOptn.bSkipVisn2 && IO_GetX(xVSN_VsnBusy2)){EM_SetErrMsg(eiVSN_Busy , "VISION2"); return ;}

                      if(!OM.CmnOptn.bSkipVisn1)IO_SetY(yVSN_VsnChang1 , true) ;
                      if(!OM.CmnOptn.bSkipVisn2)IO_SetY(yVSN_VsnChang2 , true) ;

                      iStep++;
                      continue ;

            case  12: if(!OM.CmnOptn.bSkipVisn1&&!IO_GetX(xVSN_VsnBusy1)) continue ;
                      if(!OM.CmnOptn.bSkipVisn2&&!IO_GetX(xVSN_VsnBusy2)) continue ;

                      IO_SetY(yVSN_VsnChang1 , false) ;
                      IO_SetY(yVSN_VsnChang2 , false) ;

                      iStep++;
                      continue ;

            case  13: if(!OM.CmnOptn.bSkipVisn1&&IO_GetX(xVSN_VsnBusy1)) continue ;
                      if(!OM.CmnOptn.bSkipVisn2&&IO_GetX(xVSN_VsnBusy2)) continue ;

                      iStep = 0 ;
                      return ;
        }
    }
}

void ThreadVisnLotStart()         //3089 PLCC     
{
    static bool bAlreadyDoing = false ;
    CDelayTimer Timer ;

    if(bAlreadyDoing) return ;

    bAlreadyDoing = true ;

    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 3000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiVSN_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
            IO_SetY(yVSN_VsnLotStart1 , false) ;
            IO_SetY(yVSN_VsnLotStart2 , false) ;
            bAlreadyDoing = false ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Vision Communction", sTemp.c_str());
                      IO_SetY(yVSN_VsnLotStart1 , false) ;
                      IO_SetY(yVSN_VsnLotStart2 , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;

            case  10: //iSendCnt = 0 ;
                      IO_SetY(yVSN_VsnLotStart1 , false) ;
                      IO_SetY(yVSN_VsnLotStart2 , false) ;

                      if(!OM.CmnOptn.bSkipVisn1){
                          if(!IO_GetX(xVSN_VsnReady1)){
                              EM_SetErrMsg(eiVSN_NotReady , "Vision1");
                              bAlreadyDoing = false ;
                              iStep = 0 ;
                              return ;
                          }
                          if(IO_GetX(xVSN_VsnBusy1)){
                              EM_SetErrMsg(eiVSN_Busy , "Vision1");
                              bAlreadyDoing = false ;
                              iStep = 0 ;
                              return ;
                          }

                      }

                      if(!OM.CmnOptn.bSkipVisn2){
                          if(!IO_GetX(xVSN_VsnReady2)){
                              EM_SetErrMsg(eiVSN_NotReady , "Vision2");
                              bAlreadyDoing = false ;
                              iStep = 0 ;
                              return ;
                          }
                          if(IO_GetX(xVSN_VsnBusy2)){
                              EM_SetErrMsg(eiVSN_Busy , "Vision2");
                              bAlreadyDoing = false ;
                              iStep = 0 ;
                              return ;
                          }
                      }







                      iStep++;
                      continue ;

            case  11: if(!OM.CmnOptn.bSkipVisn1)IO_SetY(yVSN_VsnLotStart1 , true ) ;    //sun Visn1,Visn2 스킵 따로 가기에 따른 사용여부를 여기로 옮긴다.
                      if(!OM.CmnOptn.bSkipVisn2)IO_SetY(yVSN_VsnLotStart2 , true ) ;
                      iStep++;
                      continue ;

            case  12: if(!OM.CmnOptn.bSkipVisn1&&IO_GetX(xVSN_VsnReady1 )) continue ;
                      if(!OM.CmnOptn.bSkipVisn2&&IO_GetX(xVSN_VsnReady2 )) continue ;

                      IO_SetY(yVSN_VsnLotStart1 , false) ;
                      IO_SetY(yVSN_VsnLotStart2 , false) ;
                      iStep++;
                      continue ;

            case  13: if(!OM.CmnOptn.bSkipVisn1&&!IO_GetX(xVSN_VsnReady1 )) continue ;
                      if(!OM.CmnOptn.bSkipVisn2&&!IO_GetX(xVSN_VsnReady2 )) continue ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;
        }
    }
}

//그냥 여기서 Vision  Reset은 3개다 관장.

bool CStageVision::SaveVisnDevice(AnsiString _sDevice)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;
    int        sState      ;

    //Set Path.

    sPath = OM.MstOptn.sVisnDeviceFile ;
    sData = _sDevice;

    //폴더 뚫기 위해.
    //if (!FileExists(sPath)) CreateDir(sPath);

    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
    hFile = FileCreate(sPath.c_str());
    if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
    FileSeek (hFile , 0                , SEEK_END         );
    FileWrite(hFile , _sDevice.c_str() , _sDevice.Length());

    //Close File.
    FileClose(hFile);

    return true ;

}

bool CStageVision::SaveVisnLotNo(AnsiString _sLotNo)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;
    int        sState      ;

    //Set Path.

    sPath = OM.MstOptn.sVisnLotNoFile ;
    sData = _sLotNo;

    //폴더 뚫기 위해.
    //if (!FileExists(sPath)) CreateDir(sPath);

    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
    hFile = FileCreate(sPath.c_str());
    if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
    FileSeek (hFile , 0               , SEEK_END         );
    FileWrite(hFile , _sLotNo.c_str() , _sLotNo.Length());

    //Close File.
    FileClose(hFile);

    return true ;

}

bool CStageVision::VisnJobChange()
{
    if(!SaveVisnDevice(OM.GetCrntDev())) {
        FM_MsgOk("ERR","Vision Device 쓰기 실패");
        return false ;
    }
    CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnJobChange) ;
}

bool CStageVision::VisnLotStart()
{
    if(!SaveVisnLotNo(LOT.GetLotNo())) {
        FM_MsgOk("ERR","Vision LotNo 쓰기 실패");
        return false ;
    }
    CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnLotStart) ;
}


void CStageVision::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    bool bNeedResetVisn1 = IO_GetX(xVSN_VsnBusy1) && !OM.CmnOptn.bSkipVisn1 ;
    bool bNeedResetVisn2 = IO_GetX(xVSN_VsnBusy2) && !OM.CmnOptn.bSkipVisn2 ;

    if(bNeedResetVisn1 || bNeedResetVisn2){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnReset) ;
    }
}

bool CStageVision::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStageVision::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CStageVision::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveMotr(miSTG_YVisn , piSTG_YVisnWorkStt1);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miSTG_YVisn , piSTG_YVisnWorkStt1)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStageVision::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: MoveMotr(miSTG_YVisn , piSTG_YVisnWait);

                 Step.iToStop++;
                 return false;

        case 11: if(!MoveMotr(miSTG_YVisn , piSTG_YVisnWait))return false;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CStageVision::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool isCycleWork = DM.ARAY[riSTR].GetCntStat(csUnkwn) && MT_GetStopInpos(miSTG_TTble) && STG.GetSeqStep() == CStage::scIdle;
        bool isCycleEnd  = DM.ARAY[riSTR].CheckAllStat(csEmpty);



        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CStageVision::CycleHome()     //sun DLL direct access.
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

        case 10: MT_DoHome(miSTG_YVisn);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miSTG_YVisn)) return false ;
                 MT_GoAbsMan(miSTG_YVisn , PM.GetValue(miSTG_YVisn , pvSTG_YVisnWait ));
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GoAbsMan(miSTG_YVisn , PM.GetValue(miSTG_YVisn , pvSTG_YVisnWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}


void CStageVision::SetTrgPos(bool _bFrstVisn)
{
    double dTemp    ;
    int r,c ;

    AnsiString sTemp ;
    double * dTrgPos = new double[MAX_TOOL_SOCK_COL] ;


    double dTrgSttPos ;

    if(_bFrstVisn) {
        dTrgSttPos = PM.GetValue(miSTG_YVisn , pvSTG_YVisnTrgStt1) ;
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
            dTemp = dTrgSttPos + OM.DevInfo.dScktPitch * i;
            dTrgPos[i] = dTemp + OM.MstOptn.dTrigerOffset ;  //sun 트리거 옵셑 추가.
            sTemp += AnsiString(dTemp) + AnsiString(" , ") ;
        }
    }
    else {
        dTrgSttPos = PM.GetValue(miSTG_YVisn , pvSTG_YVisnTrgStt2) ;
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
            dTemp = dTrgSttPos - OM.DevInfo.dScktPitch * i;
            dTrgPos[i] = dTemp + OM.MstOptn.dTrigerOffset ;  //sun 트리거 옵셑 추가.
            sTemp += AnsiString(dTemp) + AnsiString(" , ") ;
        }
    }

    Trace("Trigger" , sTemp.c_str());

    MT_SetAxtTrgPos(miSTG_YVisn , MAX_TOOL_SOCK_COL , dTrgPos , 1000 , true , true) ;

    delete [] dTrgPos ;


}

void CStageVision::ResetTrgPos()
{
    MT_ResetAxtTrgPos(miSTG_YVisn) ;
}

bool CStageVision::GetVisnRslt(EN_ARAY_ID _iAray , int _iVisnNo)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    int iFailItm    ;
    int iSocketCnt = DM.ARAY[riSTR].GetMaxRow()+1;

    //bool bLtToRt = GetLtToRt() ;



    EN_CHIP_STAT iRslt ;

    if(_iVisnNo == 1) sPath = OM.MstOptn.sVisnRsltFile1 ;
    if(_iVisnNo == 2) sPath = OM.MstOptn.sVisnRsltFile2 ;

    if(!FileExists(sPath)) {
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
            DM.ARAY[_iAray].SetStat(i ,0, (EN_CHIP_STAT)(OM.CmnOptn.iVSNFailBin+1)) ;
        }
        Trace("Err",(sPath + " no file").c_str());
        return false ;
    }

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //메모리 동적 할당 나중에 해제하기...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace("Visn Data File" , AllField.c_str());

    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return false ; }

    AllField.Delete(1,3);

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0               ) { Trace("Err","iFailRow < 0"                ); return false ; }
        if(iFailRow > MAX_TOOL_SOCK_COL) { Trace("Err","iFailRow > MAX_TOOL_SOCK_COL"); return false ; }
        AllField.Delete(1,2);

        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0               ) { Trace("Err","iFailCol < 0"                ); return false ; }
        if(iFailCol > MAX_TOOL_SOCK_COL) { Trace("Err","iFailCol > MAX_TOOL_SOCK_COL"); return false ; }
        AllField.Delete(1,2);

        iFailItm = StrToIntDef("0x"+AllField.SubString(1,1),0);
        if(iFailItm <  0             ) { Trace("Err","iFailItm < 0"            ); return false ; }
        if(iFailItm >= MAX_CHIP_STAT ) { Trace("Err","iFailItm > MAX_CHIP_STAT"); return false ; }

        AllField.Delete(1,1);

        if(_iVisnNo == 1) {
           DM.ARAY[riMVS].SetStat(iFailRow-1 , 0 , (EN_CHIP_STAT)(OM.CmnOptn.iVSNFailBin+1));

           //if(ScmGetSckStat(_iAray,0,) != csEmpty){
           //    ScmSetSckStat(_iAray , 0,MAX_TOOL_SOCK_COL - (iFailRow-1)-1, (EN_CHIP_STAT)(OM.CmnOptn.iVSNFailBin+1)) ;
           //}
        }
        else {
           DM.ARAY[riMVS].SetStat( MAX_TOOL_SOCK_COL - iFailRow , 0 , (EN_CHIP_STAT)(OM.CmnOptn.iVSNFailBin+1));

           //if(ScmGetSckStat(_iAray,0,iFailRow-1) != csEmpty){
           //    ScmSetSckStat(_iAray , 0,iFailRow-1, (EN_CHIP_STAT)(OM.CmnOptn.iVSNFailBin+1)) ;
           //}
        }
    }

    delete [] pszBuffer;

    UserFile.FileDelete(sPath+"bak");
    UserFile.FileCopy  (sPath, sPath+"bak");
    UserFile.FileDelete(sPath);



    return true ;
}

//One Cycle.
bool CStageVision::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
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

    bool r1,r2 ;
    int r,c ;
    int iFailCnt ;

    EN_ARAY_ID riTarget ;
    EN_CHIP_STAT iStat;
    int iTemp ;

    const bool bMotionSkipOnInspSkip = true ; //VT스킵시에 모션도 스킵 하는것 .. 셑업할때 드라이런 써야 되서 한다.
    bool bSkipVsn1 = OM.CmnOptn.bSkipVisn1;
    bool bSkipVsn2 = OM.CmnOptn.bSkipVisn2;
    bool bSkipInsp = bSkipVsn1&&bSkipVsn2;


    bool bNeedInsp1 = DM.ARAY[riSTR].GetStat(3,0)!=csEmpty && !OM.CmnOptn.bSkip1stTol ; //ScmGetSckStat(ScmGetRearSTGAray(),0,0)!=csEmpty && !OM.CmnOptn.bSkip1stTol ;
    bool bNeedInsp2 = DM.ARAY[riSTR].GetStat(2,0)!=csEmpty && !OM.CmnOptn.bSkip2ndTol ;//ScmGetSckStat(ScmGetRearSTGAray(),0,1)!=csEmpty && !OM.CmnOptn.bSkip2ndTol ;
    bool bNeedInsp3 = DM.ARAY[riSTR].GetStat(1,0)!=csEmpty && !OM.CmnOptn.bSkip3thTol ;//ScmGetSckStat(ScmGetRearSTGAray(),0,2)!=csEmpty && !OM.CmnOptn.bSkip3thTol ;
    bool bNeedInsp4 = DM.ARAY[riSTR].GetStat(0,0)!=csEmpty && !OM.CmnOptn.bSkip4thTol ;//ScmGetSckStat(ScmGetRearSTGAray(),0,3)!=csEmpty && !OM.CmnOptn.bSkip4thTol ;

    static bool bBusyOn1 = false ;
    static bool bBusyOn2 = false ;

   // if(Step.iCycle
   //
   // = IO_GetX(xVSN_VsnBusy1)
   // IO_GetX(xVSN_VsnBusy2)

    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;



    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(bMotionSkipOnInspSkip && bSkipInsp) { //bMotionSkipOnInspSkip가 트루고 비젼 다 스킵일때는 그냥 모션도 안하고 끝남
                     for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                          iTemp = rand()%100;
                          if(iTemp < OM.MstOptn.iInspRandMask) {
                              iStat = OM.CmnOptn.iVSNFailBin+1 ; //콤보박스는 0부터시작 Rslt는 1부터 시작.
                          }
                          else {
                              iStat = csGood;
                          }
                          if(iStat == 0) iStat = OM.CmnOptn.iVSNFailBin +1;
                          else           iStat = csGood;
                          //if(DM.ARAY[ScmGetRghtSTGAray()].GetStat(r,0)!=csEmpty)DM.ARAY[riMVS].SetStat(r,0,(EN_CHIP_STAT)iStat);
                          if(DM.ARAY[riSTR].GetStat(r,0)!=csEmpty)DM.ARAY[riMVS].SetStat(r,0,(EN_CHIP_STAT)iStat);
                      }
                      for(int r=0 ; r < MAX_TOOL_SOCK_COL ; r++){
                          if(GetSeqStep()!=scIdle){ //오토런때만 실제 STG어레이에 마스킹 옮김.
                              iStat = DM.ARAY[riMVS].GetStat(r,0);
                              if(DM.ARAY[riSTR].GetStat(r,0)!=csEmpty) {
                                  DM.ARAY[riSTR].SetStat(r,0,iStat) ;
                              }
                          }
                      }

                     Step.iCycle=0;
                     return true ;
                  }


                  if(!bSkipVsn1){
                      if(!IO_GetX(xVSN_VsnReady1)){
                          EM_SetErrMsg(eiVSN_NotReady , "Vision1");
                          Step.iCycle=0;
                          return true ;
                      }

                  }

                  if(!bSkipVsn2){
                      if(!IO_GetX(xVSN_VsnReady2)){
                          EM_SetErrMsg(eiVSN_NotReady , "Vision2");
                          Step.iCycle=0;
                          return true ;
                      }
                  }



                  m_tmCycle.Clear();

                  Step.iCycle++;
                  return false;

        case  11: if(m_tmCycle.OnDelay(true , 1000)){
                      EM_SetErr(eiVSN_Busy);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!bSkipVsn1&&IO_GetX(xVSN_VsnBusy1)) return false;
                  if(!bSkipVsn2&&IO_GetX(xVSN_VsnBusy2)) return false;


                  MoveMotr(miSTG_YVisn , piSTG_YVisnWorkStt1);
                  IO_SetY(yVSN_TrgRellay2 , false);
                  IO_SetY(yVSN_TrgRellay1 , !bSkipVsn1);

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miSTG_YVisn , piSTG_YVisnWorkStt1)) return false ;
                  ResetTrgPos();
                  if(!bSkipVsn1)SetTrgPos(true);

                  MoveMotr(miSTG_YVisn , piSTG_YVisnWorkEnd1 , true);

                  Step.iCycle++;
                  return false ;



        case  13: if(!MoveMotr(miSTG_YVisn , piSTG_YVisnWorkEnd1 , true)) return false ;

                  //     if( bSkipVsn1 &&  bSkipVsn2) DM.ARAY[ScmGetRghtSTGAray()].SetStat(csGood);
                  //else if(!bSkipVsn1 &&  bSkipVsn2) GetVisnRslt(ScmGetRghtSTGAray() , DM.ARAY[ScmGetRghtSTGAray()].GetMaxCol());
                  //else if( bSkipVsn1 && !bSkipVsn2) DM.ARAY[ScmGetRghtSTGAray()].SetStat(csGood);
                  //else                              GetVisnRslt(ScmGetRghtSTGAray() , DM.ARAY[ScmGetRghtSTGAray()].GetMaxCol());

                  ResetTrgPos();
                  //sun 트리거 스위치 2번으로.
                  IO_SetY(yVSN_TrgRellay1 , false);
                  if(!bSkipVsn2)IO_SetY(yVSN_TrgRellay2 , true);

                  MoveMotr(miSTG_YVisn , piSTG_YVisnWorkStt2);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miSTG_YVisn , piSTG_YVisnWorkStt2)) return false ;

                  if(!bSkipVsn2)SetTrgPos(false);

                  MoveMotr(miSTG_YVisn , piSTG_YVisnWorkEnd2 , true);
                  Step.iCycle++;
                  return false ;


        case  15: if(!MoveMotr(miSTG_YVisn , piSTG_YVisnWorkEnd2  , true)) return false ;

                  ResetTrgPos();

                  if(!bSkipVsn1 && IO_GetX(xVSN_VsnBusy1)) return false ;
                  if(!bSkipVsn2 && IO_GetX(xVSN_VsnBusy2)) return false ;

                  IO_SetY(yVSN_TrgRellay1 , false);
                  IO_SetY(yVSN_TrgRellay2 , false);

                  for(int r=0 ; r < MAX_TOOL_SOCK_COL ; r++) {
                      DM.ARAY[riMVS].SetStat(csGood);
                  }

                  if(OM.MstOptn.iInspRandMask) {  //랜덤 마킹.
                      for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                          iTemp = rand()%100;
                          if(iTemp < OM.MstOptn.iInspRandMask) {
                              iStat = OM.CmnOptn.iVSNFailBin+1 ; //콤보박스는 0부터시작 Rslt는 1부터 시작.
                          }
                          else {
                              iStat = csGood;
                          }
                          if(iStat == 0) iStat = OM.CmnOptn.iVSNFailBin +1;
                          else           iStat = csGood;
                          DM.ARAY[riMVS].SetStat(r,0,(EN_CHIP_STAT)iStat);
                      }
                  }
                  else { //정상 플로우
                      if(!bSkipVsn1) {
                          if(!GetVisnRslt(riMVS , 1)) {
                              EM_SetErrMsg(eiVSN_WrongData,"vision1 GetResult Failed");
                              Step.iCycle=0;
                              return true ;
                          }
                      }
                      if(!bSkipVsn2) {
                          if(!GetVisnRslt(riMVS , 2)) {
                              EM_SetErrMsg(eiVSN_WrongData,"vision2 GetResult Failed");
                              Step.iCycle=0;
                              return true ;
                          }
                      }
                  }

                  for(int r=0 ; r < MAX_TOOL_SOCK_COL ; r++){
                      if(GetSeqStep()!=scIdle){ //오토런때만 실제 STG어레이에 마스킹 옮김.
                          iStat = DM.ARAY[riMVS].GetStat(r,0);
                          if(DM.ARAY[riSTR].GetStat(r,0)!=csEmpty) { //자제가 실제 포켓에 있는놈만 데이터 스테이지로 옮김.
                              DM.ARAY[riSTR].SetStat(r,0,iStat) ;
                          }
                      }
                  }

                  /*
                  const int iOSTRslt = 0 ;
                  const int iVTRslt  = 1 ;
                  const int iVSNRslt = 2 ;


                  */

                  if(GetSeqStep() != scIdle) {  //메뉴얼로 세팅할때 자꾸 스테이지 돌려 놓고 검사 하면 삑사리 나서...
                      if(DM.ARAY[riSTR].GetStat(3,0) != csEmpty&&
                         DM.ARAY[riSTR].GetSubStat(3,0,iOSTRslt) == csGood &&  //OST,VT모두 굿인상태에서만.
                         DM.ARAY[riSTR].GetSubStat(3,0,iVTRslt ) == csGood) {
                          if(DM.ARAY[riSTR].GetStat(3,0) == (EN_CHIP_STAT)OM.CmnOptn.iVSNFailBin+1) m_iNGCntPock1++ ;
                          else                                                                      m_iNGCntPock1=0 ;
                      }
                      if(DM.ARAY[riSTR].GetStat(2,0) != csEmpty&&
                         DM.ARAY[riSTR].GetSubStat(2,0,iOSTRslt) == csGood &&  //OST,VT모두 굿인상태에서만.
                         DM.ARAY[riSTR].GetSubStat(2,0,iVTRslt ) == csGood) {
                          if(DM.ARAY[riSTR].GetStat(2,0) == (EN_CHIP_STAT)OM.CmnOptn.iVSNFailBin+1) m_iNGCntPock2++ ;
                          else                                                                      m_iNGCntPock2=0 ;
                      }
                      if(DM.ARAY[riSTR].GetStat(1,0) != csEmpty&&
                         DM.ARAY[riSTR].GetSubStat(1,0,iOSTRslt) == csGood &&  //OST,VT모두 굿인상태에서만.
                         DM.ARAY[riSTR].GetSubStat(1,0,iVTRslt ) == csGood) {
                          if(DM.ARAY[riSTR].GetStat(1,0) == (EN_CHIP_STAT)OM.CmnOptn.iVSNFailBin+1) m_iNGCntPock3++ ;
                          else                                                                      m_iNGCntPock3=0 ;
                      }
                      if(DM.ARAY[riSTR].GetStat(0,0) != csEmpty&&
                         DM.ARAY[riSTR].GetSubStat(0,0,iOSTRslt) == csGood &&  //OST,VT모두 굿인상태에서만.
                         DM.ARAY[riSTR].GetSubStat(0,0,iVTRslt ) == csGood) {
                          if(DM.ARAY[riSTR].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iVSNFailBin+1) m_iNGCntPock4++ ;
                          else                                                                      m_iNGCntPock4=0 ;
                      }


                      if(OM.CmnOptn.iVSContNGCnt != 0){ //0으로 설정 되어 있으면 사용 안함.
                          sTemp = "";
                          //if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock1){EM_SetErrMsg(eiVSN_SerialNG , "1번포켓 VISION 불량 연속"); m_iNGCntPock1 = 0 ;}
                          //if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock2){EM_SetErrMsg(eiVSN_SerialNG , "2번포켓 VISION 불량 연속"); m_iNGCntPock2 = 0 ;}
                          //if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock3){EM_SetErrMsg(eiVSN_SerialNG , "3번포켓 VISION 불량 연속"); m_iNGCntPock3 = 0 ;}
                          //if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock4){EM_SetErrMsg(eiVSN_SerialNG , "4번포켓 VISION 불량 연속"); m_iNGCntPock4 = 0 ;}

                          if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock1){
                              if(sTemp=="") sTemp =  "1";
                              else          sTemp+= ",1";
                          }
                          if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock2){
                              if(sTemp=="") sTemp =  "2";
                              else          sTemp+= ",2";
                          }
                          if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock3){
                              if(sTemp=="") sTemp =  "3";
                              else          sTemp+= ",3";
                          }
                          if(OM.CmnOptn.iVSContNGCnt<=m_iNGCntPock4){
                              if(sTemp=="") sTemp =  "4";
                              else          sTemp+= ",4";
                          }
                          if(sTemp != "") { //만약 한포켓이라도 메세지가 세팅이 되었으면.\
                              Trace("m_iNGCntPock",(String(m_iNGCntPock1) + "_" + String(m_iNGCntPock2) + "_" + String(m_iNGCntPock3) + "_" + String(m_iNGCntPock4)).c_str() );
                              m_iNGCntPock1 = 0 ;
                              m_iNGCntPock2 = 0 ;
                              m_iNGCntPock3 = 0 ;
                              m_iNGCntPock4 = 0 ;
                              EM_SetErrMsg(eiVSN_SerialNG , (sTemp + "번포켓 VISION 불량 연속").c_str());
                          }
                      }
                 }


                 Step.iCycle=0;
                 return true;
    }

}

//---------------------------------------------------------------------------
bool CStageVision::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miSTG_YVisn){
        switch(_iPstnId) {
            default                  :  break ;
            case piSTG_YVisnWait     :  break ;
            case piSTG_YVisnTrgStt1  :  break ;
            case piSTG_YVisnTrgStt2  :  break ;
            case piSTG_YVisnWorkStt1 :  break ;
            case piSTG_YVisnWorkEnd1 :  break ;
            case piSTG_YVisnWorkStt2 :  break ;
            case piSTG_YVisnWorkEnd2 :  break ;
        }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CStageVision::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    //if(_iActr == aiLDR_IndxUpUp){
    //    if(_bFwd == ccFwd) {
    //        //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
    //    }
    //}
    //else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    //}


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

//bool CStageVision::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
//{
//    if (!CheckSafe(_iMotr , _iPstnId)) return false;
//
//    double dPosition = GetMotrPos(_iMotr , _iPstnId);
//
//    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
//    else            return MT_GoAbsMan(_iMotr , dPosition);
//}

bool CStageVision::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }
}







void CStageVision::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CStageVision::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CStageVision::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miSTG_YVisn);
    return ; //이파트는 필요 없다.

}

bool CStageVision::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miSTG_YVisn)) return true  ; //이파트는 필요 없다.
    else                                           return false ;

}

bool CStageVision::CheckStop()
{
    if(!MT_GetStop(miSTG_YVisn)) return false ;

    //if(!AT_Done(aiLDR_IndxUpUp)) return false ;

    return true ;
}

void CStageVision::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if(m_dLastIdxPos > MT_GetMaxPos(miSTG_YVisn)) m_dLastIdxPos = MT_GetMaxPos(miSTG_YVisn) ;
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}





