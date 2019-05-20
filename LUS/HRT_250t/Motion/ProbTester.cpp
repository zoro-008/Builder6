//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ProbTester.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
CProbeTester PTR;
CProbeTester::CProbeTester()
{
    m_bProbeEnd = false ;
    m_iProbeRslt = prFail ;

    m_bRslt = false ;
//    m_bRslt = false ;

    m_iStep = 0 ;

    m_sPartName = "ProbeTester " ;

    IO_SetY(yTST_StartTest , true); //비접으로 쓰는듯.





}

CProbeTester::~CProbeTester()
{

}

void CProbeTester::Update()
{
    if( m_iProbeCmd == pcNone) return ;

    else if( m_iProbeCmd == pcProbe) {
        if(CycleProbe()) m_iProbeCmd = pcNone ;
    }
    else if( m_iProbeCmd == pcReset) {
        if(CycleReset()) m_iProbeCmd = pcNone ;
    }

}

bool CProbeTester::CycleProbe() //이거 나중에 확인 해야함... 사양서의 예제 좀 이상함.   스타트 3번 날리게 되어 있음. 레디도 시점이 이상함.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    static int iPreStep = 0 ;
    if (m_tmCycle.OnDelay(m_iStep && m_iStep == iPreStep && !OM.MstOptn.bDebugMode, 1000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : m_iStep=%02d" , __FUNC__ , m_iStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        m_iProbeRslt = prTimeOut ;
        m_iStep = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s m_iStep=%02d" , __FUNC__ , m_iStep );
    if(m_iStep != iPreStep) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    iPreStep = m_iStep ;

    //if(Stat.bReqStop) {
    //    m_iStep = 0;
    //    return true;
    //}

    //Cycle.
    switch (m_iStep) {
        default : if(m_iStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iStep=%02d" , __FUNC__ , m_iStep );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  m_iProbeRslt = prTimeOut ;
                  m_iStep = 0 ;
                  return true ;

        case  10: //if(!IO_GetX(xTST_Ready)) {
                  //    EM_SetErr(eiPTR_PorbeNotReady);
                  //    m_iStep = 0;
                  //    return true;
                  //}
                  m_iProbeRslt = prFail ;
//                  m_bFwdRslt = false ;
//                  m_bBwdRslt = false ;
                  m_bRslt = false ;

                  //첫번째 정방향.
                  IO_SetY(yTST_StartTest,false); //비접으로 쓰는듯.
                  m_tmTemp.Clear();

                  m_iStep++;
                  return false;

        case  11: if(!m_tmTemp.OnDelay(true , 5)) return false ; //Start 시그널 2ms 이상 유지해야함.
                  IO_SetY(yTST_StartTest , true) ;
                  m_tmTemp.Clear();
                  m_iStep++;
                  return false ;

        case  12: if(m_tmTemp.OnDelay(true , OM.DevOptn.iPrbContactTime)) {
                      m_iProbeRslt = prTimeOut ;
                      m_iStep++;
                      return false ;
                  }
                  if(!IO_GetXUp(xTST_End) && !IO_GetXDn(xTST_End) && !IO_GetX(xTST_End))return false ;

                  //JS 잠시 주석..

//                  if(!IO_GetX(xTST_Forward) && IO_GetX(xTST_Backward)) {
                      m_bRslt = IO_GetX(xTST_Result) ;
//                  }
//                  else {
//                      m_iProbeRslt = prError ;
//                      m_iStep = 0 ;
//                      return true ;
//                  }
//
//                  //두번째 역방향.
//                  IO_SetY(yTST_StartTest,false); //비접으로 쓰는듯.
//                  m_tmTemp.Clear();
//
//                  m_iStep++;
//                  return false;
//
//        case  13: if(!m_tmTemp.OnDelay(true , 2)) return false ; //Start 시그널 2ms 이상 유지해야함.
//                  IO_SetY(yTST_StartTest , true) ;
//                  m_iStep++;
//                  return false ;
//
//        case  14: if(IO_GetX(xTST_EndTest))return false ;
//
//                  if( IO_GetX(xTST_Forward) && !IO_GetX(xTST_Backward)) {
//                      m_bBwdRslt = !IO_GetX(xTST_Result) ;
//                  }
//                  else {
//                      m_iProbeRslt = prError ;
//                      m_iStep = 0 ;
//                      return true ;
//                  }
//
//                  //두번째 역방향.
//                  IO_SetY(yTST_StartTest,false); //비접으로 쓰는듯.
//                  m_tmTemp.Clear();
//
//                  m_iStep++;
//                  return false ;
//
//        case  15: if(!m_tmTemp.OnDelay(true , 2)) return false ; //Start 시그널 2ms 이상 유지해야함.
//                  IO_SetY(yTST_StartTest , true) ;

                  if (m_bRslt) m_iProbeRslt = prGood ;
//                  else if(m_bBwdRslt) m_iProbeRslt = prBwdGood ;
//                  else if(m_bBwdRslt) m_iProbeRslt = prBwdGood ;
                  else                m_iProbeRslt = prFail    ;

                  m_iStep++;
                  return false ;

        case  13: //if(!IO_GetX(xTST_Backward))return false ;
                  //if(!IO_GetX(xTST_Forward ))return false ;
//                  if(!IO_GetX(xTST_Ready   ))return false ;
                  m_iStep = 0 ;
                  return true ;


    }

}

bool CProbeTester::CycleReset() //나중에 리셑하는 방법 문의 해서 구현 하기.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    static int iPreStep = 0 ;
    if (m_tmCycle.OnDelay(m_iStep && m_iStep == iPreStep && !OM.MstOptn.bDebugMode, 1000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : m_iStep=%02d" , __FUNC__ , m_iStep );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        m_iProbeRslt = prTimeOut ;
        m_iStep = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s m_iStep=%02d" , __FUNC__ , m_iStep );
    if(m_iStep != iPreStep) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    iPreStep = m_iStep ;

    //if(Stat.bReqStop) {
    //    m_iStep = 0;
    //    return true;
    //}

    //Cycle.
    switch (m_iStep) {
        default : if(m_iStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iStep=%02d" , __FUNC__ , m_iStep );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  m_iProbeRslt = prTimeOut ;
                  m_iStep = 0 ;
                  return true ;

        case  10:
                  m_iStep=0;
                  return true ;
    }

}

void CProbeTester::StartCycle(EN_PROBE_CMD _iCmd)
{
    m_iProbeCmd  = _iCmd  ;
    m_bProbeEnd  = false  ;
    m_iProbeRslt = prFail ;
    m_iStep      = 10     ;
}

bool CProbeTester::GetCycleEnd() //싸이클 끝났는지 확인.
{
    return m_iProbeCmd == pcNone ;

}


EN_PROBE_RSLT CProbeTester::GetProbeRslt()
{
    return m_iProbeRslt ;
}
