#ifndef __AXT_LIBRARY_H__
#define __AXT_LIBRARY_H__

#include "AxtLIBDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTLIB Library - ���ն��̺귯�� �� ���̽����� ����
	������ǰ
		BIHR - ISA Half size, 2 module
		BIFR - ISA Full size, 4 module
		BPHR - PCI Half size, 2 module
		BPFR - PCI Full size, 4 module
		BV3R - VME 3U size, 2 module
		BV6R - VME 6U size, 4 module
		BC3R - CompactPCI 3U size, 2 module
		BC6R - CompactPCI 6U size, 4 module
 *------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	/// <<���ն��̺귯�� �ʱ�ȭ �� ����>>
	// ���� ���̺귯���� �ʱ�ȭ �Ѵ�..
	BOOL   PASCAL EXPORT AxtInitialize(HANDLE hWnd, INT16 nIrqNo);
	// ���� ���̺귯���� ��� �������� (�ʱ�ȭ�� �Ǿ�����)�� Ȯ���Ѵ�
	BOOL   PASCAL EXPORT AxtIsInitialized();
	// ���� ���̺귯���� ����� �����Ѵ�.
	void   PASCAL EXPORT AxtClose();

	/// <<���̽����� ���� �� �ݱ�>>
	// ������ ����(ISA, PCI, VME, CompactPCI)�� �ʱ�ȭ �Ǿ������� Ȯ���Ѵ�
	INT16  PASCAL EXPORT AxtIsInitializedBus(INT16 BusType);
	// ���ο� ���̽����带 ���ն��̺귯���� �߰��Ѵ�.
	INT16  PASCAL EXPORT AxtOpenDevice(INT16 BusType, UINT32 dwBaseAddr);
	// ���ο� ���̽����带 �迭�� �̿��Ͽ� �Ѳ����� ���ն��̺귯���� �߰��Ѵ�.
	INT16  PASCAL EXPORT AxtOpenDeviceAll(INT16 BusType, INT16 nLen, UINT32 *dwBaseAddr);
	// ���ο� ���̽����带 �ڵ����� ���ն��̺귯���� �߰��Ѵ�.
	INT16  PASCAL EXPORT AxtOpenDeviceAuto(INT16 BusType);
	// �߰��� ���̽����带 ���� �ݴ´�
	void   PASCAL EXPORT AxtCloseDeviceAll();

	/// <<���̽����������ͷ�Ʈ ����� �㰡 �� ����>>
	// ���̽������� ���ͷ�Ʈ�� ����� �㰡�Ѵ�
	void   PASCAL EXPORT AxtEnableInterrupt(INT16 nBoardNo);
	// ���̽������� ���ͷ�Ʈ�� ��� ���������� Ȯ���Ѵ�
	BOOL   PASCAL EXPORT AxtIsEnableInterrupt(INT16 nBoardNo);
	// ���̽������� ���ͷ�Ʈ�� ����� �����Ѵ�
	void   PASCAL EXPORT AxtDisableInterrupt(INT16 nBoardNo);

	// <<���̽������� ���ͷ�Ʈ ����ũ �� �÷��� ��������>>
	// ���̽������� ���ͷ�Ʈ �÷��� �������͸� Ŭ���� �Ѵ�
	void   PASCAL EXPORT AxtInterruptFlagClear(INT16 nBoardNo);
	// ���̽����忡 ������ �� ����� ���ͷ�Ʈ�� ����� �� �ֵ��� �ش� ���� ����� ����Ѵ�
	void   PASCAL EXPORT AxtWriteInterruptMaskModule(INT16 nBoardNo, UINT8 Mask);
	// ������ ���ͷ�Ʈ ����ũ �������͸� �д´�
	UINT8  PASCAL EXPORT AxtReadInterruptMaskModule(INT16 nBoardNo);
	// ���̽������� ���ͷ�Ʈ �÷��� ���������� ������ �д´�
	UINT8  PASCAL EXPORT AxtReadInterruptFlagModule(INT16 nBoardNo);

	/// <<���� ����>>
	// ������ ������ (���µ�) ���̽����� ������ �����Ѵ�
	INT16  PASCAL EXPORT AxtGetBoardCounts();
	// (���µ�) ��� ���̽����� ������ �����Ѵ�
	INT16  PASCAL EXPORT AxtGetBoardCountsBus(INT16 nBusType);
	// ������ ���̽����忡 ������ ����� ID �� ����� ������ �����Ѵ�
	INT16  PASCAL EXPORT AxtGetModuleCounts(INT16 nBoardNo, UINT8 *ModuleID);
	// ������ ���̽����忡 ������ ����� ������ ��� ID�� ������ �����Ѵ�
	INT16  PASCAL EXPORT AxtGetModelCounts(INT16 nBoardNo, UINT8 ModuleID);
	// ��� ���̽����忡 ������ ����� ������ ���ID�� ���� ����� ������ �����Ѵ�
	INT16  PASCAL EXPORT AxtGetModelCountsAll(UINT8 ModuleID);
	// ������ ���̽������� ID�� �����Ѵ�
	INT16  PASCAL EXPORT AxtGetBoardID(INT16 nBoardNo);
	// ������ ���̽������� Adress�� �����Ѵ�.
	UINT32 PASCAL EXPORT AxtGetBoardAddress(INT16 nBoardNo);

	// Log Level�� �����Ѵ�.
	void   PASCAL EXPORT AxtSetLogLevel(INT16 nLogLevel);
	// Log Level�� Ȯ���Ѵ�.
	INT16  PASCAL EXPORT AxtGetLogLevel();

	/// Library Version Infomation
	char * PASCAL EXPORT AxtGetLibVersion();
	char * PASCAL EXPORT AxtGetLibDate();

	INT16  PASCAL EXPORT Axtget_error_code();
	char * PASCAL EXPORT Axtget_error_msg(INT16 ErrorCode);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_LIBRARY_H__
