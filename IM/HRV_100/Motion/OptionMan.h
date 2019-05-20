//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

struct CMstDevOptn {   //device�� ���� Dev_Info
    double dGainPreLoad      ; double dOfstPreLoad      ;
    double dGainPeakStroke   ; double dOfstPeakStroke   ;
    double dGainCustomStroke ; double dOfstCustomStroke ;
    double dGainSensitivity  ; double dOfstSensitivity  ;
    double dGainLinearity    ; double dOfstLinearity    ;
    double dGainImpedance    ; double dOfstImpedance    ;
    double dGainHysteresis   ; double dOfstHysteresis   ;

    //��ä�� ���� 0150826 ��û... ����̽� �� �����V �Ե���....�ű�
    double dMstOfsX[MAX_STAGE_CNT] ; //���� ���������� ���� �����V.
    double dMstOfsY[MAX_STAGE_CNT] ; //���� ���������� ���� �����V.


} ;
//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    bool   bUseDuplex        ; //simplex�ܹ���  duplex�����.
    int    iTestCrntMin      ; //�ܹ��� �˻��϶��� ������� �ʴ´�.
    int    iTestCrntMax      ; //�˻�� �̼���ġ���� ������ �ΰ��Ѵ�.
    int    iStableDelay      ; //���� ���� �� ���� ��ũġ ��µ��� �ϰ� �ϰ� �Ҷ� 2�� �����̻��.
    int    iStepDelay        ;
    int    iStepIncrement    ;
    int    iDrvTtStrtCrnt    ; //2015.04.30 �⵿̱ �븮 ��û. ���� �����ϰ� ���� ���µ� ����̺� ƿƮ ���� ��ġ �߰�.
    int    iDrvTtTestCrnt    ; //������ ������ ���������� ƿƮ �˻� �Ѵ�.
    int    iCustomStroke     ; //���� �����Ѱ����� ���� �˻��Ѵ�.
    int    iPreLoadCheckHt   ; //�����ε� üũ �ϴ� ���̰�. �̳��̰� ó���� mA�� �����.
    int    iImpdCheckCrnt    ; //���Ǵ��� üũ Ŀ��Ʈ.
    int    iSensiSttCrnt     ; //����Ƽ��Ƽ ���� ����.
    int    iSensiEndCrnt     ; //����Ƽ��Ƽ ���� ����.
    int    iLinearSttCrnt    ; //20150430 �⵿̱ �븮 ��û.. ������ �������� �ְ� ���� ��� ���� ���ϾƼ�� ��ġȭ
    int    iLinearEndCrnt    ; //20150430 �⵿̱ �븮 ��û.. ������ �������� �ְ� ���� ��� ���� ���ϾƼ�� ��ġȭ
    bool   bUseBfCttOfs      ; //�����ϱ����� ����ƿƮ �� ���ؼ� SttCenter��� ���������� ��� �۾� �ϴ� �ɼ� 20150211 IM ��û�߰�.
    int    iGraphMinY        ; //�׷��� �������� �ϱ� ���� �������� ����.
    int    iGraphMaxY        ; //�׷��� �������� �ϱ� ���� �������� ����.
    int    iHysteSttCrnt     ; //�����׸��ý� ���� ����.
    int    iHysteEndCrnt     ; //�����׸��ý� ���� ����.
    double dPreHysteMaxOfst  ; //1�� �����׸��ý� �˻� �����V



    //�� �̹��� ƿƮ ���� �� ��� ����.
    bool   bUseTiltInitR   ; double dMinTiltInitR   ; double dMaxTiltInitR   ;
    bool   bUseTiltInitT   ; double dMinTiltInitT   ; double dMaxTiltInitT   ;
    bool   bUseTiltInit    ; double dMinTiltInit    ; double dMaxTiltInit    ;
    bool   bUseTiltFull    ; double dMinTiltFull    ; double dMaxTiltFull    ;
    bool   bUseTiltDrive   ; double dMinTiltDrive   ; double dMaxTiltDrive   ;
    bool   bUseTiltDriveAbs; double dMinTiltDriveAbs; double dMaxTiltDriveAbs;
    bool   bUseTiltMax     ; double dMinTiltMax     ; double dMaxTiltMax     ;
    bool   bUseTiltLength  ; double dMinTiltLength  ; double dMaxTiltLength  ;

    //���� ���� ���� �� ��� ����.
    bool   bUsePreLoad     ; double dMinPreLoad     ; double dMaxPreLoad     ;
    bool   bUsePeakStroke  ; double dMinPeakStroke  ; double dMaxPeakStroke  ;
    bool   bUseCustomStroke; double dMinCustomStroke; double dMaxCustomStroke;
    bool   bUseSensitivity ; double dMinSensitivity ; double dMaxSensitivity ;
    bool   bUseLinearity   ; double dMinLinearity   ; double dMaxLinearity   ;
    bool   bUseImpedance   ; double dMinImpedance   ; double dMaxImpedance   ;
    bool   bUseHysteresis  ; double dMinHysteresis  ; double dMaxHysteresis  ;
    bool   bUseCaseShort   ; //20150716 ���̽� ��Ʈ �˻� �߰�.


    //Height ������ ���� �����V , ����̽� ���� ���� �ٴϳ�. ������â���� ���� ����.










} ;


struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bAutoSaveRsltData    ;
    bool   bAutoSaveRsltImg     ;
    bool   bAutoSaveRsltSc      ; //��ũ����.
    String sDataSavePath       ; //VT1 ��ſ� �����н�

    bool   bUseStage1 ;
    bool   bUseStage2 ;
    bool   bUseStage3 ;
    bool   bUseStage4 ;
    bool   bUseStage5 ;

    double dRegiGain  ;  //���װ� ���� ����.
    double dRegiofset ;

    double dCrntGain  ;  //�����ϴ� ���� �� �� ����.
    double dCrntOfst  ;  //�����ϴ� ���� �� �� �����V.

    double dHghtGain  ;  //���� ���� �� �� ����.
    double dHghtOfst  ;  //���� ���� �� �� �����V.



    double dVisnYMaxDpMin     ; //Display�󿡼� ǥ���ϴ� �ƽ�Y ��...

    bool   bDoubleHold        ; //�������� Ȧ�� �Ǹ����� �ѹ� ��Ҵ� ���� �ɼ�.


} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool   bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    double dVisnYMaxMin       ; //Vision Y Max ǥ�� ��

    //����� ���� ���� ������ �Ķ�..... Text ���� �����Ѵ�. UI �� ����.
    double d2GainPreLoad       ; double d2OfstPreLoad       ;
    double d2GainPeakStroke    ; double d2OfstPeakStroke    ;
    double d2GainCustomStroke  ; double d2OfstCustomStroke  ;
    double d2GainSensitivity   ; double d2OfstSensitivity   ;
    double d2GainLinearity     ; double d2OfstLinearity     ;
    double d2GainImpedance     ; double d2OfstImpedance     ;
    double d2GainHysteresis    ; double d2OfstHysteresis    ;

    double d2GainTiltInitR     ; double d2OfstTiltInitR     ;
    double d2GainTiltInitT     ; double d2OfstTiltInitT     ;
    double d2GainTiltInit      ; double d2OfstTiltInit      ;
    double d2GainTiltDrive     ; double d2OfstTiltDrive     ;
    double d2GainTiltDriveAbs  ; double d2OfstTiltDriveAbs  ;
    double d2GainTiltFull      ; double d2OfstTiltFull      ;
    double d2GainTiltMax       ; double d2OfstTiltMax       ;
    double d2GainTiltLength    ; double d2OfstTiltLength    ;





    //20150406 Drive Tilt Test Current , Sensitive Check Start Current , Sensitive Check End Current �� ���ð��� Current , Height ���� ����.
    bool   bSetUnitHeight ; //Drive Tilt �� ����Ƽ��Ƽ ���� ������ ���������� �Ұ����� ���̰����� �Ұ� ����.

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

        CMstDevOptn  MstDevOptn  ;
        CDevInfo     DevInfo     ;
        CDevOptn     DevOptn     ;
        CCmnOptn     CmnOptn     ;
        CMstOptn     MstOptn     ;
        CEqpOptn     EqpOptn     ;



        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (AnsiString _sName);

        void LoadJobFile   (AnsiString _sDevName); void SaveJobFile   (AnsiString _sDevName);

        void LoadMstDevOptn(AnsiString _sDevName); void SaveMstDevOptn(AnsiString _sDevName);
        void LoadDevInfo   (AnsiString _sDevName); void SaveDevInfo   (AnsiString _sDevName);
        void LoadDevOptn   (AnsiString _sDevName); void SaveDevOptn   (AnsiString _sDevName);
        void LoadCmnOptn   (                    ); void SaveCmnOptn   (                    );
        void LoadMstOptn   (                    ); void SaveMstOptn   (                    );
        void LoadEqpOptn   (                    ); void SaveEqpOptn   (                    );
        void LoadLastInfo  (                    ); void SaveLastInfo  (                    );

        bool m_bSaved[MAX_STAGE_CNT] ; //����Ʈ �׷��� �ٽ� �׷��ֱ�����.


};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
