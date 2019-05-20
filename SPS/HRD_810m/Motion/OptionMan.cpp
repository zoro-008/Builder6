//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "VisionMan.h"
#include "DataMan.h"
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
    VSM.LoadJobFile(true , _sDevName);




    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    VSM.LoadJobFile(false , _sDevName);




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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlotCnt    " , DevInfo.iCsSlotCnt   );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlotPitch  " , DevInfo.dCsSlotPitch );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dWfPitchX     " , DevInfo.dWfPitchX    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dWfPitchY     " , DevInfo.dWfPitchY    );


    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWFrstCol     " , DevInfo.iWFrstCol    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWFrstRow     " , DevInfo.iWFrstRow    );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iMapType      " , DevInfo.iMapType     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sExtention    " , DevInfo.sExtention   ); 
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWfColCnt     " , DevInfo.iWfColCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWfRowCnt     " , DevInfo.iWfRowCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sGoodStr1     " , DevInfo.sGoodStr1    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sEmptStr1     " , DevInfo.sEmptStr1    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sMaskStr1     " , DevInfo.sMaskStr1    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iFlatAngle    " , DevInfo.iFlatAngle   );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iMskLine      " , DevInfo.iMskLine     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "bMskExist     " , DevInfo.bMskExist    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "bUseHex       " , DevInfo.bUseHex      );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iBdXCnt       " , DevInfo.iBdXCnt      );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iBdYCnt       " , DevInfo.iBdYCnt      );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dBdXPch       " , DevInfo.dBdXPch      );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dBdYPch       " , DevInfo.dBdYPch      );


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
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iCsSlotCnt    " , DevInfo.iCsSlotCnt   );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlotPitch  " , DevInfo.dCsSlotPitch );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dWfPitchX     " , DevInfo.dWfPitchX    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dWfPitchY     " , DevInfo.dWfPitchY    );


    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWFrstCol     " , DevInfo.iWFrstCol    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWFrstRow     " , DevInfo.iWFrstRow    );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iMapType      " , DevInfo.iMapType     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sExtention    " , DevInfo.sExtention   );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWfColCnt     " , DevInfo.iWfColCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWfRowCnt     " , DevInfo.iWfRowCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sGoodStr1     " , DevInfo.sGoodStr1    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sEmptStr1     " , DevInfo.sEmptStr1    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sMaskStr1     " , DevInfo.sMaskStr1    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iFlatAngle    " , DevInfo.iFlatAngle   );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iMskLine      " , DevInfo.iMskLine     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "bMskExist     " , DevInfo.bMskExist    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "bUseHex       " , DevInfo.bUseHex      );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iBdXCnt       " , DevInfo.iBdXCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iBdYCnt       " , DevInfo.iBdYCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dBdXPch       " , DevInfo.dBdXPch      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dBdYPch       " , DevInfo.dBdYPch      );


}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    int Temp;
    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iVisnChkChipColCnt " , DevOptn.iVisnChkChipColCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iVisnChkChipRowCnt " , DevOptn.iVisnChkChipRowCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPenCheckCnt       " , DevOptn.iPenCheckCnt       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bAutoOrigin        " , DevOptn.bAutoOrigin        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriMathod         " , Temp                       ); DevOptn.iOriMathod = Temp;
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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iVisnChkChipColCnt " , DevOptn.iVisnChkChipColCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iVisnChkChipRowCnt " , DevOptn.iVisnChkChipRowCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPenCheckCnt       " , DevOptn.iPenCheckCnt       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bAutoOrigin        " , DevOptn.bAutoOrigin        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriMathod         " , DevOptn.iOriMathod         );
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoorSkip              " , CmnOptn.bDoorSkip              );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop           " , CmnOptn.bLoadingStop           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseBarcode            " , CmnOptn.bUseBarcode            );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sSourcePath            " , CmnOptn.sSourcePath            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sWorkedPath            " , CmnOptn.sWorkedPath            );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVsBfGrabDly           " , CmnOptn.iVsBfGrabDly           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dStageTLmtErr          " , CmnOptn.dStageTLmtErr          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dVisnPenYOft           " , CmnOptn.dVisnPenYOft           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPenDnCnt              " , CmnOptn.iPenDnCnt              );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPenAtDnDly            " , CmnOptn.iPenAtDnDly            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPenDnOverCnt          " , CmnOptn.iPenDnOverCnt          ); 

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadOnlyOneMapMarking " , CmnOptn.bLoadOnlyOneMapMarking );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sLoadOnlyOneMapMrkPath " , CmnOptn.sLoadOnlyOneMapMrkPath );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUse1stBarcode         " , CmnOptn.bUse1stBarcode         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseEndCam             " , CmnOptn.bUseEndCam             );

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
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoorSkip              " , CmnOptn.bDoorSkip              );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop           " , CmnOptn.bLoadingStop           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseBarcode            " , CmnOptn.bUseBarcode            );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sSourcePath            " , CmnOptn.sSourcePath            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sWorkedPath            " , CmnOptn.sWorkedPath            );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVsBfGrabDly           " , CmnOptn.iVsBfGrabDly           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dStageTLmtErr          " , CmnOptn.dStageTLmtErr          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dVisnPenYOft           " , CmnOptn.dVisnPenYOft           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPenDnCnt              " , CmnOptn.iPenDnCnt              );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPenAtDnDly            " , CmnOptn.iPenAtDnDly            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPenDnOverCnt          " , CmnOptn.iPenDnOverCnt          );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadOnlyOneMapMarking " , CmnOptn.bLoadOnlyOneMapMarking );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sLoadOnlyOneMapMrkPath " , CmnOptn.sLoadOnlyOneMapMrkPath );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUse1stBarcode         " , CmnOptn.bUse1stBarcode         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseEndCam             " , CmnOptn.bUseEndCam             );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode           " , MstOptn.bDebugMode           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sDeviceBackUpPath    " , MstOptn.sDeviceBackUpPath    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDryRun              " , MstOptn.bDryRun              );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bVacuum              " , MstOptn.bVacuum              );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bHopJumpChip         " , MstOptn.bHopJumpChip         );
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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode           " , MstOptn.bDebugMode           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sDeviceBackUpPath    " , MstOptn.sDeviceBackUpPath    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDryRun              " , MstOptn.bDryRun              );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bVacuum              " , MstOptn.bVacuum              );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bHopJumpChip         " , MstOptn.bHopJumpChip         );

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRD-810M";

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

