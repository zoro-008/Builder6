
//---------------------------------------------------------------------------
#ifndef LotUnitH
#define LotUnitH

#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#define MAX_LOT_CNT 50
#define LOG_FOLDER "d:\\LotLog\\"

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
        TStringGrid * m_pLotFolder  ;
        TStringGrid * m_pLotFile    ;
    public :
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
            int        iCntRslt1   ; // Empty
            int        iCntRslt2   ; // Chip
            int        iCntRslt3   ; // Dust
            int        iCntRslt4   ; // LFlow
            int        iCntRslt5   ; // Broken
            int        iCntRslt6   ; // Crack
            int        iCntRslt7   ; // Burr
            int        iCntRslt8   ; // Zener
            int        iCntRslt9   ; // Wire
            int        iCntRslt10  ; // Orient
            int        iCntRslt11  ; // Pmeasure
            int        iCntRslt12  ; // Pdist
            int        iCntRslt13  ; // Match
            int        iCntRslt14  ; // Flow
            int        iCntHeatFail; // Heat Fail
        };

        struct SLotInfo {
//            SLotInfo();
            ~SLotInfo(){
                static int i = 0 ;
                AnsiString sTemp = i ;
                Trace("~SLotInfo",sTemp.c_str());
            };

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

            int        iCntRslt1   ; // Empty       
            int        iCntRslt2   ; // Chip
            int        iCntRslt3   ; // Dust
            int        iCntRslt4   ; // LFlow
            int        iCntRslt5   ; // Broken
            int        iCntRslt6   ; // Crack
            int        iCntRslt7   ; // Burr
            int        iCntRslt8   ; // Zener
            int        iCntRslt9   ; // Wire
            int        iCntRslt10  ; // Orient
            int        iCntRslt11  ; // Pmeasure
            int        iCntRslt12  ; // Pdist
            int        iCntRslt13  ; // Match
            int        iCntRslt14  ; // Flow
            int        iCntHeatFail; // Heat Fail
        };

        SDayInfo  DayInfo    ;
        SLotInfo  LotInfo    ;
        bool      m_bLotOpen ;
        bool      m_bLotEnd  ;

        __fastcall  CLot(void);
        __fastcall ~CLot(void);

        void __fastcall ClearLotInfo(void);
        void __fastcall ClearDayInfo(void);

        //Lot Processing.
        void __fastcall LotOpen  (AnsiString  _sLotNo , AnsiString _sJobFile = "", AnsiString _sOperator = "");
        void __fastcall LotEnd   (void          );

        AnsiString __fastcall GetCrntLot (void);

        void __fastcall pnLotWorkChip (TPanel * _pnLotNo    ,
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
        void __fastcall LoadLotInfo() ;
        void __fastcall SaveLotInfo() ;

        void __fastcall LoadDayInfo() ;
        void __fastcall SaveDayInfo() ;

        void __fastcall WriteLotLog   () ;
        void __fastcall WriteLotDayLog() ;

        void __fastcall DelPastLotLog () ;

        //Update Lot Efficiency.
        void __fastcall Update(EN_SEQ_STAT Stat);


        //적절히 운용 한다. 김대기.
        void __fastcall AddDayInfoLotCnt  (int _iNo){DayInfo.iLotCnt   += _iNo ;}
        void __fastcall AddDayInfoWorkStrp(int _iNo){DayInfo.iWorkStrp += _iNo ;}
        void __fastcall AddDayInfoWorkChip(int _iNo){DayInfo.iWorkChip += _iNo ;}
        void __fastcall AddDayInfoFailChip(int _iNo){DayInfo.iFailChip += _iNo ;}
        void __fastcall AddDayInfoChips   (int _iCntRslt1   ,
                                           int _iCntRslt2   ,
                                           int _iCntRslt3   ,
                                           int _iCntRslt4   ,
                                           int _iCntRslt5   ,
                                           int _iCntRslt6   ,
                                           int _iCntRslt7   ,
                                           int _iCntRslt8   ,
                                           int _iCntRslt9   ,
                                           int _iCntRslt10  ,
                                           int _iCntRslt11  ,
                                           int _iCntRslt12  ,
                                           int _iCntRslt13  ,
                                           int _iCntRslt14  ,
                                           int _iCntHeatFail){
                                               DayInfo.iCntRslt1    +=  _iCntRslt1   ;
                                               DayInfo.iCntRslt2    +=  _iCntRslt2   ;
                                               DayInfo.iCntRslt3    +=  _iCntRslt3   ;
                                               DayInfo.iCntRslt4    +=  _iCntRslt4   ;
                                               DayInfo.iCntRslt5    +=  _iCntRslt5   ;
                                               DayInfo.iCntRslt6    +=  _iCntRslt6   ;
                                               DayInfo.iCntRslt7    +=  _iCntRslt7   ;
                                               DayInfo.iCntRslt8    +=  _iCntRslt8   ;
                                               DayInfo.iCntRslt9    +=  _iCntRslt9   ;
                                               DayInfo.iCntRslt10   +=  _iCntRslt10  ;
                                               DayInfo.iCntRslt11   +=  _iCntRslt11  ;
                                               DayInfo.iCntRslt12   +=  _iCntRslt12  ;
                                               DayInfo.iCntRslt13   +=  _iCntRslt13  ;
                                               DayInfo.iCntRslt14   +=  _iCntRslt14  ;
                                               DayInfo.iCntHeatFail +=  _iCntHeatFail;
                                           }

        void __fastcall AddLotInfoWorkStrp(int _iNo){LotInfo.iWorkStrp += _iNo ;}
        void __fastcall AddLotInfoWorkChip(int _iNo){LotInfo.iWorkChip += _iNo ;}
        void __fastcall AddLotInfoFailChip(int _iNo){LotInfo.iFailChip += _iNo ;}
        void __fastcall AddLotInfoChips   (int _iCntRslt1   ,
                                           int _iCntRslt2   ,
                                           int _iCntRslt3   ,
                                           int _iCntRslt4   ,
                                           int _iCntRslt5   ,
                                           int _iCntRslt6   ,
                                           int _iCntRslt7   ,
                                           int _iCntRslt8   ,
                                           int _iCntRslt9   ,
                                           int _iCntRslt10  ,
                                           int _iCntRslt11  ,
                                           int _iCntRslt12  ,
                                           int _iCntRslt13  ,
                                           int _iCntRslt14  ,
                                           int _iCntHeatFail){
                                               LotInfo.iCntRslt1    +=  _iCntRslt1   ;
                                               LotInfo.iCntRslt2    +=  _iCntRslt2   ;
                                               LotInfo.iCntRslt3    +=  _iCntRslt3   ;
                                               LotInfo.iCntRslt4    +=  _iCntRslt4   ;
                                               LotInfo.iCntRslt5    +=  _iCntRslt5   ;
                                               LotInfo.iCntRslt6    +=  _iCntRslt6   ;
                                               LotInfo.iCntRslt7    +=  _iCntRslt7   ;
                                               LotInfo.iCntRslt8    +=  _iCntRslt8   ;
                                               LotInfo.iCntRslt9    +=  _iCntRslt9   ;
                                               LotInfo.iCntRslt10   +=  _iCntRslt10  ;
                                               LotInfo.iCntRslt11   +=  _iCntRslt11  ;
                                               LotInfo.iCntRslt12   +=  _iCntRslt12  ;
                                               LotInfo.iCntRslt13   +=  _iCntRslt13  ;
                                               LotInfo.iCntRslt14   +=  _iCntRslt14  ;
                                               LotInfo.iCntHeatFail +=  _iCntHeatFail;
                                           }

        void __fastcall UpdateDate     (TStringGrid * _sgDate);
        void __fastcall UpdateLotName  (TStringGrid * _sgDate    , TStringGrid * _sgLot );
        void __fastcall DispLotDate    (TStringGrid * _sgDate    , TStringGrid * _sgLot  , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDate    (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall SaveLotData    (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDayDate (TStringGrid * _sgLotName , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDayDate (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall SaveLotDayData (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);

//        bool __fastcall WriteArayData(EN_ARAY_ID riId                   ); //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
//        bool __fastcall ReadArayData (AnsiString sPath , EN_ARAY_ID riId); //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.

};

extern CLot LT;

//---------------------------------------------------------------------------
#endif
