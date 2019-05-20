//---------------------------------------------------------------------------

#ifndef FormMstPkgH
#define FormMstPkgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMstPkg : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TImage *Image1;
        TPanel *Panel5;
        TBitBtn *btMstInspc;
        void __fastcall btMstInspcClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmMstPkg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMstPkg *FrmMstPkg;
//---------------------------------------------------------------------------
#endif
