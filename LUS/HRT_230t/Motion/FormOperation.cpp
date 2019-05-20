//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
//#include "FormDeviceSet.h"
#include "FormLotOpen.h"

//#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Sort.h"
#include "Heater.h"
#include "Rail.h"
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
    EM_SetDisp(false);

    DM_Init();
    Man_Init();

    pcManual  -> ActivePageIndex = 1;

}
//---------------------------------------------------------------------------
void TFrmOperation::DM_Init ()
{
    DM.ARAY[riRAL].SetParent(pnRAL ); DM.ARAY[riRAL].SetConfig(false , "RAL "); DM.ARAY[riRAL].ClearDispItem();
    DM.ARAY[riWRK].SetParent(pnWRK ); DM.ARAY[riWRK].SetConfig(false , "WRK "); DM.ARAY[riWRK].ClearDispItem();
    DM.ARAY[riRAB].SetParent(pnRAB ); DM.ARAY[riRAB].SetConfig(false , "RAB "); DM.ARAY[riRAB].ClearDispItem();


    DM.ARAY[riRAL].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riRAL].SetDispItem(csUnkwn  ,"Unkwn "     ) ;

    DM.ARAY[riRAB].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riRAB].SetDispItem(csUnkwn  ,"Unkwn "     ) ;

    DM.ARAY[riWRK].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riWRK].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riWRK].SetDispItem(csRslt1  ,"Bin1(G) "   ) ;
    DM.ARAY[riWRK].SetDispItem(csRslt2  ,"Bin2(F) "   ) ;
    DM.ARAY[riWRK].SetDispItem(csRslt3  ,"Bin3 "      ) ;
    DM.ARAY[riWRK].SetDispItem(csRslt4  ,"Bin4 "      ) ;
    DM.ARAY[riWRK].SetDispItem(csRslt5  ,"Bin5 "      ) ;

    //PopupMenuEnable
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        DM.ARAY[i].PopupMenuEnable(true);
    }
    DM.ARAY[riRAL].PopupMenuEnable(true);
    DM.ARAY[riWRK].PopupMenuEnable(true);
}
void TFrmOperation::Man_Init()
{
    //Manual Button Caption
    String sTemp ;
    int    iTag = 0 ;
    iTag = mcAllHome          ; sTemp = "[" + AnsiString(iTag) + "]" + "ALL HOME     "; SetManualBtn(btMan1_1 , sTemp , iTag) ; btMan1_1->Font->Color = clFuchsia ;
    iTag = mcFED_Home         ; sTemp = "[" + AnsiString(iTag) + "]" + "FEEDING HOME "; SetManualBtn(btMan1_2 , sTemp , iTag) ;
    iTag = mcRAL_Home         ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL HOME    "; SetManualBtn(btMan1_3 , sTemp , iTag) ;
    iTag = mcWRK_Home         ; sTemp = "[" + AnsiString(iTag) + "]" + "PROBE HOME   "; SetManualBtn(btMan1_4 , sTemp , iTag) ;
    iTag = mcSRT_Home         ; sTemp = "[" + AnsiString(iTag) + "]" + "SORTING HOME "; SetManualBtn(btMan1_5 , sTemp , iTag) ;

    iTag = mcFED_CycleFeed    ; sTemp = "[" + AnsiString(iTag) + "]" + "FEEDING CYCLE"; SetManualBtn(btMan2_1 , sTemp , iTag) ;
    iTag = mcRAL_CycleWorkIdx ; sTemp = "[" + AnsiString(iTag) + "]" + "INDEX CYCLE  "; SetManualBtn(btMan2_2 , sTemp , iTag) ;
    iTag = mcWRK_CycleWork    ; sTemp = "[" + AnsiString(iTag) + "]" + "PROBE CYCLE  "; SetManualBtn(btMan2_3 , sTemp , iTag) ;
    iTag = mcSRT_Bin1         ; sTemp = "[" + AnsiString(iTag) + "]" + "MOVE BIN1    "; SetManualBtn(btMan2_4 , sTemp , iTag) ; btMan2_4->Font->Color = clGreen ;
    iTag = mcSRT_Bin2         ; sTemp = "[" + AnsiString(iTag) + "]" + "MOVE BIN2    "; SetManualBtn(btMan2_5 , sTemp , iTag) ; btMan2_5->Font->Color = clGreen ;
    iTag = mcSRT_Bin3         ; sTemp = "[" + AnsiString(iTag) + "]" + "MOVE BIN3    "; SetManualBtn(btMan2_6 , sTemp , iTag) ; btMan2_6->Font->Color = clGreen ;
    iTag = mcSRT_Bin4         ; sTemp = "[" + AnsiString(iTag) + "]" + "MOVE BIN4    "; SetManualBtn(btMan2_7 , sTemp , iTag) ; btMan2_7->Font->Color = clGreen ;
    iTag = mcSRT_Bin5         ; sTemp = "[" + AnsiString(iTag) + "]" + "MOVE BIN5    "; SetManualBtn(btMan2_8 , sTemp , iTag) ; btMan2_8->Font->Color = clGreen ;
    iTag = mcAllCycleOne      ; sTemp = "[" + AnsiString(iTag) + "]" + "ONE CYCLE    "; SetManualBtn(btMan2_9 , sTemp , iTag) ;
    iTag = mcMoveBin3         ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL MOVE BIN 3"; SetManualBtn(btMan2_A , sTemp , iTag) ; btMan2_A->Font->Color = clBlue ;
    iTag = mcMoveBin4         ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL MOVE BIN 4"; SetManualBtn(btMan2_B , sTemp , iTag) ; btMan2_B->Font->Color = clBlue ;
    iTag = mcMoveBin5         ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL MOVE BIN 5"; SetManualBtn(btMan2_C , sTemp , iTag) ; btMan2_C->Font->Color = clBlue ;

    iTag = mcRalEndBlow       ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL END BLOW"; SetManualBtn(btMan2_D , sTemp , iTag) ;

    iTag = mcAllHome          ; sTemp = "[" + AnsiString(iTag) + "]" + "ALL HOME     "; SetManualBtn(btMan2_E , sTemp , iTag) ; btMan2_E->Font->Color = clFuchsia ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::SetManualBtn(TBitBtn *_btn , String _sCaption , int _iTag)
{
    _btn -> Caption = _sCaption.Trim() ;
    _btn -> Tag     = _iTag     ;
    _btn -> Visible = true      ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;
    //Oper Level Display
    LV_Display();

    //Lot Info Display
    LT_Display();

    //Error Display
    EM_DisplayList(lbErr);

    //Option Display
    OP_Dispaly();

    //DataMan Update
    DM.UpdateAray();


    if(OM.DevOptn.iEpmtyBin != 1) { pnBinCnt1 -> Caption = SRT.GetBinCnt(1); }
    if(OM.DevOptn.iEpmtyBin != 2) { pnBinCnt2 -> Caption = SRT.GetBinCnt(2); }
    if(OM.DevOptn.iEpmtyBin != 3) { pnBinCnt3 -> Caption = SRT.GetBinCnt(3); }
    if(OM.DevOptn.iEpmtyBin != 4) { pnBinCnt4 -> Caption = SRT.GetBinCnt(4); }
    if(OM.DevOptn.iEpmtyBin != 5) { pnBinCnt5 -> Caption = SRT.GetBinCnt(5); }

    if(OM.DevOptn.iEpmtyBin == 1) { pnBinCnt1 -> Caption = "Empty";}
    if(OM.DevOptn.iEpmtyBin == 2) { pnBinCnt2 -> Caption = "Empty";}
    if(OM.DevOptn.iEpmtyBin == 3) { pnBinCnt3 -> Caption = "Empty";}
    if(OM.DevOptn.iEpmtyBin == 4) { pnBinCnt4 -> Caption = "Empty";}
    if(OM.DevOptn.iEpmtyBin == 5) { pnBinCnt5 -> Caption = "Empty";}

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    //Button Start Enabled
    btStart  -> Enabled = LT.m_bLotOpen ;
    pnRailTemp->Caption = HTR.GetTemp1();
    pnWorkTemp->Caption = HTR.GetTemp2();

    if(HTR.bStopTemp) {
        pnSetTemp1 -> Caption = OM.DevOptn.iStopTemp ; 
        pnSetTemp2 -> Caption = OM.DevOptn.iStopTemp ;
    }
    else {
        pnSetTemp1 -> Caption = OM.DevOptn.iTempSet1 ;
        pnSetTemp2 -> Caption = OM.DevOptn.iTempSet2 ;
    }

    if(IO_GetY(yETC_HeatOn)){
        btHeatOn->Caption = "HEAT ON";
        btHeatOn->Font->Color = clRed;
    }
    else {
        btHeatOn->Caption = "HEAT OFF";
        btHeatOn->Font->Color = clBlack ;
    }

    btMan2_D ->Caption = IO_GetY(yRAL_Blower) ? "RALEND BLOW ON" : "RALEND BLOW OFF" ;

//    btHeatOn->

    pnWorkCycle -> Caption = ((int)(RAL.m_dWorkCycleTime))/1000.0 ;

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void TFrmOperation::LV_Display()
{
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
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }
*/
}
void TFrmOperation::LT_Display()
{
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
        pnLotChipUpeh  ,     
        NULL           ,     
        pnLotWorkChip  ,     
        pnLotFailChip  );    


                             
    LT.DispDayInfo(          
        pnDayRunTime    ,    
        pnDayIdleTime   ,    
        pnDayJamTime    ,    
        pnDayTotalTime  ,    
        pnDayChipUpeh   ,
        NULL            ,    
        pnDayWorkChip   ,    
        pnDayFailChip   ,
        pnDayLotCount   );
}
void TFrmOperation::OP_Dispaly()
{
    pnOptn0 -> Color = OM.CmnOptn.bIgnrHeatErr ? clLime : clRed ; pnOptn0 -> Caption = OM.CmnOptn.bIgnrHeatErr ? "ON" : "OFF" ;
    pnOptn1 -> Color = OM.CmnOptn.bFeedingStop ? clLime : clRed ; pnOptn1 -> Caption = OM.CmnOptn.bFeedingStop ? "ON" : "OFF" ;


}
void __fastcall TFrmOperation::pnOptn0Click(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

    switch(iIndex) {
        case 0 :                                                       break ;
        case 1 :                                                       break ;
        case 2 :                                                       break ;
        case 3 :                                                       break ;
        case 4 :                                                       break ;
        case 5 :                                                       break ;
        case 6 :                                                       break ;
        case 7 :                                                       break ;
    }

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 : OM.CmnOptn.bIgnrHeatErr  = !OM.CmnOptn.bIgnrHeatErr ; break ;
        case 1 : OM.CmnOptn.bFeedingStop  = !OM.CmnOptn.bFeedingStop ; break ;

    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    if(!LT.m_bLotOpen) {
        FM_MsgOk("Error" , "LotOpen Please.");
        return;
    }

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
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::pnBinCnt1DblClick(TObject *Sender)
{
    int iSel = ((TPanel *)Sender) -> Tag ;

    if( FM_MsgYesNo( "Confirm" , (AnsiString(iSel) + "번 Bin을 비우시겠습니까?").c_str() ) != mrYes) return ;

    SRT.ClearBinCnt(iSel);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if( FM_MsgYesNo( "Confirm" , "LOT 을 종료 하시겠습니까?" ) != mrYes) return ;

    Trace("LotEnd",LT.GetCrntLot().c_str());

    SRT.ClearBinCnt(1);
    SRT.ClearBinCnt(2);
    SRT.ClearBinCnt(3);
    SRT.ClearBinCnt(4);
    SRT.ClearBinCnt(5);

    LT.LotInfo.dEndTime = Now();
    LT.WriteLotLog   ();
    LT.WriteLotDayLog();
    LT.LotEnd();

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    FrmLotOpen->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btHeatOnClick(TObject *Sender)
{
    IO_SetY(yETC_HeatOn,!IO_GetY(yETC_HeatOn));
}
//---------------------------------------------------------------------------



void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
    if( FM_MsgYesNo( "Confirm" , "모든 Bin을 비우시겠습니까?") != mrYes) return ;

    SRT.ClearBinCnt(1);
    SRT.ClearBinCnt(2);
    SRT.ClearBinCnt(3);
    SRT.ClearBinCnt(4);
    SRT.ClearBinCnt(5);
}
//---------------------------------------------------------------------------

