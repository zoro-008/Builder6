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

    public : //생성자 소멸자.

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

        //스케일이 적용된 Pixel. 첨조만 성립.
        unsigned char __stdcall GetPixelSc (int _x , int _y );
        unsigned char __stdcall GetPixelScR(int _x , int _y );
        unsigned char __stdcall GetPixelScG(int _x , int _y );
        unsigned char __stdcall GetPixelScB(int _x , int _y );

        //(0,0) , (1,0) , (0,1) , (1,1) 네픽셀의 밝기와 float형 위치값을 입력
        unsigned char __stdcall GetSubPixel(float _x , float _y ) ; */

        bool __stdcall CImage::CheckRangeOver(int   _x , int   _y) { return (0>_x   || _x>=m_iWidth   || 0>_y   || _y>=m_iHeight  );}
        bool __stdcall CImage::CheckRangeOver(float _x , float _y) { return (0>_x-1 || _x>=m_iWidth+1 || 0>_y-1 || _y>=m_iHeight+1);}

                                                                                       //느려서 못쓰겠다.
        unsigned char __stdcall CImage::GetPixel (int _x , int _y                   ){ /*if(!m_pImage  || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImage  + (_x + m_iMemWidth8 * _y)                                ); };  //픽셀밝기값을 얻어 온다.
        unsigned char __stdcall CImage::GetPixelR(int _x , int _y                   ){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 2 ); };
        unsigned char __stdcall CImage::GetPixelG(int _x , int _y                   ){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 1 ); };
        unsigned char __stdcall CImage::GetPixelB(int _x , int _y                   ){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return 0 ;*/ return *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT     ); };

        void          __stdcall CImage::SetPixel (int _x , int _y , unsigned char _v){ /*if(!m_pImage  || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImage  + (_x + m_iMemWidth8 * _y)                                )= _v ; };  //픽셀밝기값을 세팅한다.
        void          __stdcall CImage::SetPixelR(int _x , int _y , unsigned char _v){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 2 )= _v ; };
        void          __stdcall CImage::SetPixelG(int _x , int _y , unsigned char _v){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT + 1 )= _v ; };
        void          __stdcall CImage::SetPixelB(int _x , int _y , unsigned char _v){ /*if(!m_pImageC || CheckRangeOver(_x,_y)) return   ;*/        *(m_pImageC + (_x + m_iMemWidth8 * _y) * m_iBitCnt/BYTE_BIT_COUNT     )= _v ; };

        //스케일이 적용된 Pixel. 첨조만 성립.
        unsigned char __stdcall CImage::GetPixelSc (int _x , int _y ){ /*if(!m_pImage  || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImage  + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY)                                ); };   //픽셀밝기값을 얻어 온다.
        unsigned char __stdcall CImage::GetPixelScR(int _x , int _y ){ /*if(!m_pImageC || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImageC + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY) * m_iBitCnt/BYTE_BIT_COUNT + 2 ); };
        unsigned char __stdcall CImage::GetPixelScG(int _x , int _y ){ /*if(!m_pImageC || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImageC + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY) * m_iBitCnt/BYTE_BIT_COUNT + 1 ); };
        unsigned char __stdcall CImage::GetPixelScB(int _x , int _y ){ /*if(!m_pImageC || CheckRangeOver(_x/m_fScaleX,_y/m_fScaleY)) return 0 ;*/ return *(m_pImageC + (int)(_x/m_fScaleX + m_iMemWidth8 * _y/m_fScaleY) * m_iBitCnt/BYTE_BIT_COUNT     ); };
                                                                                                                                                                           
        //(0,0) , (1,0) , (0,1) , (1,1) 네픽셀의 밝기와 float형 위치값을 입력
        unsigned char __stdcall CImage::GetSubPixel(float _x , float _y ) {
            if(!m_pImage  || CheckRangeOver(_x,_y)) return 0 ;

            unsigned char PxV00 , PxV10 , PxV01 , PxV11 ; //서브픽셀을 구할 네개의 픽셀값.
            float        PxSubX , PxSubY ; //픽셀 소수점. 픽셀비중을 이것으로 구한다.
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
        int  __stdcall GetMem8Width(); //  GetImg()를 써서 메모리 수동 조작시에 4의 배수 넓이를 가져가야 한다.
        int  __stdcall GetMem24Width(); //  GetImg()를 써서 메모리 수동 조작시에 4의 배수 넓이를 가져가야 한다.

        float __stdcall GetScaleX() ;
        float __stdcall GetScaleY() ;

        int __stdcall GetImgX(int _x);
        int __stdcall GetImgY(int _y);

    public :
        bool __stdcall LoadBmp (char * _FilePath); //이미지를 로드 하는 함수.
        bool __stdcall LoadJpg (char * _FilePath); //
        bool __stdcall SaveBmp (char * _FilePath); //이미지를 세이브 하는 함수.
        bool __stdcall SaveJpg (char * _FilePath); //


        //외부에서 더블버퍼링 할때
        //Scale
        int __stdcall ShowImg      (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgGrey  (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgFalse (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgColor (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgBinary(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH, int _iThreshold );
        int __stdcall ShowImgR     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgG     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);
        int __stdcall ShowImgB     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);

        bool __stdcall SetSize   (int _iWidth , int _iHeight , int _iBitCnt); //사이즈 바꾸는 함수 인데.

        //화면에 이미지만 다이렉트로 그릴경우 더블버퍼링 지원 함수.
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

        //8비트만 지원 함...
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 8000ms 회당 80ms
        //release mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 6500ms 회당 65ms

        //2013.07.10 memcpy 방식으로 수정후.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 7000ms 회당 70ms
        //release mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 6000ms 회당 60ms

        //2013.07.10 sin cos for에서 뺌. 방식으로 수정후.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 6500ms 회당 65ms

        //2013.07.10 이미지 예외 처리 없애고GetPixel만쓴경우.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 6000ms 회당 60ms

        //2013.07.10 이미지 GetPixel한수 안쓰고 메모리 바로 쓴경우.
        //debug   mode 640X480 Intel(R) Core(TM) i5 CPU M560 @ 2.67GHz 1.3Ghz, 2.98GB RAM 에서 100회 약 3200ms 회당 32ms

        void __stdcall Rotate   (TRect _tRect , float _fAngle);
        void __stdcall Trim     (TRect _tRect , IImage * _tDstImg);
        void __stdcall TrimScale(TRect _tRect , IImage * _tDstImg , float _fScale);

    public:
        //검사 영역 들이 이미지의 크기에서 벋어나 있을때에 이미지영역으로 세팅 하는 함수군 return 은 위치가 변경 됐으면 true ;
        bool __stdcall SetRect (TRect *_pRect , int _iMargin = 0   );
        bool __stdcall SetLine (TPoint *_pSpnt , TPoint *_pEpnt , int _iMargin = 0 );
        bool __stdcall SetPoint (TPoint *_pPnt , int _iMargin = 0 );

        //todo
        //Rect 받아서 일부분만 복사해서 리턴 해주는 함수.

};

#endif





/*
wingdi.h 파일에 BITMAPFILEHEADER  선언을 보면 아래와 같습니다.

#include <pshpack2.h>
typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits; 
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER; 
#include <poppack.h> 

구조체의 내용을 직접 size 계산해 보십시요 
WORD 형이 3 개 , DWORD형이 2개 입니다. 
그럼 SIZE = 2 * 3  +  4 * 2  =  6 + 8  =  14    , 14Byte 입니다.

그런데 프로그램에서  sizeof(BITMAPFILEHEADER) 해 보십시요
16 이 나올겁니다.

왜 16이 나올까요?
원래 32bit CPU  시스템에서 메모리할당을 32bit씩 합니다.
위와 같은 구조체를 그냥 sizeof하면 16byte가 나옵니다

이걸 14byte나오게 하기 위해서 1byte또는 2byte단위로 묶도록 설정해주는것이 필요합니다. 

위 구조체에서도 2byte단위로 묶도록   #include <pshpack2.h>  이 문구가 앞에 있습니다. 

그런데 잘안되네요  ==> 원인은 저도 잘 모릅니다. 

---------------------------------------------------------

그래서 그 위 구조체를 그대로 복사해서 다른이름으로 선언해봣습니다. 

#pragma pack(push,1)  // 또는 #pragma pack(push,2)   , 또는 #pragma pack(2) 
typedef struct tagBITMAPFILEHEADER2 { 
        WORD    bfType; 
        DWORD   bfSize;
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits;
} BITMAPFILEHEADER2; 
#pragma pack(pop)


BITMAPFILEHEADER2 라는 이름으로 

위에서 #pragma pack(push,1) 문구는 1byte씩 묶어라는 말입니다.

코드에서

int hsize=sizeof(BITMAPFILEHEADER);
int hsize2=sizeof(BITMAPFILEHEADER2);

해보십시요

hsize=16 이지만
hsize2 = 14 입니다.

재선언한 구조체를 이용해 보세요

#include <pshpack2.h> 이 왜 안먹히는지는 연구좀 해봐야....

그럼 즐플.....

*/



