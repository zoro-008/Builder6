//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSpinMotor.h"
#include "OptionMan.h"
#include "PaixMotion.h"
#include "global.h"
#include "SLogUnit.h"
#include "Utildefine.h"
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
    pPaix -> SetServoOn(miSpinMt , true);
    btJogMove -> BevelOuter = bvLowered ;
    pPaix -> MoveJogRPM(miSpinMt, OM.DevOptn.iSpinMtJogSpeed);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btJogMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btJogMove -> BevelOuter = bvRaised ;
    pPaix -> DecStop(miSpinMt);

}
//---------------------------------------------------------------------------

void __fastcall TFrmSpinMT::btOKClick(TObject *Sender)
{
    if(edJogSpeed -> Text == "") edJogSpeed -> Text = 0;
    int iSpeed = edJogSpeed -> Text.ToInt();
    if(iSpeed > 2000) iSpeed = 2000;
    else if(iSpeed < 1) iSpeed = 1;

    Trace("Jog Speed Change", (String(OM.DevOptn.iSpinMtJogSpeed) + " To " + iSpeed).c_str() ) ;

    OM.DevOptn.iSpinMtJogSpeed = iSpeed;
    pnJogSpeed -> Caption = OM.DevOptn.iSpinMtJogSpeed;



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
    //OM.LoadDevOptn(OM.m_sCrntDev);

    pnJogSpeed -> Caption = OM.DevOptn.iSpinMtJogSpeed ;

    edJogSpeed -> Text = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSpinMT::tmSpinMTTimer(TObject *Sender)
{
    tmSpinMT -> Enabled = false;

    bool bBusy;

    bBusy   = g_NmcData.nBusy[0];

    pnReady-> Color = bBusy?(TColor)0x00FF8080:clLime;
    pnBusy -> Color = bBusy?clLime:(TColor)0x00FF8080;


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
    edJogSpeed -> Text = OM.DevOptn.iSpinMtJogSpeed;
    pnButton -> Visible = true;

    edJogSpeed -> Text    = "0";
    edJogSpeed -> Visible = true;

}
//---------------------------------------------------------------------------



