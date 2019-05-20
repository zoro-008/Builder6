//---------------------------------------------------------------------------
#ifndef SPCUnitH
#define SPCUnitH

#include <Grids.hpp>
#include <Series.hpp>

#include "SMInterfaceUnit.h"
#include "Timer.h"
#include "GridData.h"
#include "Array.h"
//---------------------------------------------------------------------------
#define ERR_FOLDER "d:\\ErrLog\\"+OM.EqpOptn.sModelName+"\\"
#define LOT_FOLDER "d:\\LotLog\\"+OM.EqpOptn.sModelName+"\\"
#define DAY_FOLDER "d:\\DayLog\\"+OM.EqpOptn.sModelName+"\\"
#define WRK_FOLDER "d:\\WrkLog\\"+OM.EqpOptn.sModelName+"\\"





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
        bool LoadErrIni (TDateTime _tSttData , TDateTime _tEndData , TData * _tData); //_tData가 GetErrCnt로 받아온 숫자만큼 할당이 되어 있어야함.

        void SortErrData(bool _bNumberTime , int _iDataCnt , TData * _tData);

    public :
        bool SetErr(int _iErrNo , AnsiString _sErrName , AnsiString _sErrMsg , AnsiString _sLotId);
        bool ClrErr();

        void DispErrData (TDateTime _tSttData , TDateTime _tEndData , TStringGrid * _sgErrDate , bool _bNumberTime); //true면 에러넘버 , false면 시간.
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
            double     dUPEH      ;
            double     dUPH       ;
            double     dTickTime  ;
            AnsiString sJobFile   ;
            void Clear(){
                sLotId     = "";
                iWorkCnt   = 0 ;

                dSttTime   = 0.0 ;
                dEndTime   = 0.0 ;
                dWorkTime  = 0.0 ;
                dErrTime   = 0.0 ;
                dStopTime  = 0.0 ;
                dTotalTime = 0.0 ;
                dUPEH      = 0   ;
                dUPH       = 0   ;
                dTickTime  = 0.0 ;
                sJobFile   = "";
            }
        } ;

        //20150823 레이언스 FOS때문에 추가...
        //자제 하나하나의 리스트를 보여 줘야 한다.
        CGridData DispData ;



    public :
        CLotData();
        ~CLotData();
        void Init();
        void Close();

        void Update(EN_SEQ_STAT Stat);

        void WriteLotInfo();

    protected :
        TData  m_tData ;

        //TickTime.
        CCycleTimer m_tmTick    ;

        void LoadSaveLotIni (bool _bLoad); //장비 껐다 킬때쓰는 놈.
        bool SaveArrayData  (TData & _tData , int _iMgzNo , int _iSlotNo , CArray & _rData); //자료 기록용.

    public :
        TData GetData(){return m_tData;}
        void  SetData(TData Data) { m_tData = Data ;}

        AnsiString GetCrntLotNo();

        void ClearData();

        //20150823 레이언스 FOS때문에 추가...
        //자제 하나하나의 리스트를 보여 줘야 한다.
        void DispWorkList       (String _sFilePath , TStringGrid * _sgWorkList);
        void DispWorkDateList   (TStringGrid * _sgDateList);
        void SaveWorkListCSV    (String _sFilePath);
        void SaveWorkListXLS    (String _sFilePath);

        void DispDateList       (TStringGrid * _sgDateList);
        void DispLotList        (AnsiString _sDate , TStringGrid * _sgLotList);
        void DispMgzAndSlotList (AnsiString _sDate , AnsiString _sLotId , TStringGrid * _sgMgz , TStringGrid * _sgSlot );
        void DispStripMap       (AnsiString _sDate , AnsiString _sLotId , int _iMgz , int _iSlot , TStringGrid * _sgMap);
        void DispLotInfo        (TStringGrid * _sgDayInfo); //오퍼레이션 창용.

};

class CDayData
{
    public :
        struct TData {
            double     dWorkTime   ;
            double     dStopTime   ;
            double     dErrTime    ;
            double     dTotalTime  ;
            double     dUPEH       ;
            double     dUPH        ;
            int        iWorkCnt    ;
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
        void LoadSaveDayIni (bool _bLoad , TData & _tData );//장비 껐다 킬때쓰는 놈.

        //스트립 한장 단위로 저장을 한다.
        bool SaveDayData    (TData & _tData,TDateTime _tDateTime);

        void ClearData();

        void DispDayList (TStringGrid * _sgDateList);
        void DispDayInfo (TStringGrid * _sgDayInfo ); //오퍼레이션 창용.

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
