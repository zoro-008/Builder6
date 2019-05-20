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
#include "UserINI.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
    //Load Language
//    LAG.LanguageSel(FrmOption,FM_GetLan());
//    LanguageSel(FM_GetLan());
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
        edLimAllFailCnt    -> Text    = OM.CmnOptn.iCheckFailCnt            ;
    }
    else {
        OM.CmnOptn.bDryRun          = cbDryRun         -> Checked         ;
        OM.CmnOptn.bIgnrDoor        = cbIgnrDoor       -> Checked         ;
        OM.CmnOptn.bIgnrInsp        = cbIgnrInsp       -> Checked         ;
        OM.CmnOptn.bIgnrIdCehck     = cbIgnrIdCheck    -> Checked         ;
        OM.CmnOptn.bIgnrHeatAlm     = cbIgnrHeatAlarm  -> Checked         ;
        OM.CmnOptn.bIgnrStrOverAlm  = cbIgnrStrOverAlm -> Checked         ;
        OM.CmnOptn.bAutoOperation   = cbAutoOperation  -> Checked         ;
        OM.CmnOptn.bAutoConvesion   = cbAutoConvesion  -> Checked         ;
        OM.CmnOptn.bLoadingStop     = cbLoadingStop    -> Checked         ;
        OM.CmnOptn.bCheckFailCnt    = cbHeatFail       -> Checked         ;
        OM.CmnOptn.iCheckFailCnt    = edLimAllFailCnt  -> Text.ToIntDef(0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOption::LanguageSel(EN_LAN_SEL lvType)
{
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sName    , sNameTemp    ;
    AnsiString sCaption , sCaptionTemp ;
    AnsiString sComName ;
    bool       bOri     ;

    for (int i = 0; i <= MAX_LAN_SEL   ; i++)
    {
             if(i == (int)lsEnglish ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_E.ini" ; bOri = false ;}
        else if(i == (int)lsKorean  ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_K.ini" ; bOri = false ;}
        else if(i == (int)lsChinese ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_C.ini" ; bOri = false ;}
        else                          {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + ".ini"   ;
                                       UserINI.ClearFile(sPath) ;
                                       bOri  = true ;                                                  }

        for (int i = 0; i < ComponentCount; i++)
        {
            if (Components[i]->ClassNameIs("TLabel"))
            {
                sComName = Components[i]->ClassName();
                sName    = ((TLabel*)FindComponent(Components[i]->Name))->Name          ;
                sCaption = ((TLabel*)FindComponent(Components[i]->Name))->Caption       ;
            }
            else if (Components[i]->ClassNameIs("TCheckBox")) {
                sComName = Components[i]->ClassName();
                sName    = ((TCheckBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TCheckBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else if (Components[i]->ClassNameIs("TButton")) {
                sComName = Components[i]->ClassName();
                sName    = ((TButton*)FindComponent(Components[i]->Name))->Name         ;
                sCaption = ((TButton*)FindComponent(Components[i]->Name))->Caption      ;
            }
            else if (Components[i]->ClassNameIs("TBitBtn")) {
                sComName = Components[i]->ClassName();
                sName    = ((TBitBtn*)FindComponent(Components[i]->Name))->Name         ;
                sCaption = ((TBitBtn*)FindComponent(Components[i]->Name))->Caption      ;
            }
            else if (Components[i]->ClassNameIs("TSpeedButton")) {
                sComName = Components[i]->ClassName();
                sName    = ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    ;
                sCaption = ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption ;
            }
//            else if (Components[i]->ClassNameIs("TTabSheet")) {
//                sComName = Components[i]->ClassName();
//                sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
//                sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
//            }
            else if (Components[i]->ClassNameIs("TGroupBox")) {
                sComName = Components[i]->ClassName();
                sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else continue ;
            
            if(sComName == "" || sName == "") continue ;

            //Save Device.
            if(bOri) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
            else if(sCaption != "") {
                UserINI.Load(sPath.c_str()  , sComName  , sName , sCaptionTemp);
                if( sCaptionTemp == "" ) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
            }
        }
    }

    //Set Dir.
         if(lvType == lsEnglish ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_E.ini" ;
    else if(lvType == lsKorean  ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_K.ini" ;
    else if(lvType == lsChinese ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_C.ini" ;
    else                          return true ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TLabel"))
        {
            sComName = Components[i]->ClassName();
            sName    = ((TLabel*)FindComponent(Components[i]->Name))->Name          ;
            sCaption = ((TLabel*)FindComponent(Components[i]->Name))->Caption       ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TLabel*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TLabel*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TCheckBox")) {
            sComName = Components[i]->ClassName();
            sName    = ((TCheckBox*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TCheckBox*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TCheckBox*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TCheckBox*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TButton")) {
            sComName = Components[i]->ClassName();
            sName    = ((TButton*)FindComponent(Components[i]->Name))->Name         ;
            sCaption = ((TButton*)FindComponent(Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TButton*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TButton*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TBitBtn")) {
            sComName = Components[i]->ClassName();
            sName    = ((TBitBtn*)FindComponent(Components[i]->Name))->Name         ;
            sCaption = ((TBitBtn*)FindComponent(Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TBitBtn*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TSpeedButton")) {
            sComName = Components[i]->ClassName();
            sName    = ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    ;
            sCaption = ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
//        else if (Components[i]->ClassNameIs("TTabSheet")) {
//            sComName = Components[i]->ClassName();
//            sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
//            sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
//            if(sComName == "" || sName == "") continue ;
//            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
//            ((TTabSheet*)FindComponent(Components[i]->Name))->Name    = sName          ;
//            ((TTabSheet*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
//        }
        else if (Components[i]->ClassNameIs("TGroupBox")) {
            sComName = Components[i]->ClassName();
            sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TGroupBox*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TGroupBox*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
    }

    return true ;
}
