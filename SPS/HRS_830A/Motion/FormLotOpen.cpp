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

    //No use lot�ɼǽÿ��� �ʿ���. �ɼ� ����������.
    //if(LotNo == "")LotNo = Now().FormatString("yyyy-mm-dd_hh:nn:ss");

    LDR._iMgzCnt = OM.DevOptn.iLotEndMgzCnt ;

    for(int i = 1 ; i < sgLotName -> RowCount ; i++) {
        if(sgLotName -> Cells[1][i] != "") LTQ.LotOpen() ;
    }

    LDR.SetMgzCnt (OM.DevOptn.iLotEndMgzCnt);//(); // OM.DevOptn.iLotEndMgzCnt > _MgzCnt ���� �ʱ�ȭ �ؼ� �̷���.

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    //StringGrid�� Update�Ѵ�.
    UserFile.GridSearchDir(ExtractFilePath(Application -> ExeName) + "JobFile\\" , sgDevice , 1 , true);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����

    //������ â.
    for(int i = 0 ; i < sgDevice -> RowCount ; i++) {
        if (sgDevice->Cells[1][i] == OM.GetCrntDev()) {
            sgDevice -> Row = i;

        }
    }

    edSelDevice -> Text = OM.GetCrntDev();
    edAutoLot   -> Text = LTQ.GetAutoLotNo() ;


    //LotNo ť â.
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
    int i ; //�׳� ���ڸ� �󺧸� �ؼ� �ִ� ��.
    for(i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        if(sgLotAdd -> Cells[1][i] == "") break ;
    }

    //�� �߰��ÿ� 10����
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
        //���ڵ� �ڸ��� �ɼ�.
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
            FM_MsgOk("ERR","�� �����߿��� �ް���ī��Ʈ�� �ٲܼ� �����ϴ�.");
        }
    }
    else {
        LDR._iMgzCnt = edMgzCnt-> Text.ToIntDef(1) ; //���Ҷ� �̷��� �Ǿ�� Lot�� ť���� �����´�.
    }

    if(mrYes==1){ edMgzCnt -> Text = OM.DevOptn.iLotEndMgzCnt                  ;}    // YES�� �Է��� ���, �ڽ� �ȿ� �ִ� ���� ����
    else        { OM.DevOptn.iLotEndMgzCnt = StrToIntDef(edMgzCnt -> Text , 3) ;}    // NO�� �Է��� ���, ���� �ڽ� �ȿ� �ִ� ���� �������� �ʱ�

    OM.SaveDevOptn(OM.GetCrntDev());
}
//---------------------------------------------------------------------------

