//---------------------------------------------------------------------------
#include <VCL.H>
#include <winbase.h>
#include <stdio.h>
#pragma hdrstop

#include "PaixMotion.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "SLogUnit.h"

#include "global.h"

#define FIRMVER_20050727 4

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------


CPaixMotion *pPaix;

__fastcall CPaixMotion::CPaixMotion(void):TThread(TRUE)
{
    Init();
}

__fastcall CPaixMotion::~CPaixMotion(void)
{
    SetServoOn(Axis1 , 0);
    SetServoOn(Axis2 , 0);
    //pPaix->SetServoOnOff   (Axis1,1    );
    //pPaix->SetMotorOnOff   (Axis2,0    );

    nmc_CloseDevice(IP);
    pPaix->Terminate();
    //pPaix->WaitFor(); //미확인 지움. 종료시 자꾸 걸림.
}
void CPaixMotion::Init(void)
{
    AnsiString sTemp = IP;
    m_nMotionMode[0] = 1;
    m_nMotionMode[1] = 1;
    Stat[1].bHomeDone = false;

    iORCnt = 0;

    memset (&Stat         , 0, (sizeof(TStat)*MAX_MOTR));

    pThreadCallBack = NULL ;

    //192.168.0.99  피씨 모션 랜카드 아이피.
    //192.168.1.99  피씨 모션 랜카드 아이피.

    m_bRemote = nmc_OpenDevice(m_nPaixCtlNo);
    if(!SetPaixCtlNo(IP))
    {
        //ShowMessage("Motion Connection Failed [192.168.0." + sTemp + "]");
        Trace("Error","Motion Connetion Failed");
    }
    else {
        m_bConnect = true;
    }
    Resume();

}

//---------------------------------------------------------------------------
bool CPaixMotion::PaixInit(int nPaixCtlNo)
{
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetPaixCtlNo(int nPaixCtlNo)
{
    AnsiString sTemp = IP;
    if( nPaixCtlNo < 0 || nPaixCtlNo > 255 )
    {
        ClosePaixCtl();
        m_bRemote = false;
        Sleep(10);
        return false;
    }
    m_nPaixCtlNo = nPaixCtlNo;
    SetIpAddress(192, 168, 0);
    //if(!m_bRemote) { ShowMessage("Motion Connection Ping Test Failed [192.168.0." + sTemp + "]"); return m_bRemote;}
    //nmc_set_event(m_nPaixCtlNo, true);
    m_bRemote = nmc_PingCheck(nPaixCtlNo , 50);
    m_bRemote = nmc_OpenDevice(nPaixCtlNo);
    if(m_bRemote != 0) {
        ShowMessage("Motion Connection Ping Test Failed [192.168.0." + sTemp + "]");
        Trace("Error","Motion Connetion Failed");
        return m_bRemote;
    }
    else {m_bNotConnect = false; m_bRemote = true;}

    return m_bRemote;
}
//---------------------------------------------------------------------------
bool CPaixMotion::OpenPaixCtl(int nPaixCtlNo)
{
    if( nPaixCtlNo < 1)
        return false;

    if( m_nPaixCtlNo )
    {
        return SetPaixCtlNo(nPaixCtlNo);
    }
    return false;
}
//---------------------------------------------------------------------------
bool CPaixMotion::ClosePaixCtl(void)
{
    if( m_nPaixCtlNo )
    {
        nmc_CloseDevice(m_nPaixCtlNo);
        Remote = false;
        //m_nPaixCtlNo = 0;
    }
    return true;
}
//---------------------------------------------------------------------------
void CPaixMotion::SetIpAddress(short nField0,short nField1,short nField2)
{

    nmc_SetIPAddress(m_nPaixCtlNo, nField0,nField1,nField2);
}
//---------------------------------------------------------------------------
short CPaixMotion::GetLogic(int nAxis, PPARALOGIC pLogic)
{
    int nRet = 0 ;
    nRet = nmc_GetParaLogic(m_nPaixCtlNo,nAxis, pLogic);
    Log[nAxis].SetLog(__FUNC__, nRet);
    return nRet;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetSpeed(short nAxis, PPARASPEED pSpeed)
{
	int nRet;
	nRet = nmc_GetParaSpeed(m_nPaixCtlNo, nAxis, pSpeed);
        Log[nAxis].SetLog(__FUNC__, nRet);
	return nRet;
}
//---------------------------------------------------------------------------
//bool CPaixMotion::GetCmdSpeed()
//{
//    int nRet = 0;
//    if (!Remote) return false;
//
//    double * dCmdSpeed;
//    nRet = nmc_GetAxesCmdSpeed(m_nPaixCtlNo, dCmdSpeed);
//
//    switch( nRet ) {
//        case NMC_NOTCONNECT : m_bRemote = FALSE;
//        case 1:
//        case 2:
//        return false;
//    }
//
//    for(int i = 0; i < MAX_MOTR; i++){
//        m_dCmdSpeed[i] = dCmdSpeed[i];
//    }
//
//    return true;
//}
//---------------------------------------------------------------------------
//bool CPaixMotion::GetEncSpeed()
//{
//    int nRet = 0;
//    if (!Remote) return false;
//
//    double * dEncSpeed;
//    //double dEncSpeed[MAX_MOTR];
//    nRet = nmc_GetAxesCmdSpeed(m_nPaixCtlNo, dEncSpeed);
//
//    switch( nRet ) {
//        case NMC_NOTCONNECT : m_bRemote = FALSE;
//        case 1:
//        case 2:
//        return false;
//    }
//
//    for(int i = 0; i < MAX_MOTR; i++){
//        m_dEncSpeed[i] = dEncSpeed[i];
//    }
//
//    return true;
//}
//------------------------------------------------------------------------------
//bool CPaixMotion::GetCmdEncSpeed()
//{
//    int nRet = 0;
//    if (!Remote) return false;
//
//    double * dCmdSpeed;
//    double * dEncSpeed;
//    nRet = nmc_GetAxesCmdEncSpeed(m_nPaixCtlNo, dCmdSpeed, dEncSpeed);
//
//    switch( nRet ) {
//        case NMC_NOTCONNECT : m_bRemote = FALSE;
//        case 1:
//        case 2:
//        return false;
//    }
//
//    for(int i = 0; i < MAX_MOTR; i++){
//        m_dCmdSpeed[i] = dCmdSpeed[i];
//        m_dEncSpeed[i] = dEncSpeed[i];
//    }
//
//    return true;
//
//}
//------------------------------------------------------------------------------
bool CPaixMotion::SetEmgLogic(short OnOff)
{
    int nRet = 0 ;

    if( !Remote) return false;
    nRet = nmc_SetEmgLogic(m_nPaixCtlNo , 0 , OnOff);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetPlusLimitLogic(int nAxis, short OnOff)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetPlusLimitLogic(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetMinusLimitLogic(int nAxis, short OnOff)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetMinusLimitLogic(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetSWLimitLogic(int nAxis, short OnOff, double dSwMinusPos, double dSwPlusPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetSWLimitLogic(m_nPaixCtlNo, nAxis , OnOff , dSwMinusPos , dSwPlusPos);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetAlarmLogic(int nAxis, short OnOff)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetAlarmLogic(m_nPaixCtlNo , nAxis , OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetNearLogic      (int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetNearLogic(m_nPaixCtlNo , nAxis , OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;

    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetInPoLogic      (int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetInPoLogic(m_nPaixCtlNo , nAxis , OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetBusyOffMode(int nAxis, short OnOff)
{
    int nRet = 0;
    nRet = nmc_SetBusyOffMode(m_nPaixCtlNo,nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet )  {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetSvReadyLogic   (int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;
    nRet = nmc_SetSReadyLogic(m_nPaixCtlNo , nAxis , OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetHwLimitStopMode(int nAxis, short nMode)
{
    int nRet = 0;
    nRet = nmc_SetLimitStopMode(m_nPaixCtlNo,nAxis, nMode);
    Log[nAxis].SetLog(__FUNC__, nRet);
    switch( nRet )
    {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;

}
//------------------------------------------------------------------------------
/*
    nENCMODE
        0 - 4
        1 - 2
        2 - 1체배
*/
bool CPaixMotion::SetEncCount (int nAxis, short nCountMode)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetEncoderCount(m_nPaixCtlNo, nAxis, nCountMode);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
/*
    nCountDir
        0 = A|B(+)
        1 = B|A(-)
*/
bool CPaixMotion::SetEncDir   (int nAxis, short nCountDir )
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetEncoderDir(m_nPaixCtlNo, nAxis, nCountDir);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetEncZLogic(int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetEncoderZLogic(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
/*
    nMode
      2 Pulse
        0 = Low  Cw/CCw
        1 = Low  CCw/Cw
        2 = High Cw/CCw
        3 = High CCw/Cw
      1 Pulse
        4 = Low  Cw/CCw
        5 = Low  CCw/Cw
        6 = High Cw/CCw
        7 = High CCw/Cw
*/
bool CPaixMotion::SetPulseLogic(int nAxis, short nMode)
{
     int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetPulseLogic(m_nPaixCtlNo, nAxis, nMode);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
/*
    OnOff
      0 = Off
      1 = On
*/
bool CPaixMotion::SetCurrentOn   (int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetCurrentOn(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetServoOn(int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    if(nAxis == 0 && OnOff == 1){
        //SetOutputXBit(yETC_SpinBreak , true);
    }
    else if(nAxis == 0 && OnOff == 0){
        //SetOutputXBit(yETC_SpinBreak , false);
    }

    nRet = nmc_SetServoOn(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetAlarmResetOn(int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetDCCOn(int nAxis, short OnOff)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetDccOn(m_nPaixCtlNo, nAxis, OnOff);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetSpeed(int nAxis, double dSttSpeed, double dAcc, double dDec, double dDSpeed)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis >= MAX_MOTR || !Remote) return false;

    nRet = nmc_SetSpeed(m_nPaixCtlNo, nAxis, dSttSpeed, dAcc, dDec, dDSpeed);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetSCurveSpeed(int nAxis, double dSttSpeed, double dAcc, double dDec, double dDSpeed)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    if( g_NmcData.nBusy[nAxis] == 1 ){
        nRet = nmc_SetOverrideRunSpeed(m_nPaixCtlNo,nAxis, dAcc,dDec, dDSpeed);
    }

    else{
        nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo,nAxis, dSttSpeed, dAcc, dDec, dDSpeed);
    }
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ){
	case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetAccSpeed   (int nAxis, double dAcc)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetAccSpeed(m_nPaixCtlNo, nAxis, dAcc);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetDecSpeed   (int nAxis, double dDec)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetDecSpeed(m_nPaixCtlNo, nAxis, dDec);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
short CPaixMotion::SetRatio(short nAxis, double fRatio)
{
    short nRet = nmc_SetUnitPerPulse(m_nPaixCtlNo, nAxis, fRatio);
    Log[nAxis].SetLog(__FUNC__, nRet);
    return nRet;
}
//---------------------------------------------------------------------------
double CPaixMotion::GetRatio(short nAxis)
{
	return nmc_GetUnitPerPulse(m_nPaixCtlNo, nAxis);
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetOverrideRunSpeed(int nAxis, double dAcc, double dDec, double dDSpeed)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetOverrideRunSpeed(m_nPaixCtlNo, nAxis, dAcc, dDec, dDSpeed);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetOverrideDSpeed(int nAxis, double dDSpeed)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetOverrideDriveSpeed(m_nPaixCtlNo, nAxis, dDSpeed);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::AbsMove(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    int iPulsePos = 0 ;
    iPulsePos = GetPulsePos(nAxis , dPos);

    nRet = nmc_AbsMove(m_nPaixCtlNo, nAxis, iPulsePos);


    if(nAxis == miTiltMt) {
        m_dTiltTrgPos = dPos ;
    }

    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::IncMove(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    int iPulsePos = 0 ;
    iPulsePos = GetPulsePos(nAxis , dPos);

    nRet = nmc_RelMove(m_nPaixCtlNo, nAxis, iPulsePos);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
/*
bool CPaixMotion::VelMove(int nAxis, double dPos, double dDSpeed, short nMode) //iMode 0:AbsMove, 1:IncMove
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;
    //if(!GetBusyStatus(nAxis)) return false;

    nRet = nmc_VelMove(m_nPaixCtlNo, nAxis, dPos, dDSpeed, nMode);       //고려 안함.
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
*/
//------------------------------------------------------------------------------
/*
bool CPaixMotion::AbsOver(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_AbsOver(m_nPaixCtlNo, nAxis, dPos);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}*/

//------------------------------------------------------------------------------
bool CPaixMotion::JogMove(int nAxis, short CwCCw)    //0:Cw , 1:CCw
{
    int nRet;

    nRet = nmc_JogMove(m_nPaixCtlNo, nAxis, CwCCw);
    Log[nAxis].SetLog(__FUNC__, nRet);
    switch( nRet )
    {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SuddenStop (int nAxis)//긴급정지
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SuddenStop(m_nPaixCtlNo, nAxis);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::DecStop    (int nAxis)//감속 정지
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_DecStop(m_nPaixCtlNo, nAxis);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::AllAxisStop(int iMove) //0:감속정지, 1:긴급정지
{
    int nRet = 0;

    if(!Remote) return false;

    nRet = nmc_AllAxisStop(m_nPaixCtlNo, iMove);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool CPaixMotion::HomeMove(int nAxis, short nMode, short nEndMode, double dOffset)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    int iPulseOfsPos = 0 ;
    iPulseOfsPos = GetPulsePos(nAxis , dOffset);

    nRet = nmc_HomeMove(m_nPaixCtlNo, nAxis, 0x80|nMode, 0x00|nEndMode, iPulseOfsPos, 0);

    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetHomeSpeed(int nAxis, double dHomeSpeed1, double dHomeSpeed2, double dHomeSpeed3)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    nRet = nmc_SetHomeSpeed(m_nPaixCtlNo, nAxis, dHomeSpeed1, dHomeSpeed2, dHomeSpeed3);

    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::GetHomeStatus()
{
    int nRet = 0;

    if(!Remote) return false;

    NMCHOMEFLAG nmcHomeFlag;

    nRet = nmc_GetHomeStatus(m_nPaixCtlNo, &nmcHomeFlag);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
bool CPaixMotion::GetHomeDone(int nAxis)
{
    bool bRet;

    bRet = Stat[nAxis].bHomeDone;

    return bRet;
}
//------------------------------------------------------------------------------
bool CPaixMotion::InterPolation2Axis(int nAxis1, double dPos1, int nAxis2, double dPos2, short nOpt)//0:상대 이동, 1:절대 이동
{
}
//------------------------------------------------------------------------------
bool CPaixMotion::InterPolation3Axis(int nAxis1, double dPos1, int nAxis2, double dPos2, int nAxis3, double dPos3, short nOpt)//0:상대 이동, 1:절대 이동
{
}
//------------------------------------------------------------------------------
bool CPaixMotion::InterPolationArc  (int nAxis1, double dCenter1, double dCenter2, double dAngle, short nOpt)//0:상대 좌표, 1:절대 좌표
{
}
//------------------------------------------------------------------------------
//IO

short CPaixMotion::SetOutputEToggle(short nBit)
{
    short ret = nmc_SetEXDIOOutTogPin(m_nPaixCtlNo, nBit);
    return ret;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputXToggle(short nBit)
{
    short ret = nmc_SetDIOOutTogPin(m_nPaixCtlNo, nBit);
    return ret;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputXBit(short nBit, short nValue)
{
    short ret =  nmc_SetDIOOutputBit(m_nPaixCtlNo, nBit,nValue);
    return ret;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputEBit(short nBit, short nValue)
{
    short ret =  nmc_SetEXDIOOutPin(m_nPaixCtlNo, nBit,nValue);
    return ret;
}

short CPaixMotion::SetOutputMBit(short nBit, short nValue)
{
    short ret =  nmc_SetMDIOOutPin(m_nPaixCtlNo, nBit,nValue);
    return ret;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputMToggle(short nBit)
{
    short ret =  nmc_SetMDIOOutTogPin(m_nPaixCtlNo, nBit);
    return ret;
}

//---------------------------------------------------------------------------
short CPaixMotion::GetInputX()
{
    short ret =  nmc_GetDIOInput( m_nPaixCtlNo, g_lInputXIo);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputE()
{
    short ret = nmc_GetEXDIOInput( m_nPaixCtlNo, g_lInputEIo);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputM()
{
    short ret = nmc_GetMDIOInput( m_nPaixCtlNo, g_lInputMIo);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetOutputX()
{
    short ret = nmc_GetDIOOutput( m_nPaixCtlNo, g_lOutputXIo);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::SetOutputX(short *OutputX)
{
    short ret = nmc_SetDIOOutput( m_nPaixCtlNo, OutputX);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetOutputE()
{
    short ret = nmc_GetEXDIOOutput( m_nPaixCtlNo, g_lOutputEIo);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::SetOutputE(short *OutputE)
{
    short ret = nmc_SetEXDIOOutput( m_nPaixCtlNo, OutputE);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetOutputM()
{
    short ret = nmc_GetMDIOOutput( m_nPaixCtlNo, g_lOutputMIo);
    return ret;
}
//---------------------------------------------------------------------------
short CPaixMotion::SetOutputM(short *OutputM)
{
    short ret = nmc_SetMDIOOutput( m_nPaixCtlNo, OutputM);
    return ret;
}
//---------------------------------------------------------------------------
                                                                                                          
void CPaixMotion::GetInputX(short *pInputX)
{
    for( int i =0; i < 64; i ++) pInputX[i] = g_lInputXIo[i];
}

void CPaixMotion::GetInputE(short *pInputE)
{
    for( int i =0; i < 16; i ++) pInputE[i] = g_lInputEIo[i];
}
void CPaixMotion::GetInputM(short *pInputM)
{
    for( int i =0; i < 8; i ++) pInputM[i] = g_lInputMIo[i];
}

void CPaixMotion::GetOutputX(short *pOutputX)
{
    for( int i =0; i <64; i ++) pOutputX[i] = g_lOutputXIo[i];
}

void CPaixMotion::GetOutputE(short *pOutputE)
{
    for( int i =0; i < 16; i ++) {
        pOutputE[i] = g_lOutputEIo[i];
    }
}

void CPaixMotion::GetOutputM(short *pOutputM)
{
    for( int i =0; i < 8; i ++) pOutputM[i] = g_lOutputMIo[i];
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputXBit(short nBitNo, short *nInput)
{
    short nret = nmc_GetDIOInputBit( m_nPaixCtlNo, nBitNo, nInput);
    return nret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputEBit(short nBitNo, short *nInput)
{
    short nret = nmc_GetEXDIOInputBit( m_nPaixCtlNo, nBitNo,nInput);
    return nret;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputMBit(short nBitNo, short *nInput)
{
    short nret = nmc_GetMDIOInputBit( m_nPaixCtlNo, nBitNo,nInput);
    return nret;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSlowStop(int nAxis)
{
    Stat[nAxis].bJogP = false;
    Stat[nAxis].bJogN = false;
    DecStop(nAxis);
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::StopMotor(int nAxis)
{
    Stat[nAxis].bJogP = false;
    Stat[nAxis].bJogN = false;
    SuddenStop(nAxis);
    return true;
}
//---------------------------------------------------------------------------
void CPaixMotion::SetRemote(short bRemote)
{
    m_bRemote = bRemote;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetIpAddress(short *nField0,short *nField1,short *nField2,short *nField3)
{
    short a0,a1,a2,a3 ;
    return nmc_GetIPAddress(&a0,&a1,&a2,&a3);

    *nField0=a0;
    *nField1=a1;
    *nField2=a2;
    *nField3=a3;


}
//---------------------------------------------------------------------------
void __fastcall CPaixMotion::Execute()
{
    //GetNmcStatus(&g_NmcData);
    //GetInputX(g_lInputXIo);
    //GetOutputX(g_lOutputXIo);

    int nRet;
    char str[64];
    short bPreRemote = m_bRemote ;
    while( !Terminated )
    {
        //Sleep(10);
        //Sequence Thread Func.
        if(pThreadCallBack != NULL) pThreadCallBack();

        if(bPreRemote == true && m_bRemote == false )
        {
            m_bConnect    = false;
            m_bNotConnect = true ;
            m_bRemote = OpenPaixCtl(m_nPaixCtlNo);
            //PostMessage(frmMain->Handle,PAIX_SENSORREAD,PAIX_NOTCONNECT,0L);
            continue;
        }
        nRet = nmc_GetAxesExpress(m_nPaixCtlNo, &g_NmcData);

        if(nRet == 0){
            SendMessage(m_hWnd, PAIX_SENSORREAD, PAIX_SENSORREAD,0);
        }
        else {
            if(m_hWnd) SendMessage(m_hWnd, PAIX_SENSORREAD,PAIX_NOTCONNECT,nRet);
            m_bRemote = false;
            continue;
        }
        nRet = nmc_GetAxesMotionOut(m_nPaixCtlNo, &g_NmcMotionOut);

        //Update.
        for(int i = 0 ; i < MAX_MOTR ; i++) {
            if(Stat[i].bHomeStart ) {
                HomeProc (i);
            }
        }
        for(int i = 0; i < MAX_MOTR ; i++){
            GetCmdPos(i);
        }
        if(!Stat[Axis1].iHomeStep) {
            //GetAxisExpress();
        }

        if(!Stat[Axis2].iHomeStep) {
            //GetAxisExpress();
            //if(StatAxisInfo[Axis1].nPLimit && Stat[Axis1].bJogN){
            //    //StopMotor(Axis2);
            //}
            //if(StatAxisInfo[Axis1].nNear && !Stat[Axis1].bJogP){
                //Stat[0].bHomeDone = false ;
                //StopMotor(Axis2);
            //}
        }
        nRet = GetInputX();
        if( nRet == 0) {
            if( m_hWnd ) SendMessage(m_hWnd,PAIX_SENSORREAD,PAIX_IO_INPUT_X,0);
        }
        else {
            if( m_hWnd )SendMessage(m_hWnd,PAIX_SENSORREAD,PAIX_NOTCONNECT,nRet);
            m_bRemote = false;
            continue;
        }
        nRet = GetOutputX();
        if( nRet == 0){
            if( m_hWnd ) SendMessage(m_hWnd,PAIX_SENSORREAD,PAIX_IO_OUTPUT_X,0);
        }
        else {
            if( m_hWnd )SendMessage(m_hWnd,PAIX_SENSORREAD,PAIX_NOTCONNECT,nRet);
            m_bRemote = false;
            continue;
        }
    }
    m_hWnd = NULL;
    ClosePaixCtl();
}


void __fastcall CPaixMotion::SetRotorPara()
{
    //SetRatio          (Axis1, 1);
    //SetEmgLogic       (true) ; //true, false  (Active Level)
    //SetPlusLimitLogic (Axis1, false ) ; //true, false
    //SetMinusLimitLogic(Axis1, false ) ; //true, false
    //SetAlarmLogic     (Axis1, true  ) ; //true, false
    //SetNearLogic      (Axis1, false ) ; //true, false
    //SetInPoLogic      (Axis1, false  ) ; //true, false
    //SetSvReadyLogic   (Axis1, true  ) ; //true, false
    //SetEncZLogic      (Axis1, true  ) ; //true, false
    //SetEncDir         (Axis1, 0     ) ; //0 = A|B(+) : 1 = B|A(-)
    //SetEncCount       (Axis1, 0     ) ; //0:4체배 1:2체배 2:1체배
    //SetCurrentOn      (Axis1, 1  ) ; //true, false
    ////SetServoOn        (Axis1, 1  ) ; //true, false
    //SetDCCOn          (Axis1, 1  ) ; //true, false
    //SetAlarmResetOn   (Axis1, 0  ) ;
    //SetPulseLogic     (Axis1, 0  ) ; //0: 2Pulse Low CW/CCW , 1: Low CCW/CW 2: High CW/CCW 3: High CCW/CW
    //                                  //4: 1Pulse Low CW/CCW , 5: Low CCW/CW 6: High CW/CCW 7: High CCW/CW
    ////SetSWLimitLogic   (Axis1, 0, 1000000000 , -100000000000000); //Use ::: 0: No 1: Yes, minus Limit Pos , plus Limit Pos
    //
    //SetSpeed(Axis1,1,32768, 1000, 5000); //StartSpeed, Acc, Dec, Driv
    //SetHomeSpeed(Axis1, 500, 500, 1000);
    ////SetSpeedDist    (Axis1,1, 3800,360,0 );    //StartSpeed , Acc , Speed , Jerk
    //
    //
    //SetRatio          (Axis2, 1);
    //SetPlusLimitLogic (Axis2, false ) ; //true, false
    //SetMinusLimitLogic(Axis2, false ) ; //true, false
    //SetAlarmLogic     (Axis2, true  ) ; //true, false
    //SetNearLogic      (Axis2, false ) ; //true, false
    //SetInPoLogic      (Axis2, false ) ; //true, false
    //SetSvReadyLogic   (Axis2, true  ) ; //true, false
    //SetEncZLogic      (Axis2, true  ) ; //true, false
    //SetEncDir         (Axis2, 0     ) ; //0 = A|B(+) : 1 = B|A(-)
    //SetEncCount       (Axis2, 0     ) ; //0:4체배 1:2체배 2:1체배
    //SetCurrentOn      (Axis2, 1  ) ; //true, false
    //SetServoOn        (Axis2, 1  ) ; //true, false
    //SetDCCOn          (Axis2, 1  ) ; //true, false
    //SetAlarmResetOn   (Axis2, 0  ) ;
    //SetPulseLogic     (Axis2, 0  ) ; //0: 2Pulse Low CW/CCW , 1: Low CCW/CW 2: High CW/CCW 3: High CCW/CW
                                     //4: 1Pulse Low CW/CCW , 5: Low CCW/CW 6: High CW/CCW 7: High CCW/CW

    //SetSpeed(Axis2,1, 1000, 1000, 5000); //StartSpeed, Acc, Dec, Driv
    //SetHomeSpeed(Axis2, 500, 500, 1000);

    SetEmgLogic       (       OM.CmnOptn.iEmgLogic         ) ; //true, false  (Active Level)

    SetRatio          (Axis1, OM.CmnOptn.iRatio1           ) ;
    SetPlusLimitLogic (Axis1, OM.CmnOptn.iPlusLimitLogic1  ) ; //true, false
    SetMinusLimitLogic(Axis1, OM.CmnOptn.iMinusLimitLogic1 ) ; //true, false
    SetAlarmLogic     (Axis1, OM.CmnOptn.iAlarmLogic1      ) ; //true, false
    SetNearLogic      (Axis1, OM.CmnOptn.iNearLogic1       ) ; //true, false
    SetInPoLogic      (Axis1, OM.CmnOptn.iInPoLogic1       ) ; //true, false
    SetSvReadyLogic   (Axis1, OM.CmnOptn.iSvReadyLogic1    ) ; //true, false
    SetEncZLogic      (Axis1, OM.CmnOptn.iEncZLogic1       ) ; //true, false
    SetEncDir         (Axis1, OM.CmnOptn.iEncDir1          ) ; //0 = A|B(+) : 1 = B|A(-)
    SetEncCount       (Axis1, OM.CmnOptn.iEncCount1        ) ; //0:4체배 1:2체배 2:1체배
    SetCurrentOn      (Axis1, OM.CmnOptn.iCurrentOn1       ) ; //true, false
    SetDCCOn          (Axis1, OM.CmnOptn.iDCCOn1           ) ; //true, false
    SetAlarmResetOn   (Axis1, OM.CmnOptn.iAlarmResetOn1    ) ;
    SetPulseLogic     (Axis1, OM.CmnOptn.iPulseLogic1      ) ; //0: 2Pulse Low CW/CCW , 1: Low CCW/CW 2: High CW/CCW 3: High CCW/CW
                                                             //4: 1Pulse Low CW/CCW , 5: Low CCW/CW 6: High CW/CCW 7: High CCW/CW
    SetSpeed(Axis1,1,327680, 327680, 1310720); //StartSpeed, Acc, Dec, Driv
    //SetHomeSpeed(Axis1, 500, 500, 1000); 조그로 잡고 있음

    SetRatio          (Axis2, OM.CmnOptn.iRatio2          );
    SetPlusLimitLogic (Axis2, OM.CmnOptn.iPlusLimitLogic2 ) ; //true, false
    SetMinusLimitLogic(Axis2, OM.CmnOptn.iMinusLimitLogic2) ; //true, false
    SetAlarmLogic     (Axis2, OM.CmnOptn.iAlarmLogic2     ) ; //true, false
    SetNearLogic      (Axis2, OM.CmnOptn.iNearLogic2      ) ; //true, false
    SetInPoLogic      (Axis2, OM.CmnOptn.iInPoLogic2      ) ; //true, false
    SetSvReadyLogic   (Axis2, OM.CmnOptn.iSvReadyLogic2   ) ; //true, false
    SetEncZLogic      (Axis2, OM.CmnOptn.iEncZLogic2      ) ; //true, false
    SetEncDir         (Axis2, OM.CmnOptn.iEncDir2         ) ; //0 = A|B(+) : 1 = B|A(-)
    SetEncCount       (Axis2, OM.CmnOptn.iEncCount2       ) ; //0:4체배 1:2체배 2:1체배
    SetCurrentOn      (Axis2, OM.CmnOptn.iCurrentOn2      ) ; //true, false
    SetDCCOn          (Axis2, OM.CmnOptn.iDCCOn2          ) ; //true, false
    SetAlarmResetOn   (Axis2, OM.CmnOptn.iAlarmResetOn2   ) ;
    SetPulseLogic     (Axis2, OM.CmnOptn.iPulseLogic2     ) ; //0: 2Pulse Low CW/CCW , 1: Low CCW/CW 2: High CW/CCW 3: High CCW/CW
                                     //4: 1Pulse Low CW/CCW , 5: Low CCW/CW 6: High CW/CCW 7: High CCW/CW

    if(OM.CmnOptn.bUsedSpinLimit) SetSWLimitLogic(Axis1, true , OM.CmnOptn.iSpinMLimit, OM.CmnOptn.iSpinPLimit);
    else                          SetSWLimitLogic(Axis1, false, OM.CmnOptn.iSpinMLimit, OM.CmnOptn.iSpinPLimit);

    if(OM.CmnOptn.bUsedTiltLimit) SetSWLimitLogic(Axis2, true , OM.CmnOptn.iTiltMLimit, OM.CmnOptn.iTiltPLimit);
    else                          SetSWLimitLogic(Axis2, false, OM.CmnOptn.iTiltMLimit, OM.CmnOptn.iTiltPLimit);

    SetSpeed(Axis2,10, 200000, 200000, 200000); //StartSpeed, Acc, Dec, Driv
    //SetHomeSpeed(Axis2, 500, 500, 1000);  조그로 잡고 있음
}

//---------------------------------------------------------------------------
void __fastcall CPaixMotion::MoveJogRPM(int nAxis,int RPM)
{
    if( nAxis < 0 && nAxis >= MAX_MOTR || !Remote) return ;

    bool  bNeg = RPM < 0 ;
    short CwCCw = bNeg ? 1 : 0;

     if(g_NmcData.nBusy[nAxis]){                      //Speed Override Set
         if(nAxis == miSpinMt){
             //SetSpeed(nAxis , 1, 32768, 32768, GetPulseSpeedFromRPM(nAxis, RPM));
             SetOverrideDSpeed(nAxis , GetPulseSpeedFromRPM(nAxis, RPM));
         }
         if(nAxis == miTiltMt){
             //SetSpeed(nAxis , 1, 32768, 32768, GetPulseSpeedFromRPM(nAxis,RPM));
             SetOverrideDSpeed(nAxis , GetPulseSpeedFromRPM(nAxis, RPM));
         }
     }
     else  {
         SetSpeed(nAxis , 1, 327680, 327680, 3276800);   //Speed Set
         JogMove(nAxis , CwCCw);                     //JogMove(int nAxis, short CwCCw)    //0:Cw , 1:CCw
         RPM = GetPulseSpeedFromRPM(nAxis, RPM);
         SetOverrideDSpeed(nAxis , abs(RPM));
     }

     //if(nAxis == 0)
     //if(nAxis == 1)

}

double __fastcall CPaixMotion::GetRPM(int nAxis)
{
    double dRPM;
    PPARASPEED pSpeed;
    const int ONEMINUTE_SEC = 60;

    GetSpeed(nAxis, pSpeed);

    if(nAxis == miSpinMt){
        dRPM = pSpeed -> dDrive * ONEMINUTE_SEC / SPIN_PULSE_PER_REV ;
    }
    if(nAxis == miTiltMt){
        dRPM = pSpeed -> dDrive * ONEMINUTE_SEC / TILT_PULSE_PER_REV / TILT_REDUCTION ;
    }
    return dRPM;
}

void __fastcall CPaixMotion::MoveHome(int nAxis)
{
    if( nAxis < 0 && nAxis >= MAX_MOTR || !Remote) return ;

    Stat[nAxis].bHomeStart = true  ;
    Stat[nAxis].iHomeStep  = 10    ;
    Stat[nAxis].bHomeDone  = false ;

}


bool __fastcall CPaixMotion::HomeProc(int nAxis)
{
    //ReadSensorAll(&nmcData);

    static CDelayTimer tmSetZeroDelay  ;
    int iAcc = 327680;
    int iDcc = 3276;

    if(nAxis == Axis1) {
        switch(Stat[nAxis].iHomeStep) {

            default : Stat[nAxis].iHomeStep = 0 ; break ;

            case 10 : SetSpeed   (nAxis, 100, 3000, 3000, 5000 );
                      //SetRatio   (nAxis,PULSE_PER_REV             );
                      SetServoOn (nAxis,1                         );
                      JogMove    (nAxis,CCW                       );

                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 11 : //nmc_GetAxesExpress(m_nPaixCtlNo, &NmcData);
                      if(!g_NmcData.nNear[nAxis])return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 12 : //nmc_GetAxesExpress(m_nPaixCtlNo, &NmcData);
                      if(g_NmcData.nBusy[nAxis])    return false ;
                      SetSpeed      (nAxis, 10, 3000, 3000, 5000 );    //StartSpeed , Acc , Speed , Jerk
                      JogMove (nAxis, CW  );
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 13 : if(g_NmcData.nNear[nAxis]) return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 14 : if(g_NmcData.nBusy[nAxis])    return false ;
                      tmSetZeroDelay.Clear();
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 15 : if(!tmSetZeroDelay.OnDelay(true , 2000)) return false ; //모터 튜닝상태 이상할때 아직 찾아가고 있다.
                      //SetSpeed    (nAxis, 10, 327680, 327680, 3276800 );    //StartSpeed , Acc , Speed , Jerk
                      SetSpeed    (nAxis, 10, iAcc, iDcc, GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.iTiltMtSpeed));    //StartSpeed , Acc , Speed , Jerk
                      //SetSpeed    (nAxis, 10, 327680, 327680, GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed) * 2);    //StartSpeed , Acc , Speed , Jerk
                      SetCmdPos(nAxis, 0);
                      SetEncPos(nAxis, 0);
                      Stat[nAxis].iHomeStep  = 0 ;
                      Stat[nAxis].bHomeStart = false ;
                      Stat[nAxis].bHomeDone  = true  ;
                      //m_pMsgFunc("Axis1 HomeDone");
                      HomeTemp = false;
                      return true ;
        }

    }
}


/*
void __fastcall CPaixMotion::MoveShake(int nAxis , double _dTime , double _dRange)
{
    Stat[nAxis].iShakeStep  = 10     ;
    Stat[nAxis].bShaking    = true   ;
    Stat[nAxis].dShakeTime  = _dTime ;
    Stat[nAxis].dShakeRange = _dRange ;
}
bool __fastcall CPaixMotion::ShakeProc(int nAxis)
{
    //ReadSensorAll(&nmcData);

    if(nAxis != Axis1) return false ; //다른축은 못움직임.

    switch(Stat[nAxis].iShakeStep){

        default : Stat[nAxis].iShakeStep = 0 ; break ;

        case 10 : m_ShakeTimer.Clear();
                  m_pMsgFunc("Start Shaking");
                  Stat[nAxis].iShakeStep++ ;
                  return false ;

        case 11 : if(nmcData.nmcBit.nBusy0)    return false ;
                  pPaix -> RelMove (0,Stat[nAxis].dShakeRange ) ;
                  Stat[nAxis].iShakeStep++ ;
                  return false ;

        case 12 : if(nmcData.nmcBit.nBusy0)    return false ;
                  pPaix -> RelMove (0,-Stat[nAxis].dShakeRange ) ;
                  Stat[nAxis].iShakeStep++ ;
                  return false ;

        case 13 : if(nmcData.nmcBit.nBusy0)    return false ;
                  if(!m_ShakeTimer.OnDelay(true , Stat[nAxis].dShakeTime)) {Stat[nAxis].iShakeStep = 11 ; return false ; }
                  Stat[nAxis].bShaking    = false ;
                  Stat[nAxis].iShakeStep = 0  ;
                  return true  ;
    }
}
void CPaixMotion::SetDriveSpeed(int nAxis , int RPM )
{
    bool bNeg = RPM < 0 ;
    int nRet ;

    if(bNeg) RPM = abs(RPM);

    double RPS = RPM  / 60.0 ;                    //Rotation per Second.
    double APS = RPS * 360 ;                      //Angle    per Second. 잠시 수정.
    double PPS = RPS * PULSE_PER_REV;

    nmc_drive_pps(m_nPaixCtlNo, nAxis+1, PPS);

    return ;

}

bool CPaixMotion::OR_Start(int iAllTime , int iCnt , int *Speed , int *Time , int iAccTime)
{
    int nAxis = Axis1 ;

    Stat[nAxis].iORStep     = 10      ;
    Stat[nAxis].bOverRide   = true    ;
    Stat[nAxis].iMaxCnt     = iCnt    ;
    Stat[nAxis].iAccTime    = iAccTime;

    double dTotal = 0.0 ;
    int    iTemp  = 0   ;
    for(int i =0; i<= iCnt; i++ )
    {
//        if(!i) iTotal += Speed[i]*Time[i] ;
//        else   iTotal += Speed[i]*(Time[i]-Time[i-1]) ;
        dTotal += ((Speed[i]/2)*iAccTime )+ (Speed[i]*(Time[i]-iAccTime));

    }

    iTemp = Speed[0]*iAllTime ;

    dTotal = dTotal / iTemp ;

    dTotal = iAllTime*dTotal ;

    Stat[nAxis].iAllTime = (int)iAllTime   ;
    Stat[nAxis].dTotal   = dTotal   ;
    Stat[nAxis].iRPM     = Speed[0] ;

//    Stat[nAxis].dSpeed = new double(iCnt);
//    Stat[nAxis].dTime  = new double(iCnt);

    for(int i = 0; i <iCnt; i++ )
    {
        Stat[nAxis].iSpeed[i] = Speed[i] ;
        Stat[nAxis].iTime[i]  = Time [i] ;
    }



}
bool CPaixMotion::OR_Proc(int nAxis)
{
    if(nAxis != Axis1) return false ; //다른축은 못움직임.

//    int dTotal = 0;
//    int iCnt   = 0;

    switch(Stat[nAxis].iORStep){

        default : Stat[nAxis].iShakeStep = 0 ; break ;

        case 10 : m_OverRide.Clear();
                  m_OverRide1.Clear();
//                  m_pMsgFunc("Start OR_START");
                  iORCnt = 0;
                  OR_MoveRPM(nAxis,Stat[nAxis].iRPM, Stat[nAxis].iAccTime, Stat[nAxis].dTotal);
                  Stat[nAxis].iORStep++ ;
                  return false ;

        case 11 : //if(iCnt == 0 ) iTotal = Stat[nAxis].dTime[iCnt] ;
                  //else           iTotal = Stat[nAxis].dTime[iCnt] - Stat[nAxis].dTime[Stat[nAxis].iMaxCnt-1] ;
                  if(!m_OverRide.OnDelay(true, Stat[nAxis].iAllTime) ) {
                      if(!m_OverRide1.OnDelay(true, Stat[nAxis].iTime[iORCnt]) ) return false ;
                      else {
                          m_OverRide1.Clear();
                          iORCnt++;
                          SetDriveSpeed(nAxis, Stat[nAxis].iSpeed[iORCnt]);
                          return false ;
                      }

                      return false ;
                  }
                  else {
                      Stat[nAxis].bOverRide = false ;
                      Stat[nAxis].iORStep   = 0     ;
                      return true  ;
                  }

    }
}
*/
//--------------------------------------------------------------------------
void CPaixMotion::GetNmcStatus( PNMCAXESEXPR pNmcData)
{
	memcpy( (char *)pNmcData, (char *)&g_NmcData, sizeof(NMCAXESEXPR));
}
//--------------------------------------------------------------------------
void CPaixMotion::GetNmcMotionOut( PNMCAXESMOTIONOUT pNmcMotion)
{
	memcpy( (char *)pNmcMotion, (char *)&g_NmcMotionOut,sizeof(NMCAXESMOTIONOUT));
}
//--------------------------------------------------------------------------
void CPaixMotion::GetNmcHomeFlag( PNMCHOMEFLAG pNmcHomeFlag)
{
	memcpy( (char *)pNmcHomeFlag, (char *)&g_Homeflag,sizeof(NMCHOMEFLAG));
}
//--------------------------------------------------------------------------
bool CPaixMotion::GetBusyStatus( short nAxis, short *pnBusy)
{
	if( nAxis < 0 && nAxis > 8 || !Remote)
	{
        return false;
    }
    long lPulse;
    int nRet = 0;

    nRet = nmc_GetBusyStatus(m_nPaixCtlNo, nAxis, pnBusy);
    Log[nAxis].SetLog(__FUNC__, nRet);
    switch( nRet )
    {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------
bool CPaixMotion::GetBusyStatusAll( short *pnBusy)
{
    long lPulse;
    int nRet = 0;

    nRet = nmc_GetBusyStatusAll(m_nPaixCtlNo, pnBusy);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE; break;
        case 1   :  break;
        case 2   :  break;
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool CPaixMotion::SetCmdPos(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    if(nAxis == miSpinMt){
        dPos = dPos*SPIN_PULSE_PER_REV;
    }
    else {
        dPos = dPos*TILT_PULSE_PER_REV*TILT_REDUCTION/ONEREVOLUTION_ANGLE ;
    }

    nRet = nmc_SetCmdPos(m_nPaixCtlNo, nAxis, dPos);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetEncPos(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    if(nAxis == miSpinMt){
        dPos = dPos*SPIN_PULSE_PER_REV;
    }
    else {
        dPos = dPos*TILT_PULSE_PER_REV*TILT_REDUCTION/ONEREVOLUTION_ANGLE ;
    }

    nRet = nmc_SetEncPos(m_nPaixCtlNo, nAxis, dPos);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CPaixMotion::SetCmdEncPos(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

        if(nAxis == miSpinMt){
        dPos = dPos*SPIN_PULSE_PER_REV;
    }
    else {
        dPos = dPos*TILT_PULSE_PER_REV*TILT_REDUCTION/ONEREVOLUTION_ANGLE ;
    }

    nRet = nmc_SetCmdEncPos(m_nPaixCtlNo, nAxis, dPos);
    Log[nAxis].SetLog(__FUNC__, nRet);

    switch(nRet){
        case NMC_NOTCONNECT: m_bRemote = FALSE;
        case 1:
        case 2:
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
double CPaixMotion::GetCmdPos(int nAxis)
{
    double dCmdPos ;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    dCmdPos = g_NmcData.dCmd[nAxis];

    if(nAxis == miSpinMt){
        dCmdPos = dCmdPos / SPIN_PULSE_PER_REV;
    }
    else {
        dCmdPos = (dCmdPos * ONEREVOLUTION_ANGLE / TILT_PULSE_PER_REV / TILT_REDUCTION)  ;
    }

    return dCmdPos;
}

//------------------------------------------------------------------------------
double CPaixMotion::GetEncPos(int nAxis)
{
    double dEncPos ;

    if( nAxis < 0 && nAxis > MAX_MOTR || !Remote) return false;

    dEncPos = g_NmcData.dEnc[nAxis];

    if(nAxis == miSpinMt){
        dEncPos = dEncPos / SPIN_PULSE_PER_REV;
    }
    else {
        dEncPos = (dEncPos * ONEREVOLUTION_ANGLE / TILT_PULSE_PER_REV / TILT_REDUCTION)  ;
    }

    return dEncPos;
}

    //UI상의 단위를  펄스 로 변환함.
int CPaixMotion::GetPulsePos  (int nAxis , double dPos)
{
    int iRet ;

    if( nAxis < 0 && nAxis > MAX_MOTR ) return 0.0;

    if(nAxis == miSpinMt){
        iRet = dPos / SPIN_PULSE_PER_REV;
    }
    else {
        iRet = (dPos / ONEREVOLUTION_ANGLE * TILT_PULSE_PER_REV * TILT_REDUCTION)  ;
    }

    return iRet;

}

//초당 UI스피드 입력 했을때.
int CPaixMotion::GetPulseSpeed(int nAxis , double dUnitPerSec)
{
    int iRet ;

    if( nAxis < 0 && nAxis > MAX_MOTR ) return 0.0;

    if(nAxis == miSpinMt){ //바퀴단위. 1바퀴 두바퀴.
        iRet = dUnitPerSec * SPIN_PULSE_PER_REV;
    }
    else { //각도 단위.
        iRet = (dUnitPerSec / ONEREVOLUTION_ANGLE * TILT_PULSE_PER_REV * TILT_REDUCTION)  ;
    }

    return iRet;

}

int  CPaixMotion::GetPulseSpeedFromRPM(int nAxis , double RPM)
{
    if( nAxis < 0 && nAxis >= MAX_MOTR ) return 0;

    const int ONEMINUTE_SEC = 60;
    double dPPS;
    double dRPS = RPM / (double)ONEMINUTE_SEC ;          //Rotation per Second.


    if(nAxis == miSpinMt) {
        dPPS = dRPS * SPIN_PULSE_PER_REV * SPIN_REDUCTION;
    }
    if(nAxis == miTiltMt) {
        dPPS = dRPS * TILT_PULSE_PER_REV * TILT_REDUCTION / TILT_MPS;
        //dPPS = dPPS * TILT_REDUCTION; //
    }

    return dPPS;
}

