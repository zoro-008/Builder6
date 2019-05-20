//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    double dColPitch   ;
    double dRowPitch   ;
    int    iColGrCnt   ;
    int    iRowGrCnt   ;
    double dColGrGap   ;
    double dRowGrGap   ;
    int    iColCnt     ;
    int    iRowCnt     ;
    int    iCsSlCnt    ;
    double dCsSlPitch  ;

    int    iColInspCnt ;
    int    iRowInspCnt ;

    double dRowSubGrGap;
    int    iRowSubGrCnt;
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    double dWorkXWaitOfs       ; //컨텍할때 Z축 올리는 포지션.
    double dWRKZTopSpeed       ;
    bool   bWRKZTopRepeat      ;
    //ProbOptn
    int    iPinCnt             ;
    int    iLimInsFailCnt      ;
    int    iReInspCnt          ; //프로브 검사 몇번 할건지...
    int    iLimAllFailCnt      ; //한스트립 페일 카운트 리밋.
    int    iContColCnt         ;

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

    int    iPrbSttDelay        ; //프로브 검사 딜레이.

    bool   bUseTwiceInsp       ;

    //Dev Option
    int    iLotEndMgzCnt       ;
    int    iWaitDelay          ;

    bool   bLtHeaterNum2       ;
    bool   bLtHeaterNum3       ;
    bool   bRtHeaterNum2       ;
    bool   bRtHeaterNum3       ;

    double dPSBReGripOfs       ; //포스트 버퍼 리그립 시에 이동량.
    int    iPSBReGripCol       ; //포스트 버퍼 리그립 시점 컬럼 설정.

    double dREJPitch           ;

    double dTrimPtchOfs        ;
    double dWRKPtchOfs         ;
    //double dTrimMoveOfs        ;

    int    iTrimRjtDly         ;
    int    iTrimSnrDly         ;

    int    iContactPinCnt      ;
    int    iLimContactPinCnt   ;

    int    iRejectMoveDeley    ;

    bool   bSkipPinCnt         ; //체크 시 1랏 진행 하는 동안 Pin Count Error 무시.
                                 //1랏 진행 후 해체 됨.

    int    iFrtPrcFailCnt      ; //전공정에서 불량 확인 갯수.
    int    iPRBRlrDelay        ;
    int    iPRBRlrOffDelay     ;


} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool   bDryRun           ;
    bool   bIgnrDoor         ;
    bool   bIgnrVisn         ;
    bool   bIgnrInsp         ;
    bool   bIgnrHeatAlm      ;
    bool   bLoadingStop      ;
    //bool   bAutoOperation    ; //앞뒤장비 연동 Stop Start Reset.
    //bool   bAutoConvesion    ; //뒤에 장비 자동 잡파일 체인지.
    bool   bCheckFailCnt     ;
    int    iCheckFailCnt     ;
    bool   bIgnrTrim         ;
    bool   bIgnrVacuum       ;
    bool   bSortSensorDelay  ;
    int    iSortSensorDelay  ;
    //bool   bIgnrDecChk       ;
    //bool   bIgnrTrimDecChk   ;

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
    //bool   bUsedTopViewPnch  ;

    bool   bSkipYieldErr     ; //기준 수율보다 미달 될 시 에러 띄우는 기능 선택
    double dStandardYield    ; //기준 수율 옵션

    //Bin 사용 옵션.
    bool   bUsedBin1         ;
    bool   bUsedBin2         ;
    bool   bUsedBin3         ;
    bool   bUsedBin4         ;
    bool   bUsedBin5         ;
    bool   bUsedBin6         ;
    bool   bUsedBin7         ;
    bool   bUsedBin8         ;
    bool   bUsedBin9         ;
    bool   bUsedBin10        ;

    //Bin Min Max VF Value.
    double dMinVf1           ; double dMaxVf1           ;
    double dMinVf2           ; double dMaxVf2           ;
    double dMinVf3           ; double dMaxVf3           ;
    double dMinVf4           ; double dMaxVf4           ;
    double dMinVf5           ; double dMaxVf5           ;
    double dMinVf6           ; double dMaxVf6           ;
    double dMinVf7           ; double dMaxVf7           ;
    double dMinVf8           ; double dMaxVf8           ;
    double dMinVf9           ; double dMaxVf9           ;
    double dMinVf10          ; double dMaxVf10          ;

    //Bin Min Max Zenner Value.
    double dMinVr1         ; double dMaxVr1         ;
    double dMinVr2         ; double dMaxVr2         ;
    double dMinVr3         ; double dMaxVr3         ;
    double dMinVr4         ; double dMaxVr4         ;
    double dMinVr5         ; double dMaxVr5         ;
    double dMinVr6         ; double dMaxVr6         ;
    double dMinVr7         ; double dMaxVr7         ;
    double dMinVr8         ; double dMaxVr8         ;
    double dMinVr9         ; double dMaxVr9         ;
    double dMinVr10        ; double dMaxVr10        ;

    int    iBinDefaultNo  ;





} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    AnsiString sPstEquipPath      ;
//    AnsiString sPstEquipDrv       ;
//    AnsiString sPstEquipIp        ;
    AnsiString sPreEquipPath      ;
    bool       bHeattingTest      ; //고자리에서 존나 찍기.
    bool       bBarcodeUseOnly    ;
    bool       bByChInsp          ;
    bool       bNotUstLot         ;
    bool       bShortMonitor      ; // 0볼트대가 나오면 에러 띄움.

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

    int        iTempOfs           ;
    int        iMaxTemp           ;
    int        iPreMaxTemp        ;

    bool       bUsedEngVersion    ;

    bool       bUsedBinSorting    ;

    bool       bUsed4RowCntct     ;
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
