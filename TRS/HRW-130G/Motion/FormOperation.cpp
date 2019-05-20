//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormMaster.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "PaixMotion.h"
#include "SeqUnit.h"
#include "SLogUnit.h"
#include "Timer.h"
#include "Global.h"
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
    //OM.LoadDevOptn(OM.m_sCrntDev);
    tmUpdate->Enabled = true;
    lbModelName -> Caption = g_sModelName;
    pnDeviceNo -> Caption = OM.m_sCrntDev;
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

    pnCleanRPM  -> Caption = OM.DevOptn.iCleanRPM       ;
    pnAirRPM    -> Caption = OM.DevOptn.iAirRPM         ;
    pnDryRPM    -> Caption = OM.DevOptn.iDryRPM         ;


    pnSpinSpeed -> Caption = OM.DevOptn.iSpinMtJogSpeed ;

    //int iRPM = pPaix -> GetRPM(miSpinMt);

    //pPaix -> GetNmcStatus(&nmcData);

    if(SEQ -> m_iStep == SEQ -> scCycleClean) {
        pnCleanSignal -> Caption       = "On "   ;
        pnCleanSignal -> Color         = clSilver;
        pnCleanSignal -> Font -> Color = clBlack ;
        pnAirSignal   -> Caption       = "Off"   ;
        pnAirSignal   -> Color         = clBlack ;
        pnAirSignal   -> Font -> Color = clWhite ;
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
        pnAirSignal   -> Caption       ="On "   ;
        pnAirSignal   -> Color         =clSilver;
        pnAirSignal   -> Font -> Color =clBlack ;
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
        pnAirSignal   -> Caption       ="Off"   ;
        pnAirSignal   -> Color         =clBlack ;
        pnAirSignal   -> Font -> Color =clWhite ;
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
        pnAirSignal   -> Caption       ="Off"   ;
        pnAirSignal   -> Color         =clBlack ;
        pnAirSignal   -> Font -> Color =clWhite ;
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

    static CDelayTimer Flicker ;
    lbRemote -> Caption = pPaix -> GetRemote() ? String("REMOTE") : String("OFFLINE_") + pPaix -> GetOffLineStep() ;
    if(pPaix -> GetRemote()) {
        lbRemote -> Font -> Color = clLime ;
    }
    else {
        if(Flicker.OnDelay(true , 500)){
            lbRemote -> Font -> Color =lbRemote -> Font -> Color == clSilver ? clRed : clSilver ;
            Flicker.Clear();
        }
    }


    if(pPaix -> Stat[miTiltMt].iHomeStep) {
        lbHoming -> Visible = true ;
        if(Flicker.OnDelay(true , 500)){
            lbHoming -> Font -> Color = lbHoming -> Font -> Color == clSilver ? clLime : clSilver ;
            Flicker.Clear();
        }
    }
    else {
        lbHoming -> Visible = false ;
    }

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

    //pPaix->GetNmcStatus(&g_NmcData);
    //pPaix->GetInputX(g_lInputXIo);
    //pPaix->GetOutputX(g_lOutputXIo);

    if(g_NmcData.nBusy[miSpinMt      ]){pnSpinMotor -> Color = clLime    ;}
    else                               {pnSpinMotor -> Color = 0x00FF8080;}
    if(g_NmcData.nBusy[miTiltMt      ]){pnTiltMotor -> Color = clLime    ;}
    else                               {pnTiltMotor -> Color = 0x00FF8080;}

    if(g_lOutputXIo[yETC_AirBlower1  ]){pnAir1Sol   -> Color = clLime    ;}
    else                               {pnAir1Sol   -> Color = 0x00FF8080;}

    if(g_lOutputXIo[yETC_AirBlower2  ]){pnAir2Sol   -> Color = clLime    ;}
    else                               {pnAir2Sol   -> Color = 0x00FF8080;}

    if(g_lOutputXIo[yETC_SupplyValve ]){pnWtrSupply -> Color = clLime    ;}
    else                               {pnWtrSupply -> Color = 0x00FF8080;}

    if(g_lOutputXIo[yETC_Drainage    ]){pnWtrDrain  -> Color = clLime    ;}
    else                               {pnWtrDrain  -> Color = 0x00FF8080;}

    if(g_lOutputXIo[yETC_Vaccum      ]){pnVaccumOn  -> Color = clLime    ;}
    else                               {pnVaccumOn  -> Color = 0x00FF8080;}

    if(g_lInputXIo[xETC_DoorFw       ]){pnDoorUp  -> Color = clLime    ; pnDoorDn -> Color = 0x00FF8080;}
    else if(g_lInputXIo[xETC_DoorBw  ]){pnDoorUp  -> Color = 0x00FF8080; pnDoorDn -> Color = clLime    ;}

    if(SEQ -> m_bRun != false) {btReset -> Enabled = false ; btReset -> Font -> Color = clGray ;}
    else                       {btReset -> Enabled = true  ; btReset -> Font -> Color = clBlack;}


    tmAxisStatus -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    pPaix -> SetAlarmResetOn(miSpinMt, true);
    pPaix -> SetAlarmResetOn(miTiltMt, true);


    pPaix -> SetAlarmResetOn(miSpinMt, false);
    pPaix -> SetAlarmResetOn(miTiltMt, false);

    SEQ -> m_bBtnReset = true;

    pPaix -> MoveHome(miTiltMt);

    Trace("Button","Reset Clicked");
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
     int iPanelNo = ((TPanel *)Sender) -> Tag ;

     String sButton = ((TPanel *)Sender) -> Caption ;

     switch(iPanelNo){
        default :                                    break ;
        case 1  : SEQ->m_bBtnStart = true;           break ;
        case 2  : SEQ->m_bBtnStop  = true;
                  break ;
        case 3  : pPaix -> SuddenStop(miSpinMt);
                  pPaix -> SuddenStop(miTiltMt);
                  SEQ -> m_bBtnStop = true;
                  break ;
        case 4  :                                    break ;
     }

     Trace("Button",(sButton +" Clicked").c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::lbModelNameDblClick(TObject *Sender)
{
    FrmMaster -> Show();     
}
//---------------------------------------------------------------------------

