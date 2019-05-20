
//---------------------------------------------------------------------------
#ifndef LotUnitH
#define LotUnitH

#include <Limits.h>
#include "SMInterfaceUnit.h"

//---------------------------------------------------------------------------
#define MAX_LOT_CNT 50
#define LOG_FOLDER "D:\\LotLog\\"


extern AnsiString g_sLotNo[MAX_LOT_CNT] ;

void       PushLotNo     (AnsiString _sLotNo); //큐에 집어 넣음.
AnsiString GetLotNo      (void              ); //참조만.
AnsiString PopLotNo      (void              ); //실제로 꺼냄. 큐에서.
void       DeleteLotNo   (AnsiString _sLotNo); //해당 랏넘버를 지운다.
void       DeleteLotNo   (int        _iLotNo); //다음차례만 지운다.
void       DeleteLotNoOne(                  ); //End one del
void       DeleteLotNoAll(void              ); //큐를 모두 지운다.
int        GetLotCnt     (void              ); //큐를 모두 지운다.
//Add
void       PushTopLotNo  (AnsiString _sLotNo); //큐 맨위에 집어 넣음.

class CLot
{
    private:
    public :
        //노냥 기록.
        struct SEqpInfo {
            double  dRunTime   ; // 하루 총 런 타임
            int     iTotalWork ; //토탈 갯수.
            int     iTodayWork ; // 하루 작업 갯수
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

        //노냥 기록 하기 -_-;
        void __fastcall AddTotalWorkCnt(int _iNo){
            if(EqpInfo.iTotalWork + _iNo >= INT_MAX) ClearEqpInfo();
            else                                     EqpInfo.iTotalWork += _iNo ;
        }
        //적절히 운용 한다. 김대기.
        void __fastcall AddTodayWorkCnt(int _iNo){EqpInfo.iTodayWork += _iNo ;}


};

extern CLot LT;

//---------------------------------------------------------------------------
#endif
