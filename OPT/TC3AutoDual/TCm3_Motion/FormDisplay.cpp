//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Sequence.h"
#include "ErrMan.h"
#include "FormDisplay.h"
#include "FormPassword.h"
#include "FormMsgYesNo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDisplay *FrmDisplay;
//---------------------------------------------------------------------------
__fastcall TFrmDisplay::TFrmDisplay(TComponent* Owner)
        : TForm(Owner)
{
    this -> Top    = 0 ;//989 ;
    this -> Left   = 300;
    this -> Width  = 891;//980 ;//1280 ;//908 ;//1006
    this -> Height = 35  ;

    tmUpdate -> Enabled = true ;

    SetWindowPos(Handle , HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 


//    Screen -> OnActiveControlChange = OnActiveControlChange ;
    //Show() ;

    SEQ.LT.DSP.LoadData("_LT");
    SEQ.RT.DSP.LoadData("_RT");

    tmSave -> Enabled = true ;

}

void __fastcall TFrmDisplay::CreateParams(TCreateParams &Params)
{   
    TForm::CreateParams(Params);   
    Params.ExStyle = WS_EX_APPWINDOW;
    Params.WndParent = GetDesktopWindow();   
}


//---------------------------------------------------------------------------
void __fastcall TFrmDisplay::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    pnLTotal     -> Caption =       SEQ.LT.DSP.Data.iTotalCnt             ;
    pnLPass      -> Caption =       SEQ.LT.DSP.Data.iPassCnt              ;
    pnLYield     -> Caption = (int)(SEQ.LT.DSP.Data.dYield    * 10)/ 10.0 ;
    pnLAvr       -> Caption = (int)(SEQ.LT.DSP.Data.dAvrTime  /100)/ 10.0 ;
    pnLTotalTime -> Caption = (int)(SEQ.LT.DSP.Data.dTotalTime/100)/ 10.0 ;

    pnLAdj       -> Caption = (int)(SEQ.LT.DSP.Data.dAdjTime  /100)/ 10.0 ;
    pnLBond      -> Caption = (int)(SEQ.LT.DSP.Data.dBondTime /100)/ 10.0 ;
    pnLUv        -> Caption = (int)(SEQ.LT.DSP.Data.dUVTime   /100)/ 10.0 ;
    pnLWait      -> Caption = (int)(SEQ.LT.DSP.Data.dWaitTime /100)/ 10.0 ;
    pnLWork      -> Caption = (int)(SEQ.LT.DSP.Data.dWorkTime /100)/ 10.0 ;

    pnRTotal     -> Caption =       SEQ.RT.DSP.Data.iTotalCnt             ;
    pnRPass      -> Caption =       SEQ.RT.DSP.Data.iPassCnt              ;
    pnRYield     -> Caption = (int)(SEQ.RT.DSP.Data.dYield    * 10)/ 10.0 ;
    pnRAvr       -> Caption = (int)(SEQ.RT.DSP.Data.dAvrTime  /100)/ 10.0 ;
    pnRTotalTime -> Caption = (int)(SEQ.RT.DSP.Data.dTotalTime/100)/ 10.0 ;

    pnRAdj       -> Caption = (int)(SEQ.RT.DSP.Data.dAdjTime  /100)/ 10.0 ;
    pnRBond      -> Caption = (int)(SEQ.RT.DSP.Data.dBondTime /100)/ 10.0 ;
    pnRUv        -> Caption = (int)(SEQ.RT.DSP.Data.dUVTime   /100)/ 10.0 ;
    pnRWait      -> Caption = (int)(SEQ.RT.DSP.Data.dWaitTime /100)/ 10.0 ;
    pnRWork      -> Caption = (int)(SEQ.RT.DSP.Data.dWorkTime /100)/ 10.0 ;



    btClr -> Enabled = (FrmPassword -> GetLevel() != lvOperator) &&
                       (SEQ.LT.m_iSeqStat == ssStop) &&
                       (SEQ.RT.m_iSeqStat == ssStop) ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDisplay::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDisplay::FormDeactivate(TObject *Sender)
{
   // Show() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDisplay::Panel2DblClick(TObject *Sender)
{
    //SEQ.LT.DSP.ClearTotalData();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDisplay::Panel19DblClick(TObject *Sender)
{
    //SEQ.RT.DSP.ClearTotalData();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDisplay::tmSaveTimer(TObject *Sender)
{
    tmSave -> Enabled = false;

    SEQ.LT.DSP.SaveData("_LT");
    SEQ.RT.DSP.SaveData("_RT");

    tmSave -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDisplay::btClrClick(TObject *Sender)
{
    if(mrYes!=MsgYesNo("Confirm","Are you sure?")) return ;

    AnsiString sPath = LOG_FOLDER + Now().FormatString("yyyymmdd_hhnnss") + ".csv";

    SEQ.LT.LOG.ChangeOper(sPath);
    SEQ.RT.LOG.ChangeOper(sPath);

    if(mrYes!=MsgYesNo("Confirm","You Want to Clear Count && Time Data?")) return ;

    SEQ.LT.DSP.ClearTotalData() ;
    SEQ.RT.DSP.ClearTotalData() ;

    EM.ClearOcrCnt() ;
}
//---------------------------------------------------------------------------

