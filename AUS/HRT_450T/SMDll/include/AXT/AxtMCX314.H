#ifndef __AXT_MCX314_H__
#define __AXT_MCX314_H__

#include "AxtLIBDef.h"
#include "MCX31XDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTMCX314 Library - MCX314 Motion module
	적용제품
		SMC-2V51 - MCX314 4축, 2U size
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// 
	BOOL   PASCAL EXPORT InitializeMcx314(BOOL reset);
	BOOL   PASCAL EXPORT M314IsInitialized(void);
	void   PASCAL EXPORT M314StopService();

	// Window message & procedure
	void   PASCAL EXPORT M314SetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_MCX314_INTERRUPT_PROC proc);	
			// <*> 2002.03.07 HWND => HANDLE
	//CSG 수정
	BOOL   PASCAL EXPORT M314load_parameter(INT16 nModuleNo, INT16 axis, char *nfilename);
	BOOL   PASCAL EXPORT M314save_parameter(INT16 nModuleNo, INT16 axis, char *nfilename); 
	BOOL   PASCAL EXPORT M314load_parameter_all(char *nfilename);
	BOOL   PASCAL EXPORT M314save_parameter_all(char *nfilename); 

	INT16  PASCAL EXPORT M314get_module_number(INT16 nBoardNo, INT16 nModuleNo);
	INT16  PASCAL EXPORT M314get_boardno(UINT32 address);
	INT16  PASCAL EXPORT M314get_board_count();
	INT16  PASCAL EXPORT M314get_module_count();
	
	void   PASCAL EXPORT M314reset(INT16 nModuleNo);
	INT16  PASCAL EXPORT M314get_error_code();
	char  *PASCAL EXPORT M314get_error_msg( INT16 error_code);
	double PASCAL EXPORT M314get_startstop_speed(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_startstop_speed(INT16 nModuleNo, INT16 axis, double velocity);
	double PASCAL EXPORT M314get_moveunit_perpulse(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_moveunit_perpulse(INT16 nModuleNo, INT16 axis, double unitperpulse);
	BOOL   PASCAL EXPORT M314in_motion(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314in_interpolation(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314full_stack_counter(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314contiip_write_possible(INT16 nModuleNo);
	double PASCAL EXPORT M314get_max_speed(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_max_speed(INT16 nModuleNo, INT16 axis, double max_velocity);
	BOOL   PASCAL EXPORT M314set_command_position(INT16 nModuleNo, INT16 axis, double position);
	double PASCAL EXPORT M314get_command_position(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_actual_position(INT16 nModuleNo, INT16 axis, double position);
	double PASCAL EXPORT M314get_actual_position(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_comparate_positionp(INT16 nModuleNo,  INT16 axis, double position);
	double PASCAL EXPORT M314get_comparate_positionp(INT16 nModuleNo,  INT16 axis);
	BOOL   PASCAL EXPORT M314set_comparate_positionm(INT16 nModuleNo,  INT16 axis, double position);
	double PASCAL EXPORT M314get_comparate_positionm(INT16 nModuleNo,  INT16 axis);
	BOOL   PASCAL EXPORT M314set_pulse_out_method(INT16 nModuleNo, INT16 axis, UINT8 method);
	UINT8  PASCAL EXPORT M314get_pulse_out_method(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_enc_input_method(INT16 nModuleNo, INT16 axis, UINT8 method);
	UINT8  PASCAL EXPORT M314get_enc_input_method(INT16 nModuleNo, INT16 axis);
	double PASCAL EXPORT M314get_velocity(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314position_r_override(INT16 nModuleNo, INT16 axis, double distance);
	double PASCAL EXPORT M314get_override_position(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_override_speed(INT16 nModuleNo, INT16 axis, double velocity);
	double PASCAL EXPORT M314get_override_speed(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_inposition_switch(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_alarm_switch(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_pend_limit_switch(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_nend_limit_switch(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_origin_switch(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_z_phase_switch(INT16 nModuleNo, INT16 axis);
	// 테스트 (INT16 axis 삽입)
	UINT8  PASCAL EXPORT M314get_emergency_switch(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_servo_enable(INT16 nModuleNo, INT16 axis, UINT8 value);
	UINT8  PASCAL EXPORT M314get_servo_enable(INT16 nModuleNo, INT16 axis);
//	INT16  PASCAL EXPORT M314AlarmClearSwitch(INT16 nModuleNo, INT16 axis, UINT8 value);
//	INT16  PASCAL EXPORT M314GetAlarmClearSwitch(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_inposition_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	BOOL   PASCAL EXPORT M314set_alarm_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	BOOL   PASCAL EXPORT M314set_pend_limit_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	BOOL   PASCAL EXPORT M314set_nend_limit_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	BOOL   PASCAL EXPORT M314set_origin_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	BOOL   PASCAL EXPORT M314set_z_phase_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M314get_inposition_level(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_alarm_level(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_pend_limit_level(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_nend_limit_level(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_origin_level(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_z_phase_level(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_inposition_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	BOOL   PASCAL EXPORT M314set_alarm_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	BOOL   PASCAL EXPORT M314set_pend_limit_stop_method(INT16 nModuleNo, INT16 axis, UINT8 action);
	BOOL   PASCAL EXPORT M314set_nend_limit_stop_method(INT16 nModuleNo, INT16 axis, UINT8 action);
	BOOL   PASCAL EXPORT M314set_origin_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	BOOL   PASCAL EXPORT M314set_z_phase_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M314get_inposition_enable(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_alarm_enable(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_pend_limit_stop_method(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_nend_limit_stop_method(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_origin_enable(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_z_pahase_enable(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_positive_swlimit(INT16 nModuleNo, INT16 axis, UINT8 action, double limitposition);
	BOOL   PASCAL EXPORT M314set_negative_swlimit(INT16 nModuleNo, INT16 axis, UINT8 action, double limitposition);
	double PASCAL EXPORT M314get_positive_swlimit(INT16 nModuleNo, INT16 axis);
	double PASCAL EXPORT M314get_negative_swlimit(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_end_status(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_mechanical_signal(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_output(INT16 nModuleNo, INT16 axis, UINT8 value);
	UINT8  PASCAL EXPORT M314get_output(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M314get_input(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314output_bit_on(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M314input_bit_on(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M314set_output_bit(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M314reset_output_bit(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M314change_output_bit(INT16 nModuleNo, INT16 axis, UINT8 bitNo, UINT8 state);
	BOOL   PASCAL EXPORT M314start_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration);
	BOOL   PASCAL EXPORT M314start_r_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration);
	BOOL   PASCAL EXPORT M314start_t_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	BOOL   PASCAL EXPORT M314start_rt_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	BOOL   PASCAL EXPORT M314start_s_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double jerk);
	BOOL   PASCAL EXPORT M314start_rs_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double jerk);
	BOOL   PASCAL EXPORT M314wait_for_done(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314start_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration);
	BOOL   PASCAL EXPORT M314start_r_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration);
	BOOL   PASCAL EXPORT M314start_s_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration, double *jerk);
	BOOL   PASCAL EXPORT M314start_rs_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration, double *jerk);
	BOOL   PASCAL EXPORT M314v_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *velocity, double *acceleration);
	BOOL   PASCAL EXPORT M314v_s_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *velocity, double *acceleration, double *jerk);
	BOOL   PASCAL EXPORT M314wait_for_all(INT16 nModuleNo, INT16 len, INT16 *axis );
	BOOL   PASCAL EXPORT M314set_e_stop(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_stop(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M314set_stop_all(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314set_e_stop_all(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314v_move(INT16 nModuleNo, INT16 axis, double velocity, double acceleration);
	BOOL   PASCAL EXPORT M314v_s_move(INT16 nModuleNo, INT16 axis, double velocity, double acceleration, double jerk);
	BOOL   PASCAL EXPORT M314const_move(INT16 nModuleNo, INT16 axis, double velocity);
	BOOL   PASCAL EXPORT M314map_axis(INT16 nModuleNo, INT16 n_axes, INT16 *map_array);
	BOOL   PASCAL EXPORT M314set_move_speed(INT16 nModuleNo, double speeddata);
	double PASCAL EXPORT M314get_move_speed(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314set_move_accel(INT16 nModuleNo, double accel);
	double PASCAL EXPORT M314get_move_accel(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314set_move_jerk(INT16 nModuleNo, double jerk);
	double PASCAL EXPORT M314get_move_jerk(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314set_move_profile_type(INT16 nModuleNo, INT16 type);
	INT16  PASCAL EXPORT M314get_move_profile_type(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M314move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M314r_move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M314move_3(INT16 nModuleNo, double x, double y, double z);
	BOOL   PASCAL EXPORT M314r_move_3(INT16 nModuleNo, double x, double y, double z);
	BOOL   PASCAL EXPORT M314contiend_r_move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M314contiend_r_move_3(INT16 nModuleNo, double x, double y, double z);
	BOOL   PASCAL EXPORT M314cw_arc(INT16 nModuleNo, double x_final, double y_final,double x_center, double y_center);
	BOOL   PASCAL EXPORT M314ccw_arc(INT16 nModuleNo, double x_final, double y_final,double x_center, double y_center);
	BOOL   PASCAL EXPORT M314cw_arc_1(INT16 nModuleNo, double x_final, double y_final,  double Radius);
	BOOL   PASCAL EXPORT M314ccw_arc_1(INT16 nModuleNo, double x_final, double y_final,  double Radius);
	BOOL   PASCAL EXPORT M314cw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M314ccw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M314cw_r_arc(INT16 nModuleNo, double x_final, double y_final,double x_center, double y_center);
	BOOL   PASCAL EXPORT M314ccw_r_arc(INT16 nModuleNo, double x_final, double y_final,double x_center, double y_center);
	BOOL   PASCAL EXPORT M314cw_r_arc_1(INT16 nModuleNo, double x_final, double y_final,  double Radius);
	BOOL   PASCAL EXPORT M314ccw_r_arc_1(INT16 nModuleNo, double x_final, double y_final,  double Radius);
	BOOL   PASCAL EXPORT M314conti_r_move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M314conti_r_move_3(INT16 nModuleNo, double x, double y, double z);
	BOOL   PASCAL EXPORT M314conti_cw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M314conti_ccw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M314conti_cw_r_arc(INT16 nModuleNo, double x_final, double y_final,double x_center, double y_center);
	BOOL   PASCAL EXPORT M314conti_ccw_r_arc(INT16 nModuleNo, double x_final, double y_final,double x_center, double y_center);
	BOOL   PASCAL EXPORT M314conti_cw_r_arc_1(INT16 nModuleNo, double x_final, double y_final,  double Radius);
	BOOL   PASCAL EXPORT M314conti_ccw_r_arc_1(INT16 nModuleNo, double x_final, double y_final,  double Radius);
	void   PASCAL EXPORT M314WriteInterruptMask(INT16 nModuleNo, UINT32 wMask);
	UINT32 PASCAL EXPORT M314ReadInterruptMask(INT16 nModuleNo);

	void PASCAL EXPORT M314conti_interrupt_enable(INT16 nModuleNo);
	void PASCAL EXPORT M314conti_interrupt_disable(INT16 nModuleNo);
	BOOL PASCAL EXPORT M314is_conti_interrupt_enabled(INT16 nModuleNo);

	void PASCAL EXPORT M314conti_set_auto_decel(INT16 nModuleNo, BOOL bAuto);
	BOOL PASCAL EXPORT M314conti_get_auto_decel(INT16 nModuleNo);
	void PASCAL EXPORT M314deceldisable(INT16 nModuleNo);
	void PASCAL EXPORT M314decelenable(INT16 nModuleNo);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_MCX314_H__