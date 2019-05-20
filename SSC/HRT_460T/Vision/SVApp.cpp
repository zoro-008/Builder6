//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


#include "SVApp.h"
#include "VisionMan.h"
extern "C" SVA_API bool __stdcall InitVisn     (void * hMainWnd , void * hParaWnd , char * _sDevName){return VSM.InitVisn     (hMainWnd , hParaWnd , _sDevName );}
extern "C" SVA_API bool __stdcall DelVisn      (                                                    ){return VSM.DelVisn      (                                );}
extern "C" SVA_API void __stdcall SetImgForm   (void * hImgBaseWnd  , bool _bSettingMode            ){       VSM.SetImgForm   (hImgBaseWnd , _bSettingMode     );}
extern "C" SVA_API void __stdcall LoadJobFile  (bool _bLoad , char * _sDevName                      ){       VSM.LoadJobFile  (_bLoad , _sDevName              );}
extern "C" SVA_API void __stdcall Grab         (EN_VISN_ID _iVisnId                                 ){       VSM.Grab         (_iVisnId                        );}
extern "C" SVA_API void __stdcall Live         (EN_VISN_ID _iVisnId , bool _bOn                     ){       VSM.Live         (_iVisnId , _bOn                 );}
extern "C" SVA_API bool __stdcall GetGrabEnd   (EN_VISN_ID _iVisnId                                 ){return VSM.GetGrabEnd   (_iVisnId                        );}
extern "C" SVA_API void __stdcall SetLight     (EN_VISN_ID _iVisnId                                 ){       VSM.SetLight     (_iVisnId                        );}
extern "C" SVA_API void __stdcall Inspect      (EN_VISN_ID _iVisnId , bool _bUseThread              ){       VSM.Inspect      (_iVisnId , _bUseThread          );}
extern "C" SVA_API bool __stdcall GetInspEnd   (EN_VISN_ID _iVisnId                                 ){return VSM.GetInspEnd   (_iVisnId                        );}
extern "C" SVA_API int  __stdcall GetRslt      (EN_VISN_ID _iVisnId , TRetResult * _tRslt           ){       VSM.GetRslt      (_iVisnId , _tRslt               );}
extern "C" SVA_API void __stdcall SetLevel     (EN_OPER_LEVEL _iLevel                               ){return VSM.SetLevel     (_iLevel                         );}
extern "C" SVA_API void __stdcall SetAutorun   (bool _bAutorun                                      ){       VSM.SetAutorun   (_bAutorun                       );}
extern "C" SVA_API void __stdcall SetActiveVisn(EN_VISN_ID _iVisnId                                 ){       VSM.SetActiveVisn(_iVisnId                        );}













