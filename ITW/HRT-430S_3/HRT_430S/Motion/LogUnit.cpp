//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogUnit.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "DataMan.h"
#pragma package(smart_init)

CLogUnit LOG;

__fastcall CLogUnit::CLogUnit(void)
{
//    Init();
}

__fastcall CLogUnit::~CLogUnit(void)
{
}
//---------------------------------------------------------------------------
void __fastcall CLogUnit::Init(void)
{
    //Make Dir.
//    if (!DirectoryExists(ExtractFilePath(Application -> ExeName) + "LOG"        )) CreateDir(ExtractFilePath(Application -> ExeName) + "LOG"        );
//    if (!DirectoryExists(ExtractFilePath(Application -> ExeName) + "LOG\\EVENT" )) CreateDir(ExtractFilePath(Application -> ExeName) + "LOG\\EVENT" );
//    if (!DirectoryExists(ExtractFilePath(Application -> ExeName) + "LOG\\TIME"  )) CreateDir(ExtractFilePath(Application -> ExeName) + "LOG\\TIME"  );
//    m_sPreString = "" ;
}
//---------------------------------------------------------------------------
void __fastcall CLogUnit::KillPastLog(void)
{
//    UserFile.ClearDirDate(ExtractFilePath(Application -> ExeName) + "LOG\\EVENT" , Now() - FM.ComOptn.iSavePeriod);
//    UserFile.ClearDirDate(ExtractFilePath(Application -> ExeName) + "LOG\\TIME"  , Now() - FM.ComOptn.iSavePeriod);
}
//---------------------------------------------------------------------------
void __fastcall CLogUnit::Trace(AnsiString Msg)
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    TDateTime  CurrDate;

    if(m_sPreString == Msg) return ;

    KillPastLog();

    //Set Path.
    CurrDate = Now().CurrentDate();
    Temp     = CurrDate.FormatString("yyyymmdd");
    Path     = ExtractFilePath(Application -> ExeName) + "LOG\\EVENT\\[" + Temp + "]EVENT.LOG";

    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        }

    //Save.
    Temp = "[" + Now().CurrentDateTime().FormatString("hh:mm:ss:zz") + "]" + Msg + "\r\n";
    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);

    m_sPreString = Msg ;

}

//---------------------------------------------------------------------------
bool __fastcall CLogUnit::HanraVs(void)
{
    //Local Var.
    EN_CHIP_STAT iStat;
    int        hFile       ;
    int        iRst,iRStat ;
    int        r,c         ;
    AnsiString sPath       ;
    AnsiString sTemp       ;
    TDateTime  CurrDate    ;

    KillPastLog();

    if(OM.MstOptn.bDebugMode) return true ;
    iRst = DM.ARAY[riPSB].GetCntRst();
    if(!iRst) return true ;

    //Set Path.
    CurrDate.Val = LT.LotInfo.dStartTime;
    sTemp = CurrDate.FormatString("hhnnss");

    sPath = ExtractFilePath(Application -> ExeName) + "LOG\\Vs" ;

    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath = ExtractFilePath(Application -> ExeName) + "LOG\\Vs\\" + LT.LotInfo.sLotNo + "_" + sTemp + ".dat";

    //File Open.
    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) return false;
        }

    //Save.
    sTemp.printf("%03d",iRst);
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sTemp.c_str() , sTemp.Length());

    for (int iR = 0; iR < OM.DevInfo.iRowCnt; iR ++ ) {
        for (int iC = 0; iC < OM.DevInfo.iColCnt; iC ++ ) {
            iStat = DM.ARAY[riPSB].GetStat(iR , iC);
            if ( iStat != csNone && iStat != csMask && iStat != csUnkwn && iStat != csWork && iStat != csWait && iStat != csEror && iStat != csGood && iStat != csEtc ) {
                iRStat = iStat ;
                c = OM.DevInfo.iColCnt - iC ;
                r = OM.DevInfo.iRowCnt - iR ;
                sTemp.printf("%02d%02d%1x", c, r, iRStat);
                FileSeek (hFile , 0             , SEEK_END      );
                FileWrite(hFile , sTemp.c_str() , sTemp.Length());
            }
        }
    }
    sTemp.printf("\r\n");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sTemp.c_str() , sTemp.Length());

    //Close File.
    FileClose(hFile);
    return true;
}

bool __fastcall CLogUnit::DmLog(EN_ARAY_ID riId)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sTemp,sTime ;
    int        sState      ;
    TDateTime  CurrDate;

    //Set Path.
    CurrDate.Val = LT.LotInfo.dStartTime;
    sTime = CurrDate.FormatString("yyyy'-'mm'-'dd");

    sPath = ExtractFilePath(Application -> ExeName) + "LOG\\Vision" ;

    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath = ExtractFilePath(Application -> ExeName) + "LOG\\Vision\\" + sTime + "_" + LT.LotInfo.sLotNo + ".dat";

    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) return false;
    }

    AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'Ω√ 'n'∫–' s'√  \r\n'");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sDate.c_str() , sDate.Length());

    //Save.
    for (int iRCount = 0; iRCount < OM.DevInfo.iRowCnt; iRCount ++ ) {
        sTemp.printf("R%2d =", iRCount);
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sTemp.c_str() , sTemp.Length());
        for (int iCCount = 0; iCCount < OM.DevInfo.iColCnt; iCCount ++ ) {
            sState = DM.ARAY[riId].GetStat(iRCount , iCCount);
            sTemp.printf("%2d;",sState);
            FileSeek (hFile , 0             , SEEK_END      );
            FileWrite(hFile , sTemp.c_str() , sTemp.Length());
        }
        sTemp = "\r\n";
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sTemp.c_str() , sTemp.Length());
    }

    //Close File.
    FileClose(hFile);
    return true;
}
