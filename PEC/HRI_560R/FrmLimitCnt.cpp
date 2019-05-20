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
void __fastcall TFormLimitCnt::UpdateDevOptn(bool bTable)
{
    if(bTable) {
        edTotalFailCnt  -> Text = OM.DevOptn.iCheckFailCnt    ;
        edR0FailCnt     -> Text = OM.DevOptn.iCheckR1FailCnt  ;
        edR1FailCnt     -> Text = OM.DevOptn.iCheckR2FailCnt  ;
        edR2FailCnt     -> Text = OM.DevOptn.iCheckR3FailCnt  ;
        edR3FailCnt     -> Text = OM.DevOptn.iCheckR4FailCnt  ;
        edR4FailCnt     -> Text = OM.DevOptn.iCheckR5FailCnt  ;
        edR5FailCnt     -> Text = OM.DevOptn.iCheckR6FailCnt  ;
        edR6FailCnt     -> Text = OM.DevOptn.iCheckR7FailCnt  ;
        edR7FailCnt     -> Text = OM.DevOptn.iCheckR8FailCnt  ;
        edR8FailCnt     -> Text = OM.DevOptn.iCheckR9FailCnt  ;
        edR9FailCnt     -> Text = OM.DevOptn.iCheckR10FailCnt ;
        edR10FailCnt    -> Text = OM.DevOptn.iCheckR11FailCnt ;
        edR11FailCnt    -> Text = OM.DevOptn.iCheckR12FailCnt ;
        edR12FailCnt    -> Text = OM.DevOptn.iCheckR13FailCnt ;
        edR13FailCnt    -> Text = OM.DevOptn.iCheckR14FailCnt ;

        lbR1  -> Caption = OM.DevOptn.sCheckR1FailName ;
        lbR2  -> Caption = OM.DevOptn.sCheckR2FailName ;
        lbR3  -> Caption = OM.DevOptn.sCheckR3FailName ;
        lbR4  -> Caption = OM.DevOptn.sCheckR4FailName ;
        lbR5  -> Caption = OM.DevOptn.sCheckR5FailName ;
        lbR6  -> Caption = OM.DevOptn.sCheckR6FailName ;
        lbR7  -> Caption = OM.DevOptn.sCheckR7FailName ;
        lbR8  -> Caption = OM.DevOptn.sCheckR8FailName ;
        lbR9  -> Caption = OM.DevOptn.sCheckR9FailName ;
        lbR10 -> Caption = OM.DevOptn.sCheckR10FailName ;
        lbR11 -> Caption = OM.DevOptn.sCheckR11FailName ;
        lbR12 -> Caption = OM.DevOptn.sCheckR12FailName ;
        lbR13 -> Caption = OM.DevOptn.sCheckR13FailName ;
        lbR14 -> Caption = OM.DevOptn.sCheckR14FailName ;
    }
    else {
        OM.DevOptn.iCheckFailCnt     = edTotalFailCnt  -> Text.ToInt()  ;
        OM.DevOptn.iCheckR1FailCnt   = edR0FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR2FailCnt   = edR1FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR3FailCnt   = edR2FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR4FailCnt   = edR3FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR5FailCnt   = edR4FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR6FailCnt   = edR5FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR7FailCnt   = edR6FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR8FailCnt   = edR7FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR9FailCnt   = edR8FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR10FailCnt  = edR9FailCnt     -> Text.ToInt()  ;
        OM.DevOptn.iCheckR11FailCnt  = edR10FailCnt    -> Text.ToInt()  ;
        OM.DevOptn.iCheckR12FailCnt  = edR11FailCnt    -> Text.ToInt()  ;
        OM.DevOptn.iCheckR13FailCnt  = edR12FailCnt    -> Text.ToInt()  ;
        OM.DevOptn.iCheckR14FailCnt  = edR13FailCnt    -> Text.ToInt()  ;

        OM.DevOptn.sCheckR1FailName  = lbR1  -> Caption ;
        OM.DevOptn.sCheckR2FailName  = lbR2  -> Caption ;
        OM.DevOptn.sCheckR3FailName  = lbR3  -> Caption ;
        OM.DevOptn.sCheckR4FailName  = lbR4  -> Caption ;
        OM.DevOptn.sCheckR5FailName  = lbR5  -> Caption ;
        OM.DevOptn.sCheckR6FailName  = lbR6  -> Caption ;
        OM.DevOptn.sCheckR7FailName  = lbR7  -> Caption ;
        OM.DevOptn.sCheckR8FailName  = lbR8  -> Caption ;
        OM.DevOptn.sCheckR9FailName  = lbR9  -> Caption ;
        OM.DevOptn.sCheckR10FailName = lbR10 -> Caption ;
        OM.DevOptn.sCheckR11FailName = lbR11 -> Caption ;
        OM.DevOptn.sCheckR12FailName = lbR12 -> Caption ;
        OM.DevOptn.sCheckR13FailName = lbR13 -> Caption ;
        OM.DevOptn.sCheckR14FailName = lbR14 -> Caption ;
    }

}
void __fastcall TFormLimitCnt::FormShow(TObject *Sender)
{
    OM.LoadDevOptn(OM.m_sCrntDev);
    UpdateDevOptn (toTabl       );
}
//---------------------------------------------------------------------------

void __fastcall TFormLimitCnt::btYesClick(TObject *Sender)
{
    UpdateDevOptn (toBuff       );
    OM.SaveDevOptn(OM.m_sCrntDev);
}
//---------------------------------------------------------------------------



