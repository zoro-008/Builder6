// CommThread.h: interface for the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMTHREAD_H__A978E221_15D7_11D3_970B_0000862091BF__INCLUDED_)
#define AFX_COMMTHREAD_H__A978E221_15D7_11D3_970B_0000862091BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BUFF_SIZE		2048			// thread�� �θ� ���μ����� �����͸� 
										// �ְ� ���� ����ũ��
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
	void Clear();			//ť�� ������ �����
	int GetSize();			//ť�� ����ִ� ����Ÿ�� ���̸� ����
	BOOL PutByte(BYTE b);	//ť�� 1 byte �ֱ�
	BOOL GetByte(BYTE *pb);	//ť���� 1 byte ������
private:
	BYTE m_buff[BUFF_SIZE];	//ť buffer
	int m_nHead,m_nTail;	//ť haed,tail position
};

class CCommThread  
{
public:
	CCommThread();
	virtual ~CCommThread();
	//�����Ʈ�� ����.���ڷ� ��Ʈ���� String�� �ӵ�,��Ʈ��ȣ�� �ش�.
	BOOL Open(CString sPortName, DWORD dwBaud, WORD wParam);
	void SetOwnerWindow(HWND hWnd);
	void Close();
	DWORD Write(BYTE *pBuff, DWORD nToWrite);
	DWORD Read(BYTE *pBuff, DWORD nToRead);
	int GetString(BYTE *pByte);
	void ClearData();
/////////////////////////
// �߰� 2009/12
// reading�� ���� ...
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
	HANDLE m_fhComm;	//�����Ʈ �����ڵ�
	CString m_sPortName;	//��Ʈ�̸�(COM1...)
	BOOL m_bConnected;	//��Ʈ�� ���ȴ����� ������ ��Ÿ��
	OVERLAPPED m_osRead,m_osWrite;	//��Ʈ���� IO�� ���� Overlapped Structure
	HANDLE m_hThreadWatchComm;	//��Ʈ�� ������ �Լ�. �������� �ڵ��� ����
	WORD m_wPortID;	//WM_COMM_READ�� �Բ� ������ �μ��� �������� ��Ʈ�� 
					//� ��Ʈ������ ����
	CQueue m_qRead;	//������� �θ� ���μ������� ��Ź���
	int m_nDataStatus;
};

DWORD ThreadWatchComm(CCommThread *pComm);

#endif // !defined(AFX_COMMTHREAD_H__A978E221_15D7_11D3_970B_0000862091BF__INCLUDED_)
