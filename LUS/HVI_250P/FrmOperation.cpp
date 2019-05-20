//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmOperation.h"
#include "FrmMain.h"
//---------------------------------------------------------------------------
#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "SLogUnit.h"
#include "PstnMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "Work.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOperation *FormOperation;
//---------------------------------------------------------------------------
__fastcall TFormOperation::TFormOperation(TComponent* Owner)
    : TForm(Owner)
{
    //DataMan Unit Set
    DM.ARAY[riINB].SetParent(pnInBuff   ); DM.ARAY[riINB].SetConfig(false , "pnINB    " ); DM.ARAY[riINB].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPreBuff  ); DM.ARAY[riPRB].SetConfig(false , "pnPreBuff" ); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riPRI].SetParent(pnWorkZone ); DM.ARAY[riPRI].SetConfig(false , "pnPreIdx " ); DM.ARAY[riPRI].ClearDispItem();
    DM.ARAY[riPSI].SetParent(pnWorkLast ); DM.ARAY[riPSI].SetConfig(false , "pnPstIdx " ); DM.ARAY[riPSI].ClearDispItem();


    DM.ARAY[riINB ].SetDispItem(csNone   ,"None  "     ) ;
    DM.ARAY[riINB ].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riINB ].SetDispItem(csWork   ,"Chip  "     ) ;
    DM.ARAY[riINB ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riINB ].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riINB ].SetDispItem(csFromBuf,"FromBuf"    ) ;

    DM.ARAY[riPRB ].SetDispItem(csNone   ,"None  "     ) ;
    DM.ARAY[riPRB ].SetDispItem(csWork   ,"Chip  "     ) ;
    DM.ARAY[riPRB ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riPRB ].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riPRB ].SetDispItem(csFromBuf,"FromBuf"    ) ;

    DM.ARAY[riPRI ].SetDispItem(csNone   ,"None  "     ) ;
    DM.ARAY[riPRI ].SetDispItem(csWork   ,"Chip  "     ) ;
    DM.ARAY[riPRI ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riPRI ].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riPRI ].SetDispItem(csFromBuf,"FromBuf"    ) ;

    DM.ARAY[riPSI ].SetDispItem(csNone   ,"None  "     ) ;
    DM.ARAY[riPSI ].SetDispItem(csWork   ,"Chip  "     ) ;
    DM.ARAY[riPSI ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riPSI ].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riPSI ].SetDispItem(csFromBuf,"FromBuf"    ) ;

    //메인폼 타이머에서 레벨 바꿀시에 자꾸 뻑이나서 아예 시작 할때 픽스 .
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
//        if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
        DM.ARAY[i].PopupMenuEnable(false);
    }

    //Default Set.
    clStat[csRslt0 ] = clInactiveCaption ;//csInactiveCaption
    clStat[csRslt1 ] = clBlack           ;//csMaroon
    clStat[csRslt2 ] = clRed             ;//csPurple
    clStat[csRslt3 ] = clGreen           ;//csTeal
    clStat[csRslt4 ] = clBlue            ;//csOlive
    clStat[csRslt5 ] = clLime            ;//csNavy
    clStat[csRslt6 ] = clFuchsia         ;//csMoneyGreen
    clStat[csRslt7 ] = clWhite           ;//csGreen
    clStat[csRslt8 ] = clMaroon          ;//clHighlight
    clStat[csRslt9 ] = clTeal            ;//csBlack
    clStat[csRsltA ] = clAqua            ;//csBlack
    clStat[csRsltB ] = clNavy            ;//csBlack
    clStat[csRsltC ] = clOlive           ;//csBlack
    clStat[csRsltD ] = clYellow          ;//csBlack
    clStat[csRsltE ] = clSkyBlue         ;//csBlack
    clStat[csRsltF ] = (TColor)0x009D12E0;//csBlack 더 있는거 같다 나중에 추가.dd

    clStat[csNone  ] = clInactiveBorder  ;//csWhite
    clStat[csEmpty ] = clAppWorkSpace    ;//csSilver
    clStat[csUnkwn ] = clMoneyGreen      ;//csAqua
    clStat[csWork  ] = (TColor)0x00FF9595;//clHighlight       ;//csBlue
    clStat[csFail  ] = clPurple          ;//csFuchsia
    clStat[csEtc   ] = clNone            ;//etc*/

    SEQ.Reset();


}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    //Display Lot Info
    LT.pnLotWorkChip(
        pnLotNo        ,
        pnLotJobFile   ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnLotWorkStrip ,
        pnLotWorkChip  ,
        NULL          );

    LT.DispDayInfo(
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL            ,
        NULL           ,
        NULL            ,
        NULL            ,
        NULL            ,
        NULL            ,
        NULL            ,
        NULL            );

    //EM_Disp
    EM_DisplayList(lbErr);

    //Dispr Unknown 일때에 물어보고 데이터 삭제 하거나 넣어가 하려고...
    if(SEQ._bInspDispr) { CheckStripDisappear(); SEQ._bInspDispr = false; } //함수 끝나고 자동 스타트 됍.
    if(SEQ._bInspUnkwn) { CheckStripUnknwon  (); SEQ._bInspUnkwn = false; } //함수 끝나고 자동 스타트 됍.

    //Dispaly Lot No , Mgz No , Slot No
    if(/*DM.ARAY[riWR1].GetCntExist() &&*/ !edINBLotNo -> Focused() && !edINBId->Focused()){
        edINBLotNo  -> Text = DM.ARAY[riINB].GetLotNo() ;
        edINBId     -> Text = DM.ARAY[riINB].GetID()    ;
    }
    if(/*DM.ARAY[riWR1].GetCntExist() &&*/ !edPRBLotNo -> Focused() && !edPRBId->Focused() && !btPRBInput->Focused() ) {
        edPRBLotNo  -> Text = DM.ARAY[riPRB].GetLotNo() ;
        edPRBId     -> Text = DM.ARAY[riPRB].GetID()    ;
    }
    if(/*DM.ARAY[riWR1].GetCntExist() &&*/ !edPWKLotNo -> Focused() && !edPWKId->Focused()) {
        edPWKLotNo  -> Text = DM.ARAY[riPRI].GetLotNo() ;
        edPWKId     -> Text = DM.ARAY[riPRI].GetID()    ;
    }
    if(/*DM.ARAY[riWR2].GetCntExist() &&*/ !edWRKLotNo -> Focused() && !edWRKId->Focused()) {
        edWRKLotNo  -> Text = DM.ARAY[riPSI].GetLotNo() ;
        edWRKId     -> Text = DM.ARAY[riPSI].GetID()    ;
    }

    btPRBInput  -> Visible  = FM_GetLevel() == lvMaster ; //!SEQ._bRun &&

    btINBClear  -> Enabled  = !SEQ._bRun ;
    btPRBClear  -> Enabled  = !SEQ._bRun ;
    btPWKClear  -> Enabled  = !SEQ._bRun ;
    btWRKClear  -> Enabled  = !SEQ._bRun ;

    //Display
    //이상혁 사장님 UI 맞추다 보니 생긴...Total 프로덕트 인포메이션 3개
    pnTotalFrmCnt -> Caption = WRK.m_iTotalFrameCnt ;
    pnTotalLedCnt -> Caption = WRK.m_iTotalChip     ;
    pnTotalToolCnt-> Caption = WRK.m_iTotalToolCnt  ;

    //Display Now Punch Position.
    double dTemp = MT_GetEncPos(miWRK_ZPch);
    String sTemp ;

    sTemp.sprintf("%0.2f", dTemp);
    lbWRK_ZPch -> Caption = sTemp ;
    //Map Update
    DM.UpdateAray();

    //DisPlay Bin Info.
    String sTempBin ;
    int iLotLedCnt = (OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt)) ;
    double dTempBin1 , dTempBin2 , dTempBin3 , dTempBin4 , dTempBin5 ;
    int iTempBin, iTempBin1 , iTempBin2 , iTempBin3 , iTempBin4 , iTempBin5 ;

    iTempBin1 = ((double)WRK.m_iBin1Cnt / (double)iLotLedCnt) *100 ;
    iTempBin2 = ((double)WRK.m_iBin2Cnt / (double)iLotLedCnt) *100 ;
    iTempBin3 = ((double)WRK.m_iBin3Cnt / (double)iLotLedCnt) *100 ;
    iTempBin4 = ((double)WRK.m_iBin4Cnt / (double)iLotLedCnt) *100 ;
    iTempBin5 = ((double)WRK.m_iBin5Cnt / (double)iLotLedCnt) *100 ;

    sTempBin = (String)WRK.m_iBin1Cnt + "/" + (String)iLotLedCnt ; pnBin1Cnt->Caption = sTempBin ;
    sTempBin = (String)WRK.m_iBin2Cnt + "/" + (String)iLotLedCnt ; pnBin2Cnt->Caption = sTempBin ;
    sTempBin = (String)WRK.m_iBin3Cnt + "/" + (String)iLotLedCnt ; pnBin3Cnt->Caption = sTempBin ;
    sTempBin = (String)WRK.m_iBin4Cnt + "/" + (String)iLotLedCnt ; pnBin4Cnt->Caption = sTempBin ;
    sTempBin = (String)WRK.m_iBin5Cnt + "/" + (String)iLotLedCnt ; pnBin5Cnt->Caption = sTempBin ;

    sTempBin = (String)iTempBin1 +"%" ; pnBin1Per->Caption = sTempBin ;
    sTempBin = (String)iTempBin2 +"%" ; pnBin2Per->Caption = sTempBin ;
    sTempBin = (String)iTempBin3 +"%" ; pnBin3Per->Caption = sTempBin ;
    sTempBin = (String)iTempBin4 +"%" ; pnBin4Per->Caption = sTempBin ;
    sTempBin = (String)iTempBin5 +"%" ; pnBin5Per->Caption = sTempBin ;

    pnBin1Per->Color = WRK.m_iBinNo == 0 ? clLime : clSilver ;
    pnBin2Per->Color = WRK.m_iBinNo == 1 ? clLime : clSilver ;
    pnBin3Per->Color = WRK.m_iBinNo == 2 ? clLime : clSilver ;
    pnBin4Per->Color = WRK.m_iBinNo == 3 ? clLime : clSilver ;
    pnBin5Per->Color = WRK.m_iBinNo == 4 ? clLime : clSilver ;

    if(OM.CmnOptn.bUseOnly1Bin){
        sTempBin = (String)iTempBin1 + "%" ; pnTotalBinPer -> Caption = sTempBin ;
    }
    else{
        iTempBin = (iTempBin1 + iTempBin2 + iTempBin3 + iTempBin4 + iTempBin5) / 5;
        sTempBin = (String)iTempBin + "%" ; pnTotalBinPer -> Caption = sTempBin ;
    }

    //나중에 상황 봐서.
    //pnBin1Cnt -> Color = WRK.Bin1.bBinFull ? clGreen : clBtnFace ;
    //pnBin2Cnt -> Color = WRK.Bin2.bBinFull ? clGreen : clBtnFace ;
    //pnBin3Cnt -> Color = WRK.Bin3.bBinFull ? clGreen : clBtnFace ;
    //pnBin4Cnt -> Color = WRK.Bin4.bBinFull ? clGreen : clBtnFace ;
    //pnBin5Cnt -> Color = WRK.Bin5.bBinFull ? clGreen : clBtnFace ;
    //bool bTotalBinFull = ( WRK.Bin1.bBinFull && WRK.Bin2.bBinFull && WRK.Bin3.bBinFull && WRK.Bin4.bBinFull && WRK.Bin5.bBinFull ) ;
    //pnTotalBinPer -> Color = bTotalBinFull ? clGreen : clBtnFace ;

    //double dTempBin = ((WRK.Bin1.iBinCnt + WRK.Bin2.iBinCnt + WRK.Bin3.iBinCnt + WRK.Bin4.iBinCnt + WRK.Bin5.iBinCnt) /
    //(OM.DevInfo.iCsSlCnt * OM.DevOptn.iLotEndMgzCnt * (OM.DevInfo.iRowCnt * OM.DevInfo.iColCnt)) ) * 100 ;
    //sTempBin = (String)dTempBin+ "%" ;
    //pnTotalBinPer ->Caption = sTempBin ;

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::UpdateCmmOptn(bool bTable)
{
    if(bTable) {
        TNotifyEvent ChkBoxOnClick=cbDoorSkip->OnClick;
        cbDoorSkip         -> OnClick=NULL;
        cbBarCodePrintSkip -> OnClick=NULL;
        cbUseOnly1Bin      -> OnClick=NULL;


        cbDoorSkip         -> Checked = OM.CmnOptn.bDoorSkip          ;
        cbBarCodePrintSkip -> Checked = OM.CmnOptn.bBarCodePrintSkip  ;
        cbUseOnly1Bin      -> Checked = OM.CmnOptn.bUseOnly1Bin       ;

        cbDoorSkip         -> OnClick=ChkBoxOnClick;
        cbBarCodePrintSkip -> OnClick=ChkBoxOnClick;
        cbUseOnly1Bin      -> OnClick=ChkBoxOnClick;

    }
    else {
        OM.CmnOptn.bDoorSkip           = cbDoorSkip         -> Checked  ;
        OM.CmnOptn.bBarCodePrintSkip   = cbBarCodePrintSkip -> Checked  ;
        OM.CmnOptn.bUseOnly1Bin        = cbUseOnly1Bin      -> Checked  ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true ;

    OM.LoadCmnOptn();
    UpdateCmmOptn(toTabl);
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::btPRBClearClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;


         if(sTemp == "btPRBClear") { DM.ARAY[riPRB].SetStat(csNone) ; }
    else if(sTemp == "btPWKClear") { DM.ARAY[riPRI].SetStat(csNone) ; }
    else if(sTemp == "btWRKClear") { DM.ARAY[riPSI].SetStat(csNone) ; }
    else if(sTemp == "btINBClear") { DM.ARAY[riINB].SetStat(csNone) ; }

}


//---------------------------------------------------------------------------

void __fastcall TFormOperation::btPRBInputClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

//    if(FM_MsgYesNo("Confirm","Did you want to enter Lot information ") != mrYes) return ;

    AnsiString sLotNo ;
    String sId = "";

    sLotNo  = edPRBLotNo ->Text ;
    sId     = edPRBId    ->Text ;
    DM.ARAY[riPRB].SetLotNo(sLotNo) ;
    DM.ARAY[riPRB].SetID(sId.ToIntDef(9999));
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::lbErrClick(TObject *Sender)
{
    String Temp ;
    int iTemp ;
    int Cnt,iCount ;
    char CChar ;
    Cnt   = 0  ;
    CChar = '*';

    iTemp = lbErr->ItemIndex ;
    Temp  = lbErr->Items->Strings[iTemp];
    if(Temp.Pos("[ERR"))
    {
        iTemp = Temp.SubString(Temp.Pos("[ERR")+4,3).ToIntDef(0);
        Temp  = EM_GetErrAction(iTemp);
        for(register int i = 1 ; i <= Temp.Length() ; i++) if (Temp[i] == CChar) iCount++;
        Cnt = iCount;
        mmAction->Clear();
        for (int i=0; i <= Cnt ; i++) {
            AnsiString Line  = "";
            int n = Temp.Pos("*");
            Line  = Temp.SubString(1, n ? n-1 : n);
            Temp  = Temp.Delete(1, n);
            if (!n) { mmAction -> Lines -> Add(Temp); break; }
            else    { mmAction -> Lines -> Add(Line);        }
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false ;
    static int i = 0 ;
    AnsiString sTemp = i ;
    Trace("~TFormOperation",sTemp.c_str());
}
//---------------------------------------------------------------------------
bool __fastcall TFormOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFormOperation::CheckStripUnknwon()
{
    return true ;
}

void __fastcall TFormOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;        
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::cbDoorSkipClick(TObject *Sender)
{
    String sTemp;

    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::btBinBoxRemoveClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do You Really Remove Bin Box?") != mrYes) return ;

    WRK.ClearBin(0);
    WRK.ClearBin(1);
    WRK.ClearBin(2);
    WRK.ClearBin(3);
    WRK.ClearBin(4);

}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::pnTotalFrmCntDblClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do You Really Infomation Clear?") != mrYes) return ;

    int iSel = ((TPanel *)Sender) -> Tag ;

         if(iSel == 1) WRK.m_iTotalChip     = 0 ;
    else if(iSel == 2) WRK.m_iTotalFailChip = 0 ;
    else if(iSel == 3) WRK.m_iTotalToolCnt  = 0 ;
}


void __fastcall TFormOperation::pnBin1CntClick(TObject *Sender)
{
    int iSel = ((TPanel *)Sender) -> Tag ;

        if(iSel == 1){if(FM_MsgYesNo("Confirm","Do You Really Remove 1 Bin Box?") != mrYes) return ; WRK.ClearBin(0);}
        if(iSel == 2){if(FM_MsgYesNo("Confirm","Do You Really Remove 2 Bin Box?") != mrYes) return ; WRK.ClearBin(1);}
        if(iSel == 3){if(FM_MsgYesNo("Confirm","Do You Really Remove 3 Bin Box?") != mrYes) return ; WRK.ClearBin(2);}
        if(iSel == 4){if(FM_MsgYesNo("Confirm","Do You Really Remove 4 Bin Box?") != mrYes) return ; WRK.ClearBin(3);}
        if(iSel == 5){if(FM_MsgYesNo("Confirm","Do You Really Remove 5 Bin Box?") != mrYes) return ; WRK.ClearBin(4);}
}
//---------------------------------------------------------------------------


