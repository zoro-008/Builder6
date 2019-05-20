//---------------------------------------------------------------------------

#ifndef FormHelpH
#define FormHelpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmHelp : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *tsHelp1;
        TMemo *Memo2;
        TTabSheet *tsHelp2;
        TMemo *Memo3;
private:	// User declarations
public:		// User declarations
        __fastcall TFrmHelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmHelp *FrmHelp;
//---------------------------------------------------------------------------
#endif
