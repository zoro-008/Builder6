//---------------------------------------------------------------------------
#include <vcl.H>

#pragma hdrstop
#include "Global.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//short m_lInputXIo[128];
//short m_lOutputXIo[128];
//NMCAXESMOTIONOUT NmcMotion;
//NMCAXESEXPR NmcData;
//AnsiString sModelName;//



AnsiString g_sModelName;//


short g_lInputXIo[128];
short g_lOutputXIo[128];
short g_lInputEIo[32];
short g_lOutputEIo[32];
short g_lInputMIo[32];
short g_lOutputMIo[32];
short g_nStopInfo[8];
NMCAXESMOTIONOUT g_NmcMotionOut;
NMCAXESEXPR g_NmcData; // 이것중에 엔코더값이랑 커멘드 값은 직접 가져오지 말고 GetEncPos , GetCmdPos를 쓴다.
NMCHOMEFLAG Homeflag;
