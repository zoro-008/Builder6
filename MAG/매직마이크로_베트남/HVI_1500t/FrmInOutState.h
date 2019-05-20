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
        TTimer *tmUpdate;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormInOutState(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormInOutState *FormInOutState;
//---------------------------------------------------------------------------
#endif
