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
    bool   bEmptyStrpPlace    ;
    int    iEmptyStrpCnt      ; //�Ű��� ������ �������� ���� ��� �׳� ���� �ϴ� �ɼ�...
    int    iLotEndMgzCnt      ;
    int    iMarkInspFailCnt   ; //�̰� �ʰ� �ҷ� �߻��ÿ� ����. =0�̸� 1���߻��ÿ� ����.
    int    iRailStprDnDelay   ; //��ŷ�� ������ ������ �����̸԰� �������� ������ ������ ������ �԰� �������� ������ ������.
    int    iPSBGripDelay      ; //����Ʈ ���� ���� �ʿ� ��Ʈ�� �׸��� �Դ� ������.
    String sLaserLotFormat    ; //�������� �����ִ� ���� ����.

} ;

//Common Option.
enum EN_MARK_INSP_OPTN {
    moOneRow , //���ٰ˻�. ���ٰ˻� ��ü�˻� ��ȯ�ÿ� ���� �������� �ٲ���� ��.
    moAllRow , //��ü�˻�.
    moNone   , //�˻����.
};

struct CCmnOptn {   //��� ���� �ɼ�.
    bool              bLoadingStop  ;
    bool              bIgnrDoor     ;
    bool              bSkipOriVisn  ;
    bool              bSkipLaser    ;
    bool              bSkipAlnVisn  ;
    bool              bFrstStrpOut  ;
    EN_MARK_INSP_OPTN iMarkInsp     ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool   bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool   bDryRun            ;
    bool   bAllStrpSendLsrJob ; //��� ��Ʈ�� ������ ������ �Ѱ��ִ� �ɼ�....
    double dTrigerOffset      ; //���� Ʈ���� �¿� ��ȯ�� �����V.
    String sVisnRsltFile      ; //���� ������� ���� �н�
    String sVisnDeviceFile    ; //���� ������� ���� �н�
    String sLaserDataFile     ; //������ ��ſ� �����н�

} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName     ; //sModelName �� �̸� ���÷��̿� �̴�.
} ;


//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    protected:
        AnsiString  m_sCrntDev ; //Current open device.


    public:
        void Init();
        void Close();

        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;



        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (AnsiString _sName);

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
