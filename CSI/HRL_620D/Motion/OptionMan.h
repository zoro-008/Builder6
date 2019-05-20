//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };
//enum EN_ORI_MATHOD { //����Ĩ ��� ���
//    omMatch          = 0 , //��ġ
//    omEdgeLeftAndTop = 1 , //�������� ����,���� ������ ��´�.
//    omEdgeTopAndLeft = 2 , //�������� ����,���� ������ ��´�.
//};
//Device Informations.
//struct CDevInfo {   //device�� ���� Dev_Info
//
//} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    bool   bUsePstU1  ; //����1 U�� ������ ��� ����
    bool   bUsePstU2  ;
    bool   bUsePstU3  ;
    bool   bUsePstU4  ;
    bool   bUsePstU5  ;
    bool   bUsePstU6  ;
    bool   bUsePstU7  ;
    bool   bUsePstU8  ;
    bool   bUsePstU9  ;
    bool   bUsePstU10 ;

    int    iOriLsrDevU1  ; //����1 U�� Laser_Dev_No
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
struct CCmnOptn {   //��� ���� �ɼ�.
    bool bDoorSkip    ;
    bool bLaserSkip   ;
    bool bVaccumSkip  ;
    bool bBrd2LsrSkip ;
    bool bUseOnlyRgh  ;
    bool bDryRun      ;
    bool bNotUsedVAxis;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    int iCycleTm      ;


} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
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
