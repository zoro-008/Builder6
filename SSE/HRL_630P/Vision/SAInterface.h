//---------------------------------------------------------------------------

#ifndef SAInterfaceH
#define SAInterfaceH

#include "VisnDefine.h"
//---------------------------------------------------------------------------
typedef bool (__stdcall *PSV_InitVisn     )(void * hMainWnd , void * hParaWnd , char * _sDevName);
typedef bool (__stdcall *PSV_DelVisn      )(                                                    );
typedef void (__stdcall *PSV_SetImgForm   )(void * hImgBaseWnd  , bool _bSettingMode            );
typedef void (__stdcall *PSV_LoadJobFile  )(bool _bLoad , char * _sDevName                      );
typedef void (__stdcall *PSV_Grab         )(EN_VISN_ID _iVisnId                                 );
typedef void (__stdcall *PSV_Live         )(EN_VISN_ID _iVisnId , bool _bOn                     );
typedef bool (__stdcall *PSV_GetGrabEnd   )(EN_VISN_ID _iVisnId                                 );
typedef void (__stdcall *PSV_SetLight     )(EN_VISN_ID _iVisnId                                 );
typedef void (__stdcall *PSV_Inspect      )(EN_VISN_ID _iVisnId , bool _bUseThread              );
typedef bool (__stdcall *PSV_GetInspEnd   )(EN_VISN_ID _iVisnId                                 );
typedef int  (__stdcall *PSV_GetRslt      )(EN_VISN_ID _iVisnId , TRetResult * _tRslt           );
typedef void (__stdcall *PSV_SetLevel     )(EN_OPER_LEVEL _iLevel                               );
typedef void (__stdcall *PSV_SetAutorun   )(bool _bAutorun                                      );
typedef void (__stdcall *PSV_SetActiveVisn)(EN_VISN_ID _iVisnId                                 );

extern PSV_InitVisn      SV_InitVisn      ;
extern PSV_DelVisn       SV_DelVisn       ;
extern PSV_SetImgForm    SV_SetImgForm    ;
extern PSV_LoadJobFile   SV_LoadJobFile   ;
extern PSV_Grab          SV_Grab          ;
extern PSV_Live          SV_Live          ;
extern PSV_GetGrabEnd    SV_GetGrabEnd    ;
extern PSV_SetLight      SV_SetLight      ;
extern PSV_Inspect       SV_Inspect       ;
extern PSV_GetInspEnd    SV_GetInspEnd    ;
extern PSV_GetRslt       SV_GetRslt       ;
extern PSV_SetLevel      SV_SetLevel      ;
extern PSV_SetAutorun    SV_SetAutorun    ;
extern PSV_SetActiveVisn SV_SetActiveVisn ;

bool LoadSVA(char * _pFileName);
bool FreeSVA();


#endif
