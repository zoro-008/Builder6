//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
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
CRail  RL;
const AnsiString sPartName = "Rail " ;

CRail::CRail(void)
{
    Reset();
}

void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CRail::~CRail (void)
{

}

bool CRail::FindChip(int &r , int &c )
{
    return false;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    if(_iMotr == miFRP_T){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piFRP_TWait    : dPos = PM.GetPos   (_iMotr , pvFRP_TWaitPs    ); break ;
            case piFRP_TRail    : dPos = PM.GetPos   (_iMotr , pvFRP_TRailPs    ); break ;
            case piFRP_TTrim    : dPos = PM.GetPos   (_iMotr , pvFRP_TTrimPs    ); break ;
        }
    }

    else if(_iMotr == miIDX_X){
        switch(_iPstnId) {
            default             : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piIDX_XWait    : dPos = PM.GetPos   (_iMotr , pvIDX_XWaitPs    ); break ;
            case piIDX_XPreFrpr : dPos = PM.GetPos   (_iMotr , pvIDX_XPreFrprPs ); break ;
            case piIDX_XFrpr    : dPos = PM.GetPos   (_iMotr , pvIDX_XFrprPs    ); break ;
            case piIDX_XOut     : dPos = PM.GetPos   (_iMotr , pvIDX_XOutPs     ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , PM.GetPos(_iMotr , _iPstnId)) ) return true ;

    if(_iMotr == miFRP_T){
        if(IO_GetX(xRL_2Pkg)) return false ;
    }

    else if(_iMotr == miIDX_X){

    }

    else {
        return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //Ok.
    return true;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_iPstnId == piIDX_XFrpr && MT_CmprPos(_iMotr,piIDX_XPreFrpr)) return MT_GoAbs(_iMotr , dPosition , 5.0 );
    else if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else                 return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRail::CycleHome()
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

        case 10: MoveActr(aiRL_PreBuffStp, ccFwd);
                 MoveActr(aiRL_InspStp1  , ccFwd);
                 MoveActr(aiRL_InspStp2  , ccFwd);
                 MoveActr(aiRL_InspStp3  , ccFwd);
                 MoveActr(aiRL_InspStn1  , ccBwd);
                 MoveActr(aiRL_InspStn2  , ccBwd);
                 MoveActr(aiRL_InspStn3  , ccBwd);
                 MoveActr(aiRL_InspAln1  , ccBwd);
                 MoveActr(aiRL_InspAln2  , ccBwd);
                 MoveActr(aiRL_InspAln3  , ccBwd);
                 MoveActr(aiRL_FlprAln   , ccBwd);
                 MoveActr(aiRL_PstBfGr   , ccFwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiRL_PreBuffStp, ccFwd))return false ;
                 if(!MoveActr(aiRL_InspStp1  , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspStp2  , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspStp3  , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspStn1  , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspStn2  , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspStn3  , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspAln1  , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspAln2  , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspAln3  , ccBwd))return false ;
                 if(!MoveActr(aiRL_FlprAln   , ccBwd))return false ;
                 if(!MoveActr(aiRL_PstBfGr   , ccFwd))return false ;

                 MT_Home(miIDX_X) ;
                 MT_Home(miFRP_T) ;

                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miIDX_X))return false ;
                 if(!MT_GetHomeEnd(miFRP_T))return false ;

                 MoveMotr(miIDX_X,piIDX_XWait );
                 MoveMotr(miFRP_T,piFRP_TWait );

                 Step.iHome++;
                 return false;

        case 13: if(!MoveMotr(miIDX_X,piIDX_XWait ))return false ;
                 if(!MoveMotr(miFRP_T,piFRP_TWait ))return false ;

                 MoveActr(aiRL_PstBfGr   , ccBwd);

                 Step.iHome++;
                 return false ;

        case 14: if(!MoveActr(aiRL_PstBfGr   , ccBwd))return false ;
                 MoveActr(aiRL_InspAln1 , ccFwd);
                 MoveActr(aiRL_InspAln2 , ccFwd);
                 MoveActr(aiRL_InspAln3 , ccFwd);
                 MoveActr(aiRL_FlprAln  , ccFwd);

                 Step.iHome++;
                 return false ;

        case 15: if(!MoveActr(aiRL_InspAln1 , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspAln2 , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspAln3 , ccFwd))return false ;
                 if(!MoveActr(aiRL_FlprAln  , ccFwd))return false ;
                 MoveActr(aiRL_InspAln1 , ccBwd);
                 MoveActr(aiRL_InspAln2 , ccBwd);
                 MoveActr(aiRL_InspAln3 , ccBwd);
                 MoveActr(aiRL_FlprAln  , ccBwd);
                 Step.iHome++;
                 return false ;

        case 16: if(!MoveActr(aiRL_InspAln1 , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspAln2 , ccBwd))return false ;
                 if(!MoveActr(aiRL_InspAln3 , ccBwd))return false ;
                 if(!MoveActr(aiRL_FlprAln  , ccBwd))return false ;
                 MoveActr(aiRL_InspStn1 , ccFwd);
                 MoveActr(aiRL_InspStn2 , ccFwd);
                 MoveActr(aiRL_InspStn3 , ccFwd);
                 Step.iHome++;
                 return false;

        case 17: if(!MoveActr(aiRL_InspStn1 , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspStn2 , ccFwd))return false ;
                 if(!MoveActr(aiRL_InspStn3 , ccFwd))return false ;

                 Step.iHome = 0;
                 return true ;
    }
}



//One Cycle.
bool CRail::CycleOut(void) //Rail끝단에서 장비 밖으로 배출 되는 함수.
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
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRL_PstBfGr , ccFwd);
                  MoveActr(aiID_Finger  , ccBwd);
                  MoveActr(aiRL_FlprAln , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiRL_PstBfGr , ccFwd)) return false ;
                  if(!MoveActr(aiID_Finger  , ccBwd)) return false ;
                  if(!MoveActr(aiRL_FlprAln , ccBwd)) return false ;
                  MoveMotr(miIDX_X , piIDX_XWait) ;
                  MoveMotr(miFRP_T , piFRP_TRail) ;
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  if(!MoveMotr(miFRP_T , piFRP_TRail)) return false ;
                  MoveActr(aiID_Finger  , ccFwd);
                  MoveActr(aiRL_PstBfGr , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiID_Finger  , ccFwd )) return false ;
                  if(!MoveActr(aiRL_PstBfGr , ccBwd )) return false ;
                  MoveMotr(miIDX_X , piIDX_XPreFrpr);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miIDX_X      , piIDX_XPreFrpr)) return false ;
                  MoveMotr(miIDX_X , piIDX_XOut) ;
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveMotr(miIDX_X , piIDX_XOut)) return false ;
                  //다음장비에 데이터 남김.
                  DM.ARAY[aiPostBuff].SetStat(csEmpty) ;
                  MoveMotr(miIDX_X , piIDX_XWait);
                  MoveActr(aiRL_InspStp3 , ccFwd );
                  MoveActr(aiID_Finger   , ccBwd );
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  if(!MoveActr(aiRL_InspStp3 , ccFwd)) return false ;
                  if(!MoveActr(aiID_Finger   , ccBwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}


bool CRail::CycleFlip(void) //Rail끝단에서 트림을 위해 플립을 하는 동작.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveActr(aiRL_PstBfGr , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRL_PstBfGr , ccFwd)) return false ;
                  MoveMotr(miFRP_T , piFRP_TRail);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miFRP_T , piFRP_TRail)) return false ;
                  MoveActr(aiRL_PstBfGr , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiRL_PstBfGr , ccBwd)) return false ;
                  MoveActr(aiRL_FlprAln  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiRL_FlprAln  , ccFwd)) return false ;
                  MoveActr(aiRL_PstBfGr , ccFwd);
                  MoveActr(aiID_Finger  , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiRL_PstBfGr , ccFwd)) return false ;
                  if(!MoveActr(aiID_Finger  , ccBwd)) return false ;
                  MoveMotr(miIDX_X , piIDX_XWait) ;
                  MoveActr(aiRL_FlprAln , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  if(!MoveActr(aiRL_FlprAln, ccBwd  )) return false ;
                  MoveMotr(miFRP_T , piFRP_TRail) ;
                  MoveActr(aiID_Finger  , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  17: if(!MoveMotr(miFRP_T , piFRP_TRail)) return false ;
                  if(!MoveActr(aiID_Finger  , ccBwd )) return false ;
                  MoveMotr(miFRP_T , piFRP_TTrim);
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!MoveMotr(miFRP_T , piFRP_TTrim)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

/*
bool CRail::CycleReFlip(void) //Rail끝단으로 플립한다.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRL_PstBfGr , ccFwd);
                  MoveActr(aiID_Finger  , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiRL_PstBfGr , ccFwd)) return false ;
                  if(!MoveActr(aiID_Finger  , ccBwd)) return false ;
                  MoveMotr(miIDX_X , piIDX_XWait) ;
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  MoveMotr(miFRP_T , piFRP_TRail) ;
                  MoveActr(aiID_Finger  , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveMotr(miFRP_T , piFRP_TRail)) return false ;
                  if(!MoveActr(aiID_Finger  , ccFwd )) return false ;
                  MoveActr(aiRL_PstBfGr , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiRL_PstBfGr , ccBwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}*/

bool CRail::CycleInsp3Out(void) //Insp3Zone에서 PostBuffer로 자제 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRL_PstBfGr  , ccBwd);
                  MoveActr(aiRL_FlprAln  , ccBwd);
                  MoveActr(aiID_Finger   , ccBwd);
                  MoveMotr(miFRP_T , piFRP_TRail);
                  MoveActr(aiRL_InspStn3 , ccBwd);
                  MoveActr(aiRL_FlprAln  , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiRL_PstBfGr , ccBwd )) return false ;
                  if(!MoveActr(aiRL_FlprAln , ccBwd )) return false ;
                  if(!MoveActr(aiID_Finger  , ccBwd )) return false ;
                  if(!MoveMotr(miFRP_T , piFRP_TRail)) return false ;
                  if(!MoveActr(aiRL_InspStn3, ccBwd )) return false ;
                  if(!MoveActr(aiRL_FlprAln , ccBwd )) return false ;
                  MoveMotr(miIDX_X , piIDX_XWait) ;
                  IO_SetY(yRL_InspBelt,true) ;
                  Step.iCycle++;
                  return false ;

        case  12: if(!IO_GetX(xRL_2Pkg) && !OM.CmnOptn.bDryRun) return false ; //!OM.CmnOptn.bDryRun) return false ; // m_tmDryRun.OnDelay(OM.CmnOptn.bDryRun, 2000)) return false ;

                  Step.iCycle++;
                  return false ;

        case  13:
                  Step.iCycle++;
                  return false ;

        case  14: if(IO_GetX(xRL_2Pkg) && !OM.CmnOptn.bDryRun) return false ;
                  if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  IO_SetY(yRL_InspBelt,false);
                  MoveActr(aiID_Finger,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiID_Finger,ccFwd)) return false ;
                  MoveMotr(miIDX_X , piIDX_XPreFrpr);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miIDX_X , piIDX_XPreFrpr))return false ;
                  MoveMotr(miIDX_X , piIDX_XFrpr);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miIDX_X , piIDX_XFrpr))return false ;
                  DM.ShiftArrayData(aiInsp3,aiPostBuff);
                  DM.ARAY[aiPostBuff].FlipY();

                  MoveMotr(miIDX_X , piIDX_XPreFrpr);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miIDX_X , piIDX_XPreFrpr))return false ;
                  MoveActr(aiID_Finger  , ccBwd);
                  MoveActr(aiRL_FlprAln , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!MoveActr(aiID_Finger  , ccBwd)) return false ;
                  if(!MoveActr(aiRL_FlprAln , ccFwd)) return false ;
                  MoveMotr(miIDX_X , piIDX_XWait) ;
                  MoveActr(aiRL_InspStp3, ccFwd ) ;
                  Step.iCycle++ ;
                  return false ;

        case  20: if(!MoveMotr(miIDX_X , piIDX_XWait)) return false ;
                  if(!MoveActr(aiRL_InspStp3 , ccFwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CRail::CycleInsp2Out(void) //Insp3Zone에서 PostBuffer로 자제 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiRL_InspStp3 , ccFwd);
                  MoveActr(aiRL_InspStp2 , ccBwd);
                  MoveActr(aiRL_InspStn2 , ccBwd);
                  MoveActr(aiRL_InspAln3 , ccBwd);
                  Step.iCycle ++ ;
                  return false ;

        case 11 : if(!MoveActr(aiRL_InspStp3 , ccFwd)) return false ;
                  if(!MoveActr(aiRL_InspStp2 , ccBwd)) return false ;
                  if(!MoveActr(aiRL_InspStn2 , ccBwd)) return false ;
                  if(!MoveActr(aiRL_InspAln3 , ccBwd)) return false ;
                  IO_SetY(yRL_InspBelt , true) ;
                  Step.iCycle ++ ;
                  return false ;

        case 12 : if(!IO_GetX(xRL_3InspPkg) && !OM.CmnOptn.bDryRun) return false ;
                  DM.ShiftArrayData(aiInsp2 , aiInsp3) ;
                  DM.ARAY[aiInsp3].ChangeStat(csGood , csUnkwn) ;
                  IO_SetY (yRL_InspBelt   , false) ;
                  MoveActr(aiRL_InspAln3  , ccFwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 13 : if(!MoveActr(aiRL_InspAln3  , ccFwd)) return false ;
                  MoveActr(aiRL_InspAln3  , ccBwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 14 : if(!MoveActr(aiRL_InspAln3  , ccBwd)) return false ;
                  MoveActr(aiRL_InspStn3 , ccFwd) ;
                  MoveActr(aiRL_InspStp2 , ccFwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 15 : if(!MoveActr(aiRL_InspStn3 , ccFwd)) return false ;
                  if(!MoveActr(aiRL_InspStp2 , ccFwd)) return false ;
                  Step.iCycle = 0 ;
                  return false ;
    }
}

bool CRail::CycleInsp1Out(void) //Insp3Zone에서 PostBuffer로 자제 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiRL_InspStp2 , ccFwd);
                  MoveActr(aiRL_InspStp1 , ccBwd);
                  MoveActr(aiRL_InspStn1 , ccBwd);
                  MoveActr(aiRL_InspAln2 , ccBwd);
                  Step.iCycle ++ ;
                  return false ;

        case 11 : if(!MoveActr(aiRL_InspStp2 , ccFwd)) return false ;
                  if(!MoveActr(aiRL_InspStp1 , ccBwd)) return false ;
                  if(!MoveActr(aiRL_InspStn1 , ccBwd)) return false ;
                  if(!MoveActr(aiRL_InspAln2 , ccBwd)) return false ;
                  IO_SetY(yRL_InspBelt , true) ;
                  Step.iCycle ++ ;
                  return false ;

        case 12 : if(!IO_GetX(xRL_2InspPkg) && !OM.CmnOptn.bDryRun) return false ;
                  DM.ShiftArrayData(aiInsp1 , aiInsp2) ;
                  DM.ARAY[aiInsp2].ChangeStat(csGood , csUnkwn) ;
                  IO_SetY (yRL_InspBelt  , false) ;
                  MoveActr(aiRL_InspAln2  , ccFwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 13 : if(!MoveActr(aiRL_InspAln2  , ccFwd)) return false ;
                  MoveActr(aiRL_InspAln2  , ccBwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 14 : if(!MoveActr(aiRL_InspAln2  , ccBwd)) return false ;
                  MoveActr(aiRL_InspStn2 , ccFwd) ;
                  MoveActr(aiRL_InspStp1 , ccFwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 15 : if(!MoveActr(aiRL_InspStn2 , ccFwd)) return false ;
                  if(!MoveActr(aiRL_InspStp1 , ccFwd)) return false ;
                  Step.iCycle = 0 ;
                  return false ;
    }
}
bool CRail::CyclePreBuffOut(void) //Insp3Zone에서 PostBuffer로 자제 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_InspBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiRL_InspStp1   , ccFwd);
                  MoveActr(aiRL_PreBuffStp , ccBwd);
                  MoveActr(aiRL_InspStn1   , ccBwd);
                  MoveActr(aiRL_InspAln1   , ccBwd);
                  Step.iCycle ++ ;
                  return false ;

        case 11 : if(!MoveActr(aiRL_InspStp1   , ccFwd)) return false ;
                  if(!MoveActr(aiRL_PreBuffStp , ccBwd)) return false ;
                  if(!MoveActr(aiRL_InspStn1   , ccBwd)) return false ;
                  if(!MoveActr(aiRL_InspAln1   , ccBwd)) return false ;
                  IO_SetY(yRL_PreBufBelt , true) ;
                  IO_SetY(yRL_InspBelt   , true) ;

                  Step.iCycle ++ ;
                  return false ;

        case 12 : if(!IO_GetX(xRL_1InspPkg) && !OM.CmnOptn.bDryRun) return false ;
                  DM.ShiftArrayData(aiPreBuff , aiInsp1) ;
                  DM.ARAY[aiInsp1].ChangeStat(csGood , csUnkwn) ;
                  IO_SetY(yRL_PreBufBelt , false) ;
                  IO_SetY(yRL_InspBelt   , false) ;
                  MoveActr(aiRL_InspAln1  , ccFwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 13 : if(!MoveActr(aiRL_InspAln1  , ccFwd)) return false ;
                  MoveActr(aiRL_InspAln1  , ccBwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 14 : if(!MoveActr(aiRL_InspAln1  , ccBwd)) return false ;
                  MoveActr(aiRL_InspStn1   , ccFwd) ;
                  MoveActr(aiRL_PreBuffStp , ccFwd) ;
                  Step.iCycle ++ ;
                  return false ;

        case 15 : if(!MoveActr(aiRL_InspStn1   , ccFwd)) return false ;
                  if(!MoveActr(aiRL_PreBuffStp , ccFwd)) return false ;
                  Step.iCycle = 0 ;
                  return false ;
    }
}

bool CRail::CycleIn(void) //스트립을 PreBuff로 이송.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop(), 5000 )) {
        EM_SetErr(eiRL_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //IO_SetY(yRL_PreBufBelt,false);
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  Trace(sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiRL_PreBuffStp  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiRL_PreBuffStp  , ccFwd)) return false ;
                  IO_SetY(yRL_PreBufBelt , true) ;
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xRL_PreBufPkg)&&!OM.CmnOptn.bDryRun) return false ;

                  //여기서 전장비에서 데이터 가저오는 루틴.
                  //DM.ARAY[aiPreBuff].ChangeStat(csGood , csUnkwn) ;
                  DM.ARAY[aiPreBuff].ChangeStat(csGood , csUnkwn) ;
                  IO_SetY(yRL_PreBufBelt , false) ;
                  Step.iCycle = 0;
                  return true ;
    }
}


bool CRail::Update(void) //오토런닝시에 계속 타는 함수.
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

        //모르는 스트립에러.
        if( DM.ARAY[aiPreBuff ].CheckExist(0,0) &&  IO_GetX(xRL_PreBufPkg)                                                       ) EM_SetErr(eiRL_PreBuffUnknown ) ;
        if( DM.ARAY[aiInsp1   ].CheckExist(0,0) &&  IO_GetX(xRL_1InspPkg )                                                       ) EM_SetErr(eiRL_Insp1Unknown   ) ;
        if( DM.ARAY[aiInsp2   ].CheckExist(0,0) &&  IO_GetX(xRL_2Pkg     )                                                       ) EM_SetErr(eiRL_Insp2Unknown   ) ;
        if( DM.ARAY[aiInsp3   ].CheckExist(0,0) &&  IO_GetX(xRL_3Pkg     )                                                       ) EM_SetErr(eiRL_Insp3Unknown   ) ;
        if( DM.ARAY[aiPostBuff].CheckExist(0,0) &&  IO_GetX(xRL_3Pkg     ) && MT_CmprPos(miFRP_T,GetMotrPos(miFRP_T,piFRP_TRail))) EM_SetErr(eiRL_PostBuffUnknown) ;

        //스트립 사라짐 에러.
        if(!DM.ARAY[aiPreBuff ].CheckExist(0,0) && !IO_GetX(xRL_PreBufPkg)                                                       ) EM_SetErr(eiRL_PreBuffDispr   ) ;
        if(!DM.ARAY[aiInsp1   ].CheckExist(0,0) && !IO_GetX(xRL_1InspPkg )                                                       ) EM_SetErr(eiRL_Insp1Dispr     ) ;
        if(!DM.ARAY[aiInsp2   ].CheckExist(0,0) && !IO_GetX(xRL_2Pkg     )                                                       ) EM_SetErr(eiRL_Insp2Dispr     ) ;
        if(!DM.ARAY[aiInsp3   ].CheckExist(0,0) && !IO_GetX(xRL_3Pkg     )                                                       ) EM_SetErr(eiRL_Insp3Dispr     ) ;
        if(!DM.ARAY[aiPostBuff].CheckExist(0,0) && !IO_GetX(xRL_3Pkg     ) && MT_CmprPos(miFRP_T,GetMotrPos(miFRP_T,piFRP_TRail))) EM_SetErr(eiRL_PostBuffDispr  ) ;

        if(EM_IsErr()) return false ;

        bool isExistPstBfFail  =  DM.ARAY[aiPostBuff].GetCntStat  (csFail ) || DM.ARAY[aiPostBuff].GetCntStat(csRslt0) ||
                                  DM.ARAY[aiPostBuff].GetCntStat  (csRslt1) || DM.ARAY[aiPostBuff].GetCntStat(csRslt2) ;
        bool isExistPreBuff    =  DM.ARAY[aiInsp3   ].GetCntExist (       ) ;
        bool isExistUnInsp1    =  DM.ARAY[aiInsp1   ].GetCntStat  (csUnkwn) ;
        bool isExistUnInsp2    =  DM.ARAY[aiInsp2   ].GetCntStat  (csUnkwn) ;
        bool isExistUnInsp3    =  DM.ARAY[aiInsp3   ].GetCntStat  (csUnkwn) ;
        bool isExistStrpOnRail =  DM.ARAY[aiPostBuff].CheckAllStat(csEmpty) ||
                                  DM.ARAY[aiInsp1   ].CheckAllStat(csEmpty) ||
                                  DM.ARAY[aiInsp2   ].CheckAllStat(csEmpty) ||
                                  DM.ARAY[aiInsp3   ].CheckAllStat(csEmpty) ||
                                  DM.ARAY[aiPreBuff ].CheckAllStat(csEmpty) ;
        bool isDoingInsp       = (DM.ARAY[aiInsp1   ].GetCntStat  (csUnkwn) && (DM.ARAY[aiInsp1].GetCntStat (csWork)||DM.ARAY[aiInsp1].GetCntStat (csRslt0))) ||
                                 (DM.ARAY[aiInsp2   ].GetCntStat  (csUnkwn) && (DM.ARAY[aiInsp2].GetCntStat (csWork)||DM.ARAY[aiInsp2].GetCntStat (csRslt1))) ||
                                 (DM.ARAY[aiInsp3   ].GetCntStat  (csUnkwn) && (DM.ARAY[aiInsp3].GetCntStat (csWork)||DM.ARAY[aiInsp3].GetCntStat (csRslt2))) ;


        bool isCycleOut        = !isExistPstBfFail &&  DM.ARAY[aiPostBuff].GetCntExist() && MT_CmprPos(miFRP_T,GetMotrPos(miFRP_T,piFRP_TRail)) ;
        bool isCycleFlip       =  isExistPstBfFail && !MT_CmprPos(miFRP_T,GetMotrPos(miFRP_T,piFRP_TTrim)) ;
        bool isCycleInsp3Out   = !isExistUnInsp3   &&  DM.ARAY[aiInsp3   ].GetCntExist() && !DM.ARAY[aiPostBuff].GetCntExist();
        bool isCycleInsp2Out   = !isExistUnInsp2   &&  DM.ARAY[aiInsp2   ].GetCntExist() && !DM.ARAY[aiInsp3   ].GetCntExist();
        bool isCycleInsp1Out   = !isExistUnInsp1   &&  DM.ARAY[aiInsp1   ].GetCntExist() && !DM.ARAY[aiInsp2   ].GetCntExist();
        bool isCyclePreBuffOut =  isExistPreBuff   && !isDoingInsp                       && !DM.ARAY[aiInsp1   ].GetCntExist();
        bool isCycleIn         =  IO_GetX(xRL_1Pkg) ;
        bool isConEnd          = !OM.EqpOptn.bExistOnLoader && !isExistStrpOnRail ;

        //Normal Decide Step.
             if (isCycleOut       ) {Trace(sPartName.c_str(),"CycleOut        Stt"); Step.iSeq = scOut        ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleFlip      ) {Trace(sPartName.c_str(),"CycleFlip       Stt"); Step.iSeq = scFlip       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleInsp3Out  ) {Trace(sPartName.c_str(),"CycleInsp3Out   Stt"); Step.iSeq = scInsp3Out   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleInsp2Out  ) {Trace(sPartName.c_str(),"CycleInsp2Out   Stt"); Step.iSeq = scInsp2Out   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleInsp1Out  ) {Trace(sPartName.c_str(),"CycleInsp1Out   Stt"); Step.iSeq = scInsp1Out   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePreBuffOut) {Trace(sPartName.c_str(),"CyclePreBuffOut Stt"); Step.iSeq = scPreBuffOut ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn        ) {Trace(sPartName.c_str(),"CycleIn         Stt"); Step.iSeq = scIn         ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd         ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default           :                         Trace(sPartName.c_str(),"default         End");Step.iSeq = scIdle ;  return false ;
        case  scOut       : if(CycleOut        ()){ Trace(sPartName.c_str(),"CyclePbWkToWkOt End");Step.iSeq = scIdle ;} return false ;
        case  scFlip      : if(CycleFlip       ()){ Trace(sPartName.c_str(),"CycleRdWk       End");Step.iSeq = scIdle ;} return false ;
        case  scInsp3Out  : if(CycleInsp3Out   ()){ Trace(sPartName.c_str(),"CycleWkToOt     End");Step.iSeq = scIdle ;} return false ;
        case  scInsp2Out  : if(CycleInsp2Out   ()){ Trace(sPartName.c_str(),"CycleWkToOb     End");Step.iSeq = scIdle ;} return false ;
        case  scInsp1Out  : if(CycleInsp1Out   ()){ Trace(sPartName.c_str(),"CycleObToOt     End");Step.iSeq = scIdle ;} return false ;
        case  scPreBuffOut: if(CyclePreBuffOut ()){ Trace(sPartName.c_str(),"CycleWkToBf2    End");Step.iSeq = scIdle ;} return false ;
        case  scIn        : if(CycleIn         ()){ Trace(sPartName.c_str(),"CycleMask       End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CRail::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //Ok.
    return true;
}

bool CRail::CycleToStart(void) //스타트를 하기 위한 함수.
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

bool CRail::CycleToStop(void) //스탑을 하기 위한 함수.
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

        case 10: IO_SetY(yRL_PreBufBelt,false);
                 IO_SetY(yRL_InspBelt  ,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

void CRail::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CRail::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CRail::CheckStop()
{
    if(!MT_GetStopInpos(miIDX_X)) return false ;
    if(!MT_GetStopInpos(miFRP_T)) return false ;

    if(!AT_Done(aiRL_InspAln1  )) return false ;
    if(!AT_Done(aiRL_InspAln2  )) return false ;
    if(!AT_Done(aiRL_InspAln3  )) return false ;
    if(!AT_Done(aiRL_FlprAln   )) return false ;
    if(!AT_Done(aiRL_InspStn1  )) return false ;
    if(!AT_Done(aiRL_InspStn2  )) return false ;
    if(!AT_Done(aiRL_InspStn3  )) return false ;
    if(!AT_Done(aiID_Finger    )) return false ;
    if(!AT_Done(aiRL_PstBfGr   )) return false ;
    if(!AT_Done(aiRJ_Sorter    )) return false ;
    if(!AT_Done(aiRJ_Stopper   )) return false ;
    if(!AT_Done(aiRL_PreBuffStp)) return false ;
    if(!AT_Done(aiRL_InspStp1  )) return false ;
    if(!AT_Done(aiRL_InspStp2  )) return false ;
    if(!AT_Done(aiRL_InspStp3  )) return false ;

    return true ;
}

void CRail::Load(bool IsLoad , FILE *fp)
{

}

















