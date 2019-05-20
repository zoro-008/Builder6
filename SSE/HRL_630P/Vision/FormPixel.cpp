//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormPixel.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define OFFSET 40
#define ONE_BYTE 256

//---------------------------------------------------------------------------
TFrmPixel *FrmPixel = NULL;
__fastcall TFrmPixel::TFrmPixel(TComponent* Owner , IImage * _pImage , TLineTracker * _pTracker)
    : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

    PPxView->Align = alClient;
    m_pImage   = _pImage ;
    m_pTracker = _pTracker ;

    UpdateView() ;
}

void __fastcall TFrmPixel::OnSetImage(IImage * _pImage)
{
    m_pImage = _pImage ;
}

void __fastcall TFrmPixel::OnSetTracker(TLineTracker * _pTracker)
{
    m_pTracker = _pTracker ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::FormCreate(TObject *Sender)
{
    OriginalProc = PPxView->WindowProc;
    PPxView->WindowProc = PanelProc;

    PPxView -> DoubleBuffered = true ;

    m_tSelPnt1.x = 80 ;
    m_tSelPnt1.y = 80 ;
    m_tSelPnt2.x = 90 ;
    m_tSelPnt2.y = 90 ;

    IsSelVer1 = false ;
    IsSelHor1 = false ;
    IsSelVer2 = false ;
    IsSelHor2 = false ;

    m_pLastSel = &m_tSelPnt1 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPixel::FormDestroy(TObject *Sender)
{
    PPxView->WindowProc = OriginalProc;
    static int i = 0 ;
    //Trace("",AnsiString(i).c_str());
    i++;


}
//---------------------------------------------------------------------------
void __fastcall TFrmPixel::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT) UpdateView();
}

//---------------------------------------------------------------------------

void __fastcall TFrmPixel::UpdateView()
{
    if( PPxView -> Width  <= OFFSET) return ;
    if( PPxView -> Height <= OFFSET) return ;
    if(!m_pTracker                 ) return ;
    if(!Visible                    ) return ;          

    const int AvrRange = abs(edAvrRange -> Text.ToIntDef(1)) ;

    //canvasũ��.
    rect.left   = OFFSET ;
    rect.top    = OFFSET ;
    rect.right  = PPxView->Width  - OFFSET ;
    rect.bottom = PPxView->Height - OFFSET ;

    //sp.x = m_pTracker -> StartX + *m_pRefOstX; sp.y = m_pTracker -> StartY + *m_pRefOstY;
    //ep.x = m_pTracker -> EndX   + *m_pRefOstX; ep.y = m_pTracker -> EndY   + *m_pRefOstY;
    sp.x = m_pTracker -> StartX ; sp.y = m_pTracker -> StartY ;
    ep.x = m_pTracker -> EndX   ; ep.y = m_pTracker -> EndY   ;

    //���߿� ������� ���� ���ؼ� ���� �ϰ� SetPoint ��������.
    m_pImage->SetPoint(&sp);
    m_pImage->SetPoint(&ep);

                       //X���� ũ�� ��.
    const bool IsPat = abs(ep.x - sp.x) >= abs(ep.y - sp.y) ;

    TPoint Temp ;
    if( IsPat) { //�����ѳ�.
        if(ep.x < sp.x) { //�׷����� �������� ������ �������� �ٲ��ش�.
            Temp = ep ;
            ep = sp ;
            sp = Temp ;
        }
    }
    else {
        if(ep.y < sp.y) { //�׷����� ������ �������� �ٲ۴�.
            Temp = ep ;
            ep = sp ;
            sp = Temp ;
        }
    }

    //������ ���� ���� X
    //������ ���� ���� Y
    const int DataCnt = IsPat ? (ep.x - sp.x) : (ep.y - sp.y) ;
    const int sx = IsPat ? sp.x : sp.y ;
    const int ex = IsPat ? ep.x : ep.y ;
    const int sy = IsPat ? sp.y : sp.x ;
    const int ey = IsPat ? ep.y : ep.x ;

    const bool ShowRow = cbRow -> Checked;
    const bool ShowAvr = cbAvr -> Checked;
    const bool ShowDf1 = cbDf1 -> Checked;
    const bool ShowDf2 = cbDf2 -> Checked;
    const bool ShowVer = cbVerScale -> Checked;
    const bool ShowHor = cbHorScale -> Checked;

    int SumCnt ;
    int Sum    ;
    AnsiString Text ;

    //Set Fomula of Line.
    //==========================================================================
    double a ;
    double b ;
    if(ex-sx) a = (ey - sy)/(double)(ex - sx) ;
    else      a = (ey - sy)/0.0000000000001   ;
    b = sy - a * sx ;


    //Graph Value.
    //==========================================================================
    int *PixVal = new int[DataCnt];
    int *AvrVal = new int[DataCnt];
    int *Df1Val = new int[DataCnt];
    int *Df2Val = new int[DataCnt];

    int MaxPix = 0   ;
    int MinPix = 256 ;
    int MidPix = 256 /2 ;

    int y ;
    for (register int i = sx ; i < ex ; i++) {
       y = a * i + b ;
       if(IsPat) {
           PixVal[i - sx] = m_pImage->GetPixel(i , y) ;
           if(MaxPix < m_pImage->GetPixel(i , y)) MaxPix = m_pImage->GetPixel(i , y) ;
           if(MinPix > m_pImage->GetPixel(i , y)) MinPix = m_pImage->GetPixel(i , y) ;
       }
       else {
           PixVal[i - sx] = m_pImage->GetPixel(y , i) ;
           if(MaxPix < m_pImage->GetPixel(y , i)) MaxPix = m_pImage->GetPixel(y , i) ;
           if(MinPix > m_pImage->GetPixel(y , i)) MinPix = m_pImage->GetPixel(y , i) ;
       }


    }
    MidPix = (MaxPix + MinPix) / 2 ;

    for (register int i = 0 ; i < DataCnt ; i++) {
       SumCnt = 0 ;
       Sum    = 0 ;
       for (register int j = -AvrRange ; j < AvrRange ; j ++ ) {
          if( (i + j) < 0 ) continue ;
          Sum += PixVal[i+j] ;
          SumCnt++ ;
       }
       AvrVal[i] = SumCnt ? Sum/SumCnt : PixVal[i] ;
       Df1Val[i] = (i - 1) > 0 ? AvrVal[i] - AvrVal[i-1] + MidPix : MidPix ;
       Df2Val[i] = (i - 1) > 0 ? Df1Val[i] - Df1Val[i-1] + MidPix : MidPix ;
    }







    //Display.
    //==========================================================================
    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;

    hDc       = GetDC(PPxView->Handle);
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, PPxView->Width , PPxView->Height);
    hOldMemBm = SelectObject(hMemDc, hMemBm);

    IGdi * Gdi = GDI_New(hMemDc);
    //Gdi�̿� �ؼ� �׸��͵�...
    //Erase.
    Gdi->m_tBrush.Color = PPxView -> Color ;
    Gdi->m_tPen  .Color = PPxView -> Color ;
    Gdi->Rect(true , 0 , 0  , PPxView -> Width , PPxView -> Height);

    //rect
    Gdi->m_tBrush.Color = (TColor)0x00383838 ;
    Gdi->m_tPen  .Color = clSilver ;
    Gdi->Rect(true, rect.left , rect.top , rect.right , rect.bottom + 1);

    //scale.
    register int j ;
    Gdi->m_tText.Color = clYellow;
    for(register int i = rect.bottom ; i > rect.top ; i -= (Gdi -> m_tFont.Height + 5)) {
       j =  ONE_BYTE * (rect.bottom-i) /(rect.bottom - rect.top);
       Gdi->m_tPen.Color = (TColor)clSilver ;
       Gdi->Line(rect.left-5  , i ,rect.left , i );
       Gdi->m_tPen.Color = (TColor)0x00525252;
       if(ShowVer)Gdi->Line(rect.left , i , rect.right , i );
       Gdi->Text(rect.left-25 , (i - Gdi -> m_tFont.Height / 2) ,  AnsiString(j).c_str());
    }

    Gdi -> m_tFont.Escapement = 90 ;
    for(register int i = rect.left ; i < rect.right ; i += Gdi -> m_tFont.Height + 5) {
       j = DataCnt * (i - rect.left) /(rect.right - rect.left) + sx ;
       Gdi->m_tPen.Color = (TColor)clSilver ;
       Gdi->Line(i , rect.bottom , i , rect.bottom + 17 );
       Gdi->m_tPen.Color = (TColor)0x00525252;
       if(ShowHor)Gdi->Line(i , rect.top , i , rect.bottom);
       Gdi->Text((i - Gdi -> m_tFont.Height / 2) , rect.bottom + 37 ,  AnsiString(j).c_str());
    }
    Gdi -> m_tFont.Escapement = 0 ;

    Gdi -> m_tPen.Color = clWhite ;
    Gdi -> Line(rect.left , rect.top , rect.left , rect.bottom ); // Y Bar
    Gdi -> Line(rect.left , rect.bottom , rect.right , rect.bottom ); // X Bar



    bool       IsEdge ;
    AnsiString Grey   ;
    int        DeGrey ;

    double RatioX ;
    double RatioY ;
    for (register int k = 1 ; k < DataCnt ; k++) {

        RatioX= (rect.right - rect.left)/(double)DataCnt  ;
        RatioY= (rect.bottom - rect.top)/(double)ONE_BYTE ;

        Gdi->m_tPen.Color = clBlue ;
        if(ShowRow)Gdi->Line(rect.left+k*RatioX, rect.bottom - PixVal[k]*RatioY , rect.left+(k-1)*RatioX, rect.bottom - PixVal[k-1]*RatioY );
        Gdi->m_tPen.Color = clRed ;
        if(ShowAvr)Gdi->Line(rect.left+k*RatioX, rect.bottom - AvrVal[k]*RatioY , rect.left+(k-1)*RatioX, rect.bottom - AvrVal[k-1]*RatioY );
        Gdi->m_tPen.Color = clFuchsia ;
        if(ShowDf1)Gdi->Line(rect.left+k*RatioX, rect.bottom - Df1Val[k]*RatioY , rect.left+(k-1)*RatioX, rect.bottom - Df1Val[k-1]*RatioY );
        Gdi->m_tPen.Color = clGreen ;
        if(ShowDf2)Gdi->Line(rect.left+k*RatioX, rect.bottom - Df2Val[k]*RatioY , rect.left+(k-1)*RatioX, rect.bottom - Df2Val[k-1]*RatioY );

        Grey   = "0x" + IntToHex(PixVal[k],2) + IntToHex(PixVal[k],2) + IntToHex(PixVal[k],2);
        DeGrey = StrToIntDef(Grey, 0) ;

        Gdi->m_tPen.Color = (TColor)DeGrey ;
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 1 , rect.left+(k-1)*RatioX , rect.bottom + 1);
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 2 , rect.left+(k-1)*RatioX , rect.bottom + 2);
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 3 , rect.left+(k-1)*RatioX , rect.bottom + 3);
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 4 , rect.left+(k-1)*RatioX , rect.bottom + 4);
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 5 , rect.left+(k-1)*RatioX , rect.bottom + 5);

        Gdi->m_tPen.Color = PixVal[k] > ONE_BYTE - (int)(ONE_BYTE/(double)rect.Height()*(m_pLastSel->y - OFFSET)) ? clWhite : clBlack ;
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 6 , rect.left+(k-1)*RatioX , rect.bottom + 6 );
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 7 , rect.left+(k-1)*RatioX , rect.bottom + 7 );
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 8 , rect.left+(k-1)*RatioX , rect.bottom + 8 );
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 9 , rect.left+(k-1)*RatioX , rect.bottom + 9 );
        Gdi->Line(rect.left+k*RatioX , rect.bottom + 10, rect.left+(k-1)*RatioX , rect.bottom + 10);

        //Dif1Edge
        if((k - 1) < 0 || (k + 1) > DataCnt) continue ;
        Gdi->m_tPen.Color = clFuchsia ;
        IsEdge = (Df1Val[k+1] < Df1Val[k] && Df1Val[k-1] < Df1Val[k]) || //(Df1Val[k+1] < Df1Val[k] && Df1Val[k-1] <= Df1Val[k]) ||
                 (Df1Val[k+1] > Df1Val[k] && Df1Val[k-1] > Df1Val[k]) ;//|| (Df1Val[k+1] > Df1Val[k] && Df1Val[k-1] >= Df1Val[k]) ;
        if(IsEdge && ShowDf1) {
           Gdi->m_tPen.Color = clRed ;
           Gdi->Rect(true , rect.left+(int)(k*RatioX) - 2, rect.bottom - (int)(Df1Val[k]*RatioY) - 2 , rect.left+(int)(k*RatioX) + 2 , rect.bottom - (int)(Df1Val[k]*RatioY) + 2 );
        }

        //Dif2Edge
        Gdi->m_tPen.Color = clFuchsia ;
        IsEdge = (Df2Val[k+1] < Df2Val[k] && Df2Val[k-1] < Df2Val[k]) || //(Df2Val[k+1] < Df2Val[k] && Df2Val[k-1] <= Df2Val[k]) ||
                 (Df2Val[k+1] > Df2Val[k] && Df2Val[k-1] > Df2Val[k]) ;//|| (Df2Val[k+1] > Df2Val[k] && Df2Val[k-1] >= Df2Val[k]) ;
        if(IsEdge && ShowDf2) {
           Gdi->m_tPen.Color = clBlue ;
           Gdi->Rect(true , rect.left+(int)(k*RatioX) - 2, rect.bottom - (int)(Df2Val[k]*RatioY) - 2 , rect.left+(int)(k*RatioX) + 2 , rect.bottom - (int)(Df2Val[k]*RatioY) + 2 );
        }
    }

    //SelPoint1
    Gdi->m_tPen.Color = clLime;
    Gdi->Line(rect.left , m_tSelPnt1.y , rect.right , m_tSelPnt1.y ); // Hor Bar
    Gdi->Line(m_tSelPnt1.x , rect.top , m_tSelPnt1.x , rect.bottom ); // Ver Bar
    Gdi->Text (rect.right-20 , m_tSelPnt1.y - Gdi -> m_tFont.Height - 2 , AnsiString(ONE_BYTE - (int)(ONE_BYTE/(double)rect.Height()*(m_tSelPnt1.y - OFFSET))).c_str());
    Gdi->m_tFont.Escapement = 90 ;
    Gdi->Text (  m_tSelPnt1.x - Gdi -> m_tFont.Height - 2 , rect.top + 20  , AnsiString ( DataCnt * (m_tSelPnt1.x - rect.left) /(rect.right - rect.left) + sx).c_str());
    Gdi->m_tFont.Escapement = 0  ;

    //SelPoint2
    Gdi->m_tPen.Color = clYellow;
    Gdi->Line(rect.left , m_tSelPnt2.y , rect.right , m_tSelPnt2.y ); // Hor Bar
    Gdi->Line(m_tSelPnt2.x , rect.top , m_tSelPnt2.x , rect.bottom ); // Ver Bar
    Gdi->Text (rect.right-20 , m_tSelPnt2.y - Gdi -> m_tFont.Height - 2 , AnsiString(ONE_BYTE - (int)(ONE_BYTE/(double)rect.Height()*(m_tSelPnt2.y - OFFSET))).c_str());
    Gdi->m_tFont.Escapement = 90 ;
    Gdi->Text (  m_tSelPnt2.x - Gdi -> m_tFont.Height - 2 , rect.top + 20  , AnsiString ( DataCnt * (m_tSelPnt2.x - rect.left) /(rect.right - rect.left) + sx).c_str());
    Gdi->m_tFont.Escapement = 0  ;

    Gdi->m_tPen.Color = clBlue;
    Gdi->Line(m_pLastSel->x-1 , rect.top , m_pLastSel->x-1 , rect.bottom );
    Gdi->Line(m_pLastSel->x+1 , rect.top , m_pLastSel->x+1 , rect.bottom );
    Gdi->Line(rect.left , m_pLastSel->y-1 , rect.right , m_pLastSel->y-1 );
    Gdi->Line(rect.left , m_pLastSel->y+1 , rect.right , m_pLastSel->y+1 );

    //y Diffrence.
    Gdi->m_tBrush.Color = (TColor)0x00363636 ;
    Gdi->m_tPen  .Color = clSilver ;
    Gdi->Line(rect.right   , m_tSelPnt1.y , rect.right+5 , m_tSelPnt1.y );
    Gdi->Line(rect.right   , m_tSelPnt2.y , rect.right+5 , m_tSelPnt2.y );
    Gdi->Line(rect.right+5 , m_tSelPnt1.y , rect.right+5 , m_tSelPnt2.y );
    Gdi->Rect(true, rect.right+7 , (m_tSelPnt1.y + m_tSelPnt2.y)/2 - Gdi -> m_tFont.Height , rect.right + 30 , (m_tSelPnt1.y + m_tSelPnt2.y)/2 + Gdi -> m_tFont.Height );
    Gdi->Text(rect.right+9 , (m_tSelPnt1.y + m_tSelPnt2.y)/2 - Gdi -> m_tFont.Height / 2 , AnsiString((int)(ONE_BYTE/(double)rect.Height()*abs(m_tSelPnt1.y - m_tSelPnt2.y))).c_str());

    //y Diffrence.
    Gdi -> m_tFont.Escapement = 90 ;
    Gdi->m_tBrush.Color = (TColor)0x00363636 ;
    Gdi->m_tPen  .Color = clSilver ;
    Gdi->Line(m_tSelPnt1.x , rect.top     , m_tSelPnt1.x , rect.top - 5 );
    Gdi->Line(m_tSelPnt2.x , rect.top     , m_tSelPnt2.x , rect.top - 5 );
    Gdi->Line(m_tSelPnt2.x , rect.top - 5 , m_tSelPnt1.x , rect.top - 5 );

    Gdi->Rect(true, (m_tSelPnt1.x + m_tSelPnt2.x)/2 - Gdi -> m_tFont.Height , rect.top-7 , (m_tSelPnt1.x + m_tSelPnt2.x)/2 + Gdi -> m_tFont.Height,rect.top - 30 );
    Gdi->Text((m_tSelPnt1.x + m_tSelPnt2.x)/2 - Gdi -> m_tFont.Height/2 , rect.top-9, AnsiString((int)(ONE_BYTE/(double)rect.Height()*abs(m_tSelPnt1.x - m_tSelPnt2.x))).c_str());
    Gdi -> m_tFont.Escapement = 0 ;

    delete [] PixVal;
    delete [] AvrVal;
    delete [] Df1Val;
    delete [] Df2Val;

    GDI_Del(Gdi);

    //  Copy buffer bitmap into window dc.
    BitBlt(hDc, 0, 0, PPxView->Width, PPxView->Height, hMemDc, 0, 0, SRCCOPY);

    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(PPxView->Handle,hDc);
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::PPxViewMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (X > rect.right || X < rect.left || Y > rect.bottom || Y < rect.top) return ;

   if (Shift.Contains(ssLeft)) {
      if(IsSelHor1) {m_tSelPnt1.y = Y ; UpdateView(); }
      if(IsSelVer1) {m_tSelPnt1.x = X ; UpdateView(); }

      if(IsSelHor2) {m_tSelPnt2.y = Y ; UpdateView(); }
      if(IsSelVer2) {m_tSelPnt2.x = X ; UpdateView(); }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::PPxViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

    if (Button==mbLeft) {
        IsSelVer1 = false ;
        IsSelHor1 = false ;
        IsSelVer2 = false ;
        IsSelHor2 = false ;
        UpdateView();
    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::PPxViewResize(TObject *Sender)
{
    UpdateView();
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::cbRowClick(TObject *Sender)
{
   UpdateView() ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::edAvrRangeChange(TObject *Sender)
{
   UpdateView() ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmPixel::PPxViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Button==mbLeft) {
      if(Y < m_tSelPnt1.y+3 && Y > m_tSelPnt1.y-3 && X < rect.right && X >rect.left) {m_pLastSel = &m_tSelPnt1 ; IsSelHor1 = true ;}
      if(X < m_tSelPnt1.x+3 && X > m_tSelPnt1.x-3 && Y < rect.bottom && Y >rect.top) {m_pLastSel = &m_tSelPnt1 ; IsSelVer1 = true ;}
      if(IsSelHor1 || IsSelVer1)  return  ;

      if(Y < m_tSelPnt2.y+3 && Y > m_tSelPnt2.y-3 && X < rect.right && X >rect.left) {m_pLastSel = &m_tSelPnt2 ; IsSelHor2 = true ;}
      if(X < m_tSelPnt2.x+3 && X > m_tSelPnt2.x-3 && Y < rect.bottom && Y >rect.top) {m_pLastSel = &m_tSelPnt2 ; IsSelVer2 = true ;}
   }
   UpdateView();
}


void __fastcall TFrmPixel::FormActivate(TObject *Sender)
{
    UpdateView();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPixel::tmUpdateTimer(TObject *Sender)
{
    if(!Visible  ) return ;
    if( Focused()) return ;

    tmUpdate -> Enabled = false ;

    /* ���̺�� ��ȭ ��� �ּ�.
    static int iPreStartX = 0 ;
    static int iPreStartY = 0 ;
    static int iPreEndX   = 0 ;
    static int iPreEndY   = 0 ;

    if( iPreStartX == m_pTracker -> StartX &&
        iPreStartY == m_pTracker -> StartY &&
        iPreEndX   == m_pTracker -> EndX   &&
        iPreEndY   == m_pTracker -> EndY   ){
        tmUpdate -> Enabled = true ;
        return ;
    }

    iPreStartX = m_pTracker -> StartX ;
    iPreStartY = m_pTracker -> StartY ;
    iPreEndX   = m_pTracker -> EndX   ;
    iPreEndY   = m_pTracker -> EndY   ;
    */

    UpdateView();
    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPixel::FormClose(TObject *Sender, TCloseAction &Action)
{
    //Action = caFree;       
}
//---------------------------------------------------------------------------
