//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    //Strip

} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.

    bool bUseLsrPst1 ;
    bool bUseLsrPst2 ;
    bool bUseLsrPst3 ;
    bool bUseLsrPst4 ;
    bool bUseLsrPst5 ;

    int  iFrtLsrNo1  ;
    int  iFrtLsrNo2  ;
    int  iFrtLsrNo3  ;
    int  iFrtLsrNo4  ;
    int  iFrtLsrNo5  ;

    int  iLerLsrNo1  ;
    int  iLerLsrNo2  ;
    int  iLerLsrNo3  ;
    int  iLerLsrNo4  ;
    int  iLerLsrNo5  ;

} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    bool bIgnrDoorOpen ;
    bool bSkipLaserWrk ;
    int  iVsnFailCnt   ;
    int  iAirBlowTm    ;
    int  iAlignTm      ;
    int  iUseLsrPos    ; // �Ｚ ��û ����...��ŷ �ϰ� ���� ������ ���� �ϴ� �ɼ�...

} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.

    bool   bDebugMode          ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool   bDryRun             ;

    double dOffset             ;

    String sCrntLotID          ;

    int    iPkgCnt             ;

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

    public:
        void Init();
        void Close();

        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;

        AnsiString  m_sCrntDev ; //Current open device.

        AnsiString GetCrntDev () { return m_sCrntDev ; }

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
