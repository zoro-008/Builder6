#ifndef SVAreaH
#define SVAreaH

#include "SVDefine.h"

//#include <wingdi.h>
//#include <Classes.hpp>
//new�� �������� ���� SVInterface.h�� �ִ� �Լ� �������.

#define MAX_COLOR_8 256                                               //8��Ʈ ���� �����.
#define BYTE_BIT_COUNT 8                                              //1����Ʈ�� ��Ʈ ����.
#define BMP_INFO_SIZE_8  sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD) //�ȷ�Ʈ ������ �ִ� 256�÷� ���� ũ��.
#define BMP_INFO_SIZE_24 sizeof(BITMAPINFO)                           //�޷�Ʈ �ʿ� ���� 24��Ʈ ���� ũ��.

class IArea {
    protected : //Var.
        int m_iStartX ; //Area��(0~m_iWidth )���� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ���� X
        int m_iStartY ; //Area��(0~m_iHeight)���� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ���� y
        int m_iEndX   ; //Area��(0~m_iWidth )���� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ū X
        int m_iEndY   ; //Area��(0~m_iHeight)���� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ū y

        //���� ����.
        int             m_iWidth   ;  //AREA�� ����.
        int             m_iHeight  ;  //AREA�� ����.
        int             m_iMemWidth;  //4�� ����� ���� �޸� �Ҵ��ؾ���.

        //���� �޸�.
        unsigned char * m_pArea    ;  //�̹����� ó�� ���� ������ �׷��� �̹���.

        //Display��.
        BITMAPINFO    * m_pBitInfo ;  //��ȯ ���÷��̿�. ��Ʈ������. ex)falseColor , Binary.. etc..
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


        //Load Save �Լ�.
        virtual bool __stdcall LoadBmp (char * _FilePath)=0;
        virtual bool __stdcall SaveBmp (char * _FilePath)=0;

        virtual bool __stdcall SetSize(int _iWidth , int _iHeight)=0;

        //virtual bool __stdcall SetOffset(int _iX , int _iY)=0;


        //Get �Լ�.
        virtual int  __stdcall GetHeight()=0;
        virtual int  __stdcall GetWidth ()=0;
        virtual int  __stdcall GetMemWidth ()=0;

        virtual int  __stdcall GetCntVal (unsigned char _v)=0;

    public :

        //��Ʈ�� ������ �� ��´�. �޹�� ��� ����.
        virtual bool __stdcall ShowArea (HDC _hHdc , int _x, int _y, float _fScaleX , float _fScaleY )=0;

        //�ܺο��� ������۸� �Ҷ� �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
        virtual bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor)=0;

        //ȭ�鿡 �̹����� ���̷�Ʈ�� �׸���� ������۸� ���� �Լ�. �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
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

        //�ܺο��� ������۸� �Ҷ� �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
        bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor);

        //ȭ�鿡 �̹����� ���̷�Ʈ�� �׸���� ������۸� ���� �Լ�. �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
        //���� �ȉ� ���߿� ����.
        //HBITMAP __stdcall GetHBitmapOnBlack(HDC _hHdc);
        //void    __stdcall ShowBitmapOnBlack(HDC _hHdc, int _x, int _y, int _iWidth, int _iHeight);


        //Set Area
        bool __stdcall SetArea  (int _iWidth , int _iHeight) ;
        void __stdcall ClrArea  ();

        //���� ���� �Լ�.
        void __stdcall AddCircle (TCircle _tCircle, unsigned char _iVal );
        void __stdcall AddRect   (TRect   _tRect  , unsigned char _iVal );
        */



};







#endif
 