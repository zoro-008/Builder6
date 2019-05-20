//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#define  MAX_BIN_CNT 10
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    double dColPitch  ;
    double dRowPitch  ;
    int    iColCnt    ;
    int    iRowCnt    ;
    double dCsSlPitch ;
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    double dMinBn [MAX_BIN_CNT] ;
    double dMaxBn [MAX_BIN_CNT] ;
    double dMinBnS[MAX_BIN_CNT] ;
    double dMaxBnS[MAX_BIN_CNT] ;

    int    iBnLSel[MAX_BIN_CNT] ;
    int    iBnLIdx[MAX_BIN_CNT] ;
    
    int    iBnRSel[MAX_BIN_CNT] ;
    int    iBnRIdx[MAX_BIN_CNT] ;

    int    iBinMaxCnt ;  //서플라이어에 쌓을수 있는 자제 개수.

/*
//  double dMinBn0  ;  double dMaxBn0  ; 페일 자제.
    double dMinBn1  ;  double dMaxBn1  ; //우리측에서 측정한것의 범위.
    double dMinBn2  ;  double dMaxBn2  ;
    double dMinBn3  ;  double dMaxBn3  ;
    double dMinBn4  ;  double dMaxBn4  ;
    double dMinBn5  ;  double dMaxBn5  ;
    double dMinBn6  ;  double dMaxBn6  ;
    double dMinBn7  ;  double dMaxBn7  ;
    double dMinBn8  ;  double dMaxBn8  ;
    double dMinBn9  ;  double dMaxBn9  ;
    double dMinBnA  ;  double dMaxBnA  ;

    int iBnL1Sel ; int iBnL1Idx ;     int iBnR1Sel ; int iBnR1Idx ;
    int iBnL2Sel ; int iBnL2Idx ;     int iBnR2Sel ; int iBnR2Idx ;
    int iBnL3Sel ; int iBnL3Idx ;     int iBnR3Sel ; int iBnR3Idx ;
    int iBnL4Sel ; int iBnL4Idx ;     int iBnR4Sel ; int iBnR4Idx ;
    int iBnL5Sel ; int iBnL5Idx ;     int iBnR5Sel ; int iBnR5Idx ;
    int iBnL6Sel ; int iBnL6Idx ;     int iBnR6Sel ; int iBnR6Idx ;
    int iBnL7Sel ; int iBnL7Idx ;     int iBnR7Sel ; int iBnR7Idx ;
    int iBnL8Sel ; int iBnL8Idx ;     int iBnR8Sel ; int iBnR8Idx ;
    int iBnL9Sel ; int iBnL9Idx ;     int iBnR9Sel ; int iBnR9Idx ;
    int iBnLASel ; int iBnLAIdx ;     int iBnRASel ; int iBnRAIdx ;

    double dMinBn1S ;  double dMaxBn1S ; //오퍼가 측정한 것의 범위.
    double dMinBn2S ;  double dMaxBn2S ;
    double dMinBn3S ;  double dMaxBn3S ;
    double dMinBn4S ;  double dMaxBn4S ;
    double dMinBn5S ;  double dMaxBn5S ;
    double dMinBn6S ;  double dMaxBn6S ;
    double dMinBn7S ;  double dMaxBn7S ;
    double dMinBn8S ;  double dMaxBn8S ;
    double dMinBn9S ;  double dMaxBn9S ;
    double dMinBnAS ;  double dMaxBnAS ;

    int    iBinMaxCnt ;  //서플라이어에 쌓을수 있는 자제 개수.    
*/


} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    int    iVacDelay     ;
    int    iPlaceDelay   ; //픽커가 놓는 딜레이 먹고선 검사.
    int    iPlaceEjtTime ; //피커가 놓고 딜레이 먹는 동안 이젝터 터트리는 시간.
    int    iRezeroPickCnt; //리제로를 수행하는  픽 카운트.
    double dMasterOffset ; //지그 리제로 사용시에 지그와 자제 간의 오프셑.
    bool   bLoadingStop  ; //Loading Stop
    bool   bEmptyIgnr    ; //Empty Pocker Ignr

    bool   bChuckFailIgnore   ; // 안착후 척페일 감지 센서 무시.
    bool   bUseBinFullIgnore  ; // 빈통 다 찾으면 그냥 다시 내려놓고 작업 ㄱㄱ
    bool   bUseFailBinIgnore  ; // 페일 자재 있으면 그냥 다시 내려놓고 작업 ㄱㄱ

    int    iRsltColor0;
    int    iRsltColor1;
    int    iRsltColor2;
    int    iRsltColor3;
    int    iRsltColor4;
    int    iRsltColor5;
    int    iRsltColor6;
    int    iRsltColor7;
    int    iRsltColor8;
    int    iRsltColor9;
    int    iRsltColorA;
    int    iRsltColorB;
    int    iRsltColorC;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool       bRandomSort        ; //픽커에서 렌덤으로 소팅
    bool       bRandomSupply      ; //서플라이에서 랜덤으로 서플라이.
    bool       bCheckCycle        ; //높이 측정하고 다시 트레이에 넣는 싸이클/

    int        iMgzShowTime       ; //메세지 쇼타임 시간 설정.
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
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
