//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PostBuff.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "UnLoader.h"
#include "Epoxy.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPostBuff PSB;

CPostBuff::CPostBuff()
{
    Init();

    AT_MoveCyl(aiPSB_Clamp,ccBwd);

}
CPostBuff::~CPostBuff (void)
{
    Close();
}
void CPostBuff::Init()
{
    m_sPartName   = "PostBuff " ;
    m_dLastIdxPos = 0.0 ;

    m_sWorkedId    = "";
    m_sWorkedLotNo = "";

    m_iHomeTO  = (int)eiPSB_HomeTO    ;
    m_iStartTO = (int)eiPSB_ToStartTO ;
    m_iStopTO  = (int)eiPSB_ToStopTO  ;
    m_iCycleTO = (int)eiPSB_CycleTO   ;

//    double dTemp = PM.GetValue (miPSB_XIdx , pvPSB_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1)) ;
//    PM.SetPos(miPSB_XIdx,pvPSB_XIdxPull,dTemp);

    Reset();
    Load(true);
}
void CPostBuff::Close()
{
    Load(false);
}
void CPostBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPostBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}
double CPostBuff::GetWorkPos(EN_MOTR_ID _iMotr)
{
/*
    if(_iMotr == miWRE_XIdx){
        double dWorkPos = 0.0 ;
        int r  , c  ;
        FindChip(riPSB,r,c);

        c = OM.DevInfo.iColCnt - c - 1 ;
        dWorkPos = PM.GetValue (_iMotr , pvPSB_XIdxWorkStt ) + ( OM.DevInfo.dColPitch * c ) ;
        return dWorkPos ;
    //    return MT_CmprPos(miPSB_XIdx , dWorkPos);
    }
    else return 0.0 ;
*/
    return  MT_GetCmdPos(_iMotr);
}


bool CPostBuff::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
/*
    int r1,c1;
    if(_riWhre == riPSB) {
        c = DM.ARAY[riPSB].FindLastCol(csUnkwn) ;
//        c = OM.DevInfo.iColCnt - c - 1 ;
        return (c > -1 && c > -1);
    }
    else return false ;
*/
    return false ;
}

double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;
    int r = -1 , c = -1 ;

    if(_iMotr == miPSB_XIdx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piPSB_XIdxWait    : dPos = PM.GetValue (_iMotr , pvPSB_XIdxWait    ); break ;
            case piPSB_XIdxClamp   : dPos = PM.GetValue (_iMotr , piPSB_XIdxClamp   ); break ;
            case piPSB_XIdxPull    : dPos = PM.GetValue (_iMotr , pvPSB_XIdxPull    ); break ;
            case piPSB_XIdxBack    : dPos = PM.GetValue (_iMotr , pvPSB_XIdxBack    ); break ;
            case piPSB_XIdxOut1    : dPos = PM.GetValue (_iMotr , pvPSB_XIdxOut1    ); break ;
            case piPSB_XIdxOut2    : dPos = PM.GetValue (_iMotr , pvPSB_XIdxOut2    ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

//    bool isEpxZWaitPs = MT_GetStopInpos(miPSB_ZEpx) && MT_CmprPos(miPSB_ZEpx,PM.GetValue(miPSB_ZEpx,pvPSB_ZEpxWait)) ;

    const double dIdxCrashArea = 310.0 ; //나중에 마스터 옵션으로 빼기



    if(_iMotr == miPSB_XIdx){
        if(MT_GetTrgPos(miWRD_XIdx) + GetMotrPos(_iMotr , _iPstnId) > dIdxCrashArea) {sMsg = "Index Crash!" ; bRet = false ;}
        switch(_iPstnId) {
            default                : break ;
            case piPSB_XIdxWait    : break ;
            case piPSB_XIdxClamp   : break ;
            case piPSB_XIdxPull    : break ;
            case piPSB_XIdxBack    : break ;
            case piPSB_XIdxOut1    : break ;
            case piPSB_XIdxOut2    : break ;
        }
    }
    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = !MT_GetStopInpos(miPSB_XIdx);

    if(_iActr == aiPSB_Clamp){
        if(isXMoving) {sMsg = "PSB_X is Moving" ; bRet = false ;}
    }
    else {
        sMsg = AT_GetName(_iActr) + " is Not this part Cylinder!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CPostBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPostBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPostBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmHome.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
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
    static bool bPSBNone = true ;

    switch (Step.iHome) {

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miPSB_XIdx,false);
                 AT_MoveCyl(aiPSB_Clamp  , ccBwd);
                 bPSBNone = DM.ARAY[riRAP].CheckAllStat(csNone) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Clamp  , ccBwd))return false ;
                 MT_DoHome(miPSB_XIdx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XIdx))return false ;
                 if(bPSBNone) {
                     MT_GoAbsMan(miPSB_XIdx , PM.GetValue(miPSB_XIdx , pvPSB_XIdxWait  ));
                 }
                 else {
                     MT_GoAbsMan(miPSB_XIdx , m_dLastIdxPos);
//                     return false ;
                 }
                 Step.iHome++;
                 return false ;

        case 13: if(bPSBNone) {
                     if(!MT_GoAbsMan(miPSB_XIdx , PM.GetValue(miPSB_XIdx , pvPSB_XIdxWait  ))) return false ;
                 }
                 else {
                     if(!MT_GoAbsMan(miPSB_XIdx , m_dLastIdxPos                             )) return false ;
                 }

//                 if(!bPSBNone) AT_MoveCyl(aiPSB_Clamp,ccFwd);
//
//                 Step.iHome++;
//                 return false ;
//
//        case 14: if(!bPSBNone) if(!AT_MoveCyl(aiPSB_Clamp,ccFwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPostBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CPostBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPostBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bPSBNone = true ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: bPSBNone = DM.ARAY[riRAP].CheckAllStat(csNone);
                 if( bPSBNone)MoveActr(aiPSB_Clamp  ,ccBwd);
//                 if(!bPSBNone)MoveActr(aiPSB_Clamp  ,ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if( bPSBNone) if(!MoveActr(aiPSB_Clamp  ,ccBwd)) return false ;
                 if( bPSBNone)MoveMotr(miPSB_XIdx,piPSB_XIdxWait);
//                 if(!bPSBNone) if(!MoveActr(aiPSB_Clamp  ,ccFwd)) return false ;
                 Step.iToStart ++ ;
                 return false ;

        case 12: if( bPSBNone) if(!MoveMotr(miPSB_XIdx,piPSB_XIdxWait)) return false ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPostBuff::ToStop(void) //스탑을 하기 위한 함수.
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
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: if(!MT_GetStopInpos(miPSB_XIdx)) return false ;
                 m_dLastIdxPos = GetLastCmd(miPSB_XIdx);
                 MoveActr(aiPSB_Clamp,ccBwd);
                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Clamp,ccBwd)) return false ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CPostBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Check         miWRD_XIdx
        int  iReGribStep  = DM.ARAY[riRAP].GetStep() ;
        //bool isWRD_XCrash = PM.GetValue(miWRD_XIdx , pvWRD_XHedStripWait) < MT_GetEncPos(miWRD_YHed) ;
        bool bAllNone     = DM.ARAY[riRAP].CheckAllStat(csNone) ;
        bool bReGrip      = iReGribStep < OM.DevOptn.iPSB_XRgCnt + 1 ;

        //Judgment
        bool isCyclePull     = !bAllNone && iReGribStep == 0 ;
        bool isCycleBackOut  = !bAllNone && iReGribStep >  0 && bReGrip && DM.ARAY[riULD].GetCntStat(csEmpty) &&
                                MT_CmprPos(miULD_ZCmp ,ULD.GetWorkPos(miULD_ZCmp )) &&
                                ULD.GetSeqStep() == CUnLoader :: scIdle ;

        bool isConEnd       =  bAllNone ;

//        if( DM.ARAY[riPSB].CheckAllStat(csNone) &&  )  EM_SetErr(eiPSB_Unknwn) ;
//        if(!DM.ARAY[riPSB].CheckAllStat(csNone) &&  )  EM_SetErr(eiPSB_Disp  ) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCyclePull    ) {Trace(m_sPartName.c_str(),"CyclePull     Stt"); Step.iSeq = scPull    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBackOut ) {Trace(m_sPartName.c_str(),"CycleBackOut  Stt"); Step.iSeq = scBackOut ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                      Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                          return false ;
        case scPull    : if(CyclePull    ()){ Trace(m_sPartName.c_str(),"CyclePull    End");Step.iSeq = scIdle ;} return false ;
        case scBackOut : if(CycleBackOut ()){ Trace(m_sPartName.c_str(),"CycleBackOut End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.
bool CPostBuff::CyclePull(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int      r1,r2 ;
    bool     bTemp ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Clamp, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Clamp, ccBwd)) return false ;

                  MT_GoAbs(miPSB_XIdx , PM.GetValue(miPSB_XIdx,pvPSB_XIdxClamp) , MT_GetRunVel(miWRD_XIdx)); 
//                  MoveMotr(miPSB_XIdx , piPSB_XIdxWait );
                  Step.iCycle++;
                  return false;

        case  12: //if(!MoveMotr(miPSB_XIdx , piPSB_XIdxWait )) return false ;
                  if(!MT_GoAbs(miPSB_XIdx , PM.GetValue(miPSB_XIdx,pvPSB_XIdxClamp) , MT_GetRunVel(miWRD_XIdx) )) return false ;
                  MoveActr(aiPSB_Clamp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Clamp, ccFwd)) return false ;
                  MoveMotr(miPSB_XIdx , piPSB_XIdxPull  );
                  DM.ARAY[riRAP].SetStep(1);                  
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XIdx , piPSB_XIdxPull  )) return false ;
//                  DM.ARAY[riRAP].SetStep(1);

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPostBuff::CycleBackOut(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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
    //    return true;
    //}

    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!MT_CmprPos(miPSB_XIdx,PM.GetValue(miPSB_XIdx,pvPSB_XIdxBack))) MoveActr(aiPSB_Clamp  , ccBwd); //
                  Step.iCycle++;
                  return false;

        case  11: if(!MT_CmprPos(miPSB_XIdx,PM.GetValue(miPSB_XIdx,pvPSB_XIdxBack))) if(!MoveActr(aiPSB_Clamp  , ccBwd)) return false ;
                  MoveMotr(miPSB_XIdx,piPSB_XIdxBack);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XIdx,piPSB_XIdxBack)) return false ;
                  MoveActr(aiPSB_Clamp  , ccFwd); //Pull 포지션을 셋업창에 하나 두어주자.
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Clamp  , ccFwd)) return false ;
                  if(DM.ARAY[riRAP].GetStep() >= OM.DevOptn.iPSB_XRgCnt) MT_GoAbsSlow(miPSB_XIdx,GetMotrPos(miPSB_XIdx,piPSB_XIdxOut2) );
                  else                                                   MoveMotr(miPSB_XIdx,piPSB_XIdxOut1);

                  Step.iCycle++;
                  return false;

        case  14: if(DM.ARAY[riRAP].GetStep() >= OM.DevOptn.iPSB_XRgCnt) {if(!MT_GoAbsSlow(miPSB_XIdx,GetMotrPos(miPSB_XIdx,piPSB_XIdxOut2) )) return false ;}
                  else                                                   {if(!MoveMotr(miPSB_XIdx,piPSB_XIdxOut1)) return false ;}
                  if(DM.ARAY[riRAP].GetStep() >= OM.DevOptn.iPSB_XRgCnt) {MoveMotr(miPSB_XIdx,piPSB_XIdxWait); }
                  else                                                   {MoveActr(aiPSB_Clamp, ccBwd);        }
                  Step.iCycle++;
                  return false ;

        case  15: if(DM.ARAY[riRAP].GetStep() >= OM.DevOptn.iPSB_XRgCnt) {if(!MoveMotr(miPSB_XIdx,piPSB_XIdxWait)) return false; }
                  //else                                                   {MoveActr(aiPSB_Clamp, ccBwd);        }


                  if(!MoveActr(aiPSB_Clamp, ccBwd)) return false ;

                  m_sWorkedId    = DM.ARAY[riRAP].GetID   ();
                  m_sWorkedLotNo = DM.ARAY[riRAP].GetLotNo();

                  if(DM.ARAY[riRAP].GetStep() >=  OM.DevOptn.iPSB_XRgCnt && IO_GetX(xPSB_OutDetect)) {
                      EM_SetErr(eiPSB_OutFeedingFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  else if (DM.ARAY[riRAP].GetStep() >=  OM.DevOptn.iPSB_XRgCnt && !IO_GetX(xPSB_OutDetect)) {
                      LotInfoAdd(&DM.ARAY[riPSB]);
                      if(DM.ARAY[riULD].CheckAllStat(csEmpty)) {
                          DM.ARAY[riULD].SetLotNo(DM.ARAY[riRAP].GetLotNo());
                          DM.ARAY[riULD].SetID   (DM.ARAY[riRAP].GetID   ());
                      }
                      ULD.FindChip(riULD,r,c);
                      DM.ARAY[riULD].SetStat(r,0 , csWork);
                      DM.ARAY[riRAP].SetStat(csNone) ;
                      DM.ARAY[riPSB].SetStat(csNone) ;

                      MoveMotr(miPSB_XIdx,piPSB_XIdxWait);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  DM.ARAY[riRAP].SetStep(DM.ARAY[riRAP].GetStep() + 1 );

                  MoveMotr(miPSB_XIdx,piPSB_XIdxBack);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPostBuff::LotInfoAdd(CArray * Array)
{
    //Data Map Save
    SPC.WriteArayData(Array);

//    Array.ChangeStat(csUnkwn,csWork);
    int iWorkCnt = Array->GetCntStat(csWork) ;
    LT.AddEqpInfoWorkChip(iWorkCnt);
    LT.AddEqpInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - iWorkCnt );

    LT.AddDayInfoWorkStrp(1);
    LT.AddDayInfoWorkChip(iWorkCnt);
    LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - iWorkCnt );

    LT.AddLotInfoWorkStrp(1);
    LT.AddLotInfoWorkChip(iWorkCnt);
    LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - iWorkCnt );

    OM.EqpOptn.iTotalChip     += iWorkCnt ;
    OM.EqpOptn.iTotalFailChip += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - iWorkCnt;// - Array->GetCntStat(csRslt1) ;

    if(OM.EqpOptn.iTotalChip < 0 || OM.EqpOptn.iTotalFailChip < 0) {//OverFlow
        OM.EqpOptn.iTotalChip     = 0 ;
        OM.EqpOptn.iTotalFailChip = 0 ;
    }

    LT.AddDayInfoChips(Array->GetCntStat(csRslt0 ) ,  //
                       Array->GetCntStat(csRslt1 ) ,  //
                       Array->GetCntStat(csRslt2 ) ); //


    LT.AddLotInfoChips(Array->GetCntStat(csRslt0 ) ,  //
                       Array->GetCntStat(csRslt1 ) ,  //
                       Array->GetCntStat(csRslt2 ) ); //

/*
    if(GetLastStrip()) {
//        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",Array->GetLotNo().c_str());
        LT.LotInfo.dEndTime = Now();
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        
        Trace("WriteLotLog",Array->GetLotNo().c_str());
        //LT.LotEnd();
        //널써야 겟군...dd
    }
*/
//    Array->SetStat(csNone) ;
}

bool CPostBuff::GetLastStrip()
{
    AnsiString sLastLotNo ;

    bool bLotEnd = EPX.GetEpxLotCnt() == OM.CmnOptn.iLotEndChipCnt && DM.ARAY[riRAD].CheckAllStat(csNone) ; //헤드구간 보는게 조오금 ...모랄까? 흠...

    return bLotEnd ;

/*
    if(DM.ARAY[riRAP].GetCntExist()) sLastLotNo = DM.ARAY[riRAP].GetLotNo() ;
    else                             sLastLotNo = m_sWorkedLotNo ;

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWREExist = !DM.ARAY[riRAE].CheckAllStat(csNone) ;
    bool bWRDExist = !DM.ARAY[riRAD].CheckAllStat(csNone) ;
//    bool bPSBExist = !DM.ARAY[riPSB].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
    bool bWREDiff  = sLastLotNo != DM.ARAY[riRAE].GetLotNo() ;
    bool bWRDDiff  = sLastLotNo != DM.ARAY[riRAD].GetLotNo() ;
//    bool bPSBDiff  = sLastLotNo != DM.ARAY[riPSB].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

//         if( bPSBExist) return bPSBDiff ;
         if( bWRDExist) return bWRDDiff ;
    else if( bWREExist) return bWREDiff ;
    else if( bPRBExist) return bPRBDiff ;
    else if( bLDRExist) return bLDRDiff ;
    else if(!bLastMgz ) return false    ;
    else                return true     ;
*/
}

double CPostBuff::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CPostBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPostBuff::CheckStop()
{
    if(!MT_GetStop(miPSB_XIdx)) return false ;

    if(!AT_Done(aiPSB_Clamp )) return false ;

    return true ;
}

void CPostBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos  " , m_dLastIdxPos  );
        UserINI.Load(sPath, "Member" , "m_sWorkedId    " , m_sWorkedId    );
        UserINI.Load(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos  " , m_dLastIdxPos  );
        UserINI.Save(sPath, "Member" , "m_sWorkedId    " , m_sWorkedId    );
        UserINI.Save(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );
    }
}
//---------------------------------------------------------------------------
