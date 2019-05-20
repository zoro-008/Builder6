//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int    iBin1FullCnt   ;
    int    iBin2FullCnt   ;
    int    iBin3FullCnt   ;
    int    iBin4FullCnt   ;
    int    iBin5FullCnt   ; //빈풀

    int    iFailCnt       ; //연속 페일 에러.

    int    iTempSet1      ; //온도 세팅.
    int    iTempSet2      ; //온도 세팅.

    int    iTempGap       ; //온도 세팅 알람 갭 iTempSet+iTempGap < x || x < iTempSet-iTempGap

    int    iPrbStartDelay ; //컨택전에 워크 위치에서 딜레이.
    int    iPrbContactTime; //컨택후에 딜레이.
    int    iBlowerDelay   ; //소터 위에서 불어주는 딜레이.

    int    iPinChangeCnt  ; //핀 교체 카운트.

    int    iNoPkgSec      ; //Pkg 공급 안됌 확인 하는 시간.(sec)
    int    iPkgDetect     ;

    int    iFeedMtMoveTime;
    //int    iFeedWaitTime  ;
    //int    iFedFailCnt    ;

    int    iBin3Deley     ;
    int    iSortDeley     ;
    bool   bSortPKGSensor ;

    int    iEpmtyBin      ;

    //int    iFeedComDelay  ;
    int    iStartHeatSec  ;
    int    iPKGHeatSec    ;

    int    iStopTemp      ;
    int    iStopWaitSec   ;

    int    iIdleTime    ;


    //double dFedMinMove    ;
} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool   bFeedingStop ;
    bool   bIgnrHeatErr ;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool bDebugMode       ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool bInspSkip        ;

    bool bIgnrPkgDustChk  ; //민수형이 xRAL_PkgDustChk센서 뿌러 먹어서 옵션 처리.
    bool bFedMTPsChange   ; //모터 포지션으로 움직이되 센서 꺼지면 스톱 되게 옵션 처리.
                            //원래는 움직이는 거리보다 옵션값이 크면 그 위치 까지 돌아가게 되어있음.JS
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
