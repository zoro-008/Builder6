#ifndef UTILDEFINEH
#define UTILDEFINEH

//sunsun 로더 언로더 인덱스바 투스타트 관련
//소켓 플레이스 및 쉬프트 소켓 플레이스 인포 등등.
//언로더 세퍼레이터 처음 켤때 뒤로 빠지는것.
//랏엔드시에 스테이지 회전 하는것.
//
#define EQP_SERIAL_LIST \
"\
TRS-14-1048,\
"



//소켓이나 툴의 갯수.
#define MAX_TOOL_SOCK_COL 4

//DD모터위의 스테이지의 갯수.
#define MAX_STAGE 4

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

//파트 아이디.
enum EN_PART_ID {
    piSRT  = 0 , //소터               mcLSP_Home
    piSTG      , //DD모터 스테이지.   mcLDR_Home
    piOST      , //OST 업다운.        mcLST_Home
    piVTI      , //VT                 mcSRT_Home
    piVSN      , //비젼.              mcSTG_Home
    piLSP      , //로더 서플라이.     mcOST_Home
    piLDR      , //로더.              mcVTI_Home
    piLST      , //로더 스탁.         mcVSN_Home
    piUSP      , //언로더 서플라이.   mcUSP_Home
    piULD      , //언로더.            mcULD_Home
    piUST      , //언로더 스탁.       mcUST_Home
    MAX_PART
};

enum EN_ARAY_ID {
    riSRT  =0 , //소터.
    riZIG     , //얼라이너.

    riSTF     , //앞     턴테이블1riSG1     , //턴테이블1
    riSTL     , //왼쪽   턴테이블2riSG2     , //턴테이블2
    riSTB     , //뒷쪽   턴테이블3riSG3     , //턴테이블3
    riSTR     , //오른쪽 턴테이블4riSG4     , //턴테이블4

    riPRL     , //프리 로더
    riLDR     , //로더
    riPSL     , //포스트 로더
    riPRU     , //프리 언로더.
    riULD     , //언로더.
    riPSU     , //포스트 언로더
    riRJ1     , //리젝1
    riRJ2     , //리젝2
    riRJ3     , //리젝3
    riRJ4     , //리젝4

    riMOS     , //메뉴얼 테스트용.OST
    riMVT     , //메뉴얼 테스트용.VT
    riMVS     , //메뉴얼 테스트용.VSN

    riSTU     , //스테이지 STagebUffer 버퍼 원래 지역 변수 였으나 지역변수로 했을때 소멸자에서 팝업 매뉴등을 소멸 시키지 않아 메모리 누수있음. 스테이지 버퍼 스테이지 돌릴때 필요.

    MAX_ARAY
};

//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail

//mi<파트명3자리>_<축방샹1자리><부가명칭4>
enum EN_MOTR_ID {
    miLDR_YIndx  = 0 , // 로더
    miLSP_ZElev  = 1 , // 로더 서플라이 Z
    miSRT_YPckr  = 2 , // 피커 무브 Y
    miSRT_ZPckr  = 3 , // 피커 툴   Z
    miSRT_XPckr  = 4 , // 피커 무브 X
    miSTG_TTble  = 5 , // 턴테이블
    miSTG_YVisn  = 6 , // 비젼 검사
    miSRT_XExpd  = 7 , // 익스펜딩
    miULD_YIndx  = 8 , // 언로더 Y
    miUSP_ZElev  = 9 , // 언로더 Z

    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiLSP_GuidFwBw  = 0 ,  //로더 가이드
    aiLST_StckUpDn      ,  //로더 스타커.
    aiLDR_IndxUpDn      ,  //인덱스바
    aiSRT_FrstDnUp      ,  //1번째 피커
    aiSRT_ScndDnUp      ,  //2번째 피커
    aiSRT_ThrdDnUp      ,  //3번째 피커
    aiSRT_FrthDnUp      ,  //4번째 피커
    aiOST_PshrDnUp      ,  //OST 푸셔
    aiOST_PTolUpDn      ,  //핀 툴
    aiVTI_PshrDnUp      ,  //VT 푸셔
    aiVTI_PTolUpDn      ,  //VT 핀툴.
    aiSRT_AlgnFwBw      ,  //얼라인 1,2,3,4번
    aiUSP_GuidFwBw      ,  //언로더 가이드
    aiUST_StckUpDn      ,  //이젝트
    aiULD_IndxUpDn      ,  //인덱스바

    MAX_ACTR

};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_StartSw            =  0 , //                                          x0000 =  0 , xETC_StartSw            =  0 , //
    x0001 =  1 , xETC_StopSw             =  1 , //                                          x0001 =  1 , xETC_StopSw             =  1 , //
    x0002 =  2 , xETC_ResetSw            =  2 , //                                          x0002 =  2 , xETC_ResetSw            =  2 , //
    x0003 =  3 , xETC_AirSw              =  3 , //                                          x0003 =  3 , xETC_AirSw              =  3 , //
    x0004 =  4 , xETC_InitSw             =  4 , //                                          x0004 =  4 , xETC_InitSw             =  4 , //
    x0005 =  5 , xLDR_SideEmgSw          =  5 , //                                          x0005 =  5 , xLDR_SideEmgSw          =  5 , //
    x0006 =  6 , xETC_FrntEmg1           =  6 , //                                          x0006 =  6 , xETC_FrntEmg1           =  6 , //
    x0007 =  7 , xETC_FrntEmg2           =  7 , //                                          x0007 =  7 , xETC_FrntEmg2           =  7 , //
    x0008 =  8 , xETC_MainPower          =  8 , //                                          x0008 =  8 , xETC_MainPower          =  8 , //
    x0009 =  9 , xETC_MainAir            =  9 , //                                          x0009 =  9 , xETC_MainAir            =  9 , //
    x000A = 10 , xETC_DoorLt             = 10 , //                                          x000A = 10 , xETC_FrntDoor1          = 10 , //
    x000B = 11 , xETC_DoorFt             = 11 , //                                          x000B = 11 , xETC_FrntDoor2          = 11 , //
    x000C = 12 , xETC_DoorRr             = 12 , //                                          x000C = 12 , xETC_RearDoor1          = 12 , //
    x000D = 13 , x013                    = 13 , //                                          x000D = 13 , xETC_RearDoor2          = 13 , //
    x000E = 14 , xLST_Tray1              = 14 , //                                          x000E = 14 , xLST_Tray1              = 14 , //
    x000F = 15 , xLST_Tray2Full          = 15 , //                                          x000F = 15 , xLST_Tray2Full          = 15 , //

    x0010 = 16 , xLDR_Tray3Work          = 16 , //                                          x0010 = 16 , xLDR_Tray3Work          = 16 , //
    x0011 = 17 , xLSP_Tray4              = 17 , //                                          x0011 = 17 , xLSP_Tray4              = 17 , //
    x0012 = 18 , xLSP_Tray5Sply          = 18 , //                                          x0012 = 18 , xLSP_Tray5Sply          = 18 , //
    x0013 = 19 , xLSP_LGuidFw             = 19 , //                                          x0013 = 19 , xLSP_GuidFw             = 19 , //
    x0014 = 20 , xLSP_RGuidFw             = 20 , //                                          x0014 = 20 , xLSP_GuidBw             = 20 , //
    x0015 = 21 , xLDR_IndxUp             = 21 , //                                          x0015 = 21 , xLDR_IndxUp             = 21 , //
    x0016 = 22 , xLST_EjctUpL            = 22 , //                                          x0016 = 22 , xLST_EjctUpL            = 22 , //
    x0017 = 23 , xLST_EjctDnL            = 23 , //                                          x0017 = 23 , xLST_EjctDnL            = 23 , //
    x0018 = 24 , xLST_EjctUpR            = 24 , //                                          x0018 = 24 , xLST_EjctUpR            = 24 , //
    x0019 = 25 , xLST_EjctDnR            = 25 , //                                          x0019 = 25 , xLST_EjctDnR            = 25 , //
    x001A = 26 , xSRT_VccSnsr1           = 26 , //                                          x001A = 26 , xSRT_VccSnsr1           = 26 , //
    x001B = 27 , xSRT_VccSnsr2           = 27 , //                                          x001B = 27 , xSRT_VccSnsr2           = 27 , //
    x001C = 28 , xSRT_VccSnsr3           = 28 , //                                          x001C = 28 , xSRT_VccSnsr3           = 28 , //
    x001D = 29 , xSRT_VccSnsr4           = 29 , //                                          x001D = 29 , xSRT_VccSnsr4           = 29 , //
    x001E = 30 , xSRT_PckrUp1            = 30 , //                                          x001E = 30 , xSRT_PckrUp1            = 30 , //
    x001F = 31 , xSRT_PckrUp2            = 31 , //                                          x001F = 31 , xSRT_PckrUp2            = 31 , //

    x0020 = 32 , xSRT_PckrUp3            = 32 , //                                          x0020 = 32 , xSRT_PckrUp3            = 32 , //
    x0021 = 33 , xSRT_PckrUp4            = 33 , //                                          x0021 = 33 , xSRT_PckrUp4            = 33 , //
    x0022 = 34 , xSRT_AlngFw1            = 34 , //                                          x0022 = 34 , xSRT_AlngFw1            = 34 , //
    x0023 = 35 , xSRT_AlngBw1            = 35 , //                                          x0023 = 35 , xSRT_AlngBw1            = 35 , //
    x0024 = 36 , xSRT_AlngFw2            = 36 , //                                          x0024 = 36 , xSRT_AlngFw2            = 36 , //
    x0025 = 37 , xSRT_AlngBw2            = 37 , //                                          x0025 = 37 , xSRT_AlngBw2            = 37 , //
    x0026 = 38 , xSRT_AlngFw3            = 38 , //                                          x0026 = 38 , xSRT_AlngFw3            = 38 , //
    x0027 = 39 , xSRT_AlngBw3            = 39 , //                                          x0027 = 39 , xSRT_AlngBw3            = 39 , //
    x0028 = 40 , xSRT_AlngFw4            = 40 , //                                          x0028 = 40 , xSRT_AlngFw4            = 40 , //
    x0029 = 41 , xSRT_AlngBw4            = 41 , //                                          x0029 = 41 , xSRT_AlngBw4            = 41 , //
    x002A = 42 , xOST_PshrUp              = 42 , //                                          x002A = 42 , xOST_PshrFw             = 42 , //
    x002B = 43 , xOST_PshrDn              = 43 , //                                          x002B = 43 , xOST_PshrBw             = 43 , //
    x002C = 44 , xVTI_PshrUp              = 44 , //                                          x002C = 44 , xVTI_PshrFw             = 44 , //
    x002D = 45 , xVTI_PshrDn              = 45 , //                                          x002D = 45 , xVTI_PshrBw             = 45 , //
    x002E = 46 , xOST_PTolUp              = 46 , //                                          x002E = 46 , xOST_PTolFw             = 46 , //
    x002F = 47 , xOST_PTolDn              = 47 , //                                          x002F = 47 , xOST_PTolBw             = 47 , //

    x0030 = 48 , xVTI_PTolUp             = 48 , //                                          x0030 = 48 , xVTI_PTolFw             = 48 , //
    x0031 = 49 , xVTI_PTolDn             = 49 , //                                          x0031 = 49 , xVTI_PTolBw             = 49 , //
    x0032 = 50 , xSRT_RjtTray1           = 50 , //                                          x0032 = 50 , x050                    = 50 , //
    x0033 = 51 , xSRT_RjtTray2           = 51 , //                                          x0033 = 51 , x051                    = 51 , //
    x0034 = 52 , xSRT_RjtTray3           = 52 , //                                          x0034 = 52 , x052                    = 52 , //
    x0035 = 53 , xSRT_RjtTray4           = 53 , //                                          x0035 = 53 , x053                    = 53 , //
    x0036 = 54 , xSRT_TrayDetect         = 54 , //                                          x0036 = 54 , x054                    = 54 , //
    x0037 = 55 , x055                    = 55 , //                                          x0037 = 55 , x055                    = 55 , //
    x0038 = 56 , xVSN_VsnReady1          = 56 , //                                          x0038 = 56 , xVSN_VsnReady1          = 56 , //
    x0039 = 57 , xVSN_VsnBusy1           = 57 , //                                          x0039 = 57 , xVSN_VsnBusy1           = 57 , //
    x003A = 58 , xVSN_VsnSpare1_1        = 58 , //                                          x003A = 58 , xVSN_VsnSpare1_1        = 58 , //
    x003B = 59 , xVSN_VsnSpare1_2        = 59 , //                                          x003B = 59 , xVSN_VsnSpare1_2        = 59 , //
    x003C = 60 , xVSN_VsnReady2          = 60 , //                                          x003C = 60 , xVSN_VsnReady2          = 60 , //
    x003D = 61 , xVSN_VsnBusy2           = 61 , //                                          x003D = 61 , xVSN_VsnBusy2           = 61 , //
    x003E = 62 , xVSN_VsnSpare2_1        = 62 , //                                          x003E = 62 , xVSN_VsnSpare2_1        = 62 , //
    x003F = 63 , xVSN_VsnSpare2_2        = 63 , //                                          x003F = 63 , xVSN_VsnSpare2_2        = 63 , //

    x0040 = 64 , xUST_Tray1              = 64 , //                                          x0040 = 64 , xUST_Tray1              = 64 , //
    x0041 = 65 , xUST_Tray2Full          = 65 , //                                          x0041 = 65 , xUST_Tray2Full          = 65 , //
    x0042 = 66 , xULD_Tray3Work          = 66 , //                                          x0042 = 66 , xULD_Tray3Work          = 66 , //
    x0043 = 67 , xUSP_Tray4              = 67 , //                                          x0043 = 67 , xUSP_Tray4Sply          = 67 , //
    x0044 = 68 , xUSP_Tray5Sply          = 68 , //                                          x0044 = 68 , xUSP_Tray5              = 68 , //
    x0045 = 69 , xUSP_LGuidFw            = 69 , //                                          x0045 = 69 , xUSP_GuidFw             = 69 , //
    x0046 = 70 , xUSP_RGuidFw            = 70 , //                                          x0046 = 70 , xUSP_GuidBw             = 70 , //
    x0047 = 71 , xUST_IndxUp             = 71 , //                                          x0047 = 71 , xUST_IndxUp             = 71 , //
    x0048 = 72 , xUST_LEjctUp            = 72 , //                                          x0048 = 72 , xUST_EjctUp1            = 72 , //
    x0049 = 73 , xUST_LEjctDn            = 73 , //                                          x0049 = 73 , xUST_EjctDn1            = 73 , //
    x004A = 74 , xUST_REjctUp            = 74 , //                                          x004A = 74 , xUST_EjctUp2            = 74 , //
    x004B = 75 , xUST_REjctDn            = 75 , //                                          x004B = 75 , xUST_EjctDn2            = 75 , //
    x004C = 76 , xETC_DoorRt             = 76 , //                                          x004C = 76 , x076                    = 76 , //
    x004D = 77 , xULD_EmgSw              = 77 , //                                          x004D = 77 , xULD_EmgSw              = 77 , //
    x004E = 78 , x078                    = 78 , //                                          x004E = 78 , x078                    = 78 , //
    x004F = 79 , x079                    = 79 , //                                          x004F = 79 , x079                    = 79 , //

    x0050 = 80 , xVTI_Ready1             = 80 , // VT                                       x0050 = 80 , xVTI_Ready1             = 80 , // VT
    x0051 = 81 , xVTI_Busy1              = 81 , //                                          x0051 = 81 , xVTI_Busy1              = 81 , //
    x0052 = 82 , xVTI_Spare1_1           = 82 , //                                          x0052 = 82 , xVTI_Spare1_1           = 82 , //
    x0053 = 83 , xVTI_Spare1_2           = 83 , //                                          x0053 = 83 , xVTI_Spare1_2           = 83 , //
    x0054 = 84 , xVTI_Ready2             = 84 , //                                          x0054 = 84 , xVTI_Ready2             = 84 , //
    x0055 = 85 , xVTI_Busy2              = 85 , //                                          x0055 = 85 , xVTI_Busy2              = 85 , //
    x0056 = 86 , xVTI_Spare2_1           = 86 , //                                          x0056 = 86 , xVTI_Spare2_1           = 86 , //
    x0057 = 87 , xVTI_Spare2_2           = 87 , //                                          x0057 = 87 , xVTI_Spare2_2           = 87 , //
    x0058 = 88 , xVTI_Ready3             = 88 , //                                          x0058 = 88 , xVTI_Ready3             = 88 , //
    x0059 = 89 , xVTI_Busy3              = 89 , //                                          x0059 = 89 , xVTI_Busy3              = 89 , //
    x005A = 90 , xVTI_Spare3_1           = 90 , //                                          x005A = 90 , xVTI_Spare3_1           = 90 , //
    x005B = 91 , xVTI_Spare3_2           = 91 , //                                          x005B = 91 , xVTI_Spare3_2           = 91 , //
    x005C = 92 , xVTI_Ready4             = 92 , //                                          x005C = 92 , xVTI_Ready4             = 92 , //
    x005D = 93 , xVTI_Busy4              = 93 , //                                          x005D = 93 , xVTI_Busy4              = 93 , //
    x005E = 94 , xVTI_Spare4_1           = 94 , //                                          x005E = 94 , xVTI_Spare4_1           = 94 , //
    x005F = 95 , xVTI_Spare4_2           = 95 , //                                          x005F = 95 , xVTI_Spare4_2           = 95 , //

    MAX_INPUT                                                                               //MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>                                               냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>     동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리> 동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동
enum EN_OUTPUT_ID  {                                                                        // EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp            =  0 , //                                          y0000 =  0 , yETC_StartLp            =  0 , //
    y0001 =  1 , yETC_StopLp             =  1 , //                                          y0001 =  1 , yETC_StopLp             =  1 , //
    y0002 =  2 , yETC_ResetLp            =  2 , //                                          y0002 =  2 , yETC_ResetLp            =  2 , //
    y0003 =  3 , yETC_AirLp              =  3 , //                                          y0003 =  3 , yETC_AirLp              =  3 , //
    y0004 =  4 , yETC_InitLp             =  4 , //                                          y0004 =  4 , yETC_InitLp             =  4 , //
    y0005 =  5 , yETC_MainAir            =  5 , //                                          y0005 =  5 , yETC_MainAir            =  5 , //
    y0006 =  6 , yETC_Light              =  6 , //                                          y0006 =  6 , yETC_Light              =  6 , //
    y0007 =  7 , y007                    =  7 , //                                          y0007 =  7 , y007                    =  7 , //
    y0008 =  8 , yETC_TwRedLp            =  8 , //                                          y0008 =  8 , yETC_TwRedLp            =  8 , //
    y0009 =  9 , yETC_TwYelLp            =  9 , //                                          y0009 =  9 , yETC_TwYelLp            =  9 , //
    y000A = 10 , yETC_TwGrnLp            = 10 , //                                          y000A = 10 , yETC_TwGrnLp            = 10 , //
    y000B = 11 , yETC_TwBzz              = 11 , //                                          y000B = 11 , yETC_TwBzz              = 11 , //
    y000C = 12 , yLSP_GuidBw             = 12 , //                                          y000C = 12 , yLSP_GuidBw             = 12 , //
    y000D = 13 , yLDR_IndxUp             = 13 , //                                          y000D = 13 , yLDR_IndxUp             = 13 , //
    y000E = 14 , yLST_StckUp             = 14 , //                                          y000E = 14 , yLST_StckUp             = 14 , //
    y000F = 15 , yLDR_AirBlow            = 15 , //                                          y000F = 15 , yLDR_AirBlow            = 15 , //

    y0010 = 16 , ySRT_Vcc1               = 16 , // yPRB_AirBlower                           y0010 = 16 , ySRT_Vcc1               = 16 , // yPRB_AirBlower
    y0011 = 17 , ySRT_Ejct1              = 17 , // yRAL_LIdxUpDn                            y0011 = 17 , ySRT_Ejct1              = 17 , // yRAL_LIdxUpDn
    y0012 = 18 , ySRT_Vcc2               = 18 , // yPSB_PshFwBw                             y0012 = 18 , ySRT_Vcc2               = 18 , // yPSB_PshFwBw
    y0013 = 19 , ySRT_Ejct2              = 19 , // yPSB_RlrDnUp                             y0013 = 19 , ySRT_Ejct2              = 19 , // yPSB_RlrDnUp
    y0014 = 20 , ySRT_Vcc3               = 20 , // yRAL_LRalVac                             y0014 = 20 , ySRT_Vcc3               = 20 , // yRAL_LRalVac
    y0015 = 21 , ySRT_Ejct3              = 21 , // yPSB_VsnVac                              y0015 = 21 , ySRT_Ejct3              = 21 , // yPSB_VsnVac
    y0016 = 22 , ySRT_Vcc4               = 22 , // yPSB_RwkVac                              y0016 = 22 , ySRT_Vcc4               = 22 , // yPSB_RwkVac
    y0017 = 23 , ySRT_Ejct4              = 23 , // yPRB_CmpDn                               y0017 = 23 , ySRT_Ejct4              = 23 , // yPRB_CmpDn
    y0018 = 24 , ySRT_PckrUp1            = 24 , // yPRB_CmpUp                               y0018 = 24 , ySRT_PckrUp1            = 24 , // yPRB_CmpUp
    y0019 = 25 , ySRT_PckrDn1            = 25 , // yPSB_CmpDn                               y0019 = 25 , ySRT_PckrDn1            = 25 , // yPSB_CmpDn
    y001A = 26 , ySRT_PckrUp2            = 26 , // yPSB_CmpUp                               y001A = 26 , ySRT_PckrUp2            = 26 , // yPSB_CmpUp
    y001B = 27 , ySRT_PckrDn2            = 27 , // y027/*yRWP_PkrVac*/                      y001B = 27 , ySRT_PckrDn2            = 27 , // y027/*yRWP_PkrVac*/
    y001C = 28 , ySRT_PckrUp3            = 28 , // yRWB_BrdFw                               y001C = 28 , ySRT_PckrUp3            = 28 , // yRWB_BrdFw
    y001D = 29 , ySRT_PckrDn3            = 29 , // yCPK_PkrVac                              y001D = 29 , ySRT_PckrDn3            = 29 , // yCPK_PkrVac
    y001E = 30 , ySRT_PckrUp4            = 30 , // yCSP_AlnFw                               y001E = 30 , ySRT_PckrUp4            = 30 , // yCSP_AlnFw
    y001F = 31 , ySRT_PckrDn4            = 31 , // yATP_RtrRl                               y001F = 31 , ySRT_PckrDn4            = 31 , // yATP_RtrRl

    y0020 = 32 , ySRT_Vcc                = 32 , // yATP_PkrVac                              y0020 = 32 , ySRT_Vcc                = 32 , // yATP_PkrVac
    y0021 = 33 , ySRT_AlgnFw             = 33 , // yATP_PkrEjt                              y0021 = 33 , ySRT_Algn12Up           = 33 , // yATP_PkrEjt
    y0022 = 34 , y034                    = 34 , // yPCP_PkrVacL                             y0022 = 34 , ySRT_Algn34Up           = 34 , // yPCP_PkrVacL
    y0023 = 35 , ySRT_AlgnAirBlw         = 35 , // yPCP_PkrEjtL                             y0023 = 35 , ySRT_AlgnAirBlw         = 35 , // yPCP_PkrEjtL
    y0024 = 36 , yOST_PshrDn             = 36 , // yPCP_PkrVacR                             y0024 = 36 , yOST_PshrDn             = 36 , // yPCP_PkrVacR
    y0025 = 37 , yVTI_PshrDn             = 37 , // yPCP_PkrEjtR                             y0025 = 37 , yVTI_PshrDn             = 37 , // yPCP_PkrEjtR
    y0026 = 38 , yOST_PTolUp             = 38 , // yRWP_PkrVac                              y0026 = 38 , yOST_PTolUp             = 38 , // yRWP_PkrVac
    y0027 = 39 , yVTI_PTolUp             = 39 , // yRWP_PkrEjt                              y0027 = 39 , yVTI_PTolUp             = 39 , // yRWP_PkrEjt
    y0028 = 40 , ySRT_ZBreak             = 40 , // yCUR_CmpDn                               y0028 = 40 , ySRT_ZBreak             = 40 , // yCUR_CmpDn
    y0029 = 41 , yLDR_ZBreak             = 41 , // yCUR_CmpUp                               y0029 = 41 , yLDR_ZBreak             = 41 , // yCUR_CmpUp
    y002A = 42 , yVSN_TrgRellay1         = 42 , // yPCP_EpdFw                               y002A = 42 , y042                    = 42 , // yPCP_EpdFw
    y002B = 43 , yVSN_TrgRellay2         = 43 , // y043                                     y002B = 43 , y043                    = 43 , // y043
    y002C = 44 , y044                    = 44 , // yPSB_FeedAC                              y002C = 44 , y044                    = 44 , // yPSB_FeedAC
    y002D = 45 , y045                    = 45 , // y045                                     y002D = 45 , y045                    = 45 , // y045
    y002E = 46 , y046                    = 46 , // y046                                     y002E = 46 , y046                    = 46 , // y046
    y002F = 47 , yVTI_VTPower            = 47 , // y047                                     y002F = 47 , y047                    = 47 , // y047

    y0030 = 48 , y048                    = 48 , //y048                                      y0030 = 48 , y048                    = 48 , //y048
    y0031 = 49 , y049                    = 49 , //y049                                      y0031 = 49 , y049                    = 49 , //y049
    y0032 = 50 , y050                    = 50 , //y050                                      y0032 = 50 , y050                    = 50 , //y050
    y0033 = 51 , y051                    = 51 , //y051                                      y0033 = 51 , y051                    = 51 , //y051
    y0034 = 52 , y052                    = 52 , //y052                                      y0034 = 52 , y052                    = 52 , //y052
    y0035 = 53 , y053                    = 53 , //y053                                      y0035 = 53 , y053                    = 53 , //y053
    y0036 = 54 , y054                    = 54 , //y054                                      y0036 = 54 , y054                    = 54 , //y054
    y0037 = 55 , y055                    = 55 , //y055                                      y0037 = 55 , y055                    = 55 , //y055
    y0038 = 56 , yVSN_VsnReset1          = 56 , //y056                                      y0038 = 56 , yVSN_VsnReset1          = 56 , //y056
    y0039 = 57 , yVSN_VsnChang1          = 57 , //y057                                      y0039 = 57 , yVSN_VsnChang1          = 57 , //y057
    y003A = 58 , yVSN_VsnSpare1_1        = 58 , //y058                                      y003A = 58 , yVSN_VsnSpear1_1        = 58 , //y058
    y003B = 59 , yVSN_VsnLotStart1       = 59 , //y059                                      y003B = 59 , yVSN_VsnSpare1_2        = 59 , //y059
    y003C = 60 , yVSN_VsnReset2          = 60 , //y060                                      y003C = 60 , yVSN_VsnReset2          = 60 , //y060
    y003D = 61 , yVSN_VsnChang2          = 61 , //y061                                      y003D = 61 , yVSN_VsnChang2          = 61 , //y061
    y003E = 62 , yVSN_VsnSpare2_1        = 62 , //y062                                      y003E = 62 , yVSN_VsnSpare2_1        = 62 , //y062
    y003F = 63 , yVSN_VsnLotStart2       = 63 , //y063                                      y003F = 63 , yVSN_VsnSpare2_2        = 63 , //y063

    y0040 = 64 , yUSP_GuidFw             = 64 , //                                          y0040 = 64 , yUSP_GuidFw             = 64 , //
    y0041 = 65 , yULD_IndxUp             = 65 , //                                          y0041 = 65 , yULD_IndxUp             = 65 , //
    y0042 = 66 , yUST_StckUp             = 66 , //                                          y0042 = 66 , yUST_StckUp             = 66 , //
    y0043 = 67 , y067                    = 67 , //                                          y0043 = 67 , y067                    = 67 , //
    y0044 = 68 , yULD_ZBreak             = 68 , //                                          y0044 = 68 , yULD_ZBreak             = 68 , //
    y0045 = 69 , y069                    = 69 , //                                          y0045 = 69 , y069                    = 69 , //
    y0046 = 70 , y070                    = 70 , //                                          y0046 = 70 , y070                    = 70 , //
    y0047 = 71 , y071                    = 71 , //                                          y0047 = 71 , y071                    = 71 , //
    y0048 = 72 , y072                    = 72 , //                                          y0048 = 72 , y072                    = 72 , //
    y0049 = 73 , y073                    = 73 , //                                          y0049 = 73 , y073                    = 73 , //
    y004A = 74 , y074                    = 74 , //                                          y004A = 74 , y074                    = 74 , //
    y004B = 75 , y075                    = 75 , //                                          y004B = 75 , y075                    = 75 , //
    y004C = 76 , y076                    = 76 , //                                          y004C = 76 , y076                    = 76 , //
    y004D = 77 , y077                    = 77 , //                                          y004D = 77 , y077                    = 77 , //
    y004E = 78 , y078                    = 78 , //                                          y004E = 78 , y078                    = 78 , //
    y004F = 79 , y079                    = 79 , //                                          y004F = 79 , y079                    = 79 , //

    y0050 = 80 , yVTI_Start1             = 80 , //                                          y0050 = 80 , yVTI_Start1             = 80 , //
    y0051 = 81 , yVTI_Reset1             = 81 , //                                          y0051 = 81 , yVTI_Reset1             = 81 , //
    y0052 = 82 , yVTI_Change1            = 82 , //                                          y0052 = 82 , yVTI_Change1            = 82 , //
    y0053 = 83 , yVTI_Spare1             = 83 , //                                          y0053 = 83 , yVTI_Spare1             = 83 , //
    y0054 = 84 , yVTI_Start2             = 84 , //                                          y0054 = 84 , yVTI_Start2             = 84 , //
    y0055 = 85 , yVTI_Reset2             = 85 , //                                          y0055 = 85 , yVTI_Reset2             = 85 , //
    y0056 = 86 , yVTI_Change2            = 86 , //                                          y0056 = 86 , yVTI_Change2            = 86 , //
    y0057 = 87 , yVTI_Spare2             = 87 , //                                          y0057 = 87 , yVTI_Spare2             = 87 , //
    y0058 = 88 , yVTI_Start3             = 88 , //                                          y0058 = 88 , yVTI_Start3             = 88 , //
    y0059 = 89 , yVTI_Reset3             = 89 , //                                          y0059 = 89 , yVTI_Reset3             = 89 , //
    y005A = 90 , yVTI_Change3            = 90 , //                                          y005A = 90 , yVTI_Change3            = 90 , //
    y005B = 91 , yVTI_Spare3             = 91 , //                                          y005B = 91 , yVTI_Spare3             = 91 , //
    y005C = 92 , yVTI_Start4             = 92 , //                                          y005C = 92 , yVTI_Start4             = 92 , //
    y005D = 93 , yVTI_Reset4             = 93 , //                                          y005D = 93 , yVTI_Reset4             = 93 , //
    y005E = 94 , yVTI_Change4            = 94 , //                                          y005E = 94 , yVTI_Change4            = 94 , //
    y005F = 95 , yVTI_Spare4             = 95 , //                                          y005F = 95 , yVTI_Spare4             = 95 , //


    MAX_OUTPUT
};

// 포지션 벨류   pv+<파트3자리>+_+<부가설명>+<포지션설명>
// 오프셑 오프셑 po+<파트3자리>+_+<부가설명>+<오프셑설명>
/*
    miLDR_YIndx  = 0 , // 로더
    miLSP_ZElev  = 1 , // 로더 서플라이 Z
    miSRT_YPckr  = 2 , // 피커 무브 Y
    miSRT_ZPckr  = 3 , // 피커 툴   Z
    miSRT_XPckr  = 4 , // 피커 무브 X
    miSTG_TTble  = 5 , // 턴테이블
    miSTG_YVisn  = 6 , // 비젼 검사
    miSRT_XExpd  = 7 , // 익스펜딩
    miULD_YIndx  = 8 , // 언로더 Y
    miULD_ZElev  = 9 , // 언로더 Z
*/
enum EN_PSTN_VALUE {            //
    pvLDR_YIndxWait        = 0 ,
    pvLDR_YIndxGet             ,                  
    pvLDR_YIndxWorkStt         ,                  
    pvLDR_YIndxOut             ,                  
    MAX_PSTN_MOTR0             ,

    pvLSP_ZElevWait        = 0 ,
    pvLSP_ZElevTop             ,
    pvLSP_ZElevSperate         ,                  
    MAX_PSTN_MOTR1             ,                  
                                                  
    pvSRT_YPckrWait        = 0 ,
    pvSRT_YPckrPick            ,
    pvSRT_YPckrZig             ,
    pvSRT_YPckrPocket          ,
    pvSRT_YPckrReject1Stt      ,
    pvSRT_YPckrReject2Stt      ,
    pvSRT_YPckrReject3Stt      ,
    pvSRT_YPckrReject4Stt      ,
    pvSRT_YPckrPlace           ,
    MAX_PSTN_MOTR2             ,

    pvSRT_ZPckrWait        = 0 ,
    pvSRT_ZPckrMove            ,
    pvSRT_ZPckrSTGPick         ,
    pvSRT_ZPckrSTGPlace        ,
    pvSRT_ZPckrZIGPick         ,
    pvSRT_ZPckrZIGPlace        ,
    pvSRT_ZPckrRJTPick         ,
    pvSRT_ZPckrRJTPlace        ,
    pvSRT_ZPckrLDRPick         ,
    pvSRT_ZPckrLDRPlace        ,
    pvSRT_ZPckrULDPick         ,
    pvSRT_ZPckrULDPlace        ,
    MAX_PSTN_MOTR3             ,

    pvSRT_XPckrWait        = 0 ,
    pvSRT_XPckrPickStt         ,
    pvSRT_XPckrZig             ,
    pvSRT_XPckrPocket          ,
    pvSRT_XPckrReject1Stt      ,
    pvSRT_XPckrReject2Stt      ,
    pvSRT_XPckrReject3Stt      ,
    pvSRT_XPckrReject4Stt      ,
    pvSRT_XPckrPlaceStt        ,
    MAX_PSTN_MOTR4             ,

    pvSTG_TTbleWait        = 0 ,
    pvSTG_TTbleWork            ,
    MAX_PSTN_MOTR5             ,

    pvSTG_YVisnWait        = 0 ,
    pvSTG_YVisnTrgStt1         ,
    pvSTG_YVisnTrgStt2         ,
    MAX_PSTN_MOTR6             ,

    pvSRT_XExpdWait        = 0 ,
    pvSRT_XExpdLDRTray         ,
    pvSRT_XExpdPocket          ,
    pvSRT_XExpdReject          ,
    pvSRT_XExpdZig             ,
    pvSRT_XExpdULDTray         ,
    MAX_PSTN_MOTR7             ,

    pvULD_YIndxWait        = 0 ,
    pvULD_YIndxGet             ,
    pvULD_YIndxWorkStt         ,
    pvULD_YIndxOut             ,
    MAX_PSTN_MOTR8             ,

    pvUSP_ZElevWait        = 0 ,
    pvUSP_ZElevTop             ,
    pvUSP_ZElevSperate         ,
    MAX_PSTN_MOTR9
};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {
    piLDR_YIndxWait        = 0 ,
    piLDR_YIndxGet             ,
    piLDR_YIndxWorkStt         ,
    piLDR_YIndxOut             ,
    piLDR_YIndxWork            ,
    MAX_PSTN_ID_MOTR0          ,

    piLSP_ZElevWait        = 0 ,
    piLSP_ZElevTop             ,
    piLSP_ZElevSperate         ,
    MAX_PSTN_ID_MOTR1          ,

    piSRT_YPckrWait        = 0 ,
    piSRT_YPckrPick            ,
    piSRT_YPckrZig             ,
    piSRT_YPckrPocket          ,
    piSRT_YPckrReject1Stt      ,
    piSRT_YPckrReject2Stt      ,
    piSRT_YPckrReject3Stt      ,
    piSRT_YPckrReject4Stt      ,
    piSRT_YPckrPlace           ,
    piSRT_YPckrReject1         ,//---------
    piSRT_YPckrReject2         ,
    piSRT_YPckrReject3         ,
    piSRT_YPckrReject4         ,
    MAX_PSTN_ID_MOTR2          ,

    piSRT_ZPckrWait        = 0 ,
    piSRT_ZPckrMove            ,
    piSRT_ZPckrSTGPick         ,
    piSRT_ZPckrSTGPlace        ,
    piSRT_ZPckrZIGPick         ,
    piSRT_ZPckrZIGPlace        ,
    piSRT_ZPckrRJTPick         ,
    piSRT_ZPckrRJTPlace        ,
    piSRT_ZPckrLDRPick         ,
    piSRT_ZPckrLDRPlace        ,
    piSRT_ZPckrULDPick         ,
    piSRT_ZPckrULDPlace        ,
    MAX_PSTN_ID_MOTR3          ,

    piSRT_XPckrWait        = 0 ,
    piSRT_XPckrPickStt         ,
    piSRT_XPckrZig             ,
    piSRT_XPckrPocket          ,
    piSRT_XPckrReject1Stt      ,
    piSRT_XPckrReject2Stt      ,
    piSRT_XPckrReject3Stt      ,
    piSRT_XPckrReject4Stt      ,
    piSRT_XPckrPlaceStt        ,
    piSRT_XPckrPick            ,//---------
    piSRT_XPckrReject1         ,
    piSRT_XPckrReject2         ,
    piSRT_XPckrReject3         ,
    piSRT_XPckrReject4         ,
    piSRT_XPckrPlace           ,
    MAX_PSTN_ID_MOTR4          ,

    piSTG_TTbleWait        = 0 ,
    piSTG_TTbleWork            ,
    MAX_PSTN_ID_MOTR5          ,

    piSTG_YVisnWait        = 0 ,
    piSTG_YVisnTrgStt1         ,
    piSTG_YVisnTrgStt2         ,
    piSTG_YVisnWorkStt1        ,
    piSTG_YVisnWorkEnd1        ,
    piSTG_YVisnWorkStt2        ,
    piSTG_YVisnWorkEnd2        ,
    MAX_PSTN_ID_MOTR6          ,

    piSRT_XExpdWait        = 0 ,
    piSRT_XExpdLDRTray         ,
    piSRT_XExpdPocket          ,
    piSRT_XExpdReject          ,
    piSRT_XExpdZig             ,
    piSRT_XExpdULDTray         ,
    MAX_PSTN_ID_MOTR7          ,

    piULD_YIndxWait        = 0 ,
    piULD_YIndxGet             ,
    piULD_YIndxWorkStt         ,
    piULD_YIndxOut             ,
    piULD_YIndxWork            ,
    MAX_PSTN_ID_MOTR8          ,

    piUSP_ZElevWait        = 0 ,
    piUSP_ZElevTop             ,
    piUSP_ZElevSperate         ,
    MAX_PSTN_ID_MOTR9

};


enum EN_MANUAL_CYCLE
{
//20140620 JS 정리.
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

    mcLSP_Home             = 10 ,
    mcLSP_CycleSupply           ,
    mcLSP_GuidFwBw              ,
    mcLDR_Home                  ,
    mcLDR_CycleIn               ,
    mcLDR_CycleWork             ,
    mcLDR_CycleOut              ,
    mcLDR_AirBlow               ,
    mcLDR_IndxUpDn              ,
    mcLST_Home                  ,
    mcLST_CycleStock            ,
    mcLST_StckUpDn              ,

    mcSRT_Home             = 30 ,
    mcSRT_CyclePickLDR          ,
    mcSRT_CyclePlceLDR          ,
    mcSRT_CyclePickSTG          ,
    mcSRT_CyclePlceSTG          ,
    mcSRT_CyclePickRJT1         ,
    mcSRT_CyclePlceRJT1         ,
    mcSRT_CyclePickRJT4         ,
    mcSRT_CyclePlceRJT4         ,
    mcSRT_CyclePickULD          ,
    mcSRT_CyclePlceULD          ,
    mcSRT_CyclePickZIG          ,
    mcSRT_CyclePlceZIG          ,
    mcSRT_CycleMoveLDR          ,
    mcSRT_CycleMoveSTG          ,
    mcSRT_CycleMoveRJT1         ,
    mcSRT_CycleMoveRJT4         ,
    mcSRT_CycleMoveULD          ,
    mcSRT_CycleMoveZIG          ,

    mcSTG_Home             = 50 ,
    mcSTG_CycleWork             ,
    mcSTG_Turn90                ,
    mcSTG_Turn90N               ,
    mcSTG_Turn45                ,
    mcSTG_Turn45N               ,
    mcOST_Home                  ,
    mcOST_CycleWork             ,
    mcOST_CycleTest             ,
    mcOST_PshrDnUp              ,
    mcOST_PTolUpDn              ,
    mcVTI_Home                  ,
    mcVTI_CycleWork             ,
    mcVTI_CycleTest             ,
    mcVTI_CycleBind             ,
    mcVTI_CycleUnBind           ,
    mcVTI_PshrDnUp              ,
    mcVTI_PTolUpDn              ,
    mcVTI_MainPwr               ,
    mcVSN_Home                  ,
    mcVSN_CycleWork             ,
    mcVSN_MoveP                 ,
    mcVSN_MoveN                 ,
    mcVSN_MoveTrg1              ,
    mcVSN_MoveTrg2              ,

    mcUSP_Home             = 80 ,
    mcUSP_CycleSupply           ,
    mcUSP_GuidFwBw              ,
    mcULD_Home                  ,
    mcULD_CycleIn               ,
    mcULD_CycleWork             ,
    mcULD_CycleOut              ,
    mcULD_IndxUpDn              ,
    mcUST_Home                  ,
    mcUST_CycleStock            ,
    mcUST_StckUpDn              ,



/*
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,


    mcLSP_CycleSupply      = 10 ,
    mcLDR_CycleIn               ,
    mcLDR_CycleOut              ,
    mcLST_CycleStock            ,
    mcSTG_CycleWork             ,
    mcOST_CycleWork             ,
    mcVTI_CycleWork             ,
    mcVSN_CycleWork             ,
    mcUSP_CycleSupply           ,
    mcULD_CycleIn               ,
    mcULD_CycleOut              ,
    mcUST_CycleStock            ,
    mcVTI_CycleTest             ,

    mcLSP_Home             = 25 ,
    mcLSP_GuidFwBw              ,

    mcLDR_Home             = 30 ,
    mcLDR_CycleWork             ,
    mcLDR_AirBlow               ,
    mcLDR_IndxUpDn              ,


    mcLST_Home             = 35 ,
    mcLST_StckUpDn              ,


    mcSRT_Home             = 40 ,
    mcSRT_CyclePickSTG          ,
    mcSRT_CyclePlceRJT          ,
    mcSRT_CyclePlceULD          ,
    mcSRT_CyclePickZIG          ,
    mcSRT_CyclePlceSTG          ,
    mcSRT_CyclePickLDR          ,
    mcSRT_CyclePlceZIG          ,
    mcSRT_Vcc1                  ,
    mcSRT_Ejct1                 ,
    mcSRT_Vcc2                  ,
    mcSRT_Ejct2                 ,
    mcSRT_Vcc3                  ,
    mcSRT_Ejct3                 ,
    mcSRT_Vcc4                  ,
    mcSRT_Ejct4                 ,
    mcSRT_MainVcc               ,
    mcSRT_AlgnAirBlw            ,
    mcSRT_FrstDnUp              ,
    mcSRT_ScndDnUp              ,
    mcSRT_ThrdDnUp              ,
    mcSRT_FrtdDnUp              ,
    mcSRT_AlgnUpDn              ,

    mcSTG_Home             = 70 ,

    mcOST_Home             = 75 ,
    mcOST_PshrDnUp              ,
    mcOST_PTolUpDn              ,

    mcVTI_Home             = 80 ,
    mcVTI_PshrDnUp              ,
    mcVTI_PTolUpDn              ,


    mcVSN_Home             = 85 ,

    mcUSP_Home             = 90 ,
    mcUSP_GuidFwBw              ,

    mcULD_Home             = 95 ,
    mcULD_CycleWork             ,
    mcULD_IndxUpDn              ,

    mcUST_Home             = 100,
    mcUST_StckUpDn              ,

    //Set Up용 Manual JS.
    mcSTU_Turn90           = 110,
    mcSTU_PickSTG               ,
    mcSTU_PlaceSTG              ,
    mcSTU_PickJIG               ,
    mcSTU_PlaceJIG              ,
    mcSTU_PickRJ1               ,
    mcSTU_PlaceRJ1              ,
    mcSTU_PickRJ2               ,
    mcSTU_PlaceRJ2              ,
    mcSTU_PickRJ3               ,
    mcSTU_PlaceRJ3              ,
    mcSTU_PickRJ4               ,
    mcSTU_PlaceRJ4              ,
    mcSTU_PickLDR               ,
    mcSTU_PlaceLDR              ,
    mcSTU_PickULD               ,
    mcSTU_PlaceULD              ,
*/

/*
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLSP_Home             = 10 ,
    mcLSP_CycleSupply           ,
    mcLSP_GuidFwBw              ,

    mcLDR_Home             = 15 ,
    mcLDR_CycleIn               ,
    mcLDR_CycleWork             ,
    mcLDR_CycleOut              ,
    mcLDR_AirBlow               ,
    mcLDR_IndxUpDn              ,

    mcLST_Home             = 25 ,
    mcLST_CycleStock            ,
    mcLST_StckUpDn              ,

    mcSRT_Home             = 30 ,
    mcSRT_CyclePickSTG          ,
    mcSRT_CyclePlceRJT          ,
    mcSRT_CyclePlceULD          ,
    mcSRT_CyclePickZIG          ,
    mcSRT_CyclePlceSTG          ,
    mcSRT_CyclePickLDR          ,
    mcSRT_CyclePlceZIG          ,
    mcSRT_Vcc1                  ,
    mcSRT_Ejct1                 ,
    mcSRT_Vcc2                  ,
    mcSRT_Ejct2                 ,
    mcSRT_Vcc3                  ,
    mcSRT_Ejct3                 ,
    mcSRT_Vcc4                  ,
    mcSRT_Ejct4                 ,
    mcSRT_Vcc                   ,
    mcSRT_AlgnAirBlw            ,
    mcSRT_FrstDnUp              ,
    mcSRT_ScndDnUp              ,
    mcSRT_ThrdDnUp              ,
    mcSRT_FrtdDnUp              ,
    mcSRT_AlgnUpDn              ,

    mcSTG_Home             = 60 ,
    mcSTG_CycleWork             ,

    mcOST_Home             = 65 ,
    mcOST_CycleWork             ,
    mcOST_PshrDnUp              ,
    mcOST_PTolUpDn              ,

    mcVTI_Home             = 70 ,
    mcVTI_CycleWork             ,
    mcVTI_PshrDnUp              ,
    mcVTI_PTolUpDn              ,


    mcVSN_Home             = 75 ,
    mcVSN_CycleWork             ,

    mcUSP_Home             = 80 ,
    mcUSP_CycleSupply           ,
    mcUSP_GuidFwBw              ,

    mcULD_Home             = 85 ,
    mcULD_CycleIn               ,
    mcULD_CycleWork             ,
    mcULD_CycleOut              ,
    mcULD_IndxUpDn              ,

    mcUST_Home             = 95 ,
    mcUST_CycleStock            ,
    mcUST_StckUpDn              ,
*/
    MAX_MANUAL_CYCLE
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/eiETC_Door            ,
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
/*030*/eiLSP_NeedTray        , //LDR
/*031*/eiLST_FullTray        ,
/*032*/ei032                 ,
/*033*/ei033                 ,
/*034*/ei034                 ,
/*035*/eiUSP_NeedTray        , //ULD
/*036*/eiUST_FullTray        ,
/*037*/ei037                 ,
/*038*/ei038                 ,
/*039*/ei039                 ,
/*040*/eiVSN_SerialNG        , //포켓별 연속에러.
/*041*/eiVSN_Busy            , //비지 않죽음.
/*042*/eiVSN_NotReady        , //레디 안됌.. 프로그램 안킴.
/*043*/eiVSN_TimeOut         , //비젼 시퀜스 타임아웃.
/*044*/eiVSN_WrongData       , //데이터 포멧이 맞지 않음.
/*045*/ei045                 ,
/*046*/eiVTI_SerialNG        , //포켓별 연속에러.
/*047*/eiVTI_Busy            , //비지 않죽음.
/*048*/eiVTI_NotReady        , //레디 안됌.. 프로그램 안킴.
/*050*/eiVTI_TimeOut         , //비젼 시퀜스 타임아웃.
/*051*/eiVTI_WrongData       , //파일 읽기 실패.
/*052*/eiOST_SerialNG        , //OST검사 연속에러.
/*053*/eiOST_TimeOut         , //OST검사 타임아웃 에러.
/*054*/eiOST_WrongData       , //OST 검사 결과값이 이상함.
/*055*/eiLDR_VaccMiss        , //로더쪽에서 픽 미스.
/*056*/eiSRT_VaccMiss        , //지그 및 소켓 에서 픽 미스.
/*057*/eiSRT_PlceMiss        ,
/*058*/eiSRT_RejectFull      , //리젝 다찼음.
/*059*/eiSRT_RJTNeedTray     , //리젝 구간 트레이 없음.
/*060*/eiSRT_RJTTrayRelease  , //트레이 테이블 빠짐.
/*061*/ei061                 , //
/*062*/ei062                 , //
/*063*/ei063                 , //
/*064*/ei064                 , //
/*065*/ei065                 , //
/*066*/ei066                 , //
/*067*/ei067                 , //
/*068*/ei068                 , //
/*069*/ei069                 , //
/*070*/ei070                 , //
MAX_ERR
};


#endif

