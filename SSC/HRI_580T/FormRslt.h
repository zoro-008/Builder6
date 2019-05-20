//---------------------------------------------------------------------------

#ifndef FormRsltH
#define FormRsltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmRslt : public TForm
{
__published:	// IDE-managed Components
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
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
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
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmRslt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmRslt *FrmRslt;
//---------------------------------------------------------------------------
#endif
