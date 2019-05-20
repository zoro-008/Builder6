//---------------------------------------------------------------------------
#pragma hdrstop
#include "SpcSubUnit.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "SpcUnit.h"
#include "UserFile.h"
#include "UserIni.h"
//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "sLogUnit.h"
#pragma package(smart_init)

CSpcSub SPCS;

__fastcall CSpcSub::CSpcSub(void)
{
    ClearData();
}

__fastcall CSpcSub::~CSpcSub(void)
{
}

//Rotor Height Inspection
void CSpcSub::ClearData()
{
/*
    memset(dRotorH , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL) ;
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            dRotorH[r][c] = 9.999 ;
        }
    }
*/
}

/*
void __fastcall CSpcSub::WriteDataCsv()
{
    //Local Var.
    int       hFile       ;
    String    sTemp        ;
    String    sPath       ;
    TDateTime tSTime , tETime;

    sPath = "D:\\LogData\\" + Now().FormatString("yyyy_mm") + "\\";
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());
    sPath += Now().FormatString("yyyy_mm_dd") + ".csv" ;

    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) return;
    }

    tSTime.Val = LT.LotInfo.dStartTime  ;
    tETime.Val = LT.LotInfo.dEndTime    ;

    sTemp  = "\r\nZeroPoint=," ;
    sTemp += (AnsiString)g_dCrntZeroPoint ;
    sTemp += "\r\n";
    sTemp += "Start  Time,";
    sTemp += tSTime.FormatString("hh:nn:ss") + "," ;
    sTemp += "Finish Time,";
    sTemp += tETime.FormatString("hh:nn:ss") + "," ;
    sTemp += "\r\n"   ;

//    FileSeek (hFile , 0            , SEEK_END     );
//    FileWrite(hFile , sTemp.c_str() , sTemp.Length());
    //Save.

//    sTemp = "Rotor Height \r\n";
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            sTemp += (AnsiString)dRotorH[r][c] + "," ;
        }
        sTemp += "\r\n" ;
    }
    FileSeek (hFile , 0             , SEEK_END     );
    FileWrite(hFile , sTemp.c_str() , sTemp.Length());

    //Close File.
    FileClose(hFile);

    //내가 쓰려고 저장함. 
    sTemp = tETime.FormatString("hh_nn_ss") ;
    WriteHeightData(sTemp);

    ClearData();
}

void __fastcall CSpcSub::WritePlaceDataCsv(AnsiString _sData)
{
    //Local Var.
    int       hFile       ;
    String    sTemp        ;
    String    sPath       ;
    TDateTime tSTime , tETime;

    sPath = "D:\\LogData\\" + Now().FormatString("yyyy_mm") + "\\";
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());
    sPath += Now().FormatString("yyyy_mm_dd_") + "PlaceLog.csv" ;

    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) return;
    }

    tSTime.Val = LT.LotInfo.dStartTime  ;
    tETime.Val = LT.LotInfo.dEndTime    ;

    sTemp  = _sData+"\r\n" ;

    FileSeek (hFile , 0             , SEEK_END     );
    FileWrite(hFile , sTemp.c_str() , sTemp.Length());

    //Close File.
    FileClose(hFile);

}

bool __fastcall CSpcSub::WriteHeightData(String sFileName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ;

    DelPastLotLog();

    sPath  = SPC_FOLDER ;
    sPath += "Map\\" + Now().FormatString("yyyy_mm_dd") + "\\" ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath += sFileName + ".ini" ;

    String sRslt , sTemp;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
//            sTemp = dRotorH[r][c] ;
            sTemp = sTemp.sprintf("%.3f",dRotorH[r][c])  + "_";
            sRslt += sTemp ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iRowCnt             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iColCnt             );

    return true ;
}

bool __fastcall CSpcSub::ReadHeightData (String sPath , TStringGrid * _sgMap)
{
    //Local Var.
    TUserINI   UserINI;
    String     sTemp ,sTemp2 ;
    String     sVal   ;
    String     sRslt  ;

    int iRow , iCol   ;
    //14 55 48

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol   );

    _sgMap->RowCount = iRow ;
    _sgMap->ColCount = iCol ;

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,sRslt.Pos("_")-1) ;
            sRslt.Delete(1,sRslt.Pos("_")) ;
            _sgMap->Cells[c][r] = sVal ;
            sTemp2 = sVal+"," ;
        }
    }

    Trace("Rcv Array",sTemp2.c_str()) ;

    return true ;
}
*/
void __fastcall CSpcSub::DelPastLotLog()
{
    UserFile.ClearDirDate(SPC_FOLDER , Now() - DATA_DELETE_DAY);
}
