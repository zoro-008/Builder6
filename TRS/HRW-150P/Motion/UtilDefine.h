#ifndef UTILDEFINEH
#define UTILDEFINEH

enum EN_MOTR_ID {
    miTiltMt    = 0 ,
    miSpinMt    = 1 ,


    MAX_MOTR_ID
};


enum EN_ACTR_ID {
    aiDoor       = 1 ,

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
    eiPRB_TempZ1Alrm       ,  //132 ;
    eiWRK_TempZ1Alrm       ,  //133 ;
    eiPSB_TempZ1Alrm       ,  //134 ;
    ei135                  ,  //135 ;
    ei136                  ,  //136 ;
    ei137                  ,  //137 ;
    ei138                  ,  //138 ;
    ei139                  ,  //139 ;
    ei140                  ,  //140 ;
    ei141                  ,  //141 ;
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
    MAX_ERR
};

enum EN_INPUT_ID {                             //기존
    x0000 =  0 , xETC_StartSw           =  0 , //x0000 =  0 , xETC_StartSw           =  0 , //
    x0001 =  1 , xETC_StopSw            =  1 , //x0001 =  1 , xETC_StopSw            =  1 , //
    x0002 =  2 , xETC_EmgSw             =  2 , //x0002 =  2 , xETC_EmgSw             =  2 , //
    x0003 =  3 , xETC_DoorFw            =  3 , //x0003 =  3 , xETC_DoorFw            =  3 , //
    x0004 =  4 , xETC_DoorBw            =  4 , //x0004 =  4 , xETC_DoorBw            =  4 , //
    x0005 =  5 , xETC_MaskPosSnsr       =  5 , //x0005 =  5 , xETC_Mask              =  5 , //
    x0006 =  6 , xETC_MainAir           =  6 , //x0006 =  6 , xETC_MainAir           =  6 , //
    x0007 =  7 , xETC_MskDetectFw       =  7 , //x0007 =  7 , x007                   =  7 , //
    x0008 =  8 , xETC_MskDetectBw       =  8 , //x0008 =  8 , x008                   =  8 , //
    x0009 =  9 , xETC_MskDetectNot      =  9 , //x0009 =  9 , x009                   =  9 , //
    x000A = 10 , xETC_DoorSnsrNotDetect = 10 , //x000A = 10 , x010                   = 10 , //
    x000B = 11 , x011                   = 11 , //x000B = 11 , x011                   = 11 , //
    x000C = 12 , x012                   = 12 , //  = 12 , x012                   = 12 , //
    x000D = 13 , x013                   = 13 , //x000D = 13 , x013                   = 13 , //
    x000E = 14 , x014                   = 14 , //x000E = 14 , x014                   = 14 , //
    x000F = 15 , x015                   = 15 , //x000F = 15 , x015                   = 15 , //

    MAX_INPUT_ID
};

enum EN_OUTPUT_ID  {
    //기존
    y0000 =  0 , yETC_StartLp           =  0 , //y0000 =  0 , yETC_StartLp           =  0 , //
    y0001 =  1 , yETC_StopLp            =  1 , //y0001 =  1 , yETC_StopLp            =  1 , //
    y0002 =  2 , yETC_DoorFw            =  2 , //y0002 =  2 , yETC_DoorFw            =  2 , //
    y0003 =  3 , yETC_DoorBw            =  3 , //y0003 =  3 , yETC_DoorBw            =  3 , //
    y0004 =  4 , yETC_IONAir            =  4 , //y0004 =  4 , yETC_IONAir            =  4 , //
    y0005 =  5 , yETC_BNRAir            =  5 , //y0005 =  5 , yETC_BNRAir            =  5 , //
    y0006 =  6 , yETC_PMPAir1           =  6 , //y0006 =  6 , yETC_PMPAir            =  6 , //
    y0007 =  7 , yETC_BTMValve          =  7 , //y0007 =  7 , yETC_PMPValve          =  7 , //
    y0008 =  8 , yETC_BNRValve          =  8 , //y0008 =  8 , yETC_BNRValve          =  8 , //
    y0009 =  9 , yETC_DrainValve        =  9 , //y0009 =  9 , yETC_DrainValve        =  9 , //
    y000A = 10 , yETC_TwRedLp           = 10 , //y000A = 10 , yETC_TwRedLp           = 10 , //
    y000B = 11 , yETC_TwYelLp           = 11 , //y000B = 11 , yETC_TwYelLp           = 11 , //
    y000C = 12 , yETC_TwGrnLp           = 12 , //y000C = 12 , yETC_TwGrnLp           = 12 , //
    y000D = 13 , yETC_TwBzz             = 13 , //y000D = 13 , yETC_TwBzz             = 13 , //
    y000E = 14 , yETC_PMPAir2           = 14 , //y000E = 14 , y014                   = 14 , //
    y000F = 15 , yETC_MskCycleFwBw      = 15 , //y000F = 15 , y015                   = 15 , //

    MAX_OUTPUT_ID
};
#endif
