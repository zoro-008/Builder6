#ifndef UTILDEFINEH
#define UTILDEFINEH

#define EQP_SERIAL_LIST \
"\
ITW-14-1025,\
"

//Rs232 ID
enum EN_RS232 {
    rsLaser        = 0 , //com1     PortNo = 0 , BaudRate = 57600 , ByteSize = 8, StopBits = 1 , Parity = None

    MAX_RS232
};

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

//파트 아이디.
enum EN_PART_ID {
    piPRB = 0  , //프리버퍼
    piRAL      , //레이져,비젼 밑에 레일
    piLSR      , //레이져
    piVSN      , //비젼검사
    piPSB      , //포스트버퍼
    piLDR      , //로더.
    piULD      , //언로더.
    MAX_PART
};

enum EN_ARAY_ID {
    riPRB  =0 , //프리버퍼
    riLSR     , //레이져 워크존
    riVSN     , //비젼존.
    riPSB     , //포스트버퍼 리웍존.
    riLDR     , //로더
    riULD     , //언로더.
    MAX_ARAY
};

//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail

//mi<파트명3자리>_<축방샹1자리><부가명칭4>
enum EN_MOTR_ID {
    miLSR_YAlVs  = 0  , //얼라인 비젼
    miRAL_YCvsn  = 1  , //레일 컨버젼
    miVSN_XInsp  = 2  , //비젼 검사
    miVSN_YInsp  = 3  , //비젼 검사
    miPSB_XIndx  = 4  , //포스트버퍼 인덱스
    miRAL_XLStp  = 5  , //레이져 스토퍼
    miRAL_XVStp  = 6  , //비젼 스토퍼
    miETC_Spare  = 7  , //스패어
    miLDR_YClmp  = 8  , //로더
    miLDR_ZClmp  = 9  ,
    miULD_YClmp  = 10 ,
    miULD_ZClmp  = 11 ,

    MAX_MOTR
};

//ai<파트명3자리>_<부가명칭><FWD시에위치2자리><BWD시에위치2자리>
//대기가 Bwd 작동이 Fwd
enum EN_ACTR_ID {
    aiPRB_StprUpDn  = 0 ,
    aiRAL_LAlnFwBw      ,
    aiRAL_LStpUpDn      ,            
    aiRAL_LStnUpDn      ,
    aiRAL_VAlnFwBw      ,
    aiRAL_VStpUpDn      ,
    aiRAL_VStnUpDn      ,
    aiPSB_GrprGrRl      ,
    aiPSB_RailOpCl      ,
    aiPSB_IndxDnUp      ,
    aiPSB_ElvtUpDn      ,
    aiPSB_PshrDnUp      ,
    aiPSB_PshrFwBw      ,
    aiLDR_ClmpUpDn      ,
    aiLDR_PshrFwBw      ,
    aiULD_ClmpUpDn      ,

    MAX_ACTR

};

//그냥 아이오.  x+<파트 3자리>+_+<세부설명>
//실린더 아이오 x+<파트 3자리>+_+<세부설명>+<해당행동2자리 ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_LStartSw           =  0 , //
    x0001 =  1 , xETC_LStopSw            =  1 , //
    x0002 =  2 , xETC_LResetSw           =  2 , //
    x0003 =  3 , xETC_LAirSw             =  3 , //
    x0004 =  4 , xETC_LInitSw            =  4 , //
    x0005 =  5 , xETC_RStartSw           =  5 , //
    x0006 =  6 , xETC_RStopSw            =  6 , //
    x0007 =  7 , xETC_RResetSw           =  7 , //
    x0008 =  8 , xETC_RAirSw             =  8 , //
    x0009 =  9 , xETC_RInitSw            =  9 , //
    x000A = 10 , xETC_CStartSw           = 10 , //
    x000B = 11 , xETC_CStopSw            = 11 , //
    x000C = 12 , xETC_CResetSw           = 12 , //
    x000D = 13 , xETC_CVisnSw            = 13 , //
    x000E = 14 , xETC_CMarkSw            = 14 , //
    x000F = 15 , x014                    = 15 , //

    x0010 = 16 , xETC_FDoor              = 16 , //
    x0011 = 17 , xETC_BDoor              = 17 , //
    x0012 = 18 , xPRB_PkgIn1             = 18 , //
    x0013 = 19 , xRAL_LStnUp             = 19 , //
    x0014 = 20 , xRAL_LStnDn             = 20 , //
    x0015 = 21 , xRAL_VStnUp             = 21 , //
    x0016 = 22 , xRAL_VStnDn             = 22 , //
    x0017 = 23 , xPRB_StpUp              = 23 , //
    x0018 = 24 , xPRB_StpDn              = 24 , //
    x0019 = 25 , xPRB_Pkg2               = 25 , //
    x001A = 26 , xRAL_VStpUp             = 26 , //
    x001B = 27 , xRAL_VStpDn             = 27 , //
    x001C = 28 , xRAL_VPkg4              = 28 , //
    x001D = 29 , xRAL_LStpUp             = 29 , //
    x001E = 30 , xRAL_LStpDn             = 30 , //
    x001F = 31 , xRAL_LPkg3              = 31 , //

    x0020 = 32 , xRAL_LMask              = 32 , //
    x0021 = 33 , xRAL_LAlgBw             = 33 , //
    x0022 = 34 , xPSB_GrpRl              = 34 , //
    x0023 = 35 , xPSB_GrpGr              = 35 , //
    x0024 = 36 , xRAL_VMask              = 36 , //
    x0025 = 37 , xPSB_PkgIn5             = 37 , //
    x0026 = 38 , xRAL_VAlnBw             = 38 , //
    x0027 = 39 , xPSB_RalCl              = 39 , //
    x0028 = 40 , xPSB_RalOp              = 40 , //
    x0029 = 41 , xPSB_ElvUp              = 41 , //
    x002A = 42 , xPSB_ElvDn              = 42 , //
    x002B = 43 , xPSB_RjcPkg             = 43 , //
    x002C = 44 , xPSB_Pkg5               = 44 , //
    x002D = 45 , xPSB_PkgOut6            = 45 , //
    x002E = 46 , xPSB_IdxUp              = 46 , //
    x002F = 47 , xPSB_IdxDn              = 47 , //

    x0030 = 48 , xPSB_PshFw              = 48 , //
    x0031 = 49 , xPSB_PshBw              = 49 , //
    x0032 = 50 , xPSB_PshUp              = 50 , //
    x0033 = 51 , xPSB_PshDn              = 51 , //
    x0034 = 52 , xPSB_IdxOvld            = 52 , //
    x0035 = 53 , x053                    = 53 , //
    x0036 = 54 , xETC_MainAir            = 54 , //
    x0037 = 55 , xETC_PowerOn            = 55 , //
    x0038 = 56 , x056                    = 56 , //
    x0039 = 57 , x057                    = 57 , //
    x003A = 58 , x058                    = 58 , //
    x003B = 59 , x059                    = 59 , //
    x003C = 60 , x060                    = 60 , //
    x003D = 61 , x061                    = 61 , //
    x003E = 62 , x062                    = 62 , //
    x003F = 63 , x063                    = 63 , //

    x0040 = 64 , x064                    = 64 , //
    x0041 = 65 , x065                    = 65 , //
    x0042 = 66 , x066                    = 66 , //
    x0043 = 67 , x067                    = 67 , //
    x0044 = 68 , x068                    = 68 , //
    x0045 = 69 , x069                    = 69 , //
    x0046 = 70 , x070                    = 70 , //
    x0047 = 71 , x071                    = 71 , //
    x0048 = 72 , x072                    = 72 , //
    x0049 = 73 , x073                    = 73 , //
    x004A = 74 , x074                    = 74 , //
    x004B = 75 , x075                    = 75 , //
    x004C = 76 , x076                    = 76 , //
    x004D = 77 , x077                    = 77 , //
    x004E = 78 , x078                    = 78 , //
    x004F = 79 , x079                    = 79 , //

    x0050 = 80 , xVSC_OriReady           = 80 , //
    x0051 = 81 , xVSC_OriBusy            = 81 , // 
    x0052 = 82 , xVSC_OriRslt            = 82 , // 
    x0053 = 83 , xVSC_AlnReady           = 83 , // 
    x0054 = 84 , xVSC_AlnBusy            = 84 , // 
    x0055 = 85 , xVSC_AlnRslt            = 85 , //
    x0056 = 86 , xVSC_MrkReady           = 86 , // 
    x0057 = 87 , xVSC_MrkBusy            = 87 , // 
    x0058 = 88 , xVSC_MrkInspEnd         = 88 , //
    x0059 = 89 , x089                    = 89 , //
    x005A = 90 , x090                    = 90 , //
    x005B = 91 , xLSC_Spare1             = 91 , //         
    x005C = 92 , xLSC_DataReady          = 92 , //
    x005D = 93 , xLSC_Ready              = 93 , //
    x005E = 94 , xLSC_DataReadComp       = 94 , //
    x005F = 95 , xLSC_VisnDataRead       = 95 , // 

    x0060 = 96 , xLDR_PusherOvld         = 96 , //
    x0061 = 97 , xLDR_PusherFw           = 97 , //
    x0062 = 98 , xLDR_PusherBw           = 98 , //
    x0063 = 99 , xLDR_MgzDetect1         = 99 , //
    x0064 =100 , xLDR_MgzDetect2         =100 , //
    x0065 =101 , xLDR_ClmpUp             =101 , //
    x0066 =102 , xLDR_ClmpDn1            =102 , //
    x0067 =103 , x103                    =103 , //
    x0068 =104 , xLDR_MgzIn              =104 , //
    x0069 =105 , xLDR_MgzOutFull         =105 , //
    x006A =106 , x106                    =106 , //
    x006B =107 , x107                    =107 , //
    x006C =108 , x108                    =108 , //
    x006D =109 , x109                    =109 , //
    x006E =110 , xLDR_EmgSw              =110 , //
    x006F =111 , x111                    =111 , //

    x0070 =112 , xULD_MgzDetect1         =112 , //
    x0071 =113 , xULD_MgzDetect2         =113 , //
    x0072 =114 , xULD_ClmpUp             =114 , //
    x0073 =115 , xULD_ClmpDn1            =115 , //
    x0074 =116 , x116                    =116 , //
    x0075 =117 , xULD_MgzOutFull         =117 , //
    x0076 =118 , xULD_MgzIn              =118 , //
    x0077 =119 , x119                    =119 , //
    x0078 =120 , xULD_PowerOnAC          =120 , //
    x0079 =121 , xULD_PowerOnDC          =121 , //
    x007A =122 , xULD_EmgSw              =122 , //
    x007B =123 , x123                    =123 , //
    x007C =124 , x124                    =124 , //
    x007D =125 , x125                    =125 , //
    x007E =126 , x126                    =126 , //
    x007F =127 , x127                    =127 , //

    MAX_INPUT
};

//그냥 아이오31.  y+<파트 3자리>+_+<세부설명>
//복동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<해당행동 ex) Fw , Bw , Dn 등등 2자리>
//단동 실린더 아이오 y+<파트 3자리>+_+<세부설명>+<Fw시에해당행동2자리><Bw시에해당행동2자리>
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_LStartLp           =  0 , //
    y0001 =  1 , yETC_LStopLp            =  1 , //
    y0002 =  2 , yETC_LResetLp           =  2 , //
    y0003 =  3 , yETC_LAirLp             =  3 , //
    y0004 =  4 , yETC_LInitLp            =  4 , //
    y0005 =  5 , yETC_RStartLp           =  5 , //
    y0006 =  6 , yETC_RStopLp            =  6 , //
    y0007 =  7 , yETC_RResetLp           =  7 , //
    y0008 =  8 , yETC_RAirLp             =  8 , //
    y0009 =  9 , yETC_RInitLp            =  9 , //
    y000A = 10 , yETC_CStartLp           = 10 , //
    y000B = 11 , yETC_CStopLp            = 11 , //
    y000C = 12 , yETC_CResetLp           = 12 , //
    y000D = 13 , yETC_CVisnLp            = 13 , //
    y000E = 14 , yETC_CMarkLp            = 14 , //
    y000F = 15 , y015                    = 15 , // yPRB_FeedAC

    y0010 = 16 , yETC_TwRedLp            = 16 , // yPRB_AirBlower
    y0011 = 17 , yETC_TwYelLp            = 17 , // yRAL_LIdxUpDn
    y0012 = 18 , yETC_TwGrnLp            = 18 , // yPSB_PshFwBw
    y0013 = 19 , yETC_TwBzz              = 19 , // yPSB_RlrDnUp
    y0014 = 20 , yETC_MainSol            = 20 , // yRAL_LRalVac
    y0015 = 21 , y021                    = 21 , // yPSB_VsnVac
    y0016 = 22 , y022                    = 22 , // yPSB_RwkVac
    y0017 = 23 , y023                    = 23 , // yPRB_CmpDn
    y0018 = 24 , yPRB_FeedAc             = 24 , // yPRB_CmpUp
    y0019 = 25 , yRAL_FeedAc             = 25 , // yPSB_CmpDn
    y001A = 26 , yPSB_RjcBelt            = 26 , // yPSB_CmpUp
    y001B = 27 , yPRB_StpUp              = 27 , // y027/*yRWP_PkrVac*/
    y001C = 28 , yRAL_LStpUp             = 28 , // yRWB_BrdFw
    y001D = 29 , yRAL_VStpUp             = 29 , // yCPK_PkrVac
    y001E = 30 , yRAL_LStnUp             = 30 , // yCSP_AlnFw
    y001F = 31 , yRAL_LStnDn             = 31 , // yATP_RtrRl

    y0020 = 32 , yRAL_VStnUp             = 32 , // yATP_PkrVac
    y0021 = 33 , yRAL_VStnDn             = 33 , // yATP_PkrEjt
    y0022 = 34 , yRAL_LAlnFw             = 34 , // yPCP_PkrVacL
    y0023 = 35 , yRAL_VAlnFw             = 35 , // yPCP_PkrEjtL
    y0024 = 36 , yPSB_GrpGr              = 36 , // yPCP_PkrVacR      
    y0025 = 37 , yPSB_RalOp              = 37 , // yPCP_PkrEjtR
    y0026 = 38 , yPSB_ElvUp              = 38 , // yRWP_PkrVac
    y0027 = 39 , yPSB_IdxDn              = 39 , // yRWP_PkrEjt
    y0028 = 40 , yPSB_PshFw              = 40 , // yCUR_CmpDn
    y0029 = 41 , yPSB_PshDn              = 41 , // yCUR_CmpUp
    y002A = 42 , yPRB_AirBlow            = 42 , // yPCP_EpdFw
    y002B = 43 , y043                    = 43 , // y043
    y002C = 44 , y044                    = 44 , // yPSB_FeedAC
    y002D = 45 , y045                    = 45 , // y045
    y002E = 46 , y046                    = 46 , // y046
    y002F = 47 , y047                    = 47 , // y047

    y0030 = 48 , yVSC_OriStart           = 48 , //y048
    y0031 = 49 , yVSC_Spear1             = 49 , //y049
    y0032 = 50 , yVSC_AlnStart           = 50 , //y050
    y0033 = 51 , yVSC_Spear2             = 51 , //y051
    y0034 = 52 , yVSC_MrkStart           = 52 , //y052
    y0035 = 53 , yVSC_Reset              = 53 , //y053
    y0036 = 54 , yVSC_AutoTrain          = 54 , //y054
    y0037 = 55 , yVSC_JobChange          = 55 , //y055
    y0038 = 56 , yVSC_Spear3             = 56 , //y056
    y0039 = 57 , y057                    = 57 , //y057
    y003A = 58 , y058                    = 58 , //y058
    y003B = 59 , yLSC_Spear1             = 59 , //y059
    y003C = 60 , yLSC_Start              = 60 , //y060
    y003D = 61 , yLSC_MarkEnable         = 61 , //y061
    y003E = 62 , yLSC_DataRead           = 62 , //y062
    y003F = 63 , yLSC_Reset              = 63 , //y063

    y0040 = 64 , yLDR_MgzInAC            = 64 , //
    y0041 = 65 , yLDR_MgzOutAC           = 65 , //
    y0042 = 66 , yLDR_ClmpDn             = 66 , //
    y0043 = 67 , yLDR_ClmpUp             = 67 , //
    y0044 = 68 , yLDR_PusherFw           = 68 , //
    y0045 = 69 , yLDR_ZBreak             = 69 , //
    y0046 = 70 , y070                    = 70 , //
    y0047 = 71 , y071                    = 71 , //
    y0048 = 72 , y072                    = 72 , //
    y0049 = 73 , y073                    = 73 , //
    y004A = 74 , y074                    = 74 , //
    y004B = 75 , y075                    = 75 , //
    y004C = 76 , y076                    = 76 , //
    y004D = 77 , y077                    = 77 , //
    y004E = 78 , y078                    = 78 , //
    y004F = 79 , y079                    = 79 , //

    y0050 = 80 , yULD_MgzOutAC           = 80 , //
    y0051 = 81 , yULD_MgzInAC            = 81 , //
    y0052 = 82 , yULD_ClmpDn             = 82 , //
    y0053 = 83 , yULD_ClmpUp             = 83 , //
    y0054 = 84 , yULD_ZBreak             = 84 , //
    y0055 = 85 , y085                    = 85 , //
    y0056 = 86 , y086                    = 86 , //
    y0057 = 87 , y087                    = 87 , //
    y0058 = 88 , y088                    = 88 , //
    y0059 = 89 , y089                    = 89 , //
    y005A = 90 , y090                    = 90 , //
    y005B = 91 , y091                    = 91 , //
    y005C = 92 , y092                    = 92 , //
    y005D = 93 , y093                    = 93 , //
    y005E = 94 , y094                    = 94 , //
    y005F = 95 , y095                    = 95 , //

    MAX_OUTPUT
};

// 포지션 벨류   pv+<파트3자리>+_+<부가설명>+<포지션설명>
// 오프셑 오프셑 po+<파트3자리>+_+<부가설명>+<오프셑설명>
enum EN_PSTN_VALUE {
    pvLSR_YAlVsWait        = 0 ,
    pvLSR_YAlVsWork            ,
    pvLSR_YAlVsCal             ,
    MAX_PSTN_MOTR0             ,

    pvRAL_YCvsnWait        = 0 ,
    pvRAL_YCvsnWork            ,
    MAX_PSTN_MOTR1             ,

    pvVSN_XInspWait        = 0 ,
    pvVSN_XInspWorkStt         ,
    pvVSN_XInspMskChange       ,
    MAX_PSTN_MOTR2             ,

    pvVSN_YInspWait        = 0 ,
    pvVSN_YInspWorkStt         ,
    pvVSN_YInspMskChange       ,
    MAX_PSTN_MOTR3             ,

    pvPSB_XIndxWait        = 0 ,
    pvPSB_XIndxRjct            ,
    pvPSB_XIndxOut             ,
    MAX_PSTN_MOTR4             ,

    pvRAL_XLStpWait        = 0 ,
    pvRAL_XLStpWork            ,
    MAX_PSTN_MOTR5             ,

    pvRAL_XVStpWait        = 0 ,
    pvRAL_XVStpWork            ,
    MAX_PSTN_MOTR6             ,

    pvSpare                = 0 ,
    MAX_PSTN_MOTR7             ,

    pvLDR_YClmpWait        = 0 ,
    pvLDR_YClmpWork            ,
    pvLDR_YClmpPick            ,
    pvLDR_YClmpPlace           ,
    MAX_PSTN_MOTR8             ,

    pvLDR_ZClmpWait        = 0 ,
    pvLDR_ZClmpPickFwd         ,
    pvLDR_ZClmpClampOn         ,
    pvLDR_ZClmpPickBwd         ,
    pvLDR_ZClmpWorkStart       ,
    pvLDR_ZClmpPlaceFwd        ,
    pvLDR_ZClmpClampOff        ,
    pvLDR_ZClmpPlaceBwd        ,
    MAX_PSTN_MOTR9             ,

    pvULD_YClmpWait        = 0 ,
    pvULD_YClmpWork            ,
    pvULD_YClmpPick            ,
    pvULD_YClmpPlace           ,
    MAX_PSTN_MOTR10             ,

    pvULD_ZClmpWait        = 0 ,
    pvULD_ZClmpPickFwd         ,
    pvULD_ZClmpClampOn         ,
    pvULD_ZClmpPickBwd         ,
    pvULD_ZClmpWorkStart       ,
    pvULD_ZClmpPlaceFwd        ,
    pvULD_ZClmpClampOff        ,
    pvULD_ZClmpPlaceBwd        ,
    MAX_PSTN_MOTR11
};

// 포지션 벨류   pi+<파트3자리>+_+<부가설명>+<포지션설명>
// 포지션이 정해져 있지 않는 것은 (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) 뒤로 빼야
// PstnMan에서 CheckSafe및 버튼 클릭 시에 맞아 떨어진다. 주의....
enum EN_PSTN_ID {
    piLSR_YAlVsWait        = 0 ,
    piLSR_YAlVsWork            ,
    piLSR_YAlVsCal             ,
    MAX_PSTN_ID_MOTR0          ,

    piRAL_YCvsnWait        = 0 ,
    piRAL_YCvsnWork            ,
    MAX_PSTN_ID_MOTR1          ,

    piVSN_XInspWait        = 0 ,
    piVSN_XInspWorkStt         ,
    piVSN_XInspMskChange       ,
    piVSN_XInspTrgStt          ,
    piVSN_XInspTrgEnd          ,
    //piVSN_XInspWorkWait        ,

    MAX_PSTN_ID_MOTR2          ,

    piVSN_YInspWait        = 0 ,
    piVSN_YInspWorkStt         ,
    piVSN_YInspMskChange       ,
    piVSN_YInspTrg             ,//저장하지 않고 그냥 1칸씩 올려가면서 검사.
    //piVSN_YInspWorkWait        ,//검사 시작전에 대기위치.

    MAX_PSTN_ID_MOTR3          ,

    piPSB_XIndxWait        = 0 ,
    piPSB_XIndxRjct            ,
    piPSB_XIndxOut             ,
    MAX_PSTN_ID_MOTR4          ,

    piRAL_XLStpWait        = 0 ,
    piRAL_XLStpWork            ,
    MAX_PSTN_ID_MOTR5          ,

    piRAL_XVStpWait        = 0 ,
    piRAL_XVStpWork            ,
    MAX_PSTN_ID_MOTR6          ,

    piSpare                = 0 ,
    MAX_PSTN_ID_MOTR7          ,

    piLDR_YClmpWait        = 0 ,
    piLDR_YClmpWork            ,
    piLDR_YClmpPick            ,
    piLDR_YClmpPlace           ,
    MAX_PSTN_ID_MOTR8          ,

    piLDR_ZClmpWait        = 0 ,
    piLDR_ZClmpPickFwd         ,
    piLDR_ZClmpClampOn         ,
    piLDR_ZClmpPickBwd         ,
    piLDR_ZClmpWorkStart       ,
    piLDR_ZClmpPlaceFwd        ,
    piLDR_ZClmpClampOff        ,
    piLDR_ZClmpPlaceBwd        ,
    piLDR_ZClmpWork            ,
    MAX_PSTN_ID_MOTR9          ,

    piULD_YClmpWait        = 0 ,
    piULD_YClmpWork            ,
    piULD_YClmpPick            ,
    piULD_YClmpPlace           ,
    MAX_PSTN_ID_MOTR10          ,

    piULD_ZClmpWait        = 0 ,
    piULD_ZClmpPickFwd         ,
    piULD_ZClmpClampOn         ,
    piULD_ZClmpPickBwd         ,
    piULD_ZClmpWorkStart       ,
    piULD_ZClmpPlaceFwd        ,
    piULD_ZClmpClampOff        ,
    piULD_ZClmpPlaceBwd        ,
    piULD_ZClmpWork            ,
    MAX_PSTN_ID_MOTR11

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
    mcRailOutAll           =  2 ,

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

    mcPRB_Home             = 30 ,
    mcPRB_StprUpDn              ,
    mcPRB_FeedAC                ,
    mcPRB_AirBlow               ,
    mcPRB_CycleVisn             ,

    mcRAL_Home                  ,
    mcRAL_LStpUpDn              ,
    mcRAL_LStnUpDn              ,
    mcRAL_LAlnFwBw              ,
    mcRAL_VStpUpDn              ,
    mcRAL_VStnUpDn              ,
    mcRAL_VAlnFwBw              ,
    mcRAL_Bind                  ,
    mcRAL_UnBind                ,
    mcRAL_FeedAC                ,
    mcRAL_MoveNextPart          ,
    mcRAL_CycleCnvr             ,

    mcLSR_Home                  ,
    mcLSR_AVsnFwBw              ,
    mcLSR_CycleLsr              ,
    mcLSR_CycleAVsn             ,
    mcLSR_CycleWork             ,
    mcLSR_CycleCntrMrk          , //레이져-비젼 캘리브레이션 할때 레이져 센터 마킹하는 사이클

    mcVSN_Home                  ,
    mcVSN_XPitchP               ,
    mcVSN_XPitchN               ,
    mcVSN_YPitchP               ,
    mcVSN_YPitchN               ,
    mcVSN_CycleVisn             ,
    mcVSN_CycleSttPos           ,
    mcVSN_CycleTrain            ,
    mcVSN_MaskChange            ,

    mcPSB_Home                  ,
    mcPSB_GrprGrRl              ,
    mcPSB_RailOpCl              ,
    mcPSB_IndxDnUp              ,
    mcPSB_ElvtUpDn              ,
    mcPSB_PshrDnUp              ,
    mcPSB_PshrFwBw              ,
    mcPSB_CycleReject           ,

    mcULD_Home             =140 ,
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
/*040*/eiVSN_InspNG          , //비젼 공용.
/*041*/eiVSN_Busy            ,
/*042*/eiVSN_NotReady        ,
/*043*/eiVSN_TimeOut         ,
/*044*/eiVSN_FailCnt         , //마킹비젼 페일카운트
/*045*/eiVSN_TrainSel        , //트레인 선택해 주세요.
/*046*/eiPSB_IdxOvld         ,
/*047*/eiRAL_NotWorkPos      ,
/*048*/eiLSR_CommErr         ,
/*050*/eiLSR_TimeOut         ,
/*051*/eiLSR_DislikeFormat   ,
/*052*/eiLSR_LotFrstStrip    ,
/*053*/eiVSN_CommErr         ,
/*054*/ei054                 ,
/*055*/ei055                 ,
/*056*/ei056                 ,
/*057*/ei057                 ,
/*058*/ei058                 ,
/*059*/ei059                 ,
/*060*/MAX_ERR
};


#endif






























































