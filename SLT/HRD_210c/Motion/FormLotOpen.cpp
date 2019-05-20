//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLotOpen.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "SLogUnit.h"
#include "UserFile.h"
#include "UserINI.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmLotOpen::TFrmLotOpen(TComponent* Owner)
    : TForm(Owner)
{
    sgLotName  -> DoubleBuffered = true ;

    sgLotName -> RowCount = MAX_LOT_CNT + 1 ;
//    sgLotName -> ColWidths[0] = 20  ;
//    sgLotName -> ColWidths[1] = 200 ;

    for(int i = 1 ; i < MAX_LOT_CNT + 1 ; i++) {
        sgLotName -> Cells[0][i] = i ;
    }

}

//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btLotOpenClick(TObject *Sender)
{
    Trace("btLotOpen","btLotOpenClicked");
    AnsiString LotNo    = GetLotNo().Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;
    AnsiString Operator = edOperator  -> Text.Trim() ;

//    bool bStart = DM.ARAY[riLDR].GetCntStat(csNone) &&
//                  DM.ARAY[riPRB].GetCntStat(csNone) &&
//                  DM.ARAY[riWR1].GetCntStat(csNone) &&
//                  DM.ARAY[riWR2].GetCntStat(csNone) &&
//                  DM.ARAY[riWR3].GetCntStat(csNone) &&
//                  DM.ARAY[riPSB].GetCntStat(csNone) &&
//                  DM.ARAY[riULD].GetCntStat(csNone) && !LT.m_bLotOpen;


//    if(!bStart    ) {Close(); return;}
    if(LT.m_bLotOpen ) {Close(); return;}
    if(LotNo == ""   ) {Close(); return;}

    LT.LotOpen(LotNo,Device,Operator) ;
//    LDR.SetMgzCnt (0);
    Close();

}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(EXE_FOLDER + "JobFile\\" , sgDevice , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.m_sCrntDev) {
            sgDevice -> Row = i;
            edSelDevice -> Text = sgDevice->Cells[1][i];
        }
    }

    edMgzCnt  -> Text = OM.DevOptn.iLotEndMgzCnt ;
    edBarcode -> SetFocus();

    tmLotName -> Enabled = true ;
}
//--------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------



void __fastcall TFrmLotOpen::tmLotNameTimer(TObject *Sender)
{
    for(int i=0 ; i<MAX_LOT_CNT ; i++ ){
        sgLotName -> Cells[1][i+1] = g_sLotNo[i];
    }
    edLotId -> Text = g_sLotNo[0] ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::btAddClick(TObject *Sender)
{
    if(edBarcode->Text.Trim() == "" ) return ;
    PushLotNo(edBarcode->Text);
    edBarcode->Text = "";
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::btLotNoDelClick(TObject *Sender)
{
    DeleteLotNoOne();
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::edBarcodeKeyPress(TObject *Sender, char &Key)
{
    if(Key != VK_RETURN) return ;
    if(edBarcode->Text.Trim() == "" ) return ;
    PushLotNo(edBarcode->Text);
    edBarcode->Text = "";

}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::FormDestroy(TObject *Sender)
{
    tmLotName->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmLotName->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::btLotNoAllDelClick(TObject *Sender)
{
    DeleteLotNoAll();
}
//---------------------------------------------------------------------------


