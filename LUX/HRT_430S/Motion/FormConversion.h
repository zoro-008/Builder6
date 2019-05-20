//---------------------------------------------------------------------------

#ifndef FormConversionH
#define FormConversionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFrmConversion : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TShape *spTop;
    TShape *spMask;
    TShape *spBoard;
    TShape *spTopTrim;
    TShape *spBtmTrim;
    TTimer *tmFlick;
    TLabel *lbWarning;
    TShape *Shape1;
    TImage *Image2;
    TImage *Image3;
    TShape *Shape2;
    TImage *Image4;
    TShape *Shape3;
    TImage *Image5;
    TShape *Shape4;
    TImage *Image6;
    TShape *Shape5;
    TImage *imssss;
    TShape *spTop2;
    TShape *spMask2;
    TShape *spBoard2;
    TShape *spBtmTrim2;
    TShape *spTopTrim2;
    TButton *btOk;
    void __fastcall tmFlickTimer(TObject *Sender);
    void __fastcall btOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmConversion(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmConversion *FrmConversion;
//---------------------------------------------------------------------------
#endif
