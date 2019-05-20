//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmVisionRlstSet.h"
#include "FrmOperation.h"
//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVisionRlstSet *FormVisionRlstSet;
//---------------------------------------------------------------------------
__fastcall TFormVisionRlstSet::TFormVisionRlstSet(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormVisionRlstSet::btNoClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormVisionRlstSet::UpdateCmmOptn(bool bTable)
{
    if(bTable) {
        edDis_Name1   -> Text = OM.CmnOptn.sCheckR1FailName    ;
        edDis_Name2   -> Text = OM.CmnOptn.sCheckR2FailName    ;
        edDis_Name3   -> Text = OM.CmnOptn.sCheckR3FailName    ;
        edDis_Name4   -> Text = OM.CmnOptn.sCheckR4FailName    ;
        edDis_Name5   -> Text = OM.CmnOptn.sCheckR5FailName    ;
        edDis_Name6   -> Text = OM.CmnOptn.sCheckR6FailName    ;
        edDis_Name7   -> Text = OM.CmnOptn.sCheckR7FailName    ;
        edDis_Name8   -> Text = OM.CmnOptn.sCheckR8FailName    ;
        edDis_Name9   -> Text = OM.CmnOptn.sCheckR9FailName    ;
        edDis_Name10  -> Text = OM.CmnOptn.sCheckR10FailName   ;
        edDis_Name11  -> Text = OM.CmnOptn.sCheckR11FailName   ;
        edDis_Name12  -> Text = OM.CmnOptn.sCheckR12FailName   ;
        edDis_Name13  -> Text = OM.CmnOptn.sCheckR13FailName   ;
        edDis_Name14  -> Text = OM.CmnOptn.sCheckR14FailName   ;

        edPriorty1    -> Text = OM.CmnOptn.iLevelRslt1         ;
        edPriorty2    -> Text = OM.CmnOptn.iLevelRslt2         ;
        edPriorty3    -> Text = OM.CmnOptn.iLevelRslt3         ;
        edPriorty4    -> Text = OM.CmnOptn.iLevelRslt4         ;
        edPriorty5    -> Text = OM.CmnOptn.iLevelRslt5         ;
        edPriorty6    -> Text = OM.CmnOptn.iLevelRslt6         ;
        edPriorty7    -> Text = OM.CmnOptn.iLevelRslt7         ;
        edPriorty8    -> Text = OM.CmnOptn.iLevelRslt8         ;
        edPriorty9    -> Text = OM.CmnOptn.iLevelRslt9         ;
        edPriorty10   -> Text = OM.CmnOptn.iLevelRslt10        ;
        edPriorty11   -> Text = OM.CmnOptn.iLevelRslt11        ;
        edPriorty12   -> Text = OM.CmnOptn.iLevelRslt12        ;
        edPriorty13   -> Text = OM.CmnOptn.iLevelRslt13        ;
        edPriorty14   -> Text = OM.CmnOptn.iLevelRslt14        ;

    }
    else {
        OM.CmnOptn.sCheckR1FailName   =  edDis_Name1   -> Text ;
        OM.CmnOptn.sCheckR2FailName   =  edDis_Name2   -> Text ;
        OM.CmnOptn.sCheckR3FailName   =  edDis_Name3   -> Text ;
        OM.CmnOptn.sCheckR4FailName   =  edDis_Name4   -> Text ;
        OM.CmnOptn.sCheckR5FailName   =  edDis_Name5   -> Text ;
        OM.CmnOptn.sCheckR6FailName   =  edDis_Name6   -> Text ;
        OM.CmnOptn.sCheckR7FailName   =  edDis_Name7   -> Text ;
        OM.CmnOptn.sCheckR8FailName   =  edDis_Name8   -> Text ;
        OM.CmnOptn.sCheckR9FailName   =  edDis_Name9   -> Text ;
        OM.CmnOptn.sCheckR10FailName  =  edDis_Name10  -> Text ;
        OM.CmnOptn.sCheckR11FailName  =  edDis_Name11  -> Text ;
        OM.CmnOptn.sCheckR12FailName  =  edDis_Name12  -> Text ;
        OM.CmnOptn.sCheckR13FailName  =  edDis_Name13  -> Text ;
        OM.CmnOptn.sCheckR14FailName  =  edDis_Name14  -> Text ;

        OM.CmnOptn.iLevelRslt1        =  edPriorty1    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt2        =  edPriorty2    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt3        =  edPriorty3    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt4        =  edPriorty4    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt5        =  edPriorty5    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt6        =  edPriorty6    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt7        =  edPriorty7    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt8        =  edPriorty8    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt9        =  edPriorty9    -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt10       =  edPriorty10   -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt11       =  edPriorty11   -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt12       =  edPriorty12   -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt13       =  edPriorty13   -> Text.ToInt() ;
        OM.CmnOptn.iLevelRslt14       =  edPriorty14   -> Text.ToInt() ;

    }
}
void __fastcall TFormVisionRlstSet::btYesClick(TObject *Sender)
{
    UpdateCmmOptn (toBuff);
    OM.SaveCmnOptn(      );
    FormOperation->LbResultChange();      
}
//---------------------------------------------------------------------------

void __fastcall TFormVisionRlstSet::FormShow(TObject *Sender)
{
    OM.LoadCmnOptn(      );
    UpdateCmmOptn (toTabl);        
}
//---------------------------------------------------------------------------

