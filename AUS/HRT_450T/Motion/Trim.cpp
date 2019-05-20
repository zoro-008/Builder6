//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Trim.h"
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
CTrim  TR;
const AnsiString sPartName = "Trim " ;

CTrim::CTrim(void)
{
    Reset();
}

void CTrim::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CTrim::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CTrim::~CTrim (void)
{

}

bool CTrim::FindChip(int &r , int &c )
{
    if(DM.ARAY[aiPostBuff].GetCntStat(csFail )) DM.ARAY[aiPostBuff].FindFrstRowCol(csFail  , r , c);
    if(DM.ARAY[aiPostBuff].GetCntStat(csRslt0)) DM.ARAY[aiPostBuff].FindFrstRowCol(csRslt0 , r , c);
    if(DM.ARAY[aiPostBuff].GetCntStat(csRslt1)) DM.ARAY[aiPostBuff].FindFrstRowCol(csRslt1 , r , c);
    if(DM.ARAY[aiPostBuff].GetCntStat(csRslt2)) DM.ARAY[aiPostBuff].FindFrstRowCol(csRslt2 , r , c);

    return (r > -1 && r > -1);
}

double CTrim::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    //요놈. 플라잉 방식이라 맵핑 필요 없음.
    FindChip(r , c);

    //X , Y Pos Setting.
    double dXPos = PM.GetPos(miTRM_X , pvTRM_XWorkSttPs) +
                   c * OM.DevInfo.dColPitch +
                   (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * c / OM.DevInfo.iColGrCnt ;
    double dYPos = PM.GetPos(miTRM_Y , pvTRM_YWorkSttPs) +
                   r * OM.DevInfo.dRowPitch +
                   (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * c / OM.DevInfo.iRowGrCnt ;

    double dPos  = 0.0 ;

    if(_iMotr == miTRM_X){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piTRM_XWait    : dPos = PM.GetPos   (_iMotr , pvVS2_XWaitPs    ); break ;
            case piTRM_XWorkStt : dPos = PM.GetPos   (_iMotr , pvVS2_XInspSttPs ); break ;
            case piTRM_XWork    : dPos = dXPos                                   ; break ;
        }
    }

    else if(_iMotr == miTRM_Y){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piTRM_YWait    : dPos = PM.GetPos   (_iMotr , pvVS2_YWaitPs    ); break ;
            case piTRM_YWorkStt : dPos = PM.GetPos   (_iMotr , pvVS2_YInspSttPs ); break ;
            case piTRM_YWork    : dPos = dYPos                                   ; break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CTrim::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , PM.GetPos(_iMotr , _iPstnId)) ) return true ;

    if(_iMotr == miTRM_X){
        if(AT_Complete(aiTR_Trimmer , ccFwd)) return false ;
    }

    else if(_iMotr == miTRM_Y){
        if(AT_Complete(aiTR_Trimmer , ccFwd)) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CTrim::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //Ok.
    return true;
}

bool CTrim::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CTrim::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CTrim::CycleHome()
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

        case 10: MoveActr(aiTR_Trimmer , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiTR_Trimmer , ccBwd)) return false ;
                 MT_Home(miTRM_X) ;
                 MT_Home(miTRM_Y) ;
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miTRM_X))return false ;
                 if(!MT_GetHomeEnd(miTRM_Y))return false ;
                 MoveMotr(miTRM_X,piTRM_XWait );
                 MoveMotr(miTRM_Y,piTRM_YWait );
                 Step.iHome++;
                 return false ;

        case 13: if(!MoveMotr(miTRM_X,piTRM_XWait ))return false ;
                 if(!MoveMotr(miTRM_Y,piTRM_YWait ))return false ;
                 Step.iHome = 0 ;
                 return true ;
    }
}



//One Cycle.
bool CTrim::CycleTrim(void) //Rail끝단에서 장비 밖으로 배출 되는 함수.
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

        case  10: MoveActr(aiTR_Trimmer , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiTR_Trimmer , ccBwd)) return false ;
                  MoveMotr(miTRM_X,piTRM_XWork );
                  MoveMotr(miTRM_Y,piTRM_YWork );
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miTRM_X,piTRM_XWork )) return false ;
                  if(!MoveMotr(miTRM_Y,piTRM_YWork )) return false ;
                  MoveActr(aiTR_Trimmer , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiTR_Trimmer , ccFwd)) return false ;
                  FindChip(r,c);
                  DM.ARAY[aiPostBuff].SetStat(r,c,csEmpty) ;
                  Step.iCycle++ ;
                  return false ;

        case  14: Step.iCycle++ ;
                  return true ;
    }
}

bool CTrim::Update(void) //오토런닝시에 계속 타는 함수.
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

        bool isFLP_TrimPos    =  MT_CmprPos(miFRP_T , GetMotrPos(miFRP_T , piFRP_TTrim)) ;


        bool isCycleTrim      =  isFLP_TrimPos &&
                                (DM.ARAY[aiPostBuff].GetCntStat(csFail ) ||
                                 DM.ARAY[aiPostBuff].GetCntStat(csRslt0) ||
                                 DM.ARAY[aiPostBuff].GetCntStat(csRslt1) ||
                                 DM.ARAY[aiPostBuff].GetCntStat(csRslt2) );


        bool isConEnd         = !isCycleTrim ;

        //Normal Decide Step.
             if (isCycleTrim ) {Trace(sPartName.c_str(),"CycleTrim Stt"); Step.iSeq = scTrim ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default    :                  Trace(sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scTrim: if(CycleTrim()){ Trace(sPartName.c_str(),"CycleTrim End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CTrim::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CTrim::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CTrim::ToStart(void) //스타트를 하기 위한 함수.
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

bool CTrim::ToStop(void) //스탑을 하기 위한 함수.
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

void CTrim::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CTrim::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CTrim::CheckStop()
{
    if(!MT_GetStopInpos(miVS2_X)) return false ;
    if(!MT_GetStopInpos(miVS2_Y)) return false ;
    if(!MT_GetStopInpos(miVS1_X)) return false ;
    if(!MT_GetStopInpos(miVS3_X)) return false ;

    return true ;
}

void CTrim::Load(bool IsLoad , FILE *fp)
{

}

















