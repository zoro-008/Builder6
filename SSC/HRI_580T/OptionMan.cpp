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
    Trace("","");
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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch " , DevInfo.dColPitch  ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch " , DevInfo.dRowPitch  ); if(!DevInfo.dRowPitch )DevInfo.dRowPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt " , DevInfo.iColGrCnt  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt " , DevInfo.iRowGrCnt  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap " , DevInfo.dColGrGap  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap " , DevInfo.dRowGrGap  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt   " , DevInfo.iColCnt    ); if(!DevInfo.iColCnt   )DevInfo.iColCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt   " , DevInfo.iRowCnt    ); if(!DevInfo.iRowCnt   )DevInfo.iRowCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlCnt  " , DevInfo.iCsSlCnt   ); if(!DevInfo.iCsSlCnt  )DevInfo.iCsSlCnt   = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlPitch" , DevInfo.dCsSlPitch ); if(!DevInfo.dCsSlPitch)DevInfo.dCsSlPitch = 1 ;
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
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch " , DevInfo.dColPitch  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowPitch " , DevInfo.dRowPitch  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColGrCnt " , DevInfo.iColGrCnt  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowGrCnt " , DevInfo.iRowGrCnt  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColGrGap " , DevInfo.dColGrGap  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowGrGap " , DevInfo.dRowGrGap  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColCnt   " , DevInfo.iColCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowCnt   " , DevInfo.iRowCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iCsSlCnt  " , DevInfo.iCsSlCnt   );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlPitch" , DevInfo.dCsSlPitch );

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
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sVisnIndexId   " , DevOptn.sVisnIndexId    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPSB_XInsVel   " , DevOptn.dPSB_XInsVel    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iColInspCnt    " , DevOptn.iColInspCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRowInspCnt    " , DevOptn.iRowInspCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseUv         " , DevOptn.bUseUv          );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt  " , DevOptn.iLotEndMgzCnt   );
    /*
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt1   " , DevOptn.iSortItRslt1    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt2   " , DevOptn.iSortItRslt2    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt3   " , DevOptn.iSortItRslt3    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt4   " , DevOptn.iSortItRslt4    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt5   " , DevOptn.iSortItRslt5    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt6   " , DevOptn.iSortItRslt6    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt7   " , DevOptn.iSortItRslt7    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt8   " , DevOptn.iSortItRslt8    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt9   " , DevOptn.iSortItRslt9    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt10  " , DevOptn.iSortItRslt10   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt11  " , DevOptn.iSortItRslt11   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt12  " , DevOptn.iSortItRslt12   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt13  " , DevOptn.iSortItRslt13   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItRslt14  " , DevOptn.iSortItRslt14   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSortItFail    " , DevOptn.iSortItFail     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt1    " , DevOptn.iLevelRslt1     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt2    " , DevOptn.iLevelRslt2     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt3    " , DevOptn.iLevelRslt3     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt4    " , DevOptn.iLevelRslt4     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt5    " , DevOptn.iLevelRslt5     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt6    " , DevOptn.iLevelRslt6     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt7    " , DevOptn.iLevelRslt7     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt8    " , DevOptn.iLevelRslt8     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt9    " , DevOptn.iLevelRslt9     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt10   " , DevOptn.iLevelRslt10    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt11   " , DevOptn.iLevelRslt11    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt12   " , DevOptn.iLevelRslt12    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt13   " , DevOptn.iLevelRslt13    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt14   " , DevOptn.iLevelRslt14    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelFail     " , DevOptn.iLevelFail      );
    */

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPRBStprDnDelay" , DevOptn.iPRBStprDnDelay );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWK1StprDnDelay" , DevOptn.iWK1StprDnDelay );




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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sVisnIndexId   " , DevOptn.sVisnIndexId    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPSB_XInsVel   " , DevOptn.dPSB_XInsVel    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iColInspCnt    " , DevOptn.iColInspCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRowInspCnt    " , DevOptn.iRowInspCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseUv         " , DevOptn.bUseUv          );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt  " , DevOptn.iLotEndMgzCnt   );

    /*
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt1   " , DevOptn.iSortItRslt1    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt2   " , DevOptn.iSortItRslt2    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt3   " , DevOptn.iSortItRslt3    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt4   " , DevOptn.iSortItRslt4    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt5   " , DevOptn.iSortItRslt5    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt6   " , DevOptn.iSortItRslt6    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt7   " , DevOptn.iSortItRslt7    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt8   " , DevOptn.iSortItRslt8    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt9   " , DevOptn.iSortItRslt9    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt10  " , DevOptn.iSortItRslt10   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt11  " , DevOptn.iSortItRslt11   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt12  " , DevOptn.iSortItRslt12   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt13  " , DevOptn.iSortItRslt13   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItRslt14  " , DevOptn.iSortItRslt14   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSortItFail    " , DevOptn.iSortItFail     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt1    " , DevOptn.iLevelRslt1     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt2    " , DevOptn.iLevelRslt2     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt3    " , DevOptn.iLevelRslt3     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt4    " , DevOptn.iLevelRslt4     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt5    " , DevOptn.iLevelRslt5     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt6    " , DevOptn.iLevelRslt6     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt7    " , DevOptn.iLevelRslt7     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt8    " , DevOptn.iLevelRslt8     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt9    " , DevOptn.iLevelRslt9     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt10   " , DevOptn.iLevelRslt10    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt11   " , DevOptn.iLevelRslt11    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt12   " , DevOptn.iLevelRslt12    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt13   " , DevOptn.iLevelRslt13    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt14   " , DevOptn.iLevelRslt14    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelFail     " , DevOptn.iLevelFail      );
    */

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPRBStprDnDelay" , DevOptn.iPRBStprDnDelay );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWK1StprDnDelay" , DevOptn.iWK1StprDnDelay );
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load.
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bDryRun        " , CmnOptn.bDryRun       );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bNoTrimFail    " , CmnOptn.bNoTrimFail   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bIgnrDoor      " , CmnOptn.bIgnrDoor     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bVisn1Skip     " , CmnOptn.bVisn1Skip    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bVisn2Skip     " , CmnOptn.bVisn2Skip    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bSortByItem    " , CmnOptn.bSortByItem   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bAutoVsChange  " , CmnOptn.bAutoVsChange );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bAutoOperation " , CmnOptn.bAutoOperation);
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bCheckFailCnt  " , CmnOptn.bCheckFailCnt );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iCheckFailCnt  " , CmnOptn.iCheckFailCnt );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bCheckSFailCnt " , CmnOptn.bCheckSFailCnt);
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iCheckSFailCnt " , CmnOptn.iCheckSFailCnt);
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSrtMotrDelay  " , CmnOptn.iSrtMotrDelay );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bTrimCheck     " , CmnOptn.bTrimCheck    );

    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt1   " , CmnOptn.iSortItRslt1    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt2   " , CmnOptn.iSortItRslt2    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt3   " , CmnOptn.iSortItRslt3    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt4   " , CmnOptn.iSortItRslt4    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt5   " , CmnOptn.iSortItRslt5    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt6   " , CmnOptn.iSortItRslt6    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt7   " , CmnOptn.iSortItRslt7    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt8   " , CmnOptn.iSortItRslt8    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt9   " , CmnOptn.iSortItRslt9    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt10  " , CmnOptn.iSortItRslt10   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt11  " , CmnOptn.iSortItRslt11   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt12  " , CmnOptn.iSortItRslt12   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt13  " , CmnOptn.iSortItRslt13   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItRslt14  " , CmnOptn.iSortItRslt14   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iSortItFail    " , CmnOptn.iSortItFail     );

    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt1    " , CmnOptn.iLevelRslt1     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt2    " , CmnOptn.iLevelRslt2     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt3    " , CmnOptn.iLevelRslt3     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt4    " , CmnOptn.iLevelRslt4     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt5    " , CmnOptn.iLevelRslt5     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt6    " , CmnOptn.iLevelRslt6     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt7    " , CmnOptn.iLevelRslt7     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt8    " , CmnOptn.iLevelRslt8     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt9    " , CmnOptn.iLevelRslt9     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt10   " , CmnOptn.iLevelRslt10    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt11   " , CmnOptn.iLevelRslt11    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt12   " , CmnOptn.iLevelRslt12    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt13   " , CmnOptn.iLevelRslt13    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelRslt14   " , CmnOptn.iLevelRslt14    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "iLevelFail     " , CmnOptn.iLevelFail      );

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
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bDryRun        " , CmnOptn.bDryRun       );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bNoTrimFail    " , CmnOptn.bNoTrimFail   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bIgnrDoor      " , CmnOptn.bIgnrDoor     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bVisn1Skip     " , CmnOptn.bVisn1Skip    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bVisn2Skip     " , CmnOptn.bVisn2Skip    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bSortByItem    " , CmnOptn.bSortByItem   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bAutoVsChange  " , CmnOptn.bAutoVsChange );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bAutoOperation " , CmnOptn.bAutoOperation);
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bCheckFailCnt  " , CmnOptn.bCheckFailCnt );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iCheckFailCnt  " , CmnOptn.iCheckFailCnt );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bCheckSFailCnt " , CmnOptn.bCheckSFailCnt);
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iCheckSFailCnt " , CmnOptn.iCheckSFailCnt);
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSrtMotrDelay  " , CmnOptn.iSrtMotrDelay );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bTrimCheck     " , CmnOptn.bTrimCheck    );

    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt1   " , CmnOptn.iSortItRslt1    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt2   " , CmnOptn.iSortItRslt2    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt3   " , CmnOptn.iSortItRslt3    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt4   " , CmnOptn.iSortItRslt4    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt5   " , CmnOptn.iSortItRslt5    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt6   " , CmnOptn.iSortItRslt6    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt7   " , CmnOptn.iSortItRslt7    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt8   " , CmnOptn.iSortItRslt8    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt9   " , CmnOptn.iSortItRslt9    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt10  " , CmnOptn.iSortItRslt10   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt11  " , CmnOptn.iSortItRslt11   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt12  " , CmnOptn.iSortItRslt12   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt13  " , CmnOptn.iSortItRslt13   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItRslt14  " , CmnOptn.iSortItRslt14   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iSortItFail    " , CmnOptn.iSortItFail     );

    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt1    " , CmnOptn.iLevelRslt1     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt2    " , CmnOptn.iLevelRslt2     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt3    " , CmnOptn.iLevelRslt3     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt4    " , CmnOptn.iLevelRslt4     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt5    " , CmnOptn.iLevelRslt5     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt6    " , CmnOptn.iLevelRslt6     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt7    " , CmnOptn.iLevelRslt7     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt8    " , CmnOptn.iLevelRslt8     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt9    " , CmnOptn.iLevelRslt9     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt10   " , CmnOptn.iLevelRslt10    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt11   " , CmnOptn.iLevelRslt11    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt12   " , CmnOptn.iLevelRslt12    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt13   " , CmnOptn.iLevelRslt13    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelRslt14   " , CmnOptn.iLevelRslt14    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "iLevelFail     " , CmnOptn.iLevelFail      );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load.
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bDebugMode        " , MstOptn.bDebugMode        );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "sPreEquipPath     " , MstOptn.sPreEquipPath     );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "sVisnPath         " , MstOptn.sVisnPath         );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "sPstEquipDrv      " , MstOptn.sPstEquipDrv      );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "sPstEquipIp       " , MstOptn.sPstEquipIp       );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "sPreEquipPath     " , MstOptn.sPreEquipPath     );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bBarcodeOnly      " , MstOptn.bBarcodeOnly      );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bUld_YHome        " , MstOptn.bUld_YHome        );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bPSB_XYTrmHome    " , MstOptn.bPSB_XYTrmHome    );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bLdr_YHome        " , MstOptn.bLdr_YHome        );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bEmptyNoTrim      " , MstOptn.bEmptyNoTrim      );
    UserINI.Load(sPath.c_str(), "MstOptn"  , "bUseHangul        " , MstOptn.bUseHangul        );

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
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bDebugMode        " , MstOptn.bDebugMode         );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "sPreEquipPath     " , MstOptn.sPreEquipPath      );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "sVisnPath         " , MstOptn.sVisnPath          );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "sPstEquipDrv      " , MstOptn.sPstEquipDrv       );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "sPstEquipIp       " , MstOptn.sPstEquipIp        );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "sPstEquipPath     " , MstOptn.sPstEquipPath      );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bBarcodeOnly      " , MstOptn.bBarcodeOnly       );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bUld_YHome        " , MstOptn.bUld_YHome         );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bPSB_XYTrmHome    " , MstOptn.bPSB_XYTrmHome     );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bLdr_YHome        " , MstOptn.bLdr_YHome         );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bEmptyNoTrim      " , MstOptn.bEmptyNoTrim       );
    UserINI.Save(sPath.c_str(), "MstOptn"  , "bUseHangul        " , MstOptn.bUseHangul         );

}
void COptionMan::LoadEqpOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRI-580T";
    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    //if(!FileExists(sPath)) {
    //    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " , sModelName );
    //    UserINI.Save(sPath, "EqpOptn"  , "bExistLoader  " , "0"        );
    //    UserINI.Save(sPath, "EqpOptn"  , "bExistUnLoader" , "1"        );
    //}

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );
    UserINI.Load(sPath, "EqpOptn"  , "bExistLoader  " , EqpOptn.bExistLoader   );
    UserINI.Load(sPath, "EqpOptn"  , "bExistUnLoader" , EqpOptn.bExistUnLoader );
}
void COptionMan::SaveEqpOptn() //≥Õ æµ¿œ¿Ã æ¯±∏≥∂...
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


