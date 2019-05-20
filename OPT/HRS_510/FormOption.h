//---------------------------------------------------------------------------

#ifndef FormOptionH
#define FormOptionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel5;
    TImage *Image2;
    TLabel *Label19;
        TBitBtn *btSave;
        TCheckBox *cbLoadingStop;
        TPanel *Panel1;
        TPanel *Panel2;
        TImage *Image1;
        TLabel *Label1;
        TLabel *Label17;
        TEdit *edVacDelay;
        TLabel *Label18;
        TLabel *Label2;
        TEdit *edPlaceDelay;
        TLabel *Label3;
        TCheckBox *cbEmptyIgnr;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *edPlaceEjtTime;
    TLabel *Label8;
        TEdit *edRezeroTrayCnt;
    TLabel *Label9;
    TCheckBox *cbChuckFailIgnore;
    TCheckBox *cbUseBinFullIgnore;
    TPanel *Panel4;
    TPanel *Panel6;
    TImage *Image3;
    TLabel *Label23;
    TEdit *edsRsltName2;
    TEdit *edsRsltName3;
    TEdit *edsRsltName4;
    TEdit *edsRsltName5;
    TEdit *edsRsltName6;
    TEdit *edsRsltName7;
    TEdit *edsRsltName8;
    TEdit *edsRsltName9;
    TEdit *edsRsltNameA;
    TEdit *edsRsltNameB;
    TEdit *edsRsltNameC;
    TEdit *edsRsltName1;
    TPanel *pnRsltColor1;
    TPanel *pnRsltColor2;
    TPanel *pnRsltColor3;
    TPanel *pnRsltColor4;
    TPanel *pnRsltColor5;
    TPanel *pnRsltColor6;
    TPanel *pnRsltColor7;
    TPanel *pnRsltColor8;
    TPanel *pnRsltColor9;
    TPanel *pnRsltColorA;
    TPanel *pnRsltColorB;
    TPanel *pnRsltColor0;
    TColorDialog *cdSelColor;
    TCheckBox *cbUseFailBinIgnore;
        TEdit *Edit1;
        TPanel *pnRsltColorC;
        TCheckBox *cbNotUseTool1;
        TCheckBox *cbNotUseTool2;
        TCheckBox *cbNotUseTool3;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall cbLoadingStopClick(TObject *Sender);
    void __fastcall pnRsltColor1Click(TObject *Sender);
    void __fastcall cbUseBinFullIgnoreClick(TObject *Sender);
    void __fastcall cbUseFailBinIgnoreClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif
