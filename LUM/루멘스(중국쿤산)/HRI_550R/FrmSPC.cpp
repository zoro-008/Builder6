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

    sgLotInfo   ->Cells[0][0]  = "WriteTime";
    sgLotInfo   ->Cells[1][0]  = "LotNo"    ;
    sgLotInfo   ->Cells[2][0]  = "JobFile"  ;
    sgLotInfo   ->Cells[3][0]  = "Operator" ;
    sgLotInfo   ->Cells[4][0]  = "StartTime";
    sgLotInfo   ->Cells[5][0]  = "EndTime"  ;
    sgLotInfo   ->Cells[6][0]  = "RunTime"  ;
    sgLotInfo   ->Cells[7][0]  = "IdleTime" ;
    sgLotInfo   ->Cells[8][0]  = "JamTime"  ;
    sgLotInfo   ->Cells[9][0]  = "TotalTime";
    sgLotInfo   ->Cells[10][0] = "StrpUPEH" ;
    sgLotInfo   ->Cells[11][0] = "ChipUPEH" ;
    sgLotInfo   ->Cells[12][0] = "StrpUPH"  ;
    sgLotInfo   ->Cells[13][0] = "ChipUPH"  ;
    sgLotInfo   ->Cells[14][0] = "WorkStrp" ;
    sgLotInfo   ->Cells[15][0] = "WorkChip" ;
    sgLotInfo   ->Cells[16][0] = "FailChip" ;
    sgLotInfo   ->Cells[17][0] = "Empty"    ;
    sgLotInfo   ->Cells[18][0] = "Chip"     ;
    sgLotInfo   ->Cells[19][0] = "Dust"     ;
    sgLotInfo   ->Cells[20][0] = "L_Flow"   ;
    sgLotInfo   ->Cells[21][0] = "Broken"   ;
    sgLotInfo   ->Cells[22][0] = "Crack"    ;
    sgLotInfo   ->Cells[23][0] = "Burr"     ;
    sgLotInfo   ->Cells[24][0] = "Zener"    ;
    sgLotInfo   ->Cells[25][0] = "Wire"     ;
    sgLotInfo   ->Cells[26][0] = "Orient"   ;
    sgLotInfo   ->Cells[27][0] = "PMeasure" ;
    sgLotInfo   ->Cells[28][0] = "PDist"    ;
    sgLotInfo   ->Cells[29][0] = "Matching" ;
    sgLotInfo   ->Cells[30][0] = "Flow"     ;
//    sgLotInfo   ->Cells[31][0] = "Heater"   ;

    sgLotDayInfo->Cells[0][0]  = "WriteTime,"   ;
    sgLotDayInfo->Cells[1][0]  = "RunTime,"     ;
    sgLotDayInfo->Cells[2][0]  = "IdleTime,"    ;
    sgLotDayInfo->Cells[3][0]  = "JamTime,"     ;
    sgLotDayInfo->Cells[4][0]  = "TotalTime,"   ;
    sgLotDayInfo->Cells[5][0]  = "StrpUPEH,"    ;
    sgLotDayInfo->Cells[6][0]  = "ChipUPEH,"    ;
    sgLotDayInfo->Cells[7][0]  = "StrpUPH,"     ;
    sgLotDayInfo->Cells[8][0]  = "ChipUPH,"     ;
    sgLotDayInfo->Cells[9][0]  = "WorkStrp,"    ;
    sgLotDayInfo->Cells[10][0] = "WorkChip,"    ;
    sgLotDayInfo->Cells[11][0] = "FailChip,"    ;
    sgLotDayInfo->Cells[12][0] = "LotCnt,"      ;
    sgLotDayInfo->Cells[13][0] = "CntRslt1,"    ;
    sgLotDayInfo->Cells[14][0] = "CntRslt2,"    ;
    sgLotDayInfo->Cells[15][0] = "CntRslt3,"    ;
    sgLotDayInfo->Cells[16][0] = "CntRslt4,"    ;
    sgLotDayInfo->Cells[17][0] = "CntRslt5,"    ;
    sgLotDayInfo->Cells[18][0] = "CntRslt6,"    ;
    sgLotDayInfo->Cells[19][0] = "CntRslt7,"    ;
    sgLotDayInfo->Cells[20][0] = "CntRslt8,"    ;
    sgLotDayInfo->Cells[21][0] = "CntRslt9,"    ;
    sgLotDayInfo->Cells[22][0] = "CntRslt10,"   ;
    sgLotDayInfo->Cells[23][0] = "CntRslt11,"   ;
    sgLotDayInfo->Cells[24][0] = "CntRslt12,"   ;
    sgLotDayInfo->Cells[25][0] = "CntRslt13,"   ;
    sgLotDayInfo->Cells[26][0] = "CntRslt14,"   ;
//    sgLotDayInfo->Cells[27][0] = "CntHeatFail," ;

    DM.ARAY[riSPC].SetParent(pnLotMap  ); DM.ARAY[riSPC].SetConfig(false , "LotMap  "); DM.ARAY[riSPC].ClearDispItem();

    DM.ARAY[riSPC].SetDispItem(csEmpty   ,"Empty "        ) ; //DM.ARAY[riPSB ].SetDispColor(csEmpty ,clSilver          );
    DM.ARAY[riSPC].SetDispItem(csUnkwn   ,"Unknown "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen      );
    DM.ARAY[riSPC].SetDispItem(csFail    ,"Review Fail "  ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riSPC].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPSB ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riSPC].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riPSB ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riSPC].SetDispItem(csRslt1   ,"V Empty "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt1 ,clBlack           );
    DM.ARAY[riSPC].SetDispItem(csRslt2   ,"V Chip "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt2 ,clRed             );
    DM.ARAY[riSPC].SetDispItem(csRslt3   ,"V Dust "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt3 ,clGreen           );
    DM.ARAY[riSPC].SetDispItem(csRslt4   ,"V LFlow "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt4 ,clBlue            );
    DM.ARAY[riSPC].SetDispItem(csRslt5   ,"V Broken "     ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt5 ,clLime            );
    DM.ARAY[riSPC].SetDispItem(csRslt6   ,"V Crack "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt6 ,clFuchsia         );
    DM.ARAY[riSPC].SetDispItem(csRslt7   ,"V Burr "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt7 ,clWhite           );
    DM.ARAY[riSPC].SetDispItem(csRslt8   ,"V Zener "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt8 ,clMaroon          );
    DM.ARAY[riSPC].SetDispItem(csRslt9   ,"V Wire "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt9 ,clTeal            );
    DM.ARAY[riSPC].SetDispItem(csRslt10  ,"V Orient "     ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt10,clAqua            );
    DM.ARAY[riSPC].SetDispItem(csRslt11  ,"V Pmeasure "   ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt11,clNavy            );
    DM.ARAY[riSPC].SetDispItem(csRslt12  ,"V Pdist "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt12,clOlive           );
    DM.ARAY[riSPC].SetDispItem(csRslt13  ,"V Match "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt13,clYellow          );
    DM.ARAY[riSPC].SetDispItem(csRslt14  ,"V Flow "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt14,clSkyBlue         );

    lbFailName1  -> Caption = OM.CmnOptn.sCheckR1FailName    ;
    lbFailName2  -> Caption = OM.CmnOptn.sCheckR2FailName    ;
    lbFailName3  -> Caption = OM.CmnOptn.sCheckR3FailName    ;
    lbFailName4  -> Caption = OM.CmnOptn.sCheckR4FailName    ;
    lbFailName5  -> Caption = OM.CmnOptn.sCheckR5FailName    ;
    lbFailName6  -> Caption = OM.CmnOptn.sCheckR6FailName    ;
    lbFailName7  -> Caption = OM.CmnOptn.sCheckR7FailName    ;
    lbFailName8  -> Caption = OM.CmnOptn.sCheckR8FailName    ;
    lbFailName9  -> Caption = OM.CmnOptn.sCheckR9FailName    ;
    lbFailName10 -> Caption = OM.CmnOptn.sCheckR10FailName   ;
    lbFailName11 -> Caption = OM.CmnOptn.sCheckR11FailName   ;
    lbFailName12 -> Caption = OM.CmnOptn.sCheckR12FailName   ;
    lbFailName13 -> Caption = OM.CmnOptn.sCheckR13FailName   ;
    lbFailName14 -> Caption = OM.CmnOptn.sCheckR14FailName   ;


//    LAG.ReadFrmSPC();
//    if(FM_GetLan() == lsKorean) LAG.LoadFrmSPCHan();
//    switch(FM_GetLan())
//    {
//        default        :                             break ;
//        case lsEnglish :                             break ;
//        case lsKorean  : LAG.LoadFrmSPCHan();     break ;
//        case lsChinese :                             break ;
//    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::UpdateComOptn(bool _bToTable)
{
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

}
//---------------------------------------------------------------------------
void __fastcall TFormSPC::sgDateClick(TObject *Sender)
{
    LT.UpdateLotName(sgDate , sgLot);
    sgLot -> Col = 1 ;
    sgLotClick(Sender);
    //LT.DispLotDate(sgDate , sgLot);
}
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

void __fastcall TFormSPC::btErrDataViewClick(TObject *Sender)
{
    SPC.DispErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btErrCntViewClick(TObject *Sender)
{
    SPC.DispErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btMtbaViewClick(TObject *Sender)
{
//    ChatMtba -> CleanupInstance();
//    S1_Mtba->Clear();
    SPC.DispMtba(edMtbaDT->Date.FormatString("yyyymm") , S1_Mtba);
//    dbMtba->LeftAxis
//    Series1->AddXY()
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btMtbaSaveClick(TObject *Sender)
{
    SPC.SaveMtba(edMtbaDT->Date.FormatString("yyyymm"),S1_Mtba);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btErrCntSaveClick(TObject *Sender)
{
    SPC.SaveErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btErrDataSaveClick(TObject *Sender)
{
    SPC.SaveErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btLotInfoViewClick(TObject *Sender)
{
    LT.DispLotDate(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btLotInfoSaveClick(TObject *Sender)
{
    LT.SaveLotData(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::sgLotDayNameClick(TObject *Sender)
{
    LT.DispLotDayDate(sgLotDayName , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btLotDayViewClick(TObject *Sender)
{
    LT.DispLotDayDate(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::btLotDaySaveClick(TObject *Sender)
{
    LT.SaveLotDayData(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFormSPC::sgSlotNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath  = LOG_FOLDER + sgDate->Cells[1][sgDate -> Row] + "\\" + sgLot->Cells[1][sgLot -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] + "\\" + sgSlotNo->Cells[1][sgSlotNo -> Row] ;
    LT.ReadArayData(sPath , riSPC);
//    SPC.ReadLotMap(sPath , sgLotMap);
}
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

void __fastcall TFormSPC::sgLotInfoClick(TObject *Sender)
{
//    edTotal  -> Text = sgLotInfo -> Cells[17][sgLotInfo -> Row] ;
    AnsiString sTemp = sgLotInfo -> Cells[0][1];

    edRslt1  -> Text = sgLotInfo -> Cells[18][sgLotInfo -> Row] ;
    edRslt2  -> Text = sgLotInfo -> Cells[19][sgLotInfo -> Row] ;
    edRslt3  -> Text = sgLotInfo -> Cells[20][sgLotInfo -> Row] ;
    edRslt4  -> Text = sgLotInfo -> Cells[21][sgLotInfo -> Row] ;
    edRslt5  -> Text = sgLotInfo -> Cells[22][sgLotInfo -> Row] ;
    edRslt6  -> Text = sgLotInfo -> Cells[23][sgLotInfo -> Row] ;
    edRslt7  -> Text = sgLotInfo -> Cells[24][sgLotInfo -> Row] ;
    edRslt8  -> Text = sgLotInfo -> Cells[25][sgLotInfo -> Row] ;
    edRslt9  -> Text = sgLotInfo -> Cells[26][sgLotInfo -> Row] ;
    edRslt10 -> Text = sgLotInfo -> Cells[27][sgLotInfo -> Row] ;
    edRslt11 -> Text = sgLotInfo -> Cells[28][sgLotInfo -> Row] ;
    edRslt12 -> Text = sgLotInfo -> Cells[29][sgLotInfo -> Row] ;
    edRslt13 -> Text = sgLotInfo -> Cells[30][sgLotInfo -> Row] ;
    edRslt14 -> Text = sgLotInfo -> Cells[31][sgLotInfo -> Row] ;
    edFail   -> Text = sgLotInfo -> Cells[32][sgLotInfo -> Row] ;
    edTotal  -> Text = sgLotInfo -> Cells[17][sgLotInfo -> Row] ;

}
//---------------------------------------------------------------------------







