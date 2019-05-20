//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info

} ;

//Device Option.
#define MAX_OSP_PIN 48
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int  iEpmtyBin      ; // ���� ���� �ִ� Bin �� Ȯ��
} ;


struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bLoadingStop         ;
    bool   bFRunSkip            ;
    bool   bRRunSkip            ;

    int    iBin1FullMaxCnt    ; // Bin1 Box�� ���� ����ũ �ִ� ���� ����
    int    iBin2FullMaxCnt    ; // Bin2 Box�� ���� ����ũ �ִ� ���� ����

    double dLdrPickDely       ; // �δ� �� ������
    double dUldMskDetSnrDely  ; // ��δ� ����ũ ���� ���� ������
    double dFirstAirBlowTime  ; // ���� ù ���� ���� ��ο� �ð�
    double dSecondAirBlowTime ; // ���� �� ��° ���� ��ο� �ð�
    double dPickVccDely       ; // ��δ� Pick Vaccum ������ �ð�
    double dPackOpenVccDely   ; // ��δ� Pack Open Vaccum ������ �ð�
    double dPickEjctDely      ; // ��δ� Pick Ejector ������ �ð�
    double dPackEjctDely      ; // ��δ� Pack Ejector ������ �ð�
    double dMaxMaskBoxCnt     ; // ��δ� ����ũ �ڽ��� ���� ����ũ �ִ� ����
    double dPickBwBfrDely     ; // ��δ� ����ũ �����ϱ� �� ������ �ð�, ������ ���� �־� ���� �������� Pick Bwd�� Pick Rotate�� �� ��, ����ũ �� �������� Ÿ�̹� ������ �����ֱ� ���� ����
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool bDebugMode ;

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
