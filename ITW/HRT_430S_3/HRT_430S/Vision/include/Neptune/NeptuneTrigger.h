
#pragma once

#define		MAX_TRIGGER_PARAM			255

typedef enum _tagENeptuneTriggerSource
{
	NEPTUNE_TRIGGER_SOURCE_LINE1	= 0,	// external(H/W trigger)
	NEPTUNE_TRIGGER_SOURCE_SW		= 7		// software trigger
} ENeptuneTriggerSource;


typedef enum _tagENeptuneTriggerMode
{
	NEPTUNE_TRIGGER_MODE_0 = 0,		// trigger mode 0
	NEPTUNE_TRIGGER_MODE_1,			// trigger mode 1
	NEPTUNE_TRIGGER_MODE_2,			// trigger mode 2
	NEPTUNE_TRIGGER_MODE_3,			// trigger mode 3
	NEPTUNE_TRIGGER_MODE_4,			// trigger mode 4
	NEPTUNE_TRIGGER_MODE_5,			// trigger mode 5
	NEPTUNE_TRIGGER_MODE_6,			// trigger mode 6
	NEPTUNE_TRIGGER_MODE_7,			// trigger mode 7
	NEPTUNE_TRIGGER_MODE_8,			// trigger mode 8
	NEPTUNE_TRIGGER_MODE_9,			// trigger mode 9
	NEPTUNE_TRIGGER_MODE_10,		// trigger mode 10
	NEPTUNE_TRIGGER_MODE_11,		// trigger mode 11
	NEPTUNE_TRIGGER_MODE_12,		// trigger mode 12
	NEPTUNE_TRIGGER_MODE_13,		// trigger mode 13
	NEPTUNE_TRIGGER_MODE_14,		// trigger mode 14
	NEPTUNE_TRIGGER_MODE_15			// trigger mode 15
} ENeptuneTriggerMode;


typedef enum _tagENeptunePolarity
{
	NEPTUNE_POLARITY_RISINGEDGE		= 0,	// rising edge
	NEPTUNE_POLARITY_FALLINGEDGE	= 1,	// falling edge
	NEPTUNE_POLARITY_ANYEDGE		= 2,	// any edge
	NEPTUNE_POLARITY_LEVELHIGH		= 3,	// high level
	NEPTUNE_POLARITY_LEVELLOW		= 4		// low level
} ENeptunePolarity;


typedef struct _tagNEPTUNE_TRIGGER_INFO {
	ENeptuneBoolean		bSupport;		// trigger support flag
	_uint16_t			nModeFlag;		// bit mask for trigger mode
	_uint16_t			nSourceFlag;	// bit mask for trigger source (bit0 = H/W, bit7 = S/W)
	_uint16_t			nPolarityFlag;	// bit mask for polarity
	_uint16_t			nParamMin;		// trigger parameter minimum value
	_uint16_t			nParamMax;		// trigger parameter maximum value

	_tagNEPTUNE_TRIGGER_INFO()
	{
		bSupport = NEPTUNE_BOOL_FALSE;
		nModeFlag = 0;
		nSourceFlag = 0;
		nPolarityFlag = 0;
		nParamMin = 0;
		nParamMax = 0;
	}
} NEPTUNE_TRIGGER_INFO, *PNEPTUNE_TRIGGER_INFO;


typedef struct _tagNEPTUNE_TRIGGER {
	ENeptuneTriggerSource		Source;		// trigger source value
	ENeptuneTriggerMode			Mode;		// trigger mode value
	ENeptunePolarity			Polarity;	// trigger polarity value
	ENeptuneBoolean				OnOff;		// trigger on/off
	_uint16_t					nParam;		// trigger parameter

	_tagNEPTUNE_TRIGGER()
	{
		OnOff = NEPTUNE_BOOL_FALSE;
		Mode = NEPTUNE_TRIGGER_MODE_0;
		Source = NEPTUNE_TRIGGER_SOURCE_LINE1;
		Polarity = NEPTUNE_POLARITY_FALLINGEDGE;
		nParam = 0;
	}
} NEPTUNE_TRIGGER, *PNEPTUNE_TRIGGER;


typedef struct _tagNEPTUNE_TRIGGER_PARAM {
	_uint32_t		nFrameOrder;		// frame sequence number
	_uint32_t		nIncrement;			// end of table(0) or continuous(1)
	_uint32_t		nGainValue;			// gain feature value
	_uint32_t		nShutterValue;		// shutter feature value

	_tagNEPTUNE_TRIGGER_PARAM()
	{
		nShutterValue = 0;
		nGainValue = 0;
		nIncrement = 0;
		nFrameOrder = 0;
	}
} NEPTUNE_TRIGGER_PARAM, *PNEPTUNE_TRIGGER_PARAM;


typedef struct _tagNEPTUNE_TRIGGER_TABLE {
	NEPTUNE_TRIGGER_PARAM	Param[MAX_TRIGGER_PARAM];	// trigger parameter : max 255
	_uint32_t				Index;		// 0 ~ 15

	_tagNEPTUNE_TRIGGER_TABLE()
	{
		Index = 0;
		memset(Param, 0, sizeof(NEPTUNE_TRIGGER_PARAM)*MAX_TRIGGER_PARAM);
	}
} NEPTUNE_TRIGGER_TABLE, *PNEPTUNE_TRIGGER_TABLE;



////////////////////// strobe //////////////////////////
typedef enum _tagENeptuneStrobe
{
	NEPTUNE_STROBE0			= 0,
	NEPTUNE_STROBE1			= 1,
	NEPTUNE_STROBE2			= 2,
	NEPTUNE_STROBE3			= 3,
	NEPTUNE_STROBE4			= 4,
} ENeptuneStrobe;


typedef struct _tagNEPTUNE_STROBE_INFO {
	ENeptuneBoolean			bSupport;		// support of strobe
	_uint16_t				nStrobeFlag;	// support strobes bit flag
	_uint16_t				nPolarityFlag;	// strobe polarity support bit flag
	_uint16_t				nDurationMin;	// strobe duration minimum value
	_uint16_t				nDurationMax;	// strobe duration maximum value
	_uint16_t				nDelayMin;		// strobe delay minimum value
	_uint16_t				nDelayMax;		// strobe delay maximum value

	_tagNEPTUNE_STROBE_INFO()
	{
		bSupport = NEPTUNE_BOOL_FALSE;
		nStrobeFlag = 0;
		nDurationMin = 0;
		nDurationMax = 0;
		nDelayMin = 0;
		nDelayMax = 0;
		nPolarityFlag = 0;
	}
} NEPTUNE_STROBE_INFO, *PNEPTUNE_STROBE_INFO;


typedef struct _tagNEPTUNE_STROBE {
	ENeptuneBoolean				OnOff;		// strobe on/off control
	ENeptuneStrobe				Strobe;		// strobe index
	_uint16_t					nDuration;	// strobe duration value
	_uint16_t					nDelay;		// strobe delay value
	ENeptunePolarity			Polarity;	// strobe polarity

	_tagNEPTUNE_STROBE()
	{
		OnOff = NEPTUNE_BOOL_FALSE;
		Strobe = NEPTUNE_STROBE1;
		nDuration = 0;
		nDelay = 0;
		Polarity = NEPTUNE_POLARITY_LEVELHIGH;
	}
} NEPTUNE_STROBE, *PNEPTUNE_STROBE;


