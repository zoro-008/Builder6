//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMode.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMode *FrmMode;
//---------------------------------------------------------------------------
__fastcall TFrmMode::TFrmMode(TComponent* Owner)
        : TForm(Owner)
{
    this -> Top    = 35  ;//989 ;
    this -> Left   = 300 ;
    this -> Width  = 300 ;//1280 ;//908 ;//1006
    this -> Height = 35  ;

    tmUpdate -> Enabled = true ;
}

void __fastcall TFrmMode::CreateParams(TCreateParams &Params)
{
    TForm::CreateParams(Params);
    Params.ExStyle = WS_EX_APPWINDOW;
    Params.WndParent = GetDesktopWindow();   
}


//---------------------------------------------------------------------------
void __fastcall TFrmMode::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;


    //if(!FrmMode -> Visible) FrmMode -> Show() ;
    static bool sFlick = false ;
    AnsiString sMode  ;
    TColor     tColor ;


    const int NORMAL      = 0 ;
    const int RE_ADJUST   = 1 ;
    const int LONG_RUN    = 2 ;
    const int ADJUST_ONLY = 3 ;
    const int DATA_CHECK  = 4 ;
    const int DISP_FIRST  = 5 ;

    switch(OM.DevOptn.iModeSelect) {
        case NORMAL       : sMode = "NORMAL"      ; tColor = sFlick ? clYellow : clWhite   ; break ;
        case RE_ADJUST    : sMode = "RE_ADJUST"   ; tColor = sFlick ? clYellow : clBlue    ; break ;
        case LONG_RUN     : sMode = "LONG_RUN"    ; tColor = sFlick ? clYellow : clGreen   ; break ;
        case ADJUST_ONLY  : sMode = "ADJUST_ONLY" ; tColor = sFlick ? clYellow : clRed     ; break ;
        case DATA_CHECK   : sMode = "DATA_CHECK"  ; tColor = sFlick ? clYellow : clLime    ; break ;
        case DISP_FIRST   : sMode = "DISP_FIRST"  ; tColor = sFlick ? clYellow : clFuchsia ; break ;
    }

    sFlick = !sFlick ;

    pnMode -> Caption       = sMode  ;
    pnMode -> Font -> Color = tColor ;


    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMode::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------





