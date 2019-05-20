//---------------------------------------------------------------------------

#ifndef FormErrListH
#define FormErrListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmErrList : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *sgErrList;
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmErrList(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmErrList *FrmErrList;
//---------------------------------------------------------------------------
#endif
