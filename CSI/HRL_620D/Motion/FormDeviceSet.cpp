
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Filectrl.hpp>
#pragma hdrstop

#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "OptionMan.h"
//#include "DataMan.h"
#include "SLogUnit.h"
#include "LotUnit.h"
#include "PstnMan.h"
#include "UserFile.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
    //Set String Grid Width.


    PM.SetWindow(pnBaseT1 , miSTG_TBase);
    PM.SetWindow(pnBaseT2 , miSTG_TBase);
    PM.SetWindow(pnBrd1U  , miWRK_UBrd1);
    PM.SetWindow(pnBrd1V  , miWRK_VBrd1);
    PM.SetWindow(pnBrd2U  , miWRK_UBrd2);
    PM.SetWindow(pnBrd2V  , miWRK_VBrd2);

    btBrd1VccL   -> Tag = (int)yWRK_Brd1VccL    ;
    btBrd1VccR   -> Tag = (int)yWRK_Brd1VccR    ;
    btBrd2VccL   -> Tag = (int)yWRK_Brd2VccL    ;
    btBrd2VccR   -> Tag = (int)yWRK_Brd2VccR    ;
    btDstCollct1 -> Tag = (int)yETC_LaserWrkVcc ;
    btDstCollct2 -> Tag = (int)yETC_LaserWrkVcc ;

    SetUnitInit();
    MtWindowSet();

    rgUnitUser = new TRadioGroup(this);
    edUnitUser = new TEdit      (this);


    cbBrd1U_1st -> Enabled = false;
    cbBrd2U_1st -> Enabled = false;
    cbBrd2U_2nd -> Enabled = false;
    cbBrd2U_3rd -> Enabled = false;
    cbBrd2U_4th -> Enabled = false;
    cbBrd2U_5th -> Enabled = false;
    cbBrd2U_6th -> Enabled = false;
    cbBrd2U_7th -> Enabled = false;
    cbBrd2U_8th -> Enabled = false;
    cbBrd2U_9th -> Enabled = false;
    cbBrd2U_10th-> Enabled = false;

    tmUpdate -> Enabled = true;


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::EditEnable()
{

    if(!OM.DevOptn.bUsePstU1 ){ cbBrd2U_1st -> Checked = false; }
    else                      { cbBrd2U_1st -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU2 ){ cbBrd2U_2nd -> Checked = false; }
    else                      { cbBrd2U_2nd -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU3 ){ cbBrd2U_3rd -> Checked = false; }
    else                      { cbBrd2U_3rd -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU4 ){ cbBrd2U_4th -> Checked = false; }
    else                      { cbBrd2U_4th -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU5 ){ cbBrd2U_5th -> Checked = false; }
    else                      { cbBrd2U_5th -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU6 ){ cbBrd2U_6th -> Checked = false; }
    else                      { cbBrd2U_6th -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU7 ){ cbBrd2U_7th -> Checked = false; }
    else                      { cbBrd2U_7th -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU8 ){ cbBrd2U_8th -> Checked = false; }
    else                      { cbBrd2U_8th -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU9 ){ cbBrd2U_9th -> Checked = false; }
    else                      { cbBrd2U_9th -> Checked = true ; }
    if(!OM.DevOptn.bUsePstU10){ cbBrd2U_10th-> Checked = false; }
    else                      { cbBrd2U_10th-> Checked = true ; }


    return;
}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MtWindowSet()
{

    for(int i=0 ; i<MAX_MOTR ; i++) {
        FraMotor  [i] = new TFraMotr();
        FraMotor  [i] ->SetIdType(i ,MT_GetDirType(i));
        FraMotor  [i] ->Show();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    tmUpdate -> Enabled = true;

    PM.Init();
    EditEnable();
    PM.UpdatePstn(toTabl);
    UpdateDevOptn(toTabl);



    PM.SetParent(pnBaseT1 , miSTG_TBase) ;
    FraMotor[miSTG_TBase]->Parent = pnTBase1 ;
    FraMotor[miWRK_UBrd1]->Parent = pnUBrd1  ;
    FraMotor[miWRK_VBrd1]->Parent = pnVBrd1  ;

    pcDeviceSet -> ActivePage = TabSheet3;

    switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 : rgUnitUser = rgJogUnit1 ;
                  edUnitUser = edJogUnit1 ; break ;
         case 1 : rgUnitUser = rgJogUnit2 ;
                  edUnitUser = edJogUnit2 ; break ;
     }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}


//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        cbBrd1U_1st -> Checked = OM.DevOptn.bUsePstU1 ;
        cbBrd1U_2nd -> Checked = OM.DevOptn.bUsePstU2 ;
        cbBrd1U_3rd -> Checked = OM.DevOptn.bUsePstU3 ;
        cbBrd1U_4th -> Checked = OM.DevOptn.bUsePstU4 ;
        cbBrd1U_5th -> Checked = OM.DevOptn.bUsePstU5 ;
        cbBrd1U_6th -> Checked = OM.DevOptn.bUsePstU6 ;
        cbBrd1U_7th -> Checked = OM.DevOptn.bUsePstU7 ;
        cbBrd1U_8th -> Checked = OM.DevOptn.bUsePstU8 ;
        cbBrd1U_9th -> Checked = OM.DevOptn.bUsePstU9 ;
        cbBrd1U_10th-> Checked = OM.DevOptn.bUsePstU10;

        edLsrDev_Brd1U_1st -> Text = OM.DevOptn.iOriLsrDevU1 ;
        edLsrDev_Brd1U_2nd -> Text = OM.DevOptn.iOriLsrDevU2 ;
        edLsrDev_Brd1U_3rd -> Text = OM.DevOptn.iOriLsrDevU3 ;
        edLsrDev_Brd1U_4th -> Text = OM.DevOptn.iOriLsrDevU4 ;
        edLsrDev_Brd1U_5th -> Text = OM.DevOptn.iOriLsrDevU5 ;
        edLsrDev_Brd1U_6th -> Text = OM.DevOptn.iOriLsrDevU6 ;
        edLsrDev_Brd1U_7th -> Text = OM.DevOptn.iOriLsrDevU7 ;
        edLsrDev_Brd1U_8th -> Text = OM.DevOptn.iOriLsrDevU8 ;
        edLsrDev_Brd1U_9th -> Text = OM.DevOptn.iOriLsrDevU9 ;
        edLsrDev_Brd1U_10th-> Text = OM.DevOptn.iOriLsrDevU10;

        edLsrDev_Brd2U_1st -> Text = OM.DevOptn.iRvsLsrDevU1 ;
        edLsrDev_Brd2U_2nd -> Text = OM.DevOptn.iRvsLsrDevU2 ;
        edLsrDev_Brd2U_3rd -> Text = OM.DevOptn.iRvsLsrDevU3 ;
        edLsrDev_Brd2U_4th -> Text = OM.DevOptn.iRvsLsrDevU4 ;
        edLsrDev_Brd2U_5th -> Text = OM.DevOptn.iRvsLsrDevU5 ;
        edLsrDev_Brd2U_6th -> Text = OM.DevOptn.iRvsLsrDevU6 ;
        edLsrDev_Brd2U_7th -> Text = OM.DevOptn.iRvsLsrDevU7 ;
        edLsrDev_Brd2U_8th -> Text = OM.DevOptn.iRvsLsrDevU8 ;
        edLsrDev_Brd2U_9th -> Text = OM.DevOptn.iRvsLsrDevU9 ;
        edLsrDev_Brd2U_10th-> Text = OM.DevOptn.iRvsLsrDevU10;

    }
    else
    {
        int        iTemp ; //비교 하기 위해서..JH

        iTemp = cbBrd1U_1st -> Checked ; if (OM.DevOptn.bUsePstU1 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU1  values are different."  ); }
        iTemp = cbBrd1U_2nd -> Checked ; if (OM.DevOptn.bUsePstU2 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU2  values are different."  ); }
        iTemp = cbBrd1U_3rd -> Checked ; if (OM.DevOptn.bUsePstU3 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU3  values are different."  ); }
        iTemp = cbBrd1U_4th -> Checked ; if (OM.DevOptn.bUsePstU4 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU4  values are different."  ); }
        iTemp = cbBrd1U_5th -> Checked ; if (OM.DevOptn.bUsePstU5 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU5  values are different."  ); }
        iTemp = cbBrd1U_6th -> Checked ; if (OM.DevOptn.bUsePstU6 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU6  values are different."  ); }
        iTemp = cbBrd1U_7th -> Checked ; if (OM.DevOptn.bUsePstU7 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU7  values are different."  ); }
        iTemp = cbBrd1U_8th -> Checked ; if (OM.DevOptn.bUsePstU8 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU8  values are different."  ); }
        iTemp = cbBrd1U_9th -> Checked ; if (OM.DevOptn.bUsePstU9 != iTemp) { Trace("Operator" , "Both Dev_bUsePstU9  values are different."  ); }
        iTemp = cbBrd1U_10th-> Checked ; if (OM.DevOptn.bUsePstU10!= iTemp) { Trace("Operator" , "Both Dev_bUsePstU10 values are different."  ); }

        AnsiString dddd = edLsrDev_Brd1U_1st -> Text.ToIntDef(0);

        iTemp = edLsrDev_Brd1U_1st -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU1 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU1  values are different."  ); }
        iTemp = edLsrDev_Brd1U_2nd -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU2 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU2  values are different."  ); }
        iTemp = edLsrDev_Brd1U_3rd -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU3 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU3  values are different."  ); }
        iTemp = edLsrDev_Brd1U_4th -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU4 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU4  values are different."  ); }
        iTemp = edLsrDev_Brd1U_5th -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU5 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU5  values are different."  ); }
        iTemp = edLsrDev_Brd1U_6th -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU6 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU6  values are different."  ); }
        iTemp = edLsrDev_Brd1U_7th -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU7 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU7  values are different."  ); }
        iTemp = edLsrDev_Brd1U_8th -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU8 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU8  values are different."  ); }
        iTemp = edLsrDev_Brd1U_9th -> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU9 != iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU9  values are different."  ); }
        iTemp = edLsrDev_Brd1U_10th-> Text.ToIntDef(0) ; if (OM.DevOptn.iOriLsrDevU10!= iTemp) { Trace("Operator" , "Both Dev_iOriLsrDevU10 values are different."  ); }

        iTemp = edLsrDev_Brd2U_1st -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU1 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU1  values are different."  ); }
        iTemp = edLsrDev_Brd2U_2nd -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU2 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU2  values are different."  ); }
        iTemp = edLsrDev_Brd2U_3rd -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU3 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU3  values are different."  ); }
        iTemp = edLsrDev_Brd2U_4th -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU4 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU4  values are different."  ); }
        iTemp = edLsrDev_Brd2U_5th -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU5 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU5  values are different."  ); }
        iTemp = edLsrDev_Brd2U_6th -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU6 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU6  values are different."  ); }
        iTemp = edLsrDev_Brd2U_7th -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU7 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU7  values are different."  ); }
        iTemp = edLsrDev_Brd2U_8th -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU8 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU8  values are different."  ); }
        iTemp = edLsrDev_Brd2U_9th -> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU9 != iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU9  values are different."  ); }
        iTemp = edLsrDev_Brd2U_10th-> Text.ToIntDef(0) ; if (OM.DevOptn.iRvsLsrDevU10!= iTemp) { Trace("Operator" , "Both Dev_iRvsLsrDevU10 values are different."  ); }

        OM.DevOptn.iOriLsrDevU1 = edLsrDev_Brd1U_1st -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU2 = edLsrDev_Brd1U_2nd -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU3 = edLsrDev_Brd1U_3rd -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU4 = edLsrDev_Brd1U_4th -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU5 = edLsrDev_Brd1U_5th -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU6 = edLsrDev_Brd1U_6th -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU7 = edLsrDev_Brd1U_7th -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU8 = edLsrDev_Brd1U_8th -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU9 = edLsrDev_Brd1U_9th -> Text.ToIntDef(0) ;
        OM.DevOptn.iOriLsrDevU10= edLsrDev_Brd1U_10th-> Text.ToIntDef(0) ;

        OM.DevOptn.iRvsLsrDevU1 = edLsrDev_Brd2U_1st -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU2 = edLsrDev_Brd2U_2nd -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU3 = edLsrDev_Brd2U_3rd -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU4 = edLsrDev_Brd2U_4th -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU5 = edLsrDev_Brd2U_5th -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU6 = edLsrDev_Brd2U_6th -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU7 = edLsrDev_Brd2U_7th -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU8 = edLsrDev_Brd2U_8th -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU9 = edLsrDev_Brd2U_9th -> Text.ToIntDef(0) ;
        OM.DevOptn.iRvsLsrDevU10= edLsrDev_Brd2U_10th-> Text.ToIntDef(0) ;

        OM.DevOptn.bUsePstU1 = cbBrd1U_1st -> Checked   ;
        OM.DevOptn.bUsePstU2 = cbBrd1U_2nd -> Checked   ;
        OM.DevOptn.bUsePstU3 = cbBrd1U_3rd -> Checked   ;
        OM.DevOptn.bUsePstU4 = cbBrd1U_4th -> Checked   ;
        OM.DevOptn.bUsePstU5 = cbBrd1U_5th -> Checked   ;
        OM.DevOptn.bUsePstU6 = cbBrd1U_6th -> Checked   ;
        OM.DevOptn.bUsePstU7 = cbBrd1U_7th -> Checked   ;
        OM.DevOptn.bUsePstU8 = cbBrd1U_8th -> Checked   ;
        OM.DevOptn.bUsePstU9 = cbBrd1U_9th -> Checked   ;
        OM.DevOptn.bUsePstU10= cbBrd1U_10th-> Checked   ;
    }
    EditEnable();
}

void __fastcall TFrmDeviceSet::btSaveDeviceClick(TObject *Sender)
{
    if(!OM.DevOptn.bUsePstU1 && !OM.DevOptn.bUsePstU2 && !OM.DevOptn.bUsePstU3 &&
       !OM.DevOptn.bUsePstU4 && !OM.DevOptn.bUsePstU5 && !OM.DevOptn.bUsePstU6 ){
        FM_MsgTime("Warning" , "사용하는 포지션을 최소 1개 이상 선택 해 주세요." , 2000); return;
    }

    if (FM_MsgYesNo("Confirm" , "저장 하시겠습니까?") != mrYes)  return;
    pcDeviceSet -> Hide();
    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    PM.Init();

    UpdateDevOptn(toTabl);
    PM.UpdatePstn(toTabl);
    EditEnable();

    pcDeviceSet -> Show();
    Trace("Operator", "Device Set Form_Save Button Click");

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btBrd1VccLClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iSel , !IO_GetY(iSel));

         if(iSel == 4) IO_SetY(iSel , !IO_GetY(iSel));
    else if(iSel == 5) IO_SetY(iSel , !IO_GetY(iSel));
    else if(iSel == 6) IO_SetY(iSel , !IO_GetY(iSel));
    else if(iSel == 7) IO_SetY(iSel , !IO_GetY(iSel));
    else if(iSel == 8) IO_SetY(iSel , !IO_GetY(iSel));

    /*
         if(iTag==0){ if(!IO_GetX(xWRK_Brd1VccL)) { IO_SetY(yWRK_Brd1VccL , false); }else{ IO_SetY(yWRK_Brd1VccL , true); }}
    else if(iTag==1){ if(!IO_GetX(xWRK_Brd1VccR)) { IO_SetY(yWRK_Brd1VccR , false); }else{ IO_SetY(yWRK_Brd1VccR , true); }}
    else if(iTag==2){ if(!IO_GetX(xWRK_Brd2VccL)) { IO_SetY(yWRK_Brd2VccL , false); }else{ IO_SetY(yWRK_Brd2VccL , true); }}
    else if(iTag==3){ if(!IO_GetX(xWRK_Brd2VccR)) { IO_SetY(yWRK_Brd2VccR , false); }else{ IO_SetY(yWRK_Brd2VccR , true); }}
*/}
//---------------------------------------------------------------------------
void TFrmDeviceSet::SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit)
{
    m_dUnit     = _dUnit     ;
    m_iUnitType = _iUnitType ;
}
//---------------------------------------------------------------------------
void TFrmDeviceSet::SetUnitInit()
{
    if(rgJogUnit1->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnit1->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnit1->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnit1->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnit1->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnit1->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnit1->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit1->Text,0.0));

    btUCcw1 -> Tag = (int)miWRK_UBrd1 ;
    btUCw1  -> Tag = (int)miWRK_UBrd1 ;
    btVCcw1 -> Tag = (int)miWRK_VBrd1 ;
    btVCw1  -> Tag = (int)miWRK_VBrd1 ;
    btTCcw1 -> Tag = (int)miSTG_TBase ;
    btTCw1  -> Tag = (int)miSTG_TBase ;
    btUCcw2 -> Tag = (int)miWRK_UBrd2 ;
    btUCw2  -> Tag = (int)miWRK_UBrd2 ;
    btVCcw2 -> Tag = (int)miWRK_VBrd2 ;
    btVCw2  -> Tag = (int)miWRK_VBrd2 ;
    btTCcw2 -> Tag = (int)miSTG_TBase ;
    btTCw2  -> Tag = (int)miSTG_TBase ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::rgJogUnit1Click(TObject *Sender)
{
    if(rgJogUnit1->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utJog ,0   ); }
    if(rgJogUnit1->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,1   ); }
    if(rgJogUnit1->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.5 ); }
    if(rgJogUnit1->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.1 ); }
    if(rgJogUnit1->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.05); }
    if(rgJogUnit1->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,0.01); }
    if(rgJogUnit1->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) {FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit1->Text,0.0));}



}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btStop1Click(TObject *Sender)
{
    MT_Stop(miSTG_TBase) ;
    MT_Stop(miWRK_UBrd1) ;
    MT_Stop(miWRK_UBrd2) ;
    MT_Stop(miWRK_VBrd1) ;
    MT_Stop(miWRK_VBrd2) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btCcwMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

    bool bOrigPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) - 1.0) <= MT_GetCmdPos(miSTG_TBase) &&
                        (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) + 1.0) ;

    bool bRvrsPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )-1.0  <= MT_GetCmdPos(miSTG_TBase)) &&
                        (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )+1.0 ) ;

         if(m_iId==miWRK_UBrd1) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요. "); return ; }}
    else if(m_iId==miWRK_UBrd2) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요. "); return ; }}
    else if(m_iId==miWRK_VBrd1) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요. "); return ; }
                                  if(bOrigPstn)                  { FM_MsgOk("ERROR","V축이 움직일 수 있는 위치가 아닙니다."); return ; }}
    else if(m_iId==miWRK_VBrd2) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요. "); return ; }
                                  if(bRvrsPstn)                  { FM_MsgOk("ERROR","V축이 움직일 수 있는 위치가 아닙니다."); return ; }}
    else if(m_iId==miSTG_TBase) { if((MT_GetCmdPos(miWRK_UBrd1) < PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_UBrd1) > PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait) + 2)) {
                                          FM_MsgOk("ERROR","1ST 보드 U축이 워크 포지션 상태 입니다."); return;      }
                                  if((MT_GetCmdPos(miWRK_UBrd2) < PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_UBrd2) > PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait) + 2)) {
                                          FM_MsgOk("ERROR","2ND 보드 U축이 워크 포지션 상태 입니다."); return;      }
                                  if((MT_GetCmdPos(miWRK_VBrd1) < PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_VBrd1) > PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait) + 2)) {
                                          FM_MsgOk("ERROR","1ST 보드 V축이 워크 포지션 상태 입니다."); return;      }
                                  if((MT_GetCmdPos(miWRK_VBrd2) < PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_VBrd2) > PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait) + 2)) {
                                          FM_MsgOk("ERROR","2ND 보드 V축이 워크 포지션 상태 입니다."); return;      }
                                  if(!MT_GetHomeSnsr(miWRK_UBrd1) || !MT_GetHomeSnsr(miWRK_UBrd2))                  {
                                          FM_MsgOk("ERROR","보드 U축 홈 센서가 감지 되지 않습니다.") ; return;      }
                                  if(IO_GetX(xSTG_TltDetect))                                                       {
                                          FM_MsgOk("ERROR","틸팅 센서가 감지 됩니다.")               ; return;      }}

    if(pcDeviceSet -> TabIndex == 0){
        if(rgJogUnit1->ItemIndex == 0) SetUnit(utJog ,0   );
        if(rgJogUnit1->ItemIndex == 1) SetUnit(utMove,1   );
        if(rgJogUnit1->ItemIndex == 2) SetUnit(utMove,0.5 );
        if(rgJogUnit1->ItemIndex == 3) SetUnit(utMove,0.1 );
        if(rgJogUnit1->ItemIndex == 4) SetUnit(utMove,0.05);
        if(rgJogUnit1->ItemIndex == 5) SetUnit(utMove,0.01);
        if(rgJogUnit1->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit1->Text,0.0));
    }
    if(pcDeviceSet -> TabIndex == 1){
        if(rgJogUnit2->ItemIndex == 0) SetUnit(utJog ,0   );
        if(rgJogUnit2->ItemIndex == 1) SetUnit(utMove,1   );
        if(rgJogUnit2->ItemIndex == 2) SetUnit(utMove,0.5 );
        if(rgJogUnit2->ItemIndex == 3) SetUnit(utMove,0.1 );
        if(rgJogUnit2->ItemIndex == 4) SetUnit(utMove,0.05);
        if(rgJogUnit2->ItemIndex == 5) SetUnit(utMove,0.01);
        if(rgJogUnit2->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit2->Text,0.0));
    }


   /* else if(m_iId==miSTG_TBase) { if(MT_GetCmdPos(miWRK_UBrd1) != PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)) { return; }
                                  if(MT_GetCmdPos(miWRK_UBrd2) != PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)) { return; }
                                  if(MT_GetCmdPos(miWRK_VBrd1) != PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)) { return; }
                                  if(MT_GetCmdPos(miWRK_VBrd2) != PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)) { return; }}
   */
         if(m_iUnitType == utJog   ) MT_JogN    ((EN_MOTR_ID)m_iId            ) ;
    else if(m_iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId , -m_dUnit ) ;
    else if(m_iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId , -m_dPitch) ;

    Trace("Operator", ("Operation Form_" + sTemp + "_Motor_Ccw Move").c_str()) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btCcwMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;

    if(m_iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;

    Trace("Operator", "Operation Form_Motor_Ccw Move Stop") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btCwMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

    bool bOrigPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) - 1.0) <= MT_GetCmdPos(miSTG_TBase) &&
                        (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) + 1.0) ;

    bool bRvrsPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )-1.0  <= MT_GetCmdPos(miSTG_TBase)) &&
                        (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )+1.0 ) ;

         if(m_iId==miWRK_UBrd1) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요."); return ; }}
    else if(m_iId==miWRK_UBrd2) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요."); return ; }}
    else if(m_iId==miWRK_VBrd1) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요. "); return ; }
                                  if(bOrigPstn)                  { FM_MsgOk("ERROR","V축이 움직일 수 있는 위치가 아닙니다."); return ; }}
    else if(m_iId==miWRK_VBrd2) { if(!IO_GetX(xSTG_WrkPstDetect)){ FM_MsgOk("ERROR","베이스 판을 워크 위치로 돌려 주세요. "); return ; }
                                  if(bRvrsPstn)                  { FM_MsgOk("ERROR","V축이 움직일 수 있는 위치가 아닙니다."); return ; }}
    else if(m_iId==miSTG_TBase) { if((MT_GetCmdPos(miWRK_UBrd1) < PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_UBrd1) > PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait) + 2)) {
                                          FM_MsgOk("ERROR","1ST 보드 U축이 워크 포지션 상태 입니다."); return;      }
                                  if((MT_GetCmdPos(miWRK_UBrd2) < PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_UBrd2) > PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait) + 2)) {
                                          FM_MsgOk("ERROR","2ND 보드 U축이 워크 포지션 상태 입니다."); return;      }
                                  if((MT_GetCmdPos(miWRK_VBrd1) < PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_VBrd1) > PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait) + 2)) {
                                          FM_MsgOk("ERROR","1ST 보드 V축이 워크 포지션 상태 입니다."); return;      }
                                  if((MT_GetCmdPos(miWRK_VBrd2) < PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait) - 2) ||
                                     (MT_GetCmdPos(miWRK_VBrd2) > PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait) + 2)) {
                                          FM_MsgOk("ERROR","2ND 보드 V축이 워크 포지션 상태 입니다."); return;      }
                                  if(!MT_GetHomeSnsr(miWRK_UBrd1) || !MT_GetHomeSnsr(miWRK_UBrd2))                  {
                                          FM_MsgOk("ERROR","보드 U축 홈 센서가 감지 되지 않습니다.") ; return;      }
                                  if(IO_GetX(xSTG_TltDetect))                                                       {
                                          FM_MsgOk("ERROR","틸팅 센서가 감지 됩니다.")               ; return;      }}
                                          
     if(pcDeviceSet -> TabIndex == 0){
        if(rgJogUnit1->ItemIndex == 0) SetUnit(utJog ,0   );
        if(rgJogUnit1->ItemIndex == 1) SetUnit(utMove,1   );
        if(rgJogUnit1->ItemIndex == 2) SetUnit(utMove,0.5 );
        if(rgJogUnit1->ItemIndex == 3) SetUnit(utMove,0.1 );
        if(rgJogUnit1->ItemIndex == 4) SetUnit(utMove,0.05);
        if(rgJogUnit1->ItemIndex == 5) SetUnit(utMove,0.01);
        if(rgJogUnit1->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit1->Text,0.0));
    }
    if(pcDeviceSet -> TabIndex == 1){
        if(rgJogUnit2->ItemIndex == 0) SetUnit(utJog ,0   );
        if(rgJogUnit2->ItemIndex == 1) SetUnit(utMove,1   );
        if(rgJogUnit2->ItemIndex == 2) SetUnit(utMove,0.5 );
        if(rgJogUnit2->ItemIndex == 3) SetUnit(utMove,0.1 );
        if(rgJogUnit2->ItemIndex == 4) SetUnit(utMove,0.05);
        if(rgJogUnit2->ItemIndex == 5) SetUnit(utMove,0.01);
        if(rgJogUnit2->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit2->Text,0.0));
    }





    /*else if(m_iId==miSTG_TBase) { if(MT_GetCmdPos(miWRK_UBrd1) != PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)) { return; }
                                  if(MT_GetCmdPos(miWRK_UBrd2) != PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)) { return; }
                                  if(MT_GetCmdPos(miWRK_VBrd1) != PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)) { return; }
                                  if(MT_GetCmdPos(miWRK_VBrd2) != PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)) { return; }}
    */
         if(m_iUnitType == utJog   ) MT_JogP    ((EN_MOTR_ID)m_iId            ) ;
    else if(m_iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  m_dUnit ) ;
    else if(m_iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  m_dPitch) ;

    Trace("Operator", ("Operation Form_" + sTemp + "_Motor_Cw Move").c_str()) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btCwMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    if(m_iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;
    Trace("Operator", "Operation Form_Motor_Cw Move Stop") ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 : FraMotor[miSTG_TBase]->Parent = pnTBase1 ;
                 FraMotor[miWRK_UBrd1]->Parent = pnUBrd1  ;
                 FraMotor[miWRK_VBrd1]->Parent = pnVBrd1  ;

                 PM.SetParent(pnBaseT1 , miSTG_TBase) ;
                 PM.SetParent(pnBrd1U  , miWRK_UBrd1) ;
                 PM.SetParent(pnBrd1V  , miWRK_VBrd1) ;

                 break;

        case 1 : FraMotor[miSTG_TBase]->Parent = pnTBase2 ;
                 FraMotor[miWRK_UBrd2]->Parent = pnUBrd2  ;
                 FraMotor[miWRK_VBrd2]->Parent = pnVBrd2  ;

                 PM.SetParent(pnBaseT2 , miSTG_TBase);
                 PM.SetParent(pnBrd2U  , miWRK_UBrd2);
                 PM.SetParent(pnBrd2V  , miWRK_VBrd2);

                 break;
    }

    switch ( pcDeviceSet -> ActivePageIndex ) {
        case 0 :                           break ;
        case 1 :                           break ;
        case 2 :                           break ;
        case 3 : rgUnitUser = rgJogUnit1 ;
                 edUnitUser = edJogUnit1 ; break ;
        case 4 : rgUnitUser = rgJogUnit2 ;
                 edUnitUser = edJogUnit2 ; break ;



    }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------

int TFrmDeviceSet::CheckBoxcheck()
{   
}

//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;

    btBrd1VccL   -> Font -> Color = IO_GetY(yWRK_Brd1VccL   ) ? clLime : clBlack ;
    btBrd1VccR   -> Font -> Color = IO_GetY(yWRK_Brd1VccR   ) ? clLime : clBlack ;
    btBrd2VccL   -> Font -> Color = IO_GetY(yWRK_Brd2VccL   ) ? clLime : clBlack ;
    btBrd2VccR   -> Font -> Color = IO_GetY(yWRK_Brd2VccR   ) ? clLime : clBlack ;
    btDstCollct1 -> Font -> Color = IO_GetY(yETC_LaserWrkVcc) ? clLime : clBlack ;
    btDstCollct2 -> Font -> Color = IO_GetY(yETC_LaserWrkVcc) ? clLime : clBlack ;

    btBrd1VccL   -> Caption = IO_GetY(yWRK_Brd1VccL   ) ? "LEFT ON"  : "LEFT OFF"  ;
    btBrd1VccR   -> Caption = IO_GetY(yWRK_Brd1VccR   ) ? "RIGHT ON" : "RIGHT OFF" ;
    btBrd2VccL   -> Caption = IO_GetY(yWRK_Brd2VccL   ) ? "LEFT ON"  : "LEFT OFF"  ;
    btBrd2VccR   -> Caption = IO_GetY(yWRK_Brd2VccR   ) ? "RIGHT ON" : "RIGHT OFF" ;
    btDstCollct1 -> Caption = IO_GetY(yETC_LaserWrkVcc) ? "ON"       : "OFF"       ;
    btDstCollct2 -> Caption = IO_GetY(yETC_LaserWrkVcc) ? "ON"       : "OFF"       ;

    if(OM.CmnOptn.bNotUsedVAxis) {
        Panel9 -> Visible = false;
        Panel1 -> Visible = false;
    }

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------




