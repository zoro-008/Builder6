//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#define  MAX_BIN_CNT 10
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    double dColPitch  ;
    double dRowPitch  ;
    int    iColCnt    ;
    int    iRowCnt    ;
    double dCsSlPitch ;
} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    double dMinBn [MAX_BIN_CNT] ;
    double dMaxBn [MAX_BIN_CNT] ;
    double dMinBnS[MAX_BIN_CNT] ;
    double dMaxBnS[MAX_BIN_CNT] ;

    int    iBnLSel[MAX_BIN_CNT] ;
    int    iBnLIdx[MAX_BIN_CNT] ;
    
    int    iBnRSel[MAX_BIN_CNT] ;
    int    iBnRIdx[MAX_BIN_CNT] ;

    int    iBinMaxCnt ;  //���ö��̾ ������ �ִ� ���� ����.

/*
//  double dMinBn0  ;  double dMaxBn0  ; ���� ����.
    double dMinBn1  ;  double dMaxBn1  ; //�츮������ �����Ѱ��� ����.
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

    double dMinBn1S ;  double dMaxBn1S ; //���۰� ������ ���� ����.
    double dMinBn2S ;  double dMaxBn2S ;
    double dMinBn3S ;  double dMaxBn3S ;
    double dMinBn4S ;  double dMaxBn4S ;
    double dMinBn5S ;  double dMaxBn5S ;
    double dMinBn6S ;  double dMaxBn6S ;
    double dMinBn7S ;  double dMaxBn7S ;
    double dMinBn8S ;  double dMaxBn8S ;
    double dMinBn9S ;  double dMaxBn9S ;
    double dMinBnAS ;  double dMaxBnAS ;

    int    iBinMaxCnt ;  //���ö��̾ ������ �ִ� ���� ����.    
*/


} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    int    iVacDelay     ;
    int    iPlaceDelay   ; //��Ŀ�� ���� ������ �԰� �˻�.
    int    iPlaceEjtTime ; //��Ŀ�� ���� ������ �Դ� ���� ������ ��Ʈ���� �ð�.
    int    iRezeroPickCnt; //�����θ� �����ϴ�  �� ī��Ʈ.
    double dMasterOffset ; //���� ������ ���ÿ� ���׿� ���� ���� �����V.
    bool   bLoadingStop  ; //Loading Stop
    bool   bEmptyIgnr    ; //Empty Pocker Ignr

    bool   bChuckFailIgnore   ; // ������ ô���� ���� ���� ����.
    bool   bUseBinFullIgnore  ; // ���� �� ã���� �׳� �ٽ� �������� �۾� ����
    bool   bUseFailBinIgnore  ; // ���� ���� ������ �׳� �ٽ� �������� �۾� ����

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
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool       bRandomSort        ; //��Ŀ���� �������� ����
    bool       bRandomSupply      ; //���ö��̿��� �������� ���ö���.
    bool       bCheckCycle        ; //���� �����ϰ� �ٽ� Ʈ���̿� �ִ� ����Ŭ/

    int        iMgzShowTime       ; //�޼��� ��Ÿ�� �ð� ����.
} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
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
