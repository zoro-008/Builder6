//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SpcUnit.h"
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
    OM.SaveEqpOptn();

    clStat[csRslt0 ] = (TColor)OM.CmnOptn.iRsltColor0;
    clStat[csRslt1 ] = (TColor)OM.CmnOptn.iRsltColor1;
    clStat[csRslt2 ] = (TColor)OM.CmnOptn.iRsltColor2;
    clStat[csRslt3 ] = (TColor)OM.CmnOptn.iRsltColor3;
    clStat[csRslt4 ] = (TColor)OM.CmnOptn.iRsltColor4;
    clStat[csRslt5 ] = (TColor)OM.CmnOptn.iRsltColor5;
    clStat[csRslt6 ] = (TColor)OM.CmnOptn.iRsltColor6;
    clStat[csRslt7 ] = (TColor)OM.CmnOptn.iRsltColor7;
    clStat[csRslt8 ] = (TColor)OM.CmnOptn.iRsltColor8;
    clStat[csRslt9 ] = (TColor)OM.CmnOptn.iRsltColor9;
    clStat[csRsltA ] = (TColor)OM.CmnOptn.iRsltColorA;
    clStat[csRsltB ] = (TColor)OM.CmnOptn.iRsltColorB;
    clStat[csRsltC ] = (TColor)OM.CmnOptn.iRsltColorC;

//    UserMsg2("Confirm" , "저장 되었습니다.");
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbLoadingStop       -> Checked = OM.CmnOptn.bLoadingStop     ;
        cbEmptyIgnr         -> Checked = OM.CmnOptn.bEmptyIgnr       ;
        edVacDelay          -> Text    = OM.CmnOptn.iVacDelay        ;
        edPlaceDelay        -> Text    = OM.CmnOptn.iPlaceDelay      ;
        edPlaceEjtTime      -> Text    = OM.CmnOptn.iPlaceEjtTime    ;
        edRezeroPickCnt     -> Text    = OM.CmnOptn.iRezeroPickCnt   ;
        edMasterOffset      -> Text    = OM.CmnOptn.dMasterOffset    ;
        cbChuckFailIgnore   -> Checked = OM.CmnOptn.bChuckFailIgnore ;
        cbUseBinFullIgnore  -> Checked = OM.CmnOptn.bUseBinFullIgnore;
        cbUseFailBinIgnore  -> Checked = OM.CmnOptn.bUseFailBinIgnore;

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

    }
    else {
        OM.CmnOptn.bLoadingStop       = cbLoadingStop       -> Checked         ;
        OM.CmnOptn.bEmptyIgnr         = cbEmptyIgnr         -> Checked         ;
        OM.CmnOptn.iVacDelay          = edVacDelay          -> Text.ToIntDef(0);
        OM.CmnOptn.iPlaceDelay        = edPlaceDelay        -> Text.ToIntDef(0);
        OM.CmnOptn.iPlaceEjtTime      = edPlaceEjtTime      -> Text.ToIntDef(0);
        OM.CmnOptn.iRezeroPickCnt     = edRezeroPickCnt     -> Text.ToIntDef(0);
        OM.CmnOptn.dMasterOffset      = edMasterOffset      -> Text.ToDouble( );
        OM.CmnOptn.bChuckFailIgnore   = cbChuckFailIgnore   -> Checked         ;
        OM.CmnOptn.bUseBinFullIgnore  = cbUseBinFullIgnore  -> Checked         ;
        OM.CmnOptn.bUseFailBinIgnore  = cbUseFailBinIgnore  -> Checked         ;

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

void __fastcall TFrmOption::pnRsltColor1Click(TObject *Sender)
{
    TPanel * SelPanel = (TPanel *) Sender ;

    cdSelColor -> Execute() ;

    SelPanel -> Color = cdSelColor -> Color ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::cbUseBinFullIgnoreClick(TObject *Sender)
{
    if(!OM.CmnOptn.bUseBinFullIgnore) {
        if(!DM.ARAY[riPCK].CheckAllStat(csNone) ||
           !DM.ARAY[riSRT].CheckAllStat(csNone) ||
           !DM.ARAY[riSTN].CheckAllStat(csNone) ) {
            FM_MsgTime ("Notification" , "Plz Remove Pick Sorter Station Rotor" , OM.MstOptn.iMgzShowTime);
            cbUseBinFullIgnore->Checked = false ;
//            return ;
        }

    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::cbUseFailBinIgnoreClick(TObject *Sender)
{
    if(!OM.CmnOptn.bUseBinFullIgnore) {
        if(!DM.ARAY[riPCK].CheckAllStat(csNone) ||
           !DM.ARAY[riSRT].CheckAllStat(csNone) ||
           !DM.ARAY[riSTN].CheckAllStat(csNone) ) {
            FM_MsgTime ("Notification" , "Plz Remove Pick Sorter Station Rotor" , OM.MstOptn.iMgzShowTime);
            cbUseBinFullIgnore->Checked = false ;
//            return ;
        }

    }    
}
//---------------------------------------------------------------------------

