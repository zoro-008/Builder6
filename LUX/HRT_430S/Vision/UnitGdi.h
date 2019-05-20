//---------------------------------------------------------------------------

#ifndef UnitGdiH
#define UnitGdiH

#include <graphics.hpp>

//---------------------------------------------------------------------------
// ������ ShyGuy.
// ����   SUN Kye Won 2009.01.04
//---------------------------------------------------------------------------
//using namespace Graphics ;
class CGdi
{
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

        double m_dXScale ;
        double m_dYScale ;
        double m_dXOffset;
        double m_dYOffset;

    protected:
        void __stdcall Init();
        void __stdcall MakeText  ();
        void __stdcall MakePen   ();
        void __stdcall MakePenExt();
        void __stdcall DelePen   ();
        void __stdcall MakeBrush ();
        void __stdcall DeleBrush ();
        void __stdcall MakeFont  ();
        void __stdcall DeleFont  ();

    public:
        CGdi(HDC _hDc){
            m_hDc = _hDc ;

            m_dXScale  = 1.0;
            m_dYScale  = 1.0;
            m_dXOffset = 0.0;
            m_dYOffset = 0.0;

            Init();
        }
        CGdi(HDC _hDc,double _dXScale,double _dYScale,double _dXOffset , double _dYOffset){
            m_hDc = _hDc ;

            m_dXScale  = _dXScale ;
            m_dYScale  = _dYScale ;
            m_dXOffset = _dXOffset;
            m_dYOffset = _dYOffset;

            Init();
        }

        ~CGdi(){}

        void __stdcall SetScale (double _fXScale  , double _fYScale );
        void __stdcall SetOffset(double _fXOffset , double _fYOffset);

        //Make Func ��, �귯�� , ��Ʈ��� ������ ���� �ϴ� �⺻ �Լ���.
        void __stdcall Text     (int x, int y, char *str);
        void __stdcall Line     (int    x1, int    y1, int x2, int y2       );
        void __stdcall LineExt  (int    x1, int    y1, int x2, int y2       );
        void __stdcall Triangle (bool IsFill, int  x1, int  y1, int  x2, int y2, int x3, int y3);
        void __stdcall Rect     (bool IsFill, int  x1, int y1, int x2, int y2, int cs=0);
        void __stdcall Circle   (bool IsFill, int x1, int y1, int x2, int y2, int cs);
        void __stdcall TraceLine(TPoint *op, int cnt);
//        void __stdcall Move(int x , int y);


        void __stdcall Line     (TPoint p1, TPoint p2                       );

        void __stdcall LineExt  (TPoint p1, TPoint p2                       );
        void __stdcall LineCross(int    x , int    y ,                 int s);
        void __stdcall LineCross(TPoint p ,                            int s);
        void __stdcall LineX    (int    x , int    y ,                 int s);
        void __stdcall LineX    (TPoint p ,                            int s);
        void __stdcall LinePlus (int    x , int    y ,                 int s);
        void __stdcall LinePlus (TPoint p ,                            int s);
        void __stdcall LineAllow(int    x1, int    y1, int x2, int y2, int s=0);


        void __stdcall Triangle (bool IsFill, TPoint p1, TPoint p2, TPoint p3);


        void __stdcall RectNS   (bool IsFill, int  x1, int y1, int x2, int y2, int cs=0);
        void __stdcall Rect     (bool IsFill, TPoint p1, TPoint p2, int cs=0);
        void __stdcall Rect     (bool IsFill, TPoint p, int length, int cs=0);
        void __stdcall Rect     (bool IsFill, RECT r , int cs=0);
        void __stdcall Rect     (bool IsFill, TRect r , int cs=0);
        void __stdcall RectPoint(bool IsFill, int  x1, int y1, int x2, int y2, int cs=0, COLORREF pencolor=clAqua, int pensize=1, COLORREF ptcolor=clFuchsia, int ptsize=4);
        void __stdcall RectPoint(bool IsFill, RECT r ,                         int cs=0, COLORREF pencolor=clAqua, int pensize=1, COLORREF ptcolor=clFuchsia, int ptsize=4);


        void __stdcall Circle(bool IsFill, TPoint p1, TPoint p2, int cs);
        void __stdcall Circle(bool IsFill, RECT r, int cs);

        void __stdcall Circle(bool IsFill, int    x, int y, int r, int cs);
        void __stdcall Circle(bool IsFill, TPoint p,        int r, int cs);



        void  __stdcall Pixel(int x, int y, COLORREF color);
        void  __stdcall Pixel(TPoint p, COLORREF color);
        double __stdcall PixelXSize(int pixel);
        double __stdcall PixelYSize(int pixel);


};


#endif
