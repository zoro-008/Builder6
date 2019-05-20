//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Work.h"
#include "ProbTester.h"
#include "Sort.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#include "Rail.h"

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CWork WRK;

CWork::CWork()
{
    Init();
}
CWork::~CWork (void)
{
    Close();
}
void CWork::Init()
{
    m_sPartName   = "Work " ;

    m_iHomeTO  = (int)eiWRK_HomeTO    ;
    m_iStartTO = (int)eiWRK_ToStartTO ;
    m_iStopTO  = (int)eiWRK_ToStopTO  ;
    m_iCycleTO = (int)eiWRK_CycleTO   ;

    Reset();
    m_iFailCnt = 0 ;
    m_iPinCnt  = 0 ;
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

bool CWork::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    //int r1,c1;
    //if(_riWhre == riRAE) {
    //    c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
    //    return (c > -1 && c > -1);
    //}
    return false ;
}

double CWork::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miWRK_ZPrb){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                  ); break ;
            case piWRK_ZPrbWait : dPos = PM.GetValue (_iMotr , pvWRK_ZPrbWait ); break ;
            case piWRK_ZPrbWork : dPos = PM.GetValue (_iMotr , pvWRK_ZPrbWork ); break ;
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

    if(_iMotr == miWRK_ZPrb){
        //sMsg = "Epoxy_Z Axis is Not Wait Position" ;
        //bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CWork::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isZMoving = !MT_GetStopInpos(miWRK_ZPrb);

    //if(_iActr == aiWRK_IdxUpDn ){
    //    //sMsg = "Epoxy_X_Index is Moving" ;
    //    //bRet = false ;
    //}

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
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
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_Reset(miWRK_ZPrb);
                 MT_SetServo(miWRK_ZPrb,true);
                 Step.iHome++;
                 return false ;

        case 11: MT_DoHome(miWRK_ZPrb);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRK_ZPrb))return false ;
                 MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb , pvWRK_ZPrbWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWRK_ZPrb , PM.GetValue(miWRK_ZPrb , pvWRK_ZPrbWait))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CWork::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: MoveMotr(miWRK_ZPrb,piWRK_ZPrbWait);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRK_ZPrb,piWRK_ZPrbWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWork::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

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
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop);
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: MoveMotr(miWRK_ZPrb,piWRK_ZPrbWait);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRK_ZPrb,piWRK_ZPrbWait)) return false ;
                 Step.iToStop = 0;
                 return true ;
    }
}

bool CWork::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = 1;//Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Judgment
        bool isCycleWork = DM.ARAY[riWRK].CheckAllStat(csUnkwn) ;//&& !DM.ARAY[riWRK].CheckAllStat((EN_CHIP_STAT)OM.DevOptn.iEpmtyBin);
        bool isConEnd    = !isCycleWork ;


        //모르는 스트립에러.
        //if( DM.ARAY[riWRK ].CheckAllStat(csNone) &&  IO_GetX(xWRK_PkgExist)) EM_SetErr(eiWRK_PkgUnknown ) ;

        //스트립 사라짐 에러.
        //if(!DM.ARAY[riWRK ].CheckAllStat(csNone) && !IO_GetX(xWRK_PkgExist)) EM_SetErr(eiWRK_PkgDispr   ) ;
        //주석 처리 이유 : 자재 Masking 해주는 것과 자재 위치가 맞지 않음.

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleWork ) {Trace(m_sPartName.c_str(),"CycleWork  Stt"); Step.iSeq = scWork ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default     :                   Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle :                                                                                    return false ;
        case scWork : if(CycleWork ()){ Trace(m_sPartName.c_str(),"CycleWork End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.
bool CWork::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait);
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
    //    return true;
    //}

    int iTemp = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10:
                  Step.iCycle++;
                  return false;

        case  11: if(m_iPinCnt >= OM.DevOptn.iPinChangeCnt) {
                      EM_SetErr(eiWRK_PinChange);
                      MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  MoveMotr(miWRK_ZPrb , piWRK_ZPrbWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZPrb , piWRK_ZPrbWork)) return false ;
                  m_iPinCnt++;
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iPrbStartDelay)) return false ;
                  if(!OM.MstOptn.bInspSkip) PTR.StartCycle(pcProbe);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!OM.MstOptn.bInspSkip) {
                      if(!PTR.GetCycleEnd())return false ;

                      if(PTR.GetProbeRslt() == prTimeOut) {
                          EM_SetErr(eiWRK_PrbTestTO);
                          MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait);
                          Step.iCycle = 0 ;
                          return true ;
                      }  //여기 나중에 빈 어떻게 사용해야 하는지 답변 오면 수정.
                      else if(PTR.GetProbeRslt() == prFail   ) {
                          DM.ARAY[riWRK].SetStat(csRslt3);
//                          SRT.SetBin(2,true);
//                          SRT.SetSort(true);
//                          SRT.AddBinCnt(2,1);
                          m_iFailCnt ++ ;
                          if(m_iFailCnt >= OM.DevOptn.iFailCnt) {
                              EM_SetErr(eiWRK_FailCnt);
                              MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait);
                              m_iFailCnt  = 0 ;
                              Step.iCycle = 0 ;
                              return true ;
                          }
                      }
                      else if(PTR.GetProbeRslt() == prGood) {
                          if(SRT.GetBinCnt(1) < OM.DevOptn.iBin1FullCnt){
                              DM.ARAY[riWRK].SetStat(csRslt1);
//                              SRT.SetBin(1,true);
//                              SRT.SetSort(true);
//                              SRT.AddBinCnt(1,1);
                              m_iFailCnt  = 0 ;
                          }
                          else {
                              DM.ARAY[riWRK].SetStat(csRslt2);
                              m_iFailCnt  = 0 ;
                          }
                      }
//                      else if(PTR.GetProbeRslt() == prFwdGood) {
//                          DM.ARAY[riWRK].SetStat(csRslt1);
//                          SRT.SetBin(1);
//                          SRT.AddBinCnt(1,1);
//                      }
                      else if(PTR.GetProbeRslt() == prError  ) {EM_SetErr(eiWRK_PrbTestEr);}
                      else                                     {EM_SetErr(eiWRK_PrbTestEr);}
                  }
                  else{
                      iTemp = rand()%2 ;
                      if(iTemp == 0) {
                          DM.ARAY[riWRK].SetStat(csRslt1);
//                          SRT.SetBin(1,true);
//                          SRT.SetSort(true);
//                          SRT.AddBinCnt(1,1);
                          m_iFailCnt  = 0 ;
                      }
                      else {
                          DM.ARAY[riWRK].SetStat(csRslt2);
//                          SRT.SetBin(2,true);
//                          SRT.SetSort(true);
//                          SRT.AddBinCnt(2,1);
                          m_iFailCnt ++ ;
                          if(m_iFailCnt >= OM.DevOptn.iFailCnt) {
                              EM_SetErr(eiWRK_FailCnt);
                              m_iFailCnt  = 0 ;
                              MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait);
                              Step.iCycle = 0 ;
                              return true ;
                          }

                      }

                  }

//                  CSort::SStat Stat ;
//                  Stat = SRT.GetStat() ;
//                  Stat.bSortMove  = true ;
//                  SRT.SetStat(Stat);

                  LT.AddEqpInfoWorkChip(1);
                  LT.AddDayInfoWorkChip(1);
                  LT.AddLotInfoWorkChip(1);
//                  LT.AddEqpInfoWorkChip(1);

                  if(PTR.GetProbeRslt() == prFail) {
                      LT.AddDayInfoFailChip(1);
                      LT.AddLotInfoFailChip(1);
                      LT.AddEqpInfoFailChip(1);

                  }

                  //나중에 여기서 랏 관련 계산.

                  MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRK_ZPrb , piWRK_ZPrbWait)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

double CWork::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CWork::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CWork::CheckStop()
{
    if(!MT_GetStop(miWRK_ZPrb)) return false ;

    //if(!AT_Done(aiWRK_IdxUpDn )) return false ;

    return true ;
}

void CWork::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iFailCnt " , m_iFailCnt );
        UserINI.Load(sPath, "Member" , "m_iPinCnt  " , m_iPinCnt  );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iFailCnt " , m_iFailCnt );
        UserINI.Save(sPath, "Member" , "m_iPinCnt  " , m_iPinCnt  );
    }
}
//---------------------------------------------------------------------------

