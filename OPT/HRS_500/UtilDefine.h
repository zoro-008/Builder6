#ifndef UTILDEFINEH
#define UTILDEFINEH


enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_GRADE_STAT {  //Bin 통들 지랄. CheckBox Item Index ==
    gsGradeA = 0  , //
    gsGradeB = 1  , //
    gsGradeC = 2  , //
    gsGradeD = 3  , //
    gsGradeE = 4  , //
    gsGradeF = 5  , //
    gsGradeG = 6  , //
    gsGradeH = 7  , //
    gsGradeI = 8  , //
    gsGradeJ = 9  , //
    gsFail   = 10 , // 로터 설정 범위내에 없음.
    gsCFail  = 11 , // 로터 안착 페일.
    gsBuffer = 12 , // 버퍼로 사용.

    MAX_GRADE_STAT
};

enum EN_ARAY_ID {
    riPRB       = 0  , //로더 바로 밑에
    riWRK       = 1  , //작업중
    riWKE       = 2  , //작업완료후에 인덱스 앞단에 끌려다니는 상태.
    riPSB       = 3  , //작업 완료 부터
    riPCK       = 4  , //픽커.
    riSTN       = 5  , //스테이션.
    riSRT       = 6  , //소터.
    MAX_ARAY
};

enum EN_MOTR_ID {
    miLDR_Z  = 0  , //
    miPCK_Y  = 1  , //
    miRAL_X  = 2  , //
    miSRT_X  = 3  , //
    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_Sptr   = 0   , // 로더 위쪽 트레이 스토퍼 실린더
    aiPCK_UpDn   = 1   , // 로더 아래쪽 클램프 실린더
    aiRAL_Indx   = 2   , // 레일 인덱스 업다운.
    aiSRT_Grip   = 3   , // 소터 그리퍼
    aiSRT_Flip   = 4   , // 소터 플리퍼
    aiULD_Stck   = 5   , // 소터 왼쪽 빈 0
    aiSPL_LBn0   = 6   , // 소터 왼쪽 빈 1
    aiSPL_LBn1   = 7   , // 소터 왼쪽 빈 2
    aiSPL_LBn2   = 8   , // 소터 왼쪽 빈 3
    aiSPL_LBn3   = 9   , // 소터 왼쪽 빈 4
    aiSPL_LBn4   = 10  , // 소터 왼쪽 빈 5
    aiSPL_LBn5   = 11  , // 소터 왼쪽 빈 6
    aiSPL_LBn6   = 12  , // 소터 왼쪽 빈 7
    aiSPL_LBn7   = 13  , //
    aiSPL_LBn8   = 14  ,
    aiSPL_LBn9   = 15  , // 소터 오른쪽 빈 0
    aiSPL_RBn0   = 16  , // 소터 오른쪽 빈 1
    aiSPL_RBn1   = 17  , // 소터 오른쪽 빈 2
    aiSPL_RBn2   = 18  , // 소터 오른쪽 빈 3
    aiSPL_RBn3   = 19  , // 소터 오른쪽 빈 4
    aiSPL_RBn4   = 20  , // 소터 오른쪽 빈 5
    aiSPL_RBn5   = 21  , // 소터 오른쪽 빈 6
    aiSPL_RBn6   = 22  , // 소터 오른쪽 빈 7
    aiSPL_RBn7   = 23  ,
    aiSPL_RBn8   = 24  ,
    aiSPL_RBn9   = 25  , // 언로더 스타커.
    MAX_ACTR
};

enum EN_ERR_ID {
/*001*/ eiETC_MainAir        =  1   ,
/*002*/ eiETC_ToStartTO             ,
/*003*/ eiETC_ToStopTO              ,
/*004*/ eiETC_Emergency             ,
/*005*/ eiETC_FDoor                 ,
/*006*/ eiETC_BDoor                 ,
/*007*/ eiETC_LotEnd                ,
/*008*/ eiETC_AllHomeTO             ,
/*009*/ eiETC_ManCycleTO            ,
/*010*/ ei010                       ,
/*011*/ eiLDR_CycleTO               ,
/*012*/ eiRAL_CycleTO               ,
/*013*/ eiPCK_CycleTO               ,
/*014*/ eiSRT_CycleTO               ,
/*015*/ eiULD_CycleTO               ,
/*016*/ eiSPL_CycleTO               ,
/*017*/ ei017                       ,
/*018*/ ei018                       ,
/*019*/ eiLDR_HomeTO                ,
/*020*/ eiRAL_HomeTO                ,
/*021*/ eiPCK_HomeTO                ,
/*022*/ eiSRT_HomeTO                ,
/*023*/ eiULD_HomeTO                ,
/*024*/ eiSPL_HomeTO                ,
/*025*/ ei025                       ,
/*026*/ ei026                       ,
/*027*/ eiLDR_ToStartTO             ,
/*028*/ eiRAL_ToStartTO             ,
/*029*/ eiPCK_ToStartTO             ,
/*030*/ eiSRT_ToStartTO             ,
/*031*/ eiULD_ToStartTO             ,
/*032*/ eiSPL_ToStartTO             ,
/*033*/ ei033                       ,
/*034*/ ei034                       ,
/*035*/ eiLDR_ToStopTO              ,
/*036*/ eiRAL_ToStopTO              ,
/*037*/ eiPCK_ToStopTO              ,
/*038*/ eiSRT_ToStopTO              ,
/*039*/ eiULD_ToStopTO              ,
/*040*/ eiSPL_ToStopTO              ,
/*041*/ ei041                       ,
/*042*/ ei042                       ,
/*043*/ eiLDR_ZHomeEnd              ,
/*044*/ eiLDR_ZNegLim               ,
/*045*/ eiLDR_ZPosLim               ,
/*046*/ eiLDR_ZAlarm                ,
/*047*/ ei047                       ,
/*048*/ eiRAL_XHomeEnd              ,
/*049*/ eiRAL_XNegLim               ,
/*050*/ eiRAL_XPosLim               ,
/*051*/ eiRAL_XAlarm                ,
/*052*/ ei052                       ,
/*053*/ eiPCK_YHomeEnd              ,
/*054*/ eiPCK_YNegLim               ,
/*055*/ eiPCK_YPosLim               ,
/*056*/ eiPCK_YAlarm                ,
/*057*/ ei057                       ,
/*058*/ eiSRT_XHomeEnd              ,
/*059*/ eiSRT_XNegLim               ,
/*060*/ eiSRT_XPosLim               ,
/*061*/ eiSRT_XAlarm                ,
/*062*/ ei062                       ,
/*063*/ ei063                       ,
/*064*/ ei064                       ,
/*065*/ ei065                       ,
/*066*/ ei066                       ,
/*067*/ ei067                       ,
/*068*/ eiLDR_SptrTO                ,
/*069*/ eiPCK_UpDnTO                ,
/*070*/ eiRAL_IndxTO                ,
/*071*/ eiSRT_GripTO                ,
/*072*/ eiSRT_FlipTO                ,
/*073*/ eiULD_StckTO                ,
/*074*/ eiSPL_LBn0TO                ,
/*075*/ eiSPL_LBn1TO                ,
/*076*/ eiSPL_LBn2TO                ,
/*077*/ eiSPL_LBn3TO                ,
/*078*/ eiSPL_LBn4TO                ,
/*079*/ eiSPL_LBn5TO                ,
/*080*/ eiSPL_LBn6TO                ,
/*081*/ eiSPL_LBn7TO                ,
/*082*/ eiSPL_LBn8TO                ,
/*083*/ eiSPL_LBn9TO                ,
/*084*/ eiSPL_RBn0TO                ,
/*085*/ eiSPL_RBn1TO                ,
/*086*/ eiSPL_RBn2TO                ,
/*087*/ eiSPL_RBn3TO                ,
/*088*/ eiSPL_RBn4TO                ,
/*089*/ eiSPL_RBn5TO                ,
/*090*/ eiSPL_RBn6TO                ,
/*091*/ eiSPL_RBn7TO                ,
/*092*/ eiSPL_RBn8TO                ,
/*093*/ eiSPL_RBn9TO                ,
/*094*/ EI094                       ,
/*095*/ ei095                       ,
/*096*/ ei096                       ,
/*097*/ ei097                       ,
/*098*/ eiPRB_Unknwn                ,
/*099*/ eiWRK_Unknwn                ,
/*100*/ eiWKE_Unknwn                ,
/*101*/ eiPSB_Unknwn                ,
/*102*/ eiSTN_Unknwn                ,
/*103*/ eiPCK_Unknwn                ,
/*104*/ ei104                       ,
/*105*/ eiPRB_Dispr                 ,
/*106*/ eiWRK_Dispr                 ,
/*107*/ eiWKE_Dispr                 ,
/*108*/ eiPSB_Dispr                 ,
/*109*/ eiSTN_Dispr                 ,
/*110*/ eiPCK_Dispr                 ,
/*111*/ ei111                       ,
/*112*/ eiLDR_Sply                  , //로더에서 트레이 내렸는데 안착 안됌.
/*113*/ eiSRT_Station               , //피커가 내려 놨는데 자제 없음 혹은 안착 안됌.
/*114*/ ei114                       , //
/*115*/ eiSPL_BinFull               , //빈 카운트 오버.
/*116*/ eiSPL_LimitFull             , //빈 리밋 센서 풀.
/*117*/ eiULD_TrayFeed              , //언로더 쪽에 트레이가 피딩이 안되고 걸림.
/*118*/ eiSPL_PKGFeed               , //페키지가 배출이 안됌.
/*119*/ eiSTR_PKGFeed               , //페키지가 안떨어짐.
/*120*/ eiPCK_Failed                , //피커 베큠 에러.
/*121*/ ei121                       ,
/*122*/ ei122                       ,
/*123*/ ei123                       ,
/*124*/ ei124                       ,
/*125*/ ei125                       ,
/*126*/ ei126                       ,
/*127*/ ei127                       ,
/*128*/ ei128                       ,
/*129*/ ei129                       ,

        MAX_ERR
};


enum EN_INPUT_ID {
    x0000 =  0 ,  xETC_LStartSw              =  0 , // xETC_L << Left
    x0001 =  1 ,  xETC_LStopSw               =  1 , //
    x0002 =  2 ,  xETC_LResetSw              =  2 , //
    x0003 =  3 ,  xETC_LAirSw                =  3 , //
    x0004 =  4 ,  xETC_LInitSw               =  4 , //
    x0005 =  5 ,  xETC_RStartSw              =  5 , // xETC_R << Right
    x0006 =  6 ,  xETC_RStopSw               =  6 , //
    x0007 =  7 ,  xETC_RResetSw              =  7 , //
    x0008 =  8 ,  xETC_RAirSw                =  8 , //
    x0009 =  9 ,  xETC_RInitSw               =  9 , //
    x000A = 10 ,  x010                       = 10 , //
    x000B = 11 ,  x011                       = 11 , //
    x000C = 12 ,  xETC_PowerOn               = 12 , //
    x000D = 13 ,  xETC_MainSol               = 13 , //
    x000E = 14 ,  xETC_EmgSw1                = 14 , //
    x000F = 15 ,  xETC_EmgSw2                = 15 , //

    x0010 = 16 ,  xLDR_TrayDetect            = 16 , // 감지는 Detect로 통일
    x0011 = 17 ,  xLDR_TrayStprFw            = 17 , //
    x0012 = 18 ,  xLDR_TrayStprBw            = 18 , //
    x0013 = 19 ,  xULD_TrayUp                = 19 , //
    x0014 = 20 ,  xULD_TrayDn                = 20 , //
    x0015 = 21 ,  xULD_TrayFull              = 21 , //
    x0016 = 22 ,  x022                       = 22 , //
    x0017 = 23 ,  x023                       = 23 , //
    x0018 = 24 ,  xRAL_Detect1               = 24 , // 로더 안착1
    x0019 = 25 ,  xRAL_Detect2               = 25 , // 로더 안착2
    x001A = 26 ,  xRAL_Detect3               = 26 , // 워크존
    x001B = 27 ,  xRAL_Detect4               = 27 , // 워크존 빠짐  *일단 사용안함.
    x001C = 28 ,  xRAL_Detect5               = 28 , // 언로더존 도착.
    x001D = 29 ,  xPCK_Vacuum                = 29 , //
    x001E = 30 ,  xPCK_PickUp                = 30 , //
    x001F = 31 ,  xPCK_PickDn                = 31 , //

    x0020 = 32 ,  xSRT_StnDetect             = 32 , // 스테이션 디텍트.
    x0021 = 33 ,  xRAL_IdxUp                 = 33 , //
    x0022 = 34 ,  xRAL_IdxDn                 = 34 , //
    x0023 = 35 ,  xSRT_RtrCmp                = 35 , //
    x0024 = 36 ,  xSRT_RtrUnCmp              = 36 , //
    x0025 = 37 ,  xSRT_TurnFw                = 37 , //
    x0026 = 38 ,  xSRT_TurnBw                = 38 , //
    x0027 = 39 ,  xSPL_FullDetect            = 39 , //
    x0028 = 40 ,  xSPL_LDetect0              = 40 , //
    x0029 = 41 ,  xSPL_LDetect1              = 41 , //
    x002A = 42 ,  xSPL_LDetect2              = 42 , //
    x002B = 43 ,  xSPL_LDetect3              = 43 , //
    x002C = 44 ,  xSPL_LDetect4              = 44 , //
    x002D = 45 ,  xSPL_LDetect5              = 45 , //
    x002E = 46 ,  xSPL_LDetect6              = 46 , //
    x002F = 47 ,  xSPL_LDetect7              = 47 , //

    x0030 = 48 ,  xSPL_LDetect8              = 48 , //
    x0031 = 49 ,  xSPL_LDetect9              = 49 , //
    x0032 = 50 ,  xSPL_RDetect0              = 50 , //
    x0033 = 51 ,  xSPL_RDetect1              = 51 , //
    x0034 = 52 ,  xSPL_RDetect2              = 52 , //
    x0035 = 53 ,  xSPL_RDetect3              = 53 , //
    x0036 = 54 ,  xSPL_RDetect4              = 54 , //
    x0037 = 55 ,  xSPL_RDetect5              = 55 , //
    x0038 = 56 ,  xSPL_RDetect6              = 56 , //
    x0039 = 57 ,  xSPL_RDetect7              = 57 , //
    x003A = 58 ,  xSPL_RDetect8              = 58 , //
    x003B = 59 ,  xSPL_RDetect9              = 59 , //
    x003C = 60 ,  xSRT_ChuckFail             = 60 , //
    x003D = 61 ,  x061                       = 61 , //
    x003E = 62 ,  x062                       = 62 , //
    x003F = 63 ,  x063                       = 63 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yETC_LStartLp              =  0 , // xETC_L << Left
    y0001 =  1 ,  yETC_LStopLp               =  1 , //
    y0002 =  2 ,  yETC_LResetLp              =  2 , //
    y0003 =  3 ,  yETC_LAirLp                =  3 , //
    y0004 =  4 ,  yETC_LInitLp               =  4 , //
    y0005 =  5 ,  yETC_RStartLp              =  5 , // xETC_R << Right
    y0006 =  6 ,  yETC_RStopLp               =  6 , //
    y0007 =  7 ,  yETC_RResetLp              =  7 , //
    y0008 =  8 ,  yETC_RAirLp                =  8 , //
    y0009 =  9 ,  yETC_RInitLp               =  9 , //
    y000A = 10 ,  y010                       = 10 , //
    y000B = 11 ,  y011                       = 11 , //
    y000C = 12 ,  yETC_TwRedLp               = 12 , //
    y000D = 13 ,  yETC_TwYelLp               = 13 , //
    y000E = 14 ,  yETC_TwGrnLp               = 14 , //
    y000F = 15 ,  yETC_TwBzz                 = 15 , //

    y0010 = 16 ,  yETC_MainSol               = 16 , //
    y0011 = 17 ,  yRAL_FeedingMt             = 17 , //
    y0012 = 18 ,  yPCK_PickFw                = 18 , //
    y0013 = 19 ,  yLDR_TrayStppr             = 19 , //
    y0014 = 20 ,  yULD_TrayUp                = 20 , //
    y0015 = 21 ,  yETC_VacuSol               = 21 , //
    y0016 = 22 ,  yRAL_Idx                   = 22 , //
    y0017 = 23 ,  yRAL_RtrCmp                = 23 , //
    y0018 = 24 ,  ySRT_TurnFw                = 24 , //
    y0019 = 25 ,  yPCK_Vacuum                = 25 , //
    y001A = 26 ,  yPCK_VacEjt                = 26 , //
    y001B = 27 ,  yPCK_PickBw                = 27 , //
    y001C = 28 ,  ySRT_TurnBw                = 28 , //
    y001D = 29 ,  ySRT_StnAir                = 29 , //
    y001E = 30 ,  y030                       = 30 , //
    y001F = 31 ,  y031                       = 31 , //

    y0020 = 32 ,  ySPL_LStppr0               = 32 , //
    y0021 = 33 ,  ySPL_LStppr1               = 33 , //
    y0022 = 34 ,  ySPL_LStppr2               = 34 , //
    y0023 = 35 ,  ySPL_LStppr3               = 35 , //
    y0024 = 36 ,  ySPL_LStppr4               = 36 , //
    y0025 = 37 ,  ySPL_LStppr5               = 37 , //
    y0026 = 38 ,  ySPL_LStppr6               = 38 , //
    y0027 = 39 ,  ySPL_LStppr7               = 39 , //
    y0028 = 40 ,  ySPL_LStppr8               = 40 , //
    y0029 = 41 ,  ySPL_LStppr9               = 41 , //
    y002A = 42 ,  ySPL_RStppr0               = 42 , //
    y002B = 43 ,  ySPL_RStppr1               = 43 , //
    y002C = 44 ,  ySPL_RStppr2               = 44 , //
    y002D = 45 ,  ySPL_RStppr3               = 45 , //
    y002E = 46 ,  ySPL_RStppr4               = 46 , //
    y002F = 47 ,  ySPL_RStppr5               = 47 , //

    y0030 = 48 ,  ySPL_RStppr6               = 48 , //
    y0031 = 49 ,  ySPL_RStppr7               = 49 , //
    y0032 = 50 ,  ySPL_RStppr8               = 50 , //
    y0033 = 51 ,  ySPL_RStppr9               = 51 , //
    y0034 = 52 ,  y052                       = 52 , //
    y0035 = 53 ,  y053                       = 53 , //
    y0036 = 54 ,  y054                       = 54 , //
    y0037 = 55 ,  y055                       = 55 , //
    y0038 = 56 ,  y056                       = 56 , //
    y0039 = 57 ,  y057                       = 57 , //
    y003A = 58 ,  y058                       = 58 , //
    y003B = 59 ,  y059                       = 59 , //
    y003C = 60 ,  y060                       = 60 , //
    y003D = 61 ,  y061                       = 61 , //
    y003E = 62 ,  y062                       = 62 , //
    y003F = 63 ,  y063                       = 63 , //
    MAX_OUTPUT
};


enum EN_PSTN_ID { //프로그램에서 가야할 아이디...
    piLDR_ZWait       = 0 ,
    piLDR_ZTop            ,
    piLDR_ZSparate        ,
    MAX_PSTN_ID_MOTR0     ,//miLDR_Z

    piPCK_YWait       = 0 ,
    piPCK_YPickStart      ,
    piPCK_YPlce           ,
    piPCK_YZig            ,
    piPCK_YPick           ,
    piPCK_YPutDown        ,
    MAX_PSTN_ID_MOTR1     ,//miPCK_Y

    piRAL_XWait       = 0 ,
    piRAL_XWorkIn         ,
    piRAL_XWorkStart      ,
    piRAL_XOut            , //마지막 트레이일경우만 사용.

    piRAL_XWork           ,
    MAX_PSTN_ID_MOTR2     ,//miRAL_X

    piSRT_XWait       = 0 ,
    piSRT_XPick           ,
    piSRT_XLBn9           ,
    piSRT_XLBn8           ,
    piSRT_XLBn7           ,
    piSRT_XLBn6           ,
    piSRT_XLBn5           ,
    piSRT_XLBn4           ,
    piSRT_XLBn3           ,
    piSRT_XLBn2           ,
    piSRT_XLBn1           ,
    piSRT_XLBn0           ,
    piSRT_XRBn0           ,
    piSRT_XRBn1           ,
    piSRT_XRBn2           ,
    piSRT_XRBn3           ,
    piSRT_XRBn4           ,
    piSRT_XRBn5           ,
    piSRT_XRBn6           ,
    piSRT_XRBn7           ,
    piSRT_XRBn8           ,
    piSRT_XRBn9           ,
    MAX_PSTN_ID_MOTR3     ,//miSRT_X

};

enum EN_PSTN_VALUE {
    pvLDR_ZWait       = 0 ,
    pvLDR_ZTop            ,
    pvLDR_ZSparate        ,
    MAX_PSTN_MOTR0        ,//miLDR_Z

    pvPCK_YWait       = 0 ,
    pvPCK_YPickStart      ,
    pvPCK_YPlce           ,
    pvPCK_YZig            ,
    MAX_PSTN_MOTR1        ,//miPCK_Y

    pvRAL_XWait       = 0 ,
    pvRAL_XWorkIn         ,    
    pvRAL_XWorkStart      ,
    pvRAL_XOut            ,
    MAX_PSTN_MOTR2        ,//miRAL_X

    pvSRT_XWait       = 0 ,
    pvSRT_XPick           ,
    pvSRT_XLBn9           ,
    poSRT_XPtch           ,
    MAX_PSTN_MOTR3        ,//miSRT_X
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_Home             = 10 , //
    mcLDR_Supply           = 11 , //

    mcRAL_Home             = 20 , //
    mcRAL_In               = 21 , //
    mcRAL_WorkStt          = 22 , //
    mcRAL_Work             = 23 , //
    mcRAL_Out              = 24 , //

    mcULD_Home             = 30 , //
    mcULD_In               = 31 , //
    mcULD_Stock            = 32 , //

    mcPCK_Home             = 40 , //
    mcPCK_Pick             = 41 , //
    mcPCK_Place            = 42 , //
    mcPCK_ReZero           = 43 , //
    mcPCK_Station          = 44 ,
    mcPCK_ReZeroCheck      = 45 ,

    mcSRT_Home             = 50 , //
    mcSRT_Pick             = 51 , //
    mcSRT_Sort             = 52 , //

//    mcSPL_ROne             = 60 , //
//    mcSPL_RAll             = 61 , //


    mcETC_MainAirOnOff     = 60 , //

    MAX_MANUAL_CYCLE
};
#endif



