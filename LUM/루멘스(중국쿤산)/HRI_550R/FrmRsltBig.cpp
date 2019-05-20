//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "OptionMan.h"
//---------------------------------------------------------------------------
#include "FrmRsltBig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRsltBig *FormRsltBig;
//---------------------------------------------------------------------------
__fastcall TFormRsltBig::TFormRsltBig(TComponent* Owner)
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



void __fastcall TFormRsltBig::FormShow(TObject *Sender)
{
    lbR1  -> Caption = OM.CmnOptn.sCheckR1FailName ;
    lbR2  -> Caption = OM.CmnOptn.sCheckR2FailName ;
    lbR3  -> Caption = OM.CmnOptn.sCheckR3FailName ;
    lbR4  -> Caption = OM.CmnOptn.sCheckR4FailName ;
    lbR5  -> Caption = OM.CmnOptn.sCheckR5FailName ;
    lbR6  -> Caption = OM.CmnOptn.sCheckR6FailName ;
    lbR7  -> Caption = OM.CmnOptn.sCheckR7FailName ;
    lbR8  -> Caption = OM.CmnOptn.sCheckR8FailName ;
    lbR9  -> Caption = OM.CmnOptn.sCheckR9FailName ;
    lbR10 -> Caption = OM.CmnOptn.sCheckR10FailName ;
    lbR11 -> Caption = OM.CmnOptn.sCheckR11FailName ;
    lbR12 -> Caption = OM.CmnOptn.sCheckR12FailName ;
    lbR13 -> Caption = OM.CmnOptn.sCheckR13FailName ;
    lbR14 -> Caption = OM.CmnOptn.sCheckR14FailName ;
}
//---------------------------------------------------------------------------

                                                                                                                     \

