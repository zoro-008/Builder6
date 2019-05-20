//---------------------------------------------------------------------------

#ifndef FrmManualH
#define FrmManualH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormManual : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel26;
    TBitBtn *btnInit;
        TBitBtn *btLotEnd;
        TTimer *tmUpdate;
        TPageControl *PageControl;
        TTabSheet *TabSheet1;
        TBitBtn *btMan1_2;
        TBitBtn *btMan1_3;
        TBitBtn *btMan1_4;
        TBitBtn *btMan1_5;
        TBitBtn *btMan1_6;
        TBitBtn *btMan1_7;
        TBitBtn *btMan1_8;
        TBitBtn *btMan1_9;
        TBitBtn *btMan1_10;
        TBitBtn *btMan1_11;
        TBitBtn *btMan1_12;
        TBitBtn *btMan1_13;
        TBitBtn *btMan1_14;
        TBitBtn *btMan1_15;
        TBitBtn *btMan1_16;
        TBitBtn *btMan1_17;
        TBitBtn *btMan1_18;
        TBitBtn *btMan1_19;
        TBitBtn *btMan1_20;
        TBitBtn *btMan1_21;
        TBitBtn *btMan1_22;
        TBitBtn *btMan1_23;
        TBitBtn *btMan1_24;
        TBitBtn *btMan1_1;
        TTabSheet *TabSheet5;
        TBitBtn *btMan2_1;
        TBitBtn *btMan2_2;
        TBitBtn *btMan2_3;
        TBitBtn *btMan2_4;
        TBitBtn *btMan2_6;
        TBitBtn *btMan2_14;
        TBitBtn *btMan2_7;
        TBitBtn *btMan2_24;
        TBitBtn *btMan2_10;
        TBitBtn *btMan2_11;
        TBitBtn *btMan2_12;
        TBitBtn *btMan2_22;
        TBitBtn *btMan2_23;
        TBitBtn *btMan2_5;
        TBitBtn *btMan2_21;
        TBitBtn *btMan2_13;
        TTimer *Timer1;
        TBitBtn *btMan2_8;
        TBitBtn *btMan2_9;
        TBitBtn *btMan2_16;
        TBitBtn *btMan2_15;
        TBitBtn *btMan2_17;
        TBitBtn *btMan2_18;
        TBitBtn *btMan2_19;
        TBitBtn *btMan2_20;
    void __fastcall btMan1_1Click(TObject *Sender);
    void __fastcall btLotEndClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall btnInitClick(TObject *Sender);
        void __fastcall tmUpdateTimerTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormManual(TComponent* Owner);

   TPanel *Panel;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormManual *FormManual;
//---------------------------------------------------------------------------
#endif

