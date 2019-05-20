#ifndef UTILDEFINEH
#define UTILDEFINEH

//sunsun 로더 언로더 인덱스바 투스타트 관련
//소켓 플레이스 및 쉬프트 소켓 플레이스 인포 등등.
//언로더 세퍼레이터 처음 켤때 뒤로 빠지는것.
//랏엔드시에 스테이지 회전 하는것.
//
#define MODEL "HRM-930b"
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
    rsDispensorFt  = 0 , //com1     PortNo = 1 , BaudRate = 19200 , ByteSize = 8, StopBits = 0 , Parity = None
    rsHeightSensor = 1 , //com2     PortNo = 2 , BaudRate =  9600 , ByteSize = 8, StopBits = 0 , Parity = None
    rsLoadCell     = 2 , //com3     PortNo = 3 , BaudRate =  9600 , ByteSize = 8, StopBits = 2 , Parity =  Odd
    rsUV           = 3 , //com4     PortNo = 4 , BaudRate =  9600 , ByteSize = 8, StopBits = 0 , Parity = None
    rsDispensorRr  = 4 , //com5     PortNo = 5 , BaudRate = 19200 , ByteSize = 8, StopBits = 0 , Parity = None

    MAX_RS232
};


//enum EN_VAC_ERR_STAT { //현재 랏 런 하고 있는 상태.
//    vsRetry        = 0 , //에러났던 곳을 다시 픽하고 안찝히면 다시 에러.
//    vsErrSkipPick  = 1 , //에러 난 곳 스킵.
//    vsErrLotEnd    = 2 , //랏엔드.
//};

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};


//파트 아이디.
enum EN_PART_ID {
    piLDR_F  = 0 , //Loader Fornt
    piLDR_R      , //Loader Rear
    piIDX        , //Rear Index
    piRTL        , //Right Tool
    piLTL        , //Left Tool
    piSTG        , //Stage
    piULD        , //Unloader

    MAX_PART
};

enum EN_ARAY_ID {
    riLDR_F  =0 , //Loader Fornt Top
    riLDR_R     , //Loader Rear  Top
    riIDX       , //Rear Index
    riRTT       , //Rotator
    riTRF       , //Transfer
    riPRL       , //Pre Rail
    riSTG       , //Stage Work Zone.
    riWKO       , //Work Out
    riPTL       , //포스트레일.
    riULD       , //UnLoader

    MAX_ARAY
};

enum EN_CHIP_STAT {  //시퀜스 상의 칩으 상태들.
    csRetFail    = -1 , //이건 그냥 리턴에서만 쓰고 평소시에는 쓰면 안됌.
    csNone       =  0 ,
    csUnkwn           ,
    csMask            , //Test모드 에서...
    csAttach          ,
    csEmpty           ,
    csWork            ,
    csWorkEnd         ,
    csDetect          , //Loader(F,R) 자재 유무 스캔
    csMidLOri         , //STG 미들블럭 오리엔테이션.
    csMidLAlign       , //STG 미들블럭 얼라인.
    csMidRHeight      , //STG 미들블럭 높이측정.마름모로 함.
    csCmsLDisp1       , //STG 첫번째 디스펜싱 -> 리어 디스펜서로 되어있음
    csCmsLDisp2       , //STG 두번째 디스펜싱 -> 프론트 디스펜서로 되어있음
    csCmsLOri         , //RTT 시모스 오리엔테이션.
    csCmsRNeed        , //STG 시모스 안착 필요.
    csCmsLAlign       , //STG 시모스 왼쪽 비젼으로 프리얼라인.
    csCmsRPreAlign    , //STG 시모스 높이 측정을 정확히 하기 위해 프리 얼라인을 해놓음.
    csCmsRHeight      , //STG 시모스 오른쪽 높이측정기로 높이 측정.
    csCmsRAlign       , //STG 시모스 오른쪽 비젼으로 정밀얼라인 후 갭검사 위치까지 붙임.
    csCmsRZHeight     , //높이 측정하여 Z축만 이동.
    csCmsREndAlign    , //STG 시모스 비전 완료 검사.
    csCmsREndHeight   , //STG 시모스 완료 검사.
    csCmsLTopUV1      , //STG 상부 유부이 조사.
    csCmsLTopUV2      , //STG 상부 유부이 조사.

    csCmsFix          , //STG 굳히기.
    csCmsRFixAlign    , //STG 시모스 비전 완료 검사.
    csCmsRFixHeight   , //STG 시모스 완료 검사.
    csMidREndHeight   , //STG 모든 시모스 합착 완료후 최종 높이 측정 검사.////
    csCmsLHeight      , //로테이터에서 자제 높이만 측정해서 다시 로더에 넣는 모드일때.... 로테이터..
    MAX_CHIP_STAT

};

//mi<파트명3자리>_<축방샹1자리><부가명칭4>
enum EN_MOTR_ID {

    miLTL_XGenRr = 0  , // Pana X Linear Top Stage Rear
    miLTL_XGenFt = 1  , // Pana X Linear Top Stage Front
    miLTL_YDispr = 2  , // Pana Y Linear Top Stage Front
    miLTL_ZDispr = 3  , // JE   Z Dispensor
    miRTL_XGenRr = 4  , // X Linear Top Stage Rear
    miRTL_XGenFt = 5  , // X Linear Top Stage Front
    miRTL_YVisn  = 6  , // Y Linear Top Stage Align
    miRTL_ZVisn  = 7  , // Z Align Vision
    miLTL_YVisn  = 8  , // Y Orientation Vision
    miRTL_YTrspr = 9  , // Y Transfer Uint
    miSTG_XStage = 10 , // X Linear Bottom Stage
    miSTG_YStage = 11 , // Y Linear Bottom Stage
    miLTL_ZVisn  = 12 , // Z Orientation Vision
    miIDX_XIndxR = 13 , // Index X Index Rear
    miRTL_ZTrsfr = 14 , // Stage Z Transfer
    miRTL_YConvs = 15 , // Right COMS Transfer Conversion
    miIDX_TRotat = 16 , // Rail Rotator
    miSTG_YConvs = 17 , // Rail Conversion
    miLDR_YConvR = 18 , // COMS Loader Conversion
    miLDR_ZElevR = 19 , // COMS Loader Z Elevator
    miLDR_ZElevF = 20 , // Loader Z Elevator
    miSpare1     = 21 , // Spare1
    miULD_ZElevt = 22 , // Unloader Z Elevator
    miSpare2     = 23 , // Spare2

    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiLTL_MGrperDnUp = 0  , // Griper Main Up/Dn
    aiLTL_SGrperDnUp = 1  , // Griper Sub  Up/Dn
    aiLTL_WrkPshDnUp = 2  , // Work Pusher
    aiRTL_LWkPshUpDn = 3  , // Pull Index UpDn // aiRTL_PulPshUpDn
    aiRTL_RWkPshUpDn = 4  , // Out Index UpDn  // aiRTL_OutPshUpDn
    aiIDX_RoatorUpDn = 5  ,
    aiLTL_DisprFDnUp = 6  , // 디스펜서 실린지 첫 번째 (도면상 10cc)
    aiLTL_DisprRDnUp = 7  , // 디스펜서 실린지 두 번째 (도면상 30cc)
    aiLTL_UVCureDnUp = 8  , // UV 경화기
    aiSTG_Fix1stFwBw = 9  , // Rail Middle Block Fix First  FwBw
    aiSTG_Fix2ndFwBw = 10 , // Rail Middle Block Fix Second FwBw
    aiSTG_Fix3rdFwBw = 11 , // Rail Middle Block Fix Third  FwBw
    aiSTG_Fix4thFwBw = 12 , // Rail Middle Block Fix Fourth FwBw
    aiSTG_Fix5thFwBw = 13 , // Rail Middle Block Fix Fifth  FwBw
    aiLTL_GriperFwBW = 14 , // Left Tool Griper 전진 후진
//  aiSTG_FIonizer   = 14 , // 미정
//  aiSTG_RIonizer   = 15 , // 미정

    MAX_ACTR

};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {

    //Ft - Front , Rr - Rear 으로 통일 함.
    x0000 =  0 , xETC_LStartSw        =  0 , //  xETC_StartSw // 기존 Fos에 있던 것!
    x0001 =  1 , xETC_LStopSw         =  1 , //  xETC_StopSw
    x0002 =  2 , xETC_LResetSw        =  2 , //  xETC_ResetSw
    x0003 =  3 , xETC_LAirSw          =  3 , //  xETC_AirSw
    x0004 =  4 , xETC_LInitSw         =  4 , //  xETC_InitSw
    x0005 =  5 , xETC_LEmgSw          =  5 , //  xETC_EmgSw
    x0006 =  6 , xETC_DoorFt          =  6 , //  xETC_MainPower
    x0007 =  7 , xETC_DoorRr          =  7 , //  xETC_MainAir
    x0008 =  8 , xETC_DoorLt          =  8 , //  xETC_DoorFt
    x0009 =  9 , xETC_DoorRt          =  9 , //  xETC_DoorRr
    x000A = 10 , xETC_MainAirChk      = 10 , //  xETC_DoorLt
    x000B = 11 , xETC_ServoOn         = 11 , //  xETC_DoorRt
    x000C = 12 , xLDR_FMidDetectR     = 12 , //  xLDR_FMgzDetect1
    x000D = 13 , xLDR_FMidDetectL     = 13 , //  xLDR_FMgzDetect2
    x000E = 14 , xLDR_RPcbDetectR     = 14 , //  xLDR_RMgzDetect1
    x000F = 15 , xLDR_RPcbDetectL     = 15 , //  xLDR_RMgzDetect2

    x0010 = 16 , xLDR_Detect1         = 16 , //  xULD_FMgzDetect1
    x0011 = 17 , xPRL_Detect2         = 17 , //  xULD_FMgzDetect2
    x0012 = 18 , xSTG_Detect3         = 18 , //  xULD_RMgzDetect1
    x0013 = 19 , xSTG_Detect4         = 19 , //  xULD_RMgzDetect2
    x0014 = 20 , xWKO_Detect5         = 20 , //  xIDX_FIndxUp
    x0015 = 21 , xPTL_Detect6         = 21 , //  xIDX_FIndxDn
    x0016 = 22 , xULD_Detect7         = 22 , //  xIDX_FIndxNotOvLd
    x0017 = 23 , x023                 = 23 , //  xIDX_RIndxUp
    x0018 = 24 , xSTG_Fix1Fw          = 24 , //  xIDX_RIndxDn
    x0019 = 25 , x025                 = 25 , //  xIDX_RIndxNotOvLd
    x001A = 26 , xSTG_Fix2Fw          = 26 , //  xPRI_FIndxUp
    x001B = 27 , x027                 = 27 , //  xPRI_FIndxDn
    x001C = 28 , xSTG_Fix3Fw          = 28 , //  xPRI_FIndxNotOvLd
    x001D = 29 , x029                 = 29 , //  xPRI_RIndxUp
    x001E = 30 , xSTG_Fix4Fw          = 30 , //  xPRI_RIndxDn
    x001F = 31 , x031                 = 31 , //  xPRI_RIndxNotOvLd

    x0020 = 32 , xSTG_Fix5Fw          = 32 , // xULD_FPushUp
    x0021 = 33 , xLTL_GrprSnsr        = 33 , // xULD_FPushDn
    x0022 = 34 , xLTL_MGrprUp         = 34 , // xULD_FPushLt
    x0023 = 35 , xLTL_MGrprDn         = 35 , // xULD_FPushRt
    x0024 = 36 , xLTL_SGrprUp         = 36 , // xULD_RPushUp
    x0025 = 37 , xLTL_SGrprDn         = 37 , // xULD_RPushDn
    x0026 = 38 , xLTL_WrkPshUp        = 38 , // xULD_RPushLt
    x0027 = 39 , xLTL_WrkPshDn        = 39 , // xULD_RPushRt
    x0028 = 40 , xLTL_WrkPshSnsr      = 40 , // xPRI_FDetect1
    x0029 = 41 , xLTL_DipsrFUp        = 41 , // xPRI_FDetect2
    x002A = 42 , xLTL_DipsrFDn        = 42 , // xIDX_FWrkDetect3
    x002B = 43 , xLTL_DipsrRUp        = 43 , // xIDX_FDetect4
    x002C = 44 , xLTL_DipsrRDn        = 44 , // xULD_FDetect5
    x002D = 45 , xLTL_UVSpotUp        = 45 , // xPRI_RDetect1
    x002E = 46 , xLTL_UVSpotDn        = 46 , // xPRI_RDetect2
    x002F = 47 , xSTG_StgVcum1        = 47 , // xIDX_RWrkDetect3

    x0030 = 48 , xSTG_StgVcum2        = 48 ,
    x0031 = 49 , xLTL_GriperFw        = 49 ,
    x0032 = 50 , xLTL_GriperBw        = 50 ,
    x0033 = 51 , x051                 = 51 ,
    x0034 = 52 , x052                 = 52 ,
    x0035 = 53 , x053                 = 53 ,
    x0036 = 54 , x054                 = 54 ,
    x0037 = 55 , x055                 = 55 ,
    x0038 = 56 , x056                 = 56 ,
    x0039 = 57 , x057                 = 57 ,
    x003A = 58 , x058                 = 58 ,
    x003B = 59 , x059                 = 59 ,
    x003C = 60 , x060                 = 60 ,
    x003D = 61 , x061                 = 61 ,
    x003E = 62 , x062                 = 62 ,
    x003F = 63 , x063                 = 63 ,

    x0040 = 64 , xETC_RStartSw        = 64 ,
    x0041 = 65 , xETC_RStopSw         = 65 ,
    x0042 = 66 , xETC_RResetSw        = 66 ,
    x0043 = 67 , xETC_RAirSw          = 67 ,
    x0044 = 68 , xETC_RInitSw         = 68 ,
    x0045 = 69 , xIDX_Detect          = 69 , // CMOS Detect
    x0046 = 70 , xIDX_RotatorUp       = 70 ,
    x0047 = 71 , xIDX_RotatorDn       = 71 ,
    x0048 = 72 , xRTL_LWrkIndexUp     = 72 , // xRTL_PullIndexUp (기존)
    x0049 = 73 , xRTL_LWrkIndexDn     = 73 , // xRTL_PullIndexDn (기존)
    x004A = 74 , xRTL_RWrkIndexUp     = 74 , // xRTL_OutIndexUp  (기존)
    x004B = 75 , xRTL_RwrkIndexDn     = 75 , // xRTL_OutIndexDn  (기존)
    x004C = 76 , xRTL_PullDetect      = 76 ,
    x004D = 77 , xRTL_OutDetect       = 77 ,
    x004E = 78 , xIDX_IdxVcumSnr      = 78 ,
    x004F = 79 , xSTG_StgVcum3        = 79 , // Rotator Vacuum Sensor

    x0050 = 80 , xRTL_TransfrVcum1    = 80 , // Transfer Vacuum
    x0051 = 81 , xRTL_TransfrVcum2    = 81 ,
    x0052 = 82 , x082                 = 82 , //xULD_Detect6
    x0053 = 83 , x083                 = 83 ,
    x0054 = 84 , xETC_NdleNotCheck    = 84 ,
    x0055 = 85 , xRTL_TrsfJigDetect   = 85 , // 트랜스퍼 쪽 지그 감지 센서
    x0056 = 86 , xIDX_PkgCrashDetect  = 86 , // 인덱스 바 자재 있을 시 충돌 감지 센서
    x0057 = 87 , xULD_MidDetect       = 87 ,
    x0058 = 88 , x088                 = 88 ,
    x0059 = 89 , x089                 = 89 ,
    x005A = 90 , x090                 = 90 ,
    x005B = 91 , x091                 = 91 ,
    x005C = 92 , x092                 = 92 ,
    x005D = 93 , x093                 = 93 ,
    x005E = 94 , x094                 = 94 ,
    x005F = 95 , x095                 = 95 ,

    x0060 = 96 , xVisnL_Ready         = 96  , // Transfer Vacuum
    x0061 = 97 , xVisnL_Busy          = 97  ,
    x0062 = 98 , xVisnL_InspOk        = 98  ,
    x0063 = 99 , xVisnL_Spare1        = 99  ,
    x0064 = 100, xVisnL_Spare2        = 100 ,
    x0065 = 101, x101                 = 101 ,
    x0066 = 102, x102                 = 102 ,
    x0067 = 103, x103                 = 103 ,
    x0068 = 104, xVisnR_Ready         = 104 ,
    x0069 = 105, xVisnR_Busy          = 105 ,
    x006A = 106, xVisnR_InspOk        = 106 ,
    x006B = 107, xVisnR_Spare1        = 107 ,
    x006C = 108, xVisnR_Spare2        = 108 ,
    x006D = 109, x109                 = 109 ,
    x006E = 110, x110                 = 110 ,
    x006F = 111, x111                 = 111 ,

    MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>     자리
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리> 동
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_LStartLp        =  0 , //
    y0001 =  1 , yETC_LStopLp         =  1 , //
    y0002 =  2 , yETC_LResetLp        =  2 , //
    y0003 =  3 , yETC_LAirLp          =  3 , //
    y0004 =  4 , yETC_LInitLp         =  4 , //
    y0005 =  5 , yETC_MainAirSol      =  5 , //
    y0006 =  6 , yLTL_Dispr2OnOff     =  6 , //
    y0007 =  7 , y007                 =  7 , //
    y0008 =  8 , yETC_TwRedLp         =  8 , //
    y0009 =  9 , yETC_TwYelLp         =  9 , //
    y000A = 10 , yETC_TwGrnLp         = 10 , //
    y000B = 11 , yETC_TwBzz           = 11 , //
    y000C = 12 , yLTL_MGrprDnUp       = 12 , //
    y000D = 13 , yLTL_SGrprDnUp       = 13 , //
    y000E = 14 , yLTL_WrkPshDnUp     = 14 , //
    y000F = 15 , yRTL_LWkIdxDnUp     = 15 , // yRTL_PulIdxUpDn (기존)
    //Ft - Front , Rr - Rear 으로 통일 함.
    y0010 = 16 , yRTL_RWkIdxDnUp     = 16 , // yRTL_OutIdxUpDn (기존)
    y0011 = 17 , yIDX_RotatorUpDn     = 17 , //
    y0012 = 18 , yLTL_DisprFDn        = 18 , //
    y0013 = 19 , yLTL_DisprRDn        = 19 , //
    y0014 = 20 , yLTL_UVCuringDn      = 20 , //
    y0015 = 21 , yIDX_IDXIonizer      = 21 , //
    y0016 = 22 , yTRS_TRSIonizer      = 22 , //
    y0017 = 23 , ySTG_UVAirSol        = 23 , //
    y0018 = 24 , yLTL_CleanOn         = 24 , //
    y0019 = 25 , yLTL_GriperFwBW      = 25 , //
    y001A = 26 , yETC_LightOnOff      = 26 , // // 형광등
    y001B = 27 , yLTL_Dispr1OnOff     = 27 , //
    y001C = 28 , yLTL_DisprZBrakeOff  = 28 , //
    y001D = 29 , yLTL_Visn1ZBrakeOff  = 29 , // Left Tool Vision Z Brake
    y001E = 30 , yLDR_RearZBrakeOff   = 30 , //
    y001F = 31 , yLDR_FrontZBrakeOff  = 31 , //

    y0020 = 32 , yETC_RStartLp        = 32 , //
    y0021 = 33 , yETC_RStopLp         = 33 , //
    y0022 = 34 , yETC_RResetLp        = 34 , //
    y0023 = 35 , yETC_RAirLp          = 35 , //
    y0024 = 36 , yETC_RInitLp         = 36 , //
    y0025 = 37 , yRTL_WeightOff       = 37 , //
    y0026 = 38 , y038                 = 38 , //
    y0027 = 39 , y039                 = 39 , //
    y0028 = 40 , ySTG_Fix1FwBw        = 40 , //
    y0029 = 41 , ySTG_Fix2FwBw        = 41 , //
    y002A = 42 , ySTG_Fix3FwBw        = 42 , //
    y002B = 43 , ySTG_Fix4FwBw        = 43 , //
    y002C = 44 , ySTG_Fix5FwBw        = 44 , //
    y002D = 45 , yIDX_RttPumpOn       = 45 , // Rotator Vacuum Pump On/Off
    y002E = 46 , ySTG_VcumSol         = 46 , // 프로그램 켜면 무조건 켜면 된다.  yIDX_RtatPumpOn ySTG_VcumPump1On ySTG_StgVcumPump2On 세개쓸려면 온시켜야함.
    y002F = 47 , ySTG_VcumPump1On     = 47 , //

    y0030 = 48 , ySTG_VcumPump2On     = 48 , //
    y0031 = 49 , yLTL_UVHumeSuction   = 49 , //
    y0032 = 50 , yIDX_IdxVac          = 50 , //
    y0033 = 51 , yRTL_TransfrVac1     = 51 , //
    y0034 = 52 , yRTL_TransfrVac2     = 52 , //
    y0035 = 53 , yRTL_Visn2ZBrakeOff  = 53 , // Right Tool Vision Z Brake
    y0036 = 54 , yRTL_TrsfZBrakeOff   = 54 , //
    y0037 = 55 , yULD_ZBrakeOff       = 55 , //
    y0038 = 56 , ySTG_VcumPump3On     = 56 , //
    y0039 = 57 , y057                 = 57 , //
    y003A = 58 , y058                 = 58 , //
    y003B = 59 , y059                 = 59 , //
    y003C = 60 , y060                 = 60 , //
    y003D = 61 , y061                 = 61 , //
    y003E = 62 , y062                 = 62 , //
    y003F = 63 , y063                 = 63 , //

    y0040 = 64 , yVisnL_Command       = 64 , //Insp = Inspection
    y0041 = 65 , yVisnL_JobChange     = 65 , //
    y0042 = 66 , yVisnL_Reset         = 66 , //
    y0043 = 67 , yVisnL_Spare1        = 67 , //
    y0044 = 68 , yVisnL_InspStart     = 68 , //
    y0045 = 69 , y069                 = 69 , //
    y0046 = 70 , y070                 = 70 , //
    y0047 = 71 , y071                 = 71 , //
    y0048 = 72 , yVisnR_Command       = 72 , //
    y0049 = 73 , yVisnR_JobChange     = 73 , //
    y004A = 74 , yVisnR_Reset         = 74 , //
    y004B = 75 , yVisnR_Live          = 75 , //
    y004C = 76 , yVisnR_InspStart     = 76 , //
    y004D = 77 , y077                 = 77 , //
    y004E = 78 , y078                 = 78 , //
    y004F = 79 , y079                 = 79 , //

    MAX_OUTPUT
};


enum EN_PSTN_VALUE {

    //miLTL_XGenRr = 1  , //Pana X Linear Top Stage Rear (홈센서가 리어 쪽에 붙어 있어서 이 축을 마스터 축으로 설정)
    pvLTL_XWait            = 0 ,//대기 위치.
    pvLTL_XGripGrip1           ,//첫번째 미들블럭 그립 위치
    pvLTL_XGripGrip2           ,//두번째 미들블럭 그립 위치
    pvLTL_XGripPull            ,//미들블럭 잡아 빼는 위치.
    pvLTL_XIndxBack            ,//미들블럭 뒤로 돌아가는 위치.
    pvLTL_XIndxWork            ,//미들블럭 스테이지 워크존까지 위치.
    pvLTL_XDispClean           ,//니들 청소 위치.
    pvLTL_XDispFtCheck         ,//니들 높이측정 체크센서 위치.
    pvLTL_XDispRrCheck         ,//니들 높이측정 체크센서 위치.
    pvLTL_XDispUVCheck         ,//유브이 높이측정 체크센서 위치.
    pvLTL_XVisnHtCheck         ,//높이 측정기 터체센서 위치
    pvLTL_XVisnCheck           ,//공용 비젼 터치센서 센터 위치.
    pvLTL_XVisnCmsOri          ,//미들블럭 역삽 검사.
    pvLTL_XWorkOut             ,//미들블럭 워크 아웃까지 미는 위치
    pvLTL_XVisnRttCnt          ,//비젼 로테이터 센터.
    MAX_PSTN_MOTR0             ,

    pvLTL_XGenFtWait       = 0 ,//대기 위치.
    MAX_PSTN_MOTR1             ,

    //miLTL_YDispr = 2  ,
    pvLTL_YDispWait        = 0 , //대기 위치.
    pvLTL_YDispClean           , //니들 청소 위치.
    pvLTL_YDispFtCheck         , //니들 높이측정 체크센서 위치.
    pvLTL_YDispRrCheck         , //니들 높이측정 체크센서 위치.
    pvLTL_YDispUVCheck         , //유부이 높이측정 체크센서 위치.
    MAX_PSTN_MOTR2             ,

    //miLTL_ZDispr = 3  , //JE   Z Dispensor
    pvLTL_ZDispWait        = 0 , //대기 위치.
    pvLTL_ZDispFtCheck         , //Z축 니들 높이측정 체크센서 위치
    pvLTL_ZDispRrCheck         , //Z축 니들 높이측정 체크센서 위치
    pvLTL_ZDispUVWork          , //Z축 유브이 조사 높이.
    pvLTL_ZDispClean           , //Z축 니들 청소 위치.
    pvLTL_ZDispUVTest          , //Z축 UV 조도계 테스트 높이
    MAX_PSTN_MOTR3             ,

    //miRTL_XGenRr = 4  , // X Linear Top Stage Rear
    pvRTL_XWait            = 0 , // 라이트 갠트리 리어.
    pvRTL_XLIdxOutStt          , // 자제 배출 시작 위치.
    pvRTL_XLIdxOutEnd          , // 자제 배출 끝 위치.
    pvRTL_XRIdxOutStt          , // 자제 배출 시작 위치.
    pvRTL_XRIdxOutEnd          , // 자제 배출 끝 위치.
    pvRTL_XTrsfCheck           , // 스테이지 체크 센서 위치.  로테
    pvRTL_XTrsfRotator         , // 로테이터에서 집는 위치.
    pvRTL_XTrsfRotatorRvs      , // 로테이터에서 뒤집힌 자제 집는 위치.
    pvRTL_XVisnCheck           , // 비전 체크센서 위치.
    pvRTL_XVisnStgCnt          , // 스테이지의 센터를 세팅하고 비
    pvRTL_XVisnHtCheck         , // 높이측정기 체크센서 위치.
    pvRTL_XVisnCfCheck         , // 공초점센서 체크센서 위치
    //poRTL_XTrsfPlceOfs         , // 짝짝이 셉자제의 경우 트렌스퍼가 X축 오프셑을 먹여서 집어야 하기에 어쩔수 없이 플레이스는 자동인데 넣었음.
    MAX_PSTN_MOTR4             ,

    //miRTL_XGenFt = 5  , // X Linear Top Stage Front
    pvRTL_XGenFtWait       = 0 , // 대기 위치.
    MAX_PSTN_MOTR5             ,

    //miRTL_YVisn  = 6  , // Y Linear Top Stage Align
    pvRTL_YVisnWait        = 0 ,
    pvRTL_YVisnCheck           , // 비전 체크센서 위치.
    pvRTL_YVisnStgCnt          , // 스테이지의 센터를 세팅하고 비
    pvRTL_YVisnHtCheck         , // 높이측정기 체크센서 위치.
    pvRTL_YVisnCfCheck         , // 공초점 센서 체크센서 위치
    MAX_PSTN_MOTR6             ,

    //miRTL_ZVisn  = 7  , // Z Align Vision
    pvRTL_ZVisnWait        = 0 ,
    pvRTL_ZVisnCheckWork       , //체크 센서위치에서의 포커스를 맞
    pvRTL_ZVisnCmsAlign        ,
    pvRTL_ZVisnCmsConfocal     ,
    MAX_PSTN_MOTR7             ,

    //miLTL_YVisn  = 8  , // Y Orientation Vision
    pvLTL_YVisnWait        = 0 , //
    pvLTL_YVisnHtCheck         , //높이 측정기 터체센서 위치.
    pvLTL_YVisnCheck           , //공용 비젼 터치센서 센터 위치.
    pvLTL_YVisnCmsOri          , //미들블럭 역삽 검사.
    pvLTL_YVisnRttCnt          ,
    MAX_PSTN_MOTR8             ,

    //miRTL_YTrspr = 9  , // Y Transfer Uint
    pvRTL_YTrsfWait        = 0 , //
    pvRTL_YTrsfCheck           , //
    pvRTL_YTrsfRoatator        , //
    pvRTL_YTrsfRotatorRvs      , // 로테이터에서 뒤집힌 자제 집는 위치.
    MAX_PSTN_MOTR9             ,

    // miSTG_XStage = 10 , // X Linear Bottom Stage
    pvSTG_XStageWait       = 0 , // 대기 위치.
    pvSTG_XStageCntr           , // 비젼으로 확인하여 터치센서의 
    MAX_PSTN_MOTR10            ,

    // miSTG_YStage = 11 , // Y Linear Bottom Stage
    pvSTG_YStageWait       = 0 , // 대기 위치.
    pvSTG_YStageCntr           , // 비젼으로 확인 하여 레일폭의 센
    MAX_PSTN_MOTR11            ,

    //miLTL_ZVisn  = 12 , // Z Orientation Vision
    pvLTL_ZVisnWait        = 0 , // 대기 위치.
    pvLTL_ZVisnCheck           , //체크 센서위치에서의 포커스를 맞
    pvLTL_ZVisnMid             , //오리엔테이션
    pvLTL_ZVisnCmsOri          , //오리엔테이션
    pvLTL_ZVisnCmsAlign        , //얼라인.
    MAX_PSTN_MOTR12            ,

    // miIDX_XIndxR = 13 , // Index X Index Rear
    pvIDX_XIndxRWait       = 0 , // 대기 위치.
    pvIDX_XIndxRGetWait        , // 작업 대기 위치
    pvIDX_XIndxRCheck1         , // CMOS 로더에서 CMOS Index 공급
    pvIDX_XIndxRCheck2         , // CMOS 로더에서 CMOS Index 공급
    pvIDX_XIndxRRotator        , // CMOS 로테이트 존까지의 이송 위
    poIDX_XIndxRGet            , //체크된 위치에서 뜨는 위치까지 오프셑.
    MAX_PSTN_MOTR13            ,

    // miRTL_ZTrsfr = 14 , // Stage Z Transfer
    pvRTL_ZTrsfrWait       = 0 , // 대기 위치.
    pvRTL_ZTrsfrPick           , // 로테이트 존에 있는 CMOS 자재
    pvRTL_ZTrsfrPlace          , // COMS 자재를 스테이지 위에 놓거
    MAX_PSTN_MOTR14            ,

    //miRTL_YConvs = 15 , // Right COMS Transfer Conversion
    pvRTL_YConvsWait       = 0 ,
    pvRTL_YConvsWork1st        ,
    pvRTL_YConvsWork2nd        ,
    MAX_PSTN_MOTR15            ,

    //miIDX_TRotat = 16 , // Rail Rotator
    pvIDX_TRotatWait       = 0 , // 대기 위치.
    pvIDX_TRotatRotate         , // 180도 돌린위치.
    pvIDX_TRotatNormal         , // 안돌린 위치.
    MAX_PSTN_MOTR16            ,

    //miSTG_YConvs = 17 , // Rail Conversion
    pvSTG_YConvsWait       = 0 , // 대기 위치.
    pvSTG_YConvsWork           , // 레일 폭 조절 위치
    MAX_PSTN_MOTR17            ,

    //miLDR_YConvR = 18 , // COMS Loader Conversion
    pvLDR_YConvRWait       = 0 , // 대기 위치.
    pvLDR_YConvRWork           ,
    MAX_PSTN_MOTR18            ,

    // miLDR_ZElevR = 19 , // COMS Loader Z Elevator
    pvLDR_ZElevRWait       = 0 ,
    pvLDR_ZElevRPickFrst       , //워크일때는 워크포지션에 있다가
    pvLDR_ZElevRPlceFrst       , //플레이스일때는 플레이스 포지션
    pvLDR_ZElevRCheckFrst      , //와이드셉자제의 경우 2mm 높이는 바람에 Pick위치에서 센서 감지가 안된다.
    MAX_PSTN_MOTR19            ,

    // miLDR_ZElevF = 20 , // Loader Z Elevator
    pvLDR_ZElevFWait       = 0 ,
    pvLDR_ZElevFPickFrst       , //워크일때는 워크포지션에 있다가
    MAX_PSTN_MOTR20            ,

    //miSpare1     = 21 , // Spare1
    pvSpare1Wait           = 0 ,
    MAX_PSTN_MOTR21            ,

    // miULD_ZElevt = 22 , // Unloader Z Elevator
    pvULD_ZElevtWait       = 0 ,
    pvULD_ZElevtPickFrst       ,
    //pvULD_ZElevFPlceFrst       , //플레이스일때는 플레이스 포지션
    pvULD_ZElevtPlce           ,
    MAX_PSTN_MOTR22            ,

    //miSpare2     = 23 ,
    pvSpare2Wait           = 0 ,
    MAX_PSTN_MOTR23
};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {
    //miLTL_XGenRr = 0  , //Pana X Linear Top Stage Rear  (miLTL_XGenFt 동기화)
    piLTL_XWait            = 0 ,//대기 위치.      piLTL_XWait
    piLTL_XGripGrip1           ,//미들블럭 그립 위치1
    piLTL_XGripGrip2           ,//미들블럭 그립 위치2
    piLTL_XGripPull            ,//미들블럭 잡아 빼는 위치.
    piLTL_XIndxBack            ,//미들블럭 뒤로 돌아가는 위치.
    piLTL_XIndxWork            ,//미들블럭 스테이지 워크존까지 미는 위치.
    piLTL_XDispClean           ,//니들 청소 위치.
    piLTL_XDispFtCheck         ,//니들 높이측정 체크센서 위치.
    piLTL_XDispRrCheck         ,//니들 높이측정 체크센서 위치.
    piLTL_XDispUVCheck         ,//유브이 높이측정 체크센서 위치.
    piLTL_XVisnHtCheck         ,//높이 측정기 터체센서 위치
    piLTL_XVisnCheck           ,//공용 비젼 터치센서 센터 위치.
    piLTL_XVisnCmsOri          ,//미들블럭 역삽 검사.
    piLTL_XWorkOut             ,//미들블럭 워크 아웃까지 미는 위치.
    piLTL_XVisnRttCnt          ,//비전 로테이터 센터.
    MAX_PSTN_ID_MOTR0          ,

    //miLTL_XGenFt = 1  ,
    piLTL_XGenFtWait       = 0 ,//대기 위치.
    MAX_PSTN_ID_MOTR1          ,



    //miLTL_YDispr = 2  ,
    piLTL_YDispWait        = 0 , //대기 위치.
    piLTL_YDispClean           ,//니들 청소 위치.
    piLTL_YDispFtCheck         ,//니들 높이측정 체크센서 위치.
    piLTL_YDispRrCheck         ,//니들 높이측정 체크센서 위치.
    piLTL_YDispUVCheck         ,//유부이 높이측정 체크센서 위치.
    MAX_PSTN_ID_MOTR2          ,

    //miLTL_ZDispr = 3  , //JE   Z Dispensor
    piLTL_ZDispWait        = 0 , //대기 위치.
    piLTL_ZDispFtCheck         , //Z축 니들 높이측정 체크센서 위치.
    piLTL_ZDispRrCheck         , //Z축 니들 높이측정 체크센서 위치.
    piLTL_ZDispUVWork          , //Z축 유브이 조사 높이.
    piLTL_ZDispClean           , //Z축 니들 청소 위치.
    piLTL_ZDispUVTest          , //Z축 UV 조도계 테스트 높이
    MAX_PSTN_ID_MOTR3          ,

    //miRTL_XGenRr = 4  , // X Linear Top Stage Rear
    piRTL_XWait            = 0 , // 라이트 갠트리 리어.
    piRTL_XLIdxOutStt          , // 자제 배출 시작 위치.
    piRTL_XLIdxOutEnd          , // 자제 배출 끝 위치.
    piRTL_XRIdxOutStt          , // 자제 배출 시작 위치.
    piRTL_XRIdxOutEnd          , // 자제 배출 끝 위치.
    piRTL_XTrsfCheck           , // 스테이지 체크 센서 위치.  로테이터에서 트렌스퍼 조립 하고 왼쪽 비젼으로 로테이터 센터서 부터 체크센서까지 거리 측정하여 역산으로 넣는다.
    piRTL_XTrsfRotator         , // 로테이터에서 집는 위치.
    piRTL_XTrsfRotatorRvs      , // 로테이터에서 뒤집힌 자제 집는 위치.
    piRTL_XVisnCheck           , // 비전 체크센서 위치.
    piRTL_XVisnStgCnt          , // 스테이지의 센터를 세팅하고 비젼으로 그곳을 본 포지션.
    piRTL_XVisnHtCheck         , // 높이측정기 체크센서 위치.
    piRTL_XVisnCfCheck         , // 콘포컬 측정기 체크센서 위치.
    MAX_PSTN_ID_MOTR4          ,

    //miRTL_XGenFt = 5  , // X Linear Top Stage Front
    piRTL_XGenFtWait       = 0 , // 대기 위치.
    MAX_PSTN_ID_MOTR5          ,

    //miRTL_YVisn  = 6  , // Y Linear Top Stage Align
    piRTL_YVisnWait        = 0 ,
    piRTL_YVisnCheck           , // 비전 체크센서 위치.
    piRTL_YVisnStgCnt          , // 스테이지의 센터를 세팅하고 비젼으로 그곳을 본 포지션.
    piRTL_YVisnHtCheck         , // 높이측정기 체크센서 위치.
    piRTL_YVisnCfCheck         , // 콘포컬 측정기 체크센서 위치.
    MAX_PSTN_ID_MOTR6          ,

    //miRTL_ZVisn  = 7  , // Z Align Vision
    piRTL_ZVisnWait        = 0 ,
    piRTL_ZVisnCheckWork       , //체크 센서위치에서의 포커스를 맞춘 높이...  추후 검사들은 높이 측정기로 측정 하고 자동 보정.
    piRTL_ZVisnCmsAlign        ,
    piRTL_ZVisnCmsConfocal     ,
    MAX_PSTN_ID_MOTR7          ,

    //miLTL_YVisn  = 8  , // Y Orientation Vision
    piLTL_YVisnWait        = 0 ,//
    piLTL_YVisnHtCheck         ,//높이 측정기 터체센서 위치
    piLTL_YVisnCheck           ,//공용 비젼 터치센서 센터 위치.
    piLTL_YVisnCmsOri          ,//미들블럭 역삽 검사.
    piLTL_YVisnRttCnt          ,
    MAX_PSTN_ID_MOTR8          ,

    //miRTL_YTrspr = 9  , // Y Transfer Uint
    piRTL_YTrsfWait        = 0 ,//
    piRTL_YTrsfCheck           ,//
    piRTL_YTrsfRoatator        ,//
    piRTL_YTrsfRotatorRvs      ,
    MAX_PSTN_ID_MOTR9          ,

    //miSTG_XStage = 10 , // X Linear Bottom Stage
    piSTG_XStageWait       = 0 , // 대기 위치.
    piSTG_XStageCntr           , // 비젼으로 확인하여 터치센서의 센터와 맞춘다.  레일얼라인 실린더 감안하여 블럭을 넣고 레일을 꽉끼게 해서 센터잡음.
    piSTG_XStageWork           , // 작업 위치... 위치값은 캐드기준으로 계산한다.
    MAX_PSTN_ID_MOTR10         ,

    //miSTG_YStage = 11 , // Y Linear Bottom Stage
    piSTG_YStageWait       = 0 , // 대기 위치.
    piSTG_YStageCntr           , // 비젼으로 확인 하여 레일폭의 센터로 맞춘다. 레일얼라인 실린더 감안하여 블럭을 넣고 레일을 꽉끼게 해서 센터잡음.
    piSTG_YStageWork           , // 작업 위치... 위치값은 캐드기준으로 계산한다.
    MAX_PSTN_ID_MOTR11         ,

    //miLTL_ZVisn  = 12 , // Z Orientation Vision
    piLTL_ZVisnWait        = 0 , // 대기 위치.
    piLTL_ZVisnCheck           , //체크 센서위치에서의 포커스를 맞춘 높이...  추후 검사들은 높이 측정기로 측정 하고 자동 보정.
    piLTL_ZVisnMid             ,
    piLTL_ZVisnCmsOri          ,
    piLTL_ZVisnCmsAlign        ,
    MAX_PSTN_ID_MOTR12         ,

    // miIDX_XIndxR = 13 , // Index X Index Rear
    piIDX_XIndxRWait       = 0 , // 대기 위치.
    piIDX_XIndxRGetWait        , // 작업 대기 위치
    piIDX_XIndxRCheck1         , // CMOS 로더에서 CMOS Index 공급 첫 번째 위치
    piIDX_XIndxRCheck2         , // CMOS 로더에서 CMOS Index 공급 첫 번째 위치
    piIDX_XIndxRRotator        , // CMOS 로테이트 존까지의 이송 위치
    MAX_PSTN_ID_MOTR13         ,

    // miRTL_ZTrsfr = 14 , // Stage Z Transfer
    piRTL_ZTrsfrWait       = 0 , // 대기 위치.
    piRTL_ZTrsfrPick           , // 로테이트 존에 있는 CMOS 자재 집는 위치
    piRTL_ZTrsfrPlace          , // COMS 자재를 스테이지 위에 놓거나 Attach하는 위치
    MAX_PSTN_ID_MOTR14         ,

    //miRTL_YConvs = 15 , // Right COMS Transfer Conversion
    piRTL_YConvsWait       = 0 ,
    piRTL_YConvsWork1st        ,
    piRTL_YConvsWork2nd        ,
    MAX_PSTN_ID_MOTR15         ,

    //miIDX_TRotat = 16 , // Rail Rotator
    piIDX_TRotatWait       = 0 , // 대기 위치.
    piIDX_TRotatRotate         ,
    piIDX_TRotatNormal         ,
    MAX_PSTN_ID_MOTR16         ,

    //miSTG_YConvs = 17 , // Rail Conversion
    piSTG_YConvsWait       = 0 , // 대기 위치.
    piSTG_YConvsWork           , // 레일 폭 조절 위치
    MAX_PSTN_ID_MOTR17         ,

    //miLDR_YConvR = 18 , // COMS Loader Conversion
    piLDR_YConvRWait       = 0 , // 대기 위치.
    piLDR_YConvRWork           ,
    MAX_PSTN_ID_MOTR18         ,

    // miLDR_ZElevR = 19 , // COMS Loader Z Elevator
    piLDR_ZElevRWait       = 0 ,
    piLDR_ZElevRPickFrst       , //워크일때는 워크포지션에 있다가 인덱스 삽입후 플레이스 포지션으로 이동함.
    piLDR_ZElevRPlceFrst       , //플레이스일때는 플레이스 포지션에 있다가 인덱스 삽입후 워크 포지션으로 이동함.
    piLDR_ZElevRCheckFrst      , //와이드셉자제의 경우 2mm 높이는 바람에 Pick위치에서 센서 감지가 안된다.
                                      
    piLDR_ZElevRDetect         , //로더 자재 유무 확인 위치
    piLDR_ZElevRPick           ,
    piLDR_ZElevRPlce           ,
    MAX_PSTN_ID_MOTR19         ,

    // miLDR_ZElevF = 20 , // Loader Z Elevator
    piLDR_ZElevFWait       = 0 ,
    piLDR_ZElevFPickFrst       , //워크일때는 워크포지션에 있다가 인덱스 삽입후 플레이스 포지션으로 이동함.
    piLDR_ZElevFPlceFrst       ,

    piLDR_ZElevFDetect         ,//로더 자재 유무 확인 위치
    piLDR_ZElevFPick           ,
    piLDR_ZElevFPlce           ,
    MAX_PSTN_ID_MOTR20         ,

    //miSpare1     = 21 , // Spare1
    piSpare1Wait           = 0 ,
    MAX_PSTN_ID_MOTR21         ,

    // miULD_ZElevt = 22 , // Unloader Z Elevator
    piULD_ZElevtWait       = 0 ,
    piULD_ZElevtPickFrst       ,
    piULD_ZElevFPlceFrst       , //플레이스일때는 플레이스 포지션에 있다가 인덱스 삽입후 워크 포지션으로 이동함.
    piULD_ZElevtPick           ,
    piULD_ZElevFPlce           ,
    MAX_PSTN_ID_MOTR22         ,

    //miSpare2     = 23 ,
    piSpare2Wait           = 0 ,
    MAX_PSTN_ID_MOTR23

};


enum EN_MANUAL_CYCLE
{
    mcNoneCycle              =  0 ,
    mcAllHome                =  1 ,
    mcLDR_FHome              =  2 ,
    mcLDR_RHome              =  3 ,
    mcIDX_Home               =  4 ,
    mcLTL_Home               =  5 ,
    mcRTL_Home               =  6 ,
    mcSTG_Home               =  7 ,
    mcULD_Home               =  8 ,

    mcLDR_FCyclePick         = 10 ,
    mcLDR_FPitchUp           = 11 ,
    mcLDR_FPitchDn           = 12 ,

    mcLDR_RCyclePick         = 20 ,
    mcLDR_RCyclePlce         = 21 ,
    mcLDR_RPitchUp           = 22 ,
    mcLDR_RPitchDn           = 23 ,

    mcIDXCycleRotator        = 30 ,

    mcLTLCycleMidSupply      = 40 ,
    mcLTLCycleMidToWork      = 41 ,
    mcLTLCycleClean          = 42 ,
    mcLTLCycleMidLOri        = 43 ,
    mcLTLCycleMidLAlign      = 44 ,
    mcLTLCycleTopLUV         = 45 ,
    mcLTLCycleCmsLDisp       = 46 ,
    mcLTLCycleCmsLAlign      = 47 ,
    mcLTLCycleCmsLOri        = 48 ,
    mcLTLCycleHtReZero       = 49 ,
    mcLTLCycleManNdlCheck    = 50 ,
    mcLTLCycleManUVTest      = 51 ,
    mcLTLCycleWorkOut        = 52 ,

    mcRTLCycleMidRHeight     = 60 ,
    mcRTLCycleTrsfPick       = 61 ,
    mcRTLCycleTrsfPlace      = 62 ,
    mcRTLCycleCmsRHeight     = 63 ,
    mcRTLCycleCmsRAlign      = 64 ,
    mcRTLCycleCmsRGap        = 65 ,
    mcRTLCycleMidREndHeight  = 67 ,
    mcRTLCycleOut            = 68 ,
    mcRTLCycleHtReZero       = 69 ,
    mcRTLCycleWkoToPtl       = 70 ,
    mcRTLCycleCmsREndAlign   = 71 ,
    mcRTLCycleCmsRZHeight    = 72 ,

    mcULDCyclePick           = 80 ,
    mcULDCyclePlce           = 81 ,
    mcULDPitchUp             = 82 ,
    mcULDPitchDn             = 83 ,


    mcETCCycleCnvrs          = 90 , //컨버전 사이클.
    mcCycleCadPos            = 91 , //컨버전 사이클.

    mcCycleManualEndHeight   = 92 , //LTL_CycleMidToWork - LTL_MidLAlign - RTL_MidREndHeight 연속동작하는 매뉴얼 사이클
    mcCycleLDR_IdxGet        = 93 , //리어 로더에서 모듈 Pick 해서 로테이터로 옮기는 사이클
    mcCycleLDR_IdxPlce       = 94 , //로테이터에서 모듈 Pick 해서 리어로더로 옮기는 사이클
    mcCycleLDR_FPreGet       = 95 , //프론트 로더에서 프리인덱스까지 끌고나오는 사이클
    mcCyclePstRailULDPlce    = 96 , //포스트레일 - 언로더로 미들블럭(자재) 밀어내는 사이클
    mcCycleSTGChange         = 97 , //스테이지 체인지 사이클


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
/*026*/eiPKG_Dispr           , //
/*027*/eiPKG_Unknwn          ,
/*028*/ei028                 , //--------------------------------- 장비공통에러.
/*029*/eiLTL_UVUseTime       ,
/*030*/eiPRT_OverLoad        ,
/*031*/eiVSN_InspRangeOver   , // 현재 안쓰고 있는 에러
/*032*/eiVSN_InspNG          ,
/*033*/eiVSN_ComErr          , //비전 통신 에러.
/*034*/eiPRT_Missed          ,
/*035*/eiPRT_Detect          ,
/*036*/eiPRT_RemovePkg       ,
/*037*/eiVSN_PkgCrash        ,
/*038*/eiPCK_PickMiss        ,
/*049*/eiHGT_RangeOver       , //높이 센서 스펙 Range 에러 화면에 Retry Skip창을 띄울때 이에러를 쓴다.
/*040*/eiHGT_RangeErr        , //높이 센서 Range 에러...  센서가 튀었을때 이에러가 뜬다.
/*041*/eiLTL_Disp            , // 디스펜서 에러
/*042*/eiPRT_CheckErr        , //체크 센서 터치 에러.
/*043*/eiPRT_VacErr          , //배큠에러.
/*044*/eiPRT_VaccSensor      , //픽 동작인데 배큠센서 온되어서 집을수 없는경우
/*045*/eiSTG_HexaPot         , //헥사 포트 에러.
/*046*/eiSTG_PickMiss        ,
/*047*/eiPRT_LoadCell        ,
/*048*/eiETC_RS232           ,
/*049*/eiETC_CalErr          , //연산 에러
/*050*/eiULD_MgzSupply       , //Index작업 하려는데 Unloader에 자리 없으면...
MAX_ERR
};

enum EN_TOOL_ID{
    tlNone  = 0,
    tlLVisn    ,
    tlLHght    ,
    tlLUV      ,
    tlLDspR    ,
    tlLDspF    ,
    tlRTrns    ,
    tlRVisn    ,
    tlRHght    ,
    tlStage    ,
    tlRConfocal,
};


//레포트 남기는 시점 추가(MidREndHeight 스킵해도 저장 되도록)
//스테이지 어레이와 매뉴얼 버튼 동작 다를때 뻑나는거
//매뉴얼 버튼 다음 사이클에서 깜빡이도록
//스테이지 교체 매뉴얼 사이클 만들어서 붙이는거(오퍼레이션에 버튼 추가)
//UPH 소수점 아래 4번째 자리까지
//디바이스옵션 Attach Force 5000g 초과 입력 안되도록
//Common옵션에 Attach Force Limit 제거
//아웃풋 yIDX_Ionizer를 yIDX_IDXIonizer로 이름 변경
//아웃풋 yTRS_Ionizer를 yTRS_TRSIonizer로 이름 변경
//W-Ceph 디바이스 추가

#endif


