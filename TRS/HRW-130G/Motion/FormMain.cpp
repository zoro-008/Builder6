//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
//---------------------------------------------------------------------------
//Form Reference
//---------------------------------------------------------------------------
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "FormOperation.h"
#include "FormValve.h"
#include "FormSpinMotor.h"
#include "FormSetting.h"
#include "FormTiltMotor.h"
#include "FormDevice.h"
#include "FormMaster.h"
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "UserIni.h"
#include "OptionMan.h"
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


    FrmMain -> Color = BACKGRAUNDCOLOR;
    FrmMain -> ClientHeight = 768 ;
    FrmMain -> ClientWidth  = 1024;

    //pPaix -> GetInputX(g_lInputXIo);



    //OM.LoadLastInfo();
    //OM.LoadDevOptn(OM.m_sCrntDev);
    //OM.LoadCmnOptn();

    g_sModelName = "HRW-130G";

    //Motion
    pPaix = new CPaixMotion();
    //pPaix -> SetMsgFunc(Msg) ;

    //Seq.
    SEQ = new CSEQ();
    //pSeq -> SetMsgFunc(Msg) ;

    Trace("Program","Start");


}


void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    //EndThread();

    Trace("PAIX PARA","FormDestroy()");
    
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
            FrmMsgOk->MsgOk("Error","Motor Alram상태 입니다.");
            Trace("Error","Motor Alram상태 입니다.");
        }
    }

    if(g_NmcData.nAlarm[0] == 0 && g_NmcData.nAlarm[1] == 0) {                     m_bAlrmClear = false;}


    if(SEQ -> m_bRun != false) {
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

    if(g_lInputXIo[xETC_DoorFw]) {pPaix -> SetOutputXBit(yETC_DoorFw, true ); pPaix -> SetOutputXBit(yETC_DoorBw, false);}
    else                         {pPaix -> SetOutputXBit(yETC_DoorFw, false); pPaix -> SetOutputXBit(yETC_DoorBw, true );}


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

    Trace("PAIX PARA","FormShow()");
    pPaix -> ApplyPara();


}
void __fastcall TFrmMain::pnExitClick(TObject *Sender)
{
    if(MsgYesNo("Exit","종료 하시겠습니까?") != mrYes) return;;

    Trace("Exit","Yes");
    OM.Close();

    pPaix -> SetOutputXBit(yETC_Buzz , false);
    pPaix -> SetOutputXBit(yETC_SpinBreak , false);

    FrmMain->Close();
}
//---------------------------------------------------------------------------

String GetPageName(int _iPageTag)
{
    if(_iPageTag == 0) return "MAIN"      ;
    if(_iPageTag == 1) return "DEVICE"    ;
    if(_iPageTag == 2) return "VALVE"     ;
    if(_iPageTag == 3) return "SPIN MT"   ;
    if(_iPageTag == 4) return "TILT MT"   ;
    if(_iPageTag == 5) return "SETTING"   ;
    if(_iPageTag == 6) return "" ;
}

void __fastcall TFrmMain::pnMainShowClick(TObject *Sender)
{
    TPanel *pPanel = dynamic_cast<TPanel *>(Sender);
    ClosePage(m_iPageIndex);

    Trace("PageChange" , (GetPageName(m_iPageIndex) + " to " + GetPageName(pPanel -> Tag)).c_str());

    m_iPageIndex = pPanel -> Tag;

    ShowPage (m_iPageIndex);



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

    pnMainShow    -> BevelOuter = pnMainShow    -> Tag == index ? bvLowered : bvRaised ;
    pnDeviceShow  -> BevelOuter = pnDeviceShow  -> Tag == index ? bvLowered : bvRaised ;
    pnValveShow   -> BevelOuter = pnValveShow   -> Tag == index ? bvLowered : bvRaised ;
    pnSpinMTShow  -> BevelOuter = pnSpinMTShow  -> Tag == index ? bvLowered : bvRaised ;
    pnTiltMTShow  -> BevelOuter = pnTiltMTShow  -> Tag == index ? bvLowered : bvRaised ;
    pnSettingShow -> BevelOuter = pnSettingShow -> Tag == index ? bvLowered : bvRaised ;
    pnExit        -> BevelOuter = pnExit        -> Tag == index ? bvLowered : bvRaised ;

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
        case 4: FrmSetting   -> Hide() ; break ;
        case 5: FrmSetting   -> Hide() ; break ;
    }
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


    //DevOptn 변했을때 시점.. 로그 남김.
    static int    iPreSpinMtJogSpeed = OM.DevOptn.iSpinMtJogSpeed ;
    static int    iPreTiltMtStartPs  = OM.DevOptn.iTiltMtStartPs  ;
    static int    iPreTiltMtStopPs   = OM.DevOptn.iTiltMtStopPs   ;
    static double dPreTiltMtSpeed    = OM.DevOptn.dTiltMtSpeed    ;
    static int    iPreCleanRPM       = OM.DevOptn.iCleanRPM       ;
    static int    iPreAirRPM         = OM.DevOptn.iAirRPM         ;
    static int    iPreDryRPM         = OM.DevOptn.iDryRPM         ;
    static int    iPreCleanTime      = OM.DevOptn.iCleanTime      ;
    static int    iPreAirTime        = OM.DevOptn.iAirTime        ;
    static int    iPreDryTime        = OM.DevOptn.iDryTime        ;
    static int    iPreWaterDrainTime = OM.DevOptn.iWaterDrainTime ;

    if(iPreSpinMtJogSpeed != OM.DevOptn.iSpinMtJogSpeed ){Trace("DevOptn Change", ("iSpinMtJogSpeed " + String(iPreSpinMtJogSpeed) + " to " + String(OM.DevOptn.iSpinMtJogSpeed)).c_str());}
    if(iPreTiltMtStartPs  != OM.DevOptn.iTiltMtStartPs  ){Trace("DevOptn Change", ("iTiltMtStartPs  " + String(iPreTiltMtStartPs ) + " to " + String(OM.DevOptn.iTiltMtStartPs )).c_str());}
    if(iPreTiltMtStopPs   != OM.DevOptn.iTiltMtStopPs   ){Trace("DevOptn Change", ("iTiltMtStopPs   " + String(iPreTiltMtStopPs  ) + " to " + String(OM.DevOptn.iTiltMtStopPs  )).c_str());}
    if(dPreTiltMtSpeed    != OM.DevOptn.dTiltMtSpeed    ){Trace("DevOptn Change", ("dTiltMtSpeed    " + String(dPreTiltMtSpeed   ) + " to " + String(OM.DevOptn.dTiltMtSpeed   )).c_str());}
    if(iPreCleanRPM       != OM.DevOptn.iCleanRPM       ){Trace("DevOptn Change", ("iCleanRPM       " + String(iPreCleanRPM      ) + " to " + String(OM.DevOptn.iCleanRPM      )).c_str());}
    if(iPreAirRPM         != OM.DevOptn.iAirRPM         ){Trace("DevOptn Change", ("iAirRPM         " + String(iPreAirRPM        ) + " to " + String(OM.DevOptn.iAirRPM        )).c_str());}
    if(iPreDryRPM         != OM.DevOptn.iDryRPM         ){Trace("DevOptn Change", ("iDryRPM         " + String(iPreDryRPM        ) + " to " + String(OM.DevOptn.iDryRPM        )).c_str());}
    if(iPreCleanTime      != OM.DevOptn.iCleanTime      ){Trace("DevOptn Change", ("iCleanTime      " + String(iPreCleanTime     ) + " to " + String(OM.DevOptn.iCleanTime     )).c_str());}
    if(iPreAirTime        != OM.DevOptn.iAirTime        ){Trace("DevOptn Change", ("iAirTime        " + String(iPreAirTime       ) + " to " + String(OM.DevOptn.iAirTime       )).c_str());}
    if(iPreDryTime        != OM.DevOptn.iDryTime        ){Trace("DevOptn Change", ("iDryTime        " + String(iPreDryTime       ) + " to " + String(OM.DevOptn.iDryTime       )).c_str());}
    if(iPreWaterDrainTime != OM.DevOptn.iWaterDrainTime ){Trace("DevOptn Change", ("iWaterDrainTime " + String(iPreWaterDrainTime) + " to " + String(OM.DevOptn.iWaterDrainTime)).c_str());}

    iPreSpinMtJogSpeed = OM.DevOptn.iSpinMtJogSpeed ;
    iPreTiltMtStartPs  = OM.DevOptn.iTiltMtStartPs  ;
    iPreTiltMtStopPs   = OM.DevOptn.iTiltMtStopPs   ;
    dPreTiltMtSpeed    = OM.DevOptn.dTiltMtSpeed    ;
    iPreCleanRPM       = OM.DevOptn.iCleanRPM       ;
    iPreAirRPM         = OM.DevOptn.iAirRPM         ;
    iPreDryRPM         = OM.DevOptn.iDryRPM         ;
    iPreCleanTime      = OM.DevOptn.iCleanTime      ;
    iPreAirTime        = OM.DevOptn.iAirTime        ;
    iPreDryTime        = OM.DevOptn.iDryTime        ;
    iPreWaterDrainTime = OM.DevOptn.iWaterDrainTime ;






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

