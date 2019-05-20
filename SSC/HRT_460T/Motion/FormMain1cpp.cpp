//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain1cpp.h"
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
    FM_MakeWin(pnBase) ;
    //init
    Init();
}
//---------------------------------------------------------------------------
