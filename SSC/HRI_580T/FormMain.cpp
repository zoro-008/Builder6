//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"

#include "FormOperation.h"
#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormOption.h"
#include "FormMaster.h"
#include "FormSPC.h"
#include "FormRslt.h"
#include "FormRsltBig.h"
#include "FormInform.h"

#include "MainThread.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "VisnComUnit.h"
#include "EqpComUnit.h"
#include "Rail.h"
#include "LanguageUnit.h"
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
{   //MT_SetServoAll(false);
    FM_MakeWin(pnBase) ;
    IO_SetY(yULD_ZBreak,false) ;
    //init
    Init();
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


/*
    const int disigne_width  = 1440;    // 개발당시 해상도
    const int disigne_height = 1024;
    if (Screen->Width <= 1280)            // 특정 해상도 이하이면.
    {
        Scaled = true;
        Height = Height * Screen->Height / disigne_height;
        Width  = Width  * Screen->Width  / disigne_width;
        ScaleBy(Screen->Width, disigne_width);

        // 강제로 맞춰보기.
//        int Screen_Width  = 1280;
//        int Screen_Height = 1024;
//        Width  = disigne_width;
//        Width  = Width  * Screen_Width  / disigne_width;
//        Height = Height * Screen_Height / disigne_height;
//        ScaleBy(Screen_Width, disigne_width);
    }
*/

    //Run State
//    pnIdleRun     ->  Left = 0 ;  pnIdleRun     -> Top = 0 ;
//    pnManualRun   ->  Left = 0 ;  pnManualRun   -> Top = 51;
//    pnReworkMode  ->  Left = 429; pnReworkMode  -> Top = 51;
//    pnLoadingStop ->  Left = 854; pnLoadingStop -> Top = 0 ;
//    pnIgnreDoor   ->  Left = 854; pnIgnreDoor   -> Top = 51;

    //Version
    lbDllVer -> Caption = "Dll " + FM_GetVer() ;
    lbDllAge -> Caption = FM_GetAge() ;
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

    //Last Device Load
    OM.LoadLastInfo();


    //MT_SetServoAll(true);




    //Network Drive Connect
//    if(!EC.NetDrvConnect(OM.MstOptn.sPreEquipDrv, OM.MstOptn.sPstEquipIp, OM.MstOptn.sPreEquipPath)) FM_MsgOk("ERROR","PreEqipment Net Connect Failed!");
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{

//    EC.NetCancelConnection(OM.MstOptn.sPreEquipDrv) ;
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
    FrmRsltBig   -> Parent = pnBase ; FrmRsltBig   -> Left = 0 ; FrmRsltBig   -> Top = 0 ; FrmRsltBig   -> Width = 1280 ; FrmRsltBig   -> Height = 863 ;

    ShowPage (0);

    //Load Language
    LAG.LanguageLoad(FrmOperation ,FM_GetLan());
    LAG.LanguageLoad(FrmDevice    ,FM_GetLan());
    LAG.LanguageLoad(FrmDeviceSet ,FM_GetLan());
    LAG.LanguageLoad(FrmOption    ,FM_GetLan());
    LAG.LanguageLoad(FrmSPC       ,FM_GetLan());

    tmUpdate -> Enabled = true ;    
    //    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show();  break ;
        case 1: FrmDevice    -> Show();  break ;
        case 2: FrmOption    -> Show();  break ;
        case 3: FrmSPC       -> Show();  break ;
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
    FrmMaster  -> Hide();
    FrmRsltBig -> Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
    //Check Running Status.
//    if (SEQ._bRun) {
//        UserMsg2("Warning" , "종료 할 수 없습니다.\n장비 RUNNING중 입니다.");
//        return;
//    }


    if(FM_MsgYesNo("Confirm","종료 하시겠습니까?") != mrYes) return ;

    //Language Save
    LAG.LanguageSave(FrmOperation ,FM_GetLan());
    LAG.LanguageSave(FrmDevice    ,FM_GetLan());
    LAG.LanguageSave(FrmDeviceSet ,FM_GetLan());
    LAG.LanguageSave(FrmOption    ,FM_GetLan());
    LAG.LanguageSave(FrmSPC       ,FM_GetLan());
        
    MT_SetServoAll(false) ;
    FrmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{

    OM.SaveLastInfo();
    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();
    FrmRsltBig   ->Close();

    tmUpdate -> Enabled = false ;

    EndThread();

    FM_DelWin();    
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
//    FM_MakeWin(pnBase) ;
    StartThread();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel >= lvMaster) FrmMaster -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    btExit -> Enabled = !SEQ._bRun ;

    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){
        switch(SEQ._iSeqStat) {
            default        :                                                                                           break ;
            case ssInit    : lbStat -> Caption = "INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunning : lbStat -> Caption = "RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "WORK END"; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "DEBUG INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "DEBUG ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG WORK END"; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }

    pnLoadingStop -> Visible = false ;
    pnIgnreDoor   -> Visible = OM.CmnOptn.bIgnrDoor   ? SEQ._bFlick : false ;
    pnDryRun      -> Visible = OM.CmnOptn.bDryRun     ? SEQ._bFlick : false ;
    pnVisn1Skip   -> Visible = OM.CmnOptn.bVisn1Skip  ? SEQ._bFlick : false ;
    pnVisn2Skip   -> Visible = OM.CmnOptn.bVisn2Skip  ? SEQ._bFlick : false ;
    pnNoTrim      -> Visible = OM.CmnOptn.bNoTrimFail ? SEQ._bFlick : false ;


    btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;


    lbDevice -> Caption = OM.m_sCrntDev ;



    if(RAL.m_bLotEnded) {
        RAL.m_bLotEnded = false ;

        //TFrmRslt * Form = new TFrmRslt(Application);
        TFrmRsltBig * Form = FrmRsltBig ;
        ClosePage(m_iPageIndex);


        Form -> edTime       -> Text = Now().FormatString("hh:nn:ss")     ;
        Form -> edLotNo      -> Text = RAL.EndedLot.sLotNo       ;//DM.ARAY[riWK2].GetLotNo  (        );

        Form -> edFail       -> Text = RAL.EndedLot.iCntHeatFail ;//DM.ARAY[riWK2].GetCntStat(csFail  );
        Form -> edRslt1      -> Text = RAL.EndedLot.iCntRslt1    ;//DM.ARAY[riWK2].GetCntStat(csRslt1 );
        Form -> edRslt2      -> Text = RAL.EndedLot.iCntRslt2    ;//DM.ARAY[riWK2].GetCntStat(csRslt2 );
        Form -> edRslt3      -> Text = RAL.EndedLot.iCntRslt3    ;//DM.ARAY[riWK2].GetCntStat(csRslt3 );
        Form -> edRslt4      -> Text = RAL.EndedLot.iCntRslt4    ;//DM.ARAY[riWK2].GetCntStat(csRslt4 );
        Form -> edRslt5      -> Text = RAL.EndedLot.iCntRslt5    ;//DM.ARAY[riWK2].GetCntStat(csRslt5 );
        Form -> edRslt6      -> Text = RAL.EndedLot.iCntRslt6    ;//DM.ARAY[riWK2].GetCntStat(csRslt6 );
        Form -> edRslt7      -> Text = RAL.EndedLot.iCntRslt7    ;//DM.ARAY[riWK2].GetCntStat(csRslt7 );
        Form -> edRslt8      -> Text = RAL.EndedLot.iCntRslt8    ;//DM.ARAY[riWK2].GetCntStat(csRslt8 );
        Form -> edRslt9      -> Text = RAL.EndedLot.iCntRslt9    ;//DM.ARAY[riWK2].GetCntStat(csRslt9 );
        Form -> edRslt10     -> Text = RAL.EndedLot.iCntRslt10   ;//DM.ARAY[riWK2].GetCntStat(csRslt10);
        Form -> edRslt11     -> Text = RAL.EndedLot.iCntRslt11   ;//DM.ARAY[riWK2].GetCntStat(csRslt11);
        Form -> edRslt12     -> Text = RAL.EndedLot.iCntRslt12   ;//DM.ARAY[riWK2].GetCntStat(csRslt12);
        Form -> edRslt13     -> Text = RAL.EndedLot.iCntRslt13   ;//DM.ARAY[riWK2].GetCntStat(csRslt13);
        Form -> edRslt14     -> Text = RAL.EndedLot.iCntRslt14   ;//DM.ARAY[riWK2].GetCntStat(csRslt14);

        Form -> edFailTotal  -> Text = RAL.EndedLot.iFailChip    ;
        Form -> edWork       -> Text = RAL.EndedLot.iWorkChip    ;//DM.ARAY[riWK2].GetCntStat(csWork  );

        Form -> Show() ;
    }

    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::lbStatClick(TObject *Sender)
{
//RAL.m_bLotEnded = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------

