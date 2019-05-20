//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "FrontFeed.h"
#include "FrontLaser.h"
#include "LearFeed.h"
#include "LearLaser.h"

//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "LaserConUnit.h"
//    sdfsdfsdfsdfsdfsdfsdf
#include "VisnDefine.h"
#include "SAInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{

}

void __fastcall TFrmDeviceSet::FormCreate(TObject *Sender)
{
    TPanel * pMotrPos ;
    TPanel * pMotrJog ;
    AnsiString sMotrPanel ;
    for(int i = 0 ; i < MAX_MOTR ; i++) {
        //모터포지션창.
        sMotrPanel = "pnMotrPos" + AnsiString(i) ;
        pMotrPos = ((TPanel*)FindComponent(sMotrPanel)) ;
        if(pMotrPos != NULL) PM.SetWindow(pMotrPos , i);

        //모터
        sMotrPanel = "pnMotrJog" + AnsiString(i) ;
        pMotrJog = ((TPanel*)FindComponent(sMotrPanel)) ;
        if(pMotrJog != NULL) {
            FraMotor[i] = new TFraMotr(pMotrJog);
            FraMotor[i] -> SetIdType(i ,MT_GetDirType(i));
            FraMotor[i] -> Parent = pMotrJog  ;
            FraMotor[i] -> Show();
            FraMotor[i] -> SetUnit(utJog ,0 );
        }
    }

    TPanel * pActr ;

    for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

             if(i == aiFED_Covr_FwBw)FraCylinder[i] ->SetConfig(i, AT_GetName(i) , AT_GetDirType(i) , pActr , FFD.CheckSafe );  //
        else if(i == aiFED_Algn_FwBw)FraCylinder[i] ->SetConfig(i, AT_GetName(i) , AT_GetDirType(i) , pActr , LFD.CheckSafe );  //
        else if(i == aiLSR_StgF_FwBw){
            FraCylinder[i] ->SetConfig(i, AT_GetName(i) , AT_GetDirType(i) , pActr , FFD.CheckSafe );
            pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i+2))) ;
            FraCylinder[i+2] = new TFraCylOneBt(pActr);
            FraCylinder[i+2] ->SetConfig(i, AT_GetName(i) , AT_GetDirType(i) , pActr , FFD.CheckSafe );
        }
        else if(i == aiLSR_StgL_FwBw){
            FraCylinder[i] ->SetConfig(i, AT_GetName(i) , AT_GetDirType(i) , pActr , LFD.CheckSafe );
            pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i+2))) ;
            FraCylinder[i+2] = new TFraCylOneBt(pActr);
            FraCylinder[i+2] ->SetConfig(i, AT_GetName(i) , AT_GetDirType(i) , pActr , LFD.CheckSafe );
        }

    }

    foFeedAirBlow1= new TFraOutput(pnFED1); foFeedAirBlow1-> SetConfig(yFED_AirBlow1,IO_GetYName(yFED_AirBlow1),pnFED1);
    foFeedAirBlow2= new TFraOutput(pnFED2); foFeedAirBlow2-> SetConfig(yFED_AirBlow2,IO_GetYName(yFED_AirBlow2),pnFED2);
    foFeedVaccum1 = new TFraOutput(pnFED3); foFeedVaccum1 -> SetConfig(yFED_Vcc1    ,IO_GetYName(yFED_Vcc1    ),pnFED3);
    foFeedVaccum2 = new TFraOutput(pnFED4); foFeedVaccum2 -> SetConfig(yFED_Vcc2    ,IO_GetYName(yFED_Vcc2    ),pnFED4);
    foBawlFeeder  = new TFraOutput(pnFED5); foBawlFeeder  -> SetConfig(yETC_Feeder  ,IO_GetYName(yETC_Feeder  ),pnFED5);

    foFrontSTGVcc = new TFraOutput(pnLSR1); foFrontSTGVcc -> SetConfig(yLSR_VccF    ,IO_GetYName(yLSR_VccF    ),pnLSR1);
    foLearSTGVcc  = new TFraOutput(pnLSR2); foLearSTGVcc  -> SetConfig(yLSR_VccL    ,IO_GetYName(yLSR_VccL    ),pnLSR2);
    foFrontSTGEjt = new TFraOutput(pnLSR3); foFrontSTGEjt -> SetConfig(yLSR_EjctF   ,IO_GetYName(yLSR_EjctF   ),pnLSR3);
    foLearSTGEjt  = new TFraOutput(pnLSR4); foLearSTGEjt  -> SetConfig(yLSR_EjctL   ,IO_GetYName(yLSR_EjctL   ),pnLSR4);


    
    
    
    

    
    
    















    pcDeviceSet -> ActivePageIndex = 0 ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::EditEnable()
{

    if(!OM.DevOptn.bUseLsrPst1){ cbUsedLsrPos1 -> Checked = false; }
    else                       { cbUsedLsrPos1 -> Checked = true ; }
    if(!OM.DevOptn.bUseLsrPst2){ cbUsedLsrPos2 -> Checked = false; }
    else                       { cbUsedLsrPos2 -> Checked = true ; }
    if(!OM.DevOptn.bUseLsrPst3){ cbUsedLsrPos3 -> Checked = false; }
    else                       { cbUsedLsrPos3 -> Checked = true ; }
    if(!OM.DevOptn.bUseLsrPst4){ cbUsedLsrPos4 -> Checked = false; }
    else                       { cbUsedLsrPos4 -> Checked = true ; }
    if(!OM.DevOptn.bUseLsrPst5){ cbUsedLsrPos5 -> Checked = false; }
    else                       { cbUsedLsrPos5 -> Checked = true ; }

    return;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;                  
    pcDeviceSet -> Hide();
    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    PM.Init();
    UpdateDevOptn(toTabl);
    PM.UpdatePstn(toTabl);
    EditEnable();
    pcDeviceSet -> Show();
    DM.ARAY[riFFD].SetMaxColRow(OM.MstOptn.iPkgCnt , 1);
    DM.ARAY[riFLS].SetMaxColRow(OM.MstOptn.iPkgCnt , 1);
    DM.ARAY[riLFD].SetMaxColRow(OM.MstOptn.iPkgCnt , 1);
    DM.ARAY[riLLS].SetMaxColRow(OM.MstOptn.iPkgCnt , 1);

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer -> Enabled = false;
    btSaveDevice -> Enabled = !SEQ._bRun ;
    //switch ( pcDeviceSet -> ActivePageIndex ) {
    //    default: break;
    //    case 0 : break;
    //    case 1 : break;
    //    case 2 : break;
    //    case 3 : break;
    //    case 4 : break;
    //    case 5 : break;
    //    case 6 : break;
    //    case 7 : break;
    //    case 8 : break;
    //    case 9 : break;
    //}

    //T축 야메 포지션 디스프레이.





    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {

        edFrntLsrNo1  -> Text    = OM.DevOptn.iFrtLsrNo1  ;
        edFrntLsrNo2  -> Text    = OM.DevOptn.iFrtLsrNo2  ;
        edFrntLsrNo3  -> Text    = OM.DevOptn.iFrtLsrNo3  ;
        edFrntLsrNo4  -> Text    = OM.DevOptn.iFrtLsrNo4  ;
        edFrntLsrNo5  -> Text    = OM.DevOptn.iFrtLsrNo5  ;

        edBackLsrNo1  -> Text    = OM.DevOptn.iLerLsrNo1  ;
        edBackLsrNo2  -> Text    = OM.DevOptn.iLerLsrNo2  ;
        edBackLsrNo3  -> Text    = OM.DevOptn.iLerLsrNo3  ;
        edBackLsrNo4  -> Text    = OM.DevOptn.iLerLsrNo4  ;
        edBackLsrNo5  -> Text    = OM.DevOptn.iLerLsrNo5  ;

        cbUsedLsrPos1 -> Checked = OM.DevOptn.bUseLsrPst1 ;
        cbUsedLsrPos2 -> Checked = OM.DevOptn.bUseLsrPst2 ;
        cbUsedLsrPos3 -> Checked = OM.DevOptn.bUseLsrPst3 ;
        cbUsedLsrPos4 -> Checked = OM.DevOptn.bUseLsrPst4 ;
        cbUsedLsrPos5 -> Checked = OM.DevOptn.bUseLsrPst5 ;
    }
    else
    {

        OM.DevOptn.iFrtLsrNo1  = StrToFloatDef(edFrntLsrNo1  -> Text,0);
        OM.DevOptn.iFrtLsrNo2  = StrToFloatDef(edFrntLsrNo2  -> Text,0);
        OM.DevOptn.iFrtLsrNo3  = StrToFloatDef(edFrntLsrNo3  -> Text,0);
        OM.DevOptn.iFrtLsrNo4  = StrToFloatDef(edFrntLsrNo4  -> Text,0);
        OM.DevOptn.iFrtLsrNo5  = StrToFloatDef(edFrntLsrNo5  -> Text,0);

        OM.DevOptn.iLerLsrNo1  = StrToFloatDef(edBackLsrNo1  -> Text,0);
        OM.DevOptn.iLerLsrNo2  = StrToFloatDef(edBackLsrNo2  -> Text,0);
        OM.DevOptn.iLerLsrNo3  = StrToFloatDef(edBackLsrNo3  -> Text,0);
        OM.DevOptn.iLerLsrNo4  = StrToFloatDef(edBackLsrNo4  -> Text,0);
        OM.DevOptn.iLerLsrNo5  = StrToFloatDef(edBackLsrNo5  -> Text,0);

        OM.DevOptn.bUseLsrPst1 = cbUsedLsrPos1 -> Checked;
        OM.DevOptn.bUseLsrPst2 = cbUsedLsrPos2 -> Checked;
        OM.DevOptn.bUseLsrPst3 = cbUsedLsrPos3 -> Checked;
        OM.DevOptn.bUseLsrPst4 = cbUsedLsrPos4 -> Checked;
        OM.DevOptn.bUseLsrPst5 = cbUsedLsrPos5 -> Checked;

    }
    EditEnable();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    EditEnable();

//    foFeedAirBlow1-> SetSize();
//    foFeedAirBlow2-> SetSize();
//    foFeedVaccum1 -> SetSize();
//    foFeedVaccum2 -> SetSize();
//    foBawlFeeder  -> SetSize();
//
//    foFrontSTGVcc -> SetSize();
//    foLearSTGVcc  -> SetSize();
//    foFrontSTGEjt -> SetSize();
//    foLearSTGEjt  -> SetSize();
//
    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default:                                break;
        case 0 : pnJogUnit -> Parent = pnJog1 ; break;
        case 1 : pnJogUnit -> Parent = pnJog2 ; break;
        case 2 :                                break;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::rgJogUnit1Click(TObject *Sender)
{
    for(int i=0;i<MAX_MOTR;i++) {
        if(rgJogUnit1->ItemIndex == 0) FraMotor[i]->SetUnit(utJog ,0   );
        if(rgJogUnit1->ItemIndex == 1) FraMotor[i]->SetUnit(utMove,1   );
        if(rgJogUnit1->ItemIndex == 2) FraMotor[i]->SetUnit(utMove,0.5 );
        if(rgJogUnit1->ItemIndex == 3) FraMotor[i]->SetUnit(utMove,0.1 );
        if(rgJogUnit1->ItemIndex == 4) FraMotor[i]->SetUnit(utMove,0.05);
        if(rgJogUnit1->ItemIndex == 5) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    }
}
//---------------------------------------------------------------------------










void __fastcall TFrmDeviceSet::btSendClick(TObject *Sender)
{
    LCU.SendMsg(edSendGA -> Text) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btDustSuckClick(TObject *Sender)
{
    IO_SetY(yETC_DustSuck , !IO_GetY(yETC_DustSuck));
}
//---------------------------------------------------------------------------


