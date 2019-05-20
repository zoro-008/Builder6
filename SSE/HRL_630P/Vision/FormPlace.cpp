//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormPlace.h"
#include "Epoxy.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "FormCamLightPara.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmPlace *FrmPlace;
//---------------------------------------------------------------------------
__fastcall TFrmPlace::TFrmPlace(TComponent* Owner)
        : TForm(Owner)
{

}

void TFrmPlace::Init()
{
    Left = 0 ;
    Top  = 0 ;

    m_iVisnId = viPlc ;
}

void TFrmPlace::Close()
{

}
//---------------------------------------------------------------------------

void TFrmPlace::ApplyPara(bool _bToTable)
{
    if(_bToTable) {
        cbImageSave     -> ItemIndex = VOM.VCmnOptn.iPlcImageSave         ;

        edCountX        -> Text      = VOM.VDevOptn.iCountX               ;
        edCountY        -> Text      = VOM.VDevOptn.iCountY               ;

        sbThresholdLow  -> Position  = VOM.VDevPara.PlcTsd.iThresholdLow  ;
        sbThresholdHigh -> Position  = VOM.VDevPara.PlcTsd.iThresholdHigh ;

        sbPlcPxMinPer   -> Position  = VOM.VDevOptn.dPlcPxMinPer          ;
    }
    else {
        VOM.VCmnOptn.iPlcImageSave         = cbImageSave     -> ItemIndex ;

        VOM.VDevOptn.iCountX               = StrToFloatDef(edCountX-> Text , VOM.VDevOptn.iCountX);
        VOM.VDevOptn.iCountY               = StrToFloatDef(edCountY-> Text , VOM.VDevOptn.iCountY);

        VOM.VDevPara.PlcTsd.iThresholdLow  = sbThresholdLow  -> Position  ;
        VOM.VDevPara.PlcTsd.iThresholdHigh = sbThresholdHigh -> Position  ;

        VOM.VDevOptn.dPlcPxMinPer          = sbPlcPxMinPer   -> Position ;

        ApplyPara(true);
    }
}


void __fastcall TFrmPlace::btSaveClick(TObject *Sender)
{
    ApplyPara(false);
    VOM.LoadJobFile(false);
    //Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmPlace::FormShow(TObject *Sender)
{
    ApplyPara(true);
}
//---------------------------------------------------------------------------

void TFrmPlace::FormShow(int _iPage)
{
    Show();
}







void __fastcall TFrmPlace::btCamLightClick(TObject *Sender)
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

void __fastcall TFrmPlace::sbThresholdLowChange(TObject *Sender)
{
    edThresholdLow -> Text = sbThresholdLow -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPlace::sbThresholdHighChange(TObject *Sender)
{
    edThresholdHigh -> Text = sbThresholdHigh -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPlace::sbPlcPxMinPerChange(TObject *Sender)
{
    edPlcPxMinPer -> Text = sbPlcPxMinPer -> Position ;
}
//---------------------------------------------------------------------------

