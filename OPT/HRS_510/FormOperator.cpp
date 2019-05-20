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
#include "Sorter.h"
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

    ctBinCnt -> DoubleBuffered = true ;
    ctDist   -> DoubleBuffered = true ;

    pnLBLim0 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[0] ] ;
    pnLBLim1 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[1] ] ;  
    pnLBLim2 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[2] ] ;  
    pnLBLim3 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[3] ] ;  
    pnLBLim4 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[4] ] ;  
    pnLBLim5 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[5] ] ;  
    pnLBLim6 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[6] ] ;  
    pnLBLim7 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[7] ] ;  
    pnLBLim8 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[8] ] ;  
    pnLBLim9 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[9] ] ;

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
    sCaptionName[gsFail  ] = "Fail";
    sCaptionName[gsCFail ] = "Chuck Fail";
    sCaptionName[gsNotUse] = "Not Use";


    sTemp = sCaptionName[OM.DevOptn.iBnLSel[0]] ; pnLBLim0 -> Caption = sTemp ; pnLBCnt0 -> Caption = SRT.m_iBinCnt[0];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[1]] ; pnLBLim1 -> Caption = sTemp ; pnLBCnt1 -> Caption = SRT.m_iBinCnt[1];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[2]] ; pnLBLim2 -> Caption = sTemp ; pnLBCnt2 -> Caption = SRT.m_iBinCnt[2];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[3]] ; pnLBLim3 -> Caption = sTemp ; pnLBCnt3 -> Caption = SRT.m_iBinCnt[3];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[4]] ; pnLBLim4 -> Caption = sTemp ; pnLBCnt4 -> Caption = SRT.m_iBinCnt[4];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[5]] ; pnLBLim5 -> Caption = sTemp ; pnLBCnt5 -> Caption = SRT.m_iBinCnt[5];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[6]] ; pnLBLim6 -> Caption = sTemp ; pnLBCnt6 -> Caption = SRT.m_iBinCnt[6];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[7]] ; pnLBLim7 -> Caption = sTemp ; pnLBCnt7 -> Caption = SRT.m_iBinCnt[7];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[8]] ; pnLBLim8 -> Caption = sTemp ; pnLBCnt8 -> Caption = SRT.m_iBinCnt[8];
    sTemp = sCaptionName[OM.DevOptn.iBnLSel[9]] ; pnLBLim9 -> Caption = sTemp ; pnLBCnt9 -> Caption = SRT.m_iBinCnt[9];



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




    static bool bPreRun = SEQ._bRun ;
    bPreRun = SEQ._bRun ;

    TDateTime Date = Now();


    AnsiString sDate = Date.CurrentDate().FormatString("yyyymmdd") ;
    if(PCK.m_sPreDate != sDate) PCK.ClearGraphData() ;
    PCK.m_sPreDate = sDate ;

    pnLBCnt0 -> Caption = SRT.m_iBinCnt[0];
    pnLBCnt1 -> Caption = SRT.m_iBinCnt[1];
    pnLBCnt2 -> Caption = SRT.m_iBinCnt[2];
    pnLBCnt3 -> Caption = SRT.m_iBinCnt[3];
    pnLBCnt4 -> Caption = SRT.m_iBinCnt[4];
    pnLBCnt5 -> Caption = SRT.m_iBinCnt[5];
    pnLBCnt6 -> Caption = SRT.m_iBinCnt[6];
    pnLBCnt7 -> Caption = SRT.m_iBinCnt[7];
    pnLBCnt8 -> Caption = SRT.m_iBinCnt[8];
    pnLBCnt9 -> Caption = SRT.m_iBinCnt[9];

    //제품분류 갯수 그래프
    Series1 -> Clear();

    //측정값 분포 그래프.  ctDist
    BarSeries1 -> Clear();
    const double iGap = 0.005 ;
    AnsiString sTemp ;
    int iMaxVal = 0 ;
    int    iCenterBin = MAX_VALUE_GRAPH_CNT / 2 ;
    double dBinMinVal = -(iGap * iCenterBin) ;
    for(int i = 0 ; i < MAX_VALUE_GRAPH_CNT ; i++) {
        sTemp = (String)(dBinMinVal + iGap * i) + "~" + (String)(dBinMinVal + iGap * (i+1)) ; BarSeries1 -> AddXY(i,PCK.m_iValGraphCnt[i],sTemp,clBlue);
        if(PCK.m_iValGraphCnt[i] > iMaxVal) iMaxVal = PCK.m_iValGraphCnt[i] ;
    }


    //제품분류 갯수 그래프   ctBinCnt
    Series1 -> Clear();
    for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
        sTemp = "B"+AnsiString(i) ; Series1 -> AddXY(i  ,SRT.m_iBinCnt[i],sTemp,clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLSel[i]] );
    }

    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------



void __fastcall TFrmOperator::pnLBCnt7Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Bin Count Clear ?") != mrYes ) return ;
    TPanel *pPn = dynamic_cast <TPanel *> (Sender);
    String  sName= pPn->Name;

    //빈번호 순서 Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19
    //     if(sName == "pnLBCnt9") { SPL.m_iPkgCnt[9] = 0 ; Trace("Button Click","Clear Left  Supply Bin 9 Cnt "); }
    //else if(sName == "pnLBCnt8") { SPL.m_iPkgCnt[8] = 0 ; Trace("Button Click","Clear Left  Supply Bin 8 Cnt "); }
    //else if(sName == "pnLBCnt7") { SPL.m_iPkgCnt[7] = 0 ; Trace("Button Click","Clear Left  Supply Bin 7 Cnt "); }
    //else if(sName == "pnLBCnt6") { SPL.m_iPkgCnt[6] = 0 ; Trace("Button Click","Clear Left  Supply Bin 6 Cnt "); }
    //else if(sName == "pnLBCnt5") { SPL.m_iPkgCnt[5] = 0 ; Trace("Button Click","Clear Left  Supply Bin 5 Cnt "); }
    //else if(sName == "pnLBCnt4") { SPL.m_iPkgCnt[4] = 0 ; Trace("Button Click","Clear Left  Supply Bin 4 Cnt "); }
    //else if(sName == "pnLBCnt3") { SPL.m_iPkgCnt[3] = 0 ; Trace("Button Click","Clear Left  Supply Bin 3 Cnt "); }
    //else if(sName == "pnLBCnt2") { SPL.m_iPkgCnt[2] = 0 ; Trace("Button Click","Clear Left  Supply Bin 2 Cnt "); }
    //else if(sName == "pnLBCnt1") { SPL.m_iPkgCnt[1] = 0 ; Trace("Button Click","Clear Left  Supply Bin 1 Cnt "); }
    //else if(sName == "pnLBCnt0") { SPL.m_iPkgCnt[0] = 0 ; Trace("Button Click","Clear Left  Supply Bin 0 Cnt "); }
    //
    //else if(sName == "pnRBCnt0") { SPR.m_iPkgCnt[0] = 0 ; Trace("Button Click","Clear Right Supply Bin 0 Cnt "); }
    //else if(sName == "pnRBCnt1") { SPR.m_iPkgCnt[1] = 0 ; Trace("Button Click","Clear Right Supply Bin 1 Cnt "); }
    //else if(sName == "pnRBCnt2") { SPR.m_iPkgCnt[2] = 0 ; Trace("Button Click","Clear Right Supply Bin 2 Cnt "); }
    //else if(sName == "pnRBCnt3") { SPR.m_iPkgCnt[3] = 0 ; Trace("Button Click","Clear Right Supply Bin 3 Cnt "); }
    //else if(sName == "pnRBCnt4") { SPR.m_iPkgCnt[4] = 0 ; Trace("Button Click","Clear Right Supply Bin 4 Cnt "); }
    //else if(sName == "pnRBCnt5") { SPR.m_iPkgCnt[5] = 0 ; Trace("Button Click","Clear Right Supply Bin 5 Cnt "); }
    //else if(sName == "pnRBCnt6") { SPR.m_iPkgCnt[6] = 0 ; Trace("Button Click","Clear Right Supply Bin 6 Cnt "); }
    //else if(sName == "pnRBCnt7") { SPR.m_iPkgCnt[7] = 0 ; Trace("Button Click","Clear Right Supply Bin 7 Cnt "); }
    //else if(sName == "pnRBCnt8") { SPR.m_iPkgCnt[8] = 0 ; Trace("Button Click","Clear Right Supply Bin 8 Cnt "); }
    //else if(sName == "pnRBCnt9") { SPR.m_iPkgCnt[9] = 0 ; Trace("Button Click","Clear Right Supply Bin 9 Cnt "); }






}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
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



void __fastcall TFrmOperator::Button2Click(TObject *Sender)
{
    String sTemp = "Clear Graph?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    PCK.ClearGraphData();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperator::btBinCntClearClick(TObject *Sender)
{
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Clear All Bin Count?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SRT.ClearBinCnt() ;
}
//---------------------------------------------------------------------------

