//---------------------------------------------------------------------------

#ifndef FrmOptionH
#define FrmOptionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormOption : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label85;
        TEdit *edWK2StprDnDelay;
    TLabel *Label3;
        TEdit *edWK1_XInsVel;
    TLabel *Label11;
    TLabel *Label16;
    TBitBtn *btSave;
        TCheckBox *cbAutoVsChange;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *edPrbToWk1Delay;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *edPsbToOutDelay;
        TCheckBox *cbEmptyNoMrk;
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
public:		// User declarations
    __fastcall TFormOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOption *FormOption;
//---------------------------------------------------------------------------
#endif

