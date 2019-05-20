//---------------------------------------------------------------------------

#ifndef FormMapParaH
#define FormMapParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmMapPara : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TLabel *lbExplain;
private:	// User declarations
public:		// User declarations
        __fastcall TFrmInform(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMapPara *FrmMapPara;
//---------------------------------------------------------------------------
#endif
