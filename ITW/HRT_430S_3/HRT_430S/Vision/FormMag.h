//---------------------------------------------------------------------------

#ifndef FormMagH
#define FormMagH
//---------------------------------------------------------------------------
#include "CSPIN.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMag : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdate;
    TImage *imCursor;
    TImage *imMagnifying;
    TCSpinEdit *edScale;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall edScaleChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations

public:		// User declarations
        __fastcall TFrmMag(TComponent* Owner);

        void __fastcall UpdateView();
        void __fastcall SetWindowSize();

        Graphics::TBitmap  * m_pBmp ;
        int                  m_iScale;
        int                  m_iPreX, m_iPreY;
        int                  m_iCrntX, m_iCrntY;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMag *FrmMag;
//---------------------------------------------------------------------------
#endif
