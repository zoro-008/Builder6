#ifndef __AXT_MCX312_H__
#define __AXT_MCX312_H__

#include "AxtLIBDef.h"
#include "MCX31XDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTMCX312 Library - MCX312 Motion module
	적용제품
		SMC-2V54 - MCX312 2축
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// 초기화 관련 함수
	// MCX-312(SMC-2V54)모듈의 초기화 - reset=1(ON)이면 각 레지스터를 초기화한다
	BOOL   PASCAL EXPORT InitializeMcx312(BOOL reset);
	// MCX-312(SMC-2V54)모듈이 초기화가 되었는가 ?
	BOOL   PASCAL EXPORT M312IsInitialized(void);
	// MCX-312(SMC-2V54)모듈의 사용을 종료한다
	void   PASCAL EXPORT M312StopService(void);

	/// 인터럽트 관련 함수
	// Window message & procedure
	void   PASCAL EXPORT M312SetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_MCX312_INTERRUPT_PROC proc);	
		/*	hWnd	: 윈도우 핸들, 사용하지 않으면 NULL
			wMsg	: 윈도우의 메세지, hWnd를 NULL로 지정했으면 의미 없음.
					  디폴트값을 사용한다면 0을 지정
			proc	: 함수 포인터를 사용한다면 함수의 포인터를 지정
					  함수 원형(Prototype)을 알고싶다면 AxtLIB.h 파일을 참조	*/
	void   PASCAL EXPORT M312WriteInterruptMask(INT16 nModuleNo, UINT16 wMask);
	UINT16 PASCAL EXPORT M312ReadInterruptMask( INT16 nModuleNo);
	void   PASCAL EXPORT M312conti_interrupt_enable(INT16 nModuleNo);
	void   PASCAL EXPORT M312conti_interrupt_disable(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M312is_conti_interrupt_enabled(INT16 nModuleNo);

	//CSG 수정
	BOOL   PASCAL EXPORT M312load_parameter(INT16 nModuleNo, INT16 axis, char *nfilename);
	BOOL   PASCAL EXPORT M312save_parameter(INT16 nModuleNo, INT16 axis, char *nfilename); 
	BOOL   PASCAL EXPORT M312load_parameter_all(char *nfilename);
	BOOL   PASCAL EXPORT M312save_parameter_all(char *nfilename); 

	/// 보드 및 모듈 정보
	// 지정한 위치에 있는 MCX-312모듈의 순서(0부터 시작)를 리턴 - 장착되어 있지않으면 -1
	//테스트 ( INT16 nModulePos -> INT16 nModuleNo)
	INT16  PASCAL EXPORT M312get_module_number(INT16 nBoardNo, INT16 nModuleNo);
	// 지정한 주소(어드레스)에 있는 MCX-312모듈의 순서(0부터 시작)를 리턴 - 장착되어 있지않으면 -1
	INT16  PASCAL EXPORT M312get_boardno(UINT32 address);
	// 베이스보드의 갯수를 리턴
	INT16  PASCAL EXPORT M312get_board_count();
	// MCX-312가 장차된 모듈의 갯수를 리턴
	INT16  PASCAL EXPORT M312get_module_count();

	// MCX-312모듈을 리세트
	void   PASCAL EXPORT M312reset(INT16 nModuleNo);

	INT16  PASCAL EXPORT M312get_error_code();
	char * PASCAL EXPORT M312get_error_msg( INT16 error_code);

	// Start/Stop speed 설정 및 검색
	BOOL   PASCAL EXPORT M312set_startstop_speed(INT16 nModuleNo, INT16 axis, double velocity);
	double PASCAL EXPORT M312get_startstop_speed(INT16 nModuleNo, INT16 axis);

	// 이동단위(Move unit)의 설정 및 검색 - 1.0이면 펄스단위를 사용한다
	BOOL   PASCAL EXPORT M312set_moveunit_perpulse(INT16 nModuleNo, INT16 axis, double unitperpulse);
	double PASCAL EXPORT M312get_moveunit_perpulse(INT16 nModuleNo, INT16 axis);

	// 지정한 축(axis)이 구동중인가? - 구동중이면 1(TRUE)를 리턴
	BOOL   PASCAL EXPORT M312in_motion(INT16 nModuleNo, INT16 axis);

	// 지정한 모듈이 보간 구동중인가? - 구동중이면 1(TRUE)를 리턴
	BOOL   PASCAL EXPORT M312in_interpolation(INT16 nModuleNo);

	// 
	BOOL   PASCAL EXPORT M312full_stack_counter(INT16 nModuleNo);

	// 
	BOOL   PASCAL EXPORT M312contiip_write_possible(INT16 nModuleNo);

	// 최대(Max) 속도를 설정/검색 - 모든 구동은 최대속도로 제한된다
	BOOL   PASCAL EXPORT M312set_max_speed(INT16 nModuleNo, INT16 axis, double max_velocity);
	double PASCAL EXPORT M312get_max_speed(INT16 nModuleNo, INT16 axis);

	// Command(Internal counter) 위치를 설정/검색
	BOOL   PASCAL EXPORT M312set_command_position(INT16 nModuleNo, INT16 axis, double position);
	double PASCAL EXPORT M312get_command_position(INT16 nModuleNo, INT16 axis);

	// Actual(External counter) 위치를 설정/검색
	BOOL   PASCAL EXPORT M312set_actual_position(INT16 nModuleNo, INT16 axis, double position);
	double PASCAL EXPORT M312get_actual_position(INT16 nModuleNo, INT16 axis);

	// 
	BOOL   PASCAL EXPORT M312set_comparate_positionp(INT16 nModuleNo,  INT16 axis, double position);
	double PASCAL EXPORT M312get_comparate_positionp(INT16 nModuleNo,  INT16 axis);

	// 
	BOOL   PASCAL EXPORT M312set_comparate_positionm(INT16 nModuleNo,  INT16 axis, double position);
	double PASCAL EXPORT M312get_comparate_positionm(INT16 nModuleNo,  INT16 axis);

	// 펄스의 출력모드를 설정 - 1/2펄스 모드등...
	BOOL   PASCAL EXPORT M312set_pulse_out_method(INT16 nModuleNo, INT16 axis, UINT8 method);
	UINT8  PASCAL EXPORT M312get_pulse_out_method(INT16 nModuleNo, INT16 axis);
		/* method	:
				MCX312_PULSEOUT_POSITIVE_LEVEL | MCX312_PULSEOUT_2_PULSE								=> 0x00
				MCX312_PULSEOUT_DIR_LOW_P  | MCX312_PULSEOUT_POSITIVE_LEVEL | MCX312_PULSEOUT_1_PULSE	=> 0x01
				MCX312_PULSEOUT_NEGATIVE_LEVEL | MCX312_PULSEOUT_2_PULSE								=> 0x02
				MCX312_PULSEOUT_DIR_LOW_P  | MCX312_PULSEOUT_NEGATIVE_LEVEL | MCX312_PULSEOUT_1_PULSE	=> 0x03
				MCX312_PULSEOUT_DIR_HIGH_P | MCX312_PULSEOUT_POSITIVE_LEVEL | MCX312_PULSEOUT_1_PULSE	=> 0x05
				MCX312_PULSEOUT_DIR_HIGH_P | MCX312_PULSEOUT_NEGATIVE_LEVEL | MCX312_PULSEOUT_1_PULSE	=> 0x07
		*/

	// 엔코더의 입력모드를 설정 - 1/2/4체배 등...
	BOOL   PASCAL EXPORT M312set_enc_input_method(INT16 nModuleNo, INT16 axis, UINT8 method);
	UINT8  PASCAL EXPORT M312get_enc_input_method(INT16 nModuleNo, INT16 axis);
		/* method	:
		*/

	// 현재 구동속도를 리턴
	double PASCAL EXPORT M312get_velocity(INT16 nModuleNo, INT16 axis);

	// 구동하려고하는 위치를 오버라이드(Override) 한다
	BOOL   PASCAL EXPORT M312position_r_override(INT16 nModuleNo, INT16 axis, double distance);
	double PASCAL EXPORT M312get_override_position(INT16 nModuleNo, INT16 axis);

	// 속도를 오버라이드(Override) 한다
	BOOL   PASCAL EXPORT M312set_override_speed(INT16 nModuleNo, INT16 axis, double velocity);
	double PASCAL EXPORT M312get_override_speed(INT16 nModuleNo, INT16 axis);

	/// (+)limit
	// 센서의 사용유무를 설정/검색 - action=1(ON)이면 사용함
	BOOL   PASCAL EXPORT M312set_pend_limit_stop_method(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M312get_pend_limit_stop_method(INT16 nModuleNo, INT16 axis);
	// 센서의 active 레벨을 설정/검색 - level=1(HIGH)이면 Active High
	BOOL   PASCAL EXPORT M312set_pend_limit_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M312get_pend_limit_level(INT16 nModuleNo, INT16 axis);
	// 센서의 값을 읽음 - Acitve Low로 설정시 0(LOW)이면 1(HIGH)가 리턴됨
	UINT8  PASCAL EXPORT M312get_pend_limit_switch(INT16 nModuleNo, INT16 axis);

	/// (-)limit
	// 센서의 사용유무를 설정/검색 - action=1(ON)이면 사용함
	BOOL   PASCAL EXPORT M312set_nend_limit_stop_method(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M312get_nend_limit_stop_method(INT16 nModuleNo, INT16 axis);
	// 센서의 active 레벨을 설정/검색 - level=1(HIGH)이면 Active High
	BOOL   PASCAL EXPORT M312set_nend_limit_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M312get_nend_limit_level(INT16 nModuleNo, INT16 axis);
	// 센서의 값을 읽음 - Acitve Low로 설정시 0(LOW)이면 1(HIGH)가 리턴됨
	UINT8  PASCAL EXPORT M312get_nend_limit_switch(INT16 nModuleNo, INT16 axis);

	/// ORG(원점) 센서
	// 센서의 사용유무를 설정/검색 - action=1(ON)이면 사용함
	BOOL   PASCAL EXPORT M312set_origin_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M312get_origin_enable(INT16 nModuleNo, INT16 axis);
	// 센서의 active 레벨을 설정/검색 - level=1(HIGH)이면 Active High
	BOOL   PASCAL EXPORT M312set_origin_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M312get_origin_level(INT16 nModuleNo, INT16 axis);
	// 센서의 값을 읽음 - Acitve Low로 설정시 0(LOW)이면 1(HIGH)가 리턴됨
	UINT8  PASCAL EXPORT M312get_origin_switch(INT16 nModuleNo, INT16 axis);
		/*
			ORG(원점) 센서를 사용(Enable/Disable)할 것인지를 설정하고, 센서 작동 레벨(HIGH/LOW)
			그리고 현재 센서의 상태를 읽는다
			action : ON(1) = 원점센서를 사용함
					 OFF(0)= 원점센서를 사용하지 않음
			level  : HIGH(1) = Active High
					 LOW(0)  = Active Low
			M312v_move나 M312const_move를 사용할 때 원점센서를 사용한다고 설정하면 원점센서가
			active될때 구동을 정지함, 사용하지 않으면 원점센서가 active되어도 구동이 정지되지 않음
		*/

	/// Encoder의 Z상
	// 센서의 사용유무를 설정/검색 - action=1(ON)이면 사용함
	BOOL   PASCAL EXPORT M312set_z_phase_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M312get_z_phase_enable(INT16 nModuleNo, INT16 axis);
	// 센서의 active 레벨을 설정/검색 - level=1(HIGH)이면 Active High
	BOOL   PASCAL EXPORT M312set_z_phase_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M312get_z_phase_level(INT16 nModuleNo, INT16 axis);
	// 센서의 값을 읽음 - Acitve Low로 설정시 0(LOW)이면 1(HIGH)가 리턴됨
	UINT8  PASCAL EXPORT M312get_z_phase_switch(INT16 nModuleNo, INT16 axis);
		/*
			엔코더의 Z상을 사용(Enable/Disable)할 것인지를 설정하고, 작동 레벨(HIGH/LOW)
			그리고 현재 Z상의 상태를 읽는다
			action : ON(1) = Z상을 사용함
					 OFF(0)= Z상을 사용하지 않음
			level  : HIGH(1) = Active High
					 LOW(0)  = Active Low
			M312v_move나 M312const_move를 사용할 때 Z상을 사용한다고 설정하면 Z상이
			active될때 구동을 정지함, 사용하지 않으면 Z상이 active되어도 구동이 정지되지 않음
		*/

	UINT8  PASCAL EXPORT M312get_emergency_switch(INT16 nModuleNo);

	/// SERVO-ON
	// Servo On/Off - value=1(ON)이면 Servo ON
	BOOL   PASCAL EXPORT M312set_servo_enable(INT16 nModuleNo, INT16 axis, UINT8 value);
	UINT8  PASCAL EXPORT M312get_servo_enable(INT16 nModuleNo, INT16 axis);

	/// InPosition 신호 - 드라이버의 인포지션 신호를 사용 시, 모션 출력 완료 후 신호가 ON이 되어야 모션의 구동을 종료한다.
	// 신호의 사용유무를 설정/검색 - action=1(ON)이면 사용함
	BOOL   PASCAL EXPORT M312set_inposition_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M312get_inposition_enable(INT16 nModuleNo, INT16 axis);
	// 신호의 active 레벨을 설정/검색 - level=1(HIGH)이면 Active High
	BOOL   PASCAL EXPORT M312set_inposition_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M312get_inposition_level(INT16 nModuleNo, INT16 axis);
	// 신호의 값을 읽음 - Acitve Low로 설정시 0(LOW)이면 1(HIGH)가 리턴됨
	UINT8  PASCAL EXPORT M312get_inposition_switch(INT16 nModuleNo, INT16 axis);

	/// Alarm 신호 - 드라이버의 알람 신호를 사용 시, 알람이 발생하면 펄스출력을 중지한다
	// 신호의 사용유무를 설정/검색 - action=1(ON)이면 사용함
	BOOL   PASCAL EXPORT M312set_alarm_enable(INT16 nModuleNo, INT16 axis, UINT8 action);
	UINT8  PASCAL EXPORT M312get_alarm_enable(INT16 nModuleNo, INT16 axis);
	// 신호의 active 레벨을 설정/검색 - level=1(HIGH)이면 Active High
	BOOL   PASCAL EXPORT M312set_alarm_level(INT16 nModuleNo, INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT M312get_alarm_level(INT16 nModuleNo, INT16 axis);
	// 신호의 값을 읽음 - Acitve Low로 설정시 0(LOW)이면 1(HIGH)가 리턴됨
	UINT8  PASCAL EXPORT M312get_alarm_switch(INT16 nModuleNo, INT16 axis);

	/// (+)Software limit
	BOOL   PASCAL EXPORT M312set_positive_swlimit(INT16 nModuleNo, INT16 axis, UINT8 action, double limitposition);
	double PASCAL EXPORT M312get_positive_swlimit(INT16 nModuleNo, INT16 axis);

	/// (-)Software limit
	BOOL   PASCAL EXPORT M312set_negative_swlimit(INT16 nModuleNo, INT16 axis, UINT8 action, double limitposition);
	double PASCAL EXPORT M312get_negative_swlimit(INT16 nModuleNo, INT16 axis);

	UINT8  PASCAL EXPORT M312get_end_status(INT16 nModuleNo, INT16 axis);
	UINT8  PASCAL EXPORT M312get_mechanical_signal(INT16 nModuleNo, INT16 axis);

	/// 범용 출력 포트
	BOOL   PASCAL EXPORT M312set_output(INT16 nModuleNo, INT16 axis, UINT8 value);
	UINT8  PASCAL EXPORT M312get_output(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M312output_bit_on(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M312set_output_bit(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M312reset_output_bit(INT16 nModuleNo, INT16 axis, UINT8 bitNo);
	BOOL   PASCAL EXPORT M312change_output_bit(INT16 nModuleNo, INT16 axis, UINT8 bitNo, UINT8 state);

	/// 범용 입력 포트
	UINT8  PASCAL EXPORT M312get_input(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M312input_bit_on(INT16 nModuleNo, INT16 axis, UINT8 bitNo);

	/// 모션 구동 함수군...
	BOOL   PASCAL EXPORT M312start_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration);
	BOOL   PASCAL EXPORT M312start_r_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration);
	BOOL   PASCAL EXPORT M312start_t_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	BOOL   PASCAL EXPORT M312start_rt_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	BOOL   PASCAL EXPORT M312start_s_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double jerk);
	BOOL   PASCAL EXPORT M312start_rs_move(INT16 nModuleNo, INT16 axis, double distance, double velocity, double acceleration, double jerk);
	BOOL   PASCAL EXPORT M312wait_for_done(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M312start_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration);
	BOOL   PASCAL EXPORT M312start_r_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration);
	BOOL   PASCAL EXPORT M312start_s_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration, double *jerk);
	BOOL   PASCAL EXPORT M312start_rs_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *distance, double *velocity, double *acceleration, double *jerk);
	BOOL   PASCAL EXPORT M312v_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *velocity, double *acceleration);
	BOOL   PASCAL EXPORT M312v_s_move_all(INT16 nModuleNo, INT16 len, INT16 *axis, double *velocity, double *acceleration, double *jerk);
	BOOL   PASCAL EXPORT M312wait_for_all(INT16 nModuleNo, INT16 len, INT16 *axis );

	/// 모션 구동을 중지하는 함수...
	BOOL   PASCAL EXPORT M312set_stop(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M312set_stop_all(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M312set_e_stop(INT16 nModuleNo, INT16 axis);
	BOOL   PASCAL EXPORT M312set_e_stop_all(INT16 nModuleNo);

	BOOL   PASCAL EXPORT M312v_move(INT16 nModuleNo, INT16 axis, double velocity, double acceleration);
	BOOL   PASCAL EXPORT M312v_s_move(INT16 nModuleNo, INT16 axis, double velocity, double acceleration, double jerk);
	BOOL   PASCAL EXPORT M312const_move(INT16 nModuleNo, INT16 axis, double velocity);

	BOOL   PASCAL EXPORT M312set_move_speed(INT16 nModuleNo, double speeddata);
	double PASCAL EXPORT M312get_move_speed(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M312set_move_accel(INT16 nModuleNo, double accel);
	double PASCAL EXPORT M312get_move_accel(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M312set_move_jerk(INT16 nModuleNo, double jerk);
	double PASCAL EXPORT M312get_move_jerk(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M312set_move_profile_type(INT16 nModuleNo, INT16 type);
	BOOL   PASCAL EXPORT M312get_move_profile_type(INT16 nModuleNo);
	BOOL   PASCAL EXPORT M312move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M312r_move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M312contiend_r_move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M312cw_arc(INT16 nModuleNo, double x_final, double y_final, double x_center, double y_center);
	BOOL   PASCAL EXPORT M312ccw_arc(INT16 nModuleNo, double x_final, double y_final, double x_center, double y_center);
	BOOL   PASCAL EXPORT M312cw_arc_1(INT16 nModuleNo, double x_final, double y_final, double Radius);
	BOOL   PASCAL EXPORT M312ccw_arc_1(INT16 nModuleNo, double x_final, double y_final, double Radius);
	BOOL   PASCAL EXPORT M312cw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M312ccw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M312cw_r_arc(INT16 nModuleNo, double x_final, double y_final, double x_center, double y_center);
	BOOL   PASCAL EXPORT M312ccw_r_arc(INT16 nModuleNo, double x_final, double y_final, double x_center, double y_center);
	BOOL   PASCAL EXPORT M312cw_r_arc_1(INT16 nModuleNo, double x_final, double y_final, double Radius);
	BOOL   PASCAL EXPORT M312ccw_r_arc_1(INT16 nModuleNo, double x_final, double y_final, double Radius);
	BOOL   PASCAL EXPORT M312conti_r_move_2(INT16 nModuleNo, double x, double y);
	BOOL   PASCAL EXPORT M312conti_cw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M312conti_ccw_arc_2(INT16 nModuleNo, double x_center, double y_center, double angle);
	BOOL   PASCAL EXPORT M312conti_cw_r_arc(INT16 nModuleNo, double x_final, double y_final, double x_center, double y_center);
	BOOL   PASCAL EXPORT M312conti_ccw_r_arc(INT16 nModuleNo, double x_final, double y_final, double x_center, double y_center);
	BOOL   PASCAL EXPORT M312conti_cw_r_arc_1(INT16 nModuleNo, double x_final, double y_final, double Radius);
	BOOL   PASCAL EXPORT M312conti_ccw_r_arc_1(INT16 nModuleNo, double x_final, double y_final, double Radius);

	void PASCAL EXPORT M312conti_set_auto_decel(INT16 nModuleNo, BOOL bAuto);
	BOOL PASCAL EXPORT M312conti_get_auto_decel(INT16 nModuleNo);
	void PASCAL EXPORT M312deceldisable(INT16 nModuleNo);
	void PASCAL EXPORT M312decelenable(INT16 nModuleNo);

	BOOL PASCAL EXPORT M312set_mpg_drive(INT16 module_id, INT16 axis, UINT8 mode, double dDistance, double dSpeed);

/* <Argument>
	module_id	: SMC-2V54 모듈의 순서, 0부터 시작하면 베이스보드의 순서와 모듈의 위치에
				  따라 순차적으로 할당됨
	axis		: 모듈내의 축의 순서, 0과 1만 사용 가능
	action		: 사용 유/무를 설정할때 사용하며, 0(OFF)=Disable, 1(ON)=Enable
	level		: Active level을 설정할때 사용하며, 0(LOW)=Active Low, 1(HIGH)=Active High
	nBoardNo	: 베이스보드의 순서, 0부터 시작하며, ISA/VME의 경우에는 하위어드레스부터
				  자동 계산되며, PCI/cPCI의 경우에는 바이오스(Bios)에서 할당된
				  순서부터(EzConfig의 순서) 자동 할당된다
	nModulePos	: 베이스보드내의 모듈의 순서, 0부터 1(Half/3U size) 또는
				  0부터 3(Full/6U size)
	velocity	:
	distance	:
	acceleration:
	deceleration:
	jerk		: 
*/

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_MCX312_H__
