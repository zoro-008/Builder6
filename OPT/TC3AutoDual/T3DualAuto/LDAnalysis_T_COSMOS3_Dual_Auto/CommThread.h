// CommThread.h: interface for the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMTHREAD_H__A978E221_15D7_11D3_970B_0000862091BF__INCLUDED_)
#define AFX_COMMTHREAD_H__A978E221_15D7_11D3_970B_0000862091BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BUFF_SIZE		2048			// thread와 부모 프로세스가 데이터를 
										// 주고 받을 버퍼크기
#define WM_COMM_READ	(WM_APP+0x311)
#define WM_COMM_QUEUE	(WM_APP+0x312)
#define WM_COMM_THREAD	(WM_APP+0x313)

#define ASCII_XON		0x11
#define ASCII_XOFF		0x13

class CQueue
{
public:
	CQueue();
	virtual ~CQueue();
	void Clear();			//큐의 내용을 지우기
	int GetSize();			//큐에 들어있는 데이타의 길이를 리턴
	BOOL PutByte(BYTE b);	//큐에 1 byte 넣기
	BOOL GetByte(BYTE *pb);	//큐에서 1 byte 꺼내기
private:
	BYTE m_buff[BUFF_SIZE];	//큐 buffer
	int m_nHead,m_nTail;	//큐 haed,tail position
};

class CCommThread  
{
public:
	CCommThread();
	virtual ~CCommThread();
	//통신포트를 연다.인자로 포트명의 String과 속도,포트번호를 준다.
	BOOL Open(CString sPortName, DWORD dwBaud, WORD wParam);
	void SetOwnerWindow(HWND hWnd);
	void Close();
	DWORD Write(BYTE *pBuff, DWORD nToWrite);
	DWORD Read(BYTE *pBuff, DWORD nToRead);
	int GetString(BYTE *pByte);
	void ClearData();
/////////////////////////
// 추가 2009/12
// reading을 위해 ...
// 1. InitEvent()
// 2. Write()
// 3. WaitForReadingEnd()
// 4. GetString
//
	HANDLE m_hReadEvent;
	int WaitForReadingEnd(DWORD time_out);
	void InitEvent();
/////////////////////////
	HWND m_hOwnerWnd;
	HANDLE m_fhComm;	//통신포트 파일핸들
	CString m_sPortName;	//포트이름(COM1...)
	BOOL m_bConnected;	//포트가 열렸는지의 유무를 나타냄
	OVERLAPPED m_osRead,m_osWrite;	//포트파일 IO를 위한 Overlapped Structure
	HANDLE m_hThreadWatchComm;	//포트를 감시할 함수. 스레드의 핸들을 보관
	WORD m_wPortID;	//WM_COMM_READ와 함께 보내는 인수로 여러개의 포트중 
					//어떤 포트인지를 구분
	CQueue m_qRead;	//스레드와 부모 프로세스간의 통신버퍼
	int m_nDataStatus;
};

DWORD ThreadWatchComm(CCommThread *pComm);

#endif // !defined(AFX_COMMTHREAD_H__A978E221_15D7_11D3_970B_0000862091BF__INCLUDED_)
