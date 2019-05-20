//---------------------------------------------------------------------------

#ifndef AreaH
#define AreaH

#include "SVArea.h"


//---------------------------------------------------------------------------








class CArea: public IArea{


    protected : //Var.
        //시작점 옵셑   추후작업하면 빨라짐.
        //int m_iXOffset; //이미지 위에 Area X중에 가장 작은놈
        //int m_iYOffset; //이미지 위에 Area Y중에 가장 작은놈
        //int m_iStartX ; //Area안에서 0이 아닌놈으로 세팅되어 있는 가장 작은 X
        //int m_iStartY ; //Area안에서 0이 아닌놈으로 세팅되어 있는 가장 작은 y
        //int m_iEndX   ; //Area안에서 0이 아닌놈으로 세팅되어 있는 가장 큰 X
        //int m_iEndY   ; //Area안에서 0이 아닌놈으로 세팅되어 있는 가장 큰 y

        ////높이 넓이.
        //int             m_iWidth     ;  //이미지의 넓이.
        //int             m_iHeight    ;  //이미지의 높이.
        //
        ////실제 메모리.
        //unsigned char * m_pArea      ;  //이미지의 처음 시작 포인터 그레이 이미지.
        //
        ////Display용.
        //BITMAPINFO    * m_pBitInfo   ;  //변환 디스플레이용. 비트맵인포. ex)falseColor , Binary.. etc..
        //HBITMAP         hBitmap      ;
        //

    protected : //Func.
        int  __stdcall BitToByte(int _BitCnt) ;
        bool __stdcall SetPalette(int _iWidth , int _iHeight);
        bool __stdcall SetPaletteColor();


        COLORREF __stdcall GetColor(int _iPx){return m_iPxColor[_iPx] ;}

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

        unsigned char __stdcall GetPixel (int _x , int _y){
            if(!m_pArea   || CheckRangeOver(_x,_y))return 0;
            return *(m_pArea   + (_x + m_iMemWidth * _y));
        };
        void __stdcall SetPixel (int _x , int _y , unsigned char _v){
            if(!m_pArea   || CheckRangeOver(_x,_y))return ;
            *(m_pArea   + (_x + m_iMemWidth * _y))= _v ;


            //값을 세팅 할때는 세팅 하고
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

                        }
                    }
                }
            }
            return   ;
        }

        unsigned char * __stdcall GetArea();

        //Get 함수.
        int  __stdcall GetHeight();
        int  __stdcall GetWidth ();
        int  __stdcall GetMemWidth();


        int  __stdcall GetCntVal (unsigned char _v);

    public :
        //Load Save 함수.
        bool __stdcall LoadBmp (char * _FilePath); //이미지를 로드 하는 함수.
        bool __stdcall SaveBmp (char * _FilePath); //이미지를 세이브 하는 함수.

        bool __stdcall SetSize(int _iWidth , int _iHeight); //사이즈 바꾸는 함수 인데. 요거수행되면 올클리어 됌.
        //bool __stdcall SetOffset(int _iX , int _iY);

        
        


        //도트로 영역을 다 찍는다. 뒷배경 살아 있음.
        bool __stdcall ShowArea (HDC _hHdc , int _x, int _y , float _fScaleX , float _fScaleY );

        //외부에서 더블버퍼링 할때 뒷배경 지워지고 오로지 아리아만 그려짐. 주의.
        bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor);

        //화면에 이미지만 다이렉트로 그릴경우 더블버퍼링 지원 함수. 뒷배경 지워지고 오로지 아리아만 그려짐. 주의.
        //검증 안됌 나중에 하자.
        //HBITMAP __stdcall GetHBitmapOnBlack(HDC _hHdc);
        //void    __stdcall ShowBitmapOnBlack(HDC _hHdc, int _x, int _y, int _iWidth, int _iHeight);


        //Set Area
        bool __stdcall SetArea  (int _iWidth , int _iHeight) ;
        void __stdcall ClrArea  ();

        //가산 감산 함수.
        void __stdcall AddCircle (TCircle _tCircle, unsigned char _iVal );
        void __stdcall AddRect   (TRect   _tRect  , unsigned char _iVal );


        //todo

        //확장 함수.
        //void __stdcall ExpendLine  (TPoint  _tPnt   ){} //직선   - 점입력하여 현재 픽셀이 '1'이면 현재 픽셀 부터 그점까지 다 '1'로 세팅.
        //void __stdcall ExpendRect  (TRect   _tRect  ){} //사각형 - 넓이 높이 입력 하여 현재 픽셀이 '1'이면 현재 픽셀을 중심으로 하여 확장.
        //void __stdcall ExpendCircle(TCircle _tCircle){} //원형   - 지름입력 하여 현재 픽셀이 '1'이면 현재픽셀을 중심으로 원내부 다 '1'로 세팅.

        //축소 함수.
        //void __stdcall ReduceLine  (TPoint  _tPnt   ){} //직선   - 점입력하여 현재 픽셀이 '0'이면 현재 픽셀 부터 그점까지 다 '0'로 세팅.
        //void __stdcall ReduceRect  (TRect   _tRect  ){} //사각형 - 넓이 높이 입력 하여 현재 픽셀이 '0'이면 현재 픽셀을 중심으로 하여 '0'으로 세팅.
        //void __stdcall ReduceCircle(TCircle _tCircle){} //원형   - 지름입력 하여 현재 픽셀이 '0'이고 현재픽셀을 중심으로 원내부 다 '0'로 세팅.

        //연결 함수.
        //void __stdcall ConnectLine  (TPoint  _tPnt   ){} //직선   - 점입력하여 현재 픽셀이 '1'이고 입력된 점도'1'이면 현재 픽셀 부터 그점까지 다 '1'로 세팅.

        //에리아 비트별로 쓸수 있게.
        




};

#endif





