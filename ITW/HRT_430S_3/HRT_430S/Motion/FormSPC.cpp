//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormSPC.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "UserFile.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "LanguageUnit.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
__fastcall TFrmSPC::TFrmSPC(TComponent* Owner)
    : TForm(Owner)
{
    sgErrHistory->Cells[0][0] = "No" ;
    sgErrHistory->Cells[1][0] = "ErrNo" ;
    sgErrHistory->Cells[2][0] = "Comment" ;
    sgErrHistory->Cells[3][0] = "Occur Date(Time)" ;
    sgErrHistory->Cells[4][0] = "Release Date(Time)" ;
    sgErrCnt    ->Cells[0][0] = "No" ;
    sgErrCnt    ->Cells[1][0] = "ErrNo" ;
    sgErrCnt    ->Cells[2][0] = "Comment" ;
    sgErrCnt    ->Cells[3][0] = "Count" ;

    sgLotInfo   ->Cells[0][0]  = "WriteTime";
    sgLotInfo   ->Cells[1][0]  = "LotNo"    ;
    sgLotInfo   ->Cells[2][0]  = "JobFile"  ;
    sgLotInfo   ->Cells[3][0]  = "Operator" ;
    sgLotInfo   ->Cells[4][0]  = "StartTime";
    sgLotInfo   ->Cells[5][0]  = "EndTime"  ;
    sgLotInfo   ->Cells[6][0]  = "RunTime"  ;
    sgLotInfo   ->Cells[7][0]  = "IdleTime" ;
    sgLotInfo   ->Cells[8][0]  = "JamTime"  ;
    sgLotInfo   ->Cells[9][0]  = "TotalTime";
    sgLotInfo   ->Cells[10][0] = "StrpUPEH" ;
    sgLotInfo   ->Cells[11][0] = "ChipUPEH" ;
    sgLotInfo   ->Cells[12][0] = "StrpUPH"  ;
    sgLotInfo   ->Cells[13][0] = "ChipUPH"  ;
    sgLotInfo   ->Cells[14][0] = "WorkStrp" ;
    sgLotInfo   ->Cells[15][0] = "WorkChip" ;
    sgLotInfo   ->Cells[16][0] = "FailChip" ;
    sgLotInfo   ->Cells[17][0] = "Empty"    ;
    sgLotInfo   ->Cells[18][0] = "Bin0"     ;
    sgLotInfo   ->Cells[19][0] = "Bin1"     ;
    sgLotInfo   ->Cells[20][0] = "Bin2"     ;
    sgLotInfo   ->Cells[21][0] = "Bin3"     ;
    sgLotInfo   ->Cells[22][0] = "Bin4"     ;
    sgLotInfo   ->Cells[23][0] = "Bin5"     ;
    sgLotInfo   ->Cells[24][0] = "Bin6"     ;
    sgLotInfo   ->Cells[25][0] = "Bin7"     ;
    sgLotInfo   ->Cells[26][0] = "Bin8"     ;
    sgLotInfo   ->Cells[27][0] = "Bin9"     ;
    sgLotInfo   ->Cells[28][0] = "PDist"    ;
    sgLotInfo   ->Cells[29][0] = "Matching" ;
    sgLotInfo   ->Cells[30][0] = "Flow"     ;
    sgLotInfo   ->Cells[31][0] = "Heater"   ;

    sgLotDayInfo->Cells[0][0]  = "WriteTime,"   ;
    sgLotDayInfo->Cells[1][0]  = "RunTime,"     ;
    sgLotDayInfo->Cells[2][0]  = "IdleTime,"    ;
    sgLotDayInfo->Cells[3][0]  = "JamTime,"     ;
    sgLotDayInfo->Cells[4][0]  = "TotalTime,"   ;
    sgLotDayInfo->Cells[5][0]  = "StrpUPEH,"    ;
    sgLotDayInfo->Cells[6][0]  = "ChipUPEH,"    ;
    sgLotDayInfo->Cells[7][0]  = "StrpUPH,"     ;
    sgLotDayInfo->Cells[8][0]  = "ChipUPH,"     ;
    sgLotDayInfo->Cells[9][0]  = "WorkStrp,"    ;
    sgLotDayInfo->Cells[10][0] = "WorkChip,"    ;
    sgLotDayInfo->Cells[11][0] = "FailChip,"    ;
    sgLotDayInfo->Cells[12][0] = "LotCnt,"      ;
    sgLotDayInfo->Cells[13][0] = "CntRslt1,"    ;
    sgLotDayInfo->Cells[14][0] = "CntRslt2,"    ;
    sgLotDayInfo->Cells[15][0] = "CntRslt3,"    ;
    sgLotDayInfo->Cells[16][0] = "CntRslt4,"    ;
    sgLotDayInfo->Cells[17][0] = "CntRslt5,"    ;
    sgLotDayInfo->Cells[18][0] = "CntRslt6,"    ;
    sgLotDayInfo->Cells[19][0] = "CntRslt7,"    ;
    sgLotDayInfo->Cells[20][0] = "CntRslt8,"    ;
    sgLotDayInfo->Cells[21][0] = "CntRslt9,"    ;
    sgLotDayInfo->Cells[22][0] = "CntRslt10,"   ;
    sgLotDayInfo->Cells[23][0] = "CntRslt11,"   ;
    sgLotDayInfo->Cells[24][0] = "CntRslt12,"   ;
    sgLotDayInfo->Cells[25][0] = "CntRslt13,"   ;
    sgLotDayInfo->Cells[26][0] = "CntRslt14,"   ;
    sgLotDayInfo->Cells[27][0] = "CntHeatFail," ;

    DM.ARAY[riSPC].SetParent(pnLotMap  ); DM.ARAY[riSPC].SetConfig(false , "LotMap  "); DM.ARAY[riSPC].ClearDispItem();
    DM.ARAY[riSPC].SetDispItem(csUnkwn   ,"Unknown "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen      );
    DM.ARAY[riSPC].SetDispItem(csFail    ,"Heat Fail "    ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riSPC].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPSB ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riSPC].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riPSB ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riSPC].SetDispItem(csGood    ,"Good "         ) ; //DM.ARAY[riPSB ].SetDispColor(csWork  ,clHighlight       );


    // juhyeon
    if(!OM.MstOptn.bUsedTempRS232){
        Label23 -> Visible = false ;
        edTemp6 -> Visible = false ;
    }
    else {
        Label23 -> Visible = true ;
        edTemp6 -> Visible = true ;
    }

//    if(FM_GetLan() == lsKorean) LAG.LoadFrmSPCHan();
//    switch(FM_GetLan())
//    {
//        default        :                             break ;
//        case lsEnglish :                             break ;
//        case lsKorean  : LAG.LoadFrmSPCHan();     break ;
//        case lsChinese :                             break ;
//    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::UpdateComOptn(bool _bToTable)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::FormShow(TObject *Sender)
{
    TDateTime CurrDateTime = Now();

    edMtbaDT    ->Format = "yyyy-MM";

    edF_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edFDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edUDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edMtbaDT       ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");

    LT.UpdateDate(sgDate);
    UserFile.GridSearchFile(LOG_FOLDER , sgLotDayName , false);

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgDateClick(TObject *Sender)
{
    LT.UpdateLotName(sgDate , sgLot);
    sgLot -> Col = 1 ;
    sgLotClick(Sender);
    //LT.DispLotDate(sgDate , sgLot);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotClick(TObject *Sender)
{
    LT.DispLotDate(sgDate , sgLot , sgLotInfo);

    AnsiString sPath;
    sPath = LOG_FOLDER + sgDate ->Cells[1][sgDate   -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
    sPath.Delete(sPath.Pos(".csv"),4);
    UserFile.GridSearchDir(sPath , sgMgzNo  , 1 , false);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
    for(int i=0; i<sgMgzNo->RowCount; i++) {
        if(sgMgzNo->Cells[1][i] == "Log") sgMgzNo->Cells[1][i] = "";
    }
    sgMgzNoClick(Sender);

    LT.ReadBinInfo (sgDate, sgLot , &m_iBin0Cnt, &m_iBin1Cnt, &m_iBin2Cnt, &m_iBin3Cnt, &m_iBin4Cnt,
                                    &m_iBin5Cnt, &m_iBin6Cnt, &m_iBin7Cnt, &m_iBin8Cnt, &m_iBin9Cnt);

    sgLotInfo -> Col = 1 ;
    sgLotInfoClick(Sender);

    edBin0     -> Text = m_iBin0Cnt;
    edBin1     -> Text = m_iBin1Cnt;
    edBin2     -> Text = m_iBin2Cnt;
    edBin3     -> Text = m_iBin3Cnt;
    edBin4     -> Text = m_iBin4Cnt;
    edBin5     -> Text = m_iBin5Cnt;
    edBin6     -> Text = m_iBin6Cnt;
    edBin7     -> Text = m_iBin7Cnt;
    edBin8     -> Text = m_iBin8Cnt;
    edBin9     -> Text = m_iBin9Cnt;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrDataViewClick(TObject *Sender)
{
    SPC.DispErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrCntViewClick(TObject *Sender)
{
    SPC.DispErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btMtbaViewClick(TObject *Sender)
{
//    ChatMtba -> CleanupInstance();
//    S1_Mtba->Clear();
    SPC.DispMtba(edMtbaDT->Date.FormatString("yyyymm") , S1_Mtba);
//    dbMtba->LeftAxis
//    Series1->AddXY()
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btMtbaSaveClick(TObject *Sender)
{
    SPC.SaveMtba(edMtbaDT->Date.FormatString("yyyymm"),S1_Mtba);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrCntSaveClick(TObject *Sender)
{
    SPC.SaveErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrDataSaveClick(TObject *Sender)
{
    SPC.SaveErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotInfoViewClick(TObject *Sender)
{
    LT.DispLotDate(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotInfoSaveClick(TObject *Sender)
{
    LT.SaveLotData(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgLotDayNameClick(TObject *Sender)
{
    LT.DispLotDayDate(sgLotDayName , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotDayViewClick(TObject *Sender)
{
    LT.DispLotDayDate(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotDaySaveClick(TObject *Sender)
{
    LT.SaveLotDayData(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgSlotNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath  = LOG_FOLDER + sgDate->Cells[1][sgDate -> Row] + "\\" + sgLot->Cells[1][sgLot -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] + "\\" + sgSlotNo->Cells[1][sgSlotNo -> Row] ;
//    LT.ReadArayData(sPath , riSPC);
    SPC.ReadLotMap(sPath , sgLotMap);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgMgzNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath  = LOG_FOLDER + sgDate ->Cells[1][sgDate -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
    sPath.Delete(sPath.Pos(".csv"),4);
    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] ;
    UserFile.GridSearchFile(sPath , sgSlotNo  , 1 , false);     // ���丮 �о�ͼ� ��¥�� ���ĺ� ������ ����
    sgSlotNoClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgLotInfoClick(TObject *Sender)
{
    edTotal  -> Text = sgLotInfo -> Cells[17][sgLotInfo -> Row] ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgLotMapDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItemVf[3] , sItemVr[3] ;
    if(ARow == 0 || ACol == 0) return ;
    if(sgLotMap->RowCount == 2 && sgLotMap->ColCount == 2) return ; //�̰� �ϳ� �� ���κ꿡 �ִ°ź��� �߰���.
    if(!SPC.GetChipCnt()) return ;

    int iDftRowHeight = 18 ;
    int iRoom = 0 ;

    TRect dRect[6] ;

    for(int i =0 ; i<6 ; i++ )    dRect[i] = Rect ;

    sItemVf[0].printf("%0.2f",SPC.dMapVf[ARow-1][ACol-1][0]);
    sItemVr[0].printf("%0.2f",SPC.dMapVr[ARow-1][ACol-1][0]);
    sItemVf[1].printf("%0.2f",SPC.dMapVf[ARow-1][ACol-1][1]);
    sItemVr[1].printf("%0.2f",SPC.dMapVr[ARow-1][ACol-1][1]);
    sItemVf[2].printf("%0.2f",SPC.dMapVf[ARow-1][ACol-1][2]);
    sItemVr[2].printf("%0.2f",SPC.dMapVr[ARow-1][ACol-1][2]);

         if(OM.DevOptn.iDgChipCnt == 1 &&  OM.DevOptn.bUseZener ) iRoom = 2 ;//2ĭ
    else if(OM.DevOptn.iDgChipCnt == 1 && !OM.DevOptn.bUseZener ) iRoom = 2 ;//2ĭ
    else if(OM.DevOptn.iDgChipCnt == 2 &&  OM.DevOptn.bUseZener ) iRoom = 4 ;//4ĭ
    else if(OM.DevOptn.iDgChipCnt == 2 && !OM.DevOptn.bUseZener ) iRoom = 2 ;//2ĭ
    else if(OM.DevOptn.iDgChipCnt == 3 &&  OM.DevOptn.bUseZener ) iRoom = 6 ;//6ĭ
    else if(OM.DevOptn.iDgChipCnt == 3 && !OM.DevOptn.bUseZener ) iRoom = 3 ;//3ĭ


    switch(iRoom)
    {
        default :
        break ;
        case 2 : if( sgLotMap->RowHeights[ARow] != iDftRowHeight * 2) {
                     sgLotMap->RowHeights[ARow]  = iDftRowHeight * 2 ;
//                     sgLotMap->DefaultColWidth   = 25 ;
                     return ;
                 }
                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
        break ;
        case 3 : if( sgLotMap->RowHeights[ARow] != iDftRowHeight * 3 ) {
                     sgLotMap->RowHeights[ARow]  = iDftRowHeight * 3 ;
//                     sgLotMap->DefaultColWidth   = 25 ;
                     return;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight    ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight    ;
                 dRect[2].Top    = dRect[1].Bottom               ;
        break ;
        case 4 : if( sgLotMap->RowHeights[ARow] != iDftRowHeight * 4 ) {
                     sgLotMap->RowHeights[ARow]  = iDftRowHeight * 4 ;
//                     sgLotMap->DefaultColWidth   = 25 ;
                     return;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight  ;
                 dRect[1].Top    = dRect[0].Bottom               ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight  ;
                 dRect[2].Top    = dRect[1].Bottom               ;
                 dRect[2].Bottom = dRect[2].Top + iDftRowHeight  ;
                 dRect[3].Top    = dRect[2].Bottom               ;
                 //Rect4.Bottom = Rect4.Top + iRectHeight * 4;
        break ;
        case 6 : if( sgLotMap->RowHeights[ARow] != iDftRowHeight * 6 ){
                     sgLotMap->RowHeights[ARow]  = iDftRowHeight * 6 ;
//                     sgLotMap->DefaultColWidth   = 25 ;
                     return ;
                 }

                 dRect[0].Bottom = dRect[0].Top + iDftRowHeight    ;
                 dRect[1].Top    = dRect[0].Bottom                 ;
                 dRect[1].Bottom = dRect[1].Top + iDftRowHeight * 2;
                 dRect[2].Top    = dRect[1].Bottom                 ;
                 dRect[2].Bottom = dRect[2].Top + iDftRowHeight * 3;
                 dRect[3].Top    = dRect[2].Bottom                 ;
                 dRect[3].Bottom = dRect[3].Top + iDftRowHeight * 4;
                 dRect[4].Top    = dRect[3].Bottom                 ;
                 dRect[4].Bottom = dRect[4].Top + iDftRowHeight * 5;
                 dRect[5].Top    = dRect[4].Bottom                 ;
        break ;

    }

    int iCount = -1 ;
    for(int i=0 ; i<SPC.GetChipCnt() ; i++ )
    {
        iCount ++ ;
        if( SPC.iMapRslt[ARow-1][ACol-1][iCount] == prReady) {
            sgLotMap->Canvas->Brush->Color = clYellow ;
            sgLotMap->Canvas->FillRect(dRect[iCount])          ;
        }
        else if( SPC.iMapRslt[ARow-1][ACol-1][iCount] == prOk) {
            sgLotMap->Canvas->Brush->Color = clLime   ;
            sgLotMap->Canvas->FillRect(dRect[iCount])          ;
            sgLotMap->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(SPC.GetUseZenner()) {
                sgLotMap->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
        else if(SPC.iMapRslt[ARow-1][ACol-1][iCount] == prVFVRNg) {
            sgLotMap->Canvas->Brush->Color = clRed ;
            sgLotMap->Canvas->FillRect(dRect[iCount])       ;
            sgLotMap->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            if(SPC.GetUseZenner()) {
                sgLotMap->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( SPC.iMapRslt[ARow-1][ACol-1][i] == prVFNg) {
            sgLotMap->Canvas->Brush->Color = clFuchsia  ;
            sgLotMap->Canvas->FillRect(dRect[iCount])          ;
            sgLotMap->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgLotMap->Canvas->Brush->Color = clLime ;
            //sgLotMap->Canvas->FillRect(DnRect)      ;
            if(SPC.GetUseZenner()) {
                sgLotMap->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }

        else if( SPC.iMapRslt[ARow-1][ACol-1][i] == prVRNg) {
            sgLotMap->Canvas->Brush->Color = clOlive ;
            sgLotMap->Canvas->FillRect(dRect[iCount])          ;
            sgLotMap->Canvas->TextOut(dRect[iCount].Left + 1, dRect[iCount].Top , sItemVf[i]);
            //sgLotMap->Canvas->Brush->Color = clRed  ;
            //sgLotMap->Canvas->FillRect(DnRect)      ;
            if(SPC.GetUseZenner()) {
                sgLotMap->Canvas->TextOut(dRect[iCount+1].Left + 1, dRect[iCount+1].Top , sItemVr[i]);
                iCount++;
            }
        }
    }
/*    AnsiString sItemVf , sItemVr ;

    if(ARow == 0 || ACol == 0) return ;
    if(sgLotMap->RowCount == 2 && sgLotMap->ColCount == 2) return ;

    TRect UpRect , DnRect ;

    UpRect = Rect ;UpRect.Bottom = DnRect.Top + Rect.Height() /2 ;
    DnRect = Rect ;DnRect.Top    = DnRect.Top + Rect.Height() /2 ;

    sItemVf.printf("%0.2f",SPC.dMapVf[ARow-1][ACol-1]);
    sItemVr.printf("%0.2f",SPC.dMapVr[ARow-1][ACol-1]);

    if( SPC.iMapRslt[ARow-1][ACol-1] == prReady) {
        sgLotMap->Canvas->Brush->Color = clYellow ;
        sgLotMap->Canvas->FillRect(Rect)          ;
    }
    else if( SPC.iMapRslt[ARow-1][ACol-1] == prOk) {
        sgLotMap->Canvas->Brush->Color = clLime   ;
        sgLotMap->Canvas->FillRect(Rect)          ;
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top , sItemVf);
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top + Rect.Height() / 2, sItemVr);
    }
    else if(SPC.iMapRslt[ARow-1][ACol-1] == prVFVRNg) {
        sgLotMap->Canvas->Brush->Color = clRed ;
        sgLotMap->Canvas->FillRect(Rect)       ;
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top , sItemVf);
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top + Rect.Height() / 2, sItemVr);
    }

    else if( SPC.iMapRslt[ARow-1][ACol-1] == prVFNg) {
        sgLotMap->Canvas->Brush->Color = clFuchsia  ;
        sgLotMap->Canvas->FillRect(Rect)          ;
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top , sItemVf);
        //sgLotMap->Canvas->Brush->Color = clLime ;
        //sgLotMap->Canvas->FillRect(DnRect)      ;
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top + Rect.Height() / 2, sItemVr);
    }

    else if( SPC.iMapRslt[ARow-1][ACol-1] == prVRNg) {
        sgLotMap->Canvas->Brush->Color = clOlive ;
        sgLotMap->Canvas->FillRect(Rect)          ;
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top , sItemVf);
        //sgLotMap->Canvas->Brush->Color = clRed  ;
        //sgLotMap->Canvas->FillRect(DnRect)      ;
        sgLotMap->Canvas->TextOut(Rect.Left + 1, Rect.Top + Rect.Height() / 2, sItemVr);
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    sgLotMap -> Invalidate() ;
    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::FormClose(TObject *Sender, TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::BitBtn1Click(TObject *Sender)
{
//    prReady = -1 ,  //�˻��������.
//    prOk    =  0 ,  // �˻� Good
//    prVFNg  =  1 ,  // VF Fail
//    prVRNg  =  2 ,  // VR Fail
//    prVFVRNg=  3 ,  // VF VR Fail
//
//    MAX_PROB_RSLT


int iTemp ;
    for(int i = 0 ; i< OM.DevInfo.iRowCnt ; i++ )
    {
        for(int j = 0 ; j< OM.DevInfo.iColCnt ; j++ )
        {
            for(int k=0 ; k < OM.DevOptn.iDgChipCnt ; k++ )
            {
                iTemp = j % 4 ;
                iCChipRslt[i][j][k] = iTemp;//? 0 : j;
                dCChipVf  [i][j][k] = 2.232;
                dCChipVr  [i][j][k] = 2.232;
            }
        }
    }
    SPC.WriteLotMap(riWRK);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::BitBtn2Click(TObject *Sender)
{
    AnsiString sPath ;
    TDateTime  CurrDateTime;
//    sPath  = LOG_FOLDER + sgDate->Cells[1][sgDate -> Row] + "\\" + sgLot->Cells[1][sgLot -> Row] ;
//    sPath.Delete(sPath.Pos(".csv"),4);

    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\99\\99\\99.ini" ;
//    sPath += "\\" + sgMgzNo->Cells[1][sgMgzNo -> Row] + "\\" + sgSlotNo->Cells[1][sgSlotNo -> Row] ;
    SPC.ReadLotMap(sPath , sgLotMap);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::Button1Click(TObject *Sender)
{
    SPC.WriteLotMap(riWRK);
}
//---------------------------------------------------------------------------
