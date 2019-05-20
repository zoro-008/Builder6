
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormInform.h"
#include "UserINI.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "FormVisionSetUp.h"
#include "FormOperation.h"
#include "FormVisnMain.h"
#include "Sequence.h"
#include "ManualMan.h"
#include "Head.h"
#include "MapUnit.h"
#include "FormMapViewer.h"
//---------------------------------------------------------------------------
#include "Rs232Man.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "LabelS"
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
    //작명 방법 기술.
    //aiLDR_Stopper -> pnLDR_Stopper1 (1은 페이지 넘버)
    //miLDR_YCmp    -> pnLDR_YCmp1    (1은 페이지 넘버)

    PM.SetWindow(pnSTG_ZCstPs , miSTG_ZCst);
    PM.SetWindow(pnSTG_XTrsPs , miSTG_XTrs);
    PM.SetWindow(pnHED_XHedPs , miHED_XHed);
    PM.SetWindow(pnHED_YHedPs , miHED_YHed);
    PM.SetWindow(pnSTG_TStgPs , miSTG_TStg);
    PM.SetWindow(pnHED_ZVsnPs , miHED_ZVsn);
    PM.SetWindow(pnHED_TZomPs , miHED_TZom);

    DM.ARAY[riMSK].SetParent(pnMSK ); DM.ARAY[riMSK].SetConfig(true  , "MSK "); DM.ARAY[riMSK].ClearDispItem();


    DM.ARAY[riMSK].SetDispItem(csNone   , "자제없음"   ) ;
    DM.ARAY[riMSK].SetDispItem(csEror   , "외곽"       ) ;
    DM.ARAY[riMSK].SetDispItem(csOrig   , "기준칩"     ) ;
    DM.ARAY[riMSK].SetDispItem(csFail   , "불량자재"   ) ;
    DM.ARAY[riMSK].SetDispItem(csGood   , "정상자재"   ) ;

    DM.ARAY[riSET].SetDispItem(csNone   , "자제없음"   ) ;
    DM.ARAY[riSET].SetDispItem(csFail   , "불량자재"   ) ;
    DM.ARAY[riSET].SetDispItem(csWait   , "WF비전대기" ) ;
    DM.ARAY[riSET].SetDispItem(csGood   , "정상자재"   ) ;
    DM.ARAY[riSET].SetDispItem(csWork   , "작업완료"   ) ;
    DM.ARAY[riSET].SetDispItem(csEror   , "외각자재"   ) ;


    //Motor Window Set to Panel
    MtWindowSet();
    AtWindowSet();

    //IO Num -> Tag
    btWfr_Vac  -> Tag = (int)ySTG_WfrVac  ;
    btWfr_Vac2 -> Tag = (int)ySTG_WfrVac  ;

    //Cb Index                                                   object ediWFrstCol: TEdit


    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TEdit"))
        {
            ((TEdit*)FindComponent(Components[i]->Name))->ImeMode    = imDisable ;
            ((TEdit*)FindComponent(Components[i]->Name))->NumberOnly = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseApoint  = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseMinus   = true ;
        }
        else if(Components[i]->ClassNameIs("TPageControl"))
        {
            ((TPageControl*)FindComponent(Components[i]->Name))->ActivePageIndex = 0 ;
        }
    }

    edGoodStr1 -> NumberOnly = false ;
    edMaskStr1 -> NumberOnly = false ;
    edEmptStr1 -> NumberOnly = false ;

    edExtention -> NumberOnly = false ;

    //rgUnitUser
    rgUnitUser = new TRadioGroup(this);
    edUnitUser = new TEdit      (this);


    SetUnitInit();

    m_bFlatAngle = false;

    

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MtWindowSet()
{

    for(int i=0 ; i<MAX_MOTR ; i++) {
        FraMotor  [i] = new TFraMotr();
        FraMotor  [i] ->SetIdType(i ,MT_GetDirType(i));
        FraMotor  [i] ->Show();
    }
    MtWindowSetPitch();
}
void __fastcall TFrmDeviceSet::MtWindowSetPitch()
{
    FraMotor[miSTG_ZCst] ->SetPitch(OM.DevInfo.dCsSlotPitch);
    if(HED.m_bFnshCycTAlgn){
        FraMotor[miHED_XHed] ->SetPitch(HED.m_dRevisePitchX );//+ OM.DevInfo.dWfBldOfs) 수정 후 Ofs 값 안들어 가는지..JS;
        FraMotor[miHED_YHed] ->SetPitch(HED.m_dRevisePitchY );//+ OM.DevInfo.dWfBldOfs) 수정 후 Ofs 값 안들어 가는지..JS;
    }
    else{
        FraMotor[miHED_XHed] ->SetPitch(OM.DevInfo.dWfPitchX );//+ OM.DevInfo.dWfBldOfs) 수정 후 Ofs 값 안들어 가는지..JS;
        FraMotor[miHED_YHed] ->SetPitch(OM.DevInfo.dWfPitchY );//+ OM.DevInfo.dWfBldOfs) 수정 후 Ofs 값 안들어 가는지..JS;
    }


}
void __fastcall TFrmDeviceSet::AtWindowSet()
{
    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) {
        FraCylinder[i] = new TFraCyl();
        FraCylinder[i] ->SetIdType(i , AT_GetDirType(i));
        FraCylinder[i] ->Show();
    }
}
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "저장 하시겠습니까?") != mrYes)  return;

    //JobFileBackUp();

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    //VSM.SaveButtonClick();

    

    //DataMan SetMaxColRow
    DM.ARAY[riCST].SetMaxColRow(1                    , OM.DevInfo.iCsSlotCnt);
    //DM.ARAY[riSTG].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    DM.ARAY[riMAP].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    //DM.ARAY[riMSK].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    DM.ARAY[riTST].SetMaxColRow(OM.DevInfo.iWfColCnt , OM.DevInfo.iWfRowCnt );
    DM.ARAY[riBRD].SetMaxColRow(OM.DevInfo.iBdXCnt   , OM.DevInfo.iBdYCnt   );


    String sBinPath = EXE_FOLDER + "JobFile\\" + OM.m_sCrntDev + "\\MaskMap.dat";
    String sIniPath = EXE_FOLDER + "JobFile\\" + OM.m_sCrntDev + "\\MaskMap.ini";
    DM.ARAY[riMSK].Load(false , sBinPath , sIniPath , 0);

    MtWindowSetPitch();

    Trace("Operator", "Device Set Form_Save Button Click");

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::JobFileBackUp()
{
    String sSorc  = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    String sPath  = OM.MstOptn.sDeviceBackUpPath               + "\\" ;
    String SDest  = sPath + Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.ClearDirDate(sPath , Now() - 400);

    sPath += Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.CopyDir(sSorc , SDest);

    //Trace Log.
//    Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    btSaveDevice -> Enabled = !SEQ._bRun &&
                    pcDeviceSet -> ActivePageIndex != 2 ; //자꾸 비젼껄 이버튼 눌러서 이렇게 함.;//&& !FrmMain -> tmMainSet -> Enabled && !FrmMain -> tmSubSet -> Enabled ;

    btWfr_Vac     -> Caption = IO_GetY(ySTG_WfrVac) ? "ON" : "OFF" ;
    btWfr_Vac2    -> Caption = IO_GetY(ySTG_WfrVac) ? "ON" : "OFF" ;

    DM.UpdateAray();
    

    if(VSM.GetActiveVisn() == viWfr){btMoveToVisnPos -> Caption = "웨이퍼 세타위치로 이동" ;}
    if(VSM.GetActiveVisn() == viWfo){btMoveToVisnPos -> Caption = "맵핑기준칩 위치로 이동" ;}
    if(VSM.GetActiveVisn() == viCal){btMoveToVisnPos -> Caption = "캘패드 위치로 이동"     ;}
    if(VSM.GetActiveVisn() == viWfp){btMoveToVisnPos -> Caption = "펜마킹 패드위치로 이동" ;}

    if(VSM.GetActiveVisn() == viWfr){btVisnManual    -> Caption = "웨이퍼 세타보정" ;}
    if(VSM.GetActiveVisn() == viWfo){btVisnManual    -> Caption = "맵핑기준칩 찾기" ;}
    if(VSM.GetActiveVisn() == viCal){btVisnManual    -> Caption = "캘리브레이션"    ;}
    if(VSM.GetActiveVisn() == viWfp){btVisnManual    -> Caption = "펜마킹 보정"     ;}

    btVisnZMove         -> Visible = VSM.GetActiveVisn() == viWfr ;
    btVisnTMove         -> Visible = VSM.GetActiveVisn() == viWfr ;
    btVisnManual        -> Visible = VSM.GetActiveVisn() != viWfo ;
    btMoveToStgHwCenter -> Visible = VSM.GetActiveVisn() == viWfr ;
    btVsnZTSave         -> Visible = VSM.GetActiveVisn() == viWfr ;


    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo)
    {
        edCsSlotCnt     -> Text      = OM.DevInfo.iCsSlotCnt    ;
        edCsSlotPitch   -> Text      = OM.DevInfo.dCsSlotPitch  ;

        edWfPitchX      -> Text      = OM.DevInfo.dWfPitchX     ;
        edWfPitchY      -> Text      = OM.DevInfo.dWfPitchY     ;
        edWfColCnt      -> Text      = OM.DevInfo.iWfColCnt     ;
        edWfRowCnt      -> Text      = OM.DevInfo.iWfRowCnt     ;
        edGoodStr1      -> Text      = OM.DevInfo.sGoodStr1     ;
        edEmptStr1      -> Text      = OM.DevInfo.sEmptStr1     ;
        edMaskStr1      -> Text      = OM.DevInfo.sMaskStr1     ;
        ediWFrstRow     -> Text      = OM.DevInfo.iWFrstRow     ;
        ediWFrstCol     -> Text      = OM.DevInfo.iWFrstCol     ;
        edMskLine       -> Text      = OM.DevInfo.iMskLine      ;

        cbUseHex        -> Checked   = OM.DevInfo.bUseHex       ;
        cbMskExist      -> Checked   = OM.DevInfo.bMskExist     ;
        cbMapType       -> ItemIndex = OM.DevInfo.iMapType      ;
        rgFlatAngle     -> ItemIndex = OM.DevInfo.iFlatAngle    ;
        edExtention     -> Text      = OM.DevInfo.sExtention    ;

        edBdXCnt        -> Text      = OM.DevInfo.iBdXCnt       ;
        edBdXPch        -> Text      = OM.DevInfo.dBdXPch       ;
        edBdYCnt        -> Text      = OM.DevInfo.iBdYCnt       ;
        edBdYPch        -> Text      = OM.DevInfo.dBdYPch       ;
    }
    else
    {
        int        iTemp ; //비교 하기 위해서..JH
        double     dTemp ; //비교 하기 위해서..JH
        AnsiString sTemp ; //비교 하기 위해서..JH

        iTemp = edCsSlotCnt     -> Text.ToInt()    ; if (OM.DevInfo.iCsSlotCnt    != iTemp) { Trace("Operator" , "Both Dev_CsSlotCnt values are different."  ); }
        dTemp = edCsSlotPitch   -> Text.ToDouble() ; if (OM.DevInfo.dCsSlotPitch  != dTemp) { Trace("Operator" , "Both Dev_CsSlotPitch values are different."); }

        iTemp = edWfColCnt      -> Text.ToInt()    ; if (OM.DevInfo.iWfColCnt     != iTemp) { Trace("Operator" , "Both Dev_WfColCnt values are different."   ); }
        iTemp = edWfRowCnt      -> Text.ToInt()    ; if (OM.DevInfo.iWfRowCnt     != iTemp) { Trace("Operator" , "Both Dev_WfRowCnt values are different."   ); }
        iTemp = ediWFrstRow     -> Text.ToIntDef(0); if (OM.DevInfo.iWFrstRow     != iTemp) { Trace("Operator" , "Both Dev_WFrstRow values are different."   ); }
        iTemp = ediWFrstCol     -> Text.ToIntDef(0); if (OM.DevInfo.iWFrstCol     != iTemp) { Trace("Operator" , "Both Dev_WFrstCol values are different."   ); }
        iTemp = edMskLine       -> Text.ToIntDef(0); if (OM.DevInfo.iMskLine      != iTemp) { Trace("Operator" , "Both Dev_MskLine  values are different."   ); }

        dTemp = edWfPitchX      -> Text.ToDouble() ; if (OM.DevInfo.dWfPitchX     != dTemp) { Trace("Operator" , "Both Dev_WfWidth values are different."    ); }
        dTemp = edWfPitchY      -> Text.ToDouble() ; if (OM.DevInfo.dWfPitchY     != dTemp) { Trace("Operator" , "Both Dev_WfHeight values are different."   ); }

        sTemp = edGoodStr1      -> Text            ; if (OM.DevInfo.sGoodStr1     != sTemp) { Trace("Operator" , "Both Dev_GoodStr1 values are different."   ); }
        sTemp = edEmptStr1      -> Text            ; if (OM.DevInfo.sEmptStr1     != sTemp) { Trace("Operator" , "Both Dev_EmptStr1 values are different."   ); }
        sTemp = edMaskStr1      -> Text            ; if (OM.DevInfo.sMaskStr1     != sTemp) { Trace("Operator" , "Both Dev_MaskStr1 values are different."   ); }

        iTemp = cbUseHex        -> Checked         ; if (OM.DevInfo.bUseHex       != iTemp) { Trace("Operator" , "Both Dev_UseHex values are different."     ); }
        iTemp = cbMskExist      -> Checked         ; if (OM.DevInfo.bMskExist     != iTemp) { Trace("Operator" , "Both Dev_MaskExist values are different."  ); }
        iTemp = cbMapType       -> ItemIndex       ; if (OM.DevInfo.iMapType      != iTemp) { Trace("Operator" , "Both Dev_MapType values are different."    ); }
        iTemp = rgFlatAngle     -> ItemIndex       ; if (OM.DevInfo.iFlatAngle    != iTemp) { Trace("Operator" , "Both Dev_FlatAngle values are different."  ); }
        sTemp = edExtention     -> Text            ; if (OM.DevInfo.sExtention    != sTemp) { Trace("Operator" , "Both Dev_FlatAngle values are different."  ); }

        iTemp = edBdXCnt        -> Text.ToInt()    ; if (OM.DevInfo.iBdXCnt       != iTemp) { Trace("Operator" , "Both Dev_BdXCnt values are different."     ); }
        iTemp = edBdYCnt        -> Text.ToInt()    ; if (OM.DevInfo.iBdYCnt       != iTemp) { Trace("Operator" , "Both Dev_BdYCnt values are different."     ); }
        dTemp = edBdXPch        -> Text.ToDouble() ; if (OM.DevInfo.dBdXPch       != dTemp) { Trace("Operator" , "Both Dev_BdXPch values are different."     ); }
        dTemp = edBdYPch        -> Text.ToDouble() ; if (OM.DevInfo.dBdYPch       != dTemp) { Trace("Operator" , "Both Dev_BdYPch values are different."     ); }

        OM.DevInfo.iCsSlotCnt    = edCsSlotCnt     -> Text.ToInt()    ;
        OM.DevInfo.dCsSlotPitch  = edCsSlotPitch   -> Text.ToDouble() ;

        OM.DevInfo.iWfColCnt     = edWfColCnt      -> Text.ToInt()    ;
        OM.DevInfo.iWfRowCnt     = edWfRowCnt      -> Text.ToInt()    ;
        OM.DevInfo.iWFrstRow     = ediWFrstRow     -> Text.ToIntDef(0);
        OM.DevInfo.iWFrstCol     = ediWFrstCol     -> Text.ToIntDef(0);
        OM.DevInfo.iMskLine      = edMskLine       -> Text.ToIntDef(0);

        OM.DevInfo.dWfPitchX     = edWfPitchX      -> Text.ToDouble() ;
        OM.DevInfo.dWfPitchY     = edWfPitchY      -> Text.ToDouble() ;

        OM.DevInfo.sGoodStr1     = edGoodStr1      -> Text            ;
        OM.DevInfo.sEmptStr1     = edEmptStr1      -> Text            ;
        OM.DevInfo.sMaskStr1     = edMaskStr1      -> Text            ;

        OM.DevInfo.bUseHex       = cbUseHex        -> Checked         ;
        OM.DevInfo.bMskExist     = cbMskExist      -> Checked         ;
        OM.DevInfo.iMapType      = cbMapType       -> ItemIndex       ;
        OM.DevInfo.iFlatAngle    = rgFlatAngle     -> ItemIndex       ;
        OM.DevInfo.sExtention    = edExtention     -> Text            ;  

        OM.DevInfo.iBdXCnt       = edBdXCnt        -> Text.ToInt()    ;
        OM.DevInfo.iBdYCnt       = edBdYCnt        -> Text.ToInt()    ;
        OM.DevInfo.dBdXPch       = edBdXPch        -> Text.ToDouble() ;
        OM.DevInfo.dBdYPch       = edBdYPch        -> Text.ToDouble() ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        edVisnChkChipColCnt -> Text      = OM.DevOptn.iVisnChkChipColCnt ;
        edVisnChkChipRowCnt -> Text      = OM.DevOptn.iVisnChkChipRowCnt ;
        edPenCheckCnt       -> Text      = OM.DevOptn.iPenCheckCnt       ;
        cbOriMathod         -> ItemIndex = OM.DevOptn.iOriMathod         ;

        if(OM.DevOptn.bAutoOrigin){ cbAutoOrigin -> ItemIndex = 1 ;}
        else                      { cbAutoOrigin -> ItemIndex = 0 ;}

    }
    else
    {
        int        iTemp ;
        if(edPenCheckCnt       -> Text.ToIntDef(MAX_QUE_DATA_CNT) > MAX_QUE_DATA_CNT) edPenCheckCnt       -> Text = MAX_QUE_DATA_CNT ;

        iTemp = edVisnChkChipColCnt -> Text.ToInt(); if (OM.DevOptn.iVisnChkChipColCnt != iTemp) { Trace("Operator" , "Both Dev_VisnChkChipColCnt values are different."); }
        iTemp = edVisnChkChipRowCnt -> Text.ToInt(); if (OM.DevOptn.iVisnChkChipRowCnt != iTemp) { Trace("Operator" , "Both Dev_VisnChkChipRowCnt values are different."); }
        iTemp = edPenCheckCnt       -> Text.ToInt(); if (OM.DevOptn.iPenCheckCnt       != iTemp) { Trace("Operator" , "Both Dev_PenCheckCnt values are different."      ); }
        iTemp = cbOriMathod         -> ItemIndex   ; if (OM.DevOptn.iOriMathod         != iTemp) { Trace("Operator" , "Both Dev_OriMathod values are different."        ); }
        iTemp = false                              ; if (OM.DevOptn.bAutoOrigin        != iTemp) { Trace("Operator" , "Both Dev_AutoManual values are different."       ); }
        iTemp = true                               ; if (OM.DevOptn.bAutoOrigin        != iTemp) { Trace("Operator" , "Both Dev_AutoManual values are different."       ); }

        OM.DevOptn.iVisnChkChipColCnt = edVisnChkChipColCnt -> Text.ToInt() ;
        OM.DevOptn.iVisnChkChipRowCnt = edVisnChkChipRowCnt -> Text.ToInt() ;
        OM.DevOptn.iPenCheckCnt       = edPenCheckCnt       -> Text.ToInt() ;
        OM.DevOptn.iOriMathod         = cbOriMathod         -> ItemIndex    ;

        OM.DevOptn.bAutoOrigin        = cbAutoOrigin -> ItemIndex ;

         //Rs232 Dispensor -> Set Time
        //RSM.SetTime(OM.DevOptn.iEpxTime );

        UpdateDevOptn(true);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{


    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    pnVisnSet -> Visible = false ;

    tmUpdateTimer->Enabled = true;
//    pcDeviceSet -> ActivePageIndex = 0 ;


     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                             break ;
         case 1 : rgUnitUser = rgJogUnit1 ;
                  edUnitUser = edJogUnit1 ; break ;
         case 2 : rgUnitUser = rgJogUnit2 ;
                  edUnitUser = edJogUnit2 ; break ;
         case 3 :                           break ;
     }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );

    //pcVisionSet -> ActivePageIndex = 1 ;
    VSM.LiveAllOff(true);
    VSM.SetParentVisnMain(pnVision);
    VSM.SetParentVisnPara(pnVisnPara);
    VSM.SetParentAlgoPara(pnAlgoPara);
    VSM.SetSettingMode(true);
    //VSM.Live(VSM.GetActiveVisn() , false);

    if(cbMapType -> ItemIndex != 0){
        edGoodStr1  -> Visible = false ;  stGoodChr    -> Visible = false ;
        edEmptStr1  -> Visible = false ;  stEmptyChr   -> Visible = false ;
        ediWFrstRow -> Visible = false ;  stFstChipRow -> Visible = false ;
        ediWFrstCol -> Visible = false ;  stFstChipCol -> Visible = false ;
        edMaskStr1  -> Visible = false ;  stMarkChr    -> Visible = false ;
        //edMskLine   -> Visible = false ;  stMskLine    -> Visible = false ;

        sbAutoOrigin        -> Visible = false ; sbFrstCol           -> Visible = false ;
        sbEmptyStr          -> Visible = false ; sbFrstRow           -> Visible = false ;
        sbFlatAngle         -> Visible = false ; sbGoodStr           -> Visible = false ;
        sbMapType           -> Visible = false ; sbMaskStr           -> Visible = false ;
        sbPenCheckCnt       -> Visible = false ; sbOriMathod         -> Visible = false ;
        sbVisnChkChipRowCnt -> Visible = false ; sbVisnChkChipColCnt -> Visible = false ;
        sbMskLine           -> Visible = false ; lbSpace             -> Visible = false ;

        cbUseHex            -> Visible = false ; btHexConvert        -> Visible = false ;
        //cbMskExist          -> Visible = false ;
    }
    else {
        edGoodStr1  -> Visible = true  ;  stGoodChr    -> Visible = true  ;
        edEmptStr1  -> Visible = true  ;  stEmptyChr   -> Visible = true  ;
        ediWFrstRow -> Visible = true  ;  stFstChipRow -> Visible = true  ;
        ediWFrstCol -> Visible = true  ;  stFstChipCol -> Visible = true  ;
        edMaskStr1  -> Visible = true  ;  stMarkChr    -> Visible = true  ;
        //edMskLine   -> Visible = true  ;  stMskLine    -> Visible = true  ;

        sbAutoOrigin        -> Visible = true ; sbFrstCol           -> Visible = true ;
        sbEmptyStr          -> Visible = true ; sbFrstRow           -> Visible = true ;
        sbFlatAngle         -> Visible = true ; sbGoodStr           -> Visible = true ;
        sbMapType           -> Visible = true ; sbMaskStr           -> Visible = true ;
        sbPenCheckCnt       -> Visible = true ; sbOriMathod         -> Visible = true ;
        sbVisnChkChipRowCnt -> Visible = true ; sbVisnChkChipColCnt -> Visible = true ;
        sbMskLine           -> Visible = true ; lbSpace             -> Visible = true ;

        cbUseHex            -> Visible = true ; btHexConvert        -> Visible = true ;
        //cbMskExist          -> Visible = true ;
    }


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 : break; //데브 인포
        case 1 : break; //맵파일,마스크

        //비젼
        case 2 : //pcVisionSet -> ActivePageIndex = 1 ;
                 /*
                 VSM.SetParentVisnMain(pnVision);
                 VSM.SetParentVisnPara(pnVisnPara);
                 VSM.SetParentAlgoPara(pnAlgoPara);
                 //VSM.SetSettingMode(true);
                 //VSM.Live(VSM.GetActiveVisn() , false);
                 */


                 break ;

        case 3 : pcHED_MT -> ActivePage = HED_MOTOR1;
                 FraMotor[miHED_XHed]->Parent = pnHED_XHed  ;
                 FraMotor[miHED_YHed]->Parent = pnHED_YHed  ;
                 FraMotor[miHED_ZVsn]->Parent = pnHED_ZVsn  ;
                 FraMotor[miHED_TZom]->Parent = pnHED_TZom  ;
                 FraMotor[miSTG_TStg]->Parent = pnSTG_TStg2 ;
                 FraMotor[miSTG_XTrs]->Parent = pnSTG_XTrs2 ;
                 FraMotor[miSTG_ZCst]->Parent = pnSTG_ZCst2 ;

                 FraCylinder[aiSTG_ChkTblUD   ]->Parent = pnSTG_Chk ;
                 FraCylinder[aiSTG_WfrAlgnFB  ]->Parent = pnSTG_Alg ;
                 FraCylinder[aiSTG_TrsCmpGR   ]->Parent = pnTRS_Cmp ;
                 FraCylinder[aiHED_PenMrkDU   ]->Parent = pnHED_Pen ;
                 FraCylinder[aiHED_PenCoverUD ]->Parent = pnHED_Cov ;

                 PM.SetParent(pnHED_XHedPs , miHED_XHed);
                 PM.SetParent(pnHED_YHedPs , miHED_YHed);
                 PM.SetParent(pnHED_ZVsnPs , miHED_ZVsn);
                 PM.SetParent(pnHED_TZomPs , miHED_TZom);
                 break;



        case 4 : pcSTG_MT -> ActivePage = STG_MOTOR2;
                 FraMotor[miHED_XHed]->Parent = pnHED_XHed2 ;
                 FraMotor[miHED_YHed]->Parent = pnHED_YHed2 ;
                 FraMotor[miHED_ZVsn]->Parent = pnHED_ZVsn2 ;
                 FraMotor[miHED_TZom]->Parent = pnHED_TZom2 ;
                 FraMotor[miSTG_TStg]->Parent = pnSTG_TStg  ;
                 FraMotor[miSTG_XTrs]->Parent = pnSTG_XTrs  ;
                 FraMotor[miSTG_ZCst]->Parent = pnSTG_ZCst  ;

                 FraCylinder[aiSTG_ChkTblUD   ]->Parent = pnSTG_Chk2 ;
                 FraCylinder[aiSTG_WfrAlgnFB  ]->Parent = pnSTG_Alg2 ;
                 FraCylinder[aiSTG_TrsCmpGR   ]->Parent = pnTRS_Cmp2 ;
                 FraCylinder[aiHED_PenMrkDU   ]->Parent = pnHED_Pen2 ;
                 FraCylinder[aiHED_PenCoverUD ]->Parent = pnHED_Cov2 ;

                 PM.SetParent(pnSTG_TStgPs , miSTG_TStg);
                 PM.SetParent(pnSTG_XTrsPs , miSTG_XTrs);
                 PM.SetParent(pnSTG_ZCstPs , miSTG_ZCst);
                 break;




    }
    switch ( pcDeviceSet -> ActivePageIndex ) {
        case 0 :                           break ;
        case 1 :                           break ;
        case 2 :                           break ;
        case 3 : rgUnitUser = rgJogUnit1 ;
                 edUnitUser = edJogUnit1 ; break ;
        case 4 : rgUnitUser = rgJogUnit2 ;
                 edUnitUser = edJogUnit2 ; break ;



    }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) {
        for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );
        MtWindowSetPitch();
    }



}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::rgJogUnitClick(TObject *Sender)
{

    AnsiString sTemp;
    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utJog ,0   ); sTemp = 0   ;}
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,1   ); sTemp = 1   ;}
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.5 ); sTemp = 0.5 ;}
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.1 ); sTemp = 0.1 ;}
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.05); sTemp = 0.05;}
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.01); sTemp = 0.01;}
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0)); sTemp = edUnitUser -> Text;}
    if(rgUnitUser->ItemIndex == 7) {
        for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );
        MtWindowSetPitch();
        sTemp = utPitch;
    }
    Trace("Operator", ("Device Set Form_" + sTemp + "Jog Unit Button Click").c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::edJogUnitChange(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 5)  for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdateTimer->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btWfr_VacClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iSel , !IO_GetY(iSel));

    Trace("Operator", "Device Set Form_Vaccum Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btMakeMaskClick(TObject *Sender)
{
    int iMapType = OM.DevInfo.iMapType;

         if(iMapType == 0) odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;
    else if(iMapType == 1) odMap -> Filter = "wmap형식|*.wmap|모든파일|*.*|" ;
    else if(iMapType == 2) odMap -> Filter = "log2형식|*.log2|모든파일|*.*|" ;
    else if(iMapType == 3) odMap -> Filter = "w05형식|*.w05|모든파일|*.*|" ;

    if(!odMap->Execute()) return ;

    //Local Var.
    String sPath = odMap->FileName;

    DM.ARAY[riMSK].ClearMap();

    if(iMapType == 0){
        if(OM.DevInfo.bMskExist){
            if(!MAPU.ChangeMaskNone(sPath , iMapType , riMSK)) FM_MsgOk("ERROR","마스크 영역 세팅에 실패 했습니다.") ;
        }
        else {
            Trace("Operator", "Device Set Form_Make Mark Click");
            if(!MAPU.SetMask(riMSK)) return;
        }
    }

    if(iMapType == 1){
        if(!MAPU.ConvertCoordi1(sPath , riSTG)) FM_MsgOk("ERROR","마스크 영역 세팅에 실패 했습니다.") ;
        if(!OM.DevInfo.bMskExist){
            Trace("Operator", "Device Set Form_Make Mark Click");
            if(!MAPU.SetMask(riMSK)) return;
        }
        if(!MAPU.DeleteNullColRow(riSTG , riMSK)) return;
    }

    if(iMapType == 2){
        if(!OM.DevInfo.bMskExist){
            if(!MAPU.ConvertCoordi2(sPath , riSTG)) FM_MsgOk("ERROR","마스크 영역 세팅에 실패 했습니다.") ;
            Trace("Operator", "Device Set Form_Make Mark Click");
            if(!MAPU.SetMask(riMSK)) return;
        }
        else{
            if(!MAPU.ConvertCoordi2(sPath , riMSK)) FM_MsgOk("ERROR","마스크 영역 세팅에 실패 했습니다.") ;
        }
        //if(!MAPU.DeleteNullColRow(riSTG , riMSK)) return;
    }

    DM.ARAY[riSTG].SetStat(csNone);

    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
         if(iFlatAngle == 0) {                           }
    else if(iFlatAngle == 1) {DM.ARAY[riMSK].TurnCw90 (); m_bFlatAngle = true;}
    else if(iFlatAngle == 2) {DM.ARAY[riMSK].TurnCw180(); m_bFlatAngle = true;}
    else if(iFlatAngle == 3) {DM.ARAY[riMSK].TurnCw270(); m_bFlatAngle = true;}

    //sun 20141022 화면 락되는 것 때문에 지워봄. DM.UpdateAray();

    FM_MsgOk("Confirm","마스크 맵에 기준칩을 설정해 주시기 바랍니다." );
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::cbMapTypeChange(TObject *Sender)
{
     if(cbMapType -> ItemIndex != 0){
        edGoodStr1  -> Visible = false ;  stGoodChr    -> Visible = false ;
        edEmptStr1  -> Visible = false ;  stEmptyChr   -> Visible = false ;
        ediWFrstRow -> Visible = false ;  stFstChipRow -> Visible = false ;
        ediWFrstCol -> Visible = false ;  stFstChipCol -> Visible = false ;
        edMaskStr1  -> Visible = false ;  stMarkChr    -> Visible = false ;
        //edMskLine   -> Visible = false ;  stMskLine    -> Visible = false ;

        sbAutoOrigin        -> Visible = false ; sbFrstCol           -> Visible = false ;
        sbEmptyStr          -> Visible = false ; sbFrstRow           -> Visible = false ;
        sbFlatAngle         -> Visible = false ; sbGoodStr           -> Visible = false ;
        sbMapType           -> Visible = false ; sbMaskStr           -> Visible = false ;
        sbPenCheckCnt       -> Visible = false ; sbOriMathod         -> Visible = false ;
        sbVisnChkChipRowCnt -> Visible = false ; sbVisnChkChipColCnt -> Visible = false ;
        sbMskLine           -> Visible = false ; lbSpace             -> Visible = false ;
        cbUseHex            -> Visible = false ; btHexConvert        -> Visible = false ;
        //cbMskExist          -> Visible = false ;
    }
    else {
        edGoodStr1  -> Visible = true  ;  stGoodChr    -> Visible = true  ;
        edEmptStr1  -> Visible = true  ;  stEmptyChr   -> Visible = true  ;
        ediWFrstRow -> Visible = true  ;  stFstChipRow -> Visible = true  ;
        ediWFrstCol -> Visible = true  ;  stFstChipCol -> Visible = true  ;
        edMaskStr1  -> Visible = true  ;  stMarkChr    -> Visible = true  ;
        //edMskLine   -> Visible = true  ;  stMskLine    -> Visible = true  ;

        sbAutoOrigin        -> Visible = true ; sbFrstCol           -> Visible = true ;
        sbEmptyStr          -> Visible = true ; sbFrstRow           -> Visible = true ;
        sbFlatAngle         -> Visible = true ; sbGoodStr           -> Visible = true ;
        sbMapType           -> Visible = true ; sbMaskStr           -> Visible = true ;
        sbPenCheckCnt       -> Visible = true ; sbOriMathod         -> Visible = true ;
        sbVisnChkChipRowCnt -> Visible = true ; sbVisnChkChipColCnt -> Visible = true ;
        sbMskLine           -> Visible = true ; lbSpace             -> Visible = true ;
        cbUseHex            -> Visible = true ; btHexConvert        -> Visible = true ;
        //cbMskExist          -> Visible = true ;
    }

    if(cbMapType -> ItemIndex == 0){
        FM_MsgOk("Alram","맵파일의 확장자를 입력하세요!");
    }
    else if(cbMapType -> ItemIndex == 1) {
        edExtention -> Text ="wmap";

    }
    else if(cbMapType -> ItemIndex == 2) {
        edExtention -> Text ="log2";
    }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::brMapSaveAsClick(TObject *Sender)
{
    if(!sdMap->Execute()) return ;

    //Local Var.
    String sPath = sdMap->FileName;

    DM.ARAY[riSET].LoadIni(false , sPath) ;


}
//---------------------------------------------------------------------------

void TFrmDeviceSet::SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit)
{
    m_dUnit     = _dUnit     ;
    m_iUnitType = _iUnitType ;
}


//---------------------------------------------------------------------------

void TFrmDeviceSet::SetUnitInit()
{
    if(rgJogUnitVisn->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnitVisn->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnitVisn->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnitVisn->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnitVisn->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnitVisn->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnitVisn->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnitVisn->Text,0.0));
    if(rgJogUnitVisn->ItemIndex == 7) SetUnit(utPitch,0  );

    btZoom_Ccw -> Tag = (int)miHED_TZom ;
    btZoom_Cw  -> Tag = (int)miHED_TZom ;
    btHedZ_Up  -> Tag = (int)miHED_ZVsn ;
    btHedZ_Dn  -> Tag = (int)miHED_ZVsn ;
    btHedY_Bwd -> Tag = (int)miHED_YHed ;
    btHedY_Fwd -> Tag = (int)miHED_YHed ;
    btHedX_Lft -> Tag = (int)miHED_XHed ;
    btHedX_Rgh -> Tag = (int)miHED_XHed ;
    btStgT_Ccw -> Tag = (int)miSTG_TStg ;
    btStgT_Cw  -> Tag = (int)miSTG_TStg ;


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btJogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

    if(m_iId == miHED_XHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchX  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchX ; sTemp = "HED_XHed"; }
    }
    else if(m_iId == miHED_YHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchY  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchY ; sTemp = "HED_XHed"; }
    }

    
         if(m_iUnitType == utJog   ) MT_JogN    ((EN_MOTR_ID)m_iId            ) ;
    else if(m_iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId , -m_dUnit ) ;
    else if(m_iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId , -m_dPitch) ;

    Trace("Operator", ("Operation Form_" + sTemp + "_Motor_N Move").c_str()) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btJogNMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;

    if(m_iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;

    Trace("Operator", "Operation Form_Motor_N Move Stop") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btJogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;


    if(m_iId == miHED_XHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchX  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchX ; sTemp = "HED_XHed"; }
    }
    else if(m_iId == miHED_YHed) {
        if(!AT_MoveCyl(aiHED_PenMrkDU , ccBwd)) return ;
            if(HED.m_bFnshCycTAlgn){ m_dPitch = HED.m_dRevisePitchY  ; sTemp = "HED_XHed"; }
            else                   { m_dPitch = OM.DevInfo.dWfPitchY ; sTemp = "HED_XHed"; }
    }

         if(m_iUnitType == utJog   ) MT_JogP    ((EN_MOTR_ID)m_iId            ) ;
    else if(m_iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  m_dUnit ) ;
    else if(m_iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  m_dPitch) ;

    Trace("Operator", ("Operation Form_" + sTemp + "_Motor_P Move").c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btJogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    if(m_iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;
    Trace("Operator", "Operation Form_Motor_P Move Stop") ;
}
//---------------------------------------------------------------------------





void __fastcall TFrmDeviceSet::btMotorStopClick(TObject *Sender)
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




void __fastcall TFrmDeviceSet::rgJogUnitVisnClick(TObject *Sender)
{
    if(rgJogUnitVisn->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnitVisn->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnitVisn->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnitVisn->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnitVisn->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnitVisn->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnitVisn->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnitVisn->Text,0.0));
    if(rgJogUnitVisn->ItemIndex == 7) SetUnit(utPitch,0  );


    AnsiString sTemp;
    sTemp = rgJogUnitVisn -> ItemIndex;
    Trace("Operator", ("DeviceSet Vision Form_JogUnit_" + sTemp + "Click").c_str());
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------



void __fastcall TFrmDeviceSet::sbMaskStrClick(TObject *Sender)
{
    int iSelButton = ((TSpeedButton *)Sender)-> Tag ;

    FrmInform -> Show(iSelButton);

}
//---------------------------------------------------------------------------









void __fastcall TFrmDeviceSet::btMoveToStgHwCenterClick(TObject *Sender)
{
    if(!MT_GetStop(miHED_XHed) || !MT_GetStop(miHED_YHed)) {
        FM_MsgTime("ERROR","모터가 정지상태가 아닙니다.",2000);
        return ;
    }

    if(FM_MsgYesNo("Confirm","스테이지 하드웨어 센터 위치로 이동합니다.") != mrYes) return ;


    MM.SetManCycle(mcHED_YHedHWCntrPos);

    VSM.Live(VSM.GetActiveVisn() , true);

    //HED.MoveMotr(miHED_XHed , piHED_XHedHWCntrPos   );
    //HED.MoveMotr(miHED_YHed , piHED_YHedHWCntrPos   );



}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btMoveToVisnPosClick(TObject *Sender)
{
    if(!MT_GetStop(miHED_XHed) || !MT_GetStop(miHED_YHed)) {
        FM_MsgTime("ERROR","모터가 정지상태가 아닙니다.",2000);
        return ;
    }

    int iTag = ((TButton *)Sender) -> Tag ;

    if(VSM.GetActiveVisn() == viWfr) { if(FM_MsgYesNo("Confirm","웨이퍼 세타 얼라인 위치로 이동합니다."  ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viWfo) { if(FM_MsgYesNo("Confirm","맵핑 기준칩 위치로 이동합니다."         ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viCal) { if(FM_MsgYesNo("Confirm","캘리브레이션 패드 위치로 이동합니다."   ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viWfp) { if(FM_MsgYesNo("Confirm","펜마킹 패드 위치로 이동합니다."         ) != mrYes) return ;}
    if(iTag                == 0    ) { if(FM_MsgYesNo("Confirm","비전 Z축 포지션으로 이동합니다."        ) != mrYes) return ;}
    if(iTag                == 1    ) { if(FM_MsgYesNo("Confirm","비전 줌T 포지션으로 이동합니다."        ) != mrYes) return ;}

    if(VSM.GetActiveVisn() == viWfr) { MM.SetManCycle(mcHED_HedVTAlgn   ); }//HED.MoveMotr(miHED_XHed , piHED_XHedVTAlgn   ); HED.MoveMotr(miHED_YHed , piHED_YHedVTAlgn   ); }
    if(VSM.GetActiveVisn() == viWfo) { MM.SetManCycle(mcHED_HedVOri     ); }//HED.MoveMotr(miHED_XHed , piHED_XHedVOri     ); HED.MoveMotr(miHED_YHed , piHED_YHedVOri     ); }
    if(VSM.GetActiveVisn() == viCal) { MM.SetManCycle(mcHED_HedVCalPad  ); }//HED.MoveMotr(miHED_XHed , piHED_XHedVCalPad  ); HED.MoveMotr(miHED_YHed , piHED_YHedVCalPad  ); }
    if(VSM.GetActiveVisn() == viWfp) { MM.SetManCycle(mcHED_HedVBoardStt); }//HED.MoveMotr(miHED_XHed , piHED_XHedVBoardStt); HED.MoveMotr(miHED_YHed , piHED_YHedVBoardStt); }
    if(iTag                == 0    ) { HED.MoveMotr(miHED_ZVsn , piHED_ZVsnWork ); }
    if(iTag                == 1    ) { HED.MoveMotr(miHED_TZom , piHED_TZomWork ); }

    VSM.Live(VSM.GetActiveVisn() , true);

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btVisnPosSaveClick(TObject *Sender)
{
    if(VSM.GetActiveVisn() == viWfr) { if(FM_MsgYesNo("Confirm","스테이지 세타 얼라인 위치를 저장 합니다.") != mrYes) return ;}
    if(VSM.GetActiveVisn() == viWfo) { if(FM_MsgYesNo("Confirm","맵핑 기준칩 위치를 저장 합니다."         ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viCal) { if(FM_MsgYesNo("Confirm","캘리브레이션 패드 위치를 저장 합니다."   ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viWfp) { if(FM_MsgYesNo("Confirm","펜마킹 패드 위치를 저장 합니다."         ) != mrYes) return ;}


    if(VSM.GetActiveVisn() == viWfo) { PM.SetValue(miHED_XHed , pvHED_XHedVOri      , MT_GetCmdPos(miHED_XHed)); PM.SetValue(miHED_YHed , pvHED_YHedVOri      , MT_GetCmdPos(miHED_YHed)); }


    if(VSM.GetActiveVisn() == viCal) { PM.SetValue(miHED_XHed , pvHED_XHedVCalPad   , MT_GetCmdPos(miHED_XHed)); PM.SetValue(miHED_YHed , pvHED_YHedVCalPad   , MT_GetCmdPos(miHED_YHed)); }
     //Pen 패드시작포지션을 바꾸어서 버그이다. 20141110 선계원
    //if(VSM.GetActiveVisn() == viWfp) { PM.SetValue(miHED_XHed , pvHED_XHedVBoardStt , MT_GetCmdPos(miHED_XHed)); PM.SetValue(miHED_YHed , pvHED_YHedVBoardStt , MT_GetCmdPos(miHED_YHed)); }
    if(VSM.GetActiveVisn() == viWfr) { PM.SetValue(miHED_XHed , pvHED_XHedVTAlgn    , MT_GetCmdPos(miHED_XHed)); PM.SetValue(miHED_YHed , pvHED_YHedVTAlgn    , MT_GetCmdPos(miHED_YHed));
                                       PM.SetValue(miSTG_TStg , pvSTG_TStgTAlgn     , MT_GetCmdPos(miSTG_TStg));                                                                           }


    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btVisnZMoveClick(TObject *Sender)
{
    /*
    int iTag = ((TButton *)Sender) -> Tag ;

    if(iTag == 0) { if(FM_MsgYesNo("Confirm","헤드 Z축 비젼 포지션을 저장 합니다.") != mrYes) return ;}
    if(iTag == 1) { if(FM_MsgYesNo("Confirm","헤드 T축 줌 포지션을 저장 합니다."  ) != mrYes) return ;}

    if(iTag == 0) { PM.SetValue(miHED_ZVsn , pvHED_ZVsnWait , MT_GetCmdPos(miHED_ZVsn)); PM.SetValue(miHED_ZVsn , pvHED_ZVsnWork , MT_GetCmdPos(miHED_ZVsn)); }
    if(iTag == 1) { PM.SetValue(miHED_TZom , pvHED_TZomWait , MT_GetCmdPos(miHED_TZom)); PM.SetValue(miHED_TZom , pvHED_TZomWork , MT_GetCmdPos(miHED_TZom)); }

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    if(FM_MsgYesNo("Confirm","광학계 포지션이 바뀌었습니다. 켈리브레이션을 수행 하시겠습니까?") != mrYes) return ;

    VSM.SetActiveVisn(viCal);
    VSM.Live(viWfr , false);
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        VSM.Live(i,false);
    }



    Sleep(500);
    
    //for(int i = 0 ; i < MAX_VISN_ID ; i++) {
    //
    //    VSM.Live(i,false);
    //}


    //Sleep(500);


    MM.SetManCycle(mcHED_CycleCal) ;   */
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btVisnManualClick(TObject *Sender)
{
    if(VSM.GetActiveVisn() == viWfr) { if(FM_MsgYesNo("Confirm","웨이퍼 세타 얼라인을 수행합니다."         ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viWfo) { if(FM_MsgYesNo("Confirm","웨이퍼 기준칩을 찾습니다."                ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viCal) { if(FM_MsgYesNo("Confirm","켈리브레이션을 수행 합니다."              ) != mrYes) return ;}
    if(VSM.GetActiveVisn() == viWfp) { if(FM_MsgYesNo("Confirm","카메라와 펜의 오프셑을 보정을 수행합니다.") != mrYes) return ;}
    VSM.LiveAllOff(true);
    if(VSM.GetActiveVisn() == viWfr) {MM.SetManCycle(mcHED_CycleTAlign  );}
    if(VSM.GetActiveVisn() == viWfo) {MM.SetManCycle(mcHED_CycleOrigin  );}
    if(VSM.GetActiveVisn() == viCal) {MM.SetManCycle(mcHED_CycleCal     );}
    if(VSM.GetActiveVisn() == viWfp) {MM.SetManCycle(mcHED_CyclePenAlign);}

    //VSM.Live(VSM.GetActiveVisn() , false);



}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btVsnZTSaveClick(TObject *Sender)
{
    PM.SetValue(miHED_ZVsn , pvHED_ZVsnWait , MT_GetCmdPos(miHED_ZVsn)); PM.SetValue(miHED_ZVsn , pvHED_ZVsnWork , MT_GetCmdPos(miHED_ZVsn));
    PM.SetValue(miHED_TZom , pvHED_TZomWait , MT_GetCmdPos(miHED_TZom)); PM.SetValue(miHED_TZom , pvHED_TZomWork , MT_GetCmdPos(miHED_TZom));

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    if(FM_MsgYesNo("Confirm","광학계 포지션이 바뀌었습니다. 켈리브레이션을 수행 하시겠습니까?") != mrYes) return ;


    VSM.LiveAllOff(true);
    VSM.SetActiveVisn(viCal);


    /*
    Sleep(500);
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {

        VSM.Live(i,false);
    }
    */

    //Sleep(500);


    MM.SetManCycle(mcHED_CycleCal) ;
}
//---------------------------------------------------------------------------



void __fastcall TFrmDeviceSet::btHexConvertClick(TObject *Sender)
{
    FrmMapViewer -> Show();    
}
//---------------------------------------------------------------------------


