//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"

#include "BarslerUnit.h"
#include "ximage.h"

#include "UserFile.h"
#include "UserIni.h"
#include "FormMain.h"
#pragma package(smart_init)

#define CHECK( errc ) if ( GENAPI_E_OK != errc ) printErrorAndExit( errc )
//---------------------------------------------------------------------------
TAvi Avi ;
CBarsler *pGrabRun;
//---------------------------------------------------------------------------
__fastcall CBarsler::CBarsler(void):TThread(TRUE)
{
    pGrabBuf = new BYTE [IMAGE_WIDTH * IMAGE_HEIGHT];
    memset(pGrabBuf, 0, IMAGE_WIDTH*IMAGE_HEIGHT);

    m_ViewHandle = NULL;

    m_bGrabbing  = false;
    m_bMakingAvi = false;
    nGrabs = 0;

    BarStat.triggerSelectorValue = "FrameStart";
    m_pBfBmp = new Graphics::TBitmap();
//    m_pBmpAvi = new Graphics::TBitmap();

    InitImgBmpHeader();
    InitAvrBmpHeader();
//    InitAvi();
    Countbmp = 0;

    AviTimer.Clear() ;
}


__fastcall CBarsler::~CBarsler(void)
{

//    delete m_pBmpAvi;
//    m_pBmpAvi = NULL;

//    if(Suspended){
//    Resume();
//    }
//    Terminate();
//    WaitFor(); //미확인 지움. 종료시 자꾸 걸림.
//
//    delete pGrabBuf ;
//    pGrabBuf = NULL ;
//
//    delete m_pBfBmp ;
//    m_pBfBmp = NULL;

    if(Suspended)Resume();
    Terminate();
    //WaitFor(); //미확인 지움. 종료시 자꾸 걸림.

    delete pGrabBuf ;
    pGrabBuf = NULL ;

    delete m_pBfBmp ;
    m_pBfBmp = NULL;

}

void CBarsler::Delete()
{


}


//---------------------------------------------------------------------------
void printErrorAndExit( GENAPIC_RESULT errc )
{
    char *errMsg;
    size_t length;

    /* Retrieve the error message.
    ... First find out how big the buffer must be, */
    GenApiGetLastErrorMessage( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorMessage( errMsg, &length );

    //format
    //TRACE("%s (%#08x).\n", errMsg, errc);
    //free( errMsg);

    ///* Retrieve the more details about the error.
    //... First find out how big the buffer must be, */
    //GenApiGetLastErrorDetail( NULL, &length );
    //errMsg = (char*) malloc( length );
    ///* ... and retrieve the message. */
    //GenApiGetLastErrorDetail( errMsg, &length );
    //
    //TRACE("%s\n", errMsg);
    //free( errMsg);

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
//    Application->ProcessMessages();
    int bufferIndex;              /* Index of the buffer. */

//    BarStat.res = PylonWaitObjectWait( BarStat.hWait, INFINITE, &BarStat.isReady );
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


    BarStat.nGrabs++;


    /* Get the buffer index from the context information. */
    bufferIndex = (int) BarStat.grabResult.Context;

    /* Check to see if the image was grabbed successfully. */
    if ( BarStat.grabResult.Status == Grabbed )
    {
        /*  The grab is successfull.  */

        //unsigned char* buffer;        /* Pointer to the buffer attached to the grab result. */

        /* Get the buffer pointer from the result structure. Since we also got the buffer index,
        we could alternatively use buffers[bufferIndex]. */
        pGrabBuf = (unsigned char*) BarStat.grabResult.pBuffer;

        //double dTime = GetTickTime();
        //FrmMain -> Msg(dTime);
        //static int iPreCnt  = 0 ;
        //FrmMain -> Msg(AVI_FRAME_MS_DELAY);
        //int iCnt = dTime / AVI_FRAME_MS_DELAY ;
        //FrmMain -> Msg(iCnt);
        //if(iCnt != iPreCnt) {
            CRunThread<Func> * func = new CRunThread<Func>(&SaveBmp) ;
        //}
        //iPreCnt = iCnt ;

        HDC hdc= GetDC(m_ViewHandle);
        //ClientWidth-1 일시 화면 중간에 검은색 세로줄 생김.
        StretchDIBits( hdc  , 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT,
         pGrabBuf, ImgBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        ReleaseDC(m_ViewHandle ,hdc ) ;
    }
    else if ( BarStat.grabResult.Status == Failed )
    {
    }

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

    pGrabRun->Resume();*/

    return ;





}

//---------------------------------------------------------------------------
void CBarsler::DisPlay()
{
    HDC hdc= GetDC(m_ViewHandle);
    //ClientWidth-1 일시 화면 중간에 검은색 세로줄 생김.
    StretchDIBits( hdc  , 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT,
     pGrabBuf, ImgBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(m_ViewHandle ,hdc ) ;
}
//---------------------------------------------------------------------------
void CBarsler::InitAvrBmpHeader()
{
    AviFileHeader.bfType =  0x4d42;    // 'BM'
    AviFileHeader.bfSize = (sizeof(BITMAPFILEHEADER) - 2) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + (IMAGE_WIDTH * IMAGE_HEIGHT);               //360x240
    AviFileHeader.bfReserved1 = 0;
    AviFileHeader.bfReserved2 = 0;

    AviFileHeader.bfOffBits = (sizeof(BITMAPFILEHEADER) - 2) + sizeof(BITMAPINFOHEADER)+ 256*sizeof(RGBQUAD) ;

    AviBitmapInfo = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)+ 256*sizeof(RGBQUAD)]);
    AviBitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    AviBitmapInfo->bmiHeader.biWidth=IMAGE_WIDTH;
    AviBitmapInfo->bmiHeader.biHeight=IMAGE_HEIGHT;
    AviBitmapInfo->bmiHeader.biPlanes=1;
    AviBitmapInfo->bmiHeader.biCompression = BI_RGB;
    AviBitmapInfo->bmiHeader.biBitCount=8;
    AviBitmapInfo->bmiHeader.biCompression=0;
    AviBitmapInfo->bmiHeader.biSizeImage=IMAGE_WIDTH*IMAGE_HEIGHT;
    AviBitmapInfo->bmiHeader.biXPelsPerMeter=72;
    AviBitmapInfo->bmiHeader.biYPelsPerMeter=72;
    AviBitmapInfo->bmiHeader.biClrUsed=0;
    AviBitmapInfo->bmiHeader.biClrImportant=0;

    for(int i=0;i<256;i++) {
       AviBitmapInfo->bmiColors[i].rgbRed=(UINT8)i;
       AviBitmapInfo->bmiColors[i].rgbGreen=(UINT8)i;
       AviBitmapInfo->bmiColors[i].rgbBlue=(UINT8)i;
       AviBitmapInfo->bmiColors[i].rgbReserved=0;
    }
}
//---------------------------------------------------------------------------
void CBarsler::InitImgBmpHeader()
{
    ImgFileHeader.bfType        = 0x4D42;
    ImgFileHeader.bfSize        = sizeof(BITMAPFILEHEADER);
    ImgFileHeader.bfReserved1    = 0;
    ImgFileHeader.bfReserved2    = 0;
    ImgFileHeader.bfOffBits        = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    ImgBitmapInfo=(BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)+ 256*sizeof(RGBQUAD)]);
    ImgBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    ImgBitmapInfo->bmiHeader.biWidth = IMAGE_WIDTH;
    ImgBitmapInfo->bmiHeader.biHeight = IMAGE_HEIGHT*(-1);
//    BitmapInfo->bmiHeader.biHeight = IMAGE_HEIGHT;
    ImgBitmapInfo->bmiHeader.biPlanes = 1;
    ImgBitmapInfo->bmiHeader.biCompression = BI_RGB;
    ImgBitmapInfo->bmiHeader.biBitCount = 8;
    ImgBitmapInfo->bmiHeader.biSizeImage = IMAGE_WIDTH*IMAGE_HEIGHT*1;
    ImgBitmapInfo->bmiHeader.biXPelsPerMeter = 72;
    ImgBitmapInfo->bmiHeader.biYPelsPerMeter = 72;
    ImgBitmapInfo->bmiHeader.biClrUsed = 256;
    ImgBitmapInfo->bmiHeader.biClrImportant = 0;

    for(int i=0;i<256;i++)
    {
       ImgBitmapInfo->bmiColors[i].rgbRed=(UINT8)i;
       ImgBitmapInfo->bmiColors[i].rgbGreen=(UINT8)i;
       ImgBitmapInfo->bmiColors[i].rgbBlue=(UINT8)i;
       ImgBitmapInfo->bmiColors[i].rgbReserved=0;
    }

}
//---------------------------------------------------------------------------
void CBarsler::Open(HWND ViewHandle)
{
	if(m_bOpen) return;

    bool bColor = false ;
    m_ViewHandle = ViewHandle ;

	m_bOpen = true;

//	m_pView = pView;


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

	if(bColor)
	{
		BarStat.isAvail = PylonDeviceFeatureIsAvailable(BarStat.hDev, "EnumEntry_PixelFormat_BayerBG8");
		if ( ! BarStat.isAvail )
		{
			/* Feature is not available. */
//			TRACE("Device doesn't support the BayerBG8 pixel format");
			return;
		}
		/* ... Set the pixel data format to BayerBG8. */
		BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "PixelFormat", "BayerBG8" );
		CHECK(BarStat.res);

	}
	else
	{
		/* Set the pixel format to Mono8, where gray values will be output as 8 bit values for each pixel. */
		/* ... Check first to see if the device supports the Mono8 pixel data format. */
		BarStat.isAvail = PylonDeviceFeatureIsAvailable(BarStat.hDev, "EnumEntry_PixelFormat_Mono8");
		if ( ! BarStat.isAvail )
		{
			/* Feature is not available. */
//			TRACE("Device doesn't support the Mono8 pixel format");
			return;
		}
		/* ... Set the pixel data format to Mono8. */
		BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "PixelFormat", "Mono8" );
		CHECK(BarStat.res);
	}



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
        BarStat.buffers[i] = (unsigned char*) malloc ( BarStat.payloadSize );
        if ( NULL == BarStat.buffers[i] )
        {
//            TRACE("Out of memory.\n" );
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

//    Edit1->Text = m_strDeviceID;
//    Edit2->Text = m_strModelName;

}
//---------------------------------------------------------------------------
void CBarsler::Grab(void)
{

}

void CBarsler::StopGrab()
{
	if(!m_bGrabbing) return;

	m_bGrabbing = FALSE;

    /*  ... Stop the camera. */
    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "AcquisitionStop");
    CHECK(BarStat.res);

    Suspend();
}

void CBarsler::ContinuousShot()
{
	if(m_bGrabbing || !m_bOpen) return;

	  /* Issue an acquisition start command. Because the trigger mode is enabled, issuing the acquisition start command
	   itself will not trigger any image acquisitions. Issuing the start command simply prepares the camera to acquire images.
	   Once the camera is prepared it will acquire one image for every trigger it receives. */
    BarStat.res = PylonDeviceExecuteCommandFeature( BarStat.hDev, "AcquisitionStart");

    //ShowMessage("BarStat.res Error");
    CHECK(BarStat.res);

	m_bGrabbing = TRUE;

    m_sCamOnTime = Now().DateString() + FormatDateTime("(hh-nn-ss)",Now()) ;

    Resume();

}

//---------------------------------------------------------------------------
void CBarsler::SaveBmp()
{
    Countbmp++;
    AnsiString strSavePath ;
    strSavePath = "d:\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;
    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;
    strSavePath = strSavePath + AnsiString(Countbmp) + ".bmp";

    CxImage img;
    img.CreateFromArray(pGrabBuf,IMAGE_WIDTH,IMAGE_HEIGHT,8,IMAGE_WIDTH,true);
    img.Save(strSavePath.c_str(),CXIMAGE_FORMAT_BMP);
}

/*void CBarsler::ImgLoadBmp()
{
//    if(pGrabRun->Suspended)
//    {
        if( FrmMain->OpenPictureDialog1->Execute() == false )
        {
          return ;
        }
         Graphics::TBitmap * pOpenBmp = new Graphics::TBitmap();     // 이거 나중에 delete해야 함.
         pOpenBmp->LoadFromFile(FrmMain->OpenPictureDialog1->FileName);

         int ImgW = IMAGE_WIDTH;
         int ImgH = IMAGE_HEIGHT;
         int ImgBand = 1;    // 24비트 칼라이미지면 3,   8bit 흑백 이미지면 1

         for(int iy=0; iy<ImgH; iy++)
         {
//              Byte *ptr = (Byte *)pOpenBmp->ScanLine[ImgH - iy - 1];
            Byte *ptr = (Byte *)pOpenBmp->ScanLine[iy];
              for (int ix = 0; ix<ImgW*ImgBand; ix++)
              {
                pGrabBuf[iy*ImgBand*ImgW+ix] = ptr[ix];
              }
         }

         HDC hdc= GetDC(FrmMain->pnView->Handle);
        //ClientWidth-1 일시 화면 중간에 검은색 세로줄 생김.
        StretchDIBits( hdc  , 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT,
         pGrabBuf, ImgBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        ReleaseDC(FrmMain->pnView->Handle ,hdc ) ;
        delete pOpenBmp;
        //        CamImage->Picture->LoadFromFile(OpenPictureDialog1->FileName);
//    }
//    else
//    {
//        ShowMessage("Live Stop");
//    }
}
//---------------------------------------------------------------------------
void CBarsler::ImgSaveBmp()
{
    if( FrmMain->SavePictureDialog1->Execute() == false )
    {
      return ;
    }

    CxImage img;
    img.CreateFromArray(pGrabBuf,IMAGE_WIDTH,IMAGE_HEIGHT,8,IMAGE_WIDTH,true);
    AnsiString SaveBmpName;
    SaveBmpName = FrmMain-> SavePictureDialog1 -> FileName ;
    img.Save(SaveBmpName.c_str(),
    CXIMAGE_FORMAT_BMP);
}     */
//---------------------------------------------------------------------------

void CBarsler::Close()
{
  /* Clean up. */
    if(!m_bOpen) return;
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
        free( BarStat.buffers[i] );
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


    /* Enable software triggering. */
    /* ... Select the software trigger as the trigger source. */
    BarStat.res = PylonDeviceFeatureFromString( BarStat.hDev, "TriggerSource", "Software");
    CHECK(BarStat.res);

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
    CRunThread<Func> * func = new CRunThread<Func>(&MakeAvi) ;
}

//---------------------------------------------------------------------------
void CBarsler::MakeAvi()
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
    int iCountbmp  = Countbmp;
    Countbmp = 0;
    AnsiString sPath = "d:\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;



    AnsiString strSavePath,strBmpPath;
    TDateTime   MyDateTime;
    MyDateTime = Now();

    strSavePath = "d:\\" + FrmMain -> m_sCrntJobName + "\\" + m_sCamOnTime + "\\" ;

    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;

    strSavePath = strSavePath + MyDateTime.DateString() + FormatDateTime("(hh-nn-ss)",Now()) +".avi";



    //Msg("Avi Save Start");

    Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('x','v','i','d'), AVI_FRAME_PER_SEC);
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


    for ( int iCount = 1 ;iCount < iCountbmp ; iCount ++)
    {
        if(FrmMain->m_bStop) iCount = iCountbmp;
        if(FileExists(strBmpPath))
        {
            strBmpPath = sPath + iCount + ".bmp";
            pBmpAvi -> LoadFromFile(strBmpPath) ;
            AddAvi(pBmpAvi);
        }
    }
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
    int iCountbmp  = Countbmp;
    Countbmp = 0;
    AnsiString sPath = "d:\\Test\\" ;



    AnsiString strSavePath,strBmpPath;
    TDateTime   MyDateTime;
    MyDateTime = Now();

    strSavePath = "d:\\Test\\" ;

    if(!UserFile.DirExists(strSavePath)) UserFile.CreateDir(strSavePath) ;

    strSavePath = strSavePath + "sun.avi";



    //Msg("Avi Save Start");

    Avi.avi     = new CAviFile (strSavePath.c_str(), mmioFOURCC('D','I','B',' '), AVI_FRAME_PER_SEC);
    //xvid 시에 메모리 즞나 잡아 먹음. 현재 삼성종기원 2기가 장착 32비트 .어플리케이션에 1기가 할당되어 있음. 대략 3천 6백개 정도 저장 1기가시.
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
