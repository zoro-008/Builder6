//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"
//---------------------------------------------------------------------------
//#include "Feed.h"
//#include "FrontWork.h"
//#include "BackWork.h"
//#include "Transfer.h"

//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "sLogUnit.h"
#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "LaserConUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    DM.ARAY[riFFD].SetParent(pnFrtFed ); DM.ARAY[riFFD].SetConfig(false , "FFD"); DM.ARAY[riFFD].ClearDispItem();
    DM.ARAY[riLFD].SetParent(pnLerFed ); DM.ARAY[riLFD].SetConfig(false , "LFD"); DM.ARAY[riLFD].ClearDispItem();
    DM.ARAY[riFLS].SetParent(pnFrtLsr ); DM.ARAY[riFLS].SetConfig(false , "FLS"); DM.ARAY[riFLS].ClearDispItem();
    DM.ARAY[riLLS].SetParent(pnLerLsr ); DM.ARAY[riLLS].SetConfig(false , "LLS"); DM.ARAY[riLLS].ClearDispItem();

    DM.ARAY[riFFD].SetDispItem(csNone   ,"Empty  ") ;
    DM.ARAY[riFFD].SetDispItem(csUnkwn  ,"Exist  ") ;
    DM.ARAY[riFFD].SetDispItem(csGood   ,"Align  ") ;

    DM.ARAY[riLFD].SetDispItem(csNone   ,"Empty  ") ;
    DM.ARAY[riLFD].SetDispItem(csUnkwn  ,"Exist  ") ;
    DM.ARAY[riLFD].SetDispItem(csGood   ,"Align  ") ;

    DM.ARAY[riFLS].SetDispItem(csNone   ,"Empty  ") ;
    DM.ARAY[riFLS].SetDispItem(csUnkwn  ,"Exist  ") ;
    DM.ARAY[riFLS].SetDispItem(csWork   ,"Worked ") ;
    DM.ARAY[riFLS].SetDispItem(csWait   ,"Laser  ") ;

    DM.ARAY[riLLS].SetDispItem(csNone   ,"Empty  ") ;
    DM.ARAY[riLLS].SetDispItem(csUnkwn  ,"Exist  ") ;
    DM.ARAY[riLLS].SetDispItem(csWork   ,"Worked ") ;           
    DM.ARAY[riLLS].SetDispItem(csWait   ,"Laser  ") ;


    btMan1_1  -> Caption = "[" + AnsiString(mcETC_Airblow1) + "]" + " AirBlow1 ON/OFF"  ;  btMan1_1  -> Tag = (int)mcETC_Airblow1; btMan1_1  -> Visible = true  ;
    btMan1_2  -> Caption = "[" + AnsiString(mcETC_Airblow2) + "]" + " AirBlow2 ON/OFF"  ;  btMan1_2  -> Tag = (int)mcETC_Airblow2; btMan1_2  -> Visible = true  ;
    btMan1_3  -> Caption = "[" + AnsiString(mcETC_Vcc1    ) + "]" + " Feed VC1 ON/OFF"  ;  btMan1_3  -> Tag = (int)mcETC_Vcc1    ; btMan1_3  -> Visible = true  ;
    btMan1_4  -> Caption = "[" + AnsiString(mcETC_Vcc2    ) + "]" + " Feed VC2 ON/OFF"  ;  btMan1_4  -> Tag = (int)mcETC_Vcc2    ; btMan1_4  -> Visible = true  ;
    btMan1_5  -> Caption = "[" + AnsiString(mcETC_Covr    ) + "]" + " Cover OP/CLS"     ;  btMan1_5  -> Tag = (int)mcETC_Covr    ; btMan1_5  -> Visible = true  ;
    btMan1_6  -> Caption = "[" + AnsiString(mcETC_Algn    ) + "]" + " Align FWD/BWD"    ;  btMan1_6  -> Tag = (int)mcETC_Algn    ; btMan1_6  -> Visible = true  ;
    btMan1_7  -> Caption = "[" + AnsiString(mcFFD_Stage   ) + "]" + " F-Stage FWD/BWD"  ;  btMan1_7  -> Tag = (int)mcFFD_Stage   ; btMan1_7  -> Visible = true  ;
    btMan1_8  -> Caption = "[" + AnsiString(mcFFD_Vcc     ) + "]" + " F-STG VCC ON/OFF" ;  btMan1_8  -> Tag = (int)mcFFD_Vcc     ; btMan1_8  -> Visible = true  ;
    btMan1_9  -> Caption = "[" + AnsiString(mcFFD_Ejct    ) + "]" + " F-STG EJT ON/OFF" ;  btMan1_9  -> Tag = (int)mcFFD_Ejct    ; btMan1_9  -> Visible = true  ;
    btMan1_10 -> Caption = "[" + AnsiString(mcFFD_In      ) + "]" + " F-FEED Cycle In"  ;  btMan1_10 -> Tag = (int)mcFFD_In      ; btMan1_10 -> Visible = true  ;
    btMan1_11 -> Caption = "[" + AnsiString(mcFFD_Out     ) + "]" + " F-FEED Cycle Out" ;  btMan1_11 -> Tag = (int)mcFFD_Out     ; btMan1_11 -> Visible = true  ;
    btMan1_12 -> Caption = "[" + AnsiString(mcFFD_Align   ) + "]" + " F-STG Cycle Align";  btMan1_12 -> Tag = (int)mcFFD_Align   ; btMan1_12 -> Visible = true  ;
    btMan1_13 -> Caption = "[" + AnsiString(mcFLS_Work    ) + "]" + " F-STG Cycle Laser";  btMan1_13 -> Tag = (int)mcFLS_Work    ; btMan1_13 -> Visible = true  ;
    btMan1_14 -> Caption = "[" + AnsiString(mcFLS_Place   ) + "]" + " F-STG Cycle Place";  btMan1_14 -> Tag = (int)mcFLS_Place   ; btMan1_14 -> Visible = true  ;

    btMan2_1  -> Caption = "[" + AnsiString(mcETC_Airblow1) + "]" + " AirBlow1 ON/OFF"  ;  btMan2_1  -> Tag = (int)mcETC_Airblow1; btMan2_1  -> Visible = true  ;
    btMan2_2  -> Caption = "[" + AnsiString(mcETC_Airblow2) + "]" + " AirBlow2 ON/OFF"  ;  btMan2_2  -> Tag = (int)mcETC_Airblow2; btMan2_2  -> Visible = true  ;
    btMan2_3  -> Caption = "[" + AnsiString(mcETC_Vcc1    ) + "]" + " Feed VC1 ON/OFF"  ;  btMan2_3  -> Tag = (int)mcETC_Vcc1    ; btMan2_3  -> Visible = true  ;
    btMan2_4  -> Caption = "[" + AnsiString(mcETC_Vcc2    ) + "]" + " Feed VC2 ON/OFF"  ;  btMan2_4  -> Tag = (int)mcETC_Vcc2    ; btMan2_4  -> Visible = true  ;
    btMan2_5  -> Caption = "[" + AnsiString(mcETC_Covr    ) + "]" + " Cover OP/CLS"     ;  btMan2_5  -> Tag = (int)mcETC_Covr    ; btMan2_5  -> Visible = true  ;
    btMan2_6  -> Caption = "[" + AnsiString(mcETC_Algn    ) + "]" + " Align FWD/BWD"    ;  btMan2_6  -> Tag = (int)mcETC_Algn    ; btMan2_6  -> Visible = true  ;
    btMan2_7  -> Caption = "[" + AnsiString(mcLFD_Stage   ) + "]" + " R-Stage FWD/BWD"  ;  btMan2_7  -> Tag = (int)mcLFD_Stage   ; btMan2_7  -> Visible = true  ;
    btMan2_8  -> Caption = "[" + AnsiString(mcLFD_Vcc     ) + "]" + " R-STG VCC ON/OFF" ;  btMan2_8  -> Tag = (int)mcLFD_Vcc     ; btMan2_8  -> Visible = true  ;
    btMan2_9  -> Caption = "[" + AnsiString(mcLFD_Ejct    ) + "]" + " R-STG EJT ON/OFF" ;  btMan2_9  -> Tag = (int)mcLFD_Ejct    ; btMan2_9  -> Visible = true  ;
    btMan2_10 -> Caption = "[" + AnsiString(mcLFD_In      ) + "]" + " R-FEED Cycle In"  ;  btMan2_10 -> Tag = (int)mcLFD_In      ; btMan2_10 -> Visible = true  ;
    btMan2_11 -> Caption = "[" + AnsiString(mcLFD_Out     ) + "]" + " R-FEED Cycle Out" ;  btMan2_11 -> Tag = (int)mcLFD_Out     ; btMan2_11 -> Visible = true  ;
    btMan2_12 -> Caption = "[" + AnsiString(mcLFD_Align   ) + "]" + " R-STG Cycle Align";  btMan2_12 -> Tag = (int)mcLFD_Align   ; btMan2_12 -> Visible = true  ;
    btMan2_13 -> Caption = "[" + AnsiString(mcLLS_Work    ) + "]" + " R-STG Cycle Laser";  btMan2_13 -> Tag = (int)mcLLS_Work    ; btMan2_13 -> Visible = true  ;
    btMan2_14 -> Caption = "[" + AnsiString(mcLLS_Place   ) + "]" + " R-STG Cycle Place";  btMan2_14 -> Tag = (int)mcLLS_Place   ; btMan2_14 -> Visible = true  ;

    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;


    SEQ.Reset();
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

    int iLevel;
    static int iLev = -1;

    Trace("<@SETBUFF_01>","1");

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    //Panel3 ->Caption = DM.ARAY[riTRS_B].GetLotNo();

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

    Trace("<@SETBUFF_01>","2");

    SPC.LOT.DispLotInfo(sgLotInfo);

    Trace("<@SETBUFF_01>","3");


    SPC.DAY.DispDayInfo(sgDayInfo);

    Trace("<@SETBUFF_01>","4");


    //EM_Disp
    AnsiString Str;
    static int iPreErrCnt  = 0;
    int        iCrntErrCnt = 0;
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
                Str += EM_GetErrName(i);
                lbErr -> Items -> Add(Str);
                if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
            }
        }
    }
    iPreErrCnt = iCrntErrCnt ;

    Trace("<@SETBUFF_01>","5");




    pnIgnrDoor     -> Color  =  OM.CmnOptn.bIgnrDoorOpen ? clLime : clRed ;
    pnSkipLaserWrk -> Color  =  OM.CmnOptn.bSkipLaserWrk ? clLime : clRed ;


    pnIgnrDoor     -> Caption  =  OM.CmnOptn.bIgnrDoorOpen ? "ON" : "OFF" ;
    pnSkipLaserWrk -> Caption  =  OM.CmnOptn.bSkipLaserWrk ? "ON" : "OFF" ;


    //btLotOpen -> Enabled = !OM.MstOptn.bNotUstLot ;
    //btLotEnd  -> Enabled = !OM.MstOptn.bNotUstLot ;

    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;

    Trace("<@SETBUFF_01>","6");


    //메뉴얼 버튼 색깔 칠하기.
    for(int i = 1 ; i < pcManual -> PageCount+1 /*6 페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;
            if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
            else                             pBtn -> Font -> Color = clBlack ;
        }
    }

    Trace("<@SETBUFF_01>","7");


    lbFrntCnt -> Caption = DM.ARAY[riFFD].GetCntStat(csUnkwn);
    lbBackCnt -> Caption = DM.ARAY[riLFD].GetCntStat(csUnkwn);

    Trace("<@SETBUFF_01>","8");


    if(SEQ._bRun)BitBtn1 -> Enabled = false;
    else         BitBtn1 -> Enabled = true ;

    Trace("<@SETBUFF_01>","0");


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
    if(!LTQ.GetLotOpen()) {FM_MsgOk("ERROR" , "Can't Use While Lot Closed"); return ;}
    if(FM_MsgYesNo("Confirm","자제통을 비우고 스타트를 눌러 주세요") != mrYes) return ;

    LTQ.PopLotNo();

    IO_SetY(yETC_Feeder , false);

    LTQ.LotEnd ();

    OM.MstOptn.sCrntLotID = "";
    OM.SaveMstOptn();




}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

    if (FM_MsgYesNo("Confirm", "Would you like to invert the Option?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                        break ;
        case 1 : OM.CmnOptn.bIgnrDoorOpen = !OM.CmnOptn.bIgnrDoorOpen ; break ;
        case 2 : OM.CmnOptn.bSkipLaserWrk = !OM.CmnOptn.bSkipLaserWrk ; break ;
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
void __fastcall TFrmOperation::btPR1ClearClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

    int iTag = ((TButton*)Sender) -> Tag ;

    int r=0,c=0;


    AnsiString sLotNo , sSlotNo ,sMsg;
    switch (iTag)
    {
        default :                                           break;
        case 0 : DM.ARAY[riLFD].SetStat(csNone);          break;
        case 1 : DM.ARAY[riFFD].SetStat(csNone);          break;
        case 2 : DM.ARAY[riLLS  ].SetStat(csNone);
                 IO_SetY(yLSR_VccL  , false);
                 IO_SetY(yLSR_EjctL , false);               break;
        case 3 : DM.ARAY[riFLS  ].SetStat(csNone);
                 IO_SetY(yLSR_VccF  , false);
                 IO_SetY(yLSR_EjctF , false);               break;
        case 4 : c = DM.ARAY[riLFD].FindLastCol (csNone);
                     DM.ARAY[riLFD].SetStat(r,c,csUnkwn); break;
        case 5 : c = DM.ARAY[riFFD].FindLastCol (csNone);
                     DM.ARAY[riFFD].SetStat(r,c,csUnkwn); break;
    }

}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    SPC.DAY.ClearData() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------







void __fastcall TFrmOperation::btUWorkCntResetClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you Want to Reset the User Count?") != mrYes) return ;
    SPC.LOT.LoadSaveUserIni(false);

    SPC.m_iUserCnt++;
    SPC.m_iUserWorkCnt = 0;

    Trace("UserWorkCnt","Reset");

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btAllHomeClick(TObject *Sender)
{
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::BitBtn1Click(TObject *Sender)
{
    IO_SetY(yETC_Feeder   , !IO_GetY(yETC_Feeder));
    IO_SetY(yETC_DustSuck , true);
}
//---------------------------------------------------------------------------

