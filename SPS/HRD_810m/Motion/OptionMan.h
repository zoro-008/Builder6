//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };
enum EN_ORI_MATHOD { //����Ĩ ��� ���
    omFiducial   = 0 , //�ǵ�� ��ũ ��ġ
    omChip       = 1   //�׳� Ĩ ��ġ

};
//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    //ī��Ʈ ����.
    int    iCsSlotCnt      ;
    double dCsSlotPitch    ;

    //������ ����.
    double dWfPitchX       ;
    double dWfPitchY       ;
    int    iWfColCnt       ;
    int    iWfRowCnt       ;

    //�� ����.
    int    iMapType        ;
    int    iWFrstCol       ;
    int    iWFrstRow       ;
    String sGoodStr1       ;
    String sEmptStr1       ;
    String sMaskStr1       ;
    int    iFlatAngle      ;
    int    iMskLine        ;
    bool   bMskExist       ;
    bool   bUseHex         ;
    String sExtention      ; //������ Ȯ����.


    //ȭ��Ʈ ���� ����.
    int    iBdXCnt         ;
    int    iBdYCnt         ;
    double dBdXPch         ;
    double dBdYPch         ;



} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int  iVisnChkChipColCnt  ; //���� Ĩ���� �̵� �ϴ� Col ����.
    int  iVisnChkChipRowCnt  ; //���� Ĩ���� �̵� �ϴ� Row ����.
    int  iPenCheckCnt        ; //��� �۾� �ϰ� ��üũ�� �ϴ��� ����. 0:üũ ����. 1:�Ź� , 2:2��° ������ �˻�. .....
    bool bAutoOrigin         ; //������ �˻� �� �� �������� �Ŵ�������
    EN_ORI_MATHOD iOriMathod ;

} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bDoorSkip     ; //
    bool   bLoadingStop  ; //Loading Stop
    bool   bUseBarcode   ; //���ڵ� ��� ����.

    String sSourcePath   ; //������ �ִ°��� �����´�.
    String sWorkedPath   ; //������ �ִ°��� �������� ���.

    int    iVsBfGrabDly  ; //���� �˻��� ������ . ���� ����ȭ ����.
    double dStageTLmtErr ; //�������� ���� ���� ���� ����.
    double dVisnPenYOft  ; //������ ������� Y �����V.
    int    iPenDnCnt     ; //ȭ��Ʈ ���� ��üũ�ÿ� ���ڸ����� ��������.
    int    iPenAtDnDly   ; //�� ��ŷ�ÿ� �ٿ� �ϰ� ������.
    int    iPenDnOverCnt ; //�汳ü�ÿ� ���V�ǰ� ���� ī��Ʈ�� Ȯ�� �Ͽ� ���� ���.

    bool   bLoadOnlyOneMapMarking;  //SP �����ľ� ��û...
    String sLoadOnlyOneMapMrkPath; //������ �ܰ� ���� ��ŷ �ϰ� ������ ��� �� �����ϸ� �ڵ����� �ҷ� ���� �ϵ��� �ɼ� �߰�....JH

    bool   bUse1stBarcode ;

    bool   bUseEndCam     ; //�۾� ���� ī�޶� ��뿩��.


} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode       ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    String     sDeviceBackUpPath;
    bool       bDryRun          ; //��� ������ �� ���� ��� ���� ����� ���� �ϴ� �ɼ�. ������ ������ �̹��� ���μ����� ���Ѵ�.
    bool       bVacuum          ; //üũ �� Vacuum On �ƴ� �� Vacuum Off
    bool       bHopJumpChip     ;
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
