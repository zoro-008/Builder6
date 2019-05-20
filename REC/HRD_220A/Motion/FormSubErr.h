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
enum EN_ERR_TYPE {
    etPCKVac   = 0 ,
    etSTGVac   = 1 ,
    etPCKVsn   = 2 ,
    etDSPVsnBf = 3 ,
    etDSPVsnAt = 4 
};
class TFrmSubErr : public TForm
{
__published:	// IDE-managed Components
        TPageControl *pcSubErr;
        TTabSheet *TabSheet1;
        TTimer *tmLotName;
        TTabSheet *TabSheet2;
        TPanel *Panel2;
        TBitBtn *btVisnRetry;
        TBitBtn *btVisnSkip;
        TPanel *Panel5;
        TBitBtn *btRetry;
        TBitBtn *btVacSkip;
        TBitBtn *btLotEnd;
        TImage *Image1;
        void __fastcall btRetryClick(TObject *Sender);
        void __fastcall btVacSkipClick(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
        void __fastcall btVisnSkipClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmSubErr(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSubErr *FrmSubErr;
//---------------------------------------------------------------------------
#endif
