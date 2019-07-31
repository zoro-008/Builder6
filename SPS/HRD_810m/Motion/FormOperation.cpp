//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormDeviceSet.h"
#include "FormLotOpen.h"
#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "SLogUnit.h"
#include "MapUnit.h"
#include "UtilDefine.h"
#include "FormImage.h"
#include "PstnMan.h"
#include "Head.h"
#include "Stage.h"
#include "UserINI.h"
#include "SpcSubUnit.h"
#include "Array.h"

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "FormVisnMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------

__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    if(!OM.MstOptn.bDebugMode) SEQ.Reset(false);
    EM_SetDisp(false);

    DM_Init();
    Man_Init();

    pcManual  -> ActivePageIndex = 0;
    pcControl -> ActivePageIndex = 0;

    //pcMap -> ActivePageIndex = 0;

    m_dUnit     = 0.0 ;
    m_dPitch    = 0.0 ;
    m_iUnitType = 0   ;



    SetUnitInit();

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::GetPanelScreenShot(TPanel * _pPanel, String _sPath)
{
    Graphics::TBitmap *Bm = new Graphics::TBitmap();

    tagRECT Rect ;
    GetWindowRect(_pPanel -> Handle , &Rect);

    HDC dc;
    LOGPALETTE *lpPal;

    Bm->Width  = Rect.right - Rect.left;
    Bm->Height = Rect.bottom - Rect.top;
    dc = GetDC(0); 
    if(dc == 0) return; 

    BitBlt(Bm->Canvas->Handle,0,0,Bm->Width,Bm->Height,dc,Rect.left,Rect.top,SRCCOPY);

    ReleaseDC(0,dc);


    Bm -> SaveToFile(_sPath);

    //m_sLastSaveData = _sPath ;

    delete Bm;
};

void TFrmOperation::DM_Init ()
{
    DM.ARAY[riCST].SetParent(pnCST ); DM.ARAY[riCST].SetConfig(false , "CST "); DM.ARAY[riCST].ClearDispItem();
    DM.ARAY[riSTG].SetParent(pnSTG ); DM.ARAY[riSTG].SetConfig(true  , "STG "); DM.ARAY[riSTG].ClearDispItem();
    DM.ARAY[riMAP].SetParent(pnMAP ); DM.ARAY[riMAP].SetConfig(true  , "MAP "); DM.ARAY[riMAP].ClearDispItem();
    DM.ARAY[riBRD].SetParent(pnBRD ); DM.ARAY[riBRD].SetConfig(false , "BRD "); DM.ARAY[riBRD].ClearDispItem();

    DM.ARAY[riCST].SetDispItem(csNone   , "자제없음"   ) ;
    DM.ARAY[riCST].SetDispItem(csUnkwn  , "작업전"     ) ;
    DM.ARAY[riCST].SetDispItem(csMask   , "작업중"     ) ;
    DM.ARAY[riCST].SetDispItem(csEmpty  , "빈슬롯"     ) ;
    DM.ARAY[riCST].SetDispItem(csWork   , "작업완료"   ) ;

    DM.ARAY[riSTG].SetDispItem(csNone   , "자제없음"   ) ;
    DM.ARAY[riSTG].SetDispItem(csFail   , "불량자재"   ) ;
    DM.ARAY[riSTG].SetDispItem(csGood   , "정상자재"   ) ;
    DM.ARAY[riSTG].SetDispItem(csWait   , "WF비전대기" ) ;
    DM.ARAY[riSTG].SetDispItem(csWork   , "작업완료"   ) ;
    DM.ARAY[riSTG].SetDispItem(csEror   , "외각자재"   ) ;

    DM.ARAY[riMAP].SetDispItem(csNone   , "자제없음"   ) ;
    DM.ARAY[riMAP].SetDispItem(csFail   , "불량자재"   ) ;
    DM.ARAY[riMAP].SetDispItem(csWait   , "WF비전대기" ) ;
    DM.ARAY[riMAP].SetDispItem(csGood   , "정상자재"   ) ;
    DM.ARAY[riMAP].SetDispItem(csWork   , "작업완료"   ) ;
    DM.ARAY[riMAP].SetDispItem(csEror   , "외각자재"   ) ;

    DM.ARAY[riBRD].SetDispItem(csNone   ,"자제없음"   ) ;  //Marking 하기 전. 대기 상황. 위치 Board
    DM.ARAY[riBRD].SetDispItem(csUnkwn  ,"작업전"     ) ;  //Marking 안한 위치 Board
    DM.ARAY[riBRD].SetDispItem(csWork   ,"작업끝"     ) ;  //Marking 한 위치 Board

    //PopupMenuEnable
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        DM.ARAY[i].PopupMenuEnable(true);
    }
    DM.ARAY[riCST].PopupMenuEnable(true);
    DM.ARAY[riSTG].PopupMenuEnable(true);
    DM.ARAY[riMAP].PopupMenuEnable(true);
    DM.ARAY[riBRD].PopupMenuEnable(true);
}
//---------------------------------------------------------------------------

void TFrmOperation::Man_Init()
{
    //Manual Button Caption
    String sTemp ;
    int    iTag = 0 ;

    iTag = (int)mcSTG_CycleHome      ; sTemp = "[" + AnsiString(iTag ) + "]" + "스테이지 홈"        ; SetManualBtn(btMan1_1 , sTemp , iTag) ;
    iTag = (int)mcSTG_CycleSupply    ; sTemp = "[" + AnsiString(iTag ) + "]" + "웨이퍼 공급"        ; SetManualBtn(btMan1_2 , sTemp , iTag) ;
    iTag = (int)mcSTG_CycleExpend    ; sTemp = "[" + AnsiString(iTag ) + "]" + "웨이퍼 고정"        ; SetManualBtn(btMan1_3 , sTemp , iTag) ;
    iTag = (int)mcSTG_CyclePlace     ; sTemp = "[" + AnsiString(iTag ) + "]" + "웨이퍼 배출"        ; SetManualBtn(btMan1_4 , sTemp , iTag) ;
    iTag = (int)mcSTG_CycleUnExpend  ; sTemp = "[" + AnsiString(iTag ) + "]" + "웨이퍼 해제"        ; SetManualBtn(btMan1_5 , sTemp , iTag) ;
    iTag = (int)mcSTG_WfrAlgn        ; sTemp = "[" + AnsiString(iTag ) + "]" + "웨이퍼 레일 얼라인" ; SetManualBtn(btMan1_6 , sTemp , iTag) ;


    iTag = (int)mcHED_CycleHome      ; sTemp = "[" + AnsiString(iTag ) + "]" + "해드 홈      " ; SetManualBtn(btMan2_1 , sTemp , iTag) ;

    iTag = (int)mcHED_CycleTAlign    ; sTemp = "[" + AnsiString(iTag ) + "]" + "웨이퍼T 얼라인"; SetManualBtn(btMan2_1 , sTemp , iTag) ;
    iTag = (int)mcHED_CycleOrigin    ; sTemp = "[" + AnsiString(iTag ) + "]" + "오리진"        ; SetManualBtn(btMan2_2 , sTemp , iTag) ;
    //iTag = (int)mcHED_CycleChipAlign ; sTemp = "[" + AnsiString(iTag ) + "]" + "칩 얼라인"     ; SetManualBtn(btMan2_3 , sTemp , iTag) ;
    iTag = (int)mcHED_CycleMark      ; sTemp = "[" + AnsiString(iTag ) + "]" + "마킹"          ; SetManualBtn(btMan2_3 , sTemp , iTag) ;
    iTag = (int)mcHED_CycleCntrMove  ; sTemp = "[" + AnsiString(iTag ) + "]" + "센터무브"      ; SetManualBtn(btMan2_4 , sTemp , iTag) ;
    iTag = (int)mcHED_CycleCntrMoveT ; sTemp = "[" + AnsiString(iTag ) + "]" + "센터무브T"     ; SetManualBtn(btMan2_5 , sTemp , iTag) ;


    iTag = (int)mcHED_CycleCal       ; sTemp = "[" + AnsiString(iTag ) + "]" + "캘리브레이션"  ; SetManualBtn(btMan3_1 , sTemp , iTag) ;
    iTag = (int)mcHED_CyclePenClearPs; sTemp = "[" + AnsiString(iTag ) + "]" + "보드 클린 위치"; SetManualBtn(btMan3_2 , sTemp , iTag) ;
    iTag = (int)mcETC_LampOnOff      ; sTemp = "[" + AnsiString(iTag ) + "]" + "램프 On / Off" ; SetManualBtn(btMan3_7 , sTemp , iTag) ;
    iTag = (int)mcHED_CyclePenCheck  ; sTemp = "[" + AnsiString(iTag ) + "]" + "펜체크"        ; SetManualBtn(btMan3_8 , sTemp , iTag) ;
    iTag = (int)mcHED_CyclePenAlign  ; sTemp = "[" + AnsiString(iTag ) + "]" + "펜체크 보정"   ; SetManualBtn(btMan3_9 , sTemp , iTag) ;
    iTag = (int)mcHED_CyclePenChangPs; sTemp = "[" + AnsiString(iTag ) + "]" + "펜 교체 위치"  ; SetManualBtn(btMan3_A , sTemp , iTag) ;
    iTag = (int)mcHED_CyclePenCoverPs; sTemp = "[" + AnsiString(iTag ) + "]" + "펜 커버"       ; SetManualBtn(btMan3_B , sTemp , iTag) ;

    iTag = (int)mcSTG_VaccumOnOff    ; sTemp = "[" + AnsiString(iTag ) + "]" + "베큠 On / Off"       ; SetManualBtn(btMan1_C , sTemp , iTag) ;
    iTag = (int)mcSTG_VaccumOnOff    ; sTemp = "[" + AnsiString(iTag ) + "]" + "베큠 On / Off"       ; SetManualBtn(btMan2_C , sTemp , iTag) ;
    iTag = (int)mcETC_MainAirOnOff   ; sTemp = "[" + AnsiString(iTag ) + "]" + "이온아이저 On / Off" ; SetManualBtn(btMan1_D , sTemp , iTag) ;
    iTag = (int)mcETC_MainAirOnOff   ; sTemp = "[" + AnsiString(iTag ) + "]" + "이온아이저 On / Off" ; SetManualBtn(btMan2_D , sTemp , iTag) ;

    iTag = (int)mcAllHome            ; sTemp = "[" + AnsiString(iTag ) + "]" + "모든 홈      " ; SetManualBtn(btMan1_E , sTemp , iTag) ;
    iTag = (int)mcAllHome            ; sTemp = "[" + AnsiString(iTag ) + "]" + "모든 홈      " ; SetManualBtn(btMan2_E , sTemp , iTag) ;
    iTag = (int)mcAllHome            ; sTemp = "[" + AnsiString(iTag ) + "]" + "모든 홈      " ; SetManualBtn(btMan3_E , sTemp , iTag) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::SetManualBtn(TBitBtn *_btn , String _sCaption , int _iTag)
{
    _btn -> Caption = _sCaption.Trim() ;
    _btn -> Tag     = _iTag     ;
    _btn -> Visible = true      ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    if(SEQ._bRun) {
        FM_MsgTime("Warning","장비가 정지상태가 아닙니다.",500) ;
        return ;
    }

    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);

    AnsiString sTemp;
    sTemp = MM.GetManNo();
    Trace("Operator", ("Operation Form_" + sTemp + "Manual Action").c_str()) ;
}
//---------------------------------------------------------------------------

void TFrmOperation::LV_Display()
{
    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }
}
//---------------------------------------------------------------------------

void TFrmOperation::LT_Display()
{
    //pnLotFailChip -> Caption = DM.ARAY[riSTG].GetCntStat(csFail);
    LT.DispLotInfo(
        pnLotNo        ,             
        pnLotJobFile   ,             
        NULL           ,             
        pnLotStartTime ,             
        NULL           ,             
        pnLotRunTime   ,             
        NULL           ,             
        NULL           ,             
        NULL           ,             
        pnLotWaferUpeh ,             
        pnLotWaferUph  ,             
        NULL           ,             
        pnLotWorkChip  ,             
        pnLotFailChip  );            

    LT.DispDayInfo(
        pnDayRunTime    ,
        pnDayIdleTime   ,
        pnDayJamTime    ,
        pnDayTotalTime  ,
        pnLotWaferUpeh  ,
        pnDayWaferUph   ,
        pnDayLotCount   ,
        pnDayMarkChip   ,
        pnDayFailChip   );
}
//---------------------------------------------------------------------------

void TFrmOperation::OP_Dispaly()
{
    pnOptn0 -> Color = OM.CmnOptn.bLoadingStop  ? clLime : clRed ; pnOptn0 -> Caption = OM.CmnOptn.bLoadingStop ? "ON" : "OFF" ;
    pnOptn1 -> Color = OM.CmnOptn.bUseBarcode   ? clLime : clRed ; pnOptn1 -> Caption = OM.CmnOptn.bUseBarcode  ? "ON" : "OFF" ;
    pnOptn2 -> Color = OM.CmnOptn.bDoorSkip     ? clLime : clRed ; pnOptn2 -> Caption = OM.CmnOptn.bDoorSkip    ? "ON" : "OFF" ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnOptn1Click(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

//    if(FM_GetLevel() == lvOperator) return ;

    AnsiString sTemp;
    sTemp = iIndex;
    Trace("Operator", ("Option Button_" + sTemp + " Click").c_str()) ;

    switch(iIndex) {
        case 0 :                                                       break ;
        case 1 :                                                       break ;
        case 2 :                                                       break ;
        case 3 :                                                       break ;
        case 4 :                                                       break ;
        case 5 :                                                       break ;
        case 6 :                                                       break ;
        case 7 :                                                       break ;
    }

    //if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 : OM.CmnOptn.bLoadingStop  = !OM.CmnOptn.bLoadingStop ; break ;
        case 1 : OM.CmnOptn.bUseBarcode   = !OM.CmnOptn.bUseBarcode  ; break ;
        case 2 : OM.CmnOptn.bDoorSkip     = !OM.CmnOptn.bDoorSkip    ; break ;
    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true ;
    Trace("Operator", "Operation Form_Start Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
    Trace("Operator", "Operation Form_Stop Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;
    Trace("Operator", "Operation Form_Reset Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
    Trace("Operator", "Operation Form_Operator Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
    Trace("Operator", "Operation Form_Password Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
    Trace("Operator", "Operation Form_Oper Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;
    tmUpdateAray->Enabled = true ;

    VSM.LiveAllOff(true);
    VSM.SetParentVisnMain(pnVisnBase) ;
    VSM.SetParentVisnPara(NULL);
    VSM.SetParentAlgoPara(NULL);
    VSM.SetSettingMode(false);

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
    tmUpdateAray->Enabled = false ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    tmUpdateAray -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSourceClick(TObject *Sender)
{
    String sDir = OM.CmnOptn.sSourcePath ;

    if(!DirectoryExists(sDir)) UserFile.CreateDir(sDir.c_str());

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

    Trace("Operator", "Operation Form_Sourece Button Click") ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btWorkedClick(TObject *Sender)
{
    String sDir = OM.CmnOptn.sWorkedPath ;

    if(!DirectoryExists(sDir)) UserFile.CreateDir(sDir.c_str());

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

    Trace("Operator", "Operation Form_Worked Button Click") ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
//    FrmLotOpen -> Show();
    if(FM_MsgYesNo("Confirm","랏 작업 갯수를 Clear하겠습니까?") != mrYes) return ;
    Trace("Operator", "Operation Form_LotOpen Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","강제 랏엔드를 하겠습니까?") != mrYes) return ;

    String sLotNo = LT.GetCrntLot() ;
    Trace("btLotEndClick",sLotNo.c_str());
    Trace("Operator", "Operation Form_LotEnd Button Click") ;
    LT.LotEnd();
}
//---------------------------------------------------------------------------

void TFrmOperation::SetPitch(double _dUnit)
{
    m_dUnit     = _dUnit     ;
}
//---------------------------------------------------------------------------

void TFrmOperation::SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit)
{
    m_dUnit     = _dUnit     ;
    m_iUnitType = _iUnitType ;
}
//---------------------------------------------------------------------------
void TFrmOperation::SetUnitInit()
{
    if(rgJogUnit->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnit->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnit->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnit->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnit->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnit->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnit->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    if(rgJogUnit->ItemIndex == 7) SetUnit(utPitch,0  );

    btSTG_TUc -> Tag = (int)miSTG_TStg ;
    btSTG_TCl -> Tag = (int)miSTG_TStg ;
    btCST_ZUp -> Tag = (int)miSTG_ZCst ;
    btCST_ZDn -> Tag = (int)miSTG_ZCst ;
    btTRS_XLf -> Tag = (int)miSTG_XTrs ;
    btTRS_XRt -> Tag = (int)miSTG_XTrs ;

    btHED_XLt -> Tag = (int)miHED_XHed ;
    btHED_XRt -> Tag = (int)miHED_XHed ;
    btHED_YUp -> Tag = (int)miHED_YHed ;
    btHED_YDn -> Tag = (int)miHED_YHed ;
    btHED_TCl -> Tag = (int)miHED_TZom ;
    btHED_TUc -> Tag = (int)miHED_TZom ;
    btHed_ZUp -> Tag = (int)miHED_ZVsn ;
    btHed_ZDn -> Tag = (int)miHED_ZVsn ;

    btTRS_CmpBw -> Tag = (int)aiSTG_TrsCmpGR   ;
    btTRS_CmpFw -> Tag = (int)aiSTG_TrsCmpGR   ;
    btHED_PenBw -> Tag = (int)aiHED_PenMrkDU   ;
    btHED_PenFw -> Tag = (int)aiHED_PenMrkDU   ;
    btSTG_ChkBw -> Tag = (int)aiSTG_ChkTblUD   ;
    btSTG_ChkFw -> Tag = (int)aiSTG_ChkTblUD   ;
    btHed_CovDn -> Tag = (int)aiHED_PenCoverUD ;
    btHed_CovUp -> Tag = (int)aiHED_PenCoverUD ;
    btSTG_WfBw  -> Tag = (int)aiSTG_WfrAlgnFB  ;
    btSTG_WfFw  -> Tag = (int)aiSTG_WfrAlgnFB  ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::rgJogUnitClick(TObject *Sender)
{
    if(rgJogUnit->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnit->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnit->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnit->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnit->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnit->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnit->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    if(rgJogUnit->ItemIndex == 7) SetUnit(utPitch,0  );

    AnsiString sTemp;
    sTemp = rgJogUnit -> ItemIndex;
    Trace("Operator", ("Operation Form_JogUnit_" + sTemp + "Click").c_str()) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btJogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

         if(m_iId == miSTG_ZCst) {m_dPitch = OM.DevInfo.dCsSlotPitch ; sTemp = "STG_ZCst";}
    //else if(m_iId == miHED_XHed) {m_dPitch = OM.DevInfo.dWfPitchX     ; sTemp = "HED_XHed";}//OM.DevInfo.dWfBldOfs 수정후 안들어 가는지..JS;
    //else if(m_iId == miHED_YHed) {m_dPitch = OM.DevInfo.dWfPitchY    ; sTemp = "HED_YHed";}//OM.DevInfo.dWfBldOfs 수정후 안들어 가는지..JS;
    else if(m_iId == miHED_XHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchX  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchX ; sTemp = "HED_XHed"; }
    }//+ OM.DevInfo.dWfBldOfs 수정후 안들어 가는지..JS;
    else if(m_iId == miHED_YHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchY  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchY ; sTemp = "HED_XHed"; }
    }//+ OM.DevInfo.dWfBldOfs 수정후 안들어 가는지..JS;
         if(m_iUnitType == utJog   ) MT_JogN    ((EN_MOTR_ID)m_iId            ) ;
    else if(m_iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId , -m_dUnit ) ;
    else if(m_iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId , -m_dPitch) ;

    Trace("Operator", ("Operation Form_" + sTemp + "_Motor_N Move").c_str()) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogNMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;

    if(m_iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;

    Trace("Operator", "Operation Form_Motor_N Move Stop") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

         if(m_iId == miSTG_ZCst) {m_dPitch = OM.DevInfo.dCsSlotPitch ; sTemp = "STG_ZCst";}
    else if(m_iId == miHED_XHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchX  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchX ; sTemp = "HED_XHed"; }
    }//+ OM.DevInfo.dWfBldOfs 수정후 안들어 가는지..JS;
    else if(m_iId == miHED_YHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchY  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchY ; sTemp = "HED_YHed"; }
    }//+ OM.DevInfo.dWfBldOfs 수정후 안들어 가는지..JS;

         if(m_iUnitType == utJog   ) MT_JogP    ((EN_MOTR_ID)m_iId            ) ;
    else if(m_iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  m_dUnit ) ;
    else if(m_iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  m_dPitch) ;

    Trace("Operator", ("Operation Form_" + sTemp + "_Motor_P Move").c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    if(m_iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;
    Trace("Operator", "Operation Form_Motor_P Move Stop") ;
}
//---------------------------------------------------------------------------

typedef void (__closure *Func)();
void TFrmOperation::UpdateAray()
{
//    DM.UpdateAray();
}
void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
//    Trace("Operation Form","Update Start");


    tmUpdate->Enabled = false;

    DM.m_iTempDpCnt1 = 1;
    SEQ.m_dLastTime = Now();

    //Oper Level Display
    DM.m_iTempDpCnt1 = 2;
    LV_Display();

    //Lot Info Display
    DM.m_iTempDpCnt1 = 3;
    LT_Display();

    //Error Display
    DM.m_iTempDpCnt1 = 4;
    EM_DisplayList(lbErr);

    //Option Display
    DM.m_iTempDpCnt1 = 5;
    OP_Dispaly();

    //DataMan Update
    //CRunThread<Func> * func = new CRunThread<Func>(&UpdateAray) ;

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    //Button Start Enabled
    DM.m_iTempDpCnt1 = 6;

    btTRS_CmpBw -> Font -> Color = AT_Complete(aiSTG_TrsCmpGR   , ccBwd) ? clLime : clBlack ;
    btTRS_CmpFw -> Font -> Color = AT_Complete(aiSTG_TrsCmpGR   , ccFwd) ? clLime : clBlack ;
    btHED_PenBw -> Font -> Color = AT_Complete(aiHED_PenMrkDU   , ccBwd) ? clLime : clBlack ;
    btHED_PenFw -> Font -> Color = AT_Complete(aiHED_PenMrkDU   , ccFwd) ? clLime : clBlack ;
    btSTG_ChkBw -> Font -> Color = AT_Complete(aiSTG_ChkTblUD   , ccBwd) ? clLime : clBlack ;
    btSTG_ChkFw -> Font -> Color = AT_Complete(aiSTG_ChkTblUD   , ccFwd) ? clLime : clBlack ;
    btHed_CovDn -> Font -> Color = AT_Complete(aiHED_PenCoverUD , ccBwd) ? clLime : clBlack ;
    btHed_CovUp -> Font -> Color = AT_Complete(aiHED_PenCoverUD , ccFwd) ? clLime : clBlack ;
    btSTG_WfBw  -> Font -> Color = AT_Complete(aiSTG_WfrAlgnFB  , ccBwd) ? clLime : clBlack ;
    btSTG_WfFw  -> Font -> Color = AT_Complete(aiSTG_WfrAlgnFB  , ccFwd) ? clLime : clBlack ;





    //TODO :: 나중에 수정 JS
    DM.m_iTempDpCnt1 = 7;
    lbWfrGoodCnt -> Caption = DM.ARAY[riSTG].GetCntStat(csGood );
    lbWfrFailCnt -> Caption = DM.ARAY[riSTG].GetCntStat(csFail );
    lbWfrMaskCnt -> Caption = DM.ARAY[riSTG].GetCntStat(csEror );
    lbWfrWorkCnt -> Caption = DM.ARAY[riSTG].GetCntStat(csWork );

    lbGoodCnt    -> Caption = DM.ARAY[riMAP].GetCntStat(csGood );
    lbFailCnt    -> Caption = DM.ARAY[riMAP].GetCntStat(csFail );
    lbMarkCnt    -> Caption = DM.ARAY[riMAP].GetCntStat(csEror );

    //TODO :: 나중에 수정 JS
    //AnsiString sTemp2 ;
    //sTemp2 = sTemp2.sprintf("%.3f",HED.m_dPickCycleTime);
    //pnPickCycleTime -> Caption    = sTemp2 ;

    //Theta Set 관련.
    //btSetT   -> Enabled = HED.Rqst.bNeedTAlign ;

    //초기 위치 관련.
    DM.m_iTempDpCnt1 = 8;
    static bool bPreNeedOriDataSet = false ;
    if(HED.m_bNeedOriDataSet && !bPreNeedOriDataSet) {
        if(OM.DevOptn.iOriMathod == omFiducial) VSM.SetActiveVisn(viWfo);
        else                                    VSM.SetActiveVisn(viWfr);

        pcControl -> ActivePageIndex = 3 ;
        rgJogUnit -> ItemIndex = 7 ;

        FM_MsgOk("CHECK","기준점을 확인해주세요");
    }
    //btManOri -> Enabled = HED.Rqst.bNeedOrigin ;
    DM.m_iTempDpCnt1 = 9 ;
    spManOri -> Visible = SEQ._bFlick && HED.m_bNeedOriDataSet   ;
    bPreNeedOriDataSet = HED.m_bNeedOriDataSet ;

    //마킹에러 관련.
    static bool bPreNeedMarkTrace = false ;
    if(HED.Rqst.bNeedMarkTrace && !bPreNeedMarkTrace) {
        pcControl -> ActivePageIndex = 3 ;
        rgJogUnit -> ItemIndex = 7 ;
        VSM.SetActiveVisn(viWfp);
    }
    bPreNeedMarkTrace = HED.Rqst.bNeedMarkTrace ;
    bool bStartEnable = HED.Rqst.bNeedMarkTrace /*|| HED.Rqst.bNeedOrigin*/ ;
    spReMark   -> Visible = SEQ._bFlick && HED.Rqst.bNeedMarkTrace ;
    btReMark   -> Enabled = HED.Rqst.bNeedMarkTrace ;
    btTraceFwd -> Enabled = HED.Rqst.bNeedMarkTrace && HED.m_iMarkTraceCnt > 0                   ;
    btTraceBwd -> Enabled = HED.Rqst.bNeedMarkTrace && HED.m_iMarkTraceCnt < HED.MarkData.GetDataCnt() - 1;
    btStart    -> Enabled = !bStartEnable;


    //비젼 창 핸들링.
    DM.m_iTempDpCnt1 = 10;
    EN_VISN_ID iCrntActiveVisn = 0 ;
    static EN_VISN_ID iPreActiveVisn = 1 ;
    //if(SEQ._bRun) {
             if(HED.GetSeqStep() == CHead::scCal       || MM.GetManNo() == mcHED_CycleCal      ){iCrntActiveVisn = viCal ; }
        else if(HED.GetSeqStep() == CHead::scPenCheck  || MM.GetManNo() == mcHED_CyclePenCheck ){iCrntActiveVisn = viWfp ; }
        else if(HED.GetSeqStep() == CHead::scTAlign    || MM.GetManNo() == mcHED_CycleTAlign   ){iCrntActiveVisn = viWfr ; }
        else if(HED.GetSeqStep() == CHead::scOrigin    || MM.GetManNo() == mcHED_CycleOrigin   ){iCrntActiveVisn = OM.DevOptn.iOriMathod == 0 ? viWfo : viWfr ; }
        else if(HED.GetSeqStep() == CHead::scChipAlign || MM.GetManNo() == mcHED_CycleChipAlign){iCrntActiveVisn = viWfr ; }
        else if(HED.GetSeqStep() == CHead::scMark      || MM.GetManNo() == mcHED_CycleMark     ){iCrntActiveVisn = viWfp ; }
        else if(HED.GetSeqStep() == CHead::scMarkTrace                                         ){iCrntActiveVisn = viWfp ; }
        else                                                                                    {iCrntActiveVisn = iPreActiveVisn ; }

        if(iCrntActiveVisn != iPreActiveVisn) {
            VSM.SetActiveVisn(iCrntActiveVisn);
        }
        iPreActiveVisn = iCrntActiveVisn ;
    //}

    int iLevel;
    iLevel = FM_GetLevel();
    DM.m_iTempDpCnt1 = 11;
    if(iLevel == lvOperator){
        GroupBox1  -> Visible = false;
        GroupBox2  -> Visible = false;
        GroupBox3  -> Visible = false;
        GroupBox4  -> Visible = false;
        GroupBox6  -> Visible = false;
        GroupBox8  -> Visible = false;
        GroupBox9  -> Visible = false;
        GroupBox20 -> Visible = false;
        rgJogUnit  -> Visible = false;
        edJogUnit  -> Visible = false;
    }
    if(iLevel == lvEngineer){
        GroupBox1  -> Visible = true;
        GroupBox2  -> Visible = true;
        GroupBox3  -> Visible = true;
        GroupBox4  -> Visible = true;
        GroupBox6  -> Visible = true;
        GroupBox8  -> Visible = true;
        GroupBox9  -> Visible = true;
        GroupBox20 -> Visible = true;
        rgJogUnit  -> Visible = true;
        edJogUnit  -> Visible = true;
    }
    if(iLevel == lvMaster  ){
        GroupBox1  -> Visible = true;
        GroupBox2  -> Visible = true;
        GroupBox3  -> Visible = true;
        GroupBox4  -> Visible = true;
        GroupBox6  -> Visible = true;
        GroupBox8  -> Visible = true;
        GroupBox9  -> Visible = true;
        GroupBox20 -> Visible = true;
        rgJogUnit  -> Visible = true;
        edJogUnit  -> Visible = true;
    }

    lbPenCnt -> Caption = HED.m_iTotalPenDnCnt ;


    lbBar->Caption =  STG.Barcode->sData ;

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateArayTimer(TObject *Sender)
{
    tmUpdateAray->Enabled = false ;

    DM.UpdateAray();

    String sTemp = "Col=" + String(DM.ARAY[riSTG].GetMaxCol()) + " Row=" + String(DM.ARAY[riSTG].GetMaxRow()) ;

    Label1->Caption = sTemp ;



    tmUpdateAray->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStopHedClick(TObject *Sender)
{
    MT_Stop(miSTG_ZCst) ;
    MT_Stop(miSTG_XTrs) ;
    MT_Stop(miHED_XHed) ;
    MT_Stop(miHED_YHed) ;
    MT_Stop(miSTG_TStg) ;
    MT_Stop(miHED_ZVsn) ;
    MT_Stop(miHED_TZom) ;

    Trace("Operator", "Operation Form_MT_Stop Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSaveClick(TObject *Sender)
{
    //if(!OM.CmnOptn.bUseMapFile) return ;
    if(!sdMap->Execute()) return ;
    Trace("WriteArayData","1");
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;


    if( DM.ARAY[riSTG].GetLotNo() == "" ) sLotNo = "NoLot";
    else                                  sLotNo = DM.ARAY[riSTG].GetLotNo();

    iSlotNo = DM.ARAY[riSTG].GetID().ToIntDef(999999)%1000 ;
    iMgzNo  = DM.ARAY[riSTG].GetID().ToIntDef(999999)/1000 ;

    sPath = sdMap->FileName; //sPath + "\\" + sSlotNo + ".ini" ;

    for(int r = 0 ; r < OM.DevInfo.iWfRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iWfColCnt ; c++) {
            sTemp2 = (int)DM.ARAY[riSTG].GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riSTG].GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , DM.ARAY[riSTG].GetID().ToIntDef(999999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , DM.ARAY[riSTG].GetLotNo()             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iWfRowCnt             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iWfColCnt             );

    Trace("WriteArayData","2");
    Trace("Operator", "Operation Form_Data Save As Button Click") ;
}
//---------------------------------------------------------------------------


void TFrmOperation::PaintCallback() //Camera Callback
{
    //FrmImage -> OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btBRD_ClearClick(TObject *Sender)
{
    DM.ARAY[riBRD].SetStat(csUnkwn);
    Trace("Operator", "Operation Form_Board Clear Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSTG_ChkFwClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

         if(iSel == aiSTG_TrsCmpGR  ) bRet = STG.CheckSafe((EN_ACTR_ID)iSel , ccFwd);
    else if(iSel == aiHED_PenMrkDU  ) bRet = HED.CheckSafe((EN_ACTR_ID)iSel , ccFwd);
    else if(iSel == aiSTG_ChkTblUD  ) bRet = STG.CheckSafe((EN_ACTR_ID)iSel , ccFwd);
    else if(iSel == aiHED_PenCoverUD) bRet = HED.CheckSafe((EN_ACTR_ID)iSel , ccFwd);
    else if(iSel == aiSTG_WfrAlgnFB ) bRet = STG.CheckSafe((EN_ACTR_ID)iSel , ccFwd);

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccFwd) ;

    AnsiString sTemp;
    sTemp = iSel;
    Trace("Operator", ("Operation Form_" + sTemp + "_Cylinder Fw Click").c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSTG_ChkBwClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    bool bRet ;

         if(iSel == aiSTG_TrsCmpGR  ) bRet = STG.CheckSafe((EN_ACTR_ID)iSel , ccBwd);
    else if(iSel == aiHED_PenMrkDU  ) bRet = HED.CheckSafe((EN_ACTR_ID)iSel , ccBwd);
    else if(iSel == aiSTG_ChkTblUD  ) bRet = STG.CheckSafe((EN_ACTR_ID)iSel , ccBwd);
    else if(iSel == aiHED_PenCoverUD) bRet = HED.CheckSafe((EN_ACTR_ID)iSel , ccBwd);
    else if(iSel == aiSTG_WfrAlgnFB ) bRet = STG.CheckSafe((EN_ACTR_ID)iSel , ccBwd);

    if(!bRet) return ;

    AT_MoveCyl(iSel , ccBwd) ;
    AnsiString sTemp;
    sTemp = iSel;
    Trace("Operator", ("Operation Form_" + sTemp + "_Cylinder Bw Click").c_str()) ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btConvertOpenClick(TObject *Sender)
{

    //if(!OM.CmnOptn.bUseMapFile) return ;
    //odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
         if(OM.DevInfo.iMapType == 0) odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
    else if(OM.DevInfo.iMapType == 1) odMap -> Filter = "wmap형식|*.wmap|모든파일|*.*|" ;
    else if(OM.DevInfo.iMapType == 2) odMap -> Filter = "log2형식|*.log2|모든파일|*.*|" ;
    else                              odMap -> Filter = "w05형식|*.w05|모든파일|*.*|"   ;

    if(!odMap->Execute()) return ;

    //Local Var.
    String sPath = odMap->FileName;

    int iOriR  , iOriC ;



    if(OM.DevInfo.iMapType == 0) {
        if(OM.DevInfo.bMskExist){
            if(!MAPU.ChangeMaskNone(sPath , OM.DevInfo.iMapType , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;
        }
        else {
            Trace("Operator", "Device Set Form_Make Mark Click");
            if(!MAPU.ConvertCmn      (sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;
        }
    }
    else if(OM.DevInfo.iMapType == 1) {
        if(!MAPU.ConvertCoordi1  (sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;
        if(!MAPU.DeleteNullColRow(riMSK , riMAP)) return;
    }
    else if(OM.DevInfo.iMapType == 2){
        if(!MAPU.ConvertCoordi2  (sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;
    }
    else {
        if(!MAPU.ConvertCoordi8in(sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;
    }

    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;

    //DM.ARAY[riMAP].

         if(iFlatAngle == 0) {                           }/*0  */
    else if(iFlatAngle == 1) {DM.ARAY[riMAP].TurnCw90 ();}/*90 */
    else if(iFlatAngle == 2) {DM.ARAY[riMAP].TurnCw180();}/*180*/
    else if(iFlatAngle == 3) {DM.ARAY[riMAP].TurnCw270();}/*270*/





    if(!MAPU.MergeArray(riMSK , riMAP  , riSTG)){ FM_MsgOk("ERROR","마스크영역에 기준점을 확인 해주세요"); DM.ARAY[riSTG].SetStat(csNone); }

    if(OM.DevInfo.bMskExist && OM.DevInfo.iMapType == 0){ MAPU.ChangeMaskNone(sPath , OM.DevInfo.iMapType , riSTG); }


    if(OM.DevInfo.bMskExist) {
             if(iFlatAngle == 0) {                           }/*0  */
        else if(iFlatAngle == 1) {DM.ARAY[riSTG].TurnCw90 ();}/*90 */
        else if(iFlatAngle == 2) {DM.ARAY[riSTG].TurnCw180();}/*180*/
        else if(iFlatAngle == 3) {DM.ARAY[riSTG].TurnCw270();}/*270*/
    }





    String sLotName ;

    sLotName = ExtractFileName(odMap -> FileName);

    //sLotName.Delete(1, sLotName.Pos("Source\\"));
    //sLotName.Delete(1, sLotName.Pos("\\"));
    //sLotName.Delete(sLotName.Pos("-"), 30);

    DM.ARAY[riSTG].SetID   (sLotName);
    DM.ARAY[riSTG].SetLotNo(sLotName);
    LT.WriteArayData(riSTG);
    LT.LotInfo.sWaferName = sLotName;
    HED.m_iLastMrkChipR = 0 ;
    HED.m_iLastMrkChipC = 0 ;

    HED.MarkData.ClearData();
    HED.Rqst.bNeedMarkTrace = false ;

    STG.sMainWfName = sLotName ;

    Trace("Operator", "Operation Form_Convert Open Button Click") ;
    HED.Rqst.bNeedTAlign = true  ;
    HED.Rqst.bNeedOrigin = false ;

    //sun 20141022 화면 락되는 것 때문에 지워봄. DM.UpdateAray();

    mmMemo -> Lines -> Assign(MAPU.m_slRowList);


}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSaveAsClick(TObject *Sender)
{
    //if(!OM.CmnOptn.bUseMapFile) return ;

    sdMap -> Filter = "hrm한라맵형식|*.hrm" ;

    if(!sdMap->Execute()) return ;

    //Local Var.
    String sPath = sdMap->FileName;

    DM.ARAY[riSTG].LoadIni(false , sPath) ;
    Trace("Operator", "Operation Form_Save As Button Click") ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOpenAsClick(TObject *Sender)
{
    //if(!OM.CmnOptn.bUseMapFile) return ;
    odMap -> Filter = "hrm한라맵형식|*.hrm" ;

    if(!odMap->Execute()) return ;

    //Local Var.
    String sPath = odMap->FileName;

    DM.ARAY[riSTG].LoadIni(true , sPath) ;

    Trace("Operator", "Operation Form_Open As Button Click") ;

    HED.MarkData.ClearData();
    HED.Rqst.bNeedMarkTrace = false ;

    String sLotName ;

    sLotName = ExtractFileName(odMap -> FileName);

    STG.sMainWfName = sLotName ;

    HED.Rqst.bNeedTAlign = true  ;
    HED.Rqst.bNeedOrigin = false ;


}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMapOpenClick(TObject *Sender)
{
    //odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
         if(OM.DevInfo.iMapType == 0) odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
    else if(OM.DevInfo.iMapType == 1) odMap -> Filter = "wmap형식|*.wmap|모든파일|*.*|" ;
    else if(OM.DevInfo.iMapType == 2) odMap -> Filter = "log2형식|*.log2|모든파일|*.*|" ;
    else                              odMap -> Filter = "w05형식|*.w05|모든파일|*.*|" ;

    if(!odMap->Execute()) return ;

    //Local Var.
    String sPath = odMap->FileName;

         if(OM.DevInfo.iMapType == 0) {if(!MAPU.ConvertCmn      (sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;}
    else if(OM.DevInfo.iMapType == 1) {if(!MAPU.ConvertCoordi1  (sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;}
    else if(OM.DevInfo.iMapType == 2) {if(!MAPU.ConvertCoordi2  (sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;}
    else                              {if(!MAPU.ConvertCoordi8in(sPath , riMAP)) FM_MsgOk("ERROR","맵파일이나 DEVICE에 맵파일 정보를 확인 해주세요.") ;}

    Trace("Operator", "Operation Form_MapOpen Button Click") ;

    mmMemo -> Lines -> Assign(MAPU.m_slRowList);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btRawMapOpenClick(TObject *Sender)
{
    //odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
         if(OM.DevInfo.iMapType == 0) odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
    else if(OM.DevInfo.iMapType == 1) odMap -> Filter = "wmap형식|*.wmap|모든파일|*.*|" ;
    else if(OM.DevInfo.iMapType == 2) odMap -> Filter = "log2형식|*.log2|모든파일|*.*|" ;
    else                              odMap -> Filter = "w05형식|*.w05|모든파일|*.*|" ;

    for(int i=0; i<sgTmp->RowCount; i++) sgTmp->Cells[1][i] = "" ;
    UserFile.GridSearchFile(OM.CmnOptn.sSourcePath  , sgTmp , 0 , false);

    if(!MAPU.OpenRawMap(OM.CmnOptn.sSourcePath + "\\hanra.sinf" )) return;

    Trace("Operator", "Operation Form_RawMapOpen Button Click") ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::FormCreate(TObject *Sender)
{
    //MAPU.SetMemo(Memo1);//
    //MAPU.SetMemo(mmMemo) ;
}
//---------------------------------------------------------------------------







void __fastcall TFrmOperation::btManOriClick(TObject *Sender)
{
    int r,c ;
    DM.ARAY[riMSK].FindFrstColRow(csOrig,r,c);
    DM.ARAY[riSTG].SetTemp(r,c,0,MT_GetCmdPos(miHED_XHed));
    DM.ARAY[riSTG].SetTemp(r,c,1,MT_GetCmdPos(miHED_YHed));

    /*비전 테스트용.
    HED.m_dTempPosX = MT_GetCmdPos(miHED_XHed) ;
    HED.m_dTempPosY = MT_GetCmdPos(miHED_YHed) ;
    */

    VSM.LiveAllOff(true);

    HED.m_iLastInspChipC = c;
    HED.m_iLastInspChipR = r;
    HED.Rqst.bNeedOrigin = false ;
    HED.m_bNeedOriDataSet = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSetTClick(TObject *Sender)
{
    HED.Rqst.bNeedTAlign = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btSTG_ClearClickClick(TObject *Sender)
{
    DM.ARAY[riSTG].SetStat(csNone);
    DM.ARAY[riMAP].SetStat(csNone);
    mmMemo -> Lines -> Clear();
    Trace("Operator", "Operation Form_Stage Clear Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btRawMapClearClick(TObject *Sender)
{
    mmMemo -> Lines -> Clear();
    Trace("Operator", "Operation Form_Raw Map Clear Button Click") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btMAP_ClearClickClick(TObject *Sender)
{
    DM.ARAY[riMAP].SetStat(csNone);
    Trace("Operator", "Operation Form_MAP Clear Button Click") ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
//   STG.SendBarcodeMsg("LON\r");

//   Sleep(10);
    EM_SetErr(eiCST_Dispr);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------



void __fastcall TFrmOperation::btTraceBwdClick(TObject *Sender)
{
    MM.SetManCycle(mcHED_CycleMarkTraceBw) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btTraceFwdClick(TObject *Sender)
{
    MM.SetManCycle(mcHED_CycleMarkTraceFw) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btReMarkClick(TObject *Sender)
{
    CVisionMan::TRetResult VisnRslt;

    VSM.GetRslt(viWfp , &VisnRslt);
    if(VisnRslt.iRet != 0){
        if(mrYes != FM_MsgYesNo("Confirm" , "세팅자제 결과는 굿이여야 합니다.\r\n무시하고 진행하겠습니까?")) return ;
    }
    int r , c;

    //현재 위치이후에 찍었던것들 날려버린다.
    for(int i = 0 ; i < HED.m_iMarkTraceCnt ; i++){
        if(HED.MarkData.GetDataF(i)) {
            c = HED.MarkData.GetDataC(i);
            r = HED.MarkData.GetDataR(i);

            DM.ARAY[riSTG].SetStat(r, c, csFail);
        }
        else {
            c = HED.MarkData.GetDataC(i);
            r = HED.MarkData.GetDataR(i);

            DM.ARAY[riSTG].SetStat(r, c, csEror);
        }
    }

    HED.MarkData.ClearData(HED.m_iMarkTraceCnt);
    HED.Rqst.bNeedMarkTrace = false ;
}
//---------------------------------------------------------------------------






void __fastcall TFrmOperation::pnTControlClick(TObject *Sender)
{
/*

*/

}
//---------------------------------------------------------------------------







void __fastcall TFrmOperation::Panel20Click(TObject *Sender)
{
//AnsiString Barcode = "S2V3AWWX01-603,6z0d84,01\r";

//    Barcode.Delete(Barcode.Pos("\r"),1);

//    ShowMessage(Barcode);

}
//---------------------------------------------------------------------------







void __fastcall TFrmOperation::Panel15Click(TObject *Sender)
{
//    if(SEQ.iTempStep ==0) {
//        SEQ.iTempStep = 10 ;
//    }
//    else {
//        SEQ.iTempStep = 0 ;
//    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmOperation::Panel18Click(TObject *Sender)
{
//    SEQ.bShowErr = true ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Panel9Click(TObject *Sender)
{
 //   VSM.SetActiveVisn(0);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Panel11Click(TObject *Sender)
{
 //   VSM.SetActiveVisn(1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Panel13Click(TObject *Sender)
{
 //   STG.BarcodeLoadingTest();
}
//---------------------------------------------------------------------------



void __fastcall TFrmOperation::pnPenCntResetClick(TObject *Sender)
{

    HED.m_iTotalPenDnCnt = 0 ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btBarOnClick(TObject *Sender)
{
    STG.SendBarcodeMsg("LON/r");             //
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btBarOffClick(TObject *Sender)
{
    STG.SendBarcodeMsg("LOFF/r");
}
//---------------------------------------------------------------------------


