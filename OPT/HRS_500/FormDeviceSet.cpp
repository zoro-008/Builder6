//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormMain.h"

#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "Loader.h"
#include "Supply.h"
#include "FormMain.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "LabelS"
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
    pcDeviceSet -> ActivePageIndex = 0 ;
    PM.SetWindow(pnLDR , miLDR_Z);
    PM.SetWindow(pnPCK , miPCK_Y);
    PM.SetWindow(pnRAL , miRAL_X);
    PM.SetWindow(pnSRT , miSRT_X);


    //Motor Window Set to Panel
    for(int i=0 ; i<MAX_MOTR ; i++) FraMotor[i] = new TFraMotr();
    FraMotor[miLDR_Z]->SetIdType(miLDR_Z,mdtCwCCw_Z); FraMotor[miLDR_Z]->Parent = pnLDR_Z ;
    FraMotor[miPCK_Y]->SetIdType(miPCK_Y,mdtCCwCw_Z); FraMotor[miPCK_Y]->Parent = pnPCK_Y ;
    FraMotor[miRAL_X]->SetIdType(miRAL_X,mdtCCwCw_X); FraMotor[miRAL_X]->Parent = pnRAL_X ;
    FraMotor[miSRT_X]->SetIdType(miSRT_X,mdtCCwCw_X); FraMotor[miSRT_X]->Parent = pnSRT_X ;
    for(int i=0 ; i<MAX_MOTR ; i++) FraMotor[i]->Show();



    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i] = new TFraCyl();
    //FraCylinder[aiLDR_Sptr ]->SetIdType(aiLDR_Sptr,cdtFwBw_Y); FraCylinder[aiLDR_Sptr ]->Parent = pnLDR_Sptr   ;
    //FraCylinder[aiPCK_UpDn ]->SetIdType(aiPCK_UpDn,cdtFwBw_Z); FraCylinder[aiPCK_UpDn ]->Parent = pnPCK_UpDn   ;
    //FraCylinder[aiRAL_Indx ]->SetIdType(aiRAL_Indx,cdtFwBw_Z); FraCylinder[aiRAL_Indx ]->Parent = pnRAL_Indx   ;
    //FraCylinder[aiSRT_Grip ]->SetIdType(aiSRT_Grip,cdtBwFw_Z); FraCylinder[aiSRT_Grip ]->Parent = pnSRT_Grip   ;
    //FraCylinder[aiSRT_Flip ]->SetIdType(aiSRT_Flip,cdtFwBw_Y); FraCylinder[aiSRT_Flip ]->Parent = pnSRT_Flip   ;
    //FraCylinder[aiULD_Stck ]->SetIdType(aiULD_Stck,cdtFwBw_Z); FraCylinder[aiULD_Stck ]->Parent = pnULD_Stck   ;
    FraCylinder[aiLDR_Sptr ]->SetIdType(aiLDR_Sptr,cdtFwBw_Y); FraCylinder[aiLDR_Sptr ]->Parent = pnLDR_Sptr   ;
    FraCylinder[aiPCK_UpDn ]->SetIdType(aiPCK_UpDn,cdtBwFw_Z); FraCylinder[aiPCK_UpDn ]->Parent = pnPCK_UpDn   ;
    FraCylinder[aiRAL_Indx ]->SetIdType(aiRAL_Indx,cdtFwBw_Z); FraCylinder[aiRAL_Indx ]->Parent = pnRAL_Indx   ;
    FraCylinder[aiSRT_Grip ]->SetIdType(aiSRT_Grip,cdtFwBw_Y); FraCylinder[aiSRT_Grip ]->Parent = pnSRT_Grip   ;
    FraCylinder[aiSRT_Flip ]->SetIdType(aiSRT_Flip,cdtFwBw_Y); FraCylinder[aiSRT_Flip ]->Parent = pnSRT_Flip   ;
    FraCylinder[aiULD_Stck ]->SetIdType(aiULD_Stck,cdtFwBw_Z); FraCylinder[aiULD_Stck ]->Parent = pnULD_Stck   ;
    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i]->Show();

    //IO Num -> Tag
    btRAL_Out   -> Tag = (int)yRAL_FeedingMt;
    BtVaccumIn  -> Tag = (int)yPCK_Vacuum   ;
    btVaccumOut -> Tag = (int)yPCK_VacEjt   ;


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;                  

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    //DataMan SetMaxColRow
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWKE].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPCK].SetMaxColRow(1,1);
    DM.ARAY[riSTN].SetMaxColRow(1,1);
    DM.ARAY[riSRT].SetMaxColRow(1,1);

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    btSaveDevice -> Enabled = !SEQ._bRun ;//&& !FrmMain -> tmMainSet -> Enabled && !FrmMain -> tmSubSet -> Enabled ;

    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :
        break;

        case 1 : //TODO ::

                 btRAL_Out  ->Caption = IO_GetY(yRAL_FeedingMt) ? "ON" : "OFF" ;
                 BtVaccumIn ->Caption = IO_GetY(yPCK_Vacuum   ) ? "ON" : "OFF" ;
                 btVaccumOut->Caption = IO_GetY(yPCK_VacEjt   ) ? "ON" : "OFF" ;

        break;

        case 2 :
                 cbBBnL1Sel -> Enabled = cbBnL1Sel -> ItemIndex == gsBuffer ; cbBBnR1Sel -> Enabled = cbBnR1Sel -> ItemIndex == gsBuffer ;
                 cbBBnL2Sel -> Enabled = cbBnL2Sel -> ItemIndex == gsBuffer ; cbBBnR2Sel -> Enabled = cbBnR2Sel -> ItemIndex == gsBuffer ;
                 cbBBnL3Sel -> Enabled = cbBnL3Sel -> ItemIndex == gsBuffer ; cbBBnR3Sel -> Enabled = cbBnR3Sel -> ItemIndex == gsBuffer ;
                 cbBBnL4Sel -> Enabled = cbBnL4Sel -> ItemIndex == gsBuffer ; cbBBnR4Sel -> Enabled = cbBnR4Sel -> ItemIndex == gsBuffer ;
                 cbBBnL5Sel -> Enabled = cbBnL5Sel -> ItemIndex == gsBuffer ; cbBBnR5Sel -> Enabled = cbBnR5Sel -> ItemIndex == gsBuffer ;
                 cbBBnL6Sel -> Enabled = cbBnL6Sel -> ItemIndex == gsBuffer ; cbBBnR6Sel -> Enabled = cbBnR6Sel -> ItemIndex == gsBuffer ;
                 cbBBnL7Sel -> Enabled = cbBnL7Sel -> ItemIndex == gsBuffer ; cbBBnR7Sel -> Enabled = cbBnR7Sel -> ItemIndex == gsBuffer ;
                 cbBBnL8Sel -> Enabled = cbBnL8Sel -> ItemIndex == gsBuffer ; cbBBnR8Sel -> Enabled = cbBnR8Sel -> ItemIndex == gsBuffer ;
                 cbBBnL9Sel -> Enabled = cbBnL9Sel -> ItemIndex == gsBuffer ; cbBBnR9Sel -> Enabled = cbBnR9Sel -> ItemIndex == gsBuffer ;
                 cbBBnLASel -> Enabled = cbBnLASel -> ItemIndex == gsBuffer ; cbBBnRASel -> Enabled = cbBnRASel -> ItemIndex == gsBuffer ;

                 pnLBCnt0 -> Caption = SPL.GetPkgCnt(0);  pnLBCnt0 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[0] ] ;
                 pnLBCnt1 -> Caption = SPL.GetPkgCnt(1);  pnLBCnt1 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[1] ] ;
                 pnLBCnt2 -> Caption = SPL.GetPkgCnt(2);  pnLBCnt2 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[2] ] ;
                 pnLBCnt3 -> Caption = SPL.GetPkgCnt(3);  pnLBCnt3 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[3] ] ;
                 pnLBCnt4 -> Caption = SPL.GetPkgCnt(4);  pnLBCnt4 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[4] ] ;
                 pnLBCnt5 -> Caption = SPL.GetPkgCnt(5);  pnLBCnt5 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[5] ] ;
                 pnLBCnt6 -> Caption = SPL.GetPkgCnt(6);  pnLBCnt6 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[6] ] ;
                 pnLBCnt7 -> Caption = SPL.GetPkgCnt(7);  pnLBCnt7 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[7] ] ;
                 pnLBCnt8 -> Caption = SPL.GetPkgCnt(8);  pnLBCnt8 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[8] ] ;
                 pnLBCnt9 -> Caption = SPL.GetPkgCnt(9);  pnLBCnt9 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[9] ] ;

                 pnRBCnt0 -> Caption = SPR.GetPkgCnt(0);  pnRBCnt0 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[0] ] ;
                 pnRBCnt1 -> Caption = SPR.GetPkgCnt(1);  pnRBCnt1 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[1] ] ;
                 pnRBCnt2 -> Caption = SPR.GetPkgCnt(2);  pnRBCnt2 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[2] ] ;
                 pnRBCnt3 -> Caption = SPR.GetPkgCnt(3);  pnRBCnt3 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[3] ] ;
                 pnRBCnt4 -> Caption = SPR.GetPkgCnt(4);  pnRBCnt4 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[4] ] ;
                 pnRBCnt5 -> Caption = SPR.GetPkgCnt(5);  pnRBCnt5 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[5] ] ;
                 pnRBCnt6 -> Caption = SPR.GetPkgCnt(6);  pnRBCnt6 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[6] ] ;
                 pnRBCnt7 -> Caption = SPR.GetPkgCnt(7);  pnRBCnt7 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[7] ] ;
                 pnRBCnt8 -> Caption = SPR.GetPkgCnt(8);  pnRBCnt8 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[8] ] ;
                 pnRBCnt9 -> Caption = SPR.GetPkgCnt(9);  pnRBCnt9 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnRIdx[9] ] ;
        break;

    }

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
    if(bTo)
    {
        edColPitch      -> Text = OM.DevInfo.dColPitch   ;
        edRowPitch      -> Text = OM.DevInfo.dRowPitch   ;
//        edColGroupCount -> Text = OM.DevInfo.iColGrCnt   ;
//        edRowGroupCount -> Text = OM.DevInfo.iRowGrCnt   ;

//        edColGroupGap   -> Text = OM.DevInfo.dColGrGap   ;
//        edRowGroupGap   -> Text = OM.DevInfo.dRowGrGap   ;

        edColCount      -> Text = OM.DevInfo.iColCnt     ;
        edRowCount      -> Text = OM.DevInfo.iRowCnt     ;

//        edCsSlotCnt     -> Text = OM.DevInfo.iCsSlCnt    ;
        edCsSlotPitch   -> Text = OM.DevInfo.dCsSlPitch  ;

    }
    else
    {
        OM.DevInfo.dColPitch  = edColPitch      -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch  = edRowPitch      -> Text.ToDouble() ;

//        OM.DevInfo.iColGrCnt  = edColGroupCount -> Text.ToInt()    ;
//        OM.DevInfo.iRowGrCnt  = edRowGroupCount -> Text.ToInt()    ;

//        OM.DevInfo.dColGrGap  = edColGroupGap   -> Text.ToDouble() ;
//        OM.DevInfo.dRowGrGap  = edRowGroupGap   -> Text.ToDouble() ;

        OM.DevInfo.iColCnt    = edColCount      -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt    = edRowCount      -> Text.ToInt()    ;

//        OM.DevInfo.iCsSlCnt   = edCsSlotCnt     -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch = edCsSlotPitch   -> Text.ToDouble() ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
    //컴포넌트 배열로 한다음에 다시 컴포넌트에 박는게 더 낳은가?
    //아 지금은 패스 하고 생각좀...
        cbBnL1Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[0] ;  cbBnR1Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[0] ;
        cbBnL2Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[1] ;  cbBnR2Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[1] ;
        cbBnL3Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[2] ;  cbBnR3Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[2] ;
        cbBnL4Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[3] ;  cbBnR4Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[3] ;
        cbBnL5Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[4] ;  cbBnR5Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[4] ;
        cbBnL6Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[5] ;  cbBnR6Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[5] ;
        cbBnL7Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[6] ;  cbBnR7Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[6] ;
        cbBnL8Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[7] ;  cbBnR8Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[7] ;
        cbBnL9Sel  ->ItemIndex = OM.DevOptn.iBnLIdx[8] ;  cbBnR9Sel  ->ItemIndex = OM.DevOptn.iBnRIdx[8] ;
        cbBnLASel  ->ItemIndex = OM.DevOptn.iBnLIdx[9] ;  cbBnRASel  ->ItemIndex = OM.DevOptn.iBnRIdx[9] ;

        edMinBn1  ->Text = OM.DevOptn.dMinBn[0] ;  edMaxBn1  ->Text = OM.DevOptn.dMaxBn[0] ;
        edMinBn2  ->Text = OM.DevOptn.dMinBn[1] ;  edMaxBn2  ->Text = OM.DevOptn.dMaxBn[1] ;
        edMinBn3  ->Text = OM.DevOptn.dMinBn[2] ;  edMaxBn3  ->Text = OM.DevOptn.dMaxBn[2] ;
        edMinBn4  ->Text = OM.DevOptn.dMinBn[3] ;  edMaxBn4  ->Text = OM.DevOptn.dMaxBn[3] ;
        edMinBn5  ->Text = OM.DevOptn.dMinBn[4] ;  edMaxBn5  ->Text = OM.DevOptn.dMaxBn[4] ;
        edMinBn6  ->Text = OM.DevOptn.dMinBn[5] ;  edMaxBn6  ->Text = OM.DevOptn.dMaxBn[5] ;
        edMinBn7  ->Text = OM.DevOptn.dMinBn[6] ;  edMaxBn7  ->Text = OM.DevOptn.dMaxBn[6] ;
        edMinBn8  ->Text = OM.DevOptn.dMinBn[7] ;  edMaxBn8  ->Text = OM.DevOptn.dMaxBn[7] ;
        edMinBn9  ->Text = OM.DevOptn.dMinBn[8] ;  edMaxBn9  ->Text = OM.DevOptn.dMaxBn[8] ;
        edMinBnA  ->Text = OM.DevOptn.dMinBn[9] ;  edMaxBnA  ->Text = OM.DevOptn.dMaxBn[9] ;

        edMinBn1S ->Text = OM.DevOptn.dMinBnS[0] ; edMaxBn1S ->Text = OM.DevOptn.dMaxBnS[0] ;
        edMinBn2S ->Text = OM.DevOptn.dMinBnS[1] ; edMaxBn2S ->Text = OM.DevOptn.dMaxBnS[1] ;
        edMinBn3S ->Text = OM.DevOptn.dMinBnS[2] ; edMaxBn3S ->Text = OM.DevOptn.dMaxBnS[2] ;
        edMinBn4S ->Text = OM.DevOptn.dMinBnS[3] ; edMaxBn4S ->Text = OM.DevOptn.dMaxBnS[3] ;
        edMinBn5S ->Text = OM.DevOptn.dMinBnS[4] ; edMaxBn5S ->Text = OM.DevOptn.dMaxBnS[4] ;
        edMinBn6S ->Text = OM.DevOptn.dMinBnS[5] ; edMaxBn6S ->Text = OM.DevOptn.dMaxBnS[5] ;
        edMinBn7S ->Text = OM.DevOptn.dMinBnS[6] ; edMaxBn7S ->Text = OM.DevOptn.dMaxBnS[6] ;
        edMinBn8S ->Text = OM.DevOptn.dMinBnS[7] ; edMaxBn8S ->Text = OM.DevOptn.dMaxBnS[7] ;
        edMinBn9S ->Text = OM.DevOptn.dMinBnS[8] ; edMaxBn9S ->Text = OM.DevOptn.dMaxBnS[8] ;
        edMinBnAS ->Text = OM.DevOptn.dMinBnS[9] ; edMaxBnAS ->Text = OM.DevOptn.dMaxBnS[9] ;

        cbBBnL1Sel  ->ItemIndex = OM.DevOptn.iBnLSel[0];  cbBBnR1Sel  ->ItemIndex = OM.DevOptn.iBnRSel[0];
        cbBBnL2Sel  ->ItemIndex = OM.DevOptn.iBnLSel[1];  cbBBnR2Sel  ->ItemIndex = OM.DevOptn.iBnRSel[1];
        cbBBnL3Sel  ->ItemIndex = OM.DevOptn.iBnLSel[2];  cbBBnR3Sel  ->ItemIndex = OM.DevOptn.iBnRSel[2];
        cbBBnL4Sel  ->ItemIndex = OM.DevOptn.iBnLSel[3];  cbBBnR4Sel  ->ItemIndex = OM.DevOptn.iBnRSel[3];
        cbBBnL5Sel  ->ItemIndex = OM.DevOptn.iBnLSel[4];  cbBBnR5Sel  ->ItemIndex = OM.DevOptn.iBnRSel[4];
        cbBBnL6Sel  ->ItemIndex = OM.DevOptn.iBnLSel[5];  cbBBnR6Sel  ->ItemIndex = OM.DevOptn.iBnRSel[5];
        cbBBnL7Sel  ->ItemIndex = OM.DevOptn.iBnLSel[6];  cbBBnR7Sel  ->ItemIndex = OM.DevOptn.iBnRSel[6];
        cbBBnL8Sel  ->ItemIndex = OM.DevOptn.iBnLSel[7];  cbBBnR8Sel  ->ItemIndex = OM.DevOptn.iBnRSel[7];
        cbBBnL9Sel  ->ItemIndex = OM.DevOptn.iBnLSel[8];  cbBBnR9Sel  ->ItemIndex = OM.DevOptn.iBnRSel[8];
        cbBBnLASel  ->ItemIndex = OM.DevOptn.iBnLSel[9];  cbBBnRASel  ->ItemIndex = OM.DevOptn.iBnRSel[9];

        edBinMaxCnt ->Text = OM.DevOptn.iBinMaxCnt;

    }
    else
    {
        OM.DevOptn.iBnLIdx[0] = cbBnL1Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[0] = cbBnR1Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[1] = cbBnL2Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[1] = cbBnR2Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[2] = cbBnL3Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[2] = cbBnR3Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[3] = cbBnL4Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[3] = cbBnR4Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[4] = cbBnL5Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[4] = cbBnR5Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[5] = cbBnL6Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[5] = cbBnR6Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[6] = cbBnL7Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[6] = cbBnR7Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[7] = cbBnL8Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[7] = cbBnR8Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[8] = cbBnL9Sel  ->ItemIndex;  OM.DevOptn.iBnRIdx[8] = cbBnR9Sel  ->ItemIndex;
        OM.DevOptn.iBnLIdx[9] = cbBnLASel  ->ItemIndex;  OM.DevOptn.iBnRIdx[9] = cbBnRASel  ->ItemIndex;

        OM.DevOptn.dMinBn[0]  = StrToFloatDef(edMinBn1 ->Text , 0.0); OM.DevOptn.dMaxBn[0]  = StrToFloatDef(edMaxBn1 ->Text , 0.0);
        OM.DevOptn.dMinBn[1]  = StrToFloatDef(edMinBn2 ->Text , 0.0); OM.DevOptn.dMaxBn[1]  = StrToFloatDef(edMaxBn2 ->Text , 0.0);
        OM.DevOptn.dMinBn[2]  = StrToFloatDef(edMinBn3 ->Text , 0.0); OM.DevOptn.dMaxBn[2]  = StrToFloatDef(edMaxBn3 ->Text , 0.0);
        OM.DevOptn.dMinBn[3]  = StrToFloatDef(edMinBn4 ->Text , 0.0); OM.DevOptn.dMaxBn[3]  = StrToFloatDef(edMaxBn4 ->Text , 0.0);
        OM.DevOptn.dMinBn[4]  = StrToFloatDef(edMinBn5 ->Text , 0.0); OM.DevOptn.dMaxBn[4]  = StrToFloatDef(edMaxBn5 ->Text , 0.0);
        OM.DevOptn.dMinBn[5]  = StrToFloatDef(edMinBn6 ->Text , 0.0); OM.DevOptn.dMaxBn[5]  = StrToFloatDef(edMaxBn6 ->Text , 0.0);
        OM.DevOptn.dMinBn[6]  = StrToFloatDef(edMinBn7 ->Text , 0.0); OM.DevOptn.dMaxBn[6]  = StrToFloatDef(edMaxBn7 ->Text , 0.0);
        OM.DevOptn.dMinBn[7]  = StrToFloatDef(edMinBn8 ->Text , 0.0); OM.DevOptn.dMaxBn[7]  = StrToFloatDef(edMaxBn8 ->Text , 0.0);
        OM.DevOptn.dMinBn[8]  = StrToFloatDef(edMinBn9 ->Text , 0.0); OM.DevOptn.dMaxBn[8]  = StrToFloatDef(edMaxBn9 ->Text , 0.0);
        OM.DevOptn.dMinBn[9]  = StrToFloatDef(edMinBnA ->Text , 0.0); OM.DevOptn.dMaxBn[9]  = StrToFloatDef(edMaxBnA ->Text , 0.0);

        OM.DevOptn.dMinBnS[0] = StrToFloatDef(edMinBn1S->Text , 0.0); OM.DevOptn.dMaxBnS[0] = StrToFloatDef(edMaxBn1S->Text , 0.0);
        OM.DevOptn.dMinBnS[1] = StrToFloatDef(edMinBn2S->Text , 0.0); OM.DevOptn.dMaxBnS[1] = StrToFloatDef(edMaxBn2S->Text , 0.0);
        OM.DevOptn.dMinBnS[2] = StrToFloatDef(edMinBn3S->Text , 0.0); OM.DevOptn.dMaxBnS[2] = StrToFloatDef(edMaxBn3S->Text , 0.0);
        OM.DevOptn.dMinBnS[3] = StrToFloatDef(edMinBn4S->Text , 0.0); OM.DevOptn.dMaxBnS[3] = StrToFloatDef(edMaxBn4S->Text , 0.0);
        OM.DevOptn.dMinBnS[4] = StrToFloatDef(edMinBn5S->Text , 0.0); OM.DevOptn.dMaxBnS[4] = StrToFloatDef(edMaxBn5S->Text , 0.0);
        OM.DevOptn.dMinBnS[5] = StrToFloatDef(edMinBn6S->Text , 0.0); OM.DevOptn.dMaxBnS[5] = StrToFloatDef(edMaxBn6S->Text , 0.0);
        OM.DevOptn.dMinBnS[6] = StrToFloatDef(edMinBn7S->Text , 0.0); OM.DevOptn.dMaxBnS[6] = StrToFloatDef(edMaxBn7S->Text , 0.0);
        OM.DevOptn.dMinBnS[7] = StrToFloatDef(edMinBn8S->Text , 0.0); OM.DevOptn.dMaxBnS[7] = StrToFloatDef(edMaxBn8S->Text , 0.0);
        OM.DevOptn.dMinBnS[8] = StrToFloatDef(edMinBn9S->Text , 0.0); OM.DevOptn.dMaxBnS[8] = StrToFloatDef(edMaxBn9S->Text , 0.0);
        OM.DevOptn.dMinBnS[9] = StrToFloatDef(edMinBnAS->Text , 0.0); OM.DevOptn.dMaxBnS[9] = StrToFloatDef(edMaxBnAS->Text , 0.0);

        OM.DevOptn.iBnLSel[0] = cbBBnL1Sel -> ItemIndex;  OM.DevOptn.iBnRSel[0] = cbBBnR1Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[1] = cbBBnL2Sel -> ItemIndex;  OM.DevOptn.iBnRSel[1] = cbBBnR2Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[2] = cbBBnL3Sel -> ItemIndex;  OM.DevOptn.iBnRSel[2] = cbBBnR3Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[3] = cbBBnL4Sel -> ItemIndex;  OM.DevOptn.iBnRSel[3] = cbBBnR4Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[4] = cbBBnL5Sel -> ItemIndex;  OM.DevOptn.iBnRSel[4] = cbBBnR5Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[5] = cbBBnL6Sel -> ItemIndex;  OM.DevOptn.iBnRSel[5] = cbBBnR6Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[6] = cbBBnL7Sel -> ItemIndex;  OM.DevOptn.iBnRSel[6] = cbBBnR7Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[7] = cbBBnL8Sel -> ItemIndex;  OM.DevOptn.iBnRSel[7] = cbBBnR8Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[8] = cbBBnL9Sel -> ItemIndex;  OM.DevOptn.iBnRSel[8] = cbBBnR9Sel -> ItemIndex;
        OM.DevOptn.iBnLSel[9] = cbBBnLASel -> ItemIndex;  OM.DevOptn.iBnRSel[9] = cbBBnRASel -> ItemIndex;

        OM.DevOptn.iBinMaxCnt = edBinMaxCnt ->Text.ToIntDef(0);

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);
    tmUpdateTimer->Enabled = true;
    pcDeviceSet -> ActivePageIndex = 0 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
//     switch ( pcDeviceSet -> ActivePageIndex ) {
//         case 0 :                             break ;
//         case 1 : rgUnitUser = rgLDRJogUnit ;
//                  edUnitUser = edLDRJogUnit ; break ;
//         case 2 : rgUnitUser = rgRALJogUnit ;
//                  edUnitUser = edRALJogUnit ; break ;
//         case 3 : rgUnitUser = rgWRKJogUnit ;
//                  edUnitUser = edWRKJogUnit ; break ;
//         case 4 : rgUnitUser = rgSTLJogUnit ;
//                  edUnitUser = edSTLJogUnit ; break ;
//         case 5 : rgUnitUser = rgULDJogUnit ;
//                  edUnitUser = edULDJogUnit ; break ;
//         case 6 :                             break ;
//     }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::rgRALJogUnitClick(TObject *Sender)
{
    if(rgRALJogUnit->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgRALJogUnit->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgRALJogUnit->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgRALJogUnit->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgRALJogUnit->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgRALJogUnit->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edRALJogUnit->Text,0.0));

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btRAL_OutClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  IO_GetY(iSel);
    IO_SetY(iSel , !bState);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::edRALJogUnitChange(TObject *Sender)
{
    if(rgRALJogUnit->ItemIndex == 5)  for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edRALJogUnit->Text,0.0));
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdateTimer->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::BitBtn1Click(TObject *Sender)
{
    for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
        AT_MoveCyl(aiSPL_LBn0 + i , ccFwd);
        AT_MoveCyl(aiSPL_RBn0 + i , ccFwd);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::BitBtn2Click(TObject *Sender)
{
    for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
        AT_MoveCyl(aiSPL_LBn0 + i , ccBwd);
        AT_MoveCyl(aiSPL_RBn0 + i , ccBwd);
    }
}
//---------------------------------------------------------------------------

