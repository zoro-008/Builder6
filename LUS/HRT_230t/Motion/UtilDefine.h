#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_REGRIP_ARRAY 10
#define VOICE_COIL_ID     0

#define MAX_WAFER_ROW   50
#define MAX_WAFER_COL   50

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_ARAY_ID {
    riRAL   = 0, //���Ϸ� �ǵ��� Ĩ.
    riWRK      , //���κ� �۾� �ϴ� Ĩ.
    riRAB      , //���Ͽ��� ��ũ�� ���� �� ���۱���.
    MAX_ARAY
};

enum EN_MOTR_ID {
    miRAL_XIdx = 0  , // ���� �ε��� X
    miWRK_ZPrb = 1  , // ��ũ ���κ� Z
    //miETC_Spr  = 2  , // �����..
    miSRT_TSrt = 2  , // ����   T
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiRAL_IdxUpDn  = 0   , // ���� �ε��� ���ٿ�.
    aiFED_StpFwBw  = 1   , // �ǵ� �Ǹ��� FwBw
    MAX_ACTR
};

enum EN_INPUT_ID {

    x0000 =  0 ,  xETC_StartSw            =  0 , //��ŸƮ ����ġ
    x0001 =  1 ,  xETC_StopSw             =  1 , //��ž ����ġ
    x0002 =  2 ,  xETC_ResetSw            =  2 , //���V ����ġ
    x0003 =  3 ,  xETC_AirSw              =  3 , //���� ����ġ
    x0004 =  4 ,  xETC_InitSw             =  4 , //�̴ϼ� ����ġ
    x0005 =  5 ,  xETC_PowerOn            =  5 , //�Ŀ��� ����ġ
    x0006 =  6 ,  x006                    =  6 , //
    x0007 =  7 ,  xRAL_IdxUp              =  7 , //���� �ε��� ��
    x0008 =  8 ,  xRAL_IdxDn              =  8 , //���� �ε��� �ٿ�
    x0009 =  9 ,  xRAL_PkgInChk           =  9 , //���� ���� ���� ���� �ѷ����� �ƿ� �Ȱ� ����.
    x000A = 10 ,  xWRK_Pkg                = 10 , //������ ��ũ�� ���� ���� ����.
    x000B = 11 ,  xSRT_Bin1Exist          = 11 , //���� ����1
    x000C = 12 ,  xSRT_Bin2Exist          = 12 , //���� ����2
    x000D = 13 ,  xSRT_Bin3Exist          = 13 , //���� ����3
    x000E = 14 ,  xSRT_Bin4Exist          = 14 , //���� ����4
    x000F = 15 ,  xSRT_Bin5Exist          = 15 , //���� ����5
    x0010 = 16 ,  xRAL_Heat1UpLmt         = 16 , //���� ���� �˶�.
    x0011 = 17 ,  xRAL_Heat1Discnct       = 17 , //���� �ܼ� �˶�.
    x0012 = 18 ,  xRAL_Heat2UpLmt         = 18 , //
    x0013 = 19 ,  xRAL_Heat2DnLmt         = 19 , //
    x0014 = 20 ,  xRAL_Pkg                = 20 , //���� ���� Ȯ�μ���.
    x0015 = 21 ,  xRAL_PkgDustChk         = 21 , //�ļ� ��ġ�� ���� ����.
    x0016 = 22 ,  xSRT_PkgDetect          = 22 , //���� ���� Ȯ�� ����
    x0017 = 23 ,  xWRK_PkgDustChk         = 23 , //��ũ �� ���� �ε��� �ļ� ��ġ�� ���� ����.
    x0018 = 24 ,  xTST_EndTest            = 24 , //�׽��� �˻� ���� b����
    x0019 = 25 ,  xTST_Forward            = 25 , //�׽��� ������    b����
    x001A = 26 ,  xTST_Backward           = 26 , //�׽��� ������    b����
    x001B = 27 ,  xTST_Result             = 27 , //�׽��� ���      b����
    x001C = 28 ,  xTST_Ready              = 28 , //�׽��� �غ�Ϸ�  b����
    x001D = 29 ,  x01D                    = 29 , //
    x001E = 30 ,  x01E                    = 30 , //
    x001F = 31 ,  x01F                    = 31 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {

    y0000 =  0 ,  yETC_StartLp            =  0 , //��ŸƮ ����ġ ����
    y0001 =  1 ,  yETC_StopLp             =  1 , //��ž ����ġ ����
    y0002 =  2 ,  yETC_ResetLp            =  2 , //���V ����ġ ����
    y0003 =  3 ,  yETC_AirLp              =  3 , //���� ����ġ ����
    y0004 =  4 ,  yETC_InitLp             =  4 , //�̴ϼ� ����ġ ����
    y0005 =  5 ,  y005                    =  5 , //
    y0006 =  6 ,  yETC_MainSol            =  6 , //���μ�
    y0007 =  7 ,  y007                    =  7 , //
    y0008 =  8 ,  yRAL_IdxUp              =  8 , //�����ε��� ��
    y0009 =  9 ,  yRAL_Blower             =  9 , //���Ϻ�ο��
    y000A = 10 ,  y00A                    = 10 , //
    y000B = 11 ,  y00B                    = 11 , //
    y000C = 12 ,  yETC_HeatOn             = 12 , //���Ϳ�
    y000D = 13 ,  y00D                    = 13 , //
    y000E = 14 ,  y00E                    = 14 , //
    y000F = 15 ,  y00F                    = 15 , //

    y0010 = 16 ,  yETC_TwRedLp            = 16 , //Ÿ������ RED
    y0011 = 17 ,  yETC_TwYelLp            = 17 , //Ÿ������ YELLOW
    y0012 = 18 ,  yETC_TwGrnLp            = 18 , //Ÿ������ GREEN
    y0013 = 19 ,  yETC_TwBzz              = 19 , //Ÿ������ Buzzer
    y0014 = 20 ,  yTST_StartTest          = 20 , //
    y0015 = 21 ,  y015                    = 21 , // �׽��� ��ŸƮ B����
    y0016 = 22 ,  yFED_Stopper            = 22 , // �ϴ� ����� ��� ���´�.
    y0017 = 23 ,  yFED_Vacuum             = 23 , // �߰� �ǵ��� ��Ũ���� ��Ƽ� �ΰ��� ���°� ����.
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
/*000*/eiETC_MainPower      =  0   ,//���� �Ŀ�
/*001*/eiETC_MainAir               ,//���� ����
/*002*/eiETC_ToStartTO             ,//��ŸƮ �ð� �ʰ�
/*003*/eiETC_ToStopTO              ,//��ž �ð� �ʰ�
/*004*/eiETC_LotEnd                ,//�� ����
/*005*/eiETC_AllHomeTO             ,//�� Ȩ Ÿ�Ӿƿ�
/*006*/eiETC_ManCycleTO            ,//�޴��� ����Ŭ Ÿ�� �ƿ�
/*007*/ei007                       ,
/*008*/ei008                       ,
/*009*/eiFED_CycleTO               ,//�Ǵ� ����Ŭ Ÿ�Ӿƿ�
/*010*/eiRAL_CycleTO               ,//���� ����Ŭ Ÿ�Ӿƿ�
/*011*/eiWRK_CycleTO               ,//��ũ ����Ŭ Ÿ�Ӿƿ�
/*012*/eiSRT_CycleTO               ,//���� ����Ŭ Ÿ�Ӿƿ�
/*013*/eiFED_HomeTO                ,//�Ǵ� Ȩ ����Ŭ Ÿ�Ӿƿ�
/*014*/eiRAL_HomeTO                ,//���� Ȩ ����Ŭ Ÿ�Ӿƿ�
/*015*/eiWRK_HomeTO                ,//��ũ Ȩ ����Ŭ Ÿ�Ӿƿ�
/*016*/eiSRT_HomeTO                ,//���� Ȩ ����Ŭ Ÿ�Ӿƿ�
/*017*/eiFED_ToStartTO             ,//�Ǵ� ��ŸƮ ����Ŭ Ÿ�Ӿƿ�
/*018*/eiRAL_ToStartTO             ,//���� ��ŸƮ ����Ŭ Ÿ�Ӿƿ�
/*019*/eiWRK_ToStartTO             ,//��ũ ��ŸƮ ����Ŭ Ÿ�Ӿƿ�
/*020*/eiSRT_ToStartTO             ,//���� ��ŸƮ ����Ŭ Ÿ�Ӿƿ�
/*021*/eiFED_ToStopTO              ,//�Ǵ� ��ž ����Ŭ Ÿ�Ӿƿ�
/*022*/eiRAL_ToStopTO              ,//���� ��ž ����Ŭ Ÿ�Ӿƿ�
/*023*/eiWRK_ToStopTO              ,//��ũ ��ž ����Ŭ Ÿ�Ӿƿ�
/*024*/eiSRT_ToStopTO              ,//���� ��ž ����Ŭ Ÿ�Ӿƿ�
/*025*/eiFED_TFedHomeEnd           ,//�Ǵ� Ȩ ���� ���� ����.
/*026*/eiRAL_XIdxHomeEnd           ,//���� Ȩ ���� ���� ����.
/*027*/eiWRK_ZPrbHomeEnd           ,//��ũ Ȩ ���� ���� ����.
/*028*/eiSRT_TSrtHomeEnd           ,//���� Ȩ ���� ���� ����.
/*029*/ei029                       ,
/*030*/eiFED_TFedNegLim            ,//�ǵ� T -���� ���� ����.
/*031*/eiRAL_XIdxNegLim            ,//���� �ε��� X -���� ���� ����.
/*032*/eiWRK_ZPrbNegLim            ,//��ũ ���κ� Z -���� ����.
/*033*/eiSRT_TSrtNegLim            ,//���� T -���� ���� ����.
/*034*/ei034                       ,
/*035*/eiFED_TFedPosLim            ,//�ǵ� T +���� ���� ����.
/*036*/eiRAL_XIdxPosLim            ,//���� �ε��� X +���� ���� ����.
/*037*/eiWRK_ZPrbPosLim            ,//��ũ ���κ� Z +���� ����.
/*038*/eiSRT_TSrtPosLim            ,//���� T +���� ���� ����.
/*039*/ei039                       ,
/*040*/eiFED_TFedAlarm             ,//�ǵ� T        �˶�
/*041*/eiRAL_XCmpAlarm             ,//���� �ε��� X �˶�
/*042*/eiWRK_ZCmpAlarm             ,//��ũ ���κ� Z �˶�
/*043*/eiSRT_TIdxAlarm             ,//���� T        �˶�
/*044*/ei044                       ,
/*045*/ei045                       ,
/*046*/eiRAL_IdxUpDnTo             ,//���� �ε��� ���ٿ� Ÿ�Ӿƿ�
/*047*/ei047                       ,
/*048*/ei048                       ,
/*049*/eiSRT_Bin1Full              ,//��1�� �� ����.
/*050*/eiSRT_Bin2Full              ,//��2�� �� ����.
/*051*/eiSRT_Bin3Full              ,//��3�� �� ����.
/*052*/eiSRT_Bin4Full              ,//��4�� �� ����.
/*053*/eiSRT_Bin5Full              ,//��5�� �� ����.
/*054*/ei055                       ,
/*055*/eiSRT_Bin1NoExst            ,//��1�� �� ���� �ȉ�.
/*056*/eiSRT_Bin2NoExst            ,//��2�� �� ���� �ȉ�.
/*057*/eiSRT_Bin3NoExst            ,//��3�� �� ���� �ȉ�.
/*058*/eiSRT_Bin4NoExst            ,//��4�� �� ���� �ȉ�.
/*059*/eiSRT_Bin5NoExst            ,//��5�� �� ���� �ȉ�.
/*060*/ei060                       ,
/*061*/ei061                       ,
/*062*/eiWRK_FailCnt               ,//��ũ ���κ� ���� ���� ����.
/*063*/eiWRK_InspTO                ,//��ũ ���κ� ���� Ÿ�Ӿƿ�.
/*064*/ei064                       ,
/*065*/ei065                       , //IO���� ���� �µ� �˶�
/*066*/ei066                       , //IO���� �ܼ� �˶�.
/*067*/ei067                       ,
/*068*/ei068                       ,
/*069*/eiWRK_PinChange             , //�� ��ü  ����.
/*070*/eiRAL_NoPkg                 , //Pkg���� �ȉ�.
/*071*/eiRAL_PkgJam                , //PKG �Ǵ��� �ε��� ���̿� ����.
/*072*/eiWRK_PrbTestTO             , //���κ� �׽�Ʈ Ÿ�� �ƿ�.
/*073*/eiWRK_PrbTestEr             , //�׽��� ����.
/*074*/ei074                       , //
/*075*/ei075                       , //
/*076*/ei076                       , //
/*077*/eiRAL_PkgDispr              , //
/*078*/eiWRK_DustChip              , //�ļ� ��ġ ���� ���� �˶�.(��ũ�� ���� ��)
/*079*/eiWRK_PkgUnknown            , //���κ� ���� �˼� ���� PKG
/*080*/eiWRK_PkgDispr              , //���κ� ���� PKG �����.
/*081*/eiPTR_PorbeNotReady         , //���κ� Not Ready
/*082*/eiRAL_Heat1UpLmt            , //IO���� ���� �µ� �˶�
/*083*/eiRAL_Heat1Discnt           , //IO���� �ܼ� �˶�.
/*084*/eiRAL_Heat2UpLmt            , //IO���� ���� �µ� �˶�
/*085*/eiRAL_Heat2DnLmt            , //IO���� ���� �µ� �˶�
/*086*/eiRAL_Heat1SoftLmt          , //����Ʈ���� �÷��� ���̳ʽ� ���� �˶�.
/*087*/eiRAL_Heat2SoftLmt          , //����Ʈ���� �÷��� ���̳ʽ� ���� �˶�.
/*088*/eiRAL_DustChip              , //�ļ� ��ġ�� ���� ���� �˶�.
/*089*/eiWRK_DoubleChip            , //��ũ ��ġ�� ���� 2�� �� �˶�.
/*090*/eiSRT_PkgNotDetected        , //
/*091*/eiFED_FeedFailed            ,

       MAX_ERR
};

enum EN_PSTN_ID { //���α׷����� ������ ���̵�...
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
