#ifndef __AXT_COMM_H__
#define __AXT_COMM_H__

#include "AxtLIBDef.h"
#include "COMMDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTCOMM Library - Serial communication module
	적용제품
		COM-234R - RS-232C 4Ch
		COM-484R - RS-422/485 4Ch
 *------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// Initilize/Close 
	// 통신 기능 사용
	BOOL   PASCAL EXPORT InitializeCOMM();
	// 통신 기능이 사용 가능한가 ?
	BOOL   PASCAL EXPORT COMMIsInitialized(void);
	// 통신 기능 사용의 중지
	void   PASCAL EXPORT COMMStopService(void);

	// Window message & procedure
	void   PASCAL EXPORT COMMSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_COMM_INTERRUPT_PROC proc);

	// 베이스보드의 어드레스를 입력하여 베이스보드번호 검색
	INT16  PASCAL EXPORT COMMget_boardno(UINT32 address);
	// 모듈의 어드레스를 입력하여 모듈번호 검색
	INT16  PASCAL EXPORT COMMget_moduleno(INT16 nBoardNo, INT16 nModulePos);
	// 오픈된 베이스보드인가 ?
	BOOL   PASCAL EXPORT COMMis_open_board(INT16 nBoardNo);
	// 오픈된 모듈인가 ?
	BOOL   PASCAL EXPORT COMMis_open_module(INT16 nModuleNo);
	// 유효한 모듈번호인가 ?
	BOOL   PASCAL EXPORT COMMis_valid_moduleno(INT16 nModuleNo);
	// 채널번호를 입력하여 모듈번호 검색
	INT16  PASCAL EXPORT COMMchannelno_2_moduleno(INT16 ChannelNo);
	// 모듈번호를 입력하여 채널번호 검색
	INT16  PASCAL EXPORT COMMmoduleno_2_channelno(INT16 nModuleNo);
	// 모듈 ID
	UINT16 PASCAL EXPORT COMMget_moduleid(INT16 nModuleNo);
	// 검색된 베이스보드의 갯수를 반환
	UINT16 PASCAL EXPORT COMMget_board_count();
	// 검색된 아날로그 모듈보드의 갯수를 반환
	UINT16 PASCAL EXPORT COMMget_module_count();
	// 모듈넘버로 모듈정보를 얻어온다
	BOOL   PASCAL EXPORT COMMget_module_info(INT16 nModuleNo, INT16 *pBoardNo, INT16 *pModulePos);

	// Port Open/Close
	// 통신 포트 열기
	BOOL   PASCAL EXPORT COMMopen_port(INT16 ChannelNo, INT32 nBaudrate);
	// 통신 포트가 열려 있는가 ?
	BOOL   PASCAL EXPORT COMMis_open_port(INT16 ChannelNo);
	// 열려 있는 통신 포트 닫기
	void   PASCAL EXPORT COMMclose_port(INT16 ChannelNo);

	// Baudrate
	// 통신속도 설정
	void   PASCAL EXPORT COMMset_baudrate(INT16 ChannelNo, INT32 nValue);
	// 설정된 통신속도 검색
	INT32  PASCAL EXPORT COMMget_baudrate(INT16 ChannelNo);

	// Data length
	// 데이터 길이 설정
	void   PASCAL EXPORT COMMset_data_length(INT16 ChannelNo, INT16 nValue);
	// 설정된 데이터 길이 검색
	INT16  PASCAL EXPORT COMMget_data_length(INT16 ChannelNo);

	// Stop bit
	// 정지비트 설정
	void   PASCAL EXPORT COMMset_stop_bit(INT16 ChannelNo, INT16 nValue);
	// 설정된 정지비트 검색
	INT16  PASCAL EXPORT COMMget_stop_bit(INT16 ChannelNo);

	// Parity bit
	// 패리티비트 설정
	void   PASCAL EXPORT COMMset_parity_bit(INT16 ChannelNo, INT16 nValue);
	// 설정된 패리티비트 검색
	INT16  PASCAL EXPORT COMMget_parity_bit(INT16 ChannelNo);

	// 수신 Queue에서 문자 및 문자열 검색
	// 1문자 검색
	INT16  PASCAL EXPORT COMMfind_recv_char(INT16 ChannelNo, INT16 nCh);
	// 문자열 검색
	INT16  PASCAL EXPORT COMMfind_recv_string(INT16 ChannelNo, char *pString);				// <*> 2002.03.07  LPCTSTR => char *
	// 지정한 갯수의 문자열 검색
	INT16  PASCAL EXPORT COMMfind_recv_n_string(INT16 ChannelNo, char *pString, INT16 nLen);	// <*> 2002.03.07  LPCTSTR => char *

	// Put 문자/문자열
	// 1문자 송신
	void   PASCAL EXPORT COMMput_char(INT16 ChannelNo, INT16 nCh);
	// 문자열 송신
	INT16  PASCAL EXPORT COMMput_string(INT16 ChannelNo, char *pString);						// <*> 2002.03.07  LPCTSTR => char *
	// 지정한 갯수의 문자열 송신
	INT16  PASCAL EXPORT COMMput_n_string(INT16 ChannelNo, char *pString, INT16 nLen);		// <*> 2002.03.07  LPCTSTR => char *

	// Get 문자/문자열
	// 1문자 수신
	INT16  PASCAL EXPORT COMMget_char(INT16 ChannelNo);
	// 문자열 수신
	INT16  PASCAL EXPORT COMMget_string(INT16 ChannelNo, char *lpString);					// <*> 2002.03.07  LPSTR => char *
	// 지정한 갯수의 문자열 수신
	INT16  PASCAL EXPORT COMMget_n_string(INT16 ChannelNo, char *lpString, INT16 nLen);		// <*> 2002.03.07  LPSTR => char *

	// 수신 Queue
	// 수신 Queue를 사용할 것인지를 설정
// <+> 2002/03/20
	void   PASCAL EXPORT COMMset_recv_queue_mode(INT16 nChannelNo, BOOL fEnable);
	// 설정된 수신 Queue를 모드 검색
// <+> 2002/03/20
	BOOL   PASCAL EXPORT COMMget_recv_queue_mode(INT16 nChannelNo);
	// 수신 큐의 크기 설정
	void   PASCAL EXPORT COMMset_recv_queue_size(INT16 ChannelNo, INT16 nSize);
	// 설정된 큐의 크기 검색
	INT16  PASCAL EXPORT COMMget_recv_queue_size(INT16 ChannelNo);
	// 수신 큐에서 수신된 데이터의 갯수 검색
	INT16  PASCAL EXPORT COMMget_recv_length(INT16 ChannelNo);
	// 수신 큐가 비워져 있는가 ?
	BOOL   PASCAL EXPORT COMMis_empty_recv(INT16 ChannelNo);
	// 수신 큐를 비운다
	void   PASCAL EXPORT COMMflush(INT16 ChannelNo);

	// 송신
	BOOL   PASCAL EXPORT COMMis_empty_send(INT16 nChannelNo);

	// 수신 Threshold
	// 인터럽트 사용시, 몇개의 데이터가 수신되었을 때 인터럽트를 발생시킬 것인지 설정
	void   PASCAL EXPORT COMMset_recv_Threshold(INT16 ChannelNo, INT16 nValue);
	INT16  PASCAL EXPORT COMMget_recv_Threshold(INT16 ChannelNo);

	// Break
	void   PASCAL EXPORT COMMset_break(INT16 ChannelNo, BOOL fValue);
	BOOL   PASCAL EXPORT COMMget_break(INT16 ChannelNo);

	// Loop-back test
	// 루프백 설정/해제
	void   PASCAL EXPORT COMMset_loop_back(INT16 ChannelNo, BOOL fValue);
	// 설정/해제된 루프백 검색
	BOOL   PASCAL EXPORT COMMget_loop_back(INT16 ChannelNo);

	// 포트(채널)의 수
	INT16  PASCAL EXPORT COMMget_number_of_channel();

	// <<only RS-422/485>>
	// Receive/Sendmitter enable/disable
	void   PASCAL EXPORT COMMset_send_enable(INT16 ChannelNo, BOOL fValue);
	BOOL   PASCAL EXPORT COMMget_send_enable(INT16 ChannelNo);
	void   PASCAL EXPORT COMMset_recv_enable(INT16 ChannelNo, BOOL fValue);
	BOOL   PASCAL EXPORT COMMget_recv_enable(INT16 ChannelNo);

	// <<only RS-232C>>
	// Control
	// DTR Set/Reset
	void   PASCAL EXPORT COMMset_dtr_enable(INT16 ChannelNo, BOOL fValue);
	// get DTR
	BOOL   PASCAL EXPORT COMMget_dtr_enable(INT16 ChannelNo);
	// RTS Set/Reset
	void   PASCAL EXPORT COMMset_rts_enable(INT16 ChannelNo, BOOL fValue);
	// get RTS
	BOOL   PASCAL EXPORT COMMget_rts_enable(INT16 ChannelNo);
	// get CTS
	BOOL   PASCAL EXPORT COMMget_cts(INT16 ChannelNo);
	// get DSR
	BOOL   PASCAL EXPORT COMMget_dsr(INT16 ChannelNo);
	// get RI
	BOOL   PASCAL EXPORT COMMget_ri(INT16 ChannelNo);
	// get CD
	BOOL   PASCAL EXPORT COMMget_cd(INT16 ChannelNo);

	// Software reset
	void   PASCAL EXPORT COMMrun_software_reset(INT16 ModuleNo);

	// Interrupt
	// 인터럽트 허용
	BOOL   PASCAL EXPORT COMMenable_interrupt(INT16 ChannelNo);
	// 인터럽트가 허용 상태인가 ?
	BOOL   PASCAL EXPORT COMMis_enable_interrupt(INT16 ChannelNo);
	// 인터럽트 금지
	void   PASCAL EXPORT COMMdisable_interrupt(INT16 ChannelNo);
	
// gun5 2002.12.26 add(version이 1 인 경우 사용)
	void   PASCAL EXPORT COMMset_rs422_enable(INT16 nChannelNo, BOOL fValue);
	BOOL   PASCAL EXPORT COMMget_rs422_enable(INT16 nChannelNo);
	UINT16 PASCAL EXPORT COMMget_moduleversion(INT16 nModuleNo);

	INT16  PASCAL EXPORT COMMget_error_code();
	char*  PASCAL EXPORT COMMget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_COMM_H__