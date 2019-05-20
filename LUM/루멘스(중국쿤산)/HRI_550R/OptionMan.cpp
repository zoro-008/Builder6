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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch       " , DevInfo.dColPitch       ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch       " , DevInfo.dRowPitch       ); if(!DevInfo.dRowPitch )DevInfo.dRowPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt       " , DevInfo.iColGrCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt       " , DevInfo.iRowGrCnt       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap       " , DevInfo.dColGrGap       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap       " , DevInfo.dRowGrGap       );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt         " , DevInfo.iColCnt         ); if(!DevInfo.iColCnt   )DevInfo.iColCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt         " , DevInfo.iRowCnt         ); if(!DevInfo.iRowCnt   )DevInfo.iRowCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlCnt        " , DevInfo.iCsSlCnt        ); if(!DevInfo.iCsSlCnt  )DevInfo.iCsSlCnt   = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlPitch      " , DevInfo.dCsSlPitch      ); if(!DevInfo.dCsSlPitch)DevInfo.dCsSlPitch = 1 ;
    //Vision
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sVisnIndexId    " , DevInfo.sVisnIndexId    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsColCnt      " , DevInfo.iColInspCnt     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsRowCnt      " , DevInfo.iRowInspCnt     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColInspMrkCnt  " , DevInfo.iColInspMrkCnt  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowInspMrkCnt  " , DevInfo.iRowInspMrkCnt  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsMrkVsColCnt " , DevInfo.iInsMrkVsColCnt );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iInsMrkVsRowCnt " , DevInfo.iInsMrkVsRowCnt );


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
    //Vision
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sVisnIndexId    " , DevInfo.sVisnIndexId    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsColCnt      " , DevInfo.iColInspCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsRowCnt      " , DevInfo.iRowInspCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColInspMrkCnt  " , DevInfo.iColInspMrkCnt  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowInspMrkCnt  " , DevInfo.iRowInspMrkCnt  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsMrkVsColCnt " , DevInfo.iInsMrkVsColCnt );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iInsMrkVsRowCnt " , DevInfo.iInsMrkVsRowCnt );

}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt            " , DevOptn.iLotEndMgzCnt           );
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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt            " , DevOptn.iLotEndMgzCnt           );
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
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop        " , CmnOptn.bLoadingStop        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs1Skip            " , CmnOptn.bVs1Skip            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs2Skip            " , CmnOptn.bVs2Skip            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs3Skip            " , CmnOptn.bVs3Skip            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVsMrkSkip          " , CmnOptn.bVsMrkSkip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoorSkip           " , CmnOptn.bDoorSkip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIdleRun            " , CmnOptn.bIdleRun            );
//    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLotEndMgzCnt       " , CmnOptn.iLotEndMgzCnt       );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt1         " , CmnOptn.iLevelRslt1         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt2         " , CmnOptn.iLevelRslt2         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt3         " , CmnOptn.iLevelRslt3         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt4         " , CmnOptn.iLevelRslt4         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt5         " , CmnOptn.iLevelRslt5         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt6         " , CmnOptn.iLevelRslt6         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt7         " , CmnOptn.iLevelRslt7         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt8         " , CmnOptn.iLevelRslt8         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt9         " , CmnOptn.iLevelRslt9         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt10        " , CmnOptn.iLevelRslt10        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt11        " , CmnOptn.iLevelRslt11        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt12        " , CmnOptn.iLevelRslt12        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt13        " , CmnOptn.iLevelRslt13        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelRslt14        " , CmnOptn.iLevelRslt14        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLevelFail          " , CmnOptn.iLevelFail          );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckFailCnt       " , CmnOptn.iCheckFailCnt       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR1FailCnt     " , CmnOptn.iCheckR1FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR2FailCnt     " , CmnOptn.iCheckR2FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR3FailCnt     " , CmnOptn.iCheckR3FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR4FailCnt     " , CmnOptn.iCheckR4FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR5FailCnt     " , CmnOptn.iCheckR5FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR6FailCnt     " , CmnOptn.iCheckR6FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR7FailCnt     " , CmnOptn.iCheckR7FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR8FailCnt     " , CmnOptn.iCheckR8FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR9FailCnt     " , CmnOptn.iCheckR9FailCnt     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR10FailCnt    " , CmnOptn.iCheckR10FailCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR11FailCnt    " , CmnOptn.iCheckR11FailCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR12FailCnt    " , CmnOptn.iCheckR12FailCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR13FailCnt    " , CmnOptn.iCheckR13FailCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckR14FailCnt    " , CmnOptn.iCheckR14FailCnt    );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR1FailName    " , CmnOptn.sCheckR1FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR2FailName    " , CmnOptn.sCheckR2FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR3FailName    " , CmnOptn.sCheckR3FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR4FailName    " , CmnOptn.sCheckR4FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR5FailName    " , CmnOptn.sCheckR5FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR6FailName    " , CmnOptn.sCheckR6FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR7FailName    " , CmnOptn.sCheckR7FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR8FailName    " , CmnOptn.sCheckR8FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR9FailName    " , CmnOptn.sCheckR9FailName    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR10FailName   " , CmnOptn.sCheckR10FailName   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR11FailName   " , CmnOptn.sCheckR11FailName   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR12FailName   " , CmnOptn.sCheckR12FailName   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR13FailName   " , CmnOptn.sCheckR13FailName   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sCheckR14FailName   " , CmnOptn.sCheckR14FailName   );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR1FailSkip         " , CmnOptn.bR1FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR2FailSkip         " , CmnOptn.bR2FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR3FailSkip         " , CmnOptn.bR3FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR4FailSkip         " , CmnOptn.bR4FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR5FailSkip         " , CmnOptn.bR5FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR6FailSkip         " , CmnOptn.bR6FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR7FailSkip         " , CmnOptn.bR7FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR8FailSkip         " , CmnOptn.bR8FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR9FailSkip         " , CmnOptn.bR9FailSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR10FailSkip        " , CmnOptn.bR10FailSkip        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR11FailSkip        " , CmnOptn.bR11FailSkip        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR12FailSkip        " , CmnOptn.bR12FailSkip        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR13FailSkip        " , CmnOptn.bR13FailSkip        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bR14FailSkip        " , CmnOptn.bR14FailSkip        );

//UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWK1StprDnDelay     " , CmnOptn.iWK1StprDnDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWK2StprDnDelay     " , CmnOptn.iWK2StprDnDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dWK1_XInsVel        " , CmnOptn.dWK1_XInsVel        );
//    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWK3StprDnDelay     " , CmnOptn.iWK3StprDnDelay     );
//    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPRBStprDnDelay     " , CmnOptn.iPRBStprDnDelay     );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dWK1InsStprPsSpeed  " , CmnOptn.dWK1InsStprPsSpeed  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoVsChange       " , CmnOptn.bAutoVsChange       );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPrbToWk1Delay      " , CmnOptn.iPrbToWk1Delay      ); 
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPsbToOutDelay      " , CmnOptn.iPsbToOutDelay      );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bEmptyNoMrk         " , CmnOptn.bEmptyNoMrk        );

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
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDryRun       " , CmnOptn.bDryRun       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bMrk1Skip     " , CmnOptn.bMrk1Skip     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bMrkSam1Skip  " , CmnOptn.bMrkSam1Skip  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bMrkAllSkip   " , CmnOptn.bMrkAllSkip   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrVsErr    " , CmnOptn.bIgnrVsErr    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVsAllSkip    " , CmnOptn.bVsAllSkip    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop  " , CmnOptn.bLoadingStop  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs1Skip      " , CmnOptn.bVs1Skip      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs2Skip      " , CmnOptn.bVs2Skip      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs3Skip      " , CmnOptn.bVs3Skip      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVsMrkSkip    " , CmnOptn.bVsMrkSkip    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoorSkip     " , CmnOptn.bDoorSkip     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIdleRun      " , CmnOptn.bIdleRun      );
//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLotEndMgzCnt " , CmnOptn.iLotEndMgzCnt );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt1   " , CmnOptn.iLevelRslt1   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt2   " , CmnOptn.iLevelRslt2   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt3   " , CmnOptn.iLevelRslt3   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt4   " , CmnOptn.iLevelRslt4   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt5   " , CmnOptn.iLevelRslt5   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt6   " , CmnOptn.iLevelRslt6   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt7   " , CmnOptn.iLevelRslt7   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt8   " , CmnOptn.iLevelRslt8   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt9   " , CmnOptn.iLevelRslt9   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt10  " , CmnOptn.iLevelRslt10  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt11  " , CmnOptn.iLevelRslt11  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt12  " , CmnOptn.iLevelRslt12  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt13  " , CmnOptn.iLevelRslt13  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelRslt14  " , CmnOptn.iLevelRslt14  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLevelFail    " , CmnOptn.iLevelFail    );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR1FailName    " , CmnOptn.sCheckR1FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR2FailName    " , CmnOptn.sCheckR2FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR3FailName    " , CmnOptn.sCheckR3FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR4FailName    " , CmnOptn.sCheckR4FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR5FailName    " , CmnOptn.sCheckR5FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR6FailName    " , CmnOptn.sCheckR6FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR7FailName    " , CmnOptn.sCheckR7FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR8FailName    " , CmnOptn.sCheckR8FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR9FailName    " , CmnOptn.sCheckR9FailName    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR10FailName   " , CmnOptn.sCheckR10FailName   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR11FailName   " , CmnOptn.sCheckR11FailName   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR12FailName   " , CmnOptn.sCheckR12FailName   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR13FailName   " , CmnOptn.sCheckR13FailName   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sCheckR14FailName   " , CmnOptn.sCheckR14FailName   );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckFailCnt       " , CmnOptn.iCheckFailCnt       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR1FailCnt     " , CmnOptn.iCheckR1FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR2FailCnt     " , CmnOptn.iCheckR2FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR3FailCnt     " , CmnOptn.iCheckR3FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR4FailCnt     " , CmnOptn.iCheckR4FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR5FailCnt     " , CmnOptn.iCheckR5FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR6FailCnt     " , CmnOptn.iCheckR6FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR7FailCnt     " , CmnOptn.iCheckR7FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR8FailCnt     " , CmnOptn.iCheckR8FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR9FailCnt     " , CmnOptn.iCheckR9FailCnt     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR10FailCnt    " , CmnOptn.iCheckR10FailCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR11FailCnt    " , CmnOptn.iCheckR11FailCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR12FailCnt    " , CmnOptn.iCheckR12FailCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR13FailCnt    " , CmnOptn.iCheckR13FailCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckR14FailCnt    " , CmnOptn.iCheckR14FailCnt    );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR1FailSkip         " , CmnOptn.bR1FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR2FailSkip         " , CmnOptn.bR2FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR3FailSkip         " , CmnOptn.bR3FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR4FailSkip         " , CmnOptn.bR4FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR5FailSkip         " , CmnOptn.bR5FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR6FailSkip         " , CmnOptn.bR6FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR7FailSkip         " , CmnOptn.bR7FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR8FailSkip         " , CmnOptn.bR8FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR9FailSkip         " , CmnOptn.bR9FailSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR10FailSkip        " , CmnOptn.bR10FailSkip        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR11FailSkip        " , CmnOptn.bR11FailSkip        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR12FailSkip        " , CmnOptn.bR12FailSkip        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR13FailSkip        " , CmnOptn.bR13FailSkip        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bR14FailSkip        " , CmnOptn.bR14FailSkip        );

//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK1StprDnDelay     " , CmnOptn.iWK1StprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK2StprDnDelay     " , CmnOptn.iWK2StprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dWK1_XInsVel        " , CmnOptn.dWK1_XInsVel        );
//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK3StprDnDelay     " , CmnOptn.iWK3StprDnDelay     );
//    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPRBStprDnDelay     " , CmnOptn.iPRBStprDnDelay     );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dWK1InsStprPsSpeed  " , CmnOptn.dWK1InsStprPsSpeed  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoVsChange       " , CmnOptn.bAutoVsChange       );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPrbToWk1Delay      " , CmnOptn.iPrbToWk1Delay      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPsbToOutDelay      " , CmnOptn.iPsbToOutDelay      );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bEmptyNoMrk         " , CmnOptn.bEmptyNoMrk        );
    
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

