#ifndef SVImageH
#define SVImageH
#include <graphics.hpp>
////////////
//Image   //
//////////// 8��Ʈ ��� Bitmap 24��Ʈ �÷�(RGB) Bitmap���丸 ����. //////////

//new�� �������� ���� SVInterface.h�� �ִ� �Լ� �������.

class IImage {
    protected : //Var.
        int             m_iWidth     ;  //�̹����� ����.
        int             m_iHeight    ;  //�̹����� ����.
        int             m_iBitCnt    ;  //�̹����� ��Ʈ��. (8==��� , 24==�÷�)

        int             m_iMemWidth8  ;  //8��Ʈ�̹����� 4�� ����� �������� �̹��� ����..
        int             m_iMemWidth24 ;  //24��Ʈ�̹����� 4�� ����� �������� �̹��� ����..

        float           m_fScaleX    ;  //�̹��� ������X.
        float           m_fScaleY    ;  //�̹��� ������Y.

        //�̹��� ���μ��� �� ��� ���÷��̿�.
        BITMAPINFO    * m_pBitInfo8  ;  //8��Ʈ  ���̹����� BITMAPINFO
        BITMAPINFO    * m_pBitInfo24 ;  //24��Ʈ ���̹����� BITMAPINFO
        unsigned char * m_pImage     ;  //�̹����� ó�� ���� ������ �׷��� �̹���.
        unsigned char * m_pImageC    ;  //�̹����� ó�� ���� ������ �÷� �̹���.

        //CRITICAL_SECTION g_pTemp ;

        //Display ��.
        //BITMAPINFO    * m_pBitInfoD  ;  //��ȯ ���÷��̿�. ��Ʈ������. ex)falseColor , Binary.. etc..
        //unsigned char * m_pImageD    ;  //��ȯ ���÷��̿�. R , G , B



    public : //������ �Ҹ���.
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
        
        //�������� ����� Pixel. ÷���� ����.
        virtual unsigned char __stdcall GetPixelSc (int _x , int _y )=0;
        virtual unsigned char __stdcall GetPixelScR(int _x , int _y )=0;
        virtual unsigned char __stdcall GetPixelScG(int _x , int _y )=0;
        virtual unsigned char __stdcall GetPixelScB(int _x , int _y )=0;
        
        //(0,0) , (1,0) , (0,1) , (1,1) ���ȼ��� ���� float�� ��ġ���� �Է�
        virtual unsigned char __stdcall GetSubPixel(float _x , float _y )=0;
        
        virtual unsigned char * __stdcall GetImg ()=0;
        virtual unsigned char * __stdcall GetImgC()=0;

        virtual int  __stdcall GetHeight()=0;
        virtual int  __stdcall GetWidth ()=0;
        virtual int  __stdcall GetImgBit()=0;

        virtual int  __stdcall GetMem8Width ()=0; //GetImg()�� �Ἥ �޸� ���� ���۽ÿ� 4�� ��� ���̸� �������� �Ѵ�.
        virtual int  __stdcall GetMem24Width ()=0; //GetImg()�� �Ἥ �޸� ���� ���۽ÿ� 4�� ��� ���̸� �������� �Ѵ�.




        virtual float __stdcall GetScaleX()=0;
        virtual float __stdcall GetScaleY()=0;

        virtual int __stdcall GetImgX(int _x)=0;
        virtual int __stdcall GetImgY(int _y)=0;

    public :
        virtual bool __stdcall LoadBmp (char * _FilePath)=0; //�̹����� �ε� �ϴ� �Լ�.
        virtual bool __stdcall LoadJpg (char * _FilePath)=0; //
        virtual bool __stdcall SaveBmp (char * _FilePath)=0; //�̹����� ���̺� �ϴ� �Լ�.
        virtual bool __stdcall SaveJpg (char * _FilePath)=0; //


        //�ܺο��� ������۸� �Ҷ�
        //Scale
        virtual int __stdcall ShowImg      (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgGrey  (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgFalse (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgColor (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgBinary(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH, int _iThreshold )=0;
        virtual int __stdcall ShowImgR     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgG     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;
        virtual int __stdcall ShowImgB     (HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;

        virtual bool __stdcall SetSize   (int _iWidth , int _iHeight , int _iBitCnt)=0; //������ �ٲٴ� �Լ� �ε�.

        //ȭ�鿡 �̹����� ���̷�Ʈ�� �׸���� ������۸� ���� �Լ�.
        virtual HBITMAP __stdcall GetHBitmap(HDC _hHdc)=0;
        virtual void    __stdcall ShowBitmap(HDC _hHdc , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH)=0;

        //Set Image
        virtual bool __stdcall SetImage   (unsigned char * _pImage  , int _iWidth , int _iHeight , int _iBitCnt)=0;
        virtual void __stdcall ClrImage   ()=0;
        virtual void __stdcall operator = (IImage * _pImg)=0;

        virtual void __stdcall Rotate(TRect _tRect , float _fAngle)=0; //Rect�κи� ȸ�� ��Ű��.
        virtual void __stdcall Trim     (TRect _tRect , IImage * _tDstImg)=0;
        virtual void __stdcall TrimScale(TRect _tRect , IImage * _tDstImg , float _fScale)=0;

    public:
        //�˻� ���� ���� �̹����� ũ�⿡�� ��� �������� �̹����������� ���� �ϴ� �Լ���
        virtual bool __stdcall SetRect (TRect *_pRect , int _iMargin = 0)=0;
        virtual bool __stdcall SetLine (TPoint *_pSpnt , TPoint *_pEpnt , int _iMargin = 0)=0;
        virtual bool __stdcall SetPoint (TPoint *_pPnt , int _iMargin = 0)=0;
};


#endif