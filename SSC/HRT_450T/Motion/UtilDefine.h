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
enum EN_ARAY_ID {
    riLDR       =0 , //로더
    riPR1          , //프리버퍼존 1 로더에서 투입 완료 된곳.
    riPR2          , //프리버퍼존 2 프리버퍼에서 워크존 가기 전 대기 공간.
    riWRK          , //프리버퍼에서 워크존으로 가는 대기존.
    riPSB          , //워크존에서 빠지자 마자~ 트림 완료까지.
    riULD          ,
    riSPC          ,
    riOPT          , //옵션추가로 검사자재 바뀌도록 셋팅하도록.
    //riSSI          , //Special Strip Inspection 전수 검사 수행하지 않을때 체크용

    MAX_ARAY
};

enum EN_MOTR_ID {
    miPR1_XCmp  =0  ,    //
    miPR2_XCmp  =1  ,    //
    miWRK_XCmp  =2  ,    //
    miPSB_XCmp  =3  ,    //
    miWRK_ZTop  =4  ,    //
    miWRK_ZBtm  =5  ,    //
    miWRK_YBtm  =6  ,    // 미래선도 과제에 추가된 모터.. 변경예정.
    miWRK_XBtm  =7  ,    // 정부과제와는 다른다....모션 4번이 한개만 들어가 있다 그래서 7번이
                         // 스패어 정부과제는 10번이 스패어...JH
    miSTL_YTop  =8  ,    //
    miREJ_XBtm  =9  ,    //
    //miSTL_YBtm     ,    //
    miLDR_YCmp  =10 ,    //
    miLDR_ZCmp  =11 ,    //
    miULD_YCmp  =12 ,    //
    miULD_ZCmp  =13 ,    //

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_Cmp    = 0  ,     //
    aiLDR_Psh    = 1  ,     //
    aiPR1_Cmp    = 2  ,     //
    aiPR2_Cmp    = 3  ,     //
    //aiPRB_IdChk  = 4  ,     //
    aiWRK_Cmp    = 4  ,     //
    aiPSB_Cmp    = 5  ,     //
    aiSTL_Top    = 6  ,     //
    aiSTL_Btm    = 7  ,     //
    aiPSB_Psh    = 8  ,     //
    aiULD_Cmp    = 9  ,     //
    aiPSB_Rolr   = 10 ,     //
    aiWRK_Hold   = 11 ,     //

    MAX_ACTR
};

enum EN_ERR_ID {
    eiETC_MainAir      = 0 ,  //000 ;
    eiETC_ToStartTO        ,  //001 ;
    eiETC_ToStopTO         ,  //002 ;
    eiETC_Emergency        ,  //003 ;
    eiETC_FDoor            ,  //004 ;
    eiETC_BDoor            ,  //005 ;
    eiETC_LotEnd           ,  //006 ;
    eiETC_AllHomeTO        ,  //007 ;
    eiETC_ManCycleTO       ,  //008 ;
    eiLDR_CycleTO          ,  //009 ;
    eiPR1_CycleTO          ,  //010 ;
    eiPR2_CycleTO          ,  //011 ;
    eiWRK_CycleTO          ,  //012 ;
    eiPSB_CycleTO          ,  //013 ;
    eiSTL_CycleTO          ,  //014 ;
    eiREJ_CycleTO          ,  //015 ;
    eiULD_CycleTO          ,  //016 ;
    eiLDR_HomeTO           ,  //017 ;
    eiPR1_HomeTO           ,  //018 ;
    eiPR2_HomeTO           ,  //019 ;
    eiWRK_HomeTO           ,  //020 ;
    eiPSB_HomeTO           ,  //021 ;
    eiSTL_HomeTO           ,  //022 ;
    eiREJ_HomeTO           ,  //023 ;
    eiULD_HomeTO           ,  //024 ;
    eiLDR_ToStartTO        ,  //025 ;
    eiPR1_ToStartTO        ,  //026 ;
    eiPR2_ToStartTO        ,  //027 ;
    eiWRK_ToStartTO        ,  //028 ;
    eiPSB_ToStartTO        ,  //029 ;
    eiSTL_ToStartTO        ,  //030 ;
    eiREJ_ToStartTO        ,  //031 ;
    eiULD_ToStartTO        ,  //032 ;
    eiLDR_ToStopTO         ,  //033 ;
    eiPR1_ToStopTO         ,  //034 ;
    eiPR2_ToStopTO         ,  //035 ;
    eiWRK_ToStopTO         ,  //036 ;
    eiPSB_ToStopTO         ,  //037 ;
    eiSTL_ToStopTO         ,  //038 ;
    eiREJ_ToStopTO         ,  //039 ;
    eiULD_ToStopTO         ,  //040 ;
    eiLDR_YCmpHomeEnd      ,  //041 ;
    eiLDR_YCmpNegLim       ,  //042 ;
    eiLDR_YCmpPosLim       ,  //043 ;
    eiLDR_YCmpAlarm        ,  //044 ;
    eiLDR_ZCmpHomeEnd      ,  //045 ;
    eiLDR_ZCmpNegLim       ,  //046 ;
    eiLDR_ZCmpPosLim       ,  //047 ;
    eiLDR_ZCmpAlarm        ,  //048 ;
    eiPR1_XCmpHomeEnd      ,  //049 ;
    eiPR1_XCmpNegLim       ,  //050 ;
    eiPR1_XCmpPosLim       ,  //051 ;
    eiPR1_XCmpAlarm        ,  //052 ;
    eiPR2_XCmpHomeEnd      ,  //053 ;
    eiPR2_XCmpNegLim       ,  //054 ;
    eiPR2_XCmpPosLim       ,  //055 ;
    eiPR2_XCmpAlarm        ,  //056 ;
    eiWRK_XCmpHomeEnd      ,  //057 ;
    eiWRK_XCmpNegLim       ,  //058 ;
    eiWRK_XCmpPosLim       ,  //059 ;
    eiWRK_XCmpAlarm        ,  //060 ;
    eiPSB_XCmpHomeEnd      ,  //061 ;
    eiPSB_XCmpNegLim       ,  //062 ;
    eiPSB_XCmpPosLim       ,  //063 ;
    eiPSB_XCmpAlarm        ,  //064 ;
    eiWRK_ZTopHomeEnd      ,  //065 ;
    eiWRK_ZTopNegLim       ,  //066 ;
    eiWRK_ZTopPosLim       ,  //067 ;
    eiWRK_ZTopAlarm        ,  //068 ;
    eiWRK_ZBtmHomeEnd      ,  //069 ;
    eiWRK_ZBtmNegLim       ,  //070 ;
    eiWRK_ZBtmPosLim       ,  //071 ;
    eiWRK_ZBtmAlarm        ,  //072 ;
    eiWRK_YWrkHomeEnd      ,  //073 ;
    eiWRK_YWrkNegLim       ,  //074 ;
    eiWRK_YWrkPosLim       ,  //075 ;
    eiWRK_YWrkAlarm        ,  //076 ;
    eiSTL_YTopHomeEnd      ,  //077 ;
    eiSTL_YTopNegLim       ,  //078 ;
    eiSTL_YTopPosLim       ,  //079 ;
    eiSTL_YTopAlarm        ,  //080 ;
    eiREJ_XBtmHomeEnd      ,  //081 ;
    eiREJ_XBtmNegLim       ,  //082 ;
    eiREJ_XBtmPosLim       ,  //083 ;
    eiREJ_XBtmAlarm        ,  //084 ;
    eiULD_ZCmpHomeEnd      ,  //085 ;
    eiULD_ZCmpNegLim       ,  //086 ;
    eiULD_ZCmpPosLim       ,  //087 ;
    eiULD_ZCmpAlarm        ,  //088 ;
    eiULD_YCmpHomeEnd      ,  //089 ;
    eiULD_YCmpNegLim       ,  //090 ;
    eiULD_YCmpPosLim       ,  //091 ;
    eiULD_YCmpAlarm        ,  //092 ;
    ei093                  ,  //093 ;
    ei094                  ,  //094 ;
    ei095                  ,  //095 ;
    ei096                  ,  //096 ;
    eiPR1_CmpTO            ,  //097 ;
    eiPR2_CmpTO            ,  //098 ;
    eiPRB_IdChkTO          ,  //099 ;
    eiWRK_CmpTO            ,  //100 ;
    eiPSB_CmpTO            ,  //101 ;
    eiLDR_CmpTO            ,  //102 ;
    eiLDR_PushTO           ,  //103 ;
    eiSTL_CmpTO            ,  //104 ;
    eiULD_CmpTO            ,  //105 ;
    eiULD_PushTO           ,  //106 ;
    eiPSB_RolrTO           ,  //107 ;
    ei108                  ,  //108 ;
    ei109                  ,  //109 ;
    ei110                  ,  //110 ;
    eiLDR_ClampDispr       ,  //111 ;
    eiLDR_ClampUnknown     ,  //112 ;
    eiLDR_MgzChange        ,  //113 ;
    eiPR1_Dispr            ,  //114 ;
    eiPR1_Unknown          ,  //115 ;
    eiPR2_Dispr            ,  //116 ;
    eiPR2_Unknown          ,  //117 ;
    eiWRK_Dispr            ,  //118 ;
    eiWRK_Unknown          ,  //119 ;
    eiPSB_Dispr            ,  //120 ;
    eiPSB_Unknown          ,  //121 ;
    eiWRK_FailCntOver      ,  //122 ;
    eiWRK_FailProbeComm1   ,  //123 ;
    eiWRK_FailProbeComm2   ,  //124 ;
    eiLDR_MgzFull          ,  //125 ;
    eiLDR_PusherOverload   ,  //126 ;
    eiPR1_Overload         ,  //127 ;
    eiPR2_Overload         ,  //128 ;
    eiWRK_Overload         ,  //129 ;
    eiPSB_Overload         ,  //130 ;
    eiPRB_IdCheck          ,  //131 ;
    eiPR1_TempZ1Alrm       ,  //132 ;
    eiPR2_TempZ1Alrm       ,  //133 ;
    eiWRK_TempZ1Alrm       ,  //134 ;
    eiPSB_TempZ1Alrm       ,  //135 ;
    eiPSB_Striphitch       ,  //136 ;
    eiPR1_TempWire         ,  //137 ;
    eiPR2_TempWire         ,  //138 ;
    eiWRK_TempWire         ,  //139 ;
    eiPSB_TempWire         ,  //140 ;
    eiETC_YeildLow         ,  //141 ;
    ei142                  ,  //142 ;
    eiETC_NetSeverFail     ,  //143 ;
    eiETC_NetConectFail    ,  //144 ;
    eiETC_PostEqpLotStr    ,  //145 ;
    eiETC_PostEqpLotEnd    ,  //146 ;
    eiETC_PostEqpModelChg  ,  //147 ;
    eiETC_PostEqpData      ,  //148 ;
    eiETC_PostEqpNotReady  ,  //149 ;
    eiETC_PostEqpCommTO    ,  //150 ;
    eiWRK_PinFailCntOver   ,  //151 ;
    eiWRK_Probe1DataWrong  ,  //152 ;
    eiWRK_Probe2DataWrong  ,  //153 ;
    eiMNR_StripRemoveTo    ,  //154 ;
    eiPRB_StripOver        ,  //155 ;
    eiLDR_NeedMgz          ,  //156 ;
    eiWRK_ProbeSetFail     ,  //157 ;
    eiULD_MgzUnknown       ,  //158 ;
    eiULD_MgzDispr         ,  //159 ;
    eiULD_MgzFull          ,  //160 ;
    eiULD_NeedMgz          ,  //161 ;
    eiREJ_BoxFull          ,  //162 ;
    eiREJ_Box1             ,  //163 ;
    eiREJ_Box2             ,  //164 ;
    eiREJ_Box3             ,  //165 ;
    eiREJ_Box4             ,  //166 ;
    eiREJ_Box5             ,  //167 ;
    eiREJ_Box6             ,  //168 ;
    eiREJ_Box7             ,  //169 ;
    eiREJ_Box8             ,  //170 ;
    eiREJ_Box9             ,  //171 ;
    eiREJ_Box10            ,  //172 ;
    eiREJ_MaxFullBox       ,  //173 ;
    eiSTL_PickUp           ,  //174 ;
    eiSTL_Place            ,  //175 ;
    eiPSB_Out1Sensor       ,  //176 ;
    eiPSB_Out2Detected     ,  //177 ;
    eiPSB_Out2NotDetected  ,  //178 ;
    eiSTL_MgzUnknow        ,  //179 ;
    eiSTL_Trim             ,  //180 ;
    eiSTL_Trim1            ,  //181 ;
    eiWRK_LimPinCnt        ,  //182 ;
    eiWRK_ShrtMonitor      ,  //183 ;
    eiETC_TempAlrm         ,  //184 ;
    eiWRK_VisnNG           ,  //185 ;
    eiWRK_VisnReady        ,  //186 ;
    eiWRK_OverFail         ,  //187 ;
    MAX_ERR
};

enum EN_INPUT_ID {
    x0000 =  0 , xETC_LStartSw           =  0 , //
    x0001 =  1 , xETC_LStopSw            =  1 , //
    x0002 =  2 , xETC_LResetSw           =  2 , //
    x0003 =  3 , xETC_LMainAir           =  3 , //
    x0004 =  4 , xETC_LInitSw            =  4 , //
    x0005 =  5 , xETC_RStartSw           =  5 , //
    x0006 =  6 , xETC_RStopSw            =  6 , //
    x0007 =  7 , xETC_RResetSw           =  7 , //
    x0008 =  8 , xETC_RMainAir           =  8 , //
    x0009 =  9 , xETC_RInitSw            =  9 , //
    x000A = 10 , x010                    = 10 , //
    x000B = 11 , x011                    = 11 , //
    x000C = 12 , x012                    = 12 , //
    x000D = 13 , xETC_EmgSw1             = 13 , //
    x000E = 14 , xETC_EmgSw2             = 14 , //
    x000F = 15 , x015                    = 15 , //

    x0010 = 16 , x016                    = 16 , //
    x0011 = 17 , xETC_PowerOn            = 17 , //
    x0012 = 18 , xETC_MainSol            = 18 , //
    x0013 = 19 , xETC_FDoor              = 19 , //
    x0014 = 20 , xETC_BDoor              = 20 , //
    x0015 = 21 , x021                    = 21 , //
    x0016 = 22 , x022                    = 22 , //
    x0017 = 23 , x023                    = 23 , //
    x0018 = 24 , xPR1_Overload           = 24 , //
    x0019 = 25 , xPR2_Overload           = 25 , //
    x001A = 26 , xWRK_Overload           = 26 , //
    x001B = 27 , xPSB_Overload           = 27 , //
    x001C = 28 , xPRB_StripOver          = 28 , //
    x001D = 29 , xPR1_ClampFw            = 29 , //
    x001E = 30 , xPR1_ClampBw            = 30 , //
    x001F = 31 , xPR2_ClampFw            = 31 , //


    x0020 = 32 , xPR2_ClampBw            = 32 , //
    x0021 = 33 , xWRK_ClampFw            = 33 , //
    x0022 = 34 , xWRK_ClampBw            = 34 , //
    x0023 = 35 , xPSB_ClampFw            = 35 , //
    x0024 = 36 , xPSB_ClampBw            = 36 , //
    x0025 = 37 , xPRB_PkgIn1             = 37 , //
    x0026 = 38 , xPRB_PkgIn2             = 38 , //
    x0027 = 39 , xPSB_Pkg1               = 39 , //
    x0028 = 40 , xPSB_Pkg2               = 40 , //
    x0029 = 41 , xPSB_PkgOut1            = 41 , //
    x002A = 42 , xPSB_PkgOut2            = 42 , //
    x002B = 43 , xSTL_DevDetect          = 43 , //
    x002C = 44 , xSTL_PunchDieUp         = 44 , //
    x002D = 45 , xSTL_PunchDieDn         = 45 , //
    x002E = 46 , xSTL_UnitUp             = 46 , //
    x002F = 47 , xSTL_UnitDn             = 47 , //

    x0030 = 48 , xREJ_UnitDetect         = 48 , //
    x0031 = 49 , xPSB_PusherFw           = 49 , //
    x0032 = 50 , xPSB_PusherBw           = 50 , //
    x0033 = 51 , xPSB_LRollerFw          = 51 , //
    x0034 = 52 , xPSB_RRollerFw          = 52 , //
    x0035 = 53 , xSTL_VaccChk            = 53 , //
    x0036 = 54 , x054                    = 54 , //
    x0037 = 55 , x055                    = 55 , //
    x0038 = 56 , xPR1_Detect             = 56 , //
    x0039 = 57 , xPR2_Detect             = 57 , //
    x003A = 58 , xWRK_Detect1            = 58 , //
    x003B = 59 , x059                    = 59 , //
    x003C = 60 , xWRK_Detect2            = 60 , //
    x003D = 61 , x061                    = 61 , //
    x003E = 62 , x062                    = 62 , //
    x003F = 63 , x063                    = 63 , //

    x0040 = 64 , xREJ_Box1               = 64 , //
    x0041 = 65 , xREJ_Box2               = 65 , //
    x0042 = 66 , xREJ_Box3               = 66 , //
    x0043 = 67 , xREJ_Box4               = 67 , //
    x0044 = 68 , xREJ_Box5               = 68 , //
    x0045 = 69 , xREJ_Box6               = 69 , //
    x0046 = 70 , xREJ_Box7               = 70 , //
    x0047 = 71 , xREJ_Box8               = 71 , //
    x0048 = 72 , xREJ_Box9               = 72 , //
    x0049 = 73 , xREJ_Box10              = 73 , //
    x004A = 74 , xPRB_TempAlrm           = 74 , //
    x004B = 75 , xWRK_TempAlrm           = 75 , //
    x004C = 76 , xPSB_TempAlrm           = 76 , //
    x004D = 77 , x077                    = 77 , //
    x004E = 78 , x078                    = 78 , //
    x004F = 79 , x079                    = 79 , //

    x0050 = 80 , xETC_TempAlrm1          = 80 , //
    x0051 = 81 , xETC_TempWire1          = 81 , //
    x0052 = 82 , xETC_TempAlrm2          = 82 , //
    x0053 = 83 , xETC_TempWire2          = 83 , //
    x0054 = 84 , xETC_TempAlrm3          = 84 , //
    x0055 = 85 , xETC_TempWire3          = 85 , //
    x0056 = 86 , xETC_TempAlrm4          = 86 , //
    x0057 = 87 , xETC_TempWire4          = 87 , //
    x0058 = 88 , xETC_TempAlrm5          = 88 , //
    x0059 = 89 , xETC_TempWire5          = 89 , //
    x005A = 90 , xETC_TempAlrm6          = 90 , //
    x005B = 91 , xETC_TempWire6          = 91 , //
    x005C = 92 , xETC_TempAlrm7          = 92 , //
    x005D = 93 , xETC_TempWire7          = 93 , //
    x005E = 94 , xETC_TempAlrm8          = 94 , //
    x005F = 95 , xETC_TempWire8          = 95 , //

    x0060 = 96 , xLDR_PusherOverLoad     = 96 , //
    x0061 = 97 , xLDR_PusherFw           = 97 , //
    x0062 = 98 , xLDR_PusherBw           = 98 , //
    x0063 = 99 , xLDR_MgzDetect1         = 99 , //
    x0064 =100 , xLDR_MgzDetect2         =100 , //
    x0065 =101 , xLDR_ClampFw            =101 , //
    x0066 =102 , xLDR_MgzClamp1          =102 , //
    x0067 =103 , xLDR_MgzClamp2          =103 , //
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
    x0072 =114 , xULD_ClampFw            =114 , //
    x0073 =115 , xULD_MgzClamp1          =115 , //
    x0074 =116 , xULD_MgzClamp2          =116 , //
    x0075 =117 , xULD_MgzOutFull         =117 , //
    x0076 =118 , xULD_MgzIn              =118 , //
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
    y000F = 15 , yPRB_FeedingAC          = 15 , //

    y0010 = 16 , yPRB_AirBlower          = 16 , //
    y0011 = 17 , x017                    = 17 , //
    y0012 = 18 , yPR1_ClampFw            = 18 , //
    y0013 = 19 , yPR1_ClampBw            = 19 , //
    y0014 = 20 , yPR2_ClampFw            = 20 , //
    y0015 = 21 , yPR2_ClampBw            = 21 , //
    y0016 = 22 , yWRK_ClampFw            = 22 , //
    y0017 = 23 , yWRK_ClampBw            = 23 , //
    y0018 = 24 , yPSB_ClampFw            = 24 , //
    y0019 = 25 , yPSB_ClampBw            = 25 , //
    y001A = 26 , ySTL_YTopFw             = 26 , //
    y001B = 27 , ySTL_YBottomFw          = 27 , //
    y001C = 28 , ySTL_Ejecter            = 28 , //
    y001D = 29 , yPSB_PusherFw           = 29 , //
    y001E = 30 , yPSB_FeedingAC          = 30 , //
    y001F = 31 , yPSB_RollerFw           = 31 , //

    y0020 = 32 , ySTL_VaccSol            = 32 , //
    y0021 = 33 , ySTL_PnchVacc           = 33 , //
    y0022 = 34 , ySTL_PnchEjct           = 34 , //
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

    y0040 = 64 , yLDR_MgzInAC            = 64 , //
    y0041 = 65 , yLDR_MgzOutAC           = 65 , //
    y0042 = 66 , yLDR_ClampBw            = 66 , //
    y0043 = 67 , yLDR_ClampFw            = 67 , //
    y0044 = 68 , yLDR_PusherFw           = 68 , //
    y0045 = 69 , yLDR_ZBreak             = 69 , //
    y0046 = 70 , y070                    = 70 , //
    y0047 = 71 , y071                    = 71 , //
    y0048 = 72 , y072                    = 72 , //
    y0049 = 73 , y073                    = 73 , //
    y004A = 74 , y074                    = 74 , //
    y004B = 75 , y075                    = 75 , //
    y004C = 76 , y076                    = 76 , //
    y004D = 77 , y077                    = 77 , //
    y004E = 78 , y078                    = 78 , //
    y004F = 79 , y079                    = 79 , //

    y0050 = 80 , yULD_MgzOutAC           = 80 , //
    y0051 = 81 , yULD_MgzInAC            = 81 , //
    y0052 = 82 , yULD_ClampBw            = 82 , //
    y0053 = 83 , yULD_ClampFw            = 83 , //
    y0054 = 84 , yULD_ZBreak             = 84 , //
    y0055 = 85 , y085                    = 85 , //
    y0056 = 86 , y086                    = 86 , //
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
    piPR1_XWait       = 0  ,  //      
    piPR1_XGrip            ,  //      
    piPR1_XPull            ,
    piPR1_XBack            ,          
    piPR1_XOut             ,  //      
    piPR1_XWork            ,  //      
    MAX_PSTN_ID_MOTR0      ,  //      
                                      
    piPR2_XWait       = 0  ,  //
    piPR2_XGrip            ,  //      
    piPR2_XPull            ,  //      
    piPR2_XBack            ,  //      
    piPR2_XIdCheck         ,  //      
    piPR2_XOut             ,  //      
    piPR2_XWork            ,  //      
    MAX_PSTN_ID_MOTR1      ,  //

    piWRK_XWait       =  0 ,  //
    piWRK_XGrip            ,  //
    piWRK_XWorkStart       ,  //
    piWRK_XWork            ,  //
    piWRK_XGripOfs         ,
    MAX_PSTN_ID_MOTR2      ,  //

    piPSB_XWait       = 0  ,  //
    piPSB_XGrip            ,  //
    piPSB_XPull            ,  //
    piPSB_XBack            ,  //
    piPSB_XWorkStart       ,  //
    piPSB_XOut             ,  //
    piPSB_XWRKGrip         ,  //
    piPSB_XReGrip          ,  //
    piPSB_XWork            ,  //
    piPSB_XDrag            ,  //
    piPSB_XReBack          ,  //
    piPSB_XWRKWork         ,  //
    piPSB_XGripOfs         ,  //
    MAX_PSTN_ID_MOTR3      ,  //
                              //      
    piWRK_ZTWait      =  0 ,  //
    piWRK_ZTWork           ,  //
    MAX_PSTN_ID_MOTR4      ,  //

    piWRK_ZBWait      =  0 ,  //      
    piWRK_ZBWork           ,  //      
    piWRK_ZBChange         ,  //      
    MAX_PSTN_ID_MOTR5      ,  //      
                                      
    piWRK_YBWait      =  0 ,  //      
    piWRK_YBWork           ,  //
    piWRK_YBChange         ,  //
    MAX_PSTN_ID_MOTR6      ,  //

    piWRK_XBWait      =  0 ,  //
    piWRK_XBWork           ,  //
    MAX_PSTN_ID_MOTR7      ,  //

    piSTL_YTWait      =  0 ,  //
    piSTL_YTWorkStart      ,  //
    piSTL_YTDevCheck       ,  //
    piSTL_YTWork           ,  //
    piSTL_YTPlace          ,  //Place 포지션.
    MAX_PSTN_ID_MOTR8      ,  //

    piREJ_XBWait      =  0 ,  //
    piREJ_XBWorkStart      ,  //
    piREJ_XBWork           ,
    MAX_PSTN_ID_MOTR9      ,  //

    piLDR_YWait       =  0 ,  //
    piLDR_YWork            ,  //
    piLDR_YPick            ,  //
    piLDR_YPlace           ,  //
    MAX_PSTN_ID_MOTR10     ,  //

    piLDR_ZWait       =  0 ,  //
    piLDR_ZPickFwd         ,  //
    piLDR_ZClampOn         ,  //
    piLDR_ZPickBwd         ,  //
    piLDR_ZWorkStart       ,  //맨 아래 슬롯 기준.
    piLDR_ZPlaceFwd        ,  //
    piLDR_ZClampOff        ,  //
    piLDR_ZPlaceBwd        ,  //
    piLDR_ZWork            ,  //
    MAX_PSTN_ID_MOTR11     ,  //

    piULD_YWait       =  0 ,  //
    piULD_YWork            ,  //
    piULD_YPick            ,  //
    piULD_YPlace           ,  //
    MAX_PSTN_ID_MOTR12     ,  //

    piULD_ZWait       =  0 ,  //
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
    pvPR1_XWait       = 0  , //
    pvPR1_XGrip            , //
    pvPR1_XPull            , //
    pvPR1_XBack            , //
    pvPR1_XOut             , //
    MAX_PSTN_MOTR0         , //

    pvPR2_XWait       = 0  , //
    pvPR2_XGrip            , //
    pvPR2_XPull            , //
    pvPR2_XBack            , //
    pvPR2_XIdCheck         , //
    pvPR2_XOut             , //
    MAX_PSTN_MOTR1         , //

    pvWRK_XWait       =  0 , //
    pvWRK_XGrip            , // 
    pvWRK_XWorkStart       , // 
    MAX_PSTN_MOTR2         , // 

    pvPSB_XWait       = 0  , //
    pvPSB_XGrip            , //
    pvPSB_XPull            , //
    pvPSB_XBack            , //
    pvPSB_XWorkStart       , //
    pvPSB_XOut             , //
    pvPSB_XWRKGrip         , //
    MAX_PSTN_MOTR3         , //

    pvWRK_ZTWait      =  0 , //
    pvWRK_ZTWork           , //
    MAX_PSTN_MOTR4         , //

    pvWRK_ZBWait      =  0 , //
    pvWRK_ZBWork           , //
    pvWRK_ZBChange         , //
    MAX_PSTN_MOTR5         , //

    pvWRK_YBWait      =  0 , //
    pvWRK_YBWork           , //
    pvWRK_YBChange         ,
    MAX_PSTN_MOTR6         , //

    pvWRK_XBWait      =  0 , //
    pvWRK_XBWork           ,
    MAX_PSTN_MOTR7         , //

    pvSTL_YTWait      =  0 , //
    pvSTL_YTWorkStart      , //
    pvSTL_YTDevCheck       , //
    pvSTL_YTPlace          , // 3040떄문에 추가 JS.
    MAX_PSTN_MOTR8         , //

    pvREJ_XBWait      =  0 ,
    pvREJ_XBWorkStart      ,
    MAX_PSTN_MOTR9         ,

    pvLDR_YWait       =  0 , //
    pvLDR_YWork            , //
    pvLDR_YPick            , //
    pvLDR_YPlace           , //
    MAX_PSTN_MOTR10         , //

    pvLDR_ZWait       =  0 , //
    pvLDR_ZPickFwd         , //
    pvLDR_ZClampOn         , //
    pvLDR_ZPickBwd         , //
    pvLDR_ZWorkStart       , //
    pvLDR_ZPlaceFwd        , //
    pvLDR_ZClampOff        , //
    pvLDR_ZPlaceBwd        , //
    MAX_PSTN_MOTR11        , //

    pvULD_YWait       =  0 ,
    pvULD_YWork            ,
    pvULD_YPick            ,
    pvULD_YPlace           ,
    MAX_PSTN_MOTR12        ,

    pvULD_ZWait       =  0 ,
    pvULD_ZPickFwd         ,
    pvULD_ZClampOn         ,
    pvULD_ZPickBwd         ,
    pvULD_ZWorkStart       ,
    pvULD_ZPlaceFwd        ,
    pvULD_ZClampOff        ,
    pvULD_ZPlaceBwd        ,
    MAX_PSTN_MOTR13        ,


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
    mcLDR_Pusher                ,
    mcLDR_Clamp                 ,
    mcLDR_TranInAC              ,
    mcLDR_TranOutAC             ,

    mcPRE_IDCyl            = 30 ,
    mcPR1_Home                  ,
    mcPR1_In                    ,
    mcPR1_Drag                  ,
    mcPR1_Out                   ,
    mcPR1_Clamp                 ,
    mcPR1_Feeding               ,
    mcPR2_Home                  ,
    mcPR2_Clamp                 ,
    mcPR2_In                    ,
    mcPR2_Drag                  ,
    mcPR2_Out                   ,

    mcWRK_Home             = 50 ,
    mcWRK_Work                  ,
    mcWRK_NextMove              ,
    mcWRK_Clamp                 ,
    mcWRK_ToolChng              ,
    mcWRK_RptCntct              ,
    mcWRK_ToolContect           ,

    mcPSB_Home             = 60 ,
    mcPSB_WorkZoneCycle         ,
    mcPSB_Drag                  ,
    mcPSB_Out                   ,
    mcPSB_Clamp                 ,
    mcPSB_NextMove              ,
    mcPSB_Feeding               ,
    mcPSB_R1Pitch               ,
    mcPSB_L1Pitch               ,
    mcSTL_Pick                  ,
    mcSTL_YTopClamp             ,
    mcSTL_YBtmClamp             ,
    mcSTL_YPchUp1Pitch          ,
    mcSTL_YPchDn1Pitch          ,
    mcSTL_YDevUp1Pitch          ,
    mcSTL_YDevDn1Pitch          ,

    mcULD_Home             = 80 ,
    mcULD_Supply                ,
    mcULD_Pick                  ,
    mcULD_SttPstn               ,
    mcULD_Place                 ,
    mcULD_UpSlPitch             ,
    mcULD_DnSlPitch             ,
    mcULD_Clamp                 ,
    mcPSB_Pusher                ,
    mcULD_TranInAC              ,
    mcULD_TranOutAC             ,

    mcETC_AllClampOff      = 100,
    mcETC_AirBlower             ,
    mcETC_PrbStt                ,
    mcETC_TPrbUp                ,
    mcETC_TPrbDn                ,
    mcETC_BPrbUp                ,
    mcETC_BPrbDn                ,
    mcETC_LDRULDPlace           ,
    mcETC_PSBPlace              ,

    MAX_MANUAL_CYCLE
};

#endif



