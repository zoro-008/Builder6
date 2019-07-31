#ifndef __AXT_KE_CAMC_IP_H__
#define __AXT_KE_CAMC_IP_H__

#include "AxtLIBDef.h"
#include "CAMCIPDef.h"


/*------------------------------------------------------------------------------------------------*
    AXTKeCAMCIP Library - CAMC-IP Motion module
    적용제품
        SMC-2V03(ver 2.0 이하) - CAMC-IP 2축
    라이브러리 버전 : v2.0
    마지막 수정일 : 2005. 12. 28.
    사용상 주의사항 : Ke 라이브러리를 기술지원없이 임의로 사용 할 경우 이상동작을 할 수 있습니다.
    기술지원 문의 : Tel. 031-436-2180(아진엑스텍 기술지원팀)
 *------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	//-==================================================================================================================
	// 사용 가능한 축번호인지를 확인한다.
	BOOL   PASCAL EXPORT CIPKeIsEnableAxis(INT16 axis);

	// 메인클럭 설정( 모듈에 장착된 Oscillator가 변경될 경우에만 설정)
    void    PASCAL EXPORT CIPKeSetMainClk(INT16 axis, INT32 nMainClk);

	// Port Write/Read
    void   PASCAL EXPORT CIPKeSetPortData(INT16 axis, IPPORT_DATA_WRITE pdwItem, UINT8 wValue);
    UINT8  PASCAL EXPORT CIPKeGetPortData(INT16 axis, IPPORT_DATA_READ pdrItem);

	// Command write
    void   PASCAL EXPORT CIPKeSetCommand(INT16 axis, IPCOMMAND ccCommand);
    void   PASCAL EXPORT CIPKeSetCommandData(INT16 axis, IPCOMMAND ccCommand, UINT32 ulData);
    UINT32 PASCAL EXPORT CIPKeGetCommandData(INT16 axis, IPCOMMAND ccCommand);
	void   PASCAL EXPORT CIPKeSetCommonCommand(INT16 axis, IPCOMMAND ccCommand);
	void   PASCAL EXPORT CIPKeSetCommonCommandData(INT16 axis, IPCOMMAND ccCommand, UINT32 ulData);

	// Module configure register Write/Read(Refer H/W manuals.)
    UINT8 PASCAL EXPORT CIPReadCRCXSETTING(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteCRCXSETTING(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);
    UINT8 PASCAL EXPORT CIPReadCRCYSETTING(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteCRCYSETTING(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);
    UINT8 PASCAL EXPORT CIPReadInternalSetting(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteInternalSetting(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);
    UINT8 PASCAL EXPORT CIPReadXIOSetting(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteXIOSetting(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);
    UINT8 PASCAL EXPORT CIPReadYIOSetting(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteYIOSetting(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);
    UINT8 PASCAL EXPORT CIPReadXSYNCTRIGSetting(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteXSYNCTRIGSetting(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);
    UINT8 PASCAL EXPORT CIPReadYSYNCTRIGSetting(INT16 nBoardNo, INT16 nModulePos);
    void  PASCAL EXPORT CIPWriteYSYNCTRIGSetting(INT16 nBoardNo, INT16 nModulePos, UINT8 setting_data);

	// Range data write/read
	void   PASCAL EXPORT CIPKeSetRangeData(INT16 axis, UINT16 ulData);					// C:80	
	UINT16 PASCAL EXPORT CIPKeGetRangeData(INT16 axis);                             	// C:00
                                                                                                
	// Start/Stop Speed data write/read                                                         
	void   PASCAL EXPORT CIPKeSetStartStopSpeedData(INT16 axis, UINT16 ulData);     	// C:81
	UINT16 PASCAL EXPORT CIPKeGetStartStopSpeedData(INT16 axis);                    	// C:01
                                                                                                
	// Object Speed data write/read                                                             
	void   PASCAL EXPORT CIPKeSetObjectSpeedData(INT16 axis, UINT16 ulData);        	// C:82
	UINT16 PASCAL EXPORT CIPKeGetObjectSpeedData(INT16 axis);                       	// C:02
                                                                                                
	// Rate 1/2/3 data write/read                                                               
	void   PASCAL EXPORT CIPKeSetRate1Data(INT16 axis, UINT16 ulData);              	// C:83
	UINT16 PASCAL EXPORT CIPKeGetRate1Data(INT16 axis);                             	// C:03
	void   PASCAL EXPORT CIPKeSetRate2Data(INT16 axis, UINT16 ulData);              	// C:84
	UINT16 PASCAL EXPORT CIPKeGetRate2Data(INT16 axis);                             	// C:04
	void   PASCAL EXPORT CIPKeSetRate3Data(INT16 axis, UINT16 ulData);              	// C:85
	UINT16 PASCAL EXPORT CIPKeGetRate3Data(INT16 axis);                             	// C:05
                                                                                                
	// RCP(Rate Change Point) 12/23 write/read                                                  
	void   PASCAL EXPORT CIPKeSetRateChangePoint12(INT16 axis, UINT16 ulData);      	// C:86
	UINT16 PASCAL EXPORT CIPKeGetRateChangePoint12(INT16 axis);                     	// C:06
	void   PASCAL EXPORT CIPKeSetRateChangePoint23(INT16 axis, UINT16 ulData);      	// C:87
	UINT16 PASCAL EXPORT CIPKeGetRateChangePoint23(INT16 axis);                     	// C:07

	// Preset pulse drive
	void   PASCAL EXPORT CIPKePresetPulseDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, INT32 nTotalPulses);
	// Continuous speed drive
	void   PASCAL EXPORT CIPKeContinuousDrive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, MOVE_DIRECTION nDirection);
	// Signal search 1 drive
	void   PASCAL EXPORT CIPKeSignalSearch1Drive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, MOVE_DIRECTION nDirection);
	// Signal search 2 drive
	void   PASCAL EXPORT CIPKeSignalSearch2Drive(INT16 axis, 
				UINT16 ulRange, UINT16 ulSTSPSpeedData, MOVE_DIRECTION nDirection);

	// Sensor Positioning drive
	void   PASCAL EXPORT CIPKeSensorPositioningDrive(INT16 axis,
				UINT16 ulRange, UINT16 ulSTSPSpeedData, UINT16 ulObjectSpeedData,
				UINT16 ulRate1Data, UINT16 ulRate2Data, UINT16 ulRate3Data,
				UINT16 ulRCPoint12, UINT16 ulRCPoint23, INT32 nTotalPulses, UINT8 drive);

	// MPG drive
	void   PASCAL EXPORT CIPKeSetMpgOperation(INT16 axis, UINT8 mode);
	UINT8  PASCAL EXPORT CIPKeGetMpgOperation(INT16 axis);
	void   PASCAL EXPORT CIPKeSetMpgPresetPulseData(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetMpgPresetPulseData(INT16 axis);

	// Interrupt Mask bank1/2 write/read
	void   PASCAL EXPORT CIPKeSetInterruptOutEnableMode(INT16 axis, UINT32 bank1, UINT32 bank2);
	void   PASCAL EXPORT CIPKeResetInterruptOutEnableMode(INT16 axis);
	// Interrupt flag register of Base carrier read
	UINT8  PASCAL EXPORT CIPKeGetInterruptInfo(INT16 nBoardNo);

	// Motion status
	BOOL   PASCAL EXPORT CIPKeInMotion(INT16 axis);
	BOOL   PASCAL EXPORT CIPKeMotionDone(INT16 axis);
	UINT16 PASCAL EXPORT CIPKeWaitForDone(INT16 axis);

	// Current speed
	double PASCAL EXPORT CIPKeGetCurrentSpeed(INT16 axis);

	// Universal Input/Output
	void   PASCAL EXPORT CIPKeSetUniversalSignal(INT16 axis, UINT16 wState);
	UINT16 PASCAL EXPORT CIPKeGetUniversalSignal(INT16 axis);
	void   PASCAL EXPORT CIPKeSetUniversalSignalBit(INT16 axis, IPUNIVERSAL_SIGNAL usBit, UINT8 byState);
	UINT8  PASCAL EXPORT CIPKeGetUniversalSignalBit(INT16 axis, IPUNIVERSAL_SIGNAL usBit);

	// Motion Move
	BOOL   PASCAL EXPORT CIPKeStartMove(INT16 axis, double dDistance, double dVelocity, double dAcceleration);
	UINT16 PASCAL EXPORT CIPKeMove(INT16 axis, double dDistance, double dVelocity, double dAcceleration);
	BOOL   PASCAL EXPORT CIPKeMoveToCW(INT16 axis);
	BOOL   PASCAL EXPORT CIPKeMoveToCCW(INT16 axis);

	// Detect signal configure : signal search 1/2
	void   PASCAL EXPORT CIPKeSetDetectSignal(INT16 axis, UINT8 signal);
	UINT8  PASCAL EXPORT CIPKeGetDetectSignal(INT16 axis);

	// Internal counter
	void   PASCAL EXPORT CIPKeSetInternalCounter(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetInternalCounter(INT16 axis);
	void   PASCAL EXPORT CIPKeSetInternalComparatorData(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetInternalComparatorData(INT16 axis);

	// External counter
	void   PASCAL EXPORT CIPKeSetExternalCounter(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetExternalCounter(INT16 axis);
	void   PASCAL EXPORT CIPKeSetExternalComparatorData(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetExternalComparatorData(INT16 axis);

	// Script(axis number가 Y축(홀수) 일때 만 유효)
	void   PASCAL EXPORT CIPKeSetScript1Event(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScript1Event(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScript2Event(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScript2Event(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScript3Event(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScript3Event(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScriptQueueEvent(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScriptQueueEvent(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScript1Data(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScript1Data(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScript2Data(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScript2Data(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScript3Data(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScript3Data(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScriptQueueData(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetScriptQueueData(INT16 axis);
	void   PASCAL EXPORT CIPKeScriptQueueClear(INT16 axis);
	UINT8  PASCAL EXPORT CIPKeGetScriptQueueEventIndex(INT16 axis);
	UINT8  PASCAL EXPORT CIPKeGetScriptQueueDataIndex(INT16 axis);
	UINT8  PASCAL EXPORT CIPKeGetScriptQueueFlag(INT16 axis);
	void   PASCAL EXPORT CIPKeSetScriptQueueSize(INT16 axis, UINT16 data);
	UINT16 PASCAL EXPORT CIPKeGetScriptQueueSize(INT16 axis);
	UINT16 PASCAL EXPORT CIPKeGetScriptQueueStatus(INT16 axis);

	// Caption(axis number가 Y축(홀수) 일때 만 유효)
	void   PASCAL EXPORT CIPKeSetCaption1Event(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetCaption1Event(INT16 axis);
	void   PASCAL EXPORT CIPKeSetCaption2Event(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetCaption2Event(INT16 axis);
	void   PASCAL EXPORT CIPKeSetCaption3Event(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetCaption3Event(INT16 axis);
	void   PASCAL EXPORT CIPKeSetCaptionQueueEvent(INT16 axis, UINT32 data);
	UINT32 PASCAL EXPORT CIPKeGetCaptionQueueEvent(INT16 axis);
	UINT32 PASCAL EXPORT CIPKeGetCaption1Data(INT16 axis);
	UINT32 PASCAL EXPORT CIPKeGetCaption2Data(INT16 axis);
	UINT32 PASCAL EXPORT CIPKeGetCaption3Data(INT16 axis);
	UINT32 PASCAL EXPORT CIPKeGetCaptionQueueData(INT16 axis);
	void   PASCAL EXPORT CIPKeCaptionQueueClear(INT16 axis);
	UINT8  PASCAL EXPORT CIPKeGetCaptionQueueEventIndex(INT16 axis);
	UINT8  PASCAL EXPORT CIPKeGetCaptionQueueDataIndex(INT16 axis);
	UINT8  PASCAL EXPORT CIPKeGetCaptionQueueFlag(INT16 axis);
	UINT16 PASCAL EXPORT CIPKeGetCaptionQueueSize(INT16 axis);
	void   PASCAL EXPORT CIPKeSetCaptionQueueSize(INT16 axis, UINT16 data);
	UINT16 PASCAL EXPORT CIPKeGetCaptionQueueStatus(INT16 axis);

	// Script/Caption
	BOOL   PASCAL EXPORT CIPKeSetScriptCaption(INT16 axis, INT16 script, UINT32 event, UINT32 data);
	BOOL   PASCAL EXPORT CIPKeGetScriptCaption(INT16 axis, INT16 script, UINT32 *event, UINT32 *data);

	// Software Limit
	void   PASCAL EXPORT CIPKeSetSoftLimitMode(INT16 axis, UINT8 mode);
	UINT8  PASCAL EXPORT CIPKeGetSoftLimitMode(INT16 axis);
	void   PASCAL EXPORT CIPKeSetSoftNLimitPosition(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetSoftNLimitPosition(INT16 axis);
	void   PASCAL EXPORT CIPKeSetSoftPLimitPosition(INT16 axis, INT32 lData);
	INT32  PASCAL EXPORT CIPKeGetSoftPLimitPosition(INT16 axis);

	// Internal variable setting
	void   PASCAL EXPORT CIPKeSetMaxVelocity(INT16 axis, double dVelocity);
	void   PASCAL EXPORT CIPKeSetMaxVelocityAll(INT16 len, INT16 *pAxis, double *pVelocity);
	void   PASCAL EXPORT CIPKeSetMoveUnit(INT16 axis, double dMoveUnitPerPulse);
	void   PASCAL EXPORT CIPKeSetMoveUnitAll(INT16 len, INT16 *pAxis, double *pMoveUnitPerPulse);

	// Symmetric, Acceleration rate
	BOOL   PASCAL EXPORT CIPKeGetOptimizeDriveData(
				INT16   axis,
				double  dStartStop, 
				double  dVelocity, 
				double  dAcceleration, 
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData, 
				UINT16 *pObjectSpeedData, 
				UINT16 *pRate1,
				UINT16 *pRateChange12);

	// Symmetric, Acceleration time
	BOOL   PASCAL EXPORT CIPKeGetOptimizeDriveDataEx(
				INT16   axis,
				double  dStartStop, 
				double  dVelocity, 
				double  dAccelTime, 
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData, 
				UINT16 *pObjectSpeedData, 
				UINT16 *pRate1,
				UINT16 *pRateChange12);

	// Asymmetric, Acceleration rate
	BOOL	PASCAL EXPORT CIPKeGetOptimizeDriveDataA(
				INT16   axis,
				double  dStartStop, 
				double  dVelocity, 
				double  dAcceleration, 
				double  dDeceleration, 
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData, 
				UINT16 *pObjectSpeedData, 
				UINT16 *pRateAccel,
				UINT16 *pRateDecel,
				UINT16 *pRateChange12);

	// Asymmetric, Acceleration time
	BOOL   PASCAL EXPORT CIPKeGetOptimizeDriveDataAEx(
				INT16   axis,
				double  dStartStop, 
				double  dVelocity, 
				double  dAccelTime, 
				double  dDecelTime, 
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData, 
				UINT16 *pObjectSpeedData, 
				UINT16 *pRateAccel,
				UINT16 *pRateDecel,
				UINT16 *pRateChange12);

	// Velocity override(mode = 1). Symmetric, Acceleration rate
	BOOL   PASCAL EXPORT CIPKeGetOptimizeSpeedData(
				UINT8   mode,
				INT16   axis,
				double  dStartStopSpeed,
				double  dVelocity,
				double  dAcceleration,
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData,
				UINT16 *pObjectSpeedData,
				UINT16 *pRate1,
				UINT16 *pRateChange12);

	// Velocity override(mode = 1). Symmetric, Acceleration time
	BOOL   PASCAL EXPORT CIPKeGetOptimizeSpeedDataEx(
				UINT8   mode,
				INT16   axis,
				double  dStartStopSpeed,
				double  dVelocity,
				double  dAccelTime,
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData,
				UINT16 *pObjectSpeedData,
				UINT16 *pRate1,
				UINT16 *pRateChange12);

	// Velocity override(mode = 1). Asymmetric, Acceleration rate
	BOOL   PASCAL EXPORT CIPKeGetOptimizeSpeedDataA(
				UINT8   mode,
				INT16   axis,
				double  dStartStopSpeed,
				double  dVelocity,
				double  dAcceleration,
				double  dDeceleration,
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData,
				UINT16 *pObjectSpeedData,
				UINT16 *pRateAccel,
				UINT16 *pRateDecel,
				UINT16 *pRateChange12);

	// Velocity override(mode = 1). Asymmetric, Acceleration time
	BOOL   PASCAL EXPORT CIPKeGetOptimizeSpeedDataAEx(
				UINT8   mode,
				INT16   axis,
				double  dStartStopSpeed,
				double  dVelocity,
				double  dAccelTime,
				double  dDecelTime,
				UINT16 *pRangeData,
				UINT16 *pStartStopSpeedData,
				UINT16 *pObjectSpeedData,
				UINT16 *pRateAccel,
				UINT16 *pRateDecel,
				UINT16 *pRateChange12);

	// Stop command
	BOOL   PASCAL EXPORT CIPKeSetSStop(INT16 axis);
	BOOL   PASCAL EXPORT CIPKeSetEStop(INT16 axis);
	void   PASCAL EXPORT CIPKeSlowDownStop(INT16 axis);
	void   PASCAL EXPORT CIPKeEmergencyStop(INT16 axis);

	// SlowDownRead pulse register
	void   PASCAL EXPORT CIPKeSetSlowDownRearPulse(INT16 axis, UINT32 ulData);
	UINT32 PASCAL EXPORT CIPKeGetSlowDownRearPulse(INT16 axis);

	// Current speed register
	UINT16 PASCAL EXPORT CIPKeGetCurrentSpeedData(INT16 axis);

	// Preset drive pulse override
	void   PASCAL EXPORT CIPKeSetPresetPulseDataOverride(INT16 axis, UINT32 ulData);

	// Preset drive pulse register
	UINT32 PASCAL EXPORT CIPKeGetPresetPulseData(INT16 axis);

	// Deviation register
	INT16  PASCAL EXPORT CIPKeGetDeviationData(INT16 axis);

	// Inposition mode
	void   PASCAL EXPORT CIPKeSetInpositionWaitMode(INT16 axis);
	void   PASCAL EXPORT CIPKeResetInpositionWaitMode(INT16 axis);

	// Alarm mode
	void   PASCAL EXPORT CIPKeSetAlarmStopEnableMode(INT16 axis);
	void   PASCAL EXPORT CIPKeResetAlarmStopEnableMode(INT16 axis);

	// Drive mode
	BOOL   PASCAL EXPORT CIPKeSetChipOperationSelect(INT16 axis, UINT8 bySelect);
	UINT8  PASCAL EXPORT CIPKeGetChipOperationSelect(INT16 axis);

	// Chip Function con register
	BOOL   PASCAL EXPORT CIPKeSetChipFunction(INT16 axis, UINT32 wFunction);
	UINT32 PASCAL EXPORT CIPKeGetChipFunction(INT16 axis);

	// ESTOP signal mode
	void   PASCAL EXPORT CIPKeSetEStopSignalEnableMode(INT16 axis);
	void   PASCAL EXPORT CIPKeResetEStopSignalEnableMode(INT16 axis);

	// EndLimit signal mode
	void   PASCAL EXPORT CIPKeSetEndLimitStopEnableMode(INT16 axis);
	void   PASCAL EXPORT CIPKeResetEndLimitStopEnableMode(INT16 axis);

	// SlowDown limit signal mode
	void   PASCAL EXPORT CIPKeSetSlowLimitStopEnableMode(INT16 axis);
	void   PASCAL EXPORT CIPKeResetSlowLimitStopEnableMode(INT16 axis);

	// Mode2 register
	UINT8  PASCAL EXPORT CIPmode2_get_bit(INT16 axis, UINT8 bBitNo);
	BOOL   PASCAL EXPORT CIPmode2_change_bit(INT16 axis, UINT8 bBitNo, UINT8 bState);

	// MeChanical register
	UINT8  PASCAL EXPORT CIPmechanical_get_bit(INT16 axis, UINT8 bBitNo);

	// Base carrier board Sync register
	BOOL   PASCAL EXPORT CIPKeset_sync(INT16 nBoardNo, UINT8 byState);
	UINT8  PASCAL EXPORT CIPKeget_sync_info(INT16 nBoardNo);

	// Board register access with address
	UINT8  PASCAL EXPORT CIPget_board_reg_byte(INT16 axis, INT16 addr);

	// Gantry mode setting
	BOOL PASCAL EXPORT CIPKeSetGantryMode(INT16 axis, UINT8 bMode);
	BOOL PASCAL EXPORT CIPKeGetGantryMode(INT16 axis);

	// Dynamic Link setting
	BOOL PASCAL EXPORT CIPKeSetDynamic1Mode(INT16 axis, UINT8 bMode);
	BOOL PASCAL EXPORT CIPKeGetDynamic1Mode(INT16 axis);
	BOOL PASCAL EXPORT CIPKeSetDynamic2Mode(INT16 axis, UINT8 bMode);
	BOOL PASCAL EXPORT CIPKeGetDynamic2Mode(INT16 axis);

	// Dynamic Link signal setting.
	BOOL	PASCAL EXPORT CIPKeSetDynamic1Signal(INT16 axis, UINT8 bState);
	UINT8	PASCAL EXPORT CIPKeGetDynamic1Signal(INT16 axis);
	BOOL	PASCAL EXPORT CIPKeSetDynamic2Signal(INT16 axis, UINT8 bState);
	UINT8	PASCAL EXPORT CIPKeGetDynamic2Signal(INT16 axis);


#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_KE_CAMC_IP_H__
