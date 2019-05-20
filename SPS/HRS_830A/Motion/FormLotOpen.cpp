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
    AnsiString LotNo    = LTQ.GetLotNo(0).Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;
    //AnsiString Operator = edOperator  -> Text.Trim() ;

    bool bStart = DM.ARAY[riLDR].GetCntStat(csNone) &&
                  DM.ARAY[riPRB].GetCntStat(csNone) &&
                  DM.ARAY[riLWK].GetCntStat(csNone) &&
                  DM.ARAY[riRWK].GetCntStat(csNone) &&
                  DM.ARAY[riPSB].GetCntStat(csNone) &&
                  DM.ARAY[riULD].GetCntStat(csNone) &&LTQ.GetLotOpen();


    if(!bStart) {Close(); }

    //No use lot옵션시에만 필요함. 옵션 지워버렸음.
    //if(LotNo == "")LotNo = Now().FormatString("yyyy-mm-dd_hh:nn:ss");

    LDR._iMgzCnt = OM.DevOptn.iLotEndMgzCnt ;

    for(int i = 1 ; i < sgLotName -> RowCount ; i++) {
        if(sgLotName -> Cells[1][i] != "") LTQ.LotOpen() ;
    }

    LDR.SetMgzCnt (OM.DevOptn.iLotEndMgzCnt);//(); // OM.DevOptn.iLotEndMgzCnt > _MgzCnt 여야 초기화 해서 이렇다.

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    //StringGrid를 Update한다.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile\\" , sgDevice , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    //잡파일 창.
    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.GetCrntDev()) {
            sgDevice -> Row = i;

        }
    }

    edSelDevice -> Text = OM.GetCrntDev();
    edAutoLot   -> Text = LTQ.GetAutoLotNo() ;


    //LotNo 큐 창.
    DispLotNo();




    edMgzCnt  -> Text = OM.DevOptn.iLotEndMgzCnt ;
    edBarcode -> SetFocus();

}
//--------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btCancelClick(TObject *Sender)
{
    LTQ.SetAutoLotNo(edAutoLot -> Text);
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmLotOpen::btAutoClick(TObject *Sender)
{
    int i ; //그냥 숫자만 라벨링 해서 넣는 것.
    for(i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        if(sgLotAdd -> Cells[1][i] == "") break ;
    }

    //랏 추가시에 10개씩
    for(int j = 0 ; j < 10 ; j++) {
        if(i+j > MAX_LOT_CNT-1) return ;
        sgLotAdd -> Cells[1][i+j] = edAutoLot -> Text + "<time>" ;
    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmLotOpen::btAddClick(TObject *Sender)
{
    for(int i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        if(sgLotAdd -> Cells[1][i].Trim() != "") {
            LTQ.PushLotNo(sgLotAdd -> Cells[1][i]) ;
        }
        sgLotAdd ->Cells[1][i] = "";
    }
    DispLotNo();
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::btLotNoDelClick(TObject *Sender)
{
    LTQ.DeleteLotNoAll();

    for(int i = 1 ; i < sgLotName -> RowCount ; i++) {
        sgLotName -> Cells[1][i] = "" ;
    }

}
//---------------------------------------------------------------------------



void __fastcall TFrmLotOpen::btLotAddDelClick(TObject *Sender)
{
    for(int i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        sgLotAdd -> Cells[1][i] = "" ;
    }
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

        int i ;
        for(i = 1 ; i < sgLotAdd->RowCount ; i++) {
            if(sgLotAdd -> Cells[1][i] == "") break ;
        }
        sgLotAdd->Cells[1][i] = sLotId;
        ((TEdit*)Sender)->Text = "";
    }
}
//---------------------------------------------------------------------------


void TFrmLotOpen::DispLotNo()
{
    for(int i=0 ; i<sgLotName -> RowCount - 1 ; i++ ){
        sgLotName -> Cells[1][i+1] = LTQ.GetLotNo(i) ;
    }
    edLotId -> Text = LTQ.GetLotNo(0) ;
}

void __fastcall TFrmLotOpen::btSaveClick(TObject *Sender)
{
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes) return;

    if(LTQ.GetLotOpen()){
        if(StrToIntDef(edMgzCnt-> Text , 1) != OM.DevOptn.iLotEndMgzCnt ) {
            edMgzCnt-> Text = OM.DevOptn.iLotEndMgzCnt ;
            FM_MsgOk("ERR","랏 오픈중에는 메거진카운트를 바꿀수 없습니다.");
        }
    }
    else {
        LDR._iMgzCnt = edMgzCnt-> Text.ToIntDef(1) ; //픽할때 이렇게 되어야 Lot을 큐에서 꺼내온다.
    }

    if(mrYes==1){ edMgzCnt -> Text = OM.DevOptn.iLotEndMgzCnt                  ;}    // YES를 입력할 경우, 박스 안에 있는 값을 저장
    else        { OM.DevOptn.iLotEndMgzCnt = StrToIntDef(edMgzCnt -> Text , 3) ;}    // NO를 입력할 경우, 현재 박스 안에 있는 값을 저장하지 않기

    OM.SaveDevOptn(OM.GetCrntDev());
}
//---------------------------------------------------------------------------

