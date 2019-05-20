        //---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"

//---------------------------------------------------------------------------
#include "Loader.h"
#include "PostBuffer.h"
#include "LaserEO.h"
//#include "MarkVisn.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"                       
#include "OptionMan.h"
#include "Sequence.h"

#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "UserIni.h"
#include "MarkVisn.h"

//#include "SAInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{
    DM.ARAY[riLDR].SetParent(pnLDR ); DM.ARAY[riLDR].SetConfig(false , "LDR"); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPRB ); DM.ARAY[riPRB].SetConfig(false , "PRB"); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riLSR].SetParent(pnLSR ); DM.ARAY[riLSR].SetConfig(false , "LSR"); DM.ARAY[riLSR].ClearDispItem();
    DM.ARAY[riVSN].SetParent(pnVSN ); DM.ARAY[riVSN].SetConfig(false , "VSN"); DM.ARAY[riVSN].ClearDispItem();
    DM.ARAY[riPSB].SetParent(pnPSB ); DM.ARAY[riPSB].SetConfig(false , "PSB"); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnULD ); DM.ARAY[riULD].SetConfig(false , "ULD"); DM.ARAY[riULD].ClearDispItem();

    /*
//프리버퍼 오리검사 레이져조사 비젼검사
//Unkwn -> Mask     Work       Good
//                             Fail
    */

    DM.ARAY[riLDR].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riLDR].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riLDR].SetDispItem(csUnkwn  ,"Unknown"  ) ;

    DM.ARAY[riPRB].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riPRB].SetDispItem(csUnkwn  ,"BfVision" ) ;
    DM.ARAY[riPRB].SetDispItem(csMask   ,"VisionEnd") ;

    DM.ARAY[riLSR].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riLSR].SetDispItem(csMask   ,"NeedWork" ) ;
    DM.ARAY[riLSR].SetDispItem(csWork   ,"LsrWorked") ;

    DM.ARAY[riVSN].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riVSN].SetDispItem(csWork   ,"NoInsp"   ) ;
    DM.ARAY[riVSN].SetDispItem(csWait   ,"NeedInsp" ) ;
    DM.ARAY[riVSN].SetDispItem(csRslt0  ,"V_Good"             ) ;
    DM.ARAY[riVSN].SetDispItem(csRslt1  ,"V_Empty"            ) ;
    DM.ARAY[riVSN].SetDispItem(csRslt2  ,"V_MatchingError"    ) ;
    DM.ARAY[riVSN].SetDispItem(csRslt3  ,"V_MissingMark"      ) ;
    DM.ARAY[riVSN].SetDispItem(csRslt4  ,"V_WrongPositionMark") ;
    DM.ARAY[riVSN].SetDispItem(csRslt5  ,"V_MissingChar"      ) ;
    DM.ARAY[riVSN].SetDispItem(csRslt6  ,"V_WrongChar"        ) ;

    DM.ARAY[riPSB].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riPSB].SetDispItem(csWork   ,"NoInsp"   ) ;
    DM.ARAY[riPSB].SetDispItem(csWait   ,"NeedInsp" ) ;
    DM.ARAY[riPSB].SetDispItem(csRslt0  ,"V_Good"             ) ;
    DM.ARAY[riPSB].SetDispItem(csRslt1  ,"V_Empty"            ) ;
    DM.ARAY[riPSB].SetDispItem(csRslt2  ,"V_MatchingError"    ) ;
    DM.ARAY[riPSB].SetDispItem(csRslt3  ,"V_MissingMark"      ) ;
    DM.ARAY[riPSB].SetDispItem(csRslt4  ,"V_WrongPositionMark") ;
    DM.ARAY[riPSB].SetDispItem(csRslt5  ,"V_MissingChar"      ) ;
    DM.ARAY[riPSB].SetDispItem(csRslt6  ,"V_WrongChar"        ) ;

    DM.ARAY[riULD].SetDispItem(csNone   ,"NoMgz  " ) ;
    DM.ARAY[riULD].SetDispItem(csEmpty  ,"Empty  " ) ;
    DM.ARAY[riULD].SetDispItem(csWork   ,"Exist  " ) ;

    DM.LoadMap();

    TUserINI   UserINI;
    AnsiString sPath  ;
    int iLang         ;
    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    UserINI.Load(sPath , "Member"     , "m_iLangSel"  , iLang);

    const bool bKorean = iLang==1;


    //Manual Button Caption
    //btMan1_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan1_14 -> Tag = (int)mcAllHome            ; btMan1_14 -> Visible = true  ;
    //btMan2_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan2_14 -> Tag = (int)mcAllHome            ; btMan2_14 -> Visible = true  ;
    //btMan3_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan3_14 -> Tag = (int)mcAllHome            ; btMan3_14 -> Visible = true  ;
    //btMan4_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan4_14 -> Tag = (int)mcAllHome            ; btMan4_14 -> Visible = true  ;
    //btMan5_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan5_14 -> Tag = (int)mcAllHome            ; btMan5_14 -> Visible = true  ;


    //Loader
    btMan1_1  -> Caption = "[" + AnsiString(mcLDR_Supply      ) + "]" + (bKorean ? "매거진 공급"       : "Loading Supply"    );  btMan1_1  -> Tag = (int)mcLDR_Supply       ; btMan1_1  -> Visible = true  ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_Pick        ) + "]" + (bKorean ? "매거진 집기"       : "Loading Pick"      );  btMan1_2  -> Tag = (int)mcLDR_Pick         ; btMan1_2  -> Visible = true  ;
    btMan1_3  -> Caption = "[" + AnsiString(mcLDR_SttPstn     ) + "]" + (bKorean ? "로더 시작 위치"    : "Move Start Pos"    );  btMan1_3  -> Tag = (int)mcLDR_SttPstn      ; btMan1_3  -> Visible = true  ;
    btMan1_4  -> Caption = "[" + AnsiString(mcLDR_Place       ) + "]" + (bKorean ? "매거진 배출"       : "Loading Place"     );  btMan1_4  -> Tag = (int)mcLDR_Place        ; btMan1_4  -> Visible = true  ;
    btMan1_5  -> Caption = "[" + AnsiString(mcLDR_UpSlPitch   ) + "]" + (bKorean ? "로더 피치 업"      : "Pitch Up"          );  btMan1_5  -> Tag = (int)mcLDR_UpSlPitch    ; btMan1_5  -> Visible = true  ;
    btMan1_6  -> Caption = "[" + AnsiString(mcLDR_DnSlPitch   ) + "]" + (bKorean ? "로더 피치 다운"    : "Pitch Dn"          );  btMan1_6  -> Tag = (int)mcLDR_DnSlPitch    ; btMan1_6  -> Visible = true  ;
    btMan1_7  -> Caption = "[" + AnsiString(mcLDR_PusherFwBw  ) + "]" + (bKorean ? "로더 푸셔"         : "Pusher Fw/Bw"      );  btMan1_7  -> Tag = (int)mcLDR_PusherFwBw   ; btMan1_7  -> Visible = true  ;
    btMan1_8  -> Caption = "[" + AnsiString(mcLDR_ClampDnUp   ) + "]" + (bKorean ? "로더 클램프"       : "Clamp Dn/Up"       );  btMan1_8  -> Tag = (int)mcLDR_ClampDnUp    ; btMan1_8  -> Visible = true  ;
    btMan1_9  -> Caption = "[" + AnsiString(mcLDR_TranInAC    ) + "]" + (bKorean ? "진입 AC모터"       : "Feed In On/Off"    );  btMan1_9  -> Tag = (int)mcLDR_TranInAC     ; btMan1_9  -> Visible = true  ;
    btMan1_10 -> Caption = "[" + AnsiString(mcLDR_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"       : "Feed Out On/Off"   );  btMan1_10 -> Tag = (int)mcLDR_TranOutAC    ; btMan1_10 -> Visible = true  ;



    //Prebuffer / Postbuffer
    btMan2_1  -> Caption = "[" + AnsiString(mcPSB_GrprGrRl    ) + "]" + (bKorean ? "포스트버퍼 그리퍼" : "PSB Grip Gr/Rl"    );  btMan2_1  -> Tag = (int)mcPSB_GrprGrRl     ; btMan2_1  -> Visible = true  ;
    btMan2_2  -> Caption = "[" + AnsiString(mcPSB_RailOpCl    ) + "]" + (bKorean ? "레일 오픈/클로즈"  : "PSB Rail Op/Cl"    );  btMan2_2  -> Tag = (int)mcPSB_RailOpCl     ; btMan2_2  -> Visible = true  ;
    btMan2_3  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "포스트버퍼 인덱스" : "PSB Index Dn/Up"   );  btMan2_3  -> Tag = (int)mcPSB_IndxDnUp     ; btMan2_3  -> Visible = true  ;
    btMan2_4  -> Caption = "[" + AnsiString(mcPSB_ElvtUpDn    ) + "]" + (bKorean ? "엘리베이터"        : "PSB Elevator Up/Dn");  btMan2_4  -> Tag = (int)mcPSB_ElvtUpDn     ; btMan2_4  -> Visible = true  ;
    //btMan2_5  -> Caption = "[" + AnsiString(mcPSB_PshrDnUp    ) + "]" + (bKorean ? "업다운 푸셔"       : "PSB Pusher Dn/Up"  );  btMan2_5  -> Tag = (int)mcPSB_PshrDnUp     ; btMan2_5  -> Visible = true  ;
    btMan2_6  -> Caption = "[" + AnsiString(mcPSB_PshrFwBw    ) + "]" + (bKorean ? "전후진 푸셔"       : "PSB Pusher Fw/Bw"  );  btMan2_6  -> Tag = (int)mcPSB_PshrFwBw     ; btMan2_6  -> Visible = true  ;
    btMan2_7  -> Caption = "[" + AnsiString(mcPSB_CycleReject ) + "]" + (bKorean ? "불량 자제 배출"    : "PSB Cycle Reject"  );  btMan2_7  -> Tag = (int)mcPSB_CycleReject  ; btMan2_7  -> Visible = true  ;

    btMan2_8  -> Caption = "[" + AnsiString(mcPRB_StprUpDn    ) + "]" + (bKorean ? "프리버퍼 스토퍼"   : "PRB Stopper Up/Dn" );  btMan2_8  -> Tag = (int)mcPRB_StprUpDn     ; btMan2_8  -> Visible = true  ;
    btMan2_9  -> Caption = "[" + AnsiString(mcPRB_FeedAC      ) + "]" + (bKorean ? "프리버퍼 AC모터"   : "PRB Feed On/Off"   );  btMan2_9  -> Tag = (int)mcPRB_FeedAC       ; btMan2_9  -> Visible = true  ;
    btMan2_10 -> Caption = "[" + AnsiString(mcPRB_AirBlow     ) + "]" + (bKorean ? "프리버퍼 에어분사" : "PRB AirBlow On/Off");  btMan2_10 -> Tag = (int)mcPRB_AirBlow      ; btMan2_10 -> Visible = true  ;
    btMan2_11 -> Caption = "[" + AnsiString(mcPRB_CycleVisn   ) + "]" + (bKorean ? "프리버퍼 비전검사" : "PRB CycleVision"   );  btMan2_11 -> Tag = (int)mcPRB_CycleVisn    ; btMan2_11 -> Visible = true  ;
    //btMan2_14 -> Caption = "[" + AnsiString(mcRAL_PlaceStrp   ) + "]" + (bKorean ? "레일 자제 배출"    : "RAL Place Strip"   );  btMan2_14 -> Tag = (int)mcRAL_PlaceStrp    ; btMan2_14 -> Visible = true  ;
    btMan2_13 -> Caption = "[" + AnsiString(mcRAL_CycleCnvr   ) + "]" + (bKorean ? "레일 Y 컨버젼"     : "Rail Y Conversion"   );  btMan2_13 -> Tag = (int)mcRAL_CycleCnvr  ; btMan2_13 -> Visible = true  ;
    btMan2_14 -> Caption = "[" + AnsiString(mcRailOutAll      ) + "]" + (bKorean ? "레일 자제 배출"    : "RAL All Strip Out"   );  btMan2_14 -> Tag = (int)mcRailOutAll     ; btMan2_14 -> Visible = true  ;


    //Laser / Rail
    btMan3_1  -> Caption = "[" + AnsiString(mcRAL_LStpUpDn    ) + "]" + (bKorean ? "레이저 스토퍼"     : "RAL LStopper Up/Dn");  btMan3_1  -> Tag = (int)mcRAL_LStpUpDn     ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcRAL_LStnUpDn    ) + "]" + (bKorean ? "레이저 스테이션"   : "RAL LStation Up/Dn");  btMan3_2  -> Tag = (int)mcRAL_LStnUpDn     ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcRAL_LAlnFwBw    ) + "]" + (bKorean ? "레이저 얼라인"     : "RAL LAlign Fw/Bw"  );  btMan3_3  -> Tag = (int)mcRAL_LAlnFwBw     ; btMan3_3  -> Visible = true  ;
    btMan3_4  -> Caption = "[" + AnsiString(mcRAL_VStpUpDn    ) + "]" + (bKorean ? "비젼 스토퍼"       : "RAL VStopper Up/Dn");  btMan3_4  -> Tag = (int)mcRAL_VStpUpDn     ; btMan3_4  -> Visible = true  ;
    btMan3_5  -> Caption = "[" + AnsiString(mcRAL_VStnUpDn    ) + "]" + (bKorean ? "비젼 스테이션"     : "RAL VStation Up/Dn");  btMan3_5  -> Tag = (int)mcRAL_VStnUpDn     ; btMan3_5  -> Visible = true  ;
    btMan3_6  -> Caption = "[" + AnsiString(mcRAL_VAlnFwBw    ) + "]" + (bKorean ? "비젼 얼라인"       : "RAL VAlign Fw/Bw"  );  btMan3_6  -> Tag = (int)mcRAL_VAlnFwBw     ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcRAL_FeedAC      ) + "]" + (bKorean ? "레일 AC모터"       : "RAL Feed On/Off"   );  btMan3_7  -> Tag = (int)mcRAL_FeedAC       ; btMan3_7  -> Visible = true  ;
    btMan3_8  -> Caption = "[" + AnsiString(mcLSR_AVsnFwBw    ) + "]" + (bKorean ? "얼라인 비전 이동"  : "LSR Alg Visn Fw/Bw");  btMan3_8  -> Tag = (int)mcLSR_AVsnFwBw     ; btMan3_8  -> Visible = true  ;
    btMan3_9  -> Caption = "[" + AnsiString(mcRAL_MoveNextPart) + "]" + (bKorean ? "레일 비전으로이동" : "RAL Move MarkZone" );  btMan3_9  -> Tag = (int)mcRAL_MoveNextPart ; btMan3_9  -> Visible = true ;
    btMan3_10 -> Caption = "[" + AnsiString(mcVSN_CycleReject ) + "]" + (bKorean ? "비젼 존 자재 제거" : "VSN Zone Reject"   );  btMan3_10 -> Tag = (int)mcVSN_CycleReject  ; btMan3_10 -> Visible = true ;
    btMan3_11 -> Caption = "[" + AnsiString(mcLSR_CycleWork   ) + "]" + (bKorean ? "비젼검사 및 조사"  : "LSR Cycle Work"    );  btMan3_11 -> Tag = (int)mcLSR_CycleWork    ; btMan3_11 -> Visible = true ;
    btMan3_12 -> Caption = "[" + AnsiString(mcRAL_Bind        ) + "]" + (bKorean ? "레일 바인딩"       : "RAL Bind"          );  btMan3_12 -> Tag = (int)mcRAL_Bind         ; btMan3_12 -> Visible = true  ;
    btMan3_13 -> Caption = "[" + AnsiString(mcRAL_UnBind      ) + "]" + (bKorean ? "레일 언바인딩"     : "RAL UnBind"        );  btMan3_13 -> Tag = (int)mcRAL_UnBind       ; btMan3_13 -> Visible = true  ;
    btMan3_14 -> Caption = "[" + AnsiString(mcLSR_CycleCntrMrk) + "]" + (bKorean ? "레이져 센터 마킹"  : "LSR Center Marking");  btMan3_14 -> Tag = (int)mcLSR_CycleCntrMrk ; btMan3_14 -> Visible = true  ;

    //Vision / Rail
    btMan4_1  -> Caption = "[" + AnsiString(mcRAL_LStpUpDn    ) + "]" + (bKorean ? "레이저 스토퍼"     : "RAL LStopper Up/Dn");  btMan4_1  -> Tag = (int)mcRAL_LStpUpDn     ; btMan4_1  -> Visible = true  ;
    btMan4_2  -> Caption = "[" + AnsiString(mcRAL_LStnUpDn    ) + "]" + (bKorean ? "레이저 스테이션"   : "RAL LStation Up/Dn");  btMan4_2  -> Tag = (int)mcRAL_LStnUpDn     ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcRAL_LAlnFwBw    ) + "]" + (bKorean ? "레이저 얼라인"     : "RAL LAlign Fw/Bw"  );  btMan4_3  -> Tag = (int)mcRAL_LAlnFwBw     ; btMan4_3  -> Visible = true  ;
    btMan4_4  -> Caption = "[" + AnsiString(mcRAL_VStpUpDn    ) + "]" + (bKorean ? "비젼 스토퍼"       : "RAL VStopper Up/Dn");  btMan4_4  -> Tag = (int)mcRAL_VStpUpDn     ; btMan4_4  -> Visible = true  ;
    btMan4_5  -> Caption = "[" + AnsiString(mcRAL_VStnUpDn    ) + "]" + (bKorean ? "비젼 스테이션"     : "RAL VStation Up/Dn");  btMan4_5  -> Tag = (int)mcRAL_VStnUpDn     ; btMan4_5  -> Visible = true  ;
    btMan4_6  -> Caption = "[" + AnsiString(mcRAL_VAlnFwBw    ) + "]" + (bKorean ? "비젼 얼라인"       : "RAL VAlign Fw/Bw"  );  btMan4_6  -> Tag = (int)mcRAL_VAlnFwBw     ; btMan4_6  -> Visible = true  ;
    btMan4_7  -> Caption = "[" + AnsiString(mcVSN_MaskChange  ) + "]" + (bKorean ? "마스크 교체 위치"  : "Mask Change Pos"   );  btMan4_7  -> Tag = (int)mcVSN_MaskChange   ; btMan4_7  -> Visible = true  ;

    btMan4_8  -> Caption = "[" + AnsiString(mcVSN_XPitchP     ) + "]" + (bKorean ? "X축 피치 이동 +"   : "VSN Visn XPitch +" );  btMan4_8  -> Tag = (int)mcVSN_XPitchP      ; btMan4_8  -> Visible = true  ;
    btMan4_9  -> Caption = "[" + AnsiString(mcVSN_XPitchN     ) + "]" + (bKorean ? "X축 피치 이동 -"   : "VSN Visn XPitch -" );  btMan4_9  -> Tag = (int)mcVSN_XPitchN      ; btMan4_9  -> Visible = true  ;
    btMan4_10 -> Caption = "[" + AnsiString(mcVSN_YPitchP     ) + "]" + (bKorean ? "Y축 피치 이동 +"   : "VSN Visn YPitch +" );  btMan4_10 -> Tag = (int)mcVSN_YPitchP      ; btMan4_10 -> Visible = true  ;
    btMan4_11 -> Caption = "[" + AnsiString(mcVSN_YPitchN     ) + "]" + (bKorean ? "Y축 피치 이동 -"   : "VSN Visn YPitch -" );  btMan4_11 -> Tag = (int)mcVSN_YPitchN      ; btMan4_11 -> Visible = true  ;
    btMan4_12 -> Caption = "[" + AnsiString(mcVSN_CycleVisn   ) + "]" + (bKorean ? "마킹 비젼검사"     : "VSN Cycle Vision"  );  btMan4_12 -> Tag = (int)mcVSN_CycleVisn    ; btMan4_12 -> Visible = true  ;
    btMan4_13 -> Caption = "[" + AnsiString(mcVSN_CycleSttPos ) + "]" + (bKorean ? "비젼검사 시작위치" : "VSN Cycle Stt Pos" );  btMan4_13 -> Tag = (int)mcVSN_CycleSttPos  ; btMan4_13 -> Visible = true  ;

    //Unloader
    btMan5_1  -> Caption = "[" + AnsiString(mcULD_Supply      ) + "]" + (bKorean ? "매거진 공급"       : "UnLoading Supply"  );  btMan5_1  -> Tag = (int)mcULD_Supply       ; btMan5_1  -> Visible = true  ;
    btMan5_2  -> Caption = "[" + AnsiString(mcULD_Pick        ) + "]" + (bKorean ? "매거진 집기"       : "UnLoading Pick"    );  btMan5_2  -> Tag = (int)mcULD_Pick         ; btMan5_2  -> Visible = true  ;
    btMan5_3  -> Caption = "[" + AnsiString(mcULD_SttPstn     ) + "]" + (bKorean ? "언로더 시작 위치"  : "Move Start Pos"    );  btMan5_3  -> Tag = (int)mcULD_SttPstn      ; btMan5_3  -> Visible = true  ;
    btMan5_4  -> Caption = "[" + AnsiString(mcULD_Place       ) + "]" + (bKorean ? "매거진 배출"       : "UnLoading Place"   );  btMan5_4  -> Tag = (int)mcULD_Place        ; btMan5_4  -> Visible = true  ;
    btMan5_5  -> Caption = "[" + AnsiString(mcULD_UpSlPitch   ) + "]" + (bKorean ? "언로더 피치 업"    : "UnLoader Pitch Up" );  btMan5_5  -> Tag = (int)mcULD_UpSlPitch    ; btMan5_5  -> Visible = true  ;
    btMan5_6  -> Caption = "[" + AnsiString(mcULD_DnSlPitch   ) + "]" + (bKorean ? "언로더 피치 다운"  : "UnLoader Pitch Dn" );  btMan5_6  -> Tag = (int)mcULD_DnSlPitch    ; btMan5_6  -> Visible = true  ;
    btMan5_7  -> Caption = "[" + AnsiString(mcULD_ClampDnUp   ) + "]" + (bKorean ? "언로더 클램프"     : "Clamp Dn/Up"       );  btMan5_7  -> Tag = (int)mcULD_ClampDnUp    ; btMan5_7  -> Visible = true  ;
    //btMan5_8  -> Caption = "[" + AnsiString(mcPSB_PshrDnUp    ) + "]" + (bKorean ? "PSB 업다운 푸셔"   : "PSB Pusher Dn/Up"  );  btMan5_8  -> Tag = (int)mcPSB_PshrDnUp     ; btMan5_8  -> Visible = true  ;
    btMan5_9  -> Caption = "[" + AnsiString(mcPSB_PshrFwBw    ) + "]" + (bKorean ? "PSB 전후진 푸셔"   : "PSB Pusher Fw/Bw"  );  btMan5_9  -> Tag = (int)mcPSB_PshrFwBw     ; btMan5_9  -> Visible = true  ;
    btMan5_10 -> Caption = "[" + AnsiString(mcULD_TranInAC    ) + "]" + (bKorean ? "진입 AC모터"       : "Feeding In On/Off" );  btMan5_10 -> Tag = (int)mcULD_TranInAC     ; btMan5_10 -> Visible = true  ;
    btMan5_11 -> Caption = "[" + AnsiString(mcULD_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"       : "Feeding Out On/Off");  btMan5_11 -> Tag = (int)mcULD_TranOutAC    ; btMan5_11 -> Visible = true  ;

    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;

    btPrbClear -> Tag = riPRB ;
    btLsrClear -> Tag = riLSR ;
    btVsnClear -> Tag = riVSN ;
    btPsbClear -> Tag = riPSB ;
    btPrbMask  -> Tag = riPRB ;
    btPrbInput -> Tag = riPRB ;

   //  OM.LoadDevOptn(OM.GetCrntDev());


    //SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)                         // 오퍼레이션 화면을 한 번 보여줄 때마다, 이 함수를 탐
{
    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    if(LSR.m_bManMark) {
        cbManMark -> Font -> Color = SEQ._bFlick ? clRed : clBlack ;
        edManMark -> Enabled = true ;
        btSetManMark -> Enabled = true ;
        edManMark -> Hint = OM.DevOptn.sLaserLotFormat ;
    }
    else {
        cbManMark -> Font -> Color = clBlack ;
        edManMark -> Enabled = false ;
        btSetManMark -> Enabled = false ;
    }



    lbEmptyCnt -> Visible = FM_GetLevel() == lvMaster ;

    lbEmptyCnt -> Caption = LDR.m_iEmptyStrpCnt ;

    //Label24 -> Caption =  VSN.m_mmm;

    Label19 -> Caption = "ID:" +DM.ARAY[riLDR].GetID() + " MgzCnt:" + LDR._iMgzCnt ;
    Label20 -> Caption = "ID:" +DM.ARAY[riULD].GetID() ;

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    /*
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                              }
                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }
    */

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    //Button Start Enabled
    //if(OM.MstOptn.bNotUstLot) btStart  -> Enabled = true          ;
    //else                      btStart  -> Enabled = LT.m_bLotOpen ;

    SPC.LOT.DispLotInfo(sgLotInfo);
    SPC.DAY.DispDayInfo(sgDayInfo);

    //EM_Disp
    AnsiString Str;
    static int iPreErrCnt  ;
    int        iCrntErrCnt ;
    for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
        if (EM_GetErr(i)) iCrntErrCnt++;
    }
    if (iPreErrCnt != iCrntErrCnt ) {
        lbErr -> Clear();
        AnsiString Str;
        lbErr -> Items -> Clear();
        int iErrNo = EM_GetLastErr();
        for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
            if (EM_GetErr(i)) {
                Str.sprintf("[ERR%03d]" , i) ;
                Str += EM_GetErrName(i) + AnsiString(" ") + EM_GetErrMsg(i);
                lbErr -> Items -> Add(Str);
                if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
            }
        }
    }
    iPreErrCnt = iCrntErrCnt ;



    pnIgnrDoor     -> Color  =  OM.CmnOptn.bIgnrDoor    ? clLime : clRed ;
    pnLoadingStop  -> Color  =  OM.CmnOptn.bLoadingStop ? clLime : clRed ;
    pnSkipLaser    -> Color  =  OM.CmnOptn.bSkipLaser   ? clLime : clRed ;
    pnSkipOriVsn   -> Color  =  OM.CmnOptn.bSkipOriVisn ? clLime : clRed ;
    pnSkipAlgVsn   -> Color  =  OM.CmnOptn.bSkipAlnVisn ? clLime : clRed ;
    pnSkipMrkVsn   -> Color  =  OM.CmnOptn.iMarkInsp==0 ? clAqua :(OM.CmnOptn.iMarkInsp==1 ? clRed : clLime) ;

    pnIgnrDoor     -> Caption  =  OM.CmnOptn.bIgnrDoor    ? "ON" : "OFF" ;
    pnLoadingStop  -> Caption  =  OM.CmnOptn.bLoadingStop ? "ON" : "OFF" ;
    pnSkipLaser    -> Caption  =  OM.CmnOptn.bSkipLaser   ? "ON" : "OFF" ;
    pnSkipOriVsn   -> Caption  =  OM.CmnOptn.bSkipOriVisn ? "ON" : "OFF" ;
    pnSkipAlgVsn   -> Caption  =  OM.CmnOptn.bSkipAlnVisn ? "ON" : "OFF" ;
    pnSkipMrkVsn   -> Caption  =  OM.CmnOptn.iMarkInsp==0 ? "ONE" :(OM.CmnOptn.iMarkInsp==1 ? "ALL" : "ON") ;

    //btLotOpen -> Enabled = !OM.MstOptn.bNotUstLot ;
    //btLotEnd  -> Enabled = !OM.MstOptn.bNotUstLot ;

    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;

    //메뉴얼 버튼 색깔 칠하기.
    for(int i = 1 ; i < 6/*페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;
            if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
            else                             pBtn -> Font -> Color = clBlack ;
        }
    }

    for(int i = 0 ; i < MAX_ARAY ; i++) {
        DM.ARAY[i].UpdateAray();
    }

    bool bAllArayNone = DM.ARAY[riLDR].CheckAllStat(csNone) && DM.ARAY[riLSR].CheckAllStat(csNone) && DM.ARAY[riPRB].CheckAllStat(csNone) &&
                        DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) && DM.ARAY[riVSN].CheckAllStat(csNone)  ;

    //if(LTQ.GetLotCnt() != 0) m_bLotOpenMsgShow = false;
    if((LTQ.GetLotCnt() == 0 && bAllArayNone) || LSR.m_bManMark){
        btLotEnd -> Enabled = false;  // 랏큐에 랏이 없고 모든 어래이가 None 일 경우 버튼 인에이블...
        btStart  -> Enabled = false;

        /*
        if(!m_bLotOpenMsgShow){
            FM_MsgOk("WARNING" , "Please Lot Open");
            m_bLotOpenMsgShow = true;
        }*/
    }
    else{
        btLotEnd -> Enabled = true;
        btStart  -> Enabled = true;

    }



    /*
    if(SEQ.m_bNotLotOpen){
        FM_MsgOk("WARNING" , "Please Lot Open");
        SEQ.m_bNotLotOpen = false;

    }*/

    lbLDRLotNo -> Caption = DM.ARAY[riLDR].GetLotNo();//+"_"+DM.ARAY[riLDR].GetID().ToIntDef(0)/100;
    lbPRBLotNo -> Caption = DM.ARAY[riPRB].GetLotNo();//+"_"+DM.ARAY[riPRB].GetID().ToIntDef(0)/100;
    lbLSRLotNo -> Caption = DM.ARAY[riLSR].GetLotNo();//+"_"+DM.ARAY[riLSR].GetID().ToIntDef(0)/100;
    lbVSNLotNo -> Caption = DM.ARAY[riVSN].GetLotNo();//+"_"+DM.ARAY[riVSN].GetID().ToIntDef(0)/100;
    lbPSBLotNo -> Caption = DM.ARAY[riPSB].GetLotNo();//+"_"+DM.ARAY[riPSB].GetID().ToIntDef(0)/100;
    lbULDLotNo -> Caption = DM.ARAY[riULD].GetLotNo();//+"_"+DM.ARAY[riULD].GetID().ToIntDef(0)/100;

    lbMgnCntLDR -> Caption = DM.ARAY[riLDR].GetID().ToIntDef(0)/100;
    lbMgnCntPRB -> Caption = DM.ARAY[riPRB].GetID().ToIntDef(0)/100;
    lbMgnCntLSR -> Caption = DM.ARAY[riLSR].GetID().ToIntDef(0)/100;
    lbMgnCntVSN -> Caption = DM.ARAY[riVSN].GetID().ToIntDef(0)/100;
    lbMgnCntPSB -> Caption = DM.ARAY[riPSB].GetID().ToIntDef(0)/100;
    lbMgnCntULD -> Caption = DM.ARAY[riULD].GetID().ToIntDef(0)/100;

    //lbLaserDataPRB -> Caption =
    //lbLaserDataLSR -> Caption =
    //lbLaserDataVSN -> Caption =
    //lbLaserDataPSB -> Caption =





    if(SEQ._bRun){
        btPrbClear    -> Enabled = false;
        btLsrClear    -> Enabled = false;
        btVsnClear    -> Enabled = false;
        btPsbClear    -> Enabled = false;
        btPrbInput    -> Enabled = false;
        btPrbMask     -> Enabled = false;
        btVSNMakeGood -> Enabled = false;
        //btLotOpen     -> Enabled = false;
    }
    else{
        btPrbClear    -> Enabled = true;
        btLsrClear    -> Enabled = true;
        btVsnClear    -> Enabled = true;
        btPsbClear    -> Enabled = true;
        btPrbInput    -> Enabled = true;
        btPrbMask     -> Enabled = true;
        btVSNMakeGood -> Enabled = true;
        //btLotOpen     -> Enabled = true;
    }



    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    FrmLotOpen -> ShowModal() ;
    //if((IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) && DM.ARAY[riULD].CheckAllStat(csNone)){
    //    DM.ARAY[riULD].SetStat(csEmpty);
    //}
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    bool LDRMgzDetect ;
    bool ULDMgzDetect ;

    LDRMgzDetect =  IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    ULDMgzDetect = (IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) && !DM.ARAY[riULD].CheckAllStat(csEmpty);

    if(LDRMgzDetect && ULDMgzDetect) MM.SetManCycle(mcETC_LDRULDPlace);
    else if( LDRMgzDetect && !ULDMgzDetect) MM.SetManCycle(mcLDR_Place);
    else if(!LDRMgzDetect &&  ULDMgzDetect) MM.SetManCycle(mcULD_Place);

    //kyekye 얼로더 메거진 체인지 더 일찍 하기 랏바뀌었을때.

    LDR._iMgzCnt = OM.DevOptn.iLotEndMgzCnt ;  //kyekye
    LTQ.LotEnd ();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

//    switch(iIndex) {
//        case 0 :                                 break  ;
//        case 1 :                                 break  ;
//        case 2 : if(FM_GetLevel() == lvOperator) return ;
//        case 3 : if(FM_GetLevel() == lvOperator) return ;
//        case 4 : if(FM_GetLevel() == lvOperator) return ;
//        case 5 : if(FM_GetLevel() == lvOperator) return ;
//        case 6 : if(FM_GetLevel() == lvOperator) return ;
//        case 7 :                                 break  ;
//        case 10: if(FM_GetLevel() == lvOperator) return ;
//        case 11: if(FM_GetLevel() == lvOperator) return ;
//    }
//
    if(iIndex != 5 && FM_MsgYesNo("Confirm", "Would you like to invert the Option?") != mrYes) return ;
    else if(iIndex == 5){
        if(FM_MsgYesNo("Confirm", "Vision skip, data is saved to zero") != mrYes) return ;
    }

    switch(iIndex) {
        case 0 :                                                           break ;
        case 1 : OM.CmnOptn.bIgnrDoor      = !OM.CmnOptn.bIgnrDoor       ; break ;
        case 2 : OM.CmnOptn.bLoadingStop   = !OM.CmnOptn.bLoadingStop    ; break ;
        case 3 : OM.CmnOptn.bSkipLaser     = !OM.CmnOptn.bSkipLaser      ; break ;
        case 4 : OM.CmnOptn.bSkipOriVisn   = !OM.CmnOptn.bSkipOriVisn    ; break ;
        case 5 : OM.CmnOptn.bSkipAlnVisn   = !OM.CmnOptn.bSkipAlnVisn    ; break ;

        case 6 : if(OM.CmnOptn.iMarkInsp==2) { OM.CmnOptn.iMarkInsp  = 0;}
                 else                        { OM.CmnOptn.iMarkInsp += 1;} break ;

    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPR1LotInputClick(TObject *Sender)
{
    /*
    if(SEQ._bRun) return ;


    int iTag = ((TButton*)Sender) -> Tag ;
    AnsiString sLotNo ;
    int iMgzNo , iSlotNo ;

    switch (iTag)
    {
        default :
            break ;

        case 1 :
            sLotNo  = edPRBLotNo ->Text ;
            iMgzNo  = edPRBMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPRBSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPRB].SetLotNo(sLotNo) ;
            DM.ARAY[riPRB].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 2 :
            sLotNo  = edInspLotNo ->Text ;
            iMgzNo  = edInspMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edInspSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riWRK].SetLotNo(sLotNo) ;
            DM.ARAY[riWRK].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 3 :
            sLotNo  = edPSBLotNo ->Text ;
            iMgzNo  = edPSBMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPSBSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPSB].SetLotNo(sLotNo) ;
            DM.ARAY[riPSB].SetID(iMgzNo *100 + iSlotNo);
            break ;
     }

     */

}
//---------------------------------------------------------------------------

bool __fastcall TFrmOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
/*
    if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect) && !OM.MstOptn.bDryRun ) {
        if(FM_MsgYesNo("Err " , "[WorkZone Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riWRK ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if(DM.ARAY[riPSB].GetCntExist() && !OM.MstOptn.bDryRun) {
        if((!IO_GetX(xPSB_LDetect) && !IO_GetX(xPSB_RDetect) && !IO_GetX(xPSB_Pkg1) && !IO_GetX(xPSB_Pkg2) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) ) {
            if(FM_MsgYesNo("Err " , "[PostBuffer2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPSB].SetStat(csNone); SEQ._bBtnStart = true ;}
        }
    }
    else if ( !DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) ) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Loader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riLDR ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if ( !DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[UnLoader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
*/
    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripUnknwon()
{
//    //스트립 사라짐 에러.
//    if (DM.ARAY[riPR2].CheckAllStat(csNone)&& DM.ARAY[riPR1].CheckAllStat(csNone) && IO_GetX(xPRB_3Pkg) ) {
//        if(FM_MsgYesNo("Err " , "[PreBuffer2 Strip Inserted] \nDo you want to Set Strip Data?"     ) == mrYes ) {
//            DM.ARAY[riPR2].SetStat(csUnkwn);
//            if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR2].SetLotNo(PRB.GetWorkedLotNo()     ) ;
//            else                                                                            DM.ARAY[riPR2].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//            SEQ._bBtnStart = true ;
//        }
//    }
//
    return true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPR1InputClick(TObject *Sender)
{
//    int iTag = ((TButton*)Sender) -> Tag ;
//
//    switch (iTag)
//    {
//        default :
//            break ;
//
//        case 1 : DM.ARAY[riPR1].SetStat(csUnkwn);
//                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
//                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//                 break;
//
//        case 2 : DM.ARAY[riPR1].SetStat(csUnkwn);
//                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
//                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//                 break;
//    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    SPC.DAY.ClearData() ;

}
//---------------------------------------------------------------------------









void __fastcall TFrmOperation::btPrbClearClick(TObject *Sender)
{

    int iTag = ((TButton*)Sender) -> Tag ;

    DM.ARAY[iTag].ClearMap();

/*
    iTag += 1;

    AnsiString sLotNo , sSlotNo ,sMsg;
    switch (iTag)
    {
        default :
            break ;

        case 1 : DM.ARAY[riPRB].SetStat(csNone);
                 break;

        case 2 : DM.ARAY[riLSR].SetStat(csNone);
                 break;

        case 3 : DM.ARAY[riVSN].SetStat(csNone);
                 break ;

        case 4 : DM.ARAY[riPSB].SetStat(csNone);
                 break;

        case 5 : DM.ARAY[riPRB].SetStat(csMask);
                 break;

        case 6 : DM.ARAY[riLSR].SetStat(csMask);
                 break;

        case 7 : DM.ARAY[riVSN].SetStat(csWork);
                 break ;

        case 8 : DM.ARAY[riPSB].SetStat(csWork);
                 break;
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
    DM.ShiftArrayData(riPRB , riLSR);

    //MT_ClearPos(miULD_ZClmp);
    //MT_GoIncMan(miULD_ZClmp,-10); //12mm
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPrbInputClick(TObject *Sender)
{
    //if(SEQ._bRun) return ;
    //
    //int iTag = ((TButton*)Sender) -> Tag ;
    //
    //if(DM.ARAY[riLDR].GetCntStat(csEmpty)==0) {
    //    FM_MsgOk("ERROR","로더에 메거진이 없어 랏넘버를 알수 없습니다.");
    //    return ;
    //}
    //
    //
    //
    //AnsiString sTemp = "스트립 랏넘버가 " + DM.ARAY[riLDR].GetLotNo() + "가 맞습니까?";
    //
    //if(FM_MsgYesNo("Check",sTemp.c_str() ) != mrYes) {
    //    return ;
    //}
    //
    //DM.ARAY[iTag].SetStat(csUnkwn);
    //int iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindFrstRow(csEmpty)-1;
    //DM.ARAY[iTag].SetID((DM.ARAY[riLDR].GetID().ToIntDef(0) / 100 * 100) + iSltCnt);
    //DM.ARAY[iTag].SetLotNo(DM.ARAY[riLDR].GetLotNo());

    DM.ARAY[riLSR].SetStat(csMask);
    //DM.ARAY[riLSR].SetLotNo(Edit3 -> Text);

}
//---------------------------------------------------------------------------



void __fastcall TFrmOperation::Panel15DblClick(TObject *Sender)
{
    int r,c;
    DM.ARAY[riVSN].FindFrstRowCol(csWait , r , c) ;

    Edit1 -> Text = r;
    Edit2 -> Text = c;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btVSNMakeGoodClick(TObject *Sender)
{

    DM.ARAY[riVSN].ChangeStat(csRslt1 , csRslt0);
    DM.ARAY[riVSN].ChangeStat(csRslt2 , csRslt0);
    DM.ARAY[riVSN].ChangeStat(csRslt3 , csRslt0);
    DM.ARAY[riVSN].ChangeStat(csRslt4 , csRslt0);
    DM.ARAY[riVSN].ChangeStat(csRslt5 , csRslt0);
    DM.ARAY[riVSN].ChangeStat(csRslt6 , csRslt0);




}


//---------------------------------------------------------------------------

void __fastcall TFrmOperation::cbManMarkClick(TObject *Sender)
{
    LSR.m_bManMark = cbManMark -> Checked ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSetManMarkClick(TObject *Sender)
{
    LSR.m_sManMark = edManMark -> Text.Trim();        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button2Click(TObject *Sender)
{
    int r, c ;
    double dTemp , dColGrPtchGap;
    VSN.FindChip(riVSN , r, c);

    dColGrPtchGap = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch;
    int iGrCnt = OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt;
    for(int i = 0; i < OM.DevInfo.iColCnt; i++){
        dTemp = OM.DevInfo.dColPitch * i + (OM.DevInfo.iColGrCnt ? dColGrPtchGap * (i / iGrCnt) : 0);
        DM.ARAY[riVSN].SetStat(r, i, csMask);
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPrbMaskClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

    int iTag = ((TButton*)Sender) -> Tag ;

    DM.ARAY[iTag].SetStat(csMask);
}
//---------------------------------------------------------------------------




void __fastcall TFrmOperation::btAllHomeClick(TObject *Sender)
{
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btWorkEndClick(TObject *Sender)
{
    //메거진 카운트 안맞았을때 빈메거진 넣으면 되는데 빈메거진 안넣게 해달라고 해서
    //이렇게 작업함.
    //메거진 카운트를 잘못 하여 4로 세팅을 했는데 3개만 넣고 그냥 강제 클리어 할때
    if(FM_MsgYesNo("Confirm","랏 대기리스트와 카운트가 클리어 하겠습니까?") != mrYes) return ;


    LTQ.DeleteLotNoAll();
    LDR.SetMgzCnt(OM.DevOptn.iLotEndMgzCnt)  ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::cbWorkRepeatClick(TObject *Sender)
{
    MM.m_TempWorkRepeat =  cbWorkRepeat -> Checked ;
}
//---------------------------------------------------------------------------


