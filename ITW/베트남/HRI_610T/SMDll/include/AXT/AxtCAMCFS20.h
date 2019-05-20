#ifndef __AXT_CAMC_FS20_H__
#define __AXT_CAMC_FS20_H__

#include "AxtLIBDef.h"
#include "CAMCFSDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTCAMCFS Library - CAMC-FS 2.0�̻� Motion module
	������ǰ
		SMC-1V02 - CAMC-FS Ver2.0 �̻� 1��
		SMC-2V02 - CAMC-FS Ver2.0 �̻� 2��
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

    // ���� �ʱ�ȭ �Լ���        -======================================================================================
	// CAMC-FS�� ������ ���(SMC-1V02, SMC-2V02)�� �˻��Ͽ� �ʱ�ȭ�Ѵ�. CAMC-FS 2.0�̻� �����Ѵ�
        // reset	: 1(TRUE) = ��������(ī���� ��)�� �ʱ�ȭ�Ѵ�
        //  reset(TRUE)�϶� �ʱ� ������.
        //  1) ���ͷ�Ʈ ������� ����.
        //  2) �������� ��� ������� ����.
        //  3) �˶����� ��� ������� ����.
        //  4) ������ ����Ʈ ��� ��� ��.
        //  5) �������� ����Ʈ ��� ��� ��.            
        //  6) �޽� ��� ��� : OneLowHighLow(Pulse : Active LOW, Direction : CW{High};CCW{LOW}).
        //  7) �˻� ��ȣ : +������ ����Ʈ ��ȣ �ϰ� ����.
        //  8) �Է� ���ڴ� ���� : 2��, 4 ü��.
        //  9) �˶�, ��������, +-���� ���� ����Ʈ, +-������ ����Ʈ Active level : HIGH
        // 10) ����/�ܺ� ī���� : 0.		
    BOOL   PASCAL EXPORT InitializeCAMCFS20(BOOL reset);
	// CAMC-FS20 ����� ����� ���������� Ȯ���Ѵ�
		// ���ϰ� :  1(TRUE) = CAMC-FS20 ����� ��� �����ϴ�
	BOOL   PASCAL EXPORT CFS20IsInitialized(void);
	// CAMC-FS20�� ������ ����� ����� �����Ѵ�
	void   PASCAL EXPORT CFS20StopService();

    /// ���� ���� ���� �Լ���        -===================================================================================
	// ������ �ּҿ� ������ ���̽������� ��ȣ�� �����Ѵ�. ������ -1�� �����Ѵ�
	INT16	PASCAL EXPORT CFS20get_boardno(UINT32 address);
	// ���̽������� ������ �����Ѵ�
	INT16	PASCAL EXPORT CFS20get_numof_boards();
	// ������ ���̽����忡 ������ ���� ������ �����Ѵ�
	INT16	PASCAL EXPORT CFS20get_numof_axes(INT16 nBoardNo);
	// ���� ������ �����Ѵ�
	INT16	PASCAL EXPORT CFS20get_total_numof_axis();
	// ������ ���̽������ȣ�� ����ȣ�� �ش��ϴ� ���ȣ�� �����Ѵ�
	INT16	PASCAL EXPORT CFS20get_axisno(INT16 nBoardNo, INT16 nModuleNo);
	// ������ ���� ������ �����Ѵ�
    // nBoardNo : �ش� ���� ������ ���̽������� ��ȣ.
    // nModuleNo: �ش� ���� ������ ����� ���̽� ��峻 ��� ��ġ(0~3)
    // bModuleID: �ش� ���� ������ ����� ID : SMC-2V02(0x02)
    // nAxisPos : �ش� ���� ������ ����� ù��°���� �ι�° ������ ����.(0 : ù��°, 1 : �ι�°)
	BOOL	PASCAL EXPORT CFS20get_axis_info(INT16 nAxisNo, INT16 *nBoardNo, INT16 *nModuleNo, UINT8 *bModuleID, INT16 *nAxisPos);

    // ���� ���� �Լ���        -========================================================================================
    // ���� ���� �ʱⰪ�� ������ ���Ͽ��� �о �����Ѵ�
    // Loading parameters.
	//	1) 1Pulse�� �̵��Ÿ�(Move Unit / Pulse)
	//	2) �ִ� �̵� �ӵ�, ����/���� �ӵ�
	//	3) ���ڴ� �Է¹��, �޽� ��¹�� 
	//	4) +������ ����Ʈ����, -������ ����Ʈ����, ������ ����Ʈ �������
    //  5) +�������� ����Ʈ����,-�������� ����Ʈ����, �������� ����Ʈ �������
    //  6) �˶�����, �˶� �������
    //  7) ��������(��ġ�����Ϸ� ��ȣ)����, �������� �������
    //  8) ������� �������
    //  9) ���ڴ� �Է¹��2 ������
    // 10) ����/�ܺ� ī���� : 0. 	
	BOOL	PASCAL EXPORT CFS20load_parameter(INT16 axis, char *nfilename);
    // ���� ���� �ʱⰪ�� ������ ���Ͽ� �����Ѵ�.
    // Saving parameters.
	//	1) 1Pulse�� �̵��Ÿ�(Move Unit / Pulse)
	//	2) �ִ� �̵� �ӵ�, ����/���� �ӵ�
	//	3) ���ڴ� �Է¹��, �޽� ��¹�� 
	//	4) +������ ����Ʈ����, -������ ����Ʈ����, ������ ����Ʈ �������
    //  5) +�������� ����Ʈ����,-�������� ����Ʈ����, �������� ����Ʈ �������
    //  6) �˶�����, �˶� �������
    //  7) ��������(��ġ�����Ϸ� ��ȣ)����, �������� �������
    //  8) ������� �������
    //  9) ���ڴ� �Է¹��2 ������
	BOOL    PASCAL EXPORT CFS20save_parameter(INT16 axis, char *nfilename); 
	// ��� ���� �ʱⰪ�� ������ ���Ͽ��� �о �����Ѵ�
	BOOL    PASCAL EXPORT CFS20load_parameter_all(char *nfilename);
	// ��� ���� �ʱⰪ�� ������ ���Ͽ� �����Ѵ�
	BOOL    PASCAL EXPORT CFS20save_parameter_all(char *nfilename); 
	
	// ���ͷ�Ʈ �Լ���   -================================================================================================
	//(���ͷ�Ʈ�� ����ϱ� ���ؼ��� 
	//Window message & procedure
	//    hWnd    : ������ �ڵ�, ������ �޼����� ������ ���. ������� ������ NULL�� �Է�.
	//    wMsg    : ������ �ڵ��� �޼���, ������� �ʰų� ����Ʈ���� ����Ϸ��� 0�� �Է�.
	//    proc    : ���ͷ�Ʈ �߻��� ȣ��� �Լ��� ������, ������� ������ NULL�� �Է�.
	void   PASCAL EXPORT CFS20SetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_CAMCFS_INTERRUPT_PROC proc);	
	//-===============================================================================
    // ReadInterruptFlag���� ������ ���� flag������ �о� ���� �Լ�(���ͷ�Ʈ service routine���� ���ͷ��� �߻� ������ �Ǻ��Ѵ�.)
    // ���ϰ�: ���ͷ�Ʈ�� �߻� �Ͽ����� �߻��ϴ� ���ͷ�Ʈ flag register(CAMC-FS20 �� INTFLAG ����.)
	UINT32 PASCAL EXPORT CFS20read_interrupt_flag(INT16 axis);

    // ���� ���� �ʱ�ȭ �Լ���        -==================================================================================
	// ����Ŭ�� ����( ��⿡ ������ Oscillator�� ����� ��쿡�� ����)
	void	PASCAL EXPORT CFS20KeSetMainClk(INT32 nMainClk);
	// Drive mode 1�� ����/Ȯ���Ѵ�.
		// decelstartpoint : �����Ÿ� ���� ��� ����� ���� ��ġ ���� ��� ����(0 : �ڵ� ������, 1 : ���� ������)
		// pulseoutmethod : ��� �޽� ��� ����(typedef : PULSE_OUTPUT)
		// detecsignal : ��ȣ �˻�-1/2 ���� ��� ����� �˻� �� ��ȣ ����(typedef : DETECT_DESTINATION_SIGNAL)
	void	PASCAL EXPORT CFS20set_drive_mode1(INT16 axis, 
				UINT8 decelstartpoint,
				UINT8 pulseoutmethod,
				UINT8 detectsignal);
	UINT8	PASCAL EXPORT CFS20get_drive_mode1(INT16 axis);
	// Drive mode 2�� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_drive_mode2(INT16 axis, 
				UINT8 encmethod,
				UINT8 inpactivelevel,
				UINT8 alarmactivelevel,
				UINT8 nslmactivelevel,
				UINT8 pslmactivelevel,
				UINT8 nelmactivelevel,
				UINT8 pelmactivelevel);
	UINT16	PASCAL EXPORT CFS20get_drive_mode2(INT16 axis);
    // Unit/Pulse ����/Ȯ���Ѵ�.
		// Unit/Pulse : 1 pulse�� ���� system�� �̵��Ÿ��� ���ϸ�, �̶� Unit�� ������ ����ڰ� ���Ƿ� ������ �� �ִ�.
		// Ex) Ball screw pitch : 10mm, ���� 1ȸ���� �޽��� : 10000 ==> Unit�� mm�� ������ ��� : Unit/Pulse = 10/10000.
		// ���� unitperpulse�� 0.001�� �Է��ϸ� ��� ��������� mm�� ������. 
		// Ex) Linear motor�� ���ش��� 1 pulse�� 2 uM. ==> Unit�� mm�� ������ ��� : Unit/Pulse = 0.002/1.
	void	PASCAL EXPORT CFS20set_moveunit_perpulse(INT16 axis, double unitperpulse);
	double	PASCAL EXPORT CFS20get_moveunit_perpulse(INT16 axis);
	// Unit/Pulse�� ��������� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_movepulse_perunit(INT16 axis, double pulseperunit);
	double	PASCAL EXPORT CFS20get_movepulse_perunit(INT16 axis);
    // ���� �ӵ� ����/Ȯ���Ѵ�.(Unit/Sec)
	void	PASCAL EXPORT CFS20set_startstop_speed(INT16 axis, double velocity);
	double	PASCAL EXPORT CFS20get_startstop_speed(INT16 axis);
	// �ְ� �ӵ� ���� Unit/Sec. ���� system�� �ְ� �ӵ��� �����Ѵ�.
		// Unit/Pulse ������ ���ۼӵ� ���� ���Ŀ� �����Ѵ�.
		// ������ �ְ� �ӵ� �̻����δ� ������ �Ҽ� �����Ƿ� �����Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_max_speed(INT16 axis, double max_velocity);
	double	PASCAL EXPORT CFS20get_max_speed(INT16 axis);
    // SW�� ����� ���� ����/Ȯ���Ѵ�. �̰����� S-Curve ������ percentage�� ���� �����ϴ�.
	void	PASCAL EXPORT CFS20set_s_rate(INT16 axis, double a_percent, double b_percent);
	void	PASCAL EXPORT CFS20get_s_rate(INT16 axis, double *a_percent, double *b_percent);
	// ���� ������ ��忡�� �ܷ� �޽��� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_slowdown_rear_pulse(INT16 axis, UINT32 ulData);
	UINT32	PASCAL EXPORT CFS20get_slowdown_rear_pulse(INT16 axis);
	// ���� ���� ���� ���� ������ ���� ����� ����/Ȯ���Ѵ�.
		// 0x0 : �ڵ� ������.
		// 0x1 : ���� ������.
	BOOL    PASCAL EXPORT CFS20set_decel_point(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT CFS20get_decel_point(INT16 axis);

    // ���� ���� Ȯ�� �Լ���        -=====================================================================================
    // ���� ���� �޽� ����������� Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20in_motion(INT16 axis);
    // ���� ���� �޽� ����� ����ƴ��� Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20motion_done(INT16 axis);
    // ���� ���� �������� ���� ��µ� �޽� ī���� ���� Ȯ���Ѵ�. (Pulse)
	INT32	PASCAL EXPORT CFS20get_drive_pulse_counts(INT16 axis);
    // ���� ���� DriveStatus �������͸� Ȯ���Ѵ�.
	UINT16	PASCAL EXPORT CFS20get_drive_status(INT16 axis);
    // ���� ���� EndStatus �������͸� Ȯ���Ѵ�.
		// End Status Bit�� �ǹ�
		// 14bit : Limit(PELM, NELM, PSLM, NSLM, Soft)�� ���� ����
		// 13bit : Limit ���� ������ ���� ����
		// 12bit : Sensor positioning drive����
		// 11bit : Preset pulse drive�� ���� ����(������ ��ġ/�Ÿ���ŭ �����̴� �Լ���)
		// 10bit : ��ȣ ���⿡ ���� ����(Signal Search-1/2 drive����)
		// 9 bit : ���� ���⿡ ���� ����
		// 8 bit : Ż�� ������ ���� ����
		// 7 bit : ����Ÿ ���� ������ ���� ����
		// 6 bit : ALARM ��ȣ �Է¿� ���� ����
		// 5 bit : ������ ��ɿ� ���� ����
		// 4 bit : �������� ��ɿ� ���� ����
		// 3 bit : ������ ��ȣ �Է¿� ���� ���� (EMG Button)
		// 2 bit : �������� ��ȣ �Է¿� ���� ����
		// 1 bit : Limit(PELM, NELM, Soft) �������� ���� ����
		// 0 bit : Limit(PSLM, NSLM, Soft) ���������� ���� ����
	UINT16	PASCAL EXPORT CFS20get_end_status(INT16 axis);
    // ���� ���� Mechanical �������͸� Ȯ���Ѵ�.
		// Mechanical Signal Bit�� �ǹ�
		// 12bit : ESTOP ��ȣ �Է� Level
		// 11bit : SSTOP ��ȣ �Է� Level
		// 10bit : MARK ��ȣ �Է� Level
		// 9 bit : EXPP(MPG) ��ȣ �Է� Level
		// 8 bit : EXMP(MPG) ��ȣ �Է� Level
		// 7 bit : Encoder Up��ȣ �Է� Level(A�� ��ȣ)
		// 6 bit : Encoder Down��ȣ �Է� Level(B�� ��ȣ)
		// 5 bit : INPOSITION ��ȣ Active ����
		// 4 bit : ALARM ��ȣ Active ����
		// 3 bit : -Limit �������� ��ȣ Active ���� (Ver3.0���� ����������)
		// 2 bit : +Limit �������� ��ȣ Active ���� (Ver3.0���� ����������)
		// 1 bit : -Limit ������ ��ȣ Active ����
		// 0 bit : +Limit ������ ��ȣ Active ����
	UINT16	PASCAL EXPORT CFS20get_mechanical_signal(INT16 axis);
    // ���� ����  ���� �ӵ��� �о� �´�.(Unit/Sec)
	double	PASCAL EXPORT CFS20get_velocity(INT16 axis);
    // ���� ���� Command position�� Actual position�� ���� Ȯ���Ѵ�.
	double	PASCAL EXPORT CFS20get_error(INT16 axis);
    // ���� ���� ���� ����̺��� �̵� �Ÿ��� Ȯ�� �Ѵ�. (Unit)
	double	PASCAL EXPORT CFS20get_drivedistance(INT16 axis);

    // Encoder �Է� ��� ���� �Լ���        -=============================================================================
    // ���� ���� Encoder �Է� ����� ����/Ȯ���Ѵ�.
		// method : typedef(EXTERNAL_COUNTER_INPUT)
		// UpDownMode = 0x0    // Up/Down
		// Sqr1Mode   = 0x1    // 1ü��
		// Sqr2Mode   = 0x2    // 2ü��
		// Sqr4Mode   = 0x3    // 4ü��
	BOOL    PASCAL EXPORT CFS20set_enc_input_method(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT CFS20get_enc_input_method(INT16 axis);
    // ���� ���� �ܺ� ��ġ counter clear�� ����� ����/Ȯ���Ѵ�.
		// method : CAMC-FS chip �޴��� EXTCNTCLR �������� ����.
	BOOL	PASCAL EXPORT CFS20set_enc2_input_method(INT16 axis, UINT8 method);
	UINT8   PASCAL EXPORT CFS20get_enc2_input_method(INT16 axis);
	// ���� ���� �ܺ� ��ġ counter�� count ����� ����/Ȯ���Ѵ�.
		// reverse :
		// TRUE  : �Է� ���ڴ��� �ݴ�Ǵ� �������� count�Ѵ�.
		// FALSE : �Է� ���ڴ��� ���� ���������� count�Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_enc_reverse(INT16 axis, UINT8 reverse);
	BOOL    PASCAL EXPORT CFS20get_enc_reverse(INT16 axis);

    // �޽� ��� ��� �Լ���        -=====================================================================================
    // �޽� ��� ����� ����/Ȯ���Ѵ�.
		// method : ��� �޽� ��� ����(typedef : PULSE_OUTPUT)
		// OneHighLowHigh   = 0x0, 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
		// OneHighHighLow   = 0x1, 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
		// OneLowLowHigh    = 0x2, 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
		// OneLowHighLow    = 0x3, 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
		// TwoCcwCwHigh     = 0x4, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High 
		// TwoCcwCwLow      = 0x5, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low 
		// TwoCwCcwHigh     = 0x6, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
		// TwoCwCcwLow      = 0x7, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low
	BOOL    PASCAL EXPORT CFS20set_pulse_out_method(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT CFS20get_pulse_out_method(INT16 axis);

    // ��ġ Ȯ�� �� ��ġ �� ���� �Լ��� -===============================================================================
    // �ܺ� ��ġ ���� �����Ѵ�. ������ ���¿��� �ܺ� ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
	void	PASCAL EXPORT CFS20set_actual_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_actual_position(INT16 axis);
    // ���� ��ġ ���� �����Ѵ�. ������ ���¿��� ���� ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
	void	PASCAL EXPORT CFS20set_command_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_command_position(INT16 axis);

    // ���� ����̹� ��� ��ȣ ���� �Լ���-===============================================================================
   	// ���� Enable��� ��ȣ�� Active Level�� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_servo_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_servo_level(INT16 axis);	
    // ���� Enable(On) / Disable(Off)�� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_servo_enable(INT16 axis, UINT8 state);
	UINT8	PASCAL EXPORT CFS20get_servo_enable(INT16 axis);
	
    // ���� ����̹� �Է� ��ȣ ���� �Լ���-===============================================================================
	// ���� ��ġ�����Ϸ�(inposition)�Է� ��ȣ�� ��������� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_inposition_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT CFS20get_inposition_enable(INT16 axis);
   	// ���� ��ġ�����Ϸ�(inposition)�Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_inposition_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_inposition_level(INT16 axis);
	UINT8	PASCAL EXPORT CFS20get_inposition_switch(INT16 axis);
	BOOL    PASCAL EXPORT CFS20in_position(INT16 axis);
	// ���� �˶� �Է½�ȣ ����� ��������� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_alarm_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT CFS20get_alarm_enable(INT16 axis);
	// ���� �˶� �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_alarm_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_alarm_level(INT16 axis);
	UINT8	PASCAL EXPORT CFS20get_alarm_switch(INT16 axis);

    // ����Ʈ ��ȣ ���� �Լ���-===========================================================================================
    // ������ ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_end_limit_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT CFS20get_end_limit_enable(INT16 axis);
	// -������ ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_nend_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_nend_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT CFS20get_nend_limit_switch(INT16 axis);
	// +������ ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_pend_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_pend_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT CFS20get_pend_limit_switch(INT16 axis);
    // �������� ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_slow_limit_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT CFS20get_slow_limit_enable(INT16 axis);
	// -�������� ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_nslow_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_nslow_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT CFS20get_nslow_limit_switch(INT16 axis);
	// +�������� ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_pslow_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_pslow_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT CFS20get_pslow_limit_switch(INT16 axis);
	// -LIMIT ���� ������ ��/�������� ���θ� ����/Ȯ���Ѵ�. (Ver 3.0���� ����)
		// stop:
		// 0 : ������, 1 : ��������
	BOOL	PASCAL EXPORT CFS20set_nlimit_sel(INT16 axis, UINT8 stop);
	UINT8	PASCAL EXPORT CFS20get_nlimit_sel(INT16 axis);
	// +LIMIT ���� ������ ��/�������� ���θ� ����/Ȯ���Ѵ�. (Ver 3.0���� ����)	
		// stop:
		// 0 : ������, 1 : ��������
	BOOL	PASCAL EXPORT CFS20set_plimit_sel(INT16 axis, UINT8 stop);
	UINT8	PASCAL EXPORT CFS20get_plimit_sel(INT16 axis);

    // ����Ʈ���� ����Ʈ ���� �Լ���-=====================================================================================
    // ����Ʈ���� ����Ʈ ��������� ����/Ȯ���Ѵ�.
	void    PASCAL EXPORT CFS20set_soft_limit_enable(INT16 axis, UINT8 use);
	UINT8   PASCAL EXPORT CFS20get_soft_limit_enable(INT16 axis);
    // ����Ʈ���� ����Ʈ ���� ������ġ������ ����/Ȯ���Ѵ�.
		// sel :
		// 0x0 : ������ġ�� ���Ͽ� ����Ʈ���� ����Ʈ ��� ����.
		// 0x1 : �ܺ���ġ�� ���Ͽ� ����Ʈ���� ����Ʈ ��� ����.
	void    PASCAL EXPORT CFS20set_soft_limit_sel(INT16 axis, UINT8 sel);
	UINT8   PASCAL EXPORT CFS20get_soft_limit_sel(INT16 axis);
    // ����Ʈ���� ����Ʈ �߻��� ���� ��带 ����/Ȯ���Ѵ�.
		// mode :
		// 0x0 : ����Ʈ���� ����Ʈ ��ġ���� ������ �Ѵ�.
		// 0x1 : ����Ʈ���� ����Ʈ ��ġ���� �������� �Ѵ�.
	void    PASCAL EXPORT CFS20set_soft_limit_stopmode(INT16 axis, UINT8 mode);
	UINT8   PASCAL EXPORT CFS20get_soft_limit_stopmode(INT16 axis);
    // ����Ʈ���� ����Ʈ -��ġ�� ����/Ȯ���Ѵ�.(position = Unit)
	void	PASCAL EXPORT CFS20set_soft_nlimit_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_soft_nlimit_position(INT16 axis);
    // ����Ʈ���� ����Ʈ +��ġ�� ����/Ȯ�� �Ѵ�.(position = Unit)
	void	PASCAL EXPORT CFS20set_soft_plimit_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_soft_plimit_position(INT16 axis);

    // ������� ��ȣ-=====================================================================================================
    // ESTOP, SSTOP ��ȣ ��������� ����/Ȯ���Ѵ�.(Emergency stop, Slow-Down stop)
	BOOL    PASCAL EXPORT CFS20set_emg_signal_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT CFS20get_emg_signal_enable(INT16 axis);
	// ��������� ��/�������� ���θ� ����/Ȯ���Ѵ�.
		// stop:
		// 0 : ������, 1 : ��������
	BOOL	PASCAL EXPORT CFS20set_stop_sel(INT16 axis, UINT8 stop);
	UINT8	PASCAL EXPORT CFS20get_stop_sel(INT16 axis);

	// ���� ���� �Ÿ� ����-===============================================================================================
	// start_** : ���� �࿡�� ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�(Blocking).
	// *r*_*    : ���� �࿡�� �Էµ� �Ÿ���ŭ(�����ǥ)�� �̵��Ѵ�. "*r_*�� ������ �Էµ� ��ġ(������ǥ)�� �̵��Ѵ�.
	// *s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
	// *a*_*    : ������ �ӵ� �����ӵ��� ���Ī���� ����Ѵ�. ���ӷ� �Ǵ� ���� �ð���  ���ӷ� �Ǵ� ���� �ð��� ���� �Է¹޴´�.
	// *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.
	// �Է� ����: velocity(Unit/Sec), acceleration/deceleration(Unit/Sec^2), acceltime/deceltime(Sec), position(Unit)

	// ��Ī �����޽�(Pulse Drive), ��ٸ��� ���� �Լ�, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT16	PASCAL EXPORT CFS20move(INT16 axis, double position, double velocity, double acceleration);
	UINT16	PASCAL EXPORT CFS20move_ex(INT16 axis, double position, double velocity, double acceltime);
	UINT16	PASCAL EXPORT CFS20r_move(INT16 axis, double distance, double velocity, double acceleration);
	UINT16	PASCAL EXPORT CFS20r_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
	BOOL    PASCAL EXPORT CFS20start_move(INT16 axis, double position, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20start_move_ex(INT16 axis, double position, double velocity, double acceltime);
	BOOL    PASCAL EXPORT CFS20start_r_move(INT16 axis, double distance, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20start_r_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// ���Ī �����޽�(Pulse Drive), ��ٸ��� ���� �Լ�, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT16	PASCAL EXPORT CFS20a_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
	UINT16	PASCAL EXPORT CFS20a_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
	UINT16	PASCAL EXPORT CFS20ra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	UINT16	PASCAL EXPORT CFS20ra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
	BOOL    PASCAL EXPORT CFS20start_a_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
	BOOL    PASCAL EXPORT CFS20start_a_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
	BOOL    PASCAL EXPORT CFS20start_ra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	BOOL    PASCAL EXPORT CFS20start_ra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// ��Ī �����޽�(Pulse Drive), S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT16	PASCAL EXPORT CFS20s_move(INT16 axis, double position, double velocity, double acceleration);
	UINT16	PASCAL EXPORT CFS20s_move_ex(INT16 axis, double position, double velocity, double acceltime);
	UINT16	PASCAL EXPORT CFS20rs_move(INT16 axis, double distance, double velocity, double acceleration);
	UINT16	PASCAL EXPORT CFS20rs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
	BOOL    PASCAL EXPORT CFS20start_s_move(INT16 axis, double position, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20start_s_move_ex(INT16 axis, double position, double velocity, double acceltime);
	BOOL    PASCAL EXPORT CFS20start_rs_move(INT16 axis, double distance, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20start_rs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// ���Ī �����޽�(Pulse Drive), S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT16	PASCAL EXPORT CFS20as_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
	UINT16	PASCAL EXPORT CFS20as_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
	UINT16	PASCAL EXPORT CFS20ras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	UINT16	PASCAL EXPORT CFS20ras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�), jerk���(���� : �ۼ�Ʈ) ���������� S�� �̵�����.
	BOOL    PASCAL EXPORT CFS20start_as_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
	BOOL	PASCAL EXPORT CFS20start_as_move2(INT16 axis, double position, double velocity, double acceleration, double deceleration, double jerk);
	BOOL    PASCAL EXPORT CFS20start_as_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
	BOOL    PASCAL EXPORT CFS20start_ras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
	BOOL    PASCAL EXPORT CFS20start_ras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);

	// ��Ī ���� �޽�(Pulse Drive), S���� ����, �����ǥ, ������,
	// Non Blocking (�������� ��� ���õ�), ���� ��ġ�� �������� over_distance���� over_velocity�� �ӵ��� ���� �Ѵ�.
	BOOL   PASCAL EXPORT CFS20start_rs_move_override(INT16 axis, double distance, double velocity, double acceleration, double over_distance, double over_velocity, BOOL Target);

    // ���� ���� ����-====================================================================================================
    // ���� �����ӵ� �� �ӵ��� ���� ������ �߻����� ������ ���������� �����Ѵ�.
    // *s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    // *a*_*    : ������ �ӵ� �����ӵ��� ���Ī���� ����Ѵ�. ���ӷ� �Ǵ� ���� �ð���  ���ӷ� �Ǵ� ���� �ð��� ���� �Է¹޴´�.
    // *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.

	// ���ӵ� ��ٸ��� ���� �Լ���, ������/���ӽð�(_ex)(�ð�����:Sec) - �������� ��쿡�� �ӵ��������̵�
	// ��Ī ������ �����Լ�
	BOOL    PASCAL EXPORT CFS20v_move(INT16 axis, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20v_move_ex(INT16 axis, double velocity, double acceltime);
	// ���Ī ������ �����Լ�
	BOOL    PASCAL EXPORT CFS20v_a_move(INT16 axis, double velocity, double acceleration, double deceleration);
	BOOL    PASCAL EXPORT CFS20v_a_move_ex(INT16 axis, double velocity, double acceltime, double deceltime);
	// ���ӵ� S���� ���� �Լ���, ������/���ӽð�(_ex)(�ð�����:Sec) - �������� ��쿡�� �ӵ��������̵�
	// ��Ī ������ �����Լ�
	BOOL    PASCAL EXPORT CFS20v_s_move(INT16 axis, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20v_s_move_ex(INT16 axis, double velocity, double acceltime);
	// ���Ī ������ �����Լ�
	BOOL    PASCAL EXPORT CFS20v_as_move(INT16 axis, double velocity, double acceleration, double deceleration);
	BOOL    PASCAL EXPORT CFS20v_as_move_ex(INT16 axis, double velocity, double acceltime, double deceltime);

    // ��ȣ ���� ����-====================================================================================================
    // ���� ��ȣ�� ����/���� ������ �˻��Ͽ� ������ �Ǵ� ���������� �� �� �ִ�.
    // detect_signal : �˻� ��ȣ ����(typedef : DETECT_DESTINATION_SIGNAL)
    // PElmNegativeEdge    = 0x0,        // +Elm(End limit) �ϰ� edge
    // NElmNegativeEdge    = 0x1,        // -Elm(End limit) �ϰ� edge
    // PSlmNegativeEdge    = 0x2,        // +Slm(Slowdown limit) �ϰ� edge
    // NSlmNegativeEdge    = 0x3,        // -Slm(Slowdown limit) �ϰ� edge
    // In0DownEdge         = 0x4,        // IN0(ORG) �ϰ� edge
    // In1DownEdge         = 0x5,        // IN1(Z��) �ϰ� edge
    // In2DownEdge         = 0x6,        // IN2(����) �ϰ� edge
    // In3DownEdge         = 0x7,        // IN3(����) �ϰ� edge
    // PElmPositiveEdge    = 0x8,        // +Elm(End limit) ��� edge
    // NElmPositiveEdge    = 0x9,        // -Elm(End limit) ��� edge
    // PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) ��� edge
    // NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) ��� edge
    // In0UpEdge           = 0xc,        // IN0(ORG) ��� edge
    // In1UpEdge           = 0xd,        // IN1(Z��) ��� edge
    // In2UpEdge           = 0xe,        // IN2(����) ��� edge
    // In3UpEdge           = 0xf         // IN3(����) ��� edge
    // Signal Search1 : ���� ������ �Է� �ӵ����� �����Ͽ�, ��ȣ ������ ���� ����.
    // Signal Search2 : ���� ������ ���Ӿ��� �Է� �ӵ��� �ǰ�, ��ȣ ������ ������. 
    // ���� : Signal Search2�� �������� �����Ƿ� �ӵ��� ������� Ż���� �ⱸ���� ������ ���� �����Ƿ� �����Ѵ�.
    // *s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    // *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.

	// ��ȣ����1(Signal search 1) ��ٸ��� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
	BOOL    PASCAL EXPORT CFS20start_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
	BOOL    PASCAL EXPORT CFS20start_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
	// ��ȣ����1(Signal search 1) S���� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
	BOOL    PASCAL EXPORT CFS20start_s_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
	BOOL    PASCAL EXPORT CFS20start_s_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
	// ��ȣ����2(Signal search 2) ��ٸ��� ����, ������ ����
	BOOL    PASCAL EXPORT CFS20start_signal_search2(INT16 axis, double velocity, UINT8 detect_signal);

	// MPG(Manual Pulse Generation) ���� ����-===========================================================================
    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� ��带 ����/Ȯ���Ѵ�.
		// mode
		// 0x1 : Slave �������, �ܺ� Differential ��ȣ�� ���� ���
		// 0x2 : ���� �޽� ����, �ܺ� �Է� ��ȣ�� ���� ���� �޽� ���� ����
		// 0x4 : ���� ���� ���, �ܺ� ���� �Է� ��ȣ�� Ư�� ���� ���� ����
	BOOL	PASCAL EXPORT CFS20set_mpg_drive_mode(INT16 axis, UINT8 mode);
	UINT8	PASCAL EXPORT CFS20get_mpg_drive_mode(INT16 axis);
    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� ���� ������带 ����/Ȯ���Ѵ�.
		// mode
		// 0x0 : �ܺ� ��ȣ�� ���� ���� ����
		// 0x1 : ����ڿ� ���� ������ �������� ����
	BOOL	PASCAL EXPORT CFS20set_mpg_dir_mode(INT16 axis, UINT8 mode);
	UINT8	PASCAL EXPORT CFS20get_mpg_dir_mode(INT16 axis);
    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� ���� ������尡 ����ڿ� ����
    // ������ �������� �����Ǿ��� �� �ʿ��� ������� ���� ���� ���� ���� ����/Ȯ���Ѵ�.
		// mode
		// 0x0 : ����� ���� ���� ������ +�� ����
		// 0x1 : ����� ���� ���� ������ -�� ����
	BOOL	PASCAL EXPORT CFS20set_mpg_user_dir(INT16 axis, UINT8 mode);
	UINT8	PASCAL EXPORT CFS20get_mpg_user_dir(INT16 axis);
    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���Ǵ� EXPP/EXMP �� �Է� ��带 �����Ѵ�.
    //  2 bit : '0' : level input(���� �Է� 4 = EXPP, ���� �Է� 5 = EXMP�� �Է� �޴´�.)
    //          '1' : Differential input(���� �Է����� EXPP, EXMP�� �Է� ����,)
    //  1~0bit: "00" : 1 phase
    //          "01" : 2 phase 1 times
    //          "10" : 2 phase 2 times
    //          "11" : 2 phase 4 times
	BOOL	PASCAL EXPORT CFS20set_mpg_input_method(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT CFS20get_mpg_input_method(INT16 axis);
    // MPG��ġ ���� �����Ѵ�. ������ ���¿��� MPG ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
	BOOL	PASCAL EXPORT CFS20set_mpg_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_mpg_position(INT16 axis);

	// MPG(Manual Pulse Generation) ���� -===============================================================================
	// ������ �ӵ��� ��ٸ��� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
	BOOL    PASCAL EXPORT CFS20start_mpg(INT16 axis, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20start_mpg_ex(INT16 axis, double velocity, double acceltime);
	// ������ �ӵ��� S���� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
	BOOL    PASCAL EXPORT CFS20start_s_mpg(INT16 axis, double velocity, double acceleration);
	BOOL    PASCAL EXPORT CFS20start_s_mpg_ex(INT16 axis, double velocity, double acceltime);

    // �������̵�(������)-================================================================================================
    // ���� ���� �Ÿ� ������ ���� ���۽������� �Է��� ��ġ(������ġ)�� ������ �ٲ۴�.
	BOOL    PASCAL EXPORT CFS20position_override(INT16 axis, double overrideposition);
    // ���� ���� �Ÿ� ������ ���� ���۽������� �Է��� �Ÿ�(�����ġ)�� ������ �ٲ۴�.    
	BOOL    PASCAL EXPORT CFS20position_r_override(INT16 axis, double overridedistance);
    // ������ ���� �ʱ� ������ �ӵ��� �ٲ۴�.(set_max_speed > velocity > set_startstop_speed)
	BOOL    PASCAL EXPORT CFS20velocity_override(INT16 axis, double velocity);
    // ���� ���� ������ ����Ǳ� �� �Էµ� overrideposition���� �ּ� ��� �޽�(dec_pulse) �̻��� ��� override ������ �Ѵ�.
	BOOL    PASCAL EXPORT CFS20position_override2(INT16 axis, double overrideposition, double dec_pulse);
    // ���� �࿡ ����/���� ���� ������ ������ ���������� �ӵ� override ������ �Ѵ�.
	BOOL	PASCAL EXPORT CFS20velocity_override2(INT16 axis, double velocity, double acceleration, double deceleration, double jerk);
 
    // ���� ���� Ȯ��-====================================================================================================
    // ���� ���� ������ ����� ������ ��ٸ� �� �Լ��� �����.
	UINT16	PASCAL EXPORT CFS20wait_for_done(INT16 axis);

    // ���� ���� ����-====================================================================================================
    // ���� ���� �������Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_e_stop(INT16 axis);
    // ���� ���� ������ �������� �����Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_stop(INT16 axis);
    // ���� ���� �Էµ� �������� �����Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_stop_decel(INT16 axis, double deceleration);
    // ���� ���� �Էµ� ���� �ð����� �����Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_stop_deceltime(INT16 axis, double deceltime);

    // ���� ���� �������� ����-==========================================================================================
    // Master/Slave link �Ǵ� ��ǥ�� link ���� �ϳ��� ����Ͽ��� �Ѵ�.
    // Master/Slave link ����. (�Ϲ� ���� ������ master �� ������ slave�൵ ���� �����ȴ�.)
	BOOL    PASCAL EXPORT CFS20link(INT16 master, INT16 slave, double ratio);
    // Master/Slave link ����
	BOOL    PASCAL EXPORT CFS20endlink(INT16 slave);

	// ��ǥ�� link ����-================================================================================================
	// ���� ��ǥ�迡 �� �Ҵ� - n_axes������ŭ�� ����� ����/Ȯ���Ѵ�.(coordinate�� 1..8���� ��� ����)
	// n_axes ������ŭ�� ����� ����/Ȯ���Ѵ�. - (n_axes�� 1..4���� ��� ����)
	BOOL    PASCAL EXPORT CFS20map_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
	BOOL    PASCAL EXPORT CFS20get_mapped_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
    // ���� ��ǥ���� ���/���� ��� ����/Ȯ���Ѵ�.
		// mode:
		// 0: �����ǥ����, 1: ������ǥ ����
	void	PASCAL EXPORT CFS20set_coordinate_mode(INT16 coordinate, INT16 mode);
	INT16	PASCAL EXPORT CFS20get_coordinate_mode(INT16 coordinate);
    // ���� ��ǥ���� �ӵ� �������� ����/Ȯ���Ѵ�.
		// mode:
		// 0: ��ٸ��� ����, 1: SĿ�� ����
	void	PASCAL EXPORT CFS20set_move_profile(INT16 coordinate, INT16 mode);
	INT16	PASCAL EXPORT CFS20get_move_profile(INT16 coordinate);
    // ���� ��ǥ���� �ʱ� �ӵ��� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_move_startstop_velocity(INT16 coordinate, double velocity);
	double	PASCAL EXPORT CFS20get_move_startstop_velocity(INT16 coordinate);
    // Ư�� ��ǥ���� �ӵ��� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_move_velocity(INT16 coordinate, double velocity);
	double	PASCAL EXPORT CFS20get_move_velocity(INT16 coordinate);
    // Ư�� ��ǥ���� �������� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_move_acceleration(INT16 coordinate, double acceleration);
	double	PASCAL EXPORT CFS20get_move_acceleration(INT16 coordinate);
    // Ư�� ��ǥ���� ���� �ð�(Sec)�� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_move_acceltime(INT16 coordinate, double acceltime);
	double	PASCAL EXPORT CFS20get_move_acceltime(INT16 coordinate);
    // ���� ��������  ��ǥ���� ���� �����ӵ��� ��ȯ�Ѵ�.
	double	PASCAL EXPORT CFS20co_get_velocity(INT16 coordinate);

    // ����Ʈ���� ���� ����(���� ��ǥ�迡 ���Ͽ�)-========================================================================
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
    // 2, 3, 4���� �����̵��Ѵ�.
	BOOL    PASCAL EXPORT CFS20move_2(INT16 coordinate, double x, double y);
	BOOL    PASCAL EXPORT CFS20move_3(INT16 coordinate, double x, double y, double z);
	BOOL    PASCAL EXPORT CFS20move_4(INT16 coordinate, double x, double y, double z, double w);
 	// Non Blocking�Լ� (�������� ��� ���õ�)
    // 2, 3, 4���� ���� �̵��Ѵ�.
	BOOL    PASCAL EXPORT CFS20start_move_2(INT16 coordinate, double x, double y);
	BOOL    PASCAL EXPORT CFS20start_move_3(INT16 coordinate, double x, double y, double z);
	BOOL    PASCAL EXPORT CFS20start_move_4(INT16 coordinate, double x, double y, double z, double w);
    // ���� ��ǥ���� ������� ��� �Ϸ� üũ    
	BOOL    PASCAL EXPORT CFS20co_motion_done(INT16 coordinate);
    // ���� ��ǥ���� ������ �Ϸ�ɶ� ���� ��ٸ���.
	BOOL    PASCAL EXPORT CFS20co_wait_for_done(INT16 coordinate);

    // ���� ����(���� ����) : Master/Slave�� link�Ǿ� ���� ��� ������ �߻� �� �� �ִ�.-==================================
    // ���� ����� ���� �Ÿ� �� �ӵ� ���ӵ� ������ ���� ���� �����Ѵ�. ���� ���ۿ� ���� ����ȭ�� ����Ѵ�. 
    // start_** : ���� �࿡�� ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�.
    // *r*_*    : ���� �࿡�� �Էµ� �Ÿ���ŭ(�����ǥ)�� �̵��Ѵ�. "*r_*�� ������ �Էµ� ��ġ(������ǥ)�� �̵��Ѵ�.
    // *s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    // *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.

	// ���� �����޽�(Pulse Drive)����, ��ٸ��� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� ��� �������� �޽� ����� �Ϸ�� �� �Ѿ��)
 	UINT8	PASCAL EXPORT CFS20move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT CFS20move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	UINT8	PASCAL EXPORT CFS20r_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT CFS20r_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking�Լ� (�������� ���� ���õ�)
	BOOL    PASCAL EXPORT CFS20start_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	BOOL    PASCAL EXPORT CFS20start_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	BOOL    PASCAL EXPORT CFS20start_r_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	BOOL    PASCAL EXPORT CFS20start_r_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
 	// ���� �����޽�(Pulse Drive)����, S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� ��� �������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT8	PASCAL EXPORT CFS20s_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT CFS20s_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	UINT8	PASCAL EXPORT CFS20rs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT CFS20rs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking�Լ� (�������� ���� ���õ�)
	BOOL    PASCAL EXPORT CFS20start_s_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	BOOL    PASCAL EXPORT CFS20start_s_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	BOOL    PASCAL EXPORT CFS20start_rs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	BOOL    PASCAL EXPORT CFS20start_rs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
    //���� ��鿡 ���Ͽ� S���� ������ ���� �����ӽ��� SĿ���� ������ ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_s_rate_all(INT16 number, INT16 *axes, double *a_percent, double *b_percent);
	void	PASCAL EXPORT CFS20get_s_rate_all(INT16 number, INT16 *axes, double *a_percent, double *b_percent);

    // ���� ���� Ȯ��-====================================================================================================
    // �Է� �ش� ����� ���� ���¸� Ȯ���ϰ� ������ ���� �� ���� ��ٸ���.
	UINT8	PASCAL EXPORT CFS20wait_for_all(INT16 number, INT16 *axes);

    // ���� ���� ����-====================================================================================================
    // ���� ����� ���⸦ ������Ų��. - ��������� �������� ���������ʰ� �����.
	BOOL    PASCAL EXPORT CFS20reset_axis_sync(INT16 nLen, INT16 *aAxis);
    // ���� ����� ���⸦ ������Ų��. - ��������� �������� ���������ʰ� �����.
	BOOL    PASCAL EXPORT CFS20set_axis_sync(INT16 nLen, INT16 *aAxis);
   	// ������ ���� ���� ����/����/Ȯ���Ѵ�.
		// sync:
		// 0: Reset - ���� �������� ����.
		// 1: Set	- ���� ������.
	BOOL    PASCAL EXPORT CFS20set_sync_axis(INT16 axis, UINT8 sync);
	UINT8   PASCAL EXPORT CFS20get_sync_axis(INT16 axis);
   	// ������ ����� ���� ���� ����/����/Ȯ���Ѵ�.
		// sync:
		// 0: Reset - ���� �������� ����.
		// 1: Set	- ���� ������.	
	BOOL    PASCAL EXPORT CFS20set_sync_module(INT16 axis, UINT8 sync);
	UINT8   PASCAL EXPORT CFS20get_sync_module(INT16 axis);

    // ���� ���� ����-====================================================================================================
    // Ȩ ��ġ �����嵵 ����
	BOOL    PASCAL EXPORT CFS20emergency_stop();

    // -�����˻� =========================================================================================================
    // ���̺귯�� �󿡼� Thread�� ����Ͽ� �˻��Ѵ�. ���� : ������ Ĩ���� StartStop Speed�� ���� �� �ִ�.
    // �����˻��� �����Ѵ�.
		// bStop:
		// 0: ��������
		// 1: ������
	BOOL    PASCAL EXPORT CFS20abort_home_search(INT16 axis, UINT8 bStop);
    // �����˻��� �����Ѵ�. �����ϱ� ���� �����˻��� �ʿ��� ������ �ʿ��ϴ�.
	BOOL    PASCAL EXPORT CFS20home_search(INT16 axis);
    // �Է� ����� ���ÿ� �����˻��� �ǽ��Ѵ�.
	BOOL    PASCAL EXPORT CFS20home_search_all(INT16 number, INT16 *axes);
    // �����˻� ���� �������� Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20get_home_done(INT16 axis);
		// ��ȯ��: 0: �����˻� ������, 1: �����˻� ����
    // �ش� ����� �����˻� ���� �������� Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20get_home_done_all(INT16 number, INT16 *axes);
    // ���� ���� ���� �˻� ������ ���� ���¸� Ȯ���Ѵ�.
	UINT8	PASCAL EXPORT CFS20get_home_end_status(INT16 axis);
		// ��ȯ��: 0: �����˻� ����, 1: �����˻� ����
    // ���� ����� ���� �˻� ������ ���� ���¸� Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20get_home_end_status_all(INT16 number, INT16 *axes, UINT8 *endstatus);
    // ���� �˻��� �� ���ܸ��� method�� ����/Ȯ���Ѵ�.
		// Method�� ���� ���� 
		//    0 Bit ���� ��뿩�� ���� (0 : ������� ����, 1: �����
		//    1 Bit ������ ��� ���� (0 : ������, 1 : ���� �ð�)
		//    2 Bit ������� ���� (0 : ���� ����, 1 : �� ����)
		//    3 Bit �˻����� ���� (0 : cww(-), 1 : cw(+))
		// 7654 Bit detect signal ����(typedef : DETECT_DESTINATION_SIGNAL)
	BOOL    PASCAL EXPORT CFS20set_home_method(INT16 axis, INT16 nstep, UINT8 *method);
	BOOL    PASCAL EXPORT CFS20get_home_method(INT16 axis, INT16 nstep, UINT8 *method);
    // ���� �˻��� �� ���ܸ��� offset�� ����/Ȯ���Ѵ�.	
	BOOL    PASCAL EXPORT CFS20set_home_offset(INT16 axis, INT16 nstep, double *offset);
	BOOL    PASCAL EXPORT CFS20get_home_offset(INT16 axis, INT16 nstep, double *offset);
    // �� ���� ���� �˻� �ӵ��� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_home_velocity(INT16 axis, INT16 nstep, double *velocity);
	BOOL    PASCAL EXPORT CFS20get_home_velocity(INT16 axis, INT16 nstep, double *velocity);
    // ���� ���� ���� �˻� �� �� ���ܺ� �������� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
	BOOL    PASCAL EXPORT CFS20get_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
    // ���� ���� ���� �˻� �� �� ���ܺ� ���� �ð��� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
	BOOL    PASCAL EXPORT CFS20get_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
    // ���� �࿡ ���� �˻����� ���ڴ� 'Z'�� ���� ��� �� ���� �Ѱ谪�� ����/Ȯ���Ѵ�.(Pulse) - ������ ����� �˻� ����
	BOOL    PASCAL EXPORT CFS20set_zphase_search_range(INT16 axis, INT16 pulses);
	INT16	PASCAL EXPORT CFS20get_zphase_search_range(INT16 axis);
	// ���� ��ġ�� ����(0 Position)���� �����Ѵ�. - �������̸� ���õ�.
	BOOL    PASCAL EXPORT CFS20home_zero(INT16 axis);
	// ������ ��� ���� ���� ��ġ�� ����(0 Position)���� �����Ѵ�. - �������� ���� ���õ�
	BOOL    PASCAL EXPORT CFS20home_zero_all(INT16 number, INT16 *axes);

    // ���� �����-=======================================================================================================
    // ���� ���
    // 0 bit : ���� ��� 0(Servo-On)
    // 1 bit : ���� ��� 1(ALARM Clear)
    // 2 bit : ���� ��� 2
    // 3 bit : ���� ��� 3
    // 4 bit(PLD) : ���� ��� 4
    // 5 bit(PLD) : ���� ��� 5
    // ���� �Է�
    // 0 bit : ���� �Է� 0(ORiginal Sensor)
    // 1 bit : ���� �Է� 1(Z phase)
    // 2 bit : ���� �Է� 2
    // 3 bit : ���� �Է� 3
    // 4 bit(PLD) : ���� �Է� 5
    // 5 bit(PLD) : ���� �Է� 6
    // On ==> ���ڴ� N24V, 'Off' ==> ���ڴ� Open(float).	

    // ���� ���� ��°��� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_output(INT16 axis, UINT8 value);
	UINT8	PASCAL EXPORT CFS20get_output(INT16 axis);
    // ���� �Է� ���� Ȯ���Ѵ�.
    // '1'('On') <== ���ڴ� N24V�� �����, '0'('Off') <== ���ڴ� P24V �Ǵ� Float.
	UINT8	PASCAL EXPORT CFS20get_input(INT16 axis);
    // �ش� ���� �ش� bit�� ����� On/Off ��Ų��.
    // bitNo : 0 ~ 5.
	BOOL    PASCAL EXPORT CFS20set_output_bit(INT16 axis, UINT8 bitNo);
	BOOL    PASCAL EXPORT CFS20reset_output_bit(INT16 axis, UINT8 bitNo);
    // �ش� ���� �ش� ���� ��� bit�� ��� ���¸� Ȯ���Ѵ�.
    // bitNo : 0 ~ 5.
	BOOL    PASCAL EXPORT CFS20output_bit_on(INT16 axis, UINT8 bitNo);
    // �ش� ���� �ش� ���� ��� bit�� ���¸� �Է� state�� �ٲ۴�.
    // bitNo : 0 ~ 5. 
	BOOL    PASCAL EXPORT CFS20change_output_bit(INT16 axis, UINT8 bitNo, UINT8 state);
    // �ش� ���� �ش� ���� �Է� bit�� ���¸� Ȯ�� �Ѵ�.
    // bitNo : 0 ~ 5.
	BOOL    PASCAL EXPORT CFS20input_bit_on(INT16 axis, UINT8 bitNo);
	// ���� �Է�(Universal input) 4 ��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT CFS20set_ui4_mode(INT16 axis, UINT8 state);
	UINT8	PASCAL EXPORT CFS20get_ui4_mode(INT16 axis);
	// ���� �Է�(Universal input) 5 ��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT CFS20set_ui5_mode(INT16 axis, UINT8 state);
	UINT8	PASCAL EXPORT CFS20get_ui5_mode(INT16 axis);

    // �ܿ� �޽� clear-===================================================================================================
    // �ش� ���� ������ �ܿ� �޽� Clear ����� ��� ���θ� ����/Ȯ���Ѵ�.
    // CLR ��ȣ�� Default ��� ==> ���ڴ� Open�̴�.
	BOOL    PASCAL EXPORT CFS20set_crc_mask(INT16 axis, INT16 mask);
	UINT8	PASCAL EXPORT CFS20get_crc_mask(INT16 axis);
    // �ش� ���� �ܿ� �޽� Clear ����� Active level�� ����/Ȯ���Ѵ�.
    // Default Active level ==> '1' ==> ���ڴ� N24V
	BOOL    PASCAL EXPORT CFS20set_crc_level(INT16 axis, INT16 level);
	UINT8	PASCAL EXPORT CFS20get_crc_level(INT16 axis);
    // �ش� ���� -Emeregency End limit�� ���� Clear��� ��� ������ ����/Ȯ���Ѵ�.    
	BOOL    PASCAL EXPORT CFS20set_crc_nelm_mask(INT16 axis, INT16 mask);
	UINT8	PASCAL EXPORT CFS20get_crc_nelm_mask(INT16 axis);
    // �ش� ���� -Emeregency End limit�� Active level�� ����/Ȯ���Ѵ�. set_nend_limit_level�� �����ϰ� �����Ѵ�.    
	BOOL    PASCAL EXPORT CFS20set_crc_nelm_level(INT16 axis, INT16 level);
	UINT8	PASCAL EXPORT CFS20get_crc_nelm_level(INT16 axis);
    // �ش� ���� +Emeregency End limit�� ���� Clear��� ��� ������ ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_crc_pelm_mask(INT16 axis, INT16 mask);
	UINT8	PASCAL EXPORT CFS20get_crc_pelm_mask(INT16 axis);
    // �ش� ���� +Emeregency End limit�� Active level�� ����/Ȯ���Ѵ�. set_nend_limit_level�� �����ϰ� �����Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_crc_pelm_level(INT16 axis, INT16 level);
	UINT8	PASCAL EXPORT CFS20get_crc_pelm_level(INT16 axis);
    // �ش� ���� �ܿ� �޽� Clear ����� �Է� ������ ���� ���/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_programmed_crc(INT16 axis, INT16 data);
	UINT8	PASCAL EXPORT CFS20get_programmed_crc(INT16 axis);

    // Ʈ���� ��� ======================================================================================================
    // ����/�ܺ� ��ġ�� ���Ͽ� �ֱ�/���� ��ġ���� ������ Active level�� Trigger pulse�� �߻� ��Ų��.
    // Ʈ���� ��� �޽��� Active level�� ����/Ȯ���Ѵ�.
    // ('0' : 5V ���(0 V), 24V �͹̳� ���(Open); '1'(default) : 5V ���(5 V), 24V �͹̳� ���(N24V).
	BOOL    PASCAL EXPORT CFS20set_trigger_level(INT16 axis, UINT8 trigger_level);
	UINT8	PASCAL EXPORT CFS20get_trigger_level(INT16 axis);
    // Ʈ���� ��ɿ� ����� ���� ��ġ�� �����Ѵ�.
    // 0x0 : �ܺ� ��ġ External(Actual)
    // 0x1 : ���� ��ġ Internal(Command)
	BOOL    PASCAL EXPORT CFS20set_trigger_sel(INT16 axis, UINT8 trigger_sel);
	UINT8	PASCAL EXPORT CFS20get_trigger_sel(INT16 axis);
	// time
    // 0x00 : 4 msec(Ĩ ��� Bypass)
    // 0x01 : 8msec
    // 0x02 : 16msec
    // 0x03	: 24msec
    // ~
    // 0x0A: 80msec
    // 0x0B: 88msec
    // ~
    // 0x0F: 120msec
	BOOL    PASCAL EXPORT CFS20set_trigger_time(INT16 axis, UINT8 time);
	UINT8	PASCAL EXPORT CFS20get_trigger_time(INT16 axis);
    // ���� �࿡ Ʈ���� �߻� ����� ����/Ȯ���Ѵ�.
    // 0x0 : Ʈ���� ���� ��ġ���� Ʈ���� �߻�, ���� ��ġ ���
    // 0x1 : Ʈ���� ��ġ���� ����� �ֱ� Ʈ���� ���
    BOOL   PASCAL EXPORT CFS20set_trigger_mode(INT16 axis, UINT8 mode_sel);
    UINT8  PASCAL EXPORT CFS20get_trigger_mode(INT16 axis);
    // ���� �࿡ Ʈ���� �ֱ� �Ǵ� ���� ��ġ ���� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CFS20set_trigger_position(INT16 axis, double trigger_position);
    double PASCAL EXPORT CFS20get_trigger_position(INT16 axis);
    // ���� ���� Ʈ���� ����� ��� ���θ� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_trigger_enable(INT16 axis, UINT8 ena_status);
	UINT8	PASCAL EXPORT CFS20is_trigger_enabled(INT16 axis);
    // ���� �࿡ Ʈ���� �߻��� ���ͷ�Ʈ�� �߻��ϵ��� ����/Ȯ���Ѵ�.
	BOOL    PASCAL EXPORT CFS20set_trigger_interrupt_enable(INT16 axis, UINT8 ena_int);
	UINT8	PASCAL EXPORT CFS20is_trigger_interrupt_enabled(INT16 axis);

	// MARK ����̺� �����Լ� ===========================================================================================
	// MARK, �����޽�(Pulse Drive) ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL    PASCAL EXPORT CFS20start_pr_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	BOOL    PASCAL EXPORT CFS20start_pr_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī �����޽�(Pulse Drive) ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL    PASCAL EXPORT CFS20start_pra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	BOOL    PASCAL EXPORT CFS20start_pra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// �����޽�(Pulse Drive) ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16  PASCAL EXPORT CFS20pr_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	UINT16  PASCAL EXPORT CFS20pr_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī �����޽�(Pulse Drive) ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16  PASCAL EXPORT CFS20pra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	UINT16  PASCAL EXPORT CFS20pra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK, �����޽�(Pulse Drive) S���� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL    PASCAL EXPORT CFS20start_prs_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	BOOL    PASCAL EXPORT CFS20start_prs_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī �����޽�(Pulse Drive) S���� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL    PASCAL EXPORT CFS20start_pras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	BOOL    PASCAL EXPORT CFS20start_pras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK, �����޽�(Pulse Drive) S���� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16  PASCAL EXPORT CFS20prs_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	UINT16  PASCAL EXPORT CFS20prs_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī �����޽�(Pulse Drive) S���� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16  PASCAL EXPORT CFS20pras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	UINT16  PASCAL EXPORT CFS20pras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK Signal�� Active level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT CFS20set_mark_signal_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT CFS20get_mark_signal_level(INT16 axis);	
	UINT8	PASCAL EXPORT CFS20get_mark_signal_switch(INT16 axis);
	
	BOOL	PASCAL EXPORT CFS20set_mark_signal_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT CFS20get_mark_signal_enable(INT16 axis);

    // ��ġ �񱳱� ���� �Լ��� ==========================================================================================
	// Internal(Command) comparator���� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_internal_comparator_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_internal_comparator_position(INT16 axis);
	// External(Encoder) comparator���� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT CFS20set_external_comparator_position(INT16 axis, double position);
	double	PASCAL EXPORT CFS20get_external_comparator_position(INT16 axis);

    // �����ڵ� �б� �Լ��� =============================================================================================
	// ������ �����ڵ带 �д´�.
	INT16	PASCAL EXPORT CFS20get_error_code();
	// �����ڵ��� ������ ���ڷ� ��ȯ�Ѵ�.
	char*	PASCAL EXPORT CFS20get_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_CAMC_FS20_H__
