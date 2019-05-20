//---------------------------------------------------------------------------

#ifndef GdiUnitH
#define GdiUnitH

#include "SVGdi.h"
//---------------------------------------------------------------------------
// 제작자 ShyGuy.
// 수정   SUN Kye Won 2009.01.04
//---------------------------------------------------------------------------
//using namespace Graphics ;
class CGdi : public IGdi{
    public:
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
            Init();
        }

        ~CGdi(){}

        void __stdcall SetScale (float _fXScale  , float _fYScale );
        void __stdcall SetOffset(float _fXOffset , float _fYOffset);

        //Make Func 펜, 브러쉬 , 폰트등등 생성을 같이 하는 기본 함수들.
        void __stdcall Text(int x, int y, char *str);
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


        void __stdcall Triangle (bool IsFill, TPoint p1, TPoint p2, TPoint p3                  );


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
        float __stdcall PixelXSize(int pixel);
        float __stdcall PixelYSize(int pixel);

        //to do
        //0~255 까지 색깔 확연하게 차이나는 것 만들기 Area에 GetColor함수 대체 하기.



};


#endif
