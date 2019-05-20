//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmOption.h"
//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "OptionMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOption *FormOption;
//---------------------------------------------------------------------------
__fastcall TFormOption::TFormOption(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormOption::FormShow(TObject *Sender)
{
    UpdateCmmOptn(toTabl);
}
//---------------------------------------------------------------------------

void __fastcall TFormOption::btSaveClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFormOption::UpdateCmmOptn(bool bTable)
{
    if(bTable) {
        edWK2StprDnDelay  ->Text    = OM.CmnOptn.iWK2StprDnDelay          ;
        edWK1_XInsVel     ->Text    = OM.CmnOptn.dWK1_XInsVel             ;
        cbAutoVsChange    ->Checked = OM.CmnOptn.bAutoVsChange            ;
        edPrbToWk1Delay   ->Text    = OM.CmnOptn.iPrbToWk1Delay           ;
        edPsbToOutDelay   ->Text    = OM.CmnOptn.iPsbToOutDelay           ;
//        cbEmptyNoMrk      ->Checked = OM.CmnOptn.bEmptyNoMrk              ;
    }
    else {
        OM.CmnOptn.iWK2StprDnDelay  = edWK2StprDnDelay  ->Text.ToIntDef(50)   ;
        OM.CmnOptn.dWK1_XInsVel     = edWK1_XInsVel     ->Text.ToDouble()     ;
        OM.CmnOptn.bAutoVsChange    = cbAutoVsChange    ->Checked             ;
        OM.CmnOptn.iPrbToWk1Delay   = edPrbToWk1Delay   ->Text.ToIntDef(1000) ;
        OM.CmnOptn.iPsbToOutDelay   = edPsbToOutDelay   ->Text.ToIntDef(1000) ;
//        OM.CmnOptn.bEmptyNoMrk      = cbEmptyNoMrk      ->Checked             ;


    }

}


