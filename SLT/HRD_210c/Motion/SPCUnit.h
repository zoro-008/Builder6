//---------------------------------------------------------------------------
#ifndef SPCUnitH
#define SPCUnitH

#include <Grids.hpp>
#include <Series.hpp>

//#include "SMInterfaceUnit.h"
#include "DataMan.h"
//#include "ProbeComUnit.h"
#define ERR_FOLDER "d:\\Spc\\ErrLog\\"
#define SPC_FOLDER "d:\\Spc\\"

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

        void Init();

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

};

extern CSpc SPC;

#endif




