//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Motors.h"
#include "Actuators.h"
#include "ErrMan.h"
#include "FormErr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmErr *FrmErr = NULL;

//---------------------------------------------------------------------------
void __fastcall ShowErr(int _iNo)
{
    if (FrmErr == NULL   ) return ;
    if (FrmErr ->Visible ) return ;

    //Show Form.
    PostMessage(FrmErr -> Handle , WM_USER , 0 , 0);
    FrmErr -> tmUpdate -> Enabled = true ;

    //FrmErr -> Show() ;
}

void __fastcall HideErr()
{
    if ( FrmErr == NULL   ) return ;
    if (!FrmErr ->Visible ) return ;
    FrmErr -> tmUpdate -> Enabled = false;
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
void __fastcall TFrmErr::WmUser(TMessage &Message)
{
    Message.Result = true;
    if (this == NULL || Visible) return;
    Show();
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
 //   if     (pnTitle -> Color == 0x00FFBBBB) pnTitle -> Color =  clRed             ;
 //   else if(pnTitle -> Color == clRed     ) pnTitle -> Color = (TColor)0x00FFBBBB ;
 // TShape ±ôºý±ôºý
    tmUpdate -> Enabled = false ;
    spErrRect -> Visible  =  !spErrRect -> Visible ;
    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmErr::btCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


void __fastcall TFrmErr::FormShow(TObject *Sender)
{

    //Position = poMainFormCenter ;
    if(!EM.GetUsedErrPictr()){
        imageErr -> Visible = false;
        Panel7   -> Visible = false;
        FrmErr   -> Height  = 320;
        Panel1   -> Height  = 41;
    }

    EM.DisplayErrForm(lbErrNo, lbName, mmErrAction, mmErrMsg, imageErr, spErrRect) ;

    btClose -> SetFocus() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmErr::FormDestroy(TObject *Sender)
{
    Trace("","");        
}
//---------------------------------------------------------------------------

