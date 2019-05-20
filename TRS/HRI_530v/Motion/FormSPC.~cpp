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
    sgLotMap->Canvas->Brush->Color = clStat[sgLotMap -> Cells[ACol][ARow].ToIntDef(csNone)] ;
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








