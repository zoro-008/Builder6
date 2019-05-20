#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_REGRIP_ARRAY 10


#define MAX_WAFER_ROW   50
#define MAX_WAFER_COL   50

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_SRL_Num {
    LUS_14_1001  = 0 ,

    MAX_SRL_Num
};

enum EN_ARAY_ID {
    riRAL       = 0  , //Rail ���� Ȯ��
    riRAB            , //Rail Buffer ����.
    riWRK            , //Work Zone ���� Ȯ��


    MAX_ARAY
};

enum EN_MOTR_ID {
    miRAL_XIdx   = 0  , //  ��ũ�� X Index
    miWRK_ZPrb   = 1  , //  ��ũ�� Z Probe
    miSRT_TSrt   = 2  , //  ��ũ�� T Sorting
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiRAL_Stopper  = 0   , // ��ũ�� Stopper
    aiRAL_Index    = 1   , // ��ũ�� Index
    MAX_ACTR
};

enum EN_ERR_ID {
    eiETC_MainPower      =  0   , //
    eiETC_ToStartTO             , //001
    eiETC_ToStopTO              , //002
    eiETC_Emergency             , //003
    eiETC_AllHomeTO             , //004
    eiETC_ManCycleTO            , //005
    eiETC_LotEnd                , //006
    eiETC_MainAir               , //007
    eiFED_CycleTO               , //008
    eiRAL_CycleTO               , //009
    eiWRK_CycleTO               , //010
    eiSRT_CycleTO               , //011
    ei011                       , //012
    eiFED_HomeTO                , //013
    eiRAL_HomeTO                , //014
    eiWRK_HomeTO                , //015
    eiSRT_HomeTO                , //016
    ei016                       , //017
    eiFED_ToStartTO             , //018
    eiRAL_ToStartTO             , //019
    eiWRK_ToStartTO             , //020
    eiSRT_ToStartTO             , //021
    ei021                       , //022
    eiFED_ToStopTO              , //023
    eiRAL_ToStopTO              , //024
    eiWRK_ToStopTO              , //025
    eiSRT_ToStopTO              , //026
    ei027                       , //027
    eiRAL_XIdxHomeEnd           , //028
    eiWRK_ZPrbHomeEnd           , //029
    eiSRT_TSrtHomeEnd           , //030
    ei031                       , //031
    ei032                       , //032
    eiRAL_XIdxNegLim            , //033
    eiWRK_ZPrbNegLim            , //034
    eiSRT_TSrtNegLim            , //035
    ei036                       , //036
    ei037                       , //037
    eiRAL_XIdxPosLim            , //038
    eiWRK_ZPrbPosLim            , //039
    eiSRT_TSrtPosLim            , //040
    ei041                       , //041
    ei042                       , //042
    eiRAL_XIdxAlarm             , //043
    eiWRK_ZPrbAlarm             , //044
    eiSRT_TSrtAlarm             , //045
    ei046                       , //046
    ei047                       , //047
    eiRAL_Index                 , //048
    eiRAL_Stopper               , //049
    ei050                       , //050
    eiSRT_PkgGoodFull           , //051
    ei052                       , //052
    eiSRT_Bin3Full              , //053
    eiSRT_Bin4Full              , //054
    eiSRT_Bin5Full              , //055
    ei056                       , //056
    eiSRT_Bin1NoExst            , //057
    eiSRT_Bin2NoExst            , //058
    eiSRT_Bin3NoExst            , //059
    eiSRT_Bin4NoExst            , //060
    eiSRT_Bin5NoExst            , //061
    ei062                       , //062
    ei063                       , //063
    eiWRK_FailCnt               , //064
    eiWRK_InspTO                , //065
    ei066                       , //066
    ei067                       , //067
    ei068                       , //068
    ei069                       , //069
    ei070                       , //070
    eiWRK_PinChange             , //071
    eiRAL_NoPkg                 , //072
    eiRAL_PkgJam                , //073
    eiWRK_PrbTestTO             , //074
    eiWRK_PrbTestEr             , //075
    ei076                       , //076
    ei077                       , //077
    ei078                       , //078
    ei079                       , //079
    eiWRK_DustChip              , //080
    eiWRK_PkgUnknown            , //081
    eiWRK_PkgDispr              , //082
    eiPTR_PorbeNotReady         , //083
    eiRAL_Heat1UpLmt            , //084
    eiRAL_Heat1Discnt           , //085         
    eiRAL_Heat2UpLmt            , //086         
    eiRAL_Heat2DnLmt            , //087         
    eiRAL_Heat1SoftLmt          , //088         
    eiRAL_Heat2SoftLmt          , //089         
    eiRAL_DustChip              , //090         
    eiWRK_DoubleChip            , //091         
    eiSRT_PkgNotDetected        , //092         
    eiFED_FeedFailed            , //093         
    ei094                       , //094
    ei095                       , //095
    ei096                       , //096

    MAX_ERR
};

enum EN_INPUT_ID {

    x0000 =  0 ,  xETC_StartSw           =  0 , //��ŸƮ ����ġ
    x0001 =  1 ,  xETC_StopSw            =  1 , //��ž ����ġ
    x0002 =  2 ,  xETC_ResetSw           =  2 , //���V ����ġ
    x0003 =  3 ,  xETC_AirSw             =  3 , //���� ����ġ
    x0004 =  4 ,  xETC_InitSw            =  4 , //�̴ϼ� ����ġ
    x0005 =  5 ,  x005                   =  5 , //�Ŀ��� ����ġ
    x0006 =  6 ,  x006                   =  6 , //
    x0007 =  7 ,  x007                   =  7 , //���� �ε��� ��
    x0008 =  8 ,  xRAL_PkgInChk          =  8 , //���� �ε��� �ٿ�
    x0009 =  9 ,  xRAL_PkgDustChk        =  9 , //���� ���� ���� ���� �ѷ����� �ƿ� �Ȱ� ����.
    x000A = 10 ,  xRAL_Pkg               = 10 , //���� Ȯ�� ����.(Work Zone ���� ��.)
    x000B = 11 ,  xWRK_Detect1           = 11 , //���� ����1
    x000C = 12 ,  xWRK_Detect2           = 12 , //���� ����2
    x000D = 13 ,  xWRK_IndexDn           = 13 , //���� ����3
    x000E = 14 ,  xWRK_IndexUp           = 14 , //���� ����4
    x000F = 15 ,  xSRT_PkgDetect         = 15 , //���� ����5

    x0010 = 16 ,  xSRT_Bin1Exist         = 16 , //���� ���� �˶�.
    x0011 = 17 ,  xSRT_Bin2Exist         = 17 , //���� �ܼ� �˶�.
    x0012 = 18 ,  xSRT_Bin3Exist         = 18 , //
    x0013 = 19 ,  xSRT_Bin4Exist         = 19 , //
    x0014 = 20 ,  xSRT_Bin5Exist         = 20 , //�ļ� ��ġ�� ���� ����.
    x0015 = 21 ,  xRAL_Heat1UpLmt        = 21 , //��ũ �� ���� 2�� Ȯ�� ����
    x0016 = 22 ,  xRAL_Heat1Discnct      = 22 , //���� ���� Ȯ�� ����
    x0017 = 23 ,  xRAL_Heat2UpLmt        = 23 , //��ũ �� ���� �ε��� �ļ� ��ġ�� ���� ����.
    x0018 = 24 ,  xRAL_Heat3Discnct      = 24 , //�׽��� �˻� ���� b����
    x0019 = 25 ,  x025                   = 25 , //�׽��� ������    b����
    x001A = 26 ,  x026                   = 26 , //�׽��� ������    b����
    x001B = 27 ,  xTST_End               = 27 , //�׽��� ���      b����
    x001C = 28 ,  xTST_Forward           = 28 , //�׽��� �غ�Ϸ�  b����
    x001D = 29 ,  xTST_Reverse           = 29 , //
    x001E = 30 ,  xTST_Result            = 30 , //
    x001F = 31 ,  x031                   = 31 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {

    y0000 =  0 ,  yETC_StartLp           =  0 , //��ŸƮ ����ġ ����
    y0001 =  1 ,  yETC_StopLp            =  1 , //��ž ����ġ ����
    y0002 =  2 ,  yETC_ResetLp           =  2 , //���V ����ġ ����
    y0003 =  3 ,  yETC_AirLp             =  3 , //���� ����ġ ����
    y0004 =  4 ,  yETC_InitLp            =  4 , //�̴ϼ� ����ġ ����
    y0005 =  5 ,  yETC_MainAir           =  5 , //
    y0006 =  6 ,  yETC_Emg               =  6 , //���μ�
    y0007 =  7 ,  y007                   =  7 , //
    y0008 =  8 ,  yWRK_Stopper           =  8 , //�����ε��� ��
    y0009 =  9 ,  yWRK_Index             =  9 , //���Ϻ�ο��
    y000A = 10 ,  yRAL_AirBlower         = 10 , //
    Z000B = 11 ,  ySRT_AirBlower         = 11 , //
    y000C = 12 ,  yETC_HeatOn1           = 12 , //���Ϳ�
    y000D = 13 ,  yETC_HeatOn2           = 13 , //
    y000E = 14 ,  yETC_FeederOn          = 14 , //
    y000F = 15 ,  y015                   = 15 , //
    y0010 = 16 ,  yETC_TwRedLp           = 16 , //Ÿ������ RED
    y0011 = 17 ,  yETC_TwYelLp           = 17 , //Ÿ������ YELLOW
    y0012 = 18 ,  yETC_TwGrnLp           = 18 , //Ÿ������ GREEN
    y0013 = 19 ,  yETC_TwBzz             = 19 , //Ÿ������ Buzzer
    y0014 = 20 ,  y020                   = 20 , //
    y0015 = 21 ,  y021                   = 21 , // �׽��� ��ŸƮ B����
    y0016 = 22 ,  y022                   = 22 , // �ϴ� ����� ��� ���´�.
    y0017 = 23 ,  y023                   = 23 , // �߰� �ǵ��� ��Ũ���� ��Ƽ� �ΰ��� ���°� ����.
    y0018 = 24 ,  yTST_StartTest         = 24 , //
    y0019 = 25 ,  y025                   = 25 , //
    y001A = 26 ,  y026                   = 26 , //
    y001B = 27 ,  y027                   = 27 , //
    y001C = 28 ,  y028                   = 28 , //
    y001D = 29 ,  y029                   = 29 , //
    y001E = 30 ,  y030                   = 30 , //
    y001F = 31 ,  y031                   = 31 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID { //���α׷����� ������ ���̵�...

    piRAL_XIdxWait       = 0 ,
    piRAL_XIdxWork           ,
    MAX_PSTN_ID_MOTR0        ,

    piWRK_ZPrbWait       = 0 ,
    piWRK_ZPrbWork           ,
    MAX_PSTN_ID_MOTR1        ,

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
    pvRAL_XIdxWork           ,
    MAX_PSTN_MOTR0           ,

    pvWRK_ZPrbWait       = 0 ,
    pvWRK_ZPrbWork           ,
    MAX_PSTN_MOTR1           ,

    pvSRT_TSrtWait       = 0 ,
    pvSRT_TSrtBin1           ,
    pvSRT_TSrtBin2           ,
    pvSRT_TSrtBin3           ,
    pvSRT_TSrtBin4           ,
    pvSRT_TSrtBin5           ,
    MAX_PSTN_MOTR2           ,

};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

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




