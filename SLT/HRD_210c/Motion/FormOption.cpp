//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "OptionMan.h"
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
        FM_MsgTime("Warning" , "Can't Save during Autorunning!" , 2000);
        return;
    }
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

    if(!OM.CmnOptn.bUseMapFile) {
        DM.ARAY[riSTG].SetMaxColRow(1,1);

    }


//    OM.SaveEqpOptn();
//    UserMsg2("Confirm" , "저장 되었습니다.");
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbbDoorSkip             -> Checked = OM.CmnOptn.bDoorSkip              ;
        cbLoadingStop           -> Checked = OM.CmnOptn.bLoadingStop           ;
        cbEpoxyStop             -> Checked = OM.CmnOptn.bEpoxyStop             ; 
        cbIgnrOriVs             -> Checked = OM.CmnOptn.bIgnrOriVs             ;
        cbbIgnrRalAtVs          -> Checked = OM.CmnOptn.bIgnrRalAtVs           ;
        cbbIgnrVcSsr            -> Checked = OM.CmnOptn.bIgnrVcSsr             ;
        cbbUseStgTheta          -> Checked = OM.CmnOptn.bUseStgTheta           ;
        cbbIgnrRalBfVsEpoxyFail -> Checked = OM.CmnOptn.bIgnrRalBfVsEpoxyFail  ;
        cbUseMapFile            -> Checked = OM.CmnOptn.bUseMapFile            ;
        cbInkDieWork            -> Checked = OM.CmnOptn.bInkDieWork            ;
        cbUseBtAlign            -> Checked = OM.CmnOptn.bUseBtAlign            ;
        cbIgnrHedOverload       -> Checked = OM.CmnOptn.bIgnrHedOverload       ;
        cbUseHeadTWfr           -> Checked = OM.CmnOptn.bUseHeadTWfr           ;

        cbUseHeadTRal           -> Checked = OM.CmnOptn.bUseHeadTRal           ;

        edStageTLmtErr          -> Text    = OM.CmnOptn.dStageTLmtErr          ;
        edHedTLmtErr            -> Text    = OM.CmnOptn.dHedTLmtErr            ;
        edEjctrDnAfterDelay     -> Text    = OM.CmnOptn.iEjctrDnAfterDelay     ;
        edEjctrUpAfterDelay     -> Text    = OM.CmnOptn.iEjctrUpAfterDelay     ;
        cbWfrVisnRetest         -> Checked = OM.CmnOptn.bWfrVisnRetest         ;
        edLotEndChipCnt         -> Text    = OM.CmnOptn.iLotEndChipCnt         ;
        cbUseVcCoil             -> Checked = OM.CmnOptn.bUseVcCoil             ;
    }
    else {
        OM.CmnOptn.bDoorSkip              = cbbDoorSkip             -> Checked ;
        OM.CmnOptn.bLoadingStop           = cbLoadingStop           -> Checked ;
        OM.CmnOptn.bEpoxyStop             = cbEpoxyStop             -> Checked ;
        OM.CmnOptn.bIgnrOriVs             = cbIgnrOriVs             -> Checked ;
        OM.CmnOptn.bIgnrRalAtVs           = cbbIgnrRalAtVs          -> Checked ;
        OM.CmnOptn.bIgnrVcSsr             = cbbIgnrVcSsr            -> Checked ;
        OM.CmnOptn.bUseStgTheta           = cbbUseStgTheta          -> Checked ;
        OM.CmnOptn.bIgnrRalBfVsEpoxyFail  = cbbIgnrRalBfVsEpoxyFail -> Checked ;
        OM.CmnOptn.bUseMapFile            = cbUseMapFile            -> Checked ;
        OM.CmnOptn.bInkDieWork            = cbInkDieWork            -> Checked ;
        OM.CmnOptn.bUseBtAlign            = cbUseBtAlign            -> Checked ;
        OM.CmnOptn.bIgnrHedOverload       = cbIgnrHedOverload       -> Checked ;

        OM.CmnOptn.bUseHeadTWfr           = cbUseHeadTWfr           -> Checked ;

        OM.CmnOptn.bUseHeadTRal           = cbUseHeadTRal           -> Checked ;

        OM.CmnOptn.dStageTLmtErr          = edStageTLmtErr          -> Text.ToDouble() ;
        OM.CmnOptn.dHedTLmtErr            = edHedTLmtErr            -> Text.ToDouble() ;
        OM.CmnOptn.iEjctrDnAfterDelay     = edEjctrDnAfterDelay     -> Text.ToIntDef(50) ;
        OM.CmnOptn.iEjctrUpAfterDelay     = edEjctrUpAfterDelay     -> Text.ToIntDef(50) ;

        OM.CmnOptn.bWfrVisnRetest         = cbWfrVisnRetest         -> Checked  ;

        OM.CmnOptn.iLotEndChipCnt         = edLotEndChipCnt         -> Text.ToDouble() ;
        OM.CmnOptn.bUseVcCoil             = cbUseVcCoil             -> Checked ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::cbLoadingStopClick(TObject *Sender)
{
    //UpdateComOptn(toBuff);
}
//---------------------------------------------------------------------------



