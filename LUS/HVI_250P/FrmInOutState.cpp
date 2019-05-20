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
    IOS.SetInPutWindow (pnInput ,9,8); //Panel , Font Size , ColCnt
    IOS.SetOutPutWindow(pnOutput,9,8);
}
//---------------------------------------------------------------------------
