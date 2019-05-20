//---------------------------------------------------------------------------

#ifndef UnitAreaH
#define UnitAreaH



#include <Classes.hpp>
#include "BaseDefine.h"



//---------------------------------------------------------------------------

#define MAX_COLOR_8 256                                               //8��Ʈ ���� �����.
#define BYTE_BIT_COUNT 8                                              //1����Ʈ�� ��Ʈ ����.
#define BMP_INFO_SIZE_8  sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD) //�ȷ�Ʈ ������ �ִ� 256�÷� ���� ũ��.
#define BMP_INFO_SIZE_24 sizeof(BITMAPINFO)                           //�޷�Ʈ �ʿ� ���� 24��Ʈ ���� ũ��.

//Pixel Info.
#define AreaPx(Area, Width , x , y) (*(Area   + x + Width * y))





class CArea{//: public IArea{
    protected : //Var.
        int m_iStartX ; // �̹����󿡼� �Ƹ��� ��ġ.
        int m_iStartY ; // �̹��� ũ�⸸ŭ �Ƹ��� �Ҵ��ϰ� �Ƚᵵ �ǰ� �̹��� ���� ���� ũ��� �Ҵ��� ���.. �ܺο��� �˰� �ְų� Ȥ�� �̰��� ���� ��.

        //���� ����.
        int             m_iWidth   ;  //AREA�� ����.
        int             m_iHeight  ;  //AREA�� ����.
        int             m_iMemWidth;  //4�� ����� ���� �޸� �Ҵ��ؾ���.

        //���� �޸�.
        unsigned char * m_pArea    ;  //�̹����� ó�� ���� ������ �׷��� �̹���.

        //Display��.
        BITMAPINFO    * m_pBitInfo ;  //��ȯ ���÷��̿�. ��Ʈ������. ex)falseColor , Binary.. etc..
        HBITMAP         hBitmap    ;

        //Display Color
        COLORREF m_iPxColor[MAX_COLOR_8];


    protected : //Func.
        int  __stdcall BitToByte(int _BitCnt) ;
        bool __stdcall SetPalette(int _iWidth , int _iHeight);
        bool __stdcall SetPaletteColor();




    public : //������ �Ҹ���.

        CArea();
        ~CArea();



        void __stdcall Del();

        //Overriding the Operator
        //void operator = (CArea& _Src) {
        //    SetSize(_Src.m_iMemWidth , _Src.m_iHeight);
        //    memcpy(m_pArea      , _Src.m_pArea      , m_iMemWidth * m_iHeight );
        //    memcpy(m_pBitInfo   , _Src.m_pBitInfo   , BMP_INFO_SIZE_8 );
        //    memcpy(hBitmap      , _Src.hBitmap      , sizeof(HBITMAP) );
        //}

    public :
        bool __stdcall CheckRangeOver(int _x , int _y) { return (0>_x || _x>=m_iWidth || 0>_y || _y>=m_iHeight);}

        COLORREF __stdcall GetColor(int _iPx){return m_iPxColor[_iPx] ;}
        void SetColor(int _iPx , COLORREF _iColor){m_iPxColor[_iPx] = _iColor;}

        inline unsigned char __stdcall GetPixel (int _x , int _y){
            if(!m_pArea   || CheckRangeOver(_x,_y))return 0;
            return *(m_pArea   + (_x + m_iMemWidth * _y));
        };
        inline void __stdcall SetPixel (int _x , int _y , unsigned char _v){ //inline
            if(!m_pArea   || CheckRangeOver(_x,_y))return ;
            *(m_pArea   + (_x + m_iMemWidth * _y))= _v ;


            //���� ���� �Ҷ��� ���� �ϰ�
            /*
            if(_v) {
                if(m_iStartX > _x ) m_iStartX = _x ;
                if(m_iStartY > _y ) m_iStartY = _y ;

                if(m_iEndX   < _x ) m_iEndX   = _x ;
                if(m_iEndY   < _y ) m_iEndY   = _y ;
            }
            else {
                if(m_iStartX == _x || m_iStartY == _y || m_iEndX  == _x || m_iEndY == _y){
                    for(int x = m_iStartX ; x < m_iEndX ; x++){
                        for(int y = m_iStartY ; y < m_iEndY ; y++) {
                            //���߿� ����..
                        }
                    }
                }
            }
            */
            return   ;
        }

        unsigned char * __stdcall GetArea();

        //Get �Լ�.
        int  __stdcall GetHeight();
        int  __stdcall GetWidth ();
        int  __stdcall GetMemWidth();


    public :
        //Load Save �Լ�.
        bool __stdcall LoadBmp (char * _FilePath); //�̹����� �ε� �ϴ� �Լ�.
        bool __stdcall SaveBmp (char * _FilePath); //�̹����� ���̺� �ϴ� �Լ�.

        bool __stdcall SetSize  (int _iWidth  , int _iHeight ); //������ �ٲٴ� �Լ� �ε�. ��ż���Ǹ� ��Ŭ���� ��.
        bool __stdcall SetStart (int _iStartX , int _iEndX   ); //������ �ٲٴ� �Լ� �ε�. ��ż���Ǹ� ��Ŭ���� ��.
        int  __stdcall GetStartX(); //������ �ٲٴ� �Լ� �ε�. ��ż���Ǹ� ��Ŭ���� ��.
        int  __stdcall GetStartY();

        //��Ʈ�� ������ �� ��´�. �޹�� ��� ����.
        bool __stdcall ShowArea (HDC _hHdc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY , int _x, int _y );

        //�ܺο��� ������۸� �Ҷ� �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
        //bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor);

        //Set Area
        bool __stdcall SetArea  (int _iWidth , int _iHeight) ;
        void __stdcall ClrArea  ();

        bool __stdcall SetRect  (TRect   *_pRect , int _iMargin = 0);

        //���� ���� �Լ�.
        void __stdcall AddCircle (TCircle   _tCircle, unsigned char _iVal );
        void __stdcall AddRect   (TRect     _tRect  , unsigned char _iVal );
        void __stdcall AddLine   (double _dX1 , double _dX2 , double _dY1 , double _dY2 , unsigned char _iVal );
        void __stdcall AddLine   (TDPoint _tPnt1 , TDPoint _tPnt2 , unsigned char _iVal );

        void __stdcall AddPolygon (TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal );
        void __stdcall AddPolygon2(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal );
        void __stdcall AddPolygon3(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal );

        void __stdcall CopyPolygonFromArea (TDPoint * _aPoints, int _iPointCnt , CArea * _pSrcArea );

    protected : //Func.
        void __stdcall FindBaundary(unsigned char * _pPxEdge , TRect _tRect); //�׵θ��� ���鼭 ���� ����. ��.

        void  ErosionThread (TRect _tRect , int _iSize); //���Ű��� ���� ����.

    public :
        //�׵θ� ���鼭 ��.
        void __stdcall ErosionTrace(TDRect _tRect , int _iSize); //�׵θ��� ���鼭 ��.          //_iSize�� 10�� �����̸� ������..

        //�׵θ� ���鼭 ��.
        void __stdcall DilationTrace(TDRect _tRect , int _iSize , unsigned char _cPx); //�׵θ��� ���鼭 ��.          //_iSize�� 10�� �����̸� ������..


        //������ �̿��Ͽ� ���� ���� ������.
        void __stdcall Erosion    (TDRect _tRect , int _iSize); //����,���η� 1D�� ���� ������. //_iSize�� 10�� �̻��̸� ������.. ���Ű��� ���� ����.




};

#endif





