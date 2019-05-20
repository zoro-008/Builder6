//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoader.h"
#include "Rail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoader ULD;

CUnLoader::CUnLoader(void)
{
    Init();

}
CUnLoader::~CUnLoader (void)
{
    Close();
}

void CUnLoader::Init()
{
    m_sPartName = "UnLoader " ;
    Reset();
}

void CUnLoader::Close()
{

}

void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

    //Bottom.
    m_tmBCycle .Clear();
    m_tmBTemp  .Clear();
}



bool CUnLoader::FindChip(EN_ARAY_ID _riWhre , int &r , int &c )
{
    return false ;
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    return dPos ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
    bRet = false ;

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return false ;

}

//---------------------------------------------------------------------------
bool  CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiULD_Stck){
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

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CUnLoader::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_HomeTO);
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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MoveActr(aiULD_Stck  ,ccBwd);
                 IO_SetY(yRAL_FeedingMt,false);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiULD_Stck  ,ccBwd)) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CUnLoader::GetLastStrip()
{
    return false ;
}

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiULD_ToStartTO);

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


        case 10: MoveActr(aiULD_Stck  ,ccBwd);
                 IO_SetY(yRAL_FeedingMt,false);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiULD_Stck  ,ccBwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr(eiULD_ToStopTO);

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

        case 10: MoveActr(aiULD_Stck  ,ccBwd);
                 IO_SetY(yRAL_FeedingMt,false);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveActr(aiULD_Stck  ,ccBwd)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop && RAL.GetSeqStep() == CRail::scIdle)return false ;

        bool isCycleIn    =!DM.ARAY[riWKE].CheckAllStat(csNone) && IO_GetXDn(xRAL_Detect4);
        bool isCycleStock =!DM.ARAY[riPSB].CheckAllStat(csNone) ;
        bool isConEnd     =!IO_GetX(xRAL_Detect4) && DM.ARAY[riPSB].CheckAllStat(csNone);

        //모르는 스트립에러.
        if( DM.ARAY[riPSB].CheckAllStat(csNone) && IO_GetX(xRAL_Detect5 )) EM_SetErr(eiPSB_Unknwn) ;
        //카세트 사라짐.
        if(!DM.ARAY[riPSB].CheckAllStat(csNone) &&!IO_GetX(xRAL_Detect5 )) EM_SetErr(eiPSB_Dispr ) ;

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleIn   ) {Trace(m_sPartName.c_str(),"CycleIn    Stt"); Step.iSeq = scIn    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleStock) {Trace(m_sPartName.c_str(),"CycleStock Stt"); Step.iSeq = scStock ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                      Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                        return false ;
        case scIn    : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn    End");Step.iSeq = scIdle ;} return false ;
        case scStock : if(CycleStock   ()){ Trace(m_sPartName.c_str(),"CycleStock End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.
bool CUnLoader::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 7000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yRAL_FeedingMt,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(m_tmTemp.OnDelay(true , 6000)){
                      EM_SetErr(eiULD_TrayFeed) ;
                      DM.ShiftArrayData(riWKE , riPSB);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!IO_GetX(xRAL_Detect5))return false ;
                  DM.ShiftArrayData(riWKE , riPSB);



                  IO_SetY(yRAL_FeedingMt,false);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CycleStock()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yRAL_FeedingMt , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!m_tmTemp.OnDelay(true , 200)) return false ;
                  IO_SetY(yRAL_FeedingMt , false);
                  MoveActr(aiULD_Stck , ccFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiULD_Stck , ccFwd))return false ;
                  MoveActr(aiULD_Stck , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiULD_Stck , ccBwd)) return false ;
                  //Lot Day Info Add
                  LT.AddDayInfoWorkStrp(1);
                  DM.ARAY[riPSB].SetStat(csNone);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CUnLoader::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CUnLoader::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CUnLoader::CheckStop()
{
    if(!AT_Done(aiULD_Stck )) return false ;

    return true ;
}

void CUnLoader::Load(bool IsLoad)
{

}
//---------------------------------------------------------------------------
