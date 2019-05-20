//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLotOpen.h"

#include "LotUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "VisnComUnit.h"
#include "SMInterfaceUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmLotOpen::TFrmLotOpen(TComponent* Owner)
        : TForm(Owner)
{
    sgLotName -> RowCount = MAX_LOT_CNT + 1 ;
    sgLotName -> ColWidths[0] = 20  ;
    sgLotName -> ColWidths[1] = 200 ;

    for(int i = 1 ; i < MAX_LOT_CNT + 1 ; i++) {
        sgLotName -> Cells[0][i] = i ;
    }

    sgLotName -> Cells[1][0] = "Lot No" ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::BitBtn11Click(TObject *Sender)
{
    AnsiString LotNo[MAX_LOT_CNT];
    for( int i = 1 ; i < MAX_LOT_CNT ; i++) LotNo[i-1] = sgLotName -> Cells[1][i]   ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;
    AnsiString Operator = edOperator  -> Text.Trim() ;

    TDateTime  tLotStartTime ;

    tLotStartTime = Now();
    if(LotNo[0] == "")LotNo[0] = tLotStartTime.FormatString("yyyy-mm-dd hh-nn-ss");

    if(!IO_GetX(xHED_1Ready) || !IO_GetX(xHED_1Ready)) {
        FM_MsgOk("ERROR","Vision is not Ready!") ;
        return ;
    }

    LT.LotOpen(LotNo[0],Device,Operator) ;

    OM.LoadJobFile (Device);
    DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWK1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riWK2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riVS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ClearMap    (      );
    OM.SaveLastInfo(      );
    PM.Load        (Device);


    //VC.SendModelChange();


    if(!OM.CmnOptn.bVisnSkip) {
        VC.SendReset(); //
        VC.SendLotStart();
    }

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile\\" , sgDevice , 0 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.m_sCrntDev) {
            sgDevice -> Row = i;
            edSelDevice -> Text = sgDevice->Cells[1][i];
        }
    }

    edLotId -> SetFocus() ;

    edLotId -> SelectAll() ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::BitBtn12Click(TObject *Sender)
{
    Close();        
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btAutoClick(TObject *Sender)
{
    int i ;
    for(i = 1 ; i < MAX_LOT_CNT ; i++) {
        if(sgLotName -> Cells[1][i] == "") break ;
    }

    for(int j = 0 ; j < 10 ; j++) {
        sgLotName -> Cells[1][i] = j ;

    }


}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::tmLotNameTimer(TObject *Sender)
{
    if(sgLotName -> Cells[1][1] != "" && edLotId -> Text != sgLotName -> Cells[1][1]) edLotId -> Text = sgLotName -> Cells[1][1] ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::btClearClick(TObject *Sender)
{
    for(int i = 1 ; i < MAX_LOT_CNT + 1; i++)
    sgLotName -> Cells[1][i] = "" ;
}
//---------------------------------------------------------------------------

