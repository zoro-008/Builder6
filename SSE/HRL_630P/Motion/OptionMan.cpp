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
    memset(&MstOptn  , 0 , sizeof(MstOptn )); //엔에 String 있는 경우에는 memeset쓰면 안되지만 일단 초기화에만 있으면 문제 없으므로 패스...
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
    m_sCrntDev = _sDevName;
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
}
                                                                                                       
void COptionMan::LoadDevOptn(AnsiString _sDevName)                                                     
{                                                                                                      
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;                                                                                
                                                                                                       
    //Set Dir.                                                                                         
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;                                    
                                                                                                       
    //Load Device.

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseLsrPst1  " , DevOptn.bUseLsrPst1  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseLsrPst2  " , DevOptn.bUseLsrPst2  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseLsrPst3  " , DevOptn.bUseLsrPst3  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseLsrPst4  " , DevOptn.bUseLsrPst4  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseLsrPst5  " , DevOptn.bUseLsrPst5  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo1   " , DevOptn.iFrtLsrNo1   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo2   " , DevOptn.iFrtLsrNo2   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo3   " , DevOptn.iFrtLsrNo3   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo4   " , DevOptn.iFrtLsrNo4   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo5   " , DevOptn.iFrtLsrNo5   );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLerLsrNo1   " , DevOptn.iLerLsrNo1   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLerLsrNo2   " , DevOptn.iLerLsrNo2   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLerLsrNo3   " , DevOptn.iLerLsrNo3   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLerLsrNo4   " , DevOptn.iLerLsrNo4   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLerLsrNo5   " , DevOptn.iLerLsrNo5   );


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

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseLsrPst1  " , DevOptn.bUseLsrPst1  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseLsrPst2  " , DevOptn.bUseLsrPst2  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseLsrPst3  " , DevOptn.bUseLsrPst3  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseLsrPst4  " , DevOptn.bUseLsrPst4  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseLsrPst5  " , DevOptn.bUseLsrPst5  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo1   " , DevOptn.iFrtLsrNo1   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo2   " , DevOptn.iFrtLsrNo2   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo3   " , DevOptn.iFrtLsrNo3   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo4   " , DevOptn.iFrtLsrNo4   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFrtLsrNo5   " , DevOptn.iFrtLsrNo5   );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLerLsrNo1   " , DevOptn.iLerLsrNo1   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLerLsrNo2   " , DevOptn.iLerLsrNo2   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLerLsrNo3   " , DevOptn.iLerLsrNo3   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLerLsrNo4   " , DevOptn.iLerLsrNo4   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLerLsrNo5   " , DevOptn.iLerLsrNo5   );
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoorOpen  " , CmnOptn.bIgnrDoorOpen  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipLaserWrk  " , CmnOptn.bSkipLaserWrk  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVsnFailCnt    " , CmnOptn.iVsnFailCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iUseLsrPos     " , CmnOptn.iUseLsrPos     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iAirBlowTm     " , CmnOptn.iAirBlowTm     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iAlignTm       " , CmnOptn.iAlignTm       );

}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoorOpen  " , CmnOptn.bIgnrDoorOpen  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipLaserWrk  " , CmnOptn.bSkipLaserWrk  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVsnFailCnt    " , CmnOptn.iVsnFailCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iUseLsrPos     " , CmnOptn.iUseLsrPos     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iAirBlowTm     " , CmnOptn.iAirBlowTm     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iAlignTm       " , CmnOptn.iAlignTm       );

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode   " , MstOptn.bDebugMode );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDryRun      " , MstOptn.bDryRun    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOffset      " , MstOptn.dOffset    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iPkgCnt      " , MstOptn.iPkgCnt    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sCrntLotID   " , MstOptn.sCrntLotID );
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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sCrntLotID   " , MstOptn.sCrntLotID );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode   " , MstOptn.bDebugMode );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDryRun      " , MstOptn.bDryRun    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOffset      " , MstOptn.dOffset    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iPkgCnt      " , MstOptn.iPkgCnt    );
}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRT-430S";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

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

