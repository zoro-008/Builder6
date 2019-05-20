//---------------------------------------------------------------------------

#ifndef Rs232cH
#define Rs232cH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <winbase.h>
//---------------------------------------------------------------------------
typedef void (*RxCallback)(DWORD _dTxCnt);
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
        struct TPara {
            int PortNo  ;
            int BaudRate;
            int ByteSize;
            int StopBits;
            int Parity  ;
        };

        DCB    myDCB        ;

        void __fastcall SetDefaultDCB  (void);
        bool __fastcall CreateEventLap (void);
        void Load(bool _bLoad , int _iId) ;
    protected:
        TRS232CRxThread    *m_pRxThread;
        HANDLE              m_hPort;
        int                 m_iId   ;
        CRITICAL_SECTION    m_tCriticalSection;
        RxCallback          m_pCallback;

        TPara               Para ;

    public:		// User declarations
        OVERLAPPED      evnRead;
        OVERLAPPED      evnWrite;

        __fastcall      TRS232C();
        __fastcall      ~TRS232C();

        bool __fastcall SetCallBack(RxCallback _pCallback);
        bool __fastcall Open       (int _iId);

        void __fastcall Close(void);
        bool __fastcall IsOpened(void);
        int  __fastcall ReadData(int iCount, BYTE *lpRxBuff);
        bool __fastcall SendData(int iCount, BYTE *lpTxBuff);

        void __fastcall ClearRxBuffer(void);
        void __fastcall ClearTxBuffer(void);

        //Add dd
    //    void __fastcall WriteData(int iCount, BYTE *lpTxBuff);
    //    void __fastcall Complete (void);
};
//---------------------------------------------------------------------------
#endif
