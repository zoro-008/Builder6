#ifndef _MYMATHSRC_H_
#define _MYMATHSRC_H_
#include "Mmsystem.h"
#include "digitalv.h"
#ifdef __cplusplus
	extern "C" {
#endif


/*
 *	char * 를 넘겨 줄때 array size 는 MAX_PATH 임 
 *  MAX_PATH   255
 */

//  Define 정의
#ifndef  MYDEFINE
#define  MYDEFINE
	#define GETPOINTSTR(p)  GetPointerStr(#p)
	// Pointer Operation
	#define SET_NULL(p) { (p)=NULL; } 
	#define SAFE_DELETE(p)       { if(p) { delete (p);    } (p)=NULL; } 
	#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);  } (p)=NULL; } 
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release();} (p)=NULL; } 

	#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0]) 
	
	#define MIN(x,y) ( ((x)<(y))?(x):(y) ) 
	#define MAX(x,y) ( ((x)>(y))?(x):(y) ) 

	#define SWAP(a,b)  {(a)^=(b)^=(a)^=(b);} 
#endif

#ifndef _PAIX_BITOPERATION_
#define _PAIX_BITOPERATION_
	#define BITSET(val,bit)		(val) |= (1<<(bit))
	#define BITCLEAR(val,bit)	(val) &= (~(1<<(bit)))
	#define BITTOGGLE(val,bit)	(val) ^= (1<<(bit))
	#define BITVALUE(val, sdata, bit)   (val) = (((sdata)>> bit) & 0x01)
	#define GETBIT(bytes, bit_no) (((bytes)>>(bit_no))&0x01)
	#define SETBIT(bytes, bit_no, val)    { (bytes) &=~(1<<(bit_no)); (bytes) |= ((val) << (bit_no)); }
#endif

		
// MCI 관련
	void WINAPI MCI_SetPosition(DWORD dwPosition);
	DWORD WINAPI MCI_GetPosition(void);
	DWORD WINAPI MCI_GetMode(void);
	DWORD WINAPI MCI_GetRunningTime(void);
	BOOL WINAPI MCI_OpenWav(char *pFile);
	BOOL WINAPI MCI_OpenMP3(char *pFile);

	BOOL WINAPI MCI_Pause(void);
	BOOL WINAPI MCI_Stop(void);
	BOOL WINAPI MCI_Play(void);
	int WINAPI MCI_GetVolume(void);
	void WINAPI MCI_SetVolume(DWORD dwVolume);
// Sound 관련
	bool WINAPI SoundCardInstalled(void);
// KeyBoard 관련
	bool WINAPI GetCapslock(void);
	bool WINAPI GetNumLock(void);
	bool WINAPI GetScrollLock(void);
	void WINAPI SetCapsLock(bool bCapsLockOn);
	void WINAPI SetNumLock(bool NumLockOn);
	void WINAPI SetScrollLock(bool ScrollLockOn);

// 수학 관련
	bool WINAPI IsPrime(__int64 N);
	__int64 WINAPI LCM(__int64 A, __int64 B);
	__int64 WINAPI GCD(__int64 A, __int64 B);
	bool WINAPI DoubleEqual(double X, double Y);
	double  WINAPI CalcMachineEpsilon(void);
	__int64 WINAPI Fact(__int64 nFact);
	int WINAPI MaxBound(int x,int b);
	int WINAPI MinBound(int x,int b);
	int WINAPI Bound(int X,int BS,int BE);

// 메시지 관련
	void WINAPI ProcessMessage(void);
// 디렉토리 관련
	void WINAPI GetWindowsPath(char *Buff);
	void WINAPI GetSystemPath(char *Buff);
	void WINAPI GetProgramPath(char *Buff);

// 화면 관련

	int WINAPI GetMonitorNum(void);
	void WINAPI DesktopSize(SIZE *size);
	HDC  WINAPI GetDesktopDC(void);
	COLORREF WINAPI RGBToGray(COLORREF RGBColor);
	POINT WINAPI RatioSize(POINT Source,POINT Target);
	/*
	 *	nStatus 
	*/
#ifndef MONITOR_STATUS
#define MONITOR_STATUS	
	#define MONITOR_TURNOFF		0
	#define MONITOR_TURNON		1
	#define MONITOR_LOWPOWER	2
#endif
	void WINAPI SetMonitorStatus(int nStatus);

	bool WINAPI BoxIn(int DataX,int DataY,int X1,int Y1,int X2,int Y2);
// OS 관련
	void WINAPI RestartWindows(void);
	void WINAPI ShutDownSystem(void);
	void WINAPI LogOffSystem(void);
	void WINAPI ShowDesktopIcons(BOOL Show);
	BOOL WINAPI GetOsVer(char *OsString);
// 시스템 관련
	void WINAPI EnableTaskSwitchs(BOOL Enable);
	double WINAPI CPUSpeed(void);
	BOOL WINAPI CPUIDAvail (void);
	void WINAPI GetBrandString(char *cpustr);
	void WINAPI GetOSLanguage(char *OsLanguage);
	bool WINAPI ExecNewProcess(char *FullName);
	void WINAPI SystemDoModal(int iDialogID, HWND hParent);
	WORD WINAPI GetUserLanguage(void);
// Font 관련
	bool WINAPI FontAdd( char *FontFilename);
	bool WINAPI FontRemove( char *FontFilename);
// 시간 관련 -- 1초의 CPU 클럭을 가져옴
	__int64 WINAPI GetCpuTickInit(void);
	void WINAPI GetCpuTickStart(void);
	void WINAPI GetCpuTickEnd(void);
	double WINAPI GetCpuTickGetTime(void);
	void WINAPI GetCpuTick(__int64 *count);
// 스트링 관련
	int WINAPI Stringspliter(char *szSrc, int nSzLen, char *szFilter,int nFilerLen, 
			char *szDest, int nInx);
	char * WINAPI GetPointerStr(char *str);

	// 하드디스크 관련
	/*
        DRIVE_UNKNOWN		: The drive type cannot be determined.
        DRIVE_NO_ROOT_DIR	: The root path is invalid. For example, no volume is mounted at the path.
        DRIVE_REMOVABLE		: The disk can be removed from the drive.
        DRIVE_FIXED			: The disk cannot be removed from the drive.
        DRIVE_REMOTE		: The drive is a remote (network) drive.
        DRIVE_CDROM			: The drive is a CD-ROM drive.
        DRIVE_RAMDISK		: The drive is a RAM disk.
	 */
	int WINAPI DriveExists(char *pDrive);	// 하드 존재 여부 체크
	DWORD WINAPI GetVolumeSerial(char *pDrive);
// NotePad
	void WINAPI DumpToNotePAD(char *buffer);
// Edit Align
	/*
	#define ES_LEFT             0x0000L
	#define ES_CENTER           0x0001L
	#define ES_RIGHT            0x0002L
	#define ES_MULTILINE        0x0004L
	#define ES_UPPERCASE        0x0008L
	#define ES_LOWERCASE        0x0010L
	#define ES_PASSWORD         0x0020L
	#define ES_AUTOVSCROLL      0x0040L
	#define ES_AUTOHSCROLL      0x0080L
	#define ES_NOHIDESEL        0x0100L
	#define ES_OEMCONVERT       0x0400L
	#define ES_READONLY         0x0800L
	#define ES_WANTRETURN       0x1000L
	#if(WINVER >= 0x0400)
		#define ES_NUMBER           0x2000L
	#endif 
	*/
	void WINAPI SetEditAlign( HWND hWnd,int nAlign );


	bool WINAPI AppExec(char *CmdLine, char *CmdParams,int CmdShow);
	bool WINAPI AppIsResponding(char *ClassName, UINT uTimeout);
	void WINAPI BringMyAppToFront(HANDLE AppHandle);

// 인터넷 관련
	bool WINAPI CloseInternetConnection(void);
	bool WINAPI NetworkIsPresent(void);
	bool WINAPI EstablishInternetConnection(void);
// 마우스 관련
	int WINAPI GetMouseButtons(void);
	bool WINAPI MouseHasWheel(void);
	bool WINAPI MouseButtonsSwaped(void);
	bool WINAPI MouseIsPresent(void);
	void WINAPI EmptyMouseQueue(void);
	// Not Test
	void WINAPI AppRestart(void);
#ifdef __cplusplus
	}
#endif
		
#endif
