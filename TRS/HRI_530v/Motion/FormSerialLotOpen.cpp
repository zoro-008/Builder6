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
                                                                                                                                              

                                                                                                                                              
    //No use lot�ɼǽÿ��� �ʿ���. �ɼ� ����������.                                                                                           
    //if(LotNo == "")LotNo = Now().FormatString("yyyy-mm-dd_hh:nn:ss");                                                                       
                                                                                                                                              
  //  LDR._iMgzCnt = OM.DevOptn.iLotEndMgzCnt ;                                                                                               


    /*
    LDR.SetMgzCnt (OM.DevOptn.iLotEndMgzCnt);//(); // OM.DevOptn.iLotEndMgzCnt > _MgzCnt ���� �ʱ�ȭ �ؼ� �̷���.

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
        FM_MsgOk("ERR","�� ��⸮��Ʈ�� �������� ī��Ʈ�� �ٲܼ� �����ϴ�.");
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
        FM_MsgOk("Error" , "������ �ް����� ���� ������ ī��Ʈ�� �ٲܼ� �����ϴ�.");
        return;
    }

    // �� �κ� ������ �ʿ��մϴ�. ���� ������ ���� ���ɴϴ�. UU //
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


