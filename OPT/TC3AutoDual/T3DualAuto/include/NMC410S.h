#ifndef _NMC410S_H
#define _NMC410S_H

#ifdef __cplusplus
	extern "C" {
#endif

// ��� �Լ��� ���ϰ� 
#ifndef PAIX_RETURN_VALUE
#define PAIX_RETURN_VALUE
	#define NMC_BUSY			-7
	#define NMC_COMMERR			-6
	#define NMC_SYNTAXERR   	-5
	#define NMC_INVALID			-4
	#define NMC_UNKOWN			-3
	#define NMC_SOCKINITERR 	-2
	#define NMC_NOTCONNECT 		-1
	#define NMC_OK				0

	#define NMC_A				0	// A ���� High Active
	#define NMC_B				1	// B ���� Low Active
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

// ���� ȣȯ���� ���� DEFINE
#define nmc_busy			nmc_busy_signal
#define nmc_output16		nmc_output
#define nmc_input16			nmc_input
#define nmc_get_output16	nmc_get_output
#define nmc_output16_bit	nmc_output_bit
#define nmc_input16_bit		nmc_input_bit
#define OpenDevice          nmc_opendevice
#define CloseDevice         nmc_closedevice

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
	//---------------1 BYte ---------------			
	UINT nNear0:1;
	UINT nNear1:1;
	UINT nNear2:1;
	UINT nNear3:1;  //12
			
	UINT nPLimit0:1;
	UINT nNLimit0:1;
	UINT nAlarm0:1;  //15
			
	UINT nPLimit1:1;
	//---------------1 BYte ---------------	
	UINT nNLimit1:1;
	UINT nAlarm1:1;  //18
			
	UINT nPLimit2:1;
	UINT nNLimit2:1;
	UINT nAlarm2:1;  //21
			
	UINT nPLimit3:1;
	UINT nNLimit3:1;
	UINT nAlarm3:1;  //24
	//---------------1 BYte ---------------			
	UINT nEmer:1;    //25
	
	UINT nReserve1:7; //32
	//---------------1 BYte ---------------

	UINT nInpo0:1;
	UINT nInpo1:1;
	UINT nInpo2:1;
	UINT nInpo3:1;  //36

	UINT nHome0:1;
	UINT nHome1:1;
	UINT nHome2:1;
	UINT nHome3:1;  //40
	//---------------1 BYte ---------------
	UINT nEncZ0:1;
	UINT nEncZ1:1;
	UINT nEncZ2:1;
	UINT nEncZ3:1; //44

	UINT nOrg0:1;
	UINT nOrg1:1;
	UINT nOrg2:1;
	UINT nOrg3:1; //48
	//---------------1 BYte ---------------
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
	//---------------1 BYte ---------------
	UINT nInput4:1;  //56
	UINT nInput5:1;  
	UINT nInput6:1;  
	UINT nInput7:1;  
	
	UINT nReserve2:4;  //64
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
+ LIMIT  0		CCW
- LIMIT	 1		CW
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

	// ENC & CMD ���� ���� �����Ѵ�.
	short  WINAPI nmc_init_enc_counter(short dev_no, short axis, long value);
	short   WINAPI nmc_encoder_clear(short dev_no, short axis);

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
	short WINAPI nmc_speed_rpm(short dev_no, short axis, long start, long accdec, long max,long jerk);

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
	//short  WINAPI nmc_detail_error(short dev_no, short axis,short *error);
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
	short WINAPI nmc_input_bit(short  dev_no, short byte_no, short  bit_no, BOOL  *in);
// IO 32�� ���� �Լ� -- 
	// long �� int ������� 4 byte��
	short WINAPI nmc_input32(short  dev_no, long  *in);
	short WINAPI nmc_output32(short dev_no, long data);
	short WINAPI nmc_get_output32(short dev_no, long *out);
	short WINAPI nmc_output32_bit(short dev_no, short bit_no, BOOL data);
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
	void WINAPI nmc_set_leed(short dev_no, short axis, long leed);
	int WINAPI nmc_get_leed(short dev_no, short axis);


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
	void WINAPI nmc_set_ipaddress(int nField0,int nField1,int nField2);
/*
	PortNum �� �⺻������ 1000 �� ���� �����ϴ�.
	������ ���� �Ͻñ�ٶ��ϴ�.
*/
	void WINAPI nmc_set_portnum(int nPortNum);
	int WINAPI nmc_get_portnum(void);

// ���� ī��Ʈ Ŭ����
// 0 : ��� ON
// 1 : ��� Off
	short WINAPI nmc_dccreset_on_off(short dev_no, short axis, short on_off);

// Mapping ���� �Լ�
//{
	short WINAPI nmc_mappole(short dev_no, short axis,short onoff);
	short WINAPI nmc_mapinterrupt(short dev_no, short axis,short onoff);
	short WINAPI nmc_getmapdata(short dev_no, short axis,double *fMapdata,int *size);
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
	short  WINAPI nmc_scurve_pps_ex(short dev_no, short axis, long start, long accdec, long max);
	short  WINAPI nmc_speed_dist(short dev_no, short axis, double start, double accdec, double max, long jerk);
	short  WINAPI nmc_speed_dist_ex(short dev_no, short axis, double start, double accdec, double max);

	short   WINAPI nmc_inc_dist_move(short dev_no, short axis, double fDist);
	short   WINAPI nmc_abs_dist_move(short dev_no, short axis, double fDist);
	short   WINAPI nmc_il_dist_move(short dev_no, int naxiscount, int *naxis, double *fpulse);
	short   WINAPI nmc_al_dist_move(short dev_no, int naxiscount, int *naxis, double *fpulse);						   
// mm ��� �̵� �Լ�
	short   WINAPI nmc_rel_dist_move(short dev_no, // ��ġ ��ȣ
								short axis,		// ���ȣ
								double fDist	// �̵��� �Ÿ� mm
								);
// mm ���� �̵� �Լ�
	short   WINAPI nmc_abs_dist_move(short dev_no, // ��ġ ��ȣ
								short axis,		// ���ȣ
								double fDist	// �̵��� �Ÿ� mm
								);
	short   WINAPI nmc_get_version(short dev_no,char *str);

	short   WINAPI nmc_st_start(short dev_no, short on_off);
	short   WINAPI nmc_infinity_start(short dev_no, short axis, short dir);
	short   WINAPI nmc_ptp_start(short dev_no, short axis, long pulse);
	short   WINAPI nmc_aptp_start(short dev_no, short axis, long pulse);
	short   WINAPI nmc_get_version(short dev_no,char *str);
	int WINAPI FindMultiple(unsigned long initSpeed, unsigned long driveSpeed, 
						unsigned long accel, unsigned long jerk);

	int WINAPI 	nmc_scurve_ext(short dev_no, short axis, long start, long acc,long dec, long max,long jerk1, long jerk2);
	bool WINAPI nmc_ping(LPCSTR pstrHost);
#ifdef __cplusplus
	}
#endif
#endif

//DESCRIPTION  'NMC Windows Dynamic Link Library'     -- *def file* description ....
