//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FrmSPC.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "UserFile.h"
#include "DataMan.h"
#include "OptionMan.h"
//#include "LanguageUnit.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSPC *FormSPC;
//---------------------------------------------------------------------------
__fastcall TFormSPC::TFormSPC(TComponent* Owner)
    : TForm(Owner)
{
    sgErrHistory->Cells[0][0] = "No" ;
    sgErrHistory->Cells[1][0] = "ErrNo" ;
    sgErrHistory->Cells[2][0] = "Comment" ;
    sgErrHistory->Cells[3][0] = "Occur Date(Time)" ;
    sgErrHistory->Cells[4][0] = "Release Date(Time)" ;
    sgErrCnt    ->Cells[0][0] = "No" ;
    sgErrCnt    ->Cells[1][0] = "ErrNo" ;
    sgErrCnt    ->Cells[2][0] = "Comment" ;
    sgErrCnt    ->Cells[3][0] = "Count" ;

    SPC.SpcArray1.SetParent(pnLotMap ); SPC.SpcArray1.SetConfig(false , "LotMap     "); SPC.SpcArray1.ClearDispItem();


    SPC.SpcArray1.SetDispItem(csNone   ,"None "         ) ;
    SPC.SpcArray1.SetDispItem(csUnkwn  ,"Unkwn "        ) ;
    SPC.SpcArray1.SetDispItem(csWork   ,"Work "         ) ;
    SPC.SpcArray1.SetDispItem(csRslt0  ,"V "+OM.CmnOptn.sRsltName0+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt1  ,"V "+OM.CmnOptn.sRsltName1+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt2  ,"V "+OM.CmnOptn.sRsltName2+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt3  ,"V "+OM.CmnOptn.sRsltName3+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt4  ,"V "+OM.CmnOptn.sRsltName4+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt5  ,"V "+OM.CmnOptn.sRsltName5+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt6  ,"V "+OM.CmnOptn.sRsltName6+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt7  ,"V "+OM.CmnOptn.sRsltName7+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt8  ,"V "+OM.CmnOptn.sRsltName8+" ") ;
    SPC.SpcArray1.SetDispItem(csRslt9  ,"V "+OM.CmnOptn.sRsltName9+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltA  ,"V "+OM.CmnOptn.sRsltNameA+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltB  ,"V "+OM.CmnOptn.sRsltNameB+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltC  ,"V "+OM.CmnOptn.sRsltNameC+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltD  ,"V "+OM.CmnOptn.sRsltNameD+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltE  ,"V "+OM.CmnOptn.sRsltNameE+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltF  ,"V "+OM.CmnOptn.sRsltNameF+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltG  ,"V "+OM.CmnOptn.sRsltNameG+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltH  ,"V "+OM.CmnOptn.sRsltNameH+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltI  ,"V "+OM.CmnOptn.sRsltNameI+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltJ  ,"V "+OM.CmnOptn.sRsltNameJ+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltK  ,"V "+OM.CmnOptn.sRsltNameK+" ") ;
    SPC.SpcArray1.SetDispItem(csRsltL  ,"V "+OM.CmnOptn.sRsltNameL+" ") ;
    SPC.SpcArray1.SetDispItem(csToBuf  ,"ToBuf "        ) ;
    SPC.SpcArray1.SetDispItem(csFromBuf,"FromBuf "      ) ;

    SPC.SpcArray1.PopupMenuVisible(false);

}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::FormCreate(TObject *Sender)
{
    tmUpdate->Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::FormShow(TObject *Sender)
{
    TDateTime CurrDateTime = Now();

    edMtbaDT    ->Format = "yyyy-MM";

    edF_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edFDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edUDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edMtbaDT       ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");

    LT.UpdateDate(sgDate);
    UserFile.GridSearchFile(LOG_FOLDER , sgLotDayName , false);

    lbColor0 -> Color = (TColor)OM.CmnOptn.iRsltColor0 ; lbFailName0 -> Caption = OM.CmnOptn.sRsltName0 ;
    lbColor1 -> Color = (TColor)OM.CmnOptn.iRsltColor1 ; lbFailName1 -> Caption = OM.CmnOptn.sRsltName1 ;
    lbColor2 -> Color = (TColor)OM.CmnOptn.iRsltColor2 ; lbFailName2 -> Caption = OM.CmnOptn.sRsltName2 ;
    lbColor3 -> Color = (TColor)OM.CmnOptn.iRsltColor3 ; lbFailName3 -> Caption = OM.CmnOptn.sRsltName3 ;
    lbColor4 -> Color = (TColor)OM.CmnOptn.iRsltColor4 ; lbFailName4 -> Caption = OM.CmnOptn.sRsltName4 ;
    lbColor5 -> Color = (TColor)OM.CmnOptn.iRsltColor5 ; lbFailName5 -> Caption = OM.CmnOptn.sRsltName5 ;
    lbColor6 -> Color = (TColor)OM.CmnOptn.iRsltColor6 ; lbFailName6 -> Caption = OM.CmnOptn.sRsltName6 ;
    lbColor7 -> Color = (TColor)OM.CmnOptn.iRsltColor7 ; lbFailName7 -> Caption = OM.CmnOptn.sRsltName7 ;
    lbColor8 -> Color = (TColor)OM.CmnOptn.iRsltColor8 ; lbFailName8 -> Caption = OM.CmnOptn.sRsltName8 ;
    lbColor9 -> Color = (TColor)OM.CmnOptn.iRsltColor9 ; lbFailName9 -> Caption = OM.CmnOptn.sRsltName9 ;
    lbColorA -> Color = (TColor)OM.CmnOptn.iRsltColorA ; lbFailNameA -> Caption = OM.CmnOptn.sRsltNameA ;
    lbColorB -> Color = (TColor)OM.CmnOptn.iRsltColorB ; lbFailNameB -> Caption = OM.CmnOptn.sRsltNameB ;
    lbColorC -> Color = (TColor)OM.CmnOptn.iRsltColorC ; lbFailNameC -> Caption = OM.CmnOptn.sRsltNameC ;
    lbColorD -> Color = (TColor)OM.CmnOptn.iRsltColorD ; lbFailNameD -> Caption = OM.CmnOptn.sRsltNameD ;
    lbColorE -> Color = (TColor)OM.CmnOptn.iRsltColorE ; lbFailNameE -> Caption = OM.CmnOptn.sRsltNameE ;
    lbColorF -> Color = (TColor)OM.CmnOptn.iRsltColorF ; lbFailNameF -> Caption = OM.CmnOptn.sRsltNameF ;
    lbColorG -> Color = (TColor)OM.CmnOptn.iRsltColorG ; lbFailNameG -> Caption = OM.CmnOptn.sRsltNameG ;
    lbColorH -> Color = (TColor)OM.CmnOptn.iRsltColorH ; lbFailNameH -> Caption = OM.CmnOptn.sRsltNameH ;
    lbColorI -> Color = (TColor)OM.CmnOptn.iRsltColorI ; lbFailNameI -> Caption = OM.CmnOptn.sRsltNameI ;
    lbColorJ -> Color = (TColor)OM.CmnOptn.iRsltColorJ ; lbFailNameJ -> Caption = OM.CmnOptn.sRsltNameJ ;
    lbColorK -> Color = (TColor)OM.CmnOptn.iRsltColorK ; lbFailNameK -> Caption = OM.CmnOptn.sRsltNameK ;
    lbColorL -> Color = (TColor)OM.CmnOptn.iRsltColorL ; lbFailNameL -> Caption = OM.CmnOptn.sRsltNameL ;




}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false ;
    //Map Update
    SPC.SpcArray1.UpdateAray();

    tmUpdate->Enabled = true ;
}
//---------------------------------------------------------------------------
//LOT INFO "DATE" CLICKED EVENT dd
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgDateClick(TObject *Sender)
{
    LT.UpdateLotName(sgDate , sgLot);
    sgLot -> Col = 1 ;
    sgLotClick(Sender);
    //LT.DispLotDate(sgDate , sgLot);
}
//---------------------------------------------------------------------------
//LOT INFO "LOT NAME" CLICKED EVENT dd
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgLotClick(TObject *Sender)
{
    LT.DispLotDate(sgDate , sgLot , sgLotInfo);

    AnsiString sPath;
    sPath = LOG_FOLDER + sgDate ->Cells[1][sgDate   -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
    sPath.Delete(sPath.Pos(".csv"),4);
    UserFile.GridSearchDir(sPath , sgMgzNo  , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
//    for(int i=0; i<sgMgzNo->RowCount; i++) {
//        if(sgMgzNo->Cells[1][i] == "Log") sgMgzNo->Cells[1][i] = "";
//    }
    sgMgzNoClick(Sender);

    sgLotInfo -> Col = 1 ;
    sgLotInfoClick(Sender);
}
//---------------------------------------------------------------------------
//LOT DAY INFO CLICKED EVENT dd
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgLotDayNameClick(TObject *Sender)
{
    LT.DispLotDayDate(sgLotDayName , sgLotDayInfo);
}
//---------------------------------------------------------------------------
//LOT INFO "MGZ" CLICKED EVENT dd
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgMgzNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath  = LOG_FOLDER + sgDate ->Cells[1][sgDate -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] ;
    UserFile.GridSearchFile(sPath , sgSlotNo  , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    sgSlotNoClick(Sender);
}
//---------------------------------------------------------------------------
//LOT INFO "Slot No" CLICKED EVENT dd
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgSlotNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath  = LOG_FOLDER + sgDate->Cells[1][sgDate -> Row] + "\\" + sgLot->Cells[1][sgLot -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] + "\\" + sgSlotNo->Cells[1][sgSlotNo -> Row] ;
//TODO:    LT.ReadArayData(sPath , riSPC);
    SPC.ReadArayData(sPath );//, riSPC);
//    SPC.ReadLotMap(sPath , sgLotMap);
}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::btLotInfoViewClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;

         if(sTemp == "btLotInfoView" ) { LT.DispLotDate(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);                 }
    else if(sTemp == "btLotDayView"  ) { LT.DispLotDayDate(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo); }
    else if(sTemp == "btErrDataView" ) { SPC.DispErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);             }
    else if(sTemp == "btErrCntView"  ) { SPC.DispErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);                    }
    else if(sTemp == "btMtbaView"    ) { SPC.DispMtba(edMtbaDT->Date.FormatString("yyyymm") , S1_Mtba);                                                                   }

}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::btLotInfoSaveClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;

         if(sTemp == "btLotInfoSave"     ) { LT.SaveLotData(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);                 }
    else if(sTemp == "btLotDaySave"      ) { LT.SaveLotDayData(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo); }
    else if(sTemp == "btErrDataSave"     ) { SPC.SaveErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);             }
    else if(sTemp == "btErrCntSave"      ) { SPC.SaveErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);                    }
    else if(sTemp == "btMtbaSave"        ) { SPC.SaveMtba(edMtbaDT->Date.FormatString("yyyymm"),S1_Mtba);                                                                     }
    else if(sTemp == "btLotInfoMapSave"  )
    {
        if(sgMgzNo -> RowCount == 0 ) return ;
        if(sgMgzNo -> Cells[1][0] == "" ) return ;
        if(sgLot  ->Cells[1][sgLot -> Row] == "" ) return ;
        if(!sdSPC->Execute()) return ;

        for(int i=0; i<sgMgzNo -> RowCount; i++)
        {
            if(sgMgzNo -> Cells[1][i] == "" ) return ;
            AnsiString sPath ;
            sPath  = LOG_FOLDER + sgDate ->Cells[1][sgDate -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
            sPath.Delete(sPath.Pos(".csv"),4);
            sPath += "\\" + sgMgzNo->Cells[1][i] ;

            SPC.SaveLotMap(sPath, sdSPC->FileName);
        }
    }
}
//---------------------------------------------------------------------------
//LOT INFO "LOT INFO" CLICKED EVENT dd
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgLotInfoClick(TObject *Sender)
{
//    edTotal  -> Text = sgLotInfo -> Cells[17][sgLotInfo -> Row] ;
    AnsiString sTemp = sgLotInfo -> Cells[0][1];

    edTotal  -> Text = sgLotInfo -> Cells[17][sgLotInfo -> Row] ;

    edRslt0  -> Text = sgLotInfo -> Cells[18][sgLotInfo -> Row] ;
    edRslt1  -> Text = sgLotInfo -> Cells[19][sgLotInfo -> Row] ;
    edRslt2  -> Text = sgLotInfo -> Cells[20][sgLotInfo -> Row] ;
    edRslt3  -> Text = sgLotInfo -> Cells[21][sgLotInfo -> Row] ;
    edRslt4  -> Text = sgLotInfo -> Cells[22][sgLotInfo -> Row] ;
    edRslt5  -> Text = sgLotInfo -> Cells[23][sgLotInfo -> Row] ;
    edRslt6  -> Text = sgLotInfo -> Cells[24][sgLotInfo -> Row] ;
    edRslt7  -> Text = sgLotInfo -> Cells[25][sgLotInfo -> Row] ;
    edRslt8  -> Text = sgLotInfo -> Cells[26][sgLotInfo -> Row] ;
    edRslt9  -> Text = sgLotInfo -> Cells[27][sgLotInfo -> Row] ;
    edRsltA  -> Text = sgLotInfo -> Cells[28][sgLotInfo -> Row] ;
    edRsltB  -> Text = sgLotInfo -> Cells[29][sgLotInfo -> Row] ;
    edRsltC  -> Text = sgLotInfo -> Cells[30][sgLotInfo -> Row] ;
    edRsltD  -> Text = sgLotInfo -> Cells[31][sgLotInfo -> Row] ;
    edRsltE  -> Text = sgLotInfo -> Cells[32][sgLotInfo -> Row] ;
    edRsltF  -> Text = sgLotInfo -> Cells[33][sgLotInfo -> Row] ;
    edRsltG  -> Text = sgLotInfo -> Cells[34][sgLotInfo -> Row] ;
    edRsltH  -> Text = sgLotInfo -> Cells[35][sgLotInfo -> Row] ;
    edRsltI  -> Text = sgLotInfo -> Cells[36][sgLotInfo -> Row] ;
    edRsltJ  -> Text = sgLotInfo -> Cells[37][sgLotInfo -> Row] ;
    edRsltK  -> Text = sgLotInfo -> Cells[38][sgLotInfo -> Row] ;
    edRsltL  -> Text = sgLotInfo -> Cells[39][sgLotInfo -> Row] ;
}


