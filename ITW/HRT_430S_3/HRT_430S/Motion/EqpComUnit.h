//---------------------------------------------------------------------------
#ifndef EqpComUnitH
#define EqpComUnitH

#include "UtilDefine.h"
#include "Timer.h"

enum EN_PST_EQP_SEND //장비간 통신 프로토콜.
{
    psNone      = 0 ,
    psStart     = 1 ,
    psStop      = 2 ,
    psReset     = 3 ,
    psLotStart  = 4 ,
    psLotEnd    = 5 ,
    psSending   = 6 , //자제 보내고 있는중.
    psJobChange = 7 ,
    psSpare1    = 8 ,

    MAX_PST_EQP_SEND
};

enum EN_PRE_EQP_SEND //장비간 통신 프로토콜.
{
    rsNone      = 0 ,
    rsStart     = 1 ,
    rsStop      = 2 ,
    rsReset     = 3 ,
    rsReady     = 4 ,
    rsSpare1    = 5 ,
    rsSpare2    = 6 ,
    rsSpare3    = 7 ,
    rsSpare4    = 8 ,

    MAX_PRE_EQP_SEND
};

class CEqpComUnit
{
    private:
        void ThreadPreEqpMsg ();
        void ThreadPstEqpMsg ();

        bool m_bPstEqpReady  ;
        bool m_bPreEqpLotEnd ;

    public:
        CEqpComUnit(void);
        ~CEqpComUnit(void);

        bool GetPstEqpReady ();
        bool GetPreEqpLotEnd();

        void SetPstEqpLotEnd (bool _bOn);
        void SetPreEqpReady  (bool _bOn);
        void SetPstEqpSending(bool _bOn);

        //Send IO.
        void SendPreEqp(EN_PRE_EQP_SEND _iMsg) ;
        void SendPstEqp(EN_PST_EQP_SEND _iMsg) ;


        void Update();

        //Network Drive Connect
        bool __fastcall NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath);
        bool __fastcall NetCancelConnection(AnsiString sDrv);


        
        

        //Eqip Communication.
        bool __fastcall ReadArayData (EN_ARAY_ID riId);
        bool __fastcall WriteArayData(EN_ARAY_ID riId);

        //Lot Data Communication.
        bool __fastcall ReadLotData (void);
        bool __fastcall WriteLotData(void); //랏 오픈시에

        //Device Change Data Communication.
        bool __fastcall WriteChange (void); //디바이스 체인지 시에.
        bool __fastcall ReadChange  (void);


};

extern CEqpComUnit EC;



#endif
