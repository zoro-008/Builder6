//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormProbe.h"

#include "OptionMan.h"
#include "DataMan.h"
#include "Sequence.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmProbe *FrmProbe;
//---------------------------------------------------------------------------
__fastcall TFrmProbe::TFrmProbe(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::FormShow(TObject *Sender)
{
    Top    = 0 ;
    Left   = 1281 ;
    Width  = 1280 ;
    Height = 1024 ;

    //Left   = 0    ;

    Init();

    TARTUPINFO si;
    PROCESS_INFORMATION pi;
    HWND hWnd;
    TRect rc;

    memset(&si,0,si zeof(si));
    si.cb=sizeof(si);
    //
    CreateProcessA(NULL,"Amcap\\AmCap1.exe",NULL,NULL,true,0,NULL,NULL,&si,&pi);
    WaitForInputIdle(pi.hProcess,0xffffffff);
    Sleep(10);
    hWnd=GetForegroundWindow();
    GetWindowRect(hWnd,&rc);
    ::SetParent(hWnd,Panel100->Handle);
    SetWindowPos(hWnd,NULL,0,1000,rc.Width(),rc.Height(),0);
    if (hWnd) SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);


    Sleep(10);

    CreateProcessA(NULL,"Amcap\\AmCap2.exe",NULL,NULL,true,0,NULL,NULL,&si,&pi);
    WaitForInputIdle(pi.hProcess,0xffffffff);
    Sleep(10);
    hWnd=GetForegroundWindow();
    GetWindowRect(hWnd,&rc);
    ::SetParent(hWnd,Panel200->Handle);
    SetWindowPos(hWnd,NULL,0,1000,rc.Width(),rc.Height(),0);
    if (hWnd) SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    Sleep(10);
}
//---------------------------------------------------------------------------
void __fastcall TFrmProbe::Init(void)
{
    lbName   -> Caption = OM.EqpOptn.sModelName;
    lbDevice -> Caption = OM.GetCrntDev();=
}


