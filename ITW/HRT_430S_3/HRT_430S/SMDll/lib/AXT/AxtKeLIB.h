#ifndef __AXT_KE_LIBRARY_H__
#define __AXT_KE_LIBRARY_H__

#include "AxtLIBDef.h"

/*------------------------------------------------------------------------------------------------*
	AXTLIB Library - 통합라이브러리 및 베이스보드 관리, 1차 함수군
	적용제품
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

	/// EEPROM 관련 함수군
	// EEPROM Write word
	void   PASCAL EXPORT AxtEepromWriteWord(INT16 nBoardNo, INT16 eep_addr, UINT16 wValue);
	// EEPROM Read word
	UINT16 PASCAL EXPORT AxtEepromReadWord(INT16 nBoardNo, INT16 addr);

	/// <<보드 및 모듈 데이터R/W>>
	// 보드의 지정된 오프셋 번지에 데이터(바이트/워드)를 써 넣는다
	void   PASCAL EXPORT AxtBoardWriteByte(INT16 nBoardNo, INT16 offset, UINT8 byData);
	void   PASCAL EXPORT AxtBoardWriteWord(INT16 nBoardNo, INT16 offset, UINT16 wData);

	// 보드의 지정된 오프셋 번지에세 데이터(바이트/워드)를 읽는다
	UINT8  PASCAL EXPORT AxtBoardReadByte(INT16 nBoardNo, INT16 offset);
	UINT16 PASCAL EXPORT AxtBoardReadWord(INT16 nBoardNo, INT16 offset);

	// 보드의 지정된 모듈에서 지정한 오프셋 번지에 데이터(바이트/워드)를 써 넣는다
	void   PASCAL EXPORT AxtModuleWriteByte(INT16 nBoardNo, INT16 nModulePos, INT16 offset, UINT8 byData);
	void   PASCAL EXPORT AxtModuleWriteWord(INT16 nBoardNo, INT16 nModulePos, INT16 offset, UINT16 wData);

	// 보드의 지정된 모듈에서 지정한 오프셋 번지에세 데이터(바이트/워드)를 읽는다
	UINT8  PASCAL EXPORT AxtModuleReadByte(INT16 nBoardNo, INT16 nModulePos, INT16 offset);
	UINT16 PASCAL EXPORT AxtModuleReadWord(INT16 nBoardNo, INT16 nModulePos, INT16 offset);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_KE_LIBRARY_H__
