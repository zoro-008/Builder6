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
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "LeftTool.h"
#include "RightTool.h"
#include "Stage.h"
#include "Unloader.h"
#include "VisionCom.h"
//---------------------------------------------------------------------------
#include "Rs232UVCuring.h"
#include "Rs232Dispr.h"
#include "HexaPot.h"
#include "Confocal.h"
#include "CadPosition.h"
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

    iHeightCh = 0;

    bReceive = false;
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
        cbDebugMode           -> Checked = OM.MstOptn.bDebugMode               ;
        edLTL_RTL_Stroke      -> Text    = OM.MstOptn.dLTL_RTL_Stroke          ;
        edLTL_VISN_DISP_Strke -> Text    = OM.MstOptn.dLTL_VISN_DISP_Strke     ;
        edRTL_TRNS_VISN_Strke -> Text    = OM.MstOptn.dRTL_TRNS_VISN_Strke     ;
        cbDonUseMasterT       -> Checked = OM.MstOptn.bDonUseMasterT           ;
        cbUseLotInfo          -> Checked = OM.MstOptn.bUseLotInfo              ;
//        edPckDspStroke     -> Text    = OM.MstOptn.dPckDspStroke      ;
//        cbSkipPickrDispr   -> Checked = OM.MstOptn.bSkipPickrDispr    ;

    }
    else{
        OM.MstOptn.bDebugMode            = cbDebugMode           -> Checked ;
        OM.MstOptn.dLTL_RTL_Stroke       = StrToFloatDef(edLTL_RTL_Stroke      -> Text, 0.0) ;
        OM.MstOptn.dLTL_VISN_DISP_Strke  = StrToFloatDef(edLTL_VISN_DISP_Strke -> Text, 0.0) ;
        OM.MstOptn.dRTL_TRNS_VISN_Strke  = StrToFloatDef(edRTL_TRNS_VISN_Strke -> Text, 0.0) ;
        OM.MstOptn.bDonUseMasterT        = cbDonUseMasterT       -> Checked;
        OM.MstOptn.bUseLotInfo           = cbUseLotInfo          -> Checked;
//        OM.MstOptn.dPckDspStroke      = StrToFloatDef(edPckDspStroke -> Text, 0.0) ;
//        OM.MstOptn.bSkipPickrDispr    = cbSkipPickrDispr    -> Checked ;
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
    tmUpdate -> Enabled = false;

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

//    bool bDsprMsgEnd = DSP.Rs232_Dispr .GetMsgEnd();
    bool bLdClMsgEnd = RTL.Rs232_LoadCl.GetMsgEnd();
    bool bHghtMsgEnd = Rs232Keyence.GetMsgEnd();
    bool bUVMsgEnd   = Rs232UV.GetMsgEnd();


//    edDisprErrMsg -> Text = DSP.Rs232_Dispr.GetErrMsg();
      edUVCuringMsg -> Text = Rs232UV.GetMsg();


//    if(bDsprMsgEnd) lbDisprMsgEnd -> Caption = "True ";
//    else            lbDisprMsgEnd -> Caption = "False";

      if(bUVMsgEnd) lbUVMsgEnd    -> Caption = "True ";
      else          lbDisprMsgEnd -> Caption = "False";

    if(bHghtMsgEnd){
        edHghDataCh0 -> Text = Rs232Keyence.GetHeightData(kcChA);
        edHghDataCh1 -> Text = Rs232Keyence.GetHeightData(kcChB);
        edHghMsg     -> Text = Rs232Keyence.GetMsg();
        lbHghtMsgEnd -> Caption = "True";
    }
    else {
        lbHghtMsgEnd -> Caption = "False";
    }

    if(bLdClMsgEnd){

        edLoadClData -> Text = RTL.Rs232_LoadCl.GetLoadCellData() ; //GetLoadCellData();
        edLoadClMsg  -> Text = RTL.Rs232_LoadCl.GetMsg() ; //GetLoadCellData();
        lbLoadClMsgEnd -> Caption = "True";
    }
    else {
        lbLoadClMsgEnd -> Caption = "False";
    }
    if(bReceive){
        if(edUVDataChk -> Text != Rs232UV.GetData()) {
            edUVDataChk -> Text =  Rs232UV.GetData();
            bReceive = false;
        }

    }


//Hexa Pot.=================
    edEncX -> Text = HexaPot.GetEncPos(paX);
    edEncY -> Text = HexaPot.GetEncPos(paY);
    edEncZ -> Text = HexaPot.GetEncPos(paZ);
    edEncU -> Text = HexaPot.GetEncPos(paU);
    edEncV -> Text = HexaPot.GetEncPos(paV);
    edEncW -> Text = HexaPot.GetEncPos(paW);


    btHome   -> Enabled = HexaPot.GetStop();
    btMoveTo -> Enabled = HexaPot.GetStop();

    lbErr -> Caption = HexaPot.GetLastErrMsg() ;
//===========================








    tmUpdate -> Enabled = true;

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
    if(cbPart[piLDR_F] -> Checked) {
        MM.SetManCycle(mcLDR_FHome);}
    else if(cbPart[piLDR_R] -> Checked) {
        MM.SetManCycle(mcLDR_RHome);}
    else if(cbPart[piIDX] -> Checked) {
        MM.SetManCycle(mcIDX_Home);}
    else if(cbPart[piLTL] -> Checked) {
        MM.SetManCycle(mcLTL_Home);}
    else if(cbPart[piRTL] -> Checked) {
        MM.SetManCycle(mcRTL_Home);}
    else if(cbPart[piSTG] -> Checked) {
        MM.SetManCycle(mcSTG_Home);}
    else if(cbPart[piULD] -> Checked) {
        MM.SetManCycle(mcULD_Home);}

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

    if(cbDisprSelect -> ItemIndex == 0){
        switch(iIndex){
            case 1: IO_SetY(yLTL_Dispr1OnOff, true ); break;
            case 2: IO_SetY(yLTL_Dispr1OnOff, false); break;
            //case 1: Rs232_DisprFt.SetDisprOnOff(true );                    break;
            //case 2: Rs232_DisprFt.SetDisprOnOff(false);                    break;
            case 3: Rs232_DisprFt.SetSigmaModeOn(    );                    break;
            case 4: Rs232_DisprFt.SetMode(cbSetMode -> ItemIndex);         break;
            case 5: Rs232_DisprFt.SetLoadCh(cbLoadCh -> ItemIndex);        break;
            case 6: Rs232_DisprFt.SetPTV(edPres -> Text.ToIntDef(1),
                                           edTime -> Text.ToIntDef(1),
                                           edVacc -> Text.ToIntDef(1));    break;
            case 7: Rs232_DisprFt.SetSyringeSize(cbSyrgSize -> ItemIndex); break;
        }
    }
    else{
        switch(iIndex){
            case 1: IO_SetY(yLTL_Dispr2OnOff, true ); break;
            case 2: IO_SetY(yLTL_Dispr2OnOff, false); break;
            //case 1: Rs232_DisprRr.SetDisprOnOff(true );                    break;
            //case 2: Rs232_DisprRr.SetDisprOnOff(false);                    break;
            case 3: Rs232_DisprRr.SetSigmaModeOn(    );                    break;
            case 4: Rs232_DisprRr.SetMode(cbSetMode -> ItemIndex);         break;
            case 5: Rs232_DisprRr.SetLoadCh(cbLoadCh -> ItemIndex);        break;
            case 6: Rs232_DisprRr.SetPTV(edPres -> Text.ToIntDef(1),
                                           edTime -> Text.ToIntDef(1),
                                           edVacc -> Text.ToIntDef(1));    break;
            case 7: Rs232_DisprRr.SetSyringeSize(cbSyrgSize -> ItemIndex); break;
        }
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
        case 1: RTL.Rs232_LoadCl.WeightCheck(     );     break;
        case 2: RTL.Rs232_LoadCl.SetHold    (true );     break;
        case 3: RTL.Rs232_LoadCl.SetHold    (false);     break;
        case 4: RTL.Rs232_LoadCl.AutoZero   (     );     break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btSetTempClick(TObject *Sender)
{
    TButton * Button = (TButton *)Sender ;
    int      iIndex  =  Button -> Tag ;

//    switch(iIndex){
//        //온도 컨트롤러 붙이는 위치가...
//        case 1: Rs232_TempCh.SetTemp(cbTempCh -> ItemIndex + 1, edSetTemp -> Text.ToIntDef(1));     break;
//    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btUVChClick(TObject *Sender)
{
    TButton * Button = (TButton *)Sender ;
    int       iIndex =  Button -> Tag ;

    switch(iIndex){
        case 1: Rs232UV.SetCuringOnOff(cbUVCh2 -> ItemIndex + 1, true );     break;
        case 2: Rs232UV.SetCuringOnOff(cbUVCh2 -> ItemIndex + 1, false);     break;
        case 3: Rs232UV.SetCuring     (cbUVCh1 -> ItemIndex + 1,
                                       edUVLightData -> Text.ToIntDef(1));   break;
        case 4: Rs232UV.SendCuringData(cbUVCh3 -> ItemIndex + 1);
                bReceive = true;                                             break;
        case 5: Rs232UV.MsgReset();                                          break;
    }
}
//---------------------------------------------------------------------------





void __fastcall TFrmMaster::btHomeClick(TObject *Sender)
{
    HexaPot.DoHome();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::btMoveToClick(TObject *Sender)
{
    HexaPot.MoveAbsRes(paX , StrToFloatDef(Edit1 -> Text , 0.0));
    HexaPot.MoveAbsRes(paY , StrToFloatDef(Edit2 -> Text , 0.0));
    HexaPot.MoveAbsRes(paZ , StrToFloatDef(Edit3 -> Text , 0.0));
    HexaPot.MoveAbsRes(paU , StrToFloatDef(Edit4 -> Text , 0.0));
    HexaPot.MoveAbsRes(paV , StrToFloatDef(Edit5 -> Text , 0.0));
    HexaPot.MoveAbsRes(paW , StrToFloatDef(Edit6 -> Text , 0.0));

    HexaPot.Move();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
    double dPivotX = StrToFloatDef(edPivotX->Text,0.0);
    double dPivotY = StrToFloatDef(edPivotY->Text,0.0);
    double dPivotZ = StrToFloatDef(edPivotZ->Text,0.0);

    HexaPot.SetPivotPnt(dPivotX,dPivotY,dPivotZ);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Panel1Click(TObject *Sender)
{
    edLimMinX -> Text = HexaPot.GetLmtMinPos(paX); edLimMaxX -> Text = HexaPot.GetLmtMaxPos(paX);
    edLimMinY -> Text = HexaPot.GetLmtMinPos(paY); edLimMaxY -> Text = HexaPot.GetLmtMaxPos(paY);
    edLimMinU -> Text = HexaPot.GetLmtMinPos(paU); edLimMaxU -> Text = HexaPot.GetLmtMaxPos(paU);
    edLimMinV -> Text = HexaPot.GetLmtMinPos(paV); edLimMaxV -> Text = HexaPot.GetLmtMaxPos(paV);
    edLimMinW -> Text = HexaPot.GetLmtMinPos(paW); edLimMaxW -> Text = HexaPot.GetLmtMaxPos(paW);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button3Click(TObject *Sender)
{
    if(cbSendMsg -> ItemIndex == 0) {
        VSN_L.SendInsp     ();
    }
    else if(cbSendMsg -> ItemIndex == 1) {
        VSN_L.SendReset    ();
    }
    else if(cbSendMsg -> ItemIndex == 2) {
        VSN_L.SendCommand  (edCmd    ->Text);//String _sCommand
    }
    else if(cbSendMsg -> ItemIndex == 3) {
        VSN_L.SendJobChange(edJobName->Text);//String _sJobName
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button4Click(TObject *Sender)
{
    if(cbSendMsg -> ItemIndex == 0) {
        VSN_R.SendInsp     ();
    }
    else if(cbSendMsg -> ItemIndex == 1) {
        VSN_R.SendReset    ();
    }
    else if(cbSendMsg -> ItemIndex == 2) {
        VSN_R.SendCommand  (edCmd    ->Text);//String _sCommand
    }
    else if(cbSendMsg -> ItemIndex == 3) {
        VSN_R.SendJobChange(edJobName->Text);//String _sJobName
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::tmVisnTimer(TObject *Sender)
{
    //bool GetSendInspEnd     ()
    //bool GetSendResetEnd    ()
    //bool GetSendCommandEnd  ()
    //bool GetSendJobChangeEnd()

    pnInspEndL    -> Color = VSN_L.GetSendInspEnd()      ? clLime : clGray ;
    pnResetEndL   -> Color = VSN_L.GetSendResetEnd()     ? clLime : clGray ;
    pnCommandEndL -> Color = VSN_L.GetSendCommandEnd()   ? clLime : clGray ;
    pnJobChangeL  -> Color = VSN_L.GetSendJobChangeEnd() ? clLime : clGray ;
    pnInspLRslt   -> Color = IO_GetX(xVisnL_InspOk)      ? clLime : clGray ;
    pnInspRRslt   -> Color = IO_GetX(xVisnR_InspOk)      ? clLime : clGray ;

    pnInspEndR    -> Color = VSN_R.GetSendInspEnd()      ? clLime : clGray ;
    pnResetEndR   -> Color = VSN_R.GetSendResetEnd()     ? clLime : clGray ;
    pnCommandEndR -> Color = VSN_R.GetSendCommandEnd()   ? clLime : clGray ;
    pnJobChangeR  -> Color = VSN_R.GetSendJobChangeEnd() ? clLime : clGray ;

    String sTemp ;
    sTemp.sprintf("X:%0.3f Y:%0.3f x:%0.3f y:%0.3f" , VSN_L.GetAlignRslt().dMainX , VSN_L.GetAlignRslt().dMainY , VSN_L.GetAlignRslt().dSubX , VSN_L.GetAlignRslt().dSubY);
    lbLeftResult -> Caption = sTemp ;
    sTemp.sprintf("X:%0.3f Y:%0.3f x:%0.3f y:%0.3f" , VSN_R.GetAlignRslt().dMainX , VSN_R.GetAlignRslt().dMainY , VSN_R.GetAlignRslt().dSubX , VSN_R.GetAlignRslt().dSubY);
    lbLeftResult -> Caption = sTemp ;








}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btHexaServoOffClick(TObject *Sender)
{
    HexaPot.SetServo(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btHexaServoOnClick(TObject *Sender)
{
    HexaPot.SetServo(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btSaveTrgPosClick(TObject *Sender)
{
    HexaPot.MoveSaveTrgRes(paX);
    HexaPot.MoveSaveTrgRes(paY);
    HexaPot.MoveSaveTrgRes(paZ);
    HexaPot.MoveSaveTrgRes(paU);
    HexaPot.MoveSaveTrgRes(paV);
    HexaPot.MoveSaveTrgRes(paW);

    HexaPot.Move();


}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btCycleCmsLAlignClick(TObject *Sender)
{
    MM.SetManCycle(mcLTLCycleCmsLAlign);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btCMRectCenterClick(TObject *Sender)
{
    CCadObject * Obj = CAD.FindObjectByName("CM_Rect_00") ;

    double dX, dY;

    dX = Obj -> GetCntrPosX();
    dY = Obj -> GetCntrPosY();

    dX += g_tMidPosOfs.dX;
    dY += g_tMidPosOfs.dY;

    MM.SetCadMove(tlStage, dX , dY);

}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::btDataCopyClick(TObject *Sender)
{
    Edit1 -> Text = HexaPot.GetEncPos(paX);
    Edit2 -> Text = HexaPot.GetEncPos(paY);
    Edit3 -> Text = HexaPot.GetEncPos(paZ);
    Edit4 -> Text = HexaPot.GetEncPos(paU);
    Edit5 -> Text = HexaPot.GetEncPos(paV);
    Edit6 -> Text = HexaPot.GetEncPos(paW);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button6Click(TObject *Sender)
{
   IO_SetY(ySTG_UVAirSol , true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button7Click(TObject *Sender)
{
    IO_SetY(ySTG_UVAirSol , false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btConfocalRezeroOnClick(TObject *Sender)
{
    Confocal.Rezero(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button8Click(TObject *Sender)
{
    Confocal.Rezero(false);
}
//---------------------------------------------------------------------------

