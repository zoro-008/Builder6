//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"

//---------------------------------------------------------------------------
#include "JigPick.h"
#include "Dispnr.h"
#include "Place.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"

#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "UserIni.h"
#include "Rs232Man.h"

#include "Math.h"
//#include "SAInterface.h"

#define MAX_PICTURE 23
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{

    DM.ARAY[riDSP].SetParent(pnDSP ); DM.ARAY[riDSP].SetConfig(false , "DSP"); DM.ARAY[riDSP].ClearDispItem();
    DM.ARAY[riJIG].SetParent(pnJIG ); DM.ARAY[riJIG].SetConfig(false , "JIG"); DM.ARAY[riJIG].ClearDispItem();
    DM.ARAY[riPIK].SetParent(pnPIK ); DM.ARAY[riPIK].SetConfig(false , "PIK"); DM.ARAY[riPIK].ClearDispItem();



    DM.ARAY[riDSP].SetDispItem(csNone   ,"None   " ) ;
    DM.ARAY[riDSP].SetDispItem(csUnkwn  ,"Unknown" ) ;
    DM.ARAY[riDSP].SetDispItem(csWork   ,"Work   " ) ;

    DM.ARAY[riJIG].SetDispItem(csNone   ,"None   " ) ;
    DM.ARAY[riJIG].SetDispItem(csUnkwn  ,"Unknown" ) ;

    DM.ARAY[riPIK].SetDispItem(csNone   ,"None   " ) ;
    DM.ARAY[riPIK].SetDispItem(csUnkwn  ,"Unknown" ) ;
    DM.ARAY[riPIK].SetDispItem(csWork   ,"Work   " ) ;

    TUserINI   UserINI;
    AnsiString sPath  ;
    int iLang         ;
    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    UserINI.Load(sPath , "Member"     , "m_iLangSel"  , iLang);

    const bool bKorean = iLang==1;
    iTemp = 0;


    //Manual Button Caption
    btMan1_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan1_14 -> Tag = (int)mcAllHome            ; btMan1_14 -> Visible = true  ;
    //btMan2_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan2_14 -> Tag = (int)mcAllHome            ; btMan2_14 -> Visible = true  ;
    //btMan3_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan3_14 -> Tag = (int)mcAllHome            ; btMan3_14 -> Visible = true  ;
    //btMan4_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan4_14 -> Tag = (int)mcAllHome            ; btMan4_14 -> Visible = true  ;
    //btMan5_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan5_14 -> Tag = (int)mcAllHome            ; btMan5_14 -> Visible = true  ;


    // Stage
    btMan1_1   -> Caption = "[" + AnsiString(mcJIP_CycleJigPick   ) + "]" + (bKorean ? "싸이클 지그 픽"    : "CycleJIGPick"    );  btMan1_1   -> Tag = (int)mcJIP_CycleJigPick   ; btMan1_1   -> Visible = true  ;
    btMan1_2   -> Caption = "[" + AnsiString(mcDIP_CycleDispenser ) + "]" + (bKorean ? "싸이클 디스펜서"   : "Cycle Dispenser" );  btMan1_2   -> Tag = (int)mcDIP_CycleDispenser ; btMan1_2   -> Visible = true  ;
    btMan1_3   -> Caption = "[" + AnsiString(mcPLC_CyclePlace     ) + "]" + (bKorean ? "싸이클 배출"       : "Cycle Place"     );  btMan1_3   -> Tag = (int)mcPLC_CyclePlace     ; btMan1_3   -> Visible = true  ;
    btMan1_7   -> Caption = "[" + AnsiString(mcSTG_AllSttClear    ) + "]" + (bKorean ? "전체 상태 초기화"  : "AllSttClear"     );  btMan1_7   -> Tag = (int)mcSTG_AllSttClear    ; btMan1_7   -> Visible = true  ;
    btMan1_8   -> Caption = "[" + AnsiString(mcSTG_IdxUpDn        ) + "]" + (bKorean ? "디스펜서 업/다운"  : "Dispenser Up/Dn" );  btMan1_8   -> Tag = (int)mcSTG_IdxUpDn        ; btMan1_8   -> Visible = true  ;
    btMan1_9   -> Caption = "[" + AnsiString(mcSTG_PshFwBw        ) + "]" + (bKorean ? "피커 전/후진"      : "Picker Fw/Bw"    );  btMan1_9   -> Tag = (int)mcSTG_PshFwBw        ; btMan1_9   -> Visible = true  ;
    btMan1_10  -> Caption = "[" + AnsiString(mcSTG_Vacuum         ) + "]" + (bKorean ? "배큠 On/Off"       : "Vacuum On/Off"   );  btMan1_10  -> Tag = (int)mcSTG_Vacuum         ; btMan1_10  -> Visible = true  ;
    btMan1_11  -> Caption = "[" + AnsiString(mcSTG_Eject          ) + "]" + (bKorean ? "이젝트 On/Off"     : "Eject On/Off"    );  btMan1_11  -> Tag = (int)mcSTG_Eject          ; btMan1_11  -> Visible = true  ;
    //btMan1_12  -> Caption = "[" + AnsiString(mcSTG_Eject       ) + "]" + (bKorean ? "피커 이젝트"       : "Clamp Dn/Up"       );  btMan1_12  -> Tag = (int)mcLDR_ClampDnUp    ; btMan1_12  -> Visible = true  ;
    //btMan1_13  -> Caption = "[" + AnsiString(mcLDR_TranInAC    ) + "]" + (bKorean ? "진입 AC모터"       : "Feed In On/Off"    );  btMan1_13  -> Tag = (int)mcLDR_TranInAC     ; btMan1_13  -> Visible = true  ;
    //btMan1_14 -> Caption = "[" + AnsiString(mcLDR_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"       : "Feed Out On/Off"   );  btMan1_14 -> Tag = (int)mcLDR_TranOutAC    ; btMan1_14 -> Visible = true  ;

    btCycleJigPick   -> Tag = (int)mcJIP_CycleJigPick   ;
    btCycleDispenser -> Tag = (int)mcDIP_CycleDispenser ;
    btCyclePlace     -> Tag = (int)mcPLC_CyclePlace     ;

    AnsiString sMotrPanel;
    TPanel * pMotrJog ;

    for(int i = 0 ; i < MAX_MOTR ; i++) {
        sMotrPanel = "pnMotrJog" + AnsiString(i) ;
        pMotrJog = ((TPanel*)FindComponent(sMotrPanel)) ;
        if(pMotrJog != NULL) {
            FraMotor[i] = new TFraMotr(pMotrJog);
            FraMotor[i] -> SetIdType(i ,MT_GetDirType(i));
            FraMotor[i] -> Parent = pMotrJog  ;
            FraMotor[i] -> Show();
            FraMotor[i] -> SetUnit(utJog ,0 );
        }
    }


    Panel2    -> DoubleBuffered = true ;
    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;

    edInputJigPos -> Text = OM.DevOptn.dJigPCLOfs;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)                         // 오퍼레이션 화면을 한 번 보여줄 때마다, 이 함수를 탐
{
    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    //Button Start Enabled
    SPC.LOT.DispLotInfo(sgLotInfo);
    SPC.DAY.DispDayInfo(sgDayInfo);

    //EM_Disp
    AnsiString Str;
    static int iPreErrCnt  ;
    int        iCrntErrCnt ;
    for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
        if (EM_GetErr(i)) iCrntErrCnt++;
    }
    if (iPreErrCnt != iCrntErrCnt ) {
        lbErr -> Clear();
        AnsiString Str;
        lbErr -> Items -> Clear();
        int iErrNo = EM_GetLastErr();
        for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
            if (EM_GetErr(i)) {
                Str.sprintf("[ERR%03d]" , i) ;
                Str += EM_GetErrName(i) + AnsiString(" ") + EM_GetErrMsg(i);
                lbErr -> Items -> Add(Str);
                if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
            }
        }
    }
    iPreErrCnt = iCrntErrCnt ;

    pnDispnrSkip   -> Color    =  OM.CmnOptn.bDisprSkip   ? clLime : clRed ;

    pnDispnrSkip   -> Caption  =  OM.CmnOptn.bDisprSkip   ? "ON" : "OFF" ;

    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;

    //메뉴얼 버튼 색깔 칠하기.
    for(int i = 1 ; i < 6/*페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;
            if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
            else                             pBtn -> Font -> Color = clBlack ;
        }
    }

    for(int i = 0 ; i < MAX_ARAY ; i++) {
        DM.ARAY[i].UpdateAray();
    }

    //ChangeImage();
    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    FrmLotOpen -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    LTQ.LotEnd ();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

    if (FM_MsgYesNo("Confirm", "Would you like to invert the Option?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                           break ;
        case 1 : OM.CmnOptn.bDisprSkip     = !OM.CmnOptn.bDisprSkip      ; break ;
    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    SPC.DAY.ClearData() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btAllHomeClick(TObject *Sender)
{
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btWorkEndClick(TObject *Sender)
{
    //메거진 카운트 안맞았을때 빈메거진 넣으면 되는데 빈메거진 안넣게 해달라고 해서
    //이렇게 작업함.
    //메거진 카운트를 잘못 하여 4로 세팅을 했는데 3개만 넣고 그냥 강제 클리어 할때
    if(FM_MsgYesNo("Confirm","랏 대기리스트와 카운트가 클리어 하겠습니까?") != mrYes) return ;


    LTQ.DeleteLotNoAll();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::ChangeImage()
{
    AnsiString sPath = "D:\\BC6Works\\REC\\HRD_220M\\Bin\\Camera\\BMP\\";

    double dMTPos          = 0.0        ;
    static int iPreImgPage = 0          ;
    double dTemp = 0.0;
           int iImgPage    = iPreImgPage;

    if(JIP.GetSeqStep() != CJigPick::scIdle){
        if(AT_Complete(aiSTG_PshFwBw , ccBwd)){
            dMTPos   = (PM.GetValue(miSTG_ZPick , pvSTG_ZPickJIG) - PM.GetValue(miSTG_ZPick , pvSTG_ZPickWait)) / 5 ; //이미지 장수
            dTemp    = abs(MT_GetEncPos(miSTG_ZPick) - PM.GetValue(miSTG_ZPick , pvSTG_ZPickWait));
            iImgPage = dTemp / dMTPos ;
            iImgPage = iImgPage % 5 ;
            }
        else {
            dMTPos   = (PM.GetValue(miSTG_ZPick , pvSTG_ZPickWork) - PM.GetValue(miSTG_ZPick , pvSTG_ZPickWait)) / 4 ; //이미지 장수.
            dTemp    = abs(MT_GetEncPos(miSTG_ZPick) - PM.GetValue(miSTG_ZPick , pvSTG_ZPickWait)) ;      // 이전 이미지 장수.
            iImgPage = dTemp / dMTPos ;      // 이전 이미지 장수.
            iImgPage = iImgPage + 5;
        }
    }
    else if(DIP.GetSeqStep() != CDispnr::scIdle){
        if(AT_Complete(aiDSP_IdxUpDn , ccBwd)){
            dMTPos   = (PM.GetValue(miDSP_XDsps , pvDSP_XIndxEnd) - PM.GetValue(miDSP_XDsps , pvDSP_XIndxWait)) / 5 ;
            dTemp    = abs(MT_GetEncPos(miDSP_XDsps) - PM.GetValue(miDSP_XDsps , pvDSP_XIndxWait)) ;
            iImgPage = dTemp / dMTPos ;      // 이전 이미지 장수.
            iImgPage = iImgPage + 9 ;
        }
        else {
            dMTPos   = (PM.GetValue(miDSP_XDsps , pvDSP_XIndxEnd) - PM.GetValue(miDSP_XDsps , pvDSP_XIndxWait)) / 6 ;
            dTemp    = abs(PM.GetValue(miDSP_XDsps , pvDSP_XIndxEnd) - MT_GetEncPos(miDSP_XDsps)) ;
            iImgPage = dTemp / dMTPos ;
            iImgPage = iImgPage + 15 ;
        }
    }

    else if(PLC.GetSeqStep() != CPlace::scIdle){
        if(AT_Complete(aiSTG_PshFwBw , ccFwd)){
            dMTPos   = (PM.GetValue(miSTG_ZPick , pvSTG_ZPickPlace) - PM.GetValue(miSTG_ZPick, pvSTG_ZPickWait)) / 5 ;
            dTemp    = MT_GetEncPos(miSTG_ZPick);
            dTemp    = abs(MT_GetEncPos(miSTG_ZPick) - PM.GetValue(miSTG_ZPick , pvDSP_XIndxWait)) ;
            iImgPage = 5 - (dTemp / dMTPos) ;
            iImgPage = iImgPage + 20 ;
        }
        else {
            iImgPage = 0;
        }
    }

    if(iImgPage > MAX_PICTURE) iImgPage = 0;
    if(iImgPage != iPreImgPage){
        sPath = sPath + iImgPage + ".bmp";
        Img1 -> Picture -> LoadFromFile(sPath);
        iPreImgPage = iImgPage;
    }
}

void __fastcall TFrmOperation::btCycleJigPickClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btCycleDispenserClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btCyclePlaceClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::ImgTimerTimer(TObject *Sender)
{
    ImgTimer -> Enabled = false;
    ChangeImage();

    if(SEQ.bSEQEnd){
        tViewTime = Now() - SEQ.Seq_Time;
    }
    else{
        tViewTime = SEQ.End_Time - SEQ.Seq_Time;
    }

    pnCycleTime -> Caption = tViewTime.FormatString("hh:nn:ss") ; // hhh

    if(SEQ.Pt_Step.bIdle == true && SEQ.Pt_Step.bJigPick == false && SEQ.Pt_Step.bDispenser == false &&
       JIP.GetCycleStep() == CJigPick::scIdle && DIP.GetCycleStep() == CDispnr::scIdle && PLC.GetCycleStep() == CPlace::scIdle &&
       SEQ.m_bMsgView){
        SEQ.m_bMsgView = false;
        if(FM_MsgYesNo("Config", "지그 쪽 자재를 확인해주세요!") == mrYes) {
            SEQ.Pt_Step.bJigPick = true  ;
            SEQ.Pt_Step.bIdle    = false ;
            SEQ._bBtnStart = true;
        }
    }
    else if(SEQ.Pt_Step.bIdle == false && SEQ.Pt_Step.bJigPick == true && SEQ.Pt_Step.bDispenser == false &&
            JIP.GetCycleStep() == CJigPick::scIdle && DIP.GetCycleStep() == CDispnr::scIdle && PLC.GetCycleStep() == CPlace::scIdle &&
            SEQ.m_bMsgView){
        SEQ.m_bMsgView = false;
        if(FM_MsgYesNo("Config", "디스펜서 쪽 자재 위치를 조절하여 주세요!") == mrYes) {
            SEQ.Pt_Step.bDispenser = true ;
            SEQ.Pt_Step.bJigPick   = false;
            SEQ._bBtnStart = true;
        }

    }
    else if(SEQ.Pt_Step.bIdle == false && SEQ.Pt_Step.bJigPick == false && SEQ.Pt_Step.bDispenser == true &&
            JIP.GetCycleStep() == CJigPick::scIdle && DIP.GetCycleStep() == CDispnr::scIdle && PLC.GetCycleStep() == CPlace::scIdle &&
            SEQ.m_bMsgView){
        SEQ.m_bMsgView = false;
        if(FM_MsgYesNo("Config", "디스펜서 쪽 자재 위치를 확인하여 주세요!") == mrYes) {
            SEQ.Pt_Step.bPlace     = true;
            SEQ.Pt_Step.bDispenser = false;
            SEQ._bBtnStart = true;
        }
    }


    ImgTimer -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::rgJogUnitClick(TObject *Sender)
{
    for(int i=0;i<MAX_MOTR;i++) {
        if(rgJogUnit->ItemIndex == 0) FraMotor[i]->SetUnit(utJog ,0.0 );
        if(rgJogUnit->ItemIndex == 1) FraMotor[i]->SetUnit(utMove,1.0 );
        if(rgJogUnit->ItemIndex == 2) FraMotor[i]->SetUnit(utMove,0.5 );
        if(rgJogUnit->ItemIndex == 3) FraMotor[i]->SetUnit(utMove,0.1 );
        if(rgJogUnit->ItemIndex == 4) FraMotor[i]->SetUnit(utMove,0.05);
        if(rgJogUnit->ItemIndex == 5) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    }
}

//---------------------------------------------------------------------------



void __fastcall TFrmOperation::btInputJigPosClick(TObject *Sender)
{
    double dInputJigPos = 0.0;

    dInputJigPos = MT_GetCmdPos(miSTG_ZPick) - PM.GetValue(miSTG_ZPick , pvSTG_ZPickPlace);

    edInputJigPos -> Text = dInputJigPos;

    OM.DevOptn.dJigPCLOfs = dInputJigPos;

    OM.SaveDevOptn(OM.GetCrntDev());
}
//---------------------------------------------------------------------------





