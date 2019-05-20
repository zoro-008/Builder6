//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSerialLotOpen.h"
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
TFrmSerialLotOpen *FrmSerialLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmSerialLotOpen::TFrmSerialLotOpen(TComponent* Owner)
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
void __fastcall TFrmSerialLotOpen::btLotOpenClick(TObject *Sender)
{
    AnsiString LotNo    = LTQ.GetLotNo(0).Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;
    //AnsiString Operator = edOperator  -> Text.Trim() ;

    LTQ.SetAutoLotNo(edAutoLot -> Text);

    LTQ.LotOpen() ;

    Trace("LotOpen" , "Try");

    bool bStart = DM.ARAY[riSRT].CheckAllStat(csNone) && DM.ARAY[riZIG].CheckAllStat(csNone) && DM.ARAY[riSG1].CheckAllStat(csNone) &&
                  DM.ARAY[riSG2].CheckAllStat(csNone) && DM.ARAY[riSG3].CheckAllStat(csNone) && DM.ARAY[riSG4].CheckAllStat(csNone) &&
                  DM.ARAY[riRJ1].CheckAllStat(csNone) && DM.ARAY[riRJ2].CheckAllStat(csNone) && DM.ARAY[riRJ3].CheckAllStat(csNone) &&
                  DM.ARAY[riRJ4].CheckAllStat(csNone) && DM.ARAY[riPRL].CheckAllStat(csNone) && DM.ARAY[riLDR].CheckAllStat(csNone) &&
                  DM.ARAY[riPSL].CheckAllStat(csNone) && DM.ARAY[riPRU].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) &&
                  DM.ARAY[riPSU].CheckAllStat(csNone)   ;

                                                                                                                                              
                                                                                                                                              
                                                                                                                                              
                                                                                                                                              
    if(!bStart) {Close(); return ;}                                                                                                           
                                                                                                                                              

                                                                                                                                              
    //No use lot옵션시에만 필요함. 옵션 지워버렸음.                                                                                           
    //if(LotNo == "")LotNo = Now().FormatString("yyyy-mm-dd_hh:nn:ss");                                                                       
                                                                                                                                              
  //  LDR._iMgzCnt = OM.DevOptn.iLotEndMgzCnt ;                                                                                               


    /*
    LDR.SetMgzCnt (OM.DevOptn.iLotEndMgzCnt);//(); // OM.DevOptn.iLotEndMgzCnt > _MgzCnt 여야 초기화 해서 이렇다.

    AnsiString sTemp ;
    sTemp = "MGZ Cnt :" + AnsiString(OM.DevOptn.iLotEndMgzCnt);
    Trace("MGZ Cnt" , sTemp.c_str());

    for(int i = 1 ; i < sgLotName -> RowCount ; i++) {
        if(sgLotName -> Cells[1][i].Trim() != "") {
            Trace((AnsiString("LotCnt")+i).c_str(), sgLotName -> Cells[1][i].c_str()) ;
        }
    }
    */


    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmSerialLotOpen::FormShow(TObject *Sender)
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




    //edMgzCnt  -> Text = OM.DevOptn.iLotEndMgzCnt ;
    edBarcode -> SetFocus();

}
//--------------------------------------------------------------------------
void __fastcall TFrmSerialLotOpen::btCancelClick(TObject *Sender)
{
    LTQ.SetAutoLotNo(edAutoLot -> Text);

    Trace("LotOpen","Canceled") ;

    for(int i = 1 ; i < sgLotName -> RowCount ; i++) {
        if(sgLotName -> Cells[1][i].Trim() != "") {
            Trace((AnsiString("LotCnt")+i).c_str(), sgLotName -> Cells[1][i].c_str()) ;
        }
    }

    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmSerialLotOpen::btAutoClick(TObject *Sender)
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



void __fastcall TFrmSerialLotOpen::btAddClick(TObject *Sender)
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


void __fastcall TFrmSerialLotOpen::btLotNoDelClick(TObject *Sender)
{
    LTQ.DeleteLotNoAll();

    for(int i = 1 ; i < sgLotName -> RowCount ; i++) {
        sgLotName -> Cells[1][i] = "" ;
    }

    Trace("LotOpen","Delete LotQue");
}
//---------------------------------------------------------------------------



void __fastcall TFrmSerialLotOpen::btLotAddDelClick(TObject *Sender)
{
    for(int i = 1 ; i < sgLotAdd -> RowCount ; i++) {
        sgLotAdd -> Cells[1][i] = "" ;
    }
    Trace("LotOpen","Delete LotAdd");
}
//---------------------------------------------------------------------------
void __fastcall TFrmSerialLotOpen::edBarcodeKeyPress(TObject *Sender, char &Key)
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


void TFrmSerialLotOpen::DispLotNo()
{
    for(int i=0 ; i<sgLotName -> RowCount - 1 ; i++ ){
        sgLotName -> Cells[1][i+1] = LTQ.GetLotNo(i) ;
    }
    edLotId -> Text = LTQ.GetLotNo(0) ;
}

void __fastcall TFrmSerialLotOpen::btSaveClick(TObject *Sender)
{


    if(LTQ.GetLotCnt() ){
        FM_MsgOk("ERR","랏 대기리스트가 있을때는 카운트를 바꿀수 없습니다.");
        return ;
    }

    bool bAllArayNone = DM.ARAY[riSRT].CheckAllStat(csNone) && DM.ARAY[riZIG].CheckAllStat(csNone) && DM.ARAY[riSG1].CheckAllStat(csNone) &&
                        DM.ARAY[riSG2].CheckAllStat(csNone) && DM.ARAY[riSG3].CheckAllStat(csNone) && DM.ARAY[riSG4].CheckAllStat(csNone) &&
                        DM.ARAY[riRJ1].CheckAllStat(csNone) && DM.ARAY[riRJ2].CheckAllStat(csNone) && DM.ARAY[riRJ3].CheckAllStat(csNone) &&
                        DM.ARAY[riRJ4].CheckAllStat(csNone) && DM.ARAY[riPRL].CheckAllStat(csNone) && DM.ARAY[riLDR].CheckAllStat(csNone) &&
                        DM.ARAY[riPSL].CheckAllStat(csNone) && DM.ARAY[riPRU].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) &&
                        DM.ARAY[riPSU].CheckAllStat(csNone) ;

    //if(LTQ.GetLotOpen()) {
    if(!bAllArayNone){
        FM_MsgOk("Error" , "자제나 메거진이 남아 있으면 카운트를 바꿀수 없습니다.");
        return;
    }

    // 이 부분 수정이 필요합니다. 현재 컴파일 에러 나옵니다. UU //
    /*
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes) return;


    const int iPreMgzCnt = OM.DevOptn.iLotEndMgzCnt ;

    OM.DevOptn.iLotEndMgzCnt = edMgzCnt-> Text.ToIntDef(OM.DevOptn.iLotEndMgzCnt) ;

    edMgzCnt-> Text = OM.DevOptn.iLotEndMgzCnt ;

    OM.SaveDevOptn(OM.GetCrntDev());

    AnsiString sTemp ;
    sTemp = "MGZ Cnt Change :" + AnsiString(iPreMgzCnt) + "->" + AnsiString(OM.DevOptn.iLotEndMgzCnt);
    Trace("MGZ Cnt Change" , sTemp.c_str());

    LDR.SetMgzCnt(OM.DevOptn.iLotEndMgzCnt);
    */
}
//---------------------------------------------------------------------------

void __fastcall TFrmSerialLotOpen::tmLotNameTimer(TObject *Sender)
{
    tmLotName -> Enabled = false ;

    


    tmLotName -> Enabled = true ;
}
//---------------------------------------------------------------------------


