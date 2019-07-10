#ifndef		__NEPTUNEC_H__
#define		__NEPTUNEC_H__

#pragma once

#include "NeptuneDef.h"

#if defined(_WINDOWS)
	#if defined(_NEPTUNE_EXPORTS)
		#define NEPTUNE_C_API __declspec(dllexport)
	#else
		#define NEPTUNE_C_API __declspec(dllimport)
		
		#if (_MSC_VER >= 1600 )
			#pragma comment(lib, "NeptuneC_MD_VC100.lib")
		#elif (_MSC_VER == 1500 )
			#pragma comment(lib, "NeptuneC_MD_VC90.lib")
		#elif (_MSC_VER == 1400 )
			#pragma comment(lib, "NeptuneC_MD_VC80.lib")
		#endif
	#endif

	#define	API_CALLTYPE		__stdcall
#endif

#ifdef	__cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////////////
//
//							Initialize Functions
//
////////////////////////////////////////////////////////////////////////////////////////
/* 
	Description :
		initialize NeptuneC library
	Parameters :
		None
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcInit();

/* 
	Description :
		clear NeptuneC library
	Parameters :
		None
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcUninit();


////////////////////////////////////////////////////////////////////////////////////////
//
//							Camera Information
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get number of cameras connected to the system
	Parameters :
		[OUT] pnNumbers : number of cameras
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraCount(_uint32_t* pnNumbers);

/* 
	Description :
		get information of connected cameras
	Parameters :
		[OUT] pInfo : camera information
		[IN] nLength : number of NEPTUNE_CAM_INFO structure
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraInfo(PNEPTUNE_CAM_INFO pInfo, _uint32_t nLength);

/* 
	Description :
		set callback function called when number of device changed
	Parameters :
		[IN] DeviceCallback : callback function
		[IN] pContext : user parameter passed to the callback function
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDeviceCheckCallback(NeptuneCDevCheckCallback DeviceCallback, void* pContext);


////////////////////////////////////////////////////////////////////////////////////////
//
//							Camera Control
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		create camera handle to control. All other function needs camera handle
	Parameters :
		[IN] pstrDevID : strCamID of camera info
		[OUT] phCamHandle : created camera handle 
		[IN] eAccessFlag : camera control level
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcOpen(_char_t* pstrDevID, NeptuneCamHandle* phCamHandle, ENeptuneDevAccess eAccessFlag=NEPTUNE_DEV_ACCESS_EXCLUSIVE);

/*
	Description :
		delete camera handle
	Parameters :
		[IN] hCamHandle : camera handle to delete
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcClose(NeptuneCamHandle hCamHandle);

/*
	Description :
		get the type(1394 or GigE) of camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peType : camera type
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraType(NeptuneCamHandle hCamHandle, ENeptuneDevType* peType);

/*
	Description :
		set the heartbeat time of GigE camera(GigE camera only)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nTime : heartbeat time in mili-second unit
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetHeartbeatTime(NeptuneCamHandle hCamHandle, _ulong32_t nTime);

/*
	Description :
		register callback function which is called when selected camera is plugged off
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] UnplugCallback : callback function
		[IN] pContext : user parameter passed to the callback function
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUnplugCallback(NeptuneCamHandle hCamHandle, NeptuneCUnplugCallback UnplugCallback, void* pContext);

/*
	Description :
		show the camera control dialog
	Parameters :
		[IN] hCamHandle : camera handle to control
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcShowControlDialog(NeptuneCamHandle hCamHandle);


////////////////////////////////////////////////////////////////////////////////////////
//
//							Image Information & Settings
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get the list of pixel format supported by a camera. 
		The size of supported pixel format is returned when called ntcGetPixelFormatList(hCamHandle, NULL, &nSize).
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peList : list of pixel format
		[IN/OUT] pnSize : size of the list
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPixelFormatList(NeptuneCamHandle hCamHandle, ENeptunePixelFormat* peList, _uint32_t* pnSize);

/*
	Description :
		get the current pixel format of a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peFormat : current pixel format value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPixelFormat(NeptuneCamHandle hCamHandle, ENeptunePixelFormat* peFormat);

/*
	Description :
		get the current pixel format of a camera as a string
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eFormat : current pixel format enumeration value
		[OUT] pStr : pixel format string value
		[IN] nSize : size of the string
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPixelFormatString(NeptuneCamHandle hCamHandle, const ENeptunePixelFormat eFormat, _char_t* pStr, _uint32_t nSize);

/*
	Description :
		set pixel format to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eFormat : pixel format to set to a camera
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPixelFormat(NeptuneCamHandle hCamHandle, const ENeptunePixelFormat eFormat);	

/*
	Description :
		get data bits per image pixel
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pnBits : data bits per image pixel
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBitsPerPixel(NeptuneCamHandle hCamHandle, _uint32_t* pnBits);

/*
	Description :
		get frame rate list supported by a camera.
		The size of supported pixel format is returned when called ntcGetFrameRateList(hCamHandle, NULL, &nSize).
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peList : list of frame rate supported by a camera
		[IN/OUT] pnSize : size of the list
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameRateList(NeptuneCamHandle hCamHandle, ENeptuneFrameRate* peList, _uint32_t* pnSize);

/*
	Description :
		get current frame rate enumeration value of a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peRate : current frame rate value
		[OUT] pfValue : valid float value if the peRate is "FPS_VALUE" for GigE camera
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameRate(NeptuneCamHandle hCamHandle, ENeptuneFrameRate* peRate, _double_t* pfValue);

/*
	Description :
		get frame rate value as a string
	Parameters :
		[IN] hCamHanle : camera handle to control
		[IN] eRate : frame rate enumeration value
		[OUT] pStr : frame rate string value converted
		[IN] nSize ; size of the string
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameRateString(NeptuneCamHandle hCamHandle, const ENeptuneFrameRate eRate, _char_t* pStr, _uint32_t nSize);

/*
	Description :
		set frame rate to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eRate : frame rate to set to a camera
		[IN] fValue : frame rate float value valid only if eRate is "FPS_VALUE" for GigE camera
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameRate(NeptuneCamHandle hCamHandle, ENeptuneFrameRate eRate, _double_t fValue);

/*
	Description :
		get the current size information of an image
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pImageSize : image size information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetImageSize(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE_SIZE pImageSize);

/*
	Description :
		get the maximum image size of the assigned pixel format
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pImageSize : max image size information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetMaxImageSize(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE_SIZE pImageSize);

/*
	Description :
		set the image size. 
		Valid only if pixel format is Format7 for 1394 camera or GigE camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ImageSize : image size information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetImageSize(NeptuneCamHandle hCamHandle, NEPTUNE_IMAGE_SIZE ImageSize);

/*
	Description :
		get current bayer conversion information
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peMethod : bayer convert information(None or conversion method)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBayerConvert(NeptuneCamHandle hCamHandle, ENeptuneBayerMethod* peMethod);

/*
	Description :
		set bayer convert control
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] eMethod : bayer convert information(None or conversion method)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBayerConvert(NeptuneCamHandle hCamHandle, ENeptuneBayerMethod eMethod);

/*
	Description :
		get bayer pattern layout information
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pePattern : bayer pattern layout
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBayerLayout(NeptuneCamHandle hCamHandle, ENeptuneBayerLayout* peLayout);

/*
	Description :
		set bayer patter layout control
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] ePattern : bayer pattern layout
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBayerLayout(NeptuneCamHandle hCamHandle, ENeptuneBayerLayout ePattern);

/*
	Description :
		get the byte per packet information(for 1394 camera Format7 only)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pnBpp : byte per packet value
		[OUT, optional] pnMin : minimum byte per packet value supported
		[OUT, optional] pnMax : maximum byte per packet value supported
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBytePerPacket(NeptuneCamHandle hCamHandle, _uint32_t* pnBpp, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);

/*
	Description :
		set the byte per packet to a camera(for 1394 camera Format7 only)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nBpp : byte per packet value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBytePerPacket(NeptuneCamHandle hCamHandle, _uint32_t nBpp);


/*
	Description :
		get the current transfer packet size of GigE camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pPacketSize : packet size value
		[OUT, optional] pnMin : minimum packet size value supported
		[OUT, optional] pnMax : maximum packet size value supported
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPacketSize(NeptuneCamHandle hCamHandle, _uint32_t* pnPacketSize, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);


/*
	Description :
		set the transfer packet size of GigE camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nPacketSize : size of the packet
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPacketSize(NeptuneCamHandle hCamHandle, _uint32_t nPacketSize);


////////////////////////////////////////////////////////////////////////////////////////
//
//							Data Acquisition
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get the current acquisition mode of a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] peMode : current acquisition mode
		[OUT] pnFrames : frame number(valid only if the peMode = NEPTUNE_ACQ_MULTIFRAME)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAcquisitionMode(NeptuneCamHandle hCamHandle, ENeptuneAcquisitionMode* peMode, _uint32_t* pnFrames);

/*
	Description :
		set the acquisition mode to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eMode : acquisition mode
		[IN, optional] nFrames : multi frame image count(valid only if the eMode = NEPTUNE_ACQ_MULTIFRAME)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAcquisitionMode(NeptuneCamHandle hCamHandle, ENeptuneAcquisitionMode eMode, _uint32_t nFrames=2);

/*
	Description :
		control camera to transfer a single image frame.
		Acquisition mode should be set as NEPTUNE_ACQ_SINGLEFRAME
	Parameters :
		[IN] hCamHandle : camera handle to control
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcOneShot(NeptuneCamHandle hCamHandle);

/*
	Description :
		control camera to transfer multiple image frames.
		Acquisition mode should be set as NEPTUNE_ACQ_MULTIFRAME
	Parameters :
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcMultiShot(NeptuneCamHandle hCamHandle);

/*
	Description :
		set the number of image buffer of an NeptuneC API
		Default buffer count is 10.
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nCount : number of buffer
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBufferCount(NeptuneCamHandle hCamHandle, _uint32_t nCount);


/*
	Description
		get the size of one frame image data
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pnSize : size of an image data
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBufferSize(NeptuneCamHandle hCamHandle, _uint32_t* pnSize);


/*
	Description :
		make API to use user buffer for receiving image data
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pBuffer : data buffer pointer
		[IN] nSize : size of one frame
		[IN] nCount : number of buffers
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUserBuffer(NeptuneCamHandle hCamHandle, _void_t* pBuffer, _uint32_t nSize, _uint32_t nCount);


/*
	Description :
		set image effect(flip, mirror, negative).
		Effect is applied only in display and RGB image data.
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nEffect : image effect(combination is possible)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetEffect(NeptuneCamHandle hCamHandle, _int32_t nEffect);

/*
	Description :
		grab single image from a camera.
		return when a frame data is received and wait if no frame until nTimeOut value(in mili-second)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN/OUT] pImage : image buffer and information
		[IN] eGrabFormat : grab data format(raw or RGB)
		[IN, optional] nTimieout : timeout value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGrab(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE pImage, ENeptuneGrabFormat eGrabFormat, _uint32_t nTimeOut=1000);

/*
	Description :
		register callback function called when a new frame data is received
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] callback : callback function
		[IN] pContext : user parameter passed to the callback function
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameCallback(NeptuneCamHandle hCamHandle, NeptuneCFrameCallback callback, void* pContext=NULL);

/*
	Description :
		register callback function called when a frame data is dropped
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] callback : callback function
		[IN] pContext : user parameter passed to the callback function
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameDropCallback(NeptuneCamHandle hCamHandle, NeptuneCFrameDropCallback callback, void* pContext=NULL);

/*
	Description :
		set display window to draw image
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] hWnd : window handle to display an image
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDisplay(NeptuneCamHandle hCamHandle, HWND hWnd);

/*
	Description :
		set camera play or stop
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eState : play/stop control
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAcquisition(NeptuneCamHandle hCamHandle, ENeptuneBoolean eState);

/*
	Description :
		get current play status
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peState : play/stop state
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAcquisition(NeptuneCamHandle hCamHandle, ENeptuneBoolean* peState);

/* 
	Description :
		set multicast address
	Parameter :
		[IN] hCamHandle : camera handle to control
		[IN] pstrAddress : multicast IP address
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetMulticastAddress(NeptuneCamHandle hCamHandle, _char_t* pstrAddress);
	

/*
	Description :
		get the real image data transfer rate
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pfRate : receive frame rate
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetReceiveFrameRate(NeptuneCamHandle hCamHandle, _float32_t* pfRate);

/*
	Description :
		get the RGB image data, should be called in the FrameCallback function
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pBuffer : RGB image data
		[IN] nSize : size of the buffer
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetRGBData(NeptuneCamHandle hCamHandle, _uchar_t* pBuffer, _uint32_t nSize);

/*
	Description :
		get the RGB32 image data, should be called in the FrameCallback function
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pBuffer : RGB32 image data
		[IN] nSize : size of the buffer
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetRGB32Data(NeptuneCamHandle hCamHandle, _uchar_t* pBuffer, _uint32_t nSize);

/*
	Description :
		save snapshot to an image(jpg, tiff, bmp)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] strFileName : image file name(image format is identified by the extension of the filename)
		[IN, optional] nQuality : jpeg image quality(valid only when save to JPEG)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSaveImage(NeptuneCamHandle hCamHandle, _char_t* strFileName, _uint32_t nQuality=80);

/*
	Description :
		start AVI stream capture
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] strFileName : filename to save an AVI
		[IN] bCompress : compression flag(H264)
		[IN, optional] nBitrate : bitrate value for H264(valid only if the bCompress is true)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcStartStreamCapture(NeptuneCamHandle hCamHandle, _char_t* strFileName, ENeptuneBoolean eCompress, _uint32_t nBitrate=1000);

/*
	Description :
		stop AVI stream capture
	Parameters :
		[IN] hCamHandle : camera handle to control
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcStopStreamCapture(NeptuneCamHandle hCamHandle);

/*
	Description :
		Enable/Disable receiving dropped frame data
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ENeptuneBoolean : Enable/Disable flag
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetRecvDroppedFrame(NeptuneCamHandle hCamHandle, ENeptuneBoolean eRecvDroppedFrame);


/*
	Description :
		Enable/Disable packet resend control
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ENeptuneBoolean : Enable/Disable flag
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPacketResend(NeptuneCamHandle hCamHandle, ENeptuneBoolean eEnableResend);

/*
	Description :
		Set image rotation angle 
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ENeptuneRotationAngle : rotation angle
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetRotation(NeptuneCamHandle hCamHandle, ENeptuneRotationAngle eRotationAngle);


////////////////////////////////////////////////////////////////////////////////////////
//
//							Feature Control
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get information of an selected feature
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eFeature : feature 
		[OUT] pInfo : feature information structure
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFeature(NeptuneCamHandle hCamHandle, ENeptuneFeature eFeature, PNEPTUNE_FEATURE pInfo);

/*
	Description :
		set feature value and auto control if supported
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eFeature : feature
		[IN] Info : feature information structure
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFeature(NeptuneCamHandle hCamHandle, ENeptuneFeature eFeature, NEPTUNE_FEATURE Info);

/*
	Description :
		set shutter value with a string like as "100 ms"
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrShutter : shutter value string
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetShutterString(NeptuneCamHandle hCamHandle, _char_t* pstrShutter);


// Auto Area Control
/*
	Description :
		get information of area for auto function(AutoExposure, AutoWhiteBalance, AutoFocus)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eSelect : auto control feature to get information
		[OUT] pArea : auto area information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAutoAreaControl(NeptuneCamHandle hCamHandle, ENeptuneAutoAreaSelect eSelect, PNEPTUNE_AUTOAREA pArea);


/*
	Description :
		set information of area for auto function(AutoExposure, AutoWhiteBalance, AutoFocus)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eSelect : auto control feature to get information
		[IN] pArea : auto area information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoAreaControl(NeptuneCamHandle hCamHandle, ENeptuneAutoAreaSelect eSelect, NEPTUNE_AUTOAREA Area);


/*
	Description :
		set Auto focus working mode
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eControlMode : auto focus control mode
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAFControl(NeptuneCamHandle hCamHandle, ENeptuneAFMode eControlMode);


/*
	Description :
		save camera parameter to a file
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrFileName : file name to save camera parameter
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSaveCameraParameter(NeptuneCamHandle hCamHandle, _char_t* pstrFileName);


/*
	Description :
		load camera parameter from a file
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrFileName : file name from which to load camera parameter
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcLoadCameraParameter(NeptuneCamHandle hCamHandle, _char_t* pstrFileName);


/*
	Description : 
	    get value to shutter, gamma, sharpness, contrast, black level, gain (Only GigE Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pPackageFeature : Package feature Values
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPackageFeature(NeptuneCamHandle hCamHandle, PNEPTUNE_PACKAGE_FEATURE pPackageFeature);

/*
	Description : 
	    set value to shutter, gamma, sharpness, contrast, black level, gain at the same time (Only GigE Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] PackageFeature : Package feature Values
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPackageFeature(NeptuneCamHandle hCamHandle, NEPTUNE_PACKAGE_FEATURE PackageFeature);

/*
	Description : 
	    set value to NTSC or PAL (Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eVideoMode : mode
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNTSCPALSelector(NeptuneCamHandle hCamHandle, ENeptuneVideoMode eVideoMode);   

/*
	Description : 
	    get value to NTSC or PAL (Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] peVideoMode : mode
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNTSCPALSelector(NeptuneCamHandle hCamHandle, ENeptuneVideoMode* peVideMode);

/*
	Description : 
	    set value to "25or30" or "50or60" fps(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eFPS : FPS
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAcquisitionFrameRateSel(NeptuneCamHandle hCamHandle, ENeptuneAcqFPS eFPS);

/*
	Description : 
	    get value to "25or30" or "50or60" fps(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peFPS : FPS
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAcquisitionFrameRateSel(NeptuneCamHandle hCamHandle, ENeptuneAcqFPS* peFPS);

/*
	Description : 
	    get fps list to "25or30" or "50or60" fps(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peList : FPS list
		[IN OUT] pnSize : list size
*/
NEPTUNE_C_API  ENeptuneError API_CALLTYPE ntcGetAcquisitionFrameRateList(NeptuneCamHandle hCamHandle, ENeptuneAcqFPS* peList, _uint32_t* pnSize);

/*
	Description : 
	    set value to "1920x1080" or "1280x720" resolution(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] eResolution : Resolution
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetResolutionSelector(NeptuneCamHandle hCamHandle, ENeptuneResolution eResolution);

/*
	Description : 
	    get value to "1920x1080" or "1280x720" resolution(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peResolution : Resolution
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetResolutionSelector(NeptuneCamHandle hCamHandle, ENeptuneResolution* peResolution);

/*
	Description : 
	    get resolution list to "1920x1080" or "1280x720" fps(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peList : Resolution list
		[IN OUT] pnSize : list size
*/
NEPTUNE_C_API  ENeptuneError API_CALLTYPE ntcGetResolutionList(NeptuneCamHandle hCamHandle, ENeptuneResolution* peList, _uint32_t* pnSize);

/*
	Description : 
	    set value to "Auto", "Mono", "Color" or "EXE" mode(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eMode : Day/Night mode
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDayNightModeSelector(NeptuneCamHandle hCamHandle, ENeptuneDayNightMode eMode);

/*
	Description : 
	    get value to "Auto", "Mono", "Color" or "EXE" mode(Only USB 322UC Model Camera)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peMode : Day/Night mode
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetDayNightModeSelector(NeptuneCamHandle hCamHandle, ENeptuneDayNightMode* peMode);
////////////////////////////////////////////////////////////////////////////////////////
//
//							Trigger Information & Control
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get trigger information supported by a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pTriggerInfo : trigger information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetTriggerInfo(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER_INFO pTriggerInfo);

/*
	Description :
		get current trigger values
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pTrigger : trigger value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetTrigger(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER pTrigger);

/*
	Description :
		set trigger values to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] Trigger : trigger value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetTrigger(NeptuneCamHandle hCamHandle, NEPTUNE_TRIGGER Trigger);

/* Description :
		set exposure time and interval for trigger mode14 
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nExposure : exposure time for trigger mode14
		[IN] nInterval : interval time for trigger mode14
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetTriggerMode14Exposure(NeptuneCamHandle hCamHandle, _uint32_t nExposure, _uint32_t nInterval);

/*
	Description :
		get current trigger delay value
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pnDelay : current trigger delay value
		[OUT, optional] pnMin : minimum trigger value supported by a camera
		[OUT, optional] pnMax : maximum trigger value supported by a camera
*/
NEPTUNE_C_API ENeptuneError	API_CALLTYPE ntcGetTriggerDelay(NeptuneCamHandle hCamHandle, _uint32_t* pnDelay, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);

/*
	Description :
		set trigger delay value to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nDelay : trigger delay value
*/
NEPTUNE_C_API ENeptuneError	API_CALLTYPE ntcSetTriggerDelay(NeptuneCamHandle hCamHandle, _uint32_t nDelay);

/*
	Description :
		generate software trigger
	Parameters :
		[IN] hCamHandle : camera handle to control
*/
NEPTUNE_C_API ENeptuneError	API_CALLTYPE ntcRunSWTrigger(NeptuneCamHandle hCamHandle);

/*
	Description :
		read trigger table from a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pTriggerTable : trigger table
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadTriggerTable(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER_TABLE pTriggerTable);

/*
	Description :
		write and save trigger table to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] TriggerTable : trigger table
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSaveTriggerTable(NeptuneCamHandle hCamHandle, NEPTUNE_TRIGGER_TABLE TriggerTable);

/*
	Description :
		make camera to load specific trigger table
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nIndex : index of trigger table to load
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcLoadTriggerTable(NeptuneCamHandle hCamHandle, _uint32_t nIndex);

/*
	Description :
		set buffer count for trigger mode of USB camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nCounter : number of buffers
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUSBTriggerBufferCount(NeptuneCamHandle hCamHandle, _uint32_t nCounter);


////////////////////////////////////////////////////////////////////////////////////////
//
//							Strobe Information & Control
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get the strobe information supported by a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pStrobeInfo : strobe information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetStrobeInfo(NeptuneCamHandle hCamHandle, PNEPTUNE_STROBE_INFO pStrobeInfo);

/*
	Description :
		read strobe value from a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pStrobe : strobe value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetStrobe(NeptuneCamHandle hCamHandle, PNEPTUNE_STROBE pStrobe);

/*
	Description :
		set strobe value to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] Strobe : strobe value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetStrobe(NeptuneCamHandle hCamHandle, NEPTUNE_STROBE Strobe);


////////////////////////////////////////////////////////////////////////////////////////
//
//							AutoIris & Control
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		set AutoIris mode
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eMode : AutoIris operation mode(Auto, Manual)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoIrisMode(NeptuneCamHandle hCamHandle, ENeptuneAutoIrisMode eMode);

/*
	Description :
		get number of frames used for calculating brightness
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pnValue : current AutoIris average frame value
		[OUT, optional] pnMin : minimum AutoIris average frame value supported
		[OUT, optional] pnMax : maximum AutoIris average frame value supported
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAutoIrisAverageFrame(NeptuneCamHandle hCamHandle, _uint32_t* pnValue, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);

/*
	Description :
		get number of frames used for calculating brightness
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nValue : AutoIris average frame value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoIrisAverageFrame(NeptuneCamHandle hCamHandle, _uint32_t nValue);

/*
	Description :
		get target brightness level of AutoIris
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pnValue : current AutoIris target value
		[Out, optional] pnMin : minimum AutoIris target value
		[Out, optional] pnMax : maximum AutoIris target value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAutoIrisTargetValue(NeptuneCamHandle hCamHandle, _uint32_t* pnValue, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);

/*
	Description :
		set target brightness level of AutoIris
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] nValue : AutoIris target value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoIrisTargetValue(NeptuneCamHandle hCamHandle, _uint32_t nValue);


////////////////////////////////////////////////////////////////////////////////////////
//
//							GPIO Control
//
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		set GPIO to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] Gpio : GPIO value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetGPIO(NeptuneCamHandle hCamHandle, NEPTUNE_GPIO Gpio);


/////////////////////////////////////////////////////////////////////////////////////////
//
//							User Set Handling Functions
//
/////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get supported user set by a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pUserSet : user set information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetUserSet(NeptuneCamHandle hCamHandle, PNEPTUNE_USERSET pUserSet);

/*
	Description :
		save or load user set
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] UserSet : user set value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUserSet(NeptuneCamHandle hCamHandle, NEPTUNE_USERSET UserSet);

/*
	Description :
		make specific user set as a default user set
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eUserSet : user set value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDefaultUserSet(NeptuneCamHandle hCamHandle, ENeptuneUserSet eUserSet);

/*
	Description :
		make specific user set as a default user set when camera is power on
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eUserSet : user set value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPowerOnDefaultUserSet(NeptuneCamHandle hCamHandle, ENeptuneUserSet eUserSet);


/////////////////////////////////////////////////////////////////////////////////////////
//
//							Look-Up Table Handling Functions
//
/////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get 4-step knee table points
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pLUTPoints : 4-step knee table points
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetKneeLUT(NeptuneCamHandle hCamHandle, PNEPTUNE_KNEE_LUT pLUTPoints);

/*
	Description :
		set 4-step knee table points
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] LUTPoints : 4-step knee table points
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetKneeLUT(NeptuneCamHandle hCamHandle, NEPTUNE_KNEE_LUT LUTPoints);

/*
	Description :
		get user look-up-table value
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pLUTData : user look-up-table value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetUserLUT(NeptuneCamHandle hCamHandle, PNEPTUNE_USER_LUT pLUTData);

/*
	Description :
		set user look-up-table to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] LUTData : user look-up-table value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUserLUT(NeptuneCamHandle hCamHandle, NEPTUNE_USER_LUT LUTData);


/////////////////////////////////////////////////////////////////////////////////////////
//
//							Frame Save Control Functions(1394 camera only)
//
/////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get frame save information from a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peOnOff : frame save on/off state
		[OUT] pnFrameRemained : number of frames remained in the camera memory
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameSave(NeptuneCamHandle hCamHandle, ENeptuneBoolean* peOnOff, _uint32_t* pnFrameRemained);

/*
	Description :
		control camera to transfer saved frames
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eOnOff : frame save on/off control
		[IN] eTransfer : transfer control
		[IN] nFrames : number of frames to transfer
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameSave(NeptuneCamHandle hCamHandle, ENeptuneBoolean eOnOff, ENeptuneBoolean eTransfer, _uint32_t nFrames);


/////////////////////////////////////////////////////////////////////////////////////////
//
//							SIO Control Functions
//
/////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		set camera's serial output port information
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] Property : serial information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetSIO(NeptuneCamHandle hCamHandle,NEPTUNE_SIO_PROPERTY Property);

/*
	Description :
		write serial data to a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] Data : serial data
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteSIO(NeptuneCamHandle hCamHandle, NEPTUNE_SIO Data);

/*
	Description :
		read serial data from a camera
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] pData : serial data
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadSIO(NeptuneCamHandle hCamHandle, PNEPTUNE_SIO pData);


////////////////////////////////////////////////////////////////////////////////////////
/////////
/////////					Register Access 
/////////
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		read 4 bytes from a camera register
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ulAddr : register address
		[OUT] pulVal : register value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadRegister(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t* pulVal);

/*
	Description :
		write 4 bytes to a camera register
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ulAddr : register address
		[IN] ulVal : value to write
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteRegister(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t ulVal);

/*
	Description :
		read data a camera register
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ulAddr : start register address
		[OUT] pBuf : buffer to read data
		[IN/OUT] pnSize : number of bytes to read
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadBlock(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _uint8_t* pBuf, _ulong32_t* pnSize);

/*
	Description :
		write data to a camera register
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ulAddr : start register address
		[IN] pBuf : buffer to write
		[IN] nSize : size of the buffer
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteBlock(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _uint8_t* pBuf, _ulong32_t nSize);

/*
	Description :
		write broadcast to 1394 cameras
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] ulAddr : register address
		[IN] ulVal : value to write
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteBroadcast(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t ulVal);


//////////////////////////////////////////////////////////////////////////
// profile32 Control
//////////////////////////////////////////////////////////////////////////
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetProfileFeature(NeptuneCamHandle hCamHandle, _ulong32_t* pulLowTh, _ulong32_t* pulHighThr, _ulong32_t* pulMaxDiff, _ulong32_t* pulMinLaserWidth);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetProfileLowThreshold(NeptuneCamHandle hCamHandle, _ulong32_t ulVal);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetProfileHighThreshold(NeptuneCamHandle hCamHandle, _ulong32_t ulVal);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetProfileMaxDifference(NeptuneCamHandle hCamHandle, _ulong32_t ulVal);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetProfileMinLaserWidth(NeptuneCamHandle hCamHandle, _ulong32_t ulVal);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetProfileMaxLaserWidth(NeptuneCamHandle hCamHandle, _ulong32_t* pulVal);




////////////////////////////////////////////////////////////////////////////////////////
/////////
/////////					XML Control
/////////
////////////////////////////////////////////////////////////////////////////////////////
/*
	Description :
		get current visibility value of XML tree
	Parameters :
		[IN] hCamHandle : camera handle to control
		[OUT] peVisibility : visibility (Beginner, Expert, Guru, Invisible)
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeVisibility(NeptuneCamHandle hCamHandle, ENeptuneNodeVisibility* peVisibility);

/*
	Description :
		set visibility of XML tree
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] eVisibility : visibility 
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeVisibility(NeptuneCamHandle hCamHandle, ENeptuneNodeVisibility eVisibility);

/*
	Description :
		get child tree of a given XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrCategory : node
		[OUT] pList : child node tree
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeList(NeptuneCamHandle hCamHandle, const _char_t* pstrCategory, PNEPTUNE_XML_NODE_LIST pList);


/*
	Description :
		get child tree of a given XML node (for C#)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrCategory : node
		[OUT] pstrList : child node tree
		[IN] nSize : size of string
		[IN,OUT] pnCount : number of strings
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeListChar(NeptuneCamHandle hCamHandle, const _char_t* pstrCategory, _char_t* pstrList, _uint32_t nSize, _uint32_t* pnCount);


/*
	Description :
		get information of a XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] pInfo : information of a XML node
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeInfo(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_NODE_INFO pInfo);

/*
	Description :
		get information of integer type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] pValueInfo : integer value information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeInt(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_INT_VALUE_INFO pValueInfo);

/*
	Description :
		set value to an integer type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[IN] nValue : integer value to write
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeInt(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _int64_t nValue);

/*
	Description :
		get information of float type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] pValueInfo : float value information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeFloat(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_FLOAT_VALUE_INFO pValueInfo);

/*
	Description :
		set value to a float type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[IN] dValue : float value to write
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeFloat(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _double_t dValue);

/*
	Description :
		get information of enumeration type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] pList : enumeration value information
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeEnum(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_ENUM_LIST pList);


/*
	Description :
		get information of enumeration type XML node(for C#)
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] pstrList : enumeration value information (size is nSize*pnCount)
		[IN] nSize : should be 256
		[IN, OUT] pnCount : number of strings
		[OUT] pnIndex : current enumeration string index
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeEnumChar(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _char_t* pstrList, _uint32_t nSize, _uint32_t* pnCount, _uint32_t* pnIndex);

/*
	Description :
		set value to an enumeration type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[IN] pstrEnum : enumeration string to write
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeEnum(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrEnum);

/*
	Description :
		get information of string type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] pstrString : string value
		[IN/OUT] pnSize : size of string
		[OUT] peAccessMde : access mode of a node
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeString(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _char_t* pstrString, _uint32_t* pnSize, ENeptuneNodeAccessMode* peAccessMode);

/*
	Description :
		set value to a string type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[IN] pstrString : string value to write
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeString(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrString);

/*
	Description :
		get value of boolean type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[OUT] peState : boolean value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeBoolean(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, ENeptuneBoolean* peState);

/*
	Description :
		set value to boolean type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
		[IN] eState : boolean value
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeBoolean(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, ENeptuneBoolean eState);

/*
	Description :
		make execute command type XML node
	Parameters :
		[IN] hCamHandle : camera handle to control
		[IN] pstrNode : XML node
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeCommand(NeptuneCamHandle hCamHandle, const _char_t* pstrNode);

/*
	Description :
		Get camera list option value.
	Parameters :
		[OUT] peCameraListOpt : Camera List Option
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraListOpt(ENeptuneCameraListOpt * peCameraListOpt);

/*
	Description :
		Set camera list option value.
	Parameters :
		[IN] eCameraListOption : Camera List Option
*/
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetCameraListOpt(ENeptuneCameraListOpt eCameraListOpt);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetDisplayOption(ENeptuneDisplayOption * peDisplayOption);

NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDisplayOption(ENeptuneDisplayOption eDisplayOption);

#ifdef	__cplusplus
};
#endif


#endif		// __NEPTUNEC_H__

