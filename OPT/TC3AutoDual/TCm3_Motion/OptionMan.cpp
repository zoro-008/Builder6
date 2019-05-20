//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UserIni.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
COptionMan OM;

//---------------------------------------------------------------------------
COptionMan::COptionMan(void)
{
    Init();

    LoadCmnOptn();
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
//    memset(&Password , 0 , sizeof(Password));

    //Set Default User Level.
//    m_iCrntLvl = lvOperator ;
//    m_iSelLvl  = lvOperator ;
}

//void COptionMan::ChangeLevel(int _iLevel)
//{
//    m_iCrntLvl = _iLevel ;
//}
//
//bool COptionMan::CheckPassword (int _iLevel , AnsiString _sPassword)
//{
//    switch(_iLevel) {
//        case lvOperator : return true ;
//        case lvEngineer : return (Password.sEngrPass   == _sPassword) ;
//        case lvMaster   : return (Password.sMasterPass == _sPassword || _sPassword == "123" ) ;
//    }
//
//    return false ;
//}
//
//bool COptionMan::ChangePassword(int _iLevel , AnsiString _sOldPass , AnsiString _sNewPass)
//{
//    if(!CheckPassword(_iLevel , _sOldPass)) return false ;
//    switch(_iLevel) {
//        case lvEngineer : Password.sEngrPass   = _sNewPass ; break ;
//        case lvMaster   : Password.sMasterPass = _sNewPass ; break ;
//    }
//
//    SavePassword();
//
//    return true ;
//}
//

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
    //UserINI.Load(sPath , "DevInfo"  , "iCsMaxSlot    " , DevInfo.iCsMaxSlot    );
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    UserINI.ClearFile(sPath) ;

    //Load Device.
    //UserINI.Save(sPath , "DevInfo"  , "iCsMaxSlot    " , DevInfo.iCsMaxSlot    );
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
    UserINI.Load(sPath, "DevOptn"  , "iSptrDnDelay     " , DevOptn.iSptrDnDelay     );
    UserINI.Load(sPath, "DevOptn"  , "iSptrEndUpDelay  " , DevOptn.iSptrEndUpDelay  );

    UserINI.Load(sPath, "DevOptn"  , "dMaxZPos         " , DevOptn.dMaxZPos         );
    UserINI.Load(sPath, "DevOptn"  , "dMaxYPos         " , DevOptn.dMaxYPos         );
    UserINI.Load(sPath, "DevOptn"  , "dMaxIPos         " , DevOptn.dMaxIPos         );
    UserINI.Load(sPath, "DevOptn"  , "iMaxInspCnt      " , DevOptn.iMaxInspCnt      );
    UserINI.Load(sPath, "DevOptn"  , "iInspDelay       " , DevOptn.iInspDelay       );
    UserINI.Load(sPath, "DevOptn"  , "bInspAftrDisp    " , DevOptn.bInspAftrDisp    );
    UserINI.Load(sPath, "DevOptn"  , "bInspBefrDisp    " , DevOptn.bInspBefrDisp    );
    UserINI.Load(sPath, "DevOptn"  , "bInspDrngDisp    " , DevOptn.bInspDrngDisp    );
    UserINI.Load(sPath, "DevOptn"  , "bAPCOnReadjust   " , DevOptn.bAPCOnReadjust   );
    UserINI.Load(sPath, "DevOptn"  , "bAdjGT2ndInsp    " , DevOptn.bAdjGT2ndInsp    );
    UserINI.Load(sPath, "DevOptn"  , "bDispFirst       " , DevOptn.bDispFirst       );
    UserINI.Load(sPath, "DevOptn"  , "bBarcodeScan     " , DevOptn.bBarcodeScan     );
    UserINI.Load(sPath, "DevOptn"  , "dAgRes           " , DevOptn.dAgRes           );
    UserINI.Load(sPath, "DevOptn"  , "dSfRes           " , DevOptn.dSfRes           );
    UserINI.Load(sPath, "DevOptn"  , "dApcRes          " , DevOptn.dApcRes          );
    UserINI.Load(sPath, "DevOptn"  , "dAPCSetVal       " , DevOptn.dAPCSetVal       );
    UserINI.Load(sPath, "DevOptn"  , "dAPCCalLim       " , DevOptn.dAPCCalLim       );
    UserINI.Load(sPath, "DevOptn"  , "dAPCLim          " , DevOptn.dAPCLim          );
    UserINI.Load(sPath, "DevOptn"  , "bUseAngleCal     " , DevOptn.bUseAngleCal     );
    UserINI.Load(sPath, "DevOptn"  , "bUseShiftCal     " , DevOptn.bUseShiftCal     );
    UserINI.Load(sPath, "DevOptn"  , "bUseAPCCal       " , DevOptn.bUseAPCCal       );

    for(int i = 0 ; i < MAX_DISP_NOZZLE ; i++) {
        for(int j = 0 ; j < MAX_DISP_CNT ; j++) {
            sTemp = "iLTDisp_"+(AnsiString)i +"_"+ (AnsiString)j ;
            UserINI.Load(sPath, "DevOptn"  , sTemp , DevOptn.iLTDisp[i][j] );
            sTemp = "iRTDisp_"+(AnsiString)i +"_"+ (AnsiString)j ;
            UserINI.Load(sPath, "DevOptn"  , sTemp , DevOptn.iRTDisp[i][j] );
        }
    }

    UserINI.Load(sPath, "DevOptn"  , "iSuckTime       " , DevOptn.iSuckTime       );
    UserINI.Load(sPath, "DevOptn"  , "iDispEndDelay   " , DevOptn.iDispEndDelay   );
    UserINI.Load(sPath, "DevOptn"  , "iUVTime         " , DevOptn.iUVTime         );
    UserINI.Load(sPath, "DevOptn"  , "bUseBeamRslt    " , DevOptn.bUseBeamRslt    );
    UserINI.Load(sPath, "DevOptn"  , "bUseAvrPos      " , DevOptn.bUseAvrPos      );
    UserINI.Load(sPath, "DevOptn"  , "iAvrPosCnt      " , DevOptn.iAvrPosCnt      );
    UserINI.Load(sPath, "DevOptn"  , "bUseHomming     " , DevOptn.bUseHomming     );
    UserINI.Load(sPath, "DevOptn"  , "bInspAfterDpDone" , DevOptn.bInspAfterDpDone);
    UserINI.Load(sPath, "DevOptn"  , "bIgnrDisp       " , DevOptn.bIgnrDisp       );

    UserINI.Load(sPath, "DevOptn"  , "bROMType        " , DevOptn.bROMType        );
    UserINI.Load(sPath, "DevOptn"  , "bAskInsp        " , DevOptn.bAskInsp        );
    UserINI.Load(sPath, "DevOptn"  , "bGrabMode       " , DevOptn.bGrabMode       );
    UserINI.Load(sPath, "DevOptn"  , "bDryRun         " , DevOptn.bDryRun         );
    UserINI.Load(sPath, "DevOptn"  , "iModeSelect     " , DevOptn.iModeSelect     );
    UserINI.Load(sPath, "DevOptn"  , "bDataCheck      " , DevOptn.bDataCheck      );
    UserINI.Load(sPath, "DevOptn"  , "bInfinityInsp   " , DevOptn.bInfinityInsp   );


    UserINI.Load(sPath, "DevOptn"  , "dAPCUnit        " , DevOptn.dAPCUnit        );
    UserINI.Load(sPath, "DevOptn"  , "iAPCOnDelay     " , DevOptn.iAPCOnDelay     );

    UserINI.Load(sPath, "DevOptn"  , "iAngYVibCnt     " , DevOptn.iAngYVibCnt     );
    UserINI.Load(sPath, "DevOptn"  , "iAngZVibCnt     " , DevOptn.iAngZVibCnt     );

    UserINI.Load(sPath, "DevOptn"  , "iSfVibCnt       " , DevOptn.iSfVibCnt       );
    UserINI.Load(sPath, "DevOptn"  , "iGTDnVibCnt     " , DevOptn.iGTDnVibCnt     );
    UserINI.Load(sPath, "DevOptn"  , "dGTDnVibRng     " , DevOptn.dGTDnVibRng     );
    UserINI.Load(sPath, "DevOptn"  , "dGTUpOffset     " , DevOptn.dGTUpOffset     );
    UserINI.Load(sPath, "DevOptn"  , "iGT3rdInspCnt   " , DevOptn.iGT3rdInspCnt   );

    UserINI.Load(sPath, "DevOptn"  , "dSfVibRng       " , DevOptn.dSfVibRng       );
    UserINI.Load(sPath, "DevOptn"  , "dAngVibRng      " , DevOptn.dAngVibRng      );
    UserINI.Load(sPath, "DevOptn"  , "dFindAngle      " , DevOptn.dFindAngle      );



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
    UserINI.Save(sPath, "DevOptn"  , "iSptrDnDelay     " , DevOptn.iSptrDnDelay      );
    UserINI.Save(sPath, "DevOptn"  , "iSptrEndUpDelay  " , DevOptn.iSptrEndUpDelay   );

    UserINI.Save(sPath, "DevOptn"  , "dMaxZPos         " , DevOptn.dMaxZPos          );
    UserINI.Save(sPath, "DevOptn"  , "dMaxYPos         " , DevOptn.dMaxYPos          );
    UserINI.Save(sPath, "DevOptn"  , "dMaxIPos         " , DevOptn.dMaxIPos          );
    UserINI.Save(sPath, "DevOptn"  , "iMaxInspCnt      " , DevOptn.iMaxInspCnt       );
    UserINI.Save(sPath, "DevOptn"  , "iInspDelay       " , DevOptn.iInspDelay        );
    UserINI.Save(sPath, "DevOptn"  , "bInspAftrDisp    " , DevOptn.bInspAftrDisp     );
    UserINI.Save(sPath, "DevOptn"  , "bInspBefrDisp    " , DevOptn.bInspBefrDisp     );
    UserINI.Save(sPath, "DevOptn"  , "bInspDrngDisp    " , DevOptn.bInspDrngDisp     );
    UserINI.Save(sPath, "DevOptn"  , "bAPCOnReadjust   " , DevOptn.bAPCOnReadjust    );
    UserINI.Save(sPath, "DevOptn"  , "bAdjGT2ndInsp    " , DevOptn.bAdjGT2ndInsp     );
    UserINI.Save(sPath, "DevOptn"  , "bDispFirst       " , DevOptn.bDispFirst        ); 
    UserINI.Save(sPath, "DevOptn"  , "bBarcodeScan     " , DevOptn.bBarcodeScan      );
    UserINI.Save(sPath, "DevOptn"  , "dAgRes           " , DevOptn.dAgRes            );
    UserINI.Save(sPath, "DevOptn"  , "dSfRes           " , DevOptn.dSfRes            );
    UserINI.Save(sPath, "DevOptn"  , "dApcRes          " , DevOptn.dApcRes           );
    UserINI.Save(sPath, "DevOptn"  , "dAPCSetVal       " , DevOptn.dAPCSetVal        );
    UserINI.Save(sPath, "DevOptn"  , "dAPCCalLim       " , DevOptn.dAPCCalLim        );

    UserINI.Save(sPath, "DevOptn"  , "dAPCLim          " , DevOptn.dAPCLim           );
    UserINI.Save(sPath, "DevOptn"  , "bUseAngleCal     " , DevOptn.bUseAngleCal      );
    UserINI.Save(sPath, "DevOptn"  , "bUseShiftCal     " , DevOptn.bUseShiftCal      );
    UserINI.Save(sPath, "DevOptn"  , "bUseAPCCal       " , DevOptn.bUseAPCCal        );


    for(int i = 0 ; i < MAX_DISP_NOZZLE ; i++) {
        for(int j = 0 ; j < MAX_DISP_CNT ; j++) {
            sTemp = "iLTDisp_"+(AnsiString)i +"_"+ (AnsiString)j ;
            UserINI.Save(sPath, "DevOptn"  , sTemp , DevOptn.iLTDisp[i][j] );
            sTemp = "iRTDisp_"+(AnsiString)i +"_"+ (AnsiString)j ;
            UserINI.Save(sPath, "DevOptn"  , sTemp , DevOptn.iRTDisp[i][j] );
        }
    }

    UserINI.Save(sPath, "DevOptn"  , "iDispEndDelay   " , DevOptn.iDispEndDelay   );
    UserINI.Save(sPath, "DevOptn"  , "bUseBeamRslt    " , DevOptn.bUseBeamRslt    );
    UserINI.Save(sPath, "DevOptn"  , "bUseAvrPos      " , DevOptn.bUseAvrPos      );
    UserINI.Save(sPath, "DevOptn"  , "iAvrPosCnt      " , DevOptn.iAvrPosCnt      );
    UserINI.Save(sPath, "DevOptn"  , "bUseHomming     " , DevOptn.bUseHomming     );
    UserINI.Save(sPath, "DevOptn"  , "bInspAfterDpDone" , DevOptn.bInspAfterDpDone);
    UserINI.Save(sPath, "DevOptn"  , "bIgnrDisp       " , DevOptn.bIgnrDisp       );

    UserINI.Save(sPath, "DevOptn"  , "iSuckTime       " , DevOptn.iSuckTime       );
    UserINI.Save(sPath, "DevOptn"  , "iUVTime         " , DevOptn.iUVTime         );

    UserINI.Save(sPath, "DevOptn"  , "bROMType        " , DevOptn.bROMType        );
    UserINI.Save(sPath, "DevOptn"  , "bAskInsp        " , DevOptn.bAskInsp        );
    UserINI.Save(sPath, "DevOptn"  , "bGrabMode       " , DevOptn.bGrabMode       );
    UserINI.Save(sPath, "DevOptn"  , "bDryRun         " , DevOptn.bDryRun         );
    UserINI.Save(sPath, "DevOptn"  , "iModeSelect     " , DevOptn.iModeSelect     ); 
    UserINI.Save(sPath, "DevOptn"  , "bDataCheck      " , DevOptn.bDataCheck      );
    UserINI.Save(sPath, "DevOptn"  , "bInfinityInsp   " , DevOptn.bInfinityInsp   );

    UserINI.Save(sPath, "DevOptn"  , "dAPCUnit        " , DevOptn.dAPCUnit        );
    UserINI.Save(sPath, "DevOptn"  , "iAPCOnDelay     " , DevOptn.iAPCOnDelay     );

    UserINI.Save(sPath, "DevOptn"  , "iAngYVibCnt     " , DevOptn.iAngYVibCnt     );
    UserINI.Save(sPath, "DevOptn"  , "iAngZVibCnt     " , DevOptn.iAngZVibCnt     );

    UserINI.Save(sPath, "DevOptn"  , "iSfVibCnt       " , DevOptn.iSfVibCnt       );
    UserINI.Save(sPath, "DevOptn"  , "iGTDnVibCnt     " , DevOptn.iGTDnVibCnt     );
    UserINI.Save(sPath, "DevOptn"  , "dGTDnVibRng     " , DevOptn.dGTDnVibRng     );
    UserINI.Save(sPath, "DevOptn"  , "dGTUpOffset     " , DevOptn.dGTUpOffset     );
    UserINI.Save(sPath, "DevOptn"  , "iGT3rdInspCnt   " , DevOptn.iGT3rdInspCnt   );

    UserINI.Save(sPath, "DevOptn"  , "dSfVibRng       " , DevOptn.dSfVibRng       );
    UserINI.Save(sPath, "DevOptn"  , "dAngVibRng      " , DevOptn.dAngVibRng      );
    UserINI.Save(sPath, "DevOptn"  , "dFindAngle      " , DevOptn.dFindAngle      );


}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load.
    //UserINI.Load(sPath, "CmnOptn"  , "iDispEndDelay" , CmnOptn.iDispEndDelay );
    //UserINI.Load(sPath, "CmnOptn"  , "iSuckTime    " , CmnOptn.iSuckTime     );
}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save.
    //UserINI.Save(sPath, "CmnOptn"  , "iDispEndDelay" , CmnOptn.iDispEndDelay );
    //UserINI.Save(sPath, "CmnOptn"  , "iSuckTime    " , CmnOptn.iSuckTime     );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "MstOptn"  , "bSunMistake     " , MstOptn.bSunMistake     );
}
void COptionMan::SaveMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save.
    UserINI.Save(sPath, "MstOptn"  , "bSunMistake     " , MstOptn.bSunMistake     );
}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    //Load.
    //UserINI.Load(sPath, "EqpOptn"  , "bExstPreHeater" , EqpOptn.bExstPreHeater );

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
    //UserINI.Save(sPath, "EqpOptn"  , "bExstPreHeater" , EqpOptn.bExstPreHeater );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
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
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );
}

//void COptionMan::LoadPassword()
//{
//    //Local Var.
//    TUserINI   UserINI;
//    AnsiString sPath  ;
//
//    //Make Dir.
//    sPath= EXE_FOLDER + "Util\\Password.INI" ;
//
//    //Load
//    UserINI.Load(sPath, "Password" , "EngrPass  " , Password.sEngrPass  );
//    UserINI.Load(sPath, "PASSWORD" , "MasterPass" , Password.sMasterPass);
//}
//
//void COptionMan::SavePassword()
//{
//    //Local Var.
//    TUserINI   UserINI;
//    AnsiString sPath  ;
//
//    //Make Dir.
//    sPath= EXE_FOLDER + "Util\\Password.INI" ;
//
//    //Save
//    UserINI.Save(sPath, "PASSWORD" , "EngrPass  " , Password.sEngrPass  );
//    UserINI.Save(sPath, "PASSWORD" , "MasterPass" , Password.sMasterPass);
//}
//
//

