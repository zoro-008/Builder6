//---------------------------------------------------------------------------

#ifndef UnitAreaH
#define UnitAreaH



#include <Classes.hpp>
#include "BaseDefine.h"



//---------------------------------------------------------------------------

#define MAX_COLOR_8 256                                               //8비트 영상 색상수.
#define BYTE_BIT_COUNT 8                                              //1바이트의 비트 개수.
#define BMP_INFO_SIZE_8  sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD) //팔레트 가지고 있는 256컬러 인포 크기.
#define BMP_INFO_SIZE_24 sizeof(BITMAPINFO)                           //펄레트 필요 없는 24비트 인포 크기.

//Pixel Info.
#define AreaPx(Area, Width , x , y) (*(Area   + x + Width * y))





class CArea{//: public IArea{
    protected : //Var.
        int m_iStartX ; // 이미지상에서 아리아 위치.
        int m_iStartY ; // 이미지 크기만큼 아리아 할당하고 안써도 되고 이미지 보다 작은 크기로 할당할 경우.. 외부에서 알고 있거나 혹은 이것을 쓰면 됌.

        //높이 넓이.
        int             m_iWidth   ;  //AREA의 넓이.
        int             m_iHeight  ;  //AREA의 높이.
        int             m_iMemWidth;  //4의 배수로 넓이 메모리 할당해야함.

        //실제 메모리.
        unsigned char * m_pArea    ;  //이미지의 처음 시작 포인터 그레이 이미지.

        //Display용.
        BITMAPINFO    * m_pBitInfo ;  //변환 디스플레이용. 비트맵인포. ex)falseColor , Binary.. etc..
        HBITMAP         hBitmap    ;

        //Display Color
        COLORREF m_iPxColor[MAX_COLOR_8];


    protected : //Func.
        int  __stdcall BitToByte(int _BitCnt) ;
        bool __stdcall SetPalette(int _iWidth , int _iHeight);
        bool __stdcall SetPaletteColor();




    public : //생성자 소멸자.

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


            //값을 세팅 할때는 세팅 하고
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
                            //나중에 넣자..
                        }
                    }
                }
            }
            */
            return   ;
        }

        unsigned char * __stdcall GetArea();

        //Get 함수.
        int  __stdcall GetHeight();
        int  __stdcall GetWidth ();
        int  __stdcall GetMemWidth();


    public :
        //Load Save 함수.
        bool __stdcall LoadBmp (char * _FilePath); //이미지를 로드 하는 함수.
        bool __stdcall SaveBmp (char * _FilePath); //이미지를 세이브 하는 함수.

        bool __stdcall SetSize  (int _iWidth  , int _iHeight ); //사이즈 바꾸는 함수 인데. 요거수행되면 올클리어 됌.
        bool __stdcall SetStart (int _iStartX , int _iEndX   ); //사이즈 바꾸는 함수 인데. 요거수행되면 올클리어 됌.
        int  __stdcall GetStartX(); //사이즈 바꾸는 함수 인데. 요거수행되면 올클리어 됌.
        int  __stdcall GetStartY();

        //도트로 영역을 다 찍는다. 뒷배경 살아 있음.
        bool __stdcall ShowArea (HDC _hHdc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY , int _x, int _y );

        //외부에서 더블버퍼링 할때 뒷배경 지워지고 오로지 아리아만 그려짐. 주의.
        //bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor);

        //Set Area
        bool __stdcall SetArea  (int _iWidth , int _iHeight) ;
        void __stdcall ClrArea  ();

        bool __stdcall SetRect  (TRect   *_pRect , int _iMargin = 0);

        //가산 감산 함수.
        void __stdcall AddCircle (TCircle   _tCircle, unsigned char _iVal );
        void __stdcall AddRect   (TRect     _tRect  , unsigned char _iVal );
        void __stdcall AddLine   (double _dX1 , double _dX2 , double _dY1 , double _dY2 , unsigned char _iVal );
        void __stdcall AddLine   (TDPoint _tPnt1 , TDPoint _tPnt2 , unsigned char _iVal );

        void __stdcall AddPolygon (TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal );
        void __stdcall AddPolygon2(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal );
        void __stdcall AddPolygon3(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal );

        void __stdcall CopyPolygonFromArea (TDPoint * _aPoints, int _iPointCnt , CArea * _pSrcArea );

    protected : //Func.
        void __stdcall FindBaundary(unsigned char * _pPxEdge , TRect _tRect); //테두리를 돌면서 엣지 추출. 땀.

        void  ErosionThread (TRect _tRect , int _iSize); //병신같다 쓰지 말자.

    public :
        //테두리 돌면서 땀.
        void __stdcall ErosionTrace(TDRect _tRect , int _iSize); //테두리를 돌면서 땀.          //_iSize가 10개 이하이면 유리함..

        //테두리 돌면서 땀.
        void __stdcall DilationTrace(TDRect _tRect , int _iSize , unsigned char _cPx); //테두리를 돌면서 땀.          //_iSize가 10개 이하이면 유리함..


        //쓰레드 이용하여 가로 세로 수행함.
        void __stdcall Erosion    (TDRect _tRect , int _iSize); //가로,세로로 1D로 각각 수행함. //_iSize가 10개 이상이면 유리함.. 병신같다 쓰지 말자.




};

#endif





