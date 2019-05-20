//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTiltMotor.h"
#include "OptionMan.h"
#include "PaixMotion.h"
#include "SLogUnit.h"
#include "UtilDefine.h"
#include "global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmTiltMT *FrmTiltMT;
//---------------------------------------------------------------------------
__fastcall TFrmTiltMT::TFrmTiltMT(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btTiltCwMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag ;

    switch(iPanelNo){
        default :                                       break ;
        case 1  : btTiltCw   -> BevelOuter = bvLowered;
                  pPaix -> MoveJogRPM(miTiltMt, OM.DevOptn.dTiltMtSpeed);
                  break ;

        case 2  : btTiltCcw  -> BevelOuter = bvLowered;
                  pPaix -> MoveJogRPM(miTiltMt, -OM.DevOptn.dTiltMtSpeed);
                  break ;

        case 3  : btTiltHome -> BevelOuter = bvLowered;
                  break ;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btTiltCwMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iPanelNo = ((TPanel *) Sender) -> Tag;

    if(iPanelNo != 3){
        pPaix -> StopMotor(miTiltMt);
    }
    btTiltCw   -> BevelOuter = bvRaised ;
    btTiltCcw  -> BevelOuter = bvRaised ;
    btTiltHome -> BevelOuter = bvRaised ;
}
//---------------------------------------------------------------------------
                              

void __fastcall TFrmTiltMT::bt1Click(TObject *Sender)
{
    AnsiString sEditText ;
    int iManNo = ((TButton *)Sender) -> Tag ;

    switch(iEditNo){
        default :                                       break ;
        case 1  : sEditText = edTiltStartPs    -> Text; break ;
        case 2  : sEditText = edTiltStopPs     -> Text; break ;
        case 3  : sEditText = edTiltSettingSpd -> Text; break ;
        case 4  : sEditText = edWtrDrainTime   -> Text; break ;
    }

    if(sEditText == "0") sEditText = "";

    sEditText += iManNo;

    switch(iEditNo){
        default :                                        break ;
        case 1  : edTiltStartPs    -> Text = sEditText ; break ;
        case 2  : edTiltStopPs     -> Text = sEditText ; break ;
        case 3  : edTiltSettingSpd -> Text = sEditText ; break ;
        case 4  : edWtrDrainTime   -> Text = sEditText ; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btBackClick(TObject *Sender)
{
    AnsiString sEditText ;
    switch(iEditNo){
        default :                                       break ;
        case 1  : sEditText = edTiltStartPs    -> Text; break ;
        case 2  : sEditText = edTiltStopPs     -> Text; break ;
        case 3  : sEditText = edTiltSettingSpd -> Text; break ;
        case 4  : sEditText = edWtrDrainTime   -> Text; break ;
    }
    sEditText = sEditText.Delete(sEditText.Length(), 1);

    switch(iEditNo){
        default :                                        break ;
        case 1  : edTiltStartPs    -> Text = sEditText ; break ;
        case 2  : edTiltStopPs     -> Text = sEditText ; break ;
        case 3  : edTiltSettingSpd -> Text = sEditText ; break ;
        case 4  : edWtrDrainTime   -> Text = sEditText ; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btClearClick(TObject *Sender)
{
    switch(iEditNo){
        default :
        case 1  : edTiltStartPs    -> Text = 0; break ;
        case 2  : edTiltStopPs     -> Text = 0; break ;
        case 3  : edTiltSettingSpd -> Text = 0; break ;
        case 4  : edWtrDrainTime   -> Text = 0; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btOKClick(TObject *Sender)
{
    const int iAccDcc = 32768;
    switch(iEditNo){
        default : break ;
        case 1  : if(edTiltStartPs -> Text == ""){edTiltStartPs -> Text = 0;}
                  Trace("Tilt Stt Pos Change", (String(OM.DevOptn.iTiltMtStartPs) + " To " + edTiltStartPs -> Text.ToInt()).c_str() ) ;
                  OM.DevOptn.iTiltMtStartPs = edTiltStartPs -> Text.ToInt();
                  pnTiltStartPs -> Caption  = OM.DevOptn.iTiltMtStartPs ;
                  break ;

        case 2  : if(edTiltStopPs     -> Text == ""){edTiltStopPs     -> Text = 0;}
                  Trace("Tilt Stp Pos Change", (String(OM.DevOptn.iTiltMtStopPs) + " To " + edTiltStopPs -> Text.ToInt()).c_str() ) ;
                  OM.DevOptn.iTiltMtStopPs = edTiltStopPs -> Text.ToInt();
                  pnTiltStopPs  -> Caption = OM.DevOptn.iTiltMtStopPs ;
                  break ;

        case 3  : if(edTiltSettingSpd -> Text == ""){edTiltSettingSpd -> Text = 0;}
                  if(edTiltSettingSpd -> Text.ToInt() > 500){edTiltSettingSpd -> Text = 500;}
                  Trace("Tilt Mt Speed Change", (String(OM.DevOptn.dTiltMtSpeed) + " To " + edTiltSettingSpd -> Text.ToInt() / 100).c_str() ) ;
                  OM.DevOptn.dTiltMtSpeed     = edTiltSettingSpd -> Text.ToInt() / 100 ; //1호기랑 맞추느라 RPM단위라서 너무 빨라서 100으로 나눔
                  //pPaix -> SetSpeed(miTiltMt , 1 , iAccDcc, iAccDcc , pPaix -> GetPulseSpeedFromRPM(miTiltMt , OM.DevOptn.dTiltMtSpeed));
                  pnTiltSettingSpd -> Caption = OM.DevOptn.dTiltMtSpeed * 100 ;
                  break ;

        case 4  : if(edWtrDrainTime -> Text == ""){edWtrDrainTime -> Text = 0;}
                  Trace("Drain Time Change", (String(OM.DevOptn.iWaterDrainTime) + " To " + edWtrDrainTime -> Text.ToInt() * 1000).c_str() ) ;
                  OM.DevOptn.iWaterDrainTime  = edWtrDrainTime -> Text.ToInt() * 1000 ;
                  pnWtrDrainTime -> Caption = OM.DevOptn.iWaterDrainTime / 1000 ;
                  break ;
    }
    OM.SaveDevOptn(OM.m_sCrntDev);

    pnButton         -> Visible = false ;
    edTiltStartPs    -> Visible = false ;
    edTiltStopPs     -> Visible = false ;
    edTiltSettingSpd -> Visible = false ;
    edWtrDrainTime   -> Visible = false ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::FormShow(TObject *Sender)
{
    lbModelName -> Caption = g_sModelName;
    //OM.LoadDevOptn(OM.m_sCrntDev);

    pnTiltStartPs    -> Caption = OM.DevOptn.iTiltMtStartPs ;
    pnTiltStopPs     -> Caption = OM.DevOptn.iTiltMtStopPs  ;
    pnTiltSettingSpd -> Caption = OM.DevOptn.dTiltMtSpeed    * 100;
    pnWtrDrainTime   -> Caption = OM.DevOptn.iWaterDrainTime / 1000;

    edTiltStartPs    -> Text = 0;
    edTiltStopPs     -> Text = 0;
    edTiltSettingSpd -> Text = 0;
    edWtrDrainTime   -> Text = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::tmTiltMTTimer(TObject *Sender)
{
    tmTiltMT -> Enabled = false;
    double dTiltCmd;

    dTiltCmd = pPaix -> GetCmdPos(miTiltMt);

    AnsiString sTiltCmd ;
    sTiltCmd.printf("%.2f", dTiltCmd);
    pnTiltCmdPs -> Caption = sTiltCmd;
    tmTiltMT -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTiltMT::btTiltHomeClick(TObject *Sender)
{
    pPaix -> MoveHome(miTiltMt);
}
//---------------------------------------------------------------------------
void __fastcall TFrmTiltMT::pnTiltStartPsClick(TObject *Sender)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag;
    iEditNo = iPanelNo;

    pnButton         -> Visible = false ;
    edTiltStartPs    -> Visible = false ;
    edTiltStopPs     -> Visible = false ;
    edTiltSettingSpd -> Visible = false ;
    edWtrDrainTime   -> Visible = false ;

    switch(iPanelNo){
        default : break ;
        case 1  : edTiltStartPs    -> Visible = true; edTiltStartPs    -> Text = "0" ; pnButton -> Visible = true; break;
        case 2  : edTiltStopPs     -> Visible = true; edTiltStopPs     -> Text = "0" ; pnButton -> Visible = true; break;
        case 3  : edTiltSettingSpd -> Visible = true; edTiltSettingSpd -> Text = "0" ; pnButton -> Visible = true; break;
        case 4  : edWtrDrainTime   -> Visible = true; edWtrDrainTime   -> Text = "0" ; pnButton -> Visible = true; break;
    }                                                                              
}
//---------------------------------------------------------------------------

