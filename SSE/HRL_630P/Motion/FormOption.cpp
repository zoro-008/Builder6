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
        cbIgnrDoor       -> Checked   = OM.CmnOptn.bIgnrDoorOpen   ;
        cbSkipLaserWrk   -> Checked   = OM.CmnOptn.bSkipLaserWrk   ;

        rgUseLsrPos      -> ItemIndex = OM.CmnOptn.iUseLsrPos      ;

        edVsnInspCnt     -> Text      = OM.CmnOptn.iVsnFailCnt     ;
        edAirBlowTm      -> Text      = OM.CmnOptn.iAirBlowTm      ;
        edAlignTm        -> Text      = OM.CmnOptn.iAlignTm        ;

    }
    else {
        OM.CmnOptn.bIgnrDoorOpen   = cbIgnrDoor     -> Checked   ;
        OM.CmnOptn.bSkipLaserWrk   = cbSkipLaserWrk -> Checked   ;

        OM.CmnOptn.iUseLsrPos      = rgUseLsrPos    -> ItemIndex ;

        OM.CmnOptn.iVsnFailCnt     = StrToIntDef(edVsnInspCnt -> Text,5  );
        OM.CmnOptn.iAirBlowTm      = StrToIntDef(edAirBlowTm  -> Text,500);
        OM.CmnOptn.iAlignTm        = StrToIntDef(edAlignTm    -> Text,100);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------






