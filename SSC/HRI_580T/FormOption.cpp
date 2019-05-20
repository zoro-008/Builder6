//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
#include "DataMan.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
    //Load Language
//    LanguageSel(FM_GetLan());
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::btSaveClick(TObject *Sender)
{
     //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "저장 할 수 없습니다.\n장비 RUNNING중 입니다.");
        return;
    }
    if (FM_MsgYesNo("Confirm" , "저장 하시겠습니까?") != mrYes)  return;

    //Log Save d:\Spc\OptionLog
    AnsiString sMsg = "";
    if(cbDryRun       -> Checked != OM.CmnOptn.bDryRun        ) sMsg = "DryRun "       + AnsiString(OM.CmnOptn.bDryRun        ? "Off" : "On") ;
    if(cbNoTrimFail   -> Checked != OM.CmnOptn.bNoTrimFail    ) sMsg = "NoTrimFail "   + AnsiString(OM.CmnOptn.bNoTrimFail    ? "Off" : "On") ;
    if(cbIgnrDoor     -> Checked != OM.CmnOptn.bIgnrDoor      ) sMsg = "IgnrDoor "     + AnsiString(OM.CmnOptn.bIgnrDoor      ? "Off" : "On") ;
    if(cbVisn1Skip    -> Checked != OM.CmnOptn.bVisn1Skip     ) sMsg = "Visn1Skip "    + AnsiString(OM.CmnOptn.bVisn1Skip     ? "Off" : "On") ;
    if(cbVisn2Skip    -> Checked != OM.CmnOptn.bVisn2Skip     ) sMsg = "Visn2Skip "    + AnsiString(OM.CmnOptn.bVisn2Skip     ? "Off" : "On") ;
    if(cbSortByItem   -> Checked != OM.CmnOptn.bSortByItem    ) sMsg = "SortByItem "   + AnsiString(OM.CmnOptn.bSortByItem    ? "Off" : "On") ;
    if(cbAutoVsChange -> Checked != OM.CmnOptn.bAutoVsChange  ) sMsg = "AutoVsChange " + AnsiString(OM.CmnOptn.bAutoVsChange  ? "Off" : "On") ;
    if(cbAutoOperation-> Checked != OM.CmnOptn.bAutoOperation ) sMsg = "AutoOperation "+ AnsiString(OM.CmnOptn.bAutoOperation ? "Off" : "On") ;
    if(cbCheckSFailCnt-> Checked != OM.CmnOptn.bCheckSFailCnt ) sMsg = "CheckSFailCnt "+ AnsiString(OM.CmnOptn.bCheckSFailCnt ? "Off" : "On") ;
    if(cbCheckFailCnt -> Checked != OM.CmnOptn.bCheckFailCnt  ) sMsg = "CheckFailCnt " + AnsiString(OM.CmnOptn.bCheckFailCnt  ? "Off" : "On") ;


    AnsiString sLotNo , sSlotNo ;
    sLotNo  = "PreBuff:" + DM.ARAY[riPRB].GetLotNo() + " ";
    sLotNo += "Visn1:"   + DM.ARAY[riWK1].GetLotNo() + " ";
    sLotNo += "Visn2:"   + DM.ARAY[riWK2].GetLotNo() + " ";
    sLotNo += "PstBuff:" + DM.ARAY[riPSB].GetLotNo() + " ";

    sSlotNo  = "PreBuff:" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn1:"   + AnsiString(DM.ARAY[riWK1].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn2:"   + AnsiString(DM.ARAY[riWK2].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "PstBuff:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";

    if(sMsg != "")SPC.SaveOptionLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    //NoTrimFail
    if(cbNoTrimFail -> Checked != OM.CmnOptn.bNoTrimFail && OM.CmnOptn.bNoTrimFail && !DM.ARAY[riPSB].CheckAllStat(csNone) ) {
        if( mrYes ==FM_MsgYesNo("Confirm","Would you like to Skip Trimming PostBuffer" )) {
            DM.ARAY[riPSB].SetStat(csWork);
        }
    }







    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();
//    UserMsg2("Confirm" , "저장 되었습니다.");
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{
    if (_bToTable == toTabl) {
        cbDryRun       -> Checked = OM.CmnOptn.bDryRun       ;
        cbNoTrimFail   -> Checked = OM.CmnOptn.bNoTrimFail   ;
        cbIgnrDoor     -> Checked = OM.CmnOptn.bIgnrDoor     ;
        cbVisn1Skip    -> Checked = OM.CmnOptn.bVisn1Skip    ;
        cbVisn2Skip    -> Checked = OM.CmnOptn.bVisn2Skip    ;
        cbSortByItem   -> Checked = OM.CmnOptn.bSortByItem   ;
        cbAutoVsChange -> Checked = OM.CmnOptn.bAutoVsChange ;
        cbAutoOperation-> Checked = OM.CmnOptn.bAutoOperation;
        cbCheckFailCnt -> Checked = OM.CmnOptn.bCheckFailCnt ;
        edCheckFailCnt -> Text    = OM.CmnOptn.iCheckFailCnt ;
        cbCheckSFailCnt-> Checked = OM.CmnOptn.bCheckSFailCnt;
        edCheckSFailCnt-> Text    = OM.CmnOptn.iCheckSFailCnt;
        cbTrimFailCheck-> Checked = OM.CmnOptn.bTrimCheck    ;

        edSrtMotrDelay -> Text    = OM.CmnOptn.iSrtMotrDelay ;

        edSortItRslt1    -> Text  = OM.CmnOptn.iSortItRslt1    ;
        edSortItRslt2    -> Text  = OM.CmnOptn.iSortItRslt2    ;
        edSortItRslt3    -> Text  = OM.CmnOptn.iSortItRslt3    ;
        edSortItRslt4    -> Text  = OM.CmnOptn.iSortItRslt4    ;
        edSortItRslt5    -> Text  = OM.CmnOptn.iSortItRslt5    ;
        edSortItRslt6    -> Text  = OM.CmnOptn.iSortItRslt6    ;
        edSortItRslt7    -> Text  = OM.CmnOptn.iSortItRslt7    ;
        edSortItRslt8    -> Text  = OM.CmnOptn.iSortItRslt8    ;
        edSortItRslt9    -> Text  = OM.CmnOptn.iSortItRslt9    ;
        edSortItRslt10   -> Text  = OM.CmnOptn.iSortItRslt10   ;
        edSortItRslt11   -> Text  = OM.CmnOptn.iSortItRslt11   ;
        edSortItRslt12   -> Text  = OM.CmnOptn.iSortItRslt12   ;
        edSortItRslt13   -> Text  = OM.CmnOptn.iSortItRslt13   ;
        edSortItRslt14   -> Text  = OM.CmnOptn.iSortItRslt14   ;
        edSortItFail     -> Text  = OM.CmnOptn.iSortItFail     ;

        edLevelRslt1     -> Text  = OM.CmnOptn.iLevelRslt1     ;
        edLevelRslt2     -> Text  = OM.CmnOptn.iLevelRslt2     ;
        edLevelRslt3     -> Text  = OM.CmnOptn.iLevelRslt3     ;
        edLevelRslt4     -> Text  = OM.CmnOptn.iLevelRslt4     ;
        edLevelRslt5     -> Text  = OM.CmnOptn.iLevelRslt5     ;
        edLevelRslt6     -> Text  = OM.CmnOptn.iLevelRslt6     ;
        edLevelRslt7     -> Text  = OM.CmnOptn.iLevelRslt7     ;
        edLevelRslt8     -> Text  = OM.CmnOptn.iLevelRslt8     ;
        edLevelRslt9     -> Text  = OM.CmnOptn.iLevelRslt9     ;
        edLevelRslt10    -> Text  = OM.CmnOptn.iLevelRslt10    ;
        edLevelRslt11    -> Text  = OM.CmnOptn.iLevelRslt11    ;
        edLevelRslt12    -> Text  = OM.CmnOptn.iLevelRslt12    ;
        edLevelRslt13    -> Text  = OM.CmnOptn.iLevelRslt13    ;
        edLevelRslt14    -> Text  = OM.CmnOptn.iLevelRslt14    ;
        edLevelFail      -> Text  = OM.CmnOptn.iLevelFail      ;


    }
    else {
        OM.CmnOptn.bDryRun        = cbDryRun        -> Checked ;
        OM.CmnOptn.bNoTrimFail    = cbNoTrimFail    -> Checked ;
        OM.CmnOptn.bIgnrDoor      = cbIgnrDoor      -> Checked ;
        OM.CmnOptn.bVisn1Skip     = cbVisn1Skip     -> Checked ;
        OM.CmnOptn.bVisn2Skip     = cbVisn2Skip     -> Checked ;

        OM.CmnOptn.bSortByItem    = cbSortByItem    -> Checked ;
        OM.CmnOptn.bAutoVsChange  = cbAutoVsChange  -> Checked ;
        OM.CmnOptn.bAutoOperation = cbAutoOperation -> Checked ;

        OM.CmnOptn.bCheckFailCnt  = cbCheckFailCnt  -> Checked ;
        OM.CmnOptn.iCheckFailCnt  = edCheckFailCnt  -> Text.ToIntDef(0) ;
        OM.CmnOptn.bCheckSFailCnt = cbCheckSFailCnt -> Checked ;
        OM.CmnOptn.iCheckSFailCnt = edCheckSFailCnt -> Text.ToIntDef(0) ;
        OM.CmnOptn.bTrimCheck     = cbTrimFailCheck -> Checked ;

        OM.CmnOptn.iSrtMotrDelay  = edSrtMotrDelay  -> Text.ToIntDef(1000);



        OM.CmnOptn.iSortItRslt1    = edSortItRslt1    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt2    = edSortItRslt2    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt3    = edSortItRslt3    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt4    = edSortItRslt4    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt5    = edSortItRslt5    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt6    = edSortItRslt6    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt7    = edSortItRslt7    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt8    = edSortItRslt8    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt9    = edSortItRslt9    -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt10   = edSortItRslt10   -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt11   = edSortItRslt11   -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt12   = edSortItRslt12   -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt13   = edSortItRslt13   -> Text.ToInt()    ;
        OM.CmnOptn.iSortItRslt14   = edSortItRslt14   -> Text.ToInt()    ;
        OM.CmnOptn.iSortItFail     = edSortItFail     -> Text.ToInt()    ;

        OM.CmnOptn.iLevelRslt1     = edLevelRslt1     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt2     = edLevelRslt2     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt3     = edLevelRslt3     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt4     = edLevelRslt4     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt5     = edLevelRslt5     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt6     = edLevelRslt6     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt7     = edLevelRslt7     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt8     = edLevelRslt8     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt9     = edLevelRslt9     -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt10    = edLevelRslt10    -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt11    = edLevelRslt11    -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt12    = edLevelRslt12    -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt13    = edLevelRslt13    -> Text.ToInt()    ;
        OM.CmnOptn.iLevelRslt14    = edLevelRslt14    -> Text.ToInt()    ;
        OM.CmnOptn.iLevelFail      = edLevelFail      -> Text.ToInt()    ;


    }
}
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

    //Set Dir. Load
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
//---------------------------------------------------------------------------


