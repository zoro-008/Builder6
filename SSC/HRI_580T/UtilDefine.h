#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_VISN 2   


enum { ccBwd = 0 , ccFwd = 1};
enum { toTabl= 1 , toBuff= 0};
enum EN_ARAY_ID {
    riLDR     = 0 ,
    riPRB         ,
    riWK1         ,
    riVS1         , //비젼 진행용 버퍼. 요놈이 다 Unknwn이 없어져야 다음 싸이클 진행 가능.
    riWK2         ,
    riVS2         , //비젼 진행용 버퍼. 요놈이 다 Unknwn이 없어져야 다음 싸이클 진행 가능.
    riPSB         ,
    riULD         ,
    riWB1         , //Vision 스위치용 버퍼
    riWB2         ,
    riSPC         , //SPC Map View
    riBuf         , //LotNo 자동으로 넣을라고 하나 만듬.   

    MAX_ARAY
};

enum EN_MOTR_ID {
//실제 배열.
    miLDR_ZCmp = 0  , //야스카와             = 1
    miLDR_YCmp = 1  , //오토닉스 스텝핑 로더 = 0
    miWRK_XVsn = 2  , //비젼X       야스카와 = 6
    miWK1_YVsn = 3  , //비젼1Y      야스가와 = 3
    miWK1_ZVsn = 4  , //비젼1Z      이지서보 = 4
    miWK2_YVsn = 5  , //비젼2Y      야스가와 = 5
    miPSB_XTrm = 6  , //리젝X       이지서보 = 9
    miPSB_YTrm = 7  , //리젝Y       이지서보 = 10
    miULD_YCmp = 8  , //언로더Y     오토닉스 = 7
    miULD_ZCmp = 9  , //언로더Z     야스가와 = 8
    miPSB_TSrt = 10 , //리젝소팅T   이지서보.= 2
    miPSB_XIns = 11 , //인서트X     이지서보 = 11

//소프트 배열.
//    miLDR_YCmp = 0  , //오토닉스 스텝핑 로더 = 0
//    miLDR_ZCmp = 1  , //야스카와             = 1
//    miPSB_TSrt = 2  , //리젝소팅T   이지서보.= 2
//    miWK1_YVsn = 3  , //비젼1Y      야스가와 = 3
//    miWK1_ZVsn = 4  , //비젼1Z      이지서보 = 4
//    miWK2_YVsn = 5  , //비젼2Y      야스가와 = 5
//    miWRK_XVsn = 6  , //비젼X       야스카와 = 6
//    miULD_YCmp = 7  , //언로더Y     오토닉스 = 7
//    miULD_ZCmp = 8  , //언로더Z     야스가와 = 8
//    miPSB_XTrm = 9  , //리젝X       이지서보 = 9
//    miPSB_YTrm = 10 , //리젝Y       이지서보 = 10
//    miPSB_XIns = 11 , //인서트X     이지서보 = 11
//
    MAX_MOTR
};

enum EN_INPUT_ID {
    x0000 =  0 , xETC_LStartSw        =  0 ,
    x0001 =  1 , xETC_LStopSw         =  1 ,
    x0002 =  2 , xETC_LResetSw        =  2 ,
    x0003 =  3 , xETC_LAirSw          =  3 ,
    x0004 =  4 , xETC_LInitSw         =  4 ,
    x0005 =  5 , xETC_RStartSw        =  5 ,
    x0006 =  6 , xETC_RStopSw         =  6 ,
    x0007 =  7 , xETC_RResetSw        =  7 ,
    x0008 =  8 , xETC_RAirSw          =  8 ,
    x0009 =  9 , xETC_RInitSw         =  9 ,
    x000A = 10 , xETC_StartSw         = 10 ,
    x000B = 11 , xETC_StopSw          = 11 ,
    x000C = 12 , xETC_ResetSw         = 12 ,
    x000D = 13 , xETC_InkTestSw       = 13 ,
    x000E = 14 , xETC_VisnTestSw      = 14 ,
    x000F = 15 , x015                 = 15 ,

    x0010 = 16 , xETC_EmgSw           = 16 ,
    x0011 = 17 , xETC_PowerOn         = 17 ,
    x0012 = 18 , xETC_Door            = 18 ,
    x0013 = 19 , xETC_MainAir         = 19 ,
    x0014 = 20 , x020                 = 20 ,
    x0015 = 21 , xLDR_PushOver        = 21 ,
    x0016 = 22 , xLDR_PshrFw          = 22 ,
    x0017 = 23 , xLDR_PshrBw          = 23 ,
    x0018 = 24 , xLDR_CmpMgz1         = 24 ,
    x0019 = 25 , xLDR_CmpMgz2         = 25 ,
    x001A = 26 , xLDR_MgzIn           = 26 ,
    x001B = 27 , x027                 = 27 ,
    x001C = 28 , xLDR_MgzOutFull      = 28 ,
    x001D = 29 , x029                 = 29 ,
    x001E = 30 , xLDR_MgzCmpFw        = 30 ,
    x001F = 31 , xLDR_MgzCmpBw        = 31 ,

    x0020 = 32 , xPRB_PkgIn1          = 32 ,
    x0021 = 33 , xPRB_PkgIn2          = 33 ,
    x0022 = 34 , xPRB_Pkg             = 34 ,
    x0023 = 35 , xPRB_StprFw          = 35 ,
    x0024 = 36 , xPRB_StprBw          = 36 ,
    x0025 = 37 , xWK1_Pkg             = 37 ,
    x0026 = 38 , xWK1_StprFw          = 38 ,
    x0027 = 39 , xWK1_StprBw          = 39 ,
    x0028 = 40 , xWK2_Pkg             = 40 ,
    x0029 = 41 , xWK2_StprFw          = 41 ,
    x002A = 42 , xWK2_StprBw          = 42 ,
    x002B = 43 , xWK2_Out             = 43 , //WK2영역에서 스트립이 나간것 확인 하고 그립으로 스트립을 정지 시키는 센서.
    x002C = 44 , xPSB_TrimCheck       = 44 , //Trim Check
    x002D = 45 , xPSB_Pkg             = 45 ,
    x002E = 46 , xPSB_TrimFw          = 46 ,
    x002F = 47 , xPSB_TrimBw          = 47 ,

    x0030 = 48 , xWK1_Align           = 48 ,
    x0031 = 49 , xWK1_LiftFw          = 49 ,
    x0032 = 50 , xWK1_LiftBw          = 50 ,
    x0033 = 51 , xWK2_Align           = 51 ,
    x0034 = 52 , xWK2_LiftFw          = 52 ,
    x0035 = 53 , xWK2_LiftBw          = 53 ,
    x0036 = 54 , xHED_UVLightFw       = 54 ,
    x0037 = 55 , xHED_UVLightBw       = 55 ,
    x0038 = 56 , xPSB_Bin1            = 56 ,
    x0039 = 57 , xPSB_Bin2            = 57 ,
    x003A = 58 , xPSB_Bin3            = 58 ,
    x003B = 59 , xPSB_OutPshrUp       = 59 ,
    x003C = 60 , xPSB_OutPshrDn       = 60 ,
    x003D = 61 , xPSB_PkgDrop         = 61 ,
    x003E = 62 , xPSB_OutPshrFw       = 62 ,
    x003F = 63 , xPSB_OutPshrBw       = 63 ,

    x0040 = 64 , xULD_CmpMgz1         = 64 ,
    x0041 = 65 , xULD_CmpMgz2         = 65 ,
    x0042 = 66 , xPSB_Algn1           = 66 ,
    x0043 = 67 , xPSB_Algn2           = 67 ,
    x0044 = 68 , xPSB_FlprFw          = 68 ,
    x0045 = 69 , xPSB_FlprBw          = 69 ,
    x0046 = 70 , xPSB_IndxUp          = 70 ,
    x0047 = 71 , xPSB_IndxDn          = 71 ,
    x0048 = 72 , xULD_MgzOutFull      = 72 ,
    x0049 = 73 , x049                 = 73 ,
    x004A = 74 , xULD_MgzIn           = 74 ,
    x004B = 75 , x050                 = 75 ,
    x004C = 76 , xULD_MgzCmpFw        = 76 ,
    x004D = 77 , xULD_MgzCmpBw        = 77 ,
    x004E = 78 , xPSB_PkgOut1         = 78 ,
    x004F = 79 , xPSB_PkgOut2         = 79 ,

    x0050 = 80 , xHED_1Ready          = 80 ,
    x0051 = 81 , xHED_1Busy           = 81 ,
    x0052 = 82 , xHED_1Result         = 82 ,
    x0053 = 83 , xETC_PreEqpStart     = 83 ,
    x0054 = 84 , xETC_PreEqpStop      = 84 ,
    x0055 = 85 , xETC_PreEqpReset     = 85 ,
    x0056 = 86 , xETC_PreEqpLotStart  = 86 ,
    x0057 = 87 , xETC_PreEqpLotEnd    = 87 ,
    x0058 = 88 , xHED_2Ready          = 88 ,
    x0059 = 89 , xHED_2Busy           = 89 ,
    x005A = 90 , xHED_2Result         = 90 ,
    x005B = 91 , xETC_PreEqpSending   = 91 ,
    x005C = 92 , xETC_PreEqpJobChange = 92 ,
    x005D = 93 , xETC_PreEqpSpare     = 93 ,
    x005E = 94 , x094                 = 94 ,
    x005F = 95 , x095                 = 95 ,
    MAX_INPUT
};

enum EN_OUTPUT_ID {
    y0000 =  0 ,  yETC_LStartLp     =  0 , //
    y0001 =  1 ,  yETC_LStopLp      =  1 , //
    y0002 =  2 ,  yETC_LResetLp     =  2 , //
    y0003 =  3 ,  yETC_LAirLp       =  3 , //
    y0004 =  4 ,  yETC_LInitLp      =  4 , //
    y0005 =  5 ,  yETC_RStartLp     =  5 , //
    y0006 =  6 ,  yETC_RStopLp      =  6 , //
    y0007 =  7 ,  yETC_RResetLp     =  7 , //
    y0008 =  8 ,  yETC_RAirLp       =  8 , //
    y0009 =  9 ,  yETC_RInitLp      =  9 , //
    y000A = 10 ,  yETC_StartLp      = 10 , //
    y000B = 11 ,  yETC_StopLp       = 11 , //
    y000C = 12 ,  yETC_ResetLp      = 12 , //
    y000D = 13 ,  yETC_InkTestLp    = 13 , //
    y000E = 14 ,  yETC_VisnTestLp   = 14 , //
    y000F = 15 ,  y015              = 15 , //

    y0010 = 16 ,  yETC_TwRedLp      = 16 , //
    y0011 = 17 ,  yETC_TwYelLp      = 17 , //
    y0012 = 18 ,  yETC_TwGrnLp      = 18 , //
    y0013 = 19 ,  yETC_MainSol      = 19 , //
    y0014 = 20 ,  yETC_TwBzz        = 20 , //
    y0015 = 21 ,  y021              = 21 , //
    y0016 = 22 ,  y022              = 22 , //
    y0017 = 23 ,  y023              = 23 , //
    y0018 = 24 ,  yLDR_MgzInAC      = 24 , //
    y0019 = 25 ,  yLDR_MgzOutAC     = 25 , //
    y001A = 26 ,  yLDR_MgzCmpBw     = 26 , //
    y001B = 27 ,  yLDR_MgzCmpFw     = 27 , //
    y001C = 28 ,  yLDR_PshrFw       = 28 , //
    y001D = 29 ,  yPRB_Belt         = 29 , //
    y001E = 30 ,  yWRK_Belt         = 30 , //
    y001F = 31 ,  y031              = 31 , //

    y0020 = 32 ,  yPRB_StprFw       = 32 , //
    y0021 = 33 ,  yWK1_StprFw       = 33 , //
    y0022 = 34 ,  yWK2_StprFw       = 34 , //
    y0023 = 35 ,  y035              = 35 , //
    y0024 = 36 ,  yWK1_Algn         = 36 , //
    y0025 = 37 ,  yWK2_Algn         = 37 , //
    y0026 = 38 ,  yPSB_Algn         = 38 , //
    y0027 = 39 ,  yPRB_AirBlow      = 39 , //
    y0028 = 40 ,  yWK1_LiftFw       = 40 , //
    y0029 = 41 ,  yWK1_LiftBw       = 41 , //
    y002A = 42 ,  yWK2_LiftFw       = 42 , //
    y002B = 43 ,  yWK2_LiftBw       = 43 , //
    y002C = 44 ,  y044              = 44 , //
    y002D = 45 ,  y045              = 45 , //
    y002E = 46 ,  yPSB_PshrUp       = 46 , //
    y002F = 47 ,  yPSB_PshrFw       = 47 , //

    y0030 = 48 ,  yULD_MgzOutAC     = 48 , //
    y0031 = 49 ,  yULD_MgzInAC      = 49 , //
    y0032 = 50 ,  yULD_MgzCmpBw     = 50 , //
    y0033 = 51 ,  yULD_MgzCmpFw     = 51 , //
    y0034 = 52 ,  yLDR_ZBreak       = 52 , //
    y0035 = 53 ,  yULD_ZBreak       = 53 , //
    y0036 = 54 ,  yHED_UVLightBw    = 54 , //
    y0037 = 55 ,  yPSB_TrimFw       = 55 , //
    y0038 = 56 ,  yPSB_FlprFw       = 56 , //
    y0039 = 57 ,  yPSB_FlprBw       = 57 , //
    y003A = 58 ,  yPSB_1            = 58 , //
    y003B = 59 ,  yPSB_2            = 59 , //
    y003C = 60 ,  yPSB_FlprCmpFw    = 60 , //
    y003D = 61 ,  yPSB_SortAir      = 61 , //
    y003E = 62 ,  y062              = 62 , //
    y003F = 63 ,  y063              = 63 , //

    y0040 = 64 ,  yHED_1LotStart    = 64 , //
    y0041 = 65 ,  yHED_1Reset       = 65 , //
    y0042 = 66 ,  yHED_1JobChange   = 66 , //
    y0043 = 67 ,  yHED_1LotEnd      = 67 , //
    y0044 = 68 ,  y068              = 68 , //
    y0045 = 69 ,  y069              = 69 , //
    y0046 = 70 ,  y070              = 70 , //
    y0047 = 71 ,  y071              = 71 , //
    y0048 = 72 ,  yHED_2LotStart    = 72 , //
    y0049 = 73 ,  yHED_2Reset       = 73 , //
    y004A = 74 ,  yHED_2JobChange   = 74 , //
    y004B = 75 ,  yHED_2LotEnd      = 75 , //
    y004C = 76 ,  y076              = 76 , //
    y004D = 77 ,  y077              = 77 , //
    y004E = 78 ,  y078              = 78 , //
    y004F = 79 ,  y079              = 79 , //

    y0050 = 80 ,  yETC_PreEqpStart  = 80 , //
    y0051 = 81 ,  yETC_PreEqpStop   = 81 , //
    y0052 = 82 ,  yETC_PreEqpReset  = 82 , //
    y0053 = 83 ,  yETC_PreEqpReady  = 83 , //
    y0054 = 84 ,  yETC_PreEqpSpare1 = 84 , //
    y0055 = 85 ,  yETC_PreEqpSpare2 = 85 , //
    y0056 = 86 ,  yETC_PreEqpSpare3 = 86 , //
    y0057 = 87 ,  yETC_PreEqpSpare4 = 87 , //
    y0058 = 88 ,  y088              = 88 , //
    y0059 = 89 ,  y089              = 89 , //
    y005A = 90 ,  y090              = 90 , //
    y005B = 91 ,  y091              = 91 , //
    y005C = 92 ,  y092              = 92 , //
    y005D = 93 ,  y093              = 93 , //
    y005E = 94 ,  y094              = 94 , //
    y005F = 95 ,  y095              = 95 , //
    MAX_OUTPUT
};



enum EN_ACTR_ID {
    aiLDR_Cmp      = 0  ,
    aiLDR_Pusher   = 1  ,
    aiPRB_Stopper  = 2  ,
    aiWK1_Align    = 3  ,  //0
    aiWK1_Stopper  = 4  ,
    aiWK1_Lift     = 5  ,
    aiWK2_Align    = 6  ,
    aiWK2_Stopper  = 7  ,
    aiWK2_Lift     = 8  ,
    aiHED_UVLight  = 9  ,
    aiPSB_Align    = 10 ,
    aiPSB_FlprCmp  = 11 ,
    aiPSB_Flpr     = 12 ,
    aiPSB_Trimmer  = 13 ,
    aiPSB_Finger   = 14 ,
    aiPSB_PusherUD = 15 ,
    aiPSB_PusherFB = 16 ,
    aiULD_Cmp      = 17 ,

    MAX_ACTR           //
};

enum EN_ERR_ID {
    eiETC_MainAir          =  0 ,
    eiETC_ToStartTO             ,
    eiETC_ToStopTO              ,
    eiETC_Emergency             ,
    eiETC_Door                  ,
    eiETC_LotEnd                ,
    eiETC_AllHomeTO             ,
    eiETC_ManCycleTO            ,

    eiLDR_CycleTO          =  8 ,
    eiRAL_CycleTO               ,
    eiHED_CycleTO               ,
    eiPSB_CycleTO               ,
    eiULD_CycleTO               ,

    eiLDR_HomeTO           =  13,
    eiRAL_HomeTO                ,
    eiHED_HomeTO                ,
    eiPSB_HomeTO                ,
    eiULD_HomeTO                ,

    eiLDR_ToStartTO        =  18,
    eiRAL_ToStartTO             ,
    eiHED_ToStartTO             ,
    eiPSB_ToStartTO             ,
    eiULD_ToStartTO             ,

    eiLDR_ToStopTO         =  23,
    eiRAL_ToStopTO              ,
    eiHED_ToStopTO              ,
    eiPSB_ToStopTO              ,
    eiULD_ToStopTO              ,

    eiLDR_YCmpHomeEnd      =  28,
    eiLDR_YCmpNegLim            ,
    eiLDR_YCmpPosLim            ,
    eiLDR_YCmpAlarm             ,

    eiLDR_ZCmpHomeEnd           ,
    eiLDR_ZCmpNegLim            ,
    eiLDR_ZCmpPosLim            ,
    eiLDR_ZCmpAlarm             ,

    eiPSB_TSrtHomeEnd           ,
    eiPSB_TSrtNegLim            ,
    eiPSB_TSrtPosLim            ,
    eiPSB_TSrtAlarm             ,

    eiWK1_YVsnHomeEnd           ,
    eiWK1_YVsnNegLim            ,
    eiWK1_YVsnPosLim            ,
    eiWK1_YVsnAlarm             ,

    eiWK1_ZVsnHomeEnd           ,
    eiWK1_ZVsnNegLim            ,
    eiWK1_ZVsnPosLim            ,
    eiWK1_ZVsnAlarm             ,

    eiWK2_YVsnHomeEnd           ,
    eiWK2_YVsnNegLim            ,
    eiWK2_YVsnPosLim            ,
    eiWK2_YVsnAlarm             ,

    eiWRK_XVsnHomeEnd           ,
    eiWRK_XVsnNegLim            ,
    eiWRK_XVsnPosLim            ,
    eiWRK_XVsnAlarm             ,

    eiULD_YCmpHomeEnd           ,
    eiULD_YCmpNegLim            ,
    eiULD_YCmpPosLim            ,
    eiULD_YCmpAlarm             ,

    eiULD_ZCmpHomeEnd           ,
    eiULD_ZCmpNegLim            ,
    eiULD_ZCmpPosLim            ,
    eiULD_ZCmpAlarm             ,

    eiPSB_XTrmHomeEnd           ,
    eiPSB_XTrmNegLim            ,
    eiPSB_XTrmPosLim            ,
    eiPSB_XTrmAlarm             ,

    eiPSB_YTrmHomeEnd           ,
    eiPSB_YTrmNegLim            ,
    eiPSB_YTrmPosLim            ,
    eiPSB_YTrmAlarm             ,

    eiPSB_XCmpHomeEnd           ,
    eiPSB_XCmpNegLim            ,
    eiPSB_XCmpPosLim            ,
    eiPSB_XCmpAlarm             ,

    eiPSB_XInsHomeEnd           ,
    eiPSB_XInsNegLim            ,
    eiPSB_XInsPosLim            ,
    eiPSB_XInsAlarm             ,

    eiLDR_CmpTO                 ,
    eiLDR_PusherTO              ,
    eiPRB_StopperTO             ,
    eiWK1_AlignTO               ,
    eiWK1_StopperTO             ,
    eiWK1_LiftTO                ,
    eiWK2_AlignTO               ,
    eiWK2_StopperTO             ,
    eiWK2_LiftTO                ,
    eiHED_UVLightTO             ,
    eiPSB_AlignTO               ,
    eiPSB_FlprCmpTO             ,
    eiPSB_FlprTO                ,
    eiPSB_TrimmerTO             ,
    eiPSB_FingerTO              ,
    eiPSB_PusherUDTO            ,
    eiPSB_PusherFBTO            ,
    eiULD_CmpTO                 ,

    eiLDR_MgzDispr              ,
    eiLDR_MgzUnknown            ,
    eiPRB_PkgDispr              ,
    eiPRB_PkgUnknown            ,
    eiWK1_PkgDispr              ,
    eiWK1_PkgUnknown            ,
    eiWK2_PkgDispr              ,
    eiWK2_PkgUnknown            ,
    eiPSB_PkgDispr              ,
    eiPSB_PkgUnknown            ,
    eiULD_MgzUnknown            ,
    eiULD_MgzDispr              ,

    eiWK1_VsnFailCnt            ,
    eiWK2_VsnFailCnt            ,
    eiLDR_PshrOverload          ,
    eiHED_Visn1NotReady         ,
    eiHED_Visn2NotReady         ,
    eiPSB_NoRjBox1              ,
    eiPSB_NoRjBox2              ,
    eiPSB_NoRjBox3              ,
    eiPSB_FullRjBox             ,
    eiPRB_FileNotExst           ,
    eiLDR_MgzFull               ,
    eiULD_MgzFull               ,

    eiETC_PreEqpRead            ,
    eiHED_Visn1Read             ,
    eiHED_Visn2Read             ,
    eiHED_VisnResetCnt          ,
    eiHED_VisnCommTO            ,
    eiULD_NeedMgz               ,
    eiETC_LotOverMgz            ,
    eiETC_PreEqpNoJob           ,
    eiWK1_VsnSFailCnt           ,
    eiWK2_VsnSFailCnt           ,

    eiPSB_PusherMiss            ,
    eiPSB_TrimCheck             ,    

    MAX_ERR
};

//enum EN_MAN_ID {
//    miLT_Home = 0  ,
//
//    MAX_MAN
//};


enum EN_PSTN_ID {

    piLDR_ZCmpWait    = 0 ,
    piLDR_ZCmpPickFwd     ,
    piLDR_ZCmpClampOn     ,
    piLDR_ZCmpPickBwd     ,
    piLDR_ZCmpWorkStt     ,
    piLDR_ZCmpPlceFwd     ,
    piLDR_ZCmpClampOff    ,
    piLDR_ZCmpPlceBwd     ,

    piLDR_ZCmpWork        ,
    MAX_PSTN_ID_MOTR0     ,

    piLDR_YCmpWait    = 0 ,
    piLDR_YCmpPick        ,
    piLDR_YCmpWork        ,
    piLDR_YCmpPlce        ,
    MAX_PSTN_ID_MOTR1     ,

    piWRK_XVsnWait    = 0 ,
    piWRK_XVsnWorkStt     ,
    piWRK_XVsnTrgStt      ,
    piWRK_XVsnTrgEnd      ,
    MAX_PSTN_ID_MOTR2     ,

    piWK1_YVsnWait    = 0 ,
    piWK1_YVsnWorkStt     ,
    piWK1_YVsnWork        ,
    MAX_PSTN_ID_MOTR3     ,

    piWK1_ZVsnWait    = 0 ,
    piWK1_ZVsnWork        ,
    piWK1_ZVsnUVWork      ,
    MAX_PSTN_ID_MOTR4     ,

    piWK2_YVsnWait    = 0 ,
    piWK2_YVsnWorkStt     ,
    piWK2_YVsnWork        ,
    MAX_PSTN_ID_MOTR5     ,

    piPSB_XTrmWait    = 0 ,
    piPSB_XTrmWorkStt     ,
    piPSB_XTrmCheck       ,
    piPSB_XTrmRslt0       ,
    piPSB_XTrmRslt1       ,
    piPSB_XTrmRslt2       ,
    piPSB_XTrmRslt3       ,
    piPSB_XTrmRslt4       ,
    piPSB_XTrmRslt5       ,
    piPSB_XTrmRslt6       ,
    piPSB_XTrmRslt7       ,
    piPSB_XTrmRslt8       ,
    piPSB_XTrmRslt9       ,
    piPSB_XTrmRslt10      ,
    piPSB_XTrmRslt11      ,
    piPSB_XTrmRslt12      ,
    piPSB_XTrmRslt13      ,
    piPSB_XTrmRslt14      ,
    piPSB_XTrmRslt15      ,
    piPSB_XTrmRslt16      ,
    piPSB_XTrmFail        ,
    MAX_PSTN_ID_MOTR6     ,

    piPSB_YTrmWait    = 0 ,
    piPSB_YTrmWorkStt     ,
//    piPSB_YTrmCheck       ,
    piPSB_YTrmRslt0       ,
    piPSB_YTrmRslt1       ,
    piPSB_YTrmRslt2       ,
    piPSB_YTrmRslt3       ,
    piPSB_YTrmRslt4       ,
    piPSB_YTrmRslt5       ,
    piPSB_YTrmRslt6       ,
    piPSB_YTrmRslt7       ,
    piPSB_YTrmRslt8       ,
    piPSB_YTrmRslt9       ,
    piPSB_YTrmRslt10      ,
    piPSB_YTrmRslt11      ,
    piPSB_YTrmRslt12      ,
    piPSB_YTrmRslt13      ,
    piPSB_YTrmRslt14      ,
    piPSB_YTrmRslt15      ,
    piPSB_YTrmRslt16      ,
    piPSB_YTrmFail        ,
    MAX_PSTN_ID_MOTR7     ,

    piULD_YCmpWait    = 0 ,
    piULD_YCmpPick        ,
    piULD_YCmpWork        ,
    piULD_YCmpPlce        ,
    MAX_PSTN_ID_MOTR8     ,

    piULD_ZCmpWait    = 0 ,
    piULD_ZCmpPickFwd     ,
    piULD_ZCmpClampOn     ,
    piULD_ZCmpPickBwd     ,
    piULD_ZCmpWorkStt     ,
    piULD_ZCmpPlceFwd     ,
    piULD_ZCmpClampOff    ,
    piULD_ZCmpPlceBwd     ,

    piULD_ZCmpWork        ,
    MAX_PSTN_ID_MOTR9     ,

    piPSB_TSrtWait    = 0 ,
    piPSB_TSrtRjc0        ,
    piPSB_TSrtRjc1        ,
    piPSB_TSrtRjc2        ,
    MAX_PSTN_ID_MOTR10    ,

    piPSB_XInsWait    = 0 ,
    piPSB_XInsInsrt       ,
    piPSB_XInsOut         ,
    MAX_PSTN_ID_MOTR11

};
/*
miLDR_ZCmp
miLDR_YCmp
miWRK_XVsn
miWK1_YVsn
miWK1_ZVsn
miWK2_YVsn

miPSB_XTrm
miPSB_YTrm

miULD_YCmp
miULD_ZCmp
miPSB_TSrt
miPSB_XIns
*/
enum EN_PSTN_VALUE {
    pvLDR_ZCmpWaitPs    = 0 ,
    pvLDR_ZCmpPickFwdPs     ,
    pvLDR_ZCmpClampOnPs     ,
    pvLDR_ZCmpPickBwdPs     ,
    pvLDR_ZCmpWorkSttPs     ,
    pvLDR_ZCmpPlceFwdPs     ,
    pvLDR_ZCmpClampOffPs    ,
    pvLDR_ZCmpPlceBwdPs     ,
    MAX_PSTN_MOTR0          ,

    pvLDR_YCmpWaitPs    = 0 ,
    pvLDR_YCmpPickPs        ,
    pvLDR_YCmpWorkPs        ,
    pvLDR_YCmpPlcePs        ,
    MAX_PSTN_MOTR1          ,

    pvWRK_XVsnWaitPs    = 0 ,
    pvWRK_XVsnWorkSttPs     ,
    MAX_PSTN_MOTR2          ,

    pvWK1_YVsnWaitPs    = 0 ,
    pvWK1_YVsnWorkSttPs     ,
    MAX_PSTN_MOTR3          ,

    pvWK1_ZVsnWaitPs    = 0 ,
    pvWK1_ZVsnWorkPs        ,
    pvWK1_ZVsnUvWorkPs      ,
    MAX_PSTN_MOTR4          ,

    pvWK2_YVsnWaitPs    = 0 ,
    pvWK2_YVsnWorkSttPs     ,
    MAX_PSTN_MOTR5          ,

    pvPSB_XTrmWaitPs    = 0 ,
    pvPSB_XTrmWorkSttPs     ,
    pvPSB_XTrmCheckPs       ,
    MAX_PSTN_MOTR6          ,

    pvPSB_YTrmWaitPs    = 0 ,
    pvPSB_YTrmWorkSttPs     ,
//    pvPSB_YTrmCheckPs       ,
    MAX_PSTN_MOTR7          ,

    pvULD_YCmpWaitPs    = 0 ,
    pvULD_YCmpPickPs        ,
    pvULD_YCmpWorkPs        ,
    pvULD_YCmpPlcePs        ,
    MAX_PSTN_MOTR8          ,

    pvULD_ZCmpWaitPs    = 0 ,
    pvULD_ZCmpPickFwdPs     ,
    pvULD_ZCmpClampOnPs     ,
    pvULD_ZCmpPickBwdPs     ,
    pvULD_ZCmpWorkSttPs     ,
    pvULD_ZCmpPlceFwdPs     ,
    pvULD_ZCmpClampOffPs    ,
    pvULD_ZCmpPlceBwdPs     ,
    MAX_PSTN_MOTR9          ,

    pvPSB_TSrtWaitPs    = 0 ,
    pvPSB_TSrtRjc0Ps        ,
    pvPSB_TSrtRjc1Ps        ,
    pvPSB_TSrtRjc2Ps        ,
    MAX_PSTN_MOTR10         ,

    pvPSB_XInsWaitPs    = 0 ,
    pvPSB_XInsInsrtPs       ,
    pvPSB_XInsOutPs         ,
    MAX_PSTN_MOTR11
};

//---------------------------------------------------------------------------
enum EN_MANUAL_CYCLE
{

    mcNoneCycle         =  0  ,
    mcAllHome           =  1  ,
//    mcETC_VisnTest            ,

    mcLDR_Home          = 10  ,
    mcLDR_Supply              ,
    mcLDR_Pick                ,
    mcLDR_Work                ,
    mcLDR_Place               ,
    mcLDR_InAc                ,
    mcLDR_OutAc               ,
    mcLDR_CmpUD               ,
    mcLDR_PusherFB            ,

    mcRAL_Home          = 30  ,
    mcRAL_In                  ,
    mcRAL_MvAuto              ,
    mcRAL_Bind                ,
    mcRAL_UnBind              ,
    mcRAL_PreStprUD           ,
    mcRAL_Wk1StprUD           ,
    mcRAL_Wk2StprUD           ,
    mcRAL_Wk1LiftUD           ,
    mcRAL_Wk2LiftUD           ,


    mcHED_Home          = 50  ,
    mcHED_Insp                ,
    mcHED_InspMan             ,
    mcHED_PchXp               ,
    mcHED_PchXn               ,
    mcHED_PchYp               ,
    mcHED_PchYn               ,
    mcHED_StartPos            ,

    mcPSB_Home          = 60  ,
    mcPSB_Flip                ,
    mcPSB_Work                ,
    mcPSB_Out                 ,
    mcPSB_FingerFB            ,
    mcPSB_AlignFB             ,
    mcPSB_FlprFB              ,
    mcPSB_FlprCmpFB           ,
    mcPSB_PusherUD            ,
    mcPSB_PusherFB            ,
    mcPSB_WorkStartPos        ,
    mcPSB_PchXp               ,
    mcPSB_PchXn               ,
    mcPSB_PchYp               ,
    mcPSB_PchYn               ,
    mcPSB_TrmFB               ,

    mcULD_Home          = 80  ,
    mcULD_Supply              ,
    mcULD_Pick                ,
    mcULD_Work                ,
    mcULD_Place               ,
    mcULD_InAc                ,
    mcULD_OutAc               ,
    mcULD_CmpUD               ,
    mcULD_WorkPos             ,
    mcULD_CmpPchUp            ,
    mcULD_CmpPchDn            ,

    MAX_MANUAL_CYCLE

/*
    mcNoneCycle = 0    ,
    mcLDR_1     = 1    , //Loading MGZ up / down home
    mcLDR_2     = 2    , //Loading MGZ front / back home
    mcLDR_3     = 3    , //Loading MGZ unclamp
    mcLDR_4     = 4    , //Loading MGZ clamp
    mcLDR_5     = 5    , //Loading MGZ pusher front
    mcLDR_6     = 6    , //Loading MGZ pusher back
    mcLDR_7     = 7    , //Loading MGZ transfer AC motor on/off
    mcLDR_8     = 8    , //Loading MGZ transfer AC motor 1 cycle
    mcLDR_9     = 9    , //Loading MGZ elevator input 1 cycle
    mcLDR_10    = 10   , //Loading MGZ elevator Output 1 cycle
    mcLDR_11    = 11   , //Loading MGZ pusher 1 cycle
    mcLDR_14    = 14   , //Buffer stopper up
    mcLDR_15    = 15   , //Buffer stopper down

    mcVSN_40    = 40   , // First vision left / right home
    mcVSN_41    = 41   , // First vision front / back home
    mcVSN_43    = 43   , // First vision lift up
    mcVSN_44    = 44   , // First vision lift down
    mcVSN_45    = 45   , // First vision stopper up
    mcVSN_46    = 46   , // First vision stopper down
    mcVSN_49    = 49   , // First vision align front
    mcVSN_50    = 50   , // First vision align back
    mcVSN_54    = 54   , // First vision light uv use
    mcVSN_55    = 55   , // First vision light no use
    mcVSN_60    = 60   , // First vision up / down home
    mcVSN_61    = 61   , // Second vision front / back home
    mcVSN_63    = 63   , // Second vision lift up
    mcVSN_64    = 64   , // Second vision lift down
    mcVSN_65    = 65   , // Second vision stopper up
    mcVSN_66    = 66   , // Second vision stopper down
    mcVSN_70    = 70   , // Second vision align front
    mcVSN_71    = 71   , // Second vision align back
    mcVSN_48    = 48   , // vision 1 cycle

    mcMRK_80    = 80   , //Marking left / right home
    mcMRK_81    = 81   , //Marking front / back home
    mcMRK_82    = 82   , //Move marking Init position
    mcMRK_79    = 79   , //Move marking first position
    mcMRK_83    = 83   , //Marking Reverse Pos
    mcMRK_84    = 84   , //Marking Rail Pos
    mcMRK_85    = 85   , //Marking Clamp
    mcMRK_86    = 86   , //Marking UnClamp
    mcMRK_87    = 87   , //Marking cyclinder up
    mcMRK_88    = 88   , //Marking cyclinder down
    mcMRK_89    = 89   , //Marking up / down 1 cycle
    mcMRK_90    = 90   , //Move marking lift / right one step
    mcMRK_91    = 91   , //Move marking front / back one step
    mcMRK_92    = 92   , //Move marking square position
    mcMRK_93    = 93   , //Marking align front
    mcMRK_94    = 94   , //Marking align back
    mcMRK_97    = 97   , //Test marking data

    mcULD_105   = 105  , //Sortin Motor Home
    mcULD_106   = 106  , //Transfer Motor Home
    mcULD_101   = 101  , //Unloading pusher up
    mcULD_102   = 102  , //Unloading pusher down
    mcULD_103   = 103  , //Unloading pusher front
    mcULD_104   = 104  , //Unloading pusher back
    mcULD_100   = 100  , //Unloading pusher 1 cycle
    mcULD_107   = 107  , //Transfer Cyc Down
    mcULD_108   = 108  , //Transfer Cyc Up
    mcULD_110   = 110  , //Unloading MGZ front / back home
    mcULD_111   = 111  , //Unloading MGZ up / down home
    mcULD_112   = 112  , //Unloading MGZ unclamp
    mcULD_113   = 113  , //Unloading MGZ clamp
    mcULD_114   = 114  , //Unloading MGZ AC Motor On/Off
    mcULD_115   = 115  , //Unloading MGZ transfer AC motor 1cycle
    mcULD_116   = 116  , //Unloading MGZ input 1 cycle
    mcULD_117   = 117  , //Unloading MGZ output 1 cycle
    mcULD_118   = 118  , //Unloading MGZ one setp move

    mcETC_120   = 120  , //Remove inside index rail materials
    mcETC_121   = 121  , //Index rail 1 cycle
    mcETC_122   = 122  , //All motor home
    mcAllHome   = 122  ,
    mcETC_48    = 48   , //First vision 1 cycle
    mcETC_125   = 125  , //All belt on
    mcETC_126   = 126  , //All belt off
    mcETC_127   = 127  , //All stopper up
    mcETC_128   = 128  , //All stopper down
    mcETC_129   = 129  , //Align vision1 action
    mcETC_130   = 130  , //Align vision2 action
    mcETC_131   = 131  , //Align marking action
    mcETC_53    = 53   , //First vision reset
    mcETC_74    = 74   , //Second vision reset

    MAX_MANUAL_CYCLE = 132
*/


};

#endif

