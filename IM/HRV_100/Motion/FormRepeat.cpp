//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormRepeat.h"
#include "FormMain.h"
#include "Stage.h"
#include "Sequence.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmRepeat *FrmRepeat;
//---------------------------------------------------------------------------
__fastcall TFrmRepeat::TFrmRepeat(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmRepeat::btFullRepeatClick(TObject *Sender)
{
    STG.m_iRepeatCnt = StrToIntDef(Edit1->Text , 0);
    STG.m_eRepeatType = CStage::rtFullRepeat ;
    SEQ._bBtnStart = true;
}
//---------------------------------------------------------------------------


void __fastcall TFrmRepeat::btStopClick(TObject *Sender)
{
    STG.m_iRepeatCnt=0 ;
    STG.m_eRepeatType = CStage::rtNone ;
    SEQ._bBtnStop =true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmRepeat::btCrntRepeatClick(TObject *Sender)
{
    STG.m_iRepeatCnt     = StrToIntDef(Edit1->Text , 0);
    STG.m_eRepeatType    = CStage::rtCurrentRepeat ;
    //STG.m_iRepeatCnt-- ;
    SEQ._bBtnStart = true;
}
//---------------------------------------------------------------------------


void __fastcall TFrmRepeat::Timer1Timer(TObject *Sender)
{
    lbCntLeft -> Caption = STG.m_iRepeatCnt ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmRepeat::btPinRepeatClick(TObject *Sender)
{
    STG.m_iRepeatCnt     = StrToIntDef(Edit1->Text , 0);
    STG.m_eRepeatType    = CStage::rtPinRepeat ;
    //STG.m_iRepeatCnt-- ;
    SEQ._bBtnStart = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmRepeat::btChcukPinRepeatClick(TObject *Sender)
{
    STG.m_iRepeatCnt     = StrToIntDef(Edit1->Text , 0);
    STG.m_eRepeatType    = CStage::rtChuckPinRepeat ;
    //STG.m_iRepeatCnt-- ;
    SEQ._bBtnStart = true;
}
//---------------------------------------------------------------------------

