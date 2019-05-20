//---------------------------------------------------------------------------

#ifndef SVDefineH
#define SVDefineH
//---------------------------------------------------------------------------


/*
#include <windef.h>

#ifndef TypesHPP

  struct TPoint : public POINT
  {
    TPoint() {}
    TPoint(int _x, int _y) { x=_x; y=_y; }
    TPoint(POINT& pt)
    {
      x = pt.x;
      y = pt.y;
    }
  };
 
  typedef TPoint tagPoint;
 
  struct TRect : public RECT
  {
    TRect() {}
    TRect(const TPoint& TL, const TPoint& BR) { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TRect(int l, int t, int r, int b)         { left=l;    top=t;    right=r;    bottom=b;    }
    TRect(RECT& r)
    {
      left    = r.left;
      top     = r.top;
      right   = r.right;
      bottom  = r.bottom;
    }
    int Width () const { return right  - left; }
    int Height() const { return bottom - top ; }
    bool operator ==(const TRect& rc) const 
    {
       return left ==  rc.left  && top==rc.top &&     
              right == rc.right && bottom==rc.bottom; 
    }
    bool operator !=(const TRect& rc) const 
    {  return !(rc==*this); }
 
       __property LONG Left    = { read=left,   write=left   }; 
       __property LONG Top     = { read=top,       write=top }; 
       __property LONG Right   = { read=right,  write=right  }; 
       __property LONG Bottom  = { read=bottom, write=bottom }; 
  };
#endif
*/
//---------------------------------------------------------------------------
#include <Types.hpp>
struct TTagPoint : public TPoint
{
    int iTag ;
};

struct TFPoint
{
    float x , y ;
    TFPoint() {x=0.0 ; y=0.0;}
    TFPoint(float _fX, float _fY) { x=_fX; y=_fY; }
};

struct TDPoint
{
    double x , y ;
    TDPoint() {x=0.0 ; y=0.0;}
    TDPoint(double _fX, double _fY) { x=_fX; y=_fY; }
};

struct TFRect
{

    float left,top,right,bottom ;
    TFRect() {}
    TFRect(const TPoint& TL, const TPoint& BR){ left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TFRect(float l, float t, float r, float b){ left=l;    top=t;    right=r;    bottom=b;    }
    TFRect(TFRect& r)
    {
      left    = r.left;
      top     = r.top;
      right   = r.right;
      bottom  = r.bottom;
    }
    float Width () const { return right  - left; }
    float Height() const { return bottom - top ; }
    RECT GetRECT() {RECT Rect ; Rect.left = left ; Rect.top = top ; Rect.right = right ; Rect.bottom = bottom ; return Rect ; }


    bool operator ==(const TFRect& rc) const
    {
       return left ==  rc.left  && top==rc.top &&
              right == rc.right && bottom==rc.bottom;
    }
    bool operator !=(const TFRect& rc) const
    {  return !(rc==*this); }

       __property float Left    = { read=left  , write=left  };
       __property float Top     = { read=top   , write=top   };
       __property float Right   = { read=right , write=right };
       __property float Bottom  = { read=bottom, write=bottom};
};

struct TDRect
{

    double left,top,right,bottom ;
    TDRect() {}
    TDRect(const TPoint& TL, const TPoint& BR)    { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TDRect(double l, double t, double r, double b){ left=l;    top=t;    right=r;    bottom=b;    }
    TDRect(TFRect& r)
    {
      left    = r.left;
      top     = r.top;
      right   = r.right;
      bottom  = r.bottom;
    }
    double Width () const { return right  - left; }
    double Height() const { return bottom - top ; }
    RECT GetRECT() {RECT Rect ; Rect.left = left ; Rect.top = top ; Rect.right = right ; Rect.bottom = bottom ; return Rect ; }


    bool operator ==(const TDRect& rc) const
    {
       return left ==  rc.left  && top==rc.top &&
              right == rc.right && bottom==rc.bottom;
    }
    bool operator !=(const TDRect& rc) const
    {  return !(rc==*this); }

       __property double Left    = { read=left  , write=left  };
       __property double Top     = { read=top   , write=top   };
       __property double Right   = { read=right , write=right };
       __property double Bottom  = { read=bottom, write=bottom};
};

struct TCircle
{
    int cx,cy,rad ;
    TCircle(){cx=cy=rad=0;}
    TCircle(int _fCx , int _fCy , int _fRad){cx=_fCx; cy=_fCy ; rad=_fRad;}
};

struct TFCircle
{
    float cx,cy,rad ;
    TFCircle(){cx=cy=rad=0.0;}
    TFCircle(float _fCx , float _fCy , float _fRad){cx=_fCx; cy=_fCy ; rad=_fRad;}
};

struct TDCircle
{
    double cx,cy,rad ;
    TDCircle(){cx=cy=rad=0.0;}
    TDCircle(double _fCx , double _fCy , double _fRad){cx=_fCx; cy=_fCy ; rad=_fRad;}
};

struct _SBlob : public TRect {
    int      iBoundCnt ;

    int    iCenterX     ;
    int    iCenterY     ;
    int    iWidth       ;
    int    iHeight      ;
    int    iArea        ;
    float  fBoundary    ;

    int    iStartX      ;
    int    iStartY      ;


    float  fCompactness ;
    float  fCentroidX   ;
    float  fCentroidY   ;
    float  fAngle       ;
    float  fAspectRatio ;

    float  fXA          ;
    float  fXB          ;

    float  fYA          ;
    float  fYB          ;
};

class SBlob : public _SBlob {
    public :
       SBlob() {
           OnClear();

       };
       ~SBlob() {OnClear();};

       void OnClear() {
           iBoundCnt    = 0   ;
           iCenterX     = 0   ;
           iCenterY     = 0   ;
           iWidth       = 0   ;
           iHeight      = 0   ;
           iArea        = 0   ;
           iStartX      = 0   ;
           iStartY      = 0   ;
           fBoundary    = 0.0 ;
           fCompactness = 0.0 ;
           fCentroidX   = 0.0 ;
           fCentroidY   = 0.0 ;
           fAngle       = 0.0 ;
           fAspectRatio = 0.0 ;
           fXA          = 0.0 ;
           fXB          = 0.0 ;
           fYA          = 0.0 ;
           fYB          = 0.0 ;
           left   = 0 ;
           top    = 0 ;
           right  = 0 ;
           bottom = 0 ;
       }

       void Copy(SBlob & rhs) {
           iBoundCnt    = rhs.iBoundCnt    ;

           left         = rhs.left         ;
           right        = rhs.right        ;
           top          = rhs.top          ;
           bottom       = rhs.bottom       ;


           iCenterX     = rhs.iCenterX     ;
           iCenterY     = rhs.iCenterY     ;
           iWidth       = rhs.iWidth       ;
           iHeight      = rhs.iHeight      ;
           iArea        = rhs.iArea        ;
           fBoundary    = rhs.fBoundary    ;
           fCompactness = rhs.fCompactness ;
           fCentroidX   = rhs.fCentroidX   ;
           fCentroidY   = rhs.fCentroidY   ;
           fAngle       = rhs.fAngle       ;
           fAspectRatio = rhs.fAspectRatio ;
           fXA          = rhs.fXA          ;
           fXB          = rhs.fXB          ;
           fYA          = rhs.fYA          ;
           fYB          = rhs.fYB          ;
       }

       //Overriding the Operator
       void operator = (SBlob & rhs) {
           Copy(rhs);
       }
};

class SOcrBlob : public SBlob {
    public :
       SOcrBlob() {OnClear();};
       ~SOcrBlob() {OnClear();};

       AnsiString      sText    ;

       //unsigned char * InspInfo ;

       void OnClear() {
           OnClear();
           sText = "";
           //if(InspInfo) {
           //    delete [] InspInfo ;
           //    InspInfo = NULL ;
           //}
       }

       //Overriding the Operator
       void operator = (SOcrBlob & rhs) {
           Copy((SBlob)rhs);
           sText = rhs.sText ;
       }
};

double __fastcall GetTickTimeDouble(void)
{
    LARGE_INTEGER liEndCounter,liFrequency ;
    ::QueryPerformanceCounter  (&liEndCounter);
    ::QueryPerformanceFrequency(&liFrequency );

    return ( ((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000.0 );
}

//float __fastcall GetTickTime(void)
//{
//    LARGE_INTEGER liEndCounter,liFrequency ;
//    ::QueryPerformanceCounter  (&liEndCounter);
//    ::QueryPerformanceFrequency(&liFrequency );
//
//    return ( ((float)liEndCounter.QuadPart / (float)liFrequency.QuadPart) * 1000.0 );
//}


//TColor GetStepColor()
//{
//    const int iMaxColor = 17 ;
//    static int iColorIndex = -1 ;
//    static TColor Colors[iMaxColor]={clInactiveCaption ,
//                                     clMaroon          ,
//                                     clPurple          ,
//                                     clTeal            ,
//                                     clOlive           ,
//                                     clNavy            ,
//                                     clMoneyGreen      ,
//                                     clGreen           ,
//                                     clHighlight       ,
//                                     clSkyBlue         ,
//                                     clSilver          ,
//                                     clAqua            ,
//                                     clBlue            ,
//                                     clYellow          ,
//                                     clFuchsia         ,
//                                     clRed             ,
//                                     clLime            };
//
//    iColorIndex++ ;
//    if(iColorIndex >= iMaxColor) iColorIndex = -1 ;
//    return Colors[iColorIndex++];
//}


typedef void (__closure * InspectionFunc   )();

//런 쓰레드
#include <Classes.hpp>
template <typename T>
class CRunThread
{
    class TRunThread : public TThread {
        private:
            T Func;
        public:
            __fastcall TRunThread(T func) : TThread(false) {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute() {
                Func();
            }
    };

    public:
        CRunThread(T func) {
            TRunThread*  thread = new TRunThread(func);
        }
};

#endif

