//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMsgTime.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmMsgTime *FrmMsgTime;

//---------------------------------------------------------------------------
void __fastcall MsgTime(AnsiString _sCaption , AnsiString _sMessage , int _iShowTime)
{
    //static AnsiString sPreTitle  = _sCaption ;
    //static AnsiString sPreMessage = _sMessage ;

    if (FrmMsgTime == NULL   ) return ;
    if (FrmMsgTime ->Visible ) FrmMsgTime -> Close() ;

    //Set Caption.
    FrmMsgTime -> lbTitle   -> Caption  = _sCaption  ;
    FrmMsgTime -> lbMessage -> Caption  = _sMessage  ;
    FrmMsgTime -> tmClose   -> Interval = _iShowTime ;

    //Show Form.
    PostMessage(FrmMsgTime -> Handle , WM_USER , 0 , 0);
}

//---------------------------------------------------------------------------
void  __fastcall CloseMsgOk()
{
    if (FrmMsgTime == NULL   ) return ;
    FrmMsgTime -> Close() ;
}

//---------------------------------------------------------------------------
__fastcall TFrmMsgTime::TFrmMsgTime(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMsgTime::WmUser(TMessage &Message)
{
    Message.Result = true;
    if (this == NULL || Visible) return;
    this -> Show();
}
void __fastcall TFrmMsgTime::FormDestroy(TObject *Sender)
{
    FrmMsgTime = NULL ;

        Trace("","");
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsgTime::tmCloseTimer(TObject *Sender)
{
    tmClose -> Enabled = false ;

    Close();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMsgTime::FormShow(TObject *Sender)
{
    tmClose -> Enabled = true ;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsgTime::FormHide(TObject *Sender)
{
    tmClose -> Enabled = false ;    
}
//---------------------------------------------------------------------------

