//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAlgoPara.h"
#include "VisionBase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAlgoPara *FrmAlgoPara;
//---------------------------------------------------------------------------
__fastcall TFrmAlgoPara::TFrmAlgoPara(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void TFrmAlgoPara::Init()
{
}

void TFrmAlgoPara::FormShow (EN_VISN_ID _iVisnId)
{
    Left = 100 ;
    Top  = 100 ;

    m_iVisnId = _iVisnId ;

    VISN[m_iVisnId]->ShowAlgoPara(vePara) ;

    Show();
}

void __fastcall TFrmAlgoPara::btCloseClick(TObject *Sender)
{
    Close();        
}
//---------------------------------------------------------------------------


void __fastcall TFrmAlgoPara::btSaveClick(TObject *Sender)
{
    VISN[m_iVisnId]->UpdateAlgoPara(false , vePara) ;
    VISN[m_iVisnId]->Load          (false) ;
}
//---------------------------------------------------------------------------

