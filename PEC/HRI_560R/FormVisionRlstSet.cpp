//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVisionRlstSet.h"
#include "FormOperation.h"
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
void __fastcall TFormVisionRlstSet::UpdateDevOptn(bool bTable)
{
    if(bTable) {
        edDis_Name1   -> Text = OM.DevOptn.sCheckR1FailName    ;
        edDis_Name2   -> Text = OM.DevOptn.sCheckR2FailName    ;
        edDis_Name3   -> Text = OM.DevOptn.sCheckR3FailName    ;
        edDis_Name4   -> Text = OM.DevOptn.sCheckR4FailName    ;
        edDis_Name5   -> Text = OM.DevOptn.sCheckR5FailName    ;
        edDis_Name6   -> Text = OM.DevOptn.sCheckR6FailName    ;
        edDis_Name7   -> Text = OM.DevOptn.sCheckR7FailName    ;
        edDis_Name8   -> Text = OM.DevOptn.sCheckR8FailName    ;
        edDis_Name9   -> Text = OM.DevOptn.sCheckR9FailName    ;
        edDis_Name10  -> Text = OM.DevOptn.sCheckR10FailName   ;
        edDis_Name11  -> Text = OM.DevOptn.sCheckR11FailName   ;
        edDis_Name12  -> Text = OM.DevOptn.sCheckR12FailName   ;
        edDis_Name13  -> Text = OM.DevOptn.sCheckR13FailName   ;
        edDis_Name14  -> Text = OM.DevOptn.sCheckR14FailName   ;

        edPriorty1    -> Text = OM.DevOptn.iLevelRslt1         ;
        edPriorty2    -> Text = OM.DevOptn.iLevelRslt2         ;
        edPriorty3    -> Text = OM.DevOptn.iLevelRslt3         ;
        edPriorty4    -> Text = OM.DevOptn.iLevelRslt4         ;
        edPriorty5    -> Text = OM.DevOptn.iLevelRslt5         ;
        edPriorty6    -> Text = OM.DevOptn.iLevelRslt6         ;
        edPriorty7    -> Text = OM.DevOptn.iLevelRslt7         ;
        edPriorty8    -> Text = OM.DevOptn.iLevelRslt8         ;
        edPriorty9    -> Text = OM.DevOptn.iLevelRslt9         ;
        edPriorty10   -> Text = OM.DevOptn.iLevelRslt10        ;
        edPriorty11   -> Text = OM.DevOptn.iLevelRslt11        ;
        edPriorty12   -> Text = OM.DevOptn.iLevelRslt12        ;
        edPriorty13   -> Text = OM.DevOptn.iLevelRslt13        ;
        edPriorty14   -> Text = OM.DevOptn.iLevelRslt14        ;

    }
    else {
        OM.DevOptn.sCheckR1FailName   =  edDis_Name1   -> Text ;
        OM.DevOptn.sCheckR2FailName   =  edDis_Name2   -> Text ;
        OM.DevOptn.sCheckR3FailName   =  edDis_Name3   -> Text ;
        OM.DevOptn.sCheckR4FailName   =  edDis_Name4   -> Text ;
        OM.DevOptn.sCheckR5FailName   =  edDis_Name5   -> Text ;
        OM.DevOptn.sCheckR6FailName   =  edDis_Name6   -> Text ;
        OM.DevOptn.sCheckR7FailName   =  edDis_Name7   -> Text ;
        OM.DevOptn.sCheckR8FailName   =  edDis_Name8   -> Text ;
        OM.DevOptn.sCheckR9FailName   =  edDis_Name9   -> Text ;
        OM.DevOptn.sCheckR10FailName  =  edDis_Name10  -> Text ;
        OM.DevOptn.sCheckR11FailName  =  edDis_Name11  -> Text ;
        OM.DevOptn.sCheckR12FailName  =  edDis_Name12  -> Text ;
        OM.DevOptn.sCheckR13FailName  =  edDis_Name13  -> Text ;
        OM.DevOptn.sCheckR14FailName  =  edDis_Name14  -> Text ;

        OM.DevOptn.iLevelRslt1        =  edPriorty1    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt2        =  edPriorty2    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt3        =  edPriorty3    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt4        =  edPriorty4    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt5        =  edPriorty5    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt6        =  edPriorty6    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt7        =  edPriorty7    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt8        =  edPriorty8    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt9        =  edPriorty9    -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt10       =  edPriorty10   -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt11       =  edPriorty11   -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt12       =  edPriorty12   -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt13       =  edPriorty13   -> Text.ToInt() ;
        OM.DevOptn.iLevelRslt14       =  edPriorty14   -> Text.ToInt() ;

    }
}
void __fastcall TFormVisionRlstSet::btYesClick(TObject *Sender)
{
    UpdateDevOptn (toBuff       );
    OM.SaveDevOptn(OM.m_sCrntDev);
    FrmOperation->LbResultChange();
}
//---------------------------------------------------------------------------

void __fastcall TFormVisionRlstSet::FormShow(TObject *Sender)
{
    OM.LoadDevOptn(OM.m_sCrntDev);
    UpdateDevOptn (toTabl       );
}
//---------------------------------------------------------------------------


