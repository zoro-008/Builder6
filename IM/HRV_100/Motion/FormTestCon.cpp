//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTestCon.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
//#include "SPCUnit.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmTestCon *FrmTestCon;
//---------------------------------------------------------------------------
__fastcall TFrmTestCon::TFrmTestCon(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmTestCon::btSaveClick(TObject *Sender)
{
    //Check Running Status.
//    if (SEQ._bRun) {
//        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
//        return;
//    }
//    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());
}

//---------------------------------------------------------------------------
void __fastcall TFrmTestCon::UpdateDevOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbUseDuplex       -> ItemIndex = OM.DevOptn.bUseDuplex ? 1:0 ;
        edTestCrntMin     -> Text      = OM.DevOptn.iTestCrntMin     ;
        edTestCrntMax     -> Text      = OM.DevOptn.iTestCrntMax     ;

        edStableDelay     -> Text      = OM.DevOptn.iStableDelay     ;
        edStepDelay       -> Text      = OM.DevOptn.iStepDelay       ;
        edStepIncrement   -> Text      = OM.DevOptn.iStepIncrement   ;
        edDrvTtStrtCrnt   -> Text      = OM.DevOptn.iDrvTtStrtCrnt   ;
        edDrvTtTestCrnt   -> Text      = OM.DevOptn.iDrvTtTestCrnt   ;
        edCustomStroke    -> Text      = OM.DevOptn.iCustomStroke    ;
        edPreLoadCheckHt  -> Text      = OM.DevOptn.iPreLoadCheckHt  ;
        edImpdCheckCrnt   -> Text      = OM.DevOptn.iImpdCheckCrnt   ;
        edSensiSttCrnt    -> Text      = OM.DevOptn.iSensiSttCrnt    ;
        edSensiEndCrnt    -> Text      = OM.DevOptn.iSensiEndCrnt    ;

        edLinearSttCrnt   -> Text      = OM.DevOptn.iLinearSttCrnt   ;
        edLinearEndCrnt   -> Text      = OM.DevOptn.iLinearEndCrnt   ;

        cbUseInitBfCtt    -> Checked   = OM.DevOptn.bUseBfCttOfs     ;
        edGraphMaxY       -> Text      = OM.DevOptn.iGraphMaxY       ;
        edGraphMinY       -> Text      = OM.DevOptn.iGraphMinY       ;

        edHysteSttCrnt    -> Text      = OM.DevOptn.iHysteSttCrnt    ;
        edHysteEndCrnt    -> Text      = OM.DevOptn.iHysteEndCrnt    ;

        edPreHysteMaxOfst -> Text      = OM.DevOptn.dPreHysteMaxOfst ;



        cbUseTiltInitR    -> Checked   = OM.DevOptn.bUseTiltInitR    ;
        cbUseTiltInitT    -> Checked   = OM.DevOptn.bUseTiltInitT    ;
        cbUseTiltInit     -> Checked   = OM.DevOptn.bUseTiltInit     ;
        cbUseTiltDrive    -> Checked   = OM.DevOptn.bUseTiltDrive    ;
        cbUseTiltDriveAbs -> Checked   = OM.DevOptn.bUseTiltDriveAbs ;
        cbUseTiltFull     -> Checked   = OM.DevOptn.bUseTiltFull     ;
        cbUseTiltMax      -> Checked   = OM.DevOptn.bUseTiltMax      ;
        cbUseTiltLength   -> Checked   = OM.DevOptn.bUseTiltLength   ;

        cbUsePreLoad      -> Checked   = OM.DevOptn.bUsePreLoad      ;
        cbUsePeakStroke   -> Checked   = OM.DevOptn.bUsePeakStroke   ;
        cbUseCustomStroke -> Checked   = OM.DevOptn.bUseCustomStroke ;
        cbUseSensitivity  -> Checked   = OM.DevOptn.bUseSensitivity  ;
        cbUseLinearity    -> Checked   = OM.DevOptn.bUseLinearity    ;
        cbUseImpedance    -> Checked   = OM.DevOptn.bUseImpedance    ;
        cbUseHysteresis   -> Checked   = OM.DevOptn.bUseHysteresis   ;
        cbUseCaseShort    -> Checked   = OM.DevOptn.bUseCaseShort    ;


        edMinTiltInitR    -> Text      = OM.DevOptn.dMinTiltInitR    ;
        edMinTiltInitT    -> Text      = OM.DevOptn.dMinTiltInitT    ;
        edMinTiltInit     -> Text      = OM.DevOptn.dMinTiltInit     ;
        edMinTiltDrive    -> Text      = OM.DevOptn.dMinTiltDrive    ;
        edMinTiltDriveAbs -> Text      = OM.DevOptn.dMinTiltDriveAbs ;
        edMinTiltFull     -> Text      = OM.DevOptn.dMinTiltFull     ;
        edMinTiltMax      -> Text      = OM.DevOptn.dMinTiltMax      ;
        edMinTiltLength   -> Text      = OM.DevOptn.dMinTiltLength   ;


        edMinPreLoad      -> Text      = OM.DevOptn.dMinPreLoad      ;
        edMinPeakStroke   -> Text      = OM.DevOptn.dMinPeakStroke   ;
        edMinCustomStroke -> Text      = OM.DevOptn.dMinCustomStroke ;
        edMinSensitivity  -> Text      = OM.DevOptn.dMinSensitivity  ;
        edMinLinearity    -> Text      = OM.DevOptn.dMinLinearity    ;
        edMinImpedance    -> Text      = OM.DevOptn.dMinImpedance    ;
        edMinHysteresis   -> Text      = OM.DevOptn.dMinHysteresis   ;

        edMaxTiltInitR    -> Text      = OM.DevOptn.dMaxTiltInitR    ;
        edMaxTiltInitT    -> Text      = OM.DevOptn.dMaxTiltInitT    ;
        edMaxTiltInit     -> Text      = OM.DevOptn.dMaxTiltInit     ;
        edMaxTiltDrive    -> Text      = OM.DevOptn.dMaxTiltDrive    ;
        edMaxTiltDriveAbs -> Text      = OM.DevOptn.dMaxTiltDriveAbs ;
        edMaxTiltFull     -> Text      = OM.DevOptn.dMaxTiltFull     ;
        edMaxTiltMax      -> Text      = OM.DevOptn.dMaxTiltMax      ;
        edMaxTiltLength   -> Text      = OM.DevOptn.dMaxTiltLength   ;

        edMaxPreLoad      -> Text      = OM.DevOptn.dMaxPreLoad      ;
        edMaxPeakStroke   -> Text      = OM.DevOptn.dMaxPeakStroke   ;
        edMaxCustomStroke -> Text      = OM.DevOptn.dMaxCustomStroke ;
        edMaxSensitivity  -> Text      = OM.DevOptn.dMaxSensitivity  ;
        edMaxLinearity    -> Text      = OM.DevOptn.dMaxLinearity    ;
        edMaxImpedance    -> Text      = OM.DevOptn.dMaxImpedance    ;
        edMaxHysteresis   -> Text      = OM.DevOptn.dMaxHysteresis   ;
    }
    else {
        OM.DevOptn.bUseDuplex          = cbUseDuplex -> ItemIndex    ;
        OM.DevOptn.iTestCrntMin        = StrToIntDef   (edTestCrntMin     -> Text    , OM.DevOptn.iTestCrntMin   );
        OM.DevOptn.iTestCrntMax        = StrToIntDef   (edTestCrntMax     -> Text    , OM.DevOptn.iTestCrntMax   );


        OM.DevOptn.iStableDelay        = StrToIntDef   (edStableDelay     -> Text    , OM.DevOptn.iStableDelay   );
        OM.DevOptn.iStepDelay          = StrToIntDef   (edStepDelay       -> Text    , OM.DevOptn.iStepDelay     );
        OM.DevOptn.iStepIncrement      = StrToIntDef   (edStepIncrement   -> Text    , OM.DevOptn.iStepIncrement );
        OM.DevOptn.iDrvTtStrtCrnt      = StrToIntDef   (edDrvTtStrtCrnt   -> Text    , OM.DevOptn.iDrvTtStrtCrnt );
        OM.DevOptn.iDrvTtTestCrnt      = StrToIntDef   (edDrvTtTestCrnt   -> Text    , OM.DevOptn.iDrvTtTestCrnt );
        OM.DevOptn.iCustomStroke       = StrToIntDef   (edCustomStroke    -> Text    , OM.DevOptn.iCustomStroke  );
        OM.DevOptn.iPreLoadCheckHt     = StrToIntDef   (edPreLoadCheckHt  -> Text    , OM.DevOptn.iPreLoadCheckHt);
        OM.DevOptn.iImpdCheckCrnt      = StrToIntDef   (edImpdCheckCrnt   -> Text    , OM.DevOptn.iImpdCheckCrnt );
        OM.DevOptn.iSensiSttCrnt       = StrToIntDef   (edSensiSttCrnt    -> Text    , OM.DevOptn.iSensiSttCrnt  );
        OM.DevOptn.iSensiEndCrnt       = StrToIntDef   (edSensiEndCrnt    -> Text    , OM.DevOptn.iSensiEndCrnt  );
        OM.DevOptn.iLinearSttCrnt      = StrToIntDef   (edLinearSttCrnt   -> Text    , OM.DevOptn.iLinearSttCrnt );
        OM.DevOptn.iLinearEndCrnt      = StrToIntDef   (edLinearEndCrnt   -> Text    , OM.DevOptn.iLinearEndCrnt );

        OM.DevOptn.bUseBfCttOfs        =                cbUseInitBfCtt    -> Checked ;
        OM.DevOptn.iGraphMaxY          = StrToIntDef   (edGraphMaxY       -> Text    , OM.DevOptn.iGraphMaxY     );
        OM.DevOptn.iGraphMinY          = StrToIntDef   (edGraphMinY       -> Text    , OM.DevOptn.iGraphMinY     );

        if(StrToIntDef(edHysteSttCrnt-> Text , OM.DevOptn.iHysteSttCrnt) >= StrToFloatDef(edPreHysteMaxOfst-> Text , OM.DevOptn.dPreHysteMaxOfst)){
            FM_MsgOk("Error","'Hysteresis Check Start' is have to be smaller than 'Pre Hysteresis Max Offset'");
        }
        else {
            OM.DevOptn.iHysteSttCrnt       = StrToIntDef   (edHysteSttCrnt    -> Text    , OM.DevOptn.iHysteSttCrnt  );
            OM.DevOptn.dPreHysteMaxOfst    = StrToFloatDef (edPreHysteMaxOfst -> Text    , OM.DevOptn.dPreHysteMaxOfst);
        }

        OM.DevOptn.iHysteEndCrnt       = StrToIntDef   (edHysteEndCrnt    -> Text    , OM.DevOptn.iHysteEndCrnt  );


        OM.DevOptn.bUseTiltInitR       =                cbUseTiltInitR    -> Checked ;
        OM.DevOptn.bUseTiltInitT       =                cbUseTiltInitT    -> Checked ;
        OM.DevOptn.bUseTiltInit        =                cbUseTiltInit     -> Checked ;
        OM.DevOptn.bUseTiltDrive       =                cbUseTiltDrive    -> Checked ;
        OM.DevOptn.bUseTiltDriveAbs    =                cbUseTiltDriveAbs -> Checked ;
        OM.DevOptn.bUseTiltFull        =                cbUseTiltFull     -> Checked ;
        OM.DevOptn.bUseTiltMax         =                cbUseTiltMax      -> Checked ;
        OM.DevOptn.bUseTiltLength      =                cbUseTiltLength   -> Checked ;

        OM.DevOptn.bUsePreLoad         =                cbUsePreLoad      -> Checked ;
        OM.DevOptn.bUsePeakStroke      =                cbUsePeakStroke   -> Checked ;
        OM.DevOptn.bUseCustomStroke    =                cbUseCustomStroke -> Checked ;
        OM.DevOptn.bUseSensitivity     =                cbUseSensitivity  -> Checked ;
        OM.DevOptn.bUseLinearity       =                cbUseLinearity    -> Checked ;
        OM.DevOptn.bUseImpedance       =                cbUseImpedance    -> Checked ;
        OM.DevOptn.bUseHysteresis      =                cbUseHysteresis   -> Checked ;
        OM.DevOptn.bUseCaseShort       =                cbUseCaseShort    -> Checked ;


        OM.DevOptn.dMinTiltInitR       = StrToFloatDef (edMinTiltInitR    -> Text    , OM.DevOptn.dMinTiltInitR   );
        OM.DevOptn.dMinTiltInitT       = StrToFloatDef (edMinTiltInitT    -> Text    , OM.DevOptn.dMinTiltInitT   );
        OM.DevOptn.dMinTiltInit        = StrToFloatDef (edMinTiltInit     -> Text    , OM.DevOptn.dMinTiltInit    );
        OM.DevOptn.dMinTiltDrive       = StrToFloatDef (edMinTiltDrive    -> Text    , OM.DevOptn.dMinTiltDrive   );
        OM.DevOptn.dMinTiltDriveAbs    = StrToFloatDef (edMinTiltDriveAbs -> Text    , OM.DevOptn.dMinTiltDriveAbs);
        OM.DevOptn.dMinTiltFull        = StrToFloatDef (edMinTiltFull     -> Text    , OM.DevOptn.dMinTiltFull    );
        OM.DevOptn.dMinTiltMax         = StrToFloatDef (edMinTiltMax      -> Text    , OM.DevOptn.dMinTiltMax     );
        OM.DevOptn.dMinTiltLength      = StrToFloatDef (edMinTiltLength   -> Text    , OM.DevOptn.dMinTiltLength  );


        OM.DevOptn.dMinPreLoad         = StrToFloatDef (edMinPreLoad      -> Text    , OM.DevOptn.dMinPreLoad     );
        OM.DevOptn.dMinPeakStroke      = StrToFloatDef (edMinPeakStroke   -> Text    , OM.DevOptn.dMinPeakStroke  );
        OM.DevOptn.dMinCustomStroke    = StrToFloatDef (edMinCustomStroke -> Text    , OM.DevOptn.dMinCustomStroke);
        OM.DevOptn.dMinSensitivity     = StrToFloatDef (edMinSensitivity  -> Text    , OM.DevOptn.dMinSensitivity );
        OM.DevOptn.dMinLinearity       = StrToFloatDef (edMinLinearity    -> Text    , OM.DevOptn.dMinLinearity   );
        OM.DevOptn.dMinImpedance       = StrToFloatDef (edMinImpedance    -> Text    , OM.DevOptn.dMinImpedance   );
        OM.DevOptn.dMinHysteresis      = StrToFloatDef (edMinHysteresis   -> Text    , OM.DevOptn.dMinHysteresis  );

        OM.DevOptn.dMaxTiltInitR       = StrToFloatDef (edMaxTiltInitR    -> Text    , OM.DevOptn.dMaxTiltInitR   );
        OM.DevOptn.dMaxTiltInitT       = StrToFloatDef (edMaxTiltInitT    -> Text    , OM.DevOptn.dMaxTiltInitT   );
        OM.DevOptn.dMaxTiltInit        = StrToFloatDef (edMaxTiltInit     -> Text    , OM.DevOptn.dMaxTiltInit    );
        OM.DevOptn.dMaxTiltDrive       = StrToFloatDef (edMaxTiltDrive    -> Text    , OM.DevOptn.dMaxTiltDrive   );
        OM.DevOptn.dMaxTiltDriveAbs    = StrToFloatDef (edMaxTiltDriveAbs -> Text    , OM.DevOptn.dMaxTiltDriveAbs);
        OM.DevOptn.dMaxTiltFull        = StrToFloatDef (edMaxTiltFull     -> Text    , OM.DevOptn.dMaxTiltFull    );
        OM.DevOptn.dMaxTiltMax         = StrToFloatDef (edMaxTiltMax      -> Text    , OM.DevOptn.dMaxTiltMax     );
        OM.DevOptn.dMaxTiltLength      = StrToFloatDef (edMaxTiltLength   -> Text    , OM.DevOptn.dMaxTiltLength  );


        OM.DevOptn.dMaxPreLoad         = StrToFloatDef (edMaxPreLoad      -> Text    , OM.DevOptn.dMaxPreLoad     );
        OM.DevOptn.dMaxPeakStroke      = StrToFloatDef (edMaxPeakStroke   -> Text    , OM.DevOptn.dMaxPeakStroke  );
        OM.DevOptn.dMaxCustomStroke    = StrToFloatDef (edMaxCustomStroke -> Text    , OM.DevOptn.dMaxCustomStroke);
        OM.DevOptn.dMaxSensitivity     = StrToFloatDef (edMaxSensitivity  -> Text    , OM.DevOptn.dMaxSensitivity );
        OM.DevOptn.dMaxLinearity       = StrToFloatDef (edMaxLinearity    -> Text    , OM.DevOptn.dMaxLinearity   );
        OM.DevOptn.dMaxImpedance       = StrToFloatDef (edMaxImpedance    -> Text    , OM.DevOptn.dMaxImpedance   );
        OM.DevOptn.dMaxHysteresis      = StrToFloatDef (edMaxHysteresis   -> Text    , OM.DevOptn.dMaxHysteresis  );

        UpdateDevOptn(toTabl);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmTestCon::FormShow(TObject *Sender)
{
#ifdef COWELL
    cbUsePreLoad      -> Caption = "Start Current" ;
    cbUseCustomStroke -> Caption = "Rated Stroke"  ;
    cbUseImpedance    -> Caption = "Resistance"    ;

    lbPreLoadCheckHt -> Caption = "Check StartingCurrent Height" ;
    lbImpdCheckCrnt  -> Caption = "Resistance Check Current" ;
#endif












    UpdateDevOptn(toTabl);















    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmTestCon::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmTestCon::tmUpdateTimer(TObject *Sender)
{
    edTestCrntMin     -> Visible = cbUseDuplex -> ItemIndex != 0 ;
    lbTestCrntMinUnit -> Visible = cbUseDuplex -> ItemIndex != 0 ;
    lbTestCrntMin     -> Visible = cbUseDuplex -> ItemIndex != 0 ;


    lbDrvTtStartCrnt  -> Caption = OM.MstOptn.bSetUnitHeight ? "Drive Tilt Start Height"       : "Drive Tilt Start Current"      ;
    lbDrvTtTestCrnt   -> Caption = OM.MstOptn.bSetUnitHeight ? "Drive Tilt Test Height"        : "Drive Tilt Test Current"       ;
    lbSensiSttCrnt    -> Caption = OM.MstOptn.bSetUnitHeight ? "Sensitive Check Start Height"  : "Sensitive Check Start Current" ;
    lbSensiEndCrnt    -> Caption = OM.MstOptn.bSetUnitHeight ? "Sensitive Check End Height"    : "Sensitive Check End Current"   ;
    lbHysteSttCrnt    -> Caption = OM.MstOptn.bSetUnitHeight ? "Hysteresis Check Start Height" : "Hysteresis Check Start Current";
    lbHysteEndCrnt    -> Caption = OM.MstOptn.bSetUnitHeight ? "Hysteresis Check End Height"   : "Hysteresis Check End Current"  ;

    lbDrvTtStartUnit  -> Caption = OM.MstOptn.bSetUnitHeight ? "um" : "mA" ;
    lbDrvTtTestUnit   -> Caption = OM.MstOptn.bSetUnitHeight ? "um" : "mA" ;
    lbSensiSttUnit    -> Caption = OM.MstOptn.bSetUnitHeight ? "um" : "mA" ;
    lbSensiEndUnit    -> Caption = OM.MstOptn.bSetUnitHeight ? "um" : "mA" ;
    lbHysteSttUnit    -> Caption = OM.MstOptn.bSetUnitHeight ? "um" : "mA" ;
    lbHysteEndUnit    -> Caption = OM.MstOptn.bSetUnitHeight ? "um" : "mA" ;
}
//---------------------------------------------------------------------------


