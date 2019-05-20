//---------------------------------------------------------------------------

#ifndef FrmRsltBigH
#define FrmRsltBigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormRsltBig : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *lbR1;
        TLabel *lbR2;
        TLabel *lbR3;
        TLabel *lbR4;
        TLabel *lbR5;
        TLabel *lbR6;
        TLabel *lbR7;
        TLabel *lbR8;
        TLabel *lbR9;
        TLabel *lbR10;
        TLabel *lbR11;
        TLabel *lbR12;
        TLabel *lbR13;
        TLabel *lbR14;
        TLabel *Label16;
        TLabel *Label17;
        TEdit *edLotNo;
        TEdit *edFail;
        TEdit *edRslt1;
        TEdit *edRslt2;
        TEdit *edRslt3;
        TEdit *edRslt4;
        TEdit *edRslt5;
        TEdit *edRslt6;
        TEdit *edRslt7;
        TEdit *edRslt8;
        TEdit *edRslt9;
        TEdit *edRslt10;
        TEdit *edRslt11;
        TEdit *edRslt12;
        TEdit *edRslt13;
        TEdit *edRslt14;
        TEdit *edWork;
        TLabel *Label18;
        TEdit *edTime;
        TLabel *Label19;
        TEdit *edFailTotal;
        TLabel *Label44;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label50;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label20;
        TLabel *Label2;
        TEdit *edWorkedChip;
        TLabel *Label3;
        TEdit *edWorkedFrame;
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormRsltBig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRsltBig *FormRsltBig;
//---------------------------------------------------------------------------
#endif
