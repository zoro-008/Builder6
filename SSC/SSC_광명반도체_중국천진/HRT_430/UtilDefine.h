#ifndef UTILDEFINEH
#define UTILDEFINEH

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum EN_ARAY_ID {
    riLD1       =0 ,
    riPR1          ,
    riPR2          ,
    riWRP          ,
    riWRK          ,
    riPS1          ,
    riPS2          ,
    riTmp          , //메뉴얼 리핏을 위해서 더미 어레이 생성.
    riSPC          ,

    MAX_ARAY
};

//enum EN_MOTR_ID {
//    miPRB_XCmp  =0 ,    //
//    miWRK_XCmp     ,    //
//    miPSB_XCmp     ,    //
//    miWRK_ZTop     ,    //
//    miWRK_ZBtm     ,    //
//    miLD1_ZCmp     ,    //
//    miLD1_YCmp     ,    //
//
//    MAX_MOTR
//};

enum EN_MOTR_ID {
    miLD1_ZCmp  =0 ,    //
    miLD1_YCmp     ,    //
    miPRB_XCmp     ,    //
    miWRK_XCmp     ,    //
    miPSB_XCmp     ,    //
    miWRK_ZTop     ,    //
    miWRK_ZBtm     ,    //

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiPRB_Cmp   = 0,     //
    aiPRB_IdChk    ,     //
    aiWRK_Cmp      ,     //
    aiPSB_Cmp      ,     //
    aiLD1_Cmp      ,     //

    MAX_ACTR
};

enum EN_ERR_ID {
    eiETC_MainAir          =  0 ,   // 31 번 예정 스트립 두개 감지 센서.
    eiETC_ToStartTO             ,
    eiETC_ToStopTO              ,
    eiETC_Emergency             ,
    eiETC_FDoor                 ,
    eiETC_BDoor                 ,
    eiETC_LotEnd                ,
    eiETC_AllHomeTO             ,
    eiETC_ManCycleTO            ,

    eiPRB_CycleTO          =  9 ,    //
    eiWRK_CycleTO               ,    // 31 번 예정 스트립 두개 감지 센서.
    eiPSB_CycleTO               ,    //
    eiLD1_CycleTO               ,    //

    eiPRB_HomeTo           =  13,    //
    eiPSB_HomeTo                ,    //
    eiWRK_HomeTo                ,    //
    eiLD1_HomeTO                ,    //

    eiPRB_ToStartTO        =  17,    //
    eiPSB_ToStartTO             ,    //
    eiWRK_ToStartTO             ,    //
    eiLD1_ToStartTO             ,    //

    eiPRB_ToStopTO         =  21,    //
    eiPSB_ToStopTO              ,    //
    eiWRK_ToStopTO              ,    //
    eiLD1_ToStopTO              ,    //

    eiLD1_ZCmpHomeEnd      =  25,    //
    eiLD1_ZCmpNegLim            ,    //
    eiLD1_ZCmpPosLim            ,    //
    eiLD1_ZCmpAlarm             ,    //

    eiLD1_YCmpHomeEnd      =  29,    //
    eiLD1_YCmpNegLim            ,    //
    eiLD1_YCmpPosLim            ,    //
    eiLD1_YCmpAlarm             ,    //

    eiPSB_XCmpHomeEnd      =  33,    //
    eiPSB_XCmpPosLim            ,    //
    eiPSB_XCmpAlarm             ,    //

    eiWRK_XCmpHomeEnd      =  36,    //
    eiWRK_XCmpPosLim            ,    //
    eiWRK_XCmpAlarm             ,    //

    eiPRB_XCmpHomeEnd      =  39,    //
    eiPRB_XCmpPosLim            ,    //
    eiPRB_XCmpAlarm             ,    //

    eiWRK_ZTopHomeEnd      =  42,    //
    eiWRK_ZTopNegLim            ,    //
    eiWRK_ZTopPosLim            ,    //
    eiWRK_ZTopAlarm             ,    //

    eiWRK_ZBtmHomeEnd      =  46,    //
    eiWRK_ZBtmNegLim            ,    //
    eiWRK_ZBtmAlarm             ,    //

    eiPRB_CmpTO            =  49,    //
    eiPRB_IdChkTO               ,    //
    eiWRK_CmpTO                 ,    //
    eiPSB_CmpTO                 ,    //
    eiLD1_CmpTO                 ,    //

    //여기 모르는 스트립,스트립 사라짐.
    eiLD1_ClampDispr       =  54,   //
    eiLD1_ClampUnknown          ,   //
    eiLD1_MgzChange             ,   // 에러 리스트에 아직 미추가.
    eiPRB_Dispr                 ,   //
    eiPRB_Unknown               ,   //
    eiWRK_Dispr                 ,   //
    eiWRK_Unknown               ,   //
    eiPSB_Dispr                 ,   //
    eiPSB_Unknown               ,   //

    eiWRK_FailCntOver           ,   //
    eiWRK_FailProbeComm1        ,   // 에러 리스트에 아직 미추가.
    eiWRK_FailProbeComm2        ,   // 에러 리스트에 아직 미추가.
    eiLD1_MgzFull               ,   //
    eiLD1_PusherOverload        ,   //
    eiPRB_Overload              ,   //
    eiWRK_Overload              ,   //
    eiPSB_Overload              ,   //
    eiPRB_IdCheck               ,   //

    eiPRB_TempZ1Alrm1           ,   //
    eiPRB_TempZ1Wire2           ,   //
    eiPRB_TempZ1Alrm2           ,   //
    eiPRB_TempZ1Wire3           ,   //
    eiPRB_TempZ1Alrm3           ,   //
    eiPRB_TempZ1Wire4           ,   //
    eiPRB_TempZ1Alrm4           ,   //

    eiPSB_TempZ2Alrm1           ,   //
    eiPSB_TempZ2Wire1           ,   //
    eiPSB_TempZ2Alrm2           ,   //
    eiPSB_TempZ2Wire2           ,   //
    eiPSB_TempZ2Alrm3           ,   //
    eiPSB_TempZ2Wire3           ,   //

    eiETC_NetSeverFail          ,   //
    eiETC_NetConectFail         ,   //
    eiETC_PostEqpLotStr         ,   // 에러 리스트에 아직 미추가.
    eiETC_PostEqpLotEnd         ,   // 에러 리스트에 아직 미추가.
    eiETC_PostEqpModelChg       ,   // 에러 리스트에 아직 미추가.
    eiETC_PostEqpData           ,   // 에러 리스트에 아직 미추가.
    eiETC_PostEqpNotReady       ,   // 에러 리스트에 아직 미추가.
    eiETC_PostEqpCommTO         ,   // 에러 리스트에 아직 미추가.
//    eiETC

    eiWRK_PinFailCntOver        ,
    eiWRK_Probe1DataWrong       ,
    eiWRK_Probe2DataWrong       ,

    eiMNR_StripRemoveTo         ,
    eiPRB_StripOver             ,

    eiLD1_NeedMgz               ,   // 메가진 공급 필요.
    eiWRK_ProbeSetFail          ,   // 99번

    eiWRK_ContactCntOver        ,

    MAX_ERR
};

enum EN_INPUT_ID {
    x0000 =  0 ,  xLD1_PusherOverLoad      =  0 , //
    x0001 =  1 ,  xLD1_PusherFw            =  1 , //
    x0002 =  2 ,  xLD1_PusherBw            =  2 , //
    x0003 =  3 ,  xLD1_MgzDetect1          =  3 , //
    x0004 =  4 ,  xLD1_MgzDetect2          =  4 , //
    x0005 =  5 ,  xLD1_ClampFw             =  5 , //
    x0006 =  6 ,  x006                     =  6 , //
    x0007 =  7 ,  x007                     =  7 , //
    x0008 =  8 ,  xLD1_MgzIn               =  8 , //
    x0009 =  9 ,  xLD1_MgzOutFull          =  9 , //
    x000A = 10 ,  xLD1_MainAirCheck        = 10 , //
    x000B = 11 ,  x011                     = 11 , //
    x000C = 12 ,  x012                     = 12 , //
    x000D = 13 ,  x013                     = 13 , //
    x000E = 14 ,  xETC_LEmgSw              = 14 , //
    x000F = 15 ,  x015                     = 15 , //

    x0010 = 16 ,  xETC_LStartSw            = 16 , //
    x0011 = 17 ,  xETC_LStopSw             = 17 , //
    x0012 = 18 ,  xETC_LResetSw            = 18 , //
    x0013 = 19 ,  xETC_LMainAir            = 19 , //
    x0014 = 20 ,  xETC_LInitSw             = 20 , //
    x0015 = 21 ,  xETC_RStartSw            = 21 , //
    x0016 = 22 ,  xETC_RStopSw             = 22 , //
    x0017 = 23 ,  xETC_RResetSw            = 23 , //
    x0018 = 24 ,  xETC_RMainAir            = 24 , //
    x0019 = 25 ,  xETC_RInitSw             = 25 , //
    x001A = 26 ,  x026                     = 26 , //
    x001B = 27 ,  x027                     = 27 , //
    x001C = 28 ,  x028                     = 28 , //
    x001D = 29 ,  xETC_EmgSw1              = 29 , //
    x001E = 30 ,  xETC_EmgSw2              = 30 , //
    x001F = 31 ,  x031                     = 31 , //

    x0020 = 32 ,  x032                     = 32 , //
    x0021 = 33 ,  xETC_PowerOn             = 33 , //
    x0022 = 34 ,  xETC_MainSol             = 34 , //
    x0023 = 35 ,  xETC_FDoor               = 35 , //
    x0024 = 36 ,  xETC_BDoor               = 36 , //
    x0025 = 37 ,  xPRB_IdCheck             = 37 , //
    x0026 = 38 ,  xPSB_IdCheckBw           = 38 , //
    x0027 = 39 ,  xPSB_IdCheckFw           = 39 , //
    x0028 = 40 ,  xPRB_Overload            = 40 , //
    x0029 = 41 ,  xWRK_Overload            = 41 , //
    x002A = 42 ,  xPSB_Overload            = 42 , //
    x002B = 43 ,  xPRB_ClampFw             = 43 , //
    x002C = 44 ,  xPRB_ClampBw             = 44 , //
    x002D = 45 ,  xWRK_ClampFw             = 45 , //
    x002E = 46 ,  xWRK_ClampBw             = 46 , //
    x002F = 47 ,  xPRB_StripOver           = 47 , //

    x0030 = 48 ,  xPSB_ClampFw             = 48 , //
    x0031 = 49 ,  xPSB_ClampBw             = 49 , //
    x0032 = 50 ,  xWRK_1Pkg                = 50 , //
    x0033 = 51 ,  xWRK_2Pkg                = 51 , //
    x0034 = 52 ,  x052                     = 52 , //
    x0035 = 53 ,  xPRB_1Pkg                = 53 , //
    x0036 = 54 ,  xPRB_2Pkg                = 54 , //
    x0037 = 55 ,  xPRB_3Pkg                = 55 , //
    x0038 = 56 ,  xPSB_1Pkg                = 56 , //
    x0039 = 57 ,  xPSB_2Pkg                = 57 , //
    x003A = 58 ,  x058                     = 58 , //
    x003B = 59 ,  xPRB_TempZ1Alrm1         = 59 , //     xPRB_TempZ1Alrm1
    x003C = 60 ,  xPRB_TempZ1Wire2         = 60 , //
    x003D = 61 ,  xPRB_TempZ1Alrm2         = 61 , //
    x003E = 62 ,  xPRB_TempZ1Wire3         = 62 , //

    x003F = 63 ,  xPRB_TempZ1Alrm3         = 63 , //
    x0040 = 64 ,  xPRB_TempZ1Wire4         = 64 , //
    x0041 = 65 ,  xPRB_TempZ1Alrm4         = 65 , //
    x0042 = 66 ,  xPSB_TempZ2Alrm1         = 66 , //
    x0043 = 67 ,  xPSB_TempZ2Wire1         = 67 , //
    x0044 = 68 ,  xPSB_TempZ2Alrm2         = 68 , //
    x0045 = 69 ,  xPSB_TempZ2Wire2         = 69 , //
    x0046 = 70 ,  xPSB_TempZ2Alrm3         = 70 , //
    x0047 = 71 ,  xPSB_TempZ2Wire3         = 71 , //
    x0048 = 72 ,  xETC_PstEqpStart         = 72 , //
    x0049 = 73 ,  xETC_PstEqpStop          = 73 , //
    x004A = 74 ,  xETC_PstEqpReset         = 74 , //
    x004B = 75 ,  xETC_PstEqpReady         = 75 , //
    x004C = 76 ,  xETC_PstEqpSpare1        = 76 , //
    x004D = 77 ,  xETC_PstEqpSpare2        = 77 , //
    x004E = 78 ,  xETC_PstEqpSpare3        = 78 , //
    x004F = 79 ,  xETC_PstEqpSpare4        = 79 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yLD1_MgzInAC              =  0 , //
    y0001 =  1 ,  yLD1_MgzOutAC             =  1 , //
    y0002 =  2 ,  yLD1_ClampFw              =  2 , //
    y0003 =  3 ,  yLD1_ClampBw              =  3 , //
    y0004 =  4 ,  yLD1_PusherFw             =  4 , //
    y0005 =  5 ,  yLD1_PusherBw             =  5 , //
    y0006 =  6 ,  yLD1_ZBreak               =  6 , //
    y0007 =  7 ,  y007                      =  7 , //
    y0008 =  8 ,  y008                      =  8 , //
    y0009 =  9 ,  y009                      =  9 , //
    y000A = 10 ,  y010                      = 10 , //
    y000B = 11 ,  y011                      = 11 , //
    y000C = 12 ,  y012                      = 12 , //
    y000D = 13 ,  y013                      = 13 , //
    y000E = 14 ,  y014                      = 14 , //
    y000F = 15 ,  y015                      = 15 , //

    y0010 = 16 ,  yETC_LStartLp             = 16 , //
    y0011 = 17 ,  yETC_LStopLp              = 17 , //
    y0012 = 18 ,  yETC_LResetLp             = 18 , //
    y0013 = 19 ,  yETC_LAirLp               = 19 , //
    y0014 = 20 ,  yETC_LInitLp              = 20 , //
    y0015 = 21 ,  yETC_RStartLp             = 21 , //
    y0016 = 22 ,  yETC_RStopLp              = 22 , //
    y0017 = 23 ,  yETC_RResetLp             = 23 , //
    y0018 = 24 ,  yETC_RAirLp               = 24 , //
    y0019 = 25 ,  yETC_RInitLp              = 25 , //
    y001A = 26 ,  yETC_TwRedLp              = 26 , //
    y001B = 27 ,  yETC_TwYelLp              = 27 , //
    y001C = 28 ,  yETC_TwGrnLp              = 28 , //
    y001D = 29 ,  yETC_TwBzz                = 29 , //
    y001E = 30 ,  yETC_MainSol              = 30 , //
    y001F = 31 ,  yPRB_FeedingAC            = 31 , //

    y0020 = 32 ,  yPRB_AirBlower            = 32 , //
    y0021 = 33 ,  yPRB_ClampFw              = 33 , //
    y0022 = 34 ,  yPRB_ClampBw              = 34 , //
    y0023 = 35 ,  yWRK_ClampFw              = 35 , //
    y0024 = 36 ,  yWRK_ClampBw              = 36 , //
    y0025 = 37 ,  yPSB_ClampFw              = 37 , //
    y0026 = 38 ,  yPSB_ClampBw              = 38 , //
    y0027 = 39 ,  yPRB_ReverseFw            = 39 , //
    y0028 = 40 ,  yWRK_Probe1               = 40 , //
    y0029 = 41 ,  yWRK_Probe2               = 41 , //
    y002A = 42 ,  y042                      = 42 , //
    y002B = 43 ,  y043                      = 43 , //
    y002C = 44 ,  y044                      = 44 , //
    y002D = 45 ,  y045                      = 45 , //
    y002E = 46 ,  yETC_BlowerAC             = 46 , //
    y002F = 47 ,  y047                      = 47 , //

    y0030 = 48 ,  y048                      = 48 , //
    y0031 = 49 ,  y049                      = 49 , //
    y0032 = 50 ,  y050                      = 50 , //
    y0033 = 51 ,  y051                      = 51 , //
    y0034 = 52 ,  y052                      = 52 , //
    y0035 = 53 ,  y053                      = 53 , //
    y0036 = 54 ,  y054                      = 54 , //
    y0037 = 55 ,  y055                      = 55 , //
    y0038 = 56 ,  y056                      = 56 , //
    y0039 = 57 ,  y057                      = 57 , //
    y003A = 58 ,  y058                      = 58 , //
    y003B = 59 ,  y059                      = 59 , //
    y003C = 60 ,  y060                      = 60 , //
    y003D = 61 ,  y061                      = 61 , //
    y003E = 62 ,  y062                      = 62 , //
    y003F = 63 ,  y063                      = 63 , //

    y0040 = 64 ,  y064                      = 64 , //
    y0041 = 65 ,  y065                      = 65 , //
    y0042 = 66 ,  y066                      = 66 , //
    y0043 = 67 ,  y067                      = 67 , //
    y0044 = 68 ,  y068                      = 68 , //
    y0045 = 69 ,  y069                      = 69 , //
    y0046 = 70 ,  y070                      = 70 , //
    y0047 = 71 ,  y071                      = 71 , //
    y0048 = 72 ,  yETC_PstEqpStart          = 72 , // yETC_PstEqpStart
    y0049 = 73 ,  yETC_PstEqpStop           = 73 , // yETC_PstEqpStop
    y004A = 74 ,  yETC_PstEqpReset          = 74 , // yETC_PstEqpReset
    y004B = 75 ,  yETC_PstEqpLotStart       = 75 , // yETC_PstEqpLotStart
    y004C = 76 ,  yETC_PstEqpLotEnd         = 76 , // yETC_PstEqpLotEnd
    y004D = 77 ,  yETC_PstEqpSending        = 77 , // yETC_PstEqpStripSending
    y004E = 78 ,  yETC_PstEqpJobChange      = 78 , // yETC_PstEqpJobChange
    y004F = 79 ,  yETC_PstEqpSpare1         = 79 , // yETC_PstEqpSpare1

    MAX_OUTPUT
};

enum EN_PSTN_ID {
    piLD1_ZWait       =  0 ,  //                    
    piLD1_ZPickFwd         ,  //                    
    piLD1_ZClampOn         ,  //                    
    piLD1_ZPickBwd         ,  //                    
    piLD1_ZWorkStart       ,  //맨 아래 슬롯 기준.  
    piLD1_ZPlaceFwd        ,  //                    
    piLD1_ZClampOff        ,  //                    
    piLD1_ZPlaceBwd        ,  //
    piLD1_ZWork            ,  //                    
    MAX_PSTN_ID_MOTR0      ,  //                    
                              //                    
    piLD1_YWait       =  0 ,  //                    
    piLD1_YWork            ,  //                    
    piLD1_YPick            ,  //                    
    piLD1_YPlace           ,  //                    
    MAX_PSTN_ID_MOTR1      ,  //
                                                    
    piPRB_XWait       = 0  ,  //                    
    piPRB_XGrip            ,  //                    
    piPRB_XPull            ,  //                    
    piPRB_XBack            ,  //
    piPRB_XIdCheck         ,
    piPRB_XOut1            ,  //                    
    piPRB_XOut2            ,  //
    piPRB_XStep1           ,  // PR1에서 Pull 동작 하기 전에 스텝 타는 포지션
    p1PRB_XStep2           ,  // PR2에서 Out  동작 하기 전에 스텝 타는 포지션.
    MAX_PSTN_ID_MOTR2      ,  //                    
                              //                    
    piWRK_XWait       =  0 ,  //                    
    piWRK_XGrip            ,  //                    
    piWRK_XWorkStart       ,  //                    
    piWRK_XOut             ,  //
    piWRK_XReGrip          ,  //
    piWRK_XWork            ,  //                    
//    piWRK_XReWork          ,  //                  
    MAX_PSTN_ID_MOTR3      ,  //                    
                              //                    
    piPSB_XWait       = 0  ,  //
    piPSB_XGrip            ,  //                    
    piPSB_XPull            ,  //
    piPSB_XBack            ,  //                    
    piPSB_XOut             ,  //                    
    MAX_PSTN_ID_MOTR4      ,  //                    
                              //                    
    piWRK_ZTWait      =  0 ,  //                    
    piWRK_ZTWork           ,  //
    MAX_PSTN_ID_MOTR5      ,  //                    
                              //                    
    piWRK_ZBWait      =  0 ,  //                    
    piWRK_ZBWork           ,  //
    MAX_PSTN_ID_MOTR6      ,  //
                              //

};

enum EN_PSTN_VALUE {
    pvLD1_ZWaitPs       =  0 ,
    pvLD1_ZPickFwdPs         ,
    pvLD1_ZClampOnPs         ,
    pvLD1_ZPickBwdPs         ,
    pvLD1_ZWorkStartPs       ,
    pvLD1_ZPlaceFwdPs        ,
    pvLD1_ZClampOffPs        ,
    pvLD1_ZPlaceBwdPs        ,
    MAX_PSTN_MOTR0           ,

    pvLD1_YWaitPs       =  0 ,
    pvLD1_YWorkPs            ,
    pvLD1_YPickPs            ,
    pvLD1_YPlacePs           ,
    MAX_PSTN_MOTR1           ,

    pvPRB_XWaitPs       = 0  ,
    pvPRB_XGripPs            ,
    pvPRB_XPullPs            ,
    pvPRB_XBackPs            ,
    pvPRB_XIdCheck           ,
    pvPRB_XOut1Ps            ,
    pvPRB_XOut2Ps            ,
    MAX_PSTN_MOTR2           ,

    pvWRK_XWaitPs       =  0 ,
    pvWRK_XGripPs            ,
    pvWRK_XWorkStartPs       ,
    pvWRK_XOutPs             ,
    MAX_PSTN_MOTR3           ,

    pvPSB_XWaitPs       = 0  ,
    pvPSB_XGripPs            ,
    pvPSB_XPullPs            ,
    pvPSB_XBackPs            ,
    pvPSB_XOutPs             ,
    MAX_PSTN_MOTR4           ,

    pvWRK_ZTWaitPs      =  0 ,
    pvWRK_ZTWorkPs           ,
    MAX_PSTN_MOTR5           ,

    pvWRK_ZBWaitPs      =  0 ,
    pvWRK_ZBWorkPs           ,
    MAX_PSTN_MOTR6           ,


};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_Home             = 10 ,
    mcLDR_Supply                ,
    mcLDR_Pick                  ,
    mcLDR_SttPstn               ,
    mcLDR_Place                 ,
    mcLDR_UpSlPitch             ,
    mcLDR_DnSlPitch             ,
    mcLDR_PusherFw              ,
    mcLDR_PusherBw              ,
    mcLDR_UnClamp               ,
    mcLDR_Clamp                 ,
    mcLDR_TranInAC              ,
    mcLDR_TranOutAC             ,

    mcPRE_Home             = 30 ,
    mcPRE_In                    ,
    mcPRE_Drag                  ,
    mcPRE_Out                   ,
    mcPRE_Clamp                 ,
    mcPRE_IDCyl                 ,
    mcPRE_Feeding               ,

    mcWRK_Home             = 40 ,
    mcWRK_PreWork               ,
    mcWRK_R1Pitch               ,
    mcWRK_L1Pitch               ,
    mcWRK_PostWork              ,
    mcWRK_Clamp                 ,
    mcWRK_ToolChng              ,
    mcWRK_RptCntct              ,

    mcPST_Home             = 50 ,
    mcPST_Drag                  ,
    mcPST_Out                   ,
    mcPST_Clamp                 ,
    mcPST_4                     ,
    mcPST_5                     ,
    mcPST_6                     ,

    mcETC_AllClampOff      = 60 ,
    mcETC_RemoveStrip           ,
    mcETC_PreRemoveStrip        ,
    mcETC_WrkRemoveStrip        ,
    mcETC_PstRemoveStrip        ,
    mcETC_AirBlower             ,
    mcETC_6                     ,

    mcETC_PrbStt           = 90 ,
    mcETC_TPrbUp                ,
    mcETC_TPrbDn                ,
    mcETC_BPrbUp                ,
    mcETC_BPrbDn                ,

    MAX_MANUAL_CYCLE
};

#endif







































































/*
Work Zone Strip Unknown Error

Work Zone Strip Disappearance.
Plz Check Detection Sensor.

                */
























