//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmInOutState.h"
#include "IoState.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormInOutState *FormInOutState;
//---------------------------------------------------------------------------
__fastcall TFormInOutState::TFormInOutState(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormInOutState::FormCreate(TObject *Sender)
{
    IOS.SetInPutWindow (pnInput ,8,8);
    IOS.SetOutPutWindow(pnOutput,8,8);
//    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFormInOutState::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormInOutState::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------

void __fastcall TFormInOutState::tmUpdateTimer(TObject *Sender)
{
    IOS.InUpdate()  ;
    IOS.OutUpdate() ;
}
//---------------------------------------------------------------------------

