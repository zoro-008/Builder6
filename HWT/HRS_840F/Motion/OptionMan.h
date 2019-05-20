//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info

} ;

//Device Option.
#define MAX_OSP_PIN 48
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int  iEpmtyBin      ; // 현재 쓰고 있는 Bin 통 확인
} ;


struct CCmnOptn {   //장비 공용 옵션.
    bool   bLoadingStop         ;
    bool   bFRunSkip            ;
    bool   bRRunSkip            ;

    int    iBin1FullMaxCnt    ; // Bin1 Box에 들어가는 마스크 최대 개수 설정
    int    iBin2FullMaxCnt    ; // Bin2 Box에 들어가는 마스크 최대 개수 설정

    double dLdrPickDely       ; // 로더 픽 딜레이
    double dUldMskDetSnrDely  ; // 언로더 마스크 감지 센서 딜레이
    double dFirstAirBlowTime  ; // 레일 첫 번쨰 에어 블로우 시간
    double dSecondAirBlowTime ; // 레일 두 번째 에어 블로우 시간
    double dPickVccDely       ; // 언로더 Pick Vaccum 딜레이 시간
    double dPackOpenVccDely   ; // 언로더 Pack Open Vaccum 딜레이 시간
    double dPickEjctDely      ; // 언로더 Pick Ejector 딜레이 시간
    double dPackEjctDely      ; // 언로더 Pack Ejector 딜레이 시간
    double dMaxMaskBoxCnt     ; // 언로더 마스크 박스에 들어가는 마스크 최대 개수
    double dPickBwBfrDely     ; // 언로더 마스크 배출하기 전 딜레이 시간, 딜레이 값을 주어 다음 시퀀스인 Pick Bwd와 Pick Rotate를 할 때, 마스크 팩 떨어지는 타이밍 간섭을 없애주기 위해 만듬
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool bDebugMode ;

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


    public:
        void Init();
        void Close();

        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;


        AnsiString  m_sCrntDev ; //Current open device.

        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (AnsiString _sName);

        void LoadJobFile  (AnsiString _sDevName); void SaveJobFile  (AnsiString _sDevName);

        void LoadDevInfo  (AnsiString _sDevName); void SaveDevInfo  (AnsiString _sDevName);
        void LoadDevOptn  (AnsiString _sDevName); void SaveDevOptn  (AnsiString _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );
        void LoadLastInfo (                    ); void SaveLastInfo (                    );

        AnsiString GetBinName(int _iStat);//EN_CHIP_STAT _iStat);

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
