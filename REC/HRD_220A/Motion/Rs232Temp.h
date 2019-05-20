//---------------------------------------------------------------------------

#ifndef Rs232TempH
#define Rs232TempH

#include <ExtCtrls.hpp>
#include "uRs232c.h"



enum EN_TEMP_RQST {
    trNone         = 0 ,
    trRqstCrntTemp     ,
    trSetTemp          ,
    trSetTempAll       ,
    MAX_TEMP_RQST
};

#define MAX_TEMP_CH 3
enum EN_TEMP_CH{
    tcRailBtm = 1,
    tcRailTop = 2,
    tcStage   = 3

};

class CTempConUnit
{
    public:
        __fastcall  CTempConUnit(void);
        __fastcall ~CTempConUnit(void);

        void Init();
        void Close();

    private:

    protected:
        AnsiString m_sRcvMsg ; //LF CR까지 다 들어온 풀메세지.
        EN_TEMP_RQST m_iLastRqst ;

        int m_iSetTemp [MAX_TEMP_CH] ;
        int m_iCrntTemp[MAX_TEMP_CH] ;

        AnsiString GetChksum(AnsiString sData);
        
        TRS232C *m_pRS232C;
        void RcvTempCon  (DWORD _cbInQue);

    public:
        //타이머로 돌려 주세요.
        bool   RqstCrntTemp(); //온도좀 보내주세요 하는것. 타이머에서 널널하게 호출 한다. 한 5초에 한번..

        //채널은 1번 부터 8번까지.
        //_iCh (1~8)
        bool   SetTemp    (int _iCh , int _iTemp); //온도 세팅.
        bool   SetTempAll (int _iTemp1 ,           //한큐 온도 세팅.
                           int _iTemp2 ,
                           int _iTemp3 );
        int    GetCrntTemp(int _iCh);                 //현재 컨트롤러상 실제 온도. RqstCrntTemp돌려줘야 갱신.
        int    GetSetTemp (int _iCh);                 //마지막 세팅시도된 온도.

        //메세지 보내는 함수 STX , CheckSum , LF , CR을 붙여서 보내줌.
        bool   SendMsg(AnsiString _sMsg);
    

        //메세지 처리 하는 함수. 폼에서 메세지 받으면 세팅해줌.
        void   SetRcvMsg(AnsiString _sMsg);
        //bool   LoadTemperature();

        String sRcvMsgTemp;

};

extern CTempConUnit  Rs232_TempCh ;

#endif
