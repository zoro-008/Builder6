//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormEpoxyChange.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "FormLotOpen.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmEpoxyChange *FrmEpoxyChange;
//---------------------------------------------------------------------------
__fastcall TFrmEpoxyChange::TFrmEpoxyChange(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmEpoxyChange::FormShow(TObject *Sender)
{
    edDspEpoxy -> Text = OM.DevOptn.sEpoxyName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmEpoxyChange::btEpoxyCancelClick(TObject *Sender)
{
    FrmEpoxyChange -> Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmEpoxyChange::btEpoxyOKClick(TObject *Sender)
{
    LOT.SetEpoxyLotNo(edEpoxyLotNo -> Text);
    //if(!LOT.GetLotOpen()) FrmLotOpen -> ShowModal();
    FrmEpoxyChange -> Close();
}
//---------------------------------------------------------------------------

