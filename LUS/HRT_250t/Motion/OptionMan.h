//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int    iBin1FullCnt   ;
    int    iBin2FullCnt   ;
    int    iBin3FullCnt   ;
    int    iBin4FullCnt   ;
    int    iBin5FullCnt   ; //��Ǯ

    int    iFailCnt       ; //���� ���� ����.

    int    iTempSet1      ; //�µ� ����.
    int    iTempSet2      ; //�µ� ����.

    int    iTempGap       ; //�µ� ���� �˶� �� iTempSet+iTempGap < x || x < iTempSet-iTempGap

    int    iPrbStartDelay ; //�������� ��ũ ��ġ���� ������.
    int    iPrbContactTime; //�����Ŀ� ������.
    int    iBlowerDelay   ; //���� ������ �Ҿ��ִ� ������.

    int    iPinChangeCnt  ; //�� ��ü ī��Ʈ.

    int    iNoPkgSec      ; //Pkg ���� �ȉ� Ȯ�� �ϴ� �ð�.(sec)
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
struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bFeedingStop ;
    bool   bIgnrHeatErr ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool bDebugMode       ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool bInspSkip        ;

    bool bIgnrPkgDustChk  ; //�μ����� xRAL_PkgDustChk���� �ѷ� �Ծ �ɼ� ó��.
    bool bFedMTPsChange   ; //���� ���������� �����̵� ���� ������ ���� �ǰ� �ɼ� ó��.
                            //������ �����̴� �Ÿ����� �ɼǰ��� ũ�� �� ��ġ ���� ���ư��� �Ǿ�����.JS
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
