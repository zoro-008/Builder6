//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormPosSet.h"

//---------------------------------------------------------------------------
#include "Stage.h"

//#include "Sequence.h"

//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
//#include "ManualMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmPosSet *FrmPosSet;
//---------------------------------------------------------------------------
__fastcall TFrmPosSet::TFrmPosSet(TComponent* Owner)
    : TForm(Owner)
{

}

void __fastcall TFrmPosSet::FormCreate(TObject *Sender)
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

    //실린더 창.
    TPanel * pActr ;
    for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

        switch(i){
            default :                                                                                            break;
            case 0  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG.CheckSafe ); break;  //aiLSP_GuidFwBw
            case 1  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG.CheckSafe ); break;  //aiLST_StckUpDn
            case 2  : FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , STG.CheckSafe ); break;  //aiLDR_IndxUpDn
        }
        FraCylinder[i] ->Show();
    }

    //foLDRAirBlow = new TFraOutput(); foLDRAirBlow -> SetConfig(yLDR_AirBlow    , IO_GetYName(yLDR_AirBlow   ), pnLDR1);


    //DevInfo.
    /*시발 폼디바이스 컴파일 안됌.... 컴포넌트로 제작 해야 할듯...
    edColPitch      -> UseMinus = false ;
    edRowPitch      -> UseMinus = false ;
    edColGrCnt      -> UseMinus = false ; edColGrCnt  -> UseApoint = true  ;
    edColGrPitch    -> UseMinus = false ;
    edColCnt        -> UseMinus = false ; edColCnt    -> UseApoint = true  ;
    edRowCnt        -> UseMinus = false ; edRowCnt    -> UseApoint = true  ;

    edCsSlotCnt     -> UseMinus = false ; edCsSlotCnt -> UseApoint = true  ;
    edCsSlotPitch   -> UseMinus = false ;

    edZigColCnt     -> UseMinus = false ; edZigColCnt -> UseApoint = true  ;
    edZigRowCnt     -> UseMinus = false ; edZigRowCnt -> UseApoint = true  ;
    edZigColPitch   -> UseMinus = false ;
    edZigRowPitch   -> UseMinus = false ;

    edClipColGrPitch-> UseMinus = false ;
    edClipColPitch  -> UseMinus = false ;

    //Option
    edLotEndMgzCnt  -> UseMinus = false ; edLotEndMgzCnt -> UseApoint = true  ;
    */




}

//---------------------------------------------------------------------------
void __fastcall TFrmPosSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    //if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.GetCrntDev());

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());

    PM.UpdatePstn(toBuff);
    PM.Save(OM.GetCrntDev());
}
//---------------------------------------------------------------------------
void __fastcall TFrmPosSet::tmUpdateTimerTimer(TObject *Sender)
{                                                                                   
    tmUpdateTimer->Enabled = false;                                                 
    btSaveDevice -> Enabled = !SEQ._bRun ;

    //edEmptyStrpCnt -> Enabled = cbEmptyStrpPlace -> Checked ;

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPosSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPosSet::UpdateDevInfo(bool bTo)
{
/*
    if(bTo){
        edLDRColPitch      -> Text = OM.DevInfo.dLDRColPitch    ;
        edLDRColCnt        -> Text = OM.DevInfo.iLDRColCnt      ;

        edLDRRowPitch      -> Text = OM.DevInfo.dLDRRowPitch    ;
        edLDRRowCnt        -> Text = OM.DevInfo.iLDRRowCnt      ;

        edScktPitch        -> Text = OM.DevInfo.dScktPitch      ;

        edULDColPitch      -> Text = OM.DevInfo.dULDColPitch    ;    // YOU
        edULDColCnt        -> Text = OM.DevInfo.iULDColCnt      ;

        edULDRowPitch      -> Text = OM.DevInfo.dULDRowPitch    ;
        edULDRowCnt        -> Text = OM.DevInfo.iULDRowCnt      ;


    }
    else {
        OM.DevInfo.dLDRColPitch       = StrToFloatDef(edLDRColPitch      -> Text,1.0);
        OM.DevInfo.iLDRColCnt         = StrToIntDef  (edLDRColCnt        -> Text,1  );

        OM.DevInfo.dLDRRowPitch       = StrToFloatDef(edLDRRowPitch      -> Text,1.0);
        OM.DevInfo.iLDRRowCnt         = StrToIntDef  (edLDRRowCnt        -> Text,1  );

        OM.DevInfo.dScktPitch         = StrToFloatDef(edScktPitch        -> Text,1.0);

        OM.DevInfo.dULDColPitch       = StrToFloatDef(edULDColPitch      -> Text,1.0);      // YOU
        OM.DevInfo.iULDColCnt         = StrToIntDef  (edULDColCnt        -> Text,1  );

        OM.DevInfo.dULDRowPitch       = StrToFloatDef(edULDRowPitch      -> Text,1.0);
        OM.DevInfo.iULDRowCnt         = StrToIntDef  (edULDRowCnt        -> Text,1  );


    }
*/
}
//---------------------------------------------------------------------------
void __fastcall TFrmPosSet::UpdateDevOptn(bool bTo)
{
/*

    if(bTo){
        edPickDealy -> Text      = OM.DevOptn.iPickDelay;
        edPlceDelay -> Text      = OM.DevOptn.iPlceDelay;



    }
    else{
        OM.DevOptn.iPickDelay = StrToIntDef(edPickDealy-> Text , 100  ) ;
        OM.DevOptn.iPlceDelay = StrToIntDef(edPlceDelay-> Text , 100 ) ;

    }
*/
}
//---------------------------------------------------------------------------
void __fastcall TFrmPosSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TFrmPosSet::rgJogUnitClick(TObject *Sender)
{
    for(int i=0;i<MAX_MOTR;i++) {
        if(rgJogUnit->ItemIndex == 0) FraMotor[i]->SetUnit(utJog ,0.0 );
        if(rgJogUnit->ItemIndex == 1) FraMotor[i]->SetUnit(utMove,1.0 );
        if(rgJogUnit->ItemIndex == 2) FraMotor[i]->SetUnit(utMove,0.5 );
        if(rgJogUnit->ItemIndex == 3) FraMotor[i]->SetUnit(utMove,0.1 );
        if(rgJogUnit->ItemIndex == 4) FraMotor[i]->SetUnit(utMove,0.05);
        if(rgJogUnit->ItemIndex == 5) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmPosSet::edJogUnitChange(TObject *Sender)
{
    for(int i=0;i<MAX_MOTR;i++) {
        if(rgJogUnit->ItemIndex == 0) FraMotor[i]->SetUnit(utJog ,0.0 );
        if(rgJogUnit->ItemIndex == 1) FraMotor[i]->SetUnit(utMove,1.0 );
        if(rgJogUnit->ItemIndex == 2) FraMotor[i]->SetUnit(utMove,0.5 );
        if(rgJogUnit->ItemIndex == 3) FraMotor[i]->SetUnit(utMove,0.1 );
        if(rgJogUnit->ItemIndex == 4) FraMotor[i]->SetUnit(utMove,0.05);
        if(rgJogUnit->ItemIndex == 5) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    }
}
//---------------------------------------------------------------------------









