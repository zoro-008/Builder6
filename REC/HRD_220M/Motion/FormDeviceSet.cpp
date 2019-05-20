//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
//---------------------------------------------------------------------------
#include "JigPick.h"
#include "Dispnr.h"
#include "Place.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "RS232Man.h"
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

    //실린더 창.
    TPanel * pActr ;

    for(int i=0 ; i<MAX_ACTR ; i++) {
        pActr = ((TPanel*)FindComponent("pnAtcr" + AnsiString(i))) ;
        FraCylinder[i] = new TFraCylOneBt(pActr);

             if(aiDSP_IdxUpDn) FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , JIP.CheckSafe );
        else if(aiSTG_PshFwBw) FraCylinder[i] ->SetConfig(i , AT_GetName(i) , AT_GetDirType(i) , pActr , JIP.CheckSafe );
        FraCylinder[i] ->Show();
    }

    foVaccm = new TFraOutput(); foVaccm -> SetConfig(yETC_Vacuum ,IO_GetYName(yETC_Vacuum ) , pnVaccm);
    foEject = new TFraOutput(); foEject -> SetConfig(yETC_Eject  ,IO_GetYName(yETC_Eject  ) , pnEject);

    //DevInfo.
    /*시발 폼디바이스 컴파일 안됌.... 컴포넌트로 제작 해야 할듯...
    edColPitch      -> UseMinus = fal
    e ;
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


    pcDeviceSet -> ActivePageIndex = 0 ;

}

//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.GetCrntDev());

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.GetCrntDev());

    PM.UpdatePstn(toBuff);
    PM.Save(OM.GetCrntDev());

    DM.ARAY[riDSP].SetMaxColRow(1 , 1);
    DM.ARAY[riJIG].SetMaxColRow(1 , 1);
    DM.ARAY[riPIK].SetMaxColRow(1 , 1);

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
    btSaveDevice -> Enabled = !SEQ._bRun ;
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default: break;
        case 0 : break;
        case 1 : break;
        case 2 : break;
        case 3 : break;
        case 4 : break;
        case 5 : break;
        case 6 : break;
        case 7 : break;
        case 8 : break;
        case 9 : break;
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
    if(bTo){
        //edCsSlotPitch   -> Text = OM.DevInfo.dCsSlPitch     ;
    }
    else {
        //OM.DevInfo.dColPitch       = StrToFloatDef(edColPitch      -> Text,1.0);
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{


    if(bTo){
        edVaccmdelay     -> Text      = OM.DevOptn.iVaccmdelay     ;
        edEjectdelay     -> Text      = OM.DevOptn.iEjectdelay     ;
        edPlaceDelay     -> Text      = OM.DevOptn.iPlaceDelay     ;
        edEndDelay       -> Text      = OM.DevOptn.iEndDelay       ;

        edDispnrdelay    -> Text      = OM.DevOptn.iDisprdelay     ;
        edDisprSpd       -> Text      = OM.DevOptn.iDisprSpd       ;
        edJigPCLOfs      -> Text      = OM.DevOptn.dJigPCLOfs      ;

        edDisprIncPosCnt -> Text      = OM.DevOptn.iDisprIncPosCnt ;
        edDisprShotCnt   -> Text      = OM.DevOptn.iDisprShotCnt   ;
        edDisprRptCnt    -> Text      = OM.DevOptn.iDisprRptCnt    ;
    }
    else{
        OM.DevOptn.iVaccmdelay     = StrToIntDef(edVaccmdelay      -> Text , 100  ) ;
        OM.DevOptn.iEjectdelay     = StrToIntDef(edEjectdelay      -> Text , 100  ) ;
        OM.DevOptn.iPlaceDelay     = StrToIntDef(edPlaceDelay      -> Text , 100  ) ;
        OM.DevOptn.iEndDelay       = StrToIntDef(edEndDelay        -> Text , 100  ) ;

        OM.DevOptn.iDisprdelay     = StrToIntDef(edDispnrdelay     -> Text , 100  ) ;
        OM.DevOptn.iDisprSpd       = StrToIntDef(edDisprSpd        -> Text , 1    ) ;

        OM.DevOptn.dJigPCLOfs      = StrToIntDef(edJigPCLOfs       -> Text , 1    ) ;

        OM.DevOptn.iDisprIncPosCnt = StrToIntDef(edDisprIncPosCnt  -> Text , 1    ) ;
        OM.DevOptn.iDisprShotCnt   = StrToIntDef(edDisprShotCnt    -> Text , 1    ) ;
        OM.DevOptn.iDisprRptCnt    = StrToIntDef(edDisprRptCnt     -> Text , 1    ) ;
    }


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex ) {
        default:                               break;
        case 0 :                               break;
        case 1 : pnJogUnit -> Parent = pnJog1; break;
        case 2 :                               break;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::rgJogUnitClick(TObject *Sender)
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






