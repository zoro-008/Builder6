//---------------------------------------------------------------------------

#ifndef FormPixelH
#define FormPixelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Tracker.h"
#include "UnitImage.h"

//---------------------------------------------------------------------------
class TFrmPixel : public TForm
{
__published:	// IDE-managed Components
    TPanel *PPxView;
        TPanel *Panel1;
        TEdit *edAvrRange;
        TLabel *Label1;
        TCheckBox *cbRow;
        TCheckBox *cbAvr;
        TCheckBox *cbDf1;
        TCheckBox *cbDf2;
        TCheckBox *cbHorScale;
        TCheckBox *cbVerScale;
    TTimer *tmUpdate;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall PPxViewMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PPxViewMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PPxViewResize(TObject *Sender);
        void __fastcall cbRowClick(TObject *Sender);
        void __fastcall edAvrRangeChange(TObject *Sender);
        void __fastcall PPxViewMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    CImage       * m_pImage   ;
    TLineTracker * m_pTracker ;

    TPoint * m_pLastSel ;
    TPoint   m_tSelPnt1 ;
    TPoint   m_tSelPnt2 ;

    TRect    rect     ;
    TPoint   sp , ep  ;
    bool IsSelVer1 ;
    bool IsSelHor1 ;
    bool IsSelVer2 ;
    bool IsSelHor2 ;


    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

private :
    void __fastcall MyFunct( TWMEraseBkgnd &msg ) { NULL; };

public:		// User declarations
    __fastcall TFrmPixel(TComponent* Owner , CImage * _pImage , TLineTracker * _pTracker);
    void __fastcall OnSetImage  (CImage * _pImage ) ;
    void __fastcall OnSetTracker(TLineTracker * _pTracker) ;

    void __fastcall UpdateView();

BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER( WM_ERASEBKGND, TWMEraseBkgnd, MyFunct );
END_MESSAGE_MAP( TForm );
};
extern PACKAGE TFrmPixel *FrmPixel;
//---------------------------------------------------------------------------
#endif


/*
RECT crt;
 HDC hdc, hMemDC, hMachineDC;
 HBITMAP OldBit, OldBit2;

 hdc=GetDC(hWnd);
 GetClientRect(hWnd, &crt);

 if(hBitBuf==NULL)
 {
  hBitBuf=CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
 }
 hMemDC=CreateCompatibleDC(hdc);
 OldBit=(HBITMAP)SelectObject(hMemDC, hBitBuf);

 hMachineDC=CreateCompatibleDC(hdc);
 OldBit2=(HBITMAP)SelectObject(hMachineDC, hBitMachine);

 moveMachine(hWnd);

 TransparentBlt(hMemDC, Machine.Get("x"), Machine.Get("y"), 25, 29, hMachineDC, 0, 0, 25, 29, RGB(64, 192, 192));

 SelectObject(hMemDC, OldBit);
 SelectObject(hMachineDC, OldBit2);
 DeleteDC(hMemDC);
 DeleteDC(hMachineDC);
 ReleaseDC(hWnd, hdc);
 InvalidateRect(hWnd, NULL, FALSE);
  CI1->Canvas->Lock();
  CI1->Canvas->Unlock();


  TCanvas *Canvas=new TCanvas(Panel1); 
   Canvas->Handle = GetDC(Palen1->Handle); 

   Canvas->Draw....




*/
//    TPanel * pnMem = new TPanel(fmPixel) ;

//     SGdi *Gdi = new SGdi(pnMem->Handle);




//    TCanvas *Canvas=new TCanvas(PPxView);
//    Canvas->Handle = GetDC(PPxView->Handle);


