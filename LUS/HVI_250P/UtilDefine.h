#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_VISN 4

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum EN_ARAY_ID {
    riINB     =0 , //들어오자 마자.
    riPRB        , //들어온놈 플리퍼에 들어갔을때.
    riPRI        , //프리 인덱스가 찝었을때.
    riPSI        , //포스트 인덱스가 찝었을때.
    MAX_ARAY
};

enum EN_MOTR_ID {
    miWRK_ZPch  =0 , //
    miPRB_XIdx     , //
    miPRB_TFlp     , //
    miWRK_XSrt     , // Bin 통에 Sorting 해주는 모터요. dd
    miPRI_XIdx     , //
    miPSI_XIdx     , //
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiPRB_Idx     =0  ,     //
    aiPRI_Idx         ,     //
    aiPSI_Idx         ,     //
    MAX_ACTR
};

enum EN_ERR_ID {
    eiETC_MainAir          =  0 ,   //  000
    eiETC_ToStartTO             ,   //  001
    eiETC_ToStopTO              ,   //  002
    eiETC_Emergency             ,   //  003
    eiETC_FDoor                 ,   //  004
    eiETC_BDoor                 ,   //  005
    eiETC_RDoor                 ,   //  006
    eiETC_LotEnd                ,   //  007
    eiETC_AllHomeTO             ,   //  008
    eiETC_ManCycleTO            ,   //  009
    ei010                       ,   //  010
    eiPRB_CycleTO               ,   //  011
    eiPRI_CycleTO               ,   //  012
    eiWRK_CycleTO               ,   //  013
    eiPSI_CycleTO               ,   //  014
    ei015                       ,   //  015
    eiPRB_HomeTO                ,   //  016
    eiPRI_HomeTO                ,   //  017
    eiWRK_HomeTO                ,   //  018
    eiPSI_HomeTO                ,   //  019
    ei020                       ,   //  020
    eiPRB_ToStartTO             ,   //  021
    eiPRI_ToStartTO             ,   //  022
    eiWRK_ToStartTO             ,   //  023
    eiPSI_ToStartTO             ,   //  024
    ei025                       ,   //  025
    eiPRB_ToStopTO              ,   //  026
    eiPRI_ToStopTO              ,   //  027
    eiWRK_ToStopTO              ,   //  028
    eiPSI_ToStopTO              ,   //  029
    ei030                       ,   //  030
    ei031                       ,   //  031
    eiWRK_ZPchHomeEnd           ,   //  032
    eiWRK_ZPchNegLim            ,   //  033
    eiWRK_ZPchPosLim            ,   //  034
    eiWRK_ZPchAlarm             ,   //  035
    ei036                       ,   //  036
    eiPRB_XIdxHomeEnd           ,   //  037
    eiPRB_XIdxNegLim            ,   //  038
    eiPRB_XIdxPosLim            ,   //  039
    eiPRB_XIdxAlarm             ,   //  040
    ei041                       ,   //  041
    eiPRB_TFlpHomeEnd           ,   //  042
    eiPRB_TFlpNegLim            ,   //  043
    eiPRB_TFlpPosLim            ,   //  044
    eiPRB_TFlpAlarm             ,   //  045
    ei046                       ,   //  046
    eiWRK_XSrtHomeEnd           ,   //  047
    eiWRK_XSrtNegLim            ,   //  048
    eiWRK_XSrtPosLim            ,   //  049
    eiWRK_XSrtAlarm             ,   //  050
    ei051                       ,   //  051
    eiPRI_XIdxHomeEnd           ,   //  052
    eiPRI_XIdxNegLim            ,   //  053
    eiPRI_XIdxPosLim            ,   //  054
    eiPRI_XIdxAlarm             ,   //  055
    ei056                       ,   //  056
    eiPSI_XIdxHomeEnd           ,   //  057
    eiPSI_XIdxNegLim            ,   //  058
    eiPSI_XIdxPosLim            ,   //  059
    eiPSI_XIdxAlarm             ,   //  060
    ei061                       ,   //  061
    eiPRB_IdxTo                 ,   //  062
    eiWRK_1IdxTo                ,   //  063
    eiWRK_2IdxTo                ,   //  064
    ei065                       ,   //  065
    ei066                       ,   //  066
    eiINB_PkgDispr              ,   //  067
    eiINB_PkgUnknown            ,   //  068
    eiPRB_PkgDispr              ,   //  069
    eiPRB_PkgUnknown            ,   //  070
    eiPRI_PkgDispr              ,   //  071
    eiPRI_PkgUnknown            ,   //  072
    eiPSI_PkgDispr              ,   //  073
    eiPSI_PkgUnknown            ,   //  074
    ei075                       ,   //  075
    ei076                       ,   //  076
    eiPRB_IdxOverLoad           ,   //  077
    eiPRI_IdxOverLoad           ,   //  078
    eiPSI_IdxOverLoad           ,   //  079
    eiWRK_Bin1Full              ,   //  080
    eiWRK_Bin2Full              ,   //  081
    eiWRK_Bin3Full              ,   //  082
    eiWRK_Bin4Full              ,   //  083
    eiWRK_Bin5Full              ,   //  084
    eiWRK_BinTFull              ,   //  085
    eiPRB_FlpSsr1Detected       ,   //  086
    eiPRB_FlpSsr2Detected       ,   //  087
    eiWRK_ToolSsrNotDectected   ,   //  088
    eiETC_AreaSsrDectected      ,   //  089
    eiPSI_OutSensorDetected     ,   //  090
    eiETC_PreEqpRead            ,   //  091
    ei092                       ,   //  092
    ei093                       ,   //  093
    ei094                       ,   //  094
    eiWRK_Bin1Dispr             ,   //  095
    eiWRK_Bin2Dispr             ,   //  096
    eiWRK_Bin3Dispr             ,   //  097
    eiWRK_Bin4Dispr             ,   //  098
    eiWRK_Bin5Dispr             ,   //  099
    MAX_ERR
};

enum EN_INPUT_ID {
    x0000 =  0 ,  xETC_StartSw           =  0 , //
    x0001 =  1 ,  xETC_StopSw            =  1 , //
    x0002 =  2 ,  xETC_ResetSw           =  2 , //
    x0003 =  3 ,  xETC_AirSw             =  3 , //
    x0004 =  4 ,  xETC_InitSw            =  4 , //
    x0005 =  5 ,  x005                   =  5 , //
    x0006 =  6 ,  x006                   =  6 , //
    x0007 =  7 ,  x007                   =  7 , //
    x0008 =  8 ,  xPRB_Strip1            =  8 , //
    x0009 =  9 ,  xPRI_Strip4            =  9 , //
    x000A = 10 ,  xPSI_Strip5            = 10 , //
    x000B = 11 ,  x011                   = 11 , //
    x000C = 12 ,  xETC_EmgSw1            = 12 , //
    x000D = 13 ,  xETC_EmgSw2            = 13 , //
    x000E = 14 ,  x014                   = 14 , //
    x000F = 15 ,  x015                   = 15 , //

    x0010 = 16 ,  xETC_PowerOn           = 16 , //
    x0011 = 17 ,  xETC_MainSol           = 17 , //
    x0012 = 18 ,  xETC_AreaFront         = 18 , //
    x0013 = 19 ,  xETC_DoorFront         = 19 , //
    x0014 = 20 ,  xETC_DoorBack          = 20 , //
    x0015 = 21 ,  xETC_DoorRear          = 21 , //
    x0016 = 22 ,  x022                   = 22 , //
    x0017 = 23 ,  x023                   = 23 , //
    x0018 = 24 ,  xPRB_Strip2            = 24 , //
    x0019 = 25 ,  xPRI_Strip3            = 25 , //
    x001A = 26 ,  xPSB_Strip6            = 26 , //
    x001B = 27 ,  xPRB_IdxUp             = 27 , //
    x001C = 28 ,  xPRB_IdxDn             = 28 , //
    x001D = 29 ,  xPRB_Strip             = 29 , // 애가 도면에 Turning Ssr 라고 되어 있는 애임.
    x001E = 30 ,  xPRB_IdxOverLoad       = 30 , //
    x001F = 31 ,  x031                   = 31 , //

    x0020 = 32 ,  xWRK_ToolDetected      = 32 , //
    x0021 = 33 ,  xWRK_2IdxOverload2     = 33 , //
    x0022 = 34 ,  xWRK_1IdxUp            = 34 , //
    x0023 = 35 ,  xWRK_1IdxDn            = 35 , //
    x0024 = 36 ,  xWRK_2IdxUp            = 36 , //
    x0025 = 37 ,  xWRK_2IdxDn            = 37 , //
    x0026 = 38 ,  xWRK_1IdxOverload      = 38 , //
    x0027 = 39 ,  xWRK_2IdxOverload      = 39 , //
    x0028 = 40 ,  xWRK_Bin1              = 40 , //
    x0029 = 41 ,  xWRK_Bin2              = 41 , //
    x002A = 42 ,  xWRK_Bin3              = 42 , //
    x002B = 43 ,  xWRK_Bin4              = 43 , //
    x002C = 44 ,  xWRK_Bin5              = 44 , //
    x002D = 45 ,  x045                   = 45 , //
    x002E = 46 ,  x046                   = 46 , //
    x002F = 47 ,  x047                   = 47 , //

    x0030 = 48 ,  x048                   = 48 , //
    x0031 = 49 ,  x049                   = 49 , //
    x0032 = 50 ,  x050                   = 50 , //
    x0033 = 51 ,  x051                   = 51 , //
    x0034 = 52 ,  x052                   = 52 , //
    x0035 = 53 ,  x053                   = 53 , //
    x0036 = 54 ,  x054                   = 54 , //
    x0037 = 55 ,  x055                   = 55 , //
    x0038 = 56 ,  xETC_PreEqpStart       = 56 , //
    x0039 = 57 ,  xETC_PreEqpStop        = 57 , // IO 어디에 쳐 밖혀 잇는지 알수가 없어서 일단 여기다 쳐넣음.
    x003A = 58 ,  xETC_PreEqpReset       = 58 , //
    x003B = 59 ,  xETC_PreEqpLotStart    = 59 , //
    x003C = 60 ,  xETC_PreEqpLotEnd      = 60 , ////xWR1_1Pkg                = 60 , //
    x003D = 61 ,  xETC_PreEqpSending     = 61 , //
    x003E = 62 ,  xETC_PreEqpJobChange   = 62 , //
    x003F = 63 ,  x063                   = 63 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yETC_StartLp                =  0 , //
    y0001 =  1 ,  yETC_StopLp                 =  1 , //
    y0002 =  2 ,  yETC_ResetLp                =  2 , //
    y0003 =  3 ,  yETC_AirLp                  =  3 , //
    y0004 =  4 ,  yETC_InitLp                 =  4 , //
    y0005 =  5 ,  y005                        =  5 , //
    y0006 =  6 ,  y006                        =  6 , //
    y0007 =  7 ,  y007                        =  7 , //
    y0008 =  8 ,  yETC_TwRedLp                =  8 , //
    y0009 =  9 ,  yETC_TwYelLp                =  9 , //
    y000A = 10 ,  yETC_TwGrnLp                = 10 , //
    y000B = 11 ,  yETC_TwBzz                  = 11 , //
    y000C = 12 ,  y012                        = 12 , //
    y000D = 13 ,  yETC_MainSol                = 13 , //
    y000E = 14 ,  y014                        = 14 , //
    y000F = 15 ,  yWRK_ZBreak                 = 15 , //

    y0010 = 16 ,  yPRB_Idx                    = 16 , //
    y0011 = 17 ,  yWRK_1Idx                   = 17 , //
    y0012 = 18 ,  yWRK_2Idx                   = 18 , //
    y0013 = 19 ,  yWRK_PunchAir               = 19 , //
    y0014 = 20 ,  y020                        = 20 , //
    y0015 = 21 ,  y021                        = 21 , //
    y0016 = 22 ,  y022                        = 22 , //
    y0017 = 23 ,  y023                        = 23 , //
    y0018 = 24 ,  yETC_PreEqpStart            = 24 , //
    y0019 = 25 ,  yETC_PreEqpStop             = 25 , //
    y001A = 26 ,  yETC_PreEqpReset            = 26 , //
    y001B = 27 ,  yETC_PreEqpReady            = 27 , //
    y001C = 28 ,  y028                        = 28 , //
    y001D = 29 ,  y029                        = 29 , //
    y001E = 30 ,  y030                        = 30 , //
    y001F = 31 ,  y031                        = 31 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID {
    piWRK_ZPchWait     = 0 ,
    piWRK_ZPchWork         ,
    MAX_PSTN_ID_MOTR0      ,

    piPRB_XIdxWait     = 0 ,
    piPRB_XIdxTurn         ,
    piPRB_XIdxOut          ,
    MAX_PSTN_ID_MOTR1      ,

    piPRB_TFlpWait     = 0 ,
    piPRB_TFlpNormal       ,
    piPRB_TFlpInverse      ,
    MAX_PSTN_ID_MOTR2      ,

    piWRK_XSrtWait     = 0 ,
    piWRK_XSrtBin1         ,
    piWRK_XSrtBin2         ,
    piWRK_XSrtBin3         ,
    piWRK_XSrtBin4         ,
    piWRK_XSrtBin5         ,
    MAX_PSTN_ID_MOTR3      ,

    piPRI_XIdxWait     = 0 ,
    piPRI_XIdxWorkStt      ,
//    piPRI_XIdxRework1      ,
//    piPRI_XIdxRework2      ,
    piPRI_XIdxOut          ,
    piPRI_XIdxWork         ,
    MAX_PSTN_ID_MOTR4      ,

    piPSI_XIdxWait     = 0 ,
    piPSI_XIdxWorkStt      ,
    piPSI_XIdxPull         ,
    piPSI_XIdxBack         ,
    piPSI_XIdxOut          ,
    piPSI_XIdxWork         ,
    MAX_PSTN_ID_MOTR5      ,
};

enum EN_PSTN_VALUE {
    pvWRK_ZPchWait     = 0 ,
    pvWRK_ZPchWork         ,
    MAX_PSTN_MOTR0         ,

    pvPRB_XIdxWait     = 0 ,
    pvPRB_XIdxTurn         ,
    pvPRB_XIdxOut          ,
    MAX_PSTN_MOTR1         ,

    pvPRB_TFlpWait     = 0 ,
    pvPRB_TFlpNormal       ,
    pvPRB_TFlpInverse      ,
    MAX_PSTN_MOTR2         ,

    pvWRK_XSrtWait     = 0 ,
    pvWRK_XSrtBin1         ,
    ovWRK_XSrtPitch        , //피치 오프셑.
    MAX_PSTN_MOTR3         ,

    pvPRI_XIdxWait     = 0 ,
    pvPRI_XIdxWork         ,
//    pvPRI_XIdxRework1      ,
//    pvPRI_XIdxRework2      ,
    pvPRI_XIdxOut          ,
    MAX_PSTN_MOTR4         ,

    pvPSI_XIdxWait     = 0 ,
    pvPSI_XIdxWork         ,
    pvPSI_XIdxPull         ,
    pvPSI_XIdxBack         ,
    pvPSI_XIdxOut          ,
    MAX_PSTN_MOTR5         ,

};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcPRB_Home             = 10 , //10
    mcPRB_FlpNormal             , //11
    mcPRB_FlpInverse            , //12

    mcPRB_IdxUp                 , //13
    mcPRB_IdxDn                 , //14
    mcPRB_IdxWait               , //15
    mcPRB_IdxStrip              , //16
    mcPRB_IdxOut                , //17

    mcPRI_Home                  , //18
    mcPRI_XIdxWait              , //19
    mcPRI_XIdxWork              , //20
    mcPRI_XIdxOut               , //21
    mcPRI_IdxUp                 , //22
    mcPRI_IdxDn                 , //23
    mcPRI_IdxStripReMv          , //24

    mcWRK_Home                  , //25
    mcWRK_XSrtBin1              , //26
    mcWRK_XSrtBin2              , //27
    mcWRK_XSrtBin3              , //28
    mcWRK_XSrtBin4              , //29
    mcWRK_XSrtBin5              , //30
    mcWRK_PunchUp               , //31

    mcPSI_Home                  , //32
    mcPSI_XIdxWait              , //33
    mcPSI_XIdxPull              , //34
    mcPSI_XIdxBack              , //35
    mcPSI_XIdxOut               , //36
    mcPSI_IdxUp                 , //37
    mcPSI_IdxDn                 , //38

    MAX_MANUAL_CYCLE
};
#endif


