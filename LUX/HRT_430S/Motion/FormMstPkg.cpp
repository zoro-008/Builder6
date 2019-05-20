//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMstPkg.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMstPkg *FrmMstPkg;
//---------------------------------------------------------------------------
__fastcall TFrmMstPkg::TFrmMstPkg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMstPkg::btMstInspcClick(TObject *Sender)
{
    MM.SetManCycle(mcETC_MasterInsp);
    Close();
}
//---------------------------------------------------------------------------
