//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "Rs232c.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "Rs232Man.h"

#pragma package(smart_init)

#define TX_BUFF_LEN   4096
#define RX_BUFF_LEN   4096
#define COMM_TIMEOUT  3000 // milliseconds

//�̰� ����� ����� �޴µ�...
//�и� 430 �Ҷ� ���� ���ƴ��Ŵ� ��ﳪ�µ� �� �������� ����� �ȳ�...
//�ϰ� ���� �Ȱ���;;;dd

//---------------------------------------------------------------------------
// ����Ÿ�� ���ŵǾ����� ���θ� Ȯ���ϴ� Thread ó�� �Լ�
// Event�߻��� ����ڰ� ������ MessageMap�� ����� �Լ��� ȣ���Ѵ�
void __fastcall TRS232CRxThread::Execute()
{
    DWORD   dwEventMask;
    DWORD   dwErrorFlags;
    COMSTAT CommStatus;

	while(!Terminated)
    {
        dwEventMask = 0;
        //--- ��� ������ �̺�Ʈ�� ��ٸ���
        WaitCommEvent(myRs232c->m_hPort, &dwEventMask, &myRs232c->evnRead);
        ::Sleep(1);
        if(!dwEventMask) continue;
        EnterCriticalSection(&myRs232c->m_tCriticalSection);
        if(myRs232c->IsOpened()) {
            if(dwEventMask & EV_RXCHAR) {
                //------------ ���Ź��ۿ� ������ ���ڰ� ���� �Ǿ��� ���
                ClearCommError(myRs232c->m_hPort, &dwErrorFlags, &CommStatus);
                if(myRs232c->m_pCallback!=NULL)myRs232c->m_pCallback(CommStatus.cbInQue);
            }
//            if(dwEventMask & EV_RXFLAG)   SendMessage(myRs232c->hmyWnd, UM_EVN_RXFLAG, 0, 0);
//            if(dwEventMask & EV_TXEMPTY)  SendMessage(myRs232c->hmyWnd, UM_EVN_TXEMPTY, 0, 0);
//            if(dwEventMask & EV_CTS)      SendMessage(myRs232c->hmyWnd, UM_EVN_CTS, 0, 0);
//            if(dwEventMask & EV_DSR)      SendMessage(myRs232c->hmyWnd, UM_EVN_DSR, 0, 0);
//            if(dwEventMask & EV_RLSD)     SendMessage(myRs232c->hmyWnd, UM_EVN_RLSD, 0, 0);
//            if(dwEventMask & EV_BREAK)    SendMessage(myRs232c->hmyWnd, UM_EVN_BREAK, 0, 0);
//            if(dwEventMask & EV_ERR)      SendMessage(myRs232c->hmyWnd, UM_EVN_ERR, 0, 0);
//            if(dwEventMask & EV_RING)     SendMessage(myRs232c->hmyWnd, UM_EVN_RING, 0, 0);
//            if(dwEventMask & EV_RX80FULL) SendMessage(myRs232c->hmyWnd, UM_EVN_RX80FULL, 0, 0);
        }
        LeaveCriticalSection(&myRs232c->m_tCriticalSection);
    }
}

//---------------------------------------------------------------------------
// ������
__fastcall TRS232C::TRS232C()      
{
    m_pRxThread     = NULL;
    m_hPort         = NULL;
    m_pCallback     = NULL;

    evnRead.hEvent  = NULL;
    evnWrite.hEvent = NULL;
    SetDefaultDCB();


    InitializeCriticalSection(&m_tCriticalSection);
}
//---------------------------------------------------------------------------
__fastcall TRS232C::~TRS232C()
{
    if(m_hPort) Close();
	DeleteCriticalSection(&m_tCriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TRS232C::SetDefaultDCB(void)
{
    myDCB.DCBlength = sizeof(DCB);              /* sizeof(DCB)                     */
    myDCB.BaudRate = CBR_9600;                  /* Baudrate at which running       */
    myDCB.fBinary = true;                       /* Binary Mode (skip EOF check)    */
    myDCB.fParity = true;                       /* Enable parity checking          */
    myDCB.fOutxCtsFlow = false;                 /* CTS handshaking on output       */
    myDCB.fOutxDsrFlow = false;                 /* DSR handshaking on output       */
    myDCB.fDtrControl = DTR_CONTROL_DISABLE;    /* DTR Flow control                */
    myDCB.fDsrSensitivity = false;              /* DSR Sensitivity              */
    myDCB.fTXContinueOnXoff = false;            /* Continue TX when Xoff sent */
    myDCB.fOutX = false;                        /* Enable output X-ON/X-OFF        */
    myDCB.fInX = false;                         /* Enable input X-ON/X-OFF         */
    myDCB.fErrorChar = false;                   /* Enable Err Replacement          */
    myDCB.fNull = false;                        /* Enable Null stripping           */
    myDCB.fRtsControl = RTS_CONTROL_DISABLE;    /* Rts Flow control                */
    myDCB.fAbortOnError = false;                /* Abort all reads and writes on Error */
//    myDCB.fDummy2 = NULL;                       /* Reserved                        */
    myDCB.wReserved = 0;                        /* Not currently used              */
    myDCB.XonLim = 0;                           /* Transmit X-ON threshold         */
    myDCB.XoffLim = 0;                          /* Transmit X-OFF threshold        */
    myDCB.ByteSize = 8;                         /* Number of bits/byte, 4-8        */
    myDCB.Parity = NOPARITY;                    /* 0-4=None,Odd,Even,Mark,Space    */
    myDCB.StopBits = ONESTOPBIT;                /* 0,1,2 = 1, 1.5, 2               */
    myDCB.XonChar = 0x00;                       /* Tx and Rx X-ON character        */
    myDCB.XoffChar = 0x00;                      /* Tx and Rx X-OFF character       */
    myDCB.ErrorChar = 0x00;                     /* Error replacement char          */
    myDCB.EofChar = 0x00;                       /* End of Input character          */
    myDCB.EvtChar = 0x00;                       /* Received Event character        */
//    myDCB.wReserved1 = 0;                       /* Fill for now.                   */
}
//---------------------------------------------------------------------------
// ����Ÿ �ۼ��� Eventó�� Overlap�� �����Ѵ�
bool __fastcall TRS232C::CreateEventLap(void)
{
    evnRead.Offset = 0;
    evnRead.OffsetHigh = 0;
    evnWrite.Offset = 0;
    evnWrite.OffsetHigh  = 0;

    evnRead.hEvent  = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(!evnRead.hEvent) return(false);

    evnWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(!evnWrite.hEvent)
    {
        CloseHandle(evnRead.hEvent);
        evnRead.hEvent = NULL;
        return(false);
    }
    return(true);
}
//---------------------------------------------------------------------------
bool __fastcall TRS232C::SetCallBack(RxCallback _pCallback)
{
    if(!IsOpened()) return false ;

    m_pCallback = _pCallback ;
    return true ;
}
bool __fastcall TRS232C::Open(int _iId)
{
    m_iId = _iId ;

    Load(true ,_iId) ;

//    CallArr[_iId] = _CallbackKey ;

    char DeviceName[10];

    memset(DeviceName , 0 , 10);

    if(IsOpened()) Close();

    //-- Device Name ����
    sprintf(DeviceName, "COM%d", Para.PortNo);

    //-- Event Overlap ����
    if(!CreateEventLap()) return(false);

    //-- Port Handle ���
 	m_hPort = CreateFile(   DeviceName,
                            GENERIC_READ|GENERIC_WRITE,
                            0,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                            NULL );

    if(m_hPort == INVALID_HANDLE_VALUE){
        m_hPort = 0;
        return(false);
    }
    //-- Event Mask ����
    if(!SetCommMask(m_hPort, EV_RXCHAR)){
    	Close();
        return(false);
    }
    //-- ��� ���� ������ ����
    if(!SetupComm(m_hPort, (DWORD)RX_BUFF_LEN,(DWORD)TX_BUFF_LEN)){
    	Close();
	return(false);
    }
    //-- ��� Timeout�� ����
    COMMTIMEOUTS TimeOuts;
    TimeOuts.ReadIntervalTimeout	= 0xFFFFFFFF;
    TimeOuts.ReadTotalTimeoutMultiplier	= 0;
    TimeOuts.ReadTotalTimeoutConstant	= COMM_TIMEOUT;
    TimeOuts.WriteTotalTimeoutMultiplier= 0;
    TimeOuts.WriteTotalTimeoutConstant	= 0;
    if(!SetCommTimeouts(m_hPort, &TimeOuts)){
    	Close();
	return(false);
    }
    //-- ��� �ӵ����� DCD�� ����
    myDCB.BaudRate = (DWORD)Para.BaudRate;
    myDCB.ByteSize = (BYTE)Para.ByteSize ;

    switch(Para.StopBits){
        case 0:
            myDCB.StopBits = ONESTOPBIT;
            break;
        case 1:
            myDCB.StopBits = ONE5STOPBITS;
            break;
        case 2:
            myDCB.StopBits = TWOSTOPBITS;
            break;
    }
    switch(Para.Parity){
        case 0:
            myDCB.Parity = PARITY_NONE;
            break;
        case 1:
            myDCB.Parity = PARITY_EVEN;
            break;
        case 2:
            myDCB.Parity = PARITY_ODD;
            break;
    }
    if(!SetCommState(m_hPort, &myDCB)){
    	Close();
        return(false);
    }
    //-- ��/���� ���� Purge
    PurgeComm(m_hPort, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
    //-- ���� ������ ����
    m_pRxThread = new TRS232CRxThread(this, false);

    ClearTxBuffer();
    ClearRxBuffer();

    return(true);
}

//---------------------------------------------------------------------------
// ������ ��� Port��  Close �Ѵ�(���� ������ ����)
void __fastcall TRS232C::Close(void)
{
    if(m_pRxThread) {
        delete m_pRxThread;
        m_pRxThread = NULL;
    }
    PurgeComm(m_hPort, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	if(evnRead.hEvent  != NULL) { CloseHandle(evnRead.hEvent);  evnRead.hEvent  = NULL; }
	if(evnWrite.hEvent != NULL) { CloseHandle(evnWrite.hEvent); evnWrite.hEvent = NULL; }
	if(m_hPort   	   != NULL) { CloseHandle(m_hPort); 		m_hPort 	   	= NULL; }
}
//---------------------------------------------------------------------------
// ��� Port�� Open�Ǿ������� Ȯ���Ѵ�
bool __fastcall TRS232C::IsOpened(void)
{
    return((m_hPort != NULL));
}
//---------------------------------------------------------------------------
// ���� ���ۿ� ����� ���� iCount��ŭ lpRxBuff�� �о�´�
// Return���� ���� ���ۿ��� �о�� ���� Return
int __fastcall TRS232C::ReadData(int iCount, BYTE *lpRxBuff)
{
    DWORD   dwResultCount;

    if(!IsOpened()) return(0);

    //Sleep(100);    //�Ƴ� �ùٷ�Ѥ��ڤӤӤ��ֶ�ӡ���� �𸣰ٴ�.....�������������������� �������.
    if(!ReadFile(m_hPort, lpRxBuff, iCount, &dwResultCount, &evnRead))
    {
        if(GetLastError() == ERROR_IO_PENDING)
        {
            while(!GetOverlappedResult(m_hPort, &evnRead, &dwResultCount, FALSE))
            {
                if(GetLastError() == ERROR_IO_INCOMPLETE)	continue;
                else
                {
                    break;
                }
            }
        }
    }
    return((int)dwResultCount);
}
//---------------------------------------------------------------------------
// ��� ���ۿ� iCount��ŭ lpTxBuff�� ������ �۽��Ѵ�
// �ùٷ� �۽��ϸ� true�� Return
bool __fastcall TRS232C::SendData(int iCount, BYTE *lpTxBuff)
{
    DWORD dwWrittenCount;
    DWORD dwErrorFlags;
    bool  isWritten;

    if(!m_hPort) return(false);

	if (!WriteFile(m_hPort, lpTxBuff, (DWORD)iCount, &dwWrittenCount, &evnWrite))
    {
        dwErrorFlags = GetLastError();
        if(dwErrorFlags == ERROR_IO_PENDING)
        {
            while(!GetOverlappedResult(m_hPort, &evnWrite, &dwWrittenCount, FALSE))
            {
                if(GetLastError() == ERROR_IO_INCOMPLETE)	continue;
                else
                {
                    break;
                }
            }
        }
    }
    isWritten = (dwWrittenCount == (DWORD)iCount);

    Trace((AnsiString(m_iId)+ "Cn") .c_str() , lpTxBuff);
    return(isWritten);
}
//---------------------------------------------------------------------------
// ���� ���� Clear
void __fastcall TRS232C::ClearRxBuffer(void)
{
    if(m_hPort) PurgeComm(m_hPort, PURGE_RXCLEAR);
}
//---------------------------------------------------------------------------
// ��� ���� Clear
void __fastcall TRS232C::ClearTxBuffer(void)
{
    if(m_hPort) PurgeComm(m_hPort, PURGE_TXCLEAR);
}
//---------------------------------------------------------------------------


void TRS232C::Load(bool _bLoad , int _iId)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sId     ;
    AnsiString sPath   ;

    //Set Dir.
    sId   = (AnsiString)_iId + "_RS232" ;
    sPath = EXE_FOLDER + "Util\\RS_232.INI" ;

    if(_bLoad){
        //Save Device.
        UserINI.Load(sPath, sId  , "PortNo  " , Para.PortNo   );
        UserINI.Load(sPath, sId  , "BaudRate" , Para.BaudRate );
        UserINI.Load(sPath, sId  , "ByteSize" , Para.ByteSize );
        UserINI.Load(sPath, sId  , "StopBits" , Para.StopBits );
        UserINI.Load(sPath, sId  , "Parity  " , Para.Parity   );
        Load(false , _iId);
    }
    else {
        UserINI.Save(sPath, sId  , "PortNo  " , Para.PortNo   );
        UserINI.Save(sPath, sId  , "BaudRate" , Para.BaudRate );
        UserINI.Save(sPath, sId  , "ByteSize" , Para.ByteSize );
        UserINI.Save(sPath, sId  , "StopBits" , Para.StopBits );
        UserINI.Save(sPath, sId  , "Parity  " , Para.Parity   );
    }

}
