//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormMaster.h"
#include "FormSPC.h"

#include "FormOperation.h"
#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormOption.h"
//#include "FormUtil.h"
#include "Timer.h"
#include "LotUnit.h"
#include "Rail.h"


#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
#include "UtilDefine.h"

bool bDispUPH = true ;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

AnsiString OnGetVer(const AnsiString &sQuery)
{
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
    FM_MakeWin(pnBase) ;
    //init
    Init();

    //Visible = false ;  //ssss

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
    lbDllVer -> Caption = "Dll " + FM_GetVer() ;
    lbDllAge -> Caption = FM_GetAge() ;
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

    //Last Device Load
    OM.LoadLastInfo();

    ::Sleep(100);

    MT_SetServoAll(true);

}
//---------------------------------------------------------------------------
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
    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1280 ; FrmOperation -> Height = 863 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 863 ;
    FrmDeviceSet -> Parent = pnBase ; FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ;
    FrmOption    -> Parent = pnBase ; FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; FrmOption    -> Width = 1280 ; FrmOption    -> Height = 863 ;
    FrmSPC       -> Parent = pnBase ; FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 863 ;
    FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

    ShowPage (0);

    tmUpdate -> Enabled = true ;

    //    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
//                FM_SetLevel(lvOperator); break ;
                break ;
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
    FrmMaster -> Hide();
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

//    OM.SaveLastInfo();
    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();

    tmUpdate -> Enabled = false ;

    EndThread();

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btUtilShowClick(TObject *Sender)
{
    ClosePage(m_iPageIndex);

    FM_SetWin(pnBase) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    StartThread();

    //APC Rs-232
//    tmMainSet->Enabled = true ;

    //Ip address check

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) FrmMaster -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    btExit -> Enabled = !SEQ._bRun ;

    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){

        switch(SEQ._iSeqStat) {
            default        :
            break ;
            case ssInit    : lbStat -> Caption = "INIT"      ; lbStat -> Font -> Color = clBlue   ;                       break ;
            case ssError   : lbStat -> Caption = "ERROR"     ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ; break ;
            case ssRunWarn : lbStat -> Caption = "RUNNING"   ; lbStat -> Font -> Color = clLime   ;                       break ;
            case ssRunning : lbStat -> Caption = "RUNNING"   ; lbStat -> Font -> Color = clLime   ;                       break ;
            case ssStop    : lbStat -> Caption = "STOP"      ; lbStat -> Font -> Color = clBlack  ;                       break ;
            case ssWorkEnd : lbStat -> Caption = "LOTEND"    ; lbStat -> Font -> Color = clGray   ;                       break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "DEBUG INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "DEBUG ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunWarn : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    if(OM.CmnOptn.bVs1Skip && OM.CmnOptn.bVs2Skip && OM.CmnOptn.bVs3Skip && OM.CmnOptn.bVsMrkSkip){ OM.CmnOptn.bVsAllSkip = true; }
    else                                                                                          { OM.CmnOptn.bVsAllSkip = false;}
    // 상태 Panel 들 깜박 깜박.
    pnIgnreDoor   -> Visible = OM.CmnOptn.bDoorSkip       ? SEQ._bFlick : false ;
    pnAfterMark   -> Visible = OM.CmnOptn.bAfterMrkRemove ? SEQ._bFlick : false ;
    //pnDryRun      -> Visible = OM.CmnOptn.bDryRun      ? SEQ._bFlick : false ;
    pnInspSkip    -> Visible = OM.CmnOptn.bVsAllSkip      ? SEQ._bFlick : false ;
    pnIgnrMrk     -> Visible = OM.CmnOptn.bMrkAllSkip     ? SEQ._bFlick : false ;
    pnNoTrim      -> Visible = false ;

    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;

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

    lbDevice -> Caption = OM.m_sCrntDev ;

    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{

    MT_SetServoAll(false);

/*
    MT_SetServo((int)miLDR_XPsh , false) ;
    MT_SetServo((int)miLDR_YFed , false) ;
    MT_SetServo((int)miLDR_XPck , false) ;
    MT_SetServo((int)miLDR_ZPck , false) ;
    MT_SetServo((int)miWK1_YFlp , false) ;
    MT_SetServo((int)miWK1_XIns , false) ;
    MT_SetServo((int)miWRK_XVsn , false) ;
    MT_SetServo((int)miWRK_YVsn , false) ;
    MT_SetServo((int)miPSB_XMrk , false) ;
    MT_SetServo((int)miPSB_YMrk , false) ;
    MT_SetServo((int)miULD_ZPck , false) ;
    MT_SetServo((int)miULD_YPck , false) ;
    MT_SetServo((int)miULD_ZTrI , false) ;
    MT_SetServo((int)miULD_ZTrO , false) ;
    MT_SetServo((int)miULD_XStp , false) ;
    MT_SetServo((int)miULD_ZPpr , false) ;
*/

    FM_DelWin();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pnLotChipUpehClick(TObject *Sender)
{
    bDispUPH = !bDispUPH ;
}
//---------------------------------------------------------------------------





