//---------------------------------------------------------------------------

#ifndef ImageH
#define ImageH

#include "SVImage.h"
//---------------------------------------------------------------------------


#define BYTE_BIT_COUNT 8
class CImage : public IImage{
    protected : //Func.
        int  __stdcall BitToByte (int _BitCnt) ;
        bool __stdcall SetPalette(int _iWidth , int _iHeight);

    public : //������ �Ҹ���.

        CImage();                    //
        ~CImage();

        void __stdcall Del();

    public :
        /*
        bool __stdcall CheckRangeOver(int   _x , int   _y) ;
        bool __stdcall CheckRangeOver(float _x , float _y) ;

        unsigned char __stdcall GetPixel (int _x , int _y                   );
        unsigned char __stdcall GetPixelR(int _x , int _y                   );
        unsigned char __stdcall GetPixelG(int _x , int _y                   );
        unsigned char __stdcall GetPixelB(int _x , int _y                   );

        void          __stdcall SetPixel (int _x , int _y , unsigned char _v);
        void          __stdcall SetPixelR(int _x , int _y , unsigned char _v);
        void          __stdcall SetPixelG(int _x , int _y , unsigned char _v);
        void          __stdcall SetPixelB(int _x , int _y , unsigned char _v);

        //�������� ����� Pixel. ÷���� ����.
        unsigned char __stdcall GetPixelSc (int _x , int _y );
        unsigned char __stdcall GetPixelScR(int _x , int _y );
        unsigned char __stdcall GetPixelScG(int _x , int _y );
        unsigned char __stdcall GetPixelScB(int _x , int _y );

        //(0,0) , (1,0) , (0,1) , (1,1) ���ȼ��� ���� float�� ��ġ���� �Է�
        unsigned char __stdcall GetSubPixel(float _x , float _y ) ; */

        bool __stdcall CImage::CheckRangeOver(int   _x , int   _y) { return (0>_x   || _x>=m_iWidth   || 0>_y   || _y>=m_iHeight  );}
        bool __stdcall CImage::CheckRangeOver(float _x , float _y) { return (0>_x-1 || _x>=m_iWidth+1 || 0>_y-1 || _y>=m_iHeight+1);}

                                                                                       //������ �����ڴ�.
        unsigned char __stdcall CImage::GetPixel (int _x , int _y                   ){ /*if(!m_pImage  || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImage  + (_x + m_iMemWidth8 * _y)                                ); };  //�ȼ���Ⱚ�� ��� �´�.
        unsigned char __stdcall CImage::GetPixelR(int _x , int _y                   ){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 2 ); };
        unsigned char __stdcall CImage::GetPixelG(int _x , int _y                   ){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 1 ); };
        unsigned char __stdcall CImage::GetPixelB(int _x , int _y                   ){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT     ); };

        void          __stdcall CImage::SetPixel (int _x , int _y , unsigned char _v){ /*if(!m_pImage  || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImage  + (_x + m_iMemWidth8 * _y)                                )= _v ; };  //�ȼ���Ⱚ�� �����Ѵ�.
        void          __stdcall CImage::SetPixelR(int _x , int _y , unsigned char _v){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 2 )= _v ; };
        void          __stdcall CImage::SetPixelG(int _x , int _y , unsigned char _v){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 1 )= _v ; };
        void          __stdcall CImage::SetPixelB(int _x , int _y , unsigned char _v){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT     )= _v ; };

        //�������� ����� Pixel. ÷���� ����.
        unsigned char __stdcall CImage::GetPixelSc (int _x , int _y ){ /*if(!m_pImage  || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImage  + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY)                                ); };   //�ȼ���Ⱚ�� ��� �´�.
        unsigned char __stdcall CImage::GetPixelScR(int _x , int _y ){ /*if(!m_pImageC || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImageC + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY) * m_iBitCnt/BYTE_BIT_COUNT + 2 ); };
        unsigned char __stdcall CImage::GetPixelScG(int _x , int _y ){ /*if(!m_pImageC || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImageC + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY) * m_iBitCnt/BYTE_BIT_COUNT + 1 ); };
        unsigned char __stdcall CImage::GetPixelScB(int _x , int _y ){ /*if(!m_pImageC || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImageC + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY) * m_iBitCnt/BYTE_BIT_COUNT     ); };
                                                                                                                                                                           
        //(0,0) , (1,0) , (0,1) , (1,1) ���ȼ��� ���� float�� ��ġ���� �Է�
        unsigned char __stdcall CImage::GetSubPixel(float _x , float _y ) {
            if(!m_pImage  || CheckRangeOver(_x,_y)) return 0 ;

            unsigned char PxV00 , PxV10 , PxV01 , PxV11 ; //�����ȼ��� ���� �װ��� �ȼ���.
            float        PxSubX , PxSubY ; //�ȼ� �Ҽ���. �ȼ������� �̰����� ���Ѵ�.
            float        SubPxVal ;

            PxSubX = _x - (int)_x ;
            PxSubY = _y - (int)_y ;

            PxV00 = GetPixel(_x     , _y    );
            PxV10 = GetPixel(_x + 1 , _y    );
            PxV01 = GetPixel(_x     , _y + 1);
            PxV11 = GetPixel(_x + 1 , _y + 1);

            SubPxVal = (1-PxSubX)*(1-PxSubY)*PxV00 +
                       (  PxSubX)*(1-PxSubY)*PxV10 +
                       (1-PxSubX)*(  PxSubY)*PxV01 +
                       (  PxSubX)*(  PxSubY)*PxV11 ;


            return SubPxVal ;
        }

        unsigned char * __stdcall GetImg ();
        unsigned char * __stdcall GetImgC();

        int  __stdcall GetHeight();
        int  __stdcall GetWidth ();
        int  __stdcall GetImgBit();
        int  __stdcall GetMem8Width(); //  GetImg()�� �Ἥ �޸� ���� ���۽ÿ� 4�� ��� ���̸� �������� �Ѵ�.
        int  __stdcall GetMem24Width(); //  GetImg()�� �Ἥ �޸� ���� ���۽ÿ� 4�� ��� ���̸� �������� �Ѵ�.

        float __stdcall GetScaleX() ;
        float __stdcall GetScaleY() ;

        int __stdcall GetImgX(int _x);
        int __stdcall GetImgY(int _y);

    public :
        bool __stdcall LoadBmp (char * _FilePath); //�̹����� �ε� �ϴ� �Լ�.
        bool __stdcall LoadJpg (char * _FilePath); //
        bool __stdcall SaveBmp (char * _FilePath); //�̹����� ���̺� �ϴ� �Լ�.
        bool __stdcall SaveJpg (char * _FilePath); //


        //�ܺο��� ������۸� �Ҷ�
        //Scale
        int __stdcall ShowImg      (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgGrey  (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgFalse (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgColor (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgBinary(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH, int _iThreshold );
        int __stdcall ShowImgR     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgG     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgB     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);

        bool __stdcall SetSize   (int _iWidth , int _iHeight , int _iBitCnt); //������ �ٲٴ� �Լ� �ε�.

        //ȭ�鿡 �̹����� ���̷�Ʈ�� �׸���� ������۸� ���� �Լ�.
        HBITMAP __stdcall GetHBitmap(HDC _hHdc);
        void    __stdcall ShowBitmap(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);



        //Set Image
        bool __stdcall SetImage   (unsigned char * _pImage  , int _iWidth , int _iHeight , int _iBitCnt) ;
        void __stdcall ClrImage   ();
        void __stdcall operator = (IImage * _pImg)
        {
            SetSize(_pImg->GetWidth (),_pImg->GetHeight(),_pImg->GetImgBit());

            if(m_iBitCnt == 8) {
                memcpy(m_pImage  , _pImg->GetImg()  , m_iMemWidth8 * m_iHeight);
            }
            else {
                memcpy(m_pImageC , _pImg->GetImgC()  , m_iMemWidth8 * m_iHeight* BitToByte(m_iBitCnt));
            }
        }

        //8��Ʈ�� ���� ��...
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 8000ms ȸ�� 80ms
        //release mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 6500ms ȸ�� 65ms

        //2013.07.10 memcpy ������� ������.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 7000ms ȸ�� 70ms
        //release mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 6000ms ȸ�� 60ms

        //2013.07.10 sin cos for���� ��. ������� ������.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 6500ms ȸ�� 65ms

        //2013.07.10 �̹��� ���� ó�� ���ְ�GetPixel�������.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 6000ms ȸ�� 60ms

        //2013.07.10 �̹��� GetPixel�Ѽ� �Ⱦ��� �޸� �ٷ� �����.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM ���� 100ȸ �� 3200ms ȸ�� 32ms

        void __stdcall Rotate   (TRect _tRect , float _fAngle);
        void __stdcall Trim     (TRect _tRect , IImage * _tDstImg);
        void __stdcall TrimScale(TRect _tRect , IImage * _tDstImg , float _fScale);

    public:
        //�˻� ���� ���� �̹����� ũ�⿡�� ��� �������� �̹����������� ���� �ϴ� �Լ��� return �� ��ġ�� ���� ������ true ;
        bool __stdcall SetRect (TRect *_pRect , int _iMargin = 0   );
        bool __stdcall SetLine (TPoint *_pSpnt , TPoint *_pEpnt , int _iMargin = 0 );
        bool __stdcall SetPoint (TPoint *_pPnt , int _iMargin = 0 );

        //todo
        //Rect �޾Ƽ� �Ϻκи� �����ؼ� ���� ���ִ� �Լ�.

};

#endif





/*
wingdi.h ���Ͽ� BITMAPFILEHEADER  ������ ���� �Ʒ��� �����ϴ�.

#include <pshpack2.h>
typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits; 
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER; 
#include <poppack.h> 

����ü�� ������ ���� size ����� ���ʽÿ� 
WORD ���� 3 �� , DWORD���� 2�� �Դϴ�. 
�׷� SIZE = 2 * 3  +  4 * 2  =  6 + 8  =  14    , 14Byte �Դϴ�.

�׷��� ���α׷�����  sizeof(BITMAPFILEHEADER) �� ���ʽÿ�
16 �� ���ð̴ϴ�.

�� 16�� ���ñ��?
���� 32bit CPU  �ý��ۿ��� �޸��Ҵ��� 32bit�� �մϴ�.
���� ���� ����ü�� �׳� sizeof�ϸ� 16byte�� ���ɴϴ�

�̰� 14byte������ �ϱ� ���ؼ� 1byte�Ǵ� 2byte������ ������ �������ִ°��� �ʿ��մϴ�. 

�� ����ü������ 2byte������ ������   #include <pshpack2.h>  �� ������ �տ� �ֽ��ϴ�. 

�׷��� �߾ȵǳ׿�  ==> ������ ���� �� �𸨴ϴ�. 

---------------------------------------------------------

�׷��� �� �� ����ü�� �״�� �����ؼ� �ٸ��̸����� �����ؔf���ϴ�. 

#pragma pack(push,1)  // �Ǵ� #pragma pack(push,2)   , �Ǵ� #pragma pack(2) 
typedef struct tagBITMAPFILEHEADER2 { 
        WORD    bfType; 
        DWORD   bfSize;
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits;
} BITMAPFILEHEADER2; 
#pragma pack(pop)


BITMAPFILEHEADER2 ��� �̸����� 

������ #pragma pack(push,1) ������ 1byte�� ������ ���Դϴ�.

�ڵ忡��

int hsize=sizeof(BITMAPFILEHEADER);
int hsize2=sizeof(BITMAPFILEHEADER2);

�غ��ʽÿ�

hsize=16 ������
hsize2 = 14 �Դϴ�.

�缱���� ����ü�� �̿��� ������

#include <pshpack2.h> �� �� �ȸ��������� ������ �غ���....

�׷� ����.....

*/



