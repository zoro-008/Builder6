// CommThread.cpp: implementation of the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueue::CQueue()
{
	Clear();
}

CQueue::~CQueue()
{
}

void CQueue::Clear()
{
	m_nHead = m_nTail = 0;
	memset(m_buff, 0, BUFF_SIZE);
}

int CQueue::GetSize()
{
	return (m_nHead-m_nTail+BUFF_SIZE)%BUFF_SIZE;
}

BOOL CQueue::PutByte(BYTE b)
{
	//큐가 꽉 찼는지 검사
	if(GetSize() == (BUFF_SIZE-1)) return FALSE;
	//큐에 1 byte를 넣고 head position 증가
	m_buff[m_nHead++] = b;
	m_nHead %= BUFF_SIZE;

	return TRUE;
}

BOOL CQueue::GetByte(BYTE *pb)
{
	//큐가 비었는지 검사
	if(GetSize() == 0) return FALSE;
	//큐에서 1 byte를 꺼내고 tail position 증가
	*pb = m_buff[m_nTail++];
	m_nTail %= BUFF_SIZE;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommThread::CCommThread()
{
	m_hOwnerWnd = NULL;
	m_nDataStatus=0;
	m_hReadEvent=NULL;
}

CCommThread::~CCommThread()
{
}

int CCommThread::GetString(BYTE *pByte)
{
	BYTE nV;
    int k=0;
	while(m_qRead.GetByte(&nV))
	{
		pByte[k]=nV;
		k++;
	}
	return k;
}
void CCommThread::ClearData()
{
	m_qRead.Clear();
}

BOOL CCommThread::Open(CString sPortName, DWORD dwBaud, WORD wPortID)
{
	COMMTIMEOUTS timeouts;		//timeout value를 설정하기 위한 구조체
	DCB dcb;					//serial port의 특성을 설정하기 위한 구조체
	DWORD dwThreadID;			//스레드 생성시 스레드 ID를 얻음

	m_bConnected = FALSE;
	m_wPortID = wPortID;		//COM1->1, COM2->2, ...
	
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if(!(m_osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL))) return FALSE;

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if(!(m_osWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL))) return FALSE;

	// sPortName : "COM1", "COM2", ...
	m_sPortName = sPortName;
	m_fhComm = CreateFile(m_sPortName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, NULL);
	if(m_fhComm == INVALID_HANDLE_VALUE) return FALSE;
	
	//이벤트 설정(EV_RXCHAR)
	SetCommMask(m_fhComm, EV_RXCHAR);
	SetupComm(m_fhComm, BUFF_SIZE, BUFF_SIZE);
	PurgeComm(m_fhComm, PURGE_TXABORT|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_RXCLEAR);

	timeouts.ReadIntervalTimeout = 0xffffffff;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600/dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(m_fhComm, &timeouts);

	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_fhComm, &dcb);
	dcb.BaudRate = dwBaud;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = 0;
	dcb.fInX = dcb.fOutX = TRUE;	// Xon,Xoff를 사용
									// Xon,Xoff를 사용하지 않을 경우는 
									// CriticalSection을 설정하여 보호해야 함
	dcb.XonChar = ASCII_XON;
	dcb.XoffChar = ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;
	if(!SetCommState(m_fhComm,&dcb)) return FALSE;
///////////////////////////////////////
	m_hReadEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	::ResetEvent(m_hReadEvent);
///////////////////////////////////////
	m_bConnected = TRUE;
	m_hThreadWatchComm = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);
	if(!m_hThreadWatchComm)
	{
		Close();
		return FALSE;
	}

	return TRUE;
}

void CCommThread::SetOwnerWindow(HWND hWnd)
{
	m_hOwnerWnd = hWnd;
}

void CCommThread::Close()
{
	m_bConnected = FALSE;
	SetCommMask(m_fhComm, 0);
	PurgeComm(m_fhComm, PURGE_TXABORT|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_RXCLEAR);
	CloseHandle(m_fhComm);
///////////////////////////////////	

	CloseHandle(m_hThreadWatchComm);
	m_hThreadWatchComm = NULL;
	CloseHandle(m_hReadEvent);
	m_hReadEvent=NULL;
////////////////////////////////////
}

void CCommThread::InitEvent()
{
      ::ResetEvent(m_hReadEvent);

}
int CCommThread::WaitForReadingEnd(DWORD time_out)
{
	return	WaitForSingleObject(m_hReadEvent,time_out);
}


DWORD CCommThread::Write(BYTE *pBuff, DWORD nToWrite)
{
	DWORD dwWritten,dwError,dwErrorFlags;
	COMSTAT comstat;

	if(!WriteFile(m_fhComm,pBuff,nToWrite,&dwWritten,&m_osWrite))
	{
		// 읽을 문자나 전송할 문자가 남아 있을경우
		// Overlapped IO의 특성에 따라 ERROR_IO_PENDING 메시지가 전달됨
		if(GetLastError() == ERROR_IO_PENDING)
		{
			// timeout에 정해준 시간만큼 기다린다.
			while(!GetOverlappedResult(m_fhComm,&m_osWrite,&dwWritten,TRUE))
			{
				dwError = GetLastError();
				if(dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_fhComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError(m_fhComm, &dwErrorFlags, &comstat);
		}
	}

	return dwWritten;
}

DWORD CCommThread::Read(BYTE *pBuff, DWORD nToRead)
{
	DWORD dwRead,dwError,dwErrorFlags;
	COMSTAT comstat;

	ClearCommError(m_fhComm, &dwErrorFlags, &comstat);
	dwRead = comstat.cbInQue;

	if(dwRead > 0)
	{
		if(!ReadFile(m_fhComm,pBuff,nToRead,&dwRead,&m_osRead))
		{
			// 읽을 문자나 전송할 문자가 남아 있을경우
			// Overlapped IO의 특성에 따라 ERROR_IO_PENDING 메시지가 전달됨
			if(GetLastError() == ERROR_IO_PENDING)
			{
				// timeout에 정해준 시간만큼 기다린다.
				while(!GetOverlappedResult(m_fhComm,&m_osRead,&dwRead,TRUE))
				{
					dwError = GetLastError();
					if(dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_fhComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError(m_fhComm, &dwErrorFlags, &comstat);
			}
		}
	}

	return dwRead;
}

DWORD ThreadWatchComm(CCommThread *pComm)
{
	int i;
	int UsefulCharLen;
	DWORD dwEvent;
	OVERLAPPED os;
	BOOL bOk=TRUE;	// 리턴값
	BYTE buff[BUFF_SIZE];
	DWORD dwRead;
//	int ntest;

	memset(&os, 0, sizeof(OVERLAPPED));
	if(!(os.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL))) bOk = FALSE;
	if(!SetCommMask(pComm->m_fhComm, EV_RXCHAR)) bOk = FALSE;
	if(!bOk)
	{
		AfxMessageBox("Error in creating thread !!!");
		::PostMessage(pComm->m_hOwnerWnd, WM_COMM_THREAD, pComm->m_wPortID, 0);
		return FALSE;
	}
	
	while(pComm->m_bConnected)
	{
		dwEvent = 0;
		WaitCommEvent(pComm->m_fhComm, &dwEvent, NULL);
////////////////////////////////////////////////////////////////////
// sleep for waiting data
//
		::Sleep(100);
		if((dwEvent&EV_RXCHAR) == EV_RXCHAR)//이벤트가 데이타 입력이면
		{
 			do 
			{
				dwRead = pComm->Read(buff,BUFF_SIZE);
				
				UsefulCharLen = (BUFF_SIZE-pComm->m_qRead.GetSize());
				if(UsefulCharLen > (int)dwRead)
				{
					for(i=0; i<(int)dwRead; i++)
						pComm->m_qRead.PutByte(buff[i]);
				}
				else//itoa
				{
					AfxMessageBox("QUEUE is full !!!");
					::PostMessage(pComm->m_hOwnerWnd, WM_COMM_QUEUE, pComm->m_wPortID, 0);
				}
				Sleep(0);
			} while(dwRead);

			if(pComm->m_hOwnerWnd != NULL)
			{
			   ::PostMessage(pComm->m_hOwnerWnd, WM_COMM_READ, pComm->m_wPortID, 0);
			   ::SetEvent(pComm->m_hReadEvent);
			}
		}
		Sleep(0);
	}

	CloseHandle(os.hEvent);
 
	return TRUE;
}
