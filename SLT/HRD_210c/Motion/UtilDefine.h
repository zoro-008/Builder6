#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_REGRIP_ARRAY 10


#define MAX_WAFER_ROW   50
#define MAX_WAFER_COL   50

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_ARAY_ID {
    riLDR       = 0  , //Loader                         
    riPRB            , //Pre Buffer
    riREV            , //Work Epoxy 검사 결과.         riREV  riWRE
    riREF            , //Work Epoxy RAIL FEEDING       riREF  riRAE
    riRDV            , //Work Die Atach 검사 결과      riRDV  riWRD
    riRDF            , //Work Die Atach RAIL FEEDING   riRDF  riRAD
    riHED            , //Tool                          riHED  riDIE
    riCST            , //Cassette
    riSTG            , //WAFER Stage
    riPSV            , //PSB   검사 결과.              riPSV  riPSB
    riPSF            , //PSB RAIL FEEDING              riPSF  riRAP
    riULD            , //Unloader
    riMAP            , //

    //riDFM            , //Default Map Set. 유즈맵이 아닌경우 디폴트 맵을 세팅 할때 사용 하는 놈.

    MAX_ARAY
};

enum EN_MOTR_ID {
    miLDR_YCmp    = 0  , //  로더 Y축
    miLDR_ZCmp    = 1  , //  로더 Z축
    miPRB_XIdx    = 2  , //  피딩 1번째
    miPRB_YIns    = 3  , //  카메라 Y축
    miWRE_XIdx    = 4  , //  피딩 2번째
    miWRE_XEpx    = 5  , //  에폭시 X축 아직 넌 미정이야 .
    miWRE_YEpx    = 6  , //  에폭시 Y축
    miWRE_ZEpx    = 7  , //  에폭시 Z축
    miWRD_XIdx    = 8  , //  피딩 3번째
    miWRD_YIns    = 9  , //  피딩 4번째
    miWRD_XHed    = 10 , //  해드 X축
    miWRD_YHed    = 11 , //  해드 Y축
    miWRD_ZHed    = 12 , //  해드 Z축
    miWRD_THed    = 13 , //  해드 삼메이
    miWRD_XStg    = 14 , //  스테이지 X축
    miWRD_YStg    = 15 , //  스테이지 Y축
    miWRD_TStg    = 16 , //  스테이지 세타
    miWRD_ZEjt    = 17 , //  이젝 Z측
    miWRD_ZExd    = 18 , //  익스펜딩
    miWRD_ZCst    = 19 , //  카세트 Z축
    miWRD_XTrs    = 20 , //  트렌스퍼 X축
    miPSB_XIdx    = 21 , //  피딩 4번째
    miULD_YCmp    = 22 , //  언로더 Z축
    miULD_ZCmp    = 23 , //  언로더 Z축
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_Stopper  = 0   , // 로더 작업 때 마다 잡아주는 실린더
    aiLDR_MgzGrip  = 1   , // 로더 작업 중 다른 매거진 잡는 실린더
    aiLDR_Out      = 2   , // 로더 작업 후 배출 해 주는 실린더
    aiLDR_Pusher   = 3   , // 로더 푸셔 실린더
    aiPRB_Stopper  = 4   , // 피더 진입 실린더
    aiPRB_Clamp    = 5   , //
    aiWRE_Clamp    = 6   , //
    aiWRD_Clamp    = 7   , //
    aiPSB_Clamp    = 8   , //
    aiEJT_UpDn     = 9   , // 이젝 업 다운 실린더
    aiTRS_Chuck    = 10  , // 트렌스퍼 Check 실린더
    aiULD_Stopper  = 11  , // 언로더 작업 중 다른 매거진 잡는 실린더
    aiULD_MgzGrip  = 12  , // 언로더 작업때 마다 잡아주는 실린더
    aiULD_Out      = 13  , // 언로더 작업 후 배출 해 주는 실린더
    aiTRS_UpDn     = 14  , // 트렌스퍼 업다운 실린더.
    MAX_ACTR
};
/*
enum EN_ACTR_ID {
    aiLDR_Stopper  = 0   , // 로더 작업 때 마다 잡아주는 실린더
    aiLDR_MgzGrip  = 1   , // 로더 작업 중 다른 매거진 잡는 실린더
    aiLDR_Out      = 2   , // 로더 작업 후 배출 해 주는 실린더
    aiLDR_Pusher   = 3   , // 로더 푸셔 실린더
    aiPRB_Stopper  = 4   , // 피더 진입 실린더
    aiPRB_Clamp    = 5   , //
    aiPRB_CAlign   = 6   , // 프리버퍼 칩얼라인 빠지고 트렌스퍼 업다운추가
    aiWRE_BAlign   = 7   , // 피더 얼라인 실린더
    aiWRE_Clamp    = 8   , //
    aiWRE_CAlign   = 9   , //
    aiWRD_Clamp    = 10  , //
    aiWRD_CAlign   = 11  , //
    aiPSB_Clamp    = 12  , //
    aiEJT_UpDn     = 13  , // 이젝 업 다운 실린더
    aiTRS_Chuck    = 14  , // 트렌스퍼 Check 실린더
    aiULD_Stopper  = 15  , // 언로더 작업 중 다른 매거진 잡는 실린더
    aiULD_MgzGrip  = 16  , // 언로더 작업때 마다 잡아주는 실린더
    aiULD_Out      = 17  , // 언로더 작업 후 배출 해 주는 실린더
    aiTRS_UpDn     = 18  , //트렌스퍼 업다운 실린더.
    MAX_ACTR
};
*/

enum EN_ERR_ID {
    eiETC_MainPower      =  0   , //
    eiETC_MainAir               , //001
    eiETC_ToStartTO             , //002
    eiETC_ToStopTO              , //003
    eiETC_Emergency             , //004
    eiETC_FDoor                 , //005
    eiETC_BDoor                 , //006
    eiETC_LotEnd                , //007
    eiETC_AllHomeTO             , //008
    eiETC_ManCycleTO            , //009
    ei011                       , //010
    eiHED_VisnTO                , //011
    ei013                       , //012
    eiLDR_CycleTO               , //013
    eiPRB_CycleTO               , //014
    eiWRE_CycleTO               , //015
    eiEPX_CycleTO               , //016
    eiWRD_CycleTO               , //017
    eiHED_CycleTO               , //018
    eiSTG_CycleTO               , //019
    eiPSB_CycleTO               , //020
    eiULD_CycleTO               , //021
    ei023                       , //022
    ei024                       , //023
    eiLDR_HomeTO                , //024
    eiPRB_HomeTO                , //025
    eiWRE_HomeTO                , //026
    eiEPX_HomeTO                , //027
    eiWRD_HomeTO                , //028
    eiHED_HomeTO                , //029
    eiSTG_HomeTO                , //030
    eiPSB_HomeTO                , //031
    eiULD_HomeTO                , //032
    ei034                       , //033
    ei035                       , //034
    eiLDR_ToStartTO             , //035
    eiPRB_ToStartTO             , //036
    eiWRE_ToStartTO             , //037
    eiEPX_ToStartTO             , //038
    eiWRD_ToStartTO             , //039
    eiHED_ToStartTO             , //040
    eiSTG_ToStartTO             , //041
    eiPSB_ToStartTO             , //042
    eiULD_ToStartTO             , //043
    ei045                       , //044
    ei046                       , //045
    eiLDR_ToStopTO              , //046
    eiPRB_ToStopTO              , //047
    eiWRE_ToStopTO              , //048
    eiEPX_ToStopTO              , //049
    eiWRD_ToStopTO              , //050
    eiHED_ToStopTO              , //051
    eiSTG_ToStopTO              , //052
    eiPSB_ToStopTO              , //053
    eiULD_ToStopTO              , //054
    ei056                       , //055
    ei057                       , //056
    eiLDR_YCmpHomeEnd           , //057
    eiLDR_ZCmpHomeEnd           , //058
    eiPRB_XIdxHomeEnd           , //059
    eiPRB_YInsHomeEnd           , //060
    eiWRE_XIdxHomeEnd           , //061
    eiWRE_XEpxHomeEnd           , //062
    eiWRE_YEpxHomeEnd           , //063
    eiWRE_ZEpxHomeEnd           , //064
    eiWRD_XIdxHomeEnd           , //065
    eiWRD_YInsHomeEnd           , //066
    eiWRD_XHedHomeEnd           , //067
    eiWRD_YHedHomeEnd           , //068
    eiWRD_ZHedHomeEnd           , //069
    eiWRD_THedHomeEnd           , //070
    eiWRD_XStgHomeEnd           , //071
    eiWRD_YStgHomeEnd           , //072
    eiWRD_TStgHomeEnd           , //073
    eiWRD_ZEjtHomeEnd           , //074
    eiWRD_ZExdHomeEnd           , //075
    eiWRD_ZCstHomeEnd           , //076
    eiWRD_XTrsHomeEnd           , //077
    eiPSB_XIdxHomeEnd           , //078
    eiULD_YCmpHomeEnd           , //079
    eiULD_ZCmpHomeEnd           , //080
    eiLDR_YCmpNegLim            , //081
    eiLDR_ZCmpNegLim            , //082
    eiPRB_XIdxNegLim            , //083
    eiPRB_YInsNegLim            , //084
    eiWRE_XIdxNegLim            , //085
    eiWRE_XEpxNegLim            , //086
    eiWRE_YEpxNegLim            , //087
    eiWRE_ZEpxNegLim            , //088
    eiWRD_XIdxNegLim            , //089
    eiWRD_YInsNegLim            , //090
    eiWRD_XHedNegLim            , //091
    eiWRD_YHedNegLim            , //092
    eiWRD_ZHedNegLim            , //093
    eiWRD_THedNegLim            , //094
    eiWRD_XStgNegLim            , //095
    eiWRD_YStgNegLim            , //096
    eiWRD_TStgNegLim            , //097
    eiWRD_ZEjtNegLim            , //098
    eiWRD_ZExdNegLim            , //099
    eiWRD_ZCstNegLim            , //100
    eiWRD_XTrsNegLim            , //101
    eiPSB_XIdxNegLim            , //102
    eiULD_YCmpNegLim            , //103
    eiULD_ZCmpNegLim            , //104
    eiLDR_YCmpPosLim            , //105
    eiLDR_ZCmpPosLim            , //106
    eiPRB_XIdxPosLim            , //107
    eiPRB_YInsPosLim            , //108
    eiWRE_XIdxPosLim            , //109
    eiWRE_XEpxPosLim            , //110
    eiWRE_YEpxPosLim            , //111
    eiWRE_ZEpxPosLim            , //112
    eiWRD_XIdxPosLim            , //113
    eiWRD_YInsPosLim            , //114
    eiWRD_XHedPosLim            , //115
    eiWRD_YHedPosLim            , //116
    eiWRD_ZHedPosLim            , //117
    eiWRD_THedPosLim            , //118
    eiWRD_XStgPosLim            , //119
    eiWRD_YStgPosLim            , //120
    eiWRD_TStgPosLim            , //121
    eiWRD_ZEjtPosLim            , //122
    eiWRD_ZExdPosLim            , //123
    eiWRD_ZCstPosLim            , //124
    eiWRD_XTrsPosLim            , //125
    eiPSB_XIdxPosLim            , //126
    eiULD_YCmpPosLim            , //127
    eiULD_ZCmpPosLim            , //128
    eiLDR_YCmpAlarm             , //129
    eiLDR_ZCmpAlarm             , //130
    eiPRB_XIdxAlarm             , //131
    eiPRB_YInsAlarm             , //132
    eiWRE_XIdxAlarm             , //133
    eiWRE_XEpxAlarm             , //134
    eiWRE_YEpxAlarm             , //135
    eiWRE_ZEpxAlarm             , //136
    eiWRD_XIdxAlarm             , //137
    eiWRD_YInsAlarm             , //138
    eiWRD_XHedAlarm             , //139
    eiWRD_YHedAlarm             , //140
    eiWRD_ZHedAlarm             , //141
    eiWRD_THedAlarm             , //142
    eiWRD_XStgAlarm             , //143
    eiWRD_YStgAlarm             , //144
    eiWRD_TStgAlarm             , //145
    eiWRD_ZEjtAlarm             , //146
    eiWRD_ZExdAlarm             , //147
    eiWRD_ZCstAlarm             , //148
    eiWRD_XTrsAlarm             , //149
    eiPSB_XIdxAlarm             , //150
    eiULD_YCmpAlarm             , //151
    eiULD_ZCmpAlarm             , //152
    ei154                       , //153
    ei155                       , //154
    ei156                       , //155
    ei157                       , //156
    ei158                       , //157
    ei159                       , //158
    eiLDR_Stopper               , //159
    eiLDR_MgzGrip               , //160
    eiLDR_Out                   , //161
    eiLDR_Pusher                , //162
    eiPRB_Stopper               , //163
    eiPRB_Clamp                 , //164
    eiPRB_CAlign                , //165
    eiWRE_BAlign                , //166
    eiWRE_Clamp                 , //167
    eiWRE_CAlign                , //168
    eiWRD_Clamp                 , //169
    eiWRD_CAlign                , //170
    eiPSB_Clamp                 , //171
    eiEJT_UpDn                  , //172
    eiTRS_Chuck                 , //173
    eiULD_Stopper               , //174
    eiULD_MgzGrip               , //175
    eiULD_Out                   , //176
    ei178                       , //177
    ei179                       , //178
    ei180                       , //179
    ei181                       , //180
    eiLDR_Unknwn                , //181
    eiPRB_Unknwn                , //182
    eiWRE_Unknwn                , //183
    eiWRD_Unknwn                , //184
    eiSTG_Unknwn                , //185
    eiCST_Unknwn                , //186
    eiPSB_Unknwn                , //187
    eiULD_Unknwn                , //188
    ei190                       , //189
    ei191                       , //190
    eiLDR_Dispr                 , //191
    eiPRB_Dispr                 , //192
    eiWRE_Dispr                 , //193
    eiWRD_Dispr                 , //194
    eiSTG_Dispr                 , //195
    eiCST_Dispr                 , //196
    eiPSB_Dispr                 , //197
    eiULD_Dispr                 , //198
    ei200                       , //199
    ei201                       , //200
    ei202                       , //201
    ei203                       , //202
    eiLDR_NeedMgz               , //203
    eiLDR_PickFail              , //204
    eiLDR_PshOverload           , //205
    eiLDR_FullMgz               , //206
    eiLDR_PshFeedingFail        , //207
    ei209                       , //208
    eiPRB_OriVsFailCnt          , //209
    eiPRB_OriVsGrabFail         , //210
    eiPRB_OriVsInspFail         , //211
    eiPRB_OriVsThchFail         , //212
    ei214                       , //213
    eiHED_RbfVsFailCnt          , //214
    eiHED_RbfVsGrabFail         , //215
    eiHED_RbfVsInspFail         , //216
    eiHED_RbfVsThchFail         , //217
    eiHED_RatVsFailCnt          , //218
    eiHED_RatVsGrabFail         , //219
    eiHED_RatVsInspFail         , //220
    eiHED_RatVsThchFail         , //221
    eiSTG_WfrVsFailCnt          , //222
    eiSTG_WfrVsGrabFail         , //223
    eiSTG_WfrVsInspFail         , //224
    eiSTG_WfrVsThchFail         , //225
    eiSTG_WfoVsFailCnt          , //226
    eiSTG_WfoVsGrabFail         , //227
    eiSTG_WfoVsInspFail         , //228
    eiSTG_WfoVsThchFail         , //229
    ei231                       , //230
    ei232                       , //231
    eiEPX_Rs232ComFail          , //232
    ei234                       , //233
    ei235                       , //234
    eiWRE_OriSsrDttFail         , //235
    eiWRE_RegripPosMinOver      , //236
    eiWRE_WorkPosMaxOver        , //237
    ei239                       , //238
    ei240                       , //239
    eiWRD_RegripPosMinOver      , //240
    eiWRD_WorkPosMaxOver        , //241
    ei243                       , //242
    ei244                       , //243
    eiPSB_OutFeedingFail        , //244
    ei246                       , //245
    eiHED_VacuumSsr             , //246
    ei248                       , //247
    eiPRB_RegripPosMinOver      , //248
    eiPRB_WorkPosMaxOver        , //249
    ei251                       , //250
    eiCST_NeedChange            , //251
    eiCST_EmptyUnknwn           , //252
    eiCST_Need                  , //253
    ei255                       , //254
    eiSTG_WaferNotDetected      , //255
    eiSTG_MapConverFail         , //256
    ei258                       , //257
    ei259                       , //258
    eiULD_NeedMgz               , //259
    eiULD_PickFail              , //260
    eiULD_FullMgz               , //261
    ei262                       , //262
    ei263                       , //263
    eiHED_MotorYZCrash          , //264
    eiHED_VoiceCoilSevo         , //265
    eiHED_VoiceCoilOverload     , //266
    eiSTG_TransWaferFeedingFail , //267
    eiHED_Unknwn                , //268
    eiHED_Dispr                 , //269
    eiSTG_EjectorSoftLimit      , //272
    eiSTG_EjectorCrash          , //273
    eiSTG_TLmtErr               , //274
    eiRAD_VisnRlstOver          , //275
    eiSTG_VisnRlstOver          , //276
    eiHED_EjtVacuumFail         , //277



    MAX_ERR
};


enum EN_INPUT_ID {
//    xHED_VoiceCoilSevo ,


    x0000 =  0 ,  xETC_StartSw               =  0 , // Sw << Swich 신호
    x0001 =  1 ,  xETC_StopSw                =  1 , // Sw << Swich 신호
    x0002 =  2 ,  xETC_ResetSw               =  2 , // Sw << Swich 신호
    x0003 =  3 ,  xETC_LampSw                =  3 , // Sw << Swich 신호
    x0004 =  4 ,  xETC_EmgSw                 =  4 , // Sw << Swich 신호
    x0005 =  5 ,  xETC_FDoor                 =  5 , // F  << Forward
    x0006 =  6 ,  xETC_BDoor                 =  6 , // B  << Backward
    x0007 =  7 ,  x007                       =  7 , //
    x0008 =  8 ,  xETC_PowerOn               =  8 , //
    x0009 =  9 ,  xETC_MainSol               =  9 , //
    x000A = 10 ,  x010                       = 10 , //
    x000B = 11 ,  xLDR_InDetect              = 11 , //
    x000C = 12 ,  xLDR_MgzDetect1            = 12 , //
    x000D = 13 ,  xLDR_MgzDetect2            = 13 , //
    x000E = 14 ,  xLDR_PshOverload           = 14 , //
    x000F = 15 ,  xLDR_FullDetect            = 15 , //

    x0010 = 16 ,  xLDR_OutFw                 = 16 , // aiLDR_XOut
    x0011 = 17 ,  xLDR_OutBw                 = 17 , //
    x0012 = 18 ,  xLDR_SptrFw                = 18 , // aiLDR_Sptr
    x0013 = 19 ,  xLDR_SptrBw                = 19 , //
    x0014 = 20 ,  xLDR_MgzGripFw             = 20 , // aiLDR_Brkn
    x0015 = 21 ,  xLDR_MgzGripBw             = 21 , //
    x0016 = 22 ,  xLDR_PusherFw              = 22 , // aiLDR_Push
    x0017 = 23 ,  xLDR_PusherBw              = 23 , //
    x0018 = 24 ,  xPRB_ChckDetect            = 24 , // FEEDER   Chuck 1 Down
    x0019 = 25 ,  xWRE_ChckDetect            = 25 , //          Chuck 2 Down
    x001A = 26 ,  xWRD_ChckDetect            = 26 , //          Chuck 3 Down
    x001B = 27 ,  xPSB_ChckDetect            = 27 , //          Chuck 4 Down
    x001C = 28 ,  xPRB_InDetect              = 28 , //          BOAT In 감지
    x001D = 29 ,  xPSB_OutDetect             = 29 , //          BOAT Out 감지
    x001E = 30 ,  xPRB_StprUp                = 30 , //          진입 스토퍼 Up
    x001F = 31 ,  xPRB_StprDn                = 31 , //          진입 스토퍼 Out

    x0020 = 32 ,  xWRE_TouchSsr              = 32 , // Head     오버로드
    x0021 = 33 ,  xHED_Overload              = 33 , //          베큠 센서
    x0022 = 34 ,  xHED_Vaccum                = 34 , // Stage    웨이퍼 센서 (보류)
    x0023 = 35 ,  xSTG_Wafer                 = 35 , // Transfer 웨이퍼 센서
    x0024 = 36 ,  xTRS_Wafer                 = 36 , //
    x0025 = 37 ,  xEJT_Crash_Sensor          = 37 , //
    x0026 = 38 ,  xEJT_CylinderUp            = 38 , // Eject    실린더 업
    x0027 = 39 ,  xEJT_CylinderDn            = 39 , //          실린더 다운
    x0028 = 40 ,  xEJT_Vaccum                = 40 , //          베큠 센서
    x0029 = 41 ,  xCST_Detect                = 41 , // Cassette 감시 센서.
    x002A = 42 ,  x042                       = 42 , //
    x002B = 43 ,  x043                       = 43 , //
    x002C = 44 ,  xULD_InDetect              = 44 , // UnLoader 진입 감지 센서
    x002D = 45 ,  xULD_MgzDetect1            = 45 , //          매거진 감지 센서 1
    x002E = 46 ,  xULD_MgzDetect2            = 46 , //          매거진 감지 센서 2
    x002F = 47 ,  xULD_FullDetect            = 47 , //          매거진 풀 감지 센서

    x0030 = 48 ,  xULD_OutFw                 = 48 , // aiULD_Brkn
    x0031 = 49 ,  xULD_OutBw                 = 49 , //
    x0032 = 50 ,  xULD_MgzGripFw             = 50 , // aiULD_Sptr
    x0033 = 51 ,  xULD_MgzGripBw             = 51 , //
    x0034 = 52 ,  xULD_StprFw                = 52 , // aiULD_XOut
    x0035 = 53 ,  xWRD_Vaccum                = 53 , //
    x0036 = 54 ,  x054                       = 54 , //
    x0037 = 55 ,  x055                       = 55 , //
    x0038 = 56 ,  x056                       = 56 , //
    x0039 = 57 ,  x057                       = 57 , //
    x003A = 58 ,  x058                       = 58 , //
    x003B = 59 ,  x059                       = 59 , //
    x003C = 60 ,  x060                       = 60 , //
    x003D = 61 ,  x061                       = 61 , //
    x003E = 62 ,  x062                       = 62 , //
    x003F = 63 ,  x063                       = 63 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {

    y0000 =  0 ,  yETC_StartLp               =  0 , //
    y0001 =  1 ,  yETC_StopLp                =  1 , //
    y0002 =  2 ,  yETC_ResetLp               =  2 , //
    y0003 =  3 ,  yETC_LampLp                =  3 , //
    y0004 =  4 ,  yETC_TwRedLp               =  4 , //
    y0005 =  5 ,  yETC_TwYelLp               =  5 , //
    y0006 =  6 ,  yETC_TwGrnLp               =  6 , //
    y0007 =  7 ,  yETC_TwBzz                 =  7 , //
    y0008 =  8 ,  yETC_MainSol               =  8 , //
    y0009 =  9 ,  yETC_Lamp                  =  9 , //
    y000A = 10 ,  yLDR_OutFw                 = 10 , //
    y000B = 11 ,  y011                       = 11 , // yLDR_OutBw
    y000C = 12 ,  yLDR_SptrFw                = 12 , //
    y000D = 13 ,  y013                       = 13 , // yLDR_SptrBw
    y000E = 14 ,  yLDR_MgzGripFw             = 14 , //
    y000F = 15 ,  y015                       = 15 , // yLDR_MgzGripBw

    y0010 = 16 ,  yLDR_PushFw                = 16 , //
    y0011 = 17 ,  y017                       = 17 , // yLDR_PushBw
    y0012 = 18 ,  yLDR_BreakOff              = 18 , //
    y0013 = 19 ,  yPRB_StprUp                = 19 , //
    y0014 = 20 ,  yWRD_Eject                 = 20 , // yPRB_StprDn
    y0015 = 21 ,  yWRD_Vaccum                = 21 , //
    y0016 = 22 ,  y022                       = 22 , //
    y0017 = 23 ,  y023                       = 23 , //
    y0018 = 24 ,  yPRB_Sol1                  = 24 , //
    y0019 = 25 ,  yWRE_Sol2                  = 25 , //
    y001A = 26 ,  yWRD_Tol3                  = 26 , //
    y001B = 27 ,  yPSB_Sol4                  = 27 , //
    y001C = 28 ,  yHED_Vaccum                = 28 , //
    y001D = 29 ,  yHED_Eject                 = 29 , //
    y001E = 30 ,  y030                       = 30 , //
    y001F = 31 ,  y031                       = 31 , //

    y0020 = 32 ,  yWRE_LightOnOff            = 32 , // 이거 위치가 디스펜서 하는곳이 맞엇나?
    y0021 = 33 ,  yWRE_CleanSuction          = 33 , // 넌왜 두개나 대냐.... Rs232 로 바뀜 삭제.
    y0022 = 34 ,  y034                       = 34 , //
    y0023 = 35 ,  yETC_LampOnOff             = 35 , // 미확정 아나 미확정이라닝...한개는 익스펜딩인듯.
    y0024 = 36 ,  yTRS_Dn                    = 36 , //  트렌스퍼 실린더 다운.
    y0025 = 37 ,  y037                       = 37 , //
    y0026 = 38 ,  ySTG_Dryer                 = 38 , //
    y0027 = 39 ,  y039                       = 39 , //
    y0028 = 40 ,  yEJT_Suction               = 40 , // 이젝에 왠 석션이야...
    y0029 = 41 ,  y041                       = 41 , //
    y002A = 42 ,  yEJT_CylinderUp            = 42 , //
    y002B = 43 ,  y043                       = 43 , // yEJT_CylinderDn
    y002C = 44 ,  yTRS_ChuckSol              = 44 , //
    y002D = 45 ,  yULD_OutFw                 = 45 , //
    y002E = 46 ,  y046                       = 46 , // yULD_OutBw
    y002F = 47 ,  yULD_MgzGripFw             = 47 , //

    y0030 = 48 ,  y048                       = 48 , // yULD_SptrBw
    y0031 = 49 ,  yULD_SptrFw                = 49 , //
    y0032 = 50 ,  y050                       = 50 , // yULD_MgzGripBw
    y0033 = 51 ,  yHED_VoiceCoilServo        = 51 , //
    y0034 = 52 ,  yULD_BreakOff              = 52 , //
    y0035 = 53 ,  y053                       = 53 , //
    y0036 = 54 ,  y054                       = 54 , //
    y0037 = 55 ,  y055                       = 55 , //
    y0038 = 56 ,  y056                       = 56 , //
    y0039 = 57 ,  y057                       = 57 , //
    y003A = 58 ,  y058                       = 58 , //
    y003B = 59 ,  y059                       = 59 , //
    y003C = 60 ,  y060                       = 60 , //
    y003D = 61 ,  y061                       = 61 , //
    y003E = 62 ,  y062                       = 62 , //
    y003F = 63 ,  y063                       = 63 , //
    MAX_OUTPUT
};

enum EN_PSTN_ID { //프로그램에서 가야할 아이디...

    piLDR_YCmpWait       = 0 ,
//    piLDR_YCmpIn             ,
    piLDR_YCmpWorkStt        ,
    MAX_PSTN_ID_MOTR0        ,

    piLDR_ZCmpWait       = 0 ,
    piLDR_ZCmpIn             ,
    piLDR_ZCmpWorkStt        ,
    piLDR_ZCmpOut            ,
    piLDR_ZCmpWork           ,
    MAX_PSTN_ID_MOTR1        ,

    piPRB_XIdxWait       = 0 ,
    piPRB_XIdxWorkStt        ,
//    piPRB_XIdxReGripCntOfs   ,
//    piPRB_XIdxReGripDisOfs   ,
    piPRB_XIdxOut            ,
    piPRB_XIdxWork           ,
    MAX_PSTN_ID_MOTR2        ,

    piPRB_YInsWait       = 0 ,
    piPRB_YInsWorkStart      ,
    piPRB_YInsWork           ,
    MAX_PSTN_ID_MOTR3        ,

    piWRE_XIdxWait       = 0 ,
    piWRE_XIdxWorkStt        ,
//    piWRE_XIdxPull           ,
//    piWRE_XIdxBack           ,
    piWRE_XIdxOut            ,
    piWRE_XIdxWork           ,
    MAX_PSTN_ID_MOTR4        ,

    piWRE_XEpxWait       = 0 ,
    piWRE_XEpxWorkStt        ,
    piWRE_XEpxOri            ,
    piWRE_XEpxClean          ,
//    piWRE_XEpxWork           ,
    MAX_PSTN_ID_MOTR5        ,

    piWRE_YEpxWait       = 0 ,
    piWRE_YEpxWorkStt        ,
    piWRE_YEpxOri            ,
    piWRE_YEpxClean          ,
    piWRE_YEpxWork           ,
    MAX_PSTN_ID_MOTR6        ,

    piWRE_ZEpxWait       = 0 ,
//    piWRE_ZEpxOriStt         ,
    piWRE_ZEpxWork           ,
    piWRE_ZEpxWorkAt         ,
    piWRE_ZEpxCleanWait      ,
    piWRE_ZEpxClean          ,
    MAX_PSTN_ID_MOTR7        ,

    piWRD_XIdxWait       = 0 ,
    piWRD_XIdxWorkStt        ,
//    piWRD_XIdxPull           ,
//    piWRD_XIdxBack           ,
    piWRD_XIdxOut            ,
    piWRD_XIdxWork           ,
    MAX_PSTN_ID_MOTR8        ,

    piWRD_YInsWait       = 0 ,
    piWRD_YInsWorkStt        ,
    piWRD_YInsWork           ,
    MAX_PSTN_ID_MOTR9        ,

    piWRD_XHedWait       = 0 ,
    piWRD_XHedWafer          ,
    piWRD_XHedStrip          ,
    piWRD_XHedWork           ,
    MAX_PSTN_ID_MOTR10       ,

    piWRD_YHedWait       = 0 ,
    piWRD_YHedWafer          ,
    piWRD_YHedWaferWait      ,
    piWRD_YHedWaferZMove     ,
    piWRD_YHedStripStt       ,
    piWRD_YHedStripWait      ,
    piWRD_YHedStripZMove     ,
    piWRD_YHedWork           ,
    MAX_PSTN_ID_MOTR11       ,

    piWRD_ZHedWait       = 0 ,
    piWRD_ZHedMove           ,
    piWRD_ZHedWfWorkWait     ,
    piWRD_ZHedWfWork         ,
    piWRD_ZHedStWorkWait     ,
    piWRD_ZHedStWork         ,
    MAX_PSTN_ID_MOTR12       ,

    piWRD_THedWait       = 0 ,
//    piWRD_THedWorkStt        ,
    piWRD_THedWafer          ,
    piWRD_THedStrip          ,
    MAX_PSTN_ID_MOTR13       ,

    piWRD_XStgWait       = 0 ,
    piWRD_XStgCassette       , //카세트 쪽
    piWRD_XStgAlign          , //웨이퍼 센터에 웨이퍼 전체 세타 얼라인 하는 칩.
    piWRD_XStgFindOri        , //첫 위치 찾는 기준 마크 확인 포지션.
    piWRD_XStgFirstChip      , //첫 위치 포지션.
    piWRD_XStgWork           , //작업 위치 이건 안쓰고 GetNextPos에서 처리.
    MAX_PSTN_ID_MOTR14       ,

    piWRD_YStgWait       = 0 ,
    piWRD_YStgCassette       ,
    piWRD_YStgAlign          ,
    piWRD_YStgFindOri        ,
    piWRD_YStgFirstChip      ,
    piWRD_YStgWork           ,
    MAX_PSTN_ID_MOTR15       ,



    //piWRD_TStgWait       = 0 ,
    //piWRD_TStgCassette       ,
    //piWRD_TStgAlign          ,
    //piWRD_TStgWork           ,
    //piWRD_TStgWorkStt        ,
    //MAX_PSTN_ID_MOTR16       ,

    piWRD_TStgWait       = 0 ,
    piWRD_TStgCassette       ,
    piWRD_TStgAlign          ,
    piWRD_TStgWork           ,
    MAX_PSTN_ID_MOTR16       ,



    piWRD_ZEjtWait       = 0 ,
    piWRD_ZEjtWorkBefore     ,
    piWRD_ZEjtWorkStt        ,
    MAX_PSTN_ID_MOTR17       ,

    piWRD_ZExdWait       = 0 ,
    piWRD_ZExdGet            ,
    piWRD_ZExdExpend         ,
    MAX_PSTN_ID_MOTR18       ,

    piWRD_ZCstWait       = 0 ,
    piWRD_ZCstWorkStt        ,
    piWRD_ZCstWork           ,
    piWRD_ZCstOut            ,
    MAX_PSTN_ID_MOTR19       ,

    piWRD_XTrsWait       = 0 ,
    piWRD_XTrsPick           ,
    piWRD_XTrsPickWait       ,
    piWRD_XTrsPlace          ,
    MAX_PSTN_ID_MOTR20       ,

    piPSB_XIdxWait       = 0 ,
    piPSB_XIdxClamp          ,
    piPSB_XIdxPull           ,
    piPSB_XIdxBack           ,
    piPSB_XIdxOut1           ,
    piPSB_XIdxOut2           ,
    MAX_PSTN_ID_MOTR21       ,

    piULD_YCmpWait       = 0 ,
//    piULD_YCmpIn             ,
    piULD_YCmpWorkStt        ,
    MAX_PSTN_ID_MOTR22        ,

    piULD_ZCmpWait       = 0 ,
    piULD_ZCmpIn             ,
    piULD_ZCmpWorkStt        ,
    piULD_ZCmpOut            ,
    piULD_ZCmpWork           ,
    MAX_PSTN_ID_MOTR23       ,

};

enum EN_PSTN_VALUE {
    pvLDR_YCmpWait          = 0 ,
//    pvLDR_YCmpIn             ,
    pvLDR_YCmpWorkStt        ,
    MAX_PSTN_MOTR0           ,

    pvLDR_ZCmpWait          = 0 ,
    pvLDR_ZCmpIn             ,
    pvLDR_ZCmpWorkStt        ,
    pvLDR_ZCmpOut            ,
    MAX_PSTN_MOTR1           ,

    pvPRB_XIdxWait          = 0 ,
    pvPRB_XIdxWorkStt        ,
//    pvPRB_XIdxReGripCntOfs   ,
//    pvPRB_XIdxReGripDisOfs   ,
    pvPRB_XIdxOut            ,
    MAX_PSTN_MOTR2           ,

    pvPRB_YInsWait          = 0 ,
    pvPRB_YInsWorkStart      ,
    MAX_PSTN_MOTR3           ,

    pvWRE_XIdxWait          = 0 ,
    pvWRE_XIdxWorkStt        ,
//    pvWRE_XIdxPull           ,
//    pvWRE_XIdxBack           ,
    pvWRE_XIdxOut            ,
    MAX_PSTN_MOTR4           ,

    pvWRE_XEpxWait          = 0 ,
    pvWRE_XEpxWorkStt        ,
    pvWRE_XEpxOri            ,
    pvWRE_XEpxClean          ,
    MAX_PSTN_MOTR5           ,

    pvWRE_YEpxWait       = 0 ,
    pvWRE_YEpxWorkStt        ,
    pvWRE_YEpxOri            ,
    pvWRE_YEpxClean          ,
    MAX_PSTN_MOTR6           ,

    pvWRE_ZEpxWait       = 0 ,
//    pvWRE_ZEpxOriStt         ,
    pvWRE_ZEpxWork           ,
    pvWRE_ZEpxWorkAt         ,
    pvWRE_ZEpxCleanWait      ,    
    pvWRE_ZEpxClean          ,
    MAX_PSTN_MOTR7           ,

    pvWRD_XIdxWait       = 0 ,
    pvWRD_XIdxWorkStt        ,
//    pvWRD_XIdxPull           ,
//    pvWRD_XIdxBack           ,
    pvWRD_XIdxOut            ,
    MAX_PSTN_MOTR8           ,

    pvWRD_YInsWait       = 0 ,
    pvWRD_YInsWorkStt        ,
    MAX_PSTN_MOTR9           ,

    //HED
    pvWRD_XHedWait       = 0 ,
    pvWRD_XHedWafer          ,
    pvWRD_XHedStrip          ,
    MAX_PSTN_MOTR10          ,

    pvWRD_YHedWait       = 0 ,
    pvWRD_YHedWafer          ,
    pvWRD_YHedWaferWait      ,  //CAM 찍을수 있는 위치.
    pvWRD_YHedWaferZMove     ,  //Z축 움직일 위치.
    pvWRD_YHedStripStt       ,
    pvWRD_YHedStripWait      ,
    pvWRD_YHedStripZMove     ,

    MAX_PSTN_MOTR11          ,

    pvWRD_ZHedWait       = 0 ,
    pvWRD_ZHedMove           ,     
    pvWRD_ZHedWfWorkWait     ,
    pvWRD_ZHedWfWork         ,
    pvWRD_ZHedStWorkWait     ,
    pvWRD_ZHedStWork         ,
    pvWRD_ZHedWaferCal       ,  //Z축 보정 기준.
    pvWRD_ZHedStripCal       ,  //Z축 보정 기준.
    MAX_PSTN_MOTR12          ,

    pvWRD_THedWait       = 0 ,
//    pvWRD_THedWorkStt        ,
    pvWRD_THedWafer          ,
    pvWRD_THedStrip          ,
    MAX_PSTN_MOTR13          ,

    pvWRD_XStgWait       = 0 ,
    pvWRD_XStgCassette       ,
    pvWRD_XStgAlign          ,
    pvWRD_XStgFindOri        ,
    poWRD_XStgFirstChip      ,
    MAX_PSTN_MOTR14          ,

    pvWRD_YStgWait       = 0 ,
    pvWRD_YStgCassette       ,
    pvWRD_YStgAlign          ,
    pvWRD_YStgFindOri        ,
    poWRD_YStgFirstChip      ,
    MAX_PSTN_MOTR15          ,

    pvWRD_TStgWait       = 0 ,
    pvWRD_TStgCassette       ,
    pvWRD_TStgAlign          ,
    MAX_PSTN_MOTR16          ,

    pvWRD_ZEjtWait       = 0 ,
    pvWRD_ZEjtWorkBefore     ,
    pvWRD_ZEjtWork           ,
    MAX_PSTN_MOTR17          ,


    pvWRD_ZExdWait       = 0 ,
    pvWRD_ZExdGet            ,
    pvWRD_ZExdExpend         ,
    MAX_PSTN_MOTR18          ,

    pvWRD_ZCstWait       = 0 ,
    pvWRD_ZCstWorkStt        ,
    poWRD_ZCstPlaceOfs       ,
    MAX_PSTN_MOTR19          ,

    pvWRD_XTrsWait       = 0 ,
    pvWRD_XTrsPick           ,
    pvWRD_XTrsPickWait       ,
    pvWRD_XTrsPlace          ,
    MAX_PSTN_MOTR20          ,

    pvPSB_XIdxWait       = 0 ,
    pvPSB_XIdxClamp          ,
    pvPSB_XIdxPull           ,
    pvPSB_XIdxBack           ,
    pvPSB_XIdxOut1           ,
    pvPSB_XIdxOut2           ,
    MAX_PSTN_MOTR21          ,

    pvULD_YCmpWait       = 0 ,
//    pvULD_YCmpIn             ,
    pvULD_YCmpWorkStt        ,
    MAX_PSTN_MOTR22          ,

    pvULD_ZCmpWait       = 0 ,
    pvULD_ZCmpIn             ,
    pvULD_ZCmpWorkStt        ,
    pvULD_ZCmpOut            ,
    MAX_PSTN_MOTR23          ,

};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_CycleHome             ,
    mcLDR_CyclePick             ,
    mcLDR_CycleWork             ,
    mcLDR_CyclePush             ,
    mcLDR_CyclePlace            ,
    mcLDR_CyclePitchUp          ,
    mcLDR_CyclePitchDn          ,

    mcPRB_CycleHome             ,
    mcPRB_CycleWorkStt          ,
    mcPRB_CycleWork             ,
    mcPRB_CycleReGrip           ,
    mcPRB_CycleWorkEnd          ,

    mcRAE_CycleHome             ,
    mcRAE_CycleWorkStt          ,
    mcRAE_CycleReGrip           ,
    mcRAE_CycleWork             ,
    mcRAE_CycleWorkEnd          ,

    mcEPX_CycleHome             ,
    mcEPX_CycleClean            ,
    mcEPX_CycleOri              ,
    mcEPX_CycleWork             ,
    mcETC_ToolChange            ,
    mcETC_EpoxyOneShot          ,

    mcRAD_CycleHome             ,
    mcRAD_CycleWorkStt          ,
    mcRAD_CycleReGrip           ,
    mcRAD_CycleWork             ,
    mcRAD_CycleAtVisn           ,
    mcRAD_CycleBfVisn           ,
    mcRAD_CycleWorkEnd          ,

    mcHED_CycleHome             ,
    mcHED_CyclePick             ,
    mcHED_CyclePickNoT          ,
    mcHED_CyclePlace            ,
    mcHED_CycleWfrPlace         ,
    mcHED_CycleWfrCal           ,
    mcHED_CycleStpCal           ,
    mcHED_CycleRalPlace         ,

    mcSTG_CycleHome             ,
    mcSTG_CycleCsSupply         ,
    mcSTG_CycleStgExpend        ,
    mcSTG_CycleStgVision        ,
    mcSTG_CycleStgNextMove      ,
    mcSTG_CycleCsPlace          ,
    mcSTG_CycleCenterMove       ,
    mcSTG_CycleStartSetMc       ,
    mcSTG_CycleStartSetEg       ,
    mcSTG_CycleCenterMoveT      ,
    mcSTG_CycleUnExpend         ,
    mcSTG_CycleStgTAlign        ,

    mcSTG_ClickMove             , //Data Man -> Right Clicked

    mcPSB_CycleHome             ,
    mcPSB_CyclePull             ,
    mcPSB_CycleBackOut          ,

    mcULD_CycleHome             ,
    mcULD_CyclePick             ,
    mcULD_CycleWork             ,
    mcULD_CyclePlace            ,
    mcULD_CyclePitchUp          ,
    mcULD_CyclePitchDn          ,

    mcPickPlaceHome             ,
    mcPickPlaceTest             ,

    mcETC_CycleTest             ,

//    mcETC_MainAirOnOff     = 60 , //

    MAX_MANUAL_CYCLE
};
#endif



