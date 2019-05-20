//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
//---------------------------------------------------------------------------
//Form Reference
//---------------------------------------------------------------------------
#include "FormMaster.h"
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "FormOperation.h"
#include "FormValve.h"
#include "FormSpinMotor.h"
#include "FormSetting.h"
#include "FormTiltMotor.h"
#include "FormDevice.h"
#include "SEQUnit.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "UserIni.h"
#include "OptionMan.h"
#include "MainThread.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
//Unit Reference
//---------------------------------------------------------------------------
#include "global.h"
#include "PaixMotion.h"
#include "SEQUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define ONCOLOR (TColor)0x00DDFBFF
#define BACKGRAUNDCOLOR (TColor)0xFF8080
TFrmMain *FrmMain;

//---------------------------------------------------------------------------

__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
    //Motion
    pPaix = new CPaixMotion();
    //pPaix -> SetMsgFunc(Msg) ;

    //Seq.
    SEQ = new CSEQ();
    //pSeq -> SetMsgFunc(Msg) ;

    FrmMain -> Color = BACKGRAUNDCOLOR;
    FrmMain -> ClientHeight = 768 ;
    FrmMain -> ClientWidth  = 1024;

    iBtnNum = 0;

    //pPaix -> GetInputX(g_lInputXIo);

    if(g_lInputXIo[xETC_DoorFw]) {pPaix -> SetOutputXBit(yETC_DoorFw, true ); pPaix -> SetOutputXBit(yETC_DoorBw, false);}
    else                         {pPaix -> SetOutputXBit(yETC_DoorFw, false); pPaix -> SetOutputXBit(yETC_DoorBw, true );}

    OM.LoadLastInfo();
    OM.LoadDevOptn(OM.m_sCrntDev);
    OM.LoadCmnOptn();
    OM.LoadMstOptn();

    g_sModelName = "HRW-150P";
}

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    EndThread();
    
    //모션 관련
    if(pPaix) {
        delete pPaix;
        pPaix = NULL;
    }

    if(SEQ) {
        delete SEQ ;
        SEQ = NULL ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::ReadLogic(void)
{
    if( pPaix == NULL ) return;
}

//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;

    if(!m_bAlrmClear){
        if(g_NmcData.nAlarm[0] == 1 || g_NmcData.nAlarm[1] == 1) {
            SEQ->m_bErr = true;
            m_bAlrmClear = true;
            sTemp = (AnsiString)g_NmcData.nAlarm[0] + "   Motor Alram상태 입니다.";
            FrmMsgOk->MsgOk("Error","Motor Alram상태 입니다.");
            Trace("Error" , sTemp.c_str());
        }
    }

    if(g_NmcData.nAlarm[0] == 0 && g_NmcData.nAlarm[1] == 0) {m_bAlrmClear = false;}

    //if(SEQ -> m_bRun != false) {
    if(SEQ -> m_iStep != CSEQ::scIdle){ //  SEQ -> scIdle){
        pnDeviceShow  -> Enabled = false; pnDeviceShow  -> Font -> Color = clGray;
        pnValveShow   -> Enabled = false; pnValveShow   -> Font -> Color = clGray;
        pnSpinMTShow  -> Enabled = false; pnSpinMTShow  -> Font -> Color = clGray;
        pnTiltMTShow  -> Enabled = false; pnTiltMTShow  -> Font -> Color = clGray;
        pnSettingShow -> Enabled = false; pnSettingShow -> Font -> Color = clGray;
        pnExit        -> Enabled = false; pnExit        -> Font -> Color = clGray;
    }
    else {
        pnDeviceShow  -> Enabled = true;  pnDeviceShow  -> Font -> Color = clBlack;
        pnValveShow   -> Enabled = true;  pnValveShow   -> Font -> Color = clBlack;
        pnSpinMTShow  -> Enabled = true;  pnSpinMTShow  -> Font -> Color = clBlack;
        pnTiltMTShow  -> Enabled = true;  pnTiltMTShow  -> Font -> Color = clBlack;
        pnSettingShow -> Enabled = true;  pnSettingShow -> Font -> Color = clBlack;
        pnExit        -> Enabled = true;  pnExit        -> Font -> Color = clBlack;
    }

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true;

    FrmOperation -> Parent = pnBase ; FrmOperation -> Left = 0 ; FrmOperation -> Top = 0 ; FrmOperation -> Width = 1024 ; FrmOperation -> Height = 700 ;
    FrmDevice    -> Parent = pnBase ; FrmDevice    -> Left = 0 ; FrmDevice    -> Top = 0 ; FrmDevice    -> Width = 1024 ; FrmDevice    -> Height = 700 ; 
    FrmValve     -> Parent = pnBase ; FrmValve     -> Left = 0 ; FrmValve     -> Top = 0 ; FrmValve     -> Width = 1024 ; FrmValve     -> Height = 700 ;
    FrmSpinMT    -> Parent = pnBase ; FrmSpinMT    -> Left = 0 ; FrmSpinMT    -> Top = 0 ; FrmSpinMT    -> Width = 1024 ; FrmSpinMT    -> Height = 700 ;
    FrmSetting   -> Parent = pnBase ; FrmSetting   -> Left = 0 ; FrmSetting   -> Top = 0 ; FrmSetting   -> Width = 1024 ; FrmSetting   -> Height = 700 ;
    FrmTiltMT    -> Parent = pnBase ; FrmTiltMT    -> Left = 0 ; FrmTiltMT    -> Top = 0 ; FrmTiltMT    -> Width = 1024 ; FrmTiltMT    -> Height = 700 ;
    FrmMaster    -> Parent = pnBase ; FrmMaster    -> Left = 0 ; FrmMaster    -> Top = 0 ; FrmMaster    -> Width = 1024 ; FrmMaster    -> Height = 700 ;

    ShowPage (0);

    FrmMain -> Top = 0;
    FrmMain -> Left = 0;

    pPaix -> SetRotorPara();
}
void __fastcall TFrmMain::pnExitClick(TObject *Sender)
{
    if(MsgYesNo("Exit","종료 하시겠습니까?") != mrYes) return;;

    OM.SaveDevOptn(OM.m_sCrntDev);
    OM.SaveLastInfo();
    OM.SaveCmnOptn();
    OM.SaveMstOptn();
    OM.Close();
    pPaix -> SetOutputXBit(yETC_TwBzz , false);
    FrmMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pnMainShowClick(TObject *Sender)
{
    //OM.SaveCmnOptn();
    TPanel *pPanel = dynamic_cast<TPanel *>(Sender);
    ClosePage(m_iPageIndex);
    m_iPageIndex = pPanel -> Tag;
    ShowPage (m_iPageIndex);
    iBtnNum = m_iPageIndex;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pnMainShowMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *pPanel = dynamic_cast<TPanel *>(Sender);
    m_iPageIndex = pPanel -> Tag;


    switch(m_iPageIndex){
        case 0 : pnMainShow    -> BevelOuter = bvLowered; break ;
        case 1 : pnDeviceShow  -> BevelOuter = bvLowered; break ;
        case 2 : pnValveShow   -> BevelOuter = bvLowered; break ;
        case 3 : pnSpinMTShow  -> BevelOuter = bvLowered; break ;
        case 4 : pnTiltMTShow  -> BevelOuter = bvLowered; break ;
        case 5 : pnSettingShow -> BevelOuter = bvLowered; break ;
        case 6 : pnExit        -> BevelOuter = bvLowered; break ;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pnMainShowMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *pPanel = dynamic_cast<TPanel *>(Sender);
    m_iPageIndex = pPanel -> Tag;


    pnMainShow    -> BevelOuter = bvRaised ;
    pnDeviceShow  -> BevelOuter = bvRaised ;
    pnValveShow   -> BevelOuter = bvRaised ;
    pnSpinMTShow  -> BevelOuter = bvRaised ;
    pnTiltMTShow  -> BevelOuter = bvRaised ;
    pnSettingShow -> BevelOuter = bvRaised ;
    pnExit        -> BevelOuter = bvRaised ;

    switch(iBtnNum){
        case 0 : pnMainShow    -> BevelOuter = bvLowered; break ;
        case 1 : pnDeviceShow  -> BevelOuter = bvLowered; break ;
        case 2 : pnValveShow   -> BevelOuter = bvLowered; break ;
        case 3 : pnSpinMTShow  -> BevelOuter = bvLowered; break ;
        case 4 : pnTiltMTShow  -> BevelOuter = bvLowered; break ;
        case 5 : pnSettingShow -> BevelOuter = bvLowered; break ;
        case 6 : pnExit        -> BevelOuter = bvLowered; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall  TFrmMain::ShowPage (int index)
{
    switch(index)
    {
        case 0: FrmOperation -> Show() ; break ;
        case 1: FrmDevice    -> Show() ; break ;
        case 2: FrmValve     -> Show() ; break ;
        case 3: FrmSpinMT    -> Show() ; break ;
        case 4: FrmTiltMT    -> Show() ; break ;
        case 5: FrmSetting   -> Show() ; break ;
    }
}
//---------------------------------------------------------------------------
void __fastcall  TFrmMain::ClosePage(int index)
{
    //FrmMotorPosition ->Close();
    switch(index)
    {
        case 0: FrmOperation -> Hide() ; break ;
        case 1: FrmDevice    -> Hide() ; break ;
        case 2: FrmValve     -> Hide() ; break ;
        case 3: FrmSpinMT    -> Hide() ; break ;
        case 4: FrmTiltMT    -> Hide() ; break ;
        case 5: FrmSetting   -> Hide() ; break ;
    }
}


void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    StartThread();        
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Timer1Timer(TObject *Sender)
{
    Timer1 -> Enabled = false;

    if(!m_bEmgClear){
        pPaix -> GetInputX(g_lInputXIo);
        if(g_lInputXIo[xETC_EmgSw]) {
            SEQ->m_bErr = true;
            m_bEmgClear = true;
            FrmMsgOk->MsgOk("Error","Emergency 상태 입니다.");
            Trace("Error","Emergency 상태 입니다.");
        }
    }

    if(!g_lInputXIo[xETC_EmgSw]) { m_bEmgClear = false;}

    Timer1 -> Enabled = true;
}
//---------------------------------------------------------------------------

//bool __fastcall TFrmMain::FlickErr(void)
//{
//    if(m_bErr && !m_tmWorkErr.OnDelay(true, 5000)){
//        if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 100 )) m_bFlick = false; }
//        else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 4000)) m_bFlick = true ; }
//    }
//    else {
//        m_bErr = false;
//        m_tmWorkErr.Clear();
//    }
//}
//
//bool __fastcall TFrmMain::FlickWorkEnd(void)
//{
//    if(m_bWorkEnd && m_bWorkfinish && !m_tmWorkEnd.OnDelay(true, 5000)){
//        if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
//        else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }
//    }
//    else{
//        //m_bWorkEnd    = false;
//        m_bWorkfinish = false;
//        m_tmWorkErr.Clear();
//    }
//}

