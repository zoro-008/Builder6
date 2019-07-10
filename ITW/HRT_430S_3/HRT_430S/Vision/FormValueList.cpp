//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormValueList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmValueList *FrmValueList;
//---------------------------------------------------------------------------
__fastcall TFrmValueList::TFrmValueList(TComponent* Owner)
        : TForm(Owner)
{
    m_pSelVal = NULL ;
    m_pValueList = NULL ;
}
//---------------------------------------------------------------------------

void TFrmValueList::DisplayList()
{
    m_pValueList -> UpdateList(true , sgValueList);
}




void __fastcall TFrmValueList::AddClick(TObject *Sender)
{
    m_pValueList -> Add(edName -> Text , edComment -> Text , (CValue::EN_VAL_TYPE)cbType ->ItemIndex );
    DisplayList();
}
//---------------------------------------------------------------------------

void __fastcall TFrmValueList::InsertClick(TObject *Sender)
{
    m_pValueList -> Insert(sgValueList -> Row - 1 , edName -> Text , edComment -> Text , (CValue::EN_VAL_TYPE)cbType ->ItemIndex );
    DisplayList();
}
//---------------------------------------------------------------------------

void __fastcall TFrmValueList::DeleteClick(TObject *Sender)
{
    m_pValueList -> Delete(sgValueList -> Row - 1 );
    DisplayList();
}
//---------------------------------------------------------------------------

void __fastcall TFrmValueList::DeleteAllClick(TObject *Sender)
{
    m_pValueList -> DeleteAll();
    DisplayList();
}
//---------------------------------------------------------------------------

void __fastcall TFrmValueList::Button1Click(TObject *Sender)
{
    if(m_pSelVal) {
        m_pSelVal -> SetName   (edName    -> Text     ) ;
        m_pSelVal -> SetComment(edComment -> Text     ) ;
        m_pSelVal -> SetValType(cbType    -> ItemIndex) ;
        m_pSelVal -> SetMinVal (StrToFloatDef(edMinVal  -> Text,0)) ;
        m_pSelVal -> SetMaxVal (StrToFloatDef(edMaxVal  -> Text,0)) ;
    }



    m_pValueList -> LoadSave(false);

    DisplayList();
}
//---------------------------------------------------------------------------

void __fastcall TFrmValueList::FormShow(TObject *Sender)
{
    DisplayList();
    UpdateProperty(sgValueList -> Row - 1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmValueList::sgValueListClick(TObject *Sender)
{
    UpdateProperty(sgValueList -> Row - 1);
}
//---------------------------------------------------------------------------


void TFrmValueList::UpdateProperty(int _iNo)
{
    m_pSelVal = m_pValueList -> GetValueByName(sgValueList ->Cells[1][_iNo+1]);

    if(m_pSelVal == NULL) return ;

    edName    -> Text      = m_pSelVal -> GetName()    ;
    edComment -> Text      = m_pSelVal -> GetComment() ;
    cbType    -> ItemIndex = m_pSelVal -> GetValType() ;
    edMinVal  -> Text      = m_pSelVal -> GetMinVal()  ;
    edMaxVal  -> Text      = m_pSelVal -> GetMaxVal()  ;



}
