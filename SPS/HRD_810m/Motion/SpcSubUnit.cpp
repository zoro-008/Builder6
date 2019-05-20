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
#include "DataMan.h"
#pragma package(smart_init)

CSpcSub SPCS;

__fastcall CSpcSub::CSpcSub(void)
{
    ClearData();
}

__fastcall CSpcSub::~CSpcSub(void)
{
}

void __fastcall CSpcSub::WriteAtVsDataCsv(CArray * riAray , CVisionMan::TRetResult * _tRslt)
{
    //Local Var.
    int       hFile       ;
    String    sTemp        ;
    String    sPath       ;
    TDateTime tSTime , tETime;

    sPath = "D:\\VisionRslt\\" + Now().FormatString("yyyy_mm_dd") + "\\";
    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());


    if(riAray -> GetLotNo() == "") {
        sPath += Now().FormatString("yy.mm.dd(hh)") + ".csv" ;
    }
    else sPath += riAray -> GetLotNo() + ".csv" ;

    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) return;
        sTemp  = "TIME,";
        sTemp += "MGZ  NO,";
        sTemp += "SLOT NO,";
        sTemp += "VISION X,";
        sTemp += "VISION Y,";
        sTemp += "VISION T,";
        sTemp += "Result,";
        sTemp += "\r\n";

        FileSeek (hFile , 0             , SEEK_END     );
        FileWrite(hFile , sTemp.c_str() , sTemp.Length());

    }

    int iSlotNo , iMgzNo ;
    iSlotNo = riAray -> GetID().ToIntDef(999999)%1000 ;
    iMgzNo  = riAray -> GetID().ToIntDef(999999)/1000 ;

    sTemp  = Now().FormatString("yy.mm.dd(hh:nn:ss)") + ",";
    sTemp += (String)iMgzNo  + ",";
    sTemp += (String)iSlotNo + ",";
    sTemp += (String)_tRslt->fRsltX  + ",";
    sTemp += (String)_tRslt->fRsltY  + ",";
    sTemp += (String)_tRslt->fRsltT  + ",";

    if(_tRslt->iRet == 0 ) { //Good
        sTemp += "GOOD,";
    }
    else {
        sTemp += "FAIL,";
    }
    sTemp += "\r\n";

    FileSeek (hFile , 0             , SEEK_END     );
    FileWrite(hFile , sTemp.c_str() , sTemp.Length());

    //Close File.
    FileClose(hFile);

}


//Rotor Height Inspection
void CSpcSub::ClearData()
{


}

void __fastcall CSpcSub::DelPastLotLog()
{
    UserFile.ClearDirDate(SPC_FOLDER , Now() - 400);
}
