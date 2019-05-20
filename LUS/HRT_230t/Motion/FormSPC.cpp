//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormSPC.h"
#include "LotUnit.h"
#include "SpcUnit.h"
#include "UserFile.h"
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

    //Use Map or Data
    //m_bUseMap = false ;
    //if(m_bUseMap) {
    //    pnMap -> Align   = alClient ;
    //    pnMap -> Visible = false ;
    //    sgMap -> Align   = alClient ;
    //    sgMap -> Visible = true ;
    //}
    //
    //else {
    //    pnMap -> Align   = alClient ;
    //    pnMap -> Visible = true ;
    //    sgMap -> Align   = alClient ;
    //    sgMap -> Visible = false ;
    //
    //    SPC.SpcArray1.SetParent(pnMap ); SPC.SpcArray1.SetConfig(false , "LotMap     "); SPC.SpcArray1.ClearDispItem();
    //    SPC.SpcArray1.SetDispItem(csNone   ,"None "         ) ;
    //    SPC.SpcArray1.SetDispItem(csUnkwn  ,"Unkwn "        ) ;
    //    SPC.SpcArray1.SetDispItem(csWork   ,"Work "         ) ;
    //    SPC.SpcArray1.SetDispItem(csRslt0  ,"V Rslt0 "      ) ;
    //    SPC.SpcArray1.SetDispItem(csRslt1  ,"V Rslt1 "      ) ;
    //    SPC.SpcArray1.SetDispItem(csRslt2  ,"V Rslt2 "      ) ;
//  //      SPC.SpcArray1.SetDispItem(csToBuf  ,"ToBuf "        ) ;
//  //      SPC.SpcArray1.SetDispItem(csFromBuf,"FromBuf "      ) ;
    //
    //    SPC.SpcArray1.PopupMenuEnable(false);
    //}
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

    edF_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edFDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edUDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edMtbaDT       ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");

    //Data 표기.
    UserFile.GridSearchDir(LOG_FOLDER  , sgDate       , 1 , false);   // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
//    UserFile.GridSearchFile(LOG_FOLDER , sgDate       , 1 , false); // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    UserFile.GridSearchFile(LOG_FOLDER , sgLotDayName , 1 , false);

    tmUpdate->Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgDateClick(TObject *Sender)
{
    AnsiString sDay ,sPath ;
    sDay = sgDate->Cells[1][sgDate -> Row] ;
    sPath = LOG_FOLDER + sDay + "\\" ;
    UserFile.GridSearchFile(sPath , sgLot , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    sgLot -> Col = 1 ;
    sgLotClick(Sender);

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
    else if(sName == "btMtbaSave"   ) { SPC.SaveMtba(edMtbaDT->Date.FormatString("yyyymm") , S1_Mtba);                                                                       }
/*
    sdSPC

    SaveLotData
    SaveLotDayData
    SaveErrData
    SaveErrCnt
*/



}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotDayNameClick(TObject *Sender)
{
    SPC.DispLotDayDate(sgLotDayName , sgLotDayInfo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotInfoClick(TObject *Sender)
{
/*
    if(sgLotInfo->Cells[4][sgLotInfo->Row] == "") return ;
    //EX 14:55:48
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
*/        
/*
    String sPath ,sName ;
    sPath  = SPC_FOLDER ;
    sPath += "Map\\" + sgDate ->Cells[1][sgDate   -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);

    sName = sgLotInfo->Cells[4][sgLotInfo->Row] ;
    sName.Insert("_",sName.Pos(":")); sName.Delete(sName.Pos(":") , 1);
    sName.Insert("_",sName.Pos(":")); sName.Delete(sName.Pos(":") , 1);

    sPath += "\\" + sName + ".ini" ;

    if(m_bUseMap) {
        for(int i =0 ; i < sgMap->ColCount ; i++ ) {
            for(int j =0 ; j < sgMap->RowCount ; j++ ) {
                sgMap -> Cells[i][j] = "" ;
            }
        }

//        SPCS.ReadHeightData(sPath , sgMap);
    }
    else SPC.ReadArayData(sPath , &SPC.SpcArray1);

//    UserFile.GridSearchDir(sPath , sgMgzNo  , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
*/
//TODO ::
//    //StringGrid Clear
//    for(int i =0 ; i < sgMap->ColCount ; i++ ) {
//        for(int j =0 ; j < sgMap->RowCount ; j++ ) {
//            sgMap -> Cells[i][j] = "" ;
//        }
//    }
//
//    SPCS.ReadHeightData(sPath , sgMap);

}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::Button1Click(TObject *Sender)
{
//    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
//        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
//            dRotorH[r][c] = rand()%10 ;
//        }
//    }

//    SPCS.WriteDataCsv() ;

    LT.LotInfo.dRunTime    = Now() ;
    LT.LotInfo.dIdleTime   = Now() ;
    LT.LotInfo.dJamTime    = Now() ;
    LT.LotInfo.dTotalTime  = Now() ;
    //LT.LotInfo.iStrpUPEH   = Now() ;
    LT.LotInfo.iChipUPEH   = Now() ;
    //LT.LotInfo.iStrpUPH    = Now() ;
    LT.LotInfo.iChipUPH    = Now() ;
    //LT.LotInfo.iWorkStrp   = 3 ;
    LT.LotInfo.iWorkChip   = 4 ;
    LT.LotInfo.iFailChip   = 5 ;
    //LT.LotInfo.iCntRslt0   = 6 ;
    LT.LotInfo.iCntRslt1   = 7 ;
    LT.LotInfo.iCntRslt2   = 8 ;
    LT.LotInfo.iCntRslt3   = 9 ;
    LT.LotInfo.iCntRslt4   = 10 ;
    LT.LotInfo.iCntRslt5   = 11 ;

//    LT.WriteLotLog();

    LT.WriteLotLog   ();
    LT.WriteLotDayLog();
    //SPC.WriteArayData(&DM.ARAY[riPSB]);


}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotClick(TObject *Sender)
{
    String sPath = LOG_FOLDER ;
    String sDay  = sgDate -> Cells[1][ sgDate -> Row] ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath += sgDate ->Cells[1][sgDate   -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];;
    sPath.Delete(sPath.Pos(".csv"),4);
    UserFile.GridSearchDir(sPath , sgMgzNo  , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    sgMgzNoClick(Sender);

    sPath = LOG_FOLDER + sDay + "\\" + sgLot -> Cells[1][ sgLot -> Row];

    //StringGrid Clear
    for(int i =0 ; i < sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < sgLotInfo->RowCount ; j++ ) {
            sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    sgLotInfo -> ColCount         = 20  ;
    sgLotInfo -> DefaultColWidth  = 65  ;
    sgLotInfo -> DefaultRowHeight = 25  ;
    sgLotInfo -> ColWidths[0]     = 20  ;
//    sgLotInfo -> ColWidths[2]     = 60  ;
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

    sgLotInfo -> Col = 1 ;
    sgLotInfoClick(Sender);


}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgMgzNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath  = LOG_FOLDER + sgDate ->Cells[1][sgDate -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] ;
    UserFile.GridSearchFile(sPath , sgSlotNo  , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    sgSlotNoClick(Sender);    
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgSlotNoClick(TObject *Sender)
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

void __fastcall TFrmSPC::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false ;
    //Map Update
    SPC.SpcArray1.UpdateAray();

    tmUpdate->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::FormDestroy(TObject *Sender)
{
        tmUpdate->Enabled = false ;    
}
//---------------------------------------------------------------------------

