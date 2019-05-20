//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormBeforePick.h"
#include "Epoxy.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "FormCamLightPara.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmBeforePick *FrmBeforePick;
//---------------------------------------------------------------------------
__fastcall TFrmBeforePick::TFrmBeforePick(TComponent* Owner)
        : TForm(Owner)
{

}

void TFrmBeforePick::Init()
{
    Left = 0 ;
    Top  = 0 ;

    m_iVisnId = viBfp ;
}

void TFrmBeforePick::Close()
{

}
//---------------------------------------------------------------------------

void TFrmBeforePick::ApplyPara(bool _bToTable)
{
    if(_bToTable) {
        cbImageSave     -> ItemIndex = VOM.VCmnOptn.iBfpImageSave         ;

        edCountX        -> Text      = VOM.VDevOptn.iCountX               ;
        edCountY        -> Text      = VOM.VDevOptn.iCountY               ;

        sbThresholdLow  -> Position  = VOM.VDevPara.BfpTsd.iThresholdLow  ;
        sbThresholdHigh -> Position  = VOM.VDevPara.BfpTsd.iThresholdHigh ;

        edBfpPxMinPer   -> Text      = VOM.VDevOptn.dBfpPxMinPer          ;
    }
    else {
        VOM.VCmnOptn.iBfpImageSave         = cbImageSave     -> ItemIndex ;

        VOM.VDevOptn.iCountX               = StrToFloatDef(edCountX-> Text , VOM.VDevOptn.iCountX);
        VOM.VDevOptn.iCountY               = StrToFloatDef(edCountY-> Text , VOM.VDevOptn.iCountY);

        VOM.VDevPara.BfpTsd.iThresholdLow  = sbThresholdLow  -> Position  ;
        VOM.VDevPara.BfpTsd.iThresholdHigh = sbThresholdHigh -> Position  ;

        VOM.VDevOptn.dBfpPxMinPer          = StrToFloatDef(edBfpPxMinPer -> Text , VOM.VDevOptn.dBfpPxMinPer ) ;

        ApplyPara(true);
    }
}


void __fastcall TFrmBeforePick::btSaveClick(TObject *Sender)
{
    ApplyPara(false);
    VOM.LoadJobFile(false);
    //Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmBeforePick::FormShow(TObject *Sender)
{
    ApplyPara(true);
}
//---------------------------------------------------------------------------

void TFrmBeforePick::FormShow(int _iPage)
{
    Show();
}







void __fastcall TFrmBeforePick::btCamLightClick(TObject *Sender)
{
    if(FrmCamLightPara!=NULL) {
        delete FrmCamLightPara ;
    }

    FrmCamLightPara = new TFrmCamLightPara (this);

    FrmCamLightPara -> BorderStyle = bsNone ;
    FrmCamLightPara -> Parent = pnBase ;
    FrmCamLightPara -> Align = alClient ;
    FrmCamLightPara -> Show(viEpx) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBeforePick::sbThresholdLowChange(TObject *Sender)
{
    edThresholdLow -> Text = sbThresholdLow -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBeforePick::sbThresholdHighChange(TObject *Sender)
{
    edThresholdHigh -> Text = sbThresholdHigh -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBeforePick::sbBfpPxMinPerChange(TObject *Sender)
{
    edBfpPxMinPer -> Text = sbBfpPxMinPer -> Position ;
}
//---------------------------------------------------------------------------

