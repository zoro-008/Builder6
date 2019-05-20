//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormValve.h"
#include "PaixMotion.h"
#include "UtilDefine.h"
#include "global.h"
#include "SEQUnit.h"
#include "FormMsgOk.h"
#include "SLogUnit.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmValve *FrmValve;
//---------------------------------------------------------------------------
__fastcall TFrmValve::TFrmValve(TComponent* Owner)
        : TForm(Owner)
{
    lbModelName -> Caption = g_sModelName;
}
//---------------------------------------------------------------------------
void __fastcall TFrmValve::btDoorClick(TObject *Sender)
{
    if(SEQ -> m_bRun == false){

        int iBtNo = ((TPanel *)Sender) -> Tag;

        pPaix -> GetOutputX(g_lOutputXIo);
        switch(iBtNo){
            case  1: if(g_lOutputXIo[yETC_DoorBw]){
                         pPaix -> SetOutputXBit(yETC_DoorFw, true );
                         pPaix -> SetOutputXBit(yETC_DoorBw, false);
                         Trace("Cylinder" , "yETC_DoorFw " );
                         break;
                     }
                     else if(g_lOutputXIo[yETC_DoorFw]){
                         pPaix -> SetOutputXBit(yETC_DoorFw, false);
                         pPaix -> SetOutputXBit(yETC_DoorBw, true );
                         Trace("Cylinder" , "yETC_DoorBw " );
                         break;
                     }
                     break;
            case  2: if(g_lOutputXIo[yETC_Vaccum]){
                         pPaix->SetOutputXBit(yETC_Vaccum     , false);
                         Trace("Cylinder" , "yETC_Vaccum Off " );
                         break;
                     }
                     else {
                         pPaix->SetOutputXBit(yETC_Vaccum     , true );
                         Trace("Cylinder" , "yETC_Vaccum On " );
                         break;
                     }
                     break;
            case  3: if(g_lOutputXIo[yETC_AirBlower1]){
                         pPaix->SetOutputXBit(yETC_AirBlower1 , false);
                         Trace("Cylinder" , "yETC_AirBlower1 Off " );
                         break;
                     }
                     else {
                         pPaix->SetOutputXBit(yETC_AirBlower1 , true );
                         Trace("Cylinder" , "yETC_AirBlower1 On " );
                         break;
                     }
                     break;
            case  4: if(g_lOutputXIo[yETC_AirBlower2]){
                         pPaix->SetOutputXBit(yETC_AirBlower2 , false);
                         Trace("Cylinder" , "yETC_AirBlower2 Off " );
                         break;
                     }
                     else {
                         pPaix->SetOutputXBit(yETC_AirBlower2 , true );
                         Trace("Cylinder" , "yETC_AirBlower2 On " );
                         break;
                     }
                     break;
            case  5: if(g_lOutputXIo[yETC_SupplyValve]){
                         pPaix->SetOutputXBit(yETC_SupplyValve, false);
                         Trace("Cylinder" , "yETC_SupplyValve Off " );
                         break;
                     }
                     else {
                         pPaix->SetOutputXBit(yETC_SupplyValve, true );
                         Trace("Cylinder" , "yETC_SupplyValve On " );
                         break;
                     }
                     break;
            case  6: if(g_lOutputXIo[yETC_Drainage]){
                         pPaix->SetOutputXBit(yETC_Drainage   , false);
                         Trace("Cylinder" , "yETC_Drainage Off " );
                         break;
                     }
                     else {
                         pPaix->SetOutputXBit(yETC_Drainage   , true );
                         Trace("Cylinder" , "yETC_Drainage On " );
                         break;
                     }
                     break;
        }
    }
    else {
       FrmMsgOk->MsgOk("Error" , "장비가 동작 중입니다." );
       Trace("Error" , "장비가 동작 중입니다." );
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmValve::tmIOStatusTimer(TObject *Sender)
{
    tmIOStatus -> Enabled = false;

    pPaix->GetInputX(g_lInputXIo);
    pPaix->GetOutputX(g_lOutputXIo);

    if(g_lInputXIo[xETC_DoorFw]      ){btDoor    -> Caption = "Up "; btDoor   ->Color=clLime ;}
    else if(g_lInputXIo[xETC_DoorBw] ){btDoor    -> Caption = "Dn "; btDoor   ->Color=clBlack;}
    if(g_lOutputXIo[yETC_Vaccum]     ){btVacm    -> Caption = "On "; btVacm   ->Color=clLime ;}
    else                              {btVacm    -> Caption = "Off"; btVacm   ->Color=clBlack;}
    if(g_lOutputXIo[yETC_AirBlower1] ){btAir1    -> Caption = "On "; btAir1   ->Color=clLime ;}
    else                              {btAir1    -> Caption = "Off"; btAir1   ->Color=clBlack;}
    if(g_lOutputXIo[yETC_AirBlower2] ){btAir2    -> Caption = "On "; btAir2   ->Color=clLime ;}
    else                              {btAir2    -> Caption = "Off"; btAir2   ->Color=clBlack;}
    if(g_lOutputXIo[yETC_SupplyValve]){btWtSply  -> Caption = "On "; btWtSply ->Color=clLime ;}
    else                              {btWtSply  -> Caption = "Off"; btWtSply ->Color=clBlack;}
    if(g_lOutputXIo[yETC_Drainage   ]){btWtDrain -> Caption = "On "; btWtDrain->Color=clLime ;}
    else                              {btWtDrain -> Caption = "Off"; btWtDrain->Color=clBlack;}

    tmIOStatus -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmValve::btDoorMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iBtNo = ((TPanel *)Sender) -> Tag;

    switch(iBtNo){
        case 1: btDoor    -> BevelOuter = bvLowered; break;
        case 2: btVacm    -> BevelOuter = bvLowered; break;
        case 3: btAir1    -> BevelOuter = bvLowered; break;
        case 4: btAir2    -> BevelOuter = bvLowered; break;
        case 5: btWtSply  -> BevelOuter = bvLowered; break;
        case 6: btWtDrain -> BevelOuter = bvLowered; break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmValve::btDoorMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btDoor    -> BevelOuter = bvRaised;
    btVacm    -> BevelOuter = bvRaised;
    btAir1    -> BevelOuter = bvRaised;
    btAir2    -> BevelOuter = bvRaised;
    btWtSply  -> BevelOuter = bvRaised;
    btWtDrain -> BevelOuter = bvRaised;
}
//---------------------------------------------------------------------------

