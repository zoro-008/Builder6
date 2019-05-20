//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMsgOk.h"
#include "FormOperation.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "PaixMotion.h"
#include "SeqUnit.h"
#include "Global.h"
#include "FormMaster.h"
#include "sLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    OM.LoadDevOptn(OM.m_sCrntDev);
    tmUpdate->Enabled = true;
    lbModelName -> Caption = g_sModelName;
    pnDeviceNo -> Caption = OM.m_sCrntDev;
    //TiltSetUp용.
    Button1 -> Visible = false;
    Timer1 -> Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    int iTmClean   = SEQ -> m_tmClean.OnCheckCycle() / 1000 + 1;
    int iTmAir     = SEQ -> m_tmAir.OnCheckCycle()   / 1000 + 1;
    int iTmDry     = SEQ -> m_tmDry.OnCheckCycle()   / 1000 + 1;

    int iCleanTime = OM.DevOptn.iCleanTime / 1000 ;
    int iAirTime   = OM.DevOptn.iAirTime   / 1000 ;
    int iDryTime   = OM.DevOptn.iDryTime   / 1000 ;

    iCleanTime -= iTmClean ;
    iAirTime   -= iTmAir   ;
    iDryTime   -= iTmDry   ;

    if(iCleanTime < 0) iCleanTime = 0;
    if(iAirTime   < 0) iAirTime   = 0;
    if(iDryTime   < 0) iDryTime   = 0;

    pnCleanRPM  -> Caption = OM.DevOptn.iCleanRPM ;
    pnAirRPM    -> Caption = OM.DevOptn.iAirRPM   ;
    pnDryRPM    -> Caption = OM.DevOptn.iDryRPM   ;


    pnSpinSpeed -> Caption = OM.DevOptn.iSpinMtJogSpeed ;

    //int iRPM = pPaix -> GetRPM(miSpinMt);

    //pPaix -> GetNmcStatus(&nmcData);

    if(SEQ -> m_iStep == SEQ -> scCycleClean) {
        pnCleanSignal -> Caption       = "On "   ;
        pnCleanSignal -> Color         = clSilver;
        pnCleanSignal -> Font -> Color = clBlack ;
        pnIONSignal   -> Caption       = "Off"   ;
        pnIONSignal   -> Color         = clBlack ;
        pnIONSignal   -> Font -> Color = clWhite ;
        pnDrySignal   -> Caption       = "Off"   ;
        pnDrySignal   -> Color         = clBlack ;
        pnDrySignal   -> Font -> Color = clWhite ;
        pnSpinSpeed   -> Caption       = OM.DevOptn.iCleanRPM;
        pnCleanTime   -> Caption       = iCleanTime ;
    }
    else if(SEQ -> m_iStep == SEQ -> scCycleAIR  ) {
        pnCleanSignal -> Caption       ="Off"   ;
        pnCleanSignal -> Color         =clBlack ;
        pnCleanSignal -> Font -> Color =clWhite ;
        pnIONSignal   -> Caption       ="On "   ;
        pnIONSignal   -> Color         =clSilver;
        pnIONSignal   -> Font -> Color =clBlack ;
        pnDrySignal   -> Caption       ="Off"   ;
        pnDrySignal   -> Color         =clBlack ;
        pnDrySignal   -> Font -> Color =clWhite ;
        pnSpinSpeed   -> Caption       = OM.DevOptn.iCleanRPM;
        pnSpinSpeed   -> Caption       = OM.DevOptn.iAirRPM;
        pnAirTime     -> Caption       = iAirTime ;
    }
    else if(SEQ -> m_iStep == SEQ -> scCycleDRY  ) {
        pnCleanSignal -> Caption       ="Off"   ;
        pnCleanSignal -> Color         =clBlack ;
        pnCleanSignal -> Font -> Color =clWhite ;
        pnIONSignal   -> Caption       ="Off"   ;
        pnIONSignal   -> Color         =clBlack ;
        pnIONSignal   -> Font -> Color =clWhite ;
        pnDrySignal   -> Caption       ="On "   ;
        pnDrySignal   -> Color         =clSilver;
        pnDrySignal   -> Font -> Color =clBlack ;
        pnSpinSpeed   -> Caption       = OM.DevOptn.iCleanRPM;
        pnSpinSpeed   -> Caption       = OM.DevOptn.iAirRPM;
        pnSpinSpeed   -> Caption       = OM.DevOptn.iDryRPM;
        pnDryTime     -> Caption       = iDryTime ;
    }
    else {
        pnCleanSignal -> Caption       ="Off"   ;
        pnCleanSignal -> Color         =clBlack ;
        pnCleanSignal -> Font -> Color =clWhite ;
        pnIONSignal   -> Caption       ="Off"   ;
        pnIONSignal   -> Color         =clBlack ;
        pnIONSignal   -> Font -> Color =clWhite ;
        pnDrySignal   -> Caption       ="Off"   ;
        pnDrySignal   -> Color         =clBlack ;
        pnDrySignal   -> Font -> Color =clWhite ;
        pnSpinSpeed   -> Caption       = "0"  ;
        pnCleanTime   -> Caption       =OM.DevOptn.iCleanTime / 1000;
        pnAirTime     -> Caption       =OM.DevOptn.iAirTime   / 1000;
        pnDryTime     -> Caption       =OM.DevOptn.iDryTime   / 1000;
    }

    double dTiltCmd = pPaix -> GetCmdPos(miTiltMt);

     AnsiString sTiltCmd ;
    sTiltCmd.printf("%.2f", dTiltCmd);

    pnTiltCmdPos -> Caption = sTiltCmd;

    //if(OM.CmnOptn.bUsedWaterPump) {pnWorkName -> Caption = "Pump";   pnWorkName -> Font -> Color = clYellow  ;}
    //else                          {pnWorkName -> Caption = "Binary"; pnWorkName -> Font -> Color = clFuchsia ;}
         if(OM.DevOptn.bUseTopNozzle ) {pnWorkName -> Caption = "Top Nozzle   "; pnWorkName -> Font -> Color = clYellow  ;}
    else if(OM.DevOptn.bUseBtmNozzle ) {pnWorkName -> Caption = "Bottom Nozzle"; pnWorkName -> Font -> Color = clFuchsia ;}
    else if(OM.DevOptn.bUseTwinNozzle) {pnWorkName -> Caption = "Top&Bottom Nozzle  "; pnWorkName -> Font -> Color = clBlue    ;}

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{                                                                                                            
    int iPanelNo = ((TPanel *)Sender) -> Tag ;

    switch(iPanelNo){
        default :                                    break ;
        case 1  : btStart -> BevelOuter = bvLowered; break ;
        case 2  : btStop  -> BevelOuter = bvLowered; break ;
        case 3  : btEmg   -> BevelOuter = bvLowered; break ;
        case 4  : btReset -> BevelOuter = bvLowered; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag ;

    switch(iPanelNo){
        default :                                   break ;
        case 1  : btStart -> BevelOuter = bvRaised; break ;
        case 2  : btStop  -> BevelOuter = bvRaised; break ;
        case 3  : btEmg   -> BevelOuter = bvRaised; break ;
        case 4  : btReset -> BevelOuter = bvRaised; break ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::tmAxisStatusTimer(TObject *Sender)
{
    tmAxisStatus -> Enabled = false;

    //NMCAXESEXPR NmcData;
    pPaix -> GetNmcStatus(&g_NmcData);
    pPaix->GetInputX(m_lInputXIo);
    pPaix->GetOutputX(m_lOutputXIo);

    if(g_NmcData.nBusy[0]){pnSpinMotor -> Color = clLime    ;}
    else                  {pnSpinMotor -> Color = 0x00FF8080;}
    if(g_NmcData.nBusy[1]){pnTiltMotor -> Color = clLime    ;}
    else                  {pnTiltMotor -> Color = 0x00FF8080;}

    if(g_lOutputXIo[yETC_BNRAir     ] ){pnBNRSol   -> Color = clLime    ;}
    else                               {pnBNRSol   -> Color = 0x00FF8080;}

    //옵션 추가. JS

    if(OM.MstOptn.bUsedFrstMachine){
        if(m_lOutputXIo[yETC_PMPAir1     ] ){pnPMPSol   -> Color = clLime    ;}
        else                                {pnPMPSol   -> Color = 0x00FF8080;}
    }
    else {
             if( g_lOutputXIo[yETC_PMPAir1 ] && !g_lOutputXIo[yETC_PMPAir2]) {pnPMPSol   -> Color = clLime    ;}
        else if(!g_lOutputXIo[yETC_PMPAir1 ] &&  g_lOutputXIo[yETC_PMPAir2]) {pnPMPSol   -> Color = 0x00FF8080 ;}
    }

    if(m_lOutputXIo[yETC_IONAir     ] ){pnIONSol   -> Color = clLime    ;}
    else                               {pnIONSol   -> Color = 0x00FF8080;}

    if(m_lOutputXIo[yETC_BNRValve   ] ){pnWtrBNR   -> Color = clLime    ;}
    else                               {pnWtrBNR   -> Color = 0x00FF8080;}

    if(m_lOutputXIo[yETC_BTMValve   ] ){pnWtrPMP   -> Color = clLime    ;}
    else                               {pnWtrPMP   -> Color = 0x00FF8080;}

    if(m_lOutputXIo[yETC_DrainValve ] ){pnWtrDrain -> Color = clLime    ;}
    else                               {pnWtrDrain -> Color = 0x00FF8080;}

    if(m_lInputXIo[xETC_DoorFw]       ){pnDoorUp  -> Color = clLime    ; pnDoorDn -> Color = 0x00FF8080;}
    else if(m_lInputXIo[xETC_DoorBw]  ){pnDoorUp  -> Color = 0x00FF8080; pnDoorDn -> Color = clLime    ;}

    if(SEQ -> m_iStep != SEQ -> scIdle){btReset -> Enabled = false ; btReset -> Font -> Color = clGray ;}
    else                               {btReset -> Enabled = true  ; btReset -> Font -> Color = clBlack;}
    tmAxisStatus -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    if(SEQ -> m_bBtnResetDown == false) {
        pPaix -> SetAlarmResetOn(miSpinMt, true);
        pPaix -> SetAlarmResetOn(miTiltMt, true);

        pPaix -> SetAlarmResetOn(miSpinMt, false);
        pPaix -> SetAlarmResetOn(miTiltMt, false);

        pPaix -> MoveHome(miTiltMt);

        if(g_lInputXIo[xETC_DoorFw]){
            if(!g_lInputXIo[xETC_DoorSnsrNotDetect]){
                FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
                Trace("Error","Door Sensor가 감지되었습니다.");
                return;
            }
            pPaix -> SetOutputXBit(yETC_DoorFw, false);
            pPaix -> SetOutputXBit(yETC_DoorBw, true );
        }


        SEQ -> Reset();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
     int iPanelNo = ((TPanel *)Sender) -> Tag ;

     switch(iPanelNo){
        default :                                    break ;
        case 1  : SEQ->m_bBtnStart = true;           break ;
        case 2  : SEQ->m_bBtnStop  = true;           break ;
        case 3  : pPaix -> SuddenStop(miSpinMt);
                  pPaix -> SuddenStop(miTiltMt);
                  SEQ -> m_bBtnStop = true;
                  break ;
        case 4  :                                    break ;
     }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::lbModelNameDblClick(TObject *Sender)
{
    FrmMaster -> Show();
}
//---------------------------------------------------------------------------




void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
    if(Timer1 -> Enabled == true) Timer1 -> Enabled = false;
    else                  Timer1 -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Timer1Timer(TObject *Sender)
{
    Timer1 -> Enabled = false;

    const int iAcc = 327680;
    const int iDcc = 3276;                  //Dcc이 이상하여서 올라가거나 내려가면 Busy가 나중에 떨어짐......
    double dTiltCmdPos;                     //원인....파악 불가..............젠장.....
    dTiltCmdPos = pPaix -> GetCmdPos(miTiltMt);

    static double dLastTiltCmdPos = -1 ;

    static int iTiltStep = 0 ;
    if(iTiltStep == 0) iTiltStep = 10 ;
    switch(iTiltStep) {
        default : iTiltStep = 0  ; break ;
        case 10 : //pPaix -> SetOverrideDSpeed(miTiltMt , pPaix -> GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));
                  if(g_NmcData.nBusy[miTiltMt]) pPaix -> SetSpeed(miTiltMt , 10, iAcc , iDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed));
                  m_tmTemp.Clear();
                  iTiltStep++;
                  break ;

        case 11 : if(g_NmcData.nBusy[miTiltMt]) break ;
                  if(!m_tmTemp.OnDelay(true , 2000)) break;
                  if(pPaix -> m_dTiltTrgPos != OM.DevOptn.iTiltMtStartPs) { pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStartPs); m_tmTemp.Clear(); }
                  else                                                    { pPaix -> AbsMove(miTiltMt , OM.DevOptn.iTiltMtStopPs ); m_tmTemp.Clear(); }
                  break;
    }

    Timer1 -> Enabled = true;
}
//---------------------------------------------------------------------------


