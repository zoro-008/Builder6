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
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TPanel *Panel1;
        TPanel *Panel5;
        TBitBtn *btLDRRetry;
        TBitBtn *btLDRSkipThisPick;
        TBitBtn *btLDRSkipThisRow;
        TBitBtn *btLDRSkipThisTray;
        TBitBtn *btLDRSkipThisRowTrayOut;
        TBitBtn *btLDRTrayOut;
        TBitBtn *btLDRTrayOutLotEnd;
        TBitBtn *btLDRSkipThisRowTrayOutLotEnd;
        TPanel *Panel2;
        TBitBtn *btZIGSkip;
        TBitBtn *btZIGRetry;
        TPanel *Panel3;
        TBitBtn *btSTGRetry;
        TBitBtn *btSTGSkip;
        TImage *Image1;
        TTabSheet *TabSheet4;
        TPanel *Panel4;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TTimer *tmLotName;
        TPanel *pnLDRVacErr4;
        TPanel *pnLDRVacErr3;
        TPanel *pnLDRVacErr2;
        TPanel *pnLDRVacErr1;
        void __fastcall btLDRRetryClick(TObject *Sender);
        void __fastcall btLDRSkipThisPickClick(TObject *Sender);
        void __fastcall btLDRSkipThisRowClick(TObject *Sender);
        void __fastcall btLDRSkipThisTrayClick(TObject *Sender);
        void __fastcall btLDRSkipThisRowTrayOutClick(TObject *Sender);
        void __fastcall btLDRTrayOutClick(TObject *Sender);
        void __fastcall btLDRSkipThisRowTrayOutLotEndClick(TObject *Sender);
        void __fastcall btLDRTrayOutLotEndClick(TObject *Sender);
        void __fastcall btZIGRetryClick(TObject *Sender);
        void __fastcall btZIGSkipClick(TObject *Sender);
        void __fastcall btSTGRetryClick(TObject *Sender);
        void __fastcall btSTGSkipClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmVacErr(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVacErr *FrmVacErr;
//---------------------------------------------------------------------------
#endif
