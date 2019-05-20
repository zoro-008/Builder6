//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
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

    //Vision
    String sVisnIndexId ;
    int    iColInspCnt  ;
    int    iRowInspCnt  ;

    int    iColInspMrkCnt  ;
    int    iRowInspMrkCnt  ;

    int    iInsMrkVsColCnt ;
    int    iInsMrkVsRowCnt ;

} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int    iLotEndMgzCnt     ;

} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool   bDryRun           ;
    bool   bMrk1Skip         ;
    bool   bMrkSam1Skip      ;
    bool   bMrkAllSkip       ;
    bool   bIgnrVsErr        ;
    bool   bVsAllSkip        ;
    bool   bLoadingStop      ;
    bool   bVs1Skip          ;
    bool   bVs2Skip          ;
    bool   bVs3Skip          ;
    bool   bVsMrkSkip        ;
    bool   bDoorSkip         ;
    bool   bIdleRun          ;
//    int    iLotEndMgzCnt     ;

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

    int    iCheckFailCnt      ;
    
    int    iCheckR1FailCnt    ;
    int    iCheckR2FailCnt    ;
    int    iCheckR3FailCnt    ;
    int    iCheckR4FailCnt    ;
    int    iCheckR5FailCnt    ;
    int    iCheckR6FailCnt    ;
    int    iCheckR7FailCnt    ;
    int    iCheckR8FailCnt    ;
    int    iCheckR9FailCnt    ;
    int    iCheckR10FailCnt   ;
    int    iCheckR11FailCnt   ;
    int    iCheckR12FailCnt   ;
    int    iCheckR13FailCnt   ;
    int    iCheckR14FailCnt   ;

    String sCheckR1FailName    ;
    String sCheckR2FailName    ;
    String sCheckR3FailName    ;
    String sCheckR4FailName    ;
    String sCheckR5FailName    ;
    String sCheckR6FailName    ;
    String sCheckR7FailName    ;
    String sCheckR8FailName    ;
    String sCheckR9FailName    ;
    String sCheckR10FailName   ;
    String sCheckR11FailName   ;
    String sCheckR12FailName   ;
    String sCheckR13FailName   ;
    String sCheckR14FailName   ;

    bool   bR1FailSkip    ;
    bool   bR2FailSkip    ;
    bool   bR3FailSkip    ;
    bool   bR4FailSkip    ;
    bool   bR5FailSkip    ;
    bool   bR6FailSkip    ;
    bool   bR7FailSkip    ;
    bool   bR8FailSkip    ;
    bool   bR9FailSkip    ;
    bool   bR10FailSkip   ;
    bool   bR11FailSkip   ;
    bool   bR12FailSkip   ;
    bool   bR13FailSkip   ;
    bool   bR14FailSkip   ;



//    int    iWK1StprDnDelay    ;
    int    iWK2StprDnDelay    ;
    int    dWK1_XInsVel       ;
//    int    iWK3StprDnDelay    ;
//    int    iPRBStprDnDelay    ;

    double dWK1InsStprPsSpeed ;
    bool   bAutoVsChange      ;

    int    iPrbToWk1Delay     ;
    int    iPsbToOutDelay     ;

    bool   bEmptyNoMrk        ;

} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool       bUld_YHome         ;
    bool       bLdr_YHome         ;
    bool       bDataAccEng        ;
    String     sVisnPath          ;

    double     dTrigerOffset      ;
    bool       bPsbVsErrIgnr      ;
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
//  int        iModelName      ; //모델별 소프트 웨어 옵션을 위해 이용.  하드웨어 다른점으로 이용하면 안됨.   TModelNo
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
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
