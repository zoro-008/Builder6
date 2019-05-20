//---------------------------------------------------------------------------

#ifndef AdlinkTenkeyH
#define AdlinkTenkeyH
//---------------------------------------------------------------------------
#include "Dask.h"
class CAdlinkTenkey
{
     private:

//        HANDLE      m_hComiDA;    //ComiZoa Handle.
//        HANDLE      m_hComiAD;

//        I16         m_hAdDA  ;   //Adlink Handle.
//        I16         m_hAdAD  ;   //Adlink Handle.
        int m_iSetNo ;
        int m_iInputCnt ;
        AnsiString m_iInputChar ;
        I16 card, card_number;



    public:

         CAdlinkTenkey();
        ~CAdlinkTenkey();

        int  __fastcall GetSetNo () {return m_iSetNo;}
        int  __fastcall GetInputCnt() { return m_iInputCnt;}
        AnsiString __fastcall GetInputChar() { return m_iInputChar ; }

        void __fastcall Update   ();
        void __fastcall GetInput ();
        void __fastcall SetOutput();

};
extern CAdlinkTenkey ADL;
#endif
