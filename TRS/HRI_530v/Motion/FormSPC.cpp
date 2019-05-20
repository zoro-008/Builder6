//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormSPC.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "UserFile.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
__fastcall TFrmSPC::TFrmSPC(TComponent* Owner)
    : TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::FormShow(TObject *Sender)
{
    TDateTime CurrDateTime = Now();
                                                
    edErrSttTime -> Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edErrEndTime -> Date = CurrDateTime.FormatString("yyyy-mm-dd");

    SPC.LOT.DispDateList(sgLotDate) ;
    sgLotDateClick(Sender) ;


    //LT.UpdateDate(sgDate);
    //UserFile.GridSearchFile(LOG_FOLDER , sgLotDayName , false);

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::btErrDataViewClick(TObject *Sender)
{
    SPC.ERR.DispErrData(edErrSttTime->Date , edErrEndTime->Date , sgErrData , !rgNoTime -> ItemIndex );
    //SPC.DispErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrCntViewClick(TObject *Sender)
{
    //SPC.DispErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}

//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotDateClick(TObject *Sender)
{
    AnsiString sDate = sgLotDate -> Cells[1][sgLotDate -> Row] ;
    SPC.LOT.DispLotList(sDate,sgLotInfo);
    sgLotInfoClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotInfoClick(TObject *Sender)
{
    AnsiString sDate    = sgLotDate -> Cells[1][sgLotDate -> Row] ;
    AnsiString sLotName = sgLotInfo -> Cells[1][sgLotInfo -> Row] ;

    SPC.LOT.DispMgzAndSlotList(sDate , sLotName , sgLotMgz , sgLotSlot);
}

//---------------------------------------------------------------------------


void __fastcall TFrmSPC::sgLotMgzClick(TObject *Sender)
{
    sgLotSlot -> Row = 0 ;
    sgLotSlotClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotSlotClick(TObject *Sender)
{
    AnsiString sDate    = sgLotDate -> Cells[1][sgLotDate -> Row] ;
    AnsiString sLotName = sgLotInfo -> Cells[1][sgLotInfo -> Row] ;
    int        iMgz     = sgLotMgz  -> Row ;
    int        iSlot    = sgLotSlot -> Row ;

    sgLotMap->Canvas->Lock();
    SPC.LOT.DispStripMap(sDate , sLotName , iMgz , iSlot , sgLotMap);
    sgLotMap->Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgLotMapDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    if(ARow == 0 || ACol == 0 ) return ;
    //sgLotMap->Canvas->Brush->Color = clStat[sgLotMap -> Cells[ACol][ARow].ToIntDef(csNone)] ;
    sgLotMap->Canvas->FillRect(Rect) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    //sgLotMap -> Invalidate() ;
    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::FormClose(TObject *Sender, TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------










/*
void __fastcall CSPC::SaveErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "ErrCount" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    DelPastLog(SPC_FOLDER);

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgErrCnt->RowCount; i++){
        for(int j=0; j<_sgErrCnt->ColCount; j++){
            sData += _sgErrCnt -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}
*/
void __fastcall TFrmSPC::BitBtn1Click(TObject *Sender)
{
    if(!sdCSV->Execute()) return ;
    UserFile.SaveGridToCsv(sgLotInfo , sdCSV->FileName ) ;
}
//---------------------------------------------------------------------------

