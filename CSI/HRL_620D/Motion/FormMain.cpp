//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Form Include
//---------------------------------------------------------------------------
#include "FormMain.h"
#include "FormInform.h"
#include "FormOperation.h"
#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormLoading.h"
#include "FormMaster.h"
//---------------------------------------------------------------------------
//Log Include
//---------------------------------------------------------------------------
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Dll Include
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "MainThread.h"
#include "Sequence.h"
#include "Stage.h"
#include "OptionMan.h"
#include "PstnMan.h"
//#include "LaserConUnit.h"
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
    Init();

    EM_SetDisp(true);
    
    IO_SetY(yETC_LaserWrkVcc , true);

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    FM_MakeWin();
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
    lbVer -> Caption = "Ver " + OnGetVer("FileVersion") ;
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbDate -> Caption = Date ;

    //LCU.Init(); jh....레이저 이닛을 어디다 해야 할지...

    MotorBreakSet();

//TODO::    MT_SetServoAll(true);

}

//---------------------------------------------------------------------------
#include "Rs232Man.h"
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
    //SEQ.Reset();

    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Error" , "런닝 중 입니다!!");
        return;
    }

    if(FM_MsgYesNo("Confirm","종료 할까요?") != mrYes) return ;

    IO_SetY(yETC_LaserWrkVcc , false);

    //LCU.Close();
    LT .Close();
    STG.Close();

    EndThread();
    tmUpdate -> Enabled = false ;

    //FraCyl ->tmUpdate->Enabled = false ;
    //FraMotr->tmUpdate->Enabled = false ;

    FrmOperation -> tmUpdate -> Enabled = false ;
    FrmDevice    -> tmUpdate -> Enabled = false ;
    FrmDeviceSet -> tmUpdate -> Enabled = false ;

    //FrmOption    ->tmUpdate->Enabled = false ;
    //RSM.Close() ;

    FM_HideWin();

    //FM_DelWin();  //EndThread 후에. 아... 모르겠다...
    //FreeSML(); //이상하게 뻑남...

    FrmOperation -> Close();
    FrmDevice    -> Close();
    FrmDeviceSet -> Close();

    Trace("","");

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    Trace("","");
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    Trace("","");

}

__fastcall TFrmMain::~TFrmMain()
{
    Trace("","");
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::MotorBreakSet(void)
{

}

void __fastcall TFrmMain::btOperationShowClick(TObject *Sender)
{
    TButton *pButton = dynamic_cast<TButton *>(Sender);
    ClosePage(m_iPageIndex);
    m_iPageIndex = pButton->Tag;
    if(m_iPageIndex == 2) {
        FrmDeviceSet -> EditEnable();
        PM.UpdatePstn(toTabl);
        FrmDeviceSet -> UpdateDevOptn(toTabl);
    }
    ShowPage (m_iPageIndex);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ;  FrmOperation -> Parent = pnBase ;
    FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ;  FrmDevice    -> Parent = pnBase ;
    FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ;  FrmDeviceSet -> Parent = pnBase ;
                                                                       
    ShowPage (0);

    static bool bFirst = true ;
    if(bFirst) StartThread();//FM_SetWin (pnBase->Handle);
    bFirst = false ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0:
                Trace("ShowPage","0");
                FrmOperation -> Show() ;
                Trace("ShowPage","1");

                break ;
//                FM_SetLevel(lvOperator); break ;
        case 1: FrmDevice    -> Show() ;
                //<sun>   SV_SetPaintLocking(true);
                //<sun>   SV_SetPanelProcLocking(true);
                //<sun>   SV_SetVisnTimer  (false);  //타이머 끄고.
                //<sun>   SV_Live(viOri , false);
                //<sun>   SV_Live(viWfr , false);
                //<sun>   SV_Live(viRat , false);
                //<sun>   SV_Live(viRbf , false);
                //<sun>   SV_Live(viWfo , false);
                break ;
        case 2: FrmDeviceSet    -> Show() ;
                //<sun>   SV_SetPaintLocking(true);
                //<sun>   SV_SetPanelProcLocking(true);
                //<sun>   SV_SetVisnTimer  (false);  //타이머 끄고.
                //<sun>   SV_Live(viOri , false);
                //<sun>   SV_Live(viWfr , false);
                //<sun>   SV_Live(viRat , false);
                //<sun>   SV_Live(viRbf , false);
                //<sun>   SV_Live(viWfo , false);
                break ;

        case 3: FM_SetWin(pnBase->Handle) ; break ;
    }
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ClosePage(int index)
{
    //FrmMotorPosition ->Close();
    //FrmVisionSetUp -> Close();

    switch(index)
    {
        case 0: FrmOperation -> Close(); break ;
        case 1: FrmDevice    -> Close(); break ;
        case 2: FrmDeviceSet -> Close(); break ;
        case 3: FM_HideWin()           ; break ;
    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) {
        FM_SetWin(pnBase->Handle);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
//    Trace("Main Form","Update Start");
    tmUpdate -> Enabled = false ;
    SEQ.m_dLastTime = Now();


    //static bool bPreRun = SEQ._iStep != CSequence::scIdle ;
    //
    //if(bPreRun && SEQ._iStep == CSequence::scIdle) {
    //    SV_SetPanelProcLocking(false);
    //}              //m_iStep = scIdle ;
    //if(!bPreRun && SEQ._iStep != CSequence::scIdle) {
    //    SV_SetPanelProcLocking(true);
    //}
    //bPreRun = SEQ._iStep != CSequence::scIdle ;

    //Set Sequence State.
    //Equip Name
    if(OM.EqpOptn.sModelName == "" ) lbName -> Caption = "NoName" ;
    else                             lbName -> Caption = OM.EqpOptn.sModelName ;

    //Option Filck

    //Button Enable Level 오퍼레이터 폼 만들고 필요 없어짐.
    //btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    //btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;

    //Job File
    lbDevice -> Caption = OM.m_sCrntDev ;

    //Now Time
    lbTime -> Caption = Now().FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");

    int iLevel;
    iLevel = FM_GetLevel();

    if(iLevel == lvOperator){
        btOperationShow -> Enabled = true  ;
        btDeviceShow    -> Enabled = true  ;
        btDeviceSetShow -> Enabled = false ;
        btExit          -> Enabled = true  ;
    }
    if(iLevel == lvMaster){
        btOperationShow -> Enabled = true  ;
        btDeviceShow    -> Enabled = true  ;
        btDeviceSetShow -> Enabled = true  ;
        btExit          -> Enabled = true  ;
    }



    //레벨 바꿔주기.
    static int iPreLevel = FM_GetLevel();
    if(iPreLevel != FM_GetLevel()){
        iPreLevel = FM_GetLevel();

        //FrmVisnMain -> SetLevel(iPreLevel);

        //<sun>   SV_SetLevel(iPreLevel);
    }

    //비전 창 바꿔주기.
    static bool bPreErr = EM_IsErr() ;
    if(!bPreErr && EM_IsErr()){


    }
    bPreErr = EM_IsErr();

    bool bPreRun = SEQ._bRun ;

    bPreRun = SEQ._bRun ;

    tmUpdate -> Enabled = true  ;


//    Trace("Main Form","Update End");
}
//---------------------------------------------------------------------------







void __fastcall TFrmMain::lbDeviceDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) FrmMaster -> Show();
}
//---------------------------------------------------------------------------

