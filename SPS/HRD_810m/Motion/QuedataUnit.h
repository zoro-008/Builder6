//---------------------------------------------------------------------------
#ifndef QuedataUnitH
#define QuedataUnitH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include "UtilDefine.h"

#define MAX_QUE_DATA_CNT 100

//��Ŀ �� ��ġ ������ ������ ����.
//ť�� �ƴ� ���ص� �ƴ� ���� �Ǿ� ����.....
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
        bool GetDataF (int _iQueue); //Fail�̿�����, Error ������.
        int  GetDataCnt() ;
};

#endif
