#ifndef __IMCAMDEF_H__
#define __IMCAMDEF_H__

// Never edit this file

#define		IMC_SUCCESS					1
#define		IMC_FAIL					0

// Preview window characteristics
#define		IMC_VIEW_RESIZE				0x00000001UL
#define		IMC_VIEW_STATIC				0x00000002UL
#define		IMC_VIEW_FILL				0x00000004UL

static const GUID CAPTURE_NOTIFY_ID =  
{0x65E8773DL, 0x8F56, 0x11D0, 0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96};

#define			WM_SW_AWB_END			WM_USER+40

// Video Format definitions
#define		IMC_FORMAT_RGB24			0
#define		IMC_FORMAT_Y800				1
#define		IMC_FORMAT_Y411				2
#define		IMC_FORMAT_Y422				3
#define		IMC_FORMAT_Y444				4
#define		IMC_FORMAT_VYUY				5
#define		IMC_FORMAT_Y1600			6
#define		IMC_FORMAT_RAW8				7
#define		IMC_FORMAT_RAW16			8
#define		IMC_FORMAT_UNKNOWN			-1


// Video Mode definitions
#define		IMC_MODE_160x120			0
#define		IMC_MODE_200x150			1
#define		IMC_MODE_320x240			2
#define		IMC_MODE_340x240			3
#define		IMC_MODE_636x480			4
#define		IMC_MODE_640x480			5
#define		IMC_MODE_800x600			6
#define		IMC_MODE_1024x768			7
#define		IMC_MODE_1280x960			8
#define		IMC_MODE_1280x1024			9
#define		IMC_MODE_1360x1032			10
#define		IMC_MODE_1376x1032			11
#define		IMC_MODE_1600x1200			12
#define		IMC_MODE_1392x720			13
#define		IMC_MODE_2048x1536			14

#define		IMC_MODE_VARIABLE_MODE0		20
#define		IMC_MODE_VARIABLE_MODE1		21
#define		IMC_MODE_VARIABLE_MODE2		22
#define		IMC_MODE_VARIABLE_MODE3		23
#define		IMC_MODE_VARIABLE_MODE4		24
#define		IMC_MODE_VARIABLE_MODE5		25
#define		IMC_MODE_VARIABLE_MODE6		26
#define		IMC_MODE_VARIABLE_MODE7		27
#define		IMC_MODE_UNKNOWN			-1


// Frame rate definitions
#define 	IMC_FRATE_1_875				0
#define		IMC_FRATE_3_75				1
#define		IMC_FRATE_7_5				2
#define		IMC_FRATE_10				3
#define		IMC_FRATE_15				4
#define		IMC_FRATE_20				5
#define		IMC_FRATE_30				6
#define		IMC_FRATE_40				7
#define		IMC_FRATE_60				8
#define		IMC_FRATE_120				9
#define		IMC_FRATE_240				10
#define		IMC_FRATE_UNKNOWN			-1


//  Grab mode definitions
#define		IMC_GRAB_RAW				0	// stream data without converting	
#define		IMC_GRAB_RGB888				1	// coverting data to RGB24
//#define	IMC_GRAB_RGB555				2	// not used any more
//#define	IMC_GRAB_RGB565				3	// not used any more


// feature definitions
#define		IMC_FEATURE_BRIGHTNESS		0
#define		IMC_FEATURE_SHARPNESS		1
#define		IMC_FEATURE_GAMMA			2
#define		IMC_FEATURE_SATURATION		3
#define		IMC_FEATURE_WB_UB			4
#define		IMC_FEATURE_WB_VR			5
#define		IMC_FEATURE_HUE				6
#define		IMC_FEATURE_EXPOSURE		7
#define		IMC_FEATURE_SHUTTER			8
#define		IMC_FEATURE_GAIN			9
#define		IMC_FEATURE_IRIS			10
#define		IMC_FEATURE_ZOOM			11
#define		IMC_FEATURE_FOCUS			12
#define		IMC_FEATURE_PAN				13
#define		IMC_FEATURE_TILT			14
#define		IMC_FEATURE_OPTFILTER		15
#define		IMC_FEATURE_PIXEL_CLOCK		16
#define		IMC_FEATURE_BLANK			17
// Removed from version 3.0.0.0
#define		IMC_FEATURE_TRIGGER			18		// for USB Camera
#define		IMC_FEATURE_UNKNOWN			-1

// SW Control Definitions
#define		IMC_DATABITS_10				10
#define		IMC_DATABITS_12				12
#define		IMC_DATABITS_14				14
#define		IMC_DATABITS_16				16

#define		IMC_BAYERCONVERT_3X3		1
#define		IMC_BAYERCONVERT_5X5		2

#define		IMC_BAYER_GB_RG				0
#define		IMC_BAYER_BG_GR				1
#define		IMC_BAYER_RG_GB				2
#define		IMC_BAYER_GR_BG				3

// SIO Control Definitions
#define		IMC_BAUD_RATE_300			0
#define		IMC_BAUD_RATE_600			1
#define		IMC_BAUD_RATE_1200			2
#define		IMC_BAUD_RATE_2400			3
#define		IMC_BAUD_RATE_4800			4
#define		IMC_BAUD_RATE_9600			5
#define		IMC_BAUD_RATE_19200			6
#define		IMC_BAUD_RATE_38400			7
#define		IMC_BAUD_RATE_57600			8
#define		IMC_BAUD_RATE_115200		9
#define		IMC_BAUD_RATE_230400		10

#define		IMC_DATABIT_7				7
#define		IMC_DATABIT_8				8

#define		IMC_PARITY_NONE				0
#define		IMC_PARITY_ODD				1
#define		IMC_PARITY_EVEN				2

#define		IMC_STOPBIT_1				0
#define		IMC_STOPBIT_1_5				1
#define		IMC_STOPBIT_2				2

typedef struct {
	BOOL	bSupport;
	BOOL	bAutoCapable;
	BOOL	bAuto;
	BOOL	bOnePushCapable;
	BOOL	bOnePush;
	INT		nValue;
	INT		nMin;
	INT		nMax;
} IMC_FEATURE_PROPERTY, *PIMC_FEATURE_PROPERTY;


typedef struct {
	INT		unit_x;
	INT		unit_y;
	INT		hpos;
	INT		vpos;
	INT		hsize;
	INT		vsize;
	INT		bpp_min;
	INT		bpp_max;
	INT		bpp;
} IMC_ROI_PROPERTY, *PIMC_ROI_PROPERTY;


typedef struct {
	INT		b_gain;
	INT		g_gain;
	INT		r_gain;
} IMC_RGB_GAIN, *PIMC_RGB_GAIN;


typedef struct {
	BOOL	bTriggerOn;					// trigger enable/disable state
	BOOL	bTriggerPolarity;			// trigger polarity 0=low active, 1=high active, valid only when bTriggerOn = TRUE;
	INT		nTriggerSource;				// trigger source ID, valid only when bTriggerOn = TRUE
	INT		nTriggerMode;				// trigger mode, valid only when bTriggerOn = TRUE
	INT		nTriggerParam;				// trigger parameter, valid only when bTriggerOn = TRUE
	INT		nExposureTime;				// Valid if trigger mode = 14
	INT		nExposureTimeInterval;		// Valid if trigger mode = 14
} IMC_TRIGGER_PROPERTY, *PIMC_TRIGGER_PROPERTY;


typedef struct {
	BOOL	bPolarityControl;
	BOOL	bTriggerSourceSupport[4];
	BOOL	bSWTriggerControl;
	BOOL	bTriggerModeSupport[16];
	// 2009-05-07 : Removed : From API 2
	//BOOL	bTriggerDelaySupport;
	////
} IMC_TRIGGER_INFO, *PIMC_TRIGGER_INFO;


typedef struct 
{
	BOOL	bOneShot;
	BOOL	bMultiShot;
	BOOL	bMultiShotActive;			// TRUE if multishot is running
	INT		nMultiShotFrameCount;
} IMC_MULTISHOT_PROPERTY, *PIMC_MULTISHOT_PROPERTY;


typedef struct {
	// support
	BOOL	bSupport;					// flag of support strobe by the camera
	BOOL	bOnOffSupport;				//
	BOOL	bPolaritySupport;			// 
	INT		nMin;
	INT		nMax;
	// state
	BOOL	bOnOff;						// On=1, Off=0, means state GetStrobe(), means value SetStrobe()
	BOOL	bSignalPolarity;			// HighActive=1, LowActive=0
	INT		nDelayValue;				// delay value
	INT		nDurationValue;				// strobe duration
} IMC_STROBE_PROPERTY, *PIMC_STROBE_PROPERTY;


typedef struct {
	BOOL	bSupport;					// flag for support the multi-channle save/load by the camera
	BOOL	bSaveLoad;					// 0=LOAD, 1=SAVE
	INT		nChannelIndex;				// current memory channel
	INT		nSupportedChannelCount;		// number of channel supported by the camera
	INT		nFormatIndex;				// format index loaded
	INT		nModeIndex;					// mode index loaded
	INT		nFrameRateIndex;			// frame rate index loaded
	IMC_ROI_PROPERTY	RoiProp;		// Valid only when the nModeIndex >= IMC_MODE_VARIABLE_MODE0
} IMC_MEMORY_CHANNEL_PROPERTY, *PIMC_MEMORY_CHANNEL_PROPERTY;


typedef struct {
	INT		nX;
	INT		nY;
} IMC_POINT, *PIMC_POINT;

typedef struct {
	BOOL	bOnOff;
	BOOL	bPath;
	IMC_POINT Points[4];
} IMC_KNEE_POINT_PROPERTY, *PIMC_KNEE_POINT_PROPERTY;


typedef struct {
	INT		nBaudRate;
	INT		nCharLength;
	INT		nParity;
	INT		nStopBit;
} IMC_SIO_PROPERTY, *PIMC_SIO_PROPERTY;

#endif	// __IMCAMDEF_H__
