//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSPC.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "UserFile.h"
#include "DataMan.h"
#include "UserINI.h"
#include "LanguageUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSPC *FrmSPC;
//---------------------------------------------------------------------------
__fastcall TFrmSPC::TFrmSPC(TComponent* Owner)
    : TForm(Owner)
{
    sgErrHistory->Cells[0][0] = "No" ;
    sgErrHistory->Cells[1][0] = "ErrNo" ;
    sgErrHistory->Cells[2][0] = "Comment" ;
    sgErrHistory->Cells[3][0] = "Occur Date(Time)" ;
    sgErrHistory->Cells[4][0] = "Release Date(Time)" ;
    sgErrCnt    ->Cells[0][0] = "No" ;
    sgErrCnt    ->Cells[1][0] = "ErrNo" ;
    sgErrCnt    ->Cells[2][0] = "Comment" ;
    sgErrCnt    ->Cells[3][0] = "Count" ;

    sgLotInfo   ->Cells[0][0]  = "WriteTime";
    sgLotInfo   ->Cells[1][0]  = "LotNo"    ;
    sgLotInfo   ->Cells[2][0]  = "JobFile"  ;
    sgLotInfo   ->Cells[3][0]  = "Operator" ;
    sgLotInfo   ->Cells[4][0]  = "StartTime";
    sgLotInfo   ->Cells[5][0]  = "EndTime"  ;
    sgLotInfo   ->Cells[6][0]  = "RunTime"  ;
    sgLotInfo   ->Cells[7][0]  = "IdleTime" ;
    sgLotInfo   ->Cells[8][0]  = "JamTime"  ;
    sgLotInfo   ->Cells[9][0]  = "TotalTime";
    sgLotInfo   ->Cells[10][0] = "StrpUPEH" ;
    sgLotInfo   ->Cells[11][0] = "ChipUPEH" ;
    sgLotInfo   ->Cells[12][0] = "StrpUPH"  ;
    sgLotInfo   ->Cells[13][0] = "ChipUPH"  ;
    sgLotInfo   ->Cells[14][0] = "WorkStrp" ;
    sgLotInfo   ->Cells[15][0] = "WorkChip" ;
    sgLotInfo   ->Cells[16][0] = "FailChip" ;
    sgLotInfo   ->Cells[17][0] = "Empty"    ;
    sgLotInfo   ->Cells[18][0] = "Chip"     ;
    sgLotInfo   ->Cells[19][0] = "Dust"     ;
    sgLotInfo   ->Cells[20][0] = "L_Flow"   ;
    sgLotInfo   ->Cells[21][0] = "Broken"   ;
    sgLotInfo   ->Cells[22][0] = "Crack"    ;
    sgLotInfo   ->Cells[23][0] = "Burr"     ;
    sgLotInfo   ->Cells[24][0] = "Zener"    ;
    sgLotInfo   ->Cells[25][0] = "Wire"     ;
    sgLotInfo   ->Cells[26][0] = "Orient"   ;
    sgLotInfo   ->Cells[27][0] = "PMeasure" ;
    sgLotInfo   ->Cells[28][0] = "PDist"    ;
    sgLotInfo   ->Cells[29][0] = "Matching" ;
    sgLotInfo   ->Cells[30][0] = "Flow"     ;
    sgLotInfo   ->Cells[31][0] = "Heater"   ;

    sgLotDayInfo->Cells[0][0]  = "WriteTime,"   ;
    sgLotDayInfo->Cells[1][0]  = "RunTime,"     ;
    sgLotDayInfo->Cells[2][0]  = "IdleTime,"    ;
    sgLotDayInfo->Cells[3][0]  = "JamTime,"     ;
    sgLotDayInfo->Cells[4][0]  = "TotalTime,"   ;
    sgLotDayInfo->Cells[5][0]  = "StrpUPEH,"    ;
    sgLotDayInfo->Cells[6][0]  = "ChipUPEH,"    ;
    sgLotDayInfo->Cells[7][0]  = "StrpUPH,"     ;
    sgLotDayInfo->Cells[8][0]  = "ChipUPH,"     ;
    sgLotDayInfo->Cells[9][0]  = "WorkStrp,"    ;
    sgLotDayInfo->Cells[10][0] = "WorkChip,"    ;
    sgLotDayInfo->Cells[11][0] = "FailChip,"    ;
    sgLotDayInfo->Cells[12][0] = "LotCnt,"      ;
    sgLotDayInfo->Cells[13][0] = "CntRslt1,"    ;
    sgLotDayInfo->Cells[14][0] = "CntRslt2,"    ;
    sgLotDayInfo->Cells[15][0] = "CntRslt3,"    ;
    sgLotDayInfo->Cells[16][0] = "CntRslt4,"    ;
    sgLotDayInfo->Cells[17][0] = "CntRslt5,"    ;
    sgLotDayInfo->Cells[18][0] = "CntRslt6,"    ;
    sgLotDayInfo->Cells[19][0] = "CntRslt7,"    ;
    sgLotDayInfo->Cells[20][0] = "CntRslt8,"    ;
    sgLotDayInfo->Cells[21][0] = "CntRslt9,"    ;
    sgLotDayInfo->Cells[22][0] = "CntRslt10,"   ;
    sgLotDayInfo->Cells[23][0] = "CntRslt11,"   ;
    sgLotDayInfo->Cells[24][0] = "CntRslt12,"   ;
    sgLotDayInfo->Cells[25][0] = "CntRslt13,"   ;
    sgLotDayInfo->Cells[26][0] = "CntRslt14,"   ;
    sgLotDayInfo->Cells[27][0] = "CntHeatFail," ;

    DM.ARAY[riSPC].SetParent(pnLotMap  ); DM.ARAY[riSPC].SetConfig(false , "LotMap  "); DM.ARAY[riSPC].ClearDispItem();
    DM.ARAY[riSPC].SetDispItem(csUnkwn   ,"Unknown "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen      );
    DM.ARAY[riSPC].SetDispItem(csFail    ,"Heat Fail "    ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riSPC].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPSB ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riSPC].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riPSB ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riSPC].SetDispItem(csGood    ,"Good "         ) ; //DM.ARAY[riPSB ].SetDispColor(csWork  ,clHighlight       );

    //Load Language
//    LAG.LanguageSel(FrmSPC,FM_GetLan());
//    LanguageSel(FM_GetLan());
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::UpdateComOptn(bool _bToTable)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::FormShow(TObject *Sender)
{
    TDateTime CurrDateTime = Now();

    edMtbaDT    ->Format = "yyyy-MM";

    edF_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotInfo    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edFDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edUDataTime    ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_ErrCnt     ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edMtbaDT       ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edF_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");
    edU_LotDayInfo ->Date = CurrDateTime.FormatString("yyyy-mm-dd");

    LT.UpdateDate(sgDate);
    UserFile.GridSearchFile(LOG_FOLDER , sgLotDayName , false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::Label2Click(TObject *Sender)
{
    //LT.WriteLotLog() ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgDateClick(TObject *Sender)
{
    LT.UpdateLotName(sgDate , sgLot);
    sgLot -> Col = 1 ;
    sgLotClick(Sender);
    //LT.DispLotDate(sgDate , sgLot);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSPC::sgLotClick(TObject *Sender)
{
    LT.DispLotDate(sgDate , sgLot , sgLotInfo);

    AnsiString sPath;
    sPath = LOG_FOLDER + sgDate ->Cells[1][sgDate   -> Row] + "\\" + sgLot  ->Cells[1][sgLot -> Row];
    sPath.Delete(sPath.Pos(".csv"),4);
    UserFile.GridSearchFile(sPath , sgSlotNo , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    sgSlotNo -> Col = 1 ;
    sgSlotNoClick(Sender);
//    sgLotInfoClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrDataViewClick(TObject *Sender)
{
    SPC.DispErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrCntViewClick(TObject *Sender)
{
    SPC.DispErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btMtbaViewClick(TObject *Sender)
{
//    ChatMtba -> CleanupInstance();
//    S1_Mtba->Clear();
    SPC.DispMtba(edMtbaDT->Date.FormatString("yyyymm") , S1_Mtba);
//    dbMtba->LeftAxis
//    Series1->AddXY()
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btMtbaSaveClick(TObject *Sender)
{
    SPC.SaveMtba(edMtbaDT->Date.FormatString("yyyymm"),S1_Mtba);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrCntSaveClick(TObject *Sender)
{
    SPC.SaveErrCnt(edF_ErrCnt->Date.FormatString("yyyymmdd"),edU_ErrCnt->Date.FormatString("yyyymmdd"),sgErrCnt);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btErrDataSaveClick(TObject *Sender)
{
    SPC.SaveErrData(edFDataTime->Date.FormatString("yyyymmdd"),edUDataTime->Date.FormatString("yyyymmdd"),sgErrHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotInfoViewClick(TObject *Sender)
{
    LT.DispLotDate(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotInfoSaveClick(TObject *Sender)
{
    LT.SaveLotData(edF_LotInfo->Date.FormatString("yyyymmdd"),edU_LotInfo->Date.FormatString("yyyymmdd"),sgLotInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgLotDayNameClick(TObject *Sender)
{
    LT.DispLotDayDate(sgLotDayName , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotDayViewClick(TObject *Sender)
{
    LT.DispLotDayDate(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::btLotDaySaveClick(TObject *Sender)
{
    LT.SaveLotDayData(edF_LotDayInfo->Date.FormatString("yyyymmdd") , edU_LotDayInfo->Date.FormatString("yyyymmdd") , sgLotDayInfo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSPC::sgSlotNoClick(TObject *Sender)
{
    AnsiString sPath ;
    sPath = LOG_FOLDER + sgDate->Cells[1][sgDate -> Row] + "\\" + sgLot->Cells[1][sgLot -> Row] + "\\" + sgSlotNo->Cells[1][sgSlotNo -> Row] ;
    sPath.Delete(sPath.Pos(".csv"),4);
    LT.ReadArayData(sPath , riSPC);
}
//---------------------------------------------------------------------------
bool __fastcall TFrmSPC::LanguageSel(EN_LAN_SEL lvType)
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
            else if (Components[i]->ClassNameIs("TTabSheet")) {
                sComName = Components[i]->ClassName();
                sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
            }
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
        else if (Components[i]->ClassNameIs("TTabSheet")) {
            sComName = Components[i]->ClassName();
            sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TTabSheet*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TTabSheet*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
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


