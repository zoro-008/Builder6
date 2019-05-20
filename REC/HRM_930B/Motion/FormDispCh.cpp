//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormDispCh.h"
#include "OptionMan.h"
#include "SMInterfaceUnit.h"


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
void TFrmDispCh::Show(CRs232Dispr  * _pRs232_Dispr)
{
    m_pRs232_Dispr = _pRs232_Dispr ;

    if(_pRs232_Dispr == &Rs232_DisprFt){
        m_iActiveCh = OM.DevOptn.iDspChFt ;
    }
    else {
        m_iActiveCh = OM.DevOptn.iDspChRr ;
    }

    ShowModal();

    //TCustomForm::Show();

    //Show();
}

void __fastcall TFrmDispCh::FormShow(TObject *Sender)
{
    this -> Caption = String(m_iActiveCh+1) + "Ch Setting" ;

    UpdateDispData(true);

    tmUpdate -> Enabled = true ;

}
//---------------------------------------------------------------------------
void  TFrmDispCh::UpdateDispData(bool _toTable)
{
    double dDspVacPres ;
    double dDspPrsPres ;


    if(_toTable) {
        m_pRs232_Dispr->GetDispData(m_iActiveCh+1);
        CDelayTimer TimeOut;
        TimeOut.Clear();
        while(!m_pRs232_Dispr->GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
            Sleep(1);
            if(TimeOut.OnDelay(true , 3000)){
                break ;
            }
        }
      //cbDspCh              -> ItemIndex = OM.DevOptn.iDspCh            ;
        edDspVacPres   -> Text    = m_pRs232_Dispr->GetDispData().dVacPres   ;//OM.DevOptn.dDspVacPres       ;
        edDspPrsPres   -> Text    = m_pRs232_Dispr->GetDispData().dPrsPres   ;//OM.DevOptn.dDspPrsPres       ;
    }
    else { //ToControllor.
        //OM.DevOptn.dDspVacPres       = StrToFloatDef  (edDspVacPres       -> Text , OM.DevOptn.dDspVacPres     ) ;
        //if( OM.DevOptn.dDspVacPres < 0) {
        //    OM.DevOptn.dDspVacPres = 0;
        //}
        //if( OM.DevOptn.dDspVacPres > 20) {
        //    OM.DevOptn.dDspVacPres = 20;
        //}
        //OM.DevOptn.dDspPrsPres       = StrToFloatDef  (edDspPrsPres       -> Text , OM.DevOptn.dDspPrsPres     ) ;
        //if( OM.DevOptn.dDspPrsPres < 30) {
        //    OM.DevOptn.dDspPrsPres = 30;
        //}
        //if( OM.DevOptn.dDspPrsPres > 400) {
        //    OM.DevOptn.dDspPrsPres = 400;
        //}

        dDspVacPres = StrToFloatDef(edDspVacPres -> Text , 1  ) ;
        dDspPrsPres = StrToFloatDef(edDspPrsPres -> Text , 30 ) ;

        m_pRs232_Dispr->SetPTV(dDspPrsPres , 10 , dDspVacPres);
        CDelayTimer TimeOut;
        TimeOut.Clear();
        while(!m_pRs232_Dispr->GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
            Sleep(1);
            if(TimeOut.OnDelay(true , 1000)){
                break ;
            }
        }

        UpdateDispData(true);
    }
}

void __fastcall TFrmDispCh::tmUpdateTimer(TObject *Sender)
{
    edDspPrsPres -> Enabled = !m_pRs232_Dispr->GetDispData().bSigmaMode ;
    edDspPrsPres -> Color   = !m_pRs232_Dispr->GetDispData().bSigmaMode ? clWindow : clMedGray ;
    edDspVacPres -> Enabled = !m_pRs232_Dispr->GetDispData().bSigmaMode ;
    edDspVacPres -> Color   = !m_pRs232_Dispr->GetDispData().bSigmaMode ? clWindow : clMedGray ;
    btSetDspData -> Enabled = !m_pRs232_Dispr->GetDispData().bSigmaMode ;

    btSigmaMode -> Font -> Color = m_pRs232_Dispr->GetDispData().bSigmaMode ? clLime : clBlack ;
    btSigmaMode -> Caption       = m_pRs232_Dispr->GetDispData().bSigmaMode ? "SIGMA ON" : "SIGMA OFF" ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmDispCh::btSetDspDataClick(TObject *Sender)
{
    UpdateDispData(false);
}
//---------------------------------------------------------------------------


void __fastcall TFrmDispCh::btSigmaModeClick(TObject *Sender)
{
    if(!m_pRs232_Dispr->GetDispData().bSigmaMode){
        FM_MsgOkModal("시그마모드 진입","최고용량 실린지로 교체해주세요.");
        m_pRs232_Dispr->SetSigmaModeOn();
        Sleep(20000); //바로 클릭 못하게 그냥 화면 잡고 있자.
    }
    else {
        FM_MsgOkModal("시그마모드 해제","시그마 모드를 해제 합니다.");
        m_pRs232_Dispr->SetSigmaModeOff(m_iActiveCh+1);
        Sleep(500);

    }
    CDelayTimer TimeOut ;
    TimeOut.Clear();
    while(!m_pRs232_Dispr->GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
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
    m_pRs232_Dispr->GetSylVolm(m_iActiveCh+1);

    CDelayTimer TimeOut ;
    TimeOut.Clear();
    while(!m_pRs232_Dispr->GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        Sleep(1);
        if(TimeOut.OnDelay(true , 3000)){
            break ;
        }
    }

    lbAmount -> Caption = String(m_pRs232_Dispr->GetSylFill()) + "%" ;
}
//---------------------------------------------------------------------------

