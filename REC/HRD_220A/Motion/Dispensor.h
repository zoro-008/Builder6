//---------------------------------------------------------------------------

#ifndef DispensorH
#define DispensorH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
#include "Rs232Dispr.h"
//#include "Rs232Keyence.h"


//---------------------------------------------------------------------------
class CDispensor : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CDispensor ();
        ~CDispensor ();

        void Init();
        void Close();

    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle    =  0 ,
            scVisnBf       ,
            scDisp         ,
            scVisnAt       ,
            scClean        ,

            scManStdCal    ,
            scManSlnChange ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void InitCycleName(){
            m_sCycleName[scIdle        ]="Idle"         ;
            m_sCycleName[scVisnBf      ]="VisnBf"       ;
            m_sCycleName[scDisp        ]="Disp"         ;
            m_sCycleName[scVisnAt      ]="VisnAt"       ;
            m_sCycleName[scClean       ]="Clean"        ;

            m_sCycleName[scManStdCal   ]="ManStdCal"    ;
            m_sCycleName[scManSlnChange]="ManSlnChange" ;
        }
        void   InitCycleTime(){
            memset(&m_iCycleTime  , 0 , sizeof(int )*MAX_SEQ_CYCLE);
        }

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;
            EN_SEQ_CYCLE iLastSeq ;
        };

    protected:
        AnsiString m_sPartName  ;

        CCycleTimer m_tmDispr   ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmDelay   ;

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        int    m_iReptCnt    ;
        double m_dDisprTime  ; 

        bool   m_bBfVisn  ;
        double m_dBfVisnX , m_dBfVisnY , m_dBfVisnT ;
        double m_dPkgCntX , m_dPkgCntY , m_dPkgCntT ;


        bool   m_bAtVisn  ;
        double m_dAtVisnX , m_dAtVisnY , m_dAtVisnT ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        bool   FindChip   (int &c , EN_ARAY_ID &id);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        int    m_iDisprBoatCnt;

        bool   m_bOneSkipVisnAt;



    public: //�������̽� ���.==================================================
	void Reset (); //���V ��ư �������� Ÿ�� �Լ�.

        int  GetReptCnt();
        //Running Functions.
        void Update    ();
        bool ToStopCon (); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (); //��ž�� �ϱ� ���� �Լ�.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStep        GetStep        (){return Step          ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool         Autorun        (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:
        double GetDisprTime();
        void   SetDisprTime(double _dDisprTime);

        void   AddDisprBoatCnt();

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleVisnBf();
        bool CycleDisp  ();
        bool CycleVisnAt();
        bool CycleClean ();

        //Manual Cycle.
        bool CycleManStdCal();
        bool CycleManNdlCheck();
        bool CycleManVisnPos ();
        bool CycleManSlnChange();

        bool ToolVoid(); //���漭 ���Ҷ� �����ֱ�.

        //bool CycleManClean ();
        
        CRs232Dispr   Rs232_Dispr;
        //CRs232Keyence Rs232_Hght ;


        bool GetVisnRslt(double * _dVisnX , double * _dVisnY , double * _dVisnT , bool _bAt = false);

        //�޴���. ���߿� ������.
        //bool CycleManAlignPlace(); //��Ŀ�� �ִ°��� ����̳ʿ� ���� ���´�.
        //bool CycleManPickVisnPlace(); //����ο��� �� �����˻��ϰ� �ٽ� ���� ���´�.
        //bool CycleManPickerCheck(); //�ƹ��͵� ��� ���� ���� ���� ��ġ���� ���� ��û.


        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        //bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //�ù� ���ⱸ�� �Ϸ��� ����ũ�� ��ġ ���ƾ� ���� ;;
        bool MoveMotrSync(EN_MOTR_ID _iMotr1 , EN_MOTR_ID _iMotr2 , double dMotr1Pos , double dMotr2Pos , double _dSpeed , double _dAcc , double _dDcc);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        //�׳� DM.���� �Ǵ� �ϸ� �ɵ�.
        //bool   m_bNeedWorkAfterDisp ; //����� ������ ������ ��ž ��ư ���� ���¿��� �۾��� ���� �ؾ� �ϴ°��.
                                      //��Ŀ���� �̰� ���� ���䷱���� RqstStop�λ�Ȳ���� ���䷱�� ���������.
                                      //������ UpdateButton���� ����ž Ÿ�Ӿƿ� �÷�����.

        bool   m_bRqstPMSave ; //���ٴٵ� Ķ��... Ķ�۾��� ������ ������ �ʿ��Ѱ��. true��Ű��
                               //�� Ÿ�̸ӿ��� Ȯ�� �ϰ� UI���� ���� �Ѵ�.
        bool   m_bVsnBfErrDisp; //���� �� ����
        bool   m_bVsnAtErrDisp; //���� �� ����

        int    m_iCrntDispNode;

        bool   m_bNiddleChecked;//�������ϸ� �ѹ��� �ϵ� üũ�� �����ؾ� �Ѵ�.

};

class CSetInterpolation {
    public :
        int iMotorAixs[2] ; // ���� � ���� �� ����
        double dCenPos[2] ; // ��ȣ ���� � �ϴ� CircleCenterMove �Լ��� ȣ���� �� ���
        double dEndPos[2] ; // ���� �� ��ȣ ���� � �ϴ� �Լ����� ȣ���� �� ���

        int iCoord ; // ��ǥ�� ����
        int iAngle ; // �ڵ� ������ ��带 ������ ���, 0 ~ 360��
        int iAxisCnt ; // ���� ��� ���� ���� ����

        DWORD dwAbsRelMode ; // ���� ��� ��, ���� = 0, ��� ��� = 1 �� ����
        DWORD dwProfileset ; // ������ ��带 ����, �Ϲ� = 1, ��� ������ ��� = 2, �ڵ� ������ ��� = 3
        DWORD dwCWDir ; // ��ȣ ���� ��� ��, ��ȣ�� �׸��� ���� ����, �ݽð� ���� = 0, �ð� ���� = 1

        double dVel ; // ���� ��� ��, �⺻ ���� �ӵ��� ����
        double dAcc ; // ���� ��� ��, ���� ���ӵ��� ����
        double dDec ; // ���� ��� ��, ���� ���ӵ��� ����
};


//---------------------------------------------------------------------------
extern CDispensor DSP;
//---------------------------------------------------------------------------
#endif
