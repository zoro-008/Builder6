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

//---------------------------------------------------------------------------
COptionMan::~COptionMan(void)
{
    SaveLastInfo();
}

//---------------------------------------------------------------------------
void COptionMan::Init(void)
{
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
    //UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch     " , DevInfo.dColPitch    ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    UserINI.ClearFile(sPath) ;

    //Save Device.
    //UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch     " , DevInfo.dColPitch      );

}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBin1FullCnt    " , DevOptn.iBin1FullCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBin2FullCnt    " , DevOptn.iBin2FullCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBin3FullCnt    " , DevOptn.iBin3FullCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBin4FullCnt    " , DevOptn.iBin4FullCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBin5FullCnt    " , DevOptn.iBin5FullCnt   );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFailCnt        " , DevOptn.iFailCnt       );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTempSet1       " , DevOptn.iTempSet1      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTempSet2       " , DevOptn.iTempSet2      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTempGap        " , DevOptn.iTempGap       );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPrbStartDelay  " , DevOptn.iPrbStartDelay );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBlowerDelay    " , DevOptn.iBlowerDelay   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPrbContactTime " , DevOptn.iPrbContactTime);

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPinChangeCnt   " , DevOptn.iPinChangeCnt  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iNoPkgSec       " , DevOptn.iNoPkgSec      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFeedMtMoveTime " , DevOptn.iFeedMtMoveTime);

    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFeedWaitTime   " , DevOptn.iFeedWaitTime  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iBin3Deley      " , DevOptn.iBin3Deley     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortDeley      " , DevOptn.iSortDeley     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bSortPKGSensor  " , DevOptn.bSortPKGSensor );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpmtyBin       " , DevOptn.iEpmtyBin      );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFeedComDelay   " , DevOptn.iFeedComDelay  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iStartHeatSec   " , DevOptn.iStartHeatSec  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iStopTemp       " , DevOptn.iStopTemp      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iStopWaitSec    " , DevOptn.iStopWaitSec   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPkgDetect      " , DevOptn.iPkgDetect     );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "dFedMinMove     " , DevOptn.dFedMinMove    );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFedFailCnt     " , DevOptn.iFedFailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPKGHeatSec     " , DevOptn.iPKGHeatSec    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iIdleTime       " , DevOptn.iIdleTime      );


}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBin1FullCnt    " , DevOptn.iBin1FullCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBin2FullCnt    " , DevOptn.iBin2FullCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBin3FullCnt    " , DevOptn.iBin3FullCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBin4FullCnt    " , DevOptn.iBin4FullCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBin5FullCnt    " , DevOptn.iBin5FullCnt   );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFailCnt        " , DevOptn.iFailCnt       );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTempSet1       " , DevOptn.iTempSet1      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTempSet2       " , DevOptn.iTempSet2      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTempGap        " , DevOptn.iTempGap       );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPrbStartDelay  " , DevOptn.iPrbStartDelay );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBlowerDelay    " , DevOptn.iBlowerDelay   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPrbContactTime " , DevOptn.iPrbContactTime);

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPinChangeCnt   " , DevOptn.iPinChangeCnt  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iNoPkgSec       " , DevOptn.iNoPkgSec      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFeedMtMoveTime " , DevOptn.iFeedMtMoveTime);

    //UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFeedWaitTime   " , DevOptn.iFeedWaitTime  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iBin3Deley      " , DevOptn.iBin3Deley     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortDeley      " , DevOptn.iSortDeley     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bSortPKGSensor  " , DevOptn.bSortPKGSensor );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpmtyBin       " , DevOptn.iEpmtyBin      );
    //UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFeedComDelay   " , DevOptn.iFeedComDelay  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iStartHeatSec   " , DevOptn.iStartHeatSec  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iStopTemp       " , DevOptn.iStopTemp      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iStopWaitSec    " , DevOptn.iStopWaitSec   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPkgDetect      " , DevOptn.iPkgDetect     );
    //UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFedVacuumDelay " , DevOptn.iFedVacuumDelay);
    //UserINI.Save(sPath.c_str()  , "DevOptn"  , "dFedMinMove     " , DevOptn.dFedMinMove    );
    //UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFedFailCnt     " , DevOptn.iFedFailCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPKGHeatSec     " , DevOptn.iPKGHeatSec    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iIdleTime       " , DevOptn.iIdleTime      );

}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bFeedingStop  " , CmnOptn.bFeedingStop  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrHeatErr  " , CmnOptn.bIgnrHeatErr  );

}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bFeedingStop  " , CmnOptn.bFeedingStop  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrHeatErr  " , CmnOptn.bIgnrHeatErr  );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode      " , MstOptn.bDebugMode      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bInspSkip       " , MstOptn.bInspSkip       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bIgnrPkgDustChk " , MstOptn.bIgnrPkgDustChk );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bFedMTPsChange  " , MstOptn.bFedMTPsChange  );

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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode      " , MstOptn.bDebugMode      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bInspSkip       " , MstOptn.bInspSkip       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bIgnrPkgDustChk " , MstOptn.bIgnrPkgDustChk );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bFedMTPsChange  " , MstOptn.bFedMTPsChange  );
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
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName    );
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
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " ,  EqpOptn.sModelName    );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device "  , m_sCrntDev );

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
    UserINI.Save(sPath, "LAST WORK INFO" , "Device "  , m_sCrntDev );

}

