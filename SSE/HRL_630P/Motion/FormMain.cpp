//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormMaster.h"
#include "FormSPC.h"
#include "FormInform.h"
#include "FormLoading.h"
#include "FormOperation.h"

#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"

#include<stdio.h>
#include<string.h>
//---------------------------------------------------------------------------
#include "VisnDefine.h"
#include "SAInterface.h"


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

    //init
    Init();




    //Visible = false ;  //ssss

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::Init(void)
{


    FM_MakeWin() ;

    m_iPageIndex = 0 ;

    //FrmMain
    Top    = 0 ;
    Left   = 0 ;
    Width  = 1280 ;
    Height = 1024 ;



    //Version
    lbDllVer -> Caption = "Dll " + AnsiString(FM_GetVer()) ;
    //AnsiString ddf = lbDllVer -> Caption;
    lbDllAge -> Caption = FM_GetAge() ;
    //AnsiString dfh = lbDllAge -> Caption;
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'_ 'm'_ 'd'_ 'AM/PM' 'h': 'n''");
    lbDate -> Caption = Date ;

    Trace( "<START>", Date.c_str()) ;
    Trace( "<@GETBUFFALL>", "") ;

    //Last Device Load
    OM.LoadLastInfo();
    //IO_SetY(yLDR_ZBreak,false );
    //IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);

    //IO_SetY(yFED_InSol    , true);
    //IO_SetY(yFED_OutSol   , true);  //장비 시작 하면 키고...
    IO_SetY(yETC_DustSuck , true);
    IO_SetY(yFED_Vcc1     , true);
    IO_SetY(yFED_Vcc2     , true);
    //IO_SetY(yETC_Feeder   , true);

    //IO_SetY(yETC_DustSuck , false);

    lbName -> Caption = OM.EqpOptn.sModelName;

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
    //FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

    static bool bFirst = true ;
    if(bFirst) {
        //if(!LoadSVA("pSVApp.dll")) ShowMessage(ExtractFilePath(Application->ExeName) + "\pSVApp Loading Failed!");
        //SV_InitVisn(FrmOperation -> pnVisn -> Handle , FrmDeviceSet -> pnVisnPara-> Handle , OM.m_sCrntDev.c_str());

        StartThread();
        bFirst = false ;
    }

    ShowPage (0);



    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
                if(tmUpdate -> Enabled)SV_SetImgForm(FrmOperation -> pnVisn -> Handle , false) ;
                break ;
        case 1: FrmDevice    -> Show()    ;
                SV_SetImgForm(FrmDeviceSet -> pnSetVisn -> Handle , true ) ; break ;
        case 2: FrmOption    -> Show()    ; break ;
        case 3: FrmSPC       -> Show()    ; break ;
        case 4: FM_SetWin(pnBase-> Handle) ; break ;
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
        case 4: FM_HideWin()          ; break ;
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

    IO_SetY(yETC_DustSuck , false);
    IO_SetY(yLSR_VccF     , false);
    IO_SetY(yLSR_VccL     , false);
    IO_SetY(yLSR_EjctF    , false);
    IO_SetY(yLSR_EjctL    , false);
    IO_SetY(yETC_Feeder   , false);


    FrmLoading = new TFrmLoading(Application , false);
    ModalResult = FrmLoading->ShowModal();
    delete FrmLoading;

    tmUpdate -> Enabled = false ;
    EndThread();

    FM_DelWin();

    SV_DelVisn(); // 이거 없음 뻑....

    //FM_DelWin();




    //FreeSML();
    //


    //SV_DelVisn();
    if(!FreeSVA())ShowMessage(ExtractFilePath(Application->ExeName) + "\pSVApp Free Failed!");

    //FreeSML();

    Trace( "<@EXIT>", "<EXIT>") ;

    FrmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
      // 장비 끝나면 끄고...

    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();
    FrmSPC       ->Close();

    //tmUpdate -> Enabled = false ;
    //EndThread();

    //정훈이 주석. JH
    //SV_DelVisn();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    bool bSuccess = true ;



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

    static bool bFirst = true ;

    if(bFirst) {
        Application -> ProcessMessages();
        if(!LoadSVA("pSVApp.dll")) ShowMessage(ExtractFilePath(Application->ExeName) + "\pSVApp Loading Failed!");

        SV_InitVisn(FrmOperation -> pnVisn -> Handle ,  FrmDeviceSet -> pnSetPara-> Handle , OM.m_sCrntDev.c_str());

        bFirst = false ;
    }

    btExit -> Enabled = !SEQ._bRun ;

    Trace("<@SETBUFF_00>","1");

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

    Trace("<@SETBUFF_00>","2");

    // 상태 Panel 들 깜박 깜박.
    //pnLoadingStop -> Visible = OM.CmnOptn.bFeedingStop ? SEQ._bFlick : false ;
    pnIgnreDoor   -> Visible = OM.CmnOptn.bIgnrDoorOpen? SEQ._bFlick : false ;
    pnDryRun      -> Visible = OM.MstOptn.bDryRun      ? SEQ._bFlick : false ;

    Trace("<@SETBUFF_00>","3");

    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;

    Trace("<@SETBUFF_00>","4");

    btExit -> Enabled = SEQ._iSeqStat == ssRunning ? false : true ;

    Trace("<@SETBUFF_00>","5");

    lbDevice -> Caption = OM.m_sCrntDev ;

    Trace("<@SETBUFF_00>","0");

    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------
















