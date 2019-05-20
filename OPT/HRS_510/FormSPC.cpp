//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormSPC.h"
#include "LotUnit.h"
#include "SpcUnit.h"
#include "UserFile.h"
#include "DataMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#include "SpcSubUnit.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
__fastcall TFrmSPC::TFrmSPC(TComponent* Owner)
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

    pcSPC -> ActivePageIndex = 0 ; //Lot No Have

}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::UpdateComOptn(bool _bToTable)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::FormShow(TObject *Sender)
{
    TDateTime CurrDateTime = Now();

    edMtbaDT    ->Format = "yyyy-MM";

    //FuckFuck
    edF_LotInfo    ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edU_LotInfo    ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edFDataTime    ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edUDataTime    ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edF_ErrCnt     ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edU_ErrCnt     ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edMtbaDT       ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edF_LotDayInfo ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");
    edU_LotDayInfo ->Date = CurrDateTime;//.FormatString("dd/mm/yyyy");

    //Data 표기.
    UserFile.GridSearchFile(LOG_FOLDER              , sgDate             , 1 , false); // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    UserFile.GridSearchFile(SPC_HEIGHT_FOLDER       , sgHeightDataDate   , 1 , false); // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    UserFile.GridSearchFile(SPC_LEFT_STATOR_FOLDER  , sgLSHeightDataDate , 1 , false); // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    UserFile.GridSearchFile(SPC_RIGHT_STATOR_FOLDER , sgRSHeightDataDate , 1 , false); // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgDateClick(TObject *Sender)
{
    String sPath = LOG_FOLDER ;
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());
    sPath += sgDate ->Cells[1][sgDate   -> Row] ;

    //StringGrid Clear
    for(int i =0 ; i < sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < sgLotInfo->RowCount ; j++ ) {
            sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    sgLotInfo -> ColCount         = 18  ;
    sgLotInfo -> DefaultColWidth  = 65  ;
    sgLotInfo -> DefaultRowHeight = 25  ;
    sgLotInfo -> ColWidths[0]     = 20  ;
//    sgLotInfo -> ColWidths[1]     = 60  ;
//    sgLotInfo -> ColWidths[2]     = 80 ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    char    *pfbuf;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 800 ) return ;
        iRowCnt++ ;
    }

    sgLotInfo -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    sgLotInfo -> RowCount = iRowCnt ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::btLotInfoViewClick(TObject *Sender)
{
    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    String  sName= pBtn->Name;

         if(sName == "btLotInfoView") { SPC.DispLotDate(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);                    }
    else if(sName == "btLotInfoSave") { SPC.SaveLotData(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);                    }
    else if(sName == "btLotDayView" ) { SPC.DispLotDayDate(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);    }
    else if(sName == "btLotDaySave" ) { SPC.SaveLotDayData(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);    }
    else if(sName == "btErrDataView") { SPC.DispErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);                 }
    else if(sName == "btErrDataSave") { SPC.SaveErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);                 }
    else if(sName == "btErrCntView" ) { SPC.DispErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);                        }
    else if(sName == "btErrCntSave" ) { SPC.SaveErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);                        }
    else if(sName == "btMtbaView"   ) { SPC.DispMtba(edMtbaDT->Date.FormatString("yyyymm") , S1_Mtba);                                                                       }
    else if(sName == "btMtbaSave"   ) { SPC.SaveMtba(edMtbaDT->Date.FormatString("yyyymm"),S1_Mtba);                                                                         }


}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotDayNameClick(TObject *Sender)
{
    SPC.DispLotDayDate(sgLotDayName , sgLotDayInfo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotInfoClick(TObject *Sender)
{
    if(sgLotInfo->Cells[4][sgLotInfo->Row] == "") return ;
    //EX 14:55:48
    String sPath ,sName ;
    sPath  = SPC_FOLDER ;
    sPath += "Map\\" + sgDate ->Cells[1][sgDate   -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);

    sName = sgLotInfo->Cells[4][sgLotInfo->Row] ;
    sName.Insert("_",sName.Pos(":")); sName.Delete(sName.Pos(":") , 1);
    sName.Insert("_",sName.Pos(":")); sName.Delete(sName.Pos(":") , 1);

    sPath += "\\" + sName + ".ini" ;

    //StringGrid Clear
    for(int i =0 ; i < sgMap->ColCount ; i++ ) {
        for(int j =0 ; j < sgMap->RowCount ; j++ ) {
            sgMap -> Cells[i][j] = "" ;
        }
    }

    SPCS.ReadHeightData(sPath , sgMap);

}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::Button1Click(TObject *Sender)
{
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            dRotorH[r][c] = rand()%10 ;
        }
    }

    SPCS.WriteDataCsv() ;

    LT.LotInfo.dRunTime    = Now() ;
    LT.LotInfo.dIdleTime   = Now() ;
    LT.LotInfo.dJamTime    = Now() ;
    LT.LotInfo.dTotalTime  = Now() ;
    LT.LotInfo.iStrpUPEH   = Now() ;
    LT.LotInfo.iChipUPEH   = Now() ;
    LT.LotInfo.iStrpUPH    = Now() ;
    LT.LotInfo.iChipUPH    = Now() ;
    LT.LotInfo.iWorkStrp   = 3 ;
    LT.LotInfo.iWorkChip   = 4 ;
    LT.LotInfo.iFailChip   = 5 ;
    LT.LotInfo.iCntRslt0   = 6 ;
    LT.LotInfo.iCntRslt1   = 7 ;
    LT.LotInfo.iCntRslt2   = 8 ;
    LT.LotInfo.iCntRslt3   = 9 ;
    LT.LotInfo.iCntRslt4   = 10 ;
    LT.LotInfo.iCntRslt5   = 11 ;

    LT.WriteLotLog();

}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgHeightDataDateClick(TObject *Sender)
{
    String sPath = SPC_HEIGHT_FOLDER ;
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());
    sPath += sgHeightDataDate ->Cells[1][sgHeightDataDate   -> Row] ;

    //StringGrid Clear
    for(int i =0 ; i < sgHeightData->ColCount ; i++ ) {
        for(int j =1 ; j < sgHeightData->RowCount ; j++ ) {
            sgHeightData -> Cells[i][j] = "" ;
        }
    }

    sgHeightData -> ColCount         = 3   ;
    sgHeightData -> DefaultColWidth  = 65  ;
    sgHeightData -> DefaultRowHeight = 25  ;
    sgHeightData -> ColWidths[0]     = 50  ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    char    *pfbuf;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 50000 ) return ;
        iRowCnt++ ;
    }

    sgHeightData -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")+1) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        sgHeightData -> Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            sgHeightData -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr ;
        sItmStr.Delete(sRowStr.Pos("\r\n") ,2 ) ;
        sgHeightData -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    sgHeightData -> RowCount = iRowCnt ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgLSHeightDataDateClick(TObject *Sender)
{
    String sPath = SPC_LEFT_STATOR_FOLDER ;
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());
    sPath += sgLSHeightDataDate ->Cells[1][sgLSHeightDataDate   -> Row] ;

    //StringGrid Clear
    for(int i =0 ; i < sgLSHeightData->ColCount ; i++ ) {
        for(int j =1 ; j < sgLSHeightData->RowCount ; j++ ) {
            sgLSHeightData -> Cells[i][j] = "" ;
        }
    }

    sgLSHeightData -> ColCount         = 3   ;
    sgLSHeightData -> DefaultColWidth  = 65  ;
    sgLSHeightData -> DefaultRowHeight = 25  ;
    sgLSHeightData -> ColWidths[0]     = 50  ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    char    *pfbuf;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 50000 ) return ;
        iRowCnt++ ;
    }

    sgLSHeightData -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")+1) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        sgLSHeightData -> Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            sgLSHeightData -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr ;
        sItmStr.Delete(sRowStr.Pos("\r\n") ,2 ) ;
        sgLSHeightData -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    sgLSHeightData -> RowCount = iRowCnt ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgRSHeightDataDateClick(TObject *Sender)
{
    String sPath = SPC_RIGHT_STATOR_FOLDER ;
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());
    sPath += sgRSHeightDataDate ->Cells[1][sgRSHeightDataDate   -> Row] ;

    //StringGrid Clear
    for(int i =0 ; i < sgRSHeightData->ColCount ; i++ ) {
        for(int j =1 ; j < sgRSHeightData->RowCount ; j++ ) {
            sgRSHeightData -> Cells[i][j] = "" ;
        }
    }

    sgRSHeightData -> ColCount         = 3   ;
    sgRSHeightData -> DefaultColWidth  = 65  ;
    sgRSHeightData -> DefaultRowHeight = 25  ;
    sgRSHeightData -> ColWidths[0]     = 50  ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    char    *pfbuf;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 50000 ) return ;
        iRowCnt++ ;
    }

    sgRSHeightData -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")+1) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        sgRSHeightData -> Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            sgRSHeightData -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr ;
        sItmStr.Delete(sRowStr.Pos("\r\n") ,2 ) ;
        sgRSHeightData -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    sgRSHeightData -> RowCount = iRowCnt ;
}
//---------------------------------------------------------------------------

