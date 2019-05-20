//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PreBuff.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CPreBuff  PRB;
const AnsiString sPartName = "PreBuff " ;

CPreBuff::CPreBuff(void)
{
    Reset();
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

CPreBuff::~CPreBuff (void)
{

}

bool CPreBuff::FindChip(int &r , int &c )
{
    return false;
}

double CPreBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    dPos = MT_GetCmdPos(_iMotr);

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPreBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    return true ;
}

//---------------------------------------------------------------------------
bool  CPreBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    //Ok.
    return true;
}

bool CPreBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    return false ;
}

bool CPreBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPreBuff::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_HomeTO);
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

        case 10: MoveActr(aiPRB_Stopper, ccFwd);
                 IO_SetY(yPRB_Belt,false) ;
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiPRB_Stopper, ccFwd))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPreBuff::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    //Clear Timer.
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    //Ok.
    return true;

}

bool CPreBuff::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    //Ok.
    return true;
}

bool CPreBuff::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPRB_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
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

bool CPreBuff::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPRB_ToStopTO);

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

        case 10: IO_SetY(yPRB_Belt,false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPreBuff::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
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

        bool isCycleIn  = (IO_GetX(xPRB_PkgIn1) ||  IO_GetX(xPRB_PkgIn2)) ||
                          (OM.CmnOptn.bDryRun   && !DM.ARAY[riPRB].GetCntExist()) ;
        bool isCycleOut = !DM.ARAY[riWK1].GetCntExist() && AT_Complete(aiPRB_Stopper , ccFwd) ;
        bool isConEnd   = !isCycleIn && !isCycleOut ;

        //�𸣴� ��Ʈ������.
        if(!DM.ARAY[riPRB ].CheckExist(0,0) &&  IO_GetX(xPRB_Pkg) ) EM_SetErr(eiPRB_PkgUnknown ) ;

        //��Ʈ�� ����� ����.
        if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) EM_SetErr(eiPRB_PkgDispr   ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn  ) {Trace(sPartName.c_str(),"CycleIn  Stt"); Step.iSeq = scIn  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut ) {Trace(sPartName.c_str(),"CycleOut Stt"); Step.iSeq = scOut ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default     :                  Trace(sPartName.c_str(),"default         End");Step.iSeq = scIdle ;  return false ;
        case  scIn  : if(CycleIn  ()){ Trace(sPartName.c_str(),"CycleMask       End");Step.iSeq = scIdle ;} return false ;
        case  scOut : if(CycleOut ()){ Trace(sPartName.c_str(),"CyclePbWkToWkOt End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CPreBuff::CycleIn(void) //��Ʈ���� PreBuff�� �̼�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case 10 : IO_SetY(yPRB_Belt , true) ;
                  MoveActr(aiPRB_Stopper  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPRB_Stopper  , ccFwd)) return false ;
                  Step.iCycle++;
                  return false ;

        case 12 : if(!IO_GetX(xPRB_Pkg)&&!OM.CmnOptn.bDryRun) return false ;

                  //���⼭ ����񿡼� ������ �������� ��ƾ.
                  //DM.ARAY[riPreBuff].ChangeStat(csGood , csUnkwn) ;

                  DM.ARAY[riPRB].ChangeStat(csGood , csUnkwn) ;
                  IO_SetY(yPRB_Belt , false) ;
                  Step.iCycle = 0;
                  return true ;
    }
}

//One Cycle.
bool CPreBuff::CycleOut(void) //�������ۿ��� ��ũ������ �о����.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPRB_CycleTO);
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

        case  10: MoveActr(aiPRB_Stopper , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiPRB_Stopper , ccBwd)) return false ;
                  IO_SetY(yPRB_Belt , true);
                  IO_SetY(yWRK_Belt , true);

                  DM.ShiftArrayData(riPRB , riWK1) ;

                  Step.iCycle++ ;
                  return false ;

        case  12: IO_SetY(yPRB_Belt , true);
                  IO_SetY(yWRK_Belt , true);
                  if(!IO_GetXDn(xPRB_Pkg)) MoveActr(aiPRB_Stopper , ccFwd) ;
                  if(!IO_GetX  (xWK1_Pkg)) return false ;
                  IO_SetY(yPRB_Belt , false);
                  IO_SetY(yWRK_Belt , false);

                  MoveActr(aiPRB_Stopper , ccFwd);
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPRB_Stopper , ccFwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CPreBuff::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CPreBuff::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CPreBuff::CheckStop()
{
    if(!AT_Done(aiPRB_Stopper )) return false ;

    return true ;
}

void CPreBuff::Load(bool IsLoad , FILE *fp)
{

}

















