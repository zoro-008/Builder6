//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
#include "LanguageUnit.h"
#include "SortingTool.h"
#include "UserFile.h"
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
    if(cbVisionSkip      -> Checked != OM.CmnOptn.bVisionSkip    ) SPC.SaveOptionLog(cbVisionSkip    ->Tag,false);
    if(cbIgnrInsp        -> Checked != OM.CmnOptn.bIgnrInsp      ) SPC.SaveOptionLog(cbIgnrInsp      ->Tag,false);
    if(cbIgnrHeatAlarm   -> Checked != OM.CmnOptn.bIgnrHeatAlm   ) SPC.SaveOptionLog(cbIgnrHeatAlarm ->Tag,false);
    if(cbIgnrStrOverAlm  -> Checked != OM.CmnOptn.bIgnrStrOverAlm) SPC.SaveOptionLog(cbIgnrStrOverAlm->Tag,false);
    if(cbLoadingStop     -> Checked != OM.CmnOptn.bLoadingStop   ) SPC.SaveOptionLog(cbLoadingStop   ->Tag,false);
    if(cbHeatFail        -> Checked != OM.CmnOptn.bIgnrHeatAlm   ) SPC.SaveOptionLog(cbHeatFail      ->Tag,false);
    if(cbIgnrTrim        -> Checked != OM.CmnOptn.bIgnrTrim      ) SPC.SaveOptionLog(cbIgnrTrim      ->Tag,false);
    if(cbIgnrVacuum      -> Checked != OM.CmnOptn.bIgnrVacuum    ) SPC.SaveOptionLog(cbIgnrVacuum    ->Tag,false);
    if(cbIgnrDecChk      -> Checked != OM.CmnOptn.bIgnrDecChk    ) SPC.SaveOptionLog(cbIgnrDecChk    ->Tag,false);
    if(cbIgnrTrimDecChk  -> Checked != OM.CmnOptn.bIgnrTrimDecChk) SPC.SaveOptionLog(cbIgnrTrimDecChk->Tag,false);

    AnsiString _sPreOperator ;

    _sPreOperator = OM.CmnOptn.sOperator;

    switch(rgOperator -> ItemIndex) {
        case 0 : OM.CmnOptn.sOperator = edUser1  -> Text   ; break ;
        case 1 : OM.CmnOptn.sOperator = edUser2  -> Text   ; break ;
        case 2 : OM.CmnOptn.sOperator = edUser3  -> Text   ; break ;
        case 3 : OM.CmnOptn.sOperator = edUser4  -> Text   ; break ;
        case 4 : OM.CmnOptn.sOperator = edUser5  -> Text   ; break ;
        case 5 : OM.CmnOptn.sOperator = edUser6  -> Text   ; break ;
        case 6 : OM.CmnOptn.sOperator = edUser7  -> Text   ; break ;
        case 7 : OM.CmnOptn.sOperator = edUser8  -> Text   ; break ;
        case 8 : OM.CmnOptn.sOperator = edUser9  -> Text   ; break ;
    }

    if(_sPreOperator != OM.CmnOptn.sOperator) SEQ._bInspMstr = false;
    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();
    OM.SaveEqpOptn();
    OM.SaveMstOptn();

//    UserMsg2("Confirm" , "저장 되었습니다.");

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        if(FM_GetLevel() == (EN_LEVEL)lvOperator){
            Panel1 -> Visible = false;
            Panel3 -> Visible = false;
            Panel4 -> Visible = false;
            Panel9 -> Visible = false;

        }
        else{
            Panel1 -> Visible = true;
            Panel3 -> Visible = true;
            Panel4 -> Visible = true;
            Panel9 -> Visible = true;
        }

        cbDryRun           -> Checked = OM.CmnOptn.bDryRun                  ;
        cbIgnrDoor         -> Checked = OM.CmnOptn.bIgnrDoor                ;
        cbIgnrInsp         -> Checked = OM.CmnOptn.bIgnrInsp                ;
        cbVisionSkip       -> Checked = OM.CmnOptn.bVisionSkip              ;
        cbIgnrHeatAlarm    -> Checked = OM.CmnOptn.bIgnrHeatAlm             ;
        cbIgnrStrOverAlm   -> Checked = OM.CmnOptn.bIgnrStrOverAlm          ;
        cbLoadingStop      -> Checked = OM.CmnOptn.bLoadingStop             ;
        cbHeatFail         -> Checked = OM.CmnOptn.bCheckFailCnt            ;
        cbIgnrTrim         -> Checked = OM.CmnOptn.bIgnrTrim                ;
        edLimAllFailCnt    -> Text    = OM.CmnOptn.iCheckFailCnt            ;
        cbIgnrVacuum       -> Checked = OM.CmnOptn.bIgnrVacuum              ;
        cbSortSersor       -> Checked = OM.CmnOptn.bSortSensorDelay         ;
        edSortSensorDelay  -> Text    = OM.CmnOptn.iSortSensorDelay         ;
        cbIgnrDecChk       -> Checked = OM.CmnOptn.bIgnrDecChk              ;
        cbIgnrTrimDecChk   -> Checked = OM.CmnOptn.bIgnrTrimDecChk          ;

        edLedFailBin       -> Text    = OM.CmnOptn.iLedFailBin              ;
        edZnnrFailBin      -> Text    = OM.CmnOptn.iZnnrFailBin             ;
        edDropFailBin      -> Text    = OM.CmnOptn.iDropFailBin             ;
        edAllFailBin       -> Text    = OM.CmnOptn.iAllFailBin              ;

        edMESPath          -> Text    = OM.CmnOptn.sMESPath                 ;

        edUser1            -> Text    = OM.CmnOptn.sUser1                   ;
        edUser2            -> Text    = OM.CmnOptn.sUser2                   ;
        edUser3            -> Text    = OM.CmnOptn.sUser3                   ;
        edUser4            -> Text    = OM.CmnOptn.sUser4                   ;
        edUser5            -> Text    = OM.CmnOptn.sUser5                   ;
        edUser6            -> Text    = OM.CmnOptn.sUser6                   ;
        edUser7            -> Text    = OM.CmnOptn.sUser7                   ;
        edUser8            -> Text    = OM.CmnOptn.sUser8                   ;
        edUser9            -> Text    = OM.CmnOptn.sUser9                   ;

        if(OM.MstOptn.bUsedVfSortOption){
            edLowVf        -> Text    = OM.CmnOptn.dLowVfValue              ;
            edHighVf       -> Text    = OM.CmnOptn.dHighVfValue             ;

            edLowVfBin     -> Text    = OM.CmnOptn.iLowVfBin                ;
            edHighVfBin    -> Text    = OM.CmnOptn.iHighVfBin               ;
        }

        rgOperator -> ItemIndex = OM.CmnOptn.iSeleteOperator ;


    }
    else {
        OM.CmnOptn.bDryRun          = cbDryRun          -> Checked         ;
        OM.CmnOptn.bIgnrDoor        = cbIgnrDoor        -> Checked         ;
        OM.CmnOptn.bIgnrInsp        = cbIgnrInsp        -> Checked         ;
        OM.CmnOptn.bVisionSkip      = cbVisionSkip      -> Checked         ;
        OM.CmnOptn.bIgnrHeatAlm     = cbIgnrHeatAlarm   -> Checked         ;
        OM.CmnOptn.bIgnrStrOverAlm  = cbIgnrStrOverAlm  -> Checked         ;
        OM.CmnOptn.bLoadingStop     = cbLoadingStop     -> Checked         ;
        OM.CmnOptn.bCheckFailCnt    = cbHeatFail        -> Checked         ;
        OM.CmnOptn.bIgnrTrim        = cbIgnrTrim        -> Checked         ;
        OM.CmnOptn.iCheckFailCnt    = edLimAllFailCnt   -> Text.ToIntDef(0);
        OM.CmnOptn.bIgnrVacuum      = cbIgnrVacuum      -> Checked         ;
        OM.CmnOptn.bSortSensorDelay = cbSortSersor      -> Checked         ;
        OM.CmnOptn.iSortSensorDelay = edSortSensorDelay -> Text.ToIntDef(0);
        OM.CmnOptn.bIgnrDecChk      = cbIgnrDecChk      -> Checked         ;
        OM.CmnOptn.bIgnrTrimDecChk  = cbIgnrTrimDecChk  -> Checked         ;

        OM.CmnOptn.iLedFailBin      = edLedFailBin      -> Text.ToIntDef(0);
        OM.CmnOptn.iZnnrFailBin     = edZnnrFailBin     -> Text.ToIntDef(0);
        OM.CmnOptn.iDropFailBin     = edDropFailBin     -> Text.ToIntDef(0);
        OM.CmnOptn.iAllFailBin      = edAllFailBin      -> Text.ToIntDef(0);

        OM.CmnOptn.dLowVfValue      = edLowVf           -> Text.ToDouble() ;
        OM.CmnOptn.dHighVfValue     = edHighVf          -> Text.ToDouble() ;

        OM.CmnOptn.iLowVfBin        = edLowVfBin        -> Text.ToIntDef(0);
        OM.CmnOptn.iHighVfBin       = edHighVfBin       -> Text.ToIntDef(0);

        OM.CmnOptn.sMESPath         = edMESPath         -> Text            ;

        OM.CmnOptn.sUser1           = edUser1           -> Text            ;
        OM.CmnOptn.sUser2           = edUser2           -> Text            ;
        OM.CmnOptn.sUser3           = edUser3           -> Text            ;
        OM.CmnOptn.sUser4           = edUser4           -> Text            ;
        OM.CmnOptn.sUser5           = edUser5           -> Text            ;
        OM.CmnOptn.sUser6           = edUser6           -> Text            ;
        OM.CmnOptn.sUser7           = edUser7           -> Text            ;
        OM.CmnOptn.sUser8           = edUser8           -> Text            ;
        OM.CmnOptn.sUser9           = edUser9           -> Text            ;

        OM.CmnOptn.iSeleteOperator  = rgOperator -> ItemIndex ;


    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------



void __fastcall TFrmOption::Timer1Timer(TObject *Sender)
{
    Timer1 -> Enabled = false;
    bool bView = false;
    static bool bPreView = false;

    switch(OM.CmnOptn.iLedFailBin){
        case 0: edBinCount1 ->  Text = STL.m_iBin0Cnt; break;
        case 1: edBinCount1 ->  Text = STL.m_iBin1Cnt; break;
        case 2: edBinCount1 ->  Text = STL.m_iBin2Cnt; break;
        case 3: edBinCount1 ->  Text = STL.m_iBin3Cnt; break;
        case 4: edBinCount1 ->  Text = STL.m_iBin4Cnt; break;
        case 5: edBinCount1 ->  Text = STL.m_iBin5Cnt; break;
        case 6: edBinCount1 ->  Text = STL.m_iBin6Cnt; break;
        case 7: edBinCount1 ->  Text = STL.m_iBin7Cnt; break;
        case 8: edBinCount1 ->  Text = STL.m_iBin8Cnt; break;
        case 9: edBinCount1 ->  Text = STL.m_iBin9Cnt; break;
    }
    switch(OM.CmnOptn.iZnnrFailBin){
        case 0: edBinCount2 ->  Text = STL.m_iBin0Cnt; break;
        case 1: edBinCount2 ->  Text = STL.m_iBin1Cnt; break;
        case 2: edBinCount2 ->  Text = STL.m_iBin2Cnt; break;
        case 3: edBinCount2 ->  Text = STL.m_iBin3Cnt; break;
        case 4: edBinCount2 ->  Text = STL.m_iBin4Cnt; break;
        case 5: edBinCount2 ->  Text = STL.m_iBin5Cnt; break;
        case 6: edBinCount2 ->  Text = STL.m_iBin6Cnt; break;
        case 7: edBinCount2 ->  Text = STL.m_iBin7Cnt; break;
        case 8: edBinCount2 ->  Text = STL.m_iBin8Cnt; break;
        case 9: edBinCount2 ->  Text = STL.m_iBin9Cnt; break;
    }
    switch(OM.CmnOptn.iDropFailBin){
        case 0: edBinCount3 ->  Text = STL.m_iBin0Cnt; break;
        case 1: edBinCount3 ->  Text = STL.m_iBin1Cnt; break;
        case 2: edBinCount3 ->  Text = STL.m_iBin2Cnt; break;
        case 3: edBinCount3 ->  Text = STL.m_iBin3Cnt; break;
        case 4: edBinCount3 ->  Text = STL.m_iBin4Cnt; break;
        case 5: edBinCount3 ->  Text = STL.m_iBin5Cnt; break;
        case 6: edBinCount3 ->  Text = STL.m_iBin6Cnt; break;
        case 7: edBinCount3 ->  Text = STL.m_iBin7Cnt; break;
        case 8: edBinCount3 ->  Text = STL.m_iBin8Cnt; break;
        case 9: edBinCount3 ->  Text = STL.m_iBin9Cnt; break;
    }
    switch(OM.CmnOptn.iAllFailBin){
        case 0: edBinCount4 ->  Text = STL.m_iBin0Cnt; break;
        case 1: edBinCount4 ->  Text = STL.m_iBin1Cnt; break;
        case 2: edBinCount4 ->  Text = STL.m_iBin2Cnt; break;
        case 3: edBinCount4 ->  Text = STL.m_iBin3Cnt; break;
        case 4: edBinCount4 ->  Text = STL.m_iBin4Cnt; break;
        case 5: edBinCount4 ->  Text = STL.m_iBin5Cnt; break;
        case 6: edBinCount4 ->  Text = STL.m_iBin6Cnt; break;
        case 7: edBinCount4 ->  Text = STL.m_iBin7Cnt; break;
        case 8: edBinCount4 ->  Text = STL.m_iBin8Cnt; break;
        case 9: edBinCount4 ->  Text = STL.m_iBin9Cnt; break;
    }
    switch(OM.CmnOptn.iLowVfBin){
        case 0: edBinCount5 ->  Text = STL.m_iBin0Cnt; break;
        case 1: edBinCount5 ->  Text = STL.m_iBin1Cnt; break;
        case 2: edBinCount5 ->  Text = STL.m_iBin2Cnt; break;
        case 3: edBinCount5 ->  Text = STL.m_iBin3Cnt; break;
        case 4: edBinCount5 ->  Text = STL.m_iBin4Cnt; break;
        case 5: edBinCount5 ->  Text = STL.m_iBin5Cnt; break;
        case 6: edBinCount5 ->  Text = STL.m_iBin6Cnt; break;
        case 7: edBinCount5 ->  Text = STL.m_iBin7Cnt; break;
        case 8: edBinCount5 ->  Text = STL.m_iBin8Cnt; break;
        case 9: edBinCount5 ->  Text = STL.m_iBin9Cnt; break;
    }
    switch(OM.CmnOptn.iHighVfBin){
        case 0: edBinCount6 ->  Text = STL.m_iBin0Cnt; break;
        case 1: edBinCount6 ->  Text = STL.m_iBin1Cnt; break;
        case 2: edBinCount6 ->  Text = STL.m_iBin2Cnt; break;
        case 3: edBinCount6 ->  Text = STL.m_iBin3Cnt; break;
        case 4: edBinCount6 ->  Text = STL.m_iBin4Cnt; break;
        case 5: edBinCount6 ->  Text = STL.m_iBin5Cnt; break;
        case 6: edBinCount6 ->  Text = STL.m_iBin6Cnt; break;
        case 7: edBinCount6 ->  Text = STL.m_iBin7Cnt; break;
        case 8: edBinCount6 ->  Text = STL.m_iBin8Cnt; break;
        case 9: edBinCount6 ->  Text = STL.m_iBin9Cnt; break;
    }

    if(OM.MstOptn.bUsedVfSortOption){
        bView = true;
    }

    else{
        bView = false;
    }

    if(bView == bPreView) {
        Timer1 -> Enabled = true;
        return ;
    }

    if(OM.MstOptn.bUsedVfSortOption){
        Label8       -> Visible = true;
        Label9       -> Visible = true;
        edLowVf      -> Visible = true;
        edHighVf     -> Visible = true;

        edLowVfBin   -> Visible = true;
        edHighVfBin  -> Visible = true;
    }
    else{
        Label8       -> Visible = false;
        Label9       -> Visible = false;
        edLowVf      -> Visible = false;
        edHighVf     -> Visible = false;

        edLowVfBin   -> Visible = false;
        edHighVfBin  -> Visible = false;
    }


    bPreView = bView;

    Timer1 -> Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOption::btServerFolderClick(TObject *Sender)
{
    String Dir;
    SelectDirectory("Select a directory", "", Dir);
    ShowMessage(Dir);
    edMESPath -> Text = Dir ;
}
//---------------------------------------------------------------------------



