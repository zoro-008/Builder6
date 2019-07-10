#ifndef __AXT_KE_CAMC_FS_H__
#define __AXT_KE_CAMC_FS_H__

#include "AxtLIBDef.h"
#include "CAMCFSDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTCAMCFS Library - CAMC-FS 1.0 Motion module, 1�� �Լ���
	������ǰ
		SMC-1V02 - CAMC-FS Ver2.0 �̻� 1��
		SMC-2V02 - CAMC-FS Ver2.0 �̻� 2��
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	// ��� ������ ���ȣ������ Ȯ���Ѵ�
	BOOL   PASCAL EXPORT CFSKeIsEnableAxis(INT16 axis);
	
	// Port Write/Read
	void   PASCAL EXPORT CFSKeSetPortData(INT16 axis, FSPORT_DATA_WRITE pdwItem, UINT8 wValue);
	UINT8  PASCAL EXPORT CFSKeGetPortData(INT16 axis, FSPORT_DATA_READ pdrItem);

	// Command write
	void   PASCAL EXPORT CFSKeSetCommand(INT16 axis, FSCOMMAND ccCommand);
	void   PASCAL EXPORT CFSKeSetCommandData(INT16 axis, FSCOMMAND ccCommand, UINT32 ulData);
	UINT32 PASCAL EXPORT CFSKeGetCommandData(INT16 axis, FSCOMMAND ccCommand);

	// Data1/2/3/4 Write/Read
	void   PASCAL EXPORT CFSKeSetData1(INT16 axis, UINT8 byData);
	void   PASCAL EXPORT CFSKeSetData2(INT16 axis, UINT8 byData);
	void   PASCAL EXPORT CFSKeSetData3(INT16 axis, UINT8 byData);
	void   PASCAL EXPORT CFSKeSetData4(INT16 axis, UINT8 byData);
	UINT8  PASCAL EXPORT CFSKeGetData1(INT16 axis);
	UINT8  PASCAL EXPORT CFSKeGetData2(INT16 axis);
	UINT8  PASCAL EXPORT CFSKeGetData3(INT16 axis);
	UINT8  PASCAL EXPORT CFSKeGetData4(INT16 axis);

	// Preset pulse drive : ���� �޽��� ����
	void   PASCAL EXPORT CFSKePresetPulseDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, INT32 nTotalPulses);
	// Continous drive : ���� ���� ����
	void   PASCAL EXPORT CFSKeContinuousDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, MOVE_DIRECTION nDirection);
	// Signal search 1 drive : ��ȣ ����-1 ���� 
	void   PASCAL EXPORT CFSKeSignalSearch1Drive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, MOVE_DIRECTION nDirection);
	// Signal search 2 drive : ��ȣ ����-2 ���� 
	void   PASCAL EXPORT CFSKeSignalSearch2Drive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, MOVE_DIRECTION nDirection);

	// ���� �ӵ��� ?
	//UINT32->double
	double PASCAL EXPORT CFSKeGetCurrentSpeed(INT16 axis);
	UINT8  PASCAL EXPORT CFSKeGetUniversalSignal(INT16 axis);

	// ���� ����
	BOOL   PASCAL EXPORT CFSKeInMotion(INT16 axis);
	BOOL   PASCAL EXPORT CFSKeMotionDone(INT16 axis);
	UINT16 PASCAL EXPORT CFSKeWaitForDone(INT16 axis);
	
	BOOL   PASCAL EXPORT CFSKeStartMove(INT16 axis, double dDistance, double dVelocity, double dAcceleration);
	UINT16 PASCAL EXPORT CFSKeMove(INT16 axis, double dDistance, double dVelocity, double dAcceleration);
	
	void   PASCAL EXPORT CFSKeSetDetectSignal(INT16 axis, UINT8 signal);
	UINT8  PASCAL EXPORT CFSKeGetDetectSignal(INT16 axis);	//<+> 2002/12/06 JNS
	
	BOOL   PASCAL EXPORT CFSKeMoveToCW(INT16 axis);
	BOOL   PASCAL EXPORT CFSKeMoveToCCW(INT16 axis);
	
	BOOL   PASCAL EXPORT CFSKeSetSStop(INT16 axis);
	BOOL   PASCAL EXPORT CFSKeSetEStop(INT16 axis);
	
	// Rage data Write/Read
	void   PASCAL EXPORT CFSKeSetRangeData(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetRangeData(INT16 axis);

	// Start/stop speed data Write/Read
	void   PASCAL EXPORT CFSKeSetStartStopSpeedData(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetStartStopSpeedData(INT16 axis);
	
	// Object speed data Write/Read
	void   PASCAL EXPORT CFSKeSetObjectSpeedData(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetObjectSpeedData(INT16 axis);

	// Rate 1/2/3 Write/Read
	void   PASCAL EXPORT CFSKeSetRate1Data(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetRate1Data(INT16 axis);
	void   PASCAL EXPORT CFSKeSetRate2Data(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetRate2Data(INT16 axis);
	void   PASCAL EXPORT CFSKeSetRate3Data(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetRate3Data(INT16 axis);

	// Rate change point 12/23 Write/Read
	void   PASCAL EXPORT CFSKeSetRateChangePoint12(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetRateChangePoint12(INT16 axis);
	void   PASCAL EXPORT CFSKeSetRateChangePoint23(INT16 axis, UINT16 ulData);
	UINT16 PASCAL EXPORT CFSKeGetRateChangePoint23(INT16 axis);

	void   PASCAL EXPORT CFSKeSetSlowDownRearPulse(INT16 axis, UINT32 ulData);
	UINT32 PASCAL EXPORT CFSKeGetSlowDownRearPulse(INT16 axis);
	UINT16 PASCAL EXPORT CFSKeGetCurrentSpeedData(INT16 axis);
	void   PASCAL EXPORT CFSKeSetPresetPulseDataOverride(INT16 axis, UINT32 ulData);
	UINT32 PASCAL EXPORT CFSKeGetPresetPulseData(INT16 axis);
	INT16  PASCAL EXPORT CFSKeGetDeviationData(INT16 axis);	//<+> 2002/12/06 JNS

	// Inposition signal Enable/Disable
	void   PASCAL EXPORT CFSKeSetInpositionWaitMode(INT16 axis);
	void   PASCAL EXPORT CFSKeResetInpositionWaitMode(INT16 axis);
	
	// Alarm signal Enable/Disable
	void   PASCAL EXPORT CFSKeSetAlarmStopEnableMode(INT16 axis);
	void   PASCAL EXPORT CFSKeResetAlarmStopEnableMode(INT16 axis);
	
	// Interrupt out Enable/Disable
	void   PASCAL EXPORT CFSKeSetInterruptOutEnableMode(INT16 axis);
	void   PASCAL EXPORT CFSKeResetInterruptOutEnableMode(INT16 axis);
	
	void   PASCAL EXPORT CFSKeSlowDownStop(INT16 axis);
	void   PASCAL EXPORT CFSKeEmergencyStop(INT16 axis);

	void   PASCAL EXPORT CFSKeSetInternalCounter(INT16 axis, INT32 ulData);
	INT32  PASCAL EXPORT CFSKeGetInternalCounter(INT16 axis);
	void   PASCAL EXPORT CFSKeSetInternalComparatorData(INT16 axis, INT32 ulData);
	INT32  PASCAL EXPORT CFSKeGetInternalComparatorData(INT16 axis);
	
	void   PASCAL EXPORT CFSKeSetExternalCounter(INT16 axis, INT32 ulData);
	INT32  PASCAL EXPORT CFSKeGetExternalCounter(INT16 axis);
	void   PASCAL EXPORT CFSKeSetExternalComparatorData(INT16 axis, INT32 ulData);
	INT32  PASCAL EXPORT CFSKeGetExternalComparatorData(INT16 axis);

	//<+> 2002/12/06 JNS
	// ������ ��带 �����Ѵ�
	BOOL   PASCAL EXPORT CFSKeSetChipOperationSelect(INT16 axis, UINT8 bSelect);
			/* -----------------------------------------------------------------
				axis    : ���ȣ
				bSelect : ������ ���
					0(SYM_LINEAR)	= ��Ī ��ٸ���
					1(ASYM_LINEAR)	= ���Ī ��ٸ���
					2(SYM_CURVE)	= ��Ī ������(S-Curve)
					3(ASYM_CURVE)	= ���Ī ������(S-Curve)
				���ϰ�	: ���������� ������ �Ǹ� 1(TRUE)
				* CFSKePresetPulseDrive(), CFSKeContinuousDrive(), CFSKeSignalSearch1Drive()
				  �Լ��� �����ϱ� ���� ������ ��带 �����ؾ� �Ѵ�
			----------------------------------------------------------------- */

	//<+> 2003/2/07 LSJ, ��Ī
	// ��Ī, ������
	BOOL   PASCAL EXPORT CFSKeGetOptimizeDriveData(INT16 axis, double dStartStop, double  dVelocity, double  dAcceleration, 
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);
	// ��Ī, ���ӽð�
	BOOL   PASCAL EXPORT CFSKeGetOptimizeDriveDataEx(INT16 axis, double dStartStop, double dVelocity, double dAccelTime,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);
	// �ӵ��������̵�(mode=1), ��Ī, ������
	BOOL   PASCAL EXPORT CFSKeGetOptimizeSpeedData(UINT8 mode, INT16 axis, double dStartStopSpeed, double dVelocity, double dAcceleration,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);
	// �ӵ��������̵�(mode=1), ��Ī, ���ӽð�
	BOOL   PASCAL EXPORT CFSKeGetOptimizeSpeedDataEx(UINT8 mode, INT16 axis, double dStartStopSpeed, double dVelocity, double dAccelTime,
				UINT16 *pRangeData, UINT16 *pStartStopSpeedData, UINT16 *pObjectSpeedData, UINT16 *pRate1, UINT16 *pRateChange12);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_KE_CAMC_FS_H__