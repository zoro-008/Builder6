//---------------------------------------------------------------------------
#ifndef PaixMotionH
#define PaixMotionH
#include "nmc2.h"
#include <stdio.h>
#include "Timer.h"
#include "nmc2.h"

#define PAIX_NOTCONNECT     (WM_USER+1000)
#define PAIX_DEVICE_TYPE    (WM_USER+1001)
#define PAIX_SENSORREAD     (WM_USER+1002)
#define PAIX_TIME 	    (WM_USER+1003)
#define PAIX_OUTSTATUS      (WM_USER+1004)
#define PAIX_STOP_INFO      (WM_USER+1005)
#define PAIX_TARGET_INFO    (WM_USER+1006)
#define PAIX_HOME_INFO	    (WM_USER+1007)
#define PAIX_IO_OUTPUT_X    (WM_USER+1008)
#define PAIX_IO_OUTPUT_E    (WM_USER+1009)
#define PAIX_IO_OUTPUT_M    (WM_USER+1010)
#define PAIX_IO_INPUT_X	    (WM_USER+1011)
#define PAIX_IO_INPUT_E     (WM_USER+1012)
#define PAIX_IO_INPUT_M     (WM_USER+1013)
#define PAIX_DRV_SPEED      (WM_USER+1014)
#define PAIX_VER_CLEAR	    (WM_USER+1015)
#define PAIX_VER            (WM_USER+1016)
#define PAIX_IO_TIME 	    (WM_USER+1017)

#define IO_A 0//  false - A 접점  ,   true  - B 접점
#define IO_B 1

#define CCW 1
#define CW 0

#define Axis1 0
#define Axis2 1

#define MAX_MOTR 2
#define MAX_INPUT 16
#define MAX_OUTPUT 16
#define IP       11


//#define PULSE_PER_REV 32768   //야스카와 최대 분해능이 안나와 속도가 2000rpm이상 안된다.
                                //미쯔비시도 그냥 16384에 맞춰서 쓴다.
#define TILT_PULSE_PER_REV 10000
#define SPIN_PULSE_PER_REV 10000
#define TILT_REDUCTION 10        //10:1감속기
#define SPIN_REDUCTION 3         //10:1감속기
#define TILT_MPS       6         //Tilt모터 초당 가는 거리 계산 떄문에.
#define ONEREVOLUTION_ANGLE 360
typedef void (__closure *PaixMsgFunc)(AnsiString);

typedef void (__closure *FThreadCallBack) (void);
//---------------------------------------------------------------------------
struct TLog {
    void SetLog(AnsiString _sFunction, int _iReturn) {sFunction = _sFunction; iReturn = _iReturn;}
    AnsiString sFunction;
    int        iReturn  ;
};


















class CPaixMotion:public TThread
{
public:
    __fastcall CPaixMotion(void);
    __fastcall virtual ~CPaixMotion(void);
    void Init(void);

    void SetThreadCallBack(FThreadCallBack _pFunc){
        pThreadCallBack = _pFunc ;

    }

private:    // 내부 변수
    short m_bRemote;
    HWND m_hWnd;

    int m_nPaixCtlNo;
    int m_nMotionMode[MAX_MOTR];

    void __fastcall Execute();
    void SetRemote(short bRemote);
    bool HomeTemp;
    double dPPS;

    //시퀜스에서 쓰는 쓰레드 콜백.
    FThreadCallBack pThreadCallBack ;


public:
    //bool m_bMLimit[MAX_MOTR],m_bPLimit[MAX_MOTR],m_bZ[MAX_MOTR],m_bNear[MAX_MOTR],
    //     m_bInpo[MAX_MOTR],m_bAlarm[MAX_MOTR],m_bError[MAX_MOTR],m_bBusy[MAX_MOTR],m_bOrg[MAX_MOTR];
    //bool m_bHomeDone[MAX_MOTR];
    //long m_lPPS[MAX_MOTR];
    //long m_lEncPos[MAX_MOTR],m_lCmdPos[MAX_MOTR];
    //bool m_bEmer;
    //double m_dCmdSpeed[MAX_MOTR], m_dEncSpeed[MAX_MOTR];

    //short m_lInputXIo[128];
    //short m_lInputEIo[32];
    //short m_lInputMIo[32];
    //
    //short m_lOutputXIo[128];
    //short m_lOutputEIo[32];
    //short m_lOutputMIo[32];
    //short m_nStopInfo[8];

    //NMCAXESEXPR      NmcData;
    //NMCAXESMOTIONOUT NmcOut;
    //NMCHOMEFLAG      Homeflag;













    bool m_bConnect;
    bool m_bNotConnect;

    TLog Log[2];

private:
    //NMCDATA nmcData;
/*
    bool m_bAxis1Start;
    bool m_bAxis2Start;

    bool m_bAxis1Stop;
    bool m_bAxis2Stop;
*/
    double m_fEncPos[MAX_MOTR],m_fCmdPos[MAX_MOTR];
public:

    bool OpenPaixCtl(int nPaixCtlNo);
    bool ClosePaixCtl(void);

    bool SetPaixCtlNo(int nPaixCtlNo);
    bool PaixInit(int nPaixCtlNo);

    void SetIpAddress(short nField0,short nField1,short nField2);
    bool GetIpAddress(short *nField0,short *nField1,short *nField2,short *nField3);

    bool GetConnect(void){return m_bRemote;};
    
    __property short Remote= {read=m_bRemote,write=SetRemote};

    //Motion
    short GetLogic(int nAxis, PPARALOGIC pLogic);
    bool GetSpeed(short nAxis, PPARASPEED pSpeed);
    bool GetRingCountMode(int nAxis, double * plMaxPulse , short * plMaxEncoder, short *pnRingMode);
    bool GetParaTargetPos(int nAxis, double * pTargetPos);
    bool GetAxesMotionOut(int nAxis, PNMCAXESMOTIONOUT pOutStatus);

    //------------------------------------------------------------------------------
    //bool GetCmdSpeed();
    //bool GetEncSpeed();
    //bool GetCmdEncSpeed();
    //------------------------------------------------------------------------------
    bool SetEmgLogic       (short OnOff);
    bool SetPlusLimitLogic (int nAxis, short OnOff);
    bool SetMinusLimitLogic(int nAxis, short OnOff);
    bool SetSWLimitLogic   (int nAxis, short OnOff, double dSwMinusPos, double dSwPlusPos);
    bool SetAlarmLogic     (int nAxis, short OnOff);
    bool SetNearLogic      (int nAxis, short OnOff);
    bool SetInPoLogic      (int nAxis, short OnOff);
    bool SetBusyOffMode    (int nAxis, short OnOff);
    bool SetSvReadyLogic   (int nAxis, short OnOff);
    bool SetHwLimitStopMode(int nAxis, short OnOff);
    //------------------------------------------------------------------------------
    // ENCODER 관련
    bool SetEncCount (int nAxis, short nCountMode);
    bool SetEncDir   (int nAxis, short nCountDir );
    bool SetEncZLogic(int nAxis, short OnOff    );
    //------------------------------------------------------------------------------
    bool SetPulseLogic(int nAxis, short nMode);
    //------------------------------------------------------------------------------
    bool SetCurrentOn   (int nAxis, short OnOff);
    bool SetServoOn     (int nAxis, short OnOff);
    bool SetAlarmResetOn(int nAxis, short OnOff);
    bool SetDCCOn       (int nAxis, short OnOff);
    //------------------------------------------------------------------------------
    //dSttSpeed : dStartSpeed , dDSpeed : dDriveSpeed
    short  SetRatio(short nAxis, double fRatio);
    double GetRatio(short nAxis);
    bool SetSpeed      (int nAxis, double dSttSpeed, double dAcc, double dDec, double dDSpeed);
    bool SetSCurveSpeed(int nAxis, double dSttSpeed, double dAcc, double dDec, double dDSpeed);
    bool SetAccSpeed   (int nAxis, double dAcc);
    bool SetDecSpeed   (int nAxis, double dDec);
    bool SetOverrideRunSpeed(int nAxis, double dAcc, double dDec, double dDSpeed);
    bool SetOverrideDSpeed(int nAxis, double dDSpeed);
    bool AbsMove(int nAxis, double dPos);
    bool IncMove(int nAxis, double dPos);
    //bool VelMove(int nAxis, double dPos, double dDSpeed, short nMode); //iMode 0:AbsMove, 1:IncMove
    //bool AbsOver(int nAxis, double dPos);
    //------------------------------------------------------------------------------
    bool JogMove(int nAxis, short CwCCw);    //0:Cw , 1:CCw
    //------------------------------------------------------------------------------
    bool SuddenStop (int nAxis);//긴급정지
    bool DecStop    (int nAxis);//감속 정지
    bool AllAxisStop(int iMove); //0:감속정지, 1:긴급정지
    //------------------------------------------------------------------------------

    bool HomeMove(int nAxis, short nMode, short nEndMode, double dOffset);
    bool SetHomeSpeed(int nAxis, double dHomeSpeed1, double dHomeSpeed2, double dHomeSpeed3);
    bool GetHomeStatus();

    //------------------------------------------------------------------------------
    bool InterPolation2Axis(int nAxis1, double dPos1, int nAxis2, double dPos2, short nOpt);//0:상대 이동, 1:절대 이동
    bool InterPolation3Axis(int nAxis1, double dPos1, int nAxis2, double dPos2, int nAxis3, double dPos3, short nOpt);//0:상대 이동, 1:절대 이동
    bool InterPolationArc  (int nAxis1, double dCenter1, double dCenter2, double dAngle, short nOpt);//0:상대 좌표, 1:절대 좌표
    //------------------------------------------------------------------------------

    short SetOutputX(short *OutputX);
    short SetOutputXToggle(short nBit);
    short SetOutputXBit(short nBit, short nValue);
    short GetInputX();
    short GetOutputX();

    short SetOutputE(short *OutputE);
    short SetOutputEToggle(short nBit);
    short SetOutputEBit(short nBit, short nValue);
    short GetInputE();
    short GetOutputE();

    short SetOutputM(short *OutputM);
    short SetOutputMToggle(short nBit);
    short GetInputM();
    short GetOutputM();
    short SetOutputMBit(short nBit, short nValue);

    short SetIoTest(short mode, short delay);

    void GetInputX(short *pInputX);
    void GetInputE(short *pInputE);
    void GetInputM(short *pInputE);
    void GetOutputX(short *pOutputX);
    void GetOutputE(short *pOutputE);
    void GetOutputM(short *pOutputM);
    short GetInputMBit(short nBitNo, short *nInput);
    short GetInputEBit(short nBitNo, short *nInput);
    short GetInputXBit(short nBitNo, short *nInput);

    int GetDllVersion(char *pStr);

    bool SetSlowStop(int nAxis);
    bool StopMotor(int nAxis);

    bool SetRpm(int nAxis,long lStart, long lAcc,long lMax,long lJerk=0);

    double GetMotorRes( int nAxis);
    bool SetClock(int nAxis,short nClock);
    bool SetHomeMode(int nAxis, short nMode);
    bool SetHomeDir(int nAxis, bool bDir);
    bool SetMotorDir(int nAxis, bool bDir);

    int GetPLimit(int nAxis);
    int GetMLimit(int nAxis);
    int GetNear(int nAxis);
    int GetZ(int nAxis);
    int GetOrg(int nAxis);
    int GetInpo(int nAxis);
    int GetAlarm(int nAxis);
    int GetError(int nAxis);
    int GetSrvReday(int nAxis);
    bool GetHomeDone(int nAxis);

    //bool ReadSensorAll(PNMCDATA pNmcData);

    bool GetOffset(int nAxis, int *offset);
    bool SetMotionType(int nAxis,int nType);
    int GetMotionMode(int nAxis);

    bool SetIniFile(char *pFile);

    int GetPortNum();

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

    bool __fastcall GetOutput(BYTE *value);
    bool __fastcall SetOutput(BYTE value);
    bool __fastcall SetOutputBit(int bitno,BYTE value);

    void GetNmcStatus    (PNMCAXESEXPR pNmcData);
    void GetNmcMotionOut (PNMCAXESMOTIONOUT pNmcMotion);

    void GetNmcHomeFlag  (PNMCHOMEFLAG pNmcHomeFlag);
    bool GetBusyStatus   (short nAxis, short *pnBusy);
    bool GetBusyStatusAll(short *pnBusy);


    void SetMainWindow(HWND hWnd)
    {
    	m_hWnd = hWnd;
    }

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
    
    struct TStat {
        bool    bHomeStart  ;
        int     iHomeStep   ;
        bool    bHomeDone   ;

        bool    bJogP       ;
        bool    bJogN       ;

        int     iShakeStep  ;
        bool    bShaking    ;
        double  dShakeTime  ;
        double  dShakeRange ;

        int     iORStep     ;
        bool    bOverRide   ;
        int     iMaxCnt     ;
        int     iAccTime    ;
        int     iRPM        ;
        double  dTotal      ;
        int     iAllTime    ;

        int     iSpeed[100] ;
        int     iTime[100]  ;

    }Stat[MAX_MOTR];


public:
    double m_dTiltTrgPos ; //틸트 병신짓 모니터링.


    //dPos 는 UI상의 단위로 넣는다.
    bool SetCmdPos(int nAxis, double dPos);
    bool SetEncPos(int nAxis, double dPos);
    bool SetCmdEncPos(int nAxis, double dPos);

    //UI단위로 리턴 한다.
    double GetCmdPos(int nAxis);
    double GetEncPos(int nAxis);

    //UI상의 단위를  펄스 로 변환함.
    int GetPulsePos  (int nAxis , double dPos);
    int GetPulseSpeed(int nAxis , double dUnitPerSec);
    int GetPulseSpeedFromRPM(int nAxis , double RPM);


    void __fastcall SetRotorPara();

    void __fastcall MoveShake(int nAxis , double _dTime , double _dRange);
    //bool __fastcall ShakeProc(int nAxis);

    void SetMsgFunc(PaixMsgFunc Func) { m_pMsgFunc = Func ; }

    void __fastcall MoveHome(int nAxis);
    double __fastcall GetRPM(int nAxis);

    void __fastcall MoveJogRPM(int nAxis,int RPM );


    double TotalAng;


    bool GetbHomeDone(int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat[nAxis].bHomeDone ; }
    bool GetbShaking (int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat[nAxis].bShaking  ; }
    bool GetbOverRide(int nAxis) { if(nAxis< 0 || nAxis > MAX_MOTR) return false ; return Stat[nAxis].bOverRide ; }

    //void SetDriveSpeed(int nAxis , int RPM );

    int iORCnt ;
    bool OR_Start(int dAllTime , int iCnt , int *Speed , int *Time , int iAccTime);


    short *bSw;
};
extern CPaixMotion *pPaix;
#endif

