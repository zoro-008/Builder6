#ifndef UTILDEFINEH
#define UTILDEFINEH

#include "Chip.h"

//bin pitch 33.8
//스테이션 및 소터 피치 33


enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

enum EN_GRADE_STAT {  //Bin 통들 지랄. CheckBox Item Index ==
    gsGradeA = csRslt0  , //  0
    gsGradeB = csRslt1  , //  1
    gsGradeC = csRslt2  , //  2
    gsGradeD = csRslt3  , //  3
    gsGradeE = csRslt4  , //  4
    gsGradeF = csRslt5  , //  5
    gsGradeG = csRslt6  , //  6
    gsGradeH = csRslt7  , //  7
    gsGradeI = csRslt8  , //  8
    gsGradeJ = csRslt9  , //  9
    MAX_OK_GRADE  ,

    gsFail   = csRsltA ,  //  10 로터 설정 범위내에 없음.
    gsCFail  = csRsltB ,  //  11 로터 안착 페일.
    gsNotUse = csRsltC ,  //  12 사용안함.

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
    miLDR_Z  = 0  , // Loader Z
    miPCK_Y  = 1  , // Picker Y
    miRAL_X  = 2  , // Rail X
    miSRT_X  = 3  , // Sort X

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiLDR_SptrFB   = 0  , // Loader Tray Stopper FwBw         로더 위쪽 트레이 스토퍼 실린더
    aiLDR_SplyFB   = 1  , // Loader Buffer Supplyer FwBw      로더 버퍼 써플라이.
    aiRAL_IndxUD   = 2  , // Rail Index UpDn                  레일 인덱스 업다운.
    aiPCK_PckrDU   = 3  , // Picker DnUp                      픽커 업다운.
    aiSTN_GuidFB   = 4  , // Station Guide FwBw               스테이션 가이드.
    aiSRT_Grip1FB  = 5  , // Sorter Gripper FwBw              소터 그리퍼
    aiSRT_Grip2FB  = 6  , // Sorter Gripper FwBw              소터 그리퍼
    aiSRT_Grip3FB  = 7  , // Sorter Gripper FwBw              소터 그리퍼
    aiSRT_FlipFB   = 8  , // Flipper FwBw                     소터 플리퍼
    aiULD_StckUD   = 9  , // Unloader Stocker UpDn            언로더 스타커.

    MAX_ACTR
};

enum EN_ERR_ID {
/*000*/ ei000                =  0   ,
/*001*/ eiETC_MainAir               ,
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
/*016*/ ei016                       ,
/*017*/ ei017                       ,
/*018*/ ei018                       ,
/*019*/ eiLDR_HomeTO                ,
/*020*/ eiRAL_HomeTO                ,
/*021*/ eiPCK_HomeTO                ,
/*022*/ eiSRT_HomeTO                ,
/*023*/ eiULD_HomeTO                ,
/*024*/ ei024                       ,
/*025*/ ei025                       ,
/*026*/ ei026                       ,
/*027*/ eiLDR_ToStartTO             ,
/*028*/ eiRAL_ToStartTO             ,
/*029*/ eiPCK_ToStartTO             ,
/*030*/ eiSRT_ToStartTO             ,
/*031*/ eiULD_ToStartTO             ,
/*032*/ ei032                       ,
/*033*/ ei033                       ,
/*034*/ ei034                       ,
/*035*/ eiLDR_ToStopTO              ,
/*036*/ eiRAL_ToStopTO              ,
/*037*/ eiPCK_ToStopTO              ,
/*038*/ eiSRT_ToStopTO              ,
/*039*/ eiULD_ToStopTO              ,
/*040*/ ei040                       ,
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
/*068*/ eiLDR_SptrFBTO              ,
/*069*/ eiLDR_SplyFBTO              ,
/*070*/ eiRAL_IndxUDTO              ,
/*071*/ eiPCK_PckrDUTO              ,
/*072*/ eiSTN_GuidFBTO              ,
/*073*/ eiSRT_Grip1FBTO             ,
/*074*/ eiSRT_Grip2FBTO             ,
/*075*/ eiSRT_Grip3FBTO             ,
/*076*/ eiSRT_FlipFBTO              ,
/*077*/ eiULD_StckUDTO              ,
/*078*/ ei078                       ,
/*079*/ ei079                       ,
/*080*/ ei080                       ,
/*081*/ ei081                       ,
/*082*/ ei082                       ,
/*083*/ ei083                       ,
/*084*/ ei084                       ,
/*085*/ ei085                       ,
/*086*/ ei086                       ,
/*087*/ ei087                       ,
/*088*/ ei088                       ,
/*089*/ ei089                       ,
/*090*/ ei090                       ,
/*091*/ ei091                       ,
/*092*/ ei092                       ,
/*093*/ ei093                       ,
/*094*/ ei094                       ,
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
/*112*/ eiLDR_Sply                  ,
/*113*/ ei113                       ,
/*114*/ ei114                       ,
/*115*/ ei115                       ,
/*116*/ eiSRT_DropCheck             ,
/*117*/ eiULD_TrayFeed              ,
/*118*/ eiSRT_DropFail              ,
/*119*/ ei119                       ,
/*120*/ eiPCK_Failed                ,
/*121*/ eiPRB_NotReady              ,
/*122*/ eiULD_TrayFull              ,
/*123*/ eiSRT_NoBin                 ,
/*124*/ eiSRT_PickFail              ,
/*125*/ ei125                       ,
/*126*/ ei126                       ,
/*127*/ ei127                       ,
/*128*/ ei128                       ,
/*129*/ ei129                       ,
                                                                             
        MAX_ERR
};


enum EN_INPUT_ID {
    x0000 =  0 ,  xETC_StartSw               =  0 , // Start Button                   스타트 버튼
    x0001 =  1 ,  xETC_StopSw                =  1 , // Stop Button                    스탑 버튼
    x0002 =  2 ,  xETC_ResetSw               =  2 , // Reset Button                   리셑 버튼
    x0003 =  3 ,  xETC_PowerOn               =  3 , // Power On                       파워 온
    x0004 =  4 ,  xETC_MainAir               =  4 , // Main Air On                    메인 에어 온
    x0005 =  5 ,  xETC_EmgSw1                =  5 , // Emg Switch1                    이머전시 1
    x0006 =  6 ,  xETC_EmgSw2                =  6 , // Emg Switch2                    이머전시 2
    x0007 =  7 ,
    x0008 =  8 ,  xLDR_BuffDetect            =  8 , // Buffer Tray Detect             버퍼 트레이 감지.
    x0009 =  9 ,  xLDR_TrayDetect            =  9 , // Loader Tray Detect             로더 트레이 감지.
    x000A = 10 ,  xLDR_TraySplyFw            = 10 , // Buffer Tray Supplyer Fw        버퍼 공급 Fw
    x000B = 11 ,  xLDR_TraySplyBw            = 11 , // Buffer Tray Supplyer Bw        버퍼 공급 Bw
    x000C = 12 ,  xLDR_TrayStprFw            = 12 , // Loader Tray Supply Stopper Fw  로더 트레이 공급 스톱퍼
    x000D = 13 ,  xLDR_TrayStprBw            = 13 , // Loader Tray Supply Stopper Bw
    x000E = 14 ,  xRAL_IdxUp                 = 14 , // Rail Feeding Index Up          레일 드레그 인덱스 업
    x000F = 15 ,  xRAL_IdxDn                 = 15 , // Rail Feeding Index Dn          레일 드레그 인덱스 다운
    x0010 = 16 ,  xPCK_PickUp                = 16 , // Picker Up                      픽커 픽업 업
    x0011 = 17 ,  xPCK_PickDn                = 17 , // Picker Dn
    x0012 = 18 ,  xPCK_Vacuum1               = 18 , // Picker Vacuum1                 픽커 배큠
    x0013 = 19 ,  xPCK_Vacuum2               = 19 , // Picker Vacuum2
    x0014 = 20 ,  xPCK_Vacuum3               = 20 , // Picker Vacuum3
    x0015 = 21 ,  xSRT_StnDetect1            = 21 , // Sorter Station Detect1         스테이션 디텍트.
    x0016 = 22 ,  xSRT_StnDetect2            = 22 , // Sorter Station Detect2         스테이션 디텍트.
    x0017 = 23 ,  xSRT_StnDetect3            = 23 , // Sorter Station Detect3         스테이션 디텍트.
    x0018 = 24 ,  xSRT_FliprFw               = 24 , // Sorter Flipper Fw              소터 터닝.
    x0019 = 25 ,  xSRT_FliprBw               = 25 , // Sorter Flipper Bw
    x001A = 26 ,  xRAL_Detect1               = 26 , // Rail Detect1                   레일존 도착1.
    x001B = 27 ,  xRAL_Detect2               = 27 , // Rail Detect2                   레일존 도착2.
    x001C = 28 ,  xRAL_Detect3               = 28 , // Rail Detect3                   레일존 도착3.
    x001D = 29 ,  xRAL_Detect4               = 29 , // Rail Detect4                   레일존 도착4.
    x001E = 30 ,  xRAL_Detect5               = 30 , // Rail Detect5                   레일존 도착5.
    x001F = 31 ,
    x0020 = 32 ,  xSRT_StnGuideFw            = 32 , // Sorter Station Guide Fw        스테이션에 가이드.
    x0021 = 33 ,  xSRT_StnGuideBw            = 33 , // Sorter Station Guide Bw
    x0022 = 34 ,  xSRT_DropDetect            = 34 , // Sorter Drop Detect             SPL_FullDetect            = 39 , //
    x0023 = 35 ,  xSRT_ChuckFail1            = 35 , // Sorter Rotor Chucker Fail1     SRT_ChuckFail1            = 60 , //
    x0024 = 36 ,  xSRT_ChuckFail2            = 36 , // Sorter Rotor Chucker Fail2     SRT_ChuckFail2            = 60 , //
    x0025 = 37 ,  xSRT_ChuckFail3            = 37 , // Sorter Rotor Chucker Fail3     SRT_ChuckFail3            = 60 , //
    x0026 = 38 ,  xULD_TrayUp                = 38 , // Unloader Tray Up               x010
    x0027 = 39 ,  xULD_TrayDn                = 39 , // Unloader Tray Dn               x011
    x0028 = 40 ,  xULD_TrayFull              = 40 , // Unloader Tray Full
    x0029 = 41 ,
    x002A = 42 ,  xPRB_Ready                 = 42 , // Height Probe Ready             프로브 레디.
    x002B = 43 ,
    x002C = 44 ,
    x002D = 45 ,
    x002E = 46 ,
    x002F = 47 ,
    x0030 = 48 ,
    x0031 = 49 ,
    x0032 = 50 ,
    x0033 = 51 ,
    x0034 = 52 ,
    x0035 = 53 ,
    x0036 = 54 ,
    x0037 = 55 ,
    x0038 = 56 ,
    x0039 = 57 ,
    x003A = 58 ,
    x003B = 59 ,
    x003C = 60 ,
    x003D = 61 ,
    x003E = 62 ,
    x003F = 63 ,

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yETC_StartLp               =  0 , //Start Button Lamp
    y0001 =  1 ,  yETC_StopLp                =  1 , //Stop Button Lamp
    y0002 =  2 ,  yETC_ResetLp               =  2 , //Reset Button Lamp
    y0003 =  3 ,  yETC_TwRedLp               =  3 , //Tower Lamp Red
    y0004 =  4 ,  yETC_TwYelLp               =  4 , //Tower Lamp Yellow
    y0005 =  5 ,  yETC_TwGrnLp               =  5 , //Tower Lamp Green
    y0006 =  6 ,  yETC_TwBzz                 =  6 , //Tower Lamp Buzzer
    y0007 =  7 ,
    y0008 =  8 ,  yRAL_FeedingMt             =  8 , //Rail Feeding Motor              레일 피딩 모터
    y0009 =  9 ,  yLDR_SplyFw                =  9 , //Loader Buffer Supplyer Fw       로더 버퍼 공급.
    y000A = 10 ,  yLDR_SplyBw                = 10 , //Loader Buffer Supplyer Bw       로더 버퍼 공급.
    y000B = 11 ,  yLDR_TrayStpprFw           = 11 , //Loader Tray Stopper Fw          로더 트레이 공급 스토퍼
    y000C = 12 ,  yPCK_PickUp                = 12 , //Picker Up                       픽커 픽 FWD
    y000D = 13 ,  yPCK_PickDn                = 13 , //Picker Dn                       픽커 픽 Bwd
    y000E = 14 ,  yRAL_IdxUpDn               = 14 , //Rail Feeding Index UpDn         레일 인덱스 업다운.
    y000F = 15 ,  ySRT_RtrCmp1FwBw           = 15 , //Sorter Flipper Clamp1 FwBw      레일 로터리 클램프1 언클램프.
    y0010 = 16 ,  ySRT_StnGuideFwBw          = 16 , //Sorter Station Guide FwBw       소터 스테이션 가이드 FW Bw
    y0011 = 17 ,  ySRT_FliprFwBw             = 17 , //Sorter Flipper FwBw             소터 터닝 FwBw
    y0012 = 18 ,  yULD_TrayUpDn              = 18 , //Unloader Tray UpDn              언로더 트레이 Up / Dn
    y0013 = 19 ,  yPCK_Vacuum1               = 19 , //Picker Vaccum1
    y0014 = 20 ,  yPCK_VacEjt1               = 20 , //Pciker Eject1
    y0015 = 21 ,  yPCK_Vacuum2               = 21 , //Picker Vaccum2
    y0016 = 22 ,  yPCK_VacEjt2               = 22 , //Pciker Eject2
    y0017 = 23 ,
    y0018 = 24 ,  yPCK_Vacuum3               = 24 , //Picker Vaccum3
    y0019 = 25 ,  yPCK_VacEjt3               = 25 , //Pciker Eject3
    y001A = 26 ,  ySRT_StnAir                = 26 , //Sorter Station Air Blower       스테이션에 에어 불어주는 것.
    y001B = 27 ,  ySRT_RtrCmp2FwBw           = 27 , //Sorter Flipper Clamp2 FwBw      레일 로터리 클램프2 언클램프.
    y001C = 28 ,  ySRT_RtrCmp3FwBw           = 28 , //Sorter Flipper Clamp3 FwBw      레일 로터리 클램프3 언클램프.
    y001D = 29 ,  yPRB_Rezero                = 29 , //Height Probe Rezero             프로브 리제로.
    y001E = 30 ,  yPRB_Start                 = 30 , //Height Probe Start              프로브 측정시작.
    y001F = 31 ,

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
    piPCK_YPutDown        ,//페일자제 다시 트레이에 넣는 옵션이면 여기다 놓음.
    MAX_PSTN_ID_MOTR1     ,//miPCK_Y

    piRAL_XWait       = 0 ,
    piRAL_XGet            ,
    piRAL_XWorkIn         ,
    piRAL_XWorkStart      ,
    piRAL_XOut            , //마지막 트레이일경우만 사용.
    piRAL_XWork           ,
    piRAL_XPutDown        ,
    MAX_PSTN_ID_MOTR2     ,//miRAL_X

    piSRT_XWait       = 0 ,
    piSRT_XPick           ,
    piSRT_XBin0           ,
    piSRT_XBin1           ,
    piSRT_XBin2           ,
    piSRT_XBin3           ,
    piSRT_XBin4           ,
    piSRT_XBin5           ,
    piSRT_XBin6           ,
    piSRT_XBin7           ,
    piSRT_XBin8           ,
    piSRT_XBin9           ,
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
    pvRAL_XGet            ,
    pvRAL_XWorkIn         ,    
    pvRAL_XWorkStart      ,
    pvRAL_XOut            ,
    MAX_PSTN_MOTR2        ,//miRAL_X

    pvSRT_XWait       = 0 ,
    pvSRT_XPick           ,
    pvSRT_XBin0           ,
    poSRT_XBinPtch        ,//빈통 피치
    poSRT_XSrtPtch        ,//소터 피치
    MAX_PSTN_MOTR3        ,//miSRT_X
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_Home             = 10 , 
    mcLDR_Supply           = 11 , 

    mcRAL_Home             = 20 , 
    mcRAL_In               = 21 , 
    mcRAL_WorkStt          = 22 , 
    mcRAL_Work             = 23 , 
    mcRAL_Out              = 24 , 

    mcULD_Home             = 30 , 
    mcULD_In               = 31 , 
    mcULD_Stock            = 32 , 

    mcPCK_Home             = 40 , 
    mcPCK_Pick             = 41 , 
    mcPCK_Place            = 42 , 
    mcPCK_ReZero           = 43 , 
    mcPCK_Station          = 44 ,
    mcPCK_ReZeroCheck      = 45 ,

    mcSRT_Home             = 50 , 
    mcSRT_Pick             = 51 , 
    mcSRT_Sort             = 52 , 

    mcETC_MainAirOnOff     = 60 , 

    MAX_MANUAL_CYCLE
};
#endif



