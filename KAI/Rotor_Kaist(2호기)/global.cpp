//---------------------------------------------------------------------------
#include <vcl.H>

#pragma hdrstop

#include "Global.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

long g_lPPS[8];
BYTE g_OutputIO;
UINT g_OutputOld[8];
UINT g_nBit[8];
NMCDATA NmcData;
int g_nDistMode[8];
NMCLOGICBIT NmcLogicData;
NMCOUTSTATUS NmcOut[2];
struct NMC_STATUS_EXT NmcStatusBit;

