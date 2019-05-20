//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormMaster.h"
#include "FormSPC.h"
#include "FormManual.h"
#include "FormInform.h"
#include "FormLoading.h"
#include "FormSubErr.h"
#include "FormCadPos.h"
#include "FormCadPosRtt.h"

#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "LotUnit.h"

#include "Stage.h"
#include "LeftTool.h"
#include "RightTool.h"

#include<stdio.h>
#include<string.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

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

    lbName -> Caption = OM.EqpOptn.sModelName;
    //Last Device Load
    OM.LoadLastInfo();
    //IO_SetY(yLDR_ZBreak,false );
    //IO_SetY(yULD_ZBreak,false );
    ::Sleep(100);
    
    MT_SetServoAll(true);
    IO_SetY(yETC_MainAirSol , true);

    pnSkipDisp -> Left = 0 ;
    pnSkipDisp -> Top  = 0 ;

    sbOperationShow -> Down = true ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1280 ; FrmOperation -> Height = 863 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1280 ; FrmDevice    -> Height = 863 ;
    FrmDeviceSet -> Parent = pnBase ; FrmDeviceSet -> Left = 0 ; FrmDeviceSet -> Top = 0 ; FrmDeviceSet -> Width = 1280 ; FrmDeviceSet -> Height = 863 ;
    FrmSPC       -> Parent = pnBase ; FrmSPC       -> Left = 0 ; FrmSPC       -> Top = 0 ; FrmSPC       -> Width = 1280 ; FrmSPC       -> Height = 863 ;
    FrmOption    -> Parent = pnBase ; FrmOption    -> Left = 0 ; FrmOption    -> Top = 0 ; FrmOption    -> Width = 1280 ; FrmOption    -> Height = 863 ;
    FrmManual    -> Parent = pnBase ; FrmManual    -> Left = 0 ; FrmManual    -> Top = 0 ; FrmManual    -> Width = 1280 ; FrmManual    -> Height = 863 ;
    //FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1280 ; FrmMaster    -> Height = 863 ;

    ShowPage (0);

    //FrmVision -> Show();

    static bool bFirst = true ;
    if(bFirst) {
//        StartThread();
        bFirst = false ;
    }

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show()     ; break ;
        case 1: FrmDevice    -> Show()     ; break ;
        case 2: FrmSPC       -> Show()     ; break ;
        case 3: FrmManual    -> Show()     ; break ;
        case 4: FrmOption    -> Show()     ; break ;
        case 5: FM_SetWin(pnBase-> Handle) ; break ;
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
        case 2: FrmSPC       -> Hide(); break ;
        case 3: FrmManual    -> Hide(); break ;
        case 4: FrmOption    -> Hide(); break ; 
        case 5: FM_HideWin()          ; break ;
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
    Trace( "<EXIT>", "btExitClick1") ;

    FrmCadPos    -> tmUpdate -> Enabled = false ;
    FrmCadPosRtt -> tmUpdate -> Enabled = false ;

    FrmCadPos    -> Close();
    FrmCadPosRtt -> Close();


    if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    Trace( "<EXIT>", "btExitClick2") ;
    tmUpdate -> Enabled = false ;
    EndThread();

    FrmLoading = new TFrmLoading(Application , false);
    ModalResult = FrmLoading->ShowModal();
    delete FrmLoading;
    Trace( "<EXIT>", "btExitClick3") ;

    FM_DelWin();

    Trace( "<EXIT>", "btExitClick4") ;

    FrmMain->Close();
    Trace( "<EXIT>", "btExitClick5") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    FrmOperation ->Close();
    FrmDevice    ->Close();
    FrmDeviceSet ->Close();
    FrmOption    ->Close();
    FrmMaster    ->Close();
    FrmManual    ->Close();
    Trace( "<EXIT>", "FormCloseEnd") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
   int i;
   i++    ;

   Trace( "<@EXIT>", "FormDestroy") ;
}
//---------------------------------------------------------------------------
__fastcall TFrmMain::~TFrmMain()
{
}


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
    Trace("<@SETBUFF>","0,FormMainTimer Stt");

    btExit -> Enabled = !SEQ._bRun ;
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
    Trace("<@SETBUFF>","0,FormMainTimer 1");

    // 상태 Panel 들 깜박 깜박.
    if(OM.CmnOptn.bIgnrDoor   )  sSkipMsg += "IGNR DOOR,"  ;
    if(OM.CmnOptn.bFLoadingStop) sSkipMsg += "FRONT LOAD STOP,"  ;
    if(OM.CmnOptn.bRLoadingStop) sSkipMsg += "REAR LOAD STOP,"   ;

    //if(OM.MstOptn.bVacErrIgnr ) sSkipMsg += "IGNR VACM,"   ;
    //if(OM.CmnOptn.bSkipOST    ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "OST,"   ;}
    //if(OM.CmnOptn.bSkipVT     ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VT,"    ;}
    //if(OM.CmnOptn.bSkipVisn1  ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VISN1," ;}
    //if(OM.CmnOptn.bSkipVisn2  ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "VISN2," ;}
    //if(OM.CmnOptn.bSkip1stTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK1,"  ;}
    //if(OM.CmnOptn.bSkip2ndTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK2,"  ;}
    //if(OM.CmnOptn.bSkip3thTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK3,"  ;}
    //if(OM.CmnOptn.bSkip4thTol ) {if(!sSkipMsg.Pos("SKIP"))sSkipMsg += "SKIP "; sSkipMsg += "PCK4,"  ;}


    pnSkipDisp -> Caption = sSkipMsg ;


    pnSkipDisp -> Visible = sSkipMsg != "" ? SEQ._bFlick : false ;

    btOptionShow -> Enabled = FM_GetLevel() == lvMaster ;
    btUtilShow   -> Enabled = FM_GetLevel() == lvMaster ;
    btManualShow -> Enabled = FM_GetLevel() != lvOperator ;

    Trace("<@SETBUFF>","0,FormMainTimer 2");
    static bool bVisnCalStarted = false ;
//    if(MM.GetManNo() == mcVisnCal){
//        bVisnCalStarted = true ;
//    }
    if(bVisnCalStarted && MM.GetManNo() == mcNone) {
        bVisnCalStarted = false ;
        double dGapX , dGapY , dGapT ;
        if(MM.GetVisnGap(&dGapX , &dGapY , &dGapT)) {
            String sTemp = String(" X:")+dGapX + " Y:"+dGapY + " T:" + dGapT ;
            FM_MsgOk("Vision Gap" , sTemp.c_str());
        }
    }

    static bool bPreErr = EM_IsErr() ;
           bool bErr    = EM_IsErr() ;

    if(!bPreErr && bErr) {
        if(EM_GetLastErr()==eiHGT_RangeOver ){
            if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)+DM.ARAY[riSTG].GetCntStat(csWorkEnd)==DM.ARAY[riSTG].GetMaxCol()*DM.ARAY[riSTG].GetMaxRow()){
                FrmSubErr -> Show();
            }
        }
        if(EM_GetLastErr()==eiVSN_InspRangeOver ){
            if(DM.ARAY[riSTG].GetCntStat(csCmsREndAlign)){ //마지막 경화 완료후에만 스킵을 하고... 나머지 검사들은 스킵해봤다 병신짓 함.
                FrmSubErr -> Show();
            }
        }
    }
    bPreErr = bErr;

    lbDevice -> Caption = OM.GetCrntDev() ;


    if(FrmSubErr -> Showing && !EM_IsErr()) FrmSubErr -> Close();

    lbCrntTime -> Caption = Now().CurrentTime();


    //20180919 오성철과장 요청 비전검사하면 옵션처리로 물어보고 진행.
    if(LTL.GetStat().bShowVisnMsg){
        //플레그 초기화.
        CLeftTool::SStat Stat ;
        Stat = LTL.GetStat();
        Stat.bShowVisnMsg = false ;

        LTL.SetStat(Stat);
        FM_MsgOk("Confirm","좌측비전을 확인하고 Start 하세요.");
    }
    if(RTL.GetStat().bShowVisnMsg){
        //플레그 초기화.
        CRightTool::SStat Stat ;
        Stat = RTL.GetStat();
        Stat.bShowVisnMsg = false ;

        RTL.SetStat(Stat);
        FM_MsgOk("Confirm","우측비전을 확인하고 Start 하세요.");
    }





    Trace("<@SETBUFF>","0,FormMainTimer End");
    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    // FrmInform->Show();
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::btManualShowClick(TObject *Sender)
{
    TSpeedButton *pButton = dynamic_cast<TSpeedButton *>(Sender);
    ClosePage(m_iPageIndex);
    m_iPageIndex = pButton->Tag;
    ShowPage (m_iPageIndex);
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    StartThread();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::lbStatClick(TObject *Sender)
{
//    EM_SetErr(eiHGT_RangeOver);        
}
//---------------------------------------------------------------------------

