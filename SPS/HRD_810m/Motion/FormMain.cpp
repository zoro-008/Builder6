//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Form Include
//---------------------------------------------------------------------------
#include "FormMain.h"
#include "FormSPC.h"
#include "FormInfo.h"
#include "FormMaster.h"
#include "FormOperation.h"
#include "FormDevice.h"
#include "FormDeviceSet.h"
#include "FormOption.h"
#include "FormVisnMain.h"
#include "FormVisionSetUp.h"
#include "VisionMan.h"
#include "FormEndImg.h"

#include "UserFile.h"

//#include "Unit1.h"
//---------------------------------------------------------------------------
//Log Include
//---------------------------------------------------------------------------
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Motion Dll Include
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Head.h"
#include "MainThread.h"
#include "Sequence.h"
#include "Stage.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "Rs232Man.h"

#include "MapUnit.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

//---------------------------------------------------------------------------
void SendCmdEndVisn(EN_MSG_CMD _iCmd)
{
    Trace("TFrmEndImg","before FrmEndImg == NULL");
    if (FrmEndImg == NULL   ) return ;
    //if (FrmEndImg ->Visible ) return ;

    //Show Form.
    Trace("TFrmEndImg","Before PostMessage");
    PostMessage(FrmMain -> Handle , WM_USER , _iCmd , 0);
    Trace("TFrmEndImg","After PostMessage");



}

void GrabEndVisn()
{
    FrmEndImg -> Grab(); //어차피 카메라 이미지 그랩 콜백 호출시 화면 갱신 메세지로 하기 때문에 그냥 써도 됨.
}

bool GetGrabEndEndVisn()
{
    return FrmEndImg -> GetGrabEnd();
}

bool SaveImageEndVisn (String _sFilePath)
{
    return FrmEndImg -> SaveImg(_sFilePath);

}
//=====================이위로는 작업 종료 카메라 .......




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

    //Set Ajin Home

    //MT_SetSubHome(miWRD_THed,true);
    //AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

    IO_SetY(yETC_EjctrMain , true);
//    tmMainSet->Enabled = true ;

    HED.m_bFnshCycTAlgn = false; //티얼라인 사이클 안탔으므로 펄스..JH

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

    Trace("Application Start",("Ver (" +OnGetVer("FileVersion") + ")").c_str());

    //Last Device Load
    OM.LoadLastInfo();


//TODO::    MT_SetServoAll(true);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::WmUser(TMessage &Message)
{
    //Grab();  테스트.


    if(Message.WParam == mcCapAndShowSec) {
        String sWorkEndImgFolder =  "D:\\WorkEndImg\\" ;

        if(!UserFile.DirExists(sWorkEndImgFolder)){
            if(!UserFile.CreateDir(sWorkEndImgFolder)) {
                ShowMessage("WorkEndImg Folder Create Fail") ;
                Trace("eiSTG_WorkEndCam","WorkEndImg Folder Create Fail");
                return ;
            }
        }

        //페이지를 바꾼다 캡쳐할 페이지로.
        FrmOperation -> pcControl -> ActivePageIndex = 0 ;

        //.이 파일명에 들어가 있는 경우가 있어서 _로 치환 한다. 세이브가 안됌.
        //String sFileName = STG.m_sWorkingWfrId ;
        //sFileName = StringReplace(sFileName , "\r","",TReplaceFlags()<<rfReplaceAll);

        Trace("Save sFileName" , STG.m_sWorkingWfrId.c_str());


        //오퍼레이션창의 스크린샷.
        FrmOperation -> GetPanelScreenShot(FrmOperation -> pnCapture , sWorkEndImgFolder + STG.m_sWorkingWfrId + "Sc.bmp");

        //이미지...저장.
        SaveImageEndVisn(sWorkEndImgFolder + STG.m_sWorkingWfrId + "Im.bmp");

        FrmEndImg -> Height = 634 ;
        FrmEndImg -> Width  = 808 ;
        FrmEndImg -> Show();
        FrmEndImg -> tmClose -> Enabled = true ; //자동 닫기..

        UserFile.ClearDirDate(sWorkEndImgFolder , Now() - 30); //30일 지난것 삭제.
        Trace("Msg End" , (STG.m_sWorkingWfrId + "Im.bmp").c_str());

    }

}

void __fastcall TFrmMain::btOperationShowClick(TObject *Sender)
{
    TButton *pButton = dynamic_cast<TButton *>(Sender);
    ClosePage(m_iPageIndex);
    m_iPageIndex = pButton->Tag;
    ShowPage (m_iPageIndex);
    AnsiString sTemp;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    static bool bFirst = true ;
    if(bFirst){
        VSM.InitVisn(OM.m_sCrntDev) ;
        FrmEndImg = new TFrmEndImg(this);
    }
    bFirst = false ;



    FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; /*FrmOperation -> Width = 1280 ; FrmOperation -> Height = 870 ;*/ FrmOperation -> Parent = pnBase ;
    FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; /*FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 870 ;*/ FrmDevice    -> Parent = pnBase ;
    FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; /*FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 870 ;*/ FrmDeviceSet -> Parent = pnBase ;
    FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; /*FrmOption    -> Width = 1280 ; FrmOption    -> Height = 870 ;*/ FrmOption    -> Parent = pnBase ;
    FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; /*FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 870 ;*/ FrmSPC       -> Parent = pnBase ;
    FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; //FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;



    ShowPage (0);








    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ;
                break ;
        case 1: FrmDevice    -> Show() ;
                Trace("Operator", "Main Form_Device Set Form Show Button Click") ;
                break ;
        case 2: FrmOption    -> Show() ;
                Trace("Operator", "Main Form_Option Form Show Button Click") ;
                break ;
        case 3: FrmSPC       -> Show() ;
                Trace("Operator", "Main Form_SPC Form Show Button Click") ;
                break ;
        case 4: FM_SetWin(pnBase) ;
                Trace("Operator", "Main Form_Util Form Show Button Click") ;
                break ;
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
void __fastcall TFrmMain::btExitClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Error" , "장비가 동작 중입니다.!!");
        return;
    }





    if(FM_MsgYesNo("Confirm","종료 하시겠습니까?") != mrYes) return ;

    delete FrmEndImg ;
    FrmEndImg = NULL ;

    //MAPU.SetMemo(NULL);
    FrmOperation -> mmMemo -> Clear() ;



    EndThread();



    DM.SaveMap();

    FM_DelWin();
    //Application -> ProcessMessages();
    //Sleep(100); //DM.ARAY 뻑때문에 한번 해봄.
    //Application -> ProcessMessages();

    VSM.DelVisn();




    FrmSPC       ->tmUpdate->Enabled = false ;
    FrmOperation ->tmUpdate->Enabled = false ;
    FrmDevice    ->tmUpdate->Enabled = false ;
    FrmDeviceSet ->tmUpdateTimer->Enabled = false ;
    //FrmOption    ->tmUpdate->Enabled = false ;
    FrmMaster    ->tmUpdate->Enabled = false ;






    

    Trace("Operator", "Main Form_Exit Button Click") ;
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
//

    //FM_HideWin();

    //EndThread();

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    //
//
//
//
//
//
//
//
//


//TODO ::    MT_SetServoAll(false); //세타축 자꾸 돌아감.

    //sun 기존에 쓰레드에 있던것 다 주석 처리 하고 화면 타이머에서 하게 수정. FormOperation,FormDeviceSet
    //하고 엑시트 버튼 누를때 세이브 하게 수정.



    Trace("<EXIT>","EXIT");

    //FreeSVL();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbNameDblClick(TObject *Sender)
{
    int iLevel;
    iLevel = FM_GetLevel();
    if (iLevel == lvMaster) FrmMaster -> Show();

    Trace("Operator", "Main Form_Master Button Click") ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInfo->Show();

    Trace("Operator", "Main Form_Information Button Click") ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
//    Trace("Main Form","Update Start");
    tmUpdate -> Enabled = false ;
    SEQ.m_dLastTime = Now();


    //Set Sequence State.
    if(!OM.MstOptn.bDebugMode){
        switch(SEQ._iSeqStat) {
            default        :                                                                                                 break ;
            case ssInit    : lbStat -> Caption = "INIT"          ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "ERROR"         ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunning : lbStat -> Caption = "RUNNING"       ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssWarning : lbStat -> Caption = "WARNING"       ; lbStat -> Font -> Color = clYellow ;                      break ;
            case ssStop    : lbStat -> Caption = "STOP"          ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "LOTEND"        ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    else {
        switch(SEQ._iSeqStat) {
            default        :                                                                                             break ;
            case ssInit    : lbStat -> Caption = "D INIT"    ; lbStat -> Font -> Color = clBlue   ;                      break ;
            case ssError   : lbStat -> Caption = "D ERROR"   ; lbStat -> Font -> Color = SEQ._bFlick ? clYellow : clRed ;break ;
            case ssRunning : lbStat -> Caption = "D RUNNING" ; lbStat -> Font -> Color = clLime   ;                      break ;
            case ssWarning : lbStat -> Caption = "D WARNING" ; lbStat -> Font -> Color = clYellow ;                      break ;
            case ssStop    : lbStat -> Caption = "D STOP"    ; lbStat -> Font -> Color = clBlack  ;                      break ;
            case ssWorkEnd : lbStat -> Caption = "D LOTEND"  ; lbStat -> Font -> Color = clGray   ;                      break ;
        }
    }
    //Equip Name
    if(OM.EqpOptn.sModelName == "" ) lbName -> Caption = "NoName" ;
    else                             lbName -> Caption = OM.EqpOptn.sModelName ;

    //Job File
    lbDevice -> Caption = OM.m_sCrntDev ;

    //Now Time
    lbTime -> Caption = Now().FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");

    int iLevel;
    iLevel = FM_GetLevel();

    //Option Filck
    pnLoadingStop -> Visible = OM.CmnOptn.bLoadingStop   ? SEQ._bFlick : false ;

    if(iLevel != lvOperator) { pnIgnreDoor   -> Visible = OM.CmnOptn.bDoorSkip   ? SEQ._bFlick : false ; FrmOperation -> Panel25 -> Visible = true ;
                               pnUseBarcode  -> Visible = OM.CmnOptn.bUseBarcode ? SEQ._bFlick : false ; FrmOperation -> Panel32 -> Visible = true ;}
    else                     { pnIgnreDoor   -> Visible =!OM.CmnOptn.bDoorSkip ;                       FrmOperation -> Panel25 -> Visible = false;
                               FrmOption     -> cbDoorSkip -> Checked = false  ;
                               pnIgnreDoor   -> Visible =!OM.CmnOptn.bUseBarcode ;                     FrmOperation -> Panel32 -> Visible = false;
                               FrmOption     -> cbUseBarcode -> Checked = true  ;}


    if(iLevel == lvOperator){
        btOperationShow -> Enabled = true  ;
        btDeviceShow    -> Enabled = true  ;
        btOptionShow    -> Enabled = false ;
        btSpcShow       -> Enabled = true  ;
        btUtilShow      -> Enabled = false ;
        btExit          -> Enabled = false ;
    }

    if(iLevel == lvEngineer){
        btOperationShow -> Enabled = true  ;
        btDeviceShow    -> Enabled = true  ;
        btOptionShow    -> Enabled = true  ;
        btSpcShow       -> Enabled = true  ;
        btUtilShow      -> Enabled = false ;
        btExit          -> Enabled = true  ;
    }

    if(iLevel == lvMaster){
        btOperationShow -> Enabled = true ;
        btDeviceShow    -> Enabled = true ;
        btOptionShow    -> Enabled = true ;
        btSpcShow       -> Enabled = true ;
        btUtilShow      -> Enabled = true ;
        if(!SEQ._bRun){
            btExit          -> Enabled = true;
        }
    }

    //if(iLevel != lvMaster){
    //    btOperationShow -> Enabled = false ;
    //    btDeviceShow    -> Enabled = false ;
    //    btOptionShow    -> Enabled = false ;
    //    btSpcShow       -> Enabled = false ;
    //    btUtilShow      -> Enabled = false ;
    //    btExit          -> Enabled = false ;
    //}

    //레벨 바꿔주기.
    static int iPreLevel = FM_GetLevel();
    if(iPreLevel != FM_GetLevel()){
        iPreLevel = FM_GetLevel();

        VSM.SetLevel(iPreLevel);
    }

    //비전 창 바꿔주기.
    static bool bPreErr = EM_IsErr() ;
    if(!bPreErr && EM_IsErr()){
    }
    bPreErr = EM_IsErr();

    bool bPreRun = SEQ._bRun ;


    bPreRun = SEQ._bRun ;

    lbWaferName -> Caption = STG.sMainWfName ;




    static bool bPreCastDetect = IO_GetX(xSTG_CastDetect) ;
    bool bCastDetect = IO_GetX(xSTG_CastDetect) ;
    if(!SEQ._bRun && bCastDetect && !bPreCastDetect){
        if(FM_MsgYesNo("Confirm","카세트를 새로 마스킹 하시겠습니까?") == mrYes) DM.ARAY[riCST].SetStat(csUnkwn);
    }
    bPreCastDetect = bCastDetect ;



    //sun 기존에 쓰레드에 있던것 다 주석 처리 하고 화면 타이머에서 하게 수정. FormOperation,FormDeviceSet
    //static bool bPreRun2 = false ;
    //if(bPreRun2 && !SEQ._bRun) {
    //    DM.SaveMap();
    //}
    //bPreRun2 = SEQ._bRun ;



    lbCalPitch -> Caption = String("Pitch X:")+HED.m_dRevisePitchX + " Y:" +HED.m_dRevisePitchY ;



    tmUpdate -> Enabled = true  ;





    //int iOperator;
    //iOperator = FM_GetLevel();
    //if(iOperator == lvOperator){
    //    !OM.CmnOptn.bDoorSkip;
    //    FrmOperation -> Panel25 -> Visible = false;
    //}
    //else {
    //    OM.CmnOptn.bDoorSkip;
    //    FrmOperation -> Panel25 -> Visible = true ;
    //}

//    Trace("Main Form","Update End");
}
//---------------------------------------------------------------------------







void __fastcall TFrmMain::Button1Click(TObject *Sender)
{
    FrmEndImg -> Height = 634 ;
    FrmEndImg -> Width  = 922 ;

    Trace("TFrmEndImg","FrmEndImg -> Show()");

    FrmEndImg -> Show();
}
//---------------------------------------------------------------------------



