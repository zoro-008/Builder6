#ifndef SVImageH
#define SVImageH
#include <graphics.hpp>
////////////
//Image   //
//////////// 8비트 흑백 Bitmap 24비트 컬러(RGB) Bitmap포멧만 지원. //////////

//new로 생성하지 말고 SVInterface.h에 있는 함수 사용하자.

class IImage {
    protected : //Var.
        int             m_iWidth     ;  //이미지의 넓이.
        int             m_iHeight    ;  //이미지의 높이.
        int             m_iBitCnt    ;  //이미지의 비트수. (8==흑백 , 24==컬러)

        int             m_iMemWidth8  ;  //8비트이미지의 4의 배수로 떨어지는 이미지 넓이..
        int             m_iMemWidth24 ;  //24비트이미지의 4의 배수로 떨어지는 이미지 넓이..

        float           m_fScaleX    ;  //이미지 스케일X.
        float           m_fScaleY    ;  //이미지 스케일Y.

        //이미지 프로세싱 및 노멀 디스플레이용.
        BITMAPINFO    * m_pBitInfo8  ;  //8비트  원이미지용 BITMAPINFO
        BITMAPINFO    * m_pBitInfo24 ;  //24비트 원이미지용 BITMAPINFO
        unsigned char * m_pImage     ;  //이미지의 처음 시작 포인터 그레이 이미지.
        unsigned char * m_pImageC    ;  //이미지의 처음 시작 포인터 컬러 이미지.

        //CRITICAL_SECTION g_pTemp ;

        //Display 용.
        //BITMAPINFO    * m_pBitInfoD  ;  //변환 디스플레이용. 비트맵인포. ex)falseColor , Binary.. etc..
        //unsigned char * m_pImageD    ;  //변환 디스플레이용. R , G , B



    public : //생성자 소멸자.
        virtual void __stdcall Del()=0;

    public :
        virtual bool __stdcall CheckRangeOver(int   _x , int   _y)=0; 
        virtual bool __stdcall CheckRangeOver(float _x , float _y)=0;
        
        virtual unsigned char __stdcall GetPixel (int _x , int _y)=0;
        virtual unsigned char __stdcall GetPixelR(int _x , int _y)=0;
        virtual unsigned char __stdcall GetPixelG(int _x , int _y)=0;
        virtual unsigned char __stdcall GetPixelB(int _x , int _y)=0;
        
        virtual void __stdcall SetPixel (int _x , int _y , unsigned char _v)=0;
        virtual void __stdcall SetPixelR(int _x , int _y , unsigned char _v)=0;
        virtual void __stdcall SetPixelG(int _x , int _y , unsigned char _v)=0;
        virtual void __stdcall SetPixelB(int _x , int _y , unsigned char _v)=0;
        
        //스케일이 적용된 Pixel. 첨조만 성립.
        virtual unsigned char __stdcall GetPixelSc (int _x , int _y )=0;
        virtual unsigned char __stdcall GetPixelScR(int _x , int _y )=0;
        virtual unsigned char __stdcall GetPixelScG(int _x , int _y )=0;
        virtual unsigned char __stdcall GetPixelScB(int _x , int _y )=0;
        
        //(0,0) , (1,0) , (0,1) , (1,1) 네픽셀의 밝기와 float형 위치값을 입력
        virtual unsigned char __stdcall GetSubPixel(float _x , float _y )=0;
        
        virtual unsigned char * __stdcall GetImg ()=0;
        virtual unsigned char * __stdcall GetImgC()=0;

        virtual int  __stdcall GetHeight()=0;
        virtual int  __stdcall GetWidth ()=0;
        virtual int  __stdcall GetImgBit()=0;

        virtual int  __stdcall GetMem8Width ()=0; //GetImg()를 써서 메모리 수동 조작시에 4의 배수 넓이를 가져가야 한다.
        virtual int  __stdcall GetMem24Width ()=0; //GetImg()를 써서 메모리 수동 조작시에 4의 배수 넓이를 가져가야 한다.




        virtual float __stdcall GetScaleX()=0;
        virtual float __stdcall GetScaleY()=0;

        virtual int __stdcall GetImgX(int _x)=0;
        virtual int __stdcall GetImgY(int _y)=0;

    public :
        virtual bool __stdcall LoadBmp (char * _FilePath)=0; //이미지를 로드 하는 함수.
        virtual bool __stdcall LoadJpg (char * _FilePath)=0; //
        virtual bool __stdcall SaveBmp (char * _FilePath)=0; //이미지를 세이브 하는 함수.
        virtual bool __stdcall SaveJpg (char * _FilePath)=0; //


        //외부에서 더블버퍼링 할때
        //Scale
        virtual int __stdcall ShowImg      (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgGrey  (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgFalse (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgColor (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgBinary(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH, int _iThreshold )=0;
        virtual int __stdcall ShowImgR     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgG     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgB     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;

        virtual bool __stdcall SetSize   (int _iWidth , int _iHeight , int _iBitCnt)=0; //사이즈 바꾸는 함수 인데.

        //화면에 이미지만 다이렉트로 그릴경우 더블버퍼링 지원 함수.
        virtual HBITMAP __stdcall GetHBitmap(HDC _hHdc)=0;
        virtual void    __stdcall ShowBitmap(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;

        //Set Image
        virtual bool __stdcall SetImage   (unsigned char * _pImage  , int _iWidth , int _iHeight , int _iBitCnt)=0;
        virtual void __stdcall ClrImage   ()=0;
        virtual void __stdcall operator = (IImage * _pImg)=0;

        virtual void __stdcall Rotate(TRect _tRect , float _fAngle)=0; //Rect부분만 회전 시키기.
        virtual void __stdcall Trim     (TRect _tRect , IImage * _tDstImg)=0;
        virtual void __stdcall TrimScale(TRect _tRect , IImage * _tDstImg , float _fScale)=0;

    public:
        //검사 영역 들이 이미지의 크기에서 벋어나 있을때에 이미지영역으로 세팅 하는 함수군
        virtual bool __stdcall SetRect (TRect *_pRect , int _iMargin = 0)=0;
        virtual bool __stdcall SetLine (TPoint *_pSpnt , TPoint *_pEpnt , int _iMargin = 0)=0;
        virtual bool __stdcall SetPoint (TPoint *_pPnt , int _iMargin = 0)=0;
};


#endif