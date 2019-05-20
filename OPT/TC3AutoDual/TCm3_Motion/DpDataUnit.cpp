//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "DpDataUnit.h"
#include "UserFile.h"
#include "UserIni.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------

__fastcall CDpData::CDpData(void)
{
    memset(&Data , 0 , sizeof(TDpData));

}

//---------------------------------------------------------------------------
__fastcall CDpData::~CDpData(void)
{

}

void __fastcall CDpData::ClearEachData()
{

    Data.dWorkTime  = 0.0 ;
    Data.dAdjTime   = 0.0 ;
    Data.dBondTime  = 0.0 ;
    Data.dUVTime    = 0.0 ;
    Data.dWaitTime  = 0.0 ;
}

void __fastcall CDpData::ClearTotalData()
{
    Data.iPassCnt   = 0   ;
    Data.iTotalCnt  = 0   ;
    Data.dYield     = 0.0 ;
    Data.dAvrTime   = 0.0 ;
    Data.dTotalTime = 0.0 ;
}

void __fastcall CDpData::LoadData(AnsiString _sID)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\DpData" + _sID+".INI" ;

    //Load
    UserINI.Load(sPath, "DpData" , "iPassCnt  " , Data.iPassCnt  );
    UserINI.Load(sPath, "DpData" , "iTotalCnt " , Data.iTotalCnt );
    UserINI.Load(sPath, "DpData" , "dYield    " , Data.dYield    );

    UserINI.Load(sPath, "DpData" , "dAvrTime  " , Data.dAvrTime  );
    UserINI.Load(sPath, "DpData" , "dTotalTime" , Data.dTotalTime);

    UserINI.Load(sPath, "DpData" , "dWorkTime " , Data.dWorkTime );
    UserINI.Load(sPath, "DpData" , "dAdjTime  " , Data.dAdjTime  );
    UserINI.Load(sPath, "DpData" , "dBondTime " , Data.dBondTime );
    UserINI.Load(sPath, "DpData" , "dUVTime   " , Data.dUVTime   );
    UserINI.Load(sPath, "DpData" , "dWaitTime " , Data.dWaitTime );
}

void __fastcall CDpData::SaveData(AnsiString _sID)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\DpData" + _sID+".INI" ;

    //Load
    UserINI.Save(sPath, "DpData" , "iPassCnt  " , Data.iPassCnt  );
    UserINI.Save(sPath, "DpData" , "iTotalCnt " , Data.iTotalCnt );
    UserINI.Save(sPath, "DpData" , "dYield    " , Data.dYield    );

    UserINI.Save(sPath, "DpData" , "dAvrTime  " , Data.dAvrTime  );
    UserINI.Save(sPath, "DpData" , "dTotalTime" , Data.dTotalTime);

    UserINI.Save(sPath, "DpData" , "dWorkTime " , Data.dWorkTime );
    UserINI.Save(sPath, "DpData" , "dAdjTime  " , Data.dAdjTime  );
    UserINI.Save(sPath, "DpData" , "dBondTime " , Data.dBondTime );
    UserINI.Save(sPath, "DpData" , "dUVTime   " , Data.dUVTime   );
    UserINI.Save(sPath, "DpData" , "dWaitTime " , Data.dWaitTime );
}

