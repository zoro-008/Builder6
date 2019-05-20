//---------------------------------------------------------------------------

#ifndef FrmLimitCntH
#define FrmLimitCntH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLimitCnt : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label80;
        TEdit *edTotalFailCnt;
        TLabel *lbR2;
        TEdit *edR1FailCnt;
        TLabel *lbR3;
        TEdit *edR2FailCnt;
        TLabel *lbR4;
        TEdit *edR3FailCnt;
        TLabel *lbR5;
        TEdit *edR4FailCnt;
        TLabel *lbR6;
        TEdit *edR5FailCnt;
        TLabel *lbR7;
        TEdit *edR6FailCnt;
        TLabel *lbR8;
        TEdit *edR7FailCnt;
        TLabel *lbR9;
        TEdit *edR8FailCnt;
        TLabel *lbR10;
        TEdit *edR9FailCnt;
        TLabel *lbR11;
        TEdit *edR10FailCnt;
        TLabel *lbR12;
        TEdit *edR11FailCnt;
        TLabel *lbR13;
        TEdit *edR12FailCnt;
        TLabel *lbR14;
        TEdit *edR13FailCnt;
        TBitBtn *btYes;
        TBitBtn *btNo;
        TLabel *lbR1;
        TEdit *edR0FailCnt;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel6;
        TPanel *Panel7;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *Panel11;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *Panel14;
        TPanel *Panel15;
        TCheckBox *cbR1FailSkip;
        TCheckBox *cbR2FailSkip;
        TCheckBox *cbR3FailSkip;
        TCheckBox *cbR4FailSkip;
        TCheckBox *cbR5FailSkip;
        TCheckBox *cbR6FailSkip;
        TCheckBox *cbR7FailSkip;
        TCheckBox *cbR8FailSkip;
        TCheckBox *cbR9FailSkip;
        TCheckBox *cbR10FailSkip;
        TCheckBox *cbR11FailSkip;
        TCheckBox *cbR12FailSkip;
        TCheckBox *cbR13FailSkip;
        TCheckBox *cbR14FailSkip;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        void __fastcall btNoClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btYesClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateCmmOptn(bool bTable);
public:		// User declarations
    __fastcall TFormLimitCnt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLimitCnt *FormLimitCnt;
//---------------------------------------------------------------------------
#endif

