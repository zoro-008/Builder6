//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

struct CMstDevOptn {   //device에 대한 Dev_Info
    double dGainPreLoad      ; double dOfstPreLoad      ;
    double dGainPeakStroke   ; double dOfstPeakStroke   ;
    double dGainCustomStroke ; double dOfstCustomStroke ;
    double dGainSensitivity  ; double dOfstSensitivity  ;
    double dGainLinearity    ; double dOfstLinearity    ;
    double dGainImpedance    ; double dOfstImpedance    ;
    double dGainHysteresis   ; double dOfstHysteresis   ;

    //곽채훈 과장 0150826 요청... 디바이스 별 오프셑 먹도록....옮김
    double dMstOfsX[MAX_STAGE_CNT] ; //비젼 스테이지별 센터 오프셑.
    double dMstOfsY[MAX_STAGE_CNT] ; //비젼 스테이지별 센터 오프셑.


} ;
//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    bool   bUseDuplex        ; //simplex단방향  duplex양방향.
    int    iTestCrntMin      ; //단방향 검사일때는 사용하지 않는다.
    int    iTestCrntMax      ; //검사시 이설정치까지 전류를 인가한다.
    int    iStableDelay      ; //측정 시작 및 측정 피크치 상승도달 하고 하강 할때 2번 딜레이사용.
    int    iStepDelay        ;
    int    iStepIncrement    ;
    int    iDrvTtStrtCrnt    ; //2015.04.30 이기동 대리 요청. 원래 컨텍하고 부터 였는데 드라이브 틸트 시작 위치 추가.
    int    iDrvTtTestCrnt    ; //유저가 세팅한 전류값에서 틸트 검사 한다.
    int    iCustomStroke     ; //유저 세팅한곳에서 높이 검사한다.
    int    iPreLoadCheckHt   ; //프리로드 체크 하는 높이값. 이높이가 처음뜬 mA가 결과값.
    int    iImpdCheckCrnt    ; //임피던스 체크 커런트.
    int    iSensiSttCrnt     ; //센서티비티 측정 시점.
    int    iSensiEndCrnt     ; //센서티비티 측정 종점.
    int    iLinearSttCrnt    ; //20150430 이기동 대리 요청.. 두점의 직선에서 최고 많이 벋어난 놈을 리니어리티로 수치화
    int    iLinearEndCrnt    ; //20150430 이기동 대리 요청.. 두점의 직선에서 최고 많이 벋어난 놈을 리니어리티로 수치화
    bool   bUseBfCttOfs      ; //컨텍하기전에 구동틸트 값 구해서 SttCenter대신 기준점으로 삼고 작업 하는 옵션 20150211 IM 요청추가.
    int    iGraphMinY        ; //그래프 고정으로 하기 힘들어서 세팅으로 넣음.
    int    iGraphMaxY        ; //그래프 고정으로 하기 힘들어서 세팅으로 넣음.
    int    iHysteSttCrnt     ; //히스테리시스 측정 시점.
    int    iHysteEndCrnt     ; //히스테리시스 측정 종점.
    double dPreHysteMaxOfst  ; //1차 히스테리시스 검사 오프셑



    //빔 이미지 틸트 스펙 및 사용 여부.
    bool   bUseTiltInitR   ; double dMinTiltInitR   ; double dMaxTiltInitR   ;
    bool   bUseTiltInitT   ; double dMinTiltInitT   ; double dMaxTiltInitT   ;
    bool   bUseTiltInit    ; double dMinTiltInit    ; double dMaxTiltInit    ;
    bool   bUseTiltFull    ; double dMinTiltFull    ; double dMaxTiltFull    ;
    bool   bUseTiltDrive   ; double dMinTiltDrive   ; double dMaxTiltDrive   ;
    bool   bUseTiltDriveAbs; double dMinTiltDriveAbs; double dMaxTiltDriveAbs;
    bool   bUseTiltMax     ; double dMinTiltMax     ; double dMaxTiltMax     ;
    bool   bUseTiltLength  ; double dMinTiltLength  ; double dMaxTiltLength  ;

    //높이 측정 스펙 및 사용 여부.
    bool   bUsePreLoad     ; double dMinPreLoad     ; double dMaxPreLoad     ;
    bool   bUsePeakStroke  ; double dMinPeakStroke  ; double dMaxPeakStroke  ;
    bool   bUseCustomStroke; double dMinCustomStroke; double dMaxCustomStroke;
    bool   bUseSensitivity ; double dMinSensitivity ; double dMaxSensitivity ;
    bool   bUseLinearity   ; double dMinLinearity   ; double dMaxLinearity   ;
    bool   bUseImpedance   ; double dMinImpedance   ; double dMaxImpedance   ;
    bool   bUseHysteresis  ; double dMinHysteresis  ; double dMaxHysteresis  ;
    bool   bUseCaseShort   ; //20150716 케이스 쇼트 검사 추가.


    //Height 마스터 게인 오프셑 , 디바이스 별로 따라 다니나. 마스터창에서 수정 가능.










} ;


struct CCmnOptn {   //장비 공용 옵션.
    bool   bAutoSaveRsltData    ;
    bool   bAutoSaveRsltImg     ;
    bool   bAutoSaveRsltSc      ; //스크린샷.
    String sDataSavePath       ; //VT1 통신용 파일패스

    bool   bUseStage1 ;
    bool   bUseStage2 ;
    bool   bUseStage3 ;
    bool   bUseStage4 ;
    bool   bUseStage5 ;

    double dRegiGain  ;  //저항값 교정 게인.
    double dRegiofset ;

    double dCrntGain  ;  //전송하는 전류 값 의 게인.
    double dCrntOfst  ;  //전송하는 전류 값 의 오프셑.

    double dHghtGain  ;  //높이 측정 값 의 게인.
    double dHghtOfst  ;  //높이 측정 값 의 오프셑.



    double dVisnYMaxDpMin     ; //Display상에서 표현하는 맥스Y 분...

    bool   bDoubleHold        ; //컨텍전에 홀드 실린더를 한번 잡았다 놓는 옵션.


} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    double dVisnYMaxMin       ; //Vision Y Max 표현 분

    //코헬용 에서 쓰는 숨겨진 파라..... Text 파일 세팅한다. UI 에 없음.
    double d2GainPreLoad       ; double d2OfstPreLoad       ;
    double d2GainPeakStroke    ; double d2OfstPeakStroke    ;
    double d2GainCustomStroke  ; double d2OfstCustomStroke  ;
    double d2GainSensitivity   ; double d2OfstSensitivity   ;
    double d2GainLinearity     ; double d2OfstLinearity     ;
    double d2GainImpedance     ; double d2OfstImpedance     ;
    double d2GainHysteresis    ; double d2OfstHysteresis    ;

    double d2GainTiltInitR     ; double d2OfstTiltInitR     ;
    double d2GainTiltInitT     ; double d2OfstTiltInitT     ;
    double d2GainTiltInit      ; double d2OfstTiltInit      ;
    double d2GainTiltDrive     ; double d2OfstTiltDrive     ;
    double d2GainTiltDriveAbs  ; double d2OfstTiltDriveAbs  ;
    double d2GainTiltFull      ; double d2OfstTiltFull      ;
    double d2GainTiltMax       ; double d2OfstTiltMax       ;
    double d2GainTiltLength    ; double d2OfstTiltLength    ;





    //20150406 Drive Tilt Test Current , Sensitive Check Start Current , Sensitive Check End Current 의 세팅값을 Current , Height 인지 설정.
    bool   bSetUnitHeight ; //Drive Tilt 및 센서티비티 세팅 유닛을 전류값으로 할것인지 높이값으로 할것 인지.

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

        CMstDevOptn  MstDevOptn  ;
        CDevInfo     DevInfo     ;
        CDevOptn     DevOptn     ;
        CCmnOptn     CmnOptn     ;
        CMstOptn     MstOptn     ;
        CEqpOptn     EqpOptn     ;



        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (AnsiString _sName);

        void LoadJobFile   (AnsiString _sDevName); void SaveJobFile   (AnsiString _sDevName);

        void LoadMstDevOptn(AnsiString _sDevName); void SaveMstDevOptn(AnsiString _sDevName);
        void LoadDevInfo   (AnsiString _sDevName); void SaveDevInfo   (AnsiString _sDevName);
        void LoadDevOptn   (AnsiString _sDevName); void SaveDevOptn   (AnsiString _sDevName);
        void LoadCmnOptn   (                    ); void SaveCmnOptn   (                    );
        void LoadMstOptn   (                    ); void SaveMstOptn   (                    );
        void LoadEqpOptn   (                    ); void SaveEqpOptn   (                    );
        void LoadLastInfo  (                    ); void SaveLastInfo  (                    );

        bool m_bSaved[MAX_STAGE_CNT] ; //리절트 그래프 다시 그려주기위해.


};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
