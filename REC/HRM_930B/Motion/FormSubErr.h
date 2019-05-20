//---------------------------------------------------------------------------

#ifndef FormSubErrH
#define FormSubErrH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
//enum EN_ERR_TYPE {
//    etLeftVisn   = 0 ,
//    etRightVisn  = 1 ,
//    etEndVisn    = 2 ,
//    etHeight     = 3
//};

class TFrmSubErr : public TForm
{
__published:	// IDE-managed Components
        TPageControl *pcSubErr;
        TTabSheet *TabSheet1;
        TTimer *tmLotName;
        TPanel *Panel5;
        TBitBtn *btRetry;
        TBitBtn *btVacSkip;
        TImage *Image1;
        void __fastcall btRetryClick(TObject *Sender);
        void __fastcall btVacSkipClick(TObject *Sender);
        void __fastcall btVisnSkipClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmSubErr(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSubErr *FrmSubErr;
//---------------------------------------------------------------------------
#endif
