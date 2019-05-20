//---------------------------------------------------------------------------

#ifndef FormInformH
#define FormInformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmInform : public TForm
{
__published:	// IDE-managed Components
    TPageControl *pcExplain;
    TTabSheet *tsAllMap;
    TImage *imWide;
    TMemo *mmExplain;
    TTabSheet *tsMask;
    TTabSheet *tsEmpty;
    TTabSheet *tsMatch;
    TImage *Image2;
    TImage *imHalf1;
    TImage *Image3;
    TImage *imHalf2;
    TImage *Image4;
    TImage *Image1;
    TTabSheet *tsCntOption;
    TImage *Image5;
private:	// User declarations
public:		// User declarations
        __fastcall TFrmInform(TComponent* Owner);

     void Show(int _iTag);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInform *FrmInform;
//---------------------------------------------------------------------------
#endif
