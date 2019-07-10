#ifndef	__AXT_COMM_DEFINE_H__
#define	__AXT_COMM_DEFINE_H__

//<+> 2003.03.18 JNS

// 패리티
#define PARITYBIT_NONE					0					// 패리티 사용하지 않음
#define PARITYBIT_ODD					1					// 홀수 패리티
#define PARITYBIT_EVEN					2					// 짝수 패리티

// 데이터 길이
#define DATABIT_8						0					// 데이터 길이 : 8 bit
#define DATABIT_7						1					// 데이터 길이 : 7 bit
#define DATABIT_6						2					// 데이터 길이 : 6 bit
#define DATABIT_5						3					// 데이터 길이 : 5 bit

// Stop bit
#define STOPBIT_1						0					// Stop bit : 1 bit
#define STOPBIT_2						1					// Stop bit : 2 bit

#endif