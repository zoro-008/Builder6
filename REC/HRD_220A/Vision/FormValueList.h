//---------------------------------------------------------------------------

#ifndef FormValueListH
#define FormValueListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "UnitValueList.h"
//---------------------------------------------------------------------------
class TFrmValueList : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnProperty;
        TStringGrid *sgValueList;
        TButton *DeleteAll;
        TButton *Add;
        TButton *Delete;
        TButton *Insert;
        TEdit *edName;
        TLabel *Label1;
        TEdit *edComment;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edMinVal;
        TLabel *Label4;
        TEdit *edMaxVal;
        TLabel *Label5;
        TComboBox *cbType;
        TButton *btSave;
        void __fastcall AddClick(TObject *Sender);
        void __fastcall InsertClick(TObject *Sender);
        void __fastcall DeleteClick(TObject *Sender);
        void __fastcall DeleteAllClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall sgValueListClick(TObject *Sender);
        void __fastcall sgValueListSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
private:	// User declarations

        CValueList * m_pValueList ;

        CValue2 * m_pSelVal ;

        void DisplayList();
public:		// User declarations
        __fastcall TFrmValueList(TComponent* Owner);
        void SetValueList (CValueList * _pValueList){m_pValueList = _pValueList;}
        void UpdateProperty(int _iNo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmValueList *FrmValueList;
//---------------------------------------------------------------------------
#endif
