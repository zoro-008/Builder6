//---------------------------------------------------------------------------

#include <system.hpp>
#pragma hdrstop

#include "SVInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
PSV_InitVisn    SV_InitVisn    =NULL;
PSV_DelVisn     SV_DelVisn     =NULL;
//PSV_SetVisnName SV_SetVisnName =NULL;
PSV_SetVisnForm SV_SetVisnForm =NULL;
PSV_LoadJobFile SV_LoadJobFile =NULL;
PSV_Grab        SV_Grab        =NULL;
PSV_Ready       SV_Ready       =NULL;
PSV_Live        SV_Live        =NULL;
PSV_LiveOffAll  SV_LiveOffAll  =NULL;
PSV_GetGrabEnd  SV_GetGrabEnd  =NULL;
PSV_Inspect     SV_Inspect     =NULL;
PSV_GetInspEnd  SV_GetInspEnd  =NULL;
PSV_GrabInsp    SV_GrabInsp    =NULL;
PSV_GetRslt     SV_GetRslt     =NULL;
PSV_SetLevel    SV_SetLevel    =NULL;
PSV_SetAutorun  SV_SetAutorun  =NULL;
PSV_GetLastInsp SV_GetLastInsp =NULL;
PSV_SetPkgID    SV_SetPkgID    =NULL;

HINSTANCE g_hVInst = NULL ;
bool LoadSVision(char * _pFileName)
{
    // DLL 로드
    g_hVInst = LoadLibrary (_pFileName);

    if(!g_hVInst) return false ;

    // 클래스 적재 함수 받음
    SV_InitVisn    = (PSV_InitVisn   )GetProcAddress(g_hVInst, TEXT("InitVisn"    )); if(!SV_InitVisn   )return false ;
    SV_DelVisn     = (PSV_DelVisn    )GetProcAddress(g_hVInst, TEXT("DelVisn"     )); if(!SV_DelVisn    )return false ;
//    SV_SetVisnName = (PSV_SetVisnName)GetProcAddress(g_hVInst, TEXT("SetVisnName" )); if(!SV_SetVisnName)return false ;
    SV_SetVisnForm = (PSV_SetVisnForm)GetProcAddress(g_hVInst, TEXT("SetVisnForm" )); if(!SV_SetVisnForm)return false ;
    SV_LoadJobFile = (PSV_LoadJobFile)GetProcAddress(g_hVInst, TEXT("LoadJobFile" )); if(!SV_LoadJobFile)return false ;
    SV_Grab        = (PSV_Grab       )GetProcAddress(g_hVInst, TEXT("Grab"        )); if(!SV_Grab       )return false ;
    SV_Ready       = (PSV_Ready      )GetProcAddress(g_hVInst, TEXT("Ready"       )); if(!SV_Ready      )return false ;
    SV_Live        = (PSV_Live       )GetProcAddress(g_hVInst, TEXT("Live"        )); if(!SV_Live       )return false ;
    SV_LiveOffAll  = (PSV_LiveOffAll )GetProcAddress(g_hVInst, TEXT("LiveOffAll"  )); if(!SV_LiveOffAll )return false ;
    SV_GetGrabEnd  = (PSV_GetGrabEnd )GetProcAddress(g_hVInst, TEXT("GetGrabEnd"  )); if(!SV_GetGrabEnd )return false ;
    SV_Inspect     = (PSV_Inspect    )GetProcAddress(g_hVInst, TEXT("Inspect"     )); if(!SV_Inspect    )return false ;
    SV_GetInspEnd  = (PSV_GetInspEnd )GetProcAddress(g_hVInst, TEXT("GetInspEnd"  )); if(!SV_GetInspEnd )return false ;
    SV_GrabInsp    = (PSV_GrabInsp   )GetProcAddress(g_hVInst, TEXT("GrabInsp"    )); if(!SV_GrabInsp   )return false ;
    SV_GetRslt     = (PSV_GetRslt    )GetProcAddress(g_hVInst, TEXT("GetRslt"     )); if(!SV_GetRslt    )return false ;
    SV_SetLevel    = (PSV_SetLevel   )GetProcAddress(g_hVInst, TEXT("SetLevel"    )); if(!SV_SetLevel   )return false ;
    SV_SetAutorun  = (PSV_SetAutorun )GetProcAddress(g_hVInst, TEXT("SetAutorun"  )); if(!SV_SetAutorun )return false ;
    SV_GetLastInsp = (PSV_GetLastInsp)GetProcAddress(g_hVInst, TEXT("GetLastInsp" )); if(!SV_GetLastInsp)return false ;
    SV_SetPkgID    = (PSV_SetPkgID   )GetProcAddress(g_hVInst, TEXT("SetPkgID"    )); if(!SV_SetPkgID   )return false ;

    return true ;
}

bool FreeSVision()
{
    if(g_hVInst==NULL) return false ;
    bool bRet = FreeLibrary(g_hVInst);
    g_hVInst=NULL;
    return bRet ;

}

















