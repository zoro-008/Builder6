//---------------------------------------------------------------------------
#ifndef SPCUnitH
#define SPCUnitH

#include <Grids.hpp>
#include <Series.hpp>

#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
class CSPC
{
    private:
        //Err History
        TStringGrid * m_pErrHistory;
        //Err Count
        TStringGrid * m_pErrCnt    ;
        TStringGrid * m_pErrNoCnt  ;
        //Mtba
//        TStringGrid * m_pMtba      ;
    public :
        __fastcall CSPC::CSPC(void);
        __fastcall CSPC::~CSPC(void);

        bool __fastcall WriteErrData(int iErrNo , bool bRelease = false);
        void __fastcall SaveLastErr(int iErrNo);
        int  __fastcall GetLastErr();

        void __fastcall DispErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgDate);
        void __fastcall DispErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt);
        void __fastcall DispMtba    (AnsiString sDataTime  , TBarSeries *_S1);
        void __fastcall SaveMtba    (AnsiString sDataTime  , TBarSeries *_S1);
        void __fastcall SaveErrCnt  (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt);
        void __fastcall SaveErrData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgErrCnt);

        void __fastcall SaveOptionLog ( int iIndex , bool bInverse = true) ;//¹Ì¾áÇÏ´Ù ¿©µû ÃÄ¹Ú¾Æ¾ß °Ù¾û.
        void __fastcall SaveClearLog  (AnsiString _sJobFile , AnsiString _sLotNo ,AnsiString _sSlotNo , AnsiString _sMsg);

        void __fastcall DelPastLog (AnsiString _sFolderPath);

};

extern CSPC SPC;

#endif
