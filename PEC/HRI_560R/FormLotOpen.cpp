//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLotOpen.h"
#include "LotUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SMInterfaceUnit.h"
#include "Loader.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmLotOpen::TFrmLotOpen(TComponent* Owner)
    : TForm(Owner)
{
    sgLotName -> DoubleBuffered = true ;
    sgLotAdd  -> DoubleBuffered = true ;

    sgLotName -> RowCount = MAX_LOT_CNT + 1 ;
    sgLotName -> ColWidths[0] = 20  ;
    sgLotName -> ColWidths[1] = 200 ;

    for(int i = 1 ; i < MAX_LOT_CNT + 1 ; i++) {
        sgLotName -> Cells[0][i] = i ;
    }

    sgLotName -> Cells[1][0] = "Lot No" ;

    sgLotAdd -> RowCount = MAX_LOT_CNT + 1 ;//- LT. ;
    sgLotAdd -> ColWidths[0] = 20  ;
    sgLotAdd -> ColWidths[1] = 200 ;

    for(int i = 1 ; i < MAX_LOT_CNT + 1 ; i++) {
        sgLotAdd -> Cells[0][i] = i ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btLotOpenClick(TObject *Sender)
{
    Trace("btLotOpen","btLotOpenClicked");
    AnsiString LotNo    = GetLotNo().Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;
    AnsiString Operator = edOperator  -> Text.Trim() ;

    if(LT.m_bLotOpen ) {Close(); return;}
    if(LotNo == ""   ) {Close(); return;}

    LT.LotOpen(LotNo,Device,Operator) ;
    LDR.SetMgzCnt (0);
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile\\" , sgDevice , 1 , true);

    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.m_sCrntDev) {
            sgDevice -> Row = i;
            edSelDevice -> Text = sgDevice->Cells[1][i];
        }
    }

    edMgzCnt  -> Text = OM.DevOptn.iLotEndMgzCnt ;
    edBarcode -> SetFocus();
}
//--------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::btAutoClick(TObject *Sender)
{
    int i ;
    for(i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        if(sgLotAdd -> Cells[1][i] == "") break ;
    }

    for(int j = 0 ; j < 10 ; j++) {
        if(i+j >= MAX_LOT_CNT+1) return ;
        sgLotAdd -> Cells[1][i+j] = j ;

    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::tmLotNameTimer(TObject *Sender)
{
    for(int i=0 ; i<MAX_LOT_CNT ; i++ ){
        sgLotName -> Cells[1][i+1] = g_sLotNo[i];
    }
    edLotId -> Text = g_sLotNo[0] ;
//    if(sgLotName -> Cells[1][1] != "" && edLotId -> Text != sgLotName -> Cells[1][1]) edLotId -> Text = sgLotName -> Cells[1][1] ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::btAddClick(TObject *Sender)
{
    for(int i = 1 ; i < sgLotAdd -> RowCount + 1 ; i++) {
        if(sgLotAdd -> Cells[1][i].Trim() != "") {
            PushLotNo(sgLotAdd -> Cells[1][i]) ;
//            Memo1->Lines->Add(sgLotAdd -> Cells[1][i].Trim());
        }
    }
    for(int j = 1 ; j < sgLotAdd -> RowCount - 1 ; j++) {
        sgLotAdd ->Cells[1][j] = ""; //sgLotAdd -> Cells[1][j+1] ;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::btLotNoDelClick(TObject *Sender)
{
    DeleteLotNoAll();
}
//---------------------------------------------------------------------------



void __fastcall TFrmLotOpen::btLotAddDelClick(TObject *Sender)
{
    int i ;
    for(i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        sgLotAdd -> Cells[1][i] = "" ; 
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::edBarcodeKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_RETURN){
        AnsiString sLotId;
        sLotId = ((TEdit*)Sender)->Text;
        int i ;
        for(i = 1 ; i < sgLotAdd->RowCount ; i++) {
            if(sgLotAdd -> Cells[1][i] == "") break ;
        }
        sgLotAdd->Cells[1][i] = sLotId;
        ((TEdit*)Sender)->Text = "";
    }
}
//---------------------------------------------------------------------------


