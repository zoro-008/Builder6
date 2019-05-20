//---------------------------------------------------------------------------

#ifndef AdlinkUnitH
#define AdlinkUnitH
//---------------------------------------------------------------------------
#include "Dask.h"
class CAdlinkUnit
{
     private:

//        HANDLE      m_hComiDA;    //ComiZoa Handle.
//        HANDLE      m_hComiAD;

//        I16         m_hAdDA  ;   //Adlink Handle.
//        I16         m_hAdAD  ;   //Adlink Handle.
        int m_iSetNo ;
        I16 card, card_number;

    public:

         CAdlinkUnit();
        ~CAdlinkUnit();


        void __fastcall Update   ();
        void __fastcall GetInput ();
        void __fastcall SetOutput();

};
extern CAdlinkUnit ADL;
#endif
