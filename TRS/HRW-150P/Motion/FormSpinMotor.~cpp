//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSpinMotor.h"
#include "OptionMan.h"
#include "PaixMotion.h"
#include "Utildefine.h"
#include "FormMsgOk.h"
#include "SEQUnit.h"
#include "SLogUnit.h"
#include "global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSpinMT *FrmSpinMT;
//---------------------------------------------------------------------------
__fastcall TFrmSpinMT::TFrmSpinMT(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btJogMoveMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pPaix->GetInputX(g_lInputXIo);

    if(SEQ -> m_bRun == false) {
        if(g_lInputXIo[xETC_DoorFw]){
            pPaix -> SetServoOn(miSpinMt , true);
            btJogMove -> BevelOuter = bvLowered ;
            Sleep(100);
            pPaix -> MoveJogRPM(miSpinMt, OM.DevOptn.iSpinMtJogSpeed);
        }
        else {
            FrmMsgOk->MsgOk("Warring" , "Door가 열렸습니다." );
            Trace("Warring" , "Door가 열렸습니다." );
        }
    }
    else {
       FrmMsgOk->MsgOk("Error" , "장비가 동작 중입니다." );
       Trace("Error" , "장비가 동작 중입니다." );
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btJogMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SEQ -> m_bRun == false) {
        btJogMove -> BevelOuter = bvRaised ;
        pPaix -> DecStop(miSpinMt);
        bSpinStop = false;
    }
    else {
       FrmMsgOk->MsgOk("Error" , "장비가 동작 중입니다." );
       Trace("Error" , "장비가 동작 중입니다." );
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btOKClick(TObject *Sender)
{
    if(edJogSpeed -> Text == "") edJogSpeed -> Text = 0;
    int iSpeed = edJogSpeed -> Text.ToInt();
    if(iSpeed > 1000) iSpeed = 1000;
    else if(iSpeed < 1) iSpeed = 1;

    pnJogSpeed -> Caption = iSpeed;
    OM.DevOptn.iSpinMtJogSpeed = iSpeed ;
    //pPaix -> MoveRPM(miSpinMt, iSpeed);
    OM.SaveDevOptn(OM.m_sCrntDev);
    
    pnButton -> Visible = false;
    edJogSpeed -> Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::bt1Click(TObject *Sender)
{
    AnsiString sJogSpeed ;
    int iManNo = ((TButton *)Sender) -> Tag ;

    sJogSpeed = edJogSpeed -> Text ;
    if(sJogSpeed == "0") sJogSpeed = "";

    sJogSpeed += iManNo;

    edJogSpeed -> Text = sJogSpeed;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btBackClick(TObject *Sender)
{
    AnsiString sJogSpeed ;
    sJogSpeed = edJogSpeed -> Text;

    sJogSpeed = sJogSpeed.Delete(sJogSpeed.Length(), 1);
    edJogSpeed -> Text = sJogSpeed;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btClearClick(TObject *Sender)
{
    edJogSpeed -> Text = 0;
}
//---------------------------------------------------------------------------



void __fastcall TFrmSpinMT::FormShow(TObject *Sender)
{
    lbModelName -> Caption = g_sModelName;
    OM.LoadDevOptn(OM.m_sCrntDev);

    pnJogSpeed -> Caption = OM.DevOptn.iSpinMtJogSpeed ;

    edJogSpeed -> Text = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSpinMT::tmSpinMTTimer(TObject *Sender)
{
    tmSpinMT -> Enabled = false;

    NMCAXESEXPR NmcData;
    pPaix -> GetNmcStatus(&NmcData);

    bool bBusy;

    bBusy   = NmcData.nBusy[miSpinMt];

    pnReady-> Color = bBusy?(TColor)0x00FF8080:clLime;
    pnBusy -> Color = bBusy?clLime:(TColor)0x00FF8080;

    if(!bSpinStop && !bBusy){
        pPaix -> SetServoOn(miSpinMt , false);
        bSpinStop = true;
    }


    tmSpinMT -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::Button1Click(TObject *Sender)
{
    pPaix -> SetEncPos(miSpinMt, 0);
    pPaix -> SetCmdPos(miSpinMt, 0);

}
//---------------------------------------------------------------------------



void __fastcall TFrmSpinMT::Panel7MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pPaix -> JogMove(miSpinMt , CCW);    
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::Button4Click(TObject *Sender)
{
    PARASPEED	Speed = {0,};
    PARALOGIC	Logic = {0,};
    int nAxis;

    for( nAxis = 0; nAxis < 8 ; nAxis ++)
    {
        pPaix->GetLogic(nAxis, &Logic);                                                             
	//ReadLogic(nAxis, &Logic);
	pPaix->GetSpeed(nAxis, &Speed);
	//SpeedInfo(nAxis, &Speed);
    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmSpinMT::pnJogSpeedClick(TObject *Sender)
{
    edJogSpeed -> Text = OM.DevOptn.iSpinMtJogSpeed ;
    pnButton -> Visible = true;
    edJogSpeed -> Visible = true;        
}
//---------------------------------------------------------------------------










