//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormDispCh.h"
#include "OptionMan.h"
#include "SMInterfaceUnit.h"

#include "Dispensor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDispCh *FrmDispCh;
//---------------------------------------------------------------------------
__fastcall TFrmDispCh::TFrmDispCh(TComponent* Owner)
        : TForm(Owner)
{
    
}
//---------------------------------------------------------------------------

void __fastcall TFrmDispCh::FormShow(TObject *Sender)
{
    this -> Caption = String(OM.DevOptn.iDspCh+1) + "Ch Setting" ;

    UpdateDispData(true);

    tmUpdate -> Enabled = true ;





/*




        //해당 채널로 세팅.
        DSP.Rs232_Dispr.SetLoadCh(OM.DevOptn.iDspCh+1);
        CDelayTimer TimeOut;
        TimeOut.Clear();
        while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
            Sleep(1);
            if(TimeOut.OnDelay(true , 1000)){
                break ;
            }
        }

        DSP.Rs232_Dispr.GetDispData(OM.DevOptn.iDspCh+1);
        TimeOut.Clear();
        while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
            Sleep(1);
            if(TimeOut.OnDelay(true , 1000)){
                break ;
            }
        }
        //cbDspCh              -> ItemIndex = OM.DevOptn.iDspCh            ;
        cbDspSigma   -> Checked = DSP.Rs232_Dispr.GetDispData().bSigmaMode ;//OM.DevOptn.dDspVacPres       ;
        edDspVacPres -> Text    = DSP.Rs232_Dispr.GetDispData().dVacPres   ;//OM.DevOptn.dDspVacPres       ;
        edDspPrsPres -> Text    = DSP.Rs232_Dispr.GetDispData().dPrsPres   ;//OM.DevOptn.dDspPrsPres       ;




        edDspVacPres         -> Text    = OM.DevOptn.dDspVacPres       ;
        edDspPrsPres         -> Text    = OM.DevOptn.dDspPrsPres       ;
 */


}
//---------------------------------------------------------------------------
void  TFrmDispCh::UpdateDispData(bool _toTable)
{
    if(_toTable) {
        DSP.Rs232_Dispr.GetDispData(OM.DevOptn.iDspCh+1);
        CDelayTimer TimeOut;
        TimeOut.Clear();
        while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
            Sleep(1);
            if(TimeOut.OnDelay(true , 3000)){
                break ;
            }
        }
        OM.DevOptn.dDspVacPres = DSP.Rs232_Dispr.GetDispData().dVacPres ;
        OM.DevOptn.dDspPrsPres = DSP.Rs232_Dispr.GetDispData().dPrsPres ;
        //cbDspCh              -> ItemIndex = OM.DevOptn.iDspCh            ;
        edDspVacPres   -> Text    = OM.DevOptn.dDspVacPres   ;//
        edDspPrsPres   -> Text    = OM.DevOptn.dDspPrsPres   ;//




    }
    else { //ToControllor.
        OM.DevOptn.dDspVacPres       = StrToFloatDef  (edDspVacPres       -> Text , OM.DevOptn.dDspVacPres     ) ;
        if( OM.DevOptn.dDspVacPres < 0) {
            OM.DevOptn.dDspVacPres = 0;
        }
        if( OM.DevOptn.dDspVacPres > 20) {
            OM.DevOptn.dDspVacPres = 20;
        }
        OM.DevOptn.dDspPrsPres       = StrToFloatDef  (edDspPrsPres       -> Text , OM.DevOptn.dDspPrsPres     ) ;
        if( OM.DevOptn.dDspPrsPres < 30) {
            OM.DevOptn.dDspPrsPres = 30;
        }
        if( OM.DevOptn.dDspPrsPres > 400) {
            OM.DevOptn.dDspPrsPres = 400;
        }

        DSP.Rs232_Dispr.SetPTV(OM.DevOptn.dDspPrsPres , 10 , OM.DevOptn.dDspVacPres);
        CDelayTimer TimeOut;
        TimeOut.Clear();
        while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
            Sleep(1);
            if(TimeOut.OnDelay(true , 1000)){
                break ;
            }
        }

        UpdateDispData(true);
    }

    OM.SaveDevOptn(OM.GetCrntDev());
}

void __fastcall TFrmDispCh::tmUpdateTimer(TObject *Sender)
{
    edDspPrsPres -> Enabled = !DSP.Rs232_Dispr.GetDispData().bSigmaMode ;
    edDspPrsPres -> Color   = !DSP.Rs232_Dispr.GetDispData().bSigmaMode ? clWindow : clMedGray ;
    edDspVacPres -> Enabled = !DSP.Rs232_Dispr.GetDispData().bSigmaMode ;
    edDspVacPres -> Color   = !DSP.Rs232_Dispr.GetDispData().bSigmaMode ? clWindow : clMedGray ;
    btSetDspData -> Enabled = !DSP.Rs232_Dispr.GetDispData().bSigmaMode ;

    btSigmaMode -> Font -> Color = DSP.Rs232_Dispr.GetDispData().bSigmaMode ? clLime : clBlack ;
    btSigmaMode -> Caption       = DSP.Rs232_Dispr.GetDispData().bSigmaMode ? "SIGMA ON" : "SIGMA OFF" ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmDispCh::btSetDspDataClick(TObject *Sender)
{
    UpdateDispData(false);
}
//---------------------------------------------------------------------------


void __fastcall TFrmDispCh::btSigmaModeClick(TObject *Sender)
{
    if(!DSP.Rs232_Dispr.GetDispData().bSigmaMode){
        FM_MsgOkModal("시그마모드 진입","최고용량 실린지로 교체해주세요.");
        DSP.Rs232_Dispr.SetSigmaModeOn();
        Sleep(20000); //바로 클릭 못하게 그냥 화면 잡고 있자.
    }
    else {
        FM_MsgOkModal("시그마모드 해제","시그마 모드를 해제 합니다.");
        DSP.Rs232_Dispr.SetSigmaModeOff(OM.DevOptn.iDspCh+1);
        Sleep(500);

    }
    CDelayTimer TimeOut ;
    TimeOut.Clear();
    while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 3000)){
            break ;
        }
    }



    UpdateDispData(true);


}
//---------------------------------------------------------------------------

void __fastcall TFrmDispCh::btCheckAmountClick(TObject *Sender)
{
    DSP.Rs232_Dispr.GetSylVolm(OM.DevOptn.iDspCh+1);
    CDelayTimer TimeOut ;
    TimeOut.Clear();
    while(!DSP.Rs232_Dispr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 3000)){
            break ;
        }
    }

    lbAmount -> Caption = String(DSP.Rs232_Dispr.GetSylFill()) + "%" ;
}
//---------------------------------------------------------------------------

