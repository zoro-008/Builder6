//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "uRs232c.h"
#include "UserIni.h"
#include "SLogUnit.h"
//#include "Rs232Man.h"

#pragma package(smart_init)

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
        WaitCommEvent(myRs232c->hmyPort, &dwEventMask, &myRs232c->evnRead);
        ::Sleep(1);
        if(!dwEventMask) continue;
        EnterCriticalSection(&myRs232c->myCriticalSection);
        if(myRs232c->IsOpened())
//        if(CallArr[myRs232c->myID] != NULL)
        {
            if(dwEventMask & EV_RXCHAR)
            {
                //------------ ���Ź��ۿ� ������ ���ڰ� ���� �Ǿ��� ���
                ClearCommError(myRs232c->hmyPort, &dwErrorFlags, &CommStatus);
                myRs232c->CallArr[myRs232c->myID](CommStatus.cbInQue);
//                CallArr[myRs232c->myID](CommStatus.cbInQue);
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
        LeaveCriticalSection(&myRs232c->myCriticalSection);
    }
}

//---------------------------------------------------------------------------
// ������
__fastcall TRS232C::TRS232C()      
{
    myRxThread      = NULL;
    hmyWnd          = NULL;
    hmyPort         = NULL;
    dwmyTimeOut     = (DWORD)COMM_TIMEOUT;
    dwmyEventMask   = EV_RXCHAR;
    imyTxBuffSize   = TX_BUFF_LEN;
    imyRxBuffSize   = RX_BUFF_LEN;
    dwCount         = 4096;

    evnRead.hEvent  = NULL;
    evnWrite.hEvent = NULL;
    SetDefaultDCB();

    SetDtrControl(DTR_DISABLE);
    SetRtsControl(RTS_DISABLE);

    //Add dd
    bStart    = false ;
    bComplete = false ;
    sData     = ""    ;

    memset(&bData, 0 , sizeof(bData));

    InitializeCriticalSection(&myCriticalSection);
}
//---------------------------------------------------------------------------
__fastcall TRS232C::~TRS232C()
{
    if(hmyPort) Close();
	DeleteCriticalSection(&myCriticalSection);
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
    myDCB.fDtrControl = DTR_DISABLE;            /* DTR Flow control                */
    myDCB.fDsrSensitivity = false;              /* DSR Sensitivity              */
    myDCB.fTXContinueOnXoff = false;            /* Continue TX when Xoff sent */
    myDCB.fOutX = false;                        /* Enable output X-ON/X-OFF        */
    myDCB.fInX = false;                         /* Enable input X-ON/X-OFF         */
    myDCB.fErrorChar = false;                   /* Enable Err Replacement          */
    myDCB.fNull = false;                        /* Enable Null stripping           */
    myDCB.fRtsControl = RTS_DISABLE;            /* Rts Flow control                */
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
void __fastcall TRS232C::SetDCB(DCB *mDCB)
{
    myDCB.DCBlength = mDCB->DCBlength;                  /* sizeof(DCB)                     */
    myDCB.BaudRate = mDCB->BaudRate;                    /* Baudrate at which running       */
    myDCB.fBinary = mDCB->fBinary;                      /* Binary Mode (skip EOF check)    */
    myDCB.fParity = mDCB->fParity;                      /* Enable parity checking          */
    myDCB.fOutxCtsFlow = mDCB->fOutxCtsFlow;            /* CTS handshaking on output       */
    myDCB.fOutxDsrFlow = mDCB->fOutxDsrFlow;            /* DSR handshaking on output       */
    myDCB.fDtrControl = mDCB->fDtrControl;              /* DTR Flow control                */
    myDCB.fDsrSensitivity = mDCB->fDsrSensitivity;      /* DSR Sensitivity              */
    myDCB.fTXContinueOnXoff = mDCB->fTXContinueOnXoff;  /* Continue TX when Xoff sent */
    myDCB.fOutX = mDCB->fOutX;                          /* Enable output X-ON/X-OFF        */
    myDCB.fInX = mDCB->fInX;                            /* Enable input X-ON/X-OFF         */
    myDCB.fErrorChar = mDCB->fErrorChar;                /* Enable Err Replacement          */
    myDCB.fNull = mDCB->fNull;                          /* Enable Null stripping           */
    myDCB.fRtsControl = mDCB->fRtsControl;              /* Rts Flow control                */
    myDCB.fAbortOnError = mDCB->fAbortOnError;          /* Abort all reads and writes on Error */
//    myDCB.fDummy2 = NULL;                             /* Reserved                        */
    myDCB.wReserved = mDCB->wReserved;                  /* Not currently used              */
    myDCB.XonLim = mDCB->XonLim;                        /* Transmit X-ON threshold         */
    myDCB.XoffLim = mDCB->XoffLim;                      /* Transmit X-OFF threshold        */
    myDCB.ByteSize = mDCB->ByteSize;                    /* Number of bits/byte, 4-8        */
    myDCB.Parity = mDCB->Parity;                        /* 0-4=None,Odd,Even,Mark,Space    */
    myDCB.StopBits = mDCB->StopBits;                    /* 0,1,2 = 1, 1.5, 2               */
    myDCB.XonChar = mDCB->XonChar;                      /* Tx and Rx X-ON character        */
    myDCB.XoffChar = mDCB->XoffChar;                    /* Tx and Rx X-OFF character       */
    myDCB.ErrorChar = mDCB->ErrorChar;                  /* Error replacement char          */
    myDCB.EofChar = mDCB->EofChar;                      /* End of Input character          */
    myDCB.EvtChar = mDCB->EvtChar;                      /* Received Event character        */
//    myDCB.wReserved1 = 0;                             /* Fill for now.                   */
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
// ������ ���Ž� Timeout���� �����Ѵ�. ������  Millisecond
void __fastcall TRS232C::SetTimeOut(DWORD dwTime)
{
    dwmyTimeOut = dwTime;
}
//---------------------------------------------------------------------------
// ��ſ� ���� �ۼ��� ������ ũ�⸦ �����Ѵ�
void __fastcall TRS232C::SetCommBuffSize(int iTxBuffSize, int iRxBuffSize)
{
    imyTxBuffSize = iTxBuffSize;
    imyRxBuffSize = iRxBuffSize;
}
//---------------------------------------------------------------------------
// �����忡 ���� ȣ��� ���� �̺�Ʈ Mask���� �����Ѵ�
// �������� winbase.h�� define�Ǿ��� Events���� �����Ͽ�(OR ���� ���) ����Ѵ�
// �⺻������ EV_RXCHAR���� �����ڿ��� ������ ���Ҵ�
void __fastcall TRS232C::SetEventMask(DWORD  dwEventMask)
{
    dwmyEventMask = dwEventMask;
}
//---------------------------------------------------------------------------
// DTR��Ʈ�� ��뿩�� ����(�⺻������ �����ڿ��� DTR_DISABLE�� �����Ͽ���)
void __fastcall TRS232C::SetDtrControl(DWORD dwDtrControl)
{
    myDCB.fDtrControl = dwDtrControl;
}
//---------------------------------------------------------------------------
// RTS��Ʈ�� ��뿩�� ����(�⺻������ �����ڿ��� RTS_DISABLE�� �����Ͽ���)
void __fastcall TRS232C::SetRtsControl(DWORD dwRtsControl)
{
    myDCB.fRtsControl = dwRtsControl;
}
//---------------------------------------------------------------------------
// ���� �����带 �����Ѵ�. �ڵ����� �����ϱ� ���� CreateSuspended���� false�� ��)
void __fastcall TRS232C::CreateRxThread(void)
{
    myRxThread = new TRS232CRxThread(this, false);
}
//---------------------------------------------------------------------------
// ���� �����带 �Ҹ��Ų��
void __fastcall TRS232C::DestroyRxThread(void)
{
    if(myRxThread)
    {
        delete myRxThread;
        myRxThread = NULL;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TRS232C::CallBackReg(CallbackKey _CallbackKey)
{
    if(myID >= MAX_RS232) return false ;
    if(!IsOpened()      ) return false ;

    CallArr[myID] = _CallbackKey ;
    return true ;
}
bool __fastcall TRS232C::Open(int _iId)
{
    if(_iId >= MAX_RS232) return false ;
    myID = _iId ;

    Load(_iId) ;

//    CallArr[_iId] = _CallbackKey ;

    char DeviceName[10];

    memset(DeviceName , 0 , 10);

    if(IsOpened()) Close();

    //-- Device Name ����
    sprintf(DeviceName, "COM%d", Para.PortNo);

    //-- Event Overlap ����
    if(!CreateEventLap()) return(false);

    //-- Port Handle ���
 	hmyPort = CreateFile(   DeviceName,
                            GENERIC_READ|GENERIC_WRITE,
                            0,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                            NULL );

	if(hmyPort == INVALID_HANDLE_VALUE)
    {
        hmyPort = 0;
        return(false);
    }
    //-- Event Mask ����
	if(!SetCommMask(hmyPort, dwmyEventMask))
    {
    	Close();
        return(false);
    }
    //-- ��� ���� ������ ����
	if(!SetupComm(hmyPort, (DWORD)imyRxBuffSize,(DWORD)imyTxBuffSize))
    {
    	Close();
		return(false);
    }
    //-- ��� Timeout�� ����
	COMMTIMEOUTS	TimeOuts;
	TimeOuts.ReadIntervalTimeout		= 0xFFFFFFFF;
	TimeOuts.ReadTotalTimeoutMultiplier	= 0;
	TimeOuts.ReadTotalTimeoutConstant	= dwmyTimeOut;
	TimeOuts.WriteTotalTimeoutMultiplier= 0;
	TimeOuts.WriteTotalTimeoutConstant	= 0;
	if(!SetCommTimeouts(hmyPort, &TimeOuts))
    {
    	Close();
		return(false);
    }
    //-- ��� �ӵ����� DCD�� ����
    myDCB.BaudRate = (DWORD)Para.BaudRate;
    myDCB.ByteSize = (BYTE)Para.ByteSize;

    switch(Para.StopBits)
    {
        case 0:
            myDCB.StopBits = STOPBIT_1;
            break;
        case 1:
            myDCB.StopBits = STOPBIT_15;
            break;
        case 2:
            myDCB.StopBits = STOPBIT_2;
            break;
    }
    switch(Para.Parity)
    {
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
    	if(!SetCommState(hmyPort, &myDCB))
    {
    	Close();
        return(false);
    }
    //-- ��/���� ���� Purge
    PurgeComm(hmyPort, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
    //-- ���� ������ ����
    CreateRxThread();

    ClearTxBuffer();
    ClearRxBuffer();
    bComCnt = true;

    return(true);
}

//---------------------------------------------------------------------------
// ������ ��� Port��  Close �Ѵ�(���� ������ ����)
void __fastcall TRS232C::Close(void)
{
    DestroyRxThread();         
    PurgeComm(hmyPort, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	if(evnRead.hEvent  != NULL) { CloseHandle(evnRead.hEvent);  evnRead.hEvent  = NULL; }
	if(evnWrite.hEvent != NULL) { CloseHandle(evnWrite.hEvent); evnWrite.hEvent = NULL; }
	if(hmyPort   	   != NULL) { CloseHandle(hmyPort); 		hmyPort 	   	= NULL; }
}
//---------------------------------------------------------------------------
// ��� Port�� Open�Ǿ������� Ȯ���Ѵ�
bool __fastcall TRS232C::IsOpened(void)
{
    return((hmyPort != NULL));
}
//---------------------------------------------------------------------------
// ���� ���ۿ� ����� ���� iCount��ŭ lpRxBuff�� �о�´�
// Return���� ���� ���ۿ��� �о�� ���� Return
int __fastcall TRS232C::ReadData(int iCount, BYTE *lpRxBuff)
{
    DWORD   dwResultCount;

    if(!IsOpened()) return(0);

    //Sleep(100);    //�Ƴ� �ùٷ�Ѥ��ڤӤӤ��ֶ�ӡ���� �𸣰ٴ�.....�������������������� �������.
    if(!ReadFile(hmyPort, lpRxBuff, iCount, &dwResultCount, &evnRead))
    {
        if(GetLastError() == ERROR_IO_PENDING)
        {
            while(!GetOverlappedResult(hmyPort, &evnRead, &dwResultCount, FALSE))
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

    if(!hmyPort) return(false);

    if (!WriteFile(hmyPort, lpTxBuff, (DWORD)iCount, &dwWrittenCount, &evnWrite))
    {
        dwErrorFlags = GetLastError();
        if(dwErrorFlags == ERROR_IO_PENDING)
        {
            while(!GetOverlappedResult(hmyPort, &evnWrite, &dwWrittenCount, FALSE))
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

    Trace((AnsiString(myID)+ "Cn") .c_str() , lpTxBuff);
    return(isWritten);
}
//---------------------------------------------------------------------------
// ��� Port�� Handle���� Return�Ѵ�
HANDLE  __fastcall TRS232C::GetCommHandle(void)
{
    return(hmyPort);
}
//---------------------------------------------------------------------------
// ���� ���� Clear
void __fastcall TRS232C::ClearRxBuffer(void)
{
    if(hmyPort) PurgeComm(hmyPort, PURGE_RXCLEAR);
}
//---------------------------------------------------------------------------
// ��� ���� Clear
void __fastcall TRS232C::ClearTxBuffer(void)
{
    if(hmyPort) PurgeComm(hmyPort, PURGE_TXCLEAR);
}
//---------------------------------------------------------------------------


void TRS232C::Save(int _iId)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sId     ;
    AnsiString sPath   ;

    //Set Dir.
    sId   = (AnsiString)_iId + "_RS232" ;
    sPath = EXE_FOLDER + "Util\\RS_232.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath, sId  , "PortNo  " , Para.PortNo   );
    UserINI.Save(sPath, sId  , "BaudRate" , Para.BaudRate );
    UserINI.Save(sPath, sId  , "ByteSize" , Para.ByteSize );
    UserINI.Save(sPath, sId  , "StopBits" , Para.StopBits );
    UserINI.Save(sPath, sId  , "Parity  " , Para.Parity   );
}
void TRS232C::Load(int _iId)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sId     ;
    AnsiString sPath   ;

    //Set Dir.
    sId   = (AnsiString)_iId + "_RS232" ;
    sPath = EXE_FOLDER + "Util\\RS_232.INI" ;

    //Save Device.
    UserINI.Load(sPath, sId  , "PortNo  " , Para.PortNo   );
    UserINI.Load(sPath, sId  , "BaudRate" , Para.BaudRate );
    UserINI.Load(sPath, sId  , "ByteSize" , Para.ByteSize );
    UserINI.Load(sPath, sId  , "StopBits" , Para.StopBits );
    UserINI.Load(sPath, sId  , "Parity  " , Para.Parity   );

}
