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
    //sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    ////Load Device.
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dColPitch   " , DevInfo.dColPitch   ); if(!DevInfo.dColPitch   )DevInfo.dColPitch    = 1.0 ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "iColGrCnt   " , DevInfo.iColGrCnt   ); //if(!DevInfo.iColGrCnt   )DevInfo.iColGrCnt    = 1   ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dColGrGap   " , DevInfo.dColGrGap   ); //if(!DevInfo.dColGrGap   )DevInfo.dColGrGap    = 1.0 ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "iColCnt     " , DevInfo.iColCnt     ); if(!DevInfo.iColCnt     )DevInfo.iColCnt      = 1   ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dRowPitch   " , DevInfo.dRowPitch   ); if(!DevInfo.dRowPitch   )DevInfo.dRowPitch    = 1.0 ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "iRowGrCnt   " , DevInfo.iRowGrCnt   ); //if(!DevInfo.iRowGrCnt   )DevInfo.iRowGrCnt    = 1   ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dRowGrGap   " , DevInfo.dRowGrGap   ); //if(!DevInfo.dRowGrGap   )DevInfo.dRowGrGap    = 1.0 ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "iRowCnt     " , DevInfo.iRowCnt     ); if(!DevInfo.iRowCnt     )DevInfo.iRowCnt      = 5   ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "iCsSlCnt    " , DevInfo.iCsSlCnt    ); if(!DevInfo.iCsSlCnt    )DevInfo.iCsSlCnt     = 20  ;
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dCsSlPitch  " , DevInfo.dCsSlPitch  ); if(!DevInfo.dCsSlPitch  )DevInfo.dCsSlPitch   = 5.0 ;

    sPath = SERVER_FOLDER + "\\" + _sDevName + "\\DevInfo.INI";

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

    sPath = SERVER_FOLDER + "\\" + _sDevName + "\\DevInfo.INI";

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
    //sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;
    //
    ////Load Device.
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt     " , DevOptn.iLotEndMgzCnt     );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSBOutPshrDelay  " , DevOptn.iPSBOutPshrDelay  );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSBUpDnPshrDelay " , DevOptn.iPSBUpDnPshrDelay );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrLTimer     " , DevOptn.iDispsrLTimer     );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrRTimer     " , DevOptn.iDispsrRTimer     );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrCnt        " , DevOptn.iDispsrCnt        );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrSnsrTimer  " , DevOptn.iDispsrSnsrTimer  );

    sPath= SERVER_FOLDER + "\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt     " , DevOptn.iLotEndMgzCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSBOutPshrDelay  " , DevOptn.iPSBOutPshrDelay  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSBUpDnPshrDelay " , DevOptn.iPSBUpDnPshrDelay );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrLTimer     " , DevOptn.iDispsrLTimer     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrRTimer     " , DevOptn.iDispsrRTimer     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrCnt        " , DevOptn.iDispsrCnt        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDispsrSnsrTimer  " , DevOptn.iDispsrSnsrTimer  );



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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt     " , DevOptn.iLotEndMgzCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSBOutPshrDelay  " , DevOptn.iPSBOutPshrDelay  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSBUpDnPshrDelay " , DevOptn.iPSBUpDnPshrDelay );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrLTimer     " , DevOptn.iDispsrLTimer     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrRTimer     " , DevOptn.iDispsrRTimer     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrCnt        " , DevOptn.iDispsrCnt        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrSnsrTimer  " , DevOptn.iDispsrSnsrTimer  );

    sPath= SERVER_FOLDER + "\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt     " , DevOptn.iLotEndMgzCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSBOutPshrDelay  " , DevOptn.iPSBOutPshrDelay  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSBUpDnPshrDelay " , DevOptn.iPSBUpDnPshrDelay );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrLTimer     " , DevOptn.iDispsrLTimer     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrRTimer     " , DevOptn.iDispsrRTimer     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrCnt        " , DevOptn.iDispsrCnt        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDispsrSnsrTimer  " , DevOptn.iDispsrSnsrTimer  );


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
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop   " , CmnOptn.bLoadingStop     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor      " , CmnOptn.bIgnrDoor        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDisps     " , CmnOptn.bIgnrDisps       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bEmptyStrpCnt  " , CmnOptn.bEmptyStrpCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDsprSnsr  " , CmnOptn.bIgnrDsprSnsr    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDsprExpr  " , CmnOptn.bIgnrDsprExpr    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEmptyStrpCnt  " , CmnOptn.iEmptyStrpCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iExpiryDay     " , CmnOptn.iExpiryDay       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "LQSupplyDay    " , CmnOptn.LQSupplyDay.Val  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iDisprCleanCnt " , CmnOptn.iDisprCleanCnt   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sServerPath    " , CmnOptn.sServerPath      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iStopToShotTime" , CmnOptn.iStopToShotTime  );
}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop   " , CmnOptn.bLoadingStop     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor      " , CmnOptn.bIgnrDoor        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDisps     " , CmnOptn.bIgnrDisps       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bEmptyStrpCnt  " , CmnOptn.bEmptyStrpCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDsprSnsr  " , CmnOptn.bIgnrDsprSnsr    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDsprExpr  " , CmnOptn.bIgnrDsprExpr    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEmptyStrpCnt  " , CmnOptn.iEmptyStrpCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iExpiryDay     " , CmnOptn.iExpiryDay       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "LQSupplyDay    " , CmnOptn.LQSupplyDay.Val  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iDisprCleanCnt " , CmnOptn.iDisprCleanCnt   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sServerPath    " , CmnOptn.sServerPath      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iStopToShotTime" , CmnOptn.iStopToShotTime  );
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
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDryRun         " , MstOptn.bDryRun         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedSingleDisps" , MstOptn.bUsedSingleDisps);
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedDispsOptn  " , MstOptn.bUsedDispsOptn  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iDispsOptn      " , MstOptn.iDispsOptn      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bSkipOvldSnsr   " , MstOptn.bSkipOvldSnsr   );


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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDryRun         " , MstOptn.bDryRun         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedSingleDisps" , MstOptn.bUsedSingleDisps);
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedDispsOptn  " , MstOptn.bUsedDispsOptn  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iDispsOptn      " , MstOptn.iDispsOptn      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bSkipOvldSnsr   " , MstOptn.bSkipOvldSnsr   );

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRS_830A";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

    if(EqpOptn.sModelName =="") EqpOptn.sModelName = "HRS_830A";

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

