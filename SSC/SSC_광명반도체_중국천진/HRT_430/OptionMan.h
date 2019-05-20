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
    int    iPinCnt          ;
    int    iLimInsFailCnt   ;
    int    iReInspCnt       ; //���κ� �˻� ��� �Ұ���...
    int    iLimAllFailCnt   ; //�ѽ�Ʈ�� ���� ī��Ʈ ����.
    int    iContColCnt      ;
    int    iReGrabCnt1      ;
    double dReGrabPs1       ;
    int    iReGrabCnt2      ; //��� ���� -1 �� �ؾ� �볶....
    double dReGrabPs2       ;

    bool   bUseZener        ;

    int    i1Ch1P           ;
    int    i1Ch1N           ;
    int    i1Ch2P           ;
    int    i1Ch2N           ;
    int    i1Ch3P           ;
    int    i1Ch3N           ;
    int    i1ChLedCur       ;
    int    i1ChZenCur       ;
    int    i2Ch1P           ;
    int    i2Ch1N           ;
    int    i2Ch2P           ;
    int    i2Ch2N           ;
    int    i2Ch3P           ;
    int    i2Ch3N           ;
    int    i2ChLedCur       ;
    int    i2ChZenCur       ;

    int    i1ChDelay        ;
    int    i1ChLedTime      ;
    int    i1ChZenTime      ;
    int    i2ChDelay        ;
    int    i2ChLedTime      ;
    int    i2ChZenTime      ;

    double d1ChLow          ;
    double d1ChHigh         ;
    double d2ChLow          ;
    double d2ChHigh         ;
    double dZenerLow        ;
    double dZenerHigh       ;

    int    iPrbSttDelay     ; //���κ� �˻� ������.

    bool   bUseTwiceInsp    ;

    double dPreRailOutVel   ;
    double dPostRailOutVel  ;

    //Dev Option
    int    iLotEndMgzCnt    ;
    int    iWaitDelay       ;

    //Pin Contacts Cnt
    int    iPinCtCnt        ;
    int    iLimPinCtCnt     ;

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

} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode       ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    AnsiString sPstEquipPath    ;
//    AnsiString sPstEquipDrv     ;
//    AnsiString sPstEquipIp      ;
    AnsiString sPreEquipPath    ;
    bool       bHeattingTest    ; //���ڸ����� ���� ���.
    bool       bNoLdUl          ; //�δ� ��δ� ���� �׽�Ʈ �ϱ�.
    bool       bGripTest        ; //����ö ����� ��û����. ���� �Ŀ� �׸� Ǯ��.
    bool       bBarcodeUseOnly  ;
    bool       bByChInsp        ;
    bool       bLdr_YHome       ;
//    bool       bUseHangul       ;

    double     d1chLedOffset    ;
    double     d1chLedGain      ;
    double     d1chZennerOffset ;
    double     d1chZennerGain   ;
    double     d2chLedOffset    ;
    double     d2chLedGain      ;
    double     d2chZennerOffset ;
    double     d2chZennerGain   ;

    int        iGlProbeVer      ;

} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
//  int        iModelName      ; //�𵨺� ����Ʈ ���� �ɼ��� ���� �̿�.  �ϵ���� �ٸ������� �̿��ϸ� �ȵ�.   TModelNo
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
    bool       bExistLoader   ;
    bool       bExistUnLoader ;
    int        iLangSel       ;
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
        int         m_iLangSel ; //Current Language

        AnsiString GetCrntDev () { return m_sCrntDev ; }

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
