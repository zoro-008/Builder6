#ifndef __MCX_31X_DEFINE_H__
#define __MCX_31X_DEFINE_H__


// MCX312/MCX314
#ifndef MCX31X_AXIS_X 
#define MCX31X_AXIS_X					0x01				// X
#define MCX31X_AXIS_Y					0x02				// Y
#endif	//MCX31X_AXIS_X

// Only MCX314
#ifndef MCX314_AXIS_Z
#define MCX314_AXIS_Z					0x04				// Z
#define MCX314_AXIS_U					0x08				// U
#endif	//AXIS_Z

#ifndef HIGH
#define HIGH							0x01
#define LOW								0x00
#endif	//HIGH

#ifndef PI
#define PI								3.141592
#endif	//PI


/* Read/Write Registers */
typedef enum _REG_DATA_WRITE
{
	WR0		= 0x00,
	WR1		= 0x02,
	WR2		= 0x04,
	WR3		= 0x06,
	WR4		= 0x08,
	WR5		= 0x0A,
	WR6		= 0x0C,
	WR7		= 0x0E,
	CLRINT	= 0x10,
	PGM		= 0x14
} REG_DATA_WRITE;

typedef enum _REG_BP_WRITE
{
	BP1P	= 0x04,
	BP1M	= 0x06,
	BP2P	= 0x08,
	BP2M	= 0x0A,
	BP3P	= 0x0C,
	BP3M	= 0x0E
} REG_BP_WRITE;

typedef enum _REG_DATA_READ
{
	RR0		= 0x00,
	RR1		= 0x02,
	RR2		= 0x04,
	RR3		= 0x06,
	RR4		= 0x08,
	RR5		= 0x0A,
	RR6		= 0x0C,
	RR7		= 0x0E,
	INTSTA	= 0x10,
	BDID	= 0x12,
	PGSTA	= 0x14
} REG_DATA_READ;

/* Command Lists */
typedef enum _CMD_LST
{
	/* Data Write Commands */
	RangeWrite							= 0x00,				/* 0x00 */
	JerkWrite							= 0x01,				/* 0x01 */
	AccelWrite							= 0x02,				/* 0x02 */
	DecelWrite							= 0x03,				/* 0x03 */
	InitialSpeedWrite					= 0x04,				/* 0x04 */
	DriveSpeedWrite						= 0x05,				/* 0x05 */
	PulseNumberWrite					= 0x06,				/* 0x06 */
	ManualDecelPointWrite				= 0x07,				/* 0x07 */
	CircularCenterPointWrite			= 0x08,				/* 0x08 */
	LogicalPositionWrite				= 0x09,				/* 0x09 */
	RealPositionWrite					= 0x0A,				/* 0x0A */
	CompPWrite							= 0x0B,				/* 0x0B */
	CompNWrite							= 0x0C,				/* 0x0C */
	AccelCountOffsetWrite				= 0x0D,				/* 0x0D */
	AxisSwitchingWrite					= 0x0F,				/* 0x0F */

	/* Data Read Commands */
	LogicalPositionRead					= 0x10,				/* 0x10 */
	RealPositionRead					= 0x11,				/* 0x11 */
	CurrentSpeedRead					= 0x12,				/* 0x12 */
	AccelDecelRead						= 0x13,				/* 0x13 */

	/* Driving Commands */
	PDirFixedPulseDrive					= 0x20,				/* 0x20 */
	NDirFixedPulseDrive					= 0x21,				/* 0x21 */
	PDirContinuousDrive					= 0x22,				/* 0x22 */
	NDirContinuousDrive					= 0x23,				/* 0x23 */
	DriveStartHolding					= 0x24,				/* 0x24 */
	DriveStartHoldRelease				= 0x25,				/* 0x25 */
	DecelerationStop					= 0x26,				/* 0x26 */
	SuddenStop							= 0x27,				/* 0x27 */

	/* Interpolation Commands */
	TwoAxisLI							= 0x30,				/* 0x30 */
	ThreeAxisLI							= 0x31,				/* 0x31 */
	CWCI								= 0x32,				/* 0x32 */
	CCWCI								= 0x33,				/* 0x33 */
	TwoAxisBPI							= 0x34,				/* 0x34 */
	ThreeAxisBPI						= 0x35,				/* 0x35 */
	BPRegWriteEnable					= 0x36,				/* 0x36 */
	BPRegWriteDisable					= 0x37,				/* 0x37 */
	BPDataStack							= 0x38,				/* 0x38 */
	BPDataClear							= 0x39,				/* 0x39 */
	SingleSI							= 0x3A,				/* 0x3A */
	DecelEnable							= 0x3B,				/* 0x3B */
	DecelDisable						= 0x3C,				/* 0x3C */
	InterpolationIntrClear				= 0x3D				/* 0x3D */
} CMD_LST;

typedef enum _ACTION
{
	Disable = 0x0,
	EmgStopEnable   = 0x1,
	SlowStopEnalbe  = 0x2,
} ACTION;

#define MAX_312_BOARD					21
#define MAX_314_BOARD					21

//<+> 2002.11.20
/* Pulse Output Method					*/
typedef enum _MCX312_PULSE_OUTPUT
{
} MCX31X_PULSE_OUTPUT;

// WR2:D8 - DIR-L => 1 Pulse 모드일때만 유효
#define MCX31X_PULSEOUT_DIR_LOW_P		0x00				// +dir:Low  / -dir:High
#define MCX31X_PULSEOUT_DIR_HIGH_P		0x04				// +dir:High / -dir:Low
// WR2:D7 - PLS-L
#define MCX31X_PULSEOUT_POSITIVE_LEVEL	0x00				// positive logical level
#define MCX31X_PULSEOUT_NEGATIVE_LEVEL	0x02				// negative logical level
// WR2:D6 - PLSMD
#define MCX31X_PULSEOUT_2_PULSE			0x00				// Cw/Ccw
#define MCX31X_PULSEOUT_1_PULSE			0x01				// Pulse/Dir

// 
typedef enum _ENCODER_INPUT_METHOD
{
	MCX31X_UpDownMode					= 0x0,				// Up/Down
	MCX31X_Sqr1Mode						= 0x1,				// 1체배
	MCX31X_Sqr2Mode						= 0x2,				// 2체배
	MCX31X_Sqr4Mode						= 0x3				// 4체배
} ENCODER_INPUT_METHOD;

#endif






#if 0
//#define	MAX_AXIS		16

//#define VendorID	0x13FE		// Vendor ID
//#define DeviceID	0x1240		// Device ID

/*
typedef struct {
	BOOL   	fOpenModule;
	INT16  	BoardNo;
	INT16  	ModuleNo;
	INT16  	ModelNo;
	UINT16 	wModuleAddr;
	INT16  	nNumberOfAxis;
} Mcx312ModuleInfoStruct;
*/
#endif	//0