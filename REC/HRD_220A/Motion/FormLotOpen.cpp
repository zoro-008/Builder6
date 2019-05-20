//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLotOpen.h"
#include "LotUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "ManualMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmLotOpen::TFrmLotOpen(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btLotOpenClick(TObject *Sender)
{
    AnsiString LotNo    = edLotId     -> Text.Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;

    //AnsiString Operator = edOperator  -> Text.Trim() ;

    //FM_MsgOk("Check","�δ� ��δ� ���� ");

    DM.ClearMap();
    if(IO_GetX(xLDR_FMgzDetect2)) {
        DM.ARAY[riLDR_FBtm].SetStat(csUnkwn);
        if(IO_GetX(xLDR_FMgzDetect1)) { DM.ARAY[riLDR_FTop].SetStat(csUnkwn); }
        else                          { DM.ARAY[riLDR_FTop].SetStat(csNone ); }
    }
    else                          { DM.ARAY[riLDR_FBtm].SetStat(csNone ); }
    if(IO_GetX(xLDR_RMgzDetect2)) {
        DM.ARAY[riLDR_RBtm].SetStat(csUnkwn);
        if(IO_GetX(xLDR_RMgzDetect1)) { DM.ARAY[riLDR_RTop].SetStat(csUnkwn); }
        else                          { DM.ARAY[riLDR_RTop].SetStat(csNone ); }
    }
    else                          { DM.ARAY[riLDR_RBtm].SetStat(csNone ); }
    if(IO_GetX(xULD_FMgzDetect2)) {
        DM.ARAY[riULD_FBtm].SetStat(csEmpty);
        if(IO_GetX(xULD_FMgzDetect1)) { DM.ARAY[riULD_FTop].SetStat(csEmpty); }
        else                          { DM.ARAY[riULD_FTop].SetStat(csNone ); }
    }
    else                          { DM.ARAY[riULD_FBtm].SetStat(csNone ); }
    if(IO_GetX(xULD_RMgzDetect2)) {
        DM.ARAY[riULD_RBtm].SetStat(csEmpty);
        if(IO_GetX(xULD_RMgzDetect1)) { DM.ARAY[riULD_RTop].SetStat(csEmpty); }
        else                          { DM.ARAY[riULD_RTop].SetStat(csNone ); }
    }
    else                          { DM.ARAY[riULD_RBtm].SetStat(csNone ); }



    //DM.ARAY[riLDR_FTop].SetStat(csUnkwn);
    //DM.ARAY[riLDR_FBtm].SetStat(csUnkwn);
    //DM.ARAY[riLDR_RTop].SetStat(csUnkwn);
    //DM.ARAY[riLDR_RBtm].SetStat(csUnkwn);
    //DM.ARAY[riULD_FTop].SetStat(csEmpty);
    //DM.ARAY[riULD_FBtm].SetStat(csEmpty);
    //DM.ARAY[riULD_RTop].SetStat(csEmpty);
    //DM.ARAY[riULD_RBtm].SetStat(csEmpty);


    if(DM.ARAY[riLDR_FBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","����Ʈ �δ��� �ް����� �����ϴ�."  ); return ;}
    if(DM.ARAY[riLDR_RBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","���� �δ��� �ް����� �����ϴ�."    ); return ;}
    if(DM.ARAY[riULD_FBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","����Ʈ ��δ��� �ް����� �����ϴ�."); return ;}
    if(DM.ARAY[riULD_RBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","���� ��δ��� �ް����� �����ϴ�."  ); return ;}

    if(!DM.ARAY[riALN].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","����̳ʿ� ���� ������ Ȯ�����ּ���.");
        DM.ARAY[riALN].SetStat(csNone);
    }


    if(IO_GetX(xPCK_Vccm)) {
        FM_MsgOkModal("Check","��Ŀ�� ������ �������ּ���.");
    }
    DM.ARAY[riALN].SetStat(csNone);
    if(IO_GetX(xSTG_Vccm)) {
        FM_MsgOkModal("Check","���������� ������ �������ּ���.");
    }
    DM.ARAY[riSTG].SetStat(csNone);

    //����Ʈ �ε���
    if(!DM.ARAY[riPRI_F].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","����Ʈ �����ε����� ��Ʈ ������ Ȯ�����ּ���.");
        DM.ARAY[riPRI_F].SetStat(csNone);
    }
    if(!DM.ARAY[riIDX_F].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","����Ʈ �ε����� ��Ʈ ������ Ȯ�����ּ���.");
        DM.ARAY[riIDX_F].SetStat(csNone);
    }
    if(!DM.ARAY[riPSI_F].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","����Ʈ ����Ʈ �ε����� ��Ʈ ������ Ȯ�����ּ���.");
        DM.ARAY[riPSI_F].SetStat(csNone);
    }

    //���� �ε���
    if(!DM.ARAY[riPRI_R].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","���� �����ε����� ��Ʈ ������ Ȯ�����ּ���.");
        DM.ARAY[riPRI_R].SetStat(csNone);
    }
    if(!DM.ARAY[riIDX_R].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","���� �ε����� ��Ʈ ������ Ȯ�����ּ���.");
        DM.ARAY[riIDX_R].SetStat(csNone);
    }
    if(!DM.ARAY[riPSI_R].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","���� ����Ʈ �ε����� ��Ʈ ������ Ȯ�����ּ���.");
        DM.ARAY[riPSI_R].SetStat(csNone);
    }


    //���������� ���̺��� �����͸����� ������ �̵�.
    SetDataMap();





    LOT.LotOpen(LotNo , Device) ;

    Trace("LotOpen" , "Try");

    Close();

    FM_MsgOkModal("������ ������ Ȯ���ϼ���" , OM.DevOptn.sEpoxyName.c_str());

    FM_MsgOkModal("Confirm" , "��δ��� ��Ʈ ������ Ȯ���ϼ���");

    FM_MsgOkModal("Confirm","��� �̴ϼ��� �մϴ�.");



    MM.SetManCycle(mcAllHome);

    DSP.m_bNiddleChecked = false; //�������ϸ� �ϵ� Z���� �����ؾ� �Ѵ�.

}

void TFrmLotOpen::SetDataMap()
{
    //Boat�ø��� Ȯ��.
    String sFrontChar = OM.DevInfo.iWorkMode == wmFos ? "F" : "D" ;
    String sRearChar  = OM.DevInfo.iWorkMode == wmFos ? "M" : "M" ;
    for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
        if(g_LotData.GetCellByColTitle("FrontBoatNo",i).Pos(sFrontChar)==0&&g_LotData.GetCellByColTitle("FrontBoatNo",i)!=""){
            FM_MsgOk("Error",("FrontBoatNo : " + String(i) + " ���� BoatNo��" + sFrontChar + "�� �ƴմϴ�.").c_str());
            return ;
        }
        if(g_LotData.GetCellByColTitle("RearBoatNo",i).Pos(sRearChar)==0 && g_LotData.GetCellByColTitle("RearBoatNo",i)!=""){
            FM_MsgOk("Error",("RearBoatNo : " + String(i) + " ���� BoatNo��" + sRearChar + "�� �ƴմϴ�.").c_str());
            return ;
        }
    }


    bool bDataExist = false ;
    String sBoatId ;
    //FrontLoader
    /*if(!DM.ARAY[riLDR_FBtm].CheckAllStat(csNone)){
        for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt ; r++){
            bDataExist = false ;
            sBoatId    = "" ;
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                if(g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i) == String(r+1)&&//���Գѹ���.
                   g_LotData.GetCellByColTitle("FrontBoatNo"        ,i) != ""         ){//��Ʈ ���̵� �־�� ��.
                    sBoatId = g_LotData.GetCellByColTitle("FrontBoatNo",i);
                    DM.ARAY[riLDR_FBtm].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID(sBoatId);
                    bDataExist = true ;
                    break ;
                }
            }
            if(!bDataExist){
                DM.ARAY[riLDR_FBtm].SetStat(OM.DevInfo.iMgzSlotCnt-r-1,0,csEmpty);
                DM.ARAY[riLDR_FBtm].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID("");
            }
        }
    }*/

    if(!DM.ARAY[riLDR_FBtm].CheckAllStat(csNone)){
        for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt ; r++){
            bDataExist = false ;
            sBoatId    = "" ;
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                if(g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i) == String(r+1)&&//���Գѹ���.
                   g_LotData.GetCellByColTitle("FrontBoatNo"        ,i) != ""         ){//��Ʈ ���̵� �־�� ��.
                    sBoatId = g_LotData.GetCellByColTitle("FrontBoatNo",i);
                    DM.ARAY[riLDR_FBtm].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID(sBoatId);
                    bDataExist = true ;
                    break ;
                }
            }
            if(!bDataExist){
                DM.ARAY[riLDR_FBtm].SetStat(OM.DevInfo.iMgzSlotCnt-r-1,0,csEmpty);
                DM.ARAY[riLDR_FBtm].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID("");
            }
        }
    }



    if(!DM.ARAY[riLDR_FTop].CheckAllStat(csNone)){
        for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt ; r++){
            bDataExist = false ;
            sBoatId    = "" ;
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                if(g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i) == String(r+OM.DevInfo.iMgzSlotCnt+1)&&
                   g_LotData.GetCellByColTitle("FrontBoatNo"        ,i) != ""                                ){
                    sBoatId = g_LotData.GetCellByColTitle("FrontBoatNo",i);
                    DM.ARAY[riLDR_FTop].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID(sBoatId);
                    bDataExist = true ;
                    break ;
                }
            }
            if(!bDataExist){
                DM.ARAY[riLDR_FTop].SetStat(OM.DevInfo.iMgzSlotCnt-r-1,0,csEmpty);
                DM.ARAY[riLDR_FTop].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID("");
            }
        }
    }

    //Rear Loader .
    if(!DM.ARAY[riLDR_RBtm].CheckAllStat(csNone)){
        for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt ; r++){
            bDataExist = false ;
            sBoatId    = "" ;
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                if(g_LotData.GetCellByColTitle("RearMagazineSlotNo",i) == String(r+1) &&
                   g_LotData.GetCellByColTitle("RearBoatNo"        ,i) != ""          ){
                    sBoatId = g_LotData.GetCellByColTitle("RearBoatNo",i);
                    DM.ARAY[riLDR_RBtm].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID(sBoatId);
                    bDataExist = true ;
                    break ;
                }
            }
            if(!bDataExist){
                DM.ARAY[riLDR_RBtm].SetStat(OM.DevInfo.iMgzSlotCnt-r-1,0,csEmpty);
                DM.ARAY[riLDR_RBtm].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID("");
            }
        }
    }

    if(!DM.ARAY[riLDR_RTop].CheckAllStat(csNone)){
        for(int r = 0 ; r < OM.DevInfo.iMgzSlotCnt ; r++){
            bDataExist = false ;
            sBoatId    = "" ;
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                if(g_LotData.GetCellByColTitle("RearMagazineSlotNo",i) == String(r+OM.DevInfo.iMgzSlotCnt+1)&&
                   g_LotData.GetCellByColTitle("RearBoatNo"        ,i) != ""          ){
                    sBoatId = g_LotData.GetCellByColTitle("RearBoatNo",i);
                    DM.ARAY[riLDR_RTop].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID(sBoatId);
                    bDataExist = true ;
                    break ;
                }
            }
            if(!bDataExist){
                DM.ARAY[riLDR_RTop].SetStat(OM.DevInfo.iMgzSlotCnt-r-1,0,csEmpty);
                DM.ARAY[riLDR_RTop].CHPS[OM.DevInfo.iMgzSlotCnt-r-1][0].SetID("");
            }
        }
    }
    //

}



//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    edSelDevice -> Text = OM.GetCrntDev();


    edLotId -> SetFocus();

    g_LotData.LoadFromCsv(OM.CmnOptn.sLotFilePath);
    sgData -> ColCount = g_LotData.GetMaxCol() ;
    sgData -> RowCount = g_LotData.GetMaxRow() ;
    sgData -> ColWidths[0] = 30 ;
    for(int c = 0 ; c < sgData -> ColCount ; c++){
        for(int r = 0 ; r < sgData -> RowCount ; r++){
            sgData -> Cells[c][r] = g_LotData.GetCell(c,r);
        }
    }

    TDateTime Time ;

    edLotId        -> Text = Time.CurrentDateTime().FormatString("hh:mm:ss");
    edLotFilePath  -> Text = OM.CmnOptn.sLotFilePath ;

}
//--------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btCancelClick(TObject *Sender)
{
    Trace("LotOpen","Canceled") ;
    Close();
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::tmLotNameTimer(TObject *Sender)
{
    tmLotName -> Enabled = false ;




    tmLotName -> Enabled = true ;
}
//---------------------------------------------------------------------------







void __fastcall TFrmLotOpen::btLotFileClick(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edLotFilePath->Text = dgFilePath->FileName;
    OM.CmnOptn.sLotFilePath = dgFilePath->FileName;
    OM.SaveCmnOptn();

    g_LotData.LoadFromCsv(OM.CmnOptn.sLotFilePath);
    sgData -> ColCount = g_LotData.GetMaxCol() ;
    sgData -> RowCount = g_LotData.GetMaxRow() ;
    sgData -> ColWidths[0] = 30 ;
    for(int c = 0 ; c < sgData -> ColCount ; c++){
        for(int r = 0 ; r < sgData -> RowCount ; r++){
            sgData -> Cells[c][r] = g_LotData.GetCell(c,r);
        }
    }
}
//---------------------------------------------------------------------------




//Test���̴�.
void __fastcall TFrmLotOpen::Label19Click(TObject *Sender)
{
    return ;

    DM.ClearMap();
    //DM.ARAY[riLDR_FBtm].SetStat(csUnkwn);  DM.ARAY[riLDR_FTop].SetStat(csNone );
    //DM.ARAY[riLDR_RBtm].SetStat(csUnkwn);  DM.ARAY[riLDR_RBtm].SetStat(csNone );
    //DM.ARAY[riULD_FBtm].SetStat(csEmpty);  DM.ARAY[riULD_FBtm].SetStat(csNone );
    //DM.ARAY[riULD_RBtm].SetStat(csEmpty);  DM.ARAY[riULD_RBtm].SetStat(csNone );


    DM.ARAY[riLDR_FTop].SetStat(csUnkwn);
    DM.ARAY[riLDR_FBtm].SetStat(csUnkwn);
    DM.ARAY[riLDR_RTop].SetStat(csUnkwn);
    DM.ARAY[riLDR_RBtm].SetStat(csUnkwn);
    DM.ARAY[riULD_FTop].SetStat(csEmpty);
    DM.ARAY[riULD_FBtm].SetStat(csEmpty);
    DM.ARAY[riULD_RTop].SetStat(csEmpty);
    DM.ARAY[riULD_RBtm].SetStat(csEmpty);







    AnsiString LotNo    = edLotId     -> Text.Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;

    //���������� ���̺��� �����͸����� ������ �̵�.
    SetDataMap();

    LOT.LotOpen(LotNo , Device) ;

    Trace("LotOpen" , "Try");

    Close();

    FM_MsgOkModal("������ ������ Ȯ���ϼ���" , OM.DevOptn.sEpoxyName.c_str());

    FM_MsgOkModal("Confirm","��� �̴ϼ��� �մϴ�.");



    MM.SetManCycle(mcAllHome);

    DSP.m_bNiddleChecked = false; //�������ϸ� �ϵ� Z���� �����ؾ� �Ѵ�.

}
//---------------------------------------------------------------------------

