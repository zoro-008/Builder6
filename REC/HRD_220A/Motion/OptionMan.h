//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
enum EN_WORK_MODE { wmFos = 0 , wmDie = 1};
struct CDevInfo {   //device�� ���� Dev_Info
    EN_WORK_MODE iWorkMode        ;

    double       dMgzSlotPitch    ; //Loader Tray
    int          iMgzSlotCnt      ;
    double       dMgzTopToFrstSlot; //���� 21.0mm ����.  ��ŸƮ ������ 2���� ��.
    double       dMgzBtmToLastSlot; //���� 13.0mm ����.
    int          iBoatColCnt      ; // 10��.
    double       dBoatColPitch    ; // 41.6mm
    double       dFosWidth        ;
    double       dFosHeight       ;
    double       dFosChamper      ;
    double       dHicWidth        ;
    double       dHicHeight       ;
    double       dHicChamper      ;
    double       dDieWidth        ;
    double       dDieHeight       ;
    double       dDieChamper      ;
} ;
                 
//Device Option.
struct CDevOptn {   //Device�� ���� �ٲ�� �ɼ�.
    bool         bStIgnrVisnBefore ;
    bool         bStIgnrVisnAfter  ;
    bool         bPkIgnrAlign      ;
    bool         bPkIgnrVisnAlign  ;
    bool         bPkIgnrVisnDefect ;

    int          iPkPickDelay      ; //Pick Delay.
    int          iFrstPkgDelay     ; //���� ù������ �� �����̰� �־�� �Ѵ�.

    double       dPlaceYOfs        ; //������ ���߾ӿ� ���� ���°��� �ƴϰ� Y������ �����V�� �ִ�.
    double       dPlaceXOfs        ; //X�����V�� �ִ�.

    //�̳���� �Ѿ�� �����ϸ� �ⱸ ó���� ���� ����.
    double       dStageTolXY       ; //Stage �˻�ÿ� �˻� ��� ġ��.
    double       dStageTolAng      ; //����.
    double       dPickerTolXY      ; // �˻�ÿ� �˻� ��� ġ��.
    double       dPickerTolAng     ; //����.
    double       dPlaceTolXY       ; //After �˻�ÿ� �˻� ��� ġ��.
    double       dPlaceTolAng      ; //����.

    double       dFosPlaceSttYOfs  ; //Fos�� ��� ������ Y���� �����V�� ������ ������ ������Ŀ� ���� Place��ġ�� ��������.
    double       dFosPlaceSttYSpd  ; //Fos�� ��� ������ Y���� �����V�� ������ ������ ������Ŀ� ���� Place��ġ�� ��������. �ӵ�.
    int          iFosPlaceBfDelay  ; //Fos�� ����� �ڿ�Ȯ���� ���� ������ ������ ��ٸ���.

    double       dPlaceZOfs1       ; //FOS�� ��� Dispensing�ÿ� �ణ���� ���� �����Ѵ�. �� �����V�̰�.  �÷��̽��ÿ� 1������ �����V �������� ���ٰ�.
    double       dPlaceZOfs2       ; // 2������ �����V ���� ���� ���� ���� Attach���� ����.
    double       dAttachSpeed1     ; //Z�� dFosPlaceZOfs1~dFosPlaceZOfs2����.
    double       dAttachSpeed2     ; //Z�� dFosPlaceZOfs2~Attach����.
    double       dAttachForce      ; //���� ����.g����.
    double       dAttachForceOfs   ; //���� ����.g����. �����V.

    double       dDispZOfs         ; //����� ���� �����V.
    int          iPkPushDelay      ;

    double       dAttachStageOfsX  ; //���� �Ҷ��� �������� �̵� �����V...
    double       dAttachStageOfsY  ; // ��ź�� ������ �ƹ��� �ص� ��������� �����V�� �߻� �ǹǷ� �ִ´�.
    double       dAttachStageOfsT  ; // ��ź�� ������ �ƹ��� �ص� ��������� �����V�� �߻� �ǹǷ� �ִ´�.

    int          iRailBtmTemp      ;
    int          iRailTopTemp      ;
    int          iStageTemp        ;

    //int          iDspVacPres       ; //����� ���漭 ��Ũ.(���� 0.01kp)
    //int          iDspPrsPres       ; //����� ���漭 ����.(���� 0.1kp)

    int          iDspCh            ; //���漭 ��� ä��.
    int          iDspMinAmount     ; //���漭 �ܾ� ���� �ۼ�Ʈ...(20 �̸� 20���ϳ������� ����.)
    double       dDspVacPres       ; //����� ���漭 ��Ũ.(���� 0.01kp)
    double       dDspPrsPres       ; //����� ���漭 ����.(���� 0.1kp)


    double       dReptSttXPos      ;
    double       dReptSttYPos      ;
    double       dReptEndXPos      ;
    double       dReptEndYPos      ;
    double       dReptSpd          ;
    int          iReptCnt          ;

    AnsiString   sEpoxyName        ;

} ;


struct CCmnOptn {   //��� ���� �ɼ�.
    bool    bLoadingStop      ;
    bool    bIgnrDoor         ;

    int     iDisprClnCnt      ;
    bool    bVisnNgStop       ;

    bool    bTempIgnrErrorChk ;
    double  dTempErrorTol     ;

    String  sLotFilePath      ;

    double  dMaxPlaceZOfs1    ;
    double  dMaxPlaceZOfs2    ;
    double  dMaxAttachForce   ;
} ;

//Master Option.
struct CMstOptn {   //����Ÿ �ɼ�. ȭ��� ���� FrmMain ����̽� �� ���� Ŭ��.
    bool   bDebugMode         ; //true = �αױ�ɰ�ȭ, ������ Ÿ�Ӿƿ�����.

    double dPckDspStroke      ; //Picker �� Dispensor���� Stroke ;

    bool   bSkipPickrDispr    ; //Picker �� Dispensor�� ��ŵ.

    double dPckToStgAngleOfs  ; //���������� ��Ŀ���� �ޱ� �����V.
};
//Eqipment Option.
struct CEqpOptn {   //�𵨺� �ϵ���� �ɼ�.  ȭ��� ���� �ؽ�Ʈ ���Ͽ� ����.
    AnsiString sModelName   ;
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


};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
