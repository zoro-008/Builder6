//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmReviewSelect.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "DataMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormReviewSelect *FormReviewSelect;
//---------------------------------------------------------------------------
__fastcall TFormReviewSelect::TFormReviewSelect(TComponent* Owner)
        : TForm(Owner)
{
    DM.ARAY[riVB4 ].SetParent  (pnVb4    ); DM.ARAY[riVB4].SetConfig(false , "pnVb4   "); DM.ARAY[riVB4].ClearDispItem();

    DM.ARAY[riVB4 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS1 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riVB4 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS1 ].SetDispColor(csUnkwn ,clMoneyGreen      );
}
//---------------------------------------------------------------------------
void __fastcall TFormReviewSelect::btNoClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

bool __fastcall TFormReviewSelect::WriteArayData(EN_ARAY_ID riId )
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ,sRslt ;
    String sTemp2 = "";
    String sTemp  = "";

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + OM.m_sCrntDev + "\\ReviewSel.INI" ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sRslt += IntToHex(((int)DM.ARAY[riId].GetStat(r,c)) , 2)  ;
            sTemp2 = (int)DM.ARAY[riId].GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riId].GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iRowCnt                   );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iColCnt                   );

    return true ;
}

bool __fastcall TFormReviewSelect::ReadArayData (EN_ARAY_ID riId)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;    
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    AnsiString sTemp2= "" ;
    int iRow = 0 ;
    int iCol = 0 ;
    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + OM.m_sCrntDev + "\\ReviewSel.INI" ;

    if(!FileExists(sPath)) return false ; //SAVE ??

    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , iRow                   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , iCol                   );

    if(OM.DevInfo.iRowCnt != iRow || OM.DevInfo.iColCnt != iCol ) {
        DM.ARAY[riVS4].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
        DM.ARAY[riVS4].SetStat(csNone);
        WriteArayData(riVB4);
        return false ;
    }

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            DM.ARAY[riId].SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    return true ;
}

void __fastcall TFormReviewSelect::btYesClick(TObject *Sender)
{
    WriteArayData(riVB4);
}
//---------------------------------------------------------------------------

void __fastcall TFormReviewSelect::FormShow(TObject *Sender)
{
    ReadArayData(riVB4);        
}
//---------------------------------------------------------------------------

