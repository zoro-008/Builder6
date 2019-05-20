//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "SLogUnit.h"
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::btSaveClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateCmmOptn(bool _bToTable)
{
    if(_bToTable) {
        edWK2StprDnDelay  ->Text    = OM.CmnOptn.iWK2StprDnDelay          ;
        cbAutoVsChange    ->Checked = OM.CmnOptn.bAutoVsChange            ;
        edPrbToWk1Delay   ->Text    = OM.CmnOptn.iPrbToWk1Delay           ;
        edPsbToOutDelay   ->Text    = OM.CmnOptn.iPsbToOutDelay           ;
        cbAutoReview      ->Checked = OM.CmnOptn.bAutoReview              ;
        cbVision1Skip     ->Checked = OM.CmnOptn.bVs1Skip                 ;
        cbVision2Skip     ->Checked = OM.CmnOptn.bVs2Skip                 ;
        cbVision3Skip     ->Checked = OM.CmnOptn.bVs3Skip                 ;
        cbVision4Skip     ->Checked = OM.CmnOptn.bVsMrkSkip               ;
        edULD_VaccumDeley ->Text    = OM.CmnOptn.dULDVaccumDeley          ;
        edLDRInDeley      ->Text    = OM.CmnOptn.dLDRInDeley              ;
        cbUsePenMrkMvX    ->Checked = OM.CmnOptn.bUsePenMrkMvX            ;
        cbUsePenMrkMvY    ->Checked = OM.CmnOptn.bUsePenMrkMvY            ;
        edPenMrkMvX       ->Text    = OM.CmnOptn.dPenMrkMvX               ;
        edPenMrkMvY       ->Text    = OM.CmnOptn.dPenMrkMvY               ;
        edLDR_EmptyCnt    ->Text    = OM.CmnOptn.iLDREmptyCnt             ;
        cbUseLDREmpty     ->Checked = OM.CmnOptn.bUseLDREmpty             ;
        cbAfterMrkRemove  ->Checked = OM.CmnOptn.bAfterMrkRemove          ;
        edPSB_VisionTime  ->Text    = OM.CmnOptn.dPSB_VisionTime          ;
        edPSBReject       ->Text    = OM.CmnOptn.dPSBRejectDelay          ;
        edUldPkgEjtTime   ->Text    = OM.CmnOptn.iUldPkgEjtTime           ;
        edUldPprEjtTime   ->Text    = OM.CmnOptn.iUldPprEjtTime           ;


    }
    else {
        OM.CmnOptn.iWK2StprDnDelay  = edWK2StprDnDelay  ->Text.ToIntDef(50)   ;
        OM.CmnOptn.bAutoVsChange    = cbAutoVsChange    ->Checked             ;
        OM.CmnOptn.iPrbToWk1Delay   = edPrbToWk1Delay   ->Text.ToIntDef(1000) ;
        OM.CmnOptn.iPsbToOutDelay   = edPsbToOutDelay   ->Text.ToIntDef(1000) ;
        OM.CmnOptn.bAutoReview      = cbAutoReview      ->Checked             ;
        OM.CmnOptn.bVs1Skip         = cbVision1Skip     ->Checked             ;
        OM.CmnOptn.bVs2Skip         = cbVision2Skip     ->Checked             ;
        OM.CmnOptn.bVs3Skip         = cbVision3Skip     ->Checked             ;
        OM.CmnOptn.bVsMrkSkip       = cbVision4Skip     ->Checked             ;
        OM.CmnOptn.dULDVaccumDeley  = edULD_VaccumDeley ->Text.ToDouble()     ;
        OM.CmnOptn.dLDRInDeley      = edLDRInDeley      ->Text.ToDouble()     ;
        OM.CmnOptn.bUsePenMrkMvX    = cbUsePenMrkMvX    ->Checked             ;
        OM.CmnOptn.bUsePenMrkMvY    = cbUsePenMrkMvY    ->Checked             ;
        OM.CmnOptn.dPenMrkMvX       = edPenMrkMvX       ->Text.ToDouble()     ;
        OM.CmnOptn.dPenMrkMvY       = edPenMrkMvY       ->Text.ToDouble()     ;
        OM.CmnOptn.iLDREmptyCnt     = edLDR_EmptyCnt    ->Text.ToInt()        ;
        OM.CmnOptn.bUseLDREmpty     = cbUseLDREmpty     ->Checked             ;
        OM.CmnOptn.bAfterMrkRemove  = cbAfterMrkRemove  ->Checked             ;
        OM.CmnOptn.dPSB_VisionTime  = edPSB_VisionTime  ->Text.ToDouble()     ;
        OM.CmnOptn.dPSBRejectDelay  = edPSBReject       ->Text.ToDouble()     ;

        OM.CmnOptn.iUldPkgEjtTime   = edUldPkgEjtTime   ->Text.ToIntDef(100)  ;
        OM.CmnOptn.iUldPprEjtTime   = edUldPprEjtTime   ->Text.ToIntDef(100)  ;


    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateCmmOptn(toTabl);
}
//---------------------------------------------------------------------------





