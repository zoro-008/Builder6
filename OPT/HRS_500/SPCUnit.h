//---------------------------------------------------------------------------
#ifndef SPCUnitH
#define SPCUnitH

#include <Grids.hpp>
#include <Series.hpp>

#include "SMInterfaceUnit.h"
#include "DataMan.h"
//#include "ProbeComUnit.h"
#define ERR_FOLDER "d:\\Spc\\ErrLog\\"
#define SPC_FOLDER "d:\\Spc\\"
//너도 Sub 로 가야되...
#define SPC_HEIGHT_FOLDER       "d:\\Spc\\RotorHeightData\\"
#define SPC_LEFT_STATOR_FOLDER  "d:\\Spc\\LeftStatorHeightData\\"
#define SPC_RIGHT_STATOR_FOLDER "d:\\Spc\\RightStatorHeightData\\"
//---------------------------------------------------------------------------
class CSpc
{
    private:
        //Err History
        TStringGrid * m_pErrHistory;
        //Err Count
        TStringGrid * m_pErrCnt    ;
        TStringGrid * m_pErrNoCnt  ;
        //Mtba
        TStringGrid * m_pLotFolder  ;
        TStringGrid * m_pLotFile    ;
    public :
        __fastcall CSpc::CSpc(void);
        __fastcall CSpc::~CSpc(void);

        int  iChipCnt   ;
        bool bUseZenner ;

        CArray SpcArray1 ;

        bool __fastcall WriteErrData(int iErrNo , bool bRelease = false);
        void __fastcall SaveLastErr(int iErrNo);
        int  __fastcall GetLastErr();

        void __fastcall DispErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgDate);
        void __fastcall DispErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt);
        void __fastcall DispMtba    (AnsiString sDataTime  , TBarSeries *_S1);
        void __fastcall SaveMtba    (AnsiString sDataTime  , TBarSeries *_S1);
        void __fastcall SaveErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt);
        void __fastcall SaveErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt);

        void __fastcall DelPastLog (AnsiString _sFolderPath);

        bool __fastcall WriteLotMap  (EN_ARAY_ID riId                          ) ;
        bool __fastcall ReadLotMap (AnsiString sPath , TStringGrid *_sgLotMap) ;

        int GetChipCnt   (){ return iChipCnt  ; } ;
        bool GetUseZenner(){ return bUseZenner; } ;

        bool __fastcall WriteArayData(EN_ARAY_ID riId ) ;//
        bool __fastcall ReadArayData (AnsiString sPath) ;// , EN_ARAY_ID riId) ;//

        void __fastcall DispLotDate    (TStringGrid * _sgDate    , TStringGrid * _sgLot  , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDate    (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall SaveLotData    (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDayDate (TStringGrid * _sgLotName , TStringGrid * _sgLotInfo);
        void __fastcall DispLotDayDate (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);
        void __fastcall SaveLotDayData (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo);

        bool __fastcall WriteArayData(CArray * riAray                   ); //
        bool __fastcall ReadArayData (AnsiString sPath , CArray * riAray); //

        void __fastcall UpdateDate     (TStringGrid * _sgDate);
        void __fastcall UpdateLotName  (TStringGrid * _sgDate    , TStringGrid * _sgLot );

        void __fastcall DelPastLotLog();

        //이런 종류는 SUb 로...
        void __fastcall WriteHeightData      (double _dData);
        void __fastcall WriteLeftStatorData  (double _dData);
        void __fastcall WriteRightStatorData (double _dData);

};

extern CSpc SPC;

#endif




