#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_VISN 4   

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum EN_ARAY_ID {
    riLDR       =0 ,
    riPRB          ,

    riWR1          ,
    riVS1          ,

    riWR2          ,
    riVS2          ,

    riWR3          ,
    riVS3          ,

    riPSB          ,
    riLOT          ,  //Lot End 용 버퍼.
    riVS4          ,

    riULD          ,

    riWB1          , //메뉴얼 동작 때문에 필요.
    riWB2          ,
    riWB3          ,

    riVB4          ,

    riSPC          ,  //SPC 용 버퍼.
    MAX_ARAY
};

enum EN_MOTR_ID {
    miLDR_YCmp  =0 ,    //
    miLDR_ZCmp     ,    //
    miWRK_XVsn     ,    //
    miWRK_YVsn     ,    //
    miPSB_XMrk     ,    //
    miPSB_YMrk     ,    //
    miWK1_YFlp     ,    //
    miWK1_XIns     ,    //
    miULD_YCmp     ,    //
    miULD_ZCmp     ,    //

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_Cmp        =0  ,     //
    aiLDR_Pusher         ,     //
    aiPRB_Stopper        ,     //
    aiWK1_Stopper        ,     //
    aiWK1_Align          ,     //
    aiWK1_FlpCmp         ,     //
    aiWK1_Ins            ,     //    
    aiWK2_Stopper        ,     //
    aiWK2_Align          ,     //
    aiWK2_Lift           ,     //
    aiWK3_Stopper        ,     //
    aiWK3_Align          ,     //
    aiWK3_Lift           ,     //
    aiPSB_Stopper        ,     //
    aiPSB_Align          ,     //
    aiPSB_Lift           ,     //
    aiPSB_Marking        ,     //
    aiPSB_Pusher         ,     //
    aiULD_Cmp            ,     //

    MAX_ACTR
};

enum EN_ERR_ID {
    eiETC_MainAir          =  0 ,
    eiETC_ToStartTO             ,
    eiETC_ToStopTO              ,
    eiETC_Emergency             ,
    eiETC_FDoor                 ,
    eiETC_BDoor                 ,
    eiETC_LotEnd                ,
    eiETC_AllHomeTO             ,
    eiETC_ManCycleTO            ,

    eiLDR_CycleTO               ,
    eiRAL_CycleTO               ,
    eiHED_CycleTO               ,
    eiPSB_CycleTO               ,
    eiULD_CycleTO               ,

    eiLDR_HomeTO                ,
    eiRAL_HomeTO                ,
    eiHED_HomeTO                ,
    eiPSB_HomeTO                ,
    eiULD_HomeTO                ,

    eiLDR_ToStartTO             ,
    eiRAL_ToStartTO             ,
    eiHED_ToStartTO             ,
    eiPSB_ToStartTO             ,
    eiULD_ToStartTO             ,

    eiLDR_ToStopTO              ,
    eiRAL_ToStopTO              ,
    eiHED_ToStopTO              ,
    eiPSB_ToStopTO              ,
    eiULD_ToStopTO              ,

    eiLDR_YCmpHomeEnd           ,
    eiLDR_YCmpNegLim            ,
    eiLDR_YCmpPosLim            ,
    eiLDR_YCmpAlarm             ,

    eiLDR_ZCmpHomeEnd           ,
    eiLDR_ZCmpNegLim            ,      
    eiLDR_ZCmpPosLim            ,      
    eiLDR_ZCmpAlarm             ,      
                                       
    eiWK1_XInsHomeEnd           ,
    eiWK1_XInsNegLim            ,
    eiWK1_XInsPosLim            ,
    eiWK1_XInsAlarm             ,

    eiWK1_YFlpHomeEnd           ,
    eiWK1_YFlpNegLim            ,
    eiWK1_YFlpPosLim            ,
    eiWK1_YFlpAlarm             ,

    eiWRK_XVsnHomeEnd           ,
    eiWRK_XVsnNegLim            ,
    eiWRK_XVsnPosLim            ,
    eiWRK_XVsnAlarm             ,

    eiWRK_YVsnHomeEnd           ,
    eiWRK_YVsnNegLim            ,
    eiWRK_YVsnPosLim            ,
    eiWRK_YVsnAlarm             ,

    eiPSB_XMrkHomeEnd           ,
    eiPSB_XMrkNegLim            ,
    eiPSB_XMrkPosLim            ,
    eiPSB_XMrkAlarm             ,

    eiPSB_YMrkHomeEnd           ,
    eiPSB_YMrkNegLim            ,
    eiPSB_YMrkPosLim            ,
    eiPSB_YMrkAlarm             ,

    eiULD_YCmpHomeEnd           ,
    eiULD_YCmpNegLim            ,
    eiULD_YCmpPosLim            ,
    eiULD_YCmpAlarm             ,

    eiULD_ZCmpHomeEnd           ,
    eiULD_ZCmpNegLim            ,
    eiULD_ZCmpPosLim            ,
    eiULD_ZCmpAlarm             ,

    eiLDR_CmpTo                 ,
    eiLDR_PusherTo              ,
    eiPRB_StopperTo             ,
    eiWK1_StopperTo             ,
    eiWK1_AlignTo               ,
    eiWK1_FlpCmpTo              ,
    eiWK1_InsTo                 ,
    eiWK2_StopperTo             ,
    eiWK2_AlignTo               ,
    eiWK2_LiftTo                ,
    eiWK3_StopperTo             ,
    eiWK3_AlignTo               ,
    eiWK3_LiftTo                ,
    eiPSB_StopperTo             ,
    eiPSB_AlignTo               ,
    eiPSB_LiftTo                ,
    eiPSB_MarkingTo             ,
    eiPSB_PusherTo              ,
    eiULD_CmpTo                 ,

    eiLDR_MgzDispr              ,
    eiLDR_MgzUnknown            ,
    eiPRB_PkgDispr              ,
    eiPRB_PkgUnknown            ,
    eiWK1_PkgDispr              ,
    eiWK1_PkgUnknown            ,
    eiWK2_PkgDispr              ,
    eiWK2_PkgUnknown            ,
    eiWK3_PkgDispr              ,
    eiWK3_PkgUnknown            ,
    eiPSB_PkgDispr              ,
    eiPSB_PkgUnknown            ,
    eiULD_MgzUnknown            ,
    eiULD_MgzDispr              ,


    eiWK1_VsnFailCnt            ,
    eiWK2_VsnFailCnt            ,
    eiWK3_VsnFailCnt            ,
    eiPSB_MarkingFail           ,
    eiHED_Visn1NotReady         ,
    eiHED_Visn2NotReady         ,
    eiHED_Visn3NotReady         ,
    eiHED_Visn4NotReady         ,

    eiLDR_PshrOverload          ,
    eiLDR_MgzFull               ,
    eiLDR_NeedMgz               ,
    eiULD_MgzFull               ,
    eiULD_NeedMgz               ,

    eiHED_Visn1Read             ,
    eiHED_Visn2Read             ,
    eiHED_Visn3Read             ,
    eiHED_Visn4Read             ,
    eiHED_VisnResetCnt          ,
    eiHED_VisnCommTO            ,

    eiWK1_VsnSFailCnt           ,
    eiWK2_VsnSFailCnt           ,
    eiWK3_VsnSFailCnt           ,

    eiPSB_PusherMiss            ,

    eiPRB_OutStripExist         ,
    eiPSB_OutStripExist         ,

    eiWK1_IndexOverload         ,
    eiPRB_StripFeedingFail      ,

    eiWK1_FlipperMoveFail       ,
    eiPSB_VsnFailCnt            ,

    eiPSB_TotalFailCnt          ,
    eiPSB_SameFailCnt           ,

    eiPSB_SensorErr             ,


    MAX_ERR
};

enum EN_INPUT_ID {
    x0000 =  0 ,  xLDR_PusherOverLoad      =  0 , //
    x0001 =  1 ,  xLDR_PusherFw            =  1 , //
    x0002 =  2 ,  xLDR_PusherBw            =  2 , //
    x0003 =  3 ,  xLDR_MgzDetect1          =  3 , //
    x0004 =  4 ,  xLDR_MgzDetect2          =  4 , //
    x0005 =  5 ,  xLDR_ClampBw             =  5 , //
    x0006 =  6 ,  xLDR_ClampFw1            =  6 , //
    x0007 =  7 ,  xLDR_ClampFw2            =  7 , //
    x0008 =  8 ,  xLDR_MgzIn               =  8 , //
    x0009 =  9 ,  xLDR_MgzOutFull          =  9 , //
    x000A = 10 ,  x010                     = 10 , //
    x000B = 11 ,  x011                     = 11 , //
    x000C = 12 ,  xLDR_PowerOnAC           = 12 , //
    x000D = 13 ,  xLDR_PowerOnDC           = 13 , //
    x000E = 14 ,  xLDR_Emg                 = 14 , //
    x000F = 15 ,  x015                     = 15 , //

    x0010 = 16 ,  xULD_MgzDetect1          = 16 , //
    x0011 = 17 ,  xULD_MgzDetect2          = 17 , //
    x0012 = 18 ,  xULD_ClampBw             = 18 , //
    x0013 = 19 ,  xULD_ClampFw1            = 19 , //
    x0014 = 20 ,  xULD_ClampFw2            = 20 , //
    x0015 = 21 ,  xULD_MgzOutFull          = 21 , //
    x0016 = 22 ,  xULD_MgzIn               = 22 , //
    x0017 = 23 ,  x023                     = 23 , //
    x0018 = 24 ,  xULD_PowerOnAC           = 24 , //
    x0019 = 25 ,  xULD_PowerOnDC           = 25 , //
    x001A = 26 ,  xULD_Emg                 = 26 , //
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
    x0025 = 37 ,  xETC_LVisnSw             = 37 , //
    x0026 = 38 ,  xETC_RStartSw            = 38 , //
    x0027 = 39 ,  xETC_RStopSw             = 39 , //
    x0028 = 40 ,  xETC_RResetSw            = 40 , //
    x0029 = 41 ,  xETC_RMainAir            = 41 , //
    x002A = 42 ,  xETC_RInitSw             = 42 , //
    x002B = 43 ,  xETC_RVisnSw             = 43 , //
    x002C = 44 ,  x044                     = 44 , //
    x002D = 45 ,  x045                     = 45 , //
    x002E = 46 ,  xETC_EmgSw1              = 46 , //
    x002F = 47 ,  xETC_EmgSw2              = 47 , //

    x0030 = 48 ,  x048                     = 48 , //
    x0031 = 49 ,  xETC_PowerOn             = 49 , //
    x0032 = 50 ,  xETC_MainSol             = 50 , //
    x0033 = 51 ,  xETC_FDoor               = 51 , //
    x0034 = 52 ,  xETC_BDoor               = 52 , //
    x0035 = 53 ,  x053                     = 53 , //
    x0036 = 54 ,  x054                     = 54 , //
    x0037 = 55 ,  x055                     = 55 , //
    x0038 = 56 ,  xPRB_PkgIn1              = 56 , //
    x0039 = 57 ,  x057                     = 57 , // IO 어디에 쳐 밖혀 잇는지 알수가 없어서 일단 여기다 쳐넣음.
    x003A = 58 ,  xWR2_Align               = 58 , //
    x003B = 59 ,  xWR3_Align               = 59 , //
    x003C = 60 ,  xPRB_PkgOut2             = 60 , ////xWR1_1Pkg                = 60 , //
    x003D = 61 ,  x061                     = 61 , //
    x003E = 62 ,  xPSB_OffPshrFw           = 62 , //
    x003F = 63 ,  xPSB_OffPshrBw           = 63 , //

    x0040 = 64 ,  x064                     = 64 , //
    x0041 = 65 ,  x065                     = 65 , //
    x0042 = 66 ,  xPSB_Align               = 66 , //
    x0043 = 67 ,  xPSB_PkgOut4             = 67 , //
    x0044 = 68 ,  xPSB_PkgOut3             = 68 , //
    x0045 = 69 ,  x069                     = 69 , //
    x0046 = 70 ,  xPSB_MarkingBw           = 70 , // ???????????
    x0047 = 71 ,  xPSB_MarkingFw           = 71 , // ???????????
    x0048 = 72 ,  xPRB_StprFw              = 72 , //
    x0049 = 73 ,  xPRB_StprBw              = 73 , //
    x004A = 74 ,  xPRB_Pkg                 = 74 , //
    x004B = 75 ,  xWR1_StprFw              = 75 , //
    x004C = 76 ,  xWR1_StprBw              = 76 , //
    x004D = 77 ,  xWR1_Pkg                 = 77 , //
    x004E = 78 ,  xWR2_StprFw              = 78 , //
    x004F = 79 ,  xWR2_StprBw              = 79 , //

    x0050 = 80 ,  xWR2_Pkg                 = 80 , //
    x0051 = 81 ,  xWR3_StprFw              = 81 , //
    x0052 = 82 ,  xWR3_StprBw              = 82 , //
    x0053 = 83 ,  xWR3_Pkg                 = 83 , //
    x0054 = 84 ,  x084                     = 84 , //
    x0055 = 85 ,  x085                     = 85 , //
    x0056 = 86 ,  xWR2_LiftFw              = 86 , //
    x0057 = 87 ,  xWR2_LiftBw              = 87 , //
    x0058 = 88 ,  xWR3_LiftFw              = 88 , //
    x0059 = 89 ,  xWR3_LiftBw              = 89 , //
    x005A = 90 ,  xPSB_LiftFw              = 90 , //
    x005B = 91 ,  xPSB_LiftBw              = 91 , //
    x005C = 92 ,  xPSB_StprFw              = 92 , // ???????
    x005D = 93 ,  xPSB_StprBw              = 93 , // ???????
    x005E = 94 ,  xPSB_Pkg                 = 94 , //
    x005F = 95 ,  x095                     = 95 , //

    x0060 = 96 ,  xWR1_AlignFw             = 96 , //?????????
    x0061 = 97 ,  xWR1_AlignBw             = 97 , //        ?
    x0062 = 98 ,  xWR1_ClampFw1            = 98 , //         ?
    x0063 = 99 ,  xWR1_ClampFw2            = 99 , //         ?
    x0064 =100 ,  xWR1_ClampFw3            =100 , //?
    x0065 =101 ,  xWR1_ClampFw4            =101 , //?
    x0066 =102 ,  xWR1_IndxUp              =102 , //
    x0067 =103 ,  xWR1_IndxDn              =103 , //
    x0068 =104 ,  xWR1_IndxOverload        =104 , //
    x0069 =105 ,  xWR1_TurnDetect          =105 , // ??미지의 인풋.
    x006A =106 ,  x106                     =106 , //
    x006B =107 ,  x107                     =107 , //
    x006C =108 ,  x108                     =108 , //
    x006D =109 ,  x109                     =109 , //
    x006E =110 ,  x110                     =110 , //
    x006F =111 ,  x111                     =111 , //

    x0070 =112 ,  x112                     =112 , //
    x0071 =113 ,  x113                     =113 , //
    x0072 =114 ,  x114                     =114 , //
    x0073 =115 ,  x115                     =115 , //
    x0074 =116 ,  x116                     =116 , //
    x0075 =117 ,  x117                     =117 , //
    x0076 =118 ,  x118                     =118 , //
    x0077 =119 ,  x119                     =119 , //
    x0078 =120 ,  x120                     =120 , //
    x0079 =121 ,  x121                     =121 , //
    x007A =122 ,  x122                     =122 , //
    x007B =123 ,  x123                     =123 , //
    x007C =124 ,  x124                     =124 , //
    x007D =125 ,  x125                     =125 , //
    x007E =126 ,  x126                     =126 , //
    x007F =127 ,  x127                     =127 , //

    x0080 =128 ,  xHED_1Ready              =128 , //
    x0081 =129 ,  xHED_1Busy               =129 , //
    x0082 =130 ,  xHED_1End                =130 , //
    x0083 =131 ,  x131                     =131 , //
    x0084 =132 ,  xHED_2Ready              =132 , //
    x0085 =133 ,  xHED_2Busy               =133 , //
    x0086 =134 ,  xHED_2End                =134 , //
    x0087 =135 ,  x135                     =135 , //
    x0088 =136 ,  xHED_3Ready              =136 , //
    x0089 =137 ,  xHED_3Busy               =137 , //
    x008A =138 ,  xHED_3End                =138 , //
    x008B =139 ,  x139                     =139 , //
    x008C =140 ,  xHED_4Ready              =140 , //
    x008D =141 ,  xHED_4Busy               =141 , //
    x008E =142 ,  xHED_4End                =142 , //
    x008F =143 ,  x143                     =143 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yLDR_MgzInAC              =  0 , //
    y0001 =  1 ,  yLDR_MgzOutAC             =  1 , //
    y0002 =  2 ,  yLDR_ClampBw              =  2 , //
    y0003 =  3 ,  yLDR_ClampFw              =  3 , //
    y0004 =  4 ,  yLDR_PusherFw             =  4 , //
    y0005 =  5 ,  yLDR_ZBreak               =  5 , //
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
    y0012 = 18 ,  yULD_ClampBw              = 18 , //
    y0013 = 19 ,  yULD_ClampFw              = 19 , //
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
    y0025 = 37 ,  yETC_LVisnLp              = 37 , //
    y0026 = 38 ,  yETC_RStartLp             = 38 , //
    y0027 = 39 ,  yETC_RStopLp              = 39 , //
    y0028 = 40 ,  yETC_RResetLp             = 40 , //
    y0029 = 41 ,  yETC_RAirLp               = 41 , //
    y002A = 42 ,  yETC_RInitLp              = 42 , //
    y002B = 43 ,  yETC_RVisnLp              = 43 , //
    y002C = 44 ,  yETC_TwRedLp              = 44 , //
    y002D = 45 ,  yETC_TwYelLp              = 45 , //
    y002E = 46 ,  yETC_TwGrnLp              = 46 , //
    y002F = 47 ,  yETC_TwBzz                = 47 , //

    y0030 = 48 ,  yETC_MainSol              = 48 , //
    y0031 = 49 ,  yPRB_AirBlower            = 49 , //
    y0032 = 50 ,  yPRB_FeedingAC            = 50 , //
    y0033 = 51 ,  yWR1_FeedingAC            = 51 , //
    y0034 = 52 ,  yWR2_FeedingAC            = 52 , //
    y0035 = 53 ,  y053                      = 53 , //
    y0036 = 54 ,  y054                      = 54 , //
    y0037 = 55 ,  y055                      = 55 , //
    y0038 = 56 ,  yWR1_Align                = 56 , //
    y0039 = 57 ,  yWR2_Align                = 57 , //
    y003A = 58 ,  yWR3_Align                = 58 , //
    y003B = 59 ,  yPSB_Align                = 59 , //
    y003C = 60 ,  yPSB_OutPusher            = 60 , //
    y003D = 61 ,  y061                      = 61 , //
    y003E = 62 ,  y062                      = 62 , //
    y003F = 63 ,  y063                      = 63 , //

    y0040 = 64 ,  yPRB_Stpr                 = 64 , //
    y0041 = 65 ,  yWR1_Stpr                 = 65 , //
    y0042 = 66 ,  yWR2_Stpr                 = 66 , //
    y0043 = 67 ,  yWR3_Stpr                 = 67 , //
    y0044 = 68 ,  yPSB_Stpr                 = 68 , //
    y0045 = 69 ,  y069                      = 69 , //
    y0046 = 70 ,  yPSB_Marking              = 70 , //
    y0047 = 71 ,  y071                      = 71 , //
    y0048 = 72 ,  y072                      = 72 , //
    y0049 = 73 ,  y073                      = 73 , //
    y004A = 74 ,  yWR2_LiftBw               = 74 , //
    y004B = 75 ,  yWR2_LiftFw               = 75 , //
    y004C = 76 ,  yWR3_LiftBw               = 76 , //
    y004D = 77 ,  yWR3_LiftFw               = 77 , //
    y004E = 78 ,  yPSB_LiftBw               = 78 , //
    y004F = 79 ,  yPSB_LiftFw               = 79 , //

    y0050 = 80 ,  yWR1_Clamp                = 80 , // 터닝 클램프.
    y0051 = 81 ,  yWR1_Indx                 = 81 , // 핑거.
    y0052 = 82 ,  y082                      = 82 , //
    y0053 = 83 ,  y083                      = 83 , //
    y0054 = 84 ,  y084                      = 84 , //
    y0055 = 85 ,  y085                      = 85 , //
    y0056 = 86 ,  y086                      = 86 , //
    y0057 = 87 ,  y087                      = 87 , //
    y0058 = 88 ,  y088                      = 88 , //
    y0059 = 89 ,  y089                      = 89 , //
    y005A = 90 ,  y090                      = 90 , //
    y005B = 91 ,  y091                      = 91 , //
    y005C = 92 ,  y092                      = 92 , //
    y005D = 93 ,  y093                      = 93 , //
    y005E = 94 ,  y094                      = 94 , //
    y005F = 95 ,  y095                      = 95 , //

    y0060 = 96 ,  y096                      = 96 , //
    y0061 = 97 ,  yHED_1LotStart           = 97 , //
    y0062 = 98 ,  yHED_1JobChange          = 98 , //
    y0063 = 99 ,  yHED_1Reset               = 99 , //
    y0064 =100 ,  y100                      =100 , //
    y0065 =101 ,  yHED_2LotStart           =101 , //
    y0066 =102 ,  yHED_2JobChange          =102 , //
    y0067 =103 ,  yHED_2Reset               =103 , //
    y0068 =104 ,  y104                      =104 , //
    y0069 =105 ,  yHED_3LotStart           =105 , //
    y006A =106 ,  yHED_3JobChange          =106 , //
    y006B =107 ,  yHED_3Reset               =107 , //
    y006C =108 ,  y108                      =108 , //
    y006D =109 ,  yHED_4LotStart           =109 , //
    y006E =110 ,  yHED_4JobChange          =110 , //
    y006F =111 ,  yHED_4Reset               =111 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID {
    piLDR_YWait     = 0 ,
    piLDR_YPick         ,
    piLDR_YWork         ,
    piLDR_YPlace         ,
    MAX_PSTN_ID_MOTR0      ,

    piLDR_ZWait        = 0 ,
    piLDR_ZPickFwd         ,
    piLDR_ZClampOn         ,
    piLDR_ZPickBwd         ,
    piLDR_ZWorkStart       ,
    piLDR_ZPlaceFwd        ,
    piLDR_ZClampOff        ,
    piLDR_ZPlaceBwd        ,

    piLDR_ZWork            ,
    MAX_PSTN_ID_MOTR1      ,

    piWRK_XVsnWait     = 0 ,
    piWRK_XVsnWorkStart    ,

    piWRK_XVsnTrgStt       ,
    piWRK_XVsnTrgEnd       ,
    MAX_PSTN_ID_MOTR2      ,

    piWRK_YVsnWait     = 0 ,
    piWRK_YVsnWorkStart    ,

    piWRK_YVsnWork         ,
    MAX_PSTN_ID_MOTR3      ,

    piPSB_XMrkWait     = 0 ,
    piPSB_XMrkWorkStart    ,
    piPSB_XMrkVisn         ,

    piPSB_XVsnTrgStt       ,
    piPSB_XVsnTrgEnd       ,
    piPSB_XMrkWork         ,
    MAX_PSTN_ID_MOTR4      ,

    piPSB_YMrkWait     = 0 ,
    piPSB_YMrkWorkStart    ,
    piPSB_YMrkVisn         ,

    piPSB_YMrkVisnWork     ,    
    piPSB_YMrkWork         ,
    MAX_PSTN_ID_MOTR5      ,

//    piWK1_YFlpWait     = 0 ,
    piWK1_YFlpNormal   = 0 ,
    piWK1_YFlpInverse      ,
    MAX_PSTN_ID_MOTR6      ,

    piWK1_XInsWait     = 0 ,
    piWK1_XInsStpr         ,
    piWK1_XInsOut          ,
    MAX_PSTN_ID_MOTR7      ,

    piULD_YWait        = 0 ,
    piULD_YPick            ,
    piULD_YWork            ,
    piULD_YPlace           ,
    MAX_PSTN_ID_MOTR8      ,

    piULD_ZWait        = 0 ,
    piULD_ZPickFwd         ,
    piULD_ZClampOn         ,
    piULD_ZPickBwd         ,
    piULD_ZWorkStart       ,
    piULD_ZPlaceFwd        ,
    piULD_ZClampOff        ,
    piULD_ZPlaceBwd        ,

    piULD_ZWork            ,
    MAX_PSTN_ID_MOTR9      ,

};

enum EN_PSTN_VALUE {
    pvLDR_YWaitPs     = 0  ,
    pvLDR_YPickPs          ,
    pvLDR_YWorkPs          ,
    pvLDR_YPlacePs         ,
    MAX_PSTN_MOTR0         ,

    pvLDR_ZWaitPs        = 0 ,
    pvLDR_ZPickFwdPs         ,
    pvLDR_ZClampOnPs         ,
    pvLDR_ZPickBwdPs         ,
    pvLDR_ZWorkStartPs       ,
    pvLDR_ZPlaceFwdPs        ,
    pvLDR_ZClampOffPs        ,
    pvLDR_ZPlaceBwdPs        ,
    MAX_PSTN_MOTR1         ,

    pvWRK_XVsnWaitPs     = 0 ,
    pvWRK_XVsnWorkStartPs    ,
    MAX_PSTN_MOTR2          ,

    pvWRK_YVsnWaitPs     = 0 ,
    pvWRK_YVsnWorkStartPs    ,
    MAX_PSTN_MOTR3         ,

    pvPSB_XMrkWaitPs     = 0 ,
    pvPSB_XMrkWorkStartPs    ,
    pvPSB_XMrkVisnPs         ,
    MAX_PSTN_MOTR4         ,

    pvPSB_YMrkWaitPs     = 0 ,
    pvPSB_YMrkWorkStartPs    ,
    pvPSB_YMrkVisnPs         ,
    MAX_PSTN_MOTR5         ,

//    pvWK1_YFlpWaitPs     = 0 ,
    pvWK1_YFlpNormalPs   = 0 ,
    pvWK1_YFlpInversePs      ,
    MAX_PSTN_MOTR6         ,

    pvWK1_XInsWaitPs     = 0 ,
    pvWK1_XInsStprPs         ,
    pvWK1_XInsOutPs          ,
    MAX_PSTN_MOTR7           ,

    pvULD_YWaitPs        = 0 ,
    pvULD_YPickPs            ,
    pvULD_YWorkStartPs       ,
    pvULD_YPlacePs           ,
    MAX_PSTN_MOTR8           ,

    pvULD_ZWaitPs        = 0 ,
    pvULD_ZPickFwdPs         ,
    pvULD_ZClampOnPs         ,
    pvULD_ZPickBwdPs         ,
    pvULD_ZWorkStartPs       ,
    pvULD_ZPlaceFwdPs        ,
    pvULD_ZClampOffPs        ,
    pvULD_ZPlaceBwdPs        ,
    MAX_PSTN_MOTR9           ,

};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_Home             = 10 , //10
    mcLDR_UnClamp               , //11
    mcLDR_Clamp                 , //12
    mcLDR_PusherFw              , //13
    mcLDR_PusherBw              , //14
    mcLDR_TransferInAC          , //15
    mcLDR_TransferOutAC         , //16
    mcLDR_Supply                , //17
    mcLDR_Pick                  , //18
    mcLDR_Place                 , //19
    mcLDR_Z1PichDn              , //20
    mcLDR_Z1PichUp              , //21
    mcLDR_SttPstn               , //22
    mcRAL_PreStprUp             , //23
    mcRAL_PreStprDn             , //24

    mcRAL_Wk1Home          = 30 , //30
    mcRAL_Wk1CmpUp              , //31
    mcRAL_Wk1CmpDn              , //32
    mcRAL_Wk1StprUp             , //33
    mcRAL_Wk1StprDn             , //34
    mcRAL_Wk1AlignFB            , //35
    mcRAL_Wk1NomalPos           , //36
    mcRAL_Wk1ReversePos         , //37
    mcRAL_Wk2LiftUp             , //38
    mcRAL_Wk2LiftDn             , //39
    mcRAL_Wk2StprUp             , //40
    mcRAL_Wk2StprDn             , //41
    mcRAL_Wk2AlignFB            , //42
    mcRAL_Wk3LiftUp             , //43
    mcRAL_Wk3LiftDn             , //44
    mcRAL_Wk3StprUp             , //45
    mcRAL_Wk3StprDn             , //46
    mcRAL_Wk3AlignFB            , //47

    mcPSB_Home             = 50 , //50
    mcPSB_InitPos               , //51
    mcPSB_WorkStPos             , //52
    mcPSB_LiftUp                , //53
    mcPSB_LiftDn                , //54
    mcPSB_StprUp                , //55
    mcPSB_StprDn                , //56
    mcPSB_MarkCmpUp             , //57
    mcPSB_MarkCmpDn             , //58
    mcPSB_UD1Cycle              , //59
    mcPSB_MrkVisnInsp           , //60
    mcPsb_TransferDn            , //61 애 주석처리 되있었음...
    mcPSB_MarkingChange         , //62
    mcPSB_AlignFB               , //63
    mcPSB_TestMarking           , //64

    mcULD_Home             =  70, //70
    mcPSB_PusherFw              , //71
    mcPSB_PusherBw              , //72
    mcULD_TransferInAC          , //73
    mcULD_TransferOutAC         , //74
    mcULD_Supply                , //75
    mcULD_Pick                  , //76
    mcULD_Place                 , //77
    mcULD_Z1PichUp              , //78
    mcULD_Z1PichDn              , //79
    mcULD_SttPstn               , //80
    mcULD_UnClamp               , //81
    mcULD_Clamp                 , //82

    mcETC_RemoveIndex      =  90, //90
    mcETC_FlipHome              , //91 Transfer, Flipper Motor Home
    mcHED_InspMan               , //92
    mcETC_Sampling1Cycle        , //93
    mcETC_AllBeltOn             , //94
    mcETC_AllBeltOff            , //95
    mcETC_AllStprUp             , //96
    mcETC_ALlStprDn             , //97
    mcETC_Wk1AlignLiftUp        , //98
    mcETC_Wk2AlignLiftUp        , //99
    mcETC_Wk3AlignLiftUp        , //100
    mcETC_MrkAlignLiftUp        , //101
    mcETC_AllAlignLiftUp        , //102
    mcETC_AllAlignLiftDn        , //103
    mcETC_TransferUp            , //104
    mcETC_TransferDn            , //105
    mcETC_MainAirOnOff          , //106
    mcETC_ManLotEnd             , //107

    MAX_MANUAL_CYCLE
};
#endif


