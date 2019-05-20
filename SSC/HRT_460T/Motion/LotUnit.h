/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.05.26                                                     */
/* Modify : 2004.10.15                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//---------------------------------------------------------------------------
#ifndef LotUnitH
#define LotUnitH

#include <ExtCtrls.hpp>
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include <Grids.hpp>

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
            int        iDropChip   ;
            int        iGoodChip   ;
            int        iLotCnt     ;
        };

        struct SLotInfo {
            //Input Info.
            AnsiString sLotNo      ;
            int        iCrntLotNo  ;
            int        iLastLotNo  ;
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
            int        iDropChip   ;
            int        iGoodChip   ;
        };

        SDayInfo  DayInfo    ;
        SLotInfo  LotInfo    ;
        SLotInfo  EndInfo    ;
        bool      m_bLotOpen ;
        bool      m_bLotEnd  ;

        __fastcall  CLot(void);
        __fastcall ~CLot(void);

        void Init();
        void Close();
        void __fastcall Reset(void);

        void __fastcall ClearLotInfo(void);
        void __fastcall ClearDayInfo(void);

        //Lot Processing.
        void __fastcall LotOpen  (AnsiString _sLotNo , AnsiString _sJobFile = ""  , AnsiString _sOperator ="");
        //void __fastcall LotOpen  (AnsiString _sLotNo );

        void __fastcall LotNext  (void);
        void __fastcall LotDelete(    );
        void __fastcall LotEnd   (void);

        AnsiString  __fastcall GetLotNo();

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
                                     TPanel * _pnFailChip ,
                                     TPanel * _pnDropChip ,
                                     TPanel * _pnGoodChip );


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
                                    TPanel * _pnDropChip  ,
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

        //적절히 잘 운용 한다. 김대기.
        void __fastcall AddDayInfoLotCnt  (int _iNo){DayInfo.iLotCnt   += _iNo ;}
        void __fastcall AddDayInfoWorkStrp(int _iNo){DayInfo.iWorkStrp += _iNo ;}
        void __fastcall AddDayInfoWorkChip(int _iNo){DayInfo.iWorkChip += _iNo ;}
        void __fastcall AddDayInfoFailChip(int _iNo){DayInfo.iFailChip += _iNo ;}
        void __fastcall AddDayInfoDropChip(int _iNo){DayInfo.iDropChip += _iNo ;}
        void __fastcall AddDayInfoGoodChip(int _iNo){DayInfo.iGoodChip += _iNo ;}
//        void __fastcall AddDayInfoChips   (int _iCntGood   ,
//                                           int _iCntFail   ){
//                                               DayInfo.iGood    +=  _iCntGood   ;
//                                               DayInfo.iFail    +=  _iCntFail   ;
//                                           }


        void __fastcall AddLotInfoWorkStrp (int _iNo){LotInfo.iWorkStrp += _iNo ;}
        void __fastcall AddLotInfoWorkChip (int _iNo){LotInfo.iWorkChip += _iNo ;}
        void __fastcall AddLotInfoFailChip (int _iNo){LotInfo.iFailChip += _iNo ;}
        void __fastcall AddLotInfoDropChip (int _iNo){LotInfo.iDropChip += _iNo ;}
        void __fastcall AddLotInfoGoodChip (int _iNo){LotInfo.iGoodChip += _iNo ;}

//        void __fastcall AddLotInfoChips   (int _iCntGood   ,
//                                           int _iCntFail    )
//                                           {
//                                               LotInfo.iGood    +=  _iCntGood   ;
//                                               LotInfo.iFail    +=  _iCntFail   ;
//                                           }


        //For SPC.
        void __fastcall UpdateDate   (TStringGrid * _sgDate);
        void __fastcall UpdateLotName(TStringGrid * _sgDate , TStringGrid * _sgLot );
        void __fastcall DispLotDate(TStringGrid * _sgDate , TStringGrid * _sgLot , TStringGrid * _sgLotInfo);

        void __fastcall DispLotDate  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall SaveLotData  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo);

        void __fastcall DispLotDayDate (TStringGrid * _sgLotName , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDayDate (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall SaveLotDayData (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);

        bool __fastcall WriteArayData(EN_ARAY_ID riId                   ); //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
        bool __fastcall ReadArayData (AnsiString sPath , EN_ARAY_ID riId); //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.

        bool __fastcall WriteTrimInfo(AnsiString _sLotNo, int _iTrimCnt, int _iBinNo);
        bool __fastcall ReadTrimInfo(TStringGrid * _sgDate, TStringGrid * _sgLot , int * _iTrimCnt , int * _iBinNo);

        bool __fastcall WriteBinInfo(AnsiString _sLotNo, int _iBinNo0,
                                                         int _iBinNo1,
                                                         int _iBinNo2,
                                                         int _iBinNo3,
                                                         int _iBinNo4,
                                                         int _iBinNo5,
                                                         int _iBinNo6,
                                                         int _iBinNo7,
                                                         int _iBinNo8,
                                                         int _iBinNo9);
        bool __fastcall ReadBinInfo(TStringGrid * _sgDate, TStringGrid * _sgLot , int *_iBinNo0,
                                                                                  int *_iBinNo1,
                                                                                  int *_iBinNo2,
                                                                                  int *_iBinNo3,
                                                                                  int *_iBinNo4,
                                                                                  int *_iBinNo5,
                                                                                  int *_iBinNo6,
                                                                                  int *_iBinNo7,
                                                                                  int *_iBinNo8,
                                                                                  int *_iBinNo9);
};

extern CLot LOT;

//---------------------------------------------------------------------------
#endif
