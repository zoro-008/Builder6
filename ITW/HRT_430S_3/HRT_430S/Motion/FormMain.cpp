//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Form Reference
//---------------------------------------------------------------------------
#include "FormMain.h"
#include "FormMaster.h"
#include "FormProbe.h"
#include "FormSPC.h"
#include "FormInform.h"
//---------------------------------------------------------------------------
//Unit Reference
//---------------------------------------------------------------------------
#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "LanguageUnit.h"
#include "SVInterface.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
//Temiral Reference
//---------------------------------------------------------------------------
#include "uRs232c.h"
#include "ProbeComUnit.h"
#include "TempConUnit.h"
//---------------------------------------------------------------------------
//Option Reference
//---------------------------------------------------------------------------
#include "OptionMan.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Sequence Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "SortingTool.h"
#include "UnLoader.h"
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

    //Vision 시작! JS
    AnsiString sPath = EXE_FOLDER + "JobFile\\" + OM.m_sCrntDev;

    if(!LoadSVision("pSVision.dll")){
        ShowMessage("Dll Loading failed");
    }
    SV_InitVisn(1);
    //SV_SetVisnName(0,"Test1");
    SV_SetVisnName(0,"Test1");

    SV_LoadJobFile(sPath.c_str());

    //Last Device Load
    OM.LoadLastInfo();
//    MT_SetServoAll(true);
    IO_SetY(yLDR_ZBreak,false );
    IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_YCmp , true) ;
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_ZCmp , true) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_YCmp , true) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_ZCmp , true) ;
    MT_SetServo((int)miPRB_XCmp , true) ;
    MT_SetServo((int)miWRK_XCmp , true) ;
    MT_SetServo((int)miPSB_XCmp , true) ;
    MT_SetServo((int)miWRK_ZTop , true) ;
    MT_SetServo((int)miWRK_ZBtm , true) ;
    MT_SetServo((int)miSTL_YTop , true) ;
    MT_SetServo((int)miREJ_XBtm , true) ;

//    tmMainSet->Enabled = true ;

    tmUpdate -> Enabled = true ;
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

    LAG.LanguageLoad(FrmOperation, FM_GetLan());
    LAG.LanguageLoad(FrmDevice   , FM_GetLan());
    LAG.LanguageLoad(FrmDeviceSet, FM_GetLan());
    LAG.LanguageLoad(FrmOption   , FM_GetLan());
    LAG.LanguageLoad(FrmSPC      , FM_GetLan());

    ShowPage (0);
    FrmProbe->Show();

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    //static bool bFirst = true ;
    //if(bFirst) {
    //    Application -> ProcessMessages();
    //    if(!LoadSVA("pSVApp.dll")) ShowMessage(ExtractFilePath(Application->ExeName) + "\pSVApp Loading Failed!");
    //
    //    SV_InitVisn(FrmOperation ->   pnOperVisn -> Handle , FrmDeviceSet -> pnSetPara-> Handle , OM.m_sCrntDev.c_str());
    //
    //    bFirst = false ;
    //}

    switch(index)
    {
        case 0: FrmOperation -> Show() ;
                SV_SetVisnForm(0 , FrmOperation -> pnOperVisn -> Handle);
//                FM_SetLevel(lvOperator); break ;
                //SV_SetImgForm(FrmOperation -> pnOperVisn -> Handle , false) ;
                break ;
        case 1: FrmDevice    -> Show() ;
                SV_SetVisnForm(0 , FrmOperation -> pnOperVisn -> Handle);
                //SV_SetImgForm(FrmDeviceSet -> pnSetVisn -> Handle , true ) ;
                break ;
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

    //Language Save.
    LAG.LanguageSave(FrmOperation, FM_GetLan());
    LAG.LanguageSave(FrmDevice   , FM_GetLan());
    LAG.LanguageSave(FrmDeviceSet, FM_GetLan());
    LAG.LanguageSave(FrmOption   , FM_GetLan());
    LAG.LanguageSave(FrmSPC      , FM_GetLan());


    //소멸자 삽입부.
    LDR.Close();
    PRB.Close();
    WRK.Close();
    PSB.Close();
    ULD.Close();

    LT.Close();

    DM.Close();
    OM.Close();

    FrmMain->Close();

    Trace("<@EXIT>","Exit");
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();

    tmUpdate     -> Enabled = false ;
    tmTempuretue -> Enabled = false ;

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
    bool bSuccess = true ;
//    FM_MakeWin(pnBase) ;

    StartThread();

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
            case ssInit    : lbStat -> Caption = "INIT"      ; lbStat -> Font -> Color = clBlue   ;
            break ;
            case ssError   : lbStat -> Caption = "ERROR"     ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;
            break ;
            case ssRunning : lbStat -> Caption = "RUNNING"   ; lbStat -> Font -> Color = clLime   ;
            break ;
            case ssStop    : lbStat -> Caption = "STOP"      ; lbStat -> Font -> Color = clBlack  ;
            break ;
            case ssWorkEnd : lbStat -> Caption = "LOTEND"    ; lbStat -> Font -> Color = clGray   ;
            break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "DEBUG INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "DEBUG ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }

    // 상태 Panel 들 깜박 깜박.
    pnLoadingStop -> Visible = OM.CmnOptn.bLoadingStop ? SEQ._bFlick : false ;
    pnIgnreDoor   -> Visible = OM.CmnOptn.bIgnrDoor    ? SEQ._bFlick : false ;
    pnDryRun      -> Visible = OM.CmnOptn.bDryRun      ? SEQ._bFlick : false ;
    pnInspSkip    -> Visible = OM.CmnOptn.bIgnrInsp    ? SEQ._bFlick : false ;
    pnIgnrTrim    -> Visible = OM.CmnOptn.bIgnrTrim    ? SEQ._bFlick : false ;
    pnNoTrim      -> Visible = false ;

    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;


    lbDevice -> Caption = OM.m_sCrntDev ;

    tmUpdate -> Enabled = true  ;



}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::IdIIcmpReply(TComponent *ASender,
      const TReplyStatus &AReplyStatus)
{
//    if(AReplyStatus.BytesReceived) {
//        if(!EC.NetDrvConnect(OM.MstOptn.sPstEquipDrv, OM.MstOptn.sPstEquipIp, OM.MstOptn.sPstEquipPath)) ShowMessage("Failed to create a network drive");
//    }
//    else ShowMessage("Failed to create a network drive");
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    SV_DelVisn();
    FreeSVision();
    
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_YCmp , false) ;
    if(OM.EqpOptn.bExistLoader  ) MT_SetServo((int)miLDR_ZCmp , false) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_YCmp , false) ;
    if(OM.EqpOptn.bExistUnLoader) MT_SetServo((int)miULD_ZCmp , false) ;

    MT_SetServo((int)miPRB_XCmp , false) ;
    MT_SetServo((int)miWRK_XCmp , false) ;
    MT_SetServo((int)miPSB_XCmp , false) ;
    MT_SetServo((int)miWRK_ZTop , false) ;
    MT_SetServo((int)miWRK_ZBtm , false) ;
    MT_SetServo((int)miSTL_YTop , false) ;
    MT_SetServo((int)miREJ_XBtm , false) ;

    FM_DelWin();

    delete RS232C_Pb1ch ;
    delete RS232C_Pb2ch ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmTempuretueTimer(TObject *Sender)
{
    // juhyeon
    if(OM.MstOptn.bUsedTempRS232) {
        tmTempuretue -> Enabled = false ;
        TCU.RqstCrntTemp();
        tmTempuretue -> Enabled = true ;
    }
}
//---------------------------------------------------------------------------

