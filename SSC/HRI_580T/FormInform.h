//---------------------------------------------------------------------------

#ifndef FormInformH
#define FormInformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmInform : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
private:	// User declarations
public:		// User declarations
    __fastcall TFrmInform(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInform *FrmInform;
//---------------------------------------------------------------------------
#endif
