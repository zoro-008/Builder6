//---------------------------------------------------------------------------

#ifndef uRs232cH
#define uRs232cH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <winbase.h>
//---------------------------------------------------------------------------
#define COMM_SOH                0x01
#define COMM_STX                0x02
#define COMM_ETX                0x03
#define COMM_EOT                0x04
#define COMM_ENQ                0x05
#define COMM_ACK                0x06
#define COMM_BEL                0x07
#define COMM_LF                 0x0a
#define COMM_CR                 0x0d
#define COMM_SO                 0x0e
#define COMM_SI                 0x0f
#define COMM_DLE                0x10
#define COMM_TIME               0x11
#define COMM_FULL               0x12
#define COMM_BLD                0x13
#define COMM_NAK                0x15
#define COMM_SYN                0x16
#define COMM_FAIL               0x18
#define COMM_EOM                0x19
#define COMM_ESC                0x1b
#define COMM_FS                 0x1c
//---------------------------------------------------------------------------
#define UM_EVN_RXCHAR           WM_USER+990
#define UM_EVN_RXFLAG           WM_USER+991
#define UM_EVN_TXEMPTY          WM_USER+992
#define UM_EVN_CTS              WM_USER+993
#define UM_EVN_DSR              WM_USER+994
#define UM_EVN_RLSD             WM_USER+995
#define UM_EVN_BREAK            WM_USER+996
#define UM_EVN_ERR              WM_USER+997
#define UM_EVN_RING             WM_USER+998
#define UM_EVN_RX80FULL         WM_USER+999

#define UM_EVN_RXCHAR_PB1       WM_USER+1000
#define UM_EVN_RXCHAR_PB2       WM_USER+1001
#define UM_EVN_RXCHAR_BAR1      WM_USER+1002
#define UM_EVN_RXCHAR_TEMP      WM_USER+1003
//---------------------------------------------------------------------------
#define TX_BUFF_LEN             4096
#define RX_BUFF_LEN             4096
#define COMM_TIMEOUT            3000                // milliseconds
//---------------------------------------------------------------------------
enum TStopBits {
    STOPBIT_1  = ONESTOPBIT   ,
    STOPBIT_15 = ONE5STOPBITS ,
    STOPBIT_2  = TWOSTOPBITS
};
//---------------------------------------------------------------------------
enum TParity {
    PRTBIT_NONE  = NOPARITY,
    PRTBIT_ODD	 = ODDPARITY,
    PRTBIT_EVEN  = EVENPARITY,
    PRTBIT_MARK	 = MARKPARITY,
    PRTBIT_SPACE = SPACEPARITY
};
//---------------------------------------------------------------------------
enum TDtrControl {
    DTR_DISABLE   = DTR_CONTROL_DISABLE ,
    DTR_ENABLE    = DTR_CONTROL_ENABLE  ,
    DTR_HANDSHAKE = DTR_CONTROL_HANDSHAKE
};
//---------------------------------------------------------------------------
enum TRtsControl {
    RTS_DISABLE   = RTS_CONTROL_DISABLE,
    RTS_ENABLE	  = RTS_CONTROL_ENABLE,
    RTS_HANDSHAKE = RTS_CONTROL_HANDSHAKE,
    RTS_TOGGLE	  = RTS_CONTROL_TOGGLE
};
//---------------------------------------------------------------------------
struct TPara
{
    int PortNo;
    int BaudRate;
    int ByteSize;
    int StopBits;
    int Parity;
};


//---------------------------------------------------------------------------
class TRS232C;
//---------------------------------------------------------------------------
class TRS232CRxThread : public TThread
{
protected:
    TRS232C   *myRs232c;
    void __fastcall Execute();
public:
    __fastcall TRS232CRxThread(TRS232C *Rs232c, bool CreateSuspended) : TThread(CreateSuspended)
    {
        myRs232c = Rs232c;
    }
};
//---------------------------------------------------------------------------
class TRS232C
{
friend class TRS232CRxThread;
private:	// User declarations
    DWORD               dwmyTimeOut;
    DWORD               dwmyEventMask;
    int                 imyTxBuffSize;
    int                 imyRxBuffSize;
    DCB                 myDCB;

    void __fastcall SetDefaultDCB(void);
    bool __fastcall CreateEventLap(void);
    void __fastcall CreateRxThread(void);
    void __fastcall DestroyRxThread(void);
    void Save(int _iId) ;
    void Load(int _iId) ;
protected:
    TRS232CRxThread     *myRxThread;
    HWND                hmyWnd;
    HANDLE              hmyPort;
    int                 myID   ;
    CRITICAL_SECTION	myCriticalSection;

    TPara                Para ;

public:		// User declarations
    OVERLAPPED      evnRead;
    OVERLAPPED      evnWrite;

    __fastcall      TRS232C();
    __fastcall      ~TRS232C();

    void __fastcall SetDCB(DCB *mDCB);

    //Default Setting Func
    void __fastcall SetTimeOut(DWORD dwTime);
    void __fastcall SetCommBuffSize(int iTxBuffSize, int iRxBuffSize);
    void __fastcall SetEventMask(DWORD  dwEventMask);
    void __fastcall SetDtrControl(DWORD dwDtrControl);
    void __fastcall SetRtsControl(DWORD dwRtsControl);

    bool __fastcall Open(int _iId,HWND hWnd);
    void __fastcall Close(void);
    bool __fastcall IsOpened(void);
    int  __fastcall ReadData(int iCount, BYTE *lpRxBuff);
    bool __fastcall WriteData(int iCount, BYTE *lpTxBuff);

    HANDLE  __fastcall GetCommHandle(void);
    void __fastcall ClearRxBuffer(void);
    void __fastcall ClearTxBuffer(void);
};
//---------------------------------------------------------------------------
extern TRS232C *RS232C;
//---------------------------------------------------------------------------
#endif
