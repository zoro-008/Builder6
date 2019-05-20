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

    //FM_MsgOk("Check","로더 언로더 외의 ");

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


    if(DM.ARAY[riLDR_FBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","프론트 로더에 메거진이 없습니다."  ); return ;}
    if(DM.ARAY[riLDR_RBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","리어 로더에 메거진이 없습니다."    ); return ;}
    if(DM.ARAY[riULD_FBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","프론트 언로더에 메거진이 없습니다."); return ;}
    if(DM.ARAY[riULD_RBtm].CheckAllStat(csNone)) {FM_MsgOk("Error","리어 언로더에 메거진이 없습니다."  ); return ;}

    if(!DM.ARAY[riALN].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","얼라이너에 자제 유무를 확인해주세요.");
        DM.ARAY[riALN].SetStat(csNone);
    }


    if(IO_GetX(xPCK_Vccm)) {
        FM_MsgOkModal("Check","픽커에 자제를 제거해주세요.");
    }
    DM.ARAY[riALN].SetStat(csNone);
    if(IO_GetX(xSTG_Vccm)) {
        FM_MsgOkModal("Check","스테이지에 자제를 제거해주세요.");
    }
    DM.ARAY[riSTG].SetStat(csNone);

    //프론트 인덱스
    if(!DM.ARAY[riPRI_F].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","프론트 프리인덱스에 보트 유무를 확인해주세요.");
        DM.ARAY[riPRI_F].SetStat(csNone);
    }
    if(!DM.ARAY[riIDX_F].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","프론트 인덱스에 보트 유무를 확인해주세요.");
        DM.ARAY[riIDX_F].SetStat(csNone);
    }
    if(!DM.ARAY[riPSI_F].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","프론트 포스트 인덱스에 보트 유무를 확인해주세요.");
        DM.ARAY[riPSI_F].SetStat(csNone);
    }

    //리어 인덱스
    if(!DM.ARAY[riPRI_R].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","리어 프리인덱스에 보트 유무를 확인해주세요.");
        DM.ARAY[riPRI_R].SetStat(csNone);
    }
    if(!DM.ARAY[riIDX_R].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","리어 인덱스에 보트 유무를 확인해주세요.");
        DM.ARAY[riIDX_R].SetStat(csNone);
    }
    if(!DM.ARAY[riPSI_R].CheckAllStat(csNone)) {
        FM_MsgOkModal("Check","리어 포스트 인덱스에 보트 유무를 확인해주세요.");
        DM.ARAY[riPSI_R].SetStat(csNone);
    }


    //랏오픈파일 테이블에서 데이터맵으로 데이터 이동.
    SetDataMap();





    LOT.LotOpen(LotNo , Device) ;

    Trace("LotOpen" , "Try");

    Close();

    FM_MsgOkModal("에폭시 종류를 확인하세요" , OM.DevOptn.sEpoxyName.c_str());

    FM_MsgOkModal("Confirm" , "언로더에 보트 없음을 확인하세요");

    FM_MsgOkModal("Confirm","장비 이니셜을 합니다.");



    MM.SetManCycle(mcAllHome);

    DSP.m_bNiddleChecked = false; //랏오픈하면 니들 Z축을 보정해야 한다.

}

void TFrmLotOpen::SetDataMap()
{
    //Boat시리얼 확인.
    String sFrontChar = OM.DevInfo.iWorkMode == wmFos ? "F" : "D" ;
    String sRearChar  = OM.DevInfo.iWorkMode == wmFos ? "M" : "M" ;
    for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
        if(g_LotData.GetCellByColTitle("FrontBoatNo",i).Pos(sFrontChar)==0&&g_LotData.GetCellByColTitle("FrontBoatNo",i)!=""){
            FM_MsgOk("Error",("FrontBoatNo : " + String(i) + " 행의 BoatNo가" + sFrontChar + "가 아닙니다.").c_str());
            return ;
        }
        if(g_LotData.GetCellByColTitle("RearBoatNo",i).Pos(sRearChar)==0 && g_LotData.GetCellByColTitle("RearBoatNo",i)!=""){
            FM_MsgOk("Error",("RearBoatNo : " + String(i) + " 행의 BoatNo가" + sRearChar + "가 아닙니다.").c_str());
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
                if(g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i) == String(r+1)&&//슬롯넘버랑.
                   g_LotData.GetCellByColTitle("FrontBoatNo"        ,i) != ""         ){//보트 아이디가 있어야 함.
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
                if(g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i) == String(r+1)&&//슬롯넘버랑.
                   g_LotData.GetCellByColTitle("FrontBoatNo"        ,i) != ""         ){//보트 아이디가 있어야 함.
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




//Test용이다.
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

    //랏오픈파일 테이블에서 데이터맵으로 데이터 이동.
    SetDataMap();

    LOT.LotOpen(LotNo , Device) ;

    Trace("LotOpen" , "Try");

    Close();

    FM_MsgOkModal("에폭시 종류를 확인하세요" , OM.DevOptn.sEpoxyName.c_str());

    FM_MsgOkModal("Confirm","장비 이니셜을 합니다.");



    MM.SetManCycle(mcAllHome);

    DSP.m_bNiddleChecked = false; //랏오픈하면 니들 Z축을 보정해야 한다.

}
//---------------------------------------------------------------------------

