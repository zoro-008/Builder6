//---------------------------------------------------------------------------
#ifndef QuedataUnitH
#define QuedataUnitH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include "UtilDefine.h"

#define MAX_QUE_DATA_CNT 100

//픽커 픽 위치 데이터 용으로 만듬.
//큐도 아닌 스텍도 아닌 것이 되어 버림.....
class CQueData
{
    public :
        __fastcall CQueData::CQueData(void);
        __fastcall CQueData::~CQueData(void);

        struct TQueData {
            int  iRow  ;
            int  iCol  ;
            bool bFail ;
        };

    protected :
        TQueData Data[MAX_QUE_DATA_CNT];

        int  m_iDataCnt ;

    public :
        void Load     (bool _IsLoad);

        void ClearData(int _iClrCnt = MAX_QUE_DATA_CNT);


        void PushData (int _iRow  , int _iCol , bool _bFail);
        int  GetDataR (int _iQueue);
        int  GetDataC (int _iQueue);
        bool GetDataF (int _iQueue); //Fail이였는지, Error 였는지.
        int  GetDataCnt() ;
};

#endif
