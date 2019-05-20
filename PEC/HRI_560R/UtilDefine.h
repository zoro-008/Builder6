#ifndef UTILDEFINEH
#define UTILDEFINEH

#define MAX_VISN 4

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};
enum EN_ARAY_ID {
    riLDR       =0  ,
    riPRB       =1  ,

    riWR1       =2  ,
    riVS1       =3  ,
    riWR2       =4  ,
    riVS2       =5  ,
    riWR3       =6  ,
    riVS3       =7  ,

    riPSB       =8  ,
    riLOT       =9  ,  //Lot End 용 버퍼.
    riVS4       =10 ,

    riULD       =11 , //언로더 레일.
    riTRY       =12 , //트레이
    riUPK       =13 , //트레이 한포켓.

    riWB1       =14 , //메뉴얼 동작 때문에 필요.
    riWB2       =15 ,
    riWB3       =16 ,

    riVB4       =17 ,

    riSPC       =18 ,  //SPC 용 버퍼.

    riLT1       =19 , //TODO :: dd 추가 랏어레이 한개로는 커버가 안되보임.
    riLT2       =20 ,

    riMAP       =21 , //TODO :: dd 추가 포스트 버퍼 SPC 에서 보여주기용.

    MAX_ARAY
};

enum EN_MOTR_ID {
    miLDR_XPsh  =  0 ,  // 0 Loader X Pusher  바닥에서 올려주는놈.
    miLDR_YFed       ,  // 1 Loader Y Mgz Feeding
    miLDR_XPck       ,  // 2 Loader X Picker
    miLDR_ZPck       ,  // 3 Loader Z Picker
    miWK1_YFlp       ,  // 4 Work 1 Part Flipper
    miWRK_XVsn       ,  // 5 Work Zone Vision X
    miWRK_YVsn       ,  // 6 Work Zone Vision Y
    miPSB_XMrk       ,  // 7 PostBuffer Marking X
    miPSB_YMrk       ,  // 8 PostBuffer Marking Y
    miWK1_XIns       ,  // 9 Work 1 Park Index
    miULD_ZPck       ,  // 10 UnLoader Z Pick Up
    miULD_YPck       ,  // 11 UnLoader Y Pick Up
    miULD_ZTrI       ,  // 12 UnLoader Z In  Tray
    miULD_ZTrO       ,  // 13 UnLoader Z Out Tray
    miULD_XStp       ,  // 14 UnLoader X Stopper
    miULD_ZPpr       ,  // 15 UnLoader Z Ganzi Paper

    MAX_MOTR
};



enum EN_ACTR_ID {
    aiLDR_Pusher        =  0 ,  // Loader X Pusher 바닥에 있는놈.
    aiLDR_Rotator       =  1 ,  // Loader Rotator
    aiLDR_Clamp         =  2 ,  // Loader Picker Clamp
    aiLDR_InMgz         =  3 ,  // Loader Magezin In Pusher
    aiLDR_OutMgz        =  4 ,  // Loader Magezin Out Pusher
    aiPRB_Stopper       =  5 ,  //
    aiWK1_Stopper       =  6 ,  //
    aiWK1_Align         =  7 ,  //
    aiWK1_FlpCmp        =  8 ,  //
    aiWK1_Ins           =  9 ,  //
    aiWK2_Stopper       = 10 ,  //
    aiWK2_Align         = 11 ,  //
    aiWK3_Stopper       = 12 ,  //
    aiWK3_Align         = 13 ,  //
    aiPSB_Stopper       = 14 ,  //
    aiPSB_Align         = 15 ,  //
    aiPSB_Marking       = 16 ,  //
    aiPSB_RjctOpen      = 17 ,  //
    aiPSB_RjctUpDn      = 18 ,  //
    aiULD_TrayClamp     = 19 ,  // UnLoader Clamp
    aiULD_TrayClampPick = 20 ,  // UnLoader Pick Up
    aiULD_TrayClampMove = 21 ,  // UnLoader Moving
    aiULD_TrayColSel    = 22 ,  // xULD_TrayColSelRt
    aiULD_TrayOpen      = 23 ,  // xULD_TrayOpenFtFw
    aiULD_Hold          = 24 ,  // UnLoader Hold
    aiULD_PaperClamp    = 25 ,  // UnLoader Paper Clamp Cylinder
    aiULD_PprRotator    = 26 ,  // UnLoader Paper Detece Rotator Cylinder

    MAX_ACTR

};

enum EN_ERR_ID {
    eiETC_MainAir          =  0 ,
    eiETC_ToStartTO             ,
    eiETC_ToStopTO              ,
    eiETC_Emergency             ,
    eiETC_FDoor                 ,
    eiETC_BDoor                 ,
    eiETC_LotEnd                ,
    eiETC_AllHomeTO             ,
    eiETC_ManCycleTO            ,
    eiLDR_CycleTO               ,
    eiRAL_CycleTO               ,
    eiHED_CycleTO               ,
    eiPSB_CycleTO               ,
    eiULD_CycleTO               ,
    eiULM_CycleTO               ,
    eiLDR_HomeTO                ,
    eiRAL_HomeTO                ,
    eiHED_HomeTO                ,
    eiPSB_HomeTO                ,
    eiULD_HomeTO                ,
    eiULM_HomeTO                ,
    eiLDR_ToStartTO             ,
    eiRAL_ToStartTO             ,
    eiHED_ToStartTO             ,
    eiPSB_ToStartTO             ,
    eiULD_ToStartTO             ,
    eiULM_ToStartTO             ,
    eiLDR_ToStopTO              ,
    eiRAL_ToStopTO              ,
    eiHED_ToStopTO              ,
    eiPSB_ToStopTO              ,
    eiULD_ToStopTO              ,
    eiULM_ToStopTO              ,
    eiLDR_XPshHomeEnd           ,
    eiLDR_XPshNegLim            ,
    eiLDR_XPshPosLim            ,
    eiLDR_XPshAlarm             ,
    eiLDR_YFedHomeEnd           ,
    eiLDR_YFedNegLim            ,
    eiLDR_YFedPosLim            ,
    eiLDR_YFedAlarm             ,
    eiLDR_XPckHomeEnd           ,
    eiLDR_XPckNegLim            ,
    eiLDR_XPckPosLim            ,
    eiLDR_XPckAlarm             ,
    eiLDR_ZPckHomeEnd           ,
    eiLDR_ZPckNegLim            ,
    eiLDR_ZPckPosLim            ,
    eiLDR_ZPckAlarm             ,
    eiWK1_YFlpHomeEnd           ,
    eiWK1_YFlpNegLim            ,
    eiWK1_YFlpPosLim            ,
    eiWK1_YFlpAlarm             ,
    eiWK1_XInsHomeEnd           ,
    eiWK1_XInsNegLim            ,
    eiWK1_XInsPosLim            ,
    eiWK1_XInsAlarm             ,
    eiWRK_XVsnHomeEnd           ,
    eiWRK_XVsnNegLim            ,
    eiWRK_XVsnPosLim            ,
    eiWRK_XVsnAlarm             ,
    eiWRK_YVsnHomeEnd           ,
    eiWRK_YVsnNegLim            ,
    eiWRK_YVsnPosLim            ,
    eiWRK_YVsnAlarm             ,
    eiPSB_XMrkHomeEnd           ,
    eiPSB_XMrkNegLim            ,
    eiPSB_XMrkPosLim            ,
    eiPSB_XMrkAlarm             ,
    eiPSB_YMrkHomeEnd           ,
    eiPSB_YMrkNegLim            ,
    eiPSB_YMrkPosLim            ,
    eiPSB_YMrkAlarm             ,
    eiULD_ZPckHomeEnd           ,
    eiULD_ZPckNegLim            ,
    eiULD_ZPckPosLim            ,
    eiULD_ZPckAlarm             ,
    eiULD_YPckHomeEnd           ,
    eiULD_YPckNegLim            ,
    eiULD_YPckPosLim            ,
    eiULD_YPckAlarm             ,
    eiULD_ZTrIHomeEnd           ,
    eiULD_ZTrINegLim            ,
    eiULD_ZTrIPosLim            ,
    eiULD_ZTrIAlarm             ,
    eiULD_ZTrOHomeEnd           ,
    eiULD_ZTrONegLim            ,
    eiULD_ZTrOPosLim            ,
    eiULD_ZTrOAlarm             ,
    eiULD_XStpHomeEnd           ,
    eiULD_XStpNegLim            ,
    eiULD_XStpPosLim            ,
    eiULD_XStpAlarm             ,
    eiULD_ZPprHomeEnd           ,
    eiULD_ZPprNegLim            ,
    eiULD_ZPprPosLim            ,
    eiULD_ZPprAlarm             ,
    eiLDR_PusherTo              ,
    eiLDR_RotatorTo             ,
    eiLDR_ClampTo               ,
    eiLDR_InMgzTo               ,
    eiLDR_OutMgzTo              ,
    eiPRB_StopperTo             ,
    eiWK1_StopperTo             ,
    eiWK1_AlignTo               ,
    eiWK1_FlpCmpTo              ,
    eiWK1_InsTo                 ,
    eiWK2_StopperTo             ,
    eiWK2_AlignTo               ,
    eiWK3_StopperTo             ,
    eiWK3_AlignTo               ,
    eiPSB_StopperTo             ,
    eiPSB_AlignTo               ,
    eiPSB_MarkingTo             ,
    eiPSB_RjctOpenTo            ,
    eiPSB_RjctUpDnTo            ,
    eiULD_TrayClampTo           ,
    eiULD_TrayPickTo            ,
    eiULD_TrayMoveTo            ,
    eiULD_TrayColSelTo          ,
    eiULD_OpenTo                ,
    eiULD_HoldTo                ,
    eiULD_PaperClampTo          ,
    eiLDR_MgzDispr              ,
    eiLDR_MgzUnknown            ,
    eiPRB_PkgDispr              ,
    eiPRB_PkgUnknown            ,
    eiWK1_PkgDispr              ,
    eiWK1_PkgUnknown            ,
    eiWK2_PkgDispr              ,
    eiWK2_PkgUnknown            ,
    eiWK3_PkgDispr              ,
    eiWK3_PkgUnknown            ,
    eiPSB_PkgDispr              ,
    eiPSB_PkgUnknown            ,
    eiULD_PkgUnknown            ,
    eiULD_PkgDispr              ,
    eiULM_TryDispr              ,
    eiULM_PprDispr              ,
    eiULM_TryFull               ,
    eiWK1_VsnFailCnt            ,
    eiWK2_VsnFailCnt            ,
    eiWK3_VsnFailCnt            ,
    eiPSB_VsnFailCnt            ,
    eiPSB_MarkingFail           ,
    eiHED_Visn1NotReady         ,
    eiHED_Visn2NotReady         ,
    eiHED_Visn3NotReady         ,
    eiHED_Visn4NotReady         ,
    eiLDR_PshrOverload          ,
    eiLDR_MgzFull               ,
    eiLDR_NeedMgz               ,
    eiHED_Visn1Read             ,
    eiHED_Visn2Read             ,
    eiHED_Visn3Read             ,
    eiHED_Visn4Read             ,
    eiHED_VisnResetCnt          ,
    eiHED_VisnCommTO            ,
    eiWK1_VsnSFailCnt           ,
    eiWK2_VsnSFailCnt           ,
    eiWK3_VsnSFailCnt           ,
    eiPRB_OutStripExist         ,
    eiPSB_OutStripExist         ,
    eiWK1_IndexOverload         ,
    eiPRB_StripFeedingFail      ,
    eiREJ_NeedStrip             ,
    eiULD_StripFeedingFail      ,
    eiWK1_FlipperMoveFail       ,
    eiPSB_TotalFailCnt          ,
    eiPSB_SameFailCnt           ,
    eiULD_PkgVaccumMiss         ,
    eiULD_PprEmpty              ,
    eiLDR_InMgzDetected         ,
    eiLDR_OutMgzDetected        ,
    eiPSB_RejectFull            ,
    eiULD_HoldingOpen           ,
    eiULD_TrayInEmpty           ,
    eiULD_TrayOutFull           ,
    eiULD_PaperVaccumMiss       ,
    eiULD_PprRotator            ,
    eiULD_TrayPshDetect         ,
    eiLDR_MgzEmpty              ,
    MAX_ERR
};

enum EN_INPUT_ID {
    x0000 =  0 ,  xETC_LStartSw            =  0 , //
    x0001 =  1 ,  xETC_LStopSw             =  1 , //
    x0002 =  2 ,  xETC_LResetSw            =  2 , //
    x0003 =  3 ,  xETC_LMainAir            =  3 , //
    x0004 =  4 ,  xETC_LInitSw             =  4 , //
    x0005 =  5 ,  xETC_LVisnSw             =  5 , //
    x0006 =  6 ,  xETC_RStartSw            =  6 , //
    x0007 =  7 ,  xETC_RStopSw             =  7 , //
    x0008 =  8 ,  xETC_RResetSw            =  8 , //
    x0009 =  9 ,  xETC_RMainAir            =  9 , //
    x000A = 10 ,  xETC_RInitSw             = 10 , //
    x000B = 11 ,  xETC_RVisnSw             = 11 , //
    x000C = 12 ,  x012                     = 12 , //
    x000D = 13 ,  x013                     = 13 , //
    x000E = 14 ,  xETC_EmgSw1              = 14 , //
    x000F = 15 ,  xETC_EmgSw2              = 15 , //

    x0010 = 16 ,  x080                     = 16 , //
    x0011 = 17 ,  xETC_PowerOn             = 17 , //
    x0012 = 18 ,  xETC_MainSol             = 18 , //
    x0013 = 19 ,  xETC_FDoor               = 19 , //
    x0014 = 20 ,  xETC_BDoor               = 20 , //
    x0015 = 21 ,  x021                     = 21 , //
    x0016 = 22 ,  xWR1_TurnDetect          = 22 , // 플리퍼 회전 완료 감지 확인 센서.
    x0017 = 23 ,  x023                     = 23 , //
    x0018 = 24 ,  xPRB_PkgIn               = 24 , // 프리버퍼 맨 왼쪽 인입 센서.
    x0019 = 25 ,  xPRB_Pkg                 = 25 , // 프리버퍼 스토퍼에 있는 감지 센서.
    x001A = 26 ,  xPRB_PkgOut              = 26 , // 프리버퍼 스토퍼지나자 마자 있는 센서.
    x001B = 27 ,  xWR1_Pkg                 = 27 , // 플리퍼 스토퍼 자제 감지센서
    x001C = 28 ,  xWR2_Pkg                 = 28 , // 2nd 존의 스토퍼 자제 감지 센서.
    x001D = 29 ,  xWR3_Pkg                 = 29 , // 3rd 존의 스토퍼 자제 감지 센서.
    x001E = 30 ,  xPSB_Pkg                 = 30 , // 포스트버퍼 스토퍼 자제 감지 센서.
    x001F = 31 ,  xPSB_PkgOut              = 31 , // 레일 맨 끝에 있는 아웃 센서.

    x0020 = 32 ,  xPRB_StprUp              = 32 , //
    x0021 = 33 ,  xPRB_StprDn              = 33 , //
    x0022 = 34 ,  xWK1_AlignFw             = 34 , //
    x0023 = 35 ,  xWK1_ClampFw1            = 35 , // 클램프에 센서 4개.
    x0024 = 36 ,  xWK1_ClampFw2            = 36 , //
    x0025 = 37 ,  xWK1_AlignBw             = 37 , //
    x0026 = 38 ,  xWK1_StprUp              = 38 , //
    x0027 = 39 ,  xWK1_StprDn              = 39 , //
    x0028 = 40 ,  xWK2_StprUp              = 40 , //
    x0029 = 41 ,  xWK2_StprDn              = 41 , //
    x002A = 42 ,  xWK3_StprUp              = 42 , //
    x002B = 43 ,  xWK3_StprDn              = 43 , //
    x002C = 44 ,  xPSB_StprUp              = 44 , //
    x002D = 45 ,  xPSB_StprDn              = 45 , //
    x002E = 46 ,  xWK1_ClampFw3            = 46 , //
    x002F = 47 ,  xWK1_ClampFw4            = 47 , //

    x0030 = 48 ,  xWR2_Align               = 48 , //
    x0031 = 49 ,  xWR3_Align               = 49 , //
    x0032 = 50 ,  xPSB_RejectOpenFw        = 50 , //
    x0033 = 51 ,  xPSB_MarkingUp           = 51 , //
    x0034 = 52 ,  xPSB_MarkingDn           = 52 , //
    x0035 = 53 ,  xWK1_IndxUp              = 53 , //
    x0036 = 54 ,  xWK1_IndxDn              = 54 , //
    x0037 = 55 ,  xWK1_IndxOverload        = 55 , //
    x0038 = 56 ,  xPSB_RejectDetect        = 56 , // 리젝 배출후 앞쪽에 쎈서 있음.
    x0039 = 57 ,  xPSB_RejectLiftFw        = 57 , // 리젝 리시버 업다운.
    x003A = 58 ,  xPSB_RejectLiftBw        = 58 , //
    x003B = 59 ,  xPSB_Align               = 59 , // 레일에 달린 오픈 포워드 백워드.
    x003C = 60 ,  xPSB_RejectOpenBw        = 60 , //
    x003D = 61 ,  x061                     = 61 , //
    x003E = 62 ,  x062                     = 62 , //
    x003F = 63 ,  x063                     = 63 , //

    x0040 = 64 ,  x064                     = 64 , //
    x0041 = 65 ,  x065                     = 65 , //
    x0042 = 66 ,  xLDR_EmgSw               = 66 , //
    x0043 = 67 ,  x003                     = 67 , //
    x0044 = 68 ,  xETC_MarkingSw           = 68 , //
    x0045 = 69 ,  xETC_PassSw              = 69 , //
    x0046 = 70 ,  xETC_ReviewEndSw         = 70 , //
    x0047 = 71 ,  xLDR_PkgDetect           = 71 , //밑에 푸셔가 올라와보고 자제 있는지 없는지 확인 하는 센서. 푸셔에 달려 있다.
    x0048 = 72 ,  xLDR_MgzIn               = 72 , //메거진 진입 시켜도 되는 지 확인 센서.
    x0049 = 73 ,  xLDR_MgzInStop           = 73 , //메거진 진입 도착 확인 센서.
    x004A = 74 ,  xLDR_MgzWork             = 74 , //메거진 작업중 감지.
    x004B = 75 ,  xLDR_MgzOutStop          = 75 , //메거진 작업 끝나고 배출 시켜도 되는지 확인 센서
    x004C = 76 ,  xLDR_MgzOut              = 76 , //메거진 작업 종료후 배출 완료.
    x004D = 77 ,  xLDR_PusherUp            = 77 , //메거진 작업존에 밑에 있는 푸셔.
    x004E = 78 ,  xLDR_PusherDn            = 78 , //
    x004F = 79 ,  x079                     = 79 , //

    x0050 = 80 ,  xLDR_RotatorFw           = 80 , //자제 집어서 돌려주는 것 밑으로 향할때.
    x0051 = 81 ,  xLDR_RotatorBw           = 81 , //자제 집어서 돌려주는 것 레일로 향할때.
    x0052 = 82 ,  xLDR_ClampFw             = 82 , //로더 크램프
    x0053 = 83 ,  xLDR_ClampBw             = 83 , //
    x0054 = 84 ,  xLDR_MgzInPusherFw       = 84 , //메거진 인 푸셔.
    x0055 = 85 ,  xLDR_MgzInPusherBw       = 85 , //메거진 인 푸셔.
    x0056 = 86 ,  xLDR_MgzOutPusherFw      = 86 , //메거진 아웃.
    x0057 = 87 ,  xLDR_MgzOutPusherBw      = 87 , //
    x0058 = 88 ,  x088                     = 88 , //
    x0059 = 89 ,  xLDR_Door1               = 89 , //
    x005A = 90 ,  xLDR_Door2               = 90 , //
    x005B = 91 ,  xLDR_Door3               = 91 , //
    x005C = 92 ,  x092                     = 92 , //
    x005D = 93 ,  x093                     = 93 , //
    x005E = 94 ,  x094                     = 94 , //
    x005F = 95 ,  x095                     = 95 , //

    x0060 = 96 ,  xULD_PprClampFw          = 96 , //
    x0061 = 97 ,  xULD_PprRotatorFw        = 97 , //
    x0062 = 98 ,  xULD_EmgSw               = 98 , //
    x0063 = 99 ,  xULD_OpenSw              = 99 , // 누르면 작업 완료된 트레이 배출.
    x0064 =100 ,  xULD_TrayClampFw         =100 , // 트레이 작업이 완료되면 찝어서 옆으로 놓는 클램프.
    x0065 =101 ,  xULD_TrayClampBw         =101 , //
    x0066 =102 ,  xULD_TrayClampMoveLt     =102 , // 트레이 클램프 인 아웃 왼쪽 ,
    x0067 =103 ,  xULD_TrayClampMoveRt     =103 , //
    x0068 =104 ,  xULD_TrayClampPickFw     =104 , // 트레이 클램프 다운
    x0069 =105 ,  xULD_TrayClampPickBw     =105 , //                업
    x006A =106 ,  xULD_TrayColSelRt        =106 , // 트레이 컬럼 셀렉트  오른쪽.
    x006B =107 ,  xULD_TrayColSelLt        =107 , // 트레이 컬럼 셀렉트. 왼쪽
    x006C =108 ,  xULD_PkgVaccum           =108 , // 패키지 배큠
    x006D =109 ,  xULD_PprVaccum           =109 , // 페이퍼 배큠
    x006E =110 ,  xULD_PaperDetect         =110 , // 페이퍼 디텍트.
    x006F =111 ,  xULD_PprRotatorBw        =111 , // 페이퍼 디텍트.
    x0070 =112 ,  xULD_PkgIn               =112 , // 언로더 인입부 감지 센서.
    x0071 =113 ,  xULD_Pkg1                =113 , // 언로더 스트립 작업부 감지 센서1
    x0072 =114 ,  xULD_Pkg2                =114 , // 2
    x0073 =115 ,  xULD_Pkg3                =115 , // 3
    x0074 =116 ,  xULD_TrayHold1Fw         =116 , // 언로더 배출하지 못하게 홀드 해주는 실린더1
    x0075 =117 ,  xULD_TrayHold2Fw         =117 , // 언로더 배출하지 못하게 홀드 해주는 실린더2
    x0076 =118 ,  xULD_TrayOpenFtFw        =118 , // 앞쪽언로더 트레이 배출
    x0077 =119 ,  xULD_TrayOpenFtBw        =119 , //
    x0078 =120 ,  xULD_TrayPushDetect      =120 , //트레이 푸쉬 디텍터
    x0079 =121 ,  xULD_TrayOpenRrFw        =121 , // 뒷쪽 언로더 트레이 배출
    x007A =122 ,  xULD_TrayOpenRrBw        =122 , //
    x007B =123 ,  xULD_LTrayDetect         =123 , // 트레이 빈거랑 작업 한거 감지.
    x007C =124 ,  xULD_RTrayDetect         =124 , // 트레이 빈거랑 작업 한거 감지.
    x007D =125 ,  xULD_Door1               =125 , //
    x007E =126 ,  xULD_Door2               =126 , //
    x007F =127 ,  xULD_Door3               =127 , //

    x0080 =128 ,  xHED_1Ready              =128 , //
    x0081 =129 ,  xHED_1Busy               =129 , //
    x0082 =130 ,  xHED_1Result             =130 , //
    x0083 =131 ,  x131                     =131 , //
    x0084 =132 ,  xHED_2Ready              =132 , //
    x0085 =133 ,  xHED_2Busy               =133 , //
    x0086 =134 ,  xHED_2Result             =134 , //
    x0087 =135 ,  x135                     =135 , //
    x0088 =136 ,  xHED_3Ready              =136 , //
    x0089 =137 ,  xHED_3Busy               =137 , //
    x008A =138 ,  xHED_3Result             =138 , //
    x008B =139 ,  x139                     =139 , //
    x008C =140 ,  xHED_4Ready              =140 , //
    x008D =141 ,  xHED_4Busy               =141 , //
    x008E =142 ,  xHED_4Result             =142 , //
    x008F =143 ,  x143                     =143 , //

    MAX_INPUT
};

enum EN_OUTPUT_ID  {
    y0000 =  0 ,  yETC_LStartLp            =  0 , //
    y0001 =  1 ,  yETC_LStopLp             =  1 , //
    y0002 =  2 ,  yETC_LResetLp            =  2 , //
    y0003 =  3 ,  yETC_LAirLp              =  3 , //
    y0004 =  4 ,  yETC_LInitLp             =  4 , //
    y0005 =  5 ,  yETC_LVisnLp             =  5 , //
    y0006 =  6 ,  yETC_RStartLp            =  6 , //
    y0007 =  7 ,  yETC_RStopLp             =  7 , //
    y0008 =  8 ,  yETC_RResetLp            =  8 , //
    y0009 =  9 ,  yETC_RAirLp              =  9 , //
    y000A = 10 ,  yETC_RInitLp             = 10 , //
    y000B = 11 ,  yETC_RVisnLp             = 11 , //
    y000C = 12 ,  yETC_TwRedLp             = 12 , //
    y000D = 13 ,  yETC_TwYelLp             = 13 , //
    y000E = 14 ,  yETC_TwGrnLp             = 14 , //
    y000F = 15 ,  yETC_TwBzz               = 15 , //

    y0010 = 16 ,  yETC_MainSol             = 16 , //
    y0011 = 17 ,  yPRB_AirBlower           = 17 , //
    y0012 = 18 ,  yPRB_FeedingAC           = 18 , //
    y0013 = 19 ,  yWK1_FeedingAC           = 19 , //
    y0014 = 20 ,  yPSB_FeedingAC           = 20 , //
    y0015 = 21 ,  yPSB_RejectMotor         = 21 , //
    y0016 = 22 ,  y022                     = 22 , //
    y0017 = 23 ,  y023                     = 23 , //
    y0018 = 24 ,  yWR1_AlignFw             = 24 , //
    y0019 = 25 ,  yWR2_AlignFw             = 25 , //
    y001A = 26 ,  yWR3_AlignFw             = 26 , //
    y001B = 27 ,  yPSB_RejectOpenFw        = 27 , //
    y001C = 28 ,  yPRB_StprFw              = 28 , //
    y001D = 29 ,  yWK1_ClampFw             = 29 , //
    y001E = 30 ,  yWK1_StprFw              = 30 , //
    y001F = 31 ,  yWK2_StprFw              = 31 , //

    y0020 = 32 ,  yWK3_StprFw              = 32 , //
    y0021 = 33 ,  yPSB_StprFw              = 33 , //
    y0022 = 34 ,  y034                     = 34 , //
    y0023 = 35 ,  y035                     = 35 , //
    y0024 = 36 ,  yPSB_MarkingFw           = 36 , //
    y0025 = 37 ,  yWK1_IndxFw              = 37 , //
    y0026 = 38 ,  y038                     = 38 , //
    y0027 = 39 ,  y039                     = 39 , //
    y0028 = 40 ,  yPSB_RejectLiftFw        = 40 , //
    y0029 = 41 ,  yPSB_AlignFw             = 41 , //
    y002A = 42 ,  y042                     = 42 , //
    y002B = 43 ,  y043                     = 43 , //
    y002C = 44 ,  y044                     = 44 , //
    y002D = 45 ,  y045                     = 45 , //
    y002E = 46 ,  y046                     = 46 , //
    y002F = 47 ,  y047                     = 47 , //

    y0030 = 48 ,  y048                     = 48 , //
    y0031 = 49 ,  y049                     = 49 , //
    y0032 = 50 ,  y050                     = 50 , //
    y0033 = 51 ,  y051                     = 51 , //
    y0034 = 52 ,  y052                     = 52 , //
    y0035 = 53 ,  y053                     = 53 , //
    y0036 = 54 ,  y054                     = 54 , //
    y0037 = 55 ,  y055                     = 55 , //
    y0038 = 56 ,  y056                     = 56 , //
    y0039 = 57 ,  y057                     = 57 , //
    y003A = 58 ,  y058                     = 58 , //
    y003B = 59 ,  y059                     = 59 , //
    y003C = 60 ,  y060                     = 60 , //
    y003D = 61 ,  y061                     = 61 , //
    y003E = 62 ,  y062                     = 62 , //
    y003F = 63 ,  y063                     = 63 , //

    y0040 = 64 ,  yLDR_MarkingLp           = 64 , //
    y0041 = 65 ,  yLDR_PassLp              = 65 , //
    y0042 = 66 ,  yLDR_ReviewLp            = 66 , //
    y0043 = 67 ,  yLDR_ZBreak              = 67 , //
    y0044 = 68 ,  yLDR_PusherFw            = 68 , //
    y0045 = 69 ,  yLDR_RotatorFw           = 69 , //
    y0046 = 70 ,  yLDR_RotatorBw           = 70 , //
    y0047 = 71 ,  y071                     = 71 , //
    y0048 = 72 ,  yLDR_ClampFw             = 72 , //
    y0049 = 73 ,  yLDR_ClampBw             = 73 , //
    y004A = 74 ,  yLDR_MgzInPusherFw       = 74 , //
    y004B = 75 ,  yLDR_MgzInPusherBw       = 75 , //
    y004C = 76 ,  yLDR_MgzOutPusherFw      = 76 , //
    y004D = 77 ,  yLDR_MgzOutPusherBw      = 77 , //
    y004E = 78 ,  y078                     = 78 , //
    y004F = 79 ,  y079                     = 79 , //

    y0050 = 80 ,  y080                     = 80 , //
    y0051 = 81 ,  y081                     = 81 , //
    y0052 = 82 ,  y082                     = 82 , //
    y0053 = 83 ,  y083                     = 83 , //
    y0054 = 84 ,  y084                     = 84 , // 트레이 클램프 찝기
    y0055 = 85 ,  y085                     = 85 , //               풀기
    y0056 = 86 ,  y086                     = 86 , // 트레이클램프 왼쪽
    y0057 = 87 ,  y087                     = 87 , //              오른쪽
    y0058 = 88 ,  y088                     = 88 , // 트레이 클램프 다운.
    y0059 = 89 ,  y089                     = 89 , // 트레이 클램프 업.
    y005A = 90 ,  y090                     = 90 , //
    y005B = 91 ,  y091                     = 91 , //
    y005C = 92 ,  y092                     = 92 , // 트레이 홀드.
    y005D = 93 ,  y093                     = 93 , //
    y005E = 94 ,  y094                     = 94 , // 트레이 배출.
    y005F = 95 ,  y095                     = 95 , //

    y0060 = 96 ,  yULD_OpenLp              = 96 , // 배큠.
    y0061 = 97 ,  yULD_ZBreak1             = 97 , // 이젝터
    y0062 = 98 ,  yULD_ZBreak2             = 98 , // 배큠
    y0063 = 99 ,  yULD_FeedingAC           = 99 , // 이젝터.
    y0064 =100 ,  yULD_TrayClampFw         =100 , // Z Pick 모터 Break
    y0065 =101 ,  yULD_TrayClampBw         =101 , //
    y0066 =102 ,  yULD_TrayClampMoveLt     =102 , //
    y0067 =103 ,  yULD_TrayClampMoveRt     =103 , //
    y0068 =104 ,  yULD_TrayClampPickFw     =104 , //
    y0069 =105 ,  yULD_TrayClampPickBw     =105 , //
    y006A =106 ,  yULD_TrayColSelRt        =106 , //
    y006B =107 ,  yULD_TrayColSelLt        =107 , //
    y006C =108 ,  yULD_TrayHoldFw          =108 , //
    y006D =109 ,  yULD_TrayHoldBw          =109 , //
    y006E =110 ,  yULD_TrayOpenFw          =110 , //
    y006F =111 ,  yULD_TrayOpenBw          =111 , //

    y0070 =112 ,  yULD_PickVaccum1         =112 , //
    y0071 =113 ,  yULD_PickEjectr1         =113 , //
    y0072 =114 ,  yULD_PickVaccum2         =114 , //
    y0073 =115 ,  yULD_PickEjectr2         =115 , //
    y0074 =116 ,  yULD_ZPickBreak          =116 , //
    y0075 =117 ,  y117                     =117 , //
    y0076 =118 ,  yULD_PaperCmp            =118 , //
    y0077 =119 ,  yULD_PprRotator          =119 , //
    y0078 =120 ,  y120                     =120 , //
    y0079 =121 ,  y121                     =121 , //
    y007A =122 ,  y122                     =122 , //
    y007B =123 ,  y123                     =123 , //
    y007C =124 ,  y124                     =124 , //
    y007D =125 ,  y125                     =125 , //
    y007E =126 ,  y126                     =126 , //
    y007F =127 ,  y127                     =127 , //

    y0080 =128 ,  y128                      =128 , //
    y0081 =129 ,  yHED_1LotStart            =129 , //
    y0082 =130 ,  yHED_1JobChange           =130 , //
    y0083 =131 ,  yHED_1Reset               =131 , //
    y0084 =132 ,  y132                      =132 , //
    y0085 =133 ,  yHED_2LotStart            =133 , //
    y0086 =134 ,  yHED_2JobChange           =134 , //
    y0087 =135 ,  yHED_2Reset               =135 , //
    y0088 =136 ,  y136                      =136 , //
    y0089 =137 ,  yHED_3LotStart            =137 , //
    y008A =138 ,  yHED_3JobChange           =138 , //
    y008B =139 ,  yHED_3Reset               =139 , //
    y008C =140 ,  yHED_4Trigger             =140 , //
    y008D =141 ,  yHED_4LotStart            =141 , //
    y008E =142 ,  yHED_4JobChange           =142 , //
    y008F =143 ,  yHED_4Reset               =143 , //

    MAX_OUTPUT
};

enum EN_PSTN_ID {
    //밑에서 왔다갔다 하는놈. 푸셔.
    piLDR_XPshWait     = 0 ,
    piLDR_XPshWorkStt      ,
    piLDR_XPshWork         ,
    MAX_PSTN_ID_MOTR0      ,

    //메가진 Y축
    piLDR_YFedWait     = 0 ,
    piLDR_YFedWorkStt      ,
    piLDR_YFedPlace        ,
    piLDR_YFedWork         ,
    MAX_PSTN_ID_MOTR1      ,

    //위에서 찝는 놈 X
    piLDR_XPckWait     = 0 ,
    piLDR_XPckWorkStt      ,
    piLDR_XPckInsert       ,
    piLDR_XPckWork         ,
    MAX_PSTN_ID_MOTR2      ,

    //위에서 찝는 놈 Y
    piLDR_ZPckWait     = 0 ,
    piLDR_ZPckWork         ,
    piLDR_ZPckMove         ,  //piLDR_XPsh 가 움직이는 위치.
    piLDR_ZPckPlace        ,
    MAX_PSTN_ID_MOTR3      ,

    //스트립 플리퍼
    piWK1_YFlpNormal   = 0 ,
    piWK1_YFlpInverse      ,
    MAX_PSTN_ID_MOTR4      ,

    //비젼 메인 X
    piWRK_XVsnWait     = 0 ,
    piWRK_XVsnWorkStart    ,
    piWRK_XVsnWorkEnd      ,
    piWRK_XVsnTrgStt       ,
    piWRK_XVsnTrgEnd       ,
    MAX_PSTN_ID_MOTR5      ,

    //비젼 메인 Y
    piWRK_YVsnWait     = 0 ,
    piWRK_YVsnWorkStart    ,
    piWRK_YVsnWork         ,
    MAX_PSTN_ID_MOTR6      ,

    //포스트 버퍼 마킹 & 검사.
    piPSB_XMrkWait     = 0 ,
    piPSB_XMrkWorkStart    ,
    piPSB_XMrkVisnStart    ,
    piPSB_XMrkVisn         ,
    piPSB_XMrkWork         ,
    piPSB_XMrkReWork       ,
    MAX_PSTN_ID_MOTR7      ,

    //포스트 버퍼 마킹 & 검사.
    piPSB_YMrkWait     = 0 ,
    piPSB_YMrkWorkStart    ,
    piPSB_YMrkVisnStart    ,
    piPSB_YMrkVisn         ,
    piPSB_YMrkWork         ,
    piPSB_YMrkReWork       ,
    MAX_PSTN_ID_MOTR8      ,

    //인서트
    piWK1_XInsWait     = 0 ,
    piWK1_XInsStpr         ,
    piWK1_XInsOut          ,
    MAX_PSTN_ID_MOTR9      ,

    //언로더 픽커 Z
    piULD_ZPckWait     = 0 ,
    piULD_ZPckMove         ,
    piULD_ZPckPick         ,
    piULD_ZPckPaper        ,
    piULD_ZPckPlace        ,
    MAX_PSTN_ID_MOTR10     ,

    //언로더 픽커 Y
    piULD_YPckWait     = 0 ,
    piULD_YPckPaper        ,
    piULD_YPckPick         ,
    piULD_YPckPlaceStt     ,
    piULD_YPckPlace        ,
    MAX_PSTN_ID_MOTR11     ,

    //빈트레이 올려주는놈.
    piULD_ZTrIWait     = 0 ,
    piULD_ZTrIWorkStt      , //첫 자제 높이.
    piULD_ZTrIWorkEnd      , //마지막 자제 높이.
    MAX_PSTN_ID_MOTR12     ,

    //작업엔트 트레이 올려주는 놈.
    piULD_ZTrOWait     = 0 ,
    piULD_ZTrOWorkStt      , //첫 자제 높이.
    piULD_ZTrOWorkEnd      , //마지막 자제 높이.
    MAX_PSTN_ID_MOTR13     ,

    //언로더 스토퍼 위치 조정.
    piULD_XStpWait     = 0 ,
    piULD_XStpBwd          ,
    piULD_XStpFwd          ,
    MAX_PSTN_ID_MOTR14     ,

    //종이 올려주는 놈.
    piULD_ZPprWait     = 0 ,
    piULD_ZPprWorkStt      ,
    piULD_ZPprWorkEnd      ,
    MAX_PSTN_ID_MOTR15     ,
};

enum EN_PSTN_VALUE {
    pvLDR_XPshWait     = 0 ,
    pvLDR_XPshWorkStt      ,
    MAX_PSTN_MOTR0         ,

    pvLDR_YFedWait     = 0 ,
    pvLDR_YFedWorkStt      ,
    pvLDR_YFedPlace        ,
    MAX_PSTN_MOTR1         ,

    pvLDR_XPckWait     = 0 ,
    pvLDR_XPckWorkStt      ,
    pvLDR_XPckInsert       ,
    MAX_PSTN_MOTR2         ,

    pvLDR_ZPckWait     = 0 ,
    pvLDR_ZPckWork         ,
    pvLDR_ZPckMove         ,
    pvLDR_ZPckPlace        ,
    MAX_PSTN_MOTR3         ,

    pvWK1_YFlpNormal   = 0 ,
    pvWK1_YFlpInverse      ,
    MAX_PSTN_MOTR4         ,

    pvWRK_XVsnWait     = 0 ,
    pvWRK_XVsnWorkStart    ,
    pvWRK_XVsnWorkEnd      ,
    MAX_PSTN_MOTR5         ,

    pvWRK_YVsnWait     = 0 ,
    pvWRK_YVsnWorkStart    ,
    MAX_PSTN_MOTR6         ,

    pvPSB_XMrkWait     = 0 ,
    pvPSB_XMrkWorkStart    ,
    pvPSB_XMrkVisnStart    ,
    MAX_PSTN_MOTR7         ,

    pvPSB_YMrkWait     = 0 ,
    pvPSB_YMrkWorkStart    ,
    pvPSB_YMrkVisnStart    ,
    MAX_PSTN_MOTR8         ,

    pvWK1_XInsWait     = 0 ,
    pvWK1_XInsStpr         ,
    pvWK1_XInsOut          ,
    MAX_PSTN_MOTR9         ,

    pvULD_ZPckWait     = 0 ,
    pvULD_ZPckMove         ,
    pvULD_ZPckPick         ,
    pvULD_ZPckPaper        ,
    pvULD_ZPckPlace        ,
    MAX_PSTN_MOTR10        ,

    pvULD_YPckWait     = 0 ,
    pvULD_YPckPaper        ,
    pvULD_YPckPick         ,
    pvULD_YPckPlaceStt     ,
    MAX_PSTN_MOTR11        ,

    pvULD_ZTrIWait     = 0 ,                            
    pvULD_ZTrIWorkStt      , //첫 트레이감지 포지션.    
    pvULD_ZTrIWorkEnd      , //마지막 트레이감지 포지션.
    MAX_PSTN_MOTR12        ,

    pvULD_ZTrOWait     = 0 ,
    pvULD_ZTrOWorkStt      , //첫번째 트레이 시작 포지션.
    pvULD_ZTrOWorkEnd      , //마지막 트레이 시작 포지션.
    MAX_PSTN_MOTR13        ,

    pvULD_XStpWait     = 0 ,
    pvULD_XStpBwd          ,
    pvULD_XStpFwd          ,
    MAX_PSTN_MOTR14        ,

    pvULD_ZPprWait     = 0 ,
    pvULD_ZPprWorkStt      ,
    pvULD_ZPprWorkEnd      ,
    MAX_PSTN_MOTR15        ,
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcLDR_Home             = 10 , //10
    mcLDR_Supply                , //11
    mcLDR_Work                  , //12
    mcLDR_Pick                  , //13
    mcLDR_Out                   , //14
    mcLDR_WorkSttPs             , //15
    mcLDR_PusherUpDn            , //15
    mcLDR_RotateFwBw            , //16
    mcRAL_PreStprDn             , //24

    mcHED_Home             = 30 , //30
    mcHED_InspMan               , //31
    mcHED_Wk1CmpFB              , //32
    mcHED_Wk1InsFB              , //33
    mcHED_Wk1AlgnFB             , //34
    mcHED_Wk1StrpFB             , //35
    mcHED_Wk1InversePs          , //36
    mcHED_Wk1NormalPs           , //37
    mcHED_Wk2AlgnFB             , //38
    mcHED_Wk2StrpFB             , //39
    mcHED_Wk3AlgnFB             , //40
    mcHED_Wk3StrpFB             , //41
    mcHED_FeedingOnOff          , //42

    mcPSB_Home             = 50 , //50
    mcPSB_InitPos               , //51
    mcPSB_MrkSttPs              , //52
    mcPSB_VisnSttPs             , //53
    mcPSB_AlgnFB                , //54
    mcPSB_StprUD                , //55
    mcPSB_MarkingUD             , //56
    mcPSB_MrkChangPs            , //57
    mcPSB_TestMarking           , //58
    mcPSB_FeedingOnOff          , //59
    mcPSB_Remove                , //60
    mcPSB_ReviewNext            , //61

    mcULD_Home             =  70, //70
    mcULD_StprFwPs              , //71
    mcULD_StprBwPs              , //72
    mcULD_VaccumOnOff           , //73
    mcULD_EjectOnOff            , //74
    mcULD_TrayMove              , //75
    mcULD_TrayOpen              , //76
    mcULD_PaperSupply           , //77
    mcULD_PaperWorkStt          , //78
    mcULD_PickPs                , //79
    mcULD_FrstPlacePs           , //80
    mcULD_PaperPs               , //81
    mcULD_Pick                  , //82
    mcULD_Place                 , //83

    mcRAL_Home             =  90, //90
    mcULM_Home                  , //91 Transfer, Flipper Motor Home
    mcETC_AllBeltOn             , //92
    mcETC_AllBeltOff            , //93
    mcHED_AllStprUp             , //94
    mcETC_ALlStprDn             , //95
    mcETC_MainAirOff            , //96
    mcETC_ManLotEnd             , //97
    mcETC_InTrayStt             , //98
    mcETC_OutTrayStt            , //99
    mcETC_PaperStt              , //100
    mcETC_PaperSupply           , //101

    MAX_MANUAL_CYCLE
};
#endif


