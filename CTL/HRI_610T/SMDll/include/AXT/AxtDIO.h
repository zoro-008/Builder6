#ifndef __AXT_DIO_H__
#define __AXT_DIO_H__

#include "AxtLIBDef.h"
#include "DIODef.h"

/*------------------------------------------------------------------------------------------------*
	AXTDIO Library - Digital Input/Ouput module
	������ǰ
		SIO-DI32  - �Է� 32��
		SIO-DO32P - ��� 32��, ����Ŀ�÷� ���Ÿ��
		SIO-DO32T - ��� 32��, �Ŀ�TR ���Ÿ��
		SIO-DB32P - �Է� 16�� / ��� 32��, ����Ŀ�÷� ���Ÿ��
		SIO-DB32T - �Է� 16�� / ��� 32��, �Ŀ�TR ���Ÿ��
 *------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// �ʱ�ȭ �Լ���
	// DIO����� �ʱ�ȭ�Ѵ�. �����ִ� ��纣�̽����忡�� DIO����� �˻��Ͽ� �ʱ�ȭ�Ѵ�
	BOOL	PASCAL EXPORT InitializeDIO();
	// DIO����� ����� �� �ֵ��� ���̺귯���� �ʱ�ȭ�Ǿ��°� ?
	BOOL	PASCAL EXPORT DIOIsInitialized();
	//void	PASCAL EXPORT DIOStopService();

	// ���ͷ�Ʈ �޼��� �� �ڵ鷯�� �����Ѵ�.
	void	PASCAL EXPORT DIOSetWindowMessage(HANDLE hWnd, UINT16 wMsg, AXT_DIO_INTERRUPT_PROC proc);	

	/// ���ͷ�Ʈ ���� �Լ���
	// ������ ����� ���ͷ�Ʈ�� ����Ѵ�.
	void	PASCAL EXPORT DIOEnableInterrupt(INT16 nModuleNo);
	// ������ ����� ���ͷ�Ʈ�� �����Ѵ�.
	void	PASCAL EXPORT DIODisableInterrupt(INT16 nModuleNo);
	// ����� ���ͷ�Ʈ ������������ Ȯ���Ѵ�.
	BOOL	PASCAL EXPORT DIOIsInterruptEnabled(INT16 nModuleNo);

	/// ���� �� ��� ���� �Լ���
	// ������ ���̽����尡 �����ִ���(���µǾ�����)�� Ȯ���Ѵ�
	BOOL	PASCAL EXPORT DIOIsOpenBoard(INT16 nBoardNo);
	// ������ DIO����� �����ִ���(���µǾ�����)�� Ȯ���Ѵ�
	BOOL	PASCAL EXPORT DIOIsOpenModule(INT16 nModuleNo);
	// ��ȿ�� ����ȣ������ Ȯ���Ѵ�
	BOOL	PASCAL EXPORT DIOIsValidModuleNo(INT16 nModuleNo);
	// DIO����� ������ ���̽������� ������ �����Ѵ�
	UINT16	PASCAL EXPORT DIOget_board_count();
	// DIO����� ������ �����Ѵ�.
	UINT16	PASCAL EXPORT DIOget_module_count();
	// ������ ������ ����ϴ� ���̽����� ��ȣ�� �����Ѵ�
	INT16 	PASCAL EXPORT DIOget_boardno(UINT32 address);
		/*
			address : �������
			���ϰ�
				0..���̽�����-1
				-1	= ��ȿ���� �ʴ� ����
		*/

	// ������ ����� �𵨹�ȣ�� �����Ѵ�
	UINT16	PASCAL EXPORT DIOget_module_id(INT16 nModuleNo);
		/*
			���ϰ�
				97h(AXT_SIO_DI32)	= SIO-DI32
				98h(AXT_SIO_DO32P)	= SIO-DO32P
				99h(AXT_SIO_DB32P)	= SIO-DB32P
				9Eh(AXT_SIO_DO32T)	= SIO-DO32T
				9Fh(AXT_SIO_DB32T)	= SIO-DB32T
				00h	= ��ȿ���� �ʴ� ����ȣ
		*/
	// ������ ����� ���̽����峻 ��� ��ġ�� �����Ѵ�.
	INT16 	PASCAL EXPORT DIOget_module_pos(INT16 nModuleNo);
	// ������ ����� ��������� �����Ѵ�.
	UINT16	PASCAL EXPORT DIOget_output_number(INT16 nModuleNo);
	// ������ ����� �Է������� �����Ѵ�.
	UINT16	PASCAL EXPORT DIOget_input_number(INT16 nModuleNo);
	// ������ ���̽������� �����ġ�� �ִ� DIO����� ����ȣ�� �����Ѵ�.
	UINT16	PASCAL EXPORT DIOget_module_number(INT16 nBoardNo, INT16 nModulePos);
	// ������ ����ȣ�� ���̽������ȣ�� �����ġ�� �����Ѵ�
	BOOL	PASCAL EXPORT DIOget_module_info(INT16 nModuleNo, INT16 *pBoardNo, INT16 *nModulePos);
	// ������ ���� ����� ������ �����Ѵ�.
	UINT16	PASCAL EXPORT DIOget_open_module_count(INT16 ModuleID);
		/*
			ModuleID
				97h(AXT_SIO_DI32)	: SIO-DI32
				98h(AXT_SIO_DO32P)	: SIO-DO32P
				99h(AXT_SIO_DB32P)	: SIO-DB32P
				9Eh(AXT_SIO_DO32T)	: SIO-DO32T
				9Fh(AXT_SIO_DB32T)	: SIO-DB32T
				00h(DIO_MODULE_ALL)	: ��� DIO���
			���ϰ�	: ����� ������ �����Ѵ�

		*/

	/// Write port(Register) �Լ���
	// ���(Output) ��Ʈ�ο� 1��Ʈ�� �����͸� ��ִ´�. ���� ����
	void	PASCAL EXPORT DIOwrite_outport(UINT16 offset, BOOL bValue);
	// ���(Output) ��Ʈ�ο� 1��Ʈ�� �����͸� ��ִ´�. ������ ����� ���� ����
	void	PASCAL EXPORT DIOwrite_outport_bit(INT16 nModuleNo, UINT16 offset, BOOL bValue);
	// ���(Output) ��Ʈ�ο� 1����Ʈ�� �����͸� ��ִ´�. ������ ����� ����Ʈ ����
	void	PASCAL EXPORT DIOwrite_outport_byte(INT16 nModuleNo, UINT16 offset, UINT8 byValue);
	// ���(Output) ��Ʈ�ο� 2����Ʈ�� �����͸� ��ִ´�. ������ ����� ���� ����
	void	PASCAL EXPORT DIOwrite_outport_word(INT16 nModuleNo, UINT16 offset, UINT16 wValue);
	// ���(Output) ��Ʈ�ο� 4����Ʈ�� �����͸� ��ִ´�. ������ ����� ������� ����
	void	PASCAL EXPORT DIOwrite_outport_dword(INT16 nModuleNo, UINT16 offset, UINT32 lValue);
	// ���(Output) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�, ���� ����
	BOOL	PASCAL EXPORT DIOread_outport(UINT16 offset);
	// ���(Output) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�. ������ ����� ���� ����
	BOOL	PASCAL EXPORT DIOread_outport_bit(INT16 nModuleNo, UINT16 offset);
	// ���(Output) ��Ʈ�κ��� 1����Ʈ�� �����͸� �о���δ�, ������ ����� ����Ʈ ����
	UINT8	PASCAL EXPORT DIOread_outport_byte(INT16 nModuleNo, UINT16 offset);
	// ���(Output) ��Ʈ�κ��� 2����Ʈ�� �����͸� �о���δ�, ������ ����� ���� ����
	UINT16	PASCAL EXPORT DIOread_outport_word(INT16 nModuleNo, UINT16 offset);
	// ���(Output) ��Ʈ�κ��� 4����Ʈ�� �����͸� �о���δ�, ������ ����� ������� ����
	UINT32	PASCAL EXPORT DIOread_outport_dword(INT16 nModuleNo, UINT16 offset);
		/*
			offset
				DIOwrite_outport(),       DIOread_outport()			: 0���� ���������-1���� ��� ����
				DIOwrite_outport_bit(),   DIOread_outport_bit()		: SIO-DI32:���Ұ�, SIO-DB32:0..15, SIO-DO32:0..31
				DIOwrite_outport_byte(),  DIOread_outport_byte()	: SIO-DI32:���Ұ�, SIO-DB32:0..1,  SIO-DO32:0..3
				DIOwrite_outport_word(),  DIOread_outport_word()	: SIO-DI32:���Ұ�, SIO-DB32:0,     SIO-DO32:0..1
				DIOwrite_outport_dword(), DIOread_outport_dword()	: SIO-DI32:���Ұ�, SIO-DB32:0,     SIO-DO32:0
			���ϰ�
				DIOread_outport()		: 0(OFF), 1(ON)
				DIOread_outport_bit()	: 0(OFF), 1(ON)
				DIOread_outport_byte()	: 00h .. FFh
				DIOread_outport_word()	: 0000h .. FFFFh
				DIOread_outport_dword()	: 00000000h .. FFFFFFFFh
		*/

	/// Input port �Լ��� - �б� ���� ��Ʈ
	// �Է�(Input) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�. ���� ����
	BOOL	PASCAL EXPORT DIOread_inport(UINT16 offset);
	// �Է�(Input) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�. ������ ����� ���� ����
	BOOL	PASCAL EXPORT DIOread_inport_bit(INT16 nModuleNo, UINT16 offset);
	// �Է�(Input) ��Ʈ�κ��� 1����Ʈ�� �����͸� �о���δ�. ������ ����� ����Ʈ ����
	UINT8	PASCAL EXPORT DIOread_inport_byte(INT16 nModuleNo, UINT16 offset);
	// �Է�(Input) ��Ʈ�κ��� 2����Ʈ�� �����͸� �о���δ�. ������ ����� ���� ����
	UINT16	PASCAL EXPORT DIOread_inport_word(INT16 nModuleNo, UINT16 offset);
	// �Է�(Input) ��Ʈ�κ��� 4����Ʈ�� �����͸� �о���δ�. ������ ����� ������� ����
	UINT32	PASCAL EXPORT DIOread_inport_dword(INT16 nModuleNo, UINT16 offset);
		/*
			offset
				DIOread_inport()		: 0���� ���Է�����-1���� ��� ����
				DIOread_inport_bit()	: SIO-DI32:0..31, SIO-DB32:0..15, SIO-DO32:���Ұ�
				DIOread_inport_byte()	: SIO-DI32:0..3,  SIO-DB32:0..1,  SIO-DO32:���Ұ�
				DIOread_inport_word()	: SIO-DI32:0..1,  SIO-DB32:0,     SIO-DO32:���Ұ�
				DIOread_inport_dword()	: SIO-DI32:0,     SIO-DB32:0,     SIO-DO32:���Ұ�
			���ϰ�
				DIOread_inport()		: 0(OFF), 1(ON)
				DIOread_inport_bit()	: 0(OFF), 1(ON)
				DIOread_inport_byte()	: 00h .. FFh
				DIOread_inport_word()	: 0000h .. FFFFh
				DIOread_inport_dword()	: 00000000h .. FFFFFFFFh
		*/

	/// Interrupt Up-edge port(Register) �Լ���
	// ��¿���(Upedge) ��Ʈ�ο� 1��Ʈ�� �����͸� ��ִ´�. ���� ����
	void	PASCAL EXPORT DIOwrite_upedge(UINT16 offset, BOOL bValue);
	// ��¿���(Upedge) ��Ʈ�ο� 1��Ʈ�� �����͸� ��ִ´�. ������ ����� ���� ����
	void	PASCAL EXPORT DIOwrite_upedge_bit(INT16 nModuleNo, UINT16 offset, BOOL bValue);
	// ��¿���(Upedge) ��Ʈ�ο� 1����Ʈ�� �����͸� ��ִ´�. ������ ����� ����Ʈ ����
	void	PASCAL EXPORT DIOwrite_upedge_byte(INT16 nModuleNo, UINT16 offset, UINT8 byValue);
	// ��¿���(Upedge) ��Ʈ�ο� 2����Ʈ�� �����͸� ��ִ´�. ������ ����� ���� ����
	void	PASCAL EXPORT DIOwrite_upedge_word(INT16 nModuleNo, UINT16 offset, UINT16 wValue);
	// ��¿���(Upedge) ��Ʈ�ο� 4����Ʈ�� �����͸� ��ִ´�. ������ ����� ������� ����
	void	PASCAL EXPORT DIOwrite_upedge_dword(INT16 nModuleNo, UINT16 offset, UINT32 lValue);
	// ��¿���(Upedge) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�, ���� ����
	BOOL	PASCAL EXPORT DIOread_upedge(UINT16 offset);
	// ��¿���(Upedge) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�. ������ ����� ���� ����
	BOOL	PASCAL EXPORT DIOread_upedge_bit(INT16 nModuleNo, UINT16 offset);
	// ��¿���(Upedge) ��Ʈ�κ��� 1����Ʈ�� �����͸� �о���δ�, ������ ����� ����Ʈ ����
	UINT8	PASCAL EXPORT DIOread_upedge_byte(INT16 nModuleNo, UINT16 offset);
	// ��¿���(Upedge) ��Ʈ�κ��� 2����Ʈ�� �����͸� �о���δ�, ������ ����� ���� ����
	UINT16	PASCAL EXPORT DIOread_upedge_word(INT16 nModuleNo, UINT16 offset);
	// ��¿���(Upedge) ��Ʈ�κ��� 4����Ʈ�� �����͸� �о���δ�, ������ ����� ������� ����
	UINT32	PASCAL EXPORT DIOread_upedge_dword(INT16 nModuleNo, UINT16 offset);
		/*
			offset
				DIOwrite_upedge(),       DIOread_upedge()		: 0���� ���Է�����-1���� ��� ����
				DIOwrite_upedge_bit(),   DIOread_upedge_bit()	: SIO-DI32:0..31, SIO-DB32:0..15, SIO-DO32:���Ұ�
				DIOwrite_upedge_byte(),  DIOread_upedge_byte()	: SIO-DI32:0..3,  SIO-DB32:0..1,  SIO-DO32:���Ұ�
				DIOwrite_upedge_word(),  DIOread_upedge_word()	: SIO-DI32:0..1,  SIO-DB32:0,     SIO-DO32:���Ұ�
				DIOwrite_upedge_dword(), DIOread_upedge_dword()	: SIO-DI32:0,     SIO-DB32:0,     SIO-DO32:���Ұ�
			���ϰ�
				DIOread_upedge()		: 0(OFF), 1(ON)
				DIOread_upedge_bit()	: 0(OFF), 1(ON)
				DIOread_upedge_byte()	: 00h .. FFh
				DIOread_upedge_word()	: 0000h .. FFFFh
				DIOread_upedge_dword()	: 00000000h .. FFFFFFFFh
		*/

	/// Interrupt Down-edge port(Register) �Լ���
	// �ϰ�����(Downedge) ��Ʈ�ο� 1��Ʈ�� �����͸� ��ִ´�. ���� ����
	void	PASCAL EXPORT DIOwrite_downedge(UINT16 offset, BOOL bValue);
	// �ϰ�����(Downedge) ��Ʈ�ο� 1��Ʈ�� �����͸� ��ִ´�. ������ ����� ���� ����
	void	PASCAL EXPORT DIOwrite_downedge_bit(INT16 nModuleNo, UINT16 offset, BOOL bValue);
	// �ϰ�����(Downedge) ��Ʈ�ο� 1����Ʈ�� �����͸� ��ִ´�. ������ ����� ����Ʈ ����
	void	PASCAL EXPORT DIOwrite_downedge_byte(INT16 nModuleNo, UINT16 offset, UINT8 byValue);
	// �ϰ�����(Downedge) ��Ʈ�ο� 2����Ʈ�� �����͸� ��ִ´�. ������ ����� ���� ����
	void	PASCAL EXPORT DIOwrite_downedge_word(INT16 nModuleNo, UINT16 offset, UINT16 wValue);
	// �ϰ�����(Downedge) ��Ʈ�ο� 4����Ʈ�� �����͸� ��ִ´�. ������ ����� ������� ����
	void	PASCAL EXPORT DIOwrite_downedge_dword(INT16 nModuleNo, UINT16 offset, UINT32 lValue);
	// �ϰ�����(Downedge) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�, ���� ����
	BOOL	PASCAL EXPORT DIOread_downedge(UINT16 offset);
	// �ϰ�����(Downedge) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�. ������ ����� ���� ����
	BOOL	PASCAL EXPORT DIOread_downedge_bit(INT16 nModuleNo, UINT16 offset);
	// �ϰ�����(Downedge) ��Ʈ�κ��� 1����Ʈ�� �����͸� �о���δ�, ������ ����� ����Ʈ ����
	UINT8	PASCAL EXPORT DIOread_downedge_byte(INT16 nModuleNo, UINT16 offset);
	// �ϰ�����(Downedge) ��Ʈ�κ��� 2����Ʈ�� �����͸� �о���δ�, ������ ����� ���� ����
	UINT16	PASCAL EXPORT DIOread_downedge_word(INT16 nModuleNo, UINT16 offset);
	// �ϰ�����(Downedge) ��Ʈ�κ��� 4����Ʈ�� �����͸� �о���δ�, ������ ����� ������� ����
	UINT32	PASCAL EXPORT DIOread_downedge_dword(INT16 nModuleNo, UINT16 offset);
		/*
			offset
				DIOwrite_downedge(),       DIOread_downedge()		: 0���� ���Է�����-1���� ��� ����
				DIOwrite_downedge_bit(),   DIOread_downedge_bit()	: SIO-DI32:0..31, SIO-DB32:0..15, SIO-DO32:���Ұ�
				DIOwrite_downedge_byte(),  DIOread_downedge_byte()	: SIO-DI32:0..3,  SIO-DB32:0..1,  SIO-DO32:���Ұ�
				DIOwrite_downedge_word(),  DIOread_downedge_word()	: SIO-DI32:0..1,  SIO-DB32:0,     SIO-DO32:���Ұ�
				DIOwrite_downedge_dword(), DIOread_downedge_dword()	: SIO-DI32:0,     SIO-DB32:0,     SIO-DO32:���Ұ�
			���ϰ�
				DIOread_downedge()		: 0(OFF), 1(ON)
				DIOread_downedge_bit()	: 0(OFF), 1(ON)
				DIOread_downedge_byte()	: 00h .. FFh
				DIOread_downedge_word()	: 0000h .. FFFFh
				DIOread_downedge_dword(): 00000000h .. FFFFFFFFh
		*/
	
	/// Interrupt flag port(Register) �Լ���
	// ���ͷ�Ʈ�÷���(Flag) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�, ���� ����
	BOOL	PASCAL EXPORT DIOread_flag(UINT16 offset);
	// ���ͷ�Ʈ�÷���(Flag) ��Ʈ�κ��� 1��Ʈ�� �����͸� �о���δ�. ������ ����� ���� ����
	BOOL	PASCAL EXPORT DIOread_flag_bit(INT16 nModuleNo, UINT16 offset);
	// ���ͷ�Ʈ�÷���(Flag) ��Ʈ�κ��� 1����Ʈ�� �����͸� �о���δ�, ������ ����� ����Ʈ ����
	UINT8	PASCAL EXPORT DIOread_flag_byte(INT16 nModuleNo, UINT16 offset);
	// ���ͷ�Ʈ�÷���(Flag) ��Ʈ�κ��� 2����Ʈ�� �����͸� �о���δ�, ������ ����� ���� ����
	UINT16	PASCAL EXPORT DIOread_flag_word(INT16 nModuleNo, UINT16 offset);
	// ���ͷ�Ʈ�÷���(Flag) ��Ʈ�κ��� 4����Ʈ�� �����͸� �о���δ�, ������ ����� ������� ����
	UINT32	PASCAL EXPORT DIOread_flag_dword(INT16 nModuleNo, UINT16 offset);
		/*
			offset
				DIOread_flag()		: 0���� ���Է�����-1���� ��� ����
				DIOread_flag_bit()	: SIO-DI32:0..31, SIO-DB32:0..15, SIO-DO32:���Ұ�
				DIOread_flag_byte()	: SIO-DI32:0..3,  SIO-DB32:0..1,  SIO-DO32:���Ұ�
				DIOread_flag_word()	: SIO-DI32:0..1,  SIO-DB32:0,     SIO-DO32:���Ұ�
				DIOread_flag_dword(): SIO-DI32:0,     SIO-DB32:0,     SIO-DO32:���Ұ�
			���ϰ�
				DIOread_flag()		: 0(OFF), 1(ON)
				DIOread_flag_bit()	: 0(OFF), 1(ON)
				DIOread_flag_byte()	: 00h .. FFh
				DIOread_flag_word()	: 0000h .. FFFFh
				DIOread_flag_dword(): 00000000h .. FFFFFFFFh
		*/

/*----------------------- ���������� ����ϴ� ����(Parameter) ------------------------------------*
	nBoardNo	: ���̽������ȣ, ����� ������� 0���� �Ҵ�ȴ�
	nModuleNo	: DIO��� ��ȣ, DIO����� ������ ������� ����� ������� 0���� �Ҵ�ȴ�
 *------------------------------------------------------------------------------------------------*/

	INT16  PASCAL EXPORT DIOget_error_code();
	char * PASCAL EXPORT DIOget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_DIO_H__
