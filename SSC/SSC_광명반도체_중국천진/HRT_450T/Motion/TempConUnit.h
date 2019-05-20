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
        AnsiString m_sRcvMsg ; //LF CR���� �� ���� Ǯ�޼���.
        EN_TEMP_RQST m_iLastRqst ;

        int m_iSetTemp [MAX_TEMP_CH] ;
        int m_iCrntTemp[MAX_TEMP_CH] ;

        AnsiString GetChksum(AnsiString sData);

    public:

        //Ÿ�̸ӷ� ���� �ּ���.
        bool   RqstCrntTemp(); //�µ��� �����ּ��� �ϴ°�. Ÿ�̸ӿ��� �γ��ϰ� ȣ�� �Ѵ�. �� 5�ʿ� �ѹ�..

        //ä���� 1�� ���� 8������.
        //_iCh (1~8)
        bool   SetTemp    (int _iCh , int _iTemp); //�µ� ����.
        bool   SetTempAll (int _iTemp1 , //��ť �µ� ����.
                           int _iTemp2 ,
                           int _iTemp3 ,
                           int _iTemp4 ,
                           int _iTemp5 ,
                           int _iTemp6 ,
                           int _iTemp7 ,
                           int _iTemp8);
        bool   SetTempAll (int _iTemp) ; //��ť �µ� ����... ������� ���!!!

        int    GetCrntTemp(int _iCh);                 //���� ��Ʈ�ѷ��� ���� �µ�. RqstCrntTemp������� ����.
        int    GetSetTemp (int _iCh);                 //������ ���ýõ��� �µ�.

        //�޼��� ������ �Լ� STX , CheckSum , LF , CR�� �ٿ��� ������.
        bool   SendMsg(AnsiString _sMsg);
    

        //�޼��� ó�� �ϴ� �Լ�. ������ �޼��� ������ ��������.
        void   SetRcvMsg(AnsiString _sMsg);
        bool   LoadTemperature();

};
//---------------------------------------------------------------------------
extern CTempConUnit TCU;
//---------------------------------------------------------------------------
#endif
