//---------------------------------------------------------------------------

#include <system.hpp>
#pragma hdrstop

#include "SAInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
PSV_InitVisn      SV_InitVisn      ;
PSV_SetVisnForm   SV_SetVisnForm   ;
PSV_DelVisn       SV_DelVisn       ;
PSV_LoadJobFile   SV_LoadJobFile   ;
PSV_Grab          SV_Grab          ;
PSV_Live          SV_Live          ;
PSV_GetGrabEnd    SV_GetGrabEnd    ;
PSV_SetLight      SV_SetLight      ;
PSV_Inspect       SV_Inspect       ;
PSV_GetInspEnd    SV_GetInspEnd    ;
PSV_GetRslt       SV_GetRslt       ;
PSV_SetLevel      SV_SetLevel      ;
PSV_SetAutorun    SV_SetAutorun    ;
PSV_SetActiveVisn SV_SetActiveVisn ;

HINSTANCE g_hVInst ;
LoadSVA(char * _pFileName)
{
    // DLL 로드
    g_hVInst = LoadLibrary (_pFileName);

    // 클래스 적재 함수 받음
    SV_InitVisn       = (PSV_InitVisn      )GetProcAddress(g_hVInst, TEXT("InitVisn"     ));
    SV_SetVisnForm    = (PSV_SetVisnForm   )GetProcAddress(g_hVInst, TEXT("SetVisnForm"  ));
    SV_DelVisn        = (PSV_DelVisn       )GetProcAddress(g_hVInst, TEXT("DelVisn"      ));
    SV_LoadJobFile    = (PSV_LoadJobFile   )GetProcAddress(g_hVInst, TEXT("LoadJobFile"  ));
    SV_Grab           = (PSV_Grab          )GetProcAddress(g_hVInst, TEXT("Grab"         ));
    SV_Live           = (PSV_Live          )GetProcAddress(g_hVInst, TEXT("Live"         ));
    SV_GetGrabEnd     = (PSV_GetGrabEnd    )GetProcAddress(g_hVInst, TEXT("GetGrabEnd"   ));
    SV_SetLight       = (PSV_SetLight      )GetProcAddress(g_hVInst, TEXT("SetLight"     ));
    SV_Inspect        = (PSV_Inspect       )GetProcAddress(g_hVInst, TEXT("Inspect"      ));
    SV_GetInspEnd     = (PSV_GetInspEnd    )GetProcAddress(g_hVInst, TEXT("GetInspEnd"   ));
    SV_GetRslt        = (PSV_GetRslt       )GetProcAddress(g_hVInst, TEXT("GetRslt"      ));
    SV_SetLevel       = (PSV_SetLevel      )GetProcAddress(g_hVInst, TEXT("SetLevel"     ));
    SV_SetAutorun     = (PSV_SetAutorun    )GetProcAddress(g_hVInst, TEXT("SetAutorun"   ));
    SV_SetActiveVisn  = (PSV_SetActiveVisn )GetProcAddress(g_hVInst, TEXT("SetActiveVisn"));
}

FreeSVA()
{

    FreeLibrary(g_hVInst);


}
