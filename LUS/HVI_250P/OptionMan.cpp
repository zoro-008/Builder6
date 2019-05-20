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
//    SaveLastInfo();
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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch       " , DevInfo.dColPitch       ); if(!DevInfo.dColPitch     )DevInfo.dColPitch      = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch       " , DevInfo.dRowPitch       ); if(!DevInfo.dRowPitch     )DevInfo.dRowPitch      = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt       " , DevInfo.iColGrCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt       " , DevInfo.iRowGrCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap       " , DevInfo.dColGrGap       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap       " , DevInfo.dRowGrGap       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt         " , DevInfo.iColCnt         ); if(!DevInfo.iColCnt       )DevInfo.iColCnt        = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt         " , DevInfo.iRowCnt         ); if(!DevInfo.iRowCnt       )DevInfo.iRowCnt        = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlCnt        " , DevInfo.iCsSlCnt        ); if(!DevInfo.iCsSlCnt      )DevInfo.iCsSlCnt       = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlPitch      " , DevInfo.dCsSlPitch      ); if(!DevInfo.dCsSlPitch    )DevInfo.dCsSlPitch     = 1 ;
//    UserINI.Load(sPath.c_str()  , "DevInfo"  , "i1IdxReWorkCnt  " , DevInfo.i1IdxReWorkCnt  ); if(!DevInfo.i1IdxReWorkCnt)DevInfo.i1IdxReWorkCnt = 1 ;


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
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch       " , DevInfo.dColPitch       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowPitch       " , DevInfo.dRowPitch       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColGrCnt       " , DevInfo.iColGrCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowGrCnt       " , DevInfo.iRowGrCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColGrGap       " , DevInfo.dColGrGap       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowGrGap       " , DevInfo.dRowGrGap       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColCnt         " , DevInfo.iColCnt         );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowCnt         " , DevInfo.iRowCnt         );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iCsSlCnt        " , DevInfo.iCsSlCnt        );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlPitch      " , DevInfo.dCsSlPitch      );
//    UserINI.Save(sPath.c_str()  , "DevInfo"  , "i1IdxReWorkCnt  " , DevInfo.i1IdxReWorkCnt  );

}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTrimColCnt    " , DevOptn.iTrimColCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt  " , DevOptn.iLotEndMgzCnt  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1IdxReWorkCnt " , DevOptn.i1IdxReWorkCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2IdxReWorkCnt " , DevOptn.i2IdxReWorkCnt );

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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTrimColCnt    " , DevOptn.iTrimColCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt  " , DevOptn.iLotEndMgzCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1IdxReWorkCnt " , DevOptn.i1IdxReWorkCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2IdxReWorkCnt " , DevOptn.i2IdxReWorkCnt );

}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDryRun            " , CmnOptn.bDryRun               );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIdleRun           " , CmnOptn.bIdleRun              );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoorSkip          " , CmnOptn.bDoorSkip             );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bBarCodePrintSkip  " , CmnOptn.bBarCodePrintSkip     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoOperation     " , CmnOptn.bAutoOperation        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoConvesion     " , CmnOptn.bAutoConvesion        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseOnly1Bin       " , CmnOptn.bUseOnly1Bin          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPunchAirDelay     " , CmnOptn.iPunchAirDelay        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dPreIdxOutSpd      " , CmnOptn.dPreIdxOutSpd         );
//    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "i1IdxReWorkCnt     " , CmnOptn.i1IdxReWorkCnt        );


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
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDryRun            " , CmnOptn.bDryRun               );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIdleRun           " , CmnOptn.bIdleRun              );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoorSkip          " , CmnOptn.bDoorSkip             );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bBarCodePrintSkip  " , CmnOptn.bBarCodePrintSkip     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoOperation     " , CmnOptn.bAutoOperation        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoConvesion     " , CmnOptn.bAutoConvesion        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseOnly1Bin       " , CmnOptn.bUseOnly1Bin          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPunchAirDelay     " , CmnOptn.iPunchAirDelay        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dPreIdxOutSpd      " , CmnOptn.dPreIdxOutSpd         );      
//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "i1IdxReWorkCnt     " , CmnOptn.i1IdxReWorkCnt        );

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode    " , MstOptn.bDebugMode    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sPstEquipPath " , MstOptn.sPstEquipPath );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sPreEquipPath " , MstOptn.sPreEquipPath );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bGoodChipAdd  " , MstOptn.bGoodChipAdd  );
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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode    " , MstOptn.bDebugMode    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sPstEquipPath " , MstOptn.sPstEquipPath );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sPreEquipPath " , MstOptn.sPreEquipPath );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bGoodChipAdd  " , MstOptn.bGoodChipAdd  );

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRT-430S";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

//    if(!FileExists(sPath)) {
//        UserINI.Save(sPath, "EqpOptn"  , "sModelName    " , sModelName );
//        UserINI.Save(sPath, "EqpOptn"  , "bExistLoader  " , "1"        );
//        UserINI.Save(sPath, "EqpOptn"  , "bExistUnLoader" , "0"        );
//    }

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );
    UserINI.Load(sPath, "EqpOptn"  , "bExistLoader  " , EqpOptn.bExistLoader   );
    UserINI.Load(sPath, "EqpOptn"  , "bExistUnLoader" , EqpOptn.bExistUnLoader );

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
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " ,  EqpOptn.sModelName     );
    UserINI.Save(sPath, "EqpOptn"  , "bExistLoader  " ,  EqpOptn.bExistLoader   );
    UserINI.Save(sPath, "EqpOptn"  , "bExistUnLoader" ,  EqpOptn.bExistUnLoader );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO    " , "Device        "  , m_sCrntDev     );

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
    UserINI.Save(sPath, "LAST WORK INFO    " , "Device        "  , m_sCrntDev     );
}

