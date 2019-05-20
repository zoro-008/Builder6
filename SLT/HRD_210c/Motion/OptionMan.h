//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };
enum EN_ORI_MATHOD { //기준칩 잡는 방법
    omMatch          = 0 , //매치
    omEdgeLeftAndTop = 1 , //웨이퍼의 왼쪽,윗쪽 엣지를 잡는다.
    omEdgeTopAndLeft = 2 , //웨이퍼의 윗쪽,왼쪽 엣지를 잡는다.
};
//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    double dColPitch       ;
    double dRowPitch       ;
    int    iColGrCnt       ;
    int    iRowGrCnt       ;
    double dColGrGap       ;
    double dRowGrGap       ;
    int    iColCnt         ;
    int    iRowCnt         ;

    int    iMgzSlotCnt     ;
    double dMgzSlotPitch   ;

    int    iCsSlotCnt      ;
    double dCsSlotPitch    ;

    double dWf_PitchX      ;
    double dWf_PitchY      ;
    double dWf_BldOfs      ;
    double dWf_Diameter    ;

    int    iMapType        ;
    int    iWColCnt        ;
    int    iWRowCnt        ;
    int    iWFrstRow       ;
    int    iWFrstCol       ;

    int    iFlatAngle      ;

    int    iWXOffset       ;
    int    iWYOffset       ;

    String sGoodStr1       ;
    String sEmptStr1       ;
    int    iStrSttRow      ;
    int    iStrSttCol      ;



} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int    iLotEndMgzCnt   ;
    int    iEpxEjtOnTime   ; //Dispenser Before
    int    iEpxEjtOffTime  ; //Dispenser After

    double dWRowPitch      ; //이거 안씀. 나중에 정리.
    double dWColPitch      ;

    double dVoiceCoilStrip ;
    double dVoiceCoilStrip1;
    double dVoiceCoilMove  ;
    double dVoiceCoilWafer ;
    double dVoiceCoilWafer1;

    int    iEpxClenCnt     ; //에폭시 카운트 이거 이상일시 클린.
    int    iEpxCleanDly    ;
    int    iEpxCleanShotCnt; //에폭시 니들 청소시에 샷 횟수.

    int    iEpxTime        ;



    //HED
    int    iVacuumOnDly    ;
    int    iVacuumOffDly   ;

    //Ejector
    int    iEjtSuckOnDly   ;

    //Regrip asdfkljasg
    int    iPRBRgCol[MAX_REGRIP_ARRAY]       ;
    int    iPRBRgDis[MAX_REGRIP_ARRAY]       ;
    int    iWRERgCol[MAX_REGRIP_ARRAY]       ;
    int    iWRERgDis[MAX_REGRIP_ARRAY]       ;
    int    iWRDRgCol[MAX_REGRIP_ARRAY]       ;
    int    iWRDRgDis[MAX_REGRIP_ARRAY]       ;
    int    iPSBRgCol[MAX_REGRIP_ARRAY]       ;
    int    iPSBRgDis[MAX_REGRIP_ARRAY]       ;

    int    iPRB_XRgCnt     ;
    int    iWRE_XRgCnt     ;
    int    iWRD_XRgCnt     ;
    int    iPSB_XRgCnt     ;

    int    iPRB_XWkCon     ;
    int    iWRE_XWkCon     ;
    int    iWRD_XWkCon     ;
    int    iPSB_XWkCon     ;


    int    iLdrOrder       ;
    int    iUdrOrder       ;
    double dDFMDiameter    ;
    double dDFMRadius      ;
    double dDFMWidth       ;
    double dDFMHeight      ;
    double dDFMSttOfsX     ;
    double dDFMSttOfsY     ;

    double dPickWaitAc     ;
    double dPickWaitDc     ;
    double dPickWaitVel    ;

    double dPlaceWaitAc    ;
    double dPlaceWaitDc    ;
    double dPlaceWaitVel   ;

    int    iEjectOnDly     ;

    int    iEjectorBfDnDly ;

    //Vision
    int    iOriVsFailCnt   ; //연속 페일 카운트.
    int    iOriVsBfDly     ; //비전 검사전 딜레이 . 모터 안정화 정도.
    int    iOriVsGrabTO    ; //그랩 타임아웃.
    int    iOriVsInspTO    ; //인스펙션 타임아웃.

    int    iWfrVsFailCnt   ;
    int    iWfrVsBfDly     ; //Inlet 비전 검사전 딜레이 . 모터 안정화 정도.
    int    iWfrVsGrabTO    ;
    int    iWfrVsInspTO    ;

    int    iRalBfVsFailCnt   ;
    int    iRalBfVsBfDly     ; //Inlet 비전 검사전 딜레이 . 모터 안정화 정도.
    int    iRalBfVsGrabTO    ;
    int    iRalBfVsInspTO    ;

    int    iRalAtVsFailCnt   ;
    int    iRalAtVsBfDly     ; //Inlet 비전 검사전 딜레이 . 모터 안정화 정도.
    int    iRalAtVsGrabTO    ;
    int    iRalAtVsInspTO    ;

    int    iOriMathod        ; //매칭 옵션.   EN_ORI_MATHOD
    bool   bOperChckAtMatch  ; //오퍼가 매칭후 채크 하게

    int    iWfFullRctCntX    ; //완전한 사각형의 갯수. 항상 맵의 갯수보다 같거나 많다.
    int    iWfFullRctCntY    ;
    int    iWfFullRctMapOfsX ; //완벽한 첫 번째 사각형에서 맵상의 첩번째 사각형까지의 오프셑. 항상 0보다 크거나 같다.
    int    iWfFullRctMapOfsY ;

    int    iSecondFindPos    ; //두번째 축 찾으때 웨이퍼 칩 위치...      

    int    iLdrCanHomePrbCnt ; //로더 홈을 잡을 수 있는 프리버퍼 작업 진행 갯수.

} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool   bDoorSkip             ; //
    bool   bLoadingStop          ; //Loading Stop

    bool   bEpoxyStop            ; //Epoxy Stop

    bool   bIgnrOriVs            ; //Inlet 비전 검사 무시.
    bool   bIgnrRalAtVs          ; //
    bool   bIgnrVcSsr            ; //
    bool   bUseStgTheta          ; //스테이지 T보정 사용.

    bool   bIgnrRalBfVsEpoxyFail ;

    bool   bUseMapFile           ; //맵화일 모드 선택.
    bool   bInkDieWork           ; //잉크다이만 골라서 작업하는 모드.(no map mode에서만 사용 가능)

    String sSourcePath           ;
    String sWorkedPath           ;

    bool   bUseBtAlign           ;
    bool   bIgnrHedOverload      ;

    bool   bUseHeadTWfr          ; //해드 세타 모터 사용.
    bool   bUseHeadTRal          ; //해드 세타 모터 사용.


    double dStageTLmtErr         ; //스테이지 각도 리밋 에러.
    double dHedTLmtErr           ;

    int    iEjctrDnAfterDelay    ; //이젝터 핀 다운 하고 기다리는 시간.
    int    iEjctrUpAfterDelay    ; //이젝터 핀 업  하고 기다리는 시간. 일단 배큠 온딜레이가 있어서 만들고 안씀.

    bool   bWfrVisnRetest        ; //웨이퍼 비전 두번 검사.
    bool   bStageStop            ; //스테이지 Stop

    int    iLotEndChipCnt        ;

    bool   bUseVcCoil            ; //보이스 코일 사용여부.





//    int    iStgThetaMin  ;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode       ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool       bIgnrEpoxy       ;
    double     dEpxOriSpd       ;
    double     dEpxOriOfs       ;
    bool       bIgnrDieAttach   ;

    double     dVoiceCoilOffset ;
    double     dVoiceCoilA      ; //보이스 코일 입력값(g)을 V로 변환해 주는 환산식 A  y=ax+b
    double     dVoiceCoilB      ; //보이스 코일 입력값(g)을 V로 변환해 주는 환산식 B

    double     d1stDis          ;
    double     d2stDis          ;
    double     d3stDis          ;

    double     dStgXEjtCt       ;
    double     dStgYEjtCt       ;

    bool       bIgnrBfVision    ;
    bool       bIgnrWfVision    ;
    double     dEjtDia          ; //충돌 방지용. 이젝터 봉 직경.
    double     dStgDia          ; //충돌 방지용. 웨이퍼 직경.

    double     dStgYMaxExdAble  ;
    String     sDeviceBackUpPath;

    double     dToolCrashWfrY   ; //웨이퍼쪽 충돌 방지.
    double     dToolCrashRalY   ; //레일쪽 충돌 방지.
    double     dToolCrashZ      ;

    bool       bPickUpTestMode  ; //플레이스 신경 안쓰고 픽업만 하고 싶을때.
    bool       bEjectorDnFirst  ; //픽할때 이젝터 먼저 내려 오고 툴이 올라가는 옵션.

    bool       bDryRun          ; //장비에 부자제 및 자재 모두 없이 드라이 러닝 하는 옵션. 사진을 찍지만 이미지 프로세싱은 안한다.

    double     dRegripOfs       ; //그리퍼들 리그립시에 오프셑 적용.




} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
    int        iTotalChip      ;
    int        iTotalFailChip  ;
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
