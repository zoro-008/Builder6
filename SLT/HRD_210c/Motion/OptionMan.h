//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };
enum EN_ORI_MATHOD { //����Ĩ ��� ���
    omMatch          = 0 , //��ġ
    omEdgeLeftAndTop = 1 , //�������� ����,���� ������ ��´�.
    omEdgeTopAndLeft = 2 , //�������� ����,���� ������ ��´�.
};
//Device Informations.
struct CDevInfo {   //device�� ���� Dev_Info
    double dColPitch       ;
    double dRowPitch       ;
    int    iColGrCnt       ;
    int    iRowGrCnt       ;
    double dColGrGap       ;
    double dRowGrGap       ;
    int    iColCnt         ;
    int    iRowCnt         ;

    int    iMgzSlotCnt     ;
    double dMgzSlotPitch   ;

    int    iCsSlotCnt      ;
    double dCsSlotPitch    ;

    double dWf_PitchX      ;
    double dWf_PitchY      ;
    double dWf_BldOfs      ;
    double dWf_Diameter    ;

    int    iMapType        ;
    int    iWColCnt        ;
    int    iWRowCnt        ;
    int    iWFrstRow       ;
    int    iWFrstCol       ;

    int    iFlatAngle      ;

    int    iWXOffset       ;
    int    iWYOffset       ;

    String sGoodStr1       ;
    String sEmptStr1       ;
    int    iStrSttRow      ;
    int    iStrSttCol      ;



} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    int    iLotEndMgzCnt   ;
    int    iEpxEjtOnTime   ; //Dispenser Before
    int    iEpxEjtOffTime  ; //Dispenser After

    double dWRowPitch      ; //�̰� �Ⱦ�. ���߿� ����.
    double dWColPitch      ;

    double dVoiceCoilStrip ;
    double dVoiceCoilStrip1;
    double dVoiceCoilMove  ;
    double dVoiceCoilWafer ;
    double dVoiceCoilWafer1;

    int    iEpxClenCnt     ; //������ ī��Ʈ �̰� �̻��Ͻ� Ŭ��.
    int    iEpxCleanDly    ;
    int    iEpxCleanShotCnt; //������ �ϵ� û�ҽÿ� �� Ƚ��.

    int    iEpxTime        ;



    //HED
    int    iVacuumOnDly    ;
    int    iVacuumOffDly   ;

    //Ejector
    int    iEjtSuckOnDly   ;

    //Regrip asdfkljasg
    int    iPRBRgCol[MAX_REGRIP_ARRAY]       ;
    int    iPRBRgDis[MAX_REGRIP_ARRAY]       ;
    int    iWRERgCol[MAX_REGRIP_ARRAY]       ;
    int    iWRERgDis[MAX_REGRIP_ARRAY]       ;
    int    iWRDRgCol[MAX_REGRIP_ARRAY]       ;
    int    iWRDRgDis[MAX_REGRIP_ARRAY]       ;
    int    iPSBRgCol[MAX_REGRIP_ARRAY]       ;
    int    iPSBRgDis[MAX_REGRIP_ARRAY]       ;

    int    iPRB_XRgCnt     ;
    int    iWRE_XRgCnt     ;
    int    iWRD_XRgCnt     ;
    int    iPSB_XRgCnt     ;

    int    iPRB_XWkCon     ;
    int    iWRE_XWkCon     ;
    int    iWRD_XWkCon     ;
    int    iPSB_XWkCon     ;


    int    iLdrOrder       ;
    int    iUdrOrder       ;
    double dDFMDiameter    ;
    double dDFMRadius      ;
    double dDFMWidth       ;
    double dDFMHeight      ;
    double dDFMSttOfsX     ;
    double dDFMSttOfsY     ;

    double dPickWaitAc     ;
    double dPickWaitDc     ;
    double dPickWaitVel    ;

    double dPlaceWaitAc    ;
    double dPlaceWaitDc    ;
    double dPlaceWaitVel   ;

    int    iEjectOnDly     ;

    int    iEjectorBfDnDly ;

    //Vision
    int    iOriVsFailCnt   ; //���� ���� ī��Ʈ.
    int    iOriVsBfDly     ; //���� �˻��� ������ . ���� ����ȭ ����.
    int    iOriVsGrabTO    ; //�׷� Ÿ�Ӿƿ�.
    int    iOriVsInspTO    ; //�ν���� Ÿ�Ӿƿ�.

    int    iWfrVsFailCnt   ;
    int    iWfrVsBfDly     ; //Inlet ���� �˻��� ������ . ���� ����ȭ ����.
    int    iWfrVsGrabTO    ;
    int    iWfrVsInspTO    ;

    int    iRalBfVsFailCnt   ;
    int    iRalBfVsBfDly     ; //Inlet ���� �˻��� ������ . ���� ����ȭ ����.
    int    iRalBfVsGrabTO    ;
    int    iRalBfVsInspTO    ;

    int    iRalAtVsFailCnt   ;
    int    iRalAtVsBfDly     ; //Inlet ���� �˻��� ������ . ���� ����ȭ ����.
    int    iRalAtVsGrabTO    ;
    int    iRalAtVsInspTO    ;

    int    iOriMathod        ; //��Ī �ɼ�.   EN_ORI_MATHOD
    bool   bOperChckAtMatch  ; //���۰� ��Ī�� äũ �ϰ�

    int    iWfFullRctCntX    ; //������ �簢���� ����. �׻� ���� �������� ���ų� ����.
    int    iWfFullRctCntY    ;
    int    iWfFullRctMapOfsX ; //�Ϻ��� ù ��° �簢������ �ʻ��� ø��° �簢�������� �����V. �׻� 0���� ũ�ų� ����.
    int    iWfFullRctMapOfsY ;

    int    iSecondFindPos    ; //�ι�° �� ã���� ������ Ĩ ��ġ...      

    int    iLdrCanHomePrbCnt ; //�δ� Ȩ�� ���� �� �ִ� �������� �۾� ���� ����.

} ;

//Common Option.
struct CCmnOptn {   //��� ���� �ɼ�.
    bool   bDoorSkip             ; //
    bool   bLoadingStop          ; //Loading Stop

    bool   bEpoxyStop            ; //Epoxy Stop

    bool   bIgnrOriVs            ; //Inlet ���� �˻� ����.
    bool   bIgnrRalAtVs          ; //
    bool   bIgnrVcSsr            ; //
    bool   bUseStgTheta          ; //�������� T���� ���.

    bool   bIgnrRalBfVsEpoxyFail ;

    bool   bUseMapFile           ; //��ȭ�� ��� ����.
    bool   bInkDieWork           ; //��ũ���̸� ��� �۾��ϴ� ���.(no map mode������ ��� ����)

    String sSourcePath           ;
    String sWorkedPath           ;

    bool   bUseBtAlign           ;
    bool   bIgnrHedOverload      ;

    bool   bUseHeadTWfr          ; //�ص� ��Ÿ ���� ���.
    bool   bUseHeadTRal          ; //�ص� ��Ÿ ���� ���.


    double dStageTLmtErr         ; //�������� ���� ���� ����.
    double dHedTLmtErr           ;

    int    iEjctrDnAfterDelay    ; //������ �� �ٿ� �ϰ� ��ٸ��� �ð�.
    int    iEjctrUpAfterDelay    ; //������ �� ��  �ϰ� ��ٸ��� �ð�. �ϴ� ��Ũ �µ����̰� �־ ����� �Ⱦ�.

    bool   bWfrVisnRetest        ; //������ ���� �ι� �˻�.
    bool   bStageStop            ; //�������� Stop

    int    iLotEndChipCnt        ;

    bool   bUseVcCoil            ; //���̽� ���� ��뿩��.





//    int    iStgThetaMin  ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool       bDebugMode       ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    bool       bIgnrEpoxy       ;
    double     dEpxOriSpd       ;
    double     dEpxOriOfs       ;
    bool       bIgnrDieAttach   ;

    double     dVoiceCoilOffset ;
    double     dVoiceCoilA      ; //���̽� ���� �Է°�(g)�� V�� ��ȯ�� �ִ� ȯ��� A  y=ax+b
    double     dVoiceCoilB      ; //���̽� ���� �Է°�(g)�� V�� ��ȯ�� �ִ� ȯ��� B

    double     d1stDis          ;
    double     d2stDis          ;
    double     d3stDis          ;

    double     dStgXEjtCt       ;
    double     dStgYEjtCt       ;

    bool       bIgnrBfVision    ;
    bool       bIgnrWfVision    ;
    double     dEjtDia          ; //�浹 ������. ������ �� ����.
    double     dStgDia          ; //�浹 ������. ������ ����.

    double     dStgYMaxExdAble  ;
    String     sDeviceBackUpPath;

    double     dToolCrashWfrY   ; //�������� �浹 ����.
    double     dToolCrashRalY   ; //������ �浹 ����.
    double     dToolCrashZ      ;

    bool       bPickUpTestMode  ; //�÷��̽� �Ű� �Ⱦ��� �Ⱦ��� �ϰ� ������.
    bool       bEjectorDnFirst  ; //���Ҷ� ������ ���� ���� ���� ���� �ö󰡴� �ɼ�.

    bool       bDryRun          ; //��� ������ �� ���� ��� ���� ����� ���� �ϴ� �ɼ�. ������ ������ �̹��� ���μ����� ���Ѵ�.

    double     dRegripOfs       ; //�׸��۵� ���׸��ÿ� �����V ����.




} ;

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName      ; //sModelName �� �̸� ���÷��̿� �̴�.
    int        iTotalChip      ;
    int        iTotalFailChip  ;
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
