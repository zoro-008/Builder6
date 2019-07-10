#ifndef __AXT_PMD_H__
#define __AXT_PMD_H__

#include "AxtLIBDef.h"
#include "PMDDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTPMD Library - PMD Motion module
	적용제품
		SMC-2V53 - MC2120(PMD) 2축
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// Initilize/Close 
	BOOL	PASCAL EXPORT InitializePMD();
	BOOL	PASCAL EXPORT PMDIsInitialized(void);
	//void	PASCAL EXPORT PMDStopService();

/* <-> 2002.11.15 : 
	UINT16	PASCAL EXPORT PMDBoardReadWord(INT16 board, UINT32 offset);
	void	PASCAL EXPORT PMDBoardWriteWord(INT16 board, UINT32 offset, UINT16 wData);
*/

	void	PASCAL EXPORT PMDPokew(UINT16 module, UINT16 byOffset, UINT16 wData);
	UINT16	PASCAL EXPORT PMDPeekw(UINT16 module, UINT16 byOffset);

	//<+> 2002-08-05
	INT16	PASCAL EXPORT PMDGetModuleNo(INT16 nBoardNo, INT16 nModulePos);
	INT16	PASCAL EXPORT PMDGetBoardNo(UINT32 address);


/* Undefined
	// Window message & procedure
	void	PASCAL EXPORT PMDSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_PMD_INTERRUPT_PROC proc);	
*/

	void	PASCAL EXPORT PMDUpdate(UINT16 module, UINT16 axis);
	void	PASCAL EXPORT PMDMultiUpdate(UINT16 module, UINT16 mask);

	void	PASCAL EXPORT PMDGetHostIOError(UINT16 module, UINT16 *error_code);

	void	PASCAL EXPORT PMDClearInterrupt(UINT16 module);

	/// PID
	void	PASCAL EXPORT PMDSetKp(UINT16 module, UINT16 axis, UINT16 kp);
	void	PASCAL EXPORT PMDGetKp(UINT16 module, UINT16 axis, UINT16 *kp);
	void	PASCAL EXPORT PMDSetKi(UINT16 module, UINT16 axis, UINT16 ki);
	void	PASCAL EXPORT PMDGetKi(UINT16 module, UINT16 axis,UINT16 *ki);
	void	PASCAL EXPORT PMDSetKd(UINT16 module, UINT16 axis, UINT16 kd);
	void	PASCAL EXPORT PMDGetKd(UINT16 module, UINT16 axis, UINT16 *kd);

	void	PASCAL EXPORT PMDSetKaff(UINT16 module, UINT16 axis, UINT16 kaff);
	void	PASCAL EXPORT PMDGetKaff(UINT16 module, UINT16 axis, UINT16 *kaff);
	void	PASCAL EXPORT PMDSetKout(UINT16 module, UINT16 axis, double kout);
	void	PASCAL EXPORT PMDGetKout(UINT16 module, UINT16 axis, double *kout);
	void	PASCAL EXPORT PMDSetKvff(UINT16 module, UINT16 axis, UINT16 kvff);
	void	PASCAL EXPORT PMDGetKvff(UINT16 module, UINT16 axis, UINT16 *kvff);

	void	PASCAL EXPORT PMDSetBreakpoint(UINT16 module, UINT16 axis, UINT16 breakpoint, UINT16 sourceaxis, UINT16 action, UINT16 trigger);
	void	PASCAL EXPORT PMDGetBreakpoint(UINT16 module, UINT16 axis, UINT16 breakpoint, UINT16 *sourceaxis, UINT16 *action, UINT16 *trigger);
	void	PASCAL EXPORT PMDSetBreakpointValue(UINT16 module, UINT16 axis, UINT16 breakpoint, UINT32 value);
	void	PASCAL EXPORT PMDGetBreakpointValue(UINT16 module, UINT16 axis, UINT16 breakpoint, UINT32 *value);

	void	PASCAL EXPORT PMDGetInterruptAxis(UINT16 module, UINT16 *axismask);
	void	PASCAL EXPORT PMDSetInterruptMask(UINT16 module, UINT16 axis, UINT16 mask);
	void	PASCAL EXPORT PMDGetInterruptMask(UINT16 module, UINT16 axis, UINT16 *mask);

	void	PASCAL EXPORT PMDClearPositionError(UINT16 module, UINT16 axis);
	void	PASCAL EXPORT PMDSetAutoStopMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetAutoStopMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDGetDerivative(UINT16 module, UINT16 axis, INT16 *derror);
	void	PASCAL EXPORT PMDSetDerivativeTime(UINT16 module, UINT16 axis, UINT16 time);
	void	PASCAL EXPORT PMDGetDerivativeTime(UINT16 module, UINT16 axis, UINT16 *time);
	void	PASCAL EXPORT PMDGetIntegral(UINT16 module, UINT16 axis, double *Ierror);
	void	PASCAL EXPORT PMDSetIntegrationLimit(UINT16 module, UINT16 axis, double limit);
	void	PASCAL EXPORT PMDGetIntegrationLimit(UINT16 module, UINT16 axis, double *limit);
	void	PASCAL EXPORT PMDSetMotorBias(UINT16 module, UINT16 axis, double bias);
	void	PASCAL EXPORT PMDGetMotorBias(UINT16 module, UINT16 axis, double *bias);
	void	PASCAL EXPORT PMDSetMotorLimit(UINT16 module, UINT16 axis, double limit);
	void	PASCAL EXPORT PMDGetMotorLimit(UINT16 module, UINT16 axis, double *limit);
	void	PASCAL EXPORT PMDGetPositionError(UINT16 module, UINT16 axis, INT32 *position_error);
	void	PASCAL EXPORT PMDSetPositionErrorLimit(UINT16 module, UINT16 axis, UINT32 limit);
	void	PASCAL EXPORT PMDGetPositionErrorLimit(UINT16 module, UINT16 axis, UINT32 *limit);

	void	PASCAL EXPORT PMDAdjustActualPosition(UINT16 module, UINT16 axis, INT32 position);
	void	PASCAL EXPORT PMDSetActualPosition(UINT16 module, UINT16 axis, INT32 position);
	void	PASCAL EXPORT PMDGetActualPosition(UINT16 module, UINT16 axis, INT32 *position);
	void	PASCAL EXPORT PMDGetActualVelocity(UINT16 module, UINT16 axis, double *velocity);

	void	PASCAL EXPORT PMDSetCaptureSource(UINT16 module, UINT16 axis, UINT16 source);
	void	PASCAL EXPORT PMDGetCaptureSource(UINT16 module, UINT16 axis, UINT16 *source);
	void	PASCAL EXPORT PMDGetCaptureValue(UINT16 module, UINT16 axis, INT32 *position);
	void	PASCAL EXPORT PMDSetEncoderSource(UINT16 module, UINT16 axis, UINT16 source);
	void	PASCAL EXPORT PMDGetEncoderSource(UINT16 module, UINT16 axis, UINT16 *source);
	void	PASCAL EXPORT PMDSetBufferStart(UINT16 module, UINT16 bufferID, UINT32 address);
	void	PASCAL EXPORT PMDGetBufferStart(UINT16 module, UINT16 bufferID, UINT32 *address);
	void	PASCAL EXPORT PMDSetBufferLength(UINT16 module, UINT16 bufferID, UINT32 length);
	void	PASCAL EXPORT PMDGetBufferLength(UINT16 module, UINT16 bufferID, UINT32 *length);
	void	PASCAL EXPORT PMDSetBufferReadIndex(UINT16 module, UINT16 bufferID, UINT32 index);
	void	PASCAL EXPORT PMDGetBufferReadIndex(UINT16 module, UINT16 bufferID, UINT32 *index);
	void	PASCAL EXPORT PMDSetBufferWriteIndex(UINT16 module, UINT16 bufferID, UINT32 index);
	void	PASCAL EXPORT PMDGetBufferWriteIndex(UINT16 module, UINT16 bufferID, UINT32 *index);
	void	PASCAL EXPORT PMDWriteBuffer(UINT16 module, UINT16 bufferID, INT32 value);
	void	PASCAL EXPORT PMDReadBuffer(UINT16 module, UINT16 bufferID, INT32 *value);
	void	PASCAL EXPORT PMDGetCurrentMotorCommand(UINT16 module, UINT16 axis, double *motor_command);
	void	PASCAL EXPORT PMDSetMotorCommand(UINT16 module, UINT16 axis, double value);
	void	PASCAL EXPORT PMDGetMotorCommand(UINT16 module, UINT16 axis, double *value);
	void	PASCAL EXPORT PMDSetMotorMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetMotorMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDSetOutputMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetOutputMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDSetAcceleration(UINT16 module, UINT16 axis, double acceleration);
	void	PASCAL EXPORT PMDGetAcceleration(UINT16 module, UINT16 axis, double *acceleration);
	void	PASCAL EXPORT PMDGetCommandedAcceleration(UINT16 module, UINT16 axis, double *acceleration);
	void	PASCAL EXPORT PMDGetCommandedPosition(UINT16 module, UINT16 axis, INT32 *position);
	void	PASCAL EXPORT PMDGetCommandedVelocity(UINT16 module, UINT16 axis, double *velocity);
	void	PASCAL EXPORT PMDSetDeceleration(UINT16 module, UINT16 axis, double deceleration);
	void	PASCAL EXPORT PMDGetDeceleration(UINT16 module, UINT16 axis, double *deceleration);
	void	PASCAL EXPORT PMDSetJerk(UINT16 module, UINT16 axis, double jerk);
	void	PASCAL EXPORT PMDGetJerk(UINT16 module, UINT16 axis, double *jerk);
	void	PASCAL EXPORT PMDSetPosition(UINT16 module, UINT16 axis, INT32 position);
	void	PASCAL EXPORT PMDGetPosition(UINT16 module, UINT16 axis, INT32 *position);
	void	PASCAL EXPORT PMDSetProfileMode(UINT16 module, UINT16 axis, UINT16 profile);
	void	PASCAL EXPORT PMDGetProfileMode(UINT16 module, UINT16 axis, UINT16 *profile);
	void	PASCAL EXPORT PMDSetStopMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetStopMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDSetVelocity(UINT16 module, UINT16 axis, double velocity);
	void	PASCAL EXPORT PMDGetVelocity(UINT16 module, UINT16 axis, double *velocity);
	void	PASCAL EXPORT PMDSetAxisMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetAxisMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDSetLimitSwitchMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetLimitSwitchMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDSetMotionCompleteMode(UINT16 module, UINT16 axis, UINT16 mode);
	void	PASCAL EXPORT PMDGetMotionCompleteMode(UINT16 module, UINT16 axis, UINT16 *mode);
	void	PASCAL EXPORT PMDSetSampleTime(UINT16 module, UINT16 time);
	void	PASCAL EXPORT PMDGetSampleTime(UINT16 module, UINT16 *time);
	void	PASCAL EXPORT PMDSetSettleTime(UINT16 module, UINT16 axis, UINT16 time);
	void	PASCAL EXPORT PMDGetSettleTime(UINT16 module, UINT16 axis, UINT16 *time);
	void	PASCAL EXPORT PMDSetSettleWindow(UINT16 module, UINT16 axis, UINT16 window);
	void	PASCAL EXPORT PMDGetSettleWindow(UINT16 module, UINT16 axis, UINT16 *window);
	void	PASCAL EXPORT PMDGetTime(UINT16 module, UINT32 *chipset_time);
	void	PASCAL EXPORT PMDSetTrackingWindow(UINT16 module, UINT16 axis, UINT16 window);
	void	PASCAL EXPORT PMDGetTrackingWindow(UINT16 module, UINT16 axis, UINT16 *window);
	void	PASCAL EXPORT PMDGetActivityStatus(UINT16 module, UINT16 axis, UINT16 *status);
	void	PASCAL EXPORT PMDSetAxisOutSource(UINT16 module, UINT16 axis, UINT16 reg, UINT16 bit, UINT16 sourceaxis);
	void	PASCAL EXPORT PMDGetAxisOutSource(UINT16 module, UINT16 axis, UINT16 *reg, UINT16 *bit, UINT16 *sourceaxis);
	void	PASCAL EXPORT PMDGetEventStatus(UINT16 module, UINT16 axis, UINT16 *event_status);
	void	PASCAL EXPORT PMDGetSignalStatus(UINT16 module, UINT16 axis, UINT16 *signal_status);
	void	PASCAL EXPORT PMDSetSignalSense(UINT16 module, UINT16 axis, UINT16 mask);
	void	PASCAL EXPORT PMDGetSignalSense(UINT16 module, UINT16 axis, UINT16 *mask);
	void	PASCAL EXPORT PMDResetEventStatus(UINT16 module, UINT16 axis, UINT16 mask);
	void	PASCAL EXPORT PMDGetTraceCount(UINT16 module, UINT32 *trace_count);
	void	PASCAL EXPORT PMDSetTraceMode(UINT16 module, UINT16 mode);
	void	PASCAL EXPORT PMDGetTraceMode(UINT16 module, UINT16 *mode);
	void	PASCAL EXPORT PMDSetTracePeriod(UINT16 module, UINT16 period);
	void	PASCAL EXPORT PMDGetTracePeriod(UINT16 module, UINT16 *period);
	void	PASCAL EXPORT PMDSetTraceStart(UINT16 module, UINT16 TriggerState, UINT16 TriggerBit, UINT16 condition, UINT16 TriggerAxis);
	void	PASCAL EXPORT PMDGetTraceStart(UINT16 module, UINT16 *TriggerState, UINT16 *TriggerBit, UINT16 *condition, UINT16 *TriggerAxis);
	void	PASCAL EXPORT PMDGetTraceStatus(UINT16 module, UINT16 *mask);
	void	PASCAL EXPORT PMDSetTraceStop(UINT16 module, UINT16 TriggerState, UINT16 TriggerBit,  UINT16 condition, UINT16 TriggerAxis);
	void	PASCAL EXPORT PMDGetTraceStop(UINT16 module, UINT16 *TriggerState, UINT16 *TriggerBit, UINT16 *condition, UINT16 *TriggerAxis);
	void	PASCAL EXPORT PMDSetTraceVariable(UINT16 module, UINT16 variable_number, UINT16 variable, UINT16 axis);
	void	PASCAL EXPORT PMDGetTraceVariable(UINT16 module, UINT16 variable_number, UINT16 *variable, UINT16 *axis);
	void	PASCAL EXPORT PMDGetChecksum(UINT16 module, UINT32 *checksum);
	void	PASCAL EXPORT PMDGetVersion(UINT16 module, UINT32 *version_data);
	void	PASCAL EXPORT PMDNoOperation(UINT16 module);
	void	PASCAL EXPORT PMDReset(UINT16 module);
	void	PASCAL EXPORT PMDReadIO(UINT16 module, UINT16 axis, UINT16 address, UINT16 *data);
	void	PASCAL EXPORT PMDWriteIO(UINT16 module, UINT16 axis, UINT16 address, UINT16 data);
	void	PASCAL EXPORT PMDReadAnalog(UINT16 module, UINT16 portID, UINT16 *data);

	void	PASCAL EXPORT PMDSetGearMaster(UINT16 module, UINT16 axis, UINT16 source, UINT16 masteraxis);
	void	PASCAL EXPORT PMDGetGearMaster(UINT16 module, UINT16 axis, UINT16 *source, UINT16 *masteraxis);
	void	PASCAL EXPORT PMDSetGearRatio(UINT16 module, UINT16 slaveaxis, double ratio);
	void	PASCAL EXPORT PMDGetGearRatio(UINT16 module, UINT16 slaveaxis, double *ratio); 

	INT16	PASCAL EXPORT PMDGetErrorCode(UINT16 module);
	char *  PASCAL EXPORT PMDGetErrorMsg(INT16 ErrorCode);
	
	INT16	PASCAL EXPORT PMDSetOutput(UINT16 module, UINT16 axis, UINT16 data);
	INT16	PASCAL EXPORT PMDGetOutput(UINT16 module, UINT16 axis, UINT16 *data);
	INT16	PASCAL EXPORT PMDGetInput(UINT16 module, UINT16 axis, UINT16 *data);

	INT16	PASCAL EXPORT PMDSetEmergency(UINT16 module, UINT16 data);
	INT16	PASCAL EXPORT PMDGetEmergency(UINT16 module, UINT16 *data);

	INT16	PASCAL EXPORT PMDSetMoveunitPerpulse(UINT16 module, UINT16 axis, double unitperpulse);
	INT16	PASCAL EXPORT PMDGetMoveunitPerpulse(UINT16 module, UINT16 axis, double *unitperpulse);

	INT16	PASCAL EXPORT PMDSetFilter(UINT16 module, UINT16 axis , UINT16* coefficients);
	INT16	PASCAL EXPORT PMDGetFilter(UINT16 module, UINT16 axis, UINT16* coefficients);
	INT16	PASCAL EXPORT PMDGetFilterError(UINT16 module, UINT16 axis, INT32* positionerror, INT16* derivativeerror, INT32* integralerror);

	BOOL	PASCAL EXPORT PMDInMotion(UINT16 module, UINT16 axis);

	double	PASCAL EXPORT PMDGetLogicPosition(UINT16 module, UINT16 axis);
	INT16	PASCAL EXPORT PMDSetActualPosition_(UINT16 module, UINT16 axis, double position);
	INT16	PASCAL EXPORT PMDGetActualPosition_(UINT16 module, UINT16 axis, double *position);

	INT16	PASCAL EXPORT PMDStartRMove(UINT16 module, UINT16 axis, double distance, double velocity, double acceleration);
	INT16	PASCAL EXPORT PMDStartTMove(UINT16 module,UINT16 axis, double distance, double velocity, double acceleration,  double deceleration);
	INT16	PASCAL EXPORT PMDWaitForDone(UINT16 module, UINT16 axis);
	INT16	PASCAL EXPORT PMDStartRsMove(UINT16 module,UINT16 axis, double distance, double velocity, double acceleration, double jerk);
	INT16	PASCAL EXPORT PMDStartRMoveAll(UINT16 module,UINT16* axis, double* distance, double* velocity, double* accel);
	INT16	PASCAL EXPORT PMDWaitForAll(UINT16 module, UINT16* axis);
	INT16	PASCAL EXPORT PMDVmove(UINT16 module, UINT16 axis, double  velocity, double acceleration, double deceleration);

	INT16	PASCAL EXPORT PMDGearLink(UINT16 module, UINT16 slaveaxis, UINT16 masteraxis, UINT16 source, double ratio);
	INT16	PASCAL EXPORT PMDSetDistance(UINT16 module, UINT16 axis, double distance);	

	INT16	PASCAL EXPORT PMDStartMove(UINT16 module, UINT16 axis, double distance, double velocity, double acceleration);
	INT16	PASCAL EXPORT PMDStartSAMove(UINT16 module, UINT16 axis, double distance, double velocity, double acceleration, double jerk);
	INT16	PASCAL EXPORT PMDStartTAMove(UINT16 module, UINT16 axis, double distance, double velocity, double acceleration, double deceleration);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif	// __AXT_PMD_H__


/************************************************************************/
/* SMC-2V53 라이브러리 함수 ver 1.0                                     */
/* 최종 수정일 : 2002-07-23                                             */
/************************************************************************/
