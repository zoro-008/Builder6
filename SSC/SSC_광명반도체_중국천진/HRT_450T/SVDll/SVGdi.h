#ifndef SVGdiH
#define SVGdiH

#include <graphics.hpp>
class IGdi {
    public:
        struct TConfigText {
            COLORREF Color  ;
            COLORREF BkColor;
        };

        struct TConfigPen {
            int      Style     ;
            int      EndStyle  ;
            int      JoinStyle ;
            int      Width     ;
            COLORREF Color     ;
        };

        struct TConfigBrush {
            int      Style ;
            int      Hatch ;
            COLORREF Color ;
        };

        struct TConfigFont {
            int   Height     ;
            int   Escapement ;
            int   Italic     ;
            int   Underline  ;
            int   StrikeOut  ;
            char *Face       ;
        };

        TConfigText  m_tText     ;
        TConfigPen   m_tPen      ;
        TConfigBrush m_tBrush    ;
        TConfigFont  m_tFont     ;

    protected:
        HDC          m_hDc       ;
        HPEN         m_hNewPen   , m_hOldPen  ;
        HBRUSH       m_hNewBrush , m_hOldBrush;
        HFONT        m_hNewFont  , m_hOldFont ;
        LOGBRUSH     m_tLogbrush ;
        LOGFONT      m_tLogfont  ;

        float m_fXScale ;
        float m_fYScale ;
        int   m_iXOffset;
        int   m_iYOffset;

    public:

        virtual void __stdcall SetScale (float _fXScale  , float _fYScale )=0;
        virtual void __stdcall SetOffset(float _fXOffset , float _fYOffset)=0;

        virtual void __stdcall Text(int x, int y, char *str)=0;

        virtual void __stdcall Line     (int    x1, int    y1, int x2, int y2       )=0;
        virtual void __stdcall Line     (TPoint p1, TPoint p2                       )=0;
        virtual void __stdcall LineExt  (int    x1, int    y1, int x2, int y2       )=0;
        virtual void __stdcall LineExt  (TPoint p1, TPoint p2                       )=0;
        virtual void __stdcall LineCross(int    x , int    y ,                 int s)=0;
        virtual void __stdcall LineCross(TPoint p ,                            int s)=0;
        virtual void __stdcall LinePlus (int    x , int    y ,                 int s)=0;
        virtual void __stdcall LinePlus (TPoint p ,                            int s)=0;
        virtual void __stdcall LineAllow(int    x1, int    y1, int x2, int y2, int s=0)=0;

        virtual void __stdcall Triangle (bool IsFill, int  x1, int  y1, int  x2, int y2, int x3, int y3)=0;
        virtual void __stdcall Triangle (bool IsFill, TPoint p1, TPoint p2, TPoint p3                  )=0;

        virtual void __stdcall Rect     (bool IsFill, int  x1, int y1, int x2, int y2, int cs=0)=0;
        virtual void __stdcall RectNS   (bool IsFill, int  x1, int y1, int x2, int y2, int cs=0)=0;
        virtual void __stdcall Rect     (bool IsFill, TPoint p1, TPoint p2, int cs=0)=0;
        virtual void __stdcall Rect     (bool IsFill, TPoint p, int length, int cs=0)=0;
        virtual void __stdcall Rect     (bool IsFill, RECT r , int cs=0)=0;
        virtual void __stdcall Rect     (bool IsFill, TRect r , int cs=0)=0;
        virtual void __stdcall RectPoint(bool IsFill, int  x1, int y1, int x2, int y2, int cs=0, COLORREF pencolor=0xffff00, int pensize=1, COLORREF ptcolor=0xff00ff, int ptsize=4)=0;
        virtual void __stdcall RectPoint(bool IsFill, RECT r ,                         int cs=0, COLORREF pencolor=0xffff00, int pensize=1, COLORREF ptcolor=0xff00ff, int ptsize=4)=0;

        virtual void __stdcall Circle(bool IsFill, int x1, int y1, int x2, int y2, int cs)=0;
        virtual void __stdcall Circle(bool IsFill, TPoint p1, TPoint p2, int cs)=0;
        virtual void __stdcall Circle(bool IsFill, RECT r, int cs)=0;

        virtual void __stdcall Circle(bool IsFill, int    x, int y, int r, int cs)=0;
        virtual void __stdcall Circle(bool IsFill, TPoint p,        int r, int cs)=0;

        virtual void __stdcall TraceLine(TPoint *op, int cnt)=0;

        virtual void  __stdcall Pixel(int x, int y, COLORREF color)=0;
        virtual void  __stdcall Pixel(TPoint p, COLORREF color)=0;
        virtual float __stdcall PixelXSize(int pixel)=0;
        virtual float __stdcall PixelYSize(int pixel)=0;


};










#endif