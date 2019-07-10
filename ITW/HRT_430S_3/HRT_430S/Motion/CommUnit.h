//---------------------------------------------------------------------------
#ifndef CommUnitH
#define CommUnitH

#include "UtilDefine.h"
class CCommUnit
{
    private:

    public:
        CCommUnit(void);
        ~CCommUnit(void);
        void __fastcall Init(void);
        //Network Drive Connect
        void __fastcall NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath);

        //Eqip Communication.
        void __fastcall ReadPreEqip  (EN_ARAY_ID riId);
        bool __fastcall WritePostEqip(EN_ARAY_ID riId);

        //Vision Communication.
        bool __fastcall ReadVision  (int iVisnNo);

        //Lot Data Communication.
        void __fastcall ReadLotData (void);
        void __fastcall WriteLotData(void); //랏 오픈시에

        //Device Change Data Communication.
        void __fastcall WriteChange (void); //디바이스 체인지 시에.
        void __fastcall ReadChange  (void);


};

extern CCommUnit COM;

#endif
