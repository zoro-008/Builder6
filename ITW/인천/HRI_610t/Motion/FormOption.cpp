//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

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
        cbLoadingStop   -> Checked   = OM.CmnOptn.bLoadingStop   ;
        cbIgnrDoor      -> Checked   = OM.CmnOptn.bIgnrDoor      ;
        cbSkipOriVisn   -> Checked   = OM.CmnOptn.bSkipOriVisn   ;
        cbSkipLaser     -> Checked   = OM.CmnOptn.bSkipLaser     ;
        cbSkipAlnVisn   -> Checked   = OM.CmnOptn.bSkipAlnVisn   ;
        cbFrstStrpOut   -> Checked   = OM.CmnOptn.bFrstStrpOut   ;

        rgMarkInsp      -> ItemIndex = OM.CmnOptn.iMarkInsp      ;
    }
    else {
        OM.CmnOptn.bLoadingStop    =  cbLoadingStop   -> Checked   ;
        OM.CmnOptn.bIgnrDoor       =  cbIgnrDoor      -> Checked   ;
        OM.CmnOptn.bSkipOriVisn    =  cbSkipOriVisn   -> Checked   ;
        OM.CmnOptn.bSkipLaser      =  cbSkipLaser     -> Checked   ;
        OM.CmnOptn.bSkipAlnVisn    =  cbSkipAlnVisn   -> Checked   ;
        OM.CmnOptn.bFrstStrpOut    =  cbFrstStrpOut   -> Checked   ;

        OM.CmnOptn.iMarkInsp       =  rgMarkInsp      -> ItemIndex ;

        
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------




