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
    Init();
}

//---------------------------------------------------------------------------
COptionMan::~COptionMan(void)
{
    Close();
}

void COptionMan::Close(void)
{
    SaveLastInfo();
}
//---------------------------------------------------------------------------
void COptionMan::Init(void)
{
    //Init. Buffer.
    memset(&DevOptn  , 0 , sizeof(DevOptn ));
    //memset(&EqpOptn  , 0 , sizeof(EqpOptn ));


    Trace("OPTION","Init()" );

    LoadLastInfo(          );
    LoadMstOptn (          );
    LoadEqpOptn (          );
    LoadCmnOptn (          );
    //LoadDevInfo (m_sCrntDev);
    //LoadDevOptn (m_sCrntDev);
    LoadJobFile (m_sCrntDev);
}

void COptionMan::SetCrntDev (AnsiString _sName)
{
    m_sCrntDev = _sName ;
}

void COptionMan::LoadJobFile(AnsiString _sDevName)
{
    Trace("OPTION",("LoadJobFile()" + _sDevName).c_str() );
    LoadDevInfo (_sDevName);
    LoadDevOptn (_sDevName);
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    Trace("OPTION",("LoadJobFile()" + _sDevName).c_str() );
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{


}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{


}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    Trace("OPTION","LoadDevOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSpinMtJogSpeed " , DevOptn.iSpinMtJogSpeed  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTiltMtStartPs  " , DevOptn.iTiltMtStartPs   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTiltMtStopPs   " , DevOptn.iTiltMtStopPs    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dTiltMtSpeed    " , DevOptn.dTiltMtSpeed     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCleanRPM       " , DevOptn.iCleanRPM        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iAirRPM         " , DevOptn.iAirRPM          );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDryRPM         " , DevOptn.iDryRPM          );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCleanTime      " , DevOptn.iCleanTime       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iIONTime        " , DevOptn.iAirTime         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDRYTime        " , DevOptn.iDryTime         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWaterDrainTime " , DevOptn.iWaterDrainTime  );

}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    Trace("OPTION","SaveDevOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSpinMtJogSpeed " , DevOptn.iSpinMtJogSpeed  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTiltMtStartPs  " , DevOptn.iTiltMtStartPs   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTiltMtStopPs   " , DevOptn.iTiltMtStopPs    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dTiltMtSpeed    " , DevOptn.dTiltMtSpeed     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCleanRPM       " , DevOptn.iCleanRPM        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iAirRPM         " , DevOptn.iAirRPM          );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDryRPM         " , DevOptn.iDryRPM          );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCleanTime      " , DevOptn.iCleanTime       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iIONTime        " , DevOptn.iAirTime         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDRYTime        " , DevOptn.iDryTime         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWaterDrainTime " , DevOptn.iWaterDrainTime  );

    SaveLastInfo();
}

void COptionMan::LoadCmnOptn()
{
    Trace("OPTION","LoadCmnOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEmgLogic         " , CmnOptn.iEmgLogic         );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRatio1           " , CmnOptn.iRatio1           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPlusLimitLogic1  " , CmnOptn.iPlusLimitLogic1  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iMinusLimitLogic1 " , CmnOptn.iMinusLimitLogic1 );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iAlarmLogic1      " , CmnOptn.iAlarmLogic1      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iNearLogic1       " , CmnOptn.iNearLogic1       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iInPoLogic1       " , CmnOptn.iInPoLogic1       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iSvReadyLogic1    " , CmnOptn.iSvReadyLogic1    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEncZLogic1       " , CmnOptn.iEncZLogic1       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEncDir1          " , CmnOptn.iEncDir1          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEncCount1        " , CmnOptn.iEncCount1        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCurrentOn1       " , CmnOptn.iCurrentOn1       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iDCCOn1           " , CmnOptn.iDCCOn1           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iAlarmResetOn1    " , CmnOptn.iAlarmResetOn1    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPulseLogic1      " , CmnOptn.iPulseLogic1      );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRatio2           " , CmnOptn.iRatio2           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPlusLimitLogic2  " , CmnOptn.iPlusLimitLogic2  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iMinusLimitLogic2 " , CmnOptn.iMinusLimitLogic2 );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iAlarmLogic2      " , CmnOptn.iAlarmLogic2      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iNearLogic2       " , CmnOptn.iNearLogic2       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iInPoLogic2       " , CmnOptn.iInPoLogic2       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iSvReadyLogic2    " , CmnOptn.iSvReadyLogic2    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEncZLogic2       " , CmnOptn.iEncZLogic2       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEncDir2          " , CmnOptn.iEncDir2          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEncCount2        " , CmnOptn.iEncCount2        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCurrentOn2       " , CmnOptn.iCurrentOn2       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iDCCOn2           " , CmnOptn.iDCCOn2           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iAlarmResetOn2    " , CmnOptn.iAlarmResetOn2    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPulseLogic2      " , CmnOptn.iPulseLogic2      );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dSpinMLimit       " , CmnOptn.dSpinMLimit       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dSpinPLimit       " , CmnOptn.dSpinPLimit       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dTiltMLimit       " , CmnOptn.dTiltMLimit       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dTiltPLimit       " , CmnOptn.dTiltPLimit       );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedSpinLimit    " , CmnOptn.bUsedSpinLimit    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedTiltLimit    " , CmnOptn.bUsedTiltLimit    );
}

void COptionMan::SaveCmnOptn()
{
    Trace("OPTION","SaveCmnOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEmgLogic          " , CmnOptn.iEmgLogic         );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRatio1            " , CmnOptn.iRatio1           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPlusLimitLogic1   " , CmnOptn.iPlusLimitLogic1  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iMinusLimitLogic1  " , CmnOptn.iMinusLimitLogic1 );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iAlarmLogic1       " , CmnOptn.iAlarmLogic1      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iNearLogic1        " , CmnOptn.iNearLogic1       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iInPoLogic1        " , CmnOptn.iInPoLogic1       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iSvReadyLogic1     " , CmnOptn.iSvReadyLogic1    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEncZLogic1        " , CmnOptn.iEncZLogic1       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEncDir1           " , CmnOptn.iEncDir1          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEncCount1         " , CmnOptn.iEncCount1        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCurrentOn1        " , CmnOptn.iCurrentOn1       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iDCCOn1            " , CmnOptn.iDCCOn1           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iAlarmResetOn1     " , CmnOptn.iAlarmResetOn1    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPulseLogic1       " , CmnOptn.iPulseLogic1      );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRatio2            " , CmnOptn.iRatio2           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPlusLimitLogic2   " , CmnOptn.iPlusLimitLogic2  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iMinusLimitLogic2  " , CmnOptn.iMinusLimitLogic2 );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iAlarmLogic2       " , CmnOptn.iAlarmLogic2      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iNearLogic2        " , CmnOptn.iNearLogic2       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iInPoLogic2        " , CmnOptn.iInPoLogic2       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iSvReadyLogic2     " , CmnOptn.iSvReadyLogic2    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEncZLogic2        " , CmnOptn.iEncZLogic2       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEncDir2           " , CmnOptn.iEncDir2          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEncCount2         " , CmnOptn.iEncCount2        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCurrentOn2        " , CmnOptn.iCurrentOn2       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iDCCOn2            " , CmnOptn.iDCCOn2           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iAlarmResetOn2     " , CmnOptn.iAlarmResetOn2    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPulseLogic2       " , CmnOptn.iPulseLogic2      );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dSpinMLimit        " , CmnOptn.dSpinMLimit       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dSpinPLimit        " , CmnOptn.dSpinPLimit       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dTiltMLimit        " , CmnOptn.dTiltMLimit       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dTiltPLimit        " , CmnOptn.dTiltPLimit       );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedSpinLimit     " , CmnOptn.bUsedSpinLimit    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedTiltLimit     " , CmnOptn.bUsedTiltLimit    );


}

void COptionMan::LoadMstOptn()
{
    Trace("OPTION","LoadMstOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.



}

void COptionMan::SaveMstOptn()
{
    Trace("OPTION","SaveMstOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    


}
void COptionMan::LoadEqpOptn()
{
    Trace("OPTION","LoadEqpOptn()" );

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRW-150P";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    //Load.
    //UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName    );
    //UserINI.Load(sPath, "EqpOptn"  , "iTotalWork    " , EqpOptn.iTotalWork    );

}
void COptionMan::SaveEqpOptn()
{
    Trace("OPTION","SaveEqpOptn()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save.
    //UserINI.Save(sPath, "EqpOptn"  , "sModelName    " ,  EqpOptn.sModelName    );
    //UserINI.Save(sPath, "EqpOptn"  , "iTotalWork    " ,  EqpOptn.iTotalWork    );
}

void COptionMan::LoadLastInfo()
{
    Trace("OPTION","LoadLastInfo()" );
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
    Trace("OPTION","SaveLastInfo()" );
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "LAST WORK INFO" , "Device "  , m_sCrntDev );

}

