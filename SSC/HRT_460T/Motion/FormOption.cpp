//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
#include "LotUnit.h"
#include "postbuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmOption::InitArrayPanel()
{
    DM.ARAY[riOPT].SetParent(pnOption   ); DM.ARAY[riOPT].SetConfig(false , "riOPT"); DM.ARAY[riOPT].ClearDispItem();

    DM.ARAY[riOPT].SetDispItem(csFail   ,"Fail "  ) ;
    DM.ARAY[riOPT].SetDispItem(csGood   ,"Good "  ) ;
    DM.ARAY[riOPT].SetDispItem(csNone   ,"None "  ) ;
}

void __fastcall TFrmOption::btSaveClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
        return;
    }
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    //Log Save d:\Spc\OptionLog
    if(cbDryRun          -> Checked != OM.CmnOptn.bDryRun        ) SPC.SaveOptionLog(cbDryRun        ->Tag,false);
    if(cbIgnrDoor        -> Checked != OM.CmnOptn.bIgnrDoor      ) SPC.SaveOptionLog(cbIgnrDoor      ->Tag,false);
    if(cbIgnrVisn        -> Checked != OM.CmnOptn.bIgnrVisn      ) SPC.SaveOptionLog(cbIgnrVisn      ->Tag,false);
    if(cbIgnrInsp        -> Checked != OM.CmnOptn.bIgnrInsp      ) SPC.SaveOptionLog(cbIgnrInsp      ->Tag,false);
    if(cbIgnrHeatAlarm   -> Checked != OM.CmnOptn.bIgnrHeatAlm   ) SPC.SaveOptionLog(cbIgnrHeatAlarm ->Tag,false);
    if(cbLoadingStop     -> Checked != OM.CmnOptn.bLoadingStop   ) SPC.SaveOptionLog(cbLoadingStop   ->Tag,false);
    if(cbHeatFail        -> Checked != OM.CmnOptn.bIgnrHeatAlm   ) SPC.SaveOptionLog(cbHeatFail      ->Tag,false);
    //if(cbAutoConvesion   -> Checked != OM.CmnOptn.bAutoConvesion ) SPC.SaveOptionLog(cbAutoConvesion ->Tag,false);
    //if(cbAutoOperation   -> Checked != OM.CmnOptn.bAutoOperation ) SPC.SaveOptionLog(cbAutoOperation ->Tag,false);
    if(cbIgnrTrim        -> Checked != OM.CmnOptn.bIgnrTrim      ) SPC.SaveOptionLog(cbIgnrTrim      ->Tag,false);

    UpdateCmnOptn(toBuff);
    OM.SaveCmnOptn();
    OM.SaveEqpOptn();
//    UserMsg2("Confirm" , "저장 되었습니다.");

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateCmnOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbDryRun           -> Checked = OM.CmnOptn.bDryRun                  ;
        cbIgnrDoor         -> Checked = OM.CmnOptn.bIgnrDoor                ;
        cbIgnrInsp         -> Checked = OM.CmnOptn.bIgnrInsp                ;
        cbIgnrVisn         -> Checked = OM.CmnOptn.bIgnrVisn                ;
        cbIgnrHeatAlarm    -> Checked = OM.CmnOptn.bIgnrHeatAlm             ;
        //cbAutoOperation    -> Checked = OM.CmnOptn.bAutoOperation           ;
        //cbAutoConvesion    -> Checked = OM.CmnOptn.bAutoConvesion           ;
        cbLoadingStop      -> Checked = OM.CmnOptn.bLoadingStop             ;
        cbHeatFail         -> Checked = OM.CmnOptn.bCheckFailCnt            ;
        cbIgnrTrim         -> Checked = OM.CmnOptn.bIgnrTrim                ;
        edLimAllFailCnt    -> Text    = OM.CmnOptn.iCheckFailCnt            ;
        cbSortSersor       -> Checked = OM.CmnOptn.bSortSensorDelay         ;
        edSortSensorDelay  -> Text    = OM.CmnOptn.iSortSensorDelay         ;
        edRejectBinNo      -> Text    = OM.CmnOptn.iBinDefaultNo            ;
        edRjtBinNoInput    -> Text    = OM.CmnOptn.iBinDefaultNo            ;
        cbIgnrDrop         -> Checked = OM.CmnOptn.bIgnrDrop                ;

        cbUsedInsOptn      -> Checked = OM.CmnOptn.bUsedInsOptn             ;

        cbSkipYieldErr     -> Checked = OM.CmnOptn.bSkipYieldErr            ;
        edStandardYield    -> Text    = OM.CmnOptn.dStandardYield           ;

        cbSkipPinCnt       -> Checked = OM.DevOptn.bSkipPinCnt              ;

        cbSkipPinCnt       -> Checked = OM.DevOptn.bSkipPinCnt              ;


        //Bin 사용 옵션때문에..20150323. JS
        cbBin1             -> Checked = OM.CmnOptn.bUsedBin1                ;
        cbBin2             -> Checked = OM.CmnOptn.bUsedBin2                ;
        cbBin3             -> Checked = OM.CmnOptn.bUsedBin3                ;
        cbBin4             -> Checked = OM.CmnOptn.bUsedBin4                ;
        cbBin5             -> Checked = OM.CmnOptn.bUsedBin5                ;
        cbBin6             -> Checked = OM.CmnOptn.bUsedBin6                ;
        cbBin7             -> Checked = OM.CmnOptn.bUsedBin7                ;
        cbBin8             -> Checked = OM.CmnOptn.bUsedBin8                ;
        cbBin9             -> Checked = OM.CmnOptn.bUsedBin9                ;
        cbBin10            -> Checked = OM.CmnOptn.bUsedBin10               ;

        edMinVf1           -> Text    = OM.CmnOptn.dMinVf1                  ; edMaxVf1           -> Text    = OM.CmnOptn.dMaxVf1                   ;
        edMinVf2           -> Text    = OM.CmnOptn.dMinVf2                  ; edMaxVf2           -> Text    = OM.CmnOptn.dMaxVf2                   ;
        edMinVf3           -> Text    = OM.CmnOptn.dMinVf3                  ; edMaxVf3           -> Text    = OM.CmnOptn.dMaxVf3                   ;
        edMinVf4           -> Text    = OM.CmnOptn.dMinVf4                  ; edMaxVf4           -> Text    = OM.CmnOptn.dMaxVf4                   ;
        edMinVf5           -> Text    = OM.CmnOptn.dMinVf5                  ; edMaxVf5           -> Text    = OM.CmnOptn.dMaxVf5                   ;
        edMinVf6           -> Text    = OM.CmnOptn.dMinVf6                  ; edMaxVf6           -> Text    = OM.CmnOptn.dMaxVf6                   ;
        edMinVf7           -> Text    = OM.CmnOptn.dMinVf7                  ; edMaxVf7           -> Text    = OM.CmnOptn.dMaxVf7                   ;
        edMinVf8           -> Text    = OM.CmnOptn.dMinVf8                  ; edMaxVf8           -> Text    = OM.CmnOptn.dMaxVf8                   ;
        edMinVf9           -> Text    = OM.CmnOptn.dMinVf9                  ; edMaxVf9           -> Text    = OM.CmnOptn.dMaxVf9                   ;
        edMinVf10          -> Text    = OM.CmnOptn.dMinVf10                 ; edMaxVf10          -> Text    = OM.CmnOptn.dMaxVf10                  ;

        edMinVr1           -> Text    = OM.CmnOptn.dMinVr1                  ; edMaxVr1           -> Text    = OM.CmnOptn.dMaxVr1                   ;
        edMinVr2           -> Text    = OM.CmnOptn.dMinVr2                  ; edMaxVr2           -> Text    = OM.CmnOptn.dMaxVr2                   ;
        edMinVr3           -> Text    = OM.CmnOptn.dMinVr3                  ; edMaxVr3           -> Text    = OM.CmnOptn.dMaxVr3                   ;
        edMinVr4           -> Text    = OM.CmnOptn.dMinVr4                  ; edMaxVr4           -> Text    = OM.CmnOptn.dMaxVr4                   ;
        edMinVr5           -> Text    = OM.CmnOptn.dMinVr5                  ; edMaxVr5           -> Text    = OM.CmnOptn.dMaxVr5                   ;
        edMinVr6           -> Text    = OM.CmnOptn.dMinVr6                  ; edMaxVr6           -> Text    = OM.CmnOptn.dMaxVr6                   ;
        edMinVr7           -> Text    = OM.CmnOptn.dMinVr7                  ; edMaxVr7           -> Text    = OM.CmnOptn.dMaxVr7                   ;
        edMinVr8           -> Text    = OM.CmnOptn.dMinVr8                  ; edMaxVr8           -> Text    = OM.CmnOptn.dMaxVr8                   ;
        edMinVr9           -> Text    = OM.CmnOptn.dMinVr9                  ; edMaxVr9           -> Text    = OM.CmnOptn.dMaxVr9                   ;
        edMinVr10          -> Text    = OM.CmnOptn.dMinVr10                 ; edMaxVr10          -> Text    = OM.CmnOptn.dMaxVr10                  ;

    }
    else {
        OM.CmnOptn.bDryRun            = cbDryRun          -> Checked         ;
        OM.CmnOptn.bIgnrDoor          = cbIgnrDoor        -> Checked         ;
        OM.CmnOptn.bIgnrInsp          = cbIgnrInsp        -> Checked         ;
        OM.CmnOptn.bIgnrVisn          = cbIgnrVisn        -> Checked         ;
        OM.CmnOptn.bIgnrHeatAlm       = cbIgnrHeatAlarm   -> Checked         ;
        //OM.CmnOptn.bAutoOperation     = cbAutoOperation   -> Checked         ;
        //OM.CmnOptn.bAutoConvesion     = cbAutoConvesion   -> Checked         ;
        OM.CmnOptn.bLoadingStop       = cbLoadingStop     -> Checked         ;
        OM.CmnOptn.bCheckFailCnt      = cbHeatFail        -> Checked         ;
        OM.CmnOptn.bIgnrTrim          = cbIgnrTrim        -> Checked         ;
        OM.CmnOptn.iCheckFailCnt      = edLimAllFailCnt   -> Text.ToIntDef(0);
        OM.CmnOptn.bSortSensorDelay   = cbSortSersor      -> Checked         ;
        OM.CmnOptn.iSortSensorDelay   = edSortSensorDelay -> Text.ToIntDef(0);
        OM.CmnOptn.bIgnrDrop          = cbIgnrDrop        -> Checked         ;
        OM.CmnOptn.iBinDefaultNo      = edRjtBinNoInput   -> Text.ToIntDef(0) - 1;
        if(OM.CmnOptn.iBinDefaultNo   <= 0 || OM.CmnOptn.iBinDefaultNo >= 10) OM.CmnOptn.iBinDefaultNo = 0;
        OM.CmnOptn.bUsedInsOptn       = cbUsedInsOptn     -> Checked         ;
        OM.CmnOptn.bSkipYieldErr      = cbSkipYieldErr    -> Checked         ;
        OM.CmnOptn.dStandardYield     = edStandardYield   -> Text.ToDouble() ;

        //Bin 사용 옵션때문에..20150323. JS
        OM.CmnOptn.bUsedBin1        = cbBin1             -> Checked        ;
        OM.CmnOptn.bUsedBin2        = cbBin2             -> Checked        ;
        OM.CmnOptn.bUsedBin3        = cbBin3             -> Checked        ;
        OM.CmnOptn.bUsedBin4        = cbBin4             -> Checked        ;
        OM.CmnOptn.bUsedBin5        = cbBin5             -> Checked        ;
        OM.CmnOptn.bUsedBin6        = cbBin6             -> Checked        ;
        OM.CmnOptn.bUsedBin7        = cbBin7             -> Checked        ;
        OM.CmnOptn.bUsedBin8        = cbBin8             -> Checked        ;
        OM.CmnOptn.bUsedBin9        = cbBin9             -> Checked        ;
        OM.CmnOptn.bUsedBin10       = cbBin10            -> Checked        ;

        OM.CmnOptn.dMinVf1          =  edMinVf1          -> Text.ToDouble(); OM.CmnOptn.dMaxVf1           = edMaxVf1           -> Text.ToDouble();
        OM.CmnOptn.dMinVf2          =  edMinVf2          -> Text.ToDouble(); OM.CmnOptn.dMaxVf2           = edMaxVf2           -> Text.ToDouble();
        OM.CmnOptn.dMinVf3          =  edMinVf3          -> Text.ToDouble(); OM.CmnOptn.dMaxVf3           = edMaxVf3           -> Text.ToDouble();
        OM.CmnOptn.dMinVf4          =  edMinVf4          -> Text.ToDouble(); OM.CmnOptn.dMaxVf4           = edMaxVf4           -> Text.ToDouble();
        OM.CmnOptn.dMinVf5          =  edMinVf5          -> Text.ToDouble(); OM.CmnOptn.dMaxVf5           = edMaxVf5           -> Text.ToDouble();
        OM.CmnOptn.dMinVf6          =  edMinVf6          -> Text.ToDouble(); OM.CmnOptn.dMaxVf6           = edMaxVf6           -> Text.ToDouble();
        OM.CmnOptn.dMinVf7          =  edMinVf7          -> Text.ToDouble(); OM.CmnOptn.dMaxVf7           = edMaxVf7           -> Text.ToDouble();
        OM.CmnOptn.dMinVf8          =  edMinVf8          -> Text.ToDouble(); OM.CmnOptn.dMaxVf8           = edMaxVf8           -> Text.ToDouble();
        OM.CmnOptn.dMinVf9          =  edMinVf9          -> Text.ToDouble(); OM.CmnOptn.dMaxVf9           = edMaxVf9           -> Text.ToDouble();
        OM.CmnOptn.dMinVf10         =  edMinVf10         -> Text.ToDouble(); OM.CmnOptn.dMaxVf10          = edMaxVf10          -> Text.ToDouble();

        OM.CmnOptn.dMinVr1          =  edMinVr1          -> Text.ToDouble(); OM.CmnOptn.dMaxVr1           = edMaxVr1           -> Text.ToDouble();
        OM.CmnOptn.dMinVr2          =  edMinVr2          -> Text.ToDouble(); OM.CmnOptn.dMaxVr2           = edMaxVr2           -> Text.ToDouble();
        OM.CmnOptn.dMinVr3          =  edMinVr3          -> Text.ToDouble(); OM.CmnOptn.dMaxVr3           = edMaxVr3           -> Text.ToDouble();
        OM.CmnOptn.dMinVr4          =  edMinVr4          -> Text.ToDouble(); OM.CmnOptn.dMaxVr4           = edMaxVr4           -> Text.ToDouble();
        OM.CmnOptn.dMinVr5          =  edMinVr5          -> Text.ToDouble(); OM.CmnOptn.dMaxVr5           = edMaxVr5           -> Text.ToDouble();
        OM.CmnOptn.dMinVr6          =  edMinVr6          -> Text.ToDouble(); OM.CmnOptn.dMaxVr6           = edMaxVr6           -> Text.ToDouble();
        OM.CmnOptn.dMinVr7          =  edMinVr7          -> Text.ToDouble(); OM.CmnOptn.dMaxVr7           = edMaxVr7           -> Text.ToDouble();
        OM.CmnOptn.dMinVr8          =  edMinVr8          -> Text.ToDouble(); OM.CmnOptn.dMaxVr8           = edMaxVr8           -> Text.ToDouble();
        OM.CmnOptn.dMinVr9          =  edMinVr9          -> Text.ToDouble(); OM.CmnOptn.dMaxVr9           = edMaxVr9           -> Text.ToDouble();
        OM.CmnOptn.dMinVr10         =  edMinVr10         -> Text.ToDouble(); OM.CmnOptn.dMaxVr10          = edMaxVr10          -> Text.ToDouble();

        OM.DevOptn.bSkipPinCnt      = cbSkipPinCnt       -> Checked        ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    if(!OM.MstOptn.bUsedBinSorting) Panel4            -> Visible = false;
    else                            Panel4            -> Visible = true ;

    UpdateCmnOptn(toTabl);
    tmOption -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::btSetBinCntClick(TObject *Sender)
{
         if(edRjtBinNoInput -> Text.ToIntDef(0) >= 10 ) OM.CmnOptn.iBinDefaultNo = 9 ;
    else if(edRjtBinNoInput -> Text.ToIntDef(0) <= 1  ) OM.CmnOptn.iBinDefaultNo = 0 ;
    else OM.CmnOptn.iBinDefaultNo = edRjtBinNoInput -> Text.ToIntDef(0) - 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::tmOptionTimer(TObject *Sender)
{
    tmOption -> Enabled = false;

    edBinCount1  -> Text = PSB.m_iBin0Cnt;
    edBinCount2  -> Text = PSB.m_iBin1Cnt;
    edBinCount3  -> Text = PSB.m_iBin2Cnt;
    edBinCount4  -> Text = PSB.m_iBin3Cnt;
    edBinCount5  -> Text = PSB.m_iBin4Cnt;
    edBinCount6  -> Text = PSB.m_iBin5Cnt;
    edBinCount7  -> Text = PSB.m_iBin6Cnt;
    edBinCount8  -> Text = PSB.m_iBin7Cnt;
    edBinCount9  -> Text = PSB.m_iBin8Cnt;
    edBinCount10 -> Text = PSB.m_iBin9Cnt;

    tmOption -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmOption -> Enabled = false;        
}
//---------------------------------------------------------------------------

