//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RailEpx.h"

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
CRailEpx RAE;

CRailEpx::CRailEpx()
{
    Init();

    AT_MoveCyl(aiWRE_BAlign , ccBwd );
    AT_MoveCyl(aiWRE_CAlign , ccBwd );
    AT_MoveCyl(aiWRE_Clamp  , ccBwd );

}
CRailEpx::~CRailEpx (void)
{
    Close();
}
void CRailEpx::Init()
{
    m_sPartName   = "RailEpx " ;
    m_dLastIdxPos = 0.0 ;
    m_iLotCnt     = 0.0 ;
    bRaeSkip      = false ;

    m_iHomeTO  = (int)eiWRE_HomeTO    ;
    m_iStartTO = (int)eiWRE_ToStartTO ;
    m_iStopTO  = (int)eiWRE_ToStopTO  ;
    m_iCycleTO = (int)eiWRE_CycleTO   ;

//    double dTemp = PM.GetValue (miWRE_XIdx , pvWRE_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1)) ;
//    PM.SetPos(miWRE_XIdx,pvWRE_XIdxPull,dTemp); //고정.

    Reset();
    Load(true);
}
void CRailEpx::Close()
{
    Load(false);
}
void CRailEpx::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRailEpx::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

bool CRailEpx::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    int r1,c1;
    if(_riWhre == riRAE) {
        c = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
        return (c > -1 && c > -1);
    }
    return false ;
}

bool CRailEpx::CheckColAllFail()
{
    bool bRet = false ;
    int r,c;
    FindChip(riRAE,r,c);
    if(DM.ARAY[riWRE].GetCntColStat(c,csRslt0) == OM.DevInfo.iRowCnt) bRet = true ;
    else bRet = false ;

    return bRet ;
}

double CRailEpx::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr);
    double dReGrip  = 0.0;
    int    r  , c  ;

    if(_iMotr == miWRE_XIdx){
        if(FindChip(riRAE,r,c)){
            c = OM.DevInfo.iColCnt - c - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvWRE_XIdxWorkStt ) + ( OM.DevInfo.dColPitch * c ) ;
            if(DM.ARAY[riRAE].GetStep() >1 ) {
                for(int i =0 ; i < DM.ARAY[riRAE].GetStep() - 1 ; i++) dReGrip += OM.DevOptn.iWRERgDis[i] * OM.DevInfo.dColPitch;
                dWorkPos -= dReGrip ;
            }
        }
    }
    return dWorkPos;
}

double CRailEpx::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miWRE_XIdx){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRE_XIdxWait    : dPos = PM.GetValue (_iMotr , pvWRE_XIdxWait    ); break ;
            case piWRE_XIdxWorkStt : dPos = PM.GetValue (_iMotr , pvWRE_XIdxWorkStt ); break ;
//            case piWRE_XIdxPull    : dPos = PM.GetValue (_iMotr , pvWRE_XIdxPull    ); break ;
//            case piWRE_XIdxBack    : dPos = PM.GetValue (_iMotr , pvWRE_XIdxBack    ); break ;
            case piWRE_XIdxOut     : dPos = PM.GetValue (_iMotr , pvWRE_XIdxOut     ); break ;
            case piWRE_XIdxWork    : dPos = GetWorkPos  (_iMotr                     ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRailEpx::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

//    bool isEpxZWaitPs = MT_GetStopInpos(miWRE_ZEpx) && MT_CmprPos(miWRE_ZEpx,PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWait)) ;
    bool isEpxZWaitPs = MT_GetStopInpos(miWRE_ZEpx) && MT_GetCmdPos(miWRE_ZEpx) <= PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWait) ; //MT_CmprPos(miWRE_ZEpx,PM.GetValue(miWRE_ZEpx,pvWRE_ZEpxWait)) ;

    bool bCAlignFw    = !AT_Complete(aiWRE_CAlign ,ccBwd);

    bool bWrdClampFw  = !AT_Complete(aiWRD_Clamp  ,ccBwd);
    bool bWrdCAlignFw = !AT_Complete(aiWRD_CAlign ,ccBwd);

    if(_iMotr == miWRE_XIdx){
        if(!isEpxZWaitPs) {
            sMsg = "Epoxy_Z Axis is Not Wait Position" ;
            bRet = false ;
        }
        switch(_iPstnId) {
            if(bCAlignFw    ) { sMsg = "RAIL EPOXY Chip Align is Fwd" ; bRet = false ;}
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRE_XIdxWait    : if(PM.GetValue(_iMotr,pvPRB_XIdxWait) - MT_GetCmdPos(miWRD_XIdx) >= OM.MstOptn.d2stDis ) {
                                         sMsg = "Not Able to Move Because of a Motr Crash(EPX_X Wait Position)" ;
                                         bRet = false ;
                                     }
                                     break ;
            case piWRE_XIdxWorkStt : if(PM.GetValue(_iMotr,pvPRB_XIdxWorkStt) - MT_GetCmdPos(miWRD_XIdx) >= OM.MstOptn.d2stDis ) {
                                         sMsg = "Not Able to Move Because of a Motr Crash(EPX_X Work Stt Position)" ;
                                         bRet = false ;
                                     }
                                     break ;
//            case piWRE_XIdxPull    : if(!isEpxZWaitPs) { sMsg = "Epoxy_Z Axis is Not Wait Position" ; bRet = false ;} break ;
//            case piWRE_XIdxBack    : if(!isEpxZWaitPs) { sMsg = "Epoxy_Z Axis is Not Wait Position" ; bRet = false ;} break ;
            case piWRE_XIdxOut     : if(bWrdClampFw || bWrdCAlignFw ) {
                                         sMsg = "Work Die Attach Cylinder is Fwd" ;
                                         bRet = false ;
                                     }
                                     if(PM.GetValue(_iMotr,pvPRB_XIdxOut) - MT_GetCmdPos(miWRD_XIdx) >= OM.MstOptn.d2stDis ) {
                                         sMsg = "Not Able to Move Because of a Motr Crash(EPX_X Out Position)" ;
                                         bRet = false ;
                                     }
                                     break ;
            case piWRE_XIdxWork    : if(GetWorkPos(_iMotr) - MT_GetCmdPos(miWRD_XIdx) >= OM.MstOptn.d2stDis ) {
                                         sMsg = "Not Able to Move Because of a Motr Crash(EPX_X Work Position)" ;
                                         bRet = false ;
                                     }
                                     break ;
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
bool CRailEpx::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = !MT_GetStopInpos(miWRE_XIdx);

    if(_iActr == aiWRE_Clamp ){
        if(isXMoving) {sMsg = "Epoxy_X_Index is Moving" ; bRet = false ; }
    }

    else if(_iActr == aiWRE_BAlign || _iActr == aiWRE_CAlign){
        if(_bFwd && isXMoving){sMsg = "Epoxy_X_Index is Moving" ; bRet = false ;}
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

bool CRailEpx::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRailEpx::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRailEpx::CycleHome()     //sun DLL direct access.
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
    static bool bWreNone = true ;

    switch (Step.iHome) {

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miWRE_XIdx,false);
                 AT_MoveCyl(aiWRE_Clamp  , ccBwd);
                 AT_MoveCyl(aiWRE_BAlign , ccBwd);
                 AT_MoveCyl(aiWRE_CAlign , ccBwd);
                 bWreNone = DM.ARAY[riRAE].CheckAllStat(csNone) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRE_Clamp  , ccBwd))return false ;
                 if(!AT_MoveCyl(aiWRE_BAlign , ccBwd))return false ;
                 if(!AT_MoveCyl(aiWRE_CAlign , ccBwd))return false ;
                 MT_DoHome(miWRE_XIdx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRE_XIdx))return false ;
                 if(bWreNone) {
                     MT_GoAbsMan(miWRE_XIdx , PM.GetValue(miWRE_XIdx , pvWRE_XIdxWait  ));
                 }
                 else {
                     MT_GoAbsMan(miWRE_XIdx , m_dLastIdxPos);
//                     return false ;
                 }
                 Step.iHome++;
                 return false ;

        case 13: if(bWreNone) {
                     if(!MT_GoAbsMan(miWRE_XIdx , PM.GetValue(miWRE_XIdx , pvWRE_XIdxWait  ))) return false ;
                 }
                 else {
                     if(!MT_GoAbsMan(miWRE_XIdx , m_dLastIdxPos                             )) return false ;
                 }

//                 if(!bWreNone) AT_MoveCyl(aiWRE_Clamp,ccFwd);
//
//                 Step.iHome++;
//                 return false ;
//
//        case 14: if(!bWreNone) if(!AT_MoveCyl(aiWRE_Clamp,ccFwd)) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CRailEpx::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CRailEpx::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRailEpx::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    int r,c ;
    static bool bNone = true ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: if(!MT_GetStopInpos(miWRE_XIdx)) return false ;

                 if(OM.CmnOptn.bUseBtAlign)MoveActr(aiWRE_BAlign, ccFwd);
                 else                      MoveActr(aiWRE_BAlign, ccBwd);
                 bNone = DM.ARAY[riRAE].CheckAllStat(csNone);
                 if( !bNone) {
                     Step.iToStart = 15 ;
                     return false ;
                 }
                 bRaeSkip = false ;
                 //None 11->
                 //Not None 15->

                 //MoveActr(aiWRE_Clamp  ,ccBwd);
                 MoveActr(aiWRE_CAlign ,ccBwd);
                 //MoveActr(aiWRE_BAlign ,ccBwd);

                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MoveActr(aiWRE_Clamp  ,ccBwd)) return false ;
                 //if(!MoveActr(aiWRE_BAlign ,ccBwd)) return false ;
                 if(!MoveActr(aiWRE_CAlign ,ccBwd)) return false ;
                 MoveActr(aiWRE_Clamp  ,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveActr(aiWRE_Clamp  ,ccBwd)) return false ;
                 MoveMotr(miWRE_XIdx,piWRE_XIdxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 13: //if(!bWreNone) if(!MoveActr(aiWRE_BAlign  ,ccFwd)) return false ;
                 if(!MoveMotr(miWRE_XIdx,piWRE_XIdxWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;


        //Not None ,OM.CmnOptn.bUseBtAlign
        case 15: if(!MoveActr(aiWRE_Clamp  ,ccFwd)) return false;

                 //if(OM.CmnOptn.bUseBtAlign) MoveActr(aiWRE_BAlign ,ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 16: //if(OM.CmnOptn.bUseBtAlign) if(!MoveActr(aiWRE_BAlign ,ccFwd)) return false ;
                 MoveActr(aiWRE_CAlign ,ccFwd);
                 Step.iToStart ++ ;
                 return true ;

        case 17: if(!MoveActr(aiWRE_CAlign ,ccFwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;

    }
}

bool CRailEpx::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: bRaeSkip = false ;
                 if(!MT_GetStopInpos(miWRE_XIdx)) return false ;

                 m_dLastIdxPos = GetLastCmd(miWRE_XIdx);

                 MoveActr(aiWRE_CAlign,ccBwd);
                 MoveActr(aiWRE_Clamp ,ccBwd);

                 Step.iToStop ++;
                 return false ;

        case 11:
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CRailEpx::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;
        //Check
        int  iCNow     = DM.ARAY[riRAE].FindLastCol(csUnkwn) ;
        int  iCNxt     = DM.ARAY[riRAD].FindLastCol(csUnkwn) ;
        int  iCRvs     = OM.DevInfo.iColCnt - iCNow - 1 ; // -1 Not Use. ? 0 Not Use ? Select...
        int  iStep     = DM.ARAY[riRAE].GetStep() ;
        bool bAllNone  = DM.ARAY[riRAE].CheckAllStat(csNone ) ;
//        bool bCNxtUnkn = DM.ARAY[riRAD].GetCntStat(csUnkwn) ;

        bool isWorkCon = ((iCNow - iCNxt - OM.DevOptn.iWRE_XWkCon) >= 0) || DM.ARAY[riRAD].CheckAllStat(csNone) ;
        bool bReGrip   = iStep > 0 &&  iCRvs >= OM.DevOptn.iWRERgCol[iStep-1] && iStep < OM.DevOptn.iWRE_XRgCnt + 1 ;

        bool bCheckIdxPos   =  MT_CmprPos(miWRE_XIdx,GetWorkPos(miWRE_XIdx)) ;

        //Judgment
        bool isCycleWorkStt = !bAllNone &&  iStep == 0  && isWorkCon ;
        bool isCycleReGrip  = !bAllNone &&  iStep >  0  && bReGrip ;//&& isWorkCon ;
        bool isCycleWork    = !bAllNone &&  iStep >  0  && isWorkCon && DM.ARAY[riRAE].GetCntStat(csUnkwn) && (!bCheckIdxPos || bRaeSkip ); // !MT_CmprPos(miWRE_XIdx,GetWorkPos(miWRE_XIdx)) ;
        bool isCycleWorkEnd = !bAllNone &&                !DM.ARAY[riRAE].GetCntStat(csUnkwn) && DM.ARAY[riRAD].CheckAllStat(csNone) ;

        bool isConEnd       =  bAllNone || (OM.CmnOptn.iLotEndChipCnt && EPX.GetEpxLotCnt() >= OM.CmnOptn.iLotEndChipCnt && !DM.ARAY[riWRE].GetCntStat(csWork));

//        if( DM.ARAY[riWRE].CheckAllStat(csNone) &&  )  EM_SetErr(eiWRE_Unknwn) ;
//        if(!DM.ARAY[riWRE].CheckAllStat(csNone) &&  )  EM_SetErr(eiWRE_Disp  ) ;

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
bool CRailEpx::CycleWorkStt(void)
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
    int      r ,c  ;
    int      iMgzCnt  ;
    int      iSlotCnt ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRE_Clamp , ccBwd);
                  //MoveActr(aiWRE_BAlign, ccBwd);
                  MoveActr(aiWRE_CAlign, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRE_Clamp , ccBwd)) return false ;
                  //if(!MoveActr(aiWRE_BAlign, ccBwd)) return false ;
                  if(!MoveActr(aiWRE_CAlign, ccBwd)) return false ;
                  MoveMotr(miWRE_XIdx , piWRE_XIdxWait );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRE_XIdx , piWRE_XIdxWait )) return false ;
                  sTemp = "Cmd:"+(String)MT_GetCmdPos(miWRE_XIdx) + "  Enc:" + (String)MT_GetEncPos(miWRE_XIdx) ;
                  Trace("RailEpx qt",sTemp.c_str());
                  MoveActr(aiWRE_Clamp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiWRE_Clamp, ccFwd)) return false ;
                  MoveMotr(miWRE_XIdx , piWRE_XIdxWorkStt  );
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRE_XIdx , piWRE_XIdxWorkStt  )) return false ;
                  //if(OM.CmnOptn.bUseBtAlign)MoveActr(aiWRE_BAlign, ccFwd);
                  MoveActr(aiWRE_CAlign, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: //if(OM.CmnOptn.bUseBtAlign){if(!MoveActr(aiWRE_BAlign, ccFwd)) return false ;}
                  if(!MoveActr(aiWRE_CAlign, ccFwd)) return false ;
                  DM.ARAY[riRAE].SetStep(1);

                  FindChip(riRAE,r,c);
                  if(OM.MstOptn.bIgnrEpoxy) {
                      for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                          DM.ARAY[riRAE].SetStat(i,c,csWork);
                      }
                  }
                  for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                      if(DM.ARAY[riWRE].GetStat(i,c) == csRslt0) DM.ARAY[riRAE].SetStat(i,c,csWork);
                  } //이거 에폭시랑 타이밍이 안맞는거 아니냐?

                  m_iLotCnt++ ;
                  if(m_iLotCnt == 1 ){
                      sTemp = Now().FormatString("yyyy.mm.dd(hh.nn.ss)") ;
                      DM.ARAY[riWRE].SetLotNo(sTemp);
                      DM.ARAY[riRAE].SetLotNo(sTemp);
                      LT.LotOpen(sTemp,LT.LotInfo.sJobFile,"");
                      DM.ARAY[riRAE].SetID("1001") ;
                      DM.ARAY[riWRE].SetID("1001") ;

                  }
                  if(m_iLotCnt > 1) {
                      iMgzCnt   = ((m_iLotCnt-1) / (OM.DevInfo.iMgzSlotCnt ))+ 1;
                      iSlotCnt  = ((m_iLotCnt-1) % (OM.DevInfo.iMgzSlotCnt )) +1;
                      sTemp = iMgzCnt * 1000 + iSlotCnt ;
                      DM.ARAY[riRAE].SetID(sTemp) ;
                      DM.ARAY[riWRE].SetID(sTemp) ;

                      DM.ARAY[riWRE].SetLotNo(LT.GetCrntLot());
                      DM.ARAY[riRAE].SetLotNo(LT.GetCrntLot());

                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRailEpx::CycleReGrip(void)
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
                  dReGrip  = OM.DevOptn.iWRERgDis[DM.ARAY[riRAE].GetStep()-1] * OM.DevInfo.dColPitch;
                  dTemp    = MT_GetCmdPos(miWRE_XIdx) - dReGrip;
                  if( dTemp <= MT_GetMinPos(miWRE_XIdx) ) {
                      EM_SetErr(eiWRE_RegripPosMinOver);
                      Step.iCycle = 0 ;
                      return true;
                  }

                  MoveActr(aiWRE_Clamp  , ccBwd);
//                  MoveActr(aiWRE_BAlign , ccBwd);
                  MoveActr(aiWRE_CAlign , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRE_Clamp  , ccBwd)) return false ;

                  MT_GoIncRun(miWRE_XIdx,-dReGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MT_GetStopInpos(miWRE_XIdx)) return false ; //abs 로 바꿀까 말까 고민중...
//                  if(!MoveActr(aiWRE_BAlign , ccBwd)) return false ;
                  if(!MoveActr(aiWRE_CAlign , ccBwd)) return false ;

                  MoveActr(aiWRE_Clamp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiWRE_Clamp, ccFwd)) return false ;
                  DM.ARAY[riRAE].SetStep(DM.ARAY[riRAE].GetStep()+1);
//                  if(DM.ARAY[riWRE].GetCntStat(csUnkwn)) {
//                      MoveMotr(miPRB_XIdx , piPRB_XIdxWork );
//                      MoveMotr(miPRB_YIns , piPRB_YInsWork );
//                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}
bool CRailEpx::CycleWork(void)
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
    static bool bVsGood = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10:
                  MoveActr(aiWRE_Clamp    , ccFwd);
                  //MoveActr(aiWRE_BAlign   , ccBwd);
                  MoveActr(aiWRE_CAlign   , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRE_Clamp   , ccFwd)) return false ;
                  //if(!MoveActr(aiWRE_BAlign  , ccBwd)) return false ;
                  if(!MoveActr(aiWRE_CAlign  , ccBwd)) return false ;

                  if(GetMotrPos(miWRE_XIdx,piWRE_XIdxWork) >= MT_GetMaxPos(miWRE_XIdx) ){
                      EM_SetErr(eiWRE_WorkPosMaxOver);
                      Step.iCycle = 0;
                      return true;
                  }

                  MoveMotr(miWRE_XIdx , piWRE_XIdxWork );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRE_XIdx , piWRE_XIdxWork )) return false ;
//TODO::                  DM.ARAY[ri

                  //if(OM.CmnOptn.bUseBtAlign)MoveActr(aiWRE_BAlign   , ccFwd);
                  MoveActr(aiWRE_CAlign   , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: //if(OM.CmnOptn.bUseBtAlign){if(!MoveActr(aiWRE_BAlign   , ccFwd)) return false ; }// 이시간을 빼면 얼라인 치는 시간이 그럭저럭.}
                  if(!MoveActr(aiWRE_CAlign   , ccFwd)) return false ;
                  FindChip(riRAE,r,c);

                  if(bRaeSkip){
                      for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                          if(DM.ARAY[riWRE].GetStat(i,c) == csRslt0) DM.ARAY[riRAE].SetStat(i,c,csWork);
                      }
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(OM.MstOptn.bIgnrEpoxy) {
                      for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                          DM.ARAY[riRAE].SetStat(i,c,csWork);
                      }
                  }
                  for (int i= 0; i<OM.DevInfo.iRowCnt; i++){
                      if(DM.ARAY[riWRE].GetStat(i,c) == csRslt0) DM.ARAY[riRAE].SetStat(i,c,csWork);
                  } //이거 에폭시랑 타이밍이 안맞는거 아니냐?

                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRailEpx::CycleWorkEnd(void)
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

        case  10: bRaeSkip = false ;
                  //MoveActr(aiWRE_BAlign   , ccBwd);
                  MoveActr(aiWRE_CAlign   , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: //if(!MoveActr(aiWRE_BAlign   , ccBwd)) return false ;
                  if(!MoveActr(aiWRE_CAlign   , ccBwd)) return false ;

                  MoveActr(aiWRE_Clamp  , ccFwd); //

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiWRE_Clamp  , ccFwd)) return false ;
                  MoveMotr(miWRE_XIdx,piWRE_XIdxOut);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRE_XIdx,piWRE_XIdxOut)) return false ;
                  //Data Masking
                  DM.ShiftArrayData(riWRE,riWRD);
                  DM.ARAY[riWRD].ChangeStat(csWork,csUnkwn);
                  DM.ARAY[riRAE].SetStat(csNone );
//                  DM.ARAY[riRAE].SetID("");

                  DM.ARAY[riRAD].SetLotNo(DM.ARAY[riWRD].GetLotNo());
                  DM.ARAY[riRAD].SetID   (DM.ARAY[riWRD].GetID   ());
                  DM.ARAY[riRAD].SetStat (csUnkwn);

                  MoveActr(aiWRE_Clamp  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiWRE_Clamp  , ccBwd)) return false ;
                  MoveMotr(miWRE_XIdx,piWRE_XIdxWait);



                  Step.iCycle = 0 ;
                  return true ;

/*
                  MoveMotr(miWRE_XIdx,piWRE_XIdxPull);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRE_XIdx,piWRE_XIdxPull)) return false ;
                  MoveActr(aiWRE_Clamp  , ccBwd); //Pull 포지션을 셋업창에 하나 두어주자.
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiWRE_Clamp  , ccBwd)) return false ;
                  MoveMotr(miWRE_XIdx,piWRE_XIdxBack);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRE_XIdx,piWRE_XIdxBack)) return false ;
                  MoveActr(aiPRB_Clamp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPRB_Clamp, ccFwd)) return false ;
                  MoveMotr(miWRE_XIdx,piWRE_XIdxOut);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRE_XIdx,piWRE_XIdxOut)) return false ;
                  DM.ShiftArrayData(riWRE,riWRD);
                  DM.ARAY[riWRD].ChangeStat(csWork,csUnkwn);


                  MoveActr(aiWRE_Clamp  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiWRE_Clamp  , ccBwd)) return false ;
                  MoveMotr(miWRE_XIdx,piWRE_XIdxWait);



                  Step.iCycle = 0 ;
                  return true ;
*/
    }
}
double CRailEpx::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double _dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) _dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            _dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return _dLastIdxPos ;
}

bool CRailEpx::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRailEpx::CheckStop()
{
    if(!MT_GetStop(miWRE_XIdx)) return false ;

    if(!AT_Done(aiWRE_BAlign )) return false ;
    if(!AT_Done(aiWRE_CAlign )) return false ;
    if(!AT_Done(aiWRE_Clamp  )) return false ;

    return true ;
}

void CRailEpx::Load(bool IsLoad)
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


