//---------------------------------------------------------------------------

#ifndef FormVisionSetUpH
#define FormVisionSetUpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmVisionSetUp : public TForm
{
__published:	// IDE-managed Components
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    TWinControl * OriParent ;
public:		// User declarations
    __fastcall TFrmVisionSetUp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVisionSetUp *FrmVisionSetUp;
//---------------------------------------------------------------------------
#endif
