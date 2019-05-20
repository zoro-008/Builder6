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
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
    LAG.ReadFrmOption   ();
//    if(FM_GetLan() == lsKorean) LAG.LoadFrmOptionHan();
    switch(FM_GetLan())
    {
        default        :                             break ;
        case lsEnglish :                             break ;
        case lsKorean  : LAG.LoadFrmOptionHan();     break ;
        case lsChinese :                             break ;
    }
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
    if(cbIgnrIdCheck     -> Checked != OM.CmnOptn.bIgnrIdCehck   ) SPC.SaveOptionLog(cbIgnrIdCheck   ->Tag,false);
    if(cbIgnrInsp        -> Checked != OM.CmnOptn.bIgnrInsp      ) SPC.SaveOptionLog(cbIgnrInsp      ->Tag,false);
    if(cbIgnrHeatAlarm   -> Checked != OM.CmnOptn.bIgnrHeatAlm   ) SPC.SaveOptionLog(cbIgnrHeatAlarm ->Tag,false);
    if(cbIgnrStrOverAlm  -> Checked != OM.CmnOptn.bIgnrStrOverAlm) SPC.SaveOptionLog(cbIgnrStrOverAlm->Tag,false);
    if(cbLoadingStop     -> Checked != OM.CmnOptn.bLoadingStop   ) SPC.SaveOptionLog(cbLoadingStop   ->Tag,false);
    if(cbHeatFail        -> Checked != OM.CmnOptn.bIgnrHeatAlm   ) SPC.SaveOptionLog(cbHeatFail      ->Tag,false);
    if(cbAutoConvesion   -> Checked != OM.CmnOptn.bAutoConvesion ) SPC.SaveOptionLog(cbAutoConvesion ->Tag,false);
    if(cbAutoOperation   -> Checked != OM.CmnOptn.bAutoOperation ) SPC.SaveOptionLog(cbAutoOperation ->Tag,false);
    if(cbIgnrTrim        -> Checked != OM.CmnOptn.bIgnrTrim      ) SPC.SaveOptionLog(cbIgnrTrim      ->Tag,false);
    if(cbIgnrVacuum      -> Checked != OM.CmnOptn.bIgnrVacuum    ) SPC.SaveOptionLog(cbIgnrVacuum    ->Tag,false);
    if(cbIgnrDecChk      -> Checked != OM.CmnOptn.bIgnrDecChk    ) SPC.SaveOptionLog(cbIgnrDecChk    ->Tag,false);
    if(cbIgnrTrimDecChk  -> Checked != OM.CmnOptn.bIgnrTrimDecChk) SPC.SaveOptionLog(cbIgnrTrimDecChk->Tag,false);

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();
    OM.SaveEqpOptn();
//    UserMsg2("Confirm" , "저장 되었습니다.");

}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbDryRun           -> Checked = OM.CmnOptn.bDryRun                  ;
        cbIgnrDoor         -> Checked = OM.CmnOptn.bIgnrDoor                ;
        cbIgnrInsp         -> Checked = OM.CmnOptn.bIgnrInsp                ;
        cbIgnrIdCheck      -> Checked = OM.CmnOptn.bIgnrIdCehck             ;
        cbIgnrHeatAlarm    -> Checked = OM.CmnOptn.bIgnrHeatAlm             ;
        cbIgnrStrOverAlm   -> Checked = OM.CmnOptn.bIgnrStrOverAlm          ;
        cbAutoOperation    -> Checked = OM.CmnOptn.bAutoOperation           ;
        cbAutoConvesion    -> Checked = OM.CmnOptn.bAutoConvesion           ;
        cbLoadingStop      -> Checked = OM.CmnOptn.bLoadingStop             ;
        cbHeatFail         -> Checked = OM.CmnOptn.bCheckFailCnt            ;
        cbIgnrTrim         -> Checked = OM.CmnOptn.bIgnrTrim                ;
        edLimAllFailCnt    -> Text    = OM.CmnOptn.iCheckFailCnt            ;
        cbIgnrVacuum       -> Checked = OM.CmnOptn.bIgnrVacuum              ;
        cbSortSersor       -> Checked = OM.CmnOptn.bSortSensorDelay         ;
        edSortSensorDelay  -> Text    = OM.CmnOptn.iSortSensorDelay         ;
        cbIgnrDecChk       -> Checked = OM.CmnOptn.bIgnrDecChk              ;
        cbIgnrTrimDecChk   -> Checked = OM.CmnOptn.bIgnrTrimDecChk          ;
    }
    else {
        OM.CmnOptn.bDryRun          = cbDryRun          -> Checked         ;
        OM.CmnOptn.bIgnrDoor        = cbIgnrDoor        -> Checked         ;
        OM.CmnOptn.bIgnrInsp        = cbIgnrInsp        -> Checked         ;
        OM.CmnOptn.bIgnrIdCehck     = cbIgnrIdCheck     -> Checked         ;
        OM.CmnOptn.bIgnrHeatAlm     = cbIgnrHeatAlarm   -> Checked         ;
        OM.CmnOptn.bIgnrStrOverAlm  = cbIgnrStrOverAlm  -> Checked         ;
        OM.CmnOptn.bAutoOperation   = cbAutoOperation   -> Checked         ;
        OM.CmnOptn.bAutoConvesion   = cbAutoConvesion   -> Checked         ;
        OM.CmnOptn.bLoadingStop     = cbLoadingStop     -> Checked         ;
        OM.CmnOptn.bCheckFailCnt    = cbHeatFail        -> Checked         ;
        OM.CmnOptn.bIgnrTrim        = cbIgnrTrim        -> Checked         ;
        OM.CmnOptn.iCheckFailCnt    = edLimAllFailCnt   -> Text.ToIntDef(0);
        OM.CmnOptn.bIgnrVacuum      = cbIgnrVacuum      -> Checked         ;
        OM.CmnOptn.bSortSensorDelay = cbSortSersor      -> Checked         ;
        OM.CmnOptn.iSortSensorDelay = edSortSensorDelay -> Text.ToIntDef(0);
        OM.CmnOptn.bIgnrDecChk      = cbIgnrDecChk      -> Checked         ;
        OM.CmnOptn.bIgnrTrimDecChk  = cbIgnrTrimDecChk  -> Checked         ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------



