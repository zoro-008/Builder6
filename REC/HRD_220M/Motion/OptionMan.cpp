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
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dColPitch   " , DevInfo.dColPitch   ); if(!DevInfo.dColPitch   )DevInfo.dColPitch    = 1.0 ;
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    //UserINI.Save(sPath.c_str() , "DevInfo" , "dColPitch      " , DevInfo.dColPitch    );
}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iVaccmdelay      " , DevOptn.iVaccmdelay     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDisprdelay      " , DevOptn.iDisprdelay     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEjectdelay      " , DevOptn.iEjectdelay     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEndDelay        " , DevOptn.iEndDelay       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPlaceDelay      " , DevOptn.iPlaceDelay     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDisprSpd        " , DevOptn.iDisprSpd       );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dJigPCLOfs       " , DevOptn.dJigPCLOfs      );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDisprIncPosCnt  " , DevOptn.iDisprIncPosCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDisprShotCnt    " , DevOptn.iDisprShotCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDisprRptCnt     " , DevOptn.iDisprRptCnt    );

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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iVaccmdelay      " , DevOptn.iVaccmdelay    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDisprdelay      " , DevOptn.iDisprdelay    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEjectdelay      " , DevOptn.iEjectdelay    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEndDelay        " , DevOptn.iEndDelay       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPlaceDelay      " , DevOptn.iPlaceDelay    );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDisprSpd        " , DevOptn.iDisprSpd      );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dJigPCLOfs       " , DevOptn.dJigPCLOfs     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDisprIncPosCnt  " , DevOptn.iDisprIncPosCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDisprShotCnt    " , DevOptn.iDisprShotCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDisprRptCnt     " , DevOptn.iDisprRptCnt    );

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
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDisprSkip       " , CmnOptn.bDisprSkip     );

}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDisprSkip       " , CmnOptn.bDisprSkip     );
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
}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRD_220M";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

    if(EqpOptn.sModelName =="") sModelName = "HRD_220M";

}
void COptionMan::SaveEqpOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

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

