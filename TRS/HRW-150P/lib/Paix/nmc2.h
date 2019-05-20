#ifndef _NMC2_H_
#define _NMC2_H_

#ifdef __cplusplus
	extern "C" {
#endif

// NMC2 Equip Type
#define		NMC2_220S			0
#define		NMC2_420S			1
#define		NMC2_620S			2
#define		NMC2_820S			3

#define		NMC2_220_DIO32		4
#define		NMC2_220_DIO64		5
#define		NMC2_420_DIO32		6
#define		NMC2_420_DIO64		7
#define		NMC2_820_DIO32		8
#define		NMC2_820_DIO64		9

#define		NMC2_DIO32			10
#define		NMC2_DIO64			11
#define		NMC2_DIO96			12
#define		NMC2_DIO128			13

#define		NMC2_220			14
#define		NMC2_420			15
#define		NMC2_620			16
#define		NMC2_820			17

// NMC2 Enum Type
	#define EQUIP_TYPE_NMC_2_AXIS		0x0001
	#define EQUIP_TYPE_NMC_4_AXIS		0x0003
	#define EQUIP_TYPE_NMC_6_AXIS		0x0007
	#define EQUIP_TYPE_NMC_8_AXIS		0x000F
	// 16/16
	#define EQUIP_TYPE_NMC_IO_32		0x0010
	// 32/32
	#define EQUIP_TYPE_NMC_IO_64		0x0030
	// 48/48
	#define EQUIP_TYPE_NMC_IO_96		0x0070
	// 64/64
	#define EQUIP_TYPE_NMC_IO_128		0x00F0
	// 80/80
	#define EQUIP_TYPE_NMC_IO_160		0x01F0
	// 96/96
	#define EQUIP_TYPE_NMC_IO_192		0x03F0
	// 112/112
	#define EQUIP_TYPE_NMC_IO_224		0x07F0
	// 128/128
	#define EQUIP_TYPE_NMC_IO_256		0x0FF0

	#define EQUIP_TYPE_NMC_IO_IE		0x1000
	#define EQUIP_TYPE_NMC_IO_OE		0x2000

	#define EQUIP_TYPE_NMC_M_IO_8		0x4000


// 모든 함수의 리턴값 
#ifndef PAIX_RETURN_VALUE
#define PAIX_RETURN_VALUE
	#define NMC_INTERPOLATION		-13	
	#define	NMC_FILE_LOAD_FAIL		-12
	#define	NMC_ICMP_LOAD_FAIL		-11
	#define	NMC_NOT_EXISTS			-10
	#define	NMC_CMDNO_ERROR			-9
	#define	NMC_NOTRESPONSE			-8
	#define NMC_BUSY				-7
	#define NMC_COMMERR				-6
	#define NMC_SYNTAXERR   		-5
	#define NMC_INVALID				-4
	#define NMC_UNKOWN				-3
	#define NMC_SOCKINITERR 		-2
	#define NMC_NOTCONNECT 			-1
	#define NMC_OK					0
#endif

// Bit 연산 정의
#ifndef _PAIX_BITOPERATION_
	#define _PAIX_BITOPERATION_
	#define BITSET(val,bit)					(val) |= (1<<(bit))
	#define BITCLEAR(val,bit)				(val) &= (~(1<<(bit)))
	#define BITTOGGLE(val,bit)				(val) ^= (1<<(bit))
	#define BITVALUE(val, sdata, bit)		(val) = (((sdata)>> bit) & 0x01)
	#define GETBIT(bytes, bit_no)			(((bytes)>>(bit_no))&0x01)
	#define SETBIT(bytes, bit_no, val)		{ (bytes) &=~(1<<(bit_no)); (bytes) |= ((val) << (bit_no)); }
#endif
	
 
#ifndef _NMC_STOP_MODE_
#define _NMC_STOP_MODE_
	#define NMC_STOP_OK         0
	#define NMC_STOP_EMG 		1
	#define NMC_STOP_MLIMIT     2
	#define NMC_STOP_PLIMIT   	3
	#define NMC_STOP_ALARM   	4
	#define NMC_STOP_NEARORG   	5
	#define NMC_STOP_ENCZ   	6
#endif

#ifndef _NMC_HOME_MODE_
#define _NMC_HOME_MODE_
	#define NMC_HOME_LIMIT_P					0
	#define NMC_HOME_LIMIT_M					1
	#define NMC_HOME_NEAR_P						2
	#define NMC_HOME_NEAR_M						3
	#define NMC_HOME_Z_P						4
	#define NMC_HOME_Z_M						5

	#define	NMC_HOME_USE_Z						0x80

	#define NMC_END_NONE						0x00
	#define NMC_END_CMD_CLEAR_A_OFFSET			0x01
	#define NMC_END_ENC_CLEAR_A_OFFSET			0x02
	#define NMC_END_CMD_CLEAR_B_OFFSET			0x04
	#define NMC_END_ENC_CLEAR_B_OFFSET			0x08
#endif


typedef struct NMC_PARA_LOGIC
{
   //-------------------------------------------------------------
	short nEmg;			// EMG
	short nEncCount;		// 엔코더 카운트 모드

	short nEncDir;			// 엔코더 카운트 방향
	short nEncZ;			// 엔코더 Z

	short nNear;			// NEAR(HOME)
	short nMLimit;			// - Limit

	short nPLimit;			// + Limit
	short nAlarm;			// Alarm

	short nInp;				// INPOSITION
	short nSReady;			// Servo Ready

	short nPulseMode;		// 1p/2p Mode
	//-------------------------------------------------------------

	short nLimitStopMode;	// Limit stop mode
	short nBusyOff;			// Busy off mode

	short nSWEnable;		// sw limit 활성화 여부
   //-------------------------------------------------------------
	double  	dSWMLimitPos;
	double  	dSWPLimitPos;
}PARALOGIC,*PPARALOGIC;

typedef struct NMC_PARA_SPEED{
	double	dStart;
	double	dAcc;
	double	dDec;
	double	dDrive;
	double	dJerkAcc;
	double	dJerkDec;
}PARASPEED,*PPARASPEED;

//{
typedef struct NMC_AXES_MOTION_OUT
{
	short nCurrentOn[8];		// Motor OnOff
	short nServoOn[8];			// Servo OnOff
	short nDCCOn[8];			// Reset DCC
	short nAlarmResetOn[8];		// Reset Alarm
}NMCAXESMOTIONOUT,*PNMCAXESMOTIONOUT;


typedef struct NMC_AXES_EXPR{
	short nBusy[8];
	short nError[8];
	short nNear[8];
	short nPLimit[8];
	short nMLimit[8];

	short nAlarm[8];
	short nEmer[2];
	short nSwPLimit[8];
	short nInpo[8];
	short nHome[8];

	short nEncZ[8];
	short nOrg[8];
	short nSReady[8];

	short nReserved0[8];
	short nSwMLimit[8]; 

	long lEnc[8];
	long lCmd[8];
	double dEnc[8];
	double dCmd[8];
	char dummy[4];
}NMCAXESEXPR,*PNMCAXESEXPR;

typedef struct NMC_STOPMODE
{
	short nEmg;				// EMG
	short nMLimit;			// 엔코더 카운트 모드

	short nPLimit;			// 엔코더 체
	short nAlarm;			// 엔코더 Z

	short nNear;			// NEAR
	short nEncZ;			// - Limit
}STOPMODE,*PSTOPMODE;

typedef struct NMC_HOME_FLAG{
    short nSrchFlag[8];
    short nStatusFlag[8];
}NMCHOMEFLAG,*PNMCHOMEFLAG;

typedef struct NMC_CONT_STATUS
{
	short nStatus[2];
	short nExeNodeNo[2];
}NMCCONTSTATUS,*PNMCCONTSTATUS;

typedef struct TNMC_EQUIP_LIST
{
	/*
	192.168.0.11
		BIT0~BIT7  : 192
		BIT8~BIT15  : 168
		BIT16~BIT23  : 0
		BIT24~BIT31  : 11
	*/
	long lIP[200];
	long lModelType[200];
	short nMotionType[200];
	short nDioType[200];
	short nEXDio[200];
	short nMDio[200];

}NMCEQUIPLIST,*PNMCEQUIPLIST;

typedef struct TNMC_MAP_DATA
{
	int		nMapCount;
	long	lMapData[52];
	double	dMapData[52];
}NMCMAPDATA,*PNMCMAPDATA;


//------------------------------------------------------------------------------
	// Function
//------------------------------------------------------------------------------
short WINAPI nmc_OpenDevice(short nNmcNo);
void WINAPI nmc_CloseDevice(short nNmcNo);
//------------------------------------------------------------------------------
	//short WINAPI nmc_GetParameter(short nNmcNo, short nAxisNo, PNMCPARA pPara);
	short WINAPI nmc_GetParaLogic(short nNmcNo, short nAxisNo, PPARALOGIC pLogic);
	short WINAPI nmc_GetParaSpeed(short nNmcNo, short nAxisNo,PPARASPEED pSpeed);
	short WINAPI nmc_GetRingCountMode(short nNmcNo, short nAxisNo,long *plMaxPulse,long *plMaxEncoder, short *pnRingMode);
	short WINAPI nmc_GetParaTargetPos(short nNmcNo, short nAxisNo,double *pTargetPos);
	short WINAPI nmc_GetAxesMotionOut(short nNmcNo, PNMCAXESMOTIONOUT pOutStatus);
	
//------------------------------------------------------------------------------
	short WINAPI nmc_GetAxesCmdSpeed(short nNmcNo, double *pDrvSpeed);
	short WINAPI nmc_GetAxesEncSpeed(short nNmcNo, double *pdEncSpeed);
	short WINAPI nmc_GetAxesCmdEncSpeed(short nNmcNo, double *pdCmdSpeed, double *pdEncSpeed);
//------------------------------------------------------------------------------
short WINAPI nmc_SetEmgLogic(short nNmcNo, short nGroupNo, short nLogic);
short WINAPI nmc_SetPlusLimitLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetMinusLimitLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetSWLimitLogic(short nNmcNo, short nAxisNo, short nUse,double dSwMinusPos, double dSwPlusPos);
short WINAPI nmc_SetSWLimitLogicEx(short nNmcNo, short nAxisNo, short nUse,double dSwMinusPos, double dSwPlusPos,short nOpt);
short WINAPI nmc_SetAlarmLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetNearLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetInPoLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetSReadyLogic(short nNmcNo, short nAxisNo, short nLogic);
//------------------------------------------------------------------------------
// ENCODER 관련
	short WINAPI nmc_SetEncoderCount(short nNmcNo, short nAxisNo, short nCountMode);
	short WINAPI nmc_SetEncoderDir(short nNmcNo, short nAxisNo, short nCountDir);
	short WINAPI nmc_SetEncoderMode(short nNmcNo, short nAxisNo, short nMode);
	short WINAPI nmc_SetEncoderZLogic(short nNmcNo, short nAxisNo, short nLogic);
//------------------------------------------------------------------------------
short WINAPI nmc_SetPulseMode(short nNmcNo, short nAxisNo, short nMode);
	short WINAPI nmc_SetPulseLogic(short nNmcNo, short nAxisNo, short nLogic);
	short WINAPI nmc_Set2PulseDir(short nNmcNo, short nAxisNo, short nDir);
	short WINAPI nmc_Set1PulseDir(short nNmcNo, short nAxisNo, short nDir);
	short WINAPI nmc_SetPulseActive(short nNmcNo, short nAxisNo, short nPulseActive);
//------------------------------------------------------------------------------
short WINAPI nmc_SetSCurveSpeed(short nNmcNo, short nAxisNo,double dStartSpeed, 
									double dAcc,double dDec ,double dDriveSpeed);
short WINAPI nmc_SetSpeed(short nNmcNo, short nAxisNo,double dStartSpeed, 
						  double dAcc,double dDec ,double dDriveSpeed);
short WINAPI nmc_SetOverrideRunSpeed(short nNmcNo, short nAxisNo,double dAcc,double dDec ,double dDriveSpeed);
short WINAPI nmc_SetOverrideDriveSpeed(short nNmcNo,short nAxisNo, double dDriveSpeed);
short WINAPI nmc_SetAccSpeed(short nNmcNo, short nAxisNo, double dAcc);
short WINAPI nmc_SetDecSpeed(short nNmcNo, short nAxisNo, double dDec);
//------------------------------------------------------------------------------
short WINAPI nmc_AbsMove(short nNmcNo, short nAxisNo,double dPos);
short WINAPI nmc_RelMove(short nNmcNo, short nAxisNo,double dAmount);
short WINAPI nmc_VelMove(short nNmcNo, short nAxisNo, double dPos, double dDrive,short nMode);

short WINAPI nmc_AbsOver(short nNmcNo, short nAxisNo,double dPos);

short WINAPI nmc_VarRelMove(short nNmcNo, short nAxisCount,short *pnAxisNo,double *pdAmount);
short WINAPI nmc_VarAbsMove(short nNmcNo, short nAxisCount,short *pnAxisNo,double *pdPosList);
short WINAPI nmc_VarAbsOver(short nNmcNo, short nAxisCount,short *pnAxisNo,double *pdPosList);
//------------------------------------------------------------------------------
short WINAPI nmc_JogMove(short nNmcNo, short nAxis, short Dnir);
//------------------------------------------------------------------------------
short WINAPI nmc_SuddenStop(short nNmcNo, short nAxisNo);
short WINAPI nmc_DecStop(short nNmcNo, short nAxisNo);

short WINAPI nmc_AllAxisStop(short nNmcNo, short nMode);
short WINAPI nmc_MultiAxisStop(short nNmcNo,short nCount,short *pnAxisSelect,short nMode);
//------------------------------------------------------------------------------
short WINAPI nmc_GetAxesExpress(short nNmcNo, PNMCAXESEXPR  pNmcData);
short WINAPI nmc_GetStopInfo(short nNmcNo, short *pnStopMode);
//------------------------------------------------------------------------------
short WINAPI nmc_SetCmdPos(short nNmcNo, short nAxisNo, double dPos);
short WINAPI nmc_SetEncPos(short nNmcNo, short nAxisNo, double dPos);
short WINAPI nmc_SetCmdEncPos(short nNmcNo, short nAxisNo, double dPos);
//------------------------------------------------------------------------------
short WINAPI nmc_HomeMove(short nNmcNo, short nAxisNo, short nHomeMode,short nHomeEndMode,double dOffset, short nReserve);
short WINAPI nmc_GetHomeStatus(short nNmcNo, PNMCHOMEFLAG pHomeFlag);
short WINAPI nmc_SetHomeSpeed(short nNmcNo, short nAxisNo,
							  double dHomeSpeed0,double dHomeSpeed1,double dHomeSpeed2);
short WINAPI nmc_SetHomeSpeedEx(short nNmcNo, short nAxisNo,
							  double dHomeSpeed0,double dHomeSpeed1,double dHomeSpeed2,double dOffsetSpeed);
//------------------------------------------------------------------------------
short WINAPI nmc_Interpolation2Axis(short nNmcNo, short nAxisNo0, double dPos0,
									short nAxisNo1, double dPos1,short nOpt);
short WINAPI nmc_Interpolation3Axis(short nNmcNo, short nAxisNo0, double dPos0,
			short nAxisNo1, double dPos1, short nAxisNo2, double dPos2,short nOpt);
//------------------------------------------------------------------------------
short WINAPI nmc_InterpolationArc(short nNmcNo, short nAxisNo0, short nAxisNo1, 
								  double dCenter0,double dCenter1, double dAngle,short nOpt);

short WINAPI nmc_InterpolationRelCircle (short nNmcNo, short nAxisNo0, double CenterPulse0,double EndPulse0,
			short nAxisNo1, double CenterPulse1, double EndPulse1,short nDir);
short WINAPI nmc_InterpolationAbsCircle (short nNmcNo, short nAxisNo0, double CenterPulse0,double EndPulse0,
			short nAxisNo1, double CenterPulse1, double EndPulse1,short nDir);

//------------------------------------------------------------------------------
short WINAPI nmc_SetCurrentOn (short nNmcNo, short nAxisNo,short nOut);
short WINAPI nmc_SetServoOn (short nNmcNo, short nAxisNo,short nOut);
short WINAPI nmc_SetAlarmResetOn(short nNmcNo, short nAxisNo,short nOut);
short WINAPI nmc_SetDccOn (short nNmcNo, short nAxisNo,short nOut);
//------------------------------------------------------------------------------
short WINAPI nmc_SetMultiCurrentOn(short nNmcNo, short nCount,short *pnAxisSelect,short nOut);
short WINAPI nmc_SetMultiServoOn(short nNmcNo, short nCount,short *pnAxisSelect,short nOut);
short WINAPI nmc_SetMultiAlarmResetOn(short nNmcNo, short nCount,short *pnAxisSelect,short nOut);
short WINAPI nmc_SetMultiDccOn(short nNmcNo, short nCount,short *pnAxisSelect,short nOut);
//------------------------------------------------------------------------------
	short WINAPI nmc_SetEnableNear(short nNmcNo, short nAxisNo,short nMode);
	short WINAPI nmc_SetEnableEncZ(short nNmcNo, short nAxisNo,short nMode);
	short WINAPI nmc_SetLimitStopMode (short nNmcNo, short nAxisNo,short nStopMode);
	short WINAPI nmc_SetBusyOffMode(short nNmcNo, short nAxisNo, short nMode);

	//------------------------------------------------------------------------------
	short WINAPI nmc_SetRingCountMode (short nNmcNo, short nAxisNo,long lMaxPulse,long lMaxEncoder, short nRingMode);
	short WINAPI nmc_MoveRing (short nNmcNo, short nAxisNo,double dPos, short nMoveMode);
//------------------------------------------------------------------------------
	short WINAPI nmc_SetSyncSetup(short nNmcNo, short nMainAxisNo, 
								short nSubAxisNoEnable0, short nSubAxisNoEnable1, short nSubAxisNoEnable2);
	short WINAPI nmc_SetSync(short nNmcNo, short nGroupNo, 
								short *pnSyncGrpList0, short *pnSyncGrpList1);
	short WINAPI nmc_SyncFree(short nNmcNo, short nGroupNo);
//------------------------------------------------------------------------------

short WINAPI nmc_SetMDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
short WINAPI nmc_SetMDIOOutTogPin(short nNmcNo, short nPinNo);
short WINAPI nmc_SetMDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
short WINAPI nmc_SetMDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);
short WINAPI nmc_GetMDIOInPin(short nNmcNo, short nPinNo,short *pnInStatus);

short WINAPI nmc_GetMDIOInput(short nNmcNo, short *pnInStatus);
short WINAPI nmc_GetMDIOInputBit(short nNmcNo, short nBitNo,short *pnInStatus);
short WINAPI nmc_GetMDIOOutput(short nNmcNo, short *pnOutStatus);

short WINAPI nmc_SetMDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetMDIOOutputBit(short nNmcNo, short nBitNo, short nOutStatus);
short WINAPI nmc_SetMDIOOutputTog(short nNmcNo, short nBitNo);
short WINAPI nmc_SetMDIOOutputAll(short nNmcNo,  short *pnOnBitNo, short *pnOffBitNo);
short WINAPI nmc_SetMDIOOutputTogAll(short nNmcNo, short *pnBitNo);

	short WINAPI nmc_GetMDIOInput32(short nNmcNo, long *plInStatus);
	short WINAPI nmc_GetMDIOOutput32(short nNmcNo, long *plOutStatus);
	short WINAPI nmc_SetMDIOOutput32(short nNmcNo, long lOutStatus);

//-----------------------------------------------------------------------------
short WINAPI nmc_SetDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
short WINAPI nmc_SetDIOOutTogPin(short nNmcNo, short nPinNo);
short WINAPI nmc_SetDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
short WINAPI nmc_SetDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);
short WINAPI nmc_GetDIOInPin(short nNmcNo, short nPinNo,short *pnInStatus);

short WINAPI nmc_GetDIOInput(short nNmcNo, short *pnInStatus);
short WINAPI nmc_GetDIOInputBit(short nNmcNo, short nBitNo,short *pnInStatus);


short WINAPI nmc_GetDIOOutput(short nNmcNo, short *pnOutStatus);

short WINAPI nmc_SetDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetDIOOutputBit(short nNmcNo, short nBitno, short nOutStatus);
short WINAPI nmc_SetDIOOutputTog(short nNmcNo, short nBitNo);
short WINAPI nmc_SetDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo);
short WINAPI nmc_SetDIOOutputTogAll(short nNmcNo, short *pnBitNo);
	short WINAPI nmc_GetDIOInput64(short nNmcNo, LONG64 *plInStatus);
	short WINAPI nmc_GetDIOOutput64(short nNmcNo, LONG64 *plOutStatus);
	short WINAPI nmc_SetDIOOutput64(short nNmcNo, LONG64 lOutStatus);

	short WINAPI nmc_GetDIOInput32(short nNmcNo, short nIndex, long *plInStatus);
	short WINAPI nmc_GetDIOOutput32(short nNmcNo, short nIndex, long *plOutStatus);
	short WINAPI nmc_SetDIOOutput32(short nNmcNo, short nIndex, long lOutStatus);
//-----------------------------------------------------------------------------
short WINAPI nmc_SetEXDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
short WINAPI nmc_SetEXDIOOutTogPin(short nNmcNo, short nPinNo);
short WINAPI nmc_SetEXDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
short WINAPI nmc_SetEXDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);
short WINAPI nmc_GetEXDIOInPin(short nNmcNo, short nPinNo,short *pnInStatus);

short WINAPI nmc_GetEXDIOInput(short nNmcNo, short *pnInStatus);
short WINAPI nmc_GetEXDIOInputBit(short nNmcNo, short nBitNo,short *pnInStatus);
short WINAPI nmc_GetEXDIOOutput(short nNmcNo, short *pnOutStatus);

short WINAPI nmc_SetEXDIOOutput(short nNmcNo, short *pnOutStatus);

	short WINAPI nmc_SetEXDIOOutputBit(short nNmcNo, short nBitNo, short nOutStatus);
	short WINAPI nmc_SetEXDIOOutputTog(short nNmcNo, short nBitNo);
	short WINAPI nmc_SetEXDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo);
	short WINAPI nmc_SetEXDIOOutputTogAll(short nNmcNo, short *pnBitNo);

	short WINAPI nmc_GetEXDIOInput32(short nNmcNo, long *plInStatus);
	short WINAPI nmc_GetEXDIOOutput32(short nNmcNo, long *plOutStatus);
	short WINAPI nmc_SetEXDIOOutput32(short nNmcNo, long lOutStatus);
//-----------------------------------------------------------------------------
short WINAPI nmc_GetFirmVersion(short nNmcNo,char *pStr);
//-----------------------------------------------------------------------------
short WINAPI nmc_SetUnitPerPulse(short nNmcNo, short nAxisNo, double dRatio);
double WINAPI nmc_GetUnitPerPulse(short nNmcNo, short nAxisNo);
//-----------------------------------------------------------------------------
void WINAPI nmc_SetProtocolMethod(short nNmcNo, short nMethod);
short WINAPI nmc_GetProtocolMethod(short nNmcNo);
//-----------------------------------------------------------------------------
bool WINAPI nmc_GetIPAddress(short *pnField0,short *pnField1,short *pnField2,short *pnField3);
void WINAPI nmc_SetIPAddress(short nNmcNo,short nField0,short nField1,short nField2);
short WINAPI nmc_WriteIPAddress(short nNmcNo, short *pnIP, short *pnSubNet,short nGateway);
short WINAPI nmc_SetDefaultIPAddress(short nNmcNo);
//-----------------------------------------------------------------------------
short WINAPI nmc_GetDeviceType(short nNmcNo, long *plDeviceType);

short WINAPI nmc_GetDeviceInfo(short nNmcNo, short *pnMotionType, short *pnDioType, short *pnEXDio, short *pnMDio);
int WINAPI nmc_GetEnumList(short *pnIp,PNMCEQUIPLIST pNmcList);
//-----------------------------------------------------------------------------

short WINAPI nmc_MotCfgSaveToROM(short nNmcNo, short nMode);
short WINAPI nmc_MotCfgSetDefaultROM(short nNmcNo, short nMode);
short WINAPI nmc_MotCfgLoadFromROM(short nNmcNo, short nMode);

//------------------------------------------------------------------------------
short WINAPI nmc_AccOffsetCount(short nNmcNo, short nAxisNo, long lPulse);
//------------------------------------------------------------------------------
short WINAPI nmc_PingCheck(short nNmcNo, long lWaitTime);

//------------------------------------------------------------------------------
short WINAPI nmc_GetBusyStatus(short nNmcNo, short nAxisNo, short *pnBusyStatus);
short WINAPI nmc_GetBusyStatusAll(short nNmcNo, short *pnBusyStatus);
//------------------------------------------------------------------------------
short WINAPI nmc_SetTriggerCfg(short nNmcNo, short nAxis, short nCompMode, long lCmpAmount, double dDioOnTime, 
								short nPinNo, short nDioType, short nReserve);
short WINAPI nmc_SetTriggerEnable(short nNmcNo, short nAxis,short nEnable);
//------------------------------------------------------------------------------
short WINAPI nmc_GetMapIO(short nNmcNo,short *pnMapInStatus);
short WINAPI nmc_MapMove(short nNmcNo, short nAxis, double dPos,short nMapIndex, short nOpt);
short WINAPI nmc_MapMoveEx(short nNmcNo, short nAxis, double dPos,short nMapIndex,short nOpt, short nPosType);
short WINAPI nmc_GetMapData(short nNmcNo,short nMapIndex, PNMCMAPDATA pNmcMapData);
short WINAPI nmc_GetMapDataEx(short nNmcNo, short nMapIndex, short nDataIndex, PNMCMAPDATA pNmcMapData);
//------------------------------------------------------------------------------

short WINAPI nmc_SetGantryAxis (short nNmcNo, short nGroupNo,short nMain, short nSub);
short WINAPI nmc_SetGantryEnable(short nNmcNo, short nGroupNo, short nGantryEnable);
short WINAPI nmc_GetPowerResetCount(short nNmcNo, long *plResetCount);


//
//------------------------------------------------------------------------------

short WINAPI nmc_ContRun(short nNmcNo, short nGroupNo, short nRunMode);
short WINAPI nmc_GetContStatus (short nNmcNo, PNMCCONTSTATUS pContStatus);

short WINAPI nmc_SetContNodeLine(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dPos0, double dPos1, double dStart,double dAcc, double dDec , double dDriveSpeed);
short WINAPI nmc_SetContNodeArc(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dCenter0, double dCenter1, double dAngle, double dStart,double dAcc, double dDec, double dDriveSpeed);
short WINAPI nmc_ContNodeClear(short nNmcNo,short nGroupNo);


// Not Support
#ifdef __cplusplus
	}
#endif

#endif