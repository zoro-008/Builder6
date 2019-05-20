//---------------------------------------------------------------------------

#ifndef HeaterH
#define HeaterH
//---------------------------------------------------------------------------
#include "uRs232c.h"
#include "DataMan.h"

class CHeater
{
    public:
        //Constructor
        CHeater (void);
        ~CHeater (void);

        void Init();
        void Close();
        bool bStopTemp ;

    protected:
        static void RxCallback1(DWORD _dTxCnt); //시리얼 리시브 콜백.
        static void RxCallback2(DWORD _dTxCnt); //시리얼 리시브 콜백.
        TRS232C *   Rs232_1;
        TRS232C *   Rs232_2;

        int    HexToInt(String str) ;

        float  m_fCrntTemp1 ;
        float  m_fCrntTemp2 ;

        void   RqstTemp1();
        void   RqstTemp2();

        String m_sRcvQue1[64]  ; //일단 담아 놓는다.
        int    m_iMsgCnt1      ;
        String m_sRcvMsg1      ; //
        int    m_iRcvMsgCnt1   ; //응답 텍스트 숫자.

        String m_sRcvQue2[64]  ;
        int    m_iMsgCnt2      ;
        String m_sRcvMsg2      ;
        int    m_iRcvMsgCnt2   ;

    public:
        void   Update(); //뺑뺑이.


        void   SetTemp (int _iNo , int _iTemp);
        float  GetTemp1(          ); //Rail
        float  GetTemp2(          ); //Work

        void   Add_Chksum(char *pTxData,int iCnt);
};
extern CHeater HTR;

#endif
