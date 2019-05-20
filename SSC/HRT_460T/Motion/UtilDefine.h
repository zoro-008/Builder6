#ifndef UTILDEFINEH
#define UTILDEFINEH

#define EQP_SERIAL_LIST \
"\
SSC-13-1043,\
SSC-14-1019,\
SSC-14-1020,\
SSC-14-1021,\
SSC-14-1022,\
SSC-14-1023,\
SSC-14-1024,\
AUS-14-1041,\
SSC-14-1042,\
SSC-14-1049,\
SSC-14-1050,\
SSC-14-1051,\
SSC-14-1052,\
SSC-14-1054,\
"

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};


enum EN_RS232 {
    rsProbe1    = 0 , //com1     PortNo = 3 , BaudRate = 19200 , ByteSize = 8, StopBits = 0 , Parity = None
    rsProbe2    = 1 , //com2     PortNo = 5 , BaudRate =  9600 , ByteSize = 8, StopBits = 0 , Parity = None
    rsTempr     = 2 , //com3     PortNo = 6 , BaudRate = 19200 , ByteSize = 8, StopBits = 2 , Parity =  Odd

    MAX_RS232
};

//파트 아이디.
enum EN_PART_ID {
    piLDR  = 0 , //Loader
    piPRB      , //PreBuffer
    piWRK      , //WorkZone
    piPSB      , //PostBuffer
    piULD      , //Unloader

    MAX_PART
};

enum EN_ARAY_ID {
    riLDR       =0 , //로더
    riPRB          , //프리버퍼 존 로더에서 투입 완료 된곳.
    riWRK          , //프리버퍼에서 워크존으로 가는 대기존.
    riPSB          , //워크존에서 빠지자 마자~ 트림 완료까지.
    riULD          ,
    riSPC          ,
    riOPT          , //옵션추가로 검사자재 바뀌도록 셋팅하도록.

    MAX_ARAY
};

enum EN_MOTR_ID {
    miPRB_XIdx  =0  ,    //
    miPSB_XIdx  =1  ,    //
    miWRK_ZTop  =2  ,    //
    miWRK_YCam  =3  ,
    miWRK_XBtm  =4  ,    //
    miWRK_YBtm  =5  ,    //
    miWRK_ZBtm  =6  ,
    miREJ_XBtm  =7  ,    //
    miSTL_YTop  =8  ,    //
    miSpare     =9  ,
    miLDR_YCmp  =10 ,    //
    miLDR_ZCmp  =11 ,    //
    miULD_YCmp  =12 ,    //
    miULD_ZCmp  =13 ,    //

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_Cmp    = 0  ,     //
    aiLDR_Psh    = 1  ,     //
    aiPRB_Rlr    = 2  ,
    aiPRB_Lft    = 3  ,     //
    aiPRB_Rgt    = 4  ,     //
    aiWRK_Idx    = 5  ,     //
    aiPSB_Lft    = 6  ,     //
    aiPSB_Rgt    = 7  ,     //
    aiSTL_Top    = 8  ,     //
    aiSTL_Btm    = 9  ,     //
    aiPSB_Hld    = 10 ,     //
    aiPSB_Rolr   = 11 ,     //
    aiULD_Cmp    = 12 ,     //

    MAX_ACTR
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/eiETC_Door            ,
/*005*/ei005                 ,
/*006*/eiETC_LotEnd          ,
/*007*/eiPRT_Crash           ,
/*008*/ei008                 ,
/*009*/eiETC_ToStartTO       ,
/*010*/eiETC_ToStopTO        ,
/*011*/eiETC_AllHomeTO       ,
/*012*/eiETC_ManCycleTO      ,
/*013*/ei013                 ,
/*014*/eiPRT_CycleTO         ,
/*015*/eiPRT_HomeTo          ,
/*016*/eiPRT_ToStartTO       ,
/*017*/eiPRT_ToStopTO        ,
/*018*/ei018                 ,
/*019*/eiMTR_HomeEnd         ,
/*020*/eiMTR_NegLim          ,
/*021*/eiMTR_PosLim          ,
/*022*/eiMTR_Alarm           ,
/*023*/ei023                 ,
/*024*/eiATR_TimeOut         ,
/*025*/ei025                 ,
/*026*/eiPKG_Dispr           , //
/*027*/eiPKG_Unknwn          ,
/*028*/eiETC_RS232           ,
/*029*/ei029                 , //--------------------------------- 장비공통에러.
/*030*/eiPRT_OverLoad        ,
/*031*/eiVSN_InspRangeOver   ,
/*032*/eiVSN_InspNG          ,
/*033*/eiVSN_ComErr          , //비전 통신 에러.
/*034*/eiPRT_Missed          ,
/*035*/eiPRT_Detect          ,
/*036*/ei036                 ,
/*037*/ei037                 ,
/*038*/ei038                 ,
/*049*/ei039                 ,
/*040*/ei040                 ,
/*041*/ei041                 ,
/*042*/eiPRT_MgzFull         , //체크 센서 터치 에러.
/*043*/eiPRT_NeedMgz         ,
/*044*/eiWRK_OverFail        , //픽 동작인데 배큠센서 온되어서 집을수 없는경우
/*045*/eiETC_YeildLow        , //헥사 포트 에러.
/*046*/ei046                 ,
/*047*/eiWRK_PinFailCntOver  , //핀 에러 카운트 오버
/*048*/eiWRK_LimPinCnt       , //핀 검사 횟수 오버.
/*049*/eiWRK_FailCntOver     , //불량 갯수 초과.
/*050*/eiWRK_FailProbeComm   , //프로브 컨트롤러 통신 에러.
/*051*/eiPSB_Out2Detected    , //포스트버퍼 2번째 센서 감지.
/*052*/eiREJ_Box             , //빈통 없음 에러.
/*053*/eiETC_TempAlrm        , //온도 에러
/*054*/ei054                 , //
/*055*/eiWRK_ShrtMonitor     , //

        MAX_ERR
};


enum EN_INPUT_ID {
    x0000 =  0 , xETC_LStartSw           =  0 , //
    x0001 =  1 , xETC_LStopSw            =  1 , //
    x0002 =  2 , xETC_LResetSw           =  2 , //
    x0003 =  3 , xETC_LAirSw             =  3 , //
    x0004 =  4 , xETC_LInitSw            =  4 , //
    x0005 =  5 , xETC_RStartSw           =  5 , //
    x0006 =  6 , xETC_RStopSw            =  6 , //
    x0007 =  7 , xETC_RResetSw           =  7 , //
    x0008 =  8 , xETC_RAirSw             =  8 , //
    x0009 =  9 , xETC_RInitSw            =  9 , //
    x000A = 10 , xETC_EmgSw1             = 10 , //
    x000B = 11 , xETC_EmgSw2             = 11 , //
    x000C = 12 , xETC_MainPower          = 12 , //
    x000D = 13 , xETC_MainAir            = 13 , //
    x000E = 14 , xETC_DoorFr             = 14 , //
    x000F = 15 , xETC_DoorRr             = 15 , //

    x0010 = 16 , xPRB_FstOvrld           = 16 , //
    x0011 = 17 , xPRB_SndOvrld           = 17 , //
    x0012 = 18 , xWRK_IdxOvrld           = 18 , //
    x0013 = 19 , xPSB_FstOvrld           = 19 , //
    x0014 = 20 , xPSB_SndOvrld           = 20 , //
    x0015 = 21 , xPRB_FstUp              = 21 , //
    x0016 = 22 , xPRB_FstDn              = 22 , //
    x0017 = 23 , xPRB_SndUp              = 23 , //
    x0018 = 24 , xPRB_SndDn              = 24 , //
    x0019 = 25 , xWRK_IdxUp              = 25 , //
    x001A = 26 , xWRK_IdxDn              = 26 , //
    x001B = 27 , xPSB_FstUp              = 27 , //
    x001C = 28 , xPSB_FstDn              = 28 , //
    x001D = 29 , xPSB_SndUp              = 29 , //
    x001E = 30 , xPSB_SndDn              = 30 , //
    x001F = 31 , x031                    = 31 , //


    x0020 = 32 , xREJ_Box1               = 32 , //
    x0021 = 33 , xREJ_Box2               = 33 , //
    x0022 = 34 , xREJ_Box3               = 34 , //
    x0023 = 35 , xREJ_Box4               = 35 , //
    x0024 = 36 , xREJ_Box5               = 36 , //
    x0025 = 37 , xREJ_Box6               = 37 , //
    x0026 = 38 , xREJ_Box7               = 38 , //
    x0027 = 39 , xREJ_Box8               = 39 , //
    x0028 = 40 , xREJ_Box9               = 40 , //
    x0029 = 41 , xREJ_Box10              = 41 , //
    x002A = 42 , xPRB_PkgIn1             = 42 , //
    x002B = 43 , xPRB_PkgIn2             = 43 , //
    x002C = 44 , xSTL_Detect1            = 44 , //
    x002D = 45 , xSTL_Detect2            = 45 , //
    x002E = 46 , xPSB_PkgOut1            = 46 , //
    x002F = 47 , xPSB_PkgOut2            = 47 , //

    x0030 = 48 , xWRK_Detect             = 48 , //
    x0031 = 49 , xSTL_HlrUp              = 49 , //
    x0032 = 50 , xSTL_HlrDn              = 50 , //
    x0033 = 51 , xPRB_LRollerDn          = 51 , //  xPRB_LRollerFw
    x0034 = 52 , xPRB_LRollerUp          = 52 , //  xPRB_RRollerFw
    x0035 = 53 , xPRB_RRollerDn          = 53 , //  xPSB_LRollerFw
    x0036 = 54 , xPRB_RRollerUp          = 54 , //  xPSB_RRollerFw
    x0037 = 55 , xPSB_LRollerDn          = 55 , //
    x0038 = 56 , xPSB_LRollerUp          = 56 , //
    x0039 = 57 , xPSB_RRollerDn          = 57 , //
    x003A = 58 , xPSB_RRollerUp          = 58 , //
    x003B = 59 , xSTL_UnitUp             = 59 , //
    x003C = 60 , xSTL_UnitDn             = 60 , //
    x003D = 61 , xSTL_PunchDieUp         = 61 , //
    x003E = 62 , xSTL_PunchDieDn         = 62 , //
    x003F = 63 , x063                    = 63 , //

    x0040 = 64 , x064                    = 64 , //
    x0041 = 65 , x065                    = 65 , //
    x0042 = 66 , x066                    = 66 , //
    x0043 = 67 , x067                    = 67 , //
    x0044 = 68 , x068                    = 68 , //
    x0045 = 69 , x069                    = 69 , //
    x0046 = 70 , x070                    = 70 , //
    x0047 = 71 , x071                    = 71 , //
    x0048 = 72 , x072                    = 72 , //
    x0049 = 73 , x073                    = 73 , //
    x004A = 74 , x074                    = 74 , //
    x004B = 75 , x075                    = 75 , //
    x004C = 76 , x076                    = 76 , //
    x004D = 77 , x077                    = 77 , //
    x004E = 78 , x078                    = 78 , //
    x004F = 79 , x079                    = 79 , //

    x0050 = 80 , x080                    = 80 , //
    x0051 = 81 , x081                    = 81 , //
    x0052 = 82 , x082                    = 82 , //
    x0053 = 83 , x083                    = 83 , //
    x0054 = 84 , x084                    = 84 , //
    x0055 = 85 , x085                    = 85 , //
    x0056 = 86 , x086                    = 86 , //
    x0057 = 87 , x087                    = 87 , //
    x0058 = 88 , x088                    = 88 , //
    x0059 = 89 , x089                    = 89 , //
    x005A = 90 , x090                    = 90 , //
    x005B = 91 , x091                    = 91 , //
    x005C = 92 , x092                    = 92 , //
    x005D = 93 , x093                    = 93 , //
    x005E = 94 , x094                    = 94 , //
    x005F = 95 , x095                    = 95 , //

    x0060 = 96 , xLDR_PusherOverLoad     = 96 , //
    x0061 = 97 , x097                    = 97 , //
    x0062 = 98 , xLDR_PusherFw           = 98 , //
    x0063 = 99 , xLDR_PusherBw           = 99 , //
    x0064 =100 , xLDR_ClampBw            =100 , //
    x0065 =101 , xLDR_ClampFw            =101 , //
    x0066 =102 , xLDR_MgzDetect1         =102 , //
    x0067 =103 , xLDR_MgzDetect2         =103 , //
    x0068 =104 , xLDR_MgzIn              =104 , //
    x0069 =105 , xLDR_MgzOutFull         =105 , //
    x006A =106 , x106                    =106 , //
    x006B =107 , x107                    =107 , //
    x006C =108 , x108                    =108 , //
    x006D =109 , x109                    =109 , //
    x006E =110 , xETC_LEmgLDRSw          =110 , //
    x006F =111 , x111                    =111 , //

    x0070 =112 , xULD_MgzDetect1         =112 , //
    x0071 =113 , xULD_MgzDetect2         =113 , //
    x0072 =114 , xULD_ClampBw            =114 , //
    x0073 =115 , xULD_ClampFw            =115 , //
    x0074 =116 , xULD_MgzOutFull         =116 , //
    x0075 =117 , xULD_MgzIn              =117 , //
    x0076 =118 , x118                    =118 , //
    x0077 =119 , x119                    =119 , //
    x0078 =120 , x120                    =120 , //
    x0079 =121 , x121                    =121 , //
    x007A =122 , xETC_REmgULDSw          =122 , //
    x007B =123 , x123                    =123 , //
    x007C =124 , x124                    =124 , //
    x007D =125 , x125                    =125 , //
    x007E =126 , x126                    =126 , //
    x007F =127 , x127                    =127 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_LStartLp           =  0 , //
    y0001 =  1 , yETC_LStopLp            =  1 , //
    y0002 =  2 , yETC_LResetLp           =  2 , //
    y0003 =  3 , yETC_LAirLp             =  3 , //
    y0004 =  4 , yETC_LInitLp            =  4 , //
    y0005 =  5 , yETC_RStartLp           =  5 , //
    y0006 =  6 , yETC_RStopLp            =  6 , //
    y0007 =  7 , yETC_RResetLp           =  7 , //
    y0008 =  8 , yETC_RAirLp             =  8 , //
    y0009 =  9 , yETC_RInitLp            =  9 , //
    y000A = 10 , yETC_TwRedLp            = 10 , //
    y000B = 11 , yETC_TwYelLp            = 11 , //
    y000C = 12 , yETC_TwGrnLp            = 12 , //
    y000D = 13 , yETC_TwBzz              = 13 , //
    y000E = 14 , yETC_MainSol            = 14 , //
    y000F = 15 , yETC_LightOn            = 15 , //

    y0010 = 16 , yPRB_FstBw              = 16 , //
    y0011 = 17 , yPRB_FstFw              = 17 , //
    y0012 = 18 , yPRB_SndBw              = 18 , //
    y0013 = 19 , yPRB_SndFw              = 19 , //
    y0014 = 20 , yWRK_InxFw              = 20 , //
    y0015 = 21 , yPSB_FstBw              = 21 , //
    y0016 = 22 , yPSB_FstFw              = 22 , //
    y0017 = 23 , yPSB_SndBw              = 23 , //
    y0018 = 24 , yPSB_SndFw              = 24 , //
    y0019 = 25 , ySTL_HlrDn              = 25 , //
    y001A = 26 , yPRB_RlrDn              = 26 , //
    y001B = 27 , yPSB_RlrDn              = 27 , //
    y001C = 28 , ySTL_PnchDn             = 28 , //
    y001D = 29 , ySTL_DieUp              = 29 , //
    y001E = 30 , ySTL_Blower             = 30 , //
    y001F = 31 , y031                    = 31 , //

    y0020 = 32 , yPRB_FeedingAC          = 32 , //
    y0021 = 33 , yPSB_FeedingAC          = 33 , //
    y0022 = 34 , y034                    = 34 , //
    y0023 = 35 , y035                    = 35 , //
    y0024 = 36 , y036                    = 36 , //
    y0025 = 37 , y037                    = 37 , //
    y0026 = 38 , y038                    = 38 , //
    y0027 = 39 , y039                    = 39 , //
    y0028 = 40 , y040                    = 40 , //
    y0029 = 41 , y041                    = 41 , //
    y002A = 42 , y042                    = 42 , //
    y002B = 43 , y043                    = 43 , //
    y002C = 44 , y044                    = 44 , //
    y002D = 45 , y045                    = 45 , //
    y002E = 46 , y046                    = 46 , //
    y002F = 47 , y047                    = 47 , //

    y0030 = 48 , y048                    = 48 , //
    y0031 = 49 , y049                    = 49 , //
    y0032 = 50 , y050                    = 50 , //
    y0033 = 51 , y051                    = 51 , //
    y0034 = 52 , y052                    = 52 , //
    y0035 = 53 , y053                    = 53 , //
    y0036 = 54 , y054                    = 54 , //
    y0037 = 55 , y055                    = 55 , //
    y0038 = 56 , y056                    = 56 , //
    y0039 = 57 , y057                    = 57 , //
    y003A = 58 , y058                    = 58 , //
    y003B = 59 , y059                    = 59 , //
    y003C = 60 , y060                    = 60 , //
    y003D = 61 , y061                    = 61 , //
    y003E = 62 , y062                    = 62 , //
    y003F = 63 , y063                    = 63 , //

    y0040 = 64 , yLDR_PusherFw           = 64 , //
    y0041 = 65 , yLDR_ClampBw            = 65 , //
    y0042 = 66 , yLDR_ClampFw            = 66 , //
    y0043 = 67 , y067                    = 67 , //
    y0044 = 68 , yLDR_ZBreak             = 68 , //
    y0045 = 69 , yLDR_MgzInAC            = 69 , //
    y0046 = 70 , yLDR_MgzOutAC           = 70 , //
    y0047 = 71 , y071                    = 71 , //
    y0048 = 72 , y072                    = 72 , //
    y0049 = 73 , y073                    = 73 , //
    y004A = 74 , y074                    = 74 , //
    y004B = 75 , y075                    = 75 , //
    y004C = 76 , y076                    = 76 , //
    y004D = 77 , y077                    = 77 , //
    y004E = 78 , y078                    = 78 , //
    y004F = 79 , y079                    = 79 , //

    y0050 = 80 , yULD_ClampBw            = 80 , //
    y0051 = 81 , yULD_ClampFw            = 81 , //
    y0052 = 82 , y082                    = 82 , //
    y0053 = 83 , y083                    = 83 , //
    y0054 = 84 , yULD_ZBreak             = 84 , //
    y0055 = 85 , yULD_MgzOutAC           = 85 , //
    y0056 = 86 , yULD_MgzInAC            = 86 , //
    y0057 = 87 , y087                    = 87 , //
    y0058 = 88 , y088                    = 88 , //
    y0059 = 89 , y089                    = 89 , //           
    y005A = 90 , y090                    = 90 , //
    y005B = 91 , y091                    = 91 , //
    y005C = 92 , y092                    = 92 , //
    y005D = 93 , y093                    = 93 , //
    y005E = 94 , y094                    = 94 , //
    y005F = 95 , y095                    = 95 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID {
    piPRB_XWait       = 0  ,  //miPRB_XCmp  =0  ,    //
    piPRB_XLIn             ,  //처음 들어온 포지션
    piPRB_XLHeat           ,  //Heat 포지션 까지 가자.
    piPRB_XRGet            ,  //Heat 포지션에서 넘겨 받는다.
    piPRB_XROut            ,  //배출하는 위치
    MAX_PSTN_ID_MOTR0      ,  //

    piPSB_XWait       =  0 ,  //miPSB_XCmp  =1  ,    //
    piPSB_XLIn             ,  //
    piPSB_XLPush           ,  //
    piPSB_XRGet            ,  //
    piPSB_XRWorkStt        ,  //
    piPSB_XROut            ,  //
    piPSB_XRWork           ,  //
    MAX_PSTN_ID_MOTR1      ,  //

    piWRK_ZTWait      =  0 ,  //miWRK_ZTop  =2  ,    //
    piWRK_ZTWork           ,  //
    MAX_PSTN_ID_MOTR2      ,  //

    piWRK_YTWait      = 0  ,  //miWRK_YCam  =3  ,    //
    piWRK_YTWork           ,  //
    MAX_PSTN_ID_MOTR3      ,  //
                              //
    piWRK_XBWait      =  0 ,  //miWRK_XBtm  =4  ,    //
    piWRK_XBIn             ,  //
    piWRK_XBWorkStt        ,  //
    piWRK_XBOut            ,  //
    piWRK_XBChange         ,  //
    piWRK_XBWorkUp         ,  //
    piWRK_XBWork           ,  //
    MAX_PSTN_ID_MOTR4      ,  //

    piWRK_YBWait      =  0 ,  //miWRK_YBtm  =5  ,    //
    piWRK_YBIn             ,  //
    piWRK_YBWork           ,  //
    piWRK_YBChange         ,  //
    MAX_PSTN_ID_MOTR5      ,  //

    piWRK_ZBWait      =  0 ,  //miWRK_ZBtm  =6  ,
    piWRK_ZBWork           ,  //
    piWRK_ZBOut            ,  //
    piWRK_ZBChange         ,  //
    MAX_PSTN_ID_MOTR6      ,  //

    piREJ_XBWait      =  0 ,  //miREJ_XBtm  =7  ,    //
    piREJ_XBWorkStt        ,  //
    piREJ_XBWork           ,  //
    MAX_PSTN_ID_MOTR7      ,  //

    piSTL_YTWait      =  0 ,  //miSTL_YTop  =8  ,    //
    piSTL_YTWorkStart      ,  //
    piSTL_YTWork           ,  //
    //piSTL_YTPlace          ,  //Place 포지션.
    MAX_PSTN_ID_MOTR8      ,  //

    piSPR_Spare       =  0 ,// =  0 ,  //miSpare     =9  ,    //
    //      ,  //
    MAX_PSTN_ID_MOTR9      ,  //

    piLDR_YWait       =  0 ,  //miLDR_YCmp  =10 ,    //
    piLDR_YWork            ,  //
    piLDR_YPick            ,  //
    piLDR_YPlace           ,  //
    MAX_PSTN_ID_MOTR10     ,  //

    piLDR_ZWait       =  0 ,  //miLDR_ZCmp  =11 ,    //
    piLDR_ZPickFwd         ,  //
    piLDR_ZClampOn         ,  //
    piLDR_ZPickBwd         ,  //
    piLDR_ZWorkStart       ,  //맨 아래 슬롯 기준.
    piLDR_ZPlaceFwd        ,  //
    piLDR_ZClampOff        ,  //
    piLDR_ZPlaceBwd        ,  //
    piLDR_ZWork            ,  //
    MAX_PSTN_ID_MOTR11     ,  //

    piULD_YWait       =  0 ,  //miULD_YCmp  =12 ,    //
    piULD_YWork            ,  //
    piULD_YPick            ,  //
    piULD_YPlace           ,  //
    MAX_PSTN_ID_MOTR12     ,  //

    piULD_ZWait       =  0 ,  //miULD_ZCmp  =13 ,    //
    piULD_ZPickFwd         ,  //
    piULD_ZClampOn         ,  //
    piULD_ZPickBwd         ,  //
    piULD_ZWorkStart       ,  //
    piULD_ZPlaceFwd        ,  //
    piULD_ZClampOff        ,  //
    piULD_ZPlaceBwd        ,  //
    piULD_ZWork            ,  //
    MAX_PSTN_ID_MOTR13     ,  //
};

enum EN_PSTN_VALUE {
    pvPRB_XWait       = 0  ,  //miPRB_XCmp  =0  ,    //
    pvPRB_XLIn             ,  //처음 들어온 포지션
    pvPRB_XLHeat           ,  //Index 왼쪽에서 오른쪽으로 넘기는 포지션
    pvPRB_XRGet            ,  //Index 왼쪽에서 오른쪽으로 받아오는 포지션
    pvPRB_XROut            ,  //배출하는 위치
    MAX_PSTN_MOTR0         ,  //

    pvPSB_XWait       =  0 ,  //miPSB_XCmp  =1  ,    //
    pvPSB_XLIn             ,  //
    pvPSB_XLPush           ,  //
    pvPSB_XRGet            ,  //
    pvPSB_XRWorkStt        ,  //
    pvPSB_XROut            ,  //
    MAX_PSTN_MOTR1         ,  //

    pvWRK_ZTWait      =  0 ,  //miWRK_ZTop  =2  ,    //
    pvWRK_ZTWork           ,  //
    MAX_PSTN_MOTR2         ,  //

    pvWRK_YTWait      = 0  ,  //miWRK_YCam  =3  ,    //
    pvWRK_YTWork           ,  //
    MAX_PSTN_MOTR3         ,  //
                              //
    pvWRK_XBWait      =  0 ,  //miWRK_XBtm  =4  ,    //
    pvWRK_XBIn             ,  //
    pvWRK_XBWorkStt        ,  //
    pvWRK_XBOut            ,  //
    pvWRK_XBChange         ,  //
    MAX_PSTN_MOTR4         ,  //

    pvWRK_YBWait      =  0 ,  //miWRK_YBtm  =5  ,    //
    pvWRK_YBIn             ,  //
    pvWRK_YBWork           ,  //
    pvWRK_YBChange         ,  //
    MAX_PSTN_MOTR5         ,  //

    pvWRK_ZBWait      =  0 ,  //miWRK_ZBtm  =6  ,
    pvWRK_ZBWork           ,  //
    pvWRK_ZBOut            ,  //
    pvWRK_ZBChange         ,  //
    MAX_PSTN_MOTR6         ,  //

    pvREJ_XBWait      =  0 ,  //miREJ_XBtm  =7  ,    //
    pvREJ_XBWorkStt        ,  //
    MAX_PSTN_MOTR7         ,  //

    pvSTL_YTWait      =  0 ,  //miSTL_YTop  =8  ,    //
    pvSTL_YTWorkStart      ,  //
    //piSTL_YTPlace          ,  //Place 포지션.
    MAX_PSTN_MOTR8         ,  //

    pvSPR_Spare       =  0 ,  //miSpare     =9  ,    //
    //      ,  //
    MAX_PSTN_MOTR9         ,  //

    pvLDR_YWait       =  0 ,  //miLDR_YCmp  =10 ,    //
    pvLDR_YWork            ,  //
    pvLDR_YPick            ,  //
    pvLDR_YPlace           ,  //
    MAX_PSTN_MOTR10        ,  //

    pvLDR_ZWait       =  0 ,  //miLDR_ZCmp  =11 ,    //
    pvLDR_ZPickFwd         ,  //
    pvLDR_ZClampOn         ,  //
    pvLDR_ZPickBwd         ,  //
    pvLDR_ZWorkStart       ,  //맨 아래 슬롯 기준.
    pvLDR_ZPlaceFwd        ,  //
    pvLDR_ZClampOff        ,  //
    pvLDR_ZPlaceBwd        ,  //
    MAX_PSTN_MOTR11        ,  //

    pvULD_YWait       =  0 ,  //miULD_YCmp  =12 ,    //
    pvULD_YWork            ,  //
    pvULD_YPick            ,  //
    pvULD_YPlace           ,  //
    MAX_PSTN_MOTR12        ,  //

    pvULD_ZWait       =  0 ,  //miULD_ZCmp  =13 ,    //
    pvULD_ZPickFwd         ,  //
    pvULD_ZClampOn         ,  //
    pvULD_ZPickBwd         ,  //
    pvULD_ZWorkStart       ,  //
    pvULD_ZPlaceFwd        ,  //
    pvULD_ZClampOff        ,  //
    pvULD_ZPlaceBwd        ,  //
    MAX_PSTN_MOTR13        ,  //


};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_Home             = 10 ,
    mcLDR_Supply                ,
    mcLDR_Pick                  ,
    mcLDR_Place                 ,

    mcPRB_Home             = 20 ,
    mcPRB_In                    ,
    mcPRB_Wait                  ,
    mcPRB_Out                   ,

    mcWRK_Home             = 30 ,
    mcWRK_Work                  ,
    mcWRK_Out                   ,
    mcWRK_Visn                  ,
    mcWRK_ToolChng              ,

    mcPSB_Home             = 40 ,
    mcPSB_In                    ,
    mcPSB_Move                  ,
    mcPSB_Work                  ,
    mcPSB_Out                   ,
    mcSTL_YPchUp1Pitch          ,
    mcSTL_YPchDn1Pitch          ,

    mcULD_Home             = 50 ,
    mcULD_Supply                ,
    mcULD_Pick                  ,
    mcULD_Place                 ,

    mcETC_AllClampOff      = 60,
    mcETC_PrbStt                ,
    mcETC_LDRULDPlace           ,
    mcETC_PSBPlace              ,

    MAX_MANUAL_CYCLE
};

#endif



