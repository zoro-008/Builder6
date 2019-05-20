//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperator.h"

#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Rs232Man.h"
#include "SLogUnit.h"
#include "SpcSubUnit.h"
#include "SpcUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
//#include "Loader.h"
//#include "Rail.h"
//#include "UnLoader.h"
#include "Picker.h"
//#include "Sorter.h"
#include "Supply.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmOperator *FrmOperator;
//---------------------------------------------------------------------------
__fastcall TFrmOperator::TFrmOperator(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperator::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;

    cbBnL0Sel -> ItemIndex = OM.DevOptn.iBnLSel[0] ; cbBnR0Sel -> ItemIndex = OM.DevOptn.iBnRSel[0] ;
    cbBnL1Sel -> ItemIndex = OM.DevOptn.iBnLSel[1] ; cbBnR1Sel -> ItemIndex = OM.DevOptn.iBnRSel[1] ;
    cbBnL2Sel -> ItemIndex = OM.DevOptn.iBnLSel[2] ; cbBnR2Sel -> ItemIndex = OM.DevOptn.iBnRSel[2] ;
    cbBnL3Sel -> ItemIndex = OM.DevOptn.iBnLSel[3] ; cbBnR3Sel -> ItemIndex = OM.DevOptn.iBnRSel[3] ;
    cbBnL4Sel -> ItemIndex = OM.DevOptn.iBnLSel[4] ; cbBnR4Sel -> ItemIndex = OM.DevOptn.iBnRSel[4] ;
    cbBnL5Sel -> ItemIndex = OM.DevOptn.iBnLSel[5] ; cbBnR5Sel -> ItemIndex = OM.DevOptn.iBnRSel[5] ;
    cbBnL6Sel -> ItemIndex = OM.DevOptn.iBnLSel[6] ; cbBnR6Sel -> ItemIndex = OM.DevOptn.iBnRSel[6] ;
    cbBnL7Sel -> ItemIndex = OM.DevOptn.iBnLSel[7] ; cbBnR7Sel -> ItemIndex = OM.DevOptn.iBnRSel[7] ;
    cbBnL8Sel -> ItemIndex = OM.DevOptn.iBnLSel[8] ; cbBnR8Sel -> ItemIndex = OM.DevOptn.iBnRSel[8] ;
    cbBnL9Sel -> ItemIndex = OM.DevOptn.iBnLSel[9] ; cbBnR9Sel -> ItemIndex = OM.DevOptn.iBnRSel[9] ;

    ctBinCnt -> DoubleBuffered = true ;
    ctDist   -> DoubleBuffered = true ;

    pnLBLim0 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[0] ] ;  pnRBLim0 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[0] ] ;
    pnLBLim1 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[1] ] ;  pnRBLim1 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[1] ] ;
    pnLBLim2 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[2] ] ;  pnRBLim2 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[2] ] ;
    pnLBLim3 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[3] ] ;  pnRBLim3 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[3] ] ;
    pnLBLim4 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[4] ] ;  pnRBLim4 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[4] ] ;
    pnLBLim5 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[5] ] ;  pnRBLim5 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[5] ] ;
    pnLBLim6 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[6] ] ;  pnRBLim6 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[6] ] ;
    pnLBLim7 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[7] ] ;  pnRBLim7 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[7] ] ;
    pnLBLim8 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[8] ] ;  pnRBLim8 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[8] ] ;
    pnLBLim9 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[9] ] ;  pnRBLim9 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[9] ] ;



}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperator::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperator::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
    //FM_Update();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    int iLevel;
    static int iPreLevel = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    switch (iLevel) {
        case lvOperator : if( iPreLevel != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iPreLevel = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iPreLevel != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iPreLevel = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iPreLevel != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iPreLevel = lvMaster ;
                          }
                          break ;
    }


    String sTemp ;
    String sCaptionName[MAX_GRADE_STAT] ;
    sCaptionName[gsGradeA] = "Grade A";
    sCaptionName[gsGradeB] = "Grade B";
    sCaptionName[gsGradeC] = "Grade C";
    sCaptionName[gsGradeD] = "Grade D";
    sCaptionName[gsGradeE] = "Grade E";
    sCaptionName[gsGradeF] = "Grade F";
    sCaptionName[gsGradeG] = "Grade G";
    sCaptionName[gsGradeH] = "Grade H";
    sCaptionName[gsGradeI] = "Grade I";
    sCaptionName[gsGradeJ] = "Grade J";
    sCaptionName[gsFail  ] = "FAIL";
    sCaptionName[gsBuffer] = "Buffer ";   //+ A,B,C ...
    sCaptionName[gsCFail ] = "Chuck FAIL";

    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[0]] ; if(OM.DevOptn.iBnLIdx[0] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[0]];pnLBLim0 -> Caption = sTemp ; pnLBCnt0 -> Caption = SPL.GetPkgCnt(0);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[1]] ; if(OM.DevOptn.iBnLIdx[1] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[1]];pnLBLim1 -> Caption = sTemp ; pnLBCnt1 -> Caption = SPL.GetPkgCnt(1);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[2]] ; if(OM.DevOptn.iBnLIdx[2] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[2]];pnLBLim2 -> Caption = sTemp ; pnLBCnt2 -> Caption = SPL.GetPkgCnt(2);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[3]] ; if(OM.DevOptn.iBnLIdx[3] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[3]];pnLBLim3 -> Caption = sTemp ; pnLBCnt3 -> Caption = SPL.GetPkgCnt(3);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[4]] ; if(OM.DevOptn.iBnLIdx[4] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[4]];pnLBLim4 -> Caption = sTemp ; pnLBCnt4 -> Caption = SPL.GetPkgCnt(4);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[5]] ; if(OM.DevOptn.iBnLIdx[5] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[5]];pnLBLim5 -> Caption = sTemp ; pnLBCnt5 -> Caption = SPL.GetPkgCnt(5);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[6]] ; if(OM.DevOptn.iBnLIdx[6] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[6]];pnLBLim6 -> Caption = sTemp ; pnLBCnt6 -> Caption = SPL.GetPkgCnt(6);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[7]] ; if(OM.DevOptn.iBnLIdx[7] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[7]];pnLBLim7 -> Caption = sTemp ; pnLBCnt7 -> Caption = SPL.GetPkgCnt(7);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[8]] ; if(OM.DevOptn.iBnLIdx[8] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[8]];pnLBLim8 -> Caption = sTemp ; pnLBCnt8 -> Caption = SPL.GetPkgCnt(8);
    sTemp = sCaptionName[OM.DevOptn.iBnLIdx[9]] ; if(OM.DevOptn.iBnLIdx[9] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnLSel[9]];pnLBLim9 -> Caption = sTemp ; pnLBCnt9 -> Caption = SPL.GetPkgCnt(9);

    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[0]] ; if(OM.DevOptn.iBnRIdx[0] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[0]];pnRBLim0 -> Caption = sTemp ; pnRBCnt0 -> Caption = SPR.GetPkgCnt(0);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[1]] ; if(OM.DevOptn.iBnRIdx[1] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[1]];pnRBLim1 -> Caption = sTemp ; pnRBCnt1 -> Caption = SPR.GetPkgCnt(1);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[2]] ; if(OM.DevOptn.iBnRIdx[2] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[2]];pnRBLim2 -> Caption = sTemp ; pnRBCnt2 -> Caption = SPR.GetPkgCnt(2);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[3]] ; if(OM.DevOptn.iBnRIdx[3] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[3]];pnRBLim3 -> Caption = sTemp ; pnRBCnt3 -> Caption = SPR.GetPkgCnt(3);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[4]] ; if(OM.DevOptn.iBnRIdx[4] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[4]];pnRBLim4 -> Caption = sTemp ; pnRBCnt4 -> Caption = SPR.GetPkgCnt(4);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[5]] ; if(OM.DevOptn.iBnRIdx[5] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[5]];pnRBLim5 -> Caption = sTemp ; pnRBCnt5 -> Caption = SPR.GetPkgCnt(5);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[6]] ; if(OM.DevOptn.iBnRIdx[6] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[6]];pnRBLim6 -> Caption = sTemp ; pnRBCnt6 -> Caption = SPR.GetPkgCnt(6);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[7]] ; if(OM.DevOptn.iBnRIdx[7] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[7]];pnRBLim7 -> Caption = sTemp ; pnRBCnt7 -> Caption = SPR.GetPkgCnt(7);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[8]] ; if(OM.DevOptn.iBnRIdx[8] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[8]];pnRBLim8 -> Caption = sTemp ; pnRBCnt8 -> Caption = SPR.GetPkgCnt(8);
    sTemp = sCaptionName[OM.DevOptn.iBnRIdx[9]] ; if(OM.DevOptn.iBnRIdx[9] == (int)gsBuffer) sTemp = "B " + sCaptionName[OM.DevOptn.iBnRSel[9]];pnRBLim9 -> Caption = sTemp ; pnRBCnt9 -> Caption = SPR.GetPkgCnt(9);
/*
    for(int i=0; i<MAX_BIN_CNT; i++) {
        if(OM.DevOptn.iBnLIdx[i] == (int)gsBuffer) {
            switch(i) {
                case 0 : pnLBLim0 -> Color = clNavy ; break ;
                case 1 : pnLBLim1 -> Color = clNavy ; break ;
                case 2 : pnLBLim2 -> Color = clNavy ; break ;
                case 3 : pnLBLim3 -> Color = clNavy ; break ;
                case 4 : pnLBLim4 -> Color = clNavy ; break ;
                case 5 : pnLBLim5 -> Color = clNavy ; break ;
                case 6 : pnLBLim6 -> Color = clNavy ; break ;
                case 7 : pnLBLim7 -> Color = clNavy ; break ;
                case 8 : pnLBLim8 -> Color = clNavy ; break ;
                case 9 : pnLBLim9 -> Color = clNavy ; break ;
            }
        }
        else {
            switch(i) {
                case 0 : pnLBLim0 -> Color = clBlack ; break ;
                case 1 : pnLBLim1 -> Color = clBlack ; break ;
                case 2 : pnLBLim2 -> Color = clBlack ; break ;
                case 3 : pnLBLim3 -> Color = clBlack ; break ;
                case 4 : pnLBLim4 -> Color = clBlack ; break ;
                case 5 : pnLBLim5 -> Color = clBlack ; break ;
                case 6 : pnLBLim6 -> Color = clBlack ; break ;
                case 7 : pnLBLim7 -> Color = clBlack ; break ;
                case 8 : pnLBLim8 -> Color = clBlack ; break ;
                case 9 : pnLBLim9 -> Color = clBlack ; break ;
            }
        }

        if(OM.DevOptn.iBnRIdx[i] == (int)gsBuffer) {
            switch(i) {
                case 0 : pnRBLim0 -> Color = clNavy ; break ;
                case 1 : pnRBLim1 -> Color = clNavy ; break ;
                case 2 : pnRBLim2 -> Color = clNavy ; break ;
                case 3 : pnRBLim3 -> Color = clNavy ; break ;
                case 4 : pnRBLim4 -> Color = clNavy ; break ;
                case 5 : pnRBLim5 -> Color = clNavy ; break ;
                case 6 : pnRBLim6 -> Color = clNavy ; break ;
                case 7 : pnRBLim7 -> Color = clNavy ; break ;
                case 8 : pnRBLim8 -> Color = clNavy ; break ;
                case 9 : pnRBLim9 -> Color = clNavy ; break ;
            }
        }
        else {
            switch(i) {
                case 0 : pnRBLim0 -> Color = clBlack ; break ;
                case 1 : pnRBLim1 -> Color = clBlack ; break ;
                case 2 : pnRBLim2 -> Color = clBlack ; break ;
                case 3 : pnRBLim3 -> Color = clBlack ; break ;
                case 4 : pnRBLim4 -> Color = clBlack ; break ;
                case 5 : pnRBLim5 -> Color = clBlack ; break ;
                case 6 : pnRBLim6 -> Color = clBlack ; break ;
                case 7 : pnRBLim7 -> Color = clBlack ; break ;
                case 8 : pnRBLim8 -> Color = clBlack ; break ;
                case 9 : pnRBLim9 -> Color = clBlack ; break ;
            }
        }
    }
*/

    cbBnL0Sel -> Visible = OM.DevOptn.iBnLIdx[0] == (int)gsBuffer ; cbBnR0Sel -> Visible = OM.DevOptn.iBnRIdx[0] == (int)gsBuffer ;
    cbBnL1Sel -> Visible = OM.DevOptn.iBnLIdx[1] == (int)gsBuffer ; cbBnR1Sel -> Visible = OM.DevOptn.iBnRIdx[1] == (int)gsBuffer ;
    cbBnL2Sel -> Visible = OM.DevOptn.iBnLIdx[2] == (int)gsBuffer ; cbBnR2Sel -> Visible = OM.DevOptn.iBnRIdx[2] == (int)gsBuffer ;
    cbBnL3Sel -> Visible = OM.DevOptn.iBnLIdx[3] == (int)gsBuffer ; cbBnR3Sel -> Visible = OM.DevOptn.iBnRIdx[3] == (int)gsBuffer ;
    cbBnL4Sel -> Visible = OM.DevOptn.iBnLIdx[4] == (int)gsBuffer ; cbBnR4Sel -> Visible = OM.DevOptn.iBnRIdx[4] == (int)gsBuffer ;
    cbBnL5Sel -> Visible = OM.DevOptn.iBnLIdx[5] == (int)gsBuffer ; cbBnR5Sel -> Visible = OM.DevOptn.iBnRIdx[5] == (int)gsBuffer ;
    cbBnL6Sel -> Visible = OM.DevOptn.iBnLIdx[6] == (int)gsBuffer ; cbBnR6Sel -> Visible = OM.DevOptn.iBnRIdx[6] == (int)gsBuffer ;
    cbBnL7Sel -> Visible = OM.DevOptn.iBnLIdx[7] == (int)gsBuffer ; cbBnR7Sel -> Visible = OM.DevOptn.iBnRIdx[7] == (int)gsBuffer ;
    cbBnL8Sel -> Visible = OM.DevOptn.iBnLIdx[8] == (int)gsBuffer ; cbBnR8Sel -> Visible = OM.DevOptn.iBnRIdx[8] == (int)gsBuffer ;
    cbBnL9Sel -> Visible = OM.DevOptn.iBnLIdx[9] == (int)gsBuffer ; cbBnR9Sel -> Visible = OM.DevOptn.iBnRIdx[9] == (int)gsBuffer ;

/*
//    sTemp = "BUFFER " + (String)OM.DevOptn.iBn6Sel                          ; pnLBLim6 -> Caption = sTemp ; pnLBCnt6 -> Caption = SPL.GetPkgCnt(6);
//    sTemp = "BUFFER " + (String)OM.DevOptn.iBn7Sel                          ; pnLBLim7 -> Caption = sTemp ; pnLBCnt7 -> Caption = SPL.GetPkgCnt(7);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn8Sel                          ; pnLBLim8 -> Caption = sTemp ; pnLBCnt8 -> Caption = SPL.GetPkgCnt(8);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn9Sel                          ; pnLBLim9 -> Caption = sTemp ; pnLBCnt9 -> Caption = SPL.GetPkgCnt(9);

    sTemp = "FAIL"                                                          ; pnRBLim0 -> Caption = sTemp ; pnRBCnt0 -> Caption = SPR.GetPkgCnt(0);
    sTemp = (String)OM.DevOptn.dMinBn1 + " ~ " + (String)OM.DevOptn.dMaxBn1 ; pnRBLim1 -> Caption = sTemp ; pnRBCnt1 -> Caption = SPR.GetPkgCnt(1);
    sTemp = (String)OM.DevOptn.dMinBn2 + " ~ " + (String)OM.DevOptn.dMaxBn2 ; pnRBLim2 -> Caption = sTemp ; pnRBCnt2 -> Caption = SPR.GetPkgCnt(2);
    sTemp = (String)OM.DevOptn.dMinBn3 + " ~ " + (String)OM.DevOptn.dMaxBn3 ; pnRBLim3 -> Caption = sTemp ; pnRBCnt3 -> Caption = SPR.GetPkgCnt(3);
    sTemp = (String)OM.DevOptn.dMinBn4 + " ~ " + (String)OM.DevOptn.dMaxBn4 ; pnRBLim4 -> Caption = sTemp ; pnRBCnt4 -> Caption = SPR.GetPkgCnt(4);
    sTemp = (String)OM.DevOptn.dMinBn5 + " ~ " + (String)OM.DevOptn.dMaxBn5 ; pnRBLim5 -> Caption = sTemp ; pnRBCnt5 -> Caption = SPR.GetPkgCnt(5);
    sTemp = (String)OM.DevOptn.dMinBn6 + " ~ " + (String)OM.DevOptn.dMaxBn6 ; pnRBLim6 -> Caption = sTemp ; pnRBCnt6 -> Caption = SPR.GetPkgCnt(6);
    sTemp = (String)OM.DevOptn.dMinBn7 + " ~ " + (String)OM.DevOptn.dMaxBn7 ; pnRBLim7 -> Caption = sTemp ; pnRBCnt7 -> Caption = SPR.GetPkgCnt(7);
//    sTemp = "BUFFER " + (String)OM.DevOptn.iBn6Sel                          ; pnRBLim6 -> Caption = sTemp ; pnRBCnt6 -> Caption = SPR.GetPkgCnt(6);
//    sTemp = "BUFFER " + (String)OM.DevOptn.iBn7Sel                          ; pnRBLim7 -> Caption = sTemp ; pnRBCnt7 -> Caption = SPR.GetPkgCnt(7);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn8Sel                          ; pnRBLim8 -> Caption = sTemp ; pnRBCnt8 -> Caption = SPR.GetPkgCnt(8);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn9Sel                          ; pnRBLim9 -> Caption = sTemp ; pnRBCnt9 -> Caption = SPR.GetPkgCnt(9);
*/

    static bool bPreRun = SEQ._bRun ;
    bPreRun = SEQ._bRun ;

    TDateTime Date = Now();


    AnsiString sDate = Date.CurrentDate().FormatString("yyyymmdd") ;
    if(PCK.m_sPreDate != sDate) PCK.ClearGraphData() ;
    PCK.m_sPreDate = sDate ;

    //제품분류 갯수 그래프
    Series1 -> Clear();
    sTemp = "L9" ; Series1 -> AddXY(0  ,SPL.GetPkgCnt(9),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[9]] );
    sTemp = "L8" ; Series1 -> AddXY(1  ,SPL.GetPkgCnt(8),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[8]] );
    sTemp = "L7" ; Series1 -> AddXY(2  ,SPL.GetPkgCnt(7),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[7]] );
    sTemp = "L6" ; Series1 -> AddXY(3  ,SPL.GetPkgCnt(6),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[6]] );
    sTemp = "L5" ; Series1 -> AddXY(4  ,SPL.GetPkgCnt(5),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[5]] );
    sTemp = "L4" ; Series1 -> AddXY(5  ,SPL.GetPkgCnt(4),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[4]] );
    sTemp = "L3" ; Series1 -> AddXY(6  ,SPL.GetPkgCnt(3),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[3]] );
    sTemp = "L2" ; Series1 -> AddXY(7  ,SPL.GetPkgCnt(2),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[2]] );
    sTemp = "L1" ; Series1 -> AddXY(8  ,SPL.GetPkgCnt(1),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[1]] );
    sTemp = "L0" ; Series1 -> AddXY(9  ,SPL.GetPkgCnt(0),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[0]] );

    sTemp = "R0" ; Series1 -> AddXY(10 ,SPR.GetPkgCnt(0),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[0]] );
    sTemp = "R1" ; Series1 -> AddXY(11 ,SPR.GetPkgCnt(1),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[1]] );
    sTemp = "R2" ; Series1 -> AddXY(12 ,SPR.GetPkgCnt(2),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[2]] );
    sTemp = "R3" ; Series1 -> AddXY(13 ,SPR.GetPkgCnt(3),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[3]] );
    sTemp = "R4" ; Series1 -> AddXY(14 ,SPR.GetPkgCnt(4),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[4]] );
    sTemp = "R5" ; Series1 -> AddXY(15 ,SPR.GetPkgCnt(5),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[5]] );
    sTemp = "R6" ; Series1 -> AddXY(16 ,SPR.GetPkgCnt(6),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[6]] );
    sTemp = "R7" ; Series1 -> AddXY(17 ,SPR.GetPkgCnt(7),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[7]] );
    sTemp = "R8" ; Series1 -> AddXY(18 ,SPR.GetPkgCnt(8),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[8]] );
    sTemp = "R9" ; Series1 -> AddXY(19 ,SPR.GetPkgCnt(9),sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[9]] );


/*
    Series1 -> Clear();
    sTemp = (String)OM.DevOptn.dMinBn[0] + "~" + (String)OM.DevOptn.dMaxBn[0] ; Series1 -> AddXY(0,PCK.m_iBinGraphCnt[0],sTemp,clStat[csRslt1 ]);
    sTemp = (String)OM.DevOptn.dMinBn[1] + "~" + (String)OM.DevOptn.dMaxBn[1] ; Series1 -> AddXY(1,PCK.m_iBinGraphCnt[1],sTemp,clStat[csRslt2 ]);
    sTemp = (String)OM.DevOptn.dMinBn[2] + "~" + (String)OM.DevOptn.dMaxBn[2] ; Series1 -> AddXY(2,PCK.m_iBinGraphCnt[2],sTemp,clStat[csRslt3 ]);
    sTemp = (String)OM.DevOptn.dMinBn[3] + "~" + (String)OM.DevOptn.dMaxBn[3] ; Series1 -> AddXY(3,PCK.m_iBinGraphCnt[3],sTemp,clStat[csRslt4 ]);
    sTemp = (String)OM.DevOptn.dMinBn[4] + "~" + (String)OM.DevOptn.dMaxBn[4] ; Series1 -> AddXY(4,PCK.m_iBinGraphCnt[4],sTemp,clStat[csRslt5 ]);
    sTemp = (String)OM.DevOptn.dMinBn[5] + "~" + (String)OM.DevOptn.dMaxBn[5] ; Series1 -> AddXY(5,PCK.m_iBinGraphCnt[5],sTemp,clStat[csRslt5 ]);
    sTemp = (String)OM.DevOptn.dMinBn[6] + "~" + (String)OM.DevOptn.dMaxBn[6] ; Series1 -> AddXY(6,PCK.m_iBinGraphCnt[6],sTemp,clStat[csRslt5 ]);
    sTemp = (String)OM.DevOptn.dMinBn[7] + "~" + (String)OM.DevOptn.dMaxBn[7] ; Series1 -> AddXY(7,PCK.m_iBinGraphCnt[7],sTemp,clStat[csRslt5 ]);
    sTemp = (String)OM.DevOptn.dMinBn[8] + "~" + (String)OM.DevOptn.dMaxBn[8] ; Series1 -> AddXY(8,PCK.m_iBinGraphCnt[8],sTemp,clStat[csRslt5 ]);
    sTemp = (String)OM.DevOptn.dMinBn[9] + "~" + (String)OM.DevOptn.dMaxBn[9] ; Series1 -> AddXY(9,PCK.m_iBinGraphCnt[9],sTemp,clStat[csRslt1 ]);
*/
    //측정값 분포 그래프.
    BarSeries1 -> Clear();

    const double iGap = 0.005 ;
    int iMaxVal = 0 ;
    int    iCenterBin = MAX_VALUE_GRAPH_CNT / 2 ;
    double dBinMinVal = -(iGap * iCenterBin) ;
    for(int i = 0 ; i < MAX_VALUE_GRAPH_CNT ; i++) {
        sTemp = (String)(dBinMinVal + iGap * i) + "~" + (String)(dBinMinVal + iGap * (i+1)) ; BarSeries1 -> AddXY(i,PCK.m_iValGraphCnt[i],sTemp,clBlue);
        if(PCK.m_iValGraphCnt[i] > iMaxVal) iMaxVal = PCK.m_iValGraphCnt[i] ;
    }

    pnLIndicator -> Caption = SPL.dIndicator ;
    pnRIndicator -> Caption = SPR.dIndicator ;

    //Series2 -> Clear();
    //Series2 -> AddXY(iCenterBin,iMaxVal+3,0.0,clRed);

    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::btLbRo0Click(TObject *Sender)
{

    if(SPL.GetSeqStep()) {
//        FM_MsgOk("Confirm","Sorting Left Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Left Bin " + (String)(iTag) + "?" ;
//    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPL.SetBin(iTag,false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperator::btLbRa0Click(TObject *Sender)
{
    if(SPL.GetSeqStep()){
//        FM_MsgOk("Confirm","Sorting Left Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Left Bin " + (String)(iTag) + " All ?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPL.SetBin(iTag,true);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperator::btRbRo0Click(TObject *Sender)
{
    if(SPR.GetSeqStep()) {
//        FM_MsgOk("Confirm","Sorting Right Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Right Bin " + (String)(iTag) + "?" ;
    //if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPR.SetBin(iTag,false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperator::btRbRa0Click(TObject *Sender)
{
    if(SPR.GetSeqStep()){
        FM_MsgOk("Confirm","Sorting Right Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Right Bin " + (String)(iTag) + "All ?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPR.SetBin(iTag,true);
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperator::pnLBCnt7Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Bin Count Clear ?") != mrYes ) return ;
    TPanel *pPn = dynamic_cast <TPanel *> (Sender);
    String  sName= pPn->Name;

    //빈번호 순서 Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19
         if(sName == "pnLBCnt9") { SPL.m_iPkgCnt[9] = 0 ; Trace("Button Click","Clear Left  Supply Bin 9 Cnt "); }
    else if(sName == "pnLBCnt8") { SPL.m_iPkgCnt[8] = 0 ; Trace("Button Click","Clear Left  Supply Bin 8 Cnt "); }
    else if(sName == "pnLBCnt7") { SPL.m_iPkgCnt[7] = 0 ; Trace("Button Click","Clear Left  Supply Bin 7 Cnt "); }
    else if(sName == "pnLBCnt6") { SPL.m_iPkgCnt[6] = 0 ; Trace("Button Click","Clear Left  Supply Bin 6 Cnt "); }
    else if(sName == "pnLBCnt5") { SPL.m_iPkgCnt[5] = 0 ; Trace("Button Click","Clear Left  Supply Bin 5 Cnt "); }
    else if(sName == "pnLBCnt4") { SPL.m_iPkgCnt[4] = 0 ; Trace("Button Click","Clear Left  Supply Bin 4 Cnt "); }
    else if(sName == "pnLBCnt3") { SPL.m_iPkgCnt[3] = 0 ; Trace("Button Click","Clear Left  Supply Bin 3 Cnt "); }
    else if(sName == "pnLBCnt2") { SPL.m_iPkgCnt[2] = 0 ; Trace("Button Click","Clear Left  Supply Bin 2 Cnt "); }
    else if(sName == "pnLBCnt1") { SPL.m_iPkgCnt[1] = 0 ; Trace("Button Click","Clear Left  Supply Bin 1 Cnt "); }
    else if(sName == "pnLBCnt0") { SPL.m_iPkgCnt[0] = 0 ; Trace("Button Click","Clear Left  Supply Bin 0 Cnt "); }

    else if(sName == "pnRBCnt0") { SPR.m_iPkgCnt[0] = 0 ; Trace("Button Click","Clear Right Supply Bin 0 Cnt "); }
    else if(sName == "pnRBCnt1") { SPR.m_iPkgCnt[1] = 0 ; Trace("Button Click","Clear Right Supply Bin 1 Cnt "); }
    else if(sName == "pnRBCnt2") { SPR.m_iPkgCnt[2] = 0 ; Trace("Button Click","Clear Right Supply Bin 2 Cnt "); }
    else if(sName == "pnRBCnt3") { SPR.m_iPkgCnt[3] = 0 ; Trace("Button Click","Clear Right Supply Bin 3 Cnt "); }
    else if(sName == "pnRBCnt4") { SPR.m_iPkgCnt[4] = 0 ; Trace("Button Click","Clear Right Supply Bin 4 Cnt "); }
    else if(sName == "pnRBCnt5") { SPR.m_iPkgCnt[5] = 0 ; Trace("Button Click","Clear Right Supply Bin 5 Cnt "); }
    else if(sName == "pnRBCnt6") { SPR.m_iPkgCnt[6] = 0 ; Trace("Button Click","Clear Right Supply Bin 6 Cnt "); }
    else if(sName == "pnRBCnt7") { SPR.m_iPkgCnt[7] = 0 ; Trace("Button Click","Clear Right Supply Bin 7 Cnt "); }
    else if(sName == "pnRBCnt8") { SPR.m_iPkgCnt[8] = 0 ; Trace("Button Click","Clear Right Supply Bin 8 Cnt "); }
    else if(sName == "pnRBCnt9") { SPR.m_iPkgCnt[9] = 0 ; Trace("Button Click","Clear Right Supply Bin 9 Cnt "); }






}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::btCountAllClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Bin Count Clear ?") != mrYes ) return ;


    SPL.m_iPkgCnt[9] = 0 ;
    SPL.m_iPkgCnt[8] = 0 ;
    SPL.m_iPkgCnt[7] = 0 ;
    SPL.m_iPkgCnt[6] = 0 ;
    SPL.m_iPkgCnt[5] = 0 ;
    SPL.m_iPkgCnt[4] = 0 ;
    SPL.m_iPkgCnt[3] = 0 ;
    SPL.m_iPkgCnt[2] = 0 ;
    SPL.m_iPkgCnt[1] = 0 ;
    SPL.m_iPkgCnt[0] = 0 ;

    SPR.m_iPkgCnt[0] = 0 ;
    SPR.m_iPkgCnt[1] = 0 ;
    SPR.m_iPkgCnt[2] = 0 ;
    SPR.m_iPkgCnt[3] = 0 ;
    SPR.m_iPkgCnt[4] = 0 ;
    SPR.m_iPkgCnt[5] = 0 ;
    SPR.m_iPkgCnt[6] = 0 ;
    SPR.m_iPkgCnt[7] = 0 ;
    SPR.m_iPkgCnt[8] = 0 ;
    SPR.m_iPkgCnt[9] = 0 ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::btSupplyAllClick(TObject *Sender)
{
    SPR.m_bSetSupplyAll = true ;
    SPL.m_bSetSupplyAll = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::btSetBinClick(TObject *Sender)
{
    OM.DevOptn.iBnLSel[0] = cbBnL0Sel -> ItemIndex ; OM.DevOptn.iBnRSel[0] = cbBnR0Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[1] = cbBnL1Sel -> ItemIndex ; OM.DevOptn.iBnRSel[1] = cbBnR1Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[2] = cbBnL2Sel -> ItemIndex ; OM.DevOptn.iBnRSel[2] = cbBnR2Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[3] = cbBnL3Sel -> ItemIndex ; OM.DevOptn.iBnRSel[3] = cbBnR3Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[4] = cbBnL4Sel -> ItemIndex ; OM.DevOptn.iBnRSel[4] = cbBnR4Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[5] = cbBnL5Sel -> ItemIndex ; OM.DevOptn.iBnRSel[5] = cbBnR5Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[6] = cbBnL6Sel -> ItemIndex ; OM.DevOptn.iBnRSel[6] = cbBnR6Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[7] = cbBnL7Sel -> ItemIndex ; OM.DevOptn.iBnRSel[7] = cbBnR7Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[8] = cbBnL8Sel -> ItemIndex ; OM.DevOptn.iBnRSel[8] = cbBnR8Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[9] = cbBnL9Sel -> ItemIndex ; OM.DevOptn.iBnRSel[9] = cbBnR9Sel -> ItemIndex ;

    OM.SaveDevOptn(OM.m_sCrntDev) ;

//    pnLBLim0 -> Color = clStat[OM.DevOptn.iBnLSel[0] ] ;  pnRBLim0 -> Color = clStat[OM.DevOptn.iBnRSel[0] ] ;
//    pnLBLim1 -> Color = clStat[OM.DevOptn.iBnLSel[1] ] ;  pnRBLim1 -> Color = clStat[OM.DevOptn.iBnRSel[1] ] ;
//    pnLBLim2 -> Color = clStat[OM.DevOptn.iBnLSel[2] ] ;  pnRBLim2 -> Color = clStat[OM.DevOptn.iBnRSel[2] ] ;
//    pnLBLim3 -> Color = clStat[OM.DevOptn.iBnLSel[3] ] ;  pnRBLim3 -> Color = clStat[OM.DevOptn.iBnRSel[3] ] ;
//    pnLBLim4 -> Color = clStat[OM.DevOptn.iBnLSel[4] ] ;  pnRBLim4 -> Color = clStat[OM.DevOptn.iBnRSel[4] ] ;
//    pnLBLim5 -> Color = clStat[OM.DevOptn.iBnLSel[5] ] ;  pnRBLim5 -> Color = clStat[OM.DevOptn.iBnRSel[5] ] ;
//    pnLBLim6 -> Color = clStat[OM.DevOptn.iBnLSel[6] ] ;  pnRBLim6 -> Color = clStat[OM.DevOptn.iBnRSel[6] ] ;
//    pnLBLim7 -> Color = clStat[OM.DevOptn.iBnLSel[7] ] ;  pnRBLim7 -> Color = clStat[OM.DevOptn.iBnRSel[7] ] ;
//    pnLBLim8 -> Color = clStat[OM.DevOptn.iBnLSel[8] ] ;  pnRBLim8 -> Color = clStat[OM.DevOptn.iBnRSel[8] ] ;
//    pnLBLim9 -> Color = clStat[OM.DevOptn.iBnLSel[9] ] ;  pnRBLim9 -> Color = clStat[OM.DevOptn.iBnRSel[9] ] ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperator::GroupBox16Click(TObject *Sender)
{
    String sTemp ;
    Series1 -> Clear();
    sTemp =                              "FAIL"                                 ; Series1 -> AddXY(0,100,sTemp,clStat[csRslt0 ]);
    sTemp = (String)OM.DevOptn.dMinBn[0] + " ~ " + (String)OM.DevOptn.dMaxBn[0] ; Series1 -> AddXY(1,110,sTemp,clStat[csRslt1 ]);
    sTemp = (String)OM.DevOptn.dMinBn[1] + " ~ " + (String)OM.DevOptn.dMaxBn[1] ; Series1 -> AddXY(2,120,sTemp,clStat[csRslt2 ]);
    sTemp = (String)OM.DevOptn.dMinBn[2] + " ~ " + (String)OM.DevOptn.dMaxBn[2] ; Series1 -> AddXY(3,130,sTemp,clStat[csRslt3 ]);
    sTemp = (String)OM.DevOptn.dMinBn[3] + " ~ " + (String)OM.DevOptn.dMaxBn[3] ; Series1 -> AddXY(4,120,sTemp,clStat[csRslt4 ]);
    sTemp = (String)OM.DevOptn.dMinBn[4] + " ~ " + (String)OM.DevOptn.dMaxBn[4] ; Series1 -> AddXY(5,110,sTemp,clStat[csRslt5 ]);
}
//---------------------------------------------------------------------------



void __fastcall TFrmOperator::Button1Click(TObject *Sender)
{
    double dTemp = Edit1 -> Text.ToDouble() ;

    PCK.m_iBinGraphCnt[PCK.GetBin(dTemp)]++;
    if(PCK.GetGraphBin(dTemp) > -1) PCK.m_iValGraphCnt[PCK.GetGraphBin(dTemp)]++ ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmOperator::cbBnL9SelChange(TObject *Sender)
{
    OM.DevOptn.iBnLSel[0] = cbBnL0Sel -> ItemIndex ; OM.DevOptn.iBnRSel[0] = cbBnR0Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[1] = cbBnL1Sel -> ItemIndex ; OM.DevOptn.iBnRSel[1] = cbBnR1Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[2] = cbBnL2Sel -> ItemIndex ; OM.DevOptn.iBnRSel[2] = cbBnR2Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[3] = cbBnL3Sel -> ItemIndex ; OM.DevOptn.iBnRSel[3] = cbBnR3Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[4] = cbBnL4Sel -> ItemIndex ; OM.DevOptn.iBnRSel[4] = cbBnR4Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[5] = cbBnL5Sel -> ItemIndex ; OM.DevOptn.iBnRSel[5] = cbBnR5Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[6] = cbBnL6Sel -> ItemIndex ; OM.DevOptn.iBnRSel[6] = cbBnR6Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[7] = cbBnL7Sel -> ItemIndex ; OM.DevOptn.iBnRSel[7] = cbBnR7Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[8] = cbBnL8Sel -> ItemIndex ; OM.DevOptn.iBnRSel[8] = cbBnR8Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[9] = cbBnL9Sel -> ItemIndex ; OM.DevOptn.iBnRSel[9] = cbBnR9Sel -> ItemIndex ;

    OM.SaveDevOptn(OM.m_sCrntDev) ;

//    if(OM.DevOptn.iBnLIdx[0] == (int)gsBuffer) pnLBLim0 -> Color = clStat[OM.DevOptn.iBnLSel[0] ] ;
//    if(OM.DevOptn.iBnLIdx[1] == (int)gsBuffer) pnLBLim1 -> Color = clStat[OM.DevOptn.iBnLSel[1] ] ;
//    if(OM.DevOptn.iBnLIdx[2] == (int)gsBuffer) pnLBLim2 -> Color = clStat[OM.DevOptn.iBnLSel[2] ] ;
//    if(OM.DevOptn.iBnLIdx[3] == (int)gsBuffer) pnLBLim3 -> Color = clStat[OM.DevOptn.iBnLSel[3] ] ;
//    if(OM.DevOptn.iBnLIdx[4] == (int)gsBuffer) pnLBLim4 -> Color = clStat[OM.DevOptn.iBnLSel[4] ] ;
//    if(OM.DevOptn.iBnLIdx[5] == (int)gsBuffer) pnLBLim5 -> Color = clStat[OM.DevOptn.iBnLSel[5] ] ;
//    if(OM.DevOptn.iBnLIdx[6] == (int)gsBuffer) pnLBLim6 -> Color = clStat[OM.DevOptn.iBnLSel[6] ] ;
//    if(OM.DevOptn.iBnLIdx[7] == (int)gsBuffer) pnLBLim7 -> Color = clStat[OM.DevOptn.iBnLSel[7] ] ;
//    if(OM.DevOptn.iBnLIdx[8] == (int)gsBuffer) pnLBLim8 -> Color = clStat[OM.DevOptn.iBnLSel[8] ] ;
//    if(OM.DevOptn.iBnLIdx[9] == (int)gsBuffer) pnLBLim9 -> Color = clStat[OM.DevOptn.iBnLSel[9] ] ;
//
//    if(OM.DevOptn.iBnRIdx[0] == (int)gsBuffer) pnRBLim0 -> Color = clStat[OM.DevOptn.iBnRSel[0] ] ;
//    if(OM.DevOptn.iBnRIdx[1] == (int)gsBuffer) pnRBLim1 -> Color = clStat[OM.DevOptn.iBnRSel[1] ] ;
//    if(OM.DevOptn.iBnRIdx[2] == (int)gsBuffer) pnRBLim2 -> Color = clStat[OM.DevOptn.iBnRSel[2] ] ;
//    if(OM.DevOptn.iBnRIdx[3] == (int)gsBuffer) pnRBLim3 -> Color = clStat[OM.DevOptn.iBnRSel[3] ] ;
//    if(OM.DevOptn.iBnRIdx[4] == (int)gsBuffer) pnRBLim4 -> Color = clStat[OM.DevOptn.iBnRSel[4] ] ;
//    if(OM.DevOptn.iBnRIdx[5] == (int)gsBuffer) pnRBLim5 -> Color = clStat[OM.DevOptn.iBnRSel[5] ] ;
//    if(OM.DevOptn.iBnRIdx[6] == (int)gsBuffer) pnRBLim6 -> Color = clStat[OM.DevOptn.iBnRSel[6] ] ;
//    if(OM.DevOptn.iBnRIdx[7] == (int)gsBuffer) pnRBLim7 -> Color = clStat[OM.DevOptn.iBnRSel[7] ] ;
//    if(OM.DevOptn.iBnRIdx[8] == (int)gsBuffer) pnRBLim8 -> Color = clStat[OM.DevOptn.iBnRSel[8] ] ;
//    if(OM.DevOptn.iBnRIdx[9] == (int)gsBuffer) pnRBLim9 -> Color = clStat[OM.DevOptn.iBnRSel[9] ] ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::Button2Click(TObject *Sender)
{
    PCK.ClearGraphData();    
}
//---------------------------------------------------------------------------

