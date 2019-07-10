//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Head.h"
#include "Rail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CHead  HD;
const AnsiString sPartName = "Head " ;

CHead::CHead(void)
{
    Reset();
}

void CHead::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CHead::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CHead::~CHead (void)
{

}

bool CHead::FindChip( int &r , int &c )
{
    int r1 , c1 , r2 , c2 , r3 , c3;

    DM.ARAY[aiInsp1].FindFrstRowCol(csUnkwn , r1 , c1); r = r1 ; c = c1 ;
    DM.ARAY[aiInsp2].FindFrstRowCol(csUnkwn , r2 , c2); if(r > r2) { r = r2 ; c = c2 ;}
    DM.ARAY[aiInsp3].FindFrstRowCol(csUnkwn , r3 , c3); if(r > r3) { r = r3 ; c = c3 ;}

    return (r > -1 && r > -1);
}

double CHead::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int r  , c  ;
    const double dOfsforTrg = 5.0 ; //아진 인터럽트 트리거를 위한 옵셑.

    //요놈. 플라잉 방식이라 맵핑 필요 없음.
    FindChip( r , c ) ;

    //끄트머리에 Insp Cnt가 꽉 차지 않았을때 이용.
    int    iVtlColCnt = OM.DevInfo.iColCnt % OM.DevInfo.iColInspCnt ? (OM.DevInfo.iColCnt / OM.DevInfo.iColInspCnt) * OM.DevInfo.iColInspCnt  + OM.DevInfo.iColInspCnt :
                                                                      (OM.DevInfo.iColCnt / OM.DevInfo.iColInspCnt) * OM.DevInfo.iColInspCnt                           ;

    int    iVtlRowCnt = OM.DevInfo.iRowCnt % OM.DevInfo.iRowInspCnt ? (OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspCnt) * OM.DevInfo.iRowInspCnt  + OM.DevInfo.iRowInspCnt :
                                                                      (OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspCnt) * OM.DevInfo.iRowInspCnt                           ;


    //트리거 까지 고려한 포지션.
    double dLeftPos   = PM.GetPos(miVS2_X , pvVS2_XInspSttPs ) -
                        dOfsforTrg ;

    double dRightPos  = PM.GetPos(miVS2_X , pvVS2_XInspSttPs ) +
                        OM.DevInfo.dColPitch * iVtlColCnt + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (iVtlColCnt/OM.DevInfo.iColGrCnt) +
                        dOfsforTrg ;

    //스타트 포지션과 엔드 포지션 구분.
    double dXStartPos = (r/OM.DevInfo.iRowInspCnt)%2 ? dRightPos : dLeftPos  ;
    double dXEndPos   = (r/OM.DevInfo.iRowInspCnt)%2 ? dLeftPos  : dRightPos ;
    double dYPos      =  PM.GetPos(miVS2_Y , pvVS2_YInspSttPs ) +           //첫포지션 더하고.
                        (r/OM.DevInfo.iRowInspCnt) * OM.DevInfo.dRowPitch + //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
                        (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (r/OM.DevInfo.iRowGrCnt) ;

    int iTrgCnt = iVtlColCnt / OM.DevInfo.iColInspCnt ;

    double * dTrgPos = new double[iTrgCnt] ;

    for(int i = 0 ; i < iTrgCnt ; i++) {
        //dTrgPos[i] = dXStartPos + OM.DevInfo.iColInspCnt * i

    }

    double dPos = 0.0 ;

    if(_iMotr == miVS2_X){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piVS2_XWait       : dPos = PM.GetPos   (_iMotr , pvVS2_XWaitPs    ); break ;
            case piVS2_XInspStt    : dPos = PM.GetPos   (_iMotr , pvVS2_XInspSttPs ); break ;
            case piVS2_XInspTrgStt : dPos = dXStartPos                              ; break ;
            case piVS2_XInspTrgEnd : dPos = dXEndPos                                ; break ;
        }
    }

    else if(_iMotr == miVS2_Y){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piVS2_YWait       : dPos = PM.GetPos   (_iMotr , pvVS2_YWaitPs    ); break ;
            case piVS2_YInspStt    : dPos = PM.GetPos   (_iMotr , pvVS2_YInspSttPs ); break ;
            case piVS2_YInsp       : dPos = dYPos                                   ; break ;
        }
    }

    else if(_iMotr == miVS1_X){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piVS1_XWait       : dPos = PM.GetPos   (_iMotr , pvVS1_XWaitPs    ); break ;
            case piVS1_XInsp       : dPos = PM.GetPos   (_iMotr , pvVS1_XInspPs    ); break ;
        }
    }

    else if(_iMotr == miVS3_X){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piVS3_XWait       : dPos = PM.GetPos   (_iMotr , pvVS3_XWaitPs    ); break ;
            case piVS3_XInsp       : dPos = PM.GetPos   (_iMotr , pvVS3_XInspPs    ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    delete dTrgPos ;

    return dPos ;
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , PM.GetPos(_iMotr , _iPstnId)) ) return true ;

    if(_iMotr == miVS2_X){
    }

    else if(_iMotr == miVS2_Y){
    }

    else if(_iMotr == miVS1_X){
    }

    else if(_iMotr == miVS3_X){
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CHead::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //Ok.
    return true;
}

bool CHead::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CHead::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CHead::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome == PreStep.iHome && CheckStop() , 5000 )) {
        EM_SetErr(eiRL_HomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    if(Stat.bReqStop) {
        Step.iHome = 0;
        return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 Trace(sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_Home(miVS2_X) ;
                 MT_Home(miVS2_Y) ;
                 MT_Home(miVS1_X) ;
                 MT_Home(miVS3_X) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miVS2_X))return false ;
                 if(!MT_GetHomeEnd(miVS2_Y))return false ;
                 if(!MT_GetHomeEnd(miVS1_X))return false ;
                 if(!MT_GetHomeEnd(miVS3_X))return false ;
                 MoveMotr(miVS2_X,piVS2_XWait );
                 MoveMotr(miVS2_Y,piVS2_YWait );
                 MoveMotr(miVS1_X,piVS1_XWait );
                 MoveMotr(miVS3_X,piVS3_XWait );
                 Step.iHome++;
                 return false ;

        case 12: if(!MoveMotr(miVS2_X,piVS2_XWait ))return false ;
                 if(!MoveMotr(miVS2_Y,piVS2_YWait ))return false ;
                 if(!MoveMotr(miVS1_X,piVS1_XWait ))return false ;
                 if(!MoveMotr(miVS3_X,piVS3_XWait ))return false ;
                 Step.iHome = 0 ;
                 return true ;
    }
}



//One Cycle.
bool CHead::CycleInsp(void) //Rail끝단에서 장비 밖으로 배출 되는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        Step.iCycle = 0;
        return true ;
    }

    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miVS2_X,piVS2_XInspTrgStt );
                  MoveMotr(miVS2_Y,piVS2_YInsp       );
                  MoveMotr(miVS1_X,piVS1_XInsp       );
                  MoveMotr(miVS3_X,piVS3_XInsp       );
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveMotr(miVS2_X,piVS2_XInspTrgStt )) return false ;
                  if(!MoveMotr(miVS2_Y,piVS2_YInsp       )) return false ;
                  if(!MoveMotr(miVS1_X,piVS1_XInsp       )) return false ;
                  if(!MoveMotr(miVS3_X,piVS3_XInsp       )) return false ;
                  MoveMotr(miVS2_X,piVS2_XInspTrgEnd );
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miVS2_X,piVS2_XInspTrgEnd )) return false ;
                  //결과값을 받아 온다.
                  //FindChip(_iStat , r , c ) ;
                  Step.iCycle++ ;
                  return false ;

        case  13: Step.iCycle++ ;
                  return false ;

        case  14: Step.iCycle++ ;
                  return true ;
    }
}

bool CHead::Update(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        if(EM_IsErr()) return false ;

        bool isNeedInspStart =  DM.ARAY[aiInsp1   ].GetCntStat (csUnkwn) ||  DM.ARAY[aiInsp2 ].GetCntStat(csUnkwn) || DM.ARAY[aiInsp3 ].GetCntStat  (csUnkwn) ;
        bool isWaitInsp2Pkg  =  DM.ARAY[aiInsp1   ].GetCntExist(       ) && !DM.ARAY[aiInsp1 ].GetCntStat(csUnkwn) && DM.ARAY[aiInsp2 ].CheckAllStat(csEmpty) ;
        bool isWaitInsp1Pkg  =  DM.ARAY[aiPreBuff ].GetCntExist(       )                                           && DM.ARAY[aiInsp1 ].CheckAllStat(csEmpty) ||
                                RL.GetSeqStep() == CRail::scIn                                                     && DM.ARAY[aiInsp1 ].CheckAllStat(csEmpty) ;

        bool isCycleInsp      = isNeedInspStart  && !isWaitInsp2Pkg && !isWaitInsp1Pkg ;
        bool isConEnd         = !isCycleInsp ;

        //Normal Decide Step.
             if (isCycleInsp ) {Trace(sPartName.c_str(),"CycleInsp Stt"); Step.iSeq = scInsp ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default    :                  Trace(sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scInsp: if(CycleInsp()){ Trace(sPartName.c_str(),"CycleInsp End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CHead::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CHead::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CHead::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiRL_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: Step.iToStart ++ ;
                 return false ;

        case 11: Step.iToStart = 0 ;
                 return true ;
    }
}

bool CHead::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiRL_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

void CHead::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CHead::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CHead::CheckStop()
{
    if(!MT_GetStopInpos(miVS2_X)) return false ;
    if(!MT_GetStopInpos(miVS2_Y)) return false ;
    if(!MT_GetStopInpos(miVS1_X)) return false ;
    if(!MT_GetStopInpos(miVS3_X)) return false ;

    return true ;
}

void CHead::Load(bool IsLoad , FILE *fp)
{

}

















