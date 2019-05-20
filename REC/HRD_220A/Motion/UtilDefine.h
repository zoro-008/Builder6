#ifndef UTILDEFINEH
#define UTILDEFINEH

//sunsun 로더 언로더 인덱스바 투스타트 관련
//소켓 플레이스 및 쉬프트 소켓 플레이스 인포 등등.
//언로더 세퍼레이터 처음 켤때 뒤로 빠지는것.
//랏엔드시에 스테이지 회전 하는것.
//
#define MODEL "HRD_220a"
#define EQP_SERIAL_LIST \
"\
REC-15-1048,\
"

//쓰레드 관련 해서 디버깅 하기 위해 추가..
//imicam 윈7에서 사용시 디버깅 강제 종료 하면 카메라를 껐다켜야해서.. 넣었다.
//7용 Neptune으로 교체 해야함....
#define SKIP_VISN false


/*
Rs232 순서.
비젼것은 비젼 파일 따로 있다.
Light1 =
Light2
*/
enum EN_RS232 {
    rsDispensor    = 0 , //com3     PortNo = 3 , BaudRate = 19200 , ByteSize = 8, StopBits = 0 , Parity = None
    rsTemperature  = 1 , //com4     PortNo = 4 , BaudRate =  9600 , ByteSize = 8, StopBits = 0 , Parity = None
    rsHeightSensor = 2 , //com5     PortNo = 5 , BaudRate =  9600 , ByteSize = 8, StopBits = 0 , Parity = None
    rsLoadCell     = 3 , //com6     PortNo = 6 , BaudRate = 19200 , ByteSize = 8, StopBits = 2 , Parity =  Odd
    MAX_RS232
};

enum EN_VAC_ERR_STAT { //현재 랏 런 하고 있는 상태.
    vsRetry        = 0 , //에러났던 곳을 다시 픽하고 안찝히면 다시 에러.
    vsErrSkipPick  = 1 , //에러 난 곳 스킵.
    vsErrLotEnd    = 2 , //랏엔드.
};

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};


//파트 아이디.
enum EN_PART_ID {
    piLDR_F  = 0 , //Loader Fornt
    piLDR_R      , //Loader Rear
    piPRI_F      , //PreIndex Front
    piPRI_R      , //PreIndex Rear
    piIDX_F      , //Index Front
    piIDX_R      , //Index Rear
    piULD_F      , //Unloader Front
    piULD_R      , //Unloader Rear
    piSTG        , //Work Stage Rear
    piPCK        , //Picker
    piDSP        , //Dispensor
    MAX_PART
};

enum EN_ARAY_ID {
    riLDR_FTop  =0 , //Loader Fornt Top
    riLDR_FBtm     , //Loader Fornt Bottom
    riLDR_RTop     , //Loader Rear  Top
    riLDR_RBtm     , //Loader Rear  Bottom
    riPRI_F        , //PreIndex Front
    riPRI_R        , //PreIndex Rear
    riIDX_F        , //Index Front
    riIDX_R        , //Index Rear
    riPSI_F        , //Post Index Front
    riPSI_R        , //Rear Index Front
    riULD_FTop     , //Unloader Front Top
    riULD_FBtm     , //Unloader Front Bottom
    riULD_RTop     , //Unloader Rear  Top
    riULD_RBtm     , //Unloader Rear  Bottom
    riPCK          , //Picker
    riALN          , //Aligner
    riSTG          , //Work Stage.

    //밑에 세개는 김원식 부장 요청으로 디스플레이용으로 야매로 만듬.
    //Dispensor작업 할때는 Stage에 데이터 None으로 보이게 하고  Dispensor로 디스플레이를 하게.
    //Vision 작업 할때는 Stage에 데이터 None으로 보이게 하고 Vision으로 디스플레이 하게.
    //밑에 3개의 데이터는 항상 None이고 판넬 페어런트만 타이머에서 작업중일때 확인 해서 바꿔 준다.
    riDSP          , //Dispensor
    riVSN_R        , //뒤쪽 비젼.
    riVSN_F        , //앞쪽 비젼.




    MAX_ARAY
};

//mi<파트명3자리>_<축방샹1자리><부가명칭4>
enum EN_MOTR_ID {
    miLDR_ZElevF = 0  , //Loader Elevator Z Front
    miLDR_ZElevR = 1  , //Loader Elevator Z Rear
    miIDX_XIndxF = 2  , //Index X Front
    miIDX_XIndxR = 3  , //Index Y Front
    miDSP_XDispr = 4  , //Dispensor X Dispensor          수동튜닝 인포지션밴드10펄스 50ms 언더
    miDSP_YDispr = 5  , //Dispensor Y Dispensor          수동튜닝 인포지션밴드10펄스 50ms 언더
    miPCK_YPickr = 6  , //Picker Y Picker                수동튜닝 인포지션밴드10펄스 50ms 언더
    miPCK_ZPickr = 7  , //Picker Z Picker                수동튜닝 인포지션밴드10펄스 50ms 언더
    miPRI_XIndxF = 8  , //Pre Index X Index Front
    miPRI_XIndxR = 9  , //Pre Index X Index Rear
    miDSP_ZDispr = 10 , //Dispensor Z Dispensor          수동튜닝 인포지션밴드10펄스 50ms 언더
    miSTG_TStage = 11 , //Stage T Stage Rear
    miSTG_XStage = 12 , //Stage X Stage Rear
    miSTG_YStage = 13 , //Stage Y Stage Rear
    miPCK_XAlign = 14 , //Picker X Align
    miPCK_YAlign = 15 , //Picker Y Align
    miULD_ZElevF = 16 , //Unloader Z Elevator Front
    miULD_ZElevR = 17 , //Unloader Z Elevator Rear
    miSTG_ZStage = 18 , //Index Z Stage Rear
    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiPCK_XAlgnFwBw = 0  ,
    aiPCK_YAlgnFwBw = 1  ,
    aiDSP_NdlCrFwBw = 2  ,
    aiIDX_FIndxUpDn = 3  ,
    aiIDX_RIndxUpDn = 4  ,
    aiPRI_FIndxUpDn = 5  ,
    aiPRI_RIndxUpDn = 6  ,
    aiULD_FPushLtRt = 7  ,
    aiULD_RPushLtRt = 8  ,
    aiULD_FPushUpDn = 9  ,
    aiULD_RPushUpDn = 10 ,
    aiDSP_NdlCvFwBw = 11 ,     //Fw시에 디스펜싱 가능하게.

    MAX_ACTR

};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {

    //Ft - Front , Rr - Rear 으로 통일 함.
    x0000 =  0 , xETC_StartSw            =  0 , //
    x0001 =  1 , xETC_StopSw             =  1 , //
    x0002 =  2 , xETC_ResetSw            =  2 , //
    x0003 =  3 , xETC_AirSw              =  3 , //
    x0004 =  4 , xETC_InitSw             =  4 , //
    x0005 =  5 , xETC_EmgSw              =  5 , //
    x0006 =  6 , xETC_MainPower          =  6 , //
    x0007 =  7 , xETC_MainAir            =  7 , //
    x0008 =  8 , xETC_DoorFt             =  8 , //
    x0009 =  9 , xETC_DoorRr             =  9 , //
    x000A = 10 , xETC_DoorLt             = 10 , //
    x000B = 11 , xETC_DoorRt             = 11 , //
    x000C = 12 , xLDR_FMgzDetect1        = 12 , //
    x000D = 13 , xLDR_FMgzDetect2        = 13 , //
    x000E = 14 , xLDR_RMgzDetect1        = 14 , //
    x000F = 15 , xLDR_RMgzDetect2        = 15 , //

    x0010 = 16 , xULD_FMgzDetect1        = 16 , //
    x0011 = 17 , xULD_FMgzDetect2        = 17 , //
    x0012 = 18 , xULD_RMgzDetect1        = 18 , //
    x0013 = 19 , xULD_RMgzDetect2        = 19 , //
    x0014 = 20 , xIDX_FIndxUp            = 20 , //
    x0015 = 21 , xIDX_FIndxDn            = 21 , //
    x0016 = 22 , xIDX_FIndxNotOvLd       = 22 , //
    x0017 = 23 , xIDX_RIndxUp            = 23 , //
    x0018 = 24 , xIDX_RIndxDn            = 24 , //
    x0019 = 25 , xIDX_RIndxNotOvLd       = 25 , //
    x001A = 26 , xPRI_FIndxUp            = 26 , //
    x001B = 27 , xPRI_FIndxDn            = 27 , //
    x001C = 28 , xPRI_FIndxNotOvLd       = 28 , //
    x001D = 29 , xPRI_RIndxUp            = 29 , //
    x001E = 30 , xPRI_RIndxDn            = 30 , //
    x001F = 31 , xPRI_RIndxNotOvLd       = 31 , //

    x0020 = 32 , xULD_FPushUp            = 32 , //
    x0021 = 33 , xULD_FPushDn            = 33 , //
    x0022 = 34 , xULD_FPushLt            = 34 , //
    x0023 = 35 , xULD_FPushRt            = 35 , //
    x0024 = 36 , xULD_RPushUp            = 36 , //
    x0025 = 37 , xULD_RPushDn            = 37 , //
    x0026 = 38 , xULD_RPushLt            = 38 , //
    x0027 = 39 , xULD_RPushRt            = 39 , //
    x0028 = 40 , xPRI_FDetect1           = 40 , //
    x0029 = 41 , xPRI_FDetect2           = 41 , //
    x002A = 42 , xIDX_FWrkDetect3        = 42 , //
    x002B = 43 , xIDX_FDetect4           = 43 , //
    x002C = 44 , xULD_FDetect5           = 44 , //
    x002D = 45 , xPRI_RDetect1           = 45 , //
    x002E = 46 , xPRI_RDetect2           = 46 , //
    x002F = 47 , xIDX_RWrkDetect3        = 47 , //

    x0030 = 48 , xIDX_RDetect4           = 48 , //
    x0031 = 49 , xULD_RDetect5           = 49 , //
    x0032 = 50 , xPCK_XAlgnFw            = 50 , //
    x0033 = 51 , xPCK_XAlgnBw            = 51 , //
    x0034 = 52 , xPCK_YAlgnFw            = 52 , //
    x0035 = 53 , xPCK_YAlgnBw            = 53 , //
    x0036 = 54 , xDSP_NdlCrFw            = 54 , //
    x0037 = 55 , xDSP_NdlCrBw            = 55 , //
    x0038 = 56 , xDSP_NdleNotCheck       = 56 , //
    x0039 = 57 , xDSP_HeadDamege         = 57 , //
    x003A = 58 , xPCK_Vccm               = 58 , //
    x003B = 59 , xSTG_Vccm               = 59 , //
    x003C = 60 , x060                    = 60 , //
    x003D = 61 , x061                    = 61 , //
    x003E = 62 , xDSP_NdlCvFw            = 62 , //
    x003F = 63 , x063                    = 63 , //

    MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>     자리
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리> 동
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp            =  0 , //
    y0001 =  1 , yETC_StopLp             =  1 , //
    y0002 =  2 , yETC_ResetLp            =  2 , //
    y0003 =  3 , yETC_AirLp              =  3 , //
    y0004 =  4 , yETC_InitLp             =  4 , //
    y0005 =  5 , yETC_TwRedLp            =  5 , //
    y0006 =  6 , yETC_TwYelLp            =  6 , //
    y0007 =  7 , yETC_TwGrnLp            =  7 , //
    y0008 =  8 , yETC_TwBzz              =  8 , //
    y0009 =  9 , yETC_MainAir            =  9 , //
    y000A = 10 , yETC_Heater             = 10 , //
    y000B = 11 , yETC_Light              = 11 , //
    y000C = 12 , yLDR_FZBreak            = 12 , //
    y000D = 13 , yLDR_RZBreak            = 13 , //
    y000E = 14 , yULD_FZKreak            = 14 , //
    y000F = 15 , yULD_RZBreak            = 15 , //
    //Ft - Front , Rr - Rear 으로 통일 함.
    y0010 = 16 , yIDX_FIndxUp            = 16 , //
    y0011 = 17 , yIDX_FIndxDn            = 17 , //
    y0012 = 18 , yIDX_RIndxUp            = 18 , //
    y0013 = 19 , yIDX_RIndxDn            = 19 , //
    y0014 = 20 , yPRI_FIndxUp            = 20 , //
    y0015 = 21 , yPRI_FIndxDn            = 21 , //
    y0016 = 22 , yPRI_RIndxUp            = 22 , //
    y0017 = 23 , yPRI_RIndxDn            = 23 , //
    y0018 = 24 , yULD_FPushUp            = 24 , //
    y0019 = 25 , yULD_FPushDn            = 25 , //
    y001A = 26 , yULD_FPushLt            = 26 , //
    y001B = 27 , yULD_FPushRt            = 27 , //
    y001C = 28 , yULD_RPushUp            = 28 , //
    y001D = 29 , yULD_RPushDn            = 29 , //
    y001E = 30 , yULD_RPushLt            = 30 , //
    y001F = 31 , yULD_RPushRt            = 31 , //

    y0020 = 32 , yIDX_FStgZBreak         = 32 , //
    y0021 = 33 , yPCK_XAlgnFw            = 33 , //
    y0022 = 34 , yPCK_XAlgnBw            = 34 , //
    y0023 = 35 , yPCK_YAlgnFw            = 35 , //
    y0024 = 36 , yPCK_YAlgnBw            = 36 , //
    y0025 = 37 , yIDX_FIonizer           = 37 , //
    y0026 = 38 , yIDX_RIonizer           = 38 , //
    y0027 = 39 , yDSP_NdlCrFw            = 39 , //
    y0028 = 40 , yDSP_NdlCrBw            = 40 , //
    y0029 = 41 , yIDX_MainVccm           = 41 , //장비 켜면 일단 켬.
    y002A = 42 , yIDX_IonVccm            = 42 , //이온블로우어 불때 켬.
    y002B = 43 , ySTG_Vccm               = 43 , //
    y002C = 44 , ySTG_Ejct               = 44 , //
    y002D = 45 , yPCK_VccmOn             = 45 , //
    y002E = 46 , yPCK_Ejct               = 46 , //
    y002F = 47 , y047                    = 47 , //

    y0030 = 48 , yDSP_ZBreak             = 48 , //
    y0031 = 49 , yPCK_ZBreak             = 49 , //
    y0032 = 50 , yDSP_DispOn             = 50 , // IO 전용 디스펜서
    y0033 = 51 , yDSP_NdlCvFw            = 51 , //
    y0034 = 52 , yDSP_NdlCvBw            = 52 , //
    y0035 = 53 , yPCK_VccmOff            = 53 , //
    y0036 = 54 , yPCK_WeightOff          = 54 , // IO로 무게 센서 전원 끄기.
    y0037 = 55 , yETC_MonitorOff         = 55 , // 상시 모니터 끄기.
    y0038 = 56 , y056                    = 56 , //
    y0039 = 57 , y057                    = 57 , //
    y003A = 58 , y058                    = 58 , //
    y003B = 59 , y059                    = 59 , //
    y003C = 60 , y060                    = 60 , //
    y003D = 61 , y061                    = 61 , //
    y003E = 62 , y062                    = 62 , //
    y003F = 63 , y063                    = 63 , //

    MAX_OUTPUT
};

enum EN_PSTN_VALUE {

    //miLDR_ZElevF = 0  , //Loader Elevator Z Front
    pvLDR_ZElevFWait        = 0 ,
    pvLDR_ZElevFWorkStartBtm    ,
    pvLDR_ZElevFWorkStartTop    ,
    MAX_PSTN_MOTR0              ,

    //miLDR_ZElevR = 1  , //Loader Elevator Z Rear
    pvLDR_ZElevRWait        = 0 ,
    pvLDR_ZElevRWorkStartBtm    ,
    pvLDR_ZElevRWorkStartTop    ,
    MAX_PSTN_MOTR1              ,

    //miIDX_XIndxF = 2  , //Index X Front
    pvIDX_XIndxFWait        = 0 ,
    pvIDX_XIndxFGet             ,
    pvIDX_XIndxFWorkStart       ,
    pvIDX_XIndxFOut             ,
    MAX_PSTN_MOTR2              ,

    //miIDX_XIndxR = 3  , //Index Y Front
    pvIDX_XIndxRWait        = 0 ,
    pvIDX_XIndxRGet             ,
    pvIDX_XIndxRWorkStart       ,
    pvIDX_XIndxROut             ,
    MAX_PSTN_MOTR3              ,

    //miDSP_XDispr = 4  , //Dispensor X Dispensor=====================
    pvDSP_XDisprWait        = 0 ,
    pvDSP_XDisprVsStageCntr     ,  //Fix Common
    pvDSP_XDisprVsPad           ,
    pvDSP_XDisprNdCheck         ,
    pvDSP_XDisprNdClean         ,
    pvDSP_XDisprHtCheck         ,
    pvDSP_XDisprHtStage         ,
    pvDSP_XDisprStgFlatness1    ,
    pvDSP_XDisprStgFlatness2    ,
    pvDSP_XDisprStgFlatness3    ,
    pvDSP_XDisprStgFlatness4    ,
    poDSP_XDisprVsNiddle        ,
    poDSP_XDisprVsHeight        ,
    MAX_PSTN_MOTR4              ,

    //miDSP_YDispr = 5  , //Dispensor Y Dispensor=====================
    pvDSP_YDisprWait        = 0 ,
    pvDSP_YDisprVsStageCntr     ,  //Fix Common
    pvDSP_YDisprVsPad           ,
    pvDSP_YDisprNdCheck         ,
    pvDSP_YDisprNdClean         ,
    pvDSP_YDisprHtCheck         ,
    pvDSP_YDisprHtStage         ,
    pvDSP_YDisprStgFlatness1    ,
    pvDSP_YDisprStgFlatness2    ,
    pvDSP_YDisprStgFlatness3    ,
    pvDSP_YDisprStgFlatness4    ,
    poDSP_YDisprVsNiddle        ,
    poDSP_YDisprVsHeight        ,  
    MAX_PSTN_MOTR5              ,

    //miPCK_YPickr = 6  , //Picker Y Picker=======================
    pvPCK_YPickrWait        = 0 ,
    pvPCK_YPickrStageCntr       ,  //Fix Common
    pvPCK_YPickrPick            ,
    pvPCK_YPickrAlign           ,
    pvPCK_YPickrHeight          ,
    pvPCK_YPickrVision          ,  //Fix Common
    pvPCK_YPickrCheck           ,
    MAX_PSTN_MOTR6              ,

    //miPCK_ZPickr = 7  , //Picker Z Picker=======================
    pvPCK_ZPickrWait        = 0 ,
    pvPCK_ZPickrMove            ,
    pvPCK_ZPickrPick            ,
    pvPCK_ZPickrAlign           ,
    pvPCK_ZPickrCheck           ,
    poPCK_ZPickrVsDefect        , //Ofset
    poPCK_ZPickrVsAlign         , //Ofset
    MAX_PSTN_MOTR7              ,

    //miPRI_XIndxF = 8  , //Pre Index X Index Front
    pvPRI_XIndxFWait        = 0 ,
    pvPRI_XIndxFGet             ,
    pvPRI_XIndxFOut             ,
    pvPRI_XIndxFOutChckOn       , //자제 나올때 끌려나왔는지 체크
    pvPRI_XIndxFOutChckOff      , //자제 다나았는지 체크.
    MAX_PSTN_MOTR8              ,

    //miPRI_XIndxR = 9  , //Pre Index X Index Rear
    pvPRI_XIndxRWait        = 0 ,
    pvPRI_XIndxRGet             ,
    pvPRI_XIndxROut             ,
    pvPRI_XIndxROutChckOn       , //자제 나올때 끌려나왔는지 체크
    pvPRI_XIndxROutChckOff      , //자제 다나았는지 체크.
    MAX_PSTN_MOTR9              ,

    //miDSP_ZDispr = 10 , //Dispensor Z Dispensor===================
    pvDSP_ZDisprWait        = 0 ,
    pvDSP_ZDisprCheck           ,
    pvDSP_ZDisprMove            ,
    pvDSP_ZDisprClean           ,
    MAX_PSTN_MOTR10             ,

    //Index T Stage Rear ============================================
    pvSTG_TStagWait         = 0 ,
    pvSTG_TStagWork             ,
    MAX_PSTN_MOTR11             ,

    //Index X Stage Rear ==============================================
    pvSTG_XStagWait         = 0 ,
    pvSTG_XStagCenter           ,
    MAX_PSTN_MOTR12             ,

    //================================================================
    pvSTG_YStagWait         = 0 ,
    pvSTG_YStagCenter           ,
    MAX_PSTN_MOTR13             ,

    pvPCK_XAlignWait        = 0 ,
    pvPCK_XAlignWork            ,
    MAX_PSTN_MOTR14             ,

    pvPCK_YAlignWait        = 0 ,
    pvPCK_YAlignWork            ,
    MAX_PSTN_MOTR15             ,

    //Unloader Z Elevator Front
    pvULD_ZElevFWait        = 0 ,
    pvULD_ZElevFWorkStartTop    ,
    pvULD_ZElevFWorkStartBtm    ,
    MAX_PSTN_MOTR16             ,

    //Unloader Z Elevator Rear
    pvULD_ZElevRWait        = 0 ,
    pvULD_ZElevRWorkStartTop    ,
    pvULD_ZElevRWorkStartBtm    ,
    MAX_PSTN_MOTR17             ,

    //Index Z Stage Rear
    pvSTG_ZStagWait         = 0 ,
    pvSTG_ZStagPick             ,
    pvSTG_ZStagCheck            ,
    MAX_PSTN_MOTR18

};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {
    //miLDR_ZElevF = 0  , //Loader Elevator Z Front
    piLDR_ZElevFWait        = 0 ,
    piLDR_ZElevFWorkStartBtm    ,
    piLDR_ZElevFWorkStartTop    ,
    piLDR_ZElevFWork            ,
    MAX_PSTN_ID_MOTR0           ,

    //miLDR_ZElevR = 1  , //Loader Elevator Z Rear
    piLDR_ZElevRWait        = 0 ,
    piLDR_ZElevRWorkStartBtm    ,
    piLDR_ZElevRWorkStartTop    ,
    piLDR_ZElevRWork            ,
    MAX_PSTN_ID_MOTR1           ,

    //miIDX_XIndxF = 2  , //Index X Front
    piIDX_XIndxFWait        = 0 ,
    piIDX_XIndxFGet             ,
    piIDX_XIndxFWorkStart       ,
    piIDX_XIndxFOut             ,
    piIDX_XIndxFWork            ,
    MAX_PSTN_ID_MOTR2           ,

    //miIDX_XIndxR = 3  , //Index Y Front
    piIDX_XIndxRWait        = 0 ,
    piIDX_XIndxRGet             ,
    piIDX_XIndxRWorkStart       ,
    piIDX_XIndxROut             ,
    piIDX_XIndxRWork            ,
    MAX_PSTN_ID_MOTR3           ,

    //miDSP_XDispr = 4  , //Dispensor X Dispensor=====================
    piDSP_XDisprWait        = 0 ,
    piDSP_XDisprVsStageCntr     ,
    piDSP_XDisprVsPad           ,
    piDSP_XDisprNdCheck         ,
    piDSP_XDisprNdClean         ,
    piDSP_XDisprHtCheck         ,
    piDSP_XDisprHtStage         , //자제 없을때 스테이지 높이 측정은 센터에서 체크하는 것이 아님.
    piDSP_XDisprStgFlatness1    , //스테이지 평탄도 측정 포인트 1~4
    piDSP_XDisprStgFlatness2    ,
    piDSP_XDisprStgFlatness3    ,
    piDSP_XDisprStgFlatness4    ,

    piDSP_XDisprNdStageCntr     ,
    piDSP_XDisprNdPad           ,
    piDSP_XDisprHtStageCntr     , //자제 있을때 자제 높이 측정 포인트.
    MAX_PSTN_ID_MOTR4           ,

    //miDSP_YDispr = 5  , //Dispensor Y Dispensor=====================
    piDSP_YDisprWait        = 0 ,
    piDSP_YDisprVsStageCntr     ,
    piDSP_YDisprVsPad           ,
    piDSP_YDisprNdCheck         ,
    piDSP_YDisprNdClean         ,
    piDSP_YDisprHtCheck         ,
    piDSP_YDisprHtStage         , //센터에서 체크하는 것이 아님.
    piDSP_YDisprStgFlatness1    , //스테이지 평탄도 측정 포인트 1~4
    piDSP_YDisprStgFlatness2    ,
    piDSP_YDisprStgFlatness3    ,
    piDSP_YDisprStgFlatness4    ,

    piDSP_YDisprNdStageCntr     ,
    piDSP_YDisprNdPad           ,
    piDSP_YDisprHtStageCntr     , //자제 있을때 자제 높이 측정 포인트.
    MAX_PSTN_ID_MOTR5           ,

    //miPCK_YPickr = 6  , //Picker Y Picker=======================
    piPCK_YPickrWait        = 0 ,
    piPCK_YPickrStageCntr       ,  //Fix Common
    piPCK_YPickrPick            ,
    piPCK_YPickrAlign           ,
    piPCK_YPickrHeight          ,
    piPCK_YPickrVision          ,
    piPCK_YPickrCheck           ,
    piPCK_YPickrPlace           ,
    piPCK_YPickrPlaceOfs        , //오프셑 먹은 플레이스 값...FOS모드
    MAX_PSTN_ID_MOTR6           ,

    //miPCK_ZPickr = 7  , //Picker Z Picker=======================
    piPCK_ZPickrWait        = 0 ,
    piPCK_ZPickrMove            ,
    piPCK_ZPickrPick            ,
    piPCK_ZPickrAlign           ,
    piPCK_ZPickrCheck           ,
    piPCK_ZPickrVsDefect        ,
    piPCK_ZPickrVsAlign         ,
    piPCK_ZPickrPlace           ,
    MAX_PSTN_ID_MOTR7           ,

    //miPRI_XIndxF = 8  , //Pre Index X Index Front
    piPRI_XIndxFWait        = 0 ,
    piPRI_XIndxFGet             ,
    piPRI_XIndxFOut             ,
    piPRI_XIndxFOutChckOn       , //자제 나올때 끌려나왔는지 체크
    piPRI_XIndxFOutChckOff      , //자제 다나았는지 체크.
    MAX_PSTN_ID_MOTR8           ,

    //miPRI_XIndxR = 9  , //Pre Index X Index Rear
    piPRI_XIndxRWait        = 0 ,
    piPRI_XIndxRGet             ,
    piPRI_XIndxROut             ,
    piPRI_XIndxROutChckOn       , //자제 나올때 끌려나왔는지 체크
    piPRI_XIndxROutChckOff      , //자제 다나았는지 체크.
    MAX_PSTN_ID_MOTR9           ,

    //miDSP_ZDispr = 10 , //Dispensor Z Dispensor===================
    piDSP_ZDisprWait        = 0 ,
    piDSP_ZDisprCheck           ,
    piDSP_ZDisprMove            ,
    piDSP_ZDisprClean           ,
    piDSP_ZDisprWork            ,
    MAX_PSTN_ID_MOTR10          ,

    //Index T Stage Rear ============================================
    piSTG_TStagWait         = 0 ,
    piSTG_TStagWork             ,
    MAX_PSTN_ID_MOTR11          ,

    //Index X Stage Rear ==============================================
    piSTG_XStagWait         = 0 ,
    piSTG_XStagCenter           ,
    MAX_PSTN_ID_MOTR12          ,

    //================================================================
    piSTG_YStagWait         = 0 ,
    piSTG_YStagCenter           ,
    MAX_PSTN_ID_MOTR13          ,

    piPCK_XAlignWait        = 0 ,
    piPCK_XAlignWork            ,
    MAX_PSTN_ID_MOTR14          ,

    piPCK_YAlignWait        = 0 ,
    piPCK_YAlignWork            ,
    MAX_PSTN_ID_MOTR15          ,

    //Unloader Z Elevator Front
    piULD_ZElevFWait        = 0 ,
    piULD_ZElevFWorkStartTop    ,
    piULD_ZElevFWorkStartBtm    ,
    piULD_ZElevFWork            ,
    MAX_PSTN_ID_MOTR16          ,

    //Unloader Z Elevator Rear
    piULD_ZElevRWait        = 0 ,
    piULD_ZElevRWorkStartTop    ,
    piULD_ZElevRWorkStartBtm    ,
    piULD_ZElevRWork            ,
    MAX_PSTN_ID_MOTR17          ,

    //Index Z Stage Rear
    piSTG_ZStagWait         = 0 ,
    piSTG_ZStagPick             ,
    piSTG_ZStagCheck            ,
    piSTG_ZStagWork             ,
    MAX_PSTN_ID_MOTR18
};


enum EN_MANUAL_CYCLE
{
    mcNoneCycle              =  0 ,
    mcAllHome                =  1 ,
    mcLDR_FHome              =  2 ,
    mcLDR_RHome              =  3 ,
    mcPRI_FHome              =  4 ,
    mcPRI_RHome              =  5 ,
    mcIDX_FHome              =  6 ,
    mcIDX_RHome              =  7 ,
    mcULD_FHome              =  8 ,
    mcULD_RHome              =  9 ,
    mcSTGHome                = 10 ,
    mcPCKHome                = 11 ,
    mcDSPHome                = 12 ,

    mcVisnCal                = 15 , //픽커가 얼라인에서 집어서 비젼검사하고 스테이지에 내려 놓고 At비젼으로 검사해봐서 오프셑이 얼마나 발생하는지 확인.
    mcLdrUldMoveWait         = 16 , //로더, 언로더 Wait 포지션으로 이동
    mcStgFlatness            = 17 , //스테이지 평탄도 측정

    mcLDR_FCycleWork         = 20 ,//작업위치로 이동.
    mcLDR_FWorkSttBtm        = 21 ,
    mcLDR_FWorkSttTop        = 22 ,
    mcLDR_FPitchUp           = 23 ,//피치업.
    mcLDR_FPitchDn           = 24 ,//피치다운.

    mcLDR_RCycleWork         = 30 ,//작업위치로 이동.
    mcLDR_RWorkSttBtm        = 31 ,//작업위치로 이동.
    mcLDR_RWorkSttTop        = 32 ,//작업위치로 이동.
    mcLDR_RPitchUp           = 33 ,//피치업.
    mcLDR_RPitchDn           = 34 ,//피치다운.

    mcPRI_FCycleWork         = 40 ,//자제 꺼내 오기.
    mcPRI_FIndxUpDn          = 41 ,//인덱스 업다운.

    mcPRI_RCycleWork         = 50 ,
    mcPRI_RIndxUpDn          = 51 ,//인덱스 업다운.

    mcIDX_FCycleGet          = 60 ,//받어오기.
    mcIDX_FCycleWork         = 61 ,//피치 이동.
    mcIDX_FCycleOut          = 62 ,//작업끝나고 내보내기.
    mcIDX_FIndxUpDn          = 63 ,//인덱스 업다운.
    mcIDX_FPitchRt           = 64 ,//피치이동.
    mcIDX_FPitchLt           = 65 ,

    mcIDX_RCycleGet          = 70 ,
    mcIDX_RCycleWork         = 71 ,
    mcIDX_RCycleOut          = 72 ,
    mcIDX_RIndxUpDn          = 73 ,
    mcIDX_RPitchRt           = 74 ,
    mcIDX_RPitchLt           = 75 ,

    mcULD_FCycleWork         = 80 ,//작업위치로 이동.`
    mcULD_FWorkSttTop        = 81 ,
    mcULD_FWorkSttBtm        = 82 ,
    mcULD_FPitchUp           = 83 ,//피치업.
    mcULD_FPitchDn           = 84 ,//피치다운.

    mcULD_RCycleWork         = 90 ,//작업위치로 이동.`
    mcULD_RWorkSttTop        = 91 ,
    mcULD_RWorkSttBtm        = 92 ,
    mcULD_RPitchUp           = 93 ,//피치업.
    mcULD_RPitchDn           = 94 ,//피치다운.

    mcSTGCycleUp             = 100 ,
    mcSTGCycleDn             = 101 ,


    mcPCKCyclePick           = 110,
    mcPCKCycleAlign          = 111,
    mcPCKCycleAlignPick      = 112,
    mcPCKCycleVisn           = 113,
    mcPCKCycleAttach         = 114,
    mcPCKCyclePush           = 115,
    mcPCKCycleManVsDefectPos = 116, //디펙트 비젼 위치로 이동.
    mcPCKCycleManVsAlignPos  = 117, //얼라인 비젼 위치로 이동.
    mcPCKCycleManStdCal      = 118, //픽커 기준점 잡기.
    mcPCKCycleManAlgnPlce    = 119, //피커 얼라인에 자재 놓기
    mcPCKCycleManAlignerCal  = 120, //비젼기준으로 얼라이너 조정.

    mcDSPCycleVisnBf         = 130, //비젼 검사전.
    mcDSPCycleDisp           = 131, //디스펜싱.
    mcDSPCycleVisnAt         = 132, //
    mcDSPCycleClean          = 133,
    mcDSPCycleManNdlCheck    = 134,
    mcDSPNdlCrFwBw           = 135, //니들 실린더.
    mcDSPCycleManStdCal      = 136, //디스펜서 기준점 잡기.
    mcDSPCycleVisnPos        = 137,
    mcDSPCycleNdlClean       = 138, //이건 없애자 중복 됨.
    mcDSPCycleManSlnChange   = 139, // 실린지 체인지 포지션으로 이동.



    MAX_MANUAL_CYCLE
};

enum EN_VISN_ID
{
    viPickerDefect = 0 ,
    viPickerAlign  = 1 ,
    viStageAlignBf = 2 ,
    viStageAlignAt = 3 ,
    viNiddleAlign  = 4 ,
    MAX_VISN_ID
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/eiETC_Door            ,
/*005*/ei005                 ,
/*006*/ei006                 ,
/*007*/eiPRT_Crash           ,
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
/*027*/eiPKG_Unknwn          ,
/*028*/ei028                 ,
/*029*/ei029                 , //--------------------------------- 장비공통에러.
/*030*/eiPRT_OverLoad        ,
/*031*/eiVSN_InspRangeOver   , // 현재 안쓰고 있는 에러
/*032*/eiVSN_InspNG          ,
/*033*/ei033                 ,
/*034*/eiPRT_BoatMiss        ,
/*035*/ei035                 ,
/*036*/eiSTG_RemovePkg       ,
/*037*/ei037                 ,
/*038*/eiPCK_PickMiss        ,
/*049*/ei039                 ,
/*040*/eiPRT_TempErr         , // 현재 안쓰고 있는 에러
/*041*/ei041                 ,
/*042*/eiPRT_CheckErr        , //체크 센서 터치 에러.
/*043*/ei043                 ,
/*044*/eiPCK_VaccSensor      , //픽 동작인데 배큠센서 온되어서 집을수 없는경우
/*045*/eiDSP_Amount          , //디스펜서 잔양 부족 에러.
/*046*/eiSTG_PickMiss        ,
/*047*/eiPCK_LoadCell        ,
/*048*/eiETC_RS232           ,
/*049*/ei049                 ,
/*050*/eiULD_MgzSupply       , //Index작업 하려는데 Unloader에 자리 없으면...
MAX_ERR
};


#endif






