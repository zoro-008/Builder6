//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSetting.h"
#include "OptionMan.h"
#include "global.h"
#include "PaixMotion.h"
#include "UtilDefine.h"
#include "SEQUnit.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSetting *FrmSetting;
//---------------------------------------------------------------------------
__fastcall TFrmSetting::TFrmSetting(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TFrmSetting::btStartMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag ;

    switch(iPanelNo){
        default :                                    break ;
        case 1  : btStart -> BevelOuter = bvLowered; break ;
        case 2  : btStop  -> BevelOuter = bvLowered; break ;
        case 3  : btEmg   -> BevelOuter = bvLowered; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::btStartMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btStart -> BevelOuter = bvRaised;
    btStop  -> BevelOuter = bvRaised;
    btEmg   -> BevelOuter = bvRaised;
}
//---------------------------------------------------------------------------


void __fastcall TFrmSetting::btOKClick(TObject *Sender)
{
    int iMAX;
    switch(iEditNo){
        default : break ;
        case 1  : if(edCleanRpm -> Text == "") edCleanRpm -> Text = 0;
                  iMAX = edCleanRpm -> Text.ToInt();
                  if(iMAX > 2000) edCleanRpm -> Text = 2000;

                  Trace("Clean RPM Change", (String(OM.DevOptn.iCleanRPM) + " To " + edCleanRpm -> Text.ToInt()).c_str() ) ;

                  pnCleanRpm -> Caption  = edCleanRpm  -> Text ;
                  OM.DevOptn.iCleanRPM   = edCleanRpm  -> Text.ToInt();
                  break ;

        case 2  : if(edAirRpm    -> Text == "") edAirRpm -> Text = 0;
                  iMAX = edAirRpm -> Text.ToInt();
                  if(iMAX > 2000) edAirRpm -> Text = 2000;

                  Trace("Air RPM Change", (String(OM.DevOptn.iAirRPM) + " To " + edAirRpm -> Text.ToInt()).c_str() ) ;

                  pnAirRpm    -> Caption = edAirRpm    -> Text ;
                  OM.DevOptn.iAirRPM     = edAirRpm    -> Text.ToInt();
                  break ;

        case 3  : if(edDryRpm    -> Text == "") edDryRpm -> Text = 0;
                  iMAX = edDryRpm -> Text.ToInt();
                  if(iMAX > 2000) edDryRpm -> Text = 2000;

                  Trace("Dry RPM Change", (String(OM.DevOptn.iDryRPM) + " To " + edDryRpm -> Text.ToInt()).c_str() ) ;

                  pnDryRpm    -> Caption = edDryRpm    -> Text ;
                  OM.DevOptn.iDryRPM     = edDryRpm    -> Text.ToInt();
                  break ;

        case 4  : if(edCleanTime -> Text == "") edCleanTime -> Text = 0;
                  iMAX = edCleanTime -> Text.ToInt();
                  if(iMAX > 9999) edCleanTime -> Text = 9999;

                  Trace("Clean Time Change", (String(OM.DevOptn.iCleanTime) + " To " + edCleanTime -> Text.ToInt()*1000).c_str() ) ;

                  pnCleanTime -> Caption = edCleanTime -> Text ;
                  OM.DevOptn.iCleanTime  = edCleanTime -> Text.ToInt() * 1000;
                  break ;

        case 5  : if(edAirTime   -> Text == "") edAirTime -> Text = 0;
                  iMAX = edAirTime -> Text.ToInt();
                  if(iMAX > 9999) edAirTime -> Text = 9999;

                  Trace("Air Time Change", (String(OM.DevOptn.iAirTime) + " To " + edAirTime -> Text.ToInt()*1000).c_str() ) ;

                  pnAirTime   -> Caption = edAirTime   -> Text ;
                  OM.DevOptn.iAirTime    = edAirTime   -> Text.ToInt() * 1000;
                  break ;

        case 6  : if(edDryTime   -> Text == "") edDryTime -> Text = 0;
                  iMAX = edDryTime -> Text.ToInt();
                  if(iMAX > 9999) edDryTime -> Text = 9999;

                  Trace("Dry Time Change", (String(OM.DevOptn.iDryTime) + " To " + edDryTime -> Text.ToInt()*1000).c_str() ) ;

                  pnDryTime   -> Caption = edDryTime   -> Text ;
                  OM.DevOptn.iDryTime    = edDryTime   -> Text.ToInt() * 1000;
                  break ;
    }

    OM.SaveDevOptn(OM.m_sCrntDev);
    edCleanRpm  -> Visible = false; pnButton -> Visible = false;
    edAirRpm    -> Visible = false; pnButton -> Visible = false;
    edDryRpm    -> Visible = false; pnButton -> Visible = false;
    edCleanTime -> Visible = false; pnButton -> Visible = false;
    edAirTime   -> Visible = false; pnButton -> Visible = false;
    edDryTime   -> Visible = false; pnButton -> Visible = false;

}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::bt1Click(TObject *Sender)
{
    AnsiString sEditText ;
    int iPanelNo = ((TButton *)Sender) -> Tag ;

    switch(iEditNo){
        default :                                  break ;
        case 1  : sEditText = edCleanRpm  -> Text; break ;
        case 2  : sEditText = edAirRpm    -> Text; break ;
        case 3  : sEditText = edDryRpm    -> Text; break ;
        case 4  : sEditText = edCleanTime -> Text; break ;
        case 5  : sEditText = edAirTime   -> Text; break ;
        case 6  : sEditText = edDryTime   -> Text; break ;
    }

    if(sEditText == "0") sEditText = "";

    sEditText += iPanelNo;

    switch(iEditNo){
        default :                                   break ;
        case 1  : edCleanRpm  -> Text = sEditText ; break ;
        case 2  : edAirRpm    -> Text = sEditText ; break ;
        case 3  : edDryRpm    -> Text = sEditText ; break ;
        case 4  : edCleanTime -> Text = sEditText ; break ;
        case 5  : edAirTime   -> Text = sEditText ; break ;
        case 6  : edDryTime   -> Text = sEditText ; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::btClearClick(TObject *Sender)
{
    switch(iEditNo){
        default :                          break ;
        case 1  : edCleanRpm  -> Text = 0; break ;
        case 2  : edAirRpm    -> Text = 0; break ;
        case 3  : edDryRpm    -> Text = 0; break ;
        case 4  : edCleanTime -> Text = 0; break ;
        case 5  : edAirTime   -> Text = 0; break ;
        case 6  : edDryTime   -> Text = 0; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::btBackClick(TObject *Sender)
{
    AnsiString sEditText ;
    switch(iEditNo){
        default :                                   break ;
        case 1  : sEditText = edCleanRpm  -> Text ; break ;
        case 2  : sEditText = edAirRpm    -> Text ; break ;
        case 3  : sEditText = edDryRpm    -> Text ; break ;
        case 4  : sEditText = edCleanTime -> Text ; break ;
        case 5  : sEditText = edAirTime   -> Text ; break ;
        case 6  : sEditText = edDryTime   -> Text ; break ;
    }
    sEditText = sEditText.Delete(sEditText.Length(), 1);

    switch(iEditNo){
        default :                                   break ;
        case 1  : edCleanRpm  -> Text = sEditText ; break ;
        case 2  : edAirRpm    -> Text = sEditText ; break ;
        case 3  : edDryRpm    -> Text = sEditText ; break ;
        case 4  : edCleanTime -> Text = sEditText ; break ;
        case 5  : edAirTime   -> Text = sEditText ; break ;
        case 6  : edDryTime   -> Text = sEditText ; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::FormShow(TObject *Sender)
{
    lbModelName -> Caption = g_sModelName;
    pnCleanRpm  -> Caption = OM.DevOptn.iCleanRPM ;
    pnAirRpm    -> Caption = OM.DevOptn.iAirRPM   ;
    pnDryRpm    -> Caption = OM.DevOptn.iDryRPM   ;
    pnCleanTime -> Caption = OM.DevOptn.iCleanTime / 1000;
    pnAirTime   -> Caption = OM.DevOptn.iAirTime   / 1000;
    pnDryTime   -> Caption = OM.DevOptn.iDryTime   / 1000;

    edCleanRpm  -> Text = 0;
    edAirRpm    -> Text = 0;
    edDryRpm    -> Text = 0;
    edCleanTime -> Text = 0;
    edAirTime   -> Text = 0;
    edDryTime   -> Text = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::btEmgClick(TObject *Sender)
{
    pPaix -> SuddenStop(miSpinMt);
    pPaix -> SuddenStop(miTiltMt);
}
//---------------------------------------------------------------------------


void __fastcall TFrmSetting::pnCleanRpmClick(TObject *Sender)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag;
    iEditNo = iPanelNo ;

    edCleanRpm  -> Visible = false; pnButton -> Visible = false;
    edAirRpm    -> Visible = false; pnButton -> Visible = false;
    edDryRpm    -> Visible = false; pnButton -> Visible = false;
    edCleanTime -> Visible = false; pnButton -> Visible = false;
    edAirTime   -> Visible = false; pnButton -> Visible = false;
    edDryTime   -> Visible = false; pnButton -> Visible = false;

    switch(iPanelNo){
        default : break ;
        case 1  : edCleanRpm  -> Visible = true; edCleanRpm  -> Text = "0"; pnButton -> Visible = true; break;
        case 2  : edAirRpm    -> Visible = true; edAirRpm    -> Text = "0"; pnButton -> Visible = true; break;
        case 3  : edDryRpm    -> Visible = true; edDryRpm    -> Text = "0"; pnButton -> Visible = true; break;
        case 4  : edCleanTime -> Visible = true; edCleanTime -> Text = "0"; pnButton -> Visible = true; break;
        case 5  : edAirTime   -> Visible = true; edAirTime   -> Text = "0"; pnButton -> Visible = true; break;
        case 6  : edDryTime   -> Visible = true; edDryTime   -> Text = "0"; pnButton -> Visible = true; break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetting::btStartClick(TObject *Sender)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag ;

    switch(iPanelNo){
       default :                                    break ;
       case 1  : SEQ->m_bBtnStart = true;           break ;
       case 2  : SEQ->m_bBtnStop  = true;
                 break ;
       case 3  : pPaix -> SuddenStop(miSpinMt);
                 pPaix -> SuddenStop(miTiltMt);     break ;
    }

     String sButton = ((TPanel *)Sender) -> Caption ;
     Trace("Button",(sButton +" Clicked").c_str());
}
//---------------------------------------------------------------------------



