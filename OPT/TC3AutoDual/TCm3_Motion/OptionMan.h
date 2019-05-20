//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
//===========================================================================
struct CDevInfo {   //device에 대한 Dev_Info

} ;

//Device Option.
//===========================================================================
#define MAX_DISP_NOZZLE 6
#define MAX_DISP_CNT    5

struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int    iModeSelect     ;

    bool   bDryRun         ;
    bool   bDataCheck      ;
    bool   bInfinityInsp   ;
    bool   bInspDrngDisp   ; //토출중에도 검사하기.
    bool   bAdjGT2ndInsp   ; //토출중 검사하면서 GT도 조정 하는 옵션.
    bool   bDispFirst      ; //토출 먼저 하고 검사 하는 옵션.

    bool   bROMType        ;
    bool   bAskInsp        ;
    bool   bGrabMode       ;

    int    iMaxInspCnt     ;
    int    iInspDelay      ;

    bool   bInspBefrDisp   ; //토출전에 검사하기.  사용 안함.
    bool   bInspAftrDisp   ; //토출후에 검사하기.  사용 안함.

    bool   bAPCOnReadjust  ; //재검사시에 APC조정 하기.


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
    double dAPCCalLim      ; //한번 보정시에 변화 할 수 있는 리밋
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
struct CCmnOptn {   //장비 공용 옵션.
} ;

//Master Option.
//===========================================================================
struct CMstOptn {   //마스타 옵션. 화면상에는 없고 텍스트파일을 열어서 편집해야함.
    bool    bDebugMode  ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool    bSunMistake ; //1,2호기 DP 1,2 번 착각해서 뒤집힘. 알고 있어야 함.

} ;

//Eqipment Option.
//===========================================================================
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    int        iModelName     ; //모델별 소프트 웨어 옵션을 위해 이용.  하드웨어 다른점으로 이용하면 안됨.   TModelNo
    AnsiString sModelName     ; //sModelName 모델 이름 디스플레이용 이다.
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
