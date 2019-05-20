//---------------------------------------------------------------------------

#ifndef SLogUnitH
#define SLogUnitH
//---------------------------------------------------------------------------

#define TRACE        OnSetInfo(__LINE__, __FUNC__, __FILE__); OnTrace
#define Trace        OnSetInfo(__LINE__, __FUNC__, __FILE__); OnTrace
#define ShowMessageT OnSetInfo(__LINE__, __FUNC__, __FILE__); OnShowMessage
extern "C" {
    __declspec(dllexport)bool __stdcall OnSetInfo    ( int    line  , char * func, char *file );
    __declspec(dllexport)void __stdcall OnTrace      ( char * header, char * msg );
    __declspec(dllexport)void __stdcall OnShowMessage( char * msg);
};
#endif


/*
해더 특수 명령.
<@EXIT> : 로그 프로그램 종료.
<@SETBUFF_01> : 로그 프로그램 해당 버퍼에 세팅.
<@GETBUFFALL> : 로그 프로그램 버퍼를 모두 로깅.   50개있음.



*/
