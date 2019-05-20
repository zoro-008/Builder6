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
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "Stage.h"
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

//    PRI_F.m_iWorkPkgCnt = 0 ;
//    PRI_R.m_iWorkPkgCnt = 0 ;




    //스테이지 미드 갯수.
    //int iMidColCnt = OM.DevInfo.iMidColCnt;
    //int iMidRowCnt = OM.DevInfo.iMidRowCnt;
    //int iMidMaxCnt = iMidColCnt * iMidRowCnt;

    DM.ClearMap();
    DM.ARAY[riLDR_F].SetStat(csEmpty);
    DM.ARAY[riLDR_R].SetStat(csEmpty);
    DM.ARAY[riULD  ].SetStat(csEmpty);

    //체크 모드 Stat 변경
    ////if(OM.CmnOptn.iWorkMode == 1) { //측정모드일때.
    //    DM.ARAY[riLDR_F].SetStat(csDetect);
    //    LOT.LotOpen(LotNo , Device) ;
    //    Trace("LotOpen" , "Try");
    //    Close();
    //    return ;
    //}

    //프론트 로더 Stat 변경

    //엑셀 미들블럭 데이터 개수.
    int iMidDataCnt = g_LotData.GetMaxRow()-1;

    if(OM.CmnOptn.iWorkMode == 1) { //측정모드일때.
        DM.ARAY[riLDR_F].SetStat(csEmpty);
    }
    else if(OM.CmnOptn.iWorkMode == 0 || OM.CmnOptn.iWorkMode == 2){
        int iCrntMidDataCnt = 0 ;
        String sCrntMidId = "";
        for(int r = 0 ; r < OM.DevInfo.iLDRFMgzSlotCnt ; r++){
            for(int c = OM.DevInfo.iLDRFMgzBayCnt-1 ; c >= 0 ; c--){ //로더 슬롯은 장비전면에서 오른쪽것이 처음이다.
                if(iCrntMidDataCnt >= iMidDataCnt) continue ;
                iCrntMidDataCnt++;
                //sCrntMidId=g_LotData.GetCellByColTitle("MiddleBlockID",iCrntMidDataCnt);
                //if(sCrntMidId != ""){
                    //DM.ARAY[riLDR_F].CHPS[r][c].SetID(sCrntMidId);
                    DM.ARAY[riLDR_F].SetStat(r,c,csUnkwn);
                //}
            }
        }
    }

    //리어 로더 Stat 변경
    if(OM.CmnOptn.iWorkMode == 2) {
        DM.ARAY[riLDR_R].SetStat(csEmpty);
    }
    else if(OM.CmnOptn.iWorkMode == 0 || OM.CmnOptn.iWorkMode == 1){
        int iCrntModDataCnt = 0 ;
        const int iStgModCnt = OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt ;
        int iCrntStgDataIdx = 0 ; //iStgModCnt
        //엑셀 모듈 데이터 갯수
        int iModDataCnt = iMidDataCnt * OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt ;
        String sCrntModId = "";
        String sCrntColTilte = "" ;
        for(int r = 0 ; r < OM.DevInfo.iLDRRMgzSlotCnt ; r++){
            for(int c = OM.DevInfo.iLDRRMgzBayCnt-1 ; c >= 0 ; c--){ //로더 슬롯은 장비전면에서 오른쪽것이 처음이다.
                if(iCrntModDataCnt >= iModDataCnt) continue ;
                iCrntStgDataIdx = iCrntModDataCnt % iStgModCnt ; //미들블럭 개별 카운트 인덱스.
                sCrntColTilte = String("ModuleID") + (iCrntStgDataIdx % OM.DevInfo.iMidColCnt) + (iCrntStgDataIdx / OM.DevInfo.iMidColCnt) ;
                //sCrntModId=g_LotData.GetCellByColTitle(sCrntColTilte,iCrntModDataCnt/OM.DevInfo.iMidColCnt+1);
                sCrntModId=g_LotData.GetCellByColTitle(sCrntColTilte,iCrntModDataCnt/iStgModCnt+1);
                iCrntModDataCnt++;
                if(sCrntModId != ""){
                    DM.ARAY[riLDR_R].CHPS[r][c].SetID(sCrntModId);
                    DM.ARAY[riLDR_R].SetStat(r,c,csUnkwn);
                }
            }
        }
    }

    LOT.LotOpen(LotNo , Device) ;
    Trace("LotOpen" , "Try");
    Close();

    FM_MsgOkModal("에폭시 종류를 확인하세요" , OM.DevOptn.sEpoxyName.c_str());

    LOT.SetEpoxyLotNo(edEpoxyLotNo -> Text);


//  MM.SetManCycle(mcAllHome);
//  DSP.m_bNiddleChecked = false; //랏오픈하면 니들 Z축을 보정해야 한다.

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





