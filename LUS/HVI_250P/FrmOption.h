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
    TBitBtn *btSave;
        TCheckBox *cbAutoOperation;
        TCheckBox *cbAutoConvesion;
        TEdit *ed1IdxReWorkCnt;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edPunchAirDelay;
        TLabel *Label4;
        TEdit *edPreIdxOutSpd;
        TLabel *Label5;
        TLabel *Label6;
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

