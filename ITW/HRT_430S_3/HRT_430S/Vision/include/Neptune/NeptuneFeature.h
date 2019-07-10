
#pragma once


/////////////////////////////////////////////////////////
/////
/////				General Feature
/////
//////////////////////////////////////////////////////////

typedef enum _tagENeptuneFeature {
	NEPTUNE_FEATURE_GAMMA				= 0,	// AnalogControls, Gamma
	NEPTUNE_FEATURE_GAIN				= 1,	// AnalogControls, Gain or GainRaw
	NEPTUNE_FEATURE_RGAIN				= 2,	// AnalogControls, Gain or GainRaw
	NEPTUNE_FEATURE_GGAIN				= 3,	// AnalogControls, Gain or GainRaw
	NEPTUNE_FEATURE_BGAIN				= 4,	// AnalogControls, Gain or GainRaw
	NEPTUNE_FEATURE_BLACKLEVEL			= 5,	// AnalogControls, BlackLevel or BlackLevelRaw
	NEPTUNE_FEATURE_SHARPNESS			= 6,	// AnalogControls, Sharpness or SharpnessRaw
	NEPTUNE_FEATURE_SATURATION			= 7,	// AnalogControls, Saturation or SaturationRaw
	NEPTUNE_FEATURE_AUTOEXPOSURE		= 8,	// AcquisitionControl, AutoExposure
	NEPTUNE_FEATURE_SHUTTER				= 9,	// AcquisitionControl, ExposureTime
	NEPTUNE_FEATURE_HUE					= 10,	// AnalogControls, Hue or HueRaw
	NEPTUNE_FEATURE_PAN					= 11,	// AcquisitionControl, PanCtrl
	NEPTUNE_FEATURE_TILT				= 12,	// AcquisitionControl, TiltCtrl
	NEPTUNE_FEATURE_OPTFILTER			= 13,	// AnalogControls, OpticalFilter
	NEPTUNE_FEATURE_AUTOSHUTTER_MIN		= 14,	// CustomControl, AutoShutterSpeedMin
	NEPTUNE_FEATURE_AUTOSHUTTER_MAX		= 15,	// CustomControl, AutoShutterSpeedMin
	NEPTUNE_FEATURE_AUTOGAIN_MIN		= 16,	// CustomControl, AutoGainMin
	NEPTUNE_FEATURE_AUTOGAIN_MAX		= 17,	// CustomControl, AutoGainMax
	NEPTUNE_FEATURE_TRIGNOISEFILTER		= 18,	// CustomControl, TriggerNoiseFilter
	NEPTUNE_FEATURE_BRIGHTLEVELIRIS		= 19,	// CustomControl, BrightLevelForIRIS (Read Only)
	NEPTUNE_FEATURE_SNOWNOISEREMOVE		= 20,	// CustomControl, SnowNosieRemoveControl
	NEPTUNE_FEATURE_WATCHDOG			= 21,	// CustomControl, WDGControl
	NEPTUNE_FEATURE_WHITEBALANCE		= 22,	// AnalogControls, BalanceWhiteAudo
	NEPTUNE_FEATURE_CONTRAST			= 23,	// CustomControl, Contrast
	NEPTUNE_FEATURE_LCD_BLUE_GAIN		= 24,	// CustomControl, LED_BLUE_GAIN
	NEPTUNE_FEATURE_LCD_RED_GAIN		= 25,	// CustomControl, LED_RED_GAIN
} ENeptuneFeature;


typedef struct _tagNEPTUNE_FEATURE {
	ENeptuneBoolean			bSupport;
	ENeptuneBoolean			bOnOff;				// on/off state, on/off control(SnowNoiseRemove only)
	_uint8_t				SupportAutoModes;	// bit flag for support(bit0:Off, bit1:Once, bit2:Continuous)
	ENeptuneAutoMode		AutoMode;			// current Auto mode, valid only when "SuporeAutoMode != 0"
	_int32_t				Min;				// minimum value
	_int32_t				Max;				// maximum value
	_int32_t				Inc;				// increment step
	_int32_t				Value;				// current value
	ENeptuneNodeAccessMode	ValueAccessMode;	// access state of the value		

	_tagNEPTUNE_FEATURE()
	{
		bOnOff = NEPTUNE_BOOL_TRUE;
		bSupport = NEPTUNE_BOOL_FALSE;
		SupportAutoModes = 0;
		AutoMode = NEPTUNE_AUTO_OFF;
		Min = 0;		// minimum value
		Max = 0;		// maximum value
		Inc = 0;		// increment step
		Value = 0;		// current value
		ValueAccessMode = NEPTUNE_NODE_ACCESSMODE_NA;
	}
} NEPTUNE_FEATURE, *PNEPTUNE_FEATURE;


//////////////////////////////////////////////////////////////////////////
////
////               Packet feature
///
//////////////////////////////////////////////////////////////////////////
typedef struct _tagNEPTUNE_PACKAGE_FEATURE{
	_uint32_t Gain;	
	_uint32_t Sharpness;
	_uint32_t Shutter;	
	_uint32_t BlackLevel;
	_uint32_t Contrast;
	_uint32_t Gamma;
} NEPTUNE_PACKAGE_FEATURE, *PNEPTUNE_PACKAGE_FEATURE;


/////////////////////////////////////////////////////////
/////
/////					UserSet
/////
//////////////////////////////////////////////////////////
typedef enum _tagENeptuneUserSet {
	NEPTUNE_USERSET_DEFAULT		= 0,
	NEPTUNE_USERSET_1			= 1,
	NEPTUNE_USERSET_2			= 2,
	NEPTUNE_USERSET_3			= 3,
	NEPTUNE_USERSET_4			= 4,
	NEPTUNE_USERSET_5			= 5,
	NEPTUNE_USERSET_6			= 6,
	NEPTUNE_USERSET_7			= 7,
	NEPTUNE_USERSET_8			= 8,
	NEPTUNE_USERSET_9			= 9,
	NEPTUNE_USERSET_10			= 10,
	NEPTUNE_USERSET_11			= 11,
	NEPTUNE_USERSET_12			= 12,
	NEPTUNE_USERSET_13			= 13,
	NEPTUNE_USERSET_14			= 14,
	NEPTUNE_USERSET_15			= 15
} ENeptuneUserSet;


typedef enum _tagENeptuneUserSetCommand {
	NEPTUNE_USERSET_CMD_LOAD	= 0,
	NEPTUNE_USERSET_CMD_SAVE	= 1,
} ENeptuneUserSetCommand;


typedef struct _tagNEPTUNE_USERSET {
	_uint16_t					SupportUserSet;		// bit flag for supported user set, 0 is "Default"
	ENeptuneUserSet				UserSetIndex;		// user set index to save or load
	ENeptuneUserSetCommand		Command;			// save or load

	_tagNEPTUNE_USERSET()
	{
		SupportUserSet = 0;
		UserSetIndex = NEPTUNE_USERSET_DEFAULT;
		Command = NEPTUNE_USERSET_CMD_LOAD;
	}
} NEPTUNE_USERSET, *PNEPTUNE_USERSET;


/////////////////////////////////////////////////////////
/////
/////				Auto Iris
/////
//////////////////////////////////////////////////////////
typedef enum _tagENeptuneAutoIrisMode
{
	NEPTUNE_AUTOIRIS_MODE_MANUAL = 0,
	NEPTUNE_AUTOIRIS_MODE_AUTO	 = 1,
} ENeptuneAutoIrisMode;



/////////////////////////////////////////////////////////
/////
/////				Look-up Table
/////
//////////////////////////////////////////////////////////
#define KNEE_POINT_TABLE_SIZE		4
typedef struct _tagNEPTUNE_POINT {
	_uint32_t	x;		// x-coordinate
	_uint32_t	y;		// y-coordinate

	_tagNEPTUNE_POINT()
	{
		x = 0;
		y = 0;
	}
} NEPTUNE_POINT, *PNEPTUNE_POINT;


typedef struct _tagNEPTUNE_KNEE_LUT {
	NEPTUNE_POINT		Points[KNEE_POINT_TABLE_SIZE];		// 4 points
	ENeptuneBoolean		bEnable;							// enable/disable state/control

	_tagNEPTUNE_KNEE_LUT()
	{
		bEnable = NEPTUNE_BOOL_FALSE;
		memset(Points, 0, sizeof(NEPTUNE_POINT)*KNEE_POINT_TABLE_SIZE);
	}
} NEPTUNE_KNEE_LUT, *PNEPTUNE_KNEE_LUT;


#define USER_LUT_TABLE_SIZE			4096
typedef struct _tagNEPTUNE_USER_LUT {
	_uint16_t			SupportLUT;					// bit Flag
	_uint16_t			LUTIndex;					// current LUT index
	ENeptuneBoolean		bEnable;					// enable/disable state/control
	_uint16_t			Data[USER_LUT_TABLE_SIZE];	// LUT data, valid only in Set function

	_tagNEPTUNE_USER_LUT()
	{
		SupportLUT = 0;
		LUTIndex = 0;
		bEnable = NEPTUNE_BOOL_FALSE;
		memset(Data, 0, sizeof(_uint16_t)*USER_LUT_TABLE_SIZE);
	}
} NEPTUNE_USER_LUT, *PNEPTUNE_USER_LUT;


/////////////////////////////////////////////////////////
/////
/////						SIO
/////
//////////////////////////////////////////////////////////
typedef enum _tagENeptuneSIOParity
{
	NEPTUNE_SIO_PARITY_NONE		= 0,
	NEPTUNE_SIO_PARITY_ODD		= 1,
	NEPTUNE_SIO_PARITY_EVEN		= 2,
} ENeptuneSIOParity;


typedef struct _tagNEPTUNE_SIO_PROPERTY
{
	ENeptuneBoolean		bEnable;	// RS232 enable
	_uint32_t			Baudrate;	// serial baudrate
	ENeptuneSIOParity	Parity;		// parity bit
	_uint32_t			DataBit;	// data bit
	_uint32_t			StopBit;	// stop bit

	_tagNEPTUNE_SIO_PROPERTY()
	{
		bEnable = NEPTUNE_BOOL_FALSE;
		Baudrate = 9600;
		Parity = NEPTUNE_SIO_PARITY_NONE;
		DataBit = 8;
		StopBit = 1;
	}
} NEPTUNE_SIO_PROPERTY, *PNEPTUNE_SIO_PROPERTY;


typedef struct _tagNEPTUNE_SIO
{
	_uint32_t		TextCount;		// should be smaller than or equal to 256
	_char_t			strText[256];	// RS232 data
	_uint32_t		TimeOut;		// in ms unit

	_tagNEPTUNE_SIO()
	{
		TextCount = 0;
		memset(strText, 0, sizeof(_char_t)*256);
		TimeOut = 100;
	}
} NEPTUNE_SIO, *PNEPTUNE_SIO;



/////////////////////////////////////////////////////////
/////
/////				Auto Area Control
/////
//////////////////////////////////////////////////////////
typedef enum _tagENeptuneAutoAreaSelect
{
	NEPTUNE_AUTOAREA_SELECT_AE		= 0,	// for AutoExposure
	NEPTUNE_AUTOAREA_SELECT_AWB		= 1,	// for AutoWhiteBalance
	NEPTUNE_AUTOAREA_SELECT_AF		= 2		// for AutoFocus
} ENeptuneAutoAreaSelect;


typedef enum _tagENeptuneAutoAreaSize
{
	NEPTUNE_AUTOAREA_SIZE_SELECTED	= 0,	// selected size
	NEPTUNE_AUTOAREA_SIZE_FULL		= 1,	// full image size
} ENeptuneAutoAreaSize;


typedef struct _tagNEPTUNE_AUTOAREA {
	ENeptuneBoolean			OnOff;
	ENeptuneAutoAreaSize	SizeControl;
	NEPTUNE_IMAGE_SIZE		AreaSize;

	_tagNEPTUNE_AUTOAREA()
	{
		OnOff = NEPTUNE_BOOL_FALSE;
		SizeControl = NEPTUNE_AUTOAREA_SIZE_FULL;
		memset(&AreaSize, 0, sizeof(NEPTUNE_IMAGE_SIZE));
	};
} NEPTUNE_AUTOAREA, *PNEPTUNE_AUTOAREA;



/////////////////////////////////////////////////////////
/////
/////				Auto Focus Control
/////
//////////////////////////////////////////////////////////
typedef enum _tagENeptuneAFMode
{
	NEPTUNE_AF_ORIGIN			= 0,		// set focus to origin point
	NEPTUNE_AF_ONEPUSH			= 1,		// one-push auto focus
	NEPTUNE_AF_STEP_FORWARD		= 2,		// move one step forward
	NEPTUNE_AF_STEP_BACKWARD	= 3			// move one step backward
} ENeptuneAFMode;






