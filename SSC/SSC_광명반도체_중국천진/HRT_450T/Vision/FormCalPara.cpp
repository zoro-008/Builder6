//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCalPara.h"
#include "VisionBase.h"
#include "CalibrationUnit.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCalPara *FrmCalPara;
//---------------------------------------------------------------------------
__fastcall TFrmCalPara::TFrmCalPara(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TFrmCalPara::Init()
{
}

void TFrmCalPara::FormShow (EN_VISN_ID  _iVisnId)
{
    Left = 100 ;
    Top  = 100 ;

    m_iVisnId = _iVisnId ;

    //ShowPara(TWinControl* _wcBase);

    TVisnStaticPara Para = Vision[m_iVisnId]->GetVisnStaticPara() ;

    Cal[Para.iCalId]->ShowPara(pnBase) ;
}

void __fastcall TFrmCalPara::btCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCalPara::btSaveClick(TObject *Sender)
{
    TVisnStaticPara Para = Vision[m_iVisnId]->GetVisnStaticPara() ;
    Cal[Para.iCalId]->ApplyPara(false);
    Cal[Para.iCalId]->Load(false);
}
//---------------------------------------------------------------------------

