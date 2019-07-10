#ifndef __AXT_KE_LIBRARY_H__
#define __AXT_KE_LIBRARY_H__

#include "AxtLIBDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTLIB Library - ���ն��̺귯�� �� ���̽����� ����, 1�� �Լ���
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

	/// EEPROM ���� �Լ���
	// EEPROM Write word
	void   PASCAL EXPORT AxtEepromWriteWord(INT16 nBoardNo, INT16 eep_addr, UINT16 wValue);
	// EEPROM Read word
	UINT16 PASCAL EXPORT AxtEepromReadWord(INT16 nBoardNo, INT16 addr);

	/// <<���� �� ��� ������R/W>>
	// ������ ������ ������ ������ ������(����Ʈ/����)�� �� �ִ´�
	void   PASCAL EXPORT AxtBoardWriteByte(INT16 nBoardNo, INT16 offset, UINT8 byData);
	void   PASCAL EXPORT AxtBoardWriteWord(INT16 nBoardNo, INT16 offset, UINT16 wData);

	// ������ ������ ������ �������� ������(����Ʈ/����)�� �д´�
	UINT8  PASCAL EXPORT AxtBoardReadByte(INT16 nBoardNo, INT16 offset);
	UINT16 PASCAL EXPORT AxtBoardReadWord(INT16 nBoardNo, INT16 offset);

	// ������ ������ ��⿡�� ������ ������ ������ ������(����Ʈ/����)�� �� �ִ´�
	void   PASCAL EXPORT AxtModuleWriteByte(INT16 nBoardNo, INT16 nModulePos, INT16 offset, UINT8 byData);
	void   PASCAL EXPORT AxtModuleWriteWord(INT16 nBoardNo, INT16 nModulePos, INT16 offset, UINT16 wData);

	// ������ ������ ��⿡�� ������ ������ �������� ������(����Ʈ/����)�� �д´�
	UINT8  PASCAL EXPORT AxtModuleReadByte(INT16 nBoardNo, INT16 nModulePos, INT16 offset);
	UINT16 PASCAL EXPORT AxtModuleReadWord(INT16 nBoardNo, INT16 nModulePos, INT16 offset);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_KE_LIBRARY_H__
