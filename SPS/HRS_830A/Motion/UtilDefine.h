#ifndef UTILDEFINEH
#define UTILDEFINEH

#define EQP_SERIAL_LIST \
"\
SPS-14-1026,\
"

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum { toLeft = 0 , toRigt = 1};

//파트 아이디.
enum EN_PART_ID {
    piPRB = 0  , // 프리버퍼
    piWRK      , // 워크
    piPSB      , // 포스트버퍼
    piLDR      , // 로더.
    piULD      , // 언로더

    MAX_PART
};

enum EN_ARAY_ID {
    riPRB  = 0 , // 프리버퍼
    riLWK      , // 워크 왼쪽에 작업중인 자재
    riRWK      , // 워크 오른쪽 작업중인 자재(포스트 버퍼 넘기기 전)
    riPSB      , // 포스트버퍼
    riLDR      , // 로더
    riULD      , // 언로더

    MAX_ARAY
};

//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail

//mi<파트명3자리>_<축방샹1자리><부가명칭4>
enum EN_MOTR_ID {
    miPRB_XIndx  = 0  ,
    miPSB_XIndx  = 1  , //Ezi to Ezi로 묶기 위해서JS
    miWRK_XIndx  = 2  , //JN  to JN 으로 묶기 위해서JS
    miWRK_YDsps  = 3  , // DISPENSER == DSPS
    miLDR_YClmp  = 4  , //로더
    miLDR_ZClmp  = 5  ,
    miULD_YClmp  = 6  ,
    miULD_ZClmp  = 7  ,

    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiPRB_IdxUpDn  = 0 ,
    aiWRK_LIdUpDn      ,
    aiWRK_RIdUpDn      ,
    aiPSB_IdxUpDn      ,
    aiPSB_PshFwBw      ,
    aiPSB_PshDnUp      ,
    aiLDR_CmpUpDn      ,
    aiLDR_PshFwBw      ,
    aiULD_CmpUpDn      ,

    MAX_ACTR

};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_StartSw            =  0 , //
    x0001 =  1 , xETC_StopSw             =  1 , //
    x0002 =  2 , xETC_ResetSw            =  2 , //
    x0003 =  3 , xETC_AirSw              =  3 , //
    x0004 =  4 , xETC_InitSw             =  4 , //
    x0005 =  5 , xETC_FEmgSw             =  5 , //
    x0006 =  6 , xETC_FDoor              =  6 , //
    x0007 =  7 , xETC_BDoor              =  7 , //
    x0008 =  8 , xETC_MainPower          =  8 , //
    x0009 =  9 , xPRB_PkgIn              =  9 , //
    x000A = 10 , xPRB_PkgDetect          = 10 , //
    x000B = 11 , xPSB_PkgOut             = 11 , //
    x000C = 12 , xPRB_IdxOvld            = 12 , //
    x000D = 13 , xWRK_IdxOvld            = 13 , //
    x000E = 14 , xPSB_IdxOvld            = 14 , //
    x000F = 15 , xPRB_IdxUp              = 15 , //
    x0010 = 16 , xWRK_LIdxUp             = 16 , //
    x0011 = 17 , xWRK_RIdxUp             = 17 , //
    x0012 = 18 , xPSB_IdxUp              = 18 , //
    x0013 = 19 , xPSB_PshrOvld           = 19 , // Tank Part
    x0014 = 20 , xPSB_PshrFw             = 20 , //
    x0015 = 21 , xPSB_PshrBw             = 21 , //
    x0016 = 22 , xPSB_PshrUp             = 22 , //
    x0017 = 23 , xPRB_PshrDn             = 23 , //
    x0018 = 24 , xDSP_RQSnsr             = 24 , // residual quantity Sensor 잔량 감지
    x0019 = 25 , xDSP_LDiscSnsr          = 25 , // Left discharge           토출 감지
    x001A = 26 , xDSP_RDiscSnsr          = 26 , // Right discharge          토출 감지
    x001B = 27 , x027                    = 27 , //
    x001C = 28 , x028                    = 28 , //
    x001D = 29 , xWRK_RPkgDetect         = 29 , //
    x001E = 30 , xETC_MainAir            = 30 , //
    x001F = 31 , xETC_BEmgSw             = 31 , //
    x0020 = 32 , xLDR_PshrOvld           = 32 , //
    x0021 = 33 , xLDR_PshrFw             = 33 , //
    x0022 = 34 , xLDR_PahrBw             = 34 , //
    x0023 = 35 , xLDR_MgzDetect1         = 35 , //
    x0024 = 36 , xLDR_MgzDetect2         = 36 , //
    x0025 = 37 , xLDR_ClmpUp             = 37 , //
    x0026 = 38 , xLDR_ClmpDn             = 38 , //
    x0027 = 39 , x039                    = 39 , //
    x0028 = 40 , xLDR_MgzIn              = 40 , //
    x0029 = 41 , xLDR_MgzOutFull         = 41 , //
    x002A = 42 , x042                    = 42 , //
    x002B = 43 , x043                    = 43 , //
    x002C = 44 , x044                    = 44 , //
    x002D = 45 , x045                    = 45 , //
    x002E = 46 , xLDR_EmgSw              = 46 , //
    x002F = 47 , x047                    = 47 , //
    x0030 = 48 , xULD_MgzDetect1         = 48 , //
    x0031 = 49 , xULD_MgzDetect2         = 49 , //
    x0032 = 50 , xULD_ClmpUp             = 50 , //
    x0033 = 51 , xULD_ClmpDn             = 51 , //
    x0034 = 52 , x052                    = 52 , //
    x0035 = 53 , xULD_MgzOutFull         = 53 , //
    x0036 = 54 , xULD_MgzIn              = 54 , //
    x0037 = 55 , x055                    = 55 , //
    x0038 = 56 , x056                    = 56 , //
    x0039 = 57 , x057                    = 57 , //
    x003A = 58 , xULD_EmgSw              = 58 , //
    x003B = 59 , x059                    = 59 , //
    x003C = 60 , x060                    = 60 , //
    x003D = 61 , x061                    = 61 , //
    x003E = 62 , x062                    = 62 , //
    x003F = 63 , x063                    = 63 , //

    MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리>
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp            =  0 , //
    y0001 =  1 , yETC_StopLp             =  1 , //
    y0002 =  2 , yETC_ResetLp            =  2 , //
    y0003 =  3 , yETC_AirLp              =  3 , //
    y0004 =  4 , yETC_InitLp             =  4 , //
    y0005 =  5 , yETC_MainAir            =  5 , //
    y0006 =  6 , yETC_EjctAirSol         =  6 , //
    y0007 =  7 , y007                    =  7 , //
    y0008 =  8 , yETC_TwRedLp            =  8 , //
    y0009 =  9 , yETC_TwYelLp            =  9 , //
    y000A = 10 , yETC_TwGrnLp            = 10 , //
    y000B = 11 , yETC_TwBzz              = 11 , //
    y000C = 12 , yPRB_IdxUpDn            = 12 , //
    y000D = 13 , yWRK_LIdxUpDn           = 13 , //
    y000E = 14 , yWRK_RIdxUpDn           = 14 , //
    y000F = 15 , yPSB_IdxUpDn            = 15 , //

    y0010 = 16 , yPRB_AirBlow            = 16 , //
    y0011 = 17 , yPSB_PshrFw             = 17 , //
    y0012 = 18 , yPSB_PshrBw             = 18 , //
    y0013 = 19 , yPSB_PshrUpDn           = 19 , //
    y0014 = 20 , yWRK_LDispensor         = 20 , //
    y0015 = 21 , yWRK_RDispensor         = 21 , //
    y0016 = 22 , y022                    = 22 , //
    y0017 = 23 , y023                    = 23 , //
    y0018 = 24 , yETC_Suction            = 24 , //
    y0019 = 25 , y025                    = 25 , //
    y001A = 26 , y026                    = 26 , //
    y001B = 27 , y027                    = 27 , //
    y001C = 28 , y028                    = 28 , //
    y001D = 29 , y029                    = 29 , //
    y001E = 30 , y030                    = 30 , //
    y001F = 31 , y031                    = 31 , //

    y0040 = 32 , yLDR_MgzInAC            = 32 , //
    y0041 = 33 , yLDR_MgzOutAC           = 33 , //
    y0042 = 34 , yLDR_ClmpDn             = 34 , //
    y0043 = 35 , yLDR_ClmpUp             = 35 , //
    y0044 = 36 , yLDR_PshrFwBw           = 36 , //
    y0045 = 37 , yLDR_ZBreak             = 37 , //
    y0046 = 38 , y038                    = 38 , //
    y0047 = 39 , y039                    = 39 , //
    y0048 = 40 , y040                    = 40 , //
    y0049 = 41 , y041                    = 41 , //
    y004A = 42 , y042                    = 42 , //
    y004B = 43 , y043                    = 43 , //
    y004C = 44 , y044                    = 44 , //
    y004D = 45 , y045                    = 45 , //
    y004E = 46 , y046                    = 46 , //
    y004F = 47 , y047                    = 47 , //

    y0050 = 48 , yULD_MgzOutAC           = 48 , //
    y0051 = 49 , yULD_MgzInAC            = 49 , //
    y0052 = 50 , yULD_ClmpDn             = 50 , //
    y0053 = 51 , yULD_ClmpUp             = 51 , //
    y0054 = 52 , yULD_ZBreak             = 52 , //
    y0055 = 53 , y053                    = 53 , //
    y0056 = 54 , y054                    = 54 , //
    y0057 = 55 , y055                    = 55 , //
    y0058 = 56 , y056                    = 56 , //
    y0059 = 57 , y057                    = 57 , //
    y005A = 58 , y058                    = 58 , //
    y005B = 59 , y059                    = 59 , //
    y005C = 60 , y060                    = 60 , //
    y005D = 61 , y061                    = 61 , //
    y005E = 62 , y062                    = 62 , //
    y005F = 63 , y063                    = 63 , //

    MAX_OUTPUT
};

// 포지션 벨류   pv+<파트3자리>+_+<부가설명>+<포지션설명>
// 오프셑 오프셑 po+<파트3자리>+_+<부가설명>+<오프셑설명>
enum EN_PSTN_VALUE {
    pvPRB_XIndxWait        = 0 ,
    pvPRB_XIndxWork            ,
    pvPRB_XIndxBack            ,
    pvPRB_XIndxOut             ,
    MAX_PSTN_MOTR0             ,

    pvPSB_XIndxWait        = 0 ,
    pvPSB_XIndxOut             ,
    MAX_PSTN_MOTR1             ,

    pvWRK_XIndxWait        = 0 ,
    pvWRK_XIndxWorkStt         ,
    pvWRK_XIndxRWorkStt        ,
    pvWRK_XIndxOut             ,
    MAX_PSTN_MOTR2             ,

    pvWRK_YDspsWait        = 0 ,
    pvWRK_YDspsWorkStt         ,
    MAX_PSTN_MOTR3             ,

    pvLDR_YClmpWait        = 0 ,
    pvLDR_YClmpWork            ,
    pvLDR_YClmpPick            ,
    pvLDR_YClmpPlace           ,
    MAX_PSTN_MOTR4             ,

    pvLDR_ZClmpWait        = 0 ,
    pvLDR_ZClmpPickFwd         ,
    pvLDR_ZClmpClampOn         ,
    pvLDR_ZClmpPickBwd         ,
    pvLDR_ZClmpWorkStart       ,
    pvLDR_ZClmpPlaceFwd        ,
    pvLDR_ZClmpClampOff        ,
    pvLDR_ZClmpPlaceBwd        ,
    MAX_PSTN_MOTR5             ,

    pvULD_YClmpWait        = 0 ,
    pvULD_YClmpWork            ,
    pvULD_YClmpPick            ,
    pvULD_YClmpPlace           ,
    MAX_PSTN_MOTR6             ,

    pvULD_ZClmpWait        = 0 ,
    pvULD_ZClmpPickFwd         ,
    pvULD_ZClmpClampOn         ,
    pvULD_ZClmpPickBwd         ,
    pvULD_ZClmpWorkStart       ,
    pvULD_ZClmpPlaceFwd        ,
    pvULD_ZClmpClampOff        ,
    pvULD_ZClmpPlaceBwd        ,
    MAX_PSTN_MOTR7
};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {
    piPRB_XIndxWait        = 0 ,
    piPRB_XIndxWork            ,
    piPRB_XIndxBack            ,
    piPRB_XIndxOut             ,
    MAX_PSTN_ID_MOTR0          ,

    piPSB_XIndxWait        = 0 ,
    piPSB_XIndxOut             ,
    MAX_PSTN_ID_MOTR1          ,

    piWRK_XIndxWait        = 0 ,
    piWRK_XIndxWorkStt         ,
    piWRK_XIndxRWorkStt        ,
    piWRK_XIndxOut             ,
    piWRK_XIndxWork            ,
    piWRK_XIndxRWork           ,
    MAX_PSTN_ID_MOTR2          ,

    piWRK_YDspsWait        = 0 ,
    piWRK_YDspsWorkStt         ,
    piWRK_YDspsWork            ,
    MAX_PSTN_ID_MOTR3          ,

    piLDR_YClmpWait        = 0 ,
    piLDR_YClmpWork            ,
    piLDR_YClmpPick            ,
    piLDR_YClmpPlace           ,
    MAX_PSTN_ID_MOTR4          ,

    piLDR_ZClmpWait        = 0 ,
    piLDR_ZClmpPickFwd         ,
    piLDR_ZClmpClampOn         ,
    piLDR_ZClmpPickBwd         ,
    piLDR_ZClmpWorkStart       ,
    piLDR_ZClmpPlaceFwd        ,
    piLDR_ZClmpClampOff        ,
    piLDR_ZClmpPlaceBwd        ,
    piLDR_ZClmpWork            ,
    MAX_PSTN_ID_MOTR5          ,

    piULD_YClmpWait        = 0 ,
    piULD_YClmpWork            ,
    piULD_YClmpPick            ,
    piULD_YClmpPlace           ,
    MAX_PSTN_ID_MOTR6          ,

    piULD_ZClmpWait        = 0 ,
    piULD_ZClmpPickFwd         ,
    piULD_ZClmpClampOn         ,
    piULD_ZClmpPickBwd         ,
    piULD_ZClmpWorkStart       ,
    piULD_ZClmpPlaceFwd        ,
    piULD_ZClmpClampOff        ,
    piULD_ZClmpPlaceBwd        ,
    piULD_ZClmpWork            ,
    MAX_PSTN_ID_MOTR7

};


enum EN_MANUAL_CYCLE
{

/*
    piPRB = 0  , //프리버퍼
    piRAL      , //레이져,비젼 밑에 레일
    piLSR      , //레이져
    piVSN      , //비젼검사
    piPSB      , //포스트버퍼
    piLDR      , //로더.
    piULD      , //언로더.
*/
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,
    mcStopTODisprShot      =  2 , 

    mcLDR_Home             = 10 ,
    mcLDR_Supply                ,
    mcLDR_Pick                  ,
    mcLDR_SttPstn               ,
    mcLDR_Place                 ,
    mcLDR_UpSlPitch             ,
    mcLDR_DnSlPitch             ,
    mcLDR_PusherFwBw            ,
    mcLDR_ClampDnUp             ,
    mcLDR_TranInAC              ,
    mcLDR_TranOutAC             ,
 ///////////////////////////////////////////////////////////////////////////////

    mcPRB_Home             = 30 ,
    mcPRB_CycleIn               ,
    mcPRB_CycleOut              ,
    mcPRB_AirBlow               ,
    mcPRB_IdxUpDn               ,

    mcWRK_Home             = 50 ,
    mcWRK_CycleIn               ,
    mcWRK_CycleOut              ,
    mcWRK_CycleWork             , // Work Zone의 전채적안 동작을 허는 것인가??
    mcWRK_LIdxUpDn              ,
    mcWRK_RIdxUpDn              ,
    mcWRK_DspsWait              ,
    mcWRK_DspsWork              ,

    mcPSB_Home             = 70 ,
    mcPSB_CycleOut              ,
    mcPSB_IndxDnUp              ,

 ///////////////////////////////////////////////////////////////////////////////
    mcULD_Home             = 140,
    mcULD_Supply                ,
    mcULD_Pick                  ,
    mcULD_SttPstn               ,
    mcULD_Place                 ,
    mcULD_UpSlPitch             ,
    mcULD_DnSlPitch             ,
    mcULD_ClampDnUp             ,
    mcPSB_PusherFwBw            ,
    mcULD_TranInAC              ,
    mcULD_TranOutAC             ,


    mcETC_LDRULDPlace           ,

    MAX_MANUAL_CYCLE
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/eiETC_FDoor           ,
/*005*/eiETC_BDoor           ,
/*006*/ei006                 ,
/*007*/ei007                 ,
/*008*/ei008                 ,
/*009*/eiETC_ToStartTO       , 
/*010*/eiETC_ToStopTO        , 
/*011*/eiETC_AllHomeTO       , 
/*012*/eiETC_ManCycleTO      , 
/*013*/ei013                 , 
/*014*/eiPRT_CycleTO         ,
/*015*/eiPRT_HomeTo          ,
/*016*/eiPRT_ToStartTO       ,
/*017*/eiPRT_ToStopTO        ,
/*018*/ei018                 , //piPRB
/*019*/eiMTR_HomeEnd         , //piRAL
/*020*/eiMTR_NegLim          , //piLSR
/*021*/eiMTR_PosLim          , //piVSN
/*022*/eiMTR_Alarm           , //piPSB
/*023*/ei023                 , //piLDR
/*024*/eiATR_TimeOut         , //piULD
/*025*/ei025                 ,
/*026*/eiPKG_Dispr           ,
/*027*/eiPKG_Unknwn          , //--------------------------------- 장비공통에러.
/*028*/ei028                 ,
/*029*/ei029                 ,
/*030*/eiLDR_NeedMgz         , //LDR
/*031*/eiLDR_PshrOvld        ,
/*032*/eiLDR_FullMgz         ,
/*033*/ei033                 ,
/*034*/ei034                 ,
/*035*/eiULD_NeedMgz         , //ULD
/*036*/eiULD_FullMgz         ,
/*037*/ei037                 ,
/*038*/ei038                 ,
/*039*/ei039                 ,
/*040*/eiPRB_IndxOvld        , //비젼 공용.
/*041*/eiWRK_IndxOvld        ,
/*042*/eiPSB_IndxOvld        ,
/*043*/ei043                 ,
/*044*/ei044                 ,
/*045*/ei045                 , //트레인 선택해 주세요.
/*046*/eiDSP_discharge       , //디스펜서 토출 못함 에러.
/*047*/eiDSP_RQSensor        , //디스펜서 액 없음에러.
/*048*/ei048                 ,
/*049*/ei049                 ,
/*050*/MAX_ERR
};


#endif

