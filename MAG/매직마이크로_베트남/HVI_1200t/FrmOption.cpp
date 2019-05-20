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
    if(OM.MstOptn.bEmptyNoTrm && cbEmptyOnlyTrm -> Checked){
        if(FM_MsgYesNo("Inform","1차BM 자동 트림 또는 마킹 하시겠습니까?") == mrYes){
            OM.MstOptn.bEmptyNoTrm = false;
            FormMaster -> cbEmptyNoTrm -> Checked = false ;
            OM.SaveMstOptn();
            cbEmptyOnlyTrm -> Checked = true;
        }
        else {
            cbEmptyOnlyTrm -> Checked = false;
        }
    }
    if(bTable) {
        edPRBStprDnDelay  ->Text    = OM.CmnOptn.iPRBStprDnDelay          ;
        edWK1StprDnDelay  ->Text    = OM.CmnOptn.iWK1StprDnDelay          ;
        edWK2StprDnDelay  ->Text    = OM.CmnOptn.iWK2StprDnDelay          ;

        cbAutoVsChange    ->Checked = OM.CmnOptn.bAutoVsChange            ;
        cbAutoOperation   ->Checked = OM.CmnOptn.bAutoOperation           ;

        if(edLDRChangePs ->Text.ToIntDef(0) < 16 ) OM.CmnOptn.iLDRChangeCnt = 16 ; // dd
        edLDRChangePs     ->Text    = OM.CmnOptn.iLDRChangeCnt            ;
        edUldOutDelay     ->Text    = OM.CmnOptn.iUldOutDelay             ;
        cbDWidth          ->Checked = OM.CmnOptn.bDWidth                  ;
//        cbUsePenMrkMvDn   ->Checked = OM.CmnOptn.bUsePenMrkMvDn           ;
//        edPenMrkMoveDnLen ->Text    = OM.CmnOptn.dPenMrkMoveDnLen         ;

//        cbUsePenMrkMvX    ->Checked = OM.CmnOptn.bUsePenMrkMvX            ;
//        cbUsePenMrkMvY    ->Checked = OM.CmnOptn.bUsePenMrkMvY            ;
//        edPenMrkMvX       ->Text    = OM.CmnOptn.dPenMrkMvX               ;
//        edPenMrkMvY       ->Text    = OM.CmnOptn.dPenMrkMvY               ;

        cbEmptyOnlyTrm    ->Checked = OM.CmnOptn.bEmptyOnlyTrm            ;
        cb1stBMOnlyTrm    ->Checked = OM.CmnOptn.b1stBMOnlyTrm            ;

        //
    }
    else {
        OM.CmnOptn.iPRBStprDnDelay  = edPRBStprDnDelay  ->Text.ToIntDef(50) ;
        OM.CmnOptn.iWK1StprDnDelay  = edWK1StprDnDelay  ->Text.ToIntDef(50) ;
        OM.CmnOptn.iWK2StprDnDelay  = edWK2StprDnDelay  ->Text.ToIntDef(50) ;

        OM.CmnOptn.bAutoVsChange    = cbAutoVsChange    ->Checked             ;
        OM.CmnOptn.bAutoOperation   = cbAutoOperation   ->Checked             ;


        OM.CmnOptn.iUldOutDelay     = edUldOutDelay     ->Text.ToIntDef(1000) ;
        OM.CmnOptn.iLDRChangeCnt    = edLDRChangePs     ->Text.ToIntDef(16)   ;
        OM.CmnOptn.bDWidth          = cbDWidth          ->Checked             ;
//        OM.CmnOptn.bUsePenMrkMvDn   = cbUsePenMrkMvDn   ->Checked             ;
//        OM.CmnOptn.dPenMrkMoveDnLen = StrToFloatDef(edPenMrkMoveDnLen ->Text.ToDouble() , 0.0) ;

//        OM.CmnOptn.bUsePenMrkMvX    = cbUsePenMrkMvX    ->Checked    ;
//        OM.CmnOptn.bUsePenMrkMvY    = cbUsePenMrkMvY    ->Checked    ;
//        OM.CmnOptn.dPenMrkMvX       = StrToFloatDef(edPenMrkMvX ->Text.ToDouble() , 0.0) ;
//        OM.CmnOptn.dPenMrkMvY       = StrToFloatDef(edPenMrkMvY ->Text.ToDouble() , 0.0) ;

        OM.CmnOptn.bEmptyOnlyTrm    = cbEmptyOnlyTrm    ->Checked             ;
        OM.CmnOptn.b1stBMOnlyTrm    = cb1stBMOnlyTrm    ->Checked             ;


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


