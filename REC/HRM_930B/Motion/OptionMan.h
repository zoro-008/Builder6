//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
enum EN_WORK_MODE { wmNormal = 0 , wmHeight = 1}; //0:���� �۾� 1:�δ����� ������ �������Ϳ��� ���� ������ �ϰ� �ٽ� �ִ´�.
enum EN_DISP_MODE { dmBoth = 0 , dmFrst = 1 , dmScnd = 2 , dmNotUse = 3 }; //������� �ϴ� ���� 0:�ڿ���ȭ&������ȭ 1: �ڿ���ȭ , 2:������ȭ , 3 = ����.}


struct CDevInfo {   //device�� ���� Dev_Info
    double       dLDRFMgzSlotPitch; //Loader Tray
    int          iLDRFMgzSlotCnt  ;
    int          iLDRFMgzBayCnt   ;
    double       dLDRRMgzSlotPitch; //Loader Tray
    int          iLDRRMgzSlotCnt  ;
    int          iLDRRMgzBayCnt   ;
    double       dULDMgzSlotPitch ; //Loader Tray
    int          iULDMgzSlotCnt   ;
    int          iULDMgzBayCnt    ;

//   double        dMidWidth        ;
//    double       dMidHeight       ;
    int          iMidColCnt       ; // 10��.
    int          iMidRowCnt       ; // 10��.

//    double       dCmsWidth        ;
//    double       dCmsHeight       ;
//    double       dPcbWidth        ;
//    double       dPcbHeight       ;
} ;

//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.



    double       dPlaceZOfs        ; //�̵���� CmosPCB�� �����ؼ� ���� ���� �ʴµ� �װſ� ���� �����V.
    double       dAttachSpeed1     ; //Z�� dFosPlaceZOfs1~dFosPlaceZOfs2����.
    double       dAttachSpeed2     ; //Z�� dFosPlaceZOfs2~Attach����.
    double       dAttachForce      ; //���� ����.g����.
    double       dAttachForceOfs   ; //���� ����.g����. �����V.
    double       dAttachDelay      ; //���� �� ������.

    int          iFixDelay         ; //������ �ڿ���ȭ ������

    double       dDispZOfs         ; //����� ���� �����V.

    double       dStageInspOfsX    ; //���� �Ҷ��� �������� �̵� �����V X,Y�� �����V�� ������ ���� ���� ���� �� �����˻��ϴ� �������̴�.
    double       dStageInspOfsY    ;

    int          iDspChFt          ; //���漭 ��� ä��.
    int          iDspChRr          ; //���漭 ��� ä��.

    int          iDspMinAmount     ; //���漭 �ܾ� ���� �ۼ�Ʈ...(20 �̸� 20���ϳ������� ����.)

    AnsiString   sEpoxyName        ;

    //29�߰�.
    double       dMidCmsGapFrst    ; //ù��° ���� �̵���� �ø𽺰��� ��.
    double       dMidCmsGap        ; //�̵���� �ø𽺰��� ��.

    //bool         bEpoxyFirst       ; // ������ ���� �۾� �ϴ¿ɼ�.
    bool         bUsePush          ; // Cmos ������ ���� ��� ����.
    //bool         bUseDispRear      ; //�̰� üũ�Ǿ� ������ Rear���漭 ��� �ȵǾ� ������ ����Ʈ���.
    double       dDispAccDcc       ; //����� ������.

    int          iUVTopBrightness  ; // Ch5      ��� UV ��� ����.
    int          iUVBtmBrightness1 ; // Ch1
    int          iUVBtmBrightness2 ; // Ch2
    int          iUVBtmBrightness3 ; // Ch3
    int          iUVBtmBrightness4 ; // Ch4
    bool         bUseBtmUV         ; // �Ϻ� UV ��� ���� üũ

    double       dLVisnTolXY       ; //���밪 Stage �˻�ÿ� �˻� ��� ġ��.
    double       dLVisnTolAng      ; //���밪 ����.
    double       dRVisnTolXY       ; //���밪 Stage �˻�ÿ� �˻� ��� ġ��.
    double       dRVisnTolAng      ; //���밪 ����.
    double       dREndVisnTolXY    ; //������ ��밪 ������ ���̰� ���� �˻��ϴ� ����.
    double       dREndVisnTolAng   ; //������ ��밪 ������ ���̰� ���� �˻��ϴ� ����.
    double       dRHgtTolZ         ; //���� ���� �ϰ� ���� ������ �����Ͽ� Ȯ����.

    double       dCmsAtachGapX     ; //Cmos���� Attach ����.. (ex 0.030mm)
    double       dCmsAtachGapY     ; //

    double       dActiveEdgeGapX   ; //2�����ո��� ��� �����ո��� ġ�� �˻縦 �ؼ� �����Ҷ� �浹 ������ �ϴµ� ��Ƽ�� ������ �������� �������� ������
    double       dActiveEdgeGapY   ; //���� �����鿡�� ��Ƽ�꿡���� ������ ġ���� �־�� ��. �׷��� �浹 ������.







    int          iLineHeightCnt    ; // ���պ� ���� �����ÿ� �������� ����.

    int          iEpoxyDispCntF    ; //�Ǹ��� 1ea�� ��� ������ �ִ� ��
    int          iEpoxyDispCntR    ; //�Ǹ��� 1ea�� ��� ������ �ִ� ��

    bool         bUseSTG_Fix1      ; //�������� ���ε� �Ǹ��� ��뿩��.
    bool         bUseSTG_Fix2      ; //�������� ���ε� �Ǹ��� ��뿩��.
    bool         bUseSTG_Fix3      ; //�������� ���ε� �Ǹ��� ��뿩��.
    bool         bUseSTG_Fix4      ; //�������� ���ε� �Ǹ��� ��뿩��.
    bool         bUseSTG_Fix5      ; //�������� ���ε� �Ǹ��� ��뿩��.

    double       dDspVacPres       ; //����� ���漭 ��Ũ.(���� 0.01kp)
    double       dDspPrsPres       ; //����� ���漭 ����.(���� 0.1kp)

    bool         bNotUseRotate     ; //C2MP������ �ȵ����� ���δ�.

    int          iUvCntFrst        ; //�ڿ���ȭ UV ������ Ƚ��
    int          iUvCntScnd        ; //������ȭ UV ������ Ƚ��

    double       dSlaveHtOfsZ      ; //���϶� �����̺����� Z�� �����V�� �ش�.

    double       dHexaTopHeightOfs ; //���� ��� ��Ʈ ž���� ���� �ִ� ����� �־....

    int          iHeightMethod     ; //MIN, MAX, AVERAGE
    double       dHeightPitchX     ; //���� ���ݻ�� ���� ������ �����Ͽ� ���� ���� ������ ����Ѵ�.
    double       dHeightPitchY     ; //���� ���ݻ�� ���� ������ �����Ͽ� ���� ���� ������ ����Ѵ�.
    int          iHeightStepCnt    ; //��� ��������.

    bool         bUseSTG_Vac1      ; //�������� ��Ũ1 ���
    bool         bUseSTG_Vac2      ; //�������� ��Ũ2
    bool         bUseSTG_Vac3      ; //�������� ��Ũ3

    bool         bUseSTG_Vac1Frst  ; //�������� ��Ũ1 ���
    bool         bUseSTG_Vac2Frst  ; //�������� ��Ũ2
    bool         bUseSTG_Vac3Frst  ; //�������� ��Ũ3




    double       dBfUVOfsX00       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.
    double       dBfUVOfsX10       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.
    double       dBfUVOfsX01       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.
    double       dBfUVOfsX11       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.

    double       dBfUVOfsY00       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.
    double       dBfUVOfsY10       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.
    double       dBfUVOfsY01       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.
    double       dBfUVOfsY11       ; //������ ��ȭ�� Ʋ������ �� ���� �����V.

    double       dUVTestSpeed      ; //UV Test �� �̵� �ӵ�.
    double       dUVTestDist       ; //UV Test �� �̵� �Ÿ�.

    double       dMBStdHght        ; //�̵�� ���� ����.
    double       dMBHghtTol        ; //�̵�� �������� ������.
    double       dHexaTopStdHght   ; //�����Ʈ ž�� ���� ����.
    double       dHexaTopHghtTol   ; //�����Ʈ ž�� �������� ������.
    double       dCmsHeightOfs     ; //CMOS ���� ��忡�� ����Ʈ ���÷��� �ϴµ� �����ϴ� �����V

    bool         bUseFstDisp       ; //����� 1 ��� ���� üũ (�ڿ���ȭ)
    bool         bUseSecDisp       ; //����� 2 ��� ���� üũ (������ȭ)

    int          iFstDisp          ; //����� 1�� ����� �� (Front/Rear)
    int          iSecDisp          ; //����� 2�� ����� �� (Front/Rear)

    double       dLengthTol        ; //2018.08.08
                                     //CMOS ���� ���� Vision �˻� �� ����� ���̰�
                                     //���� ���� ������ ���̺��� ���ϰ� ũ�ų� ������ ���� ��쵵�� �ϱ����� �ɼ�
                                     //���������� ���� - Vision �˻� ��� ���� < dLengthTol ||
                                     //���������� ���� - Vision �˻� ��� ���� > dLengthTol
                                     //NG
    //�������� ȥ�� ���� ���� ��ŵ�ϵ��� �ɼ� ó�� ����
    bool         bStgMxErrSkip     ;

    EN_DISP_MODE GetDispMode(){
        if( bUseFstDisp && bUseSecDisp) return dmBoth ;
        if( bUseFstDisp &&!bUseSecDisp) return dmFrst ;
        if(!bUseFstDisp && bUseSecDisp) return dmScnd ;
        return dmNotUse ;
    }


} ;


struct CCmnOptn {   //��� ���� �ɼ�.
    bool    bFLoadingStop     ;
    bool    bRLoadingStop     ;
    bool    bIgnrDoor         ;
    bool    bVisnNgStop       ;

    int     iDisprClnTime     ;
    String  sLotFilePath      ;

    double  dMaxPlaceZOfs1    ;
    double  dMaxPlaceZOfs2    ;
    //double  dMaxAttachForce   ;

    int     iRightVisnDelay   ;
    int     iRightHeightDelay ;
    int     iRightHeightStepDelay;

    int     iHexaPotRelDelay  ;

    bool    bUseConfocal      ; //������ ���� ��� ����

    bool    bIgnrCmsREndAlign ; //CMOS Right End Align ��� ����
    bool    bIgnrCmsREndHeight; //CMOS Right End Height ��� ����
    bool    bIgnrCmsRFixAlign ; //CMOS Right Fix Align ��� ����
    bool    bIgnrCmsRFixHeight; //CMOS Right Fix Height ��� ����
    bool    bIgnrMidREndHeight; //CMOS End Height Check ��� ����

    int     iTopUVLimitTime   ; //UV ������ Ÿ�� ����.
    int     iBtmUVLimitTime   ;

    bool    bCheckVisnPos     ; //�����˻��� ��Ǻ������� �ѹ��� ��ž�ϸ鼭 �����â ����.

    EN_WORK_MODE iWorkMode    ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool   bDebugMode           ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.
    //29�߰�
    double dLTL_RTL_Stroke      ; //����Ʈ ����Ʈ ���� ��Ʈ��.
    double dLTL_VISN_DISP_Strke ; //����Ʈ ������ ������ ���漭 ��Ʈ��.
    double dRTL_TRNS_VISN_Strke ; //����Ʈ ������ Ʈ�����ۿ� ���� ��Ʈ��.

    bool   bDonUseMasterT       ; //�������� T�� ���� ���� ����.

    bool   bUseLotInfo          ;

};

//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName   ;
} ;


//���� ����.
struct CEqpStat {
    //bool bShowVisnMsg ; //�����˻��ϰ� ��� �̵����� �޼��� Ȯ�� �ϴ� ��.
}   ;


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
        CEqpStat   EqpStat  ;


        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (AnsiString _sName);

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
