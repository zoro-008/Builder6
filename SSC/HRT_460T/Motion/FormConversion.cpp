//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormConversion.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmConversion *FrmConversion;
//---------------------------------------------------------------------------
__fastcall TFrmConversion::TFrmConversion(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmConversion::tmFlickTimer(TObject *Sender)
{
    tmFlick -> Enabled = false ;

    spTop      -> Visible = !spTop      -> Visible ;
    spMask     -> Visible = !spMask     -> Visible ;
    spBoard    -> Visible = !spBoard    -> Visible ;
    spTopTrim  -> Visible = !spTopTrim  -> Visible ;
    spBtmTrim  -> Visible = !spBtmTrim  -> Visible ;

    spTop2     -> Visible = !spTop2     -> Visible ;
    spMask2    -> Visible = !spMask2    -> Visible ;
    spBoard2   -> Visible = !spBoard2   -> Visible ;
    spTopTrim2 -> Visible = !spTopTrim2 -> Visible ;
    spBtmTrim2 -> Visible = !spBtmTrim2 -> Visible ;

    lbWarning  -> Font -> Color = spBtmTrim2 -> Visible ? clRed : clBlack ;



    tmFlick -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmConversion::btOkClick(TObject *Sender)
{
    this -> Close() ;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmConversion::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmFlick -> Enabled = false;        
}
//---------------------------------------------------------------------------

