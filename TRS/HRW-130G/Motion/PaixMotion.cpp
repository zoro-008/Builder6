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
    Trace("PAIX Com","CPaixMotion()");
    Init();
}

__fastcall CPaixMotion::~CPaixMotion(void)
{
    m_hWnd = NULL;

    SetServoOn(Axis1 , 0);
    SetServoOn(Axis2 , 0);

    Terminate();
    //WaitFor(); //미확인 지움. 종료시 자꾸 걸림.

    ClosePaixCtl();

    Trace("PAIX Com",("m_iOffLineStep=" + String(m_iOffLineStep)).c_str());
    Trace("PAIX Com","~CPaixMotion");
}

void CPaixMotion::Init(void)
{
    AnsiString sTemp ;

    memset (&Stat , 0, (sizeof(TStat)*MAX_MOTR));

    pThreadCallBack = NULL ;

    m_bRemote = false ;
    m_iOffLineStep = 0 ; //리오픈 디버깅용.

    if(OpenPaixCtlNo(IP)) {
        Resume();
    }
    else {
        //Resume();
    }
}

//---------------------------------------------------------------------------
bool CPaixMotion::OpenPaixCtlNo(int nPaixCtlNo)
{
    short nRet ;
    String sTemp ;
    m_nPaixCtlNo = nPaixCtlNo ;
    nmc_SetIPAddress (nPaixCtlNo, 192,168,0);

    nRet = nmc_PingCheck(nPaixCtlNo,50);
    if(nRet != NMC_OK){
         sTemp = "Motion Connection Ping Test Failed [192.168.0." + String(nPaixCtlNo) + "] with Retern " + String(nRet) ;
         Trace("PAIX Com Err" , sTemp.c_str());
         ShowMessage(sTemp);
         return false ;
    }
    else {
         Trace("PAIX Com" , "Ping Test Success!");
    }

    nRet = nmc_OpenDevice(nPaixCtlNo);
    if(nRet != NMC_OK){
         sTemp = "nmc_OpenDevice Failed with Reteurn "+ String(nRet) ;
         Trace("PAIX Com Err" , sTemp.c_str());
         ShowMessage(sTemp);
         return false ;
    }
    else {
         Trace("PAIX Com" , "nmc_OpenDevice Success");
    }

    m_bRemote = true ;

    return true ;
}

//---------------------------------------------------------------------------
bool CPaixMotion::ClosePaixCtl(void)
{
    nmc_CloseDevice(m_nPaixCtlNo);
    Sleep(10); //확인은 못해봤지만 샘플에 클로즈 하고 슬립을 줌.
    m_bRemote = false;

    return true ;
}

//이것은 쓰레드에서 계속 탈 가능성이 있어서 트레이스 및 메세지창  넣지 않는다. iReOpenStep이것을 장비끌때 로그로 남김.
bool CPaixMotion::ReOpenPaixCtl(void) //중간에 끊어져서 닫고 오픈할때.
{

    short nRet ;
    String sTemp ;

    //일단 닫고.
    nmc_CloseDevice(m_nPaixCtlNo);
    m_bRemote = false;
    Sleep(10); //확인은 못해봤지만 샘플에 클로즈 하고 슬립을 줌.



    nRet = nmc_PingCheck(m_nPaixCtlNo,50);
    if(nRet != NMC_OK){
         //sTemp = "Motion Connection Ping Test Failed [192.168.0." + String(m_nPaixCtlNo) + "] with Retern " + String(nRet) ;
         //Trace("PAIX Com Err" , sTemp);
         //ShowMessage(sTemp);
         m_iOffLineStep = 1 ;
         return false ;
    }
    else {
         //Trace("PAIX Com" , "Ping Test Success!");
    }

    nRet = nmc_OpenDevice(m_nPaixCtlNo);
    if(nRet != NMC_OK){
         //sTemp = "nmc_OpenDevice Failed with Reteurn "+ String(nRet) ;
         //Trace("PAIX Com Err" , sTemp);
         //ShowMessage(sTemp);
         m_iOffLineStep = 2 ;
         return false ;
    }
    else {
         Trace("PAIX Com" , "ReOpenPaixCtl Success");
    }

    m_bRemote = true ;
    m_iOffLineStep = 0 ;
    return m_bRemote ;
}

//---------------------------------------------------------------------------
void __fastcall CPaixMotion::Execute()
{
    //GetNmcStatus(&g_NmcData);
    //GetInputX(g_lInputXIo);
    //GetOutputX(g_lOutputXIo);

    int nRet;
    char str[64];
    while( !Terminated )
    {
        //Sleep(10);
        //Sequence Thread Func.
        if(pThreadCallBack != NULL) pThreadCallBack();

        if(m_bRemote == false )
        {

            ReOpenPaixCtl();
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
            Trace("PAIX Com Err" , ("nmc_GetAxesExpress With Return "+String(nRet)).c_str());
            continue;
        }


        nRet = nmc_GetAxesMotionOut(m_nPaixCtlNo, &g_NmcMotionOut);
        //Update.
        for(int i = 0 ; i < MAX_MOTR ; i++) {
            if(Stat[i].bHomeStart ) {
                HomeProc (i);
            }
        }

        //for(int i = 0; i < MAX_MOTR ; i++){
        //    GetCmdPos(i);
        //}

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
            Trace("PAIX Com Err" , ("GetInputX With Return "+String(nRet)).c_str());
            continue;
        }
        nRet = GetOutputX();
        if( nRet == 0){
            if( m_hWnd ) SendMessage(m_hWnd,PAIX_SENSORREAD,PAIX_IO_OUTPUT_X,0);
        }
        else {
            if( m_hWnd )SendMessage(m_hWnd,PAIX_SENSORREAD,PAIX_NOTCONNECT,nRet);
            m_bRemote = false;
            Trace("PAIX Com Err" , ("GetOutputX With Return "+String(nRet)).c_str());
            continue;
        }
    }
    //m_hWnd = NULL;
    //ClosePaixCtl(); 소멸자로 이동 20141222 선
}

//---------------------------------------------------------------------------
/* 필요 없다.. 그냥쓰자.
void CPaixMotion::SetIpAddress(short nField0,short nField1,short nField2)
{

    nmc_SetIPAddress(m_nPaixCtlNo, nField0,nField1,nField2);
}*/
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
bool CPaixMotion::SetEmgLogic(short OnOff)
{
    int nRet = 0 ;

    Trace("PAIX PARA" ,("nmc_SetEmgLogic(" + String(OnOff) +")").c_str());

    if( !m_bRemote) return false;


    nRet = nmc_SetEmgLogic(m_nPaixCtlNo , 0 , OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEmgLogic With Return "+String(nRet)).c_str());
    }

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

    Trace("PAIX PARA" ,("SetPlusLimitLogic(" +String(nAxis)+ "," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;


    nRet = nmc_SetPlusLimitLogic(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetPlusLimitLogic With Return "+String(nRet)).c_str());
    }

    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE;  break;
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

    Trace("PAIX PARA" ,("SetMinusLimitLogic(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetMinusLimitLogic(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetMinusLimitLogic With Return "+String(nRet)).c_str());
    }

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

    Trace("PAIX PARA" ,("SetSWLimitLogic(" +String(nAxis) +"," + String(OnOff) + "," + String(dSwMinusPos) +"," + String(dSwPlusPos)+")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetSWLimitLogic(m_nPaixCtlNo, nAxis , OnOff , dSwMinusPos , dSwPlusPos);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetSWLimitLogic With Return "+String(nRet)).c_str());
    }

    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE;  break;
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

    Trace("PAIX PARA" ,("SetAlarmLogic(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetAlarmLogic(m_nPaixCtlNo , nAxis , OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetAlarmLogic With Return "+String(nRet)).c_str());
    }

    Log[nAxis].SetLog(__FUNC__, nRet);

    switch( nRet ) {
        case NMC_NOTCONNECT : m_bRemote = FALSE;  break;
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

    Trace("PAIX PARA" ,("SetNearLogic(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetNearLogic(m_nPaixCtlNo , nAxis , OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetNearLogic With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetInPoLogic      (int nAxis, short OnOff)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetInPoLogic(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetInPoLogic(m_nPaixCtlNo , nAxis , OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetInPoLogic With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetBusyOffMode(int nAxis, short OnOff)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetBusyOffMode(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    nRet = nmc_SetBusyOffMode(m_nPaixCtlNo,nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetBusyOffMode With Return "+String(nRet)).c_str());
    }
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

    Trace("PAIX PARA" ,("SetSvReadyLogic(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetSReadyLogic(m_nPaixCtlNo , nAxis , OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetSReadyLogic With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetHwLimitStopMode(int nAxis, short nMode)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetHwLimitStopMode(" +String(nAxis) +"," + String(nMode) +")").c_str());

    nRet = nmc_SetLimitStopMode(m_nPaixCtlNo,nAxis, nMode);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetLimitStopMode With Return "+String(nRet)).c_str());
    }
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

    Trace("PAIX PARA" ,("SetEncCount(" +String(nAxis) +"," + String(nCountMode) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetEncoderCount(m_nPaixCtlNo, nAxis, nCountMode);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEncoderCount With Return "+String(nRet)).c_str());
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
/*
    nCountDir
        0 = A|B(+)
        1 = B|A(-)
*/
bool CPaixMotion::SetEncDir   (int nAxis, short nCountDir )
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetEncDir(" +String(nAxis) +"," + String(nCountDir) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetEncoderDir(m_nPaixCtlNo, nAxis, nCountDir);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEncoderDir With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetEncZLogic(int nAxis, short OnOff)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetEncZLogic(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetEncoderZLogic(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEncoderZLogic With Return "+String(nRet)).c_str());
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

     Trace("PAIX PARA" ,("SetPulseLogic(" +String(nAxis)+ "," + String(nMode) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetPulseLogic(m_nPaixCtlNo, nAxis, nMode);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetPulseLogic With Return "+String(nRet)).c_str());
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
/*
    OnOff
      0 = Off
      1 = On
*/
bool CPaixMotion::SetCurrentOn   (int nAxis, short OnOff)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetCurrentOn(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetCurrentOn(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetCurrentOn With Return "+String(nRet)).c_str());
    }
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

    Trace("PAIX PARA" ,("SetServoOn(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    if(nAxis == 0 && OnOff == 1){
        SetOutputXBit(yETC_SpinBreak , true);
    }
    else if(nAxis == 0 && OnOff == 0){
        //20141014 JS
        SetOutputXBit(yETC_SpinBreak , false);
        //SetOutputXBit(yETC_SpinBreak , true);
    }

    nRet = nmc_SetServoOn(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetServoOn With Return "+String(nRet)).c_str());
    }
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

    Trace("PAIX PARA" ,("SetAlarmResetOn(" +String(nAxis) +"," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetAlarmResetOn With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetDCCOn(int nAxis, short OnOff)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetDCCOn(" +String(nAxis)+ "," + String(OnOff) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetDccOn(m_nPaixCtlNo, nAxis, OnOff);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetDccOn With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetSpeed(int nAxis, double dSttSpeed, double dAcc, double dDec, double dDSpeed)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetSpeed(" +String(nAxis) +"," + String(dDSpeed) +")").c_str());

    if( nAxis < 0 && nAxis >= MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetSpeed(m_nPaixCtlNo, nAxis, dSttSpeed, dAcc, dDec, dDSpeed);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetSpeed With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetSCurveSpeed(int nAxis, double dSttSpeed, double dAcc, double dDec, double dDSpeed)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetSCurveSpeed(" +String(nAxis)+ "," + String(dDSpeed) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    if( g_NmcData.nBusy[nAxis] == 1 ){
        nRet = nmc_SetOverrideRunSpeed(m_nPaixCtlNo,nAxis, dAcc,dDec, dDSpeed);
        if(nRet != NMC_OK) {
            Trace("PAIX Com Err" , ("nmc_SetOverrideRunSpeed With Return "+String(nRet)).c_str());
        }
    }

    else{
        nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo,nAxis, dSttSpeed, dAcc, dDec, dDSpeed);
        if(nRet != NMC_OK) {
            Trace("PAIX Com Err" , ("nmc_SetSCurveSpeed With Return "+String(nRet)).c_str());
        }
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

    Trace("PAIX PARA" ,("SetAccSpeed(" +String(nAxis)+ "," + String(dAcc) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetAccSpeed(m_nPaixCtlNo, nAxis, dAcc);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetAccSpeed With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetDecSpeed   (int nAxis, double dDec)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetDecSpeed(" +String(nAxis)+ "," + String(dDec) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetDecSpeed(m_nPaixCtlNo, nAxis, dDec);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetDecSpeed With Return "+String(nRet)).c_str());
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
short CPaixMotion::SetRatio(short nAxis, double fRatio)
{
    Trace("PAIX PARA" ,("SetRatio(" +String(nAxis) +"," + String(fRatio) +")").c_str());

    short nRet = nmc_SetUnitPerPulse(m_nPaixCtlNo, nAxis, fRatio);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetUnitPerPulse With Return "+String(nRet)).c_str());
    }

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

    Trace("PAIX PARA" ,("SetOverrideRunSpeed(" +String(nAxis) +"," + String(dDSpeed) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetOverrideRunSpeed(m_nPaixCtlNo, nAxis, dAcc, dDec, dDSpeed);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetOverrideRunSpeed With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetOverrideDSpeed(int nAxis, double dDSpeed)
{
    int nRet = 0;

    Trace("PAIX PARA" ,("SetOverrideDSpeed(" +String(nAxis) +"," + String(dDSpeed) +")").c_str());

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetOverrideDriveSpeed(m_nPaixCtlNo, nAxis, dDSpeed);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetOverrideDriveSpeed With Return "+String(nRet)).c_str());
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
bool CPaixMotion::AbsMove(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    int iPulsePos = 0 ;
    iPulsePos = GetPulsePos(nAxis , dPos);

    nRet = nmc_AbsMove(m_nPaixCtlNo, nAxis, iPulsePos);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_AbsMove With Return "+String(nRet)).c_str());
    }

    if(nAxis == 1) {
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

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    int iPulsePos = 0 ;
    iPulsePos = GetPulsePos(nAxis , dPos);

    nRet = nmc_RelMove(m_nPaixCtlNo, nAxis, iPulsePos);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_RelMove With Return "+String(nRet)).c_str());
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
bool CPaixMotion::JogMove(int nAxis, short CwCCw)    //0:Cw , 1:CCw
{
    int nRet;

    nRet = nmc_JogMove(m_nPaixCtlNo, nAxis, CwCCw);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_JogMove With Return "+String(nRet)).c_str());
    }
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

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SuddenStop(m_nPaixCtlNo, nAxis);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SuddenStop With Return "+String(nRet)).c_str());
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
bool CPaixMotion::DecStop    (int nAxis)//감속 정지
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_DecStop(m_nPaixCtlNo, nAxis);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_DecStop With Return "+String(nRet)).c_str());
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
bool CPaixMotion::AllAxisStop(int iMove) //0:감속정지, 1:긴급정지
{
    int nRet = 0;

    if(!m_bRemote) return false;

    nRet = nmc_AllAxisStop(m_nPaixCtlNo, iMove);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_AllAxisStop With Return "+String(nRet)).c_str());
    }
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

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    int iPulseOfsPos = 0 ;
    iPulseOfsPos = GetPulsePos(nAxis , dOffset);

    nRet = nmc_HomeMove(m_nPaixCtlNo, nAxis, 0x80|nMode, 0x00|nEndMode, iPulseOfsPos, 0);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_HomeMove With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetHomeSpeed(int nAxis, double dHomeSpeed1, double dHomeSpeed2, double dHomeSpeed3)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    nRet = nmc_SetHomeSpeed(m_nPaixCtlNo, nAxis, dHomeSpeed1, dHomeSpeed2, dHomeSpeed3);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetHomeSpeed With Return "+String(nRet)).c_str());
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
bool CPaixMotion::GetHomeStatus()
{
    int nRet = 0;

    if(!m_bRemote) return false;

    NMCHOMEFLAG nmcHomeFlag;

    nRet = nmc_GetHomeStatus(m_nPaixCtlNo, &nmcHomeFlag);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetHomeStatus With Return "+String(nRet)).c_str());
    }

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
    short nRet = nmc_SetEXDIOOutTogPin(m_nPaixCtlNo, nBit);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEXDIOOutTogPin With Return "+String(nRet)).c_str());
    }
    return nRet;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputXToggle(short nBit)
{
    short nRet = nmc_SetDIOOutTogPin(m_nPaixCtlNo, nBit);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetDIOOutTogPin With Return "+String(nRet)).c_str());
    }
    return nRet;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputXBit(short nBit, short nValue)
{
    short nRet =  nmc_SetDIOOutputBit(m_nPaixCtlNo, nBit,nValue);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetDIOOutputBit With Return "+String(nRet)).c_str());
    }
    return nRet;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputEBit(short nBit, short nValue)
{
    short nRet =  nmc_SetEXDIOOutPin(m_nPaixCtlNo, nBit,nValue);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEXDIOOutPin With Return "+String(nRet)).c_str());
    }
    return nRet;
}

short CPaixMotion::SetOutputMBit(short nBit, short nValue)
{
    short nRet =  nmc_SetMDIOOutPin(m_nPaixCtlNo, nBit,nValue);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetMDIOOutPin With Return "+String(nRet)).c_str());
    }
    return nRet;
}

//---------------------------------------------------------------------------
short CPaixMotion::SetOutputMToggle(short nBit)
{
    short nRet =  nmc_SetMDIOOutTogPin(m_nPaixCtlNo, nBit);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetMDIOOutTogPin With Return "+String(nRet)).c_str());
    }
    return nRet;
}

//---------------------------------------------------------------------------
short CPaixMotion::GetInputX()
{
    short nRet =  nmc_GetDIOInput( m_nPaixCtlNo, g_lInputXIo);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetDIOInput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputE()
{
    short nRet = nmc_GetEXDIOInput( m_nPaixCtlNo, g_lInputEIo);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetEXDIOInput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputM()
{
    short nRet = nmc_GetMDIOInput( m_nPaixCtlNo, g_lInputMIo);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetMDIOInput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetOutputX()
{
    short nRet = nmc_GetDIOOutput( m_nPaixCtlNo, g_lOutputXIo);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetDIOOutput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::SetOutputX(short *OutputX)
{
    short nRet = nmc_SetDIOOutput( m_nPaixCtlNo, OutputX);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetDIOOutput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetOutputE()
{
    short nRet = nmc_GetEXDIOOutput( m_nPaixCtlNo, g_lOutputEIo);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetEXDIOOutput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::SetOutputE(short *OutputE)
{
    short nRet = nmc_SetEXDIOOutput( m_nPaixCtlNo, OutputE);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEXDIOOutput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetOutputM()
{
    short nRet = nmc_GetMDIOOutput( m_nPaixCtlNo, g_lOutputMIo);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetMDIOOutput With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::SetOutputM(short *OutputM)
{
    short nRet = nmc_SetMDIOOutput( m_nPaixCtlNo, OutputM);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetMDIOOutput With Return "+String(nRet)).c_str());
    }
    return nRet;
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
    short nRet = nmc_GetDIOInputBit( m_nPaixCtlNo, nBitNo, nInput);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetDIOInputBit With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputEBit(short nBitNo, short *nInput)
{
    short nRet = nmc_GetEXDIOInputBit( m_nPaixCtlNo, nBitNo,nInput);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetEXDIOInputBit With Return "+String(nRet)).c_str());
    }
    return nRet;
}
//---------------------------------------------------------------------------
short CPaixMotion::GetInputMBit(short nBitNo, short *nInput)
{
    short nRet = nmc_GetMDIOInputBit( m_nPaixCtlNo, nBitNo,nInput);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetMDIOInputBit With Return "+String(nRet)).c_str());
    }
    return nRet;
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
bool CPaixMotion::GetIpAddress(short *nField0,short *nField1,short *nField2,short *nField3)
{
    short a0,a1,a2,a3 ;





    short nRet = nmc_GetIPAddress(&a0,&a1,&a2,&a3);

    *nField0=a0;
    *nField1=a1;
    *nField2=a2;
    *nField3=a3;

    return nRet ;

}



void __fastcall CPaixMotion::ApplyPara()
{
/*
    int    iRatio1             ;
    int    iPlusLimitLogic1    ;
    int    iMinusLimitLogic1   ;
    int    iAlarmLogic1        ;
    int    iNearLogic1         ;
    int    iInPoLogic1         ;
    int    iSvReadyLogic1      ;
    int    iEncZLogic1         ;
    int    iEncDir1            ;
    int    iEncCount1          ;
    int    iCurrentOn1         ;
    int    iDCCOn1             ;
    int    iAlarmResetOn1      ;
    int    iPulseLogic1        ;
    bool   bUsedSpinLimit      ;
    double dSpinMLimit         ;
    double dSpinPLimit         ;
*/

    Trace("PAIX PARA","ApplyPara()");
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
    SetPulseLogic     (Axis1, OM.CmnOptn.iPulseLogic1      ) ; //0: 2Pulse Low CW/CCW , 1: Low CCW/CW 2: High CW/CCW 3: High CCW/CW//4: 1Pulse Low CW/CCW , 5: Low CCW/CW 6: High CW/CCW 7: High CCW/CW
    SetSWLimitLogic(Axis1, OM.CmnOptn.bUsedSpinLimit , GetPulsePos(Axis1 , OM.CmnOptn.dSpinMLimit), GetPulsePos(Axis1 , OM.CmnOptn.dSpinPLimit));

    SetSpeed(Axis1,1, 10000, 10000 , 1310720); //StartSpeed, Acc, Dec, Driv
    //SetHomeSpeed(Axis1, 500, 500, 1000); 조그로 잡고 있음

    //20150107 선계원 회사에서..비지오프 시점 설정을 인포지션이 들어오고 꺼지게 설정.
    SetBusyOffMode(Axis1 , true );


    SetRatio          (Axis2, OM.CmnOptn.iRatio2          ) ;
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
    SetPulseLogic     (Axis2, OM.CmnOptn.iPulseLogic2     ) ; //0: 2Pulse Low CW/CCW , 1: Low CCW/CW 2: High CW/CCW 3: High CCW/CW//4: 1Pulse Low CW/CCW , 5: Low CCW/CW 6: High CW/CCW 7: High CCW/CW
    SetSWLimitLogic(Axis2, OM.CmnOptn.bUsedTiltLimit , GetPulsePos(Axis2 , OM.CmnOptn.dTiltMLimit), GetPulsePos(Axis2 , OM.CmnOptn.dTiltPLimit));

    SetSpeed(Axis2,10, 32768, 32768, 327680); //StartSpeed, Acc, Dec, Driv
    //SetHomeSpeed(Axis2, 500, 500, 1000);  조그로 잡고 있음

    //20150107 선계원 회사에서..비지오프 시점 설정을 인포지션이 들어오고 꺼지게 설정.
    SetBusyOffMode(Axis2 , true );
}

//---------------------------------------------------------------------------
void __fastcall CPaixMotion::MoveJogRPM(int nAxis,int RPM)
{
    if( nAxis < 0 && nAxis >= MAX_MOTR || !m_bRemote) return ;

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
    if( nAxis < 0 && nAxis >= MAX_MOTR || !m_bRemote) return ;

    Stat[nAxis].bHomeStart = true  ;
    Stat[nAxis].iHomeStep  = 10    ;
    Stat[nAxis].bHomeDone  = false ;

}

//20150107 모션 비지 시그널 인포지션 후 꺼지게 해서..
//일단 모니터링 하고 그래도 병신짓 하면 파익스 홈으로 바꿔 보자....
bool __fastcall CPaixMotion::HomeProc(int nAxis)
{
    //ReadSensorAll(&nmcData);

    static CDelayTimer tmSetZeroDelay  ;
    int iAcc = 327680;
    int iDcc = 3276;

    AnsiString sTemp ;
    static int iPreHomeStep ;
    sTemp = sTemp.sprintf("iTiltHomeStep=%02d" , Stat[nAxis].iHomeStep );
    if(Stat[nAxis].iHomeStep != iPreHomeStep) {
        Trace(__FUNC__,sTemp.c_str());
    }
    iPreHomeStep = Stat[nAxis].iHomeStep ;


    if(nAxis == Axis2) {
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

            case 13 : if(g_NmcData.nNear[nAxis]) return false ;      //여기 헛점이 있네... 감속 정지시 가감속 낮게 하면 센서 넘어가서 병신짓할 소지 있음.
                      StopMotor(nAxis);                              //
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 14 : if(g_NmcData.nBusy[nAxis])    return false ;
                      tmSetZeroDelay.Clear();
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 15 : if(!tmSetZeroDelay.OnDelay(true , 2000)) return false ;
                      //SetSpeed    (nAxis, 10, 327680, 327680, 3276800 );    //StartSpeed , Acc , Speed , Jerk
                      SetSpeed    (nAxis, 10, iAcc, iDcc, GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));    //StartSpeed , Acc , Speed , Jerk
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
	if( nAxis < 0 && nAxis > 8 || !m_bRemote)
	{
        return false;
    }
    long lPulse;
    int nRet = 0;

    nRet = nmc_GetBusyStatus(m_nPaixCtlNo, nAxis, pnBusy);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetBusyStatus With Return "+String(nRet)).c_str());
    }
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
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_GetBusyStatusAll With Return "+String(nRet)).c_str());
    }

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

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    if(nAxis == miSpinMt){
        dPos = dPos*SPIN_PULSE_PER_REV;
    }
    else {
        dPos = dPos*TILT_PULSE_PER_REV*TILT_REDUCTION/ONEREVOLUTION_ANGLE ;
    }

    nRet = nmc_SetCmdPos(m_nPaixCtlNo, nAxis, dPos);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetCmdPos With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetEncPos(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    if(nAxis == miSpinMt){
        dPos = dPos*SPIN_PULSE_PER_REV;
    }
    else {
        dPos = dPos*TILT_PULSE_PER_REV*TILT_REDUCTION/ONEREVOLUTION_ANGLE ;
    }

    nRet = nmc_SetEncPos(m_nPaixCtlNo, nAxis, dPos);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetEncPos With Return "+String(nRet)).c_str());
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
bool CPaixMotion::SetCmdEncPos(int nAxis, double dPos)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

        if(nAxis == miSpinMt){
        dPos = dPos*SPIN_PULSE_PER_REV;
    }
    else {
        dPos = dPos*TILT_PULSE_PER_REV*TILT_REDUCTION/ONEREVOLUTION_ANGLE ;
    }

    nRet = nmc_SetCmdEncPos(m_nPaixCtlNo, nAxis, dPos);
    if(nRet != NMC_OK) {
        Trace("PAIX Com Err" , ("nmc_SetCmdEncPos With Return "+String(nRet)).c_str());
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
double CPaixMotion::GetCmdPos(int nAxis)
{
    double dCmdPos ; //펄스 단위 포지션.
    double dRetPos ; //실제 화면 단위 포지션.

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    //dCmdPos = g_NmcData.dCmd[nAxis]; 20141223 선계원 lcmd가 펄스 단위 이다.
    dCmdPos = g_NmcData.lCmd[nAxis];


    

    if(nAxis == miSpinMt){
        dRetPos = dCmdPos / SPIN_PULSE_PER_REV;
    }
    else {

        dRetPos = (dCmdPos * ONEREVOLUTION_ANGLE / TILT_PULSE_PER_REV / TILT_REDUCTION)  ;


    }

    return dRetPos;
}

//------------------------------------------------------------------------------
double CPaixMotion::GetEncPos(int nAxis)
{
    double dEncPos ;
    double dRetPos ; //실제 화면 단위 포지션.

    if( nAxis < 0 && nAxis > MAX_MOTR || !m_bRemote) return false;

    //dEncPos = g_NmcData.dEnc[nAxis]; 20141223 선계원 lcmd가 펄스 단위 이다.
    dEncPos = g_NmcData.lEnc[nAxis];

    if(nAxis == miSpinMt){
        dRetPos = dEncPos / SPIN_PULSE_PER_REV;
    }
    else {
        dRetPos = (dEncPos * ONEREVOLUTION_ANGLE / TILT_PULSE_PER_REV / TILT_REDUCTION)  ;
    }

    return dRetPos;
}

    //UI상의 단위를  펄스 로 변환함.
int CPaixMotion::GetPulsePos  (int nAxis , double dPos)
{
    int iRet ;

    if( nAxis < 0 && nAxis > MAX_MOTR ) return 0.0;

    if(nAxis == miSpinMt){
        iRet = dPos / SPIN_PULSE_PER_REV ;
    }
    else {
        //iRet = (dPos / ONEREVOLUTION_ANGLE * TILT_PULSE_PER_REV * TILT_REDUCTION)  ;
        iRet = (dPos * TILT_PULSE_PER_REV * TILT_REDUCTION / ONEREVOLUTION_ANGLE)  ;
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
        //iRet = (dUnitPerSec / ONEREVOLUTION_ANGLE * TILT_PULSE_PER_REV * TILT_REDUCTION)  ;
        iRet = (dUnitPerSec * TILT_PULSE_PER_REV * TILT_REDUCTION / ONEREVOLUTION_ANGLE)  ;
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
        dPPS = dRPS * SPIN_PULSE_PER_REV;
    }
    if(nAxis == miTiltMt) {
        dPPS = dRPS * TILT_PULSE_PER_REV;
        dPPS = dPPS * TILT_REDUCTION; //
    }

    return dPPS;
}

