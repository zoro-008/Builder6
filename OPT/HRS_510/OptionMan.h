//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------
#define  MAX_BIN_CNT         10 //���� ����.
#define  MAX_PICKER_CNT      3  //��Ŀ ����.
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    double dColPitch  ;
    double dRowPitch  ;
    int    iColCnt    ;
    int    iRowCnt    ;
    double dCsSlPitch ;
} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    double dMinBn [MAX_OK_GRADE] ;
    double dMaxBn [MAX_OK_GRADE] ;

    int    iBnLSel[MAX_BIN_CNT] ;

    double dMasterOffset[MAX_PICKER_CNT] ; //���� ������ ���ÿ� ���׿� ���� ���� �����V. ������ ���� �� �� �ְ� ����.




} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    int    iVacDelay     ;
    int    iPlaceDelay   ; //��Ŀ�� ���� ������ �԰� �˻�.
    int    iPlaceEjtTime ; //��Ŀ�� ���� ������ �Դ� ���� ������ ��Ʈ���� �ð�.
    //int    iRezeroPickCnt; //�����θ� �����ϴ�  �� ī��Ʈ.
    int    iRezeroTrayCnt; //�����θ� �����ϴ�  Ʈ���� ī��Ʈ.





    bool   bLoadingStop  ; //Loading Stop
    bool   bEmptyIgnr    ; //Empty Pocker Ignr

    bool   bChuckFailIgnore   ; // ������ ô���� ���� ���� ����.
    bool   bUseBinFullIgnore  ; // ���� �� ã���� �׳� �ٽ� �������� �۾� ����
    bool   bUseFailBinIgnore  ; // ���� ���� ������ �׳� �ٽ� �������� �۾� ����

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


    bool bNotUseTool1 ; //20140224 �ʸ��ɿ��� ���� ���� ���� ���ϳ� ��ŵ �ϰ� ������ �ְ� ����. ���ͱպ��� ��û. ����� ����.
    bool bNotUseTool2 ;
    bool bNotUseTool3 ;


} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool       bRandomSort        ; //��Ŀ���� �������� ����
    bool       bRandomSupply      ; //���ö��̿��� �������� ���ö���.
    bool       bCheckCycle        ; //���� �����ϰ� �ٽ� Ʈ���̿� �ִ� ����Ŭ/

    int        iMgzShowTime       ; //�޼��� ��Ÿ�� �ð� ����.
} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
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
