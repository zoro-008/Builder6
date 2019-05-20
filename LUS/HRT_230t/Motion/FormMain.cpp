//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Form Include
//---------------------------------------------------------------------------
#include "FormMain.h"
#include "FormSPC.h"
#include "FormInform.h"
#include "FormMaster.h"
#include "FormOperation.h"
#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormOption.h"
//---------------------------------------------------------------------------
//Log Include
//---------------------------------------------------------------------------
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Motion Dll Include
//---------------------------------------------------------------------------
#include "MainThread.h"
#include "Sequence.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

AnsiString OnGetVer(const AnsiString &sQuery)
{
   //sQuery "CompanyName", "FileDescription", "FileVersion", "InternalName", "LegalCopyright", "LegalTradeMarks", "OriginalFileName", "ProductName", "ProductVersion", "Comments"

   DWORD dwHandle = 0, dwVersionInfoSize;
   UINT uLength;
   LPVOID pFileInfo, ptr;
   AnsiString sOut; // 리턴될 버전 정보.

   AnsiString filename = Application->ExeName;

   dwVersionInfoSize = GetFileVersionInfoSize(filename.c_str(), &dwHandle);

   pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

   GetFileVersionInfo(filename.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);
   VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &ptr, &uLength);

   if(ptr == NULL) return "ERROR" ;

   WORD *id = (WORD *) ptr;
   AnsiString szString = "\\StringFileInfo\\" + IntToHex(id[0], 4) + IntToHex(id[1], 4) + "\\" + sQuery;

   VerQueryValue(pFileInfo, szString.c_str(), &ptr, &uLength);
   sOut = AnsiString((char *) ptr);
   HeapFree(GetProcessHeap(), 0, pFileInfo );
   return sOut;
}
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
    : TForm(Owner)
{
    //init
    Init();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    FM_MakeWin(pnBase) ;

    //LoadSVL("pSVL");

    StartThread();

    lbName -> Caption = OM.EqpOptn.sModelName ;

//    tmMainSet->Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Init(void)
{
    m_iPageIndex = 0 ;

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
//    Left   = 1921 ;
//    ClientWidth  = 1280 ;
//    ClientHeight = 1024 ;
//    Width  = 1280 ;
//    Height = 1024 ;

    //Version
    //lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

    //Last Device Load
    OM.LoadLastInfo();

    MotorBreakSet();

    MT_SetServoAll(true);

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::MotorBreakSet(void)
{
//Output Control
//    IO_SetY(yLD1_ZBreak,false );
//    IO_SetY(yULD_ZBreak,false );
//    ::Sleep(100);

}

void __fastcall TFrmMain::btOperationShowClick(TObject *Sender)
{
    TButton *pButton = dynamic_cast<TButton *>(Sender);
    ClosePage(m_iPageIndex);
    m_iPageIndex = pButton->Tag;
    ShowPage (m_iPageIndex);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; /*FrmOperation -> Width = 1280 ; FrmOperation -> Height = 870 ;*/ FrmOperation -> Parent = pnBase ;
    FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; /*FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 870 ;*/ FrmDevice    -> Parent = pnBase ;
    FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; /*FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 870 ;*/ FrmDeviceSet -> Parent = pnBase ;
    FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; /*FrmOption    -> Width = 1280 ; FrmOption    -> Height = 870 ;*/ FrmOption    -> Parent = pnBase ;
    FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; /*FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 870 ;*/ FrmSPC       -> Parent = pnBase ;
    FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; //FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

//    pnMenu -> Align  = alBottom ;
//    pnMenu -> Parent = pnBase   ;

    ShowPage (0);

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ; break ;
//                FM_SetLevel(lvOperator); break ;
        case 1: FrmDevice    -> Show() ; break ;
        case 2: FrmOption    -> Show() ; break ;
        case 3: FrmSPC       -> Show() ; break ;
    }
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ClosePage(int index)
{
    //FrmMotorPosition ->Close();
    switch(index)
    {
        case 0: FrmOperation -> Hide(); break ;
        case 1: FrmDevice    -> Hide();
                FrmDeviceSet -> Hide(); break ;
        case 2: FrmOption    -> Hide(); break ;
        case 3: FrmSPC       -> Hide(); break ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Error" , "During Auto Running!!");
        return;
    }

    if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    FrmMain->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;

    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();

    EndThread();

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    MT_SetServoAll(false);

    FM_DelWin();

    Trace("<EXIT>","EXIT");

    //FreeSVL();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btUtilShowClick(TObject *Sender)
{
    ClosePage(m_iPageIndex);

    FM_SetWin(pnBase) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) FrmMaster -> Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    btExit -> Enabled = !SEQ._bRun ;

    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "INIT"          ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "ERROR"         ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunWarn : lbStat -> Caption = "RUNWARN"       ; lbStat -> Font -> Color = clRed    ;                      break ;
            case ssRunning : lbStat -> Caption = "RUNNING"       ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "STOP"          ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "LOTEND"        ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "DEBUG INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "DEBUG ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunWarn : lbStat -> Caption = "RUNWARN"       ; lbStat -> Font -> Color = clRed    ;                      break ;
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    //Equip Name
    if(OM.EqpOptn.sModelName == "" ) lbName -> Caption = "NoName" ;
    else                             lbName -> Caption = OM.EqpOptn.sModelName ;

    //Option Filck
    pnLoadingStop -> Visible = OM.CmnOptn.bFeedingStop   ? SEQ._bFlick : false ;
    pnIgnrHeatErr -> Visible = OM.CmnOptn.bIgnrHeatErr   ? SEQ._bFlick : false ;
    //pnIgnreDoor   -> Visible = OM.CmnOptn.bDoorSkip      ? SEQ._bFlick : false ;

    //Button Enable Level 오퍼레이터 폼 만들고 필요 없어짐.
    //btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    //btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;

    //Job File
    lbDevice -> Caption = OM.m_sCrntDev ;

    //Now Time
    lbTime -> Caption = Now().FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");

    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

