//---------------------------------------------------------------------------


#pragma hdrstop

#include "RsltData.h"

//---------------------------------------------------------------------------
TVisnRslt    g_VisnRslt   [MAX_STAGE_CNT];
TVisnRsltPnt g_VisnRsltPnt[MAX_STAGE_CNT];
THeightRslt  g_HeightRslt [MAX_STAGE_CNT];
TTiltData    g_TiltData   [MAX_STAGE_CNT];
TDriveData   g_DriveData  [MAX_STAGE_CNT];
bool         g_bInspEnd   [MAX_STAGE_CNT];


#pragma package(smart_init)
