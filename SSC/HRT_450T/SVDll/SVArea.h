#ifndef SVAreaH
#define SVAreaH

#include "SVDefine.h"

//#include <wingdi.h>
//#include <Classes.hpp>
//new로 생성하지 말고 SVInterface.h에 있는 함수 사용하자.

#define MAX_COLOR_8 256                                               //8비트 영상 색상수.
#define BYTE_BIT_COUNT 8                                              //1바이트의 비트 개수.
#define BMP_INFO_SIZE_8  sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD) //팔레트 가지고 있는 256컬러 인포 크기.
#define BMP_INFO_SIZE_24 sizeof(BITMAPINFO)                           //펄레트 필요 없는 24비트 인포 크기.

class IArea {
    protected : //Var.
        int m_iStartX ; //Area안(0~m_iWidth )에서 0이 아닌놈으로 세팅되어 있는 가장 작은 X
        int m_iStartY ; //Area안(0~m_iHeight)에서 0이 아닌놈으로 세팅되어 있는 가장 작은 y
        int m_iEndX   ; //Area안(0~m_iWidth )에서 0이 아닌놈으로 세팅되어 있는 가장 큰 X
        int m_iEndY   ; //Area안(0~m_iHeight)에서 0이 아닌놈으로 세팅되어 있는 가장 큰 y

        //높이 넓이.
        int             m_iWidth   ;  //AREA의 넓이.
        int             m_iHeight  ;  //AREA의 높이.
        int             m_iMemWidth;  //4의 배수로 넓이 메모리 할당해야함.

        //실제 메모리.
        unsigned char * m_pArea    ;  //이미지의 처음 시작 포인터 그레이 이미지.

        //Display용.
        BITMAPINFO    * m_pBitInfo ;  //변환 디스플레이용. 비트맵인포. ex)falseColor , Binary.. etc..
        HBITMAP         hBitmap    ;

    public :
        //Display Color
        COLORREF m_iPxColor[MAX_COLOR_8];

        virtual void __stdcall Del()=0 ;

    protected : //Func.
        virtual int  __stdcall BitToByte(int _BitCnt) =0;
        virtual bool __stdcall SetPalette(int _iWidth , int _iHeight)=0;
        virtual bool __stdcall SetPaletteColor()=0;

        virtual COLORREF __stdcall GetColor(int _iPx) =0;
    public :
        virtual bool          __stdcall CheckRangeOver(int _x , int _y) =0;

        virtual unsigned char __stdcall GetPixel (int _x , int _y )=0;
        virtual void          __stdcall SetPixel (int _x , int _y , unsigned char _v)=0;

        virtual unsigned char * __stdcall GetArea()=0;


        //Load Save 함수.
        virtual bool __stdcall LoadBmp (char * _FilePath)=0;
        virtual bool __stdcall SaveBmp (char * _FilePath)=0;

        virtual bool __stdcall SetSize(int _iWidth , int _iHeight)=0;

        //virtual bool __stdcall SetOffset(int _iX , int _iY)=0;


        //Get 함수.
        virtual int  __stdcall GetHeight()=0;
        virtual int  __stdcall GetWidth ()=0;
        virtual int  __stdcall GetMemWidth ()=0;

        virtual int  __stdcall GetCntVal (unsigned char _v)=0;

    public :

        //도트로 영역을 다 찍는다. 뒷배경 살아 있음.
        virtual bool __stdcall ShowArea (HDC _hHdc , int _x, int _y, float _fScaleX , float _fScaleY )=0;

        //외부에서 더블버퍼링 할때 뒷배경 지워지고 오로지 아리아만 그려짐. 주의.
        virtual bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor)=0;

        //화면에 이미지만 다이렉트로 그릴경우 더블버퍼링 지원 함수. 뒷배경 지워지고 오로지 아리아만 그려짐. 주의.
        //virtual HBITMAP __stdcall GetHBitmapOnBlack(HDC _hHdc)=0;
        //virtual void    __stdcall ShowBitmapOnBlack(HDC _hHdc, int _x, int _y, int _iWidth, int _iHeight)=0;


        //Set Area
        virtual bool __stdcall SetArea  (int _iWidth , int _iHeight)=0 ;
        virtual void __stdcall ClrArea  ()=0;

        //Make Area
        virtual void __stdcall AddCircle (TCircle _tCircle, unsigned char _iVal )=0;
        virtual void __stdcall AddRect   (TRect   _tRect  , unsigned char _iVal )=0;



        /*
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
        */



};







#endif
 