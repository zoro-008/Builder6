//---------------------------------------------------------------------------

#ifndef FormSerialH
#define FormSerialH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmSerial : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edSerial;
        TButton *btSetSerial;
        TListBox *lbSerial;
        TButton *btClose;
        void __fastcall btSetSerialClick(TObject *Sender);
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmSerial(TComponent* Owner);
        bool CheckSourceSerial();
        AnsiString GetSerial();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSerial *FrmSerial;
//---------------------------------------------------------------------------
#endif
