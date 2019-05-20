#ifndef UTILDEFINEH
#define UTILDEFINEH

//sunsun �δ� ��δ� �ε����� ����ŸƮ ����
//���� �÷��̽� �� ����Ʈ ���� �÷��̽� ���� ���.
//��δ� ���۷����� ó�� �Ӷ� �ڷ� �����°�.
//������ÿ� �������� ȸ�� �ϴ°�.
//
#define MODEL "HRM-930b"
#define EQP_SERIAL_LIST \
"\
REC-15-1048,\
"

//������ ���� �ؼ� ����� �ϱ� ���� �߰�..
//imicam ��7���� ���� ����� ���� ���� �ϸ� ī�޶� �����Ѿ��ؼ�.. �־���.
//7�� Neptune���� ��ü �ؾ���....
#define SKIP_VISN false

/*
Rs232 ����.
�������� ���� ���� ���� �ִ�.
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


//enum EN_VAC_ERR_STAT { //���� �� �� �ϰ� �ִ� ����.
//    vsRetry        = 0 , //�������� ���� �ٽ� ���ϰ� �������� �ٽ� ����.
//    vsErrSkipPick  = 1 , //���� �� �� ��ŵ.
//    vsErrLotEnd    = 2 , //������.
//};

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};


//��Ʈ ���̵�.
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
    riPTL       , //����Ʈ����.
    riULD       , //UnLoader

    MAX_ARAY
};

enum EN_CHIP_STAT {  //�ó��� ���� Ĩ�� ���µ�.
    csRetFail    = -1 , //�̰� �׳� ���Ͽ����� ���� ��ҽÿ��� ���� �ȉ�.
    csNone       =  0 ,
    csUnkwn           ,
    csMask            , //Test��� ����...
    csAttach          ,
    csEmpty           ,
    csWork            ,
    csWorkEnd         ,
    csDetect          , //Loader(F,R) ���� ���� ��ĵ
    csMidLOri         , //STG �̵�� ���������̼�.
    csMidLAlign       , //STG �̵�� �����.
    csMidRHeight      , //STG �̵�� ��������.������� ��.
    csCmsLDisp1       , //STG ù��° ����� -> ���� ���漭�� �Ǿ�����
    csCmsLDisp2       , //STG �ι�° ����� -> ����Ʈ ���漭�� �Ǿ�����
    csCmsLOri         , //RTT �ø� ���������̼�.
    csCmsRNeed        , //STG �ø� ���� �ʿ�.
    csCmsLAlign       , //STG �ø� ���� �������� ���������.
    csCmsRPreAlign    , //STG �ø� ���� ������ ��Ȯ�� �ϱ� ���� ���� ������� �س���.
    csCmsRHeight      , //STG �ø� ������ ����������� ���� ����.
    csCmsRAlign       , //STG �ø� ������ �������� ���о���� �� ���˻� ��ġ���� ����.
    csCmsRZHeight     , //���� �����Ͽ� Z�ุ �̵�.
    csCmsREndAlign    , //STG �ø� ���� �Ϸ� �˻�.
    csCmsREndHeight   , //STG �ø� �Ϸ� �˻�.
    csCmsLTopUV1      , //STG ��� ������ ����.
    csCmsLTopUV2      , //STG ��� ������ ����.

    csCmsFix          , //STG ������.
    csCmsRFixAlign    , //STG �ø� ���� �Ϸ� �˻�.
    csCmsRFixHeight   , //STG �ø� �Ϸ� �˻�.
    csMidREndHeight   , //STG ��� �ø� ���� �Ϸ��� ���� ���� ���� �˻�.////
    csCmsLHeight      , //�������Ϳ��� ���� ���̸� �����ؼ� �ٽ� �δ��� �ִ� ����϶�.... ��������..
    MAX_CHIP_STAT

};

//mi<��Ʈ��3�ڸ�>_<��漧1�ڸ�><�ΰ���Ī4>
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

//ai<��Ʈ��3�ڸ�>_<�ΰ���Ī><FWD�ÿ���ġ2�ڸ�><BWD�ÿ���ġ2�ڸ�>
//��Ⱑ Bwd �۵��� Fwd
enum EN_ACTR_ID {
    aiLTL_MGrperDnUp = 0  , // Griper Main Up/Dn
    aiLTL_SGrperDnUp = 1  , // Griper Sub  Up/Dn
    aiLTL_WrkPshDnUp = 2  , // Work Pusher
    aiRTL_LWkPshUpDn = 3  , // Pull Index UpDn // aiRTL_PulPshUpDn
    aiRTL_RWkPshUpDn = 4  , // Out Index UpDn  // aiRTL_OutPshUpDn
    aiIDX_RoatorUpDn = 5  ,
    aiLTL_DisprFDnUp = 6  , // ���漭 �Ǹ��� ù ��° (����� 10cc)
    aiLTL_DisprRDnUp = 7  , // ���漭 �Ǹ��� �� ��° (����� 30cc)
    aiLTL_UVCureDnUp = 8  , // UV ��ȭ��
    aiSTG_Fix1stFwBw = 9  , // Rail Middle Block Fix First  FwBw
    aiSTG_Fix2ndFwBw = 10 , // Rail Middle Block Fix Second FwBw
    aiSTG_Fix3rdFwBw = 11 , // Rail Middle Block Fix Third  FwBw
    aiSTG_Fix4thFwBw = 12 , // Rail Middle Block Fix Fourth FwBw
    aiSTG_Fix5thFwBw = 13 , // Rail Middle Block Fix Fifth  FwBw
    aiLTL_GriperFwBW = 14 , // Left Tool Griper ���� ����
//  aiSTG_FIonizer   = 14 , // ����
//  aiSTG_RIonizer   = 15 , // ����

    MAX_ACTR

};

//�׳� ���̿�.  x+<��Ʈ 3�ڸ�>+_+<���μ���>
//�Ǹ��� ���̿� x+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ2�ڸ� ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {

    //Ft - Front , Rr - Rear ���� ���� ��.
    x0000 =  0 , xETC_LStartSw        =  0 , //  xETC_StartSw // ���� Fos�� �ִ� ��!
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
    x0048 = 72 , xRTL_LWrkIndexUp     = 72 , // xRTL_PullIndexUp (����)
    x0049 = 73 , xRTL_LWrkIndexDn     = 73 , // xRTL_PullIndexDn (����)
    x004A = 74 , xRTL_RWrkIndexUp     = 74 , // xRTL_OutIndexUp  (����)
    x004B = 75 , xRTL_RwrkIndexDn     = 75 , // xRTL_OutIndexDn  (����)
    x004C = 76 , xRTL_PullDetect      = 76 ,
    x004D = 77 , xRTL_OutDetect       = 77 ,
    x004E = 78 , xIDX_IdxVcumSnr      = 78 ,
    x004F = 79 , xSTG_StgVcum3        = 79 , // Rotator Vacuum Sensor

    x0050 = 80 , xRTL_TransfrVcum1    = 80 , // Transfer Vacuum
    x0051 = 81 , xRTL_TransfrVcum2    = 81 ,
    x0052 = 82 , x082                 = 82 , //xULD_Detect6
    x0053 = 83 , x083                 = 83 ,
    x0054 = 84 , xETC_NdleNotCheck    = 84 ,
    x0055 = 85 , xRTL_TrsfJigDetect   = 85 , // Ʈ������ �� ���� ���� ����
    x0056 = 86 , xIDX_PkgCrashDetect  = 86 , // �ε��� �� ���� ���� �� �浹 ���� ����
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

//�׳� ���̿�31.  y+<��Ʈ 3�ڸ�>+_+<���μ���>
//���� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ ex) Fw , Bw , Dn ��� 2�ڸ�>     �ڸ�
//�ܵ� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<Fw�ÿ��ش��ൿ2�ڸ�><Bw�ÿ��ش��ൿ2�ڸ�> ��
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
    y000F = 15 , yRTL_LWkIdxDnUp     = 15 , // yRTL_PulIdxUpDn (����)
    //Ft - Front , Rr - Rear ���� ���� ��.
    y0010 = 16 , yRTL_RWkIdxDnUp     = 16 , // yRTL_OutIdxUpDn (����)
    y0011 = 17 , yIDX_RotatorUpDn     = 17 , //
    y0012 = 18 , yLTL_DisprFDn        = 18 , //
    y0013 = 19 , yLTL_DisprRDn        = 19 , //
    y0014 = 20 , yLTL_UVCuringDn      = 20 , //
    y0015 = 21 , yIDX_IDXIonizer      = 21 , //
    y0016 = 22 , yTRS_TRSIonizer      = 22 , //
    y0017 = 23 , ySTG_UVAirSol        = 23 , //
    y0018 = 24 , yLTL_CleanOn         = 24 , //
    y0019 = 25 , yLTL_GriperFwBW      = 25 , //
    y001A = 26 , yETC_LightOnOff      = 26 , // // ������
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
    y002E = 46 , ySTG_VcumSol         = 46 , // ���α׷� �Ѹ� ������ �Ѹ� �ȴ�.  yIDX_RtatPumpOn ySTG_VcumPump1On ySTG_StgVcumPump2On ���������� �½��Ѿ���.
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

    //miLTL_XGenRr = 1  , //Pana X Linear Top Stage Rear (Ȩ������ ���� �ʿ� �پ� �־ �� ���� ������ ������ ����)
    pvLTL_XWait            = 0 ,//��� ��ġ.
    pvLTL_XGripGrip1           ,//ù��° �̵�� �׸� ��ġ
    pvLTL_XGripGrip2           ,//�ι�° �̵�� �׸� ��ġ
    pvLTL_XGripPull            ,//�̵�� ��� ���� ��ġ.
    pvLTL_XIndxBack            ,//�̵�� �ڷ� ���ư��� ��ġ.
    pvLTL_XIndxWork            ,//�̵�� �������� ��ũ������ ��ġ.
    pvLTL_XDispClean           ,//�ϵ� û�� ��ġ.
    pvLTL_XDispFtCheck         ,//�ϵ� �������� üũ���� ��ġ.
    pvLTL_XDispRrCheck         ,//�ϵ� �������� üũ���� ��ġ.
    pvLTL_XDispUVCheck         ,//������ �������� üũ���� ��ġ.
    pvLTL_XVisnHtCheck         ,//���� ������ ��ü���� ��ġ
    pvLTL_XVisnCheck           ,//���� ���� ��ġ���� ���� ��ġ.
    pvLTL_XVisnCmsOri          ,//�̵�� ���� �˻�.
    pvLTL_XWorkOut             ,//�̵�� ��ũ �ƿ����� �̴� ��ġ
    pvLTL_XVisnRttCnt          ,//���� �������� ����.
    MAX_PSTN_MOTR0             ,

    pvLTL_XGenFtWait       = 0 ,//��� ��ġ.
    MAX_PSTN_MOTR1             ,

    //miLTL_YDispr = 2  ,
    pvLTL_YDispWait        = 0 , //��� ��ġ.
    pvLTL_YDispClean           , //�ϵ� û�� ��ġ.
    pvLTL_YDispFtCheck         , //�ϵ� �������� üũ���� ��ġ.
    pvLTL_YDispRrCheck         , //�ϵ� �������� üũ���� ��ġ.
    pvLTL_YDispUVCheck         , //������ �������� üũ���� ��ġ.
    MAX_PSTN_MOTR2             ,

    //miLTL_ZDispr = 3  , //JE   Z Dispensor
    pvLTL_ZDispWait        = 0 , //��� ��ġ.
    pvLTL_ZDispFtCheck         , //Z�� �ϵ� �������� üũ���� ��ġ
    pvLTL_ZDispRrCheck         , //Z�� �ϵ� �������� üũ���� ��ġ
    pvLTL_ZDispUVWork          , //Z�� ������ ���� ����.
    pvLTL_ZDispClean           , //Z�� �ϵ� û�� ��ġ.
    pvLTL_ZDispUVTest          , //Z�� UV ������ �׽�Ʈ ����
    MAX_PSTN_MOTR3             ,

    //miRTL_XGenRr = 4  , // X Linear Top Stage Rear
    pvRTL_XWait            = 0 , // ����Ʈ ��Ʈ�� ����.
    pvRTL_XLIdxOutStt          , // ���� ���� ���� ��ġ.
    pvRTL_XLIdxOutEnd          , // ���� ���� �� ��ġ.
    pvRTL_XRIdxOutStt          , // ���� ���� ���� ��ġ.
    pvRTL_XRIdxOutEnd          , // ���� ���� �� ��ġ.
    pvRTL_XTrsfCheck           , // �������� üũ ���� ��ġ.  ����
    pvRTL_XTrsfRotator         , // �������Ϳ��� ���� ��ġ.
    pvRTL_XTrsfRotatorRvs      , // �������Ϳ��� ������ ���� ���� ��ġ.
    pvRTL_XVisnCheck           , // ���� üũ���� ��ġ.
    pvRTL_XVisnStgCnt          , // ���������� ���͸� �����ϰ� ��
    pvRTL_XVisnHtCheck         , // ���������� üũ���� ��ġ.
    pvRTL_XVisnCfCheck         , // ���������� üũ���� ��ġ
    //poRTL_XTrsfPlceOfs         , // ¦¦�� �������� ��� Ʈ�����۰� X�� �����V�� �Կ��� ����� �ϱ⿡ ��¿�� ���� �÷��̽��� �ڵ��ε� �־���.
    MAX_PSTN_MOTR4             ,

    //miRTL_XGenFt = 5  , // X Linear Top Stage Front
    pvRTL_XGenFtWait       = 0 , // ��� ��ġ.
    MAX_PSTN_MOTR5             ,

    //miRTL_YVisn  = 6  , // Y Linear Top Stage Align
    pvRTL_YVisnWait        = 0 ,
    pvRTL_YVisnCheck           , // ���� üũ���� ��ġ.
    pvRTL_YVisnStgCnt          , // ���������� ���͸� �����ϰ� ��
    pvRTL_YVisnHtCheck         , // ���������� üũ���� ��ġ.
    pvRTL_YVisnCfCheck         , // ������ ���� üũ���� ��ġ
    MAX_PSTN_MOTR6             ,

    //miRTL_ZVisn  = 7  , // Z Align Vision
    pvRTL_ZVisnWait        = 0 ,
    pvRTL_ZVisnCheckWork       , //üũ ������ġ������ ��Ŀ���� ��
    pvRTL_ZVisnCmsAlign        ,
    pvRTL_ZVisnCmsConfocal     ,
    MAX_PSTN_MOTR7             ,

    //miLTL_YVisn  = 8  , // Y Orientation Vision
    pvLTL_YVisnWait        = 0 , //
    pvLTL_YVisnHtCheck         , //���� ������ ��ü���� ��ġ.
    pvLTL_YVisnCheck           , //���� ���� ��ġ���� ���� ��ġ.
    pvLTL_YVisnCmsOri          , //�̵�� ���� �˻�.
    pvLTL_YVisnRttCnt          ,
    MAX_PSTN_MOTR8             ,

    //miRTL_YTrspr = 9  , // Y Transfer Uint
    pvRTL_YTrsfWait        = 0 , //
    pvRTL_YTrsfCheck           , //
    pvRTL_YTrsfRoatator        , //
    pvRTL_YTrsfRotatorRvs      , // �������Ϳ��� ������ ���� ���� ��ġ.
    MAX_PSTN_MOTR9             ,

    // miSTG_XStage = 10 , // X Linear Bottom Stage
    pvSTG_XStageWait       = 0 , // ��� ��ġ.
    pvSTG_XStageCntr           , // �������� Ȯ���Ͽ� ��ġ������ 
    MAX_PSTN_MOTR10            ,

    // miSTG_YStage = 11 , // Y Linear Bottom Stage
    pvSTG_YStageWait       = 0 , // ��� ��ġ.
    pvSTG_YStageCntr           , // �������� Ȯ�� �Ͽ� �������� ��
    MAX_PSTN_MOTR11            ,

    //miLTL_ZVisn  = 12 , // Z Orientation Vision
    pvLTL_ZVisnWait        = 0 , // ��� ��ġ.
    pvLTL_ZVisnCheck           , //üũ ������ġ������ ��Ŀ���� ��
    pvLTL_ZVisnMid             , //���������̼�
    pvLTL_ZVisnCmsOri          , //���������̼�
    pvLTL_ZVisnCmsAlign        , //�����.
    MAX_PSTN_MOTR12            ,

    // miIDX_XIndxR = 13 , // Index X Index Rear
    pvIDX_XIndxRWait       = 0 , // ��� ��ġ.
    pvIDX_XIndxRGetWait        , // �۾� ��� ��ġ
    pvIDX_XIndxRCheck1         , // CMOS �δ����� CMOS Index ����
    pvIDX_XIndxRCheck2         , // CMOS �δ����� CMOS Index ����
    pvIDX_XIndxRRotator        , // CMOS ������Ʈ �������� �̼� ��
    poIDX_XIndxRGet            , //üũ�� ��ġ���� �ߴ� ��ġ���� �����V.
    MAX_PSTN_MOTR13            ,

    // miRTL_ZTrsfr = 14 , // Stage Z Transfer
    pvRTL_ZTrsfrWait       = 0 , // ��� ��ġ.
    pvRTL_ZTrsfrPick           , // ������Ʈ ���� �ִ� CMOS ����
    pvRTL_ZTrsfrPlace          , // COMS ���縦 �������� ���� ����
    MAX_PSTN_MOTR14            ,

    //miRTL_YConvs = 15 , // Right COMS Transfer Conversion
    pvRTL_YConvsWait       = 0 ,
    pvRTL_YConvsWork1st        ,
    pvRTL_YConvsWork2nd        ,
    MAX_PSTN_MOTR15            ,

    //miIDX_TRotat = 16 , // Rail Rotator
    pvIDX_TRotatWait       = 0 , // ��� ��ġ.
    pvIDX_TRotatRotate         , // 180�� ������ġ.
    pvIDX_TRotatNormal         , // �ȵ��� ��ġ.
    MAX_PSTN_MOTR16            ,

    //miSTG_YConvs = 17 , // Rail Conversion
    pvSTG_YConvsWait       = 0 , // ��� ��ġ.
    pvSTG_YConvsWork           , // ���� �� ���� ��ġ
    MAX_PSTN_MOTR17            ,

    //miLDR_YConvR = 18 , // COMS Loader Conversion
    pvLDR_YConvRWait       = 0 , // ��� ��ġ.
    pvLDR_YConvRWork           ,
    MAX_PSTN_MOTR18            ,

    // miLDR_ZElevR = 19 , // COMS Loader Z Elevator
    pvLDR_ZElevRWait       = 0 ,
    pvLDR_ZElevRPickFrst       , //��ũ�϶��� ��ũ�����ǿ� �ִٰ�
    pvLDR_ZElevRPlceFrst       , //�÷��̽��϶��� �÷��̽� ������
    pvLDR_ZElevRCheckFrst      , //���̵�������� ��� 2mm ���̴� �ٶ��� Pick��ġ���� ���� ������ �ȵȴ�.
    MAX_PSTN_MOTR19            ,

    // miLDR_ZElevF = 20 , // Loader Z Elevator
    pvLDR_ZElevFWait       = 0 ,
    pvLDR_ZElevFPickFrst       , //��ũ�϶��� ��ũ�����ǿ� �ִٰ�
    MAX_PSTN_MOTR20            ,

    //miSpare1     = 21 , // Spare1
    pvSpare1Wait           = 0 ,
    MAX_PSTN_MOTR21            ,

    // miULD_ZElevt = 22 , // Unloader Z Elevator
    pvULD_ZElevtWait       = 0 ,
    pvULD_ZElevtPickFrst       ,
    //pvULD_ZElevFPlceFrst       , //�÷��̽��϶��� �÷��̽� ������
    pvULD_ZElevtPlce           ,
    MAX_PSTN_MOTR22            ,

    //miSpare2     = 23 ,
    pvSpare2Wait           = 0 ,
    MAX_PSTN_MOTR23
};

// ������ ����   pi+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����Ǽ���>
// �������� ������ ���� �ʴ� ���� (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) �ڷ� ����
// PstnMan���� CheckSafe�� ��ư Ŭ�� �ÿ� �¾� ��������. ����....
enum EN_PSTN_ID {
    //miLTL_XGenRr = 0  , //Pana X Linear Top Stage Rear  (miLTL_XGenFt ����ȭ)
    piLTL_XWait            = 0 ,//��� ��ġ.      piLTL_XWait
    piLTL_XGripGrip1           ,//�̵�� �׸� ��ġ1
    piLTL_XGripGrip2           ,//�̵�� �׸� ��ġ2
    piLTL_XGripPull            ,//�̵�� ��� ���� ��ġ.
    piLTL_XIndxBack            ,//�̵�� �ڷ� ���ư��� ��ġ.
    piLTL_XIndxWork            ,//�̵�� �������� ��ũ������ �̴� ��ġ.
    piLTL_XDispClean           ,//�ϵ� û�� ��ġ.
    piLTL_XDispFtCheck         ,//�ϵ� �������� üũ���� ��ġ.
    piLTL_XDispRrCheck         ,//�ϵ� �������� üũ���� ��ġ.
    piLTL_XDispUVCheck         ,//������ �������� üũ���� ��ġ.
    piLTL_XVisnHtCheck         ,//���� ������ ��ü���� ��ġ
    piLTL_XVisnCheck           ,//���� ���� ��ġ���� ���� ��ġ.
    piLTL_XVisnCmsOri          ,//�̵�� ���� �˻�.
    piLTL_XWorkOut             ,//�̵�� ��ũ �ƿ����� �̴� ��ġ.
    piLTL_XVisnRttCnt          ,//���� �������� ����.
    MAX_PSTN_ID_MOTR0          ,

    //miLTL_XGenFt = 1  ,
    piLTL_XGenFtWait       = 0 ,//��� ��ġ.
    MAX_PSTN_ID_MOTR1          ,



    //miLTL_YDispr = 2  ,
    piLTL_YDispWait        = 0 , //��� ��ġ.
    piLTL_YDispClean           ,//�ϵ� û�� ��ġ.
    piLTL_YDispFtCheck         ,//�ϵ� �������� üũ���� ��ġ.
    piLTL_YDispRrCheck         ,//�ϵ� �������� üũ���� ��ġ.
    piLTL_YDispUVCheck         ,//������ �������� üũ���� ��ġ.
    MAX_PSTN_ID_MOTR2          ,

    //miLTL_ZDispr = 3  , //JE   Z Dispensor
    piLTL_ZDispWait        = 0 , //��� ��ġ.
    piLTL_ZDispFtCheck         , //Z�� �ϵ� �������� üũ���� ��ġ.
    piLTL_ZDispRrCheck         , //Z�� �ϵ� �������� üũ���� ��ġ.
    piLTL_ZDispUVWork          , //Z�� ������ ���� ����.
    piLTL_ZDispClean           , //Z�� �ϵ� û�� ��ġ.
    piLTL_ZDispUVTest          , //Z�� UV ������ �׽�Ʈ ����
    MAX_PSTN_ID_MOTR3          ,

    //miRTL_XGenRr = 4  , // X Linear Top Stage Rear
    piRTL_XWait            = 0 , // ����Ʈ ��Ʈ�� ����.
    piRTL_XLIdxOutStt          , // ���� ���� ���� ��ġ.
    piRTL_XLIdxOutEnd          , // ���� ���� �� ��ġ.
    piRTL_XRIdxOutStt          , // ���� ���� ���� ��ġ.
    piRTL_XRIdxOutEnd          , // ���� ���� �� ��ġ.
    piRTL_XTrsfCheck           , // �������� üũ ���� ��ġ.  �������Ϳ��� Ʈ������ ���� �ϰ� ���� �������� �������� ���ͼ� ���� üũ�������� �Ÿ� �����Ͽ� �������� �ִ´�.
    piRTL_XTrsfRotator         , // �������Ϳ��� ���� ��ġ.
    piRTL_XTrsfRotatorRvs      , // �������Ϳ��� ������ ���� ���� ��ġ.
    piRTL_XVisnCheck           , // ���� üũ���� ��ġ.
    piRTL_XVisnStgCnt          , // ���������� ���͸� �����ϰ� �������� �װ��� �� ������.
    piRTL_XVisnHtCheck         , // ���������� üũ���� ��ġ.
    piRTL_XVisnCfCheck         , // ������ ������ üũ���� ��ġ.
    MAX_PSTN_ID_MOTR4          ,

    //miRTL_XGenFt = 5  , // X Linear Top Stage Front
    piRTL_XGenFtWait       = 0 , // ��� ��ġ.
    MAX_PSTN_ID_MOTR5          ,

    //miRTL_YVisn  = 6  , // Y Linear Top Stage Align
    piRTL_YVisnWait        = 0 ,
    piRTL_YVisnCheck           , // ���� üũ���� ��ġ.
    piRTL_YVisnStgCnt          , // ���������� ���͸� �����ϰ� �������� �װ��� �� ������.
    piRTL_YVisnHtCheck         , // ���������� üũ���� ��ġ.
    piRTL_YVisnCfCheck         , // ������ ������ üũ���� ��ġ.
    MAX_PSTN_ID_MOTR6          ,

    //miRTL_ZVisn  = 7  , // Z Align Vision
    piRTL_ZVisnWait        = 0 ,
    piRTL_ZVisnCheckWork       , //üũ ������ġ������ ��Ŀ���� ���� ����...  ���� �˻���� ���� ������� ���� �ϰ� �ڵ� ����.
    piRTL_ZVisnCmsAlign        ,
    piRTL_ZVisnCmsConfocal     ,
    MAX_PSTN_ID_MOTR7          ,

    //miLTL_YVisn  = 8  , // Y Orientation Vision
    piLTL_YVisnWait        = 0 ,//
    piLTL_YVisnHtCheck         ,//���� ������ ��ü���� ��ġ
    piLTL_YVisnCheck           ,//���� ���� ��ġ���� ���� ��ġ.
    piLTL_YVisnCmsOri          ,//�̵�� ���� �˻�.
    piLTL_YVisnRttCnt          ,
    MAX_PSTN_ID_MOTR8          ,

    //miRTL_YTrspr = 9  , // Y Transfer Uint
    piRTL_YTrsfWait        = 0 ,//
    piRTL_YTrsfCheck           ,//
    piRTL_YTrsfRoatator        ,//
    piRTL_YTrsfRotatorRvs      ,
    MAX_PSTN_ID_MOTR9          ,

    //miSTG_XStage = 10 , // X Linear Bottom Stage
    piSTG_XStageWait       = 0 , // ��� ��ġ.
    piSTG_XStageCntr           , // �������� Ȯ���Ͽ� ��ġ������ ���Ϳ� �����.  ���Ͼ���� �Ǹ��� �����Ͽ� ���� �ְ� ������ �˳��� �ؼ� ��������.
    piSTG_XStageWork           , // �۾� ��ġ... ��ġ���� ĳ��������� ����Ѵ�.
    MAX_PSTN_ID_MOTR10         ,

    //miSTG_YStage = 11 , // Y Linear Bottom Stage
    piSTG_YStageWait       = 0 , // ��� ��ġ.
    piSTG_YStageCntr           , // �������� Ȯ�� �Ͽ� �������� ���ͷ� �����. ���Ͼ���� �Ǹ��� �����Ͽ� ���� �ְ� ������ �˳��� �ؼ� ��������.
    piSTG_YStageWork           , // �۾� ��ġ... ��ġ���� ĳ��������� ����Ѵ�.
    MAX_PSTN_ID_MOTR11         ,

    //miLTL_ZVisn  = 12 , // Z Orientation Vision
    piLTL_ZVisnWait        = 0 , // ��� ��ġ.
    piLTL_ZVisnCheck           , //üũ ������ġ������ ��Ŀ���� ���� ����...  ���� �˻���� ���� ������� ���� �ϰ� �ڵ� ����.
    piLTL_ZVisnMid             ,
    piLTL_ZVisnCmsOri          ,
    piLTL_ZVisnCmsAlign        ,
    MAX_PSTN_ID_MOTR12         ,

    // miIDX_XIndxR = 13 , // Index X Index Rear
    piIDX_XIndxRWait       = 0 , // ��� ��ġ.
    piIDX_XIndxRGetWait        , // �۾� ��� ��ġ
    piIDX_XIndxRCheck1         , // CMOS �δ����� CMOS Index ���� ù ��° ��ġ
    piIDX_XIndxRCheck2         , // CMOS �δ����� CMOS Index ���� ù ��° ��ġ
    piIDX_XIndxRRotator        , // CMOS ������Ʈ �������� �̼� ��ġ
    MAX_PSTN_ID_MOTR13         ,

    // miRTL_ZTrsfr = 14 , // Stage Z Transfer
    piRTL_ZTrsfrWait       = 0 , // ��� ��ġ.
    piRTL_ZTrsfrPick           , // ������Ʈ ���� �ִ� CMOS ���� ���� ��ġ
    piRTL_ZTrsfrPlace          , // COMS ���縦 �������� ���� ���ų� Attach�ϴ� ��ġ
    MAX_PSTN_ID_MOTR14         ,

    //miRTL_YConvs = 15 , // Right COMS Transfer Conversion
    piRTL_YConvsWait       = 0 ,
    piRTL_YConvsWork1st        ,
    piRTL_YConvsWork2nd        ,
    MAX_PSTN_ID_MOTR15         ,

    //miIDX_TRotat = 16 , // Rail Rotator
    piIDX_TRotatWait       = 0 , // ��� ��ġ.
    piIDX_TRotatRotate         ,
    piIDX_TRotatNormal         ,
    MAX_PSTN_ID_MOTR16         ,

    //miSTG_YConvs = 17 , // Rail Conversion
    piSTG_YConvsWait       = 0 , // ��� ��ġ.
    piSTG_YConvsWork           , // ���� �� ���� ��ġ
    MAX_PSTN_ID_MOTR17         ,

    //miLDR_YConvR = 18 , // COMS Loader Conversion
    piLDR_YConvRWait       = 0 , // ��� ��ġ.
    piLDR_YConvRWork           ,
    MAX_PSTN_ID_MOTR18         ,

    // miLDR_ZElevR = 19 , // COMS Loader Z Elevator
    piLDR_ZElevRWait       = 0 ,
    piLDR_ZElevRPickFrst       , //��ũ�϶��� ��ũ�����ǿ� �ִٰ� �ε��� ������ �÷��̽� ���������� �̵���.
    piLDR_ZElevRPlceFrst       , //�÷��̽��϶��� �÷��̽� �����ǿ� �ִٰ� �ε��� ������ ��ũ ���������� �̵���.
    piLDR_ZElevRCheckFrst      , //���̵�������� ��� 2mm ���̴� �ٶ��� Pick��ġ���� ���� ������ �ȵȴ�.
                                      
    piLDR_ZElevRDetect         , //�δ� ���� ���� Ȯ�� ��ġ
    piLDR_ZElevRPick           ,
    piLDR_ZElevRPlce           ,
    MAX_PSTN_ID_MOTR19         ,

    // miLDR_ZElevF = 20 , // Loader Z Elevator
    piLDR_ZElevFWait       = 0 ,
    piLDR_ZElevFPickFrst       , //��ũ�϶��� ��ũ�����ǿ� �ִٰ� �ε��� ������ �÷��̽� ���������� �̵���.
    piLDR_ZElevFPlceFrst       ,

    piLDR_ZElevFDetect         ,//�δ� ���� ���� Ȯ�� ��ġ
    piLDR_ZElevFPick           ,
    piLDR_ZElevFPlce           ,
    MAX_PSTN_ID_MOTR20         ,

    //miSpare1     = 21 , // Spare1
    piSpare1Wait           = 0 ,
    MAX_PSTN_ID_MOTR21         ,

    // miULD_ZElevt = 22 , // Unloader Z Elevator
    piULD_ZElevtWait       = 0 ,
    piULD_ZElevtPickFrst       ,
    piULD_ZElevFPlceFrst       , //�÷��̽��϶��� �÷��̽� �����ǿ� �ִٰ� �ε��� ������ ��ũ ���������� �̵���.
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


    mcETCCycleCnvrs          = 90 , //������ ����Ŭ.
    mcCycleCadPos            = 91 , //������ ����Ŭ.

    mcCycleManualEndHeight   = 92 , //LTL_CycleMidToWork - LTL_MidLAlign - RTL_MidREndHeight ���ӵ����ϴ� �Ŵ��� ����Ŭ
    mcCycleLDR_IdxGet        = 93 , //���� �δ����� ��� Pick �ؼ� �������ͷ� �ű�� ����Ŭ
    mcCycleLDR_IdxPlce       = 94 , //�������Ϳ��� ��� Pick �ؼ� ����δ��� �ű�� ����Ŭ
    mcCycleLDR_FPreGet       = 95 , //����Ʈ �δ����� �����ε������� �������� ����Ŭ
    mcCyclePstRailULDPlce    = 96 , //����Ʈ���� - ��δ��� �̵��(����) �о�� ����Ŭ
    mcCycleSTGChange         = 97 , //�������� ü���� ����Ŭ


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
/*028*/ei028                 , //--------------------------------- �����뿡��.
/*029*/eiLTL_UVUseTime       ,
/*030*/eiPRT_OverLoad        ,
/*031*/eiVSN_InspRangeOver   , // ���� �Ⱦ��� �ִ� ����
/*032*/eiVSN_InspNG          ,
/*033*/eiVSN_ComErr          , //���� ��� ����.
/*034*/eiPRT_Missed          ,
/*035*/eiPRT_Detect          ,
/*036*/eiPRT_RemovePkg       ,
/*037*/eiVSN_PkgCrash        ,
/*038*/eiPCK_PickMiss        ,
/*049*/eiHGT_RangeOver       , //���� ���� ���� Range ���� ȭ�鿡 Retry Skipâ�� ��ﶧ �̿����� ����.
/*040*/eiHGT_RangeErr        , //���� ���� Range ����...  ������ Ƣ������ �̿����� ���.
/*041*/eiLTL_Disp            , // ���漭 ����
/*042*/eiPRT_CheckErr        , //üũ ���� ��ġ ����.
/*043*/eiPRT_VacErr          , //��Ũ����.
/*044*/eiPRT_VaccSensor      , //�� �����ε� ��Ũ���� �µǾ ������ ���°��
/*045*/eiSTG_HexaPot         , //��� ��Ʈ ����.
/*046*/eiSTG_PickMiss        ,
/*047*/eiPRT_LoadCell        ,
/*048*/eiETC_RS232           ,
/*049*/eiETC_CalErr          , //���� ����
/*050*/eiULD_MgzSupply       , //Index�۾� �Ϸ��µ� Unloader�� �ڸ� ������...
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


//����Ʈ ����� ���� �߰�(MidREndHeight ��ŵ�ص� ���� �ǵ���)
//�������� ��̿� �Ŵ��� ��ư ���� �ٸ��� �����°�
//�Ŵ��� ��ư ���� ����Ŭ���� �����̵���
//�������� ��ü �Ŵ��� ����Ŭ ���� ���̴°�(���۷��̼ǿ� ��ư �߰�)
//UPH �Ҽ��� �Ʒ� 4��° �ڸ�����
//����̽��ɼ� Attach Force 5000g �ʰ� �Է� �ȵǵ���
//Common�ɼǿ� Attach Force Limit ����
//�ƿ�ǲ yIDX_Ionizer�� yIDX_IDXIonizer�� �̸� ����
//�ƿ�ǲ yTRS_Ionizer�� yTRS_TRSIonizer�� �̸� ����
//W-Ceph ����̽� �߰�

#endif


