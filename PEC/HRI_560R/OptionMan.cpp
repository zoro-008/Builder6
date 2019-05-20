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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch       " , DevInfo.dColPitch       ); if(!DevInfo.dColPitch  )DevInfo.dColPitch   = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch       " , DevInfo.dRowPitch       ); if(!DevInfo.dRowPitch  )DevInfo.dRowPitch   = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt       " , DevInfo.iColGrCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt       " , DevInfo.iRowGrCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap       " , DevInfo.dColGrGap       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap       " , DevInfo.dRowGrGap       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt         " , DevInfo.iColCnt         ); if(!DevInfo.iColCnt    )DevInfo.iColCnt     = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt         " , DevInfo.iRowCnt         ); if(!DevInfo.iRowCnt    )DevInfo.iRowCnt     = 1 ;
    //Vision
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sVisnIndexId    " , DevInfo.sVisnIndexId    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsColCnt      " , DevInfo.iColInspCnt     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsRowCnt      " , DevInfo.iRowInspCnt     );
    //UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColInspMrkCnt  " , DevInfo.iColInspMrkCnt  );
    //UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowInspMrkCnt  " , DevInfo.iRowInspMrkCnt  );
    //UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsMrkVsColCnt " , DevInfo.iInsMrkVsColCnt );
    //UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsMrkVsRowCnt " , DevInfo.iInsMrkVsRowCnt );

	UserINI.Load(sPath.c_str()  , "DevInfo"  , "iLDRColSlCnt   " , DevInfo.iLDRColCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dLDRColSlPitch " , DevInfo.dLDRColPitch     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iLDRRowSlCnt   " , DevInfo.iLDRRowCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dLDRRowSlPitch " , DevInfo.dLDRRowPitch     );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iULDColSlCnt   " , DevInfo.iULDColCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dULDColSlPitch " , DevInfo.dULDColPitch     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iULDRowSlCnt   " , DevInfo.iULDRowCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dULDRowSlPitch " , DevInfo.dULDRowPitch     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iULDPkgCnt     " , DevInfo.iULDPkgCnt       );
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
    //Vision
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sVisnIndexId    " , DevInfo.sVisnIndexId    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsColCnt      " , DevInfo.iColInspCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsRowCnt      " , DevInfo.iRowInspCnt     );
    //UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColInspMrkCnt  " , DevInfo.iColInspMrkCnt  );
    //UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowInspMrkCnt  " , DevInfo.iRowInspMrkCnt  );
    //UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsMrkVsColCnt " , DevInfo.iInsMrkVsColCnt );
    //UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsMrkVsRowCnt " , DevInfo.iInsMrkVsRowCnt );

	UserINI.Save(sPath.c_str()  , "DevInfo"  , "iLDRColSlCnt   " , DevInfo.iLDRColCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dLDRColSlPitch " , DevInfo.dLDRColPitch     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iLDRRowSlCnt   " , DevInfo.iLDRRowCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dLDRRowSlPitch " , DevInfo.dLDRRowPitch     );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iULDColSlCnt   " , DevInfo.iULDColCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dULDColSlPitch " , DevInfo.dULDColPitch     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iULDRowSlCnt   " , DevInfo.iULDRowCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dULDRowSlPitch " , DevInfo.dULDRowPitch     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iULDPkgCnt     " , DevInfo.iULDPkgCnt       );
}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt " , DevOptn.iLotEndMgzCnt    );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt1         " , DevOptn.iLevelRslt1         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt2         " , DevOptn.iLevelRslt2         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt3         " , DevOptn.iLevelRslt3         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt4         " , DevOptn.iLevelRslt4         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt5         " , DevOptn.iLevelRslt5         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt6         " , DevOptn.iLevelRslt6         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt7         " , DevOptn.iLevelRslt7         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt8         " , DevOptn.iLevelRslt8         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt9         " , DevOptn.iLevelRslt9         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt10        " , DevOptn.iLevelRslt10        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt11        " , DevOptn.iLevelRslt11        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt12        " , DevOptn.iLevelRslt12        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt13        " , DevOptn.iLevelRslt13        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelRslt14        " , DevOptn.iLevelRslt14        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLevelFail          " , DevOptn.iLevelFail          );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR1FailName    " , DevOptn.sCheckR1FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR2FailName    " , DevOptn.sCheckR2FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR3FailName    " , DevOptn.sCheckR3FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR4FailName    " , DevOptn.sCheckR4FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR5FailName    " , DevOptn.sCheckR5FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR6FailName    " , DevOptn.sCheckR6FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR7FailName    " , DevOptn.sCheckR7FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR8FailName    " , DevOptn.sCheckR8FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR9FailName    " , DevOptn.sCheckR9FailName    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR10FailName   " , DevOptn.sCheckR10FailName   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR11FailName   " , DevOptn.sCheckR11FailName   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR12FailName   " , DevOptn.sCheckR12FailName   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR13FailName   " , DevOptn.sCheckR13FailName   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "sCheckR14FailName   " , DevOptn.sCheckR14FailName   );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckFailCnt       " , DevOptn.iCheckFailCnt       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR1FailCnt     " , DevOptn.iCheckR1FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR2FailCnt     " , DevOptn.iCheckR2FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR3FailCnt     " , DevOptn.iCheckR3FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR4FailCnt     " , DevOptn.iCheckR4FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR5FailCnt     " , DevOptn.iCheckR5FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR6FailCnt     " , DevOptn.iCheckR6FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR7FailCnt     " , DevOptn.iCheckR7FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR8FailCnt     " , DevOptn.iCheckR8FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR9FailCnt     " , DevOptn.iCheckR9FailCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR10FailCnt    " , DevOptn.iCheckR10FailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR11FailCnt    " , DevOptn.iCheckR11FailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR12FailCnt    " , DevOptn.iCheckR12FailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR13FailCnt    " , DevOptn.iCheckR13FailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCheckR14FailCnt    " , DevOptn.iCheckR14FailCnt    );
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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt " , DevOptn.iLotEndMgzCnt    );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt1         " , DevOptn.iLevelRslt1         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt2         " , DevOptn.iLevelRslt2         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt3         " , DevOptn.iLevelRslt3         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt4         " , DevOptn.iLevelRslt4         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt5         " , DevOptn.iLevelRslt5         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt6         " , DevOptn.iLevelRslt6         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt7         " , DevOptn.iLevelRslt7         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt8         " , DevOptn.iLevelRslt8         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt9         " , DevOptn.iLevelRslt9         );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt10        " , DevOptn.iLevelRslt10        );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt11        " , DevOptn.iLevelRslt11        );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt12        " , DevOptn.iLevelRslt12        );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt13        " , DevOptn.iLevelRslt13        );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelRslt14        " , DevOptn.iLevelRslt14        );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLevelFail          " , DevOptn.iLevelFail          );  

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR1FailName    " , DevOptn.sCheckR1FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR2FailName    " , DevOptn.sCheckR2FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR3FailName    " , DevOptn.sCheckR3FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR4FailName    " , DevOptn.sCheckR4FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR5FailName    " , DevOptn.sCheckR5FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR6FailName    " , DevOptn.sCheckR6FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR7FailName    " , DevOptn.sCheckR7FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR8FailName    " , DevOptn.sCheckR8FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR9FailName    " , DevOptn.sCheckR9FailName    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR10FailName   " , DevOptn.sCheckR10FailName   );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR11FailName   " , DevOptn.sCheckR11FailName   );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR12FailName   " , DevOptn.sCheckR12FailName   );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR13FailName   " , DevOptn.sCheckR13FailName   );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "sCheckR14FailName   " , DevOptn.sCheckR14FailName   );  

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckFailCnt       " , DevOptn.iCheckFailCnt       );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR1FailCnt     " , DevOptn.iCheckR1FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR2FailCnt     " , DevOptn.iCheckR2FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR3FailCnt     " , DevOptn.iCheckR3FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR4FailCnt     " , DevOptn.iCheckR4FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR5FailCnt     " , DevOptn.iCheckR5FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR6FailCnt     " , DevOptn.iCheckR6FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR7FailCnt     " , DevOptn.iCheckR7FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR8FailCnt     " , DevOptn.iCheckR8FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR9FailCnt     " , DevOptn.iCheckR9FailCnt     );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR10FailCnt    " , DevOptn.iCheckR10FailCnt    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR11FailCnt    " , DevOptn.iCheckR11FailCnt    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR12FailCnt    " , DevOptn.iCheckR12FailCnt    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR13FailCnt    " , DevOptn.iCheckR13FailCnt    );  
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCheckR14FailCnt    " , DevOptn.iCheckR14FailCnt    );  
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDryRun             " , CmnOptn.bDryRun             );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bMrk1Skip           " , CmnOptn.bMrk1Skip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bMrkSam1Skip        " , CmnOptn.bMrkSam1Skip        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bMrkAllSkip         " , CmnOptn.bMrkAllSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrVsErr          " , CmnOptn.bIgnrVsErr          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVsAllSkip          " , CmnOptn.bVsAllSkip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs1Skip            " , CmnOptn.bVs1Skip            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs2Skip            " , CmnOptn.bVs2Skip            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs3Skip            " , CmnOptn.bVs3Skip            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVsMrkSkip          " , CmnOptn.bVsMrkSkip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoorSkip           " , CmnOptn.bDoorSkip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIdleRun            " , CmnOptn.bIdleRun            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bCheckFailCnt       " , CmnOptn.bCheckFailCnt       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAfterMrkRemove     " , CmnOptn.bAfterMrkRemove     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoReview         " , CmnOptn.bAutoReview         );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWK2StprDnDelay     " , CmnOptn.iWK2StprDnDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dWK1_XInsVel        " , CmnOptn.dWK1_XInsVel        );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dWK1InsStprPsSpeed  " , CmnOptn.dWK1InsStprPsSpeed  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoVsChange       " , CmnOptn.bAutoVsChange       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPrbToWk1Delay      " , CmnOptn.iPrbToWk1Delay      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPsbToOutDelay      " , CmnOptn.iPsbToOutDelay      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iULDMgzDelay        " , CmnOptn.iULDMgzDelay        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dPSBRejectDelay     " , CmnOptn.dPSBRejectDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dULDVaccumDeley     " , CmnOptn.dULDVaccumDeley     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dLDRInDeley         " , CmnOptn.dLDRInDeley         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsePenMrkMvX       " , CmnOptn.bUsePenMrkMvX       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsePenMrkMvY       " , CmnOptn.bUsePenMrkMvY       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dPenMrkMvX          " , CmnOptn.dPenMrkMvX          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dPenMrkMvY          " , CmnOptn.dPenMrkMvY          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLDREmptyCnt        " , CmnOptn.iLDREmptyCnt        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseLDREmpty        " , CmnOptn.bUseLDREmpty        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dPSB_VisionTime     " , CmnOptn.dPSB_VisionTime     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iUldPkgEjtTime      " , CmnOptn.iUldPkgEjtTime      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iUldPprEjtTime      " , CmnOptn.iUldPprEjtTime      );



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
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDryRun             " , CmnOptn.bDryRun             );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bMrk1Skip           " , CmnOptn.bMrk1Skip           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bMrkSam1Skip        " , CmnOptn.bMrkSam1Skip        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bMrkAllSkip         " , CmnOptn.bMrkAllSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrVsErr          " , CmnOptn.bIgnrVsErr          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVsAllSkip          " , CmnOptn.bVsAllSkip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs1Skip            " , CmnOptn.bVs1Skip            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs2Skip            " , CmnOptn.bVs2Skip            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs3Skip            " , CmnOptn.bVs3Skip            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVsMrkSkip          " , CmnOptn.bVsMrkSkip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoorSkip           " , CmnOptn.bDoorSkip           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIdleRun            " , CmnOptn.bIdleRun            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bCheckFailCnt       " , CmnOptn.bCheckFailCnt       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAfterMrkRemove     " , CmnOptn.bAfterMrkRemove     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoReview         " , CmnOptn.bAutoReview         );




//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK1StprDnDelay     " , CmnOptn.iWK1StprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK2StprDnDelay     " , CmnOptn.iWK2StprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dWK1_XInsVel        " , CmnOptn.dWK1_XInsVel        );
//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK3StprDnDelay     " , CmnOptn.iWK3StprDnDelay     );
//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPRBStprDnDelay     " , CmnOptn.iPRBStprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dWK1InsStprPsSpeed  " , CmnOptn.dWK1InsStprPsSpeed  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoVsChange       " , CmnOptn.bAutoVsChange       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPrbToWk1Delay      " , CmnOptn.iPrbToWk1Delay      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPsbToOutDelay      " , CmnOptn.iPsbToOutDelay      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iULDMgzDelay        " , CmnOptn.iULDMgzDelay        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPaperCheckCnt      " , CmnOptn.iPaperCheckCnt      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dPSBRejectDelay     " , CmnOptn.dPSBRejectDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dULDVaccumDeley     " , CmnOptn.dULDVaccumDeley     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dLDRInDeley         " , CmnOptn.dLDRInDeley         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsePenMrkMvX       " , CmnOptn.bUsePenMrkMvX       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsePenMrkMvY       " , CmnOptn.bUsePenMrkMvY       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dPenMrkMvX          " , CmnOptn.dPenMrkMvX          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dPenMrkMvY          " , CmnOptn.dPenMrkMvY          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLDREmptyCnt        " , CmnOptn.iLDREmptyCnt        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseLDREmpty        " , CmnOptn.bUseLDREmpty        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dPSB_VisionTime     " , CmnOptn.dPSB_VisionTime     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iUldPkgEjtTime      " , CmnOptn.iUldPkgEjtTime      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iUldPprEjtTime      " , CmnOptn.iUldPprEjtTime      );


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
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUld_YHome    " , MstOptn.bUld_YHome    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bLdr_YHome    " , MstOptn.bLdr_YHome    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDataAccEng   " , MstOptn.bDataAccEng   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnPath     " , MstOptn.sVisnPath     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dTrigerOffset " , MstOptn.dTrigerOffset );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bPsbVsErrIgnr " , MstOptn.bPsbVsErrIgnr );

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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUld_YHome    " , MstOptn.bUld_YHome    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bLdr_YHome    " , MstOptn.bLdr_YHome    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDataAccEng   " , MstOptn.bDataAccEng   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnPath     " , MstOptn.sVisnPath     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dTrigerOffset " , MstOptn.dTrigerOffset );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bPsbVsErrIgnr " , MstOptn.bPsbVsErrIgnr );

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
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " ,  EqpOptn.sModelName     );
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
    UserINI.Load(sPath, "TOTAL CHIP        " , "iTotalChip    "  , iTotalChip     );
    UserINI.Load(sPath, "TOTAL FAIL CHIP   " , "iTotalFailChip"  , iTotalFailChip );
    UserINI.Load(sPath, "Lot End Form Delay" , "bLotFormDelay "  , bLotFormDelay );

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
    UserINI.Save(sPath, "TOTAL CHIP        " , "iTotalChip    "  , iTotalChip     );
    UserINI.Save(sPath, "TOTAL FAIL CHIP   " , "iTotalFailChip"  , iTotalFailChip );
    UserINI.Save(sPath, "Lot End Form Delay" , "bLotFormDelay "  , bLotFormDelay );

}

