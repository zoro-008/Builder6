#ifndef UTILDEFINEH
#define UTILDEFINEH

//bin pitch 33.8
//스테이션 및 소터 피치 33


enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};


enum EN_ARAY_ID {
    riCST       = 0  , //Cassette Zone
    riSTG       = 1  , //Stage  Zone
    riMAP       = 2  , //Stage Map File
    riMSK       = 3  , //Dot Mask.
    riTST       = 4  , //Test Map.
    riBRD       = 5  , //White Board.
    riSET       = 6  ,
    MAX_ARAY
};


enum EN_MOTR_ID {
    miSTG_ZCst  = 0  , // Cassette Z
    miSTG_XTrs  = 1  , // Stage Trasfer X
    miHED_XHed  = 2  , // Head Pen Marking X
    miHED_YHed  = 3  , // Head Pen Marking Y
    miSTG_TStg  = 4  , // Stage Seta T
    miHED_ZVsn  = 5  , // Head Vision Z
    miHED_TZom  = 6  , // Head Vision Zoom

    MAX_MOTR
};


enum EN_ACTR_ID {
    aiSTG_TrsCmpGR   = 0  , // Stage Transfer Wafer Clamp GripRelease
    aiHED_PenMrkDU   = 1  , // Head Pen Marking DnUp
    aiSTG_ChkTblUD   = 2  , // Stage Chuck Table UpDn
    aiSTG_WfrAlgnFB  = 3  , // Stage Cassette Wafer Align FwBw
    aiHED_PenCoverUD = 4  , // Head Pen Cover UpDn

    MAX_ACTR
};

enum EN_ERR_ID {
/*000*/ eiETC_MainPwr           =  0,
/*001*/ eiETC_MainAir               ,
/*002*/ eiETC_ToStartTO             ,
/*003*/ eiETC_ToStopTO              ,
/*004*/ eiETC_Emergency             ,
/*005*/ eiETC_FDoor                 ,
/*006*/ eiETC_BDoor                 ,
/*007*/ eiETC_RSideDoor             ,
/*008*/ eiETC_LSideDoor             ,
/*009*/ eiETC_LotEnd                ,
/*010*/ eiETC_AllHomeTO             ,
/*011*/ eiETC_ManCycleTO            ,
/*012*/ eiSTG_CycleTO               ,
/*013*/ eiHED_CycleTO               ,
/*014*/ ei014                       ,
/*015*/ ei015                       ,
/*016*/ ei016                       ,
/*017*/ eiSTG_HomeTO                ,
/*018*/ eiHED_HomeTO                ,
/*019*/ ei019                       ,
/*020*/ ei020                       ,
/*021*/ eiSTG_ToStartTO             ,
/*022*/ eiHED_ToStartTO             ,
/*023*/ ei023                       ,
/*024*/ ei024                       ,
/*025*/ eiSTG_ToStopTO              ,
/*026*/ eiHED_ToStopTO              ,
/*027*/ ei027                       ,
/*028*/ ei028                       ,
/*029*/ eiSTG_ZCstHomeEnd           ,
/*030*/ eiSTG_XTrsHomeEnd           ,
/*031*/ eiHED_XHedHomeEnd           ,
/*032*/ eiHED_YHedHomeEnd           ,
/*033*/ eiSTG_TStgHedHomeEnd        ,
/*034*/ eiHED_ZVsnHedHomeEnd        ,
/*035*/ eiHED_TZomHedHomeEnd        ,
/*036*/ eiSTG_ZCstNegLim            ,
/*037*/ eiSTG_XTrsNegLim            ,
/*038*/ eiHED_XHedNegLim            ,
/*039*/ eiHED_YHedNegLim            ,
/*040*/ eiSTG_TStgHedNegLim         ,
/*041*/ eiHED_ZVsnHedNegLim         ,
/*042*/ eiHED_TZomHedNegLim         ,
/*043*/ eiSTG_ZCstPosLim            ,
/*044*/ eiSTG_XTrsPosLim            ,
/*045*/ eiHED_XHedPosLim            ,
/*046*/ eiHED_YHedPosLim            ,
/*047*/ eiSTG_TStgPosLim            ,
/*048*/ eiHED_ZVsnPosLim            ,
/*049*/ eiHED_TZomPosLim            ,
/*050*/ eiSTG_ZCstAlarm             ,
/*051*/ eiSTG_XTrsAlarm             ,
/*052*/ eiHED_XHedAlarm             ,
/*053*/ eiHED_YHedAlarm             ,
/*054*/ eiSTG_TStgAlarm             ,
/*055*/ eiHED_ZVsnAlarm             ,
/*056*/ eiHED_TZomAlarm             ,
/*057*/ ei057                       ,
/*058*/ ei058                       ,
/*059*/ ei059                       ,
/*060*/ ei060                       ,
/*061*/ ei061                       ,
/*062*/ ei062                       ,
/*063*/ ei063                       ,
/*064*/ ei064                       ,
/*065*/ eiSTG_TrsCmpGRTO            ,
/*066*/ eiHED_PenMrkDUTO            ,
/*067*/ eiSTG_ChkTblUDTO            ,
/*068*/ eiHED_PenCover              ,
/*069*/ eiCST_Unknwn                ,
/*070*/ eiSTG_Unknwn                ,
/*071*/ ei071                       ,
/*072*/ ei072                       ,
/*073*/ ei073                       ,
/*074*/ ei074                       ,
/*075*/ ei075                       ,
/*076*/ ei076                       ,
/*077*/ ei077                       ,
/*078*/ ei078                       ,
/*079*/ ei079                       ,
/*080*/ ei080                       ,
/*081*/ eiCST_Dispr                 ,
/*082*/ eiSTG_Dispr                 ,
/*083*/ ei083                       ,
/*084*/ ei084                       ,
/*085*/ eiCST_Sply                  ,
/*086*/ eiSTG_PickFailed            ,
/*087*/ eiSTG_TransWaferFeedingFail ,
/*088*/ eiSTG_MapConverFail         ,
/*089*/ eiSTG_WaferNotDetected      ,
/*090*/ eiSTG_TrsOverloadDetect     ,
/*091*/ ei091                       ,
/*092*/ ei092                       ,
/*093*/ ei093                       ,
/*094*/ ei094                       ,
/*095*/ ei095                       ,
/*096*/ ei096                       ,
/*097*/ ei097                       ,
/*098*/ ei098                       ,
/*099*/ ei099                       ,
/*100*/ ei100                       ,
/*101*/ eiHED_OriVisnInspFail       , //오리엔테이션 비젼 검사 에러.                       ,
/*102*/ eiHED_OriVisnRlstOver       , //오리엔테이션 비젼 검사 결과 벋어남.                       ,
/*103*/ ei103                       ,
/*104*/ ei104                       ,
/*105*/ ei105                       ,
/*106*/ ei106                       ,
/*107*/ ei107                       ,
/*108*/ ei108                       ,
/*109*/ ei109                       ,
/*110*/ eiHED_PenCntOver           , //마킹 카운트 오버에러.
/*111*/ eiCST_Need                  ,
/*112*/ eiCST_NeedChange            ,
/*112*/ eiHED_VisnGrabFail          ,
/*113*/ eiHED_VisnInspFail          ,
/*114*/ eiHED_VisnRlstOver          ,
/*114*/ eiSTG_BarcodeFail           ,
/*115*/ eiSTG_MapLoadingFail        ,
/*116*/ eiHED_WhiteFull             , //화이트 보드 풀.
/*117*/ eiHED_VisnTO                ,
/*118*/ eiSTG_NoStnd                , //마스크에 기준칩이 없음.
/*119*/ eiSTG_NoOrig                , //마스크에 오리진이 없음.
/*120*/ eiHED_PenFail               , //펜찍힘 검사 에러.
/*121*/ eiHED_PenFailOper           , //펜찍힘 검사 에러..오퍼가 세팅해줘야 하는 에러.
/*122*/
/*123*/
/*124*/ //                            ,
/*125*/ //ei125                       ,
/*126*/ //ei126                       ,
/*127*/ //ei127                       ,
/*128*/ //ei128                       ,
/*129*/ //ei129                       ,

        MAX_ERR
};


enum EN_INPUT_ID {
    x0000 =  0 ,  xETC_StartSw               =  0 , // Start Button                   스타트 버튼
    x0001 =  1 ,  xETC_StopSw                =  1 , // Stop Button                    스탑 버튼
    x0002 =  2 ,  xETC_ResetSw               =  2 , // Reset Button                   리셑 버튼
    x0003 =  3 ,  xETC_MainPwrOn             =  3 , // Main Power On                       파워 온
    x0004 =  4 ,  xETC_MainAir               =  4 , // Main Air On                    메인 에어 온
    x0005 =  5 ,  xETC_EmgSwF                =  5 , // Emg Switch1                    이머전시 1
    x0006 =  6 ,  xETC_EmgSwL                =  6 , // Emg Switch2                    이머전시 2
    x0007 =  7 ,  xETC_EmgSwR                =  7 , // Emg Switch3                    이머전시 3
    x0008 =  8 ,  xSTG_CastDetect            =  8 , // Loader Cassette Detect         로더 카세트 유무 감지.
    x0009 =  9 ,  xSTG_TfWfDetect            =  9 , // Transfer Wafer Detect          트랜스퍼 웨이퍼 감지.
    x000A = 10 ,  xSTG_SgWfDetect            = 10 , // Stage Wafer Detect             스테이지 웨이퍼 감지
    x000B = 11 ,  xSTG_PrsSenser             = 11 , //
    x000C = 12 ,  xSTG_ChkTblUp              = 12 , // Chuck Table Up                 척 테이블 Up
    x000D = 13 ,  xSTG_ChkTblDn              = 13 , // Chuck Table Dn                 척 테이블 Dn
    x000E = 14 ,  xHED_PenMrkUp              = 14 , // Pen Marking Up                 펜 마킹 Up
    x000F = 15 ,  xHED_PenMrkDn              = 15 , // Pen Marking Dn                 펜 마킹 Dw

    x0010 = 16 ,  xETC_FrntDoorL             = 16 , // Front Door Detect1
    x0011 = 17 ,  xETC_FrntDoorR             = 17 , // Front Door Detect2
    x0012 = 18 ,  xETC_RightDoor             = 18 , // Right Side Door Detect
    x0013 = 19 ,  xETC_LeftDoor              = 19 , // Left Side Door Detect
    x0014 = 20 ,  xETC_BackDoorL             = 20 , // Back Door Detect1
    x0015 = 21 ,  xETC_BackDoorR             = 21 , // Back Door Detect2
    x0016 = 22 ,  xHED_PenCoverDn            = 22 , //
    x0017 = 23 ,  xHED_PenCoverUp            = 23 , //
    x0018 = 24 ,  xSTG_OvrLdDetect           = 24 , //
    x0019 = 25 ,  x025                       = 25 , //
    x001A = 26 ,  x026                       = 26 , //
    x001B = 27 ,  x027                       = 27 , //
    x001C = 28 ,  x028                       = 28 , //
    x001D = 29 ,  x029                       = 29 , //
    x001E = 30 ,  x030                       = 30 , //
    x001F = 31 ,  x031                       = 31 , //

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
    y0007 =  7 ,  yETC_TopLight              =  7 , //LED Door Lamp
    y0008 =  8 ,  yETC_MainAirSol            =  8 , //Main Air Sol
    y0009 =  9 ,  ySTG_ChkTblUp              =  9 , //Chuck Table Up
    y000A = 10 ,  ySTG_ChkTblDn              = 10 , //Chuck Table Dn
    y000B = 11 ,  ySTG_WaferCmp              = 11 , //Transfer Wafer Clamp            트랜스퍼 웨이퍼 클램프 실린더
    y000C = 12 ,  yHED_PenMrkUp              = 12 , //Pen Marking Up
    y000D = 13 ,  yHED_PenMrkDn              = 13 , //Pen Marking Dn
    y000E = 14 ,  ySTG_WfrVac                = 14 , //Stage Wafer Vaccum.
    y000F = 15 ,  ySTG_WfrEjtr               = 15 , //

    y0010 = 16 ,  yCST_ZBreakOff             = 16 , //
    y0011 = 17 ,  yETC_EjctrMain             = 17 , //
    y0012 = 18 ,  ySTG_WfrAlignFw            = 18 , //
    y0013 = 19 ,  ySTG_WfrAlignBw            = 19 , //
    y0014 = 20 ,  ySTG_Ionizer               = 20 , //
    y0015 = 21 ,  yHED_PenCoverUp            = 21 , //
    y0016 = 22 ,  yHED_PenCoverDn            = 22 , //
    y0017 = 23 ,  y023                       = 23 , //
    y0018 = 24 ,  y024                       = 24 , //
    y0019 = 25 ,  y025                       = 25 , //
    y001A = 26 ,  y026                       = 26 , //
    y001B = 27 ,  y027                       = 27 , //
    y001C = 28 ,  y028                       = 28 , //
    y001D = 29 ,  y029                       = 29 , //
    y001E = 30 ,  y030                       = 30 , //
    y001F = 31 ,  y031                       = 31 , //

    MAX_OUTPUT
};

//세이브 할 값들.
//pv는 Position Value.
//po는 Position Offset.
enum EN_PSTN_VALUE {
    pvSTG_ZCstWait       = 0 ,
    pvSTG_ZCstPickStt        ,
    poSTG_ZCstPlaceOfs       ,
    MAX_PSTN_MOTR0           ,

    pvSTG_XTrsWait       = 0 ,
    pvSTG_XTrsPick           ,
    pvSTG_XTrsPickWait       ,
    pvSTG_XTrsBarcode        ,
    pvSTG_XTrsPlace          ,
    MAX_PSTN_MOTR1           ,

    pvHED_XHedWait        = 0,
    pvHED_XHedVTAlgn         ,
    pvHED_XHedVOri           ,
    pvHED_XHedVCalPad        ,
    pvHED_XHedVBoardStt      ,
    pvHED_XHedPCover         ,
    pvHED_XHedHWCntrPos      ,
    poHED_XHedVisnPenOfs     ,
    poHED_XHedChipPenOfs     ,
    MAX_PSTN_MOTR2           ,

    pvHED_YHedWait       = 0 ,
    pvHED_YHedVTAlgn         ,
    pvHED_YHedVOri           ,
    pvHED_YHedVCalPad        ,
    pvHED_YHedVBoardStt      ,
    pvHED_YHedPCover         ,
    pvHED_YHedHWCntrPos      ,
    poHED_YHedVisnPenOfs     ,
    poHED_YHedChipPenOfs     ,
    MAX_PSTN_MOTR3           ,

    pvSTG_TStgWait       = 0 ,
    pvSTG_TStgTAlgn          ,
    MAX_PSTN_MOTR4           ,

    pvHED_ZVsnWait       = 0 ,
    pvHED_ZVsnWork           ,
    MAX_PSTN_MOTR5           ,

    pvHED_TZomWait       = 0 ,
    pvHED_TZomWork           ,
    MAX_PSTN_MOTR6           ,
};


enum EN_PSTN_ID { //프로그램에서 가야할 아이디...

    piSTG_ZCstWait       = 0 ,
    piSTG_ZCstPickStt        ,
    piSTG_ZCstPick           ,
    piSTG_ZCstPlace          ,
    MAX_PSTN_ID_MOTR0        ,

    piSTG_XTrsWait       = 0 ,
    piSTG_XTrsPick           ,
    piSTG_XTrsPickWait       ,
    piSTG_XTrsBarcode        ,
    piSTG_XTrsPlace          ,
    MAX_PSTN_ID_MOTR1        ,

    piHED_XHedWait       = 0 ,
    piHED_XHedVTAlgn         , //Theta 얼라인 센터에 있는 칩 위치.        
    piHED_XHedVOri           , //기준칩 검사 위치.                        
    piHED_XHedVCalPad        , //캐리브레이션 페드 위치.                  
    piHED_XHedVBoardStt      , //화이트 보드 스타트 위치.
    piHED_XHedPCover         , //펜 커버 위치
    piHED_XHedHWCntrPos      ,
    piHED_XHedVBoard         , //화이트 보드 워크 위치.                   
    piHED_XHedVChipAlign     , //칩 얼라인 위치.                          
    piHED_XHedPWrk           , //펜 찍는 위치.                            
    piHED_XHedVWrk           , //펜 찍은데 확인 하는 위치.
    piHED_XHedPBoardStt      , //화이트 보드에 팬 스타트 위치.
    piHED_XHedPBoard         , //화이트 보드에 팬 작업 위치.
    piHED_XHedVClick         , //비전 클릭 위치.
    piHED_XHedVMarkTrace     , //마크 역추적 위치.
    MAX_PSTN_ID_MOTR2        ,

    piHED_YHedWait       = 0 ,
    piHED_YHedVTAlgn         ,
    piHED_YHedVOri           ,
    piHED_YHedVCalPad        ,
    piHED_YHedVBoardStt      ,
    piHED_YHedPCover         ,
    piHED_YHedHWCntrPos      ,
    piHED_YHedVBoard         ,
    piHED_YHedVChipAlign     ,
    piHED_YHedPWrk           ,
    piHED_YHedVWrk           ,
    piHED_YHedPBoardStt      ,
    piHED_YHedPBoard         ,
    piHED_YHedVClick         , //비전 클릭 위치.
    piHED_YHedVMarkTrace     ,
    MAX_PSTN_ID_MOTR3        ,

    piSTG_TStgWait       = 0 ,
    piSTG_TStgTAlgn          ,
    piSTG_TStgWork           ,
    MAX_PSTN_ID_MOTR4        ,

    piHED_ZVsnWait       = 0 ,
    piHED_ZVsnWork           ,
    MAX_PSTN_ID_MOTR5        ,

    piHED_TZomWait       = 0 ,
    piHED_TZomWork           ,
    MAX_PSTN_ID_MOTR6

};



enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,
    mcETC_MainAirOnOff     =  2 ,

    mcSTG_CycleHome        = 10 ,
    mcSTG_CycleSupply           ,
    mcSTG_CycleExpend           ,
    mcSTG_CyclePlace            ,
    mcSTG_CycleUnExpend         ,
    mcSTG_VaccumOnOff           ,
    mcSTG_WfrAlgn               ,

    mcHED_CycleHome        = 20 ,
    mcHED_CycleTAlign           ,
    mcHED_CycleOrigin           ,
    mcHED_CycleChipAlign        ,
    mcHED_CycleMark             ,
    mcHED_CycleClickMove        ,
    mcHED_CycleCntrMove         ,
    mcHED_CycleCntrMoveT        ,


    mcHED_CycleCal         = 30 ,
    mcHED_CyclePenClearPs       ,
    mcHED_CyclePenChangPs       ,
    mcHED_CyclePenCheck         ,
    mcHED_CyclePenAlign         ,
    mcHED_CycleMarkTraceBw      ,
    mcHED_CycleMarkTraceFw      ,
    mcHED_CyclePenCoverPs       ,

    mcETC_LampOnOff             ,

    mcHED_YHedHWCntrPos         ,
    mcHED_HedVTAlgn             ,
    mcHED_HedVOri               ,
    mcHED_HedVCalPad            ,
    mcHED_HedVBoardStt          ,

    MAX_MANUAL_CYCLE
};
#endif



