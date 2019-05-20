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
        cbAutoOperation -> Checked = OM.CmnOptn.bAutoOperation ;
        cbAutoConvesion -> Checked = OM.CmnOptn.bAutoConvesion ;
        ed1IdxReWorkCnt -> Text    = OM.CmnOptn.i1IdxReWorkCnt ;
        edPunchAirDelay -> Text    = OM.CmnOptn.iPunchAirDelay ;
        edPreIdxOutSpd  -> Text    = OM.CmnOptn.dPreIdxOutSpd  ;


//        edLotEndMgzCnt -> Text = OM.DevOptn.iLotEndMgzCnt ;
//        edTrimColCnt   -> Text = OM.DevOptn.iTrimColCnt   ;
    }
    else {
        OM.CmnOptn.bAutoOperation = cbAutoOperation -> Checked ;
        OM.CmnOptn.bAutoConvesion = cbAutoConvesion -> Checked ;
        OM.CmnOptn.i1IdxReWorkCnt = ed1IdxReWorkCnt -> Text.ToIntDef(1) ;
        OM.CmnOptn.iPunchAirDelay = edPunchAirDelay -> Text.ToIntDef(0) ;
        OM.CmnOptn.dPreIdxOutSpd  = edPreIdxOutSpd  -> Text.ToDouble( ) ;
//        OM.DevOptn.iLotEndMgzCnt = edLotEndMgzCnt -> Text.ToIntDef(0) ;
//        OM.DevOptn.iTrimColCnt   = edTrimColCnt   -> Text.ToIntDef(0) ;
    }

}


