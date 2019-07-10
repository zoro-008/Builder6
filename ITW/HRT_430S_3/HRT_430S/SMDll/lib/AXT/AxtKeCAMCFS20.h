#ifndef __AXT_KE_CAMC_FS20_H__
#define __AXT_KE_CAMC_FS20_H__

#include "AxtLIBDef.h"
#include "CAMCFSDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTCAMCFS Library - CAMC-FS 2.0이상 Motion module, 1차 함수군
	적용제품
		SMC-1V02 - CAMC-FS Ver2.0 이상 1축
		SMC-2V02 - CAMC-FS Ver2.0 이상 2축
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	// 사용 가능한 축번호인지를 확인한다
	BOOL   PASCAL EXPORT CFS20KeIsEnableAxis(INT16 axis);

	// Port Write/Read
	void   PASCAL EXPORT CFS20KeSetPortData(INT16 axis, FSPORT_DATA_WRITE pdwItem, UINT8 wValue);
	UINT8  PASCAL EXPORT CFS20KeGetPortData(INT16 axis, FSPORT_DATA_READ pdrItem);

	// Command write
	void   PASCAL EXPORT CFS20KeSetCommand(INT16 axis, FSCOMMAND ccCommand);
	void   PASCAL EXPORT CFS20KeSetCommandData(INT16 axis, FSCOMMAND ccCommand, UINT32 ulData);
	UINT32 PASCAL EXPORT CFS20KeGetCommandData(INT16 axis, FSCOMMAND ccCommand);
	
	// Data1/2/3/4 Write/Read
	void   PASCAL EXPORT CFS20KeSetData1(INT16 axis, UINT8 byData);
	void   PASCAL EXPORT CFS20KeSetData2(INT16 axis, UINT8 byData);
	void   PASCAL EXPORT CFS20KeSetData3(INT16 axis, UINT8 byData);
	void   PASCAL EXPORT CFS20KeSetData4(INT16 axis, UINT8 byData);
	UINT8  PASCAL EXPORT CFS20KeGetData1(INT16 axis);
	UINT8  PASCAL EXPORT CFS20KeGetData2(INT16 axis);
	UINT8  PASCAL EXPORT CFS20KeGetData3(INT16 axis);
	UINT8  PASCAL EXPORT CFS20KeGetData4(INT16 axis);

	/// Range data
	void   PASCAL EXPORT CFS20KeSetRangeData(INT16 axis, UINT16 ulData);				// C:80
	UINT16 PASCAL EXPORT CFS20KeGetRangeData(INT16 axis);								// C:00

	/// Start/Stop Speed data
	void   PASCAL EXPORT CFS20KeSetStartStopSpeedData(INT16 axis, UINT16 wData);		// C:81
	UINT16 PASCAL EXPORT CFS20KeGetStartStopSpeedData(INT16 axis);						// C:01

	/// Object Speed data
	void   PASCAL EXPORT CFS20KeSetObjectSpeedData(INT16 axis, UINT16 wData);			// C:82
	UINT16 PASCAL EXPORT CFS20KeGetObjectSpeedData(INT16 axis);							// C:02

	/// Rate data 1/2/3
	void   PASCAL EXPORT CFS20KeSetRate1Data(INT16 axis, UINT16 ulData);				// C:83
	UINT16 PASCAL EXPORT CFS20KeGetRate1Data(INT16 axis);								// C:03
	void   PASCAL EXPORT CFS20KeSetRate2Data(INT16 axis, UINT16 ulData);				// C:84
	UINT16 PASCAL EXPORT CFS20KeGetRate2Data(INT16 axis);								// C:04
	void   PASCAL EXPORT CFS20KeSetRate3Data(INT16 axis, UINT16 ulData);				// C:85
	UINT16 PASCAL EXPORT CFS20KeGetRate3Data(INT16 axis);								// C:05

	/// RCP(Rate Change Point) 12/23
	void   PASCAL EXPORT CFS20KeSetRateChangePoint12(INT16 axis, UINT16 ulData);		// C:86
	UINT16 PASCAL EXPORT CFS20KeGetRateChangePoint12(INT16 axis);						// C:06
	void   PASCAL EXPORT CFS20KeSetRateChangePoint23(INT16 axis, UINT16 ulData);		// C:87
	UINT16 PASCAL EXPORT CFS20KeGetRateChangePoint23(INT16 axis);						// C:07

/*
	//! 08 / 88
	BOOL   PASCAL EXPORT CFS20KeSetSW1Data(INT16 axis, UINT16 wSW1);					// C:88
	UINT16 PASCAL EXPORT CFS20KeGetSW1Data(INT16 axis);									// C:08
	//! 09 / 89
	BOOL   PASCAL EXPORT CFS20KeSetSW2Data(INT16 axis, UINT16 wSW2);					// C:89
	UINT16 PASCAL EXPORT CFS20KeGetSW2Data(INT16 axis);									// C:09
*/

	// Preset pulse drive
	void   PASCAL EXPORT CFS20KePresetPulseDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, INT32 nTotalPulses);
	// Continous drive
	void   PASCAL EXPORT CFS20KeContinuousDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, MOVE_DIRECTION nDirection);
	// Signal search 1 drive
	void   PASCAL EXPORT CFS20KeSignalSearch1Drive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, MOVE_DIRECTION nDirection);
	// Signal search 2 drive
	void   PASCAL EXPORT CFS20KeSignalSearch2Drive(INT16 axis,
				UINT16 ulRange, UINT16 ulSTSPSpeedData, MOVE_DIRECTION nDirection);

	// 2004.03.31 안병건 추가
	void   PASCAL EXPORT CFS20KeSensorPositioningDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, INT32 nTotalPulses, UINT8 drive);
	
	//UINT32->double
	double PASCAL EXPORT CFS20KeGetCurrentSpeed(INT16 axis);

	/// Universal Input/Output
	void   PASCAL EXPORT CFS20KeSetUniversalSignal(INT16 axis, UINT16 wState);
	UINT16 PASCAL EXPORT CFS20KeGetUniversalSignal(INT16 axis);
	void   PASCAL EXPORT CFS20KeSetUniversalSignalBit(INT16 axis, FSUNIVERSAL_SIGNAL usBit, UINT8 byState);
	UINT8  PASCAL EXPORT CFS20KeGetUniversalSignalBit(INT16 axis, FSUNIVERSAL_SIGNAL usBit);

	void   PASCAL EXPORT CFS20KeSetUniversalSignal2(INT16 axis, UINT16 wState);
	UINT16 PASCAL EXPORT CFS20KeGetUniversalSignal2(INT16 axis);

	/// Motion status
	BOOL   PASCAL EXPORT CFS20KeInMotion(INT16 axis);
	BOOL   PASCAL EXPORT CFS20KeMotionDone(INT16 axis);
	UINT16 PASCAL EXPORT CFS20KeWaitForDone(INT16 axis);

	/// Motion move
	BOOL   PASCAL EXPORT CFS20KeStartMove(INT16 axis, double dDistance, double dVelocity, double dAcceleration);
	UINT16 PASCAL EXPORT CFS20KeMove(INT16 axis, double dDistance, double dVelocity, double dAcceleration);
	BOOL   PASCAL EXPORT CFS20KeMoveToCW(INT16 axis);
	BOOL   PASCAL EXPORT CFS20KeMoveToCCW(INT16 axis);
	
	/// Detect signal : Signal search 1/2
	void   PASCAL EXPORT CFS20KeSetDetectSignal(INT16 axis, UINT8 signal);
	UINT8  PASCAL EXPORT CFS20KeGetDetectSignal(INT16 axis);							//<+> 2002/12/06 JNS
	
	void   PASCAL EXPORT CFS20KeSetSlowDownRearPulse(INT16 axis, UINT32 ulData);
	UINT32 PASCAL EXPORT CFS20KeGetSlowDownRearPulse(INT16 axis);
	UINT16 PASCAL EXPORT CFS20KeGetCurrentSpeedData(INT16 axis);
	void   PASCAL EXPORT CFS20KeSetPresetPulseDataOverride(INT16 axis, UINT32 ulData);
	UINT32 PASCAL EXPORT CFS20KeGetPresetPulseData(INT16 axis);
	INT16  PASCAL EXPORT CFS20KeGetDeviationData(INT16 axis);	//<*> 2002/12/06 JNS

	/// Inposition Enable/Disable
	void   PASCAL EXPORT CFS20KeSetInpositionWaitMode(INT16 axis);
	void   PASCAL EXPORT CFS20KeResetInpositionWaitMode(INT16 axis);
	
	/// Alarm Enable/Disable
	void   PASCAL EXPORT CFS20KeSetAlarmStopEnableMode(INT16 axis);
	void   PASCAL EXPORT CFS20KeResetAlarmStopEnableMode(INT16 axis);
	
	/// Interrupt out Enable/Disable
	void   PASCAL EXPORT CFS20KeSetInterruptOutEnableMode(INT16 axis);
	void   PASCAL EXPORT CFS20KeResetInterruptOutEnableMode(INT16 axis);

	/// Motion stop
	void   PASCAL EXPORT CFS20KeSlowDownStop(INT16 axis);								// C:AB
	void   PASCAL EXPORT CFS20KeEmergencyStop(INT16 axis);								// C:AC
	BOOL   PASCAL EXPORT CFS20KeSetSStop(INT16 axis);		//$$ CFS20KeSlowDownStop()와 동일기능
	BOOL   PASCAL EXPORT CFS20KeSetEStop(INT16 axis);		//$$ CFS20KeEmergencyStop()와 동일기능

	//<+> 2002/12/06 JNS
	// 가감속 모드를 설정한다
	BOOL   PASCAL EXPORT CFS20KeSetChipOperationSelect(INT16 axis, UINT8 bSelect);		// C:AD
	UINT8  PASCAL EXPORT CFS20KeGetChipOperationSelect(INT16 axis);						// C:2D	//<+>

	/// Internal counter
	void   PASCAL EXPORT CFS20KeSetInternalCounter(INT16 axis, INT32 ulData);			// C:E0
	INT32  PASCAL EXPORT CFS20KeGetInternalCounter(INT16 axis);							// C:60
	void   PASCAL EXPORT CFS20KeSetInternalComparatorData(INT16 axis, INT32 ulData);	// C:E1
	INT32  PASCAL EXPORT CFS20KeGetInternalComparatorData(INT16 axis);					// C:61
	
	/// External counter
	void   PASCAL EXPORT CFS20KeSetExternalCounter(INT16 axis, INT32 ulData);			// C:E4
	INT32  PASCAL EXPORT CFS20KeGetExternalCounter(INT16 axis);							// C:64
	void   PASCAL EXPORT CFS20KeSetExternalComparatorData(INT16 axis, INT32 ulData);	// C:E5
	INT32  PASCAL EXPORT CFS20KeGetExternalComparatorData(INT16 axis);					// C:65

	//<+> 2002/12/06 JNS
	/// Script
	void   PASCAL EXPORT CFS20KeSetScript1Event(INT16 axis, UINT32 data);				// C:C0
	UINT32 PASCAL EXPORT CFS20KeGetScript1Event(INT16 axis);							// C:40
	void   PASCAL EXPORT CFS20KeSetScript2Event(INT16 axis, UINT32 data);				// C:C1
	UINT32 PASCAL EXPORT CFS20KeGetScript2Event(INT16 axis);							// C:41
	void   PASCAL EXPORT CFS20KeSetScript3Event(INT16 axis, UINT32 data);				// C:C2
	UINT32 PASCAL EXPORT CFS20KeGetScript3Event(INT16 axis);							// C:42
	void   PASCAL EXPORT CFS20KeSetScriptQueueEvent(INT16 axis, UINT32 data);			// C:C3
	UINT32 PASCAL EXPORT CFS20KeGetScriptQueueEvent(INT16 axis);						// C:43
	void   PASCAL EXPORT CFS20KeSetScript1Data(INT16 axis, UINT32 data);				// C:C4
	UINT32 PASCAL EXPORT CFS20KeGetScript1Data(INT16 axis);								// C:44
	void   PASCAL EXPORT CFS20KeSetScript2Data(INT16 axis, UINT32 data);				// C:C5
	UINT32 PASCAL EXPORT CFS20KeGetScript2Data(INT16 axis);								// C:45
	void   PASCAL EXPORT CFS20KeSetScript3Data(INT16 axis, UINT32 data);				// C:C6
	UINT32 PASCAL EXPORT CFS20KeGetScript3Data(INT16 axis);								// C:46
	void   PASCAL EXPORT CFS20KeSetScriptQueueData(INT16 axis, UINT32 data);			// C:C7
	UINT32 PASCAL EXPORT CFS20KeGetScriptQueueData(INT16 axis);							// C:47
	void   PASCAL EXPORT CFS20KeScriptQueueClear(INT16 axis);							// C:C8
	UINT8  PASCAL EXPORT CFS20KeGetScriptQueueEventIndex(INT16 axis);					// C:49
	UINT8  PASCAL EXPORT CFS20KeGetScriptQueueDataIndex(INT16 axis);					// C:4A
	UINT8  PASCAL EXPORT CFS20KeGetScriptQueueFlag(INT16 axis);							// C:4B
	void   PASCAL EXPORT CFS20KeSetScriptQueueSize(INT16 axis, UINT16 data);			// C:CC
	UINT16 PASCAL EXPORT CFS20KeGetScriptQueueSize(INT16 axis);							// C:4C
	UINT16 PASCAL EXPORT CFS20KeGetScriptQueueStatus(INT16 axis);						// C:4D
	/// Caption
	void   PASCAL EXPORT CFS20KeSetCaption1Event(INT16 axis, UINT32 data);				// C:D0
	UINT32 PASCAL EXPORT CFS20KeGetCaption1Event(INT16 axis);							// C:50
	void   PASCAL EXPORT CFS20KeSetCaption2Event(INT16 axis, UINT32 data);				// C:D1
	UINT32 PASCAL EXPORT CFS20KeGetCaption2Event(INT16 axis);							// C:51
	void   PASCAL EXPORT CFS20KeSetCaption3Event(INT16 axis, UINT32 data);				// C:D2
	UINT32 PASCAL EXPORT CFS20KeGetCaption3Event(INT16 axis);							// C:52
	void   PASCAL EXPORT CFS20KeSetCaptionQueueEvent(INT16 axis, UINT32 data);			// C:D3
	UINT32 PASCAL EXPORT CFS20KeGetCaptionQueueEvent(INT16 axis);						// C:53
	UINT32 PASCAL EXPORT CFS20KeGetCaption1Data(INT16 axis);							// C:54
	UINT32 PASCAL EXPORT CFS20KeGetCaption2Data(INT16 axis);							// C:55
	UINT32 PASCAL EXPORT CFS20KeGetCaption3Data(INT16 axis);							// C:56
	UINT32 PASCAL EXPORT CFS20KeGetCaptionQueueData(INT16 axis);						// C:57
	void   PASCAL EXPORT CFS20KeCaptionQueueClear(INT16 axis);							// C:D8
	UINT8  PASCAL EXPORT CFS20KeGetCaptionQueueEventIndex(INT16 axis);					// C:59
	UINT8  PASCAL EXPORT CFS20KeGetCaptionQueueDataIndex(INT16 axis);					// C:5A
	UINT8  PASCAL EXPORT CFS20KeGetCaptionQueueFlag(INT16 axis);						// C:5B
	UINT16 PASCAL EXPORT CFS20KeGetCaptionQueueSize(INT16 axis);						// C:5C
	void   PASCAL EXPORT CFS20KeSetCaptionQueueSize(INT16 axis, UINT16 data);			// C:DC
	UINT16 PASCAL EXPORT CFS20KeGetCaptionQueueStatus(INT16 axis);						// C:5D

	//<+> 2002/12/06 JNS
	/// Script&Caption
	BOOL   PASCAL EXPORT CFS20KeSetScriptCaption(INT16 axis, INT16 sc, UINT32 evnet, UINT32 data);
	BOOL   PASCAL EXPORT CFS20KeGetScriptCaption(INT16 axis, INT16 sc, UINT32 *evnet, UINT32 *data);

	//<+> 2002/12/06 JNS, Soft limit
	void   PASCAL EXPORT CFS20KeSetSoftLimitMode(INT16 axis, UINT8 mode);
	UINT8  PASCAL EXPORT CFS20KeGetSoftLimitMode(INT16 axis);
	void   PASCAL EXPORT CFS20KeSetSoftNLimitPosition(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CFS20KeGetSoftNLimitPosition(INT16 axis);
	void   PASCAL EXPORT CFS20KeSetSoftPLimitPosition(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CFS20KeGetSoftPLimitPosition(INT16 axis);

	//<+> 2002/12/06 JNS, MPG(Manual Pulse Generation)
	void   PASCAL EXPORT CFS20KeSetMpgOperation(INT16 axis, UINT8 mode);
	UINT8  PASCAL EXPORT CFS20KeGetMpgOperation(INT16 axis);
	void   PASCAL EXPORT CFS20KeSetMpgPresetPulseData(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CFS20KeGetMpgPresetPulseData(INT16 axis);

	//<+> 2002/12/06 JNS, 대칭
	// 대칭, 가속율
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeDriveData(INT16 axis, double dStartStop, double  dVelocity, double  dAcceleration, 
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);
	// 대칭, 가속시간
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeDriveDataEx(INT16 axis, double dStartStop, double dVelocity, double dAccelTime,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);
	// 속도오버라이드(mode=1), 대칭, 가속율
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeSpeedData(UINT8 mode, INT16 axis, double dStartStopSpeed, double dVelocity, double dAcceleration,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);
	// 속도오버라이드(mode=1), 대칭, 가속시간
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeSpeedDataEx(UINT8 mode, INT16 axis, double dStartStopSpeed, double dVelocity, double dAccelTime,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);

	//<+> 2002/12/06 JNS, 비대칭
	// 비대칭, 가속율
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeDriveDataA(INT16 axis, double dStartStop, double dVelocity, double dAcceleration, double dDeceleration, 
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRateAccel, UINT16 *pRateDecel, UINT16 *pRateChange12);
	// 비대칭, 가속시간
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeDriveDataAEx(INT16 axis, double dStartStop, double dVelocity, double dAccelTime, double dDecelTime,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRateAccel, UINT16 *pRateDecel, UINT16 *pRateChange12);
	// 속도오버라이드(mode=1), 비대칭, 가속율
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeSpeedDataA(UINT8 mode, INT16 axis, double dStartStopSpeed, double dVelocity, double dAcceleration, double dDeceleration, 
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRateAccel, UINT16 *pRateDecel, UINT16 *pRateChange12);
	// 속도오버라이드(mode=1), 비대칭, 가속시간
	BOOL   PASCAL EXPORT CFS20KeGetOptimizeSpeedDataAEx(UINT8 mode, INT16 axis, double dStartStopSpeed, double dVelocity, double dAccelTime, double dDecelTime, 
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRateAccel, UINT16 *pRateDecel, UINT16 *pRateChange12);

	//<+> gun5, 축 기준으로 레지스터를 읽는 함수
	UINT8  PASCAL EXPORT CFS20get_board_reg_byte(INT16 axis, UINT32 offset);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_KE_CAMC_FS20_H__




/* -----------------------------------------------------------------
	axis    : 축번호
	bSelect : 가감속 모드
		0(SYM_LINEAR)	= 대칭 사다리꼴
		1(ASYM_LINEAR)	= 비대칭 사다리꼴
		2(SYM_CURVE)	= 대칭 포물선(S-Curve)
		3(ASYM_CURVE)	= 비대칭 포물선(S-Curve)
	리턴값	: 정상적으로 설정이 되면 1(TRUE)
	* CFS20KePresetPulseDrive(), CFS20KeContinuousDrive(),
	  CFS20KeSignalSearch1Drive() 함수를 실행하기 전에 가감속
	  모드를 설정해야 한다
----------------------------------------------------------------- */



/*
	UINT32 PASCAL EXPORT CFS20KeGetCaption1Data(INT16 axis);							// C:54
	UINT32 PASCAL EXPORT CFS20KeGetCaption2Data(INT16 axis);							// C:55
	UINT32 PASCAL EXPORT CFS20KeGetCaption3Data(INT16 axis);							// C:56
	UINT32 PASCAL EXPORT CFS20KeGetCaptionQueueData(INT16 axis);						// C:57
*/
