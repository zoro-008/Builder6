//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "Stage.h"
//#include "Vision.h"
//#include "DataMan.h"

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

    const int iCheckBoxGap = 25 ;
    for(int i= 0 ; i < MAX_PART ; i++) {
        cbPart[i] = new TCheckBox (this);
        cbPart[i] -> Parent = GroupBox8 ;
        cbPart[i] -> Left   = 10        ;
        cbPart[i] -> Top    = 20 + iCheckBoxGap * i ;
        cbPart[i] -> Caption = SEQ.m_pPart[i]->GetPartName();
        sgCycleStep -> Cells[0][i+1]  = SEQ.m_pPart[i]->GetPartName();
    }


    for(int i= 0 ; i < MAX_PART ; i++) {
        sgCycleStep -> Cells[0][i+1]  = SEQ.m_pPart[i]->GetPartName();
    }

    cbPartSel -> Clear() ;
    for(int i = 0 ; i < MAX_PART ; i++) {
        cbPartSel -> AddItem(SEQ.m_pPart[i]->GetPartName(),NULL) ;
    }
    cbPartSel -> ItemIndex = 0 ;




}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::btSaveMotorClick(TObject *Sender)
{
    UpdateMstOptn (toBuff);
    UpdateEqpOptn (toBuff);

    UpdateMstDevOptn (toBuff);

    OM.SaveMstOptn(      );
    OM.SaveEqpOptn(      );

    OM.SaveMstDevOptn(OM.GetCrntDev());



}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::FormShow(TObject *Sender)
{
    //OM.LoadMstOptn(      );
    //OM.LoadEqpOptn(      );
#ifdef COWELL
    lbPreLoad    -> Caption = "Start Current" ;
    lbPeakStroke -> Caption = "Rated Stroke"  ;
    lbImpedance  -> Caption = "Resistance"    ;
#endif






    UpdateMstOptn   (toTabl);
    UpdateEqpOptn   (toTabl);
    UpdateMstDevOptn(toTabl);

    UpdateTimeName();
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateMstDevOptn(bool bTable)
{
    if(bTable){
        edGainPreLoad      -> Text = OM.MstDevOptn.dGainPreLoad     ; edOfstPreLoad      -> Text = OM.MstDevOptn.dOfstPreLoad     ;
        edGainPeakStroke   -> Text = OM.MstDevOptn.dGainPeakStroke  ; edOfstPeakStroke   -> Text = OM.MstDevOptn.dOfstPeakStroke  ;
        edGainCustomStroke -> Text = OM.MstDevOptn.dGainCustomStroke; edOfstCustomStroke -> Text = OM.MstDevOptn.dOfstCustomStroke;
        edGainSensitivity  -> Text = OM.MstDevOptn.dGainSensitivity ; edOfstSensitivity  -> Text = OM.MstDevOptn.dOfstSensitivity ;
        edGainImpedance    -> Text = OM.MstDevOptn.dGainImpedance   ; edOfstImpedance    -> Text = OM.MstDevOptn.dOfstImpedance   ;
        edGainHysteresis   -> Text = OM.MstDevOptn.dGainHysteresis  ; edOfstHysteresis   -> Text = OM.MstDevOptn.dOfstHysteresis  ;
        edGainLinearity    -> Text = OM.MstDevOptn.dGainLinearity   ; edOfstLinearity    -> Text = OM.MstDevOptn.dOfstLinearity   ;
    }
    else{
        OM.MstDevOptn.dGainPreLoad      = StrToFloatDef(edGainPreLoad      -> Text , OM.MstDevOptn.dGainPreLoad     ); OM.MstDevOptn.dOfstPreLoad      = StrToFloatDef(edOfstPreLoad      -> Text , OM.MstDevOptn.dOfstPreLoad     );
        OM.MstDevOptn.dGainPeakStroke   = StrToFloatDef(edGainPeakStroke   -> Text , OM.MstDevOptn.dGainPeakStroke  ); OM.MstDevOptn.dOfstPeakStroke   = StrToFloatDef(edOfstPeakStroke   -> Text , OM.MstDevOptn.dOfstPeakStroke  );
        OM.MstDevOptn.dGainCustomStroke = StrToFloatDef(edGainCustomStroke -> Text , OM.MstDevOptn.dGainCustomStroke); OM.MstDevOptn.dOfstCustomStroke = StrToFloatDef(edOfstCustomStroke -> Text , OM.MstDevOptn.dOfstCustomStroke);
        OM.MstDevOptn.dGainSensitivity  = StrToFloatDef(edGainSensitivity  -> Text , OM.MstDevOptn.dGainSensitivity ); OM.MstDevOptn.dOfstSensitivity  = StrToFloatDef(edOfstSensitivity  -> Text , OM.MstDevOptn.dOfstSensitivity );
        OM.MstDevOptn.dGainImpedance    = StrToFloatDef(edGainImpedance    -> Text , OM.MstDevOptn.dGainImpedance   ); OM.MstDevOptn.dOfstImpedance    = StrToFloatDef(edOfstImpedance    -> Text , OM.MstDevOptn.dOfstImpedance   );
        OM.MstDevOptn.dGainHysteresis   = StrToFloatDef(edGainHysteresis   -> Text , OM.MstDevOptn.dGainHysteresis  ); OM.MstDevOptn.dOfstHysteresis   = StrToFloatDef(edOfstHysteresis   -> Text , OM.MstDevOptn.dOfstHysteresis  );
        OM.MstDevOptn.dGainLinearity    = StrToFloatDef(edGainLinearity    -> Text , OM.MstDevOptn.dGainLinearity   ); OM.MstDevOptn.dOfstLinearity    = StrToFloatDef(edOfstLinearity    -> Text , OM.MstDevOptn.dOfstLinearity   );
        UpdateMstDevOptn(toTabl);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMaster::UpdateMstOptn(bool bTable)
{
    if(bTable){
        cbDebugMode        -> Checked = OM.MstOptn.bDebugMode     ;
        edVisnYMaxMin      -> Text    = OM.MstOptn.dVisnYMaxMin   ;

        edMstOfsX1         -> Text    = OM.MstDevOptn.dMstOfsX[0]    ;
        edMstOfsX2         -> Text    = OM.MstDevOptn.dMstOfsX[1]    ;
        edMstOfsX3         -> Text    = OM.MstDevOptn.dMstOfsX[2]    ;
        edMstOfsX4         -> Text    = OM.MstDevOptn.dMstOfsX[3]    ;
        edMstOfsX5         -> Text    = OM.MstDevOptn.dMstOfsX[4]    ;

        edMstOfsY1         -> Text    = OM.MstDevOptn.dMstOfsY[0]    ;
        edMstOfsY2         -> Text    = OM.MstDevOptn.dMstOfsY[1]    ;
        edMstOfsY3         -> Text    = OM.MstDevOptn.dMstOfsY[2]    ;
        edMstOfsY4         -> Text    = OM.MstDevOptn.dMstOfsY[3]    ;
        edMstOfsY5         -> Text    = OM.MstDevOptn.dMstOfsY[4]    ;

        cbSetUnitHeight    -> ItemIndex = OM.MstOptn.bSetUnitHeight ? 1 : 0 ;




    }
    else{
        OM.MstOptn.bDebugMode     =               cbDebugMode   -> Checked ;
        OM.MstOptn.dVisnYMaxMin   = StrToFloatDef(edVisnYMaxMin -> Text    , OM.MstOptn.dVisnYMaxMin  );

        OM.MstDevOptn.dMstOfsX[0]    = StrToFloatDef(edMstOfsX1    -> Text    , OM.MstDevOptn.dMstOfsX[0]   );
        OM.MstDevOptn.dMstOfsX[1]    = StrToFloatDef(edMstOfsX2    -> Text    , OM.MstDevOptn.dMstOfsX[1]   );
        OM.MstDevOptn.dMstOfsX[2]    = StrToFloatDef(edMstOfsX3    -> Text    , OM.MstDevOptn.dMstOfsX[2]   );
        OM.MstDevOptn.dMstOfsX[3]    = StrToFloatDef(edMstOfsX4    -> Text    , OM.MstDevOptn.dMstOfsX[3]   );
        OM.MstDevOptn.dMstOfsX[4]    = StrToFloatDef(edMstOfsX5    -> Text    , OM.MstDevOptn.dMstOfsX[4]   );

        OM.MstDevOptn.dMstOfsY[0]    = StrToFloatDef(edMstOfsY1    -> Text    , OM.MstDevOptn.dMstOfsY[0]   );
        OM.MstDevOptn.dMstOfsY[1]    = StrToFloatDef(edMstOfsY2    -> Text    , OM.MstDevOptn.dMstOfsY[1]   );
        OM.MstDevOptn.dMstOfsY[2]    = StrToFloatDef(edMstOfsY3    -> Text    , OM.MstDevOptn.dMstOfsY[2]   );
        OM.MstDevOptn.dMstOfsY[3]    = StrToFloatDef(edMstOfsY4    -> Text    , OM.MstDevOptn.dMstOfsY[3]   );
        OM.MstDevOptn.dMstOfsY[4]    = StrToFloatDef(edMstOfsY5    -> Text    , OM.MstDevOptn.dMstOfsY[4]   );

        OM.MstOptn.bSetUnitHeight =               cbSetUnitHeight-> ItemIndex ;

        UpdateMstOptn(toTabl);
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
    //for(int i= 0 ; i < MAX_PART ; i++) {
    //    if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Reset() ;
    //}


    //하... 좀 고민 해보자..
    if(cbPart[piSTG] -> Checked) {MM.SetManCycle(mcSTG_Home);}

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











void __fastcall TFrmMaster::Button10Click(TObject *Sender)
{
   String sTemp = Edit1 -> Text ;
   String sMsg ="";
   String sNo  ;

   /*
   STG.GetAsciiFromStringNo(sTemp , 4 , sNo);

   sMsg += (char)0x02 ;
   sMsg +=  sNo ;
   //sMsg += (char)0x30;
   //sMsg += (char)0x31;
   //sMsg += (char)0x30;
   sMsg += (char)0x03 ;
   //+ (char)0x03 ;
   STG.m_pVcmCon->SendData(sMsg.Length(),sMsg.c_str());
   */
   sMsg = STG.GetSendVcmConMsg(sTemp.ToIntDef(0)) ;
   STG.m_pVcmCon -> sData = "" ;
   STG.m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;


   //
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button11Click(TObject *Sender)
{
   String sMsg ="" ;

   sMsg += (char)0x02 ;
   //sMsg +=  sNo ;
   sMsg += (char)0x44;
   sMsg += (char)0x41;
   sMsg += (char)0x54;
   sMsg += (char)0x41;
   sMsg += (char)0x03 ;
   //+ (char)0x03 ;
   STG.m_pVcmCon->sData = "" ;
   STG.m_pVcmCon->SendData(sMsg.Length(),sMsg.c_str());
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button12Click(TObject *Sender)
{
    String sMsg = "MS,01";//\r\n"

    //sMsg += 0x0A ;
    sMsg += (char)0x0D ;




    STG.m_pHgtSnr -> SendData(sMsg.Length(),sMsg.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button13Click(TObject *Sender)
{
    String sMsg = Edit2 -> Text ;

    //sMsg += 0x0A ;
    sMsg += (char)0x0D ;




    STG.m_pHgtSnr -> SendData(sMsg.Length(),sMsg.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button14Click(TObject *Sender)
{
    String sTemp = STG.GetHeight(STG.m_pHgtSnr -> sData ) ;

    ShowMessage(sTemp);
}
//---------------------------------------------------------------------------






