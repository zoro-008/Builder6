//---------------------------------------------------------------------------

#ifndef BarslerUnitH
#define BarslerUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
#include "avifile.h"

#include "Timer.h"
#include <pylonc\\PylonC.h>
//Pylon 설치후 include 패스를 잡아 줘야 됨.(C:\Program Files\Basler\Pylon 2.3\pylonc\include)
//---------------------------------------------------------------------------
/*-------------------------------------------------------------------------------
설   명 : CLEANUP - 포인터 메모리 클리어
-------------------------------------------------------------------------------*/
#ifndef __CLEANUP_DEFINED
#define __CLEANUP_DEFINED
template<typename T>
__inline void CLEANUP(T* &p)
{
    if( p )
    {
        delete p; p = NULL;
    }
}

template<typename T>
__inline void CLEANARRAY(T* &p)
{
    if( p )
    {
        delete [] p; p = NULL;
    }
}
#endif

#define AVI_FRAME_PER_SEC 30.0
#define AVI_FRAME_MS_DELAY 1000/AVI_FRAME_PER_SEC
typedef void (__closure *PFunc)();
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            PFunc Func;

        public:
            __fastcall TRunThread(PFunc func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            __fastcall ~TRunThread()
            {
            }

            void __fastcall Execute()
            {
                Func();
            }

    };

    public:
        CRunThread(PFunc func)
        {

            TRunThread*  thread = new TRunThread(func);
        }
};
//---------------------------------------------------------------------------
double __fastcall GetTime(void)
{
//Trace("GetTime","stt");
    LARGE_INTEGER liEndCounter,liFrequency  ;
    QueryPerformanceCounter  (&liEndCounter);
    QueryPerformanceFrequency(&liFrequency );

//Trace("GetTime","end");
    return (((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000.0 );
}


//---------------------------------------------------------------------------
struct TAvi {
    CAviFile    * avi             ;
    HBITMAP       hBackBitmap     ;
    LPVOID        pBits           ;
    BITMAPINFO    bmpInfo         ;
    HDC	          hBackDC         ;
    bool          bFailed         ;
    PAINTSTRUCT   ps              ;
    };

//extern TAvi Avi ;
//---------------------------------------------------------------------------
#define NUM_BUFFERS		10         /* Number of buffers used for grabbing */
#define	IMAGE_WIDTH		656
#define IMAGE_HEIGHT     	492          //646504
#define PIXEL_BIT 24             //Color시에 24 흑백일떄 8
//---------------------------------------------------------------------------

class CBarsler : public TThread
{
private:
    struct TBarStat {
	GENAPIC_RESULT              res;                        /* Return value of pylon methods. */
        size_t                      numDevices;                 /* Number of available devices. */
        PYLON_DEVICE_HANDLE         hDev;                       /* Handle for the pylon device. */
        PYLON_STREAMGRABBER_HANDLE  hGrabber;                   /* Handle for the pylon stream grabber. */
        PYLON_WAITOBJECT_HANDLE     hWait;                      /* Handle used for waiting for a grab to be finished. */
        int32_t                     payloadSize;                /* Size of an image frame in bytes. */
        unsigned char              *buffers[NUM_BUFFERS];       /* Buffers used for grabbing. */
        PYLON_STREAMBUFFER_HANDLE   bufHandles[NUM_BUFFERS];    /* Handles for the buffers. */
        PylonGrabResult_t           grabResult;                 /* Stores the result of a grab operation. */
        int                         nGrabs;                     /* Counts the number of buffers grabbed. */
        size_t                      nStreams;                   /* The number of streams the device provides. */
        _Bool                       isAvail;                    /* Used for checking feature availability. */
        _Bool                       isReady;                    /* Used as an output parameter. */
        const char*                 triggerSelectorValue;       /* Preselect the trigger for image acquisition. */
        _Bool                       isAvailFrameStart;         /* Used for checking feature availability. */
        _Bool                       isAvailAcquisitionStart;   /* Used for checking feature availability. */
    } BarStat ;


protected:
    bool m_bGrabbing;
    bool m_bMakingAvi;

    CRITICAL_SECTION m_pImgCs;

    AnsiString m_sCamOnTime   ;



    bool m_bOpen;

    TSize AoiSize;

    CDelayTimer AviTimer ;



    BITMAPFILEHEADER ImgFileHeader ;  //    BITMAPFILEHEADER bmfh ;
    LPBITMAPINFO     ImgBitmapInfo ;  //    LPBITMAPINFO     bmif ;
    //BITMAPFILEHEADER AviFileHeader ;
    //LPBITMAPINFO     AviBitmapInfo ;

    TAvi Avi ;


    void __fastcall Execute();
    void __fastcall UpdateGrab();

    void InitImgBmpHeader();


public:
    double m_dSaveTime ;
    //Graphics::TBitmap * m_pBfBmp  ;

    void ConvertYUV422toRGB24(int width,int height , unsigned char* pSrc, unsigned char* pDst);
//    Graphics::TBitmap * m_pBmpAvi ;
    __fastcall CBarsler(void);

    __fastcall virtual ~CBarsler(void);

    void Delete();

    double m_dFramePerSec ;

    void OpenAvr();
    void CloseAvr();



    BYTE *m_pImgBuff;

    HWND m_ViewHandle ;

    bool m_bLive;

    int  m_iCountBmp;
    int  m_iThreadCnt ;

    void Open(HWND ViewHandle) ;
    //void AcquisitionStart() ;
    //void AcquisitionStop() ;
    void Close();

    void Grab();
    void TriggerSW();
    void TriggerHW();

    void SetStartTime();

    void SaveImg();

    void DisPlay(HDC _hHdc);
    void SendPaintMsg();

    bool IsMakingAvi(){return m_bMakingAvi ; }
    void MakeAvi();
    void MakeAviThread();

    void AddAvi(Graphics::TBitmap * Bitmap);
    void MakeTestAvi(int a);


    int m_iTestCntAdd  ;
    int m_iTestCntNAdd ;

};
extern CBarsler *m_BarCam;
//---------------------------------------------------------------------------
#endif
