
#pragma once

// XML node
#define		MAX_NODE_LIST_COUNT		128	// for XML tree node
#define		MAX_ENUM_LIST_COUNT		64	// for enumeration
#define		MAX_XML_NODE_STRING_LENGTH	256

// camera information
#define		MAX_STRING_LENGTH			512
#define		MAC_LENGTH					32
#define		MAX_INTERFACE_NUM			140

// type definitions
typedef int					_int32_t;
typedef unsigned int		_uint32_t;

typedef short				_int16_t;
typedef unsigned short      _uint16_t;

typedef char				_int8_t;
typedef unsigned char		_uint8_t;

typedef char				_char_t;
typedef unsigned char		_uchar_t;				// BYTE

typedef _uchar_t			_bool8_t;

typedef __int64				_int64_t;
typedef unsigned __int64	_uint64_t;

typedef long				_long32_t;
typedef unsigned long		_ulong32_t;

typedef __int64				_long64_t;
typedef unsigned __int64	_ulong64_t;

typedef double				_double_t;
typedef float				_float32_t;

typedef void				_void_t;

typedef _uint32_t				Pixel_Format;
typedef _uint32_t				FireWire_Frame_Rate;
typedef _uint32_t				Strobe;


#if	defined(__GNUC__)
typedef void *					HANDLE;
typedef unsigned char			BYTE;
typedef unsigned long			ULONG;
#endif
	
enum ENeptuneError {
	NEPTUNE_ERR_Fail					= -1,
	NEPTUNE_ERR_Success					= 0,
	NEPTUNE_ERR_AlreadyInitialized		= -100,
	NEPTUNE_ERR_APINotInitialized		= -101,
	NEPTUNE_ERR_NotInitialized			= -102,
	NEPTUNE_ERR_GC						= -103,
	NEPTUNE_ERR_TimeOut					= -104,
	NEPTUNE_ERR_TLInitFail				= -200,
	NEPTUNE_ERR_NoInterface				= -201,
	NEPTUNE_ERR_DeviceCheck				= -202,
	NEPTUNE_ERR_InvalidParameter		= -203,
	NEPTUNE_ERR_NotSupport				= -204,
	NEPTUNE_ERR_AccessDenied			= -205,
	NEPTUNE_ERR_InvalidAddress			= -206,
	NEPTUNE_ERR_InvalidArraySize		= -207,
	NEPTUNE_ERR_Interface				= -208,
	NEPTUNE_ERR_DeviceInfo				= -209,
	NEPTUNE_ERR_MemoryAlloc				= -210,
	NEPTUNE_ERR_DeviceOpen				= -211,
	NEPTUNE_ERR_DevicePort				= -212,
	NEPTUNE_ERR_DeviceURL				= -213,
	NEPTUNE_ERR_DeviceWrite				= -214,
	NEPTUNE_ERR_DeviceXML				= -215,
	NEPTUNE_ERR_DeviceHeartbeat			= -216,
	NEPTUNE_ERR_DeviceClose				= -217,
	NEPTUNE_ERR_DeviceStream			= -218,
	NEPTUNE_ERR_DeviceNotStreaming		= -219,
	// XML
	NEPTUNE_ERR_InvalidXMLNode			= -300,
	NEPTUNE_ERR_StreamCount				= -303,
	NEPTUNE_ERR_AccessTimeOut			= -304,
	NEPTUNE_ERR_OutOfRange				= -305,
	NEPTUNE_ERR_InvalidChannel			= -306,
	NEPTUNE_ERR_InvalidBuffer			= -307,
	// File
	NEPTUNE_ERR_FileAccessError			= -400,
};


enum ENeptuneBoolean {
	NEPTUNE_BOOL_FALSE		= 0,
	NEPTUNE_BOOL_TRUE		= 1
};


enum ENeptuneEffect {
	NEPTUNE_EFFECT_NONE		= 0,
	NEPTUNE_EFFECT_FLIP		= 0x01,		// flip image
	NEPTUNE_EFFECT_MIRROR	= 0x02,		// mirror
	NEPTUNE_EFFECT_NEGATIVE	= 0x04		// negative
} ;

enum ENeptuneAutoMode {
	NEPTUNE_AUTO_OFF				= 0,		// manual mode
	NEPTUNE_AUTO_ONCE				= 1,		// once(one-shot) mode
	NEPTUNE_AUTO_CONTINUOUS			= 2,		// auto mode
};	

enum ENeptunePixelFormat {
	Unknown_PixelFormat			= -1,
	// 1394 Camera pixel format list.
	Format0_320x240_YUV422		= 0,
	Format0_640x480_YUV411		= 1,
	Format0_640x480_YUV422		= 2,
	Format0_640x480_Mono8		= 3,
	Format0_640x480_Mono16		= 4,
	Format1_800x600_YUV422		= 5,
	Format1_800x600_Mono8		= 6,
	Format1_1024x768_YUV422		= 7,
	Format1_1024x768_Mono8		= 8,
	Format1_800x600_Mono16		= 9,
	Format1_1024x768_Mono16		= 10,

	Format2_1280x960_YUV422		= 11,
	Format2_1280x960_Mono8		= 12,
	Format2_1600x1200_YUV422	= 13,
	Format2_1600x1200_Mono8		= 14,
	Format2_1280x960_Mono16		= 15,
	Format2_1600x1200_Mono16	= 16,

	Format7_Mode0_Mono8			= 17,
	Format7_Mode0_YUV411		= 18,
	Format7_Mode0_YUV422		= 19,
	Format7_Mode0_Mono16		= 20,
	Format7_Mode0_Raw8			= 21,
	Format7_Mode0_Raw16			= 22,
	Format7_Mode0_Mono12		= 23,
	Format7_Mode0_Raw12			= 24,
		
	Format7_Mode1_Mono8			= 25,
	Format7_Mode1_YUV411		= 26,
	Format7_Mode1_YUV422		= 27,
	Format7_Mode1_Mono16		= 28,
	Format7_Mode1_Raw8			= 29,
	Format7_Mode1_Raw16			= 30,
	Format7_Mode1_Mono12		= 31,
	Format7_Mode1_Raw12			= 32,

	Format7_Mode2_Mono8			= 33,
	Format7_Mode2_YUV411		= 34,
	Format7_Mode2_YUV422		= 35,
	Format7_Mode2_Mono16		= 36,
	Format7_Mode2_Raw8			= 37,
	Format7_Mode2_Raw16			= 38,
	Format7_Mode2_Mono12		= 39,
	Format7_Mode2_Raw12			= 40,

	// GigE Camera pixel format list.
	Mono8						= 101,
	Mono10						= 102,
	Mono12						= 103,
	Mono16						= 104,
	BayerGR8					= 105,
	BayerGR10					= 106,
	BayerGR12					= 107,
	YUV411Packed				= 108,
	YUV422Packed				= 109
};

enum ENeptuneFrameRate {
	FPS_UNKNOWN			= -1,
	FPS_1_875			= 0,			// 1.875 frame (1394 camera)
	FPS_3_75			= 1,			// 3.75 frame (1394 camera)
	FPS_7_5				= 2,			// 7.5 frame (1394 camera)
	FPS_15				= 3,			// 15 frame (1394 camera)
	FPS_30				= 4,			// 30 frame (1394 camera)
	FPS_60				= 5,			// 60 frame (1394 camera)
	FPS_120				= 6,			// 120 frame (1394 camera)
	FPS_240				= 7,			// 240 frame (1394 camera)
	FPS_VALUE			= 20			// frame rate value(GigE camera)
};

enum ENeptunePixelType {
	NEPTUNE_PIXEL_MONO			= 1,
	NEPTUNE_PIXEL_BAYER			= 2,
	NEPTUNE_PIXEL_RGB			= 3,
	NEPTUNE_PIXEL_YUV			= 4,
	NEPTUNE_PIXEL_RGBPLANAR		= 5
};

enum ENeptuneBayerLayout {
	NEPTUNE_BAYER_GB_RG = 0,		// GB/RG layout
	NEPTUNE_BAYER_BG_GR = 1,		// BG/GR layout
	NEPTUNE_BAYER_RG_GB = 2,		// RG/GB layout
	NEPTUNE_BAYER_GR_BG = 3,		// GR/BG layout
};


enum ENeptuneBayerMethod {
	NEPTUNE_BAYER_METHOD_NONE = 0,			// no bayer conversion
	NEPTUNE_BAYER_METHOD_BILINEAR = 1,		// bilinear conversion
	NEPTUNE_BAYER_METHOD_HQ = 2,			// HQ conversion
	NEPTUNE_BAYER_METHOD_NEAREST = 3		// nearest conversion
};


enum ENeptuneAcquisitionMode {
	NEPTUNE_ACQ_CONTINUOUS		= 0,			// continuous
	NEPTUNE_ACQ_MULTIFRAME		= 1,			// multi frame
	NEPTUNE_ACQ_SINGLEFRAME		= 2,			// single frame
};


enum ENeptuneStreamMode {
	NEPTUNE_STRM_UNICAST	= 0,
	NEPTUNE_STRM_MULTICAST	= 1
};

enum ENeptuneImageFormat
{
	NEPTUNE_IMAGE_FORMAT_BMP	= 0,
	NEPTUNE_IMAGE_FORMAT_JPG	= 1,
	NEPTUNE_IMAGE_FORMAT_TIF	= 2
};

enum ENeptuneGrabFormat
{
	NEPTUNE_GRAB_RAW	= 0,
	NEPTUNE_GRAB_RGB	= 1
};

enum ENeptuneDeviceChangeState
{
	NEPTUNE_DEVICE_ADDED		= 0,	// camera count is increased
	NEPTUNE_DEVICE_REMOVED		= 1		// camera count is decreased
};

                  
struct NEPTUNE_IMAGE_SIZE {
	_int32_t		nStartX;	// start point of X coordinate(width direction)
	_int32_t		nStartY;	// start point of Y coordinate(height direction)
	_int32_t		nSizeX;		// width 
	_int32_t		nSizeY;		// height

	NEPTUNE_IMAGE_SIZE()
	{
		nStartY = 0;
		nStartY = 0;
		nSizeX = 0;
		nSizeY = 0;
	}
};
typedef	NEPTUNE_IMAGE_SIZE*			PNEPTUNE_IMAGE_SIZE;


struct NEPTUNE_IMAGE {
	_uint32_t 				uiWidth;		// image data width
	_uint32_t 				uiHeight;		// image data height
	_uint32_t 				uiBitDepth;		// data bits per pixel

	_uchar_t*				pData;			// image buffer
	_uint32_t				uiSize;			// buffer length
	_uint32_t				uiIndex;		// buffer index

	_uint64_t 				uiTimestamp;	// data timestamp

	NEPTUNE_IMAGE()
	{
		uiWidth = 0;
		uiHeight = 0;
		uiBitDepth = 0;
		pData = NULL;
		uiSize = 0;
		uiIndex = 0;
		uiTimestamp = 0;
	}

};
typedef NEPTUNE_IMAGE*			PNEPTUNE_IMAGE;	


// camera handle
typedef void *			NeptuneCamHandle;	

// device check callback
typedef void (CALLBACK *NeptuneCDevCheckCallback)(ENeptuneDeviceChangeState eState, void* pContext);

// camera unplug/plug callback
typedef void (CALLBACK *NeptuneCUnplugCallback)(void* pContext);

// image received callback
typedef void (CALLBACK *NeptuneCFrameCallback)(PNEPTUNE_IMAGE pImage, void* pContext);

// frame drop callback
typedef void (CALLBACK *NeptuneCFrameDropCallback)(void* pContext);



#include "NeptuneXML.h"
#include "NeptuneDev.h"
#include "NeptuneTrigger.h"
#include "NeptuneGPIO.h"
#include "NeptuneFeature.h"




