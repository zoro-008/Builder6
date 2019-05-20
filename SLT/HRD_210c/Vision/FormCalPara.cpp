//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCalPara.h"
#include "FormVisnMain.h"
#include "Vision.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCalPara *FrmCalPara = NULL;
//---------------------------------------------------------------------------
__fastcall TFrmCalPara::TFrmCalPara(TComponent* Owner)
        : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;
}
//---------------------------------------------------------------------------
//__fastcall TFrmCalPara::TFrmCalPara(TComponent* Owner , int _iVisnId , bool _bInit)
//        : TForm(Owner)
//{
//
//
//}

void TFrmCalPara::Show(int _iVisnId)
{
    m_iVisnId = _iVisnId ;

    VISN[m_iVisnId]->Cal -> ShowParaForm(pnBase);

    VISN[m_iVisnId]->Cal -> ApplyPara(true);

    Caption = AnsiString(_iVisnId) + AnsiString(" Vision");

    m_iOriViewMode = VISN[m_iVisnId]->GetViewMode() ;

    VISN[m_iVisnId]->SetViewMode(vmCal) ;
    VISN[m_iVisnId]->PaintCallback();



    TForm::Show();
}


void TFrmCalPara::ApplyPara(bool _bToTable)
{




}



void __fastcall TFrmCalPara::btCloseClick(TObject *Sender)
{
    Trace("Operator", "FormCalPara Form_Close Button Click");
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCalPara::btSaveClick(TObject *Sender)
{
    VISN[m_iVisnId]->Cal -> ApplyPara(false);
    VISN[m_iVisnId]->LoadCalPara(false);
    Trace("Operator", "FormCalPara Form_Save Button Click");
}
//---------------------------------------------------------------------------


void __fastcall TFrmCalPara::btCalClick(TObject *Sender)
{
    VISN[m_iVisnId]->Calibration() ;
    Trace("Operator", "FormCalPara Form_Calibration Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmCalPara::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    VISN[m_iVisnId]->SetViewMode(m_iOriViewMode) ;
    VISN[m_iVisnId]->PaintCallback();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCalPara::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}
//---------------------------------------------------------------------------

