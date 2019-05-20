//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Motors.h"
#include "Actuators.h"
#include "ErrMan.h"
#include "SLogUnit.h"
#include "FormErr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmErr *FrmErr;

//---------------------------------------------------------------------------
void __fastcall ShowErr(int _iNo)
{
    Trace("ShowErr","before FrmErr == NULL");
    if (FrmErr == NULL   ) return ;
    if (FrmErr ->Visible ) return ;

    //Show Form.
    Trace("ShowErr","Before PostMessage");
    PostMessage(FrmErr -> Handle , WM_USER , 0 , 0);
    Trace("ShowErr","After PostMessage");

    //FrmErr -> Show() ;
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
    //Left = 826 ;
    //Top  = 396 ;
    //Position = poMainFormCenter ;
    //Position = poDesktopCenter ;

    Left = 414 ;
    Top  = 138 ;

    //tmUpdate -> Enabled = true;
}

//---------------------------------------------------------------------------
__fastcall TFrmErr::~TFrmErr()
{
    tmUpdate -> Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmErr::WmUser(TMessage &Message)
{
    Message.Result = true;
    if (this == NULL || Visible) return;
    this -> Show();
}

void __fastcall TFrmErr::btResetClick(TObject *Sender)
{
    MT.ResetAll();
    AT.Reset();
    EM.Clear();

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmErr::tmUpdateTimer(TObject *Sender)
{
    //if     (pnTitle -> Color == 0x00FFBBBB) pnTitle -> Color =  clRed             ;
    //else if(pnTitle -> Color == clRed     ) pnTitle -> Color = (TColor)0x00FFBBBB ;
    //TShape ±ôºý±ôºý
    spErrRect -> Visible = !spErrRect -> Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFrmErr::btCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


void __fastcall TFrmErr::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true ;
    //Position = poMainFormCenter ;
    if(!EM.GetUsedErrPictr()){
        imageErr -> Visible = false;
        Panel7   -> Visible = false;
        FrmErr   -> Height  = 320;
        Panel1   -> Height  = 41;
    }

    EM.DisplayErrForm(lbErrNo, lbName, mmErrAction, imageErr, spErrRect) ;

    btClose -> SetFocus() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmErr::FormHide(TObject *Sender)
{
    tmUpdate -> Enabled = false;
}
//---------------------------------------------------------------------------

