#ifndef UTILDEFINEH
#define UTILDEFINEH

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum EN_ARAY_ID {
    riLDR       =0 , //로더
    riPRB          , //프리버퍼존 1 로더에서 투입 완료 된곳.
    riWRK          , //작업중.
    riPSB          , //워크존에서 빠지자 마자~ 트림 완료까지.
    riTmp          , //메뉴얼 리핏을 위해서 더미 어레이 생성.
    riSPC          ,
    riSTL          ,
    riREJ          ,
    riULD          ,

    //riPR2          , //프리버퍼존 1에서 스텝 타고 드레그 타고 풀,백,ID 체크 마친곳.
    //riWRP          , //프리버퍼에서 건네 받자 마자
    //riPS2          , //언로더 진입 대기.
    //riPSH          ,

    MAX_ARAY
};

enum EN_MOTR_ID {
    miPRB_XCmp  =0 ,    //
    miWRK_XCmp     ,    //
    miPSB_XCmp     ,    //
    miWRK_ZTop     ,    //
    miWRK_ZBtm     ,    //
    miWRK_YBtm     ,    // 미래선도 과제에 추가된 모터.. 변경예정.
    miSTL_YTop     ,    //
    miREJ_XBtm     ,    //
    miSTL_YBtm     ,    //
    miLDR_YCmp     ,    //
    miLDR_ZCmp     ,    //
    miULD_YCmp     ,    //
    miULD_ZCmp     ,    //

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiPRB_Cmp   = 0,     //
    aiPRB_IdChk    ,     //
    aiWRK_Cmp      ,     //
    aiPSB_Cmp      ,     //
    aiLDR_Cmp      ,     //
    aiLDR_Psh      ,     //
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
    eiLDR_CycleTO          ,   //012 eiLDR_CycleTO             
    eiSTL_CycleTO          ,   //013 eiSTL_CycleTO             
    eiREJ_CycleTO          ,   //014 eiREJ_CycleTO             
    eiULD_CycleTO          ,   //015 eiULD_CycleTO             
    eiPRB_HomeTo           ,   //016 eiPRB_HomeTo              
    eiPSB_HomeTo           ,   //017 eiPSB_HomeTo              
    eiWRK_HomeTo           ,   //018 eiWRK_HomeTo              
    eiLDR_HomeTO           ,   //019 eiLDR_HomeTO              
    eiSTL_HomeTo           ,   //020 eiSTL_HomeTo              
    eiREJ_HomeTo           ,   //021 eiREJ_HomeTo              
    eiULD_HomeTO           ,   //022 eiULD_HomeTO              
    eiPRB_ToStartTO        ,   //023 eiPRB_ToStartTO           
    eiPSB_ToStartTO        ,   //024 eiPSB_ToStartTO           
    eiWRK_ToStartTO        ,   //025 eiWRK_ToStartTO           
    eiLDR_ToStartTO        ,   //026 eiLDR_ToStartTO           
    eiSTL_ToStartTO        ,   //027 eiSTL_ToStartTO           
    eiREJ_ToStartTO        ,   //028 eiREJ_ToStartTO           
    eiULD_ToStartTO        ,   //029 eiULD_ToStartTO           
    eiPRB_ToStopTO         ,   //030 eiPRB_ToStopTO            
    eiPSB_ToStopTO         ,   //031 eiPSB_ToStopTO            
    eiWRK_ToStopTO         ,   //032 eiWRK_ToStopTO            
    eiLDR_ToStopTO         ,   //033 eiLDR_ToStopTO            
    eiSTL_ToStopTO         ,   //034 eiSTL_ToStopTO            
    eiREJ_ToStopTO         ,   //035 eiREJ_ToStopTO            
    eiULD_ToStopTO         ,   //036 eiULD_ToStopTO            
    eiLDR_ZCmpHomeEnd      ,   //037 eiLDR_ZCmpHomeEnd         
    eiLDR_ZCmpNegLim       ,   //038 eiLDR_ZCmpNegLim          
    eiLDR_ZCmpPosLim       ,   //039 eiLDR_ZCmpPosLim          
    eiLDR_ZCmpAlarm        ,   //040 eiLDR_ZCmpAlarm           
    eiLDR_YCmpHomeEnd      ,   //041 eiLDR_YCmpHomeEnd         
    eiLDR_YCmpNegLim       ,   //042 eiLDR_YCmpNegLim          
    eiLDR_YCmpPosLim       ,   //043 eiLDR_YCmpPosLim          
    eiLDR_YCmpAlarm        ,   //044 eiLDR_YCmpAlarm           
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
    eiLDR_CmpTO            ,   //085 eiLDR_CmpTO               
    eiLDR_PushTO           ,   //086 eiLDR_PushTO              
    eiSTL_CmpTO            ,   //087 eiSTL_CmpTO               
    eiULD_CmpTO            ,   //088 eiULD_CmpTO               
    eiULD_PushTO           ,   //089 eiULD_PushTO              
    eiLDR_MgzDispr         ,   //090 eiLDR_ClampDispr          
    eiLDR_MgzUnknown       ,   //091 eiLDR_ClampUnknown        
    eiLDR_MgzChange        ,   //092 eiLDR_MgzChange           
    eiPRB_Dispr            ,   //093 eiPRB_Dispr               
    eiPRB_Unknown          ,   //094 eiPRB_Unknown             
    eiWRK_Dispr            ,   //095 eiWRK_Dispr               
    eiWRK_Unknown          ,   //096 eiWRK_Unknown             
    eiPSB_Dispr            ,   //097 eiPSB_Dispr               
    eiPSB_Unknown          ,   //098 eiPSB_Unknown             
    eiWRK_FailCntOver      ,   //099 eiWRK_FailCntOver         
    eiWRK_FailProbeComm1   ,   //100 eiWRK_FailProbeComm1      
    eiWRK_FailProbeComm2   ,   //101 eiWRK_FailProbeComm2      
    eiLDR_MgzFull          ,   //102 eiLDR_MgzFull             
    eiLDR_PusherOverload   ,   //103 eiLDR_PusherOverload      
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
    eiLDR_NeedMgz          ,   //137 eiLDR_NeedMgz             
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
    eiPRB_TempZ1Wire5      ,   //166 eiPRB_TempZ1Alrm1         
    eiWRK_YWrkHomeEnd      ,   //167                           
    eiWRK_YWrkNegLim       ,   //168                           
    eiWRK_YWrkPosLim       ,   //169                           
    eiWRK_YWrkAlarm        ,   //170                           
    eiWRK_VisnNG           ,   //171
    eiWRK_PosErr           ,   //172 자재 포지션 잃어버림.
    eiLDR_PickMiss         ,   //173 매거진 픽 동작중 못가지고 옴.
    eiULD_PickMiss         ,   //174 매거진 픽 동작중 못가지고 옴.
    eiETC_TempAlrm         ,
    eiETC_LotErr           ,   
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
    x0015 = 21 , xPRB_IdCheck            = 21 , //
    x0016 = 22 , xPSB_IdCheckBw          = 22 , //
    x0017 = 23 , xPSB_IdCheckFw          = 23 , //
    x0018 = 24 , xPRB_Overload           = 24 , //
    x0019 = 25 , xWRK_Overload           = 25 , //
    x001A = 26 , xPSB_Overload           = 26 , //
    x001B = 27 , xPRB_ClampFw            = 27 , //
    x001C = 28 , xPRB_ClampBw            = 28 , //
    x001D = 29 , xWRK_ClampFw            = 29 , //
    x001E = 30 , xWRK_ClampBw            = 30 , //
    x001F = 31 , xPRB_StripOver          = 31 , //

    x0020 = 32 , xPSB_ClampFw            = 32 , //
    x0021 = 33 , xPSB_ClampBw            = 33 , //
    x0022 = 34 , xWRK_1Pkg               = 34 , //
    x0023 = 35 , xWRK_2Pkg               = 35 , //
    x0024 = 36 , x036                    = 36 , //
    x0025 = 37 , xPRB_1Pkg               = 37 , //
    x0026 = 38 , xPRB_2Pkg               = 38 , //
    x0027 = 39 , xPRB_3Pkg               = 39 , //
    x0028 = 40 , xPSB_1Pkg               = 40 , //
    x0029 = 41 , xPSB_2Pkg               = 41 , //
    x002A = 42 , xPSB_3Pkg               = 42 , //
    x002B = 43 , xPSB_4Pkg               = 43 , //
    x002C = 44 , xPSB_PkgOut1            = 44 , //
    x002D = 45 , xPSB_PkgOut2            = 45 , //
    x002E = 46 , xPSB_PusherFw           = 46 , //
    x002F = 47 , xPSB_PusherBw           = 47 , //

    x0030 = 48 , xSTL_UnitUp             = 48 , //
    x0031 = 49 , xSTL_UnitDn             = 49 , //
    x0032 = 50 , xSTL_Vacuum             = 50 , //
    x0033 = 51 , xSTL_DevDetect          = 51 , //
    x0034 = 52 , xREJ_FullCheck          = 52 , //
    x0035 = 53 , xREJ_Box1               = 53 , //
    x0036 = 54 , xREJ_Box2               = 54 , //
    x0037 = 55 , xREJ_Box3               = 55 , //
    x0038 = 56 , xREJ_Box4               = 56 , //
    x0039 = 57 , xREJ_Box5               = 57 , //
    x003A = 58 , xREJ_Box6               = 58 , //
    x003B = 59 , xREJ_Box7               = 59 , //
    x003C = 60 , xREJ_Box8               = 60 , //
    x003D = 61 , xREJ_Box9               = 61 , //
    x003E = 62 , xREJ_Box10              = 62 , //
    x003F = 63 , x063                    = 63 , //

    x0040 = 64 , xSTL_PunchingUp         = 64 , //
    x0041 = 65 , xSTL_PunchingDn         = 65 , //
    x0042 = 66 , x066                    = 66 , //
    x0043 = 67 , x067                    = 67 , //
    x0044 = 68 , xPRB_TempZ1Alrm1        = 68 , //
    x0045 = 69 , xPRB_TempZ1Wire1        = 69 , //
    x0046 = 70 , xPRB_TempZ1Alrm2        = 70 , //
    x0047 = 71 , xPRB_TempZ1Wire2        = 71 , //
    x0048 = 72 , xPRB_TempZ1Alrm3        = 72 , //
    x0049 = 73 , xPRB_TempZ1Wire3        = 73 , //
    x004A = 74 , xPRB_TempZ1Alrm4        = 74 , //
    x004B = 75 , xPRB_TempZ1Wire4        = 75 , //
    x004C = 76 , xPRB_TempZ1Alrm5        = 76 , //
    x004D = 77 , xPRB_TempZ1Wire5        = 77 , //
    x004E = 78 , x078                    = 78 , //
    x004F = 79 , x079                    = 79 , //

    x0050 = 80 , xPSB_TempZ2Alrm1        = 80 , //
    x0051 = 81 , xPSB_TempZ2Wire1        = 81 , //
    x0052 = 82 , xPSB_TempZ2Alrm2        = 82 , //
    x0053 = 83 , xPSB_TempZ2Wire2        = 83 , //
    x0054 = 84 , xPSB_TempZ2Alrm3        = 84 , //
    x0055 = 85 , xPSB_TempZ2Wire3        = 85 , //
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
    y0011 = 17 , yPRB_ClampFw            = 17 , //
    y0012 = 18 , yPRB_ClampBw            = 18 , //
    y0013 = 19 , yWRK_ClampFw            = 19 , //
    y0014 = 20 , yWRK_ClampBw            = 20 , //
    y0015 = 21 , yPSB_ClampFw            = 21 , //
    y0016 = 22 , yPSB_ClampBw            = 22 , //
    y0017 = 23 , yPRB_ReverseFw          = 23 , //
    y0018 = 24 , yWRK_Probe1             = 24 , //
    y0019 = 25 , yWRK_Probe2             = 25 , //
    y001A = 26 , yPSB_PusherFw           = 26 , //
    y001B = 27 , yETC_MainSol2           = 27 , //Eject용 Air Blower라고 함.JS
    y001C = 28 , yPSB_FeedingAC          = 28 , //
    y001D = 29 , y029                    = 29 , //
    y001E = 30 , yETC_BlowerAC           = 30 , //
    y001F = 31 , y031                    = 31 , //
    y0020 = 32 , ySTL_YTopFw             = 32 , //
    y0021 = 33 , ySTL_Vaccum             = 33 , //
    y0022 = 34 , ySTL_VaccumEje          = 34 , //
    y0023 = 35 , ySTL_YBottomFw          = 35 , //
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
    y0030 = 48 , y048                    = 48 , //yPRB_AirBlower
    y0031 = 49 , y049                    = 49 , //yPRB_ClampFw
    y0032 = 50 , y050                    = 50 , //yPRB_ClampBw
    y0033 = 51 , y051                    = 51 , //yWRK_ClampFw
    y0034 = 52 , y052                    = 52 , //yWRK_ClampBw
    y0035 = 53 , y053                    = 53 , //yPSB_ClampFw
    y0036 = 54 , y054                    = 54 , //yPSB_ClampBw
    y0037 = 55 , y055                    = 55 , //yPRB_ReverseFw
    y0038 = 56 , y056                    = 56 , //yWRK_Probe1
    y0039 = 57 , y057                    = 57 , //yWRK_Probe2
    y003A = 58 , y058                    = 58 , //yPSB_PusherFw
    y003B = 59 , y059                    = 59 , //yETC_MainSol2
    y003C = 60 , y060                    = 60 , //yPSB_FeedingAC
    y003D = 61 , y061                    = 61 , //y061
    y003E = 62 , y062                    = 62 , //yETC_BlowerAC
    y003F = 63 , y063                    = 63 , //y063
    y0040 = 64 , yLDR_MgzInAC            = 64 , //ySTL_YTopFw
    y0041 = 65 , yLDR_MgzOutAC           = 65 , //ySTL_Vaccum
    y0042 = 66 , yLDR_ClampFw            = 66 , //ySTL_VaccumEje
    y0043 = 67 , yLDR_ClampBw            = 67 , //ySTL_YBottomFw
    y0044 = 68 , yLDR_PusherFw           = 68 , //y068
    y0045 = 69 , yLDR_ZBreak             = 69 , //y069
    y0046 = 70 , y070                    = 70 , //y070
    y0047 = 71 , y071                    = 71 , //y071
    y0048 = 72 , y072                    = 72 , //yETC_Heater1_2
    y0049 = 73 , y073                    = 73 , //yETC_Heater1_3
    y004A = 74 , y074                    = 74 , //yETC_Heater2_2
    y004B = 75 , y075                    = 75 , //yETC_Heater2_3
    y004C = 76 , y076                    = 76 , //y076
    y004D = 77 , y077                    = 77 , //y077
    y004E = 78 , y078                    = 78 , //y078
    y004F = 79 , y079                    = 79 , //y079
    y0050 = 80 , yULD_MgzOutAC           = 80 , //ySTL_YTopFw
    y0051 = 81 , yULD_MgzInAC            = 81 , //ySTL_Vaccum            
    y0052 = 82 , yULD_ClampFw            = 82 , //ySTL_VaccumEje         
    y0053 = 83 , yULD_ClampBw            = 83 , //ySTL_YBottomFw         
    y0054 = 84 , yULD_ZBreak             = 84 , //y068                   
    y0055 = 85 , y085                    = 85 , //y069                   
    y0056 = 86 , y086                    = 86 , //y070                   
    y0057 = 87 , y087                    = 87 , //y071                   
    y0058 = 88 , y088                    = 88 , //yETC_Heater1_2         
    y0059 = 89 , y089                    = 89 , //yETC_Heater1_3         
    y005A = 90 , y090                    = 90 , //yETC_Heater2_2         
    y005B = 91 , y091                    = 91 , //yETC_Heater2_3         
    y005C = 92 , y092                    = 92 , //y076                   
    y005D = 93 , y093                    = 93 , //y077
    y005E = 94 , y094                    = 94 , //y078
    y005F = 95 , y095                    = 95 , //y079

    MAX_OUTPUT
};

enum EN_PSTN_ID {
    piPRB_XWait       = 0  ,  //
    piPRB_XGrip            ,  //
    piPRB_XReGrip          ,  //
    piPRB_XPull            ,  //
    piPRB_XBack            ,  //
    piPRB_XIdCheck         ,
    piPRB_XOut1            ,  //
    piPRB_XOut2            ,  //
    piPRB_XStep1           ,  // PR1에서 Pull 동작 하기 전에 스텝 타는 포지션
    p1PRB_XStep2           ,  // PR2에서 Out  동작 하기 전에 스텝 타는 포지션.
    MAX_PSTN_ID_MOTR0      ,  //

    piWRK_XWait       =  0 ,  //
    piWRK_XGrip            ,  //
    piWRK_XWorkStart       ,  //
    piWRK_XOut             ,  //
    piWRK_XReGrip          ,  //
    piWRK_XWork            ,  //
//    piWRK_XReWork          ,  //
    MAX_PSTN_ID_MOTR1      ,  //

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
    MAX_PSTN_ID_MOTR2      ,  //
                              //
    piWRK_ZTWait      =  0 ,  //
    piWRK_ZTWork           ,  //
    piWRK_ZTChange         ,  //
    MAX_PSTN_ID_MOTR3      ,  //

    piWRK_ZBWait      =  0 ,  //
    piWRK_ZBWork           ,  //
    piWRK_ZBChange         ,  //
    MAX_PSTN_ID_MOTR4      ,  //

    piWRK_YBWait      =  0 ,  //
    piWRK_YBWork           ,  //
    piWRK_YBContect        ,  //  사이드뷰를 위해 추가 JH
    piWRK_YBChange         ,  //
    MAX_PSTN_ID_MOTR5      ,  //

    piSTL_YTWait      =  0 ,  //
    piSTL_YTWorkStart      ,  //
    piSTL_YTPlace          ,  //
    piSTL_YTDevCheck       ,
    piSTL_YTChange         ,  //
    piSTL_YTWork           ,  //
    MAX_PSTN_ID_MOTR6      ,  //

    piREJ_XBWait      =  0 ,  //
    piREJ_XBWorkStart      ,  //
    piREJ_XBWork           ,
    MAX_PSTN_ID_MOTR7      ,  //

    piSTL_YBWait      =  0 ,  //
    piSTL_YBWorkStart      ,  //
    piSTL_YBChange         ,  //
    piSTL_YBWork           ,  //
    piSTL_YBStep           ,  //
    MAX_PSTN_ID_MOTR8      ,  //

    piLDR_YWait       =  0 ,  //
    piLDR_YWork            ,  //
    piLDR_YPick            ,  //
    piLDR_YPlace           ,  //
    MAX_PSTN_ID_MOTR9      ,  //

    piLDR_ZWait       =  0 ,  //
    piLDR_ZPickFwd         ,  //
    piLDR_ZClampOn         ,  //
    piLDR_ZPickBwd         ,  //
    piLDR_ZWorkStart       ,  //맨 아래 슬롯 기준.
    piLDR_ZPlaceFwd        ,  //
    piLDR_ZClampOff        ,  //
    piLDR_ZPlaceBwd        ,  //
    piLDR_ZWork            ,  //
    MAX_PSTN_ID_MOTR10      ,  //

    piULD_YWait       =  0 ,  //
    piULD_YWork            ,  //
    piULD_YPick            ,  //
    piULD_YPlace           ,  //
    MAX_PSTN_ID_MOTR11     ,  //

    piULD_ZWait       =  0 ,  //
    piULD_ZPickFwd         ,  //
    piULD_ZClampOn         ,  //
    piULD_ZPickBwd         ,  //
    piULD_ZWorkStart       ,  //
    piULD_ZPlaceFwd        ,  //
    piULD_ZClampOff        ,  //
    piULD_ZPlaceBwd        ,  //
    piULD_ZWork            ,  //
    MAX_PSTN_ID_MOTR12     ,  //
};

enum EN_PSTN_VALUE {
    pvPRB_XWaitPs       = 0  , //
    pvPRB_XGripPs            , //
    pvPRB_XReGripPs           , //
    pvPRB_XPullPs            , //
    pvPRB_XBackPs            , //
    pvPRB_XIdCheck           , //
    pvPRB_XOut1Ps            , //
    pvPRB_XOut2Ps            , //
    MAX_PSTN_MOTR0           , //

    pvWRK_XWaitPs       =  0 , //
    pvWRK_XGripPs            , //
    pvWRK_XWorkStartPs       , //
    pvWRK_XOutPs             , //
    MAX_PSTN_MOTR1           , //

    pvPSB_XWaitPs       = 0  , //
    pvPSB_XGripPs            , //
    pvPSB_XPullPs            , //
    pvPSB_XBackPs            , //
    pvPSB_XWorkStartPs       , //
    pvPSB_XOutPs             , //
//    pvPSB_XReGripPs          , //
    MAX_PSTN_MOTR2           , //

    pvWRK_ZTWaitPs      =  0 , //
    pvWRK_ZTWorkPs           , //
    pvWRK_ZTChangePs         , //
    MAX_PSTN_MOTR3           , //

    pvWRK_ZBWaitPs      =  0 , //
    pvWRK_ZBWorkPs           , //
    pvWRK_ZBChangePs         , //
    MAX_PSTN_MOTR4           , //

    pvWRK_YBWaitPs      =  0 , //
    pvWRK_YBWorkPs           , //
    pvWRK_YBContectPs        , //   사이드뷰 위해서 추가 JH
    pvWRK_YBChangePs         , //
    MAX_PSTN_MOTR5           , //

    pvSTL_YTWaitPs      =  0 , //
    pvSTL_YTWorkStartPs      , //
    pvSTL_YTPlacePs          ,
    pvSTL_YTDevCheckPs       ,
    pvSTL_YTChangePs         ,
    MAX_PSTN_MOTR6           ,

    pvREJ_XBWaitPs      =  0 ,
    pvREJ_XBWorkStartPs      ,
    MAX_PSTN_MOTR7           ,

    pvSTL_YBWaitPs      =  0 ,
    pvSTL_YBWorkStartPs      ,
    pvSTL_YBChangePs         ,
    MAX_PSTN_MOTR8           ,

    pvLDR_YWaitPs       =  0 , //
    pvLDR_YWorkPs            , //
    pvLDR_YPickPs            , //
    pvLDR_YPlacePs           , //
    MAX_PSTN_MOTR9           , //

    pvLDR_ZWaitPs       =  0 , //
    pvLDR_ZPickFwdPs         , //
    pvLDR_ZClampOnPs         , //
    pvLDR_ZPickBwdPs         , //
    pvLDR_ZWorkStartPs       , //
    pvLDR_ZPlaceFwdPs        , //
    pvLDR_ZClampOffPs        , //
    pvLDR_ZPlaceBwdPs        , //
    MAX_PSTN_MOTR10           , //

    pvULD_YWaitPs       =  0 ,
    pvULD_YWorkPs            ,
    pvULD_YPickPs            ,
    pvULD_YPlacePs           ,
    MAX_PSTN_MOTR11          ,

    pvULD_ZWaitPs       =  0 ,
    pvULD_ZPickFwdPs         ,
    pvULD_ZClampOnPs         ,
    pvULD_ZPickBwdPs         ,
    pvULD_ZWorkStartPs       ,
    pvULD_ZPlaceFwdPs        ,
    pvULD_ZClampOffPs        ,
    pvULD_ZPlaceBwdPs        ,
    MAX_PSTN_MOTR12          ,


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
    mcSTL_YTopUpPnch1Pitch      ,
    mcSTL_YTopDnPnch1Pitch      ,
    mcSTL_YBtmUp1pitch          ,
    mcSTL_YBtmDn1pitch          ,
    mcSTL_YTopUpSnsr1Pitch      ,
    mcSTL_YTopDnSnsr1Pitch      ,

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

    mcETC_PrbStt           = 100,
    mcETC_TPrbUp                ,
    mcETC_TPrbDn                ,
    mcETC_BPrbUp                ,
    mcETC_BPrbDn                ,
    mcETC_LDRULDPlace           ,


    MAX_MANUAL_CYCLE
};

#endif



