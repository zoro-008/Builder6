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

    int    iColInspCnt;
    int    iRowInspCnt;

} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    //ProbOptn
    int    iPinCnt          ;
    int    iLimInsFailCnt   ;
    int    iReInspCnt       ; //프로브 검사 몇번 할건지...
    int    iLimAllFailCnt   ; //한스트립 페일 카운트 리밋.
    int    iContColCnt      ;
    int    iReGrabCnt1      ;
    double dReGrabPs1       ;
    int    iReGrabCnt2      ; //사용 안함 -1 로 해야 대낭....
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

    int    iPrbSttDelay     ; //프로브 검사 딜레이.

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
struct CCmnOptn {   //장비 공용 옵션.
    bool   bDryRun           ;
    bool   bIgnrDoor         ;
    bool   bIgnrIdCehck      ;
    bool   bIgnrInsp         ;
    bool   bIgnrHeatAlm      ;
    bool   bIgnrStrOverAlm   ;
    bool   bLoadingStop      ;
    bool   bAutoOperation    ; //앞뒤장비 연동 Stop Start Reset.
    bool   bAutoConvesion    ; //뒤에 장비 자동 잡파일 체인지.
    bool   bCheckFailCnt     ;
    int    iCheckFailCnt     ;

} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode       ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    AnsiString sPstEquipPath    ;
//    AnsiString sPstEquipDrv     ;
//    AnsiString sPstEquipIp      ;
    AnsiString sPreEquipPath    ;
    bool       bHeattingTest    ; //고자리에서 존나 찍기.
    bool       bNoLdUl          ; //로더 언로더 없이 테스트 하기.
    bool       bGripTest        ; //차민철 과장님 요청사항. 컨택 후에 그립 풀기.
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
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
//  int        iModelName      ; //모델별 소프트 웨어 옵션을 위해 이용.  하드웨어 다른점으로 이용하면 안됨.   TModelNo
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
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
