//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVisionSetUp.h"
#include "FormVisnMain.h"

#include "VisionMan.h"
//#include "FormOperation.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmVisionSetUp *FrmVisionSetUp;
//---------------------------------------------------------------------------
__fastcall TFrmVisionSetUp::TFrmVisionSetUp(TComponent* Owner)
    : TForm(Owner)
{




}
//---------------------------------------------------------------------------
void __fastcall TFrmVisionSetUp::FormShow(TObject *Sender)
{



    /*
    VSM.Live(viOri   , false);
    VSM.Live(viWfr   , false);
    VSM.Live(viRalAt , false);
    VSM.Live(viRalBf , false);

    OriParent = FrmVisnMain -> Parent ;
    FrmVisnMain -> Parent = pnVisnBase ;
    FrmVisnMain -> Align  = alClient   ;
    FrmVisnMain -> Show();
    */

}
//---------------------------------------------------------------------------
void __fastcall TFrmVisionSetUp::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    /*
    VSM.Live(viOri   , false);
    VSM.Live(viWfr   , false);
    VSM.Live(viRalAt , false);
    VSM.Live(viRalBf , false);

    FrmVisnMain -> Parent = OriParent ;
    FrmVisnMain -> Align = alClient ;
    FrmVisnMain -> Show();
    */
}
//---------------------------------------------------------------------------
void __fastcall TFrmVisionSetUp::FormCreate(TObject *Sender)
{
    FrmVisnMain -> Parent = this ;
    FrmVisnMain -> Align  = alClient   ;
    FrmVisnMain -> Show();

    SetWindowPos(this->Handle , HWND_TOP	 , 0 , 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE/* | SWP_NOOWNERZORDER*/);
}
//---------------------------------------------------------------------------

