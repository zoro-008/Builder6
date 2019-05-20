#ifndef SVISIONH
#define SVISIONH

#define SVA_API	__declspec(dllexport)

extern "C" { //Alghorithm
    SVA_API bool __stdcall InitVisn     (                                     );
    SVA_API bool __stdcall DelVisn      (                                     );
    //SVA_API void __stdcall SetVisnName  (int    _iVisnId  , char * _sVisnName );
    SVA_API void __stdcall SetVisnForm  (int    _iVisnId  , void * _hParentWnd);
    SVA_API void __stdcall LoadJobFile  (char * _sDevName                     );
    SVA_API void __stdcall Ready        (int    _iVisnId                      );
    SVA_API void __stdcall Grab         (int    _iVisnId                      );
    SVA_API void __stdcall Live         (int    _iVisnId  , bool _bOn         );
    SVA_API void __stdcall LiveOffAll   (                                     );
    SVA_API bool __stdcall GetGrabEnd   (int    _iVisnId                      );
    SVA_API void __stdcall Inspect      (int    _iVisnId                      );
    SVA_API bool __stdcall GetInspEnd   (int    _iVisnId                      );
    SVA_API void __stdcall GrabInsp     (int    _iVisnId                      );
    SVA_API bool __stdcall GetRslt      (int    _iVisnId  , char  * _tRslt    );
    SVA_API void __stdcall SetLevel     (                   int    _iLevel    ); //0:oper 1: engineer 2:Master
    SVA_API void __stdcall SetAutorun   (                   bool   _bRun      );
    SVA_API int  __stdcall GetLastInsp  (                                     );
    SVA_API void __stdcall SetPkgID     (int    _iVisnId  , char * _sPkgID    );


}









#endif


