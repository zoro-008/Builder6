//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------


//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info

    double dColPitch    ;
    double dRowPitch    ;
    int    iColGrCnt    ;
    int    iRowGrCnt    ;
    double dColGrGap    ;
    double dRowGrGap    ;
    int    iColCnt      ;
    int    iRowCnt      ;
    int    iCsSlCnt     ;
    double dCsSlPitch   ;

} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    AnsiString sVisnIndexId    ; //비젼에 잡파일 아이디. 화일 명이 아니고 이놈이 잡파일 아이디다.
    double     dPSB_XInsVel    ;
    int        iColInspCnt     ; //검사 하는 Col Count.
    int        iRowInspCnt     ; //검사 하는 Row Count.
    bool       bUseUv          ; //UV조명. 사용.
    int        iLotEndMgzCnt   ; //LOT Mgz Cnt

    /*
    int        iSortItRslt1    ;
    int        iSortItRslt2    ;
    int        iSortItRslt3    ;
    int        iSortItRslt4    ;
    int        iSortItRslt5    ;
    int        iSortItRslt6    ;
    int        iSortItRslt7    ;
    int        iSortItRslt8    ;
    int        iSortItRslt9    ;
    int        iSortItRslt10   ;
    int        iSortItRslt11   ;
    int        iSortItRslt12   ;
    int        iSortItRslt13   ;
    int        iSortItRslt14   ;
    int        iSortItFail     ;

    int        iLevelRslt1     ;
    int        iLevelRslt2     ;
    int        iLevelRslt3     ;
    int        iLevelRslt4     ;
    int        iLevelRslt5     ;
    int        iLevelRslt6     ;
    int        iLevelRslt7     ;
    int        iLevelRslt8     ;
    int        iLevelRslt9     ;
    int        iLevelRslt10    ;
    int        iLevelRslt11    ;
    int        iLevelRslt12    ;
    int        iLevelRslt13    ;
    int        iLevelRslt14    ;
    int        iLevelFail      ;
    */
    int        iWK1StprDnDelay ;
    int        iPRBStprDnDelay ;


} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool bDryRun       ;
    bool bNoTrimFail   ;
    bool bIgnrDoor     ;
    bool bVisn1Skip    ;
    bool bVisn2Skip    ;
    bool bSortByItem   ;
    bool bTrimCheck    ;

    bool bAutoOperation; //전장비와 뒷장비가 같이 연동됌. Start Stop
    bool bAutoVsChange ; //자동 비젼 잡파일 체인지.

    
    bool bCheckFailCnt ; //페일 카운트로 에러 띄우는것 사용/미사용.
    int  iCheckFailCnt ; //비젼페일 갯수를 확인 하여 에러를 띄운다.

    bool bCheckSFailCnt; //같은 항목 페일 카운트로 에러 띄우는것 사용/미사용.
    int  iCheckSFailCnt; //같은 항목 비젼페일 갯수를 확인 하여 에러를 띄운다.

    int  iSrtMotrDelay ; //Sorting motor Start Delay.



    int        iSortItRslt1    ;
    int        iSortItRslt2    ;
    int        iSortItRslt3    ;
    int        iSortItRslt4    ;
    int        iSortItRslt5    ;
    int        iSortItRslt6    ;
    int        iSortItRslt7    ;
    int        iSortItRslt8    ;
    int        iSortItRslt9    ;
    int        iSortItRslt10   ;
    int        iSortItRslt11   ;
    int        iSortItRslt12   ;
    int        iSortItRslt13   ;
    int        iSortItRslt14   ;
    int        iSortItFail     ;

    int        iLevelRslt1     ;
    int        iLevelRslt2     ;
    int        iLevelRslt3     ;
    int        iLevelRslt4     ;
    int        iLevelRslt5     ;
    int        iLevelRslt6     ;
    int        iLevelRslt7     ;
    int        iLevelRslt8     ;
    int        iLevelRslt9     ;
    int        iLevelRslt10    ;
    int        iLevelRslt11    ;
    int        iLevelRslt12    ;
    int        iLevelRslt13    ;
    int        iLevelRslt14    ;
    int        iLevelFail      ;




} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode       ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    AnsiString sPreEquipPath    ;
    AnsiString sVisnPath        ;
    AnsiString sPstEquipDrv     ;
//  AnsiString sVisnDrv         ;
    AnsiString sPstEquipIp      ;
//  AnsiString sVisnIp          ;
    AnsiString sPstEquipPath    ;

    bool       bBarcodeOnly     ; //Only Barcode Input when Lot Open
    bool       bUld_YHome       ;
    bool       bPSB_XYTrmHome   ;
    bool       bLdr_YHome       ;
    bool       bEmptyNoTrim     ;
    bool       bUseHangul       ;
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
    bool       bExistLoader    ;
    bool       bExistUnLoader  ;
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
