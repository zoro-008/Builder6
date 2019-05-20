#ifndef UTILDEFINEH
#define UTILDEFINEH

//통신하는 비젼피씨.
#define MAX_COM_VISN 6

//안한거 리스트.dd
/*
Trim One Skip 이거 시퀀스에 적용 안되있음.dd
Lot End 데이터 남는거랑 SPC 데이터 남기는거 미확인.dd
FrmVisionRlstSet 에 색깔 지정할수 있게 하는거 깜박. dd
마스터 옵션들 정리. dd
IO리스트들
Err 리스트들
기타 리스트들...dd

리뷰비전 시퀀스 아직 안함. 이거 아직 IO 도 안나옴...dd
장비간 통신 아직 미확인. dd
SPC 첫페이지 이거 아직 안함.dd
*/
enum EN_SRL_Num {
    LUS_12_1636  = 0 ,
    LUS_12_1637      ,

    MAX_SRL_Num
};

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_ARAY_ID {
    riLDR       = 0  , //로더 탑 데이터.
    riPRB       = 1  , //레일 프리버퍼.
    riWR1       = 2  , //워크존1
    riVS1       = 3  , //워크존1 비젼프로세스.
    riWR2       = 4  , //워크존2
    riVS2       = 5  , //워크존2 비젼프로세스.
    riWR3       = 6  , //워크존3
    riVS3       = 7  , //워크존3 비젼프로세스.
    riPSB       = 8  , //포스트 버퍼 & 트림.
    riULD       = 9  , //언로더 탑.
    riLot       = 10 , //랏엔드시에 쓰임.
    riBuf       = 11 , //아오 비전 더블클릭해서 보는거 이거 데이터 덮어 씌울라고 ... dd
    MAX_ARAY
};

enum EN_MOTR_ID { //나중에 전장 도면 보고 순서 수정.
    miWRK_XVsn  =  0 , // 워크존 Vision X축
    miWRK_YVsn  =  1 , // 워크존 Vision Y축
    miPSB_XTrm  =  2 , // 포스트 버퍼 트림 X
    miPSB_YTrm  =  3 , // 포스트 버퍼 트림 Y
    miPSB_TFlp  =  4 , // 포스트 버퍼 플립퍼
    miPSB_XIns  =  5 , // 포스트 버퍼 Insert
    miWR1_XCvr  =  6 , // 워크존 1 Conversion X
    miWR2_XCvr  =  7 , // 워크존 2 Conversion X
    miWR3_XCvr  =  8 , // 워크존 3 Conversion X
    miSpare     =  9 , // 모터 스페어.
    miLDR_YCmp  = 10 , // 로더 Clamp Y축
    miLDR_ZCmp  = 11 , // 로더 Clamp Z축
    miULD_YCmp  = 12 , // 언로더 Clamp Y축
    miULD_ZCmp  = 13 , // 언로더 Clamp Z축
    MAX_MOTR
};


enum EN_ACTR_ID {            //아오 오타가 너무 많아서 옆에 숫자 지워 버림.dd
    aiLDR_ClmpUpDn    = 0 , //로더 클램프
    aiLDR_PshrFwBw        , //로더 푸셔
    aiPRB_StprUpDn        , //프리퍼버 스토퍼
    aiWK1_StprUpDn        , //워크존 1 스토퍼
    aiWK1_AlgnFwBw        , //워크존 1 얼라인
    aiWK1_SttnUpDn        , //워크존 1 스테이션
    aiWK2_StprUpDn        , //워크존 2 스토퍼
    aiWK2_AlgnFwBw        , //워크존 2 얼라인
    aiWK2_SttnUpDn        , //워크존 2 스테이션
    aiWK3_StprUpDn        , //워크존 3 스토퍼
    aiWK3_AlgnFwBw        , //워크존 3 얼라인
    aiWK3_SttnUpDn        , //워크존 3 스테이션
    aiPSB_AlgnFwBw        , //포스트 버퍼 얼라인
    aiPSB_IndxUpDn        , //포스트버퍼 인데스핑거 업다운 ... 유주현
    aiPSB_FlprOpCl        , //포스트버퍼 플립퍼
    aiPSB_GripUpDn        , //포스트버퍼 백 그리퍼.
    aiPSB_PnchDnUp        , //포스트버퍼 펀치
    aiPSB_SortFwBw        , //포스트버퍼 소트.
    aiPSB_SrtStprFwBw     , //포스트버퍼 소트 스토퍼
    aiPSB_PshrFwBw        , //포스트버퍼 푸셔 전후진
    aiPSB_PshrDnUp        , //포스트버퍼 푸셔 업다운
    aiULD_ClmpUpDn        , //언로더 클램프
    MAX_ACTR
};

enum EN_ERR_ID {
/*000*/    eiETC_MainPower        = 0 ,
/*001*/    eiETC_MainAir              ,
/*002*/    eiETC_ToStartTO            ,
/*003*/    eiETC_ToStopTO             ,
/*004*/    eiETC_Emergency            ,
/*005*/    eiETC_FDoor                ,
/*006*/    eiETC_BDoor                ,
/*007*/    eiETC_LotEnd               ,
/*008*/    eiETC_AllHomeTO            ,
/*009*/    eiETC_ManCycleTO           ,
/*010*/    eiLDR_CycleTO              ,
/*011*/    eiPRB_CycleTO              ,
/*012*/    eiRAL_CycleTO              ,
/*013*/    eiHED_CycleTO              ,
/*014*/    eiPSB_CycleTO              ,
/*015*/    eiULD_CycleTO              ,
/*016*/    ei016                      ,
/*017*/    eiLDR_HomeTO               ,
/*018*/    eiPRB_HomeTO               ,
/*019*/    eiRAL_HomeTO               ,
/*020*/    eiHED_HomeTO               ,
/*021*/    eiPSB_HomeTO               ,
/*022*/    eiULD_HomeTO               ,
/*023*/    ei023                      ,
/*024*/    eiLDR_ToStartTO            ,
/*025*/    eiPRB_ToStartTO            ,
/*026*/    eiRAL_ToStartTO            ,
/*027*/    eiHED_ToStartTO            ,
/*028*/    eiPSB_ToStartTO            ,
/*029*/    eiULD_ToStartTO            ,
/*030*/    ei030                      ,
/*031*/    eiLDR_ToStopTO             ,
/*032*/    eiPRB_ToStopTO             ,
/*033*/    eiRAL_ToStopTO             ,
/*034*/    eiHED_ToStopTO             ,
/*035*/    eiPSB_ToStopTO             ,
/*036*/    eiULD_ToStopTO             ,
/*037*/    ei037                      ,
/*038*/    eiLDR_YCmpHomeEnd          ,
/*039*/    eiLDR_YCmpNegLim           ,
/*040*/    eiLDR_YCmpPosLim           ,
/*041*/    eiLDR_YCmpAlarm            ,
/*042*/    ei042                      ,
/*043*/    eiLDR_ZCmpHomeEnd          ,
/*044*/    eiLDR_ZCmpNegLim           ,
/*045*/    eiLDR_ZCmpPosLim           ,
/*046*/    eiLDR_ZCmpAlarm            ,
/*047*/    ei047                      ,
/*048*/    eiWRK_XVsnHomeEnd          ,
/*049*/    eiWRK_XVsnNegLim           ,
/*050*/    eiWRK_XVsnPosLim           ,
/*051*/    eiWRK_XVsnAlarm            ,
/*052*/    ei052                      ,
/*053*/    eiWRK_YVsnHomeEnd          ,
/*054*/    eiWRK_YVsnNegLim           ,
/*055*/    eiWRK_YVsnPosLim           ,
/*056*/    eiWRK_YVsnAlarm            ,
/*057*/    ei057                      ,
/*058*/    eiWR1_XCvrHomeEnd          ,
/*059*/    eiWR1_XCvrNegLim           ,
/*060*/    eiWR1_XCvrPosLim           ,
/*061*/    eiWR1_XCvrAlarm            ,
/*062*/    ei062                      ,
/*063*/    eiWR2_XCvrHomeEnd          ,
/*064*/    eiWR2_XCvrNegLim           ,
/*065*/    eiWR2_XCvrPosLim           ,
/*066*/    eiWR2_XCvrAlarm            ,
/*067*/    ei067                      ,
/*068*/    eiWR3_XCvrHomeEnd          ,
/*069*/    eiWR3_XCvrNegLim           ,
/*070*/    eiWR3_XCvrPosLim           ,
/*071*/    eiWR3_XCvrAlarm            ,
/*072*/    ei072                      ,
/*073*/    eiPSB_XTrmHomeEnd          ,
/*074*/    eiPSB_XTrmNegLim           ,
/*075*/    eiPSB_XTrmPosLim           ,
/*076*/    eiPSB_XTrmAlarm            ,
/*077*/    ei077                      ,
/*078*/    eiPSB_YTrmHomeEnd          ,
/*079*/    eiPSB_YTrmNegLim           ,
/*080*/    eiPSB_YTrmPosLim           ,
/*081*/    eiPSB_YTrmAlarm            ,
/*082*/    ei082                      ,
/*083*/    eiPSB_TFlpHomeEnd          ,
/*084*/    eiPSB_TFlpNegLim           ,
/*085*/    eiPSB_TFlpPosLim           ,
/*086*/    eiPSB_TFlpAlarm            ,
/*087*/    ei087                      ,
/*088*/    eiPSB_XInsHomeEnd          ,
/*089*/    eiPSB_XInsNegLim           ,
/*090*/    eiPSB_XInsPosLim           ,
/*091*/    eiPSB_XInsAlarm            ,
/*092*/    ei092                      ,
/*093*/    eiULD_YCmpHomeEnd          ,
/*094*/    eiULD_YCmpNegLim           ,
/*095*/    eiULD_YCmpPosLim           ,
/*096*/    eiULD_YCmpAlarm            ,
/*097*/    ei097                      ,
/*098*/    eiULD_ZCmpHomeEnd          ,
/*099*/    eiULD_ZCmpNegLim           ,
/*100*/    eiULD_ZCmpPosLim           ,
/*101*/    eiULD_ZCmpAlarm            ,
/*102*/    ei102                      ,
/*103*/    ei103                      ,
/*104*/    eiLDR_ClmpUpDnTO           ,
/*105*/    eiLDR_PshrFwBwTO           ,
/*106*/    eiPRB_StprUpDnTO           ,
/*107*/    eiWK1_StprUpDnTO           ,
/*108*/    eiWK1_AlgnFwBwTO           ,
/*109*/    eiWK1_SttnUpDnTO           ,
/*110*/    eiWK2_StprUpDnTO           ,
/*111*/    eiWK2_AlgnFwBwTO           ,
/*112*/    eiWK2_SttnUpDnTO           ,
/*113*/    eiWK3_StprUpDnTO           ,
/*114*/    eiWK3_AlgnFwBwTO           ,
/*115*/    eiWK3_SttnUpDnTO           ,
/*116*/    eiPSB_AlgnFwBwTO           ,
/*117*/    eiPSB_IndxUpDnTO           ,
/*118*/    eiPSB_FlprOpClTO           ,
/*119*/    eiPSB_GripUpDnTO           ,
/*120*/    eiPSB_PnchDnUpTO           ,
/*121*/    eiPSB_SortFwBwTO           ,
/*122*/    eiPSB_SrtStprFwBwTO        ,
/*123*/    eiPSB_PshrFwBwTO           ,
/*124*/    eiPSB_PshrDnUpTO           ,
/*125*/    eiULD_ClmpUpDnTO           ,
/*126*/    ei126                      ,
/*127*/    ei127                      ,
/*128*/    eiLDR_MgzDispr             ,
/*129*/    eiLDR_MgzUnknown           ,
/*130*/    eiPRB_PkgDispr             ,
/*131*/    eiPRB_PkgUnknown           ,
/*132*/    eiWR1_PkgDispr             ,
/*133*/    eiWR1_PkgUnknown           ,
/*134*/    eiWR2_PkgDispr             ,
/*135*/    eiWR2_PkgUnknown           ,
/*136*/    eiWR3_PkgDispr             ,
/*137*/    eiWR3_PkgUnknown           ,
/*138*/    eiPSB_PkgDispr             ,
/*139*/    eiPSB_PkgUnknown           ,
/*140*/    eiULD_MgzUnknown           ,
/*141*/    eiULD_MgzDispr             ,
/*142*/    ei142                      ,
/*143*/    ei143                      ,
/*144*/    eiPSB_VsnFailCnt           ,
/*145*/    eiPSB_VsnSFailCnt          ,
/*146*/    eiPSB_TrimFail             ,
/*147*/    eiHED_Visn1_1NotReady      ,
/*148*/    eiHED_Visn1_2NotReady      ,
/*149*/    eiHED_Visn2_1NotReady      ,
/*150*/    eiHED_Visn2_2NotReady      ,
/*151*/    eiHED_Visn3_1NotReady      ,
/*152*/    eiHED_Visn3_2NotReady      ,
/*153*/    ei153                      ,
/*154*/    ei154                      ,
/*155*/    eiLDR_PshrOverload         ,
/*156*/    eiLDR_MgzFull              ,
/*157*/    eiLDR_NeedMgz              ,
/*158*/    eiULD_MgzFull              ,
/*159*/    eiULD_NeedMgz              ,
/*160*/    ei160                      ,
/*161*/    ei161                      ,
/*162*/    eiHED_Visn1_1Read          ,
/*163*/    eiHED_Visn1_2Read          ,
/*164*/    eiHED_Visn2_1Read          ,
/*165*/    eiHED_Visn2_2Read          ,
/*166*/    eiHED_Visn3_1Read          ,
/*167*/    eiHED_Visn3_2Read          ,
/*168*/    eiHED_VisnResetCnt         ,
/*169*/    eiHED_VisnCommTO           ,
/*170*/    eiPSB_PusherMiss           ,
/*11**/    eiULD_PusherMiss           ,
/*172*/    eiWR1_IndexOverload        ,
/*173*/    eiPRB_StripFeedingFail     ,
/*174*/    eiPSB_TotalFailCnt         ,
/*175*/    eiPSB_SameFailCnt          ,
/*176*/    eiPSB_NoRjBox1             ,
/*177*/    eiPSB_NoRjBox2             ,
/*178*/    eiPSB_NoRjBox3             ,
/*179*/    eiPSB_SortPos              ,
/*180*/    eiLDR_BtmMgzDetect         ,
/*181*/    eiULD_BtmMgzDetect         ,
/*182*/    eiPSB_InsertOverload       ,
/*183*/    eiULD_OutFeedingFail       ,
/*184*/    eiULR_PkgDispr             ,
/*185*/    eiULR_PkgUnknown           ,
/*186*/    eiRAL_1Pkg                 ,
/*187*/    eiRAL_2Pkg                 ,
/*188*/    eiRAL_3Pkg                 ,
/*189*/    eiRAL_4Pkg                 ,
/*190*/    eiSameFailCnt0             ,
/*191*/    eiSameFailCnt1             ,
/*192*/    eiSameFailCnt2             ,
/*193*/    eiSameFailCnt3             ,
/*194*/    eiSameFailCnt4             ,
/*195*/    eiSameFailCnt5             ,
/*196*/    eiSameFailCnt6             ,
/*197*/    eiSameFailCnt7             ,
/*198*/    eiSameFailCnt8             ,
/*199*/    eiSameFailCnt9             ,
/*200*/    eiSameFailCntA             ,
/*201*/    eiSameFailCntB             ,
/*202*/    eiSameFailCntC             ,
/*203*/    eiSameFailCntD             ,
/*204*/    eiSameFailCntE             ,
/*205*/    eiSameFailCntF             ,
/*206*/    eiSameFailCntG             ,
/*207*/    eiSameFailCntH             ,
/*208*/    eiSameFailCntI             ,
/*209*/    eiSameFailCntJ             ,
/*210*/    eiSameFailCntK             ,
/*211*/    eiSameFailCntL             ,
/*212*/    eiPSB_MarkingTO            ,
/*213*/    eiULD_RemoveMgz            ,
    MAX_ERR
                                      
//  eiPSB_VsnSFailCnt           , //  ,
//  eiPSB_OutStripExist         , //
//  eiWR1_FlipperMoveFail       , //

};


enum EN_INPUT_ID {
    x0000 =  0 , xETC_LStartSw             =  0 , //
    x0001 =  1 , xETC_LStopSw              =  1 , //
    x0002 =  2 , xETC_LResetSw             =  2 , //
    x0003 =  3 , xETC_LMainAir             =  3 , //
    x0004 =  4 , xETC_LInitSw              =  4 , //
    x0005 =  5 , xETC_LVisnSw              =  5 , //
    x0006 =  6 , xETC_RStartSw             =  6 , //
    x0007 =  7 , xETC_RStopSw              =  7 , //
    x0008 =  8 , xETC_RResetSw             =  8 , //
    x0009 =  9 , xETC_RMainAir             =  9 , //
    x000A = 10 , xETC_RInitSw              = 10 , //
    x000B = 11 , xETC_RVisnSw              = 11 , //
    x000C = 12 , x012                      = 12 , //
    x000D = 13 , x013                      = 13 , //
    x000E = 14 , xETC_EmgSw1               = 14 , //
    x000F = 15 , xETC_EmgSw2               = 15 , //

    x0010 = 16 , x016                      = 16 , //
    x0011 = 17 , xETC_PowerOn              = 17 , //
    x0012 = 18 , xETC_MainSol              = 18 , //
    x0013 = 19 , xETC_FDoor                = 19 , //
    x0014 = 20 , xETC_BDoor                = 20 , //
    x0015 = 21 , x021                      = 21 , //
    x0016 = 22 , x022                      = 22 , //
    x0017 = 23 , x023                      = 23 , //
    x0018 = 24 , xPRB_PkgIn1               = 24 , //
    x0019 = 25 , xWR1_Align                = 25 , //
    x001A = 26 , xWR2_Align                = 26 , //
    x001B = 27 , xWR3_Align                = 27 , //
    x001C = 28 , xPSB_PusherUp             = 28 , //  //xPSB_PusherUp             = 28 , //
    x001D = 29 , xPSB_PusherDn             = 29 , //  //xPSB_PusherDn             = 29 , //
    x001E = 30 , xPSB_PusherFw             = 30 , //  //xPSB_PusherFw             = 30 , //
    x001F = 31 , xPSB_PusherBw             = 31 , //  //xPSB_PusherBw             = 31 , //

    x0020 = 32 , x032                      = 32 , //
    x0021 = 33 , xPSB_PkgIn2               = 33 , //
    x0022 = 34 , xPSB_Align                = 34 , //
    x0023 = 35 , xPSB_PkgOut3              = 35 , //
    x0024 = 36 , xPSB_PkgOut4              = 36 , //
    x0025 = 37 , x037                      = 37 , //
    x0026 = 38 , x038                      = 38 , //
    x0027 = 39 , x039                      = 39 , //
    x0028 = 40 , xPRB_StprFw               = 40 , //
    x0029 = 41 , xPRB_StprBw               = 41 , //
    x002A = 42 , xPRB_Pkg                  = 42 , //
    x002B = 43 , xWR1_StprFw               = 43 , //
    x002C = 44 , xWR1_StprBw               = 44 , //
    x002D = 45 , xWR1_Pkg                  = 45 , //
    x002E = 46 , xWR2_StprFw               = 46 , //
    x002F = 47 , xWR2_StprBw               = 47 , //

    x0030 = 48 , xWR2_Pkg                  = 48 , //
    x0031 = 49 , xWR3_StprFw               = 49 , //
    x0032 = 50 , xWR3_StprBw               = 50 , //
    x0033 = 51 , xWR3_Pkg                  = 51 , //
    x0034 = 52 , xWR1_LiftFw               = 52 , //
    x0035 = 53 , xWR1_LiftBw               = 53 , //
    x0036 = 54 , xWR2_LiftFw               = 54 , //
    x0037 = 55 , xWR2_LiftBw               = 55 , //
    x0038 = 56 , xWR3_LiftFw               = 56 , //
    x0039 = 57 , xWR3_LiftBw               = 57 , //
    x003A = 58 , xPSB_InsertUp             = 58 , //
    x003B = 59 , xPSB_InsertDn             = 59 , //
    x003C = 60 , xPSB_InsertOverload       = 60 , //
    x003D = 61 , x061                      = 61 , //
    x003E = 62 , x062                      = 62 , //
    x003F = 63 , x063                      = 63 , //

    x0040 = 64 , xPSB_SortFw               = 64 , // 소팅 실린더.
    x0041 = 65 , xPSB_SortMd               = 65 , //
    x0042 = 66 , xPSB_SortBw               = 66 , //
    x0043 = 67 , xPSB_SortStprFw           = 67 , //
    x0044 = 68 , xPSB_SortStprBw           = 68 , //
    x0045 = 69 , xPSB_LEDSortFull          = 69 , //
    x0046 = 70 , xPSB_RejectBox1           = 70 , //
    x0047 = 71 , xPSB_RejectBox2           = 71 , //
    x0048 = 72 , xPSB_RejectBox3           = 72 , //
    x0049 = 73 , x073                      = 73 , //
    x004A = 74 , xPSB_PunchUp              = 74 , //
    x004B = 75 , xPSB_PunchDn              = 75 , //
    x004C = 76 , xPSB_FlpprOpen            = 76 , //
    x004D = 77 , xPSB_FlpprClose           = 77 , //
    x004E = 78 , xPSB_GrprUp               = 78 , //
    x004F = 79 , xPSB_GrprDn               = 79 , //
    
    x0050 = 80 , x080                      = 80 , //
    x0051 = 81 , x081                      = 81 , //
    x0052 = 82 , x082                      = 82 , //
    x0053 = 83 , x083                      = 83 , //
    x0054 = 84 , x084                      = 84 , //
    x0055 = 85 , x085                      = 85 , //
    x0056 = 86 , x086                      = 86 , //
    x0057 = 87 , x087                      = 87 , //
    x0058 = 88 , x088                      = 88 , //
    x0059 = 89 , x089                      = 89 , //
    x005A = 90 , x090                      = 90 , //
    x005B = 91 , x091                      = 91 , //
    x005C = 92 , x092                      = 92 , //
    x005D = 93 , x093                      = 93 , //
    x005E = 94 , x094                      = 94 , //
    x005F = 95 , x095                      = 95 , //
    //Visin Part
    x0060 = 96 , xVisn1_1Ready             = 96 , //
    x0061 = 97 , xVisn1_1Busy              = 97 , //
    x0062 = 98 , xVisn1_1End               = 98 , //
    x0063 = 99 , xVisn1_1Spare1            = 99 , //
    x0064 =100 , xVisn1_1Spare2            =100 , //
    x0065 =101 , xVisn1_2Ready             =101 , //
    x0066 =102 , xVisn1_2Busy              =102 , //
    x0067 =103 , xVisn1_2End               =103 , //
    x0068 =104 , xVisn1_2Spare1            =104 , //
    x0069 =105 , xVisn1_2Spare2            =105 , //
    x006A =106 , xVisn2_1Ready             =106 , //
    x006B =107 , xVisn2_1Busy              =107 , //
    x006C =108 , xVisn2_1End               =108 , //
    x006D =109 , xVisn2_1Spare1            =109 , //
    x006E =110 , xVisn2_1Spare2            =110 , //
    x006F =111 , xVisn2_2Ready             =111 , //
    
    x0070 =112 , xVisn2_2Busy              =112 , //
    x0071 =113 , xVisn2_2End               =113 , //
    x0072 =114 , xVisn2_2Spare1            =114 , //
    x0073 =115 , xVisn2_2Spare2            =115 , //
    x0074 =116 , xVisn3_1Ready             =116 , //
    x0075 =117 , xVisn3_1Busy              =117 , //
    x0076 =118 , xVisn3_1End               =118 , //
    x0077 =119 , xVisn3_1Spare1            =119 , //
    x0078 =120 , xVisn3_1Spare2            =120 , //
    x0079 =121 , xVisn3_2Ready             =121 , //
    x007A =122 , xVisn3_2Busy              =122 , //
    x007B =123 , xVisn3_2End               =123 , //
    x007C =124 , xVisn3_2Spare1            =124 , //
    x007D =125 , xVisn3_2Spare2            =125 , //
    x007E =126 , x126                      =126 , //
    x007F =127 , x127                      =127 , //
    // Loader Part
    x0080 =128 , xLDR_PshrOverLoad         =128 , //
    x0081 =129 , xLDR_PshrFw               =129 , //
    x0082 =130 , xLDR_PshrBw               =130 , //
    x0083 =131 , xLDR_MgzDetect1           =131 , //
    x0084 =132 , xLDR_MgzDetect2           =132 , //
    x0085 =133 , xLDR_ClampFw              =133 , //
    x0086 =134 , xLDR_MgzClamp1            =134 , //
    x0087 =135 , x135                      =135 , //
    x0088 =136 , xLDR_MgzIn                =136 , //
    x0089 =137 , xLDR_MgzOutFull           =137 , //
    x008A =138 , x138                      =138 , //
    x008B =139 , x139                      =139 , //
    x008C =140 , x140                      =140 , //
    x008D =141 , x141                      =141 , //
    x008E =142 , xETC_LEmgLDRSw            =142 , //
    x008F =143 , x143                      =143 , //
    // UnLoader Part
    x0090 =144 , xULD_MgzDetect1           =144 , //
    x0091 =145 , xULD_MgzDetect2           =145 , //
    x0092 =146 , xULD_ClampFw              =146 , //
    x0093 =147 , xULD_MgzClamp1            =147 , //
    x0094 =148 , x148                      =148 , //
    x0095 =149 , xULD_MgzOutFull           =149 , //
    x0096 =150 , xULD_MgzIn                =150 , //
    x0097 =151 , x151                      =151 , //
    x0098 =152 , x152                      =152 , //
    x0099 =153 , x153                      =153 , //
    x009A =154 , xETC_REmgULDSw            =154 , //
    x009B =155 , x155                      =155 , //
    x009C =156 , x156                      =156 , //
    x009D =157 , x157                      =157 , //
    x009E =158 , x158                      =158 , //
    x009F =159 , x159                      =159 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
//    yETC_PunchLp     = 0 ,
//    yETC_PassLp      = 0 ,
//    yETC_ReviewEndLp = 0 ,

    y0000 =  0 ,  yETC_LStartLp             =  0 , //
    y0001 =  1 ,  yETC_LStopLp              =  1 , //
    y0002 =  2 ,  yETC_LResetLp             =  2 , //
    y0003 =  3 ,  yETC_LAirLp               =  3 , //
    y0004 =  4 ,  yETC_LInitLp              =  4 , //
    y0005 =  5 ,  yETC_LVisnLp              =  5 , //
    y0006 =  6 ,  yETC_RStartLp             =  6 , //
    y0007 =  7 ,  yETC_RStopLp              =  7 , //
    y0008 =  8 ,  yETC_RResetLp             =  8 , //
    y0009 =  9 ,  yETC_RAirLp               =  9 , //
    y000A = 10 ,  yETC_RInitLp              = 10 , //
    y000B = 11 ,  yETC_RVisnLp              = 11 , //
    y000C = 12 ,  yETC_TwRedLp              = 12 , //
    y000D = 13 ,  yETC_TwYelLp              = 13 , //
    y000E = 14 ,  yETC_TwGrnLp              = 14 , //
    y000F = 15 ,  yETC_TwBzz                = 15 , //

    y0010 = 16 ,  yETC_MainSol              = 16 , //yULD_MgzOutAC
    y0011 = 17 ,  yPRB_AirBlower            = 17 , //yULD_MgzInAC
    y0012 = 18 ,  yPRB_FeedingAC            = 18 , //yULD_ClampBw
    y0013 = 19 ,  yWRK_FeedingAC            = 19 , //yULD_ClampFw
    y0014 = 20 ,  x020                      = 20 , //yULD_ZBreak
    y0015 = 21 ,  y021                      = 21 , //
    y0016 = 22 ,  y022                      = 22 , //
    y0017 = 23 ,  y023                      = 23 , //
    y0018 = 24 ,  yWR1_Align                = 24 , //
    y0019 = 25 ,  yWR2_Align                = 25 , //
    y001A = 26 ,  yWR3_Align                = 26 , //
    y001B = 27 ,  yPSB_Align                = 27 , //
    y001C = 28 ,  yPSB_PusherFB             = 28 , //
    y001D = 29 ,  yPSB_PusherUD             = 29 , //
    y001E = 30 ,  y030                      = 30 , //
    y001F = 31 ,  y031                      = 31 , //

    y0020 = 32 ,  yPRB_Stpr                 = 32 , //
    y0021 = 33 ,  yWR1_Stpr                 = 33 , //
    y0022 = 34 ,  yWR2_Stpr                 = 34 , //
    y0023 = 35 ,  yWR3_Stpr                 = 35 , //
    y0024 = 36 ,  yPSB_PuncUpDn             = 36 , // 왜 여깃는지...-_-+ dd
    y0025 = 37 ,  y037                      = 37 , //
    y0026 = 38 ,  y039                      = 38 , ////y038                      = 38 , //
    y0027 = 39 ,  yy039                     = 39 , //
    y0028 = 40 ,  yWR1_LiftFw               = 40 , //
    y0029 = 41 ,  yWR1_LiftBw               = 41 , //
    y002A = 42 ,  yWR2_LiftFw               = 42 , //
    y002B = 43 ,  yWR2_LiftBw               = 43 , //
    y002C = 44 ,  yWR3_LiftFw               = 44 , //
    y002D = 45 ,  yWR3_LiftBw               = 45 , //
    y002E = 46 ,  y046                      = 46 , //
    y002F = 47 ,  y047                      = 47 , //

    y0030 = 48 ,  yPSB_IndxUpDn             = 48 , // Finger
    y0031 = 49 ,  yPSB_SortIn               = 49 , //
    y0032 = 50 ,  yPSB_SortOut              = 50 , //
    y0033 = 51 ,  yPSB_SortStpr             = 51 , //
    y0034 = 52 ,  yPSB_GrprUpDn             = 52 , //
    y0035 = 53 ,  yPSB_FlpprOpCl            = 53 , //
    y0036 = 54 ,  y054                      = 54 , //
    y0037 = 55 ,  y055                      = 55 , //
    y0038 = 56 ,  y056                      = 56 , //
    y0039 = 57 ,  y057                      = 57 , //
    y003A = 58 ,  y058                      = 58 , //
    y003B = 59 ,  y059                      = 59 , //
    y003C = 60 ,  y060                      = 60 , //
    y003D = 61 ,  y061                      = 61 , //
    y003E = 62 ,  y062                      = 62 , //
    y003F = 63 ,  y063                      = 63 , //

    // Vision
    y0040 = 64 ,  yVisn1_1Spare             = 64 , //
    y0041 = 65 ,  yVisn1_1LotStart          = 65 , //
    y0042 = 66 ,  yVisn1_1Reset             = 66 , //
    y0043 = 67 ,  yVisn1_1JobChange         = 67 , //
    y0044 = 68 ,  yVisn1_1ESReady           = 68 , //
    y0045 = 69 ,  yVisn1_2Spare             = 69 , //
    y0046 = 70 ,  yVisn1_2LotStart          = 70 , //
    y0047 = 71 ,  yVisn1_2Reset             = 71 , //
    y0048 = 72 ,  yVisn1_2JobChange         = 72 , //
    y0049 = 73 ,  yVisn1_2ESReady           = 73 , //
    y004A = 74 ,  yVisn2_1Spare             = 74 , //
    y004B = 75 ,  yVisn2_1LotStart          = 75 , //
    y004C = 76 ,  yVisn2_1Reset             = 76 , //
    y004D = 77 ,  yVisn2_1JobChange         = 77 , //
    y004E = 78 ,  yVisn2_1ESReady           = 78 , //
    y004F = 79 ,  yVisn2_2Spare             = 79 , //

    y0050 = 80 ,  yVisn2_2LotStart          = 80 , //
    y0051 = 81 ,  yVisn2_2Reset             = 81 , //
    y0052 = 82 ,  yVisn2_2JobChange         = 82 , //
    y0053 = 83 ,  yVisn2_2ESReady           = 83 , //
    y0054 = 84 ,  yVisn3_1Spare             = 84 , //
    y0055 = 85 ,  yVisn3_1LotStart          = 85 , //
    y0056 = 86 ,  yVisn3_1Reset             = 86 , //
    y0057 = 87 ,  yVisn3_1JobChange         = 87 , //
    y0058 = 88 ,  yVisn3_1ESReady           = 88 , //
    y0059 = 89 ,  yVisn3_2Spare             = 89 , //
    y005A = 90 ,  yVisn3_2LotStart          = 90 , //
    y005B = 91 ,  yVisn3_2Reset             = 91 , //
    y005C = 92 ,  yVisn3_2JobChange         = 92 , //
    y005D = 93 ,  yVisn3_2ESReady           = 93 , //
    y005E = 94 ,  y094                      = 94 , //
    y005F = 95 ,  y095                      = 95 , //

    // Loader
    y0060 = 96 ,  yLDR_MgzInAC              = 96 , //
    y0061 = 97 ,  yLDR_MgzOutAC             = 97 , //
    y0062 = 98 ,  yLDR_ClampBw              = 98 , //
    y0063 = 99 ,  yLDR_ClampFw              = 99 , //
    y0064 =100 ,  yLDR_PusherFw             =100 , //
    y0065 =101 ,  yLDR_ZBreak               =101 , //
    y0066 =102 ,  y102                      =102 , //
    y0067 =103 ,  y103                      =103 , //
    y0068 =104 ,  y104                      =104 , //
    y0069 =105 ,  y105                      =105 , //
    y006A =106 ,  y106                      =106 , //
    y006B =107 ,  y107                      =107 , //
    y006C =108 ,  y108                      =108 , //
    y006D =109 ,  y109                      =109 , //
    y006E =110 ,  y110                      =110 , //
    y006F =111 ,  y111                      =111 , //

    // UnLoader
    y0070 =112 ,  yULD_MgzOutAC             =112 , //
    y0071 =113 ,  yULD_MgzInAC              =113 , //
    y0072 =114 ,  yULD_ClampBw              =114 , //
    y0073 =115 ,  yULD_ClampFw              =115 , //
    y0074 =116 ,  yULD_ZBreak               =116 , //
    y0075 =117 ,  y117                      =117 , //
    y0076 =118 ,  y118                      =118 , //
    y0077 =119 ,  y119                      =119 , //
    y0078 =120 ,  y120                      =120 , //
    y0079 =121 ,  y121                      =121 , //
    y007A =122 ,  y122                      =122 , //
    y007B =123 ,  y123                      =123 , //
    y007C =124 ,  y124                      =124 , //
    y007D =125 ,  y125                      =125 , //
    y007E =126 ,  y126                      =126 , //
    y007F =127 ,  y127                      =127 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID { //프로그램에서 가야할 아이디...
    piWRK_XVsnWait       = 0 , //
    piWRK_XVsnWorkStart      , //트리거 시작 셑업 포지션.
    piWRK_XVsnRWorkStart     , //오른쪽비젼만 쓸때 시작 위치.
    piWRK_XVsnLWorkEnd       , //왼쪽비젼만 쓸때 끝 위치.
    piWRK_XVsnTrgStt         , //트리거 시작 셑업 포지션에서 조금 덜가서.
    piWRK_XVsnTrgEnd         , //트리거 엔드 셑업 포지션에서 조금 더가서.
    piWRK_XVsnOffsetWrkStt   , //트리거 시작 셋업 포지션에서 오프셋 뺌.dd
    piWRK_XVsnWorkEnd        , //트리거 엔드 셑업 포지션.
    MAX_PSTN_ID_MOTR0        , //miWRK_XVsn

    piWRK_YVsnWait       = 0 ,
    piWRK_YVsnWorkStart      ,
    piWRK_YVsnWork           ,
    MAX_PSTN_ID_MOTR1        , //miWRK_YVsn

    piPSB_XTrmWait       = 0 ,
    piPSB_XTrmWorkStart      ,
    piPSB_XTrmRslt0          ,
    piPSB_XTrmRslt1          ,
    piPSB_XTrmRslt2          ,
    piPSB_XTrmRslt3          ,
    piPSB_XTrmRslt4          ,
    piPSB_XTrmRslt5          ,
    piPSB_XTrmRslt6          ,
    piPSB_XTrmRslt7          ,
    piPSB_XTrmRslt8          ,
    piPSB_XTrmRslt9          ,
    piPSB_XTrmRsltA          ,
    piPSB_XTrmRsltB          ,
    piPSB_XTrmRsltC          ,
    piPSB_XTrmRsltD          ,
    piPSB_XTrmRsltE          ,
    piPSB_XTrmRsltF          ,
    piPSB_XTrmRsltG          ,
    piPSB_XTrmRsltH          ,
    piPSB_XTrmRsltI          ,
    piPSB_XTrmRsltJ          ,
    piPSB_XTrmRsltK          ,
    piPSB_XTrmRsltL          ,
    piPSB_XVsnRslt           ,
    piPSB_XVsnRsltMM         ,
    MAX_PSTN_ID_MOTR2        , //miPSB_XTrm

    piPSB_YTrmWait       = 0 ,
    piPSB_YTrmWorkStart      ,
    piPSB_YTrmRslt0          ,
    piPSB_YTrmRslt1          ,
    piPSB_YTrmRslt2          ,
    piPSB_YTrmRslt3          ,
    piPSB_YTrmRslt4          ,
    piPSB_YTrmRslt5          ,
    piPSB_YTrmRslt6          ,
    piPSB_YTrmRslt7          ,
    piPSB_YTrmRslt8          ,
    piPSB_YTrmRslt9          ,
    piPSB_YTrmRsltA          ,
    piPSB_YTrmRsltB          ,
    piPSB_YTrmRsltC          ,
    piPSB_YTrmRsltD          ,
    piPSB_YTrmRsltE          ,
    piPSB_YTrmRsltF          ,
    piPSB_YTrmRsltG          ,
    piPSB_YTrmRsltH          ,
    piPSB_YTrmRsltI          ,
    piPSB_YTrmRsltJ          ,
    piPSB_YTrmRsltK          ,
    piPSB_YTrmRsltL          ,
    piPSB_YVsnRslt           ,
    piPSB_YVsnRsltMM         ,
    MAX_PSTN_ID_MOTR3        , //miPSB_YTrm

    piPSB_TFlpWait       = 0 ,
    piPSB_TFlpWork           ,
    MAX_PSTN_ID_MOTR4        , //miPSB_TFlp

    piPSB_XInsWait       = 0 ,
    piPSB_XInsInsert         ,
    piPSB_XInsOut            ,
    MAX_PSTN_ID_MOTR5        , //miPSB_XIns

    piWR1_XCvrWait       = 0 ,
    piWR1_XCvrWork           ,
    MAX_PSTN_ID_MOTR6        , //miWK1_XCvr

    piWR2_XCvrWait       = 0 ,
    piWR2_XCvrWork           ,
    MAX_PSTN_ID_MOTR7        , //miWK2_XCvr

    piWR3_XCvrWait       = 0 ,
    piWR3_XCvrWork           ,
    MAX_PSTN_ID_MOTR8        , //miWK3_XCvr

    piMT_Spare           = 0 , //스페어 파트
    MAX_PSTN_ID_MOTR9        , //

    piLDR_YWait         =  0 ,  //
    piLDR_YWork              ,  //
    piLDR_YPick              ,  //
    piLDR_YPlace             ,  //
    MAX_PSTN_ID_MOTR10       ,  //

    piLDR_ZWait         =  0 ,  //
    piLDR_ZPickFwd           ,  //
    piLDR_ZClampOn           ,  //
    piLDR_ZPickBwd           ,  //
    piLDR_ZWorkStart         ,  //맨 아래 슬롯 기준.
    piLDR_ZPlaceFwd          ,  //
    piLDR_ZClampOff          ,  //
    piLDR_ZPlaceBwd          ,  //
    piLDR_ZWork              ,  //
    MAX_PSTN_ID_MOTR11       ,  //

    piULD_YWait         =  0 ,  //
    piULD_YWork              ,  //
    piULD_YPick              ,  //
    piULD_YPlace             ,  //
    MAX_PSTN_ID_MOTR12       ,  //

    piULD_ZWait         =  0 ,  //
    piULD_ZPickFwd           ,  //
    piULD_ZClampOn           ,  //
    piULD_ZPickBwd           ,  //
    piULD_ZWorkStart         ,  //
    piULD_ZPlaceFwd          ,  //
    piULD_ZClampOff          ,  //
    piULD_ZPlaceBwd          ,  //
    piULD_ZWork              ,  //
    MAX_PSTN_ID_MOTR13       ,  //

};

enum EN_PSTN_VALUE {
    pvWRK_XVsnWait       = 0 ,
    pvWRK_XVsnWorkStart      ,
    //pvWRK_XVsnWorkEnd        ,
    pvWRK_XVsnRWorkStart     , //오른쪽비젼만 쓸때 시작 위치.
    pvWRK_XVsnLWorkEnd       , //왼쪽비젼만 쓸때 끝 위치.
    MAX_PSTN_MOTR0           , //miWRK_XVsn

    pvWRK_YVsnWait       = 0 ,
    pvWRK_YVsnWorkStart      ,
    MAX_PSTN_MOTR1           , //miWRK_YVsn

    pvPSB_XTrmWait       = 0 ,
    pvPSB_XTrmWorkStart      ,
    MAX_PSTN_MOTR2           , //miPSB_XTrm

    pvPSB_YTrmWait       = 0 ,
    pvPSB_YTrmWorkStart      ,
    MAX_PSTN_MOTR3           , //miPSB_YTrm

    pvPSB_TFlpWait       = 0 ,
    pvPSB_TFlpWork           ,
    MAX_PSTN_MOTR4           , //miPSB_TFlp

    pvPSB_XInsWait       = 0 ,
    pvPSB_XInsInsert         ,
    pvPSB_XInsOut            ,
    MAX_PSTN_MOTR5           , //miPSB_XIns

    pvWR1_XCvrWait       = 0 ,
    pvWR1_XCvrWork           ,
    MAX_PSTN_MOTR6           , //miWK1_XCvr

    pvWR2_XCvrWait       = 0 ,
    pvWR2_XCvrWork           ,
    MAX_PSTN_MOTR7           , //miWK2_XCvr

    pvWR3_XCvrWait       = 0 ,
    pvWR3_XCvrWork           ,
    MAX_PSTN_MOTR8           , //miWK3_XCvr

    pvMT_Spare          = 0 , //스페어 파트
    MAX_PSTN_MOTR9          , //
    
    pvLDR_YWait         =  0 , //
    pvLDR_YWork              , //
    pvLDR_YPick              , //
    pvLDR_YPlace             , //
    MAX_PSTN_MOTR10          , //

    pvLDR_ZWait         =  0 , //
    pvLDR_ZPickFwd           , //
    pvLDR_ZClampOn           , //
    pvLDR_ZPickBwd           , //
    pvLDR_ZWorkStart         , //
    pvLDR_ZPlaceFwd          , //
    pvLDR_ZClampOff          , //
    pvLDR_ZPlaceBwd          , //
    MAX_PSTN_MOTR11          , //

    pvULD_YWait         =  0 ,
    pvULD_YWork              ,
    pvULD_YPick              ,
    pvULD_YPlace             ,
    MAX_PSTN_MOTR12          ,

    pvULD_ZWait         =  0 ,
    pvULD_ZPickFwd           ,
    pvULD_ZClampOn           ,
    pvULD_ZPickBwd           ,
    pvULD_ZWorkStart         ,
    pvULD_ZPlaceFwd          ,
    pvULD_ZClampOff          ,
    pvULD_ZPlaceBwd          ,
    MAX_PSTN_MOTR13          ,

//    MAX_PSTN_MOTR15      = 0 , //miULD_Spar
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            = 0   ,
    mcAllHome              = 999 ,

    mcLDR_Home             = 1 ,
    mcLDR_Supply           = 2  , //
    mcLDR_Pick             = 3  , //
    mcLDR_SttPstn          = 4  , //
    mcLDR_Place            = 5  , //
    mcLDR_UpSlPitch        = 6  , //
    mcLDR_DnSlPitch        = 7  , //
    mcLDR_Pusher           = 8  , //
    mcLDR_Clamp            = 9  , //
    mcLDR_TranInAC         = 10 , //
    mcLDR_TranOutAC        = 11 , //   = 1  , //
    mcRAL_PreStprUp        = 12 , //
    mcRAL_PreStprDn        = 13 , //

    mcRAL_Home             = 20  , //
    mcRAL_WR1LiftUp        = 21  , //
    mcRAL_WR1LiftDn        = 22  , //
    mcRAL_WR1StprUp        = 23  , //
    mcRAL_WR1StprDn        = 24  , //
    mcRAL_WR1AlgnFw        = 25  , //
    mcRAL_WR1AlgnBw        = 26  , //
    mcRAL_WR2LiftUp        = 27  , //
    mcRAL_WR2LiftDn        = 28  , //
    mcRAL_WR2StprUp        = 29  , //
    mcRAL_WR2StprDn        = 30  , //
    mcRAL_WR2AlgnFw        = 31  , //
    mcRAL_WR2AlgnBw        = 32  , //
    mcRAL_WR3LiftUp        = 33  , //
    mcRAL_WR3LiftDn        = 34  , //
    mcRAL_WR3StprUp        = 35  , //
    mcRAL_WR3StprDn        = 36  , //
    mcRAL_WR3AlgnFw        = 37  , //
    mcRAL_WR3AlgnBw        = 38  , //
    mcHED_WorkFstPs        = 39  , //
    mcRAL_VisnYZeroPs      = 40  , //

    mcPSB_Home             = 60  , //
    mcPSB_TWaitPs          = 61  , //
    mcPSB_TFirstPs         = 62  , //
    mcPSB_TFlpCmpUp        = 63  , //
    mcPSB_TFlpCmpDn        = 64  , //
    mcPSB_TAlignFw         = 65  , //
    mcPSB_TAlignBw         = 66  , //
    mcPSB_TFlpNorPs        = 67  , //
    mcPSB_TFlpInsPs        = 68  , //
    mcPSB_TTrimAllTest     = 69  , //
    mcPSB_TTrimUpDn        = 70  , //
    mcPSB_TTrimXMv         = 71  , //
    mcPSB_TTrimYMv         = 72  , //
    mcPSB_IdxUp            = 73  , //
    mcPSB_IdxDn            = 74  , //

    mcULD_Home             = 90  , //
    mcULD_Supply           = 91  , //
    mcULD_Pick             = 92  , //
    mcULD_SttPstn          = 93  , //
    mcULD_Place            = 94  , //
    mcULD_UpSlPitch        = 95  , //
    mcULD_DnSlPitch        = 96  , //
    mcULD_Clamp            = 97  , //
    mcPSB_PshrUpDn         = 98  ,
    mcPSB_PshrFwBw         = 99  ,
    mcULD_TranInAC         = 100 , //
    mcULD_TranOutAC        = 101 , //

    mcETC_Sampling1Cycle   = 120 , //
    mcETC_AllBeltOn        = 121 , //
    mcETC_AllBeltOff       = 122 , //
    mcETC_AllStprUp        = 123 , //
    mcETC_AllStprDn        = 124 , //
    mcETC_Wk1AlignLiftUp   = 125 , //
    mcETC_Wk2AlignLiftUp   = 126 , //
    mcETC_Wk3AlignLiftUp   = 127 , //
    mcETC_InsAlignFlp      = 128 , //
    mcETC_AllAlignLiftUp   = 129 , //
    mcETC_AllLiftDn        = 130 , //
    mcETC_MainAirOnOff     = 131 , //
    mcETC_ManLotEnd        = 132 , //
    mcETC_AllStripOut      = 133 , //
    mcETC_1CycleMv         = 134 , //

    MAX_MANUAL_CYCLE
};
#endif


