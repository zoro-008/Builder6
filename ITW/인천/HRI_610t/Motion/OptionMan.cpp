//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UserIni.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
COptionMan OM;

//---------------------------------------------------------------------------
COptionMan::COptionMan(void)
{

}

//---------------------------------------------------------------------------
COptionMan::~COptionMan(void)
{

}

//---------------------------------------------------------------------------
void COptionMan::Init(void)
{
    //Init. Buffer.
    memset(&DevInfo  , 0 , sizeof(DevInfo ));
    memset(&DevOptn  , 0 , sizeof(DevOptn ));
    memset(&CmnOptn  , 0 , sizeof(CmnOptn ));
    memset(&MstOptn  , 0 , sizeof(MstOptn ));
    memset(&EqpOptn  , 0 , sizeof(EqpOptn ));

    LoadLastInfo(          );
    LoadMstOptn (          );
    LoadEqpOptn (          );
    LoadCmnOptn (          );
    LoadJobFile (m_sCrntDev);
}
void COptionMan::Close(void)
{
    SaveLastInfo();
}

void COptionMan::SetCrntDev (AnsiString _sName)
{
    m_sCrntDev = _sName ;
}

void COptionMan::LoadJobFile(AnsiString _sDevName)
{
    LoadDevInfo (_sDevName);
    LoadDevOptn (_sDevName);
    //Set Current Device Name.
    SetCrntDev  (_sDevName);
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    //Set Current Device Name.
    SetCrntDev  (_sDevName);
}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    UserINI.Load(sPath.c_str() , "DevInfo" , "dColPitch   " , DevInfo.dColPitch   ); if(!DevInfo.dColPitch   )DevInfo.dColPitch    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iColGrCnt   " , DevInfo.iColGrCnt   ); //if(!DevInfo.iColGrCnt   )DevInfo.iColGrCnt    = 1   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dColGrGap   " , DevInfo.dColGrGap   ); //if(!DevInfo.dColGrGap   )DevInfo.dColGrGap    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iColCnt     " , DevInfo.iColCnt     ); if(!DevInfo.iColCnt     )DevInfo.iColCnt      = 1   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dRowPitch   " , DevInfo.dRowPitch   ); if(!DevInfo.dRowPitch   )DevInfo.dRowPitch    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iRowGrCnt   " , DevInfo.iRowGrCnt   ); //if(!DevInfo.iRowGrCnt   )DevInfo.iRowGrCnt    = 1   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dRowGrGap   " , DevInfo.dRowGrGap   ); //if(!DevInfo.dRowGrGap   )DevInfo.dRowGrGap    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iRowCnt     " , DevInfo.iRowCnt     ); if(!DevInfo.iRowCnt     )DevInfo.iRowCnt      = 5   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iCsSlCnt    " , DevInfo.iCsSlCnt    ); if(!DevInfo.iCsSlCnt    )DevInfo.iCsSlCnt     = 20  ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dCsSlPitch  " , DevInfo.dCsSlPitch  ); if(!DevInfo.dCsSlPitch  )DevInfo.dCsSlPitch   = 5.0 ;

}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    UserINI.Save(sPath.c_str() , "DevInfo" , "dColPitch      " , DevInfo.dColPitch    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iColGrCnt      " , DevInfo.iColGrCnt    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dColGrGap      " , DevInfo.dColGrGap    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iColGrCnt      " , DevInfo.iColGrCnt    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dRowPitch      " , DevInfo.dRowPitch    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dRowGrGap      " , DevInfo.dRowGrGap    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iRowGrCnt      " , DevInfo.iRowGrCnt    );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iColCnt        " , DevInfo.iColCnt      );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iRowCnt        " , DevInfo.iRowCnt      );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iCsSlCnt       " , DevInfo.iCsSlCnt     );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dCsSlPitch     " , DevInfo.dCsSlPitch   );
}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt      " , DevOptn.iLotEndMgzCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iMarkInspFailCnt   " , DevOptn.iMarkInspFailCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRailStprDnDelay   " , DevOptn.iRailStprDnDelay );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSBGripDelay      " , DevOptn.iPSBGripDelay    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sLaserLotFormat    " , DevOptn.sLaserLotFormat  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEmptyStrpCnt      " , DevOptn.iEmptyStrpCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bEmptyStrpPlace    " , DevOptn.bEmptyStrpPlace  );
}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt      " , DevOptn.iLotEndMgzCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iMarkInspFailCnt   " , DevOptn.iMarkInspFailCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRailStprDnDelay   " , DevOptn.iRailStprDnDelay );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSBGripDelay      " , DevOptn.iPSBGripDelay    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sLaserLotFormat    " , DevOptn.sLaserLotFormat  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEmptyStrpCnt      " , DevOptn.iEmptyStrpCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bEmptyStrpPlace    " , DevOptn.bEmptyStrpPlace  );
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    int iTemp ;
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bFrstStrpOut  " , CmnOptn.bFrstStrpOut  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop  " , CmnOptn.bLoadingStop  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor     " , CmnOptn.bIgnrDoor     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipOriVisn  " , CmnOptn.bSkipOriVisn  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipLaser    " , CmnOptn.bSkipLaser    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipAlnVisn  " , CmnOptn.bSkipAlnVisn  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iMarkInsp     " , iTemp                 );CmnOptn.iMarkInsp = (EN_MARK_INSP_OPTN)iTemp ;

}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bFrstStrpOut  " ,      CmnOptn.bFrstStrpOut  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop  " ,      CmnOptn.bLoadingStop  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor     " ,      CmnOptn.bIgnrDoor     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipOriVisn  " ,      CmnOptn.bSkipOriVisn  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipLaser    " ,      CmnOptn.bSkipLaser    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipAlnVisn  " ,      CmnOptn.bSkipAlnVisn  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iMarkInsp     " , (int)CmnOptn.iMarkInsp     );

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode         " , MstOptn.bDebugMode          );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDryRun            " , MstOptn.bDryRun             );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dTrigerOffset      " , MstOptn.dTrigerOffset       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnRsltFile      " , MstOptn.sVisnRsltFile       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnDeviceFile    " , MstOptn.sVisnDeviceFile     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sLaserDataFile     " , MstOptn.sLaserDataFile      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bAllStrpSendLsrJob " , MstOptn.bAllStrpSendLsrJob  );

}
void COptionMan::SaveMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode         " , MstOptn.bDebugMode          );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDryRun            " , MstOptn.bDryRun             );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dTrigerOffset      " , MstOptn.dTrigerOffset       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnRsltFile      " , MstOptn.sVisnRsltFile       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnDeviceFile    " , MstOptn.sVisnDeviceFile     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sLaserDataFile     " , MstOptn.sLaserDataFile      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bAllStrpSendLsrJob " , MstOptn.bAllStrpSendLsrJob  );

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRI_610t";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

    if(EqpOptn.sModelName =="") sModelName = "HRI_610t";

}
void COptionMan::SaveEqpOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save.
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );

    if (m_sCrntDev == "") m_sCrntDev  = "NONE";
}

void COptionMan::SaveLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );
}

