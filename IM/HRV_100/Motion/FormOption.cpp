//---------------------------------------------------------------------------

#include <vcl.h>
#include <FileCtrl.hpp>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
//#include "SPCUnit.h"
#include "DataMan.h"
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
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
        return;
    }
    //if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{                                                                                   
    if (_bToTable == toTabl) {
        edDataSavePath     -> Text    = OM.CmnOptn.sDataSavePath     ;
        cbAutoSaveRsltData -> Checked = OM.CmnOptn.bAutoSaveRsltData ;
        cbAutoSaveRsltImg  -> Checked = OM.CmnOptn.bAutoSaveRsltImg  ;
        cbAutoSaveRsltSc   -> Checked = OM.CmnOptn.bAutoSaveRsltSc   ;
        cbUseStage1        -> Checked = OM.CmnOptn.bUseStage1        ;
        cbUseStage2        -> Checked = OM.CmnOptn.bUseStage2        ;
        cbUseStage3        -> Checked = OM.CmnOptn.bUseStage3        ;
        cbUseStage4        -> Checked = OM.CmnOptn.bUseStage4        ;
        cbUseStage5        -> Checked = OM.CmnOptn.bUseStage5        ;
        edRegiGain         -> Text    = OM.CmnOptn.dRegiGain         ;
        edRegiofset        -> Text    = OM.CmnOptn.dRegiofset        ;
        edVisnYMaxDpMin    -> Text    = OM.CmnOptn.dVisnYMaxDpMin    ;
        edCrntGain         -> Text    = OM.CmnOptn.dCrntGain         ;
        edCrntOfst         -> Text    = OM.CmnOptn.dCrntOfst         ;
        edHghtGain         -> Text    = OM.CmnOptn.dHghtGain         ;
        edHghtOfst         -> Text    = OM.CmnOptn.dHghtOfst         ;
        cbDoubleHold       -> Checked = OM.CmnOptn.bDoubleHold       ; 

    }
    else {
        OM.CmnOptn.sDataSavePath     = edDataSavePath     -> Text    ;
        OM.CmnOptn.bAutoSaveRsltData = cbAutoSaveRsltData -> Checked ;
        OM.CmnOptn.bAutoSaveRsltImg  = cbAutoSaveRsltImg  -> Checked ;
        OM.CmnOptn.bAutoSaveRsltSc   = cbAutoSaveRsltSc   -> Checked ;
        OM.CmnOptn.bUseStage1        = cbUseStage1        -> Checked ;
        OM.CmnOptn.bUseStage2        = cbUseStage2        -> Checked ;
        OM.CmnOptn.bUseStage3        = cbUseStage3        -> Checked ;
        OM.CmnOptn.bUseStage4        = cbUseStage4        -> Checked ;
        OM.CmnOptn.bUseStage5        = cbUseStage5        -> Checked ;
        OM.CmnOptn.dRegiGain         = StrToFloatDef(edRegiGain  -> Text,1.0);
        OM.CmnOptn.dRegiofset        = StrToFloatDef(edRegiofset -> Text,0.0);

        OM.CmnOptn.dVisnYMaxDpMin     = StrToFloatDef(edVisnYMaxDpMin     -> Text    , OM.CmnOptn.dVisnYMaxDpMin);
        if(OM.CmnOptn.dVisnYMaxDpMin > OM.MstOptn.dVisnYMaxMin) OM.CmnOptn.dVisnYMaxDpMin = OM.MstOptn.dVisnYMaxMin ;

        OM.CmnOptn.dCrntGain         = StrToFloatDef(edCrntGain  -> Text,1.0);
        OM.CmnOptn.dCrntOfst         = StrToFloatDef(edCrntOfst  -> Text,0.0);

        OM.CmnOptn.dHghtGain         = StrToFloatDef(edHghtGain  -> Text,1.0);
        OM.CmnOptn.dHghtOfst         = StrToFloatDef(edHghtOfst  -> Text,0.0);

        OM.CmnOptn.bDoubleHold       = cbDoubleHold -> Checked ;

        UpdateComOptn(toTabl);
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
#ifdef COWELL
    lbRegiofset -> Caption = "Resistance Offset";
    lbRegiGain  -> Caption = "Resistance Gain"  ;
#endif

    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------




void __fastcall TFrmOption::btVisnRsltFile1Click(TObject *Sender)
{
    String Dir; 
    SelectDirectory("Select a directory", "", Dir);
    //ShowMessage(Dir);
    if(Dir != "") edDataSavePath -> Text = Dir ;


    //String Dir;
    //SelectDirectory(Dir, [sdAllowCreate], 0) ;
    //ShowMessage(Dir);


}
//---------------------------------------------------------------------------



