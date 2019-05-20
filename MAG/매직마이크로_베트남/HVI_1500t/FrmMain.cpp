//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmMain.h"
#include "FrmOperation.h"
#include "FrmManual.h"
#include "FrmDevice.h"
#include "FrmDeviceSet.h"
#include "FrmLotOpen.h"
#include "FrmSPC.h"
#include "FrmMaster.h"
#include "FrmRsltBig.h"
#include "FrmOption.h"
#include "FrmInOutState.h"
#include "FrmLimitCnt.h"
#include "FrmVisionRlstSet.h"

//---------------------------------------------------------------------------
#include "MainThread.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "IoState.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
//SMDLL Reference
//---------------------------------------------------------------------------
#include "SMDllDefine.h"
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;

bool bDispUPH = true ;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
    FM_MakeWin(pnMain) ;
//    DM.LoadMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DestroyPart()
{
//    LT.SaveEqpInfo();
//    LT.SaveLotInfo();
//    LT.SaveDayInfo(); //할라면 너네도 클로즈 하나 만들어라..dd
    LT.Close();

    DM.SaveMap();

    LDR.Close();
    PRB.Close();
    RAL.Close();
    HED.Close();
    PSB.Close();
    ULD.Close();

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::btnRunClick(TObject *Sender)
{

    SEQ._bBtnStart = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;
    m_bReset = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnRunMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pnRun -> Color = clLime ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnRunMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pnRun -> Color = clGray ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnStopMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pnStop -> Color = clLime ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnStopMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pnStop -> Color = clGray ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnResetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pnReset -> Color = clYellow ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnResetMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pnReset -> Color = clGray ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnExitClick(TObject *Sender)
{
    //Check Running Status.


    if (SEQ._bRun) {
        FM_MsgOk("Error" , "During Auto Running!!");
        return;
    }

    if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

//    DM.SaveMap();
    tmUpdate->Enabled = false ;
    FormManual    ->tmUpdate->Enabled = false ;

    FormOperation      ->tmUpdate->Enabled = false ;
    FormDevice         ->tmUpdate->Enabled = false ;
    FormLotOpen        ->tmLotName->Enabled = false ;
    FormSPC            ->tmUpdate->Enabled = false ;
    FormMaster         ->tmMaster->Enabled = false ;

    Trace("TFormMain","1");
    DestroyPart();
    Trace("TFormMain","2");

//    LDR.Close();
//    PRB.Close();
//    RAL.Close();
//    HED.Close();
//    PSB.Close();
//    ULD.Close();

    MT_SetServoAll(false) ;
    Close();
    Trace("TFormMain","3");
    Trace("<EXIT>","EXIT");    

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormShow(TObject *Sender)
{
    FormOperation -> Parent = pnMain; FormOperation ->Left = 0 ; FormOperation ->Top = 0 ;
    FormManual    -> Parent = pnMain; FormManual    ->Left = 0 ; FormManual    ->Top = 0 ;
    FormDevice    -> Parent = pnMain; FormDevice    ->Left = 0 ; FormDevice    ->Top = 0 ;
    FormSPC       -> Parent = pnMain; FormSPC       ->Left = 0 ; FormSPC       ->Top = 0 ;
//    FormMaster    -> Parent = pnMain; FormMaster    ->Left = 0 ; FormMaster    ->Top = 0 ;
    FormRsltBig   -> Parent = pnMain; FormRsltBig   ->Left = 0 ; FormRsltBig   ->Top = 0 ;
    ShowPage(0); //Operation Form
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnMenuClick(TObject *Sender)
{
    TButton *Btn = dynamic_cast <TButton *>(Sender);
    SEQ._bAutoMode = false ;
    ClosePage();
    ShowPage(Btn->Tag);

    //랏 엔드시에 부져 꺼지게.
    LT.m_bLotEnd = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::imMenuClick(TObject *Sender)
{
    TImage *Btn = dynamic_cast <TImage *>(Sender);
    SEQ._bAutoMode = false ;
    ClosePage();
    ShowPage(Btn->Tag);

    //랏 엔드시에 부져 꺼지게.
    LT.m_bLotEnd = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::pnMenuClick(TObject *Sender)
{
    TPanel *Btn = dynamic_cast <TPanel *>(Sender);
    SEQ._bAutoMode = false ;
    ClosePage();
    ShowPage(Btn->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ClosePage(void)
{
    FormOperation ->Hide();
    FormManual    ->Hide();
    FormDevice    ->Hide();
    FormDeviceSet ->Hide();
    FormSPC       ->Hide();
    FormMaster    ->Hide();
    FormRsltBig   ->Hide();
    FormInOutState->Hide();
    FM_HideWin();
}
//--------------------------------------------------------------------------
void __fastcall TFormMain::ShowPage(int Index)
{
    pnAuto  ->Color = clGray;
    pnManual->Color = clGray;
    pnAuto  ->Color = clLime;
    pnManual->Color = clLime;
    pnAuto  ->Color = clGray;
    pnManual->Color = clGray;
                            //FM_SetWin(pnMain) ;    FormSPC->Show();
    switch(Index){
        //Operation
        case 0: FormOperation -> Show() ;
                SEQ._bAutoMode = true   ; break ;
        //Manual
        case 1: FormManual    -> Show() ;
                SEQ._bAutoMode = false  ; break ;
        //Lot Info.
        case 2: FormSPC       -> Show() ; break ;
        //Device Setup
        case 3: FormDevice    -> Show() ;
                SEQ._bAutoMode = false  ; break ;
        //MachineSetup
        case 4: FM_SetWin(pnMain) ;
                SEQ._bAutoMode = false  ; break ;
        //MainMenu
        case 5:                           break ;
    }
}


//---------------------------------------------------------------------------

void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    m_iPageIndex = 0 ;

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
    Width  = 1280 ;
    Height = 1024 ;

    //Version
    lbDllVer -> Caption = "Dll " + FM_GetVer() ;
    lbDllAge -> Caption = FM_GetAge() ;

    //Last Device Load
    OM.iTotalChip     = 0 ;
    OM.iTotalFailChip = 0 ;
    OM.LoadLastInfo();
    IO_SetY(yLDR_ZBreak,false );
    IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

//    if(!OM.MstOptn.bDebugMode)
//    {
        MT_SetServoAll(true);
/*
        MT_SetServo((int)miLDR_YTop  , true) ;
        MT_SetServo((int)miLDR_ZTop  , true) ;
        MT_SetServo((int)miLDR_ZBtm  , true) ;
        MT_SetServo((int)miWRK_XVsn  , true) ;
        MT_SetServo((int)miWRK_YVsn  , true) ;
        MT_SetServo((int)miWR1_XCvr  , true) ;
        MT_SetServo((int)miWR2_XCvr  , true) ;
        MT_SetServo((int)miWR3_XCvr  , true) ;
        MT_SetServo((int)miPSB_XTrm  , true) ;
        MT_SetServo((int)miPSB_YTrm  , true) ;
        MT_SetServo((int)miULD_YTop  , true) ;
        MT_SetServo((int)miULD_ZTop  , true) ;
        MT_SetServo((int)miULD_ZBtm  , true) ;
*/
//    }

//    IO_SetY(yLDR_ZBreak,false );
//    IO_SetY(yULD_ZBreak,false );


//    //Last Device Load
//    OM.LoadLastInfo();

    StartThread();
    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnLotAppClick(TObject *Sender)
{
    FormLotOpen->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){

        switch(SEQ._iSeqStat) {
            default        : pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
            break ;
            case ssInit    : lbStat -> Caption = "INIT"      ; lbStat -> Font -> Color = clBlue   ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
            break ;
            case ssError   : lbStat -> Caption = "ERROR"     ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
            break ;
            case ssRunning : lbStat -> Caption = "RUNNING"   ; lbStat -> Font -> Color = clLime   ;
                             pnRun  -> Color = clLime   ;
                             pnStop -> Color = clGray   ;
            break ;
            case ssStop    : lbStat -> Caption = "STOP"      ; lbStat -> Font -> Color = clBlack  ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clRed    ;
            break ;
            case ssWorkEnd : lbStat -> Caption = "LOTEND"    ; lbStat -> Font -> Color = clGray   ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
            break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        : pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
                             break ;

            case ssInit    : lbStat -> Caption = "DEBUG INIT"    ; lbStat -> Font -> Color = clBlue   ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
                             break ;

            case ssError   : lbStat -> Caption = "DEBUG ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
                             break ;

            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;
                             pnRun  -> Color = clLime   ;
                             pnStop -> Color = clGray   ;
                             break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clRed    ;
                             break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;
                             pnRun  -> Color = clGray   ;
                             pnStop -> Color = clGray   ;
                             break ;
        }
    }

    switch (FM_GetLevel()) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }
/*
    static int iLev = -1;
    int iLevel = FM_GetLevel();
    String sTemp ;
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  //if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  //if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                              }
                              iLev = lvEngineer ;
                          }

                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  //if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(true);
                                  sTemp = i ;
                                  Trace("DM.ARAY[i].PopupMenuEnable(true)",sTemp.c_str());
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }
*/
    //강정삼 계장 요청.
    pnTrimSkip -> Visible = OM.CmnOptn.bTrmAllSkip     ? SEQ._bFlick : false ;
    //

    //Btn Enabled
    btnManual   -> Enabled = !SEQ._bRun ;
    btnDevSet   -> Enabled = FM_GetLevel() != lvOperator ;//&& !SEQ._bRun;
    btnMachine  -> Enabled = FM_GetLevel() != lvOperator ;//&& !SEQ._bRun; // dd debug 용으로 잠시 뺌.dd
    btnMainMenu -> Enabled = FM_GetLevel() != lvOperator ;//&& !SEQ._bRun;
    btnExit     -> Enabled = !SEQ._bRun ;

    //처리.
    btnRun -> Enabled = SEQ._bAutoMode ;
    pnRun  -> Enabled = SEQ._bAutoMode ;
    imRun  -> Enabled = SEQ._bAutoMode ;

    pnAuto   -> Color = SEQ._bAutoMode ? clLime : clGray ;
    pnManual -> Color = SEQ._bAutoMode ? clGray : clLime ;


    lbDevice -> Caption = OM.m_sCrntDev ;

    if(IO_GetXUp(xETC_LResetSw) || IO_GetXUp(xETC_RResetSw) || m_bReset) pnReset -> Color = clLime ;
    else                                                                 pnReset -> Color = clGray ;
    m_bReset = false ;

    if(PSB.m_bLotEnded && DM.ARAY[riULD].CheckAllStat(csNone)){ //TODO :: dd ULR 이거 사용여부 미결정.dd
        PSB.m_bLotEnded = false ;

        FormRsltBig -> edTime      -> Text = Now().FormatString("hh:nn:ss")     ;
        FormRsltBig -> edLotNo     -> Text = PSB.EndedLot.sLotNo       ;

        FormRsltBig -> edRslt0     -> Text = PSB.EndedLot.iCntRslt0   ;
        FormRsltBig -> edRslt1     -> Text = PSB.EndedLot.iCntRslt1   ;
        FormRsltBig -> edRslt2     -> Text = PSB.EndedLot.iCntRslt2   ;
        FormRsltBig -> edRslt3     -> Text = PSB.EndedLot.iCntRslt3   ;
        FormRsltBig -> edRslt4     -> Text = PSB.EndedLot.iCntRslt4   ;
        FormRsltBig -> edRslt5     -> Text = PSB.EndedLot.iCntRslt5   ;
        FormRsltBig -> edRslt6     -> Text = PSB.EndedLot.iCntRslt6   ;
        FormRsltBig -> edRslt7     -> Text = PSB.EndedLot.iCntRslt7   ;
        FormRsltBig -> edRslt8     -> Text = PSB.EndedLot.iCntRslt8   ;
        FormRsltBig -> edRslt9     -> Text = PSB.EndedLot.iCntRslt9   ;
        FormRsltBig -> edRsltA     -> Text = PSB.EndedLot.iCntRsltA   ;
        FormRsltBig -> edRsltB     -> Text = PSB.EndedLot.iCntRsltB   ;
        FormRsltBig -> edRsltC     -> Text = PSB.EndedLot.iCntRsltC   ;
        FormRsltBig -> edRsltD     -> Text = PSB.EndedLot.iCntRsltD   ;
        FormRsltBig -> edRsltE     -> Text = PSB.EndedLot.iCntRsltE   ;
        FormRsltBig -> edRsltF     -> Text = PSB.EndedLot.iCntRsltF   ;
        FormRsltBig -> edRsltG     -> Text = PSB.EndedLot.iCntRsltG   ;
        FormRsltBig -> edRsltH     -> Text = PSB.EndedLot.iCntRsltH   ;
        FormRsltBig -> edRsltI     -> Text = PSB.EndedLot.iCntRsltI   ;
        FormRsltBig -> edRsltJ     -> Text = PSB.EndedLot.iCntRsltJ   ;
        FormRsltBig -> edRsltK     -> Text = PSB.EndedLot.iCntRsltK   ;
        FormRsltBig -> edRsltL     -> Text = PSB.EndedLot.iCntRsltL   ;

        FormRsltBig -> edGoodCnt   -> Text = PSB.EndedLot.iWorkChip - PSB.EndedLot.iFailChip ;//DM.ARAY[riWK2].GetCntStat(csRslt14);
        FormRsltBig -> edFailTotal -> Text = PSB.EndedLot.iFailChip    ;
        FormRsltBig -> edWork      -> Text = PSB.EndedLot.iWorkChip    ;//DM.ARAY[riWK2].GetCntStat(csWork  );

        FormRsltBig -> Show() ;
    }

    if(bDispUPH) {
        LT.DispDayInfo(NULL ,
                       NULL ,
                       NULL ,
                       NULL ,
                       NULL ,
                       NULL ,
                       NULL ,
                       pnLotChipUpeh,
                       NULL ,
                       NULL ,
                       NULL ,
                       NULL );
    }
    else {
        pnLotChipUpeh -> Caption = RAL.m_lStrpCycleTime/(float)1000 ;
    }


    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::lbModeClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) FormMaster -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
//    tmUpdate -> Enabled = false ;

    Trace("TFormMain","4");
    FormOperation     ->Close();
    FormManual        ->Close();
    FormDevice        ->Close();
    FormDeviceSet     ->Close();
    FormLotOpen       ->Close();
    FormSPC           ->Close();
    FormOption        ->Close();
    FormLimitCnt      ->Close();
    FormVisionRlstSet ->Close();
    FormMaster        ->Close();
    FormRsltBig       ->Close();
    Trace("TFormMain","5");
    EndThread();
    Trace("TFormMain","6");
    MT_SetServoAll(false);
    Trace("TFormMain","7");
    FM_DelWin();
    Trace("TFormMain","8");

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::pnLotChipUpehClick(TObject *Sender)
{
    bDispUPH = !bDispUPH ;
}
//---------------------------------------------------------------------------







