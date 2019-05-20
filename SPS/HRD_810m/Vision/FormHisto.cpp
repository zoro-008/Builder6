//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormHisto.h"
#include "SVInterface.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TFrmHisto *FrmHisto = NULL;
__fastcall TFrmHisto::TFrmHisto(TComponent* Owner , IImage * _pImage , TRectTracker * _pTracker)
    : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

    PView->Align = alClient;
    PView->DoubleBuffered = true;

    m_pImage   = _pImage   ;
    m_pTracker = _pTracker ;
}

void __fastcall TFrmHisto::OnSetImage(IImage * _pImage)
{
    m_pImage   = _pImage   ;
}

void __fastcall TFrmHisto::OnSetTracker(TRectTracker * _pTracker)
{
    m_pTracker = _pTracker ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::FormCreate(TObject *Sender)
{
    OriginalProc = PView->WindowProc;
    PView->WindowProc = PanelProc;
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::FormDestroy(TObject *Sender)
{
    PView->WindowProc = OriginalProc;

    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT) UpdateView();
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::FormShow(TObject *Sender)
{
    x = 40;
    y = 40;
    w = PView->Width ;
    h = PView->Height;

    PLoThBar.x = x    ;
    PLoThBar.y = y    ;
    PHiThBar.x = x+256;
    PHiThBar.y = y    ;
    PUpDaBar.x = 0 ;
    PUpDaBar.y = 0 ;

    IGdi * Gdi = GDI_New(PView->Handle);

    Gdi->m_tBrush.Color = clBlack;
    Gdi->Rect(true, x+1, y, w, h);

    GDI_Del(Gdi);

    UpdateView();
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::UpdateView()
{
    if(!Visible) return ;

    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;

    hDc       = GetDC(PView->Handle);
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, PView->Width  , PView->Height);
    hOldMemBm = SelectObject(hMemDc, hMemBm);

    IGdi * Gdi ;
    Gdi = GDI_New(hMemDc);

    //Gdi이용 해서 그릴것들...
    Gdi -> m_tBrush.Color = clBlack;
    //Gdi -> Rect(true, PUpDaBar.x+10, PUpDaBar.y+10, PUpDaBar.x+40,PUpDaBar.y+22);
    Gdi -> Rect(true , 0 , 0 , Width ,Height);
    Gdi -> Line(PUpDaBar.x, PUpDaBar.y, PUpDaBar.x, h-PUpDaBar.y);

    Gdi->m_tPen.Color = clSilver;
    Gdi->Line(x    , h-y, w-x  , h-y   );   // X Bar
    Gdi->Line(x+ 64, h-y, x+ 64, h-y+10);   // X Bar Scale
    Gdi->Line(x+128, h-y, x+128, h-y+10);   // X Bar Scale
    Gdi->Line(x+192, h-y, x+192, h-y+10);   // X Bar Scale
    Gdi->Line(x+255, h-y, x+255, h-y+10);   // X Bar Scale

    Gdi->m_tText.Color = clWhite;
    Gdi->Text(x    -5, h-y+10+5, "0"  );
    Gdi->Text(x+ 64-5, h-y+10+5, "64" );
    Gdi->Text(x+128-5, h-y+10+5, "128");
    Gdi->Text(x+192-5, h-y+10+5, "192");
    Gdi->Text(x+255-5, h-y+10+5, "255");

    TRect rect = m_pTracker -> GetTRect() ;

    //m_pImage -> SetRect(m_iImage , &rect) ; //Image -> SetRect(&rect) ;

    int Histogram[256] ;
    int Max = 1;
    int MaxId = 0 ;
    AnsiString sText ;

    memset(Histogram , 0 , sizeof(Histogram));

    int iRectWidth  = rect.right  - rect.left ;
    int iRectHeight = rect.bottom - rect.top  ;
    for(register int x = rect.left ; x < rect.right ; x++)
        for(register int y = rect.top ; y < rect.bottom ; y++)
           Histogram[m_pImage->GetPixel(x,y)]++;

    for (int i=0; i<256; i++) {
        if(Histogram[i] > Max ) {
            Max = Histogram[i] ;
            MaxId = i ;
        }
    }

    Gdi->m_tPen.Color = clBlue;
    for (int i=0; i<256; i++) {
        int v = Histogram[i] / (double)Max * (h - y - 40);
        Gdi->Line(x+i, h-y-v, x+i, h-y);
        if(i == MaxId) {
            sText  = MaxId ;
            Gdi->Text( x+i, h-y-v-25, sText.c_str() );
            sText  = AnsiString(Max) + "ea " ;
            sText += Max*100/(iRectWidth*iRectHeight) ;
            sText += "%" ;
            Gdi->Text( x+i, h-y-v-15, sText.c_str() );
        }
    }

    Gdi->m_tPen .Color = clLime;
    Gdi->m_tText.Color = clLime;
    Gdi->Text(PLoThBar.x+10, PLoThBar.y+10, AnsiString(PLoThBar.x-x).c_str());
    Gdi->Line(PLoThBar.x, PLoThBar.y, PLoThBar.x, h-PLoThBar.y);   // Y Bar

    Gdi->m_tPen .Color = clRed;
    Gdi->m_tText.Color = clRed;
    Gdi->Text(PHiThBar.x+10, PHiThBar.y+10, AnsiString(PHiThBar.x-x).c_str());
    Gdi->Line(PHiThBar.x, PHiThBar.y, PHiThBar.x, h-PHiThBar.y);

    //  Copy buffer bitmap into window dc.
    BitBlt(hDc, 0, 0, PView->Width, PView->Height, hMemDc, 0, 0, SRCCOPY);

    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(PView->Handle,hDc);
    GDI_Del(Gdi);
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::PViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button==mbLeft && X > x-1 && X < x+256) {
        if (X > PLoThBar.x-4 && X < PLoThBar.x+4 && Y > PLoThBar.y && Y < h-PLoThBar.y) IsLoThBar = true;
        if (X > PHiThBar.x-4 && X < PHiThBar.x+4 && Y > PHiThBar.y && Y < h-PHiThBar.y) IsHiThBar = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::PViewMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if (Shift.Contains(ssLeft)) {
        if (IsLoThBar && X > x-1 && X < x+256 && X < PHiThBar.x) {
            PUpDaBar   = PLoThBar;
            PLoThBar.x = X;
            UpdateView();
//            OwnerForm->IsBinary = true;
//            OwnerForm->UpdateView(true, PHiThBar.x-x, PLoThBar.x-x, 255, 0, false);
//            CameraTable.Camera[OwnerForm->Vision.iCamId].OnPaintBitmap(OwnerForm->PSelView, PHiThBar.x-x, PLoThBar.x-x, 255, 0, Scale);
            return;
        }
        if (IsHiThBar && X > x-1 && X < x+256 && X > PLoThBar.x) {
            PUpDaBar   = PHiThBar;
            PHiThBar.x = X;
            UpdateView();
//            OwnerForm->IsBinary = true;
//            OwnerForm->UpdateView(true, PHiThBar.x-x, PLoThBar.x-x, 255, 0, false);
//            CameraTable.Camera[OwnerForm->Vision.iCamId].OnPaintBitmap(OwnerForm->PSelView, PHiThBar.x-x, PLoThBar.x-x, 255, 0, Scale);
            return;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmHisto::PViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button==mbLeft) {
        UpdateView();
        IsLoThBar = false;
        IsHiThBar = false;
    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmHisto::FormActivate(TObject *Sender)
{
    UpdateView();
}
//---------------------------------------------------------------------------

void __fastcall TFrmHisto::tmUpdateTimer(TObject *Sender)
{
    if(!Visible    ) return ;
    if( Focused()  ) return ;
    if(!m_pTracker ) return ;

    static int iPreLeft   = 0 ;
    static int iPreTop    = 0 ;
    static int iPreWidth  = 0 ;
    static int iPreHeight = 0 ;

    if( iPreLeft   == m_pTracker -> Left   &&
        iPreTop    == m_pTracker -> Top    &&
        iPreWidth  == m_pTracker -> Width  &&
        iPreHeight == m_pTracker -> Height ){
        return ;
    }

    iPreLeft   = m_pTracker -> Left   ;
    iPreTop    = m_pTracker -> Top    ;
    iPreWidth  = m_pTracker -> Width  ;
    iPreHeight = m_pTracker -> Height ;


    tmUpdate -> Enabled = false ;
    UpdateView();
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHisto::FormClose(TObject *Sender, TCloseAction &Action)
{
    //Action = caFree;
}
//---------------------------------------------------------------------------

