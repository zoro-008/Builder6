#ifndef __AXT_AIO_H__
#define __AXT_AIO_H__

#include "AxtLIBDef.h"
#include "AIODef.h"

/*------------------------------------------------------------------------------------------------*
	AXTAIO Library - Analog Input/Ouput module
	������ǰ
		SIO-AI4R  - AD 4Ch, 12bit, 1U size
		SIO-AO4R  - DA 4Ch, 12bit, 1U size
		SIO-AI16H - AD 16Ch, 16bit, 2U size
		SIO-AO8H  - DA 8Ch, 16bit, 2U size
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	// Initilize/Close 
	BOOL	PASCAL EXPORT InitializeAIO();																	
	BOOL	PASCAL EXPORT AIOIsInitialized();
	//void	PASCAL EXPORT AIOStopService(void);

	// Window message & procedure
	void	PASCAL EXPORT AIOSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_AIO_INTERRUPT_PROC proc);	                               
	BOOL	PASCAL EXPORT AIOrun_software_reset(INT16 nModuleNo);									// ����Ʈ���� ����

	// EEPROM �а���
	void	PASCAL EXPORT AIOeeprom_write_word(INT16 nModuleNo, INT16 eep_addr, UINT16 wValue);
	UINT16	PASCAL EXPORT AIOeeprom_read_word(INT16 nModuleNo, INT16 eep_addr);

	// ���� �� ��� ���� �˻�
	INT16	PASCAL EXPORT AIOget_boardno(UINT32 address);											// ���̽� ��巹���� ���̽� ���� ��ȣ ���ϱ�
	INT16	PASCAL EXPORT AIOget_moduleno(INT16 nBoardNo, INT16 nModulePos);						// ���̽����� ��ȣ�� �����ġ�� ����ȣ ���ϱ�
	BOOL	PASCAL EXPORT AIOis_open_board(INT16 nBoardNo);											// ���µ� ���̽������ΰ� ?
	BOOL	PASCAL EXPORT AIOis_open_module(INT16 nModuleNo);										// ���µ� ����ΰ� ?
	BOOL	PASCAL EXPORT AIOis_valid_moduleno(INT16 nModuleNo);									// ��ȿ�� ����ȣ�ΰ� ?	
	UINT16	PASCAL EXPORT AIOget_output_number(INT16 nModuleNo);									// ����� ���ä�μ��� ��ȯ
	UINT16	PASCAL EXPORT AIOget_input_number(INT16 nModuleNo);										// ����� �Է�ä�μ��� ��ȯ
	UINT16	PASCAL EXPORT AIOget_moduleid(INT16 nModuleNo);											// ��� ID
	UINT16	PASCAL EXPORT AIOget_board_count();														// �˻��� ���̽������� ������ ��ȯ
	UINT16	PASCAL EXPORT AIOget_module_count();													// �˻��� �Ƴ��α� ��⺸���� ������ ��ȯ
	BOOL	PASCAL EXPORT AIOget_module_info(INT16 nModuleNo, INT16 *pBoardNo, INT16 *pModulePos);	// ���ѹ��� ��������� ���´�

	// ## �Ƴ��α� ��°��� ##
	// ��¸�� �����Լ�
	double	PASCAL EXPORT AIOread_dac(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOwrite_dac(INT16 nChannelNo, double dVolt);								// D/A ���
	BOOL	PASCAL EXPORT AIOset_range_dac(INT16 nChannelNo, double dVmin, double dVmax);			// D/A ��� ���й��� ����
	BOOL	PASCAL EXPORT AIOget_range_dac(INT16 nChannelNo, double* dVmin, double* dVmax);			// D/A ��� ���й��� ������ �б�

	// ��¸�� Calibration�Լ�
	double	PASCAL EXPORT AIOmanual_calibration_dac(INT16 nDAChannelNo, INT16 nADChannelNo, double dVolt, INT16 nOffset);
	BOOL	PASCAL EXPORT AIOsave_calibration_data_dac(INT16 nDAChannelNo, double dVolt, INT16 nOffset);
	BOOL	PASCAL EXPORT AIOone_channel_calibration_dac(INT16 nDAChannelNo, INT16 nADChannelNo);
	INT16	PASCAL EXPORT AIOget_calibration_data_dac(INT16 nChannelNo, double dVolt);
	UINT16	PASCAL EXPORT AIOauto_calibration_dac(INT16 nDAModuleNo, INT16 nADModuleNo);

	// ��¸�� �����˻��Լ�
	INT16	PASCAL EXPORT AIOmoduleno_2_channelno_dac(INT16 nModuleNo);
	INT16	PASCAL EXPORT AIOchannelno_2_moduleno_dac(INT16 nChannelNo);	// AO
	INT16	PASCAL EXPORT AIOchannelno_2_boardno_dac(INT16 nChannelNo);
	INT16	PASCAL EXPORT AIOget_module_number_dac();
	INT16	PASCAL EXPORT AIOget_channel_number_dac();

	// ## �Ƴ��α� �Է� ���� ##	
	// �Ƴ��α� �Է� �����Լ�
	BOOL	PASCAL EXPORT AIOset_range_adc(INT16 nChannelNo, double dVmin, double dVmax);			// ������� ������ ����
	BOOL	PASCAL EXPORT AIOset_overflower_mode_adc(INT16 nChannelNo, BOOL bMode);
	BOOL	PASCAL EXPORT AIOset_limit_adc(INT16 nChannelNo, UINT16 wLower, UINT16 wUpper);
	BOOL	PASCAL EXPORT AIOset_trigger_mode_adc(INT16 nModuleNo, INT16 nTriggerMode);				// Ʈ���� ��� ����
	BOOL	PASCAL EXPORT AIOset_sample_freq_adc(INT16 nModuleNo, double dValue);					// Ÿ�̸� ���ļ� ������ ����[10 - 50000]
	BOOL	PASCAL EXPORT AIOset_timer_const_adc(INT16 nModuleNo, double dValue);					// Ÿ�̸� �ð� ������ ����[20 - 100000]

	// �Ƴ��α� �Է� ä�� ���� �����Լ�
	BOOL	PASCAL EXPORT AIOmulti_set_range_adc(INT16 nCHNum, INT16 *pnCHList, double dVmin, double dVmax);
	BOOL	PASCAL EXPORT AIOmulti_set_overflower_mode_adc(INT16 nCHNum, INT16 *pnCHList, BOOL bMode);
	BOOL	PASCAL EXPORT AIOmulti_set_limit_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 wLower, UINT16 wUpper);
	BOOL	PASCAL EXPORT AIOmulti_set_interrupt_mask_adc(INT16 nCHNum, INT16 *pnCHList, UINT8 byIntMask);
	BOOL	PASCAL EXPORT AIOmulti_enable_interrupt_adc(INT16 nCHNum, INT16 *pnCHList);
	BOOL	PASCAL EXPORT AIOmulti_disable_interrupt_adc(INT16 nCHNum, INT16 *pnCHList);

	// �Է¸�� ������ �б��Լ�
	BOOL	PASCAL EXPORT AIOget_range_adc(INT16 nChannelNo, double* dVmin, double* dVmax);			// ������� ���� ������ �б�
	BOOL	PASCAL EXPORT AIOget_overflower_mode_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOget_limit_adc(INT16 nChannelNo, UINT16 *wLower, UINT16 *wUpper);
	INT16	PASCAL EXPORT AIOget_trigger_mode_adc(INT16 nModuleNo);									// Ʈ���� ��� �˻�
	UINT32	PASCAL EXPORT AIOget_sample_freq_adc(INT16 nModuleNo);									// Ÿ�̸� ������ ���ļ� ������ �˻�
	double	PASCAL EXPORT AIOget_timer_const_adc(INT16 nModuleNo);									// Ÿ�̸� ������ �ð� ������ �˻�	

	// �Է¸�� ���ͷ�Ʈ ���� �Լ�
	BOOL	PASCAL EXPORT AIOenable_interrupt_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOis_enable_interrupt_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOdisable_interrupt_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOset_interrupt_mask_adc(INT16 nChannelNo, UINT8 byIntMask);
	UINT8	PASCAL EXPORT AIOget_interrupt_mask_adc(INT16 nChannelNo);		// �Ⱥ��� ���� 2003.06.13.
	
	// A/D ��ȯ �Լ�
	// A/D Conversion �� ��ȯ���� Digit�� ��ȯ
	double	PASCAL EXPORT AIOread_one_volt_adc(INT16 nChannelNo);									// A/D Conversion �� ��ȯ���� �������·� ��ȯ
	BOOL	PASCAL EXPORT AIOread_one_digit_adc(INT16 nChannelNo, UINT16 *wDigit);		
	BOOL	PASCAL EXPORT AIOmulti_read_one_volt_adc(INT16 nCHNum, INT16 *pnCHList, INT16 nDataNum, double *pdVolt[]);
	BOOL	PASCAL EXPORT AIOmulti_read_one_digit_adc(INT16 nCHNum, INT16 *pnCHList, INT16 nDataNum, UINT16 *pwDigit[]);	
	BOOL	PASCAL EXPORT AIOmulti_start_channel_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 uBufSize);	// H/W Ÿ�̸Ӹ� �̿��� A/D Conversion ����
	BOOL	PASCAL EXPORT AIOmulti_start_filter_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 uFilterNum, UINT16 uBufSize);
	BOOL	PASCAL EXPORT AIOmulti_stop_channel_adc();												// H/W Ÿ�̸Ӹ� �̿��� A/D Conversion ����
	BOOL	PASCAL EXPORT AIOset_immediate_access_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 *pwSize, UINT16 *pwRetLength);
	BOOL	PASCAL EXPORT AIOstart_immediate_access_adc(double *pdBuffer[]);
	double	PASCAL EXPORT AIOread_channel_volt_adc(INT16 nChannelNo);								// H/W Ÿ�̸Ӹ� �̿��� A/D Conversion�� ����Ÿ ���а����� �б�
	BOOL	PASCAL EXPORT AIOread_channel_digit_adc(INT16 nChannelNo, UINT16 *wDigit);				// H/W Ÿ�̸Ӹ� �̿��� A/D Conversion�� ����Ÿ Digit�� �б�	
	
	// ���� ���� Check �Լ�
	BOOL	PASCAL EXPORT AIOis_buffer_empty_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOis_buffer_upper_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOis_buffer_lower_adc(INT16 nChannelNo);	
	UINT16	PASCAL EXPORT AIOget_data_length_adc(INT16 nChannelNo);									// ��ȯ�ȵ������� ����

	// �Է¸�� ���� �˻��Լ�
	INT16	PASCAL EXPORT AIOchannelno_2_boardno_adc(INT16 nChannelNo);
	INT16	PASCAL EXPORT AIOchannelno_2_moduleno_adc(INT16 nChannelNo);	// AI
	INT16	PASCAL EXPORT AIOmoduleno_2_channelno_adc(INT16 nModuleNo);
	INT16	PASCAL EXPORT AIOget_module_number_adc();
	INT16	PASCAL EXPORT AIOget_channel_number_adc();

	INT16  PASCAL EXPORT AIOget_error_code();
	char * PASCAL EXPORT AIOget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_AIO_H__
