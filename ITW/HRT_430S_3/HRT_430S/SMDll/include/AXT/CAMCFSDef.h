#ifndef	__CAMC_FS_DEFINE_H__
#define	__CAMC_FS_DEFINE_H__

/*-------------------------------------------------------------------------------------------------*
 *        CAMC-FS (SMC-1V02, SMC-2V02)                                                             *
 *-------------------------------------------------------------------------------------------------*/


#ifndef __COMMON_CAMC_CHIP_DEFINE_H__
#define __COMMON_CAMC_CHIP_DEFINE_H__

/*-------------------------------------------------------------------------------------------------*
 *        CAMC-5M, CAMC-FS 1.0 / 2.0에서 공통으로 사용하는 매크로...                               *
 *-------------------------------------------------------------------------------------------------*/


#ifndef POSITIVE_SENSE
#define POSITIVE_SENSE	1
#define NEGATIVE_SENSE	-1	
#endif

// 2004년 3월 8일 안병건 추가
#ifndef SYNC_SET
#define MASTER	1
#define SLAVE	0
#endif

// 2004년 3월 9일 안병건 추가
#ifndef UI_MODE
#define UI4		0
#define UI5		0
#define JOG		1
#define MARK	2
#endif

typedef enum _SP_MODE
{
	DRIVE1	= 1,
	DRIVE2	= 2,
	DRIVE3	= 3
} SP_MODE;

typedef enum _INPUT_METHOD
{
	DIFF_INPUT	= 0x00,					// Differential input
	LEVEL_INPUT	= 0x01					// Level input
} INPUT_METHOD;

typedef enum _INPUT_MODE
{
	Phase		= 0x0,					// 단상
	Mode1		= 0x1,					// 1체배
	Mode2		= 0x2,					// 2체배
	Mode4		= 0x3					// 4체배
} INPUT_MODE;

/* Main clock							*/
#define F_33M_CLK						33000000L			/* 33.000 MHz */
#define F_32_768M_CLK					32768000L			/* 32.768 MHz */
#define	F_20M_CLK						20000000L			/* 20.000 MHz */
#define	F_16_384M_CLK					16384000L			/* 16.384 MHz : Default */


/* MODE1 DATA							
 *
 *	감속 개시 POINT 검출 방식
 */
typedef enum _FSDETECT_DOWN_START_POINT
{
	AutoDetect  = 0x0,
	RestPulse   = 0x1
} FSDETECT_DOWN_START_POINT;

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

/* Universal Input/Output				*/
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
	INT16  	ModuleVersion;				//<+> 2003-01-16 JNS
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

#endif	// __COMMON_CAMC_CHIP_DEFINE_H__


///////////////////////////////////////////////////////
// 2005/08/11 김민호 수정
// CAMC5MDef.h 와의 충돌에 의해 위치 변경
typedef enum _MPG_MODE
{
	SLAVE_MODE		= 1,
	PRST_DRV_MODE	= 2,
	CONT_DRV_MODE	= 4
} MPG_MODE;
////////////////////////////////////////////////////////

/* Write port							*/
typedef enum _FSPORT_DATA_WRITE
{
	FsData1Write	= 0x00,
	FsData2Write	= 0x01,
	FsData3Write	= 0x02,
	FsData4Write	= 0x03,
	FsCommandWrite	= 0x04
} FSPORT_DATA_WRITE;

/* Read port							*/
typedef enum _FSPORT_DATA_READ
{
	FsData1Read		= 0x00,
	FsData2Read		= 0x01,
	FsData3Read		= 0x02,
	FsData4Read		= 0x03,
	FsCommandRead	= 0x04
} FSPORT_DATA_READ;

/* FS Universal Input/Output			*/
typedef	enum _FSUNIVERSAL_SIGNAL
{
	FSUS_OUT0							= 0x0001,			// Bit 0
	FSUS_SVON							= 0x0001,			// Bit 0, Servo ON
	FSUS_OUT1							= 0x0002,			// Bit 1
	FSUS_ALMC							= 0x0002,			// Bit 1, Alarm Clear
	FSUS_OUT2							= 0x0004,			// Bit 2
	FSUS_OUT3							= 0x0008,			// Bit 3
	FSUS_IN0							= 0x0010,			// Bit 4
	FSUS_ORG							= 0x0010,			// Bit 4, Origin
	FSUS_IN1							= 0x0020,			// Bit 5
	FSUS_PZ								= 0x0020,			// Bit 5, Encoder Z상
	FSUS_IN2							= 0x0040,			// Bit 6
	FSUS_IN3							= 0x0080,			// Bit 7

	// [V2.0이상]
	FSUS_OPCODE0						= 0x0100,			// Bit 8
	FSUS_OPCODE1						= 0x0200,			// Bit 9
	FSUS_OPCODE2						= 0x0400,			// Bit 10
	FSUS_OPDATA0						= 0x0800,			// Bit 11
	FSUS_OPDATA1						= 0x1000,			// Bit 12
	FSUS_OPDATA2						= 0x2000,			// Bit 13
	FSUS_OPDATA3						= 0x4000			// Bit 14
} FSUNIVERSAL_SIGNAL;

/* FS End status : 0x0000이면 정상종료	*/
typedef	enum _FSEND_STATUS
{
	FSEND_STATUS_SLM					= 0x0001,			// Bit 0, limit 감속정지 신호 입력에 의한 종료
	FSEND_STATUS_ELM					= 0x0002,			// Bit 1, limit 급정지 신호 입력에 의한 종료
	FSEND_STATUS_SSTOP_SIGNAL			= 0x0004,			// Bit 2, 감속 정지 신호 입력에 의한 종료
	FSEND_STATUS_ESTOP_SIGANL			= 0x0008,			// Bit 3, 급정지 신호 입력에 의한 종료
	FSEND_STATUS_SSTOP_COMMAND			= 0x0010,			// Bit 4, 감속 정지 명령에 의한 종료
	FSEND_STATUS_ESTOP_COMMAND			= 0x0020,			// Bit 5, 급정지 정지 명령에 의한 종료
	FSEND_STATUS_ALARM_SIGNAL			= 0x0040,			// Bit 6, Alarm 신호 입력에 희한 종료
	FSEND_STATUS_DATA_ERROR				= 0x0080,			// Bit 7, 데이터 설정 에러에 의한 종료

	//[V2.0이상]
	FSEND_STATUS_DEVIATION_ERROR		= 0x0100,			// Bit 8, 탈조 에러에 의한 종료
	FSEND_STATUS_ORIGIN_DETECT			= 0x0200,			// Bit 9, 원점 검출에 의한 종료
	FSEND_STATUS_SIGNAL_DETECT			= 0x0400,			// Bit 10, 신호 검출에 의한 종료(Signal search-1/2 drive 종료)
	FSEND_STATUS_PRESET_PULSE_DRIVE		= 0x0800,			// Bit 11, Preset pulse drive 종료
	FSEND_STATUS_SENSOR_PULSE_DRIVE		= 0x1000,			// Bit 12, Sensor pulse drive 종료
	FSEND_STATUS_LIMIT					= 0x2000,			// Bit 13, Limit 완전정지에 의한 종료
	FSEND_STATUS_SOFTLIMIT				= 0x4000			// Bit 14, Soft limit에 의한 종료
} FSEND_STATUS;

/* FS Drive status						*/
typedef	enum _FSDRIVE_STATUS
{
	FSDRIVE_STATUS_BUSY					= 0x0001,			// Bit 0, BUSY(드라이브 구동 중)
	FSDRIVE_STATUS_DOWN					= 0x0002,			// Bit 1, DOWN(감속 중)
	FSDRIVE_STATUS_CONST				= 0x0004,			// Bit 2, CONST(등속 중)
	FSDRIVE_STATUS_UP					= 0x0008,			// Bit 3, UP(가속 중)
	FSDRIVE_STATUS_ICL					= 0x0010,			// Bit 4, ICL(내부 위치 카운터 < 내부 위치 카운터 비교값)
	FSDRIVE_STATUS_ICG					= 0x0020,			// Bit 5, ICG(내부 위치 카운터 > 내부 위치 카운터 비교값)
	FSDRIVE_STATUS_ECL					= 0x0040,			// Bit 6, ECL(외부 위치 카운터 < 외부 위치 카운터 비교값)
	FSDRIVE_STATUS_ECG					= 0x0080,			// Bit 7, ECG(외부 위치 카운터 > 외부 위치 카운터 비교값)

	//[V2.0이상]
	FSDRIVE_STATUS_DEVIATION_ERROR		= 0x0100			// Bit 8, 드라이브 방향 신호(0=CW/1=CCW)
} FSDRIVE_STATUS;

/* FS Mechanical signal					*/
typedef	enum _FSMECHANICAL_SIGNAL
{
	FSMECHANICAL_PELM_LEVEL				= 0x0001,			// Bit 0, +Limit 급정지 신호 입력 Level
	FSMECHANICAL_NELM_LEVEL				= 0x0002,			// Bit 1, -Limit 급정지 신호 입력 Level
	FSMECHANICAL_PSLM_LEVEL				= 0x0004,			// Bit 2, +limit 감속정지 신호 입력 Level
	FSMECHANICAL_NSLM_LEVEL				= 0x0008,			// Bit 3, -limit 감속정지 신호 입력 Level
	FSMECHANICAL_ALARM_LEVEL			= 0x0010,			// Bit 4, Alarm 신호 입력 Level
	FSMECHANICAL_INP_LEVEL				= 0x0020,			// Bit 5, Inposition 신호 입력 Level
	FSMECHANICAL_ENC_DOWN_LEVEL			= 0x0040,			// Bit 6, 엔코더 DOWN(B상) 신호 입력 Level
	FSMECHANICAL_ENC_UP_LEVEL			= 0x0080,			// Bit 7, 엔코더 UP(A상) 신호 입력 Level

	//[V2.0이상]
	FSMECHANICAL_EXMP_LEVEL				= 0x0100,			// Bit 8, EXMP 신호 입력 Level
	FSMECHANICAL_EXPP_LEVEL				= 0x0200,			// Bit 9, EXPP 신호 입력 Level
	FSMECHANICAL_MARK_LEVEL				= 0x0400,			// Bit 10, MARK# 신호 입력 Level
	FSMECHANICAL_SSTOP_LEVEL			= 0x0800,			// Bit 11, SSTOP 신호 입력 Level
	FSMECHANICAL_ESTOP_LEVEL			= 0x1000,			// Bit 12, ESTOP 신호 입력 Level
} FSMECHANICAL_SIGNAL;



/* 드라이브 동작 설정					*/
typedef enum _FSDRIVE_OPERATION
{
	SYM_LINEAR							= 0x00,				// 대칭 사다리꼴
	ASYM_LINEAR							= 0x01,				// 비대칭 사다리꼴
	SYM_CURVE							= 0x02,				// 대칭 포물선(S-Curve)
	ASYM_CURVE							= 0x03				// 비대칭 포물선(S-Curve)
} FSDRIVE_OPERATION;

/* FS COMMAND LIST							*/
typedef enum _FSCOMMAND
{
    // PGM-1 Group Register
    FsRangeDataRead						= 0x00,				// PGM-1 RANGE READ, 16bit, 0xFFFF
    FsRangeDataWrite					= 0x80,				// PGM-1 RANGE WRITE
    FsStartStopSpeedDataRead			= 0x01,				// PGM-1 START/STOP SPEED DATA READ, 16bit, 
    FsStartStopSpeedDataWrite			= 0x81,				// PGM-1 START/STOP SPEED DATA WRITE
    FsObjectSpeedDataRead				= 0x02,				// PGM-1 OBJECT SPEED DATA READ, 16bit, 
    FsObjectSpeedDataWrite				= 0x82,				// PGM-1 OBJECT SPEED DATA WRITE
    FsRate1DataRead						= 0x03,				// PGM-1 RATE-1 DATA READ, 16bit, 0xFFFF
    FsRate1DataWrite					= 0x83,				// PGM-1 RATE-1 DATA WRITE
    FsRate2DataRead						= 0x04,				// PGM-1 RATE-2 DATA READ, 16bit, 0xFFFF
    FsRate2DataWrite					= 0x84,				// PGM-1 RATE-2 DATA WRITE
    FsRate3DataRead						= 0x05,				// PGM-1 RATE-3 DATA READ, 16bit, 0xFFFF
    FsRate3DataWrite					= 0x85,				// PGM-1 RATE-3 DATA WRITE
    FsRateChangePoint12Read				= 0x06,				// PGM-1 RATE CHANGE POINT 1-2 READ, 16bit, 0xFFFF
    FsRateChangePoint12Write			= 0x86,				// PGM-1 RATE CHANGE POINT 1-2 WRITE
    FsRateChangePoint23Read				= 0x07,				// PGM-1 RATE CHANGE POINT 2-3 READ, 16bit, 0xFFFF
    FsRateChangePoint23Write			= 0x87,				// PGM-1 RATE CHANGE POINT 2-3 WRITE
    FsSw1DataRead						= 0x08,				// PGM-1 SW-1 DATA READ, 15bit, 0x7FFF
    FsSw1DataWrite						= 0x88,				// PGM-1 SW-1 DATA WRITE
    FsSw2DataRead						= 0x09,				// PGM-1 SW-2 DATA READ, 15bit, 0x7FFF
    FsSw2DataWrite						= 0x89,				// PGM-1 SW-2 DATA WRITE
    FsPwmOutDataRead					= 0x0A,				// PGM-1 PWM 출력 설정 DATA READ(0~6), 3bit, 0x00
    FsPwmOutDataWrite					= 0x8A,				// PGM-1 PWM 출력 설정 DATA WRITE
    FsSlowDownRearPulseRead				= 0x0B,				// PGM-1 SLOW DOWN/REAR PULSE READ, 32bit, 0x00000000
    FsSlowDownRearPulseWrite			= 0x8B,				// PGM-1 SLOW DOWN/REAR PULSE WRITE
    FsCurrentSpeedDataRead				= 0x0C,				// PGM-1 현재 SPEED DATA READ, 16bit, 0x0000
	FsNoOperation_8C					= 0x8C,				// No operation
    FsCurrentSpeedComparateDataRead		= 0x0D,				// PGM-1 현재 SPEED 비교 DATA READ, 16bit, 0x0000
    FsCurrentSpeedComparateDataWrite	= 0x8D,				// PGM-1 현재 SPEED 비교 DATA WRITE
    FsDrivePulseCountRead				= 0x0E,				// PGM-1 DRIVE PULSE COUNTER READ, 32bit, 0x00000000
	FsNoOperation_8E					= 0x8E,				// No operation
    FsPresetPulseDataRead				= 0x0F,				// PGM-1 PRESET PULSE DATA READ, 32bit, 0x00000000
	FsNoOperation_8F					= 0x8F,				// No operation

	// PGM-1 Update Group Register
    FsURangeDataRead					= 0x10,				// PGM-1 UP-DATE RANGE READ, 16bit, 0xFFFF
    FsURangeDataWrite					= 0x90,				// PGM-1 UP-DATE RANGE WRITE
    FsUStartStopSpeedDataRead			= 0x11,				// PGM-1 UP-DATE START/STOP SPEED DATA READ, 16bit, 
    FsUStartStopSpeedDataWrite			= 0x91,				// PGM-1 UP-DATE START/STOP SPEED DATA WRITE
    FsUObjectSpeedDataRead				= 0x12,				// PGM-1 UP-DATE OBJECT SPEED DATA READ, 16bit, 
    FsUObjectSpeedDataWrite				= 0x92,				// PGM-1 UP-DATE OBJECT SPEED DATA WRITE
    FsURate1DataRead					= 0x13,				// PGM-1 UP-DATE RATE-1 DATA READ, 16bit, 0xFFFF
    FsURate1DataWrite					= 0x93,				// PGM-1 UP-DATE RATE-1 DATA WRITE
    FsURate2DataRead					= 0x14,				// PGM-1 UP-DATE RATE-2 DATA READ, 16bit, 0xFFFF
    FsURate2DataWrite					= 0x94,				// PGM-1 UP-DATE RATE-2 DATA WRITE
    FsURate3DataRead					= 0x15,				// PGM-1 UP-DATE RATE-3 DATA READ, 16bit, 0xFFFF
    FsURate3DataWrite					= 0x95,				// PGM-1 UP-DATE RATE-3 DATA WRITE
    FsURateChange12DataRead				= 0x16,				// PGM-1 UP-DATE RATE CHANGE POINT 1-2 READ, 16bit, 0xFFFF
    FsURateChange12DataWrite			= 0x96,				// PGM-1 UP-DATE RATE CHANGE POINT 1-2 WRITE
    FsURateChange23DataRead				= 0x17,				// PGM-1 UP-DATE RATE CHANGE POINT 2-3 READ, 16bit, 0xFFFF
    FsURateChange23DataWrite			= 0x97,				// PGM-1 UP-DATE RATE CHANGE POINT 2-3 WRITE
    FsUSw1DataRead						= 0x18,				// PGM-1 UP-DATE SW-1 DATA READ, 15bit, 0x7FFF
    FsUSw1DataWrite						= 0x98,				// PGM-1 UP-DATE SW-1 DATA WRITE
    FsUSw2DataRead						= 0x19,				// PGM-1 UP-DATE SW-2 DATA READ, 15bit, 0x7FFF
    FsUSw2DataWrite						= 0x99,				// PGM-1 UP-DATE SW-2 DATA WRITE
    FsUCurrentSpeedChangeDataRead		= 0x1A,				// PGM-1 CURRENT SPEED CHANGE DATA READ
    FsUCurrentSpeedChangeDataWrote		= 0x9A,				// PGM-1 CURRENT SPEED CHANGE DATA WRITE
    FsUSlowDownRearPulseRead			= 0x1B,				// PGM-1 UP-DATE SLOW DOWN/REAR PULSE READ, 32bit, 0x00000000
    FsUSlowDownRearPulseWrite			= 0x9B,				// PGM-1 UP-DATE SLOW DOWN/REAR PULSE WRITE
    FsUCurrentSpeedDataRead				= 0x1C,				// PGM-1 현재 SPEED DATA READ, 16bit, 0x0000
	FsNoOperation_9C					= 0x9C,				// No operation
    FsUCurrentSpeedComparateDataRead	= 0x1D,				// PGM-1 UP-DATE 현재 SPEED 비교 DATA READ, 16bit, 0x0000
    FsUCurrentSpeedComparateDataWrite	= 0x9D,				// PGM-1 UP-DATE 현재 SPEED 비교 DATA WRITE
    FsUDrivePulseCounterDataRead		= 0x1E,				// PGM-1 DRIVE PULSE COUNTER READ, 32bit, 0x00000000
	FsNoOperation_9E					= 0x9E,				// No operation
    FsUPresetPulseDataRead				= 0x1F,				// PGM-1 UP-DATE PRESET PULSE DATA READ, 32bit, 0x00000000
	FsNoOperation_9F					= 0x9F,				// No operation

	// PGM-2 Group Register
	FsNoOperation_20					= 0x20,				// No operation
    FsPresetPulseDriveP					= 0xA0,				// +PRESET PULSE DRIVE, 32
	FsNoOperation_21					= 0x21,				// No operation
    FsContinuousDriveP					= 0xA1,				// +CONTINUOUS DRIVE
	FsNoOperation_22					= 0x22,				// No operation
    FsSignalSearch1DriveP				= 0xA2,				// +SIGNAL SEARCH-1 DRIVE
	FsNoOperation_23					= 0x23,				// No operation
    FsSignalSearch2DriveP				= 0xA3,				// +SIGNAL SEARCH-2 DRIVE
	FsNoOperation_24					= 0x24,				// No operation
    FsOriginSearchDriveP				= 0xA4,				// +ORIGIN(원점) SEARCH DRIVE
	FsNoOperation_25					= 0x25,				// No operation
    FsPresetPulseDriveN					= 0xA5,				// -PRESET PULSE DRIVE, 32
	FsNoOperation_26					= 0x26,				// No operation
    FsContinuousDriveN					= 0xA6,				// -CONTINUOUS DRIVE
	FsNoOperation_27					= 0x27,				// No operation
    FsSignalSearch1DriveN				= 0xA7,				// -SIGNAL SEARCH-1 DRIVE
	FsNoOperation_28					= 0x28,				// No operation
    FsSignalSearch2DriveN				= 0xA8,				// -SIGNAL SEARCH-2 DRIVE
	FsNoOperation_29					= 0x29,				// No operation
    FsOriginSearchDriveN				= 0xA9,				// -ORIGIN(원점) SEARCH DRIVE
	FsNoOperation_2A					= 0x2A,				// No operation
    FsPresetPulseDataOverride			= 0xAA,				// PRESET PULSE DATA OVERRIDE(ON_BUSY)
	FsNoOperation_2B					= 0x2B,				// No operation
    FsSlowDownStop						= 0xAB,				// SLOW DOWN STOP
	FsNoOperation_2C					= 0x2C,				// No operation
    FsEmergencyStop						= 0xAC,				// EMERGENCY STOP
    FsDriveOperationSelectDataRead		= 0x2D,				// 드라이브 동작 설정 DATA READ
    FsDriveOperationSelectDataWrite		= 0xAD,				// 드라이브 동작 설정 DATA WRITE
    FsMpgOperationSettingDataRead		= 0x2E,				// MPG OPERATION SETTING DATA READ, 3bit, 0x00		<+> 2002-11-15 FS2.0 - JNS
    FsMpgOperationSettingDataWrite		= 0xAE,				// MPG OPERATION SETTING DATA WRITE					<+> 2002-11-15 FS2.0 - JNS
    FsMpgPresetPulseDataRead			= 0x2F,				// MPG PRESET PULSE DATA READ, 32bit, 0x00000000	<+> 2002-11-15 FS2.0 - JNS
    FsMpgPresetPulseDataWrite			= 0xAF,				// MPG PRESET PULSE DATA WRITE						<+> 2002-11-15 FS2.0 - JNS
    
	/* Extension Group Register */
	FsNoOperation_30					= 0x30,				// No operation
    FsSensorPositioningDrive1P			= 0xB0,				// +SENSOR POSITIONING DRIVE I
	FsNoOperation_31					= 0x31,				// No operation
    FsSensorPositioningDrive1N			= 0xB1,				// -SENSOR POSITIONING DRIVE I
	FsNoOperation_32					= 0x32,				// No operation
    FsSensorPositioningDrive2P			= 0xB2,				// +SENSOR POSITIONING DRIVE II
	FsNoOperation_33					= 0x33,				// No operation
    FsSensorPositioningDrive2N			= 0xB3,				// -SENSOR POSITIONING DRIVE II
	FsNoOperation_34					= 0x34,				// No operation
    FsSensorPositioningDrive3P			= 0xB4,				// +SENSOR POSITIONING DRIVE III
	FsNoOperation_35					= 0x35,				// No operation
    FsSensorPositioningDrive3N			= 0xB5,				// -SENSOR POSITIONING DRIVE III
    FsSoftlimitSettingDataRead			= 0x36,				// SOFT LIMIT 설정 READ, 3bit, 0x00
    FsSoftlimitSettingDataWrite			= 0xB6,				// SOFT LIMIT 설정 WRITE
    FsNegativeSoftlimitDataRead			= 0x37,				// -SOFT LIMIT 비교 레지스터 설정 READ, 32bit, 0x80000000
    FsNegativeSoftlimitDataWrite		= 0xB7,				// -SOFT LIMIT 비교 레지스터 설정 WRITE
    FsPositiveSoftlimitDataRead			= 0x38,				// +SOFT LIMIT 비교 레지스터 설정 READ, 32bit, 0x7FFFFFFF
    FsPositiveSoftlimitDataWrite		= 0xB8,				// +SOFT LIMIT 비교 레지스터 설정 WRITE
    FsTriggerModeSettingDataRead		= 0x39,				// TRIGGER MODE 설정 READ, 32bit, 0x00010000
    FsTriggerModeSettingDataWrite		= 0xB9,				// TRIGGER MODE 설정 WRITE
    FsTriggerComparatorDataRead			= 0x3A,				// TRIGGER 비교 데이터 설정 READ, 32bit, 0x00000000
    FsTriggerComparatorDataWrite		= 0xBA,				// TRIGGER 비교 데이터 설정 WRITE
    FsInternalCounterMDataRead			= 0x3B,				// INTERNAL M-DATA 설정 READ, 32bit, 0x80000000
    FsInternalCounterMDataWrite			= 0xBB,				// INTERNAL M-DATA 설정 WRITE
    FsExternalCounterMDataRead			= 0x3C,				// EXTERNAL M-DATA 설정 READ, 32bit, 0x80000000
    FsExternalCounterMDataWrite			= 0xBC,				// EXTERNAL M-DATA 설정 WRITE
	FsNoOperation_BD					= 0xBD,				// No operation
	FsNoOperation_3D					= 0x3D,				// No operation
	FsNoOperation_3E					= 0x3E,				// No operation
	FsNoOperation_BE					= 0xBE,				// No operation
	FsNoOperation_3F					= 0x3F,				// No operation
	FsNoOperation_BF					= 0xBF,				// No operation
   	
	/* Scripter Group Register			*/
    FsScriptOperSetReg1Read				= 0x40,				// 스크립트 동작 설정 레지스터-1 READ, 32bit, 0x00000000
    FsScriptOperSetReg1Write			= 0xC0,				// 스크립트 동작 설정 레지스터-1 WRITE
    FsScriptOperSetReg2Read				= 0x41,				// 스크립트 동작 설정 레지스터-2 READ, 32bit, 0x00000000
    FsScriptOperSetReg2Write			= 0xC1,				// 스크립트 동작 설정 레지스터-2 WRITE
    FsScriptOperSetReg3Read				= 0x42,				// 스크립트 동작 설정 레지스터-3 READ, 32bit, 0x00000000 
    FsScriptOperSetReg3Write			= 0xC2,				// 스크립트 동작 설정 레지스터-3 WRITE
    FsScriptOperSetRegQueueRead			= 0x43,				// 스크립트 동작 설정 레지스터-Queue READ, 32bit, 0x00000000
    FsScriptOperSetRegQueueWrite		= 0xC3,				// 스크립트 동작 설정 레지스터-Queue WRITE
    FsScriptOperDataReg1Read			= 0x44,				// 스크립트 동작 데이터 레지스터-1 READ, 32bit, 0x00000000 
    FsScriptOperDataReg1Write			= 0xC4,				// 스크립트 동작 데이터 레지스터-1 WRITE
    FsScriptOperDataReg2Read			= 0x45,				// 스크립트 동작 데이터 레지스터-2 READ, 32bit, 0x00000000 
    FsScriptOperDataReg2Write			= 0xC5,				// 스크립트 동작 데이터 레지스터-2 WRITE
    FsScriptOperDataReg3Read			= 0x46,				// 스크립트 동작 데이터 레지스터-3 READ, 32bit, 0x00000000 
    FsScriptOperDataReg3Write			= 0xC6,				// 스크립트 동작 데이터 레지스터-3 WRITE
    FsScriptOperDataRegQueueRead		= 0x47,				// 스크립트 동작 데이터 레지스터-Queue READ, 32bit, 0x00000000 
    FsScriptOperDataRegQueueWrite		= 0xC7,				// 스크립트 동작 데이터 레지스터-Queue WRITE
	FsNoOperation_48					= 0x48,				// No operation
    FsScriptOperQueueClear				= 0xC8,				// 스크립트 Queue clear
    FsScriptOperSetQueueIndexRead		= 0x49,				// 스크립트 동작 설정 Queue 인덱스 READ, 4bit, 0x00
	FsNoOperation_C9					= 0xC9,				// No operation
    FsScriptOperDataQueueIndexRead		= 0x4A,				// 스크립트 동작 데이터 Queue 인덱스 READ, 4bit, 0x00
	FsNoOperation_CA					= 0xCA,				// No operation
    FsScriptOperQueueFlagRead			= 0x4B,				// 스크립트 Queue Full/Empty Flag READ, 4bit, 0x05
	FsNoOperation_CB					= 0xCB,				// No operation
    FsScriptOperQueueSizeSettingRead	= 0x4C,				// 스크립트 Queue size 설정(0~13) READ, 16bit, 0xD0D0
    FsScriptOperQueueSizeSettingWrite	= 0xCC,				// 스크립트 Queue size 설정(0~13) WRITE
    FsScriptOperQueueStatusRead			= 0x4D,				// 스크립트 Queue status READ, 12bit, 0x005
	FsNoOperation_CD					= 0xCD,				// No operation
	FsNoOperation_4E					= 0x4E,				// No operation
	FsNoOperation_CE					= 0xCE,				// No operation
	FsNoOperation_4F					= 0x4F,				// No operation
	FsNoOperation_CF					= 0xCF,				// No operation

	/* Caption Group Register */
    FsCaptionOperSetReg1Read			= 0x50,				// 갈무리 동작 설정 레지스터-1 READ, 32bit, 0x00000000
    FsCaptionOperSetReg1Write			= 0xD0,				// 갈무리 동작 설정 레지스터-1 WRITE
    FsCaptionOperSetReg2Read			= 0x51,				// 갈무리 동작 설정 레지스터-2 READ, 32bit, 0x00000000
    FsCaptionOperSetReg2Write			= 0xD1,				// 갈무리 동작 설정 레지스터-2 WRITE
    FsCaptionOperSetReg3Read			= 0x52,				// 갈무리 동작 설정 레지스터-3 READ, 32bit, 0x00000000 
    FsCaptionOperSetReg3Write			= 0xD2,				// 갈무리 동작 설정 레지스터-3 WRITE
    FsCaptionOperSetRegQueueRead		= 0x53,				// 갈무리 동작 설정 레지스터-Queue READ, 32bit, 0x00000000
    FsCaptionOperSetRegQueueWrite		= 0xD3,				// 갈무리 동작 설정 레지스터-Queue WRITE
	FsCaptionOperDataReg1Read			= 0x54,				// 갈무리 동작 데이터 레지스터-1 READ, 32bit, 0x00000000 
	FsNoOperation_D4					= 0xD4,				// No operation
	FsCaptionOperDataReg2Read			= 0x55,				// 갈무리 동작 데이터 레지스터-2 READ, 32bit, 0x00000000 
	FsNoOperation_D5					= 0xD5,				// No operation
	FsCaptionOperDataReg3Read			= 0x56,				// 갈무리 동작 데이터 레지스터-3 READ, 32bit, 0x00000000 
	FsNoOperation_D6					= 0xD6,				// No operation
    FsCaptionOperDataRegQueueRead		= 0x57,				// 갈무리 동작 데이터 레지스터-Queue READ, 32bit, 0x00000000 
	FsNoOperation_D7					= 0xD7,				// No operation
	FsNoOperation_58					= 0x58,				// No operation
    FsCaptionOperQueueClear				= 0xD8,				// 갈무리 Queue clear
    FsCaptionOperSetQueueIndexRead		= 0x59,				// 갈무리 동작 설정 Queue 인덱스 READ, 4bit, 0x00
	FsNoOperation_D9					= 0xD9,				// No operation
    FsCaptionOperDataQueueIndexRead		= 0x5A,				// 갈무리 동작 데이터 Queue 인덱스 READ, 4bit, 0x00
	FsNoOperation_DA					= 0xDA,				// No operation
    FsCaptionOperQueueFlagRead			= 0x5B,				// 갈무리 Queue Full/Empty Flag READ, 4bit, 0x05
	FsNoOperation_DB					= 0xDB,				// No operation
    FsCaptionOperQueueSizeSettingRead	= 0x5C,				// 갈무리 Queue size 설정(0~13) READ, 16bit, 0xD0D0
    FsCaptionOperQueueSizeSettingWrite	= 0xDC,				// 갈무리 Queue size 설정(0~13) WRITE
    FsCaptionOperQueueStatusRead		= 0x5D,				// 갈무리 Queue status READ, 12bit, 0x005
	FsNoOperation_DD					= 0xDD,				// No operation
	FsNoOperation_5E					= 0x5E,				// No operation
	FsNoOperation_DE					= 0xDE,				// No operation
	FsNoOperation_5F					= 0x5F,				// No operation
	FsNoOperation_DF					= 0xDF,				// No operation

	/* BUS - 1 Group Register			*/
    FsInternalCounterRead				= 0x60,				// INTERNAL COUNTER DATA READ(Signed), 32bit, 0x00000000
    FsInternalCounterWrite				= 0xE0,				// INTERNAL COUNTER DATA WRITE(Signed)
    FsInternalCounterComparatorDataRead	= 0x61,				// INTERNAL COUNTER COMPARATE DATA READ(Signed), 32bit, 0x00000000
    FsInternalCounterComparatorDataWrite= 0xE1,				// INTERNAL COUNTER COMPARATE DATA WRITE(Signed)
    FsInternalCounterPreScaleDataRead	= 0x62,				// INTERNAL COUNTER PRE-SCALE DATA READ, 8bit, 0x00
    FsInternalCounterPreScaleDataWrite	= 0xE2,				// INTERNAL COUNTER PRE-SCALE DATA WRITE
    FsInternalCounterNCountDataRead		= 0x63,				// INTERNAL COUNTER P-DATA READ, 32bit, 0x7FFFFFFF
    FsInternalCounterNCountDataWrite	= 0xE3,				// INTERNAL COUNTER P-DATA WRITE
    FsExternalCounterRead				= 0x64,				// EXTERNAL COUNTER DATA READ READ(Signed), 32bit, 0x00000000
    FsExternalCounterWrite				= 0xE4,				// EXTERNAL COUNTER DATA READ WRITE(Signed)
    FsExternalCounterComparatorDataRead	= 0x65,				// EXTERNAL COUNTER COMPARATE DATA READ(Signed), 32bit, 0x00000000
    FsExternalCounterComparatorDataWrite= 0xE5,				// EXTERNAL COUNTER COMPARATE DATA WRITE(Signed)
    FsExternalCounterPreScaleDataRead	= 0x66,				// EXTERNAL COUNTER PRE-SCALE DATA READ, 8bit, 0x00
    FsExternalCounterPreScaleDataWrite	= 0xE6,				// EXTERNAL COUNTER PRE-SCALE DATA WRITE
    FsExternalCounterNCountDataRead		= 0x67,				// EXTERNAL COUNTER P-DATA READ, 32bit, 0x7FFFFFFF
    FsExternalCounterNCountDataWrite	= 0xE7,				// EXTERNAL COUNTER P-DATA WRITE
    FsExternalSpeedDataRead				= 0x68,				// EXTERNAL SPEED DATA READ, 32bit, 0x00000000
    FsExternalSpeedDataWrite			= 0xE8,				// EXTERNAL SPEED DATA WRITE
    FsExternalSpeedComparateDataRead	= 0x69,				// EXTERNAL SPEED COMPARATE DATA READ, 32bit, 0x00000000
    FsExternalSpeedComparateDataWrite	= 0xE9,				// EXTERNAL SPEED COMPARATE DATA WRITE
    FsExternalSensorFilterBandWidthDataRead	= 0x6A,			// 외부 센서 필터 대역폭 설정 DATA READ, 8bit, 0x05
    FsExternalSensorFilterBandWidthDataWrite= 0xEA,			// 외부 센서 필터 대역폭 설정 DATA WRITE
    FsOffRangeDataRead					= 0x6B,				// OFF-RANGE DATA READ, 8bit, 0x00
    FsOffRangeDataWrite					= 0xEB,				// OFF-RANGE DATA WRITE
    FsDeviationDataRead					= 0x6C,				// DEVIATION DATA READ, 16bit, 0x0000
	FsNoOperation_EC					= 0xEC,				// No operation
    FsPgmRegChangeDataRead				= 0x6D,				// PGM REGISTER CHANGE DATA READ
    FsPgmRegChangeDataWrite				= 0xED,				// PGM REGISTER CHANGE DATA WRITE
	FsNoOperation_6E					= 0x6E,				// No operation
    FsCompareRegisterInputChangeDataWrite	= 0xEE,			// COMPARE REGISTER INPUT CHANGE
	FsNoOperation_6F					= 0x6F,				// No operation
	FsNoOperation_EF					= 0xEF,				// No operation
//?	FsCompareRegisterInputChangeDataRead= 0x6E,				//<+> 2002-11-15 FS2.0 - JNS
    
	/* BUS - 2 Group Register			*/
    FsChipFunctionSetDataRead			= 0x70,				// 칩 기능 설정 DATA READ, 13bit, 0x0C3E
    FsChipFunctionSetDataWrite			= 0xF0,				// 칩 기능 설정 DATA WRITE
    FsMode1Read							= 0x71,				// MODE1 DATA READ, 8bit, 0x00
    FsMode1Write						= 0xF1,				// MODE1 DATA WRITE
    FsMode2Read							= 0x72,				// MODE2 DATA READ, 11bit, 0x200
    FsMode2Write						= 0xF2,				// MODE2 DATA WRITE
    FsUniversalSignalRead				= 0x73,				// UNIVERSAL IN READ, 11bit, 0x0000
    FsUniversalSignalWrite				= 0xF3,				// UNIVERSAL OUT WRITE
    FsEndStatusRead						= 0x74,				// END STATUS DATA READ, 15bit, 0x0000
	FsNoOperation_F4					= 0xF4,				// No operation
    FsMechanicalSignalRead				= 0x75,				// MECHANICAL SIGNAL DATA READ, 13bit
	FsNoOperation_F5					= 0xF5,				// No operation
    FsDriveStatusRead					= 0x76,				// DRIVE STATE DATA READ, 9bit
	FsNoOperation_F6					= 0xF6,				// No operation
    FsExternalCounterSetDataRead		= 0x77,				// EXTERNAL COUNTER 설정 DATA READ, 9bit, 0x00
    FsExternalCounterSetDataWrite		= 0xF7,				// EXTERNAL COUNTER 설정 DATA WRITE
	FsNoOperation_78					= 0x78,				// No operation
    FsRegisterClear						= 0xF8,				// REGISTER CLEAR(INITIALIZATION)
    FsInterruptFlagRead					= 0x79,				// Interrupt Flag READ, 32bit, 0x00000000
    FsInterruptOutCommand				= 0xF9,				// Interrupt 발생 Command
    FsInterruptMaskRead					= 0x7A,				// Interrupt Mask READ, 32bit, 0x00000001
    FsInterruptMaskWrite				= 0xFA,				// Interrupt Mask WRITE
    FsEMode1DataRead					= 0x7B,				// EMODE1 DATA READ, 8bit, 0x00
    FsEMode1DataWrite					= 0xFB,				// EMODE1 DATA WRITE
    FsEUniversalOutRead					= 0x7C,				// Extension UNIVERSAL OUT READ, 8bit, 0x00
    FsEUniversalOutWrite				= 0xFC,				// Extension UNIVERSAL OUT WRITE
	FsNoOperation_7D					= 0x7D,				// No operation
    FsLimitStopDisableWrite				= 0xFD,				// LIMIT 완전정지 해제
    FsUserInterruptSourceSelectRegRead	= 0x7E,				// USER Interrupt source selection register READ, 32bit, 0x00000000
    FsUserInterruptSourceSelectRegWrite	= 0xFE,				// USER Interrupt source selection register WRITE
	FsNoOperation_7F					= 0x7F,				// No operation
	FsNoOperation_FF					= 0xFF,				// No operation
} FSCOMMAND;

/*
	모듈의 Version :
		- CAMC-FS 1.0 이면 0	=> CFS
		- CAMC-FS 2.0 이면 2	=> CFS, CFS20
		- CAMC-FS 2.1 이면 4	=> CFS, CFS20
*/
#define CAMC_FS_VERSION_10				0					// FS Ver 1.0
#define CAMC_FS_VERSION_20				2					// FS Ver 2.0
#define CAMC_FS_VERSION_20_KDNS			3					// FS Ver 2.0 - for KDNS
#define CAMC_FS_VERSION_21				4					// FS Ver 2.1
#define CAMC_FS_VERSION_30				5					// FS Ver 3.0


// 스크립트/갈무리 동작 설정 레지스터-1/2/3/Queue
#define SCRIPT_REG1						1					// 스크립트 레지스터-1
#define SCRIPT_REG2						2					// 스크립트 레지스터-2
#define SCRIPT_REG3						3					// 스크립트 레지스터-3
#define SCRIPT_REG_QUEUE				4					// 스크립트 레지스터-Queue
#define CAPTION_REG1					11					// 갈무리 레지스터-1
#define CAPTION_REG2					12					// 갈무리 레지스터-2
#define CAPTION_REG3					13					// 갈무리 레지스터-3
#define CAPTION_REG_QUEUE				14					// 갈무리 레지스터-Queue

// CFS20KeSetScriptCaption의 event입력을 위한 값 define.
// bit 31 : 0=한번만실행, 1=계속 실행
#define OPERATION_ONCE_RUN				0x00000000			// bit 31 OFF
#define OPERATION_CONTINUE_RUN			0x80000000			// bit 31 ON
// bit 30..26 : Don't care
// bit 25..24 : 00=1번이벤트만검출, 01=OR연산, 10=AND연산, 11=XOR연산
#define OPERATION_EVENT_NONE			0x00000000			// bit 25=OFF, 24=OFF
#define OPERATION_EVENT_OR				0x01000000			// bit 25=OFF, 24=ON
#define OPERATION_EVENT_AND				0x02000000			// bit 25=ON,  24=OFF
#define OPERATION_EVENT_XOR				0x03000000			// bit 25=ON,  24=ON
// bit 23..16 : 2번 검출 이벤트 설정
#define OPERATION_EVENT_2(Event)		((Event&0xFF)<<16)	// bit 23..16
// bit 15..8 : 1번 검출 이벤트 설정
#define OPERATION_EVENT_1(Event)		((Event&0xFF)<<8)	// bit 15..8
// bit 7..0 : 이벤트 검출 후 실행할 Command
#define OPERATION_EVENT_COMMAND(Command)	(Command&0xFF)	// bit 7..0 : enum _FSCOMMAND 참조


// CFS20SetScriptCaption의 event_logic입력을 위한 값 define.
// bit 7 : 0=한번만실행, 1=계속 실행
#define FSSC_ONE_TIME_RUN			0x00			// bit 7 OFF
#define FSSC_ALWAYS_RUN				0x80			// bit 7 ON
// bit 6 bit : sc에 따라서 다음과 같은 차이점이 있음.
//	sc = SCRIPT_REG1, SCRIPT_REG2, SCRIPT_REG3 일 때. Don't care.
//	sc = SCRIPT_REG_QUEUE 일 때. Script 동작시 인터럽트 출력 유무. 해당 인터럽트 mask가 enable 되어 있어야 함.
//		0(인터럽트 발생하지 않음), 1(해당 script 수행시 인터럽트 발생) 
//	sc = CAPTION_REG1, CAPTION_REG2, CAPTION_REG3 일 때. Don't care.
//	sc = CAPTION_REG_QUEUE. Caption 동작시 인터럽트 출력 유무. 해당 인터럽트 mask가 enable되어 있어야 함.
//		0(인터럽트 발생하지 않음), 1(해당 caption 수행시 인터럽트 발생) 
#define IPSCQ_INTERRUPT_DISABLE		0x00			// bit 6 OFF
#define IPSCQ_INTERRUPT_ENABLE		0x40			// bit 6 ON
// bit 1..0 bit : 00=1번이벤트만검출, 01=OR연산, 10=AND연산, 11=XOR연산
#define FSSC_EVENT_OP_NONE			0x00			// bit 1=OFF, 0=OFF
#define FSSC_EVENT_OP_OR			0x01			// bit 1=OFF, 0=ON
#define FSSC_EVENT_OP_AND			0x02			// bit 1=ON,  0=OFF
#define FSSC_EVENT_OP_XOR			0x03			// bit 1=ON,  0=ON

/* EVENT LIST							*/
typedef enum _FSEVENT
{
	EVENT_NONE							= 0x00,				// 검출사항 없음
	EVENT_DRIVE_END						= 0x01,				// 드라이브 종료
	EVENT_PRESETDRIVE_START				= 0x02,				// 지정펄스 수 드라이브 시작
	EVENT_PRESETDRIVE_END				= 0x03,				// 지정펄스 수 드라이브 종료
	EVENT_CONTINOUSDRIVE_START			= 0x04,				// 연속 드라이브 시작
	EVENT_CONTINOUSDRIVE_END			= 0x05,				// 연속 드라이브 종료
	EVENT_SIGNAL_SEARCH_1_START			= 0x06,				// 신호 검출-1 드라이브 시작
	EVENT_SIGNAL_SEARCH_1_END			= 0x07,				// 신호 검출-1 드라이브 종료
	EVENT_SIGNAL_SEARCH_2_START			= 0x08,				// 신호 검출-2 드라이브 시작
	EVENT_SIGNAL_SEARCH_2_END			= 0x09,				// 신호 검출-2 드라이브 종료
	EVENT_ORIGIN_DETECT_START			= 0x0A,				// 원점검출 드라이브 시작
	EVENT_ORIGIN_DETECT_END				= 0x0B,				// 원점검출 드라이브 종료
	EVENT_SPEED_UP						= 0x0C,				// 가속
	EVENT_SPEED_CONST					= 0x0D,				// 등속
	EVENT_SPEED_DOWN					= 0x0E,				// 감속
	EVENT_ICG							= 0x0F,				// 내부위치카운터 > 내부위치비교값
	EVENT_ICE							= 0x10,				// 내부위치카운터 = 내부위치비교값
	EVENT_ICL							= 0x11,				// 내부위치카운터 < 내부위치비교값
	EVENT_ECG							= 0x12,				// 외부위치카운터 > 외부위치비교값
	EVENT_ECE							= 0x13,				// 외부위치카운터 = 외부위치비교값
	EVENT_ECL							= 0x14,				// 외부위치카운터 < 외부위치비교값
	EVENT_EPCG							= 0x15,				// 외부펄스카운터 > 외부펄스카운터비교값
	EVENT_EPCE							= 0x16,				// 외부펄스카운터 = 외부펄스카운터비교값
	EVENT_EPCL							= 0x17,				// 외부펄스카운터 < 외부펄스카운터비교값
	EVENT_SPG							= 0x18,				// 현재속도데이터 > 현재속도비교데이터
	EVENT_SPE							= 0x19,				// 현재속도데이터 = 현재속도비교데이터
	EVENT_SPL							= 0x1A,				// 현재속도데이터 < 현재속도비교데이터
	EVENT_SP12G							= 0x1B,				// 현재속도데이터 > Rate Change Point 1-2
	EVENT_SP12E							= 0x1C,				// 현재속도데이터 = Rate Change Point 1-2
	EVENT_SP12L							= 0x1D,				// 현재속도데이터 < Rate Change Point 1-2
	EVENT_SP23G							= 0x1E,				// 현재속도데이터 > Rate Change Point 2-3
	EVENT_SP23E							= 0x1F,				// 현재속도데이터 = Rate Change Point 2-3
	EVENT_SP23L							= 0x20,				// 현재속도데이터 < Rate Change Point 2-3
	EVENT_OBJECT_SPEED					= 0x21,				// 현재속도데이터 = 목표속도데이터
	EVENT_SS_SPEED						= 0x22,				// 현재속도데이터 = 시작속도데이터
	EVENT_ESTOP							= 0x23,				// 급속정지
	EVENT_SSTOP							= 0x24,				// 감속정지
	EVENT_PELM							= 0x25,				// +Emergency Limit 신호 입력
	EVENT_NELM							= 0x26,				// -Emergency Limit 신호 입력
	EVENT_PSLM							= 0x27,				// +Slow Down Limit 신호 입력
	EVENT_NSLM							= 0x28,				// -Slow Down Limit 신호 입력
	EVENT_DEVIATION_ERROR				= 0x29,				// 탈조 에러 발생
	EVENT_DATA_ERROR					= 0x2A,				// 데이터 설정 에러 발생
	EVENT_ALARM_ERROR					= 0x2B,				// Alarm 신호 입력
	EVENT_ESTOP_COMMAND					= 0x2C,				// 급속 정지 명령 실행
	EVENT_SSTOP_COMMAND					= 0x2D,				// 감속 정지 명령 실행
	EVENT_ESTOP_SIGNAL					= 0x2E,				// 급속 정지 신호 입력
	EVENT_SSTOP_SIGNAL					= 0x2F,				// 감속 정지 신호 입력
	EVENT_ELM							= 0x30,				// Emergency Limit 신호 입력
	EVENT_SLM							= 0x31,				// Slow Down Limit 신호 입력
	EVENT_INPOSITION					= 0x32,				// Inposition 신호 입력
	EVENT_IN0_HIGH						= 0x33,				// IN0 High 신호 입력
	EVENT_IN0_LOW						= 0x34,				// IN0 Low  신호 입력
	EVENT_IN1_HIGH						= 0x35,				// IN1 High 신호 입력
	EVENT_IN1_LOW						= 0x36,				// IN1 Low  신호 입력
	EVENT_IN2_HIGH						= 0x37,				// IN2 High 신호 입력
	EVENT_IN2_LOW						= 0x38,				// IN2 Low  신호 입력
	EVENT_IN3_HIGH						= 0x39,				// IN3 High 신호 입력
	EVENT_IN3_LOW						= 0x3A,				// IN3 Low  신호 입력
	EVENT_OUT0_HIGH						= 0x3B,				// OUT0 High 신호 입력
	EVENT_OUT0_LOW						= 0x3C,				// OUT0 Low  신호 입력
	EVENT_OUT1_HIGH						= 0x3D,				// OUT1 High 신호 입력
	EVENT_OUT1_LOW						= 0x3E,				// OUT1 Low  신호 입력
	EVENT_OUT2_HIGH						= 0x3F,				// OUT2 High 신호 입력
	EVENT_OUT2_LOW						= 0x40,				// OUT2 Low  신호 입력
	EVENT_OUT3_HIGH						= 0x41,				// OUT3 High 신호 입력
	EVENT_OUT3_LOW						= 0x42,				// OUT3 Low  신호 입력
	EVENT_SENSOR_DRIVE1_START			= 0x43,				// Sensor Positioning drive I 시작
	EVENT_SENSOR_DRIVE1_END				= 0x44,				// Sensor Positioning drive I 종료
	EVENT_SENSOR_DRIVE2_START			= 0x45,				// Sensor Positioning drive II 시작
	EVENT_SENSOR_DRIVE2_END				= 0x46,				// Sensor Positioning drive II 종료
	EVENT_SENSOR_DRIVE3_START			= 0x47,				// Sensor Positioning drive III 시작
	EVENT_SENSOR_DRIVE3_END				= 0x48,				// Sensor Positioning drive III 종료
	EVENT_1STCOUNTER_NDATA_CLEAR		= 0x49,				// 1'st counter N-data count clear
	EVENT_2NDCOUNTER_NDATA_CLEAR		= 0x4A,				// 2'nd counter N-data count clear
	EVENT_MARK_SIGNAL_HIGH				= 0x4B,				// Mark# signal high
	EVENT_MARK_SIGNAL_LOW				= 0x4C,				// Mark# signal low
	EVENT_EUIO0_HIGH					= 0x4D,				// EUIO0 High 신호 입력
	EVENT_EUIO0_LOW						= 0x4E,				// EUIO0 Low  신호 입력
	EVENT_EUIO1_HIGH					= 0x4F,				// EUIO1 High 신호 입력
	EVENT_EUIO1_LOW						= 0x50,				// EUIO1 Low  신호 입력
	EVENT_EUIO2_HIGH					= 0x51,				// EUIO2 High 신호 입력
	EVENT_EUIO2_LOW						= 0x52,				// EUIO2 Low  신호 입력
	EVENT_EUIO3_HIGH					= 0x53,				// EUIO3 High 신호 입력
	EVENT_EUIO3_LOW						= 0x54,				// EUIO3 Low  신호 입력
	EVENT_EUIO4_HIGH					= 0x55,				// EUIO4 High 신호 입력
	EVENT_EUIO4_LOW						= 0x56,				// EUIO4 Low  신호 입력
	EVENT_SOFTWARE_PLIMIT				= 0x57,				// +Software Limit
	EVENT_SOFTWARE_NLIMIT				= 0x58,				// -Software Limit
	EVENT_SOFTWARE_LIMIT				= 0x59,				// Software Limit
	EVENT_TRIGGER_ENABLE				= 0x5A,				// Trigger enable
	EVENT_INT_GEN_SOURCE				= 0x5B,				// Interrupt Generated by any source
	EVENT_INT_GEN_CMDF9					= 0x5C,				// Interrupt Generated by Command "F9"
	EVENT_PRESETDRIVE_TRI_START			= 0x5D,				// Preset 삼각구동 시작
	EVENT_BUSY_HIGH						= 0x5E,				// 드라이브 busy High
	EVENT_BUSY_LOW						= 0x5F,				// 드라이브 busy Low
	EVENT_UNCONDITIONAL_RUN				= 0xFF,				// 무조건 수행(Queue command 한정)
} FSEVENT;

#endif // __CAMC_FS_DEFINE_H__
