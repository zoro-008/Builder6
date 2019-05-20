//---------------------------------------------------------------------------

#ifndef FormMultiH
#define FormMultiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMulti : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnBase1;
        TPanel *pnBase2;
        TPanel *pnBase3;
        TPanel *pnBase4;
        TPanel *pnBase0;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmMulti(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMulti *FrmMulti;
//---------------------------------------------------------------------------
#endif
