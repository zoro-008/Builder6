//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "FormValve.h"
#include "Timer.h"
#include "global.h"
#include "SEQUnit.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
        : TForm(Owner)
{
    m_pOnimg  = new Graphics::TBitmap() ;
    m_pOffimg = new Graphics::TBitmap() ;

    ImageList1 -> GetBitmap(0,m_pOffimg); //레프트 라이트 업 다운 레프트R 라이트R
    ImageList1 -> GetBitmap(1,m_pOnimg );
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btIncMinus1Click(TObject *Sender)
{
    if( pPaix == NULL ) return;
    double fdist;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;
    double dPulse;

    if(nAxis == miSpinMt) dPulse = edOut1 -> Text.ToDouble();
    if(nAxis == miTiltMt) dPulse = edOut2 -> Text.ToDouble();

    pPaix -> IncMove(nAxis, -dPulse);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btIncPlus1Click(TObject *Sender)
{
    if( pPaix == NULL ) return;
    double fdist;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;
    double dPulse;

    if(nAxis == miSpinMt) dPulse = edOut1 -> Text.ToDouble();
    if(nAxis == miTiltMt) dPulse = edOut2 -> Text.ToDouble();

    pPaix -> IncMove(nAxis, dPulse);

}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btAbsMinus1Click(TObject *Sender)
{
    if( pPaix == NULL ) return;
    double fdist;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;
    double dPulse ;

    if(nAxis == miSpinMt) dPulse = edOut1 -> Text.ToDouble();
    if(nAxis == miTiltMt) dPulse = edOut2 -> Text.ToDouble();

    pPaix -> AbsMove(nAxis, -dPulse);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btAbsPlus1Click(TObject *Sender)
{
    if( pPaix == NULL ) return;
    double fdist;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;
    double dPulse ;

    if(nAxis == miSpinMt) dPulse = edOut1 -> Text.ToDouble();
    if(nAxis == miTiltMt) dPulse = edOut2 -> Text.ToDouble();

    pPaix -> AbsMove(nAxis, dPulse);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btStop1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    if(cbStop->Checked)pPaix->DecStop(nAxis);
    else               pPaix->SuddenStop(nAxis);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btJogCCw1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if( pPaix == NULL )        return;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    pPaix->JogMove(nAxis,CCW);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btJogCw1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if( pPaix == NULL )        return;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    pPaix->JogMove(nAxis,CW);

}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btJogCCw1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if( pPaix == NULL )        return;
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    if(cbStop->Checked) pPaix->DecStop   (nAxis);
    else                pPaix->SuddenStop(nAxis);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btCurrent1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    pPaix->SetCurrentOn(nAxis,!g_NmcMotionOut.nCurrentOn[nAxis]);
}
//--------------------------------------------------------------------------
void __fastcall TFrmMaster::GetNmcMotionOut()
{
	pPaix->GetNmcMotionOut( &g_NmcMotionOut);
        SetOutStatus();
}
//-------------------------------------------------------------------------
void __fastcall TFrmMaster::SetMLimit( int nAxis, bool bOn)
{
    if(nAxis == 0){pnHMLimit1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;}
    else          {pnHMLimit2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;}
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetPLimit( int nAxis, bool bOn)
{
    if(nAxis == 0){pnHPLimit1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;}
    else          {pnHPLimit2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;}
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetZ( int nAxis, bool bOn)
{
    if(nAxis == 0){pnEncZ1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;}
    else          {pnEncZ2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;}
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetNear( int nAxis, bool bOn)
{
    if(nAxis == 0){
        pnNear1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnNear2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetInpo( int nAxis, bool bOn)
{
    if(nAxis == 0){
        pnInpo1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnInpo2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetAlarm( int nAxis, bool bOn)
{
    if(nAxis == 0){
        pnAlarm1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnAlarm2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetBusy(int nAxis, bool bOn)
{
    if(nAxis == 0){
        pnBusy1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnBusy2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
}
//-------------------------------------------------------------------------
void __fastcall TFrmMaster::SetHome( int nAxis, bool bOn)
{
    if(nAxis == 0){
        pnHomeDone1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnHomeDone2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
}
//-------------------------------------------------------------------------
void __fastcall TFrmMaster::SetReady( int nAxis, bool bOn)
{
    if(nAxis == 0){
        pnSReady1 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnSReady2 -> Color = bOn?clRed:(TColor)0x00DDFBFF;
    }

}

//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetSwMinus(int nId, bool onoff)
{
    if(nId == 0){
        pnSMLimit1 -> Color = onoff?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnSMLimit2 -> Color = onoff?clRed:(TColor)0x00DDFBFF;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetSwPlus(int nId, bool onoff)
{
    if(nId == 0){
        pnSPLimit1 -> Color = onoff?clRed:(TColor)0x00DDFBFF;
    }
    else {
        pnSPLimit2 -> Color = onoff?clRed:(TColor)0x00DDFBFF;
    }
}
//-------------------------------------------------------------------------
void __fastcall TFrmMaster::SetOutStatus(void)
{
    if(g_NmcMotionOut.nCurrentOn[0])    {btCurrent1 -> Glyph = m_pOnimg ; btCurrent1 -> Caption = "On" ;}
    else                                {btCurrent1 -> Glyph = m_pOffimg; btCurrent1 -> Caption = "Off";}
    if(g_NmcMotionOut.nServoOn[0])      {btServo1   -> Glyph = m_pOnimg ; btServo1   -> Caption = "On" ;}
    else                                {btServo1   -> Glyph = m_pOffimg; btServo1   -> Caption = "Off";}
    if(g_NmcMotionOut.nAlarmResetOn[0]) {btAlReset1 -> Glyph = m_pOnimg ; btAlReset1 -> Caption = "On" ;}
    else                                {btAlReset1 -> Glyph = m_pOffimg; btAlReset1 -> Caption = "Off";}
    if(g_NmcMotionOut.nDCCOn[0])        {btDCC1     -> Glyph = m_pOnimg ; btDCC1     -> Caption = "On" ;}
    else                                {btDCC1     -> Glyph = m_pOffimg; btDCC1     -> Caption = "Off";}
    if(g_NmcMotionOut.nCurrentOn[1])    {btCurrent2 -> Glyph = m_pOnimg ; btCurrent2 -> Caption = "On" ;}
    else                                {btCurrent2 -> Glyph = m_pOffimg; btCurrent2 -> Caption = "Off";}
    if(g_NmcMotionOut.nServoOn[1])      {btServo2   -> Glyph = m_pOnimg ; btServo2   -> Caption = "On" ;}
    else                                {btServo2   -> Glyph = m_pOffimg; btServo2   -> Caption = "Off";}
    if(g_NmcMotionOut.nAlarmResetOn[1]) {btAlReset2 -> Glyph = m_pOnimg ; btAlReset2 -> Caption = "On" ;}
    else                                {btAlReset2 -> Glyph = m_pOffimg; btAlReset2 -> Caption = "Off";}
    if(g_NmcMotionOut.nDCCOn[1])        {btDCC2     -> Glyph = m_pOnimg ; btDCC2     -> Caption = "On" ;}
    else                                {btDCC2     -> Glyph = m_pOffimg; btDCC2     -> Caption = "Off";}
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btServo1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag;

    int nAxis = iBtNo -1;

    pPaix->SetServoOn(nAxis,!g_NmcMotionOut.nServoOn [nAxis]);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAlReset1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag;

    int nAxis = iBtNo -1;

    pPaix->SetAlarmResetOn(nAxis,!g_NmcMotionOut.nAlarmResetOn [nAxis]);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btDCC1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag;

    int nAxis = iBtNo -1;

    pPaix->SetDCCOn(nAxis,!g_NmcMotionOut.nDCCOn[nAxis]);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::GetSensorRead(TMessage msg)
{
	long pulse, enc;
	int i;

	switch(msg.WParam ) {
	    case PAIX_OUTSTATUS: SetOutStatus(); break;
	    case PAIX_SENSORREAD: {
                //pPaix->GetNmcStatus(&g_NmcData);

                pnEmg1->Color = g_NmcData.nEmer[0]?clRed:(TColor)0x00DDFBFF;

	        for( int i = 0; i < MAX_MOTR; i ++) {
                    SetMLimit (i, g_NmcData.nMLimit  [i]);
	            SetNear   (i, g_NmcData.nNear    [i]);
	            SetPLimit (i, g_NmcData.nPLimit  [i]);
	            SetBusy   (i, g_NmcData.nBusy    [i]);
	            SetZ      (i, g_NmcData.nEncZ    [i]);
	            SetInpo   (i, g_NmcData.nInpo    [i]);
	            SetAlarm  (i, g_NmcData.nAlarm   [i]);
	            SetReady  (i, g_NmcData.nSReady  [i]);
	            SetSwPlus (i, g_NmcData.nSwPLimit[i]);
	            SetSwMinus(i, g_NmcData.nSwMLimit[i]);
                    //SetHome(i,NmcData.nHome[i]);
                }
                SetHome(miTiltMt, pPaix -> GetHomeDone(miTiltMt));
                edCmdValue1->Text = pPaix->GetCmdPos(miTiltMt);
                edEncValue1->Text = pPaix->GetEncPos(miTiltMt);
                edCmdValue2->Text = pPaix->GetCmdPos(miSpinMt);//pPaix->GetCmdPos(miTiltMt);
                edEncValue2->Text = pPaix->GetEncPos(miSpinMt);//pPaix->GetEncPos(miTiltMt);
                edTrgValue2->Text = pPaix->m_dTiltTrgPos ;

            }
            //case PAIX_IO_OUTPUT_X:DisplayOutputX();
            //break;
            //case PAIX_IO_INPUT_X: DisplayInputX();
            //break;
        }
}
void __fastcall TFrmMaster::FormShow(TObject *Sender)
{
    pPaix->SetMainWindow(this->Handle);

    cbAxis0Limit -> Checked = false;
    cbAxis1Limit -> Checked = false;

    PARASPEED pSpeed;
    PARALOGIC NmcLogic;

    LoadCmnOption();
    LoadMstOption();

    int nRet = pPaix -> GetSpeed(0, &pSpeed);
    int iRatio1 = pPaix -> GetRatio(0);
    int iRatio2 = pPaix -> GetRatio(1);
    if(!nRet){
        edRatio1 -> Text = iRatio1 ;
        edStart1 -> Text = pSpeed.dStart;
        edAcc1   -> Text = pSpeed.dAcc  ;
        edDec1   -> Text = pSpeed.dDec  ;
        edMax1   -> Text = pSpeed.dDrive;
    }
    nRet = pPaix -> GetSpeed(1, &pSpeed);

    if(!nRet){
        edRatio2 -> Text = iRatio2 ;
        edStart2 -> Text = pSpeed.dStart;
        edAcc2   -> Text = pSpeed.dAcc  ;
        edDec2   -> Text = pSpeed.dDec  ;
        edMax2   -> Text = pSpeed.dDrive;
    }

    if(OM.CmnOptn.bUsedTiltLimit) cbAxis0Limit -> Checked = true ;
    else                          cbAxis0Limit -> Checked = false;
    if(OM.CmnOptn.bUsedSpinLimit) cbAxis1Limit -> Checked = true ;
    else                          cbAxis1Limit -> Checked = false;

    edAxis0MLimt -> Text = OM.CmnOptn.iTiltMLimit * ONEREVOLUTION_ANGLE / TILT_PULSE_PER_REV / TILT_REDUCTION;
    edAxis0PLimt -> Text = OM.CmnOptn.iTiltPLimit * ONEREVOLUTION_ANGLE / TILT_PULSE_PER_REV / TILT_REDUCTION;

    edAxis1MLimt -> Text = OM.CmnOptn.iSpinMLimit / SPIN_PULSE_PER_REV ;
    edAxis1PLimt -> Text = OM.CmnOptn.iSpinPLimit / SPIN_PULSE_PER_REV ;



}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Timer1Timer(TObject *Sender)
{
    Timer1->Enabled = false;
    //pPaix->GetNmcMotionOut(&g_NmcMotionOut);
    //pPaix->GetOutputX(g_lOutputXIo);
    //pPaix->GetInputX(g_lInputXIo);
    SetOutStatus();

    TPanel *pPanel = NULL;
    int iTemp;

    for(int i = 0 ; i < 16; i++){
        pPanel = (TPanel *)FindComponent(String("pnIn") + i);
        if( pPanel == NULL ) return;
        pPanel -> Color = g_lInputXIo[i]?clRed:clBtnFace;
    }
    for(int i = 0 ; i < 16; i++){
        pPanel = (TPanel *)FindComponent(String("pnOut") + i);
        if( pPanel == NULL ) return;
        pPanel -> Color = g_lOutputXIo[i]==1?clRed:clBtnFace;
        iTemp = g_lOutputXIo[i];
        iTemp = iTemp;
    }

    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btSpdSet1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    SetSpeed(nAxis);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetSpeed(int nAxis)
{
    double dStart, dAcc, dDec, dMax;

    if(nAxis == 0){
        dStart = StrToFloatDef(edStart1 -> Text, 100);
        dAcc   = StrToFloatDef(edAcc1   -> Text, 400);
        dDec   = StrToFloatDef(edDec1   -> Text, 400);
        dMax   = StrToFloatDef(edMax1   -> Text,1000);
    }
    else {
        dStart = StrToFloatDef(edStart2 -> Text, 100);
        dAcc   = StrToFloatDef(edAcc2   -> Text, 400);
        dDec   = StrToFloatDef(edDec2   -> Text, 400);
        dMax   = StrToFloatDef(edMax2   -> Text,1000);
    }
    pPaix -> SetSpeed(nAxis, dStart, dAcc, dDec, dMax);
}


void __fastcall TFrmMaster::cbEncCount1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbEncCount1 -> ItemIndex;
    }
    else {
        nMode = cbEncCount2 -> ItemIndex;
    }
    pPaix->SetEncCount(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::cbEncZ1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbEncZ1 -> ItemIndex;
    }
    else {
        nMode = cbEncZ2 -> ItemIndex;
    }
    pPaix->SetEncZLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbEncDir1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbEncDir1 -> ItemIndex;
    }
    else {
        nMode = cbEncDir2 -> ItemIndex;
    }
    pPaix->SetEncDir(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbNear1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbNear1 -> ItemIndex;
    }
    else {
        nMode = cbNear2 -> ItemIndex;
    }
    pPaix->SetNearLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::cbMLimit1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbMLimit1 -> ItemIndex;
    }
    else {
        nMode = cbMLimit2 -> ItemIndex;
    }
    pPaix->SetMinusLimitLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbPLimit1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbPLimit1 -> ItemIndex;
    }
    else {
        nMode = cbPLimit2 -> ItemIndex;
    }
    pPaix->SetPlusLimitLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::cbAlarm1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbAlarm1 -> ItemIndex;
    }
    else {
        nMode = cbAlarm2 -> ItemIndex;
    }
    pPaix->SetAlarmLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbInpo1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbInpo1 -> ItemIndex;
    }
    else {
        nMode = cbInpo2 -> ItemIndex;
    }
    pPaix->SetInPoLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbPulse1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbPulse1 -> ItemIndex;
    }
    else {
        nMode = cbPulse2 -> ItemIndex;
    }
    pPaix->SetPulseLogic(nAxis,nMode);
    SaveCmnOption();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbBusy1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbBusy1 -> ItemIndex;
    }
    else {
        nMode = cbBusy2 -> ItemIndex;
    }
    pPaix->SetBusyOffMode(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbSvReady1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbSvReady1 -> ItemIndex;
    }
    else {
        nMode = cbSvReady2 -> ItemIndex;
    }
    pPaix->SetSvReadyLogic(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbStop1Click(TObject *Sender)
{
    if( pPaix == NULL )    return;
    int iCbNo = ((TComboBox *)Sender) -> Tag ;
    int nAxis = iCbNo -1;
    int nMode ;

    if(nAxis == 0) {
        nMode = cbStop1 -> ItemIndex;
    }
    else {                                                                                              
        nMode = cbStop2 -> ItemIndex;
    }
    pPaix->SetHwLimitStopMode(nAxis,nMode);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::cbEmgChange(TObject *Sender)
{
    pPaix->SetEmgLogic(cbEmg->ItemIndex);
    SaveCmnOption();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btSetRatio1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;
    double dRatio;

    if(nAxis == 0){
        dRatio = StrToFloatDef(edRatio1 -> Text, 1);
    }
    else{
        dRatio = StrToFloatDef(edRatio2 -> Text, 1);
    }

    if( dRatio <= 0 )
    {
        ShowMessage("UnitPerPulse 에러");
        return;
    }

    pPaix->SetRatio(nAxis, dRatio);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::edCmdValue1DblClick(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    pPaix -> SetCmdPos(nAxis, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::edEncValue1DblClick(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    pPaix -> SetEncPos(nAxis, 0);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::btScvSet1Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo -1;

    pPaix -> SetOverrideDSpeed(nAxis , pPaix -> GetPulseSpeedFromRPM(nAxis, Edit1 -> Text.ToInt()));
    //SetScvSpeed(nAxis);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::SetScvSpeed(int nAxis)
{
    double dStart, dAcc, dDec, dMax;

    if(nAxis == 0){
        dStart = StrToFloatDef(edStart1 -> Text, 100);
        dAcc   = StrToFloatDef(edAcc1   -> Text, 400);
        dDec   = StrToFloatDef(edDec1   -> Text, 400);
        dMax   = StrToFloatDef(edMax1   -> Text,1000);
    }
    else {
        dStart = StrToFloatDef(edStart2 -> Text, 100);
        dAcc   = StrToFloatDef(edAcc2   -> Text, 400);
        dDec   = StrToFloatDef(edDec2   -> Text, 400);
        dMax   = StrToFloatDef(edMax2   -> Text,1000);
    }

    pPaix -> SetSCurveSpeed(nAxis, dStart, dAcc, dDec, dMax);
}
void __fastcall TFrmMaster::DisplayInputX()
{
//    pPaix->GetInputX(m_lInputXIo);
//
//    int u = m_lInputXIo[0];
//
//    if(u == 1){FrmValve -> Temp = true;}
//    else FrmValve -> Temp = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::DisplayOutputX()
{
//    pPaix->GetOutputX(m_lOutputXIo);

//    int u = m_lOutputXIo[0];
}
void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    if(cbAxis0Limit -> Checked) {
        OM.CmnOptn.bUsedTiltLimit = true;
        OM.CmnOptn.iTiltMLimit = edAxis0MLimt -> Text.ToInt() * TILT_PULSE_PER_REV / (TILT_REDUCTION * ONEREVOLUTION_ANGLE);
        OM.CmnOptn.iTiltPLimit = edAxis0PLimt -> Text.ToInt() * TILT_PULSE_PER_REV / (TILT_REDUCTION * ONEREVOLUTION_ANGLE);

        pPaix -> SetSWLimitLogic(miTiltMt, true , OM.CmnOptn.iTiltMLimit, OM.CmnOptn.iTiltPLimit);
    }
    else {
        OM.CmnOptn.bUsedTiltLimit = false;
        OM.CmnOptn.iTiltMLimit = edAxis0MLimt -> Text.ToInt() * TILT_PULSE_PER_REV / (TILT_REDUCTION * ONEREVOLUTION_ANGLE);
        OM.CmnOptn.iTiltPLimit = edAxis0PLimt -> Text.ToInt() * TILT_PULSE_PER_REV / (TILT_REDUCTION * ONEREVOLUTION_ANGLE);

        pPaix -> SetSWLimitLogic(miTiltMt, false , OM.CmnOptn.iTiltMLimit, OM.CmnOptn.iTiltPLimit);
    }
    if(cbAxis1Limit -> Checked) {
        OM.CmnOptn.bUsedSpinLimit = true;
        OM.CmnOptn.iSpinMLimit = edAxis1MLimt -> Text.ToInt() * SPIN_PULSE_PER_REV;
        OM.CmnOptn.iSpinPLimit = edAxis1PLimt -> Text.ToInt() * SPIN_PULSE_PER_REV;

        pPaix -> SetSWLimitLogic(miSpinMt, true , OM.CmnOptn.iSpinMLimit, OM.CmnOptn.iSpinPLimit);
    }
    else{
        OM.CmnOptn.bUsedSpinLimit = false;
        OM.CmnOptn.iSpinMLimit = edAxis1MLimt -> Text.ToInt() * SPIN_PULSE_PER_REV;
        OM.CmnOptn.iSpinPLimit = edAxis1PLimt -> Text.ToInt() * SPIN_PULSE_PER_REV;

        pPaix -> SetSWLimitLogic(miSpinMt, false, OM.CmnOptn.iSpinMLimit, OM.CmnOptn.iSpinPLimit);
    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::pnOut0Click(TObject *Sender)
{
    TPanel *pPanel = NULL;
    pPanel = dynamic_cast<TPanel *>(Sender);

    pPanel = (TPanel *)FindComponent(String("pnOut") + pPanel->Tag);

    if(pPanel->Color == clRed){
        pPaix -> SetOutputXBit(pPanel->Tag, false);
    }
    else {
        pPaix -> SetOutputXBit(pPanel->Tag, true);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
    int iBtNo = ((TButton *)Sender) -> Tag ;
    int nAxis = iBtNo - 1;

    int iTemp = Edit1 -> Text.ToInt();
    if(nAxis == miSpinMt){
        Edit1 -> Text = pPaix -> GetPulseSpeedFromRPM(nAxis, iTemp);
    }
    if(nAxis == miTiltMt){
        Edit1 -> Text = pPaix -> GetPulseSpeedFromRPM(nAxis, iTemp);
    }
}
//---------------------------------------------------------------------------




void __fastcall TFrmMaster::Timer2Timer(TObject *Sender)
{
    Timer2 -> Enabled = false;

    pPaix -> GetInputX(g_lInputXIo);

    if(g_lInputXIo[xETC_StartSw]){ SEQ ->m_bBtnStart = true; }
    if(g_lInputXIo[xETC_StopSw ]){ SEQ ->m_bBtnStop  = true; }

    Timer2 -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::SaveCmnOption()
{
    OM.CmnOptn.iEmgLogic         = cbEmg       -> ItemIndex;

    OM.CmnOptn.iRatio1           = 1;
    OM.CmnOptn.iPlusLimitLogic1  = cbPLimit1   -> ItemIndex;
    OM.CmnOptn.iMinusLimitLogic1 = cbMLimit1   -> ItemIndex;
    OM.CmnOptn.iAlarmLogic1      = cbAlarm1    -> ItemIndex;
    OM.CmnOptn.iNearLogic1       = cbNear1     -> ItemIndex;
    OM.CmnOptn.iInPoLogic1       = cbInpo1     -> ItemIndex;
    OM.CmnOptn.iSvReadyLogic1    = cbSvReady1  -> ItemIndex;
    OM.CmnOptn.iEncZLogic1       = cbEncZ1     -> ItemIndex;
    OM.CmnOptn.iEncDir1          = cbEncDir1   -> ItemIndex;
    OM.CmnOptn.iEncCount1        = cbEncCount1 -> ItemIndex;

    OM.CmnOptn.iRatio2           = 1;
    OM.CmnOptn.iPlusLimitLogic2  = cbPLimit2   -> ItemIndex;
    OM.CmnOptn.iMinusLimitLogic2 = cbMLimit2   -> ItemIndex;
    OM.CmnOptn.iAlarmLogic2      = cbAlarm2    -> ItemIndex;
    OM.CmnOptn.iNearLogic2       = cbNear2     -> ItemIndex;
    OM.CmnOptn.iInPoLogic2       = cbInpo2     -> ItemIndex;
    OM.CmnOptn.iSvReadyLogic2    = cbSvReady2  -> ItemIndex;
    OM.CmnOptn.iEncZLogic2       = cbEncZ2     -> ItemIndex;
    OM.CmnOptn.iEncDir2          = cbEncDir2   -> ItemIndex;
    OM.CmnOptn.iEncCount2        = cbEncCount2 -> ItemIndex;

    OM.SaveCmnOptn();
}

void __fastcall TFrmMaster::LoadCmnOption()
{
    cbEmg       -> ItemIndex = OM.CmnOptn.iEmgLogic;

    edRatio1    -> Text      = OM.CmnOptn.iRatio1  ;
    cbPLimit1   -> ItemIndex = OM.CmnOptn.iPlusLimitLogic1 ;
    cbMLimit1   -> ItemIndex = OM.CmnOptn.iMinusLimitLogic1;
    cbAlarm1    -> ItemIndex = OM.CmnOptn.iAlarmLogic1     ;
    cbNear1     -> ItemIndex = OM.CmnOptn.iNearLogic1      ;
    cbInpo1     -> ItemIndex = OM.CmnOptn.iInPoLogic1      ;
    cbSvReady1  -> ItemIndex = OM.CmnOptn.iSvReadyLogic1   ;
    cbEncZ1     -> ItemIndex = OM.CmnOptn.iEncZLogic1      ;
    cbEncDir1   -> ItemIndex = OM.CmnOptn.iEncDir1         ;
    cbEncCount1 -> ItemIndex = OM.CmnOptn.iEncCount1       ;

    edRatio2    -> Text      = OM.CmnOptn.iRatio2           ;
    cbPLimit2   -> ItemIndex = OM.CmnOptn.iPlusLimitLogic2  ;
    cbMLimit2   -> ItemIndex = OM.CmnOptn.iMinusLimitLogic2 ;
    cbAlarm2    -> ItemIndex = OM.CmnOptn.iAlarmLogic2      ;
    cbNear2     -> ItemIndex = OM.CmnOptn.iNearLogic2       ;
    cbInpo2     -> ItemIndex = OM.CmnOptn.iInPoLogic2       ;
    cbSvReady2  -> ItemIndex = OM.CmnOptn.iSvReadyLogic2    ;
    cbEncZ2     -> ItemIndex = OM.CmnOptn.iEncZLogic2       ;
    cbEncDir2   -> ItemIndex = OM.CmnOptn.iEncDir2          ;
    cbEncCount2 -> ItemIndex = OM.CmnOptn.iEncCount2        ;
}

void __fastcall TFrmMaster::SaveMstOption()
{
    OM.MstOptn.bUsedFrstMachine = cbFrstmachine -> Checked;
    OM.MstOptn.bIngrCvrSnsr     = cbIngrCvrSnsr -> Checked;
    OM.MstOptn.bUsedDryRun      = cbDryRun      -> Checked;

    OM.SaveMstOptn();
}
void __fastcall TFrmMaster::LoadMstOption()
{
    cbFrstmachine -> Checked = OM.MstOptn.bUsedFrstMachine ;
    cbDryRun      -> Checked = OM.MstOptn.bUsedDryRun      ;
    cbIngrCvrSnsr -> Checked = OM.MstOptn.bIngrCvrSnsr     ;
}

void __fastcall TFrmMaster::Button4Click(TObject *Sender)
{
    SaveMstOption();
}
//---------------------------------------------------------------------------

