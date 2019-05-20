#ifndef UTILDEFINEH
#define UTILDEFINEH

//#define USED_MOTER //모터 사용일떄.
#define USED_UNMTR //모터 사용안할때.

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};


//파트 아이디.
enum EN_PART_ID {
    piLDR_F = 0 ,
    piPRE_F = 1 ,
    piPST_F = 2 ,
    piULD_F = 3 ,
    piLDR_R = 4 ,
    piPRE_R = 5 ,
    piPST_R = 6 ,
    piULD_R = 7 ,

    MAX_PART
};

enum EN_ARAY_ID {
    riLDR_F     = 0,   //Front Loder
    riULD_F     = 1,   //Front UnLoader Mask
    riPCK_F     = 2,   //Front UnLoader Pack
    riLDR_R     = 3,   //Rear Loder
    riULD_R     = 4,   //Rear UnLoader Mask
    riPCK_R     = 5,   //Rear UnLoader Pack
//    riPRE_F     = 1,   //Front Rail 1
//    riPST_F     = 2,   //Front Rail 2
//    riPRE_R     = 6,   //Rear Rail 1
//    riPST_R     = 7,   //Rear Rail 2

    MAX_ARAY
};

enum EN_MOTR_ID { //나중에 전장 도면 보고 순서 수정.
                  //모터 없음..
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_FGrprFwBw         = 0 ,
    aiLDR_FPickUpFwBw       = 1 ,
    aiLDR_FGrprUpDn         = 2 ,
    aiLDR_FGrprXFwBw        = 3 ,
    aiLDR_FLoadingFwBw      = 4 ,
    aiULD_FFoldUpDn         = 5 ,
    aiULD_FPickUpFwBw       = 6 ,
    aiULD_FRotateFwBw       = 7 ,
    aiULD_FPackOpenFwBw     = 8 ,
    aiULD_FPackSrtFwBw      = 9 ,
    
    aiLDR_RGrprFwBw         = 10,
    aiLDR_RPickUpFwBw       = 11,
    aiLDR_RGrprUpDn         = 12,
    aiLDR_RGrprXFwBw        = 13,
    aiLDR_RLoadingFwBw      = 14,
    aiULD_RFoldUpDn         = 15,
    aiULD_RPickUpFwBw       = 16,
    aiULD_RRotateFwBw       = 17,
    aiULD_RPackOpenFwBw     = 18,
    aiULD_RPackSrtFwBw      = 19,

    MAX_ACTR
};

enum EN_INPUT_ID {   //나중에 전장 도면 보고 바꾸기.
    x0000 =  0 ,  xETC_StartSw            =  0 ,
    x0001 =  1 ,  xETC_StopSw             =  1 ,
    x0002 =  2 ,  xETC_LoadChgSw          =  2 ,
    x0003 =  3 ,  xETC_EmgSw              =  3 ,
    x0004 =  4 ,  xRAL_FMaskDetect1       =  4 ,
    x0005 =  5 ,  xRAL_FMaskDetect2       =  5 ,
    x0006 =  6 ,  xLDR_FGrprFw            =  6 ,
    x0007 =  7 ,  xLDR_FPickDnFw          =  7 ,
    x0008 =  8 ,  xLDR_FGrprUp            =  8 ,
    x0009 =  9 ,  xLDR_FGrprXFw           =  9 ,
    x000A = 10 ,  xLDR_FMaskDetect        = 10 ,
    x000B = 11 ,  xLDR_FClampFw           = 11 ,
    x000C = 12 ,  xLDR_FBrushFw           = 12 ,
    x000D = 13 ,  xLDR_FLoadingFw         = 13 ,
    x000E = 14 ,  xULD_FRotateFw          = 14 ,
    x000F = 15 ,  xULD_FPackPickUp        = 15 ,

    x0010 = 16 ,  xULD_FFoldUpDn          = 16 ,
    x0011 = 17 ,  xULD_FPackOpenFw        = 17 ,
    x0012 = 18 ,  xULD_FPackSrtFw         = 18 ,
    x0013 = 19 ,  xULD_FPackVccm          = 19 ,
    x0014 = 20 ,  xULD_FPickVccm          = 20 ,
    x0015 = 21 ,  xULD_RPackVccm          = 21 ,
    x0016 = 22 ,  xULD_RPickVccm          = 22 ,
    x0017 = 23 ,  xLDR_FGrprOpenSnr       = 23 ,
    x0018 = 24 ,  xULD_FMaskDetect        = 24 ,
    x0019 = 25 ,  xULD_FRotateBw          = 25 ,
    x001A = 26 ,  xULD_FPackPickDn        = 26 ,
    x001B = 27 ,  xLDR_FMaskUpLmt         = 27 , // Limit = Lmt
    x001C = 28 ,  xLDR_FMaskDnLmt         = 28 ,
    x001D = 29 ,  xLDR_FLoadingBw         = 29 ,
    x001E = 30 ,  x01E                    = 30 ,
    x001F = 31 ,  x01F                    = 31 ,

    x0020 = 32 ,  xRAL_RMaskDetect1       = 32 ,
    x0021 = 33 ,  xRAL_RMaskDetect2       = 33 ,
    x0022 = 34 ,  xLDR_RGrprFw            = 34 ,
    x0023 = 35 ,  xLDR_RPickUpFw          = 35 ,
    x0024 = 36 ,  xLDR_RGrprUpDn          = 36 ,
    x0025 = 37 ,  xLDR_RGrprXFw           = 37 ,
    x0026 = 38 ,  xLDR_RMaskDetect        = 38 ,
    x0027 = 39 ,  xLDR_RClampFw           = 39 ,
    x0028 = 40 ,  xLDR_RBrushFw           = 40 ,
    x0029 = 41 ,  xLDR_RLoadingFw         = 41 ,
    x002A = 42 ,  xULD_RRotateFw          = 42 ,
    x002B = 43 ,  xULD_RPackPickUp        = 43 ,
    x002C = 44 ,  xULD_RFoldUpDn          = 44 ,
    x002D = 45 ,  xULD_RPackOpenSnr       = 45 ,
    x002E = 46 ,  xULD_RPackSrtFw         = 46 ,
    x002F = 47 ,  xETC_MainAirPrsrChk     = 47 , // Prsr = Pressure

    x0030 = 48 ,  xLDR_RGrprOpenSnr       = 48 ,
    x0031 = 49 ,  xULD_RMaskDetect        = 49 ,
    x0032 = 50 ,  xULD_RRotateBw          = 50 ,
    x0033 = 51 ,  xULD_RPackPickDn        = 51 ,
    x0034 = 52 ,  xLDR_RMaskUpLmt         = 52 , // Limit = Lmt
    x0035 = 53 ,  xLDR_RMaskDnLmt         = 53 ,
    x0036 = 54 ,  xLDR_RLoadingBw         = 54 ,
    x0037 = 55 ,  x037                    = 55 ,
    x0038 = 56 ,  x038                    = 56 ,
    x0039 = 57 ,  x039                    = 57 ,
    x003A = 58 ,  x03A                    = 58 ,
    x003B = 59 ,  x03B                    = 59 ,
    x003C = 60 ,  x03C                    = 60 ,
    x003D = 61 ,  x03D                    = 61 ,
    x003E = 62 ,  x03E                    = 62 ,
    x003F = 63 ,  x03F                    = 63 ,

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yETC_StartLp            =  0 ,
    y0001 =  1 ,  yETC_StopLp             =  1 ,
    y0002 =  2 ,  yETC_LoadChgLp          =  2 ,
    y0003 =  3 ,  yULD_FPickUpVccm        =  3 ,
    y0004 =  4 ,  yULD_FPickUpEjct        =  4 ,
    y0005 =  5 ,  yULD_FPackOpenVccm      =  5 ,
    y0006 =  6 ,  yULD_FPackOpenEjct      =  6 ,
    y0007 =  7 ,  yLDR_FGrprFwBw          =  7 ,
    y0008 =  8 ,  yLDR_FPickUpFwBw        =  8 ,
    y0009 =  9 ,  yLDR_FGrprUpDn          =  9 ,
    y000A = 10 ,  yLDR_FGrprXFwBw         = 10 ,
    y000B = 11 ,  yLDR_FLoadingFw         = 11 ,
    y000C = 12 ,  yLDR_FLoadingBw         = 12 ,
    y000D = 13 ,  yLDR_FMaskClampFwBw     = 13 ,
    y000E = 14 ,  yLDR_FMaskBrushFwBw     = 14 ,
    y000F = 15 ,  yLDR_FAirBlowOn         = 15 ,

    y0010 = 16 ,  yULD_FMaskFoldUp        = 16 ,
    y0011 = 17 ,  yULD_FMaskFoldDn        = 17 ,
    y0012 = 18 ,  yULD_FPackPickUp        = 18 ,
    y0013 = 19 ,  yULD_FPackPickDn        = 19 ,
    y0014 = 20 ,  yULD_FPackRotateFw      = 20 ,
    y0015 = 21 ,  yULD_FPackRotateBw      = 21 ,
    y0016 = 22 ,  yULD_FPackOpenFwBw      = 22 ,
    y0017 = 23 ,  yULD_FPackSortingFw     = 23 ,
    y0018 = 24 ,  yULD_FPackSortingBw     = 24 ,
    y0019 = 25 ,  yETC_BuzzerVoice        = 25 ,
    y001A = 26 ,  y01A                    = 26 ,
    y001B = 27 ,  yLDR_FMaskUpDnCw        = 27 ,
    y001C = 28 ,  yLDR_FMaskUpDnCcw       = 28 ,
    y001D = 29 ,  yRAL_ConMotorCw         = 29 , // Con = Conveyor
    y001E = 30 ,  yULD_FConMotorCw        = 30 ,
    y001F = 31 ,  y01F                    = 31 ,

    y0020 = 32 ,  yULD_RPickUpVccm        = 32 , //
    y0021 = 33 ,  yULD_RPickUpEjct        = 33 , //
    y0022 = 34 ,  yULD_RPackOpenVccm      = 34 , //
    y0023 = 35 ,  yULD_RPackOpenEjct      = 35 , //
    y0024 = 36 ,  yLDR_RGrprFw            = 36 , //
    y0025 = 37 ,  yLDR_RPickUpFw          = 37 , //
    y0026 = 38 ,  yLDR_RGrprUpDn          = 38 , //
    y0027 = 39 ,  yLDR_RGrprXFw           = 39 , //
    y0028 = 40 ,  yLDR_RLoadingFw         = 40 , //
    y0029 = 41 ,  yLDR_RLoadingBw         = 41 , //
    y002A = 42 ,  yLDR_RMaskClampFwBw     = 42 , //
    y002B = 43 ,  yLDR_RMaskBrushFwBw     = 43 , //
    y002C = 44 ,  yLDR_RAirBlowOn         = 44 , //
    y002D = 45 ,  yULD_RMaskFoldUp        = 45 , //
    y002E = 46 ,  yULD_RMaskFoldDn        = 46 , //
    y002F = 47 ,  yULD_RPackPickUp        = 47 , //

    y0030 = 48 ,  yULD_RPackPickDn        = 48 , //
    y0031 = 49 ,  yULD_RPackRotateFw      = 49 , //
    y0032 = 50 ,  yULD_RPackRotateBw      = 50 , //
    y0033 = 51 ,  yULD_RPackOpenFwBw      = 51 , //
    y0034 = 52 ,  yULD_RPackSortingFw     = 52 , //
    y0035 = 53 ,  yULD_RPackSortingBw     = 53 , //
    y0036 = 54 ,  yRAL_FFoldAirBlow1      = 54 , //
    y0037 = 55 ,  yRAL_FFoldAirBlow2      = 55 , //
    y0038 = 56 ,  yRAL_RFoldAirBlow1      = 56 , //
    y0039 = 57 ,  yRAL_RFoldAirBlow2      = 57 , //
    y003A = 58 ,  y058                    = 58 , //
    y003B = 59 ,  y059                    = 59 , //
    y003C = 60 ,  y060                    = 60 , //
    y003D = 61 ,  yLDR_RMaskUpDnCw        = 61 , //
    y003E = 62 ,  yLDR_RMaskUpDnCcw       = 62 , //
    y003F = 63 ,  yULD_RConMotorCw        = 63 , //

    MAX_OUTPUT
};


enum EN_PSTN_ID { //프로그램에서 가야할 아이디...
    MAX_PSTN_ID_MOTR0        ,
};

enum EN_PSTN_VALUE {
    MAX_PSTN_MOTR0           ,
};


enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_CycleManLoadChg  =  2 ,

    /* Fornt Manual */
    mcLDR_FHome            =  10, // Loader Front
    mcLDR_FCycleWork       =  11,
    mcLDR_FCycleSupply     =  12,
//    mcLDR_FCycleManLoadChg =  13,
    mcLDR_FGrprFwBw        =  14,
    mcLDR_FPickUpFwBw      =  15,
    mcLDR_FGrprUpDn        =  16,
    mcLDR_FGrprXFwBw       =  17,
    mcLDR_FLoadingFwBw     =  18,
    mcLDR_FAirBlow         =  19,

    mcRAL_FHome            =  30, // Rail Front
    mcRAL_FFstAirBlower    =  31,
    mcRAL_FSndAirBlower    =  32,

    mcULD_FHome            =  40, // Unloader Front
    mcULD_FCycleIn         =  41,
    mcULD_FCycleSupply     =  42,
    mcULD_FCycleWork       =  43,
    mcULD_FFoldUpDn        =  44,
    mcULD_FPickUpFwBw      =  45,
    mcULD_FRotateFwBw      =  46,
    mcULD_FPackOpenFwBw    =  47,
    mcULD_FPackSrtFwBw     =  48,

    /* Rear Manual */
    mcLDR_RHome            =  50, // Loader Rear
    mcLDR_RCycleWork       =  51,
    mcLDR_RCycleSupply     =  52,
//    mcLDR_RCycleManLoadChg =  53,
    mcLDR_RGrprFwBw        =  54,
    mcLDR_RPickUpFwBw      =  55,
    mcLDR_RGrprUpDn        =  56,
    mcLDR_RGrprXFwBw       =  57,
    mcLDR_RLoadingFwBw     =  58,
    mcLDR_RAirBlow         =  59,

    mcRAL_RHome            =  70, // Rail Rear
    mcRAL_RFstAirBlower    =  71,
    mcRAL_RSndAirBlower    =  72,

    mcULD_RHome            =  80, // UNloader Rear
    mcULD_RCycleIn         =  81,
    mcULD_RCycleSupply     =  82,
    mcULD_RCycleWork       =  83,
    mcULD_RFoldUpDn        =  84,
    mcULD_RPickUpFwBw      =  85,
    mcULD_RRotateFwBw      =  86,
    mcULD_RPackOpenFwBw    =  87,
    mcULD_RPackSrtFwBw     =  88,

    MAX_MANUAL_CYCLE
};                                     
                                       
enum EN_ERR_ID {                       
    ei000                  =  0  ,     
    eiETC_MainAir          =  1  ,     
    eiETC_ToStartTO        =  2  ,
    eiETC_ToStopTO         =  3  ,     
    eiETC_Emergency        =  4  ,     
    ei005                  =  5  ,
    ei006                  =  6  ,     
    eiETC_LotEnd           =  7  ,
    eiETC_AllHomeTO        =  8  ,     
    eiETC_ManCycleTO       =  9  ,     
    ei010                  =  10 ,     
    eiLDR_CycleTO          =  11 ,     
    eiRAL_CycleTO          =  12 ,     
    eiULD_CycleTO          =  13 ,     
    eiLDR_HomeTO           =  14 ,
    eiRAL_HomeTO           =  15 ,     
    eiULD_HomeTO           =  16 ,
    eiLDR_ToStartTO        =  17 ,     
    eiRAL_ToStartTO        =  18 ,     
    eiULD_ToStartTO        =  19 ,     
    eiLDR_ToStopTO         =  20 ,
    eiRAL_ToStopTO         =  21 ,
    eiULD_ToStopTO         =  22 ,     
    ei023                  =  23 ,     
    eiLDR_Supply           =  24 , // 로더에서 마스크 +Limit 와 마스크 감지 센서 둘다 센싱 됬을 때, 마스크 없음을 알리는 에러
    eiLDR_PlaceFail        =  25 , // 로더에서 마스크를 레일로 떨어뜨릴 때, 타이밍 어긋나면 발생하는 에러
    eiULD_PickErr          =  26 , // 언로더 마스크팩 픽 못 했을 때 에러
    ei027                  =  27 ,
    eiATR_TimeOut          =  28 ,
    ei029                  =  29 ,
    eiULD_MskSnrErr        =  30 , // 언로더 AC모터 쪽 Mask 감지 안됨 에러
    eiULD_BinMaxOverErr    =  31 , // 언로더 Bin 박스 풀 에러

    MAX_ERR
};

#endif


