//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmOption.h"
#include "FrmMaster.h"
//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "OptionMan.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
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
    String sTemp = "" ;
    if(bTable) {
        edPRBStprDnDelay  ->Text    = OM.CmnOptn.iPRBStprDnDelay          ;
        edWK1StprDnDelay  ->Text    = OM.CmnOptn.iWK1StprDnDelay          ;
        edWK2StprDnDelay  ->Text    = OM.CmnOptn.iWK2StprDnDelay          ;


        cbAutoVsChange    ->Checked = OM.CmnOptn.bAutoVsChange            ;
        cbIgnrEmptyTrm    ->Checked = OM.CmnOptn.bIgnrEmptyTrm            ;
        cbAutoOperation   ->Checked = OM.CmnOptn.bAutoOperation           ;
    }
    else {
        OM.CmnOptn.iPRBStprDnDelay  = edPRBStprDnDelay  ->Text.ToIntDef(50) ;
        OM.CmnOptn.iWK1StprDnDelay  = edWK1StprDnDelay  ->Text.ToIntDef(50) ;
        OM.CmnOptn.iWK2StprDnDelay  = edWK2StprDnDelay  ->Text.ToIntDef(50) ;

        OM.CmnOptn.bAutoVsChange    = cbAutoVsChange    ->Checked             ;
        OM.CmnOptn.bAutoOperation   = cbAutoOperation   ->Checked             ;


    }
    //  CmnOptn.bTrm1Skip
    //  CmnOptn.bTrmAllSkip
    //CmnOptn.bLoadingStop
    //CmnOptn.bVs1_1Skip
    //CmnOptn.bVs2_1Skip
    //CmnOptn.bVs3_1Skip
    //CmnOptn.bVs1_2Skip
    //CmnOptn.bVs2_2Skip
    //CmnOptn.bVs3_2Skip
    //CmnOptn.bDoorSkip
    //
    //  CmnOptn.bVsSndSkip
    //  CmnOptn.bAutoVsChange
    //CmnOptn.iWK1StprDnDelay
    //CmnOptn.iWK2StprDnDelay
    //CmnOptn.bEmptyNoTrm
    //CmnOptn.iPSBSortMoveDelay
    //CmnOptn.bUseTrimMachine
}   //CmnOptn.iCheckFailCnt


