#ifndef UTILDEFINEH
#define UTILDEFINEH

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum EN_ARAY_ID {
    riLD1       =0 , //로더
    riPR1          , //프리버퍼존 1 로더에서 투입 완료 된곳.
    riPR2          , //프리버퍼존 1에서 스텝 타고 드레그 타고 풀,백,ID 체크 마친곳.
    riWRP          , //프리버퍼에서 건네 받자 마자
    riWRK          , //작업중.
    riPS1          , //워크존에서 빠지자 마자~ 트림 완료까지.
    riPS2          , //언로더 진입 대기.
//    riPSH          ,
    riTmp          , //메뉴얼 리핏을 위해서 더미 어레이 생성.
    riSPC          ,
    riSTL          ,
    riREJ          ,
    riULD          ,

    MAX_ARAY
};

enum EN_MOTR_ID {
    miLD1_YCmp  =0 ,    //
    miLD1_ZCmp     ,    //
    miPRB_XCmp     ,    //
    miWRK_XCmp     ,    //
    miPSB_XCmp     ,    //
    miWRK_ZTop     ,    //
    miWRK_ZBtm     ,    //
    miSTL_YTop     ,    //
    miREJ_XBtm     ,    //
    miSTL_YBtm     ,    //
    miULD_YCmp     ,    //
    miULD_ZCmp     ,    //

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiPRB_Cmp   = 0,     //
    aiPRB_IdChk    ,     //
    aiWRK_Cmp      ,     //
    aiPSB_Cmp      ,     //
    aiLD1_Cmp      ,     //
    aiLD1_Psh      ,     //
    aiSTL_Top      ,     //
    aiSTL_Btm      ,     //
    aiPSB_Psh      ,     //
    aiULD_Cmp      ,     //

    MAX_ACTR
};

enum EN_ERR_ID {
    eiETC_MainAir      = 0 ,   //000 eiETC_MainAir
    eiETC_ToStartTO        ,   //001 eiETC_ToStartTO
    eiETC_ToStopTO         ,   //002 eiETC_ToStopTO
    eiETC_Emergency        ,   //003 eiETC_Emergency
    eiETC_FDoor            ,   //004 eiETC_FDoor
    eiETC_BDoor            ,   //005 eiETC_BDoor
    eiETC_LotEnd           ,   //006 eiETC_LotEnd
    eiETC_AllHomeTO        ,   //007 eiETC_AllHomeTO
    eiETC_ManCycleTO       ,   //008 eiETC_ManCycleTO
    eiPRB_CycleTO          ,   //009 eiPRB_CycleTO
    eiWRK_CycleTO          ,   //010 eiWRK_CycleTO
    eiPSB_CycleTO          ,   //011 eiPSB_CycleTO
    eiLD1_CycleTO          ,   //012 eiLD1_CycleTO
    eiSTL_CycleTO          ,   //013 eiSTL_CycleTO
    eiREJ_CycleTO          ,   //014 eiREJ_CycleTO
    eiULD_CycleTO          ,   //015 eiULD_CycleTO
    eiPRB_HomeTo           ,   //016 eiPRB_HomeTo
    eiPSB_HomeTo           ,   //017 eiPSB_HomeTo
    eiWRK_HomeTo           ,   //018 eiWRK_HomeTo
    eiLD1_HomeTO           ,   //019 eiLD1_HomeTO
    eiSTL_HomeTo           ,   //020 eiSTL_HomeTo
    eiREJ_HomeTo           ,   //021 eiREJ_HomeTo
    eiULD_HomeTO           ,   //022 eiULD_HomeTO
    eiPRB_ToStartTO        ,   //023 eiPRB_ToStartTO
    eiPSB_ToStartTO        ,   //024 eiPSB_ToStartTO
    eiWRK_ToStartTO        ,   //025 eiWRK_ToStartTO
    eiLD1_ToStartTO        ,   //026 eiLD1_ToStartTO
    eiSTL_ToStartTO        ,   //027 eiSTL_ToStartTO
    eiREJ_ToStartTO        ,   //028 eiREJ_ToStartTO
    eiULD_ToStartTO        ,   //029 eiULD_ToStartTO
    eiPRB_ToStopTO         ,   //030 eiPRB_ToStopTO
    eiPSB_ToStopTO         ,   //031 eiPSB_ToStopTO
    eiWRK_ToStopTO         ,   //032 eiWRK_ToStopTO
    eiLD1_ToStopTO         ,   //033 eiLD1_ToStopTO
    eiSTL_ToStopTO         ,   //034 eiSTL_ToStopTO
    eiREJ_ToStopTO         ,   //035 eiREJ_ToStopTO
    eiULD_ToStopTO         ,   //036 eiULD_ToStopTO
    eiLD1_ZCmpHomeEnd      ,   //037 eiLD1_ZCmpHomeEnd
    eiLD1_ZCmpNegLim       ,   //038 eiLD1_ZCmpNegLim
    eiLD1_ZCmpPosLim       ,   //039 eiLD1_ZCmpPosLim
    eiLD1_ZCmpAlarm        ,   //040 eiLD1_ZCmpAlarm
    eiLD1_YCmpHomeEnd      ,   //041 eiLD1_YCmpHomeEnd
    eiLD1_YCmpNegLim       ,   //042 eiLD1_YCmpNegLim
    eiLD1_YCmpPosLim       ,   //043 eiLD1_YCmpPosLim
    eiLD1_YCmpAlarm        ,   //044 eiLD1_YCmpAlarm
    eiPSB_XCmpHomeEnd      ,   //045 eiPSB_XCmpHomeEnd
    eiPSB_XCmpPosLim       ,   //046 eiPSB_XCmpPosLim
    eiPSB_XCmpAlarm        ,   //047 eiPSB_XCmpAlarm
    eiWRK_XCmpHomeEnd      ,   //048 eiWRK_XCmpHomeEnd
    eiWRK_XCmpPosLim       ,   //049 eiWRK_XCmpPosLim
    eiWRK_XCmpAlarm        ,   //050 eiWRK_XCmpAlarm
    eiPRB_XCmpHomeEnd      ,   //051 eiPRB_XCmpHomeEnd
    eiPRB_XCmpPosLim       ,   //052 eiPRB_XCmpPosLim
    eiPRB_XCmpAlarm        ,   //053 eiPRB_XCmpAlarm
    eiWRK_ZTopHomeEnd      ,   //054 eiWRK_ZTopHomeEnd
    eiWRK_ZTopNegLim       ,   //055 eiWRK_ZTopNegLim
    eiWRK_ZTopPosLim       ,   //056 eiWRK_ZTopPosLim
    eiWRK_ZTopAlarm        ,   //057 eiWRK_ZTopAlarm
    eiWRK_ZBtmHomeEnd      ,   //058 eiWRK_ZBtmHomeEnd
    eiWRK_ZBtmNegLim       ,   //059 eiWRK_ZBtmNegLim
    eiWRK_ZBtmAlarm        ,   //060 eiWRK_ZBtmAlarm
    eiSTL_YTopHomeEnd      ,   //061 eiSTL_YTopHomeEnd
    eiSTL_YTopNegLim       ,   //062 eiSTL_YTopNegLim
    eiSTL_YTopPosLim       ,   //063 eiSTL_YTopPosLim
    eiSTL_YTopAlarm        ,   //064 eiSTL_YTopAlarm
    eiREJ_XBtmHomeEnd      ,   //065 eiREJ_XBtmHomeEnd
    eiREJ_XBtmNegLim       ,   //066 eiREJ_XBtmNegLim
    eiREJ_XBtmPosLim       ,   //067 eiREJ_XBtmPosLim
    eiREJ_XBtmAlarm        ,   //068 eiREJ_XBtmAlarm
    eiSTL_YBtmHomeEnd      ,   //069 eiSTL_YBtmHomeEnd
    eiSTL_YBtmNegLim       ,   //070 eiSTL_YBtmNegLim
    eiSTL_YBtmPosLim       ,   //071 eiSTL_YBtmPosLim
    eiSTL_YBtmAlarm        ,   //072 eiSTL_YBtmAlarm
    eiULD_ZCmpHomeEnd      ,   //073 eiULD_ZCmpHomeEnd
    eiULD_ZCmpNegLim       ,   //074 eiULD_ZCmpNegLim
    eiULD_ZCmpPosLim       ,   //075 eiULD_ZCmpPosLim
    eiULD_ZCmpAlarm        ,   //076 eiULD_ZCmpAlarm
    eiULD_YCmpHomeEnd      ,   //077 eiULD_YCmpHomeEnd
    eiULD_YCmpNegLim       ,   //078 eiULD_YCmpNegLim
    eiULD_YCmpPosLim       ,   //079 eiULD_YCmpPosLim
    eiULD_YCmpAlarm        ,   //080 eiULD_YCmpAlarm
    eiPRB_CmpTO            ,   //081 eiPRB_CmpTO
    eiPRB_IdChkTO          ,   //082 eiPRB_IdChkTO
    eiWRK_CmpTO            ,   //083 eiWRK_CmpTO
    eiPSB_CmpTO            ,   //084 eiPSB_CmpTO
    eiLD1_CmpTO            ,   //085 eiLD1_CmpTO
    eiLD1_PushTO           ,   //086 eiLD1_PushTO
    eiSTL_CmpTO            ,   //087 eiSTL_CmpTO
    eiULD_CmpTO            ,   //088 eiULD_CmpTO
    eiULD_PushTO           ,   //089 eiULD_PushTO
    eiLD1_ClampDispr       ,   //090 eiLD1_ClampDispr
    eiLD1_ClampUnknown     ,   //091 eiLD1_ClampUnknown
    eiLD1_MgzChange        ,   //092 eiLD1_MgzChange
    eiPRB_Dispr            ,   //093 eiPRB_Dispr
    eiPRB_Unknown          ,   //094 eiPRB_Unknown
    eiWRK_Dispr            ,   //095 eiWRK_Dispr
    eiWRK_Unknown          ,   //096 eiWRK_Unknown
    eiPSB_Dispr            ,   //097 eiPSB_Dispr
    eiPSB_Unknown          ,   //098 eiPSB_Unknown
    eiWRK_FailCntOver      ,   //099 eiWRK_FailCntOver
    eiWRK_FailProbeComm1   ,   //100 eiWRK_FailProbeComm1
    eiWRK_FailProbeComm2   ,   //101 eiWRK_FailProbeComm2
    eiLD1_MgzFull          ,   //102 eiLD1_MgzFull
    eiLD1_PusherOverload   ,   //103 eiLD1_PusherOverload
    eiPRB_Overload         ,   //104 eiPRB_Overload
    eiWRK_Overload         ,   //105 eiWRK_Overload
    eiPSB_Overload         ,   //106 eiPSB_Overload
    eiPRB_IdCheck          ,   //107 eiPRB_IdCheck
    eiPRB_TempZ1Alrm1      ,   //108 eiPRB_TempZ1Alrm1
    eiPRB_TempZ1Wire1      ,   //109 eiPRB_TempZ1Wire2
    eiPRB_TempZ1Alrm2      ,   //110 eiPRB_TempZ1Alrm2
    eiPRB_TempZ1Wire2      ,   //111 eiPRB_TempZ1Wire3
    eiPRB_TempZ1Alrm3      ,   //112 eiPRB_TempZ1Alrm3
    eiPRB_TempZ1Wire3      ,   //113 eiPRB_TempZ1Wire4
    eiPRB_TempZ1Alrm4      ,   //114 eiPRB_TempZ1Alrm4
    eiPRB_TempZ1Wire4      ,   //115 eiPRB_TempZ1Alrm5
    eiPSB_TempZ2Alrm1      ,   //116 eiPSB_TempZ2Alrm1
    eiPSB_TempZ2Wire1      ,   //117 eiPSB_TempZ2Wire1
    eiPSB_TempZ2Alrm2      ,   //118 eiPSB_TempZ2Alrm2
    eiPSB_TempZ2Wire2      ,   //119 eiPSB_TempZ2Wire2
    eiPSB_TempZ2Alrm3      ,   //120 eiPSB_TempZ2Alrm3
    eiPSB_TempZ2Wire3      ,   //121 eiPSB_TempZ2Wire3
    eiPSB_TempZ2Alrm4      ,   //122 eiPSB_TempZ2Alrm4
    eiPSB_TempZ2Wire4      ,   //123 eiPSB_TempZ2Wire4
    eiETC_NetSeverFail     ,   //124 eiETC_NetSeverFail
    eiETC_NetConectFail    ,   //125 eiETC_NetConectFail
    eiETC_PostEqpLotStr    ,   //126 eiETC_PostEqpLotStr
    eiETC_PostEqpLotEnd    ,   //127 eiETC_PostEqpLotEnd
    eiETC_PostEqpModelChg  ,   //128 eiETC_PostEqpModelChg
    eiETC_PostEqpData      ,   //129 eiETC_PostEqpData
    eiETC_PostEqpNotReady  ,   //130 eiETC_PostEqpNotReady
    eiETC_PostEqpCommTO    ,   //131 eiETC_PostEqpCommTO
    eiWRK_PinFailCntOver   ,   //132 eiWRK_PinFailCntOver
    eiWRK_Probe1DataWrong  ,   //133 eiWRK_Probe1DataWrong
    eiWRK_Probe2DataWrong  ,   //134 eiWRK_Probe2DataWrong
    eiMNR_StripRemoveTo    ,   //135 eiMNR_StripRemoveTo
    eiPRB_StripOver        ,   //136 eiPRB_StripOver
    eiLD1_NeedMgz          ,   //137 eiLD1_NeedMgz
    eiWRK_ProbeSetFail     ,   //138 eiWRK_ProbeSetFail
    eiULD_MgzUnknown       ,   //139 eiULD_MgzUnknown
    eiULD_MgzDispr         ,   //140 eiULD_MgzDispr
    eiULD_MgzFull          ,   //141 eiULD_MgzFull
    eiULD_NeedMgz          ,   //142 eiULD_NeedMgz
    eiREJ_BoxFull          ,   //143 eiREJ_BoxFull
    eiREJ_Box1             ,   //144 eiREJ_Box1
    eiREJ_Box2             ,   //145 eiREJ_Box2
    eiREJ_Box3             ,   //146 eiREJ_Box3
    eiREJ_Box4             ,   //147 eiREJ_Box4
    eiREJ_Box5             ,   //148 eiREJ_Box5
    eiREJ_Box6             ,   //149 eiREJ_Box6
    eiREJ_Box7             ,   //150 eiREJ_Box7
    eiREJ_Box8             ,   //151 eiREJ_Box8
    eiREJ_Box9             ,   //152 eiREJ_Box9
    eiREJ_Box10            ,   //153 eiREJ_Box10
    eiREJ_MaxFullBox       ,   //154 eiREJ_MaxFullBox
    eiSTL_PickUp           ,   //155 eiSTL_PickUp
    eiSTL_Place            ,   //156 eiSTL_Place
    eiPSB_Out1Sensor       ,   //157 eiPSB_Out1Sensor
    eiPSB_Out2Detected     ,   //158 eiPSB_Out2Detected
    eiPSB_Out2NotDetected  ,   //159 eiPSB_Out2NotDetected
    eiSTL_MgzUnknow        ,   //160 eiSTL_MgzUnknow
    eiSTL_Trim             ,   //161 eiSTL_MgzUnknow
    eiSTL_Trim1            ,   //162 eiSTL_MgzUnknow
    eiWRK_LimPinCnt        ,   //163 eiWRK_LimPinCnt
    eiWRK_ShrtMonitor      ,   //164 eiWRK_ShrtMonitor
    eiPRB_TempZ1Alrm5      ,   //165 eiPRB_TempZ1Alrm1
    MAX_ERR
};

enum EN_INPUT_ID {
    x0000 =  0 ,  xLD1_PusherOverLoad      =  0 , //
    x0001 =  1 ,  xLD1_PusherFw            =  1 , //
    x0002 =  2 ,  xLD1_PusherBw            =  2 , //
    x0003 =  3 ,  xLD1_MgzDetect1          =  3 , //
    x0004 =  4 ,  xLD1_MgzDetect2          =  4 , //
    x0005 =  5 ,  xLD1_ClampFw             =  5 , //
    x0006 =  6 ,  xLD1_MgzClamp1           =  6 , //
    x0007 =  7 ,  xLD1_MgzClamp2           =  7 , //
    x0008 =  8 ,  xLD1_MgzIn               =  8 , //
    x0009 =  9 ,  xLD1_MgzOutFull          =  9 , //
    x000A = 10 ,  x010                     = 10 , //
    x000B = 11 ,  x011                     = 11 , //
    x000C = 12 ,  xLD1_PowerOnAC           = 12 , //
    x000D = 13 ,  xLD1_PowerOnDC           = 13 , //
    x000E = 14 ,  xETC_LEmgLD1Sw           = 14 , //
    x000F = 15 ,  x015                     = 15 , //

    x0010 = 16 ,  xULD_MgzDetect1          = 16 , //
    x0011 = 17 ,  xULD_MgzDetect2          = 17 , //
    x0012 = 18 ,  xULD_ClampFw             = 18 , //
    x0013 = 19 ,  xULD_MgzClamp1           = 19 , //
    x0014 = 20 ,  xULD_MgzClamp2           = 20 , //
    x0015 = 21 ,  xULD_MgzOutFull          = 21 , //
    x0016 = 22 ,  xULD_MgzIn               = 22 , //
    x0017 = 23 ,  x023                     = 23 , //
    x0018 = 24 ,  xULD_PowerOnAC           = 24 , //
    x0019 = 25 ,  xULD_PowerOnDC           = 25 , //
    x001A = 26 ,  xETC_LEmgULDSw           = 26 , //
    x001B = 27 ,  x027                     = 27 , //
    x001C = 28 ,  x028                     = 28 , //
    x001D = 29 ,  x029                     = 29 , //
    x001E = 30 ,  x030                     = 30 , //
    x001F = 31 ,  x031                     = 31 , //

    x0020 = 32 ,  xETC_LStartSw            = 32 , //
    x0021 = 33 ,  xETC_LStopSw             = 33 , //
    x0022 = 34 ,  xETC_LResetSw            = 34 , //
    x0023 = 35 ,  xETC_LMainAir            = 35 , //
    x0024 = 36 ,  xETC_LInitSw             = 36 , //
    x0025 = 37 ,  xETC_RStartSw            = 37 , //
    x0026 = 38 ,  xETC_RStopSw             = 38 , //
    x0027 = 39 ,  xETC_RResetSw            = 39 , //
    x0028 = 40 ,  xETC_RMainAir            = 40 , //
    x0029 = 41 ,  xETC_RInitSw             = 41 , //
    x002A = 42 ,  x042                     = 42 , //
    x002B = 43 ,  x043                     = 43 , //
    x002C = 44 ,  x044                     = 44 , //
    x002D = 45 ,  xETC_EmgSw1              = 45 , //
    x002E = 46 ,  xETC_EmgSw2              = 46 , //
    x002F = 47 ,  x047                     = 47 , //

    x0030 = 48 ,  x032                     = 48 , //
    x0031 = 49 ,  xETC_PowerOn             = 49 , //
    x0032 = 50 ,  xETC_MainSol             = 50 , //
    x0033 = 51 ,  xETC_FDoor               = 51 , //
    x0034 = 52 ,  xETC_BDoor               = 52 , //
    x0035 = 53 ,  xPRB_IdCheck             = 53 , //
    x0036 = 54 ,  xPSB_IdCheckBw           = 54 , //
    x0037 = 55 ,  xPSB_IdCheckFw           = 55 , //
    x0038 = 56 ,  xPRB_Overload            = 56 , //
    x0039 = 57 ,  xWRK_Overload            = 57 , //
    x003A = 58 ,  xPSB_Overload            = 58 , //
    x003B = 59 ,  xPRB_ClampFw             = 59 , //
    x003C = 60 ,  xPRB_ClampBw             = 60 , //
    x003D = 61 ,  xWRK_ClampFw             = 61 , //
    x003E = 62 ,  xWRK_ClampBw             = 62 , //
    x003F = 63 ,  xPRB_StripOver           = 63 , //

    x0040 = 64 ,  xPSB_ClampFw             = 64 , //
    x0041 = 65 ,  xPSB_ClampBw             = 65 , //
    x0042 = 66 ,  xWRK_1Pkg                = 66 , //
    x0043 = 67 ,  xWRK_2Pkg                = 67 , //
    x0044 = 68 ,  x052                     = 68 , //
    x0045 = 69 ,  xPRB_1Pkg                = 69 , //
    x0046 = 70 ,  xPRB_2Pkg                = 70 , //
    x0047 = 71 ,  xPRB_3Pkg                = 71 , //
    x0048 = 72 ,  xPSB_1Pkg                = 72 , //
    x0049 = 73 ,  xPSB_2Pkg                = 73 , //
    x004A = 74 ,  xPSB_3Pkg                = 74 , //
    x004B = 75 ,  xPSB_4Pkg                = 75 , //
    x004C = 76 ,  xPSB_PkgOut1             = 76 , //
    x004D = 77 ,  xPSB_PkgOut2             = 77 , //
    x004E = 78 ,  xPSB_PusherFw            = 78 , //
    x004F = 79 ,  xPSB_PusherBw            = 79 , //

    x0050 = 80 ,  xSTL_UnitUp              = 80 , //
    x0051 = 81 ,  xSTL_UnitDn              = 81 , //
    x0052 = 82 ,  xSTL_Vacuum              = 82 , //
    x0053 = 83 ,  xREJ_FullCheck           = 83 , //
    x0054 = 84 ,  x084                     = 84 , //
    x0055 = 85 ,  xREJ_Box1                = 85 , //
    x0056 = 86 ,  xREJ_Box2                 = 86 , //
    x0057 = 87 ,  xREJ_Box3                = 87 , //
    x0058 = 88 ,  xREJ_Box4                = 88 , //
    x0059 = 89 ,  xREJ_Box5                = 89 , //
    x005A = 90 ,  xREJ_Box6                = 90 , //
    x005B = 91 ,  xREJ_Box7                = 91 , //
    x005C = 92 ,  xREJ_Box8                = 92 , //
    x005D = 93 ,  xREJ_Box9                = 93 , //
    x005E = 94 ,  xREJ_Box10               = 94 , //
    x005F = 95 ,  x095                     = 95 , //

    x0060 = 96 ,  xSTL_PunchingUp          = 96 , //
    x0061 = 97 ,  xSTL_PunchingDn          = 97 , //
    x0062 = 98 ,  xSTL_DevDetect           = 98 , //
    x0063 = 99 ,  x099                     = 99 , //
    x0064 =100 ,  xPRB_TempZ1Alrm1         =100 , //
    x0065 =101 ,  xPRB_TempZ1Wire1         =101 , //
    x0066 =102 ,  xPRB_TempZ1Alrm2         =102 , //
    x0067 =103 ,  xPRB_TempZ1Wire2         =103 , //
    x0068 =104 ,  xPRB_TempZ1Alrm3         =104 , //
    x0069 =105 ,  xPRB_TempZ1Wire3         =105 , //
    x006A =106 ,  xPRB_TempZ1Alrm4         =106 , //
    x006B =107 ,  xPRB_TempZ1Wire4         =107 , //
    x006C =108 ,  xPRB_TempZ1Alrm5         =108 , //
    x006D =109 ,  x109                     =109 , //
    x006E =110 ,  xPSB_TempZ2Alrm1         =110 , //
    x006F =111 ,  xPSB_TempZ2Wire1         =111 , //

    x0070 =112 ,  xPSB_TempZ2Alrm2         =112 , //
    x0071 =113 ,  xPSB_TempZ2Wire2         =113 , //
    x0072 =114 ,  xPSB_TempZ2Alrm3         =114 , //
    x0073 =115 ,  xPSB_TempZ2Wire3         =115 , //
    x0074 =116 ,  xPSB_TempZ2Alrm4         =116 , //
    x0075 =117 ,  xPSB_TempZ2Wire4         =117 , //
    x0076 =118 ,  x118                     =118 , //
    x0077 =119 ,  x119                     =119 , //
    x0078 =120 ,  xETC_PstEqpStart         =120 , //
    x0079 =121 ,  xETC_PstEqpStop          =121 , //
    x007A =122 ,  xETC_PstEqpReset         =122 , //
    x007B =123 ,  xETC_PstEqpLotStart      =123 , //
    x007C =124 ,  xETC_PstEqpLotEnd        =124 , //
    x007D =125 ,  xETC_PstEqpSending       =125 , //
    x007E =126 ,  xETC_PstEqpJobChange     =126 , //
    x007F =127 ,  x127                     =127 , //

    x0080 =128 ,  xTEN_AlignReady          =128 , //
    x0081 =129 ,  xTEN_AlignComplete       =129 , //
    x0082 =130 ,  x130                     =130 , //
    x0083 =131 ,  x131                     =131 , //
    x0084 =132 ,  x132                     =132 , //
    x0085 =133 ,  x133                     =133 , //
    x0086 =134 ,  x134                     =134 , //
    x0087 =135 ,  x135                     =135 , //
    x0088 =136 ,  xTEN_MarkingReady        =136 , //
    x0089 =137 ,  xTEN_MarkingComplete     =137 , //
    x008A =138 ,  xTEN_MarkingData1        =138 , //
    x008B =139 ,  xTEN_MarkingData2        =139 , //
    x008C =140 ,  xTEN_OrientReady         =140 , //
    x008D =141 ,  xTEN_OrientComplete      =141 , //
    x008E =142 ,  xTEN_OrientData1         =142 , //
    x008F =143 ,  xTEN_OrientData2         =143 , //


    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yLD1_MgzInAC              =  0 , //
    y0001 =  1 ,  yLD1_MgzOutAC             =  1 , //
    y0002 =  2 ,  yLD1_ClampFw              =  2 , //
    y0003 =  3 ,  yLD1_ClampBw              =  3 , //
    y0004 =  4 ,  yLD1_PusherFw             =  4 , //
    y0005 =  5 ,  yLD1_ZBreak               =  5 , //
    y0006 =  6 ,  y006                      =  6 , //
    y0007 =  7 ,  y007                      =  7 , //
    y0008 =  8 ,  y008                      =  8 , //
    y0009 =  9 ,  y009                      =  9 , //
    y000A = 10 ,  y010                      = 10 , //
    y000B = 11 ,  y011                      = 11 , //
    y000C = 12 ,  y012                      = 12 , //
    y000D = 13 ,  y013                      = 13 , //
    y000E = 14 ,  y014                      = 14 , //
    y000F = 15 ,  y015                      = 15 , //

    y0010 = 16 ,  yULD_MgzOutAC             = 16 , //
    y0011 = 17 ,  yULD_MgzInAC              = 17 , //
    y0012 = 18 ,  yULD_ClampFw              = 18 , //
    y0013 = 19 ,  yULD_ClampBw              = 19 , //
    y0014 = 20 ,  yULD_ZBreak               = 20 , //
    y0015 = 21 ,  y021                      = 21 , //
    y0016 = 22 ,  y022                      = 22 , //
    y0017 = 23 ,  y023                      = 23 , //
    y0018 = 24 ,  y024                      = 24 , //
    y0019 = 25 ,  y025                      = 25 , //
    y001A = 26 ,  y026                      = 26 , //
    y001B = 27 ,  y027                      = 27 , //
    y001C = 28 ,  y028                      = 28 , //
    y001D = 29 ,  y029                      = 29 , //
    y001E = 30 ,  y030                      = 30 , //
    y001F = 31 ,  y031                      = 31 , //

    y0020 = 32 ,  yETC_LStartLp             = 32 , //
    y0021 = 33 ,  yETC_LStopLp              = 33 , //
    y0022 = 34 ,  yETC_LResetLp             = 34 , //
    y0023 = 35 ,  yETC_LAirLp               = 35 , //
    y0024 = 36 ,  yETC_LInitLp              = 36 , //
    y0025 = 37 ,  yETC_RStartLp             = 37 , //
    y0026 = 38 ,  yETC_RStopLp              = 38 , //
    y0027 = 39 ,  yETC_RResetLp             = 39 , //
    y0028 = 40 ,  yETC_RAirLp               = 40 , //
    y0029 = 41 ,  yETC_RInitLp              = 41 , //
    y002A = 42 ,  yETC_TwRedLp              = 42 , //
    y002B = 43 ,  yETC_TwYelLp              = 43 , //
    y002C = 44 ,  yETC_TwGrnLp              = 44 , //
    y002D = 45 ,  yETC_TwBzz                = 45 , //
    y002E = 46 ,  yETC_MainSol              = 46 , //
    y002F = 47 ,  yPRB_FeedingAC            = 47 , //

    y0030 = 48 ,  yPRB_AirBlower            = 48 , //
    y0031 = 49 ,  yPRB_ClampFw              = 49 , //
    y0032 = 50 ,  yPRB_ClampBw              = 50 , //
    y0033 = 51 ,  yWRK_ClampFw              = 51 , //
    y0034 = 52 ,  yWRK_ClampBw              = 52 , //
    y0035 = 53 ,  yPSB_ClampFw              = 53 , //
    y0036 = 54 ,  yPSB_ClampBw              = 54 , //
    y0037 = 55 ,  yPRB_ReverseFw            = 55 , //
    y0038 = 56 ,  yWRK_Probe1               = 56 , //
    y0039 = 57 ,  yWRK_Probe2               = 57 , //
    y003A = 58 ,  yPSB_PusherFw             = 58 , //
    y003B = 59 ,  yETC_MainSol2             = 59 , //
    y003C = 60 ,  yPSB_FeedingAC            = 60 , //
    y003D = 61 ,  y061                      = 61 , //
    y003E = 62 ,  yETC_BlowerAC             = 62 , //
    y003F = 63 ,  y063                      = 63 , //

    y0040 = 64 ,  ySTL_YTopFw               = 64 , //
    y0041 = 65 ,  ySTL_Vaccum               = 65 , //
    y0042 = 66 ,  ySTL_VaccumEje            = 66 , //
    y0043 = 67 ,  ySTL_YBottomFw            = 67 , //
    y0044 = 68 ,  y068                      = 68 , //
    y0045 = 69 ,  y069                      = 69 , //
    y0046 = 70 ,  y070                      = 70 , //
    y0047 = 71 ,  y071                      = 71 , //
    y0048 = 72 ,  yETC_Heater1_2            = 72 , //
    y0049 = 73 ,  yETC_Heater1_3            = 73 , //
    y004A = 74 ,  yETC_Heater2_2            = 74 , //
    y004B = 75 ,  yETC_Heater2_3            = 75 , //
    y004C = 76 ,  y076                      = 76 , //
    y004D = 77 ,  y077                      = 77 , //
    y004E = 78 ,  y078                      = 78 , //
    y004F = 79 ,  y079                      = 79 , //

    y0050 = 80 ,  y080                      = 80 , //
    y0051 = 81 ,  y081                      = 81 , //
    y0052 = 82 ,  y082                      = 82 , //
    y0053 = 83 ,  y083                      = 83 , //
    y0054 = 84 ,  y084                      = 84 , //
    y0055 = 85 ,  y085                      = 85 , //
    y0056 = 86 ,  y086                      = 86 , //
    y0057 = 87 ,  y087                      = 87 , //
    y0058 = 88 ,  yETC_PstEqpStart          = 88 , //
    y0059 = 89 ,  yETC_PstEqpStop           = 89 , //
    y005A = 90 ,  yETC_PstEqpReset          = 90 , //
    y005B = 91 ,  yETC_PstEqpLotStart       = 91 , //
    y005C = 92 ,  yETC_PstEqpLotEnd         = 92 , //
    y005D = 93 ,  yETC_PstEqpSending        = 93 , //
    y005E = 94 ,  yETC_PstEqpJobChange      = 94 , //
    y005F = 95 ,  y095                      = 95 , //

    y0060 = 96 ,  yTEN_AlignStart           = 96 , //
    y0061 = 97 ,  yTEN_AlignReset           = 97 , //
    y0062 = 98 ,  yTEN_OrientStart          = 98 , //
    y0063 = 99 ,  yTEN_OrientReset          = 99 , //
    y0064 =100 ,  y100                      =100 , //
    y0065 =101 ,  y101                      =101 , //
    y0066 =102 ,  y102                      =102 , //
    y0067 =103 ,  y103                      =103 , //
    y0068 =104 ,  yTEN_MarkingStart         =104 , //
    y0069 =105 ,  yTEN_MarkingReset         =105 , //
    y006A =106 ,  yTEN_MarkingTeaching      =106 , //
    y006B =107 ,  y107                      =107 , //
    y006C =108 ,  y108                      =108 , //
    y006D =109 ,  y109                      =109 , //
    y006E =110 ,  y110                      =110 , //
    y006F =111 ,  y111                      =111 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID {

    piLD1_YWait       =  0 ,  //
    piLD1_YWork            ,  //
    piLD1_YPick            ,  //
    piLD1_YPlace           ,  //
    MAX_PSTN_ID_MOTR0      ,  //

    piLD1_ZWait       =  0 ,  //
    piLD1_ZPickFwd         ,  //
    piLD1_ZClampOn         ,  //
    piLD1_ZPickBwd         ,  //
    piLD1_ZWorkStart       ,  //맨 아래 슬롯 기준.
    piLD1_ZPlaceFwd        ,  //
    piLD1_ZClampOff        ,  //
    piLD1_ZPlaceBwd        ,  //
    piLD1_ZWork            ,  //
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

    piWRK_XWait       =  0 ,  //
    piWRK_XGrip            ,  //
    piWRK_XWorkStart       ,  //
    piWRK_XOut             ,  //
    piWRK_XReGrip          ,  //
    piWRK_XWork            ,  //
//    piWRK_XReWork          ,  //
    MAX_PSTN_ID_MOTR3      ,  //

    piPSB_XWait       = 0  ,  //
    piPSB_XGrip            ,  //
    piPSB_XPull            ,  //
    piPSB_XBack            ,  //
    piPSB_XWorkStart       ,  //
    piPSB_XOut             ,  //
    piPSB_XReGrip          ,  //
    piPSB_XWork            ,  //
    piPSB_XDrag            ,  //
    piPSB_XReBack          ,  //
    MAX_PSTN_ID_MOTR4      ,  //
                              //
    piWRK_ZTWait      =  0 ,  //
    piWRK_ZTWork           ,  //
    MAX_PSTN_ID_MOTR5      ,  //

    piWRK_ZBWait      =  0 ,  //
    piWRK_ZBWork           ,  //
    MAX_PSTN_ID_MOTR6      ,  //

    piSTL_YTWait      =  0 ,  // 
    piSTL_YTWorkStart      ,  // 
    piSTL_YTPlace          ,  // 
    piSTL_YTDevCheck       ,
    piSTL_YTWork           ,  //
    MAX_PSTN_ID_MOTR7      ,  //

    piREJ_XBWait      =  0 ,  //
    piREJ_XBWorkStart      ,  //
    piREJ_XBWork           ,
    MAX_PSTN_ID_MOTR8      ,  //

    piSTL_YBWait      =  0 ,  //
    piSTL_YBWorkStart      ,  //
    piSTL_YBWork           ,  //
    piSTL_YBStep           ,
    MAX_PSTN_ID_MOTR9      ,  //

    piULD_YWait       =  0 ,  //
    piULD_YWork            ,  //
    piULD_YPick            ,  //
    piULD_YPlace           ,  //
    MAX_PSTN_ID_MOTR10     ,  //

    piULD_ZWait       =  0 ,  //
    piULD_ZPickFwd         ,  //
    piULD_ZClampOn         ,  //
    piULD_ZPickBwd         ,  //
    piULD_ZWorkStart       ,  //
    piULD_ZPlaceFwd        ,  //
    piULD_ZClampOff        ,  //
    piULD_ZPlaceBwd        ,  //
    piULD_ZWork            ,  //
    MAX_PSTN_ID_MOTR11     ,  //
};

enum EN_PSTN_VALUE {

    pvLD1_YWaitPs       =  0 , //
    pvLD1_YWorkPs            , //
    pvLD1_YPickPs            , //
    pvLD1_YPlacePs           , //
    MAX_PSTN_MOTR0           , //

    pvLD1_ZWaitPs       =  0 , //
    pvLD1_ZPickFwdPs         , //
    pvLD1_ZClampOnPs         , //
    pvLD1_ZPickBwdPs         , //
    pvLD1_ZWorkStartPs       , //
    pvLD1_ZPlaceFwdPs        , //
    pvLD1_ZClampOffPs        , //
    pvLD1_ZPlaceBwdPs        , //
    MAX_PSTN_MOTR1           , //

    pvPRB_XWaitPs       = 0  , //
    pvPRB_XGripPs            , //
    pvPRB_XPullPs            , //
    pvPRB_XBackPs            , //
    pvPRB_XIdCheck           , //
    pvPRB_XOut1Ps            , //
    pvPRB_XOut2Ps            , //
    MAX_PSTN_MOTR2           , //

    pvWRK_XWaitPs       =  0 , //
    pvWRK_XGripPs            , //
    pvWRK_XWorkStartPs       , //
    pvWRK_XOutPs             , //
    MAX_PSTN_MOTR3           , //

    pvPSB_XWaitPs       = 0  , //
    pvPSB_XGripPs            , //
    pvPSB_XPullPs            , //
    pvPSB_XBackPs            , //
    pvPSB_XWorkStartPs       , //
    pvPSB_XOutPs             , //
//    pvPSB_XReGripPs          , //
    MAX_PSTN_MOTR4           , //

    pvWRK_ZTWaitPs      =  0 , //
    pvWRK_ZTWorkPs           , //
    MAX_PSTN_MOTR5           , //

    pvWRK_ZBWaitPs      =  0 , //
    pvWRK_ZBWorkPs           , //
    MAX_PSTN_MOTR6           , //

    pvSTL_YTWaitPs      =  0 , //
    pvSTL_YTWorkStartPs      , //
    pvSTL_YTPlacePs          ,
    pvSTL_YTDevCheckPs       ,
    MAX_PSTN_MOTR7           ,

    pvREJ_XBWaitPs      =  0 ,
    pvREJ_XBWorkStartPs      ,
    MAX_PSTN_MOTR8           ,

    pvSTL_YBWaitPs      =  0 ,
    pvSTL_YBWorkStartPs      ,
    MAX_PSTN_MOTR9           ,

    pvULD_YWaitPs       =  0 ,
    pvULD_YWorkPs            ,
    pvULD_YPickPs            ,
    pvULD_YPlacePs           ,
    MAX_PSTN_MOTR10          ,

    pvULD_ZWaitPs       =  0 ,
    pvULD_ZPickFwdPs         ,
    pvULD_ZClampOnPs         ,
    pvULD_ZPickBwdPs         ,
    pvULD_ZWorkStartPs       ,
    pvULD_ZPlaceFwdPs        ,
    pvULD_ZClampOffPs        ,
    pvULD_ZPlaceBwdPs        ,
    MAX_PSTN_MOTR11          ,


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

    mcPSB_Home             = 50 ,
    mcPSB_Drag                  ,
    mcPSB_Out                   ,
    mcPSB_Clamp                 ,
    mcPSB_Feeding               ,
    mcPSB_R1Pitch               ,
    mcPSB_L1Pitch               ,
    mcSTL_Pick                  ,
    mcSTL_Place                 ,
    mcSTL_YTopClamp             ,
    mcSTL_YBtmClamp             ,
    mcSTL_YTopUp1Pitch          ,
    mcSTL_YTopDn1Pitch          ,
    mcSTL_YBtmUp1pitch          ,
    mcSTL_YBtmDn1pitch          ,

    mcULD_Home             = 70 ,
    mcULD_Supply                ,
    mcULD_Pick                  ,
    mcULD_SttPstn               ,
    mcULD_Place                 ,
    mcULD_UpSlPitch             ,
    mcULD_DnSlPitch             ,
    mcULD_Clamp                 ,
    mcPSB_PusherFw              ,
    mcPSB_PusherBw              ,
    mcULD_TranInAC              ,
    mcULD_TranOutAC             ,

    mcETC_AllClampOff      = 90 ,
//    mcETC_RemoveStrip           ,
//    mcETC_PreRemoveStrip        ,
//    mcETC_WrkRemoveStrip        ,
//    mcETC_PstRemoveStrip        ,
    mcETC_AirBlower             ,
    mcETC_6                     ,

    mcETC_PrbStt           = 100,
    mcETC_TPrbUp                ,
    mcETC_TPrbDn                ,
    mcETC_BPrbUp                ,
    mcETC_BPrbDn                ,
    mcETC_LDRULDPlace           ,


    MAX_MANUAL_CYCLE
};

#endif



