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
            int        iFailCnt   ;
            int        iRslt1Cnt  ;
            int        iRslt2Cnt  ;
            int        iRslt3Cnt  ;
            int        iRslt4Cnt  ;
            int        iRslt5Cnt  ;
            int        iRslt6Cnt  ;
            int        iRslt7Cnt  ;
            int        iRslt8Cnt  ;
            int        iPck1VHCnt ; //20150330 ���������� ��û VH�ҷ� �Ķ󺰷� ī�����Ͽ� �α׿� ����.
            int        iPck2VHCnt ;
            int        iPck3VHCnt ;
            int        iPck4VHCnt ;

            double     dSttTime   ;
            double     dEndTime   ;
            double     dWorkTime  ;
            double     dErrTime   ;
            double     dStopTime  ;
            double     dTotalTime ;
            int        iUPEH      ;
            double     dTickTime  ;
            AnsiString sJobFile   ;
            void Clear(){
                sLotId     = "";
                iWorkCnt   = 0 ;
                iFailCnt   = 0 ;
                iRslt1Cnt  = 0 ;
                iRslt2Cnt  = 0 ;
                iRslt3Cnt  = 0 ;
                iRslt4Cnt  = 0 ;
                iRslt5Cnt  = 0 ;
                iRslt6Cnt  = 0 ;
                iRslt7Cnt  = 0 ;
                iRslt8Cnt  = 0 ;
                iPck1VHCnt = 0 ;
                iPck2VHCnt = 0 ;
                iPck3VHCnt = 0 ;
                iPck4VHCnt = 0 ;

                dSttTime   = 0.0 ;
                dEndTime   = 0.0 ;
                dWorkTime  = 0.0 ;
                dErrTime   = 0.0 ;
                dStopTime  = 0.0 ;
                dTotalTime = 0.0 ;
                iUPEH      = 0   ;
                dTickTime  = 0.0 ;
                sJobFile   = "";
            }
        } ;

    public :
        CLotData();
        ~CLotData();
        void Init();
        void Close();

        void Update(EN_SEQ_STAT Stat);

        void WriteLotInfo();

    protected :
        TData  m_tData ;

        bool LoadSaveLotIni (bool _bLoad); //��� ���� ų������ ��.
        bool SaveArrayData  (TData & _tData , int _iMgzNo , int _iSlotNo , CArray & _rData); //�ڷ� ��Ͽ�.
        bool SaveData       (TData & _tData);

    public :
        TData GetData(){return m_tData;}
        void  SetData(TData Data) { m_tData = Data ;}

        AnsiString GetCrntLotNo();

        void ClearData();

        void AddWorkCnt();

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
            int        iFailCnt    ;
            int        iLotCnt     ;
        };

    public :
        CDayData();
        ~CDayData();
        void Init();
        void Close();

        void Update(AnsiString _sCrntLotNo, EN_SEQ_STAT Stat);

        void WriteDayInfo();

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
