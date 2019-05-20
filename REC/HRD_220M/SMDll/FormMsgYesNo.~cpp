//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMsgYesNo.h"
#include "SLogUnit.h"
#include "IOs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMsgYesNo *FrmMsgYesNo;

int __fastcall MsgYesNo(AnsiString _sCaption, AnsiString _sMessage)
{
    if (FrmMsgYesNo == NULL   ) return -1;
    if (FrmMsgYesNo ->Visible ) return -2;

    //Set Caption.
    FrmMsgYesNo -> lbTitle   -> Caption = _sCaption ;
    FrmMsgYesNo -> lbMessage -> Caption = _sMessage ;

    FrmMsgYesNo -> Timer1 -> Enabled = true ;
    //Show Form.
    return FrmMsgYesNo -> ShowModal();
}


//---------------------------------------------------------------------------
__fastcall TFrmMsgYesNo::TFrmMsgYesNo(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsgYesNo::btYesClick(TObject *Sender)
{
    Timer1 -> Enabled = false ;
    ModalResult = mrYes ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsgYesNo::btNoClick(TObject *Sender)
{
    ModalResult = mrNo ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsgYesNo::FormShow(TObject *Sender)
{
    btYes -> SetFocus() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsgYesNo::FormDestroy(TObject *Sender)
{
    Trace("","");
}
//---------------------------------------------------------------------------


void __fastcall TFrmMsgYesNo::Timer1Timer(TObject *Sender)
{
    Timer1 -> Enabled = false;
    if(IO.GetX(1)){
        keybd_event(0x20 , 0, 0, 0);
        keybd_event(0x20 , 0, KEYEVENTF_KEYUP, 0);
    }

    Timer1 -> Enabled = true ;
}
//---------------------------------------------------------------------------

