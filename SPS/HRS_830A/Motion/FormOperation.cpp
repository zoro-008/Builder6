//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PostBuffer.h"
#include "Work.h"
#include "Laser.h"
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

//#include "SAInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    DM.ARAY[riLDR].SetParent(pnLDR ); DM.ARAY[riLDR].SetConfig(false , "LDR"); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPRB ); DM.ARAY[riPRB].SetConfig(false , "PRB"); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riLWK].SetParent(pnLWK ); DM.ARAY[riLWK].SetConfig(false , "LWK"); DM.ARAY[riLWK].ClearDispItem();
    DM.ARAY[riRWK].SetParent(pnRWK ); DM.ARAY[riRWK].SetConfig(false , "RWK"); DM.ARAY[riRWK].ClearDispItem();
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
    DM.ARAY[riPRB].SetDispItem(csUnkwn  ,"UnKnown"  ) ;

    DM.ARAY[riLWK].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riLWK].SetDispItem(csUnkwn  ,"Unknown"  ) ;
    DM.ARAY[riLWK].SetDispItem(csWork   ,"Work   "  ) ;

    DM.ARAY[riRWK].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riRWK].SetDispItem(csWork   ,"Work   "  ) ;

    DM.ARAY[riPSB].SetDispItem(csNone   ,"NoStrip"  ) ;
    DM.ARAY[riPSB].SetDispItem(csWork   ,"Work   "  ) ;

    DM.ARAY[riULD].SetDispItem(csNone   ,"NoMgz  " ) ;
    DM.ARAY[riULD].SetDispItem(csEmpty  ,"Empty  " ) ;
    DM.ARAY[riULD].SetDispItem(csWork   ,"Exist  " ) ;

    DM.LoadMap();

    const bool bKorean = true;


    //주현이가 해야할 일...JS
    //Manual Button Caption
    btMan1_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan1_14 -> Tag = (int)mcAllHome            ; btMan1_14 -> Visible = true  ;
    btMan2_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan2_14 -> Tag = (int)mcAllHome            ; btMan2_14 -> Visible = true  ;
    btMan3_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan3_14 -> Tag = (int)mcAllHome            ; btMan3_14 -> Visible = true  ;
    btMan4_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan4_14 -> Tag = (int)mcAllHome            ; btMan4_14 -> Visible = true  ;
    btMan5_14 -> Caption = "[" + AnsiString(mcAllHome         ) + "]" + (bKorean ? "전체 홈" : "ALL HOME");  btMan5_14 -> Tag = (int)mcAllHome            ; btMan5_14 -> Visible = true  ;

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
    btMan1_11 -> Caption = "[" + AnsiString(mcLDR_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"       : "Feed Out On/Off"   );  btMan1_11 -> Tag = (int)mcLDR_TranOutAC    ; btMan1_11 -> Visible = false ;
    btMan1_12 -> Caption = "[" + AnsiString(mcLDR_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"       : "Feed Out On/Off"   );  btMan1_12 -> Tag = (int)mcLDR_TranOutAC    ; btMan1_12 -> Visible = false ;
    btMan1_13 -> Caption = "[" + AnsiString(mcLDR_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"       : "Feed Out On/Off"   );  btMan1_13 -> Tag = (int)mcLDR_TranOutAC    ; btMan1_13 -> Visible = false ;

    //Prebuffer

    btMan2_1  -> Caption = "[" + AnsiString(mcPRB_CycleIn     ) + "]" + (bKorean ? "싸이클 인"         : "PRB Cycle In"      );  btMan2_1  -> Tag = (int)mcPRB_CycleIn      ; btMan2_1  -> Visible = true  ;
    btMan2_2  -> Caption = "[" + AnsiString(mcPRB_CycleOut    ) + "]" + (bKorean ? "싸이클 아웃"       : "PRB Cycle Out"     );  btMan2_2  -> Tag = (int)mcPRB_CycleOut     ; btMan2_2  -> Visible = true  ;
    btMan2_3  -> Caption = "[" + AnsiString(mcPRB_AirBlow     ) + "]" + (bKorean ? "에어 블로우"       : "PRB AirBlow"       );  btMan2_3  -> Tag = (int)mcPRB_AirBlow      ; btMan2_3  -> Visible = true  ;
    btMan2_4  -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_4  -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_4  -> Visible = true  ;
    btMan2_5  -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_5  -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_5  -> Visible = false ;
    btMan2_6  -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_6  -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_6  -> Visible = false ;
    btMan2_7  -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_7  -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_7  -> Visible = false ;
    btMan2_8  -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_8  -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_8  -> Visible = false ;
    btMan2_9  -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_9  -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_9  -> Visible = false ;
    btMan2_10 -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_10 -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_10 -> Visible = false ;
    btMan2_11 -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_11 -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_11 -> Visible = false ;
    btMan2_12 -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "싸이클 이동"       : "PRB Cycle Move"    );  btMan2_12 -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_12 -> Visible = false ;
    btMan2_13 -> Caption = "[" + AnsiString(mcPRB_IdxUpDn     ) + "]" + (bKorean ? "인덱스 업/다운"    : "PRB Index Up/Dn"   );  btMan2_13 -> Tag = (int)mcPRB_IdxUpDn      ; btMan2_13 -> Visible = false ;

    //Work
    btMan3_1  -> Caption = "[" + AnsiString(mcWRK_CycleIn     ) + "]" + (bKorean ? "싸이클 인"           : "Cycle In"          );  btMan3_1  -> Tag = (int)mcWRK_CycleIn      ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcWRK_CycleOut    ) + "]" + (bKorean ? "싸이클 아웃"         : "Cycle Out"         );  btMan3_2  -> Tag = (int)mcWRK_CycleOut     ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcWRK_CycleWork   ) + "]" + (bKorean ? "싸이클 워크"         : "Cycle Work"        );  btMan3_3  -> Tag = (int)mcWRK_CycleWork    ; btMan3_3  -> Visible = true  ;
    btMan3_4  -> Caption = "[" + AnsiString(mcWRK_LIdxUpDn    ) + "]" + (bKorean ? "좌측 인덱스 업/다운" : "Left Index Up/Dn"  );  btMan3_4  -> Tag = (int)mcWRK_LIdxUpDn     ; btMan3_4  -> Visible = true  ;
    btMan3_5  -> Caption = "[" + AnsiString(mcWRK_RIdxUpDn    ) + "]" + (bKorean ? "우측 인덱스 업/다운" : "Right Index Up/Dn" );  btMan3_5  -> Tag = (int)mcWRK_RIdxUpDn     ; btMan3_5  -> Visible = true  ;
    btMan3_6  -> Caption = "[" + AnsiString(mcWRK_DspsWait    ) + "]" + (bKorean ? "디스펜서 대기"       : "Dispensor Wait"    );  btMan3_6  -> Tag = (int)mcWRK_DspsWait     ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_7  -> Tag = (int)mcWRK_DspsWork     ; btMan3_7  -> Visible = true  ;
    btMan3_8  -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_8  -> Tag = (int)mcWRK_DspsWork     ; btMan3_8  -> Visible = false ;
    btMan3_9  -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_9  -> Tag = (int)mcWRK_DspsWork     ; btMan3_9  -> Visible = false ;
    btMan3_10 -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_10 -> Tag = (int)mcWRK_DspsWork     ; btMan3_10 -> Visible = false ;
    btMan3_11 -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_11 -> Tag = (int)mcWRK_DspsWork     ; btMan3_11 -> Visible = false ;
    btMan3_12 -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_12 -> Tag = (int)mcWRK_DspsWork     ; btMan3_12 -> Visible = false ;
    btMan3_13 -> Caption = "[" + AnsiString(mcWRK_DspsWork    ) + "]" + (bKorean ? "디스펜서 워크"       : "Dispensor Work"    );  btMan3_13 -> Tag = (int)mcWRK_DspsWork     ; btMan3_13 -> Visible = false ;

    //Post
    btMan4_1  -> Caption = "[" + AnsiString(mcPSB_CycleOut    ) + "]" + (bKorean ? "싸이클 아웃"         : "Cycle Out"         );  btMan4_1  -> Tag = (int)mcPSB_CycleOut     ; btMan4_1  -> Visible = true  ;
    btMan4_2  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_2  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_3  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_3  -> Visible = false ;
    btMan4_4  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_4  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_4  -> Visible = false ;
    btMan4_5  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_5  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_5  -> Visible = false ;
    btMan4_6  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_6  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_6  -> Visible = false ;
    btMan4_7  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_7  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_7  -> Visible = false ;
    btMan4_8  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_8  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_8  -> Visible = false ;
    btMan4_9  -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_9  -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_9  -> Visible = false ;
    btMan4_10 -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_10 -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_10 -> Visible = false ;
    btMan4_11 -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_11 -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_11 -> Visible = false ;
    btMan4_12 -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_12 -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_12 -> Visible = false ;
    btMan4_13 -> Caption = "[" + AnsiString(mcPSB_IndxDnUp    ) + "]" + (bKorean ? "인덱스 다운/업"      : "Index Dn/Up"       );  btMan4_13 -> Tag = (int)mcPSB_IndxDnUp     ; btMan4_13 -> Visible = false ;

    //Unloader
    btMan5_1  -> Caption = "[" + AnsiString(mcULD_Supply      ) + "]" + (bKorean ? "매거진 공급"         : "UnLoading Supply"  );  btMan5_1  -> Tag = (int)mcULD_Supply       ; btMan5_1  -> Visible = true  ;
    btMan5_2  -> Caption = "[" + AnsiString(mcULD_Pick        ) + "]" + (bKorean ? "매거진 집기"         : "UnLoading Pick"    );  btMan5_2  -> Tag = (int)mcULD_Pick         ; btMan5_2  -> Visible = true  ;
    btMan5_3  -> Caption = "[" + AnsiString(mcULD_SttPstn     ) + "]" + (bKorean ? "언로더 시작 위치"    : "Move Start Pos"    );  btMan5_3  -> Tag = (int)mcULD_SttPstn      ; btMan5_3  -> Visible = true  ;
    btMan5_4  -> Caption = "[" + AnsiString(mcULD_Place       ) + "]" + (bKorean ? "매거진 배출"         : "UnLoading Place"   );  btMan5_4  -> Tag = (int)mcULD_Place        ; btMan5_4  -> Visible = true  ;
    btMan5_5  -> Caption = "[" + AnsiString(mcULD_UpSlPitch   ) + "]" + (bKorean ? "언로더 피치 업"      : "UnLoader Pitch Up" );  btMan5_5  -> Tag = (int)mcULD_UpSlPitch    ; btMan5_5  -> Visible = true  ;
    btMan5_6  -> Caption = "[" + AnsiString(mcULD_DnSlPitch   ) + "]" + (bKorean ? "언로더 피치 다운"    : "UnLoader Pitch Dn" );  btMan5_6  -> Tag = (int)mcULD_DnSlPitch    ; btMan5_6  -> Visible = true  ;
    btMan5_7  -> Caption = "[" + AnsiString(mcULD_ClampDnUp   ) + "]" + (bKorean ? "로더 클램프"         : "Clamp Dn/Up"       );  btMan5_7  -> Tag = (int)mcULD_ClampDnUp    ; btMan5_7  -> Visible = true  ;
    btMan5_8  -> Caption = "[" + AnsiString(mcPSB_PusherFwBw  ) + "]" + (bKorean ? "포스트버퍼 푸셔"     : "Pusher Fw/Bw"      );  btMan5_8  -> Tag = (int)mcPSB_PusherFwBw   ; btMan5_8  -> Visible = true  ;
    btMan5_9  -> Caption = "[" + AnsiString(mcULD_TranInAC    ) + "]" + (bKorean ? "진입 AC모터"         : "Feeding In On/Off" );  btMan5_9  -> Tag = (int)mcULD_TranInAC     ; btMan5_9  -> Visible = true  ;
    btMan5_10 -> Caption = "[" + AnsiString(mcULD_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"         : "Feeding Out On/Off");  btMan5_10 -> Tag = (int)mcULD_TranOutAC    ; btMan5_10 -> Visible = true  ;
    btMan5_11 -> Caption = "[" + AnsiString(mcULD_TranOutAC   ) + "]" + (bKorean ? "마스킹 카운터"       : "Feeding Out On/Off");  btMan5_11 -> Tag = (int)mcULD_TranOutAC    ; btMan5_11 -> Visible = false ;
    btMan5_12 -> Caption = "[" + AnsiString(mcULD_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"         : "Feeding Out On/Off");  btMan5_12 -> Tag = (int)mcULD_TranOutAC    ; btMan5_12 -> Visible = false ;
    btMan5_13 -> Caption = "[" + AnsiString(mcULD_TranOutAC   ) + "]" + (bKorean ? "배출 AC모터"         : "Feeding Out On/Off");  btMan5_13 -> Tag = (int)mcULD_TranOutAC    ; btMan5_13 -> Visible = false ;


    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;

    btPRBClear -> Tag = riPRB ;
    btLWKClear -> Tag = riLWK ;
    btRWKClear -> Tag = riRWK ;
    btPSBClear -> Tag = riPSB ;
    btPRBInput -> Tag = riPRB ;

    //SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)
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

    Label19 -> Caption = "ID: " +DM.ARAY[riLDR].GetID() + " MgzCnt: " + LDR._iMgzCnt ;
    Label20 -> Caption = "ID: " +DM.ARAY[riULD].GetID() ;
    Label1  -> Caption = WRK._iDisprCnt;

    pnIgnrDoor       -> Color  =  OM.CmnOptn.bIgnrDoor        ? clLime : clRed ;
    pnLoadingStop    -> Color  =  OM.CmnOptn.bLoadingStop     ? clLime : clRed ;
    pnIgnrDisps      -> Color  =  OM.CmnOptn.bIgnrDisps       ? clLime : clRed ;
    pnSkipDisprSnsr  -> Color  =  OM.CmnOptn.bIgnrDsprSnsr    ? clLime : clRed ;
    pnSkipDisprExpr  -> Color  =  OM.CmnOptn.bIgnrDsprExpr    ? clLime : clRed ;


    pnIgnrDoor       -> Caption  =  OM.CmnOptn.bIgnrDoor      ? "ON"   : "OFF" ;
    pnLoadingStop    -> Caption  =  OM.CmnOptn.bLoadingStop   ? "ON"   : "OFF" ;
    pnIgnrDisps      -> Caption  =  OM.CmnOptn.bIgnrDisps     ? "ON"   : "OFF" ;
    pnSkipDisprSnsr  -> Caption  =  OM.CmnOptn.bIgnrDsprSnsr  ? "ON"   : "OFF" ;
    pnSkipDisprExpr  -> Caption  =  OM.CmnOptn.bIgnrDsprExpr  ? "ON"   : "OFF" ;

    lbLDRLotNo -> Caption = DM.ARAY[riLDR].GetLotNo()+"_"+DM.ARAY[riLDR].GetID().ToIntDef(0)/100;
    lbPRBLotNo -> Caption = DM.ARAY[riPRB].GetLotNo()+"_"+DM.ARAY[riPRB].GetID().ToIntDef(0)/100;
    lbLWKLotNo -> Caption = DM.ARAY[riLWK].GetLotNo()+"_"+DM.ARAY[riLWK].GetID().ToIntDef(0)/100;
    lbRWKLotNo -> Caption = DM.ARAY[riRWK].GetLotNo()+"_"+DM.ARAY[riRWK].GetID().ToIntDef(0)/100;
    lbPSBLotNo -> Caption = DM.ARAY[riPSB].GetLotNo()+"_"+DM.ARAY[riPSB].GetID().ToIntDef(0)/100;
    lbULDLotNo -> Caption = DM.ARAY[riULD].GetLotNo()+"_"+DM.ARAY[riULD].GetID().ToIntDef(0)/100;

    

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }


    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

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

    bool bAllArayNone = DM.ARAY[riLDR].CheckAllStat(csNone) && DM.ARAY[riPRB].CheckAllStat(csNone) &&
                        DM.ARAY[riLWK].CheckAllStat(csNone) && DM.ARAY[riRWK].CheckAllStat(csNone) &&
                        DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) ;


    if(!LTQ.GetLotOpen()){
        btLotEnd -> Enabled = false;  // 랏큐에 랏이 없고 모든 어래이가 None 일 경우 버튼 인에이블...
        btStart  -> Enabled = false;
    }
    else{
        btLotEnd -> Enabled = true;
        btStart  -> Enabled = true;
    }

    if(SEQ._bRun){
        btPRBClear    -> Enabled = false;
        btLWKClear    -> Enabled = false;
        btRWKClear    -> Enabled = false;
        btPSBClear    -> Enabled = false;
        btPRBInput    -> Enabled = false;
    }
    else{
        btPRBClear    -> Enabled = true;
        btLWKClear    -> Enabled = true;
        btRWKClear    -> Enabled = true;
        btPSBClear    -> Enabled = true;
        btPRBInput    -> Enabled = true;
    }
    //Liquid 액체 공급 일자.
    TDateTime LQDay , TempDay;
    LQDay.Val = OM.CmnOptn.LQSupplyDay.Val;
    int iTemp = Now().Val - LQDay.Val;
    edLQSupplyDay -> Text = iTemp;
    edSettingDay  -> Text = OM.CmnOptn.iExpiryDay;

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    FrmLotOpen -> ShowModal() ;
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

    /*
    bool LDRMgzDetect ;
    bool ULDMgzDetect ;

    LDRMgzDetect = IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    ULDMgzDetect = IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);

    if(LDRMgzDetect && ULDMgzDetect) MM.SetManCycle(mcETC_LDRULDPlace);
    else if( LDRMgzDetect && !ULDMgzDetect) MM.SetManCycle(mcLDR_Place);
    else if(!LDRMgzDetect &&  ULDMgzDetect) MM.SetManCycle(mcULD_Place);
    */

    //kyekye 얼로더 메거진 체인지 더 일찍 하기 랏바뀌었을때.


    LDR._iMgzCnt = OM.DevOptn.iLotEndMgzCnt ;  //kyekye


    MM.SetManCycle(mcETC_LDRULDPlace);

    LTQ.LotEnd ();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

    if (FM_MsgYesNo("Confirm", "Would you like to invert the Option?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                           break ;
        case 1 : OM.CmnOptn.bIgnrDoor      = !OM.CmnOptn.bIgnrDoor       ; break ;
        case 2 : OM.CmnOptn.bLoadingStop   = !OM.CmnOptn.bLoadingStop    ; break ;
        case 3 : OM.CmnOptn.bIgnrDisps     = !OM.CmnOptn.bIgnrDisps      ; break ;
        case 4 : OM.CmnOptn.bIgnrDsprSnsr  = !OM.CmnOptn.bIgnrDsprSnsr   ; break ;
        case 5 : OM.CmnOptn.bIgnrDsprExpr  = !OM.CmnOptn.bIgnrDsprExpr   ; break ;
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

void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    SPC.DAY.ClearData() ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPRBClearClick(TObject *Sender)
{

    int iTag = ((TButton*)Sender) -> Tag ;

    DM.ARAY[iTag].ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPRBInputClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

    int iTag = ((TButton*)Sender) -> Tag ;

    DM.ARAY[iTag].SetStat(csUnkwn);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btDayResetClick(TObject *Sender)
{
    OM.CmnOptn.LQSupplyDay.Val = Now();
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::tmLabelTimerTimer(TObject *Sender)
{
    tmLabelTimer -> Enabled = false;

    tmLabelTimer -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btULDMaskClick(TObject *Sender)
{
    int r, c = 0;

    r = DM.ARAY[riULD].FindFrstRow(csEmpty) ;

    DM.ARAY[riULD].SetStat(r, c, csWork);


}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmStopToShotTimer(TObject *Sender)
{
    tmStopToShot -> Enabled = false;

    if(IO_GetX(xETC_FDoor) && IO_GetX(xETC_BDoor)){
        if(SEQ._iSeqStat == ssStop && OM.CmnOptn.iStopToShotTime != 0 && SEQ.m_tmStopTime.OnDelay(true , OM.CmnOptn.iStopToShotTime * 60000)){
            MM.SetManCycle(mcStopTODisprShot);
            SEQ.m_tmStopTime.Clear();
        }
    }

    tmStopToShot -> Enabled = true;

}
//---------------------------------------------------------------------------

