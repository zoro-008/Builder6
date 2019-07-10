#ifndef __AXT_CAMC_IP_H__
#define __AXT_CAMC_IP_H__

#include "AxtLIBDef.h"
#include "CAMCIPDef.h"

/*------------------------------------------------------------------------------------------------*
    AXTCAMCIP Library - CAMC-IP Motion module
    적용제품
        SMC-2V03(ver 2.0 이하) - CAMC-IP 2축
    라이브러리 버전 : v2.0
    마지막 수정일 : 2005. 12. 28.
    기술지원 문의 : Tel. 031-436-2180(아진엑스텍 기술지원팀)
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

    /// 보드 초기화 함수군        -======================================================================================
    // CAMC-IP가 장착된 모듈(SMC-2V03)을 검색하여 초기화한다. 
        //  reset    : 1(TRUE) = 초기화시 레지스터(카운터 등)를 초기화한다
        //  reset(TRUE)일때 초기 설정값.
        //  1) 인터럽트 사용하지 않음.
        //  2) 인포지션 기능 사용하지 않음.
        //  3) 알람정지 기능 사용하지 않음.
        //  4) 급정지 리미트 기능 사용 함.
        //  5) 감속정지 리미트 기능 사용 함.
        //  6) 펄스 출력 모드 : OneLowHighLow(Pulse : Active LOW, Direction : CW{High};CCW{LOW}).
        //  7) 검색 신호 : +급정지 리미트 신호 하강 에지.
        //  8) 입력 인코더 설정 : 2상, 4 체배.
        //  9) 알람, 인포지션, +-감속 정지 리미트, +-급정지 리미트 Active level : HIGH
        // 10) 내부/외부 카운터 : 0.
    BOOL   PASCAL EXPORT InitializeCAMCIP(BOOL reset);
    // CAMC-IP 모듈의 사용이 가능한지를 확인한다
		// 리턴값 :  1(TRUE) = CAMC-IP 모듈을 사용 가능하다.
    BOOL   PASCAL EXPORT CIPIsInitialized(void);
    // CAMC-IP가 장착된 모듈의 사용을 종료한다
    void   PASCAL EXPORT CIPStopService();

    /// 보드 정보 관련 함수군        -===================================================================================
    // 지정한 주소에 장착된 베이스보드의 번호를 리턴한다. 없으면 -1을 리턴한다
    INT16    PASCAL EXPORT CIPget_boardno(UINT32 address);
    // 베이스보드의 갯수를 리턴한다
    INT16    PASCAL EXPORT CIPget_numof_boards();
    // 지정한 베이스보드에 장착된 축의 갯수를 리턴한다
    INT16    PASCAL EXPORT CIPget_numof_axes(INT16 nBoardNo);
    // 축의 갯수를 리턴한다
    INT16    PASCAL EXPORT CIPget_total_numof_axis();
    // 지정한 베이스보드번호와 모듈번호에 해당하는 축번호를 리턴한다
    INT16    PASCAL EXPORT CIPget_axisno(INT16 nBoardNo, INT16 nModuleNo);
    // 지정한 축의 정보를 리턴한다.
		// nBoardNo : 해당 축이 장착된 베이스보드의 번호.
		// nModuleNo: 해당 축이 장착된 모듈의 베이스 모드내 모듈 위치(0~3)
		// bModuleID: 해당 축이 장착된 모듈의 ID : SMC-2V03(0x05)
		// nAxisPos : 해당 축이 장착된 모듈의 첫번째인지 두번째 축인지 정보.(0 : 첫번째, 1 : 두번째)
    BOOL    PASCAL EXPORT CIPget_axis_info(INT16 nAxisNo, INT16 *nBoardNo, INT16 *nModuleNo, UINT8 *bModuleID, INT16 *nAxisPos);

    // 전체 시스템내의 지정한 축의 모듈 번호를 리턴한다.
    INT16    PASCAL EXPORT CIPget_axisno_2_moduleno(INT16 axisno);

    /// 파일 관련 함수군        -========================================================================================
    // 지정 축의 초기값을 지정한 파일에서 읽어서 설정한다
    // Loading parameters.
	//	1) 1Pulse당 이동거리(Move Unit / Pulse)
	//	2) 최대 이동 속도, 시작/정지 속도
	//	3) 엔코더 입력방식, 펄스 출력방식 
	//	4) +급정지 리미트레벨, -급정지 리미트레벨, 급정지 리미트 사용유무
    //  5) +감속정지 리미트레벨,-감속정지 리미트레벨, 감속정지 리미트 사용유무
    //  6) 알람레벨, 알람 사용유무
    //  7) 인포지션(위치결정완료 신호)레벨, 인포지션 사용유무
    //  8) 비상정지 사용유무
    //  9) 내부/외부 카운터 : 0. 	
    BOOL    PASCAL EXPORT CIPload_parameter(INT16 axis, char *nfilename);
    // 지정 축의 초기값을 지정한 파일에 저장한다.
    // Saving parameters.
	//	1) 1Pulse당 이동거리(Move Unit / Pulse)
	//	2) 최대 이동 속도, 시작/정지 속도
	//	3) 엔코더 입력방식, 펄스 출력방식 
	//	4) +급정지 리미트레벨, -급정지 리미트레벨, 급정지 리미트 사용유무
    //  5) +감속정지 리미트레벨,-감속정지 리미트레벨, 감속정지 리미트 사용유무
    //  6) 알람레벨, 알람 사용유무
    //  7) 인포지션(위치결정완료 신호)레벨, 인포지션 사용유무
    //  8) 비상정지 사용유무
    BOOL    PASCAL EXPORT CIPsave_parameter(INT16 axis, char *nfilename); 
    // 모든 축의 초기값을 지정한 파일에서 읽어서 설정한다
    BOOL    PASCAL EXPORT CIPload_parameter_all(char *nfilename);
    // 모든 축의 초기값을 지정한 파일에 저장한다
    BOOL    PASCAL EXPORT CIPsave_parameter_all(char *nfilename); 

    // 인터럽트 함수군   -================================================================================================
    //(인터럽트를 사용하기 위해서는 
    //CIPWriteInterruptMask, CIPKeSetInterruptOutEnableMode, CIPSetWindowMessage등으로 설정 되어야 한다. 예제 소스 참조)
    //Window message & procedure
    //    hWnd    : 윈도우 핸들, 윈도우 메세지를 받을때 사용. 사용하지 않으면 NULL을 입력.
    //    wMsg    : 윈도우 핸들의 메세지, 사용하지 않거나 디폴트값을 사용하려면 0을 입력.
    //    proc    : 인터럽트 발생시 호출될 함수의 포인터, 사용하지 않으면 NULL을 입력.
    void   PASCAL EXPORT CIPSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_CAMCIP_INTERRUPT_PROC proc);    
    //-===============================================================================
    // ReadInterruptFlag에서 설정된 내부 flag변수를 읽어 보는 함수(인터럽트 service routine에서 인터럽터 발생 요인을 판별한다.)
    // bank : 
    // 0x0 : 인터럽트가 발생 하였을때 발생하는 인터럽트 flag register bank1(CAMC-IP 의 INTFLAG1 참조.)
    // 0x1 : 인터럽트가 발생 하였을때 발생하는 인터럽트 flag register bank2(CAMC-IP 의 INTFLAG2 참조.) 
    UINT32  PASCAL EXPORT CIPread_interrupt_flag(INT16 axis, UINT32 bank);
    // 해당 축의 인터럽터 발생 요인을 설정한다. bank내용은 CAMC-IP의 INTMASK1, INTMASK2 내용을 참조.
    void   PASCAL EXPORT CIPKeSetInterruptOutEnableMode(INT16 axis, UINT32 bank1, UINT32 bank2);
    void   PASCAL EXPORT CIPKeResetInterruptOutEnableMode(INT16 axis);
    // 해당 SMC-2V03 보드의 인터럽터 사용 여부를 설정한다. 해당 축의 get_axis_info를 사용하여 입력 값을 찾아 넣는다.
    void   PASCAL EXPORT CIPWriteInterruptMask(INT16 nBoardNo, INT16 nModulePos, UINT8 Mask);
    UINT8  PASCAL EXPORT CIPReadInterruptMask(INT16 nBoardNo, INT16 nModulePos);
		// 해당 모듈의 인터럽트 Flag를 확인한다.
		// 0 bit : 해당 모듈의 0축(x축)의 인터럽트가 발생 여부.
		// 1 bit : 해당 모듈의 1축(y축)의 인터럽트가 발생 여부.
    UINT8  PASCAL EXPORT CIPReadInterruptFlag(INT16 nBoardNo, INT16 nModulePos);

    // 구동 설정 초기화 함수군        -==================================================================================
	// 메인클럭 설정( 모듈에 장착된 Oscillator가 변경될 경우에만 설정)
    void    PASCAL EXPORT CIPKeSetMainClk(INT16 axis, INT32 nMainClk);
    // Drive mode 1의 설정/확인한다.
		// pwm_ppls_mode : SMC-2V03에서는 해당 사항 없음.
		// decelstartpoint : 지정거리 구동 기능 사용중 감속 위치 지정 방식 설정(0 : 자동 가감속, 1 : 수동 가감속)
		// pulseoutmethod : 출력 펄스 방식 설정(typedef : PULSE_OUTPUT)
		// detecsignal : 신호 검색-1/2 구동 기능 사용중 검색 할 신호 설정(typedef : DETECT_DESTINATION_SIGNAL)
    void    PASCAL EXPORT CIPset_drive_mode1(INT16 axis, 
                UINT8 pwm_ppls_mode,
                UINT8 decelstartpoint,
                UINT8 pulseoutmethod,
                UINT8 detectsignal);
	UINT16    PASCAL EXPORT CIPget_drive_mode1(INT16 axis);

    // Drive mode 2의 설정/확인한다.
		// encmethod : 입력 인코더 방식 설정(typedef : EXTERNAL_COUNTER_INPUT)    
    void    PASCAL EXPORT CIPset_drive_mode2(INT16 axis, 
                UINT8 estopactivelevel,
                UINT8 sstopactivelevel,
                UINT8 trigactivelevel,
                UINT8 intactivelevel,
                UINT8 markactivelevel,
                UINT8 encmethod,
                UINT8 inpactivelevel,
                UINT8 alarmactivelevel,
                UINT8 nslmactivelevel,
                UINT8 pslmactivelevel,
                UINT8 nelmactivelevel,
                UINT8 pelmactivelevel);
    UINT16    PASCAL EXPORT CIPget_drive_mode2(INT16 axis);
    // Unit/Pulse 설정/확인
		// Unit/Pulse : 1 pulse에 대한 system의 이동거리를 말하며, 이때 Unit의 기준은 사용자가 임의로 생각할 수 있다.
		// Ex) Ball screw pitch : 10mm, 모터 1회전당 펄스수 : 10000 ==> Unit을 mm로 생각할 경우 : Unit/Pulse = 10/10000.
		// 따라서 unitperpulse에 0.001을 입력하면 모든 제어단위가 mm로 설정됨. 
		// Ex) Linear motor의 분해능이 1 pulse당 2 uM. ==> Unit을 mm로 생각할 경우 : Unit/Pulse = 0.002/1.
    void   PASCAL EXPORT CIPset_moveunit_perpulse(INT16 axis, double unitperpulse);
    double PASCAL EXPORT CIPget_moveunit_perpulse(INT16 axis);
    // pulse/Unit 설정/확인한다.
    void   PASCAL EXPORT CIPset_movepulse_perunit(INT16 axis, double pulseperunit);
    double PASCAL EXPORT CIPget_movepulse_perunit(INT16 axis);
    // 시작 속도 설정/확인한다.(Unit/Sec)
    void   PASCAL EXPORT CIPset_startstop_speed(INT16 axis, double velocity);
    double PASCAL EXPORT CIPget_startstop_speed(INT16 axis);
    // 최고 속도 설정/확인한다.(Unit/Sec) - 제어 system의 최고 속도를 설정한다. 구동속도는 최대속도로 제한된다.
		// Unit/Pulse 설정과 시작속도 설정 이후에 설정한다.
		// 설정된 최고 속도 이상으로는 구동을 할수 없으므로 주의한다.
    BOOL   PASCAL EXPORT CIPset_max_speed(INT16 axis, double max_velocity);
	double PASCAL EXPORT CIPget_max_speed(INT16 axis);
    // SW에 관계된 값을 설정/확인한다. 이값으로 S-Curve 구간을 percentage로 설정 가능하다.
    void   PASCAL EXPORT CIPset_s_rate(INT16 axis, double accel_percent, double decel_percent);
    void   PASCAL EXPORT CIPget_s_rate(INT16 axis, double *accel_percent, double *decel_percent);
    // 수동 가감속 모드에서 잔량 펄스를 설정/확인한다.
    void   PASCAL EXPORT CIPset_slowdown_rear_pulse(INT16 axis, UINT32 ulData);
    UINT32 PASCAL EXPORT CIPget_slowdown_rear_pulse(INT16 axis);
    // CAMC_IP의 interrupt mask bit 0를 설정하여 모션 완료에 대한 인터럽트를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_motiondone_interrupt_enable(INT16 axis, UINT8 ena_int);
    UINT8  PASCAL EXPORT CIPis_motiondone_interrupt_enabled(INT16 axis);
    // 지정 축의 감속 시작 포인터 검출 방식을 설정/확인한다.
		// 0x0 : 자동 가감속.
		// 0x1 : 수동 가감속.
    BOOL   PASCAL EXPORT CIPset_decel_point(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_decel_point(INT16 axis);    

    // 구동 상태 확인 함수군        -=====================================================================================
    // 지정 축의 펄스 출력 상태를 확인한다.
    BOOL   PASCAL EXPORT CIPin_motion(INT16 axis);
    // 지정 축의 펄스 출력 종료 상태를 확인한다.
    BOOL   PASCAL EXPORT CIPmotion_done(INT16 axis);
    // 지정 축의 구동시작 이후 출력된 펄스 카운터 값을 확인한다.(Pulse)
    INT32  PASCAL EXPORT CIPget_drive_pulse_counts(INT16 axis);
    // 지정 축의 DriveStatus 레지스터를 확인한다.
    UINT32 PASCAL EXPORT CIPget_drive_status(INT16 axis);
    // 지정 축의 EndStatus 레지스터를 확인한다.
		// End Status Bit별 의미
		// 15bit : Interpolation drvie에 의한 종료
		// 14bit : Soft Limit에 의한 종료
		// 13bit : Limit(PELM, NELM, PSLM, NSLM) 신호에 의한 종료
		// 12bit : Sensor positioning drive종료
		// 11bit : Preset pulse drive에 의한 종료(지정한 위치/거리만큼 움직이는 함수군)
		// 10bit : 신호 검출에 의한 종료(Signal Search-1/2 drive종료)
		// 9 bit : 원점 검출에 의한 종료
		// 8 bit : 탈조 에러에 의한 종료
		// 7 bit : 데이타 설정 에러에 의한 종료
		// 6 bit : ALARM 신호 입력에 의한 종료
		// 5 bit : 급정지 명령에 의한 종료
		// 4 bit : 감속정지 명령에 의한 종료
		// 3 bit : 급정지 신호 입력에 의한 종료 (EMG Button)
		// 2 bit : 감속정지 신호 입력에 의한 종료
		// 1 bit : Limit(PELM, NELM, Soft) 급정지에 의한 종료
		// 0 bit : Limit(PSLM, NSLM, Soft) 감속정지에 의한 종료
    UINT16 PASCAL EXPORT CIPget_end_status(INT16 axis);
    // 지정 축의 Mechanical 레지스터를 확인한다.
		// Mechanical Signal Bit별 의미
		// 14bit : MODE8_16 신호 입력 Level
		// 13bit : SYNC 신호 입력  Level
		// 12bit : ESTOP 신호 입력 Level
		// 11bit : SSTOP 신호 입력 Level
		// 10bit : MARK 신호 입력 Level
		// 9 bit : EXPP(MPG) 신호 입력 Level
		// 8 bit : EXMP(MPG) 신호 입력 Level
		// 7 bit : Encoder Up신호 입력 Level(A상 신호)
		// 6 bit : Encoder Down신호 입력 Level(B상 신호)
		// 5 bit : INPOSITION 신호 Active 상태
		// 4 bit : ALARM 신호 Active 상태
		// 3 bit : -Limit 감속정지 신호 Active 상태 (Ver3.0부터 사용되지않음)
		// 2 bit : +Limit 감속정지 신호 Active 상태 (Ver3.0부터 사용되지않음)
		// 1 bit : -Limit 급정지 신호 Active 상태
		// 0 bit : +Limit 급정지 신호 Active 상태
    UINT16 PASCAL EXPORT CIPget_mechanical_signal(INT16 axis);
    // 지정 축의  현재 속도를 읽어 온다.(Unit/Sec)
    double PASCAL EXPORT CIPget_velocity(INT16 axis);
    // 지정 축의 Command position과 Actual position의 차를 확인한다.
    double PASCAL EXPORT CIPget_error(INT16 axis);
    // 지정 축의 최후 드라이브의 이동 거리를 확인 한다. (Unit)
    double PASCAL EXPORT CIPget_drivedistance(INT16 axis);

    //Encoder 입력 방식 설정 함수군        -=============================================================================
    // 지정 축의 Encoder 입력 방식을 설정/확인한다.
		// method : typedef(EXTERNAL_COUNTER_INPUT)
		// UpDownMode = 0x0    // Up/Down
		// Sqr1Mode   = 0x1    // 1체배
		// Sqr2Mode   = 0x2    // 2체배
		// Sqr4Mode   = 0x3    // 4체배
    BOOL   PASCAL EXPORT CIPset_enc_input_method(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_enc_input_method(INT16 axis);
    // 지정 축의 외부 위치 counter clear의 기능을 설정/확인한다.
		// method : CAMC-IP chip 메뉴얼 EXTCNTCLR 레지스터 참조.
    BOOL   PASCAL EXPORT CIPset_enc2_input_method(INT16 axis, UINT8 method);
    UINT16 PASCAL EXPORT CIPget_enc2_input_method(INT16 axis);
    // 지정 축의 외부 위치 counter의 count 방식을 설정/확인한다.
		// reverse :
		// TRUE  : 입력 인코더에 반대되는 방향으로 count한다.
		// FALSE : 입력 인코더에 따라 정상적으로 count한다.
    BOOL   PASCAL EXPORT CIPset_enc_reverse(INT16 axis, UINT8 reverse);
    BOOL   PASCAL EXPORT CIPget_enc_reverse(INT16 axis);

    //펄스 출력 방식 함수군        -=====================================================================================
    // 펄스 출력 방식을 설정/확인한다.
		// method : 출력 펄스 방식 설정(typedef : PULSE_OUTPUT)
		// OneHighLowHigh   = 0x0, 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
		// OneHighHighLow   = 0x1, 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
		// OneLowLowHigh    = 0x2, 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
		// OneLowHighLow    = 0x3, 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
		// TwoCcwCwHigh     = 0x4, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High 
		// TwoCcwCwLow      = 0x5, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low 
		// TwoCwCcwHigh     = 0x6, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
		// TwoCwCcwLow      = 0x7, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
    BOOL   PASCAL EXPORT CIPset_pulse_out_method(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_pulse_out_method(INT16 axis);

    // 위치 확인 및 위치 비교 설정 함수군 -===============================================================================
    // 외부 위치 값을 설정한다. 현재의 상태에서 외부 위치를 특정 값으로 설정/확인한다.(position = Unit)
    void   PASCAL EXPORT CIPset_actual_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_actual_position(INT16 axis);
	// 외부 위치 비교기에 값을 설정/확인한다.(position = Unit)
    void   PASCAL EXPORT CIPset_actual_comparator_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_actual_comparator_position(INT16 axis);
    // 내부 위치 값을 설정한다. 현재의 상태에서 내부 위치를 특정 값으로 설정/확인한다.(position = Unit)
    void   PASCAL EXPORT CIPset_command_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_command_position(INT16 axis);
    // 내부 위치 비교기에 값을 설정/확인한다 (position = Unit)
    void   PASCAL EXPORT CIPset_command_comparator_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_command_comparator_position(INT16 axis);

    // 서보 드라이버 출력 신호 설정 함수군-===============================================================================
   	// 서보 Enable출력 신호의 Active Level을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_servo_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_servo_level(INT16 axis);
    // 서보 Enable(On) / Disable(Off)을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_servo_enable(INT16 axis, UINT8 state);
    UINT8  PASCAL EXPORT CIPget_servo_enable(INT16 axis);

    // 서보 드라이버 입력 신호 설정 함수군-===============================================================================
	// 서보 위치결정완료(inposition)입력 신호의 사용유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_inposition_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_inposition_enable(INT16 axis);
   	// 서보 위치결정완료(inposition)입력 신호의 Active Level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_inposition_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_inposition_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_inposition_switch(INT16 axis);
    BOOL   PASCAL EXPORT CIPin_position(INT16 axis);
	// 서보 알람 입력신호 기능의 사용유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_alarm_enable(INT16 axis, UINT8 use);
	UINT8  PASCAL EXPORT CIPget_alarm_enable(INT16 axis);
	// 서보 알람 입력 신호의 Active Level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_alarm_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_alarm_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_alarm_switch(INT16 axis);

	//MARK 신호(Sensor positioning drive에 사용)
	BOOL   PASCAL EXPORT CIPset_mark_signal_level(INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT CIPget_mark_signal_level(INT16 axis);
	UINT8  PASCAL EXPORT CIPget_mark_signal_switch(INT16 axis);
	BOOL   PASCAL EXPORT CIPset_mark_signal_enable(INT16 axis, UINT8 use);
	UINT8  PASCAL EXPORT CIPget_mark_signal_enable(INT16 axis);

    // 리미트 신호 설정 함수군-===========================================================================================
    // 급정지 리미트 기능 사용유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_end_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_end_limit_enable(INT16 axis);
	// -급정지 리미트 입력 신호의 Active Level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_nend_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_nend_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_nend_limit_switch(INT16 axis);
	// +급정지 리미트 입력 신호의 Active Level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_pend_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_pend_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_pend_limit_switch(INT16 axis);
    // 감속정지 리미트 기능 사용유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_slow_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_slow_limit_enable(INT16 axis);
	// -감속정지 리미트 입력 신호의 Active Level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_nslow_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_nslow_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_nslow_limit_switch(INT16 axis);
	// +감속정지 리미트 입력 신호의 Active Level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_pslow_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_pslow_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_pslow_limit_switch(INT16 axis);
    // +급정지 리미트 기능 사용유무를 설정/확인한다.(급정지 리미트에 대하여 +와 -에 대하여 각각 설정 가능하다.)
    BOOL   PASCAL EXPORT CIPset_pend_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_pend_limit_enable(INT16 axis);
    // -급정지 리미트 기능 사용유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_nend_limit_enable(INT16 axis, UINT8 use);    
    UINT8  PASCAL EXPORT CIPget_nend_limit_enable(INT16 axis);
    // +감속정지 리미트 기능 사용유무를 설정/확인한다.(감속정지 리미트에 대하여 +와 -에 대하여 각가 설정 가능하다.)
    BOOL   PASCAL EXPORT CIPset_pslow_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_pslow_limit_enable(INT16 axis);
    // -감속정지 리미트 기능 사용유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_nslow_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_nslow_limit_enable(INT16 axis);

    // 소프트웨어 리미트 설정 함수군-=====================================================================================
    // 소프트웨어 리미트 사용유무를 설정/확인한다.
    void   PASCAL EXPORT CIPset_soft_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_soft_limit_enable(INT16 axis);
    // 소프트웨어 리미트 사용시 기준위치정보를 설정/확인한다.
		// sel :
		// 0x0 : 내부위치에 대하여 소프트웨어 리미트 기능 실행.
		// 0x1 : 외부위치에 대하여 소프트웨어 리미트 기능 실행.
    void   PASCAL EXPORT CIPset_soft_limit_sel(INT16 axis, UINT8 sel);
    UINT8  PASCAL EXPORT CIPget_soft_limit_sel(INT16 axis);
    // 소프트웨어 리미트 발생시 정지 모드를 설정/확인한다.
		// mode :
		// 0x0 : 소프트웨어 리미트 위치에서 급정지 한다.
		// 0x1 : 소프트웨어 리미트 위치에서 감속정지 한다.
    void   PASCAL EXPORT CIPset_soft_limit_stopmode(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_soft_limit_stopmode(INT16 axis);
    // 소프트웨어 리미트 -위치값 설정/확인한다.(position = Unit)
    void   PASCAL EXPORT CIPset_soft_nlimit_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_soft_nlimit_position(INT16 axis);
    // 소프트웨어 리미트 +위치값 설정/확인 한다.(position = Unit)
    void   PASCAL EXPORT CIPset_soft_plimit_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_soft_plimit_position(INT16 axis);

    // 비상정지 신호-=====================================================================================================
    // ESTOP, SSTOP 신호 사용유무를 설정/확인한다.(Emergency stop, Slow-Down stop)
    BOOL   PASCAL EXPORT CIPset_emg_signal_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_emg_signal_enable(INT16 axis);
    // SSTOP 입력신호의 Active level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_sstop_signal_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_sstop_signal_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_sstop_signal_switch(INT16 axis);
    // ESTOP 입력신호의 Active level을 설정/확인/상태확인한다.
    BOOL   PASCAL EXPORT CIPset_estop_signal_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_estop_signal_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_estop_signal_switch(INT16 axis);

    //단축 지정 거리 구동-===============================================================================================
    //start_** : 지정 축에서 구동 시작후 함수를 return한다. "start_*" 가 없으면 이동 완료후 return한다(Blocking).
    //*r*_*    : 지정 축에서 입력된 거리만큼(상대좌표)로 이동한다. "*r_*이 없으면 입력된 위치(절대좌표)로 이동한다.
    //*s*_*    : 구동중 속도 프로파일을 "S curve"를 이용한다. "*s_*"가 없다면 사다리꼴 가감속을 이용한다.
    //*a*_*    : 구동중 속도 가감속도를 비대칭으로 사용한다. 가속률 또는 가속 시간과  감속률 또는 감속 시간을 각각 입력받는다.
    //*_ex     : 구동중 가감속도를 가속 또는 감속 시간으로 입력 받는다. "*_ex"가 없다면 가감속률로 입력 받는다.
    //입력 값들: velocity(Unit/Sec), acceleration/deceleration(Unit/Sec^2), acceltime/deceltime(Sec), position(Unit)

	// 대칭 지정펄스(Pulse Drive), 사다리꼴 구동 함수, 절대/상대좌표(r), 가속율/가속시간(_ex)(시간단위:Sec)
	// Blocking함수 (제어권이 펄스 출력이 완료된 후 넘어옴)
    UINT16 PASCAL EXPORT CIPmove(INT16 axis, double position, double velocity, double acceleration);
    UINT16 PASCAL EXPORT CIPmove_ex(INT16 axis, double position, double velocity, double acceltime);
    UINT16 PASCAL EXPORT CIPr_move(INT16 axis, double distance, double velocity, double acceleration);
    UINT16 PASCAL EXPORT CIPr_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking함수 (구동중일 경우 무시됨)
	BOOL   PASCAL EXPORT CIPstart_move(INT16 axis, double position, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_move_ex(INT16 axis, double position, double velocity, double acceltime);
    BOOL   PASCAL EXPORT CIPstart_r_move(INT16 axis, double distance, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_r_move_ex(INT16 axis, double distance, double velocity, double acceltime);

	// 비대칭 지정펄스(Pulse Drive), 사다리꼴 구동 함수, 절대/상대좌표(r), 가속율/가속시간(_ex)(시간단위:Sec)
	// Blocking함수 (제어권이 펄스 출력이 완료된 후 넘어옴)
    UINT16 PASCAL EXPORT CIPa_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPa_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
    UINT16 PASCAL EXPORT CIPra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// Non Blocking함수 (구동중일 경우 무시됨)
    BOOL   PASCAL EXPORT CIPstart_a_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_a_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
    BOOL   PASCAL EXPORT CIPstart_ra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_ra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
 	// 대칭 지정펄스(Pulse Drive), S자형 구동, 절대/상대좌표(r), 가속율/가속시간(_ex)(시간단위:Sec)
	// Blocking함수 (제어권이 펄스 출력이 완료된 후 넘어옴)
    UINT16 PASCAL EXPORT CIPs_move(INT16 axis, double position, double velocity, double acceleration);
    UINT16 PASCAL EXPORT CIPs_move_ex(INT16 axis, double position, double velocity, double acceltime);
    UINT16 PASCAL EXPORT CIPrs_move(INT16 axis, double distance, double velocity, double acceleration);	
    UINT16 PASCAL EXPORT CIPrs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking함수 (구동중일 경우 무시됨)
    BOOL   PASCAL EXPORT CIPstart_s_move(INT16 axis, double position, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_s_move_ex(INT16 axis, double position, double velocity, double acceltime);
    BOOL   PASCAL EXPORT CIPstart_rs_move(INT16 axis, double distance, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_rs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// 비대칭 지정펄스(Pulse Drive), S자형 구동, 절대/상대좌표(r), 가속율/가속시간(_ex)(시간단위:Sec)
	// Blocking함수 (제어권이 펄스 출력이 완료된 후 넘어옴)
	UINT16 PASCAL EXPORT CIPas_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPas_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
    UINT16 PASCAL EXPORT CIPras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// Non Blocking함수 (구동중일 경우 무시됨), jerk사용(단위 : 퍼센트) 포물선가속 S자 이동사용시.	
	BOOL   PASCAL EXPORT CIPstart_as_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_as_move2(INT16 axis, double position, double velocity, double acceleration, double deceleration, double jerk);
    BOOL   PASCAL EXPORT CIPstart_as_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime); 
    BOOL   PASCAL EXPORT CIPstart_ras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_ras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);

	// MARK, 대칭 Sensor positioning 사다리꼴 구동, 상대좌표, 가속율/가속시간(Sec)
	BOOL   PASCAL EXPORT CIPstart_pr_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_pr_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, 비대칭 Sensor positioning 사다리꼴 구동, 상대좌표, 가속율/가속시간(Sec)
	BOOL   PASCAL EXPORT CIPstart_pra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_pra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// Sensor positioning 사다리꼴 구동, 상대좌표, 가속율/가속시간(Sec). 구동이 완료될때까지 대기
	UINT16 PASCAL EXPORT CIPpr_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPpr_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, 비대칭 Sensor positioning 사다리꼴 구동, 상대좌표, 가속율/가속시간(Sec). 구동이 완료될때까지 대기
	UINT16 PASCAL EXPORT CIPpra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPpra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK, 대칭 Sensor positioning S자형 구동, 상대좌표, 가속율/가속시간(Sec)
	BOOL   PASCAL EXPORT CIPstart_prs_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_prs_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, 비대칭 Sensor positioning S자형 구동, 상대좌표, 가속율/가속시간(Sec)
	BOOL   PASCAL EXPORT CIPstart_pras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_pras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK, Sensor positioning S자형 구동, 상대좌표, 가속율/가속시간(Sec). 구동이 완료될때까지 대기
	UINT16 PASCAL EXPORT CIPprs_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPprs_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, 비대칭 Sensor positioning S자형 구동, 상대좌표, 가속율/가속시간(Sec). 구동이 완료될때까지 대기
	UINT16 PASCAL EXPORT CIPpras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPpras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	
	// 대칭 지정 펄스(Pulse Drive), S자형 구동, 상대좌표, 가속율,
	// Non Blocking (구동중일 경우 무시됨), 현재 위치를 기준으로 over_distance에서 over_velocity로 속도를 변경 한다.
	BOOL   PASCAL EXPORT CIPstart_rs_move_override(INT16 axis, double distance, double velocity, double acceleration, double over_distance, double over_velocity, BOOL Target);

    //단축 연속 구동-====================================================================================================
    // 지정 가감속도 및 속도로 정지 조건이 발생하지 않으면 지속적으로 구동한다.
    //*s*_*    : 구동중 속도 프로파일을 "S curve"를 이용한다. "*s_*"가 없다면 사다리꼴 가감속을 이용한다.
    //*a*_*    : 구동중 속도 가감속도를 비대칭으로 사용한다. 가속률 또는 가속 시간과  감속률 또는 감속 시간을 각각 입력받는다.
    //*_ex     : 구동중 가감속도를 가속 또는 감속 시간으로 입력 받는다. "*_ex"가 없다면 가감속률로 입력 받는다.
 
	// 정속도 사다리꼴 구동 함수군, 가속율/가속시간(_ex)(시간단위:Sec) - 구동중일 경우에는 속도오버라이드
	// 대칭 가감속 구동함수
	BOOL   PASCAL EXPORT CIPv_move(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPv_move_ex(INT16 axis, double velocity, double acceltime);
	// 비대칭 가감속 구동함수
    BOOL   PASCAL EXPORT CIPv_a_move(INT16 axis, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPv_a_move_ex(INT16 axis, double velocity, double acceltime, double deceltime);
	// 정속도 S자형 구동 함수군, 가속율/가속시간(_ex)(시간단위:Sec) - 구동중일 경우에는 속도오버라이드
	// 대칭 가감속 구동함수
    BOOL   PASCAL EXPORT CIPv_s_move(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPv_s_move_ex(INT16 axis, double velocity, double acceltime);
	// 비대칭 가감속 구동함수
    BOOL   PASCAL EXPORT CIPv_as_move(INT16 axis, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPv_as_move_ex(INT16 axis, double velocity, double acceltime, double deceltime);
        
    // 신호 검출 구동-====================================================================================================
    // 지정 신호의 상향/하향 에지를 검색하여 급정지 또는 감속정지를 할 수 있다.
    // detect_signal : 검색 신호 설정(typedef : DETECT_DESTINATION_SIGNAL)
    // PElmNegativeEdge    = 0x0,        // +Elm(End limit) 하강 edge
    // NElmNegativeEdge    = 0x1,        // -Elm(End limit) 하강 edge
    // PSlmNegativeEdge    = 0x2,        // +Slm(Slowdown limit) 하강 edge
    // NSlmNegativeEdge    = 0x3,        // -Slm(Slowdown limit) 하강 edge
    // In0DownEdge         = 0x4,        // IN0(ORG) 하강 edge
    // In1DownEdge         = 0x5,        // IN1(Z상) 하강 edge
    // In2DownEdge         = 0x6,        // IN2(범용) 하강 edge
    // In3DownEdge         = 0x7,        // IN3(범용) 하강 edge
    // PElmPositiveEdge    = 0x8,        // +Elm(End limit) 상승 edge
    // NElmPositiveEdge    = 0x9,        // -Elm(End limit) 상승 edge
    // PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) 상승 edge
    // NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) 상승 edge
    // In0UpEdge           = 0xc,        // IN0(ORG) 상승 edge
    // In1UpEdge           = 0xd,        // IN1(Z상) 상승 edge
    // In2UpEdge           = 0xe,        // IN2(범용) 상승 edge
    // In3UpEdge           = 0xf         // IN3(범용) 상승 edge
    // Signal Search1 : 구동 시작후 입력 속도까지 가속하여, 신호 검출후 감속 정지.
    // Signal Search2 : 구동 시작후 가속없이 입력 속도가 되고, 신호 검출후 급정지. 
    // 주의 : Signal Search2는 가감속이 없으므로 속도가 높을경우 탈조및 기구부의 무리가 갈수 있으므로 주의한다.
    // *s*_*    : 구동중 속도 프로파일을 "S curve"를 이용한다. "*s_*"가 없다면 사다리꼴 가감속을 이용한다.
    // *_ex     : 구동중 가감속도를 가속 또는 감속 시간으로 입력 받는다. "*_ex"가 없다면 가감속률로 입력 받는다.

    // 신호검출1(Signal search 1) 사다리꼴 구동, 가속율/가속시간(_ex)(시간단위:Sec)
	BOOL   PASCAL EXPORT CIPstart_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
    BOOL   PASCAL EXPORT CIPstart_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
    // 신호검출1(Signal search 1) S자형 구동, 가속율/가속시간(_ex)(시간단위:Sec)
	BOOL   PASCAL EXPORT CIPstart_s_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
    BOOL   PASCAL EXPORT CIPstart_s_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
    // 신호검출2(Signal search 2) 사다리꼴 구동, 가감속 없음
	BOOL   PASCAL EXPORT CIPstart_signal_search2(INT16 axis, double velocity, UINT8 detect_signal);

    // MPG(Manual Pulse Generation) 구동 설정-===========================================================================
    // 지정 축에 MPG(Manual Pulse Generation) 드라이버의 구동 모드를 설정/확인한다.
		// mode
		// 0x1 : Slave 구동모드, 외부 Differential 신호에 의한 출력
		// 0x2 : 지정 펄스 구동, 외부 입력 신호에 의한 지정 펄스 구동 시작
		// 0x4 : 연속 구동 모드, 외부 접점 입력 신호의 특정 레벨 동안 구동
    BOOL   PASCAL EXPORT CIPset_mpg_drive_mode(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_mpg_drive_mode(INT16 axis);
	// 지정 축에 MPG(Manual Pulse Generation) 드라이버의 지정 펄스 구동시 사용하는 거리 정보를 설정/확인한다.
	BOOL   PASCAL EXPORT CIPset_mpg_position(INT16 axis, double position);	//(position = Unit)
	double PASCAL EXPORT CIPget_mpg_position(INT16 axis);

    // 지정 축에 MPG(Manual Pulse Generation) 드라이버의 구동 방향 결정모드를 설정/확인한다.
		// mode
		// 0x0 : 외부 신호에 의한 방향 결정
		// 0x1 : 사용자에 의해 지정된 방향으로 구동
    BOOL   PASCAL EXPORT CIPset_mpg_dir_mode(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_mpg_dir_mode(INT16 axis);
    // 지정 축에 MPG(Manual Pulse Generation) 드라이버의 구동 방향 결정모드가 사용자에 의해
    // 지정된 방향으로 설정되었을 때 필요한 사용자의 구동 방향 지정 값을 설정/확인한다.
		// mode
		// 0x0 : 사용자 지정 구동 방향을 +로 설정
		// 0x1 : 사용자 지정 구동 방향을 -로 설정
    BOOL   PASCAL EXPORT CIPset_mpg_user_dir(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_mpg_user_dir(INT16 axis);

    // 지정 축에 MPG(Manual Pulse Generation) 드라이버에 사용되는 EXPP/EXMP 의 입력 모드를 설정한다.
    //  2 bit : '0' : level input(범용 입력 4 = EXPP, 범용 입력 5 = EXMP로 입력 받는다.)
    //          '1' : Differential input(차동 입력으로 EXPP, EXMP를 입력 받음,)
    //  1~0bit: "00" : 1 phase
    //          "01" : 2 phase 1 times
    //          "10" : 2 phase 2 times
    //          "11" : 2 phase 4 times
    BOOL   PASCAL EXPORT CIPset_mpg_input_method(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_mpg_input_method(INT16 axis);
    
    // MPG(Manual Pulse Generation) 구동 -===============================================================================
	// 설정된 속도로 사다리꼴 구동, 가속율/가속시간(_ex)(시간단위:Sec)
    BOOL   PASCAL EXPORT CIPstart_mpg(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_mpg_ex(INT16 axis, double velocity, double acceltime);
	// 설정된 속도로 S자형 구동, 가속율/가속시간(_ex)(시간단위:Sec)
    BOOL   PASCAL EXPORT CIPstart_s_mpg(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_s_mpg_ex(INT16 axis, double velocity, double acceltime);

    // 오버라이드(구동중)-================================================================================================
    // 단축 지정 거리 구동시 구동 시작시점에서 입력한 위치(절대위치)를 구동중 바꾼다.
    BOOL   PASCAL EXPORT CIPposition_override(INT16 axis, double overrideposition);
    // 단축 지정 거리 구동시 구동 시작시점에서 입력한 거리(상대위치)를 구동중 바꾼다.    
    BOOL   PASCAL EXPORT CIPposition_r_override(INT16 axis, double overridedistance);
    // 구동중 구동 초기 설정한 속도를 바꾼다.(set_max_speed > dbVel > set_startstop_speed)
    BOOL   PASCAL EXPORT CIPvelocity_override(INT16 axis, double dbVel);
    // 지정 축의 구동이 종료되기 전 입력된 overrideposition까지 최소 출력 펄스(dec_pulse) 이상일 경우 override 동작을 한다.
    BOOL   PASCAL EXPORT CIPposition_override2(INT16 axis, double overrideposition, double dec_pulse);
    // 지정 축에 가속/감속 프로 파일을 가지는 가감속으로 속도 override 동작을 한다.
    BOOL   PASCAL EXPORT CIPvelocity_override2(INT16 axis, double velocity, double acceleration, double deceleration, double jerk);
    
    // 단축 구동 확인-====================================================================================================
    // 지정 축의 구동이 종료될 때까지 기다린 후 함수를 벗어난다.
    UINT16 PASCAL EXPORT CIPwait_for_done(INT16 axis);
    
    // 단축 구동 정지-====================================================================================================
    // 지정 축을 급정지한다.
    BOOL   PASCAL EXPORT CIPset_e_stop(INT16 axis);
    // 지정 축을 구동시 감속율로 정지한다.
    BOOL   PASCAL EXPORT CIPset_stop(INT16 axis);
    // 지정 축을 입력된 감속율로 정지한다.
    BOOL   PASCAL EXPORT CIPset_stop_decel(INT16 axis, double deceleration);
    // 지정 축을 감속 시간으로 정지한다.
    BOOL   PASCAL EXPORT CIPset_stop_deceltime(INT16 axis, double deceltime);

    // 다축 동기 구동관련 설정-==========================================================================================
    // Master/Slave link 또는 좌표계 link 둘중 하나를 사용하여야 한다.
    // Master/Slave link 설정. (일반 단축 구동시 master 축 구동시 slave축도 같이 구동된다.)
    // Master/Slave link 설정
    BOOL   PASCAL EXPORT CIPlink(INT16 master, INT16 slave, double ratio);
    // Master/Slave link 해제
    BOOL   PASCAL EXPORT CIPendlink(INT16 slave);
    
    // 좌표계 link 설정-================================================================================================
	// 지정 좌표계에 축 할당 - n_axes갯수만큼의 축수를 설정/확인한다.(coordinate는 1..8까지 사용 가능)
	// n_axes 갯수만큼의 축수를 설정/확인한다. - (n_axes는 1..4까지 사용 가능)
    BOOL   PASCAL EXPORT CIPmap_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
    BOOL   PASCAL EXPORT CIPget_mapped_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
    // 지정 좌표계의 상대/절대 모드 설정/확인한다.
  		// mode:
		// 0: 상대좌표구동, 1: 절대좌표 구동
    void   PASCAL EXPORT CIPset_coordinate_mode(INT16 coordinate, INT16 mode);
    INT16  PASCAL EXPORT CIPget_coordinate_mode(INT16 coordinate);
    // 지정 좌표계의 속도 프로파일 설정/확인한다.
		// mode:
		// 0: 사다리꼴 구동, 1: S커브 구동
    void   PASCAL EXPORT CIPset_move_profile(INT16 coordinate, INT16 mode);
    INT16  PASCAL EXPORT CIPget_move_profile(INT16 coordinate);	
	// 지정 좌표계의 초기 속도를 설정/확인한다.
	//    void   PASCAL EXPORT CIPset_move_startstop_velocity(INT16 coordinate, double velocity);
	//    double PASCAL EXPORT CIPget_move_startstop_velocity(INT16 coordinate);
    // 특정 좌표계의 속도를 설정/확인한다.
    void   PASCAL EXPORT CIPset_move_velocity(INT16 coordinate, double velocity);
    double PASCAL EXPORT CIPget_move_velocity(INT16 coordinate);
    // 특정 좌표계의 가속율을 설정/확인한다.
    void   PASCAL EXPORT CIPset_move_acceleration(INT16 coordinate, double acceleration);
	double PASCAL EXPORT CIPget_move_acceleration(INT16 coordinate);
    // 특정 좌표계의 가속 시간(Sec)을 설정/확인한다.
    void   PASCAL EXPORT CIPset_move_acceltime(INT16 coordinate, double acceltime);
    double PASCAL EXPORT CIPget_move_acceltime(INT16 coordinate);
    // 보간 구동중인  좌표계의 현재 구동속도를 반환한다.
    double PASCAL EXPORT CIPco_get_velocity(INT16 coordinate);

    // 소프트웨어 보간 구동(지정 좌표계에 대하여)-========================================================================
	// Blocking함수 (제어권이 펄스 출력이 완료된 후 넘어옴)
    // 2, 3, 4축이 동시이동한다.
    BOOL   PASCAL EXPORT CIPmove_2(INT16 coordinate, double x, double y);
    BOOL   PASCAL EXPORT CIPmove_3(INT16 coordinate, double x, double y, double z);
    BOOL   PASCAL EXPORT CIPmove_4(INT16 coordinate, double x, double y, double z, double w);
 	// Non Blocking함수 (구동중일 경우 무시됨)
    // 2, 3, 4축이 동시이동한다.
    BOOL   PASCAL EXPORT CIPstart_move_2(INT16 coordinate, double x, double y);
    BOOL   PASCAL EXPORT CIPstart_move_3(INT16 coordinate, double x, double y, double z);
    BOOL   PASCAL EXPORT CIPstart_move_4(INT16 coordinate, double x, double y, double z, double w);
    // 지정 좌표계의 모든축의 모션 완료 체크    
    BOOL   PASCAL EXPORT CIPco_motion_done(INT16 coordinate);
    // 지정 좌표계의 구동이 완료될때 까지 기다린다.
    BOOL   PASCAL EXPORT CIPco_wait_for_done(INT16 coordinate);

    // 다축 구동(동기 구동) : Master/Slave로 link되어 있을 경우 오류가 발생 할 수 있다.-==================================
    // 지정 축들을 지정 거리 및 속도 가속도 정보로 동기 시작 구동한다. 구동 시작에 대한 동기화시 사용한다. 
    // start_** : 지정 축에서 구동 시작후 함수를 return한다. "start_*" 가 없으면 이동 완료후 return한다.
    // *r*_*    : 지정 축에서 입력된 거리만큼(상대좌표)로 이동한다. "*r_*이 없으면 입력된 위치(절대좌표)로 이동한다.
    // *s*_*    : 구동중 속도 프로파일을 "S curve"를 이용한다. "*s_*"가 없다면 사다리꼴 가감속을 이용한다.
    // *_ex     : 구동중 가감속도를 가속 또는 감속 시간으로 입력 받는다. "*_ex"가 없다면 가감속률로 입력 받는다.

	// 다축 지정펄스(Pulse Drive)구동, 사다리꼴 구동, 절대/상대좌표(r), 가속율/가속시간(_ex)(시간단위:Sec)
	// Blocking함수 (제어권이 모든 구동축의 펄스 출력이 완료된 후 넘어옴)
    UINT8  PASCAL EXPORT CIPmove_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	UINT8  PASCAL EXPORT CIPmove_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    UINT8  PASCAL EXPORT CIPr_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    UINT8  PASCAL EXPORT CIPr_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking함수 (구동중인 축은 무시됨)
	BOOL   PASCAL EXPORT CIPstart_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    BOOL   PASCAL EXPORT CIPstart_r_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_r_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
 	// 다축 지정펄스(Pulse Drive)구동, S자형 구동, 절대/상대좌표(r), 가속율/가속시간(_ex)(시간단위:Sec)
	// Blocking함수 (제어권이 모든 구동축의 펄스 출력이 완료된 후 넘어옴)
    UINT8  PASCAL EXPORT CIPs_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
    UINT8  PASCAL EXPORT CIPs_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    UINT8  PASCAL EXPORT CIPrs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    UINT8  PASCAL EXPORT CIPrs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking함수 (구동중인 축은 무시됨)
    BOOL   PASCAL EXPORT CIPstart_s_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_s_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    BOOL   PASCAL EXPORT CIPstart_rs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_rs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
    //지정 축들에 대하여 S자형 구동을 위한 가감속시의 S커브의 비율을 설정/확인한다.
    void   PASCAL EXPORT CIPset_s_rate_all(INT16 number, INT16 *axes, double *accel_percent, double *decel_percent);
    void   PASCAL EXPORT CIPget_s_rate_all(INT16 number, INT16 *axes, double *accel_percent, double *decel_percent);

    // 다축 구동 확인-====================================================================================================
    // 입력 해당 축들의 구동 상태를 확인하고 구동이 끝날 때 까지 기다린다.
    UINT8  PASCAL EXPORT CIPwait_for_all(INT16 number, INT16 *axes);

    // 다축 동기 설정-====================================================================================================
    // 지정 축들의 동기를 해제시킨다. - 구동명령이 내려져도 구동되지않고 대기함.
    BOOL   PASCAL EXPORT CIPreset_axis_sync(INT16 nLen, INT16 *aAxis);
    // 지정 축들의 동기를 해제시킨다. - 구동명령이 내려져도 구동되지않고 대기함.
    BOOL   PASCAL EXPORT CIPset_axis_sync(INT16 nLen, INT16 *aAxis);

    // 다축 구동 정지-====================================================================================================
    // 홈 서치 쓰레드도 정지
	BOOL   PASCAL EXPORT CIPemergency_stop();        

    // -원점검색 =========================================================================================================
    // 라이브러리 상에서 Thread를 사용하여 검색한다. 주의 : 구동후 칩내의 StartSto speed가 변할 수 있다.
    // 원점검색을 종료한다.
		// bStop:
		// 0: 감속정지
		// 1: 급정지
    BOOL   PASCAL EXPORT CIPabort_home_search(INT16 axis, UINT8 bStop);
    // 원점검색을 시작한다. 시작하기 전에 원점검색에 필요한 설정이 필요하다.
    BOOL   PASCAL EXPORT CIPhome_search(INT16 axis);
    // 입력 축들을 동시에 원점검색을 실시한다.
    BOOL   PASCAL EXPORT CIPhome_search_all(INT16 number, INT16 *axes);
    // 원점검색 완료 상태를 확인한다.
		// 반환값: 0: 원점검색 진행중, 1: 원점검색 종료
    BOOL   PASCAL EXPORT CIPget_home_done(INT16 axis);
    // 해당 축들의 원점검색 상태를 확인한다.
    BOOL   PASCAL EXPORT CIPget_home_done_all(INT16 number, INT16 *axes);
    // 지정 축의 원점 검색 실행후 종료 상태를 확인한다.
		// 반환값: 0: 원점검색 실패, 1: 원점검색 성공
    UINT8  PASCAL EXPORT CIPget_home_end_status(INT16 axis);
    // 지정 축들의 원점 검색 실행후 종료 상태를 확인한다.
    BOOL   PASCAL EXPORT CIPget_home_end_status_all(INT16 number, INT16 *axes, UINT8 *endstatus);
    // 원점 검색시 각 스텝마다 method를 설정/확인한다.
    // Method에 대한 설명 
    //    0 Bit 스텝 사용여부 설정 (0 : 사용하지 않음, 1: 사용함
    //    1 Bit 가감속 방법 설정 (0 : 가속율, 1 : 가속 시간)
    //    2 Bit 정지방법 설정 (0 : 감속 정지, 1 : 급 정지)
    //    3 Bit 검색방향 설정 (0 : cww(-), 1 : cw(+))
    // 7654 Bit detect signal 설정(typedef : DETECT_DESTINATION_SIGNAL)
    BOOL   PASCAL EXPORT CIPset_home_method(INT16 axis, INT16 nstep, UINT8 *method);
    BOOL   PASCAL EXPORT CIPget_home_method(INT16 axis, INT16 nstep, UINT8 *method);
    // 원점 검색시 각 스텝마다 offset을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_home_offset(INT16 axis, INT16 nstep, double *offset);
    BOOL   PASCAL EXPORT CIPget_home_offset(INT16 axis, INT16 nstep, double *offset);
    // 각 축의 원점 검색 속도를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_home_velocity(INT16 axis, INT16 nstep, double *velocity);
    BOOL   PASCAL EXPORT CIPget_home_velocity(INT16 axis, INT16 nstep, double *velocity);
    // 지정 축의 원점 검색 시 각 스텝별 가속율을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
    BOOL   PASCAL EXPORT CIPget_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
    // 지정 축의 원점 검색 시 각 스텝별 가속 시간을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
    BOOL   PASCAL EXPORT CIPget_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
    // 지정 축에 원점 검색에서 엔코더 'Z'상 검출 사용 시 구동 한계값를 설정/확인한다.(Pulse) - 범위를 벗어나면 검색 실패
    BOOL   PASCAL EXPORT CIPset_zphase_search_range(INT16 axis, INT16 pulses);
    INT16  PASCAL EXPORT CIPget_zphase_search_range(INT16 axis);
	// 현재 위치를 원점(0 Position)으로 설정한다 - 구동중이면 무시됨.
    BOOL   PASCAL EXPORT CIPhome_zero(INT16 axis);
	// 설정한 모든 축의 현재 위치를 원점(0 Position)으로 설정한다. - 구동중인 축은 무시됨
    BOOL   PASCAL EXPORT CIPhome_zero_all(INT16 number, INT16 *axes);

    //지정한 축에 시작/드라이브 속도 및 가감속율을 입력하고 설정한 원점 검색 모드를 사용하여
    //CAMC-IP 칩 내의 원점 검색 드라이브를 수행한다. 원점 구동 시작 후 함수를 벗어난다.
    //주의 : CAMC-IP 칩 내의 원점 검색 수행시 +-End limit와 Org sensor가 결선 되어야 하며 End limit 기능이 Enable되어야 한다.
    //2 bit : 원점검색 방향.(0 : Plus direction, 1 : Minus direction)
    //1 bit : 원점검색시 사용할 속도 Profile(0 : Trapzoidal, 1: S-curve)
    //0 bit : 원점 센서의 최종 검색 신호 선택(0 : falling edge, 1:rising edge)
    BOOL   PASCAL EXPORT CIPstart_home_search_chip(INT16 axis, double startstop, double velocity, double acceleration, UINT8 home_search_mode);
    BOOL   PASCAL EXPORT CIPstart_home_search_chip_ex(INT16 axis, double startstop, double velocity, double acceltime, UINT8 home_search_mode);
    //지정한 축에 시작/드라이브 속도 및 가감속율을 입력하고 설정한 원점 검색 모드를 사용하여
    //CAMC-IP 칩 내의 원점 검색 드라이브를 수행한다. 원점 구동 시작 후 함수를 벗어난다.
    //3 bit : 원점검색 완료후 위치 clear 설정(0: Clear count after search, 1: Maintain count after search)
    //2 bit : 원점검색 방향.(0 : Plus direction, 1 : Minus direction)
    //1 bit : 원점검색시 사용할 속도 Profile(0 : Trapzoidal, 1: S-curve)
    //0 bit : 원점 센서의 최종 검색 신호 선택(0 : falling edge, 1:rising edge)
    BOOL   PASCAL EXPORT CIPhome_search_chip(INT16 axis, double startstop, double velocity, double acceleration, UINT8 home_search_mode);
    BOOL   PASCAL EXPORT CIPhome_search_chip_ex(INT16 axis, double startstop, double velocity, double acceltime, UINT8 home_search_mode);

    // 범용 입출력-=======================================================================================================
    // 범용 출력 값을 설정한다. 해당 축의 value(0x00 ~ 0x3F)에 해당하는 범용출력을 'On'시킨다.
    // 범용 출력
    // 0 bit(CAMC-IP :UIO0) : 범용 출력 0(Servo-On)
    // 1 bit(CAMC-IP :UIO1) : 범용 출력 1(ALARM Clear)
    // 2 bit(CAMC-IP :UIO2) : 범용 출력 2
    // 3 bit(CAMC-IP :UIO3) : 범용 출력 3
    // 4 bit(CAMC-IP :UIO4) : 범용 출력 4
    // 5 bit(PLD)  : 범용 출력 5
    // 범용 입력
    // 0 bit(CAMC-IP :UIO5) : 범용 입력 0(ORiginal Sensor)
    // 1 bit(CAMC-IP :UIO6) : 범용 입력 1(Z phase)
    // 2 bit(CAMC-IP :UIO7) : 범용 입력 2
    // 3 bit(CAMC-IP :UIO8) : 범용 입력 3
    // 4 bit(CAMC-IP :UIO9) : 범용 입력 4
    // 5 bit(CAMC-IP :UIO10) : 범용 입력 5
    // 6 bit(CAMC-IP :UIO11) : 범용 입력 6
    // On ==> 단자대 N24V, 'Off' ==> 단자대 Open(float).

    // 범용 출력값을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_output(INT16 axis, UINT8 value);
    UINT8  PASCAL EXPORT CIPget_output(INT16 axis);
    // 범용 입력 값을 확인한다.
    // '1'('On') <== 단자대 N24V와 연결됨, '0'('Off') <== 단자대 P24V 또는 Float.
    UINT8  PASCAL EXPORT CIPget_input(INT16 axis);
    // 해당 축의 해당 bit의 출력을 'On' 시킨다.
    // bitNo : 0 ~ 5.
    BOOL   PASCAL EXPORT CIPset_output_bit(INT16 axis, UINT8 bitNo);
    // 해당 축의 해당 범용 출력을 'Off' 시킨다.
    BOOL   PASCAL EXPORT CIPreset_output_bit(INT16 axis, UINT8 bitNo);    
    // 해당 축의 해당 범용 출력 bit의 출력 상태를 확인한다.
    // bitNo : 0 ~ 5.    
    BOOL   PASCAL EXPORT CIPoutput_bit_on(INT16 axis, UINT8 bitNo);
    // 해당 축의 해당 범용 출력 bit의 상태를 입력 state로 바꾼다.
    // bitNo : 0 ~ 5. 
    BOOL   PASCAL EXPORT CIPchange_output_bit(INT16 axis, UINT8 bitNo, UINT8 state);
    // 해당 축의 해당 범용 입력 bit의 상태를 확인 한다.
    // bitNo : 0 ~ 6.
    BOOL   PASCAL EXPORT CIPinput_bit_on(INT16 axis, UINT8 bitNo);
    
    // 잔여 펄스 clear-===================================================================================================
	// 해당 축의 서보팩 잔여 펄스 Clear 출력의 사용 여부를 설정/확인한다.    
    // CLR 신호의 Default 출력 ==> 단자대 Open이다.
    BOOL   PASCAL EXPORT CIPset_crc_mask(INT16 axis, INT16 mask);
    UINT8  PASCAL EXPORT CIPget_crc_mask(INT16 axis);
    // 해당 축의 잔여 펄스 Clear 출력의 Active level을 설정/확인한다.
    // Default Active level ==> '1' ==> 단자대 N24V
    BOOL   PASCAL EXPORT CIPset_crc_level(INT16 axis, INT16 level);
	UINT8  PASCAL EXPORT CIPget_crc_level(INT16 axis);
    // 해당 축의 -Emeregency End limit에 대한 Clear출력 사용 유무를 설정/확인한다.    
    BOOL   PASCAL EXPORT CIPset_crc_nelm_mask(INT16 axis, INT16 mask);
    UINT8  PASCAL EXPORT CIPget_crc_nelm_mask(INT16 axis);
    // 해당 축의 -Emeregency End limit의 Active level을 설정/확인한다. set_nend_limit_level과 동일하게 설정한다.    
    BOOL   PASCAL EXPORT CIPset_crc_nelm_level(INT16 axis, INT16 level);
    UINT8  PASCAL EXPORT CIPget_crc_nelm_level(INT16 axis);
    // 해당 축의 +Emeregency End limit에 대한 Clear출력 사용 유무를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_crc_pelm_mask(INT16 axis, INT16 mask);
    UINT8  PASCAL EXPORT CIPget_crc_pelm_mask(INT16 axis);
    // 해당 축의 +Emeregency End limit의 Active level을 설정/확인한다. set_nend_limit_level과 동일하게 설정한다.
    BOOL   PASCAL EXPORT CIPset_crc_pelm_level(INT16 axis, INT16 level);
    UINT8  PASCAL EXPORT CIPget_crc_pelm_level(INT16 axis);    
    // 해당 축의 잔여 펄스 Clear 출력을 입력 값으로 강제 출력/확인한다.
    BOOL   PASCAL EXPORT CIPset_programmed_crc(INT16 axis, INT16 data);
    UINT8  PASCAL EXPORT CIPget_programmed_crc(INT16 axis);

    //-트리거 기능 ======================================================================================================
    // 내부/외부 위치에 대하여 주기/절대 위치에서 설정된 Active level의 Trigger pulse를 발생 시킨다.
    // 트리거 출력 펄스의 Active level을 설정/확인한다.
    // ('0' : 5V 출력(0 V), 24V 터미널 출력(Open); '1'(default) : 5V 출력(5 V), 24V 터미널 출력(N24V).
    BOOL   PASCAL EXPORT CIPset_trigger_level(INT16 axis, UINT8 trigger_level);
    UINT8  PASCAL EXPORT CIPget_trigger_level(INT16 axis);
    // 트리거 기능에 사용할 기준 위치를 선택/확인한다.
		// trigger_sel
		// 0x0 : 내부 위치 Internal(Command)
		// 0x1 : 외부 위치 External(Actual)
    BOOL   PASCAL EXPORT CIPset_trigger_sel(INT16 axis, UINT8 trigger_sel);
    UINT8  PASCAL EXPORT CIPget_trigger_sel(INT16 axis);
    // 트리거 펄스폭을 설정/확인한다.
		// time
		// 00h : 4 msec(칩 출력 Bypass)
		// 01h : 16 mSec
 		// 02h : 32 msec
		// 03h : 64 msec
    BOOL   PASCAL EXPORT CIPset_trigger_time(INT16 axis, UINT8 time);
    UINT8  PASCAL EXPORT CIPget_trigger_time(INT16 axis);
    // 지정 축에 트리거 발생 방식을 설정/확인한다.
		// 0x0 : 트리거 절대 위치에서 트리거 발생, 절대 위치 방식
		// 0x1 : 트리거 위치값을 사용한 주기 트리거 방식
    BOOL   PASCAL EXPORT CIPset_trigger_mode(INT16 axis, UINT8 mode_sel);
    UINT8  PASCAL EXPORT CIPget_trigger_mode(INT16 axis);
    // 지정 축에 트리거 주기 또는 절대 위치 값을 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_trigger_position(INT16 axis, double trigger_position);
    double PASCAL EXPORT CIPget_trigger_position(INT16 axis);
    // 지정 축의 트리거 기능의 사용 여부를 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_trigger_enable(INT16 axis, UINT8 ena_status);
    UINT8  PASCAL EXPORT CIPis_trigger_enabled(INT16 axis);    
    // 지정 축에 트리거 발생시 인터럽트를 발생하도록 설정/확인한다.
    BOOL   PASCAL EXPORT CIPset_trigger_interrupt_enable(INT16 axis, UINT8 ena_int);
    UINT8  PASCAL EXPORT CIPis_trigger_interrupt_enabled(INT16 axis);

    //-칩 내부 보간 기능 설정============================================================================================
    // Module 번호는 CIPget_axisno_2_moduleno로 확인한다.
	// 지정 모듈에 장착된 CAMC-IP 보간 기능을 초기화 한다.
	BOOL	PASCAL EXPORT CIPset_path_move_initialize(INT16 nMouleNo);
	// 지정 모듈에 장착된 CAMC-IP 보간 queue의 사용 가능 유무를 확인한다.
	BOOL	PASCAL EXPORT CIPis_path_move_queue_free(INT16 nMouleNo);
	// 지정 모듈에 장착된 CAMC-IP 보간 queue의 입력 데이타 개수를 확인 한다..
	UINT8	PASCAL EXPORT CIPget_path_move_queue_index(INT16 nMouleNo);
    // 지정 모듈에 장착된 CAMC-IP의 보간 시작 속도를 설정/확인한다.
    void    PASCAL EXPORT CIPset_path_move_startstop_velocity(INT16 nMouleNo, double velocity);
    double  PASCAL EXPORT CIPget_path_move_startstop_velocity(INT16 nMouleNo);
    // 지정 모듈에 장착된 CAMC-IP의 보간 구동 속도를 설정/확인한다.        
    void    PASCAL EXPORT CIPset_path_move_velocity(INT16 nMouleNo, double velocity);
    double  PASCAL EXPORT CIPget_path_move_velocity(INT16 nMouleNo);
    // 지정 모듈에 장착된 CAMC-IP의 보간 구동 가감속률을 설정/확인한다.
    void    PASCAL EXPORT CIPset_path_move_acceleration(INT16 nMouleNo, double acceleration);
    double  PASCAL EXPORT CIPget_path_move_acceleration(INT16 nMouleNo);
    // 지정 모듈에 장착된 CAMC-IP의 보간 구동 가감속 시간을 설정/확인한다.    
    void    PASCAL EXPORT CIPset_path_move_acceltime(INT16 nMouleNo, double acceltime);
    double  PASCAL EXPORT CIPget_path_move_acceltime(INT16 nMouleNo);
    // 지정 모듈에 장착된 CAMC-IP의 보간 구동 중 사요할 속도 프로파일을 설정/확인한다.            
    void    PASCAL EXPORT CIPset_path_move_profile(INT16 nMouleNo, INT16 mode);
    INT16   PASCAL EXPORT CIPget_path_move_profile(INT16 nMouleNo);

    //-칩 내부 보간 드라이버 실행========================================================================================
    //linear_mode(자동 가감속은 단일 보간 패턴에 대하여 사용 가능하다. 수동 가감속을 설정시는 감속 시점을 수동으로 설정해야 한다.)
    // 0 Bit : 수동 가감속 사용여부 (0 : 사용하지 않음, 1: 사용)
    // 1 Bit : 2축 속도 보상 모드 사용 여부 (0 : 사용하지 않음, 1 : 사용)
    // 2 Bit : 자동 가감속 사용여부 (0 : 사용하지 않음, 1 : 사용), 설정시 수동 가감속 사용여부는 무시된다.
    // 3 Bit : Queue 적체 여부(0 : 구동 즉시 실행(현재 Queue에 입력 값이 있다면 마지막에 해당 보간 실행됨, 1 : Queue에 적체)
    //circular_mode(보간 두 축의 unit/pulse(pulse/unit)은 동일 하여야 한다.)
    // 0Bit  : 가감속 사용여부 (0 : 사용하지 않음, 1: 사용)
    // 1Bit  : 2축 속도 보상 모드 사용 여부 (0 : 사용하지 않음, 1 : 사용)
    // 2Bit  : 보간 방향 (0 : CW, 1 : CCW)
    // 3 Bit : Queue 적체 여부(0 : 구동 즉시 실행(현재 Queue에 입력 값이 있다면 마지막에 해당 보간 실행됨, 1 : Queue에 적체)
    //start_** : 구동 시작후 함수를 return한다. "start_*" 가 없으면 이동 완료후 return한다.
    //*r*_*    : 입력된 위치정보는 거리(상대좌표) 이다. "*r_*이 없으면 입력된 위치정보는 (절대좌표)이다.
    //*_ex     : 구동중 가감속도를 가속 또는 감속 시간으로 입력 받는다. "*_ex"가 없다면 가감속률로 입력 받는다.
    //*_m*     : 원호 보간에 사용하는 원의 중심점 좌표를 현재 위치를 기준으로 하는 상대 좌표 값으로 입력 받는다.
    //_circular: x/y축의 종점 정보, x/y의 원의 중심 정보를 설정하고 원보 보간한다.
    //_circular_1: 현재점과 종점에서 입력 반지름으로 원을 그렸을 때 두원의 교점을 보간원원 중심점으로 생각하고 원호 보간을 수행한다.
    //             이때 radius값이 (+) 이면 작게도는 원, (-) 이면 크게 도는 원
    //_circular_2: 지정 모듈의 두 축에 중심점을 입력하고 0~360 범위인 보간 각도를 입력하여 단위 원호 보간구동을 실행한다.
    BOOL    PASCAL EXPORT CIPpath_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    BOOL    PASCAL EXPORT CIPpath_move_linear_ex(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_linear_ex(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
	
    BOOL    PASCAL EXPORT CIPpath_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPpath_move_circular_ex(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_circular_ex(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
	BOOL    PASCAL EXPORT CIPmr_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPmr_path_move_circular_ex(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    
	BOOL    PASCAL EXPORT CIPpath_move_circular_1(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPpath_move_circular_1_ex(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_circular_1(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_circular_1_ex(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);

    BOOL    PASCAL EXPORT CIPpath_move_circular_2(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPpath_move_circular_2_ex(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_circular_2(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPr_path_move_circular_2_ex(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
 
	BOOL    PASCAL EXPORT CIPstart_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    BOOL    PASCAL EXPORT CIPstart_path_move_linear_ex(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_linear_ex(INT16 nModuleNo, double x, double y, UINT8 linear_mode);

    BOOL    PASCAL EXPORT CIPstart_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_path_move_circular_ex(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_circular_ex(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
	BOOL    PASCAL EXPORT CIPstart_mr_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_mr_path_move_circular_ex(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);

    BOOL    PASCAL EXPORT CIPstart_path_move_circular_1(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_path_move_circular_1_ex(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_circular_1(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_circular_1_ex(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
  
	BOOL    PASCAL EXPORT CIPstart_path_move_circular_2(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_path_move_circular_2_ex(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_circular_2(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    BOOL    PASCAL EXPORT CIPstart_r_path_move_circular_2_ex(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);

    // 지정 모듈의 두 축을 이용하여 최후 보간 완료 지점을 기준으로한 상대좌표로 종료점을 입력하여
    // 직선 보간구동을 Queue 에 적체 또는 구동을 시작한다. 구동 속도 파라미터는 현재 설정되어 있는
    // 값을 이용한다. 연속 보간의 하나의 패턴에 대한 입력을 수행한다. 최초 연속 보간은 “conti_” 없는
    // 보간 드라이브 함수를 감속 사용하지 않음으로 하고 구동하면 된다.
    BOOL    PASCAL EXPORT CIPconti_r_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    // 지정 모듈의 두 축에 최후 보간 완료 지점을 기준으로한 상대좌표로 중심점을 입력하고 0~360
    // 범위인 보간 각도를 입력하여 단위 원호 보간구동을 Queue 에 적체 또는 구동을 시작한다. 구동
    // 속도 파라미터는 현재 설정되어 있는 값을 이용한다. 연속 보간의 하나의 패턴에 대한 입력을
    // 수행한다. 최초 연속 보간은 “conti_” 없는 보간 드라이브 함수를 감속 사용하지 않음으로 하고
    // 구동하면 된다.    
    BOOL    PASCAL EXPORT CIPconti_r_path_move_circular_2(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    // 지정 모듈의 두 축에 최후 보간 완료 지점을 기준으로한 상대 좌표로 종료점을 입력하고, 부호가
    // 있는 반지름 값을 입력하여 단위 원호 보간구동을 Queue에 적체 또는 구동을 시작한다. 구동 속도
    // 파라미터는 현재 설정되어 있는 값을 이용한다. 연속 보간의 하나의 패턴에 대한 입력을 수행한다.
    // 최초 연속 보간은 “conti_” 없는 보간 드라이브 함수를 감속 사용하지 않음으로 하고 구동하면
    // 된다.
    BOOL    PASCAL EXPORT CIPconti_r_path_move_circular_1(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    // 지정 모듈의 두 축에 최후 보간 완료 지점을 기준으로한 상대 좌표로 종료점 및 원점을 입력하여
    // 원호 보간구동을 Queue 에 적체 또는 구동을 시작한다. 구동 속도 파라미터는 현재 설정되어 있는
    // 값을 이용한다. 연속 보간의 하나의 패턴에 대한 입력을 수행한다. 최초 연속 보간은 “conti_” 없는
    // 보간 드라이브 함수를 감속 사용하지 않음으로 하고 구동하면 된다.
    BOOL    PASCAL EXPORT CIPconti_r_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    // 지정 모듈의 두 축을 이용하여 최후 보간 완료 지점을 기준으로한 상대좌표로 종료점을 입력하여
    // 직선 보간구동을 Queue 에 적체 또는 구동을 시작한다. 구동 속도 파라미터는 현재 설정되어 있는
    // 값을 이용한다. 연속 보간의 하나의 패턴에 대한 입력을 수행한다. 최초 연속 보간은 “conti_” 없는
    // 보간 드라이브 함수를 감속 사용하지 않음으로 하고 구동하면 된다. 연속 보간시 최후 패턴에
    // 대하여 사용하면 가속시 사용된 펄스 개수를 감안한 감속 지점이 자동으로 추출되어 실행된다.
    BOOL    PASCAL EXPORT CIPcontiend_r_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);

    //-==================================================================================================================
    // Script/Caption 설정을 위한 함수
    BOOL   PASCAL EXPORT CIPSetScriptCaption(INT16 axis, INT16 sc, IPEVENT event1, IPEVENT event2, UINT8 event_logic, IPCOMMAND cmd, UINT32 data);
    // sc(입력) ==========================================================================
    //        SCRIPT_REG1                    1    // 스크립트 레지스터-1
    //        SCRIPT_REG2                    2    // 스크립트 레지스터-2
    //        SCRIPT_REG3                    3    // 스크립트 레지스터-3
    //        SCRIPT_REG_QUEUE            4    // 스크립트 레지스터-Queue
    //        CAPTION_REG1                11    // 갈무리 레지스터-1
    //        CAPTION_REG2                12    // 갈무리 레지스터-2
    //        CAPTION_REG3                13    // 갈무리 레지스터-3
    //        CAPTION_REG_QUEUE            14    // 갈무리 레지스터-Queue
    // event1(입력)  ====================================================================
    //        모니터링 할 event 첫번째. IPEVENT에 define된 것을 사용.
    // event2(입력)  ====================================================================
    //        모니터링 할 event 두번째. IPEVENT에 define된 것을 사용.
    // event_logic(입력) ==================================================================
    //        7 bit : 0(One time execution), 1(Always execution)
    //        6 bit : sc에 따라서 다음과 같은 차이점이 있음.
    //                sc = SCRIPT_REG1, SCRIPT_REG2, SCRIPT_REG3 일 때. Script 동작시 사용할 데이타 입력 설정.
    //                    0(data 사용), 1(ALU 출력 결과를 사용) 
    //                sc = SCRIPT_REG_QUEUE 일 때. Script 동작시 인터럽트 출력 유무. 해당 인터럽트 mask가 enable 되어 있어야 함.
    //                    0(인터럽트 발생하지 않음), 1(해당 script 수행시 인터럽트 발생) 
    //                sc = CAPTION_REG1, CAPTION_REG2, CAPTION_REG3 일 때. Don't care.
    //                sc = CAPTION_REG_QUEUE. Caption 동작시 인터럽트 출력 유무. 해당 인터럽트 mask가 enable되어 있어야 함.
    //                    0(인터럽트 발생하지 않음), 1(해당 caption 수행시 인터럽트 발생) 
    //        5 ~ 4bit : "00" : Don't execute command 
    //                   "01" : Execute command in X
    //                   "10" : Execute command in Y
    //                   "11" : Execute command in X,Y(Caption:Don't execution)
    //        3 bit : Second event source axis selection(0 : X axis, 1 : Y axis)
    //        2 bit : First event source axis selection(0 : X axis, 1 : Y axis)  
    //        1~0 bit :   "00" : Use first event source only
    //                    "01" : OR operation
    //                    "11" : AND operation
    //                    "11" : XOR operation
    // cmd(입력) ========================================================================
    //        이벤트 발생시 동작 시킬 명령어. IPCOMMAND에 define된 것을 사용.
    
    //-==================================================================================================================
    // Inter Axis, Inter Module sync setting(UIO11을 이용한 모듈간, 축간 동기 기능 구현을 위한 설정)
    // 모듈내 범용 IO를 사용한 Sync 기능을 사용하기 위한 설정, SMC-2V03 version 2.0 미만에서만 지원.
    BOOL    PASCAL EXPORT CIPset_sync_mode(INT16 axis, UINT8 mode);
    UINT8   PASCAL EXPORT CIPget_sync_mode(INT16 axis);
    BOOL    PASCAL EXPORT CIPset_sync_mode_master(INT16 axis);
    BOOL    PASCAL EXPORT CIPset_sync_mode_slave(INT16 axis);

    // 같은 베이스 보드내 모듈간 Sync를 기능을 사용하기위한 설정.
    // 모듈내 범용 IO를 사용한 Sync 기능을 사용하기 위한 설정, SMC-2V03 version 2.0 미만에서만 지원.
    BOOL    PASCAL EXPORT CIPset_sync_all_mode(INT16 nModuleNo, UINT8 mode);
    UINT8   PASCAL EXPORT CIPget_sync_all_mode(INT16 nModuleNo);
    BOOL    PASCAL EXPORT CIPset_sync_all_mode_master(INT16 nModuleNo);
    BOOL    PASCAL EXPORT CIPset_sync_all_mode_slave(INT16 nModuleNo);
    BOOL    PASCAL EXPORT CIPset_sync_all_mode_disable(INT16 nModuleNo);
 
    // 에러코드 읽기 함수군 =============================================================================================
	// 마지막 에러코드를 읽는다.
	INT16 PASCAL EXPORT CIPget_error_code();
	// 에러코드의 원인을 문자로 반환한다.
	char* PASCAL EXPORT CIPget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_CAMC_IP_H__
