//---------------------------------------------------------------------------

#ifndef FormInfoH
#define FormInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmInfo : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
private:	// User declarations
public:		// User declarations
        __fastcall TFrmInfo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInfo *FrmInfo;
//---------------------------------------------------------------------------
#endif
