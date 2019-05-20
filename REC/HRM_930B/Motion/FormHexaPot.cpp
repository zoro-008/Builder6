//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormHexaPot.h"
#include "HexaPot.h"
#include "UtilDefine.h"
#include "Stage.h"
#include "ManualMan.h"
#include "RightTool.h"
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmHexaPot *FrmHexaPot;
//---------------------------------------------------------------------------
__fastcall TFrmHexaPot::TFrmHexaPot(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//enum ePI_Axis {
//    paX = 0 ,
//    paY = 1 ,
//    paZ = 2 ,
//    paU = 3 ,
//    paV = 4 ,
//    paW = 5 ,
//    MAX_PI_AXIS
//};

















void __fastcall TFrmHexaPot::rb1umClick(TObject *Sender)
{
    edHexaPitch -> Text = "0.001" ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::rb5umClick(TObject *Sender)
{
    edHexaPitch -> Text = "0.005" ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::rb10umClick(TObject *Sender)
{
    edHexaPitch -> Text = "0.01"  ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::edHexaPitchExit(TObject *Sender)
{
    double dTemp = StrToFloatDef(edHexaPitch -> Text ,0.001);
    edHexaPitch -> Text = dTemp ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btHexaPClick(TObject *Sender)
{
    //ePI_Axis Axis = dynamic_cast<TBitBtn*>(Sender) -> Tag ;
    ePI_Axis Axis = ((TBitBtn *)Sender) -> Tag ;
    double JogPos = edHexaPitch -> Text.ToDouble();
    HexaPot.MoveIncRes(Axis, +JogPos);

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btHexaNClick(TObject *Sender)
{
    ePI_Axis Axis = ((TBitBtn *)Sender) -> Tag ;
    double JogPos = edHexaPitch -> Text.ToDouble();
    HexaPot.MoveIncRes(Axis, -JogPos);

    HexaPot.Move();
}
//---------------------------------------------------------------------------



void __fastcall TFrmHexaPot::tmEncTimer(TObject *Sender)
{
    edEncX -> Text = HexaPot.GetEncPos(paX);
    edEncY -> Text = HexaPot.GetEncPos(paY);
    edEncZ -> Text = HexaPot.GetEncPos(paZ);
    edEncU -> Text = HexaPot.GetEncPos(paU);
    edEncV -> Text = HexaPot.GetEncPos(paV);
    edEncW -> Text = HexaPot.GetEncPos(paW);

    lbErr -> Caption = HexaPot.GetLastErrMsg() ;

    edEncLinearX -> Text = MT_GetEncPos(miSTG_XStage);
    edEncLinearY -> Text = MT_GetEncPos(miSTG_YStage);


    btVisnLive -> Caption      = IO_GetY(yVisnR_Live) ? "Vision Live On" : "Vision Live Off";
    btVisnLive -> Font ->Color = IO_GetY(yVisnR_Live) ? clLime           : clBlack          ;

    int r,c ;
    bool bExistWork = STG.GetWorkingRC(r,c);

    btMainMaster -> Enabled = bExistWork ;
    btMainSlave  -> Enabled = bExistWork ;
    btSubMaster  -> Enabled = bExistWork && r != 0 && c != 0;
    btSubSlave   -> Enabled = bExistWork && r != 0 && c != 0;



}
//---------------------------------------------------------------------------



void __fastcall TFrmHexaPot::btLinearXRightClick(TObject *Sender)
{
    double JogPos = edHexaPitch -> Text.ToDouble();
    MT_GoIncMan(miSTG_XStage , -JogPos) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btLinearXLeftClick(TObject *Sender)
{
    double JogPos = edHexaPitch -> Text.ToDouble();
    MT_GoIncMan(miSTG_XStage , JogPos) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btLinearYBwdClick(TObject *Sender)
{
    double JogPos = edHexaPitch -> Text.ToDouble();
    MT_GoIncMan(miSTG_YStage , -JogPos) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btLinearYFwdClick(TObject *Sender)
{
    double JogPos = edHexaPitch -> Text.ToDouble();
    MT_GoIncMan(miSTG_YStage ,  JogPos) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btMainMasterClick(TObject *Sender)
{

    RTL.SetCadOfsGain(tlRVisn);

    int iPosId = ((TButton *)Sender) -> Tag ;

    int r,c ;
    bool bExistWork = STG.GetWorkingRC(r,c);

    CCadVisnPoint * pVisnPos = NULL ;
    if(c == 0 && r == 0) { //스텐드 얼론 놈들.
        if(CAD.GetCol()>1) {
            if(iPosId == 0) {//Main Master
                pVisnPos = CAD.CM[c][r].CM_RAlnMR ; //지금작업하는놈을 맞춰줘야 하는 기준
            }
            else if(iPosId == 1) { //Main Slave
                pVisnPos = CAD.CM[c][r].CM_RAlnSR ; //지금작업하는놈을 맞춰줘야 하는 기준
            }
        }
        else if(CAD.GetRow()>1) {
            if(iPosId == 0) {//Main Master
                pVisnPos = CAD.CM[c][r].CM_RAlnMB ; //지금작업하는놈을 맞춰줘야 하는 기준
            }
            else if(iPosId == 1) { //Main Slave
                pVisnPos = CAD.CM[c][r].CM_RAlnSB ; //지금작업하는놈을 맞춰줘야 하는 기준
            }
        }
    }
    else if(c>0 && r>0){  //멀티 버팅 놈들.
        if(iPosId == 0) {//Main Master
            pVisnPos = CAD.CM[c][r].CM_RAlnML ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
        else if(iPosId == 1) { //Main Slave
            pVisnPos = CAD.CM[c][r].CM_RAlnSL ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
        else if(iPosId == 2) {//Sub Master
            pVisnPos = CAD.CM[c][r].CM_RAlnMT ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
        else if(iPosId == 3) { //Sub Slave
            pVisnPos = CAD.CM[c][r].CM_RAlnST ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
    }
    else if(c>0){
        if(iPosId == 0) {//Main Master
            pVisnPos = CAD.CM[c][r].CM_RAlnML ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
        else if(iPosId == 1) { //Main Slave
            pVisnPos = CAD.CM[c][r].CM_RAlnSL ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
    }
    else if(r>0){
        if(iPosId == 0) {//Main Master
            pVisnPos = CAD.CM[c][r].CM_RAlnMT ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
        else if(iPosId == 1) { //Main Slave
            pVisnPos = CAD.CM[c][r].CM_RAlnST ; //지금작업하는놈을 맞춰줘야 하는 기준
        }
    }
    else { //첫째장. 첫번째 장은 기준없이 그냥 비전 기준으로 붙인다.
        return ;
    }

    double dX = pVisnPos -> GetCntrPosX() ;
    double dY = pVisnPos -> GetCntrPosY() ;

    dX = dX-g_tMidPosOfs.dX;
    dY = dY-g_tMidPosOfs.dY;

    MM.SetCadMove(tlRVisn, dX,dY);

}
//---------------------------------------------------------------------------

void __fastcall TFrmHexaPot::btVisnLiveClick(TObject *Sender)
{
    IO_SetY(yVisnR_Live , !IO_GetY(yVisnR_Live));
}
//---------------------------------------------------------------------------


