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
        cbDispnrSkip   -> Checked   = OM.CmnOptn.bDisprSkip   ;
    }
    else {
        OM.CmnOptn.bDisprSkip    =  cbDispnrSkip   -> Checked   ;
    }


}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------




