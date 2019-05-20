//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------


//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info

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
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    AnsiString sVisnIndexId    ; //������ ������ ���̵�. ȭ�� ���� �ƴϰ� �̳��� ������ ���̵��.
    double     dPSB_XInsVel    ;
    int        iColInspCnt     ; //�˻� �ϴ� Col Count.
    int        iRowInspCnt     ; //�˻� �ϴ� Row Count.
    bool       bUseUv          ; //UV����. ���.
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
struct CCmnOptn {   //��� ���� �ɼ�.
    bool bDryRun       ;
    bool bNoTrimFail   ;
    bool bIgnrDoor     ;
    bool bVisn1Skip    ;
    bool bVisn2Skip    ;
    bool bSortByItem   ;
    bool bTrimCheck    ;

    bool bAutoOperation; //������ ����� ���� ������. Start Stop
    bool bAutoVsChange ; //�ڵ� ���� ������ ü����.

    
    bool bCheckFailCnt ; //���� ī��Ʈ�� ���� ���°� ���/�̻��.
    int  iCheckFailCnt ; //�������� ������ Ȯ�� �Ͽ� ������ ����.

    bool bCheckSFailCnt; //���� �׸� ���� ī��Ʈ�� ���� ���°� ���/�̻��.
    int  iCheckSFailCnt; //���� �׸� �������� ������ Ȯ�� �Ͽ� ������ ����.

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
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode       ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
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
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
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
