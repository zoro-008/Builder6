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
�ش� Ư�� ���.
<@EXIT> : �α� ���α׷� ����.
<@SETBUFF_01> : �α� ���α׷� �ش� ���ۿ� ����.
<@GETBUFFALL> : �α� ���α׷� ���۸� ��� �α�.   50������.



*/
