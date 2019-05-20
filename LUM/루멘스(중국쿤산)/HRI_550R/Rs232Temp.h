//---------------------------------------------------------------------------

#ifndef Rs232TempH
#define Rs232TempH

#include <ExtCtrls.hpp>
#include "uRs232c.h"

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

        void Init();
        void Close();

    private:

    protected:
        AnsiString m_sRcvMsg ; //LF CR���� �� ���� Ǯ�޼���.
        EN_TEMP_RQST m_iLastRqst ;

        int m_iSetTemp [MAX_TEMP_CH] ;
        int m_iCrntTemp[MAX_TEMP_CH] ;

        AnsiString GetChksum(AnsiString sData);
        
        TRS232C *RS232C_TempCon;
        void RcvTempCon  (DWORD _cbInQue);

    public:
        //Ÿ�̸ӷ� ���� �ּ���.
        bool   RqstCrntTemp(); //�µ��� �����ּ��� �ϴ°�. Ÿ�̸ӿ��� �γ��ϰ� ȣ�� �Ѵ�. �� 5�ʿ� �ѹ�..

        //ä���� 1�� ���� 8������.
        //_iCh (1~8)
        bool   SetTemp    (int _iCh , int _iTemp); //�µ� ����.
        int    GetCrntTemp(int _iCh);                 //���� ��Ʈ�ѷ��� ���� �µ�. RqstCrntTemp������� ����.
        int    GetSetTemp (int _iCh);                 //������ ���ýõ��� �µ�.

        //�޼��� ������ �Լ� STX , CheckSum , LF , CR�� �ٿ��� ������.
        bool   SendMsg(AnsiString _sMsg);
    

        //�޼��� ó�� �ϴ� �Լ�. ������ �޼��� ������ ��������.
        void   SetRcvMsg(AnsiString _sMsg);
        //bool   LoadTemperature();

};
#endif
