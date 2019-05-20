//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
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
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

        //Display Setting.

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        edBin1FullMaxCnt    -> Text     = OM.CmnOptn.iBin1FullMaxCnt    ;
        edBin2FullMaxCnt    -> Text     = OM.CmnOptn.iBin2FullMaxCnt    ;

        cbLoadingStop       -> Checked  = OM.CmnOptn.bLoadingStop       ;
        cbFRunSkip          -> Checked  = OM.CmnOptn.bFRunSkip          ;
        cbRRunSkip          -> Checked  = OM.CmnOptn.bRRunSkip          ;
        edFirstAirBlowTime  -> Text     = OM.CmnOptn.dFirstAirBlowTime  ;
        edSecondAirBlowTime -> Text     = OM.CmnOptn.dSecondAirBlowTime ;
        edLdrPickDely       -> Text     = OM.CmnOptn.dLdrPickDely       ;
        edUldMskDetSnrDely  -> Text     = OM.CmnOptn.dUldMskDetSnrDely  ;
        edPackOpenVccDely   -> Text     = OM.CmnOptn.dPackOpenVccDely   ;
        edPackEjctDely      -> Text     = OM.CmnOptn.dPackEjctDely      ;
        edPickVccDely       -> Text     = OM.CmnOptn.dPickVccDely       ;
        edPickEjctDely      -> Text     = OM.CmnOptn.dPickEjctDely      ;
        edPickBwBfrDely     -> Text     = OM.CmnOptn.dPickBwBfrDely     ;

        }

    else {
        OM.CmnOptn.iBin1FullMaxCnt    = StrToFloatDef(edBin1FullMaxCnt    -> Text, 150) ;
        OM.CmnOptn.iBin2FullMaxCnt    = StrToFloatDef(edBin2FullMaxCnt    -> Text, 150) ;

        OM.CmnOptn.bLoadingStop       = cbLoadingStop                     -> Checked    ;
        OM.CmnOptn.bFRunSkip          = cbFRunSkip                        -> Checked    ;
        OM.CmnOptn.bRRunSkip          = cbRRunSkip                        -> Checked    ;
        OM.CmnOptn.dFirstAirBlowTime  = StrToFloatDef(edFirstAirBlowTime  -> Text, 200) ;
        OM.CmnOptn.dSecondAirBlowTime = StrToFloatDef(edSecondAirBlowTime -> Text, 200) ;
        OM.CmnOptn.dLdrPickDely       = StrToFloatDef(edLdrPickDely       -> Text, 500) ;
        OM.CmnOptn.dUldMskDetSnrDely  = StrToFloatDef(edUldMskDetSnrDely  -> Text, 500) ;
        OM.CmnOptn.dPackOpenVccDely   = StrToFloatDef(edPackOpenVccDely   -> Text, 500) ;
        OM.CmnOptn.dPackEjctDely      = StrToFloatDef(edPackEjctDely      -> Text, 300) ;
        OM.CmnOptn.dPickVccDely       = StrToFloatDef(edPickVccDely       -> Text, 500) ;
        OM.CmnOptn.dPickEjctDely      = StrToFloatDef(edPickEjctDely      -> Text, 300) ;
        OM.CmnOptn.dPickBwBfrDely     = StrToFloatDef(edPickBwBfrDely     -> Text, 1000);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{

//    cbIgnrDoor -> Visible = FM_GetLevel() == lvMaster ;
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------











