//---------------------------------------------------------------------------

#ifndef TempConUnitH
#define TempConUnitH

#include <ExtCtrls.hpp>

#define MAX_TEMP_CH 8

enum EN_TEMP_RQST {
    trNone         = 0 ,
    trRqstCrntTemp     ,
    trSetTemp          ,
    trSetTempAll       ,
    MAX_TEMP_RQST
};

class CTempConUnit
{
    public:
        __fastcall  CTempConUnit(void);
        __fastcall ~CTempConUnit(void);

    private:
        AnsiString m_sRcvMsg ; //LF CR까지 다 들어온 풀메세지.
        EN_TEMP_RQST m_iLastRqst ;

        int m_iSetTemp [MAX_TEMP_CH] ;
        int m_iCrntTemp[MAX_TEMP_CH] ;

        AnsiString GetChksum(AnsiString sData);

    public:

        //타이머로 돌려 주세요.
        bool   RqstCrntTemp(); //온도좀 보내주세요 하는것. 타이머에서 널널하게 호출 한다. 한 5초에 한번..

        //채널은 1번 부터 8번까지.
        //_iCh (1~8)
        bool   SetTemp    (int _iCh , int _iTemp); //온도 세팅.
        bool   SetTempAll (int _iTemp1 , //한큐 온도 세팅.
                           int _iTemp2 ,
                           int _iTemp3 ,
                           int _iTemp4 ,
                           int _iTemp5 ,
                           int _iTemp6 ,
                           int _iTemp7 ,
                           int _iTemp8);
        bool   SetTempAll (int _iTemp) ; //한큐 온도 세팅... 선과장님 배신!!!

        int    GetCrntTemp(int _iCh);                 //현재 컨트롤러상 실제 온도. RqstCrntTemp돌려줘야 갱신.
        int    GetSetTemp (int _iCh);                 //마지막 세팅시도된 온도.

        //메세지 보내는 함수 STX , CheckSum , LF , CR을 붙여서 보내줌.
        bool   SendMsg(AnsiString _sMsg);
    

        //메세지 처리 하는 함수. 폼에서 메세지 받으면 세팅해줌.
        void   SetRcvMsg(AnsiString _sMsg);
        bool   LoadTemperature();

};
//---------------------------------------------------------------------------
extern CTempConUnit TCU;
//---------------------------------------------------------------------------
#endif
