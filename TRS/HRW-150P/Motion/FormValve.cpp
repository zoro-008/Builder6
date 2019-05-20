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
#include "OptionMan.h"
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
void __fastcall TFrmValve::btDoorUpClick(TObject *Sender)
{
    if(SEQ -> m_bRun == false) {

        int iBtNo = ((TPanel *)Sender) -> Tag;

        if(!g_lInputXIo[xETC_DoorSnsrNotDetect]){
            FrmMsgOk->MsgOk("Error","Door Sensor가 감지되었습니다.");
            Trace("Error","Door Sensor가 감지되었습니다.");
            return;
        }

        //pPaix -> GetOutputX(g_lOutputXIo);
        switch(iBtNo){
            case  1: pPaix -> SetOutputXBit(yETC_DoorFw , true );
                     pPaix -> SetOutputXBit(yETC_DoorBw , false);
                     break;
            case  2: pPaix -> SetOutputXBit(yETC_DoorFw , false);
                     pPaix -> SetOutputXBit(yETC_DoorBw , true );
                     break;

            case  3: if(g_lOutputXIo[yETC_BNRAir    ]) pPaix->SetOutputXBit(yETC_BNRAir     , false);
                     else                              pPaix->SetOutputXBit(yETC_BNRAir     , true );
                     break;

            //옵션 추가. JS
            case  4: if(OM.MstOptn.bUsedFrstMachine){
                         if(!g_lOutputXIo[yETC_PMPAir1  ]) pPaix->SetOutputXBit(yETC_PMPAir1   , true );
                         else                              pPaix->SetOutputXBit(yETC_PMPAir1   , false);
                         break;
                     }
                     else {
                         if(!g_lOutputXIo[yETC_PMPAir1  ]){pPaix->SetOutputXBit(yETC_PMPAir1   , true  ); pPaix->SetOutputXBit(yETC_PMPAir2 , false);}
                         else                             {pPaix->SetOutputXBit(yETC_PMPAir1   , false ); pPaix->SetOutputXBit(yETC_PMPAir2 , true );}
                         break;
                     }

            case  5: if(g_lOutputXIo[yETC_IONAir    ]) pPaix->SetOutputXBit(yETC_IONAir     , false);
                     else                              pPaix->SetOutputXBit(yETC_IONAir     , true );
                     break;

            case  6: if(g_lOutputXIo[yETC_BNRValve  ]) pPaix->SetOutputXBit(yETC_BNRValve   , false);
                     else                              pPaix->SetOutputXBit(yETC_BNRValve   , true );
                     break;

            case  7: if(g_lOutputXIo[yETC_BTMValve  ]) pPaix->SetOutputXBit(yETC_BTMValve   , false);
                     else                              pPaix->SetOutputXBit(yETC_BTMValve   , true );
                     break;

            case  8: if(g_lOutputXIo[yETC_DrainValve]) pPaix->SetOutputXBit(yETC_DrainValve , false);
                     else                              pPaix->SetOutputXBit(yETC_DrainValve , true );
                     break;


            case  9: if(!OM.MstOptn.bUsedFrstMachine){ pPaix->SetOutputXBit(yETC_MskCycleFwBw , true ); break; }
                     else                            {                                                  break; }

            case 10: if(!OM.MstOptn.bUsedFrstMachine){ pPaix->SetOutputXBit(yETC_MskCycleFwBw , false); break; }
                     else                            {                                                  break; }


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

    if(g_lInputXIo[xETC_DoorFw     ]) {btDoorUp  -> Caption = "Up "; btDoorUp  ->Color=clLime ;}
    else                              {btDoorUp  -> Caption = "Up "; btDoorUp  ->Color=clBlack;}

    if(g_lInputXIo[xETC_DoorBw     ]) {btDoorDn  -> Caption = "Dn "; btDoorDn  ->Color=clLime ;}
    else                              {btDoorDn  -> Caption = "Dn "; btDoorDn  ->Color=clBlack;}

    if(g_lOutputXIo[yETC_BNRAir    ]) {btBNRAir  -> Caption = "On "; btBNRAir  ->Color=clLime ;}
    else                              {btBNRAir  -> Caption = "Off"; btBNRAir  ->Color=clBlack;}

    if(!OM.MstOptn.bUsedFrstMachine){
        if(g_lOutputXIo[yETC_PMPAir1 ]) {btPMPAir  -> Caption = "On "; btPMPAir  ->Color=clLime ;}
        else                            {btPMPAir  -> Caption = "Off"; btPMPAir  ->Color=clBlack;}
    }

    if(g_lOutputXIo[yETC_PMPAir1 ]) {btPMPAir  -> Caption = "On "; btPMPAir  ->Color=clLime ;}
    else                            {btPMPAir  -> Caption = "Off"; btPMPAir  ->Color=clBlack;}

    if(g_lOutputXIo[yETC_IONAir    ]) {btIONAir  -> Caption = "On "; btIONAir  ->Color=clLime ;}
    else                              {btIONAir  -> Caption = "Off"; btIONAir  ->Color=clBlack;}

    if(g_lOutputXIo[yETC_BNRValve  ]) {btWtBNR   -> Caption = "On "; btWtBNR   ->Color=clLime ;}
    else                              {btWtBNR   -> Caption = "Off"; btWtBNR   ->Color=clBlack;}

    if(g_lOutputXIo[yETC_DrainValve]) {btWtDrain -> Caption = "On "; btWtDrain ->Color=clLime ;}
    else                              {btWtDrain -> Caption = "Off"; btWtDrain ->Color=clBlack;}

    if(g_lOutputXIo[yETC_BTMValve  ]) {btWtPMP   -> Caption = "On "; btWtPMP   ->Color=clLime ;}
    else                              {btWtPMP   -> Caption = "Off"; btWtPMP   ->Color=clBlack;}

    if(g_lInputXIo[xETC_MskDetectBw]) {btMskBwd  ->Color=clLime   ; btMskFwd  ->Color=clBlack ;}
    else                              {btMskBwd  ->Color=clBlack  ; btMskFwd  ->Color=clLime  ;}

    tmIOStatus -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmValve::btDoorUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iBtNo = ((TPanel *)Sender) -> Tag;

    switch(iBtNo){
        case 1: btDoorUp  -> BevelOuter = bvLowered; break;
        case 2: btDoorDn  -> BevelOuter = bvLowered; break;
        case 3: btBNRAir  -> BevelOuter = bvLowered; break;
        case 4: btPMPAir  -> BevelOuter = bvLowered; break;
        case 5: btIONAir  -> BevelOuter = bvLowered; break;
        case 6: btWtBNR   -> BevelOuter = bvLowered; break;
        case 7: btWtPMP   -> BevelOuter = bvLowered; break;
        case 8: btWtDrain -> BevelOuter = bvLowered; break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmValve::btDoorUpMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btDoorUp   -> BevelOuter = bvRaised;
    btDoorDn   -> BevelOuter = bvRaised;
    btBNRAir   -> BevelOuter = bvRaised;
    btPMPAir   -> BevelOuter = bvRaised;
    btIONAir   -> BevelOuter = bvRaised;
    btWtBNR    -> BevelOuter = bvRaised;
    btWtPMP    -> BevelOuter = bvRaised;
    btWtDrain  -> BevelOuter = bvRaised;

}
//---------------------------------------------------------------------------

