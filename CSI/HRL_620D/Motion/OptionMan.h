//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };
//enum EN_ORI_MATHOD { //기준칩 잡는 방법
//    omMatch          = 0 , //매치
//    omEdgeLeftAndTop = 1 , //웨이퍼의 왼쪽,윗쪽 엣지를 잡는다.
//    omEdgeTopAndLeft = 2 , //웨이퍼의 윗쪽,왼쪽 엣지를 잡는다.
//};
//Device Informations.
//struct CDevInfo {   //device에 대한 Dev_Info
//
//} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    bool   bUsePstU1  ; //보드1 U축 포지션 사용 할지
    bool   bUsePstU2  ;
    bool   bUsePstU3  ;
    bool   bUsePstU4  ;
    bool   bUsePstU5  ;
    bool   bUsePstU6  ;
    bool   bUsePstU7  ;
    bool   bUsePstU8  ;
    bool   bUsePstU9  ;
    bool   bUsePstU10 ;

    int    iOriLsrDevU1  ; //보드1 U축 Laser_Dev_No
    int    iOriLsrDevU2  ;
    int    iOriLsrDevU3  ;
    int    iOriLsrDevU4  ;
    int    iOriLsrDevU5  ;
    int    iOriLsrDevU6  ;
    int    iOriLsrDevU7  ;
    int    iOriLsrDevU8  ;
    int    iOriLsrDevU9  ;
    int    iOriLsrDevU10 ;

    int    iRvsLsrDevU1  ;
    int    iRvsLsrDevU2  ;
    int    iRvsLsrDevU3  ;
    int    iRvsLsrDevU4  ;
    int    iRvsLsrDevU5  ;
    int    iRvsLsrDevU6  ;
    int    iRvsLsrDevU7  ;
    int    iRvsLsrDevU8  ;
    int    iRvsLsrDevU9  ;
    int    iRvsLsrDevU10 ;

} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool bDoorSkip    ;
    bool bLaserSkip   ;
    bool bVaccumSkip  ;
    bool bBrd2LsrSkip ;
    bool bUseOnlyRgh  ;
    bool bDryRun      ;
    bool bNotUsedVAxis;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    int iCycleTm      ;


} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
    int        iTotalWork      ;
} ;


//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    public:
        CDevOptn   DevOptn  ;
        CEqpOptn   EqpOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;

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
