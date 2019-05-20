//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    //
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int iVaccmdelay     ; // Vaccum delay .
    int iDisprdelay     ; // Dispensor delay.
    int iEjectdelay     ; // Eject delay.
    int iPlaceDelay     ; // Place delay.
    int iEndDelay       ; // Place delay

    int iDisprSpd       ; // Dispenser Speed.

    int iDisprIncPosCnt ;
    int iDisprShotCnt   ;
    int iDisprRptCnt    ;

    double dJigPCLOfs ;

} ;

struct CCmnOptn {   //장비 공용 옵션.
    bool bDisprSkip;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool   bDryRun            ;
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName     ; //sModelName 모델 이름 디스플레이용 이다.
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
