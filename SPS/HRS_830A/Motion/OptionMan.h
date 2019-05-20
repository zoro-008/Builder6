//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    //Strip
    double dColPitch       ;
    int    iColGrCnt       ;
    double dColGrGap       ;
    int    iColCnt         ;

    double dRowPitch       ;
    int    iRowGrCnt       ;
    double dRowGrGap       ;
    int    iRowCnt         ;

    //Mgz
    int    iCsSlCnt        ;
    double dCsSlPitch      ;

} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int    iLotEndMgzCnt     ; //작업 Lot 갯수
    int    iPSBOutPshrDelay  ; //포스트버퍼 Out Pusher 딜레이
    int    iPSBUpDnPshrDelay ; //포스트버퍼 Up Down Pusher 딜레이
    int    iDispsrLTimer     ; //워크존 Dispenser Left 타이머
    int    iDispsrRTimer     ; //워크존 Dispenser Right 타이머
    int    iDispsrCnt        ; //디스펜서 쏴주는 카운트
    int    iDispsrSnsrTimer  ; //디스펜서 센서 타임.
} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool      bLoadingStop   ;
    bool      bIgnrDoor      ;
    bool      bIgnrDisps     ;
    bool      bEmptyStrpCnt  ;
    bool      bIgnrDsprSnsr  ;
    bool      bIgnrDsprExpr  ;
    int       iEmptyStrpCnt  ;
    int       iExpiryDay     ;
    TDateTime LQSupplyDay    ;
    int       iDisprCleanCnt ;
    int       iStopToShotTime;

    String    sServerPath    ;

} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool   bDryRun            ;
    bool   bUsedSingleDisps   ;
    bool   bUsedDispsOptn     ;
    int    iDispsOptn         ;
    bool   bSkipOvldSnsr      ;
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    String sModelName     ; //sModelName 모델 이름 디스플레이용 이다.

} ;

//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    protected:
        String  m_sCrntDev ; //Current open device.


    public:
        void Init();
        void Close();

        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;



        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (String _sName);

        void LoadJobFile  (String _sDevName); void SaveJobFile  (String _sDevName);

        void LoadDevInfo  (String _sDevName); void SaveDevInfo  (String _sDevName);
        void LoadDevOptn  (String _sDevName); void SaveDevOptn  (String _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );
        void LoadLastInfo (                    ); void SaveLastInfo (                    );

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
