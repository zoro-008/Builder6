//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "PstBuff.h"
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
    lbRslt0  -> Caption = OM.CmnOptn.sRsltName0 ;   pnRsltColor0 -> Color = (TColor)OM.CmnOptn.iRsltColor0 ;
    lbRslt1  -> Caption = OM.CmnOptn.sRsltName1 ;   pnRsltColor1 -> Color = (TColor)OM.CmnOptn.iRsltColor1 ;
    lbRslt2  -> Caption = OM.CmnOptn.sRsltName2 ;   pnRsltColor2 -> Color = (TColor)OM.CmnOptn.iRsltColor2 ;
    lbRslt3  -> Caption = OM.CmnOptn.sRsltName3 ;   pnRsltColor3 -> Color = (TColor)OM.CmnOptn.iRsltColor3 ;
    lbRslt4  -> Caption = OM.CmnOptn.sRsltName4 ;   pnRsltColor4 -> Color = (TColor)OM.CmnOptn.iRsltColor4 ;
    lbRslt5  -> Caption = OM.CmnOptn.sRsltName5 ;   pnRsltColor5 -> Color = (TColor)OM.CmnOptn.iRsltColor5 ;
    lbRslt6  -> Caption = OM.CmnOptn.sRsltName6 ;   pnRsltColor6 -> Color = (TColor)OM.CmnOptn.iRsltColor6 ;
    lbRslt7  -> Caption = OM.CmnOptn.sRsltName7 ;   pnRsltColor7 -> Color = (TColor)OM.CmnOptn.iRsltColor7 ;
    lbRslt8  -> Caption = OM.CmnOptn.sRsltName8 ;   pnRsltColor8 -> Color = (TColor)OM.CmnOptn.iRsltColor8 ;
    lbRslt9  -> Caption = OM.CmnOptn.sRsltName9 ;   pnRsltColor9 -> Color = (TColor)OM.CmnOptn.iRsltColor9 ;
    lbRsltA  -> Caption = OM.CmnOptn.sRsltNameA ;   pnRsltColorA -> Color = (TColor)OM.CmnOptn.iRsltColorA ;
    lbRsltB  -> Caption = OM.CmnOptn.sRsltNameB ;   pnRsltColorB -> Color = (TColor)OM.CmnOptn.iRsltColorB ;
    lbRsltC  -> Caption = OM.CmnOptn.sRsltNameC ;   pnRsltColorC -> Color = (TColor)OM.CmnOptn.iRsltColorC ;
    lbRsltD  -> Caption = OM.CmnOptn.sRsltNameD ;   pnRsltColorD -> Color = (TColor)OM.CmnOptn.iRsltColorD ;
    lbRsltE  -> Caption = OM.CmnOptn.sRsltNameE ;   pnRsltColorE -> Color = (TColor)OM.CmnOptn.iRsltColorE ;
    lbRsltF  -> Caption = OM.CmnOptn.sRsltNameF ;   pnRsltColorF -> Color = (TColor)OM.CmnOptn.iRsltColorF ;
    lbRsltG  -> Caption = OM.CmnOptn.sRsltNameG ;   pnRsltColorG -> Color = (TColor)OM.CmnOptn.iRsltColorG ;
    lbRsltH  -> Caption = OM.CmnOptn.sRsltNameH ;   pnRsltColorH -> Color = (TColor)OM.CmnOptn.iRsltColorH ;
    lbRsltI  -> Caption = OM.CmnOptn.sRsltNameI ;   pnRsltColorI -> Color = (TColor)OM.CmnOptn.iRsltColorI ;
    lbRsltJ  -> Caption = OM.CmnOptn.sRsltNameJ ;   pnRsltColorJ -> Color = (TColor)OM.CmnOptn.iRsltColorJ ;
    lbRsltK  -> Caption = OM.CmnOptn.sRsltNameK ;   pnRsltColorK -> Color = (TColor)OM.CmnOptn.iRsltColorK ;
    lbRsltL  -> Caption = OM.CmnOptn.sRsltNameL ;   pnRsltColorL -> Color = (TColor)OM.CmnOptn.iRsltColorL ;
}
//---------------------------------------------------------------------------



