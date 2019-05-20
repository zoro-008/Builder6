//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"

#include "Sequence.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "Heater.h"
#include "Work.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
    //작명 방법 기술.
    //aiLDR_Stopper -> pnLDR_Stopper1 (1은 페이지 넘버)
    //miLDR_YCmp    -> pnLDR_YCmp1    (1은 페이지 넘버)

    PM.SetWindow(pnRAL_XIdx , miRAL_XIdx);
    PM.SetWindow(pnWRK_ZPrb , miWRK_ZPrb);
    PM.SetWindow(pnSRT_TSrt , miSRT_TSrt);

    //Motor Window Set to Panel
    MtWindowSet();
    AtWindowSet();

    //Fixed Value .
//    PM.SetPos(miWRE_XIdx,pvWRE_XIdxPull,PM.GetValue (miWRE_XIdx , pvWRE_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1))); //고정.
//    PM.SetPos(miWRD_XIdx,pvWRD_XIdxPull,PM.GetValue (miWRD_XIdx , pvWRD_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1)));

    //IO Num -> Tag
    btRAl_Air  -> Tag = (int)yRAL_AirBlower ;
    btSRT_Air  -> Tag = (int)ySRT_AirBlower ;
    btTSE_Test -> Tag = (int)yTST_StartTest ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TEdit"))
        {
            ((TEdit*)FindComponent(Components[i]->Name))->ImeMode    = imDisable ;
            ((TEdit*)FindComponent(Components[i]->Name))->NumberOnly = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseApoint  = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseMinus   = true ;
        }
        else if(Components[i]->ClassNameIs("TPageControl"))
        {
            ((TPageControl*)FindComponent(Components[i]->Name))->ActivePageIndex = 0 ;
        }
    }

    rgUnitUser = rgJogUnit1  ;
    edUnitUser = edJogUnit1  ;

    FraMotor[miRAL_XIdx]->Parent = pnRAL_XIdx ;
    FraMotor[miWRK_ZPrb]->Parent = pnWRK_ZPrb ;
    FraMotor[miSRT_TSrt]->Parent = pnSRT_TSrt ;

    FraCylinder[aiRAL_Index  ]->Parent = pnRAL_IdxUpDn ;
    FraCylinder[aiRAL_Stopper]->Parent = pnRal_Stpr    ;


    PM.SetParent(pnRAL_XIdxp ,miRAL_XIdx);
    PM.SetParent(pnWRK_ZPrbp ,miWRK_ZPrb);
    PM.SetParent(pnSRT_TSrtp ,miSRT_TSrt);


//    FraMotor[miSRT_TSrt]->SetPitch()

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MtWindowSetPitch()
{
//    FraMotor  [miLDR_YCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    //FraMotor  [miSRT_TSrt] ->SetPitch(PM.GetValue(miSRT_TSrt,poSRT_TSrtPitch));
}
void __fastcall TFrmDeviceSet::MtWindowSet()
{

    for(int i=0 ; i<MAX_MOTR ; i++) {
        FraMotor  [i] = new TFraMotr();
        FraMotor  [i] ->SetIdType(i ,MT_GetDirType(i));
        FraMotor  [i] ->Show();
    }

    //FraMotor  [miFED_TFed] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    //FraMotor  [miRAL_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
    //FraMotor  [miWRK_ZPrb] ->SetPitch(OM.DevInfo.dRowPitch);
    //FraMotor  [miSRT_TSrt] ->SetPitch(PM.GetValue(miSRT_TSrt , poSRT_TSrtPitch));

//    for(int i=0 ; i<MAX_MOTR ; i++) FraMotor[i]->Show();
}
void __fastcall TFrmDeviceSet::AtWindowSet()
{
    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) {
        FraCylinder[i] = new TFraCyl();
        FraCylinder[i] ->SetIdType(i ,AT_GetDirType(i));
        FraCylinder[i] ->Show();
    }

//    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i]->Show();
}
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
     Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

//    JobFileBackUp();

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    //DataMan SetMaxColRow
    DM.ARAY[riRAL].SetMaxColRow(1,1);
    DM.ARAY[riWRK].SetMaxColRow(1,1);


//    HTR.SetTemp(1,OM.DevOptn.iTempSet1);
//    HTR.SetTemp(2,OM.DevOptn.iTempSet2);
    //OM.DevOptn.iTempSet


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::JobFileBackUp()
{
    String sSorc  = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    String sPath  = ExtractFilePath(Application -> ExeName) + "BackUp\\JobFile\\" ;
    String SDest  = sPath + Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.ClearDirDate(sPath , Now() - DATA_DELETE_DAY);

    sPath += Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.CopyDir(sSorc , SDest);

    //Trace Log.
//    Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    btSaveDevice -> Enabled = !SEQ._bRun ;//&& !FrmMain -> tmMainSet -> Enabled && !FrmMain -> tmSubSet -> Enabled ;

    btRAl_Air  -> Caption = IO_GetY(btRAl_Air  -> Tag) ? "ON" : "OFF" ;
    btSRT_Air  -> Caption = IO_GetY(btSRT_Air  -> Tag) ? "ON" : "OFF" ;
    btTSE_Test -> Caption = IO_GetY(btTSE_Test -> Tag) ? "ON" : "OFF" ;

    lbPinCnt->Caption = WRK.GetPinCnt() ;

    btTSE_Test -> Enabled = !SEQ._bRun ;

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
    tmHeat       ->Enabled = false;
    Timer1       ->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo)
    {
        //Feeding.
        //edNoPkgSec      -> Text = OM.DevOptn.iNoPkgSec      ;
    }

    else {
        //Feeding.
        //OM.DevOptn.iNoPkgSec      = StrToFloatDef(edNoPkgSec      -> Text , 10    )  ;
        UpdateDevInfo(toTabl);
    }
//WRK.

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        //Feeding.
        edNoPkgSec        -> Text = OM.DevOptn.iNoPkgSec      ;
        edPkgDetect       -> Text = OM.DevOptn.iPkgDetect     ;
        edIdleTime        -> Text = OM.DevOptn.iIdleTime      ;
        //edFeedWaitTime    -> Text = OM.DevOptn.iFeedWaitTime  ;
        //edFeedComDelay    -> Text = OM.DevOptn.iFeedComDelay  ;
        //edFedMinMove      -> Text = OM.DevOptn.dFedMinMove    ;
        //edFedFailCnt      -> Text = OM.DevOptn.iFedFailCnt    ;

        //Rail
        edBlowerDelay     -> Text = OM.DevOptn.iBlowerDelay   ;

        //Work
        edFailCnt         -> Text = OM.DevOptn.iFailCnt       ;
        edPrbStartDelay   -> Text = OM.DevOptn.iPrbStartDelay ;
        edPinChangeCnt    -> Text = OM.DevOptn.iPinChangeCnt  ;
        edPrbContactTime  -> Text = OM.DevOptn.iPrbContactTime;

        //Sorter
        edBin1FullCnt     -> Text = OM.DevOptn.iBin1FullCnt   ;
        edBin2FullCnt     -> Text = OM.DevOptn.iBin2FullCnt   ;
        edBin3FullCnt     -> Text = OM.DevOptn.iBin3FullCnt   ;
        edBin4FullCnt     -> Text = OM.DevOptn.iBin4FullCnt   ;
        edBin5FullCnt     -> Text = OM.DevOptn.iBin5FullCnt   ;
        edBin3Deley       -> Text = OM.DevOptn.iBin3Deley     ;
        edSortDeley       -> Text = OM.DevOptn.iSortDeley     ;

        cbSortPKGSensor   -> Checked = OM.DevOptn.bSortPKGSensor;

        //Heater
        edTempSet1        -> Text = OM.DevOptn.iTempSet1      ;
        edTempSet2        -> Text = OM.DevOptn.iTempSet2      ;
        edTempGap         -> Text = OM.DevOptn.iTempGap       ;
        edEpmtyBin        -> Text = OM.DevOptn.iEpmtyBin      ;


        edStartHeatSec    -> Text = OM.DevOptn.iStartHeatSec  ;
        edStopTemp        -> Text = OM.DevOptn.iStopTemp      ;
        edStopWaitSec     -> Text = OM.DevOptn.iStopWaitSec   ;
        edPKGHeatSec      -> Text = OM.DevOptn.iPKGHeatSec    ;
    }

    else {
        //Feeding.
        OM.DevOptn.iNoPkgSec        = StrToFloatDef(edNoPkgSec      -> Text , 10    ) ;
        OM.DevOptn.iPkgDetect       = StrToFloatDef(edPkgDetect     -> Text , 10    ) ;
        OM.DevOptn.iIdleTime        = StrToFloatDef(edIdleTime      -> Text , 10    ) ;
        //OM.DevOptn.iFeedWaitTime    = StrToFloatDef(edFeedWaitTime  -> Text , 100   ) ;
        //OM.DevOptn.iFedFailCnt      = StrToFloatDef(edFedFailCnt    -> Text , 5     ) ;

        //Rail
        OM.DevOptn.iBlowerDelay     = StrToFloatDef(edBlowerDelay   -> Text , 200   ) ;


        //Work
        OM.DevOptn.iFailCnt         = StrToFloatDef(edFailCnt       -> Text , 20    ) ;
        OM.DevOptn.iPrbStartDelay   = StrToFloatDef(edPrbStartDelay -> Text , 10    ) ;
        OM.DevOptn.iPinChangeCnt    = StrToFloatDef(edPinChangeCnt  -> Text , 200000) ;
        OM.DevOptn.iPrbContactTime  = StrToFloatDef(edPrbContactTime-> Text , 1000  ) ;

        //Sorter
        OM.DevOptn.iBin1FullCnt     = StrToFloatDef(edBin1FullCnt   -> Text , 2000  ) ;
        OM.DevOptn.iBin2FullCnt     = StrToFloatDef(edBin2FullCnt   -> Text , 2000  ) ;
        OM.DevOptn.iBin3FullCnt     = StrToFloatDef(edBin3FullCnt   -> Text , 2000  ) ;
        OM.DevOptn.iBin4FullCnt     = StrToFloatDef(edBin4FullCnt   -> Text , 2000  ) ;
        OM.DevOptn.iBin5FullCnt     = StrToFloatDef(edBin5FullCnt   -> Text , 2000  ) ;
        OM.DevOptn.iBin3Deley       = StrToFloatDef(edBin3Deley     -> Text , 300   ) ;
        OM.DevOptn.iSortDeley       = StrToFloatDef(edSortDeley     -> Text , 300   ) ;

        OM.DevOptn.bSortPKGSensor   = cbSortPKGSensor -> Checked ;

        //Heater
        OM.DevOptn.iTempSet1        = StrToFloatDef(edTempSet1      -> Text , 100   ) ;
        OM.DevOptn.iTempSet2        = StrToFloatDef(edTempSet2      -> Text , 100   ) ;
        OM.DevOptn.iTempGap         = StrToFloatDef(edTempGap       -> Text , 10    ) ;
        OM.DevOptn.iEpmtyBin        = edEpmtyBin                    -> Text.ToIntDef(3);

        //OM.DevOptn.iFeedComDelay    = edFeedComDelay                -> Text.ToIntDef(0);
        OM.DevOptn.iStartHeatSec    = edStartHeatSec                -> Text.ToIntDef(0);

        OM.DevOptn.iStopTemp        = edStopTemp                    -> Text.ToIntDef(30);
        OM.DevOptn.iStopWaitSec     = edStopWaitSec                 -> Text.ToIntDef(30);
        //OM.DevOptn.iFedVacuumDelay  = edFedVacuumDelay              -> Text.ToIntDef(0 );
        //OM.DevOptn.dFedMinMove      = edFedMinMove                  -> Text.ToDouble(  );
        OM.DevOptn.iPKGHeatSec      = edPKGHeatSec                  -> Text.ToIntDef(10);

        UpdateDevOptn(toTabl);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);
    tmUpdateTimer->Enabled = true;
    tmHeat       ->Enabled = false;
    pcDeviceSet -> ActivePageIndex = 1 ;

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utPitch,0  );
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{

    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 : //사용안함.
        break;

        case 1 :
        break;

        case 2 :
        break;

    }
     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                            break ;
         case 1 : rgUnitUser = rgJogUnit1  ;
                  edUnitUser = edJogUnit1  ; break ;
         case 2 :                            break ;

     }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::rgJogUnit1Click(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::edJogUnitChange(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 6)  for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdateTimer->Enabled = false ;
    tmHeat       ->Enabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSRT_AirClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;

    IO_SetY(iSel , !IO_GetY(iSel));
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btIncMoveClick(TObject *Sender)
{
    //MT_SetPos(miFED_TFed,0);
    //MT_GoIncMan(miFED_TFed,edIncMove->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btPinCntResetClick(TObject *Sender)
{
    WRK.SetPinCnt(0);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btTSE_TestClick(TObject *Sender)
{
    IO_SetY(yTST_StartTest,true);
    ::Sleep(10);
    IO_SetY(yTST_StartTest,false);

}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::btFeedClick(TObject *Sender)
{
     IO_SetY(yETC_FeederOn,!IO_GetY(yETC_FeederOn));

     if(IO_GetY(yETC_FeederOn)) btFeed -> Caption = "ON ";
     else                       btFeed -> Caption = "OFF";
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::Timer1Timer(TObject *Sender)
{
    Timer1 -> Enabled  = false;

    if(IO_GetX(xTST_End)) Panel17 -> Color = clLime ;

    Timer1 -> Enabled = true;
}
//---------------------------------------------------------------------------

