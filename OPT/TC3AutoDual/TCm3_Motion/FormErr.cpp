//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "IOs.h"
#include "FormErr.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmErr *FrmErr;

//---------------------------------------------------------------------------
void __fastcall ShowErr(int _iNo , AnsiString _sCmmt , AnsiString _sSolut , AnsiString _sSetTime)
{
    if (FrmErr == NULL   ) return ;
    if (FrmErr ->Visible ) {
        FrmErr -> Close();
    }

    //Set Caption.
    FrmErr -> lbTitle   -> Caption = "ERR_"+(AnsiString)_iNo ;
    FrmErr -> lbName    -> Caption = _sCmmt      ;
    FrmErr -> lbSolut   -> Caption = _sSolut     ;
    FrmErr -> lbSetTime -> Caption = _sSetTime   ;

    //Show Form.
    PostMessage(FrmErr -> Handle , WM_USER , 0 , 0);
}

void __fastcall HideErr()
{
    if ( FrmErr == NULL   ) return ;
    if (!FrmErr ->Visible ) return ;

    PostMessage(FrmErr -> Handle , WM_CLOSE , 0 , 0 );

}
//---------------------------------------------------------------------------
__fastcall TFrmErr::TFrmErr(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmErr::WmUser(TMessage &Message)
{
    Message.Result = true;
    if (this == NULL) return;
    this -> Show();
}

void __fastcall TFrmErr::btOkClick(TObject *Sender)
{
    SEQ.Reset() ;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmErr::tmUpdateTimer(TObject *Sender)
{
    if     (pnTitle -> Color == 0x00FFBBBB) pnTitle -> Color =  clRed             ;
    else if(pnTitle -> Color == clRed     ) pnTitle -> Color = (TColor)0x00FFBBBB ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmErr::FormShow(TObject *Sender)
{
    lbSetTime -> Caption = "SetTime : " + Now().FormatString("yyyy_mm_dd hh:nn:ss") ;
}
//---------------------------------------------------------------------------

