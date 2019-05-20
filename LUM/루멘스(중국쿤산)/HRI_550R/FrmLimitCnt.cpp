//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmLimitCnt.h"
//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "DataMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLimitCnt *FormLimitCnt;
//---------------------------------------------------------------------------
__fastcall TFormLimitCnt::TFormLimitCnt(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLimitCnt::btNoClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormLimitCnt::UpdateCmmOptn(bool bTable)
{
    if(bTable) {
        edTotalFailCnt  -> Text = OM.CmnOptn.iCheckFailCnt    ;
        edR0FailCnt     -> Text = OM.CmnOptn.iCheckR1FailCnt  ;
        edR1FailCnt     -> Text = OM.CmnOptn.iCheckR2FailCnt  ;
        edR2FailCnt     -> Text = OM.CmnOptn.iCheckR3FailCnt  ;
        edR3FailCnt     -> Text = OM.CmnOptn.iCheckR4FailCnt  ;
        edR4FailCnt     -> Text = OM.CmnOptn.iCheckR5FailCnt  ;
        edR5FailCnt     -> Text = OM.CmnOptn.iCheckR6FailCnt  ;
        edR6FailCnt     -> Text = OM.CmnOptn.iCheckR7FailCnt  ;
        edR7FailCnt     -> Text = OM.CmnOptn.iCheckR8FailCnt  ;
        edR8FailCnt     -> Text = OM.CmnOptn.iCheckR9FailCnt  ;
        edR9FailCnt     -> Text = OM.CmnOptn.iCheckR10FailCnt ;
        edR10FailCnt    -> Text = OM.CmnOptn.iCheckR11FailCnt ;
        edR11FailCnt    -> Text = OM.CmnOptn.iCheckR12FailCnt ;
        edR12FailCnt    -> Text = OM.CmnOptn.iCheckR13FailCnt ;
        edR13FailCnt    -> Text = OM.CmnOptn.iCheckR14FailCnt ;

        lbR1  -> Caption = OM.CmnOptn.sCheckR1FailName ;
        lbR2  -> Caption = OM.CmnOptn.sCheckR2FailName ;
        lbR3  -> Caption = OM.CmnOptn.sCheckR3FailName ;
        lbR4  -> Caption = OM.CmnOptn.sCheckR4FailName ;
        lbR5  -> Caption = OM.CmnOptn.sCheckR5FailName ;
        lbR6  -> Caption = OM.CmnOptn.sCheckR6FailName ;
        lbR7  -> Caption = OM.CmnOptn.sCheckR7FailName ;
        lbR8  -> Caption = OM.CmnOptn.sCheckR8FailName ;
        lbR9  -> Caption = OM.CmnOptn.sCheckR9FailName ;
        lbR10 -> Caption = OM.CmnOptn.sCheckR10FailName ;
        lbR11 -> Caption = OM.CmnOptn.sCheckR11FailName ;
        lbR12 -> Caption = OM.CmnOptn.sCheckR12FailName ;
        lbR13 -> Caption = OM.CmnOptn.sCheckR13FailName ;
        lbR14 -> Caption = OM.CmnOptn.sCheckR14FailName ;

        cbR1FailSkip    -> Checked = OM.CmnOptn.bR1FailSkip  ;
        cbR2FailSkip    -> Checked = OM.CmnOptn.bR2FailSkip  ;
        cbR3FailSkip    -> Checked = OM.CmnOptn.bR3FailSkip  ;
        cbR4FailSkip    -> Checked = OM.CmnOptn.bR4FailSkip  ;
        cbR5FailSkip    -> Checked = OM.CmnOptn.bR5FailSkip  ;
        cbR6FailSkip    -> Checked = OM.CmnOptn.bR6FailSkip  ;
        cbR7FailSkip    -> Checked = OM.CmnOptn.bR7FailSkip  ;
        cbR8FailSkip    -> Checked = OM.CmnOptn.bR8FailSkip  ;
        cbR9FailSkip    -> Checked = OM.CmnOptn.bR9FailSkip  ;
        cbR10FailSkip   -> Checked = OM.CmnOptn.bR10FailSkip ;
        cbR11FailSkip   -> Checked = OM.CmnOptn.bR11FailSkip ;
        cbR12FailSkip   -> Checked = OM.CmnOptn.bR12FailSkip ;
        cbR13FailSkip   -> Checked = OM.CmnOptn.bR13FailSkip ;
        cbR14FailSkip   -> Checked = OM.CmnOptn.bR14FailSkip ;
    }
    else {
        OM.CmnOptn.iCheckFailCnt    = edTotalFailCnt  -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR1FailCnt  = edR0FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR2FailCnt  = edR1FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR3FailCnt  = edR2FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR4FailCnt  = edR3FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR5FailCnt  = edR4FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR6FailCnt  = edR5FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR7FailCnt  = edR6FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR8FailCnt  = edR7FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR9FailCnt  = edR8FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR10FailCnt = edR9FailCnt     -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR11FailCnt = edR10FailCnt    -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR12FailCnt = edR11FailCnt    -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR13FailCnt = edR12FailCnt    -> Text.ToInt()  ;
        OM.CmnOptn.iCheckR14FailCnt = edR13FailCnt    -> Text.ToInt()  ;

        OM.CmnOptn.bR1FailSkip      = cbR1FailSkip    -> Checked ;
        OM.CmnOptn.bR2FailSkip      = cbR2FailSkip    -> Checked ;
        OM.CmnOptn.bR3FailSkip      = cbR3FailSkip    -> Checked ;
        OM.CmnOptn.bR4FailSkip      = cbR4FailSkip    -> Checked ;
        OM.CmnOptn.bR5FailSkip      = cbR5FailSkip    -> Checked ;
        OM.CmnOptn.bR6FailSkip      = cbR6FailSkip    -> Checked ;
        OM.CmnOptn.bR7FailSkip      = cbR7FailSkip    -> Checked ;
        OM.CmnOptn.bR8FailSkip      = cbR8FailSkip    -> Checked ;
        OM.CmnOptn.bR9FailSkip      = cbR9FailSkip    -> Checked ;
        OM.CmnOptn.bR10FailSkip     = cbR10FailSkip   -> Checked ;
        OM.CmnOptn.bR11FailSkip     = cbR11FailSkip   -> Checked ;
        OM.CmnOptn.bR12FailSkip     = cbR12FailSkip   -> Checked ;
        OM.CmnOptn.bR13FailSkip     = cbR13FailSkip   -> Checked ;
        OM.CmnOptn.bR14FailSkip     = cbR14FailSkip   -> Checked ;

    }

}
void __fastcall TFormLimitCnt::FormShow(TObject *Sender)
{
    OM.LoadCmnOptn(      );
    UpdateCmmOptn (toTabl);
}
//---------------------------------------------------------------------------

void __fastcall TFormLimitCnt::btYesClick(TObject *Sender)
{
    UpdateCmmOptn (toBuff);
    OM.SaveCmnOptn(      );
}
//---------------------------------------------------------------------------


