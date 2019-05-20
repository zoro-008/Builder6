//---------------------------------------------------------------------------

#include <windows.h>
#pragma hdrstop

#include "SMInterfaceUnit.h"


#pragma package(smart_init)

PFM_MakeWin FM_MakeWin ;
PFM_SetWin  FM_SetWin  ;
PFM_DelWin  FM_DelWin  ;
PFM_HideWin FM_HideWin ;
PFM_GetVer  FM_GetVer  ;
PFM_GetAge  FM_GetAge  ;
PFM_GetLan  FM_GetLan  ;

PFM_MsgOk      FM_MsgOk      ;
PFM_MsgOkModal FM_MsgOkModal ;
PFM_MsgYesNo   FM_MsgYesNo   ;
PFM_MsgTime    FM_MsgTime    ;
PFM_CloseMsgOk FM_CloseMsgOk ;

PFM_PswdShow FM_PswdShow;
PFM_GetLevel FM_GetLevel;
PFM_SetLevel FM_SetLevel;

PSM_Update SM_Update ;

PMT_SetHomeEnd     MT_SetHomeEnd    ;
PMT_GetHomeEnd     MT_GetHomeEnd    ;
PMT_GetHomeEndAll  MT_GetHomeEndAll ;
PMT_SetServo       MT_SetServo      ;
PMT_GetServo       MT_GetServo      ;
PMT_GetServoAll    MT_GetServoAll   ;
PMT_SetServoAll    MT_SetServoAll   ;
PMT_SetReset       MT_SetReset      ;
PMT_GetAlarm       MT_GetAlarm      ;
PMT_GetHomeSnsr    MT_GetHomeSnsr   ;
PMT_GetPLimSnsr    MT_GetPLimSnsr   ;
PMT_GetNLimSnsr    MT_GetNLimSnsr   ;
PMT_GetStopInpos   MT_GetStopInpos  ;
PMT_GetStop        MT_GetStop       ;
PMT_GetZphaseSnsr  MT_GetZphaseSnsr ;

PMT_GetTrgPos MT_GetTrgPos ;
PMT_GetCmdPos MT_GetCmdPos ;
PMT_GetEncPos MT_GetEncPos ;
PMT_GetErrPos MT_GetErrPos ;
PMT_ClearPos  MT_ClearPos  ;
PMT_SetPos    MT_SetPos    ;
PMT_CmprPos   MT_CmprPos   ;
PMT_GetMinPos MT_GetMinPos ;
PMT_GetMaxPos MT_GetMaxPos ;
PMT_GetRunVel MT_GetRunVel ;
PMT_GetManVel MT_GetManVel ;

PMT_Reset    MT_Reset    ;
PMT_ResetAll MT_ResetAll ;
PMT_Stop     MT_Stop     ;
PMT_EmgStop  MT_EmgStop  ;

PMT_Home       MT_Home       ;
PMT_DoHome     MT_DoHome     ;
PMT_DoRpt      MT_DoRpt      ;

PMT_JogP MT_JogP ;
PMT_JogN MT_JogN ;

PMT_GoAbs     MT_GoAbs     ;
PMT_GoInc     MT_GoInc     ;
PMT_GoAbsRun  MT_GoAbsRun  ;
PMT_GoIncRun  MT_GoIncRun  ;
PMT_GoAbsSlow MT_GoAbsSlow ;
PMT_GoIncSlow MT_GoIncSlow ;
PMT_GoAbsMan  MT_GoAbsMan  ;
PMT_GoIncMan  MT_GoIncMan  ;
PMT_GoAbsAcDc MT_GoAbsAcDc ;
PMT_GoIncAcDc MT_GoIncAcDc ;

PMT_GetName     MT_GetName     ;
PMT_GetMotorCnt MT_GetMotorCnt ;
PMT_GetDirType  MT_GetDirType  ;

PMT_SetAxtTrgPos   MT_SetAxtTrgPos   ;
PMT_ResetAxtTrgPos MT_ResetAxtTrgPos ;
PMT_OneShotAxtTrg  MT_OneShotAxtTrg  ;

PMT_SetIntrptPos MT_SetIntrptPos ;
PMT_ResetIntrpt  MT_ResetIntrpt  ;

PMT_SetOverrideMaxSpeed MT_SetOverrideMaxSpeed ;
PMT_SetOverrideVel      MT_SetOverrideVel      ;
PMT_SetOverrideVelAtPos MT_SetOverrideVelAtPos ;

// JH_
PMT_GetContCrntIdx     MT_GetContCrntIdx     ;
PMT_LineMove           MT_LineMove           ;
PMT_ContiWriteClear    MT_ContiWriteClear    ;
PMT_ContiSetAxisMap    MT_ContiSetAxisMap    ;
PMT_ContiSetAbsRelMode MT_ContiSetAbsRelMode ;
PMT_ContiBeginNode     MT_ContiBeginNode     ;
PMT_ContiEndNode       MT_ContiEndNode       ;
PMT_ContiStart         MT_ContiStart         ;
PMT_CircleCenterMove   MT_CircleCenterMove   ;
PMT_CirclePointMove    MT_CirclePointMove    ;
PMT_CircleRadiusMove   MT_CircleRadiusMove   ;
PMT_CircleAngleMove    MT_CircleAngleMove    ;

PMT_SetGantryEnable  MT_SetGantryEnable  ;
PMT_SetGantryDisable MT_SetGantryDisable ;
PMT_SetHomeMethod    MT_SetHomeMethod    ;
PMT_SetHomeVel       MT_SetHomeVel       ;
PMT_SetHomeStart     MT_SetHomeStart     ;
PMT_GetHomeResult    MT_GetHomeResult    ;


PAT_Reset      AT_Reset      ;
PAT_Err        AT_Err        ;
PAT_Complete   AT_Complete   ;
PAT_Done       AT_Done       ;
PAT_GetName    AT_GetName    ;
PAT_MoveCyl    AT_MoveCyl    ;
PAT_GetCmd     AT_GetCmd     ;
PAT_GetActCnt  AT_GetActCnt  ;
PAT_GetDirType AT_GetDirType ;

PIO_SetY   IO_SetY   ;
PIO_GetY   IO_GetY   ;
PIO_GetYDn IO_GetYDn ;
PIO_GetYUp IO_GetYUp ;

PIO_GetX   IO_GetX   ;
PIO_GetXDn IO_GetXDn ;
PIO_GetXUp IO_GetXUp ;

PIO_SetTestMode IO_SetTestMode ;
PIO_GetTestMode IO_GetTestMode ;

PIO_GetXName IO_GetXName ;
PIO_GetYName IO_GetYName ;

PEM_Clear      EM_Clear      ;
PEM_SetErr     EM_SetErr     ;
PEM_SetDisp    EM_SetDisp    ;
PEM_SetErrMsg  EM_SetErrMsg  ;
PEM_GetErrMsg  EM_GetErrMsg  ;

PEM_IsErr          EM_IsErr          ;
PEM_GetLastErr     EM_GetLastErr     ;
PEM_GetLastErrName EM_GetLastErrName ;
PEM_GetErrName     EM_GetErrName     ;
PEM_GetErrAction   EM_GetErrAction   ;

PEM_GetErrCnt      EM_GetErrCnt ;
PEM_GetMaxErr      EM_GetMaxErr ;
PEM_GetErr         EM_GetErr    ;


PTL_MuteSnd TL_MuteSnd ;

PAI_SetY AI_SetY ;
PAI_GetY AI_GetY ;
PAI_GetX AI_GetX ;

HINSTANCE g_hSMLInst ;


bool LoadProc()
{
    if(!g_hSMLInst) return false ;

    FM_MakeWin               =(PFM_MakeWin            )GetProcAddress(g_hSMLInst, "FM_MakeWin"               );
    FM_SetWin                =(PFM_SetWin             )GetProcAddress(g_hSMLInst, "FM_SetWin"                );
    FM_DelWin                =(PFM_DelWin             )GetProcAddress(g_hSMLInst, "FM_DelWin"                );
    FM_HideWin               =(PFM_HideWin            )GetProcAddress(g_hSMLInst, "FM_HideWin"               );
    FM_GetVer                =(PFM_GetVer             )GetProcAddress(g_hSMLInst, "FM_GetVer"                );
    FM_GetAge                =(PFM_GetAge             )GetProcAddress(g_hSMLInst, "FM_GetAge"                );
    FM_GetLan                =(PFM_GetLan             )GetProcAddress(g_hSMLInst, "FM_GetLan"                );

    FM_MsgOk                 =(PFM_MsgOk              )GetProcAddress(g_hSMLInst, "FM_MsgOk"                 );
    FM_MsgOkModal            =(PFM_MsgOkModal         )GetProcAddress(g_hSMLInst, "FM_MsgOkModal"            );
    FM_MsgYesNo              =(PFM_MsgYesNo           )GetProcAddress(g_hSMLInst, "FM_MsgYesNo"              );
    FM_MsgTime               =(PFM_MsgTime            )GetProcAddress(g_hSMLInst, "FM_MsgTime"               );
    FM_CloseMsgOk            =(PFM_CloseMsgOk         )GetProcAddress(g_hSMLInst, "FM_CloseMsgOk"            );

    FM_PswdShow              =(PFM_PswdShow           )GetProcAddress(g_hSMLInst, "FM_PswdShow"              );
    FM_GetLevel              =(PFM_GetLevel           )GetProcAddress(g_hSMLInst, "FM_GetLevel"              );
    FM_SetLevel              =(PFM_SetLevel           )GetProcAddress(g_hSMLInst, "FM_SetLevel"              );

    SM_Update                =(PSM_Update             )GetProcAddress(g_hSMLInst, "SM_Update"                );

    MT_SetHomeEnd            =(PMT_SetHomeEnd         )GetProcAddress(g_hSMLInst, "MT_SetHomeEnd"            );
    MT_GetHomeEnd            =(PMT_GetHomeEnd         )GetProcAddress(g_hSMLInst, "MT_GetHomeEnd"            );
    MT_GetHomeEndAll         =(PMT_GetHomeEndAll      )GetProcAddress(g_hSMLInst, "MT_GetHomeEndAll"         );
    MT_SetServo              =(PMT_SetServo           )GetProcAddress(g_hSMLInst, "MT_SetServo"              );
    MT_GetServo              =(PMT_GetServo           )GetProcAddress(g_hSMLInst, "MT_GetServo"              );
    MT_GetServoAll           =(PMT_GetServoAll        )GetProcAddress(g_hSMLInst, "MT_GetServoAll"           );
    MT_SetServoAll           =(PMT_SetServoAll        )GetProcAddress(g_hSMLInst, "MT_SetServoAll"           );
    MT_SetReset              =(PMT_SetReset           )GetProcAddress(g_hSMLInst, "MT_SetReset"              );
    MT_GetAlarm              =(PMT_GetAlarm           )GetProcAddress(g_hSMLInst, "MT_GetAlarm"              );
    MT_GetHomeSnsr           =(PMT_GetHomeSnsr        )GetProcAddress(g_hSMLInst, "MT_GetHomeSnsr"           );
    MT_GetPLimSnsr           =(PMT_GetPLimSnsr        )GetProcAddress(g_hSMLInst, "MT_GetPLimSnsr"           );
    MT_GetNLimSnsr           =(PMT_GetNLimSnsr        )GetProcAddress(g_hSMLInst, "MT_GetNLimSnsr"           );
    MT_GetStopInpos          =(PMT_GetStopInpos       )GetProcAddress(g_hSMLInst, "MT_GetStopInpos"          );
    MT_GetStop               =(PMT_GetStop            )GetProcAddress(g_hSMLInst, "MT_GetStop"               );
    MT_GetZphaseSnsr         =(PMT_GetZphaseSnsr      )GetProcAddress(g_hSMLInst, "MT_GetZphaseSnsr"         );

    MT_GetTrgPos             =(PMT_GetTrgPos          )GetProcAddress(g_hSMLInst, "MT_GetTrgPos"             );
    MT_GetCmdPos             =(PMT_GetCmdPos          )GetProcAddress(g_hSMLInst, "MT_GetCmdPos"             );
    MT_GetEncPos             =(PMT_GetEncPos          )GetProcAddress(g_hSMLInst, "MT_GetEncPos"             );
    MT_GetErrPos             =(PMT_GetErrPos          )GetProcAddress(g_hSMLInst, "MT_GetErrPos"             );
    MT_ClearPos              =(PMT_ClearPos           )GetProcAddress(g_hSMLInst, "MT_ClearPos"              );
    MT_SetPos                =(PMT_SetPos             )GetProcAddress(g_hSMLInst, "MT_SetPos"                );
    MT_CmprPos               =(PMT_CmprPos            )GetProcAddress(g_hSMLInst, "MT_CmprPos"               );
    MT_GetMinPos             =(PMT_GetMinPos          )GetProcAddress(g_hSMLInst, "MT_GetMinPos"             );
    MT_GetMaxPos             =(PMT_GetMaxPos          )GetProcAddress(g_hSMLInst, "MT_GetMaxPos"             );
    MT_GetRunVel             =(PMT_GetRunVel          )GetProcAddress(g_hSMLInst, "MT_GetRunVel"             );
    MT_GetManVel             =(PMT_GetManVel          )GetProcAddress(g_hSMLInst, "MT_GetManVel"             );

    MT_Reset                 =(PMT_Reset              )GetProcAddress(g_hSMLInst, "MT_Reset"                 );
    MT_ResetAll              =(PMT_ResetAll           )GetProcAddress(g_hSMLInst, "MT_ResetAll"              );
    MT_Stop                  =(PMT_Stop               )GetProcAddress(g_hSMLInst, "MT_Stop"                  );
    MT_EmgStop               =(PMT_EmgStop            )GetProcAddress(g_hSMLInst, "MT_EmgStop"               );

    MT_Home                  =(PMT_Home               )GetProcAddress(g_hSMLInst, "MT_Home"                  );
    MT_DoHome                =(PMT_DoHome             )GetProcAddress(g_hSMLInst, "MT_DoHome"                );
    MT_DoRpt                 =(PMT_DoRpt              )GetProcAddress(g_hSMLInst, "MT_DoRpt"                 );

    MT_JogP                  =(PMT_JogP               )GetProcAddress(g_hSMLInst, "MT_JogP"                  );
    MT_JogN                  =(PMT_JogN               )GetProcAddress(g_hSMLInst, "MT_JogN"                  );

    MT_GoAbs                 =(PMT_GoAbs              )GetProcAddress(g_hSMLInst, "MT_GoAbs"                 );
    MT_GoInc                 =(PMT_GoInc              )GetProcAddress(g_hSMLInst, "MT_GoInc"                 );
    MT_GoAbsRun              =(PMT_GoAbsRun           )GetProcAddress(g_hSMLInst, "MT_GoAbsRun"              );
    MT_GoIncRun              =(PMT_GoIncRun           )GetProcAddress(g_hSMLInst, "MT_GoIncRun"              );
    MT_GoAbsSlow             =(PMT_GoAbsSlow          )GetProcAddress(g_hSMLInst, "MT_GoAbsSlow"             );
    MT_GoIncSlow             =(PMT_GoIncSlow          )GetProcAddress(g_hSMLInst, "MT_GoIncSlow"             );
    MT_GoAbsMan              =(PMT_GoAbsMan           )GetProcAddress(g_hSMLInst, "MT_GoAbsMan"              );
    MT_GoIncMan              =(PMT_GoIncMan           )GetProcAddress(g_hSMLInst, "MT_GoIncMan"              );
    MT_GoAbsAcDc             =(PMT_GoAbsAcDc          )GetProcAddress(g_hSMLInst, "MT_GoAbsAcDc"             );
    MT_GoIncAcDc             =(PMT_GoIncAcDc          )GetProcAddress(g_hSMLInst, "MT_GoIncAcDc"             );

    MT_GetName               =(PMT_GetName            )GetProcAddress(g_hSMLInst, "MT_GetName"               );
    MT_GetMotorCnt           =(PMT_GetMotorCnt        )GetProcAddress(g_hSMLInst, "MT_GetMotorCnt"           );
    MT_GetDirType            =(PMT_GetDirType         )GetProcAddress(g_hSMLInst, "MT_GetDirType"            );

    MT_SetAxtTrgPos          =(PMT_SetAxtTrgPos       )GetProcAddress(g_hSMLInst, "MT_SetAxtTrgPos"          );
    MT_ResetAxtTrgPos        =(PMT_ResetAxtTrgPos     )GetProcAddress(g_hSMLInst, "MT_ResetAxtTrgPos"        );
    MT_OneShotAxtTrg         =(PMT_OneShotAxtTrg      )GetProcAddress(g_hSMLInst, "MT_OneShotAxtTrg"         );

    MT_SetIntrptPos          =(PMT_SetIntrptPos       )GetProcAddress(g_hSMLInst, "MT_SetIntrptPos"          );
    MT_ResetIntrpt           =(PMT_ResetIntrpt        )GetProcAddress(g_hSMLInst, "MT_ResetIntrpt"           );

    MT_SetOverrideMaxSpeed   =(PMT_SetOverrideMaxSpeed)GetProcAddress(g_hSMLInst, "MT_SetOverrideMaxSpeed"   ); if(!MT_SetOverrideMaxSpeed)MessageBox(0,"PMT_SetOverrideMaxSpeed","Function Loading Err",0);
    MT_SetOverrideVel        =(PMT_SetOverrideVel     )GetProcAddress(g_hSMLInst, "MT_SetOverrideVel"        ); if(!MT_SetOverrideVel     )MessageBox(0,"PMT_SetOverrideVel     ","Function Loading Err",0);
    MT_SetOverrideVelAtPos   =(PMT_SetOverrideVelAtPos)GetProcAddress(g_hSMLInst, "MT_SetOverrideVelAtPos"   ); if(!MT_SetOverrideVelAtPos)MessageBox(0,"PMT_SetOverrideVelAtPos","Function Loading Err",0);

    // 보간 구동을 위해 추가
    MT_ContiBeginNode        =(PMT_ContiBeginNode     )GetProcAddress(g_hSMLInst, "MT_ContiBeginNode"        );
    MT_ContiEndNode          =(PMT_ContiEndNode       )GetProcAddress(g_hSMLInst, "MT_ContiEndNode"          );
    MT_ContiSetAbsRelMode    =(PMT_ContiSetAbsRelMode )GetProcAddress(g_hSMLInst, "MT_ContiSetAbsRelMode"    );
    MT_ContiSetAxisMap       =(PMT_ContiSetAxisMap    )GetProcAddress(g_hSMLInst, "MT_ContiSetAxisMap"       );
    MT_ContiStart            =(PMT_ContiStart         )GetProcAddress(g_hSMLInst, "MT_ContiStart"            );
    MT_ContiWriteClear       =(PMT_ContiWriteClear    )GetProcAddress(g_hSMLInst, "MT_ContiWriteClear"       );
    MT_GetContCrntIdx        =(PMT_GetContCrntIdx     )GetProcAddress(g_hSMLInst, "MT_GetContCrntIdx"        );
    MT_LineMove              =(PMT_LineMove           )GetProcAddress(g_hSMLInst, "MT_LineMove"              );
    MT_CircleCenterMove      =(PMT_CircleCenterMove   )GetProcAddress(g_hSMLInst, "MT_CircleCenterMove"      );
    MT_CirclePointMove       =(PMT_CirclePointMove    )GetProcAddress(g_hSMLInst, "MT_CirclePointMove"       );
    MT_CircleRadiusMove      =(PMT_CircleRadiusMove   )GetProcAddress(g_hSMLInst, "MT_CircleRadiusMove"      );
    MT_CircleAngleMove       =(PMT_CircleAngleMove    )GetProcAddress(g_hSMLInst, "MT_CircleAngleMove"       );

    MT_SetGantryEnable       =(PMT_SetGantryEnable    )GetProcAddress(g_hSMLInst, "MT_SetGantryEnable"       );
    MT_SetGantryDisable      =(PMT_SetGantryDisable   )GetProcAddress(g_hSMLInst, "MT_SetGantryDisable"      );
    MT_SetHomeMethod         =(PMT_SetHomeMethod      )GetProcAddress(g_hSMLInst, "MT_SetHomeMethod"         );
    MT_SetHomeVel            =(PMT_SetHomeVel         )GetProcAddress(g_hSMLInst, "MT_SetHomeVel"            );
    MT_SetHomeStart          =(PMT_SetHomeStart       )GetProcAddress(g_hSMLInst, "MT_SetHomeStart"          );
    MT_GetHomeResult         =(PMT_GetHomeResult      )GetProcAddress(g_hSMLInst, "MT_GetHomeResult"         );

    AT_Reset                 =(PAT_Reset              )GetProcAddress(g_hSMLInst, "AT_Reset"                 );
    AT_Err                   =(PAT_Err                )GetProcAddress(g_hSMLInst, "AT_Err"                   );
    AT_Complete              =(PAT_Complete           )GetProcAddress(g_hSMLInst, "AT_Complete"              );
    AT_Done                  =(PAT_Done               )GetProcAddress(g_hSMLInst, "AT_Done"                  );
    AT_GetName               =(PAT_GetName            )GetProcAddress(g_hSMLInst, "AT_GetName"               );
    AT_MoveCyl               =(PAT_MoveCyl            )GetProcAddress(g_hSMLInst, "AT_MoveCyl"               );
    AT_GetCmd                =(PAT_GetCmd             )GetProcAddress(g_hSMLInst, "AT_GetCmd"                );
    AT_GetActCnt             =(PAT_GetActCnt          )GetProcAddress(g_hSMLInst, "AT_GetActCnt"             );
    AT_GetDirType            =(PAT_GetDirType         )GetProcAddress(g_hSMLInst, "AT_GetDirType"            );

    IO_SetY                  =(PIO_SetY               )GetProcAddress(g_hSMLInst, "IO_SetY"                  );
    IO_GetY                  =(PIO_GetY               )GetProcAddress(g_hSMLInst, "IO_GetY"                  );
    IO_GetYDn                =(PIO_GetYDn             )GetProcAddress(g_hSMLInst, "IO_GetYDn"                );
    IO_GetYUp                =(PIO_GetYUp             )GetProcAddress(g_hSMLInst, "IO_GetYUp"                );

    IO_GetX                  =(PIO_GetX               )GetProcAddress(g_hSMLInst, "IO_GetX"                  );
    IO_GetXDn                =(PIO_GetXDn             )GetProcAddress(g_hSMLInst, "IO_GetXDn"                );
    IO_GetXUp                =(PIO_GetXUp             )GetProcAddress(g_hSMLInst, "IO_GetXUp"                );

    IO_SetTestMode           =(PIO_SetTestMode        )GetProcAddress(g_hSMLInst, "IO_SetTestMode"           );
    IO_GetTestMode           =(PIO_GetTestMode        )GetProcAddress(g_hSMLInst, "IO_GetTestMode"           );

    IO_GetXName              =(PIO_GetXName           )GetProcAddress(g_hSMLInst, "IO_GetXName"              );
    IO_GetYName              =(PIO_GetYName           )GetProcAddress(g_hSMLInst, "IO_GetYName"              );

    EM_Clear                 =(PEM_Clear              )GetProcAddress(g_hSMLInst, "EM_Clear"                 );
    EM_SetErr                =(PEM_SetErr             )GetProcAddress(g_hSMLInst, "EM_SetErr"                );
    EM_SetDisp               =(PEM_SetDisp            )GetProcAddress(g_hSMLInst, "EM_SetDisp"               );
    EM_SetErrMsg             =(PEM_SetErrMsg          )GetProcAddress(g_hSMLInst, "EM_SetErrMsg"             );
    EM_GetErrMsg             =(PEM_GetErrMsg          )GetProcAddress(g_hSMLInst, "EM_GetErrMsg"             );

    EM_IsErr                 =(PEM_IsErr              )GetProcAddress(g_hSMLInst, "EM_IsErr"                 );
    EM_GetLastErr            =(PEM_GetLastErr         )GetProcAddress(g_hSMLInst, "EM_GetLastErr"            );
    EM_GetLastErrName        =(PEM_GetLastErrName     )GetProcAddress(g_hSMLInst, "EM_GetLastErrName"        );
    EM_GetErrName            =(PEM_GetErrName         )GetProcAddress(g_hSMLInst, "EM_GetErrName"            );
    EM_GetErrAction          =(PEM_GetErrAction       )GetProcAddress(g_hSMLInst, "EM_GetErrAction"          );

    EM_GetErrCnt             =(PEM_GetErrCnt          )GetProcAddress(g_hSMLInst, "EM_GetErrCnt"             );
    EM_GetMaxErr             =(PEM_GetMaxErr          )GetProcAddress(g_hSMLInst, "EM_GetMaxErr"             );
    EM_GetErr                =(PEM_GetErr             )GetProcAddress(g_hSMLInst, "EM_GetErr"                );


    TL_MuteSnd               =(PTL_MuteSnd            )GetProcAddress(g_hSMLInst, "TL_MuteSnd"               );

    AI_SetY                  =(PAI_SetY               )GetProcAddress(g_hSMLInst, "AI_SetY"                  );
    AI_GetY                  =(PAI_GetY               )GetProcAddress(g_hSMLInst, "AI_GetY"                  );
    AI_GetX                  =(PAI_GetX               )GetProcAddress(g_hSMLInst, "AI_GetX"                  );

	return true ;
}


bool LoadSML(char * _pFileName)
{
    // DLL 로드
    g_hSMLInst = LoadLibrary (_pFileName);

    if(!g_hSMLInst) return false ;//MessageBoxA()("Load SML Failed!");



    return LoadProc() ;

}

/*
bool LoadSML_W(LPCWSTR _pFileName)
{
    // DLL 로드
    g_hSMLInst = LoadLibrary (_pFileName);

    if(!g_hSMLInst) return false ;//MessageBoxA()("Load SML Failed!");



    return LoadProc() ;

}
*/

bool FreeSML()
{
    FM_MakeWin             = NULL ;
    FM_SetWin              = NULL ;
    FM_DelWin              = NULL ;
    FM_HideWin             = NULL ;
    FM_GetVer              = NULL ;
    FM_GetAge              = NULL ;
    FM_GetLan              = NULL ;

    FM_MsgOk               = NULL ;
    FM_MsgOkModal          = NULL ;
    FM_MsgYesNo            = NULL ;
    FM_MsgTime             = NULL ;
    FM_CloseMsgOk          = NULL ;

    FM_PswdShow            = NULL ;
    FM_GetLevel            = NULL ;
    FM_SetLevel            = NULL ;

    SM_Update              = NULL ;

    MT_SetHomeEnd          = NULL ;
    MT_GetHomeEnd          = NULL ;
    MT_GetHomeEndAll       = NULL ;
    MT_SetServo            = NULL ;
    MT_GetServo            = NULL ;
    MT_GetServoAll         = NULL ;
    MT_SetServoAll         = NULL ;
    MT_SetReset            = NULL ;
    MT_GetAlarm            = NULL ;
    MT_GetHomeSnsr         = NULL ;
    MT_GetPLimSnsr         = NULL ;
    MT_GetNLimSnsr         = NULL ;
    MT_GetStopInpos        = NULL ;
    MT_GetStop             = NULL ;
    MT_GetZphaseSnsr       = NULL ;

    MT_GetTrgPos           = NULL ;
    MT_GetCmdPos           = NULL ;
    MT_GetEncPos           = NULL ;
    MT_GetErrPos           = NULL ;
    MT_ClearPos            = NULL ;
    MT_SetPos              = NULL ;
    MT_CmprPos             = NULL ;
    MT_GetMinPos           = NULL ;
    MT_GetMaxPos           = NULL ;
    MT_GetRunVel           = NULL ;
    MT_GetManVel           = NULL ;

    MT_Reset               = NULL ;
    MT_ResetAll            = NULL ;
    MT_Stop                = NULL ;
    MT_EmgStop             = NULL ;

    MT_Home                = NULL ;
    MT_DoHome              = NULL ;
    MT_DoRpt               = NULL ;

    MT_JogP                = NULL ;
    MT_JogN                = NULL ;

    MT_GoAbs               = NULL ;
    MT_GoInc               = NULL ;
    MT_GoAbsRun            = NULL ;
    MT_GoIncRun            = NULL ;
    MT_GoAbsSlow           = NULL ;
    MT_GoIncSlow           = NULL ;
    MT_GoAbsMan            = NULL ;
    MT_GoIncMan            = NULL ;
    MT_GoAbsAcDc           = NULL ;
    MT_GoIncAcDc           = NULL ;

    MT_GetName             = NULL ;
    MT_GetMotorCnt         = NULL ;
    MT_GetDirType          = NULL ;

    MT_SetAxtTrgPos        = NULL ;
    MT_ResetAxtTrgPos      = NULL ;
    MT_OneShotAxtTrg       = NULL ;

    MT_SetIntrptPos        = NULL ;
    MT_ResetIntrpt         = NULL ;

    MT_SetOverrideMaxSpeed = NULL ;
    MT_SetOverrideVel      = NULL ;
    MT_SetOverrideVelAtPos = NULL ;

    MT_ContiSetAxisMap     = NULL ;
    MT_ContiSetAbsRelMode  = NULL ;
    MT_ContiBeginNode      = NULL ;
    MT_ContiEndNode        = NULL ;
    MT_ContiStart          = NULL ;
    MT_GetContCrntIdx      = NULL ;
    MT_LineMove            = NULL ;
    MT_CircleCenterMove    = NULL ;
    MT_CirclePointMove     = NULL ;
    MT_CircleRadiusMove    = NULL ;
    MT_CircleAngleMove     = NULL ;

    AT_Reset               = NULL ;
    AT_Err                 = NULL ;
    AT_Complete            = NULL ;
    AT_Done                = NULL ;
    AT_GetName             = NULL ;
    AT_MoveCyl             = NULL ;
    AT_GetCmd              = NULL ;
    AT_GetActCnt           = NULL ;
    AT_GetDirType          = NULL ;

    IO_SetY                = NULL ;
    IO_GetY                = NULL ;
    IO_GetYDn              = NULL ;
    IO_GetYUp              = NULL ;

    IO_GetX                = NULL ;
    IO_GetXDn              = NULL ;
    IO_GetXUp              = NULL ;

    IO_SetTestMode         = NULL ;
    IO_GetTestMode         = NULL ;

    IO_GetXName            = NULL ;
    IO_GetYName            = NULL ;

    EM_Clear               = NULL ;
    EM_SetErr              = NULL ;
    EM_SetDisp             = NULL ;
    EM_SetErrMsg           = NULL ;
    EM_GetErrMsg           = NULL ;

    EM_IsErr               = NULL ;
    EM_GetLastErr          = NULL ;
    EM_GetLastErrName      = NULL ;
    EM_GetErrName          = NULL ;
    EM_GetErrAction        = NULL ;

    EM_GetErrCnt           = NULL ;
    EM_GetMaxErr           = NULL ;
    EM_GetErr              = NULL ;


    TL_MuteSnd             = NULL ;

    AI_SetY                = NULL ;
    AI_GetY                = NULL ;
    AI_GetX                = NULL ;

    FreeLibrary(g_hSMLInst);

    return true ;
}



