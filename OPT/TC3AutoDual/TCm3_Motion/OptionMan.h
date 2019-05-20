//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
//===========================================================================
struct CDevInfo {   //device�� ���� Dev_Info

} ;

//Device Option.
//===========================================================================
#define MAX_DISP_NOZZLE 6
#define MAX_DISP_CNT    5

struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int    iModeSelect     ;

    bool   bDryRun         ;
    bool   bDataCheck      ;
    bool   bInfinityInsp   ;
    bool   bInspDrngDisp   ; //�����߿��� �˻��ϱ�.
    bool   bAdjGT2ndInsp   ; //������ �˻��ϸ鼭 GT�� ���� �ϴ� �ɼ�.
    bool   bDispFirst      ; //���� ���� �ϰ� �˻� �ϴ� �ɼ�.

    bool   bROMType        ;
    bool   bAskInsp        ;
    bool   bGrabMode       ;

    int    iMaxInspCnt     ;
    int    iInspDelay      ;

    bool   bInspBefrDisp   ; //�������� �˻��ϱ�.  ��� ����.
    bool   bInspAftrDisp   ; //�����Ŀ� �˻��ϱ�.  ��� ����.

    bool   bAPCOnReadjust  ; //��˻�ÿ� APC���� �ϱ�.


    bool   bBarcodeScan    ;

    //Angle.
    bool   bUseAngleCal    ;
    double dAgRes          ;
    double dMaxZPos        ;
    double dMaxYPos        ;
    int    iAngZVibCnt     ;
    int    iAngYVibCnt     ;
    double dAngVibRng      ;
    double dFindAngle      ;

    //Shift
    bool   bUseShiftCal    ;
    double dSfRes          ;
    double dMaxIPos        ;
    int    iSfVibCnt       ;
    double dSfVibRng       ;

    //APC
    bool   bUseAPCCal      ;
    double dApcRes         ;
    double dAPCSetVal      ;
    double dAPCCalLim      ; //�ѹ� �����ÿ� ��ȭ �� �� �ִ� ����
    double dAPCLim         ; //apc limit

    //Delay
    int    iSptrDnDelay    ;
    int    iSptrEndUpDelay ;
    int    iUVTime         ;

    //GT Up Down.
    int    iGTDnVibCnt     ;
    double dGTDnVibRng     ;
    double dGTUpOffset     ;
    int    iGT3rdInspCnt   ;
    double dAPCUnit        ;
    int    iAPCOnDelay     ;


    //Dispense
    //Left Zone ;
    int iLTDisp[MAX_DISP_NOZZLE][MAX_DISP_CNT];

    //Right Zone
    int iRTDisp[MAX_DISP_NOZZLE][MAX_DISP_CNT];

    int    iSuckTime      ;
    int    iDispEndDelay  ;

    bool   bUseBeamRslt   ;

    bool   bUseAvrPos     ;
    int    iAvrPosCnt     ;

    bool   bUseHomming    ;
    bool   bInspAfterDpDone;

    bool   bIgnrDisp      ;





//    double dZVibRange  ;
//    double dXVibRange  ;
//    double dIVibRange  ;
//
//    bool   bSupportUse ;



} ;

//Common Option.
//===========================================================================
struct CCmnOptn {   //��� ���� �ɼ�.
} ;

//Master Option.
//===========================================================================
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��󿡴� ���� �ؽ�Ʈ������ ��� �����ؾ���.
    bool    bDebugMode  ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool    bSunMistake ; //1,2ȣ�� DP 1,2 �� �����ؼ� ������. �˰� �־�� ��.

} ;

//Eqipment Option.
//===========================================================================
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    int        iModelName     ; //�𵨺� ����Ʈ ���� �ɼ��� ���� �̿�.  �ϵ���� �ٸ������� �̿��ϸ� �ȵ�.   TModelNo
    AnsiString sModelName     ; //sModelName �� �̸� ���÷��̿� �̴�.
} ;


//Password
//===========================================================================
//struct CPassword {
//    AnsiString  sEngrPass   ;
//    AnsiString  sMasterPass ;
//} ;
//
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
//        CPassword  Password ;

        AnsiString  m_sCrntDev ; //Current open device.
        int         m_iSelLvl  ; //Selected Level.
        int         m_iCrntLvl ; //Current Level.

        void Init(void);

//        void ChangeLevel   (int _iLevel);
//        bool CheckPassword (int _iLevel , AnsiString _sPassword);
//        bool ChangePassword(int _iLevel , AnsiString _sOldPass , AnsiString _sNewPass) ;


        void LoadJobFile  (AnsiString _sDevName); void SaveJobFile  (AnsiString _sDevName);

        void LoadDevInfo  (AnsiString _sDevName); void SaveDevInfo  (AnsiString _sDevName);
        void LoadDevOptn  (AnsiString _sDevName); void SaveDevOptn  (AnsiString _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );

        void LoadLastInfo (                    ); void SaveLastInfo (                    );
//        void LoadPassword (                    ); void SavePassword (                    );

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
