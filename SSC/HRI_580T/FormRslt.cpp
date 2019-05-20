//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormRslt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmRslt *FrmRslt;
//---------------------------------------------------------------------------
__fastcall TFrmRslt::TFrmRslt(TComponent* Owner)
        : TForm(Owner)
{
/*

csFail  ) , //"Heat Fail"
csRslt1 ) , //"V Empty"
csRslt2 ) , //"V Chip"
csRslt3 ) , //"V Dust"
csRslt4 ) , //"V LFlow"
csRslt5 ) , //"V Broken"
csRslt6 ) , //"V Crack"
csRslt7 ) , //"V Burr"
csRslt8 ) , //"V Zener"
csRslt9 ) , //"V Wire"
csRslt10) , //"V Orient"
csRslt11) , //"V Pmeasure"
csRslt12) , //"V Pdist"
csRslt13) , //"V Match"
csRslt14) );//"V Flow"


lbFail  
lbRslt1 
lbRslt2 
lbRslt3 
lbRslt4 
lbRslt5
lbRslt6 
lbRslt7 
lbRslt8
lbRslt9 
lbRslt10
lbRslt11
lbRslt12
lbRslt13
lbRslt14

*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmRslt::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action = caFree ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmRslt::FormShow(TObject *Sender)
{
    Left = 5   ;
    Top  = 100 ;
}
//---------------------------------------------------------------------------

