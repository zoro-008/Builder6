//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
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
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    bool   bEmptyStrpPlace    ;
    int    iEmptyStrpCnt      ; //매거진 자제가 연속으로 없을 경우 그냥 배출 하는 옵션...
    int    iLotEndMgzCnt      ;
    int    iMarkInspFailCnt   ; //이거 초과 불량 발생시에 에러. =0이면 1개발생시에 에러.
    int    iRailStprDnDelay   ; //마킹존 스토퍼 내리고 딜레이먹고 레이져존 스토퍼 내리고 딜레이 먹고 프리버퍼 스토퍼 내리고.
    int    iPSBGripDelay      ; //포스트 버퍼 인입 쪽에 스트립 그립퍼 먹는 딜레이.
    String sLaserLotFormat    ; //레이저에 보내주는 랏의 포멧.

} ;

//Common Option.
enum EN_MARK_INSP_OPTN {
    moOneRow , //한줄검사. 한줄검사 전체검사 전환시에 비젼 잡파일을 바꿔줘야 함.
    moAllRow , //전체검사.
    moNone   , //검사없음.
};

struct CCmnOptn {   //장비 공용 옵션.
    bool              bLoadingStop  ;
    bool              bIgnrDoor     ;
    bool              bSkipOriVisn  ;
    bool              bSkipLaser    ;
    bool              bSkipAlnVisn  ;
    bool              bFrstStrpOut  ;
    EN_MARK_INSP_OPTN iMarkInsp     ;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool   bDryRun            ;
    bool   bAllStrpSendLsrJob ; //모든 스트립 레이저 잡파일 넘겨주는 옵션....
    double dTrigerOffset      ; //비젼 트리거 좌우 전환시 오프셑.
    String sVisnRsltFile      ; //비젼 결과값용 파일 패스
    String sVisnDeviceFile    ; //비젼 결과값용 파일 패스
    String sLaserDataFile     ; //레이져 통신용 파일패스

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
