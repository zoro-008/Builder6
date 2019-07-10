//---------------------------------------------------------------------------

#ifndef FormRsltH
#define FormRsltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmRslt : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label16;
        TLabel *Label17;
        TEdit *edLotNo;
        TEdit *edFail;
        TEdit *edWork;
        TLabel *Label18;
        TEdit *edTime;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmRslt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmRslt *FrmRslt;
//---------------------------------------------------------------------------
#endif
