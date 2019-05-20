//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    int    iColGrCnt       ;
    int    iRowGrCnt       ;
    int    iColCnt         ;
    int    iRowCnt         ;
    int    iColInspCnt     ;
    int    iRowInspCnt     ;
    int    iLDRColCnt      ;
    int    iLDRRowCnt      ;
    int    iULDColCnt      ;
    int    iULDRowCnt      ;
    int    iULDPkgCnt      ;

    //Vision
    String sVisnIndexId    ;

    double dColGrGap       ;
    double dRowGrGap       ;
    double dColPitch       ;
    double dRowPitch       ;
    double dLDRColPitch    ;
    double dLDRRowPitch    ;
    double dULDColPitch    ;
    double dULDRowPitch    ;

} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int    iLotEndMgzCnt     ;
    
    //Mask
    int    iLevelRslt1       ;
    int    iLevelRslt2       ;
    int    iLevelRslt3       ;
    int    iLevelRslt4       ;
    int    iLevelRslt5       ;
    int    iLevelRslt6       ;
    int    iLevelRslt7       ;
    int    iLevelRslt8       ;
    int    iLevelRslt9       ;
    int    iLevelRslt10      ;
    int    iLevelRslt11      ;
    int    iLevelRslt12      ;
    int    iLevelRslt13      ;
    int    iLevelRslt14      ;
    int    iLevelFail        ;

    int    iCheckFailCnt     ;

    int    iCheckR1FailCnt   ;
    int    iCheckR2FailCnt   ;
    int    iCheckR3FailCnt   ;
    int    iCheckR4FailCnt   ;
    int    iCheckR5FailCnt   ;
    int    iCheckR6FailCnt   ;
    int    iCheckR7FailCnt   ;
    int    iCheckR8FailCnt   ;
    int    iCheckR9FailCnt   ;
    int    iCheckR10FailCnt  ;
    int    iCheckR11FailCnt  ;
    int    iCheckR12FailCnt  ;
    int    iCheckR13FailCnt  ;
    int    iCheckR14FailCnt  ;

    String sCheckR1FailName  ;
    String sCheckR2FailName  ;
    String sCheckR3FailName  ;
    String sCheckR4FailName  ;
    String sCheckR5FailName  ;
    String sCheckR6FailName  ;
    String sCheckR7FailName  ;
    String sCheckR8FailName  ;
    String sCheckR9FailName  ;
    String sCheckR10FailName ;
    String sCheckR11FailName ;
    String sCheckR12FailName ;
    String sCheckR13FailName ;
    String sCheckR14FailName ;

} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bDryRun           ;
    bool   bMrk1Skip         ;
    bool   bMrkSam1Skip      ;
    bool   bMrkAllSkip       ;
    bool   bIgnrVsErr        ;
    bool   bVsAllSkip        ;
    bool   bVs1Skip          ;
    bool   bVs2Skip          ;
    bool   bVs3Skip          ;
    bool   bVsMrkSkip        ;
    bool   bDoorSkip         ;
    bool   bIdleRun          ;
    bool   bCheckFailCnt     ;
    bool   bAutoReview       ;
    bool   bAutoVsChange     ;
//    int    iLotEndMgzCnt     ;
    bool   bUsePenMrkMvX     ;
    bool   bUsePenMrkMvY     ;
    bool   bUseLDREmpty      ;
    bool   bAfterMrkRemove   ;


    int    iWK2StprDnDelay   ;
    int    dWK1_XInsVel      ;
    int    iPrbToWk1Delay    ;
    int    iPsbToOutDelay    ;
    int    iULDMgzDelay      ;
    int    iPaperCheckCnt    ;
    int    iLDREmptyCnt      ;

    double dWK1InsStprPsSpeed;
    double dPSBRejectDelay   ;
    double dULDVaccumDeley   ;
    double dLDRInDeley       ;
    double dPenMrkMvX        ;
    double dPenMrkMvY        ;
    double dPSB_VisionTime   ;


    int    iUldPkgEjtTime    ;
    int    iUldPprEjtTime    ;


} ;


//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool       bUld_YHome         ;
    bool       bLdr_YHome         ;
    bool       bDataAccEng        ;
    bool       bPsbVsErrIgnr      ;

    String     sVisnPath          ;

    double     dTrigerOffset      ;

} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
//  int        iModelName      ; //�𵨺� ����Ʈ ���� �ɼ��� ���� �̿�.  �ϵ���� �ٸ������� �̿��ϸ� �ȵ�.   TModelNo
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
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

        int    iTotalChip     ;
        int    iTotalFailChip ;
        bool   bLotFormDelay  ;

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
