#ifndef __AXT_COMM_H__
#define __AXT_COMM_H__

#include "AxtLIBDef.h"
#include "COMMDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTCOMM Library - Serial communication module
	������ǰ
		COM-234R - RS-232C 4Ch
		COM-484R - RS-422/485 4Ch
 *------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// Initilize/Close 
	// ��� ��� ���
	BOOL   PASCAL EXPORT InitializeCOMM();
	// ��� ����� ��� �����Ѱ� ?
	BOOL   PASCAL EXPORT COMMIsInitialized(void);
	// ��� ��� ����� ����
	void   PASCAL EXPORT COMMStopService(void);

	// Window message & procedure
	void   PASCAL EXPORT COMMSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_COMM_INTERRUPT_PROC proc);

	// ���̽������� ��巹���� �Է��Ͽ� ���̽������ȣ �˻�
	INT16  PASCAL EXPORT COMMget_boardno(UINT32 address);
	// ����� ��巹���� �Է��Ͽ� ����ȣ �˻�
	INT16  PASCAL EXPORT COMMget_moduleno(INT16 nBoardNo, INT16 nModulePos);
	// ���µ� ���̽������ΰ� ?
	BOOL   PASCAL EXPORT COMMis_open_board(INT16 nBoardNo);
	// ���µ� ����ΰ� ?
	BOOL   PASCAL EXPORT COMMis_open_module(INT16 nModuleNo);
	// ��ȿ�� ����ȣ�ΰ� ?
	BOOL   PASCAL EXPORT COMMis_valid_moduleno(INT16 nModuleNo);
	// ä�ι�ȣ�� �Է��Ͽ� ����ȣ �˻�
	INT16  PASCAL EXPORT COMMchannelno_2_moduleno(INT16 ChannelNo);
	// ����ȣ�� �Է��Ͽ� ä�ι�ȣ �˻�
	INT16  PASCAL EXPORT COMMmoduleno_2_channelno(INT16 nModuleNo);
	// ��� ID
	UINT16 PASCAL EXPORT COMMget_moduleid(INT16 nModuleNo);
	// �˻��� ���̽������� ������ ��ȯ
	UINT16 PASCAL EXPORT COMMget_board_count();
	// �˻��� �Ƴ��α� ��⺸���� ������ ��ȯ
	UINT16 PASCAL EXPORT COMMget_module_count();
	// ���ѹ��� ��������� ���´�
	BOOL   PASCAL EXPORT COMMget_module_info(INT16 nModuleNo, INT16 *pBoardNo, INT16 *pModulePos);

	// Port Open/Close
	// ��� ��Ʈ ����
	BOOL   PASCAL EXPORT COMMopen_port(INT16 ChannelNo, INT32 nBaudrate);
	// ��� ��Ʈ�� ���� �ִ°� ?
	BOOL   PASCAL EXPORT COMMis_open_port(INT16 ChannelNo);
	// ���� �ִ� ��� ��Ʈ �ݱ�
	void   PASCAL EXPORT COMMclose_port(INT16 ChannelNo);

	// Baudrate
	// ��żӵ� ����
	void   PASCAL EXPORT COMMset_baudrate(INT16 ChannelNo, INT32 nValue);
	// ������ ��żӵ� �˻�
	INT32  PASCAL EXPORT COMMget_baudrate(INT16 ChannelNo);

	// Data length
	// ������ ���� ����
	void   PASCAL EXPORT COMMset_data_length(INT16 ChannelNo, INT16 nValue);
	// ������ ������ ���� �˻�
	INT16  PASCAL EXPORT COMMget_data_length(INT16 ChannelNo);

	// Stop bit
	// ������Ʈ ����
	void   PASCAL EXPORT COMMset_stop_bit(INT16 ChannelNo, INT16 nValue);
	// ������ ������Ʈ �˻�
	INT16  PASCAL EXPORT COMMget_stop_bit(INT16 ChannelNo);

	// Parity bit
	// �и�Ƽ��Ʈ ����
	void   PASCAL EXPORT COMMset_parity_bit(INT16 ChannelNo, INT16 nValue);
	// ������ �и�Ƽ��Ʈ �˻�
	INT16  PASCAL EXPORT COMMget_parity_bit(INT16 ChannelNo);

	// ���� Queue���� ���� �� ���ڿ� �˻�
	// 1���� �˻�
	INT16  PASCAL EXPORT COMMfind_recv_char(INT16 ChannelNo, INT16 nCh);
	// ���ڿ� �˻�
	INT16  PASCAL EXPORT COMMfind_recv_string(INT16 ChannelNo, char *pString);				// <*> 2002.03.07  LPCTSTR => char *
	// ������ ������ ���ڿ� �˻�
	INT16  PASCAL EXPORT COMMfind_recv_n_string(INT16 ChannelNo, char *pString, INT16 nLen);	// <*> 2002.03.07  LPCTSTR => char *

	// Put ����/���ڿ�
	// 1���� �۽�
	void   PASCAL EXPORT COMMput_char(INT16 ChannelNo, INT16 nCh);
	// ���ڿ� �۽�
	INT16  PASCAL EXPORT COMMput_string(INT16 ChannelNo, char *pString);						// <*> 2002.03.07  LPCTSTR => char *
	// ������ ������ ���ڿ� �۽�
	INT16  PASCAL EXPORT COMMput_n_string(INT16 ChannelNo, char *pString, INT16 nLen);		// <*> 2002.03.07  LPCTSTR => char *

	// Get ����/���ڿ�
	// 1���� ����
	INT16  PASCAL EXPORT COMMget_char(INT16 ChannelNo);
	// ���ڿ� ����
	INT16  PASCAL EXPORT COMMget_string(INT16 ChannelNo, char *lpString);					// <*> 2002.03.07  LPSTR => char *
	// ������ ������ ���ڿ� ����
	INT16  PASCAL EXPORT COMMget_n_string(INT16 ChannelNo, char *lpString, INT16 nLen);		// <*> 2002.03.07  LPSTR => char *

	// ���� Queue
	// ���� Queue�� ����� �������� ����
// <+> 2002/03/20
	void   PASCAL EXPORT COMMset_recv_queue_mode(INT16 nChannelNo, BOOL fEnable);
	// ������ ���� Queue�� ��� �˻�
// <+> 2002/03/20
	BOOL   PASCAL EXPORT COMMget_recv_queue_mode(INT16 nChannelNo);
	// ���� ť�� ũ�� ����
	void   PASCAL EXPORT COMMset_recv_queue_size(INT16 ChannelNo, INT16 nSize);
	// ������ ť�� ũ�� �˻�
	INT16  PASCAL EXPORT COMMget_recv_queue_size(INT16 ChannelNo);
	// ���� ť���� ���ŵ� �������� ���� �˻�
	INT16  PASCAL EXPORT COMMget_recv_length(INT16 ChannelNo);
	// ���� ť�� ����� �ִ°� ?
	BOOL   PASCAL EXPORT COMMis_empty_recv(INT16 ChannelNo);
	// ���� ť�� ����
	void   PASCAL EXPORT COMMflush(INT16 ChannelNo);

	// �۽�
	BOOL   PASCAL EXPORT COMMis_empty_send(INT16 nChannelNo);

	// ���� Threshold
	// ���ͷ�Ʈ ����, ��� �����Ͱ� ���ŵǾ��� �� ���ͷ�Ʈ�� �߻���ų ������ ����
	void   PASCAL EXPORT COMMset_recv_Threshold(INT16 ChannelNo, INT16 nValue);
	INT16  PASCAL EXPORT COMMget_recv_Threshold(INT16 ChannelNo);

	// Break
	void   PASCAL EXPORT COMMset_break(INT16 ChannelNo, BOOL fValue);
	BOOL   PASCAL EXPORT COMMget_break(INT16 ChannelNo);

	// Loop-back test
	// ������ ����/����
	void   PASCAL EXPORT COMMset_loop_back(INT16 ChannelNo, BOOL fValue);
	// ����/������ ������ �˻�
	BOOL   PASCAL EXPORT COMMget_loop_back(INT16 ChannelNo);

	// ��Ʈ(ä��)�� ��
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
	// ���ͷ�Ʈ ���
	BOOL   PASCAL EXPORT COMMenable_interrupt(INT16 ChannelNo);
	// ���ͷ�Ʈ�� ��� �����ΰ� ?
	BOOL   PASCAL EXPORT COMMis_enable_interrupt(INT16 ChannelNo);
	// ���ͷ�Ʈ ����
	void   PASCAL EXPORT COMMdisable_interrupt(INT16 ChannelNo);
	
// gun5 2002.12.26 add(version�� 1 �� ��� ���)
	void   PASCAL EXPORT COMMset_rs422_enable(INT16 nChannelNo, BOOL fValue);
	BOOL   PASCAL EXPORT COMMget_rs422_enable(INT16 nChannelNo);
	UINT16 PASCAL EXPORT COMMget_moduleversion(INT16 nModuleNo);

	INT16  PASCAL EXPORT COMMget_error_code();
	char*  PASCAL EXPORT COMMget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_COMM_H__