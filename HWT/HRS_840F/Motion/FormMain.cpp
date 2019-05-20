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
#include "FormDevice.h"
#include "LanguageUnit.h"

#include "Loader.h"
#include "PreRail.h"
#include "PostRail.h"
#include "UnLoader.h"

#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "UserINI.h"
#include <stdio.h>
#include <string.h>
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
    Width  = 1024 ;
    Height = 768 ;

    //Version
    //lbDllVer -> Caption = "Dll " + AnsiString(FM_GetVer()) ;
    lbDllAge -> Caption = FM_GetAge() ;
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'_ 'm'_ 'd'_ 'AM/PM' 'h': 'n''");
    lbDate -> Caption = Date ;
    lbName -> Caption = OM.EqpOptn.sModelName;
    //Last Device Load


    AnsiString sPath = EXE_FOLDER + "JobFile\\" + OM.m_sCrntDev;


    OM.LoadLastInfo();
//    IO_SetY(yETC_MainAirSol , true); // IO 삭제됨
//    MT_SetServoAll(true);

//    pnSkipDisp -> Left = 0 ; // 오브젝트 트리뷰에서도 못 찾겠습니다 (디스펜서인지, 디스플레이인지 모르겠지만;;)
//    pnSkipDisp -> Top  = 0 ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{

    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; //FrmOperation -> Width = 1024 ; FrmOperation -> Height = 626 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; //FrmDevice    -> Width = 1024 ; FrmDevice    -> Height = 626 ;
//    FrmDeviceSet -> Parent = pnBase ; FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ;
    FrmOption    -> Parent = pnBase ; FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; //FrmOption    -> Width = 1024 ; FrmOption    -> Height = 626 ;
//    FrmSPC       -> Parent = pnBase ; FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; //FrmSPC       -> Width = 1024 ; FrmSPC       -> Height = 626 ;
//    FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;


    ShowPage (0);

    LAG.LanguageSave(FrmOperation, FM_GetLan());
    LAG.LanguageSave(FrmDevice   , FM_GetLan());
    LAG.LanguageSave(FrmOption   , FM_GetLan());

    static bool bFirst = true ;

    if(bFirst) {
        StartThread();
        bFirst = false ;
    }

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
                FM_SetLevel(lvOperator); break;

        case 1: FrmDevice    -> Show()     ; break ;
        case 2: FrmOption    -> Show()     ; break ;
//        case 3: FrmSPC       -> Show()     ; break ;
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
        case 1: FrmDevice    -> Hide(); break ;
//                FrmDeviceSet -> Hide(); break ;
        case 2: FrmOption    -> Hide(); break ;
//        case 3: FrmSPC       -> Hide(); break ;
        case 4: FM_HideWin()          ; break ;
    }
    FrmMaster -> Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("에러 발생" , "현재 장비가 동작 중입니다!!");
        return;
    }

    if(FM_MsgYesNo("확인", "종료 하시겠습니까??") != mrYes) return ;
    tmUpdate -> Enabled = false ;
    EndThread();

    //Language Save.
    LAG.LanguageSave(FrmOperation, FM_GetLan());
    LAG.LanguageSave(FrmDevice   , FM_GetLan());
    LAG.LanguageSave(FrmOption   , FM_GetLan());

    FrmLoading = new TFrmLoading(Application , false);
    ModalResult = FrmLoading->ShowModal();
    delete FrmLoading;

    FM_DelWin();
    //FreeSML();

    Trace( "<EXIT>", "<EXIT>") ;

    LDR_F.Close();
    LDR_R.Close();
    PRE_F.Close();
    PRE_R.Close();
    PST_F.Close();
    PST_R.Close();
    ULD_F.Close();
    ULD_R.Close();

    LOT.Close();
    DM.Close();
    OM.Close();

    FrmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    FrmOperation ->Close();
    FrmDevice    ->Close();
//    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
   int i;
   i++ ;
}
//---------------------------------------------------------------------------
__fastcall TFrmMain::~TFrmMain()
{

}


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

//    btExit -> Enabled = !SEQ._bRun ;
    String sSkipMsg = "" ;

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
    /*
    if(OM.CmnOptn.bIgnrDoor   ) sSkipMsg += "IGNR DOOR,"  ;
    if(OM.CmnOptn.bLoadingStop) sSkipMsg += "LOAD STOP,"  ;
    if(OM.MstOptn.bVacErrIgnr ) sSkipMsg += "IGNR VACM,"   ;
    if(OM.CmnOptn.bSkipOST    ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "OST,"   ;}
    if(OM.CmnOptn.bSkipVT     ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VT,"    ;}
    if(OM.CmnOptn.bSkipVisn1  ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VISN1," ;}
    if(OM.CmnOptn.bSkipVisn2  ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VISN2," ;}
    if(OM.CmnOptn.bSkip1stTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK1,"  ;}
    if(OM.CmnOptn.bSkip2ndTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK2,"  ;}
    if(OM.CmnOptn.bSkip3thTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK3,"  ;}
    if(OM.CmnOptn.bSkip4thTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK4,"  ;}
    */

//    pnSkipDisp -> Caption = sSkipMsg ;                             // 이 2개가 무엇인고...
//    pnSkipDisp -> Visible = sSkipMsg != "" ? SEQ._bFlick : false ;

//    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
//    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;


    lbDevice -> Caption = OM.GetCrntDev() ;

    lbLotNo -> Caption = LOT.GetLotNo() ;

    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
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

