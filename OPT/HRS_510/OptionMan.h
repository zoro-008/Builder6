//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------
#define  MAX_BIN_CNT         10 //빈통 갯수.
#define  MAX_PICKER_CNT      3  //픽커 갯수.
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    double dColPitch  ;
    double dRowPitch  ;
    int    iColCnt    ;
    int    iRowCnt    ;
    double dCsSlPitch ;
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    double dMinBn [MAX_OK_GRADE] ;
    double dMaxBn [MAX_OK_GRADE] ;

    int    iBnLSel[MAX_BIN_CNT] ;

    double dMasterOffset[MAX_PICKER_CNT] ; //지그 리제로 사용시에 지그와 자제 간의 오프셑. 잡파일 적용 할 수 있게 변경.




} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    int    iVacDelay     ;
    int    iPlaceDelay   ; //픽커가 놓는 딜레이 먹고선 검사.
    int    iPlaceEjtTime ; //피커가 놓고 딜레이 먹는 동안 이젝터 터트리는 시간.
    //int    iRezeroPickCnt; //리제로를 수행하는  픽 카운트.
    int    iRezeroTrayCnt; //리제로를 수행하는  트레이 카운트.





    bool   bLoadingStop  ; //Loading Stop
    bool   bEmptyIgnr    ; //Empty Pocker Ignr

    bool   bChuckFailIgnore   ; // 안착후 척페일 감지 센서 무시.
    bool   bUseBinFullIgnore  ; // 빈통 다 찾으면 그냥 다시 내려놓고 작업 ㄱㄱ
    bool   bUseFailBinIgnore  ; // 페일 자재 있으면 그냥 다시 내려놓고 작업 ㄱㄱ

    int    iRsltColor0;
    int    iRsltColor1;
    int    iRsltColor2;
    int    iRsltColor3;
    int    iRsltColor4;
    int    iRsltColor5;
    int    iRsltColor6;
    int    iRsltColor7;
    int    iRsltColor8;
    int    iRsltColor9;
    int    iRsltColorA;
    int    iRsltColorB;
    int    iRsltColorC;


    bool bNotUseTool1 ; //20140224 필리핀에서 뭔가 고장 나서 툴하나 스킵 하고 돌릴수 있게 수정. 박익균부장 요청. 선계원 수정.
    bool bNotUseTool2 ;
    bool bNotUseTool3 ;


} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool       bRandomSort        ; //픽커에서 렌덤으로 소팅
    bool       bRandomSupply      ; //서플라이에서 랜덤으로 서플라이.
    bool       bCheckCycle        ; //높이 측정하고 다시 트레이에 넣는 싸이클/

    int        iMgzShowTime       ; //메세지 쇼타임 시간 설정.
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
