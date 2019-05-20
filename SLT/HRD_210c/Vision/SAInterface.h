//---------------------------------------------------------------------------

#ifndef SAInterfaceH
#define SAInterfaceH

//#include "SVImage.h"
#include "VisnDefine.h"
//---------------------------------------------------------------------------
typedef bool (__stdcall *PSV_InitVisn     )(char         * _sDevName);
typedef bool (__stdcall *PSV_SetVisnForm  )(void         * hWnd   );
typedef bool (__stdcall *PSV_DelVisn      )(                   );
typedef void (__stdcall *PSV_LoadJobFile  )(bool _bLoad , char * _sDevName);
typedef void (__stdcall *PSV_Grab         )(EN_VISN_ID _iVisnId );             //사진찍기.
typedef void (__stdcall *PSV_Live         )(EN_VISN_ID _iVisnId , bool _bOn);  //라이브 키기
typedef bool (__stdcall *PSV_GetGrabEnd   )(EN_VISN_ID _iVisnId );             //사진찍은거 이미지 받기 끝났는지.
typedef void (__stdcall *PSV_SetLight     )(EN_VISN_ID _iVisnId );             //조명 느릴경우 미리 조명 세팅 해놓기
typedef void (__stdcall *PSV_Inspect      )(EN_VISN_ID _iVisnId , bool _bUseThread); //검사.
typedef bool (__stdcall *PSV_GetInspEnd   )(EN_VISN_ID _iVisnId );                          //검사 끝??
typedef int  (__stdcall *PSV_GetRslt      )(EN_VISN_ID _iVisnId , TRetResult * _tRslt); //결과.
typedef void (__stdcall *PSV_SetLevel     )(int _iLevel);
typedef void (__stdcall *PSV_SetAutorun   )(bool _bAutorun);
typedef void (__stdcall *PSV_SetActiveVisn)(EN_VISN_ID _iVisnId);

extern PSV_InitVisn      SV_InitVisn      ;
extern PSV_SetVisnForm   SV_SetVisnForm   ;
extern PSV_DelVisn       SV_DelVisn       ;
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

LoadSVA(char * _pFileName);
FreeSVA();


#endif
