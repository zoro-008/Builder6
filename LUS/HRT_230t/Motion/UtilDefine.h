#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_REGRIP_ARRAY 10
#define VOICE_COIL_ID     0

#define MAX_WAFER_ROW   50
#define MAX_WAFER_COL   50

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_ARAY_ID {
    riRAL   = 0, //레일로 피딩된 칩.
    riWRK      , //프로브 작업 하는 칩.
    riRAB      , //레일에서 워크존 가기 전 버퍼구간.
    MAX_ARAY
};

enum EN_MOTR_ID {
    miRAL_XIdx = 0  , // 레일 인덱스 X
    miWRK_ZPrb = 1  , // 워크 프로브 Z
    //miETC_Spr  = 2  , // 스페어..
    miSRT_TSrt = 2  , // 쏘터   T
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiRAL_IdxUpDn  = 0   , // 레일 인덱스 업다운.
    aiFED_StpFwBw  = 1   , // 피드 실린더 FwBw
    MAX_ACTR
};

enum EN_INPUT_ID {

    x0000 =  0 ,  xETC_StartSw            =  0 , //스타트 스위치
    x0001 =  1 ,  xETC_StopSw             =  1 , //스탑 스위치
    x0002 =  2 ,  xETC_ResetSw            =  2 , //리셑 스위치
    x0003 =  3 ,  xETC_AirSw              =  3 , //에어 스위치
    x0004 =  4 ,  xETC_InitSw             =  4 , //이니셜 스위치
    x0005 =  5 ,  xETC_PowerOn            =  5 , //파워온 스위치
    x0006 =  6 ,  x006                    =  6 , //
    x0007 =  7 ,  xRAL_IdxUp              =  7 , //레일 인덱스 업
    x0008 =  8 ,  xRAL_IdxDn              =  8 , //레일 인덱스 다운
    x0009 =  9 ,  xRAL_PkgInChk           =  9 , //왼쪽 자제 감지 센서 롤러에서 아웃 된것 감지.
    x000A = 10 ,  xWRK_Pkg                = 10 , //오른쪽 워크존 자제 감지 센서.
    x000B = 11 ,  xSRT_Bin1Exist          = 11 , //빈통 감지1
    x000C = 12 ,  xSRT_Bin2Exist          = 12 , //빈통 감지2
    x000D = 13 ,  xSRT_Bin3Exist          = 13 , //빈통 감지3
    x000E = 14 ,  xSRT_Bin4Exist          = 14 , //빈통 감지4
    x000F = 15 ,  xSRT_Bin5Exist          = 15 , //빈통 감지5
    x0010 = 16 ,  xRAL_Heat1UpLmt         = 16 , //히터 리밋 알람.
    x0011 = 17 ,  xRAL_Heat1Discnct       = 17 , //히터 단선 알람.
    x0012 = 18 ,  xRAL_Heat2UpLmt         = 18 , //
    x0013 = 19 ,  xRAL_Heat2DnLmt         = 19 , //
    x0014 = 20 ,  xRAL_Pkg                = 20 , //레일 자재 확인센서.
    x0015 = 21 ,  xRAL_PkgDustChk         = 21 , //파손 위치에 자재 센서.
    x0016 = 22 ,  xSRT_PkgDetect          = 22 , //자재 배출 확인 센서
    x0017 = 23 ,  xWRK_PkgDustChk         = 23 , //워크 존 전에 인덱스 파손 위치에 자재 센서.
    x0018 = 24 ,  xTST_EndTest            = 24 , //테스터 검사 엔드 b접점
    x0019 = 25 ,  xTST_Forward            = 25 , //테스터 정방향    b접점
    x001A = 26 ,  xTST_Backward           = 26 , //테스터 역방향    b접점
    x001B = 27 ,  xTST_Result             = 27 , //테스터 결과      b접점
    x001C = 28 ,  xTST_Ready              = 28 , //테스터 준비완료  b접점
    x001D = 29 ,  x01D                    = 29 , //
    x001E = 30 ,  x01E                    = 30 , //
    x001F = 31 ,  x01F                    = 31 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {

    y0000 =  0 ,  yETC_StartLp            =  0 , //스타트 스위치 램프
    y0001 =  1 ,  yETC_StopLp             =  1 , //스탑 스위치 램프
    y0002 =  2 ,  yETC_ResetLp            =  2 , //리셑 스위치 램프
    y0003 =  3 ,  yETC_AirLp              =  3 , //에어 스위치 램프
    y0004 =  4 ,  yETC_InitLp             =  4 , //이니셜 스위치 램프
    y0005 =  5 ,  y005                    =  5 , //
    y0006 =  6 ,  yETC_MainSol            =  6 , //메인솔
    y0007 =  7 ,  y007                    =  7 , //
    y0008 =  8 ,  yRAL_IdxUp              =  8 , //레일인덱스 업
    y0009 =  9 ,  yRAL_Blower             =  9 , //레일브로우어
    y000A = 10 ,  y00A                    = 10 , //
    y000B = 11 ,  y00B                    = 11 , //
    y000C = 12 ,  yETC_HeatOn             = 12 , //히터온
    y000D = 13 ,  y00D                    = 13 , //
    y000E = 14 ,  y00E                    = 14 , //
    y000F = 15 ,  y00F                    = 15 , //

    y0010 = 16 ,  yETC_TwRedLp            = 16 , //타워램프 RED
    y0011 = 17 ,  yETC_TwYelLp            = 17 , //타워램프 YELLOW
    y0012 = 18 ,  yETC_TwGrnLp            = 18 , //타워램프 GREEN
    y0013 = 19 ,  yETC_TwBzz              = 19 , //타워램프 Buzzer
    y0014 = 20 ,  yTST_StartTest          = 20 , //
    y0015 = 21 ,  y015                    = 21 , // 테스터 스타트 B접점
    y0016 = 22 ,  yFED_Stopper            = 22 , // 일단 여기로 잡아 놓는다.
    y0017 = 23 ,  yFED_Vacuum             = 23 , // 추가 피딩전 베큠으로 잡아서 두개씩 들어가는것 방지.
    y0018 = 24 ,  y018                    = 24 , //
    y0019 = 25 ,  y019                    = 25 , //
    y001A = 26 ,  y01A                    = 26 , //
    y001B = 27 ,  y01B                    = 27 , //
    y001C = 28 ,  y01C                    = 28 , //
    y001D = 29 ,  y01D                    = 29 , //
    y001E = 30 ,  y01E                    = 30 , //
    y001F = 31 ,  y01F                    = 31 , //

    MAX_OUTPUT
};

enum EN_ERR_ID {
/*000*/eiETC_MainPower      =  0   ,//메인 파워
/*001*/eiETC_MainAir               ,//메인 공압
/*002*/eiETC_ToStartTO             ,//스타트 시간 초과
/*003*/eiETC_ToStopTO              ,//스탑 시간 초과
/*004*/eiETC_LotEnd                ,//랏 엔드
/*005*/eiETC_AllHomeTO             ,//올 홈 타임아웃
/*006*/eiETC_ManCycleTO            ,//메뉴얼 사이클 타임 아웃
/*007*/ei007                       ,
/*008*/ei008                       ,
/*009*/eiFED_CycleTO               ,//피더 싸이클 타임아웃
/*010*/eiRAL_CycleTO               ,//레일 싸이클 타임아웃
/*011*/eiWRK_CycleTO               ,//워크 싸이클 타임아웃
/*012*/eiSRT_CycleTO               ,//쏘터 싸이클 타임아웃
/*013*/eiFED_HomeTO                ,//피더 홈 싸이클 타임아웃
/*014*/eiRAL_HomeTO                ,//레일 홈 싸이클 타임아웃
/*015*/eiWRK_HomeTO                ,//워크 홈 싸이클 타임아웃
/*016*/eiSRT_HomeTO                ,//쏘터 홈 싸이클 타임아웃
/*017*/eiFED_ToStartTO             ,//피더 스타트 싸이클 타임아웃
/*018*/eiRAL_ToStartTO             ,//레일 스타트 싸이클 타임아웃
/*019*/eiWRK_ToStartTO             ,//워크 스타트 싸이클 타임아웃
/*020*/eiSRT_ToStartTO             ,//쏘터 스타트 싸이클 타임아웃
/*021*/eiFED_ToStopTO              ,//피더 스탑 싸이클 타임아웃
/*022*/eiRAL_ToStopTO              ,//레일 스탑 싸이클 타임아웃
/*023*/eiWRK_ToStopTO              ,//워크 스탑 싸이클 타임아웃
/*024*/eiSRT_ToStopTO              ,//쏘터 스탑 싸이클 타임아웃
/*025*/eiFED_TFedHomeEnd           ,//피더 홈 엔드 되지 않음.
/*026*/eiRAL_XIdxHomeEnd           ,//레일 홈 엔드 되지 않음.
/*027*/eiWRK_ZPrbHomeEnd           ,//워크 홈 엔드 되지 않음.
/*028*/eiSRT_TSrtHomeEnd           ,//쏘터 홈 엔드 되지 않음.
/*029*/ei029                       ,
/*030*/eiFED_TFedNegLim            ,//피딩 T -리밋 센서 감지.
/*031*/eiRAL_XIdxNegLim            ,//레일 인덱스 X -리밋 센서 감지.
/*032*/eiWRK_ZPrbNegLim            ,//워크 프로브 Z -센서 감지.
/*033*/eiSRT_TSrtNegLim            ,//소터 T -리밋 센서 감지.
/*034*/ei034                       ,
/*035*/eiFED_TFedPosLim            ,//피딩 T +리밋 센서 감지.
/*036*/eiRAL_XIdxPosLim            ,//레일 인덱스 X +리밋 센서 감지.
/*037*/eiWRK_ZPrbPosLim            ,//워크 프로브 Z +센서 감지.
/*038*/eiSRT_TSrtPosLim            ,//소터 T +리밋 센서 감지.
/*039*/ei039                       ,
/*040*/eiFED_TFedAlarm             ,//피딩 T        알람
/*041*/eiRAL_XCmpAlarm             ,//레일 인덱스 X 알람
/*042*/eiWRK_ZCmpAlarm             ,//워크 프로브 Z 알람
/*043*/eiSRT_TIdxAlarm             ,//소터 T        알람
/*044*/ei044                       ,
/*045*/ei045                       ,
/*046*/eiRAL_IdxUpDnTo             ,//레일 인덱스 업다운 타임아웃
/*047*/ei047                       ,
/*048*/ei048                       ,
/*049*/eiSRT_Bin1Full              ,//빈1번 통 만재.
/*050*/eiSRT_Bin2Full              ,//빈2번 통 만재.
/*051*/eiSRT_Bin3Full              ,//빈3번 통 만재.
/*052*/eiSRT_Bin4Full              ,//빈4번 통 만재.
/*053*/eiSRT_Bin5Full              ,//빈5번 통 만재.
/*054*/ei055                       ,
/*055*/eiSRT_Bin1NoExst            ,//빈1번 통 감지 안됌.
/*056*/eiSRT_Bin2NoExst            ,//빈2번 통 감지 안됌.
/*057*/eiSRT_Bin3NoExst            ,//빈3번 통 감지 안됌.
/*058*/eiSRT_Bin4NoExst            ,//빈4번 통 감지 안됌.
/*059*/eiSRT_Bin5NoExst            ,//빈5번 통 감지 안됌.
/*060*/ei060                       ,
/*061*/ei061                       ,
/*062*/eiWRK_FailCnt               ,//워크 프로브 연속 페일 에러.
/*063*/eiWRK_InspTO                ,//워크 프로브 동작 타임아웃.
/*064*/ei064                       ,
/*065*/ei065                       , //IO히터 리밋 온도 알람
/*066*/ei066                       , //IO히터 단선 알람.
/*067*/ei067                       ,
/*068*/ei068                       ,
/*069*/eiWRK_PinChange             , //핀 교체  에러.
/*070*/eiRAL_NoPkg                 , //Pkg공급 안됌.
/*071*/eiRAL_PkgJam                , //PKG 피더랑 인덱스 사이에 끼임.
/*072*/eiWRK_PrbTestTO             , //프로브 테스트 타임 아웃.
/*073*/eiWRK_PrbTestEr             , //테스터 에러.
/*074*/ei074                       , //
/*075*/ei075                       , //
/*076*/ei076                       , //
/*077*/eiRAL_PkgDispr              , //
/*078*/eiWRK_DustChip              , //파손 위치 자재 있음 알람.(워크존 가기 전)
/*079*/eiWRK_PkgUnknown            , //프로브 존에 알수 없는 PKG
/*080*/eiWRK_PkgDispr              , //프로브 존에 PKG 사라짐.
/*081*/eiPTR_PorbeNotReady         , //프로브 Not Ready
/*082*/eiRAL_Heat1UpLmt            , //IO히터 리밋 온도 알람
/*083*/eiRAL_Heat1Discnt           , //IO히터 단선 알람.
/*084*/eiRAL_Heat2UpLmt            , //IO히터 리밋 온도 알람
/*085*/eiRAL_Heat2DnLmt            , //IO히터 리밋 온도 알람
/*086*/eiRAL_Heat1SoftLmt          , //소프트웨어 플러스 마이너스 리밋 알람.
/*087*/eiRAL_Heat2SoftLmt          , //소프트웨어 플러스 마이너스 리밋 알람.
/*088*/eiRAL_DustChip              , //파손 위치에 자재 있음 알람.
/*089*/eiWRK_DoubleChip            , //워크 위치에 자재 2개 들어감 알람.
/*090*/eiSRT_PkgNotDetected        , //
/*091*/eiFED_FeedFailed            ,

       MAX_ERR
};

enum EN_PSTN_ID { //프로그램에서 가야할 아이디...
    piRAL_XIdxWait       = 0 ,
    piRAL_XIdxOut            ,
    MAX_PSTN_ID_MOTR0        ,

    piWRK_ZPrbWait       = 0 ,
    piWRK_ZPrbWork           ,
    MAX_PSTN_ID_MOTR1        ,

    //piETC_Spr            = 0 ,
    //MAX_PSTN_ID_MOTR2        ,

    piSRT_TSrtWait       = 0 ,
    piSRT_TSrtBin1           ,
    piSRT_TSrtBin2           ,
    piSRT_TSrtBin3           ,
    piSRT_TSrtBin4           ,
    piSRT_TSrtBin5           ,                      
    MAX_PSTN_ID_MOTR2        ,
};

enum EN_PSTN_VALUE {
    pvRAL_XIdxWait       = 0 ,
    pvRAL_XIdxOut            ,
    MAX_PSTN_MOTR0           ,

    pvWRK_ZPrbWait       = 0 ,
    pvWRK_ZPrbWork           ,
    MAX_PSTN_MOTR1           ,

    //pvETC_Spr            = 0 ,
    //MAX_PSTN_MOTR2           ,

    pvSRT_TSrtWait       = 0 ,
    pvSRT_TSrtBin1           ,
    poSRT_TSrtPitch          ,
    MAX_PSTN_MOTR2           ,
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcFED_Home                  ,
    mcFED_CycleFeed             ,

    mcRAL_Home                  ,
    mcRAL_CycleWorkIdx          ,

    mcWRK_Home                  ,
    mcWRK_CycleWork             ,

    mcSRT_Home                  ,
    mcSRT_Bin1                  ,
    mcSRT_Bin2                  ,
    mcSRT_Bin3                  ,
    mcSRT_Bin4                  ,
    mcSRT_Bin5                  ,

    mcAllCycleOne               ,
    mcMoveBin3                  ,
    mcMoveBin4                  ,
    mcMoveBin5                  ,

    mcRalEndBlow                ,

    MAX_MANUAL_CYCLE
};
#endif










































































//073
//074
//075
//076
//077
//078
//079
//080
//081
//082
//083
//084
//085
//086
//087
//088
//089
//090
//091
//092
//093
//094
//095
//096
//097
//098
//099
//100
//101
//102
//103
//104
//105
//106
//107
//108
//109
//110
//111
//112
//113
//114
//115
//116
//117
//118
//119
//120
//121
//122
//123
//124
//125
//126
//127
//128
//129
//130
//131
//132
//133
//134
//135
//136
//137
//138
//139
//140
//141
//142
//143
//144
//145
//146
//147
//148
//149
//150
//151
//152
//153
//154
//155
//156
//157
//158
//159
//160
//161
//162
//163
//164
//165
//166
//167
//168
//169
//170
//171
//172
//173
//174
//175
//176
//177
//178
//179
//180
//181
//182
//183
//184
//185
//186
//187
//188
//189
//190
//191
//192
//193
//194
//195
//196
//197
//198
//199
//200
//201
//202
//203
//204
//205
//206
//207
//208
//209
//210
//211
//212
//213
//214
//215
//216
//217
//218
//219
//220
//221
//222
//223
//224
//225
//226
//227
//228
//229
//230
//231
//232
//233
//234
//235
//236
//237
//238
//239
//240
//241
//242
//243
//244
//245
//246
//247
//248
//249
//250
//251
//252
//253
//254
//255
//256
//257
//258
//259
//260
//261
//262
