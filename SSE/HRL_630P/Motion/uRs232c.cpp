//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "uRs232c.h"
#include "UserIni.h"
#include "SLogUnit.h"
//#include "Rs232Man.h"

#pragma package(smart_init)

//이거 만들어 놓기는 햇는데...
//분명 430 할때 병신 같아던거는 기억나는데 모가 불편한지 기억이 안남...
//하고 나니 똑같음;;;dd

//---------------------------------------------------------------------------
// 데이타가 수신되었는지 여부를 확인하는 Thread 처리 함수
// Event발생시 사용자가 정의한 MessageMap에 선언된 함수를 호출한다
void __fastcall TRS232CRxThread::Execute()
{
    DWORD   dwEventMask;
    DWORD   dwErrorFlags;
    COMSTAT CommStatus;

	while(!Terminated)
    {
        dwEventMask = 0;
        //--- 통신 버퍼의 이벤트를 기다린다
        WaitCommEvent(myRs232c->hmyPort, &dwEventMask, &myRs232c->evnRead);
        ::Sleep(1);
        if(!dwEventMask) continue;
        EnterCriticalSection(&myRs232c->myCriticalSection);
        if(myRs232c->IsOpened())
//        if(CallArr[myRs232c->myID] != NULL)
        {
            if(dwEventMask & EV_RXCHAR)
            {
                //------------ 수신버퍼에 데이터 문자가 수신 되었을 경우
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
// 생성자
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
// 데이타 송수신 Event처리 Overlap을 생성한다
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
// 데이터 수신시 Timeout값을 지정한다. 단위는  Millisecond
void __fastcall TRS232C::SetTimeOut(DWORD dwTime)
{
    dwmyTimeOut = dwTime;
}
//---------------------------------------------------------------------------
// 통신에 사용될 송수신 버퍼의 크기를 지정한다
void __fastcall TRS232C::SetCommBuffSize(int iTxBuffSize, int iRxBuffSize)
{
    imyTxBuffSize = iTxBuffSize;
    imyRxBuffSize = iRxBuffSize;
}
//---------------------------------------------------------------------------
// 쓰레드에 의해 호출될 수신 이벤트 Mask값을 지정한다
// 지정값은 winbase.h에 define되어진 Events값을 조합하여(OR 연산 사용) 사용한다
// 기본적으로 EV_RXCHAR값은 생성자에서 지정해 놓았다
void __fastcall TRS232C::SetEventMask(DWORD  dwEventMask)
{
    dwmyEventMask = dwEventMask;
}
//---------------------------------------------------------------------------
// DTR콘트롤 사용여부 지정(기본적으로 생성자에서 DTR_DISABLE로 설정하였음)
void __fastcall TRS232C::SetDtrControl(DWORD dwDtrControl)
{
    myDCB.fDtrControl = dwDtrControl;
}
//---------------------------------------------------------------------------
// RTS콘트롤 사용여부 지정(기본적으로 생성자에서 RTS_DISABLE로 설정하였음)
void __fastcall TRS232C::SetRtsControl(DWORD dwRtsControl)
{
    myDCB.fRtsControl = dwRtsControl;
}
//---------------------------------------------------------------------------
// 수신 쓰레드를 생성한다. 자동으로 수행하기 위해 CreateSuspended값을 false로 함)
void __fastcall TRS232C::CreateRxThread(void)
{
    myRxThread = new TRS232CRxThread(this, false);
}
//---------------------------------------------------------------------------
// 수신 쓰레드를 소멸시킨다
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

    //-- Device Name 생성
    sprintf(DeviceName, "COM%d", Para.PortNo);

    //-- Event Overlap 생성
    if(!CreateEventLap()) return(false);

    //-- Port Handle 얻기
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
    //-- Event Mask 지정
	if(!SetCommMask(hmyPort, dwmyEventMask))
    {
    	Close();
        return(false);
    }
    //-- 통신 버퍼 사이지 지정
	if(!SetupComm(hmyPort, (DWORD)imyRxBuffSize,(DWORD)imyTxBuffSize))
    {
    	Close();
		return(false);
    }
    //-- 통신 Timeout값 지정
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
    //-- 통신 속도등의 DCD값 지정
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
    //-- 송/수신 버퍼 Purge
    PurgeComm(hmyPort, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
    //-- 수신 쓰레드 생성
    CreateRxThread();

    ClearTxBuffer();
    ClearRxBuffer();
    bComCnt = true;

    return(true);
}

//---------------------------------------------------------------------------
// 열려진 통신 Port를  Close 한다(수신 쓰레드 포함)
void __fastcall TRS232C::Close(void)
{
    DestroyRxThread();         
    PurgeComm(hmyPort, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	if(evnRead.hEvent  != NULL) { CloseHandle(evnRead.hEvent);  evnRead.hEvent  = NULL; }
	if(evnWrite.hEvent != NULL) { CloseHandle(evnWrite.hEvent); evnWrite.hEvent = NULL; }
	if(hmyPort   	   != NULL) { CloseHandle(hmyPort); 		hmyPort 	   	= NULL; }
}
//---------------------------------------------------------------------------
// 통신 Port가 Open되었는지를 확인한다
bool __fastcall TRS232C::IsOpened(void)
{
    return((hmyPort != NULL));
}
//---------------------------------------------------------------------------
// 수신 버퍼에 저장된 값을 iCount만큼 lpRxBuff에 읽어온다
// Return값은 수신 버퍼에서 읽어온 값을 Return
int __fastcall TRS232C::ReadData(int iCount, BYTE *lpRxBuff)
{
    DWORD   dwResultCount;

    if(!IsOpened()) return(0);

    //Sleep(100);    //아나 싀바루ㅡㅡ자ㅣㅣㄼ주라ㅣ줃라ㅣ 모르겟당.....ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ 수정요망.
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
// 통신 버퍼에 iCount만큼 lpTxBuff의 내용을 송신한다
// 올바로 송신하면 true를 Return
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
// 통신 Port의 Handle값을 Return한다
HANDLE  __fastcall TRS232C::GetCommHandle(void)
{
    return(hmyPort);
}
//---------------------------------------------------------------------------
// 수신 버퍼 Clear
void __fastcall TRS232C::ClearRxBuffer(void)
{
    if(hmyPort) PurgeComm(hmyPort, PURGE_RXCLEAR);
}
//---------------------------------------------------------------------------
// 통신 버퍼 Clear
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
