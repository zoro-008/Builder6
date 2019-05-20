//---------------------------------------------------------------------------
#include <VCL.H>
#include <winbase.h>
#include <stdio.h>
#pragma hdrstop

#include "PaixMotion.h"
#include "MYUtil.h"
#include "FormMain.h"

#define FIRMVER_20050727 4
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CPaixMotion *pPaix;

__fastcall CPaixMotion::CPaixMotion(void):TThread(TRUE)
{
    m_nMotionMode[0] = 0;
    m_nMotionMode[1] = 0;
    m_nMotionMode[2] = 0;
    m_nMotionMode[3] = 0;

    iORCnt = 0;

//    m_bAxis1Stop = false;
//    m_bAxis2Stop = false;
    memset (&Stat , 0 , (sizeof(TStat)*MAX_MOTR));
    //for(int i = 0 ; i < MAX_MOTR ; i++) {
    //    Stat[i].bShakeDone  = true  ;
    //    Stat[i].iShakeStep  = 0     ;
    //}



    memset (&StatAxis , 0 , (sizeof(TStatAxis)*MAX_MOTR));


    //SetIpAddress(192,168,2);

    //192.168.0.1  피씨 모션 랜카드 아이피.      <->  파익스 컨트롤러 아이피 192.168.0.20
    //192.168.1.1  피씨 카메라 랜카드 아이피.    <->  바슬러 카메라  아이피 192.168.1.2

    SetIpAddress(192,168,0);
    if(!SetPaixCtlNo(20))//11
    {
        ShowMessage("Motion Connection Failed [192.168.0.20]");
        //Msg("Motion Connect Fail [Check Motion Cable]");
        //pPaix = NULL;
        //return ;
    }
    SetRotorPara();
    Resume();


}

__fastcall CPaixMotion::~CPaixMotion(void)
{
    pPaix->SetServoOnOff   (Axis1,1    );
    pPaix->SetMotorOnOff   (Axis2,0    );

    pPaix->Terminate();
    //pPaix->WaitFor(); //미확인 지움. 종료시 자꾸 걸림.
}


//---------------------------------------------------------------------------
bool CPaixMotion::PaixInit(int nPaixCtlNo)
{
    int nRet;
    init_nmc(m_nPaixCtlNo);
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetPaixCtlNo(int nPaixCtlNo)
{
    if( nPaixCtlNo < 0 )
        return false;
    {
        ClosePaixCtl();
        m_bRemote = false;
        Sleep(10);
    }
    m_nPaixCtlNo = nPaixCtlNo;
    nmc_set_event(m_nPaixCtlNo, true);
    m_bRemote = nmc_opendevice(m_nPaixCtlNo);
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
    int nRet;
    if( m_nPaixCtlNo )
    {
        nmc_closedevice(m_nPaixCtlNo);
        Remote = false;
        //m_nPaixCtlNo = 0;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetRpm(int nAxis,long lStart, long lAcc,long lMax,long lJerk)
{
    int nRet = 0;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    //m_nMotionMode[nAxis] = 0;

    nRet = nmc_speed_rpm(m_nPaixCtlNo,nAxis+1,lStart,lAcc,lMax,lJerk);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        TRACE("SPeed Error");
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSpeedPPS(int nAxis,long lStart, long lAcc,long lMax,long lJerk)
{
    int nRet = 0;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    //m_nMotionMode[nAxis] = 0;
//    nRet = nmc_drive_pps(m_nPaixCtlNo,nAxis+1,  lAcc);
    nRet = nmc_scurve_pps(m_nPaixCtlNo,nAxis+1,  lStart,lAcc,lMax,lJerk); //잠시 수정.


    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
            TRACE("SPeed Error");

        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSpeedDist(int nAxis,double lStart, double lAcc,double lMax,long lJerk)
{
    int nRet = 0;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    //m_nMotionMode[nAxis] = 1;
    nRet = nmc_speed_dist(m_nPaixCtlNo,nAxis+1,lStart,lAcc,lMax,lJerk); //잠시 수정.
//	short WINAPI nmc_speed_pps(short dev_no, short axis, long start, long accdec, long max);
//    nRet = nmc_speed_pps(m_nPaixCtlNo,nAxis+1,lStart,lAcc,lMax); //잠시 수정.

//    long ppsSpeed = lAcc ;
//    nRet = nmc_drive_pps(m_nPaixCtlNo,nAxis+1, ppsSpeed);


    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        TRACE("SPeed Error");
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
/*
    nENCMODE
        0 - 4
        1 - 2
        2 - 1체배
*/
bool CPaixMotion::SetEncCountMode( int nAxis,int nEncMode)
{
    int nRet = 0;
    //int nEnc;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;

    nRet = nmc_encoder_counter_mode(m_nPaixCtlNo, nAxis+1,nEncMode);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetMotorMode( int nAxis,int nMotorMode)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;

    nRet |= nmc_select_motor(m_nPaixCtlNo, nAxis+1, nMotorMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetMotorRes( int nAxis,long lRes)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;

    nmc_set_resolution(m_nPaixCtlNo, nAxis+1, lRes);
    return true;
}
//---------------------------------------------------------------------------
double CPaixMotion::GetMotorRes( int nAxis)
{
    return nmc_get_resolution(m_nPaixCtlNo, nAxis+1);
}
//---------------------------------------------------------------------------

bool CPaixMotion::AbsMove( int nAxis, double fDist)
{
    double a,b;
    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    long lPulse,nRot;
    int nRet = 0;

    switch( m_nMotionMode[nAxis] )
    {
    case 0: // Pulse
        lPulse = fDist;
    break;
    case 1:
        nRot = nmc_get_resolution(m_nPaixCtlNo, nAxis+1);
        lPulse = fDist *nRot;
    break;
    case 2: // mm
        a = nmc_get_resolution(m_nPaixCtlNo, nAxis+1) ;
        b = nmc_get_leed(m_nPaixCtlNo, nAxis+1) ;
        lPulse = (fDist*a)/b;
    break;
    }
    nRet = nmc_aptp_move(m_nPaixCtlNo, nAxis+1, lPulse);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 0:
        break;
    default:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::RelMove( int nAxis, double fDist)
{
    int nRet = 0;
    int nRot,nPulse;
    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    switch( m_nMotionMode[nAxis] )
    {
    case 0: // Pulse
        nRet = nmc_ptp_move(m_nPaixCtlNo, nAxis+1, fDist);
    break;
    case 1:
        nRot = nmc_get_resolution(m_nPaixCtlNo, nAxis+1);
        fDist *= nRot;
        nRet = nmc_ptp_move(m_nPaixCtlNo, nAxis+1, fDist);
    break;
    case 2:
        //nRot = nmc_get_resolution(m_nPaixCtlNo, nAxis+1)/nmc_get_leed(m_nPaixCtlNo, nAxis+1);
        //fDist *= nRot;

        nPulse = (nmc_get_resolution(m_nPaixCtlNo, nAxis+1)*fDist)/nmc_get_leed(m_nPaixCtlNo, nAxis+1);



        nRet = nmc_ptp_move(m_nPaixCtlNo, nAxis+1, nPulse);
//        nRet = nmc_aptp_move(m_nPaixCtlNo, nAxis+1, fDist);

    break;
    }
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::MotionDone(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    short nValue;
    if( nmc_busy(m_nPaixCtlNo,nAxis+1,&nValue) != NMC_OK )
        return false;

    if( GetEmer() )
        return true;
    if( nValue == 1 )
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::MotionDoneAll()
{
	NMCDATA nmcData;
	short nReturn=0;

	nReturn = nmc_get_sensor_all(m_nPaixCtlNo, &nmcData);

	if( nReturn < 0 )
		return nReturn;

    if( GetEmer() )
        return true;
    if( nmcData.nmcBit.nBusy0 || nmcData.nmcBit.nBusy1 ||
    	nmcData.nmcBit.nBusy2 ||
	    nmcData.nmcBit.nBusy3 )
        return false;
    return true;
}
//---------------------------------------------------------------------------
double CPaixMotion::GetEnCPos(int nAxis)
{
    int nRet;
    double fEncPos;
    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }

    nRet = nmc_get_enc(m_nPaixCtlNo,nAxis+1,&m_lEncPos[nAxis]);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    fEncPos = m_lEncPos[nAxis];
    fEncPos = (fEncPos/nmc_get_resolution(m_nPaixCtlNo,nAxis))*nmc_get_leed(m_nPaixCtlNo,nAxis);
    m_fEncPos[nAxis] = fEncPos;
    return fEncPos;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetEncPos(int nAxis, double fPos)
{
    int nRet;
    long lEncPos;
    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    lEncPos = fPos;
    nRet = nmc_set_enc(m_nPaixCtlNo,nAxis+1,lEncPos);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
double CPaixMotion::GetCmdPos(int nAxis)
{
    int nRet;
    double fPos;
    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_get_cmd(m_nPaixCtlNo,nAxis+1,m_lCmdPos+nAxis);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    fPos = m_lCmdPos[nAxis];                      //NmcData.Cmd[nAxis] * GetLeed(nAxis)/ GetMotorRes(nAxis);
    fPos = (fPos/nmc_get_resolution(m_nPaixCtlNo,nAxis))*nmc_get_leed(m_nPaixCtlNo,nAxis);
    m_fCmdPos[nAxis] = fPos;
    return fPos;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetCmdPos(int nAxis, double fPos)
{
    int nRet = 0 ;
    long lPulse;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    lPulse = fPos;
    nRet = nmc_set_cmd(m_nPaixCtlNo, nAxis+1, lPulse);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
//  false - A 접점  ,   true  - B 접점
bool CPaixMotion::SetEmerLogic(int nLogic)
{
    int nRet = 0 ;

    if( !Remote)
        return false;
    nRet = nmc_es_logic(m_nPaixCtlNo,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
//  false - A 접점  ,   true  - B 접점
bool CPaixMotion::SetZLogic(int nAxis,int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_z_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
//  false - A 접점  ,   true  - B 접점
bool CPaixMotion::SetNearLogic(int nAxis,int nLogic)
{
    int nRet = 0;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_near_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
//  0 - A 접점  ,   1  - B 접점
bool CPaixMotion::SetLimitLogic(int nAxis,int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_limit_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetEmer(void)
{
    int nRet;
    short nValue;
    nRet = nmc_es_signal(m_nPaixCtlNo,&nValue);

    switch( nRet )
    {
    case  NMC_OK :
        if( nValue == 1 )
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetEmer(void)
{
    nmc_stop_motor(m_nPaixCtlNo,1);
    nmc_stop_motor(m_nPaixCtlNo,2);
    nmc_stop_motor(m_nPaixCtlNo,3);
    nmc_stop_motor(m_nPaixCtlNo,4);
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSlowStop(int nAxis)
{
    Stat[nAxis].bJogP = false;
    Stat[nAxis].bJogN = false;
    nmc_sstop_motor(m_nPaixCtlNo,nAxis+1);
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::StopMotor(int nAxis)
{
    Stat[nAxis].bJogP = false;
    Stat[nAxis].bJogN = false;
    nmc_stop_motor(m_nPaixCtlNo,nAxis+1);
    return true;
}
//---------------------------------------------------------------------------
// false - 정 true - 반
bool CPaixMotion::SetMotorDir(int nAxis, bool bDir)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    nRet = nmc_pulse_logic(m_nPaixCtlNo,nAxis+1, bDir);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
/*
    0 : - Limit
    1 : + Limit
    2 : Near Sensor
*/
bool CPaixMotion::SetHomeMode(int nAxis, short nMode)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    int nRet;
    switch( nMode )
    {
    case 0: // + Limit
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x00);
    break;
    case 1: // - Limit
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x01);
    break;
    case 2: // Near +
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x02);
    break;
    case 3: // Near -
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x03);
    break;
    //  엔코더
    case 4: // + Limit
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x00|0x80);
    break;
    case 5: // - Limit
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x01|0x80);
    break;
    case 6: // Near +
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x02|0x80);
    break;
    case 7: // Near -
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x03|0x80);
    break;
    case 8: // Z -
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x05|0x80);
    break;
    case 9: // Z +
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x04|0x80);
    break;
    case 10: // Org +
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x06);
    break;
    case 11: // Org -
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x07);
    break;
    case 12: // Org +
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x06|0x80);
    break;
    case 13: // Org -
            nRet = nmc_home_mode(m_nPaixCtlNo,nAxis+1, 0x07|0x80);
    break;
    }

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
/*
    0 : CCW, 1 : CW
*/
bool CPaixMotion::SetHomeDir(int nAxis, bool bDir)
{
    return true;
}
//---------------------------------------------------------------------------

bool CPaixMotion::AbsMoveWith( int nCount, int * nAxis, double *fDist)
{
    if( !Remote)
    {
        return false;
    }
    int nRet = 0,i,nRot;
    long lDist[4];
    switch( m_nMotionMode[0] )
    {
    case 0: // Pulse
            lDist[i] = fDist[i];
    break;
    case 1:
        for( i = 0; i < nCount;i++)
        {
            nRot = nmc_get_resolution(m_nPaixCtlNo, nAxis[i]);
            lDist[i] = fDist[i] * nRot;
        }
    break;
    case 2: // mm
        for( i = 0; i < nCount;i++)
        {
            lDist[i] = fDist[i] * nmc_get_resolution(m_nPaixCtlNo, nAxis[i]) /nmc_get_leed(m_nPaixCtlNo, nAxis[i]) ;;
        }
    break;
    }

//    nRet = nmc_al_move(m_nPaixCtlNo, nCount, nAxis, lDist);
    switch( nCount )
    {
    case 2:
        nRet = nmc_sync_two_move(m_nPaixCtlNo,nAxis[0],lDist[0],nAxis[1],lDist[1],1);
    break;
    case 3:
        nRet = nmc_sync_three_move(m_nPaixCtlNo,nAxis[0],lDist[0],nAxis[1],lDist[1],nAxis[2],lDist[2],1);
    break;
    }

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::RelMoveWith(  int nCount, int * nAxis, double *fDist)
{
    if( !Remote)
    {
        return false;
    }
    int nRet = 0,i;
    long lDist[4];
    switch( m_nMotionMode[0] )
    {
    case 0: // Pulse
        for( i = 0; i < nCount;i++)
            lDist[i] = fDist[i];
    break;
    case 1:
        for( i = 0; i < nCount;i++)
            lDist[i] = fDist[i] * nmc_get_resolution(m_nPaixCtlNo, nAxis[i]);
    break;
    case 2: // mm
        for( i = 0; i < nCount;i++)
            lDist[i] = fDist[i] * nmc_get_resolution(m_nPaixCtlNo, nAxis[i]) /nmc_get_leed(m_nPaixCtlNo, nAxis[i]) ;;
    break;
    } 
    switch( nCount )
    {
    case 2:
        nRet = nmc_sync_two_move(m_nPaixCtlNo,nAxis[0],lDist[0],nAxis[1],lDist[1],0);
    break;
    case 3:
        nRet = nmc_sync_three_move(m_nPaixCtlNo,nAxis[0],lDist[0],nAxis[1],lDist[1],nAxis[2],lDist[2],0);
    break;
    }       
    //nRet = nmc_il_move(m_nPaixCtlNo, nCount, nAxis, lDist);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetEncInputMode(int nAxis,int nMode)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_encoder_input_mode(m_nPaixCtlNo,nAxis+1,nMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetEncMode(int nAxis, int nLogic)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_encoder_counter_mode(m_nPaixCtlNo,nAxis+1,nLogic);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
int CPaixMotion::GetPLimit(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_for_signal(m_nPaixCtlNo,nAxis+1,&nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }
    return nValue;
    //return nRet;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetMLimit(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_rev_signal(m_nPaixCtlNo,nAxis+1,&nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }
    return nValue;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetNear(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_near_signal(m_nPaixCtlNo,nAxis+1,&nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }
    return nValue;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetInpo(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_inpos_signal(m_nPaixCtlNo,nAxis+1,&nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }
    return nValue;
}
//---------------------------------------------------------------------------
bool CPaixMotion::JogMove(int nAxis, int nDir)
{
    int nRet;

    SetServoOnOff(nAxis , false) ;

    nRet = nmc_infinity_move(m_nPaixCtlNo, nAxis+1, nDir);


    Stat[nAxis].bJogP = (nDir == 0) ;
    Stat[nAxis].bJogN = (nDir == 1) ;

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetPulseLogic(int nAxis, int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_pulse_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetAlarmLogic(int nAxis, int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_alarm_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    if( nRet == NMC_NOTCONNECT)
    {
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetInpoLogic(int nAxis, int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_inpos_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetBusy(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_busy(m_nPaixCtlNo,nAxis+1, &nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }
    return nValue;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetZ(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
	if( m_bRemote == FALSE )
    	return 0;
    nRet = nmc_z_signal(m_nPaixCtlNo,nAxis+1,&nValue);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return 0;
    case 0:
		m_bZ[nAxis] = nValue;

    }
    return nValue;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetOffset(int nAxis, int *offset)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    //nRet = nmc_get_offset(m_nPaixCtlNo,nAxis+1,offset);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------

bool CPaixMotion::SetCmd(int nAxis, long Value)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_set_cmd(m_nPaixCtlNo, nAxis+1, Value);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetEnc(int nAxis, long Value)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_set_enc(m_nPaixCtlNo, nAxis+1, Value);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetMotionType(int nAxis,int nType)
{
    if( !Remote)
       return false;
    int nRet;
    nRet = nmc_set_movemode(m_nPaixCtlNo, nAxis+1,nType);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetAlarm(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_alarm_signal(m_nPaixCtlNo,nAxis+1,&nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nValue;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetError(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
    nRet = nmc_error_signal(m_nPaixCtlNo,nAxis+1,&nValue);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nValue;
}
//---------------------------------------------------------------------------
int CPaixMotion::SetServoOnOff(int nAxis,short OnOff)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_servo_on_off(m_nPaixCtlNo,nAxis+1, OnOff);

    //if(OnOff == false) SetCmdPos(nAxis , GetEnCPos(nAxis)) ;

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nRet;
}
//---------------------------------------------------------------------------
int CPaixMotion::SetAlarmOnOff(int nAxis,short OnOff)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_alarmreset_on_off(m_nPaixCtlNo, nAxis+1, OnOff);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nRet;
}
//---------------------------------------------------------------------------
// 스텝일 경우 CO+, CO- 출력임
int CPaixMotion::SetMotorOnOff(int nAxis,short OnOff)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_motor_on_off(m_nPaixCtlNo, nAxis+1, OnOff);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nRet;
}
//---------------------------------------------------------------------------
int CPaixMotion::SetAbsOnOff(int nAxis,short OnOff)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_set_movemode(m_nPaixCtlNo, nAxis+1, OnOff);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nRet;
}
//---------------------------------------------------------------------------
long CPaixMotion::GetPps(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    long data;
    int nRet;
    nRet = nmc_pps_monitor(m_nPaixCtlNo, nAxis+1,&data);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return data;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetDllVersion(char *pStr)
{
    int nRet,nLen;

    nRet = nmc_get_version(m_nPaixCtlNo,pStr);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }

    //if( strlen(pStr) == 0 )

    return 0;
}
//---------------------------------------------------------------------------

bool CPaixMotion::HomeMove(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_home_move(m_nPaixCtlNo, nAxis+1);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }

//    Stat[nAxis].bHomeStart = true ;
//
//    if(nAxis == Axis1) NmcData.nmcBit.nHome0 = false ;
//    if(nAxis == Axis2) NmcData.nmcBit.nHome1 = false ;

    return nRet;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetHomeDone(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;

    //nRet = nmc_is_home(m_nPaixCtlNo,nAxis+1);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nRet;
}
//---------------------------------------------------------------------------
void CPaixMotion::SetRemote(int bRemote)
{
    m_bRemote = bRemote;
}
//---------------------------------------------------------------------------
bool CPaixMotion::RepeatMove(int nAxis,double lPulse)
{
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::ReadSensorAll(PNMCDATA pNmcData)
{
    int nRet,i;
	if( m_bRemote == FALSE )
    	return false;
    nRet = nmc_get_sensor_all( m_nPaixCtlNo,pNmcData);
    if(nRet != NMC_OK)
    {
        Sleep(0);
        m_bRemote = FALSE;
        return NMC_NOTCONNECT;
    }
    return nRet;
}

//---------------------------------------------------------------------------
bool CPaixMotion::SetLeed(int nAxis,double lLeed)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;

    nmc_set_leed(m_nPaixCtlNo, nAxis+1, lLeed);
    return true;
}
//---------------------------------------------------------------------------
double CPaixMotion::GetLeed(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    double fleed = nmc_get_leed(m_nPaixCtlNo, nAxis+1);
    return fleed;
}
//---------------------------------------------------------------------------
void CPaixMotion::SetMotionMode(int nAxis,int nMode)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return;
    m_nMotionMode[nAxis] = nMode;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetMotionMode(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return 0;
    return m_nMotionMode[nAxis];
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetIniFile(char *pFile)
{
    int nRet;
    if( !Remote)
        return false;

    //nRet = nmc_load_from_file(m_nPaixCtlNo,		// 장치 번호 pFile);	// 파일 이름 : Full Path
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void CPaixMotion::SetRepeatCount(int nAxis,int nCount )
{
}

//---------------------------------------------------------------------------
bool CPaixMotion::SetClock(int nAxis,short nClock)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_select_clock(m_nPaixCtlNo, nAxis+1, nClock);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void CPaixMotion::SetIpAddress(short nField0,short nField1,short nField2)
{
    nmc_set_ipaddress(m_nPaixCtlNo, nField0,nField1,nField2);
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetIpAddress(short *nField0,short *nField1,short *nField2,short *nField3)
{
    int a0,a1,a2,a3 ;
    return nmc_get_ipaddress(&a0,&a1,&a2,&a3);

    *nField0=a0;
    *nField1=a1;
    *nField2=a2;
    *nField3=a3;


}
//---------------------------------------------------------------------------
bool CPaixMotion::AngleNearMove( int nAxis, double fDist)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    long lPulse;
    int nRet = 0;

    lPulse = fDist;// * m_lRes[nAxis]/m_lLead[nAxis];

    nRet = nmc_anglestop_move(m_nPaixCtlNo, nAxis+1, lPulse);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 0:
        break;
    default:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
int CPaixMotion::SetDccOnOff(int nAxis, short OnOff)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    nRet = nmc_dccreset_on_off(m_nPaixCtlNo, nAxis+1, OnOff);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return false;
    }
    return nRet;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetOrgLogic(int nAxis,int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_org_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetOrg(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;
	if( m_bRemote == FALSE )
    	return 0;
    nRet = nmc_org_signal(m_nPaixCtlNo,nAxis+1,&nValue);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return 0;
    case 0:
	    m_bOrg[nAxis] = nValue;
    }
    return nValue;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetSrvReday(int nAxis)
{
    if( nAxis < 0 && nAxis > 4 || !Remote)
       return false;
    int nRet;
    short nValue;

    nRet = nmc_srvready_signal(m_nPaixCtlNo,nAxis+1,&nValue);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
        return 0;
    }
    return nValue;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSSpeedPPS(int nAxis,long lStart, long lAcc,long lMax)
{
    int nRet = 0;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    //nRet = nmc_sspeed_pps(m_nPaixCtlNo,nAxis+1,lStart,lAcc,lMax);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::WithRelMove( int nCount, int * nAxis, double *fDist)
{
    if( !Remote)
    {
        return false;
    }
    int nRet,i;
    long lDist[4];
    switch( m_nMotionMode[0] )
    {
    case 0: // Pulse
        for( i = 0; i < nCount;i++)
            lDist[i] = fDist[i];
    break;
    case 1:
        for( i = 0; i < nCount;i++)
        {
            lDist[i] = fDist[i] * nmc_get_resolution(m_nPaixCtlNo, nAxis[i]);
        }
        nRet = nmc_imul_move(m_nPaixCtlNo, nCount, nAxis,lDist);
    break;
    case 2: // mm
        for( i = 0; i < nCount;i++)
        {
            fDist[i] = fDist[i] * nmc_get_resolution(m_nPaixCtlNo, nAxis[i]) /nmc_get_leed(m_nPaixCtlNo, nAxis[i]) ;;
        }
    break;
    }

    nRet = nmc_imul_move(m_nPaixCtlNo, nCount, nAxis,lDist);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::WithAbsMove( int nCount, int * nAxis, double *fDist)
{
    if( !Remote)
    {
        return false;
    }
    int nRet = 0,i;
    long lDist[4];
    switch( m_nMotionMode[0] )
    {
    case 0: // Pulse
        for( i = 0; i < nCount;i++)
        {
            lDist[i] = fDist[i];
        }
    break;
    case 1:
        for( i = 0; i < nCount;i++)
        {
            lDist[i] = fDist[i] *nmc_get_resolution(m_nPaixCtlNo, nAxis[i]);
        }
        nRet = nmc_imul_move(m_nPaixCtlNo, nCount, nAxis,lDist);
    break;
    case 2: // mm
        for( i = 0; i < nCount;i++)
        {
            lDist[i] = fDist[i] * nmc_get_resolution(m_nPaixCtlNo, nAxis[i]) /nmc_get_leed(m_nPaixCtlNo, nAxis[i]) ;;
        }
    break;
    }
    nRet = nmc_amul_move(m_nPaixCtlNo, nCount, nAxis,lDist);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSampleMove(int nAxis,int nMode,int nCount,long pulse,long delay)
{
    if( !Remote)
    {
        return false;
    }
    int nRet = nmc_runsample(m_nPaixCtlNo, nAxis, nMode, nCount, pulse,delay);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SyncTwoMove( int axis1, long pulse1, int axis2, long pulse2, int opt)
{
    int nRet = 0;

    do
    {
        if( m_bRemote == false )
        {
            Sleep(10);
            m_bRemote = OpenPaixCtl(m_nPaixCtlNo);
        	continue;
        }
        nRet = nmc_sync_two_move(m_nPaixCtlNo, axis1+1, pulse1, axis2+1, pulse2,opt);
    }
    while(nRet != 0);
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SyncThreeMove( int axis1, long pulse1, int axis2, long pulse2, int axis3, long pulse3,int opt)
{
    if( !Remote)
    {
        return false;
    }
    int nRet = 0;

    nRet = nmc_sync_three_move(m_nPaixCtlNo, axis1+1, pulse1, axis2+1, pulse2,axis3+1, pulse3,opt);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::RelCircleMove(short axis1, short axis2,long center0,long center1, long end0,long end1,short dir)
{
    int nRet = 0;

    do
    {
        if( m_bRemote == false )
        {
            Sleep(10);
            m_bRemote = OpenPaixCtl(m_nPaixCtlNo);
        	continue;
        }
	    nRet = nmc_ici_move(m_nPaixCtlNo,					// 장치 번호
						axis1+1, axis2+1,		// 원호 보간을 할 축
                        center0, center1,	// 원의 중심점
                        end0, end1,			// 축의 원호 끝점
                        dir);						// 0 : CW 1 : CCW
    }
    while( nRet != 0);
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::AbsCircleMove(short axis1, short axis2,long center0,long center1, long end0,long end1,short dir)
{ 
    if( !Remote)
    {
        return false;
    }
    int nRet = 0;
	nRet = nmc_aci_move(m_nPaixCtlNo,					// 장치 번호
						axis1+1, axis2+1,		// 원호 보간을 할 축
                        center0, center1,	// 원의 중심점
                        end0, end1,			// 축의 원호 끝점
                        dir);						// 0 : CW 1 : CCW
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetCmdAll( long *pos)
{
    int nRet = 0;
    int nCount = 0;
    do
    {
    	nRet = nmc_get_cmd_all(m_nPaixCtlNo,pos);
        if( nRet != 0 )
        {
            m_bRemote = OpenPaixCtl(m_nPaixCtlNo);
            continue;
        }
        else
            return true;
    }
    while( nCount < 3 ) ;
    return false;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetHomeflag( long *homeflag)
{
    int nRet = 0;
    int nCount = 0;
    do
    {
    	nRet = nmc_get_homeflag(m_nPaixCtlNo, homeflag);
        if( nRet != 0 )
        {
            m_bRemote = OpenPaixCtl(m_nPaixCtlNo);
            continue;
        }
        else
            return true;
    }
    while( nCount < 3 );
    return false;
}
//---------------------------------------------------------------------------
int CPaixMotion::GetPortNum()
{
    return nmc_get_portnum(m_nPaixCtlNo);
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetMinusLimitLogic(int nAxis,int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_minus_limit_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetPlusLimitLogic(int nAxis,int nLogic)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_plus_limit_logic(m_nPaixCtlNo,nAxis+1,nLogic);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSwPlusLimitOnff(int nAxis,short onoff, long pulse)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_sw_plus_limit_onoff(m_nPaixCtlNo,nAxis+1,onoff,pulse);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSwMinusLimitOnff(int nAxis,short onoff, long pulse)
{
    int nRet = 0 ;

    if( nAxis < 0 && nAxis > 4 || !Remote)
    {
        return false;
    }
    nRet = nmc_sw_minus_limit_onoff(m_nPaixCtlNo,nAxis+1,onoff,pulse);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::GetLogicAll(PNMCLOGICBIT pLogicBit)
{
    int nRet = 0 ;
    nRet = nmc_get_logic_all(m_nPaixCtlNo,pLogicBit);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetLogicAll(PNMCLOGICBIT pLogicBit)
{
    int nRet = 0 ;
    nRet = nmc_set_logic_all(m_nPaixCtlNo,pLogicBit);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::GetOutput(BYTE *value)
{
	int nRet;
	if( pPaix == NULL)
    	return false;;
    nRet = nmc_get_output8(m_nPaixCtlNo,value);
    switch( nRet )
    {
    case -1:
    	return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::SetOutput(BYTE value)
{
	int nRet;
	if( pPaix == NULL)
    	return false;;
    nRet = nmc_output8(m_nPaixCtlNo,value);
    switch( nRet )
    {
    case -1:
    	return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::SetOutputBit(int bitno,BYTE value)
{
	int nRet;
	if( pPaix == NULL)
    	return false;;
    nRet = nmc_output8_bit(m_nPaixCtlNo,bitno,value);
    switch( nRet )
    {
    case -1:
    	return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool CPaixMotion::SetSCurveExt(int nAxis,long lStart, long lAcc,long lAccJerk,long lMax,long lDec,long lDecJerk)
{
    int nRet = 0;
    if( nAxis < 0 && nAxis > 4 || !Remote)
        return false;
    nRet = nmc_scurve_ext(m_nPaixCtlNo,nAxis+1,lStart, lAcc,lDec, lMax,lAccJerk, lDecJerk);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        TRACE("SPeed Error");
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::SetPoint(short nLineNo,short nMode, long lDelay,
						   short axis1,long lpulse1,short axis2,long lpulse2,
						   short axis3,long lpulse3,short axis4,long lpulse4)
{
    int nRet = 0;
	nRet = nmc_set_point(m_nPaixCtlNo,nLineNo,nMode,lDelay,
						   axis1,lpulse1,axis2,lpulse2,axis3,lpulse3,axis4,lpulse4);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
	/*	MODE
		0 : POINT MOVE STOP
		1 : 0 LINE START
		2 : Current LINE START
		3 : Total Line Clear & STOP
	*/
bool __fastcall CPaixMotion::MovePoint( short nMode)
{
    int nRet = 0;
    nRet = nmc_run_point(m_nPaixCtlNo,nMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::GetPointMoveStatus(short *signal)
{
    int nRet = 0;
	nRet = nmc_pointmove_status(m_nPaixCtlNo,signal);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::GetPointInfo(short nLineNo, POINTDATA *pointdata)
{
    int nRet = 0;
	nRet = nmc_get_point_info(m_nPaixCtlNo,nLineNo,pointdata);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::GetPointCurrentLine(short *lineno)
{
    int nRet = 0;
    nRet = nmc_point_currentline(m_nPaixCtlNo, lineno);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::RunPointNo( short nLineNo)
{
    int nRet = 0;
    nRet = nmc_run_lineno(m_nPaixCtlNo,nLineNo);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void CPaixMotion::GetStatus(PNMCDATA pnmcData)
{
    *pnmcData = nmcData;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::MovePointNo( short nLineNo)
{
    int nRet = 0;
    nRet = nmc_move_point(m_nPaixCtlNo,nLineNo);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::PulseOutInverse( int nAxis, bool binv)
{
    int nRet = 0;
    nRet = nmc_twopulse_dir(m_nPaixCtlNo,nAxis+1, binv);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::OnePulseDir(int nAxis, int nDir)
{
    int nRet = 0;
    nRet = nmc_onepulse_dir(m_nPaixCtlNo,nAxis+1, nDir);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;

}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::MpgMode(int nAxis, int nMode)
{
    int nRet = 0;
    nRet = nmc_mpg_mode(m_nPaixCtlNo,nAxis+1, nMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;

}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::HwLimitStopMode(int nAxis, int nMode)
{
    int nRet = 0;
    nRet = nmc_limit_stopmode(m_nPaixCtlNo,nAxis+1, nMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;

}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::CompCompareMode(int nAxis, int nMode)
{
    int nRet = 0;
    nRet = nmc_comp_compare(m_nPaixCtlNo,nAxis+1, nMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;

}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::BusyOffMode(int nAxis, int nMode)
{
    int nRet = 0;
    nRet = nmc_endbusy_mode(m_nPaixCtlNo,nAxis+1, nMode);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;

}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::SetInpoRange(int nAxis, short enable, short time, short repeatcount,long inporange)
{
    int nRet = 0;
    nRet = nmc_inporange_enable(m_nPaixCtlNo, nAxis, enable, time, repeatcount,inporange,100,0);
    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::ClearInpoError(int nAxis)
{
    int nRet = 0;
    nRet = nmc_clear_inpo_error(m_nPaixCtlNo, nAxis);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall CPaixMotion::GetInpoError(long *inpoerror)
{
    int nRet = 0;
    nRet = nmc_get_inpo_error(m_nPaixCtlNo, inpoerror);

    switch( nRet )
    {
    case NMC_NOTCONNECT:
    	m_bRemote = FALSE;
    case 1:
    case 2:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------

int  __fastcall CPaixMotion::GetStopInfo(void)
{
    int nRet = 0;
    nRet = nmc2_get_stop_info(m_nPaixCtlNo,&NmcStatusBit);

    return nRet;
}
//---------------------------------------------------------------------------
void __fastcall CPaixMotion::Execute()
{
    int nRet;
    char str[64];
    GetCpuTickInit();
    while( !Terminated )
    {
        Sleep(0);
        if( m_bRemote == false )
        {
            m_bRemote = OpenPaixCtl(m_nPaixCtlNo);
            //PostMessage(frmMain->Handle,PAIX_SENSORREAD,PAIX_NOTCONNECT,0L);
            continue;
        }
        GetCpuTickStart();
        nRet = ReadSensorAll(&nmcData);
        GetStatus(&NmcData);
        GetCpuTickEnd();
        if( nRet < 0 )
        {
            SendMessage(FrmMain->Handle,PAIX_SENSORREAD,PAIX_NOTCONNECT,nRet);
            sprintf(str,"QALL ERROR: %d",nRet);
	    OutputDebugString(str);
            continue;
        }

        SendMessage(FrmMain->Handle,PAIX_SENSORREAD,PAIX_TIME,GetCpuTickGetTime()*10000);
        SendMessage(FrmMain->Handle,PAIX_SENSORREAD,PAIX_SENSORREAD,(LPARAM)&nmcData);
        nRet = GetStopInfo();
        if( nRet == 0 )
        {
            SendMessage(FrmMain->Handle,PAIX_SENSORREAD,PAIX_STOP_INFO,0);
        }

        //Update.
        for(int i = 0 ; i < MAX_MOTR ; i++) {
            if(Stat[i].bHomeStart ) HomeProc (i);
            if(Stat[i].bShaking   ) ShakeProc(i);
            if(Stat[i].bOverRide  ) OR_Proc  (i);
            if(NmcData.nmcBit.nEmer) {
                Stat[i].bHomeDone = false ;
                StopMotor(i);
            }
        }

        if(!Stat[Axis1].iHomeStep) {
            if(NmcData.nmcBit.nPLimit0 && !Stat[Axis1].bJogN){
                //Stat[Axis1].bHomeDone = falsStat[Axis1].bJogN = false;e ;
                StopMotor(Axis1);

            }
            if(NmcData.nmcBit.nNLimit0 && !Stat[Axis1].bJogP){
                //Stat[0].bHomeDone = false ;
                StopMotor(Axis1);

            }
        }

        if(!Stat[Axis2].iHomeStep) {
            if(NmcData.nmcBit.nPLimit1 && !Stat[Axis2].bJogN){
                //Stat[1].bHomeDone = false ;
                StopMotor(Axis2);
            }
            if(NmcData.nmcBit.nNLimit1 && !Stat[Axis2].bJogP){
                //Stat[1].bHomeDone = false ;
                StopMotor(Axis2);
            }
        }
    }

    ClosePaixCtl();
}


void __fastcall CPaixMotion::SetRotorPara()
{
    SetEmerLogic    (0          ) ; //0:NC(nomal closed) 1:NO(nomal open)

    SetMotorRes     (Axis1,PULSE_PER_REV) ; //Yaskawa 32768 2^17  =>분해능안나와 16384
    SetLeed         (Axis1,360  ) ; //360도
    SetEncCountMode (Axis1,2    ) ; //0:4체배 1:2체배 2:1체배
    SetZLogic       (Axis1,1    ) ; //0:NC 1:NO
    SetEncInputMode (Axis1,0    ) ; //0:EA|EB 1:EB|EA 2:CW|CCW 3:CCW|CW
    SetNearLogic    (Axis1,0    ) ; //0:NC 1:NO
    SetLimitLogic   (Axis1,0    ) ; //0:NC 1:NO
    SetAlarmLogic   (Axis1,1    ) ; //0:NC 1:NO
    PulseOutInverse (Axis1,0    ) ; //0:CCW|CW 1:CW|CCW
    SetPulseLogic   (Axis1,1    ) ; //0:부논리 1:정논리
    SetHomeMode     (Axis1,8    ) ; //0:+ Limit 1:- Limit 2:+Near 3:-Near 8 : -Z
    SetClock        (Axis1,0    ) ; //0:2Clock 1:1Clock
    SetOrgLogic     (Axis1,1    ) ; //0:NC 1:NO
    SetInpoLogic    (Axis1,1    ) ; //0:NC 1:NO

    SetCmd          (Axis1,0.0  );
    SetEnc          (Axis1,0.0  );
    SetServoOnOff   (Axis1,false);

    SetSpeedDist    (Axis1,1, 3800,360,0 );    //StartSpeed , Acc , Speed , Jerk

    SetMotionMode   (Axis1,2              );    //0:Pulse 1:RPM 2:Position

    SetMotorRes     (Axis2,5000 ) ; //드라이버에서 설정. 5000
    SetLeed         (Axis2,2    ) ; //2mm
    SetEncCountMode (Axis2,0    ) ; //0:4체배 1:2체배 2:1체배
    SetZLogic       (Axis2,0    ) ; //0:NC 1:NO
    SetEncInputMode (Axis2,0    ) ; //0:EA|EB 1:EB|EA 2:CW|CCW 3:CCW|CW
    SetNearLogic    (Axis2,0    ) ; //0:NC 1:NO
    SetLimitLogic   (Axis2,1    ) ; //0:NC 1:NO
    SetAlarmLogic   (Axis2,0    ) ; //0:NC 1:NO
    PulseOutInverse (Axis2,0    ) ; //0:CCW|CW 1:CW|CCW
    SetPulseLogic   (Axis2,0    ) ; //0:부논리 1:정논리
    SetHomeMode     (Axis2,2    ) ; //0:+ Limit 1:- Limit 2:+Near 3:-Near  이건 왜 이럴까?
    SetClock        (Axis2,0    ) ; //0:2Clock 1:1Clock
    SetOrgLogic     (Axis2,0    ) ; //0:NC 1:NO
    SetInpoLogic    (Axis2,1    ) ; //0:NC 1:NO

    SetCmd          (Axis2,0.0  );
    SetEnc          (Axis2,0.0  );
    SetMotorOnOff   (Axis2,true );

    SetSpeedDist    (Axis2,1, 20 , 10 , 0 );    //StartSpeed , Acc , Speed , Jerk //10이하면 삽질함.
    SetMotionMode   (Axis2,2               );    //0:Pulse 1:RPM 2:Position

//    nmc_speed_pps   (20,Axis1,40, 360 , 3800*PULSE_PER_REV );    //StartSpeed , Acc , Speed , Jerk
    SetSpeedDist    (Axis1,1, 3800,360,0 );    //StartSpeed , Acc , Speed , Jerk
//bool CPaixMotion::SetSpeedDist(int nAxis,double lStart, double lAcc,double lMax,long lJerk)





/*
    SetEmerLogic    (1          ) ; //0:NC(nomal closed) 1:NO(nomal open)

    SetMotorRes     (Axis1,PULSE_PER_REV) ; //Yaskawa 32768 2^17
    SetLeed         (Axis1,360  ) ; //360도
    SetEncCountMode (Axis1,2    ) ; //0:4체배 1:2체배 2:1체배
    SetZLogic       (Axis1,1    ) ; //0:NC 1:NO
    SetEncInputMode (Axis1,0    ) ; //0:EA|EB 1:EB|EA 2:CW|CCW 3:CCW|CW
    SetNearLogic    (Axis1,0    ) ; //0:NC 1:NO
    SetLimitLogic   (Axis1,0    ) ; //0:NC 1:NO
    SetAlarmLogic   (Axis1,1    ) ; //0:NC 1:NO
    PulseOutInverse (Axis1,0    ) ; //0:CCW|CW 1:CW|CCW
    SetPulseLogic   (Axis1,1    ) ; //0:부논리 1:정논리
    SetHomeMode     (Axis1,8    ) ; //0:+ Limit 1:- Limit 2:+Near 3:-Near 8 : -Z
    SetClock        (Axis1,0    ) ; //0:2Clock 1:1Clock
    SetOrgLogic     (Axis1,0    ) ; //0:NC 1:NO
    SetInpoLogic    (Axis1,0    ) ; //0:NC 1:NO

    SetCmd          (Axis1,0.0  );
    SetEnc          (Axis1,0.0  );
    SetServoOnOff   (Axis1,false);

    SetSpeedDist    (Axis1,40, 3800,360,0 );    //StartSpeed , Acc , Speed , Jerk

    SetMotionMode   (Axis1,2              );    //0:Pulse 1:RPM 2:Position

    SetMotorRes     (Axis2,5000 ) ; //드라이버에서 설정. 5000
    SetLeed         (Axis2,2    ) ; //2mm
    SetEncCountMode (Axis2,0    ) ; //0:4체배 1:2체배 2:1체배
    SetZLogic       (Axis2,0    ) ; //0:NC 1:NO
    SetEncInputMode (Axis2,0    ) ; //0:EA|EB 1:EB|EA 2:CW|CCW 3:CCW|CW
    SetNearLogic    (Axis2,1    ) ; //0:NC 1:NO
    SetLimitLogic   (Axis2,1    ) ; //0:NC 1:NO
    SetAlarmLogic   (Axis2,0    ) ; //0:NC 1:NO
    PulseOutInverse (Axis2,0    ) ; //0:CCW|CW 1:CW|CCW
    SetPulseLogic   (Axis2,1    ) ; //0:부논리 1:정논리
    SetHomeMode     (Axis2,2    ) ; //0:+ Limit 1:- Limit 2:+Near 3:-Near  이건 왜 이럴까?
    SetClock        (Axis2,0    ) ; //0:2Clock 1:1Clock
    SetOrgLogic     (Axis2,0    ) ; //0:NC 1:NO
    SetInpoLogic    (Axis2,1    ) ; //0:NC 1:NO

    SetCmd          (Axis2,0.0  );
    SetEnc          (Axis2,0.0  );
    SetMotorOnOff   (Axis2,true );

    SetSpeedDist    (Axis2,40, 20 , 4 , 0 );    //StartSpeed , Acc , Speed , Jerk
    SetMotionMode   (Axis2,2              );    //0:Pulse 1:RPM 2:Position

//    nmc_speed_pps   (20,Axis1,40, 360 , 3800*PULSE_PER_REV );    //StartSpeed , Acc , Speed , Jerk
    SetSpeedDist    (Axis1,40, 3800,360,0 );    //StartSpeed , Acc , Speed , Jerk
//bool CPaixMotion::SetSpeedDist(int nAxis,double lStart, double lAcc,double lMax,long lJerk)
*/
}

//---------------------------------------------------------------------------
void __fastcall CPaixMotion::MoveRPM(int nAxis,int RPM , int Acc_mSec , int Rot_mSec)
{
     if( nAxis < 0 && nAxis >= MAX_MOTR || !Remote) return ;

     //SetServoOnOff(nAxis , false) ;

     bool bNeg = RPM < 0 ;

     if(bNeg) RPM = abs(RPM);

     double RPS = RPM  / 60.0 ;                    //Rotation per Second.
     double APS = RPS * 360 ;                      //Angle    per Second. 잠시 수정.
     double PPS = RPS * PULSE_PER_REV;             //16384

     int    MaxAPS = 36000 ;
     int    MaxPPS = 100 * PULSE_PER_REV ;

     double Acc_s = Acc_mSec/1000.0 ;
     int    AccRate     =  APS/Acc_s ;
     int    AccRateP    =  PPS/Acc_s ;

     int    MaxAccRate  =  MaxAPS/Acc_s ;
     int    MaxAccRateP =  MaxPPS/Acc_s ;

     TotalAng = APS * (Acc_mSec+ Rot_mSec)/1000.0 ;

//     SetSpeedDist    (nAxis,40, AccRate , APS ,0 );  //잠시 수정.
     SetSpeedDist    (nAxis,1, MaxAccRate , MaxAPS ,0 );  //잠시 수정.
//     SetSpeedPPS     (nAxis,40, AccRateP, PPS ,0 );
     SetSpeedPPS     (nAxis,1, MaxAccRateP, MaxPPS ,0 );

//     SetRpm          (nAxis,40, AccRateP, PPS ,0 );

     if(bNeg) TotalAng =  -TotalAng ;

     SetDriveSpeed(nAxis , RPM );

     RelMove         (nAxis,TotalAng);



//     Sleep(1000);

//     nmc_drive_pps(m_nPaixCtlNo, nAxis+1, PPS);

}
void __fastcall CPaixMotion::OR_MoveRPM(int nAxis,int RPM , int Acc_mSec , int Rot_mSec)
{
     if( nAxis < 0 && nAxis >= MAX_MOTR || !Remote) return ;

     //SetServoOnOff(nAxis , false) ;

     bool bNeg = RPM < 0 ;

     if(bNeg) RPM = abs(RPM);

     double RPS = RPM  / 60.0 ;                    //Rotation per Second.
     double APS = RPS * 360 ;                      //Angle    per Second. 잠시 수정.
     double PPS = RPS * PULSE_PER_REV;             //16384

     int    MaxAPS = 36000 ;
     int    MaxPPS = 100 * PULSE_PER_REV ;

     double Acc_s = Acc_mSec/1000.0 ;
     int    AccRate     =  APS/Acc_s ;
     int    AccRateP    =  PPS/Acc_s ;

     int    MaxAccRate  =  MaxAPS/Acc_s ;
     int    MaxAccRateP =  MaxPPS/Acc_s ;

//     TotalAng = APS * (Acc_mSec+ Rot_mSec)/1000.0 ;
     TotalAng = APS * (Rot_mSec)/1000.0 ;

//     SetSpeedDist    (nAxis,40, AccRate , APS ,0 );  //잠시 수정.
     SetSpeedDist    (nAxis,1, MaxAccRate , MaxAPS ,0 );  //잠시 수정.
//     SetSpeedPPS     (nAxis,40, AccRateP, PPS ,0 );
     SetSpeedPPS     (nAxis,1, MaxAccRateP, MaxPPS ,0 );

//     SetRpm          (nAxis,40, AccRateP, PPS ,0 );

     if(bNeg) TotalAng =  -TotalAng ;

     SetDriveSpeed(nAxis , RPM );

     RelMove         (nAxis,TotalAng);



//     Sleep(1000);

//     nmc_drive_pps(m_nPaixCtlNo, nAxis+1, PPS);

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

    if(nAxis == Axis1) {
        switch(Stat[nAxis].iHomeStep) {

            default : Stat[nAxis].iHomeStep = 0 ; break ;

            case 10 : SetSpeedDist    (nAxis,1, 360,36,0 );    //StartSpeed , Acc , Speed , Jerk
                      SetMotionMode   (nAxis,2              );    //0:Pulse 1:RPM 2:Position
                      SetMotorRes     (nAxis,PULSE_PER_REV) ; //Yaskawa 32768 2^17
                      SetLeed         (nAxis,360  ) ; //360도
                      SetServoOnOff   (nAxis,false);
                      JogMove         (nAxis,CCW  );

                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 11 : //if(!nmcData.nmcBit.nOrg0)   return false ;
                      if(!nmcData.nmcBit.nOrg0)   return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 12 : if(nmcData.nmcBit.nBusy0)    return false ;
                      SetSpeedDist    (nAxis,1, 360,3,0 );    //StartSpeed , Acc , Speed , Jerk
                      JogMove (nAxis,CW  );
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 13 : if(nmcData.nmcBit.nOrg0) return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 14 : if(nmcData.nmcBit.nBusy0)    return false ;
                      SetSpeedDist    (nAxis,1, 1800,360,0 );    //StartSpeed , Acc , Speed , Jerk
                      SetEnc(nAxis , 0.0) ;
                      SetCmd(nAxis , 0.0) ;
                      Stat[nAxis].iHomeStep  = 0 ;
                      Stat[nAxis].bHomeStart = false ;
                      Stat[nAxis].bHomeDone  = true  ;
                      m_pMsgFunc("Axis1 HomeDone");
                      return true ;
        }

    }

/*    else if (nAxis == Axis2) {
        switch(Stat[nAxis].iHomeStep) {

            default : Stat[nAxis].iHomeStep = 0 ; break ;

            case 10 : SetSpeedDist    (nAxis,40, 20 , 4 , 0 );    //StartSpeed , Acc , Speed , Jerk
                      SetMotionMode   (nAxis,2              );    //0:Pulse 1:RPM 2:Position
                      SetMotorRes     (nAxis,5000 ) ; //드라이버에서 설정. 5000
                      SetLeed         (nAxis,2    ) ; //2mm
                      SetMotorOnOff   (nAxis,true );
                      JogMove         (nAxis,CCW  );

                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 11 : if( nmcData.nmcBit.nNLimit1) JogMove (nAxis,CW ); //StopMotor(nAxis); 후에 JogMove 하던것 수정 애가 조그안하고 멈춰 있음.
                      if(!nmcData.nmcBit.nNear1  ) return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 12 : if(nmcData.nmcBit.nBusy1) return false ;
                      SetSpeedDist (nAxis,40, 20 , 1 ,0 );    //StartSpeed , Acc , Speed , Jerk
                      JogMove (nAxis,CW  );
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 13 : if(nmcData.nmcBit.nNear1) return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 14 : if(nmcData.nmcBit.nBusy1)    return false ;
                      SetSpeedDist (nAxis,40, 20,4,0 );    //StartSpeed , Acc , Speed , Jerk
                      SetEnc(nAxis , 0.0) ;
                      SetCmd(nAxis , 0.0) ;
                      Stat[nAxis].iHomeStep = 0 ;
                      Stat[nAxis].bHomeStart = false ;
                      Stat[nAxis].bHomeDone  = true  ;
                      m_pMsgFunc("Axis2 HomeDone");
                      return true ;
        }

    }
*/
    else if (nAxis == Axis2) {
        switch(Stat[nAxis].iHomeStep) {

            default : Stat[nAxis].iHomeStep = 0 ; break ;

            case 10 : SetSpeedDist    (nAxis,1, 20 , 4 , 0 );    //StartSpeed , Acc , Speed , Jerk
                      SetMotionMode   (nAxis,2              );    //0:Pulse 1:RPM 2:Position
                      SetMotorRes     (nAxis,5000 ) ; //드라이버에서 설정. 5000
                      SetLeed         (nAxis,2    ) ; //2mm
                      SetMotorOnOff   (nAxis,true );
                      JogMove         (nAxis,CCW  );

                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 11 : if(!nmcData.nmcBit.nNLimit1)return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 12 : if(nmcData.nmcBit.nBusy1) return false ;
                      SetSpeedDist (nAxis,1, 20 , 1 ,0 );    //StartSpeed , Acc , Speed , Jerk
                      JogMove (nAxis,CW  );
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 13 : if(nmcData.nmcBit.nNLimit1) return false ;
                      StopMotor(nAxis);
                      Stat[nAxis].iHomeStep++ ;
                      return false ;

            case 14 : if(nmcData.nmcBit.nBusy1)    return false ;
                      SetSpeedDist (nAxis,1, 20,4,0 );    //StartSpeed , Acc , Speed , Jerk
                      SetEnc(nAxis , 0.0) ;
                      SetCmd(nAxis , 0.0) ;
                      Stat[nAxis].iHomeStep = 0 ;
                      Stat[nAxis].bHomeStart = false ;
                      Stat[nAxis].bHomeDone  = true  ;
                      m_pMsgFunc("Axis2 HomeDone");
                      return true ;
        }

    }
}

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

/*
bool CPaixMotion::SetDriveSpeed(int nAxis , int RPM , int Acc_mSec , int Rot_mSec)
{


    bool bNeg = RPM < 0 ;
    int nRet ;

    if(bNeg) RPM = abs(RPM);

    double RPS = RPM  / 60.0 ;                    //Rotation per Second.
    double APS = RPS * 360 ;                      //Angle    per Second. 잠시 수정.
    double PPS = RPS * PULSE_PER_REV;

    double Acc_s = Acc_mSec/1000.0 ;
    int    AccRate =  APS/Acc_s ;
    int    AccRateP = PPS/Acc_s ;

    TotalAng = APS * (Acc_mSec+ Rot_mSec)/1000.0 ;

    if(bNeg) TotalAng =  -TotalAng ;

    RelMove         (nAxis,TotalAng);

    nmc_drive_pps(m_nPaixCtlNo, nAxis+1, PPS);

    return true;

}*/
