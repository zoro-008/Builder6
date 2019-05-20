//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTiltMotor.h"
#include "OptionMan.h"
#include "PaixMotion.h"
#include "UtilDefine.h"
#include "SEQUnit.h"
#include "FormMsgOk.h"
#include "SLogUnit.h"
#include "global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmTiltMT *FrmTiltMT;
//---------------------------------------------------------------------------
__fastcall TFrmTiltMT::TFrmTiltMT(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btTiltCwMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SEQ -> m_bRun == false) {
        int iPanelNo = ((TPanel *)Sender) -> Tag ;

        switch(iPanelNo){
            default :                                       break ;
            case 1  : btTiltCw   -> BevelOuter = bvLowered;
                      pPaix -> MoveJogRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed);
                      break ;
            case 2  : btTiltCcw  -> BevelOuter = bvLowered;
                      pPaix -> MoveJogRPM(miTiltMt, -OM.DevOptn.iTiltMtSpeed);
                      break ;
            case 3  : btTiltHome -> BevelOuter = bvLowered;
                      break ;
        }
    }
    else {
       FrmMsgOk->MsgOk("Error" , "장비가 동작 중입니다." );
       Trace("Error" , "장비가 동작 중입니다." );
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btTiltCwMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SEQ -> m_bRun == false) {
        int iPanelNo = ((TPanel *) Sender) -> Tag;

        if(iPanelNo != 3){
            pPaix -> StopMotor(miTiltMt);
        }
        btTiltCw   -> BevelOuter = bvRaised ;
        btTiltCcw  -> BevelOuter = bvRaised ;
        btTiltHome -> BevelOuter = bvRaised ;
    }
    else {
       FrmMsgOk->MsgOk("Error" , "장비가 동작 중입니다." );
       Trace("Error" , "장비가 동작 중입니다." );
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmTiltMT::bt1Click(TObject *Sender)
{
    AnsiString sEditText ;
    int iManNo = ((TButton *)Sender) -> Tag ;

    switch(iEditNo){
        default :                                       break ;
        case 1  : sEditText = edTiltStartPs    -> Text; break ;
        case 2  : sEditText = edTiltStopPs     -> Text; break ;
        case 3  : sEditText = edTiltSettingSpd -> Text; break ;
        case 4  : sEditText = edWtrDrainTime   -> Text; break ;
    }

    if(sEditText == "0") sEditText = "";

    sEditText += iManNo;

    switch(iEditNo){
        default :                                        break ;
        case 1  : edTiltStartPs    -> Text = sEditText ; break ;
        case 2  : edTiltStopPs     -> Text = sEditText ; break ;
        case 3  : edTiltSettingSpd -> Text = sEditText ; break ;
        case 4  : edWtrDrainTime   -> Text = sEditText ; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btBackClick(TObject *Sender)
{
    AnsiString sEditText ;
    switch(iEditNo){
        default :                                       break ;
        case 1  : sEditText = edTiltStartPs    -> Text; break ;
        case 2  : sEditText = edTiltStopPs     -> Text; break ;
        case 3  : sEditText = edTiltSettingSpd -> Text; break ;
        case 4  : sEditText = edWtrDrainTime   -> Text; break ;
    }
    sEditText = sEditText.Delete(sEditText.Length(), 1);

    switch(iEditNo){
        default :                                        break ;
        case 1  : edTiltStartPs    -> Text = sEditText ; break ;
        case 2  : edTiltStopPs     -> Text = sEditText ; break ;
        case 3  : edTiltSettingSpd -> Text = sEditText ; break ;
        case 4  : edWtrDrainTime   -> Text = sEditText ; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btClearClick(TObject *Sender)
{
    switch(iEditNo){
        default :
        case 1  : edTiltStartPs    -> Text = 0; break ;
        case 2  : edTiltStopPs     -> Text = 0; break ;
        case 3  : edTiltSettingSpd -> Text = 0; break ;
        case 4  : edWtrDrainTime   -> Text = 0; break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::btOKClick(TObject *Sender)
{
    int iEdit;
    switch(iEditNo){
        default : break ;
        case 1  : if(edTiltStartPs -> Text == ""){edTiltStartPs -> Text = 0;}
                  if(edTiltStartPs -> Text.ToInt() > 50) edTiltStartPs -> Text = 50;
                  pnTiltStartPs -> Caption  = edTiltStartPs -> Text;
                  OM.DevOptn.iTiltMtStartPs = edTiltStartPs -> Text.ToInt();
                  break ;

        case 2  : if(edTiltStopPs -> Text == ""        ) edTiltStopPs -> Text = 0  ;
                  if(edTiltStopPs -> Text.ToInt() > 130) edTiltStopPs -> Text = 130;
                  pnTiltStopPs  -> Caption = edTiltStopPs -> Text;
                  OM.DevOptn.iTiltMtStopPs = edTiltStopPs -> Text.ToInt();
                  break ;

        case 3  : if(edTiltSettingSpd -> Text == ""        ) edTiltSettingSpd -> Text = 0  ;
                  if(edTiltSettingSpd -> Text.ToInt() > 200) edTiltSettingSpd -> Text = 200;
                  pnTiltSettingSpd -> Caption = edTiltSettingSpd -> Text;
                  OM.DevOptn.iTiltMtSpeed     = edTiltSettingSpd -> Text.ToInt();
                  //pPaix -> MoveRPM(miTiltMt, OM.DevOptn.iTiltMtSpeed);
                  break ;

        case 4  : if(edWtrDrainTime -> Text == ""){edWtrDrainTime -> Text = 0;}
                  pnWtrDrainTime   -> Caption = edWtrDrainTime -> Text;
                  OM.DevOptn.iWaterDrainTime  = edWtrDrainTime -> Text.ToInt() * 1000;
                  break ;
    }
    OM.SaveDevOptn(OM.m_sCrntDev);

    pnButton         -> Visible = false ;
    edTiltStartPs    -> Visible = false ;
    edTiltStopPs     -> Visible = false ;
    edTiltSettingSpd -> Visible = false ;
    edWtrDrainTime   -> Visible = false ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::FormShow(TObject *Sender)
{
    lbModelName -> Caption = g_sModelName;
    OM.LoadDevOptn(OM.m_sCrntDev);

    pnTiltStartPs    -> Caption = OM.DevOptn.iTiltMtStartPs ;
    pnTiltStopPs     -> Caption = OM.DevOptn.iTiltMtStopPs  ;
    pnTiltSettingSpd -> Caption = OM.DevOptn.iTiltMtSpeed   ;
    pnWtrDrainTime   -> Caption = OM.DevOptn.iWaterDrainTime / 1000;

    edTiltStartPs    -> Text = 0;
    edTiltStopPs     -> Text = 0;
    edTiltSettingSpd -> Text = 0;
    edWtrDrainTime   -> Text = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTiltMT::tmTiltMTTimer(TObject *Sender)
{
    tmTiltMT -> Enabled = false;
    double dTiltCmd;

    dTiltCmd = pPaix -> GetCmdPos(miTiltMt);

    AnsiString sTiltCmd ;
    sTiltCmd.printf("%.2f", dTiltCmd);
    pnTiltCmdPs -> Caption = sTiltCmd;
    tmTiltMT -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTiltMT::btTiltHomeClick(TObject *Sender)
{
    pPaix -> MoveHome(miTiltMt);
}
//---------------------------------------------------------------------------
void __fastcall TFrmTiltMT::pnTiltStartPsClick(TObject *Sender)
{
    int iPanelNo = ((TPanel *)Sender) -> Tag;
    iEditNo = iPanelNo;

    pnButton         -> Visible = false ;
    edTiltStartPs    -> Visible = false ;
    edTiltStopPs     -> Visible = false ;
    edTiltSettingSpd -> Visible = false ;
    edWtrDrainTime   -> Visible = false ;

    switch(iPanelNo){
        default : break ;
        case 1  : edTiltStartPs    -> Visible = true; pnButton -> Visible = true; break;
        case 2  : edTiltStopPs     -> Visible = true; pnButton -> Visible = true; break;
        case 3  : edTiltSettingSpd -> Visible = true; pnButton -> Visible = true; break;
        case 4  : edWtrDrainTime   -> Visible = true; pnButton -> Visible = true; break;
    }        
}
//---------------------------------------------------------------------------





