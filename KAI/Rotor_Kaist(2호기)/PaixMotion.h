//---------------------------------------------------------------------------
#ifndef PaixMotionH
#define PaixMotionH
#include "NMC410S.h"
#include <stdio.h>
#include "global.h"
#include "Timer.h"

#define PAIX_NOTCONNECT     (WM_USER+1000)
#define PAIX_SENSORREAD     (WM_USER+1001)
#define PAIX_TIME 	    (WM_USER+1002)
#define PAIX_OUTSTATUS      (WM_USER+1003)
#define PAIX_PATTERN_CNT    (WM_USER+1004)
#define PAIX_STOP_INFO      (WM_USER+1005)
#define PAIX_TARGET_INFO    (WM_USER+1006)
#define PAIX_IO_INPUT 	    (WM_USER+1007)
#define PAIX_IO_OUTPUT 	    (WM_USER+1008)

#define IO_A 0//  false - A 접점  ,   true  - B 접점
#define IO_B 1

#define CCW 1
#define CW 0

#define Axis1 0
#define Axis2 1
#define Axis3 2
#define Axis4 3


#define MAX_MOTR 2
#define MAX_ACTR 2
//#define PULSE_PER_REV 32768   //야스카와 최대 분해능이 안나와 속도가 2000rpm이상 안된다.
                                //미쯔비시도 그냥 16384에 맞춰서 쓴다.
#define PULSE_PER_REV 2880 //1440
#define MOTOR_CONVERSION 2
#define MAX_DriveAxis 4
typedef void (__closure *PaixMsgFunc)(AnsiString);
//---------------------------------------------------------------------------
class CPaixMotion:public TThread
{
public:
    __fastcall CPaixMotion(void);
    __fastcall virtual ~CPaixMotion(void);

private:    // 내부 변수
    BOOL m_bRemote;

    int m_nPaixCtlNo;
    int m_nMotionMode[MAX_DriveAxis];

    void __fastcall Execute();
    void SetRemote(int bRemote);

public:
    bool m_bMLimit  [MAX_DriveAxis], m_bPLimit[MAX_DriveAxis],
         m_bZ       [MAX_DriveAxis], m_bNear  [MAX_DriveAxis],
         m_bInpo    [MAX_DriveAxis], m_bAlarm [MAX_DriveAxis],
         m_bError   [MAX_DriveAxis], m_bBusy  [MAX_DriveAxis],
         m_bOrg     [MAX_DriveAxis];
    bool m_bHomeDone[MAX_DriveAxis];
    long m_lPPS     [MAX_DriveAxis];
    long m_lEncPos  [MAX_DriveAxis], m_lCmdPos[MAX_DriveAxis];
    bool m_bEmer;
    NMCDATA nmcData;
/*
    bool m_bAxis1Start;
    bool m_bAxis2Start;

    bool m_bAxis1Stop;
    bool m_bAxis2Stop;
*/
    double m_fEncPos[MAX_DriveAxis], m_fCmdPos[MAX_DriveAxis];
public:
    int GetDllVersion(char *pStr);
    bool SetPaixCtlNo(int nPaixCtlNo);
    bool OpenPaixCtl(int nPaixCtlNo);
    bool ClosePaixCtl(void);

    bool SetRpm(int nAxis,long lStart, long lAcc,long lMax,long lJerk=0);
    bool SetSpeedPPS(int nAxis,long lStart, long lAcc,long lMax,long lJerk = 0);
    bool SetSpeedDist(int nAxis,double lStart, double lAcc,double lMax,long lJerk = 0);
    bool SetMotorMode( int nAxis,int nMotorMode);
    bool SetMotorRes( int nAxis,long lRes);
    double GetMotorRes( int nAxis);
    bool SetLeed( int nAxis,double lLeed);
    bool SetEncCountMode( int nAxis,int nEncMode);
    bool SetClock(int nAxis,short nClock);
    bool SetHomeMode(int nAxis, short nMode);
    bool SetHomeDir(int nAxis, bool bDir);
    bool SetMotorDir(int nAxis, bool bDir);

    bool AbsMove( int nAxis, double fDist);
    bool RelMove( int nAxis, double fDist);
    bool AngleNearMove( int nAxis, double fDist);
    bool JogMove(int nAxis, int nDir);
    bool HomeMove(int nAxis);
    bool RepeatMove(int nAxis,double lPulse);



    //bool MoveActr(int nActr, bool _bFwd);
    
    double GetEnCPos(int nAxis);
    bool SetEncPos(int nAxis, double fPos);
    bool SetEncInputMode(int nAxis,int nMode = 3);

    double GetCmdPos(int nAxis);
    bool SetCmdPos(int nAxis, double fPos);

    bool MotionDone(int nAxis);
    bool MotionDoneAll(void);

    bool SetEmerLogic(int nLogic);
    bool SetZLogic(int nAxis,int nLogic);
    bool SetOrgLogic(int nAxis,int nLogic);
    bool SetNearLogic(int nAxis,int nLogic);
    bool SetLimitLogic(int nAxis,int nLogic);
    bool SetPlusLimitLogic(int nAxis,int nLogic);
    bool SetMinusLimitLogic(int nAxis,int nLogic);
    bool SetPulseLogic(int nAxis, int nLogic);
    bool SetAlarmLogic(int nAxis, int nLogic);
    bool SetInpoLogic(int nAxis, int nLogic);
    bool SetEncMode(int nAxis, int nLogic = 0);

    bool GetEmer(void);
    bool SetEmer(void);
    bool StopMotor(int nAxis);
    bool SetSlowStop(int nAxis);

    bool RelMoveWith( int nCount, int * nAxis, double *fDist);
    bool AbsMoveWith( int nCount, int * nAxis, double *fDist);

    int GetPLimit(int nAxis);
    int GetMLimit(int nAxis);
    int GetNear(int nAxis);
    int GetBusy(int nAxis);
    int GetZ(int nAxis);
    int GetOrg(int nAxis);
    int GetInpo(int nAxis);
    int GetAlarm(int nAxis);
    int GetError(int nAxis);
    int GetHomeDone(int nAxis);
    int GetSrvReday(int nAxis);
    
    bool ReadSensorAll(PNMCDATA pNmcData);
    bool PaixInit(int nPaixCtlNo);
    bool GetOffset(int nAxis, int *offset);
    bool SetCmd(int nAxis, long Value);
    bool SetEnc(int nAxis, long Value);
    bool SetMotionType(int nAxis,int nType);
    void SetMotionMode(int nAxis,int nMode);
    int GetMotionMode(int nAxis);

    int SetServoOnOff(int nAxis,short OnOff);
    int SetAlarmOnOff(int nAxis,short OnOff);
    int SetMotorOnOff(int nAxis,short OnOff);
    int SetAbsOnOff(int nAxis,short OnOff);
    int SetDccOnOff(int nAxis, short OnOff);

    long GetPps(int nAxis);
    bool GetConnect(void){return m_bRemote;};

    bool SetIniFile(char *pFile);
    void SetIpAddress(short nField0,short nField1,short nField2);
    bool GetIpAddress(short *nField0,short *nField1,short *nField2,short *nField3);
    int GetPortNum();

    __property int Remote= {read=m_bRemote,write=SetRemote};


    void SetRepeatCount(int nAxis,int nCount = 20 );
	bool SetSSpeedPPS(int nAxis,long lStart, long lAcc,long lMax);
    bool SetSCurveExt(int nAxis,long lStart, long lAcc,long lAccJerk,long lMax,long lDec,long lDecJerk);
    bool WithRelMove( int nCount, int * nAxis, double *fDist);
    bool WithAbsMove( int nCount, int * nAxis, double *fDist);
    bool SetSampleMove(int nAxis,int nMode,int nCount = 20,long pulse = 10000,long delay=50 );
    bool SyncTwoMove( int axis1, long pulse1, int axis2, long pulse2, int opt = 0);
    bool SyncThreeMove( int axis1, long pulse1, int axis2, long pulse2, int axis3, long pulse3,int opt = 0);
    bool RelCircleMove(short axis1,short axis2, long center0,long center1, long end0,long end1,short dir = 0);
    bool AbsCircleMove(short axis1,short axis2, long center0,long center1, long end0,long end1,short dir = 0);
    bool GetCmdAll( long *pos);
    bool GetHomeflag( long *homeflag);
    bool SetSwMinusLimitOnff(int nAxis,short onoff, long pulse);
    bool SetSwPlusLimitOnff(int nAxis,short onoff, long pulse);
    bool GetLogicAll(PNMCLOGICBIT pLogicBit);
    bool SetLogicAll(PNMCLOGICBIT pLogicBit);
    
    bool __fastcall GetOutput(BYTE *value);
    bool __fastcall SetOutput(BYTE value);
    bool __fastcall SetOutputBit(int bitno,BYTE value);

    bool __fastcall SetPoint(short nLineNo, short nMode, long lDelay,
						   short axis1,long lpulse1,
						   short axis2 = 0,long lpulse2 = 0,
						   short axis3 = 0,long lpulse3 = 0,
						   short axis4 = 0,long lpulse4 = 0);
	/*	MODE
		0 : POINT MOVE STOP
		1 : 0 LINE START
		2 : Current LINE START
		3 : Total Line Clear & STOP
	*/
    bool __fastcall MovePoint( short nMode);
    bool __fastcall GetPointMoveStatus(short *signal);
    bool __fastcall GetPointInfo(short nLineNo, POINTDATA *pointdata);
    bool __fastcall GetPointCurrentLine(short *lineno);
    bool __fastcall MovePointNo(short nLineNo);
    bool __fastcall RunPointNo( short nLineNo);
    void GetStatus(PNMCDATA pnmcData);
    double GetLeed(int nAxis);
    bool __fastcall PulseOutInverse( int nAxis,bool binv);
    bool __fastcall OnePulseDir(int nAxis, int nDir);
    bool __fastcall MpgMode(int nAxis, int nMode);
    bool __fastcall HwLimitStopMode(int nAxis, int nMode);
    bool __fastcall CompCompareMode(int nAxis, int nMode);
    bool __fastcall BusyOffMode(int nAxis, int nMode);
    bool __fastcall SetInpoRange(int nAxis, short enable, short time, short repeatcount,long inporange);
    bool __fastcall ClearInpoError(int nAxis);
    bool __fastcall GetInpoError(long *inpoerror);
    int  __fastcall GetStopInfo(void);
    void TRACE(char * szFormat, ...)
    {
        int 	nBuf;
        char 	szBuffer[4097] = {0,};
        va_list args;

        va_start(args, szFormat);
        nBuf = vsprintf(szBuffer, szFormat, args);
        va_end(args);
        if (nBuf < 0) return;

        OutputDebugString(szBuffer);
    }

public:  //sun add
    bool __fastcall HomeProc(int nAxis);

    PaixMsgFunc m_pMsgFunc ;

    CDelayTimer m_ShakeTimer ;
    CDelayTimer m_OverRide   ;
    CDelayTimer m_OverRide1  ;

    CDelayTimer m_AngleTimer ;
    CDelayTimer m_OceanTimer ;
    struct TStat {
        bool    bHomeStart   ;
        int     iHomeStep    ;
        bool    bHomeDone    ;

        bool    bJogP        ;
        bool    bJogN        ;

        int     iShakeStep   ;
        bool    bShaking     ;
        double  dShakeTime   ;
        double  dShakeRange  ;
        int     iShakeAcc    ; // juhyeon. MOVESHAKE 가속도 입력하는 부분 추가로 인해 만듬.
        int     iShakeSpeed  ;

        int     iORStep      ;
        bool    bOverRide    ;
        int     iMaxCnt      ;
        int     iAccTime     ;
        int     iRPM         ;
        double  dTotal       ;
        int     iAllTime     ;

        int     iAngleStep   ;
        double  dAngDelayTime;
        double  dFirstAngle  ;
        double  dMoveAngle   ;
        bool    bMoveAngle   ;

        int     iSpeed[100]  ;
        int     iTime[100]   ;

    }Stat[MAX_MOTR];

    struct TActrStat{
        bool    bFwd         ;
        int     iActrIONum   ;
        bool    bFwdSnsor    ;

        bool    bMoveDone    ;
        bool    bActrStart   ;
        int     iActrStep    ;
    }ActrStat[MAX_ACTR];

    struct TStatAxis {
        bool bStart ;
        int  iStep  ;
        bool bDone  ;

    }StatAxis[MAX_MOTR];

public:
    void __fastcall SetRotorPara();

    void __fastcall MoveShake(int nAxis , double _dTime , double _dRange, int _iAcc, int _iSpeed);
    bool __fastcall ShakeProc(int nAxis);

    void __fastcall MoveAng  (int nAxis , double _dTime , double _dFirstAngle, double _dAngle);
    bool __fastcall AngleProc(int nAxis);

    void SetMsgFunc(PaixMsgFunc Func) { m_pMsgFunc = Func ; }

    void __fastcall MoveHome(int nAxis);

    void __fastcall MoveRPM (int nAxis, int RPM , int Acc_mSec , int Rot_mSec);
    void __fastcall OR_MoveRPM(int nAxis,int RPM , int Acc_mSec , int Rot_mSec);

    //Cylinder Move
    void __fastcall MoveActr(int nActr, bool _bFwd);
    bool __fastcall ActrProc(int nActr);

    double TotalAng;
    void __fastcall RotateReset (int nAxis) {
        double dTemp ;
        int    iTemp ;

        dTemp = NmcData.Cmd[nAxis] * GetLeed(nAxis)/ GetMotorRes(nAxis); ;
        iTemp = dTemp / 360 ;
        dTemp = dTemp - iTemp * 360 ;

        if(dTemp <0) dTemp = dTemp + 360 ; //-각도였을때 +로 바꿔줌.

        SetCmdPos(nAxis,dTemp*GetMotorRes(nAxis)/GetLeed(nAxis));

        dTemp = NmcData.Enc[nAxis] * GetLeed(nAxis)/ GetMotorRes(nAxis); ;
        iTemp = dTemp / 360 ;
        dTemp = dTemp - iTemp * 360 ;

        if(dTemp <0) dTemp = dTemp + 360 ; //-각도였을때 +로 바꿔줌.

        SetEncPos(nAxis,dTemp*GetMotorRes(nAxis)/GetLeed(nAxis));
    }

    bool GetbHomeDone(int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat    [nAxis].bHomeDone ; }
    bool GetbShaking (int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat    [nAxis].bShaking  ; }
    bool GetbOverRide(int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat    [nAxis].bOverRide ; }
    bool GetbMoveAng (int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat    [nAxis].bMoveAngle; }
    bool GetbMoveActr(int nActr) { if(nActr< 0 || nActr > MAX_ACTR) return false ; return ActrStat[nActr].bMoveDone ; }

//    bool SetDriveSpeed(int nAxis , int RPM , int Acc_mSec , int Rot_mSec);
    void SetDriveSpeed(int nAxis , int RPM );

    int iORCnt ;
    bool OR_Start(int dAllTime , int iCnt , int *Speed , int *Time , int iAccTime);
    bool OR_Proc (int nAxis);

    void OceanViewShow ();
    void OceanViewClick();

    bool m_bUseOVSave ;

    CDelayTimer m_tmTemp    ;
    double m_dMoveAng ;

    String sErrMsg;

};
extern CPaixMotion *pPaix;
#endif
