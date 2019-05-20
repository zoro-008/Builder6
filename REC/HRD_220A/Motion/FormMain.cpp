//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormVision.h"
#include "FormMaster.h"
#include "FormSPC.h"
#include "FormManual.h"
#include "FormInform.h"
#include "FormLoading.h"
#include "FormSubErr.h"

#include "MainThread.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "LotUnit.h"

#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
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
    IO_SetY(yETC_MainAir , true);
    IO_SetY(yETC_Heater, true);

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

    FrmVision -> Show();

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


    if(FM_MsgYesNo("Confirm","Are you Sure?") != mrYes) return ;

    if(IO_GetX(xPCK_Vccm)){
        FM_MsgOkModal("Warning" , "PC전원 Off시에 픽커에 자제가 떨어집니다.");
    }

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
    if(OM.CmnOptn.bIgnrDoor   ) sSkipMsg += "IGNR DOOR,"  ;
    if(OM.CmnOptn.bLoadingStop) sSkipMsg += "LOAD STOP,"  ;
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
    if(MM.GetManNo() == mcVisnCal){
        bVisnCalStarted = true ;
    }
    if(bVisnCalStarted && MM.GetManNo() == mcNone) {
        bVisnCalStarted = false ;
        double dGapX , dGapY , dGapT ;
        if(MM.GetVisnGap(&dGapX , &dGapY , &dGapT)) {
            String sTemp = String(" X:")+dGapX + " Y:"+dGapY + " T:" + dGapT ;
            FM_MsgOk("Vision Gap" , sTemp.c_str());
        }
    }


    if(!FrmSubErr -> Showing){
        if(PCK.m_bVacErrDisp){
            FrmSubErr -> pcSubErr -> ActivePageIndex = 0 ;
            FrmSubErr -> Tag = etPCKVac ;
            FrmSubErr -> Show();
            PCK.m_bVacErrDisp = false ;
        }
        if(STG.m_bVacErrDisp){
            FrmSubErr -> pcSubErr -> ActivePageIndex = 0 ;
            FrmSubErr -> Tag = etSTGVac ;
            FrmSubErr -> Show();
            STG.m_bVacErrDisp = false ;
        }
        if(PCK.m_bVsnErrDisp){
            FrmSubErr -> pcSubErr -> ActivePageIndex = 1 ;
            FrmSubErr -> Tag = etPCKVsn ;
            FrmSubErr -> Show();
            PCK.m_bVsnErrDisp = false ;
        }
        if(DSP.m_bVsnBfErrDisp){
            FrmSubErr -> pcSubErr -> ActivePageIndex = 1 ;
            FrmSubErr -> Tag = etDSPVsnBf ;
            FrmSubErr -> Show();
            DSP.m_bVsnBfErrDisp = false ;
        }
        if(DSP.m_bVsnAtErrDisp){
            FrmSubErr -> pcSubErr -> ActivePageIndex = 1 ;
            FrmSubErr -> Tag = etDSPVsnAt ;
            FrmSubErr -> Show();
            DSP.m_bVsnAtErrDisp = false ;
        }
    }

    if(FrmSubErr -> Showing && !EM_IsErr()) FrmSubErr -> Close();

    lbCrntTime -> Caption = Now().CurrentTime();

    lbCrntDevice -> Caption = OM.GetCrntDev() ;


    Trace("<@SETBUFF>","0,FormMainTimer End");
    tmUpdate -> Enabled = true  ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::lbVerDblClick(TObject *Sender)
{
    FrmInform->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmTempTimer(TObject *Sender)
{
    tmTemp -> Enabled = false ;

    Trace("<@SETBUFF>","2,FormMainTempTimer Stt");
    int iTemp[MAX_TEMP_CH];
    String sTemp;

    Rs232_TempCh.RqstCrntTemp();

    static bool bFirst = true ;
    if(bFirst) {bFirst = false ; tmTemp -> Enabled = true ; return ;} //첫턴에 온도 에러 뜬다.. 아직 못받아서..

    //if(!OM.CmnOptn.bTempIgnrErrorChk) {
    //    if(Rs232_TempCh.GetCrntTemp(tcRailBtm) > OM.DevOptn.iRailBtmTemp + 10) EM_SetErrMsg(eiPRT_TempErr , "Rail Bottom온도가 설정 온도 보다 높습니다.");
    //    if(Rs232_TempCh.GetCrntTemp(tcRailTop) > OM.DevOptn.iRailTopTemp + 10) EM_SetErrMsg(eiPRT_TempErr , "Rail Top온도가 설정 온도 보다 높습니다.");
    //    if(Rs232_TempCh.GetCrntTemp(tcStage  ) > OM.DevOptn.iStageTemp   + 10) EM_SetErrMsg(eiPRT_TempErr , "Stage 온도가 설정 온도 보다 높습니다.");
    //
    //    if(Rs232_TempCh.GetCrntTemp(tcRailBtm) < OM.DevOptn.iRailBtmTemp - 10) EM_SetErrMsg(eiPRT_TempErr , "Rail Bottom온도가 설정 온도 보다 낮습니다.");
    //    if(Rs232_TempCh.GetCrntTemp(tcRailTop) < OM.DevOptn.iRailTopTemp - 10) EM_SetErrMsg(eiPRT_TempErr , "Rail Top온도가 설정 온도 보다 낮습니다.");
    //    if(Rs232_TempCh.GetCrntTemp(tcStage  ) < OM.DevOptn.iStageTemp   - 10) EM_SetErrMsg(eiPRT_TempErr , "Stage 온도가 설정 온도 보다 낮습니다.");
    //
    //}

    if(!OM.CmnOptn.bTempIgnrErrorChk && SEQ._bRun) {
        for(int i = 0; i < MAX_TEMP_CH; i++){
            iTemp[i] = Rs232_TempCh.GetCrntTemp(i + 1);
            if(i == 0 && iTemp[0] > OM.DevOptn.iRailBtmTemp + OM.CmnOptn.dTempErrorTol) EM_SetErrMsg(eiPRT_TempErr , (String("Rail Bottom온도가 설정 온도 보다 ")+ OM.CmnOptn.dTempErrorTol + "도 이상 높습니다.").c_str());
            if(i == 1 && iTemp[1] > OM.DevOptn.iRailTopTemp + OM.CmnOptn.dTempErrorTol) EM_SetErrMsg(eiPRT_TempErr , (String("Rail Top온도가 설정 온도 보다 "   )+ OM.CmnOptn.dTempErrorTol + "도 이상 높습니다.").c_str());
            if(i == 2 && iTemp[2] > OM.DevOptn.iStageTemp   + OM.CmnOptn.dTempErrorTol) EM_SetErrMsg(eiPRT_TempErr , (String("Stage 온도가 설정 온도 보다 "     )+ OM.CmnOptn.dTempErrorTol + "도 이상 높습니다.").c_str());
    
            if(i == 0 && iTemp[0] < OM.DevOptn.iRailBtmTemp - OM.CmnOptn.dTempErrorTol) EM_SetErrMsg(eiPRT_TempErr , (String("Rail Bottom온도가 설정 온도 보다 ")+ OM.CmnOptn.dTempErrorTol + "도 이상 낮습니다.").c_str());
            if(i == 1 && iTemp[1] < OM.DevOptn.iRailTopTemp - OM.CmnOptn.dTempErrorTol) EM_SetErrMsg(eiPRT_TempErr , (String("Rail Top온도가 설정 온도 보다 "   )+ OM.CmnOptn.dTempErrorTol + "도 이상 낮습니다.").c_str());
            if(i == 2 && iTemp[2] < OM.DevOptn.iStageTemp   - OM.CmnOptn.dTempErrorTol) EM_SetErrMsg(eiPRT_TempErr , (String("Stage 온도가 설정 온도 보다 "     )+ OM.CmnOptn.dTempErrorTol + "도 이상 낮습니다.").c_str());
        }
    }

    if(IO_GetY(yETC_Heater)) {
        if(Rs232_TempCh.GetCrntTemp(1) > OM.DevOptn.iRailBtmTemp + 50) {IO_SetY(yETC_Heater,false); FM_MsgOk("Warning","Rail Bottom온도가 설정 온도 보다 50이상 높아서 강제 Off합니다.");Trace("Warning","Rail Bottom온도가 설정 온도 보다 50이상 높아서 강제 Off합니다.");}
        if(Rs232_TempCh.GetCrntTemp(2) > OM.DevOptn.iRailTopTemp + 50) {IO_SetY(yETC_Heater,false); FM_MsgOk("Warning","Rail Top온도가 설정 온도 보다 50이상 높아서 강제 Off합니다."   );Trace("Warning","Rail Top온도가 설정 온도 보다 50이상 높아서 강제 Off합니다."   );}
        if(Rs232_TempCh.GetCrntTemp(3) > OM.DevOptn.iStageTemp   + 50) {IO_SetY(yETC_Heater,false); FM_MsgOk("Warning","Stage 온도가 설정 온도 보다 50이상 높아서 강제 Off합니다."     );Trace("Warning","Stage 온도가 설정 온도 보다 50이상 높아서 강제 Off합니다."     );}
    }
    else { //켜는것은 껏다키거나 IO로 누르자.
        if((Rs232_TempCh.GetCrntTemp(1) < OM.DevOptn.iRailBtmTemp )&&
           (Rs232_TempCh.GetCrntTemp(2) < OM.DevOptn.iRailTopTemp )&&
           (Rs232_TempCh.GetCrntTemp(3) < OM.DevOptn.iStageTemp   )){IO_SetY(yETC_Heater,true); /*FM_MsgOk("Warning","온도가 안정권으로 복귀되어 On합니다."     );*/Trace("Warning","온도가 안정권으로 복귀되어 On합니다."     );}
    }

    STG.Spc.dCrntStgTemp = Rs232_TempCh.GetCrntTemp(3);

    Trace("<@SETBUFF>","2,FormMainTempTimer End");
    tmTemp -> Enabled = true ;
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

