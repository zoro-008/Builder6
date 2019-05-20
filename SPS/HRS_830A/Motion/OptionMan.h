//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    //Strip
    double dColPitch       ;
    int    iColGrCnt       ;
    double dColGrGap       ;
    int    iColCnt         ;

    double dRowPitch       ;
    int    iRowGrCnt       ;
    double dRowGrGap       ;
    int    iRowCnt         ;

    //Mgz
    int    iCsSlCnt        ;
    double dCsSlPitch      ;

} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int    iLotEndMgzCnt     ; //�۾� Lot ����
    int    iPSBOutPshrDelay  ; //����Ʈ���� Out Pusher ������
    int    iPSBUpDnPshrDelay ; //����Ʈ���� Up Down Pusher ������
    int    iDispsrLTimer     ; //��ũ�� Dispenser Left Ÿ�̸�
    int    iDispsrRTimer     ; //��ũ�� Dispenser Right Ÿ�̸�
    int    iDispsrCnt        ; //���漭 ���ִ� ī��Ʈ
    int    iDispsrSnsrTimer  ; //���漭 ���� Ÿ��.
} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    bool      bLoadingStop   ;
    bool      bIgnrDoor      ;
    bool      bIgnrDisps     ;
    bool      bEmptyStrpCnt  ;
    bool      bIgnrDsprSnsr  ;
    bool      bIgnrDsprExpr  ;
    int       iEmptyStrpCnt  ;
    int       iExpiryDay     ;
    TDateTime LQSupplyDay    ;
    int       iDisprCleanCnt ;
    int       iStopToShotTime;

    String    sServerPath    ;

} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool   bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool   bDryRun            ;
    bool   bUsedSingleDisps   ;
    bool   bUsedDispsOptn     ;
    int    iDispsOptn         ;
    bool   bSkipOvldSnsr      ;
} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    String sModelName     ; //sModelName �� �̸� ���÷��̿� �̴�.

} ;

//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    protected:
        String  m_sCrntDev ; //Current open device.


    public:
        void Init();
        void Close();

        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;



        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (String _sName);

        void LoadJobFile  (String _sDevName); void SaveJobFile  (String _sDevName);

        void LoadDevInfo  (String _sDevName); void SaveDevInfo  (String _sDevName);
        void LoadDevOptn  (String _sDevName); void SaveDevOptn  (String _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );
        void LoadLastInfo (                    ); void SaveLastInfo (                    );

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
