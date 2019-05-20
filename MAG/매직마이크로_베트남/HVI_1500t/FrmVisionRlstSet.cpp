//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmVisionRlstSet.h"
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
    m_bChanged = true ;
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
        ediRsltLevel0 -> Text = OM.CmnOptn.iRsltLevel0 ;
        ediRsltLevel1 -> Text = OM.CmnOptn.iRsltLevel1 ;
        ediRsltLevel2 -> Text = OM.CmnOptn.iRsltLevel2 ;
        ediRsltLevel3 -> Text = OM.CmnOptn.iRsltLevel3 ;
        ediRsltLevel4 -> Text = OM.CmnOptn.iRsltLevel4 ;
        ediRsltLevel5 -> Text = OM.CmnOptn.iRsltLevel5 ;
        ediRsltLevel6 -> Text = OM.CmnOptn.iRsltLevel6 ;
        ediRsltLevel7 -> Text = OM.CmnOptn.iRsltLevel7 ;
        ediRsltLevel8 -> Text = OM.CmnOptn.iRsltLevel8 ;
        ediRsltLevel9 -> Text = OM.CmnOptn.iRsltLevel9 ;
        ediRsltLevelA -> Text = OM.CmnOptn.iRsltLevelA ;
        ediRsltLevelB -> Text = OM.CmnOptn.iRsltLevelB ;
        ediRsltLevelC -> Text = OM.CmnOptn.iRsltLevelC ;
        ediRsltLevelD -> Text = OM.CmnOptn.iRsltLevelD ;
        ediRsltLevelE -> Text = OM.CmnOptn.iRsltLevelE ;
        ediRsltLevelF -> Text = OM.CmnOptn.iRsltLevelF ;
        ediRsltLevelG -> Text = OM.CmnOptn.iRsltLevelG ;
        ediRsltLevelH -> Text = OM.CmnOptn.iRsltLevelH ;
        ediRsltLevelI -> Text = OM.CmnOptn.iRsltLevelI ;
        ediRsltLevelJ -> Text = OM.CmnOptn.iRsltLevelJ ;
        ediRsltLevelK -> Text = OM.CmnOptn.iRsltLevelK ;
        ediRsltLevelL -> Text = OM.CmnOptn.iRsltLevelL ;

        edsRsltName0  -> Text = OM.CmnOptn.sRsltName0  ;
        edsRsltName1  -> Text = OM.CmnOptn.sRsltName1  ;
        edsRsltName2  -> Text = OM.CmnOptn.sRsltName2  ;
        edsRsltName3  -> Text = OM.CmnOptn.sRsltName3  ;
        edsRsltName4  -> Text = OM.CmnOptn.sRsltName4  ;
        edsRsltName5  -> Text = OM.CmnOptn.sRsltName5  ;
        edsRsltName6  -> Text = OM.CmnOptn.sRsltName6  ;
        edsRsltName7  -> Text = OM.CmnOptn.sRsltName7  ;
        edsRsltName8  -> Text = OM.CmnOptn.sRsltName8  ;
        edsRsltName9  -> Text = OM.CmnOptn.sRsltName9  ;
        edsRsltNameA  -> Text = OM.CmnOptn.sRsltNameA  ;
        edsRsltNameB  -> Text = OM.CmnOptn.sRsltNameB  ;
        edsRsltNameC  -> Text = OM.CmnOptn.sRsltNameC  ;
        edsRsltNameD  -> Text = OM.CmnOptn.sRsltNameD  ;
        edsRsltNameE  -> Text = OM.CmnOptn.sRsltNameE  ;
        edsRsltNameF  -> Text = OM.CmnOptn.sRsltNameF  ;
        edsRsltNameG  -> Text = OM.CmnOptn.sRsltNameG  ;
        edsRsltNameH  -> Text = OM.CmnOptn.sRsltNameH  ;
        edsRsltNameI  -> Text = OM.CmnOptn.sRsltNameI  ;
        edsRsltNameJ  -> Text = OM.CmnOptn.sRsltNameJ  ;
        edsRsltNameK  -> Text = OM.CmnOptn.sRsltNameK  ;
        edsRsltNameL  -> Text = OM.CmnOptn.sRsltNameL  ;

        pnRsltColor0 -> Color = (TColor)OM.CmnOptn.iRsltColor0  ;
        pnRsltColor1 -> Color = (TColor)OM.CmnOptn.iRsltColor1  ;
        pnRsltColor2 -> Color = (TColor)OM.CmnOptn.iRsltColor2  ;
        pnRsltColor3 -> Color = (TColor)OM.CmnOptn.iRsltColor3  ;
        pnRsltColor4 -> Color = (TColor)OM.CmnOptn.iRsltColor4  ;
        pnRsltColor5 -> Color = (TColor)OM.CmnOptn.iRsltColor5  ;
        pnRsltColor6 -> Color = (TColor)OM.CmnOptn.iRsltColor6  ;
        pnRsltColor7 -> Color = (TColor)OM.CmnOptn.iRsltColor7  ;
        pnRsltColor8 -> Color = (TColor)OM.CmnOptn.iRsltColor8  ;
        pnRsltColor9 -> Color = (TColor)OM.CmnOptn.iRsltColor9  ;
        pnRsltColorA -> Color = (TColor)OM.CmnOptn.iRsltColorA  ;
        pnRsltColorB -> Color = (TColor)OM.CmnOptn.iRsltColorB  ;
        pnRsltColorC -> Color = (TColor)OM.CmnOptn.iRsltColorC  ;
        pnRsltColorD -> Color = (TColor)OM.CmnOptn.iRsltColorD  ;
        pnRsltColorE -> Color = (TColor)OM.CmnOptn.iRsltColorE  ;
        pnRsltColorF -> Color = (TColor)OM.CmnOptn.iRsltColorF  ;
        pnRsltColorG -> Color = (TColor)OM.CmnOptn.iRsltColorG  ;
        pnRsltColorH -> Color = (TColor)OM.CmnOptn.iRsltColorH  ;
        pnRsltColorI -> Color = (TColor)OM.CmnOptn.iRsltColorI  ;
        pnRsltColorJ -> Color = (TColor)OM.CmnOptn.iRsltColorJ  ;
        pnRsltColorK -> Color = (TColor)OM.CmnOptn.iRsltColorK  ;
        pnRsltColorL -> Color = (TColor)OM.CmnOptn.iRsltColorL  ;

        pnRsltColor22-> Color = (TColor)OM.CmnOptn.iRsltColor22 ;
        pnRsltColor24-> Color = (TColor)OM.CmnOptn.iRsltColor24 ;
        pnRsltColor25-> Color = (TColor)OM.CmnOptn.iRsltColor25 ;
        pnRsltColor26-> Color = (TColor)OM.CmnOptn.iRsltColor26 ;
        pnRsltColor27-> Color = (TColor)OM.CmnOptn.iRsltColor27 ;


        edSort0  -> Text = OM.CmnOptn.iSort0  ;
        edSort1  -> Text = OM.CmnOptn.iSort1  ;
        edSort2  -> Text = OM.CmnOptn.iSort2  ;
        edSort3  -> Text = OM.CmnOptn.iSort3  ;
        edSort4  -> Text = OM.CmnOptn.iSort4  ;
        edSort5  -> Text = OM.CmnOptn.iSort5  ;
        edSort6  -> Text = OM.CmnOptn.iSort6  ;
        edSort7  -> Text = OM.CmnOptn.iSort7  ;
        edSort8  -> Text = OM.CmnOptn.iSort8  ;
        edSort9  -> Text = OM.CmnOptn.iSort9  ;
        edSortA  -> Text = OM.CmnOptn.iSortA  ;
        edSortB  -> Text = OM.CmnOptn.iSortB  ;
        edSortC  -> Text = OM.CmnOptn.iSortC  ;
        edSortD  -> Text = OM.CmnOptn.iSortD  ;
        edSortE  -> Text = OM.CmnOptn.iSortE  ;
        edSortF  -> Text = OM.CmnOptn.iSortF  ;
        edSortG  -> Text = OM.CmnOptn.iSortG  ;
        edSortH  -> Text = OM.CmnOptn.iSortH  ;
        edSortI  -> Text = OM.CmnOptn.iSortI  ;
        edSortJ  -> Text = OM.CmnOptn.iSortJ  ;
        edSortK  -> Text = OM.CmnOptn.iSortK  ;
        edSortL  -> Text = OM.CmnOptn.iSortL  ;
    }
    else {
        OM.CmnOptn.iRsltLevel0 = ediRsltLevel0 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel1 = ediRsltLevel1 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel2 = ediRsltLevel2 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel3 = ediRsltLevel3 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel4 = ediRsltLevel4 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel5 = ediRsltLevel5 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel6 = ediRsltLevel6 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel7 = ediRsltLevel7 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel8 = ediRsltLevel8 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevel9 = ediRsltLevel9 -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelA = ediRsltLevelA -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelB = ediRsltLevelB -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelC = ediRsltLevelC -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelD = ediRsltLevelD -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelE = ediRsltLevelE -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelF = ediRsltLevelF -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelG = ediRsltLevelG -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelH = ediRsltLevelH -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelI = ediRsltLevelI -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelJ = ediRsltLevelJ -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelK = ediRsltLevelK -> Text.ToIntDef(0)  ;
        OM.CmnOptn.iRsltLevelL = ediRsltLevelL -> Text.ToIntDef(0)  ;

        OM.CmnOptn.sRsltName0  = edsRsltName0  -> Text  ;
        OM.CmnOptn.sRsltName1  = edsRsltName1  -> Text  ;
        OM.CmnOptn.sRsltName2  = edsRsltName2  -> Text  ;
        OM.CmnOptn.sRsltName3  = edsRsltName3  -> Text  ;
        OM.CmnOptn.sRsltName4  = edsRsltName4  -> Text  ;
        OM.CmnOptn.sRsltName5  = edsRsltName5  -> Text  ;
        OM.CmnOptn.sRsltName6  = edsRsltName6  -> Text  ;
        OM.CmnOptn.sRsltName7  = edsRsltName7  -> Text  ;
        OM.CmnOptn.sRsltName8  = edsRsltName8  -> Text  ;
        OM.CmnOptn.sRsltName9  = edsRsltName9  -> Text  ;
        OM.CmnOptn.sRsltNameA  = edsRsltNameA  -> Text  ;
        OM.CmnOptn.sRsltNameB  = edsRsltNameB  -> Text  ;
        OM.CmnOptn.sRsltNameC  = edsRsltNameC  -> Text  ;
        OM.CmnOptn.sRsltNameD  = edsRsltNameD  -> Text  ;
        OM.CmnOptn.sRsltNameE  = edsRsltNameE  -> Text  ;
        OM.CmnOptn.sRsltNameF  = edsRsltNameF  -> Text  ;
        OM.CmnOptn.sRsltNameG  = edsRsltNameG  -> Text  ;
        OM.CmnOptn.sRsltNameH  = edsRsltNameH  -> Text  ;
        OM.CmnOptn.sRsltNameI  = edsRsltNameI  -> Text  ;
        OM.CmnOptn.sRsltNameJ  = edsRsltNameJ  -> Text  ;
        OM.CmnOptn.sRsltNameK  = edsRsltNameK  -> Text  ;
        OM.CmnOptn.sRsltNameL  = edsRsltNameL  -> Text  ;

        OM.CmnOptn.iRsltColor0 = pnRsltColor0 -> Color  ;
        OM.CmnOptn.iRsltColor1 = pnRsltColor1 -> Color  ;
        OM.CmnOptn.iRsltColor2 = pnRsltColor2 -> Color  ;
        OM.CmnOptn.iRsltColor3 = pnRsltColor3 -> Color  ;
        OM.CmnOptn.iRsltColor4 = pnRsltColor4 -> Color  ;
        OM.CmnOptn.iRsltColor5 = pnRsltColor5 -> Color  ;
        OM.CmnOptn.iRsltColor6 = pnRsltColor6 -> Color  ;
        OM.CmnOptn.iRsltColor7 = pnRsltColor7 -> Color  ;
        OM.CmnOptn.iRsltColor8 = pnRsltColor8 -> Color  ;
        OM.CmnOptn.iRsltColor9 = pnRsltColor9 -> Color  ;
        OM.CmnOptn.iRsltColorA = pnRsltColorA -> Color  ;
        OM.CmnOptn.iRsltColorB = pnRsltColorB -> Color  ;
        OM.CmnOptn.iRsltColorC = pnRsltColorC -> Color  ;
        OM.CmnOptn.iRsltColorD = pnRsltColorD -> Color  ;
        OM.CmnOptn.iRsltColorE = pnRsltColorE -> Color  ;
        OM.CmnOptn.iRsltColorF = pnRsltColorF -> Color  ;
        OM.CmnOptn.iRsltColorG = pnRsltColorG -> Color  ;
        OM.CmnOptn.iRsltColorH = pnRsltColorH -> Color  ;
        OM.CmnOptn.iRsltColorI = pnRsltColorI -> Color  ;
        OM.CmnOptn.iRsltColorJ = pnRsltColorJ -> Color  ;
        OM.CmnOptn.iRsltColorK = pnRsltColorK -> Color  ;

        OM.CmnOptn.iRsltColor22= pnRsltColor22-> Color  ;
        OM.CmnOptn.iRsltColor24= pnRsltColor24-> Color  ;
        OM.CmnOptn.iRsltColor25= pnRsltColor25-> Color  ;
        OM.CmnOptn.iRsltColor26= pnRsltColor26-> Color  ;
        OM.CmnOptn.iRsltColor27= pnRsltColor27-> Color  ;

        OM.CmnOptn.iSort0 = edSort0  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort1 = edSort1  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort2 = edSort2  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort3 = edSort3  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort4 = edSort4  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort5 = edSort5  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort6 = edSort6  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort7 = edSort7  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort8 = edSort8  -> Text.ToIntDef(1);
        OM.CmnOptn.iSort9 = edSort9  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortA = edSortA  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortB = edSortB  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortC = edSortC  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortD = edSortD  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortE = edSortE  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortF = edSortF  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortG = edSortG  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortH = edSortH  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortI = edSortI  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortJ = edSortJ  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortK = edSortK  -> Text.ToIntDef(1);
        OM.CmnOptn.iSortL = edSortL  -> Text.ToIntDef(1);
    }
}
void __fastcall TFormVisionRlstSet::btOkClick(TObject *Sender)
{
    UpdateCmmOptn (toBuff);
    OM.SaveCmnOptn(      );

    m_bChanged = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFormVisionRlstSet::FormShow(TObject *Sender)
{
    OM.LoadCmnOptn(      );
    UpdateCmmOptn (toTabl);        
}
//---------------------------------------------------------------------------


void __fastcall TFormVisionRlstSet::pnRsltColor0Click(TObject *Sender)
{
    TPanel * SelPanel = (TPanel *) Sender ;

    cdSelColor -> Execute() ;

    SelPanel -> Color = cdSelColor -> Color ;


}
//---------------------------------------------------------------------------

