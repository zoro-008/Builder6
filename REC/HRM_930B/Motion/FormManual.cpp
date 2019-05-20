//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormManual.h"                                        

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "ManualMan.h"
#include "SLogUnit.h"
#include "DataMan.h"
#include "Stage.h"
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


void __fastcall TFrmManual::FormCreate(TObject *Sender)
{
    //���߿� �߰��Ǹ� Visible �ٲٱ�.

    btMan1_1  -> Caption = /*"[" + AnsiString(mcCycleLDR_IdxGet       ) + "]" +*/ "����δ� �Ⱦ� - �������� ���� �̼�"   ;  btMan1_1  -> Tag = (int)mcCycleLDR_IdxGet     ; btMan1_1  -> Visible = true ;
//    btMan1_2  -> Caption = "[" + AnsiString(mcRTLCycleMidRHeight       ) + "]" + "RTL CycleMidRHeight     "  ;  btMan5_1  -> Tag = (int)mcRTLCycleMidRHeight     ; btMan5_1  -> Visible = true  ;
//    btMan1_3  -> Caption = "[" + AnsiString(mcRTLCycleTrsfPick         ) + "]" + "RTL CycleTrsfPick       "  ;  btMan5_2  -> Tag = (int)mcRTLCycleTrsfPick       ; btMan5_2  -> Visible = true  ;
//    btMan1_4  -> Caption = /*"[" + AnsiString(mcLTLCycleMidToWork     ) + "]" +*/ "L �������� - ��ũ�� ���� �̼�"        ;  btMan1_4  -> Tag = (int)mcLTLCycleMidToWork   ; btMan1_4  -> Visible = true ;
//    btMan1_5  -> Caption = /*"[" + AnsiString(mcLTLCycleWorkOut       ) + "]" +*/ "L ��ũ�� - ��ũ�ƿ� ���� �̼�"        ;  btMan1_5  -> Tag = (int)mcLTLCycleWorkOut     ; btMan1_5  -> Visible = true ;
//    btMan1_6  -> Caption = /*"[" + AnsiString(mcRTLCycleWkoToPtl      ) + "]" +*/ "R ��ũ�ƿ� - ����Ʈ���� ���� �̼�"    ;  btMan1_6  -> Tag = (int)mcRTLCycleWkoToPtl    ; btMan1_6  -> Visible = true ;
    btMan1_7  -> Caption = /*"[" + AnsiString(mcCycleLDR_FPreGet        ) + "]" +*/ "����Ʈ�δ� - �������� ���� �̼�"    ;  btMan1_7  -> Tag = (int)mcCycleLDR_FPreGet    ; btMan1_7  -> Visible = true ;
//    btMan1_8  -> Caption = /*"[" + AnsiString(mcCycleLDR_FPreGet      ) + "]" +*/ "����Ʈ�δ� - �����ε��� ���� �̼�"    ;  btMan1_8  -> Tag = (int)mcCycleLDR_FPreGet    ; btMan1_8  -> Visible = true ;
//    btMan1_9  -> Caption = /*"[" + AnsiString(mcLTL_XDispRrCheck      ) + "]" +*/ "L �̵���� ���������̼� �˻�"         ;  btMan1_9  -> Tag = (int)mcLTLCycleMidLOri     ; btMan1_9  -> Visible = true ;
//    btMan1_10 -> Caption = /*"[" + AnsiString(mcLTL_XDispUVCheck      ) + "]" +*/ "L �̵���� ����� �˻�"               ;  btMan1_10 -> Tag = (int)mcLTLCycleMidLAlign   ; btMan1_10 -> Visible = true ;
//    btMan1_11 -> Caption = /*"[" + AnsiString(mcLTL_XVisnHtCheck      ) + "]" +*/ "L ���漭 �Ǹ��� Ŭ��"               ;  btMan1_11 -> Tag = (int)mcLTLCycleClean       ; btMan1_11 -> Visible = true ;
//    btMan1_12 -> Caption = /*"[" + AnsiString(mcLTL_XVisnCheck        ) + "]" +*/ "L ���漭 ����"                      ;  btMan1_12 -> Tag = (int)mcLTLCycleCmsLDisp    ; btMan1_12 -> Visible = true ;
//    btMan1_13 -> Caption = /*"[" + AnsiString(mcLDR_FHome             ) + "]" +*/ "L ��� UV ����"                       ;  btMan1_13 -> Tag = (int)mcLTLCycleTopLUV      ; btMan1_13 -> Visible = true ;
//    btMan1_14  -> Caption = /*"[" + AnsiString(mcCycleLDR_FPreGet     ) + "]" +*/ "����Ʈ�δ� - �����ε��� ���� �̼�"    ;  btMan1_14  -> Tag = (int)mcCycleLDR_FPreGet    ; btMan1_14  -> Visible = true ;

    btMan2_1  -> Caption = /*"[" + AnsiString(mcLTLCycleMidLOri       ) + "]" +*/ "�̵���� ���������̼�"                ;  btMan2_1  -> Tag = (int)mcLTLCycleMidLOri   ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Caption = /*"[" + AnsiString(mcLTLCycleMidLAlign     ) + "]" +*/ "�̵���� �����"                      ;  btMan2_2  -> Tag = (int)mcLTLCycleMidLAlign ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Caption = /*"[" + AnsiString(mcLTLCycleCmsLOri       ) + "]" +*/ "CMOS �������� ���������̼�"           ;  btMan2_3  -> Tag = (int)mcLTLCycleCmsLOri   ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Caption = /*"[" + AnsiString(mcLTLCycleCmsLDisp      ) + "]" +*/ "CMOS �������� �����"               ;  btMan2_4  -> Tag = (int)mcLTLCycleCmsLAlign ; btMan2_4  -> Visible = true  ;
//    btMan2_5  -> Caption = "[" + AnsiString(mcRTLCycleCmsRFixEnd    ) + "]" + "RTL CycleCmsRFixEnd     "  ;  btMan2_5  -> Tag = (int)mcRTLCycleCmsRFixEnd     ; btMan2_5  -> Visible = true  ;
//    btMan2_6  -> Caption = "[" + AnsiString(                        ) + "]" + "               ";  btMan2_6  -> Tag = (int) ; btMan2_6  -> Visible = true ;
    btMan2_7  -> Caption = /*"[" + AnsiString(mcLTLCycleMidToWork     ) + "]" +*/ "L �������� - ��ũ�� ���� �̼�"        ;  btMan2_7  -> Tag = (int)mcLTLCycleMidToWork ; btMan2_7  -> Visible = true ;
//    btMan2_8  -> Caption = "[" + AnsiString(                        ) + "]" + "               ";  btMan2_8  -> Tag = (int) ; btMan2_8  -> Visible = true ;
//    btMan2_9  -> Caption = "[" + AnsiString(                        ) + "]" + "               ";  btMan2_9  -> Tag = (int) ; btMan2_9  -> Visible = true ;
//    btMan2_10 -> Caption = "[" + AnsiString(                        ) + "]" + "               ";  btMan2_10 -> Tag = (int) ; btMan2_10 -> Visible = true ;
//    btMan2_11 -> Caption = "[" + AnsiString(                        ) + "]" + "               ";  btMan2_11 -> Tag = (int) ; btMan2_11 -> Visible = true ;
    btMan2_12 -> Caption = /*"[" + AnsiString(mcLTLCycleClean         ) + "]" +*/ "���漭 Ŭ��"                        ;  btMan2_12 -> Tag = (int)mcLTLCycleClean     ; btMan2_12 -> Visible = true ;  //���漭 Ŭ��
    btMan2_13 -> Caption = /*"[" + AnsiString(mcLTLCycleCmsLDisp      ) + "]" +*/ "���漭 ����"                        ;  btMan2_13 -> Tag = (int)mcLTLCycleCmsLDisp  ; btMan2_13 -> Visible = true ;  //���漭
    btMan2_14 -> Caption = /*"[" + AnsiString(mcLTLCycleTopLUV        ) + "]" +*/ "��� UV ����"                         ;  btMan2_14 -> Tag = (int)mcLTLCycleTopLUV    ; btMan2_14 -> Visible = true ; //��� ������


    btMan3_1  -> Caption = /*"[" + AnsiString(mcCycleLDR_IdxPlce      ) + "]" +*/ "�������� �Ⱦ� - ����δ� ���� �̼�"   ;  btMan3_1  -> Tag = (int)mcCycleLDR_IdxPlce  ; btMan3_1  -> Visible = true  ; //�������� ȸ��
//    btMan3_2  -> Caption = "[" + AnsiString(mcIDXCycleRotator       ) + "]" + "�������� �Ⱦ� - ����δ� ���� �̼�";  btMan3_2  -> Tag = (int)mcIDXCycleRotator         ; btMan3_2  -> Visible = true  ; //�������� - ����δ�
//  btMan3_3  -> Caption = "[" + AnsiString(mcPCKCycleAlignPick     ) + "]" + "CycleAlignPick   ";  btMan3_3  -> Tag = (int)mcPCKCycleAlignPick      ; btMan3_3  -> Visible = true  ;
//  btMan3_4  -> Caption = "[" + AnsiString(mcPCKCycleVisn          ) + "]" + "CycleVisn        ";  btMan3_4  -> Tag = (int)mcPCKCycleVisn           ; btMan3_4  -> Visible = true  ;
//  btMan3_5  -> Caption = "[" + AnsiString(mcPCKCycleAttach        ) + "]" + "CycleAttach      ";  btMan3_5  -> Tag = (int)mcPCKCycleAttach         ; btMan3_5  -> Visible = true  ;
//    btMan3_6  -> Caption = "[" + AnsiString(mcPCKCyclePush          ) + "]" + "CyclePush        ";  btMan3_6  -> Tag = (int)mcPCKCyclePush           ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = /*"[" + AnsiString(mcLTLCycleWorkOut       ) + "]" +*/ "L ��ũ�� - ��ũ�ƿ� ���� �̼�"        ;  btMan3_7  -> Tag = (int)mcLTLCycleWorkOut     ; btMan3_7  -> Visible = true ;
//    btMan3_8  -> Caption = /*"[" + AnsiString(mcPCKCycleManVsDefectPos) + "]" +*/ "�Ϻ� UV ����"                         ;  btMan3_8  -> Tag = (int)mc ; btMan3_8  -> Visible = true  ; //�Ϻ� ������
//  btMan3_9  -> Caption = "[" + AnsiString(mcPCKCycleManVsAlignPos ) + "]" + "Vision Align Pos ";  btMan3_9  -> Tag = (int)mcPCKCycleManVsAlignPos  ; btMan3_9  -> Visible = true  ;
//  btMan3_10 -> Caption = "[" + AnsiString(mcPCKCycleManVsAlignPos ) + "]" + "Vision Align Pos ";  btMan3_9  -> Tag = (int)mcPCKCycleManVsAlignPos  ; btMan3_9  -> Visible = true  ;
//  btMan3_11 -> Caption = "[" + AnsiString(mcPCKCycleManVsAlignPos ) + "]" + "Vision Align Pos ";  btMan3_9  -> Tag = (int)mcPCKCycleManVsAlignPos  ; btMan3_9  -> Visible = true  ;
//  btMan3_12 -> Caption = "[" + AnsiString(mcPCKCycleManVsAlignPos ) + "]" + "Vision Align Pos ";  btMan3_9  -> Tag = (int)mcPCKCycleManVsAlignPos  ; btMan3_9  -> Visible = true  ;
//  btMan3_13 -> Caption = "[" + AnsiString(mcPCKCycleManVsAlignPos ) + "]" + "Vision Align Pos ";  btMan3_9  -> Tag = (int)mcPCKCycleManVsAlignPos  ; btMan3_9  -> Visible = true  ;
//    btMan3_14  -> Caption = /*"[" + AnsiString(mcLTLCycleWorkOut       ) + "]" +*/ "L ��ũ�� - ��ũ�ƿ� ���� �̼�"        ;  btMan3_14  -> Tag = (int)mcLTLCycleWorkOut     ; btMan3_14  -> Visible = true ;


    btMan4_1  -> Caption = /*"[" + AnsiString(mcRTLCycleTrsfPick     ) + "]" +*/ "Ʈ������ ��(�������� ȸ��)"              ;  btMan4_1  -> Tag = (int)mcRTLCycleTrsfPick    ; btMan4_1  -> Visible = true  ; //Ʈ������ ��
    btMan4_2  -> Caption = /*"[" + AnsiString(mcRTLCycleTrsfPlace    ) + "]" +*/ "Ʈ������ �÷��̽�"                       ;  btMan4_2  -> Tag = (int)mcRTLCycleTrsfPlace   ; btMan4_2  -> Visible = true  ; //Ʈ������ �÷��̽�
//    btMan4_3  -> Caption = "[" + AnsiString(mcLTLCycleClean        ) + "]" + "LTL CycleClean      ";  btMan4_3  -> Tag = (int)mcLTLCycleClean        ; btMan4_3  -> Visible = true  ;
//    btMan4_4  -> Caption = "[" + AnsiString(mcLTLCycleMidLOri      ) + "]" + "LTL CycleMidLOri    ";  btMan4_4  -> Tag = (int)mcLTLCycleMidLOri      ; btMan4_4  -> Visible = true  ;
//    btMan4_5  -> Caption = "[" + AnsiString(mcLTLCycleMidLAlign    ) + "]" + "LTL CycleMidLAlign  ";  btMan4_5  -> Tag = (int)mcLTLCycleMidLAlign    ; btMan4_5  -> Visible = true  ;
//    btMan4_6  -> Caption = "[" + AnsiString(mcLTLCycleTopLUV       ) + "]" + "LTL CycleTopLUV     ";  btMan4_6  -> Tag = (int)mcLTLCycleTopLUV       ; btMan4_6  -> Visible = true  ;
    btMan4_7  -> Caption = /*"[" + AnsiString(mcRTLCycleWkoToPtl     ) + "]" +*/ "R ��ũ�ƿ� - ����Ʈ���� ���� �̼�"       ;  btMan4_7  -> Tag = (int)mcRTLCycleWkoToPtl    ; btMan4_7  -> Visible = true ;
//    btMan4_8  -> Caption = "[" + AnsiString(mcLTLCycleCmsLAlign    ) + "]" + "LTL CycleCmsLAlign  ";  btMan4_8  -> Tag = (int)mcLTLCycleCmsLAlign    ; btMan4_8  -> Visible = true  ;
    btMan4_9  -> Caption = /*"[" + AnsiString(mcRTLCycleMidRHeight   ) + "]" +*/ "�̵���� ���� ����"                      ;  btMan4_9 -> Tag = (int)mcRTLCycleMidRHeight        ; btMan4_9  -> Visible = true  ;
    btMan4_10 -> Caption = /*"[" + AnsiString(mcLTLCycleHtReZero     ) + "]" +*/ "CMOS ���� ����"                          ;  btMan4_10 -> Tag = (int)mcRTLCycleCmsRHeight       ; btMan4_10 -> Visible = true  ;
    btMan4_11 -> Caption = /*"[" + AnsiString(mcRTLCycleCmsRHeight   ) + "]" +*/ "CMOS ���� ����"                          ;  btMan4_11 -> Tag = (int)mcRTLCycleCmsRZHeight      ; btMan4_11 -> Visible = true  ;
    btMan4_12 -> Caption = /*"[" + AnsiString(mcRTLCycleMidREndHeight) + "]" +*/ "��ȭ �� ���� ����"                        ;  btMan4_12 -> Tag = (int)mcRTLCycleMidREndHeight    ; btMan4_12 -> Visible = true  ;                    //�ø� ����
    btMan4_13 -> Caption = /*"[" + AnsiString(mcRTLCycleMidREndHeight) + "]" +*/ "��ȭ �� ���� ����"                        ;  btMan4_13 -> Tag = (int)mcRTLCycleMidREndHeight    ; btMan4_13 -> Visible = true  ;
    btMan4_14 -> Caption = /*"[" + AnsiString(mcRTLCycleMidREndHeight) + "]" +*/ "���� ���� ����"                           ;  btMan4_14 -> Tag = (int)mcRTLCycleMidREndHeight    ; btMan4_14 -> Visible = true  ;


    btMan5_1  -> Caption = /*"[" + AnsiString(mcCycleManualEndHeight  ) + "]" +*/ "����ǰ ���� ����\n""(�������ۿ� ��ǰ ���� Ŭ��)";  btMan5_1  -> Tag = (int)mcCycleManualEndHeight    ; btMan5_1  -> Visible = true ;
//    btMan5_2  -> Caption = /*"[" + AnsiString(mcCycleManualEndHeight) + "]" +*/ "����ǰ ��������"                      ;  btMan5_2  -> Tag = (int)mcCycleManualEndHeight; btMan5_2  -> Visible = true ;
//    btMan5_3  -> Caption = "[" + AnsiString(mcRTLCycleTrsfPlace     ) + "]" + "RTL CycleTrsfPlace      "  ;  btMan5_3  -> Tag = (int)mcRTLCycleTrsfPlace      ; btMan5_3  -> Visible = true  ;
//    btMan5_4  -> Caption = "[" + AnsiString(mcRTLCycleCmsRHeight    ) + "]" + "RTL CycleCmsRHeight     "  ;  btMan5_4  -> Tag = (int)mcRTLCycleCmsRHeight     ; btMan5_4  -> Visible = true  ;
//    btMan5_5  -> Caption = "[" + AnsiString(mcRTLCycleCmsRAlign     ) + "]" + "RTL CycleCmsRAlign      "  ;  btMan5_5  -> Tag = (int)mcRTLCycleCmsRAlign      ; btMan5_5  -> Visible = true  ;
//  btMan5_6  -> Caption = "[" + AnsiString(mcRTLCycleCmsRGap         ) + "]" + "RTL CycleCmsRGap        "  ;  btMan5_6  -> Tag = (int)mcRTLCycleCmsRGap        ; btMan5_6  -> Visible = true  ;
    btMan5_7  -> Caption = /*"[" + AnsiString(mcCyclePstRailULDPlce   ) + "]" +*/ "R ����Ʈ���� - ��δ� ���� �̼�"      ;  btMan5_7  -> Tag = (int)mcCyclePstRailULDPlce ; btMan5_7  -> Visible = true ;
//    btMan5_8  -> Caption = "[" + AnsiString(mcRTLCycleMidREndHeight ) + "]" + "RTL CycleMidREndHeight  "  ;  btMan5_8  -> Tag = (int)mcRTLCycleMidREndHeight  ; btMan5_8  -> Visible = true  ;
//    btMan5_9  -> Caption = "[" + AnsiString(mcRTLCycleOut           ) + "]" + "RTL CycleOut            "  ;  btMan5_9  -> Tag = (int)mcRTLCycleOut            ; btMan5_9  -> Visible = true  ;
//    btMan5_10 -> Caption = "[" + AnsiString(mcRTLCycleHtReZero      ) + "]" + "RTL CycleHtReZero       "  ;  btMan5_10  -> Tag = (int)mcRTLCycleHtReZero       ; btMan5_10 -> Visible = true  ;
    btMan5_11 -> Caption = /*"[" + AnsiString(mcRTLCycleCmsRAlign     ) + "]" +*/ "����Ʈ ���� ���� �����"              ;  btMan5_11  -> Tag = (int)mcRTLCycleCmsRAlign       ; btMan5_11 -> Visible = true  ;
    btMan5_12 -> Caption = /*"[" + AnsiString(mcRTLCycleCmsRAlign     ) + "]" +*/ "����Ʈ ���� ���� �����"              ;  btMan5_12  -> Tag = (int)mcRTLCycleCmsRAlign       ; btMan5_12 -> Visible = true  ;
    btMan5_13 -> Caption = /*"[" + AnsiString(mcRTLCycleCmsREndAlign  ) + "]" +*/ "��ȭ �� ���� �����"                  ;  btMan5_13  -> Tag = (int)mcRTLCycleCmsREndAlign    ; btMan5_13 -> Visible = true  ;
    btMan5_14 -> Caption = /*"[" + AnsiString(mcRTLCycleCmsREndAlign  ) + "]" +*/ "��ȭ �� ���� �����"                  ;  btMan5_14  -> Tag = (int)mcRTLCycleCmsREndAlign    ; btMan5_14 -> Visible = true  ;

//  btMan6_1  -> Caption = "[" + AnsiString(mcULDCyclePick             ) + "]" + "ULD CyclePick           "  ;  btMan6_1  -> Tag = (int)mcULDCyclePick    ; btMan6_1  -> Visible = true  ;
//  btMan6_2  -> Caption = "[" + AnsiString(mcULDCyclePlce             ) + "]" + "ULD CyclePlce           "  ;  btMan6_2  -> Tag = (int)mcULDCyclePlce    ; btMan6_2  -> Visible = true  ;
//  btMan6_14 -> Caption = "[" + AnsiString(mcULD_Home                 ) + "]" + "ULD Home                "  ;  btMan6_14 -> Tag = (int)mcULD_Home        ; btMan6_14 -> Visible = true  ;



//    btSTGVccm   -> Tag = (int)ySTG_Vccm ;
//    btSTGEjct   -> Tag = (int)ySTG_Ejct ;

//    btPCKVccmOn -> Tag = (int)yPCK_VccmOn ;
//    btPCKVccmOff-> Tag = (int)yPCK_VccmOff;
//    btPCKEjct   -> Tag = (int)yPCK_Ejct   ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmManual::tmUpdateTimer(TObject *Sender)
{
//Manual Enabled
    pnManual -> Enabled = !SEQ._bRun ;

    String sBtnName ;
    TBitBtn *  pBtn ;

    Trace("<@SETBUFF>","1,FormOperTimer 3");
    //�޴��� ��ư ���� ĥ�ϱ�.
    for(int i = 1 ; i < 6/*������6��*/ ; i++) {
        for(int j = 1 ; j < 15 /*��ư14��*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;

    int r , c ;

    //�Ŵ��� ��ư ���� ����Ŭ �����̰� ��
    if(DM.ARAY[riLDR_F].GetCntStat(csUnkwn) &&
       DM.ARAY[riPRL].GetCntStat(csNone) &&
       DM.ARAY[riSTG].GetCntStat(csNone) &&
       DM.ARAY[riWKO].GetCntStat(csNone) &&
       DM.ARAY[riPTL].GetCntStat(csNone)){
        btMan1_7 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan1_7 -> Font -> Color = clBlack  ;
    }


    if(DM.ARAY[riPRL].GetCntStat(csUnkwn)){
        btMan2_7 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan2_7 -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riSTG].GetCntStat(csWorkEnd)){
        btMan3_7 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan3_7 -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riWKO].GetCntStat(csWork)){
        btMan4_7 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan4_7 -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riPTL].GetCntStat(csWork)){
        btMan5_7 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan5_7 -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riLDR_R].GetCntStat(csUnkwn) &&                           //������� Ȯ���ؾ���
       DM.ARAY[riRTT].GetCntStat(csNone)){
        btMan1_1 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan1_1 -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riRTT].GetCntStat(csCmsLOri)){
        btMan2_3 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan2_3 -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riLDR_R].GetCntStat(csEmpty) &&
       DM.ARAY[riRTT].GetCntStat(csCmsLOri)){
        btMan3_1   -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan3_1   -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riSTG].GetCntStat(csMidLOri)){
        btMan2_1   -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan2_1   -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riSTG].GetCntStat(csMidLAlign)){
        btMan2_2   -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan2_2   -> Font -> Color = clBlack  ;
    }

    if(DM.ARAY[riSTG].GetCntStat(csMidRHeight)){
        btMan4_9   -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
    }
    else {
        btMan4_9   -> Font -> Color = clBlack  ;
    }

    if(STG.GetWorkingRC(r,c)){
        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsLDisp1 || DM.ARAY[riSTG].GetStat(r,c)==csCmsLDisp2){
            btMan2_12  -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
            btMan2_13  -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan2_12  -> Font -> Color = clBlack  ;
            btMan2_13  -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRNeed &&
           DM.ARAY[riRTT].GetCntStat(csAttach)) {
            btMan4_1  -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_1  -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRNeed &&
           DM.ARAY[riTRF].GetCntStat(csAttach)) {
            btMan4_2  -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_2  -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsLAlign) {
            btMan2_4  -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan2_4  -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRPreAlign) {
            btMan5_11 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan5_11 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRPreAlign) {
            btMan5_11 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan5_11 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRHeight) {
            btMan4_10 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_10 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRAlign) {
            btMan5_12 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan5_12 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRZHeight) {
            btMan4_11 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_11 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsREndAlign) {
            btMan5_13 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan5_13 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsLTopUV1 || DM.ARAY[riSTG].GetStat(r,c)==csCmsLTopUV2) {
            btMan2_14 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan2_14 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsREndHeight) {
            btMan4_12 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_12 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRFixAlign) {
            btMan5_14 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan5_14 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csCmsRFixHeight) {
            btMan4_13 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_13 -> Font -> Color = clBlack  ;
        }

        if(DM.ARAY[riSTG].GetStat(r,c)==csMidREndHeight) {
            btMan4_14 -> Font -> Color = SEQ._bFlick ? clBlack : clLime ;
        }
        else {
            btMan4_14 -> Font -> Color = clBlack  ;
        }
    }
    else {
        btMan2_12  -> Font -> Color = clBlack  ;
        btMan2_13  -> Font -> Color = clBlack  ;
    }





















































/*
    btMan2_2  -> Caption = "[" + AnsiString(mcPRI_FIndxUpDn      ) + "]" + (bKorean ? "F PRI IndxUpDn "    : "F PRI IndxUpDn ");  btMan2_2  -> Tag = (int)mcPRI_FIndxUpDn      ; btMan2_2  -> Visible = true ;
    btMan2_5  -> Caption = "[" + AnsiString(mcIDX_FIndxUpDn      ) + "]" + (bKorean ? "F IDX IndxUpDn "    : "F IDX IndxUpDn ");  btMan2_5  -> Tag = (int)mcIDX_FIndxUpDn      ; btMan2_5  -> Visible = true ;
    btMan2_9  -> Caption = "[" + AnsiString(mcPRI_RIndxUpDn      ) + "]" + (bKorean ? "R PRI IndxUpDn "    : "R PRI IndxUpDn ");  btMan2_9  -> Tag = (int)mcPRI_RIndxUpDn      ; btMan2_9  -> Visible = true ;
    btMan2_12 -> Caption = "[" + AnsiString(mcIDX_RIndxUpDn      ) + "]" + (bKorean ? "R IDX IndxUpDn "    : "R IDX IndxUpDn ");  btMan2_12 -> Tag = (int)mcIDX_RIndxUpDn      ; btMan2_12 -> Visible = true ;
    btMan4_6  -> Caption = "[" + AnsiString(mcDSPNdlCrFwBw       ) + "]" + (bKorean ? "Niddle CleanFwBw" : "Niddle CleanFwBw" );  btMan4_6  -> Tag = (int)mcDSPCycleManStdCal       ; btMan4_6  -> Visible = true  ;
*/

//            if(pBtn -> Name == "btMan1_1") {
//                //pBtn -> Font -> Color = IO_GetY(yVTI_VTPower) ?  clLime : clBlack ;
//            }
//            else if(pBtn -> Name == "btMan2_2"){//mcPRI_FIndxUpDn
//                pBtn -> Font -> Color = AT_Complete(aiPRI_FIndxUpDn , ccFwd) ? clLime : clBlack ;
//            }
//            else if(pBtn -> Name == "btMan2_5"){//mcIDX_FIndxUpDn
//                pBtn -> Font -> Color = AT_Complete(aiIDX_FIndxUpDn , ccFwd) ? clLime : clBlack ;
//            }
//            else if(pBtn -> Name == "btMan2_9"){//mcPRI_RIndxUpDn
//                pBtn -> Font -> Color = AT_Complete(aiIDX_RIndxUpDn , ccFwd) ? clLime : clBlack ;
//            }
//            else if(pBtn -> Name == "btMan2_12"){//mcIDX_RIndxUpDn
//                pBtn -> Font -> Color = AT_Complete(aiIDX_RIndxUpDn , ccFwd) ? clLime : clBlack ;
//            }
//            else if(pBtn -> Name == "btMan4_6"){//mcDSPNdlCrFwBw
//                pBtn -> Font -> Color = AT_Complete(aiDSP_NdlCrFwBw , ccFwd) ? clLime : clBlack ;
//            }
//            else if(pBtn -> Name == "btMan4_7"){
//                pBtn -> Font -> Color = MT_GetCmdPos(miDSP_YDispr) == 400 && MT_GetCmdPos(miDSP_XDispr) == 35 ? clLime : clBlack ;
//            }
//            else {
//                if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
//                else                             pBtn -> Font -> Color = clBlack ;
//            }
        }
    }
    Trace("<@SETBUFF>","1,FormOperTimer 4");
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
















void __fastcall TFrmManual::btMan1_8Click(TObject *Sender)
{
    TSpeedButton * Button = (TSpeedButton*)Sender ;
    String sTemp = Button -> Caption + " ������ �����մϱ�?" ;

    if(FM_MsgYesNo("Confirm" ,sTemp.c_str()) != mrYes) return ;


    int iManNo = Button -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
