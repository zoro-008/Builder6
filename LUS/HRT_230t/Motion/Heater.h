//---------------------------------------------------------------------------

#ifndef HeaterH
#define HeaterH
//---------------------------------------------------------------------------
#include "Rs232c.h"
#include "DataMan.h"

class CHeater
{
    public:
        //Constructor
        CHeater (void);
        ~CHeater (void);
        bool bStopTemp ;

    protected:
        static void RxCallback(DWORD _dTxCnt); //시리얼 리시브 콜백.
        TRS232C Rs232c ;

        int    HexToInt(String str) ;

        float  m_fCrntTemp1 ;
        float  m_fCrntTemp2 ;

        void   RqstTemp1();
        void   RqstTemp2();

        String m_sRcvQue[64]  ; //일단 담아 놓는다.
        int    m_iMsgCnt      ;

        String m_sRcvMsg      ; //
        int    m_iRcvMsgCnt   ; //응답 텍스트 숫자.

    public:
        void   Update(); //뺑뺑이.

        void   SetTemp (int _iNo , int _iTemp);
        float  GetTemp1(          ); //Rail
        float  GetTemp2(          ); //Work
};
extern CHeater HTR;

#endif
