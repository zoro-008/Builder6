//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMaster.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "LoaderSupply.h"
#include "LoaderStock.h"
#include "Stage.h"
#include "StageVT.h"
#include "StageOST.h"
#include "StageVision.h"
#include "Sort.h"

#include "Unloader.h"
#include "UnloaderSupply.h"
#include "UnloaderStock.h"
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
        cbVacErrIgnr       -> Checked = OM.MstOptn.bVacErrIgnr        ;
        edInspRandMask     -> Text    = OM.MstOptn.iInspRandMask      ;


        edTrigerOffset     -> Text    = OM.MstOptn.dTrigerOffset      ;

        edVisnRsltFile1    -> Text    = OM.MstOptn.sVisnRsltFile1     ;
        edVisnRsltFile2    -> Text    = OM.MstOptn.sVisnRsltFile2     ;
        edVisnDeviceFile   -> Text    = OM.MstOptn.sVisnDeviceFile    ;
        edVisnLotNoFile    -> Text    = OM.MstOptn.sVisnLotNoFile     ;
        edVTDataFile1      -> Text    = OM.MstOptn.sVTDataFile1       ;
        edVTDataFile2      -> Text    = OM.MstOptn.sVTDataFile2       ;
        edVTDataFile3      -> Text    = OM.MstOptn.sVTDataFile3       ;
        edVTDataFile4      -> Text    = OM.MstOptn.sVTDataFile4       ;

        edOSTOffset1       -> Text    = OM.MstOptn.dOSTOffset1        ;
        edOSTOffset2       -> Text    = OM.MstOptn.dOSTOffset2        ;
        edOSTOffset3       -> Text    = OM.MstOptn.dOSTOffset3        ;
        edOSTOffset4       -> Text    = OM.MstOptn.dOSTOffset4        ;
        edOSTGain1         -> Text    = OM.MstOptn.dOSTGain1          ;
        edOSTGain2         -> Text    = OM.MstOptn.dOSTGain2          ;
        edOSTGain3         -> Text    = OM.MstOptn.dOSTGain3          ;
        edOSTGain4         -> Text    = OM.MstOptn.dOSTGain4          ;
    }
    else{
        OM.MstOptn.bDebugMode         = cbDebugMode         -> Checked ;
        OM.MstOptn.bVacErrIgnr        = cbVacErrIgnr        -> Checked ;
        OM.MstOptn.sVisnRsltFile1     = edVisnRsltFile1     -> Text    ;
        OM.MstOptn.sVisnRsltFile2     = edVisnRsltFile2     -> Text    ;
        OM.MstOptn.sVisnDeviceFile    = edVisnDeviceFile    -> Text    ;
        OM.MstOptn.sVisnLotNoFile     = edVisnLotNoFile     -> Text    ;
        OM.MstOptn.sVTDataFile1       = edVTDataFile1       -> Text    ;
        OM.MstOptn.sVTDataFile2       = edVTDataFile2       -> Text    ;
        OM.MstOptn.sVTDataFile3       = edVTDataFile3       -> Text    ;
        OM.MstOptn.sVTDataFile4       = edVTDataFile4       -> Text    ;

        OM.MstOptn.dOSTOffset1        = StrToFloatDef(edOSTOffset1 -> Text,0.0) ;
        OM.MstOptn.dOSTOffset2        = StrToFloatDef(edOSTOffset2 -> Text,0.0) ;
        OM.MstOptn.dOSTOffset3        = StrToFloatDef(edOSTOffset3 -> Text,0.0) ;
        OM.MstOptn.dOSTOffset4        = StrToFloatDef(edOSTOffset4 -> Text,0.0) ;
        OM.MstOptn.dOSTGain1          = StrToFloatDef(edOSTGain1   -> Text,1.0) ;
        OM.MstOptn.dOSTGain2          = StrToFloatDef(edOSTGain2   -> Text,1.0) ;
        OM.MstOptn.dOSTGain3          = StrToFloatDef(edOSTGain3   -> Text,1.0) ;
        OM.MstOptn.dOSTGain4          = StrToFloatDef(edOSTGain4   -> Text,1.0) ;

        OM.MstOptn.dTrigerOffset      = StrToFloatDef(edTrigerOffset -> Text,0.0) ;
        OM.MstOptn.iInspRandMask      = StrToIntDef  (edInspRandMask -> Text,0  ) ;


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





void __fastcall TFrmMaster::btVisnRsltFile1Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVisnRsltFile1->Text = dgFilePath->FileName;


}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button1Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVTDataFile1 ->Text = dgFilePath->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::btPartHomeClick(TObject *Sender)
{
    //for(int i= 0 ; i < MAX_PART ; i++) {
    //    if(cbPart[i] -> Checked) SEQ.m_pPart[i] -> Reset() ;
    //}


    //하... 좀 고민 해보자..
    if(cbPart[piLSP] -> Checked) {MM.SetManCycle(mcLSP_Home);}
    if(cbPart[piLDR] -> Checked) {MM.SetManCycle(mcLDR_Home);}
    if(cbPart[piLST] -> Checked) {MM.SetManCycle(mcLST_Home);}
    if(cbPart[piSRT] -> Checked) {MM.SetManCycle(mcSRT_Home);}
    if(cbPart[piSTG] -> Checked) {MM.SetManCycle(mcSTG_Home);}
    if(cbPart[piOST] -> Checked) {MM.SetManCycle(mcOST_Home);}
    if(cbPart[piVTI] -> Checked) {MM.SetManCycle(mcVTI_Home);}
    if(cbPart[piVSN] -> Checked) {MM.SetManCycle(mcVSN_Home);}
    if(cbPart[piUSP] -> Checked) {MM.SetManCycle(mcUSP_Home);}
    if(cbPart[piULD] -> Checked) {MM.SetManCycle(mcULD_Home);}
    if(cbPart[piUST] -> Checked) {MM.SetManCycle(mcUST_Home);}

}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button3Click(TObject *Sender)
{
    IO_SetY(yVSN_TrgRellay1 , true);
    IO_SetY(yVSN_TrgRellay2 , true);

    ::Sleep(100);

    MT_OneShotAxtTrg(miSTG_YVisn , true , 1000);

    IO_SetY(yVSN_TrgRellay1 , false);
    IO_SetY(yVSN_TrgRellay2 , false);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button2Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVisnDeviceFile->Text = dgFilePath->FileName;
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



void __fastcall TFrmMaster::btVisnRsltFile2Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVisnRsltFile2->Text = dgFilePath->FileName;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button21Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVTDataFile2 ->Text = dgFilePath->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button22Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVTDataFile3 ->Text = dgFilePath->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button23Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVTDataFile4 ->Text = dgFilePath->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button4Click(TObject *Sender)
{
    if(!dgFilePath->Execute())    return ;
    edVisnLotNoFile->Text = dgFilePath->FileName;        
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::tmOSTMsgTimer(TObject *Sender)
{
    tmOSTMsg -> Enabled = false ;

    Memo1 -> Text = OST.m_pRs232_OST -> sData ;

    String sOST ;
    String sIDD ;
    String sIDS ;

    String sTotal ="";



    for(int a = 0 ; a < MAX_TOOL_SOCK_COL ; a++) {
        sOST = String(a+1) + "OST:";
        for(int i = 0 ; i < MAX_OSP_PIN ; i++) {
            sOST+=OST.OSTRslt[a].bRsltOSTFail[i] ? "1":"0" ;
        }

        sIDD = String(a+1) + "IDD:";
        sIDD += " (AVDD)"; sIDD += OST.OSTRslt[a].dRsltAVDD_IDD ;
        sIDD += " (CVDD)"; sIDD += OST.OSTRslt[a].dRsltCVDD_IDD ;
        sIDD += " (HVDD)"; sIDD += OST.OSTRslt[a].dRsltHVDD_IDD ;
        sIDD += " (DVDD)"; sIDD += OST.OSTRslt[a].dRsltDVDD_IDD ;

        sIDS = String(a+1) + "IDS:";
        sIDS += " (AVDD)"; sIDS += OST.OSTRslt[a].dRsltAVDD_IDS ;
        sIDS += " (CVDD)"; sIDS += OST.OSTRslt[a].dRsltCVDD_IDS ;
        sIDS += " (HVDD)"; sIDS += OST.OSTRslt[a].dRsltHVDD_IDS ;
        sIDS += " (DVDD)"; sIDS += OST.OSTRslt[a].dRsltDVDD_IDS ;

        sTotal += sOST + "\r\n" +
                  sIDD + "\r\n" +
                  sIDS + "\r\n" ;
    }

    Memo2 -> Text = sTotal ;

    tmOSTMsg -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button5Click(TObject *Sender)
{








    String sCmd = Edit3 -> Text  ;

    //sCmd = "[DVC,1#" ;
    //sCmd = "[DVC,1#" ;
    //sCmd = "[DVC,1#" ;

    OST.m_pRs232_OST -> sData = "";

    OST.m_pRs232_OST -> SendData(sCmd.Length() , sCmd.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::BitBtn1Click(TObject *Sender)
{
    bool bRet ;
    OST.GetOSTFail(0,OST.m_pRs232_OST -> sData,bRet);
    OST.GetOSTFail(1,OST.m_pRs232_OST -> sData,bRet);
    OST.GetOSTFail(2,OST.m_pRs232_OST -> sData,bRet);
    OST.GetOSTFail(3,OST.m_pRs232_OST -> sData,bRet);

    OST.GetIDDFail(0,OST.m_pRs232_OST -> sData,bRet);
    OST.GetIDDFail(1,OST.m_pRs232_OST -> sData,bRet);
    OST.GetIDDFail(2,OST.m_pRs232_OST -> sData,bRet);
    OST.GetIDDFail(3,OST.m_pRs232_OST -> sData,bRet);

    OST.GetIDSFail(0,OST.m_pRs232_OST -> sData,bRet);
    OST.GetIDSFail(1,OST.m_pRs232_OST -> sData,bRet);
    OST.GetIDSFail(2,OST.m_pRs232_OST -> sData,bRet);
    OST.GetIDSFail(3,OST.m_pRs232_OST -> sData,bRet);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button6Click(TObject *Sender)
{
 AnsiString sTemp =
//"]OST1:00000,06451,00511\r\n]IDD1:1504,2192,0031,1613\r\n]IDS1:1503,2188,0029,1612\r\n]OST2:00000,00000,00000\r\n]IDD2:0001,0000,0000,0000\r\n]IDS2:0000,0000,0000,0000\r\n]OST3:00000,00000,00000\r\n]IDD3:0000,0000,0000,0000\r\n]IDS3:0000,0000,0000,0000\r\n]OST4:00000,00000,00000\r\n]IDD4:0000,0000,0000,0000\r\n]IDS4:0000,0000,0000,0000\r\n#";
"]OST1:00000,00000,00000\r\n]IDD1:0040,0017,0070,0000\r\n]IDS1:0002,0022,0145,0021\r\n]OST2:00000,00000,00000\r\n]IDD2:0036,0017,0067,0000\r\n]IDS2:0001,0013,0147,0025\r\n]OST3:00000,00000,00000\r\n]IDD3:0030,0014,0063,0000\r\n]IDS3:0001,0015,0180,0013\r\n]OST4:00000,00000,00000\r\n]IDD4:0038,0016,0072,0000\r\n]IDS4:0002,0019,0139,0023\r\n#";











    OST.m_pRs232_OST -> sData = sTemp ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Memo1Change(TObject *Sender)
{
    OST.m_pRs232_OST -> sData =  Memo1 -> Text ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMaster::Button8Click(TObject *Sender)
{
    MM.SetManCycle(mcOST_CycleTest);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMaster::Button9Click(TObject *Sender)
{
    String sMsg = "[DVC,"+AnsiString(OM.DevOptn.iOSTDeviceNo)+"#" ;
    OST.m_pRs232_OST -> SendData(sMsg.Length(),sMsg.c_str());
}
//---------------------------------------------------------------------------

