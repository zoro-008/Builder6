//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info

    double dLDRColPitch    ; //Loader Tray
    int    iLDRColCnt      ;
    double dLDRRowPitch    ;
    int    iLDRRowCnt      ;

    double dScktPitch      ; //Table Socket 비젼 트리거에서 쓴다.

    double dULDColPitch    ; //UnLoader Tray
    int    iULDColCnt      ;
    double dULDRowPitch    ;
    int    iULDRowCnt      ;

} ;


/*
1 OST:110000000000000011001100100110001111111110000000
1IDD: (AVDD)0.03008 (CVDD)0.04384 (HVDD)0.00062 (DVDD)0.03226
1IDS: (AVDD)0.03006 (CVDD)0.04376 (HVDD)0.00058 (DVDD)0.03224
2 OST:000000000000000000000000000000000000000000000000
2IDD: (AVDD)8E-5 (CVDD)0 (HVDD)0 (DVDD)0
2IDS: (AVDD)0 (CVDD)0 (HVDD)0 (DVDD)0
3 OST:000000000000000000000000000000000000000000000000
3IDD: (AVDD)0 (CVDD)0 (HVDD)0 (DVDD)0
3IDS: (AVDD)0 (CVDD)0 (HVDD)0 (DVDD)0
4 OST:000000000000000000000000000000000000000000000000
4IDD: (AVDD)0 (CVDD)0 (HVDD)0 (DVDD)0
4IDS: (AVDD)0 (CVDD)0 (HVDD)0 (DVDD)0
*/

//Device Option.
#define MAX_OSP_PIN 48
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int iPickDelay ;
    int iPlceDelay ;

    int iLotTrayCnt; //일단 안씀.

    int iOSTDeviceNo;

    double dIDD_AVDDOkMin ; double dIDD_AVDDOkMax ;
    double dIDD_CVDDOkMin ; double dIDD_CVDDOkMax ;
    double dIDD_HVDDOkMin ; double dIDD_HVDDOkMax ;
    double dIDD_DVDDOkMin ; double dIDD_DVDDOkMax ;

    double dIDS_AVDDOkMin ; double dIDS_AVDDOkMax ;
    double dIDS_CVDDOkMin ; double dIDS_CVDDOkMax ;
    double dIDS_HVDDOkMin ; double dIDS_HVDDOkMax ;
    double dIDS_DVDDOkMin ; double dIDS_DVDDOkMax ;

    int iOST_I2cFailTime ;

    bool bVTPwrAllwaysOn ;

    String sOSTPinName[MAX_OSP_PIN] ;

    double dAVDD_IDDGain[MAX_TOOL_SOCK_COL];
    double dCVDD_IDDGain[MAX_TOOL_SOCK_COL];
    double dHVDD_IDDGain[MAX_TOOL_SOCK_COL];
    double dDVDD_IDDGain[MAX_TOOL_SOCK_COL];
    double dAVDD_IDSGain[MAX_TOOL_SOCK_COL];
    double dCVDD_IDSGain[MAX_TOOL_SOCK_COL];
    double dHVDD_IDSGain[MAX_TOOL_SOCK_COL];
    double dDVDD_IDSGain[MAX_TOOL_SOCK_COL];

    double dAVDD_IDDOfst[MAX_TOOL_SOCK_COL];
    double dCVDD_IDDOfst[MAX_TOOL_SOCK_COL];
    double dHVDD_IDDOfst[MAX_TOOL_SOCK_COL];
    double dDVDD_IDDOfst[MAX_TOOL_SOCK_COL];
    double dAVDD_IDSOfst[MAX_TOOL_SOCK_COL];
    double dCVDD_IDSOfst[MAX_TOOL_SOCK_COL];
    double dHVDD_IDSOfst[MAX_TOOL_SOCK_COL];
    double dDVDD_IDSOfst[MAX_TOOL_SOCK_COL];

} ;


struct CCmnOptn {   //장비 공용 옵션.
    bool bLoadingStop ;
    bool bIgnrDoor    ;
    bool bSkipOST     ;
    bool bSkipVT      ;
    bool bSkipVisn1   ;
    bool bSkipVisn2   ;

    bool bSkip1stTol  ;
    bool bSkip2ndTol  ;
    bool bSkip3thTol  ;
    bool bSkip4thTol  ;

    bool bOSTNotUseCrnt ; //전류 측정 제외 옵션.
    bool bOSTNotUseOst  ; //OST 측정 제외 옵션.

    int  iOSTOSTBin   ;
    int  iOSTCrnBin   ;
    int  iVTDIMBin    ;
    int  iVTBLKBin    ;
    int  iVTVHBin     ;
    int  iVTDFTBin    ;
    int  iVTDFT2Bin   ; //진성디펙트 20150331 이윤승팀장요청추가.
    int  iVSNFailBin  ;

    int  iBfrVTPowrDelay ;
    int  iBfrVTInspDelay ;

    //연속 에러들.
    int  iVTContBlkCnt   ;
    int  iVTContDftCnt   ;
    int  iVTContDft2Cnt  ;
    int  iVTContVHCnt    ;


    int  iVSContNGCnt    ;
    int  iOSContNGCnt    ;

    int  iStgAftStopDelay; //스테이지 돌고서 스탑하고 딜레이.

    String sRj1Name ;
    String sRj2Name ;
    String sRj3Name ;
    String sRj4Name ;

    bool bVisnAutoConv ; //비젼 오토 컨버젼.

    double dStgWorkOffset ; //OST 수율이 약3시간 구동시에 떨어지기 시작해여 0.01도 간격으로 이동시켜 주면 수율이 올라감. 15.34~15.37에서 사용중. 야매로 해놓음.

    int  iOSTLogDays ;

} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool   bVacErrIgnr        ;
    int    iInspRandMask      ; //검사 결과 무시하고 랜덤으로 iInspRandMask 수치 만큼 페일 마스킹 하기.0이면 안씀

    double dTrigerOffset      ;
    String sVisnRsltFile1     ; //비젼 결과값용 파일 패스
    String sVisnRsltFile2     ; //비젼 결과값용 파일 패스
    String sVisnDeviceFile    ; //비젼 결과값용 파일 패스
    String sVisnLotNoFile     ; //비젼 랏넘버.용 파일 패스
    String sVTDataFile1       ; //VT1 통신용 파일패스
    String sVTDataFile2       ; //VT2 통신용 파일패스
    String sVTDataFile3       ; //VT3 통신용 파일패스
    String sVTDataFile4       ; //VT4 통신용 파일패스

    double dOSTOffset1        ; //OST 케이블 길이에 따른 오프셑 최소분해능 오프셑.
    double dOSTOffset2        ;
    double dOSTOffset3        ;
    double dOSTOffset4        ;
    double dOSTGain1          ;
    double dOSTGain2          ;
    double dOSTGain3          ;
    double dOSTGain4          ;




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

        AnsiString GetBinName(int _iStat);//EN_CHIP_STAT _iStat);

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
