//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };
enum EN_ORI_MATHOD { //기준칩 잡는 방법
    omFiducial   = 0 , //피듀셜 마크 메치
    omChip       = 1   //그냥 칩 메치

};
//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    //카세트 관련.
    int    iCsSlotCnt      ;
    double dCsSlotPitch    ;

    //웨이퍼 관련.
    double dWfPitchX       ;
    double dWfPitchY       ;
    int    iWfColCnt       ;
    int    iWfRowCnt       ;

    //맵 관련.
    int    iMapType        ;
    int    iWFrstCol       ;
    int    iWFrstRow       ;
    String sGoodStr1       ;
    String sEmptStr1       ;
    String sMaskStr1       ;
    int    iFlatAngle      ;
    int    iMskLine        ;
    bool   bMskExist       ;
    bool   bUseHex         ;
    String sExtention      ; //맵파일 확장자.


    //화이트 보드 관련.
    int    iBdXCnt         ;
    int    iBdYCnt         ;
    double dBdXPch         ;
    double dBdYPch         ;



} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int  iVisnChkChipColCnt  ; //기준 칩에서 이동 하는 Col 갯수.
    int  iVisnChkChipRowCnt  ; //기준 칩에서 이동 하는 Row 갯수.
    int  iPenCheckCnt        ; //몇번 작업 하고 펜체크를 하는지 설정. 0:체크 안함. 1:매번 , 2:2번째 찍을때 검사. .....
    bool bAutoOrigin         ; //오리진 검사 할 때 오토인지 매뉴얼인지
    EN_ORI_MATHOD iOriMathod ;

} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool   bDoorSkip     ; //
    bool   bLoadingStop  ; //Loading Stop
    bool   bUseBarcode   ; //바코드 사용 여부.

    String sSourcePath   ; //서버에 있는것을 가져온다.
    String sWorkedPath   ; //서버에 있는것을 가져오면 백업.

    int    iVsBfGrabDly  ; //비전 검사전 딜레이 . 모터 안정화 정도.
    double dStageTLmtErr ; //스테이지 각도 리밋 에러 범위.
    double dVisnPenYOft  ; //비전과 펜사이의 Y 오프셑.
    int    iPenDnCnt     ; //화이트 보드 팬체크시에 제자리에서 몇번찍는지.
    int    iPenAtDnDly   ; //펜 마킹시에 다운 하고 딜레이.
    int    iPenDnOverCnt ; //펜교체시에 리셑되고 펜의 카운트를 확인 하여 에러 띄움.

    bool   bLoadOnlyOneMapMarking;  //SP 김진식씨 요청...
    String sLoadOnlyOneMapMrkPath; //웨이퍼 외곽 영역 마킹 하고 싶은데 계속 한 맵파일만 자동으로 불러 오게 하도록 옵션 추가....JH

    bool   bUse1stBarcode ;

    bool   bUseEndCam     ; //작업 종료 카메라 사용여부.


} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode       ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    String     sDeviceBackUpPath;
    bool       bDryRun          ; //장비에 부자제 및 자재 모두 없이 드라이 러닝 하는 옵션. 사진을 찍지만 이미지 프로세싱은 안한다.
    bool       bVacuum          ; //체크 시 Vacuum On 아닐 시 Vacuum Off
    bool       bHopJumpChip     ;
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
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
