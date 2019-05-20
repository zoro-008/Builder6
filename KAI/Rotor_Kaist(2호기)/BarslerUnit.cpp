//---------------------------------------------------------------------------

#include <vcl.h>
#include <jpeg.hpp>
#pragma hdrstop

#include "FormMain.h"

#include "BarslerUnit.h"
#include "ximage.h"

#include "UserFile.h"
#include "UserIni.h"

#pragma package(smart_init)

#define CHECK( errc ) if ( GENAPI_E_OK != errc ) printErrorAndExit( errc )

#define BMP_INFO_SIZE_8  sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD) //팔레트 가지고 있는 256컬러 인포 크기.
#define BMP_INFO_SIZE_24 sizeof(BITMAPINFO)                           //펄레트 필요 없는 24비트 인포 크기.
#define CLIP(t) (((t)>255)?255:(((t)<0)?0:(t)))//값의 범위가  0 <= t < 256 되도록 한다
//---------------------------------------------------------------------------

CBarsler *m_BarCam;

const bool bSaveAsBmp = false ;

//---------------------------------------------------------------------------
//YUV422->RGB24 변환 LookUpTable
int tab_76284[256];   // table (Y-16)*76284
int tab_cbv[256];     // table coefficient blue U =(U-128)*1332252
int tab_cgv[256];     // table coefficient green V =(V-128)*53281
int tab_cgu[256];     // table coefficient green U =(U-128)*25625
int tab_cru[256];     // table coefficient red V =(V-128)*104595
BYTE clipOri[1024];
BYTE * clip ;

void InitYUV422toRGB24Table2( void ) //소수 테이블 쓰면 졸라느림.
{
   for( int i = 0; i < 256; i++ ){
                                     tab_cbv[i] = 1.596*(i - 128);
      tab_cgu[i] =  0.391*(i - 128); tab_cgv[i] = 0.813*(i - 128);
      tab_cru[i] =  2.018*(i - 128);

      tab_76284[i] = 1.164*(i - 16);
   }

   clip = clipOri + 384;

   for( int i=-384; i<640; i++ )
      clip[i] = (i<0) ? 0 : ((i>255) ? 255 : i);
}

void InitYUV422toRGB24Table( void )
{
   for( int i = 0; i < 256; i++ ){
                                       tab_cbv[i] = (i - 128) * 104595;
      tab_cgu[i] = (i - 128) * 25625 ; tab_cgv[i] = (i - 128) * 53281 ;
      tab_cru[i] = (i - 128) * 132252;

      tab_76284[i] = 76284 * (i - 16);
   }

   clip = clipOri + 384;

   for( int i=-384; i<640; i++ )
      clip[i] = (i<0) ? 0 : ((i>255) ? 255 : i);
}

void CBarsler::ConvertYUV422toRGB24(int width,int height , unsigned char* pSrc, unsigned char* pDst)
{

    int i,j;
    unsigned char* dst=pDst;
    unsigned char* src=pSrc;
    int Y1,Y2,U,V;
    int R1,G1,B1 , R2,G2,B2;
    int r1,g1,b1 , r2,g2,b2;

    double dTime = GetTickCount() ;
    int iPxByteCnt = PIXEL_BIT / 8 ;

    for(i=0;i<width*height;i=i+2){
        Y1=src[i*2+1];
        Y2=src[i*2+3];
        U =src[i*2  ];
        V =src[i*2+2];

        B1 = clip[(tab_76284[Y1]              + tab_cbv[V] )>> 16];  
        G1 = clip[(tab_76284[Y1] - tab_cgu[U] - tab_cgv[V] )>> 16];  
        R1 = clip[(tab_76284[Y1] + tab_cru[U]              )>> 16];  
        B2 = clip[(tab_76284[Y2]              + tab_cbv[V] )>> 16];  
        G2 = clip[(tab_76284[Y2] - tab_cgu[U] - tab_cgv[V] )>> 16];  
        R2 = clip[(tab_76284[Y2] + tab_cru[U]              )>> 16];

        dst[ i*   iPxByteCnt+2]= B1;
        dst[ i*   iPxByteCnt+1]= G1;
        dst[ i*   iPxByteCnt  ]= R1;
        dst[(i+1)*iPxByteCnt+2]= B2;
        dst[(i+1)*iPxByteCnt+1]= G2;
        dst[(i+1)*iPxByteCnt  ]= R2;
    }

    dTime = GetTickCount() - dTime ;

}


__fastcall CBarsler::CBarsler(void):TThread(TRUE)
{
    m_pImgBuff = new BYTE [IMAGE_WIDTH * IMAGE_HEIGHT * PIXEL_BIT/8];
    memset(m_pImgBuff, 0, IMAGE_WIDTH*IMAGE_HEIGHT * PIXEL_BIT/8);

    m_ViewHandle = NULL;



    m_bGrabbing  = false;
    m_bMakingAvi = false;

    BarStat.triggerSelectorValue = "FrameStart";


    InitImgBmpHeader();

    m_iCountBmp = 0;
    m_bLive = false ;

    AviTimer.Clear() ;

    InitYUV422toRGB24Table();

    InitializeCriticalSection(&m_pImgCs);

    m_iThreadCnt = 0 ;
}


__fastcall CBarsler::~CBarsler(void)
{

    if(!Suspended)Suspend();
    Terminate();
    //WaitFor(); //미확인 지움. 종료시 자꾸 걸림.

    DeleteCriticalSection(&m_pImgCs);

    delete [] m_pImgBuff ;
    m_pImgBuff = NULL ;

    delete [] ImgBitmapInfo ;
    ImgBitmapInfo = NULL ;
}

void CBarsler::Delete()
{


}


//---------------------------------------------------------------------------
void printErrorAndExit( GENAPIC_RESULT errc )
{
    char *errMsg;
    size_t length;
    String sErrDetail;
    String sErrMsg ;

    /* Retrieve the error message.
    ... First find out how big the buffer must be, */
    GenApiGetLastErrorMessage( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorMessage( errMsg, &length );
    sErrMsg = errMsg ;


    //format
    //TRACE("%s (%#08x).\n", errMsg, errc);
    free( errMsg);

    /* Retrieve the more details about the error.
    ... First find out how big the buffer must be, */
    GenApiGetLastErrorDetail( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorDetail( errMsg, &length );

    sErrDetail = errMsg ;

    ShowMessage(sErrMsg + " : " + sErrDetail);

    free( errMsg);

    PylonTerminate();  /* Releases all pylon resources. */
    exit(EXIT_FAILURE);
}
//---------------------------------------------------------------------------
void __fastcall CBarsler::Execute()
{
    int iCnt = 0 ;
    while(!Terminated)
    {
        //UpdateGrab();
        //Synchronize(UpdateGrab); //잠시 수정.
        UpdateGrab();
        //FrmMain -> Msg(iCnt++);

    }
}

void __fastcall CBarsler::UpdateGrab()
{
    int bufferIndex;              /* Index of the buffer. */

    BarStat.res = PylonWaitObjectWait( BarStat.hWait, 10, &BarStat.isReady );
    CHECK(BarStat.res);
    if ( ! BarStat.isReady )
    {
        return ; // Stop grabbing.
    }


    /* Since the wait operation was successful, the result of at least one grab
       operation is available. Retrieve it. */
    BarStat.res = PylonStreamGrabberRetrieveResult( BarStat.hGrabber, &BarStat.grabResult, &BarStat.isReady );
    CHECK(BarStat.res);
    if ( ! BarStat.isReady )
    {
        /* Oops. No grab result available? We should never have reached this point.
           Since the wait operation above returned without a timeout, a grab result
           should be available. */
//        TRACE("Failed to retrieve a grab result\n");
        return ;
    }



    /* Get the buffer index from the context information. */
    bufferIndex = (int) BarStat.grabResult.Context;

    /* Check to see if the image was grabbed successfully. */
    unsigned char * pImg ;
    if ( BarStat.grabResult.Status == Grabbed )
    {
        /*  The grab is successfull.  */

        //unsigned char* buffer;        /* Pointer to the buffer attached to the grab result. */
        static int    iGrabCnt = 0 ;
        static double dGrabCntTime = GetTime();
        iGrabCnt++;
        double dGrab100Time ;
        if(iGrabCnt >=100){
            iGrabCnt = 0 ;
            dGrab100Time = GetTime() - dGrabCntTime ;
            m_dFramePerSec = 1000/(dGrab100Time / 100.0) ;
            dGrabCntTime = GetTime();
        }

        /* Get the buffer pointer from the result structure. Since we also got the buffer index,
        we could alternatively use buffers[bufferIndex]. */

        pImg = (unsigned char*) BarStat.grabResult.pBuffer;

        //EnterCriticalSection(&m_pImgCs);
        double dTemp = GetTime() ;
        if(PIXEL_BIT == 8) {
            memcpy(m_pImgBuff , pImg , IMAGE_WIDTH * IMAGE_HEIGHT) ;
        }
        else {
            ConvertYUV422toRGB24(IMAGE_WIDTH ,IMAGE_HEIGHT , pImg , m_pImgBuff);
        }
        m_dSaveTime = GetTime() - dTemp ;


        SendPaintMsg();

        //if(m_bLive) {
        //
        //    //CRunThread thread(&SaveImg);
        //}

        if(m_bLive) Grab();
    }
    else if ( BarStat.grabResult.Status == Failed )
    {
    }

    m_bGrabbing= false; 

    /* Once finished with the processing, requeue the buffer to be filled again. */
    BarStat.res = PylonStreamGrabberQueueBuffer( BarStat.hGrabber, BarStat.grabResult.hBuffer, (void*) bufferIndex );
    CHECK(BarStat.res);

    /*
    static DWORD dPreTick = GetTickCount() ;
    static DWORD dSum = 0.0 ;
    static double   iCnt = 0 ;
    DWORD dCrntTick = GetTickCount();
    dSum += dCrntTick - dPreTick ;
    dPreTick = dCrntTick ;
    iCnt++;
    AnsiString disp;
    disp.sprintf("%02f",dSum/iCnt);

    m_BarCam->Resume();*/

    return ;
}

void CBarsler::SendPaintMsg()
{
    if(m_ViewHandle)PostMessage(m_ViewHandle , WM_PAINT , 0 , 0 );
}
//---------------------------------------------------------------------------
void CBarsler::InitImgBmpHeader()
{
    const int iSizeOfBfh  = 14 ; //sizeof(BITMAPFILEHEADER) ; 빌더버그 있어서 14인데 16으로 나옴.
    const int iSizeOfPlt  = PIXEL_BIT == 8 ? sizeof(RGBQUAD) * 256 : 0 ;
    //biSizeImage = ((((bih.biWidth * m_iBitCnt) + 31) & ~31) >> 3) * bih.biHeight 귀찮아서 4바이트 단위 고려안함.
    const int iSizeOfImg  = IMAGE_WIDTH * IMAGE_HEIGHT*PIXEL_BIT/8 ;
    const int iOftToImg   = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfPlt ;
    const int iSizeOfFile = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfImg + iSizeOfPlt ;

    ImgFileHeader.bfType        = 0x4D42;
    ImgFileHeader.bfSize        = iSizeOfFile ;
    ImgFileHeader.bfReserved1   = 0;
    ImgFileHeader.bfReserved2   = 0;
    ImgFileHeader.bfOffBits     = iOftToImg ; //sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

     ImgBitmapInfo=PIXEL_BIT == 8 ? (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)+ 256*sizeof(RGBQUAD)]) :
                                   (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)                     ]) ;
     ImgBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    ImgBitmapInfo->bmiHeader.biWidth = IMAGE_WIDTH;
    ImgBitmapInfo->bmiHeader.biHeight = IMAGE_HEIGHT*(-1);
//    BitmapInfo->bmiHeader.biHeight = IMAGE_HEIGHT;
    ImgBitmapInfo->bmiHeader.biPlanes = 1;
    ImgBitmapInfo->bmiHeader.biCompression = BI_RGB;
    ImgBitmapInfo->bmiHeader.biBitCount = PIXEL_BIT;
    ImgBitmapInfo->bmiHeader.biSizeImage = IMAGE_WIDTH*IMAGE_HEIGHT*PIXEL_BIT/8;
    ImgBitmapInfo->bmiHeader.biXPelsPerMeter = 72;
    ImgBitmapInfo->bmiHeader.biYPelsPerMeter = 72;
    ImgBitmapInfo->bmiHeader.biClrUsed = 256;
    ImgBitmapInfo->bmiHeader.biClrImportant = 0;

    if(PIXEL_BIT != 8) return ;
    for(int i=0;i<256;i++)
    {
       ImgBitmapInfo->bmiColors[i].rgbRed=(UINT8)i;
       ImgBitmapInfo->bmiColors[i].rgbGreen=(UINT8)i;
       ImgBitmapInfo->bmiColors[i].rgbBlue=(UINT8)i;
       ImgBitmapInfo->bmiColors[i].rgbReserved=0;
    }
}

void CBarsler::OpenAvr()
{
    m_bMakingAvi = true ;
    //Graphics::TBitmap * pBmpAvi = new Graphics::TBitmap();
    AnsiString sPath = "d:\\Image\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;

    AnsiString strSavePath,strBmpPath;

    strSavePath = "d:\\Image\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;

    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;





    //저장되고 속도 빠르고 용량이큼.

    //strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    //Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('D','I','B',' '), AVI_FRAME_PER_SEC);

    //이건 저장이 아예 안되네.
    //strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    //Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('D','I','V','X'), AVI_FRAME_PER_SEC);


    //저장이 안됨.
    //strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    //Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('M','P','G','4'), AVI_FRAME_PER_SEC);


    //이것은 용량 작고 속도가 느리고 깍두기 생김.
    strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('I','V','5','0'), AVI_FRAME_PER_SEC);

    Avi.pBits   = NULL  ;
    Avi.bFailed = false ;
    HDC hdc     = GetDC(m_ViewHandle);
    Avi.hBackDC = CreateCompatibleDC(hdc);

    const int iAviPxBit = 24 ; //흑백이나 컬러나 무조건 3으로..
    Avi.bmpInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);                                                                            ;
    Avi.bmpInfo.bmiHeader.biWidth         = IMAGE_WIDTH ;
    Avi.bmpInfo.bmiHeader.biHeight        = IMAGE_HEIGHT ;
    Avi.bmpInfo.bmiHeader.biPlanes        = 1;
    Avi.bmpInfo.bmiHeader.biBitCount      = iAviPxBit;
    Avi.bmpInfo.bmiHeader.biCompression   = BI_RGB;
    Avi.bmpInfo.bmiHeader.biSizeImage     = IMAGE_WIDTH*IMAGE_HEIGHT*iAviPxBit/8; //무조건 알지비..
    Avi.bmpInfo.bmiHeader.biXPelsPerMeter = 72 ;
    Avi.bmpInfo.bmiHeader.biYPelsPerMeter = 72 ;
    Avi.bmpInfo.bmiHeader.biClrUsed       = 0  ;
    Avi.bmpInfo.bmiHeader.biClrImportant  = 0  ;

    //Avi.bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    //Avi.bmpInfo.bmiHeader.biBitCount=24;
    //Avi.bmpInfo.bmiHeader.biCompression=BI_RGB;
    //Avi.bmpInfo.bmiHeader.biSizeImage=IMAGE_WIDTH*IMAGE_HEIGHT*PIXEL_BIT/8;
    //Avi.bmpInfo.bmiHeader.biPlanes=1;
    //Avi.bmpInfo.bmiHeader.biHeight=IMAGE_HEIGHT;
    //Avi.bmpInfo.bmiHeader.biWidth=IMAGE_WIDTH;

    Avi.hBackBitmap    = CreateDIBSection(Avi.hBackDC,&Avi.bmpInfo,DIB_RGB_COLORS,&Avi.pBits,NULL,NULL);
    if(Avi.hBackBitmap==NULL)
    {
        MessageBox(NULL,"Unable to create bitmap","Error",MB_OK);
        return ;
    }
    ReleaseDC(m_ViewHandle,hdc);

    /*
    if(bSaveAsBmp){
        for ( int iCount = 1 ;iCount < m_iCountBmp ; iCount ++){
            strBmpPath = sPath + iCount + ".bmp";
            pBmpAvi -> LoadFromFile(strBmpPath) ;
            AddAvi(pBmpAvi);
        }
    }
    else {
        TJPEGImage *jpg = new TJPEGImage ();
        for ( int iCount = 1 ;iCount < m_iCountBmp ; iCount ++){
            strBmpPath = sPath + iCount + ".jpg";
            jpg -> LoadFromFile(strBmpPath) ;
            pBmpAvi -> Assign(jpg);
            AddAvi(pBmpAvi);
        }
        delete jpg ;
    }
    */


}

void CBarsler::CloseAvr()
{
    ReleaseDC(m_ViewHandle , Avi.hBackDC);
    delete Avi.avi;

    m_bMakingAvi = false ;

}

//---------------------------------------------------------------------------
void CBarsler::DisPlay(HDC _hHdc)
{
    //if(m_ViewHandle)PostMessage(m_ViewHandle , WM_PAINT , 0 , 0 );


    HDC hdc= _hHdc;
    //ClientWidth-1 일시 화면 중간에 검은색 세로줄 생김.
    StretchDIBits( hdc  , 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT,
     m_pImgBuff, ImgBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(m_ViewHandle ,hdc ) ;
}


//---------------------------------------------------------------------------
void CBarsler::Open(HWND ViewHandle)
{
    if(m_bOpen) return;

    m_ViewHandle = ViewHandle ;

	m_bOpen = true;


   /* Before using any pylon methods, the pylon runtime must be initialized. */
    PylonInitialize();

//    TRACE("Enumerating devices ...\n");

    /* Enumerate all camera devices. You must call
       PylonEnumerateDevices() before creating a device. */
    BarStat.res = PylonEnumerateDevices( &BarStat.numDevices );
    CHECK(BarStat.res);
    if ( 0 == BarStat.numDevices )
    {
//        TRACE("No devices found.\n" );
        /* Before exiting a program, PylonTerminate() should be called to release
           all pylon related resources. */
        ShowMessage("Camera Connection Failed [Check the Camera Cable]");
        PylonTerminate();
        return ;
        //exit(EXIT_FAILURE);
    }

//    TRACE("Opening first device ...\n");

    /* Get a handle for the first device found.  */
    BarStat.res = PylonCreateDeviceByIndex( 0, &BarStat.hDev );
    CHECK(BarStat.res);

    /* Before using the device, it must be opened. Open it for setting
       parameters and for grabbing images. */
    BarStat.res = PylonDeviceOpen( BarStat.hDev, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM );
    CHECK(BarStat.res);

    /* Print out the name of the camera we are using. */
    char buf[256];        
	size_t siz = sizeof(buf);

	BarStat.res = PylonDeviceFeatureToString( BarStat.hDev, "DeviceModelName", buf, &siz );
        CHECK(BarStat.res);
//	m_strModelName = buf;


//	if(m_strModelName.Right(1)=="c")
//	{
//		m_pView->m_BAYER_HOME = CColorConverter::poB;
//	}


	siz = sizeof(buf);
	BarStat.res = PylonDeviceFeatureToString( BarStat.hDev, "DeviceID", buf, &siz );
// 	m_strDeviceID = buf;



	AoiSize.cx = IMAGE_WIDTH;
	AoiSize.cy = IMAGE_HEIGHT;
	// Beware : Windows Bitmaps have to have a DWORD alligned width
	AoiSize.cx = AoiSize.cx & ~3;

	// Create suitable bitmaps
//	if(bColor)
//		m_ptrBitmap.Create(AoiSize, 8, CDib::TopDown, CDib::Color);
//	else
//		m_ptrBitmap.Create(AoiSize, 8, CDib::TopDown, CDib::Monochrome);




	BarStat.res = PylonDeviceSetIntegerFeature( BarStat.hDev, "Width", AoiSize.cx );
    CHECK(BarStat.res);

	BarStat.res = PylonDeviceSetIntegerFeature( BarStat.hDev, "Height", AoiSize.cy );
    CHECK(BarStat.res);

    if(PIXEL_BIT == 8) BarStat.isAvail = PylonDeviceFeatureIsAvailable(BarStat.hDev, "EnumEntry_PixelFormat_Mono8");
    else               BarStat.isAvail = PylonDeviceFeatureIsAvailable(BarStat.hDev, "EnumEntry_PixelFormat_YUV422Packed");
    if ( ! BarStat.isAvail )
    {
        //  TRACE("Device doesn't support the BayerBG8 pixel format");
        return;
    }
    if(PIXEL_BIT == 8) BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "PixelFormat", "Mono8" );
    else               BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "PixelFormat", "YUV422Packed" );
    CHECK(BarStat.res);



// software trigger	
	const char*                 triggerSelectorValue = "FrameStart"; /* Preselect the trigger for image acquisition. */
    _Bool                        isAvailFrameStart;          /* Used for checking feature availability. */
    _Bool                        isAvailAcquisitionStart;    /* Used for checking feature availability. */

    /* Check the available camera trigger mode(s) to select the appropriate one: acquisition start trigger mode (used by previous cameras;
    do not confuse with acquisition start command) or frame start trigger mode (equivalent to previous acquisition start trigger mode). */
    isAvailAcquisitionStart = PylonDeviceFeatureIsAvailable(BarStat.hDev, "EnumEntry_TriggerSelector_AcquisitionStart");
    isAvailFrameStart = PylonDeviceFeatureIsAvailable(BarStat.hDev, "EnumEntry_TriggerSelector_FrameStart");

    /* Check to see if the camera implements the acquisition start trigger mode only. */
    if (isAvailAcquisitionStart && !isAvailFrameStart)
    {
    /* ... Select the software trigger as the trigger source. */
        BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerSelector", "AcquisitionStart");
        CHECK(BarStat.res);
        BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerMode", "On");
        CHECK(BarStat.res);
        triggerSelectorValue = "AcquisitionStart";
    }
    else
    {
        /* Camera may have the acquisition start trigger mode and the frame start trigger mode implemented.
        In this case, the acquisition trigger mode must be switched off. */
        if (isAvailAcquisitionStart)
        {
            BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerSelector", "AcquisitionStart");
            CHECK(BarStat.res);
            BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerMode", "Off");
            CHECK(BarStat.res);
        }
        /* To trigger each single frame by software or external hardware trigger: Enable the frame start trigger mode. */
        BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerSelector", "FrameStart");
        CHECK(BarStat.res);
        BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerMode", "On");
        CHECK(BarStat.res);
    }

    /* Note: the trigger selector must be set to the appropriate trigger mode
    before setting the trigger source or issuing software triggers.
    Frame start trigger mode for newer cameras, acquisition start trigger mode for previous cameras. */
    PylonDeviceFeatureFromString(BarStat.hDev, "TriggerSelector", triggerSelectorValue);

    triggerSelectorValue = "OutputLine1";
    PylonDeviceFeatureFromString(BarStat.hDev, "LineSelector"   , triggerSelectorValue);

    triggerSelectorValue = "ExposureActive";
    PylonDeviceFeatureFromString(BarStat.hDev, "LineSource"     , triggerSelectorValue);

    triggerSelectorValue = "FrameStart"; /* Preselect the trigger for image acquisition. */

	/* When using software triggering, the Continuous frame mode should be used. Once
       acquisition is started, the camera sends one image each time a software trigger is 
       issued. */
//    res = PylonDeviceFeatureFromString( hDev, "AcquisitionMode", "Continuous" );
//    CHECK(res);


    /* For GigE cameras, we recommend increasing the packet size for better
       performance. When the network adapter supports jumbo frames, set the packet 
       size to a value > 1500, e.g., to 8192. In this sample, we only set the packet size
       to 1500. */
    /* ... Check first to see if the GigE camera packet size parameter is supported and if it is writable. */
    BarStat.isAvail = PylonDeviceFeatureIsWritable(BarStat.hDev, "GevSCPSPacketSize");
    if ( BarStat.isAvail )
    {
        /* ... The device supports the packet size feature. Set a value. */
        BarStat.res = PylonDeviceSetIntegerFeature( BarStat.hDev, "GevSCPSPacketSize", 576 );
        CHECK(BarStat.res);
    }


    /* Image grabbing is done using a stream grabber.
       A device may be able to provide different streams. A separate stream grabber must 
       be used for each stream. In this sample, we create a stream grabber for the default 
       stream, i.e., the first stream ( index == 0 ).
    */

    /* Get the number of streams supported by the device and the transport layer. */
    BarStat.res = PylonDeviceGetNumStreamGrabberChannels( BarStat.hDev, &BarStat.nStreams );
    CHECK(BarStat.res);
    if ( BarStat.nStreams < 1 )
    {
//        TRACE("The transport layer doesn't support image streams.\n");
        PylonTerminate();
        exit(EXIT_FAILURE);
    }

    /* Create and open a stream grabber for the first channel. */
    BarStat.res = PylonDeviceGetStreamGrabber( BarStat.hDev, 0, &BarStat.hGrabber );
    CHECK(BarStat.res);
    BarStat.res = PylonStreamGrabberOpen( BarStat.hGrabber );
    CHECK(BarStat.res);

    /* Get a handle for the stream grabber's wait object. The wait object
       allows waiting for buffers to be filled with grabbed data. */
    BarStat.res = PylonStreamGrabberGetWaitObject( BarStat.hGrabber, &BarStat.hWait );
    CHECK(BarStat.res);

    /* Determine the required size of the grab buffer. Since activating chunks will increase the
       payload size and thus the required buffer size, do this after enabling the chunks. */
    BarStat.res = PylonDeviceGetIntegerFeatureInt32( BarStat.hDev, "PayloadSize", &BarStat.payloadSize );
    CHECK(BarStat.res);

    /* Allocate memory for grabbing.  */
    for (int i = 0; i < NUM_BUFFERS; ++i )
    {
        //BarStat.buffers[i] = (unsigned char*) malloc ( BarStat.payloadSize );
        BarStat.buffers[i] = new unsigned char[BarStat.payloadSize];
        if ( NULL == BarStat.buffers[i] )
        {
            //TRACE("Out of memory.\n" );
            PylonTerminate();
            return;
        }
    }


    /* We must tell the stream grabber the number and size of the buffers 
       we are using. */
    /* .. We will not use more than NUM_BUFFERS for grabbing. */
    BarStat.res = PylonStreamGrabberSetMaxNumBuffer( BarStat.hGrabber, NUM_BUFFERS );
    CHECK(BarStat.res);
    /* .. We will not use buffers bigger than payloadSize bytes. */
    BarStat.res = PylonStreamGrabberSetMaxBufferSize( BarStat.hGrabber, BarStat.payloadSize );
    CHECK(BarStat.res);


    /*  Allocate the resources required for grabbing. After this, critical parameters
        that impact the payload size must not be changed until FinishGrab() is called. */
    BarStat.res = PylonStreamGrabberPrepareGrab( BarStat.hGrabber );
    CHECK(BarStat.res);


    /* Before using the buffers for grabbing, they must be registered at
    the stream grabber. For each registered buffer, a buffer handle
    is returned. After registering, these handles are used instead of the
    raw pointers. */
    for ( int i = 0; i < NUM_BUFFERS; ++i )
    {
        BarStat.res = PylonStreamGrabberRegisterBuffer( BarStat.hGrabber, BarStat.buffers[i], BarStat.payloadSize,  &BarStat.bufHandles[i] );
        CHECK(BarStat.res);

    }

    /* Feed the buffers into the stream grabber's input queue. For each buffer, the API 
       allows passing in a pointer to additional context information. This pointer
       will be returned unchanged when the grab is finished. In our example, we use the index of the 
       buffer as context information. */
    for ( int i = 0; i < NUM_BUFFERS; ++i )
    {
        BarStat.res = PylonStreamGrabberQueueBuffer( BarStat.hGrabber, BarStat.bufHandles[i], (void*) i );
        CHECK(BarStat.res);
    }

    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "AcquisitionStart");
    CHECK(BarStat.res);

    //ShowMessage("BarStat.res Error");


    Resume();


//    Edit1->Text = m_strDeviceID;
//    Edit2->Text = m_strModelName;

}
//---------------------------------------------------------------------------
void CBarsler::Grab(void)
{
//    BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerMode", "On");
//    CHECK(BarStat.res);
    if(m_bGrabbing) return ;
    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "TriggerSoftware");
    CHECK(BarStat.res);
    //SaveImg();
}

//마지막 카메라 클로우즈 할때 한번 하면 될듯.
//void CBarsler::AcquisitionStop()
//{
//    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "AcquisitionStop");
//    CHECK(BarStat.res);
//}

//처음 카메라 오픈 할때 한번 하면 될듯.
//void CBarsler::AcquisitionStart()
//{
//	if(m_bGrabbing || !m_bOpen) return;
//
//	  /* Issue an acquisition start command. Because the trigger mode is enabled, issuing the acquisition start command
//	   itself will not trigger any image acquisitions. Issuing the start command simply prepares the camera to acquire images.
//	   Once the camera is prepared it will acquire one image for every trigger it receives. */
//    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "AcquisitionStart");
//
//    //ShowMessage("BarStat.res Error");
//    CHECK(BarStat.res);
//
//    m_sCamOnTime = Now().DateString() + FormatDateTime("(hh-nn-ss)",Now()) ;
//
//
//
//
//}

void CBarsler::SetStartTime()
{
    m_sCamOnTime   =  FormatDateTime("yyyymmdd_hhnnss",Now()) ;
    m_iCountBmp    = 0 ;
    m_iTestCntAdd  = 0 ;
    m_iTestCntNAdd = 0 ;
}

//---------------------------------------------------------------------------

void CBarsler::SaveImg()
{
    //static bool bSaveEnd = true ;
    //
    //if(!bSaveEnd) return ;
    //bSaveEnd = false ;

    if(!m_pImgBuff) return ;
    m_iThreadCnt++;
    m_iCountBmp++;
    int iCountBmp = m_iCountBmp ;//쓰레드에서 돌기때문에 밑에 화일 이름 빼먹고 갈수 있음.
    AnsiString strSavePath ;
    strSavePath = "d:\\Image\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;
    if(!UserFile.DirExists(strSavePath)) {
        UserFile.CreateDir(strSavePath) ;

    }

    Graphics::TBitmap * bmp = new Graphics::TBitmap();

    unsigned char  *pPx  ;

    if(PIXEL_BIT == 8){
        struct s_pal{
            LOGPALETTE pal;
            PALETTEENTRY dummy[256];
        };
        s_pal new_pal;
        new_pal.pal.palVersion=0x300;
        new_pal.pal.palNumEntries=256;
        for(int i=0;i<256;i++){
            new_pal.pal.palPalEntry[i].peRed=i;
            new_pal.pal.palPalEntry[i].peGreen=i;
            new_pal.pal.palPalEntry[i].peBlue=i;
        }
        //SetPaletteEntries(bmp->Palette,0,256,Out8bppNOED.lppe);
        bmp->PixelFormat = pf8bit  ;
        bmp->Width       = IMAGE_WIDTH  ;
        bmp->Height      = IMAGE_HEIGHT ;
        bmp->Palette=CreatePalette(&new_pal.pal);
    }
    else {
        bmp->PixelFormat = pf24bit  ;
        bmp->Width       = IMAGE_WIDTH  ;
        bmp->Height      = IMAGE_HEIGHT ;
    }

    for (int y = 0; y < bmp->Height; y++){
        pPx = (unsigned char  *)bmp->ScanLine[y] ;
        memcpy(pPx , m_pImgBuff +  y * bmp->Width * PIXEL_BIT /8 , bmp->Width * PIXEL_BIT /8);
    }


    if(bSaveAsBmp){
        strSavePath = strSavePath + AnsiString(iCountBmp) + ".bmp";
        bmp -> SaveToFile(strSavePath);
    }
    else {
        strSavePath = strSavePath + AnsiString(iCountBmp) + ".jpg";
        TJPEGImage *jpg = new TJPEGImage ();
        jpg -> Assign(bmp);
        jpg -> SaveToFile(strSavePath);
        delete jpg ;
    }

    //AddAvi(bmp);

    delete bmp ;
    bmp = NULL;

    //m_iThreadCnt--;

    //bSaveEnd = true ;

    return ;

}


void CBarsler::Close()
{
  /* Clean up. */
    if(!m_bOpen) return;

    if(!Suspended)Suspend();


    /*  ... Stop the camera. */
    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "AcquisitionStop");
    CHECK(BarStat.res);

    /* ... We must issue a cancel call to ensure that all pending buffers are put into the
    stream grabber's output queue. */
    BarStat.res = PylonStreamGrabberCancelGrab( BarStat.hGrabber );
    CHECK(BarStat.res);

    /* ... The buffers can now be retrieved from the stream grabber. */
    do
    {
        BarStat.res = PylonStreamGrabberRetrieveResult( BarStat.hGrabber, &BarStat.grabResult, &BarStat.isReady );
        CHECK(BarStat.res);
    } while ( BarStat.isReady );

    /* ... When all buffers are retrieved from the stream grabber, they can be deregistered.
       After deregistering the buffers, it is safe to free the memory. */

    for (int i = 0; i < NUM_BUFFERS; ++i )   
    {
        BarStat.res = PylonStreamGrabberDeregisterBuffer( BarStat.hGrabber, BarStat.bufHandles[i] );
        CHECK(BarStat.res);
        //free (BarStat.buffers[i] );
        delete [] BarStat.buffers[i] ;
        BarStat.buffers[i]= NULL ;

    }

    /* ... Release grabbing related resources. */
    BarStat.res = PylonStreamGrabberFinishGrab( BarStat.hGrabber );
    CHECK(BarStat.res);

    /* After calling PylonStreamGrabberFinishGrab(), parameters that impact the payload size (e.g., 
       the AOI width and height parameters) are unlocked and can be modified again. */

    /* ... Close the stream grabber. */
    BarStat.res = PylonStreamGrabberClose( BarStat.hGrabber );
    CHECK(BarStat.res);



    /* ... Close and release the pylon device. The stream grabber becomes invalid
       after closing the pylon device. Don't call stream grabber related methods after
       closing or releasing the device. */
    BarStat.res = PylonDeviceClose( BarStat.hDev );
    CHECK(BarStat.res);
    BarStat.res = PylonDestroyDevice ( BarStat.hDev );
    CHECK(BarStat.res);


    /* ... Shut down the pylon runtime system. Don't call any pylon method after
       calling PylonTerminate(). */
    PylonTerminate();

//	m_ptrBitmap.Release();

}

void CBarsler::TriggerSW()
{
//-----------------------------------------------------------------------------
//소프트웨어 트리거 모드 설정

//    BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerSelector", "AcquisitionStart");
//    CHECK(BarStat.res);
//    BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerMode", "On");
//    CHECK(BarStat.res);

    /* Enable software triggering. */
    /* ... Select the software trigger as the trigger source. */
    BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "TriggerSource", "Software");
    CHECK(BarStat.res);
    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "TriggerSoftware");
    CHECK(BarStat.res);

//    BarStat.res = PylonDeviceFeatureFromString(BarStat.hDev, "TriggerSelector", "AcquisitionStart");
//    CHECK(BarStat.res);


}
void CBarsler::TriggerHW()
{
//----------------------------------------------------------------------------
//하드웨어 트리거 설정
    /* Enable software triggering. */
    /* ... Select the software trigger as the trigger source. */
    BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "TriggerSource", "Line1");
    CHECK(BarStat.res);

    BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "LineSelector", "Line1");
    CHECK(BarStat.res);

//    res = PylonDeviceSetFloatFeature( hDev, "LineDebouncerTimeAbs", 100 );
//    CHECK(res);
//----------------------------------------------------------------------------
}

void CBarsler::MakeAviThread()
{
    CRunThread thread(&MakeAvi);
}

//---------------------------------------------------------------------------
void CBarsler::MakeAvi()
{

    m_bMakingAvi = true ;
    Graphics::TBitmap * pBmpAvi = new Graphics::TBitmap();
    //m_iCountBmp = 0;
    AnsiString sPath = "d:\\Image\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;

    AnsiString strSavePath,strBmpPath;
    TDateTime   MyDateTime;
    MyDateTime = Now();

    strSavePath = "d:\\Image\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;

    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;


    //저장되고 속도 빠르고 용량이큼.
    strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('D','I','B',' '), AVI_FRAME_PER_SEC);

    //이건 저장이 아예 안되네.
    //strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    //Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('D','I','V','X'), AVI_FRAME_PER_SEC);


    //저장이 안됨.
    //strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    //Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('M','P','G','4'), AVI_FRAME_PER_SEC);


    //이것은 용량 작고 속도가 느리고 깍두기 생김.
    //strSavePath = strSavePath + FormatDateTime("yyyymmdd_hhnnss",Now()) +".avi";
    //Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('I','V','5','0'), AVI_FRAME_PER_SEC);

    Avi.pBits   = NULL  ;
    Avi.bFailed = false ;
    HDC hdc     = GetDC(m_ViewHandle);
    Avi.hBackDC = CreateCompatibleDC(hdc);

    const int iAviPxBit = 24 ; //흑백이나 컬러나 무조건 3으로..
    Avi.bmpInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);                                                                            ;
    Avi.bmpInfo.bmiHeader.biWidth         = IMAGE_WIDTH ;
    Avi.bmpInfo.bmiHeader.biHeight        = IMAGE_HEIGHT ;
    Avi.bmpInfo.bmiHeader.biPlanes        = 1;
    Avi.bmpInfo.bmiHeader.biBitCount      = iAviPxBit;
    Avi.bmpInfo.bmiHeader.biCompression   = BI_RGB;
    Avi.bmpInfo.bmiHeader.biSizeImage     = IMAGE_WIDTH*IMAGE_HEIGHT*iAviPxBit/8; //무조건 알지비..
    Avi.bmpInfo.bmiHeader.biXPelsPerMeter = 72 ;
    Avi.bmpInfo.bmiHeader.biYPelsPerMeter = 72 ;
    Avi.bmpInfo.bmiHeader.biClrUsed       = 0  ;
    Avi.bmpInfo.bmiHeader.biClrImportant  = 0  ;

    //Avi.bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    //Avi.bmpInfo.bmiHeader.biBitCount=24;
    //Avi.bmpInfo.bmiHeader.biCompression=BI_RGB;
    //Avi.bmpInfo.bmiHeader.biSizeImage=IMAGE_WIDTH*IMAGE_HEIGHT*PIXEL_BIT/8;
    //Avi.bmpInfo.bmiHeader.biPlanes=1;
    //Avi.bmpInfo.bmiHeader.biHeight=IMAGE_HEIGHT;
    //Avi.bmpInfo.bmiHeader.biWidth=IMAGE_WIDTH;

    Avi.hBackBitmap    = CreateDIBSection(Avi.hBackDC,&Avi.bmpInfo,DIB_RGB_COLORS,&Avi.pBits,NULL,NULL);
    if(Avi.hBackBitmap==NULL)
    {
        MessageBox(NULL,"Unable to create bitmap","Error",MB_OK);
        return ;
    }
    ReleaseDC(m_ViewHandle,hdc);

    if(bSaveAsBmp){
        for ( int iCount = 1 ;iCount < m_iCountBmp ; iCount ++){
            strBmpPath = sPath + iCount + ".bmp";
            pBmpAvi -> LoadFromFile(strBmpPath) ;
            AddAvi(pBmpAvi);
        }
    }
    else {
        TJPEGImage *jpg = new TJPEGImage ();
        for ( int iCount = 1 ;iCount < m_iCountBmp ; iCount ++){
            strBmpPath = sPath + iCount + ".jpg";
            jpg -> LoadFromFile(strBmpPath) ;
            pBmpAvi -> Assign(jpg);
            AddAvi(pBmpAvi);
        }
        delete jpg ;
    }
    m_iCountBmp = 0;
    ReleaseDC(m_ViewHandle , Avi.hBackDC);
    delete Avi.avi;

    delete pBmpAvi ;

    pBmpAvi = NULL ;


    m_bMakingAvi = false ;

}

//---------------------------------------------------------------------------
void CBarsler::AddAvi(Graphics::TBitmap * Bitmap)
{
    RECT rect;
    HBITMAP hBackOldBitmap=(HBITMAP)SelectObject(Avi.hBackDC,Avi.hBackBitmap);
    //SetStretchBltMode(Avi.hBackDC , HALFTONE);
    BitBlt(Avi.hBackDC,0,0,IMAGE_WIDTH,IMAGE_HEIGHT,Bitmap->Canvas->Handle,0,0,SRCCOPY);
//    BitBlt(Avi.hBackDC,0,0,ClientWidth,ClientHeight,Bitmap->Handle,0,0,SRCCOPY);
    SelectObject(Avi.hBackDC,hBackOldBitmap);
    GdiFlush();

    if(FAILED(Avi.avi->AppendNewFrame(Avi.hBackBitmap)))	//avi.AppendNewFrame(320, 240, pBits, 32)))
    {
    	Avi.bFailed = true;
    	//ShowMessage("Add Avi Error");
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void CBarsler::MakeTestAvi(int a)
{
/*    m_bMakingAvi = true ;
    AnsiString strSavePath,strBmpPath;
    TDateTime   MyDateTime;
    MyDateTime = Now();

    strSavePath = "d:\\Avi\\" ;

    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;

    strSavePath = strSavePath + FrmMain -> m_sCrntJobName+ "_" + MyDateTime.DateString() + FormatDateTime("(hh-nn-ss)",Now()) +".avi";
*/
   //static bool bMaking = true ;
    m_bMakingAvi = true ;
    Graphics::TBitmap * pBmpAvi = new Graphics::TBitmap();
    int iCountbmp  = m_iCountBmp;
    AnsiString sPath = "d:\\Test\\" ;



    AnsiString strSavePath,strBmpPath;
    TDateTime   MyDateTime;
    MyDateTime = Now();

    strSavePath = "d:\\Test\\" ;

    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;

    strSavePath = strSavePath + "sun.avi";



    //Msg("Avi Save Start");

    Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('D','I','B',' '), AVI_FRAME_PER_SEC);
    Avi.pBits   = NULL  ;
    Avi.bFailed = false ;
    HDC hdc     = GetDC(m_ViewHandle);
    Avi.hBackDC = CreateCompatibleDC(hdc);

    ZeroMemory(&Avi.bmpInfo,sizeof(Avi.bmpInfo));
    Avi.bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    Avi.bmpInfo.bmiHeader.biBitCount=32;
    Avi.bmpInfo.bmiHeader.biCompression=BI_RGB;
    Avi.bmpInfo.bmiHeader.biSizeImage=IMAGE_WIDTH*IMAGE_HEIGHT*4;
    Avi.bmpInfo.bmiHeader.biPlanes=1;
    Avi.bmpInfo.bmiHeader.biHeight=IMAGE_HEIGHT;
    Avi.bmpInfo.bmiHeader.biWidth=IMAGE_WIDTH;

    Avi.hBackBitmap    = CreateDIBSection(Avi.hBackDC,&Avi.bmpInfo,DIB_RGB_COLORS,&Avi.pBits,NULL,NULL);
    if(Avi.hBackBitmap==NULL)
    {
        MessageBox(NULL,"Unable to create bitmap","Error",MB_OK);
        return ;
    }
    ReleaseDC(m_ViewHandle,hdc);


    for ( int iCount = 1 ;iCount < a ; iCount ++)
    {
        strBmpPath = sPath + iCount + ".bmp";
        pBmpAvi -> LoadFromFile(strBmpPath) ;
        AddAvi(pBmpAvi);
    }
    ReleaseDC(m_ViewHandle , Avi.hBackDC);
    delete Avi.avi;

    delete pBmpAvi ;

    pBmpAvi = NULL ;


    m_bMakingAvi = false ;
}
