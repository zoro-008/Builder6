#ifndef __AXT_CAMC_IP_H__
#define __AXT_CAMC_IP_H__

#include "AxtLIBDef.h"
#include "CAMCIPDef.h"

/*------------------------------------------------------------------------------------------------*
    AXTCAMCIP Library - CAMC-IP Motion module
    ������ǰ
        SMC-2V03(ver 2.0 ����) - CAMC-IP 2��
    ���̺귯�� ���� : v2.0
    ������ ������ : 2005. 12. 28.
    ������� ���� : Tel. 031-436-2180(���������� ���������)
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

    /// ���� �ʱ�ȭ �Լ���        -======================================================================================
    // CAMC-IP�� ������ ���(SMC-2V03)�� �˻��Ͽ� �ʱ�ȭ�Ѵ�. 
        //  reset    : 1(TRUE) = �ʱ�ȭ�� ��������(ī���� ��)�� �ʱ�ȭ�Ѵ�
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
    BOOL   PASCAL EXPORT InitializeCAMCIP(BOOL reset);
    // CAMC-IP ����� ����� ���������� Ȯ���Ѵ�
		// ���ϰ� :  1(TRUE) = CAMC-IP ����� ��� �����ϴ�.
    BOOL   PASCAL EXPORT CIPIsInitialized(void);
    // CAMC-IP�� ������ ����� ����� �����Ѵ�
    void   PASCAL EXPORT CIPStopService();

    /// ���� ���� ���� �Լ���        -===================================================================================
    // ������ �ּҿ� ������ ���̽������� ��ȣ�� �����Ѵ�. ������ -1�� �����Ѵ�
    INT16    PASCAL EXPORT CIPget_boardno(UINT32 address);
    // ���̽������� ������ �����Ѵ�
    INT16    PASCAL EXPORT CIPget_numof_boards();
    // ������ ���̽����忡 ������ ���� ������ �����Ѵ�
    INT16    PASCAL EXPORT CIPget_numof_axes(INT16 nBoardNo);
    // ���� ������ �����Ѵ�
    INT16    PASCAL EXPORT CIPget_total_numof_axis();
    // ������ ���̽������ȣ�� ����ȣ�� �ش��ϴ� ���ȣ�� �����Ѵ�
    INT16    PASCAL EXPORT CIPget_axisno(INT16 nBoardNo, INT16 nModuleNo);
    // ������ ���� ������ �����Ѵ�.
		// nBoardNo : �ش� ���� ������ ���̽������� ��ȣ.
		// nModuleNo: �ش� ���� ������ ����� ���̽� ��峻 ��� ��ġ(0~3)
		// bModuleID: �ش� ���� ������ ����� ID : SMC-2V03(0x05)
		// nAxisPos : �ش� ���� ������ ����� ù��°���� �ι�° ������ ����.(0 : ù��°, 1 : �ι�°)
    BOOL    PASCAL EXPORT CIPget_axis_info(INT16 nAxisNo, INT16 *nBoardNo, INT16 *nModuleNo, UINT8 *bModuleID, INT16 *nAxisPos);

    // ��ü �ý��۳��� ������ ���� ��� ��ȣ�� �����Ѵ�.
    INT16    PASCAL EXPORT CIPget_axisno_2_moduleno(INT16 axisno);

    /// ���� ���� �Լ���        -========================================================================================
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
    BOOL    PASCAL EXPORT CIPload_parameter(INT16 axis, char *nfilename);
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
    BOOL    PASCAL EXPORT CIPsave_parameter(INT16 axis, char *nfilename); 
    // ��� ���� �ʱⰪ�� ������ ���Ͽ��� �о �����Ѵ�
    BOOL    PASCAL EXPORT CIPload_parameter_all(char *nfilename);
    // ��� ���� �ʱⰪ�� ������ ���Ͽ� �����Ѵ�
    BOOL    PASCAL EXPORT CIPsave_parameter_all(char *nfilename); 

    // ���ͷ�Ʈ �Լ���   -================================================================================================
    //(���ͷ�Ʈ�� ����ϱ� ���ؼ��� 
    //CIPWriteInterruptMask, CIPKeSetInterruptOutEnableMode, CIPSetWindowMessage������ ���� �Ǿ�� �Ѵ�. ���� �ҽ� ����)
    //Window message & procedure
    //    hWnd    : ������ �ڵ�, ������ �޼����� ������ ���. ������� ������ NULL�� �Է�.
    //    wMsg    : ������ �ڵ��� �޼���, ������� �ʰų� ����Ʈ���� ����Ϸ��� 0�� �Է�.
    //    proc    : ���ͷ�Ʈ �߻��� ȣ��� �Լ��� ������, ������� ������ NULL�� �Է�.
    void   PASCAL EXPORT CIPSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_CAMCIP_INTERRUPT_PROC proc);    
    //-===============================================================================
    // ReadInterruptFlag���� ������ ���� flag������ �о� ���� �Լ�(���ͷ�Ʈ service routine���� ���ͷ��� �߻� ������ �Ǻ��Ѵ�.)
    // bank : 
    // 0x0 : ���ͷ�Ʈ�� �߻� �Ͽ����� �߻��ϴ� ���ͷ�Ʈ flag register bank1(CAMC-IP �� INTFLAG1 ����.)
    // 0x1 : ���ͷ�Ʈ�� �߻� �Ͽ����� �߻��ϴ� ���ͷ�Ʈ flag register bank2(CAMC-IP �� INTFLAG2 ����.) 
    UINT32  PASCAL EXPORT CIPread_interrupt_flag(INT16 axis, UINT32 bank);
    // �ش� ���� ���ͷ��� �߻� ������ �����Ѵ�. bank������ CAMC-IP�� INTMASK1, INTMASK2 ������ ����.
    void   PASCAL EXPORT CIPKeSetInterruptOutEnableMode(INT16 axis, UINT32 bank1, UINT32 bank2);
    void   PASCAL EXPORT CIPKeResetInterruptOutEnableMode(INT16 axis);
    // �ش� SMC-2V03 ������ ���ͷ��� ��� ���θ� �����Ѵ�. �ش� ���� get_axis_info�� ����Ͽ� �Է� ���� ã�� �ִ´�.
    void   PASCAL EXPORT CIPWriteInterruptMask(INT16 nBoardNo, INT16 nModulePos, UINT8 Mask);
    UINT8  PASCAL EXPORT CIPReadInterruptMask(INT16 nBoardNo, INT16 nModulePos);
		// �ش� ����� ���ͷ�Ʈ Flag�� Ȯ���Ѵ�.
		// 0 bit : �ش� ����� 0��(x��)�� ���ͷ�Ʈ�� �߻� ����.
		// 1 bit : �ش� ����� 1��(y��)�� ���ͷ�Ʈ�� �߻� ����.
    UINT8  PASCAL EXPORT CIPReadInterruptFlag(INT16 nBoardNo, INT16 nModulePos);

    // ���� ���� �ʱ�ȭ �Լ���        -==================================================================================
	// ����Ŭ�� ����( ��⿡ ������ Oscillator�� ����� ��쿡�� ����)
    void    PASCAL EXPORT CIPKeSetMainClk(INT16 axis, INT32 nMainClk);
    // Drive mode 1�� ����/Ȯ���Ѵ�.
		// pwm_ppls_mode : SMC-2V03������ �ش� ���� ����.
		// decelstartpoint : �����Ÿ� ���� ��� ����� ���� ��ġ ���� ��� ����(0 : �ڵ� ������, 1 : ���� ������)
		// pulseoutmethod : ��� �޽� ��� ����(typedef : PULSE_OUTPUT)
		// detecsignal : ��ȣ �˻�-1/2 ���� ��� ����� �˻� �� ��ȣ ����(typedef : DETECT_DESTINATION_SIGNAL)
    void    PASCAL EXPORT CIPset_drive_mode1(INT16 axis, 
                UINT8 pwm_ppls_mode,
                UINT8 decelstartpoint,
                UINT8 pulseoutmethod,
                UINT8 detectsignal);
	UINT16    PASCAL EXPORT CIPget_drive_mode1(INT16 axis);

    // Drive mode 2�� ����/Ȯ���Ѵ�.
		// encmethod : �Է� ���ڴ� ��� ����(typedef : EXTERNAL_COUNTER_INPUT)    
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
    // Unit/Pulse ����/Ȯ��
		// Unit/Pulse : 1 pulse�� ���� system�� �̵��Ÿ��� ���ϸ�, �̶� Unit�� ������ ����ڰ� ���Ƿ� ������ �� �ִ�.
		// Ex) Ball screw pitch : 10mm, ���� 1ȸ���� �޽��� : 10000 ==> Unit�� mm�� ������ ��� : Unit/Pulse = 10/10000.
		// ���� unitperpulse�� 0.001�� �Է��ϸ� ��� ��������� mm�� ������. 
		// Ex) Linear motor�� ���ش��� 1 pulse�� 2 uM. ==> Unit�� mm�� ������ ��� : Unit/Pulse = 0.002/1.
    void   PASCAL EXPORT CIPset_moveunit_perpulse(INT16 axis, double unitperpulse);
    double PASCAL EXPORT CIPget_moveunit_perpulse(INT16 axis);
    // pulse/Unit ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_movepulse_perunit(INT16 axis, double pulseperunit);
    double PASCAL EXPORT CIPget_movepulse_perunit(INT16 axis);
    // ���� �ӵ� ����/Ȯ���Ѵ�.(Unit/Sec)
    void   PASCAL EXPORT CIPset_startstop_speed(INT16 axis, double velocity);
    double PASCAL EXPORT CIPget_startstop_speed(INT16 axis);
    // �ְ� �ӵ� ����/Ȯ���Ѵ�.(Unit/Sec) - ���� system�� �ְ� �ӵ��� �����Ѵ�. �����ӵ��� �ִ�ӵ��� ���ѵȴ�.
		// Unit/Pulse ������ ���ۼӵ� ���� ���Ŀ� �����Ѵ�.
		// ������ �ְ� �ӵ� �̻����δ� ������ �Ҽ� �����Ƿ� �����Ѵ�.
    BOOL   PASCAL EXPORT CIPset_max_speed(INT16 axis, double max_velocity);
	double PASCAL EXPORT CIPget_max_speed(INT16 axis);
    // SW�� ����� ���� ����/Ȯ���Ѵ�. �̰����� S-Curve ������ percentage�� ���� �����ϴ�.
    void   PASCAL EXPORT CIPset_s_rate(INT16 axis, double accel_percent, double decel_percent);
    void   PASCAL EXPORT CIPget_s_rate(INT16 axis, double *accel_percent, double *decel_percent);
    // ���� ������ ��忡�� �ܷ� �޽��� ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_slowdown_rear_pulse(INT16 axis, UINT32 ulData);
    UINT32 PASCAL EXPORT CIPget_slowdown_rear_pulse(INT16 axis);
    // CAMC_IP�� interrupt mask bit 0�� �����Ͽ� ��� �Ϸῡ ���� ���ͷ�Ʈ�� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_motiondone_interrupt_enable(INT16 axis, UINT8 ena_int);
    UINT8  PASCAL EXPORT CIPis_motiondone_interrupt_enabled(INT16 axis);
    // ���� ���� ���� ���� ������ ���� ����� ����/Ȯ���Ѵ�.
		// 0x0 : �ڵ� ������.
		// 0x1 : ���� ������.
    BOOL   PASCAL EXPORT CIPset_decel_point(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_decel_point(INT16 axis);    

    // ���� ���� Ȯ�� �Լ���        -=====================================================================================
    // ���� ���� �޽� ��� ���¸� Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPin_motion(INT16 axis);
    // ���� ���� �޽� ��� ���� ���¸� Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPmotion_done(INT16 axis);
    // ���� ���� �������� ���� ��µ� �޽� ī���� ���� Ȯ���Ѵ�.(Pulse)
    INT32  PASCAL EXPORT CIPget_drive_pulse_counts(INT16 axis);
    // ���� ���� DriveStatus �������͸� Ȯ���Ѵ�.
    UINT32 PASCAL EXPORT CIPget_drive_status(INT16 axis);
    // ���� ���� EndStatus �������͸� Ȯ���Ѵ�.
		// End Status Bit�� �ǹ�
		// 15bit : Interpolation drvie�� ���� ����
		// 14bit : Soft Limit�� ���� ����
		// 13bit : Limit(PELM, NELM, PSLM, NSLM) ��ȣ�� ���� ����
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
    UINT16 PASCAL EXPORT CIPget_end_status(INT16 axis);
    // ���� ���� Mechanical �������͸� Ȯ���Ѵ�.
		// Mechanical Signal Bit�� �ǹ�
		// 14bit : MODE8_16 ��ȣ �Է� Level
		// 13bit : SYNC ��ȣ �Է�  Level
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
    UINT16 PASCAL EXPORT CIPget_mechanical_signal(INT16 axis);
    // ���� ����  ���� �ӵ��� �о� �´�.(Unit/Sec)
    double PASCAL EXPORT CIPget_velocity(INT16 axis);
    // ���� ���� Command position�� Actual position�� ���� Ȯ���Ѵ�.
    double PASCAL EXPORT CIPget_error(INT16 axis);
    // ���� ���� ���� ����̺��� �̵� �Ÿ��� Ȯ�� �Ѵ�. (Unit)
    double PASCAL EXPORT CIPget_drivedistance(INT16 axis);

    //Encoder �Է� ��� ���� �Լ���        -=============================================================================
    // ���� ���� Encoder �Է� ����� ����/Ȯ���Ѵ�.
		// method : typedef(EXTERNAL_COUNTER_INPUT)
		// UpDownMode = 0x0    // Up/Down
		// Sqr1Mode   = 0x1    // 1ü��
		// Sqr2Mode   = 0x2    // 2ü��
		// Sqr4Mode   = 0x3    // 4ü��
    BOOL   PASCAL EXPORT CIPset_enc_input_method(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_enc_input_method(INT16 axis);
    // ���� ���� �ܺ� ��ġ counter clear�� ����� ����/Ȯ���Ѵ�.
		// method : CAMC-IP chip �޴��� EXTCNTCLR �������� ����.
    BOOL   PASCAL EXPORT CIPset_enc2_input_method(INT16 axis, UINT8 method);
    UINT16 PASCAL EXPORT CIPget_enc2_input_method(INT16 axis);
    // ���� ���� �ܺ� ��ġ counter�� count ����� ����/Ȯ���Ѵ�.
		// reverse :
		// TRUE  : �Է� ���ڴ��� �ݴ�Ǵ� �������� count�Ѵ�.
		// FALSE : �Է� ���ڴ��� ���� ���������� count�Ѵ�.
    BOOL   PASCAL EXPORT CIPset_enc_reverse(INT16 axis, UINT8 reverse);
    BOOL   PASCAL EXPORT CIPget_enc_reverse(INT16 axis);

    //�޽� ��� ��� �Լ���        -=====================================================================================
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
    BOOL   PASCAL EXPORT CIPset_pulse_out_method(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_pulse_out_method(INT16 axis);

    // ��ġ Ȯ�� �� ��ġ �� ���� �Լ��� -===============================================================================
    // �ܺ� ��ġ ���� �����Ѵ�. ������ ���¿��� �ܺ� ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
    void   PASCAL EXPORT CIPset_actual_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_actual_position(INT16 axis);
	// �ܺ� ��ġ �񱳱⿡ ���� ����/Ȯ���Ѵ�.(position = Unit)
    void   PASCAL EXPORT CIPset_actual_comparator_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_actual_comparator_position(INT16 axis);
    // ���� ��ġ ���� �����Ѵ�. ������ ���¿��� ���� ��ġ�� Ư�� ������ ����/Ȯ���Ѵ�.(position = Unit)
    void   PASCAL EXPORT CIPset_command_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_command_position(INT16 axis);
    // ���� ��ġ �񱳱⿡ ���� ����/Ȯ���Ѵ� (position = Unit)
    void   PASCAL EXPORT CIPset_command_comparator_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_command_comparator_position(INT16 axis);

    // ���� ����̹� ��� ��ȣ ���� �Լ���-===============================================================================
   	// ���� Enable��� ��ȣ�� Active Level�� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_servo_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_servo_level(INT16 axis);
    // ���� Enable(On) / Disable(Off)�� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_servo_enable(INT16 axis, UINT8 state);
    UINT8  PASCAL EXPORT CIPget_servo_enable(INT16 axis);

    // ���� ����̹� �Է� ��ȣ ���� �Լ���-===============================================================================
	// ���� ��ġ�����Ϸ�(inposition)�Է� ��ȣ�� ��������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_inposition_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_inposition_enable(INT16 axis);
   	// ���� ��ġ�����Ϸ�(inposition)�Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_inposition_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_inposition_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_inposition_switch(INT16 axis);
    BOOL   PASCAL EXPORT CIPin_position(INT16 axis);
	// ���� �˶� �Է½�ȣ ����� ��������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_alarm_enable(INT16 axis, UINT8 use);
	UINT8  PASCAL EXPORT CIPget_alarm_enable(INT16 axis);
	// ���� �˶� �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_alarm_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_alarm_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_alarm_switch(INT16 axis);

	//MARK ��ȣ(Sensor positioning drive�� ���)
	BOOL   PASCAL EXPORT CIPset_mark_signal_level(INT16 axis, UINT8 level);
	UINT8  PASCAL EXPORT CIPget_mark_signal_level(INT16 axis);
	UINT8  PASCAL EXPORT CIPget_mark_signal_switch(INT16 axis);
	BOOL   PASCAL EXPORT CIPset_mark_signal_enable(INT16 axis, UINT8 use);
	UINT8  PASCAL EXPORT CIPget_mark_signal_enable(INT16 axis);

    // ����Ʈ ��ȣ ���� �Լ���-===========================================================================================
    // ������ ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_end_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_end_limit_enable(INT16 axis);
	// -������ ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_nend_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_nend_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_nend_limit_switch(INT16 axis);
	// +������ ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_pend_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_pend_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_pend_limit_switch(INT16 axis);
    // �������� ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_slow_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_slow_limit_enable(INT16 axis);
	// -�������� ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_nslow_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_nslow_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_nslow_limit_switch(INT16 axis);
	// +�������� ����Ʈ �Է� ��ȣ�� Active Level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_pslow_limit_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_pslow_limit_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_pslow_limit_switch(INT16 axis);
    // +������ ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.(������ ����Ʈ�� ���Ͽ� +�� -�� ���Ͽ� ���� ���� �����ϴ�.)
    BOOL   PASCAL EXPORT CIPset_pend_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_pend_limit_enable(INT16 axis);
    // -������ ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_nend_limit_enable(INT16 axis, UINT8 use);    
    UINT8  PASCAL EXPORT CIPget_nend_limit_enable(INT16 axis);
    // +�������� ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.(�������� ����Ʈ�� ���Ͽ� +�� -�� ���Ͽ� ���� ���� �����ϴ�.)
    BOOL   PASCAL EXPORT CIPset_pslow_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_pslow_limit_enable(INT16 axis);
    // -�������� ����Ʈ ��� ��������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_nslow_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_nslow_limit_enable(INT16 axis);

    // ����Ʈ���� ����Ʈ ���� �Լ���-=====================================================================================
    // ����Ʈ���� ����Ʈ ��������� ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_soft_limit_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_soft_limit_enable(INT16 axis);
    // ����Ʈ���� ����Ʈ ���� ������ġ������ ����/Ȯ���Ѵ�.
		// sel :
		// 0x0 : ������ġ�� ���Ͽ� ����Ʈ���� ����Ʈ ��� ����.
		// 0x1 : �ܺ���ġ�� ���Ͽ� ����Ʈ���� ����Ʈ ��� ����.
    void   PASCAL EXPORT CIPset_soft_limit_sel(INT16 axis, UINT8 sel);
    UINT8  PASCAL EXPORT CIPget_soft_limit_sel(INT16 axis);
    // ����Ʈ���� ����Ʈ �߻��� ���� ��带 ����/Ȯ���Ѵ�.
		// mode :
		// 0x0 : ����Ʈ���� ����Ʈ ��ġ���� ������ �Ѵ�.
		// 0x1 : ����Ʈ���� ����Ʈ ��ġ���� �������� �Ѵ�.
    void   PASCAL EXPORT CIPset_soft_limit_stopmode(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_soft_limit_stopmode(INT16 axis);
    // ����Ʈ���� ����Ʈ -��ġ�� ����/Ȯ���Ѵ�.(position = Unit)
    void   PASCAL EXPORT CIPset_soft_nlimit_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_soft_nlimit_position(INT16 axis);
    // ����Ʈ���� ����Ʈ +��ġ�� ����/Ȯ�� �Ѵ�.(position = Unit)
    void   PASCAL EXPORT CIPset_soft_plimit_position(INT16 axis, double position);
    double PASCAL EXPORT CIPget_soft_plimit_position(INT16 axis);

    // ������� ��ȣ-=====================================================================================================
    // ESTOP, SSTOP ��ȣ ��������� ����/Ȯ���Ѵ�.(Emergency stop, Slow-Down stop)
    BOOL   PASCAL EXPORT CIPset_emg_signal_enable(INT16 axis, UINT8 use);
    UINT8  PASCAL EXPORT CIPget_emg_signal_enable(INT16 axis);
    // SSTOP �Է½�ȣ�� Active level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_sstop_signal_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_sstop_signal_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_sstop_signal_switch(INT16 axis);
    // ESTOP �Է½�ȣ�� Active level�� ����/Ȯ��/����Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_estop_signal_level(INT16 axis, UINT8 level);
    UINT8  PASCAL EXPORT CIPget_estop_signal_level(INT16 axis);
    UINT8  PASCAL EXPORT CIPget_estop_signal_switch(INT16 axis);

    //���� ���� �Ÿ� ����-===============================================================================================
    //start_** : ���� �࿡�� ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�(Blocking).
    //*r*_*    : ���� �࿡�� �Էµ� �Ÿ���ŭ(�����ǥ)�� �̵��Ѵ�. "*r_*�� ������ �Էµ� ��ġ(������ǥ)�� �̵��Ѵ�.
    //*s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    //*a*_*    : ������ �ӵ� �����ӵ��� ���Ī���� ����Ѵ�. ���ӷ� �Ǵ� ���� �ð���  ���ӷ� �Ǵ� ���� �ð��� ���� �Է¹޴´�.
    //*_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.
    //�Է� ����: velocity(Unit/Sec), acceleration/deceleration(Unit/Sec^2), acceltime/deceltime(Sec), position(Unit)

	// ��Ī �����޽�(Pulse Drive), ��ٸ��� ���� �Լ�, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
    UINT16 PASCAL EXPORT CIPmove(INT16 axis, double position, double velocity, double acceleration);
    UINT16 PASCAL EXPORT CIPmove_ex(INT16 axis, double position, double velocity, double acceltime);
    UINT16 PASCAL EXPORT CIPr_move(INT16 axis, double distance, double velocity, double acceleration);
    UINT16 PASCAL EXPORT CIPr_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
	BOOL   PASCAL EXPORT CIPstart_move(INT16 axis, double position, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_move_ex(INT16 axis, double position, double velocity, double acceltime);
    BOOL   PASCAL EXPORT CIPstart_r_move(INT16 axis, double distance, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_r_move_ex(INT16 axis, double distance, double velocity, double acceltime);

	// ���Ī �����޽�(Pulse Drive), ��ٸ��� ���� �Լ�, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
    UINT16 PASCAL EXPORT CIPa_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPa_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
    UINT16 PASCAL EXPORT CIPra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
    BOOL   PASCAL EXPORT CIPstart_a_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_a_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
    BOOL   PASCAL EXPORT CIPstart_ra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_ra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
 	// ��Ī �����޽�(Pulse Drive), S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
    UINT16 PASCAL EXPORT CIPs_move(INT16 axis, double position, double velocity, double acceleration);
    UINT16 PASCAL EXPORT CIPs_move_ex(INT16 axis, double position, double velocity, double acceltime);
    UINT16 PASCAL EXPORT CIPrs_move(INT16 axis, double distance, double velocity, double acceleration);	
    UINT16 PASCAL EXPORT CIPrs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�)
    BOOL   PASCAL EXPORT CIPstart_s_move(INT16 axis, double position, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_s_move_ex(INT16 axis, double position, double velocity, double acceltime);
    BOOL   PASCAL EXPORT CIPstart_rs_move(INT16 axis, double distance, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_rs_move_ex(INT16 axis, double distance, double velocity, double acceltime);
	// ���Ī �����޽�(Pulse Drive), S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
	UINT16 PASCAL EXPORT CIPas_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPas_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime);
    UINT16 PASCAL EXPORT CIPras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    UINT16 PASCAL EXPORT CIPras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);
	// Non Blocking�Լ� (�������� ��� ���õ�), jerk���(���� : �ۼ�Ʈ) ���������� S�� �̵�����.	
	BOOL   PASCAL EXPORT CIPstart_as_move(INT16 axis, double position, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_as_move2(INT16 axis, double position, double velocity, double acceleration, double deceleration, double jerk);
    BOOL   PASCAL EXPORT CIPstart_as_move_ex(INT16 axis, double position, double velocity, double acceltime, double deceltime); 
    BOOL   PASCAL EXPORT CIPstart_ras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPstart_ras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime);

	// MARK, ��Ī Sensor positioning ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL   PASCAL EXPORT CIPstart_pr_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_pr_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī Sensor positioning ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL   PASCAL EXPORT CIPstart_pra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_pra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// Sensor positioning ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16 PASCAL EXPORT CIPpr_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPpr_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī Sensor positioning ��ٸ��� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16 PASCAL EXPORT CIPpra_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPpra_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK, ��Ī Sensor positioning S���� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL   PASCAL EXPORT CIPstart_prs_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_prs_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī Sensor positioning S���� ����, �����ǥ, ������/���ӽð�(Sec)
	BOOL   PASCAL EXPORT CIPstart_pras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	BOOL   PASCAL EXPORT CIPstart_pras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	// MARK, Sensor positioning S���� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16 PASCAL EXPORT CIPprs_move(INT16 axis, double distance, double velocity, double acceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPprs_move_ex(INT16 axis, double distance, double velocity, double acceltime, UINT8 drive);
	// MARK, ���Ī Sensor positioning S���� ����, �����ǥ, ������/���ӽð�(Sec). ������ �Ϸ�ɶ����� ���
	UINT16 PASCAL EXPORT CIPpras_move(INT16 axis, double distance, double velocity, double acceleration, double deceleration, UINT8 drive);
	UINT16 PASCAL EXPORT CIPpras_move_ex(INT16 axis, double distance, double velocity, double acceltime, double deceltime, UINT8 drive);
	
	// ��Ī ���� �޽�(Pulse Drive), S���� ����, �����ǥ, ������,
	// Non Blocking (�������� ��� ���õ�), ���� ��ġ�� �������� over_distance���� over_velocity�� �ӵ��� ���� �Ѵ�.
	BOOL   PASCAL EXPORT CIPstart_rs_move_override(INT16 axis, double distance, double velocity, double acceleration, double over_distance, double over_velocity, BOOL Target);

    //���� ���� ����-====================================================================================================
    // ���� �����ӵ� �� �ӵ��� ���� ������ �߻����� ������ ���������� �����Ѵ�.
    //*s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    //*a*_*    : ������ �ӵ� �����ӵ��� ���Ī���� ����Ѵ�. ���ӷ� �Ǵ� ���� �ð���  ���ӷ� �Ǵ� ���� �ð��� ���� �Է¹޴´�.
    //*_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.
 
	// ���ӵ� ��ٸ��� ���� �Լ���, ������/���ӽð�(_ex)(�ð�����:Sec) - �������� ��쿡�� �ӵ��������̵�
	// ��Ī ������ �����Լ�
	BOOL   PASCAL EXPORT CIPv_move(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPv_move_ex(INT16 axis, double velocity, double acceltime);
	// ���Ī ������ �����Լ�
    BOOL   PASCAL EXPORT CIPv_a_move(INT16 axis, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPv_a_move_ex(INT16 axis, double velocity, double acceltime, double deceltime);
	// ���ӵ� S���� ���� �Լ���, ������/���ӽð�(_ex)(�ð�����:Sec) - �������� ��쿡�� �ӵ��������̵�
	// ��Ī ������ �����Լ�
    BOOL   PASCAL EXPORT CIPv_s_move(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPv_s_move_ex(INT16 axis, double velocity, double acceltime);
	// ���Ī ������ �����Լ�
    BOOL   PASCAL EXPORT CIPv_as_move(INT16 axis, double velocity, double acceleration, double deceleration);
    BOOL   PASCAL EXPORT CIPv_as_move_ex(INT16 axis, double velocity, double acceltime, double deceltime);
        
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
	BOOL   PASCAL EXPORT CIPstart_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
    BOOL   PASCAL EXPORT CIPstart_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
    // ��ȣ����1(Signal search 1) S���� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
	BOOL   PASCAL EXPORT CIPstart_s_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
    BOOL   PASCAL EXPORT CIPstart_s_signal_search1_ex(INT16 axis, double velocity, double acceltime, UINT8 detect_signal);
    // ��ȣ����2(Signal search 2) ��ٸ��� ����, ������ ����
	BOOL   PASCAL EXPORT CIPstart_signal_search2(INT16 axis, double velocity, UINT8 detect_signal);

    // MPG(Manual Pulse Generation) ���� ����-===========================================================================
    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� ��带 ����/Ȯ���Ѵ�.
		// mode
		// 0x1 : Slave �������, �ܺ� Differential ��ȣ�� ���� ���
		// 0x2 : ���� �޽� ����, �ܺ� �Է� ��ȣ�� ���� ���� �޽� ���� ����
		// 0x4 : ���� ���� ���, �ܺ� ���� �Է� ��ȣ�� Ư�� ���� ���� ����
    BOOL   PASCAL EXPORT CIPset_mpg_drive_mode(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_mpg_drive_mode(INT16 axis);
	// ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� �޽� ������ ����ϴ� �Ÿ� ������ ����/Ȯ���Ѵ�.
	BOOL   PASCAL EXPORT CIPset_mpg_position(INT16 axis, double position);	//(position = Unit)
	double PASCAL EXPORT CIPget_mpg_position(INT16 axis);

    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� ���� ������带 ����/Ȯ���Ѵ�.
		// mode
		// 0x0 : �ܺ� ��ȣ�� ���� ���� ����
		// 0x1 : ����ڿ� ���� ������ �������� ����
    BOOL   PASCAL EXPORT CIPset_mpg_dir_mode(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_mpg_dir_mode(INT16 axis);
    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���� ���� ������尡 ����ڿ� ����
    // ������ �������� �����Ǿ��� �� �ʿ��� ������� ���� ���� ���� ���� ����/Ȯ���Ѵ�.
		// mode
		// 0x0 : ����� ���� ���� ������ +�� ����
		// 0x1 : ����� ���� ���� ������ -�� ����
    BOOL   PASCAL EXPORT CIPset_mpg_user_dir(INT16 axis, UINT8 mode);
    UINT8  PASCAL EXPORT CIPget_mpg_user_dir(INT16 axis);

    // ���� �࿡ MPG(Manual Pulse Generation) ����̹��� ���Ǵ� EXPP/EXMP �� �Է� ��带 �����Ѵ�.
    //  2 bit : '0' : level input(���� �Է� 4 = EXPP, ���� �Է� 5 = EXMP�� �Է� �޴´�.)
    //          '1' : Differential input(���� �Է����� EXPP, EXMP�� �Է� ����,)
    //  1~0bit: "00" : 1 phase
    //          "01" : 2 phase 1 times
    //          "10" : 2 phase 2 times
    //          "11" : 2 phase 4 times
    BOOL   PASCAL EXPORT CIPset_mpg_input_method(INT16 axis, UINT8 method);
    UINT8  PASCAL EXPORT CIPget_mpg_input_method(INT16 axis);
    
    // MPG(Manual Pulse Generation) ���� -===============================================================================
	// ������ �ӵ��� ��ٸ��� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
    BOOL   PASCAL EXPORT CIPstart_mpg(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_mpg_ex(INT16 axis, double velocity, double acceltime);
	// ������ �ӵ��� S���� ����, ������/���ӽð�(_ex)(�ð�����:Sec)
    BOOL   PASCAL EXPORT CIPstart_s_mpg(INT16 axis, double velocity, double acceleration);
    BOOL   PASCAL EXPORT CIPstart_s_mpg_ex(INT16 axis, double velocity, double acceltime);

    // �������̵�(������)-================================================================================================
    // ���� ���� �Ÿ� ������ ���� ���۽������� �Է��� ��ġ(������ġ)�� ������ �ٲ۴�.
    BOOL   PASCAL EXPORT CIPposition_override(INT16 axis, double overrideposition);
    // ���� ���� �Ÿ� ������ ���� ���۽������� �Է��� �Ÿ�(�����ġ)�� ������ �ٲ۴�.    
    BOOL   PASCAL EXPORT CIPposition_r_override(INT16 axis, double overridedistance);
    // ������ ���� �ʱ� ������ �ӵ��� �ٲ۴�.(set_max_speed > dbVel > set_startstop_speed)
    BOOL   PASCAL EXPORT CIPvelocity_override(INT16 axis, double dbVel);
    // ���� ���� ������ ����Ǳ� �� �Էµ� overrideposition���� �ּ� ��� �޽�(dec_pulse) �̻��� ��� override ������ �Ѵ�.
    BOOL   PASCAL EXPORT CIPposition_override2(INT16 axis, double overrideposition, double dec_pulse);
    // ���� �࿡ ����/���� ���� ������ ������ ���������� �ӵ� override ������ �Ѵ�.
    BOOL   PASCAL EXPORT CIPvelocity_override2(INT16 axis, double velocity, double acceleration, double deceleration, double jerk);
    
    // ���� ���� Ȯ��-====================================================================================================
    // ���� ���� ������ ����� ������ ��ٸ� �� �Լ��� �����.
    UINT16 PASCAL EXPORT CIPwait_for_done(INT16 axis);
    
    // ���� ���� ����-====================================================================================================
    // ���� ���� �������Ѵ�.
    BOOL   PASCAL EXPORT CIPset_e_stop(INT16 axis);
    // ���� ���� ������ �������� �����Ѵ�.
    BOOL   PASCAL EXPORT CIPset_stop(INT16 axis);
    // ���� ���� �Էµ� �������� �����Ѵ�.
    BOOL   PASCAL EXPORT CIPset_stop_decel(INT16 axis, double deceleration);
    // ���� ���� ���� �ð����� �����Ѵ�.
    BOOL   PASCAL EXPORT CIPset_stop_deceltime(INT16 axis, double deceltime);

    // ���� ���� �������� ����-==========================================================================================
    // Master/Slave link �Ǵ� ��ǥ�� link ���� �ϳ��� ����Ͽ��� �Ѵ�.
    // Master/Slave link ����. (�Ϲ� ���� ������ master �� ������ slave�൵ ���� �����ȴ�.)
    // Master/Slave link ����
    BOOL   PASCAL EXPORT CIPlink(INT16 master, INT16 slave, double ratio);
    // Master/Slave link ����
    BOOL   PASCAL EXPORT CIPendlink(INT16 slave);
    
    // ��ǥ�� link ����-================================================================================================
	// ���� ��ǥ�迡 �� �Ҵ� - n_axes������ŭ�� ����� ����/Ȯ���Ѵ�.(coordinate�� 1..8���� ��� ����)
	// n_axes ������ŭ�� ����� ����/Ȯ���Ѵ�. - (n_axes�� 1..4���� ��� ����)
    BOOL   PASCAL EXPORT CIPmap_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
    BOOL   PASCAL EXPORT CIPget_mapped_axes(INT16 coordinate, INT16 n_axes, INT16 *map_array);
    // ���� ��ǥ���� ���/���� ��� ����/Ȯ���Ѵ�.
  		// mode:
		// 0: �����ǥ����, 1: ������ǥ ����
    void   PASCAL EXPORT CIPset_coordinate_mode(INT16 coordinate, INT16 mode);
    INT16  PASCAL EXPORT CIPget_coordinate_mode(INT16 coordinate);
    // ���� ��ǥ���� �ӵ� �������� ����/Ȯ���Ѵ�.
		// mode:
		// 0: ��ٸ��� ����, 1: SĿ�� ����
    void   PASCAL EXPORT CIPset_move_profile(INT16 coordinate, INT16 mode);
    INT16  PASCAL EXPORT CIPget_move_profile(INT16 coordinate);	
	// ���� ��ǥ���� �ʱ� �ӵ��� ����/Ȯ���Ѵ�.
	//    void   PASCAL EXPORT CIPset_move_startstop_velocity(INT16 coordinate, double velocity);
	//    double PASCAL EXPORT CIPget_move_startstop_velocity(INT16 coordinate);
    // Ư�� ��ǥ���� �ӵ��� ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_move_velocity(INT16 coordinate, double velocity);
    double PASCAL EXPORT CIPget_move_velocity(INT16 coordinate);
    // Ư�� ��ǥ���� �������� ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_move_acceleration(INT16 coordinate, double acceleration);
	double PASCAL EXPORT CIPget_move_acceleration(INT16 coordinate);
    // Ư�� ��ǥ���� ���� �ð�(Sec)�� ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_move_acceltime(INT16 coordinate, double acceltime);
    double PASCAL EXPORT CIPget_move_acceltime(INT16 coordinate);
    // ���� ��������  ��ǥ���� ���� �����ӵ��� ��ȯ�Ѵ�.
    double PASCAL EXPORT CIPco_get_velocity(INT16 coordinate);

    // ����Ʈ���� ���� ����(���� ��ǥ�迡 ���Ͽ�)-========================================================================
	// Blocking�Լ� (������� �޽� ����� �Ϸ�� �� �Ѿ��)
    // 2, 3, 4���� �����̵��Ѵ�.
    BOOL   PASCAL EXPORT CIPmove_2(INT16 coordinate, double x, double y);
    BOOL   PASCAL EXPORT CIPmove_3(INT16 coordinate, double x, double y, double z);
    BOOL   PASCAL EXPORT CIPmove_4(INT16 coordinate, double x, double y, double z, double w);
 	// Non Blocking�Լ� (�������� ��� ���õ�)
    // 2, 3, 4���� �����̵��Ѵ�.
    BOOL   PASCAL EXPORT CIPstart_move_2(INT16 coordinate, double x, double y);
    BOOL   PASCAL EXPORT CIPstart_move_3(INT16 coordinate, double x, double y, double z);
    BOOL   PASCAL EXPORT CIPstart_move_4(INT16 coordinate, double x, double y, double z, double w);
    // ���� ��ǥ���� ������� ��� �Ϸ� üũ    
    BOOL   PASCAL EXPORT CIPco_motion_done(INT16 coordinate);
    // ���� ��ǥ���� ������ �Ϸ�ɶ� ���� ��ٸ���.
    BOOL   PASCAL EXPORT CIPco_wait_for_done(INT16 coordinate);

    // ���� ����(���� ����) : Master/Slave�� link�Ǿ� ���� ��� ������ �߻� �� �� �ִ�.-==================================
    // ���� ����� ���� �Ÿ� �� �ӵ� ���ӵ� ������ ���� ���� �����Ѵ�. ���� ���ۿ� ���� ����ȭ�� ����Ѵ�. 
    // start_** : ���� �࿡�� ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�.
    // *r*_*    : ���� �࿡�� �Էµ� �Ÿ���ŭ(�����ǥ)�� �̵��Ѵ�. "*r_*�� ������ �Էµ� ��ġ(������ǥ)�� �̵��Ѵ�.
    // *s*_*    : ������ �ӵ� ���������� "S curve"�� �̿��Ѵ�. "*s_*"�� ���ٸ� ��ٸ��� �������� �̿��Ѵ�.
    // *_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.

	// ���� �����޽�(Pulse Drive)����, ��ٸ��� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� ��� �������� �޽� ����� �Ϸ�� �� �Ѿ��)
    UINT8  PASCAL EXPORT CIPmove_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
	UINT8  PASCAL EXPORT CIPmove_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    UINT8  PASCAL EXPORT CIPr_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    UINT8  PASCAL EXPORT CIPr_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking�Լ� (�������� ���� ���õ�)
	BOOL   PASCAL EXPORT CIPstart_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    BOOL   PASCAL EXPORT CIPstart_r_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_r_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
 	// ���� �����޽�(Pulse Drive)����, S���� ����, ����/�����ǥ(r), ������/���ӽð�(_ex)(�ð�����:Sec)
	// Blocking�Լ� (������� ��� �������� �޽� ����� �Ϸ�� �� �Ѿ��)
    UINT8  PASCAL EXPORT CIPs_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
    UINT8  PASCAL EXPORT CIPs_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    UINT8  PASCAL EXPORT CIPrs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    UINT8  PASCAL EXPORT CIPrs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
	// Non Blocking�Լ� (�������� ���� ���õ�)
    BOOL   PASCAL EXPORT CIPstart_s_move_all(INT16 number, INT16 *axes, double *positions, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_s_move_all_ex(INT16 number, INT16 *axes, double *positions, double *velocities, double *acceltimes);
    BOOL   PASCAL EXPORT CIPstart_rs_move_all(INT16 number, INT16 *axes, double *distances, double *velocities, double *accelerations);
    BOOL   PASCAL EXPORT CIPstart_rs_move_all_ex(INT16 number, INT16 *axes, double *distances, double *velocities, double *acceltimes);
    //���� ��鿡 ���Ͽ� S���� ������ ���� �����ӽ��� SĿ���� ������ ����/Ȯ���Ѵ�.
    void   PASCAL EXPORT CIPset_s_rate_all(INT16 number, INT16 *axes, double *accel_percent, double *decel_percent);
    void   PASCAL EXPORT CIPget_s_rate_all(INT16 number, INT16 *axes, double *accel_percent, double *decel_percent);

    // ���� ���� Ȯ��-====================================================================================================
    // �Է� �ش� ����� ���� ���¸� Ȯ���ϰ� ������ ���� �� ���� ��ٸ���.
    UINT8  PASCAL EXPORT CIPwait_for_all(INT16 number, INT16 *axes);

    // ���� ���� ����-====================================================================================================
    // ���� ����� ���⸦ ������Ų��. - ��������� �������� ���������ʰ� �����.
    BOOL   PASCAL EXPORT CIPreset_axis_sync(INT16 nLen, INT16 *aAxis);
    // ���� ����� ���⸦ ������Ų��. - ��������� �������� ���������ʰ� �����.
    BOOL   PASCAL EXPORT CIPset_axis_sync(INT16 nLen, INT16 *aAxis);

    // ���� ���� ����-====================================================================================================
    // Ȩ ��ġ �����嵵 ����
	BOOL   PASCAL EXPORT CIPemergency_stop();        

    // -�����˻� =========================================================================================================
    // ���̺귯�� �󿡼� Thread�� ����Ͽ� �˻��Ѵ�. ���� : ������ Ĩ���� StartSto speed�� ���� �� �ִ�.
    // �����˻��� �����Ѵ�.
		// bStop:
		// 0: ��������
		// 1: ������
    BOOL   PASCAL EXPORT CIPabort_home_search(INT16 axis, UINT8 bStop);
    // �����˻��� �����Ѵ�. �����ϱ� ���� �����˻��� �ʿ��� ������ �ʿ��ϴ�.
    BOOL   PASCAL EXPORT CIPhome_search(INT16 axis);
    // �Է� ����� ���ÿ� �����˻��� �ǽ��Ѵ�.
    BOOL   PASCAL EXPORT CIPhome_search_all(INT16 number, INT16 *axes);
    // �����˻� �Ϸ� ���¸� Ȯ���Ѵ�.
		// ��ȯ��: 0: �����˻� ������, 1: �����˻� ����
    BOOL   PASCAL EXPORT CIPget_home_done(INT16 axis);
    // �ش� ����� �����˻� ���¸� Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPget_home_done_all(INT16 number, INT16 *axes);
    // ���� ���� ���� �˻� ������ ���� ���¸� Ȯ���Ѵ�.
		// ��ȯ��: 0: �����˻� ����, 1: �����˻� ����
    UINT8  PASCAL EXPORT CIPget_home_end_status(INT16 axis);
    // ���� ����� ���� �˻� ������ ���� ���¸� Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPget_home_end_status_all(INT16 number, INT16 *axes, UINT8 *endstatus);
    // ���� �˻��� �� ���ܸ��� method�� ����/Ȯ���Ѵ�.
    // Method�� ���� ���� 
    //    0 Bit ���� ��뿩�� ���� (0 : ������� ����, 1: �����
    //    1 Bit ������ ��� ���� (0 : ������, 1 : ���� �ð�)
    //    2 Bit ������� ���� (0 : ���� ����, 1 : �� ����)
    //    3 Bit �˻����� ���� (0 : cww(-), 1 : cw(+))
    // 7654 Bit detect signal ����(typedef : DETECT_DESTINATION_SIGNAL)
    BOOL   PASCAL EXPORT CIPset_home_method(INT16 axis, INT16 nstep, UINT8 *method);
    BOOL   PASCAL EXPORT CIPget_home_method(INT16 axis, INT16 nstep, UINT8 *method);
    // ���� �˻��� �� ���ܸ��� offset�� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_home_offset(INT16 axis, INT16 nstep, double *offset);
    BOOL   PASCAL EXPORT CIPget_home_offset(INT16 axis, INT16 nstep, double *offset);
    // �� ���� ���� �˻� �ӵ��� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_home_velocity(INT16 axis, INT16 nstep, double *velocity);
    BOOL   PASCAL EXPORT CIPget_home_velocity(INT16 axis, INT16 nstep, double *velocity);
    // ���� ���� ���� �˻� �� �� ���ܺ� �������� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
    BOOL   PASCAL EXPORT CIPget_home_acceleration(INT16 axis, INT16 nstep, double *acceleration);
    // ���� ���� ���� �˻� �� �� ���ܺ� ���� �ð��� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
    BOOL   PASCAL EXPORT CIPget_home_acceltime(INT16 axis, INT16 nstep, double *acceltime);
    // ���� �࿡ ���� �˻����� ���ڴ� 'Z'�� ���� ��� �� ���� �Ѱ谪�� ����/Ȯ���Ѵ�.(Pulse) - ������ ����� �˻� ����
    BOOL   PASCAL EXPORT CIPset_zphase_search_range(INT16 axis, INT16 pulses);
    INT16  PASCAL EXPORT CIPget_zphase_search_range(INT16 axis);
	// ���� ��ġ�� ����(0 Position)���� �����Ѵ� - �������̸� ���õ�.
    BOOL   PASCAL EXPORT CIPhome_zero(INT16 axis);
	// ������ ��� ���� ���� ��ġ�� ����(0 Position)���� �����Ѵ�. - �������� ���� ���õ�
    BOOL   PASCAL EXPORT CIPhome_zero_all(INT16 number, INT16 *axes);

    //������ �࿡ ����/����̺� �ӵ� �� ���������� �Է��ϰ� ������ ���� �˻� ��带 ����Ͽ�
    //CAMC-IP Ĩ ���� ���� �˻� ����̺긦 �����Ѵ�. ���� ���� ���� �� �Լ��� �����.
    //���� : CAMC-IP Ĩ ���� ���� �˻� ����� +-End limit�� Org sensor�� �ἱ �Ǿ�� �ϸ� End limit ����� Enable�Ǿ�� �Ѵ�.
    //2 bit : �����˻� ����.(0 : Plus direction, 1 : Minus direction)
    //1 bit : �����˻��� ����� �ӵ� Profile(0 : Trapzoidal, 1: S-curve)
    //0 bit : ���� ������ ���� �˻� ��ȣ ����(0 : falling edge, 1:rising edge)
    BOOL   PASCAL EXPORT CIPstart_home_search_chip(INT16 axis, double startstop, double velocity, double acceleration, UINT8 home_search_mode);
    BOOL   PASCAL EXPORT CIPstart_home_search_chip_ex(INT16 axis, double startstop, double velocity, double acceltime, UINT8 home_search_mode);
    //������ �࿡ ����/����̺� �ӵ� �� ���������� �Է��ϰ� ������ ���� �˻� ��带 ����Ͽ�
    //CAMC-IP Ĩ ���� ���� �˻� ����̺긦 �����Ѵ�. ���� ���� ���� �� �Լ��� �����.
    //3 bit : �����˻� �Ϸ��� ��ġ clear ����(0: Clear count after search, 1: Maintain count after search)
    //2 bit : �����˻� ����.(0 : Plus direction, 1 : Minus direction)
    //1 bit : �����˻��� ����� �ӵ� Profile(0 : Trapzoidal, 1: S-curve)
    //0 bit : ���� ������ ���� �˻� ��ȣ ����(0 : falling edge, 1:rising edge)
    BOOL   PASCAL EXPORT CIPhome_search_chip(INT16 axis, double startstop, double velocity, double acceleration, UINT8 home_search_mode);
    BOOL   PASCAL EXPORT CIPhome_search_chip_ex(INT16 axis, double startstop, double velocity, double acceltime, UINT8 home_search_mode);

    // ���� �����-=======================================================================================================
    // ���� ��� ���� �����Ѵ�. �ش� ���� value(0x00 ~ 0x3F)�� �ش��ϴ� ��������� 'On'��Ų��.
    // ���� ���
    // 0 bit(CAMC-IP :UIO0) : ���� ��� 0(Servo-On)
    // 1 bit(CAMC-IP :UIO1) : ���� ��� 1(ALARM Clear)
    // 2 bit(CAMC-IP :UIO2) : ���� ��� 2
    // 3 bit(CAMC-IP :UIO3) : ���� ��� 3
    // 4 bit(CAMC-IP :UIO4) : ���� ��� 4
    // 5 bit(PLD)  : ���� ��� 5
    // ���� �Է�
    // 0 bit(CAMC-IP :UIO5) : ���� �Է� 0(ORiginal Sensor)
    // 1 bit(CAMC-IP :UIO6) : ���� �Է� 1(Z phase)
    // 2 bit(CAMC-IP :UIO7) : ���� �Է� 2
    // 3 bit(CAMC-IP :UIO8) : ���� �Է� 3
    // 4 bit(CAMC-IP :UIO9) : ���� �Է� 4
    // 5 bit(CAMC-IP :UIO10) : ���� �Է� 5
    // 6 bit(CAMC-IP :UIO11) : ���� �Է� 6
    // On ==> ���ڴ� N24V, 'Off' ==> ���ڴ� Open(float).

    // ���� ��°��� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_output(INT16 axis, UINT8 value);
    UINT8  PASCAL EXPORT CIPget_output(INT16 axis);
    // ���� �Է� ���� Ȯ���Ѵ�.
    // '1'('On') <== ���ڴ� N24V�� �����, '0'('Off') <== ���ڴ� P24V �Ǵ� Float.
    UINT8  PASCAL EXPORT CIPget_input(INT16 axis);
    // �ش� ���� �ش� bit�� ����� 'On' ��Ų��.
    // bitNo : 0 ~ 5.
    BOOL   PASCAL EXPORT CIPset_output_bit(INT16 axis, UINT8 bitNo);
    // �ش� ���� �ش� ���� ����� 'Off' ��Ų��.
    BOOL   PASCAL EXPORT CIPreset_output_bit(INT16 axis, UINT8 bitNo);    
    // �ش� ���� �ش� ���� ��� bit�� ��� ���¸� Ȯ���Ѵ�.
    // bitNo : 0 ~ 5.    
    BOOL   PASCAL EXPORT CIPoutput_bit_on(INT16 axis, UINT8 bitNo);
    // �ش� ���� �ش� ���� ��� bit�� ���¸� �Է� state�� �ٲ۴�.
    // bitNo : 0 ~ 5. 
    BOOL   PASCAL EXPORT CIPchange_output_bit(INT16 axis, UINT8 bitNo, UINT8 state);
    // �ش� ���� �ش� ���� �Է� bit�� ���¸� Ȯ�� �Ѵ�.
    // bitNo : 0 ~ 6.
    BOOL   PASCAL EXPORT CIPinput_bit_on(INT16 axis, UINT8 bitNo);
    
    // �ܿ� �޽� clear-===================================================================================================
	// �ش� ���� ������ �ܿ� �޽� Clear ����� ��� ���θ� ����/Ȯ���Ѵ�.    
    // CLR ��ȣ�� Default ��� ==> ���ڴ� Open�̴�.
    BOOL   PASCAL EXPORT CIPset_crc_mask(INT16 axis, INT16 mask);
    UINT8  PASCAL EXPORT CIPget_crc_mask(INT16 axis);
    // �ش� ���� �ܿ� �޽� Clear ����� Active level�� ����/Ȯ���Ѵ�.
    // Default Active level ==> '1' ==> ���ڴ� N24V
    BOOL   PASCAL EXPORT CIPset_crc_level(INT16 axis, INT16 level);
	UINT8  PASCAL EXPORT CIPget_crc_level(INT16 axis);
    // �ش� ���� -Emeregency End limit�� ���� Clear��� ��� ������ ����/Ȯ���Ѵ�.    
    BOOL   PASCAL EXPORT CIPset_crc_nelm_mask(INT16 axis, INT16 mask);
    UINT8  PASCAL EXPORT CIPget_crc_nelm_mask(INT16 axis);
    // �ش� ���� -Emeregency End limit�� Active level�� ����/Ȯ���Ѵ�. set_nend_limit_level�� �����ϰ� �����Ѵ�.    
    BOOL   PASCAL EXPORT CIPset_crc_nelm_level(INT16 axis, INT16 level);
    UINT8  PASCAL EXPORT CIPget_crc_nelm_level(INT16 axis);
    // �ش� ���� +Emeregency End limit�� ���� Clear��� ��� ������ ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_crc_pelm_mask(INT16 axis, INT16 mask);
    UINT8  PASCAL EXPORT CIPget_crc_pelm_mask(INT16 axis);
    // �ش� ���� +Emeregency End limit�� Active level�� ����/Ȯ���Ѵ�. set_nend_limit_level�� �����ϰ� �����Ѵ�.
    BOOL   PASCAL EXPORT CIPset_crc_pelm_level(INT16 axis, INT16 level);
    UINT8  PASCAL EXPORT CIPget_crc_pelm_level(INT16 axis);    
    // �ش� ���� �ܿ� �޽� Clear ����� �Է� ������ ���� ���/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_programmed_crc(INT16 axis, INT16 data);
    UINT8  PASCAL EXPORT CIPget_programmed_crc(INT16 axis);

    //-Ʈ���� ��� ======================================================================================================
    // ����/�ܺ� ��ġ�� ���Ͽ� �ֱ�/���� ��ġ���� ������ Active level�� Trigger pulse�� �߻� ��Ų��.
    // Ʈ���� ��� �޽��� Active level�� ����/Ȯ���Ѵ�.
    // ('0' : 5V ���(0 V), 24V �͹̳� ���(Open); '1'(default) : 5V ���(5 V), 24V �͹̳� ���(N24V).
    BOOL   PASCAL EXPORT CIPset_trigger_level(INT16 axis, UINT8 trigger_level);
    UINT8  PASCAL EXPORT CIPget_trigger_level(INT16 axis);
    // Ʈ���� ��ɿ� ����� ���� ��ġ�� ����/Ȯ���Ѵ�.
		// trigger_sel
		// 0x0 : ���� ��ġ Internal(Command)
		// 0x1 : �ܺ� ��ġ External(Actual)
    BOOL   PASCAL EXPORT CIPset_trigger_sel(INT16 axis, UINT8 trigger_sel);
    UINT8  PASCAL EXPORT CIPget_trigger_sel(INT16 axis);
    // Ʈ���� �޽����� ����/Ȯ���Ѵ�.
		// time
		// 00h : 4 msec(Ĩ ��� Bypass)
		// 01h : 16 mSec
 		// 02h : 32 msec
		// 03h : 64 msec
    BOOL   PASCAL EXPORT CIPset_trigger_time(INT16 axis, UINT8 time);
    UINT8  PASCAL EXPORT CIPget_trigger_time(INT16 axis);
    // ���� �࿡ Ʈ���� �߻� ����� ����/Ȯ���Ѵ�.
		// 0x0 : Ʈ���� ���� ��ġ���� Ʈ���� �߻�, ���� ��ġ ���
		// 0x1 : Ʈ���� ��ġ���� ����� �ֱ� Ʈ���� ���
    BOOL   PASCAL EXPORT CIPset_trigger_mode(INT16 axis, UINT8 mode_sel);
    UINT8  PASCAL EXPORT CIPget_trigger_mode(INT16 axis);
    // ���� �࿡ Ʈ���� �ֱ� �Ǵ� ���� ��ġ ���� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_trigger_position(INT16 axis, double trigger_position);
    double PASCAL EXPORT CIPget_trigger_position(INT16 axis);
    // ���� ���� Ʈ���� ����� ��� ���θ� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_trigger_enable(INT16 axis, UINT8 ena_status);
    UINT8  PASCAL EXPORT CIPis_trigger_enabled(INT16 axis);    
    // ���� �࿡ Ʈ���� �߻��� ���ͷ�Ʈ�� �߻��ϵ��� ����/Ȯ���Ѵ�.
    BOOL   PASCAL EXPORT CIPset_trigger_interrupt_enable(INT16 axis, UINT8 ena_int);
    UINT8  PASCAL EXPORT CIPis_trigger_interrupt_enabled(INT16 axis);

    //-Ĩ ���� ���� ��� ����============================================================================================
    // Module ��ȣ�� CIPget_axisno_2_moduleno�� Ȯ���Ѵ�.
	// ���� ��⿡ ������ CAMC-IP ���� ����� �ʱ�ȭ �Ѵ�.
	BOOL	PASCAL EXPORT CIPset_path_move_initialize(INT16 nMouleNo);
	// ���� ��⿡ ������ CAMC-IP ���� queue�� ��� ���� ������ Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT CIPis_path_move_queue_free(INT16 nMouleNo);
	// ���� ��⿡ ������ CAMC-IP ���� queue�� �Է� ����Ÿ ������ Ȯ�� �Ѵ�..
	UINT8	PASCAL EXPORT CIPget_path_move_queue_index(INT16 nMouleNo);
    // ���� ��⿡ ������ CAMC-IP�� ���� ���� �ӵ��� ����/Ȯ���Ѵ�.
    void    PASCAL EXPORT CIPset_path_move_startstop_velocity(INT16 nMouleNo, double velocity);
    double  PASCAL EXPORT CIPget_path_move_startstop_velocity(INT16 nMouleNo);
    // ���� ��⿡ ������ CAMC-IP�� ���� ���� �ӵ��� ����/Ȯ���Ѵ�.        
    void    PASCAL EXPORT CIPset_path_move_velocity(INT16 nMouleNo, double velocity);
    double  PASCAL EXPORT CIPget_path_move_velocity(INT16 nMouleNo);
    // ���� ��⿡ ������ CAMC-IP�� ���� ���� �����ӷ��� ����/Ȯ���Ѵ�.
    void    PASCAL EXPORT CIPset_path_move_acceleration(INT16 nMouleNo, double acceleration);
    double  PASCAL EXPORT CIPget_path_move_acceleration(INT16 nMouleNo);
    // ���� ��⿡ ������ CAMC-IP�� ���� ���� ������ �ð��� ����/Ȯ���Ѵ�.    
    void    PASCAL EXPORT CIPset_path_move_acceltime(INT16 nMouleNo, double acceltime);
    double  PASCAL EXPORT CIPget_path_move_acceltime(INT16 nMouleNo);
    // ���� ��⿡ ������ CAMC-IP�� ���� ���� �� ����� �ӵ� ���������� ����/Ȯ���Ѵ�.            
    void    PASCAL EXPORT CIPset_path_move_profile(INT16 nMouleNo, INT16 mode);
    INT16   PASCAL EXPORT CIPget_path_move_profile(INT16 nMouleNo);

    //-Ĩ ���� ���� ����̹� ����========================================================================================
    //linear_mode(�ڵ� �������� ���� ���� ���Ͽ� ���Ͽ� ��� �����ϴ�. ���� �������� �����ô� ���� ������ �������� �����ؾ� �Ѵ�.)
    // 0 Bit : ���� ������ ��뿩�� (0 : ������� ����, 1: ���)
    // 1 Bit : 2�� �ӵ� ���� ��� ��� ���� (0 : ������� ����, 1 : ���)
    // 2 Bit : �ڵ� ������ ��뿩�� (0 : ������� ����, 1 : ���), ������ ���� ������ ��뿩�δ� ���õȴ�.
    // 3 Bit : Queue ��ü ����(0 : ���� ��� ����(���� Queue�� �Է� ���� �ִٸ� �������� �ش� ���� �����, 1 : Queue�� ��ü)
    //circular_mode(���� �� ���� unit/pulse(pulse/unit)�� ���� �Ͽ��� �Ѵ�.)
    // 0Bit  : ������ ��뿩�� (0 : ������� ����, 1: ���)
    // 1Bit  : 2�� �ӵ� ���� ��� ��� ���� (0 : ������� ����, 1 : ���)
    // 2Bit  : ���� ���� (0 : CW, 1 : CCW)
    // 3 Bit : Queue ��ü ����(0 : ���� ��� ����(���� Queue�� �Է� ���� �ִٸ� �������� �ش� ���� �����, 1 : Queue�� ��ü)
    //start_** : ���� ������ �Լ��� return�Ѵ�. "start_*" �� ������ �̵� �Ϸ��� return�Ѵ�.
    //*r*_*    : �Էµ� ��ġ������ �Ÿ�(�����ǥ) �̴�. "*r_*�� ������ �Էµ� ��ġ������ (������ǥ)�̴�.
    //*_ex     : ������ �����ӵ��� ���� �Ǵ� ���� �ð����� �Է� �޴´�. "*_ex"�� ���ٸ� �����ӷ��� �Է� �޴´�.
    //*_m*     : ��ȣ ������ ����ϴ� ���� �߽��� ��ǥ�� ���� ��ġ�� �������� �ϴ� ��� ��ǥ ������ �Է� �޴´�.
    //_circular: x/y���� ���� ����, x/y�� ���� �߽� ������ �����ϰ� ���� �����Ѵ�.
    //_circular_1: �������� �������� �Է� ���������� ���� �׷��� �� �ο��� ������ �������� �߽������� �����ϰ� ��ȣ ������ �����Ѵ�.
    //             �̶� radius���� (+) �̸� �۰Ե��� ��, (-) �̸� ũ�� ���� ��
    //_circular_2: ���� ����� �� �࿡ �߽����� �Է��ϰ� 0~360 ������ ���� ������ �Է��Ͽ� ���� ��ȣ ���������� �����Ѵ�.
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

    // ���� ����� �� ���� �̿��Ͽ� ���� ���� �Ϸ� ������ ���������� �����ǥ�� �������� �Է��Ͽ�
    // ���� ���������� Queue �� ��ü �Ǵ� ������ �����Ѵ�. ���� �ӵ� �Ķ���ʹ� ���� �����Ǿ� �ִ�
    // ���� �̿��Ѵ�. ���� ������ �ϳ��� ���Ͽ� ���� �Է��� �����Ѵ�. ���� ���� ������ ��conti_�� ����
    // ���� ����̺� �Լ��� ���� ������� �������� �ϰ� �����ϸ� �ȴ�.
    BOOL    PASCAL EXPORT CIPconti_r_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);
    // ���� ����� �� �࿡ ���� ���� �Ϸ� ������ ���������� �����ǥ�� �߽����� �Է��ϰ� 0~360
    // ������ ���� ������ �Է��Ͽ� ���� ��ȣ ���������� Queue �� ��ü �Ǵ� ������ �����Ѵ�. ����
    // �ӵ� �Ķ���ʹ� ���� �����Ǿ� �ִ� ���� �̿��Ѵ�. ���� ������ �ϳ��� ���Ͽ� ���� �Է���
    // �����Ѵ�. ���� ���� ������ ��conti_�� ���� ���� ����̺� �Լ��� ���� ������� �������� �ϰ�
    // �����ϸ� �ȴ�.    
    BOOL    PASCAL EXPORT CIPconti_r_path_move_circular_2(INT16 nModuleNo, double x_cen, double y_cen, double angle, UINT8 circular_mode);
    // ���� ����� �� �࿡ ���� ���� �Ϸ� ������ ���������� ��� ��ǥ�� �������� �Է��ϰ�, ��ȣ��
    // �ִ� ������ ���� �Է��Ͽ� ���� ��ȣ ���������� Queue�� ��ü �Ǵ� ������ �����Ѵ�. ���� �ӵ�
    // �Ķ���ʹ� ���� �����Ǿ� �ִ� ���� �̿��Ѵ�. ���� ������ �ϳ��� ���Ͽ� ���� �Է��� �����Ѵ�.
    // ���� ���� ������ ��conti_�� ���� ���� ����̺� �Լ��� ���� ������� �������� �ϰ� �����ϸ�
    // �ȴ�.
    BOOL    PASCAL EXPORT CIPconti_r_path_move_circular_1(INT16 nModuleNo, double x_end, double y_end, double radius, UINT8 circular_mode);
    // ���� ����� �� �࿡ ���� ���� �Ϸ� ������ ���������� ��� ��ǥ�� ������ �� ������ �Է��Ͽ�
    // ��ȣ ���������� Queue �� ��ü �Ǵ� ������ �����Ѵ�. ���� �ӵ� �Ķ���ʹ� ���� �����Ǿ� �ִ�
    // ���� �̿��Ѵ�. ���� ������ �ϳ��� ���Ͽ� ���� �Է��� �����Ѵ�. ���� ���� ������ ��conti_�� ����
    // ���� ����̺� �Լ��� ���� ������� �������� �ϰ� �����ϸ� �ȴ�.
    BOOL    PASCAL EXPORT CIPconti_r_path_move_circular(INT16 nModuleNo, double x_end, double y_end, double x_cen, double y_cen, UINT8 circular_mode);
    // ���� ����� �� ���� �̿��Ͽ� ���� ���� �Ϸ� ������ ���������� �����ǥ�� �������� �Է��Ͽ�
    // ���� ���������� Queue �� ��ü �Ǵ� ������ �����Ѵ�. ���� �ӵ� �Ķ���ʹ� ���� �����Ǿ� �ִ�
    // ���� �̿��Ѵ�. ���� ������ �ϳ��� ���Ͽ� ���� �Է��� �����Ѵ�. ���� ���� ������ ��conti_�� ����
    // ���� ����̺� �Լ��� ���� ������� �������� �ϰ� �����ϸ� �ȴ�. ���� ������ ���� ���Ͽ�
    // ���Ͽ� ����ϸ� ���ӽ� ���� �޽� ������ ������ ���� ������ �ڵ����� ����Ǿ� ����ȴ�.
    BOOL    PASCAL EXPORT CIPcontiend_r_path_move_linear(INT16 nModuleNo, double x, double y, UINT8 linear_mode);

    //-==================================================================================================================
    // Script/Caption ������ ���� �Լ�
    BOOL   PASCAL EXPORT CIPSetScriptCaption(INT16 axis, INT16 sc, IPEVENT event1, IPEVENT event2, UINT8 event_logic, IPCOMMAND cmd, UINT32 data);
    // sc(�Է�) ==========================================================================
    //        SCRIPT_REG1                    1    // ��ũ��Ʈ ��������-1
    //        SCRIPT_REG2                    2    // ��ũ��Ʈ ��������-2
    //        SCRIPT_REG3                    3    // ��ũ��Ʈ ��������-3
    //        SCRIPT_REG_QUEUE            4    // ��ũ��Ʈ ��������-Queue
    //        CAPTION_REG1                11    // ������ ��������-1
    //        CAPTION_REG2                12    // ������ ��������-2
    //        CAPTION_REG3                13    // ������ ��������-3
    //        CAPTION_REG_QUEUE            14    // ������ ��������-Queue
    // event1(�Է�)  ====================================================================
    //        ����͸� �� event ù��°. IPEVENT�� define�� ���� ���.
    // event2(�Է�)  ====================================================================
    //        ����͸� �� event �ι�°. IPEVENT�� define�� ���� ���.
    // event_logic(�Է�) ==================================================================
    //        7 bit : 0(One time execution), 1(Always execution)
    //        6 bit : sc�� ���� ������ ���� �������� ����.
    //                sc = SCRIPT_REG1, SCRIPT_REG2, SCRIPT_REG3 �� ��. Script ���۽� ����� ����Ÿ �Է� ����.
    //                    0(data ���), 1(ALU ��� ����� ���) 
    //                sc = SCRIPT_REG_QUEUE �� ��. Script ���۽� ���ͷ�Ʈ ��� ����. �ش� ���ͷ�Ʈ mask�� enable �Ǿ� �־�� ��.
    //                    0(���ͷ�Ʈ �߻����� ����), 1(�ش� script ����� ���ͷ�Ʈ �߻�) 
    //                sc = CAPTION_REG1, CAPTION_REG2, CAPTION_REG3 �� ��. Don't care.
    //                sc = CAPTION_REG_QUEUE. Caption ���۽� ���ͷ�Ʈ ��� ����. �ش� ���ͷ�Ʈ mask�� enable�Ǿ� �־�� ��.
    //                    0(���ͷ�Ʈ �߻����� ����), 1(�ش� caption ����� ���ͷ�Ʈ �߻�) 
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
    // cmd(�Է�) ========================================================================
    //        �̺�Ʈ �߻��� ���� ��ų ��ɾ�. IPCOMMAND�� define�� ���� ���.
    
    //-==================================================================================================================
    // Inter Axis, Inter Module sync setting(UIO11�� �̿��� ��Ⱓ, �ణ ���� ��� ������ ���� ����)
    // ��⳻ ���� IO�� ����� Sync ����� ����ϱ� ���� ����, SMC-2V03 version 2.0 �̸������� ����.
    BOOL    PASCAL EXPORT CIPset_sync_mode(INT16 axis, UINT8 mode);
    UINT8   PASCAL EXPORT CIPget_sync_mode(INT16 axis);
    BOOL    PASCAL EXPORT CIPset_sync_mode_master(INT16 axis);
    BOOL    PASCAL EXPORT CIPset_sync_mode_slave(INT16 axis);

    // ���� ���̽� ���峻 ��Ⱓ Sync�� ����� ����ϱ����� ����.
    // ��⳻ ���� IO�� ����� Sync ����� ����ϱ� ���� ����, SMC-2V03 version 2.0 �̸������� ����.
    BOOL    PASCAL EXPORT CIPset_sync_all_mode(INT16 nModuleNo, UINT8 mode);
    UINT8   PASCAL EXPORT CIPget_sync_all_mode(INT16 nModuleNo);
    BOOL    PASCAL EXPORT CIPset_sync_all_mode_master(INT16 nModuleNo);
    BOOL    PASCAL EXPORT CIPset_sync_all_mode_slave(INT16 nModuleNo);
    BOOL    PASCAL EXPORT CIPset_sync_all_mode_disable(INT16 nModuleNo);
 
    // �����ڵ� �б� �Լ��� =============================================================================================
	// ������ �����ڵ带 �д´�.
	INT16 PASCAL EXPORT CIPget_error_code();
	// �����ڵ��� ������ ���ڷ� ��ȯ�Ѵ�.
	char* PASCAL EXPORT CIPget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_CAMC_IP_H__
