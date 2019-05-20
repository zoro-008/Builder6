//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int iSpinMtJogSpeed ;
    int iTiltMtStartPs  ;
    int iTiltMtStopPs   ;
    int dTiltMtSpeed    ;
    int iCleanRPM       ;
    int iAirRPM         ;
    int iDryRPM         ;
    int iCleanTime      ;
    int iAirTime        ;
    int iDryTime        ;
    int iWaterDrainTime ;

} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    int    iEmgLogic           ;

    int    iRatio1             ;
    int    iPlusLimitLogic1    ;
    int    iMinusLimitLogic1   ;
    int    iAlarmLogic1        ;
    int    iNearLogic1         ;
    int    iInPoLogic1         ;
    int    iSvReadyLogic1      ;
    int    iEncZLogic1         ;
    int    iEncDir1            ;
    int    iEncCount1          ;
    int    iCurrentOn1         ;
    int    iDCCOn1             ;
    int    iAlarmResetOn1      ;
    int    iPulseLogic1        ;
    bool   bUsedSpinLimit      ;
    double dSpinMLimit         ;
    double dSpinPLimit         ;

    int    iRatio2             ;
    int    iPlusLimitLogic2    ;
    int    iMinusLimitLogic2   ;
    int    iAlarmLogic2        ;
    int    iNearLogic2         ;
    int    iInPoLogic2         ;
    int    iSvReadyLogic2      ;
    int    iEncZLogic2         ;
    int    iEncDir2            ;
    int    iEncCount2          ;
    int    iCurrentOn2         ;
    int    iDCCOn2             ;
    int    iAlarmResetOn2      ;
    int    iPulseLogic2        ;
    bool   bUsedTiltLimit      ;
    double dTiltMLimit         ;
    double dTiltPLimit         ;



} ;

//Master Option.
//struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
//} ;

//Eqipment Option.
//struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
//    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
//    int        iTotalWork      ;
//} ;


//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    public:
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        //CEqpOptn   EqpOptn  ;

        AnsiString  m_sCrntDev ; //Current open device.

        AnsiString GetCrntDev () { return m_sCrntDev ; }

        void SetCrntDev (AnsiString _sName);
        void Init(void);
        void Close(void);
        void LoadJobFile  (AnsiString _sDevName); void SaveJobFile  (AnsiString _sDevName);

        void LoadDevInfo  (AnsiString _sDevName); void SaveDevInfo  (AnsiString _sDevName);
        void LoadDevOptn  (AnsiString _sDevName); void SaveDevOptn  (AnsiString _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );
        void LoadLastInfo (                    ); void SaveLastInfo (                    );

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
