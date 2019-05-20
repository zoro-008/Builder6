 //---------------------------------------------------------------------------
#include <math.h>

#pragma hdrstop

#include "UnitGdi.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
#define ORGINAL_FONT_SIZE 12

/*
void __stdcall CGdi::Init()
{
    m_tPen.Style       = PS_SOLID;
    m_tPen.EndStyle    = PS_ENDCAP_FLAT;
    m_tPen.JoinStyle   = PS_JOIN_MITER;
    m_tPen.Width       = 1;
    m_tPen.Color       = clBlack;

    m_tBrush.Style     = BS_SOLID;
    m_tBrush.Color     = clBlack;
    m_tBrush.Hatch     = HS_HORIZONTAL;

    m_tFont.Height     = ORGINAL_FONT_SIZE;
    m_tFont.Escapement = 0;
    m_tFont.Italic     = 0;
    m_tFont.Underline  = 0;
    m_tFont.StrikeOut  = 0;
    m_tFont.Face       = "";

    m_tText.Color      = clYellow;
    m_tText.BkColor    = clWhite;

    
    

    

}

void __stdcall CGdi::MakeText()
{
    SetBkMode   (m_hDc, TRANSPARENT);
    SetBkColor  (m_hDc, m_tText.BkColor);
    SetTextColor(m_hDc, m_tText.Color);
}

void __stdcall CGdi::MakePen()
{
    m_hNewPen =  CreatePen(m_tPen.Style, m_tPen.Width, m_tPen.Color);
    m_hOldPen = (HPEN)SelectObject(m_hDc, m_hNewPen);
    SetROP2   (m_hDc, R2_COPYPEN);
    SetBkMode (m_hDc, TRANSPARENT);
}

void __stdcall CGdi::MakePenExt()
{
    m_tLogbrush.lbStyle = m_tBrush.Style;
    m_tLogbrush.lbColor = m_tBrush.Color;
    m_tLogbrush.lbHatch = m_tBrush.Hatch;

    m_hNewPen = ExtCreatePen(PS_GEOMETRIC | m_tPen.Style | m_tPen.EndStyle | m_tPen.JoinStyle, m_tPen.Width, &m_tLogbrush, 0, NULL);
    m_hOldPen = (HPEN)SelectObject(m_hDc, m_hNewPen);
    SetROP2   (m_hDc, R2_COPYPEN);
    SetBkMode (m_hDc, TRANSPARENT);
}

void __stdcall CGdi::DelePen()
{
    SelectObject(m_hDc, m_hOldPen);
    DeleteObject(m_hNewPen);
}

void __stdcall CGdi::MakeBrush()
{
    m_tLogbrush.lbStyle = m_tBrush.Style;
    m_tLogbrush.lbColor = m_tBrush.Color;
    m_tLogbrush.lbHatch = m_tBrush.Hatch;

    m_hNewBrush = CreateBrushIndirect(&m_tLogbrush);

    m_hOldBrush = (HBRUSH)SelectObject(m_hDc, m_hNewBrush);
    SetROP2   (m_hDc, R2_COPYPEN);
    SetBkMode (m_hDc, TRANSPARENT);
}

void __stdcall CGdi::DeleBrush()
{
    SelectObject(m_hDc, m_hOldBrush);
    DeleteObject(m_hNewBrush);
}

void __stdcall CGdi::MakeFont()
{
    m_hNewFont = CreateFont(m_tFont.Height, 0, m_tFont.Escapement*10, 0, FW_MEDIUM, m_tFont.Italic, m_tFont.Underline, m_tFont.StrikeOut,
                          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, m_tFont.Face);
    m_hOldFont = (HFONT)SelectObject(m_hDc, m_hNewFont);
}

void __stdcall CGdi::DeleFont()
{
    SelectObject(m_hDc, m_hOldFont);
    DeleteObject(m_hNewFont);
}

void __stdcall CGdi::SetScale (double _fXScale  , double _fYScale ) { m_dXScale  = _fXScale  ; m_dYScale  = _fYScale  ; }
void __stdcall CGdi::SetOffset(double _fXOffset , double _fYOffset) { m_dXOffset = _fXOffset ; m_dYOffset = _fYOffset ; }

//출력 함수군.
//기본적으로 속에서 Make를 한다.
//복합 출력 함수 만들때 Make 안하게 주의..
/////////////////////////////////////////////////////////////////////////////////
void __stdcall CGdi::Text(int x, int y, char *str)
{
    if (m_hDc==NULL) return;

    int OriHeight = m_tFont.Height ;
    //m_tFont.Height *= m_dYScale ;

    MakeFont();
    MakeText();

    
    //    int ix1 = int(3 - pImg->GetOffsetX())*pImg->GetScaleX();
    //    int iy1 = int(3 - pImg->GetOffsetY())*pImg->GetScaleY();
    //
    //    int ix2 = int(5 - pImg->GetOffsetX())*pImg->GetScaleX();
    //    int iy2 = int(5 - pImg->GetOffsetY())*pImg->GetScaleY();


    x += m_dXOffset;
    y += m_dYOffset;

    x*=m_dXScale;
    y*=m_dYScale;

    TextOut(m_hDc, x, y, str, strlen(str));

    m_tFont.Height = OriHeight ;
    DeleFont();
}

//void __stdcall CGdi::Move(int x , int y)
//{
//    if (m_hDc==NULL) return;
//    TPoint p;
//    MoveToEx(m_hDc, x+m_dXOffset, y+m_dYOffset, &p);
//}

void __stdcall CGdi::Line(int x1, int y1, int x2, int y2)
{
    if (m_hDc==NULL) return;

    MakePen();

    x1 += m_dXOffset;
    y1 += m_dYOffset;
    x2 += m_dXOffset;
    y2 += m_dYOffset;

    x1*=m_dXScale;
    y1*=m_dYScale;
    x2*=m_dXScale;
    y2*=m_dYScale;



    TPoint p;
    MoveToEx(m_hDc, x1, y1, &p); //이상하게 라인투로 사각형 그리면 그냥 rectangle 쓰는 것 보다 1픽셀 더감... 나중에 알아보자. 왜그런지.
    LineTo  (m_hDc, x2, y2    );
    DelePen();
}



void __stdcall CGdi::LineExt(int x1, int y1, int x2, int y2)
{
    if (m_hDc==NULL) return;

    MakePenExt();

    x1 += m_dXOffset;
    y1 += m_dYOffset;
    x2 += m_dXOffset;
    y2 += m_dYOffset;

    x1*=m_dXScale;
    y1*=m_dYScale;
    x2*=m_dXScale;
    y2*=m_dYScale;

    TPoint p;
    MoveToEx(m_hDc, x1, y1, &p);
    LineTo  (m_hDc, x2, y2    );
    DelePen();

}


void __stdcall CGdi::Triangle(bool IsFill, int x1, int y1, int x2, int y2, int x3, int y3)
{
    if (m_hDc==NULL) return;

    MakePen();
    MakeBrush();

    x1 += m_dXOffset;
    y1 += m_dYOffset;
    x2 += m_dXOffset;
    y2 += m_dYOffset;
    x3 += m_dXOffset;
    y3 += m_dYOffset;

    TPoint p[4];
    p[0].x = x1; p[0].y = y1;
    p[1].x = x2; p[1].y = y2;
    p[2].x = x3; p[2].y = y3;
    p[3].x = x1; p[3].y = y1;

    p[0].x *= m_dXScale; p[0].y *= m_dYScale;
    p[1].x *= m_dXScale; p[1].y *= m_dYScale;
    p[2].x *= m_dXScale; p[2].y *= m_dYScale;
    p[3].x *= m_dXScale; p[3].y *= m_dYScale;

    if (IsFill) {
        SetPolyFillMode(m_hDc, WINDING);
        Polygon(m_hDc, p, 4);
    }
    else Polyline(m_hDc, p, 4);

    DelePen();
    DeleBrush();
}



void __stdcall CGdi::Rect(bool IsFill, int x1, int y1, int x2, int y2, int cs)
{
    if (m_hDc==NULL) return;
    if (m_tBrush.Style!=BS_HATCHED) m_tBrush.Style = IsFill ? BS_SOLID : BS_HOLLOW;

    if (cs!=0) LinePlus(x1+abs(x2-x1)/2, y1+abs(y2-y1)/2, cs);

    MakePen();
    MakeBrush();

    if (x1==x2 || y1==y2) {
            x1 += m_dXOffset; x1 *= m_dXScale;
            y1 += m_dYOffset; y1 *= m_dYScale;
            x2 += m_dXOffset; x2 *= m_dXScale;
            y2 += m_dYOffset; y2 *= m_dYScale;

            TPoint p;
            MoveToEx(m_hDc, x1, y1, &p);
            LineTo  (m_hDc, x2, y2    );


    }//Line(x1, y1, x2, y2);
    else {
        x1 += m_dXOffset; x1 *= m_dXScale;
        y1 += m_dYOffset; y1 *= m_dYScale;
        x2 += m_dXOffset; x2 *= m_dXScale;
        y2 += m_dYOffset; y2 *= m_dYScale;

        Rectangle(m_hDc, x1, y1, x2, y2);
    }

    DelePen();
    DeleBrush();
}

void __stdcall CGdi::Circle(bool IsFill, int x1, int y1, int x2, int y2, int cs)
{
    if (m_hDc==NULL) return;
    if (m_tBrush.Style!=BS_HATCHED) m_tBrush.Style = IsFill ? BS_SOLID : BS_HOLLOW;

    if (cs!=0) LinePlus(x1+abs(x2-x1)/2, y1+abs(y2-y1)/2, cs);

    MakePen();
    MakeBrush();

    x1 += m_dXOffset; x1 *= m_dXScale;
    y1 += m_dYOffset; y1 *= m_dYScale;
    x2 += m_dXOffset; x2 *= m_dXScale;
    y2 += m_dYOffset; y2 *= m_dYScale;

    Ellipse(m_hDc, x1, y1, x2, y2);

    DelePen();
    DeleBrush();
}




void __stdcall CGdi::TraceLine(TPoint *op, int cnt)
{
    if (m_hDc==NULL) return;

    TPoint *np = new TPoint[cnt+1];
    memcpy(np, op, cnt);
    np[cnt] = op[0];

    MakePen();
    MakeBrush();

    Polyline(m_hDc, np, cnt+1);

    DelePen();
    DeleBrush();
    delete[] np;
}





//void __stdcall CGdi::MakePalette()
//{
//    LOGPALETTE *pal;
//    HPALETTE hPal;
//
//    int palSize = GetDeviceCaps(m_hDc, SIZEPALETTE);
//
//    pal = (LOGPALETTE *)new char [sizeof(LOGPALETTE) + palSize*sizeof(PALETTEENTRY)];
//    pal->palVersion = 0x300;
//    pal->palNumEntries = palSize;
//    for (int i=0; i<palSize; i++) {
//        pal->palPalEntry[i].peRed   = (BYTE)i;
//        pal->palPalEntry[i].peGreen = (BYTE)i;
//        pal->palPalEntry[i].peBlue  = (BYTE)i;
//        pal->palPalEntry[i].peFlags = 0;
//    }
//    hPal = CreatePalette(pal);
//    delete pal;
//    SelectPalette(m_hDc, hPal, FALSE);
//    RealizePalette(m_hDc);
//    DeleteObject(hPal);
//}


void __stdcall CGdi::Line(TPoint p1, TPoint p2) {Line(p1.x, p1.y, p2.x, p2.y);}
void __stdcall CGdi::LineExt(TPoint p1, TPoint p2){LineExt(p1.x, p1.y, p2.x, p2.y);}
void __stdcall CGdi::LineX(int x , int y , int s)
{
    Line(x -s , y -s , x +s , y +s );
    Line(x +s , y -s , x -s , y +s );
}
void __stdcall CGdi::LineX(TPoint p , int s)
{
    LineX(p.x , p.y , s);
}


void __stdcall CGdi::LineCross(int x, int y, int s)
{
    Line(x-s, y-s, x+s, y+s);
    Line(x+s, y-s, x-s, y+s);
}
void __stdcall CGdi::LineCross(TPoint p, int s) {LineCross(p.x, p.y, s);}
void __stdcall CGdi::LinePlus(int x, int y, int s)
{
    Line(x-s, y  , x+s, y  );
    Line(x  , y-s, x  , y+s);
}

void __stdcall CGdi::LinePlus(TPoint p, int s) {LinePlus(p.x, p.y, s);}


void __stdcall CGdi::LineAllow(int x1, int y1, int x2, int y2, int s)
{
    double  Length = 10;
    double  r2;
    double  r3 = 15;

         if (x2 > x1) r2 = ( 180.0 * atan((double)(y2 - y1) / (double)(x1 - x2))) / 3.141519;
    else if (x2 < x1) r2 = ((180.0 * atan((double)(y2 - y1) / (double)(x1 - x2))) / 3.141519) + 180;
    else {
        if  (y2 < y1) r2 =  90;
        else          r2 = -90;
    }

    TPoint a, b;
    a = Point(x2 - (int)(Length * cos((3.141519 * (r2 + r3)) / 180)),
              y2 + (int)(Length * sin((3.141519 * (r2 + r3)) / 180)));

    b = Point(x2 - (int)(Length * cos((3.141519 * (r2 - r3)) / 180)),
              y2 + (int)(Length * sin((3.141519 * (r2 - r3)) / 180)));

    TPoint Points[3];
    Points[0].x = x2;
    Points[0].y = y2; 
    Points[1] = a;
    Points[2] = b; 

    Line(x1,y1,x2,y2);
    Triangle(true , Points[0],Points[1],Points[2]);

}

void __stdcall CGdi::Triangle(bool IsFill, TPoint p1, TPoint p2, TPoint p3){Triangle(IsFill, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);}

void __stdcall CGdi::RectNS(bool IsFill, int x1, int y1, int x2, int y2, int cs)
{
    if (m_hDc==NULL) return;

    if (cs!=0) LinePlus(x1+abs(x2-x1)/2, y1+abs(y2-y1)/2, cs);

    if (x1==x2 || y1==y2) Line(x1, y1, x2, y2);
    else {
        Rect(IsFill, x1, y1, x2, y2, cs);
    }

}

void __stdcall CGdi::Rect(bool IsFill, TPoint p1, TPoint p2, int cs)
{
    Rect(IsFill, p1.x, p1.y, p2.x, p2.y, cs);
}

void __stdcall CGdi::Rect(bool IsFill, TPoint p, int length, int cs)
{
    Rect(IsFill, p.x-length, p.y-length, p.x+length, p.y+length, cs);
}

void __stdcall CGdi::Rect(bool IsFill, RECT r, int cs) {Rect(IsFill, r.left, r.top, r.right, r.bottom, cs);}
void __stdcall CGdi::Rect(bool IsFill, TRect r, int cs) {Rect(IsFill, r.left, r.top, r.right, r.bottom, cs);}


void __stdcall CGdi::RectPoint(bool IsFill, int x1, int y1, int x2, int y2, int cs, COLORREF pencolor, int pensize, COLORREF ptcolor, int ptsize)
{
    m_tPen.Color = pencolor;
    m_tPen.Width = pensize;
    Rect(IsFill, x1, y1, x2, y2, cs);

    m_tPen.Color = ptcolor;
    m_tBrush.Color = ptcolor;
    int cx = x1+(x2-x1)*10/2/10;
    int cy = y1+(y2-y1)*10/2/10;
    Rect(true, x1-ptsize, y1-ptsize, x1+ptsize, y1+ptsize, 0);
    Rect(true, cx-ptsize, y1-ptsize, cx+ptsize, y1+ptsize, 0);
    Rect(true, x2-ptsize, y1-ptsize, x2+ptsize, y1+ptsize, 0);
    Rect(true, x2-ptsize, cy-ptsize, x2+ptsize, cy+ptsize, 0);
    Rect(true, x2-ptsize, y2-ptsize, x2+ptsize, y2+ptsize, 0);
    Rect(true, cx-ptsize, y2-ptsize, cx+ptsize, y2+ptsize, 0);
    Rect(true, x1-ptsize, y2-ptsize, x1+ptsize, y2+ptsize, 0);
    Rect(true, x1-ptsize, cy-ptsize, x1+ptsize, cy+ptsize, 0);
}

void __stdcall CGdi::RectPoint(bool IsFill, RECT r, int cs, COLORREF pencolor, int pensize, COLORREF ptcolor, int ptsize)
{
   RectPoint(IsFill, r.left, r.top, r.right, r.bottom, cs, pencolor, pensize, ptcolor, ptsize);
}

void __stdcall CGdi::Circle(bool IsFill, TPoint p1, TPoint p2, int cs) {Circle(IsFill, p1.x  , p1.y , p2.x   , p2.y    , cs);}
void __stdcall CGdi::Circle(bool IsFill, RECT   r            , int cs) {Circle(IsFill, r.left, r.top, r.right, r.bottom, cs);}

void __stdcall CGdi::Circle(bool IsFill, int    x, int y, int r, int cs) {Circle(IsFill,   x-r,   y-r,   x+r,   y+r, cs);}
void __stdcall CGdi::Circle(bool IsFill, TPoint p,        int r, int cs) {Circle(IsFill, p.x-r, p.y-r, p.x+r, p.y+r, cs);}

void __stdcall CGdi::Pixel(int x, int y, COLORREF color)
{
    if(m_dXScale <= 1.0 && m_dYScale <= 1.0) {
        x += m_dXOffset; x *= m_dXScale;
        y += m_dYOffset; y *= m_dYScale;
        SetPixelV(m_hDc, x, y, color);
    }


    //else if(m_dXScale <= 1.0 ) {
    //    Line(x, y, x+1 , y);  // Line(int x1, int y1, int x2, int y2)
    //}
    //else if(m_dYScale <= 1.0) {
    //    Line(x, y, x , y+1);  // Line(int x1, int y1, int x2, int y2)
    //}
    else {
        m_tPen  .Color = color ;
        m_tBrush.Color = color ;
        Rect (true , x , y , x+1 , y+1);
    }
}

void __stdcall CGdi::Pixel(TPoint p, COLORREF color)
{

    Pixel(p.x, p.y, color);

}
double __stdcall CGdi::PixelXSize(int pixel)
{
    double mm = GetDeviceCaps(m_hDc, HORZSIZE);
    double pc = GetDeviceCaps(m_hDc, HORZRES );
    return mm / pc * pixel;
}

double __stdcall CGdi::PixelYSize(int pixel)
{
    double mm = GetDeviceCaps(m_hDc, VERTSIZE);
    double pc = GetDeviceCaps(m_hDc, VERTRES );
    return mm / pc * pixel;
}
*/

void __stdcall CGdi::Init()
{
    m_tPen.Style       = PS_SOLID;
    m_tPen.EndStyle    = PS_ENDCAP_FLAT;
    m_tPen.JoinStyle   = PS_JOIN_MITER;
    m_tPen.Width       = 1;
    m_tPen.Color       = clBlack;

    m_tBrush.Style     = BS_SOLID;
    m_tBrush.Color     = clBlack;
    m_tBrush.Hatch     = HS_HORIZONTAL;

    m_tFont.Height     = ORGINAL_FONT_SIZE;
    m_tFont.Escapement = 0;
    m_tFont.Italic     = 0;
    m_tFont.Underline  = 0;
    m_tFont.StrikeOut  = 0;
    m_tFont.Face       = "";

    m_tText.Color      = clYellow;
    m_tText.BkColor    = clWhite;

    
    

    

}

void __stdcall CGdi::MakeText()
{
    SetBkMode   (m_hDc, TRANSPARENT);
    SetBkColor  (m_hDc, m_tText.BkColor);
    SetTextColor(m_hDc, m_tText.Color);
}

void __stdcall CGdi::MakePen()
{
    m_hNewPen =  CreatePen(m_tPen.Style, m_tPen.Width, m_tPen.Color);
    m_hOldPen = (HPEN)SelectObject(m_hDc, m_hNewPen);
    SetROP2   (m_hDc, R2_COPYPEN);
    SetBkMode (m_hDc, TRANSPARENT);
}

void __stdcall CGdi::MakePenExt()
{
    m_tLogbrush.lbStyle = m_tBrush.Style;
    m_tLogbrush.lbColor = m_tBrush.Color;
    m_tLogbrush.lbHatch = m_tBrush.Hatch;

    m_hNewPen = ExtCreatePen(PS_GEOMETRIC | m_tPen.Style | m_tPen.EndStyle | m_tPen.JoinStyle, m_tPen.Width, &m_tLogbrush, 0, NULL);
    m_hOldPen = (HPEN)SelectObject(m_hDc, m_hNewPen);
    SetROP2   (m_hDc, R2_COPYPEN);
    SetBkMode (m_hDc, TRANSPARENT);
}

void __stdcall CGdi::DelePen()
{
    SelectObject(m_hDc, m_hOldPen);
    DeleteObject(m_hNewPen);
}

void __stdcall CGdi::MakeBrush()
{
    m_tLogbrush.lbStyle = m_tBrush.Style;
    m_tLogbrush.lbColor = m_tBrush.Color;
    m_tLogbrush.lbHatch = m_tBrush.Hatch;

    m_hNewBrush = CreateBrushIndirect(&m_tLogbrush);

    m_hOldBrush = (HBRUSH)SelectObject(m_hDc, m_hNewBrush);
    SetROP2   (m_hDc, R2_COPYPEN);
    SetBkMode (m_hDc, TRANSPARENT);
}

void __stdcall CGdi::DeleBrush()
{
    SelectObject(m_hDc, m_hOldBrush);
    DeleteObject(m_hNewBrush);
}

void __stdcall CGdi::MakeFont()
{
    m_hNewFont = CreateFont(m_tFont.Height, 0, m_tFont.Escapement*10, 0, FW_MEDIUM, m_tFont.Italic, m_tFont.Underline, m_tFont.StrikeOut,
                          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, m_tFont.Face);
    m_hOldFont = (HFONT)SelectObject(m_hDc, m_hNewFont);
}

void __stdcall CGdi::DeleFont()
{
    SelectObject(m_hDc, m_hOldFont);
    DeleteObject(m_hNewFont);
}

void __stdcall CGdi::SetScale (double _fXScale  , double _fYScale ) { m_dXScale  = _fXScale  ; m_dYScale  = _fYScale  ; }
void __stdcall CGdi::SetOffset(double _fXOffset , double _fYOffset) { m_dXOffset = _fXOffset ; m_dYOffset = _fYOffset ; }

//출력 함수군.
//기본적으로 속에서 Make를 한다.
//복합 출력 함수 만들때 Make 안하게 주의..
/////////////////////////////////////////////////////////////////////////////////
void __stdcall CGdi::Text(double x, double y, char *str)
{
    if (m_hDc==NULL) return;

    double OriHeight = m_tFont.Height ;
    //m_tFont.Height *= m_dYScale ;

    MakeFont();
    MakeText();

    
    //    double ix1 = double(3 - pImg->GetOffsetX())*pImg->GetScaleX();
    //    double iy1 = double(3 - pImg->GetOffsetY())*pImg->GetScaleY();
    //
    //    double ix2 = double(5 - pImg->GetOffsetX())*pImg->GetScaleX();
    //    double iy2 = double(5 - pImg->GetOffsetY())*pImg->GetScaleY();


    x += m_dXOffset;
    y += m_dYOffset;

    x*=m_dXScale;
    y*=m_dYScale;

    TextOut(m_hDc, x, y, str, strlen(str));

    m_tFont.Height = OriHeight ;
    DeleFont();
}

//void __stdcall CGdi::Move(double x , double y)
//{
//    if (m_hDc==NULL) return;
//    TPoint p;
//    MoveToEx(m_hDc, x+m_dXOffset, y+m_dYOffset, &p);
//}

void __stdcall CGdi::Line(double x1, double y1, double x2, double y2)
{
    if (m_hDc==NULL) return;

    MakePen();

    x1 += m_dXOffset;
    y1 += m_dYOffset;
    x2 += m_dXOffset;
    y2 += m_dYOffset;

    x1*=m_dXScale;
    y1*=m_dYScale;
    x2*=m_dXScale;
    y2*=m_dYScale;



    TPoint p;
    MoveToEx(m_hDc, x1, y1, &p); //이상하게 라인투로 사각형 그리면 그냥 rectangle 쓰는 것 보다 1픽셀 더감... 나중에 알아보자. 왜그런지.
    LineTo  (m_hDc, x2, y2    );
    DelePen();
}



void __stdcall CGdi::LineExt(double x1, double y1, double x2, double y2)
{
    if (m_hDc==NULL) return;

    MakePenExt();

    x1 += m_dXOffset;
    y1 += m_dYOffset;
    x2 += m_dXOffset;
    y2 += m_dYOffset;

    x1*=m_dXScale;
    y1*=m_dYScale;
    x2*=m_dXScale;
    y2*=m_dYScale;

    TPoint p;
    MoveToEx(m_hDc, x1, y1, &p);
    LineTo  (m_hDc, x2, y2    );
    DelePen();

}


void __stdcall CGdi::Triangle(bool IsFill, double x1, double y1, double x2, double y2, double x3, double y3)
{
    if (m_hDc==NULL) return;

    MakePen();
    MakeBrush();

    x1 += m_dXOffset;
    y1 += m_dYOffset;
    x2 += m_dXOffset;
    y2 += m_dYOffset;
    x3 += m_dXOffset;
    y3 += m_dYOffset;

    TPoint p[4];
    p[0].x = x1; p[0].y = y1;
    p[1].x = x2; p[1].y = y2;
    p[2].x = x3; p[2].y = y3;
    p[3].x = x1; p[3].y = y1;

    p[0].x *= m_dXScale; p[0].y *= m_dYScale;
    p[1].x *= m_dXScale; p[1].y *= m_dYScale;
    p[2].x *= m_dXScale; p[2].y *= m_dYScale;
    p[3].x *= m_dXScale; p[3].y *= m_dYScale;

    if (IsFill) {
        SetPolyFillMode(m_hDc, WINDING);
        Polygon(m_hDc, p, 4);
    }
    else Polyline(m_hDc, p, 4);

    DelePen();
    DeleBrush();
}



void __stdcall CGdi::Rect(bool IsFill, double x1, double y1, double x2, double y2, double cs)
{
    if (m_hDc==NULL) return;
    if (m_tBrush.Style!=BS_HATCHED) m_tBrush.Style = IsFill ? BS_SOLID : BS_HOLLOW;

    if (cs!=0) LinePlus(x1+abs(x2-x1)/2, y1+abs(y2-y1)/2, cs);

    MakePen();
    MakeBrush();

    if (x1==x2 || y1==y2) {
            x1 += m_dXOffset; x1 *= m_dXScale;
            y1 += m_dYOffset; y1 *= m_dYScale;
            x2 += m_dXOffset; x2 *= m_dXScale;
            y2 += m_dYOffset; y2 *= m_dYScale;

            TPoint p;
            MoveToEx(m_hDc, x1, y1, &p);
            LineTo  (m_hDc, x2, y2    );


    }//Line(x1, y1, x2, y2);
    else {
        x1 += m_dXOffset; x1 *= m_dXScale;
        y1 += m_dYOffset; y1 *= m_dYScale;
        x2 += m_dXOffset; x2 *= m_dXScale;
        y2 += m_dYOffset; y2 *= m_dYScale;

        Rectangle(m_hDc, x1, y1, x2, y2);
    }

    DelePen();
    DeleBrush();
}

void __stdcall CGdi::Circle(bool IsFill, double x1, double y1, double x2, double y2, double cs)
{
    if (m_hDc==NULL) return;
    if (m_tBrush.Style!=BS_HATCHED) m_tBrush.Style = IsFill ? BS_SOLID : BS_HOLLOW;

    if (cs!=0) LinePlus(x1+abs(x2-x1)/2, y1+abs(y2-y1)/2, cs);

    MakePen();
    MakeBrush();

    x1 += m_dXOffset; x1 *= m_dXScale;
    y1 += m_dYOffset; y1 *= m_dYScale;
    x2 += m_dXOffset; x2 *= m_dXScale;
    y2 += m_dYOffset; y2 *= m_dYScale;

    Ellipse(m_hDc, x1, y1, x2, y2);

    DelePen();
    DeleBrush();
}




void __stdcall CGdi::TraceLine(TPoint *op, int cnt)
{
    if (m_hDc==NULL) return;       

    TPoint *np = new TPoint[cnt+1];
    memcpy(np, op, cnt);
    np[cnt] = op[0];

    MakePen();
    MakeBrush();

    Polyline(m_hDc, np, cnt+1);

    DelePen();
    DeleBrush();
    delete[] np;
}





//void __stdcall CGdi::MakePalette()
//{
//    LOGPALETTE *pal;
//    HPALETTE hPal;
//
//    double palSize = GetDeviceCaps(m_hDc, SIZEPALETTE);
//
//    pal = (LOGPALETTE *)new char [sizeof(LOGPALETTE) + palSize*sizeof(PALETTEENTRY)];
//    pal->palVersion = 0x300;
//    pal->palNumEntries = palSize;
//    for (double i=0; i<palSize; i++) {
//        pal->palPalEntry[i].peRed   = (BYTE)i;
//        pal->palPalEntry[i].peGreen = (BYTE)i;
//        pal->palPalEntry[i].peBlue  = (BYTE)i;
//        pal->palPalEntry[i].peFlags = 0;
//    }
//    hPal = CreatePalette(pal);
//    delete pal;
//    SelectPalette(m_hDc, hPal, FALSE);
//    RealizePalette(m_hDc);
//    DeleteObject(hPal);
//}


void __stdcall CGdi::Line(TPoint p1, TPoint p2) {Line(p1.x, p1.y, p2.x, p2.y);}
void __stdcall CGdi::LineExt(TPoint p1, TPoint p2){LineExt(p1.x, p1.y, p2.x, p2.y);}
void __stdcall CGdi::LineX(double x , double y , double s)
{
    Line(x -s , y -s , x +s , y +s );
    Line(x +s , y -s , x -s , y +s );
}
void __stdcall CGdi::LineX(TPoint p , double s)
{
    LineX(p.x , p.y , s);
}


void __stdcall CGdi::LineCross(double x, double y, double s)
{
    Line(x-s, y-s, x+s, y+s);
    Line(x+s, y-s, x-s, y+s);
}
void __stdcall CGdi::LineCross(TPoint p, double s) {LineCross(p.x, p.y, s);}
void __stdcall CGdi::LinePlus(double x, double y, double s)
{
    Line(x-s, y  , x+s, y  );
    Line(x  , y-s, x  , y+s);
}

void __stdcall CGdi::LinePlus(TPoint p, double s) {LinePlus(p.x, p.y, s);}


void __stdcall CGdi::LineAllow(double x1, double y1, double x2, double y2, double s)
{
    double  Length = 10;
    double  r2;
    double  r3 = 15;

         if (x2 > x1) r2 = ( 180.0 * atan((double)(y2 - y1) / (double)(x1 - x2))) / 3.141519;
    else if (x2 < x1) r2 = ((180.0 * atan((double)(y2 - y1) / (double)(x1 - x2))) / 3.141519) + 180;
    else {
        if  (y2 < y1) r2 =  90;
        else          r2 = -90;
    }

    TPoint a, b;
    a = Point(x2 - (double)(Length * cos((3.141519 * (r2 + r3)) / 180)),
              y2 + (double)(Length * sin((3.141519 * (r2 + r3)) / 180)));

    b = Point(x2 - (double)(Length * cos((3.141519 * (r2 - r3)) / 180)),
              y2 + (double)(Length * sin((3.141519 * (r2 - r3)) / 180)));

    TPoint Points[3];
    Points[0].x = x2;
    Points[0].y = y2; 
    Points[1] = a;
    Points[2] = b; 

    Line(x1,y1,x2,y2);
    Triangle(true , Points[0],Points[1],Points[2]);

}

void __stdcall CGdi::Triangle(bool IsFill, TPoint p1, TPoint p2, TPoint p3){Triangle(IsFill, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);}

void __stdcall CGdi::RectNS(bool IsFill, double x1, double y1, double x2, double y2, double cs)
{
    if (m_hDc==NULL) return;

    if (cs!=0) LinePlus(x1+abs(x2-x1)/2, y1+abs(y2-y1)/2, cs);

    if (x1==x2 || y1==y2) Line(x1, y1, x2, y2);
    else {
        Rect(IsFill, x1, y1, x2, y2, cs);
    }

}

void __stdcall CGdi::Rect(bool IsFill, TPoint p1, TPoint p2, double cs)
{
    Rect(IsFill, p1.x, p1.y, p2.x, p2.y, cs);
}

void __stdcall CGdi::Rect(bool IsFill, TPoint p, double length, double cs)
{
    Rect(IsFill, p.x-length, p.y-length, p.x+length, p.y+length, cs);
}

void __stdcall CGdi::Rect(bool IsFill, RECT r, double cs) {Rect(IsFill, r.left, r.top, r.right, r.bottom, cs);}
void __stdcall CGdi::Rect(bool IsFill, TRect r, double cs) {Rect(IsFill, r.left, r.top, r.right, r.bottom, cs);}


void __stdcall CGdi::RectPoint(bool IsFill, double x1, double y1, double x2, double y2, double cs, COLORREF pencolor, double pensize, COLORREF ptcolor, double ptsize)
{
    m_tPen.Color = pencolor;
    m_tPen.Width = pensize;
    Rect(IsFill, x1, y1, x2, y2, cs);

    m_tPen.Color = ptcolor;
    m_tBrush.Color = ptcolor;
    double cx = x1+(x2-x1)*10/2/10;
    double cy = y1+(y2-y1)*10/2/10;
    Rect(true, x1-ptsize, y1-ptsize, x1+ptsize, y1+ptsize, 0);
    Rect(true, cx-ptsize, y1-ptsize, cx+ptsize, y1+ptsize, 0);
    Rect(true, x2-ptsize, y1-ptsize, x2+ptsize, y1+ptsize, 0);
    Rect(true, x2-ptsize, cy-ptsize, x2+ptsize, cy+ptsize, 0);
    Rect(true, x2-ptsize, y2-ptsize, x2+ptsize, y2+ptsize, 0);
    Rect(true, cx-ptsize, y2-ptsize, cx+ptsize, y2+ptsize, 0);
    Rect(true, x1-ptsize, y2-ptsize, x1+ptsize, y2+ptsize, 0);
    Rect(true, x1-ptsize, cy-ptsize, x1+ptsize, cy+ptsize, 0);
}

void __stdcall CGdi::RectPoint(bool IsFill, RECT r, double cs, COLORREF pencolor, double pensize, COLORREF ptcolor, double ptsize)
{
   RectPoint(IsFill, r.left, r.top, r.right, r.bottom, cs, pencolor, pensize, ptcolor, ptsize);
}

void __stdcall CGdi::Circle(bool IsFill, TPoint p1, TPoint p2, double cs) {Circle(IsFill, p1.x  , p1.y , p2.x   , p2.y    , cs);}
void __stdcall CGdi::Circle(bool IsFill, RECT   r            , double cs) {Circle(IsFill, r.left, r.top, r.right, r.bottom, cs);}

void __stdcall CGdi::Circle(bool IsFill, double    x, double y, double r, double cs) {Circle(IsFill,   x-r,   y-r,   x+r,   y+r, cs);}
void __stdcall CGdi::Circle(bool IsFill, TPoint p,        double r, double cs) {Circle(IsFill, p.x-r, p.y-r, p.x+r, p.y+r, cs);}

void __stdcall CGdi::Pixel(double x, double y, COLORREF color)
{
    if(m_dXScale <= 1.0 && m_dYScale <= 1.0) {
        x += m_dXOffset; x *= m_dXScale;
        y += m_dYOffset; y *= m_dYScale;
        SetPixelV(m_hDc, x, y, color);
    }


    //else if(m_dXScale <= 1.0 ) {
    //    Line(x, y, x+1 , y);  // Line(double x1, double y1, double x2, double y2)
    //}
    //else if(m_dYScale <= 1.0) {
    //    Line(x, y, x , y+1);  // Line(double x1, double y1, double x2, double y2)
    //}
    else {
        m_tPen  .Color = color ;
        m_tBrush.Color = color ;
        Rect (true , x , y , x+1 , y+1);
    }
}

void __stdcall CGdi::Pixel(TPoint p, COLORREF color)
{

    Pixel(p.x, p.y, color);

}
double __stdcall CGdi::PixelXSize(double pixel)
{
    double mm = GetDeviceCaps(m_hDc, HORZSIZE);
    double pc = GetDeviceCaps(m_hDc, HORZRES );
    return mm / pc * pixel;
}

double __stdcall CGdi::PixelYSize(double pixel)
{
    double mm = GetDeviceCaps(m_hDc, VERTSIZE);
    double pc = GetDeviceCaps(m_hDc, VERTRES );
    return mm / pc * pixel;
}
