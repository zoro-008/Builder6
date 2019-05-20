
//---------------------------------------------------------------------------
#ifndef LotUnitH
#define LotUnitH

#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Limits.h>
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "Array.h"

//---------------------------------------------------------------------------
#define MAX_LOT_CNT 50
#define LOG_FOLDER "D:\\SPC\\LotLog\\"


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
            int   iTotalChip      ; //토탈 갯수.
            int   iTotalFailChip  ; //토탈 페일 갯수.
        };

        //날별.
        struct SDayInfo {
            double     dRunTime    ;
            double     dIdleTime   ;
            double     dJamTime    ;
            double     dTotalTime  ;

            int        iWrkWafer   ;
            int        iMarkChip   ;
            int        iFailChip   ;
        };

        //랏별
        struct SLotInfo {
            //Input Info.
            AnsiString sWaferName  ;       
            int        iCrntLotNo  ;       
            AnsiString sJobFile    ;       
            AnsiString sOperator   ;       

            //Time.
            double     dStartTime  ;
            double     dEndTime    ;
            double     dRunTime    ;
            double     dIdleTime   ;
            double     dJamTime    ;
            double     dTotalTime  ;

            int        iWrkWafer   ;       
            int        iMarkChip   ;       
            int        iFailChip   ;       
        };

        SEqpInfo  EqpInfo    ;
        SDayInfo  DayInfo    ;
        SLotInfo  LotInfo    ;
        bool      m_bLotOpen ;
        bool      m_bLotEnd  ;

        __fastcall  CLot(void);
        __fastcall ~CLot(void);

        void __fastcall ClearEqpInfo(void);
        void __fastcall ClearLotInfo(void);
        void __fastcall ClearDayInfo(void);

        //Lot Processing.
        void __fastcall LotOpen  (AnsiString  _sLotNo = "" , AnsiString _sJobFile = "", AnsiString _sOperator = "");
        void __fastcall LotEnd   (void          );

        AnsiString __fastcall GetCrntLot (void);

        void __fastcall DispEqpInfo (TPanel * _pnTotalCnt ,
                                     TPanel * _pnFailCnt  ,
                                     TPanel * _pnGoodCnt  ,
                                     TPanel * _pnYield    );

        void __fastcall DispLotInfo (TPanel * _pnWaferName,
                                     TPanel * _pnJobFile  ,
                                     TPanel * _pnOperator ,
                                     TPanel * _pnStartTime,
                                     TPanel * _pnEndTime  ,
                                     TPanel * _pnRunTime  ,
                                     TPanel * _pnIdleTime ,
                                     TPanel * _pnJamTime  ,
                                     TPanel * _pnTotalTime,
                                     TPanel * _pnStrpUPEH ,
                                     TPanel * _pnStrpUPH  ,
                                     TPanel * _pnWorkStrp ,
                                     TPanel * _pnWorkChip ,
                                     TPanel * _pnFailChip );


        void __fastcall DispDayInfo(TPanel * _pnRunTime   ,
                                    TPanel * _pnIdleTime  ,
                                    TPanel * _pnJamTime   ,
                                    TPanel * _pnTotalTime ,
                                    TPanel * _pnStrpUPEH  ,
                                    TPanel * _pnStrpUPH   ,
                                    TPanel * _pnWorkStrp  ,
                                    TPanel * _pnMarkChip  ,
                                    TPanel * _pnFailChip  );

        //File Processing.
        void __fastcall LoadEqpInfo() ;
        void __fastcall SaveEqpInfo() ;


        void __fastcall LoadLotInfo() ;
        void __fastcall SaveLotInfo() ;

        void __fastcall LoadDayInfo() ;
        void __fastcall SaveDayInfo() ;

        void __fastcall WriteLotLog   () ;
        void __fastcall WriteLotDayLog() ;

        void __fastcall DelPastLotLog () ;

        //Update Lot Efficiency.
        void __fastcall Update(EN_SEQ_STAT Stat);

        //노냥 기록 하기 -_-;
        void __fastcall AddEqpInfoWorkChip(int _iNo){
            if(EqpInfo.iTotalChip + _iNo >= INT_MAX) ClearEqpInfo();
            else                                     EqpInfo.iTotalChip += _iNo ;
        }
        void __fastcall AddEqpInfoFailChip(int _iNo){
            if(EqpInfo.iTotalFailChip + _iNo >= INT_MAX) ClearEqpInfo();
            else                                         EqpInfo.iTotalFailChip += _iNo ;
        }


        //적절히 운용 한다. 김대기.
        void __fastcall AddDayInfoWorkStrp(int _iNo){DayInfo.iWrkWafer += _iNo ;}
        void __fastcall AddDayInfoMarkChip(int _iNo){DayInfo.iMarkChip += _iNo ;}
        void __fastcall AddDayInfoFailChip(int _iNo){DayInfo.iFailChip += _iNo ;}

        void __fastcall AddLotInfoWrkWafer(int _iNo){LotInfo.iWrkWafer += _iNo ;}
        void __fastcall AddLotInfoMarkChip(int _iNo){LotInfo.iMarkChip += _iNo ;}
        void __fastcall AddLotInfoFailChip(int _iNo){LotInfo.iFailChip += _iNo ;}

        bool __fastcall WriteArayData(EN_ARAY_ID riId                   );
        bool __fastcall ReadArayData (AnsiString sPath , EN_ARAY_ID riId);

};

extern CLot LT;

//---------------------------------------------------------------------------
#endif
