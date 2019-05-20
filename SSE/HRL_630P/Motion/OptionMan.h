//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    //Strip

} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.

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
struct CCmnOptn {   //장비 공용 옵션.
    bool bIgnrDoorOpen ;
    bool bSkipLaserWrk ;
    int  iVsnFailCnt   ;
    int  iAirBlowTm    ;
    int  iAlignTm      ;
    int  iUseLsrPos    ; // 삼성 요청 사항...마킹 하고 싶은 포지션 선택 하는 옵션...

} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.

    bool   bDebugMode          ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool   bDryRun             ;

    double dOffset             ;

    String sCrntLotID          ;

    int    iPkgCnt             ;

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
