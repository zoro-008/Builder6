//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    double dColPitch  ;
    double dRowPitch  ;
    int    iColGrCnt  ;
    int    iRowGrCnt  ;
    double dColGrGap  ;
    double dRowGrGap  ;
    int    iColCnt    ;
    int    iRowCnt    ;
    int    iCsSlCnt   ;
    double dCsSlPitch ;

    int    iColInspCnt;
    int    iRowInspCnt;

} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    //ProbOptn
    int    iPinCnt             ;
    int    iLimInsFailCnt      ;
    int    iReInspCnt          ; //���κ� �˻� ��� �Ұ���...
    int    iLimAllFailCnt      ; //�ѽ�Ʈ�� ���� ī��Ʈ ����.
    int    iContColCnt         ;
    int    iPR1MoveCnt         ;
    double dPR1OutOfs          ;
    int    iPR2MoveCnt         ;
    double dPR2OutOfs          ;
    int    iWRKMoveCnt         ;

    bool   bUseZener           ;

    int    i1Ch1P              ;
    int    i1Ch1N              ;
    int    i1Ch2P              ;
    int    i1Ch2N              ;
    int    i1Ch3P              ;
    int    i1Ch3N              ;
    int    i1ChLedCur          ;
    int    i1ChZenCur          ;
    int    i2Ch1P              ;
    int    i2Ch1N              ;
    int    i2Ch2P              ;
    int    i2Ch2N              ;
    int    i2Ch3P              ;
    int    i2Ch3N              ;
    int    i2ChLedCur          ;
    int    i2ChZenCur          ;

    int    i1ChDelay           ;
    int    i1ChLedTime         ;
    int    i1ChZenTime         ;
    int    i2ChDelay           ;
    int    i2ChLedTime         ;
    int    i2ChZenTime         ;



    //Daegyoem Probe
    bool   bChRvsCrnt          ;
    int    iSwitchingDly       ;
    int    iDgChipCnt          ;
    bool   bSideLEDControl     ;

    double d1ChLow             ;
    double d1ChHigh            ;
    double d2ChLow             ;
    double d2ChHigh            ;
    double dZenerLow           ;
    double dZenerHigh          ;

    int    iPrbSttDelay        ; //���κ� �˻� ������.

    bool   bUseTwiceInsp       ;

    double dPreRailOutVel      ;
    double dPostRailOutVel     ;

    //Dev Option
    int    iLotEndMgzCnt       ;
    int    iWaitDelay          ;

    bool   bLtHeaterNum2       ;
    bool   bLtHeaterNum3       ;
    bool   bRtHeaterNum2       ;
    bool   bRtHeaterNum3       ;

    double dPSBReGripOfs       ; //����Ʈ ���� ���׸� �ÿ� �̵���.
    int    iPSBReGripCol       ; //����Ʈ ���� ���׸� ���� �÷� ����.

    double dREJPitch           ;

    double dTrimPtchOfs        ;
    double dWorkPtchOfs        ; //
    double dWRKPtchOfs         ;
    double dPSBPtchOfs         ;
    //double dTrimMoveOfs        ;

    int    iTrimRjtDly         ;
    int    iTrimSnrDly         ;

    int    iContactPinCnt      ;
    int    iLimContactPinCnt   ;
    double dPSBPshDeley        ;

    double dGripOfs            ;


} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bDryRun           ;
    bool   bIgnrDoor         ;
    bool   bIgnrIdCehck      ;
    bool   bIgnrInsp         ;
    bool   bIgnrHeatAlm      ;
    bool   bIgnrStrOverAlm   ;
    bool   bLoadingStop      ;
    bool   bAutoOperation    ; //�յ���� ���� Stop Start Reset.
    bool   bAutoConvesion    ; //�ڿ� ��� �ڵ� ������ ü����.
    bool   bCheckFailCnt     ;
    int    iCheckFailCnt     ;
    bool   bIgnrTrim         ;
    bool   bIgnrVacuum       ;
    bool   bSortSensorDelay  ;
    int    iSortSensorDelay  ;
    bool   bIgnrDecChk       ;
    bool   bIgnrTrimDecChk   ;

    int    iTemperature1     ;
    int    iTemperature2     ;
    int    iTemperature3     ;
    int    iTemperature4     ;
    int    iTemperature5     ;
    int    iTemperature6     ;
    int    iTemperature7     ;
    int    iTemperature8     ;
    bool   bIgnrDrop         ;

    bool   bUsedInsOptn      ;
    bool   bUsedTopViewPnch  ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    AnsiString sPstEquipPath      ;
//    AnsiString sPstEquipDrv       ;
//    AnsiString sPstEquipIp        ;
    AnsiString sPreEquipPath      ;
    bool       bHeattingTest      ; //���ڸ����� ���� ���.
    bool       bNoLdUl            ; //�δ� ��δ� ���� �׽�Ʈ �ϱ�.
    bool       bGripTest          ; //����ö ����� ��û����. ���� �Ŀ� �׸� Ǯ��.
    bool       bPRBCmp            ; //�������� Ŭ���� �巹�� ���� ���� �Ҷ� �׸��� �ѹ� Ǯ���ٰ� ����. ���ϻ��� �������� �������� �����ε� ������.
    bool       bBarcodeUseOnly    ;
    bool       bByChInsp          ;
    bool       bLdr_YHome         ;
    bool       bUseDaegyeomProbe  ;
    bool       bNotUstLot         ;
    bool       bShortMonitor      ; // 0��Ʈ�밡 ������ ���� ���.

    double     d1chLedOffset      ;
    double     d1chLedGain        ;
    double     d1chLedSetOfs      ;
    double     d1chZennerOffset   ;
    double     d1chZennerGain     ;
    double     d1chZennerSetOfs   ;

    double     d2chLedOffset      ;
    double     d2chLedGain        ;
    double     d2chLedSetOfs      ;
    double     d2chZennerOffset   ;
    double     d2chZennerGain     ;
    double     d2chZennerSetOfs   ;

    int        iBarcodeNum        ;

    bool       bDataAccEng        ; //Engineer ���� Aray ������ ���� ����.

    double     dWRKCmpOfs         ;
    bool       bUseReContact      ;

    int        iTempOfs           ;
    int        iMaxTemp           ;

    bool       bUsedVision        ; // 1ȣ��� ������ �����ϱ� �ϳ� �߰� �� =...JH
    bool       bUsedTempRS232     ;

    bool       bUsedEngVersion    ;

    bool       bUsed3040Pnch      ;


} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
//  int        iModelName      ; //�𵨺� ����Ʈ ���� �ɼ��� ���� �̿�.  �ϵ���� �ٸ������� �̿��ϸ� �ȵ�.   TModelNo
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
    bool       bExistLoader   ;
    bool       bExistUnLoader ;
} ;


//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    public:
        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;

        AnsiString  m_sCrntDev ; //Current open device.

        AnsiString GetCrntDev () { return m_sCrntDev ; }

        void Close(void);
        void SetCrntDev (AnsiString _sName);
        void Init(void);
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
