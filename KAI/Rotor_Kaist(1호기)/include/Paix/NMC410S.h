#ifndef _NMC410S_H
#define _NMC410S_H

#ifdef __cplusplus
	extern "C" {
#endif

// ��� �Լ��� ���ϰ� 
#ifndef PAIX_RETURN_VALUE
#define PAIX_RETURN_VALUE
	#define	NMC_NOTRESPONSE		-8
	#define NMC_BUSY			-7
	#define NMC_COMMERR			-6
	#define NMC_SYNTAXERR   	-5
	#define NMC_INVALID			-4
	#define NMC_UNKOWN			-3
	#define NMC_SOCKINITERR 	-2
	#define NMC_NOTCONNECT 		-1
	#define NMC_OK				0
#endif

// Bit ���� ����
#ifndef _PAIX_BITOPERATION_
	#define _PAIX_BITOPERATION_
	#define BITSET(val,bit)		(val) |= (1<<(bit))
	#define BITCLEAR(val,bit)	(val) &= (~(1<<(bit)))
	#define BITTOGGLE(val,bit)	(val) ^= (1<<(bit))
	#define BITVALUE(val, sdata, bit)   (val) = (((sdata)>> bit) & 0x01)
	#define GETBIT(bytes, bit_no) (((bytes)>>(bit_no))&0x01)
	#define SETBIT(bytes, bit_no, val)    { (bytes) &=~(1<<(bit_no)); (bytes) |= ((val) << (bit_no)); }
#endif
// ���� Ȩ ���� �÷���
	#define HOME_READY			0	// Ȩ ���� �Ϸ�
	#define HOME_START			1	// Ȩ ���� ����
	#define HOME_NEAR			2	// NEAR ��ġ��
	#define HOME_EDGE			3	// EDGE ��ġ�� ( Near or Limit ���� )
	#define HOME_ESTOP			4	// ���� �½� ��� ��ž
	#define HOME_HOME			5	// Z �� ��ġ��
	#define HOME_END			6	// Z �� ��ġ �Ϸ�
    	#define HOME_NONE           		7	// 
	#define HOME_EMG			11
	#define HOME_ALARM			12	
// ���� ȣȯ���� ���� DEFINE
#define nmc_busy			nmc_busy_signal
#define nmc_output16		nmc_output
#define nmc_input16			nmc_input
#define nmc_get_output16	nmc_get_output
#define nmc_output16_bit	nmc_output_bit
#define nmc_input16_bit		nmc_input_bit

#define nmc_pulse_counter  	nmc_get_cmd			
#define nmc_encoder_counter nmc_get_enc			
#define nAxisXEnc			Enc[0]
#define nAxisYEnc			Enc[1]
#define nAxisZEnc			Enc[2]
#define nAxisREnc			Enc[3]
#define nAxisXCmd			Cmd[0]
#define nAxisYCmd			Enc[1]
#define nAxisZCmd			Enc[2]
#define nAxisRCmd			Enc[3]
#define nHome0				nHomeDone0
#define nHome1				nHomeDone1
#define nHome2				nHomeDone2
#define nHome3				nHomeDone3

#define NMCIOBIT16 NMCIOBIT
#define PNMCIOBIT16 PNMCIOBIT 

// 2009.07.13 �߰�
struct  NMC_OUT_MOD
{
	unsigned short 	MotorOn0:1;		// Motor OnOff
	unsigned short 	ServoOn0:1;		// Servo OnOff
	unsigned short 	ResetDCC0:1;	// Reset DCC
	unsigned short 	ResetAlm0:1;		// Reset Alarm

	unsigned short 	MotorOn1:1;
	unsigned short 	ServoOn1:1;
	unsigned short 	ResetDCC1:1;
	unsigned short 	ResetAlm1:1;

	unsigned short 	MotorOn2:1;
	unsigned short 	ServoOn2:1;
	unsigned short 	ResetDCC2:1;
	unsigned short 	ResetAlm2:1;

	unsigned short 	MotorOn3:1;
	unsigned short 	ServoOn3:1;
	unsigned short 	ResetDCC3:1;
	unsigned short 	ResetAlm3:1;
}; 

typedef union NMC_OUTSTATUS{	//WR3
   long            all;
   struct NMC_OUT_MOD		bit[2];
}NMCOUTSTATUS,*PNMCOUTSTATUS;
//	2008.05.06 - LIGIC READ �߰�
typedef struct NMCLOGIC{
	UINT nOrg1:1;
	UINT nOrg2:1;
	UINT nOrg3:1;
	UINT nOrg4:1;

	UINT nInpo1:1;
	UINT nInpo2:1;
	UINT nInpo3:1;
	UINT nInpo4:1;//8
	//---------------1 BYte ---------------	1		
	UINT nAlarm1:1;
	UINT nAlarm2:1;
	UINT nAlarm3:1;
	UINT nAlarm4:1;

	UINT nMLimit1:1;
	UINT nMLimit2:1;
	UINT nMLimit3:1;
	UINT nMLimit4:1;//16
	//---------------1 BYte ---------------	2  -0
	UINT nPLimit1:1; 
	UINT nPLimit2:1;
	UINT nPLimit3:1;
	UINT nPLimit4:1;

	UINT nNear1:1;
	UINT nNear2:1;
	UINT nNear3:1;
	UINT nNear4:1;//24
	//---------------1 BYte ---------------	3		
	UINT nZ1:1;
	UINT nZ2:1;
	UINT nZ3:1;
	UINT nZ4:1;

	UINT nEmer:1;
	UINT nReserve1:3; //32

	//---------------1 BYte --------------- 4 -1
	UINT nEncCount1:2;
	UINT nEncCount2:2;
	UINT nEncCount3:2;
	UINT nEncCount4:2;
	//---------------1 BYte --------------- 5
	UINT nEncInput1:2;
	UINT nEncInput2:2;
	UINT nEncInput3:2;
	UINT nEncInput4:2;
	//---------------1 BYte --------------- 6 -2
	UINT nPulse1:1;
	UINT nPulse2:1;
	UINT nPulse3:1;
	UINT nPulse4:1;
	
	UINT nClock1:1;
	UINT nClock2:1;
	UINT nClock3:1;
	UINT nClock4:1;
	//---------------1 BYte --------------- 7
    UINT nPulseDir1:1;
    UINT nPulseDir2:1;
    UINT nPulseDir3:1;
    UINT nPulseDir4:1;
	
	UINT nEndBusyMode1:1;
	UINT nEndBusyMode2:1;	
	UINT nEndBusyMode3:1;
	UINT nEndBusyMode4:1;	
	//---------------1 BYte --------------- 8 -3
}NMCLOGICBIT,*PNMCLOGICBIT;

//
// 2005.8.17 �߰�
// nmc_get_sensor_all �� ���
//    BIT : 1 ON
//    BIT : 0 OFF
typedef struct NMCBit{
	UINT nBusy0:1;
	UINT nBusy1:1;
	UINT nBusy2:1;
	UINT nBusy3:1;  //4
			
	UINT nError0:1;
	UINT nError1:1;
	UINT nError2:1;
	UINT nError3:1;  //8
	//---------------1 BYte ---------------	1		
	UINT nNear0:1;
	UINT nNear1:1;
	UINT nNear2:1;
	UINT nNear3:1;  //12
			
	UINT nPLimit0:1;
	UINT nNLimit0:1;
	UINT nAlarm0:1;  //15
			
	UINT nPLimit1:1;
	//---------------1 BYte ---------------	2
	UINT nNLimit1:1;
	UINT nAlarm1:1;  //18
			
	UINT nPLimit2:1;
	UINT nNLimit2:1;
	UINT nAlarm2:1;  //21
			
	UINT nPLimit3:1;
	UINT nNLimit3:1;
	UINT nAlarm3:1;  //24
	//---------------1 BYte ---------------	3		
	UINT nEmer:1;    //25
	
	UINT nPointMove:1; //28
	UINT nReserve1:2; //28
		
	UINT nSwPLimit1:1; //29
	UINT nSwPLimit2:1; //30
	UINT nSwPLimit3:1; //31
	UINT nSwPLimit4:1; //32
	//---------------1 BYte --------------- 4

	UINT nInpo0:1;
	UINT nInpo1:1;
	UINT nInpo2:1;
	UINT nInpo3:1;  //36

	UINT nHome0:1;
	UINT nHome1:1;
	UINT nHome2:1;
	UINT nHome3:1;  //40
	//---------------1 BYte --------------- 1
	UINT nEncZ0:1;
	UINT nEncZ1:1;
	UINT nEncZ2:1;
	UINT nEncZ3:1; //44

	UINT nOrg0:1;
	UINT nOrg1:1;
	UINT nOrg2:1;
	UINT nOrg3:1; //48
	//---------------1 BYte --------------- 2
	UINT nSReady0:1;
	UINT nSReady1:1;
	UINT nSReady2:1;
	UINT nSReady3:1; //52

	// 2 �࿡���� ���, 4�࿡���� ����
	//INPUT 8BIT ��	
	UINT nInput0:1;  
	UINT nInput1:1;  
	UINT nInput2:1;  
	UINT nInput3:1;  
	//---------------1 BYte --------------- 3
	UINT nInput4:1;  //56
	UINT nInput5:1;  
	UINT nInput6:1;  
	UINT nInput7:1;  
	
	//UINT nReserve2:4;  //64
	UINT nSwMLimit1:1; //29
	UINT nSwMLimit2:1; //30
	UINT nSwMLimit3:1; //31
	UINT nSwMLimit4:1; //32
	//---------------1 BYte --------------- 3
}NMCBIT,*PNMCBIT;

typedef struct NMCData
{
	NMCBIT nmcBit;
	int Enc[4];
	int Cmd[4];
	double fEnc[4];
	double fCmd[4];
}NMCDATA,*PNMCDATA;

typedef struct NMCIOBit{
	UINT nBit0:1;
	UINT nBit1:1;	
	UINT nBit2:1;
	UINT nBit3:1;
	UINT nBit4:1;	
	UINT nBit5:1;
	UINT nBit6:1;
	UINT nBit7:1;	
	UINT nBit8:1;
	UINT nBit9:1;
	UINT nBit10:1;	
	UINT nBit11:1;
	UINT nBit12:1;
	UINT nBit13:1;	
	UINT nBit14:1;
	UINT nBit15:1;
}NMCIOBIT,*PNMCIOBIT;
// 32 �� IO struct		
typedef struct NMCIOBit32{
	UINT nBit0:1;
	UINT nBit1:1;	
	UINT nBit2:1;
	UINT nBit3:1;
	UINT nBit4:1;	
	UINT nBit5:1;
	UINT nBit6:1;
	UINT nBit7:1;	
	UINT nBit8:1;
	UINT nBit9:1;
	UINT nBit10:1;	
	UINT nBit11:1;
	UINT nBit12:1;
	UINT nBit13:1;	
	UINT nBit14:1;
	UINT nBit15:1;
	UINT nBit16:1;
	UINT nBit17:1;
	UINT nBit18:1;
	UINT nBit19:1;
	UINT nBit20:1;
	UINT nBit21:1;
	UINT nBit22:1;
	UINT nBit23:1;
	UINT nBit24:1;
	UINT nBit25:1;
	UINT nBit26:1;
	UINT nBit27:1;
	UINT nBit28:1;
	UINT nBit29:1;
	UINT nBit30:1;
	UINT nBit31:1;
}NMCIOBIT32,*PNMCIOBIT32;
// AND �߰� 
typedef struct POINT_DATA
{
	short nMode;
	long lDelay;
	short nAxis[4];
	long lPulse[4];
}POINTDATA;


typedef struct NMC_STATUS{
	// 1 ��
	UINT nStopOrg0:1;
	UINT nStopNear0:1;
	UINT nStopEncZ0:1;
	UINT nStopSReady0:1;
	UINT nStopPLimit0:1;
	UINT nStopMLimit0:1;
	UINT nStopAlarm0:1;
	UINT nStopEmg0:1;
	// 2��
	UINT nStopOrg1:1;
	UINT nStopNear1:1;
	UINT nStopEncZ1:1;
	UINT nStopSReady1:1;
	UINT nStopPLimit1:1;
	UINT nStopMLimit1:1;
	UINT nStopAlarm1:1;
	UINT nStopEmg1:1;

	// 3 ��
	UINT nStopOrg2:1;
	UINT nStopNear2:1;
	UINT nStopEncZ2:1;
	UINT nStopSReady2:1;
	UINT nStopPLimit2:1;
	UINT nStopMLimit2:1;
	UINT nStopAlarm2:1;
	UINT nStopEmg2:1;
	
	// 4��
	UINT nStopOrg3:1;
	UINT nStopNear3:1;
	UINT nStopEncZ3:1;
	UINT nStopSReady3:1;
	UINT nStopPLimit3:1;
	UINT nStopMLimit3:1;
	UINT nStopAlarm3:1;
	UINT nStopEmg3:1;

	// 5 ��
	UINT nStopOrg4:1;
	UINT nStopNear4:1;
	UINT nStopEncZ4:1;
	UINT nStopSReady4:1;
	UINT nStopPLimit4:1;
	UINT nStopMLimit4:1;
	UINT nStopAlarm4:1;
	UINT nStopEmg4:1;
	// 6��
	UINT nStopOrg5:1;
	UINT nStopNear5:1;
	UINT nStopEncZ5:1;
	UINT nStopSReady5:1;
	UINT nStopPLimit5:1;
	UINT nStopMLimit5:1;
	UINT nStopAlarm5:1;
	UINT nStopEmg5:1;

	// 7 ��
	UINT nStopOrg6:1;
	UINT nStopNear6:1;
	UINT nStopEncZ6:1;
	UINT nStopSReady6:1;
	UINT nStopPLimit6:1;
	UINT nStopMLimit6:1;
	UINT nStopAlarm6:1;
	UINT nStopEmg6:1;
	// 8��
	UINT nStopOrg7:1;
	UINT nStopNear7:1;
	UINT nStopEncZ7:1;
	UINT nStopSReady7:1;
	UINT nStopPLimit7:1;
	UINT nStopMLimit7:1;
	UINT nStopAlarm7:1;
	UINT nStopEmg7:1;
}NMC_STATUSBIT,*PNMC_STATUSBIT;


struct NMC_STATUS_EXT{
	// 1 ��
	short nStopOrg[8];
	short nStopNear[8];
	short nStopEncZ[8];
	short nStopSReady[8];
	short nStopPLimit[8];
	short nStopMLimit[8];
	short nStopAlarm[8];
	short nStopEmg[8];
};


struct NMC_SAM_STATUS_EXT{
	// 1 ��
	short nRun[8];
	short nStatus[8];
	long lMotorRotCount[8];
	long  lMotorRes[8];
};


// ��ġ ���� ����
	int WINAPI nmc_opendevice(short dev_no);
	void WINAPI nmc_closedevice(short dev_no);

// ���� �Լ�
	short WINAPI nmc_sstop_motor(short dev_no, short axis);
	short WINAPI nmc_sstop_choice_motor(short dev_no, short axis);
	short WINAPI nmc_stop_motor(short dev_no, short axis);
	short WINAPI nmc_stop_choice_motor(short dev_no, short axis);
	short WINAPI nmc_stop_delay(short dev_no, short axis);

// Ȩ �̵� �Լ�
/*
	Ȩ �̵��� �̵� �Ϸ�� NMCBIT�� Home ���Ǵ� nmc_is_homt ���� Ȯ���ؾ� ��
*/
short  WINAPI nmc_home_move(short dev_no, short axis);

/* 
����    ��	ȸ�� ����		���
+ LIMIT  0		
- LIMIT	 1		CcW
NEAR(+)	 2		CW
NEAR(-)  3		CCW
Z  ��	0x80    			Z ���� ���� ��ġ ����� or ������ 
*/
short WINAPI nmc_home_mode(short dev_no, short axis,short mode);
short WINAPI nmc_is_home(short dev_no, short axis,short *signal);

// NMC �ʱ�ȭ
	void WINAPI init_nmc(short dev_no);

// 2005.8.11 �Լ��� ����
// nmc_resolution  =>nmc_set_resolution
	void WINAPI nmc_set_resolution(short dev_no, short axis, long resolution);
	short WINAPI nmc_select_clock(short dev_no, short axis, short clock);
// 2006 0227 ���ķ� �������� ����
	short WINAPI nmc_select_motor(short dev_no, short axis, short motor);

// LOGIC ���� �Լ�
	short WINAPI nmc_es_logic(short dev_no, BOOL logic);
	short WINAPI nmc_near_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_z_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_limit_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_plus_limit_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_minus_limit_logic(short dev_no, short axis, BOOL logic);

	short WINAPI nmc_sw_plus_limit_onoff(short dev_no, short axis, short onoff,long CompP);
	short WINAPI nmc_sw_minus_limit_onoff(short dev_no, short axis, short onoff,long CompM);

	short WINAPI nmc_pulse_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_alarm_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_inpos_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc_org_logic(short dev_no, short axis, BOOL logic);

// ���ڴ� ���� �Լ�
	short  WINAPI nmc_encoder_input_mode(short dev_no, short axis, short mode);
	/* MODE
		2 - 1ü�� ,	1 - 2ü�� , 0 - 4ü��
	*/
	short WINAPI nmc_encoder_counter_mode(short dev_no, 
										short axis, 
										short mode);

short WINAPI nmc_servo_on_off(short dev_no,		// ��ġ��ȣ
							  short axis,		// ���ȣ
							  short on_off);	// 0 : OFF 1 : ON
short WINAPI nmc_alarmreset_on_off(short dev_no, // ��ġ ��ȣ
								   short axis,	 // �� ��ȣ
								   short on_off); // 0 : OFF 1 : ON
short WINAPI nmc_motor_on_off(short dev_no,		// ��ġ ��ȣ
							  short axis,		// �� ��ȣ
							  short on_off);	// 0 : OFF 1 : ON
// �ӵ� ���� �Լ�
	short WINAPI nmc_speed_pps(short dev_no, short axis, long start, long accdec, long max);
	short WINAPI nmc_scurve_pps(short dev_no, short axis, long start, long accdec, long max,long jerk);
	short WINAPI nmc_speed_rpm(short dev_no, short axis, double start, double accdec, double max,double jerk);

// �̵� ���� �Լ�

	short WINAPI nmc_infinity_move(short dev_no, short axis, short dir);
	short WINAPI nmc_ptp_move(short dev_no, short axis, long pulse);
	short WINAPI nmc_aptp_move(short dev_no, short axis, long pulse);
	short WINAPI nmc_ici_move(short dev_no,					// ��ġ ��ȣ
						   short axis1, short axis2,		// ��ȣ ������ �� ��
						   long center_0, long center_1,	// ���� �߽���
						   long end_0, long end_1,			// ���� ��ȣ ����
						   short dir);						// 0 : CW 1 : CCW
	short WINAPI nmc_aci_move(short dev_no,					// ��ġ ��ȣ
						   short axis1, short axis2,		// ��ȣ ������ �� ��
						   long center_0, long center_1,	// ���� �߽���
						   long end_0, long end_1,			// ���� ��ȣ ����
						   short dir);						// 0 : CW 1 : CCW
	// ���� ���� �Լ� - 3 �� ������ ����
	// �� �����Ϳ����� �ø��������� �Է��� �ؾ� ��
	/*
		int nCount = 0,nRet;
		int nAxis[3] = { 1,2,4 };
		long nPos[3] = { 10000,5000,200000};

		nmc_il_move(m_nPaixCtlNo, nCount, nAxis, nPos);
	*/
	short WINAPI nmc_il_move(short dev_no, int naxiscount, int *naxis, long *pulse);
	short WINAPI nmc_al_move(short dev_no, int naxiscount, int *naxis, long *pulse);
	
	// ���� ���� �Լ�
	// �� �����Ϳ����� �ø��������� �Է��� �ؾ� ��
	/*
		int nCount = 0,nRet;
		int nAxis[3] = { 1,2,3 };
		long nPos[3] = { 10000,5000,200000};

		nmc_imull_move(m_nPaixCtlNo, nCount, nAxis, nPos);
	*/

	short WINAPI nmc_amul_move(short dev_no, int naxiscount,int *axis,long *pulse);
	short WINAPI nmc_imul_move(short dev_no, int naxiscount,int *axis,long *pulse);

	//{ �ñ׳� read �Լ�
	/*
		signal 0 : OFF
			   1 : ON
	*/
	short WINAPI nmc_es_signal(short dev_no,short *signal);				
	short WINAPI nmc_busy_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_alarm_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_near_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_z_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_for_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_rev_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_inpos_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_error_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_org_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc_srvready_signal(short dev_no, short axis,short *signal);
	
//}
// ���� �ӵ� �б�
	short WINAPI nmc_pps_monitor(short dev_no, short axis,long *data);
// Near or Z �� ã��
	short WINAPI EnableZ(short dev_no,short axis, BOOL mode);
	short WINAPI EnableNear(short dev_no,short axis, BOOL mode);

// IO 16 ���� �Լ�
	short WINAPI nmc_get_ionum(short dev_no);
	short WINAPI nmc_output(short dev_no, short byte_no, short data);
	short WINAPI nmc_input(short dev_no, short byte_no, short *in);
	short WINAPI nmc_get_output(short dev_no, short byte_no, short *out);
	short WINAPI nmc_output_bit (short dev_no, short byte_no, short bit_no, BOOL data);
	short WINAPI nmc_outputbit(short dev_no, short byte_no, short bitno,short data);
	short WINAPI nmc_input_bit(short  dev_no, short byte_no, short  bit_no, BOOL  *in);
// IO 32�� ���� �Լ� -- 
	// long �� int ������� 4 byte��
	short WINAPI nmc_input32(short  dev_no, long  *in);
	short WINAPI nmc_output32(short dev_no, long data);
	short WINAPI nmc_get_output32(short dev_no, long *out);
	short WINAPI nmc_output32_bit(short dev_no, short bit_no, BOOL data);
	short WINAPI nmc_output32bit(short dev_no, short bitno,short data);
	short WINAPI nmc_input32_bit(short  dev_no, short  bit_no, BOOL  *in);
	/*
	 	Type = 0 : 16/16 IO
		Type = 1 : 32/32 IO
	 */
	short WINAPI nmc_getiotype(short dev_no,int *pType);

// 2 �� IO 8�� ���� �Լ� - ��� OUT 4 / IN 4
	short WINAPI nmc_output8(short dev_no, BYTE data);
	short WINAPI nmc_input8(short dev_no, BYTE *in);
	short WINAPI nmc_get_output8(short dev_no, BYTE *out);
	short WINAPI nmc_output8_bit(short dev_no, short bit_no, BOOL data);
	short WINAPI nmc_input8_bit(short  dev_no, short  bit_no, BOOL  *in);

//////////////////////////////////////////////////////////////////////////////////////////

// Mode - 0 : Cmd Pulse Mode 
//		  1 : Enc Pulse Mode �� ��� Cmd & Enc ������ �ݵ�� 1:1 �̾�� ��
	short WINAPI nmc_set_movemode(short dev_no,short axis,short mode);

// �޽� & ���ڴ� �� ����
	short WINAPI nmc_set_enc(short dev_no, short axis, long value);
	short WINAPI nmc_set_cmd(short dev_no, short axis, long value);

//////////////////////////////////////////////////////////////////////////////////////////
	int WINAPI nmc_get_resolution(short dev_no, short axis);
	void WINAPI nmc_set_leed(short dev_no, short axis, double fleed);
	double WINAPI nmc_get_leed(short dev_no, short axis);


// ���� & Cmd & Enc ���ÿ� �б�
	short  WINAPI nmc_get_sensor_all(short dev_no,	// ��ġ ��ȣ
								PNMCDATA pNmcData);	// struct NMCDATA ����

// 4�� ���� ���ÿ� �б�
	short WINAPI nmc_get_sensor(short dev_no,PNMCBIT pNmcBit);
// Pulse 4�� ���ÿ� �б�
	short WINAPI nmc_get_cmd_all(short dev_no,long *pulse);
	// 1�� �б�
	short WINAPI nmc_get_cmd(short dev_no,short axis,long *pulse);
// ENC 4�� ���ÿ� �б�
	short  WINAPI nmc_get_enc_all(short dev_no,long *enc);
	// 1�� �б�
	short WINAPI nmc_get_enc(short dev_no,short axis,long *enc);

//2005.11.15 �߰�
/*
	���� PC �� IP ������ �о� �ɴϴ�.
	��ġ�� �����ϱ� ���ؼ� �ݵ�� IP ������ �о� ���ñ� �ٶ��ϴ�.
	���� PC�� IP �ּҰ� "192.168.0.XXX" ��� nField0 = 192, nField1 = 168, nFiedl2 = 0 �� 
	�ּҰ��� �����ϴ�.
	nmc-40X �ø���� nmc-16X �ø���� �⺻������ 192.168.0.xxx �� �ּҰ��� �����ϴ�.
	IP �ּҸ� ���� �� ���ڴ� int �� array, size �� 2 �Դϴ�.
	int nField0[2],nField1[2],nField2[2],nField3[2];
*/
	bool WINAPI nmc_get_ipaddress(int *nField0,int *nField1,int *nField2,int *nField3);

	// ��ġ�� IP ������ �����մϴ�. PC�� IP �����ϰ�� ������ �����ϴ�.
	void WINAPI nmc_set_ipaddress(int dev_no,int nField0,int nField1,int nField2);
/*
	PortNum �� �⺻������ 1000 �� ���� �����ϴ�.
	������ ���� �Ͻñ�ٶ��ϴ�.
*/
	void WINAPI nmc_set_portnum(short dev_no, int nPortNum);
	int WINAPI nmc_get_portnum(short dev_no);

// ���� ī��Ʈ Ŭ����
// 0 : ��� ON
// 1 : ��� Off
	short WINAPI nmc_dccreset_on_off(short dev_no, short axis, short on_off);

// Mapping ���� �Լ�
//{
	short WINAPI nmc_mappole(short dev_no, short axis,short onoff);
	short WINAPI nmc_mapinterrupt(short dev_no, short axis,short onoff);
	short WINAPI nmc_getmapdata(short dev_no, short axis,int *pMapdata,int *size);
//}
// ���� ��� �߰�	
	short WINAPI nmc_anglestop_move(short dev_no, short axis, long pulse);
// Test ��
	int  WINAPI nmc_writestring(short dev_no, char *str);

// 4 �� ���� �б�
	short WINAPI nmc_get_cmdtodist_all(short dev_no, double *fdata);
	short WINAPI nmc_get_enctodist_all(short dev_no, double *fdata);
	short WINAPI nmc_get_cmdtodist(short dev_no, short axis,double *fdata);
	short WINAPI nmc_get_enctodist(short dev_no, short axis,double *fdata);
// �ӵ� 
	short WINAPI nmc_scurve_pps_ex(short dev_no, short axis, long start, long accdec, long max);
	short WINAPI nmc_speed_dist(short dev_no, short axis, double start, double accdec, double max, long jerk);
	short WINAPI nmc_speed_dist_ex(short dev_no, short axis, double start, double accdec, double max);

	short WINAPI nmc_inc_dist_move(short dev_no, short axis, double fDist);
	short WINAPI nmc_abs_dist_move(short dev_no, short axis, double fDist);
	short WINAPI nmc_il_dist_move(short dev_no, int naxiscount, int *naxis, double *fpulse);
	short WINAPI nmc_al_dist_move(short dev_no, int naxiscount, int *naxis, double *fpulse);						   

	short   WINAPI nmc_get_version(short dev_no,char *str);

	short   WINAPI nmc_infinity_start(short dev_no, short axis, short dir);
	short   WINAPI nmc_ptp_start(short dev_no, short axis, long pulse);
	short   WINAPI nmc_aptp_start(short dev_no, short axis, long pulse);

	short   WINAPI nmc_get_version(short dev_no,char *str);
	int WINAPI FindMultiple(unsigned long initSpeed, unsigned long driveSpeed, 
						unsigned long accel, unsigned long jerk);

	int WINAPI 	nmc_scurve_ext(short dev_no, short axis, long start, long acc,long dec, long max,long jerk1, long jerk2);
	int WINAPI nmc_runsample(short dev_no, short axis, short mode, short count, long pulse,long delay);
// 2008.1.24 �߰�
	// OPT => 0 ��� OPT => 1 ����
	short WINAPI nmc_sync_two_move(short dev_no, int axis1, long pulse1, int axis2, long pulse2,short opt);
	short WINAPI nmc_sync_three_move(short dev_no, int axis1, long pulse1, int axis2, long pulse2,int axis3, long pulse3,short opt);
	short WINAPI nmc_get_homeflag(short  dev_no, long *homeflag);
//
	short  WINAPI nmc_outputtime(short dev_no, short bitno, long time);

//	2008.05.06 - LIGIC READ �߰�
	short WINAPI nmc_get_logic_all(short dev_no,PNMCLOGICBIT pLogicBit);
//	2009.06.05 - LIGIC SET �߰�
	short WINAPI nmc_set_logic_all(short dev_no,PNMCLOGICBIT pLogicBit);
//	2008.10.08 - AND �߰�

#define AND_REL				0 // ��� �̵�
#define AND_ABS				1 // ���� �̵�
#define AND_SYNC2_REL 		2 // 2�� ��� �̵�
#define AND_SYNC2_ABS		3 // 2�� ���� �̵�
#define AND_SYNC3_REL		4 // 3�� ��� �̵�
#define AND_SYNC3_ABS		5 // 3�� ���� �̵�
#define AND_RESTART			6 // �ڵ� �����
#define AND_CIR_REL_CW		7 // ��ȣ ��� CW
#define AND_CIR_REL_CCW		8 // ��ȣ ��� CCW
#define AND_CIR_ABS_CW		9 // ��ȣ ���� CW
#define AND_CIR_ABS_CCW		10 // ��ȣ ���� CCW
#define AND_FOR_START		11 // �ݺ� ���� ����
#define AND_FOR_END			12 // �ݺ� ���� ��

	short WINAPI nmc_set_point(short dev_no,short nLineNum,short nMode, long lDelay,
						   short axis1,long lpulse1,
						   short axis2,long lpulse2,
						   short axis3,long lpulse3,
						   short axis4,long lpulse4);
	/*	MODE
		0 : POINT MOVE STOP
		1 : 0 LINE START
		2 : Current LINE START
		3 : Total Line Clear & STOP
	*/
	short WINAPI nmc_run_point(short dev_no,int mode);
	short WINAPI nmc_pointmove_status(short dev_no,short *status);
	short WINAPI nmc_point_currentline(short dev_no, short *lineno);
	short WINAPI nmc_get_point_info(short dev_no,short lineno,POINTDATA *pointdata);
	short WINAPI nmc_run_lineno(short dev_no,short lineno);
	short WINAPI nmc_move_point(short dev_no,short lineno);
	void WINAPI nmc_set_event(short dev_no, bool bEvent);
	/*
		0:	// + : LOW		- : HI
		1:	// + : HI		1 : LOW
	*/
	short  WINAPI nmc_onepulse_dir(short dev_no, short axis, int dir);
	/*
		0 : Exteranl signals disabled
		1:	continuous driving mode
		2:	fixed driving mode
		3:	manual pulsar mode
	*/
	short  WINAPI nmc_mpg_mode(short dev_no, short axis, int mode);
	/*
		0 : CCW/CW
		1 : CW/CCW
	*/
	short  WINAPI nmc_twopulse_dir(short dev_no, short axis, int dir);
	/*
		0 : sudden stop
		1 : decelerating stop
	*/
	short  WINAPI nmc_limit_stopmode(short dev_no, short axis, int mode);
	/*
		setting if real position counter or logical position counter is going to be compared with COMP+/-register
		0 : logical position counter, 1 : real position counter
	*/
	short WINAPI nmc_comp_compare(short dev_no, short axis, int mode);
	short WINAPI nmc_endbusy_mode(short dev_no, short axis, BOOL logic);
	
	
	// NMC 8 Axis Controller ���� ��ɾ�
	short WINAPI nmc2_set_logic_all(short dev_no,PNMCLOGICBIT pLogicBit);
	short WINAPI nmc2_get_logic_all(short dev_no,PNMCLOGICBIT pLogicBit);
	short WINAPI nmc2_get_sensor_all(short dev_no, PNMCDATA pNmcData);
	short WINAPI nmc2_is_home(short dev_no, short axis,short *signal);
	short WINAPI nmc2_get_sensor(short dev_no,	PNMCBIT pNmcBit);
	short WINAPI nmc2_get_cmd_all(short dev_no,long *pulse);
	short WINAPI nmc2_get_cmd(short dev_no,short axis,long *pulse);
	short WINAPI nmc2_get_enc_all(short dev_no,long *enc);
	short WINAPI nmc2_get_enc(short dev_no,short axis,long *enc);
	short WINAPI nmc2_busy_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_alarm_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_near_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_z_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_for_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_rev_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_inpos_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_srvready_signal(short dev_no, short axis,short *signal);
	short WINAPI nmc2_es_signal(short dev_no,short *signal);
	short WINAPI nmc2_get_homeflag(short  dev_no, long *homeflag);
	short WINAPI nmc2_imul_move(short dev_no, int naxiscount,int *axis,long *pulse);
	short WINAPI nmc2_amul_move(short dev_no, int naxiscount,int *axis,long *pulse);
	short WINAPI nmc2_alarmreset_on_off(short dev_no, short axis, short on_off,short time);
		// 2009.07.13 �߰�
	short WINAPI nmc2_srvready_logic(short dev_no, short axis, BOOL logic);
	short WINAPI nmc2_es_logic(short dev_no, short equ_no, BOOL logic);
	short WINAPI nmc2_aptp_vel(short dev_no, short axis, long pulse, long start, long accdec, long max, long jerk);
	short WINAPI nmc2_ptp_vel(short dev_no, short axis, long pulse, long start, long accdec, long max, long jerk);
	
	int WINAPI nmc2_aptp_scurve(short dev_no, short axis, long pulse, 
								long start, long acc,long dec, long max,long jerk1, long jerk2);
	int WINAPI nmc2_ptp_scurve(short dev_no, short axis, long pulse, 
								long start, long acc,long dec, long max,long jerk1, long jerk2);
	/*
		DCC TIme
		value		clearing pulse width(us)
		0			10
		1			20
		2			100
		3			200
		4			1000
		5			2000
		6			10000
		7			20000
	*/
	short WINAPI nmc2_dcc_enable(short dev_no, short axis, short enable, short dccwidth);
	short WINAPI nmc2_dcc_logic(short dev_no, short axis, BOOL logic);
	/*
		filtertime
		value		Remove Max Noise width		Input delay
		0			1.75us						2us
		1			224us						256us
		2			448us						512us
		3			896us						1.024ms
		4			1.792ms						2.048ms
		5			3.584ms						4.096ms
		6			7.168ms						8.192ms
		7			14.336ms					16.384ms
	*/
	short WINAPI nmc2_infilter_enable(short dev_no, short axis, short enable, short filtertime);

	short WINAPI nmc2_axisoutstatus(short dev_no, long *outstatus);
	short WINAPI nmc2_motion_write_param(short dev_no);
	short WINAPI nmc2_motion_read_param	(short dev_no);
	short WINAPI nmc_home_start(short dev_no, short axis, short homemode, short endmode, long lOffset);
	
	short WINAPI nmc_inporange_enable(short dev_no, short axis, short enable, short time, 
					short repeatcount,long inporange, long propval, long diffval);
	short WINAPI nmc_get_inpo_error(short dev_no, long *inpoerror);
	short WINAPI nmc_clear_inpo_error(short dev_no, short axis);
	short WINAPI nmc_set_resettime(short dev_no,long ltime);	
	short WINAPI nmc_get_stop_info(short dev_no, PNMC_STATUSBIT pstatus);
	short WINAPI nmc2_get_stop_info(short dev_no, struct NMC_STATUS_EXT *pNmcStatusExt);
	void WINAPI nmc_set_pingcheck(short dev_no, BOOL bPing);
	short WINAPI nmc_get_current_speed(short dev_no, short axis,long *speed);
	short WINAPI nmc_get_current_speed_all(short dev_no, long *speed);
	short WINAPI nmc_run_samsrv_enc(short dev_no, short axis);
	short WINAPI nmc_get_samsrv_status(short dev_no, struct NMC_SAM_STATUS_EXT * pSamStatus);
	short WINAPI nmc_encoder_clear(short dev_no, short axis);
	short WINAPI nmc_drive_pps(short dev_no,short axis, long drivespeed);
#ifdef __cplusplus
	}
#endif
#endif

//DESCRIPTION  'NMC Windows Dynamic Link Library'     -- *def file* description ....
