//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmLotOpen.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SMInterfaceUnit.h"
#include "Loader.h"
#include "SLogUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormLotOpen *FormLotOpen;
//---------------------------------------------------------------------------
__fastcall TFormLotOpen::TFormLotOpen(TComponent* Owner)
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
void __fastcall TFormLotOpen::btLotOpenClick(TObject *Sender)
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

    LT.LotOpen(Lo tNo,Device,Operator) ;
    LDR.SetMgzCnt (0);

    /*
    LTQ.PushLotNo(edBarcode -> Text);
    AnsiString AddLotNo = LotNo;
    if(LotNo == "")LotNo = Now().FormatString("yyyy-mm-dd_hh:nn:ss");
    SPC.LOT.SetLotID(LotNo);

    OM.MstOptn.sCrntLotID = LotNo;
    OM.SaveMstOptn();
    */






















    Close();

}
//---------------------------------------------------------------------------
void __fastcall TFormLotOpen::FormShow(TObject *Sender)
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

    btAuto -> Visible = OM.MstOptn.bUseLotAuto ;
//    edLotId -> SetFocus() ;

//    edLotId -> SelectAll() ;
}
//--------------------------------------------------------------------------
void __fastcall TFormLotOpen::btCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormLotOpen::btAutoClick(TObject *Sender)
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


void __fastcall TFormLotOpen::tmLotNameTimer(TObject *Sender)
{
    for(int i=0 ; i<MAX_LOT_CNT ; i++ ){
        sgLotName -> Cells[1][i+1] = g_sLotNo[i];
    }
    edLotId -> Text = g_sLotNo[0] ;
//    if(sgLotName -> Cells[1][1] != "" && edLotId -> Text != sgLotName -> Cells[1][1]) edLotId -> Text = sgLotName -> Cells[1][1] ;
}
//---------------------------------------------------------------------------

void __fastcall TFormLotOpen::btAddClick(TObject *Sender)
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


void __fastcall TFormLotOpen::btLotNoDelClick(TObject *Sender)
{
    DeleteLotNoAll();
}
//---------------------------------------------------------------------------



void __fastcall TFormLotOpen::btLotAddDelClick(TObject *Sender)
{
    int i ;
    for(i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        sgLotAdd -> Cells[1][i] = "" ; 
    }
}
//---------------------------------------------------------------------------



void __fastcall TFormLotOpen::edBarcodeKeyPress(TObject *Sender, char &Key)
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


void __fastcall TFormLotOpen::Button1Click(TObject *Sender)
{
    PushTopLotNo(LT.GetCrntLot());
}
//---------------------------------------------------------------------------

