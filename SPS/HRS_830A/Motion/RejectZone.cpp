//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "RejectZone.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CRejectZone REJ;

CRejectZone::CRejectZone(void)
{

}

CRejectZone::~CRejectZone (void)
{

}

void CRejectZone::Init()
{
    m_sPartName = "RejectZone " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CRejectZone::Close()
{
    Load(false);
}

void CRejectZone::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRejectZone::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}



bool CRejectZone::FindChip(int &r , int &c )
{
    c = DM.ARAY[riPSB].FindLastCol(csFail) ;

    return (c > -1 && c > -1);
}

double CRejectZone::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos     = 0.0 ;
    int r,c ;

    int iMaxCol = DM.ARAY[riPSB].GetMaxCol();
    FindChip(r , c);

    //횡으로 플립된값.
    int fc = (iMaxCol - c) -1 ;

    //1그룹별 칩의 갯수.
    int iGroupChipCnt = iMaxCol / OM.DevInfo.iColGrCnt ;


    //그릅단위의 포지션.
    double dGroupStartPos = fc / iGroupChipCnt * OM.DevInfo.dColGrPitch ;

    //그룹을 뺀 나머지 칩단위 포지션.
    double dChipPos       = (fc % iGroupChipCnt * OM.DevInfo.dColGrPitch) * OM.DevInfo.dColPitch;
    double dWorkPos       = PM.GetValue (_iMotr , pvPSB_XCmpWorkStt ) + dGroupStartPos + dChipPos;

    if(_iMotr == miPSB_XCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piPSB_XCmpWait    : dPos = PM.GetValue (_iMotr , pvPSB_XCmpWait    ); break ;
            case piPSB_XCmpGrip    : dPos = PM.GetValue (_iMotr , pvPSB_XCmpGrip    ); break ;
            case piPSB_XCmpPull    : dPos = PM.GetValue (_iMotr , pvPSB_XCmpPull    ); break ;
            case piPSB_XCmpBack    : dPos = PM.GetValue (_iMotr , pvPSB_XCmpBack    ); break ;
            case piPSB_XCmpWorkStt : dPos = PM.GetValue (_iMotr , pvPSB_XCmpWorkStt ); break ;
            case piPSB_XCmpOut     : dPos = PM.GetValue (_iMotr , pvPSB_XCmpOut     ); break ;
            case piPSB_XCmpWork    : dPos = dWorkPos ;                                 break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRejectZone::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //인덱스 핑거 올라와 있는지.
    bool isIdxUp   = AT_Complete(aiPSB_CmpDnUp  , ccFwd) ;

    //모터 -방향으로 가는지.
    bool isMotrNeg = MT_GetCmdPos(miPSB_XCmp) > GetMotrPos(miPSB_XCmp , _iPstnId) ;


    if(_iMotr == miPSB_XCmp){
        if(isMotrNeg && isIdxUp) {sMsg = "Index Pinger is Up"; bRet = false ;}
        switch(_iPstnId) {
            default                :  break ;
            case piPSB_XCmpWait    :  break ;
            case piPSB_XCmpGrip    :  break ;
            case piPSB_XCmpPull    :  break ;
            case piPSB_XCmpBack    :  break ;
            case piPSB_XCmpWorkStt :  break ;
            case piPSB_XCmpOut     :  break ;
            case piPSB_XCmpWork    :  break ;
        }




    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CRejectZone::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool isXMoving = !MT_GetStopInpos(miPSB_XCmp);

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiPSB_CmpDnUp) {
        if(_bFwd) {
            if(isXMoving) { sMsg = AnsiString(MT_GetName(miPSB_XCmp))+" is Moving" ; bRet = false ;}
        }
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CRejectZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRejectZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRejectZone::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiPSB_HomeTo);
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

        case 10: AT_MoveCyl(aiPSB_CmpDnUp  ,ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_CmpDnUp  ,ccBwd)) return false ;
                 MT_SetHomeEnd(miPSB_XCmp , false);
                 MT_DoHome(miPSB_XCmp);
                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XCmp)) return false;
                 MT_GoAbsMan(miPSB_XCmp, PM.GetValue(miPSB_XCmp , pvPSB_XCmpWait));
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPSB_XCmp, PM.GetValue(miPSB_XCmp , pvPSB_XCmpWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CRejectZone::Autorun(void) //오토런닝시에 계속 타는 함수.
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if (IO_GetX(xPSB_CmpOvld ) ) EM_SetErr(eiPSB_CmpOvld    );

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        bool bWorkPos    =  GetMotrPos(miPSB_XCmp,piPSB_XCmpWork) == MT_GetCmdPos(miPSB_XCmp) ;
        bool bWrkStart   =  !DM.ARAY[riWRK].CheckAllStat(csNone) && DM.ARAY[riWRK].GetCntStat(csUnkwn) == 0 &&
                                                                    DM.ARAY[riWRK].GetCntStat(csWait ) == 0 &&
                                                                    DM.ARAY[riWRK].GetCntStat(csMask ) == 0 ;

        bool isCycleIn   =  DM.ARAY[riPSB].CheckAllStat(csNone) && !DM.ARAY[riWRK].CheckAllStat(csNone) && bWrkStart ;
        bool isCycleMove =  DM.ARAY[riPSB].GetCntStat  (csFail) && !bWorkPos ;
        bool isCycleOut  = !DM.ARAY[riPSB].GetCntStat  (csFail) && !DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].GetCntStat(csEmpty) && ULD.GetSeqStep() == CUnLoader :: scIdle ;

        bool isCycleEnd  =  DM.ARAY[riPSB].CheckAllStat(csNone) ;

        //Unknown Strip
        //if (IO_GetX(xPSB_Detect) && !OM.MstOptn.bDryRun && DM.ARAY[riPSB].CheckAllStat(csNone)) EM_SetErr(eiPSB_Unknown) ;

        //Dissapear Strip
        //if (isCycleOut && !OM.MstOptn.bDryRun) {
        //    if (!DM.ARAY[riPSB].CheckAllStat(csNone) && !IO_GetX(xPSB_Detect) ) EM_SetErr(eiPSB_Dispr)   ;
        //}

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleOut  ) {Trace(m_sPartName.c_str(),"CycleOut  Stt"); Step.iSeq = scOut  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleMove ) {Trace(m_sPartName.c_str(),"CycleMove Stt"); Step.iSeq = scMove ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn   ) {Trace(m_sPartName.c_str(),"CycleIn   Stt"); Step.iSeq = scIn   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                   return false ;
        case scIn    : if(CycleIn  ()){ Trace(m_sPartName.c_str(),"CycleIn   End");Step.iSeq = scIdle ;} return false ;
        case scMove  : if(CycleMove()){ Trace(m_sPartName.c_str(),"CycleMove End");Step.iSeq = scIdle ;} return false ;
        case scOut   : if(CycleOut ()){ Trace(m_sPartName.c_str(),"CycleOut  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CRejectZone::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CRejectZone::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRejectZone::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: bExstUnKnown = DM.ARAY[riPSB].GetCntStat(csFail)>0 ;
                 IO_SetY(yPSB_RwkVac,bExstUnKnown);
                 //MoveActr(aiPSB_CmpDnUp , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 11: //if(!MoveActr(aiPSB_CmpDnUp , ccFwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CRejectZone::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiPSB_ToStopTO);

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

        case 10: IO_SetY(yPSB_FeedAC , false );
                 MoveActr(aiPSB_PshFwBw , ccBwd);
                 MoveActr(aiPSB_RlrDnUp , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_PshFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_RlrDnUp , ccBwd)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//리워크 존까지 가져가는 것.
bool CRejectZone::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yPSB_RwkVac,false);
                  MoveActr(aiPSB_CmpDnUp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_CmpDnUp , ccBwd)) return false ;

                  MoveMotr(miPSB_XCmp , piPSB_XCmpWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp , piPSB_XCmpWait)) return false ;
                  MoveActr(aiPSB_CmpDnUp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_CmpDnUp , ccFwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XCmpPull);
                  DM.ShiftArrayData(riWRK,riPSB);
                  DM.ARAY[riPSB].SetStep(0); //워크 시작 트리거.
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XCmpPull)) return false ;
                  MoveActr(aiPSB_CmpDnUp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPSB_CmpDnUp , ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XCmpBack);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPSB_XCmp , piPSB_XCmpBack)) return false ;
                  Step.iCycle=0;
                  return true;
    }
}

//작업 위치로 이동.
bool CRejectZone::CycleMove()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yPSB_RwkVac,false);
                  MoveActr(aiPSB_CmpDnUp , ccFwd);
                  MoveActr(aiPSB_PshFwBw , ccBwd);
                  MoveActr(aiPSB_RlrDnUp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_CmpDnUp , ccFwd)) return false ;
                  if(!MoveActr(aiPSB_PshFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_RlrDnUp , ccBwd)) return false ;

                  MoveMotr(miPSB_XCmp , piPSB_XCmpWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp , piPSB_XCmpWork)) return false ;
                  DM.ARAY[riPSB].SetStep(DM.ARAY[riPSB].GetStep() + 1); //워크 시작 트리거. 정해진 피치 이동이 아니라 쓸려면 파인드칩으로 찾아서 매겨야함.
                  IO_SetY(yPSB_RwkVac,true);
                  Step.iCycle=0;
                  return true;
    }
}

//Back위치에서 Out위치까지 밀어 넣음.
bool CRejectZone::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());

        IO_SetY(yPSB_FeedAC,false);
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
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yPSB_RwkVac,false);
                  MoveActr(aiPSB_CmpDnUp , ccFwd);
                  MoveActr(aiPSB_PshFwBw , ccBwd);
                  MoveActr(aiPSB_RlrDnUp , ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_CmpDnUp , ccFwd)) return false ;
                  if(!MoveActr(aiPSB_PshFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_RlrDnUp , ccBwd)) return false ;
                  if(ULD.GetSeqStep() != CUnLoader :: scIdle) return false  ; //졸라짜증나는 1싸이클 사이 에 들어 올수 있어서 막아놓음.
                  MoveMotr(miPSB_XCmp , piPSB_XCmpOut);

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp , piPSB_XCmpOut)) return false ;
                  MoveActr(aiPSB_CmpDnUp , ccBwd);
                  MoveActr(aiPSB_RlrDnUp , ccFwd);

                  //
                  IO_SetY(yPSB_FeedAC , true);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_CmpDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_RlrDnUp , ccFwd)) return false ;
                  //
                  MoveMotr(miPSB_XCmp , piPSB_XCmpWait);

                  MoveActr(aiPSB_PshFwBw , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XCmpWait)) return false ;
                  if(!MoveActr(aiPSB_PshFwBw , ccBwd)      ) return false ;
                  if(IO_GetX  (xPSB_PkgPshrDn7            )) return false ;

                  MoveActr(aiPSB_PshFwBw , ccFwd);
                  Step.iCycle++;
                  return false ;


        case  15: if(!MoveActr(aiPSB_PshFwBw , ccFwd)) return false ;
                  MoveActr(aiPSB_PshFwBw , ccBwd);
                  MoveActr(aiPSB_RlrDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiPSB_PshFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_RlrDnUp , ccBwd)) return false ;
                  IO_SetY(yPSB_FeedAC,false);

                  DM.ARAY[riPSB].SetStat(csNone) ;
                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);

                  Step.iCycle=0;
                  return true;
    }
}

void CRejectZone::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CRejectZone::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRejectZone::CheckStop()
{
    if(!MT_GetStop(miPSB_XCmp)) return false ;

    if(!AT_Done(aiPSB_CmpDnUp)) return false ;
    if(!AT_Done(aiPSB_PshFwBw)) return false ;
    if(!AT_Done(aiPSB_RlrDnUp)) return false ;


    return true ;
}

void CRejectZone::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PreBuff.INI";

    //if ( _bLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );

    //else {
    //    UserINI.ClearFile(sPath) ;
    //    UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    //}

}
//---------------------------------------------------------------------------
