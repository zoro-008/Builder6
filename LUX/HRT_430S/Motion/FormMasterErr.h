//---------------------------------------------------------------------------

#ifndef FormMasterErrH
#define FormMasterErrH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmMasterErr : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPanel *Panel1;
        TPanel *Panel5;
        TBitBtn *btLDRRetry;
        TImage *Image1;
        TTimer *tmLotName;
        void __fastcall btLDRRetryClick(TObject *Sender);
        void __fastcall btLDRSkipThisPickClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmMasterErr(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVisnErr *FrmMasterErr;
//---------------------------------------------------------------------------
#endif
