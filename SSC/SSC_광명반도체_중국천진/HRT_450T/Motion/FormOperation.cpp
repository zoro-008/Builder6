//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff1.h"
#include "WorkZone.h"
#include "SortingTool.h"
#include "PostBuffer.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"

#include "FormProbe.h"
#include "ProbeComUnit.h"
#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "TempConUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{


    //Manual Button Caption
    btMan1_1  -> Caption = "[" + AnsiString(mcLDR_Home          ) + "]" + "Loading home"     ;  btMan1_1  -> Tag = (int)mcLDR_Home           ; btMan1_1  -> Visible = true  ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_Supply        ) + "]" + "Loading Supply"   ;  btMan1_2  -> Tag = (int)mcLDR_Supply         ; btMan1_2  -> Visible = true  ;
    btMan1_3  -> Caption = "[" + AnsiString(mcLDR_Pick          ) + "]" + "Loading Pick"     ;  btMan1_3  -> Tag = (int)mcLDR_Pick           ; btMan1_3  -> Visible = true  ;
    btMan1_4  -> Caption = "[" + AnsiString(mcLDR_SttPstn       ) + "]" + "Move Start Pos"   ;  btMan1_4  -> Tag = (int)mcLDR_SttPstn        ; btMan1_4  -> Visible = true  ;
    btMan1_5  -> Caption = "[" + AnsiString(mcLDR_Place         ) + "]" + "Loading Place"    ;  btMan1_5  -> Tag = (int)mcLDR_Place          ; btMan1_5  -> Visible = true  ;
    btMan1_6  -> Caption = "[" + AnsiString(mcLDR_UpSlPitch     ) + "]" + "Pitch Up"         ;  btMan1_6  -> Tag = (int)mcLDR_UpSlPitch      ; btMan1_6  -> Visible = true  ;
    btMan1_7  -> Caption = "[" + AnsiString(mcLDR_DnSlPitch     ) + "]" + "Pitch Dn"         ;  btMan1_7  -> Tag = (int)mcLDR_DnSlPitch      ; btMan1_7  -> Visible = true  ;
    btMan1_8  -> Caption = "[" + AnsiString(mcLDR_Pusher        ) + "]" + "Pusher F/B"       ;  btMan1_8  -> Tag = (int)mcLDR_Pusher         ; btMan1_8  -> Visible = true  ;
    btMan1_9  -> Caption = "[" + AnsiString(mcLDR_Clamp         ) + "]" + "LDR Clamp U/D"    ;  btMan1_9  -> Tag = (int)mcLDR_Clamp          ; btMan1_9  -> Visible = true  ;
    btMan1_10 -> Caption = "[" + AnsiString(mcLDR_TranInAC      ) + "]" + "Feeding In"       ;  btMan1_10 -> Tag = (int)mcLDR_TranInAC       ; btMan1_10 -> Visible = true  ;
    btMan1_11 -> Caption = "[" + AnsiString(mcLDR_TranOutAC     ) + "]" + "Feeding Out"      ;  btMan1_11 -> Tag = (int)mcLDR_TranOutAC      ; btMan1_11 -> Visible = true  ;
    btMan1_12 -> Caption = "[" + AnsiString(mcLDR_TranInAC      ) + "]" + "Feeding In"       ;  btMan1_12 -> Tag = (int)mcLDR_TranInAC       ; btMan1_12 -> Visible = false ;
    btMan1_13 -> Caption = "[" + AnsiString(mcLDR_TranOutAC     ) + "]" + "Feeding Out"      ;  btMan1_13 -> Tag = (int)mcLDR_TranOutAC      ; btMan1_13 -> Visible = false ;
    btMan1_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan1_14 -> Tag = (int)mcAllHome            ; btMan1_14 -> Visible = true  ;

    btMan2_1  -> Caption = "[" + AnsiString(mcPR1_Home          ) + "]" + "PreRail1 Home"    ;  btMan2_1  -> Tag = (int)mcPR1_Home           ; btMan2_1  -> Visible = true  ;
    btMan2_2  -> Caption = "[" + AnsiString(mcPRE_IDCyl         ) + "]" + "ID Cyl U/D"       ;  btMan2_2  -> Tag = (int)mcPRE_IDCyl          ; btMan2_2  -> Visible = true  ;
    btMan2_3  -> Caption = "[" + AnsiString(mcPR1_In            ) + "]" + "PreRail1 In"      ;  btMan2_3  -> Tag = (int)mcPR1_In             ; btMan2_3  -> Visible = false ;
    btMan2_4  -> Caption = "[" + AnsiString(mcPR1_Drag          ) + "]" + "PreRail1 Drag"    ;  btMan2_4  -> Tag = (int)mcPR1_Drag           ; btMan2_4  -> Visible = false ;
    btMan2_5  -> Caption = "[" + AnsiString(mcPR1_Out           ) + "]" + "PreRail1 Out"     ;  btMan2_5  -> Tag = (int)mcPR1_Out            ; btMan2_5  -> Visible = false ;
    btMan2_6  -> Caption = "[" + AnsiString(mcPR1_Clamp         ) + "]" + "PRE1 Clamp U/D"   ;  btMan2_6  -> Tag = (int)mcPR1_Clamp          ; btMan2_6  -> Visible = true  ;
    btMan2_7  -> Caption = "[" + AnsiString(mcPR1_Feeding       ) + "]" + "Feeding On/Off"   ;  btMan2_7  -> Tag = (int)mcPR1_Feeding        ; btMan2_7  -> Visible = true  ;
    btMan2_8  -> Caption = "[" + AnsiString(mcPR2_Home          ) + "]" + "PreRail2 Home"    ;  btMan2_8  -> Tag = (int)mcPR2_Home           ; btMan2_8  -> Visible = true  ;
    btMan2_9  -> Caption = "[" + AnsiString(mcPR2_Clamp         ) + "]" + "PRE2 Clamp U/D"   ;  btMan2_9  -> Tag = (int)mcPR2_Clamp          ; btMan2_9  -> Visible = true  ;
    btMan2_10 -> Caption = "[" + AnsiString(mcPR2_Drag          ) + "]" + "PreRail2 Drag"    ;  btMan2_10 -> Tag = (int)mcPR2_Drag           ; btMan2_10 -> Visible = false ;
    btMan2_11 -> Caption = "[" + AnsiString(mcPR2_Out           ) + "]" + "PreRail2 Out"     ;  btMan2_11 -> Tag = (int)mcPR2_Out            ; btMan2_11 -> Visible = false ;
    btMan2_12 -> Caption = "[" + AnsiString(mcPR2_Clamp         ) + "]" + "PRE2 Clamp U/D"   ;  btMan2_12 -> Tag = (int)mcPR2_Clamp          ; btMan2_12 -> Visible = false ;
    btMan2_13 -> Caption = "[" + AnsiString(mcPR2_Drag          ) + "]" + "PreRail2 Drag"    ;  btMan2_13 -> Tag = (int)mcPR2_Drag           ; btMan2_13 -> Visible = false ;
    btMan2_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan2_14 -> Tag = (int)mcAllHome            ; btMan2_14 -> Visible = true  ;

    btMan3_1  -> Caption = "[" + AnsiString(mcWRK_Home         ) + "]" + "Probe Home"        ;  btMan3_1  -> Tag = (int)mcWRK_Home           ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcWRK_Work         ) + "]" + "Wrok"              ;  btMan3_2  -> Tag = (int)mcWRK_Work           ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcWRK_NextMove     ) + "]" + "Next Move"         ;  btMan3_3  -> Tag = (int)mcWRK_NextMove       ; btMan3_3  -> Visible = true  ;
    btMan3_4  -> Caption = "[" + AnsiString(mcWRK_Clamp        ) + "]" + "WRK Clamp U/D"     ;  btMan3_4  -> Tag = (int)mcWRK_Clamp          ; btMan3_4  -> Visible = true  ;
    btMan3_5  -> Caption = "[" + AnsiString(mcWRK_ToolChng     ) + "]" + "Tool Change "      ;  btMan3_5  -> Tag = (int)mcWRK_ToolChng       ; btMan3_5  -> Visible = true  ;
    btMan3_6  -> Caption = "[" + AnsiString(mcWRK_RptCntct     ) + "]" + "Repeat Contect"    ;  btMan3_6  -> Tag = (int)mcWRK_RptCntct       ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcWRK_ToolContect  ) + "]" + "Tool Contect"      ;  btMan3_7  -> Tag = (int)mcWRK_ToolContect    ; btMan3_7  -> Visible = true  ;
    btMan3_8  -> Caption = "[" + AnsiString(mcWRK_Home         ) + "]" + "Probe Home"        ;  btMan3_8  -> Tag = (int)mcWRK_Home           ; btMan3_8  -> Visible = false ;
    btMan3_9  -> Caption = "[" + AnsiString(mcWRK_Work         ) + "]" + "Wrok"              ;  btMan3_9  -> Tag = (int)mcWRK_Work           ; btMan3_9  -> Visible = false ;
    btMan3_10 -> Caption = "[" + AnsiString(mcWRK_NextMove     ) + "]" + "Next Move"         ;  btMan3_10 -> Tag = (int)mcWRK_NextMove       ; btMan3_10 -> Visible = false ;
    btMan3_11 -> Caption = "[" + AnsiString(mcWRK_Clamp        ) + "]" + "WRK Clamp U/D"     ;  btMan3_11 -> Tag = (int)mcWRK_Clamp          ; btMan3_11 -> Visible = false ;
    btMan3_12 -> Caption = "[" + AnsiString(mcWRK_ToolChng     ) + "]" + "Tool Change "      ;  btMan3_12 -> Tag = (int)mcWRK_ToolChng       ; btMan3_12 -> Visible = false ;
    btMan3_13 -> Caption = "[" + AnsiString(mcWRK_RptCntct     ) + "]" + "Repeat Contect"    ;  btMan3_13 -> Tag = (int)mcWRK_RptCntct       ; btMan3_13 -> Visible = false ;
    btMan3_14 -> Caption = "[" + AnsiString(mcAllHome          ) + "]" + "ALL HOME"          ;  btMan3_14 -> Tag = (int)mcAllHome            ; btMan3_14 -> Visible = true  ;

    btMan4_1  -> Caption = "[" + AnsiString(mcPSB_Home         ) + "]" + "PostRail Home"     ;  btMan4_1  -> Tag = (int)mcPSB_Home           ; btMan4_1  -> Visible = true  ;
    btMan4_2  -> Caption = "[" + AnsiString(mcPSB_WorkZoneCycle) + "]" + "Work Zone Cycle"   ;  btMan4_2  -> Tag = (int)mcPSB_WorkZoneCycle  ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcPSB_Drag         ) + "]" + "PostRail Drag"     ;  btMan4_3  -> Tag = (int)mcPSB_Drag           ; btMan4_3  -> Visible = true  ;
    btMan4_4  -> Caption = "[" + AnsiString(mcPSB_Out          ) + "]" + "PostRail Out"      ;  btMan4_4  -> Tag = (int)mcPSB_Out            ; btMan4_4  -> Visible = true  ;
    btMan4_5  -> Caption = "[" + AnsiString(mcPSB_Clamp        ) + "]" + "PostRail Clamp U/D";  btMan4_5  -> Tag = (int)mcPSB_Clamp          ; btMan4_5  -> Visible = true  ;
    btMan4_6  -> Caption = "[" + AnsiString(mcPSB_Feeding      ) + "]" + "Feeding On/Off"    ;  btMan4_6  -> Tag = (int)mcPSB_Feeding        ; btMan4_6  -> Visible = true  ;
    btMan4_7  -> Caption = "[" + AnsiString(mcPSB_R1Pitch      ) + "]" + "Go Right 1 Pitch"  ;  btMan4_7  -> Tag = (int)mcPSB_R1Pitch        ; btMan4_7  -> Visible = true  ;
    btMan4_8  -> Caption = "[" + AnsiString(mcPSB_L1Pitch      ) + "]" + "Go Left 1 Pitch"   ;  btMan4_8  -> Tag = (int)mcPSB_L1Pitch        ; btMan4_8  -> Visible = true  ;
    btMan4_9  -> Caption = "[" + AnsiString(mcSTL_Pick         ) + "]" + "Sorting Pick"      ;  btMan4_9  -> Tag = (int)mcSTL_Pick           ; btMan4_9  -> Visible = true  ;
    btMan4_10 -> Caption = "[" + AnsiString(mcSTL_YTopClamp    ) + "]" + "STL Top CYL U/D"   ;  btMan4_10 -> Tag = (int)mcSTL_YTopClamp      ; btMan4_10 -> Visible = true  ;
    btMan4_11 -> Caption = "[" + AnsiString(mcSTL_YBtmClamp    ) + "]" + "STL Btm CYL U/D"   ;  btMan4_11 -> Tag = (int)mcSTL_YBtmClamp      ; btMan4_11 -> Visible = true  ;
    btMan4_12 -> Caption = "[" + AnsiString(mcSTL_YTopClamp    ) + "]" + "STL Top CYL U/D"   ;  btMan4_12 -> Tag = (int)mcSTL_YTopClamp      ; btMan4_12 -> Visible = false ;
    btMan4_13 -> Caption = "[" + AnsiString(mcSTL_YBtmClamp    ) + "]" + "STL Btm CYL U/D"   ;  btMan4_13 -> Tag = (int)mcSTL_YBtmClamp      ; btMan4_13 -> Visible = false ;
    btMan4_14 -> Caption = "[" + AnsiString(mcAllHome          ) + "]" + "ALL HOME"          ;  btMan4_14 -> Tag = (int)mcAllHome            ; btMan4_14 -> Visible = true  ;

    btMan5_1  -> Caption = "[" + AnsiString(mcULD_Home         ) + "]" + "UnLoading home"    ;  btMan5_1  -> Tag = (int)mcULD_Home           ; btMan5_1  -> Visible = true  ;
    btMan5_2  -> Caption = "[" + AnsiString(mcULD_Supply       ) + "]" + "UnLoading Supply"  ;  btMan5_2  -> Tag = (int)mcULD_Supply         ; btMan5_2  -> Visible = true  ;
    btMan5_3  -> Caption = "[" + AnsiString(mcULD_Pick         ) + "]" + "UnLoading Pick"    ;  btMan5_3  -> Tag = (int)mcULD_Pick           ; btMan5_3  -> Visible = true  ;
    btMan5_4  -> Caption = "[" + AnsiString(mcULD_SttPstn      ) + "]" + "Move Start Pos"    ;  btMan5_4  -> Tag = (int)mcULD_SttPstn        ; btMan5_4  -> Visible = true  ;
    btMan5_5  -> Caption = "[" + AnsiString(mcULD_Place        ) + "]" + "UnLoading Place"   ;  btMan5_5  -> Tag = (int)mcULD_Place          ; btMan5_5  -> Visible = true  ;
    btMan5_6  -> Caption = "[" + AnsiString(mcULD_UpSlPitch    ) + "]" + "Pitch Up"          ;  btMan5_6  -> Tag = (int)mcULD_UpSlPitch      ; btMan5_6  -> Visible = true  ;
    btMan5_7  -> Caption = "[" + AnsiString(mcULD_DnSlPitch    ) + "]" + "Pitch Dn"          ;  btMan5_7  -> Tag = (int)mcULD_DnSlPitch      ; btMan5_7  -> Visible = true  ;
    btMan5_8  -> Caption = "[" + AnsiString(mcULD_Clamp        ) + "]" + "ULD Clamp U/D"     ;  btMan5_8  -> Tag = (int)mcULD_Clamp          ; btMan5_8  -> Visible = true  ;
    btMan5_9  -> Caption = "[" + AnsiString(mcPSB_Pusher       ) + "]" + "PSB Pusher F/W"    ;  btMan5_9  -> Tag = (int)mcPSB_Pusher         ; btMan5_9  -> Visible = true  ;
    btMan5_10 -> Caption = "[" + AnsiString(mcULD_TranInAC     ) + "]" + "Feeding In"        ;  btMan5_10 -> Tag = (int)mcULD_TranInAC       ; btMan5_10 -> Visible = true  ;
    btMan5_11 -> Caption = "[" + AnsiString(mcULD_TranOutAC    ) + "]" + "Feeding Out"       ;  btMan5_11 -> Tag = (int)mcULD_TranOutAC      ; btMan5_11 -> Visible = true  ;
    btMan5_12 -> Caption = "[" + AnsiString(mcULD_TranInAC     ) + "]" + "Feeding In"        ;  btMan5_12 -> Tag = (int)mcULD_TranInAC       ; btMan5_12 -> Visible = false ;
    btMan5_13 -> Caption = "[" + AnsiString(mcULD_TranOutAC    ) + "]" + "Feeding Out"       ;  btMan5_13 -> Tag = (int)mcULD_TranOutAC      ; btMan5_13 -> Visible = false ;
    btMan5_14 -> Caption = "[" + AnsiString(mcAllHome          ) + "]" + "ALL HOME"          ;  btMan5_14 -> Tag = (int)mcAllHome            ; btMan5_14 -> Visible = true  ;

    btMan7_1  -> Caption = "[" + AnsiString(mcETC_AllClampOff  ) + "]" + "AllClampOff"       ;  btMan7_1  -> Tag = (int)mcETC_AllClampOff    ; btMan7_1  -> Visible = true  ;
    btMan7_2  -> Caption = "[" + AnsiString(mcETC_AirBlower    ) + "]" + "AirBlower"         ;  btMan7_2  -> Tag = (int)mcETC_AirBlower      ; btMan7_2  -> Visible = true  ;
    btMan7_3  -> Caption = "[" + AnsiString(mcETC_PrbStt       ) + "]" + "Probe Start"       ;  btMan7_3  -> Tag = (int)mcETC_PrbStt         ; btMan7_3  -> Visible = true  ;
    btMan7_4  -> Caption = "[" + AnsiString(mcETC_TPrbUp       ) + "]" + "Top Probe Up"      ;  btMan7_4  -> Tag = (int)mcETC_TPrbUp         ; btMan7_4  -> Visible = true  ;
    btMan7_5  -> Caption = "[" + AnsiString(mcETC_TPrbDn       ) + "]" + "Top Probe Dn"      ;  btMan7_5  -> Tag = (int)mcETC_TPrbDn         ; btMan7_5  -> Visible = true  ;
    btMan7_6  -> Caption = "[" + AnsiString(mcETC_BPrbUp       ) + "]" + "Bottom Probe Up"   ;  btMan7_6  -> Tag = (int)mcETC_BPrbUp         ; btMan7_6  -> Visible = true  ;
    btMan7_7  -> Caption = "[" + AnsiString(mcETC_BPrbDn       ) + "]" + "Bottom Probe Dn"   ;  btMan7_7  -> Tag = (int)mcETC_BPrbDn         ; btMan7_7  -> Visible = true  ;
    btMan7_8  -> Caption = "[" + AnsiString(mcETC_LDRULDPlace  ) + "]" + "LDR ULD Place"     ;  btMan7_8  -> Tag = (int)mcETC_LDRULDPlace    ; btMan7_8  -> Visible = true  ;
    btMan7_9  -> Caption = "[" + AnsiString(mcSTL_YPchUp1Pitch ) + "]" + "STL Y Punch 1 Up"  ;  btMan7_9  -> Tag = (int)mcSTL_YPchUp1Pitch   ; btMan7_9  -> Visible = true  ;
    btMan7_10 -> Caption = "[" + AnsiString(mcSTL_YPchDn1Pitch ) + "]" + "STL Y Punch 1 Dn"  ;  btMan7_10 -> Tag = (int)mcSTL_YPchDn1Pitch   ; btMan7_10 -> Visible = true  ;
    btMan7_11 -> Caption = "[" + AnsiString(mcSTL_YDevUp1Pitch ) + "]" + "STL Y Sensor 1 Up" ;  btMan7_11 -> Tag = (int)mcSTL_YDevUp1Pitch   ; btMan7_11 -> Visible = true  ;
    btMan7_12 -> Caption = "[" + AnsiString(mcSTL_YDevDn1Pitch ) + "]" + "STL Y Sensor 1 Up" ;  btMan7_12 -> Tag = (int)mcSTL_YDevDn1Pitch   ; btMan7_12 -> Visible = true  ;
    btMan7_13 -> Caption = "[" + AnsiString(mcETC_PSBPlace     ) + "]" + "PSB Place Strp"    ;  btMan7_13 -> Tag = (int)mcETC_PSBPlace       ; btMan7_13 -> Visible = true  ;
    btMan7_14 -> Caption = "[" + AnsiString(mcAllHome          ) + "]" + "ALL HOME"          ;  btMan7_14 -> Tag = (int)mcAllHome            ; btMan7_14 -> Visible = true  ;

    if(OM.MstOptn.bUsedTempRS232){
        pnTemperature -> Visible = false;
        pnTmpr_NotVsn -> Visible = false;
        GroupBox7     -> Visible = false;
        GroupBox8     -> Visible = false;
        GroupBox9     -> Visible = false;
        GroupBox10    -> Visible = false;
        GroupBox11    -> Visible = false;
        GroupBox12    -> Visible = false;
        GroupBox13    -> Visible = false;
        GroupBox14    -> Visible = false;
        GroupBox15    -> Visible = false;
        GroupBox16    -> Visible = false;
        GroupBox17    -> Visible = false;
        GroupBox18    -> Visible = false;
        GroupBox19    -> Visible = false;
        GroupBox20    -> Visible = false;
        GroupBox21    -> Visible = false;
        GroupBox22    -> Visible = false;
    }

    SEQ.Reset();
}

void TFrmOperation::InitArrayPanel()
{
    DM.ARAY[riLDR].SetParent(pnLoader   ); DM.ARAY[riLDR].SetConfig(false , "riLDR"); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPR1].SetParent(pnPreBuff1 ); DM.ARAY[riPR1].SetConfig(false , "riPR1"); DM.ARAY[riPR1].ClearDispItem();
    DM.ARAY[riPR2].SetParent(pnPreBuff2 ); DM.ARAY[riPR2].SetConfig(false , "riPR2"); DM.ARAY[riPR2].ClearDispItem();
    DM.ARAY[riWRK].SetParent(pnWorkZone ); DM.ARAY[riWRK].SetConfig(false , "riWRK"); DM.ARAY[riWRK].ClearDispItem();
    DM.ARAY[riPSB].SetParent(pnPostBuff ); DM.ARAY[riPSB].SetConfig(false , "riPSB"); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnUnLoader ); DM.ARAY[riULD].SetConfig(false , "riULD"); DM.ARAY[riULD].ClearDispItem();

    DM.ARAY[riLDR].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riLDR].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riLDR].SetDispItem(csNone   ,"None "      ) ;

    DM.ARAY[riPR1].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riPR1].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riPR1].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riPR1].SetDispItem(csFromBuf,"FromBuf "   ) ;

    DM.ARAY[riPR2].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riPR2].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riPR2].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riPR2].SetDispItem(csFromBuf,"FromBuf "   ) ;

    DM.ARAY[riWRK].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riWRK].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riWRK].SetDispItem(csWork   ,"Work "      ) ;
    DM.ARAY[riWRK].SetDispItem(csGood   ,"Good "      ) ;
    DM.ARAY[riWRK].SetDispItem(csFail   ,"Fail "      ) ;
    DM.ARAY[riWRK].SetDispItem(csDrop   ,"Drop "      ) ;
    DM.ARAY[riWRK].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riWRK].SetDispItem(csFromBuf,"FromBuf "   ) ;

    DM.ARAY[riPSB].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riPSB].SetDispItem(csGood   ,"Good "      ) ;
    DM.ARAY[riPSB].SetDispItem(csFail   ,"Fail "      ) ;
    DM.ARAY[riPSB].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riPSB].SetDispItem(csWork   ,"Work "      ) ; 
    DM.ARAY[riPSB].SetDispItem(csMask   ,"Mask "      ) ; 
    DM.ARAY[riPSB].SetDispItem(csToBuf  ,"ToBuf "     ) ; 
    DM.ARAY[riPSB].SetDispItem(csFromBuf,"FromBuf "   ) ;

    DM.ARAY[riULD].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riULD].SetDispItem(csWork   ,"Work "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riULD].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );

    //Used Vision Option 떄문.JS
    PageControl1 -> ActivePage = TabSheet5;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;

    edLimAllFailCnt -> Text = OM.CmnOptn.iCheckFailCnt;
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
    //FM_Update();
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
                                                                                                     
    for(int i = 0 ; i < MAX_ARAY ; i++) {
        DM.ARAY[i].UpdateAray();
    }
    lbLDRMgzCnt -> Caption = LDR.GetMgzCnt();
//    Edit1 -> Text = STL._sLastID ;
    //int iBinCnt = STL._iBinCnt + 1;
    lbCnt   -> Caption = WRK.m_iDataErrCnt   ;
    lbCntRe -> Caption = WRK.m_iDataErrCntRe ;

    if(DM.ARAY[riPR1].GetCntExist() && !edPR1LotNo -> Focused() && !edPR1MgzNo->Focused() && !edPR1SlotNo->Focused() &&!btPR1LotInput->Focused() ) {
        edPR1LotNo  -> Text = DM.ARAY[riPR1].GetLotNo() ;
        edPR1MgzNo  -> Text = DM.ARAY[riPR1].GetID().ToIntDef(9999)/100 ;
        edPR1SlotNo -> Text = DM.ARAY[riPR1].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riPR2].GetCntExist() && !edPR2LotNo -> Focused() && !edPR2MgzNo->Focused() && !edPR2SlotNo->Focused() &&!btPR2LotInput->Focused() ) {
        edPR2LotNo  -> Text = DM.ARAY[riPR2].GetLotNo() ;
        edPR2MgzNo  -> Text = DM.ARAY[riPR2].GetID().ToIntDef(9999)/100 ;
        edPR2SlotNo -> Text = DM.ARAY[riPR2].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riWRK].GetCntExist() && !edInspLotNo->Focused() && !edInspMgzNo->Focused() && !edInspSlotNo->Focused() && !btInspLotInput->Focused() ) {
        edInspLotNo  -> Text = DM.ARAY[riWRK].GetLotNo() ;
        edInspMgzNo  -> Text = DM.ARAY[riWRK].GetID().ToIntDef(9999)/100 ;
        edInspSlotNo -> Text = DM.ARAY[riWRK].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riPSB].GetCntExist() && !edPSBLotNo->Focused() && !edPSBMgzNo->Focused() && !edPSBSlotNo->Focused() && !btPSBLotInput->Focused() ) {
        edPSBLotNo  -> Text  = DM.ARAY[riPSB].GetLotNo() ;
        edPSBMgzNo  -> Text = DM.ARAY[riPSB].GetID().ToIntDef(9999)/100 ;
        edPSBSlotNo -> Text = DM.ARAY[riPSB].GetID().ToIntDef(99)%100;
    }

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        case lvContol   : btOperator -> Caption = " CONTROL"; break ;
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
    if(OM.MstOptn.bNotUstLot) btStart  -> Enabled = true          ;
    else                      btStart  -> Enabled = LT.m_bLotOpen ;

    LT.DispLotInfo(
        pnLotNo        ,
        pnLotJobFile   ,
        NULL           ,
        pnLotStartTime ,
        NULL           ,
        pnLotRunTime   ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnLotChipUpeh  ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnLotWorkChip  ,
        pnLotFailChip  ,
        NULL           ,
        pnLotGoodChip  );

    LT.DispDayInfo(
        pnDayRunTime    ,
        pnDayIdleTime   ,
        pnDayJamTime    ,
        pnDayTotalTime  ,
        NULL            ,
        pnDayChipUpeh   ,
        NULL            ,
        NULL            ,
        NULL            ,
        pnDayWorkChip   ,
        pnDayFailChip   ,
        NULL            ,
        pnDayLotCount   );

    //EM_
    EM_DisplayList(lbErr);

    pnIgnoreTrim    -> Color  =  OM.CmnOptn.bIgnrTrim       ? clLime : clRed ;
    pnIgnrDoor      -> Color  =  OM.CmnOptn.bIgnrDoor       ? clLime : clRed ;
    pnInspSkip      -> Color  =  OM.CmnOptn.bIgnrInsp       ? clLime : clRed ;
    pnIgnrHeatAlm   -> Color  =  OM.CmnOptn.bIgnrHeatAlm    ? clLime : clRed ;
    pnIgnrStrAlm    -> Color  =  OM.CmnOptn.bIgnrStrOverAlm ? clLime : clRed ;
    pnLoadingStop   -> Color  =  OM.CmnOptn.bLoadingStop    ? clLime : clRed ;
    pnCheckFailCnt  -> Color  =  OM.CmnOptn.bCheckFailCnt   ? clLime : clRed ;
    pnIgnoreTrim    -> Color  =  OM.CmnOptn.bIgnrTrim       ? clLime : clRed ;
//  pnAutoConvesion -> Color  =  OM.CmnOptn.bAutoConvesion  ? clLime : clRed ;

    pnIgnoreTrim    -> Caption  =  OM.CmnOptn.bIgnrTrim       ? "ON" : "OFF" ;
    pnIgnrDoor      -> Caption  =  OM.CmnOptn.bIgnrDoor       ? "ON" : "OFF" ;
    pnInspSkip      -> Caption  =  OM.CmnOptn.bIgnrInsp       ? "ON" : "OFF" ;
    pnIgnrHeatAlm   -> Caption  =  OM.CmnOptn.bIgnrHeatAlm    ? "ON" : "OFF" ;
    pnIgnrStrAlm    -> Caption  =  OM.CmnOptn.bIgnrStrOverAlm ? "ON" : "OFF" ;
    pnLoadingStop   -> Caption  =  OM.CmnOptn.bLoadingStop    ? "ON" : "OFF" ;
    pnCheckFailCnt  -> Caption  =  OM.CmnOptn.bCheckFailCnt   ? "ON" : "OFF" ;
    pnIgnoreTrim    -> Caption  =  OM.CmnOptn.bIgnrTrim       ? "ON" : "OFF" ;
//  pnAutoOperation -> Caption  =  OM.CmnOptn.bAutoOperation  ? "ON" : "OFF" ;
//  pnAutoConvesion -> Caption  =  OM.CmnOptn.bAutoConvesion  ? "ON" : "OFF" ;

    btPR1LotInput  -> Visible = !SEQ._bRun ;
    btPR2LotInput  -> Visible = !SEQ._bRun ;
    btInspLotInput -> Visible = !SEQ._bRun ;
    btPSBLotInput  -> Visible = !SEQ._bRun ;

    if(SEQ._bInspDispr) { CheckStripDisappear(); SEQ._bInspDispr = false; } //함수 끝나고 자동 스타트 됍.
    if(SEQ._bInspUnkwn) { CheckStripUnknwon  (); SEQ._bInspUnkwn = false; } //함수 끝나고 자동 스타트 됍.
//Lot 엔드시 화면에 보여주기 용.

    //bool isLotChanged = (DM.ARAY[riULD].GetLotNo()!="" && DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS2].GetLotNo() && !DM.ARAY[riPS2].CheckAllStat(csNone)) ||
    //                    (DM.ARAY[riULD].GetLotNo()!="" && DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS1].GetLotNo() && !DM.ARAY[riPS1].CheckAllStat(csNone) && DM.ARAY[riPS2].CheckAllStat(csNone));

    btLotOpen -> Enabled = !OM.MstOptn.bNotUstLot ;
    btLotEnd  -> Enabled = !OM.MstOptn.bNotUstLot ;




    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;

    for(int i = 1 ; i < 8/*페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;
            if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
            else                             pBtn -> Font -> Color = clBlack ;
        }
    }
    if(!OM.MstOptn.bUsedTempRS232){
        TPanel *  pPanl1     ;
        TPanel *  pPanl2     ;
        AnsiString sPanelName1 , sPanelName2 ;
        int   iCrntTemp;
        int   iSetTemp;

        for(int i = 1 ; i <= MAX_TEMP_CH + 1; i++) {
            sPanelName1 = "pnCrtTMPR" + AnsiString(i)            ;
            sPanelName2 = "pnCrtTMPR" + AnsiString(i) + "_NotVsn";
            pPanl1 = (TPanel*)FindComponent(sPanelName1);
            pPanl2 = (TPanel*)FindComponent(sPanelName2);
            if(pPanl1 == NULL || pPanl2 == NULL) continue ;
            iCrntTemp = TCU.GetCrntTemp(i);
            pPanl1 -> Caption = iCrntTemp;
            pPanl2 -> Caption = iCrntTemp;
        }

        for(int i = 1 ; i <= MAX_TEMP_CH + 1 ; i++) {
            sPanelName1 = "pnSetTMPR" + AnsiString(i)            ;
            sPanelName2 = "pnSetTMPR" + AnsiString(i) + "_NotVsn";
            pPanl1 = (TPanel*)FindComponent(sPanelName1);
            pPanl2 = (TPanel*)FindComponent(sPanelName2);
            if(pPanl1 == NULL || pPanl2 == NULL) continue ;
            iCrntTemp = TCU.GetSetTemp(i);
            pPanl1 -> Caption = iCrntTemp;
            pPanl2 -> Caption = iCrntTemp;
        }
    }
    /*
    for(int i = 1 ; i <= MAX_TEMP_CH + 1; i++) {
        sPanelName = "pnCrtTMPR" + AnsiString(i);
        pPanl = (TPanel*)FindComponent(sPanelName);
        if(pPanl == NULL) continue ;
        iCrntTemp = TCU.GetCrntTemp(i);
        pPanl -> Caption = iCrntTemp;
    }

    for(int i = 1 ; i <= MAX_TEMP_CH + 1 ; i++) {
        sPanelName = "pnSetTMPR" + AnsiString(i);
        pPanl = (TPanel*)FindComponent(sPanelName);
        if(pPanl == NULL) continue ;
        iSetTemp = TCU.GetSetTemp(i);
        pPanl -> Caption = iSetTemp;
    }
    */
    lbCnt -> Caption = DM.ARAY[riPR1].GetStep();

    if(OM.MstOptn.bUsedVision){ TabSheet6 -> TabVisible = true ;}
    else                      { TabSheet6 -> TabVisible = false;}
    int iWorkChip , iFailChip , iGoodChip;
    iWorkChip = pnLotWorkChip -> Caption.ToInt() ;
    iFailChip = pnLotFailChip -> Caption.ToInt() ;
    iGoodChip = iWorkChip - iFailChip ;

    //pnLotGoodChip -> Caption = iGoodChip;

    pnLotTrimChip -> Caption = STL.m_iTrimCnt;

    edBinCnt -> Text = STL.m_iBinCnt;

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    //if(SEQ._bRun) {FM_MsgOk("ERROR" , "Can't Use While Running"); return ;}
    FrmLotOpen -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    //bool isAllHomeEnd = true;

    //Inspect.
    //if (!MT_GetHomeEnd(miPR1_XCmp)){isAllHomeEnd = false; }//EM_SetErr(eiPRB_XCmpAlarm); isAllHomeEnd = false; }
    //if (!MT_GetHomeEnd(miWRK_XCmp)){isAllHomeEnd = false; }//EM_SetErr(eiWRK_XCmpAlarm); isAllHomeEnd = false; }
    //if (!MT_GetHomeEnd(miPSB_XCmp)){isAllHomeEnd = false; }//EM_SetErr(eiPSB_XCmpAlarm); isAllHomeEnd = false; }
    //if (!MT_GetHomeEnd(miWRK_ZTop)){isAllHomeEnd = false; }//EM_SetErr(eiWRK_ZTopAlarm); isAllHomeEnd = false; }
    //if (!MT_GetHomeEnd(miWRK_ZBtm)){isAllHomeEnd = false; }//EM_SetErr(eiWRK_ZBtmAlarm); isAllHomeEnd = false; }
    //if(OM.EqpOptn.bExistLoader  ){ if (!MT_GetHomeEnd(miLDR_ZCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLDR_ZCmpAlarm); isAllHomeEnd = false; } }
    //if(OM.EqpOptn.bExistLoader  ){ if (!MT_GetHomeEnd(miLDR_YCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLDR_YCmpAlarm); isAllHomeEnd = false; } }
    //if(OM.EqpOptn.bExistUnLoader){ if (!MT_GetHomeEnd(miULD_ZCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLDR_ZCmpAlarm); isAllHomeEnd = false; } }
    //if(OM.EqpOptn.bExistUnLoader){ if (!MT_GetHomeEnd(miULD_YCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLDR_YCmpAlarm); isAllHomeEnd = false; } }

    //if(isAllHomeEnd) SEQ._bBtnStart = true ;
    //else FM_MsgOk("ERROR","PLEASE All Home ") ;

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
    if(!LT.m_bLotOpen) {FM_MsgOk("ERROR" , "Can't Use While Lot Closed"); return ;}
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    bool LDRMgzDetect ;
    bool ULDMgzDetect ;
    LDRMgzDetect = IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    ULDMgzDetect = IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);

    if(LDRMgzDetect && ULDMgzDetect) MM.SetManCycle(mcETC_LDRULDPlace);
    else if( LDRMgzDetect && !ULDMgzDetect) MM.SetManCycle(mcLDR_Place);
    else if(!LDRMgzDetect &&  ULDMgzDetect) MM.SetManCycle(mcULD_Place);

//    if(DM.ARAY[riLDR].GetCntExist() && DM.ARAY[riULD].GetCntExist()) MM.SetManCycle(mcETC_LDRULDPlace);
//    else if(DM.ARAY[riLDR].GetCntExist()) MM.SetManCycle(mcLDR_Place);
//    else if(DM.ARAY[riULD].GetCntExist()) MM.SetManCycle(mcULD_Place);
//    준선 수정.2012년 11월 21일

    LT.LotEnd ();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

//    if(FM_GetLevel() == lvOperator) return ;

    switch(iIndex) {
        case 0 :                                 break  ;
        case 1 :                                 break  ;
        case 2 : if(FM_GetLevel() == lvOperator) return ;
        case 3 : if(FM_GetLevel() == lvOperator) return ;
        case 4 : if(FM_GetLevel() == lvOperator) return ;
        case 5 : if(FM_GetLevel() == lvOperator) return ;
        case 6 : if(FM_GetLevel() == lvOperator) return ;
        case 7 :                                 break  ;
        case 10: if(FM_GetLevel() == lvOperator) return ;
        case 11: if(FM_GetLevel() == lvOperator) return ;
    }

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                            break ;
        case 1 : OM.CmnOptn.bIgnrDoor       = !OM.CmnOptn.bIgnrDoor       ; break ;
        case 2 : OM.CmnOptn.bIgnrIdCehck    = !OM.CmnOptn.bIgnrIdCehck    ; break ;
        case 3 : OM.CmnOptn.bIgnrInsp       = !OM.CmnOptn.bIgnrInsp       ; break ;
//        case 4 : OM.CmnOptn.bIgnrHeatAlm    = !OM.CmnOptn.bIgnrHeatAlm    ; break ;
        case 5 : OM.CmnOptn.bIgnrStrOverAlm = !OM.CmnOptn.bIgnrStrOverAlm ; break ;
//        case 5 : OM.CmnOptn.bIgnrStrOverAlm = !OM.CmnOptn.bIgnrStrOverAlm ; break ;
        case 6 : OM.CmnOptn.bLoadingStop    = !OM.CmnOptn.bLoadingStop    ; break ;
        case 7 : OM.CmnOptn.bCheckFailCnt   = !OM.CmnOptn.bCheckFailCnt   ; break ;
        case 10: OM.CmnOptn.bIgnrTrim       = !OM.CmnOptn.bIgnrTrim       ; break ;
        case 11: OM.CmnOptn.bIgnrHeatAlm    = !OM.CmnOptn.bIgnrHeatAlm    ; break ;
        //case 6 : OM.CmnOptn.bAutoOperation  = !OM.CmnOptn.bAutoOperation  ; break ;
        //case 7 : OM.CmnOptn.bAutoConvesion  = !OM.CmnOptn.bAutoConvesion  ; break ;
    }

    SPC.SaveOptionLog(iIndex);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPR1ClearClick(TObject *Sender)
{
    if(SEQ._bRun) return ;
    
    int iTag = ((TButton*)Sender) -> Tag ;

    AnsiString sLotNo , sSlotNo ,sMsg;
    switch (iTag)
    {
        default :
            break ;

        case 1 : sLotNo   = "PreBuff1 : " + DM.ARAY[riPR1].GetLotNo() + " " ;
                 sSlotNo  = "PreBuff1 : " + AnsiString(DM.ARAY[riPR1].GetID().ToIntDef(99)%100) + " ";
                 sMsg     = "Clear Bt Clicked" ;
                 SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

                 DM.ARAY[riPR1].SetStat(csNone);
                 break;

        case 2 : sLotNo   = "PreBuff2 : " + DM.ARAY[riPR2].GetLotNo() + " " ;
                 sSlotNo  = "PreBuff2 : " + AnsiString(DM.ARAY[riPR2].GetID().ToIntDef(99)%100) + " ";
                 sMsg     = "Clear Bt Clicked" ;
                 SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

                 DM.ARAY[riPR2].SetStat(csNone);
                 break;

        case 3 : sLotNo   = "Work Zone : " + DM.ARAY[riWRK].GetLotNo() + " ";;
                 sSlotNo  = "Work Zone : " + AnsiString(DM.ARAY[riWRK].GetID().ToIntDef(99)%100) + " ";
                 sMsg     = "Clear Bt Clicked" ;
                 SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

                 DM.ARAY[riWRK].SetStat(csNone);
                 break ;

        case 4 : sLotNo   = "PostBuff : " + DM.ARAY[riPSB].GetLotNo() + " ";;
                 sSlotNo  = "PostBuff : " + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";
                 sMsg     = "Clear Bt Clicked" ;
                 SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

                 DM.ARAY[riPSB].SetStat(csNone);
                 break;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPR1LotInputClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

//    if(FM_MsgYesNo("Confirm","Did you want to enter Lot information ") != mrYes) return ;

    int iTag = ((TButton*)Sender) -> Tag ;
    AnsiString sLotNo ;
    int iMgzNo , iSlotNo ;

    switch (iTag)
    {
        default :
            break ;

        case 1 :
            sLotNo  = edPR1LotNo ->Text ;
            iMgzNo  = edPR1MgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPR1SlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPR1].SetLotNo(sLotNo) ;
            DM.ARAY[riPR1].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 2 :
            sLotNo  = edPR2LotNo ->Text ;
            iMgzNo  = edPR2MgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPR2SlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPR2].SetLotNo(sLotNo) ;
            DM.ARAY[riPR2].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 3 :
            sLotNo  = edInspLotNo ->Text ;
            iMgzNo  = edInspMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edInspSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riWRK].SetLotNo(sLotNo) ;
            DM.ARAY[riWRK].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 4 :
            sLotNo  = edPSBLotNo ->Text ;
            iMgzNo  = edPSBMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPSBSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPSB].SetLotNo(sLotNo) ;
            DM.ARAY[riPSB].SetID(iMgzNo *100 + iSlotNo);
            break ;
     }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::edLimAllFailCntChange(TObject *Sender)
{
    OM.CmnOptn.iCheckFailCnt = edLimAllFailCnt -> Text.ToIntDef(OM.CmnOptn.iCheckFailCnt);

    edLimAllFailCnt -> Text = OM.CmnOptn.iCheckFailCnt;

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
//    if (!DM.ARA Y[riPR2].CheckAllStat(csNone) && !IO_GetX(xPRB_3Pkg) ) {
//        if(FM_MsgYesNo("Err " , "[PreBuffer2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPR2 ].SetStat(csNone); SEQ._bBtnStart = true ;}
//    }
    //if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect1) && !IO_GetX(xWRK_Detect2) && !OM.CmnOptn.bDryRun ) {
    //    if(FM_MsgYesNo("Err " , "[WorkZone Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riWRK ].SetStat(csNone); SEQ._bBtnStart = true ;}
    //}
    //else if(DM.ARAY[riPSB].GetCntExist() && !OM.CmnOptn.bDryRun) {
    //    if((!IO_GetX(xPSB_LDetect) && !IO_GetX(xPSB_Pkg1) && !IO_GetX(xPSB_Pkg2) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) ) {
    //        if(FM_MsgYesNo("Err " , "[PostBuffer2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPSB].SetStat(csNone); SEQ._bBtnStart = true ;}
    //    }
    //}
    if ( !DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) ) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Loader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riLDR ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if ( !DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[UnLoader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }

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
    int iTag = ((TButton*)Sender) -> Tag ;

    switch (iTag)
    {
        default :
            break ;

        case 1 : DM.ARAY[riPR1].SetStat(csUnkwn);
                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
                 break;

        case 2 : DM.ARAY[riPR2].SetStat(csUnkwn);
                 if(DM.ARAY[riPR1].CheckAllStat(csNone) || DM.ARAY[riPR1].CheckAllStat(csUnkwn)) DM.ARAY[riPR2].SetLotNo(PR1.GetWorkedLotNo()     ) ;
                 else                                                                            DM.ARAY[riPR2].SetLotNo(DM.ARAY[riPR1].GetLotNo()) ;
                 break;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    LT.ClearDayInfo() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOKClick(TObject *Sender)
{
    pnTemperature -> Visible = false;
    pnTmpr_NotVsn -> Visible = false;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::pnCrtTMPR1DblClick(TObject *Sender)
{
    int iTag = ((TPanel*)Sender) -> Tag ;

    switch(iTag){
        default :                                       break;
        case 1  : edTemp1_NotVsn -> Text = OM.CmnOptn.iTemperature1;
                  edTemp2_NotVsn -> Text = OM.CmnOptn.iTemperature2;
                  edTemp3_NotVsn -> Text = OM.CmnOptn.iTemperature3;
                  edTemp4_NotVsn -> Text = OM.CmnOptn.iTemperature4;
                  edTemp5_NotVsn -> Text = OM.CmnOptn.iTemperature5;
                  edTemp6_NotVsn -> Text = OM.CmnOptn.iTemperature6;
                  edTemp7_NotVsn -> Text = OM.CmnOptn.iTemperature7;
                  edTemp8_NotVsn -> Text = OM.CmnOptn.iTemperature8;
                  pnTmpr_NotVsn -> Visible = true;
                  break;

        case 2  : edTemp1 -> Text = OM.CmnOptn.iTemperature1;
                  edTemp2 -> Text = OM.CmnOptn.iTemperature2;
                  edTemp3 -> Text = OM.CmnOptn.iTemperature3;
                  edTemp4 -> Text = OM.CmnOptn.iTemperature4;
                  edTemp5 -> Text = OM.CmnOptn.iTemperature5;
                  edTemp6 -> Text = OM.CmnOptn.iTemperature6;
                  edTemp7 -> Text = OM.CmnOptn.iTemperature7;
                  edTemp8 -> Text = OM.CmnOptn.iTemperature8;
                  pnTemperature -> Visible = true;
                  break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btTemp1Click(TObject *Sender)
{
    int iTag = ((TButton*)Sender) -> Tag ;

    int iSetTemp = 0;
    int iTemp8   = 0;


    switch (iTag)
    {
        default :                                       break;

        case 1  : if(pnTemperature -> Visible){ iSetTemp = edTemp1 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature1 = edTemp1 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp1_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature1 = edTemp1_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 2  : if(pnTemperature -> Visible){ iSetTemp = edTemp2 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature2 = edTemp2 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp2_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature2 = edTemp2_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 3  : if(pnTemperature -> Visible){ iSetTemp = edTemp3 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature3 = edTemp3 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp3_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature3 = edTemp3_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 4  : if(pnTemperature -> Visible){ iSetTemp = edTemp4 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature4 = edTemp4 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp4_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature4 = edTemp4_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 5  : if(pnTemperature -> Visible){ iSetTemp = edTemp5 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature5 = edTemp5 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp5_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature5 = edTemp5_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 6  : if(pnTemperature -> Visible){ iSetTemp = edTemp6 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature6 = edTemp6 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp6_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature6 = edTemp6_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 7  : if(pnTemperature -> Visible){ iSetTemp = edTemp7 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature7 = edTemp7 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp7_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature7 = edTemp7_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 8  : if(pnTemperature -> Visible){ iSetTemp = edTemp8 -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature8 = edTemp8 -> Text.ToInt(); }
                  else                        { iSetTemp = edTemp8_NotVsn -> Text.ToInt()  ;
                                                OM.CmnOptn.iTemperature8 = edTemp8_NotVsn -> Text.ToInt(); }

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 9  : if(pnTemperature -> Visible){ 
                      iSetTemp = edTempAll -> Text.ToInt();
                      iTemp8   = edTemp8   -> Text.ToInt();
                      OM.CmnOptn.iTemperature1 = edTempAll -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature2 = edTempAll -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature3 = edTempAll -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature4 = edTempAll -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature5 = edTempAll -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature6 = edTempAll -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature7 = edTempAll -> Text.ToInt() ;
                      //OM.CmnOptn.iTemperature8 = edTempAll -> Text.ToInt() ;
                      TCU.SetTempAll(iSetTemp , iSetTemp , iSetTemp , iSetTemp ,
                                     iSetTemp , iSetTemp , iSetTemp , iTemp8 );
                      break;
                  }
                  else{
                      iSetTemp = edTempAll_NotVsn -> Text.ToInt();
                      iTemp8   = edTemp8_NotVsn   -> Text.ToInt();
                      OM.CmnOptn.iTemperature1 = edTempAll_NotVsn -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature2 = edTempAll_NotVsn -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature3 = edTempAll_NotVsn -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature4 = edTempAll_NotVsn -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature5 = edTempAll_NotVsn -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature6 = edTempAll_NotVsn -> Text.ToInt() ;
                      OM.CmnOptn.iTemperature7 = edTempAll_NotVsn -> Text.ToInt() ;
                      //OM.CmnOptn.iTemperature8 = edTempAll -> Text.ToInt() ;
                      TCU.SetTempAll(iSetTemp , iSetTemp , iSetTemp , iSetTemp ,
                                     iSetTemp , iSetTemp , iSetTemp , iTemp8 );
                      break;
                   }
    }
    /*
    switch (iTag)
    {
        default :                                       break;

        case 1  : iSetTemp = edTemp1 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature1 = edTemp1 -> Text.ToInt();

                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 2  : iSetTemp = edTemp2 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature2 = edTemp2 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 3  : iSetTemp = edTemp3 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature3 = edTemp3 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 4  : iSetTemp = edTemp4 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature4 = edTemp4 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 5  : iSetTemp = edTemp5 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature5 = edTemp5 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 6  : iSetTemp = edTemp6 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature6 = edTemp6 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 7  : iSetTemp = edTemp7 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature7 = edTemp7 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 8  : iSetTemp = edTemp8 -> Text.ToInt()  ;
                  OM.CmnOptn.iTemperature8 = edTemp8 -> Text.ToInt() ;
                  TCU.SetTemp(iTag , iSetTemp)        ; break;

        case 9  : iSetTemp = edTempAll -> Text.ToInt();
                  iTemp8   = edTemp8   -> Text.ToInt();
                  OM.CmnOptn.iTemperature1 = edTempAll -> Text.ToInt() ;
                  OM.CmnOptn.iTemperature2 = edTempAll -> Text.ToInt() ;
                  OM.CmnOptn.iTemperature3 = edTempAll -> Text.ToInt() ;
                  OM.CmnOptn.iTemperature4 = edTempAll -> Text.ToInt() ;
                  OM.CmnOptn.iTemperature5 = edTempAll -> Text.ToInt() ;
                  OM.CmnOptn.iTemperature6 = edTempAll -> Text.ToInt() ;
                  OM.CmnOptn.iTemperature7 = edTempAll -> Text.ToInt() ;
                  //OM.CmnOptn.iTemperature8 = edTempAll -> Text.ToInt() ;
                  TCU.SetTempAll(iSetTemp , iSetTemp , iSetTemp , iSetTemp ,
                                 iSetTemp , iSetTemp , iSetTemp , iTemp8 );
                  break;
    } */
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Panel3Click(TObject *Sender)
{
    //DM.ShiftArrayData(riPR1,riPR2);
   // PCM.Start( );
   DM.ARAY[riPSB].SetStep(1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
  // AnsiString sname ;

   sname = edEdit1 -> Text ;

   STL.m_iTrimCnt ++ ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button2Click(TObject *Sender)
{
    LT.WriteTrimInfo(sname, STL.m_iTrimCnt, STL._iBinCnt);
    STL.m_iTrimCnt = 0;
}
//---------------------------------------------------------------------------

