//---------------------------------------------------------------------------

#ifndef FormHistoH
#define FormHistoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "Tracker.h"
#include "UnitImage.h"

//---------------------------------------------------------------------------
class TFrmHisto : public TForm
{
__published:	// IDE-managed Components
    TPanel *PView;
    TTimer *tmUpdate;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall PViewMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PViewMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    int x, y, w, h;

    TPoint PLoThBar ;
    TPoint PHiThBar ;
    TPoint PUpDaBar ;

    bool  IsLoThBar ;
    bool  IsHiThBar ;

    CImage       * m_pImage   ;
    TRectTracker * m_pTracker ;

    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

public:		// User declarations
    void __fastcall UpdateView();

    __fastcall TFrmHisto(TComponent* Owner , CImage * _pImage , TRectTracker * _pTracker);
    void __fastcall OnSetImage  (CImage * _pImage) ;
    void __fastcall OnSetTracker(TRectTracker * _pTracker) ;
};
extern PACKAGE TFrmHisto *FrmHisto;
//---------------------------------------------------------------------------
#endif

