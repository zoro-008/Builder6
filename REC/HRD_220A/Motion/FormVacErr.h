//---------------------------------------------------------------------------

#ifndef FormVacErrH
#define FormVacErrH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmVacErr : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPanel *Panel1;
        TPanel *Panel5;
        TBitBtn *btRetry;
        TBitBtn *btSkip;
        TBitBtn *btLotEnd;
        TTimer *tmLotName;
        void __fastcall btRetryClick(TObject *Sender);
        void __fastcall btSkipClick(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmVacErr(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVacErr *FrmVacErr;
//---------------------------------------------------------------------------
#endif
