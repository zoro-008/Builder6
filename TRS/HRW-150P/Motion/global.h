//---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include "nmc2.h"
//---------------------------------------------------------------------------
//extern short m_lInputXIo[128];
//extern short m_lOutputXIo[128];
//extern NMCAXESMOTIONOUT NmcMotion;
//extern NMCAXESEXPR NmcData;
//extern AnsiString sModelName;


extern AnsiString g_sModelName;

extern short g_lInputXIo[128];
extern short g_lOutputXIo[128];
extern short g_lInputEIo[32];
extern short g_lOutputEIo[32];
extern short g_lInputMIo[32];
extern short g_lOutputMIo[32];
extern short g_nStopInfo[8];
extern NMCAXESMOTIONOUT g_NmcMotionOut;
extern NMCAXESEXPR g_NmcData;
extern NMCHOMEFLAG g_Homeflag;


#endif
