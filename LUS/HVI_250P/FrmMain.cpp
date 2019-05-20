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
//---------------------------------------------------------------------------
#include "MainThread.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "DataMan.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "Work.h"
//---------------------------------------------------------------------------
//SMDll Define Header , InterfaceUnit
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
//    DM.ClearMap();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::btnRunClick(TObject *Sender)
{
    bool isAllHomeEnd = true;

//    //Inspect.
//    if (!MT_GetHomeEnd(miWRK_ZPch)){isAllHomeEnd = false; }
//    if (!MT_GetHomeEnd(miPRB_XIdx)){isAllHomeEnd = false; }
//    if (!MT_GetHomeEnd(miPRB_TFlp)){isAllHomeEnd = false; }
//    if (!MT_GetHomeEnd(miWRK_XSrt)){isAllHomeEnd = false; }
//    if (!MT_GetHomeEnd(miPRI_XIdx)){isAllHomeEnd = false; }
//    if (!MT_GetHomeEnd(miPSI_XIdx)){isAllHomeEnd = false; }
//
//    if(!LT.m_bLotOpen) {FM_MsgOk("ERR","Lot Open Please. " ) ; return ;}
//    if(!isAllHomeEnd)  {FM_MsgOk("ERROR","PLEASE All Home ") ; return ;}
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

    Trace("<EXIT>","EXIT");

    FormOperation -> tmUpdate -> Enabled = false ;
    FormManual    -> tmUpdate -> Enabled = false ;
    FormDevice    -> tmUpdate -> Enabled = false ;
    FormSPC       -> tmUpdate -> Enabled = false ;
    FormMaster    -> tmMaster -> Enabled = false ;

    static int i = 0 ;
    AnsiString sTemp = i ;
    Trace("~TFormMain",sTemp.c_str());



//    LDR.Load(false) ;
//    PRB.Load(false) ;
//    WRK.Load(false) ;
//    PSB.Load(false) ;

    MT_SetServoAll(false) ;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormShow(TObject *Sender)
{
    FormOperation -> Parent = pnMain; FormOperation ->Left = 0 ; FormOperation ->Top = 0 ;
    FormManual    -> Parent = pnMain; FormManual    ->Left = 0 ; FormManual    ->Top = 0 ;
    FormDevice    -> Parent = pnMain; FormDevice    ->Left = 0 ; FormDevice    ->Top = 0 ;
    FormSPC       -> Parent = pnMain; FormSPC       ->Left = 0 ; FormSPC       ->Top = 0 ;
    FormMaster    -> Parent = pnMain; FormMaster    ->Left = 0 ; FormMaster    ->Top = 0 ;
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

    //¶ù ¿£µå½Ã¿¡ ºÎÁ® ²¨Áö°Ô.
    LT.m_bLotEnd = false ;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::imMenuClick(TObject *Sender)
{
    TImage *Btn = dynamic_cast <TImage *>(Sender);
    SEQ._bAutoMode = false ;
    ClosePage();
    ShowPage(Btn->Tag);

    //¶ù ¿£µå½Ã¿¡ ºÎÁ® ²¨Áö°Ô.
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
    FormOperation->Hide();
    FormManual   ->Hide();
    FormDevice   ->Hide();
    FormDeviceSet->Hide();
    FormSPC      ->Hide();
    FormMaster   ->Hide();
    FormRsltBig  ->Hide();
    FM_HideWin();
}
//--------------------------------------------------------------------------
void __fastcall TFormMain::ShowPage(int Index)
{
    pnAuto  ->Color = clGray;
    pnManual->Color = clGray;
    pnAuto  -> Color = clLime;
    pnManual-> Color = clLime;
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
    OM.LoadLastInfo();
    IO_SetY(yWRK_ZBreak,false );
//    IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

//    MT_SetServoAll(true);
//    MT_SetServo((int)miLDR_YCmp , true) ;
//    MT_SetServo((int)miLDR_ZCmp , true) ;
    MT_SetServo((int)miWRK_ZPch , true) ;
    MT_SetServo((int)miPRB_XIdx , true) ;
    MT_SetServo((int)miPRB_TFlp , true) ;
    MT_SetServo((int)miWRK_XSrt , true) ;
    MT_SetServo((int)miPRI_XIdx , true) ;
    MT_SetServo((int)miPSI_XIdx , true) ;

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

    static int iLev = -1;
    int iLevel = FM_GetLevel();
    String sTemp ;
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvEngineer ;
                          }

                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(true);
                                  DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }

    //Btn Enabled
    btnManual   -> Enabled = !SEQ._bRun ;
    btnDevSet   -> Enabled = FM_GetLevel() != lvOperator ;//&& (!SEQ._bRun ;
    btnMachine  -> Enabled = FM_GetLevel() != lvOperator ;//&& (!SEQ._bRun;
    btnMainMenu -> Enabled = FM_GetLevel() != lvOperator ;//&& (!SEQ._bRun;
    btnExit     -> Enabled = !SEQ._bRun ;

    btnRun -> Enabled = SEQ._bAutoMode ;
    pnRun  -> Enabled = SEQ._bAutoMode ;
    imRun  -> Enabled = SEQ._bAutoMode ;

    pnAuto   -> Color = SEQ._bAutoMode ? clLime : clGray ;
    pnManual -> Color = SEQ._bAutoMode ? clGray : clLime ;

    //Display Device Name
    lbDevice -> Caption = OM.m_sCrntDev ;

    //Display Reset
    if(IO_GetXUp(xETC_ResetSw)  ||
//       IO_GetXUp(xETC_RResetSw) ||
       m_bReset) pnReset -> Color = clLime ;
    else pnReset -> Color = clGray ;
    m_bReset = false ;

    //WRK Lot End . FormRsltBig Show
    if(WRK.m_bLotEnded ){
        FormRsltBig -> edTime        -> Text = Now().FormatString("hh:nn:ss") ;
        FormRsltBig -> edLotNo       -> Text = WRK.EndedLot.sLotNo            ;
        FormRsltBig -> edWorkedFrame -> Text = WRK.EndedLot.iWorkStrp         ;
        FormRsltBig -> edWorkedChip  -> Text = WRK.EndedLot.iWorkChip         ;

        FormRsltBig -> Show() ;
        WRK.m_bLotEnded = false ;
    }
    //Display Lot Day Info else Display Chip Upeh
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
        pnLotChipUpeh -> Caption = WRK.m_lStrpCycleTime/(float)1000 ;
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
    if (iLevel >= lvMaster) FormMaster -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;

    YouDie();
    FormOperation->Close();
    FormManual   ->Close();
    FormDevice   ->Close();
    FormDeviceSet->Close();
    FormSPC      ->Close();
    FormMaster   ->Close();
    FormRsltBig  ->Close();


    EndThread();

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    MT_SetServo((int)miWRK_ZPch , false) ;
    MT_SetServo((int)miPRB_XIdx , false) ;
    MT_SetServo((int)miPRB_TFlp , false) ;
    MT_SetServo((int)miWRK_XSrt , false) ;
    MT_SetServo((int)miPRI_XIdx , false) ;
    MT_SetServo((int)miPSI_XIdx , false) ;

    FM_DelWin();

//    YouDie();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::pnLotChipUpehClick(TObject *Sender)
{
    bDispUPH = !bDispUPH ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::YouDie()
{
//    DM.~CDataMan();
//    PRB.~CPreBuff();
//    WRK.~CWork();

//--
    DM.SaveMap();
    PRB.Close();
    WRK.Close();
    OM.SaveLastInfo();


}
//---------------------------------------------------------------------------









