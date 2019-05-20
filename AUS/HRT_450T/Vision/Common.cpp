//---------------------------------------------------------------------------


#pragma hdrstop

#include "Common.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


AnsiString g_sJobFileName ;
AnsiString g_sJobFilePath ;

EN_VISN_ID       g_iActivVisnId      ;
bool             g_bSettingMode      ; //현제 세팅창 모드인지.
EN_TRAIN_PAGE_ID g_iActivParaPageId  ; //현제 세팅창에서 어떤 창 모드인지.
EN_OPER_LEVEL    g_iCrntLevel        ; //현제 레벨.

int g_iYameRet ;









