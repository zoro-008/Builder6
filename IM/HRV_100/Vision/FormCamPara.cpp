//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCamPara.h"
#include "CamUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCamPara *FrmCamPara;
//---------------------------------------------------------------------------
__fastcall TFrmCamPara::TFrmCamPara(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCamPara::FormShow(TObject *Sender)
{
    g_pCam[ciCam1]->ShowParaForm(pnBase);
}
//---------------------------------------------------------------------------
void __fastcall TFrmCamPara::btSaveClick(TObject *Sender)
{
    g_pCam[ciCam1]->UpdatePara(false);
    g_pCam[ciCam1]->LoadPara(false , EXE_FOLDER + "\\Util\\Vision\\ImCamPara.ini");
}
//---------------------------------------------------------------------------
                                                              //D:\BC6Works\IM\HRV_100\bin\Util\Vision
