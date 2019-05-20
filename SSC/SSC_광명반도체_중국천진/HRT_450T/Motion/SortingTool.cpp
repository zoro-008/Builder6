//---------------------------------------------------------------------------
#pragma hdrstop

#include "SortingTool.h"

#include "PostBuff.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ProbeComUnit.h"
#include "LotUnit.h"
#include "Array.h"
#include "PostBuff.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
CSortingTool STL;
//---------------------------------------------------------------------------
CSortingTool::CSortingTool(void)
{
    m_sPartName = "SortingTool " ;
    Reset();
    m_dFailChipPos = 0 ;
    Load(true);
    TopRowCnt = -1;
    BtmRowCnt = -1;

    InitCycleName();
}
//---------------------------------------------------------------------------
void CSortingTool::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
//---------------------------------------------------------------------------
void CSortingTool::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}
//---------------------------------------------------------------------------
CSortingTool::~CSortingTool (void)
{
    Load(false);
}
//---------------------------------------------------------------------------
bool CSortingTool::GetReadyPos(EN_CHIP_STAT Mode)
{

    //요놈들 문제가 의심되어 디버깅용으로 많이 풀어 놨음. 요거만 삽질 않하면됨.
    double dXCmdPos    = MT_GetCmdPos(miPSB_XCmp);                            //명령 포지션
    double dXEncPos    = MT_GetEncPos(miPSB_XCmp);                            //엔코더 포지션
    double dXInpos     = 0.1 ;                                                //넉넉하게 메뉴얼로 삽입한 인포지션.
    double dFailPos    = PSB.GetMotrPos(miPSB_XCmp , piPSB_XWork , Mode);     //페일자제가 있는 포지션
    bool   bStoped     = MT_GetStop(miPSB_XCmp);                              //모터정지여부.
    bool   bXInposStop = bStoped && (abs(dXCmdPos - dXEncPos) < dXInpos) ;    //메뉴얼로 삽입한 인포지션 적용한 인포지션 스탑 확인.

    //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;              //인포지션스탑이고 최종명령했던 포지션과 페일자제 있는 포지션이 다를 경우 이동필요.
    bool   bReadyPos   = bXInposStop && abs(dXCmdPos - dFailPos)< dXInpos ;   //이상하다 dXCmdPos == dFailPos 일때 같지 않다고 리턴됨.

    return bReadyPos ;
    ////////////////////////////////////////////////////////////////////////
}

bool CSortingTool::FindChip(int &r , int &c , EN_CHIP_STAT Stat)
{
    DM.ARAY[riPSB].FindLastColFrstRow(Stat,r,c ) ;
    return (r > -1 && c > -1);
}
//---------------------------------------------------------------------------
double  CSortingTool::RejectMove(void)
{
    if((DM.ARAY[riPSB].GetLotNo() != m_sLastStrip) /*&& (m_sLastID != "" )*/)
    {
        m_iBinCnt++;
        if(m_iBinCnt >= MAXBIN)   m_iBinCnt = 0;
        m_sLastStrip = DM.ARAY[riPSB].GetLotNo();
    }

    m_dFailChipPos = PM.GetValue(miREJ_XBtm , pvREJ_XBWorkStart) + (OM.DevOptn.dREJPitch * m_iBinCnt) ;

    return m_dFailChipPos;

}
//---------------------------------------------------------------------------
double CSortingTool::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int Col , Col1 , Col2 , Col3;
    int Row , Row1 , Row2 , Row3;
    double dPos = 0.0;
    int r1 , c1 , r2 , c2 , r3 , c3;

    FindChip(Row1 , Col1 , csFail);
    FindChip(Row2 , Col2 , csWork);
    FindChip(Row3 , Col3 , csMask);

    if(Step.iSeq == scDevChk) {Row = Row1;}
    if(Step.iSeq == scPunch ) {Row = OM.CmnOptn.bIgnrDecChk ? Row1 : Row2;}
    if(Step.iSeq == scTrmChk) {Row = Row3;}

    Row = OM.DevInfo.iRowCnt - Row - 1 ;  // 맨밑이 워크 스타트 포지션으로 바꿈.

    double dY1Pos;
    //X , Y Pos Setting.
    if(Step.iSeq == scDevChk || Step.iSeq == scTrmChk) {
        dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTDevCheck) + (OM.DevInfo.dRowPitch * Row) ;
    }
    else {
        dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) + (OM.DevInfo.dRowPitch * Row) ;
    }

    if( OM.DevInfo.iRowGrCnt > 1 ) {
        //iColGrCnt는  fc  의 그룹갯수. OM.DevInfo.iColGrCnt이거는 한그룹당 LED 갯수.
        int iRowGrCnt = Row / OM.DevInfo.iRowGrCnt ;
        dY1Pos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    }

    double dYTStepPos;
    double dYBStepPos;

    if(_iMotr == miSTL_YTop){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piSTL_YTWait        : dPos = PM.GetValue (_iMotr , pvSTL_YTWait            ); break ;
            case piSTL_YTWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YTWorkStart       ); break ;
            case piSTL_YTDevCheck    : dPos = PM.GetValue (_iMotr , pvSTL_YTDevCheck        ); break ;
            case piSTL_YTPlace       : dPos = PM.GetValue (_iMotr , pvSTL_YTPlace           ); break ;
            case piSTL_YTWork        : dPos = dY1Pos                                         ; break ;


        }
    }
    else if(_iMotr == miREJ_XBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piREJ_XBWait        : dPos = PM.GetValue (_iMotr , pvREJ_XBWait            ); break ;
            case piREJ_XBWorkStart   : dPos = PM.GetValue (_iMotr , pvREJ_XBWorkStart       ); break ;
            case piREJ_XBWork        : dPos = m_dFailChipPos                                 ; break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
//---------------------------------------------------------------------------
bool CSortingTool::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId))) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isSortingDown  =  AT_Complete(aiSTL_Top , ccFwd) ;
    bool isPunchingUp   =  AT_Complete(aiSTL_Btm , ccFwd) ;

    if(_iMotr == miSTL_YTop){
        if(isSortingDown) {sMsg = "During Punching"; bRet = false ;}
    }
    else if(_iMotr == miREJ_XBtm){
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CSortingTool::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;
    int iPunchOfs = 3;

    bool isXMovingTop = !MT_GetStopInpos(miSTL_YTop);
    bool isPSBXMoving = !MT_GetStopInpos(miPSB_XCmp);
    //bool isYTopWaitPs = MT_GetStopInpos(miSTL_YTop) && (MT_GetCmdPos(miSTL_YTop) <= PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) - iPunchOfs);

    if(_iActr == aiSTL_Top) {
        if( !_bFwd) {
            //if(isYTopWaitPs) { sMsg = "Punching Damage Position "         ; bRet = false ;}
            if(isXMovingTop) { sMsg = MT_GetName(miSTL_YTop)+" is Moving" ; bRet = false ;}
            if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}
        }
    }
    else if(_iActr == aiSTL_Btm) {
        if( !_bFwd) {
            if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}

        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool CSortingTool::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}
//---------------------------------------------------------------------------
bool CSortingTool::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
//---------------------------------------------------------------------------
bool CSortingTool::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_HomeTO);
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

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiPSB_Cmp,ccBwd); //혹시 모를 간섭 .
                 AT_MoveCyl(aiSTL_Top,ccBwd);
                 AT_MoveCyl(aiSTL_Btm,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Cmp,ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Top,ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Btm,ccBwd))return false ;
                 MT_DoHome(miSTL_YTop);
                 MT_DoHome(miREJ_XBtm);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miSTL_YTop))return false;
                 if(!MT_GetHomeEnd(miREJ_XBtm))return false;
                 MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait));
                 MT_GoAbsMan(miREJ_XBtm, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait)))return false ;
                 if(!MT_GoAbsMan(miREJ_XBtm, m_dLastIndPos))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CSortingTool::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        int iMaxCol, fc , iReGribCol;
        int c1 , r1 ;

        iMaxCol    = DM.ARAY[riWRK].GetMaxCol();
        iReGribCol = OM.DevOptn.iPSBReGripCol ; //30이라고 넣으면 30까지 작업 하고 31부터 리그립.   iMaxCol / 2 ; //스트립 중간까지 작업 하고 리그립한다.
        bool bExistFail     = FindChip(r1 , c1 , csFail);

        fc = (iMaxCol - c1) -1  ; //원래 데이터를 뒤집은 것.
        bool isExstTrim1    =  bExistFail && fc <  iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //스트립 전반전에 트림 할 것이 있느냐 하는 옵션.
        bool isExstTrim2    =  bExistFail && fc >= iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //스트립 후반전에 트림 할 것이 있느냐 하는 옵션.

        bool isPSBFailReady =  GetReadyPos(csFail) && ((isExstTrim1 && PSB.GetRegribCnt() == 0) || (isExstTrim2 && PSB.GetRegribCnt() == 1)) &&
                               PSB.GetCycleStep() == CPostBuff::scIdle;

        bool isTrimCon     =  OM.CmnOptn.bIgnrDecChk ? DM.ARAY[riPSB].GetCntStat(csFail) : DM.ARAY[riPSB].GetCntStat(csWork) && PSB.GetCycleStep() == CPostBuff::scIdle ;  //MT_GetStop(miPSB_XCmp);
        bool isGetReadyPos =  OM.CmnOptn.bIgnrDecChk ? GetReadyPos(csFail) : GetReadyPos(csWork) && !GetReadyPos(csFail) && PSB.GetCycleStep() == CPostBuff::scIdle; //PSB.GetCycleStep() == CPstBuffer::scIdle 이거 있어야 홀드 안남. 3번 인덱스 프리버퍼 동작중에  포지션 인식되어 디바이스 체크 들어감.

        bool isCycDevChk   =  DM.ARAY[riPSB].GetCntStat(csFail) && isPSBFailReady && !OM.CmnOptn.bIgnrTrim  && !OM.CmnOptn.bIgnrDecChk && MT_GetStop(miPSB_XCmp)  ;
        bool isCycTrmChk   = !DM.ARAY[riPSB].GetCntStat(csWork) &&
                              GetReadyPos(csMask) && !OM.CmnOptn.bIgnrTrim && DM.ARAY[riPSB].GetCntStat(csMask) && !OM.CmnOptn.bIgnrTrimDecChk;


        ////


        bool isCyclePunch  =  isTrimCon                           &&
                              DM.ARAY[riPSB].GetSubStep()         &&
                              //DM.ARAY[riSTL].GetCntStat(csNone)   &&
                              isGetReadyPos                       &&
                             !OM.CmnOptn.bIgnrTrim                &&
                              PSB.GetSeqStep() == CPostBuff :: scIdle ;

        bool isCycleEnd  = DM.ARAY[riPSB].CheckAllStat(csNone) ;//&& //DM.ARAY[riSTL].CheckAllStat(csNone)                    ;



        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycDevChk ) {Trace(m_sPartName.c_str(),"CycleDevChk Stt"); Step.iSeq = scDevChk   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCyclePunch) {Trace(m_sPartName.c_str(),"CyclePunch  Stt"); Step.iSeq = scPunch    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycTrmChk ) {Trace(m_sPartName.c_str(),"CycleTrmChk Stt"); Step.iSeq = scTrmChk   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }
    //Cycle.
    switch (Step.iSeq) {
        default        :                       Trace(m_sPartName.c_str(),"default   End"  );Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                          return false ;
        case scDevChk  : if(CycleDevCheck ()){ Trace(m_sPartName.c_str(),"CycleDevChk End");Step.iSeq = scIdle ;} return false ;
        case scPunch   : if(CyclePunch    ()){ Trace(m_sPartName.c_str(),"CyclePunch  End");Step.iSeq = scIdle ;} return false ;
        case scTrmChk  : if(CycleDevCheck ()){ Trace(m_sPartName.c_str(),"CycleTrmChk End");Step.iSeq = scIdle ;} return false ;
    }
    return false ;

}
//---------------------------------------------------------------------------
bool CSortingTool::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    Stat.bReqStop = true ;
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CSortingTool::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false; ///fail 시 계속 검사 할수도...  Stat.iReInspCnt = 0;

    Step.iToStart = 10 ;

    //Ok.
    return true;
}

bool CSortingTool::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiSTL_ToStartTO);

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

        case 10: MoveActr(aiSTL_Top , ccBwd);
                 MoveActr(aiSTL_Btm , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiSTL_Top , ccBwd))return false ;
                 if(!MoveActr(aiSTL_Btm , ccBwd))return false ;
                 MoveMotr(miSTL_YTop , piSTL_YTWait);
                 MoveMotr(miREJ_XBtm , piREJ_XBWork);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miREJ_XBtm , piREJ_XBWork)) return false ;
                 if(!MoveMotr(miSTL_YTop , piSTL_YTWait)) return false ;
                 //DM.ARAY[riSTL].SetStat(csNone);

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CSortingTool::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiSTL_ToStopTO);

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

        case 10: AT_MoveCyl(aiSTL_Btm,ccBwd);
                 AT_MoveCyl(aiSTL_Top,ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSTL_Btm,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSTL_Top,ccBwd)) return false ;
                 if(!MT_GetAlarm   (miREJ_XBtm) &&
                    !MT_GetNLimSnsr(miREJ_XBtm) &&
                    !MT_GetPLimSnsr(miREJ_XBtm) ) m_dLastIndPos = MT_GetCmdPos(miREJ_XBtm) ;
                 else                             m_dLastIndPos = GetMotrPos  (miREJ_XBtm , piREJ_XBWait) ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CSortingTool::CycleDevCheck(void)
{
    int r1 , c1 , r2 , c2 , r3 , c3;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_CycleTO);
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
    FindChip(r1 , c1 , csFail);
    FindChip(r2 , c2 , csWork);
    FindChip(r3 , c3 , csMask);
    int iLastCol = OM.DevInfo.iColCnt - 1;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiSTL_Top , ccBwd);
                  MoveActr(aiSTL_Btm , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  if(!MoveActr(aiSTL_Btm , ccFwd)) return false;
                  Step.iCycle++;
                  return false;

        case  12: MoveMotr(miSTL_YTop , piSTL_YTWork);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimSnrDly)) return false;
                  if(IO_GetX(xSTL_DevDetect)) {
                      if(!OM.CmnOptn.bIgnrDecChk     && Step.iSeq == scDevChk){
                          DM.ARAY[riPSB].SetStat(r1,c1,csWork );
                          m_iLastDecCol = c1;
                          m_iLastDecRow = r1;
                          Trace("WorkMask",(AnsiString(r1) + "," + AnsiString(c1)).c_str());
                      }
                      if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk) {
                          EM_SetErr(eiSTL_Trim);
                          DM.ARAY[riPSB].SetStat(r3,c3,csFail);
                          m_iLastTrmDecCol = c3 ;
                          m_iLastTrmDecRow = r3 ;
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }
                  else {
                      if(!OM.CmnOptn.bIgnrDecChk     && Step.iSeq == scDevChk) {
                          DM.ARAY[riPSB].SetStat(r1,c1,csEmpty);
                          m_iLastDecCol    = c1 ;
                          m_iLastDecRow    = r1 ;
                      }
                      if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk) {
                          DM.ARAY[riPSB].SetStat(r3,c3,csEmpty);
                          //m_sLastLotNo = DM.ARAY[riPSB].GetLotNo();
                          m_iTrimCnt++;
                          //LT.WriteTrimInfo(DM.ARAY[riPSB].GetLotNo(), m_iTrimCnt, m_iBinCnt);
                          m_iLastTrmDecCol = c3 ;
                          m_iLastTrmDecRow = r3 ;
                      }
                  }
                  Step.iCycle++;
                  return false;

        case  15: if(!OM.CmnOptn.bIgnrDecChk && Step.iSeq == scDevChk) {
                      if(DM.ARAY[riPSB].FindLastCol(csFail) != m_iLastDecCol && !DM.ARAY[riPSB].GetCntStat(csWork)){
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle++;
                          return false ;
                      }
                      else {
                          Step.iCycle = 0;
                          return true ;
                      }
                  }

                      //if(DM.ARAY[riPSB].GetCntStat(csFail)){
                      //    Step.iCycle = 0;
                      //    return true;
                      //}
                      //else if(c1 == m_iLastDecCol && r1 == m_iLastDecRow){
                      //    Step.iCycle = 0;
                      //    return true;
                      //}
                      //else if(!DM.ARAY[riPSB].GetCntStat(csFail)){
                      //    MoveMotr(miSTL_YTop , piSTL_YTWait);
                      //    Step.iCycle++;
                      //    return false;
                      //}
                      //else {
                      //    Step.iCycle = 0;
                      //    return true ;
                      //}
                      //
                      //if(DM.ARAY[riPSB].GetCntStat(csWork)){
                      //    Step.iCycle = 0;
                      //    return true;
                      //}
                      //else {
                      //    MoveMotr(miSTL_YTop , piSTL_YTWait);
                      //    Step.iCycle++;
                      //    return false ;
                      //}
                  else if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk){
                      if(c3 == m_iLastTrmDecCol){
                          Step.iCycle = 0;
                          return true;
                      }
                      else if(c3 == m_iLastTrmDecCol && r3 == m_iLastTrmDecRow){
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                      else {
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                  }
                  else {
                      MoveMotr(miSTL_YTop , piSTL_YTWait);
                      Step.iCycle++;
                      return false;
                  }
                  /*
                  if(!OM.CmnOptn.bIgnrDecChk && Step.iSeq == scDevChk) {
                      if(c1 == m_iLastDecCol){
                          Step.iCycle = 0;
                          return true;
                      }
                      else if(c1 == m_iLastDecCol && r1 == m_iLastDecRow){
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                      else {
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                  }
                  else if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk){
                      if(c3 == m_iLastTrmDecCol){
                          Step.iCycle = 0;
                          return true;
                      }
                      else if(c3 == m_iLastTrmDecCol && r3 == m_iLastTrmDecRow){
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                      else {
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                  }
                  else {
                      MoveMotr(miSTL_YTop , piSTL_YTWait);
                      Step.iCycle++;
                      return false;
                  }
                  */

        case  16: MoveActr(aiSTL_Btm , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miSTL_YTop , piSTL_YTWait)) return false;
                  if(!MoveActr(aiSTL_Btm , ccBwd        )) return false;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CSortingTool::CyclePunch(void)
{
    int r1 , c1 , r2 , c2 , r3 , c3;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_CycleTO);
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

    //방향설정
//    if(!m_bDir) DM.ARAY[riPS1].FindLastColFrstRow(csFail , c , r);
//    else        DM.ARAY[riPS1].FindLastColRow    (csFail , c , r);
    //DM.ARAY[riPS1].FindLastColFrstRow(csFail , r , c);
    FindChip(r1 , c1 , csFail);
    FindChip(r2 , c2 , csWork);
    //FindChip(r3 , c3 , csMask);

    if(OM.CmnOptn.bIgnrTrim)
    {
        Trace("IgnrTrimOption_Msg","Pick_ Cycle");
        Step.iCycle = 0;
        return true;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miREJ_XBtm , piREJ_XBWork);
                  if(OM.CmnOptn.bUsedTopViewPnch) IO_SetY(ySTL_VaccSol , true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miREJ_XBtm , piREJ_XBWork)) return false;
                  if(m_iBinCnt == 0 && !IO_GetX(xREJ_Box1     )) {EM_SetErr(eiREJ_Box1     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 1 && !IO_GetX(xREJ_Box2     )) {EM_SetErr(eiREJ_Box2     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 2 && !IO_GetX(xREJ_Box3     )) {EM_SetErr(eiREJ_Box3     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 3 && !IO_GetX(xREJ_Box4     )) {EM_SetErr(eiREJ_Box4     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 4 && !IO_GetX(xREJ_Box5     )) {EM_SetErr(eiREJ_Box5     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 5 && !IO_GetX(xREJ_Box6     )) {EM_SetErr(eiREJ_Box6     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 6 && !IO_GetX(xREJ_Box7     )) {EM_SetErr(eiREJ_Box7     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 7 && !IO_GetX(xREJ_Box8     )) {EM_SetErr(eiREJ_Box8     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 8 && !IO_GetX(xREJ_Box9     )) {EM_SetErr(eiREJ_Box9     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 9 && !IO_GetX(xREJ_Box10    )) {EM_SetErr(eiREJ_Box10    ); Step.iCycle = 0 ; return true ; }
                  MoveActr(aiSTL_Top , ccBwd);
                  PSB.MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiSTL_Top     , ccBwd       )) return false;
                  if(!PSB.MoveActr(aiPSB_Cmp , ccFwd       )) return false;
                  Step.iCycle++;
                  return false;

        case  13: MoveActr(aiSTL_Btm , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiSTL_Btm , ccFwd)) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false ;
                  MoveActr(aiSTL_Top , ccFwd);
                  if(OM.CmnOptn.bUsedTopViewPnch){
                      IO_SetY(ySTL_PnchVacc , true );
                      IO_SetY(ySTL_PnchEjct , false);
                      m_tmTemp.Clear();
                      Step.iCycle = 20;
                      return false;
                  }
                  IO_SetY(ySTL_Ejecter , true);
                  m_tmTemp.Clear();
                  //m_tmTemp.OnDelay(true, OM.DevOptn.iTrimRjtDly);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  if(!m_tmTemp.OnDelay(true ,  OM.DevOptn.iTrimRjtDly)) return false ;
                  IO_SetY(ySTL_Ejecter , false);
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  Step.iCycle = 100;
                  return false;

        case  20: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimRjtDly)) return false;
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTPlace);
                  Step.iCycle++;
                  return false ;

        case  22: if(!MoveMotr(miSTL_YTop , piSTL_YTPlace)) return false;
                  MoveActr(aiSTL_Top , ccFwd);
                  Step.iCycle++;
                  return false;

        case  23: if(!MoveActr(aiSTL_Top , ccFwd))return false;
                  IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , true );
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  24: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimRjtDly)) return false;
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  25: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , false);
                  Step.iCycle = 100;
                  return false ;


        case 100: if(!OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      DM.ARAY[riPSB].SetStat(r2,c2,csEmpty);
                      m_iLastWorkCol = c2;
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && !OM.CmnOptn.bIgnrTrimDecChk) {
                      DM.ARAY[riPSB].SetStat(r1,c1,csMask);
                      m_iLastWorkCol = c1;
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      DM.ARAY[riPSB].SetStat(r1,c1,csEmpty);
                      m_iLastWorkCol = c1;
                  }
                  else {
                      DM.ARAY[riPSB].SetStat(r2,c2,csMask);
                      m_iLastWorkCol = c2;
                  }
                  Step.iCycle++;
                  return false;

        //준선이 머리는 바보....젠장... 가라로 한다 우선...퇴근!!.
        case 101: if(!OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      if(c2 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      }
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && !OM.CmnOptn.bIgnrTrimDecChk) {
                      //if(c1 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      //}
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      if(c1 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      }
                  }
                  else {
                      //if(c2 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      //}
                  }
                  MoveActr(aiSTL_Btm , ccBwd);
                  MoveMotr(miSTL_YTop , piSTL_YTWait);
                  Step.iCycle++;
                  return false;

        case 102: if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  if(!MoveMotr(miSTL_YTop , piSTL_YTWait)) return false;
                  Step.iCycle = 0 ;
                  return true ;

    }
}

void CSortingTool::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CSortingTool::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CSortingTool::CheckStop()
{
    if(!MT_GetStop(miSTL_YTop)) return false ;
    if(!MT_GetStop(miREJ_XBtm)) return false ;
    if(!AT_Done(aiSTL_Top    )) return false ;
    if(!AT_Done(aiSTL_Btm    )) return false ;

    return true ;
}

void CSortingTool::Load(bool IsLoad)   //고민해야함.....머 세이브 할지.....
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\SortingTool.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLastID     "  , m_sLastID       );
        UserINI.Load(sPath, "Member" , "m_iBinCnt     "  , m_iBinCnt       );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos "  , m_dLastIndPos   );
        UserINI.Load(sPath, "Member" , "m_sLastStrip  "  , m_sLastStrip    );
        UserINI.Load(sPath, "Member" , "m_dFailChipPos"  , m_dFailChipPos  );
        UserINI.Load(sPath, "Member" , "m_iTrimCnt    "  , m_iTrimCnt      );
        UserINI.Load(sPath, "Member" , "m_sLastLotNo  "  , m_sLastLotNo    );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLastID"       , m_sLastID       );
        UserINI.Save(sPath, "Member" , "m_iBinCnt"       , m_iBinCnt       );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos"   , m_dLastIndPos   );
        UserINI.Save(sPath, "Member" , "m_sLastStrip"    , m_sLastStrip    );
        UserINI.Save(sPath, "Member" , "m_dFailChipPos"  , m_dFailChipPos  );
        UserINI.Save(sPath, "Member" , "m_iTrimCnt    "  , m_iTrimCnt      );
        UserINI.Save(sPath, "Member" , "m_sLastLotNo  "  , m_sLastLotNo    );
    }
}
//---------------------------------------------------------------------------













