//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAfterPick.h"
#include "Epoxy.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "FormCamLightPara.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmAfterPick *FrmAfterPick;
//---------------------------------------------------------------------------
__fastcall TFrmAfterPick::TFrmAfterPick(TComponent* Owner)
        : TForm(Owner)
{

}

void TFrmAfterPick::Init()
{
    Left = 0 ;
    Top  = 0 ;

    m_iVisnId = viAtp ;
}

void TFrmAfterPick::Close()
{
    TForm::Close();
}
//---------------------------------------------------------------------------

void TFrmAfterPick::ApplyPara(bool _bToTable)
{
    if(_bToTable) {
        cbImageSave     -> ItemIndex = VOM.VCmnOptn.iAtpImageSave         ;

        edCountX        -> Text      = VOM.VDevOptn.iCountX               ;
        edCountY        -> Text      = VOM.VDevOptn.iCountY               ;

        sbThresholdLow  -> Position  = VOM.VDevPara.AtpTsd.iThresholdLow  ;
        sbThresholdHigh -> Position  = VOM.VDevPara.AtpTsd.iThresholdHigh ;

        edAtpPxMinPer   -> Text      = VOM.VDevOptn.dAtpPxMinPer          ;
    }
    else {
        VOM.VCmnOptn.iAtpImageSave         = cbImageSave     -> ItemIndex ;

        VOM.VDevOptn.iCountX               = StrToFloatDef(edCountX-> Text , VOM.VDevOptn.iCountX);
        VOM.VDevOptn.iCountY               = StrToFloatDef(edCountY-> Text , VOM.VDevOptn.iCountY);

        VOM.VDevPara.AtpTsd.iThresholdLow  = sbThresholdLow  -> Position  ;
        VOM.VDevPara.AtpTsd.iThresholdHigh = sbThresholdHigh -> Position  ;

        VOM.VDevOptn.dAtpPxMinPer          = StrToFloatDef(edAtpPxMinPer -> Text , VOM.VDevOptn.dAtpPxMinPer ) ;

        ApplyPara(true);
    }
}


void __fastcall TFrmAfterPick::btSaveClick(TObject *Sender)
{
    ApplyPara(false);
    VOM.LoadJobFile(false);
    //Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAfterPick::FormShow(TObject *Sender)
{
    ApplyPara(true);
}
//---------------------------------------------------------------------------

void TFrmAfterPick::FormShow(int _iPage)
{
    Show();
}







void __fastcall TFrmAfterPick::btCamLightClick(TObject *Sender)
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

void __fastcall TFrmAfterPick::sbThresholdLowChange(TObject *Sender)
{
    edThresholdLow -> Text = sbThresholdLow -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmAfterPick::sbThresholdHighChange(TObject *Sender)
{
    edThresholdHigh -> Text = sbThresholdHigh -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmAfterPick::sbAtpPxMinPerChange(TObject *Sender)
{
    edAtpPxMinPer -> Text = sbAtpPxMinPer -> Position ;
}
//---------------------------------------------------------------------------

