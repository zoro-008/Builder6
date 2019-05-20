//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMsgOk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmMsgOk *FrmMsgOk;

//---------------------------------------------------------------------------
void __fastcall TFrmMsgOk::MsgOk(AnsiString _sCaption , AnsiString _sMessage)
{
    if (FrmMsgOk == NULL   ) return ;
    if (FrmMsgOk ->Visible ) return ;

    //Set Caption.
    FrmMsgOk -> lbTitle   -> Caption = _sCaption ;
    FrmMsgOk -> lbMessage -> Caption = _sMessage ;

    //Show Form.
    PostMessage(FrmMsgOk -> Handle , WM_USER , 0 , 0);
}

//---------------------------------------------------------------------------
void __fastcall TFrmMsgOk::MsgOkModal(AnsiString _sCaption , AnsiString _sMessage)
{
    //static AnsiString sPreTitle  = _sCaption ;
    //static AnsiString sPreMessage = _sMessage ;

    if (FrmMsgOk == NULL   ) return ;
    if (FrmMsgOk ->Visible ) return ;

    //Set Caption.
    FrmMsgOk -> lbTitle   -> Caption = _sCaption ;
    FrmMsgOk -> lbMessage -> Caption = _sMessage ;

    //Show Form.
    FrmMsgOk -> ShowModal() ;
}

void  __fastcall TFrmMsgOk::CloseMsgOk()
{
    if (FrmMsgOk == NULL   ) return ;
    FrmMsgOk -> Close() ;
}

//---------------------------------------------------------------------------
__fastcall TFrmMsgOk::TFrmMsgOk(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMsgOk::WmUser(TMessage &Message)
{
    Message.Result = true;
    if (this == NULL || Visible) return;
    this -> Show();
}
void __fastcall TFrmMsgOk::btOkClick(TObject *Sender)
{
    ModalResult = mrOk ;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMsgOk::FormDestroy(TObject *Sender)
{
    FrmMsgOk = NULL ;
}
//---------------------------------------------------------------------------

