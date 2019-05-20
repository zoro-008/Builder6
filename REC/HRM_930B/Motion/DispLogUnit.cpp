//---------------------------------------------------------------------------


#pragma hdrstop

#include "DispLogUnit.h"

void * g_pHandle = NULL;
//---------------------------------------------------------------------------
void SendLog(String _sLog )
{
    if(g_pHandle==NULL) return ;

    static String sLog ;

    sLog = _sLog ;

    int nBuf = sLog.Length() ;

    DWORD  dwError = GetLastError();
    COPYDATASTRUCT cds;
    cds.cbData = nBuf + 1;
    cds.lpData = (void *) sLog.c_str();

    SendMessage(g_pHandle , WM_COPYDATA, NULL, (LPARAM)&cds);
}

void SetLogHandle(void * _pHandle)
{
    g_pHandle = _pHandle ;
}

#pragma package(smart_init)
