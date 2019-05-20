//---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include "nmc410s.h"
//---------------------------------------------------------------------------
extern long g_lPPS[8];
extern BYTE g_OutputIO;
extern UINT g_OutputOld[8];
extern UINT g_nBit[8];
extern NMCDATA NmcData;
extern int g_nDistMode[8];
extern NMCLOGICBIT NmcLogicData;
extern NMCOUTSTATUS NmcOut[2];
extern struct NMC_STATUS_EXT NmcStatusBit;

#endif
