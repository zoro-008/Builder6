//---------------------------------------------------------------------------

#ifndef SVInterfaceH
#define SVInterfaceH

//#include "VisnDefine.h"
//---------------------------------------------------------------------------
typedef bool (__stdcall *PSV_InitVisn    )(                                     );
typedef bool (__stdcall *PSV_DelVisn     )(                                     );
//typedef void (__stdcall *PSV_SetVisnName )(int    _iVisnId  , char * _sVisnNo   );
typedef void (__stdcall *PSV_SetVisnForm )(int    _iVisnId  , void * _hParentWnd);
typedef void (__stdcall *PSV_LoadJobFile )(char * _sDevName                     );
typedef void (__stdcall *PSV_Ready       )(int    _iVisnId                      );
typedef void (__stdcall *PSV_Grab        )(int    _iVisnId                      );
typedef void (__stdcall *PSV_Live        )(int    _iVisnId  , bool _bOn         );
typedef void (__stdcall *PSV_LiveOffAll  )(                                     );
typedef bool (__stdcall *PSV_GetGrabEnd  )(int    _iVisnId                      );
typedef void (__stdcall *PSV_Inspect     )(int    _iVisnId                      );
typedef bool (__stdcall *PSV_GetInspEnd  )(int    _iVisnId                      );
typedef void (__stdcall *PSV_GrabInsp    )(int    _iVisnId                      );
typedef bool (__stdcall *PSV_GetRslt     )(int    _iVisnId  , char  * _tRslt    );  //Char *는 메모리 만들어서 넘겨야 함.
typedef void (__stdcall *PSV_SetLevel    )(                   int    _iLevel    ); //0:oper 1: engineer 2:Master
typedef void (__stdcall *PSV_SetAutorun  )(                   bool   _bRun      );
typedef int  (__stdcall *PSV_GetLastInsp )(                                     ); //마지막 검사 끝난 아이디.
typedef void (__stdcall *PSV_SetPkgID    )(int    _iVisnId  , char  * _sPkgID   ); //이미지저장용 PKG ID



extern PSV_InitVisn    SV_InitVisn    ;
extern PSV_DelVisn     SV_DelVisn     ;
//extern PSV_SetVisnName SV_SetVisnName ;
extern PSV_SetVisnForm SV_SetVisnForm ;
extern PSV_LoadJobFile SV_LoadJobFile ;
extern PSV_Ready       SV_Ready       ;
extern PSV_Grab        SV_Grab        ;
extern PSV_Live        SV_Live        ;
extern PSV_LiveOffAll  SV_LiveOffAll  ;
extern PSV_GetGrabEnd  SV_GetGrabEnd  ;
extern PSV_Inspect     SV_Inspect     ;
extern PSV_GetInspEnd  SV_GetInspEnd  ;
extern PSV_GrabInsp    SV_GrabInsp    ;
extern PSV_GetRslt     SV_GetRslt     ;
extern PSV_SetLevel    SV_SetLevel    ;
extern PSV_SetAutorun  SV_SetAutorun  ;
extern PSV_GetLastInsp SV_GetLastInsp ;
extern PSV_SetPkgID    SV_SetPkgID    ;







/*

extern PSV_InitVisn    SV_InitVisn
extern PSV_DelVisn     SV_DelVisn
extern PSV_SetVisnName SV_SetVisnName
extern PSV_SetImgForm  SV_SetImgForm
extern PSV_LoadJobFile SV_LoadJobFile
extern PSV_Grab        SV_Grab
extern PSV_Live        SV_Live
extern PSV_GetGrabEnd  SV_GetGrabEnd
extern PSV_Inspect     SV_Inspect
extern PSV_GetInspEnd  SV_GetInspEnd
extern PSV_GrabInsp    SV_GrabInsp
extern PSV_GetRslt     SV_GetRslt

typedef bool (__stdcall *PSV_InitVisn     )(int    _iVisnCnt    , char * _sDevName              );
typedef bool (__stdcall *PSV_DelVisn      )(                                                    );
typedef void (__stdcall *PSV_SetImgForm   )(void * hImgBaseWnd  , bool _bSettingMode            );
typedef void (__stdcall *PSV_LoadJobFile  )(bool _bLoad , char * _sDevName                      );
typedef void (__stdcall *PSV_Grab         )(int        _iVisnId                                 );
typedef void (__stdcall *PSV_Live         )(int        _iVisnId , bool _bOn                     );
typedef bool (__stdcall *PSV_GetGrabEnd   )(int        _iVisnId                                 );
typedef void (__stdcall *PSV_SetLight     )(int        _iVisnId                                 );
typedef void (__stdcall *PSV_Inspect      )(int        _iVisnId , bool _bUseThread              );
typedef bool (__stdcall *PSV_GetInspEnd   )(int        _iVisnId                                 );
typedef int  (__stdcall *PSV_GetRslt      )(int        _iVisnId , bool * _tRslt                 );
typedef void (__stdcall *PSV_SetLevel     )(int        _iLevel                                  );
typedef void (__stdcall *PSV_SetAutorun   )(bool _bAutorun                                      );
typedef void (__stdcall *PSV_SetActiveVisn)(int  _iVisnId                                       );

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
*/
bool LoadSVision(char * _pFileName);
bool FreeSVision();


#endif
