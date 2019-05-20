//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMulti.h"
#include "FormRslt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMulti *FrmMulti;
//---------------------------------------------------------------------------
__fastcall TFrmMulti::TFrmMulti(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMulti::FormCreate(TObject *Sender)
{
    FrmRslt[0] = new TFrmRslt(pnBase0);
    FrmRslt[1] = new TFrmRslt(pnBase1);
    FrmRslt[2] = new TFrmRslt(pnBase2);
    FrmRslt[3] = new TFrmRslt(pnBase3);
    FrmRslt[4] = new TFrmRslt(pnBase4);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMulti::FormShow(TObject *Sender)
{
    FrmRslt[0] -> Parent = pnBase0; FrmRslt[0] -> SetStgId(0); FrmRslt[0] -> Show() ;
    FrmRslt[1] -> Parent = pnBase1; FrmRslt[1] -> SetStgId(1); FrmRslt[1] -> Show() ;
    FrmRslt[2] -> Parent = pnBase2; FrmRslt[2] -> SetStgId(2); FrmRslt[2] -> Show() ;
    FrmRslt[3] -> Parent = pnBase3; FrmRslt[3] -> SetStgId(3); FrmRslt[3] -> Show() ;
    FrmRslt[4] -> Parent = pnBase4; FrmRslt[4] -> SetStgId(4); FrmRslt[4] -> Show() ;

}


//---------------------------------------------------------------------------

