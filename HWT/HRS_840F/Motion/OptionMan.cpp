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
    memset(&DevInfo  , 0 , sizeof(DevInfo ));  //
    memset(&DevOptn  , 0 , sizeof(DevOptn ));  //sOSTPinName 가 String 이라 계속 하면 메모리 누수 되지만 일단 한번만 하기에 냄겨둠.
    memset(&CmnOptn  , 0 , sizeof(CmnOptn ));  //sRj1Name  가 String 이라 계속 하면 메모리 누수 되지만 일단 한번만 하기에 냄겨둠.
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
    //UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch       " , DevInfo.dColPitch       ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Save Device.
    //UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch       " , DevInfo.dColPitch       );

}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpmtyBin       " , DevOptn.iEpmtyBin      );
}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpmtyBin       " , DevOptn.iEpmtyBin      );


}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;      

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop       " , CmnOptn.bLoadingStop       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bFRunSkip          " , CmnOptn.bFRunSkip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bRunSkip           " , CmnOptn.bRRunSkip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iBin1FullMaxCnt    " , CmnOptn.iBin1FullMaxCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iBin2FullMaxCnt    " , CmnOptn.iBin2FullMaxCnt    );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dLdrPickDely       " , CmnOptn.dLdrPickDely       );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dUldMskDetSnrDely  " , CmnOptn.dUldMskDetSnrDely  );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dFirstAirBlowTime  " , CmnOptn.dFirstAirBlowTime  );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dSecondAirBlowTime " , CmnOptn.dSecondAirBlowTime );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dPickVccmDely      " , CmnOptn.dPickVccDely       );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dPackOpenVccDely   " , CmnOptn.dPackOpenVccDely   );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dPickEjctDely      " , CmnOptn.dPickEjctDely      );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dPackEjctDely      " , CmnOptn.dPackEjctDely      );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dPickBwBfrDely     " , CmnOptn.dPickBwBfrDely     );
}


void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop       " , CmnOptn.bLoadingStop       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bFRunSkip          " , CmnOptn.bFRunSkip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bRRunSkip          " , CmnOptn.bRRunSkip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iBin1FullMaxCnt    " , CmnOptn.iBin1FullMaxCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iBin2FullMaxCnt    " , CmnOptn.iBin2FullMaxCnt    );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dLdrPickDely       " , CmnOptn.dLdrPickDely       );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dUldMskDetSnrDely  " , CmnOptn.dUldMskDetSnrDely  );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dFirstAirBlowTime  " , CmnOptn.dFirstAirBlowTime  );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dSecondAirBlowTime " , CmnOptn.dSecondAirBlowTime );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dPickVccmDely      " , CmnOptn.dPickVccDely       );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dPackOpenVccDely   " , CmnOptn.dPackOpenVccDely   );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dPickEjctDely      " , CmnOptn.dPickEjctDely      );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dPackEjctDely      " , CmnOptn.dPackEjctDely      );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dPickBwBfrDely     " , CmnOptn.dPickBwBfrDely     );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;


}
void COptionMan::SaveMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    UserINI.ClearFile(sPath) ;

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRS_840F";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

    if(EqpOptn.sModelName =="") EqpOptn.sModelName = "HRS_840F";

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


AnsiString COptionMan::GetBinName(int _iStat)
{
    AnsiString sRet = "" ;

    return sRet ;

}
