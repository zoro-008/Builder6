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
        edCheckFailCnt   -> Text = OM.CmnOptn.iCheckFailCnt  ;

        edCheckFailCnt0  -> Text = OM.CmnOptn.iCheckFailCnt0 ;
        edCheckFailCnt1  -> Text = OM.CmnOptn.iCheckFailCnt1 ;
        edCheckFailCnt2  -> Text = OM.CmnOptn.iCheckFailCnt2 ;
        edCheckFailCnt3  -> Text = OM.CmnOptn.iCheckFailCnt3 ;
        edCheckFailCnt4  -> Text = OM.CmnOptn.iCheckFailCnt4 ;
        edCheckFailCnt5  -> Text = OM.CmnOptn.iCheckFailCnt5 ;
        edCheckFailCnt6  -> Text = OM.CmnOptn.iCheckFailCnt6 ;
        edCheckFailCnt7  -> Text = OM.CmnOptn.iCheckFailCnt7 ;
        edCheckFailCnt8  -> Text = OM.CmnOptn.iCheckFailCnt8 ;
        edCheckFailCnt9  -> Text = OM.CmnOptn.iCheckFailCnt9 ;
        edCheckFailCntA  -> Text = OM.CmnOptn.iCheckFailCntA ;
        edCheckFailCntB  -> Text = OM.CmnOptn.iCheckFailCntB ;
        edCheckFailCntC  -> Text = OM.CmnOptn.iCheckFailCntC ;
        edCheckFailCntD  -> Text = OM.CmnOptn.iCheckFailCntD ;
        edCheckFailCntE  -> Text = OM.CmnOptn.iCheckFailCntE ;
        edCheckFailCntF  -> Text = OM.CmnOptn.iCheckFailCntF ;
        edCheckFailCntG  -> Text = OM.CmnOptn.iCheckFailCntG ;
        edCheckFailCntH  -> Text = OM.CmnOptn.iCheckFailCntH ;
        edCheckFailCntI  -> Text = OM.CmnOptn.iCheckFailCntI ;
        edCheckFailCntJ  -> Text = OM.CmnOptn.iCheckFailCntJ ;
        edCheckFailCntK  -> Text = OM.CmnOptn.iCheckFailCntK ;
        edCheckFailCntL  -> Text = OM.CmnOptn.iCheckFailCntL ;


        lbRsltName0 -> Caption = OM.CmnOptn.sRsltName0 ;
        lbRsltName1 -> Caption = OM.CmnOptn.sRsltName1 ;
        lbRsltName2 -> Caption = OM.CmnOptn.sRsltName2 ;
        lbRsltName3 -> Caption = OM.CmnOptn.sRsltName3 ;
        lbRsltName4 -> Caption = OM.CmnOptn.sRsltName4 ;
        lbRsltName5 -> Caption = OM.CmnOptn.sRsltName5 ;
        lbRsltName6 -> Caption = OM.CmnOptn.sRsltName6 ;
        lbRsltName7 -> Caption = OM.CmnOptn.sRsltName7 ;
        lbRsltName8 -> Caption = OM.CmnOptn.sRsltName8 ;
        lbRsltName9 -> Caption = OM.CmnOptn.sRsltName9 ;
        lbRsltNameA -> Caption = OM.CmnOptn.sRsltNameA ;
        lbRsltNameB -> Caption = OM.CmnOptn.sRsltNameB ;
        lbRsltNameC -> Caption = OM.CmnOptn.sRsltNameC ;
        lbRsltNameD -> Caption = OM.CmnOptn.sRsltNameD ;
        lbRsltNameE -> Caption = OM.CmnOptn.sRsltNameE ;
        lbRsltNameF -> Caption = OM.CmnOptn.sRsltNameF ;
        lbRsltNameG -> Caption = OM.CmnOptn.sRsltNameG ;
        lbRsltNameH -> Caption = OM.CmnOptn.sRsltNameH ;
        lbRsltNameI -> Caption = OM.CmnOptn.sRsltNameI ;
        lbRsltNameJ -> Caption = OM.CmnOptn.sRsltNameJ ;
        lbRsltNameK -> Caption = OM.CmnOptn.sRsltNameK ;
        lbRsltNameL -> Caption = OM.CmnOptn.sRsltNameL ;

        pnRsltColor0 -> Color      = (TColor)OM.CmnOptn.iRsltColor0 ;
        pnRsltColor1 -> Color      = (TColor)OM.CmnOptn.iRsltColor1 ;
        pnRsltColor2 -> Color      = (TColor)OM.CmnOptn.iRsltColor2 ;
        pnRsltColor3 -> Color      = (TColor)OM.CmnOptn.iRsltColor3 ;
        pnRsltColor4 -> Color      = (TColor)OM.CmnOptn.iRsltColor4 ;
        pnRsltColor5 -> Color      = (TColor)OM.CmnOptn.iRsltColor5 ;
        pnRsltColor6 -> Color      = (TColor)OM.CmnOptn.iRsltColor6 ;
        pnRsltColor7 -> Color      = (TColor)OM.CmnOptn.iRsltColor7 ;
        pnRsltColor8 -> Color      = (TColor)OM.CmnOptn.iRsltColor8 ;
        pnRsltColor9 -> Color      = (TColor)OM.CmnOptn.iRsltColor9 ;
        pnRsltColorA -> Color      = (TColor)OM.CmnOptn.iRsltColorA ;
        pnRsltColorB -> Color      = (TColor)OM.CmnOptn.iRsltColorB ;
        pnRsltColorC -> Color      = (TColor)OM.CmnOptn.iRsltColorC ;
        pnRsltColorD -> Color      = (TColor)OM.CmnOptn.iRsltColorD ;
        pnRsltColorE -> Color      = (TColor)OM.CmnOptn.iRsltColorE ;
        pnRsltColorF -> Color      = (TColor)OM.CmnOptn.iRsltColorF ;
        pnRsltColorG -> Color      = (TColor)OM.CmnOptn.iRsltColorG ;
        pnRsltColorH -> Color      = (TColor)OM.CmnOptn.iRsltColorH ;
        pnRsltColorI -> Color      = (TColor)OM.CmnOptn.iRsltColorI ;
        pnRsltColorJ -> Color      = (TColor)OM.CmnOptn.iRsltColorJ ;
        pnRsltColorK -> Color      = (TColor)OM.CmnOptn.iRsltColorK ;
        pnRsltColorL -> Color      = (TColor)OM.CmnOptn.iRsltColorL ;
    }
    else {
        OM.CmnOptn.iCheckFailCnt   = edCheckFailCnt   -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt0  = edCheckFailCnt0  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt1  = edCheckFailCnt1  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt2  = edCheckFailCnt2  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt3  = edCheckFailCnt3  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt4  = edCheckFailCnt4  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt5  = edCheckFailCnt5  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt6  = edCheckFailCnt6  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt7  = edCheckFailCnt7  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt8  = edCheckFailCnt8  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCnt9  = edCheckFailCnt9  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntA  = edCheckFailCntA  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntB  = edCheckFailCntB  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntC  = edCheckFailCntC  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntD  = edCheckFailCntD  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntE  = edCheckFailCntE  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntF  = edCheckFailCntF  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntG  = edCheckFailCntG  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntH  = edCheckFailCntH  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntI  = edCheckFailCntI  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntJ  = edCheckFailCntJ  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntK  = edCheckFailCntK  -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iCheckFailCntL  = edCheckFailCntL  -> Text.ToIntDef(0)  ;
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






