//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormManual.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "ManualMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmManual *FrmManual;
//---------------------------------------------------------------------------
__fastcall TFrmManual::TFrmManual(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------







void __fastcall TFrmManual::btSTGVccmClick(TObject *Sender)
{
    int iIONo = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iIONo , !IO_GetY(iIONo));
}
//---------------------------------------------------------------------------



void __fastcall TFrmManual::FormCreate(TObject *Sender)
{
    //나중에 추가되면 Visible 바꾸기.
    //Front.
  //btMan1_1  -> Caption = "[" + AnsiString(mcLDR_FCycleWork        ) + "]" + "F Cycle Work "  ;  btMan1_1  -> Tag = (int)mcLDR_FCycleWork     ; btMan1_1  -> Visible = true  ;
  //btMan1_2  -> Caption = "[" + AnsiString(mcLDR_FWorkSttBtm       ) + "]" + "F WorkStt Btm"  ;  btMan1_2  -> Tag = (int)mcLDR_FWorkSttBtm    ; btMan1_2  -> Visible = true  ;
  //btMan1_3  -> Caption = "[" + AnsiString(mcLDR_FWorkSttTop       ) + "]" + "F WorkStt Top"  ;  btMan1_3  -> Tag = (int)mcLDR_FWorkSttTop    ; btMan1_3  -> Visible = true  ;
  //btMan1_4  -> Caption = "[" + AnsiString(mcLDR_FPitchUp          ) + "]" + "F Pitch Up   "  ;  btMan1_4  -> Tag = (int)mcLDR_FPitchUp       ; btMan1_4  -> Visible = true  ;
  //btMan1_5  -> Caption = "[" + AnsiString(mcLDR_FPitchDn          ) + "]" + "F Pitch Dn   "  ;  btMan1_5  -> Tag = (int)mcLDR_FPitchDn       ; btMan1_5  -> Visible = true  ;

  //Rear
  //btMan1_8  -> Caption = "[" + AnsiString(mcLDR_RCycleWork        ) + "]" + "R Cycle Work "  ;  btMan1_8  -> Tag = (int)mcLDR_RCycleWork     ; btMan1_8  -> Visible = true  ;
  //btMan1_9  -> Caption = "[" + AnsiString(mcLDR_RWorkSttBtm       ) + "]" + "R WorkStt Btm"  ;  btMan1_9  -> Tag = (int)mcLDR_RWorkSttBtm    ; btMan1_9  -> Visible = true  ;
  //btMan1_10 -> Caption = "[" + AnsiString(mcLDR_RWorkSttTop       ) + "]" + "R WorkStt Top"  ;  btMan1_10 -> Tag = (int)mcLDR_RWorkSttTop    ; btMan1_10 -> Visible = true  ;
  //btMan1_11 -> Caption = "[" + AnsiString(mcLDR_RPitchUp          ) + "]" + "R Pitch Up   "  ;  btMan1_11 -> Tag = (int)mcLDR_RPitchUp       ; btMan1_11 -> Visible = true  ;
  //btMan1_12 -> Caption = "[" + AnsiString(mcLDR_RPitchDn          ) + "]" + "R Pitch Dn   "  ;  btMan1_12 -> Tag = (int)mcLDR_RPitchDn       ; btMan1_12 -> Visible = true  ;

  //Index
  //btMan2_1  -> Caption = "[" + AnsiString(mcPRI_FCycleWork        ) + "]" + "F PRI CycleWork";  btMan2_1  -> Tag = (int)mcPRI_FCycleWork     ; btMan2_1  -> Visible = true ;
  //btMan2_2  -> Caption = "[" + AnsiString(mcPRI_FIndxUpDn         ) + "]" + "F PRI IndxUpDn ";  btMan2_2  -> Tag = (int)mcPRI_FIndxUpDn      ; btMan2_2  -> Visible = true ;
  //btMan2_3  -> Caption = "[" + AnsiString(mcIDX_FCycleGet         ) + "]" + "F IDX CycleGet ";  btMan2_3  -> Tag = (int)mcIDX_FCycleGet      ; btMan2_3  -> Visible = true ;
  //btMan2_4  -> Caption = "[" + AnsiString(mcIDX_FCycleOut         ) + "]" + "F IDX CycleOut ";  btMan2_4  -> Tag = (int)mcIDX_FCycleOut      ; btMan2_4  -> Visible = true ;
  //btMan2_5  -> Caption = "[" + AnsiString(mcIDX_FIndxUpDn         ) + "]" + "F IDX IndxUpDn ";  btMan2_5  -> Tag = (int)mcIDX_FIndxUpDn      ; btMan2_5  -> Visible = true ;
  //btMan2_6  -> Caption = "[" + AnsiString(mcIDX_FPitchRt          ) + "]" + "F IDX PitchRt  ";  btMan2_6  -> Tag = (int)mcIDX_FPitchRt       ; btMan2_6  -> Visible = true ;
  //btMan2_7  -> Caption = "[" + AnsiString(mcIDX_FPitchLt          ) + "]" + "F IDX PitchLt  ";  btMan2_7  -> Tag = (int)mcIDX_FPitchLt       ; btMan2_7  -> Visible = true ;
  //btMan2_8  -> Caption = "[" + AnsiString(mcPRI_RCycleWork        ) + "]" + "R PRI CycleWork";  btMan2_8  -> Tag = (int)mcPRI_RCycleWork     ; btMan2_8  -> Visible = true ;
  //btMan2_9  -> Caption = "[" + AnsiString(mcPRI_RIndxUpDn         ) + "]" + "R PRI IndxUpDn ";  btMan2_9  -> Tag = (int)mcPRI_RIndxUpDn      ; btMan2_9  -> Visible = true ;
  //btMan2_10 -> Caption = "[" + AnsiString(mcIDX_RCycleGet         ) + "]" + "R IDX CycleGet ";  btMan2_10 -> Tag = (int)mcIDX_RCycleGet      ; btMan2_10 -> Visible = true ;
  //btMan2_11 -> Caption = "[" + AnsiString(mcIDX_RCycleOut         ) + "]" + "R IDX CycleOut ";  btMan2_11 -> Tag = (int)mcIDX_RCycleOut      ; btMan2_11 -> Visible = true ;
  //btMan2_12 -> Caption = "[" + AnsiString(mcIDX_RIndxUpDn         ) + "]" + "R IDX IndxUpDn ";  btMan2_12 -> Tag = (int)mcIDX_RIndxUpDn      ; btMan2_12 -> Visible = true ;
  //btMan2_13 -> Caption = "[" + AnsiString(mcIDX_RPitchRt          ) + "]" + "R IDX PitchRt  ";  btMan2_13 -> Tag = (int)mcIDX_RPitchRt       ; btMan2_13 -> Visible = true ;
  //btMan2_14 -> Caption = "[" + AnsiString(mcIDX_RPitchLt          ) + "]" + "R IDX PitchLt  ";  btMan2_14 -> Tag = (int)mcIDX_RPitchLt       ; btMan2_14 -> Visible = true ;

    // Picker
    btMan3_1  -> Caption = "[" + AnsiString(mcPCKCyclePick          ) + "]" + "CyclePick        ";  btMan3_1  -> Tag = (int)mcPCKCyclePick           ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcPCKCycleAlign         ) + "]" + "CycleAlign       ";  btMan3_2  -> Tag = (int)mcPCKCycleAlign          ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcPCKCycleAlignPick     ) + "]" + "CycleAlignPick   ";  btMan3_3  -> Tag = (int)mcPCKCycleAlignPick      ; btMan3_3  -> Visible = true  ;
    btMan3_4  -> Caption = "[" + AnsiString(mcPCKCycleVisn          ) + "]" + "CycleVisn        ";  btMan3_4  -> Tag = (int)mcPCKCycleVisn           ; btMan3_4  -> Visible = true  ;
  //btMan3_5  -> Caption = "[" + AnsiString(mcPCKCycleAttach        ) + "]" + "CycleAttach      ";  btMan3_5  -> Tag = (int)mcPCKCycleAttach         ; btMan3_5  -> Visible = true  ;
  //btMan3_6  -> Caption = "[" + AnsiString(mcPCKCyclePush          ) + "]" + "CyclePush        ";  btMan3_6  -> Tag = (int)mcPCKCyclePush           ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcPCKCycleManAlgnPlce   ) + "]" + "CycleAlignPlace  ";  btMan3_7  -> Tag = (int)mcPCKCycleManAlgnPlce    ; btMan3_7  -> Visible = true  ;
    btMan3_8  -> Caption = "[" + AnsiString(mcPCKCycleManVsDefectPos) + "]" + "Vision Defect Pos";  btMan3_8  -> Tag = (int)mcPCKCycleManVsDefectPos ; btMan3_8  -> Visible = true  ;
    btMan3_9  -> Caption = "[" + AnsiString(mcPCKCycleManVsAlignPos ) + "]" + "Vision Align Pos ";  btMan3_9  -> Tag = (int)mcPCKCycleManVsAlignPos  ; btMan3_9  -> Visible = true  ;

    // Dispensor
    btMan4_1  -> Caption = "[" + AnsiString(mcDSPCycleVisnBf        ) + "]" + "CycleVisnBf      ";  btMan4_1  -> Tag = (int)mcDSPCycleVisnBf          ; btMan4_1  -> Visible = true  ;
  //btMan4_2  -> Caption = "[" + AnsiString(mcDSPCycleDisp          ) + "]" + "CycleDisp        ";  btMan4_2  -> Tag = (int)mcDSPCycleDisp            ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcDSPCycleVisnAt        ) + "]" + "CycleVisnAt      ";  btMan4_3  -> Tag = (int)mcDSPCycleVisnAt          ; btMan4_3  -> Visible = true  ;
    btMan4_4  -> Caption = "[" + AnsiString(mcDSPCycleClean         ) + "]" + "CycleClean       ";  btMan4_4  -> Tag = (int)mcDSPCycleClean           ; btMan4_4  -> Visible = true  ;
    btMan4_5  -> Caption = "[" + AnsiString(mcDSPCycleManNdlCheck   ) + "]" + "CycleManNdlCheck ";  btMan4_5  -> Tag = (int)mcDSPCycleManNdlCheck     ; btMan4_5  -> Visible = true  ;
//    btMan4_6  -> Caption = "[" + AnsiString(mcDSPNdlCrFwBw          ) + "]" + "Niddle CleanFwBw ";  btMan4_6  -> Tag = (int)mcDSPNdlCrFwBw            ; btMan4_6  -> Visible = true  ;
    btMan4_7  -> Caption = "[" + AnsiString(mcDSPCycleManSlnChange  ) + "]" + "Move Sylinge Pos ";  btMan4_7  -> Tag = (int)mcDSPCycleManSlnChange    ; btMan4_7  -> Visible = true  ;
    btMan4_8  -> Caption = "[" + AnsiString(mcSTGCycleUp            ) + "]" + "CycleStageUp     ";  btMan4_8  -> Tag = (int)mcSTGCycleUp              ; btMan4_8  -> Visible = true  ;
    btMan4_9  -> Caption = "[" + AnsiString(mcSTGCycleDn            ) + "]" + "CycleStageDn     ";  btMan4_9  -> Tag = (int)mcSTGCycleDn              ; btMan4_9  -> Visible = true  ;
//    btMan4_10 -> Caption = "[" + AnsiString(mcDSPCycleNdlClean      ) + "]" + "CycleNdlClean    ";  btMan4_10 -> Tag = (int)mcDSPCycleNdlClean        ; btMan4_10 -> Visible = true  ;
    btMan4_11 -> Caption = "[" + AnsiString(mcDSPCycleVisnPos       ) + "]" + "CycleVisnPos     ";  btMan4_11 -> Tag = (int)mcDSPCycleManStdCal       ; btMan4_11 -> Visible = true  ;

    // UnLoader Front
  //btMan5_1  -> Caption = "[" + AnsiString(mcULD_FCycleWork        ) + "]" + "F Cycle Work "  );  btMan1_1  -> Tag = (int)mcULD_FCycleWork     ; btMan5_1  -> Visible = true  ;
  //btMan5_2  -> Caption = "[" + AnsiString(mcULD_FWorkSttBtm       ) + "]" + "F WorkStt Btm"  );  btMan1_2  -> Tag = (int)mcULD_FWorkSttBtm    ; btMan5_2  -> Visible = true  ;
  //btMan5_3  -> Caption = "[" + AnsiString(mcULD_FWorkSttTop       ) + "]" + "F WorkStt Top"  );  btMan1_3  -> Tag = (int)mcULD_FWorkSttTop    ; btMan5_3  -> Visible = true  ;
  //btMan5_4  -> Caption = "[" + AnsiString(mcULD_FPitchUp          ) + "]" + "F Pitch Up   "  );  btMan1_4  -> Tag = (int)mcULD_FPitchUp       ; btMan5_4  -> Visible = true  ;
  //btMan5_5  -> Caption = "[" + AnsiString(mcULD_FPitchDn          ) + "]" + "F Pitch Dn   "  );  btMan1_5  -> Tag = (int)mcULD_FPitchDn       ; btMan5_5  -> Visible = true  ;

  //UnLoader Rear
  //btMan5_8  -> Caption = "[" + AnsiString(mcULD_RCycleWork        ) + "]" + "R Cycle Work "  );  btMan1_8  -> Tag = (int)mcULD_RCycleWork     ; btMan5_8  -> Visible = true  ;
  //btMan5_9  -> Caption = "[" + AnsiString(mcULD_RWorkSttBtm       ) + "]" + "R WorkStt Btm"  );  btMan1_9  -> Tag = (int)mcULD_RWorkSttBtm    ; btMan5_9  -> Visible = true  ;
  //btMan5_10 -> Caption = "[" + AnsiString(mcULD_RWorkSttTop       ) + "]" + "R WorkStt Top"  );  btMan1_10 -> Tag = (int)mcULD_RWorkSttTop    ; btMan5_10 -> Visible = true  ;
  //btMan5_11 -> Caption = "[" + AnsiString(mcULD_RPitchUp          ) + "]" + "R Pitch Up   "  );  btMan1_11 -> Tag = (int)mcULD_RPitchUp       ; btMan5_11 -> Visible = true  ;
  //btMan5_12 -> Caption = "[" + AnsiString(mcULD_RPitchDn          ) + "]" + "R Pitch Dn   "  );  btMan1_12 -> Tag = (int)mcULD_RPitchDn       ; btMan5_12 -> Visible = true  ;

    btSTGVccm   -> Tag = (int)ySTG_Vccm ;
    btSTGEjct   -> Tag = (int)ySTG_Ejct ;

    btPCKVccmOn -> Tag = (int)yPCK_VccmOn ;
    btPCKVccmOff-> Tag = (int)yPCK_VccmOff;
    btPCKEjct   -> Tag = (int)yPCK_Ejct   ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmManual::tmUpdateTimer(TObject *Sender)
{
//Manual Enabled
    pnManual -> Enabled = !SEQ._bRun ;

    String sBtnName ;
    TBitBtn *  pBtn ;

    Trace("<@SETBUFF>","1,FormOperTimer 3");
    //메뉴얼 버튼 색깔 칠하기.
    for(int i = 1 ; i < 6/*페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;

/*
    btMan2_2  -> Caption = "[" + AnsiString(mcPRI_FIndxUpDn      ) + "]" + (bKorean ? "F PRI IndxUpDn "    : "F PRI IndxUpDn ");  btMan2_2  -> Tag = (int)mcPRI_FIndxUpDn      ; btMan2_2  -> Visible = true ;
    btMan2_5  -> Caption = "[" + AnsiString(mcIDX_FIndxUpDn      ) + "]" + (bKorean ? "F IDX IndxUpDn "    : "F IDX IndxUpDn ");  btMan2_5  -> Tag = (int)mcIDX_FIndxUpDn      ; btMan2_5  -> Visible = true ;
    btMan2_9  -> Caption = "[" + AnsiString(mcPRI_RIndxUpDn      ) + "]" + (bKorean ? "R PRI IndxUpDn "    : "R PRI IndxUpDn ");  btMan2_9  -> Tag = (int)mcPRI_RIndxUpDn      ; btMan2_9  -> Visible = true ;
    btMan2_12 -> Caption = "[" + AnsiString(mcIDX_RIndxUpDn      ) + "]" + (bKorean ? "R IDX IndxUpDn "    : "R IDX IndxUpDn ");  btMan2_12 -> Tag = (int)mcIDX_RIndxUpDn      ; btMan2_12 -> Visible = true ;
    btMan4_6  -> Caption = "[" + AnsiString(mcDSPNdlCrFwBw       ) + "]" + (bKorean ? "Niddle CleanFwBw" : "Niddle CleanFwBw" );  btMan4_6  -> Tag = (int)mcDSPCycleManStdCal       ; btMan4_6  -> Visible = true  ;
*/

            if(pBtn -> Name == "btMan1_1") {
                //pBtn -> Font -> Color = IO_GetY(yVTI_VTPower) ?  clLime : clBlack ;
            }
            else if(pBtn -> Name == "btMan2_2"){//mcPRI_FIndxUpDn
                pBtn -> Font -> Color = AT_Complete(aiPRI_FIndxUpDn , ccFwd) ? clLime : clBlack ;
            }
            else if(pBtn -> Name == "btMan2_5"){//mcIDX_FIndxUpDn
                pBtn -> Font -> Color = AT_Complete(aiIDX_FIndxUpDn , ccFwd) ? clLime : clBlack ;
            }
            else if(pBtn -> Name == "btMan2_9"){//mcPRI_RIndxUpDn
                pBtn -> Font -> Color = AT_Complete(aiIDX_RIndxUpDn , ccFwd) ? clLime : clBlack ;
            }
            else if(pBtn -> Name == "btMan2_12"){//mcIDX_RIndxUpDn
                pBtn -> Font -> Color = AT_Complete(aiIDX_RIndxUpDn , ccFwd) ? clLime : clBlack ;
            }
//            else if(pBtn -> Name == "btMan4_6"){//mcDSPNdlCrFwBw
//                pBtn -> Font -> Color = AT_Complete(aiDSP_NdlCrFwBw , ccFwd) ? clLime : clBlack ;
//            }
            else if(pBtn -> Name == "btMan4_7"){
                pBtn -> Font -> Color = MT_GetCmdPos(miDSP_YDispr) == 400 && MT_GetCmdPos(miDSP_XDispr) == 35 ? clLime : clBlack ;
            }
            else {
                if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
                else                             pBtn -> Font -> Color = clBlack ;
            }
        }
    }
    Trace("<@SETBUFF>","1,FormOperTimer 4");
}
//---------------------------------------------------------------------------

void __fastcall TFrmManual::btMan3_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmManual::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmManual::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------












