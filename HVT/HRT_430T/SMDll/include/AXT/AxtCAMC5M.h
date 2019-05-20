#ifndef __AXT_CAMC_5M_H__
#define __AXT_CAMC_5M_H__

#include "AxtLIBDef.h"
#include "CAMC5MDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTCAMC5M Library - CAMC-5M Motion module
	������ǰ
		SMC-1V01 - CAMC-5M 1��
		SMC-2V01 - CAMC-5M 2��
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// �ʱ�ȭ �Լ���
	// CAMC-5M�� ������ ���(SMC-1V01, SMC-2V01)�� �˻��Ͽ� �ʱ�ȭ�Ѵ�. CAMC-5M�� �����Ѵ�
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
	BOOL    PASCAL EXPORT InitializeCAMC5M(BOOL reset);
    // CAMC-5M ����� ����� ���������� Ȥ���Ѵ�
	BOOL    PASCAL EXPORT C5MIsInitialized(void);
		// ���ϰ� :  1(TRUE) = CAMC-5M ����� ��� �����ϴ�
	// CAMC-5M�� ������ ����� ����� �����Ѵ�
	void    PASCAL EXPORT C5MStopService();

    /// ���� ���� ���� �Լ���        -===================================================================================
	// ������ �ּҿ� ������ ���̽������� ��ȣ�� �����Ѵ�. ������ -1�� �����Ѵ�
	INT16	PASCAL EXPORT C5Mget_boardno(UINT32 address);
	// ���̽������� ������ �����Ѵ�
	INT16	PASCAL EXPORT C5Mget_numof_boards();
	// ������ ���̽����忡 ������ ���� ������ �����Ѵ�
	INT16	PASCAL EXPORT C5Mget_numof_axes(INT16 nBoardNo);
	// ���� ������ �����Ѵ�
	INT16	PASCAL EXPORT C5Mget_total_numof_axis();	
	// ������ ���̽������ȣ�� ����ȣ�� �ش��ϴ� ���ȣ�� �����Ѵ�
	INT16	PASCAL EXPORT C5Mget_axisno(INT16 nBoardNo, INT16 nModuleNo);
	// ������ ���� ������ �����Ѵ�
    // nBoardNo : �ش� ���� ������ ���̽������� ��ȣ.
    // nModuleNo: �ش� ���� ������ ����� ���̽� ��峻 ��� ��ġ(0~3)
    // bModuleID: �ش� ���� ������ ����� ID : SMC-2V01(0x01)
    // nAxisPos : �ش� ���� ������ ����� ù��°���� �ι�° ������ ����.(0 : ù��°, 1 : �ι�°)
	BOOL	PASCAL EXPORT C5Mget_axis_info(INT16 nAxisNo, INT16 *nBoardNo, INT16 *nModuleNo, UINT8 *bModuleID, INT16 *nAxisPos);

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
    //  9) ����/�ܺ� ī���� : 0. 	
	BOOL	PASCAL EXPORT C5Mload_parameter(INT16 axis, char *nfilename);
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
	BOOL	PASCAL EXPORT C5Msave_parameter(INT16 axis, char *nfilename); 
	// ��� ���� �ʱⰪ�� ������ ���Ͽ��� �о �����Ѵ�
	BOOL	PASCAL EXPORT C5Mload_parameter_all(char *nfilename);
	// ��� ���� �ʱⰪ�� ������ ���Ͽ� �����Ѵ�
	BOOL	PASCAL EXPORT C5Msave_parameter_all(char *nfilename); 

	// ���ͷ�Ʈ �Լ���   -================================================================================================
	//(���ͷ�Ʈ�� ����ϱ� ���ؼ��� 
	//Window message & procedure
	//    hWnd    : ������ �ڵ�, ������ �޼����� ������ ���. ������� ������ NULL�� �Է�.
	//    wMsg    : ������ �ڵ��� �޼���, ������� �ʰų� ����Ʈ���� ����Ϸ��� 0�� �Է�.
	//    proc    : ���ͷ�Ʈ �߻��� ȣ��� �Լ��� ������, ������� ������ NULL�� �Է�.
	void	PASCAL EXPORT C5MSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_CAMC5M_INTERRUPT_PROC proc);	

    // ���� ���� �ʱ�ȭ �Լ���        -==================================================================================
	// ����Ŭ�� ����( ��⿡ ������ Oscillator�� ����� ��쿡�� ����)
	void	PASCAL EXPORT C5MKeSetMainClk(INT32 nMainClk);
	// Drive mode 1�� ����/Ȯ���Ѵ�.
		// decelstartpoint : �����Ÿ� ���� ��� ����� ���� ��ġ ���� ��� ����(0 : �ڵ� ������, 1 : ���� ������)
		// pulseoutmethod : ��� �޽� ��� ����(typedef : PULSE_OUTPUT)
		// detecsignal : ��ȣ �˻�-1/2 ���� ��� ����� �˻� �� ��ȣ ����(typedef : DETECT_DESTINATION_SIGNAL)
	void	PASCAL EXPORT C5Mset_drive_mode1(INT16 axis, 
				UINT8 decelstartpoint,
				UINT8 pulseoutmethod,
				UINT8 detectsignal);
	UINT8	PASCAL EXPORT C5Mget_drive_mode1(INT16 axis);
	// Drive mode 2�� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_drive_mode2(INT16 axis, 
				UINT8 encmethod,
				UINT8 inpactivelevel,
				UINT8 alarmactivelevel,
				UINT8 nslmactivelevel,
				UINT8 pslmactivelevel,
				UINT8 nelmactivelevel,
				UINT8 pelmactivelevel);
	UINT8	PASCAL EXPORT C5Mget_drive_mode2(INT16 axis);
    // Unit/Pulse ����/Ȯ���Ѵ�.
		// Unit/Pulse : 1 pulse�� ���� system�� �̵��Ÿ��� ���ϸ�, �̶� Unit�� ������ ����ڰ� ���Ƿ� ������ �� �ִ�.
		// Ex) Ball screw pitch : 10mm, ���� 1ȸ���� �޽��� : 10000 ==> Unit�� mm�� ������ ��� : Unit/Pulse = 10/10000.
		// ���� unitperpulse�� 0.001�� �Է��ϸ� ��� ��������� mm�� ������. 
		// Ex) Linear motor�� ���ش��� 1 pulse�� 2 uM. ==> Unit�� mm�� ������ ��� : Unit/Pulse = 0.002/1.
	void	PASCAL EXPORT C5Mset_moveunit_perpulse(INT16 axis, double unitperpulse);
	double	PASCAL EXPORT C5Mget_moveunit_perpulse(INT16 axis);
	// Unit/Pulse�� ��������
    // pulse/Unit ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_movepulse_perunit(INT16 axis, INT32 pulseperunit);
	INT32	PASCAL EXPORT C5Mget_movepulse_perunit(INT16 axis);
    // ���� �ӵ� ����/Ȯ���Ѵ�.(Unit/Sec)
	void	PASCAL EXPORT C5Mset_startstop_speed(INT16 axis, double velocity);
	double	PASCAL EXPORT C5Mget_startstop_speed(INT16 axis);
	// �ְ� �ӵ� ���� Unit/Sec. ���� system�� �ְ� �ӵ��� �����Ѵ�.
		// Unit/Pulse ������ ���ۼӵ� ���� ���Ŀ� �����Ѵ�.
		// ������ �ְ� �ӵ� �̻����δ� ������ �Ҽ� �����Ƿ� �����Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_max_speed(INT16 axis, double max_velocity);
	double	PASCAL EXPORT C5Mget_max_speed(INT16 axis);
    // SW�� ����� ���� ����/Ȯ���Ѵ�. �̰����� Quasi S-Curve ������ percentage�� ���� �����ϴ�.
	void	PASCAL EXPORT C5Mset_qs_rate(INT16 axis, double a_percent, double b_percent);
	void	PASCAL EXPORT C5Mget_qs_rate(INT16 axis, double *a_percent, double *b_percent);
	// ���� ������ ��忡�� �ܷ� �޽��� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_slowdown_rear_pulse(INT16 axis, UINT32 ulData);
	UINT32	PASCAL EXPORT C5Mget_slowdown_rear_pulse(INT16 axis);
	// ���� ���� ���� ���� ������ ���� ����� ����/Ȯ���Ѵ�.
		// method
		// 0x0 : �ڵ� ������.
		// 0x1 : ���� ������.
	BOOL	PASCAL EXPORT C5Mset_decel_point(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT C5Mget_decel_point(INT16 axis);

    // ���� ���� Ȯ�� �Լ���        -=====================================================================================
    // ���� ���� �޽� ����������� Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Min_motion(INT16 axis);
    // ���� ���� �޽� ����� ����ƴ��� Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mmotion_done(INT16 axis);
    // ���� ���� �������� ���� ��µ� �޽� ī���� ���� Ȯ���Ѵ�. (Pulse)
	INT32	PASCAL EXPORT C5Mget_drive_pulse_counts(INT16 axis);
    // ���� ���� DriveStatus �������͸� Ȯ���Ѵ�.
	UINT8	PASCAL EXPORT C5Mget_drive_status(INT16 axis);
    // ���� ���� EndStatus �������͸� Ȯ���Ѵ�.
		// End Status Bit�� �ǹ�
		// 7 bit : ����Ÿ ���� ������ ���� ����
		// 6 bit : ALARM ��ȣ �Է¿� ���� ����
		// 5 bit : ������ ��ɿ� ���� ����
		// 4 bit : �������� ��ɿ� ���� ����
		// 3 bit : ������ ��ȣ �Է¿� ���� ���� (EMG Button)
		// 2 bit : �������� ��ȣ �Է¿� ���� ����
		// 1 bit : Limit(PELM, NELM, Soft) �������� ���� ����
		// 0 bit : Limit(PSLM, NSLM, Soft) ���������� ���� ����
	UINT8	PASCAL EXPORT C5Mget_end_status(INT16 axis);
    // ���� ���� Mechanical �������͸� Ȯ���Ѵ�.
		// 7 bit : Encoder Up��ȣ �Է� Level(A�� ��ȣ)
		// 6 bit : Encoder Down��ȣ �Է� Level(B�� ��ȣ)
		// 5 bit : INPOSITION ��ȣ Active ����
		// 4 bit : ALARM ��ȣ Active ����
		// 3 bit : -Limit �������� ��ȣ Active ����
		// 2 bit : +Limit �������� ��ȣ Active ����
		// 1 bit : -Limit ������ ��ȣ Active ����
		// 0 bit : +Limit ������ ��ȣ Active ����
	UINT8	PASCAL EXPORT C5Mget_mechanical_signal(INT16 axis);
    // ���� ����  ���� �ӵ��� �о� �´�.(Unit/Sec)
	double	PASCAL EXPORT C5Mget_velocity(INT16 axis);
    // ���� ���� Command position�� Actual position�� ���� Ȯ���Ѵ�.
	double	PASCAL EXPORT C5Mget_error(INT16 axis);
    // ���� ���� ���� ����̺��� �̵� �Ÿ��� Ȯ�� �Ѵ�. (Unit)
	double	PASCAL EXPORT C5Mget_drivedistance(INT16 axis);

    //Encoder �Է� ��� ���� �Լ���        -=============================================================================
    // ���� ���� Encoder �Է� ����� ����/Ȯ���Ѵ�.
		// method : typedef(EXTERNAL_COUNTER_INPUT)
		// UpDownMode = 0x0    // Up/Down
		// Sqr1Mode   = 0x1    // 1ü��
		// Sqr2Mode   = 0x2    // 2ü��
		// Sqr4Mode   = 0x3    // 4ü��
	BOOL	PASCAL EXPORT C5Mset_enc_input_method(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT C5Mget_enc_input_method(INT16 axis);

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
	BOOL	PASCAL EXPORT C5Mset_pulse_out_method(INT16 axis, UINT8 method);
	UINT8	PASCAL EXPORT C5Mget_pulse_out_method(INT16 axis);

    // ��ġ Ȯ�� �� ��ġ �� ���� �Լ��� -===============================================================================
    // �ܺ� ��ġ ���� �����Ѵ�. ������ ���¿��� �ܺ� ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
	void	PASCAL EXPORT C5Mset_actual_position(INT16 axis, double position);
	double	PASCAL EXPORT C5Mget_actual_position(INT16 axis);
    // ���� ��ġ ���� �����Ѵ�. ������ ���¿��� ���� ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
	void	PASCAL EXPORT C5Mset_command_position(INT16 axis, double position);
	double	PASCAL EXPORT C5Mget_command_position(INT16 axis);

    // ���� ����̹� ��� ��ȣ ���� �Լ���-===============================================================================
   	// ���� Enable��� ��ȣ�� Active Level�� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_servo_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_servo_level(INT16 axis);
    // ���� Enable(On) / Disable(Off)�� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_servo_enable(INT16 axis, UINT8 state);
	UINT8	PASCAL EXPORT C5Mget_servo_enable(INT16 axis);

    // ���� ����̹� �Է� ��ȣ ���� �Լ���-===============================================================================
	// ���� ��ġ�����Ϸ�(inposition)�Է� ��ȣ�� ��������� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_inposition_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT C5Mget_inposition_enable(INT16 axis);
   	// ���� ��ġ�����Ϸ�(inposition)�Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_inposition_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_inposition_level(INT16 axis);
	UINT8	PASCAL EXPORT C5Mget_inposition_switch(INT16 axis);
	BOOL	PASCAL EXPORT C5Min_position(INT16 axis);
	// ���� �˶� �Է½�ȣ ����� ��������� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_alarm_enable(INT16 axis, UINT8 use);
	UINT8	PASCAL EXPORT C5Mget_alarm_enable(INT16 axis);
	// ���� �˶� �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_alarm_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_alarm_level(INT16 axis);
	UINT8	PASCAL EXPORT C5Mget_alarm_switch(INT16 axis);

    // ����Ʈ ��ȣ ���� �Լ���-===========================================================================================
	// -������ ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_nend_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_nend_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT C5Mget_nend_limit_switch(INT16 axis);
	// +������ ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_pend_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_pend_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT C5Mget_pend_limit_switch(INT16 axis);
	// -�������� ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_nslow_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_nslow_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT C5Mget_nslow_limit_switch(INT16 axis);
	// +�������� ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_pslow_limit_level(INT16 axis, UINT8 level);
	UINT8	PASCAL EXPORT C5Mget_pslow_limit_level(INT16 axis);
	UINT8	PASCAL EXPORT C5Mget_pslow_limit_switch(INT16 axis);
	// -LIMIT ���� ������ ��/�������� ���θ� ����/Ȯ���Ѵ�.
		// stop:
		// 0 : ������, 1 : �������� (Ver 3.0���� ����)
	BOOL	PASCAL EXPORT C5Mset_nlimit_sel(INT16 axis, UINT8 stop);
	UINT8	PASCAL EXPORT C5Mget_nlimit_sel(INT16 axis);
	// +LIMIT ���� ������ ��/�������� ���θ� ����/Ȯ���Ѵ�. (Ver 3.0���� ����)
	BOOL	PASCAL EXPORT C5Mset_plimit_sel(INT16 axis, UINT8 stop);
	UINT8	PASCAL EXPORT C5Mget_plimit_sel(INT16 axis);

    // ������� ��ȣ-=====================================================================================================
	// ��������� ��/�������� ���θ� ����/Ȯ���Ѵ�.	(Ver 3.0���� ����)
	BOOL	PASCAL EXPORT C5Mset_stop_sel(INT16 axis, UINT8 stop);
	UINT8	PASCAL EXPORT C5Mget_stop_sel(INT16 axis);

	// ���� ���� �Ÿ� ����-===============================================================================================
	// start_** : ���� �࿡�� ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�(Blocking).
	// *r*_*    : ���� �࿡�� �Էµ� �Ÿ���ŭ(�����ǥ)�� �̵��Ѵ�. "*r_*�� ������ �Էµ� ��ġ(������ǥ)�� �̵��Ѵ�.
	// *qs*_*   : ������ �ӵ� ���������� "Quasi S curve"�� �̿��Ѵ�. "*qs_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
	// *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.
	// �Է� ����: velocity(Unit/Sec), acceleration/deceleration(Unit/Sec^2), acceltime/deceltime(Sec), position(Unit)

	// �����޽�(Pulse Drive), ��ٸ��� ���� �Լ�, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT8	PASCAL EXPORT C5Mmove(INT16 axis, double position, double velocity, double acceleration);
	UINT8	PASCAL EXPORT C5Mmove_ex(INT16 axis, double position, double velocity, double acceltime);
	UINT8	PASCAL EXPORT C5Mr_move(INT16 axis, double distance, double velocity, double acceleration);
	UINT8	PASCAL EXPORT C5Mr_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
	BOOL	PASCAL EXPORT C5Mstart_move(INT16 axis, double position, double velocity, double acceleration);
	BOOL	PASCAL EXPORT C5Mstart_move_ex(INT16 axis, double position, double velocity, double acceltime);
	BOOL	PASCAL EXPORT C5Mstart_r_move(INT16 axis, double distance, double velocity, double acceleration);
	BOOL	PASCAL EXPORT C5Mstart_r_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// �����޽�(Pulse Drive), Quasi S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT8	PASCAL EXPORT C5Mqs_move(INT16 axis, double position, double velocity, double acceleration);
	UINT8	PASCAL EXPORT C5Mqs_move_ex(INT16 axis, double position, double velocity, double acceltime);
	UINT8	PASCAL EXPORT C5Mrqs_move(INT16 axis, double distance, double velocity, double acceleration);
	UINT8	PASCAL EXPORT C5Mrqs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
	BOOL	PASCAL EXPORT C5Mstart_qs_move(INT16 axis, double position, double velocity, double acceleration);
	BOOL	PASCAL EXPORT C5Mstart_qs_move_ex(INT16 axis, double position, double velocity, double acceltime);
	BOOL	PASCAL EXPORT C5Mstart_rqs_move(INT16 axis, double distance, double velocity, double acceleration);
	BOOL	PASCAL EXPORT C5Mstart_rqs_move_ex(INT16 axis, double distance, double velocity, double acceltime);

    // ���� ���� ����-====================================================================================================
    // ���� �����ӵ� �� �ӵ��� ���� ������ �߻����� ������ ���������� �����Ѵ�.
    // *qs*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    // *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.

	// ���ӵ� ��ٸ��� ���� �Լ���, ������/���ӽð�(_ex)(�ð�����:Sec) - �������� ��쿡�� �ӵ��������̵�
	BOOL	PASCAL EXPORT C5Mv_move(INT16 axis, double velocity, double acceleration);
	BOOL	PASCAL EXPORT C5Mv_move_ex(INT16 axis, double velocity, double acceltime);
	// ���ӵ� Quasi S���� ���� �Լ���, ������/���ӽð�(_ex)(�ð�����:Sec) - �������� ��쿡�� �ӵ��������̵�
	BOOL	PASCAL EXPORT C5Mv_qs_move(INT16 axis, double velocity, double acceleration);
	BOOL	PASCAL EXPORT C5Mv_qs_move_ex(INT16 axis, double velocity, double acceltime);

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
	BOOL	PASCAL EXPORT C5Mstart_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
	BOOL	PASCAL EXPORT C5Mstart_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
	// ��ȣ����1(Signal search 1) Quasi S���� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
	BOOL	PASCAL EXPORT C5Mstart_qs_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
	BOOL	PASCAL EXPORT C5Mstart_qs_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
	// ��ȣ����2(Signal search 2) ��ٸ��� ����, ������ ����
	BOOL	PASCAL EXPORT C5Mstart_signal_search2(INT16 axis, double velocity, UINT8 detect_signal);

    // �������̵�(������)-================================================================================================
    // ���� ���� �Ÿ� ������ ���� ���۽������� �Է��� ��ġ(������ġ)�� ������ �ٲ۴�.
	BOOL	PASCAL EXPORT C5Mposition_override(INT16 axis, double overrideposition);
    // ���� ���� �Ÿ� ������ ���� ���۽������� �Է��� �Ÿ�(�����ġ)�� ������ �ٲ۴�.    
	BOOL	PASCAL EXPORT C5Mposition_r_override(INT16 axis, double overridedistance);
	
    // ���� ���� Ȯ��-====================================================================================================
    // ���� ���� ������ ����� ������ ��ٸ� �� �Լ��� �����.
	UINT8	PASCAL EXPORT C5Mwait_for_done(INT16 axis);
	
    // ���� ���� ����-====================================================================================================
    // ���� ���� �������Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_e_stop(INT16 axis);
	// ���� ���� ������ �������� �����Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_stop(INT16 axis);
	// ���� ���� �Էµ� �������� �����Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_stop_decel(INT16 axis, double deceleration);
	// ���� ���� �Էµ� ���� �ð����� �����Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_stop_deceltime(INT16 axis, double deceltime);

    // ���� ���� �������� ����-==========================================================================================
    // Master/Slave link �Ǵ� ��ǥ�� link ���� �ϳ��� ����Ͽ��� �Ѵ�.
    // Master/Slave link ����. (�Ϲ� ���� ������ master �� ������ slave�൵ ���� �����ȴ�.)
    // Master/Slave link ����
	BOOL	PASCAL EXPORT C5Mlink(INT16 master, INT16 slave, double ratio);
    // Master/Slave link ����
	BOOL	PASCAL EXPORT C5Mendlink(INT16 slave);

	// ��ǥ�� link ����-================================================================================================
	// ���� ��ǥ�迡 �� �Ҵ� - n_axes������ŭ�� ����� ����/Ȯ���Ѵ�.(coordinate�� 1..8���� ��� ����)
	// n_axes ������ŭ�� ����� ����/Ȯ���Ѵ�. - (n_axes�� 1..4���� ��� ����)
	BOOL	PASCAL EXPORT C5Mmap_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
	BOOL	PASCAL EXPORT C5Mget_mapped_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
    // ���� ��ǥ���� ���/���� ��� ����/Ȯ���Ѵ�.
		// mode:
		// 0: �����ǥ����, 1: ������ǥ ����
	void	PASCAL EXPORT C5Mset_coordinate_mode(INT16 coordinate, INT16 mode);
	INT16	PASCAL EXPORT C5Mget_coordinate_mode(INT16 coordinate);
    // ���� ��ǥ���� �ӵ� �������� ����/Ȯ���Ѵ�.
		// mode:
		// 0: ��ٸ��� ����, 1: Quasi SĿ�� ����
	void	PASCAL EXPORT C5Mset_move_profile(INT16 coordinate, INT16 mode);
	INT16	PASCAL EXPORT C5Mget_move_profile(INT16 coordinate);
    // ���� ��ǥ���� �ʱ� �ӵ��� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_move_startstop_velocity(INT16 coordinate, double velocity);
	double	PASCAL EXPORT C5Mget_move_startstop_velocity(INT16 coordinate);
    // ���� ��ǥ���� �ʱ� �ӵ��� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_move_velocity(INT16 coordinate, double velocity);
	double	PASCAL EXPORT C5Mget_move_velocity(INT16 coordinate);
    // Ư�� ��ǥ���� �������� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_move_acceleration(INT16 coordinate, double acceleration);
	double	PASCAL EXPORT C5Mget_move_acceleration(INT16 coordinate);
    // Ư�� ��ǥ���� ���� �ð�(Sec)�� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_move_acceltime(INT16 coordinate, double acceltime);
	double	PASCAL EXPORT C5Mget_move_acceltime(INT16 coordinate);
    // ���� ��������  ��ǥ���� ���� �ӵ��� ��ȯ�Ѵ�.
	double	PASCAL EXPORT C5Mco_get_velocity(INT16 coordinate);

    // ����Ʈ���� ���� ����(���� ��ǥ�迡 ���Ͽ�)-========================================================================
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
    // 2, 3, 4���� �����̵��Ѵ�.
	BOOL	PASCAL EXPORT C5Mmove_2(INT16 coordinate, double x, double y);
	BOOL	PASCAL EXPORT C5Mmove_3(INT16 coordinate, double x, double y, double z);
	BOOL	PASCAL EXPORT C5Mmove_4(INT16 coordinate, double x, double y, double z, double w);
 	// Non Blocking�Լ� (�������� ��� ���õ�)
    // 2, 3, 4���� ���� �̵��Ѵ�.
	BOOL	PASCAL EXPORT C5Mstart_move_2(INT16 coordinate, double x, double y);
	BOOL	PASCAL EXPORT C5Mstart_move_3(INT16 coordinate, double x, double y, double z);
	BOOL	PASCAL EXPORT C5Mstart_move_4(INT16 coordinate, double x, double y, double z, double w);
    // ���� ��ǥ���� ������� ��� �Ϸ� üũ    
	BOOL	PASCAL EXPORT C5Mco_motion_done(INT16 coordinate);
    // ���� ��ǥ���� ������ �Ϸ�ɶ� ���� ��ٸ���.
	BOOL	PASCAL EXPORT C5Mco_wait_for_done(INT16 coordinate);

    // ���� ����(���� ����) : Master/Slave�� link�Ǿ� ���� ��� ������ �߻� �� �� �ִ�.-==================================
    // ���� ����� ���� �Ÿ� �� �ӵ� ���ӵ� ������ ���� ���� �����Ѵ�. ���� ���ۿ� ���� ����ȭ�� ����Ѵ�. 
    // start_** : ���� �࿡�� ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�.
    // *r*_*    : ���� �࿡�� �Էµ� �Ÿ���ŭ(�����ǥ)�� �̵��Ѵ�. "*r_*�� ������ �Էµ� ��ġ(������ǥ)�� �̵��Ѵ�.
    // *qs*_*    : ������ �ӵ� ���������� "Quasi S curve"�� �̿��Ѵ�. "*qs_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    // *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.

	// ���� �����޽�(Pulse Drive)����, ��ٸ��� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� ��� �������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT8	PASCAL EXPORT C5Mmove_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT C5Mmove_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	UINT8	PASCAL EXPORT C5Mr_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT C5Mr_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking�Լ� (�������� ���� ���õ�)
	BOOL	PASCAL EXPORT C5Mstart_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	BOOL	PASCAL EXPORT C5Mstart_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	BOOL	PASCAL EXPORT C5Mstart_r_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	BOOL	PASCAL EXPORT C5Mstart_r_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
 	// ���� �����޽�(Pulse Drive)����, S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� ��� �������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT8	PASCAL EXPORT C5Mqs_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT C5Mqs_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	UINT8	PASCAL EXPORT C5Mrqs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	UINT8	PASCAL EXPORT C5Mrqs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking�Լ� (�������� ���� ���õ�)
	BOOL	PASCAL EXPORT C5Mstart_qs_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	BOOL	PASCAL EXPORT C5Mstart_qs_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
	BOOL	PASCAL EXPORT C5Mstart_rqs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
	BOOL	PASCAL EXPORT C5Mstart_rqs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
    //���� ��鿡 ���Ͽ� S���� ������ ���� �����ӽ��� Quasi SĿ���� ������ ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_qs_rate_all(INT16 number, INT16 *axes, double *a_percent, double *b_percent);
	void	PASCAL EXPORT C5Mget_qs_rate_all(INT16 number, INT16 *axes, double *a_percent, double *b_percent);

    // ���� ���� Ȯ��-====================================================================================================
    // �Է� �ش� ����� ���� ���¸� Ȯ���ϰ� ������ ���� �� ���� ��ٸ���.
	UINT8	PASCAL EXPORT C5Mwait_for_all(INT16 number, INT16 *axes);

    // ���� ���� ����-====================================================================================================
    // ���� ����� ���⸦ ������Ų��. - ��������� �������� ���������ʰ� �����.
	BOOL	PASCAL EXPORT C5Mreset_axis_sync(INT16 nLen, INT16 *aAxis);
    // ���� ����� ���⸦ ������Ų��. - ��������� �������� ���������ʰ� �����.
	BOOL	PASCAL EXPORT C5Mset_axis_sync(INT16 nLen, INT16 *aAxis);
	// ������ ���̽� ���忡 ������ ����� ���⼳��/Ȯ���Ѵ�. 
		// byState:
		// bit 7 : (SUB4 Axis 1) - 0: ���� �������� ����, 1: ���� ������
		// bit 6 : (SUB4 Axis 0) - 0: ���� �������� ����, 1: ���� ������
		// bit 5 : (SUB3 Axis 1) - 0: ���� �������� ����, 1: ���� ������
		// bit 4 : (SUB3 Axis 0) - 0: ���� �������� ����, 1: ���� ������
		// bit 3 : (SUB2 Axis 1) - 0: ���� �������� ����, 1: ���� ������
		// bit 2 : (SUB2 Axis 0) - 0: ���� �������� ����, 1: ���� ������
		// bit 1 : (SUB1 Axis 1) - 0: ���� �������� ����, 1: ���� ������
		// bit 0 : (SUB1 Axis 0) - 0: ���� �������� ����, 1: ���� ������
	BOOL	PASCAL EXPORT C5Mset_sync(INT16 nBoardNo, UINT8 byState);
	UINT8	PASCAL EXPORT C5Mget_sync_info(INT16 nBoardNo);

    // ���� ���� ����-====================================================================================================
    // Ȩ ��ġ �����嵵 ����
	BOOL	PASCAL EXPORT C5Memergency_stop();
	
    // -�����˻� =========================================================================================================
    // ���̺귯�� �󿡼� Thread�� ����Ͽ� �˻��Ѵ�. ���� : ������ Ĩ���� StartStop Speed�� ���� �� �ִ�.
    // �����˻��� �����Ѵ�.
	/// ���� �˻� �Լ���
	BOOL	PASCAL EXPORT C5Mabort_home_search(INT16 axis, UINT8 bStop);
		// bStop:
		// 0: ��������
		// 1: ������
    // �����˻��� �����Ѵ�. �����ϱ� ���� �����˻��� �ʿ��� ������ �ʿ��ϴ�.
	BOOL	PASCAL EXPORT C5Mhome_search(INT16 axis);
    // �Է� ����� ���ÿ� �����˻��� �ǽ��Ѵ�.
	BOOL	PASCAL EXPORT C5Mhome_search_all(INT16 number, INT16 *axes);
    // �����˻� ���� �������� Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mget_home_done(INT16 axis);
		// ��ȯ��: 0: �����˻� ������, 1: �����˻� ����
    // �ش� ����� �����˻� ���� �������� Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mget_home_done_all(INT16 number, INT16 *axes);
    // ���� ���� ���� �˻� ������ ���� ���¸� Ȯ���Ѵ�.
	UINT8	PASCAL EXPORT C5Mget_home_end_status(INT16 axis);
		// ��ȯ��: 0: �����˻� ����, 1: �����˻� ����
    // ���� ����� ���� �˻� ������ ���� ���¸� Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mget_home_end_status_all(INT16 number, INT16 *axes, UINT8 *endstatus);	
    //���� �˻��� �� ���ܸ��� method�� ����/Ȯ���Ѵ�.
		//Method�� ���� ���� 
		//    0 Bit ���� ��뿩�� ���� (0 : ������� ����, 1: �����
		//    1 Bit ������ ��� ���� (0 : ������, 1 : ���� �ð�)
		//    2 Bit ������� ���� (0 : ���� ����, 1 : �� ����)
		//    3 Bit �˻����� ���� (0 : cww(-), 1 : cw(+))
		// 7654 Bit detect signal ����(typedef : DETECT_DESTINATION_SIGNAL)
	BOOL	PASCAL EXPORT C5Mset_home_method(INT16 axis, INT16 nstep, UINT8 *method);
	BOOL	PASCAL EXPORT C5Mget_home_method(INT16 axis, INT16 nstep, UINT8 *method);
    //���� �˻��� �� ���ܸ��� offset�� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_home_offset(INT16 axis, INT16 nstep, double *offset);
	BOOL	PASCAL EXPORT C5Mget_home_offset(INT16 axis, INT16 nstep, double *offset);
    // �� ���� ���� �˻� �ӵ��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_home_velocity(INT16 axis, INT16 nstep, double *velocity);
	BOOL	PASCAL EXPORT C5Mget_home_velocity(INT16 axis, INT16 nstep, double *velocity);
    //���� ���� ���� �˻� �� �� ���ܺ� �������� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
	BOOL	PASCAL EXPORT C5Mget_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
    // ���� ���� ���� �˻� �� �� ���ܺ� ���� �ð��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
	BOOL	PASCAL EXPORT C5Mget_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
    // ���� �࿡ ���� �˻����� ���ڴ� 'Z'�� ���� ��� �� ���� �Ѱ谪�� ����/Ȯ���Ѵ�.(Pulse) - ������ ����� �˻� ����
	BOOL	PASCAL EXPORT C5Mset_zphase_search_range(INT16 axis, INT16 pulses);
	INT16	PASCAL EXPORT C5Mget_zphase_search_range(INT16 axis);
	// ���� ��ġ�� ����(0 Position)���� �����Ѵ�. - �������̸� ���õ�.
	BOOL	PASCAL EXPORT C5Mhome_zero(INT16 axis);
	// ������ ��� ���� ���� ��ġ�� ����(0 Position)���� �����Ѵ�. - �������� ���� ���õ�
	BOOL	PASCAL EXPORT C5Mhome_zero_all(INT16 number, INT16 *axes);
	
    // ���� �����-=======================================================================================================
    // ���� ���
    // 0 bit : ���� ��� 0(Servo-On)
    // 1 bit : ���� ��� 1(ALARM Clear)
    // 2 bit : ���� ��� 2
    // 3 bit : ���� ��� 3
    // ���� �Է�
    // 0 bit : ���� �Է� 0(ORiginal Sensor)
    // 1 bit : ���� �Է� 1(Z phase)
    // 2 bit : ���� �Է� 2
    // 3 bit : ���� �Է� 3
    // On ==> ���ڴ� N24V, 'Off' ==> ���ڴ� Open(float).		

    // ���� ���� ��°��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_output(INT16 axis, UINT8 value);
	UINT8   PASCAL EXPORT C5Mget_output(INT16 axis);
    // ���� �Է� ���� Ȯ���Ѵ�.
    // '1'('On') <== ���ڴ� N24V�� �����, '0'('Off') <== ���ڴ� P24V �Ǵ� Float.
	UINT8   PASCAL EXPORT C5Mget_input(INT16 axis);
    // �ش� ���� �ش� bit�� ����� On/Off ��Ų��.
    // bitNo : 0 ~ 3.
	BOOL	PASCAL EXPORT C5Mset_output_bit(INT16 axis, UINT8 bitNo);
	BOOL	PASCAL EXPORT C5Mreset_output_bit(INT16 axis, UINT8 bitNo);
    // �ش� ���� �ش� ���� ��� bit�� ��� ���¸� Ȯ���Ѵ�.
    // bitNo : 0 ~ 3.
	BOOL	PASCAL EXPORT C5Moutput_bit_on(INT16 axis, UINT8 bitNo);
    // �ش� ���� �ش� ���� ��� bit�� ���¸� �Է� state�� �ٲ۴�.
    // bitNo : 0 ~ 3. 
	BOOL	PASCAL EXPORT C5Mchange_output_bit(INT16 axis, UINT8 bitNo, UINT8 state);
    // �ش� ���� �ش� ���� �Է� bit�� ���¸� Ȯ�� �Ѵ�.
    // bitNo : 0 ~ 3.
	BOOL	PASCAL EXPORT C5Minput_bit_on(INT16 axis, UINT8 bitNo);

    // �ܿ� �޽� clear-===================================================================================================
    // �ش� ���� ������ �ܿ� �޽� Clear ����� ��� ���θ� ����/Ȯ���Ѵ�.
    // CLR ��ȣ�� Default ��� ==> ���ڴ� Open�̴�.
	BOOL	PASCAL EXPORT C5Mset_crc_mask(INT16 axis, INT16 mask);
	UINT8	PASCAL EXPORT C5Mget_crc_mask(INT16 axis);
    // �ش� ���� �ܿ� �޽� Clear ����� Active level�� ����/Ȯ���Ѵ�.
    // Default Active level ==> '1' ==> ���ڴ� N24V
	BOOL	PASCAL EXPORT C5Mset_crc_level(INT16 axis, INT16 level);
	UINT8	PASCAL EXPORT C5Mget_crc_level(INT16 axis);
    //�ش� ���� -Emeregency End limit�� ���� Clear��� ��� ������ ����/Ȯ���Ѵ�.    
	BOOL	PASCAL EXPORT C5Mset_crc_nelm_mask(INT16 axis, INT16 mask);
	UINT8	PASCAL EXPORT C5Mget_crc_nelm_mask(INT16 axis);
    // �ش� ���� -Emeregency End limit�� Active level�� ����/Ȯ���Ѵ�. set_nend_limit_level�� �����ϰ� �����Ѵ�.    
	BOOL	PASCAL EXPORT C5Mset_crc_nelm_level(INT16 axis, INT16 level);
	UINT8	PASCAL EXPORT C5Mget_crc_nelm_level(INT16 axis);
    // �ش� ���� +Emeregency End limit�� ���� Clear��� ��� ������ ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_crc_pelm_mask(INT16 axis, INT16 mask);
	UINT8	PASCAL EXPORT C5Mget_crc_pelm_mask(INT16 axis);
    // �ش� ���� +Emeregency End limit�� Active level�� ����/Ȯ���Ѵ�. set_nend_limit_level�� �����ϰ� �����Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_crc_pelm_level(INT16 axis, INT16 level);
	UINT8	PASCAL EXPORT C5Mget_crc_pelm_level(INT16 axis);
    // �ش� ���� �ܿ� �޽� Clear ����� �Է� ������ ���� ���/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_programmed_crc(INT16 axis, INT16 data);
	UINT8	PASCAL EXPORT C5Mget_programmed_crc(INT16 axis);

    // Ʈ���� ��� ======================================================================================================
    // ����/�ܺ� ��ġ�� ���Ͽ� �ֱ�/���� ��ġ���� ������ Active level�� Trigger pulse�� �߻� ��Ų��.
    // Ʈ���� ��� �޽��� Active level�� ����/Ȯ���Ѵ�.
		// trigger_level
		// 0 : 5V ���(0 V), 24V �͹̳� ���(Open); 
		// 1(default) : 5V ���(5 V), 24V �͹̳� ���(N24V).
	BOOL	PASCAL EXPORT C5Mset_trigger_level(INT16 axis, UINT8 trigger_level);
	UINT8	PASCAL EXPORT C5Mget_trigger_level(INT16 axis);
    // Ʈ���� ��ɿ� ����� ���� ��ġ�� �����Ѵ�.
		// trigger_sel
		// 0x0 : �ܺ� ��ġ External(Actual)
		// 0x1 : ���� ��ġ Internal(Command)
	BOOL	PASCAL EXPORT C5Mset_trigger_sel(INT16 axis, UINT8 trigger_sel);
	UINT8	PASCAL EXPORT C5Mget_trigger_sel(INT16 axis);
    // Ʈ���� �޽����� ����/Ȯ���Ѵ�.
		// time
		// 0Fh : 64 msec
		// 0Eh : 32 msec
		// 0Dh : 16 mSec
		// ..
		// 04h : 0.0625 msec
	BOOL	PASCAL EXPORT C5Mset_trigger_time(INT16 axis, UINT8 time);
	UINT8	PASCAL EXPORT C5Mget_trigger_time(INT16 axis);
    // ���� ���� Ʈ���� ����� ��� ���θ� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_trigger_enable(INT16 axis, UINT8 ena_status);
	UINT8	PASCAL EXPORT C5Mis_trigger_enabled(INT16 axis);
    // ���� �࿡ Ʈ���� �߻��� ���ͷ�Ʈ�� �߻��ϵ��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_trigger_interrupt_enable(INT16 axis, UINT8 ena_int);
	UINT8	PASCAL EXPORT C5Mis_trigger_interrupt_enabled(INT16 axis);

    // ��ġ �񱳱� ���� �Լ��� ==========================================================================================
	// Internal(Command) comparator���� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_internal_comparator_position(INT16 axis, double position);
	double	PASCAL EXPORT C5Mget_internal_comparator_position(INT16 axis);
	// External(Encoder) comparator���� ����/Ȯ���Ѵ�.
	void	PASCAL EXPORT C5Mset_external_comparator_position(INT16 axis, double position);
	double	PASCAL EXPORT C5Mget_external_comparator_position(INT16 axis);

    // ���ͷ�Ʈ ���� �Լ��� ==========================================================================================
	// �������� ���ͷ�Ʈ �߻� ��� ����/Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT C5Mset_motiondone_interrupt_enable(INT16 axis, UINT8 ena_int);
	UINT8	PASCAL EXPORT C5Mis_motiondone_interrupt_enabled(INT16 axis);

    // �����ڵ� �б� �Լ��� =============================================================================================
	// ������ �����ڵ带 �д´�.
	INT16	PASCAL EXPORT C5Mget_error_code();
	// �����ڵ��� ������ ���ڷ� ��ȯ�Ѵ�.
	char*	PASCAL EXPORT C5Mget_error_msg(INT16 ErrorCode);
	
#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_CAMC_5M_H__
