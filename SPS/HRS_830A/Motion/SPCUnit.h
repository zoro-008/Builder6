//---------------------------------------------------------------------------
#ifndef SPCUnitH
#define SPCUnitH

#include <Grids.hpp>
#include <Series.hpp>

#include "SMInterfaceUnit.h"
#include "Array.h"
//---------------------------------------------------------------------------
//
class CErrData
{
    public :
        struct TData {
            int        iErrNo   ;
            AnsiString sErrName ;
            double     dSttTime ;
            AnsiString sErrMsg  ;
            AnsiString sLotId   ;
        } ;

    public :
        CErrData();
        ~CErrData();
        void Init();
        void Close();

        void Update(AnsiString _sCrntLotNo);

    protected :
        int        m_iLastErr    ;
        AnsiString m_sLastErrMsg ;

        TData      m_tErrData ;

        void LoadSaveLastErr(bool _bLoad);

        bool SaveErrIni (TData &_tData);
        int  GetErrCnt  (TDateTime _tSttData , TDateTime _tEndData);
        bool LoadErrIni (TDateTime _tSttData , TDateTime _tEndData , TData * _tData); //_tData�� GetErrCnt�� �޾ƿ� ���ڸ�ŭ �Ҵ��� �Ǿ� �־����.

        void SortErrData(bool _bNumberTime , int _iDataCnt , TData * _tData);

    public :
        bool SetErr(int _iErrNo , AnsiString _sErrName , AnsiString _sErrMsg , AnsiString _sLotId);
        bool ClrErr();

        void DispErrData (TDateTime _tSttData , TDateTime _tEndData , TStringGrid * _sgErrDate , bool _bNumberTime); //true�� �����ѹ� , false�� �ð�.
};

//
class CLotData
{
    public :
        struct TData {
            AnsiString sLotId     ;
            int        iWorkCnt   ;
            double     dSttTime   ;
            double     dEndTime   ;
            double     dWorkTime  ;
            double     dErrTime   ;
            double     dStopTime  ;
            double     dTotalTime ;
            double     dTickTime  ;
            AnsiString sJobFile   ;
        } ;

    public :
        CLotData();
        ~CLotData();
        void Init();
        void Close();

        void Update(EN_SEQ_STAT Stat);

    protected :
        TData  m_tData ;

        bool LoadSaveLotIni (bool _bLoad); //��� ���� ų������ ��.
        bool SaveArrayData  (TData & _tData , int _iMgzNo , int _iSlotNo , CArray & _rData); //�ڷ� ��Ͽ�.

    public :
        TData GetData(){return m_tData;}

        AnsiString GetCrntLotNo();

        void ClearData();

        void DispDateList       (TStringGrid * _sgDateList);
        void DispLotList        (AnsiString _sDate , TStringGrid * _sgLotList);
        void DispMgzAndSlotList (AnsiString _sDate , AnsiString _sLotId , TStringGrid * _sgMgz , TStringGrid * _sgSlot );
        void DispStripMap       (AnsiString _sDate , AnsiString _sLotId , int _iMgz , int _iSlot , TStringGrid * _sgMap);
        void DispLotInfo        (TStringGrid * _sgDayInfo); //���۷��̼� â��.

};










class CDayData
{
    public :
        struct TData {
            double     dWorkTime   ;
            double     dStopTime   ;
            double     dErrTime    ;
            double     dTotalTime  ;
            int        iUPEH       ;
            int        iUPH        ;
            int        iWorkCnt    ;
            int        iLotCnt     ;
        };

    public :
        CDayData();
        ~CDayData();
        void Init();
        void Close();

        void Update(AnsiString _sCrntLotNo, EN_SEQ_STAT Stat);

    protected :
        TData  m_tData ;

    public :
        bool LoadSaveDayIni (bool _bLoad , TData & _tData );//��� ���� ų������ ��.

        //��Ʈ�� ���� ������ ������ �Ѵ�.
        bool SaveDayData    (TData & _tData,TDateTime _tDateTime);

        void ClearData();

        void DispDayList (TStringGrid * _sgDateList);
        void DispDayInfo (TStringGrid * _sgDayInfo ); //���۷��̼� â��.

};




class CSPC
{


    public :
        __fastcall CSPC::CSPC(void);
        __fastcall CSPC::~CSPC(void);


    public :
        CErrData ERR ;
        CLotData LOT ;
        CDayData DAY ;

        void Init();
        void Close();

        void Update(EN_SEQ_STAT Stat);




};

extern CSPC SPC;

#endif