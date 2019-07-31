#ifndef __AXT_AIO_H__
#define __AXT_AIO_H__

#include "AxtLIBDef.h"
#include "AIODef.h"

/*------------------------------------------------------------------------------------------------*
	AXTAIO Library - Analog Input/Ouput module
	적용제품
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
	BOOL	PASCAL EXPORT AIOrun_software_reset(INT16 nModuleNo);									// 소프트웨어 리셋

	// EEPROM 읽고쓰기
	void	PASCAL EXPORT AIOeeprom_write_word(INT16 nModuleNo, INT16 eep_addr, UINT16 wValue);
	UINT16	PASCAL EXPORT AIOeeprom_read_word(INT16 nModuleNo, INT16 eep_addr);

	// 보드 및 모듈 정보 검색
	INT16	PASCAL EXPORT AIOget_boardno(UINT32 address);											// 베이스 어드레스로 베이스 보드 번호 구하기
	INT16	PASCAL EXPORT AIOget_moduleno(INT16 nBoardNo, INT16 nModulePos);						// 베이스보드 번호와 모듈위치로 모듈번호 구하기
	BOOL	PASCAL EXPORT AIOis_open_board(INT16 nBoardNo);											// 오픈된 베이스보드인가 ?
	BOOL	PASCAL EXPORT AIOis_open_module(INT16 nModuleNo);										// 오픈된 모듈인가 ?
	BOOL	PASCAL EXPORT AIOis_valid_moduleno(INT16 nModuleNo);									// 유효한 모듈번호인가 ?	
	UINT16	PASCAL EXPORT AIOget_output_number(INT16 nModuleNo);									// 모듈의 출력채널수를 반환
	UINT16	PASCAL EXPORT AIOget_input_number(INT16 nModuleNo);										// 모듈의 입력채널수를 반환
	UINT16	PASCAL EXPORT AIOget_moduleid(INT16 nModuleNo);											// 모듈 ID
	UINT16	PASCAL EXPORT AIOget_board_count();														// 검색된 베이스보드의 갯수를 반환
	UINT16	PASCAL EXPORT AIOget_module_count();													// 검색된 아날로그 모듈보드의 갯수를 반환
	BOOL	PASCAL EXPORT AIOget_module_info(INT16 nModuleNo, INT16 *pBoardNo, INT16 *pModulePos);	// 모듈넘버로 모듈정보를 얻어온다

	// ## 아날로그 출력관련 ##
	// 출력모듈 설정함수
	double	PASCAL EXPORT AIOread_dac(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOwrite_dac(INT16 nChannelNo, double dVolt);								// D/A 출력
	BOOL	PASCAL EXPORT AIOset_range_dac(INT16 nChannelNo, double dVmin, double dVmax);			// D/A 출력 전압범위 설정
	BOOL	PASCAL EXPORT AIOget_range_dac(INT16 nChannelNo, double* dVmin, double* dVmax);			// D/A 출력 전압범위 설정값 읽기

	// 출력모듈 Calibration함수
	double	PASCAL EXPORT AIOmanual_calibration_dac(INT16 nDAChannelNo, INT16 nADChannelNo, double dVolt, INT16 nOffset);
	BOOL	PASCAL EXPORT AIOsave_calibration_data_dac(INT16 nDAChannelNo, double dVolt, INT16 nOffset);
	BOOL	PASCAL EXPORT AIOone_channel_calibration_dac(INT16 nDAChannelNo, INT16 nADChannelNo);
	INT16	PASCAL EXPORT AIOget_calibration_data_dac(INT16 nChannelNo, double dVolt);
	UINT16	PASCAL EXPORT AIOauto_calibration_dac(INT16 nDAModuleNo, INT16 nADModuleNo);

	// 출력모듈 정보검색함수
	INT16	PASCAL EXPORT AIOmoduleno_2_channelno_dac(INT16 nModuleNo);
	INT16	PASCAL EXPORT AIOchannelno_2_moduleno_dac(INT16 nChannelNo);	// AO
	INT16	PASCAL EXPORT AIOchannelno_2_boardno_dac(INT16 nChannelNo);
	INT16	PASCAL EXPORT AIOget_module_number_dac();
	INT16	PASCAL EXPORT AIOget_channel_number_dac();

	// ## 아날로그 입력 관련 ##	
	// 아날로그 입력 설정함수
	BOOL	PASCAL EXPORT AIOset_range_adc(INT16 nChannelNo, double dVmin, double dVmax);			// 출력전압 범위를 설정
	BOOL	PASCAL EXPORT AIOset_overflower_mode_adc(INT16 nChannelNo, BOOL bMode);
	BOOL	PASCAL EXPORT AIOset_limit_adc(INT16 nChannelNo, UINT16 wLower, UINT16 wUpper);
	BOOL	PASCAL EXPORT AIOset_trigger_mode_adc(INT16 nModuleNo, INT16 nTriggerMode);				// 트리거 모드 설정
	BOOL	PASCAL EXPORT AIOset_sample_freq_adc(INT16 nModuleNo, double dValue);					// 타이머 주파수 단위로 설정[10 - 50000]
	BOOL	PASCAL EXPORT AIOset_timer_const_adc(INT16 nModuleNo, double dValue);					// 타이머 시간 단위로 설정[20 - 100000]

	// 아날로그 입력 채널 다중 설정함수
	BOOL	PASCAL EXPORT AIOmulti_set_range_adc(INT16 nCHNum, INT16 *pnCHList, double dVmin, double dVmax);
	BOOL	PASCAL EXPORT AIOmulti_set_overflower_mode_adc(INT16 nCHNum, INT16 *pnCHList, BOOL bMode);
	BOOL	PASCAL EXPORT AIOmulti_set_limit_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 wLower, UINT16 wUpper);
	BOOL	PASCAL EXPORT AIOmulti_set_interrupt_mask_adc(INT16 nCHNum, INT16 *pnCHList, UINT8 byIntMask);
	BOOL	PASCAL EXPORT AIOmulti_enable_interrupt_adc(INT16 nCHNum, INT16 *pnCHList);
	BOOL	PASCAL EXPORT AIOmulti_disable_interrupt_adc(INT16 nCHNum, INT16 *pnCHList);

	// 입력모듈 설정값 읽기함수
	BOOL	PASCAL EXPORT AIOget_range_adc(INT16 nChannelNo, double* dVmin, double* dVmax);			// 출력전압 범위 설정값 읽기
	BOOL	PASCAL EXPORT AIOget_overflower_mode_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOget_limit_adc(INT16 nChannelNo, UINT16 *wLower, UINT16 *wUpper);
	INT16	PASCAL EXPORT AIOget_trigger_mode_adc(INT16 nModuleNo);									// 트리거 모드 검색
	UINT32	PASCAL EXPORT AIOget_sample_freq_adc(INT16 nModuleNo);									// 타이머 설정값 주파수 단위로 검색
	double	PASCAL EXPORT AIOget_timer_const_adc(INT16 nModuleNo);									// 타이머 설정값 시간 단위로 검색	

	// 입력모듈 인터럽트 설정 함수
	BOOL	PASCAL EXPORT AIOenable_interrupt_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOis_enable_interrupt_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOdisable_interrupt_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOset_interrupt_mask_adc(INT16 nChannelNo, UINT8 byIntMask);
	UINT8	PASCAL EXPORT AIOget_interrupt_mask_adc(INT16 nChannelNo);		// 안병건 수정 2003.06.13.
	
	// A/D 변환 함수
	// A/D Conversion 후 변환값을 Digit로 반환
	double	PASCAL EXPORT AIOread_one_volt_adc(INT16 nChannelNo);									// A/D Conversion 후 변환값을 전압형태로 반환
	BOOL	PASCAL EXPORT AIOread_one_digit_adc(INT16 nChannelNo, UINT16 *wDigit);		
	BOOL	PASCAL EXPORT AIOmulti_read_one_volt_adc(INT16 nCHNum, INT16 *pnCHList, INT16 nDataNum, double *pdVolt[]);
	BOOL	PASCAL EXPORT AIOmulti_read_one_digit_adc(INT16 nCHNum, INT16 *pnCHList, INT16 nDataNum, UINT16 *pwDigit[]);	
	BOOL	PASCAL EXPORT AIOmulti_start_channel_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 uBufSize);	// H/W 타이머를 이용한 A/D Conversion 시작
	BOOL	PASCAL EXPORT AIOmulti_start_filter_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 uFilterNum, UINT16 uBufSize);
	BOOL	PASCAL EXPORT AIOmulti_stop_channel_adc();												// H/W 타이머를 이용한 A/D Conversion 정지
	BOOL	PASCAL EXPORT AIOset_immediate_access_adc(INT16 nCHNum, INT16 *pnCHList, UINT16 *pwSize, UINT16 *pwRetLength);
	BOOL	PASCAL EXPORT AIOstart_immediate_access_adc(double *pdBuffer[]);
	double	PASCAL EXPORT AIOread_channel_volt_adc(INT16 nChannelNo);								// H/W 타이머를 이용한 A/D Conversion한 데이타 전압값으로 읽기
	BOOL	PASCAL EXPORT AIOread_channel_digit_adc(INT16 nChannelNo, UINT16 *wDigit);				// H/W 타이머를 이용한 A/D Conversion한 데이타 Digit로 읽기	
	
	// 버퍼 상태 Check 함수
	BOOL	PASCAL EXPORT AIOis_buffer_empty_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOis_buffer_upper_adc(INT16 nChannelNo);
	BOOL	PASCAL EXPORT AIOis_buffer_lower_adc(INT16 nChannelNo);	
	UINT16	PASCAL EXPORT AIOget_data_length_adc(INT16 nChannelNo);									// 변환된데이터의 갯수

	// 입력모듈 정보 검색함수
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
