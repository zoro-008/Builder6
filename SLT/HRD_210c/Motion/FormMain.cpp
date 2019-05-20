//---------------------------------------------------------------------------
#include <vcl.h>
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
#include "FormLoading.h"
//---------------------------------------------------------------------------
//Log Include
//---------------------------------------------------------------------------
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Dll Include
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SAInterface.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "MainThread.h"
#include "Sequence.h"
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "PostBuff.h"
#include "Stage.h"
#include "Unloader.h"
#include "OptionMan.h"
#include "PstnMan.h"
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
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    FM_MakeWin();
    SV_InitVisn(OM.m_sCrntDev.c_str());





    MT_SetSubHome(miWRD_THed,true);
    AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

    //

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
        FM_MsgOk("Error" , "During Auto Running!!");
        return;
    }

    if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    RSM.Close();
    DM .Close();
    LT .Close();

    LDR.Close();
    PRB.Close();
    RAE.Close();
    EPX.Close();
    RAD.Close();
    HED.Close();
    STG.Close();
    PSB.Close();
    ULD.Close();



    AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

    EndThread();
    tmUpdate -> Enabled = false ;
    FrmSPC ->tmUpdate->Enabled = false ;
    //FraCyl ->tmUpdate->Enabled = false ;
    //FraMotr->tmUpdate->Enabled = false ;
    FrmOperation ->tmUpdate   ->Enabled = false ;
    FrmOperation ->tmUpdateAray->Enabled = false ;



    FrmDevice    ->tmUpdate->Enabled = false ;
    FrmDeviceSet ->tmUpdate->Enabled = false ;
    //FrmOption    ->tmUpdate->Enabled = false ;
    FrmMaster    ->tmUpdate->Enabled = false ;

    //RSM.Close() ;



    SV_DelVisn(); //FrmOperation ->Close();전에 해야함.
    FreeSVA();

    FM_HideWin();
    //FM_DelWin();  //EndThread 후에. 아... 모르겠다...
    //FreeSML(); //이상하게 뻑남...



    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();


    //
//    



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

    //
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






        case 2: FrmOption       -> Show() ; break ;
        case 3: FrmSPC          -> Show() ; break ;
        case 4: FM_SetWin(pnBase->Handle) ; break ;
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
        case 1: FrmDevice    -> Close();
                FrmDeviceSet -> Close(); break ;
        case 2: FrmOption    -> Close(); break ;
        case 3: FrmSPC       -> Close(); break ;
        case 4: FM_HideWin()           ; break ;
    }
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
    if(!OM.MstOptn.bDebugMode){
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "INIT"          ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "ERROR"         ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
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
            case ssRunning : lbStat -> Caption = "DEBUG RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssStop    : lbStat -> Caption = "DEBUG STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "DEBUG LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    //Equip Name
    if(OM.EqpOptn.sModelName == "" ) lbName -> Caption = "NoName" ;
    else                             lbName -> Caption = OM.EqpOptn.sModelName ;

    //Option Filck
    pnLoadingStop -> Visible = OM.CmnOptn.bLoadingStop   ? SEQ._bFlick : false ;
    pnIgnrOriInsp -> Visible = OM.CmnOptn.bIgnrOriVs     ? SEQ._bFlick : false ;
    pnIgnreDoor   -> Visible = OM.CmnOptn.bDoorSkip      ? SEQ._bFlick : false ;

    //Button Enable Level 오퍼레이터 폼 만들고 필요 없어짐.
    //btOptionShow -> Enabled = FM_GetLevel() != lvOperator ;
    //btUtilShow   -> Enabled = FM_GetLevel() != lvOperator ;

    //Job File
    lbDevice -> Caption = OM.m_sCrntDev ;

    //Now Time
    lbTime -> Caption = Now().FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");

    btOperationShow -> Enabled = !FrmDeviceSet -> pnVisnSet -> Visible ;
    btDeviceShow    -> Enabled = !FrmDeviceSet -> pnVisnSet -> Visible ;
    btOptionShow    -> Enabled = !FrmDeviceSet -> pnVisnSet -> Visible ;
    btSpcShow       -> Enabled = !FrmDeviceSet -> pnVisnSet -> Visible ;
    btUtilShow      -> Enabled = !FrmDeviceSet -> pnVisnSet -> Visible ;
    btExit          -> Enabled = !FrmDeviceSet -> pnVisnSet -> Visible && !SEQ._bRun;

    static bool bPreDispStartForm = STG.m_bDispStartForm ;

    //int r,c ;
    if(!bPreDispStartForm && STG.m_bDispStartForm) {
        FM_MsgOk("첫자재 확인","첫자제의 위치를 확인 하고 Start를 누르세요");
        SV_SetActiveVisn(viWfr  ) ;
        SEQ._bBtnStop = true ;
        STG.m_bDispStartForm = false ;
        FrmOperation -> pcControl -> ActivePageIndex = 3 ;
    }

    if(HED.m_bDispPickCal) {
        HED.m_bDispPickCal = false ;
        AnsiString sTemp = "체크 포지션을 " + AnsiString(PM.GetValue(miWRD_ZHed , pvWRD_ZHedWaferCal)) + "에서 " + AnsiString(HED.m_dPickCalPos) + "로 바꾸겠습니까?" ;
        if(FM_MsgYesNo("Check",sTemp.c_str()) == mrYes) {
            double dGap = HED.m_dPickCalPos - PM.GetValue(miWRD_ZHed , pvWRD_ZHedWaferCal) ;
            PM.SetValue(miWRD_ZHed , pvWRD_ZHedWaferCal   , HED.m_dPickCalPos);
            PM.SetValue(miWRD_ZHed , pvWRD_ZHedWfWork     , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWfWork    ) + dGap) ;
            PM.SetValue(miWRD_ZHed , pvWRD_ZHedWfWorkWait , PM.GetValue(miWRD_ZHed , pvWRD_ZHedWfWorkWait) + dGap) ;
            PM.Save(OM.m_sCrntDev);
        }
    }

    if(HED.m_bDispPlaceCal) {
        HED.m_bDispPlaceCal = false ;
        AnsiString sTemp = "체크 포지션을 " + AnsiString(PM.GetValue(miWRD_ZHed , pvWRD_ZHedStripCal)) + "에서 " + AnsiString(HED.m_dPlaceCalPos) + "로 바꾸겠습니까?" ;
        if(FM_MsgYesNo("Check",sTemp.c_str()) == mrYes) {
            double dGap = HED.m_dPlaceCalPos - PM.GetValue(miWRD_ZHed , pvWRD_ZHedStripCal) ;
            PM.SetValue(miWRD_ZHed , pvWRD_ZHedStripCal   , HED.m_dPlaceCalPos);
            PM.SetValue(miWRD_ZHed , pvWRD_ZHedStWork     , PM.GetValue(miWRD_ZHed , pvWRD_ZHedStWork    ) + dGap) ;
            PM.SetValue(miWRD_ZHed , pvWRD_ZHedStWorkWait , PM.GetValue(miWRD_ZHed , pvWRD_ZHedStWorkWait) + dGap) ;
            PM.Save(OM.m_sCrntDev);
        }
    }






    bPreDispStartForm = STG.m_bDispStartForm ;

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

    //<sun>            if(EM_GetLastErr() == eiPRB_OriVsFailCnt    ) SV_SetActiveVisn(viOri  ) ;
    //<sun>       else if(EM_GetLastErr() == eiPRB_OriVsGrabFail   ) SV_SetActiveVisn(viOri  ) ;
    //<sun>       else if(EM_GetLastErr() == eiPRB_OriVsInspFail   ) SV_SetActiveVisn(viOri  ) ;
    //<sun>       else if(EM_GetLastErr() == eiHED_RalBfVsGrabFail ) SV_SetActiveVisn(viRbf  ) ;
    //<sun>       else if(EM_GetLastErr() == eiHED_RalBfVsInspFail ) SV_SetActiveVisn(viRbf  ) ;
    //<sun>       else if(EM_GetLastErr() == eiHED_RalBfVsThchFail ) SV_SetActiveVisn(viRbf  ) ;
    //<sun>       else if(EM_GetLastErr() == eiHED_RalAtVsGrabFail ) SV_SetActiveVisn(viRat) ;
    //<sun>       else if(EM_GetLastErr() == eiHED_RalAtVsInspFail ) SV_SetActiveVisn(viRat) ;
    //<sun>       else if(EM_GetLastErr() == eiHED_RalAtVsThchFail ) SV_SetActiveVisn(viRat) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_WfrVsGrabFail   ) SV_SetActiveVisn(viWfr  ) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_WfrVsInspFail   ) SV_SetActiveVisn(viWfr  ) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_WfrVsThchFail   ) SV_SetActiveVisn(viWfr  ) ;
    //<sun>   
    //<sun>       else if(EM_GetLastErr() == eiWRD_RalBfVsFailCnt  ) SV_SetActiveVisn(viRbf  ) ;
    //<sun>       else if(EM_GetLastErr() == eiWRD_RalAtVsFailCnt  ) SV_SetActiveVisn(viRat) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_TLmtErr         ) SV_SetActiveVisn(viWfr  ) ;
    //<sun>       else if(EM_GetLastErr() == eiRAD_VisnRlstOver    ) SV_SetActiveVisn(viRbf  ) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_VisnRlstOver    ) SV_SetActiveVisn(viWfr  ) ;
    //<sun>   
    //<sun>       else if(EM_GetLastErr() == eiSTG_WfrOriVsGrabFail) SV_SetActiveVisn(viWfo) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_WfrOriVsInspFail) SV_SetActiveVisn(viWfo) ;
    //<sun>       else if(EM_GetLastErr() == eiSTG_WfrOriVsThchFail) SV_SetActiveVisn(viWfo) ;
    //<sun>   
    //<sun>   


    }
    bPreErr = EM_IsErr();

    bool bPreRun = SEQ._bRun ;

    if(bPreRun && !SEQ._bRun) {
        DM.SaveMap();
    }
    bPreRun = SEQ._bRun ;

    tmUpdate -> Enabled = true  ;


//    Trace("Main Form","Update End");
}
//---------------------------------------------------------------------------

