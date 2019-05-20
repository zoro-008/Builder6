
//---------------------------------------------------------------------------
#ifndef LotUnitH
#define LotUnitH

#include <Limits.h>
#include "SMInterfaceUnit.h"

//---------------------------------------------------------------------------
#define MAX_LOT_CNT 50
#define LOG_FOLDER "D:\\LotLog\\"


extern AnsiString g_sLotNo[MAX_LOT_CNT] ;

void       PushLotNo     (AnsiString _sLotNo); //ť�� ���� ����.
AnsiString GetLotNo      (void              ); //������.
AnsiString PopLotNo      (void              ); //������ ����. ť����.
void       DeleteLotNo   (AnsiString _sLotNo); //�ش� ���ѹ��� �����.
void       DeleteLotNo   (int        _iLotNo); //�������ʸ� �����.
void       DeleteLotNoOne(                  ); //End one del
void       DeleteLotNoAll(void              ); //ť�� ��� �����.
int        GetLotCnt     (void              ); //ť�� ��� �����.
//Add
void       PushTopLotNo  (AnsiString _sLotNo); //ť ������ ���� ����.

class CLot
{
    private:
    public :
        //��� ���.
        struct SEqpInfo {
            double  dRunTime   ; // �Ϸ� �� �� Ÿ��
            int     iTotalWork ; //��Ż ����.
            int     iTodayWork ; // �Ϸ� �۾� ����
        };
        struct SDayInfo {
            String     sWorkedDay    ;
        };


        SEqpInfo  EqpInfo    ;
        SDayInfo  DayInfo    ;
        bool      m_bLotOpen ;
        bool      m_bLotEnd  ;
        bool      m_bRqstLotEnd ;

        __fastcall  CLot(void);
        __fastcall ~CLot(void);

        void Init();

        void Close();

        void __fastcall ClearEqpInfo(void);
        void __fastcall ClearDayInfo(void);

        //Lot Processing.
        void __fastcall LotOpen  (AnsiString  _sLotNo = "" , AnsiString _sJobFile = "", AnsiString _sOperator = "");
        void __fastcall LotEnd   (void          );

        void __fastcall DispEqpInfo (TPanel * _pnTotalTime , TPanel * _pnTodayWork );

        //File Processing.
        void __fastcall LoadEqpInfo() ;
        void __fastcall SaveEqpInfo() ;

        void __fastcall LoadDayInfo() ;
        void __fastcall SaveDayInfo() ;

        void __fastcall WriteLotDayLog() ;

        void __fastcall DelPastLotLog () ;

        //Update Lot Efficiency.
        void __fastcall Update(EN_SEQ_STAT Stat);

        //��� ��� �ϱ� -_-;
        void __fastcall AddTotalWorkCnt(int _iNo){
            if(EqpInfo.iTotalWork + _iNo >= INT_MAX) ClearEqpInfo();
            else                                     EqpInfo.iTotalWork += _iNo ;
        }
        //������ ��� �Ѵ�. ����.
        void __fastcall AddTodayWorkCnt(int _iNo){EqpInfo.iTodayWork += _iNo ;}


};

extern CLot LT;

//---------------------------------------------------------------------------
#endif
