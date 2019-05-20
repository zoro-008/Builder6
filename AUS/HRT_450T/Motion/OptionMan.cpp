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
}
//---------------------------------------------------------------------------
void COptionMan::Close()
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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch   " , DevInfo.dColPitch    ); if(!DevInfo.dColPitch   )DevInfo.dColPitch    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch   " , DevInfo.dRowPitch    ); if(!DevInfo.dRowPitch   )DevInfo.dRowPitch    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt   " , DevInfo.iColGrCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt   " , DevInfo.iRowGrCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap   " , DevInfo.dColGrGap    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap   " , DevInfo.dRowGrGap    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt     " , DevInfo.iColCnt      ); if(!DevInfo.iColCnt     )DevInfo.iColCnt      = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt     " , DevInfo.iRowCnt      ); if(!DevInfo.iRowCnt     )DevInfo.iRowCnt      = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlCnt    " , DevInfo.iCsSlCnt     ); if(!DevInfo.iCsSlCnt    )DevInfo.iCsSlCnt     = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlPitch  " , DevInfo.dCsSlPitch   ); if(!DevInfo.dCsSlPitch  )DevInfo.dCsSlPitch   = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowSubGrGap" , DevInfo.dRowSubGrGap );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowSubCnt  " , DevInfo.iRowSubGrCnt ); if(!DevInfo.iRowSubGrCnt)DevInfo.iRowSubGrCnt = 1 ;



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
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch    " , DevInfo.dColPitch     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowPitch    " , DevInfo.dRowPitch     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColGrCnt    " , DevInfo.iColGrCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowGrCnt    " , DevInfo.iRowGrCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColGrGap    " , DevInfo.dColGrGap     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowGrGap    " , DevInfo.dRowGrGap     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColCnt      " , DevInfo.iColCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowCnt      " , DevInfo.iRowCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iCsSlCnt     " , DevInfo.iCsSlCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlPitch   " , DevInfo.dCsSlPitch    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowSubGrGap " , DevInfo.dRowSubGrGap  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowSubCnt   " , DevInfo.iRowSubGrCnt  );

}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPinCnt            " , &DevOptn.iPinCnt           );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLimInsFailCnt     " , &DevOptn.iLimInsFailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iReInspCnt         " , &DevOptn.iReInspCnt        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLimAllFailCnt     " , &DevOptn.iLimAllFailCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iContColCnt        " , &DevOptn.iContColCnt       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPR1MoveCnt        " , &DevOptn.iPR1MoveCnt       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPR1OutOfs         " , &DevOptn.dPR1OutOfs        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWRKMoveCnt        " , &DevOptn.iWRKMoveCnt       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPR2MoveCnt        " , &DevOptn.iPR2MoveCnt       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPR2OutOfs         " , &DevOptn.dPR2OutOfs        );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseZener          " , &DevOptn.bUseZener         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1Ch1P             " , &DevOptn.i1Ch1P            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1Ch1N             " , &DevOptn.i1Ch1N            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1Ch2P             " , &DevOptn.i1Ch2P            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1Ch2N             " , &DevOptn.i1Ch2N            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1Ch3P             " , &DevOptn.i1Ch3P            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1Ch3N             " , &DevOptn.i1Ch3N            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1ChLedCur         " , &DevOptn.i1ChLedCur        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2ChLedCur         " , &DevOptn.i2ChLedCur        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1ChZenCur         " , &DevOptn.i1ChZenCur        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2ChZenCur         " , &DevOptn.i2ChZenCur        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2Ch1P             " , &DevOptn.i2Ch1P            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2Ch1N             " , &DevOptn.i2Ch1N            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2Ch2P             " , &DevOptn.i2Ch2P            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2Ch2N             " , &DevOptn.i2Ch2N            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2Ch3P             " , &DevOptn.i2Ch3P            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2Ch3N             " , &DevOptn.i2Ch3N            );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "d1ChLow            " , &DevOptn.d1ChLow           );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "d1ChHigh           " , &DevOptn.d1ChHigh          );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "d2ChLow            " , &DevOptn.d2ChLow           );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "d2ChHigh           " , &DevOptn.d2ChHigh          );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dZenerLow          " , &DevOptn.dZenerLow         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dZenerHigh         " , &DevOptn.dZenerHigh        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPrbSttDelay       " , &DevOptn.iPrbSttDelay      );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1ChDelay          " , &DevOptn.i1ChDelay         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1ChLedTime        " , &DevOptn.i1ChLedTime       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i1ChZenTime        " , &DevOptn.i1ChZenTime       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2ChDelay          " , &DevOptn.i2ChDelay         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2ChLedTime        " , &DevOptn.i2ChLedTime       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "i2ChZenTime        " , &DevOptn.i2ChZenTime       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bChRvsCrnt         " , &DevOptn.bChRvsCrnt        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTwiceInsp      " , &DevOptn.bUseTwiceInsp     );
    //Daegyoem Probe
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSwitchingDly      " , &DevOptn.iSwitchingDly     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDgChipCnt         " , &DevOptn.iDgChipCnt        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bSideLEDControl    " , &DevOptn.bSideLEDControl   );


    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPreRailOutVel     " , &DevOptn.dPreRailOutVel    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPostRailOutVel    " , &DevOptn.dPostRailOutVel   );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWaitDelay         " , &DevOptn.iWaitDelay        );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt      " , &DevOptn.iLotEndMgzCnt     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bLtHeaterNum2      " , &DevOptn.bLtHeaterNum2     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bLtHeaterNum3      " , &DevOptn.bLtHeaterNum3     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bRtHeaterNum2      " , &DevOptn.bRtHeaterNum2     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bRtHeaterNum3      " , &DevOptn.bRtHeaterNum3     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dREJPitch          " , &DevOptn.dREJPitch         );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dTrimPtchOfs       " , &DevOptn.dTrimPtchOfs      );
    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "dWorkPtchOfs       " , &DevOptn.dWorkPtchOfs      );


    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dWRKPtchOfs        " , &DevOptn.dWRKPtchOfs       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPSBPtchOfs        " , &DevOptn.dPSBPtchOfs       );

    //UserINI.Load(sPath.c_str()  , "DevOptn"  , "dTrimMoveOfs       " , &DevOptn.dTrimMoveOfs      );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPSBReGripOfs      " , &DevOptn.dPSBReGripOfs     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSBReGripCol      " , &DevOptn.iPSBReGripCol     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTrimRjtDly        " , &DevOptn.iTrimRjtDly       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTrimSnrDly        " , &DevOptn.iTrimSnrDly       );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iContactPinCnt     " , &DevOptn.iContactPinCnt    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLimContactPinCnt  " , &DevOptn.iLimContactPinCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPSBPshDeley       " , &DevOptn.dPSBPshDeley      );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dGripOfs           " , &DevOptn.dGripOfs          );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bSkipPinCnt        " , &DevOptn.bSkipPinCnt       );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRejectMoveDeley   " , &DevOptn.iRejectMoveDeley  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iFrtPrcFailCnt     " , &DevOptn.iFrtPrcFailCnt    );




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

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPinCnt            " , DevOptn.iPinCnt           );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLimInsFailCnt     " , DevOptn.iLimInsFailCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iReInspCnt         " , DevOptn.iReInspCnt        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLimAllFailCnt     " , DevOptn.iLimAllFailCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iContColCnt        " , DevOptn.iContColCnt       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPR1MoveCnt        " , DevOptn.iPR1MoveCnt       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPR1OutOfs         " , DevOptn.dPR1OutOfs        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWRKMoveCnt        " , DevOptn.iWRKMoveCnt       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPR2MoveCnt        " , DevOptn.iPR2MoveCnt       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPR2OutOfs         " , DevOptn.dPR2OutOfs        );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseZener          " , DevOptn.bUseZener         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1Ch1P             " , DevOptn.i1Ch1P            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1Ch1N             " , DevOptn.i1Ch1N            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1Ch2P             " , DevOptn.i1Ch2P            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1Ch2N             " , DevOptn.i1Ch2N            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1Ch3P             " , DevOptn.i1Ch3P            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1Ch3N             " , DevOptn.i1Ch3N            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1ChLedCur         " , DevOptn.i1ChLedCur        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1ChZenCur         " , DevOptn.i1ChZenCur        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2Ch1P             " , DevOptn.i2Ch1P            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2Ch1N             " , DevOptn.i2Ch1N            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2Ch2P             " , DevOptn.i2Ch2P            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2Ch2N             " , DevOptn.i2Ch2N            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2Ch3P             " , DevOptn.i2Ch3P            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2Ch3N             " , DevOptn.i2Ch3N            );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2ChLedCur         " , DevOptn.i2ChLedCur        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2ChZenCur         " , DevOptn.i2ChZenCur        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "d1ChLow            " , DevOptn.d1ChLow           );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "d1ChHigh           " , DevOptn.d1ChHigh          );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "d2ChLow            " , DevOptn.d2ChLow           );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "d2ChHigh           " , DevOptn.d2ChHigh          );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dZenerLow          " , DevOptn.dZenerLow         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dZenerHigh         " , DevOptn.dZenerHigh        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPrbSttDelay       " , DevOptn.iPrbSttDelay      );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1ChDelay          " , DevOptn.i1ChDelay         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1ChLedTime        " , DevOptn.i1ChLedTime       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i1ChZenTime        " , DevOptn.i1ChZenTime       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2ChDelay          " , DevOptn.i2ChDelay         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2ChLedTime        " , DevOptn.i2ChLedTime       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "i2ChZenTime        " , DevOptn.i2ChZenTime       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bChRvsCrnt         " , DevOptn.bChRvsCrnt        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTwiceInsp      " , DevOptn.bUseTwiceInsp     );
    //Daegyoem Probe
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSwitchingDly      " , DevOptn.iSwitchingDly     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDgChipCnt         " , DevOptn.iDgChipCnt        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bSideLEDControl    " , DevOptn.bSideLEDControl   );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPreRailOutVel     " , DevOptn.dPreRailOutVel    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPostRailOutVel    " , DevOptn.dPostRailOutVel   );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWaitDelay         " , DevOptn.iWaitDelay        );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt      " , DevOptn.iLotEndMgzCnt     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bLtHeaterNum2      " , DevOptn.bLtHeaterNum2     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bLtHeaterNum3      " , DevOptn.bLtHeaterNum3     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bRtHeaterNum2      " , DevOptn.bRtHeaterNum2     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bRtHeaterNum3      " , DevOptn.bRtHeaterNum3     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dREJPitch          " , DevOptn.dREJPitch         );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dTrimPtchOfs       " , DevOptn.dTrimPtchOfs      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dWRKPtchOfs        " , DevOptn.dWRKPtchOfs       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPSBPtchOfs        " , DevOptn.dPSBPtchOfs       );
    //UserINI.Save(sPath.c_str()  , "DevOptn"  , "dTrimMoveOfs       " , DevOptn.dTrimMoveOfs      );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPSBReGripOfs      " , DevOptn.dPSBReGripOfs     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSBReGripCol      " , DevOptn.iPSBReGripCol     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTrimRjtDly        " , DevOptn.iTrimRjtDly       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTrimSnrDly        " , DevOptn.iTrimSnrDly       );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iContactPinCnt     " , DevOptn.iContactPinCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLimContactPinCnt  " , DevOptn.iLimContactPinCnt );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPSBPshDeley       " , DevOptn.dPSBPshDeley      );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dGripOfs           " , DevOptn.dGripOfs          );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bSkipPinCnt        " , DevOptn.bSkipPinCnt       );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRejectMoveDeley   " , DevOptn.iRejectMoveDeley  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iFrtPrcFailCnt     " , DevOptn.iFrtPrcFailCnt    );



}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDryRun         " , &CmnOptn.bDryRun         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor       " , &CmnOptn.bIgnrDoor       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrIdCehck    " , &CmnOptn.bIgnrIdCehck    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrInsp       " , &CmnOptn.bIgnrInsp       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrHeatAlm    " , &CmnOptn.bIgnrHeatAlm    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrStrOverAlm " , &CmnOptn.bIgnrStrOverAlm );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop    " , &CmnOptn.bLoadingStop    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoOperation  " , &CmnOptn.bAutoOperation  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoConvesion  " , &CmnOptn.bAutoConvesion  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bCheckFailCnt   " , &CmnOptn.bCheckFailCnt   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckFailCnt   " , &CmnOptn.iCheckFailCnt   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrTrim       " , &CmnOptn.bIgnrTrim       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrVacuum     " , &CmnOptn.bIgnrVacuum     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSortSensorDelay" , &CmnOptn.bSortSensorDelay);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iSortSensorDelay" , &CmnOptn.iSortSensorDelay);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDecChk     " , &CmnOptn.bIgnrDecChk     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrTrimDecChk " , &CmnOptn.bIgnrTrimDecChk );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature1   " , &CmnOptn.iTemperature1   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature2   " , &CmnOptn.iTemperature2   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature3   " , &CmnOptn.iTemperature3   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature4   " , &CmnOptn.iTemperature4   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature5   " , &CmnOptn.iTemperature5   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature6   " , &CmnOptn.iTemperature6   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature7   " , &CmnOptn.iTemperature7   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTemperature8   " , &CmnOptn.iTemperature8   );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDrop       " , &CmnOptn.bIgnrDrop       );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedInsOptn    " , &CmnOptn.bUsedInsOptn    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedTopViewPnch" , &CmnOptn.bUsedTopViewPnch);

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipYieldErr   " , &CmnOptn.bSkipYieldErr   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dStandardYield  " , &CmnOptn.dStandardYield  );

    //Bin Option 때문에..SSC요청사항. 20150323.JS
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin1       " , &CmnOptn.bUsedBin1       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin2       " , &CmnOptn.bUsedBin2       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin3       " , &CmnOptn.bUsedBin3       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin4       " , &CmnOptn.bUsedBin4       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin5       " , &CmnOptn.bUsedBin5       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin6       " , &CmnOptn.bUsedBin6       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin7       " , &CmnOptn.bUsedBin7       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin8       " , &CmnOptn.bUsedBin8       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin9       " , &CmnOptn.bUsedBin9       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUsedBin10      " , &CmnOptn.bUsedBin10      );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf1         " , &CmnOptn.dMinVf1         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf1         " , &CmnOptn.dMaxVf1         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf2         " , &CmnOptn.dMinVf2         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf2         " , &CmnOptn.dMaxVf2         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf3         " , &CmnOptn.dMinVf3         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf3         " , &CmnOptn.dMaxVf3         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf4         " , &CmnOptn.dMinVf4         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf4         " , &CmnOptn.dMaxVf4         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf5         " , &CmnOptn.dMinVf5         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf5         " , &CmnOptn.dMaxVf5         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf6         " , &CmnOptn.dMinVf6         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf6         " , &CmnOptn.dMaxVf6         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf7         " , &CmnOptn.dMinVf7         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf7         " , &CmnOptn.dMaxVf7         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf8         " , &CmnOptn.dMinVf8         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf8         " , &CmnOptn.dMaxVf8         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf9         " , &CmnOptn.dMinVf9         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf9         " , &CmnOptn.dMaxVf9         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVf10        " , &CmnOptn.dMinVf10        );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVf10        " , &CmnOptn.dMaxVf10        );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr1         " , &CmnOptn.dMinVr1         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr1         " , &CmnOptn.dMaxVr1         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr2         " , &CmnOptn.dMinVr2         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr2         " , &CmnOptn.dMaxVr2         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr3         " , &CmnOptn.dMinVr3         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr3         " , &CmnOptn.dMaxVr3         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr4         " , &CmnOptn.dMinVr4         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr4         " , &CmnOptn.dMaxVr4         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr5         " , &CmnOptn.dMinVr5         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr5         " , &CmnOptn.dMaxVr5         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr6         " , &CmnOptn.dMinVr6         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr6         " , &CmnOptn.dMaxVr6         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr7         " , &CmnOptn.dMinVr7         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr7         " , &CmnOptn.dMaxVr7         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr8         " , &CmnOptn.dMinVr8         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr8         " , &CmnOptn.dMaxVr8         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr9         " , &CmnOptn.dMinVr9         );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr9         " , &CmnOptn.dMaxVr9         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMinVr10        " , &CmnOptn.dMinVr10        );  UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxVr10        " , &CmnOptn.dMaxVr10        );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iBinDefaultNo   " , &CmnOptn.iBinDefaultNo   );
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
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDryRun         " , CmnOptn.bDryRun         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor       " , CmnOptn.bIgnrDoor       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrIdCehck    " , CmnOptn.bIgnrIdCehck    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrInsp       " , CmnOptn.bIgnrInsp       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrHeatAlm    " , CmnOptn.bIgnrHeatAlm    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrStrOverAlm " , CmnOptn.bIgnrStrOverAlm );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop    " , CmnOptn.bLoadingStop    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoOperation  " , CmnOptn.bAutoOperation  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoConvesion  " , CmnOptn.bAutoConvesion  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bCheckFailCnt   " , CmnOptn.bCheckFailCnt   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckFailCnt   " , CmnOptn.iCheckFailCnt   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrTrim       " , CmnOptn.bIgnrTrim       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrVacuum     " , CmnOptn.bIgnrVacuum     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSortSensorDelay" , CmnOptn.bSortSensorDelay);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iSortSensorDelay" , CmnOptn.iSortSensorDelay);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDecChk     " , CmnOptn.bIgnrDecChk     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrTrimDecChk " , CmnOptn.bIgnrTrimDecChk );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature1   " , CmnOptn.iTemperature1   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature2   " , CmnOptn.iTemperature2   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature3   " , CmnOptn.iTemperature3   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature4   " , CmnOptn.iTemperature4   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature5   " , CmnOptn.iTemperature5   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature6   " , CmnOptn.iTemperature6   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature7   " , CmnOptn.iTemperature7   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTemperature8   " , CmnOptn.iTemperature8   );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDrop       " , CmnOptn.bIgnrDrop       );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedInsOptn    " , CmnOptn.bUsedInsOptn    );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedTopViewPnch" , CmnOptn.bUsedTopViewPnch);

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipYieldErr   " , &CmnOptn.bSkipYieldErr   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dStandardYield  " , &CmnOptn.dStandardYield  );

    //Bin Option 때문에..SSC요청사항. 20150323.JS
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin1       " , CmnOptn.bUsedBin1       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin2       " , CmnOptn.bUsedBin2       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin3       " , CmnOptn.bUsedBin3       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin4       " , CmnOptn.bUsedBin4       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin5       " , CmnOptn.bUsedBin5       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin6       " , CmnOptn.bUsedBin6       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin7       " , CmnOptn.bUsedBin7       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin8       " , CmnOptn.bUsedBin8       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin9       " , CmnOptn.bUsedBin9       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUsedBin10      " , CmnOptn.bUsedBin10      );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf1         " , CmnOptn.dMinVf1         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf1         " , CmnOptn.dMaxVf1         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf2         " , CmnOptn.dMinVf2         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf2         " , CmnOptn.dMaxVf2         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf3         " , CmnOptn.dMinVf3         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf3         " , CmnOptn.dMaxVf3         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf4         " , CmnOptn.dMinVf4         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf4         " , CmnOptn.dMaxVf4         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf5         " , CmnOptn.dMinVf5         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf5         " , CmnOptn.dMaxVf5         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf6         " , CmnOptn.dMinVf6         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf6         " , CmnOptn.dMaxVf6         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf7         " , CmnOptn.dMinVf7         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf7         " , CmnOptn.dMaxVf7         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf8         " , CmnOptn.dMinVf8         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf8         " , CmnOptn.dMaxVf8         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf9         " , CmnOptn.dMinVf9         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf9         " , CmnOptn.dMaxVf9         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVf10        " , CmnOptn.dMinVf10        );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVf10        " , CmnOptn.dMaxVf10        );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr1         " , CmnOptn.dMinVr1         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr1         " , CmnOptn.dMaxVr1         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr2         " , CmnOptn.dMinVr2         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr2         " , CmnOptn.dMaxVr2         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr3         " , CmnOptn.dMinVr3         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr3         " , CmnOptn.dMaxVr3         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr4         " , CmnOptn.dMinVr4         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr4         " , CmnOptn.dMaxVr4         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr5         " , CmnOptn.dMinVr5         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr5         " , CmnOptn.dMaxVr5         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr6         " , CmnOptn.dMinVr6         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr6         " , CmnOptn.dMaxVr6         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr7         " , CmnOptn.dMinVr7         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr7         " , CmnOptn.dMaxVr7         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr8         " , CmnOptn.dMinVr8         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr8         " , CmnOptn.dMaxVr8         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr9         " , CmnOptn.dMinVr9         );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr9         " , CmnOptn.dMaxVr9         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMinVr10        " , CmnOptn.dMinVr10        );  UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxVr10        " , CmnOptn.dMaxVr10        );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iBinDefaultNo   " , CmnOptn.iBinDefaultNo   );

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode         " , &MstOptn.bDebugMode        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sPstEquipPath      " , &MstOptn.sPstEquipPath     );
//    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sPstEquipDrv       " , &MstOptn.sPstEquipDrv      );
//    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sPstEquipIp        " , &MstOptn.sPstEquipIp       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bHeattingTest      " , &MstOptn.bHeattingTest     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bNoLdUl            " , &MstOptn.bNoLdUl           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bGripTest          " , &MstOptn.bGripTest         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bPRBCmp            " , &MstOptn.bPRBCmp           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bBarcodeUseOnly    " , &MstOptn.bBarcodeUseOnly   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bByChInsp          " , &MstOptn.bByChInsp         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bLdr_YHome         " , &MstOptn.bLdr_YHome        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUseDaegyeomProbe  " , &MstOptn.bUseDaegyeomProbe );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bNotUstLot         " , &MstOptn.bNotUstLot        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bShortMonitor      " , &MstOptn.bShortMonitor     );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d1chLedOffset      " , &MstOptn.d1chLedOffset     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d1chLedGain        " , &MstOptn.d1chLedGain       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d1chLedSetOfs      " , &MstOptn.d1chLedSetOfs     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d1chZennerOffset   " , &MstOptn.d1chZennerOffset  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d1chZennerGain     " , &MstOptn.d1chZennerGain    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dichZennerSetOfs   " , &MstOptn.d1chZennerSetOfs  );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2chLedOffset      " , &MstOptn.d2chLedOffset     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2chLedGain        " , &MstOptn.d2chLedGain       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2chLedSetOfs      " , &MstOptn.d2chLedSetOfs     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2chZennerOffset   " , &MstOptn.d2chZennerOffset  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2chZennerGain     " , &MstOptn.d2chZennerGain    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2chZennerSetOfs   " , &MstOptn.d2chZennerSetOfs  );


    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iBarcodeNum        " , &MstOptn.iBarcodeNum       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDataAccEng        " , &MstOptn.bDataAccEng       );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dWRKCmpOfs         " , &MstOptn.dWRKCmpOfs        );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUseReContact      " , &MstOptn.bUseReContact     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iTempOfs           " , &MstOptn.iTempOfs          );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iMaxTemp           " , &MstOptn.iMaxTemp          );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iPreMaxTemp        " , &MstOptn.iPreMaxTemp       );


    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedVision        " , &MstOptn.bUsedVision       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedTempRS232     " , &MstOptn.bUsedTempRS232    );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedEngVersion    " , &MstOptn.bUsedEngVersion   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsed3040Pnch      " , &MstOptn.bUsed3040Pnch     );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedXAxis         " , &MstOptn.bUsedXAxis        );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedBinSorting    " , &MstOptn.bUsedBinSorting   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedChinaSPC      " , &MstOptn.bUsedChinaSPC     );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUsedSmallSizePkg  " , &MstOptn.bUsedSmallSizePkg );


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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode         " ,  MstOptn.bDebugMode        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sPstEquipPath      " ,  MstOptn.sPstEquipPath     );
//    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sPstEquipDrv       " ,  MstOptn.sPstEquipDrv      );
//    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sPstEquipIp        " ,  MstOptn.sPstEquipIp       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bHeattingTest      " ,  MstOptn.bHeattingTest     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bNoLdUl            " ,  MstOptn.bNoLdUl           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bGripTest          " ,  MstOptn.bGripTest         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bPRBCmp            " ,  MstOptn.bPRBCmp           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bBarcodeUseOnly    " ,  MstOptn.bBarcodeUseOnly   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bByChInsp          " ,  MstOptn.bByChInsp         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bLdr_YHome         " ,  MstOptn.bLdr_YHome        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUseDaegyeomProbe  " ,  MstOptn.bUseDaegyeomProbe );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bNotUstLot         " ,  MstOptn.bNotUstLot        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bShortMonitor      " ,  MstOptn.bShortMonitor     );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d1chLedOffset      " ,  MstOptn.d1chLedOffset     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d1chLedGain        " ,  MstOptn.d1chLedGain       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d1chLedSetOfs      " ,  MstOptn.d1chLedSetOfs     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d1chZennerOffset   " ,  MstOptn.d1chZennerOffset  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d1chZennerGain     " ,  MstOptn.d1chZennerGain    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dichZennerSetOfs   " ,  MstOptn.d1chZennerSetOfs  );
    
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2chLedOffset      " ,  MstOptn.d2chLedOffset     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2chLedGain        " ,  MstOptn.d2chLedGain       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2chLedSetOfs      " ,  MstOptn.d2chLedSetOfs     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2chZennerOffset   " ,  MstOptn.d2chZennerOffset  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2chZennerGain     " ,  MstOptn.d2chZennerGain    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2chZennerSetOfs   " ,  MstOptn.d2chZennerSetOfs  );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iBarcodeNum        " ,  MstOptn.iBarcodeNum       );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDataAccEng        " ,  MstOptn.bDataAccEng       );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dWRKCmpOfs         " ,  MstOptn.dWRKCmpOfs        );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUseReContact      " ,  MstOptn.bUseReContact     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iTempOfs           " ,  MstOptn.iTempOfs          );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iMaxTemp           " ,  MstOptn.iMaxTemp          );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iPreMaxTemp        " ,  MstOptn.iPreMaxTemp       );


    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedVision        " ,  MstOptn.bUsedVision       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedTempRS232     " ,  MstOptn.bUsedTempRS232    );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedEngVersion    " ,  MstOptn.bUsedEngVersion   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsed3040Pnch      " ,  MstOptn.bUsed3040Pnch     );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedXAxis         " ,  MstOptn.bUsedXAxis        );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedBinSorting    " ,  MstOptn.bUsedBinSorting   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedChinaSPC      " ,  MstOptn.bUsedChinaSPC     );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUsedSmallSizePkg  " , &MstOptn.bUsedSmallSizePkg );
}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRT-450S";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

//    if(!FileExists(sPath)) {
//        UserINI.Save(sPath, "EqpOptn"  , "sModelName    " , sModelName );
//        UserINI.Save(sPath, "EqpOptn"  , "bExistLoader  " , "1"        );
//        UserINI.Save(sPath, "EqpOptn"  , "bExistUnLoader" , "0"        );
//    }

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , &EqpOptn.sModelName     );
    UserINI.Load(sPath, "EqpOptn"  , "bExistLoader  " , &EqpOptn.bExistLoader   );
    UserINI.Load(sPath, "EqpOptn"  , "bExistUnLoader" , &EqpOptn.bExistUnLoader );

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

