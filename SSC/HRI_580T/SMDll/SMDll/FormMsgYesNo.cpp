//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMsgYesNo.h"
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

