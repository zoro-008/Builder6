//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info

    double dLDRColPitch    ; //Loader Tray
    int    iLDRColCnt      ;
    double dLDRRowPitch    ;
    int    iLDRRowCnt      ;

    double dScktPitch      ; //Table Socket ���� Ʈ���ſ��� ����.

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
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int iPickDelay ;
    int iPlceDelay ;

    int iLotTrayCnt; //�ϴ� �Ⱦ�.

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


struct CCmnOptn {   //��� ���� �ɼ�.
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

    bool bOSTNotUseCrnt ; //���� ���� ���� �ɼ�.
    bool bOSTNotUseOst  ; //OST ���� ���� �ɼ�.

    int  iOSTOSTBin   ;
    int  iOSTCrnBin   ;
    int  iVTDIMBin    ;
    int  iVTBLKBin    ;
    int  iVTVHBin     ;
    int  iVTDFTBin    ;
    int  iVTDFT2Bin   ; //��������Ʈ 20150331 �����������û�߰�.
    int  iVSNFailBin  ;

    int  iBfrVTPowrDelay ;
    int  iBfrVTInspDelay ;

    //���� ������.
    int  iVTContBlkCnt   ;
    int  iVTContDftCnt   ;
    int  iVTContDft2Cnt  ;
    int  iVTContVHCnt    ;


    int  iVSContNGCnt    ;
    int  iOSContNGCnt    ;

    int  iStgAftStopDelay; //�������� ������ ��ž�ϰ� ������.

    String sRj1Name ;
    String sRj2Name ;
    String sRj3Name ;
    String sRj4Name ;

    bool bVisnAutoConv ; //���� ���� ������.

    double dStgWorkOffset ; //OST ������ ��3�ð� �����ÿ� �������� �����ؿ� 0.01�� �������� �̵����� �ָ� ������ �ö�. 15.34~15.37���� �����. �߸ŷ� �س���.

    int  iOSTLogDays ;

} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool   bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool   bVacErrIgnr        ;
    int    iInspRandMask      ; //�˻� ��� �����ϰ� �������� iInspRandMask ��ġ ��ŭ ���� ����ŷ �ϱ�.0�̸� �Ⱦ�

    double dTrigerOffset      ;
    String sVisnRsltFile1     ; //���� ������� ���� �н�
    String sVisnRsltFile2     ; //���� ������� ���� �н�
    String sVisnDeviceFile    ; //���� ������� ���� �н�
    String sVisnLotNoFile     ; //���� ���ѹ�.�� ���� �н�
    String sVTDataFile1       ; //VT1 ��ſ� �����н�
    String sVTDataFile2       ; //VT2 ��ſ� �����н�
    String sVTDataFile3       ; //VT3 ��ſ� �����н�
    String sVTDataFile4       ; //VT4 ��ſ� �����н�

    double dOSTOffset1        ; //OST ���̺� ���̿� ���� �����V �ּҺ��ش� �����V.
    double dOSTOffset2        ;
    double dOSTOffset3        ;
    double dOSTOffset4        ;
    double dOSTGain1          ;
    double dOSTGain2          ;
    double dOSTGain3          ;
    double dOSTGain4          ;




} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName     ; //sModelName �� �̸� ���÷��̿� �̴�.
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