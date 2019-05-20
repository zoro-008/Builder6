//---------------------------------------------------------------------------

#ifndef FormModeH
#define FormModeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMode : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
    TPanel *pnMode;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
protected:
        void __fastcall CreateParams(TCreateParams &Params);  

public:		// User declarations
        __fastcall TFrmMode(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMode *FrmMode;
//---------------------------------------------------------------------------
#endif
