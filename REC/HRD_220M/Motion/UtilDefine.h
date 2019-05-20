#ifndef UTILDEFINEH
#define UTILDEFINEH

#define EQP_SERIAL_LIST \
"\
ITW-14-1025,\
"

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

//파트 아이디.
enum EN_PART_ID {
    piJIP = 0  , // Stage
    piDIP      , // Dispenser
    piPLC      , // Place

    MAX_PART
};

enum EN_ARAY_ID {
    riDSP  = 0 , // Dispenser
    riJIG      , // JIG
    riPIK      , // Picker
    MAX_ARAY
};

//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail

//mi<파트명3자리>_<축방샹1자리><부가명칭4>
enum EN_MOTR_ID {
    miDSP_XDsps  = 0  , // Dsps X 축
    miSTG_ZPick  = 1  , // Pick Z 축

    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiDSP_IdxUpDn  = 0 ,
    aiSTG_PshFwBw      ,

    MAX_ACTR

};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_StartSw            =  0 , //
    x0001 =  1 , xETC_StopSw             =  1 , //
    x0002 =  2 , xETC_EmgSw              =  2 , //
    x0003 =  3 , xSTG_PickFw             =  3 , //
    x0004 =  4 , xSTG_PickBw             =  4 , //
    x0005 =  5 , xDSP_DspsUp             =  5 , //
    x0006 =  6 , xDSP_DspsDn             =  6 , //
    x0007 =  7 , xETC_PickVacuum         =  7 , //
    x0008 =  8 , xETC_TempCont           =  8 , //
    x0009 =  9 , xETC_MainPower          =  9 , //
    x000A = 10 , xETC_MainAirCheck       = 10 , //
    x000B = 11 , x011                    = 11 , //
    x000C = 12 , x012                    = 12 , //
    x000D = 13 , x013                    = 13 , //
    x000E = 14 , x014                    = 14 , //
    x000F = 15 , x015                    = 15 , //

    MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리>
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp            =  0 , //
    y0001 =  1 , yETC_StopLp             =  1 , //
    y0002 =  2 , yETC_MainAirSol         =  2 , //
    y0003 =  3 , ySTG_Break              =  3 , //
    y0004 =  4 , ySTG_PickFw             =  4 , //
    y0005 =  5 , ySTG_PickBw             =  5 , //
    y0006 =  6 , ySTG_DisnrUp            =  6 , // Dispenser Up
    y0007 =  7 , ySTG_DisnrDn            =  7 , // Dispenser Dn
    y0008 =  8 , yETC_VCMAirOnOff        =  8 , // Vacuum Air On Off
    y0009 =  9 , yETC_Vacuum             =  9 , // Vaccum
    y000A = 10 , yETC_Eject              = 10 , // Eject
    y000B = 11 , yETC_Buzz               = 11 , //
    y000C = 12 , y012                    = 12 , //
    y000D = 13 , y013                    = 13 , //
    y000E = 14 , y014                    = 14 , //
    y000F = 15 , y015                    = 15 , //

    MAX_OUTPUT
};

// 포지션 벨류   pv+<파트3자리>+_+<부가설명>+<포지션설명>
// 오프셑 오프셑 po+<파트3자리>+_+<부가설명>+<오프셑설명>
enum EN_PSTN_VALUE {
    pvDSP_XIndxWait    = 0 , // 모터 처음 위치
    pvDSP_XIndxWorkStt     , // 모터 디스펜서 용액 뿌리는 시작 위치
    pvDSP_XIndxEnd         , // 모터 디스펜서 용액 끝나는 위치
    MAX_PSTN_MOTR0         ,

    pvSTG_ZPickWait    = 0 , // 모터 어딘가 처음 위치
    pvSTG_ZPickJIG         , // 모터 지그 위치
    pvSTG_ZPickWork        , // 모터 히터 블럭 위치
    pvSTG_ZPickPlace       , // 히터 블럭 눌러주고 위로 올라갔다가 뒤로 빠져 wait로 가는 위치
    MAX_PSTN_MOTR1         ,

};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {
    piDSP_XIndxWait    = 0 ,
    piDSP_XIndxWorkStt     ,
    piDSP_XIndxEnd         ,
    MAX_PSTN_ID_MOTR0      ,

    piSTG_ZPickWait    = 0 ,
    piSTG_ZPickJIG         ,
    piSTG_ZPickWork        ,
    piSTG_ZPickPlace       ,
    MAX_PSTN_ID_MOTR1      ,

};


enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcJIP_CycleJigPick     = 10 ,
    mcDIP_CycleDispenser        ,
    mcPLC_CyclePlace            ,
    mcSTG_AllSttClear      = 16 ,
    mcSTG_IdxUpDn          = 18 ,
    mcSTG_PshFwBw               ,
    mcSTG_Vacuum                ,
    mcSTG_Eject                 ,

    MAX_MANUAL_CYCLE
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/ei004                 ,
/*005*/ei005                 ,
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
/*018*/ei018                 ,
/*019*/eiMTR_HomeEnd         ,
/*020*/eiMTR_NegLim          , 
/*021*/eiMTR_PosLim          , 
/*022*/eiMTR_Alarm           , 
/*023*/ei023                 ,
/*024*/eiATR_TimeOut         , 
/*025*/ei025                 ,
/*026*/eiPKG_Dispr           ,
/*027*/eiPKG_Unknwn          , //--------------------------------- 장비공통에러.
/*028*/ei028                 ,
/*029*/ei029                 ,
/*030*/eiPIC_Vaccum          , //PIK
/*031*/ei031                 ,
/*032*/ei032                 ,
/*033*/ei033                 ,
/*034*/ei034                 ,
/*035*/ei035                 ,
/*036*/ei036                 ,
/*037*/ei037                 ,
/*038*/ei038                 ,
/*039*/ei039                 ,
/*040*/ei040                 ,
/*041*/ei041                 ,
/*042*/ei042                 ,
/*043*/ei043                 ,
/*044*/ei044                 ,
/*045*/ei045                 ,
/*046*/ei046                 ,
/*047*/ei047                 ,
/*048*/ei048                 ,
/*050*/ei049                 ,
/*051*/ei050                 ,
/*052*/ei051                 ,
/*053*/ei053                 ,
/*054*/ei054                 ,
/*055*/ei055                 ,
/*056*/ei056                 ,
/*057*/ei057                 ,
/*058*/ei058                 ,
/*059*/ei059                 ,
/*060*/MAX_ERR
};


#endif



