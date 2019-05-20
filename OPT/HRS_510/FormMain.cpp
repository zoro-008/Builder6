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
#include "FormOperator.h"
//---------------------------------------------------------------------------
//Log Include
//---------------------------------------------------------------------------
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Motion Dll Include
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#include "MainThread.h"
#include "Sequence.h"
#include "OptionMan.h"
//#include "uRs232c.h"
#include "Sorter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

//TRS232C *Rs232_H;
//TRS232C *RS232C2;
//TRS232C *Rs232_R;

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

    StartThread();

//    tmMainSet->Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Init(void)
{
    m_iPageIndex = 0 ;

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
    Width  = 1280 ;
    Height = 1024 ;

    //Version
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

    //Last Device Load
    OM.LoadLastInfo();

    MotorBreakSet();

    MT_SetServoAll(true);

    InitRs232();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::InitRs232(void)
{
//    if(!Rs232_H->Open(0)) ShowMessage("First  Rs232 Port Open Fail") ;
//    if(!RS232C2->Open(1)) ShowMessage("Second Rs232 Port Open Fail") ;
//    if(!Rs232_R->Open(2)) ShowMessage("Third  Rs232 Port Open Fail") ;

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
    FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1280 ; FrmOperation -> Height = 863 ; FrmOperation -> Parent = pnBase ;
    FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 863 ; FrmDevice    -> Parent = pnBase ;
    FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ; FrmDeviceSet -> Parent = pnBase ;
    FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; FrmOption    -> Width = 1280 ; FrmOption    -> Height = 863 ; FrmOption    -> Parent = pnBase ;
    FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 863 ; FrmSPC       -> Parent = pnBase ;
    FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; //FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

    pnMenu -> Align  = alBottom ;
    pnMenu -> Parent = pnBase   ;

    ShowPage (0);

    FrmOperator  -> Parent = pnBase ; FrmOperator  -> Left = 0 ; FrmOperator  -> Top = 0 ; FrmOperator  -> Width = 1280 ; FrmOperator  -> Height = 918 ;
    FrmOperator  -> Show();

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
                FM_SetLevel(lvOperator); break ;
//        case 0: FrmOperator  -> Show() ; break ;
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
//        case 0: FrmOperator  -> Hide(); break ;
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
    FrmDeviceSet ->Close();
    FrmOperation ->Close();
    FrmDevice    ->Close();

    FrmOption    ->Close();
    FrmMaster    ->Close();

    EndThread();

    Trace("<EXIT>","EXIT");
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    MT_SetServoAll(false);

    FM_DelWin();
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

    //빈번호 순서 Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19
    int    iBinNo ;
    String sTemp  ;

    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "INIT"          ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "ERROR"         ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunWarn : //lbStat -> Caption = "RUNWARN"       ;
                             lbStat -> Font -> Color = clYellow  ;
                             //iBinNo = SRT.isMaxBinNo ;  //
                             //if(iBinNo ==  -1)  sTemp = "Bin Have Not Grade" ;
                             //else if(iBinNo <   10) {sTemp = "LEFT "  + (String)iBinNo       + " Bin Full" ; lbStat -> Font -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[iBinNo   ] ] ; }
                             //else                   {sTemp = "RIGHT " +  String(iBinNo - 10) + " Bin Full" ; lbStat -> Font -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[iBinNo-10] ] ; }
                             lbStat -> Caption = "RUNWARNING"    ;
                             break ;
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
            case ssRunWarn : lbStat -> Caption = "DEBUG RUNWARN" ; lbStat -> Font -> Color = clYellow ;                      break ;
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    //Equip Name
    if(OM.EqpOptn.sModelName == "" ) lbName -> Caption = "NoName" ;
    else                             lbName -> Caption = OM.EqpOptn.sModelName ;

    //Option Filck
    pnLoadingStop       -> Visible = OM.CmnOptn.bLoadingStop       ? SEQ._bFlick : false ;
    pnEmptyIgnr         -> Visible = OM.CmnOptn.bEmptyIgnr         ? SEQ._bFlick : false ;
    pnIgnoreChuckFail   -> Visible = OM.CmnOptn.bChuckFailIgnore   ? SEQ._bFlick : false ;
    pnUseFullBinIgnore  -> Visible = OM.CmnOptn.bUseBinFullIgnore  ? SEQ._bFlick : false ;

    //Button Enable Level 오퍼레이터 폼 만들고 필요 없어짐.
    //btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    //btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;

    //Job File
    lbDevice -> Caption = OM.m_sCrntDev ;

    //Now Time
    lbTime -> Caption = Now().FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");


//TODO ::
    static int iPreLevel = 0 ;

    int iLevel = FM_GetLevel() ;

    if(iPreLevel != iLevel) {
        if(iLevel == lvOperator) {
            FrmOperator -> Show() ;
        }
        else {
            FrmOperator -> Hide() ;
            FrmOperation -> FormUpdate();
        }
        iPreLevel = iLevel ;
    }


    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

