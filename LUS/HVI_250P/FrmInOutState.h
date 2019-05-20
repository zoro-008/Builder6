//---------------------------------------------------------------------------

#ifndef FrmInOutStateH
#define FrmInOutStateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormInOutState : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
        TLabel *Label2;
        TPanel *pnInput;
        TPanel *pnOutput;
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormInOutState(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormInOutState *FormInOutState;
//---------------------------------------------------------------------------
#endif
