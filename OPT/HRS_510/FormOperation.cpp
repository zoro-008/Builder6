//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"

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
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
//#include "Loader.h"
#include "Rail.h"
//#include "UnLoader.h"
#include "Picker.h"
#include "Sorter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    SEQ.Reset();

    DM.ARAY[riPRB].SetParent(pnPRB ); DM.ARAY[riPRB].SetConfig(false , "PRB "); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riWRK].SetParent(pnWRK ); DM.ARAY[riWRK].SetConfig(false , "WRK "); DM.ARAY[riWRK].ClearDispItem();
    DM.ARAY[riWKE].SetParent(pnWKE ); DM.ARAY[riWKE].SetConfig(false , "WKE "); DM.ARAY[riWKE].ClearDispItem();
    DM.ARAY[riPSB].SetParent(pnPSB ); DM.ARAY[riPSB].SetConfig(false , "PSB "); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riPCK].SetParent(pnPCK ); DM.ARAY[riPCK].SetConfig(false , "PCK "); DM.ARAY[riPCK].ClearDispItem();
    DM.ARAY[riSTN].SetParent(pnSTN ); DM.ARAY[riSTN].SetConfig(false , "STN "); DM.ARAY[riSTN].ClearDispItem();
    DM.ARAY[riSRT].SetParent(pnSRT ); DM.ARAY[riSRT].SetConfig(false , "SRT "); DM.ARAY[riSRT].ClearDispItem();

    DM.ARAY[riPRB].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riPRB ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riPRB].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riPRB ].SetDispColor(csWork  ,clMoneyGreen );

    DM.ARAY[riWRK].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riWRK].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riWRK].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riWRK].SetDispItem(csMask   ,"Mask "     ) ;

    DM.ARAY[riWKE].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riWKE].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riWKE].SetDispItem(csUnkwn  ,"Empty "     ) ;

//    DM.ARAY[riWKE].SetDispItem(csNone   ,"None "      ) ;
//    DM.ARAY[riWKE].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
//    DM.ARAY[riWKE].SetDispItem(csUnkwn  ,"Empty "     ) ;

    DM.ARAY[riPSB].SetDispItem(csNone   ,"None "      ) ;
//    DM.ARAY[riPSB].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riPSB].SetDispItem(csEmpty  ,"Empty "     ) ;

    DM.ARAY[riPCK].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riPCK].SetDispItem(csUnkwn  ,"Unkwn "     ) ;

    DM.ARAY[riSTN].SetDispItem(csNone   ,"None  "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt0  ,"gsGradeA "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt1  ,"gsGradeB "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt2  ,"gsGradeC "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt3  ,"gsGradeD "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt4  ,"gsGradeE "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt5  ,"gsGradeF "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt6  ,"gsGradeG "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt7  ,"gsGradeH "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt8  ,"gsGradeI "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRslt9  ,"gsGradeJ "     ) ;
    DM.ARAY[riSTN].SetDispItem(csRsltA  ,"gsFail "       ) ;
    DM.ARAY[riSTN].SetDispItem(csRsltB  ,"gsCFail "      ) ;

    DM.ARAY[riSRT].SetDispItem(csNone   ,"None "         ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt0  ,"gsGradeA "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt1  ,"gsGradeB "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt2  ,"gsGradeC "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt3  ,"gsGradeD "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt4  ,"gsGradeE "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt5  ,"gsGradeF "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt6  ,"gsGradeG "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt7  ,"gsGradeH "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt8  ,"gsGradeI "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRslt9  ,"gsGradeJ "     ) ;
    DM.ARAY[riSRT].SetDispItem(csRsltA  ,"gsFail "       ) ;
    DM.ARAY[riSRT].SetDispItem(csRsltB  ,"gsCFail "      ) ;

    //Default Set.
//    clStat[csRslt0 ] = clInactiveCaption ;'
//    clStat[csRslt1 ] = clMaroon          ;'
//    clStat[csRslt2 ] = clPurple          ;'
//    clStat[csRslt3 ] = clTeal            ;'
//    clStat[csRslt4 ] = clOlive           ;'
//    clStat[csRslt5 ] = clNavy            ;'
//
//    clStat[csNone  ] = csWhite           ;'
//    clStat[csEmpty ] = csSilver          ;'
//    clStat[csUnkwn ] = csAqua            ;'
//    clStat[csWork  ] = (TColor)0x00FF9595;//clHighlight       ;//csBlue
//    clStat[csFail  ] = clPurple          ;//csFuchsia
//    clStat[csEtc   ] = clNone            ;//etc*/

    clStat[csRslt0 ] = (TColor)OM.CmnOptn.iRsltColor0;
    clStat[csRslt1 ] = (TColor)OM.CmnOptn.iRsltColor1;
    clStat[csRslt2 ] = (TColor)OM.CmnOptn.iRsltColor2;
    clStat[csRslt3 ] = (TColor)OM.CmnOptn.iRsltColor3;
    clStat[csRslt4 ] = (TColor)OM.CmnOptn.iRsltColor4;
    clStat[csRslt5 ] = (TColor)OM.CmnOptn.iRsltColor5;
    clStat[csRslt6 ] = (TColor)OM.CmnOptn.iRsltColor6;
    clStat[csRslt7 ] = (TColor)OM.CmnOptn.iRsltColor7;
    clStat[csRslt8 ] = (TColor)OM.CmnOptn.iRsltColor8;
    clStat[csRslt9 ] = (TColor)OM.CmnOptn.iRsltColor9;
    clStat[csRsltA ] = (TColor)OM.CmnOptn.iRsltColorA;
    clStat[csRsltB ] = (TColor)OM.CmnOptn.iRsltColorB;
    clStat[csRsltC ] = (TColor)OM.CmnOptn.iRsltColorC;

    //Manual Button Caption
    btMan1_1  -> Caption = "[" + AnsiString(mcLDR_Home   ) + "]" + "Loading home"    ;  btMan1_1  -> Tag = (int)mcLDR_Home    ; btMan1_1  -> Visible = true  ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_Supply ) + "]" + "Loading Supply"  ;  btMan1_2  -> Tag = (int)mcLDR_Supply  ; btMan1_2  -> Visible = true  ;

    btMan2_1  -> Caption = "[" + AnsiString(mcRAL_Home   ) + "]" + "Rail Home"       ;  btMan2_1  -> Tag = (int)mcRAL_Home    ; btMan2_1  -> Visible = true  ;
    btMan2_2  -> Caption = "[" + AnsiString(mcRAL_In     ) + "]" + "Rail In Cycle"   ;  btMan2_2  -> Tag = (int)mcRAL_In      ; btMan2_2  -> Visible = true  ;
    btMan2_3  -> Caption = "[" + AnsiString(mcRAL_In     ) + "]" + "Rail Work Stt"   ;  btMan2_3  -> Tag = (int)mcRAL_WorkStt ; btMan2_3  -> Visible = true  ;
    btMan2_4  -> Caption = "[" + AnsiString(mcRAL_Work   ) + "]" + "Rail Work"       ;  btMan2_4  -> Tag = (int)mcRAL_Work    ; btMan2_4  -> Visible = true  ;
    btMan2_5  -> Caption = "[" + AnsiString(mcRAL_Out    ) + "]" + "Rail Out"        ;  btMan2_5  -> Tag = (int)mcRAL_Out     ; btMan2_5  -> Visible = true  ;
    btMan2_6  -> Caption =                                         "Rail Idx Up"     ;                                          btMan2_6  -> Visible = true  ;
    btMan2_7  -> Caption =                                         "Rail Idx Dn"     ;                                          btMan2_7  -> Visible = true  ;

    btMan3_1  -> Caption = "[" + AnsiString(mcULD_Home   ) + "]" + "UnLoading Home"  ;  btMan3_1  -> Tag = (int)mcULD_Home    ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcULD_In     ) + "]" + "UnLoading In"    ;  btMan3_2  -> Tag = (int)mcULD_In      ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcULD_Stock  ) + "]" + "UnLoading Stock" ;  btMan3_3  -> Tag = (int)mcULD_Stock   ; btMan3_3  -> Visible = true  ;

    btMan4_1  -> Caption = "[" + AnsiString(mcPCK_Home   ) + "]" + "Pick Home"       ;  btMan4_1  -> Tag = (int)mcPCK_Home    ; btMan4_1  -> Visible = true  ;
    btMan4_2  -> Caption = "[" + AnsiString(mcPCK_Pick   ) + "]" + "Pick"            ;  btMan4_2  -> Tag = (int)mcPCK_Pick    ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcPCK_Place  ) + "]" + "Place Check"     ;  btMan4_3  -> Tag = (int)mcPCK_Place   ; btMan4_3  -> Visible = true  ;
    btMan4_4  -> Caption = "[" + AnsiString(mcPCK_ReZero ) + "]" + "Rezero"          ;  btMan4_4  -> Tag = (int)mcPCK_ReZero  ; btMan4_4  -> Visible = true  ;
    btMan4_5  -> Caption = "[" + AnsiString(mcPCK_Station) + "]" + "Station"         ;  btMan4_5  -> Tag = (int)mcPCK_Station ; btMan4_5  -> Visible = true  ; 

    btMan4_11 -> Caption = "[" + AnsiString(mcPCK_ReZeroCheck ) + "]" + "ReZero Check"    ;  btMan4_11 -> Tag = (int)mcPCK_ReZeroCheck   ; btMan4_11 -> Visible = true  ;


    btMan5_1  -> Caption = "[" + AnsiString(mcSRT_Home   ) + "]" + "Sort Home"       ;  btMan5_1  -> Tag = (int)mcSRT_Home    ; btMan5_1  -> Visible = true  ;
    btMan5_2  -> Caption = "[" + AnsiString(mcSRT_Pick   ) + "]" + "Sort Pick"       ;  btMan5_2  -> Tag = (int)mcSRT_Pick    ; btMan5_2  -> Visible = true  ;
    btMan5_3  -> Caption = "[" + AnsiString(mcSRT_Sort   ) + "]" + "Sort Place"      ;  btMan5_3  -> Tag = (int)mcSRT_Sort    ; btMan5_3  -> Visible = true  ;

    btMan1_14 -> Caption = "[" + AnsiString(mcAllHome    ) + "]" + "ALL HOME"        ;  btMan1_14 -> Tag = (int)mcAllHome     ; btMan1_14 -> Visible = true  ;
    btMan2_14 -> Caption = "[" + AnsiString(mcAllHome    ) + "]" + "ALL HOME"        ;  btMan2_14 -> Tag = (int)mcAllHome     ; btMan2_14 -> Visible = true  ;
    btMan3_14 -> Caption = "[" + AnsiString(mcAllHome    ) + "]" + "ALL HOME"        ;  btMan3_14 -> Tag = (int)mcAllHome     ; btMan3_14 -> Visible = true  ;
    btMan4_14 -> Caption = "[" + AnsiString(mcAllHome    ) + "]" + "ALL HOME"        ;  btMan4_14 -> Tag = (int)mcAllHome     ; btMan4_14 -> Visible = true  ;
    btMan5_14 -> Caption = "[" + AnsiString(mcAllHome    ) + "]" + "ALL HOME"        ;  btMan5_14 -> Tag = (int)mcAllHome     ; btMan5_14 -> Visible = true  ;
    btMan7_14 -> Caption = "[" + AnsiString(mcAllHome    ) + "]" + "ALL HOME"        ;  btMan7_14 -> Tag = (int)mcAllHome     ; btMan7_14 -> Visible = true  ;

    //Init 버튼 없어서 각 메뉴얼 화면에 올홈 버튼 추가함.JS


    pcManual -> ActivePageIndex = 0;


    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        DM.ARAY[i].PopupMenuEnable(true);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormUpdate()
{
    cbBnL0Sel -> ItemIndex = OM.DevOptn.iBnLSel[0] ; 
    cbBnL1Sel -> ItemIndex = OM.DevOptn.iBnLSel[1] ; 
    cbBnL2Sel -> ItemIndex = OM.DevOptn.iBnLSel[2] ; 
    cbBnL3Sel -> ItemIndex = OM.DevOptn.iBnLSel[3] ; 
    cbBnL4Sel -> ItemIndex = OM.DevOptn.iBnLSel[4] ; 
    cbBnL5Sel -> ItemIndex = OM.DevOptn.iBnLSel[5] ; 
    cbBnL6Sel -> ItemIndex = OM.DevOptn.iBnLSel[6] ; 
    cbBnL7Sel -> ItemIndex = OM.DevOptn.iBnLSel[7] ; 
    cbBnL8Sel -> ItemIndex = OM.DevOptn.iBnLSel[8] ; 
    cbBnL9Sel -> ItemIndex = OM.DevOptn.iBnLSel[9] ;


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

    String sCaptionName[MAX_GRADE_STAT] ;
    String sTemp ;
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
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;
    FormUpdate();


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
//                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  DM.ARAY[i].PopupMenuEnable(false);
//                              }
//                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
//                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  DM.ARAY[i].PopupMenuEnable(false);
//                              }
//                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
//                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  DM.ARAY[i].PopupMenuEnable(true);
//                              }
//                              iLev = lvMaster ;
                          }
                          break ;
    }
*/
    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    //Button Start Enabled
//    btStart  -> Enabled = LT.m_bLotOpen ;

    /*
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
        pnLotFailChip  );
    */
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
        NULL            );        
//        pnDayLotCount   );

    pnWorkCycleTime -> Caption = ((int)(PCK.m_dWorkCycleTime))/1000.0 ;

    //if(cbPKGRepeat -> Checked && !SEQ._bRun) {
    //    if(PCK.GetSeqStep() == CPicker::scIdle) {
    //        MM.SetManCycle(mcPCK_Place);
    //    }
    //}
    //else if(cbGazeRepeat -> Checked && !SEQ._bRun) {
    //    if(PCK.GetSeqStep() == CPicker::scIdle) {
    //        MM.SetManCycle(mcPCK_Place);
    //    }
    //}

    if(sgData -> ColCount != OM.DevInfo.iColCnt ||
       sgData -> RowCount != OM.DevInfo.iRowCnt ) {
       sgData -> ColCount = OM.DevInfo.iColCnt ;
       sgData -> RowCount = OM.DevInfo.iRowCnt ;

    }
    AnsiString sTemp ;
    for(int y = 0 ; y < OM.DevInfo.iColCnt ; y++) {
        for(int x = 0 ; x < OM.DevInfo.iRowCnt ; x++) {
            sTemp.sprintf("%.3f",dRotorH[x][y]); // + (dRotorH[x][y] < 0) ? -0.0001 : 0.0001)
            sgData -> Cells[y][x] = sTemp ;





            //StringGrid1 -> Cells[y][x] = dRotorH[x][y] ;
        }
    }

    //EM_
    EM_DisplayList(lbErr);

    //Option
    pnLoadingStop      -> Color = OM.CmnOptn.bLoadingStop       ? clLime : clRed ;
    pnEmptyIgnr        -> Color = OM.CmnOptn.bEmptyIgnr         ? clLime : clRed ;
    pnChuckFailIgnore  -> Color = OM.CmnOptn.bChuckFailIgnore   ? clLime : clRed ;
    pnUseBinFullIgnore -> Color = OM.CmnOptn.bUseBinFullIgnore  ? clLime : clRed ;
    pnUseFailBinIgnore -> Color = OM.CmnOptn.bUseFailBinIgnore  ? clLime : clRed ;

    pnLoadingStop      -> Caption = OM.CmnOptn.bLoadingStop       ? "ON" : "OFF" ;
    pnEmptyIgnr        -> Caption = OM.CmnOptn.bEmptyIgnr         ? "ON" : "OFF" ;
    pnChuckFailIgnore  -> Caption = OM.CmnOptn.bChuckFailIgnore   ? "ON" : "OFF" ;
    pnUseBinFullIgnore -> Caption = OM.CmnOptn.bUseBinFullIgnore  ? "ON" : "OFF" ;
    pnUseFailBinIgnore -> Caption = OM.CmnOptn.bUseFailBinIgnore  ? "ON" : "OFF" ;

    DM.UpdateAray();


    lbGauge1    -> Caption = PCK.m_dPrbData[0]          ;
    sTemp = sTemp.sprintf("%.3f",PCK.m_dPrbData[0] + OM.DevOptn.dMasterOffset[0]);
    lbHeight1   -> Caption = sTemp;
    lbZeroCheck1-> Caption = PCK.m_dZeroPointCheck[0]   ;

    lbGauge2    -> Caption = PCK.m_dPrbData[1]          ;
    sTemp = sTemp.sprintf("%.3f",PCK.m_dPrbData[1] + OM.DevOptn.dMasterOffset[1]);
    lbHeight2   -> Caption = sTemp;
    lbZeroCheck2-> Caption = PCK.m_dZeroPointCheck[1]   ;

    lbGauge3    -> Caption = PCK.m_dPrbData[2]          ;
    sTemp = sTemp.sprintf("%.3f",PCK.m_dPrbData[2] + OM.DevOptn.dMasterOffset[2]);
    lbHeight3   -> Caption = sTemp;
    lbZeroCheck3-> Caption = PCK.m_dZeroPointCheck[2]   ;

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

    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
//    if(LT.m_bLotOpen) {
//        FM_MsgOk("Error" , "LotOpen Please.");
//        return;
//    }

    SEQ._bBtnStart = true ;
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
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);
}
//---------------------------------------------------------------------------

bool __fastcall TFrmOperation::CheckStripDisappear()
{
    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripUnknwon()
{
//    //스트립 사라짐 에러.
//    if (DM.ARAY[riPR2].CheckAllStat(csNone)&& DM.ARAY[riPR1].CheckAllStat(csNone) && IO_GetX(xPRB_3Pkg) ) {
//        if(FM_MsgYesNo("Err " , "[PreBuffer2 Strip Inserted] \nDo you want to Set Strip Data?"     ) == mrYes ) {
//            DM.ARAY[riPR2].SetStat(csUnkwn);
//            if(DM.ARAY[riLD1].CheckAllStat(csNone) || DM.ARAY[riLD1].CheckAllStat(csUnkwn)) DM.ARAY[riPR2].SetLotNo(PRB.GetWorkedLotNo()     ) ;
//            else                                                                            DM.ARAY[riPR2].SetLotNo(DM.ARAY[riLD1].GetLotNo()) ;
//            SEQ._bBtnStart = true ;
//        }
//    }
//
    return true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnEmptyIgnrClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

//    if(FM_GetLevel() == lvOperator) return ;

    switch(iIndex) {
        case 0 :                                                       break ;
        case 1 :                                                       break ;
        case 2 :                                                       break ;
        case 3 : if(!OM.CmnOptn.bUseBinFullIgnore) {
                     if(!DM.ARAY[riPCK].CheckAllStat(csNone) ||
                        !DM.ARAY[riSRT].CheckAllStat(csNone) ||
                        !DM.ARAY[riSTN].CheckAllStat(csNone) ) {
                         FM_MsgTime ("Notification" , "Plz Remove Pick Sorter Station Rotor" , OM.MstOptn.iMgzShowTime);
                         return ;
                     }
                 }

                 break ;
        case 4 : if(!OM.CmnOptn.bUseFailBinIgnore) {
                     if(!DM.ARAY[riPCK].CheckAllStat(csNone) ||
                        !DM.ARAY[riSRT].CheckAllStat(csNone) ||
                        !DM.ARAY[riSTN].CheckAllStat(csNone) ) {
                         FM_MsgTime ("Notification" , "Plz Remove Pick Sorter Station Rotor" , OM.MstOptn.iMgzShowTime);
                         return ;
                     }
                 }

                 break ;

    }

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 : OM.CmnOptn.bLoadingStop      = !OM.CmnOptn.bLoadingStop      ; break ;
        case 1 : OM.CmnOptn.bEmptyIgnr        = !OM.CmnOptn.bEmptyIgnr        ; break ;
        case 2 : OM.CmnOptn.bChuckFailIgnore  = !OM.CmnOptn.bChuckFailIgnore  ; break ;
        case 3 : OM.CmnOptn.bUseBinFullIgnore = !OM.CmnOptn.bUseBinFullIgnore ; break ;
        case 4 : OM.CmnOptn.bUseFailBinIgnore = !OM.CmnOptn.bUseFailBinIgnore ; break ;
    }

    OM.SaveCmnOptn();

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btLbRo0Click(TObject *Sender)
{

    //if(SPL.GetSeqStep()) {
//  //      FM_MsgOk("Confirm","Sorting Left Is Running");
    //    return ;
    //}
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Left Bin " + (String)(iTag) + "?" ;
//    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    //SPL.SetBin(iTag,false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btLbRa0Click(TObject *Sender)
{
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Clear Bin Count" + (String)(iTag) + " ?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SRT.m_iBinCnt[iTag] = 0 ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btRbRo0Click(TObject *Sender)
{
    //if(SPR.GetSeqStep()) {
//  //      FM_MsgOk("Confirm","Sorting Right Is Running");
    //    return ;
    //}
    //int iTag = ((TButton*)Sender)->Tag ;
    //String sTemp = "Remove Right Bin " + (String)(iTag) + "?" ;
    //if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    //SPR.SetBin(iTag,false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btRbRa0Click(TObject *Sender)
{
    //if(SPR.GetSeqStep()){
    //    FM_MsgOk("Confirm","Sorting Right Is Running");
    //    return ;
    //}
    //int iTag = ((TButton*)Sender)->Tag ;
    //String sTemp = "Remove Right Bin " + (String)(iTag) + "All ?" ;
    //if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;
    //
    //SPR.SetBin(iTag,true);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPrbClearClick(TObject *Sender)
{
    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    String  sName= pBtn->Name;

         if(sName == "btPrbClear") { DM.ARAY[riPRB].SetStat(csNone) ; }
    else if(sName == "btWrkClear") { DM.ARAY[riWRK].SetStat(csNone) ; }
    else if(sName == "btWkeClear") { DM.ARAY[riWKE].SetStat(csNone) ; }
    else if(sName == "btPsbClear") { DM.ARAY[riPSB].SetStat(csNone) ; }
    else if(sName == "btPckClear") { DM.ARAY[riPCK].SetStat(csNone) ; }
    else if(sName == "btStnClear") { DM.ARAY[riSTN].SetStat(csNone) ; }
    else if(sName == "btSrtClear") { DM.ARAY[riSRT].SetStat(csNone) ; }

    else return ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::pnLBCnt7Click(TObject *Sender)
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

void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------






void __fastcall TFrmOperation::btSetBinClick(TObject *Sender)
{
    OM.DevOptn.iBnLSel[0] = cbBnL0Sel -> ItemIndex ;
    OM.DevOptn.iBnLSel[1] = cbBnL1Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[2] = cbBnL2Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[3] = cbBnL3Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[4] = cbBnL4Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[5] = cbBnL5Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[6] = cbBnL6Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[7] = cbBnL7Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[8] = cbBnL8Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[9] = cbBnL9Sel -> ItemIndex ; 

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

/*
    pnLBLim0 -> Color = clStat[csRslt0            ] ;  pnRBLim0 -> Color = clStat[csRslt0            ] ;
    pnLBLim1 -> Color = clStat[csRslt1            ] ;  pnRBLim1 -> Color = clStat[csRslt1            ] ;
    pnLBLim2 -> Color = clStat[csRslt2            ] ;  pnRBLim2 -> Color = clStat[csRslt2            ] ;
    pnLBLim3 -> Color = clStat[csRslt3            ] ;  pnRBLim3 -> Color = clStat[csRslt3            ] ;
    pnLBLim4 -> Color = clStat[csRslt4            ] ;  pnRBLim4 -> Color = clStat[csRslt4            ] ;
    pnLBLim5 -> Color = clStat[csRslt5            ] ;  pnRBLim5 -> Color = clStat[csRslt5            ] ;
    pnLBLim6 -> Color = clStat[OM.DevOptn.iBn6Sel ] ;  pnRBLim6 -> Color = clStat[OM.DevOptn.iBn6Sel ] ;
    pnLBLim7 -> Color = clStat[OM.DevOptn.iBn7Sel ] ;  pnRBLim7 -> Color = clStat[OM.DevOptn.iBn7Sel ] ;
    pnLBLim8 -> Color = clStat[OM.DevOptn.iBn8Sel ] ;  pnRBLim8 -> Color = clStat[OM.DevOptn.iBn8Sel ] ;
    pnLBLim9 -> Color = clStat[OM.DevOptn.iBn9Sel ] ;  pnRBLim9 -> Color = clStat[OM.DevOptn.iBn9Sel ] ;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Label10Click(TObject *Sender)
{
    FM_MsgTime("zzz","ssssssssss",5000);    
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::cbBnL9SelChange(TObject *Sender)
{
    OM.DevOptn.iBnLSel[0] = cbBnL0Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[1] = cbBnL1Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[2] = cbBnL2Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[3] = cbBnL3Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[4] = cbBnL4Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[5] = cbBnL5Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[6] = cbBnL6Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[7] = cbBnL7Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[8] = cbBnL8Sel -> ItemIndex ; 
    OM.DevOptn.iBnLSel[9] = cbBnL9Sel -> ItemIndex ; 

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


void __fastcall TFrmOperation::btHeightTestMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    PCK.Rs232_H->sData = "";
    IO_SetY(yPRB_Start, true);
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btHeightTestMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    IO_SetY(yPRB_Start, false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btBinCntClearClick(TObject *Sender)
{

    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Clear All Bin Count?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SRT.ClearBinCnt() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btMan2_6Click(TObject *Sender)
{
    RAL.MoveActr(aiRAL_IndxUD , ccFwd);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btMan2_7Click(TObject *Sender)
{
    RAL.MoveActr(aiRAL_IndxUD , ccBwd);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
    //PCK.GetBin(-0.005);   \



    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    //PCK.Rs232_H->ReadData(_cbInQue, RcvBuff);
    //sRcvMsg = "NG,03      ,+0100,-0050,+0011,+0001,+0001@@" ; //+= (char*)RcvBuff ;
    sRcvMsg ="NG,03      ,-5242,-5228,-5368,+0000,+0000,+0000,+0000,+0000@@"

    Trace("procCom1",RcvBuff);

    if(!sRcvMsg.Pos(",03")) return ;
    if(!sRcvMsg.Pos("@" )) return ;

    sRcvMsg = StringReplace(sRcvMsg, " ", "", TReplaceFlags()<<rfReplaceAll );

    Trace("procCom1 Total",sRcvMsg.c_str());
    sTemp = sRcvMsg.SubString(sRcvMsg.Pos(",03,")+ 4,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos(",03,")+6,3);
    if(TryStrToFloat(sTemp , dTemp)) PCK.m_dPrbData[0] =  dTemp ;
    else                             PCK.m_dPrbData[0] = -9.999 ;

    sTemp = sRcvMsg.SubString(sRcvMsg.Pos(",03,")+10,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos(",03,")+12,3);
    if(TryStrToFloat(sTemp , dTemp)) PCK.m_dPrbData[1] =  dTemp ;
    else                             PCK.m_dPrbData[1] = -9.999 ;

    sTemp = sRcvMsg.SubString(sRcvMsg.Pos(",03,")+16,2) + "." + sRcvMsg.SubString(sRcvMsg.Pos(",03,")+18,3);
    if(TryStrToFloat(sTemp , dTemp)) PCK.m_dPrbData[2] =  dTemp ;
    else                             PCK.m_dPrbData[2] = -9.999 ;


//    sTemp = sRcvMsg.SubString(sRcvMsg.Pos("01,01,")+6,5);
//    동도 이것이 소숫점을 안붙여서 오옴...아나.

    PCK.Rs232_H->sData = sRcvMsg ;

    sRcvMsg  = "" ;

}
//---------------------------------------------------------------------------


