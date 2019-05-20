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
#include "FormMain.h"
#include "Sequence.h"

#include "Loader.h"
#include "Picker.h"
#include "Sorter.h"
#include "Unloader.h"
#include "Rail.h"

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
    FraMotor[miPCK_Y]->SetIdType(miPCK_Y,mdtCwCCw_Z); FraMotor[miPCK_Y]->Parent = pnPCK_Y ;
    FraMotor[miRAL_X]->SetIdType(miRAL_X,mdtCCwCw_X); FraMotor[miRAL_X]->Parent = pnRAL_X ;
    FraMotor[miSRT_X]->SetIdType(miSRT_X,mdtCCwCw_X); FraMotor[miSRT_X]->Parent = pnSRT_X ;
    for(int i=0 ; i<MAX_MOTR ; i++) FraMotor[i]->Show();

    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i] = new TFraCyl();
    FraCylinder[aiLDR_SptrFB ]->SetIdType(aiLDR_SptrFB ,cdtFwBw_Y); FraCylinder[aiLDR_SptrFB ]->Parent = pnLDR_SptrFB  ;
    FraCylinder[aiLDR_SplyFB ]->SetIdType(aiLDR_SplyFB ,cdtBwFw_X); FraCylinder[aiLDR_SplyFB ]->Parent = pnLDR_SplyFB  ;
    FraCylinder[aiRAL_IndxUD ]->SetIdType(aiRAL_IndxUD ,cdtFwBw_Z); FraCylinder[aiRAL_IndxUD ]->Parent = pnRAL_IndxUD  ;
    FraCylinder[aiPCK_PckrDU ]->SetIdType(aiPCK_PckrDU ,cdtBwFw_Z); FraCylinder[aiPCK_PckrDU ]->Parent = pnPCK_PckrDU  ;
    FraCylinder[aiSTN_GuidFB ]->SetIdType(aiSTN_GuidFB ,cdtFwBw_X); FraCylinder[aiSTN_GuidFB ]->Parent = pnSTN_GuidFB  ;
    FraCylinder[aiSRT_Grip1FB]->SetIdType(aiSRT_Grip1FB,cdtBwFw_X); FraCylinder[aiSRT_Grip1FB]->Parent = pnSRT_Grip1FB ;
    FraCylinder[aiSRT_Grip2FB]->SetIdType(aiSRT_Grip2FB,cdtBwFw_X); FraCylinder[aiSRT_Grip2FB]->Parent = pnSRT_Grip2FB ;
    FraCylinder[aiSRT_Grip3FB]->SetIdType(aiSRT_Grip3FB,cdtBwFw_X); FraCylinder[aiSRT_Grip3FB]->Parent = pnSRT_Grip3FB ;
    FraCylinder[aiSRT_FlipFB ]->SetIdType(aiSRT_FlipFB ,cdtFwBw_Y); FraCylinder[aiSRT_FlipFB ]->Parent = pnSRT_FlipFB  ;
    FraCylinder[aiULD_StckUD ]->SetIdType(aiULD_StckUD ,cdtFwBw_Z); FraCylinder[aiULD_StckUD ]->Parent = pnULD_StckUD  ;
    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i]->Show();

    //Cylincder Button 1 버튼방식.

    btLDR_SplyFB  -> Tag = aiLDR_SplyFB  ;
    btLDR_SptrFB  -> Tag = aiLDR_SptrFB  ;
    btPCK_PckrDU  -> Tag = aiPCK_PckrDU  ;
    btRAL_IndxUD  -> Tag = aiRAL_IndxUD  ;
    btSTN_GuidFB  -> Tag = aiSTN_GuidFB  ;
    btSRT_FlipFB  -> Tag = aiSRT_FlipFB  ;
    btULD_StckUD  -> Tag = aiULD_StckUD  ;
    btSRT_Grip1FB -> Tag = aiSRT_Grip1FB ;
    btSRT_Grip2FB -> Tag = aiSRT_Grip2FB ;
    btSRT_Grip3FB -> Tag = aiSRT_Grip3FB ;

    //IO Num -> Tag
    btRAL_Out -> Tag = (int)yRAL_FeedingMt;

    btPckVac1 -> Tag = (int)yPCK_Vacuum1  ;
    btPckEjt1 -> Tag = (int)yPCK_VacEjt1  ;

    btPckVac2 -> Tag = (int)yPCK_Vacuum2  ;
    btPckEjt2 -> Tag = (int)yPCK_VacEjt2  ;

    btPckVac3 -> Tag = (int)yPCK_Vacuum3  ;
    btPckEjt3 -> Tag = (int)yPCK_VacEjt3  ;

    btSTN_Air -> Tag = (int)ySRT_StnAir   ;


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
    DM.ARAY[riPCK].SetMaxColRow(MAX_PICKER_CNT,1);
    DM.ARAY[riSTN].SetMaxColRow(MAX_PICKER_CNT,1);
    DM.ARAY[riSRT].SetMaxColRow(MAX_PICKER_CNT,1);

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

                 btRAL_Out -> Caption = IO_GetY(yRAL_FeedingMt) ? "ON" : "OFF" ; btRAL_Out -> Font -> Color = IO_GetY(yRAL_FeedingMt) ? clLime : clBlack ;
                 btPckVac1 -> Caption = IO_GetY(yPCK_Vacuum1  ) ? "ON" : "OFF" ; btPckVac1 -> Font -> Color = IO_GetY(yPCK_Vacuum1  ) ? clLime : clBlack ;
                 btPckVac2 -> Caption = IO_GetY(yPCK_Vacuum2  ) ? "ON" : "OFF" ; btPckVac2 -> Font -> Color = IO_GetY(yPCK_Vacuum2  ) ? clLime : clBlack ;
                 btPckVac3 -> Caption = IO_GetY(yPCK_Vacuum3  ) ? "ON" : "OFF" ; btPckVac3 -> Font -> Color = IO_GetY(yPCK_Vacuum3  ) ? clLime : clBlack ;
                 btPckEjt1 -> Caption = IO_GetY(yPCK_VacEjt1  ) ? "ON" : "OFF" ; btPckEjt1 -> Font -> Color = IO_GetY(yPCK_VacEjt1  ) ? clLime : clBlack ;
                 btPckEjt2 -> Caption = IO_GetY(yPCK_VacEjt2  ) ? "ON" : "OFF" ; btPckEjt2 -> Font -> Color = IO_GetY(yPCK_VacEjt2  ) ? clLime : clBlack ;
                 btPckEjt3 -> Caption = IO_GetY(yPCK_VacEjt3  ) ? "ON" : "OFF" ; btPckEjt3 -> Font -> Color = IO_GetY(yPCK_VacEjt3  ) ? clLime : clBlack ;
                 btSTN_Air -> Caption = IO_GetY(ySRT_StnAir   ) ? "ON" : "OFF" ; btSTN_Air -> Font -> Color = IO_GetY(ySRT_StnAir   ) ? clLime : clBlack ;

                 btLDR_SptrFB  -> Caption = AT_Complete(aiLDR_SptrFB  , ccFwd) ? "FWD" : "BWD" ;
                 btLDR_SplyFB  -> Caption = AT_Complete(aiLDR_SplyFB  , ccFwd) ? "FWD" : "BWD" ;
                 btRAL_IndxUD  -> Caption = AT_Complete(aiRAL_IndxUD  , ccFwd) ? "FWD" : "BWD" ;
                 btPCK_PckrDU  -> Caption = AT_Complete(aiPCK_PckrDU  , ccFwd) ? "FWD" : "BWD" ;
                 btSTN_GuidFB  -> Caption = AT_Complete(aiSTN_GuidFB  , ccFwd) ? "FWD" : "BWD" ;
                 btSRT_Grip1FB -> Caption = AT_Complete(aiSRT_Grip1FB , ccFwd) ? "FWD" : "BWD" ;
                 btSRT_Grip2FB -> Caption = AT_Complete(aiSRT_Grip2FB , ccFwd) ? "FWD" : "BWD" ;
                 btSRT_Grip3FB -> Caption = AT_Complete(aiSRT_Grip3FB , ccFwd) ? "FWD" : "BWD" ;
                 btSRT_FlipFB  -> Caption = AT_Complete(aiSRT_FlipFB  , ccFwd) ? "FWD" : "BWD" ;
                 btULD_StckUD  -> Caption = AT_Complete(aiULD_StckUD  , ccFwd) ? "FWD" : "BWD" ;

                 break;

        case 2 : //pnLBCnt0 -> Caption = SPL.GetPkgCnt(0);  pnLBCnt0 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[0] ] ;
                 //pnLBCnt1 -> Caption = SPL.GetPkgCnt(1);  pnLBCnt1 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[1] ] ;
                 //pnLBCnt2 -> Caption = SPL.GetPkgCnt(2);  pnLBCnt2 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[2] ] ;
                 //pnLBCnt3 -> Caption = SPL.GetPkgCnt(3);  pnLBCnt3 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[3] ] ;
                 //pnLBCnt4 -> Caption = SPL.GetPkgCnt(4);  pnLBCnt4 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[4] ] ;
                 //pnLBCnt5 -> Caption = SPL.GetPkgCnt(5);  pnLBCnt5 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[5] ] ;
                 //pnLBCnt6 -> Caption = SPL.GetPkgCnt(6);  pnLBCnt6 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[6] ] ;
                 //pnLBCnt7 -> Caption = SPL.GetPkgCnt(7);  pnLBCnt7 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[7] ] ;
                 //pnLBCnt8 -> Caption = SPL.GetPkgCnt(8);  pnLBCnt8 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[8] ] ;
                 //pnLBCnt9 -> Caption = SPL.GetPkgCnt(9);  pnLBCnt9 -> Color = clStat[(EN_CHIP_STAT)OM.DevOptn.iBnLIdx[9] ] ;

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
        cbBnL1Sel  ->ItemIndex = OM.DevOptn.iBnLSel[0] ;
        cbBnL2Sel  ->ItemIndex = OM.DevOptn.iBnLSel[1] ;
        cbBnL3Sel  ->ItemIndex = OM.DevOptn.iBnLSel[2] ;
        cbBnL4Sel  ->ItemIndex = OM.DevOptn.iBnLSel[3] ;
        cbBnL5Sel  ->ItemIndex = OM.DevOptn.iBnLSel[4] ;
        cbBnL6Sel  ->ItemIndex = OM.DevOptn.iBnLSel[5] ;
        cbBnL7Sel  ->ItemIndex = OM.DevOptn.iBnLSel[6] ;
        cbBnL8Sel  ->ItemIndex = OM.DevOptn.iBnLSel[7] ;
        cbBnL9Sel  ->ItemIndex = OM.DevOptn.iBnLSel[8] ;
        cbBnLASel  ->ItemIndex = OM.DevOptn.iBnLSel[9] ;

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

        edMasterOffset1     -> Text    = OM.DevOptn.dMasterOffset[0] ;
        edMasterOffset2     -> Text    = OM.DevOptn.dMasterOffset[1] ;
        edMasterOffset3     -> Text    = OM.DevOptn.dMasterOffset[2] ;



    }
    else
    {
        OM.DevOptn.iBnLSel[0] = cbBnL1Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[1] = cbBnL2Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[2] = cbBnL3Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[3] = cbBnL4Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[4] = cbBnL5Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[5] = cbBnL6Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[6] = cbBnL7Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[7] = cbBnL8Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[8] = cbBnL9Sel  ->ItemIndex;  
        OM.DevOptn.iBnLSel[9] = cbBnLASel  ->ItemIndex;

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

        OM.DevOptn.dMasterOffset[0]   = edMasterOffset1     -> Text.ToDouble( );
        OM.DevOptn.dMasterOffset[1]   = edMasterOffset2     -> Text.ToDouble( );
        OM.DevOptn.dMasterOffset[2]   = edMasterOffset3     -> Text.ToDouble( );




    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);
    tmUpdateTimer->Enabled = true;
    pcDeviceSet -> ActivePageIndex = 1 ;
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
void __fastcall TFrmDeviceSet::btLDR_SplyFBClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    if(iSel == aiLDR_SptrFB ) LDR.MoveActr(aiLDR_SptrFB , AT_Complete(aiLDR_SptrFB ,ccBwd));
    if(iSel == aiLDR_SplyFB ) LDR.MoveActr(aiLDR_SplyFB , AT_Complete(aiLDR_SplyFB ,ccBwd));
    if(iSel == aiRAL_IndxUD ) RAL.MoveActr(aiRAL_IndxUD , AT_Complete(aiRAL_IndxUD ,ccBwd));
    if(iSel == aiPCK_PckrDU ) PCK.MoveActr(aiPCK_PckrDU , AT_Complete(aiPCK_PckrDU ,ccBwd));
    if(iSel == aiSTN_GuidFB ) SRT.MoveActr(aiSTN_GuidFB , AT_Complete(aiSTN_GuidFB ,ccBwd));
    if(iSel == aiSRT_Grip1FB) SRT.MoveActr(aiSRT_Grip1FB, AT_Complete(aiSRT_Grip1FB,ccBwd));
    if(iSel == aiSRT_Grip2FB) SRT.MoveActr(aiSRT_Grip2FB, AT_Complete(aiSRT_Grip2FB,ccBwd));
    if(iSel == aiSRT_Grip3FB) SRT.MoveActr(aiSRT_Grip3FB, AT_Complete(aiSRT_Grip3FB,ccBwd));
    if(iSel == aiSRT_FlipFB ) SRT.MoveActr(aiSRT_FlipFB , AT_Complete(aiSRT_FlipFB ,ccBwd));
    if(iSel == aiULD_StckUD ) ULD.MoveActr(aiULD_StckUD , AT_Complete(aiULD_StckUD ,ccBwd));
}
//---------------------------------------------------------------------------

