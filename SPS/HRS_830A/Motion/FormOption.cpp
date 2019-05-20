//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include <FileCtrl.hpp>

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
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
        return;
    }
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbLoadingStop     -> Checked   = OM.CmnOptn.bLoadingStop       ;
        cbIgnrDoor        -> Checked   = OM.CmnOptn.bIgnrDoor          ;
        cbIgnrDispsr      -> Checked   = OM.CmnOptn.bIgnrDisps         ;
        cbEmptyStrpCnt    -> Checked   = OM.CmnOptn.bEmptyStrpCnt      ;
        cbSkipDspsSnsr    -> Checked   = OM.CmnOptn.bIgnrDsprSnsr      ;
        cbIngrDsprExpr    -> Checked   = OM.CmnOptn.bIgnrDsprExpr      ;

        edEmptyStrpCnt    -> Text      = OM.CmnOptn.iEmptyStrpCnt      ;
        edExpiryDay       -> Text      = OM.CmnOptn.iExpiryDay         ;
        edDisprCleanCnt   -> Text      = OM.CmnOptn.iDisprCleanCnt     ;

        edServerPath      -> Text      = OM.CmnOptn.sServerPath        ;
        edStopToShotTime  -> Text      = OM.CmnOptn.iStopToShotTime    ;
    }
    else {
        OM.CmnOptn.bLoadingStop    =  cbLoadingStop   -> Checked   ;
        OM.CmnOptn.bIgnrDoor       =  cbIgnrDoor      -> Checked   ;
        OM.CmnOptn.bIgnrDisps      =  cbIgnrDispsr    -> Checked   ;
        OM.CmnOptn.bEmptyStrpCnt   =  cbEmptyStrpCnt  -> Checked   ;
        OM.CmnOptn.bIgnrDsprSnsr   =  cbSkipDspsSnsr  -> Checked   ;
        OM.CmnOptn.bIgnrDsprExpr   =  cbIngrDsprExpr  -> Checked   ;


        OM.CmnOptn.iEmptyStrpCnt   =  StrToIntDef(edEmptyStrpCnt   -> Text , 5);
        OM.CmnOptn.iExpiryDay      =  StrToIntDef(edExpiryDay      -> Text , 5);
        OM.CmnOptn.iDisprCleanCnt  =  StrToIntDef(edDisprCleanCnt  -> Text , 1);

        OM.CmnOptn.sServerPath     =  edServerPath     -> Text    ;
        OM.CmnOptn.iStopToShotTime =  StrToIntDef(edStopToShotTime -> Text , 5);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
    SetDay.Val = OM.CmnOptn.LQSupplyDay.Val;
    edSettingDay -> Text = SetDay.FormatString("yyyy-mm-dd");

    edEmptyStrpCnt -> BorderStyle = 20;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::btLiquidResetClick(TObject *Sender)
{
    OM.CmnOptn.LQSupplyDay.Val = Now();
    SetDay.Val = OM.CmnOptn.LQSupplyDay.Val;
    edSettingDay -> Text = SetDay.FormatString("yyyy-mm-dd");
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------


void __fastcall TFrmOption::btServerFolderClick(TObject *Sender)
{
    String Dir;
    SelectDirectory("Select a directory", "", Dir);
    //ShowMessage(Dir);
    edServerPath -> Text = Dir ;


}
//---------------------------------------------------------------------------

