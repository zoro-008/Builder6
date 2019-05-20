//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PreBuff.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "Epoxy.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPreBuff PRB;

CPreBuff::CPreBuff()
{
    Init();

    AT_MoveCyl(aiPRB_Clamp   , ccBwd );
    AT_MoveCyl(aiPRB_Stopper , ccBwd );
    AT_MoveCyl(aiPRB_CAlign  , ccBwd );

}
CPreBuff::~CPreBuff (void)
{
    Close();
}
void CPreBuff::Init()
{
    m_sPartName   = "PreBuff " ;
    m_dLastIdxPos = 0.0 ;
    m_bOriFailIgnr  = false ;

    m_bBoatSkip   = false ;

    m_bOriFailReInsp= false ;

    m_iHomeTO  = (int)eiPRB_HomeTO    ;
    m_iStartTO = (int)eiPRB_ToStartTO ;
    m_iStopTO  = (int)eiPRB_ToStopTO  ;
    m_iCycleTO = (int)eiPRB_CycleTO   ;

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

bool CPreBuff::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    int r1,c1;
    //Stt Position Down
    if(_riWhre == riPRB) {
        c = DM.ARAY[riPRB].FindLastCol(csUnkwn);
        if(c % 2) DM.ARAY[riPRB].FindLastColFrstRow(csUnkwn , r1 , c1) ;
        else      DM.ARAY[riPRB].FindLastColRow    (csUnkwn , r1 , c1) ;
        r = r1 ;
        return (r > -1 && c > -1);
    }
    return false ;
}

double CPreBuff::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr);
    double dReGrip  = 0.0;
    int    r  , c  ;

    if(_iMotr == miPRB_XIdx){
        if(FindChip(riPRB,r,c)) {
            c = OM.DevInfo.iColCnt - c - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvPRB_XIdxWorkStt ) + ( OM.DevInfo.dColPitch * c ) ;
            if(DM.ARAY[riPRB].GetStep() >1 ) {
                for(int i =0 ; i < DM.ARAY[riPRB].GetStep() - 1 ; i++) dReGrip += OM.DevOptn.iPRBRgDis[i] * OM.DevInfo.dColPitch;
                dWorkPos -= dReGrip ;
            }
        }
    }
    else if(_iMotr == miPRB_YIns){
        if(FindChip(riPRB,r,c)) {
            r = OM.DevInfo.iRowCnt - r - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvPRB_YInsWorkStart ) + ( OM.DevInfo.dRowPitch * r ) ;
        }
    }

    return dWorkPos;
}
double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miPRB_XIdx){
        switch(_iPstnId) {
            default                     : dPos = MT_GetCmdPos(_iMotr                          ); break ;
            case piPRB_XIdxWait         : dPos = PM.GetValue (_iMotr , pvPRB_XIdxWait         ); break ;
            case piPRB_XIdxWorkStt      : dPos = PM.GetValue (_iMotr , pvPRB_XIdxWorkStt      ); break ;
//            case piPRB_XIdxReGripCntOfs : dPos = PM.GetValue (_iMotr , pvPRB_XIdxReGripCntOfs ); break ; //아좀...
//            case piPRB_XIdxReGripDisOfs : dPos = PM.GetValue (_iMotr , pvPRB_XIdxReGripDisOfs ); break ;
            case piPRB_XIdxOut          : dPos = PM.GetValue (_iMotr , pvPRB_XIdxOut          ); break ;
            case piPRB_XIdxWork         : dPos = GetWorkPos  (_iMotr                          ); break ;
        }
    }
    else if(_iMotr == miPRB_YIns){
        switch(_iPstnId) {
            default                     : dPos = MT_GetCmdPos(_iMotr                          ); break ;
            case piPRB_YInsWait         : dPos = PM.GetValue (_iMotr , pvPRB_YInsWait         ); break ;
            case piPRB_YInsWorkStart    : dPos = PM.GetValue (_iMotr , pvPRB_YInsWorkStart    ); break ;
            case piPRB_YInsWork         : dPos = GetWorkPos  (_iMotr                          ); break ;
        }            //YIsp
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bStopperFw   = !AT_Complete(aiPRB_Stopper,ccBwd);
    bool bCAlignFw    = !AT_Complete(aiPRB_CAlign ,ccBwd);

    bool bWreClampFw  = !AT_Complete(aiWRE_Clamp  ,ccBwd);
    bool bWreCAlignFw = !AT_Complete(aiWRE_CAlign ,ccBwd);
    bool bWreBAlignFw = !AT_Complete(aiWRE_BAlign ,ccBwd);

    if(_iMotr == miPRB_XIdx){
        if(bCAlignFw    ) { sMsg = "PreBuffer Chip Align is Fwd" ; bRet = false ;}
        switch(_iPstnId) {
            default                     : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piPRB_XIdxWait         : if(PM.GetValue(_iMotr,pvPRB_XIdxWait) - MT_GetCmdPos(miWRE_XEpx) >= OM.MstOptn.d1stDis ) {
                                              sMsg = "Not Able to Move Because of a Motr Crash(PRB_X Wait Position)" ;
                                              bRet = false ;
                                          }
                                          break ;
            case piPRB_XIdxWorkStt      : if(bStopperFw) {
                                              sMsg = "PreBuffer Stopper is Fwd" ;
                                              bRet = false ;
                                          }
                                          if(PM.GetValue(_iMotr,pvPRB_XIdxWorkStt) - MT_GetCmdPos(miWRE_XEpx) >= OM.MstOptn.d1stDis ) {
                                              sMsg = "Not Able to Move Because of a Motr Crash(PRB_X Work Stt Position)" ;
                                              bRet = false ;
                                          }
                                          break ;
            case piPRB_XIdxOut          : if(PM.GetValue(_iMotr,pvPRB_XIdxOut) - MT_GetCmdPos(miWRE_XEpx) >= OM.MstOptn.d1stDis ) {
                                              sMsg = "Not Able to Move Because of a Motr Crash(PRB_X Out Position)" ;
                                              bRet = false ;
                                          }
                                          if( bWreClampFw || bWreCAlignFw ){//|| bWreBAlignFw ){
                                              sMsg = "Work Epoxy Cylinder is Fwd" ;
                                              bRet = false ;
                                          }

                                          break ;
            case piPRB_XIdxWork         : if(GetWorkPos(_iMotr) - MT_GetCmdPos(miWRE_XEpx) >= OM.MstOptn.d1stDis ) {
                                              sMsg = "Not Able to Move Because of a Motr Crash(PRB_X Work Position)" ;
                                              bRet = false ;
                                          }
                                          break ;

        }
    }
    else if(_iMotr == miPRB_YIns){
        switch(_iPstnId) {
            default                  : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piPRB_YInsWait      : break ;
            case piPRB_YInsWorkStart : break ;
            case piPRB_YInsWork      : break ;
        }
    }
    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart && !Step.iToStop)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = !MT_GetStopInpos(miPRB_XIdx);

    if(_iActr == aiPRB_Stopper){
        if(_bFwd && IO_GetX(xPRB_InDetect)) {sMsg = "PRB_BOAT is Detected" ; bRet = false ;}
    }
    else if(_iActr == aiPRB_Clamp){
        //CheckSafe 걸리는데 bwd 할때 걸림 인포지션이 안들어온다네...
        if(isXMoving && _bFwd) {sMsg = "PRB_X_Index is Moving" ; bRet = false ;}
    }
    else if(_iActr == aiPRB_CAlign){
        if(_bFwd && isXMoving) {sMsg = "PRB_X_Index is Moving" ; bRet = false ;}
    }

    else {
        sMsg = AT_GetName(_iActr) + " is Not this part Cylinder!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart && !Step.iToStop )FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPreBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff::CycleHome()     //sun DLL direct access.
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
    static bool bPrbNone = true ;

    switch (Step.iHome) {

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miPRB_XIdx,false);
                 MT_SetHomeEnd(miPRB_YIns,false);
                 AT_MoveCyl(aiPRB_Clamp  , ccBwd);
                 AT_MoveCyl(aiPRB_Stopper, ccBwd);
                 AT_MoveCyl(aiPRB_CAlign , ccBwd);
                 bPrbNone = DM.ARAY[riPRB].CheckAllStat(csNone) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPRB_Clamp  , ccBwd))return false ;
                 if(!AT_MoveCyl(aiPRB_Stopper, ccBwd))return false ;
                 if(!AT_MoveCyl(aiPRB_CAlign , ccBwd))return false ;

                 MT_DoHome(miPRB_XIdx);
                 MT_DoHome(miPRB_YIns);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPRB_XIdx))return false ;
                 if(!MT_GetHomeEnd(miPRB_YIns))return false ;
                 if(bPrbNone) { //TODO :: 이런거라면 홈잡을때도 충돌 조건을 확인해야함 (나중에 바꾸도록 하세요.!!!)
                     MT_GoAbsMan(miPRB_XIdx , PM.GetValue(miPRB_XIdx , pvPRB_XIdxWait  ));
                     MT_GoAbsMan(miPRB_YIns , PM.GetValue(miPRB_YIns , pvPRB_YInsWait  ));
                 }
                 else {
                     MT_GoAbsMan(miPRB_XIdx , m_dLastIdxPos);
                     MT_GoAbsMan(miPRB_YIns , PM.GetValue(miPRB_YIns , pvPRB_YInsWait  ));
//                     return false ;
                 }
                 Step.iHome++;
                 return false ;

        case 13: if(bPrbNone) {
                     if(!MT_GoAbsMan(miPRB_XIdx , PM.GetValue(miPRB_XIdx , pvPRB_XIdxWait  ))) return false ;
                     if(!MT_GoAbsMan(miPRB_YIns , PM.GetValue(miPRB_YIns , pvPRB_YInsWait  ))) return false ;
                 }
                 else {
                     if(!MT_GoAbsMan(miPRB_XIdx , m_dLastIdxPos                             )) return false ;
                     if(!MT_GoAbsMan(miPRB_YIns , PM.GetValue(miPRB_YIns , pvPRB_YInsWait  ))) return false ;
                 }

//                 if(!bPrbNone) AT_MoveCyl(aiPRB_Clamp,ccFwd);
//
//                 Step.iHome++;
//                 return false ;
//
//        case 14: if(!bPrbNone) if(!AT_MoveCyl(aiPRB_Clamp,ccFwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPreBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    static bool bPrbNone = true ;
    int r,c ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: //if(DM.ARAY[riPRB].CheckAllStat(csUnkwn)){
                 //    DM.ARAY[riPRB].SetStep(0);
                 //}

                 m_bOriFailReInsp = false ;
                 if(m_bOriFailIgnr && DM.ARAY[riPRB].GetCntStat(csRslt0)) {
                     c = DM.ARAY[riPRB].FindFrstCol(csRslt0);
                     if(c % 2) DM.ARAY[riPRB].FindFrstColLastRow(csRslt0 , r , c) ;
                     else      DM.ARAY[riPRB].FindFrstColRow(csRslt0 , r , c) ;

                     if(r > -1 && c > -1) DM.ARAY[riPRB].SetStat(r,c,csUnkwn);
                     m_bOriFailReInsp = true ;
                 }

                 m_bOriFailIgnr = false ;
/*
                 if( DM.ARAY[riPRB].CheckAllStat(csNone) &&  IO_GetX(xPRB_InDetect) )  {
                     EM_SetErr(eiPRB_Unknwn) ;
                     Step.iToStart = 0 ;
                     return true ;
                 }
*/ //지워도 되는게 맞는지 잘 모르겟당...
                 if(DM.ARAY[riPRB].CheckAllStat(csNone)){
                     m_bOriSkip = false ;
                     Step.iToStart = 20 ;
                     return false ;
                 }
                 Step.iToStart ++ ;
                 return false ;


        case 11:
                 Step.iToStart = 0 ;
                 return true ;


        case 20: MoveActr(aiPRB_Clamp  ,ccBwd);
                 MoveActr(aiPRB_Stopper,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 21: if(!MoveActr(aiPRB_Clamp  ,ccBwd)) return false ;
                 if(!MoveActr(aiPRB_Stopper,ccBwd)) return false ;
                 Step.iToStart ++ ;
                 return false ;

        case 22: MoveMotr(miPRB_XIdx  , piPRB_XIdxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 23: if(!MoveMotr(miPRB_XIdx  , piPRB_XIdxWait)) return false ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreBuff::ToStop(void) //스탑을 하기 위한 함수.
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

    static bool bNone = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: m_bOriSkip = false ;
//                 bNone = DM.ARAY[riPRB].CheckAllStat(csNone);
                 if(!MT_GetStopInpos(miPRB_XIdx)) return false ;
                 m_dLastIdxPos = GetLastCmd(miPRB_XIdx) ;
                 MoveActr(aiPRB_CAlign,ccBwd);
                 MoveActr(aiPRB_Clamp ,ccBwd);
//                 if( bNone)MoveActr(aiPRB_Clamp  ,ccBwd);

                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveActr(aiPRB_CAlign,ccBwd)) return false ;
                 if(!MoveActr(aiPRB_Clamp ,ccBwd)) return false ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CPreBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Check
        int  iCNow     = DM.ARAY[riPRB].FindLastCol(csUnkwn) ;
        int  iCNxt     = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
        int  iCRvs     = OM.DevInfo.iColCnt - iCNow - 1 ; // -1 Not Use. ? 0 Not Use ? Select...

        int  iStep     = DM.ARAY[riPRB].GetStep() ;
        bool bAllNone  = DM.ARAY[riPRB].CheckAllStat(csNone ) ;
//        bool bCNxtUnkn = DM.ARAY[riRAE].GetCntStat(csUnkwn) ;

        bool isWorkCon = ((iCNow - iCNxt - OM.DevOptn.iPRB_XWkCon) >= 0) || DM.ARAY[riRAE].CheckAllStat(csNone) ;
        bool bReGrip   = iStep > 0 && iCRvs >= OM.DevOptn.iPRBRgCol[iStep-1] && iStep < OM.DevOptn.iPRB_XRgCnt + 1 ;

//        bool bCheckIdxPos   =  MT_CmprPos(miPRB_XIdx,GetWorkPos(miPRB_XIdx)) ;


        //Judgment
        bool isCycleWorkStt =  DM.ARAY[riPRB].CheckAllStat(csUnkwn) && iStep == 0 && isWorkCon && (OM.MstOptn.bDryRun || IO_GetX(xPRB_InDetect));
        bool isCycleReGrip  = !bAllNone &&  iStep > 0 && bReGrip ;//&& isWorkCon ;
        bool isCycleWork    = !bAllNone &&  iStep > 0 && DM.ARAY[riPRB].GetCntStat(csUnkwn) && isWorkCon ;
        bool isCycleWorkEnd = !bAllNone &&              !DM.ARAY[riPRB].GetCntStat(csUnkwn) && DM.ARAY[riRAE].CheckAllStat(csNone);

        bool isConEnd       =  bAllNone || (OM.CmnOptn.iLotEndChipCnt && EPX.GetEpxLotCnt() >= OM.CmnOptn.iLotEndChipCnt) ;

//        if( DM.ARAY[riPRB].CheckAllStat(csNone) &&  IO_GetX(xPRB_InDetect) )  EM_SetErr(eiPRB_Unknwn) ;
//        if(!DM.ARAY[riPRB].CheckAllStat(csNone) && !IO_GetX(xPRB_InDetect) )  EM_SetErr(eiPRB_Disp  ) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleWorkStt ) {Trace(m_sPartName.c_str(),"CycleWorkStt  Stt"); Step.iSeq = scWorkStt ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReGrip  ) {Trace(m_sPartName.c_str(),"CycleReGrip   Stt"); Step.iSeq = scReGrip  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork     Stt"); Step.iSeq = scWork    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkEnd ) {Trace(m_sPartName.c_str(),"CycleWorkEnd  Stt"); Step.iSeq = scWorkEnd ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd      ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                      Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                          return false ;
        case scWorkStt : if(CycleWorkStt ()){ Trace(m_sPartName.c_str(),"CycleWorkStt End");Step.iSeq = scIdle ;} return false ;
        case scReGrip  : if(CycleReGrip  ()){ Trace(m_sPartName.c_str(),"CycleReGrip  End");Step.iSeq = scIdle ;} return false ;
        case scWork    : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork    End");Step.iSeq = scIdle ;} return false ;
        case scWorkEnd : if(CycleWorkEnd ()){ Trace(m_sPartName.c_str(),"CycleWorkEnd End");Step.iSeq = scIdle ;} return false ;
    }

}

//One Cycle.
bool CPreBuff::CycleWorkStt(void)
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

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPRB_Clamp , ccBwd);
                  MoveActr(aiPRB_CAlign, ccBwd); // 12
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Clamp, ccBwd)) return false ;
                  MoveMotr(miPRB_XIdx , piPRB_XIdxWait );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XIdx , piPRB_XIdxWait )) return false ;
                  MoveActr(aiPRB_Clamp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_CAlign, ccBwd)) return false ;
                  if(!MoveActr(aiPRB_Clamp , ccFwd)) return false ;

                  MoveActr(aiPRB_Stopper, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiPRB_Stopper, ccBwd)) return false ;
                  MoveMotr(miPRB_XIdx , piPRB_XIdxWorkStt  );
                  MoveMotr(miPRB_YIns , piPRB_YInsWorkStart);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miPRB_XIdx , piPRB_XIdxWorkStt  )) return false ;
                  if(!MoveMotr(miPRB_YIns , piPRB_YInsWorkStart)) return false ;
                  DM.ARAY[riPRB].SetStep(1) ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleReGrip(void)
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
    static double dReGrip  = 0.0 ;
    double dTemp = 0.0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: dReGrip  = 0.0 ;
//                  for(int i =0 ; i < DM.ARAY[riPRB].GetStep() ; i++) dReGrip += OM.DevOptn.iPRBRgDis[i] * OM.DevInfo.dColPitch;
                  dReGrip = OM.DevOptn.iPRBRgDis[DM.ARAY[riPRB].GetStep()-1] * OM.DevInfo.dColPitch;
                  dTemp = MT_GetCmdPos(miPRB_XIdx) - dReGrip;
                  if( dTemp <= MT_GetMinPos(miPRB_XIdx) ) {
                      EM_SetErr(eiPRB_RegripPosMinOver);
                      Step.iCycle = 0 ;
                      return true;
                  }

                  MoveActr(aiPRB_Clamp  , ccBwd);
                  MoveActr(aiPRB_CAlign , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Clamp  , ccBwd)) return false ;
//                  if(!MoveActr(aiPRB_CAlign , ccBwd)) return false ;
                  MT_GoIncRun(miPRB_XIdx,-dReGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MT_GetStopInpos(miPRB_XIdx)) return false ; //abs 로 바꿀까 말까 고민중...
                  if(!MoveActr(aiPRB_CAlign , ccBwd)) return false ;
                  MoveActr(aiPRB_Clamp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_Clamp, ccFwd)) return false ;
                  DM.ARAY[riPRB].SetStep(DM.ARAY[riPRB].GetStep()+1);
//                  if(DM.ARAY[riPRB].GetCntStat(csUnkwn)) {
//                      MoveMotr(miPRB_XIdx , piPRB_XIdxWork );
//                      MoveMotr(miPRB_YIns , piPRB_YInsWork );
//                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleWork(void)
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
    int iCol ;
//    double dWorkPos = 0.0 ;
    bool bWorkPos = false ;

    static int iVisnFailCnt = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //if(!MoveMotr(miPRB_XIdx , piPRB_XIdxWork )) return false ;
                  //if(!MoveMotr(miPRB_YIns , piPRB_YInsWork )) return false ;
                  MoveMotr(miPRB_YIns , piPRB_YInsWork );
                  bWorkPos = MT_CmprPos(miPRB_XIdx,GetMotrPos(miPRB_XIdx,piPRB_XIdxWork));
                  if(bWorkPos) {
                      MoveActr(aiPRB_Clamp   , ccFwd);
                      Step.iCycle  = 13;
                      return false;
                  }
                  MoveActr(aiPRB_Clamp   , ccFwd);
                  MoveActr(aiPRB_CAlign  , ccBwd);
                  Step.iCycle ++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Clamp   , ccFwd)) return false ;
                  if(!MoveActr(aiPRB_CAlign  , ccBwd)) return false ;
                  if(GetMotrPos(miPRB_XIdx,piPRB_XIdxWork) >= MT_GetMaxPos(miPRB_XIdx) ){
                      EM_SetErr(eiPRB_WorkPosMaxOver);
                      Step.iCycle = 0;
                      return true;
                  }
                  MoveMotr(miPRB_XIdx , piPRB_XIdxWork );

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XIdx , piPRB_XIdxWork )) return false ;
                  MoveActr(aiPRB_CAlign,ccFwd);
                  Step.iCycle++;
                  return false;


        case  13: if(!MoveActr(aiPRB_Clamp   , ccFwd)) return false;
                  if(!MoveActr(aiPRB_CAlign  , ccFwd)) return false;
                  if(!MoveMotr(miPRB_YIns , piPRB_YInsWork )) return false ;

                  if(OM.CmnOptn.bIgnrOriVs){
                      FindChip(riPRB,r,c);
                      DM.ARAY[riPRB].SetStat(r,c,csWork);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true,OM.DevOptn.iOriVsBfDly)) return false ;
                  VSM.Grab(viOri);
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  15: if(m_tmTemp.OnDelay(!VSM.GetGrabEnd(viOri),OM.DevOptn.iOriVsGrabTO )) {
                      EM_SetErr(eiPRB_OriVsGrabFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!VSM.GetGrabEnd(viOri)) return false ;
                  if(!OM.MstOptn.bDryRun)VSM.Inspect(viOri,false);
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case  16: if(m_tmTemp.OnDelay(!VSM.GetInspEnd(viOri),OM.DevOptn.iOriVsInspTO )) {
                      EM_SetErr(eiPRB_OriVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!OM.MstOptn.bDryRun && !VSM.GetInspEnd(viOri)) return false ;
                  if(OM.MstOptn.bDryRun) {
                      Rslt.bRslt = true ;
                      Rslt.iRet  = 0    ;
                  }
                  else VSM.GetRslt(viOri,&Rslt);
                  //GetInspEnd == 0 //알고리즘 검사

                  //Rslt.bRslt == 1 //검사 성공
                  //Rslt.bRslt == 0 //검사 실패
                  //Rslt.iRet  == 0:Pass , 1:OCV Fail

                  if(Rslt.bRslt){
                      if(Rslt.iRet == 0) {
                          iVisnFailCnt = 0 ;
                          FindChip(riPRB,r,c);
                          DM.ARAY[riPRB].SetStat(r,c,csWork);
                      }
                      else {
                          FindChip(riPRB,r,c);
                          DM.ARAY[riPRB].SetStat(r,c,csRslt0);
                          iVisnFailCnt++;
                      }
                  }
                  else {
                      EM_SetErr(eiPRB_OriVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //------
                  bWorkPos = MT_CmprPos(miPRB_XIdx,GetMotrPos(miPRB_XIdx,piPRB_XIdxWork));
                  if(!bWorkPos) MoveActr(aiPRB_CAlign , ccBwd);
                  //------


                  if(OM.DevOptn.iOriVsFailCnt && iVisnFailCnt >= OM.DevOptn.iOriVsFailCnt && !m_bOriFailReInsp && !m_bOriSkip && !m_bBoatSkip) {
                      EM_SetErr(eiPRB_OriVsFailCnt);
                      iVisnFailCnt = 0 ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  m_bOriFailReInsp = false ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CPreBuff::CycleWorkEnd(void)
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

        case  10: m_bBoatSkip = false ;

                  MoveActr(aiPRB_Clamp  , ccFwd);
                  MoveActr(aiPRB_CAlign , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPRB_Clamp  , ccFwd)) return false ;
                  if(!MoveActr(aiPRB_CAlign , ccBwd)) return false ;
                  if(!MT_GetStopInpos(miPRB_XIdx)) return false ;
                  MoveMotr(miPRB_XIdx,piPRB_XIdxOut);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPRB_XIdx,piPRB_XIdxOut)) return false ;
/*
                  //Data Masking
                  DM.ShiftArrayData(riPRB,riWRE);
                  DM.ARAY[riWRE].ChangeStat(csWork,csUnkwn);

                  DM.ARAY[riRAE].SetLotNo(DM.ARAY[riWRE].GetLotNo());
                  DM.ARAY[riRAE].SetID   (DM.ARAY[riWRE].GetID   ());
                  DM.ARAY[riRAE].SetStat (csUnkwn);

//                  DM.ARAY[riPRB].SetStat(csNone);
*/
                  MoveActr(aiPRB_Clamp, ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPRB_Clamp, ccBwd)) return false ;
                  sTemp = "Cmd:"+(String)MT_GetCmdPos(miPRB_XIdx) + "  Enc:" + (String)MT_GetEncPos(miPRB_XIdx) ;
                  Trace("PreBuff qt",sTemp.c_str());

//                  m_bOriSkip = false ;
                  //Data Masking
                  DM.ShiftArrayData(riPRB,riWRE);
                  DM.ARAY[riWRE].ChangeStat(csWork,csUnkwn);

                  DM.ARAY[riRAE].SetLotNo(DM.ARAY[riWRE].GetLotNo());
                  DM.ARAY[riRAE].SetID   (DM.ARAY[riWRE].GetID   ());
                  DM.ARAY[riRAE].SetStat (csUnkwn);

                  if(m_bOriSkip) {
                      DM.ARAY[riWRE].SetStat(csRslt0);



                  }
//                  DM.ARAY[riPRB].SetStat(csNone);


                  MoveMotr(miPRB_XIdx,piPRB_XIdxWait);
                  Step.iCycle = 0 ;
                  return true ;
    }
}
double CPreBuff::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ; 
}

bool CPreBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPreBuff::CheckStop()
{
    if(!MT_GetStop(miPRB_XIdx)) return false ;
    if(!MT_GetStop(miPRB_YIns)) return false ;

    if(!AT_Done(aiPRB_Clamp   )) return false ;
    if(!AT_Done(aiPRB_Stopper )) return false ;
    if(!AT_Done(aiPRB_CAlign  )) return false ;

    return true ;
}

void CPreBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos ); 
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos " , m_dLastIdxPos );
    }
}
//---------------------------------------------------------------------------
