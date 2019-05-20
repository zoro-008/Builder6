//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLotOpen.h"
#include "LotUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SPCUnit.h"
#include "SMInterfaceUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmLotOpen::TFrmLotOpen(TComponent* Owner)
    : TForm(Owner)
{
    //sgLotName -> DoubleBuffered = true ;
    //sgLotAdd  -> DoubleBuffered = true ;

    //sgLotName -> RowCount = MAX_LOT_CNT + 1 ;
    //sgLotName -> ColWidths[0] = 20  ;
    //sgLotName -> ColWidths[1] = 200 ;

    //for(int i = 1 ; i < MAX_LOT_CNT + 1 ; i++) {
    //    sgLotName -> Cells[0][i] = i ;
    //}

    //sgLotName -> Cells[1][0] = "Lot No" ;

    //sgLotAdd -> RowCount = MAX_LOT_CNT + 1 ;//- LT. ;
    //sgLotAdd -> ColWidths[0] = 20  ;
    //sgLotAdd -> ColWidths[1] = 200 ;

    //for(int i = 1 ; i < MAX_LOT_CNT + 1 ; i++) {
    //    sgLotAdd -> Cells[0][i] = i ;
    //}

}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btLotOpenClick(TObject *Sender)
{
    LTQ.PushLotNo(edBarcode -> Text);
    AnsiString LotNo    = LTQ.GetLotNo(0).Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;
    AnsiString Operator = edOperator  -> Text.Trim() ;
    AnsiString AddLotNo = LotNo;

    bool bStart = DM.ARAY[riFFD].GetCntStat(csNone) &&
                  DM.ARAY[riLFD].GetCntStat(csNone) &&
                  DM.ARAY[riFLS].GetCntStat(csNone) &&
                  DM.ARAY[riLLS].GetCntStat(csNone) &&LTQ.GetLotOpen();


    if(!bStart) {/*Close();*/ ModalResult = mrOk ; }

    if(LotNo == "")LotNo = Now().FormatString("yyyy-mm-dd_hh:nn:ss");

    SPC.LOT.SetLotID(LotNo);

    LTQ.LotOpen(LotNo,Device,Operator) ;


    MM.SetManCycle(mcAllHome);

    OM.MstOptn.sCrntLotID = LotNo;
    OM.SaveMstOptn();


    //if(bStart)LDR.SetMgzCnt (OM.DevOptn.iLotEndMgzCnt); // OM.DevOptn.iLotEndMgzCnt > _MgzCnt 여야 초기화 해서 이렇다.

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile\\" , sgDevice , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    //잡파일 창.
    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.m_sCrntDev) {
            sgDevice -> Row = i;

        }
    }

    edSelDevice -> Text = OM.m_sCrntDev;

    //LotNo 큐 창.
    DispLotNo();


    //edMgzCnt  -> Text = OM.DevOptn.iLotEndMgzCnt ;
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
    //int i ; //그냥 숫자만 라벨링 해서 넣는 것.
    //for(i = 1 ; i < sgLotAdd -> RowCount ; i++) {
    //    if(sgLotAdd -> Cells[1][i] == "") break ;
    //}

    ////랏 추가시에 10개씩
    //for(int j = 1 ; j <= 10 ; j++) {
    //    if(i+j > MAX_LOT_CNT-1) return ;
    //    sgLotAdd -> Cells[1][i+j] = AUTO_LOT + AnsiString(j) ;
    //
    //}
}
//---------------------------------------------------------------------------



void __fastcall TFrmLotOpen::btAddClick(TObject *Sender)
{
    //for(int i = 1 ; i < sgLotAdd -> RowCount ; i++) {
    //    if(sgLotAdd -> Cells[1][i].Trim() != "") {
    //        LTQ.PushLotNo(sgLotAdd -> Cells[1][i]) ;
    //    }
    //    sgLotAdd ->Cells[1][i] = "";
    //}
    //DispLotNo();
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::btLotNoDelClick(TObject *Sender)
{
    LTQ.DeleteLotNoAll();
}
//---------------------------------------------------------------------------



void __fastcall TFrmLotOpen::btLotAddDelClick(TObject *Sender)
{
    //for(int i = 1 ; i < sgLotAdd -> RowCount ; i++) {
    //    sgLotAdd -> Cells[1][i] = "" ;
    //}
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::edBarcodeKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_RETURN){
        AnsiString sLotId;
        sLotId = ((TEdit*)Sender)->Text;
        //바코드 자릿수 옵션.
        //if(sLotId.Trim().Length() != OM.MstOptn.iBarcodeNum) {
        //    ((TEdit*)Sender)->Text = "";
        //    return ;
        //}

        //int i ;
        //for(i = 1 ; i < sgLotAdd->RowCount ; i++) {
        //    if(sgLotAdd -> Cells[1][i] == "") break ;
        //}
        //sgLotAdd->Cells[1][i] = sLotId;

        SPC.LOT.SetLotID(sLotId); 

        ((TEdit*)Sender)->Text = "";
    }
}
//---------------------------------------------------------------------------


void TFrmLotOpen::DispLotNo()
{
    //for(int i=0 ; i<sgLotName -> RowCount - 1 ; i++ ){
    //    sgLotName -> Cells[1][i+1] = LTQ.GetLotNo(i) ;
    //}
    edLotId -> Text = LTQ.GetLotNo(0) ;
}

void __fastcall TFrmLotOpen::tmLotNameTimer(TObject *Sender)
{
    tmLotName -> Enabled = false;
    if(LTQ.GetLotOpen()){ btLotOpen -> Enabled = false; }
    else                { btLotOpen -> Enabled = true ; }
    tmLotName -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::FormCreate(TObject *Sender)
{
    tmLotName -> Enabled = true;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::FormDestroy(TObject *Sender)
{
    tmLotName -> Enabled = false;
}
//---------------------------------------------------------------------------

