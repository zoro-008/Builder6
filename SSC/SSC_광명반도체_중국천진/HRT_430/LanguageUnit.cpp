//---------------------------------------------------------------------------
#pragma hdrstop
#include "LanguageUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
//#include "FormOperation.h"
//#include "OptionMan.h"
//#include "FormDeviceSet.h"
//#include "FormOption.h"
//#include "FormSPC.h"
//#include "FormDevice.h"
//#include "SMInterfaceUnit.h"

#pragma package(smart_init)

CLangUnit LAG;

CLangUnit::CLangUnit(void)
{
}
CLangUnit::~CLangUnit(void)
{
}
//---------------------------------------------------------------------------
bool __fastcall CLangUnit::LanguageLoad(TForm *_Frm , EN_LAN_SEL lvType)
{
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sName    , sNameTemp    ;
    AnsiString sCaption , sCaptionTemp ;
    AnsiString sComName ;
    bool       bOri     ;
/*
    for (int i = 0; i <= MAX_LAN_SEL   ; i++)
    {
             if(i == (int)lsEnglish ) {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_E.ini" ; bOri = false ;}
        else if(i == (int)lsKorean  ) {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_K.ini" ; bOri = false ;}
        else if(i == (int)lsChinese ) {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_C.ini" ; bOri = false ;}
        else                          {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + ".ini"   ;
                                       UserINI.ClearFile(sPath) ;
                                       bOri  = true ;                                                  }

        for (int i = 0; i < _Frm->ComponentCount; i++)
        {
            if (_Frm->Components[i]->ClassNameIs("TLabel"))
            {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name          ;
                sCaption = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption       ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TCheckBox")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
                sCaption = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TButton")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
                sCaption = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TBitBtn")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
                sCaption = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TSpeedButton")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    ;
                sCaption = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TTabSheet")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
                sCaption = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TGroupBox")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
                sCaption = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            }
            else if (_Frm->Components[i]->ClassNameIs("TStaticText")) {
                sComName = _Frm->Components[i]->ClassName();
                sName    = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
                sCaption = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
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
*/
    //Set Dir. Load
         if(lvType == lsEnglish ) sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_E.ini" ;
    else if(lvType == lsKorean  ) sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_K.ini" ;
    else if(lvType == lsChinese ) sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_C.ini" ;
    else                          return true ;

    for (int i = 0; i < _Frm->ComponentCount; i++)
    {
        if (_Frm->Components[i]->ClassNameIs("TLabel"))
        {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name          ;
            sCaption = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption       ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TCheckBox")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;

        }
        else if (_Frm->Components[i]->ClassNameIs("TButton")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
            sCaption = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;

        }
        else if (_Frm->Components[i]->ClassNameIs("TBitBtn")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
            sCaption = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;

        }
        else if (_Frm->Components[i]->ClassNameIs("TSpeedButton")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    ;
            sCaption = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TTabSheet")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TGroupBox")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TStaticText")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name     ;
            sCaption = ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption  ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName     ;
            ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption  ;
        }
    }

    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall CLangUnit::LanguageSave(TForm *_Frm , EN_LAN_SEL lvType)
{
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sName    , sNameTemp    ;
    AnsiString sCaption , sCaptionTemp ;
    AnsiString sComName ;
    bool       bOri     ;

//    for (int i = 0; i <= 1 ; i++)
//    {
             if(lvType == lsEnglish ) {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_E.ini" ;}// bOri = false ;}
        else if(lvType == lsKorean  ) {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_K.ini" ;}// bOri = false ;}
        else if(lvType == lsChinese ) {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_C.ini" ;}// bOri = false ;}
//        else                          {sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + ".ini"   ;
//                                       UserINI.ClearFile(sPath) ;
//                                       bOri  = true ;                                                  }

    for (int i = 0; i < _Frm->ComponentCount; i++)
    {
        if (_Frm->Components[i]->ClassNameIs("TLabel"))
        {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name          ;
            sCaption = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TCheckBox")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TButton")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
            sCaption = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TBitBtn")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
            sCaption = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TSpeedButton")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    ;
            sCaption = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TTabSheet")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TGroupBox")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TStaticText")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
        }
        else continue ;

        if(sComName == "" || sName == "") continue ;

        //Save Device.
//            if(bOri) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
//            else if(sCaption != "") {
        if(sCaption != "") {
            UserINI.Load(sPath.c_str()  , sComName  , sName , sCaptionTemp);
            if( sCaptionTemp == "" ) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
        }
    }

/*
    //Set Dir. Load
         if(lvType == lsEnglish ) sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_E.ini" ;
    else if(lvType == lsKorean  ) sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_K.ini" ;
    else if(lvType == lsChinese ) sPath = EXE_FOLDER + "Language\\" + _Frm->Caption + "\\" + _Frm->Caption + "_C.ini" ;
    else                          return true ;

    for (int i = 0; i < _Frm->ComponentCount; i++)
    {
        if (_Frm->Components[i]->ClassNameIs("TLabel"))
        {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name          ;
            sCaption = ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption       ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TLabel*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TCheckBox")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TCheckBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;

        }
        else if (_Frm->Components[i]->ClassNameIs("TButton")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
            sCaption = ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;

        }
        else if (_Frm->Components[i]->ClassNameIs("TBitBtn")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name         ;
            sCaption = ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TBitBtn*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;

        }
        else if (_Frm->Components[i]->ClassNameIs("TSpeedButton")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    ;
            sCaption = ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TSpeedButton*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TTabSheet")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TTabSheet*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TGroupBox")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName          ;
            ((TGroupBox*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption       ;
        }
        else if (_Frm->Components[i]->ClassNameIs("TStaticText")) {
            sComName = _Frm->Components[i]->ClassName();
            sName    = ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name     ;
            sCaption = ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption  ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Name    = sName     ;
            ((TStaticText*)_Frm->FindComponent(_Frm->Components[i]->Name))->Caption = sCaption  ;
        }
    }
*/
    return true ;
}
//---------------------------------------------------------------------------




