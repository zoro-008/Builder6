#ifndef UTILDEFINEH
#define UTILDEFINEH



enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_ARAY_ID {


    MAX_ARAY
};

enum EN_MOTR_ID {
    miWRK_VBrd1    = 0  , //  보드 1번 V축 Y축 회전
    miWRK_VBrd2    = 1  , //  보드 2번 V축 Y축 회전
    miWRK_UBrd1    = 2  , //  보드 1번 U축 X축 회전
    miWRK_UBrd2    = 3  , //  보드 2번 U축 X축 회전
    miSTG_TBase    = 4  , //  스테이지 베이스 T축

    MAX_MOTR
};


enum EN_ACTR_ID {

    MAX_ACTR
};


enum EN_ERR_ID {
    eiETC_MainPwr           =  0 , //000
    eiETC_MainAir                , //001
    eiETC_ToStartTO              , //002
    eiETC_ToStopTO               , //003
    eiETC_EmergencyL             , //004
    eiETC_EmergencyR             , //005
    ei006                        , //006
    eiETC_AllHomeTO              , //007
    eiETC_ManCycleTO             , //008
    eiSTG_CycleTO                , //009
    eiSTG_ToStartTO              , //010
    eiSTG_ToStopTO               , //011
    ei012                        , //012
    eiSTG_VBrd1HomeEnd           , //013
    eiSTG_VBrd2HomeEnd           , //014
    eiSTG_UBrd1HomeEnd           , //015
    eiSTG_UBrd2HomeEnd           , //016
    eiSTG_TBaseHomeEnd           , //017
    eiSTG_VBrd1NegLim            , //018
    eiSTG_VBrd2NegLim            , //019
    eiSTG_UBrd1NegLim            , //020
    eiSTG_UBrd2NegLim            , //021
    eiSTG_TBaseNegLim            , //022
    eiSTG_VBrd1PosLim            , //023
    eiSTG_VBrd2PosLim            , //024
    eiSTG_UBrd1PosLim            , //025
    eiSTG_UBrd2PosLim            , //026
    eiSTG_TBasePosLim            , //027
    eiSTG_VBrd1Alarm             , //028
    eiSTG_VBrd2Alarm             , //029
    eiSTG_UBrd1Alarm             , //030
    eiSTG_UBrd2Alarm             , //031
    eiSTG_TBaseAlarm             , //032
    eiSTG_TiltDetect             , //033
    eiWRK_Brd1VccL               , //034
    eiWRK_Brd1VccR               , //035
    eiWRK_Brd2VccL               , //036
    eiWRK_Brd2VccR               , //037
    eiWRK_NotBrdPstn             , //038
    eiSTG_WrkPstDetect           , //039
    eiETC_AreaSensor             , //040
    eiSTG_LaserBlock             , //041
    eiSTG_LaserErrCode           , //042
    eiSTG_Laser                  , //043
    eiSTG_CheckSumFail           , //044
    eiETC_Door                   , //045
    ei046                        , //046
    ei047                        , //047
    ei048                        , //048
    ei049                        , //049




    MAX_ERR
};


enum EN_INPUT_ID {
//    xHED_VoiceCoilSevo ,


    x0000 =  0 ,  xETC_StartSwL     =  0 , //
    x0001 =  1 ,  xETC_StartSwR     =  1 , //
    x0002 =  2 ,  xETC_EmgSwL       =  2 , //
    x0003 =  3 ,  xETC_EmgSwR       =  3 , //
    x0004 =  4 ,  xETC_Door1        =  4 , //
    x0005 =  5 ,  xETC_Door2        =  5 , //
    x0006 =  6 ,  xSTG_WrkPstDetect =  6 , // 스테이지 워크 보드 감지 센서
    x0007 =  7 ,  xETC_AreaSensor   =  7 , // 베이스 T 동작 전에 동작 중에 감지 되면 정지 시키기 위한 센서
    x0008 =  8 ,  xWRK_Brd1VccL     =  8 , //
    x0009 =  9 ,  xWRK_Brd1VccR     =  9 , //
    x000A = 10 ,  xWRK_Brd2VccL     = 10 , //
    x000B = 11 ,  xWRK_Brd2VccR     = 11 , //
    x000C = 12 ,  xSTG_TltDetect    = 12 , //
    x000D = 13 ,  x013              = 13 , //
    x000E = 14 ,  x014              = 14 , //
    x000F = 15 ,  x015              = 15 , //



    MAX_INPUT
};

enum EN_OUTPUT_ID  {

    y0000 =  0 ,  yETC_TwRedLp      =  0 , //
    y0001 =  1 ,  yETC_TwYelLp      =  1 , //
    y0002 =  2 ,  yETC_TwGrnLp      =  2 , //
    y0003 =  3 ,  yETC_TwBzz        =  3 , //
    y0004 =  4 ,  yWRK_Brd1VccL     =  4 , //
    y0005 =  5 ,  yWRK_Brd1VccR     =  5 , //
    y0006 =  6 ,  yWRK_Brd2VccL     =  6 , //
    y0007 =  7 ,  yWRK_Brd2VccR     =  7 , //
    y0008 =  8 ,  yETC_LaserWrkVcc  =  8 , // 집진기...
    y0009 =  9 ,  y009              =  9 , //
    y000A = 10 ,  y010              = 10 , //
    y000B = 11 ,  y011              = 11 , //
    y000C = 12 ,  y012              = 12 , //
    y000D = 13 ,  y013              = 13 , //
    y000E = 14 ,  y014              = 14 , //
    y000F = 15 ,  y015              = 15 , //


    MAX_OUTPUT
};

enum EN_PSTN_ID { //프로그램에서 가야할 아이디...

    piWRK_VBrd1_Wait   = 0 ,
    piWRK_VBrd1_Wrk1       ,
    piWRK_VBrd1_Wrk2       ,
    piWRK_VBrd1_Wrk3       ,
    piWRK_VBrd1_Wrk4       ,
    piWRK_VBrd1_Wrk5       ,
    piWRK_VBrd1_Wrk6       ,
    piWRK_VBrd1_Wrk7       ,
    piWRK_VBrd1_Wrk8       ,
    piWRK_VBrd1_Wrk9       ,
    piWRK_VBrd1_Wrk10      ,
    MAX_PSTN_ID_MOTR0      ,

    piWRK_VBrd2_Wait   = 0 ,
    piWRK_VBrd2_Wrk1       ,
    piWRK_VBrd2_Wrk2       ,
    piWRK_VBrd2_Wrk3       ,
    piWRK_VBrd2_Wrk4       ,
    piWRK_VBrd2_Wrk5       ,
    piWRK_VBrd2_Wrk6       ,
    piWRK_VBrd2_Wrk7       ,
    piWRK_VBrd2_Wrk8       ,
    piWRK_VBrd2_Wrk9       ,
    piWRK_VBrd2_Wrk10      ,
    MAX_PSTN_ID_MOTR1      ,

    piWRK_UBrd1_Wait   = 0 ,
    piWRK_UBrd1_Wrk1       ,
    piWRK_UBrd1_Wrk2       ,
    piWRK_UBrd1_Wrk3       ,
    piWRK_UBrd1_Wrk4       ,
    piWRK_UBrd1_Wrk5       ,
    piWRK_UBrd1_Wrk6       ,
    piWRK_UBrd1_Wrk7       ,
    piWRK_UBrd1_Wrk8       ,
    piWRK_UBrd1_Wrk9       ,
    piWRK_UBrd1_Wrk10      ,
    MAX_PSTN_ID_MOTR2      ,

    piWRK_UBrd2_Wait   = 0 ,
    piWRK_UBrd2_Wrk1       ,
    piWRK_UBrd2_Wrk2       ,
    piWRK_UBrd2_Wrk3       ,
    piWRK_UBrd2_Wrk4       ,
    piWRK_UBrd2_Wrk5       ,
    piWRK_UBrd2_Wrk6       ,
    piWRK_UBrd2_Wrk7       ,
    piWRK_UBrd2_Wrk8       ,
    piWRK_UBrd2_Wrk9       ,
    piWRK_UBrd2_Wrk10      ,
    MAX_PSTN_ID_MOTR3      ,

    piSTG_TBaseOrign   = 0 ,
    piSTG_TBaseRvers       ,
    MAX_PSTN_ID_MOTR4      ,


};

enum EN_PSTN_VALUE {

    pvWRK_VBrd1_Wait   = 0 ,
    pvWRK_VBrd1_Wrk1       ,
    pvWRK_VBrd1_Wrk2       ,
    pvWRK_VBrd1_Wrk3       ,
    pvWRK_VBrd1_Wrk4       ,
    pvWRK_VBrd1_Wrk5       ,
    pvWRK_VBrd1_Wrk6       ,
    pvWRK_VBrd1_Wrk7       ,
    pvWRK_VBrd1_Wrk8       ,
    pvWRK_VBrd1_Wrk9       ,
    pvWRK_VBrd1_Wrk10      ,
    MAX_PSTN_MOTR0         ,

    pvWRK_VBrd2_Wait   = 0 ,
    pvWRK_VBrd2_Wrk1       ,
    pvWRK_VBrd2_Wrk2       ,
    pvWRK_VBrd2_Wrk3       ,
    pvWRK_VBrd2_Wrk4       ,
    pvWRK_VBrd2_Wrk5       ,
    pvWRK_VBrd2_Wrk6       ,
    pvWRK_VBrd2_Wrk7       ,
    pvWRK_VBrd2_Wrk8       ,
    pvWRK_VBrd2_Wrk9       ,
    pvWRK_VBrd2_Wrk10      ,
    MAX_PSTN_MOTR1         ,

    pvWRK_UBrd1_Wait   = 0 ,
    pvWRK_UBrd1_Wrk1       ,
    pvWRK_UBrd1_Wrk2       ,
    pvWRK_UBrd1_Wrk3       ,
    pvWRK_UBrd1_Wrk4       ,
    pvWRK_UBrd1_Wrk5       ,
    pvWRK_UBrd1_Wrk6       ,
    pvWRK_UBrd1_Wrk7       ,
    pvWRK_UBrd1_Wrk8       ,
    pvWRK_UBrd1_Wrk9       ,
    pvWRK_UBrd1_Wrk10      ,
    MAX_PSTN_MOTR2         ,

    pvWRK_UBrd2_Wait   = 0 ,
    pvWRK_UBrd2_Wrk1       ,
    pvWRK_UBrd2_Wrk2       ,
    pvWRK_UBrd2_Wrk3       ,
    pvWRK_UBrd2_Wrk4       ,
    pvWRK_UBrd2_Wrk5       ,
    pvWRK_UBrd2_Wrk6       ,
    pvWRK_UBrd2_Wrk7       ,
    pvWRK_UBrd2_Wrk8       ,
    pvWRK_UBrd2_Wrk9       ,
    pvWRK_UBrd2_Wrk10      ,
    MAX_PSTN_MOTR3         ,

    pvSTG_TBaseOrign   = 0 ,
    pvSTG_TBaseRvers       ,
    MAX_PSTN_MOTR4         ,

};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,
    mcCycleStart           =  2 ,
    mcCycleRotate          =  3 ,


    MAX_MANUAL_CYCLE
};
#endif



