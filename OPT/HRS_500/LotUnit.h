
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
            int        iStrpUPEH   ;
            int        iChipUPEH   ;
            int        iStrpUPH    ;
            int        iChipUPH    ;
            int        iWorkStrp   ;
            int        iWorkChip   ;
            int        iFailChip   ;
            int        iLotCnt     ;
            int        iCntRslt0   ; // Empty
            int        iCntRslt1   ; // Empty
            int        iCntRslt2   ; // Chip
            int        iCntRslt3   ; // Dust
            int        iCntRslt4   ; // LFlow
            int        iCntRslt5   ; // Broken
            int        iCntRslt6   ; // Crack
            int        iCntRslt7   ; // Burr
            int        iCntRslt8   ; // Zener
            int        iCntRslt9   ; // Wire
            int        iCntRsltA   ; // Orient
            int        iCntRsltB   ; // Pmeasure
            int        iCntRsltC   ; // Pdist
            int        iCntRsltD   ; // Match
            int        iCntRsltE   ; // Flow
            int        iCntRsltF   ; // Flow
            int        iCntRsltG   ; // Flow
            int        iCntRsltH   ; // Flow
            int        iCntRsltI   ; // Flow
            int        iCntRsltJ   ; // Flow
            int        iCntRsltK   ; // Flow
            int        iCntRsltL   ; // Flow
        };

        //랏별
        struct SLotInfo {
            //Input Info.
            AnsiString sLotNo      ;
            int        iCrntLotNo  ;
            AnsiString sJobFile    ;
            AnsiString sOperator   ;
            int        iLotMgzCnt  ;

            //Time.
            double     dStartTime  ;
            double     dEndTime    ;
            double     dRunTime    ;
            double     dIdleTime   ;
            double     dJamTime    ;
            double     dTotalTime  ;

            int        iStrpUPEH   ;
            int        iChipUPEH   ;
            int        iStrpUPH    ;
            int        iChipUPH    ;

            int        iWorkStrp   ;
            int        iWorkChip   ;
            int        iFailChip   ;

            int        iCntRslt0   ; // Empty
            int        iCntRslt1   ; // Empty
            int        iCntRslt2   ; // Chip
            int        iCntRslt3   ; // Dust
            int        iCntRslt4   ; // LFlow
            int        iCntRslt5   ; // Broken
            int        iCntRslt6   ; // Crack
            int        iCntRslt7   ; // Burr
            int        iCntRslt8   ; // Zener
            int        iCntRslt9   ; // Wire
            int        iCntRsltA   ; // Orient
            int        iCntRsltB   ; // Pmeasure
            int        iCntRsltC   ; // Pdist
            int        iCntRsltD   ; // Match
            int        iCntRsltE   ; // Flow
            int        iCntRsltF   ; // Flow
            int        iCntRsltG   ; // Flow
            int        iCntRsltH   ; // Flow
            int        iCntRsltI   ; // Flow
            int        iCntRsltJ   ; // Flow
            int        iCntRsltK   ; // Flow
            int        iCntRsltL   ; // Flow
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

        void __fastcall DispLotInfo (TPanel * _pnLotNo    ,
                                     TPanel * _pnJobFile  ,
                                     TPanel * _pnOperator ,
                                     TPanel * _pnStartTime,
                                     TPanel * _pnEndTime  ,
                                     TPanel * _pnRunTime  ,
                                     TPanel * _pnIdleTime ,
                                     TPanel * _pnJamTime  ,
                                     TPanel * _pnTotalTime,
                                     TPanel * _pnStrpUPEH ,
                                     TPanel * _pnChipUPEH ,
                                     TPanel * _pnStrpUPH  ,
                                     TPanel * _pnChipUPH  ,
                                     TPanel * _pnWorkStrp ,
                                     TPanel * _pnWorkChip ,
                                     TPanel * _pnFailChip );


        void __fastcall DispDayInfo(TPanel * _pnRunTime   ,
                                    TPanel * _pnIdleTime  ,
                                    TPanel * _pnJamTime   ,
                                    TPanel * _pnTotalTime ,
                                    TPanel * _pnStrpUPEH  ,
                                    TPanel * _pnChipUPEH  ,
                                    TPanel * _pnStrpUPH   ,
                                    TPanel * _pnChipUPH   ,
                                    TPanel * _pnWorkStrp  ,
                                    TPanel * _pnWorkChip  ,
                                    TPanel * _pnFailChip  ,
                                    TPanel * _pnWorkLot   );

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
        void __fastcall AddDayInfoLotCnt  (int _iNo){DayInfo.iLotCnt   += _iNo ;}
        void __fastcall AddDayInfoWorkStrp(int _iNo){DayInfo.iWorkStrp += _iNo ;}
        void __fastcall AddDayInfoWorkChip(int _iNo){DayInfo.iWorkChip += _iNo ;}
        void __fastcall AddDayInfoFailChip(int _iNo){DayInfo.iFailChip += _iNo ;}
        void __fastcall AddDayInfoChips   (int _iCntRslt0 = 0,
                                           int _iCntRslt1 = 0,
                                           int _iCntRslt2 = 0,
                                           int _iCntRslt3 = 0,
                                           int _iCntRslt4 = 0,
                                           int _iCntRslt5 = 0,
                                           int _iCntRslt6 = 0,
                                           int _iCntRslt7 = 0,
                                           int _iCntRslt8 = 0,
                                           int _iCntRslt9 = 0,
                                           int _iCntRsltA = 0,
                                           int _iCntRsltB = 0,
                                           int _iCntRsltC = 0,
                                           int _iCntRsltD = 0,
                                           int _iCntRsltE = 0,
                                           int _iCntRsltF = 0,
                                           int _iCntRsltG = 0,
                                           int _iCntRsltH = 0,
                                           int _iCntRsltI = 0,
                                           int _iCntRsltJ = 0,
                                           int _iCntRsltK = 0,
                                           int _iCntRsltL = 0){
                                               DayInfo.iCntRslt0 +=  _iCntRslt0;
                                               DayInfo.iCntRslt1 +=  _iCntRslt1;
                                               DayInfo.iCntRslt2 +=  _iCntRslt2;
                                               DayInfo.iCntRslt3 +=  _iCntRslt3;
                                               DayInfo.iCntRslt4 +=  _iCntRslt4;
                                               DayInfo.iCntRslt5 +=  _iCntRslt5;
                                               DayInfo.iCntRslt6 +=  _iCntRslt6;
                                               DayInfo.iCntRslt7 +=  _iCntRslt7;
                                               DayInfo.iCntRslt8 +=  _iCntRslt8;
                                               DayInfo.iCntRslt9 +=  _iCntRslt9;
                                               DayInfo.iCntRsltA +=  _iCntRsltA;
                                               DayInfo.iCntRsltB +=  _iCntRsltB;
                                               DayInfo.iCntRsltC +=  _iCntRsltC;
                                               DayInfo.iCntRsltD +=  _iCntRsltD;
                                               DayInfo.iCntRsltE +=  _iCntRsltE;
                                               DayInfo.iCntRsltF +=  _iCntRsltF;
                                               DayInfo.iCntRsltG +=  _iCntRsltG;
                                               DayInfo.iCntRsltH +=  _iCntRsltH;
                                               DayInfo.iCntRsltI +=  _iCntRsltI;
                                               DayInfo.iCntRsltJ +=  _iCntRsltJ;
                                               DayInfo.iCntRsltK +=  _iCntRsltK;
                                               DayInfo.iCntRsltL +=  _iCntRsltL;
                                           }

        void __fastcall AddLotInfoWorkStrp(int _iNo){LotInfo.iWorkStrp += _iNo ;}
        void __fastcall AddLotInfoWorkChip(int _iNo){LotInfo.iWorkChip += _iNo ;}
        void __fastcall AddLotInfoFailChip(int _iNo){LotInfo.iFailChip += _iNo ;}
        void __fastcall AddLotInfoChips   (int _iCntRslt0 = 0,
                                           int _iCntRslt1 = 0,
                                           int _iCntRslt2 = 0,
                                           int _iCntRslt3 = 0,
                                           int _iCntRslt4 = 0,
                                           int _iCntRslt5 = 0,
                                           int _iCntRslt6 = 0,
                                           int _iCntRslt7 = 0,
                                           int _iCntRslt8 = 0,
                                           int _iCntRslt9 = 0,
                                           int _iCntRsltA = 0,
                                           int _iCntRsltB = 0,
                                           int _iCntRsltC = 0,
                                           int _iCntRsltD = 0,
                                           int _iCntRsltE = 0,
                                           int _iCntRsltF = 0,
                                           int _iCntRsltG = 0,
                                           int _iCntRsltH = 0,
                                           int _iCntRsltI = 0,
                                           int _iCntRsltJ = 0,
                                           int _iCntRsltK = 0,
                                           int _iCntRsltL = 0){
                                               LotInfo.iCntRslt0 +=  _iCntRslt0;
                                               LotInfo.iCntRslt1 +=  _iCntRslt1;
                                               LotInfo.iCntRslt2 +=  _iCntRslt2;
                                               LotInfo.iCntRslt3 +=  _iCntRslt3;
                                               LotInfo.iCntRslt4 +=  _iCntRslt4;
                                               LotInfo.iCntRslt5 +=  _iCntRslt5;
                                               LotInfo.iCntRslt6 +=  _iCntRslt6;
                                               LotInfo.iCntRslt7 +=  _iCntRslt7;
                                               LotInfo.iCntRslt8 +=  _iCntRslt8;
                                               LotInfo.iCntRslt9 +=  _iCntRslt9;
                                               LotInfo.iCntRsltA +=  _iCntRsltA;
                                               LotInfo.iCntRsltB +=  _iCntRsltB;
                                               LotInfo.iCntRsltC +=  _iCntRsltC;
                                               LotInfo.iCntRsltD +=  _iCntRsltD;
                                               LotInfo.iCntRsltE +=  _iCntRsltE;
                                               LotInfo.iCntRsltF +=  _iCntRsltF;
                                               LotInfo.iCntRsltG +=  _iCntRsltG;
                                               LotInfo.iCntRsltH +=  _iCntRsltH;
                                               LotInfo.iCntRsltI +=  _iCntRsltI;
                                               LotInfo.iCntRsltJ +=  _iCntRsltJ;
                                               LotInfo.iCntRsltK +=  _iCntRsltK;
                                               LotInfo.iCntRsltL +=  _iCntRsltL;
                                           }

};

extern CLot LT;

//---------------------------------------------------------------------------
#endif
