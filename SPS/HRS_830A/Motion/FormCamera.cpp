//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCamera.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCamera *FrmCamera;
//---------------------------------------------------------------------------
__fastcall TFrmCamera::TFrmCamera(TComponent* Owner)
        : TForm(Owner)
{
    Init();
}
//---------------------------------------------------------------------------
void TFrmCamera::Init()
{
    Top    = 0 ;
    Left   = 1281 ;
    Width  = 1280 ;
    Height = 1024 ;

    lbName   -> Caption = OM.EqpOptn.sModelName;
    lbDevice -> Caption = OM.GetCrntDev();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HWND hWnd;
    TRect rc;

    memset(&si,0,sizeof(si));
    si.cb=sizeof(si);

    CreateProcessA(NULL,"Amcap\\AmCap1.exe",NULL,NULL,true,0,NULL,NULL,&si,&pi);
    WaitForInputIdle(pi.hProcess,0xffffffff);
    Sleep(10);
    hWnd=GetForegroundWindow();
    GetWindowRect(hWnd,&rc);
    ::SetParent(hWnd,Panel1->Handle);
    SetWindowPos(hWnd,NULL,0,0,rc.Width(),rc.Height(),0);
    if (hWnd) SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    Sleep(10);

    CreateProcessA(NULL,"Amcap\\AmCap2.exe",NULL,NULL,true,0,NULL,NULL,&si,&pi);
    WaitForInputIdle(pi.hProcess,0xffffffff);
    Sleep(10);
    hWnd=GetForegroundWindow();
    GetWindowRect(hWnd,&rc);
    ::SetParent(hWnd,Panel2->Handle);
    SetWindowPos(hWnd,NULL,0,0,rc.Width(),rc.Height(),0);
    if (hWnd) SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    Sleep(10);

}

void __fastcall TFrmCamera::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;

    lbDisprLTime -> Caption = "Left Dispensor Time : " + ((AnsiString)OM.DevOptn.iDispsrLTimer);
    lbDisprRTime -> Caption = "Right Dispensor Time : " + ((AnsiString)OM.DevOptn.iDispsrRTimer);

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------

