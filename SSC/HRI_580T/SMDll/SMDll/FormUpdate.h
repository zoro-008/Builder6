//---------------------------------------------------------------------------

#ifndef FormUpdateH
#define FormUpdateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmUpdate : public TForm
{
__published:	// IDE-managed Components
        TMemo *mmUpdate;
private:	// User declarations
public:		// User declarations
        __fastcall TFrmUpdate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmUpdate *FrmUpdate;
//---------------------------------------------------------------------------
#endif
