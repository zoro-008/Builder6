//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int iSpinMtJogSpeed ;
    int iTiltMtStartPs  ;
    int iTiltMtStopPs   ;
    int iTiltMtSpeed    ;
    int iCleanRPM       ;
    int iAirRPM         ;
    int iDryRPM         ;
    int iCleanTime      ;
    int iAirTime        ;
    int iDryTime        ;
    int iWaterDrainTime ;

    //20180829 ���� ��/�Ʒ� ���� �ɼ�
    bool bUseTopNozzle ;
    bool bUseBtmNozzle ;
    bool bUseTwinNozzle;

} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    int iEmgLogic           ;

    int iRatio1             ;
    int iPlusLimitLogic1    ;
    int iMinusLimitLogic1   ;
    int iAlarmLogic1        ;
    int iNearLogic1         ;
    int iInPoLogic1         ;
    int iSvReadyLogic1      ;
    int iEncZLogic1         ;
    int iEncDir1            ;
    int iEncCount1          ;
    int iCurrentOn1         ;
    int iDCCOn1             ;
    int iAlarmResetOn1      ;
    int iPulseLogic1        ;

    int iRatio2             ;
    int iPlusLimitLogic2    ;
    int iMinusLimitLogic2   ;
    int iAlarmLogic2        ;
    int iNearLogic2         ;
    int iInPoLogic2         ;
    int iSvReadyLogic2      ;
    int iEncZLogic2         ;
    int iEncDir2            ;
    int iEncCount2          ;
    int iCurrentOn2         ;
    int iDCCOn2             ;
    int iAlarmResetOn2      ;
    int iPulseLogic2        ;

    int iSpinMLimit         ;
    int iSpinPLimit         ;
    int iTiltMLimit         ;
    int iTiltPLimit         ;

    bool bUsedSpinLimit     ;
    bool bUsedTiltLimit     ;

    //bool bUsedWaterPump   ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool bUsedFrstMachine   ;
    bool bUsedDryRun        ;
    bool bUsedWater         ;
    bool bIngrCvrSnsr       ;
} ;

//Eqipment Option.
//struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
//    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
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
        CMstOptn   MstOptn  ; 
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