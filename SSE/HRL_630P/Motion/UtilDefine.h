#ifndef UTILDEFINEH
#define UTILDEFINEH

#define EQP_SERIAL_LIST \
"\
SSE-14-1011,\
"




enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

//파트 아이디.
enum EN_PART_ID {
    piFFD = 0  , // Front Feed  구간 (자재투입 , 얼라인 , 스테이지아웃)
    piLFD = 1  , // Lear  Feed  구간 (자재투입 , 얼라인 , 스테이지아웃)
    piFLS = 2  , // Front Laser 구간 (레이저워크 , 배출)
    piLLS = 3  , // Lear  Laser 구간 (레이저워크 , 배출)
    MAX_PART
};

enum EN_ARAY_ID {
    riFFD = 0  , // Front Index ARAY
    riLFD      , // Lear  Index ARAY
    riFLS      , // Front Laser ARAY
    riLLS      , // Lear  Laser ARAY
    MAX_ARAY
};

//mi<파트명3자리>_<축방샹1자리><부가명칭3>
enum EN_MOTR_ID {
    miFED_YFrt  = 0 , // Front Index ARAY
    miFED_YLer  = 1 , // Lear  Index ARAY
    miLSR_TFrt  = 2 , // Front Laser ARAY
    miLSR_TLer  = 3 , // Lear  Laser ARAY
    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiFED_Covr_FwBw = 0 , //      커버 실린더
    aiFED_Algn_FwBw = 1 , //    얼라인 실린더
    aiLSR_StgF_FwBw = 2 , // F스테이지 실린더
    aiLSR_StgL_FwBw = 3 , // L스테이지 실린더

    MAX_ACTR
};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_StartSw     =  0 , //
    x0001 =  1 , xETC_StopSw      =  1 , //
    x0002 =  2 , xETC_ResetSw     =  2 , //
    x0003 =  3 , xETC_InitSw      =  3 , //
    x0004 =  4 , xETC_EmgSw       =  4 , //
    x0005 =  5 , x005             =  5 , //
    x0006 =  6 , x006             =  6 , //
    x0007 =  7 , x007             =  7 , //
    x0008 =  8 , x008             =  8 , //
    x0009 =  9 , x009             =  9 , //
    x000A = 10 , x010             = 10 , //
    x000B = 11 , xFED_CvrFw       = 11 , //
    x000C = 12 , xFED_CvrBw       = 12 , //
    x000D = 13 , xETC_BoxOut      = 13 , //
    x000E = 14 , x014             = 14 , //
    x000F = 15 , x015             = 15 , //

    x0010 = 16 , x016             = 16 , //
    x0011 = 17 , x017             = 17 , //
    x0012 = 18 , x018             = 18 , //
    x0013 = 19 , x019             = 19 , //
    x0014 = 20 , xETC_FrntDoor    = 20 , //
    x0015 = 21 , xETC_SideDoor    = 21 , //
    x0016 = 22 , xETC_LearDoor    = 22 , //
    x0017 = 23 , x023             = 23 , //
    x0018 = 24 , xLSR_VccSnsrF    = 24 , //
    x0019 = 25 , xLSR_VccSnsrL    = 25 , //
    x001A = 26 , xLSR_FStgFw      = 26 , //
    x001B = 27 , xLSR_FStgBw      = 27 , //
    x001C = 28 , xLSR_LStgFw      = 28 , //
    x001D = 29 , xLSR_LStgBw      = 29 , //
    x001E = 30 , x030             = 30 , //
    x001F = 31 , x031             = 31 , //

    MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리>
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp     =  0 , //
    y0001 =  1 , yETC_StopLp      =  1 , //
    y0002 =  2 , yETC_ResetLp     =  2 , //
    y0003 =  3 , yETC_InitLp      =  3 , //
    y0004 =  4 , y004             =  4 , //
    y0005 =  5 , y005             =  5 , //
    y0006 =  6 , y006             =  6 , //
    y0007 =  7 , y007             =  7 , //
    y0008 =  8 , yFED_AirBlow1    =  8 , //
    y0009 =  9 , yFED_AirBlow2    =  9 , //
    y000A = 10 , yLSR_FStgFw      = 10 , //
    y000B = 11 , yLSR_LStgFw      = 11 , //
    y000C = 12 , yLSR_FStgBw      = 12 , //
    y000D = 13 , yLSR_LStgBw      = 13 , //
    y000E = 14 , yFED_AlgnFw      = 14 , //
    y000F = 15 , yFED_CovrFw      = 15 , //

    y0010 = 16 , yFED_Vcc1        = 16 , //
    y0011 = 17 , yFED_Vcc2        = 17 , //
    y0012 = 18 , y018             = 18 , //
    y0013 = 19 , y019             = 19 , //
    y0014 = 20 , yETC_TwRedLp     = 20 , //
    y0015 = 21 , yETC_TwYelLp     = 21 , //
    y0016 = 22 , yETC_TwGrnLp     = 22 , //
    y0017 = 23 , yETC_TwBzz       = 23 , //
    y0018 = 24 , yETC_DustSuck    = 24 , // 집진기...
    y0019 = 25 , yETC_Feeder      = 25 , // 피더.....
    y001A = 26 , y026             = 26 , //
    y001B = 27 , y027             = 27 , //
    y001C = 28 , yLSR_VccF        = 28 , //
    y001D = 29 , yLSR_EjctF       = 29 , //
    y001E = 30 , yLSR_VccL        = 30 , //
    y001F = 31 , yLSR_EjctL       = 31 , //



    MAX_OUTPUT
};

// 포지션 벨류   pv+<파트3자리>+_+<부가설명>+<포지션설명>
// 오프셑 오프셑 po+<파트3자리>+_+<부가설명>+<오프셑설명>
enum EN_PSTN_VALUE {
    pvFED_YFrtWait       =  0  ,
    pvFED_YFrtWork             ,
    pvFED_YFrtFeed             ,
    MAX_PSTN_MOTR0             ,

    pvFED_YLerWait       =  0  ,
    pvFED_YLerWork             ,
    pvFED_YLerFeed             ,
    MAX_PSTN_MOTR1             ,

    pvLSR_TFrtWait       =  0  ,
    pvLSR_TFrtPos1             ,
    pvLSR_TFrtPos2             ,
    pvLSR_TFrtPos3             ,
    pvLSR_TFrtPos4             ,
    pvLSR_TFrtPos5             ,
    pvLSR_TFrtPlace            ,
    MAX_PSTN_MOTR2             ,

    pvLSR_TLerWait       =  0  ,
    pvLSR_TLerPos1             ,
    pvLSR_TLerPos2             ,
    pvLSR_TLerPos3             ,
    pvLSR_TLerPos4             ,
    pvLSR_TLerPos5             ,
    pvLSR_TLerPlace            ,
    MAX_PSTN_MOTR3             ,

};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {

    piFED_YFrtWait       =  0  ,
    piFED_YFrtWork             ,
    piFED_YFrtFeed             ,
    MAX_PSTN_ID_MOTR0          ,

    piFED_YLerWait       =  0  ,
    piFED_YLerWork             ,
    piFED_YLerFeed             ,
    MAX_PSTN_ID_MOTR1          ,

    piLSR_TFrtWait       =  0  ,
    piLSR_TFrtPos1             ,
    piLSR_TFrtPos2             ,
    piLSR_TFrtPos3             ,
    piLSR_TFrtPos4             ,
    piLSR_TFrtPos5             ,
    piLSR_TFrtPlace            ,
    MAX_PSTN_ID_MOTR2          ,

    piLSR_TLerWait       =  0  ,
    piLSR_TLerPos1             ,
    piLSR_TLerPos2             ,
    piLSR_TLerPos3             ,
    piLSR_TLerPos4             ,
    piLSR_TLerPos5             ,
    piLSR_TLerPlace            ,
    MAX_PSTN_ID_MOTR3          ,
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle      = 0  ,
    mcAllHome        = 1  ,
    mcFFD_Home       = 2  ,
    mcLFD_Home       = 3  ,
    mcFLS_Home       = 4  ,
    mcLLS_Home       = 5  ,
    mcETC_Feed       = 6  ,
    mcETC_DstSck     = 7  ,
    mcETC_Airblow1   = 8  ,
    mcETC_Airblow2   = 9  ,
    mcETC_Vcc1       = 10 ,
    mcETC_Vcc2       = 11 ,
    mcETC_Covr       = 12 ,
    mcETC_Algn       = 13 ,

    mcFFD_Stage      = 20 ,
    mcFFD_Vcc             ,
    mcFFD_Ejct            ,
    mcFFD_In              ,
    mcFFD_Out             ,
    mcFFD_Align           ,

    mcLFD_Stage      = 30 ,
    mcLFD_Vcc             ,
    mcLFD_Ejct            ,
    mcLFD_In              ,
    mcLFD_Out             ,
    mcLFD_Align           ,

    mcFLS_Work       = 40 ,
    mcFLS_Place           ,

    mcLLS_Work       = 50 ,
    mcLLS_Place           ,

    MAX_MANUAL_CYCLE
};

enum EN_ERR_ID {
/*000*/  eiETC_MainAir           =0,
/*001*/  ei001                     ,
/*002*/  eiETC_Emergency           ,
/*003*/  ei003                     ,
/*004*/  eiETC_FDoor               ,
/*005*/  eiETC_SDoor               ,
/*006*/  eiETC_BDoor               ,
/*007*/  ei007                     ,
/*008*/  ei008                     ,
/*009*/  eiETC_ToStartTO           ,
/*010*/  eiETC_ToStopTO            ,
/*011*/  eiETC_AllHomeTO           ,
/*012*/  eiETC_ManCycleTO          ,
/*013*/  ei013                     ,
/*014*/  eiPRT_CycleTO             ,
/*015*/  eiPRT_HomeTo              ,
/*016*/  eiPRT_ToStartTO           ,
/*017*/  eiPRT_ToStopTO            ,
/*018*/  ei018                     ,
/*019*/  eiMTR_HomeEnd             ,
/*020*/  eiMTR_NegLim              ,
/*021*/  eiMTR_PosLim              ,
/*022*/  eiMTR_Alarm               ,
/*023*/  ei023                     ,
/*024*/  eiATR_TimeOut             ,
/*025*/  ei025                     ,
/*026*/  eiPKG_Dispr               ,
/*027*/  eiPKG_Unknwn              , //--------------------------------- 장비공통에러.
/*028*/  eiETC_LotOpen             ,
/*029*/  ei029                     ,
/*030*/  ei030                     ,
/*031*/  eiLSR_FrntTNotWaitPos     ,
/*032*/  eiLSR_RearTNotWaitPos     ,
/*033*/  eiFED_FrntVision          ,
/*034*/  eiFED_RearVision          ,
/*035*/  ei035                     ,
/*036*/  ei036                     , //트렌스퍼쪽 카운팅 센서.
/*037*/  eiETC_BoxOut              ,
/*038*/  ei038                     ,
/*039*/  ei039                     ,
/*040*/  ei040                     ,
/*041*/  eiLSR_Vaccum              ,
/*042*/  eiLSR_Eject               ,
/*043*/  ei043                     ,
/*044*/  eiFLS_LaserBlock          ,
/*045*/  eiLLS_LaserBlock          ,
/*046*/  eiLSR_LaserErrCode        ,
/*047*/  eiLSR_Laser               ,
/*048*/  eiLSR_CheckSumFail        ,
/*049*/  ei049                     ,
/*050*/  ei050                     ,

/*080*/  MAX_ERR
};


#endif






























































