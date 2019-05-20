//---------------------------------------------------------------------------

#ifndef FormRepeatH
#define FormRepeatH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmRepeat : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *Edit1;
        TButton *btCrntRepeat;
        TButton *btFullRepeat;
        TButton *btStop;
        TTimer *Timer1;
        TLabel *Label2;
        TLabel *lbCntLeft;
        TButton *btPinRepeat;
        TButton *btChcukPinRepeat;
        void __fastcall btFullRepeatClick(TObject *Sender);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btCrntRepeatClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall btPinRepeatClick(TObject *Sender);
        void __fastcall btChcukPinRepeatClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmRepeat(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmRepeat *FrmRepeat;
//---------------------------------------------------------------------------
#endif
