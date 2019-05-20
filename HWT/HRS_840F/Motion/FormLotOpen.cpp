//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLotOpen.h"
#include "LotUnit.h"
#include "DataMan.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLotOpen *FrmLotOpen;
//---------------------------------------------------------------------------
__fastcall TFrmLotOpen::TFrmLotOpen(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btLotOpenClick(TObject *Sender)
{
    AnsiString LotNo    = edLotId     -> Text.Trim() ;
    AnsiString Device   = edSelDevice -> Text.Trim() ;

    //AnsiString Operator = edOperator  -> Text.Trim() ;

    LOT.LotOpen(LotNo , Device) ;


    Trace("LotOpen" , "Try");



    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmLotOpen::FormShow(TObject *Sender)
{
    edSelDevice -> Text = OM.GetCrntDev();
    //edAutoLot   -> Text = LOT.GetAutoLotNo() ;


    edLotId -> SetFocus();

}
//--------------------------------------------------------------------------
void __fastcall TFrmLotOpen::btCancelClick(TObject *Sender)
{
    Trace("LotOpen","Canceled") ;
    Close();
}
//---------------------------------------------------------------------------


void __fastcall TFrmLotOpen::tmLotNameTimer(TObject *Sender)
{
    tmLotName -> Enabled = false ;

    tmLotName -> Enabled = true ;
}
//---------------------------------------------------------------------------


