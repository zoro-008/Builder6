//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMaster *FrmMaster;
//---------------------------------------------------------------------------
__fastcall TFrmMaster::TFrmMaster(TComponent* Owner)
    : TForm(Owner)
{

    sgCycleStep -> RowCount = MAX_PART + 1 ;
    sgCycleStep -> ColWidths[0,0] = 100;
    sgCycleStep -> Cells[1][0] = "Home    " ;
    sgCycleStep -> Cells[2][0] = "ToStart " ;
    sgCycleStep -> Cells[3][0] = "Seq     " ;
    sgCycleStep -> Cells[4][0] = "Cycle   " ;
    sgCycleStep -> Cells[5][0] = "ToStop  " ;
    sgCycleStep -> Cells[6][0] = "Pre Time" ;

    AnsiString sTemp ="";

    const int iCheckBoxGap = 25 ;
    for(int i= 0 ; i < MAX_PART ; i++) {
        cbPart[i] = new TCheckBox (this);
        cbPart[i] -> Parent = GroupBox8 ;
        cbPart[i] -> Left   = 10        ;
        cbPart[i] -> Top    = 20 + iCheckBoxGap * i ;
        sTemp = SEQ.m_pPart[i] -> GetPartName();

        cbPart[i] -> Caption = SEQ.m_pPart[i]->GetPartName();
        sgCycleStep -> Cells[0][i+1]  = SEQ.m_pPart[i]->GetPartName();
    }


    for(int i= 0 ; i < MAX_PART ; i++) {
        sgCycleStep -> Cells[0][i+1]  = SEQ.m_pPart[i]->GetPartName();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        cbPartSel -> AddItem(SEQ.m_pPart[i]->GetPartName(),NULL) ;
    }
    cbPartSel -> ItemIndex = 0 ;

    btDisprOn        -> Tag = (int)btTagNum1;
    btDisprOff       -> Tag = (int)btTagNum2;
    btDisprSigmaTest -> Tag = (int)btTagNum3;
    btDisprSetMode   -> Tag = (int)btTagNum4;
    btDisprLoadCh    -> Tag = (int)btTagNum5;
    btDisprSetPTV    -> Tag = (int)btTagNum6;
    btDisprSyrngSize -> Tag = (int)btTagNum7;

    btHghtSetZero    -> Tag = (int)btTagNum1;
    btHghtCheck      -> Tag = (int)btTagNum2;
    btHghtGetDat     -> Tag = (int)btTagNum3;

    btLoadClWghtChck -> Tag = (int)btTagNum1;
    btLoadClHoldOn   -> Tag = (int)btTagNum2;
    btLoadClHoldOff  -> Tag = (int)btTagNum3;

    btSetTemp        -> Tag = (int)btTagNum1;

    iHeightCh = 0;

}

//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btSaveMotorClick(TObject *Sender)
{
    UpdateMstOptn (toBuff);
    UpdateEqpOptn (toBuff);
    OM.SaveMstOptn(      );
    OM.SaveEqpOptn(      );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::FormShow(TObject *Sender)
{
    OM.LoadMstOptn(      );
    OM.LoadEqpOptn(      );
    UpdateMstOptn (toTabl);
    UpdateEqpOptn (toTabl);

    UpdateTimeName();
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateMstOptn(bool bTable)
{
    if(bTable){
        cbDebugMode        -> Checked = OM.MstOptn.bDebugMode         ;
        edPckDspStroke     -> Text    = OM.MstOptn.dPckDspStroke      ;
        cbSkipPickrDispr   -> Checked = OM.MstOptn.bSkipPickrDispr    ;
        edPckToStgAngleOfs -> Text    = OM.MstOptn.dPckToStgAngleOfs  ;

    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode         -> Checked ;
        OM.MstOptn.dPckDspStroke      = StrToFloatDef(edPckDspStroke -> Text, 0.0) ;
        OM.MstOptn.bSkipPickrDispr    = cbSkipPickrDispr    -> Checked ;
        OM.MstOptn.dPckToStgAngleOfs  = StrToFloatDef(edPckToStgAngleOfs -> Text,0.0)  ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateEqpOptn(bool bTable)
{
    if(bTable){
    }
    else{
    }
}
void __fastcall TFrmMaster::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::tmUpdateTimer(TObject *Sender)
{

    lbMan -> Caption = "<Man No = " + AnsiString(MM.GetManNo()) + "> <Man.m_iStep = " + MM.GetManStep() ;
    //LDR

    for(int i= 0 ; i < MAX_PART ; i++) {
        sgCycleStep->Cells[1][i+1]=SEQ.m_pPart[i]->GetHomeStep   ();
        sgCycleStep->Cells[2][i+1]=SEQ.m_pPart[i]->GetToStartStep();
        sgCycleStep->Cells[3][i+1]=SEQ.m_pPart[i]->GetCycleName  (SEQ.m_pPart[i]->GetSeqStep());
        sgCycleStep->Cells[4][i+1]=SEQ.m_pPart[i]->GetCycleStep  ();
        sgCycleStep->Cells[5][i+1]=SEQ.m_pPart[i]->GetToStopStep ();
        sgCycleStep->Cells[6][i+1]=SEQ.m_pPart[i]->GetCycleTime  (SEQ.m_pPart[i]->GetSeqStep());
    }

    for(int i=0; i< SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleMaxCnt(); i++ ) {
        sgTime -> Cells[1][i] = SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleTime(i);
    }
    //JS
    //Rs232_TempCh.RqstCrntTemp();

    bool bDsprMsgEnd = DSP.Rs232_Dispr .GetMsgEnd();
    bool bHghtMsgEnd = Rs232Keyence.GetMsgEnd();
    bool bLdClMsgEnd = PCK.Rs232_LoadCl.GetMsgEnd();

    edDisprErrMsg -> Text = DSP.Rs232_Dispr.GetErrMsg();

    if(bDsprMsgEnd) lbDisprMsgEnd -> Caption = "True ";
    else            lbDisprMsgEnd -> Caption = "False";

    if(bHghtMsgEnd){
        edHghDataCh0 -> Text = Rs232Keyence.GetHeightData(kcChA);
        edHghDataCh1 -> Text = Rs232Keyence.GetHeightData(kcChB);
        edHghMsg     -> Text = Rs232Keyence.GetMsg();
        lbHghtMsgEnd -> Caption = "True";
    }
    else {
        lbHghtMsgEnd -> Caption = "False";
    }

    //if(bLdClMsgEnd){
    
        edLoadClData -> Text = PCK.Rs232_LoadCl.GetLoadCellData() ; //GetLoadCellData();
        edLoadClMsg  -> Text = PCK.Rs232_LoadCl.GetMsg() ; //GetLoadCellData();
        lbLoadClMsgEnd -> Caption = "True";
    //}
    //else {
    //    lbLoadClMsgEnd -> Caption = "False";
    //}

    edCrntTemp1 -> Text = Rs232_TempCh.GetCrntTemp(btTagNum1);
    edCrntTemp2 -> Text = Rs232_TempCh.GetCrntTemp(btTagNum2);
    edCrntTemp3 -> Text = Rs232_TempCh.GetCrntTemp(btTagNum3);

    edGetTemp1  -> Text = Rs232_TempCh.GetSetTemp (btTagNum1);
    edGetTemp2  -> Text = Rs232_TempCh.GetSetTemp (btTagNum2);
    edGetTemp3  -> Text = Rs232_TempCh.GetSetTemp (btTagNum3);


}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartAutorunClick(TObject *Sender)
{

    for(int i= 0 ; i < MAX_PART ; i++) {
        if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Autorun() ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartResetClick(TObject *Sender)
{
    for(int i= 0 ; i < MAX_PART ; i++) {
        if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Reset() ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAllResetClick(TObject *Sender)
{
    SEQ.Reset();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartHomeClick(TObject *Sender)
{

    //하... 좀 고민 해보자..
    if(cbPart[piLDR_F] -> Checked) {
        MM.SetManCycle(mcLDR_FHome); }
    else if(cbPart[piLDR_R] -> Checked) {
        MM.SetManCycle(mcLDR_RHome); }
    else if(cbPart[piPRI_F] -> Checked) {
    MM.SetManCycle(mcPRI_FHome);}
    else if(cbPart[piPRI_R] -> Checked) {
    MM.SetManCycle(mcPRI_RHome);}
    else if(cbPart[piIDX_F] -> Checked) {
    MM.SetManCycle(mcIDX_FHome);}
    else if(cbPart[piIDX_R] -> Checked) {
    MM.SetManCycle(mcIDX_RHome);}
    else if(cbPart[piULD_F] -> Checked) {
    MM.SetManCycle(mcULD_FHome);}
    else if(cbPart[piULD_R] -> Checked) {
    MM.SetManCycle(mcULD_RHome);}
    else if(cbPart[piSTG  ] -> Checked) {
    MM.SetManCycle(mcSTGHome  );}
    else if(cbPart[piPCK  ] -> Checked) {
    MM.SetManCycle(mcPCKHome  );}
    else if(cbPart[piDSP  ] -> Checked) {
    MM.SetManCycle(mcDSPHome  );}





}
//---------------------------------------------------------------------------

void TFrmMaster::UpdateTimeName()
{
    sgTime-> RowCount = SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleMaxCnt() ;
    for(int i=0; i< sgTime->RowCount; i++ ) {
        sgTime->Rows[i]->Clear();
    }
    for(int i=0; i< SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleMaxCnt(); i++ ) {
        sgTime -> Cells[0][i] = SEQ.m_pPart[cbPartSel -> ItemIndex]->GetCycleName(i);
    }
}
void __fastcall TFrmMaster::cbPartSelChange(TObject *Sender)
{
    UpdateTimeName();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btAllCheckClick(TObject *Sender)
{
    bool bChecked = cbPart[0] -> Checked ;
    for(int i = 0 ; i < MAX_PART ; i++) {
        cbPart[i] -> Checked = !bChecked ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btDisprOnClick(TObject *Sender)
{
    TButton * Button = (TButton *)Sender ;
    int      iIndex  =  Button -> Tag ;

    switch(iIndex){
        case 1: DSP.Rs232_Dispr.SetDisprOnOff(true );                    break;
        case 2: DSP.Rs232_Dispr.SetDisprOnOff(false);                    break;
        case 3: DSP.Rs232_Dispr.SetSigmaModeOn(    );                    break;
        case 4: DSP.Rs232_Dispr.SetMode(cbSetMode -> ItemIndex);         break;
        case 5: DSP.Rs232_Dispr.SetLoadCh(cbLoadCh -> ItemIndex);        break;
        case 6: DSP.Rs232_Dispr.SetPTV(edPres -> Text.ToIntDef(1),
                                       edTime -> Text.ToIntDef(1),
                                       edVacc -> Text.ToIntDef(1));      break;
        case 7: DSP.Rs232_Dispr.SetSyringeSize(cbSyrgSize -> ItemIndex); break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btHghtSetZeroClick(TObject *Sender)
{
    TButton * Button = (TButton *)Sender ;
    int      iIndex  =  Button -> Tag ;

    switch(iIndex){
        case 1: Rs232Keyence.SetZero      (cbSetZeroCh    -> ItemIndex, true) ; break;
        case 2: Rs232Keyence.CheckHeight  (cbChkHgtCh     -> ItemIndex)       ; break;
        case 3: Rs232Keyence.GetHeightData(cbGetHghDataCh -> ItemIndex)       ;
                iHeightCh = cbGetHghDataCh -> ItemIndex;                        break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btLoadClWghtChckClick(TObject *Sender)
{
    TButton * Button = (TButton *)Sender ;
    int      iIndex  =  Button -> Tag ;

    switch(iIndex){
        case 1: PCK.Rs232_LoadCl.WeightCheck(     );     break;
        case 2: PCK.Rs232_LoadCl.SetHold    (true );     break;
        case 3: PCK.Rs232_LoadCl.SetHold    (false);     break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btSetTempClick(TObject *Sender)
{
    TButton * Button = (TButton *)Sender ;
    int      iIndex  =  Button -> Tag ;

    switch(iIndex){
        //온도 컨트롤러 붙이는 위치가...    
        case 1: Rs232_TempCh.SetTemp(cbTempCh -> ItemIndex + 1, edSetTemp -> Text.ToIntDef(1));     break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    double dVel1 = OM.DevOptn.dAttachSpeed1 > MT_GetRunVel(miPCK_ZPickr) ? MT_GetRunVel(miPCK_ZPickr) : OM.DevOptn.dAttachSpeed1 ;
    double dVel2 = OM.DevOptn.dAttachSpeed2 > MT_GetRunVel(miPCK_ZPickr) ? MT_GetRunVel(miPCK_ZPickr) : OM.DevOptn.dAttachSpeed2 ;

    double dVelMax = dVel1 > dVel2 ? dVel1 : dVel2 ;
    MT_SetOverrideMaxSpeed(miPCK_ZPickr , dVelMax) ;
                 //int _iAxisNo, double _dPos, double _dVel, double _dOverPos, double _dOverSped , bool _bCmd = false

    MT_SetOverrideVelAtPos(miPCK_ZPickr ,
                           20.0 ,
                           OM.DevOptn.dAttachSpeed1 ,
                           10.0  ,
                           OM.DevOptn.dAttachSpeed2 ,
                           true );

    //MT_GoAbs(miPCK_ZPickr , 20.0 , OM.DevOptn.dAttachSpeed1);

}
//---------------------------------------------------------------------------

