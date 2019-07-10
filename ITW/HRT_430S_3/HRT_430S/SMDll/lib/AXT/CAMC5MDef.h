#ifndef	__CAMC_5M_DEFINE_H__
#define	__CAMC_5M_DEFINE_H__

// 2004년 3월 10일 안병건 추가
#ifndef SYNC_SET
#define MASTER	1
#define SLAVE	0
#endif

/*-------------------------------------------------------------------------------------------------*
 *        CAMC-5M (SMC-1V01, SMC-2V01)                                                             *
 *-------------------------------------------------------------------------------------------------*/

/* CAMC-5M PORT_DATA 셋팅 - ADDRESS에 값을 쓰기 위한	*/
typedef enum _PORT_DATA_WRITE
{
	Data1Write			= 0x00,        
	Data2Write			= 0x01,        
	Data3Write			= 0x02,        
	Data4Write			= 0x03,        
	CommandWrite		= 0x04,    
	Mode1Write			= 0x05,       
	Mode2Write			= 0x06,       
	UniversalSignalWrite= 0x07
} PORT_DATA_WRITE;

/* ADDRESS에 값 읽기			*/
typedef enum _PORT_DATA_READ
{
	Data1Read				= 0x00,        
	Data2Read				= 0x01,        
	Data3Read				= 0x02,        
	Data4Read				= 0x03,        
	DriveStatusRead			= 0x04,        /* read	*/
	EndStatusRead			= 0x05,        /* read  */
	MechanicalSignalRead	= 0x06,        /* read	*/
	UniversalSignalRead		= 0x07
} PORT_DATA_READ;

/* COMMAND KIND			*/
typedef enum _COMMAND
{
	RangeDataWrite						= 0x00,
	RangeDataRead						= 0x01,
	StartStopSpeedDataWrite				= 0x02,
	StartStopSpeedDataRead				= 0x03,
	ObjectSpeedDataWrite				= 0x04,
	ObjectSpeedDataRead					= 0x05,
	Rate1DataWrite						= 0x06,
	Rate1DataRead						= 0x07,
	Rate2DataWrite						= 0x08,
	Rate2DataRead						= 0x09,
	Rate3DataWrite						= 0x0a,
	Rate3DataRead						= 0x0b,
	RateChangePoint12Write				= 0x0c,
	RateChangePoint12Read				= 0x0d,
	RateChangePoint23Write				= 0x0e,
	RateChangePoint23Read				= 0x0f,
	SlowDownRealPulseWrite				= 0x10,
	SlowDownRealPulseRead				= 0x11,
	CurrentSpeedDataRead				= 0x12,
	DrivePulseCountRead					= 0x13,
	PresetPulseDataOverride				= 0x14,
	PresetPulseDataRead					= 0x15,
    DeviationDataRead					= 0x16,
	NoOperation							= 0x17,
	InpositionWaitModeSet				= 0x18,
	InpositionWaitModeReset				= 0x19,
	AlarmStopEnableModeSet				= 0x1a,
	AlarmStopEnableModeReset			= 0x1b,
	InterruptOutEnableModeSet			= 0x1c,
	InterruptOutEnableModeReset			= 0x1d,
	SlowDownStop						= 0x1e,
	EmergencyStop						= 0x1f,
	PresetPulseDriveP					= 0x20,
	PresetPulseDriveN					= 0x21,
    ContinuousDriveP					= 0x22,
    ContinuousDriveN					= 0x23,
    SignalSearch1DriveP					= 0x24,
    SignalSearch1DriveN					= 0x25,
    SignalSearch2DriveP					= 0x26,
    SignalSearch2DriveN					= 0x27,
    InternalCounterWrite				= 0x28,
    InternalCounterRead					= 0x29,
    InternalComparatorDataWrite			= 0x2a,
    InternalComparatorDataRead			= 0x2b,
    ExternalCounterWrite				= 0x2c,
    ExternalCounterRead					= 0x2d,
    ExternalComparatorDataWrite			= 0x2e,
    ExternalComparatorDataRead 			= 0x2f
} COMMAND;


/*
	모듈의 Version :
*/
#define CAMC_5M_VERSION_10				0					// 5M Ver 1.0
#define CAMC_5M_VERSION_30				3					// 5M Ver 3.0


#endif // __CAMC_5M_DEFINE_H__


/*-------------------------------------------------------------------------------------------------*
 *        CAMC-5M, CAMC- 1.0 / 2.0에서 공통으로 사용하는 매크로...                               *
 *-------------------------------------------------------------------------------------------------*/

#ifndef __COMMON_CAMC_CHIP_DEFINE_H__
#define __COMMON_CAMC_CHIP_DEFINE_H__


#ifndef POSITIVE_SENSE
#define POSITIVE_SENSE	1
#define NEGATIVE_SENSE	-1	
#endif


/* Main clock							*/
#define F_33M_CLK						33000000L			/* 33.000 MHz */
#define F_32_768M_CLK					32768000L			/* 32.768 MHz */
#define	F_20M_CLK						20000000L			/* 20.000 MHz */
#define	F_16_384M_CLK					16384000L			/* 16.384 MHz : Default */


/* MODE1 DATA							
 *
 *	감속 개시 POINT 검출 방식
 */
typedef enum _DETECT_DOWN_START_POINT
{
	AutoDetect  = 0x0,										// 자동검출
	RestPulse   = 0x1										// 잔량펄스
} DETECT_DOWN_START_POINT;

/* Pulse Output Method					*/
typedef enum _PULSE_OUTPUT
{
	OneHighLowHigh   = 0x0,				// 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
	OneHighHighLow   = 0x1,				// 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
	OneLowLowHigh    = 0x2,				// 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
	OneLowHighLow    = 0x3,				// 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
	TwoCcwCwHigh     = 0x4,				// 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High	 
	TwoCcwCwLow      = 0x5,				// 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low	 
	TwoCwCcwHigh     = 0x6,				// 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
	TwoCwCcwLow      = 0x7				// 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
} PULSE_OUTPUT;

/* Detect Destination Signal			*/
typedef enum _DETECT_DESTINATION_SIGNAL
{
	PElmNegativeEdge    = 0x0,			// +Elm(End limit) 하강 edge
	NElmNegativeEdge    = 0x1,			// -Elm(End limit) 하강 edge
	PSlmNegativeEdge    = 0x2,			// +Slm(Slowdown limit) 하강 edge
	NSlmNegativeEdge    = 0x3,			// -Slm(Slowdown limit) 하강 edge
	In0DownEdge         = 0x4,			// IN0(ORG) 하강 edge
	In1DownEdge         = 0x5,			// IN1(Z상) 하강 edge
	In2DownEdge         = 0x6,			// IN2(범용) 하강 edge
	In3DownEdge         = 0x7,			// IN3(범용) 하강 edge
	PElmPositiveEdge    = 0x8,			// +Elm(End limit) 상승 edge
	NElmPositiveEdge    = 0x9,			// -Elm(End limit) 상승 edge
	PSlmPositiveEdge    = 0xa,			// +Slm(Slowdown limit) 상승 edge
	NSlmPositiveEdge    = 0xb,			// -Slm(Slowdown limit) 상승 edge
	In0UpEdge           = 0xc,			// IN0(ORG) 상승 edge
	In1UpEdge           = 0xd,			// IN1(Z상) 상승 edge
	In2UpEdge           = 0xe,			// IN2(범용) 상승 edge
	In3UpEdge           = 0xf			// IN3(범용) 상승 edge
} DETECT_DESTINATION_SIGNAL;

/* Mode2 Data   
 * External Counter Input 
 */
typedef enum _EXTERNAL_COUNTER_INPUT
{
	UpDownMode = 0x0,					// Up/Down
	Sqr1Mode   = 0x1,					// 1체배
	Sqr2Mode   = 0x2,					// 2체배
	Sqr4Mode   = 0x3					// 4체배
} EXTERNAL_COUNTER_INPUT;

typedef enum _INP_SIGNAL_ACTIVE_LEVEL
{
    InpActiveLow,
    InpActiveHigh
} INP_SIGNAL_ACTIVE_LEVEL;

typedef enum _ALM_SIGNAL_ACTIVE_LEVEL
{
    AlmActiveLow,
    AlmActiveHigh
} ALM_SIGNAL_ACTIVE_LEVEL;

typedef enum _NSLM_SIGNAL_ACTIVE_LEVEL
{
    NSlmActiveLow,
    NSlmActiveHigh
} NSLM_SIGNAL_ACTIVE_LEVEL;

typedef enum _PSLM_SIGNAL_ACTIVE_LEVEL
{
    PSlmActiveLow,
    PSlmActiveHigh
} PSLM_SIGNAL_ACTIVE_LEVEL;

typedef enum _NELM_SIGNAL_ACTIVE_LEVEL
{
    NElmActiveLow,
    NElmActiveHigh
} NELM_SIGNAL_ACTIVE_LEVEL;

typedef enum _PELM_SIGNAL_ACTIVE_LEVEL
{
    PElmActiveLow,
    PElmActiveHigh
} PELM_SIGNAL_ACTIVE_LEVEL;

/* Universal Input Detect Sensor		*/
typedef	enum _UNIVERSAL_SIGNAL
{
	US_OUT0 = 0x01,
	US_OUT1 = 0x02,
	US_OUT2 = 0x04,
	US_OUT3 = 0x08,
	US_IN0  = 0x10,
	US_IN1  = 0x20,
	US_IN2  = 0x40,
	US_IN3  = 0x80
} UNIVERSAL_SIGNAL;

/* BOARD SELECT							*/
#define BASE_ADDR			0
#define BOARD0_BASE_ADDR	0
#define BOARD1_BASE_ADDR	1
#define BOARD2_BASE_ADDR	2
#define BOARD3_BASE_ADDR	3
#define BOARD4_BASE_ADDR	4
#define BOARD5_BASE_ADDR	5
#define BOARD6_BASE_ADDR	6
#define BOARD7_BASE_ADDR	7

/* CAMC CHIP SELECT						*/
typedef enum _CAMC_CHIP_ADDR
{
	CCA_CAMC0_ADDR	=	0x00,
	CCA_CAMC1_ADDR	=	0x10,
	CCA_CAMC2_ADDR	=	0x20,
	CCA_CAMC3_ADDR	=	0x30,
	CCA_CAMC4_ADDR	=	0x40,
	CCA_CAMC5_ADDR	=	0x50,
	CCA_CAMC6_ADDR	=	0x60,
	CCA_CAMC7_ADDR	=	0x70
} CAMC_CHIP_ADDR;

/* CHIP SELECT		*/
typedef enum _CHIP_SELECT
{
	CS_CAMC0 = 0x00,
	CS_CAMC1,
	CS_CAMC2,
	CS_CAMC3,
	CS_CAMC4,
	CS_CAMC5,
	CS_CAMC6,
	CS_CAMC7,
	CS_CAMC8,
	CS_CAMC9,
	CS_CAMC10,
	CS_CAMC11,
	CS_CAMC12,
	CS_CAMC13,
	CS_CAMC14,
	CS_CAMC15,
	CS_CAMC16,
	CS_CAMC17,
	CS_CAMC18,
	CS_CAMC19,
	CS_CAMC20,
	CS_CAMC21,
	CS_CAMC22,
	CS_CAMC23,
	CS_CAMC24,
	CS_CAMC25,
	CS_CAMC26,
	CS_CAMC27,
	CS_CAMC28,
	CS_CAMC29,
	CS_CAMC30,
	CS_CAMC31
} CHIP_SELECT;

/*	AMCS Board 추가		*/
/*	2000년 12월 16일	*/
/*	작성자 : 이성재		*/
typedef enum _BOARD_SELECT {
	AMC1X	= 0x01,
	AMC2X	= 0x02,
	AMC3X	= 0x03,
	AMC4X	= 0x04,
	AMC6X	= 0x06,
	AMC8X	= 0x08
} BOARD_SELECT;

typedef struct _AXIS_INFO {
	INT16  	nBoardNo;
	CAMC_CHIP_ADDR	nAxis;
	INT16  	ModuleID;
	INT16  	ModuleVersion;				//<+> 2004-03-09 안병건 추가
	INT16  	nModuleNo;
} AXIS_INFO, *PAXIS_INFO;

/*----------------------------------------------------------------------*/
/*						칩 초기화 구조체								*/
/*----------------------------------------------------------------------*/
typedef	struct _BOARD_INFO
{
	INT16  nBoardNumber;
	INT16  uBoardAddress;
	INT16  uBoardInterrupt;
} BOARD_INFO, *PBOARD_INFO;

/*----------------------------------------------------------------------*/
/*						이동방향										*/
/*----------------------------------------------------------------------*/
typedef enum	_MOVE_DIRECTION
{
	MoveLeft	= -1,
	MoveRight	= 1
} MOVE_DIRECTION;

/*  End status : 0x0000이면 정상종료	*/
typedef	enum _END_STATUS
{
	END_STATUS_SLM						= 0x0001,			// Bit 0, limit 감속정지 신호 입력에 의한 종료
	END_STATUS_ELM						= 0x0002,			// Bit 1, limit 급정지 신호 입력에 의한 종료
	END_STATUS_SSTOP_SIGNAL				= 0x0004,			// Bit 2, 감속 정지 신호 입력에 의한 종료
	END_STATUS_ESTOP_SIGANL				= 0x0008,			// Bit 3, 급정지 신호 입력에 의한 종료
	END_STATUS_SSTOP_COMMAND			= 0x0010,			// Bit 4, 감속 정지 명령에 의한 종료
	END_STATUS_ESTOP_COMMAND			= 0x0020,			// Bit 5, 급정지 정지 명령에 의한 종료
	END_STATUS_ALARM_SIGNAL				= 0x0040,			// Bit 6, Alarm 신호 입력에 희한 종료
	END_STATUS_DATA_ERROR				= 0x0080			// Bit 7, 데이터 설정 에러에 의한 종료
} END_STATUS;

/*  Drive status						*/
typedef	enum _DRIVE_STATUS
{
	DRIVE_STATUS_BUSY					= 0x0001,			// Bit 0, BUSY(드라이브 구동 중)
	DRIVE_STATUS_DOWN					= 0x0002,			// Bit 1, DOWN(감속 중)
	DRIVE_STATUS_CONST					= 0x0004,			// Bit 2, CONST(등속 중)
	DRIVE_STATUS_UP						= 0x0008,			// Bit 3, UP(가속 중)
	DRIVE_STATUS_ICL					= 0x0010,			// Bit 4, ICL(내부 위치 카운터 < 내부 위치 카운터 비교값)
	DRIVE_STATUS_ICG					= 0x0020,			// Bit 5, ICG(내부 위치 카운터 > 내부 위치 카운터 비교값)
	DRIVE_STATUS_ECL					= 0x0040,			// Bit 6, ECL(외부 위치 카운터 < 외부 위치 카운터 비교값)
	DRIVE_STATUS_ECG					= 0x0080			// Bit 7, ECG(외부 위치 카운터 > 외부 위치 카운터 비교값)
} DRIVE_STATUS;

/*  Mechanical signal					*/
typedef	enum _MECHANICAL_SIGNAL
{
	MECHANICAL_PELM_LEVEL				= 0x0001,			// Bit 0, +Limit 급정지 신호 입력 Level
	MECHANICAL_NELM_LEVEL				= 0x0002,			// Bit 1, -Limit 급정지 신호 입력 Level
	MECHANICAL_PSLM_LEVEL				= 0x0004,			// Bit 2, +limit 감속정지 신호 입력 Level
	MECHANICAL_NSLM_LEVEL				= 0x0008,			// Bit 3, -limit 감속정지 신호 입력 Level
	MECHANICAL_ALARM_LEVEL				= 0x0010,			// Bit 4, Alarm 신호 입력 Level
	MECHANICAL_INP_LEVEL				= 0x0020,			// Bit 5, Inposition 신호 입력 Level
	MECHANICAL_ENC_DOWN_LEVEL			= 0x0040,			// Bit 6, 엔코더 DOWN(B상) 신호 입력 Level
	MECHANICAL_ENC_UP_LEVEL				= 0x0080			// Bit 7, 엔코더 UP(A상) 신호 입력 Level
} MECHANICAL_SIGNAL;

#endif	// __COMMON_CAMC_CHIP_DEFINE_H__
